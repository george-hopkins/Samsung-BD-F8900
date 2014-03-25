#ifndef DirectDrawingArea_h
#define DirectDrawingArea_h

#include "DrawingArea.h"
#include "LayerTreeHost.h"
#include "Region.h"
#include "RunLoop.h"

namespace WebCore {
    class GraphicsContext;
}

namespace WebKit {

class ShareableBitmap;
class UpdateInfo;

class DirectDrawingArea : public DrawingArea {
public:
    static PassOwnPtr<DirectDrawingArea> create(WebPage*, const WebPageCreationParameters&);
    virtual ~DirectDrawingArea();

    void layerHostDidFlushLayers();

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
    static gboolean composite(gpointer cbData);
    void forceComposite();
    bool needsComposite();
    void setNeedsComposite(bool needsComposite) { m_needsComposite = needsComposite; }
#endif
private:
    DirectDrawingArea(WebPage*, const WebPageCreationParameters&);

    // DrawingArea
    virtual void setNeedsDisplay(const WebCore::IntRect&);
    virtual void scroll(const WebCore::IntRect& scrollRect, const WebCore::IntSize& scrollOffset);
    virtual void setLayerTreeStateIsFrozen(bool);
    virtual void forceRepaint();

    virtual void enableDisplayThrottling();
    virtual void disableDisplayThrottling();

    virtual void didInstallPageOverlay();
    virtual void didUninstallPageOverlay();
    virtual void setPageOverlayNeedsDisplay(const WebCore::IntRect&);
    
    virtual void setRootCompositingLayer(WebCore::GraphicsLayer*);
    virtual void scheduleCompositingLayerSync();
    virtual void syncCompositingLayers();

#if PLATFORM(WIN)
    virtual void scheduleChildWindowGeometryUpdate(const WindowGeometry&);
#endif

    // CoreIPC message handlers.
    virtual void updateBackingStoreState(uint64_t backingStoreStateID, bool respondImmediately, const WebCore::IntSize&, const WebCore::IntSize& scrollOffset);
    virtual void didUpdate();
    virtual void suspendPainting();
    virtual void resumePainting();

    void sendDidUpdateBackingStoreState();

    void enterAcceleratedCompositingMode(WebCore::GraphicsLayer*);
    void exitAcceleratedCompositingModeSoon();
    bool exitAcceleratedCompositingModePending() const { return m_exitCompositingTimer.isActive(); }
    void exitAcceleratedCompositingMode();

    void scheduleDisplay();
    void displayTimerFired();
    void display();
    void display(UpdateInfo&);
    PassOwnPtr<WebCore::GraphicsContext> createGraphicsContext(ShareableBitmap*);

    uint64_t m_backingStoreStateID;

    Region m_dirtyRegion;
    WebCore::IntRect m_scrollRect;
    WebCore::IntSize m_scrollOffset;

    // Whether we're currently processing an UpdateBackingStoreState message.
    bool m_inUpdateBackingStoreState;

    // When true, we should send an UpdateBackingStoreState message instead of any other messages
    // we normally send to the UI process.
    bool m_shouldSendDidUpdateBackingStoreState;

    // Whether we're waiting for a DidUpdate message. Used for throttling paints so that the 
    // web process won't paint more frequent than the UI process can handle.
    bool m_isWaitingForDidUpdate;
    
    // True between sending the 'enter compositing' messages, and the 'exit compositing' message.
    bool m_compositingAccordingToProxyMessages;

    // When true, we maintain the layer tree in its current state by not leaving accelerated compositing mode
    // and not scheduling layer flushes.
    bool m_layerTreeStateIsFrozen;

    // True when we were asked to exit accelerated compositing mode but couldn't because layer tree
    // state was frozen.
    bool m_wantsToExitAcceleratedCompositingMode;

    // Whether painting is suspended. We'll still keep track of the dirty region but we 
    // won't paint until painting has resumed again.
    bool m_isPaintingSuspended;
    bool m_alwaysUseCompositing;

    // Whether we should throttle displays to a set update rate on the WebProcess side.
    bool m_shouldThrottleDisplay;

    double m_lastDisplayTime;

    RunLoop::Timer<DirectDrawingArea> m_displayTimer;
    RunLoop::Timer<DirectDrawingArea> m_exitCompositingTimer;

    // The layer tree host that handles accelerated compositing.
    RefPtr<LayerTreeHost> m_layerTreeHost;
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
    bool m_needsComposite;
    int m_tid;
#endif
};

} // namespace WebKit

#endif // DirectDrawingArea_h
