#include "config.h"
#include "BEWidget.h"
#include "BEAPICast.h"
#include "WebKitWebViewBasePrivate.h"
#include "WebPageProxy.h"
#include <wtf/RefPtr.h>

using namespace WebKit;

BEViewRef BEWidgetViewCreate(BEContextRef contextRef, const char* widgetID)
{
#if ENABLE(TVGG_WIDGET)
    WebKitWebViewBase* view = webkitWebViewBaseCreateWithWidgetID(toBEImpl(contextRef), 0, String::fromUTF8(widgetID));
    GtkWidget* toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_name(toplevel, widgetID);
    gtk_container_add(GTK_CONTAINER(toplevel), GTK_WIDGET(view));
    gtk_window_set_default(GTK_WINDOW(toplevel), GTK_WIDGET(view));
    return toBEAPI(view);
#else
    return 0;
#endif
}

char* BEWidgetPageGetWidgetID(BEViewRef viewRef)
{
#if ENABLE(TVGG_WIDGET)
    return toCopiedBEAPI(webkitWebViewBaseGetWidgetID(toBEImpl(viewRef)));
#else
    return 0;
#endif
}

bool BEWidgetPageRunJSWidgetEvent(BEPageRef pageRef, const char* eventType, char* eventData, int keyCode)
{
#if ENABLE(TVGG_WIDGET)
    WebPageProxy* page = toBEImpl(pageRef);
    return page->runJSWidgetEvent(String::fromUTF8(eventType), String::fromUTF8(eventData), keyCode);
#else
    return false;
#endif
}

bool BEWidgetPageRunJSWidgetAppEvent(BEPageRef pageRef, const char* eventType, char* eventData, long type)
{
#if ENABLE(TVGG_WIDGET)
    WebPageProxy* page = toBEImpl(pageRef);
    return page->runJSWidgetAppEvent(String::fromUTF8(eventType), String::fromUTF8(eventData), type);
#else
    return false;
#endif
}
