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

#ifndef GraphicsLayerGtk_h
#define GraphicsLayerGtk_h

#if USE(ACCELERATED_COMPOSITING)
#include <stdio.h>
#include "GraphicsLayer.h"
#include "GraphicsLayerClient.h"


namespace WebCore {

class AnimationGtkBase;

enum ChangeMask {
    NoChanges =                 0,

    ParentChange =              (1L << 0),
    ChildrenChange =            (1L << 1),
    MaskLayerChange =           (1L << 2),
    PositionChange =            (1L << 3),

    AnchorPointChange =         (1L << 4),
    SizeChange  =               (1L << 5),
    TransformChange =           (1L << 6),
    ContentChange =             (1L << 7),

    ContentsOrientationChange = (1L << 8),
    OpacityChange =             (1L << 9),
    ContentsRectChange =        (1L << 10),

    Preserves3DChange =         (1L << 11),
    MasksToBoundsChange =       (1L << 12),
    DrawsContentChange =        (1L << 13),
    ContentsOpaqueChange =      (1L << 14),

    BackfaceVisibilityChange =  (1L << 15),
    ChildrenTransformChange =   (1L << 16),
    DisplayChange =             (1L << 17),
    BackgroundColorChange =     (1L << 18),

    DistributesOpacityChange =  (1L << 19)
};

//#if !USE(ACCELERATED_COMPOSITING)
#if 0

class GraphicsLayerGtk : public GraphicsLayer {
public:
     GraphicsLayerGtk(GraphicsLayerClient*);
     virtual ~GraphicsLayerGtk();
 
     void createNativeLayerIfEmpty();
     virtual PlatformLayer* platformLayer() {
         return o;
     }
//     void setPlatformLayer(Evas_Object* o_);
     void setZoomFactor(float zoom_level) { m_zoom_level = zoom_level; }
     void setViewPosition(int x, int y) {
         view_geo_x = x;
         view_geo_y = y;
     }
 
     virtual void setNeedsDisplay();
     virtual void setNeedsDisplayInRect(const FloatRect&);
     virtual void setParent(GraphicsLayer* layer);
     virtual void setName(const String& name);
     virtual bool setChildren(const Vector<GraphicsLayer*>&);
     virtual void addChild(GraphicsLayer*);
     virtual void addChildAtIndex(GraphicsLayer*, int index);
     virtual void addChildAbove(GraphicsLayer* layer, GraphicsLayer* sibling);
     virtual void addChildBelow(GraphicsLayer* layer, GraphicsLayer* sibling);
     virtual bool replaceChild(GraphicsLayer* oldChild, GraphicsLayer* newChild);
     virtual void removeFromParent();
     virtual void setMaskLayer(GraphicsLayer* layer);
     virtual void setPosition(const FloatPoint& p);
     virtual void setAnchorPoint(const FloatPoint3D& p);
     virtual void setSize(const FloatSize& size);
     virtual void setTransform(const TransformationMatrix& t);
     virtual void setChildrenTransform(const TransformationMatrix& t);
     virtual void setPreserves3D(bool b);
     virtual void setMasksToBounds(bool b);
     virtual void setDrawsContent(bool b);
     virtual void setBackgroundColor(const Color&);
     virtual void clearBackgroundColor();
     virtual void setContentsOpaque(bool b);
     virtual void setBackfaceVisibility(bool b);
     virtual void setOpacity(float opacity);
     virtual void setContentsRect(const IntRect& r);
     virtual void setContentsToImage(Image*);
     virtual void setContentsNeedsDisplay();
     virtual void setContentsToMedia(PlatformLayer*);
     virtual void setContentsBackgroundColor(const Color&);
 
     virtual void setContentsOrientation(
             CompositingCoordinatesOrientation orientation);
     virtual void distributeOpacity(float);
     virtual void syncCompositingState();
     virtual void syncCompositingStateForThisLayerOnly();

