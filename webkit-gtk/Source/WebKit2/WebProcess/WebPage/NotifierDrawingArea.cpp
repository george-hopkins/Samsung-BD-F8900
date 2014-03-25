#include "config.h"
#include "NotifierDrawingArea.h"

#include "DrawingAreaProxyMessages.h"
#include "LayerTreeContext.h"
#include "ShareableBitmap.h"
#include "UIViewWindow.h"
#include "UpdateInfo.h"
#include "WebImage.h"
#include "WebPage.h"
#include "WebPageCreationParameters.h"
#include "WebProcess.h"
#include "WebProcessProxy.h"

#include <WebCore/GraphicsContext.h>
#include <WebCore/HTMLAreaElement.h>
#include <WebCore/HTMLImageElement.h>
#include <WebCore/HTMLNames.h>
#include <WebCore/Page.h>
#include <WebCore/Settings.h>

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
#include <WebCore/FocusController.h>
#include <WebCore/Frame.h>
#include <WebCore/FrameView.h>
#include <WebCore/RenderObject.h>
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "GraphicsContext3D.h"
#include "GraphicsLayerRenderer.h"
#include "GraphicsLayerTexture.h"
#include "LayerTextureManagerTVGG.h"
#include <cairo.h>
#include <stdio.h>
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
#include "NotifierDrawingAreaProxy.h"
#include "WebContext.h"
#include "WebPageProxy.h"
#endif

using namespace WebCore;

