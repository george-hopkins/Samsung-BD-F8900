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

#ifndef LayerTexShader_h
#define LayerTexShader_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerShader.h"

namespace WebCore {

class LayerTexShader : public LayerShader {
public:
    static PassOwnPtr<LayerTexShader> create(GraphicsContext3D* context);

    virtual void setMatrix(float* matrix);
    virtual void setSampler(int sampler);
    virtual void setAlpha(float alpha);

private:
    LayerTexShader(GraphicsContext3D* context, unsigned program);

    int m_matrixLocation;
    int m_samplerLocation;
    int m_alphaLocation;
};

}

#endif

#endif // LayerTexShader_h
