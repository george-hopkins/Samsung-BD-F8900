/*
 * Copyright (C) 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nuanti Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

using namespace HTMLNames;
using namespace std;

static inline void dispatchEventsOnWindowAndFocusedNode(Document* document, bool focused)
{
    // If we have a focused node we should dispatch blur on it before we blur the window.
    // If we have a focused node we should dispatch focus on it after we focus the window.
    // https://bugs.webkit.org/show_bug.cgi?id=27105

    // Do not fire events while modal dialogs are up.  See https://bugs.webkit.org/show_bug.cgi?id=33962
    if (Page* page = document->page()) {
        if (page->defersLoading())
            return;
    }

    if (!focused && document->focusedNode())
        document->focusedNode()->dispatchBlurEvent();
    document->dispatchWindowEvent(Event::create(focused ? eventNames().focusEvent : eventNames().blurEvent, false, false));
    if (focused && document->focusedNode())
        document->focusedNode()->dispatchFocusEvent();
}

FocusController::FocusController(Page* page)
    : m_page(page)
    , m_isActive(false)
    , m_isFocused(false)
    , m_isChangingFocusedFrame(false)
{
}

void FocusController::setFocusedFrame(PassRefPtr<Frame> frame)
{
    ASSERT(!frame || frame->page() == m_page);
    if (m_focusedFrame == frame || m_isChangingFocusedFrame)
        return;

    m_isChangingFocusedFrame = true;

    RefPtr<Frame> oldFrame = m_focusedFrame;
    RefPtr<Frame> newFrame = frame;

    m_focusedFrame = newFrame;

    // Now that the frame is updated, fire events and update the selection focused states of both frames.
    if (oldFrame && oldFrame->view()) {
        oldFrame->selection()->setFocused(false);
        oldFrame->document()->dispatchWindowEvent(Event::create(eventNames().blurEvent, false, false));
    }

    if (newFrame && newFrame->view() && isFocused()) {
        newFrame->selection()->setFocused(true);
        newFrame->document()->dispatchWindowEvent(Event::create(eventNames().focusEvent, false, false));
    }

    m_page->chrome()->focusedFrameChanged(newFrame.get());

    m_isChangingFocusedFrame = false;
}

Frame* FocusController::focusedOrMainFrame() const
{
    if (Frame* frame = focusedFrame())
        return frame;
    return m_page->mainFrame();
}

void FocusController::setFocused(bool focused)
{
    if (isFocused() == focused)
        return;

    m_isFocused = focused;

    if (!m_isFocused)
        focusedOrMainFrame()->eventHandler()->stopAutoscrollTimer();

    if (!m_focusedFrame)
        setFocusedFrame(m_page->mainFrame());

    if (m_focusedFrame->view()) {
        m_focusedFrame->selection()->setFocused(focused);
        dispatchEventsOnWindowAndFocusedNode(m_focusedFrame->document(), focused);
    }
}

inline static ShadowRoot* shadowRoot(Node* node)
{
    return node->isElementNode() ? toElement(node)->shadowRoot() : 0;
}

inline static bool isTreeScopeOwner(Node* node)
{
    return node && (node->isFrameOwnerElement() || shadowRoot(node));
}

Node* FocusController::deepFocusableNode(FocusDirection direction, Node* node, KeyboardEvent* event)
{
    // The node we found might be a HTMLFrameOwnerElement or a shadow host, so descend down the tree until we find either:
    // 1) a focusable node, or
    // 2) the deepest-nested HTMLFrameOwnerElement or shadow host.
    while (isTreeScopeOwner(node)) {
        Node* foundNode;
        if (node->isFrameOwnerElement()) {
            HTMLFrameOwnerElement* owner = static_cast<HTMLFrameOwnerElement*>(node);
            if (!owner->contentFrame())
                break;
            Document* document = owner->contentFrame()->document();
            foundNode = findFocusableNode(direction, document, 0, event);
        } else {
            ASSERT(shadowRoot(node));
            // FIXME: Some elements (e.g. HTMLInputElement and HTMLTextAreaElement) do extra work in their focus() methods.
            // Skipping these elements is the safest fix until we find a better way.
            if (node->hasTagName(inputTag) || node->hasTagName(textareaTag))
                break;
            foundNode = findFocusableNode(direction, shadowRoot(node), 0, event);
        }
        if (!foundNode)
            break;
        ASSERT(node != foundNode);
        node = foundNode;
    }
    return node;
}

bool FocusController::setInitialFocus(FocusDirection direction, KeyboardEvent* event)
{
    bool didAdvanceFocus = advanceFocus(direction, event, true);

    // If focus is being set initially, accessibility needs to be informed that system focus has moved
    // into the web area again, even if focus did not change within WebCore. PostNotification is called instead
    // of handleFocusedUIElementChanged, because this will send the notification even if the element is the same.
    if (AXObjectCache::accessibilityEnabled())
        focusedOrMainFrame()->document()->axObjectCache()->postNotification(focusedOrMainFrame()->document()->renderer(), AXObjectCache::AXFocusedUIElementChanged, true);

    return didAdvanceFocus;
}

bool FocusController::advanceFocus(FocusDirection direction, KeyboardEvent* event, bool initialFocus)
{
    switch (direction) {
    case FocusDirectionForward:
    case FocusDirectionBackward:
        return advanceFocusInDocumentOrder(direction, event, initialFocus);
    case FocusDirectionLeft:
    case FocusDirectionRight:
    case FocusDirectionUp:
    case FocusDirectionDown:
        return advanceFocusDirectionally(direction, event);
    default:
        ASSERT_NOT_REACHED();
    }

    return false;
}

#ifdef ENABLE_HBBTV
bool FocusController::handleCSSFocusNavigation(FocusDirection direction)
{
    Frame* curFrame = focusedOrMainFrame();
    ASSERT(curFrame);

    Document* focusedDocument = curFrame->document();
    if (!focusedDocument)
        return false;

    Node* focusedNode = focusedDocument->focusedNode();
    RenderObject* renderer = focusedNode ? focusedNode->renderer() : 0;

    if (!renderer)
        return false;

    StyleNavigationValue value;
    switch (direction) {
    case FocusDirectionForward:
    case FocusDirectionBackward:
        return false;
    case FocusDirectionUp:
        value = renderer->style()->navUp();
        break;
    case FocusDirectionDown:
        value = renderer->style()->navDown();
        break;
    case FocusDirectionLeft:
        value = renderer->style()->navLeft();
        break;
    case FocusDirectionRight:
        value = renderer->style()->navRight();
        break;
    case FocusDirectionNone:
    default:
        ASSERT_NOT_REACHED();
        break;
    }

    if (value.id().isNull() || value.isAuto())
        return false;

    const AtomicString& target = value.target();
    Frame* targetFrame = 0;

    // If we were in the autoscroll/panScroll mode we want to stop it.
    curFrame->eventHandler()->stopAutoscrollTimer();

    if (target == "current")
        targetFrame = curFrame;
    else if (target == "root")
        targetFrame = curFrame->tree()->top();
    else
        targetFrame = curFrame->tree()->find(target);

    // TODO:// which one is right?
    if (!targetFrame)
        return false; // targetFrame = curFrame;

    Element* anchorNode = targetFrame->document()->findAnchor(value.id().string());
    if (!anchorNode)
        return false;
    // if it's same with the current focused, it should be consumed.
    if (focusedNode == anchorNode)
        return true;

    anchorNode->scrollIntoViewIfNeeded(false);

    setFocusedNode(anchorNode, targetFrame);

    return true;
}
#endif

bool FocusController::advanceFocusInDocumentOrder(FocusDirection direction, KeyboardEvent* event, bool initialFocus)
{
    Frame* frame = focusedOrMainFrame();
    ASSERT(frame);
    Document* document = frame->document();

    Node* currentNode = document->focusedNode();
    // FIXME: Not quite correct when it comes to focus transitions leaving/entering the WebView itself
    bool caretBrowsing = focusedOrMainFrame()->settings()->caretBrowsingEnabled();

    if (caretBrowsing && !currentNode)
        currentNode = frame->selection()->start().deprecatedNode();

    document->updateLayoutIgnorePendingStylesheets();

    Node* node = findFocusableNodeAcrossTreeScope(direction, currentNode ? currentNode->treeScope() : document, currentNode, event);

    if (!node) {
        // We didn't find a node to focus, so we should try to pass focus to Chrome.
        if (!initialFocus && m_page->chrome()->canTakeFocus(direction)) {
            document->setFocusedNode(0);
            setFocusedFrame(0);
            m_page->chrome()->takeFocus(direction);
            return true;
        }

        // Chrome doesn't want focus, so we should wrap focus.
        node = findFocusableNode(direction, m_page->mainFrame()->document(), 0, event);
        node = deepFocusableNode(direction, node, event);

        if (!node)
            return false;
    }

    ASSERT(node);

    if (node == document->focusedNode())
        // Focus wrapped around to the same node.
        return true;

    if (!node->isElementNode())
        // FIXME: May need a way to focus a document here.
        return false;

    if (node->isFrameOwnerElement()) {
        // We focus frames rather than frame owners.
        // FIXME: We should not focus frames that have no scrollbars, as focusing them isn't useful to the user.
        HTMLFrameOwnerElement* owner = static_cast<HTMLFrameOwnerElement*>(node);
        if (!owner->contentFrame())
            return false;

        document->setFocusedNode(0);
        setFocusedFrame(owner->contentFrame());
        return true;
    }

    // FIXME: It would be nice to just be able to call setFocusedNode(node) here, but we can't do
    // that because some elements (e.g. HTMLInputElement and HTMLTextAreaElement) do extra work in
    // their focus() methods.

    Document* newDocument = node->document();

    if (newDocument != document)
        // Focus is going away from this document, so clear the focused node.
        document->setFocusedNode(0);

    if (newDocument)
        setFocusedFrame(newDocument->frame());

    if (caretBrowsing) {
        Position position = firstPositionInOrBeforeNode(node);
        VisibleSelection newSelection(position, position, DOWNSTREAM);
        if (frame->selection()->shouldChangeSelection(newSelection))
            frame->selection()->setSelection(newSelection);
    }

    static_cast<Element*>(node)->focus(false);
    return true;
}

Node* FocusController::findFocusableNodeAcrossTreeScope(FocusDirection direction, TreeScope* scope, Node* currentNode, KeyboardEvent* event)
{
    Node* node = findFocusableNode(direction, scope, currentNode, event);
    // If there's no focusable node to advance to, move up the tree scopes until we find one.
    while (!node && scope) {
        Node* owner = ownerOfTreeScope(scope);
        if (!owner)
            break;
        node = findFocusableNode(direction, owner->treeScope(), owner, event);
        scope = owner->treeScope();
    }
    node = deepFocusableNode(direction, node, event);
    return node;
}


Node* FocusController::findFocusableNode(FocusDirection direction, TreeScope* scope, Node* node, KeyboardEvent* event)
{
    return (direction == FocusDirectionForward)
        ? nextFocusableNode(scope, node, event)
        : previousFocusableNode(scope, node, event);
}

static Node* nextNodeWithExactTabIndex(Node* start, int tabIndex, KeyboardEvent* event)
{
    // Search is inclusive of start
    for (Node* node = start; node; node = node->traverseNextNode())
        if (node->isKeyboardFocusable(event) && node->tabIndex() == tabIndex)
            return node;

    return 0;
}

static Node* previousNodeWithExactTabIndex(Node* start, int tabIndex, KeyboardEvent* event)
{
    // Search is inclusive of start
    for (Node* n = start; n; n = n->traversePreviousNode())
        if (n->isKeyboardFocusable(event) && n->tabIndex() == tabIndex)
            return n;

    return 0;
}

static Node* nextNodeWithGreaterTabIndex(Node* start, int tabIndex, KeyboardEvent* event)
{
    // Search is inclusive of start
    int winningTabIndex = std::numeric_limits<short>::max() + 1;
    Node* winner = 0;
    for (Node* n = start; n; n = n->traverseNextNode())
        if (n->isKeyboardFocusable(event) && n->tabIndex() > tabIndex && n->tabIndex() < winningTabIndex) {
            winner = n;
            winningTabIndex = n->tabIndex();
        }

    return winner;
}

static Node* previousNodeWithLowerTabIndex(Node* start, int tabIndex, KeyboardEvent* event)
{
    // Search is inclusive of start
    int winningTabIndex = 0;
    Node* winner = 0;
    for (Node* n = start; n; n = n->traversePreviousNode())
        if (n->isKeyboardFocusable(event) && n->tabIndex() < tabIndex && n->tabIndex() > winningTabIndex) {
            winner = n;
            winningTabIndex = n->tabIndex();
        }

    return winner;
}

Node* FocusController::nextFocusableNode(TreeScope* scope, Node* start, KeyboardEvent* event)
{
    if (start) {
        // If a node is excluded from the normal tabbing cycle, the next focusable node is determined by tree order
        if (start->tabIndex() < 0) {
            for (Node* n = start->traverseNextNode(); n; n = n->traverseNextNode())
                if (n->isKeyboardFocusable(event) && n->tabIndex() >= 0)
                    return n;
        }

        // First try to find a node with the same tabindex as start that comes after start in the scope.
        if (Node* winner = nextNodeWithExactTabIndex(start->traverseNextNode(), start->tabIndex(), event))
            return winner;

        if (!start->tabIndex())
            // We've reached the last node in the document with a tabindex of 0. This is the end of the tabbing order.
            return 0;
    }

    // Look for the first node in the scope that:
    // 1) has the lowest tabindex that is higher than start's tabindex (or 0, if start is null), and
    // 2) comes first in the scope, if there's a tie.
    if (Node* winner = nextNodeWithGreaterTabIndex(scope, start ? start->tabIndex() : 0, event))
        return winner;

    // There are no nodes with a tabindex greater than start's tabindex,
    // so find the first node with a tabindex of 0.
    return nextNodeWithExactTabIndex(scope, 0, event);
}

Node* FocusController::previousFocusableNode(TreeScope* scope, Node* start, KeyboardEvent* event)
{
    Node* last;
    for (last = scope; last->lastChild(); last = last->lastChild()) { }

    // First try to find the last node in the scope that comes before start and has the same tabindex as start.
    // If start is null, find the last node in the scope with a tabindex of 0.
    Node* startingNode;
    int startingTabIndex;
    if (start) {
        startingNode = start->traversePreviousNode();
        startingTabIndex = start->tabIndex();
    } else {
        startingNode = last;
        startingTabIndex = 0;
    }

    // However, if a node is excluded from the normal tabbing cycle, the previous focusable node is determined by tree order
    if (startingTabIndex < 0) {
        for (Node* n = startingNode; n; n = n->traversePreviousNode())
            if (n->isKeyboardFocusable(event) && n->tabIndex() >= 0)
                return n;
    }

    if (Node* winner = previousNodeWithExactTabIndex(startingNode, startingTabIndex, event))
        return winner;

    // There are no nodes before start with the same tabindex as start, so look for a node that:
    // 1) has the highest non-zero tabindex (that is less than start's tabindex), and
    // 2) comes last in the scope, if there's a tie.
    startingTabIndex = (start && start->tabIndex()) ? start->tabIndex() : std::numeric_limits<short>::max();
    return previousNodeWithLowerTabIndex(last, startingTabIndex, event);
}

Node* FocusController::ownerOfTreeScope(TreeScope* scope)
{
    ASSERT(scope);
    if (scope->isShadowRoot())
        return scope->shadowHost();
    if (scope->document()->frame())
        return scope->document()->frame()->ownerElement();
    return 0;
}

static bool relinquishesEditingFocus(Node *node)
{
    ASSERT(node);
    ASSERT(node->rendererIsEditable());

    Node* root = node->rootEditableElement();
    Frame* frame = node->document()->frame();
    if (!frame || !root)
        return false;

    return frame->editor()->shouldEndEditing(rangeOfContents(root).get());
}

static void clearSelectionIfNeeded(Frame* oldFocusedFrame, Frame* newFocusedFrame, Node* newFocusedNode)
{
    if (!oldFocusedFrame || !newFocusedFrame)
        return;

    if (oldFocusedFrame->document() != newFocusedFrame->document())
        return;

    FrameSelection* s = oldFocusedFrame->selection();
    if (s->isNone())
        return;

    bool caretBrowsing = oldFocusedFrame->settings()->caretBrowsingEnabled();
    if (caretBrowsing)
        return;

    Node* selectionStartNode = s->selection().start().deprecatedNode();
    if (selectionStartNode == newFocusedNode || selectionStartNode->isDescendantOf(newFocusedNode) || selectionStartNode->shadowAncestorNode() == newFocusedNode)
        return;

    if (Node* mousePressNode = newFocusedFrame->eventHandler()->mousePressNode()) {
        if (mousePressNode->renderer() && !mousePressNode->canStartSelection()) {
            // Don't clear the selection for contentEditable elements, but do clear it for input and textarea. See bug 38696.
            Node * root = s->rootEditableElement();
            if (!root)
                return;

            if (Node* shadowAncestorNode = root->shadowAncestorNode()) {
                if (!shadowAncestorNode->hasTagName(inputTag) && !shadowAncestorNode->hasTagName(textareaTag))
                    return;
            }
        }
    }

    s->clear();
}

bool FocusController::setFocusedNode(Node* node, PassRefPtr<Frame> newFocusedFrame)
{
    RefPtr<Frame> oldFocusedFrame = focusedFrame();
    RefPtr<Document> oldDocument = oldFocusedFrame ? oldFocusedFrame->document() : 0;

    Node* oldFocusedNode = oldDocument ? oldDocument->focusedNode() : 0;
    if (oldFocusedNode == node)
        return true;

    // FIXME: Might want to disable this check for caretBrowsing
    if (oldFocusedNode && oldFocusedNode->rootEditableElement() == oldFocusedNode && !relinquishesEditingFocus(oldFocusedNode))
        return false;

    m_page->editorClient()->willSetInputMethodState();

    clearSelectionIfNeeded(oldFocusedFrame.get(), newFocusedFrame.get(), node);

    if (!node) {
        if (oldDocument)
            oldDocument->setFocusedNode(0);
        m_page->editorClient()->setInputMethodState(false);
        return true;
    }

    RefPtr<Document> newDocument = node->document();

    if (newDocument && newDocument->focusedNode() == node) {
        m_page->editorClient()->setInputMethodState(node->shouldUseInputMethod());
        return true;
    }

    if (oldDocument && oldDocument != newDocument)
        oldDocument->setFocusedNode(0);

    setFocusedFrame(newFocusedFrame);

    // Setting the focused node can result in losing our last reft to node when JS event handlers fire.
    RefPtr<Node> protect = node;
    if (newDocument) {
        bool successfullyFocused = newDocument->setFocusedNode(node);
        if (!successfullyFocused)
            return false;
    }

    if (newDocument->focusedNode() == node)
        m_page->editorClient()->setInputMethodState(node->shouldUseInputMethod());

    return true;
}

void FocusController::setActive(bool active)
{
    if (m_isActive == active)
        return;

    m_isActive = active;

    if (FrameView* view = m_page->mainFrame()->view()) {
        if (!view->platformWidget()) {
            view->updateLayoutAndStyleIfNeededRecursive();
            view->updateControlTints();
        }

        if (const HashSet<ScrollableArea*>* scrollableAreas = m_page->scrollableAreaSet()) {
            HashSet<ScrollableArea*>::const_iterator end = scrollableAreas->end();
            for (HashSet<ScrollableArea*>::const_iterator it = scrollableAreas->begin(); it != end; ++it) {
                if (!active)
                    (*it)->scrollAnimator()->contentAreaDidHide();
                else
                    (*it)->scrollAnimator()->contentAreaDidShow();
            }
        }
    }

    focusedOrMainFrame()->selection()->pageActivationChanged();

    if (m_focusedFrame && isFocused())
        dispatchEventsOnWindowAndFocusedNode(m_focusedFrame->document(), active);
}

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
IntSize FocusController::frameOffset(Frame* initialFrame)
{
    IntSize offset;
    for (Frame* frame = initialFrame; frame; frame = frame->tree()->parent()) {
        RenderBoxModelObject* renderer;
        if (frame->ownerElement() && (renderer = frame->ownerElement()->renderBoxModelObject())) {
            do {
                offset.expand(renderer->offsetLeft(), renderer->offsetTop());
            } while ((renderer = renderer->offsetParent()));
            offset -= frame->view()->scrollOffset();
        }
    }
    return offset;
}

bool FocusController::isTopmostNode(const FocusCandidate& candidate)
{
#if USE(CAIRO)
    Node* candidateNode = candidate.visibleNode;
    if (candidateNode->isFrameOwnerElement() && !isPluginElement(candidateNode))
        return true;

    Frame* frame = candidateNode->document()->frame();
    EventHandler* eventHandler = frame->eventHandler();
    IntSize offset(frameOffset(frame));

    Node* childNode;
    if (candidateNode->hasTagName(HTMLNames::aTag))
        childNode = candidateNode->firstChild();
    else
        childNode = 0;

    cairo_rectangle_int_t rect = candidate.rect;
    cairo_region_t* region = cairo_region_create_rectangle(&rect);
    if (!region)
        return false;

    while (cairo_region_num_rectangles(region)) {
        cairo_region_get_rectangle(region, 0, &rect);
        HitTestResult result = eventHandler->hitTestResultAtPoint(IntPoint(rect.x + (rect.width / 2), rect.y + (rect.height / 2)) - offset, false);

        Node* node = result.innerNode();
        if (!node || !node->parentNode() || node->parentNode()->isDocumentNode() || candidateNode->contains(node)) {
            cairo_region_destroy(region);
            return true;
        }

        if (node->hasTagName(HTMLNames::areaTag))
            node = static_cast<HTMLAreaElement*>(node)->imageElement();

        ASSERT(node && node->renderer() && node->renderer()->enclosingLayer());
        IntRect layerRect(node->renderer()->enclosingLayer()->absoluteBoundingBox());
        layerRect.move(frameOffset(node->document()->frame()));
        if (!layerRect.intersects(rect)) {
            cairo_region_destroy(region);
            return true;
        }

        cairo_rectangle_int_t cairoRect = layerRect;
        if (cairo_region_subtract_rectangle(region, &cairoRect) != CAIRO_STATUS_SUCCESS)
            break;

        while (childNode && !cairo_region_num_rectangles(region)) {
            if (childNode->renderer()) {
                cairoRect = nodeRectInAbsoluteCoordinates(childNode);
                cairo_region_union_rectangle(region, &cairoRect);
            }
            childNode = childNode->nextSibling();
        }
    }

    cairo_region_destroy(region);
    return false;
#else
    return true;
#endif
}

static bool isSuitableCandidate(FocusDirection direction, const FocusCandidate& candidate, const FocusCandidate& closest)
{
    ASSERT(candidate.alignment != closest.alignment);
    if (candidate.alignment == None)
        return false;
    if (closest.alignment == None)
        return true;

    long long candidateDistance = candidate.distance;
    long long closestDistance = closest.distance;

    if (candidate.alignment < closest.alignment) {
        if (direction == FocusDirectionLeft || direction == FocusDirectionRight)
            candidateDistance += candidate.rect.width();
        else
            candidateDistance += candidate.rect.height();
    } else {
        if (direction == FocusDirectionLeft || direction == FocusDirectionRight)
            closestDistance += closest.rect.width();
        else
            closestDistance += closest.rect.height();
    }

    return candidateDistance < closestDistance;
}
#endif

static void updateFocusCandidateIfNeeded(FocusDirection direction, const FocusCandidate& current, FocusCandidate& candidate, FocusCandidate& closest)
{
    ASSERT(candidate.visibleNode->isElementNode());
    ASSERT(candidate.visibleNode->renderer());

    // Ignore iframes that don't have a src attribute
    if (frameOwnerElement(candidate) && (!frameOwnerElement(candidate)->contentFrame() || candidate.rect.isEmpty()))
        return;

    // Ignore off screen child nodes of containers that do not scroll (overflow:hidden)
    if (candidate.isOffscreen && !canBeScrolledIntoView(direction, candidate))
        return;

    distanceDataForNode(direction, current, candidate);
    if (candidate.distance == maxDistance())
        return;

    if (candidate.isOffscreenAfterScrolling && candidate.alignment < Full)
        return;

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (!FocusController::isTopmostNode(candidate))
        return;
#endif

    if (closest.isNull()) {
        closest = candidate;
        return;
    }

    IntRect intersectionRect = intersection(candidate.rect, closest.rect);
#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (!intersectionRect.isEmpty() && !areElementsOnSameLine(closest, candidate)
        && !isMeaningfulRect(candidate.rect, intersectionRect.width() * intersectionRect.height())) {
#else
    if (!intersectionRect.isEmpty() && !areElementsOnSameLine(closest, candidate)) {
#endif
        // If 2 nodes are intersecting, do hit test to find which node in on top.
        int x = intersectionRect.x() + intersectionRect.width() / 2;
        int y = intersectionRect.y() + intersectionRect.height() / 2;
        HitTestResult result = candidate.visibleNode->document()->page()->mainFrame()->eventHandler()->hitTestResultAtPoint(IntPoint(x, y), false, true);
        if (candidate.visibleNode->contains(result.innerNode())) {
            closest = candidate;
            return;
        }
        if (closest.visibleNode->contains(result.innerNode()))
            return;
    }

    if (candidate.alignment == closest.alignment) {
        if (candidate.distance < closest.distance)
            closest = candidate;
        return;
    }

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (isSuitableCandidate(direction, candidate, closest))
        closest = candidate;
#else
    if (candidate.alignment > closest.alignment)
        closest = candidate;
#endif
}

void FocusController::findFocusCandidateInContainer(Node* container, const IntRect& startingRect, FocusDirection direction, KeyboardEvent* event, FocusCandidate& closest)
{
    ASSERT(container);
    Node* focusedNode = (focusedFrame() && focusedFrame()->document()) ? focusedFrame()->document()->focusedNode() : 0;

    Node* node = container->firstChild();
    FocusCandidate current;
    current.rect = startingRect;
    current.focusableNode = focusedNode;
    current.visibleNode = focusedNode;

    for (; node; node = (node->isFrameOwnerElement() || canScrollInDirection(node, direction)) ? node->traverseNextSibling(container) : node->traverseNextNode(container)) {
        if (node == focusedNode)
            continue;

        if (!node->isElementNode())
            continue;

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
        if ((!node->isKeyboardFocusable(event) && !node->isFrameOwnerElement() && !canScrollInDirection(node, direction))
            || m_excludedCandidateSet.contains(node))
#else
        if (!node->isKeyboardFocusable(event) && !node->isFrameOwnerElement() && !canScrollInDirection(node, direction))
#endif
            continue;

        FocusCandidate candidate = FocusCandidate(node, direction);
        if (candidate.isNull())
            continue;

        candidate.enclosingScrollableBox = container;
        updateFocusCandidateIfNeeded(direction, current, candidate, closest);
    }
}

bool FocusController::advanceFocusDirectionallyInContainer(Node* container, const IntRect& startingRect, FocusDirection direction, KeyboardEvent* event)
{
    if (!container || !container->document())
        return false;

    IntRect newStartingRect = startingRect;

    if (startingRect.isEmpty())
        newStartingRect = virtualRectForDirection(direction, nodeRectInAbsoluteCoordinates(container));

    // Find the closest node within current container in the direction of the navigation.
    FocusCandidate focusCandidate;
    findFocusCandidateInContainer(container, newStartingRect, direction, event, focusCandidate);

    if (focusCandidate.isNull()) {
        // Nothing to focus, scroll if possible.
        // NOTE: If no scrolling is performed (i.e. scrollInDirection returns false), the
        // spatial navigation algorithm will skip this container.
        return scrollInDirection(container, direction);
    }

    if (HTMLFrameOwnerElement* frameElement = frameOwnerElement(focusCandidate)) {
        // If we have an iframe without the src attribute, it will not have a contentFrame().
        // We ASSERT here to make sure that
        // updateFocusCandidateIfNeeded() will never consider such an iframe as a candidate.
        ASSERT(frameElement->contentFrame());

        if (focusCandidate.isOffscreenAfterScrolling) {
            scrollInDirection(focusCandidate.visibleNode->document(), direction);
            return true;
        }
        // Navigate into a new frame.
        IntRect rect;
        Node* focusedNode = focusedOrMainFrame()->document()->focusedNode();
        if (focusedNode && !hasOffscreenRect(focusedNode))
            rect = nodeRectInAbsoluteCoordinates(focusedNode, true /* ignore border */);
        frameElement->contentFrame()->document()->updateLayoutIgnorePendingStylesheets();
        if (!advanceFocusDirectionallyInContainer(frameElement->contentFrame()->document(), rect, direction, event)) {
            // The new frame had nothing interesting, need to find another candidate.
#if ENABLE(TVGG_SPATIAL_NAVIGATION)
            m_excludedCandidateSet.add(focusCandidate.visibleNode);
            return advanceFocusDirectionallyInContainer(container, newStartingRect, direction, event);
#else
            return advanceFocusDirectionallyInContainer(container, nodeRectInAbsoluteCoordinates(focusCandidate.visibleNode, true), direction, event);
#endif
        }
        return true;
    }

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (!const_cast<Node*>(focusCandidate.visibleNode)->getEventListeners(eventNames().clickEvent).isEmpty()) {
        if (advanceFocusDirectionallyInContainer(focusCandidate.visibleNode, startingRect, direction, event))
            return true;
    }
