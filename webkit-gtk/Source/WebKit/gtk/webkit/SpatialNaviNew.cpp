
/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.

 * @file       SpatialNavi.cpp
 * @brief       Implementation of SpatialNavigation class
 * @autor       HyunGon Yang
 * @date        2010.06.03
 */


#include "config.h"
#include "CString.h"
#include "Document.h"
#include "Frame.h"
#include "FrameView.h"
#include "HTMLFrameOwnerElement.h"
#include "HTMLNames.h"
#include "Page.h"
#include "RenderObject.h"
#include "RenderView.h"
#include "RenderLayer.h"
#include "HitTestRequest.h"
#include "HitTestResult.h"
#include "EventHandler.h"
#include "EventNames.h"
#include "MouseEventWithHitTestResults.h"
#include "Chrome.h"
#include "ClientRect.h"
#include "InputElement.h"
#include "HTMLInputElement.h"
#include "HTMLAreaElement.h"

#include "JSNode.h"
#include "JSGlobalData.h"
#include "JSFunction.h"
#include "FunctionConstructor.h"

#include "webkit/webkit.h"
#include "webkitwebviewprivate.h"
#include "webkitwebframeprivate.h"
#include "SpatialNaviNew.h"
#include "be.h"

#if ENABLE(TVGG_BACKINGSTORE)
#include "BackingStoreGtk.h"
#endif

using namespace WebKit;
using namespace WebCore;


#define __SPN_EYE_HIT__
//#define __USEMAP_CASE__
//#define __SPN_DEBUG__
//#define __SPN_PROTO__

#ifdef __SPN_PROTO__
#define SPN_PROTO(...)				do { printf("[spn proto>> %s:%d] ", __func__, __LINE__); \
									printf(__VA_ARGS__); \
									printf("\n"); } while(0)
#endif

#ifdef __SPN_DEBUG__
#define SPN_ASSERT(exp)				if(!(exp))	{\
									printf("[Assertion failed, %s, %s:%d]\n", #exp, __func__, __LINE__); \
									abort();}
#define SPN_LOG(...)					do { printf("[spn log>> %s:%d] ", __func__, __LINE__); \
									printf(__VA_ARGS__); \
									printf("\n"); } while(0)
#define SPN_DISTANCE_LOG(...)			do { printf("[spn distance>> %s:%d] ", __func__, __LINE__); \
									printf(__VA_ARGS__); \
									printf("\n"); } while(0)
#else
#define SPN_ASSERT(exp)
#define SPN_LOG(...)
#define SPN_DISTANCE_LOG(...)

#endif

#define SCROLL_BAND 	80

// 20100909 : naseogi(jangjoonho@samsung.com)
int m_nMouseX;
int m_nMouseY;
void GetMousePointer(int* x, int* y)
{
	*x = m_nMouseX;
	*y = m_nMouseY;
//	printf(">>>>>>>>> naseogi (%d, %d)\n", *x, *y);
}

namespace WebCore {

using namespace HTMLNames;

SpatialNavigator* SpatialNavigator::pInstance_ = NULL;
//static bool gbSwallowEvent = false;
static SpNodePostion gPreNodeInfo;

static inline int RectCenterX( SpRect rect)
{
	return rect.nLeft+rect.nWidth/2;
}

static inline int RectCenterY(SpRect rect)
{
	return (rect.nTop+rect.nHeight/2);
}
//y coordiante
static inline int RectBottom( SpRect rect)
{
	return (rect.nTop+rect.nHeight);
}
//x coordiante
static inline int RectRight( SpRect rect)
{
	return (rect.nLeft + rect.nWidth);
}

static inline bool hasOnFocusElement(BENode node)
{
	WebCore::Node* pTmp = (WebCore::Node*) node;
	if(!pTmp)
		return false;
	int nLoopCount =0,nNodeType=0;
	do
	{
		char* type = BE_GetAttribute(pTmp,"onfocus");
		if (!type)
			return false;
		if(strcmp(type,"\0")!=0)
		{
			if(type)
			{
				free(type);
				type = NULL;
			}
			return true;
		}
		if(type)
		{
			free(type);
			type = NULL;
		}
		nNodeType =BE_GetNodeType(pTmp);
		nLoopCount++;
		pTmp = pTmp->parentNode();
	}
	while(pTmp && nNodeType ==1 && nLoopCount<5);

	return false;
}
static inline bool hasListOnClick(BENode node)
{
	WebCore::Node* pNode = (WebCore::Node*) node;
	if(!pNode|| !pNode->isElementNode())
		return false;
	if(pNode->parentNode())
	{
		if(!pNode->parentNode()->hasTagName(liTag))
			return false;
		if(pNode->parentNode()->parentNode())
		{
			if(!pNode->parentNode()->parentNode()->hasTagName(ulTag))
			return false;
		}
	}
	if(!pNode->firstChild())
		return false;

	if(pNode->firstChild()->hasTagName(strongTag)||pNode->firstChild()->hasTagName(emTag))
	{
		char* type = BE_GetAttribute(pNode,"onclick");
		if (!type)
			return false;
		if(strcmp(type,"\0")!=0)
		{
			if(type)
			{
				free(type);
				type = NULL;
			}
			return true;
		}
		if(type)
		{
			free(type);
			type = NULL;
		}
	}
	return false;
}

static bool CheckMoveOver(Node* pNode)
{
	if(pNode->hasTagName(inputTag))
	{
		HTMLInputElement *inputElement = (HTMLInputElement*)pNode->toInputElement();
		if(inputElement)
		{
			if(inputElement->isText())
			{
				return true;
			}
		}
	}
	if(hasListOnClick((BENode)pNode) )
	{
	 	return true;
	}
	return false;
}

static Node*
NextNodeWithGreaterTabIndex(Node* pStart, int nTabIndex)
{

	int nBetterTabIndex = SHRT_MAX + 1;
	Node* pBetter = 0;
	for (Node* pNode = pStart; pNode; pNode = pNode->traverseNextNode())
	{
		if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() > nTabIndex && pNode->tabIndex() < nBetterTabIndex)
		{
		    pBetter = pNode;
		    nBetterTabIndex = pNode->tabIndex();
		}
	}
	return pBetter;
}

static Node*
PreviousNodeWithLowerTabIndex(Node* pStart, int nTabIndex)
{
	int nBetterTabIndex = 0;
	Node* pBetter = 0;
	for (Node* pNode = pStart; pNode; pNode = pNode->traversePreviousNode())
	{
		if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() < nTabIndex && pNode->tabIndex() > nBetterTabIndex)
		{
		    pBetter = pNode;
		    nBetterTabIndex = pNode->tabIndex();
		}
	}
	return pBetter;
}
static Node*
NextNodeWithTabIndex(Node* pStart, int nTabIndex)
{
	for (Node* pNode = pStart; pNode; pNode = pNode->traverseNextNode())
	{
		if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() == nTabIndex)
		{
		    return pNode;
		}
		if(BE_IsFlashElement((BENode)pNode))
		{
			return pNode;
		}
	}

    return NULL;
}
static Node*
PreviousNodeWithTabIndex(Node* pStart, int nTabIndex)
{

	for (Node* pNode = pStart; pNode; pNode = pNode->traversePreviousNode())
	{
		if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() == nTabIndex)
		{
		    return pNode;
		}
		if(BE_IsFlashElement((BENode)pNode))
		{
			return pNode;
		}
	}
	return NULL;
}

static Node*
NextFocusableNodeWithDocument(Document* pDoc,Node* pStart)
{
	if (pStart)
	{
		if (pStart->tabIndex() < 0)
		{
			for (Node* pNode = pStart->traverseNextNode(); pNode; pNode = pNode->traverseNextNode())
			{
				if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() >= 0)
				{
				    return pNode;
				}
			}
		}
		if (Node* pBetter = NextNodeWithTabIndex(pStart->traverseNextNode(), pStart->tabIndex()) )
		{
			return pBetter;
		}

		if (pStart->tabIndex() == 0)
		{
			return NULL;
		}
	}

	if (Node* pBetter = NextNodeWithGreaterTabIndex(pDoc, pStart ? pStart->tabIndex() : 0 ))
	{
		return pBetter;
	}

	return NextNodeWithTabIndex(pDoc, 0);
}

static Node*
PreviousFocusableNodeWithDocument(Document* pDoc,Node* pStart)
{
	Node* pLast;
	for (pLast = pDoc; pLast->lastChild(); pLast = pLast->lastChild())
	    ; // Empty loop.

	Node* pStartingNode;
	int nStartingTabIndex;
	if (pStart)
	{
	    pStartingNode = pStart->traversePreviousNode();
	    nStartingTabIndex = pStart->tabIndex();
	}
	else
	{
	    pStartingNode = pLast;
	    nStartingTabIndex = 0;
	}

	if (nStartingTabIndex < 0)
	{
	    for (Node* pNode = pStartingNode; pNode; pNode = pNode->traversePreviousNode())
	    {
	        if (pNode->isKeyboardFocusable(NULL) && pNode->tabIndex() >= 0)
	        {
	            return pNode;
	        }
	    }
	}

	if (Node* pBetter = PreviousNodeWithTabIndex(pStartingNode, nStartingTabIndex ))
	    return pBetter;

	nStartingTabIndex = (pStart && pStart->tabIndex()) ? pStart->tabIndex() : SHRT_MAX;
	return PreviousNodeWithLowerTabIndex(pLast, nStartingTabIndex);
}

static inline bool
CheckNegativeCoordinates( RenderObject* pRenderer, const BERect curRect)
{
    if (curRect.left > 0 && curRect.top > 0)
    {
        return true;
    }

    bool bCanBeScrolled = false;
    for (; pRenderer; pRenderer = pRenderer->parent()) {
        if (pRenderer->isBox() && toRenderBox(pRenderer)->canBeScrolledAndHasScrollableArea()) {
            bCanBeScrolled = true;
            break;
        }
    }

    return bCanBeScrolled;
}

static inline bool
IsOffVisibleScreenRect(Node* pNode, IntRect rect)
{
	FrameView* frameView = pNode->document()->view();
	if (!frameView)
	{
	    return true;
	}
	IntRect containerViewportRect = frameView->visibleContentRect();
	Frame* frame = pNode->document()->frame();
	if(frame)
	{
		if (Element* pElement = static_cast<Element*>(frame->ownerElement()))
		{
			do
			{
				containerViewportRect.move(pElement->offsetLeft(), pElement->offsetTop());
			} while ((pElement = pElement->offsetParent()));
		}
	}

	return !containerViewportRect.intersects(rect);
}
static unsigned int isqrt(unsigned int x)
{
	unsigned int m =0x40000000;//(1<<30)
	unsigned int r =0,nr;
	do{
		nr=m+r;
		if(nr<=x){
			x-=nr;
			r= nr+m;
		}
		m>>=2;
		r>>=1;
	}while (m!=0);
	return r;
}

