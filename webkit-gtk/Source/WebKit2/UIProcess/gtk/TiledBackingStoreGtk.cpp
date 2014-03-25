#include "config.h"
#include "TiledBackingStore.h"

#include "ShareableBitmap.h"
#include "UpdateInfo.h"
#include "WebPageProxy.h"
#include <WebCore/GraphicsContext.h>
#include <cairo/cairo.h>
#include <gtk/gtk.h>

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "CairoUtilities.h"
#include "GraphicsLayerRenderer.h"
#include "LayerTextureManager.h"
#endif

#if ENABLE(SEF)
#include "MMManager.h"
#endif

#if ENABLE(TVGG_TILED_BACKING_STORE)
#include "WebPage.h"
#include "WebProcess.h"
#include "UIViewWindow.h" 
#include "PageClientImpl.h"

using namespace WebCore;

namespace WebKit {
/*
//scroll frame rate
struct timeval start, end;
double num_frame = 0;
double num_frame_total = 0;
double scroll_time = 0;
double scroll_time_total = 0;
*/
#if ENABLE(TVGG_ANIMATED_SCROLL)
struct ScrollCallbackData {
    TiledBackingStore* backingstore;
    RefPtr<cairo_t> context;
    IntPoint windowPosition;
    IntRect scrollRect;
    bool xscroll;
    bool yscroll;
    int offsetX;
    int offsetY;
    int iterationIndex;
};
#define FLICK_RENDERING_INTERVAL 20 //msec
#endif

#ifdef TILE_DEBUG
FILE *fp = NULL;
#endif

void TiledBackingStore::paint(cairo_t* context, const IntRect& rect)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
    cairo_rectangle(context, rect.x(), rect.y(), rect.width(), rect.height());
    cairo_fill(context);

    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer()) {
        page->getRenderer()->setTiledBackingStoreInfo(tiledBackingStoreInfo());
        page->drawingArea()->setNeedsComposite(true);
    }
    return;
#endif

    IntRect viewRect;
    if(isFlicking())
        viewRect = this->flickRect();
    else
        viewRect = this->viewRect();

    IntRect requestRect = mapFromContents(rect);

#if ENABLE(TVGG_ANIMATED_SCROLL)
    IntRect hscrollDirtyRect = intersection(requestRect, m_hscrollbarRect);
    IntRect vscrollDirtyRect = intersection(requestRect, m_vscrollbarRect);
    if(!hscrollDirtyRect.isEmpty()) 
        m_hscrollbarUpdateflag = 1;
    if(!vscrollDirtyRect.isEmpty()) 
        m_vscrollbarUpdateflag = 1;
#endif

    requestRect.move(viewRect.x(), viewRect.y());

#if ENABLE(SEF)
    if (MMManager::getInstance()->isPipEnabled()) {
        cairo_save(context);
        MMManager::getInstance()->setMaskPIPAreaToCairoContext(context);
    }
#endif
    Tile::Coordinate topLeft = tileCoordinateForPoint(requestRect.location());
    Tile::Coordinate bottomRight = tileCoordinateForPoint(IntPoint(requestRect.maxX(), requestRect.maxY()));

    for (unsigned yCoordinate = topLeft.y(); yCoordinate <= bottomRight.y(); ++yCoordinate) {
        for (unsigned xCoordinate = topLeft.x(); xCoordinate <= bottomRight.x(); ++xCoordinate) {
            Tile::Coordinate currentCoordinate(xCoordinate, yCoordinate);
            RefPtr<Tile> currentTile = tileAt(currentCoordinate);

            if (!currentTile)
                continue; 

            IntRect dirtyRect = intersection(currentTile->rect(), requestRect);
            if (currentTile && currentTile->isReadyToPaint()){
                currentTile->paint(context, dirtyRect);
            }		
        }
    }

#if ENABLE(TVGG_ANIMATED_SCROLL) && !ENABLE(TVGG_ACCELERATED_COMPOSITING)
    paintScrollbars(context);
#endif	

#if ENABLE(SEF)
    if (MMManager::getInstance()->isPipEnabled()) {
        cairo_restore(context);
        IntRect pipRect = MMManager::getInstance()->getPipRect();
        pipRect.move(-MMManager::getInstance()->pipLocationOffset().x(), -MMManager::getInstance()->pipLocationOffset().y());
        cairo_set_operator(context, CAIRO_OPERATOR_CLEAR);
        cairo_rectangle(context, pipRect.x(), pipRect.y(), pipRect.width(), pipRect.height());
        cairo_fill(context);
    }
#endif

}

void TiledBackingStore::incorporateUpdate(ShareableBitmap* bitmap, const UpdateInfo& updateInfo)
{
    scroll(updateInfo);

#if ENABLE(TVGG_ANIMATED_SCROLL)
    updateScrollbars(updateInfo);
#else
    m_hscrollbarRect = updateInfo.hscrollbarRect;
    m_vscrollbarRect = updateInfo.vscrollbarRect;
#endif

    createTiles();

#if ENABLE(TVGG_EXTENDED_BACKING_STORE) //for scroll with no updating
    if(m_scrollflag != 1 || !isFlicking())
#endif
    {
        updateTileBuffers(bitmap, updateInfo);
    }

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer())
        page->getRenderer()->setTiledBackingStoreInfo(tiledBackingStoreInfo());
#endif

