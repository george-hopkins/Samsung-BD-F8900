/*
 * Copyright (C) 2011 Igalia S.L.
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
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301 USA
 */

#include "config.h"
#include <wtf/PassOwnPtr.h>
#include "GraphicsContext3DInternal.h"

#if ENABLE(WEBGL)

#if ENABLE(TVGG_WEBGL)
#include "GraphicsLayerRenderer.h"
#include "UIViewWindow.h"
#include <gdk/gdkx.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#if USE(SDAL)
#include "SdCommon.h"
#include "SdGfx.h"
#include <XEGLImageWrapper.h>
#endif
#else
#include <GL/glx.h>
#endif

#include "GraphicsContext3D.h"
#if !ENABLE(TVGG_WEBGL)
#include "OpenGLShims.h"
#endif

#if ENABLE(TVGG_WEBGL)
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <dlfcn.h>

#if !ENABLE(TVGG_ACCELERATED_COMPOSITING)
#define GL_SCREEN_WIDTH	1920
#define GL_SCREEN_HEIGHT 1080
#endif

#else
// We do not want to call glXMakeContextCurrent using different Display pointers,
// because it might lead to crashes in some drivers (fglrx). We use a shared display
// pointer here.
static Display* gSharedDisplay = 0;
static Display* sharedDisplay()
{
    if (!gSharedDisplay)
        gSharedDisplay = XOpenDisplay(0);
    return gSharedDisplay;
}
#endif

