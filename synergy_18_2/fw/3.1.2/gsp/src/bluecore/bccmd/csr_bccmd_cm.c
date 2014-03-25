/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_pmem.h"
#include "csr_bccmd_cm.h"
#include "csr_bccmd_sef.h"
#include "csr_hci_lib.h"
#include "csr_tm_bluecore_private_lib.h"

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
#include "csr_bc_chip_manager_test.h"
#include "csr_transport.h"
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
#include "csr_transport.h"
static CsrBool CsrBccmdCmDeviceAddrEq(CsrBccmdCmDeviceAddrType *bdAddr1,
    CsrBccmdCmDeviceAddrType *bdAddr2)
{
    if ((bdAddr1->lap == bdAddr2->lap) &&
        (bdAddr1->uap == bdAddr2->uap) &&
        (bdAddr1->nap == bdAddr2->nap))
    {
        return TRUE;
    }

    return FALSE;
}

static CsrBool CsrBccmdCmDeviceAddrEqZero(CsrBccmdCmDeviceAddrType *bdAddr)
{
    if ((bdAddr->lap == 0) && (bdAddr->uap == 0) && (bdAddr->nap == 0))
    {
        return TRUE;
    }

    return FALSE;
}

static void CsrBccmdCmDeviceAddrZero(CsrBccmdCmDeviceAddrType *bdAddr)
{
    bdAddr->lap = 0;
    bdAddr->uap = 0;
    bdAddr->nap = 0;
}

#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */

static void csrBccmdCmCancelResTimer(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    /* Make sure that bccmd response timer is cancelled */
    if (cmInst->bccmdResTimerId > 0)
    {
        CsrSchedTimerCancel(cmInst->bccmdResTimerId, NULL, NULL);
        cmInst->bccmdResTimerId = 0;
    }
}

static void csrBccmdCmCancelResetTimer(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    /* Make sure that bccmd reset timer is cancelled */
    if (cmInst->bccmdResetTimerId > 0)
    {
        CsrSchedTimerCancel(cmInst->bccmdResetTimerId, NULL, NULL);
        cmInst->bccmdResetTimerId = 0;
    }
}

static void csrBccmdCmFreeElement(CsrCmnListElm_t *elem) /* Deallocate private pointer before element is removed from list */
{
    CsrBccmdCmListType *element = (CsrBccmdCmListType *) elem;
    if (element->msg) /* A private pointer is allocated, deallocate it */
    {
        CsrPmemFree(element->msg);
        element->msg = NULL;
    }
}

static void csrBccmdCmBlueCoreResetIndSend(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CsrTmBluecoreResetIndSend();
    cmInst->state = CSR_BCCMD_CM_STATE_RESETTING;
}

static void csrBccmdCmResTimeOutHandler(CsrUint16 i, void *v)
{
    CsrBccmdInstanceData *bccmdInst = (CsrBccmdInstanceData *) v;
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CSR_UNUSED(i);
    cmInst->bccmdResTimerId = 0;
    csrBccmdCmBlueCoreResetIndSend(bccmdInst);
}

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
static CsrUint8 *csrBccmdCmBuildQueryMsg(CsrUint16 *payloadLength)
{
    /* Build and send the Bccmd Query message (Read the local device address
       direct to the Controller */
    CsrUint8 *payload = (CsrUint8 *) CsrPmemZalloc(CSR_BCCMD_CM_PSKEY_BDADDR_LEN);
    payload[2] = (CSR_BCCMD_CM_PSKEY_BDADDR_LEN / 2); /* Length in 16 bit words*/
    payload[4] = (CsrUint8) (CSR_BCCMD_CM_PSKEY_SEQNO & 0x00ff);
    payload[5] = (CsrUint8) ((CSR_BCCMD_CM_PSKEY_SEQNO & 0xff00) >> 8);
    payload[6] = 0x03; /* Varid - Persistent Stores */
    payload[7] = 0x70; /* Varid - Persistent Stores */
    payload[10] = 0x01; /* key, 16bit - always PSKEY_BDADDR (0x0001) */
    payload[12] = 0x04; /* Length 16 bit words of the pskey value (0x0004)*/
    payload[14] = 0x08; /* Stores, 16bit - which store to read data. (0x0008) */
    *payloadLength = CSR_BCCMD_CM_PSKEY_BDADDR_LEN;
    return payload;
}

