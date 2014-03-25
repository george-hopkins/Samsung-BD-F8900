#include "config.h"
#include "BEView.h"

#include "BEAPICast.h"
#include "DrawingAreaMessages.h"
#include "GtkClickCounter.h"
#include "JSKeyboardEvent.h"
#include "NativeWebKeyboardEvent.h"
#include "NativeWebWheelEvent.h"
#include "NativeWebMouseEvent.h"
#include "NotifierDrawingAreaProxy.h"
#include "NPJSObject.h"
#include "Region.h"
#include "WebEventFactory.h"
#include "WebFrame.h"
#include "WebKitWebViewBasePrivate.h"
#include "WebPage.h"
#include "WebPageProxyMessages.h"
#include "WebProcess.h"
#include "WebProcessProxy.h"

#include <WebCore/Color.h>
#include <WebCore/Cursor.h>
#include <WebCore/FileSystem.h>
#include <WebCore/Frame.h>
#include <WebCore/GtkVersioning.h>
#include <WebCore/KeyboardEvent.h>
#include <WebCore/MMManager.h>
#include <WebCore/PlatformContextCairo.h>
#include <WebCore/PlatformKeyboardEvent.h>
#include <WebCore/SnapshotDatabase.h>
#include <cairo/cairo-xlib.h>
#include <gdk/gdkx.h>
#include <wtf/PassRefPtr.h>
#include <wtf/RefPtr.h>

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
#include "GraphicsLayerRenderer.h"
#include "UIViewWindow.h"
#endif

#if USE(UNIPLAYER)
#include "MediaPlayerPrivateUniplayer.h"
#endif

#if ENABLE(TVGG_COOKIE_DELETE_FEATURE)
#include "CookieJarSoup.h"
#endif

#if ENABLE(WIDGET_ENGINE)
extern bool useWidgetEngine;
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
WebKitWebViewBase* g_currentView = 0;
#endif
#endif

using namespace WebKit;
using namespace WebCore;

#ifdef ENABLE_HBBTV
typedef enum EViewTransparency
{
    /// The unknown state is set initially and when an exposure is received.
    EVIEW_UNKNOWN,

    /// The Opaque state is set if anything is drawn
    EVIEW_OPAQUE,

    /// The Transparent state is set when the fully transparent
    EVIEW_TRANSPARENT
} EViewTransparency;

/// Here we store the last state of the transparency
static EViewTransparency transparency_state = EVIEW_UNKNOWN;
#endif // ENABLE_HBBTV

BEViewRef BEViewCreate(BEContextRef contextRef, BEPageGroupRef pageGroupRef)
{
    WebKitWebViewBase* view = webkitWebViewBaseCreate(toBEImpl(contextRef), toBEImpl(pageGroupRef));
    GtkWidget* toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(GTK_WIDGET(toplevel), "BrowserView");
    gtk_window_set_has_resize_grip(GTK_WINDOW(toplevel), false);
    gtk_container_add(GTK_CONTAINER(toplevel), GTK_WIDGET(view));
//    gtk_window_set_default(GTK_WINDOW(toplevel), GTK_WIDGET(view));
    return toBEAPI(view);
}

BEViewRef BEViewCreateWithDirectDrawing(BEContextRef contextRef, BEPageGroupRef pageGroupRef)
{
    WebKitWebViewBase* view = webkitWebViewBaseCreateWithDrawingAreaType(toBEImpl(contextRef), toBEImpl(pageGroupRef), DrawingAreaTypeDirect);
    g_object_ref_sink(G_OBJECT(view));

    return toBEAPI(view);
}

BEViewRef BEViewCreateWithNotifyDrawing(BEContextRef contextRef, BEPageGroupRef pageGroupRef)
{
    WebKitWebViewBase* view = webkitWebViewBaseCreateWithDrawingAreaType(toBEImpl(contextRef), toBEImpl(pageGroupRef), DrawingAreaTypeNotifier);
    g_object_ref_sink(G_OBJECT(view));
    return toBEAPI(view);
}

void BEViewDestroy(BEViewRef viewRef)
{
#ifdef ENABLE_HBBTV
    transparency_state = EVIEW_TRANSPARENT;
#endif // ENABLE_HBBTV

    WebKitWebViewBase* view = toBEImpl(viewRef);

#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine && g_currentView == view)
        g_currentView = 0;
#endif

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));

    // hide window before destory biew.
    BEViewHide(viewRef);

    if (gtk_widget_is_toplevel(widget))
        gtk_widget_destroy(widget);
    else {
        gtk_widget_destroy(GTK_WIDGET(view));
        g_object_unref(view);
    }
}

BEPageRef BEViewGetPage(BEViewRef viewRef)
{
    return toBEAPI(webkitWebViewBaseGetPage(toBEImpl(viewRef)));
}

void BEViewShow(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(WIDGET_ENGINE)
        if (useWidgetEngine)
            g_currentView = view;
#endif
        webkitWebViewBaseGetPage(view)->showUIView();
        return;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gtk_widget_show_all(widget);
}

