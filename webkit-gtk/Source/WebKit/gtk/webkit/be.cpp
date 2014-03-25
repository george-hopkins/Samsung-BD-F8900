#include "config.h"
#include "ClientRect.h"
#include "RenderObject.h"
#include "RenderBoxModelObject.h"
#include "FrameView.h"
#include "NodeList.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "HTMLFrameElementBase.h"
#include "Text.h"
#include "CSSMutableStyleDeclaration.h"
#include "PlatformKeyboardEvent.h"
#include "PlatformMouseEvent.h"
#include "PlatformWheelEvent.h"
#include "NP_jsobject.h"
#include "KeyboardEvent.h"
#include "JSKeyboardEvent.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "RenderView.h"
#include "JSHTMLObjectElement.h"
#include "HTMLObjectElement.h"
#include "InputElement.h"
#include "RenderWidget.h"
#include "PluginView.h"
#include "c_instance.h"
#include "npruntime_impl.h"
#include "TransformState.h"
#include "IntRect.h"
#include "GraphicsContext.h"
#include "HostWindow.h"
#include "FocusController.h"
#include "Chrome.h"
#include "htmlediting.h"
#include "HTMLFormElement.h"
#include "HTMLFormControlElement.h"
#include "HTMLInputElement.h"
#include "HTMLTextAreaElement.h"
#include "WindowsKeyboardCodes.h"
#include "JSHTMLFormElement.h"
#include "PlatformString.h"
#include "PopupMenuClient.h"
#include "HTMLAreaElement.h"
#include "HTMLImageElement.h"
#include "PageGroup.h"
#include "GCController.h"
#include "Settings.h"
#include "DocumentLoader.h"
#include "RenderLayer.h"
#include "MemoryCache.h"
#include "PageCache.h"
#include "FontCache.h"
#include "CookieJarSoup.h"

#include "webkit/webkit.h"
#include "webkitwebviewprivate.h"
#include "webkitwebframeprivate.h"

#include "be.h"
#include "SpatialNaviNewEx.h"

#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <cairo/cairo-xlib.h>
#include <dirent.h>
#include <error.h>
#include <utime.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <webkit/webkit.h>
#include <glib/gstdio.h>
#include <string>

#if ENABLE(TVGG_BACKINGSTORE)
#include "BackingStoreGtk.h"
#endif

#include "MMManager.h"

#if USE(UNIPLAYER)
#include "MediaPlayerPrivateUniplayer.h"
#endif
#include <malloc.h>

#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
#include "soup-cache.h"
#include "ResourceHandle.h"
#endif


//#define SMOOTH_SCROLL

typedef struct _BERealView
{
	WebKitWebView*		webview;
	WebCore::PluginView* pluginview;

	void*				window;

	Drawable			drawable;
	Visual*				visual;
#ifdef GTK_API_VERSION_2
	GdkRegion*			exposeRegion;
#else
	cairo_region_t*		exposeRegion;
#endif
	GQueue*				scrollQueue;
	#ifdef SMART_FOCUS_ZOOM
	int nIsZoom;
	BERect prevBestNodeRect;
	#endif

	BECreateViewCallback createView;
	BEBasicCallback		closeView;

	BENavigationPolicyCallback decideNewWindowPolicy;
	BENavigationPolicyCallback decideNavigationPolicy;

	BEBlockPopupCallback	blockPopup;

	BELoadStatusCallback	loadCommitted;
	BELoadStatusCallback	loadFinished;
	BEBasicCallback		loadProgress;
	BELoadErrorCallback	loadError;

	BEBasicCallback		titleChanged;

	BEShowPopupMenuCallback	showPopupMenu;
	BEBasicCallback		hidePopupMenu;

	BEScriptAlertCallback	alert;
	BEScriptConfirmCallback	confirm;
	BEScriptPromptCallback	prompt;
	BEConsoleMessageCallback	consoleMsg;

	BEBasicCallback		printReq;
	BEIMEFocusCallback	imeFocus;
#if ENABLE(TVGG_FLASH_IME)
	BEBasicCallback		showIME;
#endif
	BEBasicCallback		iconLoaded;
	BEBasicCallback		endDownload;
	BEBasicCallback  	notifyDownloadRequest;
#if ENABLE(TVGG_TRANSLATED_ALERT) //@20110126-sanggyu
	BEScriptTranslatedAlertCallback translatedAlert;
#endif

	void*				callbackData;

	unsigned			lastButtonType;

	unsigned			timerID;

	BEDocument			preFocusedDoc;
	BEDocument			currentFocusedDoc;
	WebKitDownload*		download;

	cairo_surface_t*	snapshot;
	float				snapshotRatio;
	int					snapshotRef;
} BERealView;

typedef struct _BEScrollData
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	offsetX;
	int	offsetY;
#ifdef SMOOTH_SCROLL
	int	remainX;
	int remainY;
#endif
} BEScrollData;

typedef struct _BEKeyEvent
{
	BERealView*		view;
	unsigned int	key;
	bool			isPress;
} BEKeyEvent;

typedef struct _BESnapshotList
{
	time_t	access;
	unsigned long size;
	char*	filename;
	struct _BESnapshotList* next;
} BESnapshotList;

typedef struct _BELodingAnimationToolKit {
	GtkWidget* mainWindow;	// target window to draw
	cairo_surface_t* cairoSurface;
	float red;
	float green;
	float blue;
	bool shouldDestroy;
} BELoadingAnimationToolKit;


#define SNAPSHOT_PATH	SEC_TV_HOME"/etc/WebkitPlugin/snapshots/"
#define DOWNLOAD_PATH	"file://"SEC_TV_HOME"/etc/WebkitPlugin/"

static unsigned int g_snapshotStorageSize = (5 * 1024 * 1024);	// 5mb

static GtkWidget* g_uiView;
static time_t g_nBEBaseTime = 0;
static unsigned long g_snapshotSize;
static bool g_drawElementFocus = false;

// mouse point
static int s_nX = 0;
static int s_nY = 0;
static bool s_bAllowMouseEvent = false;
static bool s_bContentViewFocused = false;
static int s_nPointerSpeed =  30; // normal

static BEIMEStatus imeStatus = BEIMEHide;
static BENode imeNode = NULL;

#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
WebKitSoupCache* webkit_soup_cache = NULL;
#endif

#if ENABLE(TVGG_CACHE_BOUNDARY)
BEBasicCallback cacheOutOfSpaceCallBack = NULL;
#endif

#if 0
#define BEPRINT(args...) printf(args)
#else
#define BEPRINT(args...)
#endif

static char* copyCString(WTF::CString str)
{
	unsigned len = str.length();
	char* charStr = (char*) malloc(len + 1);
	if(charStr == NULL)
		return NULL;
	memcpy(charStr, str.data(), len);
	charStr[len] = '\0';
	return charStr;
}

static inline char* skipSpace(char* iter)
{
	while(*iter)
	{
		if(!isspace(*iter))
		{
			break;
		}
		++iter;
	}
	return iter;
}

static char* getVersion(const char* path)
{
	FILE* file = fopen(path, "r");
	if(!file)
	{
		return NULL;
	}
	char buff[1024];
	if(fread(buff, sizeof(char), 1024, file) == 0)
	{
		fclose(file);
		return NULL;
	}
	fclose(file);

	char* iter = skipSpace(buff);
	if(strncmp(iter, "version", 7))
	{
		return NULL;
	}
	iter = skipSpace(iter + 7);
	if(*iter != ':')
	{
		return NULL;
	}
	iter = skipSpace(iter + 1);

	char* end = iter;
	while(*end)
	{
		if(isspace(*end))
		{
			*end = '\0';
			break;
		}
		++end;
	}

	char* version = (char*) malloc(strlen(iter) + 1);
	if(!version)
	{
		return NULL;
	}
	strcpy(version, iter);
	return version;
}

void BE_ResizeBackingStoreRect(int width, int height)
{
#if ENABLE(TVGG_BACKINGSTORE)
	WebKit::BackingStore::getInstance()->resize(width, height);
#endif
}

const char* BE_GetVersion(void)
{
	static char* version;
	if(version)
	{
		return version;
	}

	if(!(version = getVersion("/mtd_down/widgets/normal/20111000001/bin/version.txt")))
	{
		version = getVersion("/mtd_rocommon/FullBrowser/bin/version.txt");
	}
	return version;
}

#define INIT_JS_GLOBAL_DATA(g, p)	\
	JSC::JSGlobalData* g = WebCore::JSDOMWindowBase::commonJSGlobalData();	\
	JSC::JSGlobalObject* p = g->dynamicGlobalObject;	\
	g->dynamicGlobalObject = NULL;

#define RESTORE_JS_GLOBAL_DATA(g, p)	\
	g->dynamicGlobalObject = p;

static WebKitWebView* createView(WebKitWebView* view, WebKitWebFrame* frame, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->createView)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		BEView result = realView->createView(realView->callbackData);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);

		if(result)
		{
#ifdef SMART_FOCUS_ZOOM
			realView->nIsZoom = 0;
#endif
			WebCore::Page* page = WebKit::core(((BERealView*)result)->webview);
			page->setGroupName(WebKit::core(realView->webview)->groupName());
			return ((BERealView*)result)->webview;
		}
	}

	return NULL;
}

static bool closeView(WebKitWebView* view, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->closeView)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->closeView(realView->callbackData);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}

	return true;
}

static bool decideNewWindowPolicy(WebKitWebView* view, WebKitWebFrame* frame,
		WebKitNetworkRequest* request, WebKitWebNavigationAction* action,
		WebKitWebPolicyDecision* decision, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(!realView->decideNewWindowPolicy)
	{
		return false;
	}

	INIT_JS_GLOBAL_DATA(globalData, prevData);

	switch(realView->decideNewWindowPolicy(realView->callbackData,
			webkit_network_request_get_uri(request),
			(BENavigationReason) webkit_web_navigation_action_get_reason(action),
			webkit_web_view_get_main_frame(view) == frame))
	{
	case BENavigationUse:
		webkit_web_policy_decision_use(decision);
		break;
	case BENavigationDownload:
		webkit_web_policy_decision_download(decision);
		break;
	default:
		webkit_web_policy_decision_ignore(decision);
		break;
	}

	RESTORE_JS_GLOBAL_DATA(globalData, prevData);

	return true;
}

static bool decideNavigationPolicy(WebKitWebView* view, WebKitWebFrame* frame,
		WebKitNetworkRequest* request, WebKitWebNavigationAction* action,
		WebKitWebPolicyDecision* decision, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(!realView->decideNavigationPolicy)
		return false;

	INIT_JS_GLOBAL_DATA(globalData, prevData);

	switch(realView->decideNavigationPolicy(realView->callbackData,
			webkit_network_request_get_uri(request),
			(BENavigationReason) webkit_web_navigation_action_get_reason(action),
			webkit_web_view_get_main_frame(view) == frame))
	{
	case BENavigationUse:
		webkit_web_policy_decision_use(decision);
		break;
	case BENavigationDownload:
		webkit_web_policy_decision_download(decision);
		break;
	default:
		webkit_web_policy_decision_ignore(decision);
		break;
	}

	RESTORE_JS_GLOBAL_DATA(globalData, prevData);

	return true;
}

static void blockPopup(WebKitWebView* view, const char* url, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->blockPopup)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->blockPopup(realView->callbackData, url);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
}

static void notifyLoadStatus(WebKitWebFrame* frame, GParamSpec* pspec, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	WebKitWebFrame* mainFrame = webkit_web_view_get_main_frame(realView->webview);
	WebKitLoadStatus status = webkit_web_frame_get_load_status(mainFrame);

	switch(webkit_web_frame_get_load_status(frame))
	{
	case WEBKIT_LOAD_COMMITTED:
		if(realView->loadCommitted)
		{
			realView->loadCommitted(realView->callbackData, mainFrame == frame,
				status != WEBKIT_LOAD_FINISHED && status != WEBKIT_LOAD_FAILED);
		}
		break;

	case WEBKIT_LOAD_FINISHED:
		if(!WebKit::core(realView->webview)->focusController()->focusedFrame())
			WebKit::core(mainFrame)->view()->setScrollPosition(WebCore::IntPoint());

		if(realView->loadFinished)
		{
			realView->loadFinished(realView->callbackData, mainFrame == frame,
				status != WEBKIT_LOAD_FINISHED && status != WEBKIT_LOAD_FAILED);
		}
		break;

	default:
		break;
	}
}

static void frameCreated(WebKitWebView* view, WebKitWebFrame* frame, gpointer data)
{
	g_signal_connect(G_OBJECT(frame), "notify::load-status", G_CALLBACK(notifyLoadStatus), data);
}

static void notifyProgress(WebKitWebView* view, GParamSpec* pspec, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->loadProgress)
	{
		realView->loadProgress(realView->callbackData);
	}
}

static gboolean loadError(WebKitWebView* view, WebKitWebFrame* frame, const char* uri, GError* error, gpointer data)
{
	BERealView* realView = (BERealView*) data;
    WebCore::FrameView* frameview = WebKit::core(frame)->view();
	char* content;

	if(!realView->loadError || !frameview || !(content = realView->loadError(realView->callbackData, uri, error->code,
			frameview->contentsWidth(), frameview->contentsHeight(), error->message, webkit_web_view_get_main_frame(view) == frame)))
	{
		return FALSE;
	}

	webkit_web_frame_load_alternate_string(frame, content, uri, uri);
	free(content);
	return TRUE;
}

static void notifyTitle(WebKitWebView* view, GParamSpec* pspec, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->titleChanged)
	{
		realView->titleChanged(realView->callbackData);
	}
}

static void exposeView(WebKitWebView* view, gpointer updateRect, gpointer offset, gpointer data)
{
#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		return;
	}
#endif

	BERealView* realView = (BERealView*) data;
	if(!realView->pluginview || !updateRect)
		return;

	WebCore::IntRect* r = (WebCore::IntRect*) updateRect;
	if(r->isEmpty())
		return;

#ifdef GTK_API_VERSION_2
	GdkRectangle rect = { r->x(), r->y(), r->width(), r->height() };
#else
	cairo_rectangle_int_t rect = { r->x(), r->y(), r->width(), r->height() };
#endif
	WebCore::IntSize* o = (WebCore::IntSize*) offset;

	if(!o || (o->width() == 0 && o->height() == 0))
	{
		if(!realView->exposeRegion)
#ifdef GTK_API_VERSION_2
			realView->exposeRegion = gdk_region_rectangle(&rect);
#else
			realView->exposeRegion = cairo_region_create_rectangle(&rect);
#endif
		else
#ifdef GTK_API_VERSION_2
			gdk_region_union_with_rect(realView->exposeRegion, &rect);
#else
			cairo_region_union_rectangle(realView->exposeRegion, &rect);
#endif
	}
	else
	{
		GList* iter = g_queue_peek_head_link(realView->scrollQueue);
		bool hasSame = false;

		while(iter)
		{
			BEScrollData* scrollData = (BEScrollData*) iter->data;
			if(scrollData->x == rect.x && scrollData->y == rect.y
					&& scrollData->width == rect.width && scrollData->height == rect.height)
			{
				hasSame = true;
				scrollData->offsetX += o->width();
				scrollData->offsetY += o->height();
#ifdef SMOOTH_SCROLL
				scrollData->remainX += o->width();
				scrollData->remainY += o->height();
#endif
			}
			else if(scrollData->x >= rect.x && scrollData->y >= rect.y
					&& scrollData->width <= rect.width && scrollData->height <= rect.height)
			{
				scrollData->x += o->width();
				scrollData->y += o->height();
			}

			iter = g_list_next(iter);
		}

		if(!hasSame)
		{
			BEScrollData* scrollData = (BEScrollData*) malloc(sizeof(BEScrollData));
			if(scrollData)
			{
				scrollData->x = rect.x;
				scrollData->y = rect.y;
				scrollData->width = rect.width;
				scrollData->height = rect.height;
				scrollData->offsetX = o->width();
				scrollData->offsetY = o->height();
#ifdef SMOOTH_SCROLL
				scrollData->remainX = o->width();
				scrollData->remainY = o->height();
#endif
				g_queue_push_tail(realView->scrollQueue, (gpointer) scrollData);
			}
		}

		if(realView->exposeRegion)
		{
#ifdef GTK_API_VERSION_2
			GdkRegion* region = gdk_region_rectangle(&rect);
			gdk_region_union(region, realView->exposeRegion);
			if(!gdk_region_empty(region))
			{
				gdk_region_subtract(realView->exposeRegion, region);
				gdk_region_offset(region, o->width(), o->height());
				gdk_region_union(realView->exposeRegion, region);
			}
			gdk_region_destroy(region);
#else
			cairo_region_t* region = cairo_region_create_rectangle(&rect);
			cairo_region_union(region, realView->exposeRegion);
			if(!cairo_region_is_empty(region))
			{
				cairo_region_subtract(realView->exposeRegion, region);
				cairo_region_translate(region, o->width(), o->height());
				cairo_region_union(realView->exposeRegion, region);
			}
			cairo_region_destroy(region);
#endif
		}
	}

	realView->pluginview->invalidateRect(*r);
}

static void showPopupMenu(WebKitWebView* view, gpointer clientData, gpointer rectData, gpointer selectedData, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	if(realView->showPopupMenu)
	{
		WebCore::IntRect* rect = (WebCore::IntRect*) rectData;
		realView->showPopupMenu(realView->callbackData, clientData,
			rect->x(), rect->y(), rect->width(), rect->height(), *((int*)selectedData));
	}
}

static void hidePopupMenu(WebKitWebView* view, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->hidePopupMenu)
	{
		realView->hidePopupMenu(realView->callbackData);
	}
}

static void getNodeRect(BENode node, bool isAbsolute, BERect* rect, Vector<WebCore::IntRect>* rects)
{
	if(!node)
	{
		if(rect)
		{
			memset(rect, 0, sizeof(BERect));
		}
		return;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	realNode->document()->updateLayoutIgnorePendingStylesheets();
	WebCore::RenderObject* renderObject;
	WebCore::IntRect result;

	if(realNode->hasTagName(WebCore::HTMLNames::areaTag))
	{
		WebCore::HTMLImageElement* img = ((WebCore::HTMLAreaElement*)realNode)->imageElement();
		if(!img || !(renderObject = img->renderer()))
		{
			if(rect)
			{
				memset(rect, 0, sizeof(BERect));
			}
			return;
		}
		WebCore::Path path = ((WebCore::HTMLAreaElement*)realNode)->computePath(renderObject);
		WebCore::FloatRect bound = path.boundingRect();
		float zoom = renderObject->style()->effectiveZoom();
		if(zoom != 1.0f)
		{
			WebCore::FloatPoint absolutePoint = renderObject->localToAbsolute();
			bound.move(-absolutePoint.x(), -absolutePoint.y());
			bound.scale(zoom);
			bound.move(absolutePoint.x(), absolutePoint.y());
		}
		result = WebCore::IntRect(bound);

		if(rects)
		{
			(*rects).append(result);
		}
	}
	else
	{
		WebCore::RenderBoxModelObject* renderBoxModelObject = realNode->renderBoxModelObject();
		if(!renderBoxModelObject)
		{
			if(rect)
			{
				memset(rect, 0, sizeof(BERect));
			}
			return;
		}

		WebCore::FloatPoint absolutePoint = renderBoxModelObject->localToAbsolute();
		Vector<WebCore::IntRect> quads;
		renderBoxModelObject->addFocusRingRects(quads, absolutePoint.x(), absolutePoint.y());
		if(quads.isEmpty())
		{
			if(rect)
			{
				memset(rect, 0, sizeof(BERect));
			}
			return;
		}

		for(size_t i = 0; i < quads.size(); ++i)
		{
			if(quads[i].maxX() > 0 && quads[i].maxY() > 0)
			{
				result.unite(quads[i]);
			}
		}
		renderObject = realNode->renderer();

		if(rects)
		{
			*rects = quads;
		}
	}

	WebCore::RenderBlock* container = renderObject->containingBlock();
	if(container)
	{
		if(container->hasOverflowClip())
		{
			WebCore::IntRect containerRect = container->visualOverflowRect();
			if(!containerRect.isEmpty())
			{
				WebCore::FloatPoint absolutePoint = container->localToAbsolute();
				containerRect.move((int) absolutePoint.x(), (int) absolutePoint.y());

				result.intersect(containerRect);
				if(rects)
				{
					for(size_t i = 0; i < (*rects).size(); ++i)
					{
						(*rects)[i].intersect(containerRect);
					}
				}
			}
		}
	}

	if(isAbsolute)
	{
		WebCore::ScrollView* view = realNode->document()->view();
		while(view)
		{
			WebCore::IntPoint offset = view->frameRect().location();
			if(view->parent())
			{
				offset -= view->scrollOffset();
			}

			result.move(offset.x(), offset.y());
			if(rects)
			{
				for(size_t i = 0; i < (*rects).size(); ++i)
				{
					(*rects)[i].move(offset.x(), offset.y());
				}
			}

			view = view->parent();
		}
	}

	if(rect)
	{
		rect->left = (long) result.x();
		rect->top = (long) result.y();
		rect->width = (long) result.width();
		rect->height = (long) result.height();
	}
}

static void setElementFocus(WebKitWebView* view, gpointer nodeData, gpointer data)
{
	if(!nodeData || !g_drawElementFocus)
	{
		return;
	}

#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
	WebCore::Node* node = (WebCore::Node*) nodeData;
	WebCore::FrameView* frameview = node->document()->view();
	if(frameview && WebKit::BackingStore::isAvailable() &&
			WEBKIT_WEB_VIEW(frameview->hostWindow()->platformPageClient()) == WebKit::BackingStore::getInstance()->webView())
	{
		Vector<WebCore::IntRect> rects;
		if(node->document()->focusedNode() == node)
		{
			getNodeRect(node, true, NULL, &rects);
		}
		WebKit::BackingStore::getInstance()->setFocusRect(rects);
	}
#endif
}

static void getScreenPosition(WebKitWebView* view, gpointer xData, gpointer yData, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	int* x = (int*) xData;
	int* y = (int*) yData;

	GtkWidget* parent = realView->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
	int originX, originY;
	gdk_window_get_origin(gtk_widget_get_window(parent), &originX, &originY);

	WebCore::FrameView* frameview = static_cast<WebCore::FrameView*>(realView->pluginview->parent());
	if(frameview)
	{
		WebCore::IntPoint offset = frameview->contentsToWindow(realView->pluginview->frameRect().location());

		*x = originX + offset.x();
		*y = originY + offset.y();
	}
}

static gboolean scriptAlert(WebKitWebView* webView, WebKitWebFrame* frame, const gchar* message, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->alert)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->alert(realView->callbackData, message);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}

static gboolean scriptConfirm(WebKitWebView* webView, WebKitWebFrame* frame, const gchar* message, gboolean* didConfirm, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->confirm)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		*didConfirm = realView->confirm(realView->callbackData, message);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}

static gboolean scriptPrompt(WebKitWebView* webView, WebKitWebFrame* frame, const gchar* message,
							const gchar* defaultValue, gchar** value, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->confirm)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		*value = realView->prompt(realView->callbackData, message, defaultValue);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}

static gboolean consoleMessage(WebKitWebView* webView, const gchar* message, unsigned int line,
							const gchar* sourceId, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->consoleMsg)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->consoleMsg(realView->callbackData, message, line, sourceId);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}

static gboolean printRequested(WebKitWebView* webView, WebKitWebFrame* frame, gpointer data)
{
	// to do : if support to print, than refer to webkit_web_frame_print_full()
	BERealView* realView = (BERealView*) data;
	if(realView->printReq)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->printReq(realView->callbackData);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}

static void iconLoaded(WebKitWebView* view, gchar* iconUri, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->iconLoaded)
	{
		realView->iconLoaded(realView->callbackData);
	}
}

static gboolean mimeTypeDecision(WebKitWebView* webView, WebKitWebFrame* frame, WebKitNetworkRequest* request,
		gchar* mimetype, WebKitWebPolicyDecision* policyDecision, gpointer data)
{
	if(webkit_web_view_can_show_mime_type(WEBKIT_WEB_VIEW(webView), mimetype))
	{
		return FALSE;
	}
	webkit_web_policy_decision_download(policyDecision);
	return TRUE;
}

static gboolean downloadRequested(WebKitWebView* webView, WebKitDownload* download, gpointer data)
{
	BERealView* realView = (BERealView*) data;

	const char* filename = webkit_download_get_suggested_filename(download);
	if(!filename)
	{
		return FALSE;
	}

	realView->download = download;
	if(realView->notifyDownloadRequest)
	{
		realView->notifyDownloadRequest(realView->callbackData);
	}
	return TRUE;
}

#if ENABLE(TVGG_TRANSLATED_ALERT)
/* @20110126-sanggyu */
static gboolean scriptTranslatedAlert(WebKitWebView* webView, WebKitWebFrame* frame, const gchar* message, gpointer data)
{
	//fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
	BERealView* realView = (BERealView*) data;
	if (realView && realView->translatedAlert)
	{
		INIT_JS_GLOBAL_DATA(globalData, prevData);
		realView->translatedAlert(realView->callbackData, message);
		RESTORE_JS_GLOBAL_DATA(globalData, prevData);
	}
	return TRUE;
}
#endif

void imeFocusIn(GtkIMContext* context)
{
	BERealView* realView = (BERealView*) g_object_get_data(G_OBJECT(context), "v");
	if(realView && realView->imeFocus)
	{
		WebCore::Frame* frame = WebKit::core(realView->webview)->focusController()->focusedOrMainFrame();
		if(!frame)
			return;

		WebCore::Node* node = frame->document()->focusedNode();
		if(node && node->isElementNode() && ((WebCore::Element*) node)->isFormControlElement()
				&& ((WebCore::HTMLFormControlElement*) node)->isReadOnlyFormControl())
			return;

		realView->imeFocus(realView->callbackData, true);
	}
}

void imeFocusOut(GtkIMContext* context)
{
	BERealView* realView = (BERealView*) g_object_get_data(G_OBJECT(context), "v");
	if(realView && realView->imeFocus)
	{
		realView->imeFocus(realView->callbackData, false);
	}
}

#if ENABLE(TVGG_FLASH_IME)
static void showIME(WebKitWebView* view, gpointer data)
{
	BERealView* realView = (BERealView*) data;
	if(realView->showIME)
	{
		realView->showIME(realView->callbackData);
	}
}
#endif

//#define _SEF_TEST_
#ifdef _SEF_TEST_

#include "SefCommon.h"
#include "CSefPluginWrapper.h"

#define PL_SEF_VERSION "1.0"

using namespace std;

namespace sef
{

class CSefPluginImpl:public CSefPluginWrapper
{
protected:
	void* m_npp;
public:
	CSefPluginImpl(void* npp);
	virtual bool OnEvent(string eventType, string param1, string param2);
};

CSefPluginImpl::CSefPluginImpl(void* npp)
    : CSefPluginWrapper(sef::CClientAppFactory::SEF_CLIENT_WEBKIT)
{
	m_npp = npp;
}

bool CSefPluginImpl::OnEvent(string eventType, string param1, string param2)
{
	return true;
}
}

using namespace sef;

