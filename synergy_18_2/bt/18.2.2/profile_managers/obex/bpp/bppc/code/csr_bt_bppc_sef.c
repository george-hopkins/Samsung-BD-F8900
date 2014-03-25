/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bppc_handler.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_bppc_sef.h"
#include "csr_bt_bppc_soapbuilder.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */

#define CSR_BT_BPPC_SOAP_TYPE_HEADER                            "x-obex/bt-SOAP"
#define CSR_BT_BPPC_TARGET_LENGTH                               0x10
#define CSR_BT_BPPC_LENGTH_OF_OBJECT_LENGTH                     0x05
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_LENGTH               0x09
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_TAG_LENGTH           0x04
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_TOTAL_TAG_LENGTH     0x06
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_JOBID_TAG            0x03
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_FILE_SIZE_TAG        0x04
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_OFFSET_TAG           0x01
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_COUNT_TAG            0x02
#define CSR_BT_BPPC_APPLICATION_PARAMETERS_TAG_HEADER_LENGTH    0x02

#define CSR_BT_BPPC_APPLICATION_PARAMETERS_INVALID_JOBID        0x00000000
#define CSR_BT_BPPC_OPERATION_STATUS_NOT_FOUND                  0xFFFF
#define CSR_BT_BPPC_RFCOMM_OBJECT_SERVER_CHANNEL_INDEX          0x18
#define CSR_BT_BPPC_OBJECT_CHANNEL_GET_BODY_HEADER_INDEX        0x01
#define CSR_BT_BPPC_OBJECT_CHANNEL_INVALID_COUNT_VALUE          (-2)

#define CSR_BT_BPPC_OBJECT_CHANNEL_SECURITY                     (CSR_BT_SEC_SPECIFY | \
                                                                 CSR_BT_SEC_AUTHENTICATION | \
                                                                 CSR_BT_SEC_ENCRYPTION)


#define CSR_BT_BPPC_OPERATION_STATUS_START_STRING_LENGTH        0x11
#define CSR_BT_BPPC_OPERATION_STATUS_END_STRING_LENGTH          0x12


#define CSR_BT_BPPC_DPS_TARGET  {0x00, 0x00, 0x11, 0x18, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB}
#define CSR_BT_BPPC_STS_TARGET  {0x00, 0x00, 0x11, 0x23, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB}

static const CsrUint8 CSR_BT_BPPC_REJ_OBJ_TARGET[CSR_BT_BPPC_TARGET_LENGTH] = {0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};

static void csrBtBppcGetStatusChSoapInd(void       *instData,
                                        CsrUint16  obexPacketLength);

static void csrBtBppcGetJobChSoapInd(void       *instData,
                                     CsrUint16  obexPacketLength);

static CsrUint8 csrBtBppcActivateObjectChannel(BppcInstanceData *pInst);

#ifdef CSR_AMP_ENABLE
static void updateAmpm(BppcInstanceData *pInst);
#endif /* CSR_AMP_ENABLE */

static const CsrUint8 csrBtBppcReferenceObjectsServiceRecord[] =
{
    /* service class ID list */
    0x09,0x00,0x01,                     /* AttrID , ServiceClassIDList */
    0x35,0x03,                          /* 6 bytes in total DataElSeq */
    0x19,0x11,0x20,                     /* Service class = DirectPrintingReferenceObjectsService */

    /* protocol descriptor list */
    0x09,0x00,0x04,                     /* AttrId ProtocolDescriptorList */
    0x35,0x11,                          /* 17 bytes in total DataElSeq */
    0x35,0x03,                          /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,                     /* 2 byte UUID, Protocol = L2CAP */
    0x35,0x05,                          /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,                     /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,                          /* 1 byte UINT - server channel */
    0x35,0x03,                          /* 3 bytes in DataElSeq */
    0x19,0x00,0x08,                     /* 2 byte UUID, Protocol = OBEX */

    /* BrowseGroupList */
    0x09, 0x00, 0x05,                  /* AttrId = BrowseGroupList */
    0x35, 0x03,                        /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,                  /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};


/* Static upstream message library functions */
static void csrBtBppcMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BPPC_PRIM, msg);
}

static void csrBtBppcConnectCfmSend(CsrSchedQid          appHandle,
                                    CsrBtResultCode resultCode,
                                    CsrBtSupplier   resultSupplier,
                                    CsrUint8        colorSupported,
                                    CsrUint8        duplexSupported,
                                    CsrUint16       maxPeerPacketSize,
                                    CsrUint16       maxMediaWidth,
                                    CsrUint16       maxMediaLength,
                                    CsrUint8        *characterRepertoires,
                                    CsrUtf8String   *xhtmlPrintImageFormats,
                                    CsrUtf8String   *documentFormatsSupported,
                                    CsrUtf8String   *mediaTypesSupported,
                                    CsrUtf8String   *printerModelId,
                                    CsrBtConnId     btConnId)
{
    CsrBtBppcConnectCfm *pMsg = (CsrBtBppcConnectCfm*)
        CsrPmemAlloc(sizeof(CsrBtBppcConnectCfm));

    pMsg->type                      = CSR_BT_BPPC_CONNECT_CFM;
    pMsg->resultCode                = resultCode;
    pMsg->resultSupplier            = resultSupplier;
    pMsg->colorSupported            = colorSupported;
    pMsg->duplexSupported           = duplexSupported;
    pMsg->maxPeerPacketSize         = maxPeerPacketSize;
    pMsg->maxMediaWidth             = maxMediaWidth;
    pMsg->maxMediaLength            = maxMediaLength;
    pMsg->xhtmlPrintImageFormats    = xhtmlPrintImageFormats;
    pMsg->documentFormatsSupported  = documentFormatsSupported;
    pMsg->mediaTypesSupported       = mediaTypesSupported;
    pMsg->printerModelId            = printerModelId;
    pMsg->btConnId                  = btConnId;

    if (characterRepertoires)
    {
        CsrMemCpy(pMsg->characterRepertoires, characterRepertoires, CSR_BT_CHARACTER_REPERTOIRES_LENGTH);
    }
    else
    {
        CsrMemSet(pMsg->characterRepertoires, '\0', CSR_BT_CHARACTER_REPERTOIRES_LENGTH);
    }
    csrBtBppcMessagePut(appHandle, pMsg);
}


static void csrBtBppcSecurityOutCfmSend(CsrSchedQid          appHandle,
                                        CsrBtResultCode resultCode)
{
    CsrBtBppcSecurityOutCfm *pMsg   = (CsrBtBppcSecurityOutCfm*)
        CsrPmemAlloc(sizeof(CsrBtBppcSecurityOutCfm));

    pMsg->type                      = CSR_BT_BPPC_SECURITY_OUT_CFM;
    pMsg->resultCode                = resultCode;
    pMsg->resultSupplier            = CSR_BT_SUPPLIER_OBEX_PROFILES;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetPrintAttrIndSend(CsrSchedQid     appHandle,
                                         CsrUint16  objectLength,
                                         CsrUint16  objectOffset,
                                         CsrUint8 *payload,
                                         CsrUint16  payloadLength)
{
    CsrBtBppcGetPrinterAttributesInd *pMsg = (CsrBtBppcGetPrinterAttributesInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetPrinterAttributesInd));

    pMsg->type                          = CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_IND;
    pMsg->printerAttributeObjectLength  = objectLength;
    pMsg->printerAttributeObjectOffset  = objectOffset;
    pMsg->payload                       = payload;
    pMsg->payloadLength                 = payloadLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetPrintAttrCfmSend(CsrSchedQid                 appHandle,
                                         CsrBtObexResponseCode  responseCode,
                                         CsrUint16              objectLength,
                                         CsrUint16              objectOffset,
                                         CsrUint8             *payload,
                                         CsrUint16              payloadLength)
{
    CsrBtBppcGetPrinterAttributesCfm *pMsg = (CsrBtBppcGetPrinterAttributesCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetPrinterAttributesCfm));

    pMsg->type                          = CSR_BT_BPPC_GET_PRINTER_ATTRIBUTES_CFM;
    pMsg->responseCode                  = responseCode;
    pMsg->printerAttributeObjectLength  = objectLength;
    pMsg->printerAttributeObjectOffset  = objectOffset;
    pMsg->payload                       = payload;
    pMsg->payloadLength                 = payloadLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppCreateJobCfmSend(CsrSchedQid                         appHandle,
                                     CsrBtObexResponseCode          responseCode,
                                     CsrUint32                      jobId,
                                     CsrBtObexOperationStatusCode   operationStatus)
{
    CsrBtBppcCreateJobCfm *pMsg = (CsrBtBppcCreateJobCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcCreateJobCfm));

    pMsg->type              = CSR_BT_BPPC_CREATE_JOB_CFM;
    pMsg->responseCode      = responseCode;
    pMsg->operationStatus   = operationStatus;
    pMsg->jobId             = jobId;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcSendDocumentIndSend(CsrSchedQid appHandle, CsrUint16 printContentLength)
{
    CsrBtBppcSendDocumentInd *pMsg = (CsrBtBppcSendDocumentInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcSendDocumentInd));

    pMsg->type                  = CSR_BT_BPPC_SEND_DOCUMENT_IND;
    pMsg->printContentLength    = printContentLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcSendDocumentCfmSend(CsrSchedQid appHandle, CsrBtObexResponseCode responseCode)
{
    CsrBtBppcSendDocumentCfm *pMsg = (CsrBtBppcSendDocumentCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcSendDocumentCfm));

    pMsg->type          = CSR_BT_BPPC_SEND_DOCUMENT_CFM;
    pMsg->responseCode  = responseCode;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcAuthenticateIndSend(CsrSchedQid             appHandle,
                                         CsrBtDeviceAddr    deviceAddr,
                                         CsrUint8           options,
                                         CsrUint16          realmLength,
                                         CsrUint8         *realm)
{
    CsrBtBppcAuthenticateInd *pMsg = (CsrBtBppcAuthenticateInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcAuthenticateInd));

    pMsg->type          = CSR_BT_BPPC_AUTHENTICATE_IND;
    pMsg->deviceAddr    = deviceAddr;
    pMsg->options       = options;
    pMsg->realmLength   = realmLength;
    pMsg->realm         = realm;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcDeactivateCfmSend(CsrSchedQid appHandle)
{
    CsrBtBppcDeactivateCfm *pMsg = (CsrBtBppcDeactivateCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcDeactivateCfm));

    pMsg->type = CSR_BT_BPPC_DEACTIVATE_CFM;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppConnectIndSend(CsrSchedQid           appHandle,
                                   CsrBtDeviceAddr  deviceAddr,
                                   CsrUint16        obexMaxPacketSize,
                                   CsrUint32        connectionId)
{
    CsrBtBppcConnectInd *pMsg = (CsrBtBppcConnectInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcConnectInd));

    pMsg->type                  = CSR_BT_BPPC_CONNECT_IND;
    pMsg->deviceAddr            = deviceAddr;
    pMsg->obexPeerMaxPacketSize = obexMaxPacketSize;
    pMsg->connectionId          = connectionId;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcDisconnectIndSend(CsrSchedQid           appHandle,
                                       CsrUint8         channelId,
                                       CsrBtReasonCode  reasonCode,
                                       CsrBtSupplier    reasonSupplier)
{
    CsrBtBppcDisconnectInd *pMsg = (CsrBtBppcDisconnectInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcDisconnectInd));

    pMsg->type              = CSR_BT_BPPC_DISCONNECT_IND;
    pMsg->channelId         = channelId;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    pMsg->connectionId      = 0;
    csrBtBppcMessagePut(appHandle, pMsg);
}


