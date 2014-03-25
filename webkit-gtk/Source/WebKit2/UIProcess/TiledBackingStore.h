#ifndef TiledBackingStore_h
#define TiledBackingStore_h

#include <WebCore/IntSize.h>
#include <wtf/Noncopyable.h>
#include <wtf/PassOwnPtr.h>

#if PLATFORM(MAC)
#include <wtf/RetainPtr.h>
#elif PLATFORM(WIN)
#include <wtf/OwnPtr.h>
#endif

#if PLATFORM(QT)
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#endif

#if PLATFORM(GTK)
#include <RefPtrCairo.h>
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include <WebCore/ImageBackingStore.h>
#endif
#include <WebCore/WidgetBackingStore.h>
#endif

#include "TiledBackingStoreTile.h"
#include <WebCore/FloatSize.h>
#include <WebCore/IntPoint.h>
#include <WebCore/IntRect.h>
#include <WebCore/Timer.h>
#include <wtf/Assertions.h>
#include <wtf/HashMap.h>
#include <wtf/RefPtr.h>
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
#include "WebProcessProxy.h"
#include "DrawingAreaMessages.h"
#endif

#define STATIC_BACKINGSTORE
//#define MEM_DEBUG
//#define TILE_DEBUG

namespace WebCore {
    class IntRect;
    class TiledBackingStoreInfo;
    class UIViewWindow;
}

namespace WebKit {

class ShareableBitmap;
class UpdateInfo;
class WebPageProxy;

class TiledBackingStore {
    WTF_MAKE_NONCOPYABLE(TiledBackingStore);

public:
    static PassOwnPtr<TiledBackingStore> create(const WebCore::IntSize&, WebPageProxy*);
    ~TiledBackingStore();

    const WebCore::IntSize& size() const { return m_size; }
    void setSize(const WebCore::IntSize& size); 

#if ENABLE(TVGG_ANIMATED_SCROLL)
    const WebCore::IntSize& maxSize() const { return m_maxsize; }
    WebCore::IntRect scrollRect();
    bool ishorizontalScrollbarExist() { return m_hscrollbar_backingStore ? true : false; }
    bool isverticalScrollbarExist() { return m_vscrollbar_backingStore ? true : false; }
#endif

#ifdef STATIC_BACKINGSTORE
    WebPageProxy* webPageProxy() const { return m_webPageProxy; }
    void setWebPageProxy(WebPageProxy* webPageProxy) { m_webPageProxy = webPageProxy; }
#endif

#if PLATFORM(MAC)
    typedef CGContextRef PlatformGraphicsContext;
#elif PLATFORM(WIN)
    typedef HDC PlatformGraphicsContext;
#elif PLATFORM(QT)
    typedef QPainter* PlatformGraphicsContext;
#elif PLATFORM(GTK)
    typedef cairo_t* PlatformGraphicsContext;
#endif

    void paint(PlatformGraphicsContext, const WebCore::IntRect&);
    void incorporateUpdate(const UpdateInfo&);

    Vector<RefPtr<Tile> > getVisibleTiles();

    WebCore::IntRect coverRect() const;
    WebCore::IntRect viewRect() const;
    WebCore::IntRect flickRect() const;
    WebCore::FloatSize keepAreaMultiplier() const;

    WebCore::IntRect horizontalScrollbarRect() const { return m_hscrollbarRect; }
    WebCore::IntRect verticalScrollbarRect() const { return m_vscrollbarRect; }

#if ENABLE(TVGG_ANIMATED_SCROLL)
    bool isFlicking() const { return m_flicking; }

    bool conscrollFlag() const { return m_conscrollflag; }
    void setconscrollFlag(bool flag) { m_conscrollflag = flag; }

    WebCore::IntSize conscrolloffset() {return m_conscrolloffset; }
    void setconscrolloffset(WebCore::IntSize scrolloffset) { m_conscrolloffset = scrolloffset; }

    void savePreviousOffset(int offsetX, int offsetY);

    WebCore::IntPoint getWindowPosition() const;
#endif

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    void setNeedsDisplay(const WebCore::IntRect rect);
    Vector<WebCore::IntRect> needUpdateRects() { return m_needUpdateRects; }
    void setUpdateWaitingflag(bool flag) { m_updateWaitingflag = flag; }
#endif

private:
    TiledBackingStore(const WebCore::IntSize&, WebPageProxy*);

