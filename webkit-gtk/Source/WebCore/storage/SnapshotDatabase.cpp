#include "config.h"
#include "SnapshotDatabase.h"

#include "BitmapImage.h"
#include "CachedImage.h"
#include "Chrome.h"
#include "DatabaseTracker.h"
#include "DocumentLoader.h"
#include "FileSystem.h"
#include "HTMLSnapshotImageElement.h"
#include "IntSize.h"
#include "Logging.h"
#include "Page.h"
#include "SharedBuffer.h"
#include "SQLiteStatement.h"
#include "SQLiteTransaction.h"

#include <wtf/CurrentTime.h>
#include <wtf/MainThread.h>
#include <wtf/StdLibExtras.h>
#include <wtf/gobject/GOwnPtr.h>
#include <wtf/text/CString.h>

#if USE(CAIRO)
#include "RefPtrCairo.h"
#include <cairo.h>
#endif

// For methods that are meant to support API from the main thread - should not be called internally
#define ASSERT_NOT_SYNC_THREAD() ASSERT(!m_syncThreadRunning || !IS_SNAPSHOT_SYNC_THREAD())

// For methods that are meant to support the sync thread ONLY
#define IS_SNAPSHOT_SYNC_THREAD() (m_syncThread == currentThread())
#define ASSERT_SNAPSHOT_SYNC_THREAD() ASSERT(IS_SNAPSHOT_SYNC_THREAD())

