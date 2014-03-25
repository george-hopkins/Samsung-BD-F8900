/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_fts_handler.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define FTS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX      0x18

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
#define FTS_L2CAP_PSM_SERVICE_RECORD_INDEX                  0x45
#else
#define FTS_L2CAP_PSM_SERVICE_RECORD_INDEX                  CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX
#endif

#define FTS_FOLDER_NEW_FLAG                                 0x00
#define FTS_FOLDER_FORWARD_OR_ROOT_FLAG                     0x02
#define FTS_FOLDER_BACKWARD_FLAG                            0x03
#define FTS_HEADER_SIZE                                     0x10
#define CSR_BT_FTS_TARGET_LENGTH                            0x10
#define CSR_BT_FTS_GET_BODY_HEADER_INDEX                    0x01

static const char CSR_BT_FTS_TYPE_FOLDERLIST[] = {"x-obex/folder-listing"};
#define CSR_BT_FTS_TYPE_FOLDERLIST_LENGTH                   0x15

static const CsrUint8 ftpUuidString[CSR_BT_FTS_TARGET_LENGTH] = 
{ 0xF9, 0xEC, 0x7B, 0xC4, 0x95, 0x3C, 0x11, 0xD2,
  0x98, 0x4E, 0x52, 0x54, 0x00, 0xDC, 0x9E, 0x09 };
static CsrUint8 csrBtFtsActivateStartHandler(FtsInstanceDataType *pInst);

static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x06,        /* 2 byte UUID, Service class = OBEXFileTransfer */

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

    /* profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, ProfileDescriptorList */
    0x35,0x08,
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x06,        /* 2 byte UUID, Service class = OBEXFileTransfer */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    0x09,0x01,0x02,        /* 2 byte uint, version = 1.2 */
#else
    0x09,0x01,0x00,        /* 2 byte uint, version = 1.0 */
#endif
    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x11,             /* 9 byte string */
    'O','B','E','X',' ','F','i','l','e','T','r','a','n','s','f','e','r',

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    /* OBEX-over-L2CAP */
    0x09, 0x02, 0x00,      /* 16 bit attribute */
    0x09, 0x00, 0x00,      /* 16 bit L2CAP PSM - filled in by app (index:34=0x22) */
#endif

    /* BrowseGroupList    */
    0x09, 0x00, 0x05,      /* AttrId = BrowseGroupList */
    0x35, 0x03,            /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,      /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

static void csrBtFtsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_FTS_PRIM, msg);
}

static void csrBtFtsSecurityInCfmSend(CsrSchedQid            appHandle, 
                                      CsrBtResultCode   resultCode,
                                      CsrBtSupplier     resultSupplier)
{
    CsrBtFtsSecurityInCfm *prim = CsrPmemAlloc(sizeof(CsrBtFtsSecurityInCfm));
    prim->type                  = CSR_BT_FTS_SECURITY_IN_CFM;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;
    csrBtFtsMessagePut(appHandle, prim);
}