static inline bool
IsEceptionCase(eFocusDirection direction, SpPoint StartPoint, BERect StartRect, BERect DestRect)
{
	switch (direction)
	{
	case FocusDirection_Left:
	case FocusDirection_Right:
		// dest rect left < start focus x axis < dest rect right
		if(DestRect.left < StartPoint.nX && StartPoint.nX < (DestRect.left + DestRect.width))
		{
			//dest rect top < start point y axis < dest bottom
			if(DestRect.top < StartPoint.nY && StartPoint.nY <(DestRect.top + DestRect.height))
			{
				return false;
			}

			//start rect top <= dest Rect top && dest rect bottom <= start rect bottom
			if(StartRect.top <= DestRect.top && (DestRect.top + DestRect.height) <= (StartRect.top + StartRect.height))
			{
				return false;
			}

			return true;
		}
		break;
	case FocusDirection_Up :
	case FocusDirection_Down:
		//dest rect top < start focus y axis < dest rect bottom
		if(DestRect.top < StartPoint.nY &&  StartPoint.nY < (DestRect.top + DestRect.height))
		{
			//dest rect left < start point x axis < dest rect right
			if(DestRect.left < StartPoint.nX && StartPoint.nX < (DestRect.left + DestRect.width))
			{
				return false;
			}

			//start rect left <= dest rect left && dest rect right <= start rect right
			if(StartRect.left <= DestRect.left && (DestRect.left + DestRect.width) <= (StartRect.left + StartRect.width))
			{
				return false;
			}

			return true;
		}
		break;
	default:
		break;
	}

	return false;
}

static inline bool
IsRectInDirection(eFocusDirection direction, SpPoint StartPoint, BERect DestRect)
{
	switch (direction)
	{
	case FocusDirection_Left:
		//dest rect left < start focus x axis
		return (DestRect.left < StartPoint.nX);
	case FocusDirection_Right:
		//dest rect reight  > start focus x axis
		return ((DestRect.left + DestRect.width) > StartPoint.nX);
	case FocusDirection_Up :
		//dest rect top <	 start focus point y axis
		return (DestRect.top < StartPoint.nY);
	case FocusDirection_Down:
		// dest rect bottom  >  start focus point y axis
		return  ((DestRect.top + DestRect.height) > StartPoint.nY) ;
	default:
		break;
	}

	return false;
}

static inline bool
IsInDirectionLineofFocusPoint(eFocusDirection direction, SpPoint StartPoint, BERect DestRect, BERect StartRect)
{
	switch (direction)
	{
	case FocusDirection_Left:
	case FocusDirection_Right:
		// dest top <=start focus yaxis <= dest bottom
		return ( (DestRect.top <= StartPoint.nY && StartPoint.nY <= (DestRect.top + DestRect.height))
				|| ((StartRect.top <= DestRect.top)&& (DestRect.top + DestRect.height) <= (StartRect.top + StartRect.height)) );
	case FocusDirection_Up :
	case FocusDirection_Down:
		//  dest rect left <= start focus  x axis <= dest rect right
		return (  (DestRect.left <= StartPoint.nX && StartPoint.nX <= (DestRect.left + DestRect.width))
				||  ((StartRect.left <= DestRect.left) && (DestRect.left+DestRect.width <=StartRect.left+StartRect.width)) );

	default:
		return false;
	}
	return false;
}

static inline unsigned int
DotDistance(SpPoint StartPoint, SpPoint DestPoint)
{
 	unsigned int  nDotDistance = (unsigned int) ( (DestPoint.nX - StartPoint.nX) * (DestPoint.nX - StartPoint.nX) ) + ((DestPoint.nY - StartPoint.nY) * (DestPoint.nY - StartPoint.nY));
	return isqrt(nDotDistance);// i wiil change isqrt
}

//after checking IsInDirectionLineofFocusPoint
static inline unsigned int
CalcDotDistance(eFocusDirection direction, SpPoint StartPoint, BERect DestRect)
{
	SpPoint DestPoint;
	memset(&DestPoint,0,sizeof(SpPoint));
	switch (direction)
	{
	case FocusDirection_Left:
		// 1,,, start point y axis > dest rect  bottom  -> dest rect right bottom
		if(StartPoint.nY > (DestRect.top + DestRect.height))
		{
			DestPoint.nX = DestRect.left + DestRect.width;
			DestPoint.nY = DestRect.top + DestRect.height;
			//,1,,,,and (dest rect left < start point x axis  <dest rect right )  --> dest rect  (start point x, bottom)
			if(DestRect.left <= StartPoint.nX && StartPoint.nX <= (DestRect.left + DestRect.width))
			{
				DestPoint.nX = StartPoint.nX;
				DestPoint.nY = DestRect.top + DestRect.height;
			}
		}
		// 2,,,start point y axis <dest rect Top  -> dest rect right Top
		else if(StartPoint.nY < DestRect.top)
		{
			DestPoint.nX = DestRect.left + DestRect.width;
			DestPoint.nY = DestRect.top;
			//,2,,,,and (dest rect left < start point x axis  <dest rect right )  --> dest rect  (start point x, top)
			if(DestRect.left <= StartPoint.nX && StartPoint.nX <= (DestRect.left + DestRect.width))
			{
				DestPoint.nX = StartPoint.nX;
				DestPoint.nY = DestRect.top;
			}
		}
		else
		{
			SPN_LOG("error case\n");
		}
		break;

	case FocusDirection_Right:
		// 1,,, start point y axis < dest rect  bottom   -> dest rect left bottom
		if(StartPoint.nY > (DestRect.top + DestRect.height))
		{
			DestPoint.nX = DestRect.left;
			DestPoint.nY = DestRect.top + DestRect.height;
			//,1,,,,and (dest rect left < start point x axis  <dest rect right )  --> dest rect  (start point x, bottom)
			if(DestRect.left <= StartPoint.nX && StartPoint.nX <= (DestRect.left + DestRect.width))
			{
				DestPoint.nX = StartPoint.nX;
				DestPoint.nY = DestRect.top + DestRect.height;
			}
		}
		// 2,,,start point y axis < dest rect Top  -> dest rect left Top
		else if(StartPoint.nY < DestRect.top)
		{
			DestPoint.nX = DestRect.left;
			DestPoint.nY = DestRect.top;
			//,2,,,,and (dest rect left < start point x axis  <dest rect right )  --> dest rect  (start point x, top)
			if(DestRect.left <= StartPoint.nX && StartPoint.nX <= (DestRect.left + DestRect.width))
			{
				DestPoint.nX = StartPoint.nX;
				DestPoint.nY = DestRect.top;
			}
		}
		else
		{
			SPN_LOG("error case\n");
		}
		break;

	case FocusDirection_Up:
		// 1,,, dest rect left > start point x axis -> dest rect left bottom
		if(DestRect.left > StartPoint.nX)
		{
			DestPoint.nX = DestRect.left;
			DestPoint.nY = DestRect.top + DestRect.height;
			//,1,,,,and (dest rect top < start point y axis  && start point y axis  <= dest rect bottom  --> dest rect (left, start point y)
			if(DestRect.top <= StartPoint.nY && StartPoint.nY <= (DestRect.top + DestRect.height))
			{
				DestPoint.nX = DestRect.left;
				DestPoint.nY = StartPoint.nY;
			}
		}
		// 2,,,dest rect right < start point x axis-> dest rect right bottom
		else if((DestRect.left + DestRect.width) < StartPoint.nX)
		{
			DestPoint.nX = DestRect.left + DestRect.width;
			DestPoint.nY = DestRect.top + DestRect.height;
			//,2,,,,and (dest rect top < start point y axis  && start point y axis <=dest rect bottom    --> dest rect  (right, start point y)
			if(DestRect.top <= StartPoint.nY && StartPoint.nY <= (DestRect.top + DestRect.height))
			{
				DestPoint.nX = DestRect.left + DestRect.width;
				DestPoint.nY = StartPoint.nY;
			}
		}
		else
		{
			SPN_LOG("error case\n");
		}
		break;

	case FocusDirection_Down:
		//dest  right case
		// 1,,, start point x axis < dest rect left   -> dest rect left Top
		if( StartPoint.nX < DestRect.left)
		{
			DestPoint.nX = DestRect.left;
			DestPoint.nY = DestRect.top;
			//,1,,,,and (dest rect top < start point y axis  &&  start point y axis <= dest rect bottom   --> dest rect left (x, start point y)
			if(DestRect.top <= StartPoint.nY && StartPoint.nY <= (DestRect.top + DestRect.height))
			{
				DestPoint.nX = DestRect.left;
				DestPoint.nY = StartPoint.nY;
			}
			//printf("[%s][%d]down case(%d,%d)\n",__func__,__LINE__,DestPoint.nX,DestPoint.nY);
		}
		//dest left case
		// 2,,,dest rect right < start point x axis-> dest rect right Top
		else if(DestRect.left + DestRect.width < StartPoint.nX)
		{
			DestPoint.nX = DestRect.left + DestRect.width;
			DestPoint.nY = DestRect.top;
			//,2,,,,and dest rect top < start point y axis  &&  start point y axis <= dest rect bottom   --> dest rect right (x, start point y)
			if(DestRect.top <= StartPoint.nY && StartPoint.nY <= (DestRect.top + DestRect.height))
			{
				DestPoint.nX = DestRect.left + DestRect.width;
				DestPoint.nY = StartPoint.nY;
			}
			//printf("[%s][%d]down case(%d,%d)\n",__func__,__LINE__,DestPoint.nX,DestPoint.nY);
		}
		else
		{
			SPN_LOG("error case\n");
		}
		break;

	default:
		SPN_LOG("error case\n");
		break;
	}

	unsigned int nDotDistance=0;
	nDotDistance = DotDistance(StartPoint, DestPoint);
	return nDotDistance;
}

static inline bool
IsOverlaped(void)
{
	return false;
}

static inline unsigned int
CalcOverlap(unsigned int* pOverlap)
{
	return 0;
}