#ifdef MEM_DEBUG
if(m_scrollflag == 3){
    int Free, Cached, Active, Inactive;
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

#ifdef TILE_DEBUG
if(m_createflag == 1 || m_scrollflag != 0){
    fp = fopen("/mtd_down/tile.log", "a");
    
    TileMap::iterator end = m_tiles.end();
    for (TileMap::iterator it = m_tiles.begin(); it != end; ++it) {
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] m_tiles x = %d, y=%d\n", __func__, __LINE__, it->second->coordinate().x(), it->second->coordinate().y());
   }

    fclose(fp);
}
#endif

}

void TiledBackingStore::scroll(const UpdateInfo& updateInfo)
{
    m_scrollflag = 0;
    IntRect visibleRect = updateInfo.visibleRect;
    IntSize scrollOffset = updateInfo.scrollOffset;

#if ENABLE(TVGG_EXTENDED_BACKING_STORE) 
    IntRect contentRect = contentsRect();

    if(m_contentRect != contentRect){
        m_contentRect = contentRect;
        m_createflag = 1;
    }
#endif

    if (scrollOffset.isZero()){
        //No scroll 
        if(m_viewRect == visibleRect) {
            m_scrollflag = 0; 
            return;
        }
        
        //Refresh
        else { 
            m_viewRect = visibleRect;
            m_scrollflag = 3;
#if ENABLE(TVGG_ANIMATED_SCROLL)
            m_hscrollbar_backingStore.clear();
            m_vscrollbar_backingStore.clear();
#endif
            return;
        }
    }

    else{
        //scroll, but visibleRect has no change. error!! 
        if(m_viewRect == visibleRect) 
            printf("[TiledBackingStoreGtk] [%s %d] visibleRect sync Error !!!!\n", __func__, __LINE__);

        //Scroll
        else{ 
            moveViewRect(-scrollOffset.width(), -scrollOffset.height());

#if ENABLE(TVGG_ANIMATED_SCROLL)
            if (updateInfo.disableAnimation){
                m_scrollflag = 2;
                return; //Do not animate.
            }
#endif
            //This case is for hiding toolbar when view position is bottom end.
            if (m_viewRect != visibleRect){
                m_viewRect = visibleRect;
#if ENABLE(TVGG_ANIMATED_SCROLL)
                m_hscrollbar_backingStore.clear();
                m_vscrollbar_backingStore.clear();
#endif
                m_scrollflag = 2;
                return; //Do not animate.
            }

            m_scrollflag = 1; 
        }
    }
           
#if ENABLE(TVGG_ANIMATED_SCROLL)
    if(isFlicking()) {
        //m_conscrollflag = true;
        setconscrolloffset(scrollOffset);

        m_flickRect = this->viewRect();
        m_flickRect.move(scrollOffset.width(), scrollOffset.height()); //revert to previous viewrect
        //g_source_remove(m_flickTimerID);
        return;
    }
    else {
        m_flickRect = this->viewRect();
        m_flickRect.move(scrollOffset.width(), scrollOffset.height()); //revert to previous viewrect
    }

    static ScrollCallbackData data = {NULL, NULL, IntPoint(), IntRect(), 0, 0, 0, 0, 0};

    IntPoint contentWindow = this->getWindowPosition();
    RefPtr<cairo_surface_t> surface = this->getWindowSurface();
    RefPtr<cairo_t> context(adoptRef(cairo_create(surface.get())));
    cairo_translate(context.get(), contentWindow.x(), contentWindow.y());

    // set data for flicking
    data.context = context;
    data.backingstore = this;
    data.windowPosition = contentWindow;
    data.scrollRect = updateInfo.scrollRect;
    //data.xscroll = scrollOffset.width() ? true : false;
    //data.yscroll = scrollOffset.height() ? true : false;
    //data.offsetX = scrollOffset.width()+poffsetX;
    //data.offsetY = scrollOffset.height()+poffsetY;
    data.offsetX = scrollOffset.width();
    data.offsetY = scrollOffset.height();
    data.iterationIndex = 1;

    m_flicking = true;
    m_flickTimerID = g_timeout_add_full(G_PRIORITY_HIGH, FLICK_RENDERING_INTERVAL,
                                            scrollCallback, &data, scrollClear);
/* //scroll frame rate
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage *page = WebProcess::shared().mainUIWebPage();
    DrawingArea *drawingarea = page->drawingArea();
    drawingarea->scroll_flag = true;
    gettimeofday(&start, NULL);
#else
    gettimeofday(&start, NULL);
#endif
*/
#endif	
}

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
WebCore::TiledBackingStoreInfo TiledBackingStore::tiledBackingStoreInfo()
{
    WebCore::TiledBackingStoreInfo info;
    Vector<RefPtr<Tile> > visibleTiles;
    visibleTiles = getVisibleTiles();

    unsigned size = visibleTiles.size();
    IntRect viewRect;

    if (isFlicking())
        viewRect = this->flickRect();
    else
        viewRect = this->viewRect();

    for (unsigned n = 0; n < size; ++n) {
        IntRect m_rect = visibleTiles[n]->rect();
        m_rect.move(-viewRect.x(), -viewRect.y());
        unsigned textureId = visibleTiles[n]->getTextureID();
        info.addTile(textureId, m_rect);
    }

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(TVGG_ANIMATED_SCROLL)
    IntRect hscrollbarMemRect = m_hscrollbarRect;
    hscrollbarMemRect.setWidth(this->viewRect().width());
    info.setHScrollbarInfo(hscrollbarMemRect, m_hscrollbarTextureId);
    info.setVScrollbarInfo(m_vscrollbarRect, m_vscrollbarTextureId);
#endif

    return info;
}
#endif

