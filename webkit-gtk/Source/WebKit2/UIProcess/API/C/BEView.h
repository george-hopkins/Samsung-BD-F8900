/**
 * Copyright 2011 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 * @file        BEView.h
 * @brief       Declaration of BEView api
 * @author      Sangyong Park (sy302.park@samsung.com)
 * @date        2011.08.23
 */

#ifndef BEView_h
#define BEView_h

#include "BEBase.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NPObject* NPObjectRef;

/**
 * @ingroup    BEView
 * @brief      create web view
 * @param[in]  context                 runtime context
 * @param[in]  pageGroup               page group of new web view
 * @return     new web view
 */
BEViewRef BEViewCreate(BEContextRef context, BEPageGroupRef pageGroup);

/**
 * @ingroup    BEView
 * @brief      create web view with DirectDrawing (browser ui view use only)
 * @param[in]  context                 runtime context
 * @param[in]  pageGroup               page group of new web view
 * @return     new web view
 */
BEViewRef BEViewCreateWithDirectDrawing(BEContextRef context, BEPageGroupRef pageGroup);

/**
 * @ingroup    BEView
 * @brief      create web view with NotifyDrawing (browser content view use only)
 * @param[in]  context                 runtime context
 * @param[in]  pageGroup               page group of new web view
 * @return     new web view
 */
BEViewRef BEViewCreateWithNotifyDrawing(BEContextRef context, BEPageGroupRef pageGroup);

/**
 * @ingroup    BEView
 * @brief      destroy web view
 * @param[in]  view                    web view
 * @return     
 */
