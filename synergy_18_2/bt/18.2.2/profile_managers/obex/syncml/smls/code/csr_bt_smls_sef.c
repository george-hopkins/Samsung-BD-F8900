/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_smls_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_smls_sef.h"

static const CsrUint8 CSR_BT_SMLS_SYNC_TARGET_UUID[]    = {"SYNCML-SYNC"};
static const CsrUint8 CSR_BT_SMLS_SYNC_DM_TARGET_UUID[] = {"SYNCML-DM"};
#define CSR_BT_SMLS_SYNC_TARGET_UUID_LEN        0x0B
#define CSR_BT_SMLS_SYNC_DM_TARGET_UUID_LEN     0x09
#define CSR_BT_MIME_HEAD_DATASYNC_XML           "application/vnd.syncml+xml"
#define CSR_BT_MIME_HEAD_DATASYNC_WBXML         "application/vnd.syncml+wbxml"
#define CSR_BT_MIME_HEAD_DEVMAN_XML             "application/vnd.syncml.dm+xml"
#define CSR_BT_MIME_HEAD_DEVMAN_WBXML           "application/vnd.syncml.dm+wbxml"
#define CSR_BT_SMLS_GET_BODY_HEADER_INDEX                       0x01
#define CSR_BT_SMLS_HEADER_SIZE                                 0x10
#define CSR_BT_SMLS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX  0x26

static const CsrUint8 smlsServiceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,     /* AttrID, ServiceClassIDList (2 bytes) */
    0x35,0x11,          /* ATT 17 bytes in total DataElSeq */
                        /* First byte codes for 16 byte UUID, and then SyncMLServer UUID follows completely - same as SMLS_SYNCMLSERVER_UUID_128 */
    0x1C,0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00 , 0x80, 0x00, 0x00, 0x02, 0xEE, 0x00, 0x00, 0x02,

    /* protocol descriptor list */
    0x09,0x00,0x04,     /* AttrId, ProtocolDescriptorList (2 bytes)*/
    0x35,0x11,          /* 17  bytes in total DataElSeq */

    0x35,0x03,          /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,     /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,          /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,     /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,          /* 1 byte UINT - server channel template value 0 - to be filled in by app  */

    0x35,0x03,          /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,   /* 2 byte UUID, Protocol = OBEX */

    /* service name */
    0x09,0x01,0x00,     /* AttrId, Service Name (2 bytes) */
    0x25,0x0C,          /* 12 byte string */
                        /*same as smlsServiceName.*/
    'S','y','n','c','M','L','S','e','r','v','e','r',

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static void csrBtSmlsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SMLS_PRIM, msg);
}