static inline void
CalcDxDy(eFocusDirection direction, BERect StartRect, BERect DestRect, int* pDx, int* pDY)
{
	switch (direction)
	{
	case FocusDirection_Left:
		{
			//only dx consider
			//start rect left - dest rect right
			int nDx = StartRect.left - (DestRect.left + DestRect.width);
			if(nDx >0)
			{
				if(pDx)
				{
					*pDx = nDx;
				}
			}
			else
			{
				if(pDx)
				{
					*pDx = 0;
				}
			}
			if(pDY)
			{
				*pDY =0;
			}
		}
		break;
	case FocusDirection_Right:
		{
			//only dx consider
			//dest rect left - start rect right
			int nDx = DestRect.left - StartRect.left;
			if(nDx >0)
			{
				if(pDx)
				{
					*pDx = nDx;
				}
			}
			else
			{
				if(pDx)
				{
					*pDx = 0;
				}
			}
			if(pDY)
			{
				*pDY =0;
			}
		}
		break;
	case FocusDirection_Up :
		{
			//only dy consider
			//dest rect bottom - start rect top
			int nDy=  StartRect.top  - (DestRect.top + DestRect.height);
			if(nDy >0)
			{
				if(pDY)
				{
					*pDY = nDy;
				}
			}
			else
			{
				if(pDY)
				{
					*pDY = 0;
				}
			}
			if(pDx)
			{
				*pDx = 0;
			}
		}
		break;
	case FocusDirection_Down:
		{
			//only dy consider
			// dest rect top - start rect bottom
			int nDy=  DestRect.top - (StartRect.top + StartRect.height);
			if(nDy >0)
			{
				if(pDY)
				{
					*pDY = nDy;
				}
			}
			else
			{
				if(pDY)
				{
					*pDY = 0;
				}
			}
			if(pDx)
			{
				*pDx = 0;
			}
		}
		break;
	default:
		break;
	}
	return;
}

static inline void
CalcDisplacement(eFocusDirection direction, BERect StartRect, BERect DestRect, int* pXdisplacement, int* pYdisplacement)
{
	switch (direction)
	{
	case FocusDirection_Left:
	case FocusDirection_Right:
		{
			//only Ydisplacement consider
			//first check dest rect top - start rect bottom
			//second check dest rect bottom - start rect top;
			int nFirstYdisplacement = DestRect.top - (StartRect.top + StartRect.height);
			int nSecondYdisplacement = (DestRect.top + DestRect.height) - StartRect.top;
			if(nFirstYdisplacement >0 && nSecondYdisplacement >0)
			{
				if(pYdisplacement)
				{
					*pYdisplacement = nFirstYdisplacement;
				}
			}
			else if(nFirstYdisplacement <0 && nSecondYdisplacement <0)
			{
				if(pYdisplacement)
				{
					*pYdisplacement = abs(nSecondYdisplacement);
				}
			}
			else
			{
				if(pYdisplacement)
				{
					*pYdisplacement = 0;
				}
			}
			if(pXdisplacement)
			{
				*pXdisplacement =0;
			}
		}
		break;
	case FocusDirection_Up :
	case FocusDirection_Down:
		{
			//only Xdisplacement consider
			//first check dest rect left - start rect right
			//second check dest rect right - start rect left;
			int nFirstXdisplacement=  DestRect.left - (StartRect.left + StartRect.width) ;
			int nSecondXdisplacement=  (DestRect.left + DestRect.width) - StartRect.left;
			if(nFirstXdisplacement >0 && nSecondXdisplacement>0)
			{
				if(pXdisplacement)
				{
					*pXdisplacement = nFirstXdisplacement;
				}
			}
			else if(nFirstXdisplacement <0 && nSecondXdisplacement<0)
			{
				if(pXdisplacement)
				{
					*pXdisplacement =abs( nSecondXdisplacement);
				}
			}
			else
			{
				*pXdisplacement =0;
			}
			if(pYdisplacement)
			{
				*pYdisplacement = 0;
			}
		}
		break;

	default:
		SPN_LOG("error case\n");
		break;
	}
	return;
}

static inline unsigned int
TotalDistanceFunc(unsigned int nDotDistance, unsigned int nDx, unsigned int nDy, unsigned int nXdisplacement,unsigned int nYdisplacement, unsigned int nOverap)
{
	unsigned int nDf = nDotDistance + nDx + nDy + 2*(nXdisplacement + nYdisplacement) - isqrt(nOverap);
	//printf("Df :%d==DotDist(%d)==(Dx,Dy)(%d,%d)==(Xdisplacement,YDisplacement)(%d,%d)== (overlap)(%d) \n\n",nDf,nDotDistance,nDx,nDy, nXdisplacement, nYdisplacement, nOverap);
	return nDf;
}

static inline void
DecesionStartPoint(SpPoint* pStartPoint, eFocusDirection direction, BERect StartRect )
{
	switch (direction)
	{
	case FocusDirection_Left:
		if(pStartPoint)
		{
			pStartPoint->nX = StartRect.left;
			pStartPoint->nY = StartRect.top + StartRect.height / 2;
		}
		break;
	case FocusDirection_Right:
		if(pStartPoint)
		{
			pStartPoint->nX = StartRect.left + StartRect.width;
			pStartPoint->nY = StartRect.top + StartRect.height / 2;
		}
		break;
	case FocusDirection_Up:
		if(pStartPoint)
		{
			pStartPoint->nX = StartRect.left + StartRect.width / 2;
			pStartPoint->nY = StartRect.top;
		}
		break;
	case FocusDirection_Down:
		if(pStartPoint)
		{
			pStartPoint->nX = StartRect.left + StartRect.width / 2;
			pStartPoint->nY = StartRect.top + StartRect.height;
		}
		break;
	default:
		SPN_LOG("error case\n");
		break;
	}
	return;
}

bool
SpatialNavigator::IsSpnFocusable(Node* pNode)
{
	if(pNode)
	{
		if(pNode->hasTagName(iframeTag)||pNode->hasTagName(frameTag))
		{
			return false;
		}
		if(pNode->isKeyboardFocusable(NULL))
		{
			return true;
		}
#if USE(UNIPLAYER)
		if (pNode->hasTagName(videoTag))
		{
			SPN_LOG("------videoTag\n");
			return true;
		}
#endif
		if(pNode->hasTagName(inputTag))
		{
			HTMLInputElement *inputElement = (HTMLInputElement*) pNode->toInputElement();
			if(inputElement)
			{
				if(inputElement->isRadioButton())
				{
					return true;
				}
			}
		}
		if(BE_IsFlashElement((BENode)pNode))
		{
			return true;
		}
	}

    return false;
}

#ifdef SMART_FOCUS_ZOOM
static inline bool
IsSpnFocusableForSmartZoom(Node* pNode)
{
	if(pNode->isKeyboardFocusable(NULL))
	{
		return true;
	}



	if(pNode != NULL)
	{
#if USE(UNIPLAYER)
		if (pNode->hasTagName(videoTag))
		{
		    //printf("[%s][%d]------videoTag\n",__func__,__LINE__);
		    return true;
		}
#endif
		if(pNode->hasTagName(inputTag) )
		{
			Element *pElement = (WebCore::Element *)pNode;
			InputElement *inputElement = pNode->toInputElement();
			if(inputElement  != NULL)
			{
					if(strcmp(pElement->formControlType().string().utf8().data(),"radio")==0)
					{
						return true;
					}
			}
		}
		if(pNode->hasTagName(textareaTag) )
		{
			return true;
		}
		if(pNode->hasTagName(aTag))
		{
			    BERect aRect;
		          aRect = BE_GetNodeRect((void*)pNode);
			    if(aRect.width ==0 && aRect.height == 0 && pNode->isLink())
			    {
				    Node* pTmpNode = pNode->firstChild();
				    while (pTmpNode)
				    {
						if(pTmpNode->hasTagName(imgTag) || pTmpNode->hasTagName(spanTag))
						{
							BERect bRect;
							bRect = BE_GetNodeRect((void*)pTmpNode);
							if(  bRect.width > 0 || bRect.height > 0 )
							{
								return true;
							}
						}
				        pTmpNode = pTmpNode->traverseNextNode(pNode);
				    }
		    		}
		}
		if(BE_IsFlashElement((BENode)pNode))
		{
			return true;
		}

		if(pNode->hasTagName(pTag))
		{
			return true;
		}

		if(pNode->hasTagName(divTag) )
		{
			// FIXME : What is this?!
			// Remove below code. If not necessary!
			// from midow.oh@samsung.com
			//
			//std::string innerText = BE_GetInnerText((BENode)pNode);
			Node* pTmpNode = pNode->firstChild();
			while(pTmpNode)
			{
				if(pTmpNode->hasTagName(emTag) || pTmpNode->hasTagName(brTag) || pTmpNode->hasTagName(bTag) || pTmpNode->hasTagName(spanTag) || pTmpNode->hasTagName(nobrTag))
				{//
				}
				else
				{
					return false;
				}
				pTmpNode = pTmpNode->traverseNextNode(pNode);
			}

			return true;
		}
	}

	return false;
}
#endif

bool
SpatialNavigator::IsOffScreenRect(Node* pNode, IntRect DestRect)
{
#if ENABLE(TVGG_BACKINGSTORE)
	if (BackingStore::isAvailable())
	{
		BackingStore* backingStore = BackingStore::getInstance();
		WebCore::IntRect viewRect = backingStore->viewRect();
		FrameView* frameView = pNode->document()->view();
		if (!frameView)
		{
		    return true;
		}
		if(DestRect.width()>=viewRect.width() && DestRect.height() >viewRect.height())
		{
			return true;
		}
		if(pNode->document()->ownerElement())
		{
			if(pNode->document()->ownerElement()->hasTagName(iframeTag))
			{
				IntRect containerViewportRect = frameView->visibleContentRect();
				/*
				WebCore::ScrollView* view = pNode->document()->view();
				while(view)
				{
					IntRect frameRect = view->frameRect();
					containerViewportRect.move(frameRect.x(), frameRect.y());
					if(view->parent())
					containerViewportRect.move(-view->scrollOffset());
					view = view->parent();
				}
				*/
				if(containerViewportRect.width()>=viewRect.width() && containerViewportRect.height()>=viewRect.height())
				{
					/*printf("[%s][%d] IFrame >>>>>containerViewPort,(%d,%d,%d,%d) viewRect(%d,%d)rect (%d,%d,%d,%d)(%s)\n\n",__func__,__LINE__,
					containerViewportRect.x(),containerViewportRect.y(), containerViewportRect.width(), containerViewportRect.height(),viewRect.width() ,viewRect.height(),DestRect.x(),DestRect.y(), DestRect.width(), DestRect.height(),pNode->nodeName().utf8().data());*/
					return !containerViewportRect.intersects(DestRect);
				}
			}
		}
		return !viewRect.intersects(DestRect);
	}
	return true;
#else
	FrameView* frameView = pNode->document()->view();
	if (!frameView)
	{
	    return true;
	}

	RenderObject* render = pNode->renderer();
	if (!render)
	{
	    return true;
	}

	IntRect rect(render->absoluteClippedOverflowRect());
	IntRect containerViewportRect = frameView->visibleContentRect();

	return !containerViewportRect.intersects(rect);
#endif
}

Result
SpatialNavigator::GetVisibleScreenSize(int * pTop, int * pBttom)
{

#if ENABLE(TVGG_BACKINGSTORE)
	IntRect rect;
	if (BackingStore::isAvailable())
	{
		BackingStore* backingStore = BackingStore::getInstance();
		WebCore::IntRect viewRect = backingStore->viewRect();
		rect = viewRect;
	}
#else
 	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);
	IntRect rect = frame->view()->visibleContentRect();
