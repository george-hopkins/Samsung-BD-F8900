#include "config.h"
#include "BEContext.h"

#include "BEAPICast.h"
#include "WebContext.h"
#include "WKContextPrivate.h"
#include "MemoryCache.h"

#include <WebCore/SnapshotDatabase.h>
#include <stdio.h>

using namespace WebKit;
using namespace WebCore;

BEContextRef BEContextCreate()
{
    RefPtr<WebContext> context = WebContext::create(String());
    return toBEAPI(context.release().releaseRef());
}

BEContextRef BEContextGetSharedProcessContext()
{
    return toBEAPI(WebContext::sharedProcessContext());
}

BEContextRef BEContextGetSharedThreadContext()
{
    return toBEAPI(WebContext::sharedThreadContext());
}

void BEContextDestroy(BEContextRef contextRef)
{
    WebContext* context = toBEImpl(contextRef);
    if (context == WebContext::sharedProcessContext())
        return;
    delete context;
}

void BEContextSetIconDatabasePath(BEContextRef contextRef, const char* iconDatabasePath)
{
    toBEImpl(contextRef)->setIconDatabasePath(String::fromUTF8(iconDatabasePath));
}

void BEContextSetDatabaseDirectory(BEContextRef contextRef, const char* databaseDirectory)
{
    toBEImpl(contextRef)->setDatabaseDirectory(String::fromUTF8(databaseDirectory));
}

void BEContextSetLocalStorageDirectory(BEContextRef contextRef, const char* localStorageDirectory)
{
    toBEImpl(contextRef)->setLocalStorageDirectory(String::fromUTF8(localStorageDirectory));
}

BEApplicationCacheManagerRef BEContextGetApplicationCacheManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->applicationCacheManagerProxy());
}

BECookieManagerRef BEContextGetCookieManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->cookieManagerProxy());
}

BEDatabaseManagerRef BEContextGetDatabaseManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->databaseManagerProxy());
}

BEGeolocationManagerRef BEContextGetGeolocationManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->geolocationManagerProxy());
}

BEIconDatabaseRef BEContextGetIconDatabase(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->iconDatabase());
}

BEKeyValueStorageManagerRef BEContextGetKeyValueStorageManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->keyValueStorageManagerProxy());
}

BEMediaCacheManagerRef BEContextGetMediaCacheManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->mediaCacheManagerProxy());
}

BEPluginSiteDataManagerRef BEContextGetPluginSiteDataManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->pluginSiteDataManager());
}

BEResourceCacheManagerRef BEContextGetResourceCacheManager(BEContextRef contextRef)
{
    return toBEAPI(toBEImpl(contextRef)->resourceCacheManagerProxy());
}

void BEContextOpenSnapshotDatabase(BEContextRef contextRef, const double scaleFactor, const int maximumSize)
{
    WebCore::SnapshotDatabase::getDB().open(scaleFactor, maximumSize, toBEImpl(contextRef));
}

void BEContextSetDefaultSessionProperty(BEContextRef contextRef, const char* propertyName, const char* value)
{
    toBEImpl(contextRef)->setDefaultSessionProperty(String::fromUTF8(propertyName), String::fromUTF8(value));
}

void BEContextPruneToPercentage(float targetPercentLive)
{
	if (targetPercentLive == 0.0)
	{
		WKContextRef contextRef = WKContextGetSharedThreadContext();
		if (contextRef)
		{
			WKResourceCacheManagerRef cacheManagerRef = WKContextGetResourceCacheManager(contextRef);
			if (cacheManagerRef)
			{
				WKResourceCacheManagerClearCacheForAllOrigins(cacheManagerRef, WKResourceCachesToClearAll);
			}
		}
	}

	memoryCache()->setDeadDecodedDataDeletionInterval(1);
	memoryCache()->evictResources();
	memoryCache()->pruneToPercentage(targetPercentLive);
	memoryCache()->setDeadDecodedDataDeletionInterval(0);
}

#ifdef TVGG_DOCUMENT_VIEWER


void BESetDocViewerStartCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerStart)
{
    toBEImpl(contextRef)->setDocViewerStartCallback(static_cast<WKDocumentViewerCallback>(beDocViewerStart));
}

void BESetDocViewerFinishCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerFinish) 
{ 
    toBEImpl(contextRef)->setDocViewerFinishCallback(static_cast<WKDocumentViewerCallback>(beDocViewerFinish));
}

void BESetDocViewerProgressCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerProgress) 
{ 
    toBEImpl(contextRef)->setDocViewerProgressCallback(static_cast<WKDocumentViewerCallback>(beDocViewerProgress));
}

#endif

