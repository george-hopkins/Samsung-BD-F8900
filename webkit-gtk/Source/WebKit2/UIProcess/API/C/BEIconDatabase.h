#ifndef BEIconDatabase_h
#define BEIconDatabase_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

// IconDatabase Client.
typedef void (*BEIconDatabaseDidChangeIconForPageURLCallback)(BEIconDatabaseRef iconDatabase, BEURLRef pageURL, const void* clientInfo);
typedef void (*BEIconDatabaseDidRemoveAllIconsCallback)(BEIconDatabaseRef iconDatabase, const void* clientInfo);

typedef struct BEIconDatabaseClient {
    const void *                                                        clientInfo;
    BEIconDatabaseDidChangeIconForPageURLCallback                       didChangeIconForPageURL;
    BEIconDatabaseDidRemoveAllIconsCallback                             didRemoveAllIcons;
} BEIconDatabaseClient;

void BEIconDatabaseSetIconDatabaseClient(BEIconDatabaseRef iconDatabase, const BEIconDatabaseClient* client);

void BEIconDatabaseRetainIconForURL(BEIconDatabaseRef iconDatabase, const char* pageURL);
void BEIconDatabaseReleaseIconForURL(BEIconDatabaseRef iconDatabase, const char* pageURL);
void BEIconDatabaseEnableDatabaseCleanup(BEIconDatabaseRef iconDatabase);

void BEIconDatabaseRemoveAllIcons(BEIconDatabaseRef iconDatabase);
void BEIconDatabaseCheckIntegrityBeforeOpening(BEIconDatabaseRef iconDatabase);

void BEIconDatabaseClose(BEIconDatabaseRef iconDatabase);

#ifdef __cplusplus
}
#endif

#endif /* BEIconDatabase_h */
