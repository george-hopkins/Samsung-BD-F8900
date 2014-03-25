/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.

 * @file       SpatialNaviEx.cpp
 * @brief        Implementation of SpatialNaviEx api
 * @autor       HyunGon Yang
 * @date        2010.06.03
 */

#include "config.h"
#include "FrameView.h"
#include "webkit/webkit.h"
#include "webkitwebviewprivate.h"
#include "SpatialNaviNew.h"
#include "be.h"

typedef struct _BERealView
{
	WebKitWebView* webview;
} BERealView;

static Result MoveFocus(SpWebView ContentView ,eFocusDirection direction, Result InRetVal);
static Result SPNavigate(eFocusDirection direction);
static Result SPJumpFocus(eFocusDirection direction);

Result
BE_SPNavigatorHandler(SpWebView ContentView,eFocusDirection direction,bool bParentView)
{

	Result retVal = E_FAIL;
	BE_CreateSPNavigator( ContentView,bParentView);
	if(direction < FocusDirection_JumpTop)
	{
		retVal = SPNavigate( direction);
		if(retVal == S_OK || retVal == S_JUMP_TO_OTHER_VIEW)
		{
			retVal = MoveFocus(ContentView,direction,retVal);
		}
	}
	else
	{
		retVal = SPJumpFocus(direction);
	}
	BE_DestroySPNavigator();

	return retVal;
}
#ifdef SMART_FOCUS_ZOOM
void BE_SPNavigatorSmartFocusHandler( SpWebView pContentsView, int x, int y)
{
    BE_CreateSPNavigator( pContentsView,false);
    WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
    if(!pSpNavigator)
    {
	printf("[%s :%d] << Spatial Navigation GetSpatialNavigator Fail !!\n", __func__, __LINE__);
	return;
    }
    pSpNavigator->SetFocusOnNode(pContentsView, x, y);
    BE_DestroySPNavigator();
    return;
}

bool BE_SetSmartFocusX(SpWebView pContentsView, int x, int y) {

    BE_CreateSPNavigator( pContentsView,false);
    WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
    if(!pSpNavigator)
    {
	    printf("[%s :%d] << Spatial Navigation GetSpatialNavigator Fail !!\n", __func__, __LINE__);
	    return false;
    }
    pSpNavigator->SetSmartFocusX(pContentsView, x, y);
    BE_DestroySPNavigator();
    return true;
}

void BE_SPNavigatorSmartZoomHandler( SpWebView pContentsView, int x, int y)
{

    printf("********************************[%s :%d] !\n", __func__, __LINE__);
    BE_CreateSPNavigator( pContentsView,false);
    WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
    if(!pSpNavigator)
    {
	SYS_LOG("[%s :%d] << Spatial Navigation GetSpatialNavigator Fail !!\n", __func__, __LINE__);
	return;
    }
    pSpNavigator->SetSmartZoom(pContentsView, x, y);
    BE_DestroySPNavigator();
    return;
}
#endif
Result
BE_SPResetPreNode(SpWebView ContentView )
{
	BE_CreateSPNavigator( ContentView, false);
	WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
	if(!pSpNavigator)
	{
		SYS_LOG(" << Spatial Navigation GetSpatialNavigator Fail !!\n");
		return E_FAIL;
	}
	pSpNavigator->SetNodeInfo(NULL);
	BE_DestroySPNavigator();
	return S_OK;
}

Result
BE_CreateSPNavigator(SpWebView ContentView,bool bParentView)
{
	Result retVal = E_FAIL;
//	WebKitWebView* pContentView = WEBKIT_WEB_VIEW(ContentView);
	WebKitWebView* pContentView = NULL;
	if(bParentView)
	{
		pContentView = WEBKIT_WEB_VIEW(((BERealView*)ContentView));
	}
	else
	{
		pContentView = WEBKIT_WEB_VIEW(((BERealView*)ContentView)->webview);
	}
	WebCore::Page* pPage = WebKit::core(pContentView);
    	if (!pPage)
    	{
        	return E_FAIL;
    	}

	 retVal = WebCore::SpatialNavigator::CreateInstance(pPage, pContentView);
	return retVal;
}


Result
BE_DestroySPNavigator(void)
{
	Result retVal = E_FAIL;
	retVal =WebCore::SpatialNavigator::DestoryInstance();
	return retVal;
}