static void csrBtFtsConnectIndSend(CsrSchedQid           hApp, 
                                   CsrUint32        connectionId, 
                                   CsrUint16        nPacketSizeMax, 
                                   CsrBtDeviceAddr  *pDeviceAddr,
                                   CsrUint32        length, 
                                   CsrUint32        count,
                                   CsrBtConnId       cid)
{
    CsrBtFtsConnectInd *pMsg    = CsrPmemAlloc(sizeof(CsrBtFtsConnectInd));
    pMsg->type                  = CSR_BT_FTS_CONNECT_IND;
    pMsg->connectionId          = connectionId;
    pMsg->obexPeerMaxPacketSize = (CsrUint16)(nPacketSizeMax - FTS_HEADER_SIZE);
    pMsg->deviceAddr            = *pDeviceAddr;
    pMsg->length                = length;
    pMsg->count                 = count;
    pMsg->btConnId              = cid;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsGetObjIndSend(CsrSchedQid        hApp, 
                                  CsrUint32     connectionId, 
                                  CsrUint16     nameOffset,
                                  CsrUint8    *payload, 
                                  CsrUint16     payloadLength)
{
    CsrBtFtsGetObjInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsGetObjInd));
    pMsg->type              = CSR_BT_FTS_GET_OBJ_IND;
    pMsg->connectionId      = connectionId;
    pMsg->ucs2nameOffset    = nameOffset;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsGetObjNextIndSend(CsrSchedQid    hApp, 
                                      CsrUint32 connectionId)
{
    CsrBtFtsGetObjNextInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsGetObjNextInd));
    pMsg->type                  = CSR_BT_FTS_GET_OBJ_NEXT_IND;
    pMsg->connectionId          = connectionId;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsGetListFolderIndSend(CsrSchedQid     hApp, 
                                         CsrUint32  connectionId, 
                                         CsrUint16  nameOffset,
                                         CsrUint8 *payload, 
                                         CsrUint16  payloadLength)
{
    CsrBtFtsGetListFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsGetListFolderInd));
    pMsg->type                      = CSR_BT_FTS_GET_LIST_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    pMsg->ucs2nameOffset            = nameOffset;
    pMsg->payload                   = payload;
    pMsg->payloadLength             = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsGetListFolderNextIndSend(CsrSchedQid     hApp, 
                                             CsrUint32  connectionId)
{
    CsrBtFtsGetListFolderNextInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsGetListFolderNextInd));
    pMsg->type                          = CSR_BT_FTS_GET_LIST_FOLDER_NEXT_IND;
    pMsg->connectionId                  = connectionId;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsPutObjIndSend(CsrSchedQid        hApp, 
                                  CsrUint32     connectionId,
                                  CsrUint8      isFinal, 
                                  CsrUint32     lenOfObj,
                                  CsrUint16     nameOffset,
                                  CsrUint16     bodyLength, 
                                  CsrUint16     bodyOffset,
                                  CsrUint8    *payload, 
                                  CsrUint16     payloadLength)
{
    CsrBtFtsPutObjInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsPutObjInd));
    pMsg->type              = CSR_BT_FTS_PUT_OBJ_IND;
    pMsg->connectionId      = connectionId;
    pMsg->finalFlag         = isFinal;
    pMsg->lengthOfObject    = lenOfObj;
    pMsg->ucs2nameOffset    = nameOffset;
    pMsg->bodyLength        = bodyLength;
    pMsg->bodyOffset        = bodyOffset;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsPutObjNextIndSend(CsrSchedQid    hApp,
                                      CsrUint32 connectionId,
                                      CsrUint8  isFinal,
                                      CsrUint16 bodyLength,
                                      CsrUint16 bodyOffset,
                                      void      *payload,
                                      CsrUint16 payloadLength)
{
    CsrBtFtsPutObjNextInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsPutObjNextInd));
    pMsg->type                  = CSR_BT_FTS_PUT_OBJ_NEXT_IND;
    pMsg->finalFlag             = isFinal;
    pMsg->connectionId          = connectionId;
    pMsg->bodyOffset            = bodyOffset;
    pMsg->bodyLength            = bodyLength;
    pMsg->payload               = payload;
    pMsg->payloadLength         = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsDelObjIndSend(CsrSchedQid        hApp, 
                                  CsrUint32     connectionId,
                                  CsrUint16     nameOffset, 
                                  CsrUint8    *payload, 
                                  CsrUint16     payloadLength)
{
    CsrBtFtsDelObjInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsDelObjInd));
    pMsg->type              = CSR_BT_FTS_DEL_OBJ_IND;
    pMsg->connectionId      = connectionId;
    pMsg->ucs2nameOffset    = nameOffset;
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsSetRootFolderIndSend(CsrSchedQid     hApp, 
                                         CsrUint32  connectionId)
{
    CsrBtFtsSetRootFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsSetRootFolderInd));
    pMsg->type                      = CSR_BT_FTS_SET_ROOT_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsSetFolderIndSend(CsrSchedQid     hApp, 
                                     CsrUint32  connectionId,
                                     CsrUint16  nameOffset, 
                                     CsrUint8 *payload, 
                                     CsrUint16  payloadLength)
{
    CsrBtFtsSetFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsSetFolderInd));
    pMsg->type                  = CSR_BT_FTS_SET_FOLDER_IND;
    pMsg->connectionId          = connectionId;
    pMsg->ucs2nameOffset        = nameOffset;
    pMsg->payload               = payload;
    pMsg->payloadLength         = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsSetBackFolderIndSend(CsrSchedQid     hApp, 
                                         CsrUint32  connectionId)
{
    CsrBtFtsSetBackFolderInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsSetBackFolderInd));
    pMsg->type                      = CSR_BT_FTS_SET_BACK_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsSetAddFolderIndSend(CsrSchedQid      hApp, 
                                        CsrUint32   connectionId,
                                        CsrUint16   nameOffset, 
                                        CsrUint8  *payload, 
                                        CsrUint16   payloadLength)
{
    CsrBtFtsSetAddFolderInd *pMsg   = CsrPmemAlloc(sizeof(CsrBtFtsSetAddFolderInd));
    pMsg->type                      = CSR_BT_FTS_SET_ADD_FOLDER_IND;
    pMsg->connectionId              = connectionId;
    pMsg->ucs2nameOffset            = nameOffset;
    pMsg->payload                   = payload;
    pMsg->payloadLength             = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsAuthCfmSend(CsrSchedQid hApp)
{   /* Digest validation success */
    CsrBtFtsAuthenticateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsAuthenticateCfm));
    pMsg->type                    = CSR_BT_FTS_AUTHENTICATE_CFM;
    csrBtFtsMessagePut(hApp, pMsg);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void csrBtFtsCopyObjIndSend(CsrSchedQid      hApp, 
                                   CsrUint32   connectionId,
                                   CsrUint16   ucs2srcNameOffset,
                                   CsrUint16   ucs2destNameOffset,
                                   CsrUint8  *payload,
                                   CsrUint16   payloadLength)
{
    CsrBtFtsCopyObjInd *pMsg    = (CsrBtFtsCopyObjInd *) 
                                   CsrPmemAlloc(sizeof(CsrBtFtsCopyObjInd));

    pMsg->type                  = CSR_BT_FTS_COPY_OBJ_IND;
    pMsg->connectionId          = connectionId;
    pMsg->ucs2srcNameOffset     = ucs2srcNameOffset;
    pMsg->ucs2destNameOffset    = ucs2destNameOffset;   
    pMsg->payload               = payload;
    pMsg->payloadLength         = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void csrBtFtsMoveObjIndSend(CsrSchedQid      hApp, 
                                   CsrUint32   connectionId,
                                   CsrUint16   ucs2srcNameOffset,
                                   CsrUint16   ucs2destNameOffset,
                                   CsrUint8  *payload,
                                   CsrUint16   payloadLength)
{
    CsrBtFtsMoveObjInd *pMsg    = (CsrBtFtsMoveObjInd *) 
                                   CsrPmemAlloc(sizeof(CsrBtFtsMoveObjInd));

    pMsg->type                  = CSR_BT_FTS_MOVE_OBJ_IND;
    pMsg->connectionId          = connectionId;
    pMsg->ucs2srcNameOffset     = ucs2srcNameOffset;
    pMsg->ucs2destNameOffset    = ucs2destNameOffset;   
    pMsg->payload               = payload;
    pMsg->payloadLength         = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}
#endif

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void csrBtFtsSetObjPermissionsIndSend(CsrSchedQid      hApp, 
                                             CsrUint32   connectionId,
                                             CsrUint16   ucs2nameOffset,
                                             CsrUint32   permissions,
                                             CsrUint8  *payload,
                                             CsrUint16   payloadLength)
{
    CsrBtFtsSetObjPermissionsInd *pMsg = (CsrBtFtsSetObjPermissionsInd *) 
                                          CsrPmemAlloc(sizeof(CsrBtFtsSetObjPermissionsInd));

    pMsg->type           = CSR_BT_FTS_SET_OBJ_PERMISSIONS_IND;
    pMsg->connectionId   = connectionId;
    pMsg->ucs2nameOffset = ucs2nameOffset;
    pMsg->permissions    = permissions;   
    pMsg->payload        = payload;
    pMsg->payloadLength  = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}
#endif

static void csrBtFtsAbortIndSend(CsrSchedQid     hApp, 
                                 CsrUint32  connectionId,
                                 CsrUint16  descriptionOffset,
                                 CsrUint16  descriptionLength,
                                 CsrUint8 *payload, 
                                 CsrUint16  payloadLength)
{
    CsrBtFtsAbortInd *pMsg  = CsrPmemAlloc(sizeof(CsrBtFtsAbortInd));
    pMsg->type              = CSR_BT_FTS_ABORT_IND;
    pMsg->connectionId      = connectionId;
    pMsg->descriptionOffset = descriptionOffset;
    pMsg->descriptionLength = descriptionLength; 
    pMsg->payload           = payload;
    pMsg->payloadLength     = payloadLength;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsDisconnectIndSend(CsrSchedQid            hApp, 
                                      CsrUint32         connectionId, 
                                      CsrBtDeviceAddr   deviceAddr,
                                      CsrBtReasonCode   reasonCode,
                                      CsrBtSupplier     reasonSupplier)
{
    CsrBtFtsDisconnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsDisconnectInd));
    pMsg->type                  = CSR_BT_FTS_DISCONNECT_IND;
    pMsg->connectionId          = connectionId;
    pMsg->deviceAddr            = deviceAddr;
    pMsg->reasonCode            = reasonCode;
    pMsg->reasonSupplier        = reasonSupplier;
    csrBtFtsMessagePut(hApp, pMsg);
}

