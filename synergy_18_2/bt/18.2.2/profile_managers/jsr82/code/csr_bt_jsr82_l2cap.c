/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_jsr82_l2cap.h"
#include "csr_bt_jsr82_prim.h"
#include "csr_bt_jsr82_main.h"
#include "csr_bt_jsr82_maintenance_handler.h"
#include "csr_bt_jsr82_remote_device.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_message_queue.h"
#include "csr_bt_jsr82_rfcomm.h"

#define JSR82_HIGHEST_INVALID_PSM_VALUE (0xFFFF)

static psmValue *lookupPsm(psm_t psm, psmValue *psmList)
{
    psmValue *psmPtr;

    psmPtr = psmList;
    while (psmPtr)
    {
        if ((psmPtr->psm == psm) && psmPtr->assigned)
        {
            return psmPtr;
        }
        psmPtr = psmPtr->next;
    }
    return NULL;
}

static void sendGetPsmCfm(void *inst, psm_t psm, CsrBtJsr82Prim **orgMsg)
{
    CsrBtJsr82L2caGetPsmCfm *msg;
    Jsr82MainInstanceData *instData;
    psmValue *psmEntry;
    CsrBtJsr82L2caGetPsmReq *orgPrim;

    instData = (Jsr82MainInstanceData *)inst;
    orgPrim = ((CsrBtJsr82L2caGetPsmReq *)*orgMsg);

    if(psm)
    {
        psmEntry = lookupPsm(psm, instData->psmList);
        psmEntry->client = FALSE;
    }

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caGetPsmCfm));
    msg->type = CSR_BT_JSR82_L2CA_GET_PSM_CFM;
    msg->localPsm = psm;
    msg->reqID = orgPrim->reqID;
    CsrBtJsr82MessagePut(orgPrim->appHandle, msg);
}

static CsrBool csrBtJsr82FindCidElementFromCid(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given btConnId match                          */
    CsrUint32 cid   = *(CsrUint32 *) value;
    jsr82CidElement *theElement = (jsr82CidElement *) elem;

    if (theElement->cidInst)
    {
        if (theElement->cidInst->cid == cid)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrBool csrBtJsr82FindConnectableCidElement(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given btConnId match                          */
    jsr82CidElement *theElement = (jsr82CidElement *) elem;
    CSR_UNUSED(value);
    if (theElement->cidInst)
    {
        if (!(theElement->cidInst->cid))
        {
            if (theElement->cidInst->connectable)
            {
                return (TRUE);
            }
        }
    }
    return (FALSE);
}

static CsrBool csrBtJsr82FindConnectedCidElement(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given cid match                          */
    jsr82CidElement *theElement = (jsr82CidElement *) elem;
    CSR_UNUSED(value);

    if (theElement->cidInst)
    {
        if (theElement->cidInst->cid)
        {
            if (theElement->cidInst->connected)
            {
                return (TRUE);
            }
        }
    }
    return (FALSE);
}

static CsrBool csrBtJsr82FindConnectingCidElement(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given cid match                          */
    jsr82CidElement *theElement = (jsr82CidElement *) elem;
    CSR_UNUSED(value);

    if (theElement->cidInst)
    {
        if (!(theElement->cidInst->cid))
        {
            if (theElement->cidInst->connecting)
            {
                return (TRUE);
            }
        }
    }
    return (FALSE);
}

static CsrBool csrBtJsr82FindUsedCidElement(CsrCmnListElm_t *elem, void *value)
{ /* This function will return TRUE if the given cid match                          */
    jsr82CidElement *theElement = (jsr82CidElement *) elem;
    CSR_UNUSED(value);

    if (theElement->cidInst)
    {
        if (theElement->cidInst->cid)
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static psmValue *lookupPsmByCid(CsrBtConnId btConnId, psmValue *psmList)
{
    jsr82CidElement * cidElement;
    psmValue        * psmPtr = psmList;

    while (psmPtr)
    {
        cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindCidElementFromCid, &btConnId);

        if (cidElement)
        {
            return psmPtr;
        }
        psmPtr = psmPtr->next;
    }
    return NULL;
}

static jsr82CidElement *lookupCid(CsrBtConnId btConnId, psmValue *psmList)
{
    if(btConnId)
    {
        jsr82CidElement * cidElement;
        psmValue        *psmPtr = psmList;

        while (psmPtr)
        {
            cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindCidElementFromCid, &btConnId);

            if (cidElement)
            {
                return (cidElement);
            }
            psmPtr = psmPtr->next;
        }
    }
    return NULL;
}

static cidStruct *findFreeCidElement(psmValue *psmPtr)
{
    jsr82CidElement  *cidElement = (jsr82CidElement *) CsrCmnListElementAddLast(&(psmPtr->cidList), sizeof(jsr82CidElement));
    return (cidElement->cidInst);
}

static cidStruct *findConnectableCidElement(psmValue *psmPtr)
{
    CsrBool connectable = TRUE;
    jsr82CidElement * cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindConnectableCidElement, &connectable);

    if (cidElement)
    {
        return (cidElement->cidInst);
    }
    return NULL;
}

static cidStruct *findConnectedCidElement(psmValue *psmPtr)
{
    jsr82CidElement * cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindConnectedCidElement, NULL);

    if (cidElement)
    {
        return (cidElement->cidInst);
    }
    return NULL;
}

static cidStruct *findConnectingCidElement(psmValue *psmPtr)
{
    jsr82CidElement * cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindConnectingCidElement, NULL);

    if (cidElement)
    {
        return (cidElement->cidInst);
    }
    return NULL;
}