#endif


#if !ENABLE(TVGG_BACKINGSTORE)
	if (Element* pElement = static_cast<Element*>(frame->ownerElement()))
	{
	 	if(pElement->hasTagName(iframeTag))
	 	{
			frame = m_pPage->mainFrame();
			rect = frame->view()->visibleContentRect();
			pElement = static_cast<Element*>(frame->ownerElement());
	 	}
		if(pElement)
		{
			do
			{
			    rect.move(pElement->offsetLeft(), pElement->offsetTop());
			} while ((pElement = pElement->offsetParent()));
		}
	}
#endif
	if(pTop)
	{
		*pTop = rect.y();
	}
	if(pBttom)
	{
		*pBttom = rect.maxY();
	}
	return S_OK;
}


void
SpatialNavigator::SetNodeInfo(Node* pPreNode)
{
	if(!pPreNode)
	{
		memset(&gPreNodeInfo, 0, sizeof(SpNodePostion));
		SPN_LOG("Resest PreNodeInfo!!!\n");
	}
	else
	{
		BERect CurrentRect = BE_GetNodeRect(pPreNode);
		int nCurrentCenterX = CurrentRect.left + 4;
		int nCurrentCenterY = CurrentRect.top + 4;
		if(nCurrentCenterX > CurrentRect.width && nCurrentCenterY > CurrentRect.height)
		{
			nCurrentCenterX = CurrentRect.left + CurrentRect.width/2;
			nCurrentCenterY = CurrentRect.top + CurrentRect.height/4;
		}
		gPreNodeInfo.nCenterX = nCurrentCenterX;
		gPreNodeInfo.nCenterY = nCurrentCenterY;
	}
}

void
SpatialNavigator::SetNoneCusor(void)
{
	FrameView* pView = m_pPage->focusController()->focusedOrMainFrame()->view();
	if (pView)
	{
		pView->setCursor(noneCursor());
	}
}

Node*
SpatialNavigator::GetFocusablePreNode(void)
{
	Node* pPreNode = NULL;
	if(gPreNodeInfo.nCenterX !=0 && gPreNodeInfo.nCenterY !=0 )
	{
		Document* pFocusedDoc = GetFocusedDoc();
		//printf("PreNodeInfo(%d,%d)==============\n",gPreNodeInfo.nCenterX,gPreNodeInfo.nCenterY);
		pPreNode = GetNodeFromPoint(gPreNodeInfo.nCenterX, gPreNodeInfo.nCenterY,pFocusedDoc);
		if(!pPreNode)
		{
			SPN_LOG("PreNode Not Focusable!!! \n");
		}
	}
	else
	{
		memset(&gPreNodeInfo, 0, sizeof(SpNodePostion));
		SPN_LOG("There is no PreNodeInfo!!!\n");
	}

	return pPreNode;
}

Document*
SpatialNavigator::GetFocusedDoc()
{
    Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
    SPN_ASSERT(pFrame);

    Document* focusedDocument = pFrame->document();
    return focusedDocument;
}

Document*
SpatialNavigator::FindNewDocFromNode(Node* pNode,Document* pFocusedDoc)
{
    Document* pNewDocument = pNode->document();

    if (pNewDocument != pFocusedDoc)
    {
        pFocusedDoc->setFocusedNode(0);
	 SPN_LOG("focusedDocument reset!!\n");
    }

    if (pNewDocument)
    {
        m_pPage->focusController()->setFocusedFrame(pNewDocument->frame());
    }
    return pNewDocument;
}

void
SpatialNavigator::ContentsToWindowPos( int* pX, int* pY)
{
	Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
	FrameView* pFrameview = pFrame->view();
	IntPoint scrollPos = pFrameview->scrollPosition();
	(*pX) -= scrollPos.x();
	(*pY) -= scrollPos.y();
}

void*
SpatialNavigator::GetNextNode(void)
{
	void* pNode = (void*)m_pBestNode;
	return pNode;
}


void*
SpatialNavigator::GetCurrentFocusedNode(void)
{
	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);
	Document* focusedDocument = frame->document();
	if (!focusedDocument)
	    return NULL;

	void* pNode = NULL;
	Node* focusedNode = focusedDocument->focusedNode();
	pNode = (void*)focusedNode;
	if(pNode)
		FindNewDocFromNode((Node*)pNode,focusedDocument);
	return pNode;
}

#ifdef SMART_FOCUS_ZOOM
void
SpatialNavigator::SetSmartZoom(SpWebView pContentsView, int x, int y)
{
	BEResult retVal=BEFail;
	SpPoint CurrentStartPoint;
	// Zoom in
	CurrentStartPoint.nX = x;
	CurrentStartPoint.nY = y;

	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);
	Document* focusedDocument = frame->document();
	if (!focusedDocument)
	    return;

	//Node* firstNode = GetNodeFromPoint(x, y, focusedDocument);
	Node* firstNode = (Node*)BE_GetBody((BEDocument)focusedDocument);
	//Node* firstNode = focusedDocument->firstChild();
	m_pSmartZoomParent = firstNode;
	BERect firstNodeRect = BE_GetAbsoluteNodeRect(firstNode);//GetNodeRect(firstNode);
	m_nArea = (firstNodeRect.width * firstNodeRect.height);
	m_pSmartFocusBestNode = firstNode;
	frame = frame->tree()->top();
	WebCore::IntPoint pt1 = frame->view()->scrollPosition();

	for (Frame* CurrentFrame = frame; CurrentFrame; CurrentFrame = CurrentFrame->tree()->traverseNext(frame))
	{
		for (Node* candidate = firstNode; candidate; candidate = candidate->traverseNextNode())
		{
			{
				if(RecursiveNodeFetch(candidate, pt1.x() + x , pt1.y() + y ))
				    break;
			}
		}
	}
	if(m_pSmartFocusBestNode->hasTagName(imgTag) || BE_IsVideoElement((BENode)m_pSmartFocusBestNode))
	{
		retVal = BE_ZoomPage(pContentsView, 0, m_pSmartFocusBestNode);
	}
	else
	{
		retVal = BE_ZoomPage(pContentsView, 1.7, m_pSmartFocusBestNode);
	}
	if(retVal == BEFail){
		printf("[%s][%d] Fail==============================<<[%d]  !!\n", __func__, __LINE__,retVal);

	}

	return;
}

bool SpatialNavigator::RecursiveNodeFetch(Node* pNode, int x, int y)
{
	if(!pNode)
	{
	 return true;
	}

	BERect cadidateRect = BE_GetAbsoluteNodeRect(pNode);//GetNodeRect(pNode);
	if((cadidateRect.left < x) && (x < (cadidateRect.left + cadidateRect.width)) && (cadidateRect.top < y) && (y < (cadidateRect.top + cadidateRect.height)))
	{
	    //if(m_nArea >= (cadidateRect.nWidth * cadidateRect.nHeight))
		{
			m_pSmartZoomParent = pNode;
			m_nArea = (cadidateRect.width * cadidateRect.height);
			m_pSmartFocusBestNode = pNode;
			RecursiveNodeFetch(pNode->firstChild(), x, y);

		}
		/*else
		{printf("Inside Area NEXT NODE Condition \n\n");

			RecursiveNodeFetch(pNode->nextSibling(), x, y);
		}*/
	}
	else
	{
		if(!pNode->nextSibling())
		{
			return true;
		}
		RecursiveNodeFetch(pNode->nextSibling(), x, y);
	}
	return false;
}

void
SpatialNavigator::SetFocusOnNode(SpWebView pContentsView, int x, int y)
{
	BEResult retVal=BEFail;
	SpPoint CurrentStartPoint;
	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	frame = frame->tree()->top();
    	WebCore::IntPoint pt1 = frame->view()->scrollPosition();
	CurrentStartPoint.nX = pt1.x() + x;
	CurrentStartPoint.nY = pt1.y() + y;

	if(GetNearestFocusableNode(&CurrentStartPoint, false) == S_OK)
	{
		retVal=BE_SetSpElementFocus( pContentsView, m_pSmartFocusBestNode, BELeft,3 /*borderWidth*/, "#006699", 1 /*borderRadius*/,0 /*arrowWidth*/, 0/* arrowMargin*/, 0 /*transitionDuration*/);
	}
	if(retVal == BEFail){
		printf("[%s][%d] Fail==============================<<[%d]  !!\n", __func__, __LINE__,retVal);

	}
	return;
}

Result SpatialNavigator::GetNearestFocusableNode(const SpPoint* srcPoint, bool isZoom)
{
	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);

	Document* focusedDocument = frame->document();
	if (!focusedDocument)
	    return E_FAIL;

	frame = frame->tree()->top();

	Node* pNode = NULL;

	SpRect spnRect;
	memset(&spnRect,0,sizeof(SpRect));

	BestCandidate bestCandidate;
	memset(&(bestCandidate.AbsoluteRect),0,sizeof(SpRect));

	for (Frame* CurrentFrame = frame; CurrentFrame; CurrentFrame = CurrentFrame->tree()->traverseNext(frame))
	{
		//NextInDirection(, pPreNode, direction, bestCandidate);
		NextInAllDirection(CurrentFrame->document(), srcPoint, bestCandidate, isZoom);
	}

	if(NULL == bestCandidate.node)
	{
		return E_FAIL;
	}
	pNode = bestCandidate.node;

	spnRect = bestCandidate.AbsoluteRect;

	GetVisibleScreenSize(&m_nVisibleScreenTop,&m_nVisibleScreenBottom);

	if (!pNode ||  !pNode->isElementNode())
	{
		// Leave the focus on the same node but scroll.
		Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
		if(!pFrame)
		{
			return E_FAIL;
		}
	    	return S_OK;
	}

	FindNewDocFromNode(pNode,focusedDocument);
	//SetMoveOver(pNode);
	m_pSmartFocusBestNode = pNode;
	SetNodeInfo(pNode);
	SetFocus(pNode);
	return S_OK;
}

