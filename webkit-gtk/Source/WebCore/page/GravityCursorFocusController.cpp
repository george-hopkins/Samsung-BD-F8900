#include "config.h"
#include "FocusController.h"

#include "AXObjectCache.h"
#include "Chrome.h"
#include "Document.h"
#include "Editor.h"
#include "EditorClient.h"
#include "Element.h"
#include "Event.h"
#include "EventHandler.h"
#include "EventNames.h"
#include "ExceptionCode.h"
#include "Frame.h"
#include "FrameSelection.h"
#include "FrameTree.h"
#include "FrameView.h"
#include "HTMLAreaElement.h"
#include "HTMLImageElement.h"
#include "HTMLNames.h"
#include "HitTestResult.h"
#include "KeyboardEvent.h"
#include "Page.h"
#include "Range.h"
#include "RenderLayer.h"
#include "RenderObject.h"
#include "RenderWidget.h"
#include "ScrollAnimator.h"
#include "Settings.h"
#include "ShadowRoot.h"
#include "SpatialNavigation.h"
#include "Widget.h"
#include "htmlediting.h" // For firstPositionInOrBeforeNode
#include <limits>

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
#include "RenderView.h"
#endif
namespace WebCore {

#define MAXROPESIZE 250
#define MAXROPESIZE_D 62500

static Vector<IntRect> gRectNodes;
static FocusController* gpHandle = NULL;
static int getPowDistance(int x, int y, int px, int py);
static int collisionDetection(IntRect rt, int r, int x, int y);
static IntRect findFocusableCandidateInStoredVector(const IntPoint& point, Frame* mainFrame, bool ignoreScrollbarRect);
static int storeFocusableCandidate(FocusCandidate candidate);
static void updateFocusableCandidateInContainer(Node* container);
static int clearFocusableNodes();
static int updateFocusableNodes(FocusController* pHandle, Frame* mainFrame);
static IntRect findFocusableHitTest(FocusController* pHandle, const IntPoint& point, Frame* mainFrame, bool ignoreScrollbarRect);
static bool isAbleCandidate(FocusCandidate candidate);

static int getPowDistance(int x, int y, int px, int py)
{
	return (x-px) * (x-px) + (y-py) * (y-py);	
}

static int collisionDetection(IntRect rt, int r, int x, int y)
{
	int nLeft = x - r;
	int nRight = x + r;
	int nTop = y - r;
	int nBottom = y + r;
	int nDirection = 0;
	
	int nDistance = r*r;
	
	if((rt.x()+rt.width()) < nLeft || rt.x() > nRight || (rt.y()+rt.height()) < nTop || rt.y() > nBottom)
	{
		return nDistance;
	}
	
	if(x < rt.x())					nDirection |=  0x001;
	else if(x > rt.x() + rt.width())	nDirection |=  0x002;
	if(y < rt.y())					nDirection |=  0x010;
	else if(y > rt.y() + rt.height())	nDirection |=  0x020;

	switch(nDirection)
	{
		case 0x001:	// left
			nDistance = getPowDistance(x, y, rt.x(), y);
			break;
		case 0x002:	// right
			nDistance = getPowDistance(x, y, rt.x() + rt.width(), y);
			break;
		case 0x010:	// top
			nDistance = getPowDistance(x, y, x, rt.y());
			break;
		case 0x020:	// bottom
			nDistance = getPowDistance(x, y, x, rt.y()+rt.height());
			break;
		case 0x011:	// left top
			nDistance = getPowDistance(x, y, rt.x(), rt.y());
			break;
		case 0x012:	// right top
			nDistance = getPowDistance(x, y, rt.x() + rt.width(), rt.y());
			break;
		case 0x021:	// left bottom
			nDistance = getPowDistance(x, y, rt.x(), rt.y()+rt.height());
			break;
		case 0x022:	// right bottom
			nDistance = getPowDistance(x, y, rt.x() + rt.width(), rt.y()+rt.height());
			break;
		case 0:	// center
			//printf("inbound\n");
			return -1;
	}

	return nDistance;
}

static IntRect findFocusableHitTest(FocusController* pHandle, const IntPoint& point, Frame* mainFrame, bool ignoreScrollbarRect)
{
	Document* document = mainFrame->document();
	document->updateLayoutIgnorePendingStylesheets();

	IntPoint scrolledPoint = mainFrame->view()->windowToContents(point);

	HitTestResult result = mainFrame->page()->mainFrame()->eventHandler()->hitTestResultAtPoint(scrolledPoint, false, false, ShouldHitTestScrollbars);
	Element* element = FocusController::getFocusableElement(result.innerNode());
	
	if (element)
	{
		return pHandle->getFocusRect(element);
	}

	return IntRect();
}

static IntRect findFocusableCandidateInStoredVector(const IntPoint& point, Frame* mainFrame, bool ignoreScrollbarRect)
{
	int nResult = 0; 
	int nBestResult = MAXROPESIZE_D;
	int nBestIndex = 0;

	HitTestResult result = mainFrame->page()->mainFrame()->eventHandler()->hitTestResultAtPoint(point, false, false, ShouldHitTestScrollbars);
	if (ignoreScrollbarRect && (result.scrollbar() || mainFrame->view()->scrollbarAtPoint(point)))
	{
	//	printf("\nscroll in\n");
		return IntRect();
	}

	int rectCount = (int)(gRectNodes.size());

	for (int i = 0; i < rectCount; i++) {
		nResult = collisionDetection(gRectNodes[i], MAXROPESIZE, point.x(), point.y());
		if(nBestResult > nResult)
		{
			nBestResult = nResult;
			nBestIndex = i;
			if(nBestResult == -1)
				break;
		}
	}
	
	if(nBestResult < MAXROPESIZE_D)
	{
		return gRectNodes[nBestIndex];
	}
	
	return IntRect();
}

static int storeFocusableCandidate(FocusCandidate candidate)
{
	Element* element = toElement(candidate.focusableNode);

	if(element != NULL)
	{
		gRectNodes.append(gpHandle->getFocusRect(element));
	}
	return 0;
}

static bool isAbleCandidate(FocusCandidate candidate)
{
	 // Ignore iframes that don't have a src attribute
    if (frameOwnerElement(candidate) && (!frameOwnerElement(candidate)->contentFrame() || candidate.rect.isEmpty()))
        return false;

    // Ignore off screen child nodes of containers that do not scroll (overflow:hidden)
    if (candidate.isOffscreen)
        return false;

//#if ENABLE(TVGG_SPATIAL_NAVIGATION)
	if (!FocusController::isTopmostNode(candidate))
		return false;
//#endif

	return true;
}

static void updateFocusableCandidateInContainer(Node* container)
{
	Node* node = container->firstChild();

	for (; node; node = (node->isFrameOwnerElement()) ? node->traverseNextSibling(container) : node->traverseNextNode(container)) {
		if (!node->isElementNode())
            continue;

        if (!node->isKeyboardFocusable(0) && !node->isFrameOwnerElement())
            continue;

        FocusCandidate candidate = FocusCandidate(node, FocusDirectionNone);
        if (candidate.isNull())
            continue;

		if(isAbleCandidate(candidate)) {
			if (HTMLFrameOwnerElement* frameElement = frameOwnerElement(candidate)) {
				frameElement->contentFrame()->document()->updateLayoutIgnorePendingStylesheets();
				updateFocusableCandidateInContainer(frameElement->contentFrame()->document());
			}
			else
			{
				storeFocusableCandidate(candidate);
			}
		}
	}
}
static int clearFocusableNodes()
{
	gRectNodes.clear();
	return (int)(gRectNodes.size());
}
static int updateFocusableNodes(FocusController* pHandle, Frame* mainFrame)
{
	Document* document = mainFrame->document();
	document->updateLayoutIgnorePendingStylesheets();
	gpHandle = pHandle;
	gRectNodes.clear();
	updateFocusableCandidateInContainer(mainFrame->document());
	return 0;
}
//*/
IntRect FocusController::nearestFocusableNodeRectByGravityCursor(const String& command, const IntPoint& point)
{
	int nResult = 0; 
	Frame* mainFrame = m_page->mainFrame();

	if(command == "0")
	{
		nResult = updateFocusableNodes(this, mainFrame);
	}
	else if(command == "1")
	{
		return findFocusableCandidateInStoredVector(point, mainFrame, true);
	}
	else if(command == "2")
	{
		nResult = clearFocusableNodes();
	}
	else if(command == "3")
	{
		return findFocusableHitTest(this, point, mainFrame, true);
	}
	
	return IntRect();
}

} // namespace WebCore
