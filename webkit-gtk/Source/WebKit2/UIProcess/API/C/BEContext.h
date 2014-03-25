#ifndef BEContext_h
#define BEContext_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

BEContextRef BEContextCreate();
BEContextRef BEContextGetSharedProcessContext();
BEContextRef BEContextGetSharedThreadContext();

#ifdef TVGG_DOCUMENT_VIEWER
typedef void(*  DocumentViewerCallback )(BETypeRef handle, BETypeRef data);
#endif

void BEContextDestroy(BEContextRef context);

void BEContextSetIconDatabasePath(BEContextRef context, const char* iconDatabasePath);
void BEContextSetDatabaseDirectory(BEContextRef context, const char* databaseDirectory);
void BEContextSetLocalStorageDirectory(BEContextRef context, const char* localStorageDirectory);

BEApplicationCacheManagerRef BEContextGetApplicationCacheManager(BEContextRef context);
BECookieManagerRef BEContextGetCookieManager(BEContextRef context);
BEDatabaseManagerRef BEContextGetDatabaseManager(BEContextRef context);
BEGeolocationManagerRef BEContextGetGeolocationManager(BEContextRef context);
BEIconDatabaseRef BEContextGetIconDatabase(BEContextRef context);
BEKeyValueStorageManagerRef BEContextGetKeyValueStorageManager(BEContextRef context);
BEMediaCacheManagerRef BEContextGetMediaCacheManager(BEContextRef context);
BEPluginSiteDataManagerRef BEContextGetPluginSiteDataManager(BEContextRef context);
BEResourceCacheManagerRef BEContextGetResourceCacheManager(BEContextRef context);

void BEContextOpenSnapshotDatabase(BEContextRef context, const double scaleFactor, const int maximumSize);

void BEContextSetDefaultSessionProperty(BEContextRef context, const char* propertyName, const char* value);

void BEContextPruneToPercentage(float targetPercentLive);

#ifdef TVGG_DOCUMENT_VIEWER
void BESetDocViewerStartCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerStart);
void BESetDocViewerFinishCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerFinish);
void BESetDocViewerProgressCallback(BEContextRef contextRef, DocumentViewerCallback beDocViewerProgress);

#endif

#ifdef __cplusplus
}
#endif

#endif /* BEContext_h */
