/*
 * Copyright (C) 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Brent Fulgham <bfulgham@webkit.org>
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
#include "Download.h"
#include "CString.h"
#include "ResourceError.h"
#include "DataReference.h"

#include <WebCore/NotImplemented.h>
#include <stdio.h>

using namespace WebCore;

namespace WebKit {

#ifdef TVGG_DOCUMENT_VIEWER

static void *getUrlStart(void* clientInfo);

static Download* downloadFromClientInfo(const void* clientInfo)
{
    return reinterpret_cast<Download*>(const_cast<void*>(clientInfo));
}

static void
download_finished (SoupMessage *msg, void* clientInfo)
{
    Download* p_download = downloadFromClientInfo(clientInfo);

    if (msg == NULL || p_download == NULL) {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: msg=NULL or p_download=NULL return!!! \n",__func__,__LINE__);
	return;
    }

    if (msg->response_body->length <= 0) {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: [%s:%d] length=%d return!!! \n",__func__,__LINE__, msg->response_body->length);
	return;
    }

    if (p_download->getFileSession()->declaredLength <= 0) {
        WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: [%s:%d] declaredLength<=0 return!!! \n",__func__,__LINE__);
        return;
    }

    FILE* shareFile = NULL;
    shareFile = fopen ("/mtd_down/webkit/shared_doc","w");

    if (shareFile == NULL)
    {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: shareFile=NULL return!!! \n",__func__,__LINE__);
	return;
    }

    //g_main_loop_unref (loop);
    if (SOUP_STATUS_IS_SUCCESSFUL (msg->status_code)) {
                    WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, "ZDEBUG ############### write.. \n");
                    fwrite (msg->response_body->data, 1,
                        msg->response_body->length, shareFile);
    }
    fclose(shareFile);

    /* UI:Sync. */
    p_download->didFinish();
        
    return;
}

static void
got_chunk (SoupMessage *msg, void* clientInfo)
{
	Download* p_download = downloadFromClientInfo(clientInfo);
    if (msg == NULL || p_download == NULL) {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: msg=NULL or p_download=NULL return!!! \n",__func__,__LINE__);
	return;
    }

	if ((msg->response_body->length%1024)==0) {
        WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: [%s:%d] in.. (%d) \n",__func__,__LINE__, msg->response_body->length);

        p_download->didReceiveData((uint64_t)msg->response_body->length);
	}

	return;
}

static bool
sendDownloadFileName(const String& filename, Download* p_download)
{
    String extension = (filename.substring(filename.reverseFind('.') + 1)).lower();
    if (extension == "pdf"  ||
            extension == "doc"  ||
            extension == "docx" ||
            extension == "xls"  ||
            extension == "xlsx" ||
            extension == "ppt"  ||
            extension == "pptx" ||
            extension == "hwp") {
        
        bool overWrite = false;
        p_download->decideDestinationWithSuggestedFilename(filename, overWrite);
        printf ("==================== Send FileName =====================\n");
        return true;
    }

    return false;
}

static void
server_got_headers (SoupMessage *msg, const void* clientInfo)
{
    Download* p_download = downloadFromClientInfo(clientInfo);

    if (msg == NULL || p_download == NULL) {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: msg=NULL or p_download=NULL return!!! \n",__func__,__LINE__);
	return;
    }

    const char *header;
    const char *length;
    Vector <String> fileName; 

    length = soup_message_headers_get (msg->request_headers,
                                      "Content-Length");
    WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: [%s:%d] in.. (%s) \n",__func__,__LINE__, length );

    goffset declared_length = soup_message_headers_get_content_length (msg->response_headers);
    printf ("    Content-Length: %lu, body: %s\n",
                (gulong)declared_length, msg->response_body->data);

    p_download->getFileSession()->declaredLength = declared_length;

    SoupMessageHeadersIter iter;
    const char *hname, *value;
    char *path = soup_uri_to_string (soup_message_get_uri (msg), TRUE);	

    String pathString(path);


    if (SOUP_STATUS_IS_REDIRECTION (msg->status_code)) {
    	header = soup_message_headers_get_one (msg->response_headers,
    	                                       "Location");
        if (header) {
            SoupURI *uri;
            char *uri_string;

            printf ("REDIRECTION:  -> %s\n", header);

            uri = soup_uri_new_with_base (soup_message_get_uri (msg), header);
            uri_string = soup_uri_to_string (uri, FALSE);
            strncpy(p_download->getFileSession()->downUrl, uri_string, MAX_URL_SIZE);
		    p_download->getFileSession()->soupLoop = g_main_loop_new (NULL, TRUE);
            getUrlStart(p_download);
		    g_main_loop_unref (p_download->getFileSession()->soupLoop);

            g_free (uri_string);
            soup_uri_free (uri);
        }
    }



    pathString.split("/", fileName);
    unsigned size = fileName.size();

    String resultFileName = fileName[size-1];

    if (sendDownloadFileName(resultFileName, p_download)) /* 1-step */
        return;


    printf ("%s %s HTTP/1.%d  filename=%s\n", p_download->getFileSession()->method, path,
          soup_message_get_http_version (msg), resultFileName.ascii().data());
    soup_message_headers_iter_init (&iter, msg->request_headers);
    while (soup_message_headers_iter_next (&iter, &hname, &value))
          printf ("%s: %s\r\n", hname, value);
    printf ("\n");

    printf ("HTTP/1.%d %d %s\n",
          soup_message_get_http_version (msg),
          msg->status_code, msg->reason_phrase);

    soup_message_headers_iter_init (&iter, msg->response_headers);
    while (soup_message_headers_iter_next (&iter, &hname, &value)) {
        printf ("%s: %s\r\n", hname, value);
        printf ("\n");
        if (!strncmp(hname, "Content-Disposition", 19)) {
            printf ("######> %s: %s\r\n", hname, value);
            break;
        }
    }

    String parseValue(value);
    parseValue.split("=", fileName);
    resultFileName = fileName[fileName.size()-1];

    printf ("$$$> Parse filename=%s\n", resultFileName.ascii().data());

    if (sendDownloadFileName(resultFileName, p_download)) /* 2-step */
        return;


}