static cidStruct *findUsedCidElement(psmValue *psmPtr)
{
    jsr82CidElement * cidElement = JSR82_FIND_CID_ELEMENT(csrBtJsr82FindUsedCidElement, NULL);

    if (cidElement)
    {
        return (cidElement->cidInst);
    }
    return NULL;
}

static CsrBool assignPsm(psmValue *psmEntry)
{
    if (psmEntry && !psmEntry->assigned)
    {
        psmEntry->assigned = TRUE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static psmValue *findFreePsm(Jsr82MainInstanceData *instData)
{
    psmValue    *psmPtr;

    psmPtr = instData->psmList;
    while (psmPtr)
    {
        if (!(psmPtr->assigned))
        {
            return psmPtr;
        }
        psmPtr = psmPtr->next;
    }
    return NULL;
}

static void csrBtJsr82CbInitCidElement(CsrCmnListElm_t *elem)
{
    cidStruct *cidInst              = (cidStruct *) CsrPmemAlloc(sizeof(cidStruct));
    jsr82CidElement   *cidElement   = (jsr82CidElement *) elem;
    cidInst->appHandle              = 0;
    cidInst->reqID                  = 0;
    cidInst->connectable            = FALSE;
    cidInst->connecting             = FALSE;
    cidInst->connected              = FALSE;
    cidInst->attrs                  = 0;
    cidInst->cid                    = 0;
    cidInst->transmitMtu            = 0;
    cidInst->sendingPacket          = FALSE;
    cidInst->receiveMtu             = 0;
    cidInst->receivedDataLength     = 0;
    cidInst->dataReqQueue           = NULL;
    cidInst->dataQueueLength        = 0;
    cidInst->dataQueue              = NULL;
    cidInst->attrs                  = 0;
    bd_addr_zero(&(cidInst->deviceAddr));
    cidElement->cidInst             = cidInst;
}

static void csrBtJsr82CbFreeCidElement(CsrCmnListElm_t *elem)
{
    jsr82CidElement *cidElement = (jsr82CidElement *) elem;

    if (cidElement->cidInst)
    {
        CsrUint16  event;
        cidStruct * cidInst = (cidStruct *) cidElement->cidInst;

        if (cidInst->dataQueue)
        {
            CsrBtCmL2caDataInd *recvData;
            void *popMsg;

            while (CsrMessageQueuePop(&cidInst->dataQueue, &event, &popMsg))
            {
                recvData=(CsrBtCmL2caDataInd *)popMsg;

                CsrPmemFree(recvData->payload);
                CsrPmemFree(recvData);
            }
        }

        if (cidInst->dataReqQueue)
        {
            CsrBtJsr82L2caRxDataReq *dataReq;
            void *popMsg;

            while(CsrMessageQueuePop(&cidInst->dataReqQueue, &event, &popMsg))
            {
                dataReq = (CsrBtJsr82L2caRxDataReq *)popMsg;

                CsrPmemFree(dataReq);
            } /* if any data is buffered - flush it */
        }
    }
    CsrPmemFree(cidElement->cidInst);
}

static psmValue *reservePsm(Jsr82MainInstanceData *instData)
{
    psmValue   *psmPtr;
    psmValue   *newElement;

    if(instData->allocatedPsms < (CsrUint16)CSR_BT_JSR82_MAX_ALLOCATED_PSMS)
    {
        psmPtr = instData->psmList;
        while (psmPtr && psmPtr->next)
        {
            psmPtr = psmPtr->next;
        }
        newElement      = (psmValue *)CsrPmemZalloc(sizeof(psmValue));
        newElement->psm = JSR82_HIGHEST_INVALID_PSM_VALUE;
        CsrCmnListInit(&(newElement->cidList), 0, csrBtJsr82CbInitCidElement, csrBtJsr82CbFreeCidElement);

        if (psmPtr)
        {
            psmPtr->next = newElement;
        }
        else
        {
            psmPtr = newElement;
            instData->psmList = psmPtr;
        }
        return newElement;
    }
    return NULL;
}

static void getPsm(Jsr82MainInstanceData *instData, jsr82PsmAllocateCallbackType returnFunction)
{
    psmValue    *psmEntry = NULL;

    psmEntry = findFreePsm(instData);

    if (psmEntry)
    {
        assignPsm(psmEntry);
        returnFunction((void *)instData, psmEntry->psm, (CsrBtJsr82Prim **)&instData->recvMsg);
    }
    else
    {
        psmEntry = reservePsm(instData);

        if(psmEntry)
        {
            const jsr82DynSignalHandler cfmHandler[] = {CsrBtJsr82CmL2caRegisterCfmHandler};
            assignPsm(psmEntry);
            psmEntry->returnFunction = returnFunction;
            CsrBtJsr82SendCmMsgDown(instData, (CsrBtJsr82Prim **)&instData->recvMsg,
                                    CsrBtCml2caRegisterReq_struct(CSR_BT_JSR82_IFACEQUEUE,
                                                                  CSR_BT_ASSIGN_DYNAMIC_PSM,
                                                                  L2CA_FLOW_MODE_BASIC,
                                                                  0,
                                                                  CSR_BT_CM_CONTEXT_UNUSED),
                                    cfmHandler);
            CsrBtJsr82SetBusy(instData);
        }
        else
        {
            returnFunction((void *)instData, 0, (CsrBtJsr82Prim **)&instData->recvMsg);
        }
    }
}

void CsrBtJsr82L2caGetPsmReqHandler(Jsr82MainInstanceData *instData)
{
    getPsm(instData, sendGetPsmCfm);
}

CsrBool CsrBtJsr82CmL2caRegisterCfmHandler(Jsr82MainInstanceData *instData, CsrBtJsr82Prim **orgMsg)
{
    CsrBtCmL2caRegisterCfm  *prim;
    psmValue                *psmEntry;

    prim     = (CsrBtCmL2caRegisterCfm *)instData->recvMsg;
    psmEntry = lookupPsm(JSR82_HIGHEST_INVALID_PSM_VALUE, instData->psmList);

    if (psmEntry)
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            psmEntry->psm = prim->localPsm;
            instData->allocatedPsms++;
        }
        else
        {
            psmEntry->psm = 0;
        }
        CsrBtJsr82SetIdle(instData);
        psmEntry->returnFunction((void *)instData, psmEntry->psm, orgMsg);
        psmEntry->returnFunction = NULL;
    }
    else
    { /* Must never happen  */
        ;
    }
    return TRUE;
}

