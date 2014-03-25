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

#include "LayerShaderManager.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"
#include "LayerContents.h"
#include "LayerTexShader.h"
#include "LayerWebGLShader.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerShaderManager> LayerShaderManager::create(GraphicsContext3D* context)
{
    return adoptPtr(new LayerShaderManager(context));
}

LayerShaderManager::LayerShaderManager(GraphicsContext3D* context)
	: LayerManager(context)
	, m_usedShader(NULL)
{
	m_bInitialized = initialize();
}

LayerShaderManager::~LayerShaderManager()
{
	release();
}

bool LayerShaderManager::initialize()
{
	m_texShader = LayerTexShader::create(m_context.get());
	m_webGLShader = LayerWebGLShader::create(m_context.get());
	if (!m_texShader)
		return false;
	return true;
}

void LayerShaderManager::release()
{
	m_usedShader = NULL;
	m_texShader.clear();
	m_webGLShader.clear();
}

void LayerShaderManager::use(int type)
{
	LayerShader* requiredShader = m_usedShader;
	switch (type)
	{
	case LayerContents::HTMLContentsType:
	case LayerContents::ImageContentsType:
		requiredShader = m_texShader.get();
		break;

	case LayerContents::WebGLContentsType:
        requiredShader = m_webGLShader.get();
        break;
	default:
		return;
	}

	if (requiredShader == m_usedShader)
		return;

	requiredShader->use();
	m_usedShader = requiredShader;
}

void LayerShaderManager::setMatrix(float* matrix)
{
	if (m_usedShader)
		m_usedShader->setMatrix(matrix);
}

void LayerShaderManager::setSampler(int sampler)
{
	if (m_usedShader)
		m_usedShader->setSampler(sampler);
}

void LayerShaderManager::setAlpha(float alpha)
{
	if (m_usedShader)
		m_usedShader->setAlpha(alpha);
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
