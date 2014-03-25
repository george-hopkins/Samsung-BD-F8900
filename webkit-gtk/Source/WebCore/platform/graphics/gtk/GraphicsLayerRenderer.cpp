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

#include "GraphicsLayerRenderer.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "GraphicsContext3D.h"
#include "GtkLayer.h"
#include "LayerContents.h"
#include "LayerTextureManagerImpl.h"
#include "LayerTextureManagerTVGG.h"
#include "LayerShaderManager.h"
#include "LayerQuadManager.h"
#include "XEGLImageWrapper.h"

#include <gdk/gdkx.h>
#include <stdio.h>

#if USE(SDAL)
#include "SdCommon.h"
#include "SdGfx.h"
#endif

#if ENABLE(TVGG_BROWSER) && ENABLE(SEF)
#include "MMManager.h"
#endif

namespace WebCore {

static inline bool compareLayerZ(const GtkLayer* a, const GtkLayer* b)
{
    const TransformationMatrix& transformA = a->getContentsMatrix();
    const TransformationMatrix& transformB = b->getContentsMatrix();

    return transformA.m43() < transformB.m43();
}

PassRefPtr<GraphicsLayerRenderer> GraphicsLayerRenderer::create(PassRefPtr<GraphicsContext3D> context)
{
    return adoptRef(new GraphicsLayerRenderer(context));
}

StereoScopicMode GraphicsLayerRenderer::s_stereoScopicMode = StereoScopicNone;
IntSize GraphicsLayerRenderer::s_stereoScopicSize = IntSize();

GraphicsLayerRenderer::GraphicsLayerRenderer(PassRefPtr<GraphicsContext3D> context)
    : m_context(context)
    , m_bDepthTest(false)
    , m_backingTextureId(0)
    , m_rootTextureId(0)
    , m_frontID(0)
    , m_backID(0)
    , m_isContentsViewCompositingMode(false)
#if ENABLE(TVGG_THREAD_MODEL)
    , m_contentsViewGraphicsLayer(0)
#endif
#ifdef TVGG_CHECK_FPS
    , m_n(0)
    , m_elapsed1(0)
    , m_elapsed2(0)
    , m_elapsed3(0)
    , m_elapsed4(0)
    , m_elapsed5(0)
    , m_elapsed6(0)
    , m_elapsed7(0)
#endif
{
}

GraphicsLayerRenderer::~GraphicsLayerRenderer()
{
    if (m_backingTextureId) {
        m_textureManager->deleteTexture(m_backingTextureId);
        m_backingTextureId = 0;
    }
    if (m_rootTextureId) {
        m_textureManager->deleteTexture(m_rootTextureId);
        m_rootTextureId = 0;
    }
    m_textureManager.release();
    m_textureManager.clear();
    m_shaderManager.clear();
    m_quadManager.clear();
    m_backingMatrix.clear();
    m_rootMatrix.clear();

    printf("[%p] GraphicsLayerRenderer::%s()\n", this, __func__);
}

bool GraphicsLayerRenderer::initialize(bool bUseExtensionsForTexture)
{
#if 0
    if (bUseExtensionsForTexture == true) {
        Extensions3D* extensions = NULL;//m_context->getExtensions();
        if (extensions) {
            if (extensions->supports("EGL_KHR_image") &&
                extensions->supports("EGL_SEC_image_map") &&
                extensions->supports("GL_OES_EGL_image")) {
                m_textureManager = LayerTextureManagerSEC::create(m_context.get());
            }
            else {
                m_textureManager = LayerTextureManagerImpl::create(m_context.get());
            }
        }
    }
    else {
        m_textureManager = LayerTextureManagerImpl::create(m_context.get());
    }
#else
    //m_textureManager = LayerTextureManagerImpl::create(m_context.get());
    m_textureManager = LayerTextureManagerTVGG::create(m_context.get());

    //m_backingTextureId = m_textureManager->createTexture();

#endif
    m_shaderManager = LayerShaderManager::create(m_context.get());
    m_quadManager = LayerQuadManager::create(m_context.get());

    if (!m_textureManager->initialized()||!m_shaderManager->initialized()||!m_quadManager->initialized())
        return false;

    m_context->clearColor(0.0, 0.0, 0.0, 0.0);
    m_context->clearDepth(1.0);
    m_context->clearStencil(0);

    m_context->enable(GraphicsContext3D::BLEND);
    m_context->blendFunc(GraphicsContext3D::ONE, GraphicsContext3D::ONE_MINUS_SRC_ALPHA);

    m_context->disable(GraphicsContext3D::DEPTH_TEST);
    m_context->depthFunc(GraphicsContext3D::LEQUAL);

    m_context->disable(GraphicsContext3D::CULL_FACE);
    m_context->cullFace(GraphicsContext3D::BACK);
    m_context->frontFace(GraphicsContext3D::CW);

    m_quadManager->readyToRender();

    return true;
}

void GraphicsLayerRenderer::initializeBackingStoreBuffer(const IntSize& size)
{
    if (m_backingTextureId)
        textureManager()->deleteTexture(m_backingTextureId);

    m_backingTextureId = textureManager()->createTexture();
    textureManager()->updateTexture(m_backingTextureId, size, 0);
}

void GraphicsLayerRenderer::initializeRootBuffer(const IntSize& size)
{
    if (m_rootTextureId)
        textureManager()->deleteTexture(m_rootTextureId);

    m_rootTextureId = textureManager()->createTexture();
    textureManager()->updateTexture(m_rootTextureId, size, 0);
}

void* GraphicsLayerRenderer::getRootBuffer(IntSize* size)
{
    if (!m_rootTextureId) {
        return 0;
    }

    int width = 0, height = 0;
    void* ptr = 0;

    bool result = textureManager()->getBuffer(m_rootTextureId, &ptr, &width, &height);

    size->setWidth(width);
    size->setHeight(height);

    return result ? ptr : 0;
}

void* GraphicsLayerRenderer::getBackingStoreBuffer(IntSize* size)
{
    if (!m_backingTextureId)
        return 0;

    int width = 0, height = 0;
    void* ptr = NULL;

    bool result = textureManager()->getBuffer(m_backingTextureId, &ptr, &width, &height);

    size->setWidth(width);
    size->setHeight(height);

    return result ? ptr : NULL;
}

void GraphicsLayerRenderer::finish()
{
    m_context->finish();
}

#if ENABLE(TVGG_TILED_BACKING_STORE)
void GraphicsLayerRenderer::setTiledBackingStoreInfo(const TiledBackingStoreInfo& info)
{
    m_tiledBackingStoreInfo = info;
}
#endif

void GraphicsLayerRenderer::renderQuadTexture(unsigned textureId, const IntRect& rect, const LayerContents::Type type)
{
    OwnPtr<LayerTransformation> matrix = LayerTransformation::create(0);

    if (!m_shaderManager.get() || !m_textureManager.get() || !m_quadManager.get()) {
        fprintf(stderr, "%s %d : renderer manager is not created.\n", __func__, __LINE__);
        return;
    }

    matrix->setPosition(rect.location());
    matrix->setContentsSize(rect.size());
    matrix->setSize(FloatSize(rect.width(), rect.height()));
    matrix->updateLayerMatrix();
    matrix->updateContentsMatrix();

    m_shaderManager->use(type);
    m_shaderManager->setMatrix(matrix->getRenderMatrix(m_projectionMatrix));
    m_shaderManager->setSampler(0);
    m_shaderManager->setAlpha(1);

    m_textureManager->useTexture(textureId);
    m_quadManager->renderQuad();
}

void GraphicsLayerRenderer::render(int w, int h)
{
    if (!w || !h)
        return;

    IntRect screenRect(0, 0, w, h);

    // resize screen rect when StereoScopic is enabled.
    switch (GraphicsLayerRenderer::s_stereoScopicMode) {
    case StereoScopicSBS: screenRect.setWidth(w / 2); break;
    case StereoScopicTNB: screenRect.setHeight(h / 2); screenRect.setY(h / 2); break;
    default:
        if (!s_stereoScopicSize.isEmpty()) {
            screenRect.setSize(GraphicsLayerRenderer::s_stereoScopicSize);
            screenRect.setY(h - screenRect.height());
        }
        break;
    }

#ifdef TVGG_CHECK_FPS
    static struct timezone tz;
    struct timeval t0, t1;
    float dt = 0;
    gettimeofday(&t0, &tz);
    ++m_n;
#endif
    if (m_rootLayer) {
        const Vector<GraphicsLayer*>& layers = m_rootLayer->children();
        for (size_t i = 0; i < layers.size(); ++i)
            updateTransform((GtkLayer*)layers[i]->platformLayer());
    }
#if ENABLE(TVGG_THREAD_MODEL)
    if (m_contentsViewGraphicsLayer) {
        const Vector<GraphicsLayer*>& contentslayers = m_contentsViewGraphicsLayer->children();
        for (size_t i = 0; i < contentslayers.size(); ++i)
             updateTransform((GtkLayer*)contentslayers[i]->platformLayer());
    }
#endif

#ifdef TVGG_CHECK_FPS
    gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed1 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[0] %4d %.3f\n", m_n, m_elapsed1 / 100);
        m_elapsed1 = 0;
    }
#endif
#if USE(SDAL)
    XEGLCacheFlush((unsigned char*)(1), 0, 0, 0, 0);
#endif
    m_context->viewport(screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height());
    m_context->enable(GraphicsContext3D::SCISSOR_TEST);
    m_context->scissor(screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height());

#ifdef TVGG_CHECK_FPS
    t0 = t1; gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed7 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[1] %4d %.3f\n", m_n, m_elapsed7 / 100);
        m_elapsed7 = 0;
    }
