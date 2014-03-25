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

#include "LayerHTMLContents.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "LayerTextureManager.h"
#include "GraphicsContext.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerContents> LayerHTMLContents::create(GtkLayer* owner)
{
    return adoptPtr(new LayerHTMLContents(owner));
}

LayerHTMLContents::LayerHTMLContents(GtkLayer* owner)
	: LayerContents(owner)
{
}

LayerHTMLContents::~LayerHTMLContents()
{
}

void LayerHTMLContents::updateContents(LayerTextureManager* manager, const FloatSize& layerSize, const FloatRect& dirtyRect)
{
	IntRect updateRect(dirtyRect);
	IntRect boundsRect(0, 0, (int)layerSize.width(), (int)layerSize.height());

	bool useSubImageUpdate = true;

	if (layerSize != m_textureSize) {
		updateRect = boundsRect;
		useSubImageUpdate = false;
	}
	else {
		updateRect.intersect(boundsRect);
	}

    if (updateRect.isEmpty())
        return;

	void* pixels = NULL;

#if USE(CAIRO)
	cairo_status_t cairoStatus;
	cairo_surface_t* cairoSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, updateRect.width(), updateRect.height() );

	cairoStatus = cairo_surface_status(cairoSurface);
	if (cairoStatus != CAIRO_STATUS_SUCCESS) {
		printf("[%p] ContentLayerGtk::%s() - cairo surface error\n", this, __func__);
	}

	cairo_t* cairoContext = cairo_create(cairoSurface);
	cairoStatus = cairo_status(cairoContext);
	if (cairoStatus != CAIRO_STATUS_SUCCESS) {
		printf("[%p] ContentLayerGtk::%s() - cairo error\n", this, __func__);
	}

	GraphicsContext graphicsContext(cairoContext);
	graphicsContext.translate(-updateRect.x(), -updateRect.y());

	cairo_save(cairoContext);
	m_owner->paintGraphicsLayerContents(graphicsContext, updateRect);
	cairo_restore(cairoContext);

	pixels = cairo_image_surface_get_data(cairoSurface);
#else
#error "Need to implement for your platform."
#endif

	if (!m_textureId) {
		m_textureId = manager->createTexture();
		useSubImageUpdate = false;
	}

	if (useSubImageUpdate) {
		ASSERT(updateRect.width() <= m_textureSize.width());
		ASSERT(updateRect.height() <= m_textureSize.height());
		manager->updateTexture(m_textureId, updateRect, pixels);
	}
	else {
		manager->updateTexture(m_textureId, updateRect.size(), pixels);
		m_textureSize = updateRect.size();
	}

#if USE(CAIRO)
	cairo_destroy(cairoContext);
	cairo_surface_destroy(cairoSurface);
#endif
}

void LayerHTMLContents::updateTexture(LayerTextureManager* manager)
{
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