void BEViewHide(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        webkitWebViewBaseGetPage(view)->hideUIView();
        WebPage* page = WebProcess::shared().focusedWebPage();
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
        if (page && page->drawingArea() && page->getRenderer()) {
            page->getRenderer()->clear();
            page->drawingArea()->scheduleCompositingLayerSync();
        }
#endif
        return;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gtk_widget_hide(widget);
}

#ifdef ENABLE_HBBTV
bool BEViewIsFullyTransparent()
{
    if(transparency_state == EVIEW_UNKNOWN)
    {
        WebPage* page = WebProcess::shared().mainUIWebPage();
        if(page)
        {
            bool rc = true;
            float factor = 0.1f;
            const WebCore::IntRect rect(WebCore::IntPoint(0,0), page->size());

            cairo_surface_t *cs = cairo_image_surface_create(CAIRO_FORMAT_A8,
                                                             rect.width()*factor,
                                                             rect.height()*factor);
            if(cs)
            {
                cairo_t *cr = cairo_create(cs);
                if(cr)
                {
                    WebCore::GraphicsContext ctx(cr);
                    ctx.clearRect(rect);

                    WebCore::FloatSize scaleFactor(factor, factor);
                    ctx.scale(scaleFactor);

                    page->drawRect(ctx, rect);

                    unsigned char *p = cairo_image_surface_get_data(cs);
                    int size = rect.width()*factor*rect.height()*factor;

                    for(int i=0;i<size;i++)
                    {
                        if(*p)
                        {
                            rc = false;
                            break;
                        }
                        p++;
                    }
                    cairo_destroy(cr);
                }
                cairo_surface_destroy(cs);
            }
            transparency_state = rc ? EVIEW_TRANSPARENT : EVIEW_OPAQUE;
        }
    }

    return transparency_state == EVIEW_TRANSPARENT;
}
#endif // ENABLE_HBBTV

bool BEViewGetVisible(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect)
        return webkitWebViewBaseGetPage(view)->isShowUIView();

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        return gtk_widget_get_visible(widget);
    else
        return false;
}

void BEViewSetBackgroundColor(BEViewRef viewRef, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);
    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget)) {
        GdkRGBA color = { static_cast<double>(red) / 255, static_cast<double>(green) / 255,
            static_cast<double>(blue) / 255, static_cast<double>(alpha) / 255 };
        gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL, &color);
    }

    webkitWebViewBaseGetPage(view)->setBackgroundColor(Color(red, green, blue, alpha));
}

void BEViewSetPosition(BEViewRef viewRef, int x, int y)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        webkitWebViewBaseGetPage(view)->setUIViewPosition(IntPoint(x, y));
        return;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gtk_window_move(GTK_WINDOW(widget), x, y);
}

BEPoint BEViewGetPosition(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);
    BEPoint position;

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        int w, h;
        webkitWebViewBaseGetPage(view)->getUIViewRect(position.x, position.y, w, h);
        return position;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));

    if (gtk_widget_is_toplevel(widget))
        gtk_window_get_position(GTK_WINDOW(widget), &position.x, &position.x);
    else {
        position.x = 0;
        position.y = 0;
    }
    return position;
}

void BEViewSetSize(BEViewRef viewRef, int width, int height)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        webkitWebViewBaseGetPage(view)->setUIViewSize(IntSize(width, height));
        webkitWebViewBaseGetDrawingArea(view)->setSize(IntSize(width, height), IntSize());
#if ENABLE(SEF)
        MMManager::getInstance()->setScreenSize(IntSize(width, height));
#endif // #if ENABLE(SEF)
        return;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gtk_window_resize(GTK_WINDOW(widget), width, height);
    else
        webkitWebViewBaseGetDrawingArea(view)->setSize(IntSize(width, height), IntSize());
}

BESize BEViewGetSize(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);
    BESize size;

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        int x, y;
        webkitWebViewBaseGetPage(view)->getUIViewRect(x, y, size.width, size.height);
        return size;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));

    if (gtk_widget_is_toplevel(widget))
        gtk_window_get_size(GTK_WINDOW(widget), &size.width, &size.height);
    else if (webkitWebViewBaseGetPage(view)) {
        IntSize drawingAreaSize = webkitWebViewBaseGetDrawingArea(view)->size();
        size.width = drawingAreaSize.width();
        size.height = drawingAreaSize.height();
    } else {
        size.width = 0;
        size.height = 0;
    }

    return size;
}

void BEViewSetLocation(BEViewRef viewRef, int x, int y)
{
    IntPoint offset;
    WebProcess::shared().mainUIWebPage()->sendSync(Messages::WebPageProxy::ScreenToWindow(IntPoint()), Messages::WebPageProxy::ScreenToWindow::Reply(offset));
    webkitWebViewBaseGetPageClient(toBEImpl(viewRef))->setLocation(IntPoint(x - offset.x(), y - offset.y()));
}