namespace WebCore {

// This version number is in the DB and marks the current generation of the schema
// Currently, a mismatched schema causes the DB to be wiped and reset.  This isn't
// so bad during development but in the future, we would need to write a conversion
// function to advance older released schemas to "current"
static const int currentDatabaseVersion = 1;

static const int updateTimerDelay = 1;

static bool checkIntegrityOnOpen = false;

#if !LOG_DISABLED
static String urlForLogging(const String& url)
{
    static unsigned urlTruncationLength = 120;

    if (url.length() < urlTruncationLength)
        return url;
    return url.substring(0, urlTruncationLength) + "...";
}
#endif

SnapshotPageInfo::SnapshotPageInfo(const String& url)
    : m_url(url)
    , m_pageID(url.toUInt64Strict())
    , m_chrome(0)
    , m_stamp(0)
    , m_data(0)
    , m_dataSize(0)
    , m_retainCount(0)
    , m_image(0)
{
}

SnapshotPageInfo::~SnapshotPageInfo()
{
    ASSERT(!m_retaintCount);
    if (m_data)
        fastFree(m_data);
}

void SnapshotPageInfo::setImageData(void* data, int size)
{
    if (m_data)
        fastFree(m_data);

    m_data = data;
    m_dataSize = size;
}

void SnapshotPageInfo::setCachedImageData(Vector<char>& data)
{
    m_image->data(SharedBuffer::adoptVector(data), true);
}

void SnapshotPageInfo::setCachedImageData(NativeImagePtr image)
{
    if (image) {
        m_image->setImage(BitmapImage::create(image));
#if USE(CAIRO)
        cairo_surface_reference(image);
#endif
    } else
        m_image->setImage(adoptRef(static_cast<BitmapImage*>(0)));
}

void SnapshotPageInfo::clearImage() {
    CachedImage* nullImage = 0;
    m_image = nullImage;
}

bool SnapshotPageInfo::retain(HTMLSnapshotImageElement* element)
{
    if (!m_image) {
        m_image = new CachedImage(0);
        if (!m_image)
            return true;
        m_image->setLoading(true);
        if (m_pageID)
            m_chrome = element->document()->page()->chrome();
    }

    element->setImage(m_image.get());
    m_image->addClient(element);

    if (!m_pageID)
        return m_retainCount++;
    else {
        ++m_retainCount;
        m_image->setLoading(true);
        return false;
    }
}

bool SnapshotPageInfo::release(HTMLSnapshotImageElement* element)
{
    ASSERT(m_retainCount > 0 && m_image);
    if (m_image)
        m_image->removeClient(element);
    element->setImage(0);
    return --m_retainCount;
}


// ************************
// *** Main Thread Only ***
// ************************

SnapshotDatabase& SnapshotDatabase::getDB()
{
    DEFINE_STATIC_LOCAL(SnapshotDatabase, db, ());
    return db;
}

SnapshotDatabase::SnapshotDatabase()
    : m_syncTimer(this, &SnapshotDatabase::syncTimerFired)
    , m_syncThreadRunning(false)
    , m_privateBrowsingEnabled(false)
    , m_threadTerminationRequested(false)
    , m_removeSnapshotsRequested(false)
    , m_scaleFactor(0)
    , m_maximumSize(0)
    , m_context(0)
{
    LOG(SnapshotDatabase, "Creating SnapshotDatabase %p", this);
    ASSERT(isMainThread());
}

SnapshotDatabase::~SnapshotDatabase()
{
    ASSERT_NOT_REACHED();
}

bool SnapshotDatabase::open(const double scaleFactor, const int maximumSize, void* context)
{
    if (isOpen())
        return false;

    m_databaseDirectory = DatabaseTracker::tracker().databaseDirectoryPath();

    m_scaleFactor = scaleFactor;
    m_maximumSize = maximumSize;
    m_context = context;

    // Formulate the full path for the database file
    m_completeDatabasePath = pathByAppendingComponent(m_databaseDirectory, String::fromUTF8("WebpageSnapshots.db"));

    // Lock here as well as first thing in the thread so the thread doesn't actually commence until the createThread() call
    // completes and m_syncThreadRunning is properly set
    m_syncLock.lock();
    m_syncThread = createThread(SnapshotDatabase::snapshotDatabaseSyncThreadStart, this, "WebCore: SnapshotDatabase");
    m_syncThreadRunning = m_syncThread;
    m_syncLock.unlock();
    if (!m_syncThread)
        return false;

    return true;
}

void SnapshotDatabase::close()
{
    ASSERT_NOT_SYNC_THREAD();

    if (m_syncThreadRunning) {
        // Set the flag to tell the sync thread to wrap it up
        m_threadTerminationRequested = true;

        // Wake up the sync thread if it's waiting
        wakeSyncThread();

        // Wait for the sync thread to terminate
        waitForThreadCompletion(m_syncThread, 0);
    }

    m_syncThreadRunning = false;
    m_threadTerminationRequested = false;
    m_removeSnapshotsRequested = false;

    m_syncDB.close();
    ASSERT(!isOpen());
}

void SnapshotDatabase::removeAllSnapshots()
{
    ASSERT_NOT_SYNC_THREAD();

    if (!isOpen())
        return;

    LOG(SnapshotDatabase, "Requesting background thread to remove all icons");

    {
        MutexLocker locker(m_urlAndSnapshotLock);

        HashMap<String, SnapshotPageInfo*>::iterator iter = m_pageInfoMap.begin();
        HashMap<String, SnapshotPageInfo*>::iterator end = m_pageInfoMap.end();
        for (; iter != end; ++iter)
            (*iter).second->setImageData(0, 0);

        // Clear all in-memory records of things that need to be synced out to disk
        {
            MutexLocker locker(m_pendingSyncLock);
            m_pendingSync.clear();
        }

        // Clear all in-memory records of things that need to be read in from disk
        {
            MutexLocker locker(m_pendingReadingLock);
            m_pendingReading.clear();
        }
    }

    m_removeSnapshotsRequested = true;
    wakeSyncThread();
}

void SnapshotDatabase::retainSnapshotForURL(const String& url, HTMLSnapshotImageElement* element)
{
    ASSERT_NOT_SYNC_THREAD();

    // Cannot do anything with url that would end up storing it without deep copying first
    if (!isOpen())
        open(0, 0, 0);

    SnapshotPageInfo* pageInfo;
    {
        MutexLocker locker(m_urlAndSnapshotLock);
        pageInfo = getSnapshotPageInfo(url);
    }

    if (pageInfo && !pageInfo->retain(element)) {
        {
            MutexLocker locker(m_pendingReadingLock);
            m_pendingReading.add(pageInfo);
        }

        wakeSyncThread();
    }
}

void SnapshotDatabase::releaseSnapshotForImage(CachedImage* image, HTMLSnapshotImageElement* element)
{
    ASSERT_NOT_SYNC_THREAD();

    // Cannot do anything with url that would end up storing it without deep copying first

    if (!isOpen())
        return;

    // Get its retain count - if it's retained, we'd better have a SnapshotPageInfo for it
    SnapshotPageInfo* pageInfo;

    {
        MutexLocker locker(m_urlAndSnapshotLock);

        HashMap<String, SnapshotPageInfo*>::iterator iter = m_pageInfoMap.begin();
        HashMap<String, SnapshotPageInfo*>::iterator end = m_pageInfoMap.end();
        for (; iter != end; ++iter) {
            if ((*iter).second->image() == image)
                break;
        }
        ASSERT(iter != end);
        if (iter == end)
            return;

        pageInfo = (*iter).second;

        LOG(SnapshotDatabase, "Releasing url %s to a retain count of %i", urlForLogging(pageInfo->url()).ascii().data(), pageInfo->retainCount() - 1);
        ASSERT(pageInfo->retainCount() > 0);

        // If it still has a positive retain count, store the new count and bail
        if (pageInfo->release(element))
            return;

        // This pageInfo has now been fully released. Do the appropriate cleanup
        LOG(SnapshotDatabase, "No more retainers for PageInfo %s", urlForLogging(pageInfo->url()).ascii().data());
        m_pageInfoMap.remove(iter);
    }

    pageInfo->clearImage();

    {
        MutexLocker syncLocker(m_pendingSyncLock);
        MutexLocker readingLocker(m_pendingReadingLock);

        if (!m_pendingReading.contains(pageInfo) && !m_pendingSync.contains(pageInfo))
            delete pageInfo;
    }
}

#if USE(CAIRO)
static cairo_status_t writeToPNGFunction(void* closure, const unsigned char* data, unsigned int length)
{
    Vector<unsigned char>* in = reinterpret_cast<Vector<unsigned char>*>(closure);
    in->append(data, length);
    return CAIRO_STATUS_SUCCESS;
}
#endif

void SnapshotDatabase::setSnapshotDataForURL(const String& url, const NativeImagePtr snapshotData)
{
    ASSERT_NOT_SYNC_THREAD();

    if (!isOpen() || !protocolIsInHTTPFamily(url) || !m_scaleFactor || m_privateBrowsingEnabled)
        return;

    bool needResize = (m_scaleFactor != 1);

#if USE(CAIRO)
    cairo_surface_t* snapshot;
    if (needResize) {
        snapshot = cairo_image_surface_create(cairo_image_surface_get_format(snapshotData),
                                              cairo_image_surface_get_width(snapshotData) * m_scaleFactor,
                                              cairo_image_surface_get_height(snapshotData) * m_scaleFactor);
        RefPtr<cairo_t> cr = adoptRef(cairo_create(snapshot));
        cairo_scale(cr.get(), m_scaleFactor, m_scaleFactor);
        cairo_set_source_surface(cr.get(), snapshotData, 0, 0);
        cairo_set_operator(cr.get(), CAIRO_OPERATOR_SOURCE);
        cairo_paint(cr.get());
    } else
        snapshot = snapshotData;

    Vector<unsigned char> dataVector;
    dataVector.reserveCapacity(1024 * 1024 * m_scaleFactor); // initial capacity is 1 * m_scaleFactor MB 
    cairo_surface_write_to_png_stream(snapshot, writeToPNGFunction, &dataVector);

    if (needResize)
        cairo_surface_destroy(snapshot);

    int dataLength = dataVector.size();
    void* data = dataVector.releaseBuffer();
#else
    return;
#endif

    SnapshotPageInfo* pageInfo;
    {
        MutexLocker locker(m_urlAndSnapshotLock);

        // If this icon was pending a read, remove it from that set because this new data should override what is on disk
        pageInfo = m_pageInfoMap.get(url);
        if (pageInfo) {
            MutexLocker locker(m_pendingReadingLock);
            m_pendingReading.remove(pageInfo);
        } else
            pageInfo = getSnapshotPageInfo(url);
    }

    if (!pageInfo) {
        fastFree(data);
        return;
    }

    // Update the data and set the time stamp
    pageInfo->setImageData(data, dataLength);

    {
        MutexLocker locker(m_pendingSyncLock);
        m_pendingSync.add(pageInfo);
    }

    // Send notification out regarding all PageURLs that retain this icon
    // But not if we're on the sync thread because that implies this mapping
    // comes from the initial import which we don't want notifications for
    if (!IS_SNAPSHOT_SYNC_THREAD()) {
        // Start the timer to commit this change - or further delay the timer if it was already started
        scheduleOrDeferSyncTimer();
    }
}

void SnapshotDatabase::didImportSnapshotData(const String& url, Vector<char>* bitData, NativeImagePtr imageData)
{
    ASSERT_NOT_SYNC_THREAD();

    SnapshotPageInfo* pageInfo;

    {
        MutexLocker locker(m_urlAndSnapshotLock);
        pageInfo = m_pageInfoMap.get(url);
    }

    if (!pageInfo)
        return;

    if (!pageInfo->pageID() && bitData)
        pageInfo->setCachedImageData(*bitData);
    else
        pageInfo->setCachedImageData(imageData);
}

void SnapshotDatabase::setPrivateBrowsingEnabled(bool flag)
{
    m_privateBrowsingEnabled = flag;
}

bool SnapshotDatabase::isPrivateBrowsingEnabled() const
{
    return m_privateBrowsingEnabled;
}

void SnapshotDatabase::checkIntegrityBeforeOpening()
{
    checkIntegrityOnOpen = true;
}

void SnapshotDatabase::wakeSyncThread()
{
    MutexLocker locker(m_syncLock);
    m_syncCondition.signal();
}

void SnapshotDatabase::scheduleOrDeferSyncTimer()
{
    ASSERT_NOT_SYNC_THREAD();
    m_syncTimer.startOneShot(updateTimerDelay);
}

void SnapshotDatabase::syncTimerFired(Timer<SnapshotDatabase>*)
{
    ASSERT_NOT_SYNC_THREAD();
    wakeSyncThread();
}


// ******************
// *** Any Thread ***
// ******************

bool SnapshotDatabase::isOpen() const
{
    MutexLocker locker(m_syncLock);
    return m_syncDB.isOpen();
}

String SnapshotDatabase::databasePath() const
{
    MutexLocker locker(m_syncLock);
    return m_completeDatabasePath.threadsafeCopy();
}

// This method retrieves the existing SnapshotPageInfo, or creates a new one and marks it as "interested in the import" for later notification
SnapshotPageInfo* SnapshotDatabase::getSnapshotPageInfo(const String& url)
{
    // Clients of getSnapshotPageInfo() are required to acquire the m_urlAndSnapshotLock before calling this method
    ASSERT(!m_urlAndSnapshotLock.tryLock());

    String key;
    if (protocolIsInHTTPFamily(url))
        key = KURL(ParsedURLString, url).string();
    else if (url.toUInt64Strict())
        key = url;
    else
        return 0;

    SnapshotPageInfo* pageInfo = m_pageInfoMap.get(key);

    // If the initial import of all URLs hasn't completed and we have no page record, we assume we *might* know about this later and create a record for it
    if (!pageInfo) {
        String crossThreadURL = key.crossThreadString();
        LOG(SnapshotDatabase, "Creating new SnapshotPageInfo for url %s", urlForLogging(key).ascii().data());
        pageInfo = new SnapshotPageInfo(crossThreadURL);
        m_pageInfoMap.set(crossThreadURL, pageInfo);
    }

    // We've done the initial import of all URLs known in the database.  If this record doesn't exist now, it never will
     return pageInfo;
}


// ************************
// *** Sync Thread Only ***
// ************************

bool SnapshotDatabase::shouldStopThreadActivity() const
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    return m_threadTerminationRequested || m_removeSnapshotsRequested;
}

