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

#ifndef LayerTextureManager_h
#define LayerTextureManager_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerManager.h"
#include "IntSize.h"
#include "IntRect.h"

namespace WebCore {

class LayerTextureManager : public LayerManager
{
public:
	LayerTextureManager(GraphicsContext3D* context);
    virtual ~LayerTextureManager();

    virtual bool initialize();

    virtual void release() {}

    virtual unsigned createTexture() = 0;
    virtual unsigned createTextureWithUmpID(unsigned int id) { return 0; }
    virtual void deleteTexture(unsigned id) = 0;

    virtual void updateTexture(unsigned id, const IntSize& textureSize, void* pixels) = 0;
    virtual void updateTexture(unsigned id, const IntRect& updateRect, void* pixels) = 0;

    virtual void useTexture(unsigned id) = 0;

    virtual bool hasExtensions() const = 0;
	
	virtual bool getBuffer(unsigned id, void** ptr, int* width, int* height) { return false; }

protected:
    bool checkTextureSize(const IntSize& textureSize);

private:
    int m_maxTextureSize;
};

} // namespace WebCore

#endif

#endif // LayerTextureManager_h
