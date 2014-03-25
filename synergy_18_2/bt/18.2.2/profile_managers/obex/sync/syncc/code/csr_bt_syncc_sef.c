/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_syncc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_exceptionhandler.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

/* SYNC OBEX server needed stuff begin */
static void csrBtSynccServerConnectIndHandler(   void               *instData,
                                                 CsrBtDeviceAddr    deviceAddr,
                                                 CsrBtConnId        cid,
                                                 CsrUint16          maxPeerObexPacketLength,
                                                 CsrUint16          obexResponsePacketLength,
                                                 CsrUint32          length,
                                                 CsrUint32          count,
                                                 CsrUint16          targetHeaderLength,
                                                 CsrUint8           *target,
                                                 CsrBool            authenticated,
                                                 CsrBool            challenged,
                                                 CsrUint16          obexPacketLength,
                                                 CsrUint8           *obexPacket);

static void csrBtSynccServerPutIndHandler(       void               *instData,
                                                 CsrUint16          obexResponsePacketLength,
                                                 CsrBool            authenticated,
                                                 CsrBool            finalBitSet,
                                                 CsrUint16          bodyLength,
                                                 CsrUint16          bodyOffset,
                                                 CsrUint16          obexPacketLength,
                                                 CsrUint8           *obexPacket);

static void csrBtSynccServerAbortIndHandler(     void               *instData,
                                                 CsrUint16          descriptionOffset,
                                                 CsrUint16          descriptionLength,
                                                 CsrUint16          obexPacketLength,
                                                 CsrUint8           *obexPacket);

static void csrBtSynccServerDisconnectIndHandler(void               *instData,
                                                 CsrBtDeviceAddr    deviceAddr,
                                                 CsrBtReasonCode    reasonCode,
                                                 CsrBtSupplier      reasonSupplier,
                                                 CsrUint16          obexPacketLength,
                                                 CsrUint8           *obexPacket);

static void csrBtSynccServerActivateCfmHandler(  void               *instData,
                                                 CsrUint8           serverChannel,
                                                 psm_t              psm,
                                                 CsrBtResultCode    resultCode,
                                                 CsrBtSupplier      resultSupplier);
/* SYNC OBEX server needed stuff end */


#define CSR_BT_OBEX_SYNCC_TARGET            {'I','R','M','C','-','S','Y','N','C'}
#define CSR_BT_OBEX_SYNCC_TARGET_LENGTH     0x09

static CsrUint16 csrBtSynccExtractSupportedDataStores(CmnCsrBtLinkedListStruct * sdpTag)
{
    CsrUint16 supportedStores = CSR_BT_SYNC_NO_TYPE_SUPPORT;

    if (sdpTag)
    {
        CsrUint16 bchsResult, bchsTask, bchsTaskResult;
        CsrBtUuid32    service;

        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTag, 0, &service,
                                                  &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                CsrUint32 dataLen;
                CsrUint8  *data;

                if (CsrBtUtilSdrGetDataElementSequenceDataAttributeFromAttributeUuid(sdpTag, 0,
                                                                                     CSR_BT_SUPPORTED_DATA_STORES_LIST_ATTRIBUTE_IDENTIFIER, &data, &dataLen))
                {
                    CsrUint32 i;

                    for (i = 1; i < dataLen; i+=2)
                    {
                        if (data[i] == 0x01)
                        {
                            supportedStores |= CSR_BT_SYNC_PHONEBOOK_SUPPORT;
                        }
                        else if (data[i] == 0x03)
                        {
                            supportedStores |= CSR_BT_SYNC_CALENDAR_SUPPORT;
                        }
                        else if (data[i] == 0x05)
                        {
                            supportedStores |= CSR_BT_SYNC_NOTE_SUPPORT;
                        }
                        else if (data[i] == 0x06)
                        {
                            supportedStores |= CSR_BT_SYNC_MESSAGE_SUPPORT;
                        }
                        else
                        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrGeneralWarning("SYNCC", 0, 0, "Unexpected supported store value in SDP record\n");
#endif
                        }
                    }
                }
            }
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrGeneralException("SYNCC", 0, 0, "Unsupported service in SDP\n");
#endif
        }
    }
    return (supportedStores);
}

