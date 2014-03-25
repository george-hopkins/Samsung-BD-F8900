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

#ifndef LayerTransformation_h
#define LayerTransformation_h

#include "config.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "FloatPoint.h"
#include "FloatPoint3D.h"
#include "FloatSize.h"
#include "IntPoint.h"
#include "IntSize.h"
#include "TransformationMatrix.h"

#include <wtf/PassOwnPtr.h>

namespace WebCore {

class GtkLayer;

class LayerTransformation
{
public:
    static PassOwnPtr<LayerTransformation> create(GtkLayer* owner);

    virtual ~LayerTransformation();

    void setPosition(const FloatPoint& position);
    FloatPoint position() const { return m_position; }

    void setAnchorPoint(const FloatPoint3D& anchorPoint);
    FloatPoint3D anchorPoint() const { return m_anchorPoint; }

    void setSize(const FloatSize& size);
    FloatSize size() const { return m_size; }

    void setTransform(const TransformationMatrix& transform);
    const TransformationMatrix& transform() const { return m_transform; }

    void setChildrenTransform(const TransformationMatrix& transform);
    const TransformationMatrix& childrenTransform() const { return m_childrenTransform; }

    void setContentsPosition(const IntPoint& position);
    IntPoint contentsPosition() const { return m_contentsPosition; }

    void setContentsSize(const IntSize& size);
    IntSize contentsSize() const { return m_contentsSize; }

    void updateLayerMatrix();
    void updateContentsMatrix();

    float* getRenderMatrix(const TransformationMatrix& projection);
    const TransformationMatrix& getLayerMatrix() { return m_layerMatrix; }
    const TransformationMatrix& getContentsMatrix() { return m_contentsMatrix; }

protected:
    LayerTransformation(GtkLayer* owner);

private:
    void makeGLMatrix();

    GtkLayer* m_owner;

    FloatPoint m_position;
    FloatPoint3D m_anchorPoint;
    FloatSize m_size;

    IntPoint m_contentsPosition;
    IntPoint m_contentsAnchorPoint;
    IntSize m_contentsSize;

    TransformationMatrix m_transform;
    TransformationMatrix m_childrenTransform;

    TransformationMatrix m_layerMatrix;
    TransformationMatrix m_contentsMatrix;
    TransformationMatrix m_renderMatrix;
    float m_glMatrix[16];
};

}

#endif // USE(ACCELERATED_COMPOSITING)

#endif // LayerTransformation_h
