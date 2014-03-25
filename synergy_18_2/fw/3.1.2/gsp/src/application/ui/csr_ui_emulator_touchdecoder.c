/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ui_emulator_touchdecoder.h"
#include "csr_ui_keyhandler.h"
#include "csr_ui_graphics_driver.h"

typedef struct EventHandle
{
    CsrInt16            left;
    CsrInt16            top;
    CsrInt16            right;
    CsrInt16            bottom;
    CsrUint16           key;
    struct EventHandle *previous;
    struct EventHandle *next;
} EventHandle;

typedef struct
{
    EventHandle *firstEventHandle;
} EventQueue;

static EventQueue *eventQueue = NULL;

void CsrUiTouchEvent(CsrInt16 x, CsrInt16 y)
{
    EventHandle *searchPointer = NULL;

    if (eventQueue != NULL)
    {
        for (searchPointer = eventQueue->firstEventHandle; searchPointer != NULL; searchPointer = searchPointer->next)
        {
            if ((x >= searchPointer->left) && (x <= searchPointer->right)
                && (y >= searchPointer->top) && (y <= searchPointer->bottom))
            {
                CsrUiKeyEvent(searchPointer->key);
                break;
            }
        }
    }
}

EventHandle *CsrUiTouchDecoderRegisterEventHandler(CsrInt16 left, CsrInt16 top, CsrInt16 right, CsrInt16 bottom, CsrUint16 key)
{
    EventHandle *newHandle;

    if (!eventQueue)
    {
        eventQueue = (EventQueue *) CsrPmemAlloc(sizeof(EventQueue));
        eventQueue->firstEventHandle = NULL;
    }

    newHandle = (EventHandle *) CsrPmemAlloc(sizeof(EventHandle));
    newHandle->left = left;
    newHandle->top = top;
    newHandle->right = right;
    newHandle->bottom = bottom;
    newHandle->key = key;
    newHandle->next = eventQueue->firstEventHandle;
    newHandle->previous = NULL;
    if (eventQueue->firstEventHandle != NULL)
    {
        eventQueue->firstEventHandle->previous = newHandle;
    }
    eventQueue->firstEventHandle = newHandle;

    return newHandle;
}

void CsrUiRegisterKey(CsrInt16 left, CsrInt16 top, CsrInt16 right, CsrInt16 bottom, CsrUint16 key)
{
    CsrUiTouchDecoderRegisterEventHandler(left, top, right, bottom, key);

#ifdef CSR_UI_DRAW_RECTANGLE_AROUND_KEY
    CsrUiGraphicsDriverRectangle(left, top, right, bottom, 0);
#endif
}

#if 0 /* This is not currently used but might be worth saving anyway */
void CsrUiTouchDecoderDeRegisterEventHandler(EventHandle *eventHandle)
{
    if (eventHandle->previous != NULL)
    {
        eventHandle->previous->next = eventHandle->next;
        if (eventHandle->next != NULL)
        {
            eventHandle->next->previous = eventHandle->previous;
        }
    }
    else /* first */
    {
        eventQueue->firstEventHandle = eventHandle->next;
        if (eventQueue->firstEventHandle != NULL)
        {
            eventQueue->firstEventHandle->previous = NULL;
        }
    }
    CsrPmemFree(eventHandle);
}

#endif

void CsrUiTouchDecoderDeinit()
{
    EventHandle *ptr1;
    EventHandle *ptr2;

    if (eventQueue)
    {
        for (ptr1 = eventQueue->firstEventHandle; ptr1 != NULL; ptr1 = ptr2)
        {
            ptr2 = ptr1->next;
            CsrPmemFree(ptr1);
        }
        CsrPmemFree(eventQueue);
        eventQueue = NULL;
    }
}
