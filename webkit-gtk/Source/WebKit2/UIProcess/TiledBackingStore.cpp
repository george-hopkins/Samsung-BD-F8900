#include "config.h"
#include "TiledBackingStore.h"

#include "ShareableBitmap.h"
#include "UpdateInfo.h"

#if ENABLE(TVGG_TILED_BACKING_STORE)
using namespace WebCore;

namespace WebKit {

static const int defaultTileWidth = 256;
static const int defaultTileHeight = 256;

PassOwnPtr<TiledBackingStore> TiledBackingStore::create(const IntSize& size, WebPageProxy* webPageProxy)
{
    return adoptPtr(new TiledBackingStore(size, webPageProxy));
}

TiledBackingStore::TiledBackingStore(const IntSize& size, WebPageProxy* webPageProxy)
    : m_size(size)
    , m_webPageProxy(webPageProxy)
    , m_tileCreationTimer(new TileTimer(this, &TiledBackingStore::tileCreationTimerFired))
    , m_tileSize(defaultTileWidth, defaultTileHeight)
    , m_tileCreationDelay(0.01)
#if ENABLE(TVGG_EXTENDED_BACKING_STORE) 
    , m_keepAreaMultiplier(2.f, 3.5f)
    , m_coverAreaMultiplier(1.5f, 3.f)
#else
    , m_keepAreaMultiplier(1.f, 1.f)
    , m_coverAreaMultiplier(1.f, 1.f)
#endif
    , m_contentsScale(1.f)
    , m_pendingScale(0)
    , m_contentsFrozen(false)
    , m_viewRect(0,0,0,0)
    , m_flickTimerID(0)
    , m_flicking(false)
    , poffsetX(0)
    , poffsetY(0)
    , m_scrollflag(0)
    , m_createflag(0)
#if ENABLE(TVGG_ANIMATED_SCROLL)
    , m_maxsize(IntSize())
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    , m_hscrollbarTextureId(0)
    , m_vscrollbarTextureId(0)
#endif
#endif
{
    ASSERT(!m_size.isEmpty());
#ifdef TILE_DEBUG
    FILE *fp = fopen("/mtd_down/tile.log", "a");
    fprintf(fp, "[TiledBackingStore] [%s %d] Tiled BackingStore created!!!!\n", __func__, __LINE__);
    fclose(fp);
#endif
}

TiledBackingStore::~TiledBackingStore()
{
    delete m_tileCreationTimer;
#ifdef TILE_DEBUG
    FILE *fp = fopen("/mtd_down/tile.log", "a");
    fprintf(fp, "[TiledBackingStore] [%s %d] Tiled BackingStore destroy!!!!\n", __func__, __LINE__);
    fclose(fp);
#endif
}

void TiledBackingStore::incorporateUpdate(const UpdateInfo& updateInfo)
{
    
    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::create(updateInfo.bitmapHandle);
    if (!bitmap)
        return;

    ASSERT(bitmap->size() == updateInfo.updateRectBounds.size());
    
    incorporateUpdate(bitmap.get(), updateInfo);
}

} // namespace WebKit
#endif