static void csrBtJsr82SendL2caAcceptCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    psm_t psm, CsrBtConnId cid, l2ca_mtu_t transmitMtu,
    CsrBtDeviceAddr deviceAddr, CsrBtResultCode result)
{
    CsrBtJsr82L2caAcceptCfm   *msg;
    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caAcceptCfm));
    msg->type = CSR_BT_JSR82_L2CA_ACCEPT_CFM;
    msg->reqID = reqID;
    msg->localPsm = psm;
    msg->btConnId = cid;
    msg->deviceAddr = deviceAddr;
    msg->transmitMtu = transmitMtu;
    msg->resultCode = result;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82L2caAcceptReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caAcceptReq *prim = (CsrBtJsr82L2caAcceptReq *)instData->recvMsg;
    dm_security_mode_t curSecMode = CSR_BT_DEFAULT_SEC_MODE;

    if ((curSecMode < SEC_MODE2_SERVICE) && (prim->attrs & (CSR_BT_JSR82_AUTHENTICATE | CSR_BT_JSR82_ENCRYPT)))
    {
        CsrBtDeviceAddr dummyAddr = {0,0,0};
        csrBtJsr82SendL2caAcceptCfm(prim->appHandle, prim->reqID, prim->localPsm, 0, 0, dummyAddr,
            CSR_BT_RESULT_CODE_JSR82_SECURITY_FAIL);
    }
    else
    {
        cidStruct *cidElement = NULL;
        psmValue  *psmEntry   = lookupPsm(prim->localPsm, instData->psmList);

        if(psmEntry && psmEntry->assigned)
        {
            cidElement = findFreeCidElement(psmEntry);
        }

        if (cidElement)
        {
            dm_security_level_t secLevel;

            cidElement->appHandle           = prim->appHandle;
            cidElement->reqID               = prim->reqID;
            cidElement->connectable         = TRUE;
            cidElement->receiveMtu          = (l2ca_mtu_t)((prim->receiveMtu) ? prim->receiveMtu : L2CA_MTU_DEFAULT);
            cidElement->attrs               = prim->attrs;
            cidElement->transmitMtu         = prim->transmitMtu;
            secLevel                        = CsrBtJsr82SetSecurity(cidElement->attrs, TRUE);
            CsrBtCmJsr82l2caConnectAcceptReqSend(CSR_BT_JSR82_IFACEQUEUE, prim->localPsm, (CsrUint24)0x000000,
                                secLevel, cidElement->receiveMtu, JSR82_L2CA_UUID, cidElement->transmitMtu);
        }
        else
        {
            CsrBtDeviceAddr dummyAddr = {0,0,0};
            csrBtJsr82SendL2caAcceptCfm(prim->appHandle, prim->reqID, prim->localPsm, 0, 0, dummyAddr,
                CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
        }
    }
}

void CsrBtJsr82CmL2caConnectAcceptCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caConnectAcceptCfm    *prim;
    psmValue                        *psmEntry;
    cidStruct                       *cidElement;

    prim = (CsrBtCmL2caConnectAcceptCfm *)instData->recvMsg;

    psmEntry = lookupPsm(prim->localPsm, instData->psmList);
    cidElement = findConnectableCidElement(psmEntry);

    if (cidElement)
    {
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, prim->deviceAddr, prim->btConnId);

            cidElement->connectable = FALSE;
            cidElement->connected = TRUE;
            cidElement->cid = prim->btConnId;

            if (cidElement->transmitMtu == 0)
            {
                cidElement->transmitMtu = prim->mtu;
            }
            else
            { /* The transmitMtu is correct. because prim->mtu >= transmitMtu
                 and transmitMtu represent the the negotiated value           */
                ;
            }
            cidElement->receivedDataLength = 0;
            cidElement->dataQueueLength = 0;
            cidElement->deviceAddr = prim->deviceAddr;
            csrBtJsr82SendL2caAcceptCfm(cidElement->appHandle, cidElement->reqID, psmEntry->psm, cidElement->cid, cidElement->transmitMtu, cidElement->deviceAddr,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS);

            CsrBtJsr82AddConnection(instData, cidElement->deviceAddr);

        }
        else
        {
            CsrBtDeviceAddr dummyAddr = {0,0,0};
            csrBtJsr82SendL2caAcceptCfm(cidElement->appHandle, cidElement->reqID, psmEntry->psm, 0, 0, dummyAddr,
                CSR_BT_RESULT_CODE_JSR82_CONNECT_FAILED);
        }
    }
    else
    {
        CsrBtCml2caDisconnectReqSend(prim->btConnId);
    }
}

static void csrBtJsr82SendL2caOpenCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    CsrBtDeviceAddr deviceAddr, CsrBtConnId cid, l2ca_mtu_t transmitMtu,
    CsrBtResultCode result)
{
    CsrBtJsr82L2caOpenCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caOpenCfm));
    msg->type = CSR_BT_JSR82_L2CA_OPEN_CFM;
    msg->reqID = reqID;
    msg->deviceAddr.nap = deviceAddr.nap;
    msg->deviceAddr.uap = deviceAddr.uap;
    msg->deviceAddr.lap = deviceAddr.lap;
    msg->btConnId = cid;
    msg->transmitMtu = transmitMtu;
    msg->resultCode = result;

    CsrBtJsr82MessagePut(appHandle, msg);
}

