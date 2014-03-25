#include "config.h"
#include "BEPage.h"

#include "BEAPICast.h"
#include "BEView.h"
#include "WebImage.h"
#include "WebPageProxy.h"

using namespace WebKit;

uint64_t BEPageGetID(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->pageID();
}

void BEPageLoadURL(BEPageRef pageRef, const char* url)
{
    toBEImpl(pageRef)->loadURL(String::fromUTF8(url));
}

void BEPageLoadURLRequest(BEPageRef pageRef, BEURLRequestRef urlRequestRef)
{
    toBEImpl(pageRef)->loadURLRequest(toBEImpl(urlRequestRef));
}

void BEPageLoadHTMLString(BEPageRef pageRef, const char* htmlString, const char* baseURL)
{
    toBEImpl(pageRef)->loadHTMLString(String::fromUTF8(htmlString), String::fromUTF8(baseURL));
}

void BEPageLoadAlternateHTMLString(BEPageRef pageRef, const char* htmlString, const char* baseURL, const char* unreachableURL)
{
    toBEImpl(pageRef)->loadAlternateHTMLString(String::fromUTF8(htmlString), String::fromUTF8(baseURL), String::fromUTF8(unreachableURL));
}

void BEPageLoadPlainTextString(BEPageRef pageRef, const char* plainTextString)
{
    toBEImpl(pageRef)->loadPlainTextString(String::fromUTF8(plainTextString));
}

void BEPageStopLoading(BEPageRef pageRef)
{
    toBEImpl(pageRef)->stopLoading();
}

void BEPageReload(BEPageRef pageRef)
{
    toBEImpl(pageRef)->reload(false);
}

void BEPageReloadFromOrigin(BEPageRef pageRef)
{
    toBEImpl(pageRef)->reload(true);
}

void BEPageGoForward(BEPageRef pageRef)
{
    toBEImpl(pageRef)->goForward();
}

bool BEPageCanGoForward(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->canGoForward();
}

void BEPageGoBack(BEPageRef pageRef)
{
    toBEImpl(pageRef)->goBack();
}

bool BEPageCanGoBack(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->canGoBack();
}

void BEPageGoToBackForwardListItem(BEPageRef pageRef, BEBackForwardListItemRef itemRef)
{
    toBEImpl(pageRef)->goToBackForwardItem(toBEImpl(itemRef));
}

BEBackForwardListRef BEPageGetBackForwardList(BEPageRef pageRef)
{
    return toBEAPI(toBEImpl(pageRef)->backForwardList());
}

char* BEPageCopyTitle(BEPageRef pageRef)
{
    return toCopiedBEAPI(toBEImpl(pageRef)->pageTitle());
}

BEFrameRef BEPageGetMainFrame(BEPageRef pageRef)
{
    return toBEAPI(toBEImpl(pageRef)->mainFrame());
}

double BEPageGetEstimatedProgress(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->estimatedProgress();
}

char* BEPageCopyCustomUserAgent(BEPageRef pageRef)
{
    return toCopiedBEAPI(toBEImpl(pageRef)->customUserAgent());
}

void BEPageSetCustomUserAgent(BEPageRef pageRef, const char* userAgent)
{
    toBEImpl(pageRef)->setCustomUserAgent(String::fromUTF8(userAgent));
}

bool BEPageSupportsTextEncoding(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->supportsTextEncoding();
}

char* BEPageCopyCustomTextEncodingName(BEPageRef pageRef)
{
    return toCopiedBEAPI(toBEImpl(pageRef)->customTextEncodingName());
}

void BEPageSetCustomTextEncodingName(BEPageRef pageRef, const char* encodingName)
{
    toBEImpl(pageRef)->setCustomTextEncodingName(String::fromUTF8(encodingName));
}

bool BEPageSupportsTextZoom(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->supportsTextZoom();
}

void BEPageSetTextZoomFactor(BEPageRef pageRef, double zoomFactor)
{
    toBEImpl(pageRef)->setTextZoomFactor(zoomFactor);
}

double BEPageGetPageZoomFactor(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->pageZoomFactor();
}

void BEPageSetPageZoomFactor(BEPageRef pageRef, double zoomFactor)
{
    toBEImpl(pageRef)->setPageZoomFactor(zoomFactor);
}

void BEPageSetPageAndTextZoomFactors(BEPageRef pageRef, double pageZoomFactor, double textZoomFactor)
{
    toBEImpl(pageRef)->setPageAndTextZoomFactors(pageZoomFactor, textZoomFactor);
}

void BEPageSetScaleFactor(BEPageRef pageRef, double scale, BEPoint origin)
{
    toBEImpl(pageRef)->scaleWebView(scale, WebCore::IntPoint(origin.x, origin.y));
}

double BEPageGetScaleFactor(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->viewScaleFactor();
}

