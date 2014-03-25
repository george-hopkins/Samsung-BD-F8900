/*
 *  Copyright (C) 2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2006 Jon Shier (jshier@iastate.edu)
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008, 2009 Apple Inc. All rights reseved.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 *  USA
 */

#include "config.h"
#include "JSDOMWindowBase.h"

#include "Chrome.h"
#include "Console.h"
#include "DOMWindow.h"
#include "Frame.h"
#include "InspectorController.h"
#include "JSDOMWindowCustom.h"
#include "JSNode.h"
#include "Logging.h"
#include "Page.h"
#include "SecurityOrigin.h"
#include "Settings.h"
#include "WebCoreJSClientData.h"
#include <wtf/Threading.h>

using namespace JSC;

#if ENABLE(WIDGET_ENGINE)
//3 WEBKIT_WIDGETENGINE
bool		useWidgetEngine = false;
bool		(*weInitializeJSEX)(ExecState* pExecState, void* pWebCorePage, char* pszBaseURL) = NULL;
bool		(*weProcessWidgetActivateEvent)(void* pWebCorePage) = NULL;
bool		(*weProcessWidgetKeyEvent)(void* pWebCorePage, int iKeyEventType, int iKeyCode, bool bPreventDefaulted) = NULL;
int			(*weGetEncryptedFileType)(char* pszEncryptedFileName) = NULL;
char*		(*weGetDecrypedMemoryBuffer)(char* pszEncryptedFileName, int iEncryptedFileType, int* pDecryptedBufferSize, char** ppOriginalFileName) = NULL;
const char*	(*weGetMimeType)(char* pszFileName) = NULL;
void		(*weMemoryFree)(void* ptr) = NULL;
char*		(*weMakeAbsoluteUrl)(const char* pszUrl, const char* pszBaseUrl) = NULL;
bool		(*weSetSoupHeaders)(SoupMessageHeaders* soupHeaders, const char* pszUrl) = NULL;
void		(*weSetFocusObjectElement)(void* pElement, void* pPluginView, bool bSetMode) = NULL;
bool		(*weUpdateWidgetLanguage)(const char* pszLanguage, void* pWebCorePaage) = NULL;
bool		(*weUpdateWidgetUserAgent)(const char* pszUserAgent, void* pWebCorePaage) = NULL;
char*		(*weGetWidgetUserAgent)(void* pWebCorePage) = NULL;
char*		(*weGetWidgetLanguage)(void* pWebCorePage) = NULL;
int			(*weGetInputDevice)(void) = NULL;
int			(*weGetKeypadType)(void) = NULL;
char*		(*weGetWidgetCookiePath)(void* pWebCorePage, const char* pszBaseURL) = NULL;
bool		(*weProcessWidgetDestroy)(void* pWebCorePage) = NULL;
char*		(*weGetWidgetCookie)(void* pWebCorePage, const char* pszUrl) = NULL;
bool		(*weSetWidgetCookie)(void* pWebCorePage, const char* pszUrl, const char* pszCookie) = NULL;
void		(*weWidgetAlert)(void* pWebCorePage, const char* pszMessage, int iLogLevel) = NULL;
bool		(*weSetMarkCallstack)(void* pIdentifier, const char* pszFile, int iLine, const char* pszFunction, bool bSetMode) = 0;
bool		(*weProcessOnLoadEvent)(void* pWebCorePage, const char* pszEventType) = NULL;
void		(*weSetPluginInstance)(void* pPluginInstance, bool bSetMode) = NULL;
#endif