void* SnapshotDatabase::snapshotDatabaseSyncThreadStart(void* vDatabase)
{
    SnapshotDatabase* snapshotDB = static_cast<SnapshotDatabase*>(vDatabase);
    return snapshotDB->snapshotDatabaseSyncThread();
}

void* SnapshotDatabase::snapshotDatabaseSyncThread()
{
    // The call to create this thread might not complete before the thread actually starts, so we might fail this ASSERT_SNAPSHOT_SYNC_THREAD() because the pointer
    // to our thread structure hasn't been filled in yet.
    // To fix this, the main thread acquires this lock before creating us, then releases the lock after creation is complete.  A quick lock/unlock cycle here will
    // prevent us from running before that call completes
    m_syncLock.lock();
    m_syncLock.unlock();

    ASSERT_SNAPSHOT_SYNC_THREAD();

    LOG(SnapshotDatabase, "(THREAD) SnapshotDatabase sync thread started");

#if !LOG_DISABLED
    double startTime = currentTime();
#endif

    // Need to create the database path if it doesn't already exist
    makeAllDirectories(m_databaseDirectory);

#if 0
    // Existence of a journal file is evidence of a previous crash/force quit and automatically qualifies
    // us to do an integrity check
    if (!checkIntegrityOnOpen) {
        String journalFilename = m_completeDatabasePath + "-journal";
        checkIntegrityOnOpen = fileExists(journalFilename);
    }
#endif

    {
        MutexLocker locker(m_syncLock);
        if (!m_syncDB.open(m_completeDatabasePath)) {
            LOG_ERROR("Unable to open icon database at path %s - %s", m_completeDatabasePath.ascii().data(), m_syncDB.lastErrorMsg());
            return 0;
        }
    }

    if (shouldStopThreadActivity())
        return syncThreadMainLoop();

#if !LOG_DISABLED
    double timeStamp = currentTime();
    LOG(SnapshotDatabase, "(THREAD) Open took %.4f seconds", timeStamp - startTime);
#endif

    performOpenInitialization();
    if (shouldStopThreadActivity())
        return syncThreadMainLoop();

#if !LOG_DISABLED
    double newStamp = currentTime();
    LOG(SnapshotDatabase, "(THREAD) performOpenInitialization() took %.4f seconds, now %.4f seconds from thread start", newStamp - timeStamp, newStamp - startTime);
    timeStamp = newStamp;
#endif

    LOG(SnapshotDatabase, "(THREAD) Beginning sync");
    return syncThreadMainLoop();
}

