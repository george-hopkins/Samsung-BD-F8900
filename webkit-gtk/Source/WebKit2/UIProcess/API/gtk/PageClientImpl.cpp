/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
 * Copyright (C) 2011 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "PageClientImpl.h"

#include "DrawingAreaProxyImpl.h"
#include "NativeWebKeyboardEvent.h"
#include "NativeWebMouseEvent.h"
#include "NotImplemented.h"
#include "WebContext.h"
#include "WebContextMenuProxyGtk.h"
#include "WebEventFactory.h"
#include "WebKitWebViewBasePrivate.h"
#include "WebPageProxy.h"
#include "WebPopupMenuProxyGtk.h"
#include <WebCore/Cursor.h>
#include <WebCore/GtkUtilities.h>
#include <wtf/text/WTFString.h>

#if ENABLE(TVGG_BROWSER)
#include "DirectDrawingAreaProxy.h"
#include "NotifierDrawingAreaProxy.h"
#include "WebPage.h"
#include "WebPageProxyMessages.h"
#include "WebProcess.h"
#include "UIViewWindow.h"
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "GraphicsLayerRenderer.h"
#endif
#endif


using namespace WebCore;

namespace WebKit {

PageClientImpl::PageClientImpl(GtkWidget* viewWidget)
    : m_viewWidget(viewWidget)
#if ENABLE(TVGG_BROWSER)
    , m_drawingAreaType(DrawingAreaTypeImpl)
    , m_didUpdateCallback(0)
    , m_didUpdateCallbackClientInfo(0)
#endif
{
}

PageClientImpl::~PageClientImpl()
{
}

void PageClientImpl::getEditorCommandsForKeyEvent(const NativeWebKeyboardEvent& event, const AtomicString& eventType, Vector<WTF::String>& commandList)
{
    ASSERT(eventType == eventNames().keydownEvent || eventType == eventNames().keypressEvent);

    KeyBindingTranslator::EventType type = eventType == eventNames().keydownEvent ?
        KeyBindingTranslator::KeyDown : KeyBindingTranslator::KeyPress;
    m_keyBindingTranslator.getEditorCommandsForKeyEvent(const_cast<GdkEventKey*>(&event.nativeEvent()->key), type, commandList);
}

// PageClient's pure virtual functions
PassOwnPtr<DrawingAreaProxy> PageClientImpl::createDrawingAreaProxy()
{
#if ENABLE(TVGG_BROWSER)
    if (m_drawingAreaType == DrawingAreaTypeDirect)
        return DirectDrawingAreaProxy::create(webkitWebViewBaseGetPage(WEBKIT_WEB_VIEW_BASE(m_viewWidget)));
    if (m_drawingAreaType == DrawingAreaTypeNotifier)
        return NotifierDrawingAreaProxy::create(webkitWebViewBaseGetPage(WEBKIT_WEB_VIEW_BASE(m_viewWidget)));
#endif
    return DrawingAreaProxyImpl::create(webkitWebViewBaseGetPage(WEBKIT_WEB_VIEW_BASE(m_viewWidget)));
}

void PageClientImpl::setViewNeedsDisplay(const WebCore::IntRect& rect)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer())
        return;
#endif
#if ENABLE(TVGG_BROWSER)
    if (m_didUpdateCallback) {
        m_didUpdateCallback(toBEAPI(WEBKIT_WEB_VIEW_BASE(m_viewWidget)), rect.x(), rect.y(), rect.width(), rect.height(), m_didUpdateCallbackClientInfo);
        return;
    }
#endif
    gtk_widget_queue_draw_area(m_viewWidget, rect.x(), rect.y(), rect.width(), rect.height());
}

void PageClientImpl::displayView()
{
    notImplemented();
}

void PageClientImpl::scrollView(const WebCore::IntRect& scrollRect, const WebCore::IntSize& scrollOffset)
{
    setViewNeedsDisplay(scrollRect);
}