static void csrBtFtsDeactivateCfmSend(CsrSchedQid hApp)
{
    CsrBtFtsDeactivateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsDeactivateCfm));
    pMsg->type                  = CSR_BT_FTS_DEACTIVATE_CFM;
    csrBtFtsMessagePut(hApp, pMsg);
}

static CsrBool csrBtFtsCheckTargetHeader(CsrUint8   *target, 
                                         CsrUint16  targetHeaderLength)
{
    if (target && targetHeaderLength == CSR_BT_FTS_TARGET_LENGTH)
    {
        if (!CsrMemCmp(target, ftpUuidString, CSR_BT_FTS_TARGET_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);       
}

static void csrBtFtsMapResponseCode(CsrBool               finalFlag,
                                    CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
    { 
        *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }
}

static CsrUint8 csrBtFtsConnectResponseHandler(FtsInstanceDataType          *pInst,
                                               CsrBtObexResponseCode        responseCode, 
                                               ObexUtilDigestChallengeType  *digestChallenge,
                                               CsrUcs2String                *description)
{
    CsrUint8  numOfHeaders            = 0;  
    CsrUint8  headerPriorityTable[1]  = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint16 whoHeaderLength         = CSR_BT_FTS_TARGET_LENGTH;
    CsrUint8  *who                    = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    responseCode                      = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    CsrMemCpy(who, ftpUuidString, whoHeaderLength); 


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

static CsrUint8 csrBtFtsSetpathResponseHandler(FtsInstanceDataType          *pInst,
                                               CsrBtObexResponseCode        responseCode, 
                                               ObexUtilDigestChallengeType  *digestChallenge)
{
    CsrUint8  headerPriorityTable[3] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    return(ObexUtilServSetpathResponse(pInst->obexInst, responseCode, &digestChallenge, headerPriorityTable, NULL, NULL, 0, NULL));
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static CsrUint8 csrBtFtsActionResponseHandler(FtsInstanceDataType          *pInst,
                                              CsrBtObexResponseCode        responseCode, 
                                              ObexUtilDigestChallengeType  *digestChallenge)
{
    CsrUint8  headerPriorityTable[2] = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    responseCode = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    
    return(ObexUtilServActionResponse(pInst->obexInst, responseCode, &digestChallenge, headerPriorityTable, NULL, 0, NULL));
}
#endif

static CsrUint8 csrBtFtsGetWithHeadersResponseHandler(FtsInstanceDataType         *pInst,
                                                      CsrUint8                    firstHeader,
                                                      CsrUint8                    secondHeader,
                                                      CsrUint8                    bodyHeaderPriorityIndex,
                                                      ObexUtilDigestChallengeType *digestChallenge,
                                                      CsrBtObexResponseCode       responseCode, 
                                                      CsrUint32                   lengthOfObject,
                                                      CsrUint16                   bodyLength,
                                                      CsrUint8                    *body,
                                                      CsrBool                     srmpOn)
{
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);
    headerPriorityTable[0] = firstHeader;
    headerPriorityTable[1] = secondHeader;

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
                                              ObexUtilReturnSrmpValue(srmpOn))); 
}

static void csrBtFtsConnectIndHandler(void            *instData,
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
                                      CsrUint8        *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;

    if (csrBtFtsCheckTargetHeader(target, targetHeaderLength))
    {   
        if (authenticated)
        { 
            csrBtFtsAuthCfmSend(pInst->appHandle);
        }
        pInst->obexCommand = CSR_BT_OBEX_CONNECT_OPCODE;
        csrBtFtsConnectIndSend(pInst->appHandle, 
                               (CsrUint32) pInst->localServerCh, 
                               maxPeerObexPacketLength, 
                               &deviceAddr, 
                               length, 
                               count,
                               cid);
    }
    else
    {   
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target Header is missing");
        csrBtFtsConnectResponseHandler(pInst, CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                       NULL, description);
    }   
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtFtsPutObjNextIndHandler(void       *instData,
                                         CsrUint16  obexResponsePacketLength,
                                         CsrBool    authenticated,
                                         CsrBool    finalBitSet,
                                         CsrUint16  bodyLength,
                                         CsrUint16  bodyOffset,
                                         CsrUint16  obexPacketLength,
                                         CsrUint8 *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->finalFlag            = finalBitSet;

    csrBtFtsPutObjNextIndSend(pInst->appHandle,
                              (CsrUint32) pInst->localServerCh,
                              finalBitSet,
                              bodyLength,
                              bodyOffset,
                              obexPacket,
                              obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}


static void csrBtFtsPutIndHandler(void          *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     bodyLength,
                                  CsrUint16     bodyOffset,
                                  CsrUint16     obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* The first Put Request packet is received                                   */
    CsrUint16  nameOffset;

    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->obexCommand          = CSR_BT_OBEX_PUT_OPCODE;
    CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset);
    
    if (authenticated)
    { 
        csrBtFtsAuthCfmSend(pInst->appHandle);
    }

    if (finalBitSet && bodyOffset == 0)
    { /* This must be a CSR_BT_FTS_DEL_OBJ_IND because the finalBit is Set
         and there is no body header included                                   */
        csrBtFtsDelObjIndSend(pInst->appHandle, 
                              (CsrUint32) pInst->localServerCh, 
                              nameOffset, 
                              obexPacket, 
                              obexPacketLength);
    }
    else
    {
        CsrUint32  lengthOfObjectValue;
        CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &lengthOfObjectValue);

        pInst->finalFlag = finalBitSet;
        csrBtFtsPutObjIndSend(pInst->appHandle, 
                              (CsrUint32) pInst->localServerCh,
                              finalBitSet, 
                              lengthOfObjectValue, 
                              nameOffset,
                              bodyLength,
                              bodyOffset,
                              obexPacket,
                              obexPacketLength);

        if (!finalBitSet)
        { /* More OBEX PUT request packets to come                              */
            ObexUtilServSetPutContinueFunc(pInst->obexInst, csrBtFtsPutObjNextIndHandler);
        }
    }
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtFtsGetObjIndHandler(void       *instData,
                                     CsrUint16  obexResponsePacketLength,
                                     CsrBool    authenticated,
                                     CsrBool    finalBitSet,
                                     CsrUint16  obexPacketLength,
                                     CsrUint8 *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;

    csrBtFtsGetObjNextIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtsGetGetListFolderIndHandler(void         *instData,
                                               CsrUint16    obexResponsePacketLength,
                                               CsrBool      authenticated,
                                               CsrBool      finalBitSet,
                                               CsrUint16    obexPacketLength,
                                               CsrUint8   *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;

    csrBtFtsGetListFolderNextIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtsGetCollectHeaderHandler(void        *instData,
                                            CsrUint16   obexResponsePacketLength,
                                            CsrBool     authenticated,
                                            CsrBool     finalBitSet,
                                            CsrUint16   obexPacketLength,
                                            CsrUint8  *obexPacket)
{
    FtsInstanceDataType * pInst     = (FtsInstanceDataType *) instData;
    
    if (pInst->name == NULL)
    { /* Look for the name header and if present copy it                        */
        pInst->name = CsrBtObexCreateName(CSR_BT_OBEX_REQUEST, obexPacket, &(pInst->nameLength));
    }

    if (!pInst->typeHeaderFlag)
    { /* A valid type header has not been found. Look for it                    */
        CsrUint16 length;
        CsrUint16 index = CsrBtObexTypeHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &length);
        
        if (index                               && 
            length > CSR_BT_OBEX_HEADER_LENGTH  && 
            obexPacketLength >= (index + CSR_BT_OBEX_HEADER_LENGTH + CSR_BT_FTS_TYPE_FOLDERLIST_LENGTH))
        {
            if (!CsrBtStrICmpLength((char*)&(obexPacket[index + CSR_BT_OBEX_HEADER_LENGTH]), (char*)CSR_BT_FTS_TYPE_FOLDERLIST, CSR_BT_FTS_TYPE_FOLDERLIST_LENGTH))
            { /* The Type header is found. Set typeHeaderFlag TRUE to indicate
                 this                                                           */
                pInst->typeHeaderFlag = TRUE;;
            }
        }
    }

    if (finalBitSet)
    { /* All the headers is received                                            */
        if (pInst->typeHeaderFlag)
        { /* A valid type header were present, e,g, the client want to pull the
             folder listing object. Send name as payload, ie. offset 0          */
            ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtFtsGetGetListFolderIndHandler);
            csrBtFtsGetListFolderIndSend(pInst->appHandle,
                                         (CsrUint32) pInst->localServerCh,
                                         0,
                                         pInst->name,
                                         pInst->nameLength);
        }
        else
        {
            ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtFtsGetObjIndHandler);
            csrBtFtsGetObjIndSend(pInst->appHandle,
                                  (CsrUint32) pInst->localServerCh,
                                  0,
                                  pInst->name,
                                  pInst->nameLength);
        }
        pInst->name = NULL;
    }
    else
    { /* The client has more headers, e.g. the server is not allow to send and 
         body chunks until the final bit is set. Set srmpOn TRUE 
         (CSR_BT_OBEX_SRMP_WAIT) in order to make sure the app can SRMP later   */
        ObexUtilServSetGetContinueFunc(pInst->obexInst, csrBtFtsGetCollectHeaderHandler);
        csrBtFtsGetWithHeadersResponseHandler(pInst,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                              CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                              NULL,
                                              CSR_BT_OBEX_CONTINUE_RESPONSE_CODE, 
                                              0,
                                              0,
                                              NULL,
                                              TRUE);
    }
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

static void csrBtFtsGetIndHandler(void          *instData,
                                  CsrUint16     obexResponsePacketLength,
                                  CsrBool       authenticated,
                                  CsrBool       finalBitSet,
                                  CsrUint16     obexPacketLength,
                                  CsrUint8    *obexPacket)
{ /* The first Get Request packet is received                                   */
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->obexCommand          = CSR_BT_OBEX_GET_OPCODE;
    pInst->typeHeaderFlag       = FALSE;
    CsrPmemFree(pInst->name);
    pInst->name                 = NULL; 
    pInst->nameLength           = 0;

    if (authenticated)
    { 
        csrBtFtsAuthCfmSend(pInst->appHandle);
    }
    csrBtFtsGetCollectHeaderHandler(instData, 
                                    obexResponsePacketLength, 
                                    authenticated, 
                                    finalBitSet, 
                                    obexPacketLength, 
                                    obexPacket);
}

static void csrBtFtsSetpathIndHandler(void          *instData,
                                      CsrUint16     obexResponsePacketLength,
                                      CsrUint8      flags,
                                      CsrUint8      constants,
                                      CsrBool       authenticated,
                                      CsrUint16     obexPacketLength,
                                      CsrUint8    *obexPacket)
{
    CsrUint16           nameOffset;
    FtsInstanceDataType * pInst     = (FtsInstanceDataType *) instData;
    pInst->obexCommand              = CSR_BT_OBEX_SET_PATH_OPCODE;
    
    if (authenticated)
    { 
        csrBtFtsAuthCfmSend(pInst->appHandle);
    }

    switch (flags)
    {
        case FTS_FOLDER_BACKWARD_FLAG:
        {
            csrBtFtsSetBackFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
            break;
        }
        case FTS_FOLDER_FORWARD_OR_ROOT_FLAG:
        {
            CsrBool nameRes = CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, obexPacket, &nameOffset);
            if(!nameRes)
            {
                csrBtFtsSetRootFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh);
            }
            else
            {
                csrBtFtsSetFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                                        nameOffset, obexPacket, obexPacketLength);
                obexPacket = NULL;
            }
            break;
        }
        default:
        { /* FTS_FOLDER_NEW_FLAG    */
            CsrBtObexGetNameOffset(CSR_BT_OBEX_REQUEST, obexPacket, &nameOffset);
            csrBtFtsSetAddFolderIndSend(pInst->appHandle, (CsrUint32) pInst->localServerCh, 
                                        nameOffset, obexPacket, obexPacketLength);
            obexPacket = NULL;
            break;
        }
    }
 
    CSR_UNUSED(constants);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtsAbortIndHandler(void              *instData,
                                    CsrUint16         descriptionOffset,
                                    CsrUint16         descriptionLength,
                                    CsrUint16         obexPacketLength,
                                    CsrUint8        *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->obexCommand          = CSR_BT_OBEX_ABORT_OPCODE;
    CsrPmemFree(pInst->name);
    pInst->name                 = NULL;

    if (descriptionOffset == 0)
    { /* An OBEX Descriptor header is not included, CsrPmemFree obexPacket         */
        CsrPmemFree(obexPacket);
        obexPacket          = NULL;
        obexPacketLength    = 0;     
    }

    csrBtFtsAbortIndSend(pInst->appHandle, 
                         (CsrUint32) pInst->localServerCh,
                         descriptionOffset,
                         descriptionLength,
                         obexPacket, 
                         obexPacketLength);    
}

