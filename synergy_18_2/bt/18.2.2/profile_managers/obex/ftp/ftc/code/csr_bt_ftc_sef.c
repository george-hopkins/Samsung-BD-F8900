/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ftc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"

#define FTC_TARGET {0xF9, 0xEC, 0x7B, 0xC4, 0x95, 0x3C, 0x11, 0xD2, 0x98, 0x4E, 0x52, 0x54, 0x00, 0xDC, 0x9E, 0x09}
#define FTC_TARGET_LENGTH 0x10
                    
static CsrBtObexResponseCode csrBtFtcResponseCodeConverter(CsrBtObexResponseCode responseCode, CsrBool *finalFlag)
{
    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        *finalFlag = FALSE;
        return (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
    }
    else
    {
        *finalFlag = TRUE;
        return (responseCode);
    }
}

static CsrUint32 csrBtFtcGetLengthOfObject(CsrUint8 *obexPacket)
{
    if(obexPacket)
    {
        CsrUint32 lengthOfObject;
        if(CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &lengthOfObject))
        {
            return (lengthOfObject); 
        }
        else
        {
            return (0);
        }
    }
    else
    {
        return (0);
    }
}

static void csrBtFtcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_FTC_PRIM, msg);
}

static void csrBtFtcConnectResultHandler(void                     *instData,
                                         CsrBtResultCode          resultCode,
                                         CsrBtSupplier            resultSupplier,
                                         CsrBtDeviceAddr          deviceAddr,
                                         CsrBtConnId              cid,
                                         CsrUint16                maxPeerObexPacketLength,
                                         CmnCsrBtLinkedListStruct *sdpTag,
                                         CsrUint16                obexPacketLength,
                                         CsrUint8               *obexPacket)
{
    FtcInstanceData * pInst     = instData;
    CsrBtFtcConnectCfm *pMsg    = (CsrBtFtcConnectCfm *) CsrPmemAlloc(sizeof(CsrBtFtcConnectCfm));
    pMsg->type                  = CSR_BT_FTC_CONNECT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;        
    pMsg->obexPeerMaxPacketSize = maxPeerObexPacketLength;
    pMsg->btConnId              = cid;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);

    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcAuthenticateIndHandler(void     *instData,
                                      CsrBtDeviceAddr  deviceAddr,
                                      CsrUint8       options,
                                      CsrUint16      realmLength,
                                      CsrUint8    *realm)
{
    FtcInstanceData * pInst = instData;

    CsrBtFtcAuthenticateInd *pMsg = (CsrBtFtcAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtFtcAuthenticateInd));

    pMsg->type              = CSR_BT_FTC_AUTHENTICATE_IND;
    pMsg->deviceAddr        = deviceAddr;
    pMsg->options           = options;
    pMsg->realmLength       = realmLength;
    pMsg->realm             = realm;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
}

