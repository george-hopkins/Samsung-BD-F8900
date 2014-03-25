#ifndef BEBase_h
#define BEBase_h

#ifdef __cplusplus
extern "C" {
#endif

// webkit2 wrapping types
typedef uint32_t BETypeID;
typedef const void* BETypeRef;

typedef const struct BEArray* BEArrayRef;
typedef const struct BEData* BEDataRef;
typedef const struct BEError* BEErrorRef;
typedef const struct BESecurityOrigin* BESecurityOriginRef;
typedef const struct BESerializedScriptValue* BESerializedScriptValueRef;
typedef const struct BEString* BEStringRef;
typedef const struct BEURL* BEURLRef;
typedef const struct BEURLRequest* BEURLRequestRef;
typedef const struct BEURLResponse* BEURLResponseRef;

typedef const struct BEApplicationCacheManager* BEApplicationCacheManagerRef;
typedef const struct BEAuthenticationChallenge* BEAuthenticationChallengeRef;
typedef const struct BEBackForwardList* BEBackForwardListRef;
typedef const struct BEBackForwardListItem* BEBackForwardListItemRef;
typedef const struct BEContext* BEContextRef;
typedef const struct BECookieManager* BECookieManagerRef;
typedef const struct BEDatabaseManager* BEDatabaseManagerRef;
typedef const struct BEDictionary* BEDictionaryRef;
typedef const struct BEFrame* BEFrameRef;
typedef const struct BEFramePolicyListener* BEFramePolicyListenerRef;
typedef const struct BEGeolocationManager* BEGeolocationManagerRef;
typedef const struct BEGeolocationPermissionRequest* BEGeolocationPermissionRequestRef;
typedef const struct BEIconDatabase* BEIconDatabaseRef;
typedef const struct BEImage* BEImageRef;
typedef const struct BEKeyValueStorageManager* BEKeyValueStorageManagerRef;
typedef const struct BEMediaCacheManager* BEMediaCacheManagerRef;
typedef const struct BEOpenPanelParameters* BEOpenPanelParametersRef;
typedef const struct BEOpenPanelResultListener* BEOpenPanelResultListenerRef;
typedef const struct BEPage* BEPageRef;
typedef const struct BEPageGroup* BEPageGroupRef;
typedef const struct BEPluginSiteDataManager* BEPluginSiteDataManagerRef;
typedef const struct BEPreferences* BEPreferencesRef;
typedef const struct BEProtectionSpace* BEProtectionSpaceRef;
typedef const struct BEResourceCacheManager* BEResourceCacheManagerRef;
typedef const struct BEView* BEViewRef;

enum {
    BEEventModifiersShiftKey = 1 << 0,
    BEEventModifiersControlKey = 1 << 1,
    BEEventModifiersAltKey = 1 << 2,
    BEEventModifiersMetaKey = 1 << 3
};
typedef uint32_t BEEventModifiers;

enum {
    BEEventMouseButtonNoButton = -1,
    BEEventMouseButtonLeftButton = 0,
    BEEventMouseButtonMiddleButton = 1,
    BEEventMouseButtonRightButton = 2,
};
typedef int32_t BEEventMouseButton;

enum {
    BEFrameNavigationTypeLinkClicked = 0,
    BEFrameNavigationTypeFormSubmitted = 1,
    BEFrameNavigationTypeBackForward = 2,
    BEFrameNavigationTypeReload = 3,
    BEFrameNavigationTypeFormResubmitted = 4,
    BEFrameNavigationTypeOther = 5
};
typedef uint32_t BEFrameNavigationType;

enum {
    BEBrowserUIStateCreateView = 0,
    BEBrowserUIStatePointerVerySlow = 1,
    BEBrowserUIStatePointerSlow = 2,
    BEBrowserUIStatePointerStandard = 3,
    BEBrowserUIStatePointerFast = 4,
    BEBrowserUIStatePointerVeryFast = 5,
    BEBrowserUIStateDragModeOn = 6,
    BEBrowserUIStateDragModeOff = 7
};
typedef uint32_t BEBrowserUIState;

typedef const void* BENativeEventPtr;

typedef uint32_t BESameDocumentNavigationType;

// be defined types
typedef void* BEPlatformEventRef;
typedef struct _cairo* BEPlatformGraphicsContextRef;
typedef const void* BEPopupMenuItemsRef;

typedef struct {
    int x;
    int y;
} BEPoint;

typedef struct {
    int width;
    int height;
} BESize;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} BEIntRect;

typedef struct {
    double x;
    double y;
    double width;
    double height;
} BERect;

BETypeID BEGetTypeID(BETypeRef type);

BETypeRef BERetain(BETypeRef type);
void BERelease(BETypeRef type);

BEStringRef BEStringCreateWithUTF8CString(const char* string);

size_t BEStringGetMaximumUTF8CStringSize(BEStringRef string);
size_t BEStringGetUTF8CString(BEStringRef string, char* buffer, size_t bufferSize);

char* BEURLRequestCopyURL(BEURLRequestRef request);
char* BEURLResponseCopyMimeType(BEURLResponseRef response);

enum {
    BEErrorCodeCannotShowMIMEType =                             100,
    BEErrorCodeCannotShowURL =                                  101,
    BEErrorCodeFrameLoadInterruptedByPolicyChange =             102,
    BEErrorCodeCannotUseRestrictedPort =                        103,
    BEErrorCodeCannotFindPlugIn =                               200,
    BEErrorCodeCannotLoadPlugIn =                               201,
    BEErrorCodeJavaUnavailable =                                202,
    BEErrorCodePlugInCancelledConnection =                      203,
    BEErrorCodePlugInWillHandleLoad =                           204,
};
typedef uint32_t BEErrorCode;

char* BEErrorCopyDomain(BEErrorRef error);
int BEErrorGetErrorCode(BEErrorRef error);
char* BEErrorCopyFailingURL(BEErrorRef error);

char* BESerializedScriptValueToString(BESerializedScriptValueRef scriptValue);

typedef struct _cairo_surface cairo_surface_t;
cairo_surface_t* BEImageCreateCairoSurface(BEImageRef image);

void BESetLogLevel(unsigned int level);

#ifdef __cplusplus
}
#endif

#endif /* BEBase_h */