void SpatialNavigator::GetDestinationNodePoint(const SpPoint* pSrcPoint, Node* pDestNode, SpPoint* pDestPoint)
{
	pDestPoint->nX = 0;
	pDestPoint->nY = 0;

	RenderObject* pDestRender = NULL;
    pDestRender = pDestNode->renderer();

	if (pDestRender == NULL)
	{
		return;
	}

	BERect destRect = BE_GetAbsoluteNodeRect(pDestNode);

	//when source point lies within the height of destination rectangle
	if(pSrcPoint->nY >= destRect.top && pSrcPoint->nY <= (destRect.top + destRect.height))
	{
		//and lies to the left of destination rectangle
		if(pSrcPoint->nX < destRect.left)
		{
			pDestPoint->nX = destRect.left;
			pDestPoint->nY = pSrcPoint->nY;
		}
		else if(pSrcPoint->nX <= (destRect.left + destRect.width)) // lies within the destination rectangle
		{
			//pDestPoint->nX = destRect.left + destRect.width/2;
			//pDestPoint->nY = destRect.top + destRect.height/2;
			pDestPoint->nX = pSrcPoint->nX;
			pDestPoint->nY = pSrcPoint->nY;
		}
		else // lies to the right of destination rectangle
		{
			pDestPoint->nX = destRect.left + destRect.width;
			pDestPoint->nY = pSrcPoint->nY;
		}
	}
	else if(pSrcPoint->nY < destRect.top && pSrcPoint->nY < (destRect.top + destRect.height)) // source point lies above the destination rectangle
	{
		//and lies to the left of destination rectangle
		if(pSrcPoint->nX < destRect.left)
		{
			pDestPoint->nX = destRect.left;
			pDestPoint->nY = destRect.top;
		}
		else if(pSrcPoint->nX < (destRect.left + destRect.width))// lies within the destination rectangle
		{
			pDestPoint->nX = pSrcPoint->nX;
			pDestPoint->nY = destRect.top;
		}
		else // lies to the right of destination rectangle
		{
			pDestPoint->nX = destRect.left + destRect.width;
			pDestPoint->nY = destRect.top;
		}
	}
	else if(pSrcPoint->nY > destRect.top && pSrcPoint->nY > (destRect.top + destRect.height)) // source point lies below the destination rectangle
	{
		//and lies to the left of destination rectangle
		if(pSrcPoint->nX < destRect.left)
		{
			pDestPoint->nX = destRect.left;
			pDestPoint->nY = destRect.top + destRect.height;
		}
		else if(pSrcPoint->nX < (destRect.left + destRect.width))// lies within the destination rectangle
		{
			pDestPoint->nX = pSrcPoint->nX;
			pDestPoint->nY = destRect.top + destRect.height;
		}
		else // lies to the right of destination rectangle
		{
			pDestPoint->nX = destRect.left + destRect.width;
			pDestPoint->nY = destRect.top + destRect.height;
		}
	}
	else
	{
		;
	}

	return;

}


unsigned int
SpatialNavigator::GetEuclideanDistance(const SpPoint* pSrcPoint, Node* pDestNode, Document* pDoc, SpRect* pSpnRect )
{
	RenderObject* pDestRender = NULL;
	pDestRender = pDestNode->renderer();

	if (pDestRender == NULL)
	 	return numeric_limits<unsigned int>::max();

	BERect DestRect = BE_GetAbsoluteNodeRect(pDestNode);
	IntRect destIntRect(DestRect.left, DestRect.top, DestRect.width, DestRect.height);

	if(pSpnRect)
	{
		pSpnRect->nLeft = DestRect.left;
		pSpnRect->nTop = DestRect.top;
		pSpnRect->nHeight = DestRect.height;
		pSpnRect->nWidth = DestRect.width ;
	}
	unsigned int nDotDistance =0;

	SpPoint destPoint;
	memset(&destPoint,0,sizeof(SpPoint));
	GetDestinationNodePoint(pSrcPoint, pDestNode, &destPoint);
	SpPoint* tempSrcPoint = const_cast<SpPoint*> (pSrcPoint);
	nDotDistance = DotDistance( *tempSrcPoint, destPoint);
	return nDotDistance;
}
#endif

bool
SpatialNavigator::LockMoveOver( const PlatformMouseEvent& mouseEvent, Node* pPointNode)
{
	bool bSwallowEvent = false;
    	Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(pFrame);
#if ENABLE(TVGG_SPN)
	bSwallowEvent= pFrame->eventHandler()->dispatchLockEvent(eventNames().mouseoverEvent,pPointNode, false, 0, mouseEvent, true);
#endif
	SPN_DISTANCE_LOG("swallowEvent %d+++++++++++++\n",bSwallowEvent);
	return bSwallowEvent;
}

void
SpatialNavigator::GetMotionRootPosition(GtkWidget* pWidget, int* pX, int* pY)
{
	do
	{
#ifdef GTK_API_VERSION_2
		(*pX) += pWidget->allocation.x;
		(*pY) += pWidget->allocation.y;
#else
		(*pX) += gtk_widget_get_allocated_width(pWidget);
		(*pY) += gtk_widget_get_allocated_height(pWidget);
#endif
		pWidget = gtk_widget_get_parent(pWidget);
	} while(pWidget);
}

bool
SpatialNavigator::SetMoveOver(Node* pPointingNode)
{
	BERect rect = BE_GetNodeRect(pPointingNode);
	int nDestCenterX = rect.left + rect.width/2;
	int nDestCenterY = rect.top + rect.height/4;
	int nRootX=0, nRootY=0;
	GdkEventMotion event;
	memset(&event, 0, sizeof(event));
	GetMotionRootPosition(GTK_WIDGET(m_pWebView), &nRootX, &nRootY);
	//ContentsToWindowPos( &nDestCenterX, &nDestCenterY);
	event.type = GDK_MOTION_NOTIFY;
	event.x = nDestCenterX;
	event.y = nDestCenterY;
	event.x_root = nRootX;
	event.y_root = nRootY;
	event.window = gtk_widget_get_window(GTK_WIDGET(m_pWebView));
	LockMoveOver(PlatformMouseEvent (&event ),pPointingNode);
	return true;
}

Result
SpatialNavigator::SpatialFocus(eFocusDirection direction,  bool bInitialFocus)
{
    Result RetVal = E_FAIL ;
    switch (direction)
    {
	case FocusDirection_Forward:
	case FocusDirection_Backward:
		RetVal = FocusOrderAsDocument(direction, bInitialFocus);
		break;
	case FocusDirection_Left:
	case FocusDirection_Right:
	case FocusDirection_Up:
	case FocusDirection_Down:
		RetVal = FocusOrderAsDirection(direction);
		break;
	case FocusDirection_None://current focus
		{
			RetVal = S_OK;
			Node* pNode = (Node*) GetCurrentFocusedNode();
			if(pNode)
			{
				if(!VisibleHitCheck(pNode, m_pPage->focusController()->focusedOrMainFrame()->document()))
				{
					RetVal= E_FAIL;
				}
				/*
				BERect DestRect = BE_GetAbsoluteNodeRect(pNode);
				GetVisibleScreenSize(&m_nVisibleScreenTop,&m_nVisibleScreenBottom);
				if(m_nVisibleScreenTop>=0 && m_nVisibleScreenBottom>=0)
				{
					int nRectBottom = DestRect.top+DestRect.height;
					int nRectTop =DestRect.top;
					if(nRectBottom < m_nVisibleScreenTop)
					{
						SPN_Scroll(ScrollUp, ScrollByPage);
					}
					else if(nRectTop > m_nVisibleScreenBottom)
					{
						SPN_Scroll(ScrollDown, ScrollByPage);
					}
					else
					{
						SPN_LOG("Scroll Nothing!!!!\n");
					}
				}
				*/
				SetNodeInfo(pNode);
				if(pNode->document()->view())
				{
					g_signal_emit_by_name(pNode->document()->view()->hostWindow()->platformPageClient(), "set-element-focus", pNode);
				}

			}
		}
		break;
	default:
		RetVal = E_FAIL;
		break;
    }

    return RetVal;
}

Result
SpatialNavigator::JumpFocus(eFocusDirection direction)
{
	switch (direction)
	{
	case FocusDirection_JumpTop:
		return MoveJumpFocus(MOVEMENT_BUFFER_ENDS,Up);

	case FocusDirection_JumpBottom:
		return MoveJumpFocus(MOVEMENT_BUFFER_ENDS,Down);

	case FocusDirection_PageUp:
		return MoveJumpFocus(MOVEMENT_PAGES,Up);

	case FocusDirection_PageDown:
		return MoveJumpFocus(MOVEMENT_PAGES,Down);

	case FocusDirection_PageRight:
		return MoveJumpFocus(MOVEMENT_PAGES,Right);

	case FocusDirection_PageLeft:
		return MoveJumpFocus(MOVEMENT_PAGES,Left);

    case FocusDirection_LineUp:
		return MoveJumpFocus(MOVEMENT_DISPLAY_LINES, Up);

    case FocusDirection_LineDown:
		return MoveJumpFocus(MOVEMENT_DISPLAY_LINES, Down);

	default:
		return E_FAIL;
    }

	return S_OK;
}

Result
SpatialNavigator::MoveJumpFocus(eMovementStep step, eJumpDirection jumpDirection)
{
    ScrollDirection direction = ScrollUp;
    ScrollGranularity granularity = ScrollByLine;
    SPN_DISTANCE_LOG("Step:%d, JumpDirection:%d\n",step,jumpDirection);
    switch (step)
    {
	case MOVEMENT_DISPLAY_LINES:
		granularity = ScrollByLine;
		if (jumpDirection == Down)
		{
			direction = ScrollDown;
		}
		else if (jumpDirection == Up)
		{
			direction = ScrollUp;
		}
		else
		{
			;
		}
		break;
	case MOVEMENT_PAGES:
		granularity = ScrollByPage;
		if (jumpDirection == Down)
		{
			direction = ScrollDown;
		}
		else if (jumpDirection == Up)
		{
			direction = ScrollUp;
		}
		else if (jumpDirection == Right)
		{
			direction = ScrollRight;
		}
		else if (jumpDirection == Left)
		{
			direction = ScrollLeft;
		}
		else
		{
			;
		}
		break;

	case MOVEMENT_BUFFER_ENDS:
		granularity = ScrollByDocument;
		if (jumpDirection == Down)
		{
			direction = ScrollDown;
		}
		else if (jumpDirection == Up)
		{
			direction = ScrollUp;
		}
		else
		{
			;
		}
		break;

	default:
		return E_FAIL;
	}
#if ENABLE(TVGG_BACKINGSTORE)
	if(!SPN_Scroll(direction,granularity))
	{
		return E_FAIL;
	}
#else
	Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(pFrame);
	pFrame->eventHandler()->scrollRecursively(direction, granularity);
#endif
    return S_OK;
}