#endif
    m_projectionMatrix = orthoMatrix(0, w, h, 0, -1000, 1000);
    m_context->clearColor(0.0, 0.0, 0.0, 0.0);
    m_context->clear(GraphicsContext3D::DEPTH_BUFFER_BIT | GraphicsContext3D::COLOR_BUFFER_BIT);

#ifdef TVGG_CHECK_FPS
    t0 = t1; gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed2 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[2] %4d %.3f\n", m_n, m_elapsed2 / 100);
        m_elapsed2 = 0;
    }
#endif
    if (m_rootTextureId) {
#if ENABLE(TVGG_TILED_BACKING_STORE)
        unsigned n = 0;
        Vector<TiledBackingStoreInfo::TileInfo> tiles = m_tiledBackingStoreInfo.tiles();
        // draw tiles.
        for (n = 0; n < tiles.size(); n++) {
            IntRect rect = tiles[n].tileRect();
            rect.move(m_contentsViewRect.x(), m_contentsViewRect.y());
            if (rect.width() < 1 || rect.height() < 1)
                continue;

            renderQuadTexture(tiles[n].textureId(), rect, LayerContents::HTMLContentsType);
        }
        // draw scrollbar
        IntRect hscrollbarRect = m_tiledBackingStoreInfo.hscrollbarRect();
        hscrollbarRect.move(m_contentsViewRect.x(), m_contentsViewRect.y());
        IntRect vscrollbarRect = m_tiledBackingStoreInfo.vscrollbarRect();
        vscrollbarRect.move(m_contentsViewRect.x(), m_contentsViewRect.y());

        if (!hscrollbarRect.isEmpty())
            renderQuadTexture(m_tiledBackingStoreInfo.hscrollbarTextureId(), hscrollbarRect, LayerContents::HTMLContentsType);
        if (!vscrollbarRect.isEmpty())
            renderQuadTexture(m_tiledBackingStoreInfo.vscrollbarTextureId(), vscrollbarRect, LayerContents::HTMLContentsType);
#else
        if (m_backingTextureId && m_contentsViewRect.width() > 0 && m_contentsViewRect.height() > 0)
            renderQuadTexture(m_backingTextureId, m_contentsViewRect, LayerContents::HTMLContentsType);
#endif

#if ENABLE(TVGG_THREAD_MODEL)
        if (m_contentsViewGraphicsLayer) {
            m_context->viewport(screenRect.x(), screenRect.y() - m_contentsViewRect.y() * screenRect.height() / h, screenRect.width(), screenRect.height());

            const Vector<GraphicsLayer*>& contentslayers = m_contentsViewGraphicsLayer->children();
            for (size_t i = 0; i < contentslayers.size(); ++i)
                renderLayers((GtkLayer*)contentslayers[i]->platformLayer());

            m_context->viewport(screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height());
        }
#else
        if (m_frontID)
            renderQuadTexture(m_frontID, m_contentsViewRect, LayerContents::WebGLContentsType);
#endif

#if ENABLE(TVGG_BROWSER) && ENABLE(SEF)
        if (MMManager::getInstance()->isPipEnabled()) {
            IntRect pipRect = MMManager::getInstance()->getPipRect();
            pipRect.setX(pipRect.x() * screenRect.width() / w);
            pipRect.setY(pipRect.y() * screenRect.height() / h);
            pipRect.setWidth(pipRect.width() * screenRect.width() / w);
            pipRect.setHeight(pipRect.height() * screenRect.height() / h);

            m_context->enable(GraphicsContext3D::SCISSOR_TEST);
            m_context->scissor(pipRect.x(), h - pipRect.y() - pipRect.height(), pipRect.width(), pipRect.height());
            m_context->clear(GraphicsContext3D::COLOR_BUFFER_BIT);
            m_context->disable(GraphicsContext3D::SCISSOR_TEST);
        }
#endif
        renderQuadTexture(m_rootTextureId, IntRect(m_UIViewPosition, m_UIViewSize), LayerContents::HTMLContentsType);
    }
