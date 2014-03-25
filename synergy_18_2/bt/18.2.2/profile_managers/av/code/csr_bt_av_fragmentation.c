/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_util.h"
#include "csr_bt_av_fragmentation.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvAllocFragmentHead
 *
 *  DESCRIPTION
 *      Allocate a fragment 'head' to use for collecting arriving fragments.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvAllocFragmentHead( fragmentHead_t **headPtr, CsrBtConnId btConnId)
{
    fragmentHead_t    *ptr;

    if( *headPtr == NULL)
    {
        *headPtr = (fragmentHead_t *) CsrPmemAlloc( sizeof(fragmentHead_t) );
        (*headPtr)->btConnId = btConnId;
        (*headPtr)->totalLength = 0;
        (*headPtr)->next = NULL;
        (*headPtr)->fragmentPtr = NULL;

        return;
    }

    /* find the end of the list */
    ptr = *headPtr;
    while ( ptr->next != NULL )
    {
        ptr = ptr->next;
    }

    ptr->next = (fragmentHead_t *) CsrPmemAlloc( sizeof(fragmentHead_t) );
    ptr = ptr->next;

    ptr->btConnId = btConnId;
    ptr->totalLength = 0;
    ptr->next = NULL;
    ptr->fragmentPtr = NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvFreeFragments
 *
 *  DESCRIPTION
 *      Free stored fragments with a given connection id.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvFreeFragments( fragmentHead_t **headPtr, CsrBtConnId btConnId)
{
    fragmentHead_t        *head, *prevHead;
    fragmentElement_t    *fragmentPtr, *tmp;

    head = *headPtr;
    prevHead = NULL;

    if( btConnId != CLEAR_ALL_FRAGMENTS )
    {
        /* clear fragments for a specific cid */
        while( (head != NULL) && (head->btConnId != btConnId) )
        {
            prevHead = head;
            head = head->next;
        }

        if (head != NULL)
        {
            fragmentPtr = head->fragmentPtr;
            while( fragmentPtr != NULL )
            {
                CsrPmemFree(fragmentPtr->packetPtr);
                tmp = fragmentPtr;
                fragmentPtr = fragmentPtr->next;
                CsrPmemFree(tmp);
            }

            if ( head == *headPtr )
            {
                *headPtr = head->next;
                CsrPmemFree(head);
            }
            else
            {
                prevHead->next = head->next;
                CsrPmemFree(head);
            }
        }
    }
    else
    {
        /* clear all fragments */
        while (head != NULL )
        {
            fragmentPtr = head->fragmentPtr;
            while( fragmentPtr != NULL )
            {
                CsrPmemFree(fragmentPtr->packetPtr);
                tmp = fragmentPtr;
                fragmentPtr = fragmentPtr->next;
                CsrPmemFree(tmp);
            }

            prevHead = head;
            head = head->next;
            CsrPmemFree(prevHead);
        }

        *headPtr = NULL;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvAddFragment
 *
 *  DESCRIPTION
 *      Add a fragment to fragment 'head'
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvAddFragment( fragmentHead_t *headPtr, CsrBtConnId btConnId, CsrUint16 length, CsrUint8 *fragment)
{
    fragmentElement_t    **ptr;

    while( (headPtr != NULL) && (headPtr->btConnId != btConnId) )
    {
        headPtr = headPtr->next;
    }

    if( headPtr != NULL)
    {
        ptr = &headPtr->fragmentPtr;
        if( *ptr != NULL)
        {
            while( ((*ptr)->next) != NULL)
            {
                ptr = &((*ptr)->next);
            }

            (*ptr)->next = (fragmentElement_t *) CsrPmemAlloc(sizeof(fragmentElement_t));
            ptr = &((*ptr)->next);
        }
        else
        {
            *ptr = (fragmentElement_t *) CsrPmemAlloc(sizeof(fragmentElement_t));
        }

        (*ptr)->next = NULL;
        (*ptr)->packetPtr = fragment;
        (*ptr)->length = length;

        headPtr->totalLength = (CsrUint16)(headPtr->totalLength + length - 1);
    }
    else
    {
        CsrPmemFree(fragment);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDefragment
 *
 *  DESCRIPTION
 *      Defragment the fragments to a single message for given L2CAP cid
 *
 *  RETURNS
 *      Reference to the defragmented message on success, otherwise NULL
 *
 *---------------------------------------------------------------------------*/
CsrUint8 *CsrBtAvDefragment( fragmentHead_t **headPtr, CsrBtConnId btConnId, CsrUint16 *length, CsrUint8 *fragment)
{
    fragmentHead_t        *head;
    fragmentElement_t    *ptr;
    CsrUint8                *msg;
    CsrUint16             index;

    head = *headPtr;

    while( (head != NULL) && (head->btConnId != btConnId) )
    {
        head = head->next;
    }

    if( head != NULL)
    {
        head->totalLength = (CsrUint16)(head->totalLength + *length - 1);

        msg = (CsrUint8 *) CsrPmemAlloc(head->totalLength);

        ptr = head->fragmentPtr;

        CsrMemCpy( msg, ptr->packetPtr + 1, ptr->length - 1);
        *msg = *ptr->packetPtr;
        index = (CsrUint16)(ptr->length - 1);

        ptr = ptr->next;

        while (ptr != NULL)
        {
            CsrMemCpy( msg + index, ptr->packetPtr + 1, ptr->length - 1);
            index = (CsrUint16)(index + ptr->length - 1);
            ptr = ptr->next;
        }

        CsrMemCpy( msg + index, fragment + 1, *length - 1);

        *length = head->totalLength;
        CsrPmemFree(fragment);

        CsrBtAvFreeFragments( headPtr, btConnId);
        return msg;
    }
    else
    {
        return NULL;
    }
}