namespace WebKit {

double NotifierDrawingArea::m_lastCaptureTime = 0;
NotifierDrawingArea* NotifierDrawingArea::s_active = 0;

PassOwnPtr<NotifierDrawingArea> NotifierDrawingArea::create(WebPage* webPage, const WebPageCreationParameters& parameters)
{
    return adoptPtr(new NotifierDrawingArea(webPage, parameters));
}

NotifierDrawingArea::~NotifierDrawingArea()
{
    if (m_layerTreeHost)
        m_layerTreeHost->invalidate();

    if (s_active == this)
        s_active = 0;
}

NotifierDrawingArea::NotifierDrawingArea(WebPage* webPage, const WebPageCreationParameters& parameters)
    : DrawingArea(DrawingAreaTypeNotifier, webPage)
    , m_backingStoreStateID(0)
    , m_inUpdateBackingStoreState(false)
    , m_shouldSendDidUpdateBackingStoreState(false)
    , m_isWaitingForDidUpdate(false)
    , m_compositingAccordingToProxyMessages(false)
    , m_layerTreeStateIsFrozen(false)
    , m_wantsToExitAcceleratedCompositingMode(false)
    , m_isPaintingSuspended(!parameters.isVisible)
    , m_alwaysUseCompositing(false)
    , m_shouldThrottleDisplay(true)
    , m_lastDisplayTime(0)
    , m_displayTimer(WebProcess::shared().runLoop(), this, &NotifierDrawingArea::displayTimerFired)
    , m_exitCompositingTimer(WebProcess::shared().runLoop(), this, &NotifierDrawingArea::exitAcceleratedCompositingMode)
#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    , m_isFocusRectPainting(parameters.store.getBoolValueForKey(WebPreferencesKey::isSpatialNavigationEnabledKey()))
#endif
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#if ENABLE(TVGG_THREAD_MODEL)
    , m_graphicsLayer(0)
#endif
    , m_needsComposite(false)
#endif
#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    , m_isThreadModel(parameters.isThreadMode)
    , m_idleSkipTime(0)
#endif
#if ENABLE(TVGG_ANIMATED_SCROLL)
    , m_disableAnimationFlag(false)
#endif

{
    if (webPage->corePage()->settings()->acceleratedDrawingEnabled())
        m_alwaysUseCompositing = true;

    if (m_alwaysUseCompositing)
        enterAcceleratedCompositingMode(0);

    if (!s_active)
        s_active = this;
}

void NotifierDrawingArea::setNeedsDisplay(const IntRect& rect)
{
    if (s_active != this)
        return;

    IntRect dirtyRect = rect;
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    IntRect visibleRect = IntRect();
    m_webPage->getVisibleRect(true, visibleRect);
    IntSize contentSize = IntSize();
    m_webPage->getContentsSize(true, contentSize);
    IntRect contentRect = IntRect(IntPoint(), contentSize);
    contentRect.move(-visibleRect.x(), -visibleRect.y());
    dirtyRect.intersect(contentRect);
#else
    dirtyRect.intersect(m_webPage->bounds());
#endif

    if (dirtyRect.isEmpty())
        return;
		
    if (m_layerTreeHost) {
        ASSERT(m_dirtyRegion.isEmpty());

        m_layerTreeHost->setNonCompositedContentsNeedDisplay(dirtyRect);
        return;
    }

    if (m_webPage->mainFrameHasCustomRepresentation())
        return;

    m_dirtyRegion.unite(dirtyRect);
    scheduleDisplay();
}

void NotifierDrawingArea::scroll(const IntRect& scrollRect, const IntSize& scrollOffset)
{
    if (m_layerTreeHost) {
        ASSERT(m_scrollRect.isEmpty());
        ASSERT(m_scrollOffset.isEmpty());
        ASSERT(m_dirtyRegion.isEmpty());

        m_layerTreeHost->scrollNonCompositedContents(scrollRect, scrollOffset);
        return;
    }

    if (m_webPage->mainFrameHasCustomRepresentation())
        return;

    if (!m_scrollRect.isEmpty() && scrollRect != m_scrollRect) {
        unsigned scrollArea = scrollRect.width() * scrollRect.height();
        unsigned currentScrollArea = m_scrollRect.width() * m_scrollRect.height();

        if (currentScrollArea >= scrollArea) {
            // The rect being scrolled is at least as large as the rect we'd like to scroll.
            // Go ahead and just invalidate the scroll rect.
            setNeedsDisplay(scrollRect);
            return;
        }

        // Just repaint the entire current scroll rect, we'll scroll the new rect instead.
        setNeedsDisplay(m_scrollRect);
        m_scrollRect = IntRect();
        m_scrollOffset = IntSize();
    }

    // Get the part of the dirty region that is in the scroll rect.
    Region dirtyRegionInScrollRect = intersect(scrollRect, m_dirtyRegion);
    if (!dirtyRegionInScrollRect.isEmpty()) {
        // There are parts of the dirty region that are inside the scroll rect.
        // We need to subtract them from the region, move them and re-add them.
        m_dirtyRegion.subtract(scrollRect);

        // Move the dirty parts.
        Region movedDirtyRegionInScrollRect = intersect(translate(dirtyRegionInScrollRect, scrollOffset), scrollRect);

        // And add them back.
        m_dirtyRegion.unite(movedDirtyRegionInScrollRect);
    }

    // Compute the scroll repaint region.
    Region scrollRepaintRegion = subtract(scrollRect, translate(scrollRect, scrollOffset));

    m_dirtyRegion.unite(scrollRepaintRegion);

    m_scrollRect = scrollRect;
    m_scrollOffset += scrollOffset;
}

void NotifierDrawingArea::setLayerTreeStateIsFrozen(bool isFrozen)
{
    if (m_layerTreeStateIsFrozen == isFrozen)
        return;

    m_layerTreeStateIsFrozen = isFrozen;

    if (m_layerTreeHost)
        m_layerTreeHost->setLayerFlushSchedulingEnabled(!isFrozen);

    if (isFrozen)
        m_exitCompositingTimer.stop();
    else if (m_wantsToExitAcceleratedCompositingMode)
        exitAcceleratedCompositingModeSoon();
}

void NotifierDrawingArea::forceRepaint()
{
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    IntRect visibleRect = IntRect();
    m_webPage->getVisibleRect(true, visibleRect);

    IntRect coverRect = visibleRect;
    FloatSize m_coverAreaMultiplier = FloatSize(1.5f, 3.f);
    coverRect.inflateX(visibleRect.width() * (m_coverAreaMultiplier.width() - 1.f) / 2);
    coverRect.inflateY(visibleRect.height() * (m_coverAreaMultiplier.height() - 1.f) / 2);
    coverRect.move(-visibleRect.x(), -visibleRect.y());

    setNeedsDisplay(coverRect);
#else
    setNeedsDisplay(m_webPage->bounds());
#endif
    m_webPage->layoutIfNeeded();

    if (m_layerTreeHost) {
        // FIXME: We need to do the same work as the layerHostDidFlushLayers function here,
        // but clearly it doesn't make sense to call the function with that name.
        // Consider refactoring and renaming it.
        if (m_compositingAccordingToProxyMessages)
            m_layerTreeHost->forceRepaint();
        else {
            // Call setShouldNotifyAfterNextScheduledLayerFlush(false) here to
            // prevent layerHostDidFlushLayers() from being called a second time.
            m_layerTreeHost->setShouldNotifyAfterNextScheduledLayerFlush(false);
            layerHostDidFlushLayers();
        }
        return;
    }

    m_isWaitingForDidUpdate = false;
    display();
}

void NotifierDrawingArea::enableDisplayThrottling()
{
    m_shouldThrottleDisplay = true;
}

void NotifierDrawingArea::disableDisplayThrottling()
{
    m_shouldThrottleDisplay = false;
}

void NotifierDrawingArea::didInstallPageOverlay()
{
    if (m_layerTreeHost)
        m_layerTreeHost->didInstallPageOverlay();
}

void NotifierDrawingArea::didUninstallPageOverlay()
{
    if (m_layerTreeHost)
        m_layerTreeHost->didUninstallPageOverlay();

    setNeedsDisplay(m_webPage->bounds());
}

void NotifierDrawingArea::setPageOverlayNeedsDisplay(const IntRect& rect)
{
    if (m_layerTreeHost) {
        m_layerTreeHost->setPageOverlayNeedsDisplay(rect);
        return;
    }

    setNeedsDisplay(rect);
}

void NotifierDrawingArea::layerHostDidFlushLayers()
{
    ASSERT(m_layerTreeHost);

    m_layerTreeHost->forceRepaint();

    if (m_shouldSendDidUpdateBackingStoreState) {
        sendDidUpdateBackingStoreState();
        return;
    }

    if (!m_layerTreeHost)
        return;

#if USE(ACCELERATED_COMPOSITING)
    ASSERT(!m_compositingAccordingToProxyMessages);
    if (!exitAcceleratedCompositingModePending()) {
        m_webPage->send(Messages::DrawingAreaProxy::EnterAcceleratedCompositingMode(m_backingStoreStateID, m_layerTreeHost->layerTreeContext()));
        m_compositingAccordingToProxyMessages = true;
    }
#endif
}

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
gboolean NotifierDrawingArea::composite(gpointer cbData)
{
    WebPage* page = (WebPage*)cbData;
    if (page && page->getRenderer() && page->drawingArea()->needsComposite()) {
        page->getRenderer()->render(page->size().width(), page->size().height());
#if ENABLE(TVGG_THREAD_MODEL)
        WebPage* uiPage = WebProcess::shared().mainUIWebPage();
        if (uiPage && uiPage->drawingArea())
            uiPage->drawingArea()->scheduleCompositingLayerSync();
#else
        page->send(Messages::DrawingAreaProxy::SwapUmpID());
#endif
        page->drawingArea()->setNeedsComposite(false);
    }
    return true;
}
#endif

void NotifierDrawingArea::setRootCompositingLayer(GraphicsLayer* graphicsLayer)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
    m_graphicsLayer = graphicsLayer;
    if (!m_webPage->getRenderer())
        enterAcceleratedCompositingMode(graphicsLayer);
    else if (graphicsLayer && m_webPage->getRenderer())
        enterAcceleratedCompositingMode(graphicsLayer);
//        m_webPage->getRenderer()->setRootLayer(graphicsLayer);
    else if (!graphicsLayer)
        exitAcceleratedCompositingMode();