void sefTest()
{
	printf("[SEF TEST] start\n");
	static int npp = 0;
	CSefPluginImpl* newImpl = new CSefPluginImpl((void*)&npp);
	newImpl->Open(string("NNavi"), string(PL_SEF_VERSION), string("none"));	// T9 -> test
	string strResult = newImpl->Execute(string("GetFirmware"), string(""), string(""), string(""), string(""));
	const char* tempResult = strResult.c_str();

	printf("[SEF TEST] return value ");
	switch(tempResult[0])
	{
		case 'A':	// string
			printf("string(%s)\n", &tempResult[1]);
			break;
		case 'U':	// not implemented
			printf("not implemented(%s)\n", &tempResult[1]);
			break;
		case 'I':	// number : int
			printf("number(%d)\n", atoi(&tempResult[1]));
			break;
		default:
			printf("undefined(%s)\n", tempResult);
			break;
	}
}
#endif //_SEF_TEST_

static void initialize()
{
	SoupSession* session = WebCore::ResourceHandle::defaultSession();
	if(session)
	{
		g_object_set(G_OBJECT(session), SOUP_SESSION_TIMEOUT, 30, NULL);
	}
}

BEView BE_CreateView(NPP npp)
{
	static unsigned viewID;

	WebCore::PluginView* pluginview;
	if(npp && npp->ndata)
	{
		pluginview = static_cast<WebCore::PluginView*>(npp->ndata);
	}
	else
	{
		pluginview = WebCore::PluginView::currentPluginView();
	}

	GtkWidget* parent = pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
	if(!g_uiView)
	{
		g_uiView = parent;
		initialize();
	}
	else
	{
		if(g_uiView != parent)
		{
			return NULL;
		}
	}

	BERealView* realView = (BERealView*) malloc(sizeof(BERealView));
	if(!realView)
	{
		return NULL;
	}
	memset(realView, 0, sizeof(BERealView));

#ifdef _SEF_TEST_
	sefTest();
#endif //_SEF_TEST_

	WebKitWebView* webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
//	WebKitWebViewClass* webviewClass = WEBKIT_WEB_VIEW_GET_CLASS(webview);

	g_signal_connect(G_OBJECT(webview), "create-web-view", G_CALLBACK(createView), realView);
	g_signal_connect(G_OBJECT(webview), "close-web-view", G_CALLBACK(closeView), realView);

	g_signal_connect(G_OBJECT(webview), "new-window-policy-decision-requested", G_CALLBACK(decideNewWindowPolicy), realView);
	g_signal_connect(G_OBJECT(webview), "navigation-policy-decision-requested", G_CALLBACK(decideNavigationPolicy), realView);

	g_signal_connect(G_OBJECT(webview), "frame-created", G_CALLBACK(frameCreated), realView);
	g_signal_connect(G_OBJECT(webview), "notify-block-popup", G_CALLBACK(blockPopup), realView);

	g_signal_connect(G_OBJECT(webkit_web_view_get_main_frame(webview)), "notify::load-status", G_CALLBACK(notifyLoadStatus), realView);
	g_signal_connect(G_OBJECT(webview), "notify::progress", G_CALLBACK(notifyProgress), realView);
	g_signal_connect(G_OBJECT(webview), "load-error", G_CALLBACK(loadError), realView);
	g_signal_connect(G_OBJECT(webview), "notify::title", G_CALLBACK(notifyTitle), realView);
	g_signal_connect(G_OBJECT(webview), "invalidate-web-view", G_CALLBACK(exposeView), realView);
	g_signal_connect(G_OBJECT(webview), "show-popup-menu", G_CALLBACK(showPopupMenu), realView);
	g_signal_connect(G_OBJECT(webview), "hide-popup-menu", G_CALLBACK(hidePopupMenu), realView);
	g_signal_connect(G_OBJECT(webview), "set-element-focus", G_CALLBACK(setElementFocus), realView);
	g_signal_connect(G_OBJECT(webview), "get-screen-position", G_CALLBACK(getScreenPosition), realView);

	g_signal_connect(G_OBJECT(webview), "script-alert", G_CALLBACK(scriptAlert), realView);
	g_signal_connect(G_OBJECT(webview), "script-confirm", G_CALLBACK(scriptConfirm), realView);
	g_signal_connect(G_OBJECT(webview), "script-prompt", G_CALLBACK(scriptPrompt), realView);
	g_signal_connect(G_OBJECT(webview), "console-message", G_CALLBACK(consoleMessage), realView);
	g_signal_connect(G_OBJECT(webview), "print-requested", G_CALLBACK(printRequested), realView);

	g_signal_connect(G_OBJECT(webview), "icon-loaded", G_CALLBACK(iconLoaded), realView);
	g_signal_connect(G_OBJECT(webview), "mime-type-policy-decision-requested", G_CALLBACK(mimeTypeDecision), realView);
	g_signal_connect(G_OBJECT(webview), "download-requested", G_CALLBACK(downloadRequested), realView);

#if ENABLE(TVGG_FLASH_IME)
	g_signal_connect(G_OBJECT(webview), "show-ime", G_CALLBACK(showIME), realView);
#endif

#if ENABLE(TVGG_TRANSLATED_ALERT)    /* @20110126-sanggyu */
	g_signal_connect(G_OBJECT(webview), "script-translated-alert", G_CALLBACK(scriptTranslatedAlert), realView);
#endif

	webkit_web_view_set_full_content_zoom(webview, true);

	GtkIMContext* ime = webview->priv->imContext.get();
	g_object_set_data(G_OBJECT(ime), "v", realView);
	GTK_IM_CONTEXT_GET_CLASS(ime)->focus_in = imeFocusIn;
	GTK_IM_CONTEXT_GET_CLASS(ime)->focus_out = imeFocusOut;

	g_object_ref_sink(webview);

	realView->webview = webview;
	realView->scrollQueue = g_queue_new();
	realView->pluginview = pluginview;

	char name[1024];
	snprintf(name, 1024, "%u", viewID++);
	WebCore::Page* page = WebKit::core(webview);
	page->setGroupName(name);

	WebCore::Settings* settings = WebKit::core(webview)->settings();
	settings->setUseNonGenericFontFamily(false);

	BE_SetFontFamily(realView, "sans-serif");

	// [shivamidow]
	// FIXME : Alternative option
	//if (!GTK_WIDGET(webview)->parent)
	//	GTK_WIDGET(webview)->parent = parent;

	return (BEView) realView;
}

void BE_DestroyView(BEView view)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;

#if ENABLE(TVGG_BACKINGSTORE)
	if(WEBKIT_WEB_VIEW(realView->webview) == WebKit::BackingStore::getInstance()->webView())
	{
		webkit_backingstore_change_web_view(NULL);
	}
#endif

	if(realView->snapshot)
		cairo_surface_destroy(realView->snapshot);

	if(realView->exposeRegion)
#ifdef GTK_API_VERSION_2
		gdk_region_destroy(realView->exposeRegion);
#else
		cairo_region_destroy(realView->exposeRegion);
#endif
	g_queue_free(realView->scrollQueue);

	GtkWidget* webview = GTK_WIDGET(realView->webview);

	g_object_unref(webview);

	free(view);

	malloc_trim(0);
}

BEResult BE_SetWindow(BEView view, NPWindow* window)
{
	if(!view || !window)
		return BEFail;

	BERealView* realView = (BERealView*) view;
	WebKitWebView* webview = realView->webview;
	WebCore::PluginView* pluginview = realView->pluginview;

	if(window->type == NPWindowTypeDrawable)
	{
		if(window->ws_info)
			realView->visual = ((NPSetWindowCallbackStruct*) window->ws_info)->visual;
		else
			realView->visual = NULL;

#if ENABLE(TVGG_BACKINGSTORE)
		if(WebKit::BackingStore::isAvailable())
		{
			WebKit::BackingStore::getInstance()->setPluginViewRect(pluginview->frameRect());
		}
#endif
		if (pluginview->frameRect().x())
			return BEPass;

		GtkAllocation allocation = { window->x, window->y, window->width, window->height };
		gtk_widget_size_allocate(GTK_WIDGET(webview), &allocation); // this line cause to call exposeView().
	}
	else
	{
		if(window->window && realView->window != window->window)
		{
			gtk_widget_set_size_request(GTK_WIDGET(webview), window->width, window->height);

#ifdef GTK_API_VERSION_2
			GtkWidget* plug = gtk_plug_new((GdkNativeWindow) window->window);
			gtk_container_add(GTK_CONTAINER(plug), GTK_WIDGET(webview));
			gtk_widget_show_all(plug);
#endif
		}

		realView->window = window->window;
	}

	return BEPass;
}

#ifdef GTK_API_VERSION_2
static void paintRegion(WebCore::GraphicsContext* ctx, WebCore::FrameView* frameview,
						 GdkRegion* region, int offsetX, int offsetY)
{
	WTF::GOwnPtr<GdkRectangle> rects;
	int rectCount;
	gdk_region_get_rectangles(region, &rects.outPtr(), &rectCount);
	GdkRectangle* list = rects.get();

	for(int i = 0; i < rectCount; ++i)
	{
		ctx->save();

		if(offsetX || offsetY)
			ctx->translate(offsetX, offsetY);

		WebCore::IntRect rect = list[i];
		ctx->clip(rect);
//		if(priv->transparent)
//			ctx->clearRect(rect);
#ifdef SMOOTH_SCROLL
		WebCore::IntRect documentDirtyRect = rect;
//		documentDirtyRect.intersect(frameview->frameRect());

		ctx->translate(frameview->x(), frameview->y());
		documentDirtyRect.move(-frameview->x(), -frameview->y());

		ctx->translate(-frameview->scrollX(), -frameview->scrollY());
		documentDirtyRect.move(frameview->scrollX(), frameview->scrollY());

//		ctx->clip(frameview->visibleContentRect());

		frameview->paintContents(ctx, documentDirtyRect);
#else
		frameview->paint(ctx, rect);
#endif

		ctx->restore();
	}
}

static void copyRegion(XGraphicsExposeEvent* exposeEvent, Drawable drawable, GC* gc, GdkRegion* region, int offsetX, int offsetY)
{
	WTF::GOwnPtr<GdkRectangle> rects;
	int rectCount;
	gdk_region_get_rectangles(region, &rects.outPtr(), &rectCount);

	for(int i = 0; i < rectCount; ++i)
	{
		GdkRectangle* rect = rects.get() + i;
		XCopyArea(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), drawable, drawable, *gc,
			rect->x, rect->y, rect->width, rect->height,
			rect->x + offsetX, rect->y + offsetY);
	}
}
#else
static void paintRegion(WebCore::GraphicsContext* ctx, WebCore::FrameView* frameview,
						 cairo_region_t* region, int offsetX, int offsetY)
{
	int rectCount = cairo_region_num_rectangles(region);

	for(int i = 0; i < rectCount; ++i)
	{
		ctx->save();

		if(offsetX || offsetY)
			ctx->translate(offsetX, offsetY);

		cairo_rectangle_int_t cairoRect;
		cairo_region_get_rectangle(region, i, &cairoRect);
		WebCore::IntRect rect = cairoRect;
		ctx->clip(rect);
//		if(priv->transparent)
//			ctx->clearRect(rect);
#ifdef SMOOTH_SCROLL
		WebCore::IntRect documentDirtyRect = rect;
//		documentDirtyRect.intersect(frameview->frameRect());

		ctx->translate(frameview->x(), frameview->y());
		documentDirtyRect.move(-frameview->x(), -frameview->y());

		ctx->translate(-frameview->scrollX(), -frameview->scrollY());
		documentDirtyRect.move(frameview->scrollX(), frameview->scrollY());

//		ctx->clip(frameview->visibleContentRect());

		frameview->paintContents(ctx, documentDirtyRect);
#else
		frameview->paint(ctx, rect);
#endif

		ctx->restore();
	}
}

static void copyRegion(XGraphicsExposeEvent* exposeEvent, Drawable drawable, GC* gc, cairo_region_t* region, int offsetX, int offsetY)
{
	int rectCount = cairo_region_num_rectangles(region);

	for(int i = 0; i < rectCount; ++i)
	{
		cairo_rectangle_int_t rect;
		cairo_region_get_rectangle(region, i, &rect);
		XCopyArea(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), drawable, drawable, *gc,
			rect.x, rect.y, rect.width, rect.height,
			rect.x + offsetX, rect.y + offsetY);
	}
}
#endif

#ifdef SMOOTH_SCROLL
static int getScrollOffset(int remain)
{
	if(remain < 0)
	{
		if(remain > -7)
			return remain;
		else if(remain > -20)
			return -5;
		else if(remain > -50)
			return -10;
		else if(remain > -100)
			return -30;
		else
			return -50;
	}
	else
	{
		if(remain < 7)
			return remain;
		else if(remain < 20)
			return 5;
		else if(remain < 50)
			return 10;
		else if(remain < 100)
			return 30;
		else
			return 50;
	}
}
#endif

#ifdef GTK_API_VERSION_2
static void paintView(BERealView* realView, XGraphicsExposeEvent* exposeEvent)
{
	GdkRectangle eventRect = { exposeEvent->x, exposeEvent->y, exposeEvent->width, exposeEvent->height };

	if(realView->drawable != exposeEvent->drawable)
	{
		if(!realView->exposeRegion)
			realView->exposeRegion = gdk_region_rectangle(&eventRect);
		else
			gdk_region_union_with_rect(realView->exposeRegion, &eventRect);

		realView->drawable = exposeEvent->drawable;
	}

	if(!realView->exposeRegion && g_queue_is_empty(realView->scrollQueue))
	{
		return;
	}

	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	WebCore::FrameView* frameview = frame->view();

	if(!frame->contentRenderer() || !frameview)
	{
		return;
	}

	Drawable drawable;
	WebCore::GraphicsContext* ctx;

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx = (WebCore::GraphicsContext*) exposeEvent->drawable;
		drawable = cairo_xlib_surface_get_drawable(cairo_get_target(ctx->platformContext()));
	}
	else
	{
#endif
		drawable = exposeEvent->drawable;
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif

#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable()
			&& WebKit::BackingStore::getInstance()->currentView() == frame->view())
	{
		GdkRegion* region = gdk_region_rectangle(&eventRect);

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
		if(exposeEvent->send_event)
		{
			WebKit::BackingStore::getInstance()->paint(drawable, region,
				realView->pluginview->frameRect().x() - ctx->gdkExposeEvent()->area.x,
				realView->pluginview->frameRect().y() - ctx->gdkExposeEvent()->area.y);
		}
		else
		{
#endif
			WebKit::BackingStore::getInstance()->paint(exposeEvent->drawable, region, 0, 0);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
		}
#endif

		gdk_region_destroy(region);
		return;
	}
#endif

	GC gc = XDefaultGC(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), gdk_screen_get_number(gdk_screen_get_default()));
	cairo_surface_t* cs;
	GdkEvent* event;
	GdkPixmap* gdkDrawable;

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx->save();
		ctx->translate(realView->pluginview->frameRect().x(), realView->pluginview->frameRect().y());
	}
	else
	{
#endif
		Window rootWin;
		int dummyI;
		unsigned int dummyUI, width, height, depth = 0;

		XGetGeometry(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), drawable, &rootWin, &dummyI, &dummyI,
			&width, &height, &dummyUI, &depth);

		cs = cairo_xlib_surface_create(exposeEvent->display,
				drawable, realView->visual, width, height);
		if(!cs)
			return;

		cairo_t* cr = cairo_create(cs);
		ctx = new WebCore::GraphicsContext(cr);
		cairo_destroy(cr);

		event = gdk_event_new(GDK_EXPOSE);
		gdkDrawable = gdk_pixmap_foreign_new_for_screen(gdk_screen_get_default(), drawable, width, height, depth);
		gdk_drawable_set_colormap(gdkDrawable, gdk_screen_get_default_colormap(gdk_screen_get_default()));
		event->expose.window = gdkDrawable;
		g_object_ref(event->expose.window);
		ctx->setGdkExposeEvent((GdkEventExpose*) event);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif

	BEScrollData* data;
	GdkRegion* exceptRegion = gdk_region_new();

	int paintDeltaX = 0, paintDeltaY = 0;

	while((data = (BEScrollData*) g_queue_peek_head(realView->scrollQueue)) != 0)
	{
		GdkRectangle rect;
		rect.x = data->x;
		rect.y = data->y;
		rect.width = data->width;
		rect.height = data->height;

		if(!gdk_rectangle_intersect(&rect, &eventRect, NULL))
			break;

		GdkRegion* remainRegion = gdk_region_rectangle(&rect);

		int offsetX, offsetY;
#ifdef SMOOTH_SCROLL
		offsetX = getScrollOffset(data->remainX);
		offsetY = getScrollOffset(data->remainY);
		data->remainX -= offsetX;
		data->remainY -= offsetY;
#else
		offsetX = data->offsetX;
		offsetY = data->offsetY;
#endif

		GdkRectangle moveRect = { data->x - offsetX, data->y - offsetY, data->width, data->height };
		gdk_rectangle_intersect(&rect, &moveRect, &moveRect);

		GdkRegion* moveRegion = gdk_region_rectangle(&moveRect);
		gdk_region_subtract(moveRegion, exceptRegion);

		if(!gdk_region_empty(moveRegion))
		{
			copyRegion(exposeEvent, drawable, &gc, moveRegion, offsetX, offsetY);

			gdk_region_offset(moveRegion, offsetX, offsetY);
			gdk_region_subtract(remainRegion, moveRegion);
			gdk_region_union(exceptRegion, remainRegion);
		}
		else
		{
			gdk_region_destroy(moveRegion);
			moveRegion = gdk_region_rectangle(&rect);
			gdk_region_union(exceptRegion, moveRegion);
		}

		gdk_region_destroy(moveRegion);
		gdk_region_destroy(remainRegion);

#ifdef SMOOTH_SCROLL
		if(data->remainX != 0 || data->remainY != 0)
		{
			realView->pluginview->invalidateRect(rect);
			gdk_region_offset(exceptRegion, data->remainX, data->remainY);

			paintDeltaX = -data->remainX;
			paintDeltaY = -data->remainY;
			break;
		}
#endif

		g_queue_pop_head(realView->scrollQueue);
		free(data);
	}

	paintRegion(ctx, frameview, exceptRegion, paintDeltaX, paintDeltaY);
	gdk_region_destroy(exceptRegion);

	if(realView->exposeRegion && g_queue_is_empty(realView->scrollQueue))
	{
		GdkRegion* intersectRegion = gdk_region_rectangle(&eventRect);

		gdk_region_intersect(intersectRegion, realView->exposeRegion);
		paintRegion(ctx, frameview, intersectRegion, 0, 0);
		gdk_region_subtract(realView->exposeRegion, intersectRegion);
		gdk_region_destroy(intersectRegion);
//		gdk_region_destroy(realView->exposeRegion);
//		realView->exposeRegion = NULL;
	}

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx->restore();
	}
	else
	{
#endif
		delete ctx;
		gdk_event_free(event);
		g_object_unref(gdkDrawable);
		cairo_surface_destroy(cs);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif
}
#else
static void paintView(BERealView* realView, XGraphicsExposeEvent* exposeEvent)
{
	cairo_rectangle_int_t eventRect = { exposeEvent->x, exposeEvent->y, exposeEvent->width, exposeEvent->height };

	if(realView->drawable != exposeEvent->drawable)
	{
		if(!realView->exposeRegion)
			realView->exposeRegion = cairo_region_create_rectangle(&eventRect);
		else
			cairo_region_union_rectangle(realView->exposeRegion, &eventRect);

		realView->drawable = exposeEvent->drawable;
	}

	if(!realView->exposeRegion && g_queue_is_empty(realView->scrollQueue))
	{
		return;
	}

	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	WebCore::FrameView* frameview = frame->view();

	if(!frame->contentRenderer() || !frameview)
	{
		return;
	}

	Drawable drawable;
	WebCore::GraphicsContext* ctx;

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx = (WebCore::GraphicsContext*) exposeEvent->drawable;
		drawable = cairo_xlib_surface_get_drawable(cairo_get_target(ctx->platformContext()));
	}
	else
	{
#endif
		drawable = exposeEvent->drawable;
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif

#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable()
			&& WebKit::BackingStore::getInstance()->currentView() == frame->view())
	{
		GdkRegion* region = gdk_region_rectangle(&eventRect);

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
		if(exposeEvent->send_event)
		{
			WebKit::BackingStore::getInstance()->paint(drawable, region,
				realView->pluginview->frameRect().x() - ctx->gdkExposeEvent()->area.x,
				realView->pluginview->frameRect().y() - ctx->gdkExposeEvent()->area.y);
		}
		else
		{
#endif
			WebKit::BackingStore::getInstance()->paint(exposeEvent->drawable, region, 0, 0);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
		}
#endif

		gdk_region_destroy(region);
		return;
	}
#endif

	GC gc = XDefaultGC(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), gdk_screen_get_number(gdk_screen_get_default()));
	cairo_surface_t* cs;

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx->save();
		ctx->translate(realView->pluginview->frameRect().x(), realView->pluginview->frameRect().y());
	}
	else
	{
#endif
		Window rootWin;
		int dummyI;
		unsigned int dummyUI, width, height, depth = 0;

		XGetGeometry(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), drawable, &rootWin, &dummyI, &dummyI,
			&width, &height, &dummyUI, &depth);

		cs = cairo_xlib_surface_create(exposeEvent->display,
				drawable, realView->visual, width, height);
		if(!cs)
			return;

		cairo_t* cr = cairo_create(cs);
		ctx = new WebCore::GraphicsContext(cr);
		cairo_destroy(cr);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif

	BEScrollData* data;
	cairo_region_t* exceptRegion = cairo_region_create();

	int paintDeltaX = 0, paintDeltaY = 0;

	while((data = (BEScrollData*) g_queue_peek_head(realView->scrollQueue)) != 0)
	{
		cairo_rectangle_int_t rect = { data->x, data->y, data->width, data->height };

		if(!gdk_rectangle_intersect(&rect, &eventRect, NULL))
			break;

		cairo_region_t* remainRegion = cairo_region_create_rectangle(&rect);

		int offsetX, offsetY;
#ifdef SMOOTH_SCROLL
		offsetX = getScrollOffset(data->remainX);
		offsetY = getScrollOffset(data->remainY);
		data->remainX -= offsetX;
		data->remainY -= offsetY;
#else
		offsetX = data->offsetX;
		offsetY = data->offsetY;
#endif

		cairo_rectangle_int_t moveRect = { data->x - offsetX, data->y - offsetY, data->width, data->height };
		gdk_rectangle_intersect(&rect, &moveRect, &moveRect);

		cairo_region_t* moveRegion = cairo_region_create_rectangle(&moveRect);
		cairo_region_subtract(moveRegion, exceptRegion);

		if(!cairo_region_is_empty(moveRegion))
		{
			copyRegion(exposeEvent, drawable, &gc, moveRegion, offsetX, offsetY);

			cairo_region_translate(moveRegion, offsetX, offsetY);
			cairo_region_subtract(remainRegion, moveRegion);
			cairo_region_union(exceptRegion, remainRegion);
		}
		else
		{
			cairo_region_destroy(moveRegion);
			moveRegion = cairo_region_create_rectangle(&rect);
			cairo_region_union(exceptRegion, moveRegion);
		}

		cairo_region_destroy(moveRegion);
		cairo_region_destroy(remainRegion);

#ifdef SMOOTH_SCROLL
		if(data->remainX != 0 || data->remainY != 0)
		{
			realView->pluginview->invalidateRect(rect);
			cairo_region_translate(exceptRegion, data->remainX, data->remainY);

			paintDeltaX = -data->remainX;
			paintDeltaY = -data->remainY;
			break;
		}
#endif

		g_queue_pop_head(realView->scrollQueue);
		free(data);
	}

	paintRegion(ctx, frameview, exceptRegion, paintDeltaX, paintDeltaY);
	cairo_region_destroy(exceptRegion);

	if(realView->exposeRegion && g_queue_is_empty(realView->scrollQueue))
	{
		cairo_region_t* intersectRegion = cairo_region_create_rectangle(&eventRect);

		cairo_region_intersect(intersectRegion, realView->exposeRegion);
		paintRegion(ctx, frameview, intersectRegion, 0, 0);
		cairo_region_subtract(realView->exposeRegion, intersectRegion);
		cairo_region_destroy(intersectRegion);
	}

#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	if(exposeEvent->send_event)
	{
		ctx->restore();
	}
	else
	{
#endif
		delete ctx;
		cairo_surface_destroy(cs);
#if ENABLE(TVGG_DIRECT_WINDOWLESS_PLUGIN)
	}
#endif
}
#endif

static void buttonPressEvent(BERealView* realView, XButtonEvent* buttonEvent)
{
	// FIXME : We may need to insert checking code whether single click or double click.
	// We ignore it at this phase.
	int currentClickCount = 1;		// useless?

	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return;

	GdkEventButton gdkButtonEvent;
	memset(&gdkButtonEvent, 0, sizeof(GdkEventButton));

	gdkButtonEvent.type = GDK_BUTTON_PRESS;
	gdkButtonEvent.window = gtk_widget_get_window(GTK_WIDGET(webView));
	gdkButtonEvent.time = buttonEvent->time;
	gdkButtonEvent.x = buttonEvent->x;
	gdkButtonEvent.y = buttonEvent->y;
	gdkButtonEvent.x_root = (gfloat)buttonEvent->x_root;
	gdkButtonEvent.y_root = (gfloat)buttonEvent->y_root;
	gdkButtonEvent.axes = NULL;
	gdkButtonEvent.state = (GdkModifierType)buttonEvent->state;
	gdkButtonEvent.button = buttonEvent->button;
#ifdef GTK_API_VERSION_2
	gdkButtonEvent.device = gdk_display_get_default()->core_pointer;
#else
	gdkButtonEvent.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	WebCore::PlatformMouseEvent platformEvent(&gdkButtonEvent);
	platformEvent.setClickCount(currentClickCount);

	frame->eventHandler()->handleMousePressEvent(platformEvent);
}


static WebCore::IntPoint g_wheelScrollPos;

static void buttonReleaseEvent(BERealView* realView, XButtonEvent* buttonEvent)
{
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return;

	GdkEventButton gdkButtonEvent;
	memset(&gdkButtonEvent, 0, sizeof(GdkEventButton));

	gdkButtonEvent.type = GDK_BUTTON_RELEASE;
	gdkButtonEvent.window = gtk_widget_get_window(GTK_WIDGET(webView));
	gdkButtonEvent.time = buttonEvent->time;
	gdkButtonEvent.x = buttonEvent->x;
	gdkButtonEvent.y = buttonEvent->y;
	gdkButtonEvent.x_root = (gfloat)buttonEvent->x_root;
	gdkButtonEvent.y_root = (gfloat)buttonEvent->y_root;
	gdkButtonEvent.axes = NULL;
	gdkButtonEvent.state = (GdkModifierType)buttonEvent->state;
	gdkButtonEvent.button = buttonEvent->button;
#ifdef GTK_API_VERSION_2
	gdkButtonEvent.device = gdk_display_get_default()->core_pointer;
#else
	gdkButtonEvent.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	frame->eventHandler()->handleMouseReleaseEvent(WebCore::PlatformMouseEvent(&gdkButtonEvent));
}