WebCore::IntSize PageClientImpl::viewSize()
{
#if ENABLE(TVGG_BROWSER)
    if (m_drawingAreaType == DrawingAreaTypeDirect || m_drawingAreaType == DrawingAreaTypeNotifier) {
        DrawingAreaProxy* drawingAreaProxy = webkitWebViewBaseGetDrawingArea(WEBKIT_WEB_VIEW_BASE(m_viewWidget));
        if (drawingAreaProxy)
            return drawingAreaProxy->size();
        else
            return IntSize();
    }
#endif
    GtkAllocation allocation;
    gtk_widget_get_allocation(m_viewWidget, &allocation);
    return IntSize(allocation.width, allocation.height);
}

bool PageClientImpl::isViewWindowActive()
{
    notImplemented();
    return true;
}

bool PageClientImpl::isViewFocused()
{
    notImplemented();
    return true;
}

bool PageClientImpl::isViewVisible()
{
    notImplemented();
    return true;
}

bool PageClientImpl::isViewInWindow()
{
    notImplemented();
    return true;
}

void PageClientImpl::PageClientImpl::processDidCrash()
{
    notImplemented();
}

void PageClientImpl::didRelaunchProcess()
{
    notImplemented();
}

void PageClientImpl::takeFocus(bool)
{
    notImplemented();
}

void PageClientImpl::toolTipChanged(const String&, const String& newToolTip)
{
    webkitWebViewBaseSetTooltipText(WEBKIT_WEB_VIEW_BASE(m_viewWidget), newToolTip.utf8().data());
}

void PageClientImpl::setCursor(const Cursor& cursor)
{
    // [GTK] Widget::setCursor() gets called frequently
    // http://bugs.webkit.org/show_bug.cgi?id=16388
    // Setting the cursor may be an expensive operation in some backends,
    // so don't re-set the cursor if it's already set to the target value.
#if ENABLE(TVGG_BROWSER)
    if (!WebProcess::shared().isCursorVisible())
        if (cursor.type() != Cursor::None)
            return;

    if (m_drawingAreaType == DrawingAreaTypeNotifier || m_drawingAreaType == DrawingAreaTypeDirect) {
        WebPage* page = NULL;
        if (m_drawingAreaType == DrawingAreaTypeNotifier)
            page = WebProcess::shared().mainUIWebPage();
        else
            page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(WEBKIT_WEB_VIEW_BASE(m_viewWidget))->pageID());

        if (page) {
            UIViewWindow* directDrawingView = page->getUIViewWindow();
            if (directDrawingView) {
                static Cursor::Type lastCursorType = (Cursor::Type)-1;
#if ENABLE(TVGG_CUSTOM_CURSOR)
                static bool isLastCursorSmall = Cursor::isSmallCursor();
#endif
                Cursor::Type currentCursorType = cursor.type();
#if ENABLE(TVGG_DRAG_MODE)
                if (m_drawingAreaType == DrawingAreaTypeNotifier && WebProcess::shared().isDragMode())
                    currentCursorType = Cursor::Move;
#endif

#if ENABLE(TVGG_CUSTOM_CURSOR)
                if (lastCursorType != currentCursorType || isLastCursorSmall != Cursor::isSmallCursor()) {
#else
                if (lastCursorType != currentCursorType) {
#endif
                    GdkCursor* newCursor = NULL;
                    if (currentCursorType == Cursor::Custom)
                        newCursor = cursor.platformCursor().get();
                    else
                        newCursor = Cursor::fromType(currentCursorType).platformCursor().get();
                    directDrawingView->setCursor(newCursor);
                    lastCursorType = currentCursorType;
#if ENABLE(TVGG_CUSTOM_CURSOR)
                    isLastCursorSmall = Cursor::isSmallCursor();
#endif
                }
            }
            else
                page->send(Messages::WebPageProxy::SetCursor(cursor));
        }
        return;
    }
#endif
    GdkWindow* window = gtk_widget_get_window(m_viewWidget);
    GdkCursor* currentCursor = gdk_window_get_cursor(window);
#if ENABLE(TVGG_BROWSER)
    // To prevent creating cursor resource multiple times.
    GdkCursor* newCursor = Cursor::fromType(cursor.type()).platformCursor().get();
#else
    GdkCursor* newCursor = cursor.platformCursor().get();
#endif
    if (currentCursor != newCursor)
        gdk_window_set_cursor(window, newCursor);
}

void PageClientImpl::setViewportArguments(const WebCore::ViewportArguments&)
{
    notImplemented();
}