static void csrBtSmlsConnectIndSend(CsrSchedQid          appHandle, 
                                    CsrUint32       connectionId,
                                    CsrUint16       maxPeerObexPacketLength,
                                    CsrBtDeviceAddr deviceAddr, 
                                    CsrUint8        targetService,
                                    CsrUint32       count,
                                    CsrUint32       length,
                                    CsrBtConnId     btConnId)
{
    CsrBtSmlsConnectInd *pMsg = (CsrBtSmlsConnectInd *)
                                 CsrPmemAlloc(sizeof(CsrBtSmlsConnectInd));

    pMsg->type                  = CSR_BT_SMLS_CONNECT_IND;
    pMsg->connectionId          = connectionId;
    pMsg->obexPeerMaxPacketSize = (CsrUint16)(maxPeerObexPacketLength - CSR_BT_SMLS_HEADER_SIZE);
    pMsg->deviceAddr            = deviceAddr;
    pMsg->targetService         = targetService;
    pMsg->count                 = count;
    pMsg->length                = length;
    pMsg->btConnId              = btConnId;

    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsAuthCfmSend(CsrSchedQid appHandle)
{   
    CsrBtSmlsAuthenticateCfm *pMsg = (CsrBtSmlsAuthenticateCfm *)
                                      CsrPmemAlloc(sizeof(CsrBtSmlsAuthenticateCfm));
    
    pMsg->type = CSR_BT_SMLS_AUTHENTICATE_CFM;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsSecurityInCfmSend(CsrSchedQid          appHandle, 
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier   resultSupplier)
{
    CsrBtSmlsSecurityInCfm *pMsg = (CsrBtSmlsSecurityInCfm*)
                                    CsrPmemAlloc(sizeof(CsrBtSmlsSecurityInCfm));

    pMsg->type                   = CSR_BT_SMLS_SECURITY_IN_CFM;
    pMsg->resultCode             = resultCode;
    pMsg->resultSupplier         = resultSupplier;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsActivateCfmSend(CsrSchedQid          appHandle,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier   resultSupplier)
{
    CsrBtSmlsActivateCfm *pMsg  = (CsrBtSmlsActivateCfm *) 
                                   CsrPmemAlloc(sizeof(CsrBtSmlsActivateCfm));

    pMsg->type                  = CSR_BT_SMLS_ACTIVATE_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsAbortIndSend(CsrSchedQid     appHandle,
                                  CsrUint32  connectionId,
                                  CsrUint16  descriptionOffset,
                                  CsrUint16  descriptionLength,
                                  CsrUint8 *payload,
                                  CsrUint16  payloadLength)
{
    CsrBtSmlsAbortInd *pMsg  = (CsrBtSmlsAbortInd *) 
                                CsrPmemAlloc(sizeof(CsrBtSmlsAbortInd));

    pMsg->type               = CSR_BT_SMLS_ABORT_IND;
    pMsg->connectionId       = connectionId;
    pMsg->descriptionOffset  = descriptionOffset;
    pMsg->descriptionLength  = descriptionLength;
    pMsg->payload            = payload;
    pMsg->payloadLength      = payloadLength;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsDisconnectIndSend(CsrSchedQid           appHandle, 
                                       CsrUint32        connectionId,
                                       CsrBtDeviceAddr  deviceAddr,
                                       CsrBtReasonCode  reasonCode,
                                       CsrBtSupplier    reasonSupplier)
{
    CsrBtSmlsDisconnectInd *pMsg = (CsrBtSmlsDisconnectInd *)
                                    CsrPmemAlloc(sizeof(CsrBtSmlsDisconnectInd));

    pMsg->type              = CSR_BT_SMLS_DISCONNECT_IND;
    pMsg->connectionId      = connectionId;
    pMsg->deviceAddr        = deviceAddr;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsPutSmlMsgObjIndSend(CsrSchedQid     appHandle, 
                                         CsrUint32  connectionId, 
                                         CsrUint8   isFinal,
                                         CsrUint32  lenOfObj, 
                                         CsrUint8   mimeType, 
                                         CsrUint16  bodyLength,
                                         CsrUint16  bodyOffset, 
                                         CsrUint8 *payload, 
                                         CsrUint16  payloadLength)
{
    CsrBtSmlsPutSmlMsgObjInd *pMsg = (CsrBtSmlsPutSmlMsgObjInd *) 
                                      CsrPmemAlloc(sizeof(CsrBtSmlsPutSmlMsgObjInd));

    pMsg->type              = CSR_BT_SMLS_PUT_SML_MSG_OBJ_IND;
    pMsg->connectionId      = connectionId;
    pMsg->finalFlag         = isFinal;
    pMsg->lengthOfObject    = lenOfObj;
    pMsg->mimeType          = mimeType;
    pMsg->bodyLength        = bodyLength;
    pMsg->bodyOffset        = bodyOffset;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsPutSmlMsgObjNextIndSend(CsrSchedQid     appHandle, 
                                             CsrUint32  connectionId,
                                             CsrUint8   isFinal, 
                                             CsrUint8   mimeType, 
                                             CsrUint16  bodyLength,
                                             CsrUint16  bodyOffset, 
                                             CsrUint8 *payload, 
                                             CsrUint16  payloadLength)
{
    CsrBtSmlsPutSmlMsgObjNextInd *pMsg = (CsrBtSmlsPutSmlMsgObjNextInd *)
                                          CsrPmemAlloc(sizeof(CsrBtSmlsPutSmlMsgObjNextInd));

    pMsg->type          = CSR_BT_SMLS_PUT_SML_MSG_OBJ_NEXT_IND;
    pMsg->connectionId  = connectionId;
    pMsg->finalFlag     = isFinal;
    pMsg->mimeType      = mimeType,
    pMsg->bodyLength    = bodyLength;
    pMsg->bodyOffset    = bodyOffset;
    pMsg->payload       = payload;
    pMsg->payloadLength = payloadLength;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsGetSmlMsgObjNextIndSend(CsrSchedQid     appHandle, 
                                             CsrUint32  connectionId,
                                             CsrUint8   mimeType)
{
    CsrBtSmlsGetSmlMsgObjNextInd *pMsg = (CsrBtSmlsGetSmlMsgObjNextInd *)
                                          CsrPmemAlloc(sizeof(CsrBtSmlsGetSmlMsgObjNextInd));

    pMsg->type          = CSR_BT_SMLS_GET_SML_MSG_OBJ_NEXT_IND;
    pMsg->connectionId  = connectionId;
    pMsg->mimeType      = mimeType;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static void csrBtSmlsGetSmlMsgObjIndSend(CsrSchedQid     appHandle, 
                                         CsrUint32  connectionId, 
                                         CsrUint8   mimeType)
{
    CsrBtSmlsGetSmlMsgObjInd *pMsg = (CsrBtSmlsGetSmlMsgObjInd *)
                                      CsrPmemAlloc(sizeof(CsrBtSmlsGetSmlMsgObjInd));

    pMsg->type          = CSR_BT_SMLS_GET_SML_MSG_OBJ_IND;
    pMsg->connectionId  = connectionId;
    pMsg->mimeType      = mimeType;
    csrBtSmlsMessagePut(appHandle, pMsg);
}

static CsrUint8 csrBtSmlsConnectResponseHandler(SmlsInstanceDataType        *pInst,
                                                CsrBtObexResponseCode       responseCode, 
                                                ObexUtilDigestChallengeType *digestChallenge,
                                                CsrUcs2String               *description,
                                                CsrUint8                    targetService)
{
    CsrUint8  *who                    = NULL;
    CsrUint16 whoHeaderLength         = 0;
    CsrUint8  numOfHeaders            = 0;  
    CsrUint8  headerPriorityTable[1]  = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    
    if(CSR_BT_SMLS_SYNCMLSYNC_SERVICE == targetService)
    {
        whoHeaderLength = CSR_BT_SMLS_SYNC_TARGET_UUID_LEN;
        who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
        CsrMemCpy(who, CSR_BT_SMLS_SYNC_TARGET_UUID, whoHeaderLength);
    }
    else if(CSR_BT_SMLS_SYNCMLDM_SERVICE == targetService)
    {
        whoHeaderLength = CSR_BT_SMLS_SYNC_DM_TARGET_UUID_LEN;
        who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
        CsrMemCpy(who, CSR_BT_SMLS_SYNC_DM_TARGET_UUID, whoHeaderLength);
    }
    else
    {
        if ((responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        { /* The application think it is connected                              */
            csrBtSmlsDisconnectIndSend(pInst->appHandle, 
                                       (CsrUint32) pInst->localServerCh,
                                       pInst->deviceAddr,
                                       CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER,
                                       CSR_BT_SUPPLIER_OBEX_PROFILES);
        }
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
    }
    
    responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    if (description)
    {
        numOfHeaders = 1;               
    }
    return (ObexUtilServConnectResponse(pInst->obexInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        (CsrUint32) pInst->localServerCh, 
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        &digestChallenge, 
                                        &description, 
                                        0, 
                                        NULL,
                                        FALSE));
}

static CsrUint8 csrBtSmlsGetWithHeadersResponseHandler(SmlsInstanceDataType        *pInst,
                                                       CsrUint8                    firstHeader,
                                                       CsrUint8                    secondHeader,
                                                       CsrUint8                    bodyHeaderPriorityIndex,
                                                       ObexUtilDigestChallengeType *digestChallenge,
                                                       CsrBtObexResponseCode       responseCode, 
                                                       CsrUint32                   lengthOfObject,
                                                       CsrUint16                   bodyLength,
                                                       CsrUint8                    *body)
{
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

    if (lengthOfObject > 0)
    {
        headerPriorityTable[0] = firstHeader;
        headerPriorityTable[1] = secondHeader;
    }
    else
    {
        headerPriorityTable[0] = secondHeader;
    }
    return (ObexUtilServGetWithHeaderResponse(pInst->obexInst, 
                                              responseCode,
                                              &digestChallenge, 
                                              headerPriorityTable, 
                                              bodyHeaderPriorityIndex, 
                                              lengthOfObject,
                                              NULL, 
                                              NULL, 
                                              0, 
                                              NULL, 
                                              0, 
                                              NULL, 
                                              &body, 
                                              bodyLength,
                                              CSR_BT_OBEX_SRMP_INVALID)); 
}

static CsrUint8 csrBtSmlsCheckTargetHeader(CsrUint8  *target, CsrUint16 targetHeaderLength)
{
    if (target && targetHeaderLength == CSR_BT_SMLS_SYNC_TARGET_UUID_LEN &&
        (!CsrMemCmp(target, CSR_BT_SMLS_SYNC_TARGET_UUID, CSR_BT_SMLS_SYNC_TARGET_UUID_LEN)))
    {
        return (CSR_BT_SMLS_SYNCMLSYNC_SERVICE);
    }
    else if (target && targetHeaderLength == CSR_BT_SMLS_SYNC_DM_TARGET_UUID_LEN &&
             (!CsrMemCmp(target, CSR_BT_SMLS_SYNC_DM_TARGET_UUID, CSR_BT_SMLS_SYNC_DM_TARGET_UUID_LEN)))
    {
        return (CSR_BT_SMLS_SYNCMLDM_SERVICE);
    }
    return (CSR_BT_SMLS_SYNCML_SERVICE_UNKNOWN_TO_BIG);
}

static CsrUint8 csrBtSmlsReturnMimeTypeFromHeader(CsrUint8 *obexPacket, CsrUint16  obexPacketLength)
{
    CsrUint16   typeOffset;
    CsrUint16   typeLength;
    
    if (CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { /* A Type header is found                                                 */
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_MIME_HEAD_DATASYNC_XML)) &&
            (!CsrBtStrICmpLength((char *)&obexPacket[typeOffset], (char *)CSR_BT_MIME_HEAD_DATASYNC_XML, sizeof(CSR_BT_MIME_HEAD_DATASYNC_XML))))
        {
            return CSR_BT_SMLS_MIME_DATASYNC_XML;
        }
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_MIME_HEAD_DATASYNC_WBXML)) &&
            (!CsrBtStrICmpLength((char *)&obexPacket[typeOffset], (char *)CSR_BT_MIME_HEAD_DATASYNC_WBXML, sizeof(CSR_BT_MIME_HEAD_DATASYNC_WBXML))))
        {
            return CSR_BT_SMLS_MIME_DATASYNC_WBXML;
        }
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_MIME_HEAD_DEVMAN_XML)) &&
            (!CsrBtStrICmpLength((char *)&obexPacket[typeOffset], (char *)CSR_BT_MIME_HEAD_DEVMAN_XML, sizeof(CSR_BT_MIME_HEAD_DEVMAN_XML))))
        {
            return CSR_BT_SMLS_MIME_DEVMAN_XML;
        }    
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_MIME_HEAD_DEVMAN_WBXML)) &&
            (!CsrBtStrICmpLength((char *)&obexPacket[typeOffset], (char *)CSR_BT_MIME_HEAD_DEVMAN_WBXML, sizeof(CSR_BT_MIME_HEAD_DEVMAN_WBXML))))
        {
            return CSR_BT_SMLS_MIME_DEVMAN_WBXML;
        }
    }
    return CSR_BT_SMLS_MIME_INVALID_TO_BIG;
}

