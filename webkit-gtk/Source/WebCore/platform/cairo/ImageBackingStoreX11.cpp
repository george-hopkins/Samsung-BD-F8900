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
#include "UIViewWindow.h"

#include <GtkVersioning.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <cairo/cairo-xlib.h>
#include <cairo/cairo.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>


namespace WebCore {
static PassRefPtr<cairo_surface_t> createSurfaceForBackingStore(void* ptr, const IntSize& size, UIViewWindow* viewWindow)
{
    Display* display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    return adoptRef(cairo_xlib_surface_create(display, (Drawable)ptr,
                                                       viewWindow ? (Visual*)viewWindow->visual() : GDK_VISUAL_XVISUAL(gdk_screen_get_system_visual(gdk_screen_get_default())),
                                                       size.width(), size.height()));
}

class ImageBackingStorePrivate {
    WTF_MAKE_NONCOPYABLE(ImageBackingStorePrivate);
    WTF_MAKE_FAST_ALLOCATED;

public:
    Display* m_display;
    Pixmap m_pixmap;
    GC m_gc;
    RefPtr<cairo_surface_t> m_surface;

    static PassOwnPtr<ImageBackingStorePrivate> create(void* ptr, const IntSize& size, UIViewWindow* viewWindow)
    {
        return adoptPtr(new ImageBackingStorePrivate(ptr, size, viewWindow));
    }
    ~ImageBackingStorePrivate()
    {
        XFreeGC(m_display, m_gc);
    }

private:
    // We keep two copies of the surface here, which will double the memory usage, but increase
    // scrolling performance since we do not have to keep reallocating a memory region during
    // quick scrolling requests.
    ImageBackingStorePrivate(void* ptr, const IntSize& size, UIViewWindow* viewWindow)
        : m_surface(createSurfaceForBackingStore(ptr, size, viewWindow))
    {
        m_pixmap = reinterpret_cast<Pixmap>(ptr);
        m_display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
        m_gc = XCreateGC(m_display, m_pixmap, 0, 0);
    }
};

PassOwnPtr<ImageBackingStore> ImageBackingStore::create(void*ptr, const IntSize& size, UIViewWindow* viewWindow)
{
    return adoptPtr(new ImageBackingStore(ptr, size, viewWindow));
}

ImageBackingStore::ImageBackingStore(void* ptr, const IntSize& size, UIViewWindow* viewWindow)
    : m_private(ImageBackingStorePrivate::create(ptr, size, viewWindow))
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

    XCopyArea(m_private->m_display, m_private->m_pixmap, m_private->m_pixmap, m_private->m_gc, 
              targetRect.x() - scrollOffset.width(), targetRect.y() - scrollOffset.height(),
              targetRect.width(), targetRect.height(),
              targetRect.x(), targetRect.y());
}

} // namespace WebCore