namespace WebCore {

// Because of driver bugs, exiting the program when there are active pbuffers
// can crash the X server (this has been observed with the official Nvidia drivers).
// We need to ensure that we clean everything up on exit. There are several reasons
// that GraphicsContext3Ds will still be alive at exit, including user error (memory
// leaks) and the page cache. In any case, we don't want the X server to crash.
static bool cleaningUpAtExit = false;

#if ENABLE(TVGG_WEBGL)
static EGLContext sharedContext = EGL_NO_CONTEXT;
static int refCount = 0;
#endif

static Vector<GraphicsContext3D*>& activeGraphicsContexts()
{
    DEFINE_STATIC_LOCAL(Vector<GraphicsContext3D*>, contexts, ());
    return contexts;
}

void GraphicsContext3DInternal::addActiveGraphicsContext(GraphicsContext3D* context)
{
    static bool addedAtExitHandler = false;
    if (!addedAtExitHandler) {
        atexit(&GraphicsContext3DInternal::cleanupActiveContextsAtExit);
        addedAtExitHandler = true;
    }
    activeGraphicsContexts().append(context);
}

void GraphicsContext3DInternal::removeActiveGraphicsContext(GraphicsContext3D* context)
{
    if (cleaningUpAtExit)
        return;

    Vector<GraphicsContext3D*>& contexts = activeGraphicsContexts();
    size_t location = contexts.find(context);
    if (location != WTF::notFound)
        contexts.remove(location);
}

void GraphicsContext3DInternal::cleanupActiveContextsAtExit()
{
    cleaningUpAtExit = true;

    Vector<GraphicsContext3D*>& contexts = activeGraphicsContexts();
    for (size_t i = 0; i < contexts.size(); i++)
        contexts[i]->~GraphicsContext3D();

#if !ENABLE(TVGG_WEBGL)
    if (!gSharedDisplay)
        return;

    XCloseDisplay(gSharedDisplay);
    gSharedDisplay = 0;
#endif
}

#if ENABLE(TVGG_WEBGL)
PassOwnPtr<GraphicsContext3DInternal> GraphicsContext3DInternal::create(GraphicsContext3D::RenderStyle renderStyle, HostWindow* hostWindow)
{
    enum InitState { INIT_NOT_YET, INIT_SUCCESS, INIT_FAIL };

    static InitState initState = INIT_NOT_YET;
    if (initState == INIT_NOT_YET) {
#if USE(SDAL)		
        initializeSDAL();
#endif
		initState = initializeOneOff() ? INIT_SUCCESS : INIT_FAIL;
    }

    if (initState == INIT_FAIL)
        return nullptr;

    GraphicsContext3DInternal* internal = 0;

    switch (renderStyle) {
    case GraphicsContext3D::RenderOffscreen:
        internal = createFBOContext();
        break;
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    case GraphicsContext3D::RenderDoubleBufferOffscreen:
        internal = createPixmapContext();
        break;
#endif
    case GraphicsContext3D::RenderDirectlyToHostWindow:
        internal = createXWindowContext(hostWindow);
        break;
    }

    if (!internal)
        return nullptr;

    // The GraphicsContext3D constructor requires that this context is the current OpenGL context.
    internal->makeContextCurrent();
#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    internal->setHostWindow(hostWindow);
#endif

    return adoptPtr(internal);
}

#if USE(SDAL)
void GraphicsContext3DInternal::initializeSDAL()
{
    static bool bInit = false;
    if (!bInit) {
#if !ENABLE(TVGG_ACCELERATED_COMPOSITING)
        XEGLInit();
#endif
        bInit = true;
    }
}
#endif

bool GraphicsContext3DInternal::initializeOneOff()
{
    WEBGL_DEBUG("");
    EGLDisplay display = eglGetDisplay(gdk_x11_display_get_xdisplay(gdk_display_get_default()));

    if (!display) {
        WEBGL_DEBUG("Error: No EGL Display available");
        return false;
    }
#if !ENABLE(TVGG_ACCELERATED_COMPOSITING)
    if (!eglInitialize(display, 0, 0)) {
        WEBGL_DEBUG("Error: Failed to initialize");
        return false;
    }
#endif

    return true;
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createFBOContext()
{
    WEBGL_DEBUG("");
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;
    
    int iPBufferConfig;
    EGLint configCount = 0;
    EGLint cfgAttribs[] = {
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         0,
        EGL_DEPTH_SIZE,         24,
        EGL_SURFACE_TYPE,        EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    eglDisplay = eglGetDisplay(gdk_x11_display_get_xdisplay(gdk_display_get_default()));

    if (eglDisplay == EGL_NO_DISPLAY) {
        WEBGL_DEBUG("Error: No EGL Display available");
        return 0;
    }

    eglChooseConfig(eglDisplay, cfgAttribs, 0, 0, &configCount);
    EGLConfig* eglConfig = (EGLConfig*)calloc(configCount, sizeof(EGLConfig));

    if (!eglConfig)
        return 0;

    eglChooseConfig(eglDisplay, cfgAttribs, eglConfig, configCount, &configCount);
    for (iPBufferConfig = 0; iPBufferConfig < configCount; iPBufferConfig++) {
        EGLint iEGLValue = 0;
        eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_RED_SIZE, &iEGLValue);
        if (iEGLValue == 8) {
            eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_GREEN_SIZE, &iEGLValue);
            if (iEGLValue == 8) {
                eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_BLUE_SIZE, &iEGLValue);
                if (iEGLValue == 8)
                    break;
            }
        }
    }

    if (iPBufferConfig >= configCount) {
        WEBGL_DEBUG("Error: Failed to find matching EGL config");
        exit(1);
    }

    EGLint surfaceAttbs[] = { EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE };
    EGLint value = 0;

    eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig[iPBufferConfig], surfaceAttbs);
    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &value);

    if (!eglSurface) {
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create surface!");
        return 0;
    }

    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfig[iPBufferConfig], sharedContext, contextAttribs);
    refCount++;

	if(sharedContext == EGL_NO_CONTEXT)
        sharedContext = eglContext;

    if (!eglContext) {
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create context!");
        return 0;
    }
    
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    free(eglConfig);
    
    return new GraphicsContext3DInternal(eglDisplay, eglContext, eglSurface);
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createPixmapContext()
{
#if 0
    int SCREEN_WIDTH = 256;
    int SCREEN_HEIGHT = 256;

    WEBGL_DEBUG("");
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;
    
    int iPBufferConfig;
    EGLint configCount = 0;
    EGLint cfgAttribs[] = {
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_BUFFER_SIZE,        32,
        EGL_SURFACE_TYPE,       EGL_PIXMAP_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        WEBGL_DEBUG("Error: No EGL Display available");
        return 0;
    }

    eglChooseConfig(eglDisplay, cfgAttribs, 0, 0, &configCount);
    EGLConfig* eglConfig = (EGLConfig*)calloc(configCount, sizeof(EGLConfig));

    if (!eglConfig)
        return 0;

    eglChooseConfig(eglDisplay, cfgAttribs, eglConfig, configCount, &configCount);
    for (iPBufferConfig = 0; iPBufferConfig < configCount; iPBufferConfig++) {
        EGLint iEGLValue = 0;
        eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_RED_SIZE, &iEGLValue);
        if (iEGLValue == 8) {
            eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_GREEN_SIZE, &iEGLValue);
            if (iEGLValue == 8) {
                eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_BLUE_SIZE, &iEGLValue);
                if (iEGLValue == 8)
                    break;
            }
        }
    }
    printf("- hurnjoo.lee : %s %d %d\n", __func__, __LINE__, configCount);

    if (iPBufferConfig >= configCount) {
        WEBGL_DEBUG("Error: Failed to find matching EGL config");
        exit(1);
    }

    EGLImageUMP* frontBuffer = createEGLImage(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!frontBuffer) {
        printf("ASSERT: front buffer creation failed.\n");
         return 0;
    }

    unsigned int frontBufferID = getUMPSecureId(frontBuffer);

    EGLImageUMP* backBuffer = createEGLImage(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!backBuffer) {
        printf("ASSERT: back buffer creation failed.\n");
        return 0;
    }

    unsigned int backBufferID = getUMPSecureId(backBuffer);
    printf("- hurnjoo.lee : %s %d frontId : %d backId\n", __func__, __LINE__,
           frontBufferID, backBufferID);

    EGLint surfaceAttbs[] = { EGL_WIDTH, SCREEN_WIDTH, EGL_HEIGHT, SCREEN_HEIGHT, EGL_NONE };
    EGLint value = 0;

    EGLImageUMP* eglUmp = new EGLImageUMP;
    bool result = SdGfx_AllocBuffer(&eglUmp->ptr, &eglUmp->uhndl, SCREEN_WIDTH, SCREEN_HEIGHT, 4, SD_GFX_UMP_KERNEL);
    eglUmp->pixmap = (bitblt_pixmap*)malloc(1*sizeof(bitblt_pixmap));
    memset(eglUmp->pixmap, 0x0, sizeof(bitblt_pixmap));
    eglUmp->pixmap->data = /*(unsigned short*)ump;*/(unsigned short*)eglUmp->uhndl;
    eglUmp->pixmap->alpha_size = 8;
    eglUmp->pixmap->red_size = 8;
    eglUmp->pixmap->blue_size = 8;
    eglUmp->pixmap->green_size = 8;
    eglUmp->pixmap->width = GL_SCREEN_WIDTH;
    eglUmp->pixmap->height = GL_SCREEN_HEIGHT;
    eglUmp->pixmap->bytes_per_pixel = 4;
    eglUmp->pixmap->flags = BITBLT_PIXMAP_SUPPORTS_UMP;

    eglSurface = eglCreatePixmapSurface(eglDisplay, eglConfig[iPBufferConfig], (EGLNativePixmapType)eglUmp->pixmap, NULL);


    if (!eglSurface) {
        EGLint eglErrorCode = eglGetError();
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create surface! (err code:%x)", eglErrorCode);
        return 0;
    }
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    eglContext = eglCreateContext(eglDisplay, eglConfig[iPBufferConfig], sharedContext, contextAttribs);
    refCount++;

    if (sharedContext == EGL_NO_CONTEXT)
        sharedContext = eglContext;

    if (!eglContext) {
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create context!");
        return 0;
    }
    
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    free(eglConfig);
    
    GraphicsContext3DInternal* internal = new GraphicsContext3DInternal(eglDisplay, eglContext, eglSurface);
    internal->m_frontBufferID = frontBufferID;
    internal->m_backBufferID = backBufferID;
    internal->m_frontBuffer = frontBuffer;
    internal->m_backBuffer = backBuffer;

    return internal;