static void csrBccmdCmQuerySend(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    /* Check if the bccmd query message can be send or not */
    if (cmInst->bccmdResTimerId == 0) /* No bccmd messages is pending, send the Bccmd Query message direct to
         the Controller */
    {
        CsrMblk *data;
        CsrUint16 payloadLength;
        CsrUint8 *payload = csrBccmdCmBuildQueryMsg(&payloadLength);

        cmInst->bccmdResTimerId = CsrSchedTimerSet(CSR_BCCMD_CM_RES_TIMEOUT,
            csrBccmdCmResTimeOutHandler,
            0, bccmdInst);

        data = CsrMblkDataCreate(payload, payloadLength, TRUE);
        CsrHciVendorSpecificCommandReqSend(TRANSPORT_CHANNEL_BCCMD, data);
    }
}

static void csrBccmdCmQueryGetBdaddrFromMsg(CsrUint8 *msg,
    CsrBccmdCmDeviceAddrType *bdAddr) /* Take the local device address out of the received Bccmd Query message */
{
    bdAddr->lap = CSR_BCCMD_CM_BDADDR_LAP(msg);
    bdAddr->uap = CSR_BCCMD_CM_BDADDR_UAP(msg);
    bdAddr->nap = CSR_BCCMD_CM_BDADDR_NAP(msg);
}

static CsrBool csrBccmdCmQueryResetDetected(CsrBccmdInstanceData *bccmdInst,
    CsrUint8 *msg) /* Checks if an unexpected reset has occured or not */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    if (!CsrBccmdCmDeviceAddrEqZero(&cmInst->expectedBdaddr))
    {
        CsrBccmdCmDeviceAddrType recvBdaddr;
        csrBccmdCmQueryGetBdaddrFromMsg(msg, &recvBdaddr);
        if (!CsrBccmdCmDeviceAddrEq(&cmInst->expectedBdaddr, &recvBdaddr)) /* An unexpected reset must have occured */
        {
            return TRUE;
        }
    }
    return FALSE;
}

static void csrBccmdCmQuerySavedHandler(CsrBccmdInstanceData *bccmdInst)
{
    /* This function check if the Chip Manager has receive a bccmd message while
       the Bccmd Query message were pending */
    CsrBccmdCmSavedBccmdMsgType *bccmd = &bccmdInst->cmInst->savedBccmd;
    if (bccmd->msg) /* Bccmd message has received while the query message was pending */
    {
        if (CsrBccmdCmReqHandler(bccmdInst, bccmd->msg, bccmd->len,
                bccmd->highPriority))
        { /* Send the save message direct to the BlueCore Chip */
            CsrMblk *data;
            data = CsrMblkDataCreate((CsrUint8 *) bccmd->msg, bccmd->len, TRUE);
            CsrHciVendorSpecificCommandReqSend(TRANSPORT_CHANNEL_BCCMD, data);
            bccmdInst->phandle = bccmd->pHandle;
        }
        else /* The save message must not be send. E.g the Chip Manager must be in
             CSR_BCCMD_CM_STATE_RESETTING state */
        {
            CsrPmemFree(bccmd->msg);
        }
        bccmd->msg = NULL;
    }
}

static CsrBool csrBccmdCmQueryReqHandler(CsrBccmdInstanceData *bccmdInst,
    void *request, CsrUint16 len,
    CsrBool highPriority) /* A bccmd message is receive, check if a Bccmd Query message is pending */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CsrBccmdCmSavedBccmdMsgType *bccmd = &cmInst->savedBccmd;
    CsrUint8 *msg = (CsrUint8 *) request;
    if (cmInst->hciNopDetected) /* Save this Bccmd message as it is receive while a query is pending */
    {
        bccmd->msg = CsrPmemAlloc(len);
        CsrMemCpy(bccmd->msg, request, len);
        bccmd->len = len;
        bccmd->highPriority = highPriority;
        bccmd->pHandle = bccmdInst->phandle;
        return TRUE;
    }
    else /* The Bccmd Query message is not pending */
    {
        if (CSR_BCCMD_CM_IS_BDADDR_SET(msg)) /* This bccmd message will change the local device address. Update
             cmInst->expectedBdaddr */
        {
            csrBccmdCmQueryGetBdaddrFromMsg(msg, &cmInst->expectedBdaddr);
        }
        return FALSE;
    }
}