void BEViewSetRect(BEViewRef viewRef, int x, int y, int width, int height)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    webkitWebViewBaseGetDrawingArea(view)->setSize(IntSize(width, height), IntSize());

    IntPoint offset;
    WebProcess::shared().mainUIWebPage()->sendSync(Messages::WebPageProxy::ScreenToWindow(IntPoint()), Messages::WebPageProxy::ScreenToWindow::Reply(offset));

    webkitWebViewBaseGetPageClient(toBEImpl(viewRef))->setLocation(IntPoint(x - offset.x(), y - offset.y()));

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gtk_window_resize(GTK_WINDOW(widget), width, height);
    else
        webkitWebViewBaseGetDrawingArea(view)->setSize(IntSize(width, height), IntSize());

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && page->getRenderer())
        page->getRenderer()->setContentsViewLocation(IntPoint(x, y));
#endif
#if ENABLE(SEF)
    MMManager::getInstance()->setPipLocationOffset(IntPoint(x - offset.x(), y - offset.y()));
#endif // #if ENABLE(SEF)
#if USE(UNIPLAYER)
    WebCore::MediaPlayerPrivate::didContentViewRectChanged(IntRect(x, y, width, height));
#endif
}

void BEViewSetActiveNotifierDrawingArea(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);
    DrawingAreaProxy* drawingArea = webkitWebViewBaseGetDrawingArea(view);
    if (drawingArea->type() != DrawingAreaTypeNotifier)
        return;

    static_cast<NotifierDrawingAreaProxy*>(drawingArea)->setActive();
}

BESize BEViewGetScreenSize()
{
    BESize size;
    size.width = gdk_screen_get_width(gdk_screen_get_default());
    size.height = gdk_screen_get_height(gdk_screen_get_default());
    return size;
}

void BEViewSetFocus(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect) {
        webkitWebViewBaseGetPage(view)->setUIViewFocus();
        return;
    }

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        gdk_window_focus(gtk_widget_get_window(widget), GDK_CURRENT_TIME);
}

bool BEViewHasFocus(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);

    if (webkitWebViewBaseGetDrawingArea(view)->type() == DrawingAreaTypeDirect)
        return webkitWebViewBaseGetPage(view)->getUIViewHasFocus();

    GtkWidget* widget = gtk_widget_get_toplevel(GTK_WIDGET(view));
    if (gtk_widget_is_toplevel(widget))
        return gtk_widget_has_focus(GTK_WIDGET(view));
    else
        return false;
}

void BEViewSendKeyEvent(BEViewRef viewRef, unsigned keycode, bool isPress)
{
    GdkEvent gdkEvent;
    GdkEventKey* key = &gdkEvent.key;
    memset(key, 0, sizeof(GdkEventKey));
    key->type = isPress ? GDK_KEY_PRESS : GDK_KEY_RELEASE;
    key->keyval = keycode;
    key->time = GDK_CURRENT_TIME;

    GdkKeymapKey* keys;
    gint n_keys;
    if(gdk_keymap_get_entries_for_keyval(gdk_keymap_get_default(), keycode, &keys, &n_keys)) {
        key->hardware_keycode = keys[0].keycode;
        g_free(keys);
    }

    webkitWebViewBaseGetPage(toBEImpl(viewRef))->handleKeyboardEvent(NativeWebKeyboardEvent(&gdkEvent));
}

#ifdef ENABLE_HBBTV
void BEViewSendHbbTVKeyEvent(BEViewRef viewRef, unsigned keycode, bool isPress)
{
    GdkEvent gdkEvent;
    GdkEventKey* key = &gdkEvent.key;
    memset(key, 0, sizeof(GdkEventKey));
    key->type = isPress ? GDK_KEY_PRESS : GDK_KEY_RELEASE;
    key->keyval = keycode;

    GdkKeymapKey* keys;
    gint n_keys;
    if(gdk_keymap_get_entries_for_keyval(gdk_keymap_get_default(), keycode, &keys, &n_keys)) {
        key->hardware_keycode = keys[0].keycode;
        g_free(keys);
    }

    webkitWebViewBaseGetPage(toBEImpl(viewRef))->handleKeyboardEvent(NativeWebKeyboardEvent(&gdkEvent, true));
}
#endif