#endif
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createWindowContext()
{
    WEBGL_DEBUG("");

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;

    int iPBufferConfig;
    EGLint configCount = 0;
    EGLint cfgAttribs[] =
    {
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

    bitblt_window* sWindow;
    sWindow = (bitblt_window *)calloc(1, sizeof(bitblt_window));
    if (!sWindow) {
        WEBGL_DEBUG("Error: Out of memory");
        exit(1);
    }
    sWindow->width = GL_SCREEN_WIDTH;
    sWindow->height = GL_SCREEN_HEIGHT;

    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (eglDisplay == EGL_NO_DISPLAY) {
        WEBGL_DEBUG("Error: No EGL Display available");
        return 0;
    }

    eglChooseConfig(eglDisplay, cfgAttribs, 0, 0, &configCount);
    EGLConfig* eglConfig = (EGLConfig*)calloc(configCount, sizeof(EGLConfig));

    if (!eglConfig)
        return 0;

    eglChooseConfig(eglDisplay, cfgAttribs, eglConfig, configCount, &configCount);
    for (iPBufferConfig = 0; iPBufferConfig < configCount; iPBufferConfig++) {
        EGLint iEGLValue = 0;
        eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_RED_SIZE, &iEGLValue);
        if (iEGLValue == 8) {
            eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_GREEN_SIZE, &iEGLValue);
            if (iEGLValue == 8) {
                eglGetConfigAttrib(eglDisplay, eglConfig[iPBufferConfig], EGL_BLUE_SIZE, &iEGLValue);
                if (iEGLValue == 8)
                    break;
            }
        }
    }

    if (iPBufferConfig >= configCount) {
        WEBGL_DEBUG("Error: Failed to find matching EGL config");
        exit(1);
    }

    EGLint value = 0;

    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig[iPBufferConfig], (EGLNativeWindowType)(sWindow), 0);
    eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &value);
    if (!eglSurface) {
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create surface!");
        return 0;
    }

    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    eglContext = eglCreateContext(eglDisplay, eglConfig[iPBufferConfig], sharedContext, contextAttribs);

    refCount++;

    if (sharedContext == EGL_NO_CONTEXT)
        sharedContext = eglContext;

    if (!eglContext) {
        free(eglConfig);
        WEBGL_DEBUG("Error: Failed to create context!");
        return 0;
    }

    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    free(eglConfig);

    return new GraphicsContext3DInternal(eglDisplay, eglContext, eglSurface);
#else
	return 0;
