/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ops_handler.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define OPS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX      24
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define OPS_SUPPORTED_FORMATS_LIST_LENGTH_INDEX             69
#define OPS_L2CAP_PSM_SERVICE_RECORD_INDEX                  63
#else
#define OPS_SUPPORTED_FORMATS_LIST_LENGTH_INDEX             63
#define OPS_L2CAP_PSM_SERVICE_RECORD_INDEX                  CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
#endif

#define CSR_BT_OPS_GET_BODY_HEADER_INDEX                    0x02    /*ATT check up for OPS use*/

#ifdef CSR_BT_INSTALL_OPS_GET
static const char CSR_BT_OPS_TEXTTYPE_VCARD[]    = {"text/x-vcard"};
static const char CSR_BT_OPS_TEXTTYPE_VCAL[]     = {"text/x-vcalendar"};
static const char CSR_BT_OPS_TEXTTYPE_VNOTE[]    = {"text/x-vnote"};
static const char CSR_BT_OPS_TEXTTYPE_VMESSAGE[] = {"text/x-vmessage"};
#endif

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x05,        /* 2 byte UUID, Service class = OBEXObjectPush */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x11,             /* 17 bytes in total DataElSeq */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */

    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel template value 0 - to be filled in by app (index:24) */

    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19, 0x00, 0x08,      /* 2 byte UUID, Protocol = OBEX */

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */

    /* profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, ProfileDescriptorList */
    0x35,0x08,             /* 8 bytes in total DataElSeq    */ 
    0x35,0x06,             /* 6 bytes DataElSeq             */
    0x19,0x11,0x05,        /* 2 byte UUID, Service class = OBEXObjectPush */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    0x09,0x01,0x02,        /* 2 byte uint, version 1.2*/
#else
    0x09,0x01,0x00,        /* 2 byte uint, version = 1.0 */
#endif 
    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x03,             /* 3 byte string - OBEX push */
    'O','P','P',

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    /* OBEX-over-L2CAP */
    0x09, 0x02, 0x00,      /* 16 bit attribute */
    0x09, 0x00, 0x00,      /* 16 bit L2CAP PSM - filled in by app (index:34=0x22) */
#endif

    /* Supported Formats List */
    0x09,0x03,0x03,        /* AttrId - Supported Formats List */
    0x35,0x02,             /* 2 bytes in dataElSeq (58 : length of sequence) */
    /* 0x08, 0xff */       /* index:59 */
};

static void csrBtOpsActivateCfmHandler(void            *instData,
                                       CsrUint8   serverChannel,
                                       psm_t           psm,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier   resultSupplier);

static void csrBtOpsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_OPS_PRIM, msg);
}

static void csrBtOpsDeactivateCfmSend(CsrSchedQid queue)
{
    CsrBtOpsDeactivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsDeactivateCfm));

    pMsg->type = CSR_BT_OPS_DEACTIVATE_CFM;
    csrBtOpsMessagePut(queue, pMsg);
}

static void csrBtOpsSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtOpsSecurityInCfm *prim;
    prim = (CsrBtOpsSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtOpsSecurityInCfm));
    prim->type = CSR_BT_OPS_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    csrBtOpsMessagePut(appHandle, prim);
}

static void csrBtOpsDisconnectIndSend(CsrSchedQid          hApp, 
                                      CsrBtDeviceAddr deviceAddr,
                                      CsrBtReasonCode reasonCode,
                                      CsrBtSupplier   reasonSupplier)
{
    CsrBtOpsDisconnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsDisconnectInd));

    pMsg->type              = CSR_BT_OPS_DISCONNECT_IND;
    pMsg->deviceAddr        = deviceAddr;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    csrBtOpsMessagePut(hApp, pMsg);
}