static void csrBtFtsDisconnectIndHandler(void            *instData,
                                         CsrBtDeviceAddr deviceAddr,
                                         CsrBtReasonCode reasonCode,
                                         CsrBtSupplier   reasonSupplier,
                                         CsrUint16       obexPacketLength,
                                         CsrUint8      *obexPacket)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->obexCommand          = CSR_BT_OBEX_DISCONNECT_OPCODE;
    CsrPmemFree(pInst->name);
    pInst->name                 = NULL; 

    csrBtFtsDisconnectIndSend(pInst->appHandle, 
                              (CsrUint32) pInst->localServerCh, 
                              deviceAddr,
                              reasonCode,
                              reasonSupplier);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtsAuthenticateIndHandler(void            *instData,
                                           CsrBtDeviceAddr deviceAddr,
                                           CsrUint8        options,
                                           CsrUint16       realmLength,
                                           CsrUint8      *realm)
{
    FtsInstanceDataType * pInst   = (FtsInstanceDataType *) instData;
    CsrBtFtsAuthenticateInd *pMsg = CsrPmemAlloc(sizeof(CsrBtFtsAuthenticateInd));
    pMsg->type                    = CSR_BT_FTS_AUTHENTICATE_IND;
    pMsg->deviceAddr              = deviceAddr;
    pMsg->options                 = options;
    pMsg->realmLength             = realmLength;
    pMsg->realm                   = realm;
    csrBtFtsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtFtsActivateCfmHandler(void            *instData,
                                       CsrUint8   serverChannel,
                                       psm_t           psm,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier   resultSupplier)
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->localServerCh        = serverChannel;
    pInst->localPsm             = psm;
    
    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The FTP server is activate. Note the profile is allow to called this 
         function from the callback function                                    */
        ObexUtilServConnectableStart(pInst->obexInst, 
                                     CSR_BT_FTS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->localMaxPacketSize);
    }
    else
    { /* The FTP server cannot send give this information to the application
         as it has no API for it. Only thing to do is to activate it again      */
        csrBtFtsActivateStartHandler(pInst);
    }
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void csrBtFtsActionIndHandler(void                    *instData,
                                     CsrUint16               obexResponsePacketLength,
                                     CsrUint8                actionId,    
                                     CsrUint16               nameOffset,
                                     CsrUint16               destNameOffset,
                                     CsrUint32               permissions,
                                     CsrBool                 authenticated,
                                     CsrUint16               obexPacketLength,
                                     CsrUint8              *obexPacket)
{ /* An OBEX ACTION Request packet is received. Note the OBEX lib guarantee
     that the actionId header is correct                                        */
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->obexCommand          = CSR_BT_OBEX_ACTION_OPCODE;
 
    if (authenticated)
    { 
        csrBtFtsAuthCfmSend(pInst->appHandle);
    }

    switch (actionId)
    {
        case CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT:
        {
            csrBtFtsCopyObjIndSend(pInst->appHandle, 
                                   (CsrUint32) pInst->localServerCh,
                                   nameOffset,
                                   destNameOffset,
                                   obexPacket,
                                   obexPacketLength);
            break;
        }
        case CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT:
        {
            csrBtFtsMoveObjIndSend(pInst->appHandle, 
                                   (CsrUint32) pInst->localServerCh,
                                   nameOffset,
                                   destNameOffset,
                                   obexPacket,
                                   obexPacketLength);
            break;
        }
        default: /* CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS           */
        { 
            csrBtFtsSetObjPermissionsIndSend(pInst->appHandle, 
                                             (CsrUint32) pInst->localServerCh,
                                             nameOffset,
                                             permissions,
                                             obexPacket,
                                             obexPacketLength);
            break;
        }
    }
    CSR_UNUSED(obexResponsePacketLength);
}
#else
#define csrBtFtsActionIndHandler NULL
#endif

