/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 * @file        be.h
 * @brief       Declaration of be api
 * @author      Sangyong Park (sy302.park@samsung.com)
 * @date        2010.07.26
 */

#ifndef _BE_H
#define _BE_H

#include "npruntime.h"

#ifdef  __cplusplus
extern "C" {
#endif


#pragma GCC visibility push(default)

// Direction of tab move animation
typedef enum
{
    BeTabMoveDirectionNone,
    BeTabMoveDirectionLeft,
    BeTabMoveDirectionRight
} BETabMoveDirection;

// ScrollBar Mode
typedef enum
{
    BEScrollBarEnable,
    BEScrollBarDisable,
    BEScrollBarEnableOnScrolling
} BEScrollBarMode;

typedef enum
{
	BEPass,
	BEFail
} BEResult;

typedef enum
{
	BELeftBtn = 0x01,
	BEMiddleBtn = 0x02,
	BERightBtn = 0x03,
	BEWheelUp = 0x04,
	BEWheelDown = 0x05,
	BEWheelLeft = 0x06,
	BEWheelRight = 0x07,
} BEButton;

typedef enum  {
	BEIMEHide,
	BEIMEShow,
	BEIMEFlashShow
}BEIMEStatus;

typedef struct _BERect
{
	long left;
	long top;
	long width;
	long height;
} BERect;

typedef struct _BETime
{
	int nYear;		// Current Year
	int nMonth;		// 1~12
	int nDay;		// 1~31
	int nHour;		// 0~23
	int nMinute;	// 0~59
	int nSecond;	// 0~59
	int nWhatDay;	// 0~6 => SUN : 0 ...
} BETime;

typedef struct _BEMouseEvent
{
    bool isButtonDown;
    bool isWheelGranularityPage;
    char button;	// BEButton
    long x;
    long y;
    long screenX;
    long screenY;
} BEMouseEvent;

typedef enum
{
	BELeft		= 0x1,
	BERight		= 0x2,
	BETop		= 0x4,
	BEBottom	= 0x8,
	BECurrent	= 0x10
} BEDirection;

typedef enum
{
	BENoScroll,
	BEAlignCenter,
	BEAlignTop,
	BEAlignBottom,
	BEAlignLeft,
	BEAlignRight,
	BEAlignToClosestEdge
} BEScrollMode;

typedef enum
{
	BENavigationReasonLinkClicked,
	BENavigationReasonFormSubmitted,
	BENavigationReasonBackForward,
	BENavigationReasonReload,
	BENavigationReasonFormResubmitted,
	BENavigationReasonOther
} BENavigationReason;

typedef enum
{
	BENavigationIgnore,
	BENavigationUse,
	BENavigationDownload
} BENavigationPolicy;

typedef enum
{
	BESelectionPosHeadLeft = 0,
	BESelectionPosHeadTop,
	BESelectionPosTailRight,
	BESelectionPosTailBottom
} BESelectionPos;

typedef enum{
    BEFocusDirectionNone =0,
    BEFocusDirectionForward ,
    BEFocusDirectionBackward,
    BEFocusDirectionUp,
    BEFocusDirectionDown,
    BEFocusDirectionLeft,
    BEFocusDirectionRight,
    BEFocusDirectionJumpTop,
    BEFocusDirectionJumpBottom,
    BEFocusDirectionPageUp,
    BEFocusDirectionPageDown,
    BEFocusDirectionPageLeft,
    BEFocusDirectionPageRight,
    BEFocusDirectionLineUp,
    BEFocusDirectionLineDown,
    BEFocusDirectionMAX
}BEFocusDirection;

typedef void*	BEView;
typedef void*	BEDocument;
typedef void*	BENode;

typedef void (*BEBasicCallback) (void* data);
typedef BEView (*BECreateViewCallback) (void* data);
typedef BENavigationPolicy (*BENavigationPolicyCallback) (void* data, const char* url, BENavigationReason reason, bool isMainFrame);
typedef void (*BEBlockPopupCallback) (void* data, const char* url);

typedef void (*BELoadStatusCallback) (void* data, bool isMainFrame, bool isMainFrameLoading);
typedef char* (*BELoadErrorCallback) (void* data, const char* url, int code, int frameWidth, int frameHeight, const char* message, bool isMainFrame);
typedef void (*BEShowPopupMenuCallback) (void* data, void* menu, int xPos, int yPos, int width, int height, int selected);

typedef void (*BEScriptAlertCallback) (void* data, const char* message);
typedef bool (*BEScriptConfirmCallback) (void* data, const char* message);
typedef char* (*BEScriptPromptCallback) (void* data, const char* message, const char* defaultValue);
typedef void (*BEConsoleMessageCallback) (void* data, const char* message, int line, const char* source);

typedef void (*BEIMEFocusCallback) (void* data, bool isFocusIn);

#if 1 //ENABLE(TVGG_TRANSLATED_ALERT) /* @20110126-sanggyu */
typedef void (*BEScriptTranslatedAlertCallback) (void* data, const char* message);
#endif

/**
 * @ingroup    be
 * @brief      version information of be
 * @return     version information
 */
const char* BE_GetVersion(void);

/**
 * @ingroup    be
 * @brief      create web view for plugin
 * @param[in]  npp                     plugin instance handle
 * @return     new web view
 */
BEView BE_CreateView(NPP npp);

/**
 * @ingroup    be
 * @brief      destroy web view
 * @param[in]  view                    web view
 * @return
 */
void BE_DestroyView(BEView view);

/**
 * @ingroup    be
 * @brief      must be called in setwindow callback of plugin
 * @param[in]  view                    web view
 * @param[in]  window                  window information offered by plugin
 * @return     If view or window is null, then return BEFail. other case return BEPass.
 */
BEResult BE_SetWindow(BEView view, NPWindow* window);

/**
 * @ingroup    be
 * @brief      must be called in event callback of plugin
 * @param[in]  view                    web view
 * @param[in]  event                   event information offered by plugin
 * @return
 */
void BE_HandleEvent(BEView view, void* event, bool isWheelGranularityPage = false);

/**
 * @ingroup    be
 * @brief      initialize snapshot of view
 * @param[in]  view                    web view
 * @return
 */
void BE_InitSnapshot(BEView view);

/**
 * @ingroup    be
 * @brief      remove snapshot of view
 * @param[in]  view                    web view
 * @return
 */
void BE_RemoveSnapshot(BEView view);

/**
 * @ingroup    be
 * @brief      set snapshot ratio of view
 * @param[in]  view                    web view
 * @param[in]  ratio                   snapshot ratio
 * @return
 */
void BE_SetSnapshotRatio(BEView view, float ratio);

/**
 * @ingroup    be
 * @brief      draw web page to drawable on event
 * @param[in]  view                    web view
 * @param[in]  url                     url of web page
 * @param[in]  event                   event information offered by plugin
 * @param[in]  ratio                   ratio of web page if ratio is less than 0, do not change current ratio
 * @param[in]  offsetX                 offset in x axis
 * @param[in]  offsetY                 offset in y axis
 * @param[in]  backgroundColor         background color
 * @return     If it is success to draw page, then return BEPass. other case return BEFail.
 */
BEResult BE_DrawSnapshot(BEView view, const char* url, void* event, float ratio,
		float offsetX, float offsetY, unsigned int backgroundColor);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents make new web view
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetCreateViewCallback(BEView view, BECreateViewCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents close web view
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetCloseViewCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      update the rect of backing store
 * @param[int] width
 * @param[int] height
 * @return
 */
void BE_ResizeBackingStoreRect(int width, int height);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents navigate new resource
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetDecideNavigationPolicyCallback(BEView view, BENavigationPolicyCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when popup was blocked
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetBlockPopupCallback(BEView view, BEBlockPopupCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents received in first response
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetLoadCommittedCallback(BEView view, BELoadStatusCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents received all response
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetLoadFinishedCallback(BEView view, BELoadStatusCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when icon is loaded
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetIconLoadedCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when download begins
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetNotifyDownloadCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when download ends
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetDownloadEndCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents received each response
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetLoadProgressCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when fail to load web contents
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetLoadErrorCallback(BEView view, BELoadErrorCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web contents change title information
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetTitleChangedCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when web page need to show/hide poup menu
 * @param[in]  view                    web view
 * @param[in]  showCallback            show popup menu callback
 * @param[in]  hideCallback            hide popup menu callback
 * @return
 */
void BE_SetPopupMenuCallback(BEView view, BEShowPopupMenuCallback showCallback, BEBasicCallback hideCallback);

/**
 * @ingroup    be
 * @brief      set callback calling when javascript code call alert()
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetScriptAlertCallback(BEView view, BEScriptAlertCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when javascript code call confirm()
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetScriptConfirmCallback(BEView view, BEScriptConfirmCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when javascript code call prompt()
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetScriptPromptCallback(BEView view, BEScriptPromptCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when javascript code call console.log(), debug(), error(), trace()...
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetConsoleMessageCallback(BEView view, BEConsoleMessageCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when print requested in web page.
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetPrintRequestedCallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when focus in/out ime
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetIMEFocusCallback(BEView view, BEIMEFocusCallback callback);

/**
 * @ingroup    be
 * @brief      set callback calling when show ime
 * @param[in]  view                    web view
 * @param[in]  callback                callback
 * @return
 */
void BE_SetShowIMECallback(BEView view, BEBasicCallback callback);

/**
 * @ingroup    be
 * @brief      Notify current mouse point
 * @param[in]  nX
 * @param[in]  nY
 * @return
 */
void BE_NotifyMousePosChanged(int nX, int nY);

/**
 * @ingroup    be
 * @brief      get last mouse point
 * @param[in/out]  nX
 * @param[in/out]  nY
 * @return
 */
void BE_GetCurrentMousePos(int &nX, int &nY);

/**
 * @ingroup    be
 * @brief      set additional data using in callback
 * @param[in]  view                    web view
 * @param[in]  data                    additional data
 * @return
 */
void BE_SetCallbackData(BEView view, void* data);


/**
 * @ingroup    be
 * @brief      allow generating popup window
 * @param[in]  view                    web view
 * @param[in]  allow                   allow generating popup
 * @return
 */
void BE_AllowPopup(BEView view, bool allow);

/**
 * @ingroup    be
 * @brief      set default text encoding name
 * @param[in]  view                    web view
 * @param[in]  encoding                default text encoding name
 * @return
 */
void BE_SetDefaultTextEncodingName(BEView view, const char* encoding);

/**
 * @ingroup    be
 * @brief      change text encoding of web page and reload
 * @param[in]  view                    web view
 * @param[in]  encoding                text encoding name
 * @return
 */
void BE_SetCustomEncoding(BEView view, const char* encoding);

/**
 * @ingroup    be
 * @brief      use encoding detector that is detect encoding of contents
 *             if contents or HTTP header do not have encoding information.
 * @param[in]  view                    web view
 * @param[in]  use                     use encoding detector
 * @return
 */
void BE_UseEncodingDetector(BEView view, bool use);

/**
 * @ingroup    be
 * @brief      get text encoding of web page
 * @param[in]  view                    web view
 * @return     text encoding name of web page
 */
char* BE_GetEncodingName(BEView view);

/**
 * @ingroup    be
 * @brief      change text encoding of web page and reload
 * @param[in]  view                    web view
 * @param[in]  encoding                text encoding name
 * @return
 */
void BE_SetEncodingName(BEView view, const char* encoding);

/**
 * @ingroup    be
 * @brief      set accept-language property of http protocol
 * @param[in]  acceptLanguage          accept-language property
 * @return
 */
void BE_SetAcceptLanguage(const char* acceptLanguage);

/**
 * @ingroup    be
 * @brief      set font family of web view (standard, fixed, serif, sans-serif, cursive, fantasy)
 * @param[in]  fontfamily              font family name (utf-8 encode only)
 * @return
 */
void BE_SetFontFamily(BEView view, const char* fontfamily);

/**
 * @ingroup    be
 * @brief      set drawn element focus
 * @param[in]  isDraw                  is draw element focus
 * @return
 */
void BE_SetDrawElementFocus(bool isDraw);

/**
 * @ingroup    be
 * @brief      force draw web view
 * @param[in]  view                    web view
 * @return
 */
void BE_ForceDraw(BEView view);

/**
 * @ingroup    be
 * @brief      load url
 * @param[in]  view                    web view
 * @param[in]  url                     contents url
 * @return
 */
void BE_LoadURL(BEView view, const char* url);

/**
 * @ingroup    be
 * @brief      reload web view
 * @param[in]  view                    web view
 * @return
 */
void BE_Reload(BEView view);

/**
 * @ingroup    be
 * @brief      change widget
 * @param[in]  view                    web view
 * @return     If view or url is null, then return BEFail. other case return BEPass.
 */
BEResult BE_ChangeWidget(BEView view);

/**
 * @ingroup    be
 * @brief      Set tab move direction
 * @param[in]  tabMoveDirection        direction of tab move
 * @return     null
 */
void BE_SetTabMoveDirection(BETabMoveDirection tabMoveDirection);

/**
 * @ingroup    be
 * @brief      stop loading progress
 * @param[in]  view                    web view
 * @return     If view is null, then return BEFail. other case return BEPass.
 */
BEResult BE_StopLoading(BEView view);

/**
 * @ingroup    be
 * @brief      get loading progress
 * @param[in]  view                    web view
 * @return     progress value (0.0 <= value <= 1.0)
 */
float BE_GetLoadingProgress(BEView view);

/**
 * @ingroup    be
 * @brief      get title
 * @param[in]  view                    web view
 * @return     title string
 */
const char* BE_GetTitle(BEView view);

/**
 * @ingroup    be
 * @brief      get view size
 * @param[in]  view                    web view
 * @param[out] width                   view width
 * @param[out] height                  view height
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_GetViewRect(BEView view, int* width, int* height);

/**
 * @ingroup    be
 * @brief      get content size
 * @param[in]  view                    web view
 * @param[out] width                   content width
 * @param[out] height                  content height
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_GetContentSize(BEView view, int* width, int* height);

/**
 * @ingroup    be
 * @brief      get scroll position
 * @param[in]  view                    web view
 * @param[out] x                       x scroll position
 * @param[out] y                       y scroll position
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_GetScrollPos(BEView view, int* x, int* y);

/**
 * @ingroup    be
 * @brief      set scroll position
 * @param[in]  view                    web view
 * @param[in]  x                       x scroll position
 * @param[in]  y                       y scroll position
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_SetScrollPos(BEView view, int x, int y);

/**
 * @ingroup    be
 * @brief      set focus to web view
 * @param[in]  view                    web view
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_SetFocus(BEView view);

/**
 * @ingroup    be
 * @brief      get information that form has login value
 * @param[in]  view                    web view
 * @param[in]  form                    form
 * @return     If form has login value, then return BEPass. Other case return BEFail.
 */
BEResult BE_IsLoginForm(BENode node);

/**
 * @ingroup    be
 * @brief      select the content of the browser
 * @param[in]  view                    web view
 * @return     void
 */
void BE_SelectAllContent(BEView view);

/**
 * @ingroup    be
 * @brief      set login form
 * @param[in]  view                    web view
 * @param[in]  id                      id value
 * @param[in]  password                password value
 * @return     If it is success to set login form, then return BEPass. Other case return BEFail.
 */
BEResult BE_SetLoginForm(BEView view, const char* id, const char* password);

BEResult BE_CaptureWebPageToImageFile(BEView view, const char* filename);
BEResult BE_CaptureRectToImageFile(BEView view, const char* filename, BERect* rect);

/**
 * @ingroup    be
 * @brief      set snapshot storage size
 * @param[in]  size                    new snapshot storage size
 * @return     
 */
void BE_SetSnapshotStorageSize(unsigned int size);

/**
 * @ingroup    be
 * @brief      take snapshot
 * @param[in]  view                    web view
 * @param[in]  scale                   scale factor
 * @return     If view is null or scale has invalid range (scale < 0 or scale > 1), then return BEFail. Other case return BEPass.
 */
BEResult BE_TakeSnapshot(BEView view, float scale);

/**
 * @ingroup    be
 * @brief      take snapshot of parent view
 * @param[in]  view                    web view
 * @param[in]  scale                   scale factor
 * @return     If view is null or scale has invalid range, then return BEFail. Other case return BEPass.
 */
BEResult BE_TakeSnapshotOfParentView(BEView view, float scale);

/**
 * @ingroup    be
 * @brief      get snapshot url
 * @param[in]  url                     web page url
 * @param[in]  scale                   scale factor
 * @return     snapshot url
 */
char* BE_GetSnapshotURL(const char* url, float scale);

/**
 * @ingroup    be
 * @brief      send key event to web view
 * @param[in]  view                    web view
 * @param[in]  key                     key code
 * @param[in]  isPress                 press information
 * @return     If success to send key event, then return BEPass.
 */
BEResult BE_SendKeyEvent(BEView view, unsigned int key, bool isPress);

/**
 * @ingroup    be
 * @brief      send mouse event to display
 * @param[in]  isPress                 press information
 * @return
 */
void BE_SendMouseEvent(bool isPress);

/**
 * @ingroup    be
 * @brief      execute script script
 * @param[in]  view                    web view
 * @param[in]  script                  script code for executing
 * @return     If view or script is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_ExecuteScript(BEView view, const char* script);

/**
 * @ingroup    be
 * @brief      get zoom level of web view
 * @param[in]  view                    web view
 * @return     zoom level value (1.0 is 100%)
 */
float BE_GetZoomLevel(BEView view);

/**
 * @ingroup    be
 * @brief      set zoom level of web view
 * @param[in]  view                    web view
 * @param[in]  level                   zoom level
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_SetZoomLevel(BEView view, float level);

/**
 * @ingroup    be
 * @brief      get possibility whether web view can move by step in history
 * @param[in]  view                    web view
 * @param[in]  step                    0 is current page, negative is backward history, and positive is forward history.
 * @return     If view can move by stemp in history, then return BEPass. Other case return BEFail.
 */
BEResult BE_CanHistoryGo(BEView view, int steps);

/**
 * @ingroup    be
 * @brief      move web page in history
 * @param[in]  view                    web view
 * @param[in]  step                    0 is current page, negative is backward history, and positive is forward history.
 * @return     If view is null, then return BEFail. Other case return BEPass.
 */
BEResult BE_HistoryGo(BEView view, int steps);

/**
 * @ingroup    be
 * @brief      get main document in web view
 * @param[in]  view                    web view
 * @return     main document
 */
BEDocument BE_GetMainDocument(BEView view);

BEDocument BE_GetContentDocument(BENode node);

/**
 * @ingroup    be
 * @brief      get url of document
 * @param[in]  doc                     document
 * @return     url of document
 */
char* BE_GetURL(BEDocument doc);

/**
 * @ingroup    be
 * @brief      get base url of document
 * @param[in]  doc                     document
 * @return     base url of document
 */
char* BE_GetBaseURL(BEDocument doc);

/**
 * @ingroup    be
 * @brief      get focus node of view
 * @param[in]  view                    web view
 * @return     focus node
 */
BENode BE_GetFocusNode(BEView view);

/**
 * @ingroup    be
 * @brief      set focus on node
 * @param[in]  node                    focus node
 * @return
 */
void BE_SetFocusNode(BENode node);

/**
 * @ingroup    be
 * @brief      set blur on node
 * @param[in]  node                    blur node
 * @return
 */
void BE_SetBlurNode(BENode node);

/**
 * @ingroup    be
 * @brief      To return the fav icon for the website.
 * @param[in]  view                    web view
 * @return     If site is opened in webview returns it's favicon.
 */
const char* BE_GetFavicon(BEView view);

BENode BE_GetDocumentElement(BEDocument doc);
BENode BE_GetBody(BEDocument doc);
BENode BE_GetNodeByPoint(BEDocument doc, int x, int y);
BEResult BE_SelectWordAtMousePoint(BEView view, BEMouseEvent* aMouseEvent);
BEResult BE_CreateAndSelectRange(BEView view, BEMouseEvent* aMouseEvent, BEMouseEvent* bMouseEvent);
void BE_SetSelectionFromNone(BEDocument doc);
BENode BE_GetElementById(BEDocument doc, const char* id);

BENode BE_GetSpatialFocusableNode(BEView view, BEDirection dir);

BENode BE_GetParent(BENode node);
BENode BE_GetFirstChild(BENode node);
BENode BE_GetLastChild(BENode node);
BENode BE_GetPreviousSibling(BENode node);
BENode BE_GetNextSibling(BENode node);

/* NodeType

ELEMENT_NODE = 1,
ATTRIBUTE_NODE = 2,
TEXT_NODE = 3,
CDATA_SECTION_NODE = 4,
ENTITY_REFERENCE_NODE = 5,
ENTITY_NODE = 6,
PROCESSING_INSTRUCTION_NODE = 7,
COMMENT_NODE = 8,
DOCUMENT_NODE = 9,
DOCUMENT_TYPE_NODE = 10,
DOCUMENT_FRAGMENT_NODE = 11,
NOTATION_NODE = 12,
XPATH_NAMESPACE_NODE = 13
*/
int BE_GetNodeType(BENode node);
char* BE_GetNodeName(BENode node);
char* BE_GetNodeValue(BENode node);
char* BE_GetNodeInfoFromPoint(BEView view, int x, int y);

/**
 * @ingroup    be
 * @brief      get rectangle of node
 * @param[in]  node                    node
 * @return
 */
BERect BE_GetNodeRect(BENode node);

/**
 * @ingroup    be
 * @brief      get rectangle of node in absolute axis of web view
 * @param[in]  node                    node
 * @return
 */
BERect BE_GetAbsoluteNodeRect(BENode node);

char* BE_GetAttribute(BENode node, const char* name);
short BE_GetTabIndex(BENode node);
BENode* BE_GetElementsByTagName(BENode node, const char* tagname, unsigned* count);
char* BE_GetInnerText(BENode node);
char* BE_GetInnerHTML(BENode node);
char* BE_GetAbsoluteURL(BENode node, const char* url);
BEResult BE_Click(BENode node);
BEResult BE_IsVisible(BENode node);
BEResult BE_ScrollByNode(BENode node, int marginX, int marginY,
	BEScrollMode visibleX, BEScrollMode hiddenX, BEScrollMode partialX,
	BEScrollMode visibleY, BEScrollMode hiddenY, BEScrollMode partialY);

BEResult BE_ShowBlockedWebPage(BEView view, BERect* rectlist, unsigned count,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius);
BEResult BE_HideBlockedWebPage(BEView view);
BEResult BE_SetBlockFocus(BEView view, BERect rect, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, bool isDrawDimLayer, float transitionDuration);
BEResult BE_ChangeScale(BEView view, float rate, BENode node, int left, int top,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius, bool isDrawDimLayer);
//#ifdef SMART_FOCUS_ZOOM
BEResult BE_ZoomPage(BEView view, float rate, BENode node);
//#endif
BEResult BE_SetElementFocus(BEView view, BENode node, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, float transitionDuration);
BEResult BE_ShowString(BEView view, int id, const char* str, int left, int top,
	unsigned fontSize, const char* fontColor, unsigned verticalPadding,
	unsigned horizontalPadding, const char* backgroundColor, unsigned borderRadius);
BEResult BE_HideString(BEView view, int id);

/**
 * @ingroup    be
 * @brief      set cache on
 * @return     If success to set cache on, then return BEPass.
 */
BEResult BE_SetCacheAllOn(void);

/**
 * @ingroup    be
 * @brief      set cache off
 * @return     If success to set cache off, then return BEPass.
 */
BEResult BE_SetCacheAllOff(void);

/**
 * @ingroup    be
 * @brief      print cache status
 * @return     If success to print cache, then return BEPass.
 */
BEResult BE_ShowCacheStatus(void);

/**
 * @ingroup    be
 * @brief      set cache size
 * @param[in]  nCacheTotalCapacity     total capacity
 * @param[in]  nPageCacheCapacity      page capacity
 * @return     If success to set cache size, then return BEPass.
 */
BEResult BE_SetCacheSize(int nCacheTotalCapacity, unsigned int nPageCacheCapacity);

/**
 * @ingroup    be
 * @brief      release page cache immediatly
 * @param[in]  aNewCapacity
 * @return     If success, release page cache then return BEPass. otherwize return BEFail
 */
BEResult BE_ReleasePageCache(int aNewCapacity = 0);

/**
 * @ingroup    be
 * @brief      invalidate font cache
 * @return     If success, release page cache then return BEPass. otherwize return BEFail
 */
BEResult BE_InvalidateFontCache(void);

/**
 * @ingroup    be
 * @brief     prune cache
 * @return     If success, prune cache then return BEPass. otherwize return BEFail
 */
BEResult BE_PruneCache(void);

/**
 * @ingroup    be
 * @brief      delete cookies
 * @return
 */
void BE_DeleteAllCookies(void);

/**
 * @ingroup    be
 * @brief      set cookie cache off
 * @param[in]  bEnable                 cookie cache
 * @return     If success to set cookie cache, then return BEPass.
 */
bool BE_CookieCacheOff(bool bEnable);

BEResult BE_LoadSoupCache(void);
BEResult BE_DumpSoupCache(void);
BEResult BE_FlushSoupCache(void);
BEResult BE_ClearSoupCache(void);
BEResult BE_SetMaxSizeOfSoupCache(unsigned int size);
unsigned int BE_GetMaxSizeOfSoupCache(void);
void BE_SetCacheOutOfSpaceCallback(BEBasicCallback callback);



/**
 * @ingroup    be
 * @brief      reset previous document
 * @param[in]  view                    web view
 * @return     If success to reset document, then return BEPass.
 */
BEResult BE_ResetPreDocument(BEView view);

/**
 * @ingroup    be
 * @brief      reset move focus
 * @param[in]  view                    web view
 * @return     If success to reset move focus, then return BEPass.
 */
BEResult BE_ResetMoveFocus(BEView view);

/**
 * @ingroup    be
 * @brief      BE_SetSpElementFocus
 * @param[in]  view                    web view
 * @param[in]  node                    BEnode
 * @param[in]  arrowDir                focus arrowDir
 * @param[in]  borderWidth             focus border width
 * @param[in]  borderColor             focus border color
 * @param[in]  borderRadius            focus borderrRadius
 * @param[in]  arrowWidth        	   focus arrow width
 * @param[in]  arrowMargin             focus arrow margin
 * @param[in]  transitionDuration      focus transitionDuration
 * @return     If success to move focus, then return BEPass; otherwise return BEFail
 */
BEResult BE_SetSpElementFocus(BEView view, BENode node, unsigned arrowDir,
	unsigned borderWidth, const char* borderColor, unsigned borderRadius,
	unsigned arrowWidth, unsigned arrowMargin, float transitionDuration);

/**
 * @ingroup    be
 * @brief      set base time
 * @param[in]  nBaseTime               base time
 * @return
 */
void BE_SetBaseTime(unsigned int nBaseTime);

/**
 * @ingroup    be
 * @brief      get current time
 * @return     current time
 */
BETime BE_GetCurrentTime(void);

/**
 * @ingroup    be
 * @brief      get information if node is text form
 * @param[in]  node                    node
 * @return     If node is text form, then return true.
 */
bool BE_IsTextFormType(BENode node);

/**
 * @ingroup    be
 * @brief      get type of input element if node is input element
 * @param[in]  node                    node
 * @return     type of input element
 */
char* BE_GetInputType(BENode node);

/**
 * @ingroup    be
 * @brief      get value of input element if node is input element
 * @param[in]  node                    node
 * @return     value of input element
 */
char* BE_GetInputValue(BENode node);

/**
 * @ingroup    be
 * @brief      set value of input element if node is input element
 * @param[in]  node                    node
 * @param[in]  value                   value
 * @return     value of input element
 */
void BE_SetInputValue(BENode node, const char* value);

/**
 * @ingroup    be
 * @brief      set the position of selected bound
 * @param[in]  node                    node
 * @param[in]  startPos                startPos
 * @param[in]  endPos                  endPos
 * @return
 */
void BE_SetInputSelection(BENode node, int startPos, int endPos);

/**
 * @ingroup    be
 * @brief      get the position of selected region
 * @param[in]  view                    web view
 * @param[in]  pos                 specific position of selection rectangle
 * @return     position value of specific position selected.
 */
int BE_SelectionPosAt(BEView view, BESelectionPos pos);

/**
 * @ingroup    be
 * @brief      get the position of selected bound
 * @param[in]  view                    web view
 * @param[in]  pos                 specific position of selection rectangle
 * @return     position value of specific position of selected bound.
 */
int BE_SelectionBoundPosAt(BEView view, BEDirection pos);

/**
 * @ingroup    be
 * @brief      get the position of selected region
 * @param[in]  view                    web view
 * @return     If success to send key event, then return BEPass.
 */
bool BE_DoesSelectionExist(BEView view);

/**
 * @ingroup    be
 * @brief      Gets the text from the current selection.
 * @param[in]  view                    web view
 * @return     char*
 */
char* BE_GetSelectionText(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selection contains only text or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_IsSelectionTextOnly(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selection contains only text or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
BEResult BE_CopyToClipboard(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selection contains only text or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
BEResult BE_PasteFromClipboard(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can copy to clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_CanCopyClipboard(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_CanPasteClipboard(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
int BE_FocusedNodeRectPosAt(BEView view, BEDirection pos);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_IsFocusedNodeTextForm(BEView view);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_SendMouseMotionEventForce(BEView view, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
bool BE_SendMouseButtonEventForce(BEView view, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
BEResult BE_ExtendSelectionToLeft(BEView view, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Check whether the selected region can paste from clipboard or not.
 * @param[in]  view                    web view
 * @return     If the selection contains only text, then return true.
 */
BEResult BE_ExtendSelectionToRight(BEView view, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Check whether element is flash  or not.
 * @param[in]  view                    web view
 * @return     If node is flash , then return ture.
 */
bool BE_IsFlashElement(BENode node);

/**
 * @ingroup    be
 * @brief      start download if has download request
 * @param[in]  view                    web view
 * @return
 */
void BE_StartDownload(BEView view);

/**
  * @ingroup	be
 * @brief      Send GTK mouse motion  to event queue
 * @param[in]  view                    web view
 * @return     If node is flash , then return ture.
 */
bool BE_SendGtkMouseMotionEvent(NPP npp, BEMouseEvent* aMouseEvent, bool isWithSmoothCursor, bool doWarpCursor = false);

/**
 * @ingroup    be
 * @brief      Check whether element is flash  or not.
 * @param[in]  view                    web view
 * @return     If node is flash , then return ture.
 */
bool BE_SendGtkMouseButtonPressEvent(NPP npp, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Check whether element is flash  or not.
 * @param[in]  view                    web view
 * @return     If node is flash , then return ture.
 */
bool BE_SendGtkMouseButtonReleaseEvent(NPP npp, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      Hide Cursor
 * @param[in]  npp                   NPP
 * @return     none
 */
bool BE_HideCursor(NPP npp);

/**
 * @ingroup    be
 * @brief      Show Cursor
 * @param[in]  npp                   NPP
 * @return     none
 */
bool BE_ShowCursor(NPP npp, BEMouseEvent* aMouseEvent);

/**
 * @ingroup    be
 * @brief      get list size of popup menu
 * @param[in]  menu                  instance of popup menu
 * @return     list size
 */
int BE_PopupMenu_GetListSize(void* menu);

/**
 * @ingroup    be
 * @brief      is item label?
 * @param[in]  menu                  instance of popup menu
 * @param[in]  index                 index of item in menu
 * @return     If item is enabled, then return true.
 */
bool BE_PopupMenu_ItemIsLabel(void* menu, int index);

/**
 * @ingroup    be
 * @brief      is item seperator?
 * @param[in]  menu                  instance of popup menu
 * @param[in]  index                 index of item in menu
 * @return     If item is seperator, then return true.
 */
bool BE_PopupMenu_ItemIsSeperator(void* menu, int index);

/**
 * @ingroup    be
 * @brief      is item enabled?
 * @param[in]  menu                  instance of popup menu
 * @param[in]  index                 index of item in menu
 * @return     If item is enabled, then return true.
 */
bool BE_PopupMenu_ItemIsEnabled(void* menu, int index);

/**
 * @ingroup    be
 * @brief      get item text
 * @param[in]  menu                  instance of popup menu
 * @param[in]  index                 index of item in menu
 * @return     item text
 */
char* BE_PopupMenu_GetItemText(void* menu, int index);

/**
 * @ingroup    be
 * @brief      select item
 * @param[in]  menu                  instance of popup menu
 * @param[in]  index                 index of item in menu
 * @return
 */
void BE_PopupMenu_SelectItem(void* menu, int index);

/**
 * @ingroup    be
 * @brief      popup menu did hide
 * @param[in]  menu                  instance of popup menu
 * @return
 */
void BE_PopupMenu_DidHide(void* menu);


/**
 * @ingroup    be
 * @brief      check video element
 * @param[in]  node	node
 * @return
 */
bool BE_IsVideoElement(BENode node);

/**
 * @ingroup    be
 * @brief      Set rect of vertical scrollbar
 * @param[in]  view web view
 * @param[in]  x		x of vertical scrollbar
 * @param[in]  y		y of vertical scrollbar
 * @param[in]  width	width of vertical scrollbar
 * @param[in]  height	height of vertical scrollbar
 * @return
 */
BEResult BE_SetVScrollBarRect(BEView view, int x, int y, int width, int height);

/**
 * @ingroup    be
 * @brief      Set rect of horizon scrollbar
 * @param[in]  view		web view
 * @param[in]  x		x of vertical scrollbar
 * @param[in]  y		y of vertical scrollbar
 * @param[in]  width	width of vertical scrollbar
 * @param[in]  height	height of vertical scrollbar
 * @return
 */
BEResult BE_SetHScrollBarRect(BEView view, int x, int y, int width, int height);

/**
 * @ingroup    be
 * @brief      Set color of scrollbar
 * @param[in]  view		web view
 * @param[in]  red		red of scrollbar color
 * @param[in]  green	green of scrollbar color
 * @param[in]  blue		blue of scrollbar color
 * @param[in]  alpha	alpha of scrollbar color
 * @return
 */
BEResult BE_SetScrollBarColor(BEView view, int red, int green, int blue, int alpha);

/**
 * @ingroup    be
 * @brief      Set color of scrollbar thumb
 * @param[in]  view		web view
 * @param[in]  red		red of scrollbar thumb color
 * @param[in]  green	green of scrollbar thumb color
 * @param[in]  blue		blue of scrollbar thumb color
 * @param[in]  alpha	alpha of scrollbar thumb color
 * @return
 */
BEResult BE_SetScrollBarThumbColor(BEView view, int red, int green, int blue, int alpha);

/**
 * @ingroup    be
 * @brief      Set scrollbar mode
 * @param[in]  view		web view
 * @param[in]  mode		scroll bar mode
 * @return
 */
BEResult BE_SetScrollBarMode(BEView view, BEScrollBarMode mode);

/**
 * @ingroup    be
 * @brief      Set pipEnabled
 * @param[in]  view		web view
 * @param[in]  isPipEnabled
 * @return
 */
BEResult BE_SetPipEnabled(BEView view, bool isPipEnabled);

/**
 * @ingroup    be
 * @brief      Set rect of pip
 * @param[in]  view		web view
 * @param[in]  x		x of pip
 * @param[in]  y		y of pip
 * @param[in]  width	width of pip
 * @param[in]  height	height of pip
 * @return
 */
BEResult BE_SetPipRect(BEView view, int x, int y, int w, int h);

/**
 * @ingroup    be
 * @brief      Get pipEnabled
 * @param[in]  view		web view
 * @param[in]  pPipEnabled
 * @return
 */
BEResult BE_GetPipEnabled(BEView view, bool *pPipEnabled);

/**
 * @ingroup    be
 * @brief      Set Audio window
 * @param[in]  view		web view
 * @param[in]  isMainWindow
 * @return
 */
BEResult BE_SetAudioWindow(BEView view, bool isMainWindow);

/**
 * @ingroup    be
 * @brief      Get Audio window
 * @param[in]  pIsMainWindow
 * @return
 */
void BE_GetAudioWindow(int* pAudioWindow);

/**
 * @ingroup    be
 * @brief      Get rect of pip
 * @param[in]  view		web view
 * @param[in]  *px		x of pip
 * @param[in]  *py		y of pip
 * @param[in]  *pw		width of pip
 * @param[in]  *ph		height of pip
 * @return
 */
BEResult BE_GetPipRect(BEView view, int *px, int *py, int *pw, int *ph);

/**
 * @ingroup    be
 * @brief      is HTML5 Video is playing?
 * @param[in]  view		web view
 * @return     true if any HTML5 video is playing
 */
BEResult BE_IsH5VPlaying(BEView view);

/**
 * @ingroup    be
 * @brief      Pause current H5V
 * @return
 */
void BE_PauseH5V();

/**
 * @ingroup    be
 * @brief      Resume current H5V
 * @return
 */
void BE_ResumeH5V();

/**
 * @ingroup    be
 * @brief      Stop all the HTML5 Videos
 * @return
 */
void BE_StopH5V();

/**
 * @ingroup    be
 * @brief      Get DOM Window Object
 * @param[in]  view		web viekw
 * @return
 */
void* BE_GetWindow(BEView view);

/**
 * @ingroup    be
 * @brief    after searching text, highlight mark searched texts
 * @param[in]  view WebView
 * @param[in]  string string
 * @param[in]  bCaseSensitivive  CaseSensitive or not
 * @param[in]  unLimit  maximu nuber to sear text
 * @return
 */
bool BE_MarkSearchedTexts(BEView view,const char* string, bool bCaseSensitive, unsigned int unLimit);

/**
 * @ingroup    be
 * @brief      unmark all searched texts
 * @param[in]  view	WebView
 * @return
 */
void BE_UnMarkSearchedTexts(BEView view);

/**
 * @ingroup    be
 * @brief      navigate mark searched texts
 * @param[in]  view WebView
 * @param[in]  string string
 * @param[in]  bCaseSensitivive  CaseSensitive or not
 * @param[in]  bForward forward or backward
 * @return
 */
bool BE_NavigateMarkSearchedTexts( BEView view,const char* string, bool bCaseSensitive, bool bForward);

/**
 * @ingroup    be
 * @brief      Get text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
char* BE_FocusedNodeText(BEView view);

/**
 * @ingroup    be
 * @brief      Get link text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
char* BE_FocusedNodeLink(BEView view);


/**
 * @ingroup    be
 * @brief      Get link text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
void* BE_CreateLoadingAnimation(void);


/**
 * @ingroup    be
 * @brief      Get link text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
void BE_DestroyLoadingAnimation(void);


/**
 * @ingroup    be
 * @brief      Get link text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
bool BE_ShouldDestroyLoadingAnimation(void);


/**
 * @ingroup    be
 * @brief      Get link text value focused
 * @param[in]  view WebView
 * @return     Text value string
 */
void BE_FinalizeLoadingAnimation(void);


/**
 * @ingroup    be
 * @brief      check mouseover element
 * @param[in]  node BENode
 * @return
 */
bool BE_IsMouseOverElement(BENode node);

/**
 * @ingroup    be
 * @brief      wait and process other event
 * @param[in]  isWait   flag for wait or restart
 * @return
 */
void BE_Wait(bool isWait);

/**
 * @ingroup    be
 * @brief      Show keypad in Parentview
 * @param[in]  node BENode
 * @return
 */
void BE_ShowIME(BENode node);

/**
 * @ingroup    be
 * @brief      Show keypad from flash object
 * @return
 */
void BE_ShowIMEFromFlash();

/**
 * @ingroup    be
 * @brief      Hide keypad in Parentview
 * @return
 */
void BE_HideIME();

/**
 * @ingroup    be
 * @brief      Check focused node of ime is changed
 * @param[in]  node BENode
 * @return
 */
bool BE_IsFocusedIME(BENode node);

/**
 * @ingroup    be
 * @brief      hide keypad in Parentview
 * @return
 */
BEIMEStatus BE_GetIMEStatus();

/**
 * @ingroup    be
 * @brief      Check mouse event is allowed.
 * @return
 */
bool BE_IsMouseEventAllowed();


/**
 * @ingroup    be
 * @brief      Set/Get Pointer Speed => 10(very slow), 20(slow), 30(normal), 40(fast), 50(very fast)
 * @return
 */
void BE_SetPointerSpeed(int nPointerSpeed);
int BE_GetPointerSpeed();

void BE_SetContentViewFocusState(bool bFocused);
bool BE_IsContentViewFocused();


/**
 * @ingroup    be
 * @brief      Flush view
 * @param[in]  view   webview
 * @return
 */
void BE_FlushView(BEView view);

BEResult BE_GarbageCollectSoon();

/**
 * @ingroup    be
 * @brief      pause/resume view
 * @param[in]  view                    web view
 * @param[in]  isPause                 set pause / resume
 * @return
 */
void BE_SetPauseView(BEView view, bool isPause);

bool BE_IsPluginDebug();
#define BE_Debug(msg...) do{if(BE_IsPluginDebug())printf(msg);}while(0)
#define BE_FDebug(out, msg...) do{if(BE_IsPluginDebug())fprintf(out, msg);}while(0)
#define BE_DEBUG_MODULE_JS	0
#define BE_DEBUG_MODULE_PL	1
void BE_SetDebug(int module, int level);

#if 1 //ENABLE(TVGG_TRANSLATED_ALERT)
/**
 * @ingroup    be
 * @brief      Set translated alert callback
 * @param[in]  view       webview
 * @param[in]  callback   callback
 * @return
 */
void BE_SetScriptTranslatedAlertCallback(BEView view, BEScriptTranslatedAlertCallback callback);
#endif

#pragma GCC visibility pop

#ifdef  __cplusplus
}
#endif

#endif
