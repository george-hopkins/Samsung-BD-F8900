/*
 * Copyright (C) 2011, Samsung Electronics
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

#ifndef UIViewWindow_h
#define UIViewWindow_h

#include "IntRect.h"
#include "IntSize.h"
#include "RefPtrCairo.h"


#include <wtf/PassOwnPtr.h>
#include <wtf/PassRefPtr.h>

#if USE(SDAL) && ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "SdCommon.h"
#include "SdGfx.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

typedef unsigned long Window;
typedef unsigned long Pixmap;
typedef struct _XDisplay Display;

using namespace WebCore;

namespace WebCore {
class WidgetBackingStore;
class GraphicsContext;
}

namespace WebCore {

class UIViewWindow {
public:
    static UIViewWindow* getInstance();
    UIViewWindow();
    virtual ~UIViewWindow();
    virtual void initialize(const IntRect&) = 0;
    virtual void moveView(const IntPoint&) = 0;
    virtual void resizeView(const IntSize&) = 0;
    virtual PassRefPtr<cairo_surface_t> getWindowSurface() = 0;
    virtual void sync() { }
    virtual void paintIntoWindow(WidgetBackingStore*, const IntPoint&, const IntRect&) = 0;
    virtual RefPtr<cairo_surface_t> createSimilarSurface(const IntSize& size, Pixmap* pixmap) = 0;
    virtual void deletePixmap(Pixmap) = 0;
    
    virtual void setCursor(GdkCursor* newCursor) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    IntRect viewRect() { return m_rect; }
    virtual Window window() = 0;
    virtual void* visual() = 0;
    virtual void focus() = 0;
    virtual bool hasFocus() = 0;
    virtual bool isShow() = 0;
    virtual void scroll(const IntRect& scrollRect, const IntSize& scrollOffset) = 0;
protected:
    IntRect m_rect;
};

class UIViewWindowX11 : public UIViewWindow {
public:
    virtual ~UIViewWindowX11();
    static PassOwnPtr<UIViewWindow> create();
    virtual void initialize(const IntRect&);
    virtual void resizeView(const IntSize&);
    virtual void moveView(const IntPoint&);
    virtual PassRefPtr<cairo_surface_t> getWindowSurface();
    virtual void sync();
    virtual void paintIntoWindow(WidgetBackingStore*, const IntPoint&, const IntRect&);
    virtual RefPtr<cairo_surface_t> createSimilarSurface(const IntSize& size, Pixmap* pixmap);
    virtual void deletePixmap(Pixmap);

    virtual void setCursor(GdkCursor* newCursor);
    virtual void show();
    virtual void hide();
    virtual Window window() { return m_window; }
    virtual void* display() { return (void*)m_display; }
    virtual void* visual() { return (void*)m_visual; }
    virtual void focus();
    virtual bool hasFocus();
    virtual bool isShow() { return m_isShow; }
    virtual void scroll(const IntRect& scrollRect, const IntSize& scrollOffset);
protected:
    UIViewWindowX11();
private:
    Window m_window;
    Display* m_display; 
    void* m_visual;
    GtkWidget* m_dummyWidget;
    bool m_isShow;
    int m_depth;
};

#if 0&&USE(SDAL) && ENABLE(TVGG_ACCELERATED_COMPOSITING)
class UIViewWindowFB : public UIViewWindow {
public:
    virtual ~UIViewWindowFB();
    static PassOwnPtr<UIViewWindow> create();
    virtual void initialize(const IntRect&);
    virtual void resizeView(const IntSize&);
    virtual void moveView(const IntPoint&);
    virtual PassRefPtr<cairo_surface_t> getWindowSurface();
    virtual void sync();
    virtual void paintIntoWindow(WidgetBackingStore*, const IntPoint&, const IntRect&);
    virtual PassOwnPtr<WidgetBackingStore> createSimilarImageBuffer(const IntSize&);

    virtual void setCursor(GdkCursor* newCursor);
    virtual void show();
    virtual void hide();
protected:
    UIViewWindowFB();
private:
    bitblt_window* m_window;
    bool m_isShow;
};
#endif

}

#endif // UIViewWindow_h