void BEViewSendEvent(BEViewRef viewRef, NPObjectRef eventRef)
{
    if (!eventRef || !NPJSObject::isNPJSObject(eventRef))
        return;

    JSC::JSObject* event = NPJSObject::toNPJSObject(eventRef)->jsObject();
    if (!event)
        return;

    if (event->className() == "KeyboardEvent") {
        JSKeyboardEvent* jsEvent = static_cast<JSKeyboardEvent*>(event);
        const PlatformKeyboardEvent* platformEvent = static_cast<KeyboardEvent*>(jsEvent->impl())->keyEvent();

        GdkEvent gdkEvent;
        GdkEventKey* key = &gdkEvent.key;
        memset(key, 0, sizeof(GdkEventKey));
        key->type = platformEvent->type() == PlatformKeyboardEvent::KeyUp ? GDK_KEY_RELEASE : GDK_KEY_PRESS;
        key->keyval = platformEvent->nativeVirtualKeyCode();

        GdkKeymapKey* keys;
        gint n_keys;
        if(gdk_keymap_get_entries_for_keyval(gdk_keymap_get_default(), key->keyval, &keys, &n_keys)) {
            key->hardware_keycode = keys[0].keycode;
            g_free(keys);
        }

        webkitWebViewBaseGetPage(toBEImpl(viewRef))->handleKeyboardEvent(NativeWebKeyboardEvent(&gdkEvent));
    }
}

static bool handleBtnPressEvent(WebPageProxy* page, XButtonEvent* btn)
{
     if (btn->button >= 4) {
        GdkEventScroll event;
        memset(&event, 0, sizeof(GdkEventScroll));
        event.type = GDK_SCROLL;
        event.window = 0;
        event.time = btn->time;
        event.x = (gdouble) btn->x;
        event.y = (gdouble) btn->y;
        event.x_root = (gdouble) btn->x_root;
        event.y_root = (gdouble) btn->y_root;
        event.state = (GdkModifierType) btn->state;
        if (btn->button == 4)
            event.direction = GDK_SCROLL_UP;
        else if (btn->button == 5)
            event.direction = GDK_SCROLL_DOWN;
        else if (btn->button == 6)
            event.direction = GDK_SCROLL_LEFT;
        else
            event.direction = GDK_SCROLL_RIGHT;
#ifdef GTK_API_VERSION_2
        event.device = gdk_display_get_default()->core_pointer;
#else
        event.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

        page->handleWheelEvent(NativeWebWheelEvent(reinterpret_cast<GdkEvent*>(&event)));
    } else {
        GdkEventButton event;
        memset(&event, 0, sizeof(GdkEventButton));
        event.type = GDK_BUTTON_PRESS;
        event.window = 0;
        event.time = btn->time;
        event.x = (gdouble) btn->x;
        event.y = (gdouble) btn->y;
        event.x_root = (gdouble) btn->x_root;
        event.y_root = (gdouble) btn->y_root;
        event.axes = 0;
        event.state = (GdkModifierType) btn->state;
        event.button = btn->button;
#ifdef GTK_API_VERSION_2
        event.device = gdk_display_get_default()->core_pointer;
#else
        event.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

        page->handleMouseEvent(NativeWebMouseEvent(reinterpret_cast<GdkEvent*>(&event), 1));
    }

     return true;
}

static bool handleBtnReleaseEvent(WebPageProxy* page, XButtonEvent* btn)
{
    GdkEventButton event;
    memset(&event, 0, sizeof(GdkEventButton));
    event.type = GDK_BUTTON_RELEASE;
    event.window = 0;
    event.time = btn->time;
    event.x = (gdouble) btn->x;
    event.y = (gdouble) btn->y;
    event.x_root = (gdouble) btn->x_root;
    event.y_root = (gdouble) btn->y_root;
    event.axes = 0;
    event.state = (GdkModifierType) btn->state;
    event.button = btn->button;
#ifdef GTK_API_VERSION_2
    event.device = gdk_display_get_default()->core_pointer;
#else
    event.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

    page->handleMouseEvent(NativeWebMouseEvent(reinterpret_cast<GdkEvent*>(&event), 0));

    return true;
}

static bool handleExposeEvent(WebKitWebViewBase* view, XGraphicsExposeEvent* expose)
{
    DrawingAreaProxy* drawingArea = webkitWebViewBaseGetDrawingArea(view);
    if (!drawingArea || drawingArea->type() != DrawingAreaTypeNotifier)
        return FALSE;

    WebKit::Region unpaintedRegion; // This is simply unused.

#if ENABLE(TVGG_ACCELERATED_TV_PLUGIN_PAINTING)
    GraphicsContext* context = reinterpret_cast<GraphicsContext*>(expose->drawable);
    cairo_t* cr = context->platformContext()->cr();

    static_cast<NotifierDrawingAreaProxy*>(drawingArea)->paint(cr, WebCore::IntRect(expose->x, expose->y, expose->width, expose->height), unpaintedRegion);
#else
    static Visual* visual = 0;

    if (!visual) {
        Display* display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());

        XVisualInfo visualTemplate;
        visualTemplate.screen = gdk_screen_get_number(gdk_screen_get_default());
        visualTemplate.depth = gdk_visual_get_depth(gdk_screen_get_system_visual(gdk_screen_get_default()));
        visualTemplate.c_class = TrueColor;
        int numMatching;
        XVisualInfo* visualInfo = XGetVisualInfo(display, VisualScreenMask | VisualDepthMask | VisualClassMask,
                                             &visualTemplate, &numMatching);
        ASSERT(visualInfo);
        visual = visualInfo[0].visual;
        ASSERT(visual);
        XFree(visualInfo);
    }

    IntSize size = drawingArea->size();
    cairo_surface_t* surface = cairo_xlib_surface_create(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()),
                                                          expose->drawable,
                                                          visual,
                                                          size.width(),
                                                          size.height());
    cairo_t* cr = cairo_create(surface);
    cairo_surface_destroy(surface);

    static_cast<NotifierDrawingAreaProxy*>(drawingArea)->paint(cr, WebCore::IntRect(expose->x, expose->y, expose->width, expose->height), unpaintedRegion);

    cairo_destroy(cr);