#if ENABLE(TVGG_ANIMATED_SCROLL) 
gboolean TiledBackingStore::scrollCallback(gpointer callbackdata)
{
    ScrollCallbackData* data = (ScrollCallbackData*) callbackdata;
    TiledBackingStore* backingstore = data->backingstore;

    IntSize newscrolloffset = backingstore->conscrolloffset();
    if(!newscrolloffset.isZero()){
        data->offsetX = newscrolloffset.width();
        data->offsetY = newscrolloffset.height();
        //data->offsetX += newscrolloffset.width();
        //data->offsetY += newscrolloffset.height();
        backingstore->setconscrolloffset(IntSize());
    }

    // calculate dx, dy
    int dy, dx = 0;
    dx = data->offsetX * 0.3;
    dy = data->offsetY * 0.3;

    if(dx < -40) dx = -40 + dx / 4;
    else if(dx > 40) dx = 40 + dx / 4;
    else if(dx < -10) dx = -10 + dx / 2;
    else if(dx > 10) dx = 10 + dx / 2;
    else if(dx > -1 && data->offsetX < 0) dx = -1;
    else if(dx < 1 && data->offsetX > 0) dx = 1;

    if(dy < -40) dy = -40 + dy / 4;
    else if(dy > 40) dy = 40 + dy / 4;
    else if(dy < -10) dy = -10 + dy / 2;
    else if(dy > 10) dy = 10 + dy / 2;
    else if(dy > -1 && data->offsetY < 0) dy = -1;
    else if(dy < 1 && data->offsetY > 0) dy = 1;

    // if dx and dy are zero, stop flicking
    if (abs(dy) == 0 && abs(dx) == 0) {
        g_source_remove(backingstore->m_flickTimerID);

/*  //scroll frame rate
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        WebPage *page = WebProcess::shared().mainUIWebPage();
        DrawingArea *drawingarea = page->drawingArea();

        gettimeofday(&end, NULL);
        drawingarea->scroll_flag = false;
        scroll_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
        printf("----------------------------------fps = %f \n", drawingarea->num_frame/scroll_time);
        num_frame_total += drawingarea->num_frame;
        scroll_time_total += scroll_time;
        printf("----------------------------average fps = %f \n", num_frame_total/scroll_time_total);
        drawingarea->num_frame = 0;
#else
        gettimeofday(&end, NULL);
        scroll_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;
        printf("----------------------------------fps = %f \n", num_frame/scroll_time);
        num_frame_total += num_frame;
        scroll_time_total += scroll_time;
        printf("----------------------------average fps = %f \n", num_frame_total/scroll_time_total);
        num_frame = 0;
#endif
*/
        return false;
    }

    // do moving page
    data->offsetX -= dx;
    data->offsetY -= dy;
    //backingstore->savePreviousOffset(data->offsetX, data->offsetY);
    backingstore->moveFlickRect(-dx, -dy);

    if (data->scrollRect.height() != backingstore->scrollRect().height())
        data->scrollRect.setHeight(backingstore->scrollRect().height());
    
    IntRect requestRect = data->scrollRect;

    WebPage* page = WebProcess::shared().mainUIWebPage();
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (page && page->getRenderer()) {
        page->getRenderer()->setTiledBackingStoreInfo(backingstore->tiledBackingStoreInfo());
        page->drawingArea()->setNeedsComposite(true);
    }
#else
    IntPoint contentWindow = backingstore->getWindowPosition();
    if(data->windowPosition.y() != contentWindow.y()) {
        RefPtr<cairo_surface_t> surface = backingstore->getWindowSurface();
        RefPtr<cairo_t> context(adoptRef(cairo_create(surface.get())));
        cairo_translate(context.get(), contentWindow.x(), contentWindow.y());
        if(contentWindow.y() + requestRect.height() > backingstore->maxSize().height()){
            if(backingstore->ishorizontalScrollbarExist())
                requestRect.setHeight(backingstore->maxSize().height() - contentWindow.y() - backingstore->horizontalScrollbarRect().height());
            else
                requestRect.setHeight(backingstore->maxSize().height() - contentWindow.y());
        }
        backingstore->paint(context.get(), requestRect);
        data->windowPosition.setY(contentWindow.y());
        data->context = context;
    }
    else {
        if(data->windowPosition.y() + requestRect.height() > backingstore->maxSize().height()) {
            if(backingstore->ishorizontalScrollbarExist())
                requestRect.setHeight(backingstore->maxSize().height() - contentWindow.y() - backingstore->horizontalScrollbarRect().height());
            else
                requestRect.setHeight(backingstore->maxSize().height() - data->windowPosition.y());
        }
        backingstore->paint(data->context.get(), requestRect);
        //scroll frame rate
        //num_frame++;
    }
#endif
    UIViewWindow* window = page->getUIViewWindow();
    window->sync();
	
    // if iterationIndex is 20, stop flicking
    if (data->iterationIndex == 50) {
        g_source_remove(backingstore->m_flickTimerID);
        return false;
    }

    // increase iteration index
    data->iterationIndex++;
    return true;

}

