#include "config.h"
#include "NotifierDrawingAreaProxy.h"

#include "DrawingAreaMessages.h"
#include "DrawingAreaProxyMessages.h"
#include "LayerTreeContext.h"
#include "Region.h"
#include "UpdateInfo.h"
#include "WebPageProxy.h"
#include "WebProcessProxy.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
#include "GraphicsContext3D.h"
#include "GraphicsLayerRenderer.h"
#include "WebPage.h"
#include "WebProcess.h"
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
#include "WebContext.h"
#endif

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
#include "WebPageGroup.h"
#include "WebPreferences.h"
#endif

using namespace WebCore;

namespace WebKit {

#ifdef STATIC_BACKINGSTORE
OwnPtr<TiledBackingStore> NotifierDrawingAreaProxy::m_backingStore = TiledBackingStore::create(WebCore::IntSize(), NULL);
#endif
NotifierDrawingAreaProxy* NotifierDrawingAreaProxy::s_active = 0;

PassOwnPtr<NotifierDrawingAreaProxy> NotifierDrawingAreaProxy::create(WebPageProxy* webPageProxy)
{
    return adoptPtr(new NotifierDrawingAreaProxy(webPageProxy));
}

NotifierDrawingAreaProxy::NotifierDrawingAreaProxy(WebPageProxy* webPageProxy)
    : DrawingAreaProxy(DrawingAreaTypeNotifier, webPageProxy)
    , m_currentBackingStoreStateID(0)
    , m_nextBackingStoreStateID(0)
    , m_isWaitingForDidUpdateBackingStoreState(false)
    , m_hasReceivedFirstUpdate(false)
    , m_isBackingStoreDiscardable(true)
    , m_discardBackingStoreTimer(RunLoop::current(), this, &NotifierDrawingAreaProxy::discardBackingStore)
    , m_isThreadModel(false)
#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    , m_isFocusRectPainting(webPageProxy->pageGroup()->preferences()->store().getBoolValueForKey(WebPreferencesKey::isSpatialNavigationEnabledKey()))
#endif
{
#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    if (webPageProxy->process()->context()->processModel() == ProcessModelSharedSecondaryThread)
        m_isThreadModel = true;
#endif

    if (!s_active)
        s_active = this;
}

NotifierDrawingAreaProxy::~NotifierDrawingAreaProxy()
{
#if USE(ACCELERATED_COMPOSITING)
    // Make sure to exit accelerated compositing mode.
    if (isInAcceleratedCompositingMode())
        exitAcceleratedCompositingMode();
#endif

    if (s_active == this)
        s_active = 0;
}

void NotifierDrawingAreaProxy::paint(BackingStore::PlatformGraphicsContext context, const IntRect& rect, Region& unpaintedRegion)
{
    unpaintedRegion = rect;

    if (isInAcceleratedCompositingMode())
        return;

    ASSERT(m_currentBackingStoreStateID <= m_nextBackingStoreStateID);
    if (m_currentBackingStoreStateID < m_nextBackingStoreStateID) {
        // Tell the web process to do a full backing store update now, in case we previously told
        // it about our next state but didn't request an immediate update.
        sendUpdateBackingStoreState(RespondImmediately);

        // If we haven't yet received our first bits from the WebProcess then don't paint anything.
        if (!m_hasReceivedFirstUpdate)
            return;
        
        if (m_isWaitingForDidUpdateBackingStoreState) {
            // Wait for a DidUpdateBackingStoreState message that contains the new bits before we paint
            // what's currently in the backing store.
            waitForAndDispatchDidUpdateBackingStoreState();
        }

        // Dispatching DidUpdateBackingStoreState (either beneath sendUpdateBackingStoreState or
        // beneath waitForAndDispatchDidUpdateBackingStoreState) could destroy our backing store or
        // change the compositing mode.
        if (!m_backingStore || isInAcceleratedCompositingMode())
            return;
    } else {
        ASSERT(!m_isWaitingForDidUpdateBackingStoreState);
        if (!m_backingStore) {
            // The view has asked us to paint before the web process has painted anything. There's
            // nothing we can do.
            return;
        }
    }

    m_backingStore->paint(context, rect);
    unpaintedRegion.subtract(IntRect(IntPoint(), m_backingStore->size()));

#if !ENABLE(TVGG_BROWSER)
    discardBackingStoreSoon();
#endif
}

bool NotifierDrawingAreaProxy::paint(const WebCore::IntRect&, PlatformDrawingContext)
{
    ASSERT_NOT_REACHED();
    return false;
}

void NotifierDrawingAreaProxy::setActive()
{
    if (s_active == this)
        return;

    s_active = this;
#ifndef STATIC_BACKINGSTORE
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    m_backingStore = nullptr;
#endif
#endif
    m_webPageProxy->process()->send(Messages::DrawingArea::SetActive(), m_webPageProxy->pageID());
}

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
bool NotifierDrawingAreaProxy::setFocusRectPainting(bool isPainting, int scrollDirection)
{
    if (isPainting != m_isFocusRectPainting) {
        bool result;
        m_webPageProxy->process()->sendSync(Messages::DrawingArea::SetFocusRingPainting(isPainting, scrollDirection), Messages::DrawingArea::SetFocusRingPainting::Reply(result), m_webPageProxy->pageID(), CoreIPC::Connection::NoTimeout);
        if (result)
            m_isFocusRectPainting = isPainting;
        return result;
    } else
        return true;
}
#endif


#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
BackingStore* NotifierDrawingAreaProxy::getBackingStore()
{
    if (!m_backingStore || m_backingStore->size() != size())
        m_backingStore = BackingStore::create(m_webPageProxy->viewSize(), m_webPageProxy);

    return m_backingStore.get();
}
#endif

void NotifierDrawingAreaProxy::sizeDidChange()
{
    backingStoreStateDidChange(RespondImmediately);
#if ENABLE(TVGG_TILED_BACKING_STORE)
    if(m_backingStore)
        m_backingStore->setSize(m_size);
#endif
}

void NotifierDrawingAreaProxy::visibilityDidChange()
{
    if (!m_webPageProxy->isViewVisible()) {
        // Suspend painting.
        m_webPageProxy->process()->send(Messages::DrawingArea::SuspendPainting(), m_webPageProxy->pageID());
        return;
    }

    // Resume painting.
    m_webPageProxy->process()->send(Messages::DrawingArea::ResumePainting(), m_webPageProxy->pageID());

#if USE(ACCELERATED_COMPOSITING)
    // If we don't have a backing store, go ahead and mark the backing store as being changed so
    // that when paint we'll actually wait for something to paint and not flash white.
    if (!m_backingStore && m_layerTreeContext.isEmpty())
        backingStoreStateDidChange(DoNotRespondImmediately);
#endif
}

void NotifierDrawingAreaProxy::setPageIsVisible(bool)
{
}

void NotifierDrawingAreaProxy::setBackingStoreIsDiscardable(bool isBackingStoreDiscardable)
{
    if (m_isBackingStoreDiscardable == isBackingStoreDiscardable)
        return;

    m_isBackingStoreDiscardable = isBackingStoreDiscardable;
    if (m_isBackingStoreDiscardable)
        discardBackingStoreSoon();
    else
        m_discardBackingStoreTimer.stop();
}

void NotifierDrawingAreaProxy::waitForBackingStoreUpdateOnNextPaint()
{
    m_hasReceivedFirstUpdate = true;
}

void NotifierDrawingAreaProxy::update(uint64_t backingStoreStateID, const UpdateInfo& updateInfo)
{
    ASSERT_ARG(backingStoreStateID, backingStoreStateID <= m_currentBackingStoreStateID);
    if (backingStoreStateID < m_currentBackingStoreStateID)
        return;

    // FIXME: Handle the case where the view is hidden.

    incorporateUpdate(updateInfo);
    m_webPageProxy->process()->send(Messages::DrawingArea::DidUpdate(), m_webPageProxy->pageID());
}

void NotifierDrawingAreaProxy::didUpdateBackingStoreState(uint64_t backingStoreStateID, const UpdateInfo& updateInfo, const LayerTreeContext& layerTreeContext)
{
    ASSERT_ARG(backingStoreStateID, backingStoreStateID <= m_nextBackingStoreStateID);
    ASSERT_ARG(backingStoreStateID, backingStoreStateID > m_currentBackingStoreStateID);
    m_currentBackingStoreStateID = backingStoreStateID;

    m_isWaitingForDidUpdateBackingStoreState = false;

    // Stop the responsiveness timer that was started in sendUpdateBackingStoreState.
    m_webPageProxy->process()->responsivenessTimer()->stop();

    if (m_nextBackingStoreStateID != m_currentBackingStoreStateID)
        sendUpdateBackingStoreState(RespondImmediately);
    else
        m_hasReceivedFirstUpdate = true;

#if USE(ACCELERATED_COMPOSITING)
    if (layerTreeContext != m_layerTreeContext) {
        if (!m_layerTreeContext.isEmpty()) {
            exitAcceleratedCompositingMode();
            ASSERT(m_layerTreeContext.isEmpty());
        }

        if (!layerTreeContext.isEmpty()) {
            enterAcceleratedCompositingMode(layerTreeContext);
            ASSERT(layerTreeContext == m_layerTreeContext);
        }
    }

    if (isInAcceleratedCompositingMode()) {
        ASSERT(!m_backingStore);
        return;
    }
#endif

    if (m_isThreadModel == false) {
#if !ENABLE(TVGG_TILED_BACKING_STORE)
        // If we have a backing store the right size, reuse it.
        if (m_backingStore && m_backingStore->size() != updateInfo.viewSize)
            m_backingStore = nullptr;
#endif
        incorporateUpdate(updateInfo);
    }
}

void NotifierDrawingAreaProxy::enterAcceleratedCompositingMode(uint64_t backingStoreStateID, const LayerTreeContext& layerTreeContext)
{
    ASSERT_ARG(backingStoreStateID, backingStoreStateID <= m_currentBackingStoreStateID);
    if (backingStoreStateID < m_currentBackingStoreStateID)
        return;

#if USE(ACCELERATED_COMPOSITING)
    enterAcceleratedCompositingMode(layerTreeContext);
#endif
}

void NotifierDrawingAreaProxy::exitAcceleratedCompositingMode(uint64_t backingStoreStateID, const UpdateInfo& updateInfo)
{
    ASSERT_ARG(backingStoreStateID, backingStoreStateID <= m_currentBackingStoreStateID);
    if (backingStoreStateID < m_currentBackingStoreStateID)
        return;

#if USE(ACCELERATED_COMPOSITING)
    exitAcceleratedCompositingMode();
#endif

    incorporateUpdate(updateInfo);
}

void NotifierDrawingAreaProxy::incorporateUpdate(const UpdateInfo& updateInfo)
{
    ASSERT(!isInAcceleratedCompositingMode());

    if (updateInfo.updateRectBounds.isEmpty())
        return;

#if ENABLE(TVGG_TILED_BACKING_STORE)
#ifdef STATIC_BACKINGSTORE
    if (m_backingStore && m_backingStore->size() != updateInfo.viewSize)
        m_backingStore->setSize(updateInfo.viewSize);
    if (m_backingStore && m_backingStore->webPageProxy() != m_webPageProxy)
        m_backingStore->setWebPageProxy(m_webPageProxy);
#endif
    if (!m_backingStore)
        m_backingStore = TiledBackingStore::create(updateInfo.viewSize, m_webPageProxy);
#else
    if (!m_backingStore)
        m_backingStore = BackingStore::create(updateInfo.viewSize, m_webPageProxy);
#endif
    if (m_isThreadModel == false)
        m_backingStore->incorporateUpdate(updateInfo);

    bool shouldScroll = !updateInfo.scrollRect.isEmpty();

#if ENABLE(TVGG_ANIMATED_SCROLL)
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    IntRect updateRect = IntRect();
    IntRect visibleRect = updateInfo.visibleRect;
    visibleRect.setLocation(IntPoint());
#endif
    if (shouldScroll && updateInfo.disableAnimation) 
        m_webPageProxy->scrollView(updateInfo.scrollRect, updateInfo.scrollOffset);

    if (!shouldScroll || updateInfo.disableAnimation){
        for (size_t i = 0; i < updateInfo.updateRects.size(); ++i){
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
            updateRect = updateInfo.updateRects[i];
            updateRect.intersect(visibleRect);
            m_webPageProxy->setViewNeedsDisplay(updateRect);
#else
            m_webPageProxy->setViewNeedsDisplay(updateInfo.updateRects[i]);
#endif
        }
    }
#else
    if (shouldScroll)
        m_webPageProxy->scrollView(updateInfo.scrollRect, updateInfo.scrollOffset);

    for (size_t i = 0; i < updateInfo.updateRects.size(); ++i)
        m_webPageProxy->setViewNeedsDisplay(updateInfo.updateRects[i]);
#endif
    if (WebPageProxy::debugPaintFlags() & kWKDebugFlashBackingStoreUpdates)
        m_webPageProxy->flashBackingStoreUpdates(updateInfo.updateRects);

    if (shouldScroll)
        m_webPageProxy->displayView();
}

void NotifierDrawingAreaProxy::backingStoreStateDidChange(RespondImmediatelyOrNot respondImmediatelyOrNot)
{
    ++m_nextBackingStoreStateID;
    sendUpdateBackingStoreState(respondImmediatelyOrNot);
}

void NotifierDrawingAreaProxy::sendUpdateBackingStoreState(RespondImmediatelyOrNot respondImmediatelyOrNot)
{
    ASSERT(m_currentBackingStoreStateID < m_nextBackingStoreStateID);

    if (!m_webPageProxy->isValid())
        return;

    if (m_isWaitingForDidUpdateBackingStoreState)
        return;

    if (m_webPageProxy->viewSize().isEmpty())
        return;

    m_isWaitingForDidUpdateBackingStoreState = respondImmediatelyOrNot == RespondImmediately;

#if ENABLE(TVGG_THREAD_MODEL) &&ENABLE(TVGG_ACCELERATED_COMPOSITING)
    m_isWaitingForDidUpdateBackingStoreState = false;
    respondImmediatelyOrNot = DoNotRespondImmediately;
#endif

    m_webPageProxy->process()->send(Messages::DrawingArea::UpdateBackingStoreState(m_nextBackingStoreStateID, respondImmediatelyOrNot == RespondImmediately, m_size, m_scrollOffset), m_webPageProxy->pageID());
    m_scrollOffset = IntSize();

    if (m_isWaitingForDidUpdateBackingStoreState) {
        // Start the responsiveness timer. We will stop it when we hear back from the WebProcess
        // in didUpdateBackingStoreState.
        m_webPageProxy->process()->responsivenessTimer()->start();
    }

#if USE(ACCELERATED_COMPOSITING)
    if (m_isWaitingForDidUpdateBackingStoreState && !m_layerTreeContext.isEmpty()) {
        // Wait for the DidUpdateBackingStoreState message. Normally we do this in NotifierDrawingAreaProxy::paint, but that
        // function is never called when in accelerated compositing mode.
        waitForAndDispatchDidUpdateBackingStoreState();
    }
#endif
}

void NotifierDrawingAreaProxy::waitForAndDispatchDidUpdateBackingStoreState()
{
    ASSERT(m_isWaitingForDidUpdateBackingStoreState);

    if (!m_webPageProxy->isValid())
        return;
    if (m_webPageProxy->process()->isLaunching())
        return;

#if USE(ACCELERATED_COMPOSITING)
    // FIXME: waitForAndDispatchImmediately will always return the oldest DidUpdateBackingStoreState message that
    // hasn't yet been processed. But it might be better to skip ahead to some other DidUpdateBackingStoreState
    // message, if multiple DidUpdateBackingStoreState messages are waiting to be processed. For instance, we could
    // choose the most recent one, or the one that is closest to our current size.

    // The timeout, in seconds, we use when waiting for a DidUpdateBackingStoreState message when we're asked to paint.
    static const double didUpdateBackingStoreStateTimeout = 0.5;
    m_webPageProxy->process()->connection()->waitForAndDispatchImmediately<Messages::DrawingAreaProxy::DidUpdateBackingStoreState>(m_webPageProxy->pageID(), didUpdateBackingStoreStateTimeout);
#endif
}

#if USE(ACCELERATED_COMPOSITING)
void NotifierDrawingAreaProxy::enterAcceleratedCompositingMode(const LayerTreeContext& layerTreeContext)
{
    ASSERT(!isInAcceleratedCompositingMode());

    m_backingStore = nullptr;
    m_layerTreeContext = layerTreeContext;
    m_webPageProxy->enterAcceleratedCompositingMode(layerTreeContext);
}

void NotifierDrawingAreaProxy::exitAcceleratedCompositingMode()
{
    ASSERT(isInAcceleratedCompositingMode());

    m_layerTreeContext = LayerTreeContext();
    m_webPageProxy->exitAcceleratedCompositingMode();
}
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
void NotifierDrawingAreaProxy::setUmpID(unsigned int frontID, unsigned int backID)
{
    WebPage *page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer())
        page->getRenderer()->setUmpID(frontID, backID);
}

void NotifierDrawingAreaProxy::enterContentsViewAcceleratedCompositingMode(bool mode)
{
    WebPage *page = WebProcess::shared().mainUIWebPage();
    if (!mode && page && page->getRenderer())
        page->getRenderer()->setUmpID(0, 0);
}

void NotifierDrawingAreaProxy::swapUmpID()
{
    WebPage *page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer()) {
        page->getRenderer()->swapUmpID();
        page->drawingArea()->scheduleCompositingLayerSync();
    }
}
#endif

void NotifierDrawingAreaProxy::discardBackingStoreSoon()
{
    if (!m_isBackingStoreDiscardable || m_discardBackingStoreTimer.isActive())
        return;

    // We'll wait this many seconds after the last paint before throwing away our backing store to save memory.
    // FIXME: It would be smarter to make this delay based on how expensive painting is. See <http://webkit.org/b/55733>.
    static const double discardBackingStoreDelay = 2;

    m_discardBackingStoreTimer.startOneShot(discardBackingStoreDelay);
}

void NotifierDrawingAreaProxy::discardBackingStore()
{
    m_backingStore = nullptr;
    backingStoreStateDidChange(DoNotRespondImmediately);
}

} // namespace WebKit