static CsrBool csrBccmdCmQueryResHandler(CsrBccmdInstanceData *bccmdInst,
    CsrUint8 *msg)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    if (cmInst->hciNopDetected)
    {
        if (CSR_BCCMD_CM_IS_BDADDR_QUERY(msg)) /* This is a confimation on a Bccmd Query message. Return FALSE to
             indicate that this message never must be send to upper layer, as
             this message is own by the Chip Manager */
        {
            if (!csrBccmdCmQueryResetDetected(bccmdInst, msg)) /* Send the save NOP message */
            {
                cmInst->hciNopDetected = FALSE;
            }
            else /* An unexpected reset has occurred. */
            {
                csrBccmdCmBlueCoreResetIndSend(bccmdInst);
            }
            /* Check if the Chip Manager has receive a bccmd message while
               the Bccmd Query message were pending */
            csrBccmdCmQuerySavedHandler(bccmdInst);
            return TRUE;
        }
        else /* The Chip Manager could not send the Query Bccmd message when it
             received the NOP because another Bccmd message were pending. Try
             to send it now. */
        {
            csrBccmdCmQuerySend(bccmdInst);
        }
    }
    return FALSE;
}

#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */

static void csrBccmdCmBlueCorePanicIndSend(CsrBccmdInstanceData *bccmdInst)
{
    CSR_UNUSED(bccmdInst);
    CsrTmBluecorePanicIndSend();
}

static void csrBccmdCmResetTimeOutHandler(CsrUint16 i, void *v) /* The transport could not be started */
{
    CsrBccmdInstanceData *bccmdInst = (CsrBccmdInstanceData *) v;
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    cmInst->bccmdResetTimerId = 0;
    if (cmInst->bccmdResetTimerFired) /* Have try to start the transport with and without init baud rate */
    {
        csrBccmdCmBlueCorePanicIndSend(bccmdInst);
        cmInst->bccmdResetTimerFired = FALSE;
    }
    else /* Try to start the transport with init baud rate */
    {
        cmInst->bccmdResetTimerFired = TRUE;
        csrBccmdCmBlueCoreResetIndSend(bccmdInst);
    }
    CSR_UNUSED(i);
    CSR_UNUSED(v);
}

static void csrBccmdCmErrorResSend(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CSR_BCCMD_CM_FIND_ON_LIST(&(cmInst->bccmdList), bccmdInst);
    CsrCmnListDeinit(&cmInst->bccmdList);
}

static void csrBccmdCmResetToNormalHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;

    cmInst->state = CSR_BCCMD_CM_STATE_NORMAL;
}

static void csrBccmdCmResSend(CsrSchedQid pHandle,
    CsrUint8 *msg,
    CsrUint16 len)
{
    CsrBccmdCfmSend(pHandle, msg, len);
}

void CsrBccmdCmErrorResSend(CsrCmnListElm_t *elem, void *data)
{
    CsrBccmdCmListType *element = (CsrBccmdCmListType *) elem;
    CsrUint8 *msg = (CsrUint8 *) element->msg;
    CsrUint16 len = CSR_BCCMD_CM_LENGTH(msg);
    CsrSchedQid pHandle = element->pHandle;
    csrBccmdCmResSend(pHandle, msg, len);
}

void CsrBccmdCmFreeBccmdElement(CsrCmnListElm_t *elem, void *data)
{
    csrBccmdCmFreeElement(elem);
    CSR_UNUSED(data);
}

CsrBool CsrBccmdCmCompareBccmd(CsrCmnListElm_t *elem, void *value)
{
    CsrUint32 key = *(CsrUint32 *) value;
    CsrBccmdCmListType *element = (CsrBccmdCmListType *) elem;

    if (element->key == key)
    {
        return TRUE;
    }
    return FALSE;
}

