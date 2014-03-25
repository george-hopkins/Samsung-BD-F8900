/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_opc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"

#define TYPESTR_VCARD               "text/x-vcard"
#define TYPESTR_VCAL                "text/x-vcalendar"
#define TYPESTR_VNOTE               "text/x-vnote"
#define TYPESTR_VMSG                "text/x-vmessage"

static CsrUint16 csrBtOpcExtractSupportedFormats(CmnCsrBtLinkedListStruct * sdpTag)
{
    CsrUint16 supportedFormats = CSR_BT_NO_TYPE_SUPPORT;

    if (sdpTag)
    {
        CsrBtUuid32    service;
        CsrUint16    bchsResult, bchsTask, bchsTaskResult;

        if (CsrBtUtilSdrGetServiceUuid32AndResult(sdpTag, 0, &service,
                            &bchsResult, &bchsTask, &bchsTaskResult))
        {
            if (bchsResult == SDR_SDC_SEARCH_SUCCESS)
            {
                CsrUint32 dataLen;
                CsrUint8  *data;

                if (CsrBtUtilSdrGetDataElementSequenceDataAttributeFromAttributeUuid(sdpTag, 0,
                             CSR_BT_SUPPORTED_FORMATS_LIST_ATTRIBUTE_IDENTIFIER, &data, &dataLen))
                {
                    CsrUintFast32 i;

                    for (i = 1; i < dataLen; i+=2)
                    {
                        if (data[i] == 0xFF)
                        {
                            supportedFormats |= CSR_BT_OTHER_TYPE_SUPPORT;
                        }
                        else
                        {
                            supportedFormats |= (1 << ((data[i] & 0x0F) - 1));
                        }
                    }
                }
            }
        }
    }
    return (supportedFormats);
}

#ifdef CSR_BT_INSTALL_OPC_GET
static const CsrUint8 * csrBtOpcGetType(CsrUint8 bodyType)
{
    switch (bodyType)
    {
        case CSR_BT_VCARD_TYPE:
        {
            return (CsrUint8 *) (TYPESTR_VCARD);
        }
        case CSR_BT_VCAL_TYPE:
        {
            return (CsrUint8 *) (TYPESTR_VCAL);
        }
        case CSR_BT_VNOTE_TYPE:
        {
            return (CsrUint8 *) (TYPESTR_VNOTE);
        }
        case CSR_BT_VMESSAGE_TYPE:
        {
            return (CsrUint8 *) (TYPESTR_VMSG);
        }
        default:
        {
            return NULL;
        }
    }
}

static CsrBool csrBtOpcGetHeaderResultHandler(OpcInstanceDataType    *pInst,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrBool                 bodyHeader,
                                             CsrUint16               obexPacketLength,
                                             CsrUint8             *obexPacket,
                                             CsrUint16               *ucs2nameOffset)
{
    *ucs2nameOffset = 0;

    if (obexPacket)
    {
        CsrBool nameHeader = CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, ucs2nameOffset);

        if (pInst->lengthOfObject == 0)
        {
            CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &pInst->lengthOfObject);
        }

        if (!nameHeader && !bodyHeader && responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            return (FALSE);
        }
        else
        {
            return (TRUE);
        }
    }
    else
    {
        return (TRUE);
    }
}
#endif

static void csrBtOpcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_OPC_PRIM, msg);
}

static void csrBtOpcConnectResultHandler(void                     *instData,
                                         CsrBtResultCode          resultCode,
                                         CsrBtSupplier            resultSupplier,
                                         CsrBtDeviceAddr          deviceAddr,
                                         CsrBtConnId              cid,
                                         CsrUint16                maxPeerObexPacketLength,
                                         CmnCsrBtLinkedListStruct *sdpTag,
                                         CsrUint16                obexPacketLength,
                                         CsrUint8               *obexPacket)
{
    OpcInstanceDataType * pInst = instData;
    CsrBtOpcConnectCfm  * pMsg  = (CsrBtOpcConnectCfm *) CsrPmemAlloc(sizeof(CsrBtOpcConnectCfm));
    pMsg->type                  = CSR_BT_OPC_CONNECT_CFM;
    pMsg->resultCode            = resultCode;
    pMsg->resultSupplier        = resultSupplier;
    pMsg->maxPeerPacketSize     = maxPeerObexPacketLength;
    pMsg->supportedFormats      = csrBtOpcExtractSupportedFormats(sdpTag);
    pMsg->btConnId              = cid;

    csrBtOpcMessagePut(pInst->appHandle, pMsg);
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}
#ifdef CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
static void csrBtOpcAuthenticateIndHandler(void             *instData,
                                           CsrBtDeviceAddr  deviceAddr,
                                           CsrUint8         options,
                                           CsrUint16        realmLength,
                                           CsrUint8       *realm)
{
    OpcInstanceDataType * pInst = instData;

    CsrBtOpcAuthenticateInd *pMsg = (CsrBtOpcAuthenticateInd *) CsrPmemAlloc(sizeof(CsrBtOpcAuthenticateInd));

    pMsg->type              = CSR_BT_OPC_AUTHENTICATE_IND;
    pMsg->deviceAddr        = deviceAddr;
    pMsg->options           = options;
    pMsg->realmLength       = realmLength;
    pMsg->realm             = realm;
    csrBtOpcMessagePut(pInst->appHandle, pMsg);
}
#else
#define csrBtOpcAuthenticateIndHandler NULL
#endif /* CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */

static void csrBtOpcDisconnectResultHandler(void                *instData,
                                            CsrBtReasonCode     reasonCode,
                                            CsrBtSupplier reasonSupplier,
                                            CsrUint8          *obexPacket,
                                            CsrUint16            obexPacketLength)
{
    OpcInstanceDataType * pInst  = instData;
    CsrBtOpcDisconnectInd * pMsg = (CsrBtOpcDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtOpcDisconnectInd));
    pMsg->type                   = CSR_BT_OPC_DISCONNECT_IND;
    pMsg->reasonCode             = reasonCode; 
    pMsg->reasonSupplier         = reasonSupplier;
    csrBtOpcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtOpcAbortResultHandler(void                   *instData,
                                       CsrBtObexResponseCode  responseCode,
                                       CsrUint8             *obexPacket,
                                       CsrUint16               obexPacketLength)
{
    OpcInstanceDataType * pInst = instData;
    CsrBtOpcAbortCfm    * pMsg  = (CsrBtOpcAbortCfm*) CsrPmemAlloc(sizeof(CsrBtOpcAbortCfm));
    pMsg->type                  = CSR_BT_OPC_ABORT_CFM;
    csrBtOpcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtOpcPutResultHandler(void                   *instData,
                                     CsrBtObexResponseCode  responseCode,
                                     CsrUint8             *obexPacket,
                                     CsrUint16               obexPacketLength)
{
    OpcInstanceDataType * pInst = instData;
    CsrBtOpcPutCfm * pMsg       = (CsrBtOpcPutCfm *) CsrPmemAlloc(sizeof(CsrBtOpcPutCfm));
    pMsg->type                  = CSR_BT_OPC_PUT_CFM;
    pMsg->responseCode          = responseCode;
    csrBtOpcMessagePut(pInst->appHandle, pMsg);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtOpcPutContinueHandler(void     *instData,
                                       CsrUint16 obexPacketLength)
{
    OpcInstanceDataType * pInst = instData;
    CsrBtOpcPutObjectInd *pMsg  = (CsrBtOpcPutObjectInd *) CsrPmemAlloc(sizeof(CsrBtOpcPutObjectInd));
    pMsg->type                  = CSR_BT_OPC_PUT_OBJECT_IND;
    pMsg->objectLength          = obexPacketLength;
    csrBtOpcMessagePut(pInst->appHandle, pMsg);
}

#ifdef CSR_BT_INSTALL_OPC_GET
static void csrBtOpcGetResultHandler(void                    *instData,
                                     CsrBtObexResponseCode   responseCode,
                                     CsrBool                 bodyHeader,
                                     CsrUint16               bodyLength,
                                     CsrUint16               bodyOffset,
                                     CsrUint8              *obexPacket,
                                     CsrUint16               obexPacketLength)
{
    OpcInstanceDataType * pInst = instData;
    
    if (pInst->headerCfm)
    {
        CsrUint16 ucs2nameOffset;

        if (csrBtOpcGetHeaderResultHandler(pInst, responseCode, bodyHeader, obexPacketLength, obexPacket, &ucs2nameOffset))
        {
            CsrBtOpcGetHeaderCfm *pMsg  = (CsrBtOpcGetHeaderCfm *) CsrPmemAlloc(sizeof(CsrBtOpcGetHeaderCfm));
            pInst->headerCfm            = FALSE;
            pMsg->type                  = CSR_BT_OPC_GET_HEADER_CFM;
            pMsg->responseCode          = responseCode;
            pMsg->totalObjectSize       = pInst->lengthOfObject;
            pMsg->objectBodyOffset      = bodyOffset;
            pMsg->objectBodyLength      = bodyLength;
            pMsg->ucs2nameOffset        = ucs2nameOffset;
            pMsg->payloadLength         = obexPacketLength;
            pMsg->payload               = obexPacket;
            csrBtOpcMessagePut(pInst->appHandle, pMsg);
        }
        else
        { /* In order to make sure that the application has the possibility to
             use SRMP later set SRMP to CSR_BT_OBEX_SRMP_WAIT   */
            ObexUtilGetContinueRequest(pInst->obexInst, CSR_BT_OBEX_SRMP_WAIT);
            CsrPmemFree(obexPacket);
        }
    }
    else
    {
        CsrBtOpcGetObjectCfm *pMsg  = (CsrBtOpcGetObjectCfm *) CsrPmemAlloc(sizeof(CsrBtOpcGetObjectCfm));
        pMsg->type                  = CSR_BT_OPC_GET_OBJECT_CFM;
        pMsg->responseCode          = responseCode;
        pMsg->objectBodyLength      = bodyLength;
        pMsg->objectBodyOffset      = bodyOffset;
        pMsg->payloadLength         = obexPacketLength;
        pMsg->payload               = obexPacket;
        csrBtOpcMessagePut(pInst->appHandle, pMsg);
    }
}
#endif
CsrUint8 CsrBtOpcConnectReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrUint16  index;
    CsrUint8   headerPriorityTable[2]   = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
    CsrUint8   numOfHeaders             = 0;
    CsrBtOpcConnectReq          *pMsg   = (CsrBtOpcConnectReq *) msg;
    CmnCsrBtLinkedListStruct    *sdpTag = NULL;
#ifdef CSR_BT_INSTALL_OBEX_HEADER_AUTH_CHALLENGE
    ObexUtilDigestChallengeType *chal   = NULL;
#endif

    pInst->appHandle                    = pMsg->appHandle;
    pInst->localMaxPacketSize = pMsg->maxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_OBEX_OBJECT_PUSH_SERVICE_UUID, &index);
    CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_SUPPORTED_FORMATS_LIST_ATTRIBUTE_IDENTIFIER, NULL, 0);
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