static void connectWithPsm(void *inst, psm_t psm, CsrBtJsr82Prim **orgMsg)
{
    Jsr82MainInstanceData   *instData;
    psmValue                *psmEntry;
    cidStruct               *cidElement = NULL;
    CsrBtJsr82L2caOpenReq        *orgPrim;

    instData = (Jsr82MainInstanceData *)inst;
    orgPrim = ((CsrBtJsr82L2caOpenReq *)*orgMsg);

    psmEntry = lookupPsm(psm, instData->psmList);
    if (psmEntry &&
        psmEntry->assigned)
    {
        psmEntry->client = TRUE;
        cidElement = findFreeCidElement(psmEntry);
    }

    if(cidElement)
    {
        dm_security_level_t secLevel;

        cidElement->appHandle   = orgPrim->appHandle;
        cidElement->reqID       = orgPrim->reqID;
        cidElement->connecting  = TRUE;
        cidElement->receiveMtu  = (l2ca_mtu_t)((orgPrim->receiveMtu) ? orgPrim->receiveMtu : L2CA_MTU_DEFAULT);
        cidElement->attrs       = orgPrim->attrs;
        cidElement->deviceAddr  = orgPrim->deviceAddr;
        cidElement->transmitMtu = orgPrim->transmitMtu;
        secLevel                        = CsrBtJsr82SetSecurity(cidElement->attrs, FALSE);
        CsrBtCmJsr82l2caConnectReqSend(CSR_BT_JSR82_IFACEQUEUE, cidElement->deviceAddr, psmEntry->psm,
            orgPrim->remotePsm, cidElement->receiveMtu, secLevel, cidElement->transmitMtu);
        CsrBtJsr82SetBusy(instData);
    }
    else
    {
        csrBtJsr82SendL2caOpenCfm(orgPrim->appHandle, orgPrim->reqID,
            orgPrim->deviceAddr, 0, 0,
            CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED);
    }
}

void CsrBtJsr82L2caOpenReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caOpenReq *prim;
    dm_security_mode_t  curSecMode = CSR_BT_DEFAULT_SEC_MODE;

    prim = (CsrBtJsr82L2caOpenReq *)instData->recvMsg;
    if ((curSecMode < SEC_MODE2_SERVICE) && (prim->attrs & (CSR_BT_JSR82_AUTHENTICATE | CSR_BT_JSR82_ENCRYPT | CSR_BT_JSR82_AUTHORIZE)))
    {
        CsrBtDeviceAddr dummyAddr = {0,0,0};
        csrBtJsr82SendL2caOpenCfm(prim->appHandle, prim->reqID, dummyAddr,
            0, 0, CSR_BT_RESULT_CODE_JSR82_SECURITY_FAIL);
    }
    else
    {
       getPsm(instData, connectWithPsm);
    }
}

void CsrBtJsr82CmL2caConnectCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caConnectCfm   *prim;
    psmValue                *psmEntry;
    cidStruct               *cidElement;

    prim = (CsrBtCmL2caConnectCfm *)instData->recvMsg;

    psmEntry = lookupPsm(prim->localPsm, instData->psmList);
    cidElement = findConnectingCidElement(psmEntry);
    if(cidElement)
    {
        if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
            prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, prim->deviceAddr, prim->btConnId);

            if (!cidElement->connectable &&
                !cidElement->connected)
            {
                cidElement->connecting = FALSE;
                cidElement->connected = TRUE;
                cidElement->cid = prim->btConnId;

                if (cidElement->transmitMtu == 0)
                {
                    cidElement->transmitMtu = prim->mtu;
                }
                else
                { /* The transmitMtu is correct. because prim->mtu >= transmitMtu
                     and transmitMtu represent the the negotiated value           */
                    ;
                }
                cidElement->dataQueueLength = 0;
                cidElement->receivedDataLength = 0;
                cidElement->sendingPacket = FALSE;
                csrBtJsr82SendL2caOpenCfm(cidElement->appHandle,
                    cidElement->reqID, cidElement->deviceAddr, cidElement->cid,
                    cidElement->transmitMtu,
                    CSR_BT_RESULT_CODE_JSR82_SUCCESS);

                CsrBtJsr82AddConnection(instData, cidElement->deviceAddr);

            }
            else
            {
                /* Should not be possible... */
            }

        }
        else
        {
            psmEntry->assigned = FALSE;
            psmEntry->destroy = FALSE;
            psmEntry->client = FALSE;
            psmEntry->returnFunction = NULL;
            csrBtJsr82SendL2caOpenCfm(cidElement->appHandle, cidElement->reqID,
                cidElement->deviceAddr, cidElement->cid, 0,
                CSR_BT_RESULT_CODE_JSR82_CONNECT_FAILED);
            CsrBtJsr82RemoveL2capConnection(cidElement->cid, instData);
        }
    }
    else
    {
        CsrBtCml2caDisconnectReqSend(prim->btConnId);
    }
    CsrBtJsr82SetIdle(instData);
}