/* Public functions */
void CsrBccmdCmInit(CsrBccmdInstanceData *bccmdInst) /* Initialize the Chip Managers instance data */
{
    CsrBccmdCmInstanceData *cmInst = CsrPmemAlloc(sizeof(CsrBccmdCmInstanceData));
    bccmdInst->cmInst = cmInst;
    cmInst->state = CSR_BCCMD_CM_STATE_NORMAL;
    cmInst->bccmdResTimerId = 0;
    cmInst->bccmdResetTimerId = 0;
    cmInst->bccmdResetTimerFired = FALSE;
    CsrCmnListInit(&(cmInst->bccmdList), 0, NULL, csrBccmdCmFreeElement);
    cmInst->activeHighPriorityKey = CSR_BCCMD_CM_KEY_INVALID;
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    cmInst->hciNopDetected = FALSE;
    CsrMemSet(&cmInst->savedBccmd, 0, sizeof(cmInst->savedBccmd));
    CsrBccmdCmDeviceAddrZero(&cmInst->expectedBdaddr);
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */
}

void CsrBccmdCmDeinit(CsrBccmdInstanceData *bccmdInst) /* De-initialize the Chip Managers instance data */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CSR_BCCMD_CM_FREE_ALL_ON_LIST(&(cmInst->bccmdList), bccmdInst);
    CsrCmnListDeinit(&(cmInst->bccmdList));

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    if (cmInst->savedBccmd.msg)
    {
        CsrPmemFree(cmInst->savedBccmd.msg);
    }
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */
    CsrPmemFree(cmInst);
    cmInst = NULL;
}

CsrBool CsrBccmdCmReqHandler(CsrBccmdInstanceData *bccmdInst, void *request,
    CsrUint16 len, CsrBool highPriority)
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CsrUint8 *msg = (CsrUint8 *) request;
    CsrUint16 varId = CSR_BCCMD_CM_VARID(msg);

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    if (csrBccmdCmQueryReqHandler(bccmdInst, request, len, highPriority)) /* The Chip Manager has save the incoming Bccmd message because it is
         receive while a Bccmd Query message is pending. Return FALSE to
         indicate that this command shall not be send to the BlueCore Chip */
    {
        return FALSE;
    }
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */

    if ((varId != CSR_BCCMD_CM_WARM_RESET) && (varId != CSR_BCCMD_CM_COLD_RESET))
    {
#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
        CsrBcCmTestBccmdReqHandler(varId, request, highPriority);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

        if (!highPriority) /* Store copy of bcmmd request msg on input list */
        {
            CsrBccmdCmListType *element = CSR_BCCMD_CM_ADD_TO_LIST(&(cmInst->bccmdList));
            element->key = CSR_BCCMD_CM_KEY(msg);
            element->msg = CsrPmemAlloc(len);
            element->pHandle = bccmdInst->phandle;
            CsrMemCpy(element->msg, msg, len);

            /* Change bccmd request to error response */
            CSR_BCCMD_CM_SET_ERROR_RES(element->msg);

            if (cmInst->state == CSR_BCCMD_CM_STATE_RESETTING) /* The BlueCore Chip has been reset. Return FALSE to indicate
                 that this command shall not be send to the BlueCore Chip */
            {
                return FALSE;
            }
        }
        else
        {
            cmInst->activeHighPriorityKey = CSR_BCCMD_CM_KEY(msg);
        }
    }
    else /* These commands resets the BlueCore chip */
    {
        cmInst->state = CSR_BCCMD_CM_STATE_RESETTING;
    }
    csrBccmdCmCancelResetTimer(bccmdInst);
    csrBccmdCmCancelResTimer(bccmdInst);

    cmInst->bccmdResTimerId = CsrSchedTimerSet(CSR_BCCMD_CM_RES_TIMEOUT,
        csrBccmdCmResTimeOutHandler,
        0, bccmdInst);

    /* Return TRUE to indicate that this Bccmd must be send to the BlueCore */
    return TRUE;
}

