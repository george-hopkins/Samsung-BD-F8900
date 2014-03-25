#include "config.h"
#include "BEIconDatabase.h"

#include "BEAPICast.h"
#include "WebIconDatabase.h"

using namespace WebKit;

void BEIconDatabaseSetIconDatabaseClient(BEIconDatabaseRef iconDatabaseRef, const BEIconDatabaseClient* client)
{
    if (!client)
        return;

    WKIconDatabaseClient wkClient = {
        0,
        client->clientInfo,
        (WKIconDatabaseDidChangeIconForPageURLCallback) client->didChangeIconForPageURL,
        (WKIconDatabaseDidRemoveAllIconsCallback) client->didRemoveAllIcons
    };
    toBEImpl(iconDatabaseRef)->initializeIconDatabaseClient(&wkClient);
}

void BEIconDatabaseRetainIconForURL(BEIconDatabaseRef iconDatabaseRef, const char* pageURL)
{
    toBEImpl(iconDatabaseRef)->retainIconForPageURL(String::fromUTF8(pageURL));
}

void BEIconDatabaseReleaseIconForURL(BEIconDatabaseRef iconDatabaseRef, const char* pageURL)
{
    toBEImpl(iconDatabaseRef)->releaseIconForPageURL(String::fromUTF8(pageURL));
}

void BEIconDatabaseEnableDatabaseCleanup(BEIconDatabaseRef iconDatabaseRef)
{
    toBEImpl(iconDatabaseRef)->enableDatabaseCleanup();
}

void BEIconDatabaseRemoveAllIcons(BEIconDatabaseRef iconDatabaseRef)
{
    toBEImpl(iconDatabaseRef)->removeAllIcons();
}

void BEIconDatabaseCheckIntegrityBeforeOpening(BEIconDatabaseRef iconDatabaseRef)
{
    toBEImpl(iconDatabaseRef)->checkIntegrityBeforeOpening();
}

void BEIconDatabaseClose(BEIconDatabaseRef iconDatabaseRef)
{
    toBEImpl(iconDatabaseRef)->close();
}