static void csrBtSynccExtractApplicationResponse(CsrUint8 *obexPacket, CsrUint16 packetLength, CsrUtf8String **luid, 
                                                 CsrUtf8String **changeCounter, CsrUtf8String **timeStamp)
{
    *luid = NULL;
    *changeCounter = NULL;
    *timeStamp = NULL;

    if (obexPacket)
    {
        CsrUint16 index = 0;
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrUint16 headerLength = 0;
#endif

        while(index < packetLength)
        {
            if(obexPacket[index++] == CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER)
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                headerLength = (CsrUint16)CSR_BT_OBEX_GET_WORD_AT(obexPacket, index);
#endif
                break;
            }
        }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        if(headerLength)
#endif
        {
            for(index += 2; index <packetLength; index++)
            {
                if(obexPacket[index] == 0x01)
                { /* Application response: LUID */
                    if(++index < packetLength)
                    {
                        CsrUint8 luidLength = obexPacket[index++];

                        if(luidLength + index <= packetLength)
                        {
                            *luid = CsrPmemAlloc(luidLength+1);
                            CsrMemCpy(*luid, &obexPacket[index], luidLength);
                            CsrUtf8StrTruncate(*luid, luidLength);
                            index += luidLength-1;
                        }
                    }
                }
                else if(obexPacket[index] == 0x02)
                { /* Application response: Change Counter */
                    if(++index < packetLength)
                    {
                        CsrUint8 changeCounterLength = obexPacket[index++];

                        if(changeCounterLength + index <= packetLength)
                        {
                            *changeCounter = CsrPmemAlloc(changeCounterLength+1);
                            CsrMemCpy(*changeCounter, &obexPacket[index], changeCounterLength);
                            CsrUtf8StrTruncate(*changeCounter, changeCounterLength);
                            index += changeCounterLength-1;
                        }
                    }
                }
                else if(obexPacket[index] == 0x03)
                { /* Application response: Timestamp */
                    if(++index < packetLength)
                    {
                        CsrUint8 timestampLength = obexPacket[index++];

                        if(timestampLength + index <= packetLength)
                        {
                            *timeStamp = CsrPmemAlloc(timestampLength+1);
                            CsrMemCpy(*timeStamp, &obexPacket[index], timestampLength);
                            CsrUtf8StrTruncate(*timeStamp, timestampLength);
                            index += timestampLength-1;
                        }
                    }
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else
                {
                    CsrGeneralException("SYNCC", 0, 0, "Unknown application response parameter\n");
                }
#endif
            }
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        else
        {
            CsrGeneralException("SYNCC", 0, 0, "Application response header is not found\n");
        }
#endif
    }

    CsrPmemFree(obexPacket);
}

static void csrBtSynccMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SYNCC_PRIM, msg);
}