#endif

    if (canScrollInDirection(focusCandidate.visibleNode, direction)) {
        if (focusCandidate.isOffscreenAfterScrolling) {
            scrollInDirection(focusCandidate.visibleNode, direction);
            return true;
        }
        // Navigate into a new scrollable container.
        IntRect startingRect;
        Node* focusedNode = focusedOrMainFrame()->document()->focusedNode();
        if (focusedNode && !hasOffscreenRect(focusedNode))
            startingRect = nodeRectInAbsoluteCoordinates(focusedNode, true);
        return advanceFocusDirectionallyInContainer(focusCandidate.visibleNode, startingRect, direction, event);
    }
    if (focusCandidate.isOffscreenAfterScrolling) {
        Node* container = focusCandidate.enclosingScrollableBox;
        scrollInDirection(container, direction);
        return true;
    }

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (focusCandidate.isOffscreen) {
        Node* container = focusCandidate.enclosingScrollableBox;
        scrollInDirection(container, direction);
    }
#endif

    // We found a new focus node, navigate to it.
    Element* element = toElement(focusCandidate.focusableNode);
    ASSERT(element);

    element->focus(false);
    return true;
}

bool FocusController::advanceFocusDirectionally(FocusDirection direction, KeyboardEvent* event)
{
    Frame* curFrame = focusedOrMainFrame();
    ASSERT(curFrame);

    Document* focusedDocument = curFrame->document();
    if (!focusedDocument)
        return false;

    Node* focusedNode = focusedDocument->focusedNode();
    Node* container = focusedDocument;

    if (container->isDocumentNode())
        static_cast<Document*>(container)->updateLayoutIgnorePendingStylesheets();

    // Figure out the starting rect.
    IntRect startingRect;
    if (focusedNode) {
        if (!hasOffscreenRect(focusedNode)) {
            container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, focusedNode);
            startingRect = nodeRectInAbsoluteCoordinates(focusedNode, true /* ignore border */);
        } else if (focusedNode->hasTagName(areaTag)) {
            HTMLAreaElement* area = static_cast<HTMLAreaElement*>(focusedNode);
            container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, area->imageElement());
            startingRect = virtualRectForAreaElementAndDirection(area, direction);
        }
    }

    bool consumed = false;
    do {
        consumed = advanceFocusDirectionallyInContainer(container, startingRect, direction, event);
        startingRect = nodeRectInAbsoluteCoordinates(container, true /* ignore border */);
        container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, container);
        if (container && container->isDocumentNode())
            static_cast<Document*>(container)->updateLayoutIgnorePendingStylesheets();
    } while (!consumed && container);

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    m_excludedCandidateSet.clear();
#endif
    return consumed;
}

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
Element* FocusController::getFocusableElement(Node* node)
{
    while (node) {
        if (node->isElementNode() && node->isKeyboardFocusable(0))
            return toElement(node);
        node = node->parentNode();
    }

    return 0;
}

