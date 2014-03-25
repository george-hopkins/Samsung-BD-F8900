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

#include "LayerShader.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"
#include "LayerManager.h"

#include <wtf/text/CString.h>

#include <stdio.h>

// hurnjoo.lee
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//////

namespace WebCore {

LayerShader::LayerShader(GraphicsContext3D* context, unsigned program)
	: m_context(context)
	, m_program(program)
{
}

LayerShader::~LayerShader()
{
	m_context->deleteProgram(m_program);
}

void LayerShader::use()
{
    m_context->useProgram(m_program);
}

unsigned LayerShader::loadShader(GraphicsContext3D* context, unsigned type, const char* shaderSource)
{
	unsigned shader = context->createShader(type);
	if (!shader)
		return 0;

	String sourceString(shaderSource);
	context->shaderSource(shader, sourceString);

	context->compileShader(shader);

	int compiled;
	context->getShaderiv(shader, GraphicsContext3D::COMPILE_STATUS, &compiled);
	if (!compiled) {
		//String infoLog = context->getShaderInfoLog(shader);
		char temp[1000];
		int n;
		glGetShaderInfoLog(shader, 1000, &n, temp);
		printf(">>%s\n", temp);
		context->deleteShader(shader);
		return 0;
	}
	return shader;
}

unsigned LayerShader::loadProgram(GraphicsContext3D* context, const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned vertexShader = loadShader(context, GraphicsContext3D::VERTEX_SHADER, vertexShaderSource);
	if (!vertexShader) {
		printf("LayerShader::%s() - Failed to create vertex shader\n", __func__);
		return 0;
	}

	unsigned fragmentShader = loadShader(context, GraphicsContext3D::FRAGMENT_SHADER, fragmentShaderSource);
	if (!fragmentShader) {
		context->deleteShader(vertexShader);
		printf("LayerShader::%s() - Failed to create fragment shader\n", __func__);
		return 0;
	}

	unsigned programObject = context->createProgram();
	if (!programObject) {
		printf("LayerShader::%s() - Failed to create shader program\n", __func__);
		return 0;
	}

	context->attachShader(programObject, vertexShader);
	context->attachShader(programObject, fragmentShader);

	// Bind the common attrib locations.
	context->bindAttribLocation(programObject, LayerManager::s_positionAttribLocation, "attr_pos");
	context->bindAttribLocation(programObject, LayerManager::s_texCoordAttribLocation, "attr_tex");

	context->linkProgram(programObject);
	int linked;
	context->getProgramiv(programObject, GraphicsContext3D::LINK_STATUS, &linked);
	if (!linked) {
		char temp[1000];
		int n;
		glGetProgramInfoLog(programObject, 1000, &n, temp); 
		printf("LayerShader::%s() - Failed to link shader program\n", __func__);
		printf("%s", temp);
		context->deleteProgram(programObject);
		return 0;
	}

	context->deleteShader(vertexShader);
	context->deleteShader(fragmentShader);
	return programObject;
}

}

#endif