Result
SpatialNavigator::FocusOrderAsDocument(eFocusDirection direction,  bool bInitialFocus)
{
	Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
	if(!pFrame)
	{
		return E_FAIL;
	}
	Document* pDoc = pFrame->document();
	Node* pCurrentNode = pDoc->focusedNode();
	pDoc->updateLayoutIgnorePendingStylesheets();

	Node* pNode = (direction == FocusDirection_Forward)
	    ? NextFocusableNodeWithDocument(pDoc,pCurrentNode)
	    : PreviousFocusableNodeWithDocument(pDoc,pCurrentNode);

	int count=0;
	while(!VisibleHitCheck(pNode,pDoc) && count <30)
	{
		pNode = (direction == FocusDirection_Forward)
            ? NextFocusableNodeWithDocument(pDoc,pNode)
            : PreviousFocusableNodeWithDocument(pDoc,pNode);
		count++;
	}

	while (!pNode && pFrame)
	{
	    Frame* pParentFrame = pFrame->tree()->parent();
	    if (!pParentFrame)
	    {
	        break;
	    }

	    Document* pParentDoc = pParentFrame->document();

	    HTMLFrameOwnerElement* pOwner = pFrame->ownerElement();
	    if (!pOwner)
	    {
	        break;
	    }

	    pNode = (direction == FocusDirection_Forward)
	        ? NextFocusableNodeWithDocument(pParentDoc,pOwner)
	        : PreviousFocusableNodeWithDocument(pParentDoc,pOwner);
	    pFrame = pParentFrame;
	}

	pNode = FocusableNode(direction, pNode);

	if (!pNode)
	{
	    // We didn't find a node to focus, so we should try to pass focus to Chrome.
	    if (!bInitialFocus && m_pPage->chrome()->canTakeFocus((FocusDirection)direction))
	    {
	        pDoc->setFocusedNode(0);
	        m_pPage->focusController()->setFocusedFrame(0);
	        m_pPage->chrome()->takeFocus((FocusDirection)direction);
	        return S_OK;
	    }

	    // Chrome doesn't want focus, so we should wrap focus.
	    Document* pMainDoc = m_pPage->mainFrame()->document();
	    pNode = (direction == FocusDirection_Forward)
	        ? NextFocusableNodeWithDocument(pMainDoc,0)
	        : PreviousFocusableNodeWithDocument(pMainDoc,0);

	    pNode = FocusableNode(direction, pNode);

	    if (!pNode)
	        return E_FAIL;
	}
   	// Focus wrapped around to the same node.
	if (pNode == pDoc->focusedNode())
	{
	    m_pBestNode = pNode;
	    SetNodeInfo(pNode);
	    //printf("[%s][%d] found focused node in tab key\n",__func__,__LINE__);
	    return S_OK;
	}

	if (!pNode->isElementNode())
	    return E_FAIL;

	if (pNode->isFrameOwnerElement())
	{
	   HTMLFrameOwnerElement* pOwner = static_cast<HTMLFrameOwnerElement*>(pNode);
	    if (!pOwner->contentFrame()&&!BE_IsFlashElement(pNode))
	        return E_FAIL;
	    if(!BE_IsFlashElement(pNode))
	    {
	    	pDoc->setFocusedNode(0);
	    	m_pPage->focusController()->setFocusedFrame(pOwner->contentFrame());
	    //printf("[%s][%d] tab key ready focus================\n",__func__,__LINE__);
	    	return S_OK;
	      }
	}

	FindNewDocFromNode(pNode,pDoc);
	m_pBestNode = pNode;
	SetNodeInfo(pNode);
	SetFocus(pNode);

	return S_OK;
}


Result
SpatialNavigator::CheckScrollPositionNode(Node* pFocusedNode, eFocusDirection direction, SpRect rect)
{

	if(m_nVisibleScreenTop>=0 && m_nVisibleScreenBottom>=0)
	{
		int TargetLine=0;
		int nRectBottom =RectBottom(rect);
		int nRectTop =rect.nTop;

		if(direction == FocusDirection_Down)
		{
			TargetLine = m_nVisibleScreenBottom-SCROLL_BAND;
			SPN_DISTANCE_LOG("[==============Target Line :%d,Node Bottom Line:%d,m_nVisibleScreenBottom:%d============\n",TargetLine,nRectBottom,m_nVisibleScreenBottom);
			if(nRectBottom >= TargetLine /*&& nRectBottom < m_nVisibleScreenBottom*/)
			{
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Down);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Down);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Down);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Down);
				SPN_DISTANCE_LOG("Scroll Down happen !!!!\n");
				return S_OK;
			}
			else
			{
				return E_FAIL;
			}
		}
		else if(direction == FocusDirection_Up)
		{
			TargetLine = m_nVisibleScreenTop+SCROLL_BAND;
			SPN_DISTANCE_LOG("[ ==============Target Line :%d,Node Top Line:%d,m_nVisibleScreenTop:%d============\n",TargetLine,nRectTop,m_nVisibleScreenTop);
			if(nRectTop <= TargetLine /*&& nRectTop > m_nVisibleScreenTop*/)
			{

				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Up);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Up);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Up);
				MoveJumpFocus(MOVEMENT_DISPLAY_LINES,Up);
				SPN_DISTANCE_LOG("Scroll Up happen!!!!\n");
				return S_OK;
			}
			else
			{
				return E_FAIL;
			}
		}
		else
		{
			;
		}

	}
	return E_FAIL;
}

// 20100909 : naseogi(jangjoonho@samsung.com)
void SpatialNavigator::MoveMouseFocus(Node* pNode)
{
#if 1
	Frame* coreFrame = core(webkit_web_view_get_main_frame(m_pWebView));
	if (!coreFrame)
		return;

	FrameView* view = coreFrame->view();
	if (!view)
		return;

	IntPoint scrollPos = view->scrollPosition();

	BERect HitRect = BE_GetAbsoluteNodeRect(pNode);
	int nHitCenterX = (HitRect.left+ HitRect.width/4)-scrollPos.x();
	int nHitCenterY = (HitRect.top+ HitRect.height/4)-scrollPos.y();
	nHitCenterY += 50;
#if 0
	GdkDisplay *display = NULL;
	GdkScreen *screen = NULL;

	/* get default display and screen */
	display = gdk_display_get_default ();
	screen = gdk_display_get_default_screen (display);

	/* set new cusor position */
	gdk_display_warp_pointer (display, screen, nHitCenterX, nHitCenterY);
#endif
//	printf(">>>>>>>>> naseogi success (%f) (%d, %d)\n", rate, nHitCenterX, nHitCenterY);
	m_nMouseX = nHitCenterX;
	m_nMouseY = nHitCenterY;
#endif
}

Result
SpatialNavigator::FocusPreNodeOrderAsDirection(eFocusDirection direction, Node* pPreNode)
{

	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);

	Document* focusedDocument = frame->document();
	if (!focusedDocument)
	    return E_FAIL;

	BERect rect;
	rect = BE_GetNodeRect(pPreNode);
	if(!pPreNode)
	{
		 eFocusDirection CatchDirection = FocusDirection_Forward;
	       return FocusOrderAsDocument(CatchDirection,  true);
	}
	else
	{
		if (rect.width ==0 ||rect.height ==0)
		{
		 eFocusDirection CatchDirection = FocusDirection_Forward;
	        return FocusOrderAsDocument(CatchDirection,  true);
	    	}
	}

	frame = frame->tree()->top();
	Node* pNode = NULL;
	SpRect spnRect;
	memset(&spnRect,0,sizeof(SpRect));
	BestCandidate bestCandidate;
	memset(&(bestCandidate.AbsoluteRect),0,sizeof(SpRect));
	for (Frame* CurrentFrame = frame; CurrentFrame; CurrentFrame = CurrentFrame->tree()->traverseNext(frame))
	{
		NextInDirection(CurrentFrame->document(), pPreNode, direction, bestCandidate);
	}
	pNode = bestCandidate.node;
	spnRect = bestCandidate.AbsoluteRect;
	GetVisibleScreenSize(&m_nVisibleScreenTop,&m_nVisibleScreenBottom);
	float fRate = (float) webkit_web_view_get_zoom_level(m_pWebView);
	if (!pNode ||  !pNode->isElementNode())
	{
		// Leave the focus on the same node but scroll.
		ScrollDirection scrollDirection;
		switch (direction)
		{
			case FocusDirection_Left:
				scrollDirection = ScrollLeft;
				break;
			case FocusDirection_Right:
				scrollDirection = ScrollRight;
				break;
			case FocusDirection_Up:
				scrollDirection = ScrollUp;
				break;
			case FocusDirection_Down:
				scrollDirection = ScrollDown;
				break;
			default:
			return E_FAIL;
		}
#if ENABLE(TVGG_BACKINGSTORE)
		if(fRate >=2.0f)
		{
			SPN_Scroll(scrollDirection,ScrollByLine);
			SPN_Scroll(scrollDirection,ScrollByLine);
		}
		bool bRetVal = SPN_Scroll(scrollDirection,ScrollByLine);
#else
		Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
		if(!pFrame)
		{
			return E_FAIL;
		}
		bool bRetVal =pFrame->eventHandler()->scrollRecursively(scrollDirection, ScrollByLine);
#endif
		if(scrollDirection ==ScrollUp )
		{
		  if(!bRetVal)
		  {
		  	SPN_LOG("Navigator arrives Contents Top!!!!!!!!!!!!!!!!!!!!\n");
		  	return S_JUMP_TO_OTHER_VIEW;
		  }
		}
#if ENABLE(TVGG_BACKINGSTORE)
#else
  	    SPN_ScrollRecursively(scrollDirection, ScrollByLine);
#endif
	    printf("[%s][%d]scroll .....\n",__func__,__LINE__);
	    return S_OK;
	}

	FindNewDocFromNode(pNode,focusedDocument);
	m_pBestNode = pNode;
	SetNodeInfo(pNode);
	if(0.8f<=fRate && fRate<=1.5f)
	{
		if(direction == FocusDirection_Up || direction == FocusDirection_Down)
		{
			CheckScrollPositionNode(pNode, direction, spnRect);
		}
	}
	SetFocus(pNode);

	return S_OK;
}

bool SpatialNavigator::SPN_ScrollRecursively(ScrollDirection direction, ScrollGranularity granularity)
{

	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	bool handled = frame->eventHandler()->scrollOverflow(direction, granularity);
	if (!handled)
	{
		Frame* mainFrame = m_pPage->mainFrame();
		for (Frame* CurrentFrame = mainFrame; CurrentFrame && !handled; CurrentFrame = CurrentFrame->tree()->traverseNext(frame))
		{
			FrameView* view = CurrentFrame->view();
			handled = view ? view->scroll(direction, granularity) : false;
		}
	}

    return handled;
}



bool SpatialNavigator::SPN_Scroll(ScrollDirection direction, ScrollGranularity granularity)
{
	Frame* frame = m_pPage->mainFrame();
       FrameView* view = frame->view();
       bool  handled = view ? view->scroll(direction, granularity) : false;
#if ENABLE(TVGG_BACKINGSTORE)
    if (BackingStore::isAvailable()) {
	webkit_web_view_update_scroll(const_cast<WebKitWebView*>(BackingStore::getInstance()->webView()));
    }
#endif
    return handled;
}

