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

#include "LayerTextureManagerImpl.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerTextureManager> LayerTextureManagerImpl::create(GraphicsContext3D* context)
{
    return adoptPtr(new LayerTextureManagerImpl(context));
}

LayerTextureManagerImpl::LayerTextureManagerImpl(GraphicsContext3D* context)
	: LayerTextureManager(context)
{
	m_bInitialized = LayerTextureManager::initialize();
}

LayerTextureManagerImpl::~LayerTextureManagerImpl()
{
}


unsigned LayerTextureManagerImpl::createTexture()
{
    unsigned textureId = m_context->createTexture();

    m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, textureId);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MIN_FILTER, GraphicsContext3D::LINEAR);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_MAG_FILTER, GraphicsContext3D::LINEAR);

    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_S, GraphicsContext3D::CLAMP_TO_EDGE);
    m_context->texParameteri(GraphicsContext3D::TEXTURE_2D, GraphicsContext3D::TEXTURE_WRAP_T, GraphicsContext3D::CLAMP_TO_EDGE);

    return textureId;
}

void LayerTextureManagerImpl::deleteTexture(unsigned id)
{
	if (!id)
		return;

	m_context->deleteTexture(id);
}

void LayerTextureManagerImpl::updateTexture(unsigned id, const IntSize& textureSize, void* pixels)
{
	ASSERT(checkTextureSize(textureSize));

	m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, id);

    m_context->texImage2D(GraphicsContext3D::TEXTURE_2D, 0,
						  GraphicsContext3D::RGBA,
						  textureSize.width(), textureSize.height(), 0,
						  GraphicsContext3D::RGBA,
						  GraphicsContext3D::UNSIGNED_BYTE,
						  pixels);
}

void LayerTextureManagerImpl::updateTexture(unsigned id, const IntRect& updateRect, void* pixels)
{
	m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, id);

    m_context->texSubImage2D(GraphicsContext3D::TEXTURE_2D, 0,
							 updateRect.x(), updateRect.y(),
							 updateRect.width(), updateRect.height(),
							 GraphicsContext3D::RGBA,
							 GraphicsContext3D::UNSIGNED_BYTE,
							 pixels);
}

void LayerTextureManagerImpl::useTexture(unsigned id)
{
	m_context->activeTexture(GraphicsContext3D::TEXTURE0);
	m_context->bindTexture(GraphicsContext3D::TEXTURE_2D, id);
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
