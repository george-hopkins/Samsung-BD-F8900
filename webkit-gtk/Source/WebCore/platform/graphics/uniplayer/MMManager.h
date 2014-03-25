/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 * @file        MMManager.h
 * @brief       Multimedia Manager for SEC TV Platform
 * @author      Hurnjoo Lee (hurnjoo.lee@samsung.com)
 * @date        2010.12.12
 */

#ifndef MMManager_h
#define MMManager_h

#include <cairo.h>
#include <wtf/Platform.h>
#if ENABLE(SEF)
#include "CSefPluginWebkit.h"
#endif // #if ENABLE(SEF)
#include "Color.h"
#include "IntPoint.h"
#include "IntRect.h"
//TODO #include "webkitwebview.h"

namespace WebCore {
//using namespace WebKit;

class MMManager {
private:
    MMManager();
    int m_originalSource;
    int m_currentSource;
    bool m_isPipEnabled;
    bool m_isMediaOriginalSource;
    IntRect m_pipRect;
    IntSize m_screenSize;
    IntPoint m_pipLocationOffset;
    bool m_bInit;
#if USE(UNIPLAYER)
    IntRect m_contentViewRect;
#endif
#if ENABLE(SEF)
    CSefPluginWebkit* m_sefAppCommon;
    CSefPluginWebkit* m_sefAudio;
    CSefPluginWebkit* m_sefMainWindow;
    CSefPluginWebkit* m_sefPIPWindow;
    CSefPluginWebkit* m_sefScreen;
    CSefPluginWebkit* m_sefTV;
#endif // #if ENABLE(SEF)
public:
    void init(bool bPreloading=false);
    static MMManager* getInstance();
#if ENABLE(SEF)
    CSefPluginWebkit* sefAppCommon();
    CSefPluginWebkit* sefAudio();
    CSefPluginWebkit* sefMainWindow();
    CSefPluginWebkit* sefPIPWindow();
    CSefPluginWebkit* sefScreen();
    CSefPluginWebkit* sefTV();
#endif // #if ENABLE(SEF)
    IntRect getPipRect() { return m_pipRect; }
    bool isPipEnabled() { return m_isPipEnabled; }
    bool setPipEnabled(bool enable);
    void setPipRect(IntRect& rect);
    void setScreenSize(const IntSize& size) { m_screenSize = size; }
    IntSize screenSize() const { return m_screenSize; }
    void setPipLocationOffset(const IntPoint& location) { m_pipLocationOffset = location; }
    IntPoint pipLocationOffset() { return m_pipLocationOffset; }
#if USE(UNIPLAYER)
    void setContentViewRect(const IntRect& rect) { m_contentViewRect = rect; }
    IntRect contentViewRect() const { return m_contentViewRect; }
#endif
    bool returnToOriginalSource(); // for PIP
    int getCurrentSource();
    int getResolution(int *width, int *height);
    bool setMediaSource(); // for Media play (HTML5, Flash video)
    bool setAudioMainWindow(bool isMain);
    int audioWindow();
    bool get3DAutoView();
    bool set3DAutoView(bool bEnable);
    bool setTVPIP(bool bEnable);
    bool isTVPIP();
    bool sendPIPOnOffEvent(bool bOnOff);
    void setOriginalSource(int source) { m_originalSource = source; }
    void setMaskPIPAreaToCairoContext(cairo_t* context);
};

}
#endif // #ifdef MMMANAGER_H
