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

#ifndef GtkLayer_h
#define GtkLayer_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsLayerTexture.h"
#include "FloatPoint.h"
#include "FloatRect.h"

#include <wtf/PassRefPtr.h>

namespace WebCore {

class LayerTransformation;
class GraphicsContext3D;
class LayerContents;
class LayerTextureManager;
class GraphicsLayerRenderer;

class GtkLayer : public RefCounted<GtkLayer>
{
public:
    static PassRefPtr<GtkLayer> create(GraphicsLayerTexture* owner);

    virtual ~GtkLayer();

    // for debugging
    void setOwner(GraphicsLayerTexture* owner) { m_owner = owner; }
    GraphicsLayerTexture* getOwner() { return m_owner; }

    void setRenderer(GraphicsLayerRenderer* renderer);

    GtkLayer* parent();

    void setPosition(const FloatPoint& position);
    FloatPoint position() const;

    void setAnchorPoint(const FloatPoint3D& anchorPoint);
    FloatPoint3D anchorPoint() const;

    void setSize(const FloatSize& size);
    FloatSize size() const;

    void setTransform(const TransformationMatrix& transform);
    const TransformationMatrix& transform() const;

    void setChildrenTransform(const TransformationMatrix& transform);
    TransformationMatrix childrenTransform() const;

    void setContentsRect(const IntRect& rect);
    void setDrawsContent(bool b);

    void setContentsToImage(Image* image);
    void setContentsToMedia(PlatformLayer* layer);
    void setContentsToCanvas(GraphicsContext3D* context);
    void setContentsToColor(const Color& color);

    void setNeedsDisplay();
    void setNeedsDisplayInRect(const FloatRect& rect);
    void setContentsNeedsDisplay();

    bool masksToBounds() const { return m_owner->masksToBounds(); }
    bool preserves3D() const { return m_owner->preserves3D(); }
    bool backfaceVisibility() const { return m_owner->backfaceVisibility(); }

    void updateLayerMatrix();
    void updateContentsMatrix();
    void updateAlpha();
    void updateContents(LayerTextureManager* manager);
    void updateTexture(LayerTextureManager* manager);

    int contentsType();
    bool hasVisibleContents() const { return (bool)m_layerContents; }
    bool needsToUpdateContents() const { return !m_dirtyRect.isEmpty(); }

    unsigned getTexture();
	void setTexture(unsigned textureId);
    float getAlpha() { return m_layerAlpha; }

    float* getRenderMatrix(const TransformationMatrix& projection);
    const TransformationMatrix& getLayerMatrix() const;
    const TransformationMatrix& getContentsMatrix() const;

    void paintGraphicsLayerContents(GraphicsContext& context, const IntRect& clip) { m_owner->paintGraphicsLayerContents(context, clip); }

protected:
    GtkLayer(GraphicsLayerTexture* owner);

private:
    void notifySyncRequired();

    void releaseResources();

    GraphicsLayerTexture* m_owner;

    RefPtr<GraphicsLayerRenderer> m_renderer;

    OwnPtr<LayerTransformation> m_layerTransformation;
    OwnPtr<LayerContents> m_layerContents;
    float m_layerAlpha;

    FloatRect m_dirtyRect;
};

}

#endif // USE(ACCELERATED_COMPOSITING)

#endif // GtkLayer_h
