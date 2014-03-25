/** 
 * Copyright 2010 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.

 * @file       	SpatialNaviNewEx.h
 * @brief       Declaration of SpatialNaviNewEx api
 * @author      HyunGon Yang  
 * @date        2010.07.26
 */

#ifndef _SPATIAL_NAVINEW_EX
#define _SPATIAL_NAVINEW_EX

#define SMART_FOCUS_ZOOM

#ifdef  __cplusplus
extern "C" {
#endif

#pragma GCC visibility push(default)


typedef void* SpWebView;
typedef void* SpNode;
typedef void* SpDoc;
typedef void* SpNewRect;

typedef enum{
    FocusDirection_None =0,	
    FocusDirection_Forward ,
    FocusDirection_Backward,
    FocusDirection_Up,
    FocusDirection_Down,
    FocusDirection_Left,
    FocusDirection_Right,
    FocusDirection_JumpTop,
    FocusDirection_JumpBottom,
    FocusDirection_PageUp,
    FocusDirection_PageDown,
    FocusDirection_PageLeft,    
    FocusDirection_PageRight,
    FocusDirection_LineUp,    
    FocusDirection_LineDown,    
    FocusDirectionMAX
}eFocusDirection;

typedef enum{
	E_FAIL=0,
	S_OK, 
	S_JUMP_TO_OTHER_VIEW	
}Result;

 /**
  * @ingroup	SpatialNavigatorEx
  * @brief		This method is used to hanlde the spatial navigation.
  * @param[in]	pContentsView  webkit webview
  * @param[in]	Direction 		   navigation direction
  * @return		This function returns S_OK or S_JUMP_TO_OTHER_VIEW if spatial navigaotr is operating normaly; otherwise returns E_FAIL 	 
  */	
Result BE_SPNavigatorHandler( SpWebView pContentsView,eFocusDirection Direction,bool bParentView);
#ifdef SMART_FOCUS_ZOOM
void BE_SPNavigatorSmartFocusHandler( SpWebView pContentsView, int x, int y);
void BE_SPNavigatorSmartZoomHandler( SpWebView pContentsView, int x, int y);
#endif
  /**
  * @ingroup	SpatialNavigatorEx
  * @brief		This method is used to create the spatial navigator.
  * @param[in]	ContentView  webkit webview
  * @return		This function returns S_OK if spatial navigaotr is creaded;otherwise returns E_FAIL 	 
  */	
Result BE_CreateSPNavigator(SpWebView ContentView,bool bParentView);
  
/**
* @ingroup	SpatialNavigatorEx
* @brief		This method is used to destory the spatial navigator.
* @return	This function returns S_OK if spatial navigaotr is destoryed;otherwise returns E_FAIL 	 
*/	  
Result BE_DestroySPNavigator(void);

/**
 * @ingroup         be
 * @brief           set the smart focus
 * @param[BEView]   view                    web view
 * @param[int]      xCoord                  
 * @param[int]      yCoord                  
 * @return     
 */
bool BE_SetSmartFocusX(SpWebView contentView, int x, int y);

 /**
  * @ingroup		SpatialNavigatorEx
  * @brief		This method is used to get next focusable node through the spatial navigator.
  * @param[in]	Direction  	navigation direction
  * @return		This function returns SpNode or Null
  */	
SpNode BE_SPGetNextNode(eFocusDirection Direction);

 
 /**
  * @ingroup		SpatialNavigatorEx
  * @brief		This method is used to get focused document through the spatial navigator.
  * @return		This function returns SpDoc or Null
  */
SpDoc BE_GetFocusedDoc(void);

  /**
  * @ingroup		SpatialNavigatorEx
  * @brief		This method is used to reset the pre focused node information through the spatial navigator.
  * @param[in]	ContentView  webkit webview
  * @return		This function returns S_OK if spatial navigaotr is destoryed;otherwise returns E_FAIL 	 
  */
Result BE_SPResetPreNode(SpWebView ContentView) ;

#pragma GCC visibility pop

#ifdef  __cplusplus
}
#endif


#endif