void TiledBackingStore::scrollClear(gpointer callbackdata)
{
    ScrollCallbackData* data = (ScrollCallbackData*) callbackdata;
    TiledBackingStore* backingstore = data->backingstore;
/*
    if(backingstore->conscrollFlag() == true){
        backingstore->setconscrollFlag(false);
        //backingstore->m_flicking = false;
        return;
    }
*/   
    backingstore->savePreviousOffset(0, 0);

#if ENABLE(TVGG_EXTENDED_BACKING_STORE) 
    //Fill new coverRect and update visibleRect after scroll animation
    Vector<IntRect> needUpdateRects = backingstore->needUpdateRects();
    needUpdateRects.append(data->scrollRect);

    for (size_t i = 0; i < needUpdateRects.size(); ++i) {
        backingstore->setNeedsDisplay(needUpdateRects[i]);
    }

    backingstore->setUpdateWaitingflag(true);

#else
    //Update visibleRect after scroll animation
    IntRect requestRect = backingstore->viewRect();
    requestRect.setLocation(IntPoint());

    backingstore->m_webPageProxy->setViewNeedsDisplay(requestRect);
#endif

    FloatSize keepAreaMultiplier = backingstore->keepAreaMultiplier();

    //drop tiles after scroll animation
    if(keepAreaMultiplier.width() == 1.f && keepAreaMultiplier.height() == 1.f){
        if(data->offsetX == 0 && data->offsetY == 0){
            IntRect viewRect = backingstore->viewRect(); 
            backingstore->dropTilesOutsideRect(viewRect);
        }
    }

    backingstore->m_flicking = false;

}

RefPtr<cairo_surface_t> TiledBackingStore::getWindowSurface()
{
    WebPage *page = WebProcess::shared().mainUIWebPage();
    UIViewWindow* window = page->getUIViewWindow();
    RefPtr<cairo_surface_t> surface = window->getWindowSurface();
    return surface;
}

IntPoint TiledBackingStore::getWindowPosition() const
{
    PageClient* pageclient = m_webPageProxy->getPageClient();
    IntPoint contentWindow = static_cast<PageClientImpl*>(pageclient)->getLocation();
    return contentWindow;
}
#endif

void TiledBackingStore::createTiles()
{
    if (m_contentsFrozen){
        printf("[TiledBackingStoreGtk] [%s %d] contents Frozen!!\n", __func__, __LINE__);
        return;
    }

    IntRect contentRect = this->contentsRect();

    IntRect visibleRect;
    if(m_viewRect.isEmpty()){
        m_viewRect = this->visibleRect();
        visibleRect = m_viewRect;
    }
    else{
        if(m_scrollflag == 0 && m_createflag == 0){
            return;
        }
        else {
            visibleRect = m_viewRect;
        }
    }
	
    if (visibleRect.isEmpty()){
        printf("[TiledBackingStoreGtk] [%s %d] visibleRect is empty!!\n", __func__, __LINE__);
        return;
    }

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    m_needUpdateRects.clear();
#endif

#ifdef TILE_DEBUG
    fp = fopen("/mtd_down/tile.log", "a");
    IntPoint startpoint;
    IntPoint endpoint;

    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] visibleRect x = %d, y=%d, w = %d, h = %d\n", __func__, __LINE__, visibleRect.x(), visibleRect.y(), visibleRect.width(), visibleRect.height());
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] contentRect x = %d, y=%d, w = %d, h = %d\n", __func__, __LINE__, contentRect.x(), contentRect.y(), contentRect.width(), contentRect.height());
    startpoint = tileCoordinateForPoint(contentRect.location());
    endpoint = tileCoordinateForPoint(contentRect.maxXMaxYCorner());
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] start x = %d, y=%d\n", __func__, __LINE__, startpoint.x(), startpoint.y());
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] end x = %d, y=%d\n", __func__, __LINE__, endpoint.x(), endpoint.y());
#endif

    // Remove tiles that extend outside the current contents rect.
    dropOverhangingTiles();

#if ENABLE(TVGG_ANIMATED_SCROLL)
    if(m_keepAreaMultiplier.width() == 1.f && m_keepAreaMultiplier.height() == 1.f){
	//In this case, drop tiles after animation.
    }
    else{
        IntRect keepRect = visibleRect;
        // Inflates to both sides, so divide inflate delta by 2
        keepRect.inflateX(visibleRect.width() * (m_keepAreaMultiplier.width() - 1.f) / 2);
        keepRect.inflateY(visibleRect.height() * (m_keepAreaMultiplier.height() - 1.f) / 2);
        keepRect.intersect(contentsRect());

#ifdef TILE_DEBUG
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] keepRect x = %d, y=%d, w = %d, h = %d\n", __func__, __LINE__, keepRect.x(), keepRect.y(), keepRect.width(), keepRect.height());
        startpoint = tileCoordinateForPoint(keepRect.location());
        endpoint = tileCoordinateForPoint(keepRect.maxXMaxYCorner());
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] start x = %d, y=%d\n", __func__, __LINE__, startpoint.x(), startpoint.y());
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] end x = %d, y=%d\n", __func__, __LINE__, endpoint.x(), endpoint.y());
#endif

        if(isFlicking()){
            if(keepRect.contains(flickRect()))
                dropTilesOutsideRect(keepRect);
        }
        else
            dropTilesOutsideRect(keepRect);
            
    }
#else
    IntRect keepRect = visibleRect;
    // Inflates to both sides, so divide inflate delta by 2
    keepRect.inflateX(visibleRect.width() * (m_keepAreaMultiplier.width() - 1.f) / 2);
    keepRect.inflateY(visibleRect.height() * (m_keepAreaMultiplier.height() - 1.f) / 2);
    keepRect.intersect(contentsRect());
 
    dropTilesOutsideRect(keepRect);
