/*
 *  Copyright (C) 2008 Xan Lopez <xan@gnome.org>
 *  Copyright (C) 2009 Igalia S.L.
 *  Copyright (C) 2008 Apple Inc. All rights reserved.
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
#include "CookieJarSoup.h"

#include "Cookie.h"
#include "Document.h"
#include "GOwnPtrSoup.h"
#include "KURL.h"
#include <wtf/text/CString.h>

#if ENABLE(TVGG_PERSISTENT_COOKIES)
#include "Logging.h"

#  if ENABLE(TVGG_PERSISTENT_COOKIES)
#  ifndef COOKIES_FILE
#  define COOKIES_FILE "/mtd_down/webkit/etc/WebkitPlugin/.webkit-engine-cookies.db"
#  endif // COOKIES_FILE
#  endif // TVGG_PERSISTENT_COOKIES
#  if ENABLE(TVGG_PERSISTENT_COOKIES_SQL)
#include <libsoup/soup-cookie-jar-sqlite.h>
#  endif // TVGG_PERSISTENT_COOKIES_SQL
#endif // TVGG_PERSISTENT_COOKIES

#if ENABLE(WIDGET_ENGINE)
//3 WEBKIT_WIDGETENGINE
extern bool useWidgetEngine;
extern char* (*weGetWidgetCookie)(void* pWebCorePage, const char* pszUrl);
extern bool (*weSetWidgetCookie)(void* pWebCorePage, const char* pszUrl, const char* pszCookie);
extern void (*weMemoryFree)(void* ptr);
#endif

namespace WebCore {

static bool cookiesInitialized;
static SoupCookieJar* cookieJar;

#if ENABLE(TVGG_PERSISTENT_COOKIES)
SoupCookieJar* newPersistentCookieJar() {

    static const char* filename = COOKIES_FILE;

#if ENABLE(TVGG_PERSISTENT_COOKIES_TEXT)
    LOG(Network,"newCookieJar: file: %s, type: text", filename);
    return soup_cookie_jar_text_new(filename,false);
#elif ENABLE(TVGG_PERSISTENT_COOKIES_SQL)
    LOG(Network,"newCookieJar: file: %s, type: sqlite", filename);
    return soup_cookie_jar_sqlite_new(filename,false);
#endif // TVGG_PERSISTENT_COOKIES_TEXT

}
#endif // TVGG_PERSISTENT_COOKIES

SoupCookieJar* defaultCookieJar()
{
    if (!cookiesInitialized) {
        cookiesInitialized = true;

        cookieJar = soup_cookie_jar_new();
        soup_cookie_jar_set_accept_policy(cookieJar, SOUP_COOKIE_JAR_ACCEPT_NO_THIRD_PARTY);
#if ENABLE(TVGG_PERSISTENT_COOKIES)
        setDefaultCookieJar(newPersistentCookieJar());
#endif // TVGG_PERSISTENT_COOKIES
    }

    return cookieJar;
}

#if ENABLE(TVGG_COOKIE_DELETE_FEATURE)
void deleteAllCookies(SoupCookieJar* jar)
{
    GSList* cookiesList = soup_cookie_jar_all_cookies(jar);
    GSList* lE; // list enumerator

    if (cookiesList) {
        for (lE = cookiesList; lE; lE = lE->next)
            soup_cookie_jar_delete_cookie(jar, (SoupCookie *)lE->data);
        soup_cookies_free(cookiesList);
    }
    return;
}
#endif // TVGG_COOKIE_DELETE_FEATURE

void setDefaultCookieJar(SoupCookieJar* jar)
{
    cookiesInitialized = true;

    if (cookieJar)
        g_object_unref(cookieJar);

    cookieJar = jar;

    if (cookieJar)
        g_object_ref(cookieJar);
}

void setCookies(Document* document, const KURL& url, const String& value)
{
#if ENABLE(WIDGET_ENGINE)
//3 WEBKIT_WIDGETENGINE
    if (useWidgetEngine == true)
    {
        bool bResult = false;
        if (weSetWidgetCookie && document != NULL)
        {
    	    bResult = weSetWidgetCookie(document->page(), url.string().utf8().data(), value.utf8().data());
        }
        if (bResult == true)
        {
            return;
        }
    }
#endif

    SoupCookieJar* jar = defaultCookieJar();
    if (!jar)
        return;

    GOwnPtr<SoupURI> origin(soup_uri_new(url.string().utf8().data()));

    GOwnPtr<SoupURI> firstParty(soup_uri_new(document->firstPartyForCookies().string().utf8().data()));

    soup_cookie_jar_set_cookie_with_first_party(jar,
                                                origin.get(),
                                                firstParty.get(),
                                                value.utf8().data());
}

String cookies(const Document* document, const KURL& url)
{
#if ENABLE(WIDGET_ENGINE)
//3 WEBKIT_WIDGETENGINE
    if (useWidgetEngine == true)
    {
        char* pszCookie = NULL;
    	
        if (weGetWidgetCookie && document != NULL)
        {
            pszCookie = weGetWidgetCookie(document->page(), url.string().utf8().data());
        }
        if (pszCookie)
        {
            String result(String::fromUTF8(pszCookie));
            if (weMemoryFree)
            {
            	weMemoryFree(pszCookie);
            }
            return result;
        }
    }
#endif

    SoupCookieJar* jar = defaultCookieJar();
    if (!jar)
        return String();

    SoupURI* uri = soup_uri_new(url.string().utf8().data());
    char* cookies = soup_cookie_jar_get_cookies(jar, uri, FALSE);
    soup_uri_free(uri);

    String result(String::fromUTF8(cookies));
    g_free(cookies);

    return result;
}

String cookieRequestHeaderFieldValue(const Document* /*document*/, const KURL& url)
{
    SoupCookieJar* jar = defaultCookieJar();
    if (!jar)
        return String();

    SoupURI* uri = soup_uri_new(url.string().utf8().data());
    char* cookies = soup_cookie_jar_get_cookies(jar, uri, TRUE);
    soup_uri_free(uri);

    String result(String::fromUTF8(cookies));
    g_free(cookies);

    return result;
}

bool cookiesEnabled(const Document* /*document*/)
{
    return defaultCookieJar();
}

bool getRawCookies(const Document*, const KURL&, Vector<Cookie>& rawCookies)
{
    // FIXME: Not yet implemented
    rawCookies.clear();
    return false; // return true when implemented
}

void deleteCookie(const Document*, const KURL&, const String&)
{
    // FIXME: Not yet implemented
}

void getHostnamesWithCookies(HashSet<String>& hostnames)
{
    // FIXME: Not yet implemented
}

void deleteCookiesForHostname(const String& hostname)
{
    // FIXME: Not yet implemented
}

void deleteAllCookies()
{
    // FIXME: Not yet implemented
}

}