static void csrBtOpsAbortIndSend(CsrSchedQid     hApp, 
                                 CsrUint16  descriptionOffset,
                                 CsrUint16  descriptionLength,
                                 CsrUint8 *payload, 
                                 CsrUint16  payloadLength)
{
    CsrBtOpsAbortInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtOpsAbortInd));

    pMsg->type              = CSR_BT_OPS_ABORT_IND;
    pMsg->descriptionOffset = descriptionOffset;
    pMsg->descriptionLength = descriptionLength; 
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtOpsMessagePut(hApp, pMsg);
}

static void csrBtOpsPutIndSend(CsrSchedQid  hApp,
                          CsrUint8    isFinal,
                          CsrUint16   bodyTypeOffset,
                          CsrUint16   bodyTypeLength,
                          CsrUint32   totalObjectSize,
                          CsrUint16   nameOffset,
                          CsrUint16   bodyLength,
                          CsrUint16   bodyOffset,
                          CsrUint8 *payload,
                          CsrUint16   payloadLength)
{
    CsrBtOpsPutInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsPutInd));

    pMsg->type           = CSR_BT_OPS_PUT_IND;
    pMsg->finalFlag       = isFinal;
    pMsg->bodyTypeOffset  = bodyTypeOffset;
    pMsg->bodyTypeLength  = bodyTypeLength;
    pMsg->totalObjectSize = totalObjectSize;
    pMsg->ucs2nameOffset  = nameOffset;
    pMsg->bodyLength      = bodyLength;
    pMsg->bodyOffset      = bodyOffset;
    pMsg->payload         = payload;
    pMsg->payloadLength   = payloadLength;
    csrBtOpsMessagePut(hApp, pMsg);
}

static void csrBtOpsPutNextIndSend(CsrSchedQid  hApp,
                              CsrUint8    isFinal,
                              CsrUint16   bodyTypeOffset,
                              CsrUint16   bodyTypeLength,
                              CsrUint16   nameOffset,
                              CsrUint16   bodyLength,
                              CsrUint16   bodyOffset,
                              CsrUint8 *payload,
                              CsrUint16   payloadLength)
{
    CsrBtOpsPutNextInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsPutNextInd));

    pMsg->type           = CSR_BT_OPS_PUT_NEXT_IND;
    pMsg->finalFlag      = isFinal;
    pMsg->bodyTypeOffset = bodyTypeOffset;
    pMsg->bodyTypeLength = bodyTypeLength;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->bodyLength     = bodyLength;
    pMsg->bodyOffset     = bodyOffset;
    pMsg->payload        = payload;
    pMsg->payloadLength  = payloadLength;
    csrBtOpsMessagePut(hApp, pMsg);
}

static void csrBtOpsConnectIndSend(CsrSchedQid           hApp, 
                                   CsrBtDeviceAddr  deviceAddr, 
                                   CsrUint16        nMaxPacketSize,
                                   CsrUint32        length,
                                   CsrUint32        count,
                                   CsrBtConnId      cid,
                                   CsrBool          authenticated,
                                   CsrBool          challenged,
                                   CsrUint8         *obexPacket)
{
    CsrBtOpsConnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsConnectInd));

    pMsg->type                  = CSR_BT_OPS_CONNECT_IND;
    pMsg->deviceAddr            = deviceAddr;
    pMsg->obexPeerMaxPacketSize = nMaxPacketSize - 20 - CSR_BT_OBEX_MAX_TYPE_LENGTH;
    pMsg->length                = length;
    pMsg->count                 = count;
    pMsg->btConnId              = cid;
#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    pMsg->authenticated         = authenticated;
    pMsg->challenge             = challenged;
#else
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacket);
    CSR_UNUSED(challenged);
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
    csrBtOpsMessagePut(hApp, pMsg);
}

#ifdef CSR_BT_INSTALL_OPS_GET
static void csrBtOpsGetIndSend(CsrSchedQid hApp, CsrUint8 bodyType)
{
    CsrBtOpsGetInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsGetInd));

    pMsg->type           = CSR_BT_OPS_GET_IND;
    pMsg->bodyType       = bodyType;
    csrBtOpsMessagePut(hApp, pMsg);
}

