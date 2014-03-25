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

#include "GtkLayer.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "GraphicsLayerTexture.h"
#include "LayerTransformation.h"
#include "LayerContents.h"
#include "LayerHTMLContents.h"
#include "LayerImageContents.h"
#include "LayerWebGLContents.h"
#include "GraphicsContext3D.h"
#include "GraphicsLayerRenderer.h"

#include <stdio.h>

namespace WebCore {

PassRefPtr<GtkLayer> GtkLayer::create(GraphicsLayerTexture* owner)
{
	return adoptRef(new GtkLayer(owner));
}

GtkLayer::GtkLayer(GraphicsLayerTexture* owner)
	: m_owner(owner)
	#if 0
	, m_layerTransformation(NULL)
	, m_layerContents(NULL)
	#endif
	, m_layerAlpha(1.0f)
{
	m_layerTransformation = LayerTransformation::create(this);
}

GtkLayer::~GtkLayer()
{
	releaseResources();

    if(m_layerTransformation.get())
        m_layerTransformation.clear();

    if(m_layerContents.get())
        m_layerContents.clear();
}

void GtkLayer::setRenderer(GraphicsLayerRenderer* renderer)
{
	if (renderer == m_renderer)
		return;

    if (m_renderer)
        releaseResources();

    m_renderer = renderer;
}

GtkLayer* GtkLayer::parent()
{
	GraphicsLayerTexture* parent = static_cast<GraphicsLayerTexture*>(m_owner->parent());
	if (parent) {
		return (GtkLayer*)parent->platformLayer();
	}
	return NULL;
}

void GtkLayer::setPosition(const FloatPoint& position)
{
	m_layerTransformation->setPosition(position);
	notifySyncRequired();
}

void GtkLayer::setAnchorPoint(const FloatPoint3D& anchorPoint)
{
	m_layerTransformation->setAnchorPoint(anchorPoint);
	notifySyncRequired();
}

void GtkLayer::setSize(const FloatSize& size)
{
	m_layerTransformation->setSize(size);
	notifySyncRequired();
}

FloatSize GtkLayer::size() const
{
	return m_layerTransformation->contentsSize();
}

void GtkLayer::setTransform(const TransformationMatrix& transform)
{
	m_layerTransformation->setTransform(transform);
	notifySyncRequired();
}

void GtkLayer::setChildrenTransform(const TransformationMatrix& transform)
{
	m_layerTransformation->setChildrenTransform(transform);
	notifySyncRequired();
}

void GtkLayer::setContentsRect(const IntRect& rect)
{
	if (!m_layerContents.get())
		return;

	m_layerTransformation->setContentsPosition(rect.location());
	m_layerTransformation->setContentsSize(rect.size());
	notifySyncRequired();
}

void GtkLayer::setDrawsContent(bool b)
{
	if (b) {
		if (!m_layerContents.get() || m_layerContents->type() != LayerContents::HTMLContentsType) {
			m_layerContents = LayerHTMLContents::create(this);
		}

		if (!m_dirtyRect.isEmpty()) {
			notifySyncRequired();
		}
	}
}

void GtkLayer::setContentsToImage(Image* image)
{
	if (image) {
		if (!m_layerContents.get() || m_layerContents->type() != LayerContents::ImageContentsType) {
			m_layerContents = LayerImageContents::create(this);
		}
		static_cast<LayerImageContents*>(m_layerContents.get())->setImage(image);
	}
	else {
        m_layerContents.clear();
	}
}

void GtkLayer::setContentsToMedia(PlatformLayer* layer)
{
}

void GtkLayer::setContentsToCanvas(GraphicsContext3D* context)
{
    if (!m_layerContents.get() || m_layerContents->type() != LayerContents::WebGLContentsType) {
         m_layerContents = LayerWebGLContents::create(this); 
         static_cast<LayerWebGLContents*>(m_layerContents.get())->setContext(context);
    } else
         m_layerContents.clear();


//    m_owner->setDrawsContent(true);

//    static_cast<LayerWebGLContents*>(m_layerContents.get())->setTexture(layer->getTexture());
}

void GtkLayer::setContentsToColor(const Color& color)
{
}

void GtkLayer::setNeedsDisplay()
{
	m_dirtyRect.setSize(m_layerTransformation->size());

//	if (m_owner->drawsContent()) {
		notifySyncRequired();
//	}
}

void GtkLayer::setNeedsDisplayInRect(const FloatRect& rect)
{
	m_dirtyRect.unite(rect);

//	if (m_owner->drawsContent()) {
		notifySyncRequired();
//	}
}

void GtkLayer::setContentsNeedsDisplay()
{
	m_dirtyRect.unite(IntRect(m_layerTransformation->contentsPosition(),
							  m_layerTransformation->contentsSize()));

//	if (m_owner->drawsContent()) {
		notifySyncRequired();
//	}
}

void GtkLayer::updateLayerMatrix()
{
	m_layerTransformation->updateLayerMatrix();
}

void GtkLayer::updateContentsMatrix()
{
	switch (m_layerContents->type())
	{
	case LayerContents::ImageContentsType:
		m_layerTransformation->updateContentsMatrix();
		break;

	default:
		break;
	}
}

void GtkLayer::updateAlpha()
{
	GtkLayer* parentLayer = parent();
	if (parentLayer) {
		m_layerAlpha = m_owner->opacity() * parentLayer->getAlpha();
	}
	else {
		m_layerAlpha = m_owner->opacity();
	}
}

void GtkLayer::updateContents(LayerTextureManager* manager)
{
	m_layerContents->updateContents(manager, m_layerTransformation->size(), m_dirtyRect);
	m_dirtyRect = FloatRect();
}

void GtkLayer::updateTexture(LayerTextureManager* manager)
{
    if (m_layerContents)
         m_layerContents->updateTexture(manager);
}

int GtkLayer::contentsType()
{
	if (m_layerContents.get())
		return m_layerContents->type();

	return -1;
}

unsigned GtkLayer::getTexture()
{
	if (!m_layerContents.get())
		return 0;

	return m_layerContents->getTexture();
}

void GtkLayer::setTexture(unsigned textureId)
{
	if (!m_layerContents.get())
		return;

	m_layerContents->setTexture(textureId);
}

float* GtkLayer::getRenderMatrix(const TransformationMatrix& projection)
{
	return m_layerTransformation->getRenderMatrix(projection);
}

const TransformationMatrix& GtkLayer::getLayerMatrix() const
{
	return m_layerTransformation->getLayerMatrix();
}

const TransformationMatrix& GtkLayer::getContentsMatrix() const
{
	return m_layerTransformation->getContentsMatrix();
}

void GtkLayer::notifySyncRequired()
{
	if (m_owner->client()) {
		m_owner->client()->notifySyncRequired(m_owner);
	}
}

void GtkLayer::releaseResources()
{
	if (m_layerContents.get() && m_renderer)
		m_layerContents->releaseResources(m_renderer->textureManager());
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
