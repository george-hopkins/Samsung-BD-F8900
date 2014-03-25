#include "config.h"
#include "BEPreferences.h"
#include "BEAPICast.h"
#include "WebPreferences.h"
#include <WebCore/Settings.h>
#ifdef ENABLE_HBBTV
#include <WebCore/dom/Node.h>
#endif

using namespace WebKit;

void BEPreferencesSetJavaScriptEnabled(BEPreferencesRef preferencesRef, bool javaScriptEnabled)
{
    toBEImpl(preferencesRef)->setJavaScriptEnabled(javaScriptEnabled);
}

bool BEPreferencesGetJavaScriptEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->javaScriptEnabled();
}

void BEPreferencesSetLoadsImagesAutomatically(BEPreferencesRef preferencesRef, bool loadsImagesAutomatically)
{
    toBEImpl(preferencesRef)->setLoadsImagesAutomatically(loadsImagesAutomatically);
}

bool BEPreferencesGetLoadsImagesAutomatically(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->loadsImagesAutomatically();
}

void BEPreferencesSetLoadsSiteIconsIgnoringImageLoadingPreference(BEPreferencesRef preferencesRef, bool loadsSiteIconsIgnoringImageLoadingPreference)
{
    toBEImpl(preferencesRef)->setLoadsSiteIconsIgnoringImageLoadingPreference(loadsSiteIconsIgnoringImageLoadingPreference);
}

bool BEPreferencesGetLoadsSiteIconsIgnoringImageLoadingPreference(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->loadsSiteIconsIgnoringImageLoadingPreference();
}

void BEPreferencesSetOfflineWebApplicationCacheEnabled(BEPreferencesRef preferencesRef, bool offlineWebApplicationCacheEnabled)
{
    toBEImpl(preferencesRef)->setOfflineWebApplicationCacheEnabled(offlineWebApplicationCacheEnabled);
}

bool BEPreferencesGetOfflineWebApplicationCacheEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->offlineWebApplicationCacheEnabled();
}

void BEPreferencesSetLocalStorageEnabled(BEPreferencesRef preferencesRef, bool localStorageEnabled)
{
    toBEImpl(preferencesRef)->setLocalStorageEnabled(localStorageEnabled);
}

bool BEPreferencesGetLocalStorageEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->localStorageEnabled();
}

void BEPreferencesSetDatabasesEnabled(BEPreferencesRef preferencesRef, bool databasesEnabled)
{
    toBEImpl(preferencesRef)->setDatabasesEnabled(databasesEnabled);
}

bool BEPreferencesGetDatabasesEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->databasesEnabled();
}

void BEPreferencesSetXSSAuditorEnabled(BEPreferencesRef preferencesRef, bool xssAuditorEnabled)
{
    toBEImpl(preferencesRef)->setXSSAuditorEnabled(xssAuditorEnabled);
}

bool BEPreferencesGetXSSAuditorEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->xssAuditorEnabled();
}

void BEPreferencesSetFrameFlatteningEnabled(BEPreferencesRef preferencesRef, bool frameFlatteningEnabled)
{
    toBEImpl(preferencesRef)->setFrameFlatteningEnabled(frameFlatteningEnabled);
}

bool BEPreferencesGetFrameFlatteningEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->frameFlatteningEnabled();
}

void BEPreferencesSetPluginsEnabled(BEPreferencesRef preferencesRef, bool pluginsEnabled)
{
    toBEImpl(preferencesRef)->setPluginsEnabled(pluginsEnabled);
}

bool BEPreferencesGetPluginsEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->pluginsEnabled();
}

void BEPreferencesSetJavaEnabled(BEPreferencesRef preferencesRef, bool javaEnabled)
{
    toBEImpl(preferencesRef)->setJavaEnabled(javaEnabled);
}

bool BEPreferencesGetJavaEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->javaEnabled();
}

void BEPreferencesSetJavaScriptCanOpenWindowsAutomatically(BEPreferencesRef preferencesRef, bool javaScriptCanOpenWindowsAutomatically)
{
    toBEImpl(preferencesRef)->setJavaScriptCanOpenWindowsAutomatically(javaScriptCanOpenWindowsAutomatically);
}

bool BEPreferencesGetJavaScriptCanOpenWindowsAutomatically(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->javaScriptCanOpenWindowsAutomatically();
}

void BEPreferencesSetHyperlinkAuditingEnabled(BEPreferencesRef preferencesRef, bool hyperlinkAuditingEnabled)
{
    toBEImpl(preferencesRef)->setHyperlinkAuditingEnabled(hyperlinkAuditingEnabled);
}

bool BEPreferencesGetHyperlinkAuditingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->hyperlinkAuditingEnabled();
}

void BEPreferencesSetStandardFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setStandardFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopyStandardFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->standardFontFamily());
}

void BEPreferencesSetFixedFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setFixedFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopyFixedFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->fixedFontFamily());
}

void BEPreferencesSetSerifFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setSerifFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopySerifFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->serifFontFamily());
}

void BEPreferencesSetSansSerifFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setSansSerifFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopySansSerifFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->sansSerifFontFamily());
}

void BEPreferencesSetCursiveFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setCursiveFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopyCursiveFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->cursiveFontFamily());
}

void BEPreferencesSetFantasyFontFamily(BEPreferencesRef preferencesRef, const char* family)
{
    toBEImpl(preferencesRef)->setFantasyFontFamily(String::fromUTF8(family));
}

char* BEPreferencesCopyFantasyFontFamily(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->fantasyFontFamily());
}

void BEPreferencesSetDefaultFontSize(BEPreferencesRef preferencesRef, uint32_t size)
{
    toBEImpl(preferencesRef)->setDefaultFontSize(size);
}

uint32_t BEPreferencesGetDefaultFontSize(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->defaultFontSize();
}

void BEPreferencesSetDefaultFixedFontSize(BEPreferencesRef preferencesRef, uint32_t size)
{
    toBEImpl(preferencesRef)->setDefaultFixedFontSize(size);
}

uint32_t BEPreferencesGetDefaultFixedFontSize(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->defaultFixedFontSize();
}

void BEPreferencesSetMinimumFontSize(BEPreferencesRef preferencesRef, uint32_t size)
{
    toBEImpl(preferencesRef)->setMinimumFontSize(size);
}

uint32_t BEPreferencesGetMinimumFontSize(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->minimumFontSize();
}

static inline WebCore::EditableLinkBehavior toEditableLinkBehavior(BEEditableLinkBehavior behavior)
{
    switch (behavior) {
    case BEEditableLinkBehaviorDefault:
        return WebCore::EditableLinkDefaultBehavior;
    case BEEditableLinkBehaviorAlwaysLive:
        return WebCore::EditableLinkAlwaysLive;
    case BEEditableLinkBehaviorOnlyLiveWithShiftKey:
        return WebCore::EditableLinkOnlyLiveWithShiftKey;
    case BEEditableLinkBehaviorLiveWhenNotFocused:
        return WebCore::EditableLinkLiveWhenNotFocused;
    case BEEditableLinkBehaviorNeverLive:
        return WebCore::EditableLinkNeverLive;
    }

    ASSERT_NOT_REACHED();
    return WebCore::EditableLinkNeverLive;
}

void BEPreferencesSetEditableLinkBehavior(BEPreferencesRef preferencesRef, BEEditableLinkBehavior wkBehavior)
{
    toBEImpl(preferencesRef)->setEditableLinkBehavior(toEditableLinkBehavior(wkBehavior));
}

static inline BEEditableLinkBehavior toBEAPI(WebCore::EditableLinkBehavior behavior)
{
    switch (behavior) {
    case WebCore::EditableLinkDefaultBehavior:
        return BEEditableLinkBehaviorDefault;
    case WebCore::EditableLinkAlwaysLive:
        return BEEditableLinkBehaviorAlwaysLive;
    case WebCore::EditableLinkOnlyLiveWithShiftKey:
        return BEEditableLinkBehaviorOnlyLiveWithShiftKey;
    case WebCore::EditableLinkLiveWhenNotFocused:
        return BEEditableLinkBehaviorLiveWhenNotFocused;
    case WebCore::EditableLinkNeverLive:
        return BEEditableLinkBehaviorNeverLive;
    }

    ASSERT_NOT_REACHED();
    return BEEditableLinkBehaviorNeverLive;
}

BEEditableLinkBehavior BEPreferencesGetEditableLinkBehavior(BEPreferencesRef preferencesRef)
{
    return toBEAPI(static_cast<WebCore::EditableLinkBehavior>(toBEImpl(preferencesRef)->editableLinkBehavior()));
}

void BEPreferencesSetDefaultTextEncodingName(BEPreferencesRef preferencesRef, const char* name)
{
    toBEImpl(preferencesRef)->setDefaultTextEncodingName(String::fromUTF8(name));
}

char* BEPreferencesCopyDefaultTextEncodingName(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->defaultTextEncodingName());
}

void BEPreferencesSetPrivateBrowsingEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setPrivateBrowsingEnabled(enabled);
}

bool BEPreferencesGetPrivateBrowsingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->privateBrowsingEnabled();
}

void BEPreferencesSetDeveloperExtrasEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setDeveloperExtrasEnabled(enabled);
}

bool BEPreferencesGetDeveloperExtrasEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->developerExtrasEnabled();
}