static void sendL2caDisconnectCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    psm_t localPsm, CsrBtResultCode result, CsrBool localTerminated)
{
    CsrBtJsr82L2caDisconnectCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caDisconnectCfm));
    msg->type = CSR_BT_JSR82_L2CA_DISCONNECT_CFM;
    msg->reqID = reqID;
    msg->localPsm = localPsm;
    msg->resultCode = result;
    msg->localTerminated = localTerminated;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82RemoveL2capConnection(CsrBtConnId btConnId, Jsr82MainInstanceData *instData)
{
    jsr82CidElement *cidElement = lookupCid(btConnId, instData->psmList);

    if(cidElement)
    {
        CsrUint16                event;
        CsrBtCmL2caDataInd      *recvData;
        CsrBtJsr82L2caRxDataReq *dataReq;
        void *popMsg;
        cidStruct               *cidInst = cidElement->cidInst;
        psmValue *psmEntry      = lookupPsmByCid(btConnId, instData->psmList);

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, cidInst->deviceAddr, cidInst->cid);

        /* if any data requests are queued - answer them */
        while(CsrMessageQueuePop(&cidInst->dataReqQueue, &event, &popMsg))
        {
            CsrBtJsr82L2caRxDataCfm *msg;

            dataReq = (CsrBtJsr82L2caRxDataReq *)popMsg;

            msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxDataCfm));
            msg->type = CSR_BT_JSR82_L2CA_RX_DATA_CFM;
            msg->reqID = dataReq->reqID;
            msg->btConnId = cidInst->cid;
            msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
            msg->payloadLength = 0;
            msg->payload = NULL;
            CsrBtJsr82MessagePut(dataReq->appHandle, msg);
            CsrPmemFree(dataReq);
        } /* if any data is buffered - flush it */

        while (CsrMessageQueuePop(&cidInst->dataQueue, &event, &popMsg))
        {
            recvData=(CsrBtCmL2caDataInd *)popMsg;

            CsrPmemFree(recvData->payload);
            CsrPmemFree(recvData);
        }

        /* if we're in the middle of a send request - end it */
        if(cidInst->sendingPacket)
        {
            CsrBtJsr82L2caTxDataCfm *msg;

            msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caTxDataCfm));
            msg->type = CSR_BT_JSR82_L2CA_TX_DATA_CFM;
            msg->reqID = instData->reqID;
            msg->btConnId = cidInst->cid;
            msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DATAWRITE_FAIL;

            CsrBtJsr82MessagePut(instData->appHandle, msg);
        }
        CsrCmnListElementRemove(&(psmEntry->cidList), (CsrCmnListElm_t *)cidElement);
    }
}

void CsrBtJsr82L2caDisconnectReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caDisconnectReq  *prim;
    psmValue                *psmEntry;
    cidStruct               *cidElement;

    prim = (CsrBtJsr82L2caDisconnectReq *)instData->recvMsg;

    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    psmEntry = lookupPsm(prim->localPsm, instData->psmList);
    if(psmEntry && psmEntry->assigned && !psmEntry->client)
    {
        psmEntry->destroy = TRUE; /* mark this as a disconnect operation, rather than a close */
        cidElement = findConnectedCidElement(psmEntry);
        if (cidElement)
        {
            cidElement->connected = FALSE;
            CsrBtJsr82RemoveConnection(instData, cidElement->deviceAddr);
            CsrBtCml2caDisconnectReqSend(cidElement->cid);
            CsrBtJsr82SetBusy(instData);
        }
        else
        {
            cidElement = findConnectableCidElement(psmEntry);
            if(cidElement)
            {
                cidElement->connectable = FALSE;
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_JSR82_IFACEQUEUE, psmEntry->psm);
                CsrBtJsr82SetBusy(instData);
            }
            else
            {
                psmEntry->assigned = FALSE;
                psmEntry->destroy = FALSE;
                psmEntry->returnFunction = NULL;
                cidElement = findUsedCidElement(psmEntry);
                while(cidElement)
                {
                    CsrBtJsr82RemoveL2capConnection(cidElement->cid, instData);
                    cidElement = findUsedCidElement(psmEntry);
                }
                sendL2caDisconnectCfm(instData->appHandle, instData->reqID,
                    psmEntry->psm, CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
            }
        }
    }
    else
    {
        sendL2caDisconnectCfm(instData->appHandle, instData->reqID, 0,
            CSR_BT_RESULT_CODE_JSR82_COMMAND_DISALLOWED, FALSE);
    }
}

static void sendL2caCloseCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrBtConnId btConnId,
    CsrBtResultCode result, CsrBool localTerminated)
{
    CsrBtJsr82L2caCloseCfm *msg;
    msg = (CsrBtJsr82L2caCloseCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82L2caCloseCfm));

    msg->type = CSR_BT_JSR82_L2CA_CLOSE_CFM;
    msg->reqID = reqID;
    msg->btConnId = btConnId;
    msg->resultCode= result;
    msg->localTerminated = localTerminated;

    CsrBtJsr82MessagePut(appHandle, msg);
}


