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

#include "LayerTexShader.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <stdio.h>
/******************************************************************************
 Defines
******************************************************************************/

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY    0


namespace WebCore {

PassOwnPtr<LayerTexShader> LayerTexShader::create(GraphicsContext3D* context)
{
    char verLayerTexShaderString[] =
        "attribute vec4 attr_pos;     \n"
        "attribute vec2 attr_tex;     \n"
        "uniform mat4 matrix;         \n"
        "varying vec2 vTexCoord;      \n"
        "void main()                  \n"
        "{                            \n"
        "  gl_Position = matrix * attr_pos; \n"
        "  vTexCoord = attr_tex;      \n"
        "}                            \n";

    char fragmentShaderString[] =
        "precision mediump float;                            \n"
        "varying vec2 vTexCoord;                              \n"
        "uniform sampler2D texture;                          \n"
        "uniform float alpha;                                \n"
        "void main()                                         \n"
        "{                                                   \n"
		"  vec4 texColor = texture2D(texture, vTexCoord);     \n"
		"  gl_FragColor = vec4(texColor.x, texColor.y, texColor.z, texColor.w) * alpha; \n"
        "}                                                   \n";
    unsigned program = loadProgram(context, verLayerTexShaderString, fragmentShaderString);
    ASSERT(program);

    return adoptPtr(new LayerTexShader(context, program));
}

LayerTexShader::LayerTexShader(GraphicsContext3D* context, unsigned program)
    : LayerShader(context, program)
{
    m_matrixLocation = m_context->getUniformLocation(program, "matrix");
    m_samplerLocation = m_context->getUniformLocation(program, "texture");
    m_alphaLocation = m_context->getUniformLocation(program, "alpha");
}

void LayerTexShader::setMatrix(float* matrix)
{
	m_context->uniformMatrix4fv(m_matrixLocation, false, matrix, 1);
}

void LayerTexShader::setSampler(int sampler)
{
	m_context->uniform1i(m_samplerLocation, sampler);
}

void LayerTexShader::setAlpha(float alpha)
{
	m_context->uniform1f(m_alphaLocation, alpha);
}

}

#endif
