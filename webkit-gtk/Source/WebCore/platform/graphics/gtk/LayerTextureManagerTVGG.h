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

#ifndef LayerTextureManagerTVGG_h
#define LayerTextureManagerTVGG_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerTextureManager.h"

#include <wtf/PassOwnPtr.h>
#include <wtf/HashMap.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <ump/ump.h>
#include <SdGfx.h>

namespace WebCore {

class GraphicsContext3D;

class EGLImageUMP
{
public:
    EGLImageKHR eglImage;
    uint8_t* ptr; 
	void* uhndl;
	int width;
	int height;
	EGLDisplay eglDisplay;
    void* pixmap;
};

class LayerTextureManagerTVGG : public LayerTextureManager
{
public:
    static PassOwnPtr<LayerTextureManager> create(GraphicsContext3D* context);

    virtual ~LayerTextureManagerTVGG();

    virtual bool initialize();

    virtual void release();

    virtual unsigned createTexture();
    virtual unsigned createTextureWithUmpID(unsigned int id);
    virtual void deleteTexture(unsigned id);

    virtual void updateTexture(unsigned id, const IntSize& textureSize, void* pixels);
    virtual void updateTexture(unsigned id, const IntRect& updateRect, void* pixels);

    virtual void useTexture(unsigned id);

    virtual bool hasExtensions() const { return false; }
	
	bool getBuffer(unsigned id, void** ptr, int* width, int* height);

protected:
    LayerTextureManagerTVGG(GraphicsContext3D* context);

private:
    void updateEGLImage(cairo_surface_t* dstSurface, cairo_surface_t* srcSurface, int offsetX, int offsetY, int srcWidth, int srcHeight, int dstWidth, int dstHeight);

	typedef HashMap<unsigned, EGLImageUMP*> mappedEGLImages;
	mappedEGLImages m_mappedEGLImages;
};

extern "C" {
EGLImageUMP* createEGLImage(int width, int height, unsigned long window);
void deleteEGLImage(EGLImageUMP* ump);
unsigned int getUMPSecureId(EGLImageUMP* ump);
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerTextureManagerTVGG_h