static bool hasNeededUpdateFocusableCandidate(const FocusCandidate& current, FocusCandidate& candidate, FocusCandidate& closest)
{
    // Ignore iframes that don't have a src attribute
    if (frameOwnerElement(candidate) && (!frameOwnerElement(candidate)->contentFrame() || candidate.rect.isEmpty()))
        return false;

    // Ignore off screen child nodes of containers that do not scroll (overflow:hidden)
    if (candidate.isOffscreen)
        return false;

    if (!FocusController::isTopmostNode(candidate))
        return false;

    IntPoint currentPoint = current.rect.location();
    IntRect& candidateRect = candidate.rect;

    if (currentPoint.x() >= candidateRect.x() && currentPoint.x() <= candidateRect.maxX()) {
        if (currentPoint.y() < candidateRect.y())
            candidate.distance = candidateRect.y() - currentPoint.y();
        else
            candidate.distance = currentPoint.y() - candidateRect.maxY();
    } else if (currentPoint.y() >= candidateRect.y() && currentPoint.y() <= candidateRect.maxY()) {
        if (currentPoint.x() < candidateRect.x())
            candidate.distance = candidateRect.x() - currentPoint.x();
        else
            candidate.distance = currentPoint.x() - candidateRect.maxX();
    } else {
        IntPoint candidatePoint;
        if (currentPoint.x() < candidateRect.x() && currentPoint.y() < candidateRect.y())
            candidatePoint = candidateRect.minXMinYCorner();
        else if (currentPoint.x() > candidateRect.x() && currentPoint.y() < candidateRect.y())
            candidatePoint = candidateRect.maxXMinYCorner();
        else if (currentPoint.x() < candidateRect.x() && currentPoint.y() > candidateRect.y())
            candidatePoint = candidateRect.minXMaxYCorner();
        else
            candidatePoint = candidateRect.maxXMaxYCorner();

        int diffX = candidatePoint.x() - currentPoint.x();
        if (diffX < 0)
            diffX = -diffX;
        int diffY = candidatePoint.y() - currentPoint.y();
        if (diffY < 0)
            diffY = -diffY;
        candidate.distance = diffX + diffY;
    }

    if (closest.isNull())
        return true;

    if (candidate.distance < closest.distance)
        return true;
    else
        return false;
}

