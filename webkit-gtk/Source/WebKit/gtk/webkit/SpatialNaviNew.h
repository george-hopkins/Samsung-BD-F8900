/**
 * Copyright 2010 by Samsung Electronics, Inc.,
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.

 * @file       SpatialNavi.h
 * @brief       Declaration of wrt api that webkit depends on
 * @autor       HyunGon Yang
 * @date        2010.06.03
 */

#ifndef _SPATIAL_NAVINEW_H
#define _SPATIAL_NAVINEW_H

#include "FocusController.h"
#include "SpatialNaviNewEx.h"


namespace WebCore {

	using namespace std;
    class Page;
	typedef enum
	{
	  MOVEMENT_DISPLAY_LINES=0,
	  MOVEMENT_PAGES,	         	    /* move by pages */
	  MOVEMENT_BUFFER_ENDS,       /* move to ends of the buffer */
	  MOVEMENT_MAX
	} eMovementStep;

	typedef enum {
		Up=0,
		Down,
		Left,
		Right
	}eJumpDirection;

	typedef struct _SpRect
	{
		int nLeft;
		int nTop;
		int nWidth;
		int nHeight;
	} SpRect;

	typedef struct _SpPoint
	{
		int nX;
		int nY;
	}SpPoint;

	typedef struct _SpNodePositon
	{
		int nCenterX;
		int nCenterY;
	}SpNodePostion;

	struct BestCandidate {
	    BestCandidate()
	        : node(0)
	        , distance(numeric_limits<unsigned int>::max())
	    {
	    }
	    Node* node;
	    unsigned int distance;
	    SpRect AbsoluteRect;
	};
	class SpatialNavigator
	{
	public:
	    SpatialNavigator(Page* page,WebKitWebView* pWebView)
		{
			m_pPage = page;
			m_pWebView = pWebView;
			m_pBestNode= NULL;
			m_nVisibleScreenTop=0;
			m_nVisibleScreenBottom=0;
			#ifdef SMART_FOCUS_ZOOM
			m_nArea = 0;
			m_pSmartZoomParent = NULL;
			m_pSmartFocusBestNode = NULL;
			#endif
		}

		static Result CreateInstance(Page* page, WebKitWebView* pWebView)
		{
			if(pInstance_ == NULL)
				pInstance_ = new SpatialNavigator(page, pWebView);
			return S_OK;
		}
		static SpatialNavigator* GetSpatialNavigator()
		{
			return pInstance_;
		}

		static Result DestoryInstance(void)
		{
			if(pInstance_)
			{
				delete pInstance_;
				pInstance_ = NULL;
			}
			return S_OK;
		}

	 	Result SpatialFocus(eFocusDirection, bool initialFocus = false);
		Result JumpFocus(eFocusDirection);
		void* GetNextNode(void);
		Document* FindNewDocFromNode(Node* node,Document* pFocusedDoc);
		Document* GetFocusedDoc(void);
		void SetNodeInfo(Node*);
		void SetNoneCusor(void);
		void* GetCurrentFocusedNode(void);
		void GetPreNodeInfo(int* pHitX,int* pHitY);
		Node* GetNodeFromPoint(int nX, int nY, Document* pDoc);
		bool SetMoveOver(Node* );
		void SetFocus(Node* node);
		bool VisibleHitCheck(Node*pDestNode, Document* pDoc);
		bool IsSpnFocusable(Node* pNode);
#ifdef SMART_FOCUS_ZOOM
		void SetFocusOnNode(SpWebView pContentsView, int x, int y);
		void SetSmartZoom(SpWebView pContentsView, int x, int y);
		bool SetSmartFocusX(SpWebView pContentsView, int x, int y);
#endif
	private:
		Result FocusOrderAsDirection(eFocusDirection);
		Result FocusOrderAsDocument(eFocusDirection,bool initialFocus);
		void MoveMouseFocus(Node* pNode);
		Result FocusPreNodeOrderAsDirection(eFocusDirection , Node* );
		Result MoveJumpFocus(eMovementStep , eJumpDirection );
		Node* FocusableNode(eFocusDirection direction, Node* node);		
		bool VisibleSyleCheck( RenderObject* pDestRenderObject,Document* pDoc);
		unsigned int DistanceInDirection(Node*, Node*, Document*,eFocusDirection,  SpRect*);
		void NextInDirection(Document*, Node*, eFocusDirection, BestCandidate&);
		Node* HitTestFromPoint( int ,int,Node*,Document*);
		Node* HitTestFromPoint2( int nX, int nY,Node*,Document*);
		bool LockMoveOver( const PlatformMouseEvent&, Node* );
		void GetMotionRootPosition(GtkWidget* , int* , int* );
		void ContentsToWindowPos( int* , int* );
		Node* GetFocusablePreNode(void );
		Result GetVisibleScreenSize(int * width, int * height);
		Result CheckScrollPositionNode(Node* pFocusedNode,eFocusDirection direction, SpRect rect);
		bool SPN_ScrollRecursively(ScrollDirection direction, ScrollGranularity granularity);
		bool SPN_Scroll(ScrollDirection direction, ScrollGranularity granularity);
		bool IsOffScreenRect(Node* pNode,IntRect DestRect );

		int m_nVisibleScreenTop;
		int m_nVisibleScreenBottom;
	    	Page* m_pPage;
		WebKitWebView* m_pWebView;
		Node* m_pBestNode;

		static SpatialNavigator* pInstance_;
#ifdef SMART_FOCUS_ZOOM
		/** ------- Fields for Smart focus starts here ---- **/
		Node* m_pSmartFocusBestNode;
		int m_nArea;

		Node* m_pSmartZoomParent;
                bool RecursiveNodeFetch(Node* pNode, int x, int y);
		Result GetNearestFocusableNode(const SpPoint* srcPoint, bool isZoom);

		void GetDestinationNodePoint(const SpPoint* pSrcPoint, Node* pDestNode, SpPoint* pDestPoint);

		unsigned int GetEuclideanDistance(const SpPoint* pSrcPoint, Node* pDestNode, Document* pDoc, SpRect* pSpnRect );

		void NextInAllDirection(Document* pDocument, const SpPoint* pSrcPoint, BestCandidate& bestCandidate, bool isZoom);

		/** ------- Smart focus fields ends---- **/
#endif
	};


}



#endif
//_SPATIAL_NAVI_H