static void csrBtBppcAbortCfmSend(CsrSchedQid    appHandle,
                                  CsrUint8  channelId)
{
    CsrBtBppcAbortCfm *pMsg = (CsrBtBppcAbortCfm*)
        CsrPmemAlloc(sizeof(CsrBtBppcAbortCfm));

    pMsg->type              = CSR_BT_BPPC_ABORT_CFM;
    pMsg->channelId         = channelId;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetReferenceObjectIndSend(CsrSchedQid      appHandle,
                                               CsrUtf8String    *fileName,
                                               CsrInt32         count,
                                               CsrUint32        offset,
                                               CsrBool          lastPacket)
{
    CsrBtBppcGetReferenceObjectInd *pMsg = (CsrBtBppcGetReferenceObjectInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetReferenceObjectInd));

    pMsg->type              = CSR_BT_BPPC_GET_REFERENCE_OBJECT_IND;
    pMsg->fileName          = fileName;
    pMsg->count             = count;
    pMsg->offset            = offset;
    pMsg->lastPacket        = lastPacket;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetJobAttributesIndSend(CsrSchedQid       appHandle,
                                             CsrUint16    objectLength,
                                             CsrUint16    objectOffset,
                                             CsrUint8   *payload,
                                             CsrUint16    payloadLength)
{
    CsrBtBppcGetJobAttributesInd * pMsg = (CsrBtBppcGetJobAttributesInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetJobAttributesInd));

    pMsg->type                       = CSR_BT_BPPC_GET_JOB_ATTRIBUTES_IND;
    pMsg->jobAttributeObjectLength   = objectLength;
    pMsg->jobAttributeObjectOffset   = objectOffset;
    pMsg->payload                    = payload;
    pMsg->payloadLength              = payloadLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetJobAttributesCfmSend(CsrSchedQid                 appHandle,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrUint16              objectLength,
                                             CsrUint16              objectOffset,
                                             CsrUint8             *payload,
                                             CsrUint16              payloadLength)
{
    CsrBtBppcGetJobAttributesCfm * pMsg = (CsrBtBppcGetJobAttributesCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetJobAttributesCfm));

    pMsg->type                        = CSR_BT_BPPC_GET_JOB_ATTRIBUTES_CFM;
    pMsg->responseCode                = responseCode;
    pMsg->jobAttributeObjectLength    = objectLength;
    pMsg->jobAttributeObjectOffset    = objectOffset;
    pMsg->payload                     = payload;
    pMsg->payloadLength               = payloadLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppCancelJobCfmSend(CsrSchedQid                         appHandle,
                                     CsrBtObexResponseCode          responseCode,
                                     CsrUint32                      jobId,
                                     CsrBtObexOperationStatusCode   operationStatus)
{
    CsrBtBppcCancelJobCfm *pMsg = (CsrBtBppcCancelJobCfm *)
        CsrPmemAlloc(sizeof(CsrBtBppcCancelJobCfm));

    pMsg->type              = CSR_BT_BPPC_CANCEL_JOB_CFM;
    pMsg->responseCode      = responseCode;
    pMsg->operationStatus   = operationStatus;
    pMsg->jobId             = jobId;
    csrBtBppcMessagePut(appHandle, pMsg);
}

static void csrBtBppcGetEventIndSend(CsrSchedQid     appHandle,
                                     CsrUint16  objectLength,
                                     CsrUint16  objectOffset,
                                     CsrUint8 *payload,
                                     CsrUint16  payloadLength)
{
    CsrBtBppcGetEventInd  *pMsg = (CsrBtBppcGetEventInd *)
        CsrPmemAlloc(sizeof(CsrBtBppcGetEventInd));

    pMsg->type                  = CSR_BT_BPPC_GET_EVENT_IND;
    pMsg->eventObjectLength     = objectLength;
    pMsg->eventObjectOffset     = objectOffset;
    pMsg->payload               = payload;
    pMsg->payloadLength         = payloadLength;
    csrBtBppcMessagePut(appHandle, pMsg);
}

/* Static general help functions */
static CsrUint8 * CsrBtBppcBuildApplicationParameters(CsrUint32        tagValue,
                                                      CsrBool          transferModel,
                                                      CsrUint8         tag,
                                                      CsrUint16        *applicationParametersHeaderLength)
{



    CsrUint8 * applicationHeader = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BPPC_APPLICATION_PARAMETERS_LENGTH);
    applicationHeader[0]         = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    applicationHeader[1]         = 0x00;
    if (transferModel == CSR_BT_JOB_BASED_TRANSFER_MODEL)
    {
        applicationHeader[2]     = CSR_BT_BPPC_APPLICATION_PARAMETERS_LENGTH;
        applicationHeader[3]     = tag;
        applicationHeader[4]     = CSR_BT_BPPC_APPLICATION_PARAMETERS_TAG_LENGTH;
        applicationHeader[5]     = (CsrUint8)(tagValue >> 24);
        applicationHeader[6]     = (CsrUint8)(tagValue >> 16);
        applicationHeader[7]     = (CsrUint8)(tagValue >>  8);
        applicationHeader[8]     = (CsrUint8) tagValue;
    }
    else
    {
        applicationHeader[2]     = CSR_BT_OBEX_HEADER_LENGTH;
    }
    *applicationParametersHeaderLength = applicationHeader[2];
    return (applicationHeader);
}

static void csrBtBppcDeInitSoapBody(BppcInstanceData * pInst)
{
    CsrPmemFree(pInst->soapBody);
    pInst->soapBody       = NULL;
    pInst->soapBodyLength = 0;
    pInst->soapBodyIndex  = 0;
}

static void csrBtBppcDeInitObjectChInst(BppcInstanceData * pInst)
{
    CsrPmemFree(pInst->objectChName);
    pInst->objectChName            = NULL;
}

static void csrBtBppcReleaseJobChannel(BppcInstanceData *pInst,
                                       CsrUint8         jobState,
                                       CsrBool          normalDisconnect)
{
    if (ObexUtilDisconnectRequest(pInst->obexJobChannelInst, normalDisconnect, NULL) == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
    {
        pInst->jobChState = jobState;
    }
    else
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilDisconnectRequest on jobCh");
    }
}

static void csrBtBppcReleaseStatusChannel(BppcInstanceData * pInst, CsrBool normalDisconnect)
{
    if (ObexUtilDisconnectRequest(pInst->obexStatusChannelInst, normalDisconnect, NULL) == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
    {
        pInst->statusChState = CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE;
    }
    else
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilDisconnectRequest on statusCh");
    }
}

static void csrBtBppcObjectChNonConnectableCfmHandler(void        *instData,
                                                      CsrUint8    serverChannel,
                                                      psm_t       psm)
{
    BppcInstanceData    *pInst = (BppcInstanceData    *)instData;

    pInst->objectChState = CSR_BT_BPPC_OBJECT_CHANNEL_NON_CONNECTABLE_STATE;

    CSR_UNUSED(serverChannel);
    CSR_UNUSED(psm);
}

static void csrBtBppcDisconnectIndJobChStatusChSend(BppcInstanceData * pInst,
                                                    CsrBtReasonCode  reasonCode,
                                                    CsrBtSupplier    reasonSupplier)
{
    csrBtBppcDisconnectIndSend(pInst->appHandle, CSR_BT_BPPC_STATUS_CHANNEL, reasonCode, reasonSupplier);
    csrBtBppcDisconnectIndSend(pInst->appHandle, CSR_BT_BPPC_JOB_CHANNEL, reasonCode, reasonSupplier);
    /* If the object channel was connectable, make sure to make it non-connectable... */
    if (pInst->objectChState == CSR_BT_BPPC_OBJECT_CHANNEL_CONNECTABLE_STATE)
    {
        ObexUtilServNonConnectableStart(pInst->obexObjectChannelInst, csrBtBppcObjectChNonConnectableCfmHandler);
    }
}

static void csrBtBppcGetJobId(CsrUint8   *obexPacket,
                              CsrUint16  obexPacketLength,
                              CsrUint32  *jobId)
{
    CsrUint16 offset, length;

    *jobId = CSR_BT_BPPC_APPLICATION_PARAMETERS_INVALID_JOBID;

    if (obexPacket &&
        CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_REQUEST,
                                        obexPacket,
                                        obexPacketLength,
                                        &offset,
                                        &length))
    {
        if (obexPacket[offset] == CSR_BT_BPPC_APPLICATION_PARAMETERS_JOBID_TAG)
        {
            *jobId = CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, (offset + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
        }
    }
}

static void csrBtBppcCreateOrCancelJobXmlParser(BppcInstanceData *pInst,
                                                CsrUint16        bodyLength,
                                                CsrUint8       *body)
{
    CsrUint16   index;
    CsrUint8    *searchBuffer       = NULL;
    CsrUint16   start               = 0;
    CsrUint16   saveFrom            = 0;
    CsrUint16   searchBufferLength  = 0;
    CsrBool startFound              = FALSE;
    CsrUint8 *modifiedObject      = (CsrUint8 *) CsrPmemAlloc(bodyLength);

    for (index = 0; index < bodyLength; index++)
    {
        if (!CsrIsSpace(body[index]))
        {
            modifiedObject[searchBufferLength] = body[index];
            searchBufferLength++;
        }
    }
    index = 0;

    if (pInst->soapBody == NULL)
    {
        searchBuffer = modifiedObject;
    }
    else
    {
        searchBufferLength = (CsrUint16)(searchBufferLength + pInst->soapBodyLength);

        if (searchBufferLength > 0)
        {
            searchBuffer = CsrPmemAlloc(searchBufferLength);
            CsrMemCpy(searchBuffer, pInst->soapBody, pInst->soapBodyLength);
            CsrMemCpy(searchBuffer + pInst->soapBodyLength, modifiedObject, (searchBufferLength - pInst->soapBodyLength));
        }
        else
        {
            searchBuffer = NULL;
        }
        CsrPmemFree (pInst->soapBody);
        pInst->soapBody = NULL;
        CsrPmemFree(modifiedObject);
        modifiedObject=NULL;
    }

    if (searchBuffer)
    {
        while (index < searchBufferLength)
        {
            if (!startFound)
            {
                start = index;
                if ((index + CSR_BT_BPPC_OPERATION_STATUS_START_STRING_LENGTH) < searchBufferLength &&
                    !CsrMemCmp(searchBuffer + index, (unsigned char *) "<OperationStatus>", CSR_BT_BPPC_OPERATION_STATUS_START_STRING_LENGTH))
                {
                    startFound = TRUE;
                    start      = index;
                    saveFrom   = index;
                    index += (CSR_BT_BPPC_OPERATION_STATUS_START_STRING_LENGTH - 1);
                }
            }
            else
            {
                if ((index + CSR_BT_BPPC_OPERATION_STATUS_END_STRING_LENGTH) < searchBufferLength &&
                    !CsrMemCmp(searchBuffer + index, (unsigned char *) "</OperationStatus>", CSR_BT_BPPC_OPERATION_STATUS_END_STRING_LENGTH))
                {
                    CsrHexStrToUint16((char *) &searchBuffer[start + 19], &pInst->operationStatus);
                    CsrPmemFree (searchBuffer);
                    searchBuffer=NULL;
                    return;
                }
            }
            index++;
        }
        if (saveFrom < (index - 1))
        {
            pInst->soapBodyLength = (CsrUint16)(searchBufferLength - saveFrom);
            pInst->soapBody       = CsrPmemAlloc(pInst->soapBodyLength);
            CsrMemCpy(pInst->soapBody, searchBuffer + saveFrom, pInst->soapBodyLength);
        }
        CsrPmemFree (searchBuffer);
        searchBuffer=NULL;
    }
}

