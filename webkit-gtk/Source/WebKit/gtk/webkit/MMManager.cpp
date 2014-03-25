
/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 * @file        MMManager.cpp
 * @brief       Multimedia Manager for SEC TV Platform
 * @author      hurnjoo.lee (hurnjoo.lee@samsung.com)
 * @date        2010.12.12
 */

#include "config.h"
#include "MMManager.h"

#include "CString.h"
#include "IntRect.h"

#include <gdk/gdkx.h>

#if ENABLE(SEF)
#include "LegacyEmp/Window/EmpCommonWindow.h"
#endif

#if ENABLE(TVGG_BACKINGSTORE)
#include "BackingStoreGtk.h"
extern "C" void UpdateRootWindow(WebCore::IntRect* pRect);
#endif

namespace WebKit {

static int pipInspectorTimerId = -1;

static gboolean pipInspectorCallBack(gpointer callBackData)
{
#if ENABLE(SEF)
    CSefPluginWebkit* pSefMainWindow = MMManager::getInstance()->sefMainWindow();

    if (!pSefMainWindow)
        return false;

    string str = pSefMainWindow->Execute("GetSource", "", "", "", "");
    bool isMediaSource = false;
    const char* t = str.c_str();
    switch (t[0]) {
    case EMP_RETURN_TYPE_PREFIX_INTEGER:
        if (sef::PL_WINDOW_SOURCE_MEDIA == (int)atoi(&t[1]))
        isMediaSource = true;
        break;
    default:
        printf(" return type mismatched! "
                "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
        break;
    }

    if (isMediaSource) {
        if (pipInspectorTimerId != -1)
            g_source_remove(pipInspectorTimerId);
        pipInspectorTimerId = -1;
        MMManager::getInstance()->setPipEnabled(false);
        return false;
    } else {
        MMManager::getInstance()->setOriginalSource((int)atoi(&t[1]));
    }
    return true;
#else
    return false;
#endif // #if ENABLE(SEF)
}

static void pipInspectorClear(gpointer callBackData)
{
    pipInspectorTimerId = -1;
}

MMManager::MMManager()
    : m_currentSource(0)
    , m_isMediaOriginalSource(false)
    , m_isPipEnabled(false)
    , m_originalSource(0)
#if ENABLE(SEF)
    , m_sefAppCommon(NULL)
    , m_sefAudio(NULL)
    , m_sefMainWindow(NULL)
    , m_sefPIPWindow(NULL)
    , m_sefScreen(NULL)
    , m_sefTV(NULL)
#endif // #if ENABLE(SEF)
{
}

MMManager* MMManager::getInstance()
{
    static MMManager instance;
    return &instance;
}

#if ENABLE(SEF)
CSefPluginWebkit* MMManager::sefAppCommon()
{
    if (!m_sefAppCommon) {
        char* s = "AppCommon";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }
        m_sefAppCommon = sef;
    }
    return m_sefAppCommon;
}

CSefPluginWebkit* MMManager::sefAudio()
{
    if (!m_sefAudio) {
        char* s = "Audio";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }
        m_sefAudio = sef;
    }
    return m_sefAudio;
}

CSefPluginWebkit* MMManager::sefMainWindow()
{
    if (!m_sefMainWindow) {
        char* s = "Window";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }

        m_sefMainWindow = sef;

        if (sef)
            sef->Execute("SetWindow", "0", "", "", "");
    }
    return m_sefMainWindow;
}

CSefPluginWebkit* MMManager::sefPIPWindow()
{
    if (!m_sefPIPWindow) {
        char* s = "Window";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }

        m_sefPIPWindow = sef;

        if (sef)
            sef->Execute("SetWindow", "1", "", "", "");
    }
    return m_sefPIPWindow;
}

CSefPluginWebkit* MMManager::sefScreen()
{
    if (!m_sefScreen) {
        char* s = "Screen";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }
        m_sefScreen = sef;
    }
    return m_sefScreen;
}

CSefPluginWebkit* MMManager::sefTV()
{
    if (!m_sefTV) {
        char* s = "TV";
        CSefPluginWebkit* sef = new CSefPluginWebkit();
        if (!sef->Open(s, "1.000", s)) {
            printf("Opening %s Failed\n", s);
            delete sef;
            sef = NULL;
        }
        m_sefTV = sef;
    }
    return m_sefTV;
}