#endif

    IntRect coverRect = visibleRect;
    // Inflates to both sides, so divide inflate delta by 2
    coverRect.inflateX(visibleRect.width() * (m_coverAreaMultiplier.width() - 1.f) / 2);
    coverRect.inflateY(visibleRect.height() * (m_coverAreaMultiplier.height() - 1.f) / 2);
    coverRect.intersect(contentsRect());
    m_coverRect = coverRect;

#ifdef TILE_DEBUG
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] m_height = %f, dy = %d\n", __func__, __LINE__, m_coverAreaMultiplier.height(), visibleRect.height() * (m_coverAreaMultiplier.height() - 1.f) / 2);
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] coverRect x = %d, y=%d, w = %d, h = %d\n", __func__, __LINE__, coverRect.x(), coverRect.y(), coverRect.width(), coverRect.height());
    startpoint = tileCoordinateForPoint(coverRect.location());
    endpoint = tileCoordinateForPoint(coverRect.maxXMaxYCorner());
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] start x = %d, y=%d\n", __func__, __LINE__, startpoint.x(), startpoint.y());
    fprintf(fp, "[TiledBackingStoreGtk] [%s %d] end x = %d, y=%d\n", __func__, __LINE__, endpoint.x(), endpoint.y());
#endif

    // Search for the tile position closest to the viewport center that does not yet contain a tile. 
    // Which position is considered the closest depends on the tileDistance function.
    double shortestDistance = std::numeric_limits<double>::infinity();
    Vector<Tile::Coordinate> tilesToCreate;
    unsigned requiredTileCount = 0;
    Tile::Coordinate topLeft = tileCoordinateForPoint(coverRect.location());
    Tile::Coordinate bottomRight = tileCoordinateForPoint(IntPoint(coverRect.maxX(), coverRect.maxY()));
    for (unsigned yCoordinate = topLeft.y(); yCoordinate <= bottomRight.y(); ++yCoordinate) {
        for (unsigned xCoordinate = topLeft.x(); xCoordinate <= bottomRight.x(); ++xCoordinate) {
            Tile::Coordinate currentCoordinate(xCoordinate, yCoordinate);
            // If coordinate of tile is exactly same with coverRect edge, there's no tile.
            if(tileRectForCoordinate(currentCoordinate).isEmpty())
                continue;

            if (tileAt(currentCoordinate)){
                continue;
            }
            ++requiredTileCount;
#if !ENABLE(TVGG_EXTENDED_BACKING_STORE)
            // Distance is 0 for all currently visible tiles.
            double distance = tileDistance(visibleRect, currentCoordinate);
            if (distance > shortestDistance)
                continue;
            if (distance < shortestDistance) {
                tilesToCreate.clear();
                shortestDistance = distance;
            }
#endif
            tilesToCreate.append(currentCoordinate);
        }
    }
    
    // Now construct the tile(s)
    unsigned tilesToCreateCount = tilesToCreate.size();
    for (unsigned n = 0; n < tilesToCreateCount; ++n) {
        Tile::Coordinate coordinate = tilesToCreate[n];
        setTile(coordinate, Tile::create(this, coordinate));
#ifdef TILE_DEBUG
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] creat tile x = %d, y=%d\n", __func__, __LINE__, coordinate.x(), coordinate.y());
#endif
#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
        RefPtr<Tile> currentTile = tileAt(coordinate);
        IntRect tileRect = currentTile->rect();
        tileRect.move(-viewRect().x(), -viewRect().y());
        m_needUpdateRects.append(tileRect);
#endif
    }
    requiredTileCount -= tilesToCreateCount;
	
    // Keep creating tiles until the whole coverRect is covered.
    if (requiredTileCount){
        m_createflag = 1;
        m_tileCreationTimer->startOneShot(m_tileCreationDelay);
    }
    else
        m_createflag = 0;

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    // Updating reservation for newly created tiles.
    if(m_scrollflag != 1){
        for (size_t i = 0; i < m_needUpdateRects.size(); ++i) {
            setNeedsDisplay(m_needUpdateRects[i]);
        }
    }
#endif

#ifdef TILE_DEBUG
    fclose(fp);
#endif    
}

void TiledBackingStore::updateTileBuffers(ShareableBitmap* bitmap, const UpdateInfo& updateInfo)
{
	
    if (m_contentsFrozen)
        return;

    IntRect viewRect = this->viewRect();

    IntRect updateRectBounds = mapFromContents(updateInfo.updateRectBounds);
    updateRectBounds.move(viewRect.x(), viewRect.y());
    IntPoint updateRectLocation = updateRectBounds.location();

    TileMap::iterator end = m_tiles.end();
    for (TileMap::iterator it = m_tiles.begin(); it != end; ++it) {
        IntRect interRect = intersection(it->second->rect(), updateRectBounds);

        if(!interRect.isEmpty()) {
            for(size_t i =0; i < updateInfo.updateRects.size(); ++i) {
                IntRect updateRects = mapFromContents(updateInfo.updateRects[i]);
                updateRects.move(viewRect.x(), viewRect.y());
                IntRect dirtyRect = intersection(it->second->rect(), updateRects);

                if(!dirtyRect.isEmpty()) {
                    it->second->updateBackBuffer(bitmap, updateRectBounds, dirtyRect);
                }
            }
        }
        else
            continue;
    }

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
    if (m_updateWaitingflag == true) {
        IntRect requestRect = this->viewRect();
        requestRect.setLocation(IntPoint());
        m_webPageProxy->setViewNeedsDisplay(requestRect);
        m_updateWaitingflag = false;
    }
#endif

    return;

}

