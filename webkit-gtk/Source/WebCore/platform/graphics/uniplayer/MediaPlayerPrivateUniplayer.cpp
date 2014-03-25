/*
 * Copyright (C) 2007, 2009 Apple Inc.  All rights reserved.
 * Copyright (C) 2007 Collabora Ltd.  All rights reserved.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2009 Gustavo Noronha Silva <gns@gnome.org>
 * Copyright (C) 2009, 2010 Igalia S.L
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * aint with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"

#if USE(UNIPLAYER)

#include "MediaPlayerPrivateUniplayer.h"

#include "CString.h"
#include "Document.h"
#include "Frame.h"
#include "FrameView.h"
#include "GOwnPtrGStreamer.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "KURL.h"
#include "MIMETypeRegistry.h"
#include "MediaPlayer.h"
#include "NotImplemented.h"
#include "ScrollView.h"
#include "SecurityOrigin.h"
#include "TimeRanges.h"
#include "Widget.h"
#include "HTMLMediaElement.h"

#include "HostWindow.h"
#include "ResourceHandle.h"
#include <gtk/gtk.h>
#include "CSefPluginWebkit.h"
#include <sys/syscall.h>

#if ENABLE(TVGG_BACKINGSTORE)
#include "BackingStoreGtk.h"  // for drawVideo
#endif

#include "MMManager.h"
#include "BELog.h"

//#undef  BELog
//#define BELog(...) ((void)0)

typedef struct _SefMsg SefMsg;
typedef struct _SefBus SefBus;
typedef gboolean        (*SefBusFunc)           (SefBus * bus, SefMsg * message, gpointer data);

struct _SefMsg
{
    int eventType;
    std::string param1;
    std::string param2;
};

struct _SefBus
{
    GQueue *queue;
    GMutex *queue_lock;
    SefBusFunc handler;
    gpointer   handler_data;
};

static SefBus sef_bus;

static void
sef_bus_pop_all_msg(SefBus *bus)
{
  SefMsg *message;

  g_mutex_lock (bus->queue_lock);

  //BELog("have %d message(s)\n", g_queue_get_length (bus->queue));
  while (message = (SefMsg*) g_queue_pop_head (bus->queue))
      delete message;

  g_mutex_unlock (bus->queue_lock);

}

static SefMsg *
sef_bus_pop (SefBus * bus)
{
  SefMsg *message;

  g_mutex_lock (bus->queue_lock);

  //BELog("have %d message(s)\n", g_queue_get_length (bus->queue));
  message = (SefMsg*) g_queue_pop_head (bus->queue);

  g_mutex_unlock (bus->queue_lock);

  return message;
}

static gboolean
sef_bus_dispatch (gpointer user_data)
{
    gboolean keep;

    SefBus* bus = (SefBus*) user_data;

    if (G_UNLIKELY (bus == NULL))
        return TRUE;

    if (g_queue_is_empty (bus->queue))
        return TRUE;

    SefMsg *message = sef_bus_pop (bus);

    /* The message queue might be empty if some other thread or callback set
     * the bus to flushing between check/prepare and dispatch */
    if (G_UNLIKELY (message == NULL))
        return TRUE;

    SefBusFunc handler = bus->handler;

    if (!handler)
        goto no_handler;

    keep  = handler(bus, message, bus->handler_data);

    delete message;

    return keep;

no_handler:
    {
        BELog("SefBus watch dispatched without callback\n"
                "You must call g_source_set_callback().\n");
        delete message;
        return FALSE;
    }
}

#define SEF_DISPATCH_INTERVAL 100
void
sef_bus_init (SefBus * bus)
{
    bus->queue = g_queue_new ();
    bus->queue_lock = g_mutex_new ();
    bus->handler = (SefBusFunc) 0;
    g_timeout_add(SEF_DISPATCH_INTERVAL, sef_bus_dispatch, bus);
}

void
sef_bus_set_callback(SefBus        *bus,
                     SefBusFunc     func,
                     gpointer       data)
{
    if (!bus)
        return;

    bus->handler      = func;
    bus->handler_data = data;
}

/* { @20100906-sanggyu: copy from SEF/Inc/LegacyEmp/Player/MediaPlayer.h */
typedef enum
{
    CONNECTION_FAILED      = 1,
    AUTHENTICATION_FAILED  = 2,
    STREAM_NOT_FOUND       = 3,
    NETWORK_DISCONNECTED   = 4,
    NETWORK_SLOW           = 5,

    RENDER_ERROR           = 6,

    RENDERING_START        = 7,
    RENDERING_COMPLETE     = 8,
    STREAM_INFO_READY      = 9,
    DECODING_COMPLETE      = 10,   //Picture only

    BUFFERING_START        = 11,
    BUFFERING_COMPLETE     = 12,
    BUFFERING_PROGRESS     = 13,

    CURRENT_DISPLAY_TIME   = 14,

    AD_START               = 15,
    AD_END                 = 16,

    RESOLUTION_CHANGED     = 17,
    BITRATE_CHANGED        = 18,

    SUBTITLE               = 19,
    CUSTOM                 = 100
} EVENT_TYPE;

typedef enum
{
    UNKNOWN_ERROR=0,
    UNSUPPORTED_CONTAINER,
    UNSUPPORTED_VIDEO_CODEC,
    UNSUPPORTED_AUDIO_CODEC,
    UNSUPPORTED_VIDEO_RESOLUTION,
    UNSUPPORTED_VIDEO_FRAMERATE,
    CURRUPTED_STREAM,
} RENDER_ERROR_SUBTYPE;


