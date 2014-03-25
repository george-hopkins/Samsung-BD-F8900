/**
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 * @file        BEPage.h
 * @brief       Declaration of BEPage api
 * @author      Sangyong Park (sy302.park@samsung.com)
 * @date        2011.08.23
 */

#ifndef BEPage_h
#define BEPage_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup    BEPage
 * @brief      get page id
 * @param[in]  page                    page reference
 * @return     page id
 */
uint64_t BEPageGetID(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      load url
 * @param[in]  page                    page reference
 * @param[in]  url                     url
 * @return     
 */
void BEPageLoadURL(BEPageRef page, const char* url);

/**
 * @ingroup    BEPage
 * @brief      load url request
 * @param[in]  page                    page reference
 * @param[in]  urlRequest              url request
 * @return     
 */
void BEPageLoadURLRequest(BEPageRef page, BEURLRequestRef urlRequest);

/**
 * @ingroup    BEPage
 * @brief      load html string
 * @param[in]  page                    page reference
 * @param[in]  htmlString              html string
 * @param[in]  baseURL                 baseURL
 * @return     
 */
void BEPageLoadHTMLString(BEPageRef page, const char* htmlString, const char* baseURL);

/**
 * @ingroup    BEPage
 * @brief      load html string instead of unreachableURL page
 * @param[in]  page                    page reference
 * @param[in]  htmlString              html string
 * @param[in]  baseURL                 baseURL
 * @param[in]  unreachabeURL           unreachableURL
 * @return     
 */
void BEPageLoadAlternateHTMLString(BEPageRef page, const char* htmlString, const char* baseURL, const char* unreachableURL);

/**
 * @ingroup    BEPage
 * @brief      load plain text string
 * @param[in]  page                    page reference
 * @param[in]  plainTextString         plain text string
 * @return     
 */
void BEPageLoadPlainTextString(BEPageRef page, const char* plainTextString);

/**
 * @ingroup    BEPage
 * @brief      stop loading
 * @param[in]  page                    page reference
 * @return     
 */
void BEPageStopLoading(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      reload
 * @param[in]  page                    page reference
 * @return     
 */
void BEPageReload(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      reload without cached resource
 * @param[in]  page                    page reference
 * @return     
 */
void BEPageReloadFromOrigin(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      go forward in history
 * @param[in]  page                    page reference
 * @return     
 */
void BEPageGoForward(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get if can go forward in history
 * @param[in]  page                    page reference
 * @return     if can go forward, return true. else return false.
 */
bool BEPageCanGoForward(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      go back in history
 * @param[in]  page                    page reference
 * @return     
 */
void BEPageGoBack(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get if can go back in history
 * @param[in]  page                    page reference
 * @return     if can go back, return true. else return false.
 */
bool BEPageCanGoBack(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      go in history
 * @param[in]  page                    page reference
 * @param[in]  item                    back/forward list item
 * @return     
 */
void BEPageGoToBackForwardListItem(BEPageRef page, BEBackForwardListItemRef item);

/**
 * @ingroup    BEPage
 * @brief      get back/forward list
 * @param[in]  page                    page reference
 * @return     back/forward list
 */
BEBackForwardListRef BEPageGetBackForwardList(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get web page title
 * @param[in]  page                    page reference
 * @return     title
 */
char* BEPageCopyTitle(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get main frame reference
 * @param[in]  page                    page reference
 * @return     main frame reference
 */
BEFrameRef BEPageGetMainFrame(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get loading progress
 * @param[in]  page                    page reference
 * @return     loading progress
 */
double BEPageGetEstimatedProgress(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get custom user agent
 * @param[in]  page                    page reference
 * @return     custom user agent
 */
char* BEPageCopyCustomUserAgent(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      set custom user agent
 * @param[in]  page                    page reference
 * @param[in]  userAgent               custom user agent
 * @return     
 */
void BEPageSetCustomUserAgent(BEPageRef page, const char* userAgent);

/**
 * @ingroup    BEPage
 * @brief      get to support text encoding
 * @param[in]  page                    page reference
 * @return     support text encoding
 */
bool BEPageSupportsTextEncoding(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get custom text encoding name
 * @param[in]  page                    page reference
 * @return     custom text encoding name
 */
char* BEPageCopyCustomTextEncodingName(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      set custom text encoding name
 * @param[in]  page                    page reference
 * @param[in]  encodingName            custom text encoding name
 * @return     
 */
void BEPageSetCustomTextEncodingName(BEPageRef page, const char* encodingName);

/**
 * @ingroup    BEPage
 * @brief      get if supports text zoom
 * @param[in]  page                    page reference
 * @return     supports text zoom
 */
bool BEPageSupportsTextZoom(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get text zoom factor
 * @param[in]  page                    page reference
 * @return     text zoom factor
 */
double BEPageGetTextZoomFactor(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      set text zoom factor
 * @param[in]  page                    page reference
 * @param[in]  zoomFactor              text zoom factor
 * @return     
 */
void BEPageSetTextZoomFactor(BEPageRef page, double zoomFactor);

/**
 * @ingroup    BEPage
 * @brief      get page zoom factor
 * @param[in]  page                    page reference
 * @return     page zoom factor
 */
double BEPageGetPageZoomFactor(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      set page zoom factor
 * @param[in]  page                    page reference
 * @param[in]  zoomFactor              page zoom factor
 * @return     
 */
void BEPageSetPageZoomFactor(BEPageRef page, double zoomFactor);

/**
 * @ingroup    BEPage
 * @brief      set page and text zoom factor
 * @param[in]  page                    page reference
 * @param[in]  pageZoomFactor          page zoom factor
 * @param[in]  textZoomFactor          text zoom factor
 * @return     
 */
void BEPageSetPageAndTextZoomFactors(BEPageRef page, double pageZoomFactor, double textZoomFactor);

/**
 * @ingroup    BEPage
 * @brief      set scale factor
 * @param[in]  page                    page reference
 * @param[in]  scale                   scale factor
 * @param[in]  origin                  origin of web page after apply scale factor
 * @return     
 */
void BEPageSetScaleFactor(BEPageRef page, double scale, BEPoint origin);

/**
 * @ingroup    BEPage
 * @brief      get scale factor
 * @param[in]  page                    page reference
 * @return     scale factor
 */
double BEPageGetScaleFactor(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get node information under cursor (nodeAtCursor was false, focused node used)
 * @param[in]  page                    page reference
 * @param[in]  nodeAtCursor            use node at cursor position
 * @param[out] type                    input type
 * @param[out] value                   current value
 * @param[out] rect                    rectangle of input
 * @return     if success to get information, then return true. else return false.
 */
bool BEPageGetNodeInfo(BEPageRef page, bool nodeAtCursor, char** type, char** value, BEIntRect* rect);

/**
 * @ingroup    BEPage
 * @brief      set focused node value
 * @param[in]  page                    page reference
 * @param[in]  value                   value
 * @param[in]  startPosition           start position
 * @param[in]  endPosition             end position
 * @return     if success to set value, then return true.
 */
bool BEPageSetFocusedNodeValue(BEPageRef page, const char* value, int startPosition, int endPosition);

/**
 * @ingroup    BEPage
 * @brief      set selection of focused node
 * @param[in]  page                    page reference
 * @param[in]  startPosition           start position
 * @param[in]  endPosition             end position
 * @return     if success to set selection, then return true. else return false.
 */
bool BEPageSetFocusedNodeSelection(BEPageRef page, int startPosition, int endPosition);

/**
 * @ingroup    BEPage
 * @brief      get focus rectangle
 * @param[in]  page                    page reference
 * @return     focus rectangle
 */
BERect BEPageGetFocusRect(BEPageRef page);

typedef void (*BEPageRunJavaScriptFunction)(BESerializedScriptValueRef, BEErrorRef, void*);

/**
 * @ingroup    BEPage
 * @brief      run javascript code in main frame
 * @param[in]  page                    page reference
 * @param[in]  script                  javascript code
 * @param[in]  context                 parameter of function
 * @param[in]  function                called after javascript code was run
 * @return     
 */
void BEPageRunJavaScriptInMainFrame(BEPageRef page, const char* script, void* context, BEPageRunJavaScriptFunction function);

/**
 * @ingroup    BEPage
 * @brief      pause javascript
 * @param[in]  page                    page reference
 * @param[in]  paused                  paused
 * @return     
 */
void BEPageSetJavaScriptPaused(BEPageRef page, bool paused);

typedef enum {
    BEScrollUp,
    BEScrollDown,
    BEScrollLeft,
    BEScrollRight
} BEScrollDirection;

typedef enum {
    BEScrollByLine,
    BEScrollByPage,
    BEScrollByDocument,
    BEScrollByPixel
} BEScrollGranularity;

/**
 * @ingroup    BEPage
 * @brief      scroll web page
 * @param[in]  page                    page reference
 * @param[in]  direction               direction
 * @param[in]  granularity             scrolling size
 * @return     
 */
void BEPageScrollBy(BEPageRef page, BEScrollDirection direction, BEScrollGranularity granularity);

/**
 * @ingroup    BEPage
 * @brief      get snapshot of visible content
 * @param[in]  page                    page reference
 * @return     snapshot of visible content
 */
BEImageRef BEPageCreateSnapshotOfVisibleContent(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get icon url of web page
 * @param[in]  page                    page reference
 * @return     icon url
 */
char* BEPageGetIconURL(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      get contents size of web page
 * @param[in]  page                    page reference
 * @return     contents size
 */
BESize BEPageGetContentsSize(BEPageRef page);

/**
 * @ingroup    BEPage
 * @brief      check login form
 * @return     if the current element is login form, return true. else return false.
 */
bool BEPageIsLoginForm(BEPageRef pageRef);

enum {
    BEFocusDirectionBackward = 0,
    BEFocusDirectionForward = 1
};
typedef uint32_t BEFocusDirection;

typedef void (*BEPageCallback)(BEPageRef page, const void* clientInfo);

// FrameLoad Client
typedef void (*BEPageDidStartProvisionalLoadForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidReceiveServerRedirectForProvisionalLoadForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFailProvisionalLoadWithErrorForFrameCallback)(BEPageRef page, BEFrameRef frame, BEErrorRef error, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidCommitLoadForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFinishDocumentLoadForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFinishLoadForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFailLoadWithErrorForFrameCallback)(BEPageRef page, BEFrameRef frame, BEErrorRef error, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidSameDocumentNavigationForFrameCallback)(BEPageRef page, BEFrameRef frame, BESameDocumentNavigationType type, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidReceiveTitleForFrameCallback)(BEPageRef page, BEStringRef title, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFirstLayoutForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidFirstVisuallyNonEmptyLayoutForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidRemoveFrameFromHierarchyCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidDisplayInsecureContentForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDidRunInsecureContentForFrameCallback)(BEPageRef page, BEFrameRef frame, BETypeRef userData, const void* clientInfo);
typedef bool (*BEPageCanAuthenticateAgainstProtectionSpaceInFrameCallback)(BEPageRef page, BEFrameRef frame, BEProtectionSpaceRef protectionSpace, const void* clientInfo);
typedef void (*BEPageDidReceiveAuthenticationChallengeInFrameCallback)(BEPageRef page, BEFrameRef frame, BEAuthenticationChallengeRef authenticationChallenge, const void* clientInfo);
typedef void (*BEPageDidChangeBackForwardListCallback)(BEPageRef page, BEBackForwardListItemRef addedItem, BEArrayRef removedItems, const void* clientInfo);
typedef bool (*BEPageShouldGoToBackForwardListItemCallback)(BEPageRef page, BEBackForwardListItemRef item, const void* clientInfo);
typedef void (*BEPageDidFailToInitializePluginCallback)(BEPageRef page, BEStringRef mimeType, const void* clientInfo);

typedef struct {
    const void *                                                        clientInfo;
    BEPageDidStartProvisionalLoadForFrameCallback                       didStartProvisionalLoadForFrame;
    BEPageDidReceiveServerRedirectForProvisionalLoadForFrameCallback    didReceiveServerRedirectForProvisionalLoadForFrame;
    BEPageDidFailProvisionalLoadWithErrorForFrameCallback               didFailProvisionalLoadWithErrorForFrame;
    BEPageDidCommitLoadForFrameCallback                                 didCommitLoadForFrame;
    BEPageDidFinishDocumentLoadForFrameCallback                         didFinishDocumentLoadForFrame;
    BEPageDidFinishLoadForFrameCallback                                 didFinishLoadForFrame;
    BEPageDidFailLoadWithErrorForFrameCallback                          didFailLoadWithErrorForFrame;
    BEPageDidSameDocumentNavigationForFrameCallback                     didSameDocumentNavigationForFrame;
    BEPageDidReceiveTitleForFrameCallback                               didReceiveTitleForFrame;
    BEPageDidFirstLayoutForFrameCallback                                didFirstLayoutForFrame;
    BEPageDidFirstVisuallyNonEmptyLayoutForFrameCallback                didFirstVisuallyNonEmptyLayoutForFrame;
    BEPageDidRemoveFrameFromHierarchyCallback                           didRemoveFrameFromHierarchy;
    BEPageDidDisplayInsecureContentForFrameCallback                     didDisplayInsecureContentForFrame;
    BEPageDidRunInsecureContentForFrameCallback                         didRunInsecureContentForFrame;
    BEPageCanAuthenticateAgainstProtectionSpaceInFrameCallback          canAuthenticateAgainstProtectionSpaceInFrame;
    BEPageDidReceiveAuthenticationChallengeInFrameCallback              didReceiveAuthenticationChallengeInFrame;

    BEPageCallback                                                      didStartProgress;
    BEPageCallback                                                      didChangeProgress;
    BEPageCallback                                                      didFinishProgress;

    BEPageCallback                                                      processDidBecomeUnresponsive;
    BEPageCallback                                                      processDidBecomeResponsive;
    BEPageCallback                                                      processDidCrash;
    BEPageDidChangeBackForwardListCallback                              didChangeBackForwardList;
    BEPageShouldGoToBackForwardListItemCallback                         shouldGoToBackForwardListItem;
    BEPageDidFailToInitializePluginCallback                             didFailToInitializePlugin;
} BEPageLoaderClient;

/**
 * @ingroup    BEPage
 * @brief      set page loader client
 * @param[in]  page                    page reference
 * @param[in]  client                  page loader client
 * @return     
 */
void BEPageSetPageLoaderClient(BEPageRef page, const BEPageLoaderClient* client);

// Policy Client.
typedef void (*BEPageDecidePolicyForNavigationActionCallback)(BEPageRef page, BEFrameRef frame, BEFrameNavigationType navigationType, BEEventModifiers modifiers, BEEventMouseButton mouseButton, BEURLRequestRef request, BEFramePolicyListenerRef listener, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDecidePolicyForNewWindowActionCallback)(BEPageRef page, BEFrameRef frame, BEFrameNavigationType navigationType, BEEventModifiers modifiers, BEEventMouseButton mouseButton, BEURLRequestRef request, BEStringRef frameName, BEFramePolicyListenerRef listener, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageDecidePolicyForResponseCallback)(BEPageRef page, BEFrameRef frame, BEURLResponseRef response, BEURLRequestRef request, BEFramePolicyListenerRef listener, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageUnableToImplementPolicyCallback)(BEPageRef page, BEFrameRef frame, BEErrorRef error, BETypeRef userData, const void* clientInfo);

typedef struct {
    const void *                                                        clientInfo;
    BEPageDecidePolicyForNavigationActionCallback                       decidePolicyForNavigationAction;
    BEPageDecidePolicyForNewWindowActionCallback                        decidePolicyForNewWindowAction;
    BEPageDecidePolicyForResponseCallback                               decidePolicyForResponse;
    BEPageUnableToImplementPolicyCallback                               unableToImplementPolicy;
} BEPagePolicyClient;

/**
 * @ingroup    BEPage
 * @brief      set page policy client
 * @param[in]  page                    page reference
 * @param[in]  client                  page policy client
 * @return     
 */
void BEPageSetPagePolicyClient(BEPageRef page, const BEPagePolicyClient* client);

// UI Client
typedef BEPageRef (*BEPageCreateNewPageCallback)(BEPageRef page, BEDictionaryRef features, BEEventModifiers modifiers, BEEventMouseButton mouseButton, const void* clientInfo);
typedef void (*BEPageRunJavaScriptAlertCallback)(BEPageRef page, BEStringRef alertText, BEFrameRef frame, const void* clientInfo);
typedef bool (*BEPageRunJavaScriptConfirmCallback)(BEPageRef page, BEStringRef message, BEFrameRef frame, const void* clientInfo);
typedef BEStringRef (*BEPageRunJavaScriptPromptCallback)(BEPageRef page, BEStringRef message, BEStringRef defaultValue, BEFrameRef frame, const void* clientInfo);
typedef void (*BEPageTakeFocusCallback)(BEPageRef page, BEFocusDirection direction, const void* clientInfo);
typedef void (*BEPageFocusCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageUnfocusCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetStatusTextCallback)(BEPageRef page, BEStringRef text, const void* clientInfo);
typedef void (*BEPageMouseDidMoveOverElementCallback)(BEPageRef page, BEEventModifiers modifiers, BETypeRef userData, const void* clientInfo);
typedef void (*BEPageMissingPluginButtonClickedCallback)(BEPageRef page, BEStringRef mimeType, BEStringRef url, BEStringRef pluginsPageURL, const void* clientInfo);
typedef void (*BEPageDidNotHandleKeyEventCallback)(BEPageRef page, BENativeEventPtr event, const void* clientInfo);
typedef void (*BEPageDidNotHandleWheelEventCallback)(BEPageRef page, BENativeEventPtr event, const void* clientInfo);
typedef bool (*BEPageGetToolbarsAreVisibleCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetToolbarsAreVisibleCallback)(BEPageRef page, bool toolbarsVisible, const void* clientInfo);
typedef bool (*BEPageGetMenuBarIsVisibleCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetMenuBarIsVisibleCallback)(BEPageRef page, bool menuBarVisible, const void* clientInfo);
typedef bool (*BEPageGetStatusBarIsVisibleCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetStatusBarIsVisibleCallback)(BEPageRef page, bool statusBarVisible, const void* clientInfo);
typedef bool (*BEPageGetIsResizableCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetIsResizableCallback)(BEPageRef page, bool resizable, const void* clientInfo);
typedef BERect (*BEPageGetWindowFrameCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetWindowFrameCallback)(BEPageRef page, BERect frame, const void* clientInfo);
typedef bool (*BEPageRunBeforeUnloadConfirmPanelCallback)(BEPageRef page, BEStringRef message, BEFrameRef frame, const void* clientInfo);
typedef unsigned long long (*BEPageExceededDatabaseQuotaCallback)(BEPageRef page, BEFrameRef frame, BESecurityOriginRef origin, BEStringRef databaseName, BEStringRef displayName, unsigned long long currentQuota, unsigned long long currentOriginUsage, unsigned long long currentDatabaseUsage, unsigned long long expectedUsage, const void* clientInfo);
typedef void (*BEPageRunOpenPanelCallback)(BEPageRef page, BEFrameRef frame, BEOpenPanelParametersRef parameters, BEOpenPanelResultListenerRef listener, const void* clientInfo);
typedef void (*BEPageDecidePolicyForGeolocationPermissionRequestCallback)(BEPageRef page, BEFrameRef frame, BESecurityOriginRef origin, BEGeolocationPermissionRequestRef permissionRequest, const void* clientInfo);
typedef float (*BEPageHeaderHeightCallback)(BEPageRef page, BEFrameRef frame, const void* clientInfo);
typedef float (*BEPageFooterHeightCallback)(BEPageRef page, BEFrameRef frame, const void* clientInfo);
typedef void (*BEPageDrawHeaderCallback)(BEPageRef page, BEFrameRef frame, BERect rect, const void* clientInfo);
typedef void (*BEPageDrawFooterCallback)(BEPageRef page, BEFrameRef frame, BERect rect, const void* clientInfo);
typedef void (*BEPagePrintFrameCallback)(BEPageRef page, BEFrameRef frame, const void* clientInfo);
typedef void (*BEPageDidCompleteRubberBandForMainFrameCallback)(BEPageRef page, BESize initialOverhang, const void* clientInfo);
typedef void (*BEPageSaveDataToFileInDownloadsFolderCallback)(BEPageRef page, BEStringRef suggestedFilename, BEStringRef mimeType, BEURLRef originatingURL, BEDataRef data, const void* clientInfo);
typedef bool (*BEPageShouldInterruptJavaScriptCallback)(BEPageRef page, const void* clientInfo);
typedef void (*BEPageSetInputMethodStateCallback)(BEPageRef page, bool enabled, const void* clientInfo);
typedef void (*BEPageSetMouseEventStateCallback)(BEPageRef page, bool enabled, const void* clientInfo);
typedef void (*BEPageSetBrowserUIStateCallback)(BEPageRef page, int state, const void* clientInfo);
typedef void (*BEPageShowPopupMenuCallback)(BEPageRef page, BERect rect, int textDirection, const void* items, int selectedIndex, const void* clientInfo);
typedef void (*BEPageHidePopupMenuCallback)(BEPageRef page, const void* clientInfo);

typedef struct {
    const void *                                                        clientInfo;
    BEPageCreateNewPageCallback                                         createNewPage;
    BEPageCallback                                                      showPage;
    BEPageCallback                                                      close;
    BEPageTakeFocusCallback                                             takeFocus;
    BEPageFocusCallback                                                 focus;
    BEPageUnfocusCallback                                               unfocus;
    BEPageRunJavaScriptAlertCallback                                    runJavaScriptAlert;
    BEPageRunJavaScriptConfirmCallback                                  runJavaScriptConfirm;
    BEPageRunJavaScriptPromptCallback                                   runJavaScriptPrompt;
    BEPageSetStatusTextCallback                                         setStatusText;
    BEPageMouseDidMoveOverElementCallback                               mouseDidMoveOverElement;
    BEPageMissingPluginButtonClickedCallback                            missingPluginButtonClicked;
    BEPageDidNotHandleKeyEventCallback                                  didNotHandleKeyEvent;
    BEPageDidNotHandleWheelEventCallback                                didNotHandleWheelEvent;
    BEPageGetToolbarsAreVisibleCallback                                 toolbarsAreVisible;
    BEPageSetToolbarsAreVisibleCallback                                 setToolbarsAreVisible;
    BEPageGetMenuBarIsVisibleCallback                                   menuBarIsVisible;
    BEPageSetMenuBarIsVisibleCallback                                   setMenuBarIsVisible;
    BEPageGetStatusBarIsVisibleCallback                                 statusBarIsVisible;
    BEPageSetStatusBarIsVisibleCallback                                 setStatusBarIsVisible;
    BEPageGetIsResizableCallback                                        isResizable;
    BEPageSetIsResizableCallback                                        setIsResizable;
    BEPageGetWindowFrameCallback                                        getWindowFrame;
    BEPageSetWindowFrameCallback                                        setWindowFrame;
    BEPageRunBeforeUnloadConfirmPanelCallback                           runBeforeUnloadConfirmPanel;
    BEPageCallback                                                      didDraw;
    BEPageCallback                                                      pageDidScroll;
    BEPageExceededDatabaseQuotaCallback                                 exceededDatabaseQuota;
    BEPageRunOpenPanelCallback                                          runOpenPanel;
    BEPageDecidePolicyForGeolocationPermissionRequestCallback           decidePolicyForGeolocationPermissionRequest;
    BEPageHeaderHeightCallback                                          headerHeight;
    BEPageFooterHeightCallback                                          footerHeight;
    BEPageDrawHeaderCallback                                            drawHeader;
    BEPageDrawFooterCallback                                            drawFooter;
    BEPagePrintFrameCallback                                            printFrame;
    BEPageCallback                                                      runModal;
    BEPageDidCompleteRubberBandForMainFrameCallback                     didCompleteRubberBandForMainFrame;
    BEPageSaveDataToFileInDownloadsFolderCallback                       saveDataToFileInDownloadsFolder;
    BEPageShouldInterruptJavaScriptCallback                             shouldInterruptJavaScript;
    BEPageSetInputMethodStateCallback                                   setInputMethodState;
    BEPageSetMouseEventStateCallback                                    setMouseEventState;
    BEPageSetBrowserUIStateCallback                                     setBrowserUIState;
    BEPageShowPopupMenuCallback                                         showPopupMenu;
    BEPageHidePopupMenuCallback                                         hidePopupMenu;
} BEPageUIClient;

/**
 * @ingroup    BEPage
 * @brief      set page ui client
 * @param[in]  page                    page reference
 * @param[in]  client                  page ui client
 * @return     
 */
void BEPageSetPageUIClient(BEPageRef page, const BEPageUIClient* client);

#ifdef __cplusplus
}
#endif

#endif /* BEPage_h */
