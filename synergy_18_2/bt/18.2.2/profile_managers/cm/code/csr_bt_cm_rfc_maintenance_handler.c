/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_hci_sco.h"

CsrUint16 CsrBtCmRfcDetermineMtu(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, CsrUint16 proposedMtu)
{
    if (proposedMtu == 0)
    {
        if (cmData->rfcBuild)
        {
            return CSR_BT_RFC_BUILD_RFCOMM_MAX_FRAME_SIZE;
        }
        else
        {
            if (HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_5SLOT_MR_BIT, cmData->dmVar.lmpSuppFeatures))
            {
                
                aclTable *aclConnectionElement = NULL;
                
                returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

                if (aclConnectionElement && 
                    !HCI_FEATURE_IS_SUPPORTED(LMP_FEATURES_5SLOT_MR_BIT, aclConnectionElement->remoteFeatures))
                {
                    return CSR_BT_HCI_BUILD_RFCOMM_NON_EDR_MAX_FRAME_SIZE;
                }
                return CSR_BT_HCI_BUILD_RFCOMM_EDR_MAX_FRAME_SIZE;
            }
            return CSR_BT_HCI_BUILD_RFCOMM_NON_EDR_MAX_FRAME_SIZE;
        }
    }
    else
    {
        return proposedMtu;
    }
}

cmConnIdServerType CsrBtCmReturnConnIdServerStruct(CsrBtConnId bt_conn_id, CsrUint8 server)
{
    cmConnIdServerType connIdServer;
    connIdServer.bt_conn_id  = bt_conn_id;
    connIdServer.server = server;
    return (connIdServer);
}