bool BEPageGetNodeInfo(BEPageRef pageRef, bool nodeAtCursor, char** type, char** value, BEIntRect* rect)
{
    String coreType;
    String coreValue;
    WebCore::IntRect coreRect;

    WebCore::IntPoint screenPoint;
    if (nodeAtCursor) {
        BEPoint cursorPosition = BEViewGetCursorPosition();
        screenPoint.move(cursorPosition.x, cursorPosition.y);
    } else
        screenPoint.move(-1, -1);

    toBEImpl(pageRef)->getNodeInfo(screenPoint, coreType, coreValue, coreRect);
    if (coreType.isEmpty())
        return false;

    if (type)
        *type = toCopiedBEAPI(coreType);
    if (value)
        *value = toCopiedBEAPI(coreValue);
    if (rect) {
        rect->x = coreRect.x();
        rect->y = coreRect.y();
        rect->width = coreRect.width();
        rect->height = coreRect.height();
    }
    return true;
}

bool BEPageSetFocusedNodeValue(BEPageRef pageRef, const char* value, int startPosition, int endPosition)
{
    return toBEImpl(pageRef)->setFocusedNodeValue(String::fromUTF8(value), startPosition, endPosition);
}

bool BEPageSetFocusedNodeSelection(BEPageRef pageRef, int startPosition, int endPosition)
{
    return toBEImpl(pageRef)->setFocusedNodeSelection(startPosition, endPosition);
}

BERect BEPageGetFocusRect(BEPageRef pageRef)
{
    int x, y, width, height;
    toBEImpl(pageRef)->getFocusNodeRect(x, y, width, height);
    BERect rect = { x, y, width, height };
    return rect;
}

void BEPageRunJavaScriptInMainFrame(BEPageRef pageRef, const char* script, void* context, BEPageRunJavaScriptFunction callback)
{
    toBEImpl(pageRef)->runJavaScriptInMainFrame(String::fromUTF8(script), ScriptValueCallback::create(context, reinterpret_cast<WKPageRunJavaScriptFunction>(callback)));
}

void BEPageSetJavaScriptPaused(BEPageRef pageRef, bool paused)
{
    toBEImpl(pageRef)->setJavaScriptPaused(paused);
}

void BEPageScrollBy(BEPageRef pageRef, BEScrollDirection direction, BEScrollGranularity granularity)
{
    toBEImpl(pageRef)->scrollBy(static_cast<WebCore::ScrollDirection>(direction), static_cast<WebCore::ScrollGranularity>(granularity));
}

BEImageRef BEPageCreateSnapshotOfVisibleContent(BEPageRef pageRef)
{
    RefPtr<WebImage> webImage = toBEImpl(pageRef)->createSnapshotOfVisibleContent();
    return toBEAPI(webImage.release().leakRef());
}

char* BEPageGetIconURL(BEPageRef pageRef)
{
    return toCopiedBEAPI(toBEImpl(pageRef)->getIconURL());
}

BESize BEPageGetContentsSize(BEPageRef pageRef)
{
    WebCore::IntSize contentsSize = toBEImpl(pageRef)->getContentsSize(false);
    BESize size = { contentsSize.width(), contentsSize.height() };
    return size;
}

bool BEPageIsLoginForm(BEPageRef pageRef)
{
    return toBEImpl(pageRef)->isLoginForm();	
}

void BEPageSetPageLoaderClient(BEPageRef pageRef, const BEPageLoaderClient* client)
{
    if (!client)
        return;

    WKPageLoaderClient wkClient = {
        0,
        client->clientInfo,
        (WKPageDidStartProvisionalLoadForFrameCallback) client->didStartProvisionalLoadForFrame,
        (WKPageDidReceiveServerRedirectForProvisionalLoadForFrameCallback) client->didReceiveServerRedirectForProvisionalLoadForFrame,
        (WKPageDidFailProvisionalLoadWithErrorForFrameCallback) client->didFailProvisionalLoadWithErrorForFrame,
        (WKPageDidCommitLoadForFrameCallback) client->didCommitLoadForFrame,
        (WKPageDidFinishDocumentLoadForFrameCallback) client->didFinishDocumentLoadForFrame,
        (WKPageDidFinishLoadForFrameCallback) client->didFinishLoadForFrame,
        (WKPageDidFailLoadWithErrorForFrameCallback) client->didFailLoadWithErrorForFrame,
        (WKPageDidSameDocumentNavigationForFrameCallback) client->didSameDocumentNavigationForFrame,
        (WKPageDidReceiveTitleForFrameCallback) client->didReceiveTitleForFrame,
        (WKPageDidFirstLayoutForFrameCallback) client->didFirstLayoutForFrame,
        (WKPageDidFirstVisuallyNonEmptyLayoutForFrameCallback) client->didFirstVisuallyNonEmptyLayoutForFrame,
        (WKPageDidRemoveFrameFromHierarchyCallback) client->didRemoveFrameFromHierarchy,
        (WKPageDidDisplayInsecureContentForFrameCallback) client->didDisplayInsecureContentForFrame,
        (WKPageDidRunInsecureContentForFrameCallback) client->didRunInsecureContentForFrame,
        (WKPageCanAuthenticateAgainstProtectionSpaceInFrameCallback) client->canAuthenticateAgainstProtectionSpaceInFrame,
        (WKPageDidReceiveAuthenticationChallengeInFrameCallback) client->didReceiveAuthenticationChallengeInFrame,
        (WKPageCallback) client->didStartProgress,
        (WKPageCallback) client->didChangeProgress,
        (WKPageCallback) client->didFinishProgress,
        (WKPageCallback) client->processDidBecomeUnresponsive,
        (WKPageCallback) client->processDidBecomeResponsive,
        (WKPageCallback) client->processDidCrash,
        (WKPageDidChangeBackForwardListCallback) client->didChangeBackForwardList,
        (WKPageShouldGoToBackForwardListItemCallback) client->shouldGoToBackForwardListItem,
        (WKPageDidFailToInitializePluginCallback) client->didFailToInitializePlugin
    };
    toBEImpl(pageRef)->initializeLoaderClient(&wkClient);
}

