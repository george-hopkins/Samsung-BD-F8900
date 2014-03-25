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

#include "LayerTextureManagerTVGG.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "ump/ump.h"
#include "GraphicsContext3D.h"
#include "GraphicsLayerRenderer.h"
#include "UIViewWindow.h"

#include <GtkVersioning.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <XEGLImageWrapper.h>
#include <cairo.h>
#include <cairo/cairo-xlib.h>
#include <gdk/gdkx.h>
#include <stdio.h>

#if USE(SDAL)
#include "SdCommon.h"
#include "SdGfx.h"
#endif
#define TVGG_BYTES_PER_PIXEL 4

using namespace WebCore;

extern "C" {
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC glEGLImageTargetRenderbufferStorageOES;

EGLImageUMP* createEGLImage(int width, int height, unsigned long window)
{
    EGLImageUMP* eglUmp = new EGLImageUMP;

    if (!width || !height)
        return 0;

    eglUmp->width = width + (width % 2);
    eglUmp->height = height;

    Display* display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    eglUmp->eglDisplay = eglGetDisplay(display);

    eglUmp->pixmap = (void*)XCreatePixmap(display,
                             window,
                             width, height,
                             TVGG_BYTES_PER_PIXEL * 8);

    Window rootWindow;
    int pixmapX, pixmapY;
    unsigned int pixmapWidth, pixmapHeight;
    unsigned int pixmapBorderWidth;
    unsigned int pixmapDepth;

    XGetGeometry((Display*)display, (Drawable)eglUmp->pixmap, &rootWindow, &pixmapX, &pixmapY, &pixmapWidth, &pixmapHeight, &pixmapBorderWidth, &pixmapDepth);

    if (!pixmapWidth || !pixmapHeight || !pixmapDepth)
        return 0;

    XFlush(display);
    int result = XCreateEGLImage(&eglUmp->ptr, &eglUmp->uhndl, &eglUmp->eglImage, (void*)eglUmp->pixmap, display, eglUmp->eglDisplay, EGL_NO_CONTEXT, NULL);

    if (result == XEGL_FAILURE) {
        XFreePixmap(display, (Drawable)eglUmp->pixmap);
        delete eglUmp;
        return 0;
    }

    return eglUmp;
}

EGLImageUMP* createEGLImageWithUmpID(int width, int height, unsigned int id)
{
    EGLImageUMP* eglUmp = new EGLImageUMP;
#if 0

    eglUmp->width = width;
    eglUmp->height = height;

    eglUmp->uhndl = ump_handle_create_from_secure_id(id);
    eglUmp->ptr = (uint8_t*)ump_mapped_pointer_get(eglUmp->uhndl);

    eglUmp->pixmap = (bitblt_pixmap*)calloc(1, sizeof(bitblt_pixmap));
    eglUmp->pixmap->data = (unsigned short*)eglUmp->uhndl;
    eglUmp->pixmap->alpha_size = 8;
    eglUmp->pixmap->red_size = 8;
    eglUmp->pixmap->blue_size = 8;
    eglUmp->pixmap->green_size = 8;
    eglUmp->pixmap->width = width;
    eglUmp->pixmap->height = height;
    eglUmp->pixmap->bytes_per_pixel = TVGG_BYTES_PER_PIXEL;
    eglUmp->pixmap->flags = BITBLT_PIXMAP_SUPPORTS_UMP;

    eglUmp->eglDisplay = eglGetDisplay(gdk_x11_display_get_xdisplay(gdk_display_get_default()));

    eglUmp->eglImage = eglCreateImageKHR(eglUmp->eglDisplay,
                                 EGL_NO_CONTEXT,
                                 0x30b0,
                                 (EGLClientBuffer)(eglUmp->pixmap),
                                 NULL);

    EGLint eglError = eglGetError();
    if(eglError != EGL_SUCCESS) {
        printf("\n!!!! eglGetError() = %i (0x%.8x) at line %i #################\n",
                eglError, eglError, __LINE__);
    }
#endif
    return eglUmp;
}

void deleteEGLImage(EGLImageUMP* ump)
{
    if (!ump)
        return;

    if (ump->pixmap) {
        Display* display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
        XDestroyEGLImage(ump->ptr, ump->uhndl, ump->eglDisplay, ump->eglImage); 
        XFreePixmap(display, (Pixmap)ump->pixmap);
    }

    delete ump;
}

unsigned int getUMPSecureId(EGLImageUMP* ump)
{
    return ump_secure_id_get(ump->uhndl);
}

}