#endif // #if ENABLE(SEF)

bool MMManager::setPipEnabled(bool bEnable)
{
#if ENABLE(TVGG_BACKINGSTORE)
    IntRect tempRect = m_pipRect;
    IntRect viewRect = BackingStore::getInstance()->viewRect();

    tempRect.move(viewRect.x(), viewRect.y());

    if (m_isPipEnabled != bEnable) {
        m_isPipEnabled = bEnable;
        UpdateRootWindow(&tempRect);
        gdk_window_process_all_updates();
    }

    if (pipInspectorTimerId >= 0) {
        g_source_remove(pipInspectorTimerId);
        pipInspectorTimerId = -1;
    }

    if (bEnable) {
        returnToOriginalSource();
        setPipRect(m_pipRect);

        if (!m_isMediaOriginalSource) {
            pipInspectorTimerId = g_timeout_add_full(G_PRIORITY_HIGH, 500,
                    pipInspectorCallBack, 0, pipInspectorClear);
        }
    } else {
#if ENABLE(SEF)
        int currentSource = getCurrentSource();
        if (currentSource != sef::PL_WINDOW_SOURCE_MEDIA)
            m_originalSource = currentSource;
#endif
        setMediaSource();
    }

//    setTVPIP(bEnable);
//    sendPIPOnOffEvent(bEnable);

    if (m_isPipEnabled == bEnable) {
        UpdateRootWindow(&tempRect);
        gdk_window_process_all_updates();
    }
#endif

    return true;
}

void MMManager::setPipRect(IntRect& rect)
{ 
#if ENABLE(TVGG_BACKINGSTORE)
    if (m_pipRect != rect) {
        m_pipRect = rect;
        UpdateRootWindow(0);
    }
#endif
    WebCore::IntRect tempRect = rect;

#if ENABLE(TVGG_BACKINGSTORE)
    IntRect pluginViewRect = BackingStore::getInstance()->pluginViewRect();
#endif // #if ENABLE(TVGG_BACKINGSTORE)

//    tempRect.move(pluginViewRect.x(), pluginViewRect.y());
    tempRect.move(0, 70);
    tempRect.scale(3.0 / 4.0);

    char sx[16];
    char sy[16];
    char sw[16];
    char sh[16];

    // Add margin to pip area.
    tempRect.inflate(1);

    snprintf(sx, 16, "%d", tempRect.x());
    snprintf(sy, 16, "%d", tempRect.y());
    snprintf(sw, 16, "%d", tempRect.width());
    snprintf(sh, 16, "%d", tempRect.height());

#if ENABLE(SEF)
    if (isPipEnabled() && sefMainWindow()) {
        sefMainWindow()->Execute("SetScreenRect_PosSizeMode", "7", "6", "", "");
        sefMainWindow()->Execute("SetScreenRect", sx, sy, sw, sh);
    }
#endif // #if ENABLE(SEF)
}