static void csrBtOpsGetNextIndSend(CsrSchedQid hApp, CsrUint8 bodyType)
{
    CsrBtOpsGetNextInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsGetNextInd));

    pMsg->type           = CSR_BT_OPS_GET_NEXT_IND;
    pMsg->bodyType       = bodyType;
    csrBtOpsMessagePut(hApp, pMsg);
}
#endif 

static CsrUint8 csrBtOpsConnectResponseHandler(OpsInstanceDataType          *pInst,
                                               CsrBtObexResponseCode        responseCode, 
                                               ObexUtilDigestChallengeType  *digestChallenge,
                                               CsrUcs2String                *description,
                                               CsrBool                      directAuthenticateResponse)
{
    CsrUint8  numOfHeaders           = 0;
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
#ifdef CSR_BT_INSTALL_OBEX_SRV_HEADER_TARGET_WHO_CID
    CsrUint16 whoHeaderLength        = 0x00;
    CsrUint8  *who                   = NULL;
#endif
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    if (description)
    {
        numOfHeaders = 1;
    }

    return (ObexUtilServConnectResponse(pInst->obexInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        CSR_BT_UNUSED_CONNECTION_ID,
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        &digestChallenge, 
                                        &description, 
                                        0, 
                                        NULL,
                                        directAuthenticateResponse));
}

static void csrBtOpsConnectIndHandler(void            *instData,
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
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;

    if((CSR_BT_OBEX_GET_REQUEST_LENGTH(obexPacket) >= CSR_BT_MIN_OBEX_CONNECT_LENGTH) &&
       (CSR_BT_OBEX_GET_CONNECT_MAX_SIZE(obexPacket) >= CSR_BT_MIN_OBEX_PACKET_LENGTH) )
    {
        csrBtOpsConnectIndSend(pInst->appHandle, 
                               deviceAddr, 
                               maxPeerObexPacketLength, 
                               length, 
                               count, 
                               cid, 
                               authenticated,
                               challenged,
                               obexPacket);
    }
    else
    {
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "INVALID CSRMAX packet length");
        
        csrBtOpsConnectResponseHandler(pInst, 
                                       CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                       NULL, 
                                       description, 
                                       FALSE);
#else
        csrBtOpsConnectResponseHandler(pInst, 
                                       CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE, 
                                       NULL, 
                                       NULL, 
                                       FALSE);
#endif
        
    }

    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(target);
    CSR_UNUSED(targetHeaderLength);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
static void csrBtOpsAuthenticateIndHandler(void            *instData,
                                           CsrBtDeviceAddr deviceAddr,
                                           CsrUint8        options,
                                           CsrUint16       realmLength,
                                           CsrUint8      *realm);
#else
#define csrBtOpsAuthenticateIndHandler NULL
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */

#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
static void csrBtOpsAuthenticateIndHandler(void            *instData,
                                           CsrBtDeviceAddr deviceAddr,
                                           CsrUint8        options,
                                           CsrUint16       realmLength,
                                           CsrUint8      *realm)
{
    OpsInstanceDataType * pInst   = (OpsInstanceDataType *) instData;
    CsrBtOpsAuthenticateInd *pMsg = CsrPmemAlloc(sizeof(CsrBtOpsAuthenticateInd));
    pMsg->type                    = CSR_BT_OPS_AUTHENTICATE_IND;
    pMsg->deviceAddr              = deviceAddr;
    pMsg->options                 = options;
    pMsg->realmLength             = realmLength;
    pMsg->realm                   = realm;
    csrBtOpsMessagePut(pInst->appHandle, pMsg);
}
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */

static void csrBtOpsPutNextIndHandler(void       *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     bodyLength,
                                  CsrUint16     bodyOffset,
                                  CsrUint16     obexPacketLength,
                                      CsrUint8 *obexPacket)
{
    CsrUint16 nameOffset;
    CsrUint16 typeIndex;
    CsrUint16 typeLength;
    CsrUint16 typeOffset;

    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;
    pInst->finalFlag            = finalBitSet;

    CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset);

    typeIndex = CsrBtObexTypeHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeLength);

    if(typeIndex)
    {
        typeOffset = (CsrUint16)(typeIndex + CSR_BT_OBEX_HEADER_LENGTH);
        typeLength = (CsrUint16)(typeLength - CSR_BT_OBEX_HEADER_LENGTH);
    }
    else
    {
        typeLength = 0;
        typeOffset = 0;
    }

    csrBtOpsPutNextIndSend(pInst->appHandle,
                        finalBitSet,
                        typeOffset,
                        typeLength,
                        nameOffset,
                        bodyLength,
                        bodyOffset,
                        obexPacket,
                            obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}

static void csrBtOpsPutIndHandler(void          *instData,
                                      CsrUint16  obexResponsePacketLength,
                                      CsrBool    authenticated,
                                      CsrBool    finalBitSet,
                                      CsrUint16  bodyLength,
                                      CsrUint16  bodyOffset,
                                      CsrUint16  obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* The first Put packet is received*/
    CsrUint16 nameOffset;
    CsrUint32 totalObjectSize;
    CsrUint16 typeIndex;
    CsrUint16 typeLength;
    CsrUint16 typeOffset;

    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;

    CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset);
    CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &totalObjectSize);

    typeIndex = CsrBtObexTypeHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeLength);

    if(typeIndex)
    {
        typeOffset = (CsrUint16)(typeIndex + CSR_BT_OBEX_HEADER_LENGTH);
        typeLength = (CsrUint16)(typeLength - CSR_BT_OBEX_HEADER_LENGTH);
    }
    else
    {
        typeLength = 0;
        typeOffset = 0;
    }

    pInst->finalFlag = finalBitSet;
    csrBtOpsPutIndSend(pInst->appHandle,
                            finalBitSet,
                            typeOffset,
                            typeLength,
                        totalObjectSize,
                            nameOffset,
                            bodyLength,
                            bodyOffset,
                            obexPacket,
                        obexPacketLength);

    if (!finalBitSet)
    {   /* next coming Put packets to be received...*/
        ObexUtilServSetPutContinueFunc(pInst->obexInst, csrBtOpsPutNextIndHandler);
    }
    CSR_UNUSED(obexResponsePacketLength);
}

#ifdef CSR_BT_INSTALL_OPS_GET
static CsrUint8 csrBtOpsGetWithHeadersResponseHandler(OpsInstanceDataType         *pInst,
                                                      CsrUint8                    firstHeader,
                                                      CsrUint8                    secondHeader,
                                                      CsrUint8                    thirdHeader,
                                                      CsrUint8                    bodyHeaderPriorityIndex,
                                                      ObexUtilDigestChallengeType *digestChallenge,
                                                      CsrBtObexResponseCode       responseCode, 
                                                      CsrUint32                   lengthOfObject,
                                                      CsrUcs2String               *ucs2name,
                                                      CsrUint16                   bodyLength,
                                                      CsrUint8                    *body,
                                                      CsrBool                     srmpOn)
{
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
    headerPriorityTable[0] = firstHeader;
    headerPriorityTable[1] = secondHeader;
    headerPriorityTable[2] = thirdHeader;

    return (ObexUtilServGetWithHeaderResponse(pInst->obexInst, 
                                              responseCode,
                                              &digestChallenge, 
                                              headerPriorityTable, 
                                              bodyHeaderPriorityIndex, 
                                              lengthOfObject,
                                              &ucs2name, 
                                              NULL, 
                                              0, 
                                              NULL, 
                                              0, 
                                              NULL, 
                                              &body, 
                                              bodyLength,
                                              ObexUtilReturnSrmpValue(srmpOn))); 
}