#endif

    return true;
}

static bool handleMotionEvent(WebPageProxy* page, XMotionEvent* motion)
{
    GdkEventMotion event;
    memset(&event, 0, sizeof(GdkEventMotion));
    event.type = GDK_MOTION_NOTIFY;
    event.window = 0;
    event.time = motion->time;
    event.x = (gdouble) motion->x;
    event.y = (gdouble) motion->y;
    event.x_root = (gdouble) motion->x_root;
    event.y_root = (gdouble) motion->y_root;
    event.axes = 0;
    event.state = (GdkModifierType) motion->state;
    event.is_hint = motion->is_hint;
#ifdef GTK_API_VERSION_2
    event.device = gdk_display_get_default()->core_pointer;
#else
    event.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

    page->handleMouseEvent(NativeWebMouseEvent(reinterpret_cast<GdkEvent*>(&event), 0));

    return true;
}

bool BEViewHandleEvent(BEViewRef viewRef, BEPlatformEventRef eventRef)
{
    if (!eventRef)
        return false;

#ifdef ENABLE_HBBTV
    transparency_state = EVIEW_UNKNOWN;
#endif // ENABLE_HBBTV

    XEvent* event = static_cast<XEvent*>(eventRef);
    switch ((static_cast<XEvent*>(event))->type) {
    case ButtonPress:
        return handleBtnPressEvent(webkitWebViewBaseGetPage(toBEImpl(viewRef)), &static_cast<XEvent*>(event)->xbutton);
    case ButtonRelease:
        return handleBtnReleaseEvent(webkitWebViewBaseGetPage(toBEImpl(viewRef)), &static_cast<XEvent*>(event)->xbutton);
    case GraphicsExpose:
        return handleExposeEvent(toBEImpl(viewRef), &static_cast<XEvent*>(event)->xgraphicsexpose);
    case MotionNotify:
        return handleMotionEvent(webkitWebViewBaseGetPage(toBEImpl(viewRef)), &static_cast<XEvent*>(event)->xmotion);
    default:
        break;
    }

    return false;
}

void BEViewSetNotifyDrawingCallback(BEViewRef viewRef, BEViewNotifyDrawingCallback callback, const void* clientInfo)
{
    webkitWebViewBaseGetPageClient(toBEImpl(viewRef))->setDidUpdateCallback(static_cast<DidUpdateCallback>(callback), clientInfo);
}

bool BEViewSetFocusRectPainting(BEViewRef viewRef, bool isPainting, BEDirection scrollDirection)
{
    DrawingAreaProxy* drawingArea = webkitWebViewBaseGetDrawingArea(toBEImpl(viewRef));
    if (drawingArea->type() == DrawingAreaTypeNotifier) {
        FocusDirection focusDirection;
        if (scrollDirection >= BEDirectionUp && scrollDirection <= BEDirectionRight)
            focusDirection = static_cast<FocusDirection>(scrollDirection + (FocusDirectionUp - BEDirectionUp));
        else
            focusDirection = FocusDirectionNone;

        return static_cast<NotifierDrawingAreaProxy*>(drawingArea)->setFocusRectPainting(isPainting, focusDirection);
    }

    return false;
}

bool BEViewSetFocusByCursorPosition(BEViewRef viewRef)
{
    BEPoint screenPosition = BEViewGetCursorPosition();
    return webkitWebViewBaseGetPage(toBEImpl(viewRef))->setFocusByScreenPoint(IntPoint(screenPosition.x, screenPosition.y));
}

BEIntRect BEViewGetNearestFocusableNodeRect(BEViewRef viewRef, BEDirection direction, int x, int y)
{
    FocusDirection focusDirection;
    if (direction >= BEDirectionUp && direction <= BEDirectionRight)
        focusDirection = static_cast<FocusDirection>(direction + (FocusDirectionUp - BEDirectionUp));
    else
        focusDirection = FocusDirectionNone;

    IntRect rect(webkitWebViewBaseGetPage(toBEImpl(viewRef))->getNearestFocusableNodeRectByScreenPoint(focusDirection, IntPoint(x, y)));
    BEIntRect intRect = { rect.x(), rect.y(), rect.width(), rect.height() };
    return intRect;
}