static void csrBtSmlsMapResponseCode(CsrBool               finalFlag,
                                     CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
    { 
        *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }
}

static void csrBtSmlsConnectIndHandler(void            *instData,
                                       CsrBtDeviceAddr deviceAddr,
                                       CsrBtConnId     cid,
                                       CsrUint16       maxPeerObexPacketLength,
                                       CsrUint16       obexResponsePacketLength,
                                       CsrUint32       length,
                                       CsrUint32       count,
                                       CsrUint16       targetHeaderLength,
                                       CsrUint8        *target,
                                       CsrBool         authenticated,
                                       CsrBool         challenged,
                                       CsrUint16       obexPacketLength,
                                       CsrUint8      *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->targetService         = csrBtSmlsCheckTargetHeader(target, targetHeaderLength);
    pInst->deviceAddr            = deviceAddr;

    if (pInst->targetService != CSR_BT_SMLS_SYNCML_SERVICE_UNKNOWN_TO_BIG)
    {
        if (authenticated)
        { 
            csrBtSmlsAuthCfmSend(pInst->appHandle);
        }
        pInst->obexCommand      = CSR_BT_OBEX_CONNECT_OPCODE;
        csrBtSmlsConnectIndSend(pInst->appHandle, 
                                (CsrUint32) pInst->localServerCh,
                                maxPeerObexPacketLength,
                                deviceAddr, 
                                pInst->targetService,
                                count,
                                length,
                                cid);
    }
    else
    {   
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target Header is missing or is invalid");
        csrBtSmlsConnectResponseHandler(pInst, 
                                        CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                        NULL, 
                                        description, 
                                        pInst->targetService);
    }    
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtSmlsPutNextIndHandler(void       *instData,
                                       CsrUint16  obexResponsePacketLength,
                                       CsrBool    authenticated,
                                       CsrBool    finalBitSet,
                                       CsrUint16  bodyLength,
                                       CsrUint16  bodyOffset,
                                       CsrUint16  obexPacketLength,
                                       CsrUint8 *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->finalFlag             = finalBitSet;

    if (pInst->mimeType == CSR_BT_SMLS_MIME_INVALID_TO_BIG)
    {
        pInst->mimeType = csrBtSmlsReturnMimeTypeFromHeader(obexPacket, obexPacketLength);
    }

    csrBtSmlsPutSmlMsgObjNextIndSend(pInst->appHandle, 
                                     (CsrUint32) pInst->localServerCh, 
                                     (CsrUint8) finalBitSet,
                                     pInst->mimeType, 
                                     bodyLength,
                                     bodyOffset, 
                                     obexPacket, 
                                     obexPacketLength);   

    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}

static void csrBtSmlsPutIndHandler(void          *instData,
                                   CsrUint16     obexResponsePacketLength,
                                   CsrBool       authenticated,
                                   CsrBool       finalBitSet,
                                   CsrUint16     bodyLength,
                                   CsrUint16     bodyOffset,
                                   CsrUint16     obexPacketLength,
                                   CsrUint8    *obexPacket)
{ /* The first Put Request packet is received                                   */
    CsrUint32  lengthOfObject;

    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->mimeType              = csrBtSmlsReturnMimeTypeFromHeader(obexPacket, obexPacketLength);
    pInst->obexCommand           = CSR_BT_OBEX_PUT_OPCODE;
    pInst->finalFlag             = finalBitSet;
    CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &lengthOfObject);

    if (authenticated)
    { 
        csrBtSmlsAuthCfmSend(pInst->appHandle);
    }

    csrBtSmlsPutSmlMsgObjIndSend(pInst->appHandle, 
                                 (CsrUint32) pInst->localServerCh, 
                                 (CsrUint8) finalBitSet,
                                 lengthOfObject, 
                                 pInst->mimeType, 
                                 bodyLength,
                                 bodyOffset, 
                                 obexPacket, 
                                 obexPacketLength);   

    if (!finalBitSet)
    { /* More OBEX PUT request packets to come                              */
        ObexUtilServSetPutContinueFunc(pInst->obexInst, csrBtSmlsPutNextIndHandler);
    }
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtSmlsGetNextIndHandler(void          *instData,
                                       CsrUint16     obexResponsePacketLength,
                                       CsrBool       authenticated,
                                       CsrBool       finalBitSet,
                                       CsrUint16     obexPacketLength,
                                       CsrUint8    *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    
    csrBtSmlsGetSmlMsgObjNextIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, pInst->mimeType);

    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlsGetCollectHeaderHandler(void          *instData,
                                             CsrUint16     obexResponsePacketLength,
                                             CsrBool       authenticated,
                                             CsrBool       finalBitSet,
                                             CsrUint16     obexPacketLength,
                                             CsrUint8    *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;

    if (pInst->mimeType == CSR_BT_SMLS_MIME_INVALID_TO_BIG)
    {
        pInst->mimeType = csrBtSmlsReturnMimeTypeFromHeader(obexPacket, obexPacketLength);
    }

    if (finalBitSet)
    { /* All the headers is received                                            */
        ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtSmlsGetNextIndHandler);
        csrBtSmlsGetSmlMsgObjIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, pInst->mimeType);
    }
    else
    { /* The client has more headers, e.g. the server is not allow to send and 
         body chunks until the final bit is set                                 */
        ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtSmlsGetCollectHeaderHandler);
        csrBtSmlsGetWithHeadersResponseHandler(pInst,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                               CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                               NULL,
                                               CSR_BT_OBEX_CONTINUE_RESPONSE_CODE, 
                                               0,
                                               0,
                                               NULL);
    }
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlsGetIndHandler(void          *instData,
                                   CsrUint16     obexResponsePacketLength,
                                   CsrBool       authenticated,
                                   CsrBool       finalBitSet,
                                   CsrUint16     obexPacketLength,
                                   CsrUint8    *obexPacket)
{ /* The first Get Request packet is received                                   */
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->obexCommand           = CSR_BT_OBEX_GET_OPCODE;
    pInst->mimeType              = CSR_BT_SMLS_MIME_INVALID_TO_BIG;

    if (authenticated)
    { 
        csrBtSmlsAuthCfmSend(pInst->appHandle);
    }
    csrBtSmlsGetCollectHeaderHandler(instData,
                                     obexResponsePacketLength,
                                     authenticated,
                                     finalBitSet,
                                     obexPacketLength,
                                     obexPacket);
}