     virtual bool addAnimation(const KeyframeValueList&, const IntSize& /*boxSize*/, const Animation*, const String& /*animationName*/, double /*timeOffset*/);
     virtual void pauseAnimation(const String& /*animationName*/, double /*timeOffset*/) ;
     virtual void removeAnimation(const String& /*animationName*/) ;
     virtual void suspendAnimations(double time);
     virtual void resumeAnimations();

     GraphicsLayerGtk* rootLayer();
     void notifySyncRequired();
     void notifyChange(ChangeMask);
     void flushChanges(bool recursive = true, bool forceTransformUpdate = false);
     void updatePlatformLayer();

     virtual void paintPlatformLayer();
     TransformationMatrix localTransform();

     void setBaseTransform(const TransformationMatrix& transformMatrix);
     void notifyAnimationStarted();

    enum StaticContentType { HTMLContentType, PixmapContentType, ColorContentType, MediaContentType, Canvas3DContentType};

	struct ContentData {
		cairo_surface_t* pixmap;
//		Eina_Tiler* regionToUpdate;

		bool updateAll;

		StaticContentType contentType;

		float 				opacity;
		FloatPoint3D anchorPoint;
		FloatPoint 	position;
		FloatSize 	size;
		IntRect		contentRect;
		TransformationMatrix transform;

		unsigned textureId;	// FIXME: hyowon - should be deleted by GraphicsContext3D
		IntSize	 textureSize;
		TransformationMatrix worldMatrix;

		ContentData()
			: updateAll(false)
			, contentType(HTMLContentType)
			, opacity(1.0f)
			, pixmap(NULL)
			, textureId(0)
		{
		}
	};

	ContentData* GetCurrentContentData() { return &m_currentContent; }

	bool IsContentsChanged() const { return needContentsUpdate; }
	void SetContentsUpdated() { needContentsUpdate = false; }
	TransformationMatrix GetWorldMatrix(){return m_worldransform;}
	float GetZoomLevel() {return m_zoom_level;}

	float getZoomFactor() const { return m_zoom_level; }
	IntPoint getViewPosition() const { return IntPoint(view_geo_x, view_geo_y);}

	void SetEvasOpacity(double opacity);

	IntRect GetDirtyRect() {
#if 0
		IntRect rect;
		if (!m_pendingContent.regionToUpdate) {
			return rect;
		}

		Eina_Iterator* itr = eina_tiler_iterator_new(m_pendingContent.regionToUpdate);
		if (!itr) {
			return rect;
		}

		Eina_Rectangle* r;
		EINA_ITERATOR_FOREACH(itr, r) {
			rect = IntRect(*r);
			return rect;
		}
#endif
printf("[HZLE] %s %d\n", __func__, __LINE__);
		IntRect rect;
		return rect;
	}

	void ClearDirtyRect() {
#if 0
		if (m_pendingContent.regionToUpdate) {
			eina_tiler_free(m_pendingContent.regionToUpdate);
			m_pendingContent.regionToUpdate = NULL;
		}
#endif
printf("[HZLE] %s %d\n", __func__, __LINE__);
	}

private:
    virtual void updateTransform();

public:
    bool isTransformAnimationRunning;
    bool isOpacityAnimationRunning;

protected:
    TransformationMatrix m_worldransform;

private:
#if 0
    Evas_Object* o;
    Ecore_Idler* notifyFunction;
#endif

    bool isAttached;
    bool needContentsUpdate;
    bool isPainted;
    bool isTransformed;
    float m_zoom_level;
    int view_geo_x;
    int view_geo_y;

    int m_changeMask;

    TransformationMatrix m_baseTransform;

    ContentData m_pendingContent;
    ContentData m_currentContent;

   // std::list<AnimationGtkBase* > m_animationList;
};

#endif // USE(ACCELERATED_COMPOSITING)

}

#endif // USE(ACCELERATED_COMPOSITING)

#endif // GraphicsLayerGtk_h
