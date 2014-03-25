#ifndef BEPreferences_h
#define BEPreferences_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

// Defaults to true.
void BEPreferencesSetJavaScriptEnabled(BEPreferencesRef preferences, bool javaScriptEnabled);
bool BEPreferencesGetJavaScriptEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetLoadsImagesAutomatically(BEPreferencesRef preferences, bool loadsImagesAutomatically);
bool BEPreferencesGetLoadsImagesAutomatically(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetLoadsSiteIconsIgnoringImageLoadingPreference(BEPreferencesRef preferences, bool loadsSiteIconsIgnoringImageLoadingPreference);
bool BEPreferencesGetLoadsSiteIconsIgnoringImageLoadingPreference(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetOfflineWebApplicationCacheEnabled(BEPreferencesRef preferences, bool offlineWebApplicationCacheEnabled);
bool BEPreferencesGetOfflineWebApplicationCacheEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetLocalStorageEnabled(BEPreferencesRef preferences, bool localStorageEnabled);
bool BEPreferencesGetLocalStorageEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetDatabasesEnabled(BEPreferencesRef preferences, bool databasesEnabled);
bool BEPreferencesGetDatabasesEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetXSSAuditorEnabled(BEPreferencesRef preferences, bool xssAuditorEnabled);
bool BEPreferencesGetXSSAuditorEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetFrameFlatteningEnabled(BEPreferencesRef preferences, bool frameFlatteningEnabled);
bool BEPreferencesGetFrameFlatteningEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetPluginsEnabled(BEPreferencesRef preferences, bool pluginsEnabled);
bool BEPreferencesGetPluginsEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetJavaEnabled(BEPreferencesRef preferences, bool javaEnabled);
bool BEPreferencesGetJavaEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetJavaScriptCanOpenWindowsAutomatically(BEPreferencesRef preferences, bool javaScriptCanOpenWindowsAutomatically);
bool BEPreferencesGetJavaScriptCanOpenWindowsAutomatically(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetHyperlinkAuditingEnabled(BEPreferencesRef preferences, bool hyperlinkAuditingEnabled);
bool BEPreferencesGetHyperlinkAuditingEnabled(BEPreferencesRef preferences);

void BEPreferencesSetStandardFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopyStandardFontFamily(BEPreferencesRef preferencesRef);

void BEPreferencesSetFixedFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopyFixedFontFamily(BEPreferencesRef preferencesRef);

void BEPreferencesSetSerifFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopySerifFontFamily(BEPreferencesRef preferencesRef);

void BEPreferencesSetSansSerifFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopySansSerifFontFamily(BEPreferencesRef preferencesRef);

void BEPreferencesSetCursiveFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopyCursiveFontFamily(BEPreferencesRef preferencesRef);

void BEPreferencesSetFantasyFontFamily(BEPreferencesRef preferencesRef, const char* family);
char* BEPreferencesCopyFantasyFontFamily(BEPreferencesRef preferencesRef);

// Defaults to 16.
void BEPreferencesSetDefaultFontSize(BEPreferencesRef preferencesRef, uint32_t);
uint32_t BEPreferencesGetDefaultFontSize(BEPreferencesRef preferencesRef);

// Defaults to 13.
void BEPreferencesSetDefaultFixedFontSize(BEPreferencesRef preferencesRef, uint32_t);
uint32_t BEPreferencesGetDefaultFixedFontSize(BEPreferencesRef preferencesRef);

// Defaults to 0.
void BEPreferencesSetMinimumFontSize(BEPreferencesRef preferencesRef, uint32_t);
uint32_t BEPreferencesGetMinimumFontSize(BEPreferencesRef preferencesRef);

void BEPreferencesSetDefaultTextEncodingName(BEPreferencesRef preferencesRef, const char* name);
char* BEPreferencesCopyDefaultTextEncodingName(BEPreferencesRef preferencesRef);

// Defaults to false.
void BEPreferencesSetPrivateBrowsingEnabled(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetPrivateBrowsingEnabled(BEPreferencesRef preferencesRef);

// Defaults to false.
void BEPreferencesSetDeveloperExtrasEnabled(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetDeveloperExtrasEnabled(BEPreferencesRef preferencesRef);

// Defaults to true.
void BEPreferencesSetTextAreasAreResizable(BEPreferencesRef preferencesRef, bool resizable);
bool BEPreferencesGetTextAreasAreResizable(BEPreferencesRef preferencesRef);

// Defaults to false.
void BEPreferencesSetTabsToLinks(BEPreferencesRef preferences, bool tabsToLinks);
bool BEPreferencesGetTabsToLinks(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetDNSPrefetchingEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetDNSPrefetchingEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetAuthorAndUserStylesEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetAuthorAndUserStylesEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetShouldPrintBackgrounds(BEPreferencesRef preferences, bool shouldPrintBackgrounds);
bool BEPreferencesGetShouldPrintBackgrounds(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetJavaScriptCanAccessClipboard(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetJavaScriptCanAccessClipboard(BEPreferencesRef preferencesRef);

// Defaults to false
void BEPreferencesSetFullScreenEnabled(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetFullScreenEnabled(BEPreferencesRef preferencesRef);

// Defaults to true.
void BEPreferencesSetAVFoundationEnabled(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetAVFoundationEnabled(BEPreferencesRef preferencesRef);

// private api

typedef enum {
    BEFontSmoothingLevelNoSubpixelAntiAliasing = 0,
    BEFontSmoothingLevelLight = 1,
    BEFontSmoothingLevelMedium = 2,
    BEFontSmoothingLevelStrong = 3,
#if defined(WIN32) || defined(_WIN32)
    BEFontSmoothingLevelWindows = 4,
#endif
} BEFontSmoothingLevel;

typedef enum {
    BEEditableLinkBehaviorDefault,
    BEEditableLinkBehaviorAlwaysLive,
    BEEditableLinkBehaviorOnlyLiveWithShiftKey,
    BEEditableLinkBehaviorLiveWhenNotFocused,
    BEEditableLinkBehaviorNeverLive
} BEEditableLinkBehavior;

// Defaults to BEFontSmoothingLevelMedium.
void BEPreferencesSetFontSmoothingLevel(BEPreferencesRef, BEFontSmoothingLevel);
BEFontSmoothingLevel BEPreferencesGetFontSmoothingLevel(BEPreferencesRef);

// Defaults to EditableLinkNeverLive.
void BEPreferencesSetEditableLinkBehavior(BEPreferencesRef preferencesRef, BEEditableLinkBehavior);
BEEditableLinkBehavior BEPreferencesGetEditableLinkBehavior(BEPreferencesRef preferencesRef);

// Defaults to false.
void BEPreferencesSetAcceleratedDrawingEnabled(BEPreferencesRef, bool);
bool BEPreferencesGetAcceleratedDrawingEnabled(BEPreferencesRef);

// Defaults to true.
void BEPreferencesSetCanvasUsesAcceleratedDrawing(BEPreferencesRef, bool);
bool BEPreferencesGetCanvasUsesAcceleratedDrawing(BEPreferencesRef);

// Defaults to true.
void BEPreferencesSetAcceleratedCompositingEnabled(BEPreferencesRef, bool);
bool BEPreferencesGetAcceleratedCompositingEnabled(BEPreferencesRef);

// Defaults to false.
void BEPreferencesSetCompositingBordersVisible(BEPreferencesRef, bool);
bool BEPreferencesGetCompositingBordersVisible(BEPreferencesRef);

// Defaults to false.
void BEPreferencesSetCompositingRepaintCountersVisible(BEPreferencesRef, bool);
bool BEPreferencesGetCompositingRepaintCountersVisible(BEPreferencesRef);

// Defaults to false.
void BEPreferencesSetWebGLEnabled(BEPreferencesRef, bool);
bool BEPreferencesGetWebGLEnabled(BEPreferencesRef);

// Defaults to false.
void BEPreferencesSetNeedsSiteSpecificQuirks(BEPreferencesRef, bool);
bool BEPreferencesGetNeedsSiteSpecificQuirks(BEPreferencesRef);

// Defaults to false.
void BEPreferencesSetForceFTPDirectoryListings(BEPreferencesRef preferences, bool force);
bool BEPreferencesGetForceFTPDirectoryListings(BEPreferencesRef preferences);

// Defaults to the empty string.
void BEPreferencesSetFTPDirectoryTemplatePath(BEPreferencesRef preferences, const char* path);
char* BEPreferencesCopyFTPDirectoryTemplatePath(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetWebArchiveDebugModeEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetWebArchiveDebugModeEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetLocalFileContentSniffingEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetLocalFileContentSniffingEnabled(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetPageCacheEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetPageCacheEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetPaginateDuringLayoutEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetPaginateDuringLayoutEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetDOMPasteAllowed(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetDOMPasteAllowed(BEPreferencesRef preferences);

// Defaults to true.
void BEPreferencesSetWebSecurityEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetWebSecurityEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetUniversalAccessFromFileURLsAllowed(BEPreferencesRef preferences, bool allowed);
bool BEPreferencesGetUniversalAccessFromFileURLsAllowed(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetFileAccessFromFileURLsAllowed(BEPreferencesRef preferences, bool allowed);
bool BEPreferencesGetFileAccessFromFileURLsAllowed(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetSpatialNavigationEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetSpatialNavigationEnabled(BEPreferencesRef preferences);

// Defaults to false.
void BEPreferencesSetTVUIEnabled(BEPreferencesRef preferences, bool enabled);
bool BEPreferencesGetTVUIEnabled(BEPreferencesRef preferences);

#ifdef ENABLE_HBBTV
// Defaults to empty string.
void BEPreferencesSetHbbTVAppBoundary(BEPreferencesRef preferencesRef, const char* boundary);
char* BEPreferencesGetHbbTVAppBoundary(BEPreferencesRef preferencesRef);

void BEPreferencesSetHbbTVEnabled(BEPreferencesRef preferencesRef, bool enabled);
bool BEPreferencesGetHbbTVEnabled(BEPreferencesRef preferencesRef);
#endif

#ifdef __cplusplus
}
#endif

#endif /* BEPreferences_h */