BEIntRect BEViewGetNearestFocusableNodeRectByGravityCursor(BEViewRef viewRef, const char* command, int x, int y)
{
    IntRect rect(webkitWebViewBaseGetPage(toBEImpl(viewRef))->getNearestFocusableNodeRectByGravityCursor(String::fromUTF8(command), IntPoint(x, y)));
    BEIntRect intRect = { rect.x(), rect.y(), rect.width(), rect.height() };
    return intRect;
}

void BEViewTakeSnapshot(BEViewRef viewRef)
{
    WebKitWebViewBase* view = toBEImpl(viewRef);
    DrawingAreaProxy* drawingArea = webkitWebViewBaseGetDrawingArea(view);
    double scaleFactor = SnapshotDatabase::getDB().scaleFactor();
    if (!drawingArea || drawingArea->type() != DrawingAreaTypeNotifier || !scaleFactor)
        return;

    webkitWebViewBaseGetPage(view)->process()->send(Messages::DrawingArea::CaptureSnapshot(scaleFactor), webkitWebViewBaseGetPage(view)->pageID());
}

static void sendMouseClickEvent(BEViewRef viewRef, WebMouseEvent::Button button, int x, int y, bool isUseUserParam)
{
    WebPage* page = NULL;
    if (viewRef)
       page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    else
       page = WebProcess::shared().mainUIWebPage();

    if (page) {
        BEPoint pt = BEViewGetCursorPosition();

        if (isUseUserParam) {
            pt.x = x;
            pt.y = y;
        }

        page->simulateMouseDown(button, IntPoint(pt.x, pt.y), 1, 0, 0);
        page->simulateMouseUp(button, IntPoint(pt.x, pt.y), 1, 0, 0);
    }
}

static void sendMouseEvent(BEViewRef viewRef, WebMouseEvent::Button button, WebEvent::Type type, int x, int y, bool isUseUserParam)
{
#if ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine) {
        bool isPopupMenu = false;
        int winX, winY;
        GdkWindow* gdkWindow = gdk_window_at_pointer(&winX, &winY);

        gpointer data;
        if (gdkWindow) {
            gdk_window_get_user_data(gdkWindow, &data);
            isPopupMenu = GTK_IS_MENU_ITEM((GtkWidget*)data);
        }

        if (type == WebEvent::MouseDown) {
            if (isPopupMenu)
                g_signal_emit_by_name((GtkWidget*)data, "activate");

            if (viewRef)
                webkitWebViewBaseGetPage(toBEImpl(viewRef))->forceHidePopupMenu();
        }

        if (isPopupMenu)
            return;
    }
#endif

    WebPage* page = NULL;
    if (viewRef)
        page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    else
        page = WebProcess::shared().mainUIWebPage();

    if (page) {
        BEPoint pt = BEViewGetCursorPosition();

        if (isUseUserParam) {
            pt.x = x;
            pt.y = y;
        }

        if (type == WebEvent::MouseDown)
            page->simulateMouseDown(button, IntPoint(pt.x, pt.y), 1, 0, 0);
        else if (type == WebEvent::MouseUp)
            page->simulateMouseUp(button, IntPoint(pt.x, pt.y), 1, 0, 0);
    }
}

void BEViewSendMouseLeftDownEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseEvent(viewRef, WebMouseEvent::LeftButton, WebEvent::MouseDown, x, y, isUseUserParam);
}

void BEViewSendMouseLeftUpEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseEvent(viewRef, WebMouseEvent::LeftButton, WebEvent::MouseUp, x, y, isUseUserParam);
}

void BEViewSendMouseRightDownEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseEvent(viewRef, WebMouseEvent::RightButton, WebEvent::MouseDown, x, y, isUseUserParam);
}

void BEViewSendMouseRightUpEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseEvent(viewRef, WebMouseEvent::RightButton, WebEvent::MouseUp, x, y, isUseUserParam);
}

void BEViewSendMouseLeftClickEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseClickEvent(viewRef, WebMouseEvent::LeftButton, x, y, isUseUserParam);
}

void BEViewSendMouseRightClickEvent(BEViewRef viewRef, int x, int y, bool isUseUserParam)
{
    sendMouseClickEvent(viewRef, WebMouseEvent::RightButton, x, y, isUseUserParam);
}