int MMManager::getCurrentSource()
{
#if ENABLE(SEF)
    if (sefMainWindow()) {
        // get original source from sef
        int source = -1;
        string str = sefMainWindow()->Execute("GetSource", "", "", "", "");
        const char* t = str.c_str();
        switch (t[0]) {
        case EMP_RETURN_TYPE_PREFIX_INTEGER:
            source = (int)atoi(&t[1]);
            return source; 
        default:
            printf(" return type mismatched! "
                    "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
            break;
        }
    }
#endif // #if ENABLE(SEF)
    return -1;
}

void MMManager::init()
{
    static bool bInit = false;
#if ENABLE(SEF)
    if (!bInit && sefMainWindow()) {
        m_originalSource = getCurrentSource();
        if (m_originalSource == sef::PL_WINDOW_SOURCE_MEDIA) {
            m_originalSource = 0;
            m_isMediaOriginalSource = true;
        }
        bInit = true;
    }
#endif // #if ENABLE(SEF)
}

bool MMManager::returnToOriginalSource()
{
#if ENABLE(SEF)
    // set original source
    if (sefMainWindow()
        && getCurrentSource() != m_originalSource) {
        char arg[16];
        snprintf(arg, 16, "%d", m_originalSource);
        // get original source from sef
        string str = sefMainWindow()->Execute("SetSource", arg, "", "", "");
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::setMediaSource()
{
#if ENABLE(SEF)
    // set original source
    if (sefMainWindow()
            && getCurrentSource() != sef::PL_WINDOW_SOURCE_MEDIA) {
        char arg[16];
        snprintf(arg, 16, "%d", sef::PL_WINDOW_SOURCE_MEDIA);
        // get original source from sef
        string str = sefMainWindow()->Execute("SetSource", arg, "", "", "");
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::setAudioMainWindow(bool isMain)
{
#if ENABLE(SEF)
    if (sefAudio()) {
        char arg[16];
        snprintf(arg, 16, "%d", isMain ? 1 : 0);
        // get original source from sef
        string str = sefAudio()->Execute("SetWindow", arg, "", "", "");
    }
#endif // #if ENABLE(SEF)
    return true;
}

int MMManager::audioWindow()
{
#if ENABLE(SEF)
    bool audioWindow = 0;
    if (sefAudio()) {
        // get original source from sef
        string str = sefAudio()->Execute("GetWindow", "", "", "", "");
        const char* t = str.c_str();
        switch (t[0]) {
        case EMP_RETURN_TYPE_PREFIX_INTEGER:
            audioWindow = (int)atoi(&t[1]);
            return audioWindow;
        default:
            printf(" return type mismatched! "
                    "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
            break;
        }
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::get3DAutoView()
{
#if ENABLE(SEF)
    bool is3DAutoView = false;
    if (sefScreen()) {
        // get original source from sef
        string str = sefScreen()->Execute("Get3DAutoView", "", "", "", "");
        const char* t = str.c_str();
        switch (t[0]) {
        case EMP_RETURN_TYPE_PREFIX_INTEGER:
            is3DAutoView = (bool)atoi(&t[1]);
            return is3DAutoView;
        default:
            printf(" return type mismatched! "
                    "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
            break;
        }
    }
#endif // #if ENABLE(SEF)
    return false;
}

bool MMManager::set3DAutoView(bool bEnable)
{
#if ENABLE(SEF)
    if (sefScreen()) {
        string str = sefScreen()->Execute("Set3DAutoView", bEnable ? "1" : "0", "", "", "");
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::setTVPIP(bool bEnable)
{
#if ENABLE(SEF)
    if (sefPIPWindow() && sefTV()) {
        char src[16];

        // Prevent that set PIP window source to media source.
        if (m_originalSource != sef::PL_WINDOW_SOURCE_MEDIA) {
            sprintf(src, "%d", m_originalSource);
            sefPIPWindow()->Execute("SetSource", src, "2", "", "");
        }

        sefTV()->Execute("SetPIP", bEnable ? "1" : "0", "", "", "");
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::isTVPIP()
{
#if ENABLE(SEF)
    if (sefTV()) {
        // get original source from sef
        string str = sefTV()->Execute("GetPIP", "", "", "", "");
        const char* t = str.c_str();
        switch (t[0]) {
        case EMP_RETURN_TYPE_PREFIX_INTEGER:
            return (bool)atoi(&t[1]);
        default:
            printf(" return type mismatched! "
                    "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
            break;
        }
    }
#endif // #if ENABLE(SEF)
    return true;
}

bool MMManager::sendPIPOnOffEvent(bool bOnOff)
{
#if ENABLE(SEF)
    if (sefAppCommon()) {
        // get original source from sef
        string str = sefAppCommon()->Execute("SendEvent_FULLBROWSER_PIP_ONOFF"
                                             , bOnOff ? "1" : "0", "", "", "");
        const char* t = str.c_str();
        switch (t[0]) {
        case EMP_RETURN_TYPE_PREFIX_INTEGER:
            return (bool)atoi(&t[1]);
        default:
            printf(" return type mismatched! "
                    "EMP_RETURN_TYPE_PREFIX_INTEGER is expected. But %c comes\n", t[0]);
            break;
        }
    }
#endif // #if ENABLE(SEF)
    return true;
}

} // namespace WebKit


