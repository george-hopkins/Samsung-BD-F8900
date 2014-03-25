/*
 * Copyright (C) 2011 Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301 USA
 */

#ifndef GraphicsContext3DInternal_h
#define GraphicsContext3DInternal_h

#if ENABLE(TVGG_WEBGL)
#include "GraphicsContext3D.h"
#include <EGL/egl.h>
#else
typedef struct __GLXcontextRec *GLXContext;
typedef unsigned long GLXPbuffer;
typedef unsigned long GLXPixmap;
typedef unsigned char GLubyte;
typedef unsigned long Pixmap;
#endif

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "LayerTextureManagerTVGG.h" 
#endif

namespace WebCore {

class GraphicsContext3D;

class GraphicsContext3DInternal {
public:

#if ENABLE(TVGG_WEBGL)
    static PassOwnPtr<GraphicsContext3DInternal> create(GraphicsContext3D::RenderStyle, HostWindow*);
#else 
	static PassOwnPtr<GraphicsContext3DInternal> create();
#endif
    ~GraphicsContext3DInternal();

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    void swapBuffers();
    unsigned int frontBufferID() { return m_frontBufferID; }
    unsigned int backBufferID() { return m_backBufferID; }
    void setHostWindow(HostWindow* hostWindow) { m_hostWindow = hostWindow; }
    void setWebGLLayer(PassRefPtr<GtkLayer> webGLLayer) { m_webGLLayer = webGLLayer; }
    GtkLayer* webGLLayer() { return m_webGLLayer.get(); }
    HostWindow* hostWindow() { return m_hostWindow; }
#endif
	void makeContextCurrent();

private:
    friend class GraphicsContext3D;

#if ENABLE(TVGG_WEBGL)
#if USE(SDAL)
	static void initializeSDAL();
#endif
    static bool initializeOneOff();
    static GraphicsContext3DInternal* createFBOContext();
    static GraphicsContext3DInternal* createWindowContext();
    static GraphicsContext3DInternal* createXWindowContext(HostWindow*);
    static GraphicsContext3DInternal* createPixmapContext();

    GraphicsContext3DInternal(EGLContext, EGLSurface);
    GraphicsContext3DInternal(EGLDisplay, EGLContext, EGLSurface);
#else
    static GraphicsContext3DInternal* createPbufferContext();
    static GraphicsContext3DInternal* createPixmapContext();
    GraphicsContext3DInternal(GLXContext, GLXPbuffer);
    GraphicsContext3DInternal(GLXContext, Pixmap, GLXPixmap);
#endif
    static void addActiveGraphicsContext(GraphicsContext3D*);
    static void removeActiveGraphicsContext(GraphicsContext3D*);
    static void cleanupActiveContextsAtExit();

#if ENABLE(TVGG_WEBGL)
    EGLDisplay m_display;
    EGLSurface m_surface; 
    EGLContext m_context;
#else
    GLXContext m_context;
    GLXPbuffer m_pbuffer;
    Pixmap m_pixmap;
    GLXPixmap m_glxPixmap;
#endif
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) 
    EGLImageUMP* m_frontBuffer;
    EGLImageUMP* m_backBuffer;
    unsigned int m_frontBufferID;
    unsigned int m_backBufferID;
    HostWindow* m_hostWindow;
    RefPtr<GtkLayer> m_webGLLayer;
#endif
};

}

#endif // GraphicsContext3DIternal_h