    return;
#endif
    // FIXME: Instead of using nested if statements, we should keep a compositing state
    // enum in the NotifierDrawingArea object and have a changeAcceleratedCompositingState function
    // that takes the new state.

    if (graphicsLayer) {
        if (!m_layerTreeHost) {
            // We're actually entering accelerated compositing mode.
            enterAcceleratedCompositingMode(graphicsLayer);
        } else {
            // We're already in accelerated compositing mode, but the root compositing layer changed.

            m_exitCompositingTimer.stop();
            m_wantsToExitAcceleratedCompositingMode = false;

            // If we haven't sent the EnterAcceleratedCompositingMode message, make sure that the
            // layer tree host calls us back after the next layer flush so we can send it then.
            if (!m_compositingAccordingToProxyMessages)
                m_layerTreeHost->setShouldNotifyAfterNextScheduledLayerFlush(true);

            m_layerTreeHost->setRootCompositingLayer(graphicsLayer);
        }
    } else {
        if (m_layerTreeHost) {
            m_layerTreeHost->setRootCompositingLayer(0);
            if (!m_alwaysUseCompositing) {
                // We'll exit accelerated compositing mode on a timer, to avoid re-entering
                // compositing code via display() and layout.
                // If we're leaving compositing mode because of a setSize, it is safe to
                // exit accelerated compositing mode right away.
                if (m_inUpdateBackingStoreState)
                    exitAcceleratedCompositingMode();
                else
                    exitAcceleratedCompositingModeSoon();
            }
        }
    }
}

void NotifierDrawingArea::scheduleCompositingLayerSync()
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
    if (s_active != this)
        return;
#if ENABLE(TVGG_THREAD_MODEL)
    WebPage* uiPage = WebProcess::shared().mainUIWebPage();
    if (uiPage && uiPage->drawingArea())
        uiPage->drawingArea()->scheduleCompositingLayerSync();
#else
    setNeedsComposite(true);
#endif
    return;
#endif
    if (!m_layerTreeHost)
        return;
    m_layerTreeHost->scheduleLayerFlush();
}

void NotifierDrawingArea::syncCompositingLayers()
{
}

void NotifierDrawingArea::updateBackingStoreState(uint64_t stateID, bool respondImmediately, const WebCore::IntSize& size, const WebCore::IntSize& scrollOffset)
{
    ASSERT(!m_inUpdateBackingStoreState);
    m_inUpdateBackingStoreState = true;

    ASSERT_ARG(stateID, stateID >= m_backingStoreStateID);
    if (stateID != m_backingStoreStateID) {
        m_backingStoreStateID = stateID;
        m_shouldSendDidUpdateBackingStoreState = true;

        m_webPage->setSize(size);
        m_webPage->layoutIfNeeded();
        m_webPage->scrollMainFrameIfNotAtMaxScrollPosition(scrollOffset);

        if (m_layerTreeHost)
            m_layerTreeHost->sizeDidChange(size);
        else
            m_dirtyRegion = m_webPage->bounds();
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        WebPage* uiPage = WebProcess::shared().mainUIWebPage();
        if (uiPage->getRenderer()) {
            uiPage->getRenderer()->setContentsViewSize(size);
            display();
        }
#endif
    } else {
        ASSERT(size == m_webPage->size());
        if (!m_shouldSendDidUpdateBackingStoreState) {
            // We've already sent a DidUpdateBackingStoreState message for this state. We have nothing more to do.
            m_inUpdateBackingStoreState = false;
            return;
        }
    }

    // The UI process has updated to a new backing store state. Any Update messages we sent before
    // this point will be ignored. We wait to set this to false until after updating the page's
    // size so that any displays triggered by the relayout will be ignored. If we're supposed to
    // respond to the UpdateBackingStoreState message immediately, we'll do a display anyway in
    // sendDidUpdateBackingStoreState; otherwise we shouldn't do one right now.
    m_isWaitingForDidUpdate = false;

    if (respondImmediately)
        sendDidUpdateBackingStoreState();

    m_inUpdateBackingStoreState = false;
}