static void csrBtBppcDisconnectHandler(BppcInstanceData *pInst, CsrBool normalDisconnect)
{
    switch (pInst->jobChState)
    { /* Check what state the Job Channel is in */
        case CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE:
            {
                switch (pInst->statusChState)
                { /* Check what state the Status Channel is in */
                    case CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE:
                        { /* The Status channel is released. Ignore the message */
                            break;
                        }
                    case CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE:
                        { /* The Status Channel is being release. Wait to take action
                             until csrBtBppcStatusChDisconnectResultHandler is called */
                            break;
                        }
                    default:
                        { /* The Status Channel is in an invalid state */
                            CsrBtBppcGeneralException("Invalid statusCh state in csrBtBppcDisconnectHandler");
                            break;
                        }
                }
                break;
            }
        case CSR_BT_BPPC_JOB_CHANNEL_CONNECTING_STATE:
            { /* The Job Channel is being establish. No need to check the state on
                 the Status Channel as this connection only is connected after the
                 Job Channel is establish */
                csrBtBppcReleaseJobChannel(pInst,
                                           CSR_BT_BPPC_JOB_CHANNEL_CANCEL_CONNECT_STATE,
                                           normalDisconnect);
                break;
            }
        case CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE:
            { /* The Job Channel is connected */
                switch (pInst->statusChState)
                { /* Check what state the Status Channel is in */
                    case CSR_BT_BPPC_STATUS_CHANNEL_CONNECTING_STATE:
                    case CSR_BT_BPPC_STATUS_CHANNEL_CONNECTED_STATE:
                        { /* The Status Channel shall be disconnected before
                             disconnecting the Job Channel */
                            csrBtBppcReleaseStatusChannel(pInst, normalDisconnect);
                            break;
                        }
                    case CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE:
                        { /* The Status Channel is being release. Wait to take action
                             until csrBtBppcStatusChDisconnectResultHandler is called */
                            break;
                        }
                    default:
                        { /* The Status Channel is in an invalid state */
                            CsrBtBppcGeneralException("Invalid statusCh state in csrBtBppcDisconnectHandler");
                            break;
                        }
                }
                break;
            }
        default:
            { /* The Job Channel is allready being relased or it is allready
                 cancelling the connect procedure. Wait to take action until
                 csrBtBppcJobChDisconnectResultHandler is called */
                break;
            }
    }
}

CsrUint8 csrBtBppcGetSoapIndHandler(BppcInstanceData *pInst,
                                    CsrUint16        obexPacketLength,
                                    void             *obexChannel)
{ /* This function send the generated SOAP payload out on the right OBEX
     channel. (Job or Status) */
    CsrBool          finalFlag;
    CsrUint16        bodyLength;
    CsrUint8         *body  = NULL;

    if (obexPacketLength < (pInst->soapBodyLength - pInst->soapBodyIndex))
    { /* This is not the last body chunk */
        finalFlag               = FALSE;
        bodyLength              = obexPacketLength;
    }
    else
    { /* This is the last body chunk */
        finalFlag = TRUE;
        if (pInst->soapBodyIndex == 0)
        { /* The body chunk can be sent in the first packet */
            body                = pInst->soapBody;
            bodyLength          = pInst->soapBodyLength;
            pInst->soapBody     = NULL;
        }
        else
        { /* The body chunk could not be sent in the first packet */
            bodyLength  = (CsrUint16)(pInst->soapBodyLength - pInst->soapBodyIndex);
        }
    }

    if ((!body) && (pInst->soapBody != NULL))
    { /* The body is split in multiple OBEX GET packets. */
        body = (CsrUint8 *) CsrPmemAlloc(bodyLength);
        CsrMemCpy(body, &(pInst->soapBody[pInst->soapBodyIndex]), bodyLength);
    }

    if (finalFlag)
    { /* The last piece of SOAP payload is sent. Free the payload */
        csrBtBppcDeInitSoapBody(pInst);
    }
    else
    {
        pInst->soapBodyIndex = (CsrUint16) (pInst->soapBodyIndex + obexPacketLength); 
    }
    return (ObexUtilGetBodyRequest(obexChannel,
                                   finalFlag,
                                   &body,
                                   bodyLength,
                                   CSR_BT_OBEX_SRMP_INVALID));
}

static CsrUint32 csrBtBppcGetUint(CmnCsrBtLinkedListStruct *sdpTag,
                                  CsrUint16                attributeUuid)
{
    CsrUint32 returnValue = 0;
    
    if (CsrBtUtilSdrGetUintAttributeDataValueFromAttributeUuid(sdpTag,
                                                           0,
                                                           attributeUuid,
                                                           &returnValue))
    {
        return (returnValue);
    }
    else
    {
        return (0);
    }
}

static CsrCharString *csrBtBppcGetString(CmnCsrBtLinkedListStruct *sdpTag,
                                         CsrUint16                attributeUuid)
{
    CsrCharString   *string;
    CsrUint8    *byteString;
    CsrUint16   stringLength;
    if (CsrBtUtilSdrGetStringAttributeFromAttributeUuid(sdpTag, 0, attributeUuid, &byteString, &stringLength))
    {
        string = (CsrCharString *) CsrPmemAlloc(stringLength + 1);
        CsrMemCpy(string, byteString, stringLength);
        string[stringLength] = '\0';
    }
    else
    {
        string = (CsrCharString *) CsrPmemAlloc(1);
        string[0] = '\0';
    }
    return (string);
}

static CsrUint8 * csrBtBppcGetSupportedCharacterRepertoires(CmnCsrBtLinkedListStruct *sdpTag)
{
    CsrUint32 returnValue;
    CsrUint8  *data;

    if (CsrBtUtilSdrGetUintDataAttributeFromAttributeUuid(sdpTag,
                                                          0,
                                                          CSR_BT_CHARACTER_REPERTOIRES_SUPPORTED_ATTRIBUTE_IDENTIFIER,
                                                          &data,
                                                          &returnValue))
    {

        if (returnValue == CSR_BT_CHARACTER_REPERTOIRES_LENGTH)
        {
            CsrUint8 * supportedCharacterRepertoires = (CsrUint8 *) CsrPmemAlloc(CSR_BT_CHARACTER_REPERTOIRES_LENGTH);
            CsrMemCpy(supportedCharacterRepertoires, data, CSR_BT_CHARACTER_REPERTOIRES_LENGTH);
            return (supportedCharacterRepertoires);
        }
    }
    return (NULL);
}

static CsrUint8 csrBtBppcGetCancelJobIndHandler(BppcInstanceData      *pInst,
                                                CsrBtObexResponseCode responseCode,
                                                CsrBool               bodyHeader,
                                                CsrUint16             bodyLength,
                                                CsrUint16             bodyOffset,
                                                CsrUint8            *obexPacket,
                                                CsrUint16             obexPacketLength,
                                                void                  *obexChannel)
{ /* The CancelJob operation is running on either the Job or Status Channel */
    CsrUint8 status = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

    if (pInst->operationStatus == CSR_BT_BPPC_OPERATION_STATUS_NOT_FOUND && bodyLength > 0)
    { /* Get operationStatus from the OBEX Body header */
        csrBtBppcCreateOrCancelJobXmlParser(pInst, bodyLength, &obexPacket[bodyOffset]);
    }

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* The CsrBtBppcCancelJobReq procedure not finish */
        status = ObexUtilGetContinueRequest(obexChannel, CSR_BT_OBEX_SRMP_INVALID);
    }
    else
    { /* The CsrBtBppcCancelJobReq procedure is finish */
        csrBtBppCancelJobCfmSend(pInst->appHandle,
                                 responseCode,
                                 pInst->jobId,
                                 pInst->operationStatus);
        csrBtBppcDeInitSoapBody(pInst);
    }
    CSR_UNUSED(bodyHeader);
    CsrPmemFree(obexPacket);
    return (status);
}

static CsrUint8 csrBtBppcGetSoapReqHandler(BppcInstanceData             *pInst,
                                           ObexUtilGetRequestFuncType   getResultJobChHandler,
                                           ObexUtilGetRequestFuncType   getResultStatusChHandler,
                                           CsrBool                      getEventActivated)
{
    void                                *obexChannel;
    ObexUtilPutContinueRequestFuncType  getBodyHandler;
    ObexUtilGetRequestFuncType          getResultHandler;
    CsrUint8                            headerPriorityTable[5];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 5);
    headerPriorityTable[0] = CSR_BT_OBEX_UTIL_TYPE_HEADER;

    if (getEventActivated)
    { /* This message must be sent on the job channel */
        obexChannel         = pInst->obexJobChannelInst;
        getBodyHandler      = csrBtBppcGetJobChSoapInd;
        getResultHandler    = getResultJobChHandler;

    }
    else
    { /* This message must be sent on the status channel */
        obexChannel         = pInst->obexStatusChannelInst;
        getBodyHandler      = csrBtBppcGetStatusChSoapInd;
        getResultHandler    = getResultStatusChHandler;
    }

    return (ObexUtilGetRequest(obexChannel,
                               headerPriorityTable,
                               (const CsrUint8*) CSR_BT_BPPC_SOAP_TYPE_HEADER,
                               NULL,
                               NULL,
                               0,
                               NULL,
                               0,
                               NULL,
                               CSR_BT_OBEX_SRMP_INVALID,
                               getResultHandler,
                               getBodyHandler));
}

static CsrUint8 csrBtBppcGetContinueHandler(BppcInstanceData *pInst,
                                            CsrBool          getEventActivated)
{
    if (getEventActivated)
    { /* This message must be sent on the job channel */
        return (ObexUtilGetContinueRequest(pInst->obexJobChannelInst, CSR_BT_OBEX_SRMP_INVALID));
    }
    else
    { /* This message must be sent on the status channel */
        return (ObexUtilGetContinueRequest(pInst->obexStatusChannelInst, CSR_BT_OBEX_SRMP_INVALID));
    }
}

static void csrBtBppcMapResponseCode(CsrBool               finalFlag,
                                     CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
    {
        *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }
}