static void csrBtSynccConnectResultHandler(void                     * instData,
                                           CsrBtResultCode          resultCode,
                                           CsrBtSupplier            resultSupplier,
                                           CsrBtDeviceAddr          deviceAddr,
                                           CsrBtConnId               cid,
                                           CsrUint16                maxPeerObexPacketLength,
                                           CmnCsrBtLinkedListStruct * sdpTag,
                                           CsrUint16                obexPacketLength,
                                           CsrUint8                 * obexPacket)
{
    SynccInstanceData * pInst   = instData;
    CsrBtSynccConnectCfm *pMsg  = (CsrBtSynccConnectCfm *) CsrPmemAlloc(sizeof(CsrBtSynccConnectCfm));
    pMsg->type                  = CSR_BT_SYNCC_CONNECT_CFM;
    pMsg->obexPeerMaxPacketSize = maxPeerObexPacketLength;
    pMsg->supportedStores       = csrBtSynccExtractSupportedDataStores(sdpTag);
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->btConnId              = cid;

    csrBtSynccMessagePut(pInst->appHandle, pMsg);

#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = cid;
#endif /* CSR_AMP_ENABLE */
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtSynccDisconnectResultHandler(void                  * instData,
                                              CsrBtReasonCode       reasonCode,
                                              CsrBtSupplier         reasonSupplier,
                                              CsrUint8              * obexPacket,
                                              CsrUint16             obexPacketLength)
{
    SynccInstanceData * pInst     = instData;
    CsrBtSynccDisconnectInd *pMsg = (CsrBtSynccDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtSynccDisconnectInd));
    pMsg->type                    = CSR_BT_SYNCC_DISCONNECT_IND;
    pMsg->reasonCode              = reasonCode;
    pMsg->reasonSupplier          = reasonSupplier;
    csrBtSynccMessagePut(pInst->appHandle, pMsg);

#ifdef CSR_AMP_ENABLE
    pInst->masterBtConnId = 0;
#endif /* CSR_AMP_ENABLE */
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSynccAuthenticateIndHandler(void          * instData,
                                             CsrBtDeviceAddr  deviceAddr,
                                             CsrUint8       options,
                                             CsrUint16      realmLength,
                                             CsrUint8    * realm)
{
    SynccInstanceData * pInst = instData;

    CsrBtSynccAuthenticateInd *pMsg = (CsrBtSynccAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtSynccAuthenticateInd));

    pMsg->type                  = CSR_BT_SYNCC_AUTHENTICATE_IND;
    pMsg->deviceAddr            = deviceAddr;
    pMsg->options               = options;
    pMsg->realmLength           = realmLength;
    pMsg->realm                 = realm;
    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccGetObjectResultHandler(void                   *instData,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrBool                bodyHeader,
                                             CsrUint16              bodyLength,
                                             CsrUint16              bodyOffset,
                                             CsrUint8             *obexPacket,
                                             CsrUint16              obexPacketLength)
{
    SynccInstanceData * pInst = instData;

    if (responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        CsrBtSynccGetObjectCfm *pMsg      = (CsrBtSynccGetObjectCfm *) CsrPmemAlloc(sizeof(CsrBtSynccGetObjectCfm));
        pMsg->type              = CSR_BT_SYNCC_GET_OBJECT_CFM;
        pMsg->responseCode      = responseCode;
        pMsg->bodyLength        = bodyLength;
        pMsg->bodyOffset        = bodyOffset;
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        csrBtSynccMessagePut(pInst->appHandle, pMsg);

    }
    else
    {
        CsrBtSynccGetObjectInd *pMsg  = (CsrBtSynccGetObjectInd *) CsrPmemAlloc(sizeof(CsrBtSynccGetObjectInd));
        pMsg->type              = CSR_BT_SYNCC_GET_OBJECT_IND;
        pMsg->bodyLength        = bodyLength;
        pMsg->bodyOffset        = bodyOffset;
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        csrBtSynccMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtSynccAddObjectResultHandler(void                   * instData,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrUint8             * obexPacket,
                                             CsrUint16               obexPacketLength)
{
    SynccInstanceData *pInst     = instData;
    CsrBtSynccAddObjectCfm *pMsg = (CsrBtSynccAddObjectCfm *) CsrPmemAlloc(sizeof(CsrBtSynccAddObjectCfm));
    pMsg->type                   = CSR_BT_SYNCC_ADD_OBJECT_CFM;
    pMsg->responseCode           = responseCode;

    csrBtSynccExtractApplicationResponse(obexPacket, obexPacketLength, &pMsg->luid, &pMsg->changeCounter, &pMsg->timeStamp);

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccAddObjectContinueHandler(void  *    instData,
                                               CsrUint16   obexPacketLength)
{
    SynccInstanceData *pInst    = instData;
    CsrBtSynccAddObjectInd *pMsg     = (CsrBtSynccAddObjectInd *) CsrPmemAlloc(sizeof(CsrBtSynccAddObjectInd));

    pMsg->type                  = CSR_BT_SYNCC_ADD_OBJECT_IND;
    pMsg->maxAllowedPayloadSize = obexPacketLength;

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccModifyObjectResultHandler(void                    * instData,
                                                CsrBtObexResponseCode   responseCode,
                                                CsrUint8              * obexPacket,
                                                CsrUint16                obexPacketLength)
{
    SynccInstanceData *pInst    = instData;
    CsrBtSynccModifyObjectCfm *pMsg  = (CsrBtSynccModifyObjectCfm *) CsrPmemAlloc(sizeof(CsrBtSynccModifyObjectCfm));

    pMsg->type                  = CSR_BT_SYNCC_MODIFY_OBJECT_CFM;
    pMsg->responseCode          = responseCode;

    csrBtSynccExtractApplicationResponse(obexPacket, obexPacketLength, &pMsg->luid, &pMsg->changeCounter, &pMsg->timeStamp);

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccModifyObjectContinueHandler(void  *    instData,
                                                  CsrUint16   obexPacketLength)
{
    SynccInstanceData *pInst    = instData;
    CsrBtSynccModifyObjectInd *pMsg  = (CsrBtSynccModifyObjectInd *) CsrPmemAlloc(sizeof(CsrBtSynccModifyObjectInd));

    pMsg->type                  = CSR_BT_SYNCC_MODIFY_OBJECT_IND;
    pMsg->maxAllowedPayloadSize = obexPacketLength;

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccDeleteObjectResultHandler(void                    * instData,
                                                CsrBtObexResponseCode   responseCode,
                                                CsrUint8              * obexPacket,
                                                CsrUint16                obexPacketLength)
{
    SynccInstanceData * pInst       = instData;
    CsrBtSynccDeleteObjectCfm *pMsg = (CsrBtSynccDeleteObjectCfm *) CsrPmemAlloc(sizeof(CsrBtSynccDeleteObjectCfm));
    pMsg->type                      = CSR_BT_SYNCC_DELETE_OBJECT_CFM;
    pMsg->responseCode              = responseCode;

    csrBtSynccExtractApplicationResponse(obexPacket, obexPacketLength, &pMsg->luid, &pMsg->changeCounter, &pMsg->timeStamp);

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSynccAbortResultHandler(void  *    instData,
                                         CsrUint8    responseCode,
                                         CsrUint8 * obexPacket,
                                         CsrUint16   obexPacketLength)
{
    SynccInstanceData * pInst     = instData;
    CsrBtSynccAbortCfm *pMsg           = (CsrBtSynccAbortCfm*) CsrPmemAlloc(sizeof(CsrBtSynccAbortCfm));
    pMsg->type                  = CSR_BT_SYNCC_ABORT_CFM;
    csrBtSynccMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSynccSyncCommandResultHandler(void  *    instData,
                                               CsrUint32   lengthOfObject,
                                               CsrUint16   ucs2nameOffset,
                                               CsrUint16   bodyOffset,
                                               CsrUint16   bodyLength,
                                               CsrUint8 *payload,
                                               CsrUint16   payloadLength)
{
    SynccInstanceData * pInst = instData;
    CsrBtSynccSyncCommandInd *pMsg = pnew(CsrBtSynccSyncCommandInd);

    pMsg->type              = CSR_BT_SYNCC_SYNC_COMMAND_IND;
    pMsg->finalFlag         = pInst->putFinalFlag;
    pMsg->lengthOfObject    = lengthOfObject;
    pMsg->ucs2nameOffset    = ucs2nameOffset;
    pMsg->bodyOffset        = bodyOffset;
    pMsg->bodyLength        = bodyLength;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;

    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}


static void csrBtSynccSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
                                        CsrBtSupplier resultSupplier)
{
    CsrBtSynccSecurityInCfm *pMsg    = (CsrBtSynccSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtSynccSecurityInCfm));
    pMsg->type                  = CSR_BT_SYNCC_SECURITY_IN_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    csrBtSynccMessagePut(appHandle, pMsg);
}

static void csrBtSynccSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier)
{
    CsrBtSynccSecurityOutCfm *pMsg   = (CsrBtSynccSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtSynccSecurityOutCfm));
    pMsg->type                  = CSR_BT_SYNCC_SECURITY_OUT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    csrBtSynccMessagePut(appHandle, pMsg);
}

CsrUint8 CsrBtSynccConnectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrUint16  index;
    CsrUint8 tempTarget[CSR_BT_OBEX_SYNCC_TARGET_LENGTH] = CSR_BT_OBEX_SYNCC_TARGET;
    CsrUint8   numOfHeaders                  = 0;
    CsrUint8   headerPriorityTable[2]        = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
    CsrBtSynccConnectReq *pMsg               = (CsrBtSynccConnectReq *) msg;
    CmnCsrBtLinkedListStruct * sdpTag    = NULL;

    ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(pMsg->authorize,
                                                                               &pMsg->password, 
                                                                               pMsg->passwordLength,
                                                                               &pMsg->userId,
                                                                               &pMsg->realm, 
                                                                               pMsg->realmLength);

    CsrUint8 *target                     = (CsrUint8 *) CsrPmemAlloc(CSR_BT_OBEX_SYNCC_TARGET_LENGTH);
    pInst->appHandle = pMsg->appHandle;
    pInst->maxFrameSize = pMsg->maxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    CsrMemCpy(target, tempTarget, CSR_BT_OBEX_SYNCC_TARGET_LENGTH);

    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID, &index);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_DATA_STORES_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
    CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

    if (pMsg->length > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    if (pMsg->count > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    return(ObexUtilConnectRequest(pInst->obexClientInst,
                                  &sdpTag,
                                  pMsg->deviceAddr,
                                  pInst->secOutgoing,
                                  pMsg->maxPacketSize,
                                  CSR_BT_SYNCC_PROFILE_DEFAULT_MTU_SIZE,
                                  &target,
                                  CSR_BT_OBEX_SYNCC_TARGET_LENGTH,
                                  numOfHeaders,
                                  headerPriorityTable,
                                  CSR_BT_SYNCC_LP_SUPERVISION_TIMEOUT,
                                  &chal,
                                  pMsg->length,
                                  pMsg->count,
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtSynccConnectResultHandler,
                                  csrBtSynccAuthenticateIndHandler,
                                  csrBtSynccDisconnectResultHandler,
                                  NULL,
                                  pInst->windowSize,
                                  pInst->localSrmEnable));
}

CsrUint8 CsrBtSynccCancelConnectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexClientInst, FALSE, NULL));
}

CsrUint8 CsrBtSynccDisconnectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccDisconnectReq *pMsg = (CsrBtSynccDisconnectReq *) msg;

    return (ObexUtilDisconnectRequest(pInst->obexClientInst, pMsg->normalObexDisconnect, NULL));
}

CsrUint8 CsrBtSynccAuthenticateResHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccAuthenticateRes *pMsg = (CsrBtSynccAuthenticateRes *) msg;
    CsrUint8 userIdLength  = 0;

    if(pMsg->userId)
    {
        userIdLength  = (CsrUint8) CsrStrLen((const char *)pMsg->userId);
    }

    if (userIdLength > CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH)
    {
        pMsg->userId[CSR_BT_OBEX_MAX_AUTH_USERID_LENGTH] = '\0';
    }

    return(ObexUtilCliAuthenticateResponse(pInst->obexClientInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId, NULL));
}

CsrUint8 CsrBtSynccGetObjectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtSynccGetObjectReq *pMsg         = (CsrBtSynccGetObjectReq *) msg;

    return(ObexUtilGetRequest(pInst->obexClientInst,
                              headerPriorityTable,
                              NULL,
                              &pMsg->ucs2name,
                              NULL,
                              0,
                              NULL,
                              0,
                              NULL,
                              CSR_BT_OBEX_SRMP_INVALID,
                              csrBtSynccGetObjectResultHandler,
                              NULL));
}

CsrUint8 CsrBtSynccGetObjectResHandler(SynccInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilGetContinueRequest(pInst->obexClientInst, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtSynccAddObjectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtSynccAddObjectReq *pMsg         = (CsrBtSynccAddObjectReq *) msg;

    if (pMsg->lengthOfObject == 0)
    {
        headerPriorityTable[1]  = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }
    return(ObexUtilPutRequest(pInst->obexClientInst, headerPriorityTable, pMsg->lengthOfObject, NULL, &pMsg->objectName, NULL, 0, NULL, 0, NULL, csrBtSynccAddObjectContinueHandler, csrBtSynccAddObjectResultHandler));
}

CsrUint8 CsrBtSynccAddObjectResHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccAddObjectRes *pMsg         = (CsrBtSynccAddObjectRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexClientInst, pMsg->finalFlag, &pMsg->payload, pMsg->payloadLength));
}

CsrUint8 CsrBtSynccModifyObjectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6] = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtSynccModifyObjectReq *pMsg = (CsrBtSynccModifyObjectReq *) msg;
    CsrUint8 *appParametersHeader   = NULL;
    CsrUint16 appParametersHeaderLength = 0;

    if(pMsg->maxExpectedChangeCounter)
    {
        CsrUint32 maxChangeCounterLength = CsrStrLen((char *) pMsg->maxExpectedChangeCounter);

        if(maxChangeCounterLength > 0xFF)
        {
            maxChangeCounterLength = 0xFF;
        }
        appParametersHeader = CsrPmemAlloc(3 + 2 + maxChangeCounterLength);
        appParametersHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
        CSR_BT_OBEX_SET_WORD_AT(appParametersHeader, 1, 3 + 2 + maxChangeCounterLength);
        appParametersHeaderLength = 3;
        appParametersHeader[appParametersHeaderLength++] = 0x11;
        appParametersHeader[appParametersHeaderLength++] = (CsrUint8) maxChangeCounterLength;
        CsrMemCpy((char *)&appParametersHeader[appParametersHeaderLength], pMsg->maxExpectedChangeCounter, maxChangeCounterLength);
        appParametersHeaderLength += (CsrUint16) maxChangeCounterLength;

        CsrPmemFree(pMsg->maxExpectedChangeCounter);
    }
    else
    {
        headerPriorityTable[2] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }

    if (pMsg->lengthOfObject == 0)
    {
        headerPriorityTable[1] = headerPriorityTable[2];
        headerPriorityTable[2] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }
    return(ObexUtilPutRequest(pInst->obexClientInst, headerPriorityTable, pMsg->lengthOfObject, NULL, &pMsg->objectName, NULL, appParametersHeaderLength, &appParametersHeader, 0, NULL, csrBtSynccModifyObjectContinueHandler, csrBtSynccModifyObjectResultHandler));
}

CsrUint8 CsrBtSynccModifyObjectResHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccModifyObjectRes *pMsg      = (CsrBtSynccModifyObjectRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexClientInst, pMsg->finalFlag, &pMsg->payload, pMsg->payloadLength));
}

CsrUint8 CsrBtSynccDeleteObjectReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtSynccDeleteObjectReq *pMsg      = (CsrBtSynccDeleteObjectReq *) msg;
    CsrUint8 *appParametersHeader = NULL;
    CsrUint16 appParametersHeaderLength = 0;

    if(pMsg->maxExpectedChangeCounter)
    {
        CsrUint32 maxChangeCounterLength = CsrStrLen((char *) pMsg->maxExpectedChangeCounter);

        if(maxChangeCounterLength > 0xFF)
        {
            maxChangeCounterLength = 0xFF;
        }
        appParametersHeader = CsrPmemAlloc(3 + 2 + maxChangeCounterLength + ((CsrUint8)(pMsg->hardDelete ? 2 : 0)));
        appParametersHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
        CSR_BT_OBEX_SET_WORD_AT(appParametersHeader, 1, 3 + 2 + maxChangeCounterLength + ((CsrUint8)(pMsg->hardDelete ? 2 : 0)));
        appParametersHeaderLength = 3;
        appParametersHeader[appParametersHeaderLength++] = 0x11;
        appParametersHeader[appParametersHeaderLength++] = (CsrUint8) maxChangeCounterLength;
        CsrMemCpy((char *)&appParametersHeader[appParametersHeaderLength], pMsg->maxExpectedChangeCounter, maxChangeCounterLength);
        appParametersHeaderLength += (CsrUint16) maxChangeCounterLength;

        CsrPmemFree(pMsg->maxExpectedChangeCounter);
    }

    if(pMsg->hardDelete)
    {
        if(appParametersHeader == NULL)
        {
            appParametersHeader = CsrPmemAlloc(3 + 2);
            appParametersHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
            CSR_BT_OBEX_SET_WORD_AT(appParametersHeader, 1, 3 + 2);
            appParametersHeaderLength = 3;
        }

        appParametersHeader[appParametersHeaderLength++] = 0x12;
        appParametersHeader[appParametersHeaderLength++] = 0;
    }
    return(ObexUtilPutRequest(pInst->obexClientInst, headerPriorityTable, 0, NULL, &pMsg->objectName, NULL, appParametersHeaderLength, &appParametersHeader, 0, NULL, NULL, csrBtSynccDeleteObjectResultHandler));
}