static CsrBool csrBtOpsGetTypeHeader(CsrUint8  *obexPacket, 
                                     CsrUint16   obexPacketLength,
                                     CsrUint8    *cardType)
{
    CsrUint16 typeOffset;
    CsrUint16 typeLength;

    if (CsrBtObexGetTypeOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &typeOffset, &typeLength))
    { /* A type header is found                                                 */    
        if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_OPS_TEXTTYPE_VCARD)) &&
           (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]),
                                (char*)CSR_BT_OPS_TEXTTYPE_VCARD,
                                sizeof(CSR_BT_OPS_TEXTTYPE_VCARD))))
        {
            *cardType = CSR_BT_VCARD_TYPE;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_OPS_TEXTTYPE_VCAL)) &&
           (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]),
                                (char*)CSR_BT_OPS_TEXTTYPE_VCAL,
                                sizeof(CSR_BT_OPS_TEXTTYPE_VCAL))))
        {
            *cardType = CSR_BT_VCAL_TYPE;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_OPS_TEXTTYPE_VNOTE)) &&
           (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]),
                                (char*)CSR_BT_OPS_TEXTTYPE_VNOTE,
                                sizeof(CSR_BT_OPS_TEXTTYPE_VNOTE))))
        {
            *cardType = CSR_BT_VNOTE_TYPE;
        }
        else if (obexPacketLength >= (typeOffset + sizeof(CSR_BT_OPS_TEXTTYPE_VMESSAGE)) &&
           (!CsrBtStrICmpLength((char*)&(obexPacket[typeOffset]),
                                (char*)CSR_BT_OPS_TEXTTYPE_VMESSAGE,
                                sizeof(CSR_BT_OPS_TEXTTYPE_VMESSAGE))))
        {
            *cardType = CSR_BT_VMESSAGE_TYPE;
        }
        else 
        {
            *cardType = CSR_BT_UNKNOWN_TYPE;
        }
        return TRUE;
    }
    return FALSE;
}

static void csrBtOpsGetObjIndHandler(void       *instData,
                                     CsrUint16  obexResponsePacketLength,
                                     CsrBool    authenticated,
                                     CsrBool    finalBitSet,
                                     CsrUint16  obexPacketLength,
                                     CsrUint8 *obexPacket)
{
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;

    csrBtOpsGetNextIndSend(pInst->appHandle, pInst->cardType);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtOpsGetCollectHeaderHandler(void        *instData,
                                            CsrUint16   obexResponsePacketLength,
                                            CsrBool     authenticated,
                                            CsrBool     finalBitSet,
                                            CsrUint16   obexPacketLength,
                                            CsrUint8  *obexPacket)
{
    CsrUint16 length;
    OpsInstanceDataType * pInst     = (OpsInstanceDataType *) instData;

    CsrBtObexNameHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &length);

    if (length > CSR_BT_OBEX_HEADER_LENGTH)
    { /* Received a name header with an actual payload in */
        csrBtOpsGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION,
                                              NULL,
                                              CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE,
                                              0,
                                              NULL,
                                              0,
                                              NULL,
                                              FALSE);
        return;
    }

    if (!pInst->typeHeaderFlag)
    { /* A valid type header has not been found. Look for it                    */
        pInst->typeHeaderFlag = csrBtOpsGetTypeHeader(obexPacket, 
                                                      obexPacketLength, 
                                                      &pInst->cardType);
    }

    if (finalBitSet)
    { /* All the headers is received                                            */
        if (pInst->typeHeaderFlag)
        { /* A valid type header has been present, e.g. the client want to pull the
             default vcard.*/

            if (pInst->cardType == CSR_BT_UNKNOWN_TYPE)
            { /* IrOBEX v1.2 - 8.4 */
                csrBtOpsGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION,
                                              NULL,
                                              CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE,
                                              0,
                                              NULL,
                                              0,
                                              NULL,
                                              FALSE);
            }
            else
            {
                ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtOpsGetObjIndHandler);
                if (pInst->getFinalFlag)
                {
                    csrBtOpsGetIndSend(pInst->appHandle, pInst->cardType);
                }
                else
                {
                    csrBtOpsGetNextIndSend(pInst->appHandle, pInst->cardType);
                }
                pInst->cardType = CSR_BT_UNKNOWN_TYPE;
            }
        }
        else
        {   /* IrOBEX v1.2 - 8.4 */
            csrBtOpsGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_COMMON_RESPONSE_OPERATION,
                                              NULL,
                                              CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE,
                                              0,
                                              NULL,
                                              0,
                                              NULL,
                                              FALSE);
        }
    }
    else
    { /* The client has more header(s), e.g. the server is not allowed to send 
         until the final bit is set   actually I do not think this will happen in OPS !
         Set srmpOn TRUE (CSR_BT_OBEX_SRMP_WAIT) in order to make sure the app can SRMP later  */
        ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtOpsGetCollectHeaderHandler);
        csrBtOpsGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                              NULL,
                                              CSR_BT_OBEX_CONTINUE_RESPONSE_CODE, 
                                              0,
                                              NULL,
                                              0,
                                              NULL,
                                              TRUE);
    }
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void csrBtOpsGetIndHandler(void          *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* first Get packet is received*/
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;
    pInst->typeHeaderFlag       = FALSE;

    csrBtOpsGetCollectHeaderHandler(instData,
                                    obexResponsePacketLength,
                                    authenticated,
                                    finalBitSet,
                                    obexPacketLength,
                                    obexPacket);
}
#else
static void csrBtOpsGetIndHandler(void          *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* first Get packet is received*/
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;
    pInst->typeHeaderFlag       = FALSE;
    CsrPmemFree(obexPacket);
    ObexUtilServGetResponse(pInst->obexInst, 
                                        CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 
                                        NULL, 
                                        0, 
                                        CSR_BT_OBEX_SRMP_INVALID);
}
#endif