static int databaseVersionNumber(SQLiteDatabase& db)
{
    return SQLiteStatement(db, "SELECT value FROM DatabaseInfo WHERE key = 'Version';").getColumnInt(0);
}

static bool isValidDatabase(SQLiteDatabase& db)
{
    // These tables should always exist in a valid db
    if (!db.tableExists("SnapshotData") || !db.tableExists("PageInfo") || !db.tableExists("DatabaseInfo"))
        return false;

    int version = databaseVersionNumber(db);

    if (version > currentDatabaseVersion) {
        LOG(SnapshotDatabase, "Database version number %i is greater than our current version number %i", version, currentDatabaseVersion);
        return false;
    }

    if (version < currentDatabaseVersion) {
        LOG(SnapshotDatabase, "DB version is not found or below expected valid version");
        return false;
    }

    return true;
}

static void createDatabaseTables(SQLiteDatabase& db)
{
    if (!db.executeCommand("CREATE TABLE PageInfo (url TEXT NOT NULL ON CONFLICT FAIL UNIQUE ON CONFLICT REPLACE, stamp INTEGER, snapshotID INTEGER NOT NULL ON CONFLICT FAIL);")) {
        LOG_ERROR("Could not create PageInfo table in database (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
    if (!db.executeCommand("CREATE INDEX PageInfoIndex ON PageInfo (url, stamp);")) {
        LOG_ERROR("Could not create PageInfo index in database (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
    if (!db.executeCommand("CREATE TABLE SnapshotData (snapshotID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE ON CONFLICT REPLACE, data BLOB);")) {
        LOG_ERROR("Could not create SnapshotData table in database (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
    if (!db.executeCommand("CREATE INDEX SnapshotDataIndex ON SnapshotData (snapshotID);")) {
        LOG_ERROR("Could not create SnapshotData index in database (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
    if (!db.executeCommand("CREATE TABLE DatabaseInfo (key TEXT NOT NULL ON CONFLICT FAIL UNIQUE ON CONFLICT REPLACE, value TEXT NOT NULL ON CONFLICT FAIL);")) {
        LOG_ERROR("Could not create DatabaseInfo table in database (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
    if (!db.executeCommand(String("INSERT INTO DatabaseInfo VALUES ('Version', ") + String::number(currentDatabaseVersion) + ");")) {
        LOG_ERROR("Could not insert icon database version into DatabaseInfo table (%i) - %s", db.lastError(), db.lastErrorMsg());
        db.close();
        return;
    }
}

void SnapshotDatabase::performOpenInitialization()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    if (!isOpen())
        return;

    if (checkIntegrityOnOpen) {
        checkIntegrityOnOpen = false;
        if (!checkIntegrity()) {
            LOG(SnapshotDatabase, "Integrity check was bad - dumping SnapshotDatabase");

            m_syncDB.close();

            {
                MutexLocker locker(m_syncLock);
                // Should've been consumed by SQLite, delete just to make sure we don't see it again in the future;
                deleteFile(m_completeDatabasePath + "-journal");
                deleteFile(m_completeDatabasePath);
            }

            // Reopen the write database, creating it from scratch
            if (!m_syncDB.open(m_completeDatabasePath)) {
                LOG_ERROR("Unable to open icon database at path %s - %s", m_completeDatabasePath.ascii().data(), m_syncDB.lastErrorMsg());
                return;
            }
        }
    }

    if (!isValidDatabase(m_syncDB)) {
        LOG(SnapshotDatabase, "%s is missing or in an invalid state - reconstructing", m_completeDatabasePath.ascii().data());
        m_syncDB.clearAllTables();
        createDatabaseTables(m_syncDB);
    }

    m_syncDB.setMaximumSize(m_maximumSize);

    // Reduce sqlite RAM cache size from default 2000 pages (~1.5kB per page). 3MB of cache for icon database is overkill
    if (!SQLiteStatement(m_syncDB, "PRAGMA cache_size = 100;").executeCommand())
        LOG_ERROR("SQLite database could not set cache_size");
}

bool SnapshotDatabase::checkIntegrity()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    SQLiteStatement integrity(m_syncDB, "PRAGMA integrity_check;");
    if (integrity.prepare() != SQLResultOk) {
        LOG_ERROR("checkIntegrity failed to execute");
        return false;
    }

    int resultCode = integrity.step();
    if (resultCode == SQLResultOk)
        return true;

    if (resultCode != SQLResultRow)
        return false;

    int columns = integrity.columnCount();
    if (columns != 1) {
        LOG_ERROR("Received %i columns performing integrity check, should be 1", columns);
        return false;
    }

    String resultText = integrity.getColumnText(0);

    // A successful, no-error integrity check will be "ok" - all other strings imply failure
    if (resultText == "ok")
        return true;

    LOG_ERROR("Snapshot database integrity check failed - \n%s", resultText.ascii().data());
    return false;
}

void* SnapshotDatabase::syncThreadMainLoop()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    m_syncLock.lock();

    // It's possible thread termination is requested before the main loop even starts - in that case, just skip straight to cleanup
    while (!m_threadTerminationRequested) {
        m_syncLock.unlock();

#if !LOG_DISABLED
        double timeStamp = currentTime();
#endif
        LOG(SnapshotDatabase, "(THREAD) Main work loop starting");

        // If we should remove all icons, do it now.  This is an uninteruptible procedure that we will always do before quitting if it is requested
        if (m_removeSnapshotsRequested) {
            removeAllSnapshotsOnThread();
            m_removeSnapshotsRequested = false;
        }

        // Then, if the thread should be quitting, quit now!
        if (m_threadTerminationRequested)
            break;

        bool didAnyWork = true;
        while (didAnyWork) {
            bool didWrite = writeToDatabase();
            if (shouldStopThreadActivity())
                break;

            didAnyWork = readSnapshot();
            if (shouldStopThreadActivity())
                break;

            didAnyWork = didAnyWork || didWrite;
        }

#if !LOG_DISABLED
        double newstamp = currentTime();
        LOG(SnapshotDatabase, "(THREAD) Main work loop ran for %.4f seconds, %s requested to terminate", newstamp - timeStamp, shouldStopThreadActivity() ? "was" : "was not");
#endif

        m_syncLock.lock();

        // There is some condition that is asking us to stop what we're doing now and handle a special case
        // This is either removing all icons, or shutting down the thread to quit the app
        // We handle those at the top of this main loop so continue to jump back up there
        if (shouldStopThreadActivity())
            continue;

        m_syncCondition.wait(m_syncLock);
    }

    m_syncLock.unlock();

    // Thread is terminating at this point
    cleanupSyncThread();

    return 0;
}

bool SnapshotDatabase::writeToDatabase()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

#if !LOG_DISABLED
    double timeStamp = currentTime();
#endif

    bool didAnyWork = false;

    // We can copy the current work queue then clear it out - If any new work comes in while we're writing out,
    // we'll pick it up on the next pass.  This greatly simplifies the locking strategy for this method and remains cohesive with changes
    // asked for by the database on the main thread

    SQLiteTransaction syncTransaction(m_syncDB);
    syncTransaction.begin();

    while(1) {
        HashSet<SnapshotPageInfo*>::iterator iter;
        {
            MutexLocker locker(m_pendingSyncLock);
            iter = m_pendingSync.begin();
            if (iter == m_pendingSync.end())
                break;
        }

        didAnyWork = true;

        SnapshotPageInfo* pageInfo = *iter.get();
        writeSnapshotPageInfoToSQLDatabase(pageInfo);
        LOG(SnapshotDatabase, "Committed snapshot for url %s to database", urlForLogging(pageInfo->url()).ascii().data());

        {
            MutexLocker locker(m_urlAndSnapshotLock);

            if (!m_pageInfoMap.contains(pageInfo->url()))
                delete pageInfo;
        }

        {
            MutexLocker locker(m_pendingSyncLock);
            m_pendingSync.remove(iter);
        }
    }

    syncTransaction.commit();

    LOG(SnapshotDatabase, "Updating the database took %.4f seconds", currentTime() - timeStamp);

    return didAnyWork;
}

bool SnapshotDatabase::readSnapshot()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

#if !LOG_DISABLED
    double timeStamp = currentTime();
#endif

    bool didAnyWork = false;

    while(1) {
        HashSet<SnapshotPageInfo*>::iterator iter;
        {
            MutexLocker locker(m_pendingReadingLock);
            iter = m_pendingReading.begin();
            if (iter == m_pendingReading.end())
                break;
        }

        didAnyWork = true;

        SnapshotPageInfo* pageInfo = *iter.get();
        bool isValid = true;

        {
            MutexLocker locker(m_urlAndSnapshotLock);

            if (!m_pageInfoMap.contains(pageInfo->url()))
                isValid = false;
        }

        if (isValid) {
            if (!pageInfo->pageID()) {
                Vector<char>* data = new Vector<char>;
                if (data)
                    getImageDataFromSQLDatabase(pageInfo, *data);
                dispatchDidImportSnapshotDataForPageURLOnMainThread(pageInfo, data);
            } else {
                cairo_surface_t* snapshot = pageInfo->chrome()->getPageSnapshot(m_context, pageInfo->pageID());
                cairo_surface_t* data;
                if (snapshot && m_scaleFactor != 1.) {
                    cairo_surface_t* resizeSnapshot = cairo_image_surface_create(cairo_image_surface_get_format(snapshot),
                                                                                 cairo_image_surface_get_width(snapshot) * m_scaleFactor,
                                                                                 cairo_image_surface_get_height(snapshot) * m_scaleFactor);
                    if (resizeSnapshot) {
                        RefPtr<cairo_t> cr = adoptRef(cairo_create(resizeSnapshot));
                        if (cr) {
                            cairo_scale(cr.get(), m_scaleFactor, m_scaleFactor);
                            cairo_set_source_surface(cr.get(), snapshot, 0, 0);
                            cairo_set_operator(cr.get(), CAIRO_OPERATOR_SOURCE);
                            cairo_paint(cr.get());
                        }
                    }

                    cairo_surface_destroy(snapshot);
                    data = resizeSnapshot;
                } else
                    data = snapshot;
                dispatchDidImportSnapshotDataForPageURLOnMainThread(pageInfo, data);
            }
        } else
            delete pageInfo;

        {
            MutexLocker locker(m_pendingReadingLock);
            m_pendingReading.remove(iter);
        }

        if (shouldStopThreadActivity())
            return didAnyWork;
    }

    LOG(SnapshotDatabase, "Reading from database took %.4f seconds", currentTime() - timeStamp);

    return didAnyWork;
}

void SnapshotDatabase::pruneDatabase()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    if (!isOpen())
        return;

    Vector<int64_t> pageIDsToDelete;

    SQLiteStatement pageSQL(m_syncDB, "SELECT rowid FROM PageInfo ORDER BY stamp;");
    pageSQL.prepare();

    int result;
    int maxDeleteCount = static_cast<int>(static_cast<float>(3) / m_scaleFactor);
    if (maxDeleteCount < 3)
        maxDeleteCount = 3;

    while ((result = pageSQL.step()) == SQLResultRow) {
        MutexLocker locker(m_urlAndSnapshotLock);
        pageIDsToDelete.append(pageSQL.getColumnInt64(0));

        if (!(--maxDeleteCount)) {
            result = SQLResultDone;
            break;
        }
    }
    pageSQL.finalize();

    if (result != SQLResultDone)
        LOG_ERROR("Error reading PageInfo table from on-disk DB");

    // Delete page URLs that were in the table, but not in our retain count set.
    size_t numToDelete = pageIDsToDelete.size();
    if (numToDelete) {
        SQLiteTransaction pruningTransaction(m_syncDB);
        pruningTransaction.begin();

        SQLiteStatement pageDeleteSQL(m_syncDB, "DELETE FROM PageInfo WHERE rowid = (?);");
        pageDeleteSQL.prepare();

        for (size_t i = 0; i < numToDelete; ++i) {
            LOG(SnapshotDatabase, "Pruning page with rowid %lli from disk", static_cast<long long>(pageIDsToDelete[i]));
            pageDeleteSQL.bindInt64(1, pageIDsToDelete[i]);
            int result = pageDeleteSQL.step();
            if (result != SQLResultDone)
                LOG_ERROR("Unabled to delete page with id %lli from disk", static_cast<long long>(pageIDsToDelete[i]));
            pageDeleteSQL.reset();
        }

        pruningTransaction.commit();
        pageDeleteSQL.finalize();
    }

    // Deleting unreferenced snapshots from the tables has to be atomic -
    // If the user quits while these are taking place, they might have to wait.  Thankfully this will rarely be an issue
    // A user on a network home directory with a wildly inconsistent database might see quite a pause...

    SQLiteTransaction pruningTransaction(m_syncDB);
    pruningTransaction.begin();

    if (!m_syncDB.executeCommand("DELETE FROM SnapshotData WHERE rowid NOT IN (SELECT snapshotID FROM PageInfo);"))
        LOG_ERROR("Failed to execute SQL to prune unretained snapshots from the on-disk SnapshotData table");
    if (!m_syncDB.executeCommand("DELETE FROM PageInfo WHERE snapshotID NOT IN (SELECT rowid FROM SnapshotData);"))
        LOG_ERROR("Failed to execute SQL to prune unretained page infos from the on-disk PageInfo table");

    pruningTransaction.commit();

    m_syncDB.runVacuumCommand();
}

void SnapshotDatabase::removeAllSnapshotsOnThread()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    LOG(SnapshotDatabase, "Removing all snapshots on the sync thread");

    // Delete all the prepared statements so they can start over
    deleteAllPreparedStatements();

    // To reset the on-disk database, we'll wipe all its tables then vacuum it
    // This is easier and safer than closing it, deleting the file, and recreating from scratch
    m_syncDB.clearAllTables();
    m_syncDB.runVacuumCommand();
    createDatabaseTables(m_syncDB);

    LOG(SnapshotDatabase, "Dispatching notification that we removed all icons");
}

void SnapshotDatabase::deleteAllPreparedStatements()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    m_getSnapshotIDForURLStatement.clear();
    m_getImageDataStatement.clear();
    m_deletePageInfoForIDStatement.clear();
    m_deleteSnapshotDataForIDStatement.clear();
    m_updateStampStatement.clear();
    m_updateSnapshotDataStatement.clear();
    m_setSnapshotDataStatement.clear();
    m_setPageInfoStatement.clear();
}

void* SnapshotDatabase::cleanupSyncThread()
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

#if !LOG_DISABLED
    double timeStamp = currentTime();
#endif

    // If the removeIcons flag is set, remove all icons from the db.
    if (m_removeSnapshotsRequested)
        removeAllSnapshotsOnThread();

    // Sync remaining snapshots out
    LOG(SnapshotDatabase, "(THREAD) Doing final writeout and closure of sync thread");
    writeToDatabase();

    // Close the database
    MutexLocker locker(m_syncLock);

    m_databaseDirectory = String();
    m_completeDatabasePath = String();
    deleteAllPreparedStatements();
    m_syncDB.close();

#if !LOG_DISABLED
    LOG(SnapshotDatabase, "(THREAD) Final closure took %.4f seconds", currentTime() - timeStamp);
#endif

    m_syncThreadRunning = false;
    return 0;
}

// readySQLiteStatement() handles two things
// 1 - If the SQLDatabase& argument is different, the statement must be destroyed and remade.  This happens when the user
//     switches to and from private browsing
// 2 - Lazy construction of the Statement in the first place, in case we've never made this query before
inline void readySQLiteStatement(OwnPtr<SQLiteStatement>& statement, SQLiteDatabase& db, const String& str)
{
    if (statement && (statement->database() != &db || statement->isExpired())) {
        if (statement->isExpired())
            LOG(SnapshotDatabase, "SQLiteStatement associated with %s is expired", str.ascii().data());
        statement.clear();
    }
    if (!statement) {
        statement = adoptPtr(new SQLiteStatement(db, str));
        if (statement->prepare() != SQLResultOk)
            LOG_ERROR("Preparing statement %s failed", str.ascii().data());
    }
}

int64_t SnapshotDatabase::getSnapshotIDFromSQLDatabase(const String& url)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    readySQLiteStatement(m_getSnapshotIDForURLStatement, m_syncDB, "SELECT snapshotID FROM PageInfo WHERE url=(?);");
    m_getSnapshotIDForURLStatement->bindText(1, url);

    int64_t result = m_getSnapshotIDForURLStatement->step();
    if (result == SQLResultRow)
        result = m_getSnapshotIDForURLStatement->getColumnInt64(0);
    else {
        if (result != SQLResultDone)
            LOG_ERROR("getSnapshotIDFromSQLDatabase failed for url %s", urlForLogging(iconURL).ascii().data());
        result = 0;
    }

    m_getSnapshotIDForURLStatement->reset();
    return result;
}

