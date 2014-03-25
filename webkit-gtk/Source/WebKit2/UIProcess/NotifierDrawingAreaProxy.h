#ifndef NotifierDrawingAreaProxy_h
#define NotifierDrawingAreaProxy_h

#if ENABLE(TVGG_TILED_BACKING_STORE)
#include "TiledBackingStore.h"
#endif

#include "BackingStore.h"
#include "DrawingAreaProxy.h"
#include "LayerTreeContext.h"
#include "RunLoop.h"
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>

namespace WebKit {

class Region;

class NotifierDrawingAreaProxy : public DrawingAreaProxy {
public:
    static PassOwnPtr<NotifierDrawingAreaProxy> create(WebPageProxy*);
    virtual ~NotifierDrawingAreaProxy();

    void paint(BackingStore::PlatformGraphicsContext, const WebCore::IntRect&, Region& unpaintedRegion);

    void setActive();

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    bool setFocusRectPainting(bool, int);
#endif

#if ENABLE(TVGG_DISABLE_SHARED_BITMAP)
    BackingStore* getBackingStore();
#endif

private:
    explicit NotifierDrawingAreaProxy(WebPageProxy*);

    // DrawingAreaProxy
    virtual bool paint(const WebCore::IntRect&, PlatformDrawingContext);
    virtual void sizeDidChange();
    virtual void visibilityDidChange();
    virtual void setPageIsVisible(bool);
    virtual void setBackingStoreIsDiscardable(bool);
    virtual void waitForBackingStoreUpdateOnNextPaint();

    // CoreIPC message handlers
    virtual void update(uint64_t backingStoreStateID, const UpdateInfo&);
    virtual void didUpdateBackingStoreState(uint64_t backingStoreStateID, const UpdateInfo&, const LayerTreeContext&);
    virtual void enterAcceleratedCompositingMode(uint64_t backingStoreStateID, const LayerTreeContext&);
    virtual void exitAcceleratedCompositingMode(uint64_t backingStoreStateID, const UpdateInfo&);

    void incorporateUpdate(const UpdateInfo&);

    enum RespondImmediatelyOrNot { DoNotRespondImmediately, RespondImmediately };
    void backingStoreStateDidChange(RespondImmediatelyOrNot);
    void sendUpdateBackingStoreState(RespondImmediatelyOrNot);
    void waitForAndDispatchDidUpdateBackingStoreState();

#if USE(ACCELERATED_COMPOSITING)
    void enterAcceleratedCompositingMode(const LayerTreeContext&);
    void exitAcceleratedCompositingMode();

    bool isInAcceleratedCompositingMode() const { return !m_layerTreeContext.isEmpty(); }
#else
    bool isInAcceleratedCompositingMode() const { return false; }
#endif
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    virtual void setUmpID(unsigned int frontID, unsigned int backID);
    virtual void enterContentsViewAcceleratedCompositingMode(bool mode);
    virtual void swapUmpID();
#endif

    void discardBackingStoreSoon();
    void discardBackingStore();

    // The state ID corresponding to our current backing store. Updated whenever we allocate
    // a new backing store. Any messages received that correspond to an earlier state are ignored,
    // as they don't apply to our current backing store.
    uint64_t m_currentBackingStoreStateID;

    // The next backing store state ID we will request the web process update to. Incremented
    // whenever our state changes in a way that will require a new backing store to be allocated.
    uint64_t m_nextBackingStoreStateID;

#if USE(ACCELERATED_COMPOSITING)
    // The current layer tree context.
    LayerTreeContext m_layerTreeContext;
#endif

    // Whether we've sent a UpdateBackingStoreState message and are now waiting for a DidUpdateBackingStoreState message.
    // Used to throttle UpdateBackingStoreState messages so we don't send them faster than the Web process can handle.
    bool m_isWaitingForDidUpdateBackingStoreState;
    
    // For a new Drawing Area don't draw anything until the WebProcess has sent over the first content.
    bool m_hasReceivedFirstUpdate;

    bool m_isBackingStoreDiscardable;
#if ENABLE(TVGG_TILED_BACKING_STORE)
#ifdef STATIC_BACKINGSTORE
    static OwnPtr<TiledBackingStore> m_backingStore;
#else
    OwnPtr<TiledBackingStore> m_backingStore;
#endif
#else
    OwnPtr<BackingStore> m_backingStore;
#endif

    RunLoop::Timer<NotifierDrawingAreaProxy> m_discardBackingStoreTimer;

    bool m_isThreadModel;

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    bool m_isFocusRectPainting;
#endif

    static NotifierDrawingAreaProxy* s_active;
};

} // namespace WebKit

#endif // NotifierDrawingAreaProxy_h