static CsrUint8 csrBtBppcConnectResponseHandler(BppcInstanceData        *pInst,
                                                CsrBtObexResponseCode   responseCode)
{
    CsrUint16 whoHeaderLength            = CSR_BT_BPPC_TARGET_LENGTH;
    CsrUint8  *who                       = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    responseCode                         = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    CsrMemCpy(who, CSR_BT_BPPC_REJ_OBJ_TARGET, whoHeaderLength);

    return (ObexUtilServConnectResponse(pInst->obexObjectChannelInst,
                                        responseCode,
                                        whoHeaderLength,
                                        &who,
                                        (CsrUint32) pInst->objectChLocalServerCh,
                                        0,
                                        NULL,
                                        NULL,
                                        NULL,
                                        0,
                                        NULL,
                                        FALSE));
}


static CsrUint8 csrBtBppcObjectChGetWithHeadersResponseHandler(BppcInstanceData            *pInst,
                                                               CsrUint16                   appParamsHeaderLength,
                                                               CsrUint8                    *appParamsHeader,
                                                               CsrUint8                    bodyHeaderPriorityIndex,
                                                               CsrBtObexResponseCode       responseCode,
                                                               CsrUint16                   bodyLength,
                                                               CsrUint8                    *body)
{
    CsrUint8 headerPriorityTable[6];

    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

    if (appParamsHeader)
    {
        headerPriorityTable[0] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
        headerPriorityTable[1] = CSR_BT_OBEX_UTIL_BODY_HEADER;
    }

    return (ObexUtilServGetWithHeaderResponse(pInst->obexObjectChannelInst,
                                              responseCode,
                                              NULL,
                                              headerPriorityTable,
                                              bodyHeaderPriorityIndex,
                                              0,
                                              NULL,
                                              NULL,
                                              appParamsHeaderLength,
                                              &appParamsHeader,
                                              0,
                                              NULL,
                                              &body,
                                              bodyLength,
                                              CSR_BT_OBEX_SRMP_INVALID));
}

/* Objectchannel callback functions */
static void csrBtBppcObjectChActivateCfmHandler(void            *instData,
                                                CsrUint8   serverChannel,
                                                psm_t           psm,
                                                CsrBtResultCode resultCode,
                                                CsrBtSupplier   resultSupplier)
{
    BppcInstanceData * pInst     = (BppcInstanceData *) instData;
    pInst->objectChLocalServerCh = serverChannel;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The Bppc Server (The Object Channel) is activate. */
        pInst->objectChState = CSR_BT_BPPC_OBJECT_CHANNEL_NON_CONNECTABLE_STATE;
        if (pInst->jobChState == CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE)
        {
            CsrUint8 status = (ObexUtilServConnectableStart(pInst->obexObjectChannelInst,
                                                            CSR_BT_BPPC_PROFILE_DEFAULT_MTU_SIZE,
                                                            pInst->localMaxPacketLength));

            if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            {
                CsrBtBppcGeneralException("not allow to call ObexUtilServConnectableStart on objectCh");
            }
            else
            {
                pInst->objectChState = CSR_BT_BPPC_OBJECT_CHANNEL_CONNECTABLE_STATE;
            }
        }
    }
    else
    { /* Cannot send give this information to the application as it has no API
         for it. Only thing to do is to activate it again */
        csrBtBppcActivateObjectChannel(pInst);
    }
    CSR_UNUSED(psm);
}

static void csrBtBppcObjectChConnectIndHandler(void            *instData,
                                               CsrBtDeviceAddr deviceAddr,
                                               CsrBtConnId     btConnId,
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
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (CsrBtBdAddrEq(&deviceAddr, &pInst->deviceAddr))
    { /* The OBEX Object Channel and the OBEX Job Channel is connected to same
         device */
        pInst->objectChPeerMaxPacketLength = (CsrUint16)(maxPeerObexPacketLength - CSR_BT_BPPC_APPLICATION_PARAMETERS_LENGTH);
        csrBtBppConnectIndSend(pInst->appHandle,
                               deviceAddr,
                               pInst->objectChPeerMaxPacketLength,
                               (CsrUint32) pInst->objectChLocalServerCh);
#ifdef CSR_AMP_ENABLE
        pInst->objectChConnId = btConnId;
        updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */

    }
    else
    { /* The OBEX Object Channel is made from another device. Reject it */
        CsrUint8 status = csrBtBppcConnectResponseHandler(pInst,
                                                          CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE);

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppcGeneralException("not allow to call csrBtBppcConnectResponseHandler on objectCh");
        }
    }
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(length);
    CSR_UNUSED(count);
    CSR_UNUSED(targetHeaderLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(challenged);
}