void SnapshotDatabase::getImageDataFromSQLDatabase(SnapshotPageInfo* pageInfo, Vector<char>& data)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    int64_t id = getSnapshotIDFromSQLDatabase(pageInfo->url());
    if (!id)
        return;

    readySQLiteStatement(m_getImageDataStatement, m_syncDB, "SELECT data FROM SnapshotData WHERE snapshotID=(?);");
    m_getImageDataStatement->bindInt64(1, id);

    int result = m_getImageDataStatement->step();
    if (result == SQLResultRow)
        m_getImageDataStatement->getColumnBlobAsVector(0, data);
    else if (result != SQLResultDone) {
        LOG_ERROR("%s failed for url %s", __func__, urlForLogging(url).ascii().data());
        return;
    }

    m_getImageDataStatement->reset();

    if (m_maximumSize) {
        pageInfo->setTimestamp(static_cast<time_t>(currentTime()));

        readySQLiteStatement(m_updateStampStatement, m_syncDB, "UPDATE PageInfo SET stamp=(?) WHERE snapshotID=(?);");
        m_updateStampStatement->bindInt64(1, pageInfo->timestamp());
        m_updateStampStatement->bindInt64(2, id);

        if (m_updateStampStatement->step() != SQLResultDone)
            LOG_ERROR("Failed to update icon info for url %s", urlForLogging(pageInfo->url()).ascii().data());

        m_updateStampStatement->reset();
    }
}