CsrUint8 CsrBtSynccAbortReqHandler(SynccInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);

    return(ObexUtilAbortRequest(pInst->obexClientInst, NULL, csrBtSynccAbortResultHandler));
}

/* OBEX Server part of SYNC Client */
#define SYNCC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX    24
static const CsrUint8 synccObexSyncCommandServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x07,        /* 2 byte UUID, Service class = IrMCSyncCommand */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x11,             /* 17 bytes in total DataElSeq */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:27) */

    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,      /* 2 byte UUID, Protocol = OBEX */

    /* BrowseGroupList */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

    /* profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, ProfileDescriptorList */
    0x35,0x08,
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x04,        /* 2 byte UUID, Service class = IrMCSync */
    0x09,0x01,0x00,        /* 2 byte uint, version = 100 */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x14,             /* 20 bytes string */
    'S','y','n','c',' ','C','o','m','m','a','n','d',' ','S','e','r','v','i','c','e'
};

void csrBtObexSynccServerStart(SynccInstanceData *pInst)
{
    CsrUint16 recordLength    = (CsrUint16) (sizeof(synccObexSyncCommandServiceRecord));
    CsrUint8  *record         = (CsrUint8 *)CsrPmemAlloc(recordLength);

    CsrMemCpy(record, synccObexSyncCommandServiceRecord, recordLength);
    if (!pInst->obexServerInst)
    {
        pInst->obexServerInst = ObexUtilInit(CSR_BT_SYNCC_IFACEQUEUE, pInst, CSR_BT_SYNCC_SERVER_INST_ID);
    }
    ObexUtilServActivateStart(pInst->obexServerInst,
                              pInst->secIncoming,
                              CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID,
                              CSR_BT_SYNCC_LP_SUPERVISION_TIMEOUT,
                              pInst->serverChannel,
                              L2CA_PSM_INVALID,
                              recordLength,
                              SYNCC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              &record,
                              CSR_BT_OBJECT_TRANSFER_COD,
                              csrBtSynccServerConnectIndHandler,
                              csrBtSynccServerPutIndHandler,
                              NULL,
                              NULL,
                              csrBtSynccServerAbortIndHandler,
                              csrBtSynccServerDisconnectIndHandler,
                              NULL,
                              csrBtSynccServerActivateCfmHandler,
                              NULL,
                              pInst->maxFrameSize,
                              pInst->windowSize,
                              pInst->localSrmEnable);
}

CsrUint8 CsrBtSynccActivateSyncCommandReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccActivateSyncCommandReq *pMsg = msg;

    /* Only for the OBEX server begin */
    pInst->appHandle = pMsg->appHandle;
    pInst->maxFrameSize = pMsg->maxPacketSize;

    csrBtObexSynccServerStart(pInst);

    /* Only for the OBEX server end*/
    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

void CsrBtObexSynccDeactivateCfmHandler(void            *instData,
                                        CsrUint8   serverChannel,
                                        psm_t           psm)
{
    SynccInstanceData * pInst = (SynccInstanceData *) instData;
    CsrBtSynccDeactivateSyncCommandCfm *pCfm  = pnew(CsrBtSynccDeactivateSyncCommandCfm);

    pInst->activated = FALSE;
    pInst->serverChannel = serverChannel;
    ObexUtilDeinit(&pInst->obexServerInst);
    pInst->obexServerInst = NULL;
    pCfm->type = CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM;
    pCfm->resultCode = CSR_BT_RESULT_CODE_OBEX_SUCCESS;
    pCfm->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;
    csrBtSynccMessagePut(pInst->appHandle, pCfm);
    CSR_UNUSED(psm);
}

CsrUint8 CsrBtSynccDeactivateSyncCommandReqHandler(SynccInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);

    if (pInst->activated)
    {
        ObexUtilServDeactivateStart(pInst->obexServerInst,
                                    pInst->serverChannel,
                                    L2CA_PSM_INVALID,
                                    CsrBtObexSynccDeactivateCfmHandler);
    }
    else
    {
        CsrBtObexSynccDeactivateCfmHandler(pInst, pInst->serverChannel, L2CA_PSM_INVALID);
    }

    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