static void csrBtBppcObjectChDisconnectIndHandler(void            *instData,
                                                  CsrBtDeviceAddr deviceAddr,
                                                  CsrBtReasonCode reasonCode,
                                                  CsrBtSupplier   reasonSupplier,
                                                  CsrUint16       obexPacketLength,
                                                  CsrUint8        *obexPacket)
{
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcDeInitObjectChInst(pInst);
    csrBtBppcDisconnectIndSend(pInst->appHandle,
                               CSR_BT_BPPC_OBJECT_CHANNEL,
                               reasonCode,
                               reasonSupplier);
#ifdef CSR_AMP_ENABLE
    pInst->objectChConnId = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcObjectChAbortIndHandler(void              *instData,
                                             CsrUint16         descriptionOffset,
                                             CsrUint16         descriptionLength,
                                             CsrUint16         obexPacketLength,
                                             CsrUint8        *obexPacket)
{
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcDeInitObjectChInst(pInst);
    CSR_UNUSED(descriptionOffset);
    CSR_UNUSED(descriptionLength);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcObjectChDeactivateCfmHandler(void          *instData,
                                                  CsrUint8 serverChannel,
                                                  psm_t         psm)
{
    BppcInstanceData * pInst     = (BppcInstanceData *) instData;
    pInst->objectChLocalServerCh = serverChannel;
    csrBtBppcDeactivateCfmSend(pInst->appHandle);
    CSR_UNUSED(psm);
}

static void csrBtBppcObjectChGetReferenceObjectIndHandler(void        *instData,
                                                          CsrUint16   obexResponsePacketLength,
                                                          CsrBool     authenticated,
                                                          CsrBool     finalBitSet,
                                                          CsrUint16   obexPacketLength,
                                                          CsrUint8  *obexPacket)
{ /* The Get Reference Object operation is a multi-step operation */
    BppcInstanceData *pInst             = (BppcInstanceData *) instData;
    CsrUint32        missingBytesToSend = (CsrUint32)(pInst->objectChCount - pInst->objectChBytesSent);
    CsrUint32        offset             = (CsrUint32)(pInst->objectChOffset + pInst->objectChBytesSent);

    if (missingBytesToSend > obexResponsePacketLength)
    { /* Only part of the body can be include in the next response */
        csrBtBppcGetReferenceObjectIndSend(pInst->appHandle,
                                           NULL,
                                           obexResponsePacketLength,
                                           offset,
                                           FALSE);
        pInst->objectChBytesSent += obexResponsePacketLength;
    }
    else
    { /* The last body chunk can be included in the next response */
        csrBtBppcGetReferenceObjectIndSend(pInst->appHandle,
                                           NULL,
                                           missingBytesToSend,
                                           offset,
                                           TRUE);
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcObjectChGetCollectHeaderHandler(void        *instData,
                                                     CsrUint16   obexResponsePacketLength,
                                                     CsrBool     authenticated,
                                                     CsrBool     finalBitSet,
                                                     CsrUint16   obexPacketLength,
                                                     CsrUint8  *obexPacket)
{
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (!pInst->objectChName)
    { /* Look for the Name header */
        CsrUint16 nameOffset;

        if (CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset))
        { /* A name header is found */
            pInst->objectChName = (CsrUtf8String*) CsrUcs2ByteString2Utf8(obexPacket + nameOffset);
        }
    }

    if (!pInst->objectChAppParamsFound)
    { /* Look the the application parameters header */
        CsrUint16 appParametersOffset;
        CsrUint16 appParametersLength;

        if (CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_REQUEST,
                                            obexPacket,
                                            obexPacketLength,
                                            &appParametersOffset,
                                            &appParametersLength))
        { /* The application parameters header is found. Extract Count and
             Offset */
            CsrUint16 i           = 0;
            CsrUint32 returnValue = 0;

            pInst->objectChAppParamsFound = TRUE;

            while (((appParametersOffset + i + CSR_BT_BPPC_APPLICATION_PARAMETERS_TOTAL_TAG_LENGTH) <= obexPacketLength) &&
                   (i + CSR_BT_BPPC_APPLICATION_PARAMETERS_TOTAL_TAG_LENGTH <= appParametersLength))
            {
                returnValue = CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, (appParametersOffset + i + CSR_BT_BPPC_APPLICATION_PARAMETERS_TAG_HEADER_LENGTH));
                switch (obexPacket[appParametersOffset + i])
                {
                    case CSR_BT_BPPC_APPLICATION_PARAMETERS_OFFSET_TAG:
                        {
                            pInst->objectChOffset = returnValue;
                            break;
                        }
                    case CSR_BT_BPPC_APPLICATION_PARAMETERS_COUNT_TAG:
                        {
                            pInst->objectChCount = (CsrInt32)(returnValue);
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                i += CSR_BT_BPPC_APPLICATION_PARAMETERS_TOTAL_TAG_LENGTH;
            }
        }
    }

    if (finalBitSet && pInst->objectChCount != CSR_BT_BPPC_OBJECT_CHANNEL_INVALID_COUNT_VALUE)
    { /* All the headers is received */
        CsrUint16 maxResponsePacket = obexResponsePacketLength - CSR_BT_BPPC_APPLICATION_PARAMETERS_LENGTH;

        ObexUtilServSetGetContinueFunc(pInst->obexObjectChannelInst,
                                       csrBtBppcObjectChGetReferenceObjectIndHandler);

        if (pInst->objectChCount <= maxResponsePacket)
        { /* All body data can be included in the first response */
            csrBtBppcGetReferenceObjectIndSend(pInst->appHandle,
                                               pInst->objectChName,
                                               pInst->objectChCount,
                                               pInst->objectChOffset,
                                               TRUE);
        }
        else
        { /* Only part of the body data can be include in the first response */
            csrBtBppcGetReferenceObjectIndSend(pInst->appHandle,
                                               pInst->objectChName,
                                               maxResponsePacket,
                                               pInst->objectChOffset,
                                               FALSE);


            pInst->objectChBytesSent = maxResponsePacket;
        }
        pInst->objectChAppParamsFound = FALSE;
        pInst->objectChName           = NULL;
    }
    else
    { /* The Printer has more headers, e.g. the Sender is not allow to send
         a body chunks until the final bit is set */
        CsrUint8 status = ObexUtilServSetGetContinueFunc(pInst->obexObjectChannelInst,
                                                         csrBtBppcObjectChGetCollectHeaderHandler);

        if (status == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        {
            CsrBtObexResponseCode responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;

            if (finalBitSet && pInst->objectChCount == CSR_BT_BPPC_OBJECT_CHANNEL_INVALID_COUNT_VALUE)
            {
                responseCode = CSR_BT_OBEX_FORBIDDEN_RESPONSE_CODE;
            }

            status = csrBtBppcObjectChGetWithHeadersResponseHandler(pInst,
                                                                    0,
                                                                    NULL,
                                                                    CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                                    responseCode,
                                                                    0,
                                                                    NULL);
        }

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppcGeneralException("In csrBtBppcObjectChGetCollectHeaderHandler");
        }
    }
    CsrPmemFree(obexPacket);
    CSR_UNUSED(authenticated);
}

static void csrBtBppcObjectChGetIndHandler(void          *instData,
                                           CsrUint16     obexResponsePacketLength,
                                           CsrBool       authenticated,
                                           CsrBool       finalBitSet,
                                           CsrUint16     obexPacketLength,
                                           CsrUint8    *obexPacket)
{ /* The first OBEX GET packet is received. Collect all headers */
    BppcInstanceData * pInst            = (BppcInstanceData *) instData;
    pInst->objectChOffset               = 0;
    pInst->objectChCount                = CSR_BT_BPPC_OBJECT_CHANNEL_INVALID_COUNT_VALUE;
    pInst->objectChAppParamsFound       = FALSE;
    pInst->objectChBytesSent            = 0;
    csrBtBppcObjectChGetCollectHeaderHandler(pInst,
                                             obexResponsePacketLength,
                                             authenticated,
                                             finalBitSet,
                                             obexPacketLength,
                                             obexPacket);
}

static CsrUint8 csrBtBppcActivateObjectChannel(BppcInstanceData *pInst)
{
    dm_security_level_t secIncoming;
    CsrUint16           recordLength = (CsrUint16) (sizeof(csrBtBppcReferenceObjectsServiceRecord));
    CsrUint8            *record      = (CsrUint8 *) CsrPmemAlloc(recordLength);

    CsrBtScSetSecInLevel(&secIncoming,
                         CSR_BT_SEC_DEFAULT,
                         CSR_BT_SEC_NONE,
                         CSR_BT_BPPC_OBJECT_CHANNEL_SECURITY,
                         CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                         CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    CsrMemCpy(record, csrBtBppcReferenceObjectsServiceRecord, recordLength);

    return (ObexUtilServActivateStart(pInst->obexObjectChannelInst,
                                      secIncoming,
                                      CSR_BT_BASIC_PRINTING_UUID,
                                      CSR_BT_BPPC_LP_SUPERVISION_TIMEOUT,
                                      pInst->objectChLocalServerCh,
                                      L2CA_PSM_INVALID,
                                      recordLength,
                                      CSR_BT_BPPC_RFCOMM_OBJECT_SERVER_CHANNEL_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &record,
                                      (CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK | CSR_BT_RENDERING_MAJOR_SERVICE_MASK),
                                      csrBtBppcObjectChConnectIndHandler,
                                      NULL,
                                      csrBtBppcObjectChGetIndHandler,
                                      NULL,
                                      csrBtBppcObjectChAbortIndHandler,
                                      csrBtBppcObjectChDisconnectIndHandler,
                                      NULL,
                                      csrBtBppcObjectChActivateCfmHandler,
                                      NULL,
                                      pInst->localMaxPacketLength,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));

}


/* Status Channel callback functions */
static void csrBtBppcStatusChConnectResultHandler(void                     *instData,
                                                  CsrBtResultCode          resultCode,
                                                  CsrBtSupplier            resultSupplier,
                                                  CsrBtDeviceAddr          deviceAddr,
                                                  CsrBtConnId              btConnId,
                                                  CsrUint16                maxPeerObexPacketLength,
                                                  CmnCsrBtLinkedListStruct *sdpTag,
                                                  CsrUint16                obexPacketLength,
                                                  CsrUint8                 *obexPacket)
{ /* The OBEX CONNECT operation is done on the Status Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if(resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
       resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    { /* The StatusChannel is establish. */
        if (pInst->jobChState == CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE)
        {
            CsrUint8  colorSupported            = (CsrUint8)  (csrBtBppcGetUint(sdpTag, CSR_BT_COLOR_SUPPORTED__ATTRIBUTE_IDENTIFIER));
            CsrUint8  duplexSupported           = (CsrUint8)  (csrBtBppcGetUint(sdpTag, CSR_BT_DUPLEX_SUPPORTED_ATTRIBUTE_IDENTIFIER));
            CsrUint16 maxMediaWidth             = (CsrUint16) (csrBtBppcGetUint(sdpTag, CSR_BT_MAX_MEDIA_WIDTH_ATTRIBUTE_IDENTIFIER));
            CsrUint16 maxMediaLength            = (CsrUint16) (csrBtBppcGetUint(sdpTag, CSR_BT_MAX_MEDIA_LENGTH_ATTRIBUTE_IDENTIFIER));
            CsrUtf8String *xhtmlPrintImageFormats   = (CsrUtf8String *) csrBtBppcGetString(sdpTag, CSR_BT_XHTML_PRINT_IMAGE_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER);
            CsrUtf8String *documentFormatsSupported = (CsrUtf8String *) csrBtBppcGetString(sdpTag, CSR_BT_DOCUMENT_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER);
            CsrUtf8String *mediaTypesSupported      = (CsrUtf8String *) csrBtBppcGetString(sdpTag, CSR_BT_MEDIA_TYPES_SUPPORTED_ATTRIBUTE_IDENTIFIER);
            CsrUtf8String *printerModelId           = (CsrUtf8String *) csrBtBppcGetString(sdpTag, CSR_BT_PRINTER_1284ID_ATTRIBUTE_IDENTIFIER);
            CsrUint8  *characterRepertoires     = csrBtBppcGetSupportedCharacterRepertoires(sdpTag);
            pInst->statusChState                = CSR_BT_BPPC_STATUS_CHANNEL_CONNECTED_STATE;

            if (maxPeerObexPacketLength > pInst->jobChPeerMaxPacketLength)
            {  /* As the API only announce one maxPeerObexPacketLength select
                  the lowest one */
                maxPeerObexPacketLength = pInst->jobChPeerMaxPacketLength;
            }
            csrBtBppcConnectCfmSend(pInst->appHandle,
                                    resultCode,
                                    resultSupplier,
                                    colorSupported,
                                    duplexSupported,
                                    maxPeerObexPacketLength,
                                    maxMediaWidth,
                                    maxMediaLength,
                                    characterRepertoires,
                                    xhtmlPrintImageFormats,
                                    documentFormatsSupported,
                                    mediaTypesSupported,
                                    printerModelId,
                                    btConnId);
#ifdef CSR_AMP_ENABLE
            pInst->statusChConnId = btConnId;
            updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */

            CsrPmemFree(characterRepertoires);

        }
        else
        {
            CsrBtBppcGeneralException("Invalid jobCh state in csrBtBppcStatusChConnectResultHandler");
        }
    }
    else
    { /* Failed to established the StatusChannel */
        pInst->statusChState = CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE;

        switch (pInst->jobChState)
        { /* Check what state the Job Channel is in */
            case CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE:
                { /* The Job Channel is connected. Release it. */
                    csrBtBppcReleaseJobChannel(pInst,
                                               CSR_BT_BPPC_JOB_CHANNEL_CANCEL_CONNECT_STATE,
                                               TRUE);
                    break;
                }
            case CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE:
                { /* The Job Channel is idle, e.g the job Channel were release
                     while the status channel were being establish */
                    csrBtBppcConnectCfmSend(pInst->appHandle,
                                            resultCode,
                                            resultSupplier,
                                            FALSE,
                                            FALSE,
                                            CSR_BT_MIN_OBEX_PACKET_LENGTH,
                                            0,
                                            0,
                                            NULL,
                                            NULL,
                                            NULL,
                                            NULL,
                                            NULL,
                                            btConnId);
                    break;
                }
            default:
                { /* The Job Channel is in an ivalid state */
                    CsrBtBppcGeneralException("Invalid jobCh state in csrBtBppcStatusChConnectResultHandler");
                    break;
                }
        }
    }
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcStatusChDisconnectResultHandler(void                *instData,
                                                     CsrBtReasonCode     reasonCode,
                                                     CsrBtSupplier       reasonSupplier,
                                                     CsrUint8          *obexPacket,
                                                     CsrUint16           obexPacketLength)
{ /* The Status Channel is disconnected */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    switch (pInst->statusChState)
    { /* Check what state the Status Channel is in */
        case CSR_BT_BPPC_STATUS_CHANNEL_CONNECTED_STATE:
            {
                switch (pInst->jobChState)
                { /* Check what state the Job Channel is in */
                    case CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE:
                        { /* The job channel is still connected, release it */
                            csrBtBppcReleaseJobChannel(pInst,
                                                       CSR_BT_BPPC_JOB_CHANNEL_DISCONNECTING_STATE,
                                                       TRUE);
                            break;
                        }
                    case CSR_BT_BPPC_JOB_CHANNEL_DISCONNECTING_STATE:
                        { /* The Job Channel is being release. Wait to take action
                             until csrBtBppcJobChDisconnectResultHandler is called */
                            break;
                        }
                    default:
                        { /* The Job Channel is in an invalid state */
                            CsrBtBppcGeneralException("Invalid jobCh state in csrBtBppcStatusChDisconnectResultHandler");
                            break;
                        }
                }
                break;
            }
        case CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE:
            {
                switch (pInst->jobChState)
                { /* Check what state the Job Channel is in */
                    case CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE:
                        { /* The job channel is still connected, release it */
                            csrBtBppcReleaseJobChannel(pInst,
                                                       CSR_BT_BPPC_JOB_CHANNEL_DISCONNECTING_STATE,
                                                       TRUE);
                            break;
                        }
                    case CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE:
                        { /* The Job Channel is allready released */
                            csrBtBppcDisconnectIndJobChStatusChSend(pInst, reasonCode, reasonSupplier);
                            break;
                        }
                    default:
                        { /* The Job Channel is in an invalid state */
                            CsrBtBppcGeneralException("Invalid jobCh state in csrBtBppcStatusChDisconnectResultHandler");
                            break;
                        }
                }
                break;
            }
        default:
            { /* The Status Channel is in an invalid state */
                CsrBtBppcGeneralException("Invalid statusCh state in csrBtBppcStatusChDisconnectResultHandler");
                break;
            }
    }
#ifdef CSR_AMP_ENABLE
    pInst->jobChConnId = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    pInst->statusChState = CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE;
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcStatusChAbortResultHandler(void                  *instData,
                                                CsrBtObexResponseCode responseCode,
                                                CsrUint8            *obexPacket,
                                                CsrUint16             obexPacketLength)
{ /* An OBEX operation on the Status Channel has been aborted */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    csrBtBppcAbortCfmSend(pInst->appHandle, CSR_BT_BPPC_STATUS_CHANNEL);
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcGetStatusChSoapInd(void       *instData,
                                        CsrUint16  obexPacketLength)
{ /* SOAP payload is being sent on the Status Channel */
    BppcInstanceData *pInst = (BppcInstanceData *) instData;
    CsrUint8 status         = csrBtBppcGetSoapIndHandler(pInst,
                                                         obexPacketLength,
                                                         pInst->obexStatusChannelInst);


    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilGetBodyRequest on statusCh");
    }
}

static void csrBtBppcGetCancelJobIndStatusChHandler(void                  *instData,
                                                    CsrBtObexResponseCode responseCode,
                                                    CsrBool               bodyHeader,
                                                    CsrUint16             bodyLength,
                                                    CsrUint16             bodyOffset,
                                                    CsrUint8            *obexPacket,
                                                    CsrUint16             obexPacketLength)
{ /* An OBEX GET packet is received as part of the CancelJob procedure on the
     Status Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    CsrUint8 status          = csrBtBppcGetCancelJobIndHandler(pInst,
                                                               responseCode,
                                                               bodyHeader,
                                                               bodyLength,
                                                               bodyOffset,
                                                               obexPacket,
                                                               obexPacketLength,
                                                               pInst->obexStatusChannelInst);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilGetBodyRequest on statusCh");
    }
}

static void csrBtBppcGetEventIndStatusChHandler(void                  *instData,
                                                CsrBtObexResponseCode responseCode,
                                                CsrBool               bodyHeader,
                                                CsrUint16             bodyLength,
                                                CsrUint16             bodyOffset,
                                                CsrUint8            *obexPacket,
                                                CsrUint16             obexPacketLength)
{ /* An OBEX GET packet is received as part of the Get Event procedure on the
     Status Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* The CsrBtBppcGetEventReq procedure not finish */
        csrBtBppcGetEventIndSend(pInst->appHandle,
                                 bodyLength,
                                 bodyOffset,
                                 obexPacket,
                                 obexPacketLength);
    }
    else
    { /* According to the BPP_SPEC revision V12r00section 7.1.7 will this
         operation remain active. I.e. the OBEX response code must be
         CSR_BT_OBEX_CONTINUE_RESPONSE_CODE with final bit set until aborted
         by the client */
        CsrPmemFree(obexPacket);
    }
    CSR_UNUSED(bodyHeader);
}

/* Job Channel callback functions */
static void csrBtBppcJobChConnectResultHandler(void                     *instData,
                                               CsrBtResultCode          resultCode,
                                               CsrBtSupplier            resultSupplier,
                                               CsrBtDeviceAddr          deviceAddr,
                                               CsrBtConnId              btConnId,
                                               CsrUint16                maxPeerObexPacketLength,
                                               CmnCsrBtLinkedListStruct *sdpTag,
                                               CsrUint16                obexPacketLength,
                                               CsrUint8               *obexPacket)
{ /* The OBEX CONNECT operation is done on the Job Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if(resultCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
       resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    { /* The JobChannel is establish. Setup the Status Channel */
        CsrUint8  status;
        CsrUint16 index;
        CsrUint8 tempTarget[CSR_BT_BPPC_TARGET_LENGTH]  = CSR_BT_BPPC_STS_TARGET;
        CmnCsrBtLinkedListStruct * sdpTag               = NULL;
        CsrUint8 *target                                = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BPPC_TARGET_LENGTH);
        pInst->jobChPeerMaxPacketLength                 = maxPeerObexPacketLength;
        pInst->jobChState                               = CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE;
        CsrMemCpy(target, tempTarget, CSR_BT_BPPC_TARGET_LENGTH);
        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_PRINTING_STATUS_UUID, &index);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_CHARACTER_REPERTOIRES_SUPPORTED_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_DOCUMENT_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_COLOR_SUPPORTED__ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_DUPLEX_SUPPORTED_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_MEDIA_TYPES_SUPPORTED_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_MAX_MEDIA_WIDTH_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_MAX_MEDIA_LENGTH_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_PRINTER_1284ID_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrCreateAndInsertAttribute(sdpTag, index, CSR_BT_XHTML_PRINT_IMAGE_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER, NULL, 0);
        CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);
        CsrBtUtilSdrInsertPerformExtendedSearch(sdpTag, index);

        if ((pInst->objectChState != CSR_BT_BPPC_OBJECT_CHANNEL_CONNECTABLE_STATE) && (pInst->objectChLocalServerCh != CSR_BT_NO_SERVER))
        {/* If the object channel has been activated (i.e. there is a server channel) and
            it is not connectable yet, make sure to make it connectable                  */
            CsrUint8 status = (ObexUtilServConnectableStart(pInst->obexObjectChannelInst,
                                                            CSR_BT_BPPC_PROFILE_DEFAULT_MTU_SIZE,
                                                            CSR_BT_MAX_OBEX_SIGNAL_LENGTH));

            if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            {
                CsrBtBppcGeneralException("not allow to call ObexUtilServConnectableStart on objectCh");
            }
            else
            {/* Success: the object channel is connectable now */
                pInst->objectChState = CSR_BT_BPPC_OBJECT_CHANNEL_CONNECTABLE_STATE;
            }
        }

        status = ObexUtilConnectRequest(pInst->obexStatusChannelInst,
                                        &sdpTag,
                                        pInst->deviceAddr,
                                        pInst->secOutgoing,
                                        pInst->localMaxPacketLength,
                                        CSR_BT_BPPC_PROFILE_DEFAULT_MTU_SIZE,
                                        &target,
                                        CSR_BT_BPPC_TARGET_LENGTH,
                                        0,
                                        NULL,
                                        CSR_BT_BPPC_LP_SUPERVISION_TIMEOUT,
                                        NULL,
                                        0,
                                        0,
                                        NULL,
                                        0,
                                        NULL,
                                        csrBtBppcStatusChConnectResultHandler,
                                        NULL,
                                        csrBtBppcStatusChDisconnectResultHandler,
                                        NULL,
                                        pInst->windowSize,
                                        pInst->localSrmEnable);
#ifdef CSR_AMP_ENABLE
        pInst->jobChConnId = btConnId;
        updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        { /* An exception has occurred. Release the Job Channel */
            CsrBtBppcGeneralException("not allow to call ObexUtilConnectRequest on statusCh");
            csrBtBppcReleaseJobChannel(pInst,
                                       CSR_BT_BPPC_JOB_CHANNEL_CANCEL_CONNECT_STATE,
                                       FALSE);
        }
        else
        {
            pInst->statusChState = CSR_BT_BPPC_STATUS_CHANNEL_CONNECTING_STATE;
        }
    }
    else
    { /* Failed to established the JobChannel */
        pInst->jobChState = CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE;
        bd_addr_zero(&(pInst->deviceAddr));
        csrBtBppcConnectCfmSend(pInst->appHandle,
                                resultCode,
                                resultSupplier,
                                FALSE,
                                FALSE,
                                maxPeerObexPacketLength,
                                0,
                                0,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                NULL,
                                btConnId);
    }
    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcJobChAuthenticateIndHandler(void            *instData,
                                                 CsrBtDeviceAddr deviceAddr,
                                                 CsrUint8        options,
                                                 CsrUint16       realmLength,
                                                 CsrUint8      *realm)
{ /* The Job Channel has been OBEX authenticated */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcAuthenticateIndSend(pInst->appHandle, deviceAddr, options, realmLength, realm);
}

static void csrBtBppcJobChDisconnectResultHandler(void                *instData,
                                                  CsrBtReasonCode     reasonCode,
                                                  CsrBtSupplier       reasonSupplier,
                                                  CsrUint8          *obexPacket,
                                                  CsrUint16           obexPacketLength)
{ /* The Job Channel has been released */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    switch (pInst->jobChState)
    { /* Check what state the Job Channel is in */
        case CSR_BT_BPPC_JOB_CHANNEL_CONNECTED_STATE:
            { /* The Job Channel were connected */
                switch (pInst->statusChState)
                { /* Check what state the Status Channel is in */
                    case CSR_BT_BPPC_STATUS_CHANNEL_CONNECTED_STATE:
                    case CSR_BT_BPPC_STATUS_CHANNEL_CONNECTING_STATE:
                        { /* The Status Channel is being or is established.
                             Release the Status Channel and wait to take action until
                             csrBtBppcStatusChDisconnectResultHandler is called */
                            csrBtBppcReleaseStatusChannel(pInst, FALSE);
                            break;
                        }
                    case CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE:
                        { /* The Status Channel is being release. Wait to take action
                             until csrBtBppcStatusChDisconnectResultHandler is called */
                            break;
                        }
                    default:
                        { /* The statusChState must be CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE
                             e.g. the Status Channel is allready released */
                            csrBtBppcDisconnectIndJobChStatusChSend(pInst,
                                                                    reasonCode,
                                                                    reasonSupplier);
                            break;
                        }
                }
                break;
            }
        case CSR_BT_BPPC_JOB_CHANNEL_CANCEL_CONNECT_STATE:
            { /* The Job Channel were release because BPPC failed to established
                 the Status Channel */
                csrBtBppcConnectCfmSend(pInst->appHandle,
                                        CSR_BT_RESULT_CODE_OBEX_CONNECT_CANCELLED,
                                        CSR_BT_SUPPLIER_OBEX_PROFILES,
                                        FALSE,
                                        FALSE,
                                        CSR_BT_MIN_OBEX_PACKET_LENGTH,
                                        0,
                                        0,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        NULL,
                                        CSR_BT_CONN_ID_INVALID);
                break;
            }
        case CSR_BT_BPPC_JOB_CHANNEL_DISCONNECTING_STATE:
            { /* The Job Channel has initiated the release procedure. Find out why */
                switch (pInst->statusChState)
                { /* Check what state the Status Channel is in */
                    case CSR_BT_BPPC_STATUS_CHANNEL_DISCONNECTING_STATE:
                        { /* The Status Channel is being release. Wait to take action
                             until csrBtBppcStatusChDisconnectResultHandler is called */
                            break;
                        }
                    case CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE:
                        { /* The Status Channel is allready released */
                            csrBtBppcDisconnectIndJobChStatusChSend(pInst,
                                                                    reasonCode,
                                                                    reasonSupplier);
                            break;
                        }
                    default:
                        { /* The Status Channel is in an invalid state */
                            CsrBtBppcGeneralException("Invalid statusCh state in csrBtBppcJobChDisconnectResultHandler");
                            break;
                        }
                }
                break;
            }
        default:
            { /* The Job Channel is in an invalid state */
                CsrBtBppcGeneralException("Invalid jobCh state in csrBtBppcJobChDisconnectResultHandler");
                break;
            }
    }
#ifdef CSR_AMP_ENABLE
    pInst->jobChConnId = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    csrBtBppcDeInitSoapBody(pInst);
    bd_addr_zero(&(pInst->deviceAddr));
    pInst->jobChState = CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE;
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcJobChAbortResultHandler(void                  *instData,
                                             CsrBtObexResponseCode responseCode,
                                             CsrUint8            *obexPacket,
                                             CsrUint16             obexPacketLength)
{ /* An OBEX operation on the Job Channel has been aborted */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcDeInitSoapBody(pInst);
    csrBtBppcAbortCfmSend(pInst->appHandle, CSR_BT_BPPC_JOB_CHANNEL);
    CSR_UNUSED(responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcGetJobChSoapInd(void       *instData,
                                     CsrUint16  obexPacketLength)
{ /* SOAP payload is being sent over the Job Channel */
    BppcInstanceData *pInst = (BppcInstanceData *) instData;
    CsrUint8         status = csrBtBppcGetSoapIndHandler(pInst,
                                                         obexPacketLength,
                                                         pInst->obexJobChannelInst);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilGetBodyRequest on jobCh");
    }
}

static void csrBtBppcGetCreateJobIndHandler(void                  *instData,
                                            CsrBtObexResponseCode responseCode,
                                            CsrBool               bodyHeader,
                                            CsrUint16             bodyLength,
                                            CsrUint16             bodyOffset,
                                            CsrUint8            *obexPacket,
                                            CsrUint16             obexPacketLength)
{ /* An OBEX GET packet is received as part of the CreateJob procedure on the
     Job Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (pInst->jobId == CSR_BT_BPPC_APPLICATION_PARAMETERS_INVALID_JOBID)
    {
        csrBtBppcGetJobId(obexPacket, obexPacketLength, &pInst->jobId);
    }

    if (pInst->operationStatus == CSR_BT_BPPC_OPERATION_STATUS_NOT_FOUND && bodyLength > 0)
    {
        csrBtBppcCreateOrCancelJobXmlParser(pInst, bodyLength, &obexPacket[bodyOffset]);
    }

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* The CsrBtBppcCreateJobReq procedure is not finish */
        CsrUint8 status = ObexUtilGetContinueRequest(pInst->obexJobChannelInst, CSR_BT_OBEX_SRMP_INVALID);

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppcGeneralException("not allow to call ObexUtilGetBodyRequest on jobCh");
        }
    }
    else
    { /* The CsrBtBppcCreateJobReq procedure is finish */
        csrBtBppCreateJobCfmSend(pInst->appHandle,
                                 responseCode,
                                 pInst->jobId,
                                 pInst->operationStatus);
        csrBtBppcDeInitSoapBody(pInst);
    }
    CSR_UNUSED(bodyHeader);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcGetCancelJobIndJobChHandler(void                  *instData,
                                                 CsrBtObexResponseCode responseCode,
                                                 CsrBool               bodyHeader,
                                                 CsrUint16             bodyLength,
                                                 CsrUint16             bodyOffset,
                                                 CsrUint8            *obexPacket,
                                                 CsrUint16             obexPacketLength)
{ /* An OBEX GET packet is received as part of the CancelJob procedure on the
     Job Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    CsrUint8 status          = csrBtBppcGetCancelJobIndHandler(pInst,
                                                               responseCode,
                                                               bodyHeader,
                                                               bodyLength,
                                                               bodyOffset,
                                                               obexPacket,
                                                               obexPacketLength,
                                                               pInst->obexJobChannelInst);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppcGeneralException("not allow to call ObexUtilGetBodyRequest on jobCh");
    }
}

static void csrBtBppcSendDocumentResultHandler(void                   *instData,
                                               CsrBtObexResponseCode  responseCode,
                                               CsrUint8             *obexPacket,
                                               CsrUint16              obexPacketLength)
{ /* The SendDocument operation is finish */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcSendDocumentCfmSend(pInst->appHandle, responseCode);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppcSendDocumentContinueHandler(void      *instData,
                                                 CsrUint16 obexPacketLength)
{ /* The SendDocument operation needs more data */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;
    csrBtBppcSendDocumentIndSend(pInst->appHandle, obexPacketLength);
}

/* Share Job and Status Channel callback functions */
static void csrBtBppcGetPrintAttrIndHandler(void                  *instData,
                                            CsrBtObexResponseCode responseCode,
                                            CsrBool               bodyHeader,
                                            CsrUint16             bodyLength,
                                            CsrUint16             bodyOffset,
                                            CsrUint8            *obexPacket,
                                            CsrUint16             obexPacketLength)
{ /* The Get Printer Attributes operation is running on either the Job or
     Status Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* The CsrBtBppcGetPrinterAttributesReq procedure not finish */
        csrBtBppcGetPrintAttrIndSend(pInst->appHandle,
                                     bodyLength,
                                     bodyOffset,
                                     obexPacket,
                                     obexPacketLength);
    }
    else
    { /* The CsrBtBppcGetPrinterAttributesReq procedure is finish */
        csrBtBppcGetPrintAttrCfmSend(pInst->appHandle,
                                     responseCode,
                                     bodyLength,
                                     bodyOffset,
                                     obexPacket,
                                     obexPacketLength);
        csrBtBppcDeInitSoapBody(pInst);
    }
    CSR_UNUSED(bodyHeader);
}

static void csrBtBppcGetJobAttrIndHandler(void                  *instData,
                                          CsrBtObexResponseCode responseCode,
                                          CsrBool               bodyHeader,
                                          CsrUint16             bodyLength,
                                          CsrUint16             bodyOffset,
                                          CsrUint8            *obexPacket,
                                          CsrUint16             obexPacketLength)
{ /* The Get Job Attributes operation is running on either the Job or Status
     Channel */
    BppcInstanceData * pInst = (BppcInstanceData *) instData;

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    { /* The CsrBtBppcGetJobAttributesReq procedure not finish */
        csrBtBppcGetJobAttributesIndSend(pInst->appHandle,
                                         bodyLength,
                                         bodyOffset,
                                         obexPacket,
                                         obexPacketLength);
    }
    else
    { /* The CsrBtBppcGetJobAttributesReq procedure is finish */
        csrBtBppcGetJobAttributesCfmSend(pInst->appHandle,
                                         responseCode,
                                         bodyLength,
                                         bodyOffset,
                                         obexPacket,
                                         obexPacketLength);
        csrBtBppcDeInitSoapBody(pInst);
    }
    CSR_UNUSED(bodyHeader);
}

/* Public BPPC downstream message handlers */
CsrUint8 CsrBtBppcConnectReqHandler(BppcInstanceData *pInst, void *msg)
{
    CsrUint8  status = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;

    if (pInst->jobChState    == CSR_BT_BPPC_JOB_CHANNEL_IDLE_STATE &&
        pInst->statusChState == CSR_BT_BPPC_STATUS_CHANNEL_IDLE_STATE)
    { /* The application is allow to start the connect procedure */
        CsrUint16 index;
        CsrUint8   headerPriorityTable[2]               = {CSR_BT_OBEX_UTIL_LENGTH_HEADER, CSR_BT_OBEX_UTIL_COUNT_HEADER};
        CsrUint8   numOfHeaders                         = 0;
        CsrUint8 tempTarget[CSR_BT_BPPC_TARGET_LENGTH]  = CSR_BT_BPPC_DPS_TARGET;
        CsrBtBppcConnectReq *pMsg                       = (CsrBtBppcConnectReq *) msg;
        CmnCsrBtLinkedListStruct * sdpTag               = NULL;
        CsrUint8 *target                                = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BPPC_TARGET_LENGTH);
        pInst->appHandle                                = pMsg->appHandle;
        pInst->deviceAddr                               = pMsg->deviceAddr;
        pInst->localMaxPacketLength                     = pMsg->maxPacketSize;
        pInst->windowSize                               = pMsg->windowSize;
        pInst->localSrmEnable                           = pMsg->srmEnable;
        CsrMemCpy(target, tempTarget, CSR_BT_BPPC_TARGET_LENGTH);
        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag, CSR_BT_DIRECT_PRINTING_UUID, &index);
        CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

        if (pMsg->length > 0)
        {
            numOfHeaders = (CsrUint8)(numOfHeaders + 1);
        }

        if (pMsg->count > 0)
        {
            numOfHeaders = (CsrUint8)(numOfHeaders + 1);
        }

        status = ObexUtilConnectRequest(pInst->obexJobChannelInst,
                                        &sdpTag,
                                        pMsg->deviceAddr,
                                        pInst->secOutgoing,
                                        pInst->localMaxPacketLength,
                                        CSR_BT_BPPC_PROFILE_DEFAULT_MTU_SIZE,
                                        &target,
                                        CSR_BT_BPPC_TARGET_LENGTH,
                                        numOfHeaders,
                                        headerPriorityTable,
                                        CSR_BT_BPPC_LP_SUPERVISION_TIMEOUT,
                                        NULL,
                                        pMsg->length,
                                        pMsg->count,
                                        NULL,
                                        0,
                                        NULL,
                                        csrBtBppcJobChConnectResultHandler,
                                        csrBtBppcJobChAuthenticateIndHandler,
                                        csrBtBppcJobChDisconnectResultHandler,
                                        NULL,
                                        pInst->windowSize,
                                        pInst->localSrmEnable);

        if (status == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        {
            pInst->jobChState = CSR_BT_BPPC_JOB_CHANNEL_CONNECTING_STATE;
        }
    }
    return (status);
}

CsrUint8 CsrBtBppcDisconnectReqHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcDisconnectReq *pMsg = (CsrBtBppcDisconnectReq *) msg;
    csrBtBppcDisconnectHandler(pInst, pMsg->normalDisconnect);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBppcAbortReqHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcAbortReq *pMsg = (CsrBtBppcAbortReq *) msg;

    switch (pMsg->channelId)
    {
        case CSR_BT_BPPC_JOB_CHANNEL:
            {
                return (ObexUtilAbortRequest(pInst->obexJobChannelInst,
                                             NULL,
                                             csrBtBppcJobChAbortResultHandler));
            }
        case CSR_BT_BPPC_STATUS_CHANNEL:
            {
                return (ObexUtilAbortRequest(pInst->obexStatusChannelInst,
                                             NULL,
                                             csrBtBppcStatusChAbortResultHandler));
            }
        default:
            {
                CsrBtBppcGeneralException("Invalid channelId in CsrBtBppcAbortReqHandler");
                return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
            }
    }
}