#ifdef CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    if (pMsg->password)
    { /* The application want to has challenge the peer server      */
        chal = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                    &pMsg->password, 
                                                    pMsg->passwordLength,  
                                                    &pMsg->userId, 
                                                    &pMsg->realm, 
                                                    pMsg->realmLength);
    }
    else
#endif /* CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE   */
    { /* Pfree pointers to prevent a memory leak                    */    
        CsrPmemFree(pMsg->password);
        CsrPmemFree(pMsg->userId);
        CsrPmemFree(pMsg->realm);
    }

    return(ObexUtilConnectRequest(pInst->obexInst, 
                                  &sdpTag, 
                                  pMsg->deviceAddr, 
                                  pInst->secOutgoing, 
                                  pMsg->maxPacketSize, 
                                  CSR_BT_OPC_PROFILE_DEFAULT_MTU_SIZE, 
                                  NULL, 
                                  0, 
                                  numOfHeaders, 
                                  headerPriorityTable, 
                                  CSR_BT_OPC_LP_SUPERVISION_TIMEOUT, 
                                  &chal, 
                                  pMsg->length, 
                                  pMsg->count, 
                                  NULL,
                                  0,
                                  NULL,
                                  csrBtOpcConnectResultHandler, 
                                  csrBtOpcAuthenticateIndHandler, 
                                  csrBtOpcDisconnectResultHandler, 
                                  NULL,
                                  pInst->windowSize,
                                  pInst->localSrmEnable));
}

CsrUint8 CsrBtOpcDisconnectReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrBtOpcDisconnectReq *pMsg = (CsrBtOpcDisconnectReq *) msg;

    return (ObexUtilDisconnectRequest(pInst->obexInst, pMsg->normalDisconnect, NULL)); 
}

CsrUint8 CsrBtOpcCancelConnectReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return (ObexUtilDisconnectRequest(pInst->obexInst, FALSE, NULL));
}

CsrUint8 CsrBtOpcAbortReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CSR_UNUSED(msg);
    return(ObexUtilAbortRequest(pInst->obexInst, NULL, csrBtOpcAbortResultHandler));  
}

CsrUint8 CsrBtOpcPutHeaderReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrUint8     returnValue;

    CsrBtOpcPutReq   *pMsg = (CsrBtOpcPutReq *) msg;

    if (pMsg->bodyType)
    {
        CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
        returnValue = ObexUtilPutRequest(pInst->obexInst, headerPriorityTable, pMsg->lengthOfObject, pMsg->bodyType, &pMsg->ucs2name, NULL, 0, NULL, 0, NULL, csrBtOpcPutContinueHandler, csrBtOpcPutResultHandler);
        CsrPmemFree(pMsg->bodyType); 
    }
    else
    {
        CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_NAME_HEADER, CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
        returnValue = ObexUtilPutRequest(pInst->obexInst, headerPriorityTable, pMsg->lengthOfObject, NULL, &pMsg->ucs2name, NULL, 0, NULL, 0, NULL, csrBtOpcPutContinueHandler, csrBtOpcPutResultHandler);
    }
    return (returnValue);
}

CsrUint8 CsrBtOpcPutBodyReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrBtOpcPutObjectRes *pMsg = (CsrBtOpcPutObjectRes *) msg;

    return(ObexUtilPutContinueRequest(pInst->obexInst, pMsg->finalFlag, &pMsg->object, pMsg->objectLength));
}

#ifdef CSR_BT_INSTALL_OPC_GET
CsrUint8 CsrBtOpcGetHeaderReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrBtOpcGetHeaderReq *pMsg = (CsrBtOpcGetHeaderReq *) msg;
    const CsrUint8 *type   = csrBtOpcGetType(pMsg->bodyType);
    pInst->headerCfm      = TRUE;
    pInst->lengthOfObject = 0;  

    if (type)
    {
        CsrUint8 headerPriorityTable[5]  = {CSR_BT_OBEX_UTIL_TYPE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};        
        return(ObexUtilGetRequest(pInst->obexInst, 
                                  headerPriorityTable, 
                                  type, 
                                  NULL, 
                                  NULL, 
                                  0, 
                                  NULL, 
                                  0, 
                                  NULL,
                                  ObexUtilReturnSrmpValue(pMsg->srmpOn),
                                  csrBtOpcGetResultHandler, 
                                  NULL));
    }
    else
    {
        csrBtOpcGetResultHandler(pInst, (CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE),
                                 FALSE, 0, 0, NULL, 0);
        return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
    }    
}

CsrUint8 CsrBtOpcGetObjReqHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrBtOpcGetObjectReq *pMsg = (CsrBtOpcGetObjectReq *) msg;
    return(ObexUtilGetContinueRequest(pInst->obexInst, 
                                      ObexUtilReturnSrmpValue(pMsg->srmpOn)));
}
#endif

CsrUint8 CsrBtOpcSecurityOutReqHandler(OpcInstanceDataType * pInst, void *msg)
{
    CsrBtOpcSecurityOutCfm *prim;
    CsrBtOpcSecurityOutReq *pMsg;

    prim = (CsrBtOpcSecurityOutCfm*)CsrPmemAlloc(sizeof(CsrBtOpcSecurityOutCfm));
    pMsg = (CsrBtOpcSecurityOutReq*) msg;
    
    prim->type = CSR_BT_OPC_SECURITY_OUT_CFM;
    prim->resultCode = CsrBtScSetSecOutLevel(&pInst->secOutgoing, pMsg->secLevel,
        CSR_BT_OBEX_OBJECT_PUSH_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_OBJECT_PUSH_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    prim->resultSupplier = CSR_BT_SUPPLIER_OBEX_PROFILES;

    csrBtOpcMessagePut(pMsg->appHandle, prim);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

#ifdef CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
CsrUint8 CsrBtOpcAuthenticateResHandler(OpcInstanceDataType *pInst, void *msg)
{
    CsrBtOpcAuthenticateRes     *pMsg = (CsrBtOpcAuthenticateRes *) msg;
    ObexUtilDigestChallengeType *chal = NULL;
    
    if (pMsg->chalPassword)
    { /* The application want to has challenge the peer server      */
        chal = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                    &pMsg->chalPassword, 
                                                    pMsg->chalPasswordLength,  
                                                    &pMsg->chalUserId, 
                                                    &pMsg->chalRealm, 
                                                    pMsg->chalRealmLength);
    }
    else
    { /* Pfree pointers to prevent a memory leak                    */    
        CsrPmemFree(pMsg->chalUserId);
        CsrPmemFree(pMsg->chalRealm);
    }
    return(ObexUtilCliAuthenticateResponse(pInst->obexInst, 
                                           pMsg->authPasswordLength, 
                                           &pMsg->authPassword, 
                                           &pMsg->authUserId, 
                                           &chal));
}
#endif /* CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */ 

