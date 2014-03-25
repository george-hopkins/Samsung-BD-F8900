#ifndef SnapshotDatabase_h
#define SnapshotDatabase_h

#include "CachedResourceHandle.h"
#include "ImageSource.h"
#include "SQLiteDatabase.h"
#include "Timer.h"

#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/Noncopyable.h>
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>
#include <wtf/Threading.h>
#include <wtf/text/StringHash.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class CachedImage;
class Chrome;
class HTMLSnapshotImageElement;
class Image;
class IntSize;
class KURL;
class SharedBuffer;
class SQLTransaction;

class SnapshotPageInfo {
    WTF_MAKE_NONCOPYABLE(SnapshotPageInfo); WTF_MAKE_FAST_ALLOCATED;
public:
    SnapshotPageInfo(const String&);
    ~SnapshotPageInfo();

    const String& url() const { return m_url; }

    uint64_t pageID() const { return m_pageID; }
    Chrome* chrome() const { return m_chrome; }

    time_t timestamp() const { return m_stamp; }
    void setTimestamp(time_t stamp) { m_stamp = stamp; }

    void setImageData(void*, int);
    void setCachedImageData(Vector<char>&);
    void setCachedImageData(NativeImagePtr);

    void* imageData() const { return m_data; }
    int imageDataSize() const { return m_dataSize; }

    CachedImage* image() const { return m_image.get(); }
    void clearImage();

    // Returns false if the page wasn't retained beforehand, true if the retain count was already 1 or higher
    bool retain(HTMLSnapshotImageElement*);
    // Returns true if the page is still retained after the call.  False if the retain count just dropped to 0
    bool release(HTMLSnapshotImageElement*);

    inline int retainCount() const { return m_retainCount; }

private:
    String m_url;
    uint64_t m_pageID;
    Chrome* m_chrome;
    time_t m_stamp;
    void* m_data;
    int m_dataSize;
    int m_retainCount;
    CachedResourceHandle<CachedImage> m_image;
};

class SnapshotDatabase {
    WTF_MAKE_FAST_ALLOCATED;

// *** Main Thread Only ***
public:
    static SnapshotDatabase& getDB();

    ~SnapshotDatabase();

    bool open(const double, const int, void*);
    void close();

    void removeAllSnapshots();

    void retainSnapshotForURL(const String&, HTMLSnapshotImageElement*);
    void releaseSnapshotForImage(CachedImage*, HTMLSnapshotImageElement*);

    void setSnapshotDataForURL(const String&, const NativeImagePtr);

    void didImportSnapshotData(const String&, Vector<char>*, NativeImagePtr);

    void setPrivateBrowsingEnabled(bool);
    bool isPrivateBrowsingEnabled() const;

    static void checkIntegrityBeforeOpening();

private:
    SnapshotDatabase();

    void wakeSyncThread();
    void scheduleOrDeferSyncTimer();
    void syncTimerFired(Timer<SnapshotDatabase>*);

    Timer<SnapshotDatabase> m_syncTimer;
    ThreadIdentifier m_syncThread;
    bool m_syncThreadRunning;

// *** Any Thread ***
public:
    bool isOpen() const;
    String databasePath() const;
    double scaleFactor() const { return m_scaleFactor; }

private:
    SnapshotPageInfo* getSnapshotPageInfo(const String&);

    bool m_privateBrowsingEnabled;

    mutable Mutex m_syncLock;
    ThreadCondition m_syncCondition;
    String m_databaseDirectory;
    // Holding m_syncLock is required when accessing m_completeDatabasePath
    String m_completeDatabasePath;

    bool m_threadTerminationRequested;
    bool m_removeSnapshotsRequested;

    double m_scaleFactor;
    int m_maximumSize;
    void* m_context;

    Mutex m_urlAndSnapshotLock;
    // Holding m_urlAndIconLock is required when accessing any of the following data structures or the objects they contain
    HashMap<String, SnapshotPageInfo*> m_pageInfoMap;

    Mutex m_pendingReadingLock;
    // Holding m_pendingSyncLock is required when accessing any of the following data structures - when dealing with IconRecord*s, holding m_urlAndIconLock is also required
    HashSet<SnapshotPageInfo*> m_pendingReading;

    Mutex m_pendingSyncLock;
    // Holding m_pendingSyncLock is required when accessing any of the following data structures
    HashSet<SnapshotPageInfo*> m_pendingSync;

// *** Sync Thread Only ***
public:
    bool shouldStopThreadActivity() const;

private:
    static void* snapshotDatabaseSyncThreadStart(void*);
    void* snapshotDatabaseSyncThread();

    // The following block of methods are called exclusively by the sync thread to manage i/o to and from the database
    // Each method should periodically monitor m_threadTerminationRequested when it makes sense to return early on shutdown
    void performOpenInitialization();
    bool checkIntegrity();
    void* syncThreadMainLoop();
    bool writeToDatabase();
    bool readSnapshot();
    void pruneDatabase();
    void removeAllSnapshotsOnThread();
    void deleteAllPreparedStatements();
    void* cleanupSyncThread();

    int64_t getSnapshotIDFromSQLDatabase(const String&);
    void getImageDataFromSQLDatabase(SnapshotPageInfo*, Vector<char>&);
    void removeSnapshotPageInfoFromSQLDatabase(const String&);
    void writeSnapshotPageInfoToSQLDatabase(SnapshotPageInfo*);

    // Methods to dispatch client callbacks on the main thread
    void dispatchDidImportSnapshotDataForPageURLOnMainThread(const SnapshotPageInfo*, Vector<char>*);
    void dispatchDidImportSnapshotDataForPageURLOnMainThread(const SnapshotPageInfo*, NativeImagePtr);

    SQLiteDatabase m_syncDB;

    OwnPtr<SQLiteStatement> m_getSnapshotIDForURLStatement;
    OwnPtr<SQLiteStatement> m_getImageDataStatement;
    OwnPtr<SQLiteStatement> m_deletePageInfoForIDStatement;
    OwnPtr<SQLiteStatement> m_deleteSnapshotDataForIDStatement;
    OwnPtr<SQLiteStatement> m_updateStampStatement;
    OwnPtr<SQLiteStatement> m_updateSnapshotDataStatement;
    OwnPtr<SQLiteStatement> m_setSnapshotDataStatement;
    OwnPtr<SQLiteStatement> m_setPageInfoStatement;
};

} // namespace WebCore

#endif // SnapshotDatabase_h