#if ENABLE(TVGG_ANIMATED_SCROLL)
void TiledBackingStore::updateScrollbars(const UpdateInfo& updateInfo)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (!page || !page->getRenderer() || !page->getRenderer()->textureManager())
        return;

    LayerTextureManager* textureManager = page->getRenderer()->textureManager();
#endif

    //update horizontal scrollbar
    if(!updateInfo.hscroll_bitmapHandle.isNull()) {

        if (m_hscrollbarRect != updateInfo.hscrollbarRect){
            m_hscrollbarRect = updateInfo.hscrollbarRect;
            m_hscrollbar_backingStore.clear();
        }

        IntSize hscrollbarSize = updateInfo.hscrollbarRect.size();

        //for corner region between two scrollbar.
        IntSize hcrollbarMemSize = hscrollbarSize;
        hcrollbarMemSize.setWidth(viewRect().width()); 

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        void* imageBufferPtr = 0;
        if (m_hscrollbarTextureId)
            textureManager->deleteTexture(m_hscrollbarTextureId);

        m_hscrollbar_backingStore.clear();

        m_hscrollbarTextureId = textureManager->createTexture();
        textureManager->updateTexture(m_hscrollbarTextureId, hcrollbarMemSize, 0);
        textureManager->getBuffer(m_hscrollbarTextureId, &imageBufferPtr, 0, 0);

        ASSERT(ptr);

        m_hscrollbar_backingStore = ImageBackingStore::create(imageBufferPtr, hcrollbarMemSize);
        m_hscrollbarCreateflag = 1;
#else
        if (!m_hscrollbar_backingStore){
            m_hscrollbar_backingStore = WidgetBackingStore::create(m_webPageProxy->viewWidget(), hcrollbarMemSize);
            m_hscrollbarCreateflag = 1;
        }
#endif

        RefPtr<cairo_t> hcontext(adoptRef(cairo_create(m_hscrollbar_backingStore->cairoSurface())));

        if (m_hscrollbarCreateflag == 1) {
            cairo_rectangle(hcontext.get(), 0, 0, hcrollbarMemSize.width(), hcrollbarMemSize.height());
            cairo_set_source_rgb(hcontext.get(), 1, 1 ,1);
            cairo_fill(hcontext.get());
        }

        RefPtr<ShareableBitmap> hscroll_bitmap = ShareableBitmap::create(updateInfo.hscroll_bitmapHandle);
        RefPtr<cairo_surface_t> hscrollSurface = hscroll_bitmap.get()->createCairoSurface();

        cairo_set_operator(hcontext.get(), CAIRO_OPERATOR_OVER);
        //cairo_set_source_rgba(hcontext.get(), 1.f, 0, 0, 1.f);
        cairo_set_source_surface(hcontext.get(), hscrollSurface.get(), 0, 0);
        cairo_rectangle(hcontext.get(), 0, 0, hscrollbarSize.width(), hscrollbarSize.height());
        cairo_fill(hcontext.get());

        m_hscrollbarUpdateflag = 1;
    }
    else {
        if(updateInfo.hscrollbarRect.isEmpty()) {
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
            if (m_hscrollbarTextureId) {
                textureManager->deleteTexture(m_hscrollbarTextureId);
                m_hscrollbarTextureId = 0;
            }
            m_hscrollbarRect = updateInfo.hscrollbarRect;
#endif
            m_hscrollbar_backingStore.clear();
        }
    }

    
    //update vertical scrollbar
    if(!updateInfo.vscroll_bitmapHandle.isNull()) {

        if (m_vscrollbarRect != updateInfo.vscrollbarRect){
            m_vscrollbarRect = updateInfo.vscrollbarRect;
            m_vscrollbar_backingStore.clear();
        }

        IntSize vscrollbarSize = updateInfo.vscrollbarRect.size();

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        void* imageBufferPtr = 0;
        if (m_vscrollbarTextureId)
            textureManager->deleteTexture(m_vscrollbarTextureId);

        m_vscrollbar_backingStore.clear();

        m_vscrollbarTextureId = textureManager->createTexture();
        textureManager->updateTexture(m_vscrollbarTextureId, vscrollbarSize, 0);
        textureManager->getBuffer(m_vscrollbarTextureId, &imageBufferPtr, 0, 0);

        ASSERT(ptr);

        m_vscrollbar_backingStore = ImageBackingStore::create(imageBufferPtr, vscrollbarSize);
        m_vscrollbarCreateflag = 1;
#else
        if (!m_vscrollbar_backingStore){
            m_vscrollbar_backingStore = WidgetBackingStore::create(m_webPageProxy->viewWidget(), vscrollbarSize);
            m_vscrollbarCreateflag = 1;
        }
#endif

        RefPtr<cairo_t> vcontext(adoptRef(cairo_create(m_vscrollbar_backingStore->cairoSurface())));

        if (m_vscrollbarCreateflag == 1) {
            cairo_rectangle(vcontext.get(), 0, 0, vscrollbarSize.width(), vscrollbarSize.height());
            cairo_set_source_rgb(vcontext.get(), 1, 1 ,1);
            cairo_fill(vcontext.get());
        }

        RefPtr<ShareableBitmap> vscroll_bitmap = ShareableBitmap::create(updateInfo.vscroll_bitmapHandle);
        RefPtr<cairo_surface_t> vscrollSurface = vscroll_bitmap.get()->createCairoSurface();

        cairo_set_operator(vcontext.get(), CAIRO_OPERATOR_OVER);
        //cairo_set_source_rgba(vcontext.get(), 1.f, 0, 0, 1.f);
        cairo_set_source_surface(vcontext.get(), vscrollSurface.get(), 0, 0);
        cairo_rectangle(vcontext.get(), 0, 0, vscrollbarSize.width(), vscrollbarSize.height());
        cairo_fill(vcontext.get());

        m_vscrollbarUpdateflag = 1;
    }
    else {
        if(updateInfo.vscrollbarRect.isEmpty()) {
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
            if (m_vscrollbarTextureId) {
                textureManager->deleteTexture(m_vscrollbarTextureId);
                m_vscrollbarTextureId = 0;
            }
            m_vscrollbarRect = updateInfo.vscrollbarRect;
#endif
            m_vscrollbar_backingStore.clear();
        }
    }

}