Result
SPNavigate(eFocusDirection direction)
{
	Result retVal= E_FAIL;
	WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
	if(!pSpNavigator)
	{
		SYS_LOG("<< Spatial Navigation GetSpatialNavigator Fail !!\n");
		return retVal;
	}
	retVal = pSpNavigator->SpatialFocus(direction);
	if (retVal == E_FAIL)
    	{
        	SYS_LOG(" << Spatial Navigation SpatialFocus Fail !! \n");
    	}
	else if(retVal ==S_JUMP_TO_OTHER_VIEW )
	{
		SYS_LOG("<< BE_SPNavigatorHandler Jump To Other View\n");
	}
	else
	{
		printf("[%s][%d]<< Spatial Navigation SPNavigate Success !! \n",__func__,__LINE__);
	}

	return retVal;
}

Result
SPJumpFocus(eFocusDirection direction)
{
	Result retVal= E_FAIL;
	WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
	if(!pSpNavigator)
	{
		SYS_LOG("<< Spatial Navigation GetSpatialNavigator Fail !!\n");
		return retVal;
	}
	retVal=pSpNavigator->JumpFocus(direction);
	if (retVal == E_FAIL)
    	{
        	SYS_LOG("<<Spatial Navigation SPJumpFocus Fail !! \n");
   	}

	//printf("<< Spatial Navigation SPJumpFocus Success !! \n");
	return retVal;
}

SpNode
BE_SPGetNextNode(eFocusDirection direction)
{

	Result retVal = E_FAIL;
	retVal = SPNavigate( direction);
	SpNode pNode = NULL;
	if(retVal  == S_OK)
	{
		WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
		if(!pSpNavigator)
		{
			SYS_LOG("<< Spatial Navigation GetSpatialNavigator Fail !!\n");
		}
		else
		{
			pNode = pSpNavigator->GetNextNode();
			if(!pNode)
			{
				SYS_LOG("<< Spatial Navigation GetNextNode Fail !!\n");
			}

		}
	}
	return pNode;
}


SpDoc
BE_GetFocusedDoc(void)
{
	WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
	if(!pSpNavigator)
	{
		SYS_LOG(" << Spatial Navigation GetSpatialNavigator Fail !!\n");
		return false;
	}

	WebCore::Document* pFocusedDoc  = pSpNavigator->GetFocusedDoc();

	return pFocusedDoc;
}

static Result
MoveFocus(SpWebView ContentView ,eFocusDirection direction, Result InRetVal)
{
   	BEResult retVal=BEFail;
	BENode pNode = NULL;

	WebCore::SpatialNavigator* pSpNavigator =WebCore::SpatialNavigator::GetSpatialNavigator();
	if(!pSpNavigator)
	{
		SYS_LOG(" << Spatial Navigation GetSpatialNavigator Fail !!\n");
		return E_FAIL;
	}

	if(direction == FocusDirection_None)
	{
		pNode = pSpNavigator->GetCurrentFocusedNode();
		/*
		if(pNode)
		{
			BE_ScrollByNode(pNode, -4, -4,
			BEAlignCenter, BEAlignCenter, BEAlignCenter,BEAlignCenter, BEAlignCenter,BEAlignCenter);
		}
		*/
	}
	else
	{
		pNode = pSpNavigator->GetNextNode();
	}
	if(!pNode && InRetVal == S_OK)
	{
		SYS_LOG("<< we will stop focusing!!\n");
		return E_FAIL;
	}
	else if(!pNode && InRetVal == S_JUMP_TO_OTHER_VIEW)
	{
		SYS_LOG("<< Spatial Navigation GetNextNode Fail ,so  We will remove the last focus!!!!\n");
		return S_JUMP_TO_OTHER_VIEW;
	}
	else
	{
		;
	}

	switch(direction)
	{
		case FocusDirection_Left:
   		retVal=BE_SetSpElementFocus( ContentView,pNode, BELeft,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
		break;

		case FocusDirection_Right:
   		retVal=BE_SetSpElementFocus(  ContentView,pNode, BERight,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
		break;

		case FocusDirection_Up:
   		retVal=BE_SetSpElementFocus(  ContentView,pNode, BETop,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
		break;

		case FocusDirection_Down:
   		retVal=BE_SetSpElementFocus(  ContentView,pNode, BEBottom,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
		break;

		case FocusDirection_None:
		retVal=BE_SetSpElementFocus(  ContentView,pNode, BECurrent,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
		break;

		default:
			retVal = BEFail;
	}

	if(retVal == BEFail){
		SYS_LOG(" Fail==============================<<[%d]  !!\n",retVal);
		return E_FAIL;
	}
	return S_OK;
}