namespace WebCore {

const ClassInfo JSDOMWindowBase::s_info = { "Window", &JSDOMGlobalObject::s_info, 0, 0 };

JSDOMWindowBase::JSDOMWindowBase(JSGlobalData& globalData, Structure* structure, PassRefPtr<DOMWindow> window, JSDOMWindowShell* shell)
    : JSDOMGlobalObject(globalData, structure, shell->world(), shell)
    , m_impl(window)
    , m_shell(shell)
{
    ASSERT(inherits(&s_info));

    GlobalPropertyInfo staticGlobals[] = {
        GlobalPropertyInfo(Identifier(globalExec(), "document"), jsNull(), DontDelete | ReadOnly),
        GlobalPropertyInfo(Identifier(globalExec(), "window"), m_shell, DontDelete | ReadOnly)
    };
    
    addStaticGlobals(staticGlobals, WTF_ARRAY_LENGTH(staticGlobals));
}

void JSDOMWindowBase::updateDocument()
{
    ASSERT(m_impl->document());
    ExecState* exec = globalExec();
    symbolTablePutWithAttributes(exec->globalData(), Identifier(exec, "document"), toJS(exec, this, m_impl->document()), DontDelete | ReadOnly);

#if ENABLE(WIDGET_ENGINE)
//3 WEBKIT_WIDGETENGINE
    if (weInitializeJSEX)
        (weInitializeJSEX)(globalExec(), impl()->frame()->page(), const_cast<char*>(impl()->url().string().utf8().data()));
#endif
}

ScriptExecutionContext* JSDOMWindowBase::scriptExecutionContext() const
{
    return m_impl->document();
}

String JSDOMWindowBase::crossDomainAccessErrorMessage(const JSGlobalObject* other) const
{
    return m_shell->window()->impl()->crossDomainAccessErrorMessage(asJSDOMWindow(other)->impl());
}

void JSDOMWindowBase::printErrorMessage(const String& message) const
{
    printErrorMessageForFrame(impl()->frame(), message);
}

ExecState* JSDOMWindowBase::globalExec()
{
    // We need to make sure that any script execution happening in this
    // frame does not destroy it
    if (Frame *frame = impl()->frame())
        frame->keepAlive();
    return Base::globalExec();
}

bool JSDOMWindowBase::supportsProfiling() const
{
#if !ENABLE(JAVASCRIPT_DEBUGGER) || !ENABLE(INSPECTOR)
    return false;
#else
    Frame* frame = impl()->frame();
    if (!frame)
        return false;

    Page* page = frame->page();
    if (!page)
        return false;

    return page->inspectorController()->profilerEnabled();
#endif
}

bool JSDOMWindowBase::supportsRichSourceInfo() const
{
#if PLATFORM(ANDROID)
    return true;
#elif !ENABLE(JAVASCRIPT_DEBUGGER) || !ENABLE(INSPECTOR)
    return false;
#else
    Frame* frame = impl()->frame();
    if (!frame)
        return false;

    Page* page = frame->page();
    if (!page)
        return false;

    bool enabled = page->inspectorController()->enabled();
    ASSERT(enabled || !debugger());
    ASSERT(enabled || !supportsProfiling());
    return enabled;
#endif
}

bool JSDOMWindowBase::shouldInterruptScript() const
{
    ASSERT(impl()->frame());
    Page* page = impl()->frame()->page();

    // See <rdar://problem/5479443>. We don't think that page can ever be NULL
    // in this case, but if it is, we've gotten into a state where we may have
    // hung the UI, with no way to ask the client whether to cancel execution.
    // For now, our solution is just to cancel execution no matter what,
    // ensuring that we never hang. We might want to consider other solutions
    // if we discover problems with this one.
    ASSERT(page);
    if (!page)
        return true;

    return page->chrome()->shouldInterruptJavaScript();
}

void JSDOMWindowBase::willRemoveFromWindowShell()
{
    setCurrentEvent(0);
}

JSObject* JSDOMWindowBase::toThisObject(ExecState*) const
{
    return shell();
}

JSValue JSDOMWindowBase::toStrictThisObject(ExecState*) const
{
    return shell();
}

JSDOMWindowShell* JSDOMWindowBase::shell() const
{
    return m_shell;
}

JSGlobalData* JSDOMWindowBase::commonJSGlobalData()
{
    ASSERT(isMainThread());

    static JSGlobalData* globalData = 0;
    if (!globalData) {
        globalData = JSGlobalData::createLeaked(ThreadStackTypeLarge).releaseRef();
        globalData->timeoutChecker.setTimeoutInterval(10000); // 10 seconds
#ifndef NDEBUG
        globalData->exclusiveThread = currentThread();
#endif
        initNormalWorldClientData(globalData);
    }

    return globalData;
}

// JSDOMGlobalObject* is ignored, accessing a window in any context will
// use that DOMWindow's prototype chain.
JSValue toJS(ExecState* exec, JSDOMGlobalObject*, DOMWindow* domWindow)
{
    return toJS(exec, domWindow);
}

JSValue toJS(ExecState* exec, DOMWindow* domWindow)
{
    if (!domWindow)
        return jsNull();
    Frame* frame = domWindow->frame();
    if (!frame)
        return jsNull();
    return frame->script()->windowShell(currentWorld(exec));
}

JSDOMWindow* toJSDOMWindow(Frame* frame, DOMWrapperWorld* world)
{
    if (!frame)
        return 0;
    return frame->script()->windowShell(world)->window();
}

JSDOMWindow* toJSDOMWindow(JSValue value)
{
    if (!value.isObject())
        return 0;
    const ClassInfo* classInfo = asObject(value)->classInfo();
    if (classInfo == &JSDOMWindow::s_info)
        return static_cast<JSDOMWindow*>(asObject(value));
    if (classInfo == &JSDOMWindowShell::s_info)
        return static_cast<JSDOMWindowShell*>(asObject(value))->window();
    return 0;
}

} // namespace WebCore