static void findFocusableCandidateInContainerByPoint(Node* container, const FocusCandidate& current, FocusCandidate& closest)
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

        if (hasNeededUpdateFocusableCandidate(current, candidate, closest)) {
            if (HTMLFrameOwnerElement* frameElement = frameOwnerElement(candidate)) {
                frameElement->contentFrame()->document()->updateLayoutIgnorePendingStylesheets();
                findFocusableCandidateInContainerByPoint(frameElement->contentFrame()->document(), current, closest);
            } else
                closest = candidate;
        }
    }

    if (!closest.isNull() && !const_cast<Node*>(closest.visibleNode)->getEventListeners(eventNames().clickEvent).isEmpty()) {
        FocusCandidate subClosest;
        findFocusableCandidateInContainerByPoint(closest.visibleNode, current, subClosest);
        if (!subClosest.isNull())
            closest = subClosest;
    }
}

void FocusController::findFocusableCandidateInContainer(Node* container, Node* startingNode, const IntRect& startingRect, FocusDirection direction, FocusCandidate& closest)
{
    ASSERT(container);

    Node* node = container->firstChild();
    FocusCandidate current;
    current.rect = startingRect;
    current.focusableNode = startingNode;
    current.visibleNode = startingNode;

    for (; node; node = (node->isFrameOwnerElement()) ? node->traverseNextSibling(container) : node->traverseNextNode(container)) {
        if (node == startingNode)
            continue;

        if (!node->isElementNode())
            continue;

        if ((!node->isKeyboardFocusable(0) && !node->isFrameOwnerElement()) || m_excludedCandidateSet.contains(node))
            continue;

        FocusCandidate candidate = FocusCandidate(node, direction);
        if (candidate.isNull())
            continue;

        candidate.enclosingScrollableBox = container;
        updateFocusCandidateIfNeeded(direction, current, candidate, closest);
    }
}

