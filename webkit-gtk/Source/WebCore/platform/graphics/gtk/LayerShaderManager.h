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

#ifndef LayerShaderManager_h
#define LayerShaderManager_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerManager.h"
#include "IntSize.h"
#include "IntRect.h"

#include <wtf/PassOwnPtr.h>

namespace WebCore {

class LayerShader;
class LayerTexShader;
class LayerWebGLShader;

class LayerShaderManager : public LayerManager
{
public:
    static PassOwnPtr<LayerShaderManager> create(GraphicsContext3D* context);

    virtual ~LayerShaderManager();

    virtual bool initialize();

    virtual void release();

    void use(int type);

    void setMatrix(float* matrix);
    void setSampler(int sampler);
    void setAlpha(float alpha);

private:
    LayerShaderManager(GraphicsContext3D* context);

	OwnPtr<LayerTexShader> m_texShader;
	OwnPtr<LayerWebGLShader> m_webGLShader;

	LayerShader* m_usedShader;
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerShaderManager_h
