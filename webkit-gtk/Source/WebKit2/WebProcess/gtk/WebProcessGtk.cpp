/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Portions Copyright (c) 2011 Motorola Mobility, Inc.  All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY MOTOROLA INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MOTOROLA INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebProcess.h"

#include "WebProcessCreationParameters.h"
#include <WebCore/NotImplemented.h>

#if ENABLE(TVGG_BROWSER)
#include <gtk/gtk.h>
#include <WebCore/RenderThemeGtk.h>
#include <wtf/gobject/GOwnPtr.h>
#endif

namespace WebKit {

void WebProcess::platformSetCacheModel(CacheModel)
{
    notImplemented(); 
}

void WebProcess::platformClearResourceCaches(ResourceCachesToClear)
{
    notImplemented();
}

void WebProcess::platformInitializeWebProcess(const WebProcessCreationParameters&, CoreIPC::ArgumentDecoder*)
{
#if ENABLE(TVGG_BROWSER)
    GOwnPtr<gchar> base(g_path_get_dirname(g_getenv("UI_URL")));
    GOwnPtr<gchar> uri;
    if (gdk_screen_get_height(gdk_screen_get_default()) < 1080)
        uri.set(g_build_filename(base.get(), "Themes/1280x720/gtk-3.0/scrollbar.css", NULL));
    else
        uri.set(g_build_filename(base.get(), "Themes/1920x1080/gtk-3.0/scrollbar.css", NULL));

    GFile* file;
    if (uri.get() && g_file_test(uri.get() + 7, G_FILE_TEST_EXISTS)) // skip 'file://'
        file = g_file_new_for_uri(uri.get());
    else
#if PLATFORM(TVGG_ENV)
        file = g_file_new_for_path("/mtd_down/widgets/normal/20111000001/[2012]BrowserUI/Themes/1280x720/gtk-3.0/scrollbar.css");
#else
        file = g_file_new_for_path("./gtk-3.0/scrollbar.css");
#endif

    GtkCssProvider* cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_file(cssProvider, file, 0);

    GtkStyleContext* context = static_cast<WebCore::RenderThemeGtk*>(WebCore::RenderTheme::defaultTheme().get())->gtkScrollbarStyle();
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(file);
    g_object_unref(cssProvider);
#endif

    notImplemented();
}

void WebProcess::platformTerminate()
{
}

} // namespace WebKit