void SnapshotDatabase::removeSnapshotPageInfoFromSQLDatabase(const String& url)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    if (url.isEmpty())
        return;

    // There would be a transaction here to make sure these removals are atomic
    // In practice the only caller of this method is always wrapped in a transaction itself so placing another here is unnecessary

    // It's possible this icon is not in the database because of certain rapid browsing patterns (such as a stress test) where the
    // icon is marked to be added then marked for removal before it is ever written to disk.  No big deal, early return
    int64_t id = getSnapshotIDFromSQLDatabase(url);
    if (!id)
        return;

    readySQLiteStatement(m_deletePageInfoForIDStatement, m_syncDB, "DELETE FROM PageInfo WHERE snapshotID=(?);");
    m_deletePageInfoForIDStatement->bindInt64(1, id);

    if (m_deletePageInfoForIDStatement->step() != SQLResultDone)
        LOG_ERROR("m_deletePageInfoForIDStatement failed for url %s", urlForLogging(url).ascii().data());

    m_deletePageInfoForIDStatement->reset();

    readySQLiteStatement(m_deleteSnapshotDataForIDStatement, m_syncDB, "DELETE FROM SnapshotData WHERE snapshotID=(?);");
    m_deleteSnapshotDataForIDStatement->bindInt64(1, id);

    if (m_deleteSnapshotDataForIDStatement->step() != SQLResultDone)
        LOG_ERROR("m_deleteSnapshotDataForIDStatement failed for url %s", urlForLogging(iconURL).ascii().data());

    m_deleteSnapshotDataForIDStatement->reset();
}