/* } End of MediaPlayer.h */

/* { @20100906-sanggyu: copy from SEF/Inc/LegacyEmp/Window */

typedef enum _PL_WINDOW_Source
{
    PL_WINDOW_SOURCE_TV         = 0,
    PL_WINDOW_SOURCE_ATV        = 1,
    PL_WINDOW_SOURCE_DTV        = 2,
    PL_WINDOW_SOURCE_CATV       = 3,
    PL_WINDOW_SOURCE_CDTV       = 4,
    PL_WINDOW_SOURCE_PATV       = 5,
    PL_WINDOW_SOURCE_PDTV       = 6,
    PL_WINDOW_SOURCE_SDTV       = 7,
/*  PL_WINDOW_SOURCE_BSDTV      = 8,
    PL_WINDOW_SOURCE_CS1DTV     = 9,
    PL_WINDOW_SOURCE_CS2DTV     = 10, */
    //
    PL_WINDOW_SOURCE_ATV1       = 11,
    PL_WINDOW_SOURCE_ATV2       = 12,
    PL_WINDOW_SOURCE_DTV1       = 13,
    PL_WINDOW_SOURCE_DTV2       = 14,
    // AV 1~4
    PL_WINDOW_SOURCE_AV1        = 15,
    PL_WINDOW_SOURCE_AV2        = 16,
    PL_WINDOW_SOURCE_AV3        = 17,
    PL_WINDOW_SOURCE_AV4        = 18,
    //S-Video 1~4
    PL_WINDOW_SOURCE_SVIDEO1    = 19,
    PL_WINDOW_SOURCE_SVIDEO2    = 20,
    PL_WINDOW_SOURCE_SVIDEO3    = 21,
    PL_WINDOW_SOURCE_SVIDEO4    = 22,
    //Component 1~3
    PL_WINDOW_SOURCE_COMP1      = 23,
    PL_WINDOW_SOURCE_COMP2      = 24,
    PL_WINDOW_SOURCE_COMP3      = 25,
    PL_WINDOW_SOURCE_COMP4      = 26,
    // PC 1~3
    PL_WINDOW_SOURCE_PC1        = 27,
    PL_WINDOW_SOURCE_PC2        = 28,
    PL_WINDOW_SOURCE_PC3        = 29,
    PL_WINDOW_SOURCE_PC4        = 30,
    //HDMI 1~3
    PL_WINDOW_SOURCE_HDMI1      = 31,
    PL_WINDOW_SOURCE_HDMI2      = 32,
    PL_WINDOW_SOURCE_HDMI3      = 33,
    PL_WINDOW_SOURCE_HDMI4      = 34,

    // SCART
    PL_WINDOW_SOURCE_SCART1     = 35,
    PL_WINDOW_SOURCE_SCART2     = 36,
    PL_WINDOW_SOURCE_SCART3     = 37,
    PL_WINDOW_SOURCE_SCART4     = 38,

    // DVI
    PL_WINDOW_SOURCE_DVI1       = 39,
    PL_WINDOW_SOURCE_DVI2       = 40,
    PL_WINDOW_SOURCE_DVI3       = 41,
    PL_WINDOW_SOURCE_DVI4       = 42,

    //! DNet, WiseLink, PVR
    PL_WINDOW_SOURCE_MEDIA      = 43,

    // DCR HOMING
    PL_WINDOW_SOURCE_HOMING     = 44,

    PL_WINDOW_SOURCE_NONE       = 45,

    PL_WINDOW_SOURCE_MAX
} PL_WINDOW_SOURCE;

/* } End of Window */


/* { @20100825-sanggyu: CSefPluginUniPlayer */

#include <GOwnPtr.h>
#include <limits>
#include <math.h>

using namespace std;

/* current playing video tag element */

namespace WebCore {

bool CSefPluginUniPlayer::OnEvent(int eventType, string param1, string param2)
{
    SefMsg* message = NULL;

    //BELog("OnEvent %d (%s, %s) tid = %ld\n", eventType, param1.c_str(), param2.c_str(), syscall(SYS_gettid));

    message = new SefMsg;
    message->eventType = eventType;
    message->param1 = param1.c_str();
    message->param2 = param2.c_str();

    g_mutex_lock (sef_bus.queue_lock);
    g_queue_push_tail (sef_bus.queue, message);
    g_mutex_unlock (sef_bus.queue_lock);

    return true;
}

} // End of WebCore namespace

/* } End of CSefPluginUniPlayer */

/* { @20100825-sanggyu: MediaPlayerPrivate. Reference MediaPlayerGStreamer.cpp */