void NotifierDrawingArea::sendDidUpdateBackingStoreState()
{
    ASSERT(!m_isWaitingForDidUpdate);
    ASSERT(m_shouldSendDidUpdateBackingStoreState);

    m_shouldSendDidUpdateBackingStoreState = false;

    UpdateInfo updateInfo;

    if (!m_isPaintingSuspended && !m_layerTreeHost)
        display(updateInfo);

    LayerTreeContext layerTreeContext;

    if (m_isPaintingSuspended || m_layerTreeHost) {
        updateInfo.viewSize = m_webPage->size();

        if (m_layerTreeHost) {
            layerTreeContext = m_layerTreeHost->layerTreeContext();

            // We don't want the layer tree host to notify after the next scheduled
            // layer flush because that might end up sending an EnterAcceleratedCompositingMode
            // message back to the UI process, but the updated layer tree context
            // will be sent back in the DidUpdateBackingStoreState message.
            m_layerTreeHost->setShouldNotifyAfterNextScheduledLayerFlush(false);
            m_layerTreeHost->forceRepaint();
        }
    }

    m_webPage->send(Messages::DrawingAreaProxy::DidUpdateBackingStoreState(m_backingStoreStateID, updateInfo, layerTreeContext));
    m_compositingAccordingToProxyMessages = !layerTreeContext.isEmpty();
}

void NotifierDrawingArea::didUpdate()
{
    // We might get didUpdate messages from the UI process even after we've
    // entered accelerated compositing mode. Ignore them.
    if (m_layerTreeHost)
        return;

    m_isWaitingForDidUpdate = false;

    // Display if needed. We call displayTimerFired here since it will throttle updates to 60fps.
    displayTimerFired();
}

void NotifierDrawingArea::suspendPainting()
{
    ASSERT(!m_isPaintingSuspended);

    if (m_layerTreeHost)
        m_layerTreeHost->pauseRendering();

    m_isPaintingSuspended = true;
    m_displayTimer.stop();
}

void NotifierDrawingArea::resumePainting()
{
    if (!m_isPaintingSuspended) {
        // FIXME: We can get a call to resumePainting when painting is not suspended.
        // This happens when sending a synchronous message to create a new page. See <rdar://problem/8976531>.
        return;
    }

    if (m_layerTreeHost)
        m_layerTreeHost->resumeRendering();

    m_isPaintingSuspended = false;

    // FIXME: We shouldn't always repaint everything here.
    setNeedsDisplay(m_webPage->bounds());
}

void NotifierDrawingArea::enterAcceleratedCompositingMode(GraphicsLayer* graphicsLayer)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#if ENABLE(TVGG_THREAD_MODEL)
    WebPage* uiPage = WebProcess::shared().mainUIWebPage();
    m_graphicsLayer = graphicsLayer;
    if (uiPage && uiPage->getRenderer())
        uiPage->getRenderer()->setContentsViewGraphicsLayer(m_graphicsLayer);
    return;
#endif
    if (!m_webPage->getRenderer()) {
        WebPage* page = WebProcess::shared().mainUIWebPage();
        RefPtr<WebCore::GraphicsContext3D> context = WebCore::GraphicsContext3D::create(
            WebCore::GraphicsContext3D::Attributes(),
            (HostWindow*)page->getUIViewWindow(), WebCore::GraphicsContext3D::RenderOffscreen);

        context->reshape(m_webPage->size().width(), m_webPage->size().height());
        if (!context) {
            printf("ASSERT: GraphicsContext3D creation failed\n");
            return;
        }

        m_webPage->createRenderer(context);

        if (!(m_webPage->getRenderer()->initialize())) {
            printf("ASSERT: Renderer creation failed\n");
            return;
        }
// create front/back buffer of floating layer.

//        unsigned int frontBufferID = context->frontBufferID();
//        unsigned int backBufferID = context->backBufferID();
    }

    unsigned int frontBufferID = (unsigned int)m_webPage->getRenderer()->context()->platformTexture();
    unsigned int backBufferID = (unsigned int)m_webPage->getRenderer()->context()->platformTexture();

    if (frontBufferID && backBufferID)
        m_webPage->send(Messages::DrawingAreaProxy::SetUmpID(frontBufferID, backBufferID));

    m_webPage->send(Messages::DrawingAreaProxy::EnterContentsViewAcceleratedCompositingMode(true));

    if (graphicsLayer && m_webPage->getRenderer())
        m_webPage->getRenderer()->setRootLayer(graphicsLayer);

    static int tid = 0;
    if (tid)
        g_source_remove(tid);

    if (true || graphicsLayer) {
        tid = g_timeout_add_full(G_PRIORITY_HIGH, 10, composite, m_webPage, 0);
        setNeedsComposite(true);
    } else
        setNeedsComposite(false);

    return;
#endif
    m_exitCompositingTimer.stop();
    m_wantsToExitAcceleratedCompositingMode = false;

    ASSERT(!m_layerTreeHost);

    m_layerTreeHost = LayerTreeHost::create(m_webPage);
    if (!m_inUpdateBackingStoreState)
        m_layerTreeHost->setShouldNotifyAfterNextScheduledLayerFlush(true);

    m_layerTreeHost->setRootCompositingLayer(graphicsLayer);

    // Non-composited content will now be handled exclusively by the layer tree host.
    m_dirtyRegion = Region();
    m_scrollRect = IntRect();
    m_scrollOffset = IntSize();
    m_displayTimer.stop();
    m_isWaitingForDidUpdate = false;
}

