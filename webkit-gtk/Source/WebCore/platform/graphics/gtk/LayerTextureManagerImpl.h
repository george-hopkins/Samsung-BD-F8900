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

#ifndef LayerTextureManagerImpl_h
#define LayerTextureManagerImpl_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerTextureManager.h"

#include <wtf/PassOwnPtr.h>

namespace WebCore {

class LayerTextureManagerImpl : public LayerTextureManager
{
public:
    static PassOwnPtr<LayerTextureManager> create(GraphicsContext3D* context);

    virtual ~LayerTextureManagerImpl();

    virtual unsigned createTexture();
    virtual void deleteTexture(unsigned id);

    virtual void updateTexture(unsigned id, const IntSize& textureSize, void* pixels);
    virtual void updateTexture(unsigned id, const IntRect& updateRect, void* pixels);

    virtual void useTexture(unsigned id);

    virtual bool hasExtensions() const { return false; }

protected:
    LayerTextureManagerImpl(GraphicsContext3D* context);
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerTextureManagerImpl_h