CsrUint8 CsrBtBppcCancelConnectReqHandler(BppcInstanceData *pInst, void *msg)
{
    CSR_UNUSED(msg);
    csrBtBppcDisconnectHandler(pInst, FALSE);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBppcAuthenticateResHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcAuthenticateRes *pMsg = (CsrBtBppcAuthenticateRes *) msg;
    return(ObexUtilCliAuthenticateResponse(pInst->obexJobChannelInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId, NULL));
}

CsrUint8 CsrBtBppcSecurityOutReqHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcSecurityOutReq *pMsg = (CsrBtBppcSecurityOutReq*) msg;
    CsrBtResultCode rval          = CsrBtScSetSecOutLevel(&pInst->secOutgoing,
                                                          pMsg->secLevel,
                                                          CSR_BT_OBEX_PRINTING_INITIATOR_MANDATORY_SECURITY_OUTGOING,
                                                          CSR_BT_OBEX_PRINTING_INITIATOR_DEFAULT_SECURITY_OUTGOING,
                                                          CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                          CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtBppcSecurityOutCfmSend(pMsg->appHandle, rval);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBppcGetPrintAttrReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Get Printer Attributes operation. Note can run on the Job
     and the Status Channel */
    CsrBool                             getAllAttrs;

    CsrBtBppcGetPrinterAttributesReq *pMsg  = (CsrBtBppcGetPrinterAttributesReq *) msg;
    CsrUint16 contentLength                 = CsrBtBppcGetSoapEncodedGetPrinterAttrReqContentLength(pMsg, &getAllAttrs);
    pInst->soapBodyIndex                    = 0;
    pInst->soapBody                         = CsrBtBppcWriteSoapEncodedGetPrinterAttrReq(pMsg,
                                                                                         contentLength,
                                                                                         getAllAttrs,
                                                                                         &pInst->soapBodyLength);

    return (csrBtBppcGetSoapReqHandler(pInst,
                                       csrBtBppcGetPrintAttrIndHandler,
                                       csrBtBppcGetPrintAttrIndHandler,
                                       pInst->getEventActivated));
}