void NotifierDrawingArea::exitAcceleratedCompositingMode()
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
#if ENABLE(TVGG_THREAD_MODEL)
    WebPage* uiPage = WebProcess::shared().mainUIWebPage();
    if (uiPage && uiPage->getRenderer())
        uiPage->getRenderer()->setContentsViewGraphicsLayer(0);
    m_graphicsLayer = 0;
#else
    m_webPage->send(Messages::DrawingAreaProxy::EnterContentsViewAcceleratedCompositingMode(false));
#endif
    return;
#endif

    if (m_alwaysUseCompositing)
        return;

    ASSERT(!m_layerTreeStateIsFrozen);

    m_exitCompositingTimer.stop();
    m_wantsToExitAcceleratedCompositingMode = false;

    ASSERT(m_layerTreeHost);

    m_layerTreeHost->invalidate();
    m_layerTreeHost = nullptr;
    m_dirtyRegion = m_webPage->bounds();

    if (m_inUpdateBackingStoreState)
        return;

    if (m_shouldSendDidUpdateBackingStoreState) {
        sendDidUpdateBackingStoreState();
        return;
    }

    UpdateInfo updateInfo;
    if (m_isPaintingSuspended)
        updateInfo.viewSize = m_webPage->size();
    else
        display(updateInfo);

#if USE(ACCELERATED_COMPOSITING)
    // Send along a complete update of the page so we can paint the contents right after we exit the
    // accelerated compositing mode, eliminiating flicker.
    if (m_compositingAccordingToProxyMessages) {
        m_webPage->send(Messages::DrawingAreaProxy::ExitAcceleratedCompositingMode(m_backingStoreStateID, updateInfo));
        m_compositingAccordingToProxyMessages = false;
    } else {
        // If we left accelerated compositing mode before we sent an EnterAcceleratedCompositingMode message to the
        // UI process, we still need to let it know about the new contents, so send an Update message.
        m_webPage->send(Messages::DrawingAreaProxy::Update(m_backingStoreStateID, updateInfo));
    }
#endif
}

void NotifierDrawingArea::exitAcceleratedCompositingModeSoon()
{
    if (m_layerTreeStateIsFrozen) {
        m_wantsToExitAcceleratedCompositingMode = true;
        return;
    }

    if (exitAcceleratedCompositingModePending())
        return;

    m_exitCompositingTimer.startOneShot(0);
}

void NotifierDrawingArea::scheduleDisplay()
{
    ASSERT(!m_layerTreeHost);

    if (m_isWaitingForDidUpdate)
        return;

    if (m_isPaintingSuspended)
        return;

    if (m_dirtyRegion.isEmpty())
        return;

    if (m_displayTimer.isActive())
        return;

    m_displayTimer.startOneShot(0);
}

void NotifierDrawingArea::displayTimerFired()
{
#if PLATFORM(WIN)
    // For now we'll cap painting on Windows to 30fps because painting is much slower there for some reason.
    static const double minimumFrameInterval = 1.0 / 30.0;
#else
    static const double minimumFrameInterval = 1.0 / 60.0;
#endif

    if (m_shouldThrottleDisplay) {
        double timeSinceLastDisplay = currentTime() - m_lastDisplayTime;
        double timeUntilNextDisplay = minimumFrameInterval - timeSinceLastDisplay;

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
        if (m_idleSkipTime > 0.05) {
            m_displayTimer.startOneShot(minimumFrameInterval);
            m_idleSkipTime = 0;
            return;
        }
#endif
        if (timeUntilNextDisplay > 0) {
            m_displayTimer.startOneShot(timeUntilNextDisplay);
#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
            m_idleSkipTime = 0;
#endif
            return;
        }
#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
        m_idleSkipTime += timeSinceLastDisplay;
#endif
    }

    display();
}

void NotifierDrawingArea::display()
{
    ASSERT(!m_layerTreeHost);
    ASSERT(!m_isWaitingForDidUpdate);
    ASSERT(!m_inUpdateBackingStoreState);

    if (m_isPaintingSuspended)
        return;

    if (m_dirtyRegion.isEmpty())
        return;

    if (m_shouldSendDidUpdateBackingStoreState) {
        sendDidUpdateBackingStoreState();
        return;
    }

    UpdateInfo updateInfo;
    display(updateInfo);

    if (m_layerTreeHost) {
        // The call to update caused layout which turned on accelerated compositing.
        // Don't send an Update message in this case.
        return;
    }

    m_webPage->send(Messages::DrawingAreaProxy::Update(m_backingStoreStateID, updateInfo));
    m_isWaitingForDidUpdate = true;
}

static bool shouldPaintBoundsRect(const IntRect& bounds, const Vector<IntRect>& rects)
{
    const size_t rectThreshold = 10;
    const double wastedSpaceThreshold = 0.75;

    if (rects.size() <= 1 || rects.size() > rectThreshold)
        return true;

    // Attempt to guess whether or not we should use the region bounds rect or the individual rects.
    // We do this by computing the percentage of "wasted space" in the bounds.  If that wasted space
    // is too large, then we will do individual rect painting instead.
    unsigned boundsArea = bounds.width() * bounds.height();
    unsigned rectsArea = 0;
    for (size_t i = 0; i < rects.size(); ++i)
        rectsArea += rects[i].width() * rects[i].height();

    double wastedSpace = 1 - (static_cast<double>(rectsArea) / boundsArea);

    return wastedSpace <= wastedSpaceThreshold;
}

