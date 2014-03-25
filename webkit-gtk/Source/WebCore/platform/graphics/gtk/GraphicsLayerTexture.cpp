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

#include "GraphicsLayerTexture.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GtkLayer.h"
#include "FloatRect.h"

#include <wtf/text/CString.h>

#include <stdio.h>
#include <wtf/OwnPtr.h>

namespace WebCore {

PassOwnPtr<GraphicsLayer> GraphicsLayer::create(GraphicsLayerClient* client)
{
    return adoptPtr(new GraphicsLayerTexture(client));
}

GraphicsLayerTexture::GraphicsLayerTexture(GraphicsLayerClient* client)
    : GraphicsLayer(client)
{
    m_platformLayer = GtkLayer::create(this);
}

GraphicsLayerTexture::~GraphicsLayerTexture()
{
    m_platformLayer.clear();
}

void GraphicsLayerTexture::setName(const String & name)
{
    String layerName = String::format("GraphicsLayerTexture[%p]", this) + name;
    GraphicsLayer::setName(layerName);
}

bool GraphicsLayerTexture::setChildren(const Vector<GraphicsLayer*> & children)
{
    return GraphicsLayer::setChildren(children);
}

void GraphicsLayerTexture::addChild(GraphicsLayer *child)
{
    GraphicsLayer::addChild(child);
}

void GraphicsLayerTexture::addChildAtIndex(GraphicsLayer *child, int index)
{
    GraphicsLayer::addChildAtIndex(child, index);
}

void GraphicsLayerTexture::addChildAbove(GraphicsLayer *child, GraphicsLayer *sibling)
{
    GraphicsLayer::addChildAbove(child, sibling);
}

void GraphicsLayerTexture::addChildBelow(GraphicsLayer *child, GraphicsLayer *sibling)
{
    GraphicsLayer::addChildBelow(child, sibling);
}

bool GraphicsLayerTexture::replaceChild(GraphicsLayer *oldChild, GraphicsLayer *newChild)
{
    return GraphicsLayer::replaceChild(oldChild, newChild);
}

void GraphicsLayerTexture::removeFromParent()
{
    GraphicsLayer::removeFromParent();
}

void GraphicsLayerTexture::setMaskLayer(GraphicsLayer* layer)
{
    if (layer == m_maskLayer)
        return;

    GraphicsLayer::setMaskLayer(layer);
}

void GraphicsLayerTexture::setReplicatedByLayer(GraphicsLayer* layer)
{
    if (layer == m_replicaLayer)
        return;

    GraphicsLayer::setReplicatedByLayer(layer);
}

void GraphicsLayerTexture::setMasksToBounds(bool b)
{
    if (b == m_masksToBounds)
        return;

    GraphicsLayer::setMasksToBounds(b);
}

void GraphicsLayerTexture::setPosition(const FloatPoint& position)
{
    if (position == m_position)
        return;

    GraphicsLayer::setPosition(position);
    m_platformLayer->setPosition(position);
}

void GraphicsLayerTexture::setAnchorPoint(const FloatPoint3D& anchorPoint)
{
    if (anchorPoint == m_anchorPoint)
        return;

    GraphicsLayer::setAnchorPoint(anchorPoint);
    m_platformLayer->setAnchorPoint(anchorPoint);
}

void GraphicsLayerTexture::setSize(const FloatSize& size)
{
    if (size == m_size)
        return;

    GraphicsLayer::setSize(size);
    m_platformLayer->setSize(size);
}

void GraphicsLayerTexture::setTransform(const TransformationMatrix& transform)
{
    if (transform == m_transform)
        return;

#if 0
    printf(" My name is %d\n", (int) this );
    printf("************** setTransform is called\n");
    printf("===============================================\n");
    printf("Transform Matrix\n");
    printf("%f \t %f \t %f \t %f\n", transform.m11(), transform.m12(), transform.m13(), transform.m14());
    printf("%f \t %f \t %f \t %f\n", transform.m21(), transform.m22(), transform.m23(), transform.m24());
    printf("%f \t %f \t %f \t %f\n", transform.m31(), transform.m32(), transform.m33(), transform.m34());
    printf("%f \t %f \t %f \t %f\n", transform.m41(), transform.m42(), transform.m43(), transform.m44());
#endif

    GraphicsLayer::setTransform(transform);
    m_platformLayer->setTransform(transform);
}

void GraphicsLayerTexture::setChildrenTransform(const TransformationMatrix& transform)
{
    if (transform == m_childrenTransform)
        return;

    GraphicsLayer::setChildrenTransform(transform);
    m_platformLayer->setChildrenTransform(transform);
}

void GraphicsLayerTexture::setPreserves3D(bool b)
{
    if (b == m_preserves3D)
        return;

    GraphicsLayer::setPreserves3D(b);
}

void GraphicsLayerTexture::setContentsRect(const IntRect& rect)
{
    if (rect == m_contentsRect)
        return;

    GraphicsLayer::setContentsRect(rect);
    m_platformLayer->setContentsRect(rect);
}

void GraphicsLayerTexture::setDrawsContent(bool b)
{
    if (b == m_drawsContent)
        return;

    GraphicsLayer::setDrawsContent(b);
    m_platformLayer->setDrawsContent(b);
}

void GraphicsLayerTexture::setContentsToImage(Image* image)
{
    m_platformLayer->setContentsToImage(image);
}

void GraphicsLayerTexture::setContentsToMedia(PlatformLayer* layer)
{
    m_platformLayer->setContentsToMedia(layer);
}

void GraphicsLayerTexture::setContentsToCanvas(PlatformLayer* layer)
{
    if (!layer)
        return;

    if (m_platformLayer != layer)
	    m_platformLayer = layer;

    m_platformLayer->setOwner(this);
    m_platformLayer->setNeedsDisplay();
}

void GraphicsLayerTexture::setContentsBackgroundColor(const Color& color)
{
    m_platformLayer->setContentsToColor(color);
}

bool GraphicsLayerTexture::hasContentsLayer() const
{
    return m_platformLayer->hasVisibleContents();
}

void GraphicsLayerTexture::setContentsOrientation(CompositingCoordinatesOrientation orientation)
{
    if (orientation == m_contentsOrientation)
        return;

    GraphicsLayer::setContentsOrientation(orientation);
}

void GraphicsLayerTexture::setBackgroundColor(const Color& color)
{
    if (color == m_backgroundColor)
        return;

    GraphicsLayer::setBackgroundColor(color);
}

void GraphicsLayerTexture::clearBackgroundColor()
{
    GraphicsLayer::clearBackgroundColor();
}

void GraphicsLayerTexture::setContentsOpaque(bool b)
{
    if (b == m_contentsOpaque)
        return;

    GraphicsLayer::setContentsOpaque(b);
}

void GraphicsLayerTexture::setBackfaceVisibility(bool b)
{
    if (b == m_backfaceVisibility)
        return;

    GraphicsLayer::setBackfaceVisibility(b);
}

void GraphicsLayerTexture::setOpacity(float opacity)
{
    if (opacity == m_opacity)
        return;

    GraphicsLayer::setOpacity(opacity);
}

void GraphicsLayerTexture::setNeedsDisplay()
{
    m_platformLayer->setNeedsDisplay();
}

void GraphicsLayerTexture::setNeedsDisplayInRect(const FloatRect& rect)
{
    m_platformLayer->setNeedsDisplayInRect(rect);
}

void GraphicsLayerTexture::setContentsNeedsDisplay()
{
    m_platformLayer->setContentsNeedsDisplay();
}

bool GraphicsLayerTexture::addAnimation(const KeyframeValueList& values, const IntSize& boxSize, const Animation* anim, const String& keyframesName, double timeOffset)
{
    return false;
}

void GraphicsLayerTexture::pauseAnimation(const String& name, double timeOffset)
{

}

void GraphicsLayerTexture::removeAnimation(const String& name)
{

}

void GraphicsLayerTexture::suspendAnimations(double time)
{

}

void GraphicsLayerTexture::resumeAnimations()
{

}

}

#endif