static void csrBtFtcDisconnectResultHandler(void                *instData,
                                            CsrBtReasonCode     reasonCode,
                                            CsrBtSupplier reasonSupplier,
                                            CsrUint8          *obexPacket,
                                            CsrUint16            obexPacketLength)
{
    FtcInstanceData * pInst     = instData;
    CsrBtFtcDisconnectInd *pMsg = (CsrBtFtcDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtFtcDisconnectInd));
    pMsg->type                  = CSR_BT_FTC_DISCONNECT_IND;
    pMsg->reasonCode            = reasonCode;
    pMsg->reasonSupplier        = reasonSupplier;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);    
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcAbortResultHandler(void                  *instData,
                                       CsrBtObexResponseCode responseCode,
                                       CsrUint8            *obexPacket,
                                       CsrUint16              obexPacketLength)
{
    FtcInstanceData * pInst = instData;
    CsrBtFtcAbortCfm *pMsg  = (CsrBtFtcAbortCfm*) CsrPmemAlloc(sizeof(CsrBtFtcAbortCfm));
    pMsg->type              = CSR_BT_FTC_ABORT_CFM;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcPutObjCommonResultHandler(FtcInstanceData       *pInst, 
                                              CsrBtObexResponseCode responseCode, 
                                              CsrUint16              bodySize)
{
    if (pInst->headerCfm)
    {
        CsrBtFtcPutObjHeaderCfm *pMsg = (CsrBtFtcPutObjHeaderCfm *) CsrPmemAlloc(sizeof(CsrBtFtcPutObjHeaderCfm));
        pMsg->type                    = CSR_BT_FTC_PUT_OBJ_HEADER_CFM;
        pMsg->responseCode            = responseCode;
        pMsg->bodySize                = bodySize;
        pInst->headerCfm              = FALSE;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
        
    }
    else
    {
        CsrBtFtcPutObjBodyCfm *pMsg   = (CsrBtFtcPutObjBodyCfm *) CsrPmemAlloc(sizeof(CsrBtFtcPutObjBodyCfm));
        pMsg->type                    = CSR_BT_FTC_PUT_OBJ_BODY_CFM;
        pMsg->responseCode            = responseCode;
        pMsg->bodySize                = bodySize;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtFtcPutObjResultHandler(void                  *instData,
                                        CsrBtObexResponseCode responseCode, 
                                        CsrUint8            *obexPacket,
                                        CsrUint16              obexPacketLength)
{
    CsrBool          finalFlag;
    FtcInstanceData * pInst = instData;
    responseCode            = csrBtFtcResponseCodeConverter(responseCode, &finalFlag);
    csrBtFtcPutObjCommonResultHandler(pInst, responseCode, obexPacketLength);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcPutObjContinueHandler(void  *    instData,
                                          CsrUint16   obexPacketLength)
{
    FtcInstanceData * pInst = instData;
    csrBtFtcPutObjCommonResultHandler(pInst, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, obexPacketLength);
}

static void csrBtFtcGetListFolderResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode, 
                                               CsrBool                bodyHeader,
                                               CsrUint16              bodyLength,
                                               CsrUint16              bodyOffset,
                                               CsrUint8            *obexPacket,
                                               CsrUint16              obexPacketLength)
{
    CsrBool          finalFlag;
    FtcInstanceData * pInst = instData;
    
    responseCode  = csrBtFtcResponseCodeConverter(responseCode, &finalFlag);

    if (pInst->headerCfm)
    {
        CsrBtFtcGetListFolderCfm *pMsg = (CsrBtFtcGetListFolderCfm *) CsrPmemAlloc(sizeof(CsrBtFtcGetObjCfm));
        pInst->headerCfm          = FALSE;
        pMsg->type                = CSR_BT_FTC_GET_LIST_FOLDER_CFM;
        pMsg->finalFlag           = finalFlag;
        pMsg->responseCode        = responseCode;
        pMsg->lengthOfObject      = csrBtFtcGetLengthOfObject(obexPacket);
        pMsg->bodyLength          = bodyLength; 
        pMsg->bodyOffset          = bodyOffset;  
        pMsg->payload             = obexPacket;
        pMsg->payloadLength       = obexPacketLength;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
    }
    else
    {
        CsrBtFtcGetListFolderBodyCfm *pMsg = (CsrBtFtcGetListFolderBodyCfm *) CsrPmemAlloc(sizeof(CsrBtFtcGetObjCfm));
        pMsg->type                    = CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM;
        pMsg->finalFlag               = finalFlag;
        pMsg->responseCode            = responseCode;
        pMsg->bodyLength              = bodyLength; 
        pMsg->bodyOffset              = bodyOffset;  
        pMsg->payload                 = obexPacket;
        pMsg->payloadLength           = obexPacketLength;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtFtcGetObjResultHandler(void                  *instData,
                                        CsrBtObexResponseCode responseCode,
                                        CsrBool                bodyHeader,
                                        CsrUint16              bodyLength,
                                        CsrUint16              bodyOffset,
                                        CsrUint8            *obexPacket,
                                        CsrUint16              obexPacketLength)
{
    CsrBool          finalFlag;
    FtcInstanceData * pInst = instData;
    
    responseCode  = csrBtFtcResponseCodeConverter(responseCode, &finalFlag);

    if (pInst->headerCfm)
    {
        CsrBtFtcGetObjCfm *pMsg      = (CsrBtFtcGetObjCfm *) CsrPmemAlloc(sizeof(CsrBtFtcGetObjCfm));
        pInst->headerCfm        = FALSE;
        pMsg->type              = CSR_BT_FTC_GET_OBJ_CFM;
        pMsg->finalFlag         = finalFlag;
        pMsg->responseCode      = responseCode;
        pMsg->lengthOfObject    = csrBtFtcGetLengthOfObject(obexPacket);
        pMsg->bodyLength        = bodyLength; 
        pMsg->bodyOffset        = bodyOffset;  
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
        
    }
    else
    {
        CsrBtFtcGetObjBodyCfm *pMsg  = (CsrBtFtcGetObjBodyCfm *) CsrPmemAlloc(sizeof(CsrBtFtcGetObjBodyCfm));
        pMsg->type              = CSR_BT_FTC_GET_OBJ_BODY_CFM;
        pMsg->finalFlag         = finalFlag;
        pMsg->responseCode      = responseCode;
        pMsg->bodyLength        = bodyLength; 
        pMsg->bodyOffset        = bodyOffset;  
        pMsg->payload           = obexPacket;
        pMsg->payloadLength     = obexPacketLength;
        csrBtFtcMessagePut(pInst->appHandle, pMsg);
    }
}

static void csrBtFtcDelObjResultHandler(void                  *instData,
                                        CsrBtObexResponseCode responseCode,
                                        CsrUint8            *obexPacket,
                                        CsrUint16              obexPacketLength)
{
    FtcInstanceData * pInst = instData;
    CsrBtFtcDelObjCfm *pMsg = (CsrBtFtcDelObjCfm *) CsrPmemAlloc(sizeof(CsrBtFtcDelObjCfm));
    pMsg->type              = CSR_BT_FTC_DEL_OBJ_CFM;
    pMsg->responseCode      = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcSetFolderResultHandler(void                  *instData,
                                           CsrBtObexResponseCode responseCode,
                                           CsrUint8            *obexPacket,
                                           CsrUint16              obexPacketLength)
{
    FtcInstanceData * pInst    = instData;
    CsrBtFtcSetFolderCfm *pMsg = (CsrBtFtcSetFolderCfm *) CsrPmemAlloc(sizeof(CsrBtFtcSetFolderCfm));
    pMsg->type                 = CSR_BT_FTC_SET_FOLDER_CFM;
    pMsg->responseCode         = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcSetBackFolderResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrUint8            *obexPacket,
                                               CsrUint16              obexPacketLength)
{
    FtcInstanceData * pInst        = instData;
    CsrBtFtcSetBackFolderCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtFtcSetBackFolderCfm));
    pMsg->type                     = CSR_BT_FTC_SET_BACK_FOLDER_CFM;
    pMsg->responseCode             = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcSetRootFolderResultHandler(void                  *instData,
                                               CsrBtObexResponseCode responseCode,
                                               CsrUint8            *obexPacket,
                                               CsrUint16             obexPacketLength)
{
    FtcInstanceData * pInst        = instData;
    CsrBtFtcSetRootFolderCfm *pMsg = (CsrBtFtcSetRootFolderCfm *) CsrPmemAlloc(sizeof(CsrBtFtcSetRootFolderCfm));
    pMsg->type                     = CSR_BT_FTC_SET_ROOT_FOLDER_CFM;
    pMsg->responseCode             = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);

    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcSetAddFolderResultHandler(void                  *instData,
                                              CsrBtObexResponseCode responseCode,
                                              CsrUint8            *obexPacket,
                                              CsrUint16             obexPacketLength)
{
    FtcInstanceData * pInst       = instData;
    CsrBtFtcSetAddFolderCfm *pMsg = (CsrBtFtcSetAddFolderCfm *) CsrPmemAlloc(sizeof(CsrBtFtcSetAddFolderCfm));
    pMsg->type                    = CSR_BT_FTC_SET_ADD_FOLDER_CFM;
    pMsg->responseCode            = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
static void csrBtFtcCopyingObjResultHandler(void                  *instData,
                                            CsrBtObexResponseCode responseCode,
                                            CsrUint8            *obexPacket,
                                            CsrUint16             obexPacketLength)
{
    FtcInstanceData * pInst       = instData;
    CsrBtFtcCopyingObjCfm *pMsg   = (CsrBtFtcCopyingObjCfm *) CsrPmemAlloc(sizeof(CsrBtFtcCopyingObjCfm));
    pMsg->type                    = CSR_BT_FTC_COPYING_OBJ_CFM;
    pMsg->responseCode            = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcMovingObjResultHandler(void                  *instData,
                                           CsrBtObexResponseCode responseCode,
                                           CsrUint8            *obexPacket,
                                           CsrUint16             obexPacketLength)
{
    FtcInstanceData * pInst     = instData;
    CsrBtFtcMovingObjCfm *pMsg  = (CsrBtFtcMovingObjCfm *) CsrPmemAlloc(sizeof(CsrBtFtcMovingObjCfm));
    pMsg->type                  = CSR_BT_FTC_MOVING_OBJ_CFM;
    pMsg->responseCode          = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtFtcSetObjPermissionsResultHandler(void                  *instData,
                                                   CsrBtObexResponseCode responseCode,
                                                   CsrUint8            *obexPacket,
                                                   CsrUint16             obexPacketLength)
{
    FtcInstanceData * pInst             = instData;
    CsrBtFtcSetObjPermissionsCfm *pMsg  = (CsrBtFtcSetObjPermissionsCfm *) CsrPmemAlloc(sizeof(CsrBtFtcSetObjPermissionsCfm));
    pMsg->type                          = CSR_BT_FTC_SET_OBJ_PERMISSIONS_CFM;
    pMsg->responseCode                  = responseCode;
    csrBtFtcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}
#endif 

CsrUint8 CsrBtFtcConnectReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint16  index;
    CsrUint8   numOfHeaders                  = 0;
    CsrUint8   tempTarget[FTC_TARGET_LENGTH] = FTC_TARGET;
    CsrUint8   headerPriorityTable[2]        = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
    CsrBtFtcConnectReq *pMsg                 = (CsrBtFtcConnectReq *) msg;
    CmnCsrBtLinkedListStruct * sdpTag        = NULL;
    CsrUint8 *target                         = (CsrUint8 *) CsrPmemAlloc(FTC_TARGET_LENGTH);
        
    ObexUtilDigestChallengeType * chal       = ObexUtilReturnDigestChallengePointer(pMsg->authorize,
                                                                                    &pMsg->password,
                                                                                    pMsg->passwordLength,
                                                                                    &pMsg->userId,
                                                                                    &pMsg->realm,
                                                                                    pMsg->realmLength);
    pInst->appHandle                    = pMsg->appHandle;
    pInst->localMaxPacketSize = pMsg->maxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    CsrMemCpy(target, tempTarget, FTC_TARGET_LENGTH);
    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_FILE_TRANSFER_UUID, &index);
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_OBEX_L2CAP_PSM_ATTRIBUTE, NULL, 0);
#endif 
    CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

    if (pMsg->length > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1); 
    }

    if (pMsg->count > 0)
    {
        numOfHeaders = (CsrUint8)(numOfHeaders + 1);
    }

    return(ObexUtilConnectRequest(pInst->obexInst, 
                                  &sdpTag, 
                                  pMsg->destination, 
                                  pInst->secOutgoing, 
                                  pInst->localMaxPacketSize,
                                  CSR_BT_FTC_PROFILE_DEFAULT_MTU_SIZE, 
                                  &target, 
                                  FTC_TARGET_LENGTH, 
                                  numOfHeaders, 
                                  headerPriorityTable, 
                                  CSR_BT_FTC_LP_SUPERVISION_TIMEOUT, 
                                  &chal, 
                                  pMsg->length, 
                                  pMsg->count, 
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtFtcConnectResultHandler, 
                                  csrBtFtcAuthenticateIndHandler, 
                                  csrBtFtcDisconnectResultHandler, 
                                  NULL,
                                  pInst->windowSize,
                                  pInst->localSrmEnable));
}

CsrUint8 CsrBtFtcAuthenticateResHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtFtcAuthenticateRes *pMsg = (CsrBtFtcAuthenticateRes *) msg;

    return(ObexUtilCliAuthenticateResponse(pInst->obexInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId, NULL));
}

CsrUint8 CsrBtFtcGetListFolderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                        CSR_BT_OBEX_UTIL_NAME_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                        CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcGetListFolderReq *pMsg       = (CsrBtFtcGetListFolderReq *) msg;
    
    pInst->headerCfm = TRUE;
    return(ObexUtilGetRequest(pInst->obexInst, 
                              headerPriorityTable, 
                              (const CsrUint8*)("x-obex/folder-listing"), 
                              &pMsg->ucs2name, 
                              NULL,
                              0, 
                              NULL, 
                              0, 
                              NULL, 
                              ObexUtilReturnSrmpValue(pMsg->srmpOn),
                              csrBtFtcGetListFolderResultHandler, 
                              NULL));
}

CsrUint8 CsrBtFtcGetListFolderBodyReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtFtcGetListFolderBodyReq *pMsg = (CsrBtFtcGetListFolderBodyReq *) msg;
    return(ObexUtilGetContinueRequest(pInst->obexInst, 
                                      ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtcGetObjReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcGetObjReq *pMsg          = (CsrBtFtcGetObjReq *) msg;
    
    pInst->headerCfm = TRUE;
    return(ObexUtilGetRequest(pInst->obexInst, 
                              headerPriorityTable, 
                              NULL, 
                              &pMsg->ucs2name, 
                              NULL, 
                              0, 
                              NULL, 
                              0, 
                              NULL, 
                              ObexUtilReturnSrmpValue(pMsg->srmpOn), 
                              csrBtFtcGetObjResultHandler, 
                              NULL));
}

CsrUint8 CsrBtFtcGetObjBodyReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtFtcGetObjBodyReq *pMsg = (CsrBtFtcGetObjBodyReq *) msg;
    return(ObexUtilGetContinueRequest(pInst->obexInst, 
                                      ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}

CsrUint8 CsrBtFtcPutObjHeaderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcPutObjHeaderReq *pMsg    = (CsrBtFtcPutObjHeaderReq *) msg;
    pInst->headerCfm                 = TRUE;

    if (pMsg->lengthOfObject == 0)
    {
        headerPriorityTable[1] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }
    return(ObexUtilPutRequest(pInst->obexInst, headerPriorityTable, pMsg->lengthOfObject, NULL, &pMsg->ucs2name, NULL, 0, NULL, 0, NULL, csrBtFtcPutObjContinueHandler, csrBtFtcPutObjResultHandler));
}

CsrUint8 CsrBtFtcPutObjBodyReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtFtcPutObjBodyReq *pMsg          = (CsrBtFtcPutObjBodyReq *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexInst, pMsg->finalFlag, &pMsg->body, pMsg->bodyLength));
}

CsrUint8 CsrBtFtcDelObjReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcDelObjReq *pMsg              = (CsrBtFtcDelObjReq *) msg;

    return(ObexUtilPutRequest(pInst->obexInst, headerPriorityTable, 0, NULL, &pMsg->ucs2name, NULL, 0, NULL, 0, NULL, NULL, csrBtFtcDelObjResultHandler));
}

CsrUint8 CsrBtFtcSetFolderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcSetFolderReq *pMsg           = (CsrBtFtcSetFolderReq *) msg;

    return (ObexUtilSetPathRequest(pInst->obexInst, 0x02, headerPriorityTable, &pMsg->ucs2name, NULL, 0, NULL, csrBtFtcSetFolderResultHandler));
}

CsrUint8 CsrBtFtcSetBackFolderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CSR_UNUSED(msg);
    return (ObexUtilSetPathRequest(pInst->obexInst, 0x03, headerPriorityTable, NULL, NULL, 0, NULL, csrBtFtcSetBackFolderResultHandler));
}

CsrUint8 CsrBtFtcSetRootFolderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_EMPTY_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CSR_UNUSED(msg);
    return (ObexUtilSetPathRequest(pInst->obexInst, 0x02, headerPriorityTable, NULL, NULL, 0, NULL, csrBtFtcSetRootFolderResultHandler));
}

CsrUint8 CsrBtFtcSetAddFolderReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[3]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcSetAddFolderReq *pMsg        = (CsrBtFtcSetAddFolderReq *) msg;

    return (ObexUtilSetPathRequest(pInst->obexInst, 0x00, headerPriorityTable, &pMsg->ucs2name, NULL, 0, NULL, csrBtFtcSetAddFolderResultHandler));
}