static void csrBtSmlsAbortIndHandler(void              *instData,
                                     CsrUint16         descriptionOffset,
                                     CsrUint16         descriptionLength,
                                     CsrUint16         obexPacketLength,
                                     CsrUint8        *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->obexCommand           = CSR_BT_OBEX_ABORT_OPCODE;

    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket         */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;     
    }

    csrBtSmlsAbortIndSend(pInst->appHandle,
                          (CsrUint32) pInst->localServerCh,
                          descriptionOffset,
                          descriptionLength,
                          obexPacket, 
                          obexPacketLength);    
}

static void csrBtSmlsDisconnectIndHandler(void            *instData,
                                          CsrBtDeviceAddr deviceAddr,
                                          CsrBtReasonCode reasonCode,
                                          CsrBtSupplier   reasonSupplier,
                                          CsrUint16       obexPacketLength,
                                          CsrUint8      *obexPacket)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->obexCommand           = CSR_BT_OBEX_DISCONNECT_OPCODE;
    csrBtSmlsDisconnectIndSend(pInst->appHandle, 
                               (CsrUint32) pInst->localServerCh, 
                               deviceAddr,
                               reasonCode,
                               reasonSupplier);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSmlsAuthenticateIndHandler(void            *instData,
                                            CsrBtDeviceAddr deviceAddr,
                                            CsrUint8        options,
                                            CsrUint16       realmLength,
                                            CsrUint8      *realm)
{
    SmlsInstanceDataType * pInst   = (SmlsInstanceDataType *) instData;
    CsrBtSmlsAuthenticateInd *pMsg = (CsrBtSmlsAuthenticateInd *)
                                      CsrPmemAlloc(sizeof(CsrBtSmlsAuthenticateInd));

    pMsg->type          = CSR_BT_SMLS_AUTHENTICATE_IND;
    pMsg->deviceAddr    = deviceAddr;
    pMsg->options       = options;
    pMsg->realmLength   = realmLength;
    pMsg->realm         = realm;
    csrBtSmlsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSmlsActivateCfmHandler(void            *instData,
                                        CsrUint8   serverChannel,
                                        psm_t           psm,
                                        CsrBtResultCode resultCode,
                                        CsrBtSupplier   resultSupplier)
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    pInst->localServerCh         = serverChannel;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The SMLS server is activate.                                           */
        ObexUtilServConnectableStart(pInst->obexInst, 
                                     CSR_BT_SMLS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->localMaxPacketSize);
        csrBtSmlsActivateCfmSend(pInst->appHandle, CSR_BT_RESULT_CODE_OBEX_SUCCESS, CSR_BT_SUPPLIER_OBEX_PROFILES);
    }
    else
    {
        csrBtSmlsActivateCfmSend(pInst->appHandle, resultCode, resultSupplier);
    }
    CSR_UNUSED(psm);
}