    void incorporateUpdate(ShareableBitmap*, const UpdateInfo&);
    void scroll(const UpdateInfo&);

    typedef WebCore::Timer<TiledBackingStore> TileTimer;
    void startTileCreationTimer();
    void tileCreationTimerFired(TileTimer*);

    void createTiles();
    void updateTileBuffers(ShareableBitmap* bitmap, const UpdateInfo& updateInfo);

    WebCore::IntRect visibleRect() const;
    WebCore::IntRect contentsRect() const;
    void moveViewRect(int dx, int dy);
    void moveFlickRect(int dx, int dy);

    double tileDistance(const WebCore::IntRect& viewport, const Tile::Coordinate& tileCoordinate);
    void dropOverhangingTiles();
    void dropTilesOutsideRect(const WebCore::IntRect& keepRect);

    PassRefPtr<Tile> tileAt(const Tile::Coordinate& coordinate) const;
    void setTile(const Tile::Coordinate& coordinate, PassRefPtr<Tile> tile);
    void removeTile(const Tile::Coordinate& coordinate);

    WebCore::IntRect mapFromContents(const WebCore::IntRect& rect) const;

    WebCore::IntRect tileRectForCoordinate(const Tile::Coordinate& coordinate) const;
    Tile::Coordinate tileCoordinateForPoint(const WebCore::IntPoint& point) const;

#if ENABLE(TVGG_ANIMATED_SCROLL)
    static gboolean scrollCallback(gpointer callbackdata);
    static void scrollClear(gpointer callbackdata);
    RefPtr<cairo_surface_t> getWindowSurface();

    void paintScrollbars(cairo_t* cairocontext);
    void updateScrollbars(const UpdateInfo& updateInfo);
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebCore::TiledBackingStoreInfo tiledBackingStoreInfo();
#endif

    WebCore::IntSize m_size;
    WebPageProxy* m_webPageProxy;

#if ENABLE(TVGG_ANIMATED_SCROLL)
    WebCore::IntSize m_maxsize;
#endif

    typedef HashMap<Tile::Coordinate, RefPtr<Tile> > TileMap;
    TileMap m_tiles;

    //TileTimer* m_tileBufferUpdateTimer;
    TileTimer* m_tileCreationTimer;

    WebCore::IntSize m_tileSize;
    double m_tileCreationDelay;
    WebCore::FloatSize m_keepAreaMultiplier;
    WebCore::FloatSize m_coverAreaMultiplier;
    
    //WebCore::IntRect m_previousVisibleRect;
    float m_contentsScale;
    float m_pendingScale;

    bool m_contentsFrozen;

    WebCore::IntRect m_contentRect;

    WebCore::IntRect m_viewRect;
    WebCore::IntRect m_coverRect;

    WebCore::IntRect m_flickRect;
    int m_flickTimerID;
    bool m_flicking;
    int poffsetX;
    int poffsetY;

    int m_scrollflag;
    int m_createflag;

    bool m_conscrollflag;
    WebCore::IntSize m_conscrolloffset;

    WebCore::IntRect m_hscrollbarRect;
    WebCore::IntRect m_vscrollbarRect;

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    Vector<WebCore::IntRect> m_needUpdateRects;
    bool m_updateWaitingflag;
#endif

#if ENABLE(TVGG_ANIMATED_SCROLL)
    int m_hscrollbarCreateflag;
    int m_vscrollbarCreateflag;
    int m_hscrollbarUpdateflag;
    int m_vscrollbarUpdateflag;
#if PLATFORM(GTK)
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    unsigned m_hscrollbarTextureId;
    unsigned m_vscrollbarTextureId;
    OwnPtr<WebCore::ImageBackingStore> m_hscrollbar_backingStore;
    OwnPtr<WebCore::ImageBackingStore> m_vscrollbar_backingStore;
#else
    OwnPtr<WebCore::WidgetBackingStore> m_hscrollbar_backingStore;
    OwnPtr<WebCore::WidgetBackingStore> m_vscrollbar_backingStore;
#endif
#endif
#endif

    friend class Tile;

};

} // namespace WebKit

#endif // TiledBackingStore_h