namespace WebCore {

PassOwnPtr<LayerTextureManager> LayerTextureManagerTVGG::create(GraphicsContext3D* context)
{
    return adoptPtr(new LayerTextureManagerTVGG(context));
}

LayerTextureManagerTVGG::LayerTextureManagerTVGG(GraphicsContext3D* context)
    : LayerTextureManager(context)
{
    m_bInitialized = initialize();
}

LayerTextureManagerTVGG::~LayerTextureManagerTVGG()
{
}

bool LayerTextureManagerTVGG::initialize()
{
    if (!LayerTextureManager::initialize())
        return false;

    return true;
}

void LayerTextureManagerTVGG::release()
{
#if 0
    for (mappedEGLImages::iterator it = mappedEGLImages.begin(); it != m_mappedEGLImages.end(); ++it)
        deleteTexture(it->first);
#endif
}

unsigned LayerTextureManagerTVGG::createTexture()
{
    unsigned textureId = m_context->createTexture();

    m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, textureId);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);

    m_mappedEGLImages.add(textureId, 0);

    return textureId;
}

unsigned LayerTextureManagerTVGG::createTextureWithUmpID(unsigned int id)
{
    unsigned textureId = m_context->createTexture();

    m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, textureId);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);

    EGLImageUMP* eglUmp = createEGLImageWithUmpID(256, 256, id);
    ASSERT(eglUmp);
    m_mappedEGLImages.add(textureId, eglUmp);

    return textureId;
}

void LayerTextureManagerTVGG::deleteTexture(unsigned id)
{
    mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
    if (it == m_mappedEGLImages.end()) {
        return;
    }

    m_context->deleteTexture(id);

    EGLImageUMP* eglUmp = (it->second);

    if (eglUmp) {
        deleteEGLImage(eglUmp);
    }

    m_mappedEGLImages.remove(it);
}