static void csrBtSmlsDeactivateCfmHandler(void          *instData,
                                          CsrUint8 serverChannel,
                                          psm_t         psm) 
{
    SmlsInstanceDataType * pInst = (SmlsInstanceDataType *) instData;
    CsrBtSmlsDeactivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtSmlsDeactivateCfm));
    pInst->localServerCh         = serverChannel;
    pMsg->type                   = CSR_BT_SMLS_DEACTIVATE_CFM;
    csrBtSmlsMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(psm);
}

CsrUint8 CsrBtSmlsActivateReqHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsActivateReq *pMsg = (CsrBtSmlsActivateReq *) msg;
    CsrUint16 recordLength     = sizeof(smlsServiceRecord);
    CsrUint8 *record           = CsrPmemAlloc(recordLength);

    pInst->appHandle           = pMsg->appHandle;
    pInst->localMaxPacketSize = pMsg->obexMaxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;
    
    CsrMemCpy(record, smlsServiceRecord, recordLength);

    return (ObexUtilServActivateStart(pInst->obexInst,
                                      pInst->secIncoming,
                                      CSR_BT_OBEX_SYNCML_TRANSFER_UUID,
                                      CSR_BT_SMLS_LP_SUPERVISION_TIMEOUT,
                                      pInst->localServerCh,
                                      L2CA_PSM_INVALID,
                                      recordLength,
                                      CSR_BT_SMLS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      CSR_BT_OBJECT_TRANSFER_COD,
                                      csrBtSmlsConnectIndHandler,
                                      csrBtSmlsPutIndHandler,
                                      csrBtSmlsGetIndHandler,
                                      NULL,
                                      csrBtSmlsAbortIndHandler, 
                                      csrBtSmlsDisconnectIndHandler,
                                      csrBtSmlsAuthenticateIndHandler,
                                      csrBtSmlsActivateCfmHandler,
                                      NULL,
                                      pInst->localMaxPacketSize,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

CsrUint8 CsrBtSmlsConnectResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsConnectRes *pMsg = (CsrBtSmlsConnectRes *)msg;

    return (csrBtSmlsConnectResponseHandler(pInst,
                                            pMsg->responseCode, 
                                            NULL,
                                            NULL,
                                            pMsg->targetService));
}

CsrUint8 CsrBtSmlsAuthenticateReqHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsAuthenticateReq *pMsg      = (CsrBtSmlsAuthenticateReq *) msg;
    ObexUtilDigestChallengeType * chal  = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                                               &pMsg->password, 
                                                                               pMsg->passwordLength,  
                                                                               &pMsg->userId, 
                                                                               &pMsg->realm, 
                                                                               pMsg->realmLength);

    switch (pInst->obexCommand)
    {
        case CSR_BT_OBEX_CONNECT_OPCODE:
        {
            return (csrBtSmlsConnectResponseHandler(pInst, 
                                                    CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                    chal, 
                                                    NULL, 
                                                    pInst->targetService));
        }
        case CSR_BT_OBEX_PUT_OPCODE:
        {
            CsrUint8 headerPriorityTable[4];
            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 4);
            return (ObexUtilServPutWithHeaderResponse(pInst->obexInst, 
                                                      CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                      &chal, 
                                                      headerPriorityTable, 
                                                      NULL, 
                                                      NULL,
                                                      0, 
                                                      NULL, 
                                                      0, 
                                                      NULL,
                                                      CSR_BT_OBEX_SRMP_INVALID));
        }
        case CSR_BT_OBEX_GET_OPCODE:
        {
            return (csrBtSmlsGetWithHeadersResponseHandler(pInst,
                                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                           CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                           chal,
                                                           CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                           0,
                                                           0,
                                                           NULL));
        }
        default:
        { /* Can be crossing with a Abort or Disconnect                         */
            ObexUtilPfreeDigestChallengePointer(&chal);
            CsrBtSmlsFreeDownstreamMessageContents(CSR_BT_SMLS_PRIM, msg);
            return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        }
    } 
}