static void csrBtOpsAbortIndHandler(void              *instData,
                                    CsrUint16         descriptionOffset,
                                    CsrUint16         descriptionLength,
                                    CsrUint16         obexPacketLength,
                                    CsrUint8        *obexPacket)
{
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;

    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket         */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;
    }
#ifdef CSR_BT_INSTALL_OPS_GET
    pInst->getFinalFlag     = TRUE;
#endif
    csrBtOpsAbortIndSend(pInst->appHandle,
                         descriptionOffset,
                         descriptionLength,
                         obexPacket,
                         obexPacketLength);
}


static void csrBtOpsDisconnectIndHandler(void            *instData,
                                         CsrBtDeviceAddr deviceAddr,
                                         CsrBtReasonCode reasonCode,
                                         CsrBtSupplier   reasonSupplier,
                                         CsrUint16       obexPacketLength,
                                         CsrUint8      *obexPacket)
{
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;

    csrBtOpsDisconnectIndSend(pInst->appHandle, deviceAddr, reasonCode, reasonSupplier);
#ifdef CSR_BT_INSTALL_OPS_GET
    pInst->getFinalFlag = TRUE;
#endif
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}


static CsrUint8 csrBtOpsActivateStartHandler(OpsInstanceDataType *pInst)
{
    CsrUint8    *record;
    CsrUint16   recordLength = (CsrUint16) sizeof(serviceRecord);
    CsrUint16   supportedFormatsListLength = 0;
    CsrUint16   index;
    CsrUint16   formatMask = 0x0001;

    /* calculate the supported formats list length */
    if (pInst->supportedFormats == CSR_BT_ANY_TYPE_SUPPORT)
    {
        /* +1 since we need to add the CSR_BT_ANY_TYPE_SUPPORT format */
        supportedFormatsListLength = CSR_BT_NUMBER_OF_INDIVIDUAL_FORMATS + 1;
        pInst->supportedFormats = (CSR_BT_VCARD_2_1_SUPPORT | CSR_BT_VCARD_3_0_SUPPORT |
                                   CSR_BT_VCAL_1_0_SUPPORT | CSR_BT_ICAL_2_0_SUPPORT | 
                                   CSR_BT_VNOTE_SUPPORT | CSR_BT_VMESSAGE_SUPPORT |
                                   CSR_BT_OTHER_TYPE_SUPPORT);
    }
    else
    {
        CsrUintFast16 index;
        CsrUint16 formatMask = 0x0001;

        for (index = 0; index < CSR_BT_NUMBER_OF_INDIVIDUAL_FORMATS; index++)
        {
            if (formatMask & pInst->supportedFormats)
            {
                supportedFormatsListLength++;
            }
            formatMask <<= 1;
        }

        if (pInst->supportedFormats & CSR_BT_OTHER_TYPE_SUPPORT)
        {
                supportedFormatsListLength++;
        }
    }
    /* create the record */
    record = CsrPmemAlloc(recordLength + 2*supportedFormatsListLength);
    CsrMemCpy(record, serviceRecord, recordLength);
    record[OPS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX] = pInst->localServerCh;
    record[OPS_SUPPORTED_FORMATS_LIST_LENGTH_INDEX] = (CsrUint8)(2 * supportedFormatsListLength);

    /* insert the supported formats list */
    formatMask = 0x0001;
    for (index = 0; index < CSR_BT_NUMBER_OF_INDIVIDUAL_FORMATS; index++)
    {
        if (formatMask & pInst->supportedFormats)
        {
            record[recordLength++] = 0x08;    /* type:1 (unsigned int), size:0 (1 byte) */
            record[recordLength++] = (CsrUint8)(index + 1);
        }
        formatMask <<= 1;
    }

    if (pInst->supportedFormats & CSR_BT_OTHER_TYPE_SUPPORT)
    {
        record[recordLength++] = 0x08;    /* type:1 (unsigned int), size:0 (1 byte) */
        record[recordLength++] = CSR_BT_OTHER_FORMAT_TYPE;
    }

    return (ObexUtilServActivateStart(pInst->obexInst,
                                      pInst->secIncoming,
                                      CSR_BT_OBEX_OBJECT_PUSH_SERVICE_UUID,
                                      CSR_BT_OPS_LP_SUPERVISION_TIMEOUT,
                                      pInst->localServerCh,
                                      L2CA_PSM_INVALID,
                                      recordLength,
                                      OPS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                      OPS_L2CAP_PSM_SERVICE_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      CSR_BT_OBJECT_TRANSFER_COD,
                                      csrBtOpsConnectIndHandler,
                                      csrBtOpsPutIndHandler,
                                      csrBtOpsGetIndHandler,
                                      NULL,
                                      csrBtOpsAbortIndHandler,
                                      csrBtOpsDisconnectIndHandler,
                                      csrBtOpsAuthenticateIndHandler,
                                      csrBtOpsActivateCfmHandler,
                                      NULL,
                                      pInst->localMaxPacketSize,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

static void csrBtOpsActivateCfmHandler(void            *instData,
                                       CsrUint8   serverChannel,
                                       psm_t           psm,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier   resultSupplier)
{
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;
    pInst->localServerCh        = serverChannel;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The OPP server is activated. Note the profile is allowed to call this 
         function from the callback function                                    */
        ObexUtilServConnectableStart(pInst->obexInst, 
                                     CSR_BT_OPS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->localMaxPacketSize);
    }
    else
    { /* The OPP server cannot send give this information to the application
         as it has no API for it. Only thing to do is to activate downwards again      */
        csrBtOpsActivateStartHandler(pInst);
    }
    CSR_UNUSED(psm);
}

#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
CsrUint8 CsrBtOpsAuthenticateResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsAuthenticateRes *pMsg = (CsrBtOpsAuthenticateRes *) msg;
    return(ObexUtilServAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
}
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */ 

static void csrBtOpsDeactivateCfmHandler(void          *instData, 
                                         CsrUint8 serverChannel,
                                         psm_t         psm)
{
    OpsInstanceDataType * pInst = (OpsInstanceDataType *) instData;
    pInst->localServerCh        = serverChannel;
#ifdef CSR_BT_INSTALL_OPS_GET
    pInst->getFinalFlag         = TRUE;
#endif
    csrBtOpsDeactivateCfmSend(pInst->appHandle);
    CSR_UNUSED(psm);
}

CsrUint8 CsrBtOpsDeactivateReqHandler(OpsInstanceDataType *pInst, void *msg)
{
    return (ObexUtilServDeactivateStart(pInst->obexInst, 
                                        pInst->localServerCh, 
                                        L2CA_PSM_INVALID,
                                        csrBtOpsDeactivateCfmHandler));
}

static void csrBtOpsMapResponseCode(CsrBool finalFlag, CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
    {
        *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }
}


CsrUint8 CsrBtOpsPutResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsPutRes *pMsg = (CsrBtOpsPutRes *) msg;
    csrBtOpsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    return (ObexUtilServPutResponse(pInst->obexInst, 
                                    pMsg->responseCode,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

#ifdef CSR_BT_INSTALL_OPS_GET
CsrUint8 CsrBtOpsGetResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsGetRes *pMsg          = (CsrBtOpsGetRes *) msg;

    csrBtOpsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    pInst->getFinalFlag = pMsg->finalFlag;

    return (csrBtOpsGetWithHeadersResponseHandler(pInst,
                                                  CSR_BT_OBEX_UTIL_NAME_HEADER,
                                                  CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                  CSR_BT_OBEX_UTIL_BODY_HEADER,
                                                  CSR_BT_OPS_GET_BODY_HEADER_INDEX,
                                                  NULL,
                                                  pMsg->responseCode,
                                                  pMsg->totalObjectSize,
                                                  pMsg->ucs2name,
                                                  pMsg->bodyLength,
                                                  pMsg->body,
                                                  pMsg->srmpOn));
}

CsrUint8 CsrBtOpsGetNextResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsGetNextRes *pMsg = (CsrBtOpsGetNextRes *) msg;
    csrBtOpsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    pInst->getFinalFlag = pMsg->finalFlag;
    return (ObexUtilServGetResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    &pMsg->body, 
                                    (CsrUint16)pMsg->bodyLength, /* cast 32 -> 16 */
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}
#endif

#ifndef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
CsrUint8 CsrBtOpsConnectResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsConnectRes          *pMsg = (CsrBtOpsConnectRes *) msg;
    return (csrBtOpsConnectResponseHandler(pInst, 
                                           pMsg->responseCode, 
                                           NULL, 
                                           NULL, 
                                           FALSE));
}
#else
CsrUint8 CsrBtOpsConnectResHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsConnectRes          *pMsg = (CsrBtOpsConnectRes *) msg;
    ObexUtilDigestChallengeType *chal = NULL;
    if (pMsg->password)
    {
        chal  = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                     &pMsg->password, 
                                                     pMsg->passwordLength,  
                                                     &pMsg->userId, 
                                                     &pMsg->realm, 
                                                     pMsg->realmLength);
    }
    else
    {
        CsrPmemFree(pMsg->userId);
        CsrPmemFree(pMsg->realm);
    }
    return (csrBtOpsConnectResponseHandler(pInst, 
                                           pMsg->responseCode, 
                                           chal, 
                                           NULL, 
                                           pMsg->authenticateResponse));
}
#endif

CsrUint8 CsrBtOpsActivateReqHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsActivateReq *pMsg = (CsrBtOpsActivateReq *) msg;
    pInst->appHandle = pMsg->appHandle;
    pInst->supportedFormats = pMsg->supportedFormats;
    pInst->localSrmEnable = pMsg->srmEnable;
    pInst->windowSize = pMsg->windowSize;
    pInst->localMaxPacketSize = pMsg->obexMaxPacketSize;
    return (csrBtOpsActivateStartHandler(pInst));
}


/* Security level change and confirm-send */
CsrUint8 CsrBtOpsSecurityInReqHandler(OpsInstanceDataType *pInst, void *msg)
{
    CsrBtOpsSecurityInReq *pMsg = (CsrBtOpsSecurityInReq*) msg;

    CsrBtResultCode rval = CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                            CSR_BT_OBEX_OBJECT_PUSH_MANDATORY_SECURITY_INCOMING,
                            CSR_BT_OBEX_OBJECT_PUSH_DEFAULT_SECURITY_INCOMING,
                            CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                            CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtOpsSecurityInCfmSend(pMsg->appHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

