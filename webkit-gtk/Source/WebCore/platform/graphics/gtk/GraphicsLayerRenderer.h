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

#ifndef GraphicsLayerRenderer_h
#define GraphicsLayerRenderer_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsLayerTexture.h"
#include "LayerContents.h"
#include "LayerTransformation.h"

#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

#define GL_SCREEN_WIDTH 1280
#define GL_SCREEN_HEIGHT 720

//#define TVGG_CHECK_FPS 1

namespace WebCore {

#if ENABLE(TVGG_TILED_BACKING_STORE)
class TiledBackingStoreInfo {
public:
    class TileInfo {
    public:
        TileInfo(unsigned textureId, IntRect tileRect) : m_textureId(textureId), m_tileRect(tileRect) { }
        unsigned m_textureId;
        IntRect m_tileRect;
        const IntRect& tileRect() { return m_tileRect; }
        const unsigned& textureId() { return m_textureId; }
    };
    void setVisibleRect(const IntRect& rect) { m_visibleRect = rect; }
    void addTile(unsigned textureId, const IntRect& tileRect) { m_tiles.append(TileInfo(textureId, tileRect));}
    const Vector<TileInfo>& tiles() { return m_tiles; }
    void setHScrollbarInfo(IntRect hscrollbarRect, unsigned hscrollbarTextureId)
    {
        m_hscrollbarTextureId = hscrollbarTextureId;
        m_hscrollbarRect = hscrollbarRect;
    }
    void setVScrollbarInfo(IntRect vscrollbarRect, unsigned vscrollbarTextureId)
    {
        m_vscrollbarTextureId = vscrollbarTextureId;
        m_vscrollbarRect = vscrollbarRect;
    }
    IntRect hscrollbarRect() { return m_hscrollbarRect; }
    IntRect vscrollbarRect() { return m_vscrollbarRect; }
    unsigned hscrollbarTextureId() { return m_hscrollbarTextureId; }
    unsigned vscrollbarTextureId() { return m_vscrollbarTextureId; }
private:
    IntRect m_visibleRect;
    Vector<TileInfo> m_tiles;
    IntRect m_hscrollbarRect;
    IntRect m_vscrollbarRect;
    unsigned m_hscrollbarTextureId;
    unsigned m_vscrollbarTextureId;
};
#endif

class GraphicsContext3D;
class GtkLayer;
class LayerTextureManager;
class LayerShaderManager;
class LayerQuadManager;
class EGLImageUMP;

enum StereoScopicMode {
    StereoScopicNone = 0,
    StereoScopicSBS,
    StereoScopicTNB
};

class GraphicsLayerRenderer : public RefCounted<GraphicsLayerRenderer>
{
public:
	static PassRefPtr<GraphicsLayerRenderer> create(PassRefPtr<GraphicsContext3D> context);

    virtual ~GraphicsLayerRenderer();

    bool initialize(bool bUseExtensionsForTexture = false);

#if ENABLE(TVGG_TILED_BACKING_STORE)
    void setTiledBackingStoreInfo(const TiledBackingStoreInfo&);
#endif
    void clear();
    void renderQuadTexture(unsigned, const IntRect&, const LayerContents::Type);
    void render(int w, int h);
    void renderToEGLImageRenderBuffer(int w, int h);
    void finish();

    void setRootLayer(GraphicsLayer* layer) { m_rootLayer = layer; }
    void copyPixelDataFromFrameBufferToEvas(int x, int y, int w, int h, void* evas);
    void initializeBackingStoreBuffer(const IntSize& size);
    void initializeRootBuffer(const IntSize& size);
    void* getBackingStoreBuffer(IntSize* size);
    void* getRootBuffer(IntSize* size);

    void setUmpID(unsigned int frontID, unsigned int backID);
    void swapUmpID();
    void setUIViewSize(const IntSize& size) { m_UIViewSize = size; }
    void setUIViewPosition(const IntPoint& point) { m_UIViewPosition = point; }
    void setContentsViewSize(const IntSize& size) { m_contentsViewRect.setSize(size); }
    void setContentsViewLocation(const IntPoint& point) { m_contentsViewRect.setLocation(point); }
    void setContentsViewCompositingMode(bool mode) { m_isContentsViewCompositingMode = mode; }
    void setStereoScopicMode(StereoScopicMode mode) { GraphicsLayerRenderer::s_stereoScopicMode = mode; }
    void setStereoScopicSize(const IntSize& size) { GraphicsLayerRenderer::s_stereoScopicSize = size; }
#if ENABLE(TVGG_THREAD_MODEL)
    void setContentsViewGraphicsLayer(GraphicsLayer* layer) { m_contentsViewGraphicsLayer = layer; }
#endif

    LayerTextureManager* textureManager() { return m_textureManager.get(); }
    LayerShaderManager* shaderManager() { return m_shaderManager.get(); }
    LayerQuadManager* quadManager() { return m_quadManager.get(); }
    GraphicsContext3D* context() { return m_context.get(); }
protected:
    GraphicsLayerRenderer(PassRefPtr<GraphicsContext3D> context);

    void updateTransform(GtkLayer* layer);
    void renderLayers(GtkLayer* layer);
    void renderLayer(GtkLayer* layer);
private:
    TransformationMatrix orthoMatrix(float left, float right, float bottom, float top, float nearZ, float farZ);

    RefPtr<GraphicsContext3D> m_context;

    GraphicsLayer* m_rootLayer;

    OwnPtr<LayerTextureManager> m_textureManager;
    OwnPtr<LayerShaderManager> m_shaderManager;
    OwnPtr<LayerQuadManager> m_quadManager;

    TransformationMatrix m_projectionMatrix;
    OwnPtr<LayerTransformation> m_backingMatrix;
    OwnPtr<LayerTransformation> m_rootMatrix;

    bool m_bDepthTest;
    unsigned m_backingTextureId;
    unsigned m_rootTextureId;
    unsigned int m_frontID;
    unsigned int m_backID;
    unsigned m_frontTextureID;
    unsigned m_backTextureID;
    bool m_isContentsViewCompositingMode;
    IntSize m_UIViewSize;
    IntPoint m_UIViewPosition;
    IntRect m_contentsViewRect;    
    static StereoScopicMode s_stereoScopicMode;
    static IntSize s_stereoScopicSize;
#if ENABLE(TVGG_THREAD_MODEL)
    GraphicsLayer* m_contentsViewGraphicsLayer;
#endif
#if ENABLE(TVGG_TILED_BACKING_STORE)
    TiledBackingStoreInfo m_tiledBackingStoreInfo;
#endif
#ifdef TVGG_CHECK_FPS
    int m_n;
    float m_elapsed1;
    float m_elapsed2;
    float m_elapsed3;
    float m_elapsed4;
    float m_elapsed5;
    float m_elapsed6;
    float m_elapsed7;
#endif
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // GraphicsLayerRenderer_h
