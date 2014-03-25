/*
    Copyright (C) 2011 Samsung Electronics

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#if 0
#include "config.h"
#include "GraphicsLayerGtk.h"

#include "CurrentTime.h"
#include "FloatRect.h"
#include "GraphicsContext.h"
#include "Image.h"
#include "RefCounted.h"

#include <string>
#include "math.h"

#define printf //

#if USE(ACCELERATED_COMPOSITING)
#if !USE(ACCELERATED_COMPOSITING_WITH_GLES)

namespace WebCore {

static Eina_Bool _paint(void* data)
{
	printf("_Paint is called!! \n");
    GraphicsLayerGtk* obj = (GraphicsLayerGtk *)data;
    obj->paintPlatformLayer();
    return ECORE_CALLBACK_CANCEL;
}

static Eina_Bool _notify(void* data)
{
    GraphicsLayerGtk* layer = (GraphicsLayerGtk *)data;
    layer->notifySyncRequired();
    return ECORE_CALLBACK_CANCEL;
}

static Eina_Bool TimerFunction(void* data)
{
	Evas_Object* eo = (Evas_Object*)data;
	if( eo )
	{
		evas_object_del(eo);
		printf("ecore_idle_exiter_add is called!!\n");
	}

	return false;
}

static void quat_2_euler_ogl(const double x, const double y, const double z, const double w,  float& yaw, float& pitch, float& roll)
{
    float sqw = w*w;
    float sqx = x*x;
    float sqy = y*y;
    float sqz = z*z;

    pitch = asinf(2.0f * (y*z + w*x)); // rotation about x-axis
    roll = atan2f(2.0f * (w*y - x*z),(-sqx - sqy + sqz + sqw)); // rotation about y-axis
    yaw = atan2f(2.0f * (w*z - x*y),(-sqx + sqy - sqz + sqw)); // rotation about z-axis
}

PassOwnPtr<GraphicsLayer> GraphicsLayer::create(GraphicsLayerClient* client)
{
    return new GraphicsLayerGtk(client);
}


GraphicsLayerGtk::GraphicsLayerGtk(GraphicsLayerClient* client)
    : GraphicsLayer(client)
    , o(0)
    , notifyFunction(NULL)
    , m_changeMask(NoChanges)
    , isAttached(false)
    , needContentsUpdate(false)
    , isPainted(false)
    , isTransformed(false)
	, isTransformAnimationRunning(false)
	, isOpacityAnimationRunning(false)
    , m_zoom_level(1.0f)
    , view_geo_x(0)
    , view_geo_y(0)
{
    m_pendingContent.regionToUpdate = NULL;
    printf("GraphicsLayerGtk created!\n");
}

GraphicsLayerGtk::~GraphicsLayerGtk()
{
    if (notifyFunction) {
        ecore_idler_del(notifyFunction);
        notifyFunction = NULL;
    }

    // Remove animations
    std::list<AnimationGtkBase* >::iterator it =  m_animationList.begin();
    for( ; it != m_animationList.end(); ++it )
    {
    	AnimationGtkBase* pAnimation = (*it);
    	ecore_animator_del( pAnimation->m_hAnimation );
    }
    m_animationList.clear();

    if (!isAttached && o) {
        evas_object_del(o);
    	o = 0;
    }

    printf("GraphicsLayerGtk Destructor is called!\n");
}

void GraphicsLayerGtk::setPlatformLayer(Evas_Object* o_)
{
    o = o_;
    isAttached = true;
}

void GraphicsLayerGtk::setNeedsDisplay()
{
    if (m_pendingContent.regionToUpdate)
        eina_tiler_free(m_pendingContent.regionToUpdate);
    m_pendingContent.regionToUpdate = eina_tiler_new(size().width(), size().height());

    Eina_Rectangle r = {0, 0, size().width(), size().height()};
    eina_tiler_rect_add(m_pendingContent.regionToUpdate, &r);
    notifyChange(DisplayChange);
}

void GraphicsLayerGtk::setNeedsDisplayInRect(const FloatRect& rect)
{
    if (!m_pendingContent.regionToUpdate)
        m_pendingContent.regionToUpdate = eina_tiler_new(size().width(), size().height());

    Eina_Rectangle r = enclosingIntRect(rect);
    eina_tiler_rect_add(m_pendingContent.regionToUpdate, &r);
    notifyChange(DisplayChange);
}

void GraphicsLayerGtk::setContentsNeedsDisplay()
{
}

void GraphicsLayerGtk::setOpacity(float value)
{
    if (value == opacity())
        return;

    GraphicsLayer::setOpacity(value);

    m_pendingContent.opacity = value;
    notifyChange(OpacityChange);
}

void GraphicsLayerGtk::setPosition(const FloatPoint& value)
{
	if (value == position())
        return;

    printf("************** setPosition is called  %f, %f\n", value.x(), value.y());

    GraphicsLayer::setPosition(value);
    m_pendingContent.position = value;
    notifyChange(PositionChange);
}

void GraphicsLayerGtk::setTransform( const TransformationMatrix& value )
{
    if( value == transform() )
        return;

    GraphicsLayer::setTransform(value);

    printf(" My name is %d\n", (int) this );
    printf("************** setTransform is called\n");
	printf("===============================================\n");
	printf("Transform Matrix\n");
	printf("%f \t %f \t %f \t %f\n", value.m11(), value.m12(), value.m13(), value.m14());
	printf("%f \t %f \t %f \t %f\n", value.m21(), value.m22(), value.m23(), value.m24());
	printf("%f \t %f \t %f \t %f\n", value.m31(), value.m32(), value.m33(), value.m34());
	printf("%f \t %f \t %f \t %f\n", value.m41(), value.m42(), value.m43(), value.m44());

	m_pendingContent.transform = value;
    notifyChange(TransformChange);
}

void GraphicsLayerGtk::setDrawsContent(bool b)
{
    if(b == drawsContent())
        return;
    GraphicsLayer::setDrawsContent(b);
    notifyChange(DrawsContentChange);
}

void GraphicsLayerGtk::setSize(const FloatSize& value)
{
    if (value == size())
        return;
    GraphicsLayer::setSize(value);
    if (o) {
        evas_object_size_hint_min_set(o, (int)value.width(), (int)value.height());
        evas_object_resize(o, (int)value.width() * m_zoom_level, (int)value.height() * m_zoom_level);
        evas_object_image_size_set(o, (int)value.width(), (int)value.height());
    }
}

void GraphicsLayerGtk::syncCompositingState()
{
    flushChanges();
    GraphicsLayer::syncCompositingState();
}

void GraphicsLayerGtk::setContentsRect(const IntRect& value)
{
    GraphicsLayer::setContentsRect(value);
    m_pendingContent.contentRect = value;
    notifyChange(ContentsRectChange);
}

static int myWidth = 0;
static int myHeight = 0;

void GraphicsLayerGtk::setContentsToImage(Image* image)
{
    m_pendingContent.contentType = HTMLContentType;
    GraphicsLayer::setContentsToImage(image);

    if (image) {
        cairo_surface_t* img = image->nativeImageForCurrentFrame();
        if (img) {
            m_pendingContent.pixmap = img;
            m_pendingContent.contentType = PixmapContentType;
        }
    }

    myWidth = image->width();
    myHeight = image->height();

    //FIXME: moved notifyChange because notifyChange call flushChange immediately.
    notifyChange(ContentChange);
}

void GraphicsLayerGtk::setAnchorPoint(const FloatPoint3D& value)
{
    if (value == anchorPoint())
        return;
    GraphicsLayer::setAnchorPoint(value);
    m_pendingContent.anchorPoint = value; 

    notifyChange(AnchorPointChange);
}

void GraphicsLayerGtk::setContentsToMedia(PlatformLayer* media)
{
    m_pendingContent.contentType = HTMLContentType;
    GraphicsLayer::setContentsToMedia(media);

    notifyChange(ContentChange);
}

void GraphicsLayerGtk::setContentsBackgroundColor(const Color& color)
{
}

GraphicsLayerGtk* GraphicsLayerGtk::rootLayer()
{
    if (GraphicsLayerGtk* p = static_cast<GraphicsLayerGtk*>(parent()))
        return p->rootLayer();
    return this;
}

void GraphicsLayerGtk::notifySyncRequired()
{
    if (client())
        client()->notifySyncRequired(this);
    notifyFunction = 0;
}

void GraphicsLayerGtk::notifyChange(ChangeMask changeMask)
{
    m_changeMask |= changeMask;
    if (!notifyFunction) {
        notifyFunction = ecore_idler_add(_notify, this);
    }
}

void GraphicsLayerGtk::flushChanges(bool recursive, bool forceUpdateTransform)
{
    if (m_changeMask == NoChanges)
        goto afterLayerChanges;

    if (m_changeMask & (ChildrenTransformChange | Preserves3DChange | TransformChange | AnchorPointChange | SizeChange | BackfaceVisibilityChange | PositionChange | ParentChange))
        forceUpdateTransform = true;

    if (m_changeMask & ContentsRectChange) {
        if( m_currentContent.contentRect != m_pendingContent.contentRect && m_pendingContent.pixmap )
            needContentsUpdate = true;
    }


    if (m_changeMask & (ContentChange | DrawsContentChange | MaskLayerChange)) {
        switch (m_pendingContent.contentType) {
        case HTMLContentType:
            if (drawsContent()) {
                updatePlatformLayer();
            }
            break;
        case PixmapContentType:
            updatePlatformLayer();
            break;
        case ColorContentType:
            break;
        }
    }

    // FIXME: 이전과 같은 투명도일 경우 실행 하지 않는 조건 필요
    if ((m_changeMask & OpacityChange) &&  isOpacityAnimationRunning == false )
    	SetEvasOpacity(opacity());

    if (m_changeMask & DisplayChange)
        updatePlatformLayer();


    m_currentContent.pixmap = m_pendingContent.pixmap;
    m_currentContent.contentType = m_pendingContent.contentType;
    m_currentContent.position = m_pendingContent.position;
    m_currentContent.transform = m_pendingContent.transform;
    m_currentContent.opacity = m_pendingContent.opacity;
    m_currentContent.contentRect = m_pendingContent.contentRect;


    m_changeMask = NoChanges;

afterLayerChanges:
    if (forceUpdateTransform)
        updateTransform();

    if (!recursive)
        return;

    const Vector<GraphicsLayer*>& childLayers = children();
    size_t numChildren = childLayers.size();
    for (size_t i = 0; i < numChildren; ++i) {
        GraphicsLayerGtk* curChild = static_cast<GraphicsLayerGtk*>(childLayers[i]);
        curChild->flushChanges(true, forceUpdateTransform);
    }
}

void GraphicsLayerGtk::createNativeLayerIfEmpty()
{
    if (o)
        return;

    Evas_Object* root = rootLayer()->platformLayer();
    if (!root)
        return;

    // FIXME
    m_zoom_level = rootLayer()->m_zoom_level;

    view_geo_x = rootLayer()->view_geo_x;
    view_geo_y = rootLayer()->view_geo_y;

    Evas* evas = evas_object_evas_get(root);
    o = evas_object_image_add(evas);
    evas_object_image_alpha_set(o, EINA_TRUE);
    evas_object_pass_events_set(o, EINA_TRUE);

    evas_object_move(o, (int)position().x(), (int)position().y());
    evas_object_size_hint_min_set(o, (int)size().width(), (int)size().height());
    evas_object_resize(o, (int)size().width(), (int)size().height());
    evas_object_image_size_set(o, (int)size().width(), (int)size().height());
    evas_object_image_fill_set(o, 0, 0, (int)size().width(), (int)size().height());
    evas_object_image_filled_set(o, EINA_TRUE);
}

void GraphicsLayerGtk::updatePlatformLayer()
{
	paintPlatformLayer();
}

void GraphicsLayerGtk::paintPlatformLayer()
{
    if (this == rootLayer())
        return;

    printf(" My name is %d\n", (int) this );

    printf("paintPlatformLayer 1\n");

    createNativeLayerIfEmpty();

    void* pixels;
    cairo_status_t status;
    cairo_surface_t* surface;
    cairo_format_t format;
    cairo_t* cairo;
    int ow, oh;

    format = CAIRO_FORMAT_ARGB32;
    switch (m_currentContent.contentType) {
    case HTMLContentType:
        if (drawsContent()) {

        	printf("paintPlatformLayer 2\n");
            evas_object_image_size_get(o, &ow, &oh);
            pixels = evas_object_image_data_get(o, 1);

            surface = cairo_image_surface_create_for_data((unsigned char*)pixels, format, ow, oh, ow * 4);
            status = cairo_surface_status(surface);
            if (status != CAIRO_STATUS_SUCCESS) {
                printf("cairo surface error\n");
            }
            cairo = cairo_create(surface);
            status = cairo_status(cairo);
            if (status != CAIRO_STATUS_SUCCESS) {
                printf("cairo error\n");
            }
            GraphicsContext gc(cairo);
            cairo_save(cairo);

            Eina_Rectangle *r;
            Eina_Iterator *itr;


            if (!m_pendingContent.regionToUpdate) {
                break;
            }

            printf("paintPlatformLayer 3\n");

            itr = eina_tiler_iterator_new(m_pendingContent.regionToUpdate);
            if (!itr)
                break;

            printf("paintPlatformLayer 4\n");

            EINA_ITERATOR_FOREACH(itr, r) {
                IntRect rect = IntRect(*r);
                gc.clearRect(rect);

                printf("HTMLContentType is painted!!\n");
                paintGraphicsLayerContents(gc, rect);
            }
            cairo_restore(cairo);
            cairo_destroy(cairo);
            cairo_surface_destroy(surface);
            evas_object_image_data_update_add(o, 0, 0, ow, oh);
            evas_object_image_data_set(o, pixels);
        }
        break;
    case PixmapContentType:


    	printf("paintPlatformLayer 5\n");
        if(needContentsUpdate == false)
            break;


        printf("paintPlatformLayer 6\n");

        needContentsUpdate = false;

        evas_object_image_size_get(o, &ow, &oh);
        pixels = evas_object_image_data_get(o, 1);

        surface = cairo_image_surface_create_for_data((unsigned char*)pixels, format, ow, oh, ow * 4);
        status = cairo_surface_status(surface);
        if (status != CAIRO_STATUS_SUCCESS) {
            printf("cairo surface error\n");
        }
        cairo = cairo_create(surface);
        status = cairo_status(cairo);
        if (status != CAIRO_STATUS_SUCCESS) {
            printf("cairo error\n");
        }
        cairo_set_operator(cairo, CAIRO_OPERATOR_SOURCE);

        cairo_scale(cairo, ((float)ow/(float)myWidth), (float)((float)oh/(float)myHeight));
        cairo_set_source_surface(cairo, m_currentContent.pixmap, 0, 0);
        cairo_paint(cairo);
        cairo_destroy(cairo);
        cairo_surface_destroy(surface);
        evas_object_image_data_update_add(o, 0, 0, ow, oh);
        evas_object_image_data_set(o, pixels);
        break;
    }

    if (m_pendingContent.regionToUpdate) {
        eina_tiler_free(m_pendingContent.regionToUpdate);
        m_pendingContent.regionToUpdate = NULL;
    }

    if (isTransformed)
        evas_object_show(o);

    isPainted = true;

    evas_object_smart_changed(o);
}

void GraphicsLayerGtk::updateTransform()
{
    createNativeLayerIfEmpty();

    if( isTransformAnimationRunning == false )
    	m_baseTransform = transform();

    //TransformationMatrix worldMatrix = localTransform().multLeft( m_baseTransform );

    TransformationMatrix worldMatrix = localTransform();

    GraphicsLayerGtk* parentLayer = static_cast<GraphicsLayerGtk*>(parent());

    while (parentLayer) {
        worldMatrix = parentLayer->localTransform().multLeft(worldMatrix);
        parentLayer = static_cast<GraphicsLayerGtk*>(parentLayer->parent());
    }

    // Decomposition of Transform matrix
    TransformationMatrix& t =  worldMatrix;
    TransformationMatrix::DecomposedType decom;
    t.decompose( decom );

    // Create Evas_Map
    Evas_Map *map = evas_map_new(4);
    Evas_Coord x, y, w, h;
    evas_object_geometry_get(o, &x, &y, &w, &h);

    const Evas_Coord halfWidth = w/2;
    const Evas_Coord halfHeight = h/2;

    // Align the center of this evas object with origin.
    evas_map_util_points_populate_from_geometry(map, -halfWidth, -halfHeight, w, h, 0);

    // The center of this evas object is (0,0)
    Evas_Coord cx = 0;
    Evas_Coord cy = 0;

    ////////////////////////////////////////////////////
    // 3D Transformation
    // Order :  Scaling -> Rotation -> Translation

    // 1. Scaling
    evas_map_util_zoom(map, decom.scaleX * m_zoom_level, decom.scaleY * m_zoom_level, cx, cy);

    // 2. Rotation
    float yaw, pitch, roll;
    quat_2_euler_ogl( decom.quaternionX, decom.quaternionY, decom.quaternionZ, decom.quaternionW, yaw, pitch, roll);
    evas_map_util_rotate(map,  -rad2deg(yaw), cx, cy);

    //3. Translation
    Evas_Coord px[4], py[4], pz[4];
    for (int i=0; i<4; i++)
    {
        evas_map_point_coord_get(map, i, &px[i], &py[i], &pz[i]);

        px[i] += decom.translateX * m_zoom_level + view_geo_x + halfWidth * m_zoom_level * decom.scaleX;
        py[i] += decom.translateY * m_zoom_level + view_geo_y + halfHeight * m_zoom_level * decom.scaleY;

        evas_map_point_coord_set(map, i, px[i], py[i], pz[i]);
    }

    // 3D Transformation End
    ////////////////////////////////////////////////////


    evas_object_map_enable_set(o, EINA_TRUE);
    evas_object_map_set(o, map);
    evas_map_free(map);

    if (isPainted)
        evas_object_show(o);

    // Notify Flush to evas
    evas_object_smart_changed(o);

    isTransformed = true;
}

TransformationMatrix GraphicsLayerGtk::localTransform()
{
    TransformationMatrix matrix;

    const double originX = m_pendingContent.anchorPoint.x() * m_pendingContent.size.width();
    const double originY = m_pendingContent.anchorPoint.y() * m_pendingContent.size.height();

    matrix.translate3d( originX + position().x(), originY + position().y(), 0.0 );

    return matrix.multLeft(transform());
}

void GraphicsLayerGtk::setParent(GraphicsLayer* layer)
{
    GraphicsLayer::setParent(layer);
}

void GraphicsLayerGtk::setName(const String& name)
{
    GraphicsLayer::setName(name);
}

bool GraphicsLayerGtk::setChildren(const Vector<GraphicsLayer*>& vecChildren)
{
    return GraphicsLayer::setChildren(vecChildren);
}

void GraphicsLayerGtk::addChild(GraphicsLayer* child)
{
    GraphicsLayer::addChild(child);
}

void GraphicsLayerGtk::addChildAtIndex(GraphicsLayer* child, int index)
{
    GraphicsLayer::addChildAtIndex(child, index);
}

void GraphicsLayerGtk::addChildAbove(GraphicsLayer* layer, GraphicsLayer* sibling)
{
    GraphicsLayer::addChildAbove(layer, sibling);
}

void GraphicsLayerGtk::addChildBelow(GraphicsLayer* layer, GraphicsLayer* sibling)
{
    GraphicsLayer::addChildBelow(layer, sibling);
}

bool GraphicsLayerGtk::replaceChild(GraphicsLayer* oldChild, GraphicsLayer* newChild) {
    return GraphicsLayer::replaceChild(oldChild, newChild);
}

void GraphicsLayerGtk::removeFromParent()
{
    GraphicsLayer::removeFromParent();
}

void GraphicsLayerGtk::setMaskLayer(GraphicsLayer* layer)
{
    GraphicsLayer::setMaskLayer(layer);
}

void GraphicsLayerGtk::setChildrenTransform(const TransformationMatrix& t)
{
    if (t == childrenTransform())
        return;
    GraphicsLayer::setChildrenTransform(t);
}

void GraphicsLayerGtk::setPreserves3D(bool b)
{
    if (b == preserves3D())
        return;
    GraphicsLayer::setPreserves3D(b);
}

void GraphicsLayerGtk::setMasksToBounds(bool b)
{
    if (b == masksToBounds())
        return;
    GraphicsLayer::setMasksToBounds(b);
}
void GraphicsLayerGtk::setBackgroundColor(const Color& color)
{
    if (color == backgroundColor())
        return;

    GraphicsLayer::setBackgroundColor(color);
}

void GraphicsLayerGtk::clearBackgroundColor()
{
    GraphicsLayer::clearBackgroundColor();
}

void GraphicsLayerGtk::setContentsOpaque(bool b)
{
    if (b == contentsOpaque())
        return;

    GraphicsLayer::setContentsOpaque(b);
}

void GraphicsLayerGtk::setBackfaceVisibility(bool b)
{
    if (b == backfaceVisibility())
        return;

    GraphicsLayer::setBackfaceVisibility(b);
}

void GraphicsLayerGtk::setContentsOrientation(CompositingCoordinatesOrientation orientation)
{
    if (orientation == contentsOrientation())
        return;

    GraphicsLayer::setContentsOrientation(orientation);
}

void GraphicsLayerGtk::distributeOpacity(float opacity)
{
    GraphicsLayer::distributeOpacity(opacity);
}

void GraphicsLayerGtk::syncCompositingStateForThisLayerOnly()
{
    GraphicsLayer::syncCompositingStateForThisLayerOnly();
}


void GraphicsLayerGtk::setBaseTransform(const TransformationMatrix& transformMatrix)
{
    m_baseTransform = transformMatrix;
    updateTransform();
}

void GraphicsLayerGtk::SetEvasOpacity(double opacity)
{
	if( o == NULL )
		return;

	int r, g, b, a;
	evas_object_color_get(o, &r, &g, &b, &a);

	int newAlpha = (int)(opacity*255.0);

	printf("RGBA: %d %d %d %d\n", r, g, b, newAlpha);

	evas_object_color_set(o, r, g, b, newAlpha);

	evas_object_smart_changed(o);
}

void GraphicsLayerGtk::notifyAnimationStarted()
{
    // WebCore notifies javascript when the animation starts. Here we're letting it know.
    client()->notifyAnimationStarted(this, /* DOM time */ WTF::currentTime());
}


