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

#ifndef LayerQuadManager_h
#define LayerQuadManager_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerManager.h"

#include <wtf/PassOwnPtr.h>

namespace WebCore {

class LayerQuadManager : public LayerManager
{
public:
    static PassOwnPtr<LayerQuadManager> create(GraphicsContext3D* context);

    virtual ~LayerQuadManager();

    virtual bool initialize();

    virtual void release();

    void readyToRender();

    void renderQuad();

private:
    LayerQuadManager(GraphicsContext3D* context);

    unsigned m_vertexVbo;
    unsigned m_indexVbo;
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerQuadManager_h