void BEViewSendMouseWheelEvent(BEViewRef viewRef, BEDirection direction, int x, int y, bool isUseUserParam)
{
    WebPage* page = NULL;
    if (viewRef)
       page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    else
       page = WebProcess::shared().mainUIWebPage();

    BEPoint pt = BEViewGetCursorPosition();
    if (isUseUserParam) {
       pt.x = x;
       pt.y = y;
    }

    switch (direction) {
    case BEDirectionUp: direction = (BEDirection)GDK_SCROLL_UP; break;
    case BEDirectionDown: direction = (BEDirection)GDK_SCROLL_DOWN; break;
    case BEDirectionLeft:  direction = (BEDirection)GDK_SCROLL_LEFT; break;
    case BEDirectionRight:  direction = (BEDirection)GDK_SCROLL_RIGHT; break;
    default: direction = (BEDirection)GDK_SCROLL_DOWN; break;
    }

    GdkEventScroll event;
    memset(&event, 0, sizeof(GdkEventScroll));
    event.type = GDK_SCROLL;
    event.window = 0;
    event.time = 0;
    event.x = (gdouble) pt.x;
    event.y = (gdouble) pt.y;
    event.x_root = (gdouble) 0;
    event.y_root = (gdouble) 0;
    event.state = 0;
    event.direction = (GdkScrollDirection)direction;
#ifdef GTK_API_VERSION_2
    event.device = gdk_display_get_default()->core_pointer;
#else
    event.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

    page->wheelEventWrapper(WebEventFactory::createWebWheelEvent((GdkEvent*)(&event)));
}

void BEViewShowCursor(BEViewRef viewRef)
{
    WebProcess::shared().setCursorVisible(true);
    BEPoint position = BEViewGetCursorPosition();
    BEViewSetCursorPosition(viewRef, position.x, position.y, true);
}

void BEViewHideCursor(BEViewRef viewRef)
{
    BEViewSetNoneCursor(viewRef);
    WebProcess::shared().setCursorVisible(false);
}

void BEViewReloadCursor(BEViewRef viewRef)
{
    if (WebProcess::shared().isCursorVisible()) {
        BEPoint position = BEViewGetCursorPosition();
        BEViewSetCursorPosition(viewRef, position.x, position.y, false);
    }
}

void BEViewSetNoneCursor(BEViewRef viewRef)
{
    WebPage* page = NULL;
    if (viewRef)
        page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    else
        page = WebProcess::shared().mainUIWebPage();

    if (page)
        page->send(Messages::WebPageProxy::SetCursor(noneCursor()));
}

void BEViewSetCursorSize(bool isUseSmallCursor)
{
#if ENABLE(TVGG_CUSTOM_CURSOR)
    WebCore::Cursor::setCursorSize(isUseSmallCursor);
#endif
}

void BEViewClearCursor()
{
#if ENABLE(TVGG_CUSTOM_CURSOR)
    WebCore::Cursor::clearCursor(WebCore::Cursor::Pointer);
    WebCore::Cursor::clearCursor(WebCore::Cursor::Hand);
    WebCore::Cursor::clearCursor(WebCore::Cursor::IBeam);
    WebCore::Cursor::clearCursor(WebCore::Cursor::Move);
#endif
}

void BEViewSetDragMode(bool isDragMode)
{
#if ENABLE(TVGG_DRAG_MODE)
    WebProcess::shared().setDragMode(isDragMode);
#endif
    if (isDragMode)
        BEViewSendMouseLeftDownEvent(NULL);
    else
        BEViewSendMouseLeftUpEvent(NULL);

    BEPoint point = BEViewGetCursorPosition();
    BEViewSetCursorPosition(NULL, point.x, point.y, false);
}

void BEViewSetCursorPosition(BEViewRef viewRef, int x, int y, bool isWarpPointer)
{
#if ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine) {
        if (webkitWebViewBaseGetPage(toBEImpl(viewRef))->isShowPopupMenu())
            isWarpPointer = true;
    }
#endif

    if (isWarpPointer) {
        static GdkDisplay* display = gdk_display_get_default();
        if (!display)
            return;
        static GdkScreen* screen = gdk_display_get_default_screen(display);
        if (!screen)
            return;

        gdk_display_warp_pointer(display, screen, x, y);
    }

    WebPage* page = NULL;
    if (viewRef)
        page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    else
        page = WebProcess::shared().mainUIWebPage();

    if (page) {
        page->simulateMouseMotion(IntPoint(x, y), 0);
        WebProcess::shared().setPointerPosition(x, y);
    }
}

BEPoint BEViewGetCursorPosition()
{
    BEPoint position = { 0, 0 };
    WebProcess::shared().getPointerPosition(position.x, position.y);
    return position;
}

void BEViewSetMouseEventState(bool enabled)
{
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page)
        page->sendSync(Messages::WebPageProxy::SetMouseEventState(enabled), Messages::WebPageProxy::SetMouseEventState::Reply());
}

void BEViewSetBrowserUIState(int state)
{
    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page)
        page->send(Messages::WebPageProxy::SetBrowserUIState(state));
}

void BEViewInitPIP(bool bPreloading)
{
#if ENABLE(SEF)
    MMManager::getInstance()->init(bPreloading);
#endif // #if ENABLE(SEF)
}

void BEViewSetPIPRect(int x, int y, int width, int height)
{
#if ENABLE(SEF)
    IntRect PIPRect(x, y, width, height);
    IntRect prevPIPRect = MMManager::getInstance()->getPipRect();

    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page && MMManager::getInstance()->isPipEnabled()) {
        page->drawingArea()->setNeedsDisplay(prevPIPRect);
        page->drawingArea()->setNeedsDisplay(PIPRect);
    }

    MMManager::getInstance()->setPipRect(PIPRect);