CsrUint8 CsrBtBppcGetPrintAttrResHandler(BppcInstanceData *pInst, void *msg)
{ /* In the middle of the Get Printer Attributes operation. Note can run on the
     Job and the Status Channel */
    CSR_UNUSED(msg);
    return (csrBtBppcGetContinueHandler(pInst, pInst->getEventActivated));
}

CsrUint8 CsrBtBppcCreateJobReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Create Job operation. Note can only run on the Job Channel */
    CsrBtBppcCreateJobReq *pMsg = (CsrBtBppcCreateJobReq *) msg;
    CsrUint16 contentLength     = CsrBtBppcGetSoapEncodedCreateJobReqContentLength(pMsg);
    pInst->soapBodyIndex        = 0;
    pInst->jobId                = CSR_BT_BPPC_APPLICATION_PARAMETERS_INVALID_JOBID;
    pInst->operationStatus      = CSR_BT_BPPC_OPERATION_STATUS_NOT_FOUND;
    pInst->soapBody             = CsrBtBppcWriteSoapEncodedCreateJobReq(pMsg,
                                                                        contentLength,
                                                                        &pInst->soapBodyLength);

    return (csrBtBppcGetSoapReqHandler(pInst,
                                       csrBtBppcGetCreateJobIndHandler,
                                       csrBtBppcGetCreateJobIndHandler,
                                       TRUE));
}

CsrUint8 CsrBtBppcDocumentReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Send Document operation. Note can only run on the Job Channel */
    CsrUint8    status;
    CsrUint16   applicationParametersHeaderLength;
    CsrUint8 headerPriorityTable[6] = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                       CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER,
                                       CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER,
                                       CSR_BT_OBEX_UTIL_NAME_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                       CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

    CsrBtBppcSendDocumentReq *pMsg  = (CsrBtBppcSendDocumentReq *) msg;
    CsrUint8 *appParametersHeader   = CsrBtBppcBuildApplicationParameters(pMsg->jobId,
                                                                          pMsg->transferModel,
                                                                          CSR_BT_BPPC_APPLICATION_PARAMETERS_JOBID_TAG,
                                                                          &applicationParametersHeaderLength);

    if (!pMsg->docTypeDependentInfo)
    { /* No description header */
        headerPriorityTable[2] = CSR_BT_OBEX_UTIL_NAME_HEADER;
        headerPriorityTable[3] = CSR_BT_OBEX_UTIL_UNDEFINE_HEADER;
    }

    status = ObexUtilPutRequest(pInst->obexJobChannelInst,
                                headerPriorityTable,
                                0,
                                (CsrUint8*)pMsg->mimeMediaType,
                                &pMsg->ucs2documentName,
                                &pMsg->docTypeDependentInfo,
                                applicationParametersHeaderLength,
                                &appParametersHeader,
                                0,
                                NULL,
                                csrBtBppcSendDocumentContinueHandler,
                                csrBtBppcSendDocumentResultHandler);

    CsrPmemFree(pMsg->mimeMediaType);
    return (status);
}

CsrUint8 CsrBtBppcDocumentResHandler(BppcInstanceData *pInst, void *msg)
{ /* In the middle of the Send Document operation. Note can only run on the
     Job Channel */
    CsrBtBppcSendDocumentRes *pMsg  = (CsrBtBppcSendDocumentRes *) msg;
    return(ObexUtilPutContinueRequest(pInst->obexJobChannelInst,
                                      pMsg->finalFlag,
                                      &pMsg->printContent,
                                      pMsg->printContentLength));
}

CsrUint8 CsrBtBppcGetJobAttrReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Get Job Attributes operation. Note can run on the Job
     and the Status Channel */
    CsrBtBppcGetJobAttributesReq *pMsg  = (CsrBtBppcGetJobAttributesReq *) msg;
    CsrUint16 contentLength             = CsrBtBppcGetSoapEncodedGetJobAttrReqContentLength(pMsg);
    pInst->soapBodyIndex                = 0;
    pInst->soapBody                     = CsrBtBppcWriteSoapEncodedGetJobAttrReq(pMsg,
                                                                                 contentLength,
                                                                                 &pInst->soapBodyLength);

    return (csrBtBppcGetSoapReqHandler(pInst,
                                       csrBtBppcGetJobAttrIndHandler,
                                       csrBtBppcGetJobAttrIndHandler,
                                       pInst->getEventActivated));
}

CsrUint8 CsrBtBppcGetJobAttrResHandler(BppcInstanceData *pInst, void *msg)
{ /* In the middle of the Get Job Attributes operation. Note can run on the Job
     and the Status Channel */
    CSR_UNUSED(msg);
    return (csrBtBppcGetContinueHandler(pInst, pInst->getEventActivated));
}

CsrUint8 CsrBtBppcCancelJobReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Cancel Job operation. Note can run on the Job and the Status
     Channel */
    CsrBtBppcCancelJobReq *pMsg = (CsrBtBppcCancelJobReq *) msg;
    CsrUint16 contentLength     = CsrBtBppcGetSoapEncodedCancelJobReqContentLength(pMsg->jobId);
    pInst->soapBodyIndex        = 0;
    pInst->jobId                = pMsg->jobId;
    pInst->operationStatus      = CSR_BT_BPPC_OPERATION_STATUS_NOT_FOUND;
    pInst->soapBody             = CsrBtBppcWriteSoapEncodedCancelJobReq(contentLength,
                                                                        pMsg->jobId,
                                                                        &pInst->soapBodyLength);

    return (csrBtBppcGetSoapReqHandler(pInst,
                                       csrBtBppcGetCancelJobIndJobChHandler,
                                       csrBtBppcGetCancelJobIndStatusChHandler,
                                       pInst->getEventActivated));
}

CsrUint8 CsrBtBppcGetEventReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Start the Get Event operation. Note can only run on the Status Channel */
    CsrBtBppcGetEventReq *pMsg = (CsrBtBppcGetEventReq *) msg;
    CsrUint16 contentLength    = CsrBtBppcGetSoapEncodedGetEventReqContentLength(pMsg->jobId);
    pInst->soapBodyIndex       = 0;
    pInst->soapBody            = CsrBtBppcWriteSoapEncodedGetEventReq(contentLength,
                                                                      pMsg->jobId,
                                                                      &pInst->soapBodyLength);

    return (csrBtBppcGetSoapReqHandler(pInst,
                                       csrBtBppcGetEventIndStatusChHandler,
                                       csrBtBppcGetEventIndStatusChHandler,
                                       FALSE));
}

CsrUint8 CsrBtBppcGetEventResHandler(BppcInstanceData *pInst, void *msg)
{ /* In the middle of the Get Event operation. Note can only run on the Status
     Channel */
    CSR_UNUSED(msg);
    return (csrBtBppcGetContinueHandler(pInst, FALSE));
}

CsrUint8 CsrBtBppcActivateReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Activate the Object Channel */
    CsrBtBppcActivateReq *pMsg = (CsrBtBppcActivateReq *) msg;
    pInst->appHandle           = pMsg->appHandle;
    pInst->localMaxPacketLength = pMsg->obexMaxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    return (csrBtBppcActivateObjectChannel(pInst));
}

CsrUint8 CsrBtBppcDeactivateReqHandler(BppcInstanceData *pInst, void *msg)
{ /* Deactivate the Object Channel */
    return (ObexUtilServDeactivateStart(pInst->obexObjectChannelInst,
                                        pInst->objectChLocalServerCh,
                                        L2CA_PSM_INVALID,
                                        csrBtBppcObjectChDeactivateCfmHandler));
}

CsrUint8 CsrBtBppcConnectResHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcConnectRes *pMsg = (CsrBtBppcConnectRes *) msg;
    return (csrBtBppcConnectResponseHandler(pInst,
                                            pMsg->responseCode));
}

CsrUint8 CsrBtBppcGetReferenceObjectResHandler(BppcInstanceData *pInst, void *msg)
{
    CsrBtBppcGetReferenceObjectRes *pMsg = (CsrBtBppcGetReferenceObjectRes *) msg;
    csrBtBppcMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);
    CsrPmemFree(pMsg->fileName);

    if (pInst->objectChAppParamsFound)
    { /* This is a multi operation where the profile already have receive a
         response */
        return (ObexUtilServGetResponse(pInst->obexObjectChannelInst,
                                        pMsg->responseCode,
                                        &pMsg->file,
                                        (CsrUint16) pMsg->count,
                                        CSR_BT_OBEX_SRMP_INVALID));
    }
    else
    { /* The first OBEX response packet has not been sent to the Printer */
        CsrUint16 appParamsHeaderLength;
        CsrUint8  *appParamsHeader    = CsrBtBppcBuildApplicationParameters((CsrUint32) pMsg->fileSize,
                                                                            CSR_BT_JOB_BASED_TRANSFER_MODEL,
                                                                            CSR_BT_BPPC_APPLICATION_PARAMETERS_FILE_SIZE_TAG,
                                                                            &appParamsHeaderLength);
        pInst->objectChAppParamsFound = TRUE;


        return (csrBtBppcObjectChGetWithHeadersResponseHandler(pInst,
                                                               appParamsHeaderLength,
                                                               appParamsHeader,
                                                               CSR_BT_BPPC_OBJECT_CHANNEL_GET_BODY_HEADER_INDEX,
                                                               pMsg->responseCode,
                                                               (CsrUint16) pMsg->count,
                                                               pMsg->file));
    }
}

#ifdef CSR_AMP_ENABLE
static void updateAmpm(BppcInstanceData *pInst)
{
    CsrUint8            slaveCount = 0;
    /* Three channels exist - status, job and object. Therefore max number of slave channels is 2 */
    /* The status channel is regarded as master channel */
    CsrBtConnId   slavePtr[2];

    if (pInst->statusChConnId)
    {
        if (pInst->jobChConnId)
        {
            slavePtr[slaveCount] = pInst->jobChConnId;
            slaveCount++;
        }
        if (pInst->objectChConnId)
        {
            slavePtr[slaveCount] = pInst->objectChConnId;
            slaveCount++;
        }
        if (slaveCount)
        {
            CsrBtAmpmPrfMultiConnReqSend(pInst->statusChConnId, slaveCount, &slavePtr[0]);
        }
        else
        {
            CsrBtAmpmPrfMultiConnReqSend(pInst->statusChConnId, 0, NULL);
        }
    }
    else
    {
        /* Do nothing - master channel is not valid */
    }
}
#endif /* CSR_AMP_ENABLE */