void SnapshotDatabase::writeSnapshotPageInfoToSQLDatabase(SnapshotPageInfo* pageInfo)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();

    if (pageInfo->url().isEmpty())
        return;

    // Get the ID for this url
    int64_t id = getSnapshotIDFromSQLDatabase(pageInfo->url());

    pageInfo->setTimestamp(static_cast<time_t>(currentTime()));

    // If there is already an ID in place, update the database.
    // Otherwise, insert new records
    if (id) {
        readySQLiteStatement(m_updateStampStatement, m_syncDB, "UPDATE PageInfo SET stamp=(?) WHERE snapshotID=(?);");
        m_updateStampStatement->bindInt64(1, pageInfo->timestamp());
        m_updateStampStatement->bindInt64(2, id);

        if (m_updateStampStatement->step() != SQLResultDone) {
            LOG_ERROR("Failed to update snapshot info for url %s", urlForLogging(pageInfo->url()).ascii().data());
            return;
        }

        m_updateStampStatement->reset();

        readySQLiteStatement(m_updateSnapshotDataStatement, m_syncDB, "UPDATE SnapshotData SET data=(?) WHERE snapshotID=(?);");
        m_updateSnapshotDataStatement->bindInt64(2, id);

        // If we *have* image data, bind it to this statement - Otherwise bind "null" for the blob data,
        // signifying that this icon doesn't have any data
        if (pageInfo->imageData() && pageInfo->imageDataSize())
            m_updateSnapshotDataStatement->bindBlob(1, pageInfo->imageData(), pageInfo->imageDataSize());
        else
            m_updateSnapshotDataStatement->bindNull(1);

        if (m_updateSnapshotDataStatement->step() != SQLResultDone)
            LOG_ERROR("Failed to update icon data for url %s", urlForLogging(pageInfo->url()).ascii().data());

        m_updateSnapshotDataStatement->reset();
    } else {
        readySQLiteStatement(m_setSnapshotDataStatement, m_syncDB, "INSERT INTO SnapshotData (data) VALUES (?);");

        int maxTryCount = 3;
        while (maxTryCount--) {
            // If we *have* image data, bind it to this statement - Otherwise bind "null" for the blob data,
            // signifying that this icon doesn't have any data
            if (pageInfo->imageData() && pageInfo->imageDataSize())
                m_setSnapshotDataStatement->bindBlob(1, pageInfo->imageData(), pageInfo->imageDataSize());
            else
                m_setSnapshotDataStatement->bindNull(1);

            int result = m_setSnapshotDataStatement->step();
            m_setSnapshotDataStatement->reset();
            if (result == SQLResultDone)
                break;

            LOG_ERROR("Failed to set icon data for url %s", urlForLogging(pageInfo->url()).ascii().data());
            pruneDatabase();
        }

        id = m_syncDB.lastInsertRowID();

        readySQLiteStatement(m_setPageInfoStatement, m_syncDB, "INSERT INTO PageInfo (url,stamp,snapshotID) VALUES (?, ?, ?);");
        m_setPageInfoStatement->bindText(1, pageInfo->url());
        m_setPageInfoStatement->bindInt64(2, pageInfo->timestamp());
        m_setPageInfoStatement->bindInt64(3, id);

        if (m_setPageInfoStatement->step() != SQLResultDone)
            LOG_ERROR("Failed to set page info for url %s", urlForLogging(pageInfo->url()).ascii().data());

        m_setPageInfoStatement->reset();
    }
}

