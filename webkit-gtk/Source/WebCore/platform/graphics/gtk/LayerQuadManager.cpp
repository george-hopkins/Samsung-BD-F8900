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

#include "LayerQuadManager.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerQuadManager> LayerQuadManager::create(GraphicsContext3D* context)
{
    return adoptPtr(new LayerQuadManager(context));
}

LayerQuadManager::LayerQuadManager(GraphicsContext3D* context)
	: LayerManager(context)
	, m_vertexVbo(0)
	, m_indexVbo(0)
{
	m_bInitialized = initialize();
}

LayerQuadManager::~LayerQuadManager()
{
	release();
}

bool LayerQuadManager::initialize()
{
	float vertices[] = { -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,
						 -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,
						 0.5f, -0.5f, 0.0f, 1.0f,  0.0f,
						 0.5f,  0.5f, 0.0f, 1.0f,  1.0f };
	uint16_t indices[] = { 0, 1, 2, 0, 2, 3,
						   0, 1, 2, 3};

	m_vertexVbo = m_context->createBuffer();
	m_indexVbo = m_context->createBuffer();

	m_context->bindBuffer(GraphicsContext3D::ARRAY_BUFFER, m_vertexVbo);
	m_context->bufferData(GraphicsContext3D::ARRAY_BUFFER, sizeof(vertices), vertices, GraphicsContext3D::STATIC_DRAW);

	m_context->bindBuffer(GraphicsContext3D::ELEMENT_ARRAY_BUFFER, m_indexVbo);
	m_context->bufferData(GraphicsContext3D::ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GraphicsContext3D::STATIC_DRAW);

	return true;
}

void LayerQuadManager::release()
{
    m_context->deleteBuffer(m_vertexVbo);
    m_context->deleteBuffer(m_indexVbo);
}

void LayerQuadManager::readyToRender()
{
	m_context->bindBuffer(GraphicsContext3D::ARRAY_BUFFER, m_vertexVbo);
	m_context->bindBuffer(GraphicsContext3D::ELEMENT_ARRAY_BUFFER, m_indexVbo);

	unsigned offset = 0;
	m_context->vertexAttribPointer(LayerManager::s_positionAttribLocation, 3, GraphicsContext3D::FLOAT, false, 5 * sizeof(float), offset);
	offset += 3 * sizeof(float);
	m_context->vertexAttribPointer(LayerManager::s_texCoordAttribLocation, 2, GraphicsContext3D::FLOAT, false, 5 * sizeof(float), offset);

	m_context->enableVertexAttribArray(LayerManager::s_positionAttribLocation);
	m_context->enableVertexAttribArray(LayerManager::s_texCoordAttribLocation);
}

void LayerQuadManager::renderQuad()
{
	m_context->drawElements(GraphicsContext3D::TRIANGLES, 6, GraphicsContext3D::UNSIGNED_SHORT, 0);
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