void CsrBtJsr82L2caCloseReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caCloseReq *prim;
    jsr82CidElement        *cidElement;
    psmValue               *psmEntry;

    prim = (CsrBtJsr82L2caCloseReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    psmEntry    = lookupPsmByCid(prim->btConnId, instData->psmList);
    cidElement  = lookupCid(prim->btConnId, instData->psmList);

    if(psmEntry && psmEntry->client)
    {
        psmEntry->assigned = FALSE;
    }

    if(cidElement && psmEntry)
    {
        cidStruct *cidInst = cidElement->cidInst;

        if(cidInst->connected)
        {
            psmEntry->destroy = FALSE;
            cidInst->connected = FALSE;
            CsrBtJsr82RemoveConnection(instData, cidInst->deviceAddr);
            CsrBtCml2caDisconnectReqSend(cidInst->cid);
            CsrBtJsr82SetBusy(instData);
        }
        else /* other side has already closed */
        {
            sendL2caCloseCfm(instData->appHandle, instData->reqID, cidInst->cid,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
            CsrBtJsr82RemoveL2capConnection(cidInst->cid, instData);
        }
    }
    else
    {
        sendL2caCloseCfm(instData->appHandle, instData->reqID, prim->btConnId,
            CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED, FALSE);
    }

}




void CsrBtJsr82CmL2caDisconnectIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caDisconnectInd *prim = (CsrBtCmL2caDisconnectInd *)instData->recvMsg;
    jsr82CidElement          *cidElement = lookupCid(prim->btConnId, instData->psmList);
    cidStruct                *cidInst = NULL;

    if (cidElement)
    {
        cidInst = cidElement->cidInst;
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, cidInst->deviceAddr, cidInst->cid);
    }

    if (prim->reasonSupplier == CSR_BT_SUPPLIER_CM &&
        prim->reasonCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        if (prim->localTerminated == TRUE)
        {
            CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
        }
        else
        {
            CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
                CSR_BT_RESULT_CODE_JSR82_SUCCESS, FALSE);
        }
    }
    else
    {
        CsrBtJsr82PropgateL2caDisconnectEvent(instData, prim->btConnId,
            CSR_BT_RESULT_CODE_JSR82_LINK_LOST, FALSE);
    }

    if (cidElement)
    {
        cidInst = cidElement->cidInst;

        if (cidInst->connected)
        {
            CsrUint16 event;
            CsrBtJsr82L2caRxDataReq *dataReq;
            void *popMsg;
            
            cidInst->connected = FALSE;
            while(CsrMessageQueuePop(&cidInst->dataReqQueue, &event, &popMsg))
            {
                CsrBtJsr82L2caRxDataCfm *msg;
            
                dataReq = (CsrBtJsr82L2caRxDataReq *)popMsg;
                msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxDataCfm));
                msg->type = CSR_BT_JSR82_L2CA_RX_DATA_CFM;
                msg->reqID = dataReq->reqID;
                msg->btConnId = prim->btConnId;
                msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
                msg->payloadLength = 0;
                msg->payload = NULL;
                CsrBtJsr82MessagePut(dataReq->appHandle, msg);
                cidInst->receivedDataLength = 0;
                CsrPmemFree(dataReq);
            }
            if(cidInst->sendingPacket)
            {
                CsrBtJsr82L2caTxDataCfm *msg;

                msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caTxDataCfm));
                msg->type = CSR_BT_JSR82_L2CA_TX_DATA_CFM;
                msg->reqID = instData->reqID;
                msg->btConnId = cidInst->cid;
                msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DATAWRITE_FAIL;
                CsrBtJsr82MessagePut(instData->appHandle, msg);
                cidInst->sendingPacket = FALSE;
                CsrBtJsr82SetIdle(instData);
            }
            CsrBtJsr82RemoveConnection(instData, cidInst->deviceAddr);
        }
        else
        {
            psmValue *psmEntry;
            psmEntry = lookupPsmByCid(cidInst->cid, instData->psmList);

            if(psmEntry && psmEntry->destroy) /* This is a disconnect - all conns on this psm should be disconnected */
            {
                cidInst = findConnectedCidElement(psmEntry);
                if(cidInst)
                {
                    cidInst->connected = FALSE;
                    CsrBtJsr82RemoveConnection(instData, cidInst->deviceAddr);
                    CsrBtCml2caDisconnectReqSend(cidInst->cid); /* stay in BUSY_S */
                }
                else
                {
                    cidInst = findConnectableCidElement(psmEntry);
                    if (cidInst)
                    {
                        cidInst->connectable = FALSE;
                        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_JSR82_IFACEQUEUE, psmEntry->psm); /* stay in BUSY_S */
                    }
                    else /* all connections on this psm are down */
                    {
                        psmEntry->assigned = FALSE;
                        psmEntry->client = FALSE;
                        psmEntry->destroy = FALSE;
                        psmEntry->returnFunction = NULL;
                        sendL2caDisconnectCfm(instData->appHandle,
                            instData->reqID, psmEntry->psm,
                            CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                        cidInst = findUsedCidElement(psmEntry);
                        while(cidInst)
                        {
                            CsrBtJsr82RemoveL2capConnection(cidInst->cid, instData);
                            cidInst = findUsedCidElement(psmEntry);
                        }
                        CsrBtJsr82SetIdle(instData);
                    }
                }
                CsrBtJsr82RemoveL2capConnection(prim->btConnId, instData);
            }
            else /* this is a close operation - only this one connection should be affected */
            {
                sendL2caCloseCfm(instData->appHandle, instData->reqID, cidInst->cid,
                    CSR_BT_RESULT_CODE_JSR82_SUCCESS, TRUE);
                CsrBtJsr82RemoveL2capConnection(cidInst->cid, instData);
                CsrBtJsr82SetIdle(instData);
            }

        }
    }
    else
    {
        /* Ignore unknown disconnects. */
    }
}

void CsrBtJsr82CmL2caCancelAcceptConnectCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caCancelConnectAcceptCfm *prim;
    psmValue                            *psmEntry;
 
    prim = (CsrBtCmL2caCancelConnectAcceptCfm *)instData->recvMsg;
    psmEntry = lookupPsm(prim->localPsm, instData->psmList);

    if (psmEntry == NULL)
    {
        CsrBtJsr82SetIdle(instData);
    }
    else
    {
        cidStruct  *cidElement = findConnectableCidElement(psmEntry);

        if (cidElement)
        {
            cidElement->connectable = FALSE;
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_JSR82_IFACEQUEUE, psmEntry->psm); /* stay in BUSY_S */
        }
        else
        {
            
            psmEntry->assigned = FALSE;
            psmEntry->client = FALSE;
            psmEntry->destroy = FALSE;
            psmEntry->returnFunction = NULL;
            cidElement = findUsedCidElement(psmEntry);
            while(cidElement)
            {
                CsrBtJsr82RemoveL2capConnection(cidElement->cid, instData);
                cidElement = findUsedCidElement(psmEntry);
            }
            sendL2caDisconnectCfm(instData->appHandle, instData->reqID,
                psmEntry->psm, CSR_BT_RESULT_CODE_JSR82_CONNECT_FAILED, FALSE);
        
            CsrBtJsr82SetIdle(instData);
        }
    }
}


