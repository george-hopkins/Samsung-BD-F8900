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

#include "config.h"

#include "UIViewWindow.h"

#include "WidgetBackingStore.h"
#include <GtkVersioning.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <OwnPtr.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <cairo/cairo-xlib.h>
#include <dlfcn.h>

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <XEGLImageWrapper.h>
#endif

#if USE(SDAL)
#include "SdCommon.h"
#include "SdGfx.h"
#endif
namespace WebCore {

UIViewWindow::UIViewWindow()
{
}

UIViewWindow::~UIViewWindow()
{
}

PassOwnPtr<UIViewWindow> UIViewWindowX11::create()
{
    return adoptPtr(new UIViewWindowX11());
}

UIViewWindowX11::UIViewWindowX11()
: m_window(0)
, m_display(0)
, m_dummyWidget(0)
, m_isShow(false)
{
}

void UIViewWindowX11::initialize(const IntRect& rect)
{
    m_display = gdk_x11_display_get_xdisplay(gdk_display_get_default());
    m_rect = rect;

    if (m_rect.width() < 1)
        m_rect.setWidth(1);

    if (m_rect.height() < 1)
        m_rect.setHeight(1);

    if (!m_display) {
       fprintf(stderr, "\n +++++++ cannot connect to X server \n");
       exit(1);
    }

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;

    XVisualInfo* visInfo;
    XVisualInfo visTemplate;
    int numVisuals;
    Window root, xwin;
    XSetWindowAttributes attr;
    unsigned long mask;
    EGLint vid;

    static bool xInit = false;
    static bool bEglInit = false;
    if (!xInit) {
        XEGLInit(); // test
        xInit = true;
    }

    eglDisplay = eglGetDisplay((EGLNativeDisplayType) m_display);

    if (eglDisplay == EGL_NO_DISPLAY) {
       printf("Error: Got no EGL display");
       return;
    }

    EGLint attrs[] = {
        EGL_SAMPLES,            0,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         0,
        EGL_BUFFER_SIZE,        24,
        EGL_DEPTH_SIZE,         16,
        EGL_STENCIL_SIZE,       0,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLConfig ecfg;
    EGLint numConfig;

    if (!bEglInit && !eglInitialize(eglDisplay, 0, 0)) {
        printf("Error: Failed to initialize");
        return;
    }
    bEglInit = true;

    if (!eglChooseConfig(eglDisplay, attrs, &ecfg, 1, &numConfig)) {
       printf("Error: Failed to choose config");
       return;
    }
 
    if (numConfig != 1) {
       printf("Error: idn't get exactly one config, but");
       return;
    }

    if (!eglGetConfigAttrib(eglDisplay,
            ecfg, EGL_NATIVE_VISUAL_ID, &vid))
       printf("failed to get visual id");

    /* The X window visual must match the EGL config */
    visTemplate.visualid = vid;
    visInfo = XGetVisualInfo(m_display,
          VisualIDMask, &visTemplate, &numVisuals);
    if (!visInfo)
       printf("failed to get an visual of id 0x%x", vid);

    root = RootWindow(m_display, DefaultScreen(m_display));

    /* window attributes */
    attr.background_pixel = 0x00000000;
    attr.border_pixel = 0;
    attr.colormap = XCreateColormap(m_display,
          root, visInfo->visual, AllocNone);
    attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
    mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

    XSetWindowAttributes swa;
    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;

    Window win = XCreateWindow(
              m_display, root,
              m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height(),
              0, CopyFromParent, InputOutput, CopyFromParent, CWEventMask, &attr );
              //0, visInfo->depth, InputOutput, visInfo->visual, mask, &attr);
    m_visual = (void*)visInfo->visual;
    m_depth = visInfo->depth;

#else
    Window rootWindow = RootWindow(m_display, DefaultScreen(m_display));
    XSetWindowAttributes swa;
    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;

    Window win = XCreateWindow(
              m_display, rootWindow,
              m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height(), 0,
              CopyFromParent, InputOutput,
              CopyFromParent, CWEventMask,
              &swa);

    m_visual = GDK_VISUAL_XVISUAL(gdk_screen_get_system_visual(gdk_screen_get_default()));
    m_depth = gdk_visual_get_depth(gdk_screen_get_system_visual(gdk_screen_get_default()));
#endif
 
    XSetWindowAttributes xattr;
 
    xattr.override_redirect = False;
    XChangeWindowAttributes(m_display, win, CWOverrideRedirect, &xattr);
    XWMHints hints;
    hints.input = True;
    hints.flags = InputHint;
    XSetWMHints(m_display, win, &hints);
    XStoreName(m_display, win, "Browser");
  
    m_window = win;
}

UIViewWindowX11::~UIViewWindowX11()
{
    if (m_dummyWidget)
        gtk_widget_destroy(m_dummyWidget);

    if (m_display && m_window)
        XDestroyWindow(m_display, m_window);
}

void UIViewWindowX11::moveView(const IntPoint& location)
{
    if(m_rect.location() != location) {
        m_rect.setLocation(location);
        XMoveWindow(m_display, m_window, m_rect.x(), m_rect.y());
    }
}

void UIViewWindowX11::resizeView(const IntSize& size)
{
    if(m_rect.size() != size) {
        m_rect.setSize(size);
        XResizeWindow(m_display, m_window, m_rect.width(), m_rect.height());
    }
}

PassRefPtr<cairo_surface_t> UIViewWindowX11::getWindowSurface() 
{
    return adoptRef(cairo_xlib_surface_create(m_display, m_window, XDefaultVisual(m_display, 0), m_rect.width(), m_rect.height())); 
}

void UIViewWindowX11::sync()
{
#if !ENABLE(TVGG_ACCELERATED_COMPOSITING)
    XSync(m_display, false);
#endif
    XFlush(m_display);
}

void UIViewWindowX11::paintIntoWindow(WidgetBackingStore* backingStore, const IntPoint& srcLocation, const IntRect& dstRect)
{
    return;
}

RefPtr<cairo_surface_t> UIViewWindowX11::createSimilarSurface(const IntSize& size, Pixmap* pixmap)
{
    *pixmap = XCreatePixmap(m_display, m_window,
                                      size.width(), size.height(),
                                      m_depth);

    return adoptRef(cairo_xlib_surface_create(m_display,
                                              *pixmap,
                                              (Visual*)m_visual,
                                              size.width(),
                                              size.height()));
}

void UIViewWindowX11::deletePixmap(Pixmap pixmap)
{
    if (m_display)
        XFreePixmap(m_display, pixmap);
}

void UIViewWindowX11::setCursor(GdkCursor* newCursor)
{
    XDefineCursor(m_display, m_window, gdk_x11_cursor_get_xcursor(newCursor)); 
}

void UIViewWindowX11::show()
{
    if (m_display && m_window && !m_isShow) {
        XMapWindow(m_display, m_window);
        m_isShow = true;
        XSync(m_display, false);
        focus();
    }
}

void UIViewWindowX11::hide()
{
    if (m_display && m_window && m_isShow) {
        XClearWindow(m_display, m_window);
        XSync(m_display, false);
        XUnmapWindow(m_display, m_window);
        XSync(m_display, false);
        m_isShow = false;
    }
}

void UIViewWindowX11::focus()
{
    if (m_display && m_window) {
        XRaiseWindow(m_display, m_window);
        /* There is no way of knowing reliably whether we are viewable;
         * so trap errors asynchronously around the XSetInputFocus call
         */
        gdk_x11_display_error_trap_push(gdk_display_get_default());
        XSetInputFocus(m_display, m_window, RevertToParent, CurrentTime);
        gdk_x11_display_error_trap_pop_ignored(gdk_display_get_default());
    }
}

bool UIViewWindowX11::hasFocus()
{
    if (m_display && m_window) {
        Window window;
        int rtr;
        XGetInputFocus(m_display, &window, &rtr);
        if (m_window == window)
            return true;
    }
    return false;
}

void UIViewWindowX11::scroll(const IntRect& scrollRect, const IntSize& scrollOffset)
{
    IntRect targetRect(scrollRect);
    targetRect.move(scrollOffset);
    targetRect.shiftMaxXEdgeTo(targetRect.maxX() - scrollOffset.width());
    targetRect.shiftMaxYEdgeTo(targetRect.maxY() - scrollOffset.height());
    if (targetRect.isEmpty())
        return;

    GC gc = XCreateGC(m_display, m_window, 0, 0);

    XCopyArea(m_display, m_window, m_window, gc,
              targetRect.x() - scrollOffset.width(), targetRect.y() - scrollOffset.height(),
              targetRect.width(), targetRect.height(),
              targetRect.x(), targetRect.y());
    XFreeGC(m_display, gc);
}


#if 0&&USE(SDAL) && ENABLE(TVGG_ACCELERATED_COMPOSITING)
PassOwnPtr<UIViewWindow> UIViewWindowFB::create()
{
    return adoptPtr(new UIViewWindowFB());
}

UIViewWindowFB::UIViewWindowFB()
: m_window(0)
, m_isShow(false)
{
}

void UIViewWindowFB::initialize(const IntRect& rect)
{
    m_rect = rect;

    if (m_rect.width() < 1)
        m_rect.setWidth(1);

    if (m_rect.height() < 1)
        m_rect.setHeight(1);

    m_window = (bitblt_window *)calloc(1, sizeof(bitblt_window));
    if (!m_window) {
        exit(1);
    }
    m_window->width = m_rect.width();
    m_window->height = m_rect.height();
}

UIViewWindowFB::~UIViewWindowFB()
{
}

void UIViewWindowFB::moveView(const IntPoint& location)
{
    m_rect.setLocation(location);
}

void UIViewWindowFB::resizeView(const IntSize& size)
{
    m_rect.setSize(size);
}

PassRefPtr<cairo_surface_t> UIViewWindowFB::getWindowSurface() 
{
      return NULL;
//    return adoptRef(cairo_xlib_surface_create(m_display, m_window, XDefaultVisual(m_display, 0), m_rect.width(), m_rect.height())); 
}

void UIViewWindowFB::sync()
{
}

void UIViewWindowFB::paintIntoWindow(WidgetBackingStore* backingStore, const IntPoint& srcLocation, const IntRect& dstRect)
{
    return;
}

PassOwnPtr<WidgetBackingStore> UIViewWindowFB::createSimilarImageBuffer(const IntSize& size)
{
    //return WidgetBackingStore::create(m_dummyWidget, size);
    return NULL;
}


void UIViewWindowFB::setCursor(GdkCursor* newCursor)
{
}


void UIViewWindowFB::show()
{
}

void UIViewWindowFB::hide()
{
}

#endif

}