static void wheelButtonEvent(BERealView* realView, XButtonEvent* buttonEvent, WebCore::PlatformWheelEventGranularity aGranularity)
{
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return;

	GdkEventScroll gdkEventScroll;
	memset(&gdkEventScroll, 0, sizeof(GdkEventScroll));

	gdkEventScroll.type = GDK_SCROLL;
	gdkEventScroll.window = gtk_widget_get_window(GTK_WIDGET(webView));
	gdkEventScroll.time = buttonEvent->time;
	gdkEventScroll.x = buttonEvent->x;
	gdkEventScroll.y = buttonEvent->y;
	gdkEventScroll.x_root = (gfloat)buttonEvent->x_root;
	gdkEventScroll.y_root = (gfloat)buttonEvent->y_root;
	gdkEventScroll.state = (GdkModifierType)buttonEvent->state;
#ifdef GTK_API_VERSION_2
	gdkEventScroll.device = gdk_display_get_default()->core_pointer;
#else
	gdkEventScroll.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

    WebCore::IntPoint scrollUnit;
	switch(buttonEvent->button) {
	case BEWheelUp:
		gdkEventScroll.direction = GDK_SCROLL_UP;
        scrollUnit.setX(0);
        scrollUnit.setY(-WebCore::Scrollbar::pixelsPerLineStep());
		break;
	case BEWheelDown:
		gdkEventScroll.direction = GDK_SCROLL_DOWN;
        scrollUnit.setX(0);
        scrollUnit.setY(WebCore::Scrollbar::pixelsPerLineStep());
		break;
	case BEWheelLeft:
		gdkEventScroll.direction = GDK_SCROLL_LEFT;
        scrollUnit.setX(-WebCore::Scrollbar::pixelsPerLineStep());
        scrollUnit.setY(0);
		break;
	case BEWheelRight:
		gdkEventScroll.direction = GDK_SCROLL_RIGHT;
        scrollUnit.setX(WebCore::Scrollbar::pixelsPerLineStep());
        scrollUnit.setY(0);
		break;
	default:
		break;
	}

    WebCore::IntPoint beforeScrollPos = frame->view()->scrollPosition();
    //printf("beforeScrollPosX = %d, beforeScrollPosY = %d\n",beforeScrollPos.x(), beforeScrollPos.y());

	//WebCore::PlatformWheelEvent wheelEvent(&gdkEventScroll, aGranularity);
	const unsigned int scrollIntensityByPageWheelEvent = 3;    // per 480px
	if (aGranularity == WebCore::ScrollByPageWheelEvent) {
        for (unsigned int i = 0; i < scrollIntensityByPageWheelEvent; ++i) {
            WebCore::PlatformWheelEvent wheelEvent(&gdkEventScroll);
    	    frame->eventHandler()->handleWheelEvent(wheelEvent);
        }
	} else {
    	WebCore::PlatformWheelEvent wheelEvent(&gdkEventScroll);
        frame->eventHandler()->handleWheelEvent(wheelEvent);
	}
    WebCore::IntPoint afterScrollPos = frame->view()->scrollPosition();

    if ((beforeScrollPos == afterScrollPos) && !afterScrollPos.x() && !afterScrollPos.y()) {
        if (aGranularity == WebCore::ScrollByPageWheelEvent) {
            g_wheelScrollPos.setX(g_wheelScrollPos.x() + scrollUnit.x() * scrollIntensityByPageWheelEvent);
            g_wheelScrollPos.setY(g_wheelScrollPos.y() + scrollUnit.y() * scrollIntensityByPageWheelEvent);
        } else {
            g_wheelScrollPos.setX(g_wheelScrollPos.x() + scrollUnit.x());
            g_wheelScrollPos.setY(g_wheelScrollPos.y() + scrollUnit.y());
        }

        if (g_wheelScrollPos.x() < 0)
            g_wheelScrollPos.setX(0);
        if (g_wheelScrollPos.y() < 0)
            g_wheelScrollPos.setY(0);
    }

}

static void motionEvent(BERealView* realView, XMotionEvent* motionEvent)
{
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return;

	GdkEventMotion gdkMotionEvent;
	memset(&gdkMotionEvent, 0, sizeof(GdkEventMotion));

	gdkMotionEvent.type = GDK_MOTION_NOTIFY;
	gdkMotionEvent.window = gtk_widget_get_window(GTK_WIDGET(webView));
	gdkMotionEvent.time = motionEvent->time;
	gdkMotionEvent.x = motionEvent->x;
	gdkMotionEvent.y = motionEvent->y;
	gdkMotionEvent.x_root = (gfloat)motionEvent->x_root;
	gdkMotionEvent.y_root = (gfloat)motionEvent->y_root;
	gdkMotionEvent.axes = NULL;
	gdkMotionEvent.state = (GdkModifierType) motionEvent->state;	// which button?
	gdkMotionEvent.is_hint = motionEvent->is_hint;
#ifdef GTK_API_VERSION_2
	gdkMotionEvent.device = gdk_display_get_default()->core_pointer;
#else
	gdkMotionEvent.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	frame->eventHandler()->mouseMoved(WebCore::PlatformMouseEvent(&gdkMotionEvent));
}

void BE_HandleEvent(BEView view, void* event, bool isWheelGranularityPage)
{
	if(!view || !event)
	{
		return;
	}

	BERealView* realView = (BERealView*) view;
	XEvent* xevent = (XEvent*) event;
    WebCore::PlatformWheelEventGranularity granularity = WebCore::ScrollByPixelWheelEvent;

	INIT_JS_GLOBAL_DATA(globalData, prevData);

	switch(xevent->type)
	{
	case GraphicsExpose:
		paintView(realView, &xevent->xgraphicsexpose);
		break;

	case ButtonPress:
		switch(xevent->xbutton.button)
		{
		case 4: /* up */
		case 5: /* down */
		case 6: /* left */
		case 7: /* right */
            if (isWheelGranularityPage)
                granularity = WebCore::ScrollByPageWheelEvent;

			wheelButtonEvent(realView, &xevent->xbutton, granularity);
			break;
		default:
			buttonPressEvent(realView, &xevent->xbutton);
			break;
		}
		break;

	case ButtonRelease:
		/* Wheel event is valid when the button is pressed. Ignore released button event */
		switch(xevent->xbutton.button)
		{
		case 4: /* up */
		case 5: /* down */
		case 6: /* left */
		case 7: /* right */
			break;
		default:
			buttonReleaseEvent(realView, &xevent->xbutton);
			break;
		}
		break;

	case MotionNotify:
		motionEvent(realView, &xevent->xmotion);
		break;
	}

	RESTORE_JS_GLOBAL_DATA(globalData, prevData);
}

static char* getSnapshotFilepath(const char* url, const char* prefix)
{
	const char* extension = ".png";

	int prefixLength;
	if(prefix)
	{
		prefixLength = strlen(prefix);
	}
	else
	{
		prefixLength = 0;
	}

	int pathLength = strlen(SNAPSHOT_PATH);
	char* filepath = (char*) malloc(prefixLength + pathLength + strlen(extension) + strlen(url) + 1);
	if(!filepath)
	{
		return NULL;
	}

	if(prefix)
	{
		strcpy(filepath, prefix);
	}

	strcpy(filepath + prefixLength, SNAPSHOT_PATH);

	char* fileIter = filepath + prefixLength + pathLength;
	int i = 0;
	while(*url)
	{
		if(WTF::isASCIIAlphanumeric(*url))
		{
			*fileIter++ = *url;
		}
		else
		{
			*fileIter++ = '_';
		}

		++url;
		if(++i > 200)
		{
			break;
		}
	}
//	*fileIter = '\0';
//	strcat(fileName, url);
	strcpy(fileIter, extension);

	return filepath;
}

void BE_InitSnapshot(BEView view)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	ASSERT(realView->snapshotRef == 0 && realView->snapshot == 0);
	++realView->snapshotRef;
}

void BE_RemoveSnapshot(BEView view)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	if(--realView->snapshotRef == 0)
	{
		if(realView->snapshot)
		{
			cairo_surface_destroy(realView->snapshot);
			realView->snapshot = 0;
		}
		realView->snapshotRatio = 0;
	}
}

void BE_SetSnapshotRatio(BEView view, float ratio)
{
	if(!view)
		return;

	if(ratio < .5f)
		ratio = .5f;

	BERealView* realView = (BERealView*) view;
	if(realView->snapshotRatio > 0 && ratio >= realView->snapshotRatio)
		return;

	realView->snapshotRatio = ratio;
	if(realView->snapshot)
	{
		cairo_surface_destroy(realView->snapshot);
		realView->snapshot = 0;
	}
}

static void getSnapshot(BERealView* realView, unsigned int targetWidth, unsigned int targetHeight)
{
	ASSERT(realView->snapshotRef > 0);

	if(realView->snapshot)
		return;

	WebCore::Frame* frame = WebKit::core(realView->webview)->mainFrame();
	WebCore::FrameView* frameview = frame->view();
	if(!frameview)
		return;

	frameview->updateLayoutAndStyleIfNeededRecursive();

	WebCore::IntRect rect = frameview->visibleContentRect();
	float ratio;

	if(realView->snapshotRatio <= 0.f)
	{
		ratio = 1.f;
	}
	else
	{
		ratio = 1 / frame->pageZoomFactor();
		rect.setWidth(rect.width() / ratio);
		rect.setHeight(rect.height() / ratio);

		if(ratio > 1.f)
		{
			rect.setX(rect.x() + (rect.width() * (ratio - 1) / 2));
			rect.setY(rect.y() + (rect.height() * (ratio - 1) / 2));
		}

		WebCore::IntSize contentSize = frameview->contentsSize();
		int diff;
		if((diff = rect.maxX() - contentSize.width()) > 0)
			rect.setX(rect.x() - diff);
		if((diff = rect.maxY() - contentSize.height()) > 0)
			rect.setY(rect.y() - diff);
		rect.intersect(WebCore::IntRect(WebCore::IntPoint(), contentSize));
	}

	float scaleFactorW = targetWidth / (rect.width() * ratio) * 2.f;
	float scaleFactorH = targetHeight / (rect.height() * ratio) * 2.f;

	cairo_surface_t* surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			rect.width() * ratio, rect.height() * ratio);
	if(!surface)
	{
		printf("[%s:%d] Fail to allocate surface\n", __func__, __LINE__);
		return;
	}

	cairo_surface_t* snapshot = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			rect.width() * ratio * scaleFactorW, rect.height() * ratio * scaleFactorH);
	if(!snapshot)
	{
		printf("[%s:%d] Fail to allocate surface\n", __func__, __LINE__);
		cairo_surface_destroy(surface);
		return;
	}

/*	GdkEvent* event = gdk_event_new(GDK_EXPOSE);
	gdk_drawable_set_colormap(gdkDrawable, gdk_screen_get_default_colormap(gdk_screen_get_default()));
	event->expose.window = gdkDrawable;
	g_object_ref(event->expose.window);
	ctx.setGdkExposeEvent((GdkEventExpose*) event);
*/

	cairo_t* cr = cairo_create(surface);
	WebCore::GraphicsContext ctx(cr);
	cairo_destroy(cr);
	cr = cairo_create(snapshot);

	if(ratio >= 1.f)
	{
		ctx.scale(WebCore::FloatSize(ratio, ratio));
		ctx.translate(-rect.x(), -rect.y());
		ctx.clip(rect);

		frameview->paintContents(&ctx, rect);

		cairo_scale(cr, scaleFactorW, scaleFactorH);
		cairo_set_source_surface(cr, surface, 0, 0);
		cairo_paint(cr);
	}
	else
	{
		int unitW = rect.width() * ratio;
		int unitH = rect.height() * ratio;
		WebCore::IntRect gridRect(rect.x(), rect.y(), unitW, unitH);
		cairo_scale(cr, ratio * scaleFactorW, ratio * scaleFactorH);

		for(; gridRect.y() < rect.maxY(); gridRect.move(0, unitH))
		{
			for(; gridRect.x() < rect.maxX(); gridRect.move(unitW, 0))
			{
				ctx.save();

				ctx.translate(-gridRect.x(), -gridRect.y());
				ctx.clip(gridRect);
				frameview->paintContents(&ctx, gridRect);

				cairo_set_source_surface(cr, surface, gridRect.x() - rect.x(), gridRect.y() - rect.y());
				cairo_paint(cr);

				ctx.restore();
			}
			gridRect.setX(rect.x());
		}
	}

//	gdk_event_free(event);
	cairo_surface_destroy(surface);
	cairo_destroy(cr);

	realView->snapshot = snapshot;
}

BEResult BE_DrawSnapshot(BEView view, const char* url, void* event, float ratio,
		float offsetX, float offsetY, unsigned int backgroundColor)
{
	if((!view && !url) || !event)
	{
		return BEFail;
	}

	XEvent* xevent = (XEvent*) event;
	if(xevent->type != GraphicsExpose)
	{
		return BEFail;
	}

	XGraphicsExposeEvent* exposeEvent = &xevent->xgraphicsexpose;

	Window rootWin;
	int dummyI;
	unsigned int dummyUI, width, height, depth = 0;

	XGetGeometry(GDK_DISPLAY_XDISPLAY(gdk_display_get_default()), exposeEvent->drawable, &rootWin, &dummyI, &dummyI,
		&width, &height, &dummyUI, &depth);

	cairo_surface_t* cs = cairo_xlib_surface_create(exposeEvent->display, exposeEvent->drawable,
			GDK_VISUAL_XVISUAL(gdk_screen_get_system_visual(gdk_screen_get_default())), width, height);
	if(!cs)
	{
		return BEFail;
	}

	cairo_t* cr = cairo_create(cs);

	cairo_rectangle(cr, exposeEvent->x, exposeEvent->y, exposeEvent->width, exposeEvent->height);
	cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR);
	cairo_fill_preserve(cr);

	float red = (float) (backgroundColor >> 24) / 255;
	float green = (float) ((backgroundColor & 0xff0000) >> 16) / 255;
	float blue = (float) ((backgroundColor & 0xff00) >> 8) / 255;
	float alpha = (float) (backgroundColor & 0xff) / 255;
	cairo_set_source_rgba(cr, red, green, blue, alpha);
	cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
	cairo_fill(cr);

	WebCore::GraphicsContext ctx(cr);
	cairo_destroy(cr);

	cairo_surface_t* surface;
	if(view)
	{
		BERealView* realView = (BERealView*) view;
		getSnapshot(realView, width, height);
		surface = realView->snapshot;
	}
	else
	{
		char* path = getSnapshotFilepath(url, NULL);
		surface = cairo_image_surface_create_from_png(path);
		free(path);
	}

	if(!surface)
	{
		cairo_surface_destroy(cs);
		return BEFail;
	}

	int contentWidth = cairo_image_surface_get_width(surface);
	int contentHeight = cairo_image_surface_get_height(surface);

	if(ratio <= 0.f)
	{
		ratio = 1.f;
	}

	float hRatio = ratio * (((float) width) / contentWidth);
	float vRatio = ratio * (((float) height) / contentHeight);

	int marginX, marginY;
	if((marginX = (int) (width - (contentWidth * hRatio))) > 0)
	{
		marginX /= 2;
	}
	else
	{
		marginX = 0;
	}
	if((marginY = (int) (height - (contentHeight * vRatio))) > 0)
	{
		marginY /= 2;
	}
	else
	{
		marginY = 0;
	}

	cairo_scale(cr, hRatio, vRatio);
	cairo_set_source_surface(cr, surface, marginX / hRatio, marginY / vRatio);
	cairo_paint(cr);

	if(!view)
	{
		cairo_surface_destroy(surface);
	}

	cairo_surface_destroy(cs);
	return BEPass;
}

void BE_SetCreateViewCallback(BEView view, BECreateViewCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->createView = callback;
}

void BE_SetCloseViewCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->closeView = callback;
}

void BE_SetDecideNavigationPolicyCallback(BEView view, BENavigationPolicyCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->decideNavigationPolicy = callback;
}

void BE_SetBlockPopupCallback(BEView view, BEBlockPopupCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->blockPopup = callback;
}

void BE_SetLoadCommittedCallback(BEView view, BELoadStatusCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->loadCommitted = callback;
}

void BE_SetLoadFinishedCallback(BEView view, BELoadStatusCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->loadFinished = callback;
}

void BE_SetLoadProgressCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->loadProgress = callback;
}

void BE_SetLoadErrorCallback(BEView view, BELoadErrorCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->loadError = callback;
}

void BE_SetTitleChangedCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->titleChanged = callback;
}

void BE_SetPopupMenuCallback(BEView view, BEShowPopupMenuCallback showCallback, BEBasicCallback hideCallback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;

	if(showCallback && hideCallback)
	{
		realView->showPopupMenu = showCallback;
		realView->hidePopupMenu = hideCallback;
	}
	else
	{
		realView->showPopupMenu = NULL;
		realView->hidePopupMenu = NULL;
	}
}

void BE_SetIconLoadedCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->iconLoaded = callback;
}

void BE_SetNotifyDownloadCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->notifyDownloadRequest = callback;
}

void BE_SetDownloadEndCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->endDownload = callback;
}

void BE_SetScriptAlertCallback(BEView view, BEScriptAlertCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->alert = callback;
}

void BE_SetScriptConfirmCallback(BEView view, BEScriptConfirmCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->confirm = callback;
}

void BE_SetScriptPromptCallback(BEView view, BEScriptPromptCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->prompt = callback;
}

void BE_SetConsoleMessageCallback(BEView view, BEConsoleMessageCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->consoleMsg = callback;
}

void BE_SetPrintRequestedCallback(BEView view, BEBasicCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->printReq = callback;
}

void BE_SetIMEFocusCallback(BEView view, BEIMEFocusCallback callback)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->imeFocus = callback;
}

void BE_SetShowIMECallback(BEView view, BEBasicCallback callback)
{
#if ENABLE(TVGG_FLASH_IME)
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->showIME = callback;
#endif
}

#if ENABLE(TVGG_TRANSLATED_ALERT) //@20110126-sanggyu
void BE_SetScriptTranslatedAlertCallback(BEView view, BEScriptTranslatedAlertCallback callback)
{
	if (!view)
		return;

	BERealView* realView = (BERealView*) view;
    realView->translatedAlert = callback;
}
#endif

void BE_SetCallbackData(BEView view, void* data)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	realView->callbackData = data;
}

void BE_AllowPopup(BEView view, bool allow)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	WebCore::Settings* settings = WebKit::core(realView->webview)->settings();
	settings->setJavaScriptCanOpenWindowsAutomatically(allow);
}

void BE_SetDefaultTextEncodingName(BEView view, const char* encoding)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	WebCore::Settings* settings = WebKit::core(realView->webview)->settings();
	settings->setDefaultTextEncodingName(encoding);
}

void BE_SetCustomEncoding(BEView view, const char* encoding)
{
    if(!view)
		return;

    WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);

    webkit_web_view_set_custom_encoding(webview, encoding);
}

void BE_UseEncodingDetector(BEView view, bool use)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	WebCore::Settings* settings = WebKit::core(realView->webview)->settings();
	settings->setUsesEncodingDetector(use);
}

char* BE_GetEncodingName(BEView view)
{
	if(!view)
		return NULL;

	WebKitWebView* webview = ((BERealView*)view)->webview;
	WTF::String encoding = WebKit::core(webview)->mainFrame()->document()->loader()->writer()->encoding();
	return copyCString(encoding.utf8());
}

void BE_SetEncodingName(BEView view, const char* encoding)
{
	if(!view)
		return;

	WebKitWebView* webview = ((BERealView*)view)->webview;
	WebKit::core(webview)->mainFrame()->loader()->reloadWithOverrideEncoding(WTF::String::fromUTF8(encoding));
}

void BE_SetAcceptLanguage(const char* acceptLanguage)
{
	SoupSession* session = WebCore::ResourceHandle::defaultSession();
	if(!session)
	{
		return;
	}

	g_object_set(G_OBJECT(session), "accept-language", acceptLanguage, NULL);
}

void BE_SetFontFamily(BEView view, const char* fontfamily)
{
	if(!view)
		return;

	WebKitWebView* webview = ((BERealView*)view)->webview;
	WebCore::Settings* settings = WebKit::core(webview)->settings();
	WTF::String fontfamilyStr = WTF::String::fromUTF8(fontfamily);

	settings->setStandardFontFamily(fontfamilyStr);
	settings->setFixedFontFamily(fontfamilyStr);
	settings->setSerifFontFamily(fontfamilyStr);
	settings->setSansSerifFontFamily(fontfamilyStr);
	settings->setCursiveFontFamily(fontfamilyStr);
	settings->setFantasyFontFamily(fontfamilyStr);
}

void BE_SetDrawElementFocus(bool isDraw)
{
	g_drawElementFocus = isDraw;

#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
	if(!g_drawElementFocus && WebKit::BackingStore::isAvailable())
	{
		Vector<WebCore::IntRect> rects;
		WebKit::BackingStore::getInstance()->setFocusRect(rects);	// Remove focus
	}
#endif
}

void BE_ForceDraw(BEView view)
{
	if(!view)
		return;

	BERealView* realView = (BERealView*) view;
	WebKitWebView* webview = realView->webview;

	WebCore::Frame* frame = WebKit::core(webview)->mainFrame();
	if(!frame->contentRenderer() || !frame->view())
		return;
	frame->view()->forceLayout(true);

	GtkWidget* parent = realView->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();

#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebKit::BackingStore::getInstance()->setForceUpdate();
	}
#endif

	gdk_window_process_updates(gtk_widget_get_window(parent), TRUE);
}

void BE_LoadURL(BEView view, const char* url)
{
	if(!view || !url)
		return;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable() &&
			WebKit::BackingStore::getInstance()->webView() == webview)
	{
		WebKit::BackingStore::getInstance()->resetViewState();
	}
#endif
	webkit_web_view_load_uri(webview, url);
}

void BE_Reload(BEView view)
{
	if(!view)
		return;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebKit::core(webview)->mainFrame()->loader()->reload();
}

BEResult BE_ChangeWidget(BEView view)
{
#if ENABLE(TVGG_BACKINGSTORE)
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	webkit_backingstore_change_web_view(webview);
#endif
	return BEPass;
}

void BE_SetTabMoveDirection(BETabMoveDirection tabMoveDirection)
{
#if ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebKit::BackingStore::getInstance()->setTabMoveDirection(
				(TAB_MOVE_DIRECTION)tabMoveDirection);
	}
#endif
}

BEResult BE_StopLoading(BEView view)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	webkit_web_view_stop_loading(webview);
	return BEPass;
}

float BE_GetLoadingProgress(BEView view)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	return webkit_web_view_get_progress(webview);
}

const char* BE_GetTitle(BEView view)
{
	if(!view)
		return NULL;

	BERealView* realView = (BERealView*) view;
	return webkit_web_view_get_title(realView->webview);
}

BEResult BE_GetViewRect(BEView view, int* width, int* height)
{
	if(!view)
	{
		if(width)
			*width = 0;
		if(height)
			*height = 0;
		return BEFail;
	}

	WebCore::Frame* frame = WebKit::core(WEBKIT_WEB_VIEW(((BERealView*)view)->webview))->mainFrame();
	WebCore::IntRect rect = frame->view()->visibleContentRect();

	if(width)
		*width = rect.width();
	if(height)
		*height = rect.height();

	return BEPass;
}

BEResult BE_GetContentSize(BEView view, int* width, int* height)
{
	if(!view)
	{
		if(width)
			*width = 0;
		if(height)
			*height = 0;
		return BEFail;
	}

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::FrameView* frameview = WebKit::core(webview)->mainFrame()->view();
	if(!frameview)
		return BEFail;

	WebCore::IntSize size = frameview->contentsSize();

	if(width)
		*width = size.width();
	if(height)
		*height = size.height();

	return BEPass;
}

const char* BE_GetFavicon(BEView view)
{
	if(!view)
	{
		return NULL;
	}

	return webkit_web_view_get_icon_uri(((BERealView*)view)->webview);
}

BEResult BE_GetScrollPos(BEView view, int* x, int* y)
{
	if(!view)
	{
		if(x)
			*x = 0;
		if(y)
			*y = 0;
		return BEFail;
	}

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::FrameView* frameview = WebKit::core(webview)->mainFrame()->view();
	if(!frameview)
		return BEFail;

	WebCore::IntPoint point = frameview->scrollPosition();

	if(x)
		*x = point.x();
	if(y)
		*y = point.y();

    if (x && (*x == 0) && (g_wheelScrollPos.x() != 0))
        *x = g_wheelScrollPos.x();
    if (y && (*y == 0) && (g_wheelScrollPos.y() != 0))
        *y = g_wheelScrollPos.y();


	return BEPass;
}

BEResult BE_SetScrollPos(BEView view, int x, int y)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::IntPoint point(x, y);

	WebCore::FrameView* frameview = WebKit::core(webview)->mainFrame()->view();
	if(!frameview)
		return BEFail;

	frameview->setScrollPosition(point);
	return BEPass;
}

BEResult BE_SetFocus(BEView view)
{
	if (!view)
		return BEFail;

	GtkWidget* webview = GTK_WIDGET(((BERealView*)view)->webview);
	gtk_widget_grab_focus(webview);

#if ENABLE(TVGG_BACKINGSTORE)
	BERealView* realView = (BERealView*) view;
	GtkWidget* parent = realView->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
	if (WebKit::BackingStore::isAvailable() &&
			parent != webview &&
			WEBKIT_WEB_VIEW(webview) != WebKit::BackingStore::getInstance()->webView())
	{
		webkit_backingstore_change_web_view(WEBKIT_WEB_VIEW(webview));
	}
#endif

	return BEPass;
}

BEResult BE_IsLoginForm(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isHTMLElement())
		return BEFail;

	WebCore::Element* elm = (WebCore::Element*) node;
	WebCore::HTMLFormElement* formElm;
	if(elm->hasTagName(WebCore::HTMLNames::formTag))
		formElm = (WebCore::HTMLFormElement*) elm;
	else if(elm->isFormControlElement())
		formElm = ((WebCore::HTMLFormControlElement*)elm)->form();
	else
		formElm = NULL;

	if(!formElm)
		return BEFail;

	WTF::Vector<WebCore::FormAssociatedElement*> list = formElm->associatedElements();
	for(unsigned i = 0; i < list.size(); ++i)
	{
		WebCore::HTMLInputElement* inputElm = (WebCore::HTMLInputElement*) WebCore::toHTMLElement(list[i])->toInputElement();
		if(inputElm && inputElm->isPasswordField())
		{
			return BEPass;
		}
	}

	return BEFail;
}

static bool setLoginForm(WebCore::HTMLFormElement* form, WebCore::HTMLInputElement* passwordElm,
		const char* id, const char* password)
{
	if(!form)
	{
		return false;
	}

	WTF::Vector<WebCore::FormAssociatedElement*> list = form->associatedElements();
	for(unsigned i = 0; i < list.size(); ++i)
	{
		WebCore::HTMLInputElement* inputElm = (WebCore::HTMLInputElement*) WebCore::toHTMLElement(list[i])->toInputElement();
		if(inputElm && inputElm->isText())
		{
			WebCore::ExceptionCode ec;

			inputElm->Node::dispatchFocusEvent();
			inputElm->setAttribute("value", id, ec);
			inputElm->Node::dispatchBlurEvent();

			passwordElm->Node::dispatchFocusEvent();
			passwordElm->setAttribute("value", password, ec);
			passwordElm->Node::dispatchBlurEvent();
			return true;
		}
	}

	return false;
}