bool GraphicsLayerGtk::addAnimation(const KeyframeValueList& values, const IntSize& boxSize, const Animation* anim, const String& keyframesName, double timeOffset)
{
    if (!anim->duration() || !anim->iterationCount())
        return false;

    printf("************** addAnimation is called\n");

    AnimationGtkBase* newAnim = 0;

/*
    //  중복된 애니메이션이 들어오는지 체크해서 같다면 새로 생성하지 않는다.
    std::list<AnimationGtkBase* >::iterator it = m_animationList.begin();

    for( ; it != m_animationList.end(); ++it )
    {
    	AnimationGtkBase* pCurAnimation = (*it);
    	if( pCurAnimation && pCurAnimation->m_webkitAnimation == anim )
    	{
    		printf("Same Animation is called!!!! \n");
    		newAnim = pCurAnimation;
    	}
    }
    */

    if (!newAnim)
    {
        switch (values.property())
        {

        case AnimatedPropertyOpacity:
        {
           newAnim = new OpacityAnimationGtk(this, values, boxSize, anim, keyframesName);
        }
           break;
        case AnimatedPropertyWebkitTransform:
        {
        	newAnim = new TransformAnimationGtk(this, values, boxSize, anim, keyframesName);
            TransformAnimationGtk* pAnimation = static_cast<TransformAnimationGtk*>(newAnim);
        }
            break;
        default:
            return false;
        }

        newAnim->setLoopCount(anim->iterationCount());
        newAnim->m_fillsForwards = anim->fillsForwards();

     	printf("Animation Direction: %d\n", (int)anim->direction() );

         m_animationList.push_back(newAnim);
    }

	if (anim->fillsBackwards())
	{
			printf("anim->fillsBackwards() == TRUE\n");
			newAnim->setCurrentTime(0);
	}

 	newAnim->start();
 	newAnim->setCurrentTime( WTF::currentTime() );

   	return true;
}

