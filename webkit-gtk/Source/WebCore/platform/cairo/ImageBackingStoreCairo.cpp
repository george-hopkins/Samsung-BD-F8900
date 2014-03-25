/*
 * Copyright (C) 2011, Igalia S.L.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "config.h"
#include "ImageBackingStore.h"

#include "CairoUtilities.h"
#include "RefPtrCairo.h"
#include <cairo/cairo.h>
#include <gdk/gdk.h>

namespace WebCore {

static PassRefPtr<cairo_surface_t> createSurfaceForBackingStore(const IntSize& size)
{
    return adoptRef(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size.width(), size.height()));
}

static PassRefPtr<cairo_surface_t> createSurfaceForBackingStore(void* ptr, const IntSize& size)
{
    // width must be even number.
    int width = size.width() + size.width() % 2;

    return adoptRef(cairo_image_surface_create_for_data((unsigned char*)ptr, CAIRO_FORMAT_ARGB32, width, size.height(), width * 4));
}

class ImageBackingStorePrivate {
    WTF_MAKE_NONCOPYABLE(ImageBackingStorePrivate);
    WTF_MAKE_FAST_ALLOCATED;

public:
    RefPtr<cairo_surface_t> m_surface;
    RefPtr<cairo_surface_t> m_scrollSurface;

    static PassOwnPtr<ImageBackingStorePrivate> create(void* ptr, const IntSize& size)
    {
        return adoptPtr(new ImageBackingStorePrivate(ptr, size));
    }

private:
    // We keep two copies of the surface here, which will double the memory usage, but increase
    // scrolling performance since we do not have to keep reallocating a memory region during
    // quick scrolling requests.
    ImageBackingStorePrivate(void* ptr, const IntSize& size)
        : m_surface(createSurfaceForBackingStore(ptr, size))
        , m_scrollSurface(createSurfaceForBackingStore(size))
    {
    }
};

PassOwnPtr<ImageBackingStore> ImageBackingStore::create(void*ptr, const IntSize& size)
{
    return adoptPtr(new ImageBackingStore(ptr, size));
}

ImageBackingStore::ImageBackingStore(void* ptr, const IntSize& size)
    : m_private(ImageBackingStorePrivate::create(ptr, size))
{
}

ImageBackingStore::~ImageBackingStore()
{
}

cairo_surface_t* ImageBackingStore::cairoSurface()
{
    return m_private->m_surface.get();
}

void ImageBackingStore::scroll(const IntRect& scrollRect, const IntSize& scrollOffset)
{
    IntRect targetRect(scrollRect);
    targetRect.move(scrollOffset);
    targetRect.shiftMaxXEdgeTo(targetRect.maxX() - scrollOffset.width());
    targetRect.shiftMaxYEdgeTo(targetRect.maxY() - scrollOffset.height());
    if (targetRect.isEmpty())
        return;

    copyRectFromOneSurfaceToAnother(m_private->m_surface.get(), m_private->m_scrollSurface.get(),
                                    scrollOffset, targetRect);
    copyRectFromOneSurfaceToAnother(m_private->m_scrollSurface.get(), m_private->m_surface.get(),
                                    IntSize(), targetRect);
}

} // namespace WebCore