CsrUint8 CsrBtSmlsDeactivateReqHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilServDeactivateStart(pInst->obexInst, 
                                        pInst->localServerCh, 
                                        L2CA_PSM_INVALID,
                                        csrBtSmlsDeactivateCfmHandler));
}

CsrUint8 CsrBtSmlsAuthenticateResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsAuthenticateRes *pMsg = (CsrBtSmlsAuthenticateRes *) msg;
    return (ObexUtilServAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
}

CsrUint8 CsrBtSmlsPutSmlMsgObjResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsPutSmlMsgObjRes *pMsg = (CsrBtSmlsPutSmlMsgObjRes *) msg;
    csrBtSmlsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    return (ObexUtilServPutResponse(pInst->obexInst, pMsg->responseCode, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtSmlsPutSmlMsgObjNextResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsPutSmlMsgObjNextRes *pMsg = (CsrBtSmlsPutSmlMsgObjNextRes *) msg;
    csrBtSmlsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    return (ObexUtilServPutResponse(pInst->obexInst, pMsg->responseCode, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtSmlsGetSmlMsgObjResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsGetSmlMsgObjRes *pMsg = (CsrBtSmlsGetSmlMsgObjRes *) msg;
    csrBtSmlsMapResponseCode((CsrBool)pMsg->finalFlag, &pMsg->responseCode);
    return (csrBtSmlsGetWithHeadersResponseHandler(pInst,
                                                   CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                   CSR_BT_OBEX_UTIL_BODY_HEADER,
                                                   CSR_BT_SMLS_GET_BODY_HEADER_INDEX,
                                                   NULL,
                                                   pMsg->responseCode, 
                                                   pMsg->lengthOfObject,
                                                   pMsg->bodyLength,
                                                   pMsg->body));
}

CsrUint8 CsrBtSmlsGetSmlMsgObjNextResHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsGetSmlMsgObjNextRes *pMsg = (CsrBtSmlsGetSmlMsgObjNextRes *) msg;
    csrBtSmlsMapResponseCode((CsrBool)pMsg->finalFlag, &pMsg->responseCode);
    return (ObexUtilServGetResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    &pMsg->body, 
                                    pMsg->bodyLength,
                                    CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtSmlsSecurityInReqHandler(SmlsInstanceDataType *pInst, void *msg)
{
    CsrBtSmlsSecurityInReq *pMsg = (CsrBtSmlsSecurityInReq *) msg;

    CsrBtResultCode rval = CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                                                CSR_BT_OBEX_SYNCML_TRANSFER_MANDATORY_SECURITY_INCOMING,
                                                CSR_BT_OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING,
                                                CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtSmlsSecurityInCfmSend(pMsg->appHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