void GraphicsLayerGtk::pauseAnimation(const String& animationName, double timeOffset)
{
    std::list<AnimationGtkBase* >::iterator it = m_animationList.begin();

    for( ; it != m_animationList.end(); ++it )
    {
    	AnimationGtkBase* pCurAnimation = (*it);

    	if( pCurAnimation == NULL )
    		continue;

    	if( pCurAnimation->m_keyframesName == animationName )
    	{
    		pCurAnimation->pause();
    	}
    }
}

void GraphicsLayerGtk::removeAnimation(const String& animationName)
{

	std::list<AnimationGtkBase* >::iterator it = m_animationList.begin();
    for(; it != m_animationList.end(); )
    {
    	AnimationGtkBase* pCurAnimation = (*it);

    	if( pCurAnimation == NULL )
    		continue;

    	if( pCurAnimation->m_keyframesName == animationName )
    	{
        	ecore_animator_del( pCurAnimation->m_hAnimation );
    		it = m_animationList.erase( it );
    		printf("Remove Animation is called!!\n");
    	}
    	else
    		it++;
    }
}

void GraphicsLayerGtk::suspendAnimations(double time)
{
    std::list<AnimationGtkBase* >::iterator it = m_animationList.begin();
    for( ; it != m_animationList.end(); ++it )
    {
    	AnimationGtkBase* pCurAnimation = (*it);

    	if( pCurAnimation == NULL )
    		continue;

    	pCurAnimation->pause();
    }
}

void GraphicsLayerGtk::resumeAnimations()
{
    std::list<AnimationGtkBase* >::iterator it = m_animationList.begin();
    for( ; it != m_animationList.end(); ++it )
    {
    	AnimationGtkBase* pCurAnimation = (*it);

    	if( pCurAnimation == NULL )
    		continue;

   		pCurAnimation->resume();
   		pCurAnimation->setCurrentTime( WTF::currentTime() );
    }
}

}

#endif
#endif
#endif