// end..
#ifdef TVGG_CHECK_FPS
    t0 = t1; gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed3 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[3] %4d %.3f\n", m_n, m_elapsed3 / 100);
        m_elapsed3 = 0;
    }
#endif

    if (m_rootLayer) {
        const Vector<GraphicsLayer*>& layers = m_rootLayer->children();
        for (size_t i = 0; i < layers.size(); ++i)
            renderLayers((GtkLayer*)layers[i]->platformLayer());
    }

#ifdef TVGG_CHECK_FPS
    t0 = t1; gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed4 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[4] %4d %.3f\n", m_n, m_elapsed4 / 100);
        m_elapsed4 = 0;
    }
#endif
    if (m_rootTextureId) {
        int pixels = 0;
        m_context->readPixels(0, 0, 0, 0,
                              GraphicsContext3D::RGBA,
                              GraphicsContext3D::UNSIGNED_BYTE,
                              &pixels);

#ifdef TVGG_CHECK_FPS
        t0 = t1; gettimeofday(&t1, &tz);
        dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
        m_elapsed5 += dt;
        if ((m_n % 100) == 0) {
            fprintf(stderr, "[5] %4d %.3f\n", m_n, m_elapsed5 / 100);
            m_elapsed5 = 0;
        }
#endif
        XSync(gdk_x11_display_get_xdisplay(gdk_display_get_default()), false);
        m_context->swapBuffers();
    }
    m_context->disable(GraphicsContext3D::SCISSOR_TEST);