static void CsrBtDeactivateCfmHandler(void          *instData,
                                      CsrUint8 serverChannel,
                                      psm_t         psm) 
{
    FtsInstanceDataType * pInst = (FtsInstanceDataType *) instData;
    pInst->localServerCh        = serverChannel;
    pInst->localPsm             = psm;
    csrBtFtsDeactivateCfmSend(pInst->appHandle);
}

static CsrUint8 csrBtFtsActivateStartHandler(FtsInstanceDataType *pInst)
{
    CsrUint16 recordLength    = (CsrUint16) (sizeof(serviceRecord));
    CsrUint8  *record         = (CsrUint8 *)CsrPmemAlloc(recordLength);

    CsrMemCpy(record, serviceRecord, recordLength);
    
    return (ObexUtilServActivateStart(pInst->obexInst,
                                      pInst->secIncoming,
                                      CSR_BT_OBEX_FILE_TRANSFER_UUID,
                                      CSR_BT_FTS_LP_SUPERVISION_TIMEOUT,
                                      pInst->localServerCh,
                                      pInst->localPsm,
                                      recordLength,
                                      FTS_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                                      FTS_L2CAP_PSM_SERVICE_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      CSR_BT_OBJECT_TRANSFER_COD,
                                      csrBtFtsConnectIndHandler,
                                      csrBtFtsPutIndHandler,
                                      csrBtFtsGetIndHandler,
                                      csrBtFtsSetpathIndHandler,
                                      csrBtFtsAbortIndHandler,
                                      csrBtFtsDisconnectIndHandler,
                                      csrBtFtsAuthenticateIndHandler,
                                      csrBtFtsActivateCfmHandler,
                                      csrBtFtsActionIndHandler,
                                      pInst->localMaxPacketSize,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

CsrUint8 CsrBtFtsActivateReqHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsActivateReq *pMsg = (CsrBtFtsActivateReq *) msg;
    pInst->appHandle          = pMsg->appHandle;
    pInst->localSrmEnable     = pMsg->srmEnable;
    pInst->windowSize         = pMsg->windowSize;
    pInst->localMaxPacketSize = pMsg->obexMaxPacketSize;

    return (csrBtFtsActivateStartHandler(pInst));
}

CsrUint8 CsrBtFtsDeactivateReqHandler(FtsInstanceDataType *pInst, void *msg)
{
    return (ObexUtilServDeactivateStart(pInst->obexInst, 
                                        pInst->localServerCh, 
                                        pInst->localPsm,
                                        CsrBtDeactivateCfmHandler));
}

CsrUint8 CsrBtFtsConnectResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsConnectRes *pMsg = (CsrBtFtsConnectRes *) msg;
    return (csrBtFtsConnectResponseHandler(pInst, pMsg->responseCode,
                                           NULL, NULL));
}

CsrUint8 CsrBtFtsAuthenticateReqHandler(FtsInstanceDataType *pInst, void *msg) 
{
    CsrBtFtsAuthenticateReq *pMsg       = (CsrBtFtsAuthenticateReq *) msg;
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
            return (csrBtFtsConnectResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                       chal, NULL));
        }
        case CSR_BT_OBEX_PUT_OPCODE:
        {
            CsrUint8 headerPriorityTable[4]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

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
                                                      FALSE));
        }
        case CSR_BT_OBEX_GET_OPCODE:
        { 
            return (csrBtFtsGetWithHeadersResponseHandler(pInst,
                                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                          chal,
                                                          CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                          0,
                                                          0,
                                                          NULL,
                                                          FALSE));
        }
        case CSR_BT_OBEX_SET_PATH_OPCODE:
        {
            return(csrBtFtsSetpathResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, chal));
        }
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
        case CSR_BT_OBEX_ACTION_OPCODE:
        {
            return(csrBtFtsActionResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, chal));
        }