BEResult BE_SetLoginForm(BEView view, const char* id, const char* password)
{
	if(!view || !id || !password)
	{
		return BEFail;
	}

	WebCore::Frame* frame = WebKit::core(((BERealView*)view)->webview)->mainFrame();
	if(!frame)
	{
		return BEFail;
	}

	WebCore::Document* doc = frame->document();
	WebCore::Node* node = static_cast<WebCore::Node*>(doc->documentElement());

	while(1)
	{
		while(node)
		{
			if(node->isElementNode())
			{
				WebCore::InputElement* inputElm = node->toInputElement();
				if(inputElm && inputElm->isPasswordField())
				{
					WebCore::HTMLInputElement* passwordElm = (WebCore::HTMLInputElement*) inputElm;
					if(setLoginForm(passwordElm->form(), passwordElm, id, password))
					{
						return BEPass;
					}
				}

				if(node->isFrameOwnerElement())
				{
				    WebCore::HTMLFrameOwnerElement* owner = static_cast<WebCore::HTMLFrameOwnerElement*>(node);
				    doc = owner->contentDocument();
				    if(doc)
				    {
				    	break;
				    }
				    doc = node->document();
				}
			}
			node = node->traverseNextNode();
		}

		if(node)
		{
			node = static_cast<WebCore::Node*>(doc->documentElement());
		}
		else
		{
			node = static_cast<WebCore::Node*>(doc->ownerElement());
			if(node)
			{
				doc = node->document();
				node = node->traverseNextNode();
			}
			else
				break;
		}
	}

	return BEFail;
}

static cairo_surface_t* getViewSurface(WebKitWebView* webview, BERect* viewRect, float factor, bool isNormalizeZoom)
{
	if(!WEBKIT_IS_WEB_VIEW(webview))
		return NULL;

	WebCore::Frame* frame = WebKit::core(webview)->mainFrame();
	if(!frame->contentRenderer() || !frame->view())
		return NULL;

	WebCore::IntSize contentSize = frame->view()->contentsSize();

	WebCore::IntRect rect;
	if(viewRect == NULL)
	{
		rect.setWidth(contentSize.width());
		rect.setHeight(contentSize.height());
	}
	else
	{
		rect.setWidth(viewRect->width);
		rect.setHeight(viewRect->height);
	}

	int width = rect.width();
	int height = rect.height();
	if(isNormalizeZoom)
	{
		float zoom = frame->pageZoomFactor();
		rect.scale(zoom);
		if(contentSize.width() < rect.width())
		{
			rect.setWidth(contentSize.width());
			width = contentSize.width() / zoom;
		}
		if(contentSize.height() < rect.height())
		{
			rect.setHeight(contentSize.height());
			height = contentSize.height() / zoom;
		}
		width *= factor;
		height *= factor;
		factor /= frame->pageZoomFactor();
	}
	else
	{
		width *= factor;
		height *= factor;
	}

	cairo_surface_t* cs = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
	if(!cs)
		return NULL;

	frame->view()->updateLayoutAndStyleIfNeededRecursive();

	cairo_t* cr = cairo_create(cs);
	WebCore::GraphicsContext ctx(cr);
	cairo_destroy(cr);

	if(viewRect)
		ctx.translate(-viewRect->left, -viewRect->top);

	ctx.clearRect(rect);

	WebCore::FloatSize scaleFactor(factor, factor);
	ctx.scale(scaleFactor);
	frame->view()->paintContents(&ctx, rect);

	return cs;
}

static void saveSurfaceToBitmap(cairo_surface_t* cs, const char* filename)
{
	struct bmpfile_magic
	{
		unsigned char magic[2];
	};
	struct bmpfile_header
	{
		uint32_t filesz;
		uint16_t creator1;
		uint16_t creator2;
		uint32_t bmp_offset;
	};
	struct BitmapInfoHeader
	{
		uint32_t biSize;
		int32_t biWidth;
		int32_t biHeight;
		uint16_t nplanes;
		uint16_t bitBitCount;
		uint32_t compress_type;
		uint32_t bmp_bytesz;
		uint32_t hres;
		uint32_t vres;
		uint32_t ncolors;
		uint32_t nimpcolors;
	};
	static const double defaultDPI = 96.0;

	bmpfile_magic bmp_magic;
	bmpfile_header bmp_file_header;
	BitmapInfoHeader bmp_info_header;

	memset(&bmp_file_header, 0, sizeof(bmpfile_header));
	memset(&bmp_info_header, 0, sizeof(BitmapInfoHeader));

	gint bmp_width = cairo_image_surface_get_width(cs);
	gint bmp_height = cairo_image_surface_get_height(cs);
	bmp_magic.magic[0] = 'B';
	bmp_magic.magic[1] = 'M';

	bmp_file_header.filesz = sizeof(bmpfile_magic)
					+ sizeof(bmpfile_header) + sizeof(BitmapInfoHeader)
					+ bmp_width * bmp_height * 3;
	bmp_file_header.creator1 = 0;
	bmp_file_header.creator2 = 0;
	bmp_file_header.bmp_offset = sizeof(bmpfile_magic)
					+ sizeof(bmpfile_header) + sizeof(BitmapInfoHeader);

	bmp_info_header.biSize = sizeof(BitmapInfoHeader);
	bmp_info_header.biWidth = bmp_width;
	bmp_info_header.biHeight = bmp_height;

	unsigned rowPad = (4 - ((bmp_width * 3) & 3)) & 3;

	bmp_info_header.nplanes = 1;
	bmp_info_header.bitBitCount = 24;
	bmp_info_header.compress_type = 0;
	bmp_info_header.bmp_bytesz = bmp_width * bmp_height * 3
							+ bmp_info_header.biHeight * rowPad;
	bmp_info_header.hres = defaultDPI;
	bmp_info_header.vres = defaultDPI;
	bmp_info_header.ncolors = 0;
	bmp_info_header.nimpcolors = 0;

    GFile* file = g_file_new_for_path(filename);
    GError* error = NULL;

	//GFileOutputStream* bmp_file = g_file_create(file, G_FILE_CREATE_NONE, NULL, &error);
	GFileOutputStream* bmp_file = g_file_replace(file, NULL, false,
									G_FILE_CREATE_NONE, NULL, &error);
	if(bmp_file != NULL)
	{
		g_output_stream_write(G_OUTPUT_STREAM(bmp_file), &bmp_magic,
			sizeof(bmpfile_magic), NULL, &error);
		g_output_stream_write(G_OUTPUT_STREAM(bmp_file),
			&bmp_file_header, sizeof(bmpfile_header), NULL, &error);
		g_output_stream_write(G_OUTPUT_STREAM(bmp_file), &bmp_info_header,
			sizeof(BitmapInfoHeader), NULL, &error);

		unsigned char* bits = cairo_image_surface_get_data(cs);
		gsize bytes_written;

		unsigned char* bmp_buffer = (unsigned char*) g_malloc(
			bmp_info_header.bmp_bytesz);
		if(bmp_buffer == NULL)
		{
			g_output_stream_close(G_OUTPUT_STREAM(bmp_file), NULL, &error);
			cairo_surface_destroy(cs);
			return;
		}

		unsigned char* dest = bmp_buffer;
		// getting pixels from the bottom line of image
		unsigned char* src = bits + bmp_width * (bmp_height - 1) * 4;

		for (int i = 0; i < bmp_height; i++)
		{
			for (int j = 0; j < bmp_width; j++)
			{
				*dest++ = *src++;
				*dest++ = *src++;
				*dest++ = *src++;
				src++;
			}
			dest += rowPad;
			src -= bmp_width * 4 * 2;
		}

		g_output_stream_write_all(G_OUTPUT_STREAM(bmp_file), bmp_buffer,
			bmp_info_header.bmp_bytesz, &bytes_written, NULL, &error);
		g_output_stream_close(G_OUTPUT_STREAM(bmp_file), NULL, &error);
		g_free(bmp_buffer);
	}

	cairo_surface_destroy(cs);
}

BEResult BE_CaptureWebPageToImageFile(BEView view, const char* filename)
{
	if(!view || !filename)
		return BEFail;

//	webkit_web_view_full_page_to_png(WEBKIT_WEB_VIEW(view), filename);
//	webkit_web_view_page_to_bmp(WEBKIT_WEB_VIEW(((BERealView*)view)->webview), filename);
	cairo_surface_t* cs = getViewSurface(WEBKIT_WEB_VIEW(((BERealView*)view)->webview), NULL, 1.f, false);
	if(!cs)
		return BEFail;

	saveSurfaceToBitmap(cs, filename);
	return BEPass;
}

BEResult BE_CaptureRectToImageFile(BEView view, const char* filename, BERect* rect)
{
	if(!view || !filename)
		return BEFail;

//	webkit_web_view_full_page_to_png(WEBKIT_WEB_VIEW(view), filename);
//	webkit_web_view_page_to_bmp(WEBKIT_WEB_VIEW(((BERealView*)view)->webview), filename);
	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	cairo_surface_t* cs;
	if(rect)
	{
	    float zoom = WebKit::core(webview)->mainFrame()->pageZoomFactor();
		rect->left *= zoom;
		rect->top *= zoom;
		rect->width *= zoom;
		rect->height *= zoom;
		cs = getViewSurface(webview, rect, 1.f, false);
	}
	else
		cs = getViewSurface(webview, NULL, 1.f, false);

	if(!cs)
		return BEFail;

	saveSurfaceToBitmap(cs, filename);
	return BEPass;
}

void BE_SetSnapshotStorageSize(unsigned int size)
{
	g_snapshotStorageSize = size;
}

static void initSnapshot()
{
    DIR* base = opendir(SNAPSHOT_PATH);
    if (!base) {
    	if(errno == ENOENT)
    	    mkdir(SNAPSHOT_PATH, 0755);

    	return;
    }

    int pathLength = strlen(SNAPSHOT_PATH);
    char* filepath = (char*) malloc(pathLength + 257);
    if (!filepath) {
        closedir(base);
        return;
    }
	strcpy(filepath, SNAPSHOT_PATH);

	char* filename = filepath + pathLength;
	struct dirent* entry = NULL;
	while((entry = readdir(base))) {
        if(entry->d_name[0] == '.')	{
        	if(entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
        	    continue;
        }

        strncpy(filename, entry->d_name, 256);

        struct stat statinfo;
        lstat(filepath, &statinfo);

        g_snapshotSize += (unsigned long) statinfo.st_size;
	}

    free(filepath);
    closedir(base);
}

static BESnapshotList* addSnapshotList(BESnapshotList* list, time_t access, unsigned long size, char* filename)
{
	BESnapshotList* newNode = (BESnapshotList*) malloc(sizeof(BESnapshotList) + strlen(filename) + 1);
	if(!newNode)
	{
		return list;
	}

	newNode->access = access;
	newNode->size = size;
	newNode->filename = (char*) (newNode + 1);
	strcpy(newNode->filename, filename);

	if(!list || access < list->access)
	{
		newNode->next = list;
		return newNode;
	}

	BESnapshotList* prev = list;
	BESnapshotList* iter = list->next;
	while(iter)
	{
		if(access < iter->access)
		{
			prev->next = newNode;
			newNode->next = iter;
			return list;
		}
		prev = iter;
		iter = iter->next;
	}

	prev->next = newNode;
	newNode->next = NULL;
	return list;
}

static void checkSnapshotLimit(int width, int height, float scale)
{
    long estimate = (long) ((float) (width * height * 4) * scale);
    if(g_snapshotSize + estimate < g_snapshotStorageSize)
        return;

	DIR* base = opendir(SNAPSHOT_PATH);
	if (!base)
        return;

	int pathLength = strlen(SNAPSHOT_PATH);
	char* filepath = (char*) malloc(pathLength + 257);
    if (!filepath) {
        closedir(base);
    	return;
    }
	strcpy(filepath, SNAPSHOT_PATH);

	BESnapshotList* list = NULL;
	char* filename = filepath + pathLength;
	struct dirent* entry = NULL;

	while ((entry = readdir(base))) {
		if (entry->d_name[0] == '.')	{
			if (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))
				continue;
		}

		strncpy(filename, entry->d_name, 256);

		struct stat statinfo;
		lstat(filepath, &statinfo);

		list = addSnapshotList(list, statinfo.st_atime, statinfo.st_size, entry->d_name);
	}

	closedir(base);

	unsigned long threshold = (unsigned long) ((float) g_snapshotStorageSize * .8f);

	while (list) {
		strncpy(filename, list->filename, 256);
		remove(filepath);

		g_snapshotSize -= list->size;
		if(g_snapshotSize <= threshold)
			break;

		BESnapshotList* temp = list;
		list = list->next;

		free(temp);
	}

	while (list) {
		BESnapshotList* temp = list;
		list = list->next;

		free(temp);
	}

	free(filepath);
}

static time_t getCurTime()
{
	return g_nBEBaseTime + time(NULL);
}

static BEResult captureSnapshot(WebKitWebView* webview, char* path, float scale)
{
	WebCore::IntRect contentRect = WebKit::core(webview)->mainFrame()->view()->visibleContentRect();

	BERect rect;
	rect.left = 0;
	rect.top = 0;
	rect.width = contentRect.width();
	rect.height = contentRect.height();

	BEResult result;
	cairo_surface_t* cs = getViewSurface(webview, &rect, scale, true);
	if(cs)
	{
		checkSnapshotLimit(rect.width, rect.height, scale);

//		saveSurfaceToBitmap(cs, filename);
		cairo_surface_write_to_png(cs, path);
		result = BEPass;

		struct stat statinfo;
		lstat(path, &statinfo);
		g_snapshotSize += (unsigned long) statinfo.st_size;

		time_t curTime = getCurTime();
		struct utimbuf curTimeBuffer = { curTime, curTime };
		utime(path, &curTimeBuffer);

		cairo_surface_destroy(cs);
	}
	else
	{
		result = BEFail;
	}
	return result;
}

BEResult BE_TakeSnapshot(BEView view, float scale)
{
	if(!view || scale > 1.f || scale <= 0)
		return BEFail;

	char* url = BE_GetURL(BE_GetMainDocument(view));
	if(!url)
		return BEFail;
	else if(!strncmp(url, "about:", 6)) {
		free(url);
		return BEPass;
	}

	if(!g_snapshotSize)
	{
		initSnapshot();
	}

	char* path = getSnapshotFilepath(url, NULL);
	free(url);
	if(!path)
	{
		return BEFail;
	}

	BEResult result = captureSnapshot(((BERealView*)view)->webview, path, scale);
	free(path);

	//system("sync;\n");
	return result;
}

BEResult BE_TakeSnapshotOfParentView(BEView view, float scale)
{
    if (!view || scale > 1.f || scale <= 0)
        return BEFail;

	char* url = BE_GetURL(BE_GetMainDocument(view));
	if (!url)
		return BEFail;

    if (!strncmp(url, "about:", 6)) {
		free(url);
		return BEPass;
	}

	if (!g_snapshotSize)
        initSnapshot();

	const char* prefix = "full_page_";
	char* filename = (char*) malloc(strlen(prefix) + strlen(url) + 1);
	if (!filename) {
        free(url);
		return BEFail;
	}

	strcpy(filename, prefix);
	strcat(filename, url);

	char* path = getSnapshotFilepath(filename, NULL);
	free(url);
	free(filename);

    if (!path) {
        return BEFail;
    }

	GtkWidget* widget = ((BERealView*) view)->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
    if (!widget) {
    	free(path);
        return BEFail;
    }

	BEResult result = captureSnapshot(WEBKIT_WEB_VIEW(widget), path, scale);
	free(path);

	return result;
}

char* BE_GetSnapshotURL(const char* url, float scale)
{
	if(scale > 1.f || scale <= 0)
		return NULL;

	char* path = getSnapshotFilepath(url, "file://");
	if(!path)
	{
		return NULL;
	}

	if(!access(path + 7, F_OK))	// skip to "file://"
	{
		return path;
	}
	else
	{
		free(path);
		return NULL;
	}
}

BEResult BE_ExecuteScript(BEView view, const char* script)
{
	if(!view || !script)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebKit::core(webview)->mainFrame()->script()->executeScript(WTF::String::fromUTF8(script), true);
	return BEPass;
}

static gboolean handleKeyEvent(void* data)
{
	BEKeyEvent* event = (BEKeyEvent*) data;

	GdkEvent gdkEvent;
	memset(&gdkEvent, 0, sizeof(gdkEvent));
	gdkEvent.key.keyval = event->key;

#if ENABLE(TVGG_FLASH_IME)
	if(imeStatus != BEIMEFlashShow)
#endif
	{
		GdkKeymapKey* keys;
		gint n_keys;
		if(gdk_keymap_get_entries_for_keyval(gdk_keymap_get_default(), event->key, &keys, &n_keys))
		{
			gdkEvent.key.hardware_keycode = keys[0].keycode;
			g_free(keys);
		}
	}

    if (event->key == 0xFF1B)
    {
        //fprintf(stderr, "[BR] %s %d keyval %d\n", __func__, __LINE__, event->key);
        gdkEvent.key.hardware_keycode = 0xFF1B;
    }

	gdkEvent.key.window = gtk_widget_get_window(GTK_WIDGET(event->view->webview));
	if(!gdkEvent.key.window)
	{
		gdkEvent.key.window = gdk_get_default_root_window();
	}

	GObject* obj = G_OBJECT(event->view->webview);
	gboolean result;

	if(event->isPress)
	{
		gdkEvent.key.type = GDK_KEY_PRESS;
		g_signal_emit_by_name(obj, "key-press-event", &gdkEvent.key, &result);
	}
	else
	{
		gdkEvent.key.type = GDK_KEY_RELEASE;
		g_signal_emit_by_name(obj, "key_release_event", &gdkEvent.key, &result);
	}

	free(event);
	return false;
}

BEResult BE_SendKeyEvent(BEView view, unsigned int key, bool isPress)
{
	if(!view)
	{
		return BEFail;
	}

	BEKeyEvent* event = (BEKeyEvent*) malloc(sizeof(BEKeyEvent));
	if(!event)
	{
		return BEFail;
	}

	switch(key)
	{
	case VK_RETURN:
#ifdef GTK_API_VERSION_2
		key = GDK_KP_Enter;
#else
		key = GDK_KEY_KP_Enter;
#endif
		break;
	default:
		break;
	}

	event->view = (BERealView*) view;
	event->isPress = isPress;
	event->key = key;
	if(!g_timeout_add(0, handleKeyEvent, event))
	{
		free(event);
		return BEFail;
	}

	return BEPass;
}

// 20100909 : naseogi(jangjoonho@samsung.com)
extern void GetMousePointer(int* x, int* y);

void BE_SendMouseEvent(bool isPress)
{
//	printf(">>>>>>>>>>>>>>> naseogi (%s-%d)\n", __func__, __LINE__);fflush(stdout);
#if 1
	int xx, yy;
	GetMousePointer(&xx, &yy);
	GdkDisplay *display = NULL;
	GdkScreen *screen = NULL;

	/* get default display and screen */
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);

	/* set new cusor position */
	gdk_display_warp_pointer (display, screen, xx, yy);
#endif
//	printf(">>>>>>>>>>>>>>> naseogi (%s-%d)\n", __func__, __LINE__);fflush(stdout);
	GdkEvent* event = gdk_event_new(GDK_BUTTON_PRESS);

	event->button.send_event = false;
//	event->button.time = gtk_util::XTimeNow();

	gint x, y;
	GtkWidget* grab_widget = gtk_grab_get_current();
	if (grab_widget) {
		// If there is a grab, we need to target all events at it regardless of
		// what widget the mouse is over.
		event->button.window = gtk_widget_get_window(grab_widget);
		gdk_window_get_pointer(event->button.window, &x, &y, NULL);
	} else {
		event->button.window = gdk_window_at_pointer(&x, &y);
	}

//	printf(">>>>>>>>>>>>>>> naseogi (%d, %d) (%s-%d)\n", x, y, __func__, __LINE__);fflush(stdout);
	g_object_ref(event->button.window);
	event->button.x = x;
	event->button.y = y;
	gint origin_x, origin_y;
	gdk_window_get_origin(event->button.window, &origin_x, &origin_y);
	event->button.x_root = x + origin_x;
	event->button.y_root = y + origin_y;
//	printf(">>>>>>>>>>>>>>> naseogi (%d, %d) (%s-%d)\n", origin_x, origin_y, __func__, __LINE__);fflush(stdout);

	event->button.axes = NULL;
	GdkModifierType modifier;
	gdk_window_get_pointer(event->button.window, NULL, NULL, &modifier);
	event->button.state = modifier;
	event->button.button = 1;
#ifdef GTK_API_VERSION_2
	event->button.device = gdk_display_get_default()->core_pointer;
#else
	event->button.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	event->button.type = GDK_BUTTON_PRESS;
	if (isPress)
		gdk_event_put(event);

	// Also send a release event.
	GdkEvent* release_event = gdk_event_copy(event);
	release_event->button.type = GDK_BUTTON_RELEASE;
	release_event->button.time++;
	if (!isPress)
		gdk_event_put(release_event);

	gdk_event_free(event);
	gdk_event_free(release_event);
	gdk_display_warp_pointer (display, screen, 0, 50);
}

/*
static void contentsToWindowPos(WebKitWebView* webview, int* x, int* y)
{
	float rate = (float) webkit_web_view_get_zoom_level(webview);
	(*x) *= rate;
	(*y) *= rate;

	WebCore::FrameView* frameview = WebKit::core(webview)->mainFrame()->view();
	WebCore::IntPoint scrollPos = frameview->scrollPosition();
	(*x) -= scrollPos.x();
	(*y) -= scrollPos.y();
}

static void getRootPosition(GtkWidget* widget, int* x, int* y)
{
	do
	{
		(*x) += widget->allocation.x;
		(*y) += widget->allocation.y;
	} while((widget = gtk_widget_get_parent(widget)));
}

BEResult BE_SendMotionEvent(BEView view, int x, int y)
{
	if(!view)
		return BEFail;

#if 0
	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	contentsToWindowPos(webview, &x, &y);
	getRootPosition(GTK_WIDGET(webview), &x, &y);

	GdkDisplay* display = gdk_display_get_default();
	GdkScreen* screen = gdk_display_get_default_screen(display);
	gdk_display_warp_pointer(display, screen, x, y);
#endif

	BERealView* realView = (BERealView*) view;
	if(!realView->lastButtonType)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(realView->webview);
	GtkWidgetClass* viewClass = GTK_WIDGET_GET_CLASS(webview);
	if(!viewClass->motion_notify_event)
		return BEFail;

	GdkEventMotion event;
	memset(&event, 0, sizeof(event));
	event.type = GDK_MOTION_NOTIFY;

	contentsToWindowPos(webview, &x, &y);
	event.x = x;
	event.y = y;
	getRootPosition(GTK_WIDGET(webview), &x, &y);
	event.x_root = x;
	event.y_root = y;
	event.window = GTK_WIDGET(webview)->window;

	WebKit::core(webview)->mainFrame()->view()->layout();

	viewClass->motion_notify_event(GTK_WIDGET(webview), &event);

	return BEPass;
}

BEResult BE_SendButtonPressEvent(BEView view, int x, int y, BEButton type, int clickCount)
{
	if(!view || clickCount < 1)
		return BEFail;

	BERealView* realView = (BERealView*) view;
	if(realView->lastButtonType)
		BE_SendButtonReleaseEvent(view, x, y);

	WebKitWebView* webview = WEBKIT_WEB_VIEW(realView->webview);
	GtkWidgetClass* viewClass = GTK_WIDGET_GET_CLASS(webview);
	if(!viewClass->button_press_event)
		return BEFail;

	GdkEventButton event;
	memset(&event, 0, sizeof(event));
	event.type = GDK_BUTTON_PRESS;
	if(type == BELeftBtn)
		event.button = 1;
	else if(type == BERightBtn)
		event.button = 3;

	contentsToWindowPos(webview, &x, &y);
	event.x = x;
	event.y = y;
	getRootPosition(GTK_WIDGET(webview), &x, &y);
	event.x_root = x;
	event.y_root = y;
	event.window = GTK_WIDGET(webview)->window;

	WebKit::core(webview)->mainFrame()->view()->layout();

//	gboolean result;
//	g_signal_emit_by_name(webview, "button_press_event", &event, &result);
	viewClass->button_press_event(GTK_WIDGET(webview), &event);
	if(clickCount == 2)
	{
		event.type = GDK_2BUTTON_PRESS;
		viewClass->button_press_event(GTK_WIDGET(webview), &event);
	}
	else if(clickCount == 3)
	{
		event.type = GDK_3BUTTON_PRESS;
		viewClass->button_press_event(GTK_WIDGET(webview), &event);
	}

	realView->lastButtonType = event.button;

	return BEPass;
}

BEResult BE_SendButtonReleaseEvent(BEView view, int x, int y)
{
	if(!view)
		return BEFail;

	BERealView* realView = (BERealView*) view;
	if(!realView->lastButtonType)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(realView->webview);
	GtkWidgetClass* viewClass = GTK_WIDGET_GET_CLASS(webview);
	if(!viewClass->button_release_event)
		return BEFail;

	GdkEventButton event;
	memset(&event, 0, sizeof(event));
	event.type = GDK_BUTTON_RELEASE;
	event.button = realView->lastButtonType;

	contentsToWindowPos(webview, &x, &y);
	event.x = x;
	event.y = y;
	getRootPosition(GTK_WIDGET(webview), &x, &y);
	event.x_root = x;
	event.y_root = y;
	event.window = GTK_WIDGET(webview)->window;

	WebKit::core(webview)->mainFrame()->view()->layout();

//	gboolean result;
//	g_signal_emit_by_name(webview, "button_release_event", &event, &result);
	viewClass->button_release_event(GTK_WIDGET(webview), &event);

	realView->lastButtonType = 0;

	return BEPass;
}
*/

float BE_GetZoomLevel(BEView view)
{
	if(!view)
		return .0f;

	return (float) webkit_web_view_get_zoom_level(WEBKIT_WEB_VIEW(((BERealView*)view)->webview));
}

BEResult BE_SetZoomLevel(BEView view, float level)
{
	if(!view || level <= .0f)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);

	webkit_web_view_set_zoom_level(webview, level);
	return BEPass;
}

BEResult BE_CanHistoryGo(BEView view, int steps)
{
	if(!view || !webkit_web_view_can_go_back_or_forward(((BERealView*)view)->webview, steps))
		return BEFail;
	return BEPass;
}

BEResult BE_HistoryGo(BEView view, int steps)
{
	if(!view || !webkit_web_view_can_go_back_or_forward(((BERealView*)view)->webview, steps))
		return BEFail;

	webkit_web_view_go_back_or_forward(WEBKIT_WEB_VIEW(((BERealView*)view)->webview), steps);
	return BEPass;
}