#if !PLATFORM(WIN)
PassOwnPtr<GraphicsContext> NotifierDrawingArea::createGraphicsContext(ShareableBitmap* bitmap)
{
    return bitmap->createGraphicsContext();
}
#endif
/*
#define MEM_DEBUG;
int peakused;
int peakshbitmap;
*/
void NotifierDrawingArea::display(UpdateInfo& updateInfo)
{
    ASSERT(!m_isPaintingSuspended);
    ASSERT(!m_layerTreeHost);
    ASSERT(!m_webPage->size().isEmpty());

#ifdef MEM_DEBUG
    int full, used, avail, per;
    char buf[128] = {0,};
    FILE* fp = NULL;
    FILE* resfp = fopen("/mtd_down/df.result", "a");

    fprintf(resfp, "==============Peak Sh = %d, Peak Tmp = %d (kB)==============\n\n", peakshbitmap, peakused);

    fp = popen("df | grep tmpfs", "r");
    if(fp != NULL)
    {
        while(fgets(buf, 128, fp) != NULL)
        {
            if(buf != ""){
                sscanf(buf, "tmpfs %d %d %d %d\% \/tmp\n", &full, &used, &avail, &per);
                if(used >= peakused)
                    peakused = used;
                //fprintf(resfp, "Tmp Full = %d, Used = %d, Avail = %d, Usepercent = %d\%\n",  full, used, avail, per);
                fprintf(resfp, "%s", buf);
                //printf("%s\n", buf);
            }
        }
    }
    pclose(fp);
#endif

    // FIXME: It would be better if we could avoid painting altogether when there is a custom representation.
    if (m_webPage->mainFrameHasCustomRepresentation()) {
        // ASSUMPTION: the custom representation will be painting the dirty region for us.
        m_dirtyRegion = Region();
        return;
    }

    m_webPage->layoutIfNeeded();

    // The layout may have put the page into accelerated compositing mode. If the LayerTreeHost is
    // in charge of displaying, we have nothing more to do.
    if (m_layerTreeHost)
        return;

    updateInfo.viewSize = m_webPage->size();

    IntRect bounds = m_dirtyRegion.bounds();
#if !ENABLE(TVGG_EXTENDED_BACKING_STORE)
    ASSERT(m_webPage->bounds().contains(bounds));
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    RefPtr<ShareableBitmap> bitmap;
    if (m_isThreadModel)
        bitmap = ShareableBitmap::createShareable(IntSize(1,1), ShareableBitmap::SupportsAlpha);
    else
        bitmap = ShareableBitmap::createShareable(bounds.size(), ShareableBitmap::SupportsAlpha);
#else
    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::createShareable(bounds.size(), ShareableBitmap::SupportsAlpha);
#endif

    if (!bitmap)
        return;

    if (!bitmap->createHandle(updateInfo.bitmapHandle))
        return;

#ifdef MEM_DEBUG
    IntSize bitmapSize = bitmap->size();
    int bitmapByte = bitmapSize.width() * bitmapSize.height() * 4 /1024;

    fprintf(resfp, "Shareable bitmap size =              %d kB\n", bitmapByte);
    //printf("Shareable bitmap size = %d kBytes\n", bounds.size().width() * bounds.size().height() * 4 /1024);

    if(bitmapByte >= peakshbitmap)
        peakshbitmap = bitmapByte;
#endif

    Vector<IntRect> rects = m_dirtyRegion.rects();

    if (shouldPaintBoundsRect(bounds, rects)) {
        rects.clear();
        rects.append(bounds);
    }
    
#if ENABLE(TVGG_TILED_BACKING_STORE)
    m_webPage->getVisibleRect(true, updateInfo.visibleRect);

#if ENABLE(TVGG_ANIMATED_SCROLL)
    displayScrollbars(updateInfo);
#else
    Frame* frame = m_webPage->corePage()->mainFrame();
    if (frame->view()->horizontalScrollbar())
        updateInfo.hscrollbarRect = frame->view()->horizontalScrollbar()->frameRect();
    else
        updateInfo.hscrollbarRect = IntRect();
        
    if (frame->view()->verticalScrollbar())
        updateInfo.vscrollbarRect = frame->view()->verticalScrollbar()->frameRect();
    else
        updateInfo.vscrollbarRect = IntRect();
#endif

#endif

#if ENABLE(TVGG_ANIMATED_SCROLL)
    if (!m_scrollOffset.isZero() && m_disableAnimationFlag == true){
        updateInfo.disableAnimation = true;        
        setDisableAnimationFlag(false);
    }
    else {
        updateInfo.disableAnimation = false;
        setDisableAnimationFlag(false);
    }
#endif

    updateInfo.scrollRect = m_scrollRect;
    updateInfo.scrollOffset = m_scrollOffset;

    m_dirtyRegion = Region();
    m_scrollRect = IntRect();
    m_scrollOffset = IntSize();

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    OwnPtr<GraphicsContext> graphicsContext;
    cairo_t* context = 0;
    BackingStore* backingStore = 0;

    if (m_isThreadModel) {
        WebPageProxy* webPageProxy = WebContext::sharedThreadContext()->process()->webPage(m_webPage->pageID());
        if (!webPageProxy)
            return;

        NotifierDrawingAreaProxy* drawingArea = static_cast<NotifierDrawingAreaProxy*>(webPageProxy->drawingArea());
        if (!drawingArea)
            return;

        backingStore = drawingArea->getBackingStore();

        if (m_webPage->size() != backingStore->size()) {
            bounds = IntRect(WebCore::IntPoint(), backingStore->size());
            rects.clear();
            rects.append(bounds);
        }

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        WebPage* page = WebProcess::shared().mainUIWebPage();
        if (page && page->getRenderer() && page->getUIViewWindow()) {
            ImageBackingStore* buffer = backingStore->getImageBuffer();
            context = cairo_create(buffer->cairoSurface());
        } else
#endif
        {
            WidgetBackingStore* buffer = backingStore->getBuffer();
            context = cairo_create(buffer->cairoSurface());
        }

        graphicsContext = adoptPtr(new GraphicsContext(context));
    }
    else
        graphicsContext = createGraphicsContext(bitmap.get());
#else
    OwnPtr<GraphicsContext> graphicsContext = createGraphicsContext(bitmap.get());
#endif

    updateInfo.updateRectBounds = bounds;

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    if (m_isThreadModel)
        backingStore->scroll(updateInfo.scrollRect, updateInfo.scrollOffset);
    else
#endif
        graphicsContext->translate(-bounds.x(), -bounds.y());

    for (size_t i = 0; i < rects.size(); ++i) {
        m_webPage->drawRect(*graphicsContext, rects[i]);

        if (m_webPage->hasPageOverlay()) 
            m_webPage->drawPageOverlay(*graphicsContext, rects[i]);
        updateInfo.updateRects.append(rects[i]);
    }

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (m_isFocusRectPainting) {
#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
        if (m_isThreadModel)
            graphicsContext.get()->clip(bounds);
#endif

        IntPoint location(-1, -1);

        Frame* frame = m_webPage->corePage()->focusController()->focusedFrame();
        if (frame)
            location = frame->view()->paintFocusRing(graphicsContext.get());

        if (location.x() < 0 && location.y() < 0) {
            bool result;
            m_webPage->setFocusByPoint(m_focusLocation, result);
        } else
            m_focusLocation = location;
    }
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer() && page->getUIViewWindow()) {
        page->getUIViewWindow()->sync();
        scheduleCompositingLayerSync();
    }