#endif
        default:
        {
            ObexUtilPfreeDigestChallengePointer(&chal);
            return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    } 
}

CsrUint8 CsrBtFtsAuthenticateResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsAuthenticateRes *pMsg = (CsrBtFtsAuthenticateRes *) msg;
    return (ObexUtilServAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
}

CsrUint8 CsrBtFtsPutObjResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsPutObjRes *pMsg = (CsrBtFtsPutObjRes *) msg;
    csrBtFtsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    return (ObexUtilServPutResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtsPutObjNextResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsPutObjNextRes *pMsg = (CsrBtFtsPutObjNextRes *) msg;
    csrBtFtsMapResponseCode(pInst->finalFlag, &pMsg->responseCode);
    return (ObexUtilServPutResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtsDelObjResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsDelObjRes *pMsg = (CsrBtFtsDelObjRes *) msg;
    return (ObexUtilServPutResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtsGetObjResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsGetObjRes *pMsg          = (CsrBtFtsGetObjRes *) msg;
    csrBtFtsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    
    if (pMsg->lengthOfObject > 0)
    {
        return (csrBtFtsGetWithHeadersResponseHandler(pInst, 
                                                      CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                      CSR_BT_OBEX_UTIL_BODY_HEADER,
                                                      CSR_BT_FTS_GET_BODY_HEADER_INDEX,
                                                      NULL,
                                                      pMsg->responseCode, 
                                                      pMsg->lengthOfObject, 
                                                      pMsg->bodyLength, 
                                                      pMsg->body,
                                                      pMsg->srmpOn));
    }
    else
    {
        return (ObexUtilServGetResponse(pInst->obexInst, 
                                        pMsg->responseCode, 
                                        &pMsg->body, 
                                        pMsg->bodyLength,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}

CsrUint8 CsrBtFtsGetObjNextResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsGetObjNextRes *pMsg = (CsrBtFtsGetObjNextRes *) msg;
    csrBtFtsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    return (ObexUtilServGetResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    &pMsg->body, 
                                    pMsg->bodyLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtsGetListFolderResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsGetListFolderRes *pMsg = (CsrBtFtsGetListFolderRes *) msg;
    csrBtFtsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    if (pMsg->lengthOfObject > 0)
    {
        return (csrBtFtsGetWithHeadersResponseHandler(pInst, 
                                                      CSR_BT_OBEX_UTIL_LENGTH_HEADER, 
                                                      CSR_BT_OBEX_UTIL_BODY_HEADER,
                                                      CSR_BT_FTS_GET_BODY_HEADER_INDEX,
                                                      NULL,
                                                      pMsg->responseCode, 
                                                      pMsg->lengthOfObject, 
                                                      pMsg->bodyLength, 
                                                      pMsg->body,
                                                      pMsg->srmpOn));
    }
    else
    {
        return (ObexUtilServGetResponse(pInst->obexInst, 
                                        pMsg->responseCode, 
                                        &pMsg->body, 
                                        pMsg->bodyLength,
                                        ObexUtilReturnSrmpValue(pMsg->srmpOn)));
    }
}


CsrUint8 CsrBtFtsGetListFolderNextResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsGetListFolderNextRes *pMsg = (CsrBtFtsGetListFolderNextRes *) msg;
    csrBtFtsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    return (ObexUtilServGetResponse(pInst->obexInst, 
                                    pMsg->responseCode, 
                                    &pMsg->body, 
                                    pMsg->bodyLength,
                                    ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtsSetFolderResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsSetFolderRes *pMsg = (CsrBtFtsSetFolderRes *) msg;
    return(csrBtFtsSetpathResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtFtsSetBackFolderResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsSetBackFolderRes *pMsg = (CsrBtFtsSetBackFolderRes *) msg;
    return(csrBtFtsSetpathResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtFtsSetRootFolderResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(csrBtFtsSetpathResponseHandler(pInst, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, NULL));
}

CsrUint8 CsrBtFtsSetAddFolderResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsSetAddFolderRes *pMsg = (CsrBtFtsSetAddFolderRes *) msg;
    return(csrBtFtsSetpathResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtFtsSecurityInReqHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsSecurityInReq *pMsg = (CsrBtFtsSecurityInReq*) msg;

    CsrBtResultCode rval = CsrBtScSetSecInLevel(&pInst->secIncoming, pMsg->secLevel,
                            CSR_BT_OBEX_FILE_TRANSFER_MANDATORY_SECURITY_INCOMING,
                            CSR_BT_OBEX_FILE_TRANSFER_DEFAULT_SECURITY_INCOMING,
                            CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                            CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtFtsSecurityInCfmSend(pMsg->appHandle, rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 CsrBtFtsCopyObjResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsCopyObjRes *pMsg = (CsrBtFtsCopyObjRes *) msg;
    return(csrBtFtsActionResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtFtsMoveObjResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsMoveObjRes *pMsg = (CsrBtFtsMoveObjRes *) msg;
    return(csrBtFtsActionResponseHandler(pInst, pMsg->responseCode, NULL));
}

CsrUint8 CsrBtFtsSetObjPermissionsResHandler(FtsInstanceDataType *pInst, void *msg)
{
    CsrBtFtsSetObjPermissionsRes *pMsg = (CsrBtFtsSetObjPermissionsRes *) msg;
    return(csrBtFtsActionResponseHandler(pInst, pMsg->responseCode, NULL));
}
#endif