void SpatialNavigator::SetFocus(Node* node)
{
	JSC::JSGlobalData* g = WebCore::JSDOMWindowBase::commonJSGlobalData();
	JSC::JSGlobalObject* p = g->dynamicGlobalObject;
	g->dynamicGlobalObject = NULL;

	JSDOMGlobalObject* globalObject = node->document()->frame()->script()->globalObject(mainThreadNormalWorld());
	JSC::JSObject* jsObject = toJS(globalObject->globalExec(), globalObject, node).getObject();
	JSC::Identifier blurID(globalObject->globalExec(), "blur");
	JSC::JSValue prevBlur;
	JSC::PutPropertySlot slot;

// TODO	if(jsObject)
// TODO	{
// TODO		prevBlur = jsObject->getDirect(blurID);
// TODO		JSC::JSObject* jsDummyFunc = JSC::constructFunction(globalObject->globalExec(), JSC::ArgList(),
// TODO				JSC::Identifier(globalObject->globalExec(), ""), JSC::UString(), 0);
// TODO		jsObject->putDirect(globalObject->globalData(), blurID, jsDummyFunc, 0, false, slot);
// TODO	}

	if(BE_IsFlashElement(node) || node->hasTagName(videoTag))
	{
		Page* page = node->document()->page();
		if (page)
		{
			page->focusController()->setFocusedNode(static_cast<Element*>(node), node->document()->frame());
			Frame* frame = page->focusController()->focusedOrMainFrame();
			if(frame->view())
			{
				g_signal_emit_by_name(frame->view()->hostWindow()->platformPageClient(), "set-element-focus", node);
			}
		}
	 }
	else
	{
		if(CheckMoveOver(node))
		{
		 	SetMoveOver(node);
		}

		if(!node->hasTagName(inputTag) && !node->hasTagName(textareaTag) && hasOnFocusElement(node))
		{
			WebCore::Element* elm = (Element*) node;
			ExceptionCode ec;
			elm->setAttribute("onfocus", " ", ec);
			elm->focus(false);
		}
		else
		{
			WebCore::Element* elm = (Element*) node;
			elm->focus(false);
		}
	}

	if(jsObject)
	{
		if(prevBlur == JSC::JSValue())
		{
// TODO			jsObject->removeDirect(blurID);
		}
		else
		{
			jsObject->putDirect(globalObject->globalData(), blurID, prevBlur, 0, false, slot);
		}
	}
	g->dynamicGlobalObject = p;
}

#ifdef SMART_FOCUS_ZOOM
bool SpatialNavigator::SetSmartFocusX(SpWebView pContentsView, int x, int y) {

	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	Document* doc = frame->document();

	if(!doc)
		return false;

	WebCore::Document* realDoc = doc;
	float rate = (float) realDoc->frame()->pageZoomFactor();
	x *= rate;
	y *= rate;

	WebCore::IntPoint point(x, y);
	WebCore::HitTestRequest request(WebCore::HitTestRequest::Active);
	WebCore::HitTestResult result(point);
	realDoc->renderView()->layer()->hitTest(request, result);

	WebCore::Node* node = result.innerNode();

	SetFocus(node);

    return true;
}
#endif

Result
SpatialNavigator::FocusOrderAsDirection(eFocusDirection direction)
{
	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	SPN_ASSERT(frame);
	Document* focusedDocument = frame->document();
	if (!focusedDocument)
	    return E_FAIL;

	Node* focusedNode = focusedDocument->focusedNode();
	if(!focusedNode)
	{
		Node* pPreNode = GetFocusablePreNode();
		if(pPreNode ==NULL)
		{
	 		eFocusDirection CatchDirection = FocusDirection_Forward;
			SPN_LOG("We are finding focused Node from Begin point~~~~~~~~~~\n");
	 		return FocusOrderAsDocument(CatchDirection,  true);
		}
		else
		{
			return FocusPreNodeOrderAsDirection(direction,pPreNode);
		}
	}
	else
	{
		if(!IsSpnFocusable(focusedNode))
		{
			eFocusDirection CatchDirection = FocusDirection_Forward;
			return FocusOrderAsDocument(CatchDirection,  false);
		}
	}

	frame = frame->tree()->top();
	Node* pNode = NULL;
	SpRect spnRect;
	memset(&spnRect,0,sizeof(SpRect));
	BestCandidate bestCandidate;
	memset(&(bestCandidate.AbsoluteRect),0,sizeof(SpRect));
	for (Frame* CurrentFrame = frame; CurrentFrame; CurrentFrame = CurrentFrame->tree()->traverseNext(frame))
	{
		NextInDirection(CurrentFrame->document(), focusedNode, direction, bestCandidate);
	}
	pNode = bestCandidate.node;
	spnRect = bestCandidate.AbsoluteRect;
	GetVisibleScreenSize(&m_nVisibleScreenTop,&m_nVisibleScreenBottom);
	float fRate = (float) webkit_web_view_get_zoom_level(m_pWebView);
	if (!pNode || !pNode->isElementNode())
	{
		ScrollDirection scrollDirection;
		switch (direction)
		{
		    case FocusDirection_Left:
		        scrollDirection = ScrollLeft;
		        break;
		    case FocusDirection_Right:
		        scrollDirection = ScrollRight;
		        break;
		    case FocusDirection_Up:
		        scrollDirection = ScrollUp;
		        break;
		    case FocusDirection_Down:
		        scrollDirection = ScrollDown;
		        break;
		    default:
		        return E_FAIL;
		}
#if ENABLE(TVGG_BACKINGSTORE)
		if(fRate >=2.0f)
		{
			SPN_Scroll(scrollDirection,ScrollByLine);
			SPN_Scroll(scrollDirection,ScrollByLine);
		}
		bool bRetVal = SPN_Scroll(scrollDirection,ScrollByLine);
#else
		// Leave the focus on the same node but scroll.
		Frame* pFrame = m_pPage->focusController()->focusedOrMainFrame();
		if(!pFrame)
		{
			return E_FAIL;
		}
		bool bRetVal =pFrame->eventHandler()->scrollRecursively(scrollDirection, ScrollByLine);
#endif
		if(scrollDirection ==ScrollUp )
		{
			if(!bRetVal)
			{
				SPN_LOG("Navigator arrives Contents Top!!!!!!!!!!!!!!!!!!!!\n");
				return S_JUMP_TO_OTHER_VIEW;
			}
		}
#if ENABLE(TVGG_BACKINGSTORE)
#else
		SPN_ScrollRecursively(scrollDirection, ScrollByLine);
#endif
	   	printf("[%s][%d]scroll .....\n",__func__,__LINE__);
	   	return S_OK;
	}

	FindNewDocFromNode(pNode,focusedDocument);
	m_pBestNode = pNode;
	SetNodeInfo(pNode);
	if(0.8f<=fRate && fRate<=1.5f)
	{
		if(direction == FocusDirection_Up || direction == FocusDirection_Down)
		{
			CheckScrollPositionNode(pNode, direction,spnRect);
		}
	}
	SetFocus(pNode);
	return S_OK;
}


Node*
SpatialNavigator::FocusableNode(eFocusDirection direction, Node* pNode)
{
	while (pNode && pNode->isFrameOwnerElement())
	{
	    HTMLFrameOwnerElement* owner = static_cast<HTMLFrameOwnerElement*>(pNode);
	    if (!owner->contentFrame())
	    {
	        break;
	    }

	    Document* document = owner->contentFrame()->document();

	    pNode = (direction == FocusDirection_Forward)
	        ? document->nextFocusableNode(0, NULL)
	        : document->previousFocusableNode(0, NULL);
	    if (!pNode)
	    {
	        pNode = owner;
	        break;
	    }
	}

	return pNode;
}

void
SpatialNavigator::NextInDirection(Document* pDocument, Node* pStartNode, eFocusDirection direction, BestCandidate& bestCandidate)
{
	SPN_ASSERT(pDocument);
	SpRect rect;
	memset(&rect,0,sizeof(rect));
	// Walk all the child nodes and update bestCandidate if we find a nearer node.
	for (Node* candidate = pDocument->firstChild(); candidate; candidate = candidate->traverseNextNode())
	{
		if(candidate != pStartNode && IsSpnFocusable(candidate))
		{
			unsigned int distance = DistanceInDirection(pStartNode, candidate, pDocument, direction, &rect);
			if (distance < bestCandidate.distance)
			{
				bestCandidate.node = candidate;
				bestCandidate.distance = distance;
				bestCandidate.AbsoluteRect = rect;
			}
		}
		else
		{
			;
		}
	}
}


#ifdef SMART_FOCUS_ZOOM
void
SpatialNavigator::NextInAllDirection(Document* pDocument, const SpPoint* pSrcPoint, BestCandidate& bestCandidate, bool isZoom)
{
	SPN_ASSERT(pDocument);
	SpRect rect;
	memset(&rect,0,sizeof(rect));
	// Walk all the child nodes and update bestCandidate if we find a nearer node.
	for (Node* candidate = pDocument->firstChild(); candidate; candidate = candidate->traverseNextNode())
	{
		if(isZoom)
		{
			 if(/*candidate != pStartNode && */IsSpnFocusableForSmartZoom(candidate)&&!candidate->hasTagName(iframeTag)&& !candidate->hasTagName(frameTag)&& !candidate->hasTagName(htmlTag) )
			{
			    unsigned int distance = GetEuclideanDistance(pSrcPoint, candidate, pDocument,&rect );

				if (distance <= bestCandidate.distance)
			    {
					bestCandidate.node = candidate;
					bestCandidate.distance = distance;
					bestCandidate.AbsoluteRect = rect;
			    }
			}
		}
		else
		{
			if(/*candidate != pStartNode && */IsSpnFocusable(candidate)&&!candidate->hasTagName(iframeTag)&& !candidate->hasTagName(frameTag)&& !candidate->hasTagName(htmlTag) )
			{
			    unsigned int distance = GetEuclideanDistance(pSrcPoint, candidate, pDocument,&rect );

				if (distance <= bestCandidate.distance)
			    {
					bestCandidate.node = candidate;
					bestCandidate.distance = distance;
					bestCandidate.AbsoluteRect = rect;
			    }
			}

		}
	 }

}
#endif

Node*
SpatialNavigator::GetNodeFromPoint(int nX, int nY, Document* pDoc)
{
	Frame* frame = m_pPage->focusController()->focusedOrMainFrame();
	if (!frame)
		return NULL;
	if(!pDoc )
		return NULL;

	IntPoint point (nX ,nY);
	HitTestRequest request(HitTestRequest::ReadOnly | HitTestRequest::Active);
	HitTestResult result(point);
	RenderView* pRenderView = pDoc->renderView();
	if(pRenderView)
	{
		pRenderView->layer()->hitTest(request, result);
	}
	else
	{
		SPN_LOG("No RenderView****************\n");
		return NULL;
	}
	Node* target = result.innerNode();
	while (target && !IsSpnFocusable(target))
	    target = target->parentNode();
	return target;

}