BEDocument BE_GetMainDocument(BEView view)
{
	if(!view)
		return NULL;

	WebCore::Page* page = WebKit::core(WEBKIT_WEB_VIEW(((BERealView*)view)->webview));
	return (BEDocument) page->mainFrame()->document();
}

BEDocument BE_GetContentDocument(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isHTMLElement())
		return NULL;

	WebCore::Element* elm = (WebCore::Element*) node;
	if(elm->hasTagName(WebCore::HTMLNames::iframeTag)
			|| elm->hasTagName(WebCore::HTMLNames::frameTag))
		return static_cast<WebCore::HTMLFrameElementBase*>(elm)->contentDocument();
	else
		return NULL;
}

char* BE_GetURL(BEDocument doc)
{
	if(!doc)
		return NULL;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	if(!realDoc->isDocumentNode())
		return NULL;

	return copyCString(realDoc->url().string().utf8());
}

char* BE_GetBaseURL(BEDocument doc)
{
	if(!doc)
		return NULL;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	if(!realDoc->isDocumentNode())
		return NULL;

	return copyCString(realDoc->baseURL().string().utf8());
}

BENode BE_GetFocusNode(BEView view)
{
	if(!view)
	{
		return NULL;
	}

	WebCore::Frame* frame = WebKit::core(((BERealView*)view)->webview)->focusController()->focusedOrMainFrame();
	if(!frame)
	{
		return NULL;
	}

	return (BENode) frame->document()->focusedNode();
}

void BE_SetFocusNode(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isElementNode())
	{
		return;
	}

	WebCore::Element* elm = (WebCore::Element*) node;
	elm->focus();
}

void BE_SetBlurNode(BENode node)
{
        if(!node || !((WebCore::Node*) node)->isElementNode())
        {
                return;
        }

        WebCore::Element* elm = (WebCore::Element*) node;
        elm->blur();
}


BENode BE_GetDocumentElement(BEDocument doc)
{
	if(!doc)
		return NULL;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	if(!realDoc->isDocumentNode())
		return NULL;

	return (BENode) realDoc->documentElement();
}

BENode BE_GetBody(BEDocument doc)
{
	if(!doc)
		return NULL;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	if(!realDoc->isDocumentNode())
		return NULL;

	return (BENode) realDoc->body();
}

BENode BE_GetNodeByPoint(BEDocument doc, int x, int y)
{
	if(!doc)
		return NULL;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	float rate = (float) realDoc->frame()->pageZoomFactor();
	x *= rate;
	y *= rate;

	WebCore::IntPoint point(x, y);
	WebCore::HitTestRequest request(WebCore::HitTestRequest::Active);
	WebCore::HitTestResult result(point);
	realDoc->renderView()->layer()->hitTest(request, result);

	return (BENode) result.innerNode();
}

BEResult BE_SelectWordAtMousePoint(BEView view, BEMouseEvent* aMouseEvent)
{
    /*BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();

    GdkEventMotion gdkMotionEvent;
	memset(&gdkMotionEvent, 0, sizeof(GdkEventMotion));

	gdkMotionEvent.type = GDK_MOTION_NOTIFY;
	gdkMotionEvent.window = GTK_WIDGET(webView)->window;
	gdkMotionEvent.time = 0;
	gdkMotionEvent.x = aMouseEvent->x;
	gdkMotionEvent.y = aMouseEvent->y;
	gdkMotionEvent.x_root = (gfloat)aMouseEvent->x;
	gdkMotionEvent.y_root = (gfloat)aMouseEvent->y;
	gdkMotionEvent.axes = NULL;
	gdkMotionEvent.state = GDK_SHIFT_MASK;	// which button?
	gdkMotionEvent.is_hint = 0;
	gdkMotionEvent.device = gdk_display_get_default()->core_pointer;

    WebCore::PlatformMouseEvent mouseEvent(&gdkMotionEvent);
	mouseEvent.setClickCount(2);

    WebCore::HitTestRequest request(WebCore::HitTestRequest::Active);

    WebCore::FrameView* frameview = frame->view();
    WebCore::IntPoint documentPoint = frameview ? frameview->windowToContents(mouseEvent.pos()) : mouseEvent.pos();

    frame->document()->prepareMouseEvent(request, documentPoint, mouseEvent);

    WebCore::MouseEventWithHitTestResults result = frame->document()->prepareMouseEvent(request, documentPoint, mouseEvent);

    WebCore::Node* innerNode = result.targetNode();
    WebCore::VisibleSelection newSelection;

    WebCore::VisiblePosition pos(innerNode->renderer()->positionForPoint(result.localPoint()));

    if (pos.isNotNull()) {
        newSelection = WebCore::VisibleSelection(pos);
        newSelection.expandUsingGranularity(WebCore::WordGranularity);
    }

    if (newSelection.isRange()) {
        frame->setSelectionGranularity(WebCore::WordGranularity);
        if (result.event().clickCount() == 2 && frame->editor()->isSelectTrailingWhitespaceEnabled())
            newSelection.appendTrailingWhitespace();
    }

    frame->selection()->setSelection(newSelection);*/

    BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return BEFail;

	WebCore::SelectionController *sc = frame->selection();
	if (!sc || sc->isNone())
		return BEFail;

	WebCore::IntPoint aP(aMouseEvent->x, aMouseEvent->y);
	WebCore::VisiblePosition* pos = new WebCore::VisiblePosition(frame->visiblePositionForPoint(aP));

    if (!pos)
        return BEFail;

    WebCore::VisibleSelection newSelection(*pos);
    newSelection.expandUsingGranularity(WebCore::WordGranularity);

    if (newSelection.isRange())
        newSelection.appendTrailingWhitespace();

    frame->selection()->setSelection(newSelection, WebCore::WordGranularity);

    delete pos;	// prevent : RESOURCE LEAK
    return BEPass;
}

BEResult BE_CreateAndSelectRange(BEView view, BEMouseEvent* aMouseEvent, BEMouseEvent* bMouseEvent)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return BEFail;

	WebCore::SelectionController *sc = frame->selection();
	if (!sc || sc->isNone())
		return BEFail;

	WebCore::IntPoint aP(aMouseEvent->x, aMouseEvent->y);
	WebCore::VisiblePosition* aPosition = new WebCore::VisiblePosition(frame->visiblePositionForPoint(aP));

	WebCore::IntPoint bP(bMouseEvent->x, bMouseEvent->y);
	WebCore::VisiblePosition* bPosition = new WebCore::VisiblePosition(frame->visiblePositionForPoint(bP));

	if (!aPosition || !bPosition)
		return BEFail;

	//WebCore::Position extent = sc->extent();

	//if (WebCore::comparePositions(position->deepEquivalent(), extent) < 0) {
		// Change the 'base' and 'extent' positions to 'start' and 'end' positions.
        // We do it, because without this, the other modification of the selection
        // would destroy the 'start' and/or 'end' positions and set them to
        // the 'base'/'extent' positions accordingly
        WebCore::VisibleSelection sel(sc->start(), sc->end());
        sc->setSelection(sel);

        sc->setBase(*aPosition, true);
        sc->setExtent(*bPosition, true);

        // This forces webkit to show selection
        // frame->invalidateSelection();

        // Unused
        //int top_x, top_y, top_h, bottom_x, bottom_y, bottom_h;

        // Find handlers positions
        //extentSelection(view, &top_x, &top_y, &top_h, &bottom_x, &bottom_y, &bottom_h);
    //}

	delete aPosition;
	delete bPosition;

	return BEPass;
}

void BE_SetSelectionFromNone(BEDocument doc)
{
    WebCore::Document* realDoc = (WebCore::Document*) doc;
    realDoc->frame()->selection()->setSelectionFromNone();
}

BENode BE_GetElementById(BEDocument doc, const char* id)
{
	if(!doc || !id)
		return 0;

	WebCore::Document* realDoc = (WebCore::Document*) doc;
	return (BENode) realDoc->getElementById(id);
}

BENode BE_GetSpatialFocusableNode(BEView view, BEDirection dir)
{
	BE_CreateSPNavigator((SpWebView) view, false);

	BENode result;
	switch(dir)
	{
	case BELeft:
		result = BE_SPGetNextNode(FocusDirection_Left);
		break;
	case BERight:
		result = BE_SPGetNextNode(FocusDirection_Right);
		break;
	case BETop:
		result = BE_SPGetNextNode(FocusDirection_Up);
		break;
	case BEBottom:
		result = BE_SPGetNextNode(FocusDirection_Down);
		break;
	default:
		result = NULL;
	}

	BE_DestroySPNavigator();

	return result;
}

BENode BE_GetParent(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return (BENode) realNode->parentNode();
}

BENode BE_GetPreviousSibling(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return (BENode) realNode->previousSibling();
}

BENode BE_GetNextSibling(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return (BENode) realNode->nextSibling();
}

BENode BE_GetFirstChild(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return (BENode) realNode->firstChild();
}

BENode BE_GetLastChild(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return (BENode) realNode->lastChild();
}

int BE_GetNodeType(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return static_cast<int>(realNode->nodeType());
}

char* BE_GetNodeName(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return copyCString(realNode->nodeName().utf8());
}

char* BE_GetNodeInfoFromPoint(BEView view, int x, int y)
{
//	if(!view)
//		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	/*WebCore::Frame* mainFrame = WebKit::core(webview)->mainFrame();
	WebCore::IntPoint pt = mainFrame->view()->scrollPosition();
	x += pt.x();
	y += pt.y();*/
	BENode pNode = BE_GetNodeByPoint( doc, x, y );
	WebCore::Node* realNode = (WebCore::Node*) pNode;
	char* nodeName = BE_GetNodeName(pNode);
	if(nodeName != NULL)
	{
		while ( !strcmp(nodeName,"#text") )
		{
			realNode = realNode->parentNode();
			pNode = (BENode)realNode;
			free(nodeName);
			nodeName = BE_GetNodeName(pNode);
			if(nodeName == NULL)
			{
				break;
			}
		}
		if(nodeName != NULL)
		{
			free(nodeName);
		}
	}

	//WebCore::Node* realNode = (WebCore::Node*) pNode;
	char* tagname = BE_GetNodeName(pNode);
	char* type = BE_GetInputType(pNode);
	char* src = BE_GetAttribute(pNode, "src");
	char* href = BE_GetAttribute(pNode, "href");

	int len_tagname = 0;
	if(tagname != NULL)
	{
		len_tagname = strlen(tagname);
	}
	int len_type = 0;
	if(type != NULL)
	{
		len_type = strlen(type);
	}
	int len_src = 0;
	if(src != NULL)
	{
		len_src = strlen(src);
	}
	int len_href = 0;
	if(href != NULL)
	{
		len_href = strlen(href);
	}
	char* strRet = (char*) malloc( len_tagname + len_type + len_src + len_href + 4 );
	if(tagname != NULL)
	{
		strcpy( strRet, tagname );
		free(tagname);
	}
	strcat( strRet, ",");
	if(type != NULL)
	{
		strcat( strRet, type );
		free(type);
	}
	strcat( strRet, "," );
	if(src != NULL)
	{
		strcat( strRet, src );
		free(src);
	}
	strcat( strRet, "," );
	if(href != NULL)
	{
		strcat( strRet, href );
		free(href);
	}
	return strRet;
}

char* BE_GetNodeValue(BENode node)
{
	if(!node)
		return NULL;

	WebCore::Node* realNode = (WebCore::Node*) node;
	return copyCString(realNode->nodeValue().utf8());
}

BERect BE_GetNodeRect(BENode node)
{
	BERect rect;
	getNodeRect(node, false, &rect, NULL);
	return rect;
}

BERect BE_GetAbsoluteNodeRect(BENode node)
{
	BERect rect;
	getNodeRect(node, true, &rect, NULL);
	return rect;
}

char* BE_GetAttribute(BENode node, const char* name)
{
	if(!node || !((WebCore::Node*) node)->isElementNode())
		return NULL;

	WebCore::Element* elm = (WebCore::Element*) node;
	return copyCString(elm->getAttribute(name).string().utf8());
}

short BE_GetTabIndex(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isHTMLElement())
		return -1;

	WebCore::HTMLElement* elm = (WebCore::HTMLElement*) node;
	return elm->tabIndex();
}

BENode* BE_GetElementsByTagName(BENode node, const char* tagname, unsigned* count)
{
	if(!node || !tagname)
	{
		*count = 0;
		return 0;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	WTF::RefPtr<WebCore::NodeList> list = realNode->getElementsByTagName(tagname);
	*count = list->length();

	BENode* nodelist = static_cast<BENode*>(malloc(sizeof(BENode) * *count));
	if(nodelist == 0)
	{
		*count = 0;
		return 0;
	}

	for(unsigned i = 0; i < *count; i++)
		nodelist[i] = static_cast<BENode>(list->item(i));

	return nodelist;
}

char* BE_GetInnerText(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isElementNode())
		return NULL;

	WebCore::Element* elm = (WebCore::Element*) node;
	return copyCString(elm->innerText().utf8());
}

char* BE_GetInnerHTML(BENode node)
{
	if(!node || !((WebCore::Node*) node)->isHTMLElement())
		return NULL;

	WebCore::HTMLElement* elm = (WebCore::HTMLElement*) node;
	return copyCString(elm->innerHTML().utf8());
}

char* BE_GetAbsoluteURL(BENode node, const char* url)
{
	if(!node || !url)
		return NULL;

	WebCore::Document* doc = ((WebCore::Node*) node)->document();
	if(!doc)
		return NULL;

	WTF::String urlString = url;
	return copyCString(doc->completeURL(urlString).string().utf8());
}

BEResult BE_Click(BENode node)
{
	if(!node)
		return BEFail;

	WebCore::Node* realNode = (WebCore::Node*) node;

	realNode->document()->frame()->loader()->resetMultipleFormSubmissionProtection();

	INIT_JS_GLOBAL_DATA(globalData, prevData);
	realNode->dispatchSimulatedClick(0, true, false);
	RESTORE_JS_GLOBAL_DATA(globalData, prevData);

	return BEPass;
}

static void getLocalRect(WebCore::RenderLayer* layer, WebCore::RenderObject* renderer, WebCore::IntRect& rect)
{
	WebCore::FloatPoint localPoint;
	WebCore::TransformState transformState(WebCore::TransformState::ApplyTransformDirection, localPoint);
	renderer->mapLocalToContainer(layer->renderer(), false, false, transformState);
	transformState.flatten();

	WebCore::FloatPoint absPos = transformState.lastPlanarPoint();
	Vector<WebCore::IntRect> rects;
	renderer->absoluteRects(rects, absPos.x(), absPos.y());

	size_t n = rects.size();
	if(!n)
	{
		rect.setWidth(0);
		return;
	}

	rect = rects[0];
	for(size_t i = 1; i < n; ++i)
		rect.unite(rects[i]);
}

BEResult BE_IsVisible(BENode node)
{
	if(!node)
		return BEFail;

	WebCore::Node* realNode = (WebCore::Node*) node;
	WebCore::RenderObject* renderer = realNode->renderer();
	if(!renderer || renderer->style()->visibility() == WebCore::HIDDEN)
		return BEFail;

	WebCore::RenderLayer* layer = renderer->enclosingLayer();
	if(!layer)
		return BEFail;

	WebCore::IntRect rect;
	getLocalRect(layer, renderer, rect);

	while(1) {
		WebCore::IntRect boundRect = layer->localBoundingBox();
		rect.intersect(boundRect);
		if(rect.isEmpty())
			return BEFail;

		WebCore::RenderLayer* ancestor = layer->isNormalFlowOnly() ? layer->parent() : layer->stackingContext();
		if(!layer->renderer() || layer->renderer()->isRenderView() || ancestor->renderer()->isRoot())
			break;

		int offsetX = 0, offsetY = 0;
		layer->convertToLayerCoords(ancestor, offsetX, offsetY);
		rect.move(offsetX, offsetY);

		layer = ancestor;
		assert(layer);
	}

	return BEPass;
}

BEResult BE_ScrollByNode(BENode node, int marginX, int marginY,
	BEScrollMode visibleX, BEScrollMode hiddenX, BEScrollMode partialX,
	BEScrollMode visibleY, BEScrollMode hiddenY, BEScrollMode partialY)
{
	if(!node)
		return BEFail;

	WebCore::Element* realNode = (WebCore::Element*) node;
	WebCore::RenderObject* renderer = realNode->renderer();
	if(!renderer || renderer->style()->visibility() == WebCore::HIDDEN)
		return BEFail;

	WebCore::RenderLayer* layer = renderer->enclosingLayer();

	if(!layer)
		return BEFail;

	WebCore::IntRect rect = realNode->getRect();
	rect.inflateX(marginX);
	rect.inflateY(marginY);

	WebCore::ScrollAlignment alignX = { (WebCore::ScrollBehavior) visibleX,
										(WebCore::ScrollBehavior) hiddenX,
										(WebCore::ScrollBehavior) partialX };
	WebCore::ScrollAlignment alignY = { (WebCore::ScrollBehavior) visibleY,
										(WebCore::ScrollBehavior) hiddenY,
										(WebCore::ScrollBehavior) partialY };

	layer->scrollRectToVisible(rect, false, alignX, alignY);


	return BEPass;
}

static WebCore::StyledElement* lookupChildElm(WebCore::StyledElement* parent, const char* tagname)
{
	WebCore::Element* uiIter = parent->lastElementChild();
	while(uiIter)
	{
		if(uiIter->tagName() == tagname)
			return static_cast<WebCore::StyledElement*>(uiIter);

		uiIter = uiIter->previousElementSibling();
	}

	return NULL;
}

static WebCore::StyledElement* appendElm(WebCore::Document* doc, WebCore::StyledElement* parent, const char* tagname)
{
    WebCore::ExceptionCode ec = 0;
	WTF::PassRefPtr<WebCore::Element> uiElmPtr = doc->createElement(tagname, ec);
	if(ec != 0)
		return NULL;

	WebCore::StyledElement* uiElm = static_cast<WebCore::StyledElement*>(uiElmPtr.get());
	parent->appendChild(uiElm, ec);
	if(ec != 0)
		return NULL;

	WTF::String value = "hidden";
	uiElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);

	return uiElm;
}

static WebCore::StyledElement* getChildElm(WebCore::Document* doc, WebCore::StyledElement* parent, const char* tagname)
{
	WebCore::StyledElement* child = lookupChildElm(parent, tagname);
	if(child)
		return child;

	return appendElm(doc, parent, tagname);
}

static WebCore::StyledElement* getUIRoot(WebCore::Document* doc)
{
	WebCore::Element* uiRoot = static_cast<WebCore::Element*>(doc->body());
	if(!uiRoot)
		return NULL;

	while((uiRoot = uiRoot->previousElementSibling()))
//	while((uiRoot = uiRoot->nextElementSibling()))
	{
		if(uiRoot->tagName() == "__UI" && uiRoot->getAttribute("id") == "__ui")
		{
			WTF::String value = "";
			static_cast<WebCore::StyledElement*>(uiRoot)->getInlineStyleDecl()
				->setProperty(WebCore::CSSPropertyDisplay, value, false);
			return static_cast<WebCore::StyledElement*>(uiRoot);
		}
	}

	WebCore::ExceptionCode ec = 0;
	WTF::PassRefPtr<WebCore::Element> uiElmPtr = doc->createElement("__UI", ec);
	if(ec != 0)
		return NULL;

	uiRoot = uiElmPtr.get();
	uiRoot->setAttribute("id", "__ui", ec);
	if(ec != 0)
		return NULL;

	doc->documentElement()->insertBefore(uiRoot, static_cast<WebCore::Node*>(doc->body()), ec);
//	doc->documentElement()->appendChild(uiRoot, ec);
	if(ec != 0)
		return NULL;

	return static_cast<WebCore::StyledElement*>(uiRoot);
}

BEResult BE_ShowBlockedWebPage(BEView view, BERect* rectlist, unsigned count,
		unsigned borderWidth, const char* borderColor, unsigned borderRadius)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	WTF::String value = "";
	uiRoot->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);

	if(!rectlist || count == 0)
		return BEPass;

	uiRoot->removeAllChildren();

    WebCore::ExceptionCode ec = 0;

	for(unsigned i=0; i < count; ++i)
	{
		WebCore::StyledElement* blockElm = appendElm(doc, uiRoot, "__BLOCK");
		if(!blockElm)
			return BEFail;

		WTF::String style = "position:absolute;"
			"z-index:99997;"
			"left:" + WTF::String::number(rectlist[i].left - (int) borderWidth) + "px;"
			"top:" + WTF::String::number(rectlist[i].top - (int) borderWidth) + "px;"
			"width:" + WTF::String::number(rectlist[i].width) + "px;"
			"height:" + WTF::String::number(rectlist[i].height) + "px;"
			"border:" + borderColor + " solid " + WTF::String::number(borderWidth) + "px;";
		if(borderRadius > 0)
			style += "-webkit-border-radius:" + WTF::String::number(borderRadius) +"px;";

		blockElm->setAttribute("style", style, ec);
		if(ec != 0)
			return BEFail;
	}

	return BEPass;
}

BEResult BE_HideBlockedWebPage(BEView view)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	WTF::String value = "hidden";
	uiRoot->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);

	return BEPass;
}


static gboolean onAfterMove(void* data)
{
	if(!data)
		return false;

	((BERealView*)data)->timerID = 0;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)data)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();

	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return false;

	WebCore::StyledElement* focusElm = NULL;
 	WebCore::StyledElement* elm;
	WTF::String value = "";

	elm = lookupChildElm(uiRoot, "__SHADOW");
	if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);

	elm = lookupChildElm(uiRoot, "__FOCUS_ELM");
	if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
		focusElm = elm;
	else
	{
		elm = lookupChildElm(uiRoot, "__FOCUS_BLK");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			focusElm = elm;

		elm = lookupChildElm(uiRoot, "__DIM");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
	}

	if(focusElm)
	{
		elm = lookupChildElm(focusElm, "__LEFT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__RIGHT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__TOP");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__BOTTOM");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
	}

    return false;
}

static void moveFocus(BERealView* view, WebCore::Document* doc, WebCore::StyledElement* uiRoot,
		WebCore::StyledElement* focusElm, BERect* rect, unsigned arrowDir,
		unsigned borderWidth, const char* borderColor, unsigned borderRadius,
		unsigned arrowWidth, unsigned arrowMargin,
		bool isDrawDimLayer, float transitionDuration, bool isBlockMove)
{
 	WebCore::StyledElement* elm;
	WTF::String value;

	if(rect->width == 0 && rect->height == 0)
	{
		value = "none";
		focusElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);

		elm = lookupChildElm(uiRoot, "__SHADOW");
		if(elm)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);

		elm = lookupChildElm(uiRoot, "__DIM");
		if(elm)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);

		return;
	}
	WTF::String style = "position:absolute;"
//		"-webkit-box-shadow:0 5px 5px #888888;"
		"z-index:99999;"
		"left:" + WTF::String::number(rect->left - (int) borderWidth) + "px;"
		"top:" + WTF::String::number(rect->top - (int) borderWidth) + "px;"
		"width:" + WTF::String::number(rect->width) + "px;"
		"height:" + WTF::String::number(rect->height) + "px;"
		"border:" + borderColor + " solid " + WTF::String::number(borderWidth) + "px;";

	if(borderRadius > 0)
		style += "-webkit-border-radius:" + WTF::String::number(borderRadius) +"px;";

	float transitionDelay;
	if(isBlockMove)
		transitionDelay = 1.2f;
	else
		transitionDelay = .2f;

	if(transitionDuration > transitionDelay)
	{
		style += "-webkit-transition-property:top,left,width,height;"
			"-webkit-transition-duration:" + WTF::String::number(transitionDuration) + "s;";
	}
	else
	{
		style += "-webkit-transition-property:top,left,width,height;"
			"-webkit-transition-delay:.01s;";
	}

    WebCore::ExceptionCode ec = 0;
	focusElm->setAttribute("style", style, ec);
	if(ec != 0)
	{
		memset(rect, 0, sizeof(BERect));
		return;
	}

	if(!isBlockMove)
		elm = getChildElm(doc, uiRoot, "__SHADOW");
	else
		elm = NULL;

	if(elm)
	{
		const int shadowWidth = 7;

		value = "absolute";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyPosition, value, false);
		value = "url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAYAAACNiR0NAAAAsUlEQVQ4"
				"je2TbQ6DIAxA3yY4zO6y+59jl1EcS/aDklSsYuJ++pLGD+ijphUuznIz3jnASzj1DJCAj7qW+0VyjQcGIE"
				"j0EoggSozqkKYwAE8RPyqhBzolG3WyJXQiHIC3sQ7wAr61bE/YS2VbBFXtgvuOcLVZoZvWFJ7CEiZgJn/S"
				"FubItIRxRzjJ+upQqylJEjpyN+uxmcjdjVaFlrAkQR6Nme3BPlRh2VQqbf16F3/mBxPfNhLvuoq8AAAAAE"
				"lFTkSuQmCC) " + WTF::String::number(shadowWidth) + " stretch";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyWebkitBorderImage, value, false);
		value = WTF::String::number(shadowWidth) + "px";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyBorderWidth, value, false);

		value = WTF::String::number((float) rect->left - borderWidth * 2 - (float) shadowWidth * 0.1f) + "px";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyLeft, value, false);
		value = WTF::String::number((float) rect->top - borderWidth * 2 - (float) shadowWidth * 0.1f) + "px";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyTop, value, false);
		value = WTF::String::number((float) rect->width + borderWidth * 2 - (float) shadowWidth * 0.65f) + "px";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyWidth, value, false);
		value = WTF::String::number((float) rect->height + borderWidth * 2 - (float) shadowWidth * 0.65f) + "px";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyHeight, value, false);

		value = "";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		value = "hidden";
		elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
	}

	style = "position:absolute;width:0;height:0;"
		"border-style:solid;"
		"border-width:" + WTF::String::number(arrowWidth) + "px;"
		"visibility:hidden;";

	if(arrowDir & BELeft)
	{
		elm = getChildElm(doc, focusElm, "__LEFT");
		if(elm)
		{
			value = style
				+ "border-color:transparent " + WTF::String(borderColor) + " transparent transparent;"
				"left:" + WTF::String::number(- (int) (arrowWidth * 2 + borderWidth + arrowMargin)) + "px;"
				"top:" + WTF::String::number(rect->height / 2 - (int) arrowWidth) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__LEFT");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BERight)
	{
		elm = getChildElm(doc, focusElm, "__RIGHT");
		if(elm)
		{
			value = style
				+ "border-color:transparent transparent transparent " + WTF::String(borderColor) + ";"
				"left:" + WTF::String::number(rect->width + borderWidth + arrowMargin) + "px;"
				"top:" + WTF::String::number(rect->height / 2 - (int) arrowWidth) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__RIGHT");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BETop)
	{
		elm = getChildElm(doc, focusElm, "__TOP");
		if(elm)
		{
			value = style
				+ "border-color:transparent transparent " + WTF::String(borderColor) + " transparent;"
				"left:" + WTF::String::number(rect->width / 2 - (int) arrowWidth) + "px;"
				"top:" + WTF::String::number(- (int) (arrowWidth * 2 + borderWidth + arrowMargin)) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__TOP");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BEBottom)
	{
		elm = getChildElm(doc, focusElm, "__BOTTOM");
		if(elm)
		{
			value = style
				+ "border-color:" + WTF::String(borderColor) + " transparent transparent transparent;"
				"left:" + WTF::String::number(rect->width / 2 - (int) arrowWidth) + "px;"
				"top:" + WTF::String::number(rect->height + borderWidth + arrowMargin) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__BOTTOM");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(isBlockMove)
	{
		if(isDrawDimLayer && doc->view())
		{
			WebCore::IntSize contentSize = doc->view()->contentsSize();
			float zoom = doc->frame()->pageZoomFactor();
			int width = contentSize.width() / zoom;
			int height = contentSize.height() / zoom;

			style = "position:absolute;"
				"visibility:visible;"
				"z-index:99996;";

			elm = getChildElm(doc, uiRoot, "__DIM");
			if(elm)
			{
				elm->setAttribute("style", style, ec);

				value = "0px";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyLeft, value, false);
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyTop, value, false);
				value = WTF::String::number(rect->width) + "px";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyWidth, value, false);
				value = WTF::String::number(rect->height) + "px";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyHeight, value, false);
				value = "url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAMAAAADCAYAAABWKLW/AAAAGklEQVQI"
						"mWNkYGBYygAFTFA6moGBgYERWQYAI1EBqhAKGogAAAAASUVORK5CYII=) 1 repeat";	// 0.5
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyWebkitBorderImage, value, false);
				value = WTF::String::number(rect->top) + "px "
					+ WTF::String::number(width - (rect->left + rect->width)) + "px "
					+ WTF::String::number(height - (rect->top + rect->height)) + "px "
					+ WTF::String::number(rect->left) + "px";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyBorderWidth, value, false);
				value = "hidden";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
			}
		}
		else
		{
			elm = lookupChildElm(uiRoot, "__DIM");
			if(elm)
			{
				value = "none";
				elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
			}
		}
	}

	rect->left += (int) borderWidth;
	rect->top += (int) borderWidth;

	if(transitionDuration >= 0.f)
	{
		if(view->timerID > 0)
			g_source_remove(view->timerID);
		view->timerID = g_timeout_add((transitionDuration + transitionDelay) * 1000, onAfterMove, (gpointer) view);
	}
}

BEResult BE_SetBlockFocus(BEView view, BERect rect, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, bool isDrawDimLayer, float transitionDuration)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	if(!doc || !doc->view())
		return BEFail;
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	WebCore::StyledElement* focusElm = getChildElm(doc, uiRoot, "__FOCUS_BLK");
	if(!focusElm)
		return BEFail;

	moveFocus((BERealView*) view, doc, uiRoot, focusElm, &rect, arrowDir, borderWidth,
		borderColor, borderRadius, arrowWidth, arrowMargin, isDrawDimLayer, transitionDuration, true);

 	WebCore::StyledElement* elmFocusElm = lookupChildElm(uiRoot, "__FOCUS_ELM");
	if(elmFocusElm)
	{
		WTF::String value = "none";
		elmFocusElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
	}

	return BEPass;
}

