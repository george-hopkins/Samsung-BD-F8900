/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2010 Motorola Mobility, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ThreadLauncher.h"

#include <WebCore/NotImplemented.h>

#if ENABLE(TVGG_THREAD_MODEL)
#include "RunLoop.h"
#include "WebProcess.h"
#include "WebAuthDialog.h"
#include <WebCore/ResourceHandle.h>
#include <runtime/InitializeThreading.h>
#endif

namespace WebKit {

#if ENABLE(TVGG_THREAD_MODEL)
static void* webThreadBody(void* context)
{
    g_type_init();

    JSC::initializeThreading();
    WTF::initializeMainThread();

    SoupSession* session = WebCore::ResourceHandle::defaultSession();

    GRefPtr<SoupSessionFeature> sniffer = adoptGRef(static_cast<SoupSessionFeature*>(g_object_new(SOUP_TYPE_CONTENT_SNIFFER, NULL)));
    soup_session_add_feature(session, sniffer.get());

    soup_session_add_feature_by_type(session, SOUP_TYPE_CONTENT_DECODER);

    GRefPtr<SoupSessionFeature> authDialog = adoptGRef(static_cast<SoupSessionFeature*>(g_object_new(WEB_TYPE_AUTH_DIALOG, NULL)));
    soup_session_add_feature(session, authDialog.get());

    WebProcess::shared().initialize((CoreIPC::Connection::Identifier)context, RunLoop::main());
    RunLoop::run();

    return 0;
}
#endif

CoreIPC::Connection::Identifier ThreadLauncher::createWebThread()
{
#if ENABLE(TVGG_THREAD_MODEL)
    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
      return 0;
    }

    g_type_init();
    
    JSC::initializeThreading();
    WTF::initializeMainThread();

    SoupSession* session = WebCore::ResourceHandle::defaultSession();

    GRefPtr<SoupSessionFeature> sniffer = adoptGRef(static_cast<SoupSessionFeature*>(g_object_new(SOUP_TYPE_CONTENT_SNIFFER, NULL)));
    soup_session_add_feature(session, sniffer.get());

    soup_session_add_feature_by_type(session, SOUP_TYPE_CONTENT_DECODER);

    GRefPtr<SoupSessionFeature> authDialog = adoptGRef(static_cast<SoupSessionFeature*>(g_object_new(WEB_TYPE_AUTH_DIALOG, NULL)));
    soup_session_add_feature(session, authDialog.get());

    WebProcess::shared().initialize((CoreIPC::Connection::Identifier)sockets[0], RunLoop::main());

//    if (!createThread(webThreadBody, reinterpret_cast<void*>(sockets[0]), "WebKit2: WebThread")) {
//        return 0;
//    }
    
    return (int)sockets[1];
#else
    notImplemented();
    return -1;
#endif
}

} // namespace WebKit
