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

#include "LayerTextureManager.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"

#include <stdio.h>

namespace WebCore {

LayerTextureManager::LayerTextureManager(GraphicsContext3D* context)
	: LayerManager(context)
	, m_maxTextureSize(0)
{
}

LayerTextureManager::~LayerTextureManager()
{
	release();
}

bool LayerTextureManager::initialize()
{
	m_context->getIntegerv(GraphicsContext3D::MAX_TEXTURE_SIZE, &m_maxTextureSize);
	return true;
}

bool LayerTextureManager::checkTextureSize(const IntSize& textureSize)
{
    if (textureSize.width() > m_maxTextureSize || textureSize.height() > m_maxTextureSize)
        return false;
    return true;
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