Element* FocusController::findFocusableDirectionallyInContainer(Node* container, Node* startingNode, const IntRect& startingRect, FocusDirection direction)
{
    if (!container || !container->document())
        return 0;

    IntRect newStartingRect = startingRect;

    if (startingRect.isEmpty())
        newStartingRect = virtualRectForDirection(direction, nodeRectInAbsoluteCoordinates(container));

    FocusCandidate focusCandidate;
    findFocusableCandidateInContainer(container, startingNode, newStartingRect, direction, focusCandidate);

    if (focusCandidate.isNull() || focusCandidate.isOffscreen)
        return 0;

    if (HTMLFrameOwnerElement* frameElement = frameOwnerElement(focusCandidate)) {
        frameElement->contentFrame()->document()->updateLayoutIgnorePendingStylesheets();
        Element* element = findFocusableDirectionallyInContainer(frameElement->contentFrame()->document(), startingNode, startingRect, direction);
        if (!element) {
            // The new frame had nothing interesting, need to find another candidate.
            m_excludedCandidateSet.add(focusCandidate.visibleNode);
            return findFocusableDirectionallyInContainer(container, startingNode, startingRect, direction);
        }
        return element;
    }

#if ENABLE(TVGG_SPATIAL_NAVIGATION)
    if (!const_cast<Node*>(focusCandidate.visibleNode)->getEventListeners(eventNames().clickEvent).isEmpty()) {
        Element* element = findFocusableDirectionallyInContainer(focusCandidate.visibleNode, startingNode, startingRect, direction);
        if (element)
            return element;
    }
#endif

    return toElement(focusCandidate.visibleNode);
}