BEResult BE_ZoomPage(BEView view, float rate, BENode node )
{
#ifdef SMART_FOCUS_ZOOM
	if(!view)
	{
		return BEFail;
	}
    int width, height;
	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
    BE_GetViewRect(view,  &width, &height);
	BERect rect1 =  BE_GetAbsoluteNodeRect(node);//BE_GetNodeRect(node);
    float ratioWidth = ((float)width/rect1.width) ;
    BERealView* realview = (BERealView*)view;
	if(realview->nIsZoom == 0)
        {
		if(rect1.width < width)
		{
			if(rate != 0)
			{
				BE_SetZoomLevel(view,(((ratioWidth > rate)? rate : ratioWidth)));
			}
			else
			{
				BE_SetZoomLevel(view,ratioWidth);
			}
		}
	  	realview->prevBestNodeRect = BE_GetAbsoluteNodeRect(node);
        realview->nIsZoom =1;
	}
	else
	{
	    if((rect1.left == realview->prevBestNodeRect.left) && (rect1.top == realview->prevBestNodeRect.top))
		{
			BE_SetZoomLevel(view,1);
		    realview->nIsZoom = 0;
			realview->prevBestNodeRect =  BE_GetAbsoluteNodeRect(node);

		}
        else
		{
			if((rect1.width < width) && (ratioWidth != 1.0))
			{

				ratioWidth += 1;
				BE_SetZoomLevel(view,(((ratioWidth > 1.7)? 1.7 : ratioWidth)));
			}
			realview->prevBestNodeRect =  BE_GetAbsoluteNodeRect(node);//BE_GetNodeRect(node);
            realview->nIsZoom =1;
		}
	}
    BERect rect =  BE_GetAbsoluteNodeRect(node);//BE_GetNodeRect(node);


	WebCore::Frame* mainFrame = WebKit::core(webview)->mainFrame();
	WebCore::IntPoint pt = mainFrame->view()->scrollPosition();
	int shiftX=(pt.x() + (rect.left-rect1.left));
	int shiftY=(pt.y() + (rect.top-rect1.top));

	/*Code for shifting the node which is getting out of webview boundary*/
	if(pt.x() > rect.left)
	{
		shiftX = rect.left;
	}
	if(pt.y() > rect.top)
	{
		shiftY = rect.top;
	}
	if(rate != 0)
	{
		if(width < ((rect.left + rect.width) - pt.x()))
		{
			shiftX = pt.x() + ((rect.left + rect.width) - pt.x() - width);
		}
		if(height < (rect.top + rect.height) - pt.y())
		{
			if(shiftY < pt.y() + ((rect.top + rect.height) - height))
			{
				shiftY = pt.y() + ((rect.top + rect.height) - pt.y() - height);
			}
		}
	}
	else
	{
		shiftX = rect.left;
		shiftY = rect.top;
	}
	/*********************************************************************/

        //WebCore::IntPoint point((pt.x() + (rect.left-rect1.left)) , (pt.y() + (rect.top-rect1.top)));
	WebCore::IntPoint point(shiftX , shiftY);
	if(ratioWidth != 1.0)
	{
		mainFrame->view()->setScrollPosition(point);
	}
#endif
	return BEPass;
}


BEResult BE_ChangeScale(BEView view, float rate, BENode node, int left, int top,
		unsigned borderWidth, const char* borderColor, unsigned borderRadius, bool isDrawDimLayer)
{
	if(!view || !node)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	uiRoot->removeAllChildren();

	webkit_web_view_set_zoom_level(webview, rate);

	WebCore::StyledElement* focusElm = getChildElm(doc, uiRoot, "__FOCUS_BLK");
	if(!focusElm)
		return BEFail;

	BERect rect = BE_GetNodeRect(node);
	moveFocus((BERealView*) view, doc, uiRoot, focusElm, &rect, 0, borderWidth,
		borderColor, borderRadius, 0, 0, isDrawDimLayer, -1.f, true);

	rect.left -= left;
	rect.top -= top;

	WebCore::Frame* mainFrame = WebKit::core(webview)->mainFrame();
	WebCore::IntPoint point((rect.left * rate) + left, (rect.top * rate) + top);
	mainFrame->view()->setScrollPosition(point);

	return BEPass;
}

BEResult BE_SetElementFocus(BEView view, BENode node, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, float transitionDuration)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

 	WebCore::StyledElement* focusElm = getChildElm(doc, uiRoot, "__FOCUS_ELM");
	if(!focusElm)
		return BEFail;

	BERect rect;
	if(node)
		rect = BE_GetNodeRect(node);
	else
		memset(&rect, 0, sizeof(BERect));

	moveFocus((BERealView*) view, doc, uiRoot, focusElm, &rect, arrowDir, borderWidth,
		borderColor, borderRadius, arrowWidth, arrowMargin, false, transitionDuration, false);

	return BEPass;
}

BEResult BE_ShowString(BEView view, int index, const char* str, int left, int top,
		unsigned fontSize, const char* fontColor, unsigned verticalPadding,
		unsigned horizontalPadding, const char* backgroundColor, unsigned borderRadius)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	const char* strPrefix = "__STRING";
	char name[1024];
	WebCore::ExceptionCode ec = 0;

	snprintf(name, 1024, "%s%d", strPrefix, index);
	WebCore::StyledElement* strElm = lookupChildElm(uiRoot, name);
	if(!strElm)
	{
		strElm = appendElm(doc, uiRoot, name);
		if(!strElm)
			return BEFail;
	}

	strElm->removeAllChildren();

	WTF::String style = "position:absolute;"
		"display:block;"
		"z-index:99998;"
		"padding:" + WTF::String::number(verticalPadding) + "px " + WTF::String::number(horizontalPadding) + "px;"
		"font-size:" + WTF::String::number(fontSize) + "px;"
		"left:" + WTF::String::number(left) + "px;"
		"top:" + WTF::String::number(top) + "px;"
		"color:" + WTF::String(fontColor) + ";"
		"background-color:" + WTF::String(backgroundColor) + ";";

	if(borderRadius > 0)
		style += "-webkit-border-radius:" + WTF::String::number(borderRadius) +"px;";

	strElm->setAttribute("style", style, ec);
	if(ec != 0)
		return BEFail;

	WTF::String data = str;
	WTF::PassRefPtr<WebCore::Text> textPtr = doc->createTextNode(data);
	strElm->appendChild(textPtr, ec);
	if(ec != 0)
		return BEFail;

	return BEPass;
}

BEResult BE_HideString(BEView view, int index)
{
	if(!view)
		return BEFail;

	WebKitWebView* webview = WEBKIT_WEB_VIEW(((BERealView*)view)->webview);
	WebCore::Document* doc = WebKit::core(webview)->mainFrame()->document();
	WebCore::StyledElement* uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;

	const char* strPrefix = "__STRING";
	char name[1024];

	snprintf(name, 1024, "%s%d", strPrefix, index);
 	WebCore::StyledElement* strElm = lookupChildElm(uiRoot, name);
	if(!strElm)
		return BEFail;

	WTF::String value = "none";
	strElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);

	return BEPass;
}

BEResult BE_SetCacheAllOn()
{
	WebCore::MemoryCache* cache = WebCore::memoryCache();
	if(!cache)
		return BEFail;

	cache->setDisabled(false);
	webkit_set_cache_model(WEBKIT_CACHE_MODEL_WEB_BROWSER);
	return BEPass;
}

BEResult BE_SetCacheAllOff()
{
	WebCore::MemoryCache* cache = WebCore::memoryCache();
	if(!cache)
		return BEFail;

	cache->setDisabled(true);
	webkit_set_cache_model(WEBKIT_CACHE_MODEL_DOCUMENT_VIEWER);
	return BEPass;
}

BEResult BE_ShowCacheStatus()
{
	WebCore::MemoryCache* cache = WebCore::memoryCache();
	if(!cache)
		return BEFail;

	SYS_LOG("CacheModel :%d\n", webkit_get_cache_model());

	WebCore::MemoryCache::Statistics s = cache->getStatistics();
	printf("%-11s %-11s %-11s %-11s %-11s %-11s %-11s\n", "", "Count", "Size", "LiveSize", "DecodedSize", "PurgeableSize", "PurgedSize");
	printf("%-11s %-11s %-11s %-11s %-11s %-11s %-11s\n", "-----------", "-----------", "-----------", "-----------", "-----------", "-----------", "-----------");
	printf("%-11s %11d %11d %11d %11d %11d %11d\n", "Images", s.images.count, s.images.size, s.images.liveSize, s.images.decodedSize, s.images.purgeableSize, s.images.purgedSize);
	printf("%-11s %11d %11d %11d %11d %11d %11d\n", "CSS", s.cssStyleSheets.count, s.cssStyleSheets.size, s.cssStyleSheets.liveSize, s.cssStyleSheets.decodedSize, s.cssStyleSheets.purgeableSize, s.cssStyleSheets.purgedSize);
#if ENABLE(XSLT)
	printf("%-11s %11d %11d %11d %11d %11d %11d\n", "XSL", s.xslStyleSheets.count, s.xslStyleSheets.size, s.xslStyleSheets.liveSize, s.xslStyleSheets.decodedSize, s.xslStyleSheets.purgeableSize, s.xslStyleSheets.purgedSize);
#endif
	printf("%-11s %11d %11d %11d %11d %11d %11d\n", "JavaScript", s.scripts.count, s.scripts.size, s.scripts.liveSize, s.scripts.decodedSize, s.scripts.purgeableSize, s.scripts.purgedSize);
	printf("%-11s %11d %11d %11d %11d %11d %11d\n", "Fonts", s.fonts.count, s.fonts.size, s.fonts.liveSize, s.fonts.decodedSize, s.fonts.purgeableSize, s.fonts.purgedSize);
	printf("%-11s %-11s %-11s %-11s %-11s %-11s %-11s\n\n", "-----------", "-----------", "-----------", "-----------", "-----------", "-----------", "-----------");

	return BEPass;
}

BEResult BE_SetCacheSize(int cacheTotalCapacity, unsigned int pageCacheCapacity)
{
	gdouble deadDecodedDataDeletionInterval = 60;

	bool disabled = WebCore::memoryCache()->disabled();
	WebKitCacheModel cacheMode = webkit_get_cache_model();
	if (disabled == false && cacheMode == WEBKIT_CACHE_MODEL_WEB_BROWSER) {
		unsigned int cacheMinDeadCapacity = cacheTotalCapacity >> 2;
		unsigned int cacheMaxDeadCapacity = cacheTotalCapacity >> 1;
		WebCore::MemoryCache* cache = WebCore::memoryCache();
		if (!cache)
			return BEFail;

		WebCore::PageCache* pageCache = WebCore::pageCache();
		if (!pageCache)
			return BEFail;

		cache->setCapacities(cacheMinDeadCapacity, cacheMaxDeadCapacity, cacheTotalCapacity);
		cache->setDeadDecodedDataDeletionInterval(deadDecodedDataDeletionInterval);
		pageCache->setCapacity(pageCacheCapacity);
	} else {
		printf("[%s][%d] nothing to do BE_SetCache CacheMode :%d, Cached disabled :%d \n", __func__, __LINE__, cacheMode, disabled);
		return BEFail;
	}

	return BEPass;
}

BEResult BE_ReleasePageCache(int capacity)
{
	WebCore::PageCache* pageCache = WebCore::pageCache();
	if (!pageCache) {
		printf("[shivamidow] %s (%s %d) Fail to get pagetCache!\n", __FUNCTION__, __FILE__, __LINE__);
		return BEFail;
	}

	pageCache->setCapacity(0);
	pageCache->releaseAutoreleasedPagesNow();

	if (capacity > 0)
		pageCache->setCapacity(capacity);

	printf("[shivamidow] %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);

	return BEPass;
}

BEResult BE_InvalidateFontCache(void)
{
	WebCore::FontCache* fontCache = WebCore::fontCache();
	if (!fontCache) {
		printf("[shivamidow] %s (%s %d) Fail to get pagetCache!\n", __FUNCTION__, __FILE__, __LINE__);
		return BEFail;
	}

	fontCache->invalidate();

	printf("[shivamidow] %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);

	return BEPass;
}

BEResult BE_PruneCache(void)
{
#if ENABLE(TVGG_PRUNE_CACHE_NOW)
	WebCore::MemoryCache* cache = WebCore::memoryCache();
	if (!cache) {
		printf("[shivamidow] %s (%s %d) Fail to prune cache!\n", __FUNCTION__, __FILE__, __LINE__);
		return BEFail;
	}

	cache->prune();	//cache->pruneNow();

	printf("[shivamidow] %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
#endif
    return BEPass;
}

void BE_DeleteAllCookies(void)
{
#if ENABLE(TVGG_COOKIE_DELETE_FEATURE)
    SoupCookieJar* CookieJar = WebCore::defaultCookieJar();
    WebCore::deleteAllCookies(CookieJar);
#endif
}

static bool replaceCookieJar(bool privateMode)
{
	static bool isPrivateMode = false;
	static SoupCookieJar* normalModeCookieJar;

	if (isPrivateMode == privateMode)
		return true;

	if (privateMode)
	{
		normalModeCookieJar = WebCore::defaultCookieJar();
		g_object_ref(normalModeCookieJar);
		WebCore::setDefaultCookieJar(soup_cookie_jar_new());
		g_object_unref(WebCore::defaultCookieJar());
	}
	else if (normalModeCookieJar)
	{
		WebCore::setDefaultCookieJar(normalModeCookieJar);
		normalModeCookieJar = 0;
	}

	SoupSession* session = WebCore::ResourceHandle::defaultSession();
	if (!session)
		return false;

	soup_session_remove_feature_by_type(session, SOUP_TYPE_COOKIE_JAR);
	soup_session_add_feature(session, SOUP_SESSION_FEATURE(WebCore::defaultCookieJar()));

	isPrivateMode = privateMode;

	return true;
}

static bool cacheDisable(bool disabled)
{
	WebCore::MemoryCache* cache = WebCore::memoryCache();
	if (!cache)
		return false;

	if (cache->disabled() != disabled)
		cache->setDisabled(disabled);

	return true;
}

bool BE_CookieCacheOff(bool privateMode)
{
	printf("[%s][%d] Cookie Save Stop Result(cookie,cache)\n", __func__, __LINE__);
	return (replaceCookieJar(privateMode) && cacheDisable(privateMode));
}

BEResult BE_LoadSoupCache(void)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return BEFail;

    webkit_soup_cache_load(webkit_soup_cache);
#endif
    return BEPass;
}

BEResult BE_DumpSoupCache(void)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return BEFail;

    webkit_soup_cache_dump(webkit_soup_cache);
#endif
    return BEPass;
}

BEResult BE_FlushSoupCache(void)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return BEFail;

    webkit_soup_cache_flush(webkit_soup_cache);

    printf("[shivamidow] %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
#endif
    return BEPass;
}

BEResult BE_ClearSoupCache(void)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return BEFail;

    webkit_soup_cache_clear(webkit_soup_cache);

    printf("[shivamidow] %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
#endif
    return BEPass;
}

BEResult BE_SetMaxSizeOfSoupCache(unsigned int size)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return BEFail;

    webkit_soup_cache_set_max_size(webkit_soup_cache, size);
#endif
    return BEPass;
}

unsigned int BE_GetMaxSizeOfSoupCache(void)
{
#if ENABLE(TVGG_CONTROL_SOUP_CACHE)
    if(!webkit_soup_cache)
        return 0;

    return webkit_soup_cache_get_max_size(webkit_soup_cache);
#else
    return 0;
#endif
}

void BE_SetCacheOutOfSpaceCallback(BEBasicCallback callback)
{
#if ENABLE(TVGG_CACHE_BOUNDARY)
    cacheOutOfSpaceCallBack = callback;
#endif
}

BEResult BE_ResetPreDocument(BEView view)
{
	if(!view)
		return BEFail;
	if(((BERealView*)view)->currentFocusedDoc)
	{
		((BERealView*)view)->currentFocusedDoc = NULL;
		//printf("[%s:%d]reset currentFocusedDoc \n",__func__,__LINE__);
	}
	if(((BERealView*)view)->preFocusedDoc)
	{
		((BERealView*)view)->preFocusedDoc = NULL;
		//printf("[%s:%d]reset preFocusedDoc \n",__func__,__LINE__);
	}
	return BEPass;
}

BEResult BE_ResetMoveFocus(BEView view)
{
#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
	// Remove focus
	if(WebKit::BackingStore::isAvailable())
	{
		Vector<WebCore::IntRect> rects;
		WebKit::BackingStore::getInstance()->setFocusRect(rects);
		return BEPass;
	}
#endif

	WebCore::Document* doc =  (WebCore::Document*)((BERealView*)view)->currentFocusedDoc;//current document
	if(!doc)
	{
		//printf("[%s:%d] ==============current focus document Error\n",__func__,__LINE__);
		return BEFail;
	}
	WebCore::StyledElement* uiRoot = NULL;
	WebCore::StyledElement* focusElm = NULL;
	WebCore::StyledElement* elm = NULL;
	WTF::String value = "none";
	uiRoot = getUIRoot(doc);
	if(!uiRoot)
		return BEFail;
	focusElm = getChildElm(doc, uiRoot, "__FOCUS_ELM");
	if(!focusElm)
		return BEFail;
	focusElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
	if(focusElm)
	{
		elm = lookupChildElm(focusElm, "__LEFT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__RIGHT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__TOP");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__BOTTOM");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__CURRENT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
	}
	return BEPass;
}

#if 0
static gboolean onSpAfterMove(void* data)
{
	if(!data)
		return false;

//#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
//	return true;
//#endif

	((BERealView*)data)->timerID = 0;

	WebCore::Document* doc =  (WebCore::Document*)((BERealView*)data)->currentFocusedDoc;//current document
	if(!doc)
	{
		printf("[%s:%d] ==============current focus document Error\n",__func__,__LINE__);
		return false;
	}
	WebCore::StyledElement* uiRoot = NULL;
	WebCore::StyledElement* focusElm = NULL;
	WebCore::StyledElement* elm = NULL;
	WebCore::String value = "none";
	if( ((BERealView*)data)->preFocusedDoc )
	{
		WebCore::Document* pPreDoc =  (WebCore::Document*)((BERealView*)data)->preFocusedDoc;
		if(pPreDoc != doc)
		{
			uiRoot = getUIRoot(pPreDoc);
			if(!uiRoot)
				return false;
			focusElm = getChildElm(pPreDoc, uiRoot, "__FOCUS_ELM");
			if(!focusElm)
				return false;
			focusElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);

		}
		((BERealView*)data)->preFocusedDoc = (void*) doc;// save preFocusedDoc
		//printf("[%s:%d] ==============clear preFocus document \n",__func__,__LINE__);
		//printf("[%s:%d] ==============register preFocus document\n",__func__,__LINE__);
	}
	else
	{
		((BERealView*)data)->preFocusedDoc = (void*) doc;// save preFocusedDoc
		//printf("[%s:%d] ==============first register preFocus document\n",__func__,__LINE__);
	}
	if(focusElm)
	{
		elm = lookupChildElm(focusElm, "__LEFT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__RIGHT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__TOP");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__BOTTOM");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
		elm = lookupChildElm(focusElm, "__CURRENT");
		if(elm && elm->getInlineStyleDecl()->getPropertyValue(WebCore::CSSPropertyDisplay).length() == 0)
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyVisibility, value, false);
	}

    return false;
}