static void *getUrlStart(void* clientInfo)
{

    Download* p_download = downloadFromClientInfo(clientInfo);

    if (p_download == NULL) {
	WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,  "DEBUG: p_download=NULL return!!! \n",__func__,__LINE__);
	return NULL;
    }

    const char* url = p_download->getFileSession()->downUrl;

    WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,
            "ZDEBUG: test url=%s \n", url);	

    const char *name;
    SoupMessage *msg;
    const char *header;

    goffset start, end, total_length;



    msg = soup_message_new (p_download->getFileSession()->method, url);
    soup_message_set_flags (msg, SOUP_MESSAGE_NO_REDIRECT);

    g_signal_connect (msg, "got_headers",
                                        G_CALLBACK (server_got_headers), (void*)p_download);

    g_signal_connect (msg, "finished",
                                        G_CALLBACK (download_finished), (void*)p_download);

    g_signal_connect (msg, "got-chunk",
                                        G_CALLBACK (got_chunk), (void*)p_download);


    soup_session_send_message (p_download->getFileSession()->session, msg);

        

    name = soup_message_get_uri (msg)->path;

    WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION, "ZDEBUG ############### [%d].. \n",__LINE__);




    //g_main_loop_unref (p_download->getFileSession()->soupLoop);

	
	return NULL;
}
#endif

void Download::start(WebPage* initiatingWebPage)
{
#ifdef TVGG_DOCUMENT_VIEWER

    SoupURI *parsed;

    const char *cafile = NULL;

    g_type_init ();

    m_fileSession.method = SOUP_METHOD_GET;

    Download::didStart();
        
    const WebCore::KURL downloadUrl = m_request.url();


    const String urlString = downloadUrl.string();
    const UChar* baseCharacters = urlString.characters();

    CString cstring = urlString.ascii();
    const char* urlCharacters = cstring.data();

    WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,
        "ZDEBUG: [%s:%d] in.. urlValid=%d (%s) \n",__func__,__LINE__, downloadUrl.isValid(), urlCharacters);

    parsed = soup_uri_new (urlCharacters);

    if (!parsed) {
        WTFReportFatalError(__FILE__, __LINE__, WTF_PRETTY_FUNCTION,
            "Could not parse '%s' as a URL\n", urlCharacters);
        
        /* UI:Sync. */
        ResourceError resourceErr;
        CoreIPC::DataReference resumData;

        resourceErr = ResourceError("DownloadSoup", 0, urlString,"Could not parse URL");
        resumData = CoreIPC::DataReference();
        
        Download::didFail((const WebCore::ResourceError&)resourceErr, (const CoreIPC::DataReference&)resumData);
        return;
    }
    soup_uri_free (parsed);

    strncpy(m_fileSession.downUrl,(const char*)urlCharacters, MAX_URL_SIZE);

    m_fileSession.session = soup_session_async_new_with_options (
                                            SOUP_SESSION_SSL_CA_FILE, cafile,
                                            SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_CONTENT_DECODER,
                                            SOUP_SESSION_ADD_FEATURE_BY_TYPE, SOUP_TYPE_COOKIE_JAR,
                                            SOUP_SESSION_USER_AGENT, "get ",
                                            SOUP_SESSION_ACCEPT_LANGUAGE_AUTO, TRUE,
                                            NULL);

    m_fileSession.soupLoop = g_main_loop_new (NULL, TRUE);


    //m_fileSession.soupThread = g_thread_create (getUrlStart, (void*)this, TRUE, NULL);
    getUrlStart((void*)this);
    g_main_loop_unref (m_fileSession.soupLoop);
#else
    notImplemented();
#endif

}

void Download::startWithHandle(WebPage* initiatingPage, ResourceHandle*, const ResourceRequest& initialRequest, const ResourceResponse&)
{

#ifdef TVGG_DOCUMENT_VIEWER

	Download::start(initiatingPage);
	
#else
	notImplemented();
#endif
}

void Download::cancel()
{
    notImplemented();
}

void Download::platformInvalidate()
{
    notImplemented();
}

void Download::didDecideDestination(const String& destination, bool allowOverwrite)
{
    notImplemented();
}

void Download::platformDidFinish()
{
    notImplemented();
}

void Download::receivedCredential(const AuthenticationChallenge& authenticationChallenge, const Credential& credential)
{
    notImplemented();
}

void Download::receivedRequestToContinueWithoutCredential(const AuthenticationChallenge& authenticationChallenge)
{
    notImplemented();
}

void Download::receivedCancellation(const AuthenticationChallenge& authenticationChallenge)
{
    notImplemented();
}

} // namespace WebKit
