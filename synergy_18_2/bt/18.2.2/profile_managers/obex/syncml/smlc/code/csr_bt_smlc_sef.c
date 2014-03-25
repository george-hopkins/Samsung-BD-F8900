/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_smlc_handler.h"
#include "csr_bt_smlc_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"

#define CSR_BT_SMLC_SYNC_TARGET     "SYNCML-SYNC"
#define CSR_BT_SMLC_SYNC_DM_TARGET  "SYNCML-DM"

/* associate define CSR_BT_SMLC_MIME_DATASYNC_XML */
static const CsrUint8 MIME_DATASYNC_XML[] =    {"application/vnd.syncml+xml"};

/* associate define CSR_BT_SMLC_MIME_DATASYNC_WBXML */
static const CsrUint8 MIME_DATASYNC_WBXML[] =  {"application/vnd.syncml+wbxml"};

/* associate define CSR_BT_SMLC_MIME_DEVMAN_XML */
static const CsrUint8 MIME_DEVMAN_XML[] =      {"application/vnd.syncml.dm+xml"};

/* associate define CSR_BT_SMLC_MIME_DEVMAN_WBXML */
static const CsrUint8 MIME_DEVMAN_WBXML[] =    {"application/vnd.syncml.dm+wbxml"};

#define CSR_BT_SMLC_SYNC_TARGET_LENGTH              0x0B
#define CSR_BT_SMLC_SYNC_DM_TARGET_LENGTH           0x09
#define CSR_BT_SMLC_SYNCMLSERVER_UUID_128_LENGTH    0x10
#define CSR_BT_SMLC_SYNCMLSERVER_UUID_128           {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, \
                                                     0x80, 0x00, 0x00, 0x02, 0xEE, 0x00, 0x00, 0x02}


/* Opcode&PacketLength =3 + ConnectionID=5 + TypeHeader&TypeName=35 +
 * LengthOfObj=5 + BodyHeader=3 + Auth Response=43 */
#define SMLC_OBEX_PACKET_WITHDRAW_TOTAL_MAX         0x005E

static void csrBtSmlcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SMLC_PRIM, msg);
}