static void spMoveFocus(BERealView* view, WebCore::Document* doc, WebCore::StyledElement* uiRoot,
		WebCore::StyledElement* focusElm, BERect* rect, unsigned arrowDir,
		unsigned borderWidth, const char* borderColor, unsigned borderRadius,
		unsigned arrowWidth, unsigned arrowMargin,
		 float transitionDuration)
{
 	WebCore::StyledElement* elm;
	WebCore::String value;

#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
	if(WebKit::BackingStore::isAvailable())
	{
		WebCore::IntRect focusRect;
		focusRect.setX(rect->left);
		focusRect.setY(rect->top);
		focusRect.setWidth(rect->width);
		focusRect.setHeight(rect->height);
		WebKit::BackingStore::getInstance()->setSpatialNaviFocusRect(focusRect);
		return;
	}
#endif

	if(rect->width == 0 && rect->height == 0)
	{
		value = "none";
		focusElm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		printf("[%s :%d] << Move Focus Rect size is '0' case!!\n", __func__, __LINE__);
		return;
	}
	WebCore::String style = "position:absolute;"
//		"-webkit-box-shadow:0 5px 5px #888888;"
		"z-index:99999;"
		"left:" + WebCore::String::number(rect->left - (int) borderWidth) + "px;"
		"top:" + WebCore::String::number(rect->top - (int) borderWidth) + "px;"
		"width:" + WebCore::String::number(rect->width) + "px;"
		"height:" + WebCore::String::number(rect->height) + "px;"
		"border:" + borderColor + " solid " + WebCore::String::number(borderWidth) + "px;";

	if(borderRadius > 0)
		style += "-webkit-border-radius:" + WebCore::String::number(borderRadius) +"px;";

	style += "-webkit-transition-property:top,left,width,height;"
			"-webkit-transition-duration:" + WebCore::String::number(transitionDuration) + "s;";


      WebCore::ExceptionCode ec = 0;
	focusElm->setAttribute("style", style, ec);
	if(ec != 0)
	{
		memset(rect, 0, sizeof(BERect));
		return;
	}
	elm = NULL;

	style = "position:absolute;width:0;height:0;"
		"border-style:solid;"
		"border-width:" + WebCore::String::number(arrowWidth) + "px;"
		"visibility:hidden;";

	if(arrowDir & BELeft)
	{
		elm = getChildElm(doc, focusElm, "__LEFT");
		if(elm)
		{
			value = style
				+ "border-color:transparent " + WebCore::String(borderColor) + " transparent transparent;"
				"left:" + WebCore::String::number(- (int) (arrowWidth * 2 + borderWidth + arrowMargin)) + "px;"
				"top:" + WebCore::String::number(rect->height / 2 - (int) arrowWidth) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__LEFT");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BERight)
	{
		elm = getChildElm(doc, focusElm, "__RIGHT");
		if(elm)
		{
			value = style
				+ "border-color:transparent transparent transparent " + WebCore::String(borderColor) + ";"
				"left:" + WebCore::String::number(rect->width + borderWidth + arrowMargin) + "px;"
				"top:" + WebCore::String::number(rect->height / 2 - (int) arrowWidth) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__RIGHT");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BETop)
	{
		elm = getChildElm(doc, focusElm, "__TOP");
		if(elm)
		{
			value = style
				+ "border-color:transparent transparent " + WebCore::String(borderColor) + " transparent;"
				"left:" + WebCore::String::number(rect->width / 2 - (int) arrowWidth) + "px;"
				"top:" + WebCore::String::number(- (int) (arrowWidth * 2 + borderWidth + arrowMargin)) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__TOP");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}

	if(arrowDir & BEBottom)
	{
		elm = getChildElm(doc, focusElm, "__BOTTOM");
		if(elm)
		{
			value = style
				+ "border-color:" + WebCore::String(borderColor) + " transparent transparent transparent;"
				"left:" + WebCore::String::number(rect->width / 2 - (int) arrowWidth) + "px;"
				"top:" + WebCore::String::number(rect->height + borderWidth + arrowMargin) + "px;";

			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__BOTTOM");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}


	if(arrowDir & BECurrent)
	{
		elm = getChildElm(doc, focusElm, "__CURRENT");
		if(elm)
		{
			/*
			value = style
				+ "border-color:" + WebCore::String(borderColor) + " transparent transparent transparent;"
				"left:" + WebCore::String::number(rect->left +  (int) borderWidth) + "px;"
				"top:" + WebCore::String::number(rect->top +  (int) borderWidth) + "px;";
			*/
			elm->setAttribute("style", value, ec);
		}
	}
	else
	{
		elm = lookupChildElm(focusElm, "__CURRENT");
		if(elm)
		{
			value = "none";
			elm->getInlineStyleDecl()->setProperty(WebCore::CSSPropertyDisplay, value, false);
		}
	}
	rect->left += (int) borderWidth;
	rect->top += (int) borderWidth;

	if(transitionDuration >= 0.f)
	{
		if(view->timerID > 0)
			g_source_remove(view->timerID);
		view->timerID = g_timeout_add((transitionDuration) * 10, onSpAfterMove, (gpointer) view);
	}
}
#endif

BEResult BE_SetSpElementFocus(BEView view, BENode node, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, float transitionDuration)
{
#if 0
	if(!view)
		return BEFail;

	WebCore::StyledElement* uiRoot = NULL;
	WebCore::StyledElement* focusElm = NULL;

	WebCore::Document* pFocusedDoc = (WebCore::Document*)BE_GetFocusedDoc();
	if(!pFocusedDoc)
		return BEFail;

#if !ENABLE(TVGG_CAIRO_FOCUS)
	uiRoot = getUIRoot(pFocusedDoc);
	if(!uiRoot)
		return BEFail;

 	focusElm = getChildElm(pFocusedDoc, uiRoot, "__FOCUS_ELM");
	if(!focusElm)
		return BEFail;
#endif

	BERect rect;
	if(node)
#if ENABLE(TVGG_BACKINGSTORE) && ENABLE(TVGG_CAIRO_FOCUS)
		rect = BE_GetAbsoluteNodeRect(node);
#else
		rect = BE_GetNodeRect(node);
#endif
	else
		memset(&rect, 0, sizeof(BERect));

	((BERealView*)view)->currentFocusedDoc = (void*)pFocusedDoc;
	//if(node)
		//printf("[%s,%d] Current Node (x,y):(%ld,%ld) width:%ld,height:%ld :(%s) \n",__func__,__LINE__,rect.left,rect.top, rect.width, rect.height,((WebCore::Node*)(node))->nodeName().utf8().data());

	spMoveFocus((BERealView*) view, pFocusedDoc, uiRoot, focusElm, &rect, arrowDir, borderWidth,
		borderColor, borderRadius, arrowWidth, arrowMargin, transitionDuration);
#endif
	return BEPass;
}

void BE_SetBaseTime(unsigned int nBaseTime)
{
	g_nBEBaseTime = nBaseTime;
}

BETime BE_GetCurrentTime()
{
    unsigned int nCurrentTime = getCurTime();
    BETime time;
    memset(&time, 0, sizeof(BETime));

    tm *t = localtime((time_t*)&nCurrentTime);
    if (t) {
        time.nYear = t->tm_year + 1900;
        time.nMonth = t->tm_mon + 1;
        time.nDay = t->tm_mday;
        time.nHour = t->tm_hour;
        time.nMinute = t->tm_min;
        time.nSecond = t->tm_sec;
        time.nWhatDay = t->tm_wday;
    }

    return time;
}

bool BE_IsTextFormType(BENode node)
{
	if(!node)
	{
		return false;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	if(realNode->isElementNode())
	{
		WebCore::Element* element = (WebCore::Element*) realNode;
		return element->isTextFormControl();
	}

	return false;
}

char* BE_GetInputType(BENode node)
{
	if(!node)
	{
		return NULL;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;

	if(realNode->isContentEditable() == true)
	{
		WTF::String strNodeType("editable");
		return copyCString(strNodeType.utf8());
	}


	if(realNode->isElementNode())
	{
		WebCore::Element* element = (WebCore::Element*) realNode;

		// @ Note : Do not check  type of element.
		return copyCString(element->formControlType().string().utf8());
		/*
		WebCore::InputElement* inputElement = toInputElement(element);
		if(inputElement)
		{
			return copyCString(element->formControlType().string().utf8());
		}
		*/
	}

	return NULL;
}

char* BE_GetInputValue(BENode node)
{
	if(!node)
	{
		return NULL;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	if(realNode->isElementNode())
	{
		WebCore::Element* element = (WebCore::Element*) realNode;

		WebCore::HTMLInputElement* inputElement = (WebCore::HTMLInputElement*) realNode->toInputElement();
		if(inputElement)
		{
			return copyCString(inputElement->value().utf8());
		}

		char *strInputType = BE_GetInputType(node);
		if(strInputType == NULL)
		{
			return NULL;
		}

		bool isTextarea = (strcmp(strInputType, "textarea") == 0);
		free(strInputType);

		if(isTextarea)
		{
			WebCore::HTMLTextAreaElement* textAreaElement = static_cast<WebCore::HTMLTextAreaElement*>(element);
			if(textAreaElement != NULL)
			{
				return copyCString(textAreaElement->value().utf8());
			}
		}
		else if(realNode->isContentEditable() == true) // editable element.
		{
			WebCore::SelectionController* selection = realNode->document()->frame()->selection();
			if(selection != NULL)
			{
				WebCore::Node* anchorNode = selection->selection().start().anchorNode();
				if(anchorNode != NULL && anchorNode->isTextNode())
				{
					return copyCString(anchorNode->nodeValue().utf8());
				}
			}
		}
	}

	return NULL;
}

void BE_SetInputValue(BENode node, const char* value)
{
	if(!node)
	{
		return;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	if(realNode->isElementNode())
	{
		WebCore::Element* element = (WebCore::Element*) realNode;

		WebCore::HTMLInputElement* inputElement = (WebCore::HTMLInputElement*) realNode->toInputElement();
		if(inputElement)
		{
			inputElement->setValue(WTF::String::fromUTF8(value), true);
			return;
		}

		char *strInputType = BE_GetInputType(node);
		if(strInputType == NULL)
		{
			return;
		}

		bool isTextarea = (strcmp(strInputType, "textarea") == 0);
		free(strInputType);

		if(isTextarea)
		{
			WebCore::HTMLTextAreaElement* textAreaElement = static_cast<WebCore::HTMLTextAreaElement*>(element);
			if(textAreaElement != NULL)
			{
				textAreaElement->setValue(WTF::String::fromUTF8(value));
			}
		}
		else if(realNode->isContentEditable() == true)// editable element.
		{
			WebCore::SelectionController* selection = realNode->document()->frame()->selection();
			if(selection != NULL)
			{
				WebCore::Node* anchorNode = selection->selection().start().anchorNode();
				if(anchorNode != NULL)
				{
					if(anchorNode->isTextNode() == true)
					{
						WebCore::ExceptionCode code = 0;
						anchorNode->setNodeValue(WTF::String::fromUTF8(value), code);
					}
					else
					{
						const WTF::String strData(value);
						WTF::PassRefPtr<WebCore::Text> text = anchorNode->document()->createTextNode(strData);
						WebCore::ExceptionCode code = 0;

						if(anchorNode->rootEditableElement() == anchorNode)
						{
							anchorNode->appendChild(text.get(), code);
						}
						else
						{
							anchorNode->parentNode()->insertBefore(text.get(), anchorNode, code);
							//pNode->parent()->replaceChild( text.get(), pNode, code);
						}
					}
				}
			}
		}
	}
}

void BE_SetInputSelection(BENode node, int startPos, int endPos)
{
	if(!node)
	{
		return;
	}

	WebCore::Node* realNode = (WebCore::Node*) node;
	if(realNode->isElementNode())
	{
		WebCore::Element* element = (WebCore::Element*) realNode;
		endPos = (endPos > 0)? endPos : 0;
		startPos = (startPos > 0)? startPos : 0;
		startPos = (startPos < endPos)? startPos : endPos;

		WebCore::HTMLInputElement* inputElm = (WebCore::HTMLInputElement*) realNode->toInputElement();
		if(inputElm)
		{
			inputElm->setSelectionRange(startPos, endPos);
			return;
		}

		char *strInputType = BE_GetInputType(node);
		if(strInputType == NULL)
		{
			return;
		}

		bool isTextarea = (strcmp(strInputType, "textarea") == 0);
		free(strInputType);

		if(isTextarea)
		{
			WebCore::HTMLTextAreaElement* textAreaElement = static_cast<WebCore::HTMLTextAreaElement*>(element);
			if(textAreaElement != NULL)
			{
				textAreaElement->setSelectionRange(startPos, endPos);
			}
		}
		else if(realNode->isContentEditable() == true)// editable element.
		{
			WebCore::SelectionController *selection = realNode->document()->frame()->selection();
			if(selection != NULL)
			{
				WebCore::Node* anchorNode = selection->selection().start().anchorNode();
				if(anchorNode != NULL && anchorNode->isTextNode() == true)
				{
					WebCore::Position startPosition = WebCore::Position(anchorNode, startPos);
					WebCore::Position endPosition = WebCore::Position(anchorNode, endPos);
					WebCore::VisibleSelection newVisibleSelection(startPosition, endPosition);
					selection->setSelection(newVisibleSelection);
				}
			}
		}
	}
}

bool BE_DoesSelectionExist(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return false;

	/* Solution 1 */
	//if ((frame->selection()->selectionType() == WebCore::VisibleSelection::RangeSelection)
	//	&& frame->eventHandler()->mouseDownMayStartSelect())
	//	return true;
	//else
	//	return false;

	//return frame->eventHandler()->mouseDownMayStartSelect();


	/* Solution 2 */
	/* FIXME :
	  * I think this solution is the best. But it doesn't work like I think.
	  * I'm suspecting  that the reason is related with Spatial Navigation.
	  */
	//printf("[shivamidow] %s (%s %d) isNone=%d\n", __FUNCTION__, __FILE__, __LINE__, webkit_web_view_has_selection(webView));
	//return webkit_web_view_has_selection(webView);


	/* Solution 3 */
	RefPtr<WebCore::Range> selectedRange = frame->selection()->toNormalizedRange();
	if (!selectedRange)
		return false;

    Vector<WebCore::IntRect> intRects;
    selectedRange->textRects(intRects, true);

	unsigned int size = intRects.size();
	return (size > 0) ? true : false;
}

bool BE_IsSelectionTextOnly(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return false;

	RefPtr<WebCore::Range> selectedRange = frame->selection()->toNormalizedRange();

	Vector<WebCore::IntRect> intRects;
	selectedRange->textRects(intRects, true);

	unsigned size = intRects.size();
	if (size < 1)
		return false;

	int extraWidthToEndOfLine = 0;

	WebCore::IntRect selectionRect = WebCore::enclosingIntRect(frame->selection()->bounds());

	WebCore::Position start = frame->selection()->selection().start();
	WebCore::RenderObject* renderer = start.anchorNode()->renderer();
	if (!renderer)
		return false;

	WebCore::InlineBox* startInlineBox = NULL;
	int startCaretOffset;
	start.getInlineBoxAndOffset(WebCore::DOWNSTREAM, startInlineBox, startCaretOffset);
	WebCore::IntRect startCaretRect = renderer->localCaretRect(startInlineBox, startCaretOffset, &extraWidthToEndOfLine);
	if (startCaretRect != WebCore::IntRect())
		startCaretRect = renderer->localToAbsoluteQuad(WebCore::FloatRect(startCaretRect)).enclosingBoundingBox();

	if (startCaretRect.x() != intRects[0].x())
		return false;
	if (selectionRect.y() != intRects[0].y())
		return false;

	renderer = NULL;
	WebCore::Position end = frame->selection()->selection().end();
	renderer = end.anchorNode()->renderer();
	if (!renderer)
		return false;

	WebCore::InlineBox* endInlineBox = NULL;
	int endCaretOffset;
	end.getInlineBoxAndOffset(WebCore::UPSTREAM, endInlineBox, endCaretOffset);
	WebCore::IntRect endCaretRect = renderer->localCaretRect(endInlineBox, endCaretOffset);
	if (endCaretRect != WebCore::IntRect())
		endCaretRect = renderer->localToAbsoluteQuad(WebCore::FloatRect(endCaretRect)).enclosingBoundingBox();

	if (endCaretRect.x() != (intRects[size -1].x() + intRects[size -1].width()))
		return false;
	if (selectionRect.maxY() != (intRects[size -1].y() + intRects[size -1].height()))
		return false;

	return true;
}

char* BE_GetSelectionText(BEView view)
{
    BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

    return webkit_web_view_get_selected_text(webView);
}

void BE_SelectAllContent(BEView view)
{
    BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

    return webkit_web_view_select_all(webView);
}

int BE_SelectionPosAt(BEView view, BESelectionPos pos)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	int result = -1;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return result;


	RefPtr<WebCore::Range> selectedRange = frame->selection()->toNormalizedRange();

    Vector<WebCore::IntRect> intRects;
    selectedRange->textRects(intRects, true);

	unsigned size = intRects.size();
	if (size < 1)
		return result;

	switch (pos) {
	case BESelectionPosHeadLeft:
		result = intRects[0].x();
		break;
	case BESelectionPosHeadTop:
		result = intRects[0].y();
		break;
	case BESelectionPosTailRight:
		result = intRects[size -1].x() + intRects[size -1].width();
		break;
	case BESelectionPosTailBottom:
		result = intRects[size -1].y() + intRects[size -1].height();
		break;
	default:
		break;
	}

	return result;
}

int BE_SelectionBoundPosAt(BEView view, BEDirection pos)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	int result = -1;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return result;

	WebCore::IntRect selectionRect = WebCore::enclosingIntRect(frame->selection()->bounds());

	switch (pos) {
	case BELeft:
		result = selectionRect.x();
		break;
	case BETop:
		result = selectionRect.y();
		break;
	case BERight:
		result = selectionRect.maxX();
		break;
	case BEBottom:
		result = selectionRect.maxY();
		break;
	default:
		break;
	}

	return result;
}

/* NOTICE : NEVER REMOVE THIS FUNCTION
static BEResult
extentSelection(BEView view, int *top_x, int *top_y, int *top_h, int *bottom_x, int *bottom_y, int *bottom_h)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->selection() || frame->selection()->isNone())
        return BEFail;


    // Is this check necessary? Leaving it for safety.
    WebCore::RenderView* root = frame->contentRenderer();
    if (!root)
        return BEFail;

    RefPtr<WebCore::Range> selectedRange = frame->selection()->toNormalizedRange();
	if (!selectedRange)
		return BEFail;

    Vector<WebCore::IntRect> intRects;
    selectedRange->textRects(intRects, true);

    unsigned size = intRects.size();
    if (size > 0) {
        WebCore::IntRect r;

        r = intRects[0];
        *top_x = r.x();
        *top_y = r.y();
        *top_h = r.height();

        r = intRects[size - 1];
        *bottom_x = r.x() + r.width();
        *bottom_y = r.y() + r.height();
        *bottom_h = r.height();

		//if (sd->cairo_scaling) {
		//	float zoom = sd->frame->view()->scaleFactor();
		//	*top_x = float(*top_x) * zoom;
		//	*top_y = float(*top_y) * zoom;
		//	*top_h = float(*top_h) * zoom;
		//	*bottom_x = float(*bottom_x) * zoom;
		//	*bottom_y = float(*bottom_y) * zoom;
		//	*bottom_h = float(*bottom_h) * zoom;
	}

	return BEPass;
}
*/

BEResult BE_ExtendSelectionToLeft(BEView view, BEMouseEvent* aMouseEvent)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return BEFail;

	WebCore::SelectionController *sc = frame->selection();
	if (!sc || sc->isNone())
		return BEFail;

	WebCore::IntPoint p(aMouseEvent->x, aMouseEvent->y);
	WebCore::VisiblePosition* position = new WebCore::VisiblePosition(frame->visiblePositionForPoint(p));
	if (!position)
		return BEFail;

	WebCore::Position extent = sc->extent();

	if (WebCore::comparePositions(position->deepEquivalent(), extent) < 0) {
		// Change the 'base' and 'extent' positions to 'start' and 'end' positions.
        // We do it, because without this, the other modification of the selection
        // would destroy the 'start' and/or 'end' positions and set them to
        // the 'base'/'extent' positions accordingly
        WebCore::VisibleSelection sel(sc->start(), sc->end());
        sc->setSelection(sel);

        sc->setBase(*position, true);

        // This forces webkit to show selection
        // frame->invalidateSelection();

        // Unused
        //int top_x, top_y, top_h, bottom_x, bottom_y, bottom_h;

        // Find handlers positions
        //extentSelection(view, &top_x, &top_y, &top_h, &bottom_x, &bottom_y, &bottom_h);
    }

	delete position;

	return BEPass;
}

BEResult BE_ExtendSelectionToRight(BEView view, BEMouseEvent* aMouseEvent)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return BEFail;

	WebCore::SelectionController *sc = frame->selection();
	if (!sc || sc->isNone())
		return BEFail;

	WebCore::IntPoint p(aMouseEvent->x, aMouseEvent->y);
    WebCore::VisiblePosition *position = new WebCore::VisiblePosition(frame->visiblePositionForPoint(p));
	if (!position)
		return BEFail;

    WebCore::Position base = sc->base();

    // Check if the new position is after the base's position
    if (WebCore::comparePositions(position->deepEquivalent(), base) > 0) // >= ?
    {
        // Change the 'base' and 'extent' positions to 'start' and 'end' positions.
        // We do it, because without this, the other modifications of the selection
        // would destroy the 'start' and/or 'end' positions and set them to
        // the 'base'/'extent' positions accordingly
        WebCore::VisibleSelection sel(sc->start(), sc->end());
        sc->setSelection(sel);

        sc->setExtent(*position, true);

        // This forces webkit to show selection
        // frame->invalidateSelection();

        // Unused
        //int top_x, top_y, top_h, bottom_x, bottom_y, bottom_h;

        // Find handlers positions
        //extentSelection(view, &top_x, &top_y, &top_h, &bottom_x, &bottom_y, &bottom_h);
    }

	delete position;

	return BEPass;
}

BEResult BE_CopyToClipboard(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

	webkit_web_view_copy_clipboard(webView);
    //frame->editor()->command("Copy").execute();

	return BEPass;
}

BEResult BE_PasteFromClipboard(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->editor())
        return BEFail;

	webkit_web_view_paste_clipboard(webView);

	return BEPass;
}

bool BE_CanCopyClipboard(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

	return webkit_web_view_can_copy_clipboard(webView);
}

bool BE_CanPasteClipboard(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->editor())
        return false;


	//return frame->editor()->canPaste() || frame->editor()->canDHTMLPaste();
	return webkit_web_view_can_paste_clipboard(webView);
}

int BE_FocusedNodeRectPosAt(BEView view, BEDirection pos)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	int result = -1;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return result;


	WebCore::Document* focusedDocument = frame->page()->focusController()->focusedOrMainFrame()->document();
	if (!focusedDocument) {
		printf("[shivamidow] %s (%s %d) Error! There is no focused document.\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	WebCore::Node* focusedNode = focusedDocument->focusedNode();
	if (!focusedNode)
		return result;

	WebCore::RenderObject* renderer = focusedNode->renderer();
	if (!renderer || !renderer->view() || !renderer->view()->frameView())
		return result;

	WebCore::IntRect focusedNodeRect = renderer->view()->frameView()->contentsToWindow(focusedNode->getRect());
	switch (pos) {
	case BELeft:
		result = focusedNodeRect.x();
		break;
	case BETop:
		result = focusedNodeRect.y();
		break;
	case BERight:
		result = focusedNodeRect.maxX();
		break;
	case BEBottom:
		result = focusedNodeRect.maxY();
		break;
	default:
		break;
	}

	return result;
}

bool BE_IsFocusedNodeTextForm(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->page() || !frame->page()->focusController() || !frame->page()->focusController()->focusedOrMainFrame())
        return false;


	WebCore::Document* focusedDocument = frame->page()->focusController()->focusedOrMainFrame()->document();
	if (!focusedDocument) {
		printf("[shivamidow] %s (%s %d) Error! There is no focused document.\n", __FUNCTION__, __FILE__, __LINE__);
		return false;
	}

	WebCore::Node* focusedNode = focusedDocument->focusedNode();
	if (!focusedNode)
		return false;

	if (focusedNode->nodeType() != WebCore::Node::ELEMENT_NODE)
		return false;

	if (!((WebCore::Element*)focusedNode)->isTextFormControl())
		return false;

	return true;
}

static unsigned int inputEventState(BEMouseEvent* aMouseEvent)
{
    unsigned int state = 0;
	if (aMouseEvent->button == BELeftBtn)
		state |= GDK_BUTTON1_MASK;
	if (aMouseEvent->button == BEMiddleBtn)
		state |= GDK_BUTTON2_MASK;
	if (aMouseEvent->button == BERightBtn)
		state |= GDK_BUTTON3_MASK;

    return state;
}


bool BE_SendMouseMotionEventForce(BEView view, BEMouseEvent* aMouseEvent)
{
	BERealView* realView = (BERealView*) view;
    if (!realView)
        return false;

	WebKitWebView* webView = realView->webview;
    if (!webView)
        return false;

	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return false;

	XEvent xEvent;
	memset(&xEvent, 0, sizeof(XEvent));

	xEvent.xany.serial = 0; // we are unaware of the last request processed by X Server
    xEvent.xany.send_event = false;
    xEvent.xany.display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());

    GtkWidget* widget = realView->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
    xEvent.xany.window = widget ? GDK_WINDOW_XID(gtk_widget_get_window(widget)) : 0;

	GdkScreen* gscreen = gtk_widget_get_screen(widget);

	XMotionEvent& xmotion = xEvent.xmotion;
    xmotion.type = MotionNotify;
    xmotion.root = GDK_WINDOW_XID(gdk_screen_get_root_window(gscreen));
    xmotion.subwindow = 0;
    xmotion.time = currentTime() * 1000.0;
    xmotion.x = aMouseEvent->x;
    xmotion.y = aMouseEvent->y;
    xmotion.x_root = aMouseEvent->screenX;
    xmotion.y_root = aMouseEvent->screenY;
    xmotion.state = inputEventState(aMouseEvent);
    xmotion.is_hint = NotifyNormal;
    xmotion.same_screen = true;

	BE_HandleEvent(view, (void*)(&xEvent));

	return true;
}

bool BE_SendMouseButtonEventForce(BEView view, BEMouseEvent* aMouseEvent)
{
	BERealView* realView = (BERealView*) view;
    if (!realView)
        return false;

	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!realView->pluginview || !frame)
        return false;

	XEvent xEvent;
	memset(&xEvent, 0, sizeof(XEvent));

	xEvent.xany.serial = 0; // we are unaware of the last request processed by X Server
    xEvent.xany.send_event = false;
    xEvent.xany.display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());

	GtkWidget* widget = realView->pluginview->parentFrame()->view()->hostWindow()->platformPageClient();
    xEvent.xany.window = widget ? GDK_WINDOW_XID(gtk_widget_get_window(widget)) : 0;

	GdkScreen* gscreen = gtk_widget_get_screen(widget);

	XButtonEvent& xbutton = xEvent.xbutton;
	xbutton.type = aMouseEvent->isButtonDown ? ButtonPress : ButtonRelease;
	xbutton.root = GDK_WINDOW_XID(gdk_screen_get_root_window(gscreen));
	xbutton.subwindow = 0;
	xbutton.time = currentTime() * 1000.0;
	xbutton.x = aMouseEvent->x;
	xbutton.y = aMouseEvent->y;
	xbutton.x_root = aMouseEvent->screenX;
	xbutton.y_root = aMouseEvent->screenY;
	xbutton.state = inputEventState(aMouseEvent);

	switch (aMouseEvent->button) {
	case BEMiddleBtn:
	    xbutton.button = Button2;
	    break;
	case BERightBtn:
	    xbutton.button = Button3;
	    break;
	case BELeftBtn:
        xbutton.button = Button1;
        break;
    case BEWheelUp:
        xbutton.button = Button4;
        break;
    case BEWheelDown:
        xbutton.button = Button5;
        break;
    case BEWheelLeft:
        xbutton.button = 6;
        break;
    case BEWheelRight:
        xbutton.button = 7;
        break;
	default:
	    break;
	}
	xbutton.same_screen = true;

	BE_HandleEvent(view, (void*)(&xEvent), aMouseEvent->isWheelGranularityPage);

	return true;
}
//we will more modify for speed...
static bool ToLower(char *str, int len)
{
	 int i;
	 if(!str || len ==0)
	 	return false;

	 for (i=0; i<len; i++)
	 {
		  if(str[i]>='A' && str[i]<='Z')
		  {
		   	str[i] += 32;
		  }
	 }
	 return true;
}
// we will find the correct method whether flash is or not.
bool BE_IsFlashElement(BENode node)
{
	WebCore::Node* pNode = (WebCore::Node*) node;
	if (!pNode)
		return false;

	if(pNode->hasTagName(WebCore::HTMLNames::embedTag) || pNode->hasTagName(WebCore::HTMLNames::objectTag))
	{
		char* type = BE_GetAttribute(pNode,"type");
		if (!type)
			return false;

		char* classid =BE_GetAttribute(pNode,"classid");
		if (!classid) {
			free(type);
			return false;
		}

		int nClsidlen = strlen(classid);
		ToLower(classid, nClsidlen);
		if((strcmp(type,"application/x-shockwave-flash") == 0)
				|| (strcmp(classid,"clsid:d27cdb6e-ae6d-11cf-96b8-444553540000") == 0))
		{
			if(type)
			{
				free(type);
				type = NULL;
			}
			if(classid)
			{
				free(classid);
				classid = NULL;
			}
			return true;
		}

		if(type)
		{
			free(type);
			type = NULL;
		}

		if(classid)
		{
			free(classid);
			classid = NULL;
		}
	}

	return false;
}

static void notifyDownloadStatus(GObject* object, GParamSpec* pspec, gpointer data)
{
	WebKitDownload* download = WEBKIT_DOWNLOAD(object);
	BERealView* realView = (BERealView*) data;
	switch(webkit_download_get_status(download))
	{
	case WEBKIT_DOWNLOAD_STATUS_FINISHED:
		if(realView->endDownload)
		{
			realView->endDownload(realView->callbackData);
			realView->download = NULL;
		}
		break;
	case WEBKIT_DOWNLOAD_STATUS_ERROR:
		printf("download error triggered\n");
		realView->download = NULL;
		break;
	case WEBKIT_DOWNLOAD_STATUS_CANCELLED:
		printf("download Cancelled\n");
		realView->download = NULL;
		break;
	default:
		break;
	}
}

void BE_StartDownload(BEView view)
{
	BERealView* realView = (BERealView*) view;
	if(!realView || !realView->download)
	{
		return;
	}

	const char* filename = webkit_download_get_suggested_filename(realView->download);
	if(!filename)
	{
		return;
	}

	char* downloadPath = (char*) malloc(strlen(DOWNLOAD_PATH) + strlen(filename) + 1);
	if(!downloadPath)
	{
		return;
	}
	strcpy(downloadPath, DOWNLOAD_PATH);
	strcat(downloadPath, filename);

	webkit_download_set_destination_uri(realView->download, downloadPath);
	webkit_download_start(realView->download);
	g_signal_connect(realView->download, "notify::status", G_CALLBACK(notifyDownloadStatus), realView);
}

bool BE_SendGtkMouseMotionEvent(NPP npp, BEMouseEvent* aMouseEvent, bool isWithSmoothCursor, bool doWarpCursor)
{
	WebCore::PluginView* pluginView = (WebCore::PluginView*)npp->ndata;
	if (!pluginView)
        return false;

	GtkWidget* parentWidget = pluginView->parentFrame()->view()->hostWindow()->platformPageClient();
	if (!parentWidget)
		return false;

	GdkDisplay *display = gdk_display_get_default();
	if (!display)
		return false;

	GdkScreen *screen = gdk_display_get_default_screen(display);
	if (!screen)
		return false;


	GdkEvent* gdkEvent = gdk_event_new(GDK_MOTION_NOTIFY);
	if (!gdkEvent)
		return false;

	gdkEvent->motion.type = GDK_MOTION_NOTIFY;
	gdkEvent->motion.window = gtk_widget_get_window(GTK_WIDGET(parentWidget));
	gdkEvent->motion.time = currentTime() * 1000.0;
	gdkEvent->motion.x = aMouseEvent->x;
	gdkEvent->motion.y = aMouseEvent->y;
	gdkEvent->motion.x_root = (gfloat)aMouseEvent->screenX;
	gdkEvent->motion.y_root = (gfloat)aMouseEvent->screenY;
	gdkEvent->motion.axes = NULL;
	gdkEvent->motion.state = (GdkModifierType)inputEventState(aMouseEvent);	// which button?
	gdkEvent->motion.is_hint = NotifyNormal;
#ifdef GTK_API_VERSION_2
	gdkEvent->motion.device = gdk_display_get_default()->core_pointer;
#else
	gdkEvent->motion.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	g_object_ref(gdkEvent->motion.window);

	if (isWithSmoothCursor) {
		/* smooth mouse movement */
		gint curPosX = 0, curPosY = 0, unitX = 0, unitY = 0;
        gint subtractX = 0, subtractY = 0;
		gdk_window_at_pointer(&curPosX, &curPosY);

        subtractX = aMouseEvent->x - curPosX;
        unitX = (subtractX >= 0) ? 1 : -1;

        subtractY = aMouseEvent->y - curPosY;
        unitY = (subtractY >= 0) ? 1 : -1;

        gint newPosX = curPosX;
        gint newPosY = curPosY;
        if (subtractX * unitX >= subtractY * unitY) {
            while (newPosY != aMouseEvent->y) {
                newPosX += unitX;
                newPosY += unitY;
                gdk_display_warp_pointer(display, screen, newPosX, newPosY);
            }

            while (newPosX != aMouseEvent->x) {
                newPosX += unitX;
                gdk_display_warp_pointer(display, screen, newPosX, newPosY);
            }
        } else {
            while (newPosX != aMouseEvent->x) {
                newPosX += unitX;
                newPosY += unitY;
                gdk_display_warp_pointer(display, screen, newPosX, newPosY);
            }

            while (newPosY != aMouseEvent->y) {
                newPosY += unitY;
                gdk_display_warp_pointer(display, screen, newPosX, newPosY);
            }
	}
	BE_NotifyMousePosChanged(newPosX, newPosY);
	}

	/* set new cusor position */
	if (doWarpCursor) {
	    gdk_display_warp_pointer(display, screen, aMouseEvent->x, aMouseEvent->y);
	    BE_NotifyMousePosChanged(aMouseEvent->x, aMouseEvent->y);
	}


    XSync(GDK_DISPLAY_XDISPLAY(display), true);

	gdk_event_put(gdkEvent);

	gdk_event_free(gdkEvent);

	return true;
}

bool BE_SendGtkMouseButtonPressEvent(NPP npp, BEMouseEvent* aMouseEvent)
{
	WebCore::PluginView* pluginView = (WebCore::PluginView*)npp->ndata;
	if (!pluginView)
        return false;

	GtkWidget* parentWidget = pluginView->parentFrame()->view()->hostWindow()->platformPageClient();
	if (!parentWidget)
		return false;

	GdkDisplay *display = gdk_display_get_default();
	if (!display)
		return false;

	GdkEvent* gdkEvent = gdk_event_new(GDK_BUTTON_PRESS);
	if (!gdkEvent)
		return false;

	gdkEvent->button.type = GDK_BUTTON_PRESS;
	gdkEvent->button.window = gtk_widget_get_window(GTK_WIDGET(parentWidget));
	gdkEvent->button.time = currentTime() * 1000.0;
	gdkEvent->button.x = aMouseEvent->x;
	gdkEvent->button.y = aMouseEvent->y;
	gdkEvent->button.x_root = (gfloat)aMouseEvent->screenX;
	gdkEvent->button.y_root = (gfloat)aMouseEvent->screenY;
	gdkEvent->button.axes = NULL;
	gdkEvent->button.state = (GdkModifierType)inputEventState(aMouseEvent);
#ifdef GTK_API_VERSION_2
	gdkEvent->button.device = gdk_display_get_default()->core_pointer;
#else
	gdkEvent->button.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif
	switch (aMouseEvent->button) {
	case BEMiddleBtn:
	    gdkEvent->button.button = Button2;
	    break;
	case BERightBtn:
	    gdkEvent->button.button = Button3;
	    break;
	case BELeftBtn:
	default:
	    gdkEvent->button.button = Button1;
	    break;
	}

	g_object_ref(gdkEvent->button.window);

	//XSync(GDK_DISPLAY_XDISPLAY(display), true);

	gdk_event_put(gdkEvent);

	gdk_event_free(gdkEvent);

	return true;
}


bool BE_SendGtkMouseButtonReleaseEvent(NPP npp, BEMouseEvent* aMouseEvent)
{
	WebCore::PluginView* pluginView = (WebCore::PluginView*)npp->ndata;
	if (!pluginView)
        return false;

	GtkWidget* parentWidget = pluginView->parentFrame()->view()->hostWindow()->platformPageClient();
	if (!parentWidget)
		return false;

	GdkDisplay *display = gdk_display_get_default();
	if (!display)
		return false;

	GdkEvent* gdkEvent = gdk_event_new(GDK_BUTTON_RELEASE);
	if (!gdkEvent)
		return false;

	gdkEvent->button.type = GDK_BUTTON_RELEASE;
	gdkEvent->button.window = gtk_widget_get_window(GTK_WIDGET(parentWidget));
	gdkEvent->button.time = currentTime() * 1000.0;
	gdkEvent->button.x = aMouseEvent->x;
	gdkEvent->button.y = aMouseEvent->y;
	gdkEvent->button.x_root = (gfloat)aMouseEvent->screenX;
	gdkEvent->button.y_root = (gfloat)aMouseEvent->screenY;
	gdkEvent->button.axes = NULL;
	gdkEvent->button.state = (GdkModifierType)inputEventState(aMouseEvent);
#ifdef GTK_API_VERSION_2
	gdkEvent->button.device = gdk_display_get_default()->core_pointer;
#else
	gdkEvent->button.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	switch (aMouseEvent->button) {
	case BEMiddleBtn:
	    gdkEvent->button.button = Button2;
	    break;
	case BERightBtn:
	    gdkEvent->button.button = Button3;
	    break;
	case BELeftBtn:
	default:
	    gdkEvent->button.button = Button1;
	    break;
	}

	g_object_ref(gdkEvent->button.window);

	//XSync(GDK_DISPLAY_XDISPLAY(display), true);

	gdk_event_put(gdkEvent);

	gdk_event_free(gdkEvent);

	return true;
}

static unsigned int cursorTimerID = 0;
static unsigned int cursorDestroyTimerID = 0;

static gboolean hideCursor(void* data)
{
	NPP npp = (NPP)data;

	WebCore::PluginView* pluginView = (WebCore::PluginView*)npp->ndata;
	if (!pluginView || !pluginView->parentFrame())
        return false;

	WebCore::FrameView* frameview = pluginView->parentFrame()->view();
	if (!frameview)
		return false;

	frameview->setCursor(WebCore::noneCursor());

	return true;
}

static gboolean destroyCursorTimer(void* data)
{
	unsigned int* timerID = (unsigned int*)data;
	if (*timerID > 0) {
		g_source_remove(*timerID);
		*timerID = 0;
	}

	return false;
}

bool BE_HideCursor(NPP npp)
{
	if (cursorTimerID > 0) {
		g_source_remove(cursorTimerID);
		cursorTimerID = 0;
	}
	cursorTimerID = g_timeout_add(50, hideCursor, npp);
	//cursorTimerID = g_timeout_add_full(G_PRIORITY_HIGH, 50, hideCursor, npp, NULL);

    /* Set destoryer to remove a cursor hide timer */
    if (cursorDestroyTimerID > 0) {
        g_source_remove(cursorDestroyTimerID);
        cursorDestroyTimerID = 0;
    }
	cursorDestroyTimerID = g_timeout_add(3000, destroyCursorTimer, &cursorTimerID);

	s_bAllowMouseEvent = false;
	return true;
}

bool BE_ShowCursor(NPP npp, BEMouseEvent* aMouseEvent)
{
	WebCore::PluginView* pluginView = (WebCore::PluginView*)npp->ndata;
	if (!pluginView || !pluginView->parentFrame())
        return false;

	WebCore::FrameView* frameview = pluginView->parentFrame()->view();
	if (!frameview)
		return false;

	GtkWidget* parentWidget = pluginView->parentFrame()->view()->hostWindow()->platformPageClient();
	if (!parentWidget)
		return false;

	GdkDisplay *display = gdk_display_get_default();
	if (!display)
		return false;

	GdkScreen *screen = gdk_display_get_default_screen(display);
	if (!screen)
		return false;


	GdkEvent* gdkEvent = gdk_event_new(GDK_MOTION_NOTIFY);
	if (!gdkEvent)
		return false;

	gdkEvent->motion.type = GDK_MOTION_NOTIFY;
	gdkEvent->motion.window = gtk_widget_get_window(GTK_WIDGET(parentWidget));
	gdkEvent->motion.time = currentTime() * 1000.0;
	gdkEvent->motion.x = aMouseEvent->x;
	gdkEvent->motion.y = aMouseEvent->y;
	gdkEvent->motion.x_root = (gfloat)aMouseEvent->screenX;
	gdkEvent->motion.y_root = (gfloat)aMouseEvent->screenY;
	gdkEvent->motion.axes = NULL;
	gdkEvent->motion.state = (GdkModifierType)inputEventState(aMouseEvent);	// which button?
	gdkEvent->motion.is_hint = NotifyNormal;
#ifdef GTK_API_VERSION_2
	gdkEvent->motion.device = gdk_display_get_default()->core_pointer;
#else
	gdkEvent->motion.device = gdk_device_manager_get_client_pointer(gdk_display_get_device_manager(gdk_display_get_default()));
#endif

	g_object_ref(gdkEvent->motion.window);

	if (cursorTimerID > 0) {
		g_source_remove(cursorTimerID);
		cursorTimerID = 0;
	}
    if (cursorDestroyTimerID > 0) {
        g_source_remove(cursorDestroyTimerID);
        cursorDestroyTimerID = 0;
    }

	gdk_event_put(gdkEvent);

	gdk_event_free(gdkEvent);

	frameview->setCursor(WebCore::pointerCursor());

	s_bAllowMouseEvent = true;
	return true;
}


bool BE_IsMouseEventAllowed()
{
	return s_bAllowMouseEvent;
}

int BE_PopupMenu_GetListSize(void* menu)
{
	if(!menu)
	{
		return 0;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	return client->listSize();
}

bool BE_PopupMenu_ItemIsLabel(void* menu, int index)
{
	if(!menu)
	{
		return false;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	return client->itemIsLabel(index);
}

bool BE_PopupMenu_ItemIsSeperator(void* menu, int index)
{
	if(!menu)
	{
		return false;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	return client->itemIsSeparator(index);
}

bool BE_PopupMenu_ItemIsEnabled(void* menu, int index)
{
	if(!menu)
	{
		return false;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	return client->itemIsEnabled(index);
}

char* BE_PopupMenu_GetItemText(void* menu, int index)
{
	if(!menu)
	{
		return NULL;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	return copyCString(client->itemText(index).utf8());
}

void BE_PopupMenu_SelectItem(void* menu, int index)
{
	if(!menu)
	{
		return;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	client->valueChanged(index);
}

void BE_PopupMenu_DidHide(void* menu)
{
	if(!menu)
	{
		return;
	}

	WebCore::PopupMenuClient* client = (WebCore::PopupMenuClient*) menu;
	client->popupDidHide();
}

bool BE_IsVideoElement(BENode node)
{
	WebCore::Node* pNode = (WebCore::Node*) node;
	if(!pNode)
		return false;

	if (pNode->hasTagName(WebCore::HTMLNames::videoTag))
	{
		if(!pNode->renderer()||!pNode->renderer()->view()||!pNode->renderer()->view()->frameView())
		{
			return false;
		}
		//printf("[%s][%d]------video element\n",__func__,__LINE__);
		return true;
	}
	return false;
}

bool
BE_MarkSearchedTexts(BEView view,const char* string, bool bCaseSensitive, unsigned int unLimit)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;

	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame || !frame->view() || !frame->eventHandler())
        return false;

	unsigned int unMatchedCount=0;
	g_return_val_if_fail(string, 0);

	WTF::TextCaseSensitivity caseSensitivity = bCaseSensitive ? WTF::TextCaseSensitive : WTF::TextCaseInsensitive;
	unMatchedCount = WebKit::core(webView)->markAllMatchesForText(WTF::String::fromUTF8(string), caseSensitivity, false, unLimit);
	if(unMatchedCount > 0)
	{
		printf("[%s :%d] , Matched Count:%d\n", __func__, __LINE__,unMatchedCount);
	    do {
	        frame->editor()->setMarkedTextMatchesAreHighlighted(true);//shold hight light
	        frame = frame->tree()->traverseNextWithWrap(false);
	    } while (frame);
	}
	else
	{
		printf("[%s :%d] , Not Matched \n", __func__, __LINE__);
		return false;
	}

	return true;
}

void
BE_UnMarkSearchedTexts(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	if(!webView)
		return;
	 WebKit::core(webView)->unmarkAllTextMatches();
}

bool
BE_NavigateMarkSearchedTexts( BEView view,const char* string, bool bCaseSensitive, bool bForward)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	if(!webView)
		return false;
	g_return_val_if_fail(string, FALSE);

	WTF::TextCaseSensitivity caseSensitivity = bCaseSensitive ? WTF::TextCaseSensitive : WTF::TextCaseInsensitive;
	WebCore::FindDirection direction = bForward ? WebCore::FindDirectionForward : WebCore::FindDirectionBackward;

	bool bRet = WebKit::core(webView)->findString(WTF::String::fromUTF8(string), caseSensitivity, direction, true);
	return bRet;
}

char*
BE_GetSelectedText(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	if(!webView)
		return NULL;
	return webkit_web_view_get_selected_text(webView);
}

char* BE_FocusedNodeText(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return NULL;

	WebCore::Document* curFocusedDoc = (WebCore::Document*) ((BERealView*)view)->currentFocusedDoc;
	if (!curFocusedDoc)
		return NULL;

	WebCore::Node* focusedNode = curFocusedDoc->focusedNode();
	if (!focusedNode || (focusedNode->nodeType() != WebCore::Node::ELEMENT_NODE))
		return NULL;

	WebCore::Element* focusedElement = static_cast<WebCore::Element*>(focusedNode);

	return copyCString(focusedElement->innerText().utf8());
}


char* BE_FocusedNodeLink(BEView view)
{
	BERealView* realView = (BERealView*) view;
	WebKitWebView* webView = realView->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
        return NULL;

	WebCore::Document* curFocusedDoc = (WebCore::Document*) ((BERealView*)view)->currentFocusedDoc;
	if (!curFocusedDoc)
		return NULL;

	WebCore::Node* focusedNode = curFocusedDoc->focusedNode();
	if (!focusedNode || (focusedNode->nodeType() != WebCore::Node::ELEMENT_NODE))
		return NULL;

	WebCore::Element* focusedElement = static_cast<WebCore::Element*>(focusedNode);

	return copyCString(focusedElement->getURLAttribute(WebCore::HTMLNames::hrefAttr).string().utf8());
}

BEResult BE_SetVScrollBarRect(BEView view, int x, int y, int width, int height)
{
#if ENABLE(TVGG_NATIVE_SCROLLBAR) && ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebCore::IntRect rect(x, y, width, height);
		WebKit::BackingStore::getInstance()->setVScrollBarRect(rect);
		return BEPass;
	}
#endif
	return BEFail;
}

BEResult BE_SetHScrollBarRect(BEView view, int x, int y, int width, int height)
{
#if ENABLE(TVGG_NATIVE_SCROLLBAR) && ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebCore::IntRect rect(x, y, width, height);
		WebKit::BackingStore::getInstance()->setHScrollBarRect(rect);
		return BEPass;
	}
#endif
	return BEFail;
}

BEResult BE_SetScrollBarColor(BEView view, int red, int green, int blue, int alpha)
{
#if ENABLE(TVGG_NATIVE_SCROLLBAR) && ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebCore::Color col(red, green, blue, alpha);
		WebKit::BackingStore::getInstance()->setScrollBarColor(col);
		return BEPass;
	}
#endif
	return BEFail;
}

BEResult BE_SetScrollBarThumbColor(BEView view, int red, int green, int blue, int alpha)
{
#if ENABLE(TVGG_NATIVE_SCROLLBAR) && ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebCore::Color col(red, green, blue, alpha);
		WebKit::BackingStore::getInstance()->setScrollBarThumbColor(col);
		return BEPass;
	}
#endif
	return BEFail;
}

BEResult BE_SetScrollBarMode(BEView view, BEScrollBarMode mode)
{
#if ENABLE(TVGG_NATIVE_SCROLLBAR) && ENABLE(TVGG_BACKINGSTORE)
	if(WebKit::BackingStore::isAvailable())
	{
		WebKit::BackingStore::getInstance()->setScrollBarMode((SCROLLBAR_MODE)mode);
		return BEPass;
	}
#endif
	return BEFail;
}

BEResult BE_SetPipEnabled(BEView view, bool isPipEnabled)
{
#if ENABLE(TVGG_PIP) && ENABLE(TVGG_BACKINGSTORE)
    if (WebKit::BackingStore::isAvailable()) {
        WebKit::MMManager::getInstance()->setPipEnabled(isPipEnabled);
        return BEPass;
    }
#endif
    return BEFail;
}

BEResult BE_SetPipRect(BEView view, int x, int y, int w, int h)
{
#if ENABLE(TVGG_PIP) && ENABLE(TVGG_BACKINGSTORE)
    if (WebKit::BackingStore::isAvailable()) {
        WebCore::IntRect r(x, y, w, h);
        WebKit::MMManager::getInstance()->setPipRect(r);
        return BEPass;
    }
#endif
    return BEFail;
}

BEResult BE_GetPipRect(BEView view, int* px, int* py, int* pw, int* ph)
{
#if ENABLE(TVGG_PIP) && ENABLE(TVGG_BACKINGSTORE)
    if (WebKit::BackingStore::isAvailable()) {
        WebCore::IntRect r = WebKit::MMManager::getInstance()->getPipRect();
        *px = r.x();
        *py = r.y();
        *pw = r.width();
        *ph = r.height();
        return BEPass;
    }
#endif
    return BEFail;
}

BEResult BE_GetPipEnabled(BEView view, bool* pPipEnabled)
{
#if ENABLE(TVGG_PIP) && ENABLE(TVGG_BACKINGSTORE)
    if (WebKit::BackingStore::isAvailable()) {
        *pPipEnabled = WebKit::MMManager::getInstance()->isPipEnabled();
        return BEPass;
    }
#endif
    return BEFail;
}

BEResult BE_IsH5VPlaying(BEView view)
{
#if USE(UNIPLAYER)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    return (WebCore::MediaPlayerPrivate::isH5VPlaying() ? BEPass : BEFail);
#endif
    return BEFail;
}

void BE_PauseH5V()
{
#if USE(UNIPLAYER)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    WebCore::MediaPlayerPrivate::pauseH5V();
#endif
}
void BE_ResumeH5V()
{
#if USE(UNIPLAYER)
    //fprintf(stderr, "[BR] %s %d\n", __func__, __LINE__);
    WebCore::MediaPlayerPrivate::resumeH5V();
#endif
}

void BE_StopH5V()
{
#if USE(UNIPLAYER)
    WebCore::MediaPlayerPrivate::stopH5V();
#endif
}

void* BE_GetWindow(BEView view)
{
    if (!view)
        return 0;

    WebCore::Frame* frame = WebKit::core(((BERealView*)view)->webview)->mainFrame();

    if (!frame)
        return 0;

    NPObject* windowScriptObject = frame->script()->windowScriptNPObject();

    if (windowScriptObject)
        windowScriptObject->referenceCount++;

    return windowScriptObject;
}

BEResult BE_SetAudioWindow(BEView view, bool isMainWindow)
{
#if ENABLE(TVGG_PIP)
    return WebKit::MMManager::getInstance()->setAudioMainWindow(isMainWindow) ? BEPass : BEFail;
#else
    return BEPass;
#endif
}

void BE_GetAudioWindow(int* pAudioWindow)
{
#if ENABLE(TVGG_PIP)
    *pAudioWindow = WebKit::MMManager::getInstance()->audioWindow();
#endif
}

BELoadingAnimationToolKit* g_loadingAnimationToolKit = NULL;
void* BE_CreateLoadingAnimation(void)
{
	if (g_loadingAnimationToolKit)
		BE_FinalizeLoadingAnimation();

	g_loadingAnimationToolKit = (BELoadingAnimationToolKit*)malloc(sizeof(BELoadingAnimationToolKit));
	if (!g_loadingAnimationToolKit)
		return (BELoadingAnimationToolKit*)NULL;

	memset(g_loadingAnimationToolKit, 0, sizeof(BELoadingAnimationToolKit));

	return (void*)g_loadingAnimationToolKit;
}

void BE_DestroyLoadingAnimation(void)
{
	if (!g_loadingAnimationToolKit)
		return;

	g_loadingAnimationToolKit->shouldDestroy = true;
}

bool BE_ShouldDestroyLoadingAnimation(void)
{
	if (!g_loadingAnimationToolKit)
		return false;

	return g_loadingAnimationToolKit->shouldDestroy;
}

void BE_FinalizeLoadingAnimation(void)
{
	if (!g_loadingAnimationToolKit)
		return;

    if (!g_loadingAnimationToolKit->shouldDestroy)
        g_loadingAnimationToolKit->shouldDestroy = true;

	if (g_loadingAnimationToolKit->cairoSurface)
		cairo_surface_destroy(g_loadingAnimationToolKit->cairoSurface);

	if (g_loadingAnimationToolKit->mainWindow)
		gtk_widget_destroy(g_loadingAnimationToolKit->mainWindow);

	free(g_loadingAnimationToolKit);

	g_loadingAnimationToolKit = NULL;
}

bool BE_IsMouseOverElement(BENode node)
{
	WebCore::Node* pTmp = (WebCore::Node*) node;
	if(!pTmp)
		return false;
	int nLoopCount =0,nNodeType=0;
	do
	{
		char* type = BE_GetAttribute(pTmp,"onmouseover");
		if (!type)
			return false;
		if(strcmp(type,"\0")!=0)
		{
			if(type)
			{
				free(type);
				type = NULL;
			}
			return true;
		}
		if(type)
		{
			free(type);
			type = NULL;
		}
		nNodeType =BE_GetNodeType(pTmp);
		nLoopCount++;
		pTmp = pTmp->parentNode();
	}
	while(pTmp && nNodeType ==1 && nLoopCount<10);

	return false;
}

void BE_Wait(bool isWait)
{
	static GMainLoop* loop = NULL;

	if(isWait)
	{
		if(!loop)
		{
			loop = g_main_loop_new(NULL, FALSE);

			g_main_loop_run(loop);

			g_main_loop_unref(loop);
			loop = NULL;
		}
		else
			printf("[%s %d] loop was existed.", __func__, __LINE__);
	}
	else
	{
		if(g_main_loop_is_running(loop))
			g_main_loop_quit(loop);
	}
}

void BE_ShowIME(BENode node)
{
	if(!node)
	{
		return;
	}

	imeNode = node;
	imeStatus = BEIMEShow;
}

void BE_ShowIMEFromFlash()
{
#if ENABLE(TVGG_FLASH_IME) && ENABLE(TVGG_BACKINGSTORE)
	imeStatus = BEIMEFlashShow;

	WebKitWebView* webview = NULL;
	webview = const_cast<WebKitWebView*>(WebKit::BackingStore::getInstance()->webView());
	g_signal_emit_by_name(webview, "show-ime");
#endif
}

void BE_HideIME()
{
	imeNode = NULL;
	imeStatus = BEIMEHide;
}

bool BE_IsFocusedIME(BENode node)
{
	if(!node)
	{
		return false;
	}

	if(imeStatus != BEIMEShow && imeStatus != BEIMEFlashShow)
	{
		return false;
	}

	if(imeNode == node)
	{
		return true;
	}

	return false;
}

BEIMEStatus BE_GetIMEStatus()
{
	return imeStatus;
}

void BE_NotifyMousePosChanged(int nX, int nY)
{
	s_nX = nX;
	s_nY = nY;
}


void BE_GetCurrentMousePos(int &nX, int &nY)
{
	nX = s_nX;
	nY = s_nY;
}


void BE_SetPointerSpeed(int nPointerSpeed)
{
	s_nPointerSpeed = nPointerSpeed;
}


int BE_GetPointerSpeed()
{
	return s_nPointerSpeed;
}


void BE_SetContentViewFocusState(bool bFocused)
{
	s_bContentViewFocused = bFocused;
}


bool BE_IsContentViewFocused()
{
	return s_bContentViewFocused;
}


void BE_FlushView(BEView view)
{
	gdk_window_process_all_updates();
}

BEResult BE_GarbageCollectSoon()
{
    WebCore::gcController().garbageCollectSoon();

    return BEPass;
}

void BE_SetPauseView(BEView view, bool isPause)
{
	if(!view)
		return;

	WebKitWebView* webView = ((BERealView*) view)->webview;
	WebCore::Frame* frame = WebKit::core(webView)->mainFrame();
	if (!frame)
		return;

	for (; frame; frame = frame->tree()->traverseNext())
	{
		frame->script()->setPaused(isPause);
		if(isPause)
			frame->document()->suspendActiveDOMObjects(WebCore::ActiveDOMObject::DocumentWillBecomeInactive);
		else
			frame->document()->resumeActiveDOMObjects();

		if(frame->view())
		{
			const WTF::HashSet<WTF::RefPtr<WebCore::Widget> >* children = frame->view()->children();
			WTF::HashSet<WTF::RefPtr<WebCore::Widget> >::const_iterator end = children->end();
			WTF::HashSet<WTF::RefPtr<WebCore::Widget> >::const_iterator it = children->begin();
			for (; it != end; ++it)
			{
				WebCore::Widget* widget = (*it).get();
				if (widget->isPluginView())
					static_cast<WebCore::PluginView*>(widget)->setJavaScriptPaused(isPause);
			}
		}
	}
}

static bool b_debug_pl = true;
bool BE_IsPluginDebug()
{
	return b_debug_pl;
}

void BE_SetDebug(int module, int level)
{
	switch(module)
	{
		case BE_DEBUG_MODULE_JS:
			if(level == 1)
			{
				set_debug_javascript();
			}
			else
			{
				reset_debug_javascript();
			}
			break;
		case BE_DEBUG_MODULE_PL:
			if(level == 1)
			{
				b_debug_pl = true;
			}
			else
			{
				b_debug_pl = false;
			}
			break;
	}
}