Element* FocusController::findNearsetFocusableByPoint(FocusDirection direction, const IntPoint& point, Frame* mainFrame, bool ignoreScrollbarRect)
{
    Document* document = mainFrame->document();
    document->updateLayoutIgnorePendingStylesheets();

    IntPoint contentsPoint = mainFrame->view()->windowToContents(point);

    HitTestResult result = mainFrame->page()->mainFrame()->eventHandler()->hitTestResultAtPoint(contentsPoint, false, false, ShouldHitTestScrollbars);
    if (ignoreScrollbarRect && (result.scrollbar() || mainFrame->view()->scrollbarAtPoint(point)))
        return 0;

    Element* element = getFocusableElement(result.innerNode());

    if (direction == FocusDirectionNone) {
        FocusCandidate current;
        current.rect.setLocation(contentsPoint);

        FocusCandidate focusCandidate;

        if (element) {
            findFocusableCandidateInContainerByPoint(element, current, focusCandidate);
            if (!focusCandidate.isNull())
                element = toElement(focusCandidate.focusableNode);
        } else {
            findFocusableCandidateInContainerByPoint(mainFrame->document(), current, focusCandidate);
            if (focusCandidate.isNull())
                return 0;
            element = toElement(focusCandidate.focusableNode);
        }
        ASSERT(element);
    } else {
        Node* container = document;
        IntRect startingRect;

        if (element) {
            container = element->document();

            if (!hasOffscreenRect(element)) {
                container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, element);
                startingRect = nodeRectInAbsoluteCoordinates(element, true /* ignore border */);
            } else if (element->hasTagName(areaTag)) {
                HTMLAreaElement* area = static_cast<HTMLAreaElement*>(element);
                container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, area->imageElement());
                startingRect = virtualRectForAreaElementAndDirection(area, direction);
            }
        }

        element = 0;
        while (container) {
            if (container->isDocumentNode())
                static_cast<Document*>(container)->updateLayoutIgnorePendingStylesheets();
            element = findFocusableDirectionallyInContainer(container, element, startingRect, direction);
            if (element)
                break;
            startingRect = nodeRectInAbsoluteCoordinates(container, true /* ignore border */);
            container = scrollableEnclosingBoxOrParentFrameForNodeInDirection(direction, container);
        }
    }

    m_excludedCandidateSet.clear();

    return element;
}

