#include "config.h"
#include "TiledBackingStoreTile.h"

#include "WebCore/GraphicsContext.h"
#include "ShareableBitmap.h"
#include "TiledBackingStore.h"
#include "UpdateInfo.h"
#include "WebPageProxy.h"
#include "WebProcessProxy.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "GraphicsLayerRenderer.h"
#include "LayerTextureManager.h"
#include "WebPage.h"
#include "WebProcess.h"
#endif

#if ENABLE(TVGG_TILED_BACKING_STORE)
using namespace WebCore;

namespace WebKit {

Tile::Tile(TiledBackingStore* backingStore, const Coordinate& tileCoordinate)
    : m_backingStore(backingStore)
    , m_coordinate(tileCoordinate)
    , m_rect(m_backingStore->tileRectForCoordinate(tileCoordinate))
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        WebPage* page = WebProcess::shared().mainUIWebPage();
        void* ptr = 0;
        if (page && page->getRenderer()) {
            LayerTextureManager* textureManager = page->getRenderer()->textureManager();
            setTextureID(textureManager->createTexture());
            textureManager->updateTexture(getTextureID(), size(), 0);
            textureManager->getBuffer(getTextureID(), &ptr, 0, 0);
        }

        ASSERT(ptr);

        m_buffer = ImageBackingStore::create(ptr, size());
#else
        m_buffer = WidgetBackingStore::create(m_backingStore->m_webPageProxy->viewWidget(), size());
#endif

        cairo_t *cr = cairo_create(m_buffer->cairoSurface());

        cairo_rectangle(cr, 0, 0, size().width(), size().height());
        cairo_set_source_rgb(cr, 1, 1 ,1);
        cairo_fill(cr);
        cairo_destroy(cr);

#ifdef TILE_DEBUG
    FILE *fp = fopen("/mtd_down/tile.log", "a");
    fprintf(fp, "[TiledBackingStore] [%s %d] Tiled (%d, %d) created!!!!\n", __func__, __LINE__, m_coordinate.x(), m_coordinate.y());
    fclose(fp);
#endif		
}

Tile::~Tile()
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer()) {
        LayerTextureManager* textureManager = page->getRenderer()->textureManager();
        textureManager->deleteTexture(getTextureID());
    }
#endif
#ifdef TILE_DEBUG
    FILE *fp = fopen("/mtd_down/tile.log", "a");
    fprintf(fp, "[TiledBackingStore] [%s %d] Tiled (%d, %d) destroy!!!!\n", __func__, __LINE__,  m_coordinate.x(), m_coordinate.y());
    fclose(fp);
#endif
}

bool Tile::isReadyToPaint() const
{ 
    return m_buffer; 
}

void Tile::paint(cairo_t* context, const IntRect& dirtyRect)
{
    ASSERT(!m_buffer.isNull());

    IntRect viewRect;
    if(m_backingStore->isFlicking())
        viewRect = m_backingStore->flickRect();
    else
        viewRect = m_backingStore->viewRect();

    IntRect target_mRect = m_rect;
    target_mRect.move(-viewRect.x(), -viewRect.y());

    IntRect target_dirtyRect = dirtyRect;
    target_dirtyRect.move(-viewRect.x(), -viewRect.y());

    cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_surface(context, m_buffer->cairoSurface(), target_mRect.x(), target_mRect.y());
    cairo_rectangle(context, target_dirtyRect.x(), target_dirtyRect.y(), target_dirtyRect.width(), target_dirtyRect.height());
    cairo_fill(context);

}

void Tile::refreshScrollbarBackgroundColor(IntRect& dirtyRect)
{
    RefPtr<cairo_t> context(adoptRef(cairo_create(m_buffer->cairoSurface())));

    IntRect viewRect = m_backingStore->viewRect();
    IntRect checkRect;

    IntRect hscrollbarRect = m_backingStore->horizontalScrollbarRect();
    if(!hscrollbarRect.isEmpty()){
        hscrollbarRect.move(viewRect.x(), viewRect.y());
        checkRect = intersection(dirtyRect, hscrollbarRect);
        if(!checkRect.isEmpty()){
            checkRect.move(-m_rect.x(), -m_rect.y()); 
            cairo_rectangle(context.get(), checkRect.x(), checkRect.y(), checkRect.width(), checkRect.height());
            cairo_set_source_rgb(context.get(), 1, 1 ,1);
            cairo_fill(context.get());
        }
    }

    IntRect vscrollbarRect = m_backingStore->verticalScrollbarRect();
    if(!vscrollbarRect.isEmpty()){
        vscrollbarRect.move(viewRect.x(), viewRect.y());
        checkRect = intersection(dirtyRect, vscrollbarRect);
        if(!checkRect.isEmpty()){
            checkRect.move(-m_rect.x(), -m_rect.y()); 
            cairo_rectangle(context.get(), checkRect.x(), checkRect.y(), checkRect.width(), checkRect.height());
            cairo_set_source_rgb(context.get(), 1, 1 ,1);
            cairo_fill(context.get());
        }
    }

}

void Tile::updateBackBuffer(ShareableBitmap* bitmap, IntRect& updateRectBounds, IntRect& dirtyRect)
{
#if !ENABLE(TVGG_ANIMATED_SCROLL)
    refreshScrollbarBackgroundColor(dirtyRect);
#endif

    RefPtr<cairo_t> context(adoptRef(cairo_create(m_buffer->cairoSurface())));

    //Transform updateRectBound to target(tile) base coordinate.
    IntPoint srcOriginPoint = updateRectBounds.location();
    srcOriginPoint.move(-m_rect.x(), -m_rect.y()); 
	
    //Transform dirtyRect to target(tile) base coordinate.
    IntRect updateRect = dirtyRect;
    updateRect.move(-m_rect.x(), -m_rect.y()); 

    RefPtr<cairo_surface_t> bitmapSurface = bitmap->createCairoSurface();
    IntSize bitsize = bitmap->size();

    cairo_set_operator(context.get(), CAIRO_OPERATOR_SOURCE);
    cairo_set_source_surface(context.get(), bitmapSurface.get(), srcOriginPoint.x(), srcOriginPoint.y());
    cairo_rectangle(context.get(), updateRect.x(), updateRect.y(), updateRect.width(), updateRect.height());
    cairo_fill(context.get());

/*
    cairo_set_source_rgba(context.get(), 1.f, 0, 0, 1.f);
    cairo_rectangle(context.get(), updateRect.x(), updateRect.y(), updateRect.width(), updateRect.height());
    cairo_set_operator(context.get(), CAIRO_OPERATOR_OVER);
    cairo_fill(context.get());
*/

}

/*
void Tile::updateBackBuffer(ShareableBitmap* bitmap, IntRect& updateRectBounds, IntRect& dirtyRect)
{
#if !ENABLE(TVGG_ANIMATED_SCROLL)
    refreshScrollbarBackgroundColor(dirtyRect);
#endif

    IntRect updateRect = dirtyRect;
    updateRect.move(-m_rect.x(), -m_rect.y()); 

    IntRect srcRect = dirtyRect;
    srcRect.move(-updateRectBounds.x(), -updateRectBounds.y());
    
    RefPtr<cairo_t> context(adoptRef(cairo_create(m_buffer->cairoSurface())));
    GraphicsContext graphicsContext(context.get());

    bitmap->paint(graphicsContext, updateRect.location(), srcRect);
}
*/
}
#endif