#ifdef TVGG_CHECK_FPS
    t0 = t1; gettimeofday(&t1, &tz);
    dt = (t1.tv_sec - t0.tv_sec) * 1000 + (t1.tv_usec - t0.tv_usec) * 1e-3;
    m_elapsed6 += dt;
    if ((m_n % 100) == 0) {
        fprintf(stderr, "[6] %4d %.3f\n", m_n, m_elapsed6 / 100);
        m_elapsed6 = 0;
    }
#endif
}

void GraphicsLayerRenderer::clear()
{
    m_context->viewport(0, 0, m_UIViewSize.width(), m_UIViewSize.height());
    m_projectionMatrix = orthoMatrix(0, m_UIViewSize.width(), m_UIViewSize.height(), 0, -1000, 1000);
    m_context->clearColor(0.0, 0.0, 0.0, 0.0);
    m_context->clear(GraphicsContext3D::DEPTH_BUFFER_BIT | GraphicsContext3D::COLOR_BUFFER_BIT);
    XSync(gdk_x11_display_get_xdisplay(gdk_display_get_default()), false);
    m_context->swapBuffers();
}

void GraphicsLayerRenderer::renderToEGLImageRenderBuffer(int w, int h)
{
// 1. create FBO
// 2. create UMP
// 3. bind EGLImage to Render Buffer

    render(w, h);
}