bool FocusController::advanceFocusByPoint(const IntPoint& point)
{
    Element* element = findNearsetFocusableByPoint(FocusDirectionNone, point, m_page->mainFrame(), false);
    if (!element)
        return false;

    element->focus(false);
    return true;
}

IntRect FocusController::getFocusRect(Node* node)
{
    FrameView* view;
    if (!node || !(view = node->document()->view()))
        return IntRect();

    if (node->hasTagName(HTMLNames::areaTag)) {
        HTMLAreaElement* areaElement = static_cast<HTMLAreaElement*>(node);
        HTMLImageElement* imageElement = areaElement->imageElement();

        if (imageElement && imageElement->renderer()) {
            RenderObject* imageRenderer = imageElement->renderer();
            Path path = areaElement->computePath(imageRenderer);

            if (!path.isEmpty()) {
                IntPoint absolutePoint(view->convertToContainingWindow(-view->scrollPosition()));
                path.translate(FloatSize(absolutePoint.x(), absolutePoint.y()));
                return IntRect(path.boundingRect());
            }
         }
    } else if (node->renderer()) {
        RenderObject* focusedRenderer = node->renderer();

        IntRect clippedRect(focusedRenderer->absoluteClippedOverflowRect());
        clippedRect.intersect(view->visibleContentRect());
        clippedRect.move(-view->scrollOffset());
        clippedRect = view->convertToContainingWindow(clippedRect);

        IntPoint absolutePoint;
        absolutePoint = view->convertToContainingWindow(view->convertFromRenderer(focusedRenderer, absolutePoint));

        Vector<IntRect> focusRingRects;
        focusedRenderer->addFocusRingRects(focusRingRects, absolutePoint);
        unsigned rectCount = focusRingRects.size();

        IntRect focusRingRect;
        for (unsigned i = 0; i < rectCount; i++) {
            focusRingRects[i].intersect(clippedRect);
            focusRingRect.unite(focusRingRects[i]);
        }
        return focusRingRect;
    }

    return IntRect();
}

IntRect FocusController::nearestFocusableNodeRectByPoint(FocusDirection direction, const IntPoint& point)
{
    Frame* mainFrame = m_page->mainFrame();
    Element* element = findNearsetFocusableByPoint(direction, point, mainFrame, true);
    if (!element)
        return IntRect();

    return getFocusRect(element);
}
#endif

} // namespace WebCore
