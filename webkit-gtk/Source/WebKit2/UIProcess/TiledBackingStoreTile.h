#ifndef TiledBackingStoreTile_h
#define TiledBackingStoreTile_h

#include <RefPtrCairo.h>
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include <WebCore/ImageBackingStore.h>
#else
#include <WebCore/WidgetBackingStore.h>
#endif

#include "IntPointHash.h"
#include "WebCore/GraphicsContext.h"
#include "WebCore/IntPoint.h"
#include "WebCore/IntRect.h"
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

namespace WebKit {

class ShareableBitmap;
class TiledBackingStore;

class Tile : public RefCounted<Tile> {
public:
    typedef WebCore::IntPoint Coordinate;

    static PassRefPtr<Tile> create(TiledBackingStore* backingStore, const Coordinate& tileCoordinate) { return adoptRef(new Tile(backingStore, tileCoordinate)); }
    ~Tile();
    
    void updateBackBuffer(ShareableBitmap*, WebCore::IntRect&, WebCore::IntRect&);

    bool isReadyToPaint() const;
    void paint(cairo_t*, const WebCore::IntRect&);

    const Tile::Coordinate& coordinate() const { return m_coordinate; }
    const WebCore::IntRect& rect() const { return m_rect; }
    const WebCore::IntSize& size() const { return m_rect.size(); }
    const unsigned getTextureID() const { return m_textureID; }
    void setTextureID(unsigned textureid) { m_textureID = textureid; }
		
private:
    Tile(TiledBackingStore*, const Coordinate&);
    void refreshScrollbarBackgroundColor(WebCore::IntRect& dirtyRect);
    
    TiledBackingStore* m_backingStore;
    Coordinate m_coordinate;
    WebCore::IntRect m_rect;
    unsigned m_textureID;

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    OwnPtr<WebCore::ImageBackingStore> m_buffer;
#else
    OwnPtr<WebCore::WidgetBackingStore> m_buffer;
#endif

};

}
#endif // TiledBackingStoreTile_h