#endif
    // Layout can trigger more calls to setNeedsDisplay and we don't want to process them
    // until the UI process has painted the update, so we stop the timer here.
    m_displayTimer.stop();

    m_lastDisplayTime = currentTime();

#ifdef MEM_DEBUG
if(updateInfo.scrollOffset.height() != 0){
    int Free, Cached, Active, Inactive, Aanon, Inanon;
    char buf[128] = {0,};
    FILE* fp = NULL;
    FILE* resfp = fopen("/mtd_down/mem.status", "a");

    fp = popen("cat /proc/meminfo", "r");
    if(fp != NULL)
    {
        while(fgets(buf, 128, fp) != NULL)
        {
            char *strTarget = NULL;
            if((strTarget = strstr(buf, "MemFree:")) != NULL){
                sscanf(buf, "MemFree: %d kB", &Free);
                //fprintf(resfp, "%s", buf);
                fprintf(resfp, "%d\t", Free);
            }
            else if((strTarget = strstr(buf, "Cached:")) != NULL){
                sscanf(buf, "Cached: %d kB", &Cached);
                //fprintf(resfp, "%s", buf);
                fprintf(resfp, "%d\t", Cached);
            }
            else if((strTarget = strstr(buf, "Active:")) != NULL){
                sscanf(buf, "Active: %d kB", &Active);
                //fprintf(resfp, "%s", buf);
                fprintf(resfp, "%d\t", Active);
            }
            else if((strTarget = strstr(buf, "Inactive:")) != NULL){
                sscanf(buf, "Inactive: %d kB", &Inactive);
                //fprintf(resfp, "%s", buf);
                fprintf(resfp, "%d\t", Inactive);
            }
            else
                continue;
        }
    }
    fprintf(resfp, "\n");
    
    pclose(fp);
    fclose(resfp);
}
#endif

#ifdef MEM_DEBUG
    //int full, used, avail, per;
    //char buf[128] = {0,};
    //FILE* fp = NULL;

    fp = popen("df | grep tmpfs", "r");
    if(fp != NULL)
    {
        while(fgets(buf, 128, fp) != NULL)
        {
            if(buf != ""){
                sscanf(buf, "tmpfs %d %d %d %d\% \/tmp\n", &full, &used, &avail, &per);
                if(used >= peakused)
                    peakused = used;
                //fprintf(resfp, "Tmp Full = %d, Used = %d, Avail = %d, Usepercent = %d\%\n",  full, used, avail, per);
                fprintf(resfp, "%s", buf);
                //printf("%s\n", buf);
            }
        }
    }
    pclose(fp);
    fclose(resfp);
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    if (context)
        cairo_destroy(context);
#endif
}