CsrBool CsrBtCmRfcFindRfcConnElementFromIndex(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given index match the elementId  */
    CsrUint8 index                 = *(CsrUint8 *) value;
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;

    if (index == theElement->elementId)
    {
        if (theElement->cmRfcConnInst)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
CsrBool CsrBtCmRfcFindRfcConnElementFromScoHandle(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given scoHandle match            */
    hci_connection_handle_t scoHandle = *(hci_connection_handle_t *) value;
    cmRfcConnElement * theElement     = (cmRfcConnElement *) elem;

    if (theElement->cmRfcConnInst)
    {
        if (theElement->cmRfcConnInst->eScoParms.handle == scoHandle)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}
#endif

CsrBool CsrBtCmRfcFindRfcConnElementFromBtConnId(CsrCmnListElm_t *elem, void *value)
{
    CsrBtConnId btConnId = *(CsrBtConnId *) value;
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;

    if (theElement->cmRfcConnInst)
    {
        if (theElement->cmRfcConnInst->btConnId == btConnId)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}


CsrBool CsrBtCmRfcFindRfcConnElementFromConnIdRemoteServer(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given scoHandle match the scoHandle
     in the element                                                             */
    cmConnIdServerType  connIdServ      = *(cmConnIdServerType *) value;
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;

    if (theElement->cmRfcConnInst)
    {
        if (theElement->cmRfcConnInst->remoteServerChan == connIdServ.server)
        {
            if (theElement->cmRfcConnInst->btConnId == connIdServ.bt_conn_id)
            {
                return (TRUE);
            }
        }
    }
    return (FALSE);
}

CsrBool CsrBtCmRfcFindRfcConnElementFromConnIdServer(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given scoHandle match the scoHandle
     in the element                                                             */
    cmConnIdServerType  connIdServ      = *(cmConnIdServerType *) value;
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;

    if (theElement->cmRfcConnInst)
    {
        if (theElement->cmRfcConnInst->serverChannel == connIdServ.server)
        {
            if (theElement->cmRfcConnInst->btConnId == connIdServ.bt_conn_id)
            {
                return (TRUE);
            }
        }
    }
    return (FALSE);
}

cmRfcConnElement * CsrBtCmRfcFindRfcConnElementFromDeviceAddrState(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, CsrUint32 state)
{ /* Try to find the cmRfcConnElement that match with the given deviceAddr
     and state. If no match it return NULL                                      */
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    {
        if (currentElem->cmRfcConnInst)
        {
            if(state == currentElem->cmRfcConnInst->state)
            {
                if(bd_addr_eq(deviceAddr, &(currentElem->cmRfcConnInst->deviceAddr)))
                {
                    return currentElem;
                }
            }
        }
    }
    return NULL;
}

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
cmRfcConnElement * CsrBtCmRfcFindRfcConnElementFromDeviceAddrScoHandle(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr, hci_connection_handle_t scoHandle)
{ /* Try to find the cmRfcConnElement that match with the given deviceAddr
     and scoHandle. If no match it return NULL                              */
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    {
        if (currentElem->cmRfcConnInst)
        {
            if(scoHandle == currentElem->cmRfcConnInst->eScoParms.handle)
            {
                if(bd_addr_eq(deviceAddr, &(currentElem->cmRfcConnInst->deviceAddr)))
                {
                    return currentElem;
                }
            }
        }
    }
    return NULL;
}
#endif

cmRfcConnElement * CsrBtCmRfcFindRfcConnElementFromServerState(cmInstanceData_t *cmData, CsrUint8 server, CsrUint32 state)
{ /* Try to find the cmRfcConnElement that match with the given server channel
     and state. If no match it return NULL                                    */
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    {
        if (currentElem->cmRfcConnInst)
        {
            if(server == currentElem->cmRfcConnInst->serverChannel)
            {
                if(state == currentElem->cmRfcConnInst->state)
                {
                    return currentElem;
                }
            }
        }
    }
    return NULL;
}

cmRfcConnElement * CsrBtCmRfcFindRfcConnElementFromDeviceAddrState1OrState2(cmInstanceData_t *cmData,
                                            CsrBtDeviceAddr *deviceAddr, CsrUint32 state1, CsrUint32 state2)
{ /* Try to find the cmRfcConnElement that match with the given  deviceaddr and
     either of the state1 or state2. If no match it return NULL                         */
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    {
        if (currentElem->cmRfcConnInst)
        {
            if(bd_addr_eq(deviceAddr, &(currentElem->cmRfcConnInst->deviceAddr)))
            {
                if (state1 == currentElem->cmRfcConnInst->state ||
                    state2 == currentElem->cmRfcConnInst->state)
                {
                    return currentElem;
                }
            }
        }
    }
    return NULL;
}


cmRfcConnElement * CsrBtCmRfcFindRfcConnElementFromDeviceAddrStateRequestedMode(cmInstanceData_t *cmData,
                                            CsrBtDeviceAddr *deviceAddr, CsrUint32 state, CsrUint8 requestedMode)
{ /* Try to find the cmRfcConnElement that match with the given deviceaddr, the state,
     and the requestedMode. If no match it return NULL                                  */
    cmRfcConnElement *currentElem;

    for (currentElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentElem; currentElem = currentElem->next)
    {
        if (currentElem->cmRfcConnInst)
        {
            if(bd_addr_eq(deviceAddr, &(currentElem->cmRfcConnInst->deviceAddr)))
            {
                if (state == currentElem->cmRfcConnInst->state)
                {
                    if (requestedMode == currentElem->cmRfcConnInst->requestedMode)
                    {
                        return currentElem;
                    }
                }
            }
        }
    }
    return NULL;
}

static CsrBool csrBtCmRfcCheckRfcConnElementIndex(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given index match the elementId  */
    CsrUint8 index                 = *(CsrUint8 *) value;
    cmRfcConnElement * theElement = (cmRfcConnElement *) elem;

    if (index == theElement->elementId)
    {
        return (TRUE);
    }
    return (FALSE);
}

CsrBool CsrBtCmRfcSetElementCounter(cmInstanceData_t *cmData)
{
    CsrUintFast8 i;

    for (i = 0; i < CM_ERROR; i++)
    {
        cmData->rfcVar.elementCounter++;

        if (cmData->rfcVar.elementCounter == CM_ERROR)
        {
            cmData->rfcVar.elementCounter = 1;
        }
        if(!CM_FIND_RFC_ELEMENT(csrBtCmRfcCheckRfcConnElementIndex, &(cmData->rfcVar.elementCounter)))
        {
            return (TRUE);
        }
    }
    return FALSE;
}

void cleanUpConnectionTable(cmRfcConnInstType ** theLogicalLink)
{
    cmRfcConnInstType * connInst = *theLogicalLink;

    if (connInst)
    { /* Deallocate priInst and inst                                            */
        CsrUintFast16 i;

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
        if (connInst->eScoParms.handle != NO_SCO)
        {
            CsrHciDeRegisterScoHandle(connInst->eScoParms.handle);
        }
        CsrBtCmScoFreePacketTypeArray(&connInst->eScoParms.negotiateCnt);
#endif

        for( i = 0; i < CSR_BT_CM_INIT_CREDIT; i++)
        {
            if(connInst->dataControl.receivedBuffer[i] != NULL)
            { /* To prevent memory leaks, CsrPmemFree data in the receivedbuffer */
                rfc_free_primitive(connInst->dataControl.receivedBuffer[i]);
            }
        }

        CsrPmemFree(connInst->controlSignalQueue);

        CsrPmemFree(connInst);
        *theLogicalLink = NULL;
    }
    else
    { /* Nothing to deallocate                                                  */
        ;
    }
}


#endif /* #ifndef EXCLUDE_CSR_BT_RFC_MODULE */
