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

#include "LayerImageContents.h"

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)

#include "Image.h"
#include "LayerTextureManager.h"

#include <stdio.h>

namespace WebCore {

PassOwnPtr<LayerContents> LayerImageContents::create(GtkLayer* owner)
{
    return adoptPtr(new LayerImageContents(owner));
}

LayerImageContents::LayerImageContents(GtkLayer* owner)
	: LayerContents(owner)
	, m_image(NULL)
{
}

LayerImageContents::~LayerImageContents()
{
}

void LayerImageContents::updateContents(LayerTextureManager* manager, const FloatSize& layerSize, const FloatRect& dirtyRect)
{
    NativeImagePtr nativeImage = m_image->nativeImageForCurrentFrame();

    void* pixels = NULL;
    int width, height;

#if USE(CAIRO)
    cairo_surface_t* img = static_cast<cairo_surface_t*>(nativeImage);

	pixels = cairo_image_surface_get_data(img);
	width = cairo_image_surface_get_width(img);
	height = cairo_image_surface_get_height(img);
#else
#error "Need to implement for your platform."
#endif

	IntRect updateRect(dirtyRect);
	IntSize imageSize(width, height);

	bool useSubImageUpdate = true;

	if (imageSize != m_textureSize) {
		updateRect = IntRect(0, 0, imageSize.width(), imageSize.height());
		useSubImageUpdate = false;
	}
	else {
		updateRect.intersect(IntRect(0, 0, imageSize.width(), imageSize.height()));
	}

	if (!m_textureId) {
		m_textureId = manager->createTexture();
		useSubImageUpdate = false;
	}

    if (updateRect.isEmpty())
        return;

	if (useSubImageUpdate) {
		ASSERT(updateRect.width() <= m_textureSize.width());
		ASSERT(updateRect.height() <= m_textureSize.height());
		manager->updateTexture(m_textureId, updateRect, pixels);
	}
	else {
		manager->updateTexture(m_textureId, updateRect.size(), pixels);
		m_textureSize = updateRect.size();
	}
}

void LayerImageContents::setImage(Image* image)
{
	if (image == m_image)
		return;

	m_image = image;
	m_owner->setNeedsDisplayInRect(m_image->rect());
}

void LayerImageContents::updateTexture(LayerTextureManager* manager)
{
}

} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)
