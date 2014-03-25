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

#include "LayerTransformation.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GtkLayer.h"
#include "FloatSize.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerTransformation> LayerTransformation::create(GtkLayer* owner)
{
	return adoptPtr(new LayerTransformation(owner));
}

LayerTransformation::LayerTransformation(GtkLayer* owner)
	: m_owner(owner)
	, m_anchorPoint(0.5f, 0.5f, 0.0f)
	, m_contentsAnchorPoint(0.5f, 0.5f)
{
}

LayerTransformation::~LayerTransformation()
{
}

void LayerTransformation::setPosition(const FloatPoint& position)
{
	m_position = position;
}

void LayerTransformation::setAnchorPoint(const FloatPoint3D& anchorPoint)
{
	m_anchorPoint = anchorPoint;
}

void LayerTransformation::setSize(const FloatSize& size)
{
	m_size = size;
}

void LayerTransformation::setTransform(const TransformationMatrix& transform)
{
	m_transform = transform;
}

void LayerTransformation::setChildrenTransform(const TransformationMatrix& transform)
{
	m_childrenTransform = transform;
}

void LayerTransformation::setContentsPosition(const IntPoint& position)
{
	m_contentsPosition = position;
}

void LayerTransformation::setContentsSize(const IntSize& size)
{
	m_contentsSize = size;
}

void LayerTransformation::updateLayerMatrix()
{
	GtkLayer* parent = m_owner ? m_owner->parent() : NULL;

    float centerOffsetX = (0.5 - m_anchorPoint.x()) * m_size.width();
    float centerOffsetY = (0.5 - m_anchorPoint.y()) * m_size.height();

	if (parent)
        m_layerMatrix = parent->getLayerMatrix();
    else if(m_owner)
        m_layerMatrix = m_owner->getLayerMatrix();
    else
        m_layerMatrix = TransformationMatrix();

#if 0
    printf(" My name is %d\n", (int) this );
    printf("************** setTransform is called\n");
    printf("===============================================\n");
    printf("Transform Matrix\n");
    printf("%f \t %f \t %f \t %f\n", m_layerMatrix.m11(), m_layerMatrix.m12(), m_layerMatrix.m13(), m_layerMatrix.m14());
    printf("%f \t %f \t %f \t %f\n", m_layerMatrix.m21(), m_layerMatrix.m22(), m_layerMatrix.m23(), m_layerMatrix.m24());
    printf("%f \t %f \t %f \t %f\n", m_layerMatrix.m31(), m_layerMatrix.m32(), m_layerMatrix.m33(), m_layerMatrix.m34());
    printf("%f \t %f \t %f \t %f\n", m_layerMatrix.m41(), m_layerMatrix.m42(), m_layerMatrix.m43(), m_layerMatrix.m44());
#endif
    m_layerMatrix.translate3d(m_position.x() + m_anchorPoint.x() * m_size.width(),
							  m_position.y() + m_anchorPoint.y() * m_size.height(),
							  m_anchorPoint.z());

    m_layerMatrix.multiply(m_transform);

    m_layerMatrix.translate3d(centerOffsetX, centerOffsetY, -m_anchorPoint.z());

    m_contentsMatrix = m_layerMatrix;
    m_contentsMatrix.scale3d(m_size.width(), m_size.height(), 1);


    if (!m_owner || !m_owner->preserves3D()) {
    	m_layerMatrix.setM13(0);
    	m_layerMatrix.setM23(0);
    	m_layerMatrix.setM31(0);
    	m_layerMatrix.setM32(0);
    	m_layerMatrix.setM33(1);
    	m_layerMatrix.setM34(0);
    	m_layerMatrix.setM43(0);
	}
    m_layerMatrix.multiply(m_childrenTransform);
    m_layerMatrix.translate3d(-m_size.width() * 0.5, -m_size.height() * 0.5, 0);
}

void LayerTransformation::updateContentsMatrix()
{
	m_contentsMatrix = m_layerMatrix;
	m_contentsMatrix.translate3d(m_contentsPosition.x() + (m_contentsSize.width() * 0.5),
							 m_contentsPosition.y() + (m_contentsSize.height() * 0.5),
								 0);

	m_contentsMatrix.scale3d(m_contentsSize.width(), m_contentsSize.height(), 1);
}

float* LayerTransformation::getRenderMatrix(const TransformationMatrix& projection)
{
	m_renderMatrix = projection;
	m_renderMatrix.multiply(m_contentsMatrix);
	makeGLMatrix();
	return m_glMatrix;
}

void LayerTransformation::makeGLMatrix()
{
	m_glMatrix[0] = m_renderMatrix.m11();
    m_glMatrix[1] = m_renderMatrix.m12();
    m_glMatrix[2] = m_renderMatrix.m13();
    m_glMatrix[3] = m_renderMatrix.m14();
    m_glMatrix[4] = m_renderMatrix.m21();
    m_glMatrix[5] = m_renderMatrix.m22();
    m_glMatrix[6] = m_renderMatrix.m23();
    m_glMatrix[7] = m_renderMatrix.m24();
    m_glMatrix[8] = m_renderMatrix.m31();
    m_glMatrix[9] = m_renderMatrix.m32();
    m_glMatrix[10] = m_renderMatrix.m33();
    m_glMatrix[11] = m_renderMatrix.m34();
    m_glMatrix[12] = m_renderMatrix.m41();
    m_glMatrix[13] = m_renderMatrix.m42();
    m_glMatrix[14] = m_renderMatrix.m43();
    m_glMatrix[15] = m_renderMatrix.m44();
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