void BEPreferencesSetTextAreasAreResizable(BEPreferencesRef preferencesRef, bool resizable)
{
    toBEImpl(preferencesRef)->setTextAreasAreResizable(resizable);
}

bool BEPreferencesGetTextAreasAreResizable(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->textAreasAreResizable();
}

static inline FontSmoothingLevel toFontSmoothingLevel(BEFontSmoothingLevel level)
{
    switch (level) {
    case BEFontSmoothingLevelNoSubpixelAntiAliasing:
        return FontSmoothingLevelNoSubpixelAntiAliasing;
    case BEFontSmoothingLevelLight:
        return FontSmoothingLevelLight;
    case BEFontSmoothingLevelMedium:
        return FontSmoothingLevelMedium;
    case BEFontSmoothingLevelStrong:
        return FontSmoothingLevelStrong;
#if PLATFORM(WIN)
    case BEFontSmoothingLevelWindows:
        return FontSmoothingLevelWindows;
#endif
    }

    ASSERT_NOT_REACHED();
    return FontSmoothingLevelMedium;
}

void BEPreferencesSetFontSmoothingLevel(BEPreferencesRef preferencesRef, BEFontSmoothingLevel wkLevel)
{
    toBEImpl(preferencesRef)->setFontSmoothingLevel(toFontSmoothingLevel(wkLevel));
}

static inline BEFontSmoothingLevel toBEAPI(FontSmoothingLevel level)
{
    switch (level) {
    case FontSmoothingLevelNoSubpixelAntiAliasing:
        return BEFontSmoothingLevelNoSubpixelAntiAliasing;
    case FontSmoothingLevelLight:
        return BEFontSmoothingLevelLight;
    case FontSmoothingLevelMedium:
        return BEFontSmoothingLevelMedium;
    case FontSmoothingLevelStrong:
        return BEFontSmoothingLevelStrong;
#if PLATFORM(WIN)
    case FontSmoothingLevelWindows:
        return BEFontSmoothingLevelWindows;
#endif
    }

    ASSERT_NOT_REACHED();
    return BEFontSmoothingLevelMedium;
}

BEFontSmoothingLevel BEPreferencesGetFontSmoothingLevel(BEPreferencesRef preferencesRef)
{
    return toBEAPI(static_cast<FontSmoothingLevel>(toBEImpl(preferencesRef)->fontSmoothingLevel()));
}

void BEPreferencesSetAcceleratedDrawingEnabled(BEPreferencesRef preferencesRef, bool flag)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    toBEImpl(preferencesRef)->setAcceleratedDrawingEnabled(flag);
    toBEImpl(preferencesRef)->setAcceleratedCompositingEnabled(flag);
#endif
}

bool BEPreferencesGetAcceleratedDrawingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->acceleratedDrawingEnabled();
}

void BEPreferencesSetCanvasUsesAcceleratedDrawing(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setCanvasUsesAcceleratedDrawing(flag);
}

bool BEPreferencesGetCanvasUsesAcceleratedDrawing(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->canvasUsesAcceleratedDrawing();
}

void BEPreferencesSetAcceleratedCompositingEnabled(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setAcceleratedCompositingEnabled(flag);
}

bool BEPreferencesGetAcceleratedCompositingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->acceleratedCompositingEnabled();
}

void BEPreferencesSetCompositingBordersVisible(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setCompositingBordersVisible(flag);
}

bool BEPreferencesGetCompositingBordersVisible(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->compositingBordersVisible();
}

void BEPreferencesSetCompositingRepaintCountersVisible(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setCompositingRepaintCountersVisible(flag);
}

bool BEPreferencesGetCompositingRepaintCountersVisible(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->compositingRepaintCountersVisible();
}

void BEPreferencesSetWebGLEnabled(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setWebGLEnabled(flag);
}

bool BEPreferencesGetWebGLEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->webGLEnabled();
}

void BEPreferencesSetNeedsSiteSpecificQuirks(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setNeedsSiteSpecificQuirks(flag);
}

bool BEPreferencesGetNeedsSiteSpecificQuirks(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->needsSiteSpecificQuirks();
}

void BEPreferencesSetForceFTPDirectoryListings(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setForceFTPDirectoryListings(flag);
}

bool BEPreferencesGetForceFTPDirectoryListings(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->forceFTPDirectoryListings();
}

void BEPreferencesSetFTPDirectoryTemplatePath(BEPreferencesRef preferencesRef, const char* pathRef)
{
    toBEImpl(preferencesRef)->setFTPDirectoryTemplatePath(String::fromUTF8(pathRef));
}

char* BEPreferencesCopyFTPDirectoryTemplatePath(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->ftpDirectoryTemplatePath());
}

void BEPreferencesSetTabsToLinks(BEPreferencesRef preferencesRef, bool tabsToLinks)
{
    toBEImpl(preferencesRef)->setTabsToLinks(tabsToLinks);
}