class WorkItem {
public:
    WorkItem(SnapshotDatabase* snapshotDB)
        : m_snapshotDB(snapshotDB)
    { }
    virtual void performWork() = 0;
    virtual ~WorkItem() { }

protected:
    SnapshotDatabase* m_snapshotDB;
};

class ImportedSnapshotDataForURLWorkItem : public WorkItem {
public:
    ImportedSnapshotDataForURLWorkItem(SnapshotDatabase* snapshotDB, const String& url, Vector<char>* bitData, NativeImagePtr imageData)
        : WorkItem(snapshotDB)
        , m_url(new String(url.threadsafeCopy()))
        , m_bitData(bitData)
        , m_imageData(imageData)
    { }

    virtual ~ImportedSnapshotDataForURLWorkItem()
    {
        delete m_url;
        if (m_bitData)
            delete m_bitData;
#if USE(CAIRO)
        if (m_imageData)
            cairo_surface_destroy(m_imageData);
#endif
    }

    virtual void performWork()
    {
        m_snapshotDB->didImportSnapshotData(*m_url, m_bitData, m_imageData);
    }

private:
    String* m_url;
    Vector<char>* m_bitData;
    NativeImagePtr m_imageData;
};

static void performWorkItem(void* context)
{
    WorkItem* item = static_cast<WorkItem*>(context);
    item->performWork();
    delete item;
}

void SnapshotDatabase::dispatchDidImportSnapshotDataForPageURLOnMainThread(const SnapshotPageInfo* pageInfo, Vector<char>* data)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();
    callOnMainThread(performWorkItem, new ImportedSnapshotDataForURLWorkItem(this, pageInfo->url(), data, 0));
}

void SnapshotDatabase::dispatchDidImportSnapshotDataForPageURLOnMainThread(const SnapshotPageInfo* pageInfo, NativeImagePtr data)
{
    ASSERT_SNAPSHOT_SYNC_THREAD();
    callOnMainThread(performWorkItem, new ImportedSnapshotDataForURLWorkItem(this, pageInfo->url(), 0, data));
}

} // namespace WebCore
