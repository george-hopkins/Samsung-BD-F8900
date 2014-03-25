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

#ifndef LayerShader_h
#define LayerShader_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include <wtf/RefCounted.h>
#include <wtf/PassOwnPtr.h>

namespace WebCore {

class GraphicsContext3D;

class LayerShader : public RefCounted<LayerShader>
{
public:
    LayerShader(GraphicsContext3D* context, unsigned program);
    ~LayerShader();

    virtual void use();

    virtual void setMatrix(float* matrix) = 0;
    virtual void setSampler(int sampler) = 0;
    virtual void setAlpha(float alpha) = 0;

    static unsigned loadShader(GraphicsContext3D* context, unsigned type, const char* shaderSource);
    static unsigned loadProgram(GraphicsContext3D* context, const char* vertexShaderSource, const char* fragmentShaderSource);

protected:
    GraphicsContext3D* m_context;
    unsigned m_program;
};

}

#endif // #if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#endif // LayerShader_h