bool BEPreferencesGetTabsToLinks(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->tabsToLinks();
}

void BEPreferencesSetDNSPrefetchingEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setDNSPrefetchingEnabled(enabled);
}

bool BEPreferencesGetDNSPrefetchingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->dnsPrefetchingEnabled();
}

void BEPreferencesSetAuthorAndUserStylesEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setAuthorAndUserStylesEnabled(enabled);
}

bool BEPreferencesGetAuthorAndUserStylesEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->authorAndUserStylesEnabled();
}

void BEPreferencesSetShouldPrintBackgrounds(BEPreferencesRef preferencesRef, bool flag)
{
    toBEImpl(preferencesRef)->setShouldPrintBackgrounds(flag);
}

bool BEPreferencesGetShouldPrintBackgrounds(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->shouldPrintBackgrounds();
}

void BEPreferencesSetWebArchiveDebugModeEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setWebArchiveDebugModeEnabled(enabled);
}

bool BEPreferencesGetWebArchiveDebugModeEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->webArchiveDebugModeEnabled();
}

void BEPreferencesSetLocalFileContentSniffingEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setLocalFileContentSniffingEnabled(enabled);
}

bool BEPreferencesGetLocalFileContentSniffingEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->localFileContentSniffingEnabled();
}

void BEPreferencesSetPageCacheEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setUsesPageCache(enabled);
}

bool BEPreferencesGetPageCacheEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->usesPageCache();
}

void BEPreferencesSetPaginateDuringLayoutEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setPaginateDuringLayoutEnabled(enabled);
}

bool BEPreferencesGetPaginateDuringLayoutEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->paginateDuringLayoutEnabled();
}

void BEPreferencesSetDOMPasteAllowed(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setDOMPasteAllowed(enabled);
}

bool BEPreferencesGetDOMPasteAllowed(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->domPasteAllowed();
}

void BEPreferencesSetJavaScriptCanAccessClipboard(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setJavaScriptCanAccessClipboard(enabled);
}

bool BEPreferencesGetJavaScriptCanAccessClipboard(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->javaScriptCanAccessClipboard();
}

void BEPreferencesSetFullScreenEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setFullScreenEnabled(enabled);
}

bool BEPreferencesGetFullScreenEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->fullScreenEnabled();
}

void BEPreferencesSetAVFoundationEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setAVFoundationEnabled(enabled);
}

bool BEPreferencesGetAVFoundationEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->isAVFoundationEnabled();
}

void BEPreferencesSetWebSecurityEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setWebSecurityEnabled(enabled);
}

bool BEPreferencesGetWebSecurityEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->webSecurityEnabled();
}

void BEPreferencesSetUniversalAccessFromFileURLsAllowed(BEPreferencesRef preferencesRef, bool allowed)
{
    toBEImpl(preferencesRef)->setAllowUniversalAccessFromFileURLs(allowed);
}

bool BEPreferencesGetUniversalAccessFromFileURLsAllowed(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->allowUniversalAccessFromFileURLs();
}

void BEPreferencesSetFileAccessFromFileURLsAllowed(BEPreferencesRef preferencesRef, bool allowed)
{
    toBEImpl(preferencesRef)->setAllowFileAccessFromFileURLs(allowed);
}

bool BEPreferencesGetFileAccessFromFileURLsAllowed(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->allowFileAccessFromFileURLs();
}

void BEPreferencesSetSpatialNavigationEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setSpatialNavigationEnabled(enabled);
}

bool BEPreferencesGetSpatialNavigationEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->isSpatialNavigationEnabled();
}

void BEPreferencesSetTVUIEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setTVUIEnabled(enabled);
}

bool BEPreferencesGetTVUIEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->isTVUIEnabled();
}

#ifdef ENABLE_HBBTV
void BEPreferencesSetHbbTVAppBoundary(BEPreferencesRef preferencesRef, const char* boundary)
{
    toBEImpl(preferencesRef)->setHbbTVAppBoundary(String::fromUTF8(boundary));
    WebCore::Node::s_flagReattachForHbbTV = true;
}

char* BEPreferencesGetHbbTVAppBoundary(BEPreferencesRef preferencesRef)
{
    return toCopiedBEAPI(toBEImpl(preferencesRef)->hbbTVAppBoundary());
}

void BEPreferencesSetHbbTVEnabled(BEPreferencesRef preferencesRef, bool enabled)
{
    toBEImpl(preferencesRef)->setHbbTVEnabled(enabled);
    WebCore::Node::s_flagReattachForHbbTV = true;
}

bool BEPreferencesGetHbbTVEnabled(BEPreferencesRef preferencesRef)
{
    return toBEImpl(preferencesRef)->isHbbTVEnabled();
}
#endif
