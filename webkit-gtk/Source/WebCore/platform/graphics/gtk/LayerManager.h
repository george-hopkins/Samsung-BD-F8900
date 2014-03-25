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

#ifndef LayerManager_h
#define LayerManager_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsContext3D.h"

#include <wtf/RefCounted.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class LayerManager : public RefCounted<LayerManager>
{
public:
    LayerManager(GraphicsContext3D* context)
    : m_context(context)
    , m_bInitialized(false)
    {}

    virtual ~LayerManager() {}

    virtual bool initialize() = 0;

    virtual void release() = 0;

    bool initialized() const { return m_bInitialized; }

    static const unsigned s_positionAttribLocation;
    static const unsigned s_texCoordAttribLocation;

protected:
    RefPtr<GraphicsContext3D> m_context;

    bool m_bInitialized;
};

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerManager_h