void BEPageSetPagePolicyClient(BEPageRef pageRef, const BEPagePolicyClient* client)
{
    if (!client)
        return;

    WKPagePolicyClient wkClient = {
        0,
        client->clientInfo,
        (WKPageDecidePolicyForNavigationActionCallback) client->decidePolicyForNavigationAction,
        (WKPageDecidePolicyForNewWindowActionCallback) client->decidePolicyForNewWindowAction,
        (WKPageDecidePolicyForResponseCallback) client->decidePolicyForResponse,
        (WKPageUnableToImplementPolicyCallback) client->unableToImplementPolicy
    };
    toBEImpl(pageRef)->initializePolicyClient(&wkClient);
}

void BEPageSetPageUIClient(BEPageRef pageRef, const BEPageUIClient* client)
{
    if (!client)
        return;

    WKPageUIClient wkClient = {
        0,
        client->clientInfo,
        (WKPageCreateNewPageCallback) client->createNewPage,
        (WKPageCallback) client->showPage,
        (WKPageCallback) client->close,
        (WKPageTakeFocusCallback) client->takeFocus,
        (WKPageFocusCallback) client->focus,
        (WKPageUnfocusCallback) client->unfocus,
        (WKPageRunJavaScriptAlertCallback) client->runJavaScriptAlert,
        (WKPageRunJavaScriptConfirmCallback) client->runJavaScriptConfirm,
        (WKPageRunJavaScriptPromptCallback) client->runJavaScriptPrompt,
        (WKPageSetStatusTextCallback) client->setStatusText,
        (WKPageMouseDidMoveOverElementCallback) client->mouseDidMoveOverElement,
        (WKPageMissingPluginButtonClickedCallback) client->missingPluginButtonClicked,
        (WKPageDidNotHandleKeyEventCallback) client->didNotHandleKeyEvent,
        (WKPageDidNotHandleWheelEventCallback) client->didNotHandleWheelEvent,
        (WKPageGetToolbarsAreVisibleCallback) client->toolbarsAreVisible,
        (WKPageSetToolbarsAreVisibleCallback) client->setToolbarsAreVisible,
        (WKPageGetMenuBarIsVisibleCallback) client->menuBarIsVisible,
        (WKPageSetMenuBarIsVisibleCallback) client->setMenuBarIsVisible,
        (WKPageGetStatusBarIsVisibleCallback) client->statusBarIsVisible,
        (WKPageSetStatusBarIsVisibleCallback) client->setStatusBarIsVisible,
        (WKPageGetIsResizableCallback) client->isResizable,
        (WKPageSetIsResizableCallback) client->setIsResizable,
        (WKPageGetWindowFrameCallback) client->getWindowFrame,
        (WKPageSetWindowFrameCallback) client->setWindowFrame,
        (WKPageRunBeforeUnloadConfirmPanelCallback) client->runBeforeUnloadConfirmPanel,
        (WKPageCallback) client->didDraw,
        (WKPageCallback) client->pageDidScroll,
        (WKPageExceededDatabaseQuotaCallback) client->exceededDatabaseQuota,
        (WKPageRunOpenPanelCallback) client->runOpenPanel,
        (WKPageDecidePolicyForGeolocationPermissionRequestCallback) client->decidePolicyForGeolocationPermissionRequest,
        (WKPageHeaderHeightCallback) client->headerHeight,
        (WKPageFooterHeightCallback) client->footerHeight,
        (WKPageDrawHeaderCallback) client->drawHeader,
        (WKPageDrawFooterCallback) client->drawFooter,
        (WKPagePrintFrameCallback) client->printFrame,
        (WKPageCallback) client->runModal,
        (WKPageDidCompleteRubberBandForMainFrameCallback) client->didCompleteRubberBandForMainFrame,
        (WKPageSaveDataToFileInDownloadsFolderCallback) client->saveDataToFileInDownloadsFolder,
        (WKPageShouldInterruptJavaScriptCallback) client->shouldInterruptJavaScript,
        (WKPageSetInputMethodStateCallback) client->setInputMethodState,
        (WKPageSetMouseEventStateCallback) client->setMouseEventState,
        (WKPageSetBrowserUIStateCallback) client->setBrowserUIState,
        (WKPageShowPopupMenuCallback) client->showPopupMenu,
        (WKPageHidePopupMenuCallback) client->hidePopupMenu
    };
    toBEImpl(pageRef)->initializeUIClient(&wkClient);
}