void BEViewDestroy(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      get BEPage reference
 * @param[in]  view                    web view
 * @return     BEPage reference
 */
BEPageRef BEViewGetPage(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      show web view
 * @param[in]  view                    web view
 * @return     
 */
void BEViewShow(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      hide web view
 * @param[in]  view                    web view
 * @return     
 */
void BEViewHide(BEViewRef view);

#ifdef ENABLE_HBBTV
/**
 * @ingroup    BEView
 * @brief      Report if anything was rendered within view
 * @return     true if view is fully transparent, false otherwise.
 */
bool BEViewIsFullyTransparent();
#endif // ENABLE_HBBTV

/**
 * @ingroup    BEView
 * @brief      get visibility of web view
 * @param[in]  view                    web view
 * @return     visibility
 */
bool BEViewGetVisible(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      set background color of web view
 * @param[in]  view                    web view
 * @param[in]  red                     red value
 * @param[in]  green                   green value
 * @param[in]  blue                    blue value
 * @param[in]  alpha                   alpha value
 * @return     
 */
void BEViewSetBackgroundColor(BEViewRef view, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

/**
 * @ingroup    BEView
 * @brief      set position of web view in screen
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @return     
 */
void BEViewSetPosition(BEViewRef view, int x, int y);

/**
 * @ingroup    BEView
 * @brief      get position of web view in screen
 * @param[in]  view                    web view
 * @return     position
 */
BEPoint BEViewGetPosition(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      set size of web view
 * @param[in]  view                    web view
 * @param[in]  width                   width
 * @param[in]  height                  height
 * @return     
 */
void BEViewSetSize(BEViewRef view, int width, int height);

/**
 * @ingroup    BEView
 * @brief      get size of web view
 * @param[in]  view                    web view
 * @return     size
 */
BESize BEViewGetSize(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      set location value of web view in screen
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @return
 */
void BEViewSetLocation(BEViewRef viewRef, int x, int y);

/**
 * @ingroup    BEView
 * @brief      set rectangle of web view in screen
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  width                   width
 * @param[in]  height                  height
 * @return     
 */
void BEViewSetRect(BEViewRef view, int x, int y, int width, int height);

/**
 * @ingroup    BEView
 * @brief      web view should be actived if drawing area type was notifier
 * @param[in]  view                    web view
 * @return     
 */
void BEViewSetActiveNotifierDrawingArea(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      get size of screen
 * @return     size
 */
BESize BEViewGetScreenSize();

/**
 * @ingroup    BEView
 * @brief      set focus to web view
 * @param[in]  view                    web view
 * @return     
 */
void BEViewSetFocus(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      get if web view has focus
 * @param[in]  view                    web view
 * @return     
 */
bool BEViewHasFocus(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      send key to web view
 * @param[in]  view                    web view
 * @param[in]  keycode                 key code
 * @param[in]  isPress                 press or release
 * @return     
 */
void BEViewSendKeyEvent(BEViewRef view, unsigned keycode, bool isPress);

#ifdef ENABLE_HBBTV
/**
 * @ingroup    BEView
 * @brief      send key to web view
 * @param[in]  view                    web view
 * @param[in]  keycode                 key code
 * @param[in]  isPress                 press or release
 * @return     
 */
void BEViewSendHbbTVKeyEvent(BEViewRef view, unsigned keycode, bool isPress);
#endif

/**
 * @ingroup    BEView
 * @brief      send key to web view
 * @param[in]  view                    web view
 * @param[in]  event                   javascript event object reference
 * @return     
 */
void BEViewSendEvent(BEViewRef view, NPObjectRef event);

/**
 * @ingroup    BEView
 * @brief      handle platform event (x11)
 * @param[in]  view                    web view
 * @param[in]  event                   platform event
 * @return     if it is success to handle event, return true. other case return false.
 */
bool BEViewHandleEvent(BEViewRef view, BEPlatformEventRef event);

typedef void (*BEViewNotifyDrawingCallback)(BEViewRef view, int x, int y, int width, int height, const void* clientInfo);

/**
 * @ingroup    BEView
 * @brief      register NotifyDrawing callback (browser ui view use only)
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @param[in]  clientInfo              parameter of callback
 * @return     
 */
void BEViewSetNotifyDrawingCallback(BEViewRef view, BEViewNotifyDrawingCallback callback, const void* clientInfo);

enum {
    BEDirectionNone = 0,
    BEDirectionUp,
    BEDirectionDown,
    BEDirectionLeft,
    BEDirectionRight
};
typedef uint32_t BEDirection;

/**
 * @ingroup    BEView
 * @brief      set focus rect painting
 * @param[in]  view                    web view
 * @param[in]  isPainting              painting information
 * @param[in]  scrollDirection         scroll direction if isPainting was 'true' and focus rect was not exist in drawing area
 * @return     if isPainting was 'true' and focus rect was not exist after scrolling, then return false.
 */
bool BEViewSetFocusRectPainting(BEViewRef view, bool isPainting, BEDirection scrollDirection);

/**
 * @ingroup    BEView
 * @brief      set focus to node that neareset from cursor position
 * @param[in]  view                    web view
 * @return     if it is success to focus node, return true. other case return false.
 */
bool BEViewSetFocusByCursorPosition(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      get node rect that node is nearest focusable from screen position
 * @param[in]  view                    web view
 * @param[in]  direction               direction
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @return     nearest node rect
 */
BEIntRect BEViewGetNearestFocusableNodeRect(BEViewRef view, BEDirection direction, int x, int y);

/**
 * @ingroup    BEView
 * @brief      get node rect that node is nearest focusable from screen position
 * @param[in]  view                    web view
 * @param[in]  command                 command
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @return     nearest node rect
 */
BEIntRect BEViewGetNearestFocusableNodeRectByGravityCursor(BEViewRef view, const char* command, int x, int y);

/**
 * @ingroup    BEView
 * @brief      take snapshot of web view
 * @param[in]  view                    web view
 * @return     
 */
void BEViewTakeSnapshot(BEViewRef view);

/**
 * @ingroup    BEView
 * @brief      show cursor
 * @param[in]  view                    web view
 * @return     
 */
void BEViewShowCursor(BEViewRef view = 0);

/**
 * @ingroup    BEView
 * @brief      hide cursor
 * @param[in]  view                    web view
 * @return     
 */
void BEViewHideCursor(BEViewRef view = 0);

/**
 * @ingroup    BEView
 * @brief      Reload cursor
 * @param[in]  view                    web view
 * @return
 */
void BEViewReloadCursor(BEViewRef view = 0);

/**
 * @ingroup    BEView
 * @brief      set none cursor
 * @param[in]  view                    web view
 * @return
 */
void BEViewSetNoneCursor(BEViewRef view = 0);

/**
 * @ingroup    BEView
 * @brief      clear cursor shape
 * @return
 */
void BEViewClearCursor();

/**
 * @ingroup    BEView
 * @brief      set cursor size(big or small)
 * @param[in]  isSmallCursor        cursor size
 * @return
 */
void BEViewSetCursorSize(bool isUseSmallCursor);

/**
 * @ingroup    BEView
 * @brief      set drag mode
 * @param[in]  isDragMode              drag information
 * @return
 */
void BEViewSetDragMode(bool isDragMode);

/**
 * @ingroup    BEView
 * @brief      send mouse left down event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam           if true, use x, y
 * @return     
 */
void BEViewSendMouseLeftDownEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse left up event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam           if true, use x, y
 * @return     
 */
void BEViewSendMouseLeftUpEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse right down event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam           if true, use x, y
 * @return
 */
void BEViewSendMouseRightDownEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse right up event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam           if true, use x, y
 * @return
 */
void BEViewSendMouseRightUpEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse left click event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam            if true, use x, y
 * @return     
 */
void BEViewSendMouseLeftClickEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse right click event to web view
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam            if true, use x, y
 * @return     
 */
void BEViewSendMouseRightClickEvent(BEViewRef view = 0, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse wheel event to web view
 * @param[in]  view                    web view
 * @param[in]  direction               direction
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isUseUserParam            if true, use x, y
 * @return
 */
void BEViewSendMouseWheelEvent(BEViewRef view = 0, BEDirection direction = BEDirectionNone, int x = 0, int y = 0, bool isUseUserParam = false);

/**
 * @ingroup    BEView
 * @brief      send mouse event state
 * @param[in]  enabled                 enabled
 * @return     
 */
void BEViewSetMouseEventState(bool enabled);

/**
 * @ingroup    BEView
 * @brief      send browser ui state
 * @param[in]  state                   state
 * @return     
 */
void BEViewSetBrowserUIState(int state);

/**
 * @ingroup    BEView
 * @brief      set cursor position
 * @param[in]  view                    web view
 * @param[in]  x                       x position
 * @param[in]  y                       y position
 * @param[in]  isWarpPointer           warp cursor position
 * @return     
 */
void BEViewSetCursorPosition(BEViewRef view, int x, int y, bool isWarpPointer);

/**
 * @ingroup    BEView
 * @brief      get cursor position
 * @return     cursor position
 */
BEPoint BEViewGetCursorPosition();

/**
 * @ingroup    BEView
 * @brief      Init PIP
 * @return
 */
void BEViewInitPIP(bool bPreloading = false);

/**
 * @ingroup    BEView
 * @brief      Set PIP Rect
 * @return
 */
void BEViewSetPIPRect(int x, int y, int width, int height);

/**
 * @ingroup    BEView
 * @brief      Set PIP Enable
 * @return
 */
void BEViewSetPIPEnable(bool enable);

/**
 * @ingroup    BEView
 * @brief      Get PIPEnabled
 * @return     PIPEnabled
 */
bool BEViewPIPEnabled();

/**
 * @ingroup    BEView
 * @brief      Scroll callback
 * @return
 */
void BEViewDidScroll();

/**
 * @ingroup    BEView
 * @brief      Delete all cookies
 * @return
 */
void BEViewDeleteCookies();

/**
 * @ingroup    BEView
 * @brief      Get Flash Full Screen
 * @return
 */
void* BEViewGetFlashFullScreen();

enum {
    BEStereoScopicNone = 0,
    BEStereoScopicSBS,
    BEStereoScopicTNB
};
typedef uint32_t BEStereoScopicMode;

/**
 * @ingroup    BEViewSetStereoScopicMode
 * @brief      Set stereo scopic mode
 * @param[in]  mode                    stereo scopic mode
 * @return     If it is success to set stereo scopic mode, return true. other case return false.
 */
bool BEViewSetStereoScopicMode(BEStereoScopicMode mode);

/**
 * @ingroup    BEViewSetStereoScopicSize
 * @brief      Set stereo scopic size
 * @param[in]  view                    web view
 * @param[in]  width                   stereo scopic width
 * @param[in]  height                  stereo scopic height
 * @return     If it is success to set stereo scopic size, return true. other case return false.
 */
bool BEViewSetStereoScopicSize(BEViewRef viewRef, int width, int height);

/**
 * @ingroup    BEViewSetStereoScopicBufferSize
 * @brief      Set stereo scopic buffer size
 * @param[in]  view                    web view
 * @param[in]  width                   stereo scopic buffer width
 * @param[in]  height                  stereo scopic buffer height
 * @return     If it is success to set stereo scopic buffer size, return true. other case return false.
 */
bool BEViewSetStereoScopicBufferSize(BEViewRef viewRef, int width, int height);

/**
 * @ingroup    BEViewWindowObject
 * @param[in]  view                    web view
 * @brief      Set stereo scope mode
 * @return     window NPObject
 */
NPObjectRef BEViewWindowObject(BEViewRef viewRef);

/**
 * @ingroup    BEView
 * @brief      is any HTML5 Video or Audio is using uniplayer?
 * @return     true if using uniplayer, no otherwise
 */
bool BEViewIsH5VPlaying();

/**
 * @ingroup    BEView
 * @brief      Pause any HTML5 Video or Audio if it uses uniplayer
 * @return     
 */
void BEViewPauseH5V();

/**
 * @ingroup    BEView
 * @brief      Resume any HTML5 Video or Audio if it uses uniplayer
 * @return     
 */
void BEViewResumeH5V();

/**
 * @ingroup    BEView
 * @brief      Stop any HTML5 Video or Audio if it uses uniplayer
 * @return     
 */
void BEViewStopH5V();

#ifdef __cplusplus
}
#endif

#endif /* BEView_h */