CsrUint8 CsrBtSynccSyncCommandResHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtObexResponseCode responseCode;
    CsrBtSynccSyncCommandRes *pMsg = msg;

    if (pInst->putFinalFlag)
    {
        if (pMsg->responseCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && pMsg->responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            responseCode = pMsg->responseCode;
        }
        else
        {
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
    }
    else
    {
        if (pMsg->responseCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && pMsg->responseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            responseCode = pMsg->responseCode;
        }
        else
        {
            responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
    }

    ObexUtilServPutResponse(pInst->obexServerInst, responseCode, CSR_BT_OBEX_SRMP_INVALID);

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtSynccSecurityInReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccSecurityInReq *pMsg    = (CsrBtSynccSecurityInReq*)msg;

    csrBtSynccSecurityInCfmSend(pMsg->appHandle,
                                CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                                                     CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_INCOMING,
                                                     CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING,
                                                     CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                     CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER),
                                CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexServerInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtSynccSecurityOutReqHandler(SynccInstanceData *pInst, void *msg)
{
    CsrBtSynccSecurityOutReq *pMsg = (CsrBtSynccSecurityOutReq*)msg;

    csrBtSynccSecurityOutCfmSend(pMsg->appHandle,
                                 CsrBtScSetSecOutLevel(&pInst->secOutgoing, pMsg->secLevel,
                                                       CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_OUTGOING,
                                                       CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_OUTGOING,
                                                       CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                       CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER),
                                 CSR_BT_SUPPLIER_OBEX_PROFILES);

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}


static void csrBtObexSynccServerConnectable(SynccInstanceData *pInst)
{
    CsrBtSynccActivateSyncCommandCfm *pCfm  = pnew(CsrBtSynccActivateSyncCommandCfm);

    ObexUtilServConnectableStart(pInst->obexServerInst,
                                 CSR_BT_SYNCC_PROFILE_DEFAULT_MTU_SIZE,
                                 pInst->maxFrameSize);

    pCfm->type = CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM;
    pCfm->resultCode = CSR_BT_RESULT_CODE_OBEX_SUCCESS;
    pCfm->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;

    csrBtSynccMessagePut(pInst->appHandle, pCfm);
}

static void csrBtSynccServerActivateCfmHandler(void            *instData,
                                               CsrUint8   serverChannel,
                                               psm_t           psm,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier   resultSupplier)
{
    SynccInstanceData * pInst = (SynccInstanceData *) instData;
    pInst->serverChannel      = serverChannel;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The SYNC server is activated */
        pInst->activated = TRUE;
        csrBtObexSynccServerConnectable(pInst);
    }
    else
    { /* The FTP server cannot send give this information to the application
         as it has no API for it. Only thing to do is to activate it again */
        csrBtObexSynccServerStart(pInst);
    }
    CSR_UNUSED(psm);
}


static CsrBool csrBtSynccCheckTargetHeader(CsrUint8   *target,
                                           CsrUint16  targetHeaderLength)
{
    if (target && (targetHeaderLength == CSR_BT_OBEX_SYNCC_TARGET_LENGTH))
    {
        CsrUint8 tempTarget[CSR_BT_OBEX_SYNCC_TARGET_LENGTH] = CSR_BT_OBEX_SYNCC_TARGET;

        if (!CsrMemCmp(target, tempTarget, CSR_BT_OBEX_SYNCC_TARGET_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);
}

static CsrUint8 csrBtSynccServerConnectResponseSend(SynccInstanceData           *pInst,
                                                    CsrBtObexResponseCode        responseCode,
                                                    CsrUcs2String                *description)
{
    CsrUint8  numOfHeaders           = 0;
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint16 whoHeaderLength        = CSR_BT_OBEX_SYNCC_TARGET_LENGTH;
    CsrUint8  *who                   = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    CsrUint8  tempTarget[CSR_BT_OBEX_SYNCC_TARGET_LENGTH] = CSR_BT_OBEX_SYNCC_TARGET;
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    if (description)
    {
        numOfHeaders = 1;
    }
#else
    CSR_UNUSED(description);
#endif
    CsrMemCpy(who, tempTarget, whoHeaderLength);

    return (ObexUtilServConnectResponse(pInst->obexServerInst,
                                        responseCode,
                                        whoHeaderLength,
                                        &who,
                                        (CsrUint32) pInst->serverChannel,
                                        numOfHeaders,
                                        headerPriorityTable,
                                        NULL,
                                        &description,
                                        0,
                                        NULL,
                                        FALSE));
}

static void csrBtSynccServerConnectIndHandler( void                    *instData,
                                               CsrBtDeviceAddr         deviceAddr,
                                               CsrBtConnId             cid,
                                               CsrUint16               maxPeerObexPacketLength,
                                               CsrUint16               obexResponsePacketLength,
                                               CsrUint32               length,
                                               CsrUint32               count,
                                               CsrUint16               targetHeaderLength,
                                               CsrUint8                *target,
                                               CsrBool                 authenticated,
                                               CsrBool                 challenged,
                                               CsrUint16               obexPacketLength,
                                               CsrUint8                *obexPacket)
{
    SynccInstanceData *pInst = (SynccInstanceData *)instData;

    if (csrBtSynccCheckTargetHeader(target, targetHeaderLength))
    {
        csrBtSynccServerConnectResponseSend(pInst,
                                            CSR_BT_OBEX_SUCCESS_RESPONSE_CODE,
                                            NULL);
        pInst->isServerObexConnected = TRUE;
#ifdef CSR_AMP_ENABLE
        /* Tell the AMPM that the slave channel is connected */
        pInst->slaveBtConnId = cid;
        CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 1, &pInst->slaveBtConnId);
#endif /* CSR_AMP_ENABLE */
    }
    else
    {
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target missing");
#endif
        csrBtSynccServerConnectResponseSend(pInst,
                                            CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                            description);
    }
    CSR_UNUSED(count);
    CSR_UNUSED(length);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}


static void csrBtSynccServerPutNextIndHandler(void          *instData,
                                              CsrUint16     obexResponsePacketLength,
                                              CsrBool       authenticated,
                                              CsrBool       finalBitSet,
                                              CsrUint16     bodyLength,
                                              CsrUint16     bodyOffset,
                                              CsrUint16     obexPacketLength,
                                              CsrUint8    *obexPacket)
{ /* The next Put Request packet is received */
    SynccInstanceData   *pInst = (SynccInstanceData *)instData;

    pInst->putFinalFlag = finalBitSet;

    csrBtSynccSyncCommandResultHandler(pInst,
                                       0,
                                       0,
                                       bodyOffset,
                                       bodyLength,
                                       obexPacket,
                                       obexPacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtSynccServerPutIndHandler(void          *instData,
                                          CsrUint16     obexResponsePacketLength,
                                          CsrBool       authenticated,
                                          CsrBool       finalBitSet,
                                          CsrUint16     bodyLength,
                                          CsrUint16     bodyOffset,
                                          CsrUint16     obexPacketLength,
                                          CsrUint8    *obexPacket)
{ /* The first Put Request packet is received */
    SynccInstanceData   *pInst = (SynccInstanceData *)instData;
    CsrUint16           nameOffset;
    CsrUint32           totalObjectSize;

    CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, obexPacket, &nameOffset);
    CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &totalObjectSize);

    pInst->putFinalFlag = finalBitSet;

    csrBtSynccSyncCommandResultHandler(pInst,
                                       totalObjectSize,
                                       nameOffset,
                                       bodyOffset,
                                       bodyLength,
                                       obexPacket,
                                       obexPacketLength);

    if (!finalBitSet && nameOffset && totalObjectSize)
    { /* More OBEX PUT request packets to come and all headers found */
        ObexUtilServSetPutContinueFunc(pInst->obexServerInst, csrBtSynccServerPutNextIndHandler);
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtSynccServerAbortIndHandler(void                    *instData,
                                            CsrUint16               descriptionOffset,
                                            CsrUint16               descriptionLength,
                                            CsrUint16               obexPacketLength,
                                            CsrUint8              *obexPacket)
{
    SynccInstanceData             *pInst = (SynccInstanceData *)instData;
    CsrBtSynccSyncCommandAbortInd *pMsg;

    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;
    }
    pMsg = pnew(CsrBtSynccSyncCommandAbortInd);
    pMsg->type = CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND;
    pMsg->descriptionOffset   = descriptionOffset;
    pMsg->descriptionLength   = descriptionLength;
    pMsg->payload             = obexPacket;
    pMsg->payloadLength       = obexPacketLength;
    csrBtSynccMessagePut(pInst->appHandle, pMsg);
}


static void csrBtSynccServerDisconnectIndHandler(void            *instData,
                                                 CsrBtDeviceAddr deviceAddr,
                                                 CsrBtReasonCode reasonCode,
                                                 CsrBtSupplier   reasonSupplier,
                                                 CsrUint16       obexPacketLength,
                                                 CsrUint8        *obexPacket)
{
    SynccInstanceData *pInst = (SynccInstanceData *)instData;

    pInst->isServerObexConnected = FALSE;
#ifdef CSR_AMP_ENABLE
    /* Tell the AMPM that the slave channel is connected */
    pInst->slaveBtConnId = 0;
    CsrBtAmpmPrfMultiConnReqSend(pInst->masterBtConnId, 0, NULL);
#endif /* CSR_AMP_ENABLE */
    CSR_UNUSED(instData);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CsrPmemFree(obexPacket);
}