static void csrBtSmlcActivateCfmSend(CsrSchedQid             appHandle,
                                     CsrBtResultCode    resultCode,
                                     CsrBool            advEnabled,
                                     CsrBool            acceptEnabled)
{
    CsrBtSmlcActivateCfm *pMsg  = (CsrBtSmlcActivateCfm *)
                                   CsrPmemAlloc(sizeof(CsrBtSmlcActivateCfm));

    pMsg->type                  = CSR_BT_SMLC_ACTIVATE_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = CSR_BT_SUPPLIER_OBEX_PROFILES;
    pMsg->advEnabled            = advEnabled;
    pMsg->acceptEnabled         = acceptEnabled;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcDeactivateCfmSend(CsrSchedQid appHandle)
{
    CsrBtSmlcDeactivateCfm *pMsg  = (CsrBtSmlcDeactivateCfm *)
                                     CsrPmemAlloc(sizeof(CsrBtSmlcDeactivateCfm));

    pMsg->type = CSR_BT_SMLC_DEACTIVATE_CFM;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcConnectCfmSend(CsrSchedQid          appHandle,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier   resultSupplier,
                                    CsrUint16       maxPeerObexPacketLength,
                                    CsrUint16       availablePutBodySize,
                                    CsrBtConnId     btConnId)
{
    CsrBtSmlcConnectCfm *pMsg   = (CsrBtSmlcConnectCfm *)
                                   CsrPmemAlloc(sizeof(CsrBtSmlcConnectCfm));

    pMsg->type                  = CSR_BT_SMLC_CONNECT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->obexPeerMaxPacketSize = maxPeerObexPacketLength;
    pMsg->availablePutBodySize  = availablePutBodySize;
    pMsg->btConnId              = btConnId;

    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcDisconnectIndSend(CsrSchedQid          appHandle,
                                       CsrBtReasonCode reasonCode,
                                       CsrBtSupplier   reasonSupplier)
{
    CsrBtSmlcDisconnectInd *pMsg = (CsrBtSmlcDisconnectInd *)
                                    CsrPmemAlloc(sizeof(CsrBtSmlcDisconnectInd));

    pMsg->type              = CSR_BT_SMLC_DISCONNECT_IND;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcPutNextChunkSmlMsgObjCfmSend(CsrSchedQid                appHandle,
                                                  CsrBtObexResponseCode responseCode,
                                                  CsrUint16             bodySize)
{
    CsrBtSmlcPutNextChunkSmlMsgObjCfm *pMsg = (CsrBtSmlcPutNextChunkSmlMsgObjCfm *)
                                               CsrPmemAlloc(sizeof(CsrBtSmlcPutNextChunkSmlMsgObjCfm));

    pMsg->type          = CSR_BT_SMLC_PUT_NEXT_CHUNK_SML_MSG_OBJ_CFM;
    pMsg->responseCode  = responseCode;
    pMsg->maxBodySize   = bodySize;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcPutSmlMsgObjCfmSend(CsrSchedQid                 appHandle,
                                         CsrBtObexResponseCode  responseCode,
                                         CsrUint16              bodySize)

{
    CsrBtSmlcPutSmlMsgObjCfm *pMsg = (CsrBtSmlcPutSmlMsgObjCfm *)
                                      CsrPmemAlloc(sizeof(CsrBtSmlcPutSmlMsgObjCfm));

    pMsg->type          = CSR_BT_SMLC_PUT_SML_MSG_OBJ_CFM;
    pMsg->responseCode  = responseCode;
    pMsg->maxBodySize   = bodySize;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcGetSmlMsgObjCfmSend(CsrSchedQid                 appHandle,
                                         CsrUint8               finalFlag,
                                         CsrBtObexResponseCode  responseCode,
                                         CsrUint32              lengthOfObj,
                                         CsrUint16              bodyLength,
                                         CsrUint16              bodyOffset,
                                         CsrUint8               *payload,
                                         CsrUint16              payloadLength)
{
    CsrBtSmlcGetSmlMsgObjCfm *pMsg = (CsrBtSmlcGetSmlMsgObjCfm *)
                                      CsrPmemAlloc(sizeof(CsrBtSmlcGetSmlMsgObjCfm));

    pMsg->type              = CSR_BT_SMLC_GET_SML_MSG_OBJ_CFM;
    pMsg->finalFlag         = finalFlag;
    pMsg->responseCode      = responseCode;
    pMsg->lengthOfObject    = lengthOfObj;
    pMsg->bodyLength        = bodyLength;
    pMsg->bodyOffset        = bodyOffset;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcGetNextSmlMsgObjCfmSend(CsrSchedQid                 appHandle,
                                             CsrUint8               finalFlag,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrUint16              bodyLength,
                                             CsrUint16              bodyOffset,
                                             CsrUint8               *payload,
                                             CsrUint16              payloadLength)
{
    CsrBtSmlcGetNextChunkSmlMsgObjCfm *pMsg = (CsrBtSmlcGetNextChunkSmlMsgObjCfm *)
                                               CsrPmemAlloc(sizeof(CsrBtSmlcGetNextChunkSmlMsgObjCfm));

    pMsg->type          = CSR_BT_SMLC_GET_NEXT_CHUNK_SML_MSG_OBJ_CFM;
    pMsg->finalFlag     = finalFlag;
    pMsg->responseCode  = responseCode;
    pMsg->bodyLength    = bodyLength;
    pMsg->bodyOffset    = bodyOffset;
    pMsg->payload       = payload;
    pMsg->payloadLength = payloadLength;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcSecurityInCfmSend(CsrSchedQid           appHandle,
                                       CsrBtResultCode  resultCode,
                                       CsrBtSupplier    resultSupplier)
{
    CsrBtSmlcSecurityInCfm *pMsg = (CsrBtSmlcSecurityInCfm *)
                                    CsrPmemAlloc(sizeof(CsrBtSmlcSecurityInCfm));

    pMsg->type                   = CSR_BT_SMLC_SECURITY_IN_CFM;
    pMsg->resultCode             = resultCode;
    pMsg->resultSupplier         = resultSupplier;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcSecurityOutCfmSend(CsrSchedQid          appHandle,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier   resultSupplier)
{
    CsrBtSmlcSecurityOutCfm * pMsg = (CsrBtSmlcSecurityOutCfm*)
                                      CsrPmemAlloc(sizeof(CsrBtSmlcSecurityOutCfm));

    pMsg->type                     = CSR_BT_SMLC_SECURITY_OUT_CFM;
    pMsg->resultCode               = resultCode;
    pMsg->resultSupplier           = resultSupplier;
    csrBtSmlcMessagePut(appHandle, pMsg);
}

static void csrBtSmlcConnectResultHandler(void                     *instData,
                                          CsrBtResultCode          resultCode,
                                          CsrBtSupplier            resultSupplier,
                                          CsrBtDeviceAddr          deviceAddr,
                                          CsrBtConnId              cid,
                                          CsrUint16                maxPeerObexPacketLength,
                                          CmnCsrBtLinkedListStruct *sdpTag,
                                          CsrUint16                obexPacketLength,
                                          CsrUint8               *obexPacket)
{
    SmlcInstanceData * pInst    = (SmlcInstanceData *) instData;
    pInst->availablePutBodySize = (CsrUint16)(maxPeerObexPacketLength - SMLC_OBEX_PACKET_WITHDRAW_TOTAL_MAX);

    if (pInst->activated)
    {
        csrBtSmlcConnectCfmSend(pInst->appHandle,
                                resultCode,
                                resultSupplier,
                                maxPeerObexPacketLength,
                                pInst->availablePutBodySize,
                                cid);
        if (resultCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        {
            pInst->cliBusy = FALSE;
        }
    }
    else
    {
        csrBtSmlcDeactivateCfmSend(pInst->appHandle);
    }

    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlcDisconnectResultHandler(void               *instData,
                                             CsrBtReasonCode    reasonCode,
                                             CsrBtSupplier      reasonSupplier,
                                             CsrUint8         *obexPacket,
                                             CsrUint16          obexPacketLength)
{
    SmlcInstanceData * pInst = (SmlcInstanceData *) instData;
    csrBtSmlcDisconnectIndSend(pInst->appHandle, reasonCode, reasonSupplier);
    CsrPmemFree(pInst->body);
    pInst->body     = NULL;
    pInst->cliBusy  = FALSE;
    if (!pInst->activated)
    {
        csrBtSmlcDeactivateCfmSend(pInst->appHandle);
    }
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlcAuthenticateIndHandler(void            *instData,
                                            CsrBtDeviceAddr deviceAddr,
                                            CsrUint8        options,
                                            CsrUint16       realmLength,
                                            CsrUint8      *realm)
{
    SmlcInstanceData * pInst = (SmlcInstanceData *) instData;

    CsrBtSmlcAuthenticateInd *pMsg  = (CsrBtSmlcAuthenticateInd *)
                                       CsrPmemAlloc(sizeof(CsrBtSmlcAuthenticateInd));

    pMsg->type          = CSR_BT_SMLC_AUTHENTICATE_IND;
    pMsg->deviceAddr    = deviceAddr;
    pMsg->options       = options;
    pMsg->realmLength   = realmLength;
    pMsg->realm         = realm;
    csrBtSmlcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSmlcAbortResultHandler(void                   *instData,
                                        CsrBtObexResponseCode  responseCode,
                                        CsrUint8             *obexPacket,
                                        CsrUint16              obexPacketLength)
{
    SmlcInstanceData  *pInst = (SmlcInstanceData *) instData;
    CsrBtSmlcAbortCfm *pMsg  = (CsrBtSmlcAbortCfm*)
                                CsrPmemAlloc(sizeof(CsrBtSmlcAbortCfm));

    pMsg->type               = CSR_BT_SMLC_ABORT_CFM;
    csrBtSmlcMessagePut(pInst->appHandle, pMsg);
    CsrPmemFree(pInst->body);
    pInst->body = NULL;
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlcPutMsgObjContinueHandler(void      *instData,
                                              CsrUint16 obexPacketLength)
{
    SmlcInstanceData  *pInst = (SmlcInstanceData *) instData;

    if (pInst->body)
    {
        CsrUint8 returnValue = ObexUtilPutContinueRequest(pInst->obexCliInst,
                                                          pInst->finalFlag,
                                                          &pInst->body,
                                                          pInst->bodyLength);

        if (returnValue == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtSmlcStateEventException(CSR_BT_SMLC_PRIM, 0);
        }
    }
    else
    {
        if (pInst->headerCfm)
        {
            pInst->headerCfm = FALSE;
            csrBtSmlcPutSmlMsgObjCfmSend(pInst->appHandle,
                                         CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                         obexPacketLength);
        }
        else
        {
            csrBtSmlcPutNextChunkSmlMsgObjCfmSend(pInst->appHandle,
                                                  CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                                  obexPacketLength);
        }
    }
}

static void csrBtSmlcPutMsgObjResultHandler(void                  *instData,
                                            CsrBtObexResponseCode responseCode,
                                            CsrUint8            *obexPacket,
                                            CsrUint16             obexPacketLength)
{
    SmlcInstanceData  *pInst = (SmlcInstanceData *) instData;

    if (pInst->headerCfm)
    {
        csrBtSmlcPutSmlMsgObjCfmSend(pInst->appHandle,
                                     responseCode,
                                     pInst->availablePutBodySize);
    }
    else
    {
        csrBtSmlcPutNextChunkSmlMsgObjCfmSend(pInst->appHandle,
                                              responseCode,
                                              pInst->availablePutBodySize);
    }
    CsrPmemFree(pInst->body);
    CsrPmemFree(obexPacket);
    pInst->body = NULL;
    CSR_UNUSED(obexPacketLength);
}

static CsrUint32 csrBtSmlcGetLengthOfObject(CsrUint8            *obexPacket,
                                            CsrBtObexResponseCode responseCode)
{
    CsrUint32 lengthOfObj;

    if(obexPacket &&
       responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
       CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &lengthOfObj))
    {
        return (lengthOfObj);
    }
    return (0);
}

static void csrBtSmlcGetSmlMsgObjResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrBool               bodyHeader,
                                               CsrUint16             bodyLength,
                                               CsrUint16             bodyOffset,
                                               CsrUint8            *obexPacket,
                                               CsrUint16             obexPacketLength)
{
    CsrBool     isFinal;

    SmlcInstanceData *pInst = (SmlcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        isFinal      = FALSE;
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else
    {
        isFinal = TRUE;
    }

    if (pInst->headerCfm)
    {
        CsrUint32   lengthOfObj = csrBtSmlcGetLengthOfObject(obexPacket, responseCode);
        pInst->headerCfm        = FALSE;

        csrBtSmlcGetSmlMsgObjCfmSend(pInst->appHandle,
                                     isFinal,
                                     responseCode,
                                     lengthOfObj,
                                     bodyLength,
                                     bodyOffset,
                                     obexPacket,
                                     obexPacketLength);
    }
    else
    {
        csrBtSmlcGetNextSmlMsgObjCfmSend(pInst->appHandle,
                                         isFinal,
                                         responseCode,
                                         bodyLength,
                                         bodyOffset,
                                         obexPacket,
                                         obexPacketLength);
    }
}

static CsrUint8 * csrBtSmlcBuildTarget(CsrBool targetService, CsrUint8 *targetHeaderLength)
{
    CsrUint8   *target  = NULL;
    *targetHeaderLength = 0;

    if (targetService == CSR_BT_SMLC_SYNCMLSYNC_SERVICE)
    {
        target              = (CsrUint8 *) CsrPmemAlloc(CSR_BT_SMLC_SYNC_TARGET_LENGTH);
        *targetHeaderLength = CSR_BT_SMLC_SYNC_TARGET_LENGTH;
        CsrMemCpy(target, (CsrUint8 *) CSR_BT_SMLC_SYNC_TARGET, CSR_BT_SMLC_SYNC_TARGET_LENGTH);
    }
    else
    {
        target              = (CsrUint8 *) CsrPmemAlloc(CSR_BT_SMLC_SYNC_DM_TARGET_LENGTH);
        *targetHeaderLength = CSR_BT_SMLC_SYNC_DM_TARGET_LENGTH;
        CsrMemCpy(target, (CsrUint8 *)CSR_BT_SMLC_SYNC_DM_TARGET, CSR_BT_SMLC_SYNC_DM_TARGET_LENGTH);
    }
    return (target);
}

static const CsrUint8 *csrBtSmlcMimeDefToMimeTextP(CsrUint8 mimeDef)
{
    switch(mimeDef)
    {
        case CSR_BT_SMLC_MIME_DATASYNC_XML:
        {
            return MIME_DATASYNC_XML;
        }
        case CSR_BT_SMLC_MIME_DATASYNC_WBXML:
        {
            return MIME_DATASYNC_WBXML;
        }
        case CSR_BT_SMLC_MIME_DEVMAN_XML:
        {
            return MIME_DEVMAN_XML;
        }
        case CSR_BT_SMLC_MIME_DEVMAN_WBXML:
        {
            return MIME_DEVMAN_WBXML;
        }
        default:
        {
            return NULL;
        }
    }
}

CsrUint8 CsrBtSmlcActivateReqHandler(SmlcInstanceData *pInst, void *msg)
{
    if(!pInst->activated)
    {
        CsrBtSmlcActivateReq *pMsg      = (CsrBtSmlcActivateReq *) msg;
        CsrBtResultCode      resultCode = CSR_BT_RESULT_CODE_OBEX_INTERNAL_ERROR;

        if (!pMsg->advEnable && !pMsg->acceptServConnect)
        {
            pInst->activated = TRUE;
            pInst->appHandle = pMsg->appHandle;
            resultCode       = CSR_BT_RESULT_CODE_OBEX_SUCCESS;
        }
        csrBtSmlcActivateCfmSend(pMsg->appHandle, resultCode, pMsg->advEnable, pMsg->acceptServConnect);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
    else
    {
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

CsrUint8 CsrBtSmlcDeactivateReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);

    if(pInst->activated)
    {
        pInst->activated = FALSE;
        if (pInst->cliBusy)
        {
            return (ObexUtilDisconnectRequest(pInst->obexCliInst, TRUE, NULL));
        }
        else
        {
            csrBtSmlcDeactivateCfmSend(pInst->appHandle);
            return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
        }
    }
    else
    {
        return (CSR_BT_OBEX_UTIL_STATUS_EXCEPTION);
    }
}

CsrUint8 CsrBtSmlcConnectReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrUint8 returnValue = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;

    if (pInst->activated)
    {
        CsrUint16  index;
        CsrUint8   targetLength;
        CsrUint8   headerPriorityTable[2]   = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
        CsrUint8   numOfHeaders             = 0;
        CsrBtSmlcConnectReq *pMsg           = (CsrBtSmlcConnectReq *) msg;
        CmnCsrBtLinkedListStruct    *sdpTag = NULL;
        ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(pMsg->authorize,
                                                                                   &pMsg->password,
                                                                                   pMsg->passwordLength, 
                                                                                   &pMsg->userId,
                                                                                   &pMsg->realm,
                                                                                   pMsg->realmLength);
        CsrUint8   *target                  = csrBtSmlcBuildTarget(pMsg->targetService, &targetLength);
        CsrBtUuid128 uuid = CSR_BT_SMLC_SYNCMLSERVER_UUID_128;

        if (pMsg->count > 0)
        {
            numOfHeaders++;
        }

        if (pMsg->length > 0)
        {
            numOfHeaders++;
        }

        pInst->localMaxPacketSize = pMsg->maxPacketSize;
        pInst->windowSize = pMsg->windowSize;
        pInst->localSrmEnable = pMsg->srmEnable;

        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid128(sdpTag, &uuid, &index);
        CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

        returnValue = ObexUtilConnectRequest(pInst->obexCliInst,
                                             &sdpTag,
                                             pMsg->destination,
                                             pInst->secOutgoing,
                                             pInst->localMaxPacketSize,
                                             CSR_BT_SMLC_PROFILE_DEFAULT_MTU_SIZE,
                                             &target,
                                             targetLength,
                                             numOfHeaders,
                                             headerPriorityTable,
                                             CSR_BT_SMLC_LP_SUPERVISION_TIMEOUT,
                                             &chal,
                                             pMsg->length,
                                             pMsg->count,
                                             NULL,
                                             0,
                                             NULL,
                                             csrBtSmlcConnectResultHandler,
                                             csrBtSmlcAuthenticateIndHandler,
                                             csrBtSmlcDisconnectResultHandler,
                                             NULL,
                                             pInst->windowSize,
                                             pInst->localSrmEnable);

        if (returnValue == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        {
            pInst->cliBusy = TRUE;
        }
    }
    return (returnValue);
}

CsrUint8 CsrBtSmlcAbortReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilAbortRequest(pInst->obexCliInst, NULL, csrBtSmlcAbortResultHandler));
}


CsrUint8 CsrBtSmlcPutSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6] = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                       CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtSmlcPutSmlMsgObjReq *pMsg  = (CsrBtSmlcPutSmlMsgObjReq *) msg;

    pInst->finalFlag                = pMsg->finalFlag;
    pInst->bodyLength               = pMsg->bodyLength;
    pInst->body                     = pMsg->body;
    pInst->headerCfm                = TRUE;

    if (pMsg->lengthOfObject == 0)
    {
        headerPriorityTable[1] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }

    return(ObexUtilPutRequest(pInst->obexCliInst,
                              headerPriorityTable,
                              pMsg->lengthOfObject,
                              csrBtSmlcMimeDefToMimeTextP(pMsg->mimeType),
                              NULL,
                              NULL,
                              0,
                              NULL,
                              0,
                              NULL,
                              csrBtSmlcPutMsgObjContinueHandler,
                              csrBtSmlcPutMsgObjResultHandler));
}

CsrUint8 CsrBtSmlcPutNextChunkSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcPutNextChunkSmlMsgObjReq *pMsg = (CsrBtSmlcPutNextChunkSmlMsgObjReq *) msg;
    return(ObexUtilPutContinueRequest(pInst->obexCliInst, pMsg->finalFlag, &pMsg->body, pMsg->bodyLength));
}

CsrUint8 CsrBtSmlcGetSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcGetSmlMsgObjReq *pMsg   = (CsrBtSmlcGetSmlMsgObjReq *) msg;

    if(pMsg->mimeType < CSR_BT_SMLC_MIME_INVALID_TO_BIG)
    {
        CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

        pInst->headerCfm = TRUE;

        return(ObexUtilGetRequest(pInst->obexCliInst,
                                  headerPriorityTable,
                                  csrBtSmlcMimeDefToMimeTextP(pMsg->mimeType),
                                  NULL,
                                  NULL,
                                  0,
                                  NULL,
                                  0,
                                  NULL,
                                  CSR_BT_OBEX_SRMP_INVALID,
                                  csrBtSmlcGetSmlMsgObjResultHandler,
                                  NULL));
    }
    else
    {
        csrBtSmlcGetSmlMsgObjCfmSend(pInst->appHandle,
                                     TRUE,
                                     CSR_BT_OBEX_UNSUPPORTED_MEDIA_RESPONSE_CODE,
                                     0,
                                     0,
                                     0,
                                     NULL,
                                     0);

        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }
}

CsrUint8 CsrBtSmlcGetNextChunkSmlMsgObjReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilGetContinueRequest(pInst->obexCliInst, CSR_BT_OBEX_SRMP_INVALID));
}


CsrUint8 CsrBtSmlcAuthenticateResHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcAuthenticateRes *pMsg = (CsrBtSmlcAuthenticateRes *) msg;
    return(ObexUtilCliAuthenticateResponse(pInst->obexCliInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId, NULL));
}

CsrUint8 CsrBtSmlcCancelConnectReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexCliInst, FALSE, NULL));
}

CsrUint8 CsrBtSmlcDisconnectReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcDisconnectReq *pMsg = (CsrBtSmlcDisconnectReq *) msg;

    return (ObexUtilDisconnectRequest(pInst->obexCliInst, pMsg->normalDisconnect, NULL));
}

CsrUint8 CsrBtSmlcSecurityInReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcSecurityInReq *pMsg = (CsrBtSmlcSecurityInReq *) msg;
    csrBtSmlcSecurityInCfmSend(pMsg->appHandle, CSR_BT_RESULT_CODE_OBEX_SUCCESS, CSR_BT_SUPPLIER_OBEX_PROFILES);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtSmlcSecurityOutReqHandler(SmlcInstanceData *pInst, void *msg)
{
    CsrBtSmlcSecurityOutReq *pMsg = (CsrBtSmlcSecurityOutReq *) msg;

    CsrBtResultCode rval = CsrBtScSetSecOutLevel(&pInst->secOutgoing,
                                                 pMsg->secLevel,
                                                 CSR_BT_OBEX_SYNCML_TRANSFER_MANDATORY_SECURITY_INCOMING,
                                                 CSR_BT_OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING,
                                                 CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                 CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtSmlcSecurityOutCfmSend(pMsg->appHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