static void sendL2caTxDataCfm(CsrSchedQid appHandle, CsrUint32 reqID, CsrBtConnId cid,
    CsrBtResultCode  result)
{
    CsrBtJsr82L2caTxDataCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caTxDataCfm));
    msg->type = CSR_BT_JSR82_L2CA_TX_DATA_CFM;
    msg->reqID = reqID;
    msg->btConnId = cid;
    msg->resultCode= result;

    CsrBtJsr82MessagePut(appHandle, msg);
}

void CsrBtJsr82L2caTxDataReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caTxDataReq    *prim;
    jsr82CidElement            *cidElement;

    prim = (CsrBtJsr82L2caTxDataReq *)instData->recvMsg;
    instData->appHandle = prim->appHandle;
    instData->reqID = prim->reqID;

    cidElement = lookupCid(prim->btConnId, instData->psmList);

    if (cidElement && cidElement->cidInst->connected)
    {
        CsrUint8     *sendBuffer;
        CsrUint16    sendSize;
        cidStruct   *cidInst = cidElement->cidInst;

        if (prim->payloadLength > 0)
        {
            sendSize = (CsrUint16)((prim->payloadLength <= cidInst->transmitMtu) ? prim->payloadLength : cidInst->transmitMtu);
            sendBuffer = CsrPmemAlloc(sendSize);
            CsrMemCpy(sendBuffer, prim->payload, sendSize);
        }
        else
        {
            sendBuffer = NULL;
            sendSize = 0;
        }

        CsrBtCml2caDataReqSend(cidInst->cid, sendSize, (void *)sendBuffer);
        cidInst->sendingPacket = TRUE;
        CsrBtJsr82SetBusy(instData);
    }
    else
    {
        sendL2caTxDataCfm(instData->appHandle, instData->reqID, prim->btConnId,
            CSR_BT_RESULT_CODE_JSR82_DATAWRITE_FAIL);
    }
    CsrPmemFree(prim->payload);
}

void CsrBtJsr82CmL2caDataCfmHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caDataCfm  *prim;
    jsr82CidElement     *cidElement;

    prim = (CsrBtCmL2caDataCfm *)instData->recvMsg;

    cidElement = lookupCid(prim->btConnId, instData->psmList);

    if (cidElement && cidElement->cidInst->connected)
    {
        cidStruct   *cidInst   = cidElement->cidInst;
        cidInst->sendingPacket = FALSE;

        if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
            prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
        {
            sendL2caTxDataCfm(instData->appHandle, instData->reqID,
                cidInst->cid, CSR_BT_RESULT_CODE_JSR82_SUCCESS);
        }
        else
        {
            sendL2caTxDataCfm(instData->appHandle, instData->reqID,
                cidInst->cid, CSR_BT_RESULT_CODE_JSR82_DATAWRITE_FAIL);
        }
    }
    else
    {
        sendL2caTxDataCfm(instData->appHandle, instData->reqID, 0,
            CSR_BT_RESULT_CODE_JSR82_DATAWRITE_FAIL);
    }

    CsrBtJsr82SetIdle(instData);
}

static void sendL2caRxDataCfm(CsrSchedQid appHandle, CsrUint32 reqID,
    cidStruct *cidElement, CsrUint8 *payload, CsrBtResultCode result)
{
    CsrBtJsr82L2caRxDataCfm *msg;

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxDataCfm));
    msg->type = CSR_BT_JSR82_L2CA_RX_DATA_CFM;
    msg->reqID = reqID;
    msg->btConnId = cidElement->cid;
    msg->resultCode = result;
    msg->payloadLength = cidElement->receivedDataLength;
    msg->payload = payload;

    CsrBtJsr82MessagePut(appHandle, msg);

    cidElement->receivedDataLength = 0;
}

void CsrBtJsr82L2caRxDataReqHandler(Jsr82MainInstanceData *instData)
{
    /* Check if there's data on the queue, and if so, return now. If not, queue the request */

    CsrBtJsr82L2caRxDataReq *prim;
    jsr82CidElement         *cidElement;

    prim = (CsrBtJsr82L2caRxDataReq *)instData->recvMsg;

    cidElement= lookupCid(prim->btConnId, instData->psmList);

    if (cidElement && cidElement->cidInst->cid)
    {
        CsrUint16           event;
        CsrBtCmL2caDataInd *recvData;
        cidStruct          *cidInst   = cidElement->cidInst;

        if (CsrMessageQueuePop(&cidInst->dataQueue, &event, (void *)(&recvData)))
        {
            cidInst->dataQueueLength--;
            cidInst->receivedDataLength = (CsrUint16)((prim->dataLength < recvData->length) ? prim->dataLength : recvData->length); /* truncate if necessary */
            sendL2caRxDataCfm(prim->appHandle, prim->reqID, cidInst,
                recvData->payload, CSR_BT_RESULT_CODE_JSR82_SUCCESS);
            CsrPmemFree(recvData);
        }
        else
        {
            if (cidInst->connected)
            {
                CsrMessageQueuePush(&cidInst->dataReqQueue, instData->eventClass, instData->recvMsg);
                instData->recvMsg = NULL;
            }
            else
            {
                CsrBtJsr82L2caRxDataCfm *msg;

                msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxDataCfm));
                msg->type = CSR_BT_JSR82_L2CA_RX_DATA_CFM;
                msg->reqID = prim->reqID;
                msg->btConnId = prim->btConnId;
                msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
                msg->payloadLength = 0;
                msg->payload = NULL;
                CsrBtJsr82MessagePut(prim->appHandle, msg);
            }
        }
    }
    else /* no matching btConnId found */
    {
        CsrBtJsr82L2caRxDataCfm *msg;

        msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxDataCfm));
        msg->type = CSR_BT_JSR82_L2CA_RX_DATA_CFM;
        msg->reqID = prim->reqID;
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
        msg->btConnId = prim->btConnId;
        msg->payloadLength = 0;
        msg->payload = NULL;
        CsrBtJsr82MessagePut(prim->appHandle, msg);
    }
}