void GraphicsLayerRenderer::updateTransform(GtkLayer* layer)
{
    layer->setRenderer(this);

    layer->updateLayerMatrix();
    layer->updateAlpha();

    if (layer->hasVisibleContents()) {
        layer->updateContentsMatrix();
        if (layer->needsToUpdateContents()) {
            layer->updateContents(m_textureManager.get());
        }
    }

    if (!layer->getOwner())
        return;

    GraphicsLayerTexture* graphicsLayer = layer->getOwner();
    const Vector<GraphicsLayer*>& layers = graphicsLayer->children();
    for (size_t i = 0; i < layers.size(); ++i) {
        updateTransform((GtkLayer*)layers[i]->platformLayer());
    }
}

void GraphicsLayerRenderer::renderLayers(GtkLayer* layer)
{
    bool mustClearDepth = false;
    if (layer->preserves3D()) {
        if (!m_bDepthTest) {
            m_context->enable(GraphicsContext3D::DEPTH_TEST);
            mustClearDepth = true;
            m_bDepthTest = true;
        }
    }

    if (layer->hasVisibleContents()) {
        renderLayer(layer);
    }

    if (!layer->getOwner())
        return;

    GraphicsLayerTexture* graphicsLayer = layer->getOwner();
    const Vector<GraphicsLayer*>& layers = graphicsLayer->children();
    if (m_bDepthTest) {
        Vector<GtkLayer*> eflLayers;
        for (size_t i = 0; i < layers.size(); ++i)
            eflLayers.append((GtkLayer*)layers[i]->platformLayer());

        for (size_t i = 0; i < eflLayers.size(); ++i)
            std::stable_sort(eflLayers.begin(), eflLayers.end(), compareLayerZ);

        for (size_t i = 0; i < eflLayers.size(); ++i)
            renderLayers(eflLayers[i]);
    }
    else {
        for (size_t i = 0; i < layers.size(); ++i) {
            renderLayers((GtkLayer*)layers[i]->platformLayer());
        }
    }

    if (mustClearDepth) {
        m_context->disable(GraphicsContext3D::DEPTH_TEST);
        m_context->clear(GraphicsContext3D::DEPTH_BUFFER_BIT);
        m_bDepthTest = false;
    }
}

void GraphicsLayerRenderer::renderLayer(GtkLayer* layer)
{
    if (layer->backfaceVisibility())
        m_context->disable(GraphicsContext3D::CULL_FACE);
    else
        m_context->enable(GraphicsContext3D::CULL_FACE);

    layer->updateTexture(m_textureManager.get());

    m_shaderManager->use(layer->contentsType());

    m_shaderManager->setMatrix(layer->getRenderMatrix(m_projectionMatrix));
    m_shaderManager->setSampler(0);
    m_shaderManager->setAlpha(layer->getAlpha());

    m_textureManager->useTexture(layer->getTexture());

    m_quadManager->renderQuad();
}

void GraphicsLayerRenderer::copyPixelDataFromFrameBufferToEvas(int x, int y, int w, int h, void* evas)
{
    m_context->makeContextCurrent();
    m_context->readPixels(0, 0, w, h,
                          GraphicsContext3D::RGBA,
                          GraphicsContext3D::UNSIGNED_BYTE,
                          evas);
}

TransformationMatrix GraphicsLayerRenderer::orthoMatrix(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    TransformationMatrix ortho;
    if (!deltaX || !deltaY || !deltaZ)
        return ortho;
    ortho.setM11(2.0f / deltaX);
    ortho.setM41(-(right + left) / deltaX);
    ortho.setM22(2.0f / deltaY);
    ortho.setM42(-(top + bottom) / deltaY);
    ortho.setM33(-2.0f / deltaZ);
    ortho.setM43(-(nearZ + farZ) / deltaZ);
    return ortho;
}

void GraphicsLayerRenderer::setUmpID(unsigned int frontID, unsigned int backID) {
    m_frontID = frontID; m_backID = backID;
    //m_frontTextureID = textureManager()->createTextureWithUmpID(frontID);
    //m_backTextureID = textureManager()->createTextureWithUmpID(backID);
}

void GraphicsLayerRenderer::swapUmpID() { 
    unsigned int tmp = m_frontID; m_frontID = m_backID; m_backID = tmp; 
    unsigned tmpTexture = m_frontTextureID; m_frontTextureID = m_backTextureID; m_backTextureID = tmpTexture; 
}
} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