void TiledBackingStore::paintScrollbars(cairo_t* cairocontext)
{
    cairo_t* context;

    PageClient* pageclient = m_webPageProxy->getPageClient();
    IntPoint contentWindow = static_cast<PageClientImpl*>(pageclient)->getLocation();

    if(cairocontext == NULL){
        RefPtr<cairo_surface_t> surface = this->getWindowSurface();
        context = cairo_create(surface.get());
        cairo_translate(context, contentWindow.x(), contentWindow.y());
    }
    else{
        context = cairocontext;
    }

    if(m_hscrollbar_backingStore){
        if( m_hscrollbarCreateflag == 1 || m_hscrollbarUpdateflag == 1) {
            if(m_hscrollbarCreateflag == 1) m_hscrollbarCreateflag = 0;
            if(m_hscrollbarUpdateflag == 1) m_hscrollbarUpdateflag = 0;

            //for corner region between two scrollbar.
            IntSize hscrollbarSize = m_hscrollbarRect.size();
            hscrollbarSize.setWidth(viewRect().width());

            //cairo_set_operator(context, CAIRO_OPERATOR_OVER);
            cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
            //cairo_set_source_rgba(context, 1.f, 0, 0, 1.f);
            cairo_set_source_surface(context, m_hscrollbar_backingStore->cairoSurface(), m_hscrollbarRect.x(), m_hscrollbarRect.y());
            cairo_rectangle(context, m_hscrollbarRect.x(), m_hscrollbarRect.y(), hscrollbarSize.width(), hscrollbarSize.height());
            cairo_fill(context);
        }
    }
	
    if(m_vscrollbar_backingStore){
        if( m_vscrollbarCreateflag == 1 || m_vscrollbarUpdateflag == 1) {
            if(m_vscrollbarCreateflag == 1) m_vscrollbarCreateflag = 0;
            if(m_vscrollbarUpdateflag == 1) m_vscrollbarUpdateflag = 0;

            IntRect vscrollbarRect = m_vscrollbarRect;
            
            //To restrict rendering size of scrollbar.
            if(contentWindow.y() + vscrollbarRect.height() > m_maxsize.height())
                vscrollbarRect.setHeight(m_maxsize.height() - contentWindow.y());

            //cairo_set_operator(context, CAIRO_OPERATOR_OVER);
            cairo_set_operator(context, CAIRO_OPERATOR_SOURCE);
            //cairo_set_source_rgba(context, 1.f, 0, 0, 1.f);
            cairo_set_source_surface(context, m_vscrollbar_backingStore->cairoSurface(), vscrollbarRect.x(), vscrollbarRect.y());
            cairo_rectangle(context, vscrollbarRect.x(), vscrollbarRect.y(), vscrollbarRect.width(), vscrollbarRect.height());
            cairo_fill(context);
        }
    }

    if(cairocontext == NULL)
        cairo_destroy(context);
	
    return;

}

IntRect TiledBackingStore::scrollRect()
{
    IntRect scrollRect = viewRect();
    scrollRect.setLocation(IntPoint());

    if(m_hscrollbar_backingStore)	
        scrollRect.setHeight(scrollRect.height()-m_hscrollbarRect.height());

    if(m_vscrollbar_backingStore)	
        scrollRect.setWidth(scrollRect.width()-m_vscrollbarRect.width());

    return scrollRect;
}
#endif

#if ENABLE(TVGG_EXTENDED_BACKING_STORE)
void TiledBackingStore::setNeedsDisplay(const IntRect rect)
{
    m_webPageProxy->process()->send(Messages::DrawingArea::SetNeedsDisplay(rect), m_webPageProxy->pageID());
}
#endif

void TiledBackingStore::savePreviousOffset(int offsetX, int offsetY)
{
    poffsetX = offsetX;
    poffsetY = offsetY;
}

void TiledBackingStore::moveViewRect(int dx, int dy)
{
    m_viewRect.move(dx, dy);
}

void TiledBackingStore::moveFlickRect(int dx, int dy)
{
    m_flickRect.move(dx, dy);
}

IntRect TiledBackingStore::viewRect() const
{
    return m_viewRect;
}

IntRect TiledBackingStore::coverRect() const
{
    return m_coverRect;
}

IntRect TiledBackingStore::flickRect() const
{
    return m_flickRect;
}

void TiledBackingStore::setSize(const IntSize& size) 
{ 
    m_size = size; 
#if ENABLE(TVGG_ANIMATED_SCROLL)
    if(size.height() == 1080 || size.height() == 720)
        return;

    if (m_maxsize.width() < size.width() || m_maxsize.height() < size.height())
        m_maxsize = size;
#endif
}