CsrBool CsrBccmdCmResHandler(CsrBccmdInstanceData *bccmdInst, void *response) /* Clear from list upon receipt of matching bccmd response msg */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    CsrUint8 *msg = (CsrUint8 *) response;
    CsrUint32 key = CSR_BCCMD_CM_KEY(msg);
    CsrBccmdCmListType *element;

    csrBccmdCmCancelResTimer(bccmdInst);

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    if (csrBccmdCmQueryResHandler(bccmdInst, msg)) /* This response message is a confimation of the Bccmd Query message */
    {
        return FALSE;
    }
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    CsrBcCmTestCmdActivityHandler(TRANSPORT_CHANNEL_BCCMD,
        CSR_BCCMD_CM_VARID(msg),
        CSR_BC_CM_TEST_TYPE_COMPLETE,
        response);
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

    element = CSR_BCCMD_CM_FIND_AND_MATCH_ON_LIST(&(cmInst->bccmdList), &(key));
    if (element)
    {
        if (cmInst->state == CSR_BCCMD_CM_STATE_RESETTING)
        {
            if (key != cmInst->activeHighPriorityKey)
            {
                CsrUint16 cmdLen = (CsrUint16) (2 * ((msg[3] * 256) + msg[2]));
                CsrPmemFree(element->msg);
                element->msg = CsrPmemAlloc(cmdLen);
                CsrMemCpy(element->msg, msg, cmdLen);
                return FALSE;
            }
            else
            {
                cmInst->activeHighPriorityKey = CSR_BCCMD_CM_KEY_INVALID;
            }
        }
        else
        {
            CSR_BCCMD_CM_REMOVE_FROM_LIST(&(cmInst->bccmdList), element);
        }
    }
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    else /* Did not find any matching bccmd */
    {
        if (CSR_BCCMD_CM_IS_BDADDR_QUERY(msg)) /* This response message is a confimation of the Bccmd Query message
             which has trigger an unexpected reset. E.g this response is send
              by the BlueCore Chip after the transport is up running again */
        {
            return FALSE;
        }
    }
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */
    return TRUE;
}

void CsrBccmdCmResendStart(CsrBccmdInstanceData *bccmdInst) /* Begin the seamless reset procedure */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    cmInst->state = CSR_BCCMD_CM_REPLAYING;
    csrBccmdCmErrorResSend(bccmdInst);

#ifdef CSR_CHIP_MANAGER_TEST_ENABLE
    CsrBcCmTestResendStartedHandler();
#endif /* CSR_CHIP_MANAGER_TEST_ENABLE */

    csrBccmdCmResetToNormalHandler(bccmdInst);
}

void CsrBccmdCmResetDetectedRequest(CsrBccmdInstanceData *bccmdInst) /* The BlueCore chip is reset. Find out if the reset is expected or not */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    cmInst->state = CSR_BCCMD_CM_STATE_RESETTING;
    /* Stop and Start bccmd reset timer here and cancel the  bccmd response
       timer */
    csrBccmdCmCancelResTimer(bccmdInst);
    csrBccmdCmCancelResetTimer(bccmdInst);
    cmInst->bccmdResetTimerId = CsrSchedTimerSet(CSR_BCCMD_CM_RESET_TIMEOUT,
        csrBccmdCmResetTimeOutHandler,
        0, bccmdInst);
#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
    if (cmInst->hciNopDetected)
    {
        cmInst->hciNopDetected = FALSE;
    }
    /* Set expected query bdaddr to zero. An unexpected reset has already
       been detected, so when the next NOP arrive we don't check the query
       bdaddr. This avoids generating a new RESET_IND to the CM. */
    CsrBccmdCmDeviceAddrZero(&cmInst->expectedBdaddr);

    /* Check if the Chip Manager has saved a bccmd message while
       the Bccmd Query message were pending */
    csrBccmdCmQuerySavedHandler(bccmdInst);
#endif /* CSR_CHIP_MANAGER_QUERY_ENABLE */
}

void CsrBccmdCmRestartDetectedRequest(CsrBccmdInstanceData *bccmdInst) /* The BlueCore chip is restarted (NOP) after reset */
{
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    csrBccmdCmCancelResetTimer(bccmdInst);
    cmInst->bccmdResetTimerFired = FALSE;
}

#ifdef CSR_CHIP_MANAGER_QUERY_ENABLE
void CsrBccmdQueryReqHandler(CsrBccmdInstanceData *bccmdInst)
{
    CsrBccmdQueryReq *prim = (CsrBccmdQueryReq *) bccmdInst->recvMsgP;
    CsrBccmdCmInstanceData *cmInst = bccmdInst->cmInst;
    cmInst->hciNopDetected = TRUE;
    csrBccmdCmQuerySend(bccmdInst);
}

#endif
#endif /* CSR_CHIP_MANAGER_ENABLE */