void PageClientImpl::registerEditCommand(PassRefPtr<WebEditCommandProxy>, WebPageProxy::UndoOrRedo)
{
    notImplemented();
}

void PageClientImpl::clearAllEditCommands()
{
    notImplemented();
}

bool PageClientImpl::canUndoRedo(WebPageProxy::UndoOrRedo)
{
    notImplemented();
    return false;
}

void PageClientImpl::executeUndoRedo(WebPageProxy::UndoOrRedo)
{
    notImplemented();
}

FloatRect PageClientImpl::convertToDeviceSpace(const FloatRect& viewRect)
{
    notImplemented();
    return viewRect;
}

FloatRect PageClientImpl::convertToUserSpace(const FloatRect& viewRect)
{
    notImplemented();
    return viewRect;
}

IntPoint PageClientImpl::screenToWindow(const IntPoint& point)
{
#if ENABLE(TVGG_BROWSER)
    if (m_drawingAreaType == DrawingAreaTypeDirect || m_drawingAreaType == DrawingAreaTypeNotifier) {
        IntPoint result(point);
        result.move(-m_location.x(), -m_location.y());
        return result;
    }
#endif
    IntPoint widgetPositionOnScreen = convertWidgetPointToScreenPoint(m_viewWidget, IntPoint());
    IntPoint result(point);
    result.move(-widgetPositionOnScreen.x(), -widgetPositionOnScreen.y());
    return result;
}

IntRect PageClientImpl::windowToScreen(const IntRect& rect)
{
#if ENABLE(TVGG_BROWSER)
    if (m_drawingAreaType == DrawingAreaTypeDirect || m_drawingAreaType == DrawingAreaTypeNotifier) {
        IntRect result(rect);
        result.move(m_location.x(), m_location.y());
        return result;
    }
#endif
    return IntRect(convertWidgetPointToScreenPoint(m_viewWidget, rect.location()), rect.size());
}

void PageClientImpl::doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled)
{
    notImplemented();
}

void PageClientImpl::didNotHandleKeyEvent(const NativeWebKeyboardEvent& event)
{
    notImplemented();
}

void PageClientImpl::didNotHandleWheelEvent(const NativeWebWheelEvent&)
{
    notImplemented();
}

PassRefPtr<WebPopupMenuProxy> PageClientImpl::createPopupMenuProxy(WebPageProxy* page)
{
    return WebPopupMenuProxyGtk::create(m_viewWidget, page);
}

PassRefPtr<WebContextMenuProxy> PageClientImpl::createContextMenuProxy(WebPageProxy* page)
{
    return WebContextMenuProxyGtk::create(m_viewWidget, page);
}

void PageClientImpl::setFindIndicator(PassRefPtr<FindIndicator>, bool fadeOut)
{
    notImplemented();
}

#if USE(ACCELERATED_COMPOSITING)
void PageClientImpl::enterAcceleratedCompositingMode(const LayerTreeContext&)
{
    printf("- hurnjoo.lee : %s %d\n", __func__, __LINE__);
    notImplemented();
}
void PageClientImpl::exitAcceleratedCompositingMode()
{
    printf("- hurnjoo.lee : %s %d\n", __func__, __LINE__);
    notImplemented();
}
#endif // USE(ACCELERATED_COMPOSITING)

void PageClientImpl::didCommitLoadForMainFrame(bool useCustomRepresentation)
{
}

void PageClientImpl::didFinishLoadingDataForCustomRepresentation(const String& suggestedFilename, const CoreIPC::DataReference&)
{
}

double PageClientImpl::customRepresentationZoomFactor()
{
    notImplemented();
    return 0;
}

void PageClientImpl::setCustomRepresentationZoomFactor(double)
{
    notImplemented();
}

void PageClientImpl::pageClosed()
{
    notImplemented();
}

void PageClientImpl::didChangeScrollbarsForMainFrame() const
{
}

void PageClientImpl::flashBackingStoreUpdates(const Vector<IntRect>&)
{
    notImplemented();
}

void PageClientImpl::findStringInCustomRepresentation(const String&, FindOptions, unsigned)
{
    notImplemented();
}

void PageClientImpl::countStringMatchesInCustomRepresentation(const String&, FindOptions, unsigned)
{
    notImplemented();
}

} // namespace WebKit