IntRect TiledBackingStore::visibleRect() const
{
    return mapFromContents(m_webPageProxy->getVisibleRect(true));
}

IntRect TiledBackingStore::contentsRect() const
{
    return mapFromContents(IntRect(IntPoint(), m_webPageProxy->getContentsSize(true)));
}

FloatSize TiledBackingStore::keepAreaMultiplier() const
{
    return m_keepAreaMultiplier;
}

double TiledBackingStore::tileDistance(const IntRect& viewport, const Tile::Coordinate& tileCoordinate)
{
    if (viewport.intersects(tileRectForCoordinate(tileCoordinate)))
        return 0;
    
    IntPoint viewCenter = viewport.location() + IntSize(viewport.width() / 2, viewport.height() / 2);
    Tile::Coordinate centerCoordinate = tileCoordinateForPoint(viewCenter);
    
    // Manhattan distance, biased so that vertical distances are shorter.
    const double horizontalBias = 1.3;
    return abs(centerCoordinate.y() - tileCoordinate.y()) + horizontalBias * abs(centerCoordinate.x() - tileCoordinate.x());
}

Vector<RefPtr<Tile> > TiledBackingStore::getVisibleTiles()
{
    IntRect viewRect;
    if (isFlicking())
        viewRect = this->flickRect();
    else
        viewRect = this->viewRect();

    Vector<RefPtr<Tile> > visibleTiles;
	
    TileMap::iterator end = m_tiles.end();
    for (TileMap::iterator it = m_tiles.begin(); it != end; ++it) {
        IntRect tileRect = it->second->rect();
        if (tileRect.intersects(viewRect))
            visibleTiles.append(it->second);
    }

     return visibleTiles;
}

void TiledBackingStore::dropOverhangingTiles()
{    
    IntRect contentsRect = this->contentsRect();

    Vector<Tile::Coordinate> tilesToRemove;
    TileMap::iterator end = m_tiles.end();
    for (TileMap::iterator it = m_tiles.begin(); it != end; ++it) {
        Tile::Coordinate tileCoordinate = it->second->coordinate();
        IntRect tileRect = it->second->rect();
        IntRect expectedTileRect = tileRectForCoordinate(tileCoordinate);
        if (expectedTileRect != tileRect || !contentsRect.contains(tileRect))
            tilesToRemove.append(tileCoordinate);
    }
    unsigned removeCount = tilesToRemove.size();
    for (unsigned n = 0; n < removeCount; ++n){
        removeTile(tilesToRemove[n]);
#ifdef TILE_DEBUG
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] remove tile x = %d, y=%d\n", __func__, __LINE__, tilesToRemove[n].x(), tilesToRemove[n].y());
#endif
    }
}

void TiledBackingStore::dropTilesOutsideRect(const IntRect& keepRect)
{
    FloatRect keepRectF = keepRect;

    Vector<Tile::Coordinate> toRemove;
    TileMap::iterator end = m_tiles.end();
    for (TileMap::iterator it = m_tiles.begin(); it != end; ++it) {
        Tile::Coordinate coordinate = it->second->coordinate();
        FloatRect tileRect = it->second->rect();
        if (!tileRect.intersects(keepRectF))
            toRemove.append(coordinate);
    }
    unsigned removeCount = toRemove.size();
    for (unsigned n = 0; n < removeCount; ++n){
        removeTile(toRemove[n]);
#ifdef TILE_DEBUG
        fprintf(fp, "[TiledBackingStoreGtk] [%s %d] remove tile x = %d, y=%d\n", __func__, __LINE__, toRemove[n].x(), toRemove[n].y());
#endif
    }
}

PassRefPtr<Tile> TiledBackingStore::tileAt(const Tile::Coordinate& coordinate) const
{
    return m_tiles.get(coordinate);
}

void TiledBackingStore::setTile(const Tile::Coordinate& coordinate, PassRefPtr<Tile> tile)
{
    m_tiles.set(coordinate, tile);
}

void TiledBackingStore::removeTile(const Tile::Coordinate& coordinate)
{
    m_tiles.remove(coordinate);
}

IntRect TiledBackingStore::mapFromContents(const IntRect& rect) const
{
    return enclosingIntRect(FloatRect(rect.x() * m_contentsScale,
        rect.y() * m_contentsScale,
        rect.width() * m_contentsScale,
        rect.height() * m_contentsScale));
}

IntRect TiledBackingStore::tileRectForCoordinate(const Tile::Coordinate& coordinate) const
{
    IntRect rect(coordinate.x() * m_tileSize.width(),
        coordinate.y() * m_tileSize.height(),
        m_tileSize.width(),
        m_tileSize.height());

    rect.intersect(contentsRect());
    return rect;
}

Tile::Coordinate TiledBackingStore::tileCoordinateForPoint(const IntPoint& point) const
{
    int x = point.x() / m_tileSize.width();
    int y = point.y() / m_tileSize.height();
    return Tile::Coordinate(std::max(x, 0), std::max(y, 0));
}

void TiledBackingStore::startTileCreationTimer()
{
    if (m_tileCreationTimer->isActive() || m_contentsFrozen)
        return;
    m_tileCreationTimer->startOneShot(0);
}

void TiledBackingStore::tileCreationTimerFired(TileTimer*)
{
    createTiles();
}


} // namespace WebKit
#endif
