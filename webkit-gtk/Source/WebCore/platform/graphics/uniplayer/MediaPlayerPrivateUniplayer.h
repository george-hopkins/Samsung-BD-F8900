/*
 * Copyright (C) 2007, 2009 Apple Inc.  All rights reserved.
 * Copyright (C) 2007 Collabora Ltd. All rights reserved.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
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

#ifndef MediaPlayerPrivateUniplayer_h
#define MediaPlayerPrivateUniplayer_h

#include "config.h"

#if USE(UNIPLAYER)

#include "MediaPlayerPrivate.h"
#include "Timer.h"
#include "KURL.h"

#include <cairo.h>
#include <glib.h>

#include "TimeRanges.h"
#include "CSefPluginWebkit.h"

namespace WebCore {

class GraphicsContext;
class IntSize;
class IntRect;
class MediaPlayerPrivate;

class CSefPluginUniPlayer : public CSefPluginWebkit {
public:
    virtual bool OnEvent(int eventType, string param1, string param2);
    void setMediaPlayerPrivate(MediaPlayerPrivate* p) { m_pp = p; };
private:
    MediaPlayerPrivate* m_pp;
};

class MediaPlayerPrivate : public MediaPlayerPrivateInterface {
    friend class CSefPluginUniPlayer;

// From MediaPlayerPrivateInterface.h
public:
    virtual ~MediaPlayerPrivate();

    virtual void load(const String&);
    virtual void cancelLoad() { }
    
    virtual void prepareToPlay();

    virtual void play();
    virtual void pause();

    void playMediaElement();
    void pauseMediaElement();

    virtual bool supportsFullscreen() const { return true; }

    virtual IntSize naturalSize() const;

    virtual bool hasVideo() const;
    virtual bool hasAudio() const;

    virtual void setVisible(bool) { }

    virtual float duration() const;

    virtual float currentTime() const;
    virtual void seek(float);
    virtual bool seeking() const { return m_seeking; }

    virtual void setRate(float) { }  // Uniplayer does not support rate
    virtual void setPreservesPitch(bool) { }
    virtual bool paused() const;

    virtual void setVolume(float);
    virtual bool supportsMuting() const { return true; }
    virtual void setMuted(bool);

    virtual bool hasClosedCaptions() const { return false; }
    virtual void setClosedCaptionsVisible(bool) { };

    virtual MediaPlayer::NetworkState networkState() const;
    virtual MediaPlayer::ReadyState readyState() const;

    virtual float maxTimeSeekable() const;
    virtual PassRefPtr<TimeRanges> buffered() const { return TimeRanges::create(); }
    virtual unsigned bytesLoaded() const { return 0; }

    virtual void setSize(const IntSize&) { }

    virtual void paint(GraphicsContext*, const IntRect&);

    virtual void setPreload(MediaPlayer::Preload);

    virtual bool canLoadPoster() const { return false; }
    virtual void setPoster(const String&) { }
    virtual bool hasSingleSecurityOrigin() const { return true; }

public:
    void didEnd();
    void durationChanged();
    unsigned totalBytes() const { return 0; }

public:
    enum Error { E_NO_ERROR, E_NOT_SUPPORTED_CODEC, E_NO_REDIRECTION_LOCATION, E_CONNECTION_FAILED };
    enum UniplayerState { S_INITIAL, S_LOADED, S_PLAY_REQUESTED, S_RENDERING, S_PAUSED, S_WILL_STOP, S_STOPPED };
    void repaint();
    void repaintVideo();
    static void registerMediaEngine(MediaEngineRegistrar);
    bool updateStates(int eventType, string param1, string param2);
    void playPostAction();
    void setMediaLocation(String s);
    bool redirectDone() const { return m_redirectDone; }
    void setRedirectDone() { m_redirectDone = true; }

    void setError(Error e) { m_error = e; }
    bool errorOccurred() const { return m_error; };

    void cleanUp(); 
    void showVideo(bool b) const;

    static bool isH5VPlaying();
    static void pauseH5V();
    static void resumeH5V();
    static void stopH5V();
    static void willScroll();
    static void didScroll();
    static void didContentViewRectChanged(IntRect r);

private:
    MediaPlayerPrivate(MediaPlayer*);
    void commitLoad();
    void alert(const char* msg);
    void playImpl();
    void init();

    /* MediaEngine triple (create, getSupportedTypes, supportsType) */
    static PassOwnPtr<MediaPlayerPrivateInterface> create(MediaPlayer* player);
    static void getSupportedTypes(HashSet<String>&);
    static MediaPlayer::SupportsType supportsType(const String& type, const String& codecs);

    /* availabe when uniplayer is initialized */
    static bool isAvailable();

private:
    /* @20100925-sanggyu: copied from GStreamer port. 
     * Most of them is defined in HTML5 Video Specification.
     */
    MediaPlayer*              m_player;
    MediaPlayer::NetworkState m_networkState;
    MediaPlayer::ReadyState   m_readyState;
    bool                      m_paused;
    bool                      m_seeking;
    float                     m_mediaDuration;     // in milisecond
    MediaPlayer::Preload      m_preload;
    bool                      m_delayingLoad;

    /* Added for TV Port. MediaPlayerPrivateGStreamer does not have followings */
    KURL                      m_mediaLocation;
    bool                      m_redirectDone;
    float                     m_playbackPosition;  // in milisecond
    float                     m_seekTime;          // in milisecond
    IntSize                   m_videoSize;
    IntRect                   m_lastPaintRect;
    Error                     m_error;
    int                       m_maxVolume;
    CSefPluginUniPlayer*      m_uniplayer;
    UniplayerState            m_state;
};

} // end of namespace WebCore

#endif // end of USE(UNIPLAYER)
#endif // end of MediaPlayerPrivateUniplayer_h