#endif // #if ENABLE(SEF)
}

void BEViewSetPIPEnable(bool enable)
{
#if ENABLE(SEF)
    MMManager::getInstance()->setPipEnabled(enable);

    WebPage* page = WebProcess::shared().mainUIWebPage();
    if (page)
        page->drawingArea()->setNeedsDisplay(MMManager::getInstance()->getPipRect());
#endif // #if ENABLE(SEF)
}

bool BEViewPIPEnabled()
{
#if ENABLE(SEF)
    return MMManager::getInstance()->isPipEnabled();
#else
    return false;
#endif // #if ENABLE(SEF)
}

void BEViewDidScroll() // BEViewRef viewRef
{
#if USE(UNIPLAYER)
    MediaPlayerPrivate::didScroll();
#endif
}

void BEViewDeleteCookies()
{
    SoupCookieJar* CookieJar;
    CookieJar = WebCore::defaultCookieJar();
#if ENABLE(TVGG_COOKIE_DELETE_FEATURE)
    WebCore::deleteAllCookies(CookieJar);
#endif 
    //printf("[%s:%d]deleteAllCookies\n",__func__,__LINE__);
}

#include "BELog.h"

void* BEViewGetFlashFullScreen()
{
    void* value = NULL;
#if ENABLE(TVGG_FLASH_FULLSCREEN)
    value = WebPage::getFlashFullScreen();
#endif
    //BELog(">>>>>> BEViewGetFlashFullScreen : (%p)\n",value);
    return value;
}

bool BEViewSetStereoScopicMode(BEStereoScopicMode mode)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine) {
        WebPage* page = 0;
        if (g_currentView)
           page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(g_currentView)->pageID());
        else
           page = WebProcess::shared().mainUIWebPage();

        if (page && page->getRenderer() && page->drawingArea()) {
            page->getRenderer()->setStereoScopicMode((StereoScopicMode)mode);
            page->drawingArea()->scheduleCompositingLayerSync();
            return true;
        }
    }
#endif
    return false;
}

bool BEViewSetStereoScopicSize(BEViewRef viewRef, int width, int height)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine) {
        WebPage* page = 0;
        if (viewRef)
           page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
        else
           page = WebProcess::shared().mainUIWebPage();

        if (page && page->getRenderer() && page->drawingArea()) {
            page->getRenderer()->setStereoScopicSize(IntSize(width, height));
            page->drawingArea()->scheduleCompositingLayerSync();
            return true;
        }
    }
#endif
    return false;
}

bool BEViewSetStereoScopicBufferSize(BEViewRef viewRef, int width, int height)
{
#if ENABLE(TVGG_ACCELERATED_COMPOSITING) && ENABLE(WIDGET_ENGINE)
    if (useWidgetEngine) {
        WebPage* page = 0;
        if (viewRef)
           page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
        else
           page = WebProcess::shared().mainUIWebPage();

        if (page && page->getUIViewWindow() && page->drawingArea()) {
            page->getUIViewWindow()->resizeView(IntSize(width, height));
            page->drawingArea()->scheduleCompositingLayerSync();
            return true;
        }
    }
#endif
    return false;
}

NPObjectRef BEViewWindowObject(BEViewRef viewRef)
{
#if ENABLE(TVGG_THREAD_MODEL)
    WebPage* page = NULL;
    if (!viewRef)
        return 0;

    page = WebProcess::shared().webPage(webkitWebViewBaseGetPage(toBEImpl(viewRef))->pageID());
    if (!page)
        return 0;

    Frame* frame = page->mainFrame()->coreFrame();

    if (!frame)
        return 0;

    NPObject* windowScriptObject = frame->script()->windowScriptNPObject();

    if (windowScriptObject)
        windowScriptObject->referenceCount++;

    return windowScriptObject;
#endif
    return 0;
}

bool BEViewIsH5VPlaying()
{
#if USE(UNIPLAYER) && ENABLE(TVGG_THREAD_MODEL)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    return (WebCore::MediaPlayerPrivate::isH5VPlaying() ? true : false);
#endif
    return false;
}

void BEViewPauseH5V()
{
#if USE(UNIPLAYER) && ENABLE(TVGG_THREAD_MODEL)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    WebCore::MediaPlayerPrivate::pauseH5V();
#endif
}
void BEViewResumeH5V()
{
#if USE(UNIPLAYER) && ENABLE(TVGG_THREAD_MODEL)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    WebCore::MediaPlayerPrivate::resumeH5V();
#endif
}

void BEViewStopH5V()
{
#if USE(UNIPLAYER) && ENABLE(TVGG_THREAD_MODEL)
    WebCore::MediaPlayerPrivate::stopH5V();
#endif
}
