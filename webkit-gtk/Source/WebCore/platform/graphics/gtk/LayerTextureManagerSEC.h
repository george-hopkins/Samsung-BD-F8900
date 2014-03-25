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

#ifndef LayerTextureManagerSEC_h
#define LayerTextureManagerSEC_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerTextureManager.h"
#include "Extensions3DGtk.h"

#include <wtf/PassOwnPtr.h>
#include <wtf/HashMap.h>

namespace WebCore {

class GraphicsContext3D;

class LayerTextureManagerSEC : public LayerTextureManager
{
public:
    static PassOwnPtr<LayerTextureManager> create(GraphicsContext3D* context);

    virtual ~LayerTextureManagerSEC();

    virtual bool initialize();

    virtual void release();

    virtual unsigned createTexture();
    virtual void deleteTexture(unsigned id);

    virtual void updateTexture(unsigned id, const IntSize& textureSize, void* pixels);
    virtual void updateTexture(unsigned id, const IntRect& updateRect, void* pixels);

    virtual void useTexture(unsigned id);

    virtual bool hasExtensions() const { return m_extensions; }

protected:
    LayerTextureManagerSEC(GraphicsContext3D* context);

private:
    void updateEGLImage(void* dst, const void* src, int offsetX, int offsetY, int srcWidth, int srcHeight, int dstWidth, int dstHeight);

    Extensions3DGtk* m_extensions;

	typedef HashMap<unsigned, EGLImageKHR> mappedEGLImages;
	mappedEGLImages m_mappedEGLImages;
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerTextureManagerSEC_h
