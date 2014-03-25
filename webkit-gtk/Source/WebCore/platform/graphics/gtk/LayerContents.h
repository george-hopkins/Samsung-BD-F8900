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

#ifndef LayerContents_h
#define LayerContents_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GtkLayer.h"
#include <stdio.h>

namespace WebCore {

class LayerTextureManager;

class LayerContents
{
public:
	enum Type
	{
		HTMLContentsType,
		ImageContentsType,
		VideoContentsType,
		ColorContentsType,
        WebGLContentsType	
	};

    LayerContents(GtkLayer* owner);
    virtual ~LayerContents() {}

    unsigned getTexture() const { return m_textureId; }
    void setTexture(unsigned textureId) { m_textureId = textureId;printf("---%d\n", m_textureId); }

    virtual void releaseResources(LayerTextureManager* manager);

    virtual LayerContents::Type type() const = 0;

    virtual void updateContents(LayerTextureManager* manager, const FloatSize& layerSize, const FloatRect& dirtyRect) = 0;
	virtual void updateTexture(LayerTextureManager*) = 0;

protected:
    GtkLayer* m_owner;

    unsigned m_textureId;
    IntSize m_textureSize;
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerContents_h