void CsrBtJsr82CmL2caDataIndHandler(Jsr82MainInstanceData *instData)
{
    /* Check if there's a request on queue - if so, answer it now. Otherwise, queue data */
    CsrBtCmL2caDataInd  *prim       = (CsrBtCmL2caDataInd *)instData->recvMsg;
    jsr82CidElement     *cidElement = lookupCid(prim->btConnId, instData->psmList);

    CsrBtCmL2caDataResSend(prim->btConnId);

    if (cidElement)
    {
        CsrUint16 event;
        CsrBtJsr82L2caRxDataReq *dataReq;
        cidStruct          *cidInst   = cidElement->cidInst;

        if (CsrMessageQueuePop(&cidInst->dataReqQueue, &event, (void *)(&dataReq)))
        {
            cidInst->receivedDataLength = (CsrUint16)((dataReq->dataLength < prim->length) ? dataReq->dataLength : prim->length); /* truncate if necessary */
            sendL2caRxDataCfm(dataReq->appHandle, dataReq->reqID, cidInst,
                prim->payload, CSR_BT_RESULT_CODE_JSR82_SUCCESS);
            CsrPmemFree(dataReq);
        }
        else
        {
            cidInst->dataQueueLength++;
            CsrMessageQueuePush(&cidInst->dataQueue, instData->eventClass, instData->recvMsg);
            instData->recvMsg = NULL;
            while (cidInst->dataQueueLength > CSR_BT_JSR82_MAX_UNPROCESSED_DATA_PACKETS)
            {
                CsrUint16 event;
                CsrBtCmL2caDataInd *recvData;

                if(CsrMessageQueuePop(&cidInst->dataQueue, &event, (void *)(&recvData)))
                {
                    CsrPmemFree(recvData->payload);
                    CsrPmemFree(recvData);
                    cidInst->dataQueueLength--;
                }
            }
        }
    }
    else /* no connection found */
    {
        CsrPmemFree(prim->payload);
    }
}

void CsrBtJsr82L2caRxReadyReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caRxReadyReq   *prim;
    CsrBtJsr82L2caRxReadyCfm   *msg;
    jsr82CidElement            *cidElement;
    prim = (CsrBtJsr82L2caRxReadyReq *)instData->recvMsg;

    cidElement = lookupCid(prim->btConnId, instData->psmList);

    msg = CsrPmemAlloc(sizeof(CsrBtJsr82L2caRxReadyCfm));
    msg->type = CSR_BT_JSR82_L2CA_RX_READY_CFM;
    msg->reqID = prim->reqID;
    msg->btConnId = prim->btConnId;
    msg->dataReady = (CsrBool)((cidElement && cidElement->cidInst->dataQueueLength) ? TRUE : FALSE);
    if (msg->dataReady)
    {
        msg->dataLength = cidElement->cidInst->receivedDataLength;
    }
    else
    {
        msg->dataLength = 0;
    }

    if (cidElement && (cidElement->cidInst->connected || cidElement->cidInst->dataQueueLength))
    {
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
    }
    else
    {
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
    }

    CsrBtJsr82MessagePut(prim->appHandle, msg);
}

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtJsr82CmL2caModeChangeIndHandler(Jsr82MainInstanceData *instData)
{
    CsrBtCmL2caModeChangeInd * prim;
    prim = (CsrBtCmL2caModeChangeInd *) instData->recvMsg;


    /* If we're in park mode - get us into active */
    if(prim->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCml2caModeChangeReqSend(prim->btConnId, CSR_BT_ACTIVE_MODE);
    }
}
#endif
void CsrBtJsr82L2caGetConfigReqHandler(Jsr82MainInstanceData *instData)
{
    CsrBtJsr82L2caGetConfigReq *prim;
    CsrBtJsr82L2caGetConfigCfm *msg;
    jsr82CidElement            *cidElement;

    prim = (CsrBtJsr82L2caGetConfigReq *)instData->recvMsg;
    msg = (CsrBtJsr82L2caGetConfigCfm *)CsrPmemAlloc(sizeof(CsrBtJsr82L2caGetConfigCfm));

    msg->type = CSR_BT_JSR82_L2CA_GET_CONFIG_CFM;
    msg->reqID = prim->reqID;

    cidElement = lookupCid(prim->btConnId, instData->psmList);
    if(cidElement && cidElement->cidInst->connected)
    {
        msg->receiveMTU = cidElement->cidInst->receiveMtu;
        msg->transmitMTU = cidElement->cidInst->transmitMtu;
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_SUCCESS;
    }
    else
    {
        msg->receiveMTU = 0;
        msg->transmitMTU = 0;
        msg->resultCode = CSR_BT_RESULT_CODE_JSR82_DEVICE_NOT_CONNECTED;
    }

    CsrBtJsr82MessagePut(prim->appHandle, msg);
}
