/*
    Copyright (C) 2009-2010 Samsung Electronics

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

#ifndef GraphicsLayerTexture_h
#define GraphicsLayerTexture_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsLayer.h"

namespace WebCore {

class GtkLayer;

class GraphicsLayerTexture : public GraphicsLayer
{
public:
	GraphicsLayerTexture(GraphicsLayerClient* client);
	~GraphicsLayerTexture();

	virtual void setName(const String& name);

    virtual bool setChildren(const Vector<GraphicsLayer*>& children);
    virtual void addChild(GraphicsLayer* child);
	virtual void addChildAtIndex(GraphicsLayer* child, int index);
	virtual void addChildAbove(GraphicsLayer* child, GraphicsLayer* sibling);
	virtual void addChildBelow(GraphicsLayer* child, GraphicsLayer* sibling);
	virtual bool replaceChild(GraphicsLayer* oldChild, GraphicsLayer* newChild);
	virtual void removeFromParent();

	virtual void setMaskLayer(GraphicsLayer* layer);
	virtual void setReplicatedByLayer(GraphicsLayer* layer);
	virtual void setMasksToBounds(bool b);

	virtual void setPosition(const FloatPoint& position);
	virtual void setAnchorPoint(const FloatPoint3D& anchorPoint);
	virtual void setSize(const FloatSize& size);

	virtual void setTransform(const TransformationMatrix& transform);
	virtual void setChildrenTransform(const TransformationMatrix& transform);

	virtual void setPreserves3D(bool b);

	virtual void setContentsRect(const IntRect& rect);
	virtual void setDrawsContent(bool b);

	virtual void setContentsToImage(Image* image);
	virtual void setContentsToMedia(PlatformLayer* layer);
	virtual void setContentsToCanvas(PlatformLayer* layer);
	virtual void setContentsBackgroundColor(const Color& color);
	virtual bool hasContentsLayer() const;

	virtual void setContentsOrientation(CompositingCoordinatesOrientation orientation);

    virtual void setBackgroundColor(const Color& color);
    virtual void clearBackgroundColor();

    virtual void setContentsOpaque(bool b);
    virtual void setBackfaceVisibility(bool b);
    virtual void setOpacity(float opacity);

    virtual void setNeedsDisplay();
    virtual void setNeedsDisplayInRect(const FloatRect& rect);
    virtual void setContentsNeedsDisplay();

    virtual bool addAnimation(const KeyframeValueList& values, const IntSize& boxSize, const Animation* anim, const String& keyframesName, double timeOffset);
	virtual void pauseAnimation(const String& name, double timeOffset);
	virtual void removeAnimation(const String& name);

	virtual void suspendAnimations(double time);
	virtual void resumeAnimations();

	virtual PlatformLayer* platformLayer() const { return (PlatformLayer*)m_platformLayer.get(); }

//	virtual void setDebugBackgroundColor(const Color& color);
//  virtual void setDebugBorder(const Color& color, float borderWidth);

private:
	RefPtr<GtkLayer> m_platformLayer;
};

}

#endif

#endif // GraphicsLayerTexture_h