namespace WebCore {

static MediaPlayerPrivate* s_video = NULL;
static SoupSession* ss = NULL;
extern void ensureSessionIsInitialized(SoupSession* session);

gboolean mediaPlayerPrivateMessageCallback(SefBus* bus, SefMsg* message, gpointer data)
{
    MediaPlayerPrivate* mpp = (MediaPlayerPrivate*) data;

    if (s_video == data)
        mpp->updateStates(message->eventType, message->param1, message->param2);

    return true;
}

void MediaPlayerPrivate::durationChanged()
{
    BELog("%p\n", this);

    if (s_video != this)
    {
        BELog("No action is taken for inactive video.\n");
        return;
    }

    float newDuration = m_mediaDuration;

    if (m_uniplayer)
    {
        string str = m_uniplayer->Execute("GetDuration", "", "", "", "");
        const char* t = str.c_str();

        switch(t[0])
        {
            case EMP_RETURN_TYPE_PREFIX_INTEGER:
                newDuration = (float)atoi(&t[1]);
                break;
            case EMP_RETURN_TYPE_PREFIX_ASCII:
            case EMP_RETURN_TYPE_PREFIX_UNICODE:
            case EMP_RETURN_TYPE_PREFIX_ERROR:
            default:
                BELog(" return type mismatched! "
                        "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
                break;
        }
    }

    if (newDuration != m_mediaDuration)
    {
        BELog("Media duration is changed. [ %.2f ms => %.2f ms]\n", m_mediaDuration, newDuration);
        m_mediaDuration = newDuration;
        m_player->durationChanged();
    }
}

bool MediaPlayerPrivate::updateStates(int eventType, string param1, string param2)
{
    MediaPlayer::NetworkState oldNetworkState = m_networkState;
    MediaPlayer::ReadyState oldReadyState = m_readyState;

    switch (eventType)
    {
    case CONNECTION_FAILED:
        BELog("CONNECTION_FAILED\n");
        m_networkState = MediaPlayer::NetworkError;
        setError(E_CONNECTION_FAILED);
        this->alert("Network_Disconnected");
        if (m_uniplayer)
        {
            string r = m_uniplayer->Execute("Stop", "", "", "", "");
            BELog("ret = %s\n", r.c_str());
        }
        break;

    case STREAM_NOT_FOUND:
        BELog("STREAM_NOT_FOUND\n");
        m_networkState = MediaPlayer::NetworkError;
        break;

	case NETWORK_DISCONNECTED:
        BELog("NETWORK_DISCONNECTED\n");
        m_networkState = MediaPlayer::NetworkError;
        break;

    case STREAM_INFO_READY:
        BELog("STREAM_INFO_READY\n");
        m_networkState = MediaPlayer::Loading;
        m_readyState = MediaPlayer::HaveMetadata;

        /* get duration */
        this->durationChanged();
        break;

    case BUFFERING_START:
        BELog("BUFFERING_START\n");
        m_networkState = MediaPlayer::Loading;
        break;

    case BUFFERING_PROGRESS:
        BELog("BUFFERING_PROGRESS %d\n", atoi(param1.c_str()));
        m_networkState = MediaPlayer::Loading;
        break;

    case BUFFERING_COMPLETE:
        BELog("BUFFERING_COMPLETE\n");
        m_networkState = MediaPlayer::Loaded;
        m_readyState = MediaPlayer::HaveEnoughData;

        break;

    case CURRENT_DISPLAY_TIME:
        m_playbackPosition = (float)atoi(param1.c_str());
        BELog("CURRENT_DISPLAY_TIME %d ms\n", (int)m_playbackPosition);
        break;

	case AUTHENTICATION_FAILED:
        BELog("AUTHENTICATION_FAILED\n");
        break;

	case NETWORK_SLOW:
        BELog("NETWORK_SLOW\n");
        break;

    case RENDER_ERROR:
        switch (atoi(param1.c_str()))
        {
        case UNKNOWN_ERROR:
            BELog("RENDER_ERROR - UNKNOWN_ERROR\n");
            break;
        case UNSUPPORTED_CONTAINER:
            BELog("RENDER_ERROR - UNSUPPORTED_CONTAINER\n");
            break;
        case UNSUPPORTED_VIDEO_CODEC:
            BELog("RENDER_ERROR - UNSUPPORTED_VIDEO_CODEC\n");
            break;
        case UNSUPPORTED_AUDIO_CODEC:
            BELog("RENDER_ERROR - UNSUPPORTED_AUDIO_CODEC\n");
            break;
        case UNSUPPORTED_VIDEO_RESOLUTION:
            BELog("RENDER_ERROR - UNSUPPORTED_VIDEO_RESOLUTION\n");
            break;
        case UNSUPPORTED_VIDEO_FRAMERATE:
            BELog("RENDER_ERROR - UNSUPPORTED_FRAMERATE\n");
            break;
        case CURRUPTED_STREAM:
            BELog("RENDER_ERROR - CORRUPTED_STREAM\n");
            break;
        }
        m_networkState = MediaPlayer::DecodeError;
        setError(MediaPlayerPrivate::E_NOT_SUPPORTED_CODEC);
        this->alert("Playback_not_supported");

        if (m_uniplayer)
            this->cleanUp();

        break;

	case RENDERING_START:
        BELog("RENDERING_START\n");

        /* get video width, height */
        if (hasVideo())
        {
            string r = m_uniplayer->Execute("GetVideoResolution", "", "", "", "");
            BELog("Return of GetVideoWidth/Height : %s\n", r.c_str());
            int originalWidth  = atoi(r.c_str()+1);
            int originalHeight = atoi(strchr(r.c_str(), '|')+1);
            BELog("GetVideoWidth/Height : %dx%d\n", originalWidth, originalHeight);
            m_videoSize.setWidth(originalWidth);
            m_videoSize.setHeight(originalHeight);
            m_player->sizeChanged();
            this->repaint();
        }
        this->m_state = S_RENDERING;
        break;

    case RENDERING_COMPLETE:
        BELog("RENDERING_COMPLETE\n");
        this->didEnd();
        break;

    case AD_START:
        BELog("AD_START\n");
        break;

    case AD_END:
        BELog("AD_END\n");
        break;

    case RESOLUTION_CHANGED:
        BELog("RESOLUTION_CHANGED\n");
        break;

    case CUSTOM:
        BELog("CUSTOM\n");
        break;

    default:
        BELog("Not-defined Event %d\n", eventType);
        break;
    }

    if (m_networkState != oldNetworkState) {
        BELog("Network State Changed from %u to %u\n",
            oldNetworkState, m_networkState);
        m_player->networkStateChanged();
    }
    if (m_readyState != oldReadyState) {
        BELog("Ready State Changed from %u to %u\n",
            oldReadyState, m_readyState);
        m_player->readyStateChanged();
    }

    return true;
}

void MediaPlayerPrivate::init()
{
    m_networkState = MediaPlayer::Empty;
    m_readyState = MediaPlayer::HaveNothing;
    m_paused = true;
    m_seeking = false;
    m_mediaDuration = numeric_limits<float>::infinity();
    m_preload = MediaPlayer::Auto;
    m_delayingLoad = false;
    m_redirectDone = false;
    m_playbackPosition = 0.0;
    m_seekTime = 0.0;
    m_lastPaintRect = IntRect();
    m_error = E_NO_ERROR;
    m_maxVolume = 0;

    m_uniplayer = new CSefPluginUniPlayer();
    BELog("Open Player\n");
    if (!m_uniplayer->Open("Player", "1.000", "Player"))
    {
        BELog("Opening EMP Player Failed\n");
        delete m_uniplayer;
        m_uniplayer = NULL;
    }
}

MediaPlayerPrivate::MediaPlayerPrivate(MediaPlayer* player)
    : m_player(player)
    , m_mediaLocation(KURL())
    , m_state(S_INITIAL)
{
    init();
    m_uniplayer->setMediaPlayerPrivate(this);

    CSefPluginWebkit* sef_audio = MMManager::getInstance()->sefAudio();
    if (sef_audio)
    {
        string r = sef_audio->Execute("GetVolume", "", "", "", "");
        BELog("Volume at MediaPlayer Instantiation = %s\n", r.c_str());
        m_maxVolume  = atoi(r.c_str()+1);
    }
}

void MediaPlayerPrivate::cleanUp()
{
    BELog("%p\n", this);

    switch (m_state)
    {
        case S_INITIAL:
        case S_LOADED:
        case S_STOPPED:
        case S_WILL_STOP:
            // do nothing
            if (s_video == this)
            {
                BELog("Assertion Failed\n");
            }
            break;

        case S_PLAY_REQUESTED:
        case S_RENDERING:
        case S_PAUSED:
        {
            if (s_video != this)
            {
                BELog("Assertion Failed\n");
            }
            BELog("!!!!!!!!!!!!!!!!!!!!!!!!!HTML5 VIDEO STOP!!!!!!!!!!!!!!!!!\n");
            this->pauseMediaElement();
            m_state = S_WILL_STOP;
            string r = m_uniplayer->Execute("Stop", "", "", "", "");
            BELog("ret = %s\n", r.c_str());
            break;
        }
    }

    init();

    if (s_video == this)
        s_video = NULL;

    m_state = S_STOPPED;
}

MediaPlayerPrivate::~MediaPlayerPrivate()
{
    BELog("\n"); 
    this->cleanUp();
}

bool MediaPlayerPrivate::hasVideo() const
{
    HTMLMediaElement* me = static_cast<HTMLMediaElement*>(m_player->mediaPlayerClient());
    return me->isVideo();
}

bool MediaPlayerPrivate::hasAudio() const
{
    return true;
}

void MediaPlayerPrivate::pauseMediaElement()
{
    HTMLMediaElement* me = static_cast<HTMLMediaElement*>(m_player->mediaPlayerClient());
    if (me)
        me->pause();
}

void MediaPlayerPrivate::playMediaElement()
{
    HTMLMediaElement* me = static_cast<HTMLMediaElement*>(m_player->mediaPlayerClient());
    if (me)
        me->play();
}

// Returns the video size
IntSize MediaPlayerPrivate::naturalSize() const
{
    if (!hasVideo() ||  m_readyState < MediaPlayer::HaveMetadata)
        return IntSize();

    //BELog("Original video size: %dx%d\n", m_videoSize.width(), m_videoSize.height());

    return m_videoSize;
}

extern String cookies(const Document* /*document*/, const KURL& url);

static void
printHttpHeaders(const gchar * name, const gchar * val, gpointer src)
{
    BELog("  ! %s: %s\n", name, val);
}

static HashSet<String> mimeTypeCache();

static void
gotHeadersCallback(SoupMessage* msg, gpointer data)
{
    MediaPlayerPrivate* mpp = (MediaPlayerPrivate*) data;

    BELog("%p\n", data);
    soup_message_headers_foreach(msg->response_headers, printHttpHeaders, data);

    if (SOUP_STATUS_IS_REDIRECTION (msg->status_code)) {
        BELog ("%u redirect to \"%s\"\n", msg->status_code,
                soup_message_headers_get (msg->response_headers, "Location"));

        const char* strLoc = soup_message_headers_get (msg->response_headers, "Location");

        if (strLoc == NULL)
        {
            BELog("Redirection Error\n");
            mpp->setError(MediaPlayerPrivate::E_NO_REDIRECTION_LOCATION);
            return;
        }

        mpp->setMediaLocation(strLoc);

        return;
    } 
    else 
    {
        soup_session_cancel_message (ss, msg, SOUP_STATUS_CANCELLED);
        mpp->setRedirectDone();
        mpp->playPostAction();
    }
}

static void gotChunkCallback(SoupMessage* msg, SoupBuffer* chunk, gpointer data)
{
    //BELog("%p\n", data);
}

static void finishedCallback(SoupSession *session, SoupMessage* msg, gpointer data)
{
    //BELog("%p\n", data);
}

static void gotBodyCallback(SoupMessage* msg, gpointer data)
{
    //BELog("%p\n", data);
}

void MediaPlayerPrivate::playPostAction()
{
    BELog("\n");

    if (m_uniplayer)
    {
        string r = m_uniplayer->Execute("Stop", "", "", "", "");
        BELog("Return of Stop = %s\n", r.c_str());
    }

    String strURL = m_mediaLocation.string();
    BELog("UniPlayer URL: %s\n", strURL.utf8().data());

    string r;
    r = m_uniplayer->Execute("InitPlayer", strURL.utf8().data(), "", "", "");
    BELog("return of InitPlayer = %s\n", r.c_str());

    String strCookies = cookies(NULL, KURL(KURL(), m_mediaLocation));
    BELog("Cookie: %s\n", strCookies.utf8().data());

#if 1
    if (!strCookies.isEmpty())
    {
        char cLen[8]; snprintf(cLen, 8, "%d", (int)(strCookies.length()));
        BELog("SetPlayerProperty (PROPERTY_COOKIE, %s, %s)\n", strCookies.utf8().data(), cLen);
        r = m_uniplayer->Execute("SetPlayerProperty", "1", strCookies.utf8().data(), cLen, "");
        BELog("return of SetPlayerProperty = %s\n", r.c_str());
    }
#endif
    m_state = S_LOADED;
    play();
}

void MediaPlayerPrivate::playImpl()
{
    // Stop previous video
    if (s_video && s_video != this)
    {
        BELog("Stop previous video %p\n", s_video);
        s_video->cleanUp();
    }

    // Set playing video
    s_video = this;

    // Remove all message from previous video
    sef_bus_pop_all_msg(&sef_bus);

#if ENABLE(SEF)
    // Disable PIP before playing video.
    MMManager::getInstance()->setPipEnabled(false);
#endif // #if ENABLE(SEF)

    // Set callback
    sef_bus_set_callback(&sef_bus, mediaPlayerPrivateMessageCallback, this);

    // Play new video
    string r = m_uniplayer->Execute("StartPlayback", "", "", "", "");
    BELog("return of StartPlayback = %s\n", r.c_str());

    m_paused = false;
    m_state = S_PLAY_REQUESTED;
}

void MediaPlayerPrivate::play()
{
    BELog("this = %p, s_video = %p\n", this, s_video);

    switch (m_state)
    {
        case S_INITIAL:
            BELog("Load First, Can't play() on S_INITIAL\n");
            break;
        case S_WILL_STOP:
            BELog("Wait, it is under stopping.\n");
            break;
        case S_LOADED:
            BELog("\n");
            playImpl();
            break;

        case S_PLAY_REQUESTED:
        case S_RENDERING:
        case S_PAUSED:
        {
            BELog("\n");
            if (s_video != this)
            {
                BELog("Assertion Failed\n");
            }

            string r = m_uniplayer->Execute("Resume", "", "", "", "");
            BELog("Resume return = %s\n", r.c_str());
            m_paused = false;
            break;
        }
        case S_STOPPED:
        {
            BELog("Play on S_STOPPED\n");
            this->prepareToPlay();
            BELog("\n");
            playImpl();
            break;
        }
    }
}

void MediaPlayerPrivate::pause()
{
    BELog("this = %p, s_video = %p\n", this, s_video);

    switch (m_state)
    {
        case S_INITIAL:
        case S_LOADED:
        case S_PAUSED:
        case S_WILL_STOP:
        case S_STOPPED:
            BELog("No action is taken for inactive video.\n");
            break;

        case S_PLAY_REQUESTED:
        case S_RENDERING:
        {
            if (s_video != this)
            {
                BELog("Assertion Failed\n");
            }

            string r = m_uniplayer->Execute("Pause", "", "", "", "");

            if (strcmp(r.c_str(), "I1") == 0)
                m_paused = true;

            BELog("Pause return = %s m_paused = %d\n", r.c_str(), m_paused);
            break;
        }
    }
}

bool MediaPlayerPrivate::paused() const
{
    return m_paused;
}

void MediaPlayerPrivate::setVolume(float v) 
{
    BELog("New volume = %.2f, m_maxVolume = %d\n", v);

    if (s_video != this)
        return;

    CSefPluginWebkit* sef_audio = MMManager::getInstance()->sefAudio();
    if (sef_audio)
    {
        char cVol[8]; snprintf(cVol, 8, "%d", (int)(m_maxVolume * v + 0.5));
        BELog("SetVolume %f, %s\n", m_maxVolume * v,  cVol);
        string s = sef_audio->Execute("SetVolume", cVol, "", "", "");
        m_player->volumeChanged(v);
    }
}

void MediaPlayerPrivate::setMuted(bool m) 
{ 
    BELog("m = %d\n", m);

    const char* strMute = m ? "1" : "0";
    CSefPluginWebkit* sef_audio = MMManager::getInstance()->sefAudio();

    if (s_video != this)
        return;

    switch (m_state)
    {
        case S_INITIAL:
        case S_LOADED:
        case S_PAUSED:
        case S_WILL_STOP:
        case S_STOPPED:
            BELog("No action is taken for inactive video.\n");
            break;

        case S_PLAY_REQUESTED:
        case S_RENDERING:
        {
            if (s_video != this)
                BELog("Assertion Failed\n");

            if (sef_audio)
            {
                string r = sef_audio->Execute("SetUserMute", strMute, "", "", "");
                BELog("Return of setMuted : %s\n", r.c_str());
            }
            break;
        }
    }
}

float MediaPlayerPrivate::currentTime() const
{
    if (m_seeking)
        return m_seekTime / 1000.0;

    return m_playbackPosition / 1000.0;
}

void MediaPlayerPrivate::seek(float time)
{
    BELog("seek to %.2f s, currentPosition = %.2f ms\n", time, m_playbackPosition);

    switch (m_state)
    {
        case S_INITIAL:
        case S_LOADED:
        case S_PAUSED:
        case S_WILL_STOP:
        case S_STOPPED:
            BELog("No action is taken for inactive video.\n");
            break;

        case S_PLAY_REQUESTED:
        case S_RENDERING:
        {
            if (s_video != this)
                BELog("Assertion Failed: s_video != this\n");

            if (errorOccurred())
                return;

            if (!m_uniplayer)
                return;

            float t = time - m_playbackPosition / 1000.0;
            
            if (t >= -0.02 && t <= 0.02)   // avoid useless seeking
                return;

            m_seeking = true;

            char param[64]; snprintf(param, 64, "%f", t);
            m_seekTime = time;
            BELog("JumpForward(%s)\n", param);
            string r = m_uniplayer->Execute("JumpForward", param, "", "", "");
            BELog("Return of JumpForward : %s\n", r.c_str());
            if (strcmp(r.c_str(), "I1") == 0)
            {
                m_playbackPosition = time * 1000.0;
                m_player->timeChanged();
            }
            m_seeking = false;

            break;
        }
    }
}

float MediaPlayerPrivate::duration() const
{
    if (!m_uniplayer)
        return 0.0;

    if (errorOccurred())
        return 0.0;

    return m_mediaDuration / 1000.0;
}

void MediaPlayerPrivate::setMediaLocation(String s)
{
    KURL url = KURL(KURL(), s);
    BELog("New URL = %s\n", url.string().utf8().data());
    BELog("Prev URL = %s\n", m_mediaLocation.string().utf8().data());

    if (url.host().isEmpty())
    {
        url = KURL( KURL(ParsedURLString, m_mediaLocation.baseAsString()), s);
        BELog("URL2 = %s\n", url.string().utf8().data());
    }

    BELog("URL = %s\n", url.string().utf8().data());
    m_mediaLocation = url;
}

void MediaPlayerPrivate::load(const String& url)
{
    BELog("this = %p, s_video = %p\n", this, s_video);
    BELog("Load %s\n", url.utf8().data());

    m_state = S_LOADED;

    setMediaLocation(url);

    if (m_preload == MediaPlayer::None) {
        BELog("Delaying load.");
        m_delayingLoad = true;
        return;
    }

    commitLoad();
}

void MediaPlayerPrivate::commitLoad()
{
    BELog("Committing load.\n");

#if 0 // autoplay is enabled in case of audio element
    if (this->hasAudio())
    {
        BELog("\n");
        HTMLMediaElement* me = static_cast<HTMLMediaElement*>(m_player->mediaPlayerClient());
        if (me && me->autoplay())
        {
            BELog("\n");
            this->playMediaElement();
        }
    }
#endif

    if (m_networkState != MediaPlayer::Loading) {
        m_networkState = MediaPlayer::Loading;
        m_player->networkStateChanged();
    }
    if (m_readyState != MediaPlayer::HaveNothing) {
        m_readyState = MediaPlayer::HaveNothing;
        m_player->readyStateChanged();
    }
}

void MediaPlayerPrivate::prepareToPlay()
{
    BELog("\n");

    switch (m_state)
    {
        case S_INITIAL:
        case S_LOADED:
        case S_WILL_STOP:
        case S_STOPPED:
        {
            BELog("\n");

            // @20100925-sanggyu: Where should the following code bloack go ?
            ss = ResourceHandle::defaultSession();
            ensureSessionIsInitialized(ss);
            SoupMessage* soupMessage = soup_message_new(SOUP_METHOD_GET, m_mediaLocation.string().utf8().data());

            //soup_message_set_flags(soupMessage, m_soupFlags);
            //soup_message_headers_append(soupMessage, "Accept", "*/*");

            g_signal_connect(soupMessage, "got-headers", G_CALLBACK(gotHeadersCallback), this);
            g_signal_connect(soupMessage, "got-chunk", G_CALLBACK(gotChunkCallback), this);
            g_signal_connect(soupMessage, "got-body", G_CALLBACK(gotBodyCallback), this);
            soup_session_queue_message(ss, soupMessage, finishedCallback, this);

            if (m_delayingLoad) {
                m_delayingLoad = false;
                commitLoad();
            }
            break;
        }

        case S_PAUSED:
        case S_PLAY_REQUESTED:
        case S_RENDERING:
            BELog("No action is taken for loaded video.\n");
            break;
    }
}

static bool doUniInit()
{
    sef_bus_init(&sef_bus);
    return true;
}

bool MediaPlayerPrivate::isAvailable()
{
    BELog("\n");
    if (doUniInit()) {
        BELog("SUCCEED.\n");
        return true;
    }
    BELog("FAILED.\n");
    return true;
}

PassOwnPtr<MediaPlayerPrivateInterface> MediaPlayerPrivate::create(MediaPlayer* player)
{
    return adoptPtr(new MediaPlayerPrivate(player));
}

static HashSet<String> mimeTypeCache()
{
    static HashSet<String> cache;
    static bool typeListInitialized = false;

    if (!typeListInitialized) {
        cache.add(String("video/mp4"));
        cache.add(String("video/x-ms-wmv"));
        cache.add(String("audio/aac"));
        cache.add(String("audio/wav"));
        cache.add(String("audio/x-wav"));
        cache.add(String("audio/x-ms-wma"));
        cache.add(String("audio/mpeg"));
        cache.add(String("audio/mp1"));
        cache.add(String("audio/mp2"));
        cache.add(String("audio/mp3"));
        cache.add(String("audio/mp4"));
        cache.add(String("video/webm"));
        cache.add(String("audio/webm"));

        //cache.add(String("video/ogg"));
        //cache.add(String("audio/ogg"));

        typeListInitialized = true;
    }

    return cache;
}

void MediaPlayerPrivate::getSupportedTypes(HashSet<String>& types)
{
    types = mimeTypeCache();
}

MediaPlayer::SupportsType MediaPlayerPrivate::supportsType(const String& type, const String& codecs)
{
    if (type.isNull() || type.isEmpty())
        return MediaPlayer::IsNotSupported;

    // spec says we should not return "probably" if the codecs string is empty
    if (mimeTypeCache().contains(type))
        return codecs.isEmpty() ? MediaPlayer::MayBeSupported : MediaPlayer::IsSupported;

    return MediaPlayer::IsNotSupported;
}

void MediaPlayerPrivate::registerMediaEngine(MediaEngineRegistrar registrar)
{
    if (isAvailable())
        registrar(create, getSupportedTypes, supportsType, 0, 0, 0);
}

void MediaPlayerPrivate::setPreload(MediaPlayer::Preload preload)
{
    m_preload = preload;

    if (m_delayingLoad && m_preload != MediaPlayer::None) {
        m_delayingLoad = false;
        commitLoad();
    }
}

void MediaPlayerPrivate::repaint()
{
    BELog("\n");
    m_player->repaint();
}

void MediaPlayerPrivate::repaintVideo()
{
    BELog("%p\n", this);

    switch (m_state)
    {
        case S_INITIAL:
        case S_WILL_STOP:
        case S_STOPPED:
            BELog("No action is taken for loaded video.\n");
            return;
    }

    if (s_video != this)
    {
        BELog("Assertion Failed: s_video = %p, this = %p\n", s_video, this);
        return;
    }

    if (!m_uniplayer)
        return;

    if (!hasVideo())
        return;

    // Initialize video rect
    IntRect videoRect = m_lastPaintRect;
    BELog("videoRect in ContentView Coordinate = (%d %d %d %d)\n", 
            videoRect.x(), videoRect.y(), videoRect.width(), videoRect.height());

    // Convert window coordinate to screen coordinate
    IntRect videoRectInSC = m_player->frameView()->contentsToScreen(videoRect);
    BELog("videoRect in Screen Coordinate = (%d %d %d %d)\n", 
            videoRectInSC.x(), videoRectInSC.y(), videoRectInSC.width(), videoRectInSC.height());

    // Get video geometry in screen coordinate
    IntRect visibleVideo = videoRectInSC;
    IntRect screenRect(IntPoint(), MMManager::getInstance()->screenSize());
    visibleVideo.intersect(screenRect);
    BELog("visibleVideo in Screen Coordinate = (%d %d %d %d) [ in screenRect = (%d %d %d %d) ]\n", 
            visibleVideo.x(), visibleVideo.y(), visibleVideo.width(), visibleVideo.height(),
            screenRect.x(), screenRect.y(), screenRect.width(), screenRect.height());

    // Do nothing for invisible video.
    if (visibleVideo.isEmpty())
        return;

    // Do crop
    IntRect cropRect = visibleVideo;
    cropRect.move(-videoRectInSC.x(), -videoRectInSC.y());
    // Compute crop rect before applying scale
    BELog("cropRect before normalization (%d %d %d %d)\n", 
            cropRect.x(), cropRect.y(), cropRect.width(), cropRect.height());
    BELog("original video size (%d %d), Scaled video size (%d %d)\n", 
            m_videoSize.width(), m_videoSize.height(), 
            videoRect.width(), videoRect.height());
    if (videoRect.width() != 0)
    {
        BELog("Reverse Scaling Factor = %.6f\n", 
                (float) m_videoSize.width() / videoRect.width());
        cropRect.scale( (float)m_videoSize.width() / videoRect.width());
    }
    char cx[16]; snprintf(cx, 16, "%d", cropRect.x());
    char cy[16]; snprintf(cy, 16, "%d", cropRect.y());
    char cw[16]; snprintf(cw, 16, "%d", cropRect.width());
    char ch[16]; snprintf(ch, 16, "%d", cropRect.height());
    BELog("SetCropArea(%s %s %s %s false)\n", cx, cy, cw, ch);
    m_uniplayer->Execute("SetCropArea", cx, cy, cw, ch, "0", "", "", "", "");

    // Show video in final destination
    char sx[16]; snprintf(sx, 16, "%d", visibleVideo.x());
    char sy[16]; snprintf(sy, 16, "%d", visibleVideo.y());
    char sw[16]; snprintf(sw, 16, "%d", visibleVideo.width());
    char sh[16]; snprintf(sh, 16, "%d", visibleVideo.height());
    char rh[16]; snprintf(rh, 16, "%d", screenRect.height());
    BELog("SetDisplayArea(%s %s %s %s %s, false)\n", sx, sy, sw, sh, rh);
    m_uniplayer->Execute("SetDisplayArea", sx, sy, sw, sh, rh, "0", "", "", "", "");
}

void MediaPlayerPrivate::alert(const char* msg)
{
#if 0 // TODO
    gboolean retval;
    WebKitWebView* webview = const_cast<WebKitWebView*>(WebKit::BackingStore::getInstance()->webView());
    Frame* frame = m_player->frameView() ? m_player->frameView()->frame() : 0;

    if (webview && frame)
    {
        BELog("%s\n", msg);
        g_signal_emit_by_name(webview, "script-translated-alert", WebKit::kit(frame), msg, &retval);
    }
#endif
}

void MediaPlayerPrivate::paint(GraphicsContext* context, const IntRect& rect)
{
    BELog("%p (%d, %d, %d, %d)\n", this,
            rect.x(), rect.y(), rect.width(), rect.height(), syscall(SYS_gettid));

    if (context->paintingDisabled())
        return;

    if (!hasVideo())
        return;

    if (!m_player->visible())
        return;

    if (m_player->frameView())
    {
        if (m_player->frameView()->frame())
            ;
            // TODO BELog("zoom level = %lf\n", m_player->frameView()->frame()->zoomFactor());
    }

    switch (m_state)
    {
      case S_PAUSED:
      case S_RENDERING:
        context->clearRect(rect);
        if (m_lastPaintRect != rect)
        {
            BELog("m_lastPaintRect != rect \n");
            m_lastPaintRect = rect;
            repaintVideo();
        }
        break;
      case S_INITIAL:
      case S_LOADED:
      case S_PLAY_REQUESTED:
      case S_WILL_STOP:
      case S_STOPPED:
        BELog("Don't paint video for INITIAL and STOPPED, just fill with black.\n");
        context->fillRect(rect, Color(0, 0, 0), ColorSpaceDeviceRGB);
        break;
    }
}

float MediaPlayerPrivate::maxTimeSeekable() const
{
    return duration();
}

MediaPlayer::NetworkState MediaPlayerPrivate::networkState() const
{
    return m_networkState;
}

MediaPlayer::ReadyState MediaPlayerPrivate::readyState() const
{
    return m_readyState;
}

void MediaPlayerPrivate::showVideo(bool b) const
{
    const char* p = b ? "1" : "0";
    CSefPluginWebkit* sef_window = MMManager::getInstance()->sefMainWindow();

    if (sef_window)
    {
        string str = sef_window->Execute("Show", p, "", "", "");
        BELog("return of Show %s = %s\n", p, str.c_str());
    }
}

void MediaPlayerPrivate::didEnd()
{
    BELog("EOS reached\n");
    m_player->timeChanged();

    HTMLMediaElement* me = static_cast<HTMLMediaElement*>(m_player->mediaPlayerClient());
    if (me && me->ended())
    {
        BELog("ended. no loop\n");
        this->pauseMediaElement();
    }
}

bool MediaPlayerPrivate::isH5VPlaying() 
{ 
    return (s_video != NULL); 
} 

void MediaPlayerPrivate::pauseH5V()
{
    if (s_video)
        s_video->pauseMediaElement();
}

void MediaPlayerPrivate::resumeH5V()
{
    if (s_video)
        s_video->playMediaElement();
}

void MediaPlayerPrivate::stopH5V()
{
    if (s_video)
        s_video->cleanUp();
}

void MediaPlayerPrivate::willScroll()
{
    if (s_video && s_video->hasVideo())
    {
        s_video->showVideo(false);
        s_video->repaintVideo();
    }
}

void MediaPlayerPrivate::didScroll()
{
    BELog("\n");
    if (s_video && s_video->hasVideo()) {
        s_video->repaintVideo();
        //s_video->showVideo(true);
    }
}

void MediaPlayerPrivate::didContentViewRectChanged(IntRect r)
{
    MMManager::getInstance()->setContentViewRect(r);

    //BELog("contentViewRect = (%d, %d, %d, %d)\n", r.x(), r.y(), r.width(), r.height());
    if (s_video && s_video->hasVideo())
    {
        //s_video->showVideo(false);
        s_video->repaintVideo();
        //s_video->showVideo(true);
    }
}

} // End of namespace WebCore

/* } End of MediaPlayerPrivate */

#endif /* End of USE(UNIPLAYER) */
