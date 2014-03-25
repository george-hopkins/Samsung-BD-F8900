/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_main.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      getSlotHandle
 *
 *  DESCRIPTION
 *      Return userHandle from slotId
 *
 *  RETURNS
 *      handle
 *
 *---------------------------------------------------------------------------*/
CsrSchedQid getSlotHandle(hidh_instData_t *instData, CsrUint8 slot)
{
    if(slot < MAX_HID_CONNECTIONS)
    {
        return instData->slots[slot];
    }
    return CSR_SCHED_QID_INVALID;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getNewSubInst
 *
 *  DESCRIPTION
 *      Get an available device instance administration data block.
 *
 *  RETURNS
 *      device id
 *
 *---------------------------------------------------------------------------*/
CsrUint8 getNewSubInst(hidh_instData_t *instData)
{
    CsrUint8 i;

    for(i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        if(instData->subInstData[i].subState == NULL_STATE)
        {
            return i;
        }
    }

    return NOT_FOUND;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      freeSubInst
 *
 *  DESCRIPTION
 *      Free device instance administration data block.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void freeSubInst(hidh_instData_t *instData, CsrUint8 deviceId)
{
    q_element_t *q, *tmp;
    hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

    CSR_BT_HIDH_STATE_CHANGE(subInstData->subState, NULL_STATE);
    subInstData->doTerminate = FALSE;
    subInstData->useExtDataPath = FALSE;
    subInstData->ctrl_ch.cid = 0;
    subInstData->intr_ch.cid = 0;
    subInstData->timerId = 0;

    q = subInstData->ctrl_ch.qFirst;
    while(q)
    {
        tmp = q->next;
        CsrPmemFree(q->data);
        CsrPmemFree(q);
        q = tmp;
    }
    subInstData->ctrl_ch.qFirst = NULL;
    subInstData->ctrl_ch.qLast = NULL;
    subInstData->ctrl_ch.sending = FALSE;

    if (subInstData->ctrl_ch.qos)
    {
        CsrPmemFree(subInstData->ctrl_ch.qos);
        subInstData->ctrl_ch.qos = NULL;
    }

    q = subInstData->intr_ch.qFirst;
    while(q)
    {
        tmp = q->next;
        CsrPmemFree(q->data);
        CsrPmemFree(q);
        q = tmp;
    }
    subInstData->intr_ch.qFirst = NULL;
    subInstData->intr_ch.qLast = NULL;
    subInstData->intr_ch.sending = FALSE;

    if (subInstData->intr_ch.qos)
    {
        CsrPmemFree(subInstData->intr_ch.qos);
        subInstData->intr_ch.qos=NULL;
    }

    if (subInstData->serviceName)
    {
        CsrPmemFree(subInstData->serviceName);
        subInstData->serviceName = NULL;
    }

    if (subInstData->descriptor)
    {
        CsrPmemFree(subInstData->descriptor);
        subInstData->descriptor = NULL;
    }

    CsrMemSet(&subInstData->sdpInfo, 0, sizeof(CsrBtHidhSdpInfo));
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getDeviceIdFromCid
 *
 *  DESCRIPTION
 *      Find a device instance from a given connection id (CID)
 *
 *  RETURNS
 *      deviceId
 *
 *----------------------------------------------------------------------------*/
CsrUint8 getDeviceIdFromCid(CsrBtConnId cid, hidh_instData_t *instData)
{
    CsrUint8 i=0;

    for( i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        if( (instData->subInstData[i].intr_ch.cid == cid) || (instData->subInstData[i].ctrl_ch.cid == cid) )
        {
            return i;
        }
    }

    return NOT_FOUND;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getDeviceIdFromBdAddr
 *
 *  DESCRIPTION
 *      Find a device instance from a given Bluetooth address
 *
 *  RETURNS
 *      deviceId
 *
 *----------------------------------------------------------------------------*/
CsrUint8 getDeviceIdFromBdAddr(CsrBtDeviceAddr bdAddr, hidh_instData_t *instData)
{
    CsrUint8 i=0;

    for( i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        if( CsrBtBdAddrEq(&(instData->subInstData[i].bdAddr), &bdAddr) )
        {
            return i;
        }
    }

    return NOT_FOUND;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      getDeviceIdFromState
 *
 *  DESCRIPTION
 *      Find a device instance from a given device state
 *
 *  RETURNS
 *      deviceId
 *
 *----------------------------------------------------------------------------*/
CsrUint8 getDeviceIdFromState(CsrUint32 state, hidh_instData_t *instData)
{
    CsrUint8 i=0;

    for( i=0; i<MAX_HID_CONNECTIONS; i++)
    {
        if( instData->subInstData[i].subState == state)
        {
            return i;
        }
    }

    return NOT_FOUND;
}