void
SpatialNavigator::GetPreNodeInfo(int* pHitX,int* pHitY)
{
	if(pHitX)
		*pHitX =gPreNodeInfo.nCenterX;

	if(pHitY)
		*pHitY= gPreNodeInfo.nCenterY;

	//printf("[%s][%d]----HitPosition(%d,%d)\n",__func__,__LINE__,*pHitX,*pHitY);
	return ;
}


Node*
SpatialNavigator::HitTestFromPoint( int nX, int nY,Node*pDestNode,Document* pDoc)
{
	const IntPoint point(nX,nY);
	HitTestResult result = pDoc->frame()->eventHandler()->hitTestResultAtPoint(point,  false);
	Node* target = result.innerNode();

	while (target && !IsSpnFocusable(target))
	    target = target->parentNode();
	return target;
}

Node*
SpatialNavigator::HitTestFromPoint2( int nX, int nY,Node*pDestNode,Document* pDoc)
{
	IntPoint point (nX,nY);
	HitTestRequest request( HitTestRequest::ReadOnly | HitTestRequest::MouseMove);
	HitTestResult result(point);
	RenderView* pRenderView = pDoc->renderView();
	if(pRenderView)
	{
		pRenderView->layer()->hitTest(request, result);
	}
	else
	{
		return NULL;
	}

	Node* pHitNode = result.innerNode();
	while (pHitNode &&  pHitNode->nodeName().utf8() != pDestNode->nodeName().utf8())
	{
	    	pHitNode = pHitNode->parentNode();
	}

	return pHitNode;
}


bool
SpatialNavigator::VisibleHitCheck(Node* pDestNode, Document* pDoc)
{

	if(!pDestNode|| !pDoc)
		return false;

	if(pDestNode->hasTagName(areaTag))
	{

		Node* pTmp = (Node*) ((HTMLAreaElement*)pDestNode)->imageElement();
		if(!pTmp)
		{
			return false;
		}
		BERect DestRect = BE_GetNodeRect(pDestNode);
		int nDestCenterX = DestRect.left + DestRect.width/2;
		int nDestCenterY = DestRect.top + DestRect.height/2;
		Node* pHitNode =HitTestFromPoint2(nDestCenterX,nDestCenterY,pDestNode,pDoc);
		if(!pHitNode)
		{
			return false;
		}
		else
		{
			BERect HitRect = BE_GetNodeRect(pHitNode);
			int nHitCenterX = HitRect.left+ HitRect.width/2;
			int nHitCenterY = HitRect.top+ HitRect.height/2;
			if(nHitCenterX==nDestCenterX && nHitCenterY==nDestCenterY)
			{
				return true;
			}
			return false;
		}
	}

	BERect DestRect = BE_GetNodeRect(pDestNode);
	if(DestRect.width<=1 ||DestRect.height<=1)// ignore small element
		return false;
	int nDestCenterX = DestRect.left + DestRect.width/2;
	int nDestCenterY = DestRect.top + DestRect.height/2;
	bool textlink = false;
	unsigned int loopCount =0;
	Node* pTmpNode = pDestNode->firstChild();
	while(pTmpNode && loopCount<3)
	{
		if(pTmpNode->nodeType()==3)
		{
			textlink = true;
			break;

		}
		loopCount++;
		 pTmpNode = pTmpNode->traverseNextNode(pDestNode);
	}

	if(textlink)
	{
		nDestCenterX = DestRect.left + DestRect.width/2;
		nDestCenterY = DestRect.top + DestRect.height/3;
	}

	Node* HitResultNode = HitTestFromPoint(nDestCenterX, nDestCenterY, pDestNode, pDoc);
	if(!HitResultNode || ! HitResultNode->renderer()||!HitResultNode->renderer()->enclosingLayer())
	{
		return false;
	}

	RenderObject* pDestRenderObject = pDestNode->renderer();
	if(!VisibleSyleCheck( pDestRenderObject, pDoc))
		return false;
	BERect HitRect = BE_GetNodeRect(HitResultNode);
	int nHitCenterX = HitRect.left+ HitRect.width/2;
	int nHitCenterY = HitRect.top+ HitRect.height/2;
	if(textlink == true)
	{
		nHitCenterX = HitRect.left+ HitRect.width/2;
		nHitCenterY = HitRect.top+ HitRect.height/3;
	}

	if(nHitCenterX==nDestCenterX && nHitCenterY==nDestCenterY)
	{
		WebCore::Frame* coreFrame = WebKit::core(m_pWebView)->mainFrame();
		BERect DestRect2 = BE_GetAbsoluteNodeRect(pDestNode);
		int nDestCenterX2 = DestRect2.left + DestRect2.width/2;
		int nDestCenterY2 = DestRect2.top + DestRect2.height/2;
		if(textlink == true)
		{
		 	nDestCenterX2 = DestRect2.left + DestRect2.width/2;
			nDestCenterY2 = DestRect2.top + DestRect2.height/3;
		}
		Node* HitResultNode  =  HitTestFromPoint(nDestCenterX2, nDestCenterY2, pDestNode, coreFrame->document());
		if(!HitResultNode || ! HitResultNode->renderer()||!HitResultNode->renderer()->enclosingLayer())
		{
			return false;
		}

		BERect HitRect2 = BE_GetAbsoluteNodeRect(HitResultNode);
		int nHitCenterX2 = HitRect2.left+ HitRect2.width/2;
		int nHitCenterY2 = HitRect2.top+ HitRect2.height/2;
		if(textlink == true)
		{
			nHitCenterX2 = HitRect2.left+ HitRect2.width/2;
			nHitCenterY2 = HitRect2.top+ HitRect2.height/3;
		}
		if(nHitCenterX2!=nDestCenterX2 && nHitCenterY2!=nDestCenterY2)
		{
			SPN_DISTANCE_LOG("[Fail XXXHittable!!!HitResultNode (cx,cy):(%d,%d)(%s) <->DestNode (cx,cy):(%d,%d)(%s)]\n", nHitCenterX2,nHitCenterY2,HitResultNode->nodeName().utf8().data(),nDestCenterX2, nDestCenterY2, pDestNode->nodeName().utf8().data());
			return false;
		}

	}
	else
	{
		return false;
	}

	return true;
}

bool
SpatialNavigator::VisibleSyleCheck( RenderObject* pDestRenderObject,Document* pDoc)
{
	Node* owner = pDoc->ownerElement();
	if(owner)
	{
		if (owner->hasTagName(iframeTag)||owner->hasTagName(divTag)||owner->hasTagName(frameTag))
		{
			RenderObject* targetRender = owner->renderer();
			if(!targetRender)
				return false;
			if(targetRender->style())
			{
				if(targetRender->style()->visibility()!=VISIBLE || targetRender->style()->display()==NONE)
				{
					return false;
				}
			}
			//printf("[%s,%d]owner is iframe <<<<<<<<<<<<<<<<<<< display:%d \n", __func__, __LINE__, targetRender->style()->display());
		}
	}
	if(!pDestRenderObject)
	{
		return false;
	}
	if(pDestRenderObject->style())
	{
		if(pDestRenderObject->style()->visibility()!=VISIBLE ||pDestRenderObject->style()->display()==NONE)
		{
			return false;
		}
	}
	return true;
}

unsigned int
SpatialNavigator::DistanceInDirection(Node* pStartNode, Node* pDestNode, Document* pDoc,eFocusDirection direction,SpRect* pSpnRect)
{
/*	RenderObject* pStartRender = pStartNode->renderer();
	if (pStartRender == NULL)
		return numeric_limits<unsigned int>::max();
*/
	RenderObject* pDestRender = pDestNode->renderer();
	if (pDestRender == NULL && !pDestNode->hasTagName(areaTag))
		return numeric_limits<unsigned int>::max();

	BERect StartRect = BE_GetAbsoluteNodeRect(pStartNode);
	IntRect startIntRect(StartRect.left, StartRect.top, StartRect.width, StartRect.height);
	BERect DestRect = BE_GetAbsoluteNodeRect(pDestNode);
	IntRect destIntRect(DestRect.left, DestRect.top, DestRect.width, DestRect.height);

	if(IsOffScreenRect(pDestNode,destIntRect))
	{
		return numeric_limits<unsigned int>::max();
	}

	//negative coordinates
	if (!CheckNegativeCoordinates(pDestRender, DestRect))
	{
		SPN_DISTANCE_LOG("DestRender negativecoordinate !!!\n");
		return numeric_limits<unsigned int>::max();
	}

	SpPoint CurrentStartPoint;
	memset(&CurrentStartPoint,0,sizeof(SpPoint));

	DecesionStartPoint(&CurrentStartPoint, direction, StartRect );

	if (!IsRectInDirection(direction, CurrentStartPoint, DestRect))
	{
		SPN_DISTANCE_LOG("not RectInDireciton !!!\n");
		return numeric_limits<unsigned int>::max();
	}

	if (IsEceptionCase(direction, CurrentStartPoint, StartRect,DestRect))
	{
    		return numeric_limits<unsigned int>::max();
	}

#ifdef __SPN_EYE_HIT__
	if(!VisibleHitCheck(pDestNode, pDoc))
	{
		return numeric_limits<unsigned int>::max();
	}
#endif
	/*
	printf("[%s][%d]startIntRect offset move rect (%d,%d)(%d,%d)(%s)\n",__func__,__LINE__,startIntRect.x(),startIntRect.y(), startIntRect.width(), startIntRect.height(),pStartNode->nodeName().utf8().data());
	printf("[%s][%d]destIntRect offset move rect (%d,%d)(%d,%d)(%s)\n\n",__func__,__LINE__,destIntRect.x(),destIntRect.y(), destIntRect.width(), destIntRect.height(),pDestNode->nodeName().utf8().data());
	*/
	if(pSpnRect)
	{
		pSpnRect->nLeft = DestRect.left;
		pSpnRect->nTop = DestRect.top;
		pSpnRect->nHeight = DestRect.height;
		pSpnRect->nWidth = DestRect.width ;
	}
	unsigned int nDotDistance =0;
	int nDx=0 , nDy=0;
	int nXdisplacement=0 ,nYdisplacement=0;
	unsigned int nOverlap=0;
	if(IsInDirectionLineofFocusPoint( direction, CurrentStartPoint, DestRect, StartRect))
	{
		nDotDistance=0;
	}
	else
	{
		nDotDistance = CalcDotDistance( direction, CurrentStartPoint, DestRect);
		if(nDotDistance!=0)
		{
			SPN_DISTANCE_LOG("Not In Direction Line of FocusPoint === Update StartPoint Success\n");
		}
	}
	CalcDxDy( direction, StartRect, DestRect, &nDx,  &nDy);

	CalcDisplacement( direction,StartRect, DestRect,&nXdisplacement,&nYdisplacement);

	if(IsOverlaped())
	{
		CalcOverlap(&nOverlap);
	}
	return TotalDistanceFunc( nDotDistance,  nDx, nDy,  nXdisplacement,  nYdisplacement, nOverlap);
}


}// namespace WebCore