#endif
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createXWindowContext(HostWindow* hostWindow)
{
    WEBGL_DEBUG("");

    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;

    UIViewWindow* viewWindow = reinterpret_cast<UIViewWindow*>(hostWindow);
    Display* xDisplay = gdk_x11_display_get_xdisplay(gdk_display_get_default());

    if (!xDisplay) {
       WEBGL_DEBUG("Error: cannot connect to X server");
       return 0;
    }
 
    eglDisplay = eglGetDisplay((EGLNativeDisplayType) xDisplay);

    if (eglDisplay == EGL_NO_DISPLAY) {
       WEBGL_DEBUG("Error: Got no EGL display");
       return 0;
    }

    EGLint attr[] = {      
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

    if (!eglChooseConfig(eglDisplay, attr, &ecfg, 1, &numConfig)) {
       WEBGL_DEBUG("Error: Failed to choose config");
       return 0;
    }
 
    if (numConfig != 1) {
       WEBGL_DEBUG("Error: idn't get exactly one config, but");
       return 0;
    }
 
    eglSurface = eglCreateWindowSurface(eglDisplay, ecfg, (EGLNativeWindowType)viewWindow->window(), 0);
    if (eglSurface == EGL_NO_SURFACE) {
       WEBGL_DEBUG("Error: Unable to create EGL surface");
       return 0;
    }
 
    EGLint ctxattr[] = {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
    };

    eglContext = eglCreateContext(eglDisplay, ecfg, sharedContext, ctxattr);
    refCount++;

    if (sharedContext == EGL_NO_CONTEXT)
         sharedContext = eglContext;

    if (eglContext == EGL_NO_CONTEXT) {
       WEBGL_DEBUG("Error: Unable to create EGL context");
       return 0;
    }
 
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
    return new GraphicsContext3DInternal(eglDisplay, eglContext, eglSurface);
}

GraphicsContext3DInternal::GraphicsContext3DInternal(EGLDisplay display, EGLContext context, EGLSurface pbuffer)
    : m_display(display)
    , m_context(context)
    , m_surface(pbuffer)
{
    WEBGL_DEBUG();
}

GraphicsContext3DInternal::GraphicsContext3DInternal(EGLContext context, EGLSurface pbuffer)
    : m_context(context)
    , m_surface(pbuffer)
{
    WEBGL_DEBUG();
}
#else
PassOwnPtr<GraphicsContext3DInternal> GraphicsContext3DInternal::create()
{
    if (!sharedDisplay())
        return nullptr;

    static bool initialized = false;
    static bool success = true;
    if (!initialized) {
        success = initializeOpenGLShims();
        initialized = true;
    }
    if (!success)
        return nullptr;

    GraphicsContext3DInternal* internal = createPbufferContext();
    if (!internal)
        internal = createPixmapContext();
    if (!internal)
        return nullptr;

    // The GraphicsContext3D constructor requires that this context is the current OpenGL context.
    internal->makeContextCurrent();
    return adoptPtr(internal);
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createPbufferContext()
{
    int fbConfigAttributes[] = {
        GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_ALPHA_SIZE, 1,
        GLX_DEPTH_SIZE, 1,
        GLX_STENCIL_SIZE, 1,
        GLX_SAMPLE_BUFFERS, 1,
        GLX_DOUBLEBUFFER, GL_FALSE,
        GLX_SAMPLES, 4,
        0
    };
    int returnedElements;
    GLXFBConfig* configs = glXChooseFBConfig(sharedDisplay(), 0, fbConfigAttributes, &returnedElements);
    if (!configs) {
        fbConfigAttributes[20] = 0; // Attempt without anti-aliasing.
        configs = glXChooseFBConfig(sharedDisplay(), 0, fbConfigAttributes, &returnedElements);
    }

    if (!returnedElements) {
        XFree(configs);
        return 0;
    }

    // We will be rendering to a texture, so our pbuffer does not need to be large.
    static const int pbufferAttributes[] = { GLX_PBUFFER_WIDTH, 1, GLX_PBUFFER_HEIGHT, 1, 0 };
    GLXPbuffer pbuffer = glXCreatePbuffer(sharedDisplay(), configs[0], pbufferAttributes);
    if (!pbuffer) {
        XFree(configs);
        return 0;
    }

    GLXContext context = glXCreateNewContext(sharedDisplay(), configs[0], GLX_RGBA_TYPE, 0, GL_TRUE);
    XFree(configs);
    if (!context)
        return 0;

    return new GraphicsContext3DInternal(context, pbuffer);
}

GraphicsContext3DInternal* GraphicsContext3DInternal::createPixmapContext()
{
    static int visualAttributes[] = {
        GLX_RGBA,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        GLX_ALPHA_SIZE, 1,
        GLX_DOUBLEBUFFER,
        0
    };

    XVisualInfo* visualInfo = glXChooseVisual(sharedDisplay(), DefaultScreen(sharedDisplay()), visualAttributes);
    if (!visualInfo)
        return 0;
 
    GLXContext context = glXCreateContext(sharedDisplay(), visualInfo, 0, GL_TRUE);
    if (!context) {
        XFree(visualInfo);
        return 0;
    }

    Pixmap pixmap = XCreatePixmap(sharedDisplay(), DefaultRootWindow(sharedDisplay()), 1, 1, visualInfo->depth);
    if (!pixmap) {
        XFree(visualInfo);
        return 0;
    }

    GLXPixmap glxPixmap = glXCreateGLXPixmap(sharedDisplay(), visualInfo, pixmap);
    if (!glxPixmap) {
        XFreePixmap(sharedDisplay(), pixmap);
        XFree(visualInfo);
        return 0;
    }

    return new GraphicsContext3DInternal(context, pixmap, glxPixmap);
}

GraphicsContext3DInternal::GraphicsContext3DInternal(GLXContext context, GLXPbuffer pbuffer)
    : m_context(context)
    , m_pbuffer(pbuffer)
    , m_pixmap(0)
    , m_glxPixmap(0)
{
}

GraphicsContext3DInternal::GraphicsContext3DInternal(GLXContext context, Pixmap pixmap, GLXPixmap glxPixmap)
    : m_context(context)
    , m_pbuffer(0)
    , m_pixmap(pixmap)
    , m_glxPixmap(glxPixmap)
{
}
#endif

GraphicsContext3DInternal::~GraphicsContext3DInternal()
{
#if ENABLE(TVGG_WEBGL)
    WEBGL_DEBUG();
    EGLBoolean eRetStatus;
    if (m_context) {
        if  (!--refCount)
            sharedContext = 0;
	
        eRetStatus = eglDestroyContext(m_display, m_context);
        if (eRetStatus != EGL_TRUE)
            WEBGL_DEBUG("Error: Failed to destroy context");
	
        m_context = 0;
    }

    if (m_surface) {
        eRetStatus = eglDestroySurface(m_display, m_surface);
        if (eRetStatus != EGL_TRUE)
            WEBGL_DEBUG("Error: Failed to destroy surface");

        m_surface = 0;
    }

    eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
//    eRetStatus = eglTerminate(m_display);
//        if (eRetStatus != EGL_TRUE) {
//            WEBGL_DEBUG("Error: Failed to destroy egl");
//        }

    m_display = EGL_NO_DISPLAY;
#else

    if (m_context) {
        // This may be necessary to prevent crashes with NVidia's closed source drivers. Originally
        // from Mozilla's 3D canvas implementation at: http://bitbucket.org/ilmari/canvas3d/
        ::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

        ::glXMakeContextCurrent(sharedDisplay(), 0, 0, 0);
        ::glXDestroyContext(sharedDisplay(), m_context);
        m_context = 0;
    }

    if (m_pbuffer) {
        ::glXDestroyPbuffer(sharedDisplay(), m_pbuffer);
        m_pbuffer = 0;
    }
    if (m_glxPixmap) {
        glXDestroyGLXPixmap(sharedDisplay(), m_glxPixmap);
        m_glxPixmap = 0;
    }
    if (m_pixmap) {
        XFreePixmap(sharedDisplay(), m_pixmap);
        m_pixmap = 0;
    }
#endif
}

#if ENABLE(TVGG_ACCELERATED_COMPOSITING)
void GraphicsContext3DInternal::swapBuffers()
{
    makeContextCurrent();

    if (!m_context || !m_surface)
        return;

    eglSwapBuffers(m_display, m_surface);
}
#endif

void GraphicsContext3DInternal::makeContextCurrent()
{
#if ENABLE(TVGG_WEBGL)
    if (eglGetCurrentSurface(EGL_READ) == m_surface && eglGetCurrentContext() == m_context)
#else
    if (::glXGetCurrentContext() == m_context)
#endif
        return;
    if (!m_context)
        return;
#if ENABLE(TVGG_WEBGL)
    if (m_surface) {
        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
        return;
    }
#else
    if (m_pbuffer) {
        ::glXMakeCurrent(sharedDisplay(), m_pbuffer, m_context);
        return;
    }

    ASSERT(m_glxPixmap);
    ::glXMakeCurrent(sharedDisplay(), m_glxPixmap, m_context);
#endif
}

} // namespace WebCore

#endif // ENABLE_WEBGL