void NotifierDrawingArea::captureSnapshot(double scaleFactor)
{
    double time = currentTime();
    if (time - m_lastCaptureTime < 10)
        return;

    Frame* frame = m_webPage->corePage()->mainFrame();
    if (!frame || frame->loader()->state() != FrameStateComplete)
        return;

    IntSize size = frame->view()->visibleContentRect().size();
    RefPtr<WebImage> image = m_webPage->scaledSnapshotInDocumentCoordinates(IntRect(IntPoint(), size), 1, ImageOptionsShareable);
    ShareableBitmap::Handle bitmapHandle;
    if (!image || !image->bitmap()->createHandle(bitmapHandle))
        return;

    m_lastCaptureTime = time;
    m_webPage->send(Messages::WebProcessProxy::DidCaptureSnapshot(frame->document()->url().string(), bitmapHandle));
}

void NotifierDrawingArea::setActive()
{
    if (s_active == this)
        return;

    s_active = this;
    forceRepaint();

#if ENABLE(TVGG_THREAD_MODEL) && ENABLE(TVGG_ACCELERATED_COMPOSITING)
    enterAcceleratedCompositingMode(m_graphicsLayer);
#endif
}

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
void NotifierDrawingArea::setFocusRingPainting(bool isPainting, int scrollDirection, bool& result)
{
    if (m_isFocusRectPainting == isPainting) {
        result = true;
        return;
    }

    Frame* frame = m_webPage->corePage()->focusController()->focusedFrame();
    Node* node = frame ? frame->document()->focusedNode() : 0;

    RenderObject* renderer = 0;
    if (node) {
        if (node->hasTagName(HTMLNames::areaTag)) {
            HTMLImageElement* imageElement = static_cast<HTMLAreaElement*>(node)->imageElement();
            if (imageElement)
                renderer = imageElement->renderer();
        }
        else
            renderer = node->renderer();
    }

    if (isPainting) {
#if ENABLE(TVGG_SPATIAL_NAVIGATION)
        if (renderer && !m_webPage->corePage()->focusController()->getFocusRect(node).isEmpty()) {
            renderer->repaint();
            result = true;
        } else {
            m_webPage->corePage()->settings()->setSpatialNavigationEnabled(true);
            m_webPage->setFocusByPoint(m_focusLocation, result);
            if (!result)
                result = m_webPage->corePage()->focusController()->advanceFocus(static_cast<FocusDirection>(scrollDirection), 0);
            m_webPage->corePage()->settings()->setSpatialNavigationEnabled(false);
        }
#endif
    } else {
        if (renderer)
            renderer->repaint();
        result = true;
    }

    if (result) {
        m_isFocusRectPainting = isPainting;
        m_webPage->corePage()->settings()->setSpatialNavigationEnabled(isPainting);
    }
}
#endif

#if ENABLE(TVGG_ANIMATED_SCROLL)
void NotifierDrawingArea::displayScrollbars(UpdateInfo& updateInfo)
{
    Frame* frame = m_webPage->corePage()->mainFrame();

    IntRect bounds = m_dirtyRegion.bounds();
    IntRect dirtyRect;
	
    // display horizontal scrollbar
    if (frame->view()->horizontalScrollbar()){
        IntRect hscrollbarRect = frame->view()->horizontalScrollbar()->frameRect();

        if (hscrollbarRect.isEmpty())
            return;

        updateInfo.hscrollbarRect = hscrollbarRect;

        dirtyRect = intersection(bounds, hscrollbarRect);
        if (!dirtyRect.isEmpty()){
            RefPtr<ShareableBitmap> hscroll_bitmap = ShareableBitmap::createShareable(hscrollbarRect.size(), ShareableBitmap::SupportsAlpha);
    
            if (!hscroll_bitmap)
                return;
    
            if (!hscroll_bitmap->createHandle(updateInfo.hscroll_bitmapHandle))
                return;
    
            OwnPtr<GraphicsContext> hscroll_graphicsContext = createGraphicsContext(hscroll_bitmap.get());
            hscroll_graphicsContext->translate(-hscrollbarRect.x(), -hscrollbarRect.y());
    
            m_webPage->drawHorizontalScrollbar(*hscroll_graphicsContext, hscrollbarRect);
        }
    }
    else{
        updateInfo.hscrollbarRect = IntRect();	
    }

    // display vertical scrollbar
    if (frame->view()->verticalScrollbar()){
        IntRect vscrollbarRect = frame->view()->verticalScrollbar()->frameRect();

        if (vscrollbarRect.isEmpty())
            return;

        updateInfo.vscrollbarRect = vscrollbarRect;

        dirtyRect = intersection(bounds, vscrollbarRect);
        if (!dirtyRect.isEmpty()){
            RefPtr<ShareableBitmap> vscroll_bitmap = ShareableBitmap::createShareable(vscrollbarRect.size(), ShareableBitmap::SupportsAlpha);
    
            if (!vscroll_bitmap)
                return;
    
            if (!vscroll_bitmap->createHandle(updateInfo.vscroll_bitmapHandle))
                return;
    
            OwnPtr<GraphicsContext> vscroll_graphicsContext = createGraphicsContext(vscroll_bitmap.get());
            vscroll_graphicsContext->translate(-vscrollbarRect.x(), -vscrollbarRect.y());
    
            m_webPage->drawVerticalScrollbar(*vscroll_graphicsContext, vscrollbarRect);
        }
    }
    else{
        updateInfo.vscrollbarRect = IntRect();	
    }

}

void NotifierDrawingArea::setDisableAnimationFlag(bool flag)
{ 
    m_disableAnimationFlag = flag; 
    return;
}

#endif

} // namespace WebKit