void LayerTextureManagerTVGG::updateTexture(unsigned id, const IntSize& textureSize, void* pixels)
{
    mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
    if (it == m_mappedEGLImages.end()) {
        printf("[%p] LayerTextureManagerTVGG::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
        return;
    }

    if (textureSize.isEmpty())
        return;

    EGLImageUMP* eglUmp = it->second;
    UIViewWindow* viewWindow = reinterpret_cast<UIViewWindow*>(m_context->hostWindow());
    if (!eglUmp) {
        eglUmp = createEGLImage(textureSize.width(), textureSize.height(), viewWindow->window());
        if (!eglUmp)
            return;
        ASSERT(eglUmp);
        it->second = eglUmp;
    }

    int eglImageWidth = eglUmp->width;
    int eglImageHeight = eglUmp->height;

    void* ptr = eglUmp->ptr;
    if (!ptr) {
        printf("[%p] LayerTextureManagerTVGG::%s() - mapEGLImage failed\n", this, __func__);
        return;
    }

    if (!pixels)
        return;

    cairo_surface_t* dstSurface = cairo_image_surface_create_for_data((unsigned char*)ptr,
                                                       CAIRO_FORMAT_ARGB32,
                                                       eglImageWidth, eglImageHeight, eglImageWidth * TVGG_BYTES_PER_PIXEL);

    cairo_surface_t* srcSurface = cairo_image_surface_create_for_data((unsigned char*)pixels, CAIRO_FORMAT_ARGB32, textureSize.width(), textureSize.height(), textureSize.width() * TVGG_BYTES_PER_PIXEL);

    updateEGLImage(dstSurface, srcSurface, 0, 0, eglImageWidth, eglImageHeight, eglImageWidth, eglImageHeight);

    cairo_surface_destroy(dstSurface);
    cairo_surface_destroy(srcSurface);
}

void LayerTextureManagerTVGG::updateTexture(unsigned id, const IntRect& updateRect, void* pixels)
{
    mappedEGLImages::iterator it = m_mappedEGLImages.find(id);

    if (it == m_mappedEGLImages.end()) {
        printf("[%p] LayerTextureManagerTVGG::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
        return;
    }

    EGLImageUMP* eglUmp = it->second;
    ASSERT(eglUmp);

    int eglImageWidth = eglUmp->width;
    int eglImageHeight = eglUmp->height;

    void* ptr = eglUmp->ptr;
    if (!ptr) {
        printf("[%p] LayerTextureManagerTVGG::%s() - mapEGLImage failed\n", this, __func__);
        return;
    }

    if (!pixels) {
        printf("[%p] LayerTextureManagerTVGG::%s() - pixels is null.\n", this, __func__);
        return;
    }

    UIViewWindow* viewWindow = reinterpret_cast<UIViewWindow*>(m_context->hostWindow());

    cairo_surface_t* dstSurface = cairo_image_surface_create_for_data((unsigned char*)ptr,
                                                       CAIRO_FORMAT_ARGB32,
                                                       eglImageWidth, eglImageHeight, eglImageWidth * TVGG_BYTES_PER_PIXEL);
    cairo_surface_t* srcSurface = cairo_image_surface_create_for_data((unsigned char*)pixels, CAIRO_FORMAT_ARGB32, updateRect.width(), updateRect.height(), updateRect.width() * TVGG_BYTES_PER_PIXEL);

    updateEGLImage(dstSurface, srcSurface, updateRect.x(), updateRect.y(), updateRect.width(), updateRect.height(), eglImageWidth, eglImageHeight);

    cairo_surface_destroy(dstSurface);
    cairo_surface_destroy(srcSurface);
}

void LayerTextureManagerTVGG::useTexture(unsigned id)
{
    m_context->activeTexture(GraphicsContext3D::TEXTURE0);
    m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, id);

    mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
    if (it == m_mappedEGLImages.end()) {
        return;
    }

    if (!it->second) {
        return;
    }

    if (!glEGLImageTargetTexture2DOES) {
        glEGLImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
        if (!glEGLImageTargetTexture2DOES) {
            return;
        }
    }

    glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, (GLeglImageOES)((it->second)->eglImage));
}

void LayerTextureManagerTVGG::updateEGLImage(cairo_surface_t* dstSurface, cairo_surface_t* srcSurface, int offsetX, int offsetY, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
#if 0
    struct timeval start, end;
    int render_time;
    gettimeofday(&start, NULL);
#endif
    cairo_t* cr = cairo_create(dstSurface);

    cairo_set_antialias(cr, CAIRO_ANTIALIAS_NONE); // we have to turn off the antialiasing
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

    cairo_set_source_surface(cr, srcSurface, offsetX, offsetY);
    cairo_rectangle(cr, offsetX, offsetY, srcWidth, srcHeight);
    cairo_fill(cr);

    cairo_destroy(cr);

#if 0
    gettimeofday(&end, NULL);
    render_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("- hurnjoo.lee : %s %d (update time:%dms)\n", __func__, __LINE__, render_time);
#endif
}

bool LayerTextureManagerTVGG::getBuffer(unsigned id, void** ptr, int* width, int* height)
{
    mappedEGLImages::iterator it = m_mappedEGLImages.find(id);
    if (it == m_mappedEGLImages.end()) {
        printf("[%p] LayerTextureManagerTVGG::%s() - cannot find %d texture ID\n", this, __func__, (int)id);
        return true;
    }

    EGLImageUMP* eglUmp = it->second;
    ASSERT(eglUmp);

    if (!eglUmp->ptr) {
        printf("[%p] LayerTextureManagerTVGG::%s() - mapEGLImage failed\n", this, __func__);
        return false;
    }

    if (width)
        *width = eglUmp->width;

    if (height)
        *height = eglUmp->height;

    *ptr = (void*)eglUmp->ptr;

    return true;
}


} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