CsrUint8 CsrBtFtcAbortReqHandler(FtcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);

    return(ObexUtilAbortRequest(pInst->obexInst, NULL, csrBtFtcAbortResultHandler));
}

CsrUint8 CsrBtFtcDisconnectReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtFtcDisconnectReq *pMsg = (CsrBtFtcDisconnectReq *) msg;

    return (ObexUtilDisconnectRequest(pInst->obexInst, pMsg->normalDisconnect, NULL));
}

CsrUint8 CsrBtFtcCancelConnectReqHandler(FtcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexInst, FALSE, NULL));
}

static void csrBtFtcSecurityOutCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtFtcSecurityOutCfm *prim = (CsrBtFtcSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtFtcSecurityOutCfm));
    prim->type              = CSR_BT_FTC_SECURITY_OUT_CFM;
    prim->resultCode        = resultCode;
    prim->resultSupplier    = resultSupplier;
    csrBtFtcMessagePut(appHandle, prim);
}

CsrUint8 CsrBtFtcSecurityOutReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrBtResultCode rval;
    CsrBtFtcSecurityOutReq *pMsg;

    pMsg = (CsrBtFtcSecurityOutReq*)msg;
    
    rval = CsrBtScSetSecOutLevel(&pInst->secOutgoing, pMsg->secLevel,
        CSR_BT_OBEX_FILE_TRANSFER_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_FILE_TRANSFER_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtFtcSecurityOutCfmSend(pMsg->appHandle, rval,
        CSR_BT_SUPPLIER_OBEX_PROFILES);

    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
CsrUint8 CsrBtFtcCopyingObjReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[2]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcCopyingObjReq *pMsg      = (CsrBtFtcCopyingObjReq *) msg;

    return (ObexUtilActionRequest(pInst->obexInst,
                                  CSR_BT_OBEX_UTIL_ACTION_ID_COPY_OBJECT,
                                  headerPriorityTable,
                                  &pMsg->ucs2SrcName,
                                  &pMsg->ucs2DestName,
                                  CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION,
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtFtcCopyingObjResultHandler));
}

CsrUint8 CsrBtFtcMovingObjReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[2]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcMovingObjReq *pMsg       = (CsrBtFtcMovingObjReq *) msg;

    return (ObexUtilActionRequest(pInst->obexInst,
                                  CSR_BT_OBEX_UTIL_ACTION_ID_MOVE_RENAME_OBJECT,
                                  headerPriorityTable,
                                  &pMsg->ucs2SrcName,
                                  &pMsg->ucs2DestName,
                                  CSR_BT_OBEX_UTIL_ACTION_ALL_RESET_PERMISSION,
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtFtcMovingObjResultHandler));
}

CsrUint8 CsrBtFtcSetObjPermissionsReqHandler(FtcInstanceData *pInst, void *msg)
{
    CsrUint8 headerPriorityTable[2]    = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
    CsrBtFtcSetObjPermissionsReq *pMsg = (CsrBtFtcSetObjPermissionsReq *) msg;

    return (ObexUtilActionRequest(pInst->obexInst,
                                  CSR_BT_OBEX_UTIL_ACTION_ID_SET_OBJECT_PERMISSIONS,
                                  headerPriorityTable,
                                  &pMsg->ucs2name,
                                  NULL,
                                  pMsg->permissions,
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtFtcSetObjPermissionsResultHandler));
}
#endif

