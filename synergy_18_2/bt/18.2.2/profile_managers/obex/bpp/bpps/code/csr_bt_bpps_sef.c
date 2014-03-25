/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_bpps_handler.h"
#include "csr_bt_bpps_sef.h"
#include "csr_bt_bpps_soapbuilder.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_obex_util.h"
#ifdef CSR_AMP_ENABLE
#include "csr_bt_ampm_private_lib.h"
#endif /* CSR_AMP_ENABLE */
#define CSR_BT_BPPS_RFCOMM_SERVER_CHANNEL_DIRECT_PRINTING_INDEX     0x1B
#define CSR_BT_BPPS_RFCOMM_SERVER_CHANNEL_PRINTING_STATUS_INDEX     0x4E
#define CSR_BT_BPPS_SERVICE_RECORD_ATTRIBUTE_LENGTH                 0x04
#define CSR_BT_BPPS_ZERO_TERMINATION_LENGTH                         0x01
#define CSR_BT_BPPS_UUID_STR_LEN                                    0x10
#define CSR_BT_BPPS_REF_OBJ_TARGET_LEN                              0x10
#define CSR_BT_BPPS_SHORT_UUID_STR_LEN                              0x02
#define CSR_BT_BPPS_HEADER_SIZE                                     0x10
#define CSR_BT_BPPS_OBJECT_CHANNEL_UNKNOWN_FILESIZE_VALUE           (-1)
#define CSR_BT_BPPS_APP_PARAMS_LENGTH                               0x15
#define CSR_BT_BPPS_APP_PARAMS_LENGTH_NO_FILESIZE                   0x0F
#define CSR_BT_BPPS_APP_PARAMS_OFFSET_TAG                           0x01
#define CSR_BT_BPPS_APP_PARAMS_OFFSET_TAG_LENGTH                    0x04
#define CSR_BT_BPPS_APP_PARAMS_COUNT_TAG                            0x02
#define CSR_BT_BPPS_APP_PARAMS_COUNT_TAG_LENGTH                     0x04
#define CSR_BT_BPPS_APP_PARAMS_FILE_SIZE_TAG                        0x04
#define CSR_BT_BPPS_APP_PARAMS_FILE_SIZE_TAG_LENGTH                 0x04
#define CSR_BT_BPPS_APP_PARAMS_JOBID_TAG                            0x03
#define CSR_BT_BPPS_APP_PARAMS_JOBID_TAG_LENGTH                     0x04
#define CSR_BT_BPPS_APP_PARAMS_JOBID_TOTAL_LENGTH                   0x09
#define CSR_BT_BPPS_BUF_SIZE                                        0x0C
#define CSR_BT_BPPS_BYTES_IN_BODY                                   0x05
#define CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE                            0x1000
#define CSR_BT_BPPS_JOB_CH                                          (TRUE)
#define CSR_BT_BPPS_STATUS_CH                                       (FALSE)
#define CSR_BT_BPPS_CREATE_JOB_RESPONSE_BODY_INDEX                  0x01
#define CSR_BT_BPPS_SOAP_ACTION_BODY_INDEX                          0x00
#define CSR_BT_BPPS_MAX_CONNECTION                                  0x03
#define CSR_BT_BPPS_SECONDAR_CHANNEL_SECURITY                      (CSR_BT_SEC_SPECIFY         | \
                                                                   CSR_BT_SEC_AUTHENTICATION  | \
                                                                   CSR_BT_SEC_ENCRYPTION)

#define CSR_BT_BPPS_JOB_ATTR_COMBINED "JobState\0JobName\0JobOriginatingUserName\0JobMediaSheetsCompleted\0NumberOfInterveningJobs\0OperationStatus\0"
#define CSR_BT_BPPS_JOB_ATTR_COMBINED_LENGTH                        0x68

#define CSR_BT_BPPS_REF_OBJ_TARGET                                          {0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB}
static const CsrUint8 jobChTarget[CSR_BT_BPPS_UUID_STR_LEN]               = {0x00, 0x00, 0x11, 0x18, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
static const CsrUint8 statusChTarget[CSR_BT_BPPS_UUID_STR_LEN]            = {0x00, 0x00, 0x11, 0x23, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
static const CsrUint8 jobChShortTarget[CSR_BT_BPPS_SHORT_UUID_STR_LEN]    = {0x11, 0x18};
static const CsrUint8 statusChShortTarget[CSR_BT_BPPS_SHORT_UUID_STR_LEN] = {0x11, 0x23};
static const CsrUint8 CSR_BT_BPPS_REFOBJ_HEADER_TYPE[]                    = {"x-obex/referencedobject"};

#define SERVICE_RECORD_SET_BYTE_AT(dest, index, value) \
(((CsrUint8 *) dest)[(index)] = (value))

#define SERVICE_RECORD_SET_WORD_AT(dest, index, value) \
((((CsrUint8 *) dest)[index] = ((CsrUint8) (((CsrUint16) value) >> 8))) , \
(((CsrUint8 *) dest)[(index) + 1] = ((CsrUint8) (((CsrUint16) value) & 0x00FF))))

#define SERVICE_RECORD_SET_ATTR_ID(dest, attrid) \
SERVICE_RECORD_SET_BYTE_AT(dest, 0, 0x09) , \
SERVICE_RECORD_SET_WORD_AT(dest, 1, attrid) , \
dest += (sizeof(CsrUint8) + sizeof(CsrUint16))

#define SERVICE_RECORD_SET_STRING(dest, string) \
SERVICE_RECORD_SET_BYTE_AT(dest, 0, 0x25) , \
SERVICE_RECORD_SET_BYTE_AT(dest, 1, (CsrUint8) CsrStrLen((char*) string)) , \
CsrMemCpy(dest + 2, string, CsrStrLen((char*) string)) , \
dest += (sizeof(CsrUint8) + sizeof(CsrUint8) + CsrStrLen((char*) string))

#define SERVICE_RECORD_SET_UINT128(dest, value) \
SERVICE_RECORD_SET_BYTE_AT(dest, 0, 0x0C) , \
CsrMemCpy(dest + 1, value, 16) , \
dest += (sizeof(CsrUint8) + 16)

static CsrUint8 csrBtBppsActivateJobCh(BppsInstanceData *pInst);
static CsrUint8 csrBtBppsActivateStatusCh(BppsInstanceData *pInst);
static void csrBtBppsStatusChNonConnectableCfmHandler(void *instData, CsrUint8 serverChannel, psm_t psm);
#ifdef CSR_AMP_ENABLE
static void updateAmpm(BppsInstanceData *pInst);
#endif /* CSR_AMP_ENABLE */

static const CsrUint8 bppServiceRecord[] =
{
    /* service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x23,        /* Service class = PrintingStatus */
    0x19,0x11,0x18,        /* Service class = DirectPrinting */

    /* protocol descriptor list */
    0x09,0x00,0x04,        /* AttrId ProtocolDescriptorList */
    0x35,0x11,             /* 17 bytes in total DataElSeq */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,        /* 2 byte UUID, Protocol = L2CAP */
    0x35,0x05,             /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,        /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,             /* 1 byte UINT - server channel */
    0x35,0x03,             /* 3 bytes in DataElSeq */
    0x19,0x00,0x08,        /* 2 byte UUID, Protocol = OBEX */

    /* language base list */
    0x09,0x00,0x06,        /* AttrId LanguageBaseAttributeIDList. */
    0x35,0x09,             /* 9 bytes in total DataElSeq. */
    0x09,0x65,0x6e,        /* 2 byte CsrUint16, natural language = en (english).*/
    0x09,0x00,0x6a,        /* 2 byte CsrUint16, character encoding = UTF-8. */
    0x09,0x01,0x00,        /* 2 byte CsrUint16, base attr id = 0x0100. */

    /* bluetooth profile descriptor list */
    0x09,0x00,0x09,        /* AttrId, BluetoothProfileDescriptorList */
    0x35, 0x08,            /* 8 bytes in total DataElSeq */
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x22,        /* 2 byte UUID, Service class = BasicPrinting */
    0x09,0x01,0x00,        /* 2 byte UINT, Profile Version = 0x0100 */

    /* additional protocol descriptor list */
    0x09,0x00,0x0d,         /* AttrId ProtocolDescriptorList. */
    0x35,0x13,              /* 19 bytes in total DataElSeq. */
    0x35,0x11,              /* 17 bytes in total DataElSeq */
    0x35,0x03,              /* 3 bytes in DataElSeq */
    0x19,0x01,0x00,         /* 2 byte UUID, Protocol = L2CAP */
    0x35,0x05,              /* 5 bytes in DataElSeq */
    0x19,0x00,0x03,         /* 2 byte UUID Protocol = RFCOMM */
    0x08,0x00,              /* 1 byte UINT - server channel */
    0x35,0x03,              /* 3 bytes in DataElSeq */
    0x19,0x00,0x08,         /* 2 byte UUID, Protocol = OBEX */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x0e,             /* 14 byte string */
    'B', 'A', 'S', 'I', 'C', ' ', 'P', 'R', 'I', 'N', 'T', 'I', 'N', 'G',

    /* BrowseGroupList */
    0x09, 0x00, 0x05,       /* AttrId = BrowseGroupList */
    0x35, 0x03,             /* Data element seq. 3 bytes */
    0x19, 0x10, 0x02,       /* 2 byte UUID, PublicBrowseGroup = 0x1002 */
};

/* Static upstream message library functions */
static void csrBtBppsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_BPPS_PRIM, msg);
}

static void csrBtBppsActivateCfmSend(CsrSchedQid appHandle)
{
    CsrBtBppsActivateCfm *pMsg = (CsrBtBppsActivateCfm *)
                                    CsrPmemAlloc(sizeof(CsrBtBppsActivateCfm));

    pMsg->type = CSR_BT_BPPS_ACTIVATE_CFM;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsDeactivateCfmSend(CsrSchedQid appHandle)
{
    CsrBtBppsDeactivateCfm *pMsg = (CsrBtBppsDeactivateCfm *)
                                    CsrPmemAlloc(sizeof(CsrBtBppsDeactivateCfm));

    pMsg->type = CSR_BT_BPPS_DEACTIVATE_CFM;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsConnectIndSend(CsrSchedQid          appHandle,
                                    CsrUint32       connectionId,
                                    CsrUint16       maxPeerPacketSize,
                                    CsrBtDeviceAddr *deviceAddr,
                                    CsrUint32       length,
                                    CsrUint32       count,
                                    CsrBtConnId     btConnId)
{
    CsrBtBppsConnectInd *pMsg   = (CsrBtBppsConnectInd *)
                                   CsrPmemAlloc(sizeof(CsrBtBppsConnectInd));

    pMsg->type                  = CSR_BT_BPPS_CONNECT_IND;
    pMsg->connectionId          = connectionId;
    pMsg->obexPeerMaxPacketSize = maxPeerPacketSize;
    pMsg->deviceAddr            = *deviceAddr;
    pMsg->length                = length;
    pMsg->count                 = count;
    pMsg->btConnId              = btConnId;

    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsAuthenticateCfmSend(CsrSchedQid appHandle)
{
    CsrBtBppsAuthenticateCfm *pMsg = (CsrBtBppsAuthenticateCfm *)
                                      CsrPmemAlloc(sizeof(CsrBtBppsAuthenticateCfm));

    pMsg->type              = CSR_BT_BPPS_AUTHENTICATE_CFM;
    pMsg->resultCode        = CSR_BT_RESULT_CODE_OBEX_SUCCESS;
    pMsg->resultSupplier    = CSR_BT_SUPPLIER_OBEX_PROFILES;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsAuthenticateIndSend(CsrSchedQid             appHandle,
                                         CsrBtDeviceAddr    deviceAddr,
                                         CsrUint8           options,
                                         CsrUint16          realmLength,
                                         CsrUint8         *realm)
{
    CsrBtBppsAuthenticateInd *pMsg = (CsrBtBppsAuthenticateInd*)
                                      CsrPmemAlloc(sizeof(CsrBtBppsAuthenticateInd));

    pMsg->type          = CSR_BT_BPPS_AUTHENTICATE_IND;
    pMsg->deviceAddr    = deviceAddr;
    pMsg->options       = options;
    pMsg->realmLength   = realmLength;
    pMsg->realm         = realm;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsNextIndSend(CsrSchedQid     appHandle,
                                 CsrUint32  connectionId,
                                 CsrBool    finalFlag,
                                 CsrUint8 *documentBody,
                                 CsrUint16  documentBodyLength)
{

    CsrBtBppsNextInd *pMsg   = (CsrBtBppsNextInd *)
                                CsrPmemAlloc(sizeof(CsrBtBppsNextInd));

    pMsg->type          = CSR_BT_BPPS_NEXT_IND;
    pMsg->connectionId  = connectionId;
    pMsg->finalFlag     = finalFlag;
    pMsg->body          = documentBody;
    pMsg->bodyLength    = documentBodyLength;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsAbortIndSend(CsrSchedQid     appHandle,
                                  CsrUint32  connectionId)
{
    CsrBtBppsAbortInd *pMsg = (CsrBtBppsAbortInd *)
                               CsrPmemAlloc(sizeof(CsrBtBppsAbortInd));

    pMsg->type          = CSR_BT_BPPS_ABORT_IND;
    pMsg->connectionId  = connectionId;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsDisconnectIndSend(CsrSchedQid           appHandle,
                                       CsrUint32        connectionId,
                                       CsrBtReasonCode  reasonCode,
                                       CsrBtSupplier    reasonSupplier)
{
    CsrBtBppsDisconnectInd *pMsg = (CsrBtBppsDisconnectInd *)
                                    CsrPmemAlloc(sizeof(CsrBtBppsDisconnectInd));

    pMsg->type              = CSR_BT_BPPS_DISCONNECT_IND;
    pMsg->connectionId      = connectionId;
    pMsg->reasonCode        = reasonCode;
    pMsg->reasonSupplier    = reasonSupplier;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsCreateJobIndSend(CsrSchedQid    appHandle,
                                      CsrUtf8String *jobName,
                                      CsrUtf8String *jobOriginatingUserName,
                                      CsrUtf8String *documentFormat,
                                      CsrUtf8String *copies,
                                      CsrUtf8String *sides,
                                      CsrUtf8String *numberUp,
                                      CsrUtf8String *orientationRequested,
                                      CsrUtf8String *mediaSize,
                                      CsrUtf8String *mediaType,
                                      CsrUtf8String *printQuality,
                                      CsrUtf8String *cancelOnLostLink)
{
    CsrBtBppsCreateJobInd *pMsg     = (CsrBtBppsCreateJobInd *)
                                       CsrPmemAlloc(sizeof(CsrBtBppsCreateJobInd));

    pMsg->type                      = CSR_BT_BPPS_CREATE_JOB_IND;
    pMsg->jobName                   = jobName;
    pMsg->jobOriginatingUserName    = jobOriginatingUserName;
    pMsg->documentFormat            = documentFormat;
    pMsg->copies                    = copies;
    pMsg->sides                     = sides;
    pMsg->numberUp                  = numberUp;
    pMsg->orientationRequested      = orientationRequested;
    pMsg->mediaSize                 = mediaSize;
    pMsg->mediaType                 = mediaType;
    pMsg->printQuality              = printQuality;
    pMsg->cancelOnLostLink          = cancelOnLostLink;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsSendDocumentIndSend(CsrSchedQid                   appHandle,
                                         CsrBtBppsSendDocumentInd *prim)
{
    CsrBtBppsSendDocumentInd *pMsg  = (CsrBtBppsSendDocumentInd *)
                                       CsrPmemAlloc(sizeof(CsrBtBppsSendDocumentInd));

    pMsg->type                       = CSR_BT_BPPS_SEND_DOCUMENT_IND;
    pMsg->documentTypeLength         = prim->documentTypeLength;
    pMsg->documentType               = prim->documentType;
    pMsg->jobId                      = prim->jobId;
    pMsg->fileName                   = prim->fileName;
    pMsg->documentBody               = prim->documentBody;
    pMsg->documentBodyLength         = prim->documentBodyLength;
    pMsg->docTypeDependentInfo       = prim->docTypeDependentInfo;
    pMsg->docTypeDependentInfoLength = prim->docTypeDependentInfoLength;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsGetPrinterAttribsIndSend(CsrSchedQid    appHandle,
                                              CsrBool   pageHeight,
                                              CsrBool   pageWidth,
                                              CsrBool   color,
                                              CsrBool   documentFormats,
                                              CsrBool   imageFormats,
                                              CsrBool   maxCopies,
                                              CsrBool   mediaLoaded,
                                              CsrBool   mediaSizes,
                                              CsrBool   mediaTypes,
                                              CsrBool   numberUp,
                                              CsrBool   orientations,
                                              CsrBool   currentOperator,
                                              CsrBool   printerLocation,
                                              CsrBool   printerName,
                                              CsrBool   printerState,
                                              CsrBool   printerReasons,
                                              CsrBool   printQuality,
                                              CsrBool   queuedJobCount,
                                              CsrBool   sides)
{
    CsrBtBppsGetPrinterAttribsInd * pMsg = (CsrBtBppsGetPrinterAttribsInd *)
                                            CsrPmemAlloc(sizeof(CsrBtBppsGetPrinterAttribsInd));

    pMsg->type                          = CSR_BT_BPPS_GET_PRINTER_ATTRIBS_IND;
    pMsg->basicTextPageHeight           = pageHeight;
    pMsg->basicTextPageWidth            = pageWidth;
    pMsg->colorSupported                = color;
    pMsg->documentFormatsSupported      = documentFormats;
    pMsg->imageFormatsSupported         = imageFormats;
    pMsg->maxCopiesSupported            = maxCopies;
    pMsg->mediaLoaded                   = mediaLoaded;
    pMsg->mediaSizesSupported           = mediaSizes;
    pMsg->mediaTypesSupported           = mediaTypes;
    pMsg->numberUpSupported             = numberUp;
    pMsg->orientationsSupported         = orientations;
    pMsg->printerGeneralCurrentOperator = currentOperator;
    pMsg->printerLocation               = printerLocation;
    pMsg->printerName                   = printerName;
    pMsg->printerState                  = printerState;
    pMsg->printerStateReasons           = printerReasons;
    pMsg->printQualitySupported         = printQuality;
    pMsg->queuedJobCount                = queuedJobCount;
    pMsg->sidesSupported                = sides;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsGetJobAttribsIndSend(CsrSchedQid    appHandle,
                                          CsrUint32 jobId,
                                          CsrUint8  *jobAttribs,
                                          CsrUint8  jobAttribsLength)
{
    CsrUint8  i;
    CsrUint32 tempLength            = 0;
    CsrUint8                  *ptr  = jobAttribs;
    CsrBtBppsGetJobAttribsInd *pMsg = (CsrBtBppsGetJobAttribsInd *)
                                       CsrPmemAlloc(sizeof(CsrBtBppsGetJobAttribsInd));

    pMsg->type               = CSR_BT_BPPS_GET_JOB_ATTRIBS_IND;
    pMsg->jobId              = jobId;
    pMsg->jobAttribs         = jobAttribs;
    pMsg->jobAttribsCount    = 0;
    pMsg->jobAttribsLength   = 0;

    for(i = 0; tempLength < jobAttribsLength; i++)
    {
        pMsg->jobAttribsLength = (CsrUint16)(pMsg->jobAttribsLength +
                                             CsrStrLen((char*)ptr)  +
                                             CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);

        tempLength += CsrStrLen((char*)ptr) + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH;
        ptr += CsrStrLen((char*)ptr) + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH;
        ++pMsg->jobAttribsCount;
    }
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsCancelJobIndSend(CsrSchedQid    appHandle,
                                      CsrUint32 jobId)
{
    CsrBtBppsCancelJobInd *pMsg = (CsrBtBppsCancelJobInd *)
                                   CsrPmemAlloc(sizeof(CsrBtBppsCancelJobInd));

    pMsg->type  = CSR_BT_BPPS_CANCEL_JOB_IND;
    pMsg->jobId = jobId;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsGetReferencedObjIndSend(CsrSchedQid     appHandle,
                                             CsrUint8 *obexData,
                                             CsrInt32   fileSize,
                                             CsrUint16  objectOffset,
                                             CsrUint16  objectLength,
                                             CsrUint16  obexDataLength)
{
    CsrBtBppsGetReferencedObjInd *pMsg = (CsrBtBppsGetReferencedObjInd *)
                                          CsrPmemAlloc(sizeof(CsrBtBppsGetReferencedObjInd));

    pMsg->type          = CSR_BT_BPPS_GET_REFERENCED_OBJ_IND;
    pMsg->payloadLength = obexDataLength;
    pMsg->payload       = obexData;
    pMsg->bodyLength    = objectLength;
    pMsg->bodyOffset    = objectOffset;
    pMsg->filesize      = fileSize;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsGetReferencedObjCfmSend(CsrSchedQid             appHandle,
                                             void               *pMsg)
{
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsGetEventIndSend(CsrSchedQid     appHandle,
                                     CsrUint32  jobId,
                                     CsrBool    updateEvents)
{
    CsrBtBppsGetEventInd *pMsg = (CsrBtBppsGetEventInd *)
                                  CsrPmemAlloc(sizeof(CsrBtBppsGetEventInd));

    pMsg->type          = CSR_BT_BPPS_GET_EVENT_IND;
    pMsg->jobId         = jobId;
    pMsg->updateEvents  = updateEvents;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsJobCompleteCfmSend(CsrSchedQid appHandle)
{
    CsrBtBppsJobCompleteCfm *pMsg  = (CsrBtBppsJobCompleteCfm *)
                                      CsrPmemAlloc(sizeof(CsrBtBppsJobCompleteCfm));

    pMsg->type = CSR_BT_BPPS_JOB_COMPLETE_CFM;
    csrBtBppsMessagePut(appHandle, pMsg);
}

static void csrBtBppsSecurityInCfmSend(CsrSchedQid          appHandle,
                                       CsrBtResultCode resultCode)
{
    CsrBtBppsSecurityInCfm *pMsg = (CsrBtBppsSecurityInCfm*)
                                    CsrPmemAlloc(sizeof(CsrBtBppsSecurityInCfm));

    pMsg->type              = CSR_BT_BPPS_SECURITY_IN_CFM;
    pMsg->resultCode        = resultCode;
    pMsg->resultSupplier    = CSR_BT_SUPPLIER_OBEX_PROFILES;
    csrBtBppsMessagePut(appHandle, pMsg);
}

/* Static general help functions */
static void * csrBtBppsBuildGetReferencedObjCfmMsg(CsrUint8      *obexData,
                                                   CsrInt32        fileSize,
                                                   CsrUint16       objectOffset,
                                                   CsrUint16       objectLength,
                                                   CsrBtResultCode resultCode,
                                                   CsrBtSupplier   resultSupplier,
                                                   CsrUint16       obexDataLength)
{
    CsrBtBppsGetReferencedObjCfm *pMsg = (CsrBtBppsGetReferencedObjCfm *)
                                          CsrPmemAlloc(sizeof(CsrBtBppsGetReferencedObjCfm));

    pMsg->type              = CSR_BT_BPPS_GET_REFERENCED_OBJ_CFM;
    pMsg->payloadLength     = obexDataLength;
    pMsg->payload           = obexData;
    pMsg->filesize          = fileSize;
    pMsg->bodyOffset        = objectOffset;
    pMsg->bodyLength        = objectLength;
    pMsg->resultCode        = resultCode;
    pMsg->resultSupplier    = resultSupplier;
    return(pMsg);
}


static void csrBtBppsCreateServiceRecord(BppsInstanceData   *pInst,
                                         CsrUtf8String      *documentFormatsSupported,
                                         CsrUint8           *characterRepertoiresSupported,
                                         CsrUint16          characterRepertoiresSupportedLength,
                                         CsrUtf8String      *imageFormatsSupported,
                                         CsrUtf8String      *ieee1284Id)
{
    CsrUint8 *pRecordPosn      = NULL;
    pInst->serviceRecordLength = (CsrUint16) sizeof(bppServiceRecord);

    if (documentFormatsSupported)
    {
        pInst->serviceRecordLength = (CsrUint16) (pInst->serviceRecordLength                   +
                                                  CSR_BT_BPPS_SERVICE_RECORD_ATTRIBUTE_LENGTH  +
                                                  CsrStrLen((char *) documentFormatsSupported) +
                                                  CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
    }
    if (characterRepertoiresSupported)
    {
        pInst->serviceRecordLength = (CsrUint16) (pInst->serviceRecordLength                   +
                                                  CSR_BT_BPPS_SERVICE_RECORD_ATTRIBUTE_LENGTH  +
                                                   characterRepertoiresSupportedLength);
    }
    if (imageFormatsSupported)
    {
        pInst->serviceRecordLength = (CsrUint16) (pInst->serviceRecordLength                   +
                                                  CSR_BT_BPPS_SERVICE_RECORD_ATTRIBUTE_LENGTH  +
                                                  CsrStrLen((char *) imageFormatsSupported)    +
                                                  CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
    }
    if (ieee1284Id)
    {
        pInst->serviceRecordLength = (CsrUint16) (pInst->serviceRecordLength                   +
                                                  CSR_BT_BPPS_SERVICE_RECORD_ATTRIBUTE_LENGTH  +
                                                  CsrStrLen((char *) ieee1284Id)               +
                                                  CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
    }

    pInst->serviceRecord = (CsrUint8 *) CsrPmemAlloc(pInst->serviceRecordLength);
    pRecordPosn          = pInst->serviceRecord;
    CsrMemCpy(pInst->serviceRecord, bppServiceRecord, sizeof(bppServiceRecord));
    pRecordPosn         += sizeof(bppServiceRecord);

    if (documentFormatsSupported)
    {
        SERVICE_RECORD_SET_ATTR_ID(pRecordPosn, CSR_BT_DOCUMENT_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER);
        SERVICE_RECORD_SET_STRING(pRecordPosn, documentFormatsSupported);
    }

    if (characterRepertoiresSupported)
    {
        SERVICE_RECORD_SET_ATTR_ID(pRecordPosn, CSR_BT_CHARACTER_REPERTOIRES_SUPPORTED_ATTRIBUTE_IDENTIFIER);
        SERVICE_RECORD_SET_UINT128(pRecordPosn, characterRepertoiresSupported);

    }
    if (imageFormatsSupported)
    {
        SERVICE_RECORD_SET_ATTR_ID(pRecordPosn, CSR_BT_XHTML_PRINT_IMAGE_FORMATS_SUPPORTED_ATTRIBUTE_IDENTIFIER);
        SERVICE_RECORD_SET_STRING(pRecordPosn, imageFormatsSupported);
    }
    if (ieee1284Id)
    {
        SERVICE_RECORD_SET_ATTR_ID(pRecordPosn, CSR_BT_PRINTER_1284ID_ATTRIBUTE_IDENTIFIER);
        SERVICE_RECORD_SET_STRING(pRecordPosn, ieee1284Id);
    }
}

static CsrUint8 csrBtBppsCheckTargetHeader(CsrUint8   channel,
                                           CsrUint8   *target,
                                           CsrUint16  targetHeaderLength)
{

    if (channel == CSR_BT_BPPS_JOB_CHANNEL_INST_ID)
    { /* Check the Job Channels Target header */
        if (target && targetHeaderLength == CSR_BT_BPPS_UUID_STR_LEN)
        {
            if (!CsrMemCmp(target, jobChTarget, CSR_BT_BPPS_UUID_STR_LEN))
            {
                return (CSR_BT_BPPS_JOB_CHANNEL_TARGET);
            }
        }
        else if (target && targetHeaderLength == CSR_BT_BPPS_SHORT_UUID_STR_LEN)
        {
            if (!CsrMemCmp(target, jobChShortTarget, CSR_BT_BPPS_SHORT_UUID_STR_LEN))
            {
                return (CSR_BT_BPPS_JOB_CHANNEL_SHORT_TARGET);
            }
        }
    }
    else
    { /* Check the Job Channels Target header */
        if (target && targetHeaderLength == CSR_BT_BPPS_UUID_STR_LEN)
        {
            if (!CsrMemCmp(target, statusChTarget, CSR_BT_BPPS_UUID_STR_LEN))
            {
                return (CSR_BT_BPPS_STATUS_CHANNEL_TARGET);
            }
        }
        else if (target && targetHeaderLength == CSR_BT_BPPS_SHORT_UUID_STR_LEN)
        {
            if (!CsrMemCmp(target, statusChShortTarget, CSR_BT_BPPS_SHORT_UUID_STR_LEN))
            {
                return (CSR_BT_BPPS_STATUS_CHANNEL_SHORT_TARGET);
            }
        }
    }
    return (CSR_BT_BPPS_INVALID_TARGET);
}

static void csrBtBppsSetJobChConnectAble(BppsInstanceData *pInst)
{
    CsrUint8 status     = ObexUtilServConnectableStart(pInst->obexJobChannelInst,
                                                   CSR_BT_BPPS_PROFILE_DEFAULT_MTU_SIZE,
                                                   pInst->maxLocalObexPacketLength);

    pInst->jobChState   = CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE;
    csrBtBppsActivateCfmSend(pInst->appHandle);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServConnectableStart on jobCh");
    }
}

static CsrUint8 csrBtBppsConnectResponseHandler(void                         *channelInst,
                                                CsrUint8                     targetId,
                                                CsrUint32                    connectionId,
                                                CsrBtObexResponseCode        responseCode,
                                                ObexUtilDigestChallengeType  *digestChallenge,
                                                CsrUcs2String                *description)
{
    CsrUint8  *who                   = NULL;
    CsrUint16 whoHeaderLength        = 0;
    CsrUint8  numOfHeaders           = 0;
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);

    switch (targetId)
    {
        case CSR_BT_BPPS_JOB_CHANNEL_TARGET:
        {
            whoHeaderLength = CSR_BT_BPPS_UUID_STR_LEN;
            who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
            CsrMemCpy(who, jobChTarget, whoHeaderLength);
            break;
        }
        case CSR_BT_BPPS_JOB_CHANNEL_SHORT_TARGET:
        {
            whoHeaderLength = CSR_BT_BPPS_SHORT_UUID_STR_LEN;
            who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
            CsrMemCpy(who, jobChShortTarget, whoHeaderLength);
            break;
        }
        case CSR_BT_BPPS_STATUS_CHANNEL_TARGET:
        {
            whoHeaderLength = CSR_BT_BPPS_UUID_STR_LEN;
            who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
            CsrMemCpy(who, statusChTarget, whoHeaderLength);
            break;
        }
        case CSR_BT_BPPS_STATUS_CHANNEL_SHORT_TARGET:
        {
            whoHeaderLength = CSR_BT_BPPS_SHORT_UUID_STR_LEN;
            who             = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
            CsrMemCpy(who, statusChShortTarget, whoHeaderLength);
            break;
        }
        default:
        { /* Just ignore the who pointer is already set to NULL */
            break;
        }
    }
    if (description)
    {
        numOfHeaders = 1;
    }
    return (ObexUtilServConnectResponse(channelInst,
                                        responseCode,
                                        whoHeaderLength,
                                        &who,
                                        connectionId,
                                        numOfHeaders,
                                        headerPriorityTable,
                                        &digestChallenge,
                                        &description,
                                        0,
                                        NULL,
                                        FALSE));
}

static void csrBtBppsChannelRelease(BppsInstanceData *pInst)
{
    if (pInst->numOfConn > 0)
    {
        pInst->numOfConn = (CsrUint8)(pInst->numOfConn - 1);

        if (pInst->jobCompletion)
        {
            csrBtBppsJobCompleteCfmSend(pInst->appHandle);
            pInst->jobCompletion = FALSE;
        }
    }
}

static void csrBtBppsChannelConnected(BppsInstanceData *pInst)
{
    if (pInst->numOfConn < CSR_BT_BPPS_MAX_CONNECTION)
    {
        pInst->numOfConn = (CsrUint8)(pInst->numOfConn + 1);
    }
}

static CsrUcs2String * csrBtBppsObjectChGetRefObjName(BppsInstanceData *pInst)
{
    CsrUcs2String *refObjName = NULL;

    if (pInst->savedObjectChDownStreamMsg)
    {
        CsrBtBppsGetReferencedObjReq *pMsg = (CsrBtBppsGetReferencedObjReq *) pInst->savedObjectChDownStreamMsg;
        if (pMsg->objName)
        {
            refObjName = CsrUtf82Ucs2ByteString((CsrUint8*)pMsg->objName);
        }
    }
    return (refObjName);
}

static CsrUint8 * csrBtBppsObjectChBuildAppParamsGetRefObjHeader(BppsInstanceData *pInst,
                                                                 CsrUint16        *applicationParametersHeaderLength)
{
    CsrUint8 * applicationHeader        = NULL;
    *applicationParametersHeaderLength  = 0;

    if (pInst->savedObjectChDownStreamMsg)
    {
        CsrUint8 length;
        CsrBtBppsGetReferencedObjReq *pMsg  = (CsrBtBppsGetReferencedObjReq *) pInst->savedObjectChDownStreamMsg;

        if (pMsg->getFileSize)
        {
            length = CSR_BT_BPPS_APP_PARAMS_LENGTH;
        }
        else
        {
            length = CSR_BT_BPPS_APP_PARAMS_LENGTH_NO_FILESIZE;
        }

        applicationHeader                   = (CsrUint8 *) CsrPmemAlloc(length);
        applicationHeader[0]                = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
        applicationHeader[1]                = 0x00;
        applicationHeader[2]                = length;
        applicationHeader[3]                = CSR_BT_BPPS_APP_PARAMS_OFFSET_TAG;
        applicationHeader[4]                = CSR_BT_BPPS_APP_PARAMS_OFFSET_TAG_LENGTH;
        applicationHeader[5]                = (CsrUint8)(pMsg->offset >> 24);
        applicationHeader[6]                = (CsrUint8)(pMsg->offset >> 16);
        applicationHeader[7]                = (CsrUint8)(pMsg->offset >> 8);
        applicationHeader[8]                = (CsrUint8)(pMsg->offset);
        applicationHeader[9]                = CSR_BT_BPPS_APP_PARAMS_COUNT_TAG;
        applicationHeader[10]               = CSR_BT_BPPS_APP_PARAMS_COUNT_TAG_LENGTH;
        applicationHeader[11]               = (CsrUint8)(pMsg->count >> 24);
        applicationHeader[12]               = (CsrUint8)(pMsg->count >> 16);
        applicationHeader[13]               = (CsrUint8)(pMsg->count >> 8);
        applicationHeader[14]               = (CsrUint8)(pMsg->count);

        if (pMsg->getFileSize)
        {
            applicationHeader[15]           = CSR_BT_BPPS_APP_PARAMS_FILE_SIZE_TAG;
            applicationHeader[16]           = CSR_BT_BPPS_APP_PARAMS_FILE_SIZE_TAG_LENGTH;
            applicationHeader[17]           = 0x00;
            applicationHeader[18]           = 0x00;
            applicationHeader[19]           = 0x00;
            applicationHeader[20]           = 0x00;
        }
        *applicationParametersHeaderLength  = length;
    }
    return (applicationHeader);
}

static CsrUint8 * csrBtBppsBuildAppParamsJobId(CsrUint32        jobId,
                                               CsrUint16        *applicationParametersHeaderLength)
{
    CsrUint8 * applicationHeader        = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BPPS_APP_PARAMS_JOBID_TOTAL_LENGTH);
    applicationHeader[0]                = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
    applicationHeader[1]                = 0x00;
    applicationHeader[2]                = CSR_BT_BPPS_APP_PARAMS_JOBID_TOTAL_LENGTH;
    applicationHeader[3]                = CSR_BT_BPPS_APP_PARAMS_JOBID_TAG;
    applicationHeader[4]                = CSR_BT_BPPS_APP_PARAMS_JOBID_TAG_LENGTH;
    applicationHeader[5]                = (CsrUint8)(jobId >> 24);
    applicationHeader[6]                = (CsrUint8)(jobId >> 16);
    applicationHeader[7]                = (CsrUint8)(jobId >>  8);
    applicationHeader[8]                = (CsrUint8) jobId;
    *applicationParametersHeaderLength  = CSR_BT_BPPS_APP_PARAMS_JOBID_TOTAL_LENGTH;
    return (applicationHeader);
}

/* Static functions to parse a SOAP action command */
CsrBool csrBtBppsSoapValueExists(CsrUint8 *soapString, char *soapTagValue)
{
    if (CsrStrStr((char *) soapString, soapTagValue))
    {
        return (TRUE);
    }
    return (FALSE);
}

CsrUtf8String *csrBtBppsSoapGetValue(CsrUint8 *soapString, char *soapTagId)
{
    CsrCharString  soapTag[30];
    CsrCharString *soapValueStart;
    CsrCharString *soapValueEnd;
    CsrCharString  soapTagValue[500];
    CsrCharString *result = NULL;
    CsrUint16 index = 0;

    CsrStrCpy(soapTag, "<");
    CsrStrCat(soapTag, soapTagId);
    CsrStrCat(soapTag, ">");

    soapValueStart = (char*)CsrStrStr((char*) soapString, soapTag) + CsrStrLen(soapTag);

    if (soapValueStart)
    {
        CsrStrCpy(soapTag, "</");
        CsrStrCat(soapTag, soapTagId);
        CsrStrCat(soapTag, ">");

        soapValueEnd = (char*)CsrStrStr((char*) soapString, soapTag);

        if (soapValueEnd)
        {
            soapValueEnd--;

            do
            {
                if ((*soapValueStart >= 20) && (*soapValueStart <= 125))
                {
                    soapTagValue[index++] = *soapValueStart;
                }
            } while (soapValueStart++ < soapValueEnd);

            soapTagValue[index] = 0;
            result = (CsrCharString*)CsrPmemAlloc(CsrStrLen(soapTagValue)
                                                  + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
            CsrStrCpy(result, soapTagValue);
        }
    }
    return (CsrUtf8String*)result;
}

CsrUint8 * csrBtBppsSoapGetMultiValues(CsrUint8 *soapString,
                                       CsrCharString *outerSoapTagId,
                                       CsrCharString *innerSoapTagId, 
                                       CsrUint16 *number)
{
    CsrUtf8String *tempMemoryReference;
    CsrUtf8String *soapPortion;
    CsrUtf8String *soapValue;
    CsrUtf8String *soapMultiValues = NULL;
    CsrUint16   index = 0;

    *number = 0;

    tempMemoryReference = csrBtBppsSoapGetValue(soapString, outerSoapTagId);

    if (tempMemoryReference)
    {
        CsrUint16 soapValueLength;
        soapPortion = tempMemoryReference;
        /* Count number of bytes needed */
        while( (soapValue = csrBtBppsSoapGetValue((CsrUint8*)soapPortion, innerSoapTagId))
               != NULL)
        {
            soapValueLength = (CsrUint16)(CsrStrLen((char *)soapValue));
            *number = (CsrUint16)(*number + soapValueLength + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
            soapPortion += (soapValueLength + CsrStrLen("<JobAttribute>") + CsrStrLen("</JobAttribute>"));
            CsrPmemFree(soapValue);
        }

        /* Transfer data */
        soapPortion = tempMemoryReference;
        soapMultiValues = (CsrUtf8String*)CsrPmemAlloc(*number);
        index = 0;
        while ((soapValue = csrBtBppsSoapGetValue((CsrUint8*)soapPortion, innerSoapTagId)) != NULL)
        {
            CsrStrCpy((char*)(soapMultiValues + index), (char*)soapValue);
            soapValueLength = (CsrUint16) (CsrStrLen((char *)soapValue));
            index = (CsrUint16)(index + soapValueLength + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
            soapPortion += (CsrStrLen(innerSoapTagId) + 2 + soapValueLength + CsrStrLen(innerSoapTagId) + 3);
            CsrPmemFree(soapValue);
        }
        CsrPmemFree(tempMemoryReference);
    }
    return (CsrUint8*)soapMultiValues;
}

static void csrBtBppsSoapActionCreateJobIndHandler(CsrSchedQid appHandle, CsrUint8 *soapString)
{
    csrBtBppsCreateJobIndSend(appHandle,
                              csrBtBppsSoapGetValue(soapString, "JobName"),
                              csrBtBppsSoapGetValue(soapString, "JobOriginatingUserName"),
                              csrBtBppsSoapGetValue(soapString, "DocumentFormat"),
                              csrBtBppsSoapGetValue(soapString, "Copies"),
                              csrBtBppsSoapGetValue(soapString, "Sides"),
                              csrBtBppsSoapGetValue(soapString, "NumberUp"),
                              csrBtBppsSoapGetValue(soapString, "OrientationRequested"),
                              csrBtBppsSoapGetValue(soapString, "MediaSize"),
                              csrBtBppsSoapGetValue(soapString, "MediaType"),
                              csrBtBppsSoapGetValue(soapString, "PrintQuality"),
                              csrBtBppsSoapGetValue(soapString, "CancelOnLostLink"));
}

static void csrBtBppsSoapActionGetPrinterAttribsIndHandler(CsrSchedQid appHandle, CsrUint8 *soapString)
{
    csrBtBppsGetPrinterAttribsIndSend(appHandle,
                                      csrBtBppsSoapValueExists(soapString, "BasicTextPageHeight"),
                                      csrBtBppsSoapValueExists(soapString, "BasicTextPageWidth"),
                                      csrBtBppsSoapValueExists(soapString, "ColorSupported"),
                                      csrBtBppsSoapValueExists(soapString, "DocumentFormatsSupported"),
                                      csrBtBppsSoapValueExists(soapString, "ImageFormatsSupported"),
                                      csrBtBppsSoapValueExists(soapString, "MaxCopiesSupported"),
                                      csrBtBppsSoapValueExists(soapString, "MediaLoaded"),
                                      csrBtBppsSoapValueExists(soapString, "MediaSizesSupported"),
                                      csrBtBppsSoapValueExists(soapString, "MediaTypesSupported"),
                                      csrBtBppsSoapValueExists(soapString, "NumberUpSupported"),
                                      csrBtBppsSoapValueExists(soapString, "OrientationsSupported"),
                                      csrBtBppsSoapValueExists(soapString, "PrinterGeneralCurrentOperator"),
                                      csrBtBppsSoapValueExists(soapString, "PrinterLocation"),
                                      csrBtBppsSoapValueExists(soapString, "PrinterName"),
                                      csrBtBppsSoapValueExists(soapString, "PrinterState"),
                                      csrBtBppsSoapValueExists(soapString, "PrinterStateReasons"),
                                      csrBtBppsSoapValueExists(soapString, "PrintQualitySupported"),
                                      csrBtBppsSoapValueExists(soapString, "QueuedJobCount"),
                                      csrBtBppsSoapValueExists(soapString, "SidesSupported"));
}

static void csrBtBppsSoapActionGetJobAttribsIndHandler(CsrSchedQid appHandle, CsrUint8 *soapString)
{
    CsrUint16 jobAttribsLength;
    CsrUint32 jobId      = 0;
    CsrUtf8String *tempString = csrBtBppsSoapGetValue(soapString, "JobId");

    if (tempString)
    {
        jobId = CsrStrToInt((const char *)tempString);
        CsrPmemFree(tempString);
    }

    tempString = (CsrUtf8String*)csrBtBppsSoapGetMultiValues(soapString,
                                                             "RequestedJobAttributes",
                                                             "JobAttribute",
                                                             &jobAttribsLength);

    if (jobAttribsLength == 0)
    {
        CsrPmemFree(tempString);
        tempString = (CsrUtf8String*) CsrPmemAlloc(CSR_BT_BPPS_JOB_ATTR_COMBINED_LENGTH);
        CsrMemCpy(tempString, CSR_BT_BPPS_JOB_ATTR_COMBINED, CSR_BT_BPPS_JOB_ATTR_COMBINED_LENGTH);
    }
    csrBtBppsGetJobAttribsIndSend(appHandle, jobId,
                                  (CsrUint8*)tempString,
                                  (CsrUint8)jobAttribsLength);
}

static void csrBtBppsSoapActionCancelJobIndHandler(CsrSchedQid appHandle, CsrUint8 *soapString)
{
    CsrUint32 jobId       = 0;
    CsrUtf8String *jobIdString = csrBtBppsSoapGetValue(soapString, "JobId");

    if (jobIdString)
    {
        jobId = CsrStrToInt((const char *)jobIdString);
        CsrPmemFree(jobIdString);
    }
    csrBtBppsCancelJobIndSend(appHandle, jobId);
}

static void csrBtBppsSoapActionGetEventIndHandler(BppsInstanceData *pInst, CsrUint8 *soapString)
{
    CsrUtf8String *jobIdString = csrBtBppsSoapGetValue(soapString, "JobId");
    pInst->statusChGetEventJobId = 0;

    if (jobIdString)
    {
        pInst->statusChGetEventJobId = CsrStrToInt((const char *)jobIdString);
        CsrPmemFree(jobIdString);
    }
    pInst->statusChGetEventActive = TRUE;
    pInst->statusChGetReadyToSend = TRUE;
    csrBtBppsGetEventIndSend(pInst->appHandle, pInst->statusChGetEventJobId, TRUE);
}

static CsrUint8 csrBtBppsGetWithHeadersResponseHandler(void                         *channel,
                                                       CsrUint8                     bodyHeaderPriorityIndex,
                                                       CsrBtObexResponseCode        responseCode,
                                                       CsrUint16                    appParametersHeaderLength,
                                                       CsrUint8                     *appParametersHeader,
                                                       CsrUint16                    bodyLength,
                                                       CsrUint8                     *body,
                                                       ObexUtilDigestChallengeType  *digestChallenge)
{
    CsrUint8 headerPriorityTable[6];
    CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

    if (appParametersHeader)
    {
        headerPriorityTable[0] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
    }

    return (ObexUtilServGetWithHeaderResponse(channel,
                                              responseCode,
                                              &digestChallenge,
                                              headerPriorityTable,
                                              bodyHeaderPriorityIndex,
                                              0,
                                              NULL,
                                              NULL,
                                              appParametersHeaderLength,
                                              &appParametersHeader,
                                              0,
                                              NULL,
                                              &body,
                                              bodyLength,
                                              CSR_BT_OBEX_SRMP_INVALID));
}


static CsrUint8 csrBtBppsSoapActionTypeHandler(BppsInstanceData *pInst,
                                               CsrBool          channelId,
                                               void             *channel,
                                               CsrUint8         *soapString)
{
    CsrUint8 status     = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

    if (soapString)
    {
        if ( (CsrStrStr((char*) soapString, "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#CreateJob")) ||
             (CsrStrStr((char*) soapString, "SOAPACTION: \"urn:schemas-bluetooth-org:service:Printer:1#CreateJob")) )
        {
            csrBtBppsSoapActionCreateJobIndHandler(pInst->appHandle, soapString);

        }
        else if ( (CsrStrStr((char*) soapString, "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetPrinterAttributes")) ||
                  (CsrStrStr((char*) soapString, "SOAPACTION: \"urn:schemas-bluetooth-org:service:Printer:1#GetPrinterAttributes")) )
        {
            if (channelId == CSR_BT_BPPS_JOB_CH)
            {
                pInst->getPrinterAttrOperationCh = CSR_BT_BPPS_JOB_CH;
            }
            else
            {
                pInst->getPrinterAttrOperationCh = CSR_BT_BPPS_STATUS_CH;
            }
            csrBtBppsSoapActionGetPrinterAttribsIndHandler(pInst->appHandle, soapString);
        }
        else if ( (CsrStrStr((char*) soapString, "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetJobAttributes")) ||
                  (CsrStrStr((char*) soapString, "SOAPACTION: \"urn:schemas-bluetooth-org:service:Printer:1#GetJobAttributes")) )
        {
            if (channelId == CSR_BT_BPPS_JOB_CH)
            {
                pInst->getJobAttrOperationCh = CSR_BT_BPPS_JOB_CH;
            }
            else
            {
                pInst->getJobAttrOperationCh = CSR_BT_BPPS_STATUS_CH;
            }
            csrBtBppsSoapActionGetJobAttribsIndHandler(pInst->appHandle, soapString);

        }
        else if ( (CsrStrStr((char*) soapString, "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#CancelJob")) ||
                  (CsrStrStr((char*) soapString, "SOAPACTION: \"urn:schemas-bluetooth-org:service:Printer:1#CancelJob")) )
        {
            if (channelId == CSR_BT_BPPS_JOB_CH)
            {
                pInst->cancelOperationCh = CSR_BT_BPPS_JOB_CH;
            }
            else
            {
                pInst->cancelOperationCh = CSR_BT_BPPS_STATUS_CH;
            }
            csrBtBppsSoapActionCancelJobIndHandler(pInst->appHandle, soapString);

        }
        else if ( (CsrStrStr((char*) soapString, "SOAPACTION:\"urn:schemas-bluetooth-org:service:Printer:1#GetEvent")) ||
                  (CsrStrStr((char*) soapString, "SOAPACTION: \"urn:schemas-bluetooth-org:service:Printer:1#GetEvent")) )
        {
            csrBtBppsSoapActionGetEventIndHandler(pInst, soapString);
        }
        else
        {
            status = csrBtBppsGetWithHeadersResponseHandler(channel,
                                                            CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                            CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                                            0,
                                                            NULL,
                                                            0,
                                                            NULL,
                                                            NULL);
        }
    }
    else
    {
        status = csrBtBppsGetWithHeadersResponseHandler(channel,
                                                        CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                        CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                                        0,
                                                        NULL,
                                                        0,
                                                        NULL,
                                                        NULL);
    }
    return (status);
}

static CsrUint8 * csrBtBppsSoapActionMsgIndHandler(CsrUint8  **savedSoapAction,
                                                   CsrUint16 savedSoapActionLength,
                                                   CsrUint8  *obexPacket,
                                                   CsrUint16 obexPacketLength,
                                                   CsrUint16 *newSoapActionLength)
{
    CsrUint8         bodyType;
    CsrUint16        bodyOffset  = 0;
    CsrUint16        bodyLength  = 0;
    CsrUint8         *temp       = *savedSoapAction;
    *newSoapActionLength         = savedSoapActionLength;

    if (CsrBtObexGetBodyOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &bodyType, &bodyOffset, &bodyLength) && bodyLength > 0)
    { /* Note the extra byte for termination is added because the bpps soap
         parser demands it */
        if (*savedSoapAction)
        {
            CsrUint16 tempLength = (CsrUint16)(savedSoapActionLength + bodyLength);
            temp                 = (CsrUint8 *) CsrPmemAlloc(tempLength + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
            CsrMemCpy(temp, *savedSoapAction, savedSoapActionLength);
            CsrMemCpy(temp + savedSoapActionLength, obexPacket + bodyOffset, bodyLength);
            CsrPmemFree(*savedSoapAction);
            *savedSoapAction     = NULL;
            temp[tempLength]     = '\0'; /* Insert termination */
            *newSoapActionLength = tempLength;
        }
        else
        {
            *newSoapActionLength    = bodyLength;
            temp                    = (CsrUint8 *) CsrPmemAlloc(bodyLength + CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);
            CsrMemCpy(temp, obexPacket + bodyOffset, bodyLength);
            temp[bodyLength] = '\0'; /* Insert termination */
        }
    }
    return (temp);
}

static void csrBtBppsDeinitStatusChInst(BppsInstanceData * pInst)
{
    CsrPmemFree(pInst->savedStatusChSoapAction);
    pInst->savedStatusChSoapAction       = NULL;
    pInst->savedStatusChSoapActionLength = 0;
}

static void csrBtBppsInitJobChMsg(CsrBtBppsSendDocumentInd *pMsg)
{
    pMsg->type                       = CSR_BT_BPPS_SEND_DOCUMENT_IND;
    pMsg->documentTypeLength         = 0;
    pMsg->documentType               = NULL;
    pMsg->jobId                      = 0;
    pMsg->fileName                   = NULL;
    pMsg->documentBody               = NULL;
    pMsg->documentBodyLength         = 0;
    pMsg->docTypeDependentInfo       = NULL;
    pMsg->docTypeDependentInfoLength = 0;
}

static void csrBtBppsDeinitJobChInst(BppsInstanceData * pInst)
{
    CsrPmemFree(pInst->savedJobChSoapAction);
    pInst->savedJobChSoapAction         = NULL;
    pInst->savedJobChSoapActionLength   = 0;

    if (pInst->savedJobChMsg)
    {
        CsrBtBppsFreeUpstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedJobChMsg);
        CsrPmemFree(pInst->savedJobChMsg);
        pInst->savedJobChMsg = NULL;
    }
}

static void csrBtBppsDeinitObjectChInst(BppsInstanceData * pInst)
{
    if (pInst->savedObjectChDownStreamMsg)
    {
        CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedObjectChDownStreamMsg);
        CsrPmemFree(pInst->savedObjectChDownStreamMsg);
        pInst->savedObjectChDownStreamMsg = NULL;
    }

    if (pInst->savedObjectChUpStreamMsg)
    {
        CsrBtBppsFreeUpstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->savedObjectChUpStreamMsg);
        CsrPmemFree(pInst->savedObjectChUpStreamMsg);
        pInst->savedObjectChUpStreamMsg = NULL;
    }
}

static CsrUint8 *csrBtBppsBuildSoapActionCommand(char *soapString, CsrUint16 *soapActionCmdLength)
{
    CsrUint8 * soapActionCmd = (CsrUint8 *) CsrPmemAlloc(SIZE_OF_CONTENT_LENGTH        +
                                                       CSR_BT_BPPS_BYTES_IN_BODY     +
                                                       SIZE_OF_HTTP_NEWLINE          +
                                                       SIZE_OF_CONTENT_TYPE          +
                                                       SIZE_OF_HTTP_NEWLINE          +
                                                       CsrStrLen(soapString)         +
                                                       CSR_BT_BPPS_ZERO_TERMINATION_LENGTH);

    CsrStrCpy((char *)soapActionCmd, CONTENT_LENGTH);
    CsrIntToBase10((CsrInt32) CsrStrLen(soapString), (char *)(&soapActionCmd[CsrStrLen((char *)soapActionCmd)]));
    CsrStrCat((char *)soapActionCmd, HTTP_NEWLINE);
    CsrStrCat((char *)soapActionCmd, CONTENT_TYPE);
    CsrStrCat((char *)soapActionCmd, HTTP_NEWLINE);
    CsrStrCat((char *)soapActionCmd, soapString);
    *soapActionCmdLength = (CsrUint16) CsrStrLen((char *)soapActionCmd);
    return (soapActionCmd);
}

static CsrUint8 csrBtBppsSoapActionHandler(void                  *channel,
                                           CsrUint16             obexResponsePacketLength,
                                           CsrUint8              *soapAction,
                                           CsrUint16             soapActionLength,
                                           CsrUint16             soapActionIndex,
                                           CsrBool               *messageSend,
                                           CsrUint16             *consumedBytes,
                                           CsrBool               forceContinueRes)
{
    CsrUint16             bodyLength;
    CsrUint8              *body;
    CsrBtObexResponseCode rspCode;


    if (obexResponsePacketLength >= soapActionLength - soapActionIndex)
    { /* This this the last fragment that must be sent */
        if (forceContinueRes)
        { /* According to BPPS spec revision 1.2 section 7.1.7 the response
             code must always be CSR_BT_OBEX_CONTINUE_RESPONSE_CODE */
            rspCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        }
        else
        {
            rspCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
        *messageSend = TRUE;
        bodyLength   = (CsrUint16)(soapActionLength - soapActionIndex);
    }
    else
    { /* More fragments to sent */
        rspCode   = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
        *messageSend = FALSE;
        bodyLength = obexResponsePacketLength;
    }
    body            = (CsrUint8 *) CsrPmemAlloc(bodyLength);
    *consumedBytes  = bodyLength;
    CsrMemCpy(body, &soapAction[soapActionIndex], bodyLength);

    return (csrBtBppsGetWithHeadersResponseHandler(channel,
                                                   CSR_BT_BPPS_SOAP_ACTION_BODY_INDEX,
                                                   rspCode,
                                                   0,
                                                   NULL,
                                                   bodyLength,
                                                   body,
                                                   NULL));
}

/* Job Channel callback functions */
static void csrBtBppsJobChActivateCfmHandler(void            *instData,
                                             CsrUint8   serverChannel,
                                             psm_t           psm,
                                             CsrBtResultCode resultCode,
                                             CsrBtSupplier   resultSupplier)
{
    BppsInstanceData * pInst  = (BppsInstanceData *) instData;
    pInst->jobChLocalServerCh = serverChannel;

    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The Job and Status Channel is activated. Set both channels connectable */
        CsrUint8 status = ObexUtilServConnectableStart(pInst->obexStatusChannelInst,
                                                       CSR_BT_BPPS_PROFILE_DEFAULT_MTU_SIZE,
                                                       pInst->maxLocalObexPacketLength);

        if (status == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
        { /* The Status Channel is now connectable */
            csrBtBppsSetJobChConnectAble(pInst);
        }
        else
        {
             CsrBtBppsGeneralException("not allow to call ObexUtilServConnectableStart on statusCh");
        }
    }
    else
    { /* The Job Channel were not activate. Try again */
        if (csrBtBppsActivateJobCh(pInst) == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServActivateStart on jobCh");
        }
    }
    CSR_UNUSED(psm);
}

static void csrBtBppsJobChDeactivateCfmHandler(void          *instData,
                                               CsrUint8 serverChannel,
                                               psm_t         psm)
{ /* Because the Status Channel always is activate and deactivate before
     the Job Channel the Status Channel  must be deactivated */
    BppsInstanceData * pInst  = (BppsInstanceData *) instData;
    pInst->jobChState         = CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE;
    pInst->jobChLocalServerCh = serverChannel;
    csrBtBppsChannelRelease(pInst);
    bd_addr_zero(&(pInst->deviceAddr));
    csrBtBppsDeinitJobChInst(pInst);
    csrBtBppsDeactivateCfmSend(pInst->appHandle);
    CSR_UNUSED(psm);
}

static void csrBtBppsJobChConnectIndHandler(void            *instData,
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
    BppsInstanceData * pInst = (BppsInstanceData *) instData;

    if (pInst->statusChState == CSR_BT_BPPS_STATUS_CHANNEL_DEACTIVATING)
    { /* The status channel is being deactivated. Reject this request because
         the jobCh soon will be deactivated too */
        pInst->targetId = CSR_BT_BPPS_INVALID_TARGET;
    }
    else
    {
        pInst->targetId = csrBtBppsCheckTargetHeader(CSR_BT_BPPS_JOB_CHANNEL_INST_ID,
                                                     target,
                                                     targetHeaderLength);
    }


    if (pInst->targetId == CSR_BT_BPPS_INVALID_TARGET)
    {
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target Header is missing");
        CsrUint8      status       = csrBtBppsConnectResponseHandler(pInst->obexJobChannelInst,
                                                                     pInst->targetId,
                                                                     (CsrUint32) pInst->jobChLocalServerCh,
                                                                     CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                                                     NULL,
                                                                     description);

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServConnectResponse on jobCh");
        }
    }
    else
    {
        if (authenticated)
        {
            csrBtBppsAuthenticateCfmSend(pInst->appHandle);
        }
        pInst->jobChObexCommand         = CSR_BT_OBEX_CONNECT_OPCODE;
        pInst->deviceAddr               = deviceAddr;
        csrBtBppsConnectIndSend(pInst->appHandle,
                                (CsrUint32) pInst->jobChLocalServerCh,
                                (CsrUint16) (maxPeerObexPacketLength - CSR_BT_BPPS_HEADER_SIZE),
                                &deviceAddr,
                                length,
                                count,
                                btConnId);
#ifdef CSR_AMP_ENABLE
        pInst->jobChConnId = btConnId;
        updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    }
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtBppsJobChDisconnectIndHandler(void            *instData,
                                               CsrBtDeviceAddr deviceAddr,
                                               CsrBtReasonCode reasonCode,
                                               CsrBtSupplier   reasonSupplier,
                                               CsrUint16       obexPacketLength,
                                               CsrUint8      *obexPacket)
{ /* The job channel has been disconnected. Note if the status channel still
     is connected it is release by calling ObexUtilServNonConnectableStart */
    BppsInstanceData * pInst = (BppsInstanceData *) instData;
    pInst->jobChState        = CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE;
#ifdef CSR_AMP_ENABLE
    pInst->jobChConnId       = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    csrBtBppsChannelRelease(pInst);
    csrBtBppsDeinitJobChInst(pInst);

    switch (pInst->objectChState)
    {
        case CSR_BT_BPPS_OBJECT_CHANNEL_CONNECTED_STATE:
        case CSR_BT_BPPS_OBJECT_CHANNEL_SENDING_OBJECT_STATE:
        { /* A reference object is being or must be send over the object
             channel Wait to send a CSR_BT_BPPS_DISCONNECT_IND message until
             the object channel is release again */
            pInst->jobChReasonCode      = reasonCode;
            pInst->jobChReasonSupplier  = reasonSupplier;
            break;
        }
        default:
        { /* The object channel is already disconnected. Send a
             CSR_BT_BPPS_DISCONNECT_IND message to the application */
            csrBtBppsDisconnectIndSend(pInst->appHandle,
                                       (CsrUint32) pInst->jobChLocalServerCh,
                                       reasonCode,
                                       reasonSupplier);
        }
    }

    if (pInst->statusChState == CSR_BT_BPPS_STATUS_CHANNEL_CONNECTED_STATE)
    { /* The bpp client must always release the status Channel before it
         releases the job Channel. Release the status Channel */
        CsrUint8 status = ObexUtilServNonConnectableStart(pInst->obexStatusChannelInst,
                                                          csrBtBppsStatusChNonConnectableCfmHandler);

        pInst->statusChState = CSR_BT_BPPS_STATUS_CHANNEL_IDLE_STATE;
        if (pInst->statusChGetEventActive)
        {
            pInst->statusChGetEventActive = FALSE;
            csrBtBppsGetEventIndSend(pInst->appHandle, pInst->statusChGetEventJobId, FALSE);
        }

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServNonConnectableStart on statusCh");
        }
    }
    CsrPmemFree(obexPacket);
}

static void csrBtBppsJobChAbortIndHandler(void              *instData,
                                          CsrUint16         descriptionOffset,
                                          CsrUint16         descriptionLength,
                                          CsrUint16         obexPacketLength,
                                          CsrUint8        *obexPacket)
{ /* The jobCh has been aborted */
    BppsInstanceData * pInst = (BppsInstanceData *) instData;
    csrBtBppsDeinitJobChInst(pInst);
    csrBtBppsAbortIndSend(pInst->appHandle,
                          (CsrUint32) pInst->jobChLocalServerCh);

    CSR_UNUSED(descriptionOffset);
    CSR_UNUSED(descriptionLength);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppsJobChPutDocumentIndHandler(void          *instData,
                                                CsrUint16     obexResponsePacketLength,
                                                CsrBool       authenticated,
                                                CsrBool       finalBitSet,
                                                CsrUint16     bodyLength,
                                                CsrUint16     bodyOffset,
                                                CsrUint16     obexPacketLength,
                                                CsrUint8    *obexPacket)
{
    CsrUint8 *documentBody         = NULL;
    CsrUint16  documentBodyLength    = 0;
    BppsInstanceData *pInst          = (BppsInstanceData *) instData;
    pInst->jobChFinalBitSet          = finalBitSet;
    pInst->jobChResponsePacketLength = obexResponsePacketLength;

    if (bodyLength > 0)
    { /* A body header is included */
        documentBody        = (CsrUint8 *)CsrPmemAlloc(bodyLength);
        documentBodyLength  = bodyLength;
        CsrMemCpy(documentBody, (obexPacket + bodyOffset), bodyLength);
    }

    csrBtBppsNextIndSend(pInst->appHandle,
                         (CsrUint32) pInst->jobChLocalServerCh,
                         finalBitSet,
                         documentBody,
                         documentBodyLength);

    CsrPmemFree(obexPacket);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
}

static void csrBtBppsJobChPutCollectHeaderIndHandler(void          *instData,
                                                     CsrUint16     obexResponsePacketLength,
                                                     CsrBool       authenticated,
                                                     CsrBool       finalBitSet,
                                                     CsrUint16     bodyLength,
                                                     CsrUint16     bodyOffset,
                                                     CsrUint16     obexPacketLength,
                                                     CsrUint8    *obexPacket)
{ /* Look for headers is the received PUT Request message */
    CsrUint16                   length;
    CsrUint16                   offset;
    BppsInstanceData            *pInst = (BppsInstanceData *) instData;
    CsrBtBppsSendDocumentInd    *pMsg  = (CsrBtBppsSendDocumentInd *) pInst->savedJobChMsg;
    pInst->jobChResponsePacketLength   = obexResponsePacketLength;

    if (!pMsg->documentType)
    {
        if (CsrBtObexGetTypeOffset(CSR_BT_OBEX_REQUEST, obexPacket, &offset, &length))
        {
            pMsg->documentTypeLength = length;
            pMsg->documentType       = (CsrUtf8String *) CsrPmemAlloc(length);
            CsrMemCpy(pMsg->documentType, &obexPacket[offset], length);
        }
    }

    if (pMsg->jobId == 0)
    {
        if (CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_REQUEST, obexPacket, obexPacketLength, &offset, &length))
        {
            if (obexPacket[offset] == CSR_BT_BPPS_APP_PARAMS_JOBID_TAG)
            {
                pMsg->jobId = CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, (offset + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            }
        }
    }

    if (!pMsg->docTypeDependentInfo)
    {
        offset = CsrBtObexDescriptionHeaderIndex(CSR_BT_OBEX_REQUEST, obexPacket, &length);

        if (offset > 0 && (length > CSR_BT_OBEX_HEADER_LENGTH))
        { /* A valid descriptor is included */
            length                           = (CsrUint16) (length - CSR_BT_OBEX_HEADER_LENGTH);
            offset                           = (CsrUint16) (offset + CSR_BT_OBEX_HEADER_LENGTH);
            pMsg->docTypeDependentInfo       = (CsrUtf8String*) CsrUcs2ByteString2Utf8((const CsrUcs2String *)&obexPacket[offset]);
            pMsg->docTypeDependentInfoLength = (CsrUint16) (length / 2);
        }
    }

    if (!pMsg->fileName)
    {
        if(CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &offset))
        {
            pMsg->fileName = (CsrUtf8String*) CsrUcs2ByteString2Utf8((const CsrUcs2String *)&obexPacket[offset]);
        }
    }

    if (pMsg->documentBody == NULL && bodyLength > 0)
    { /* A body header is included in the OBEX PUT Request packet. E.g. all
         other headers are received */
        pMsg->documentBody       = (CsrUint8 *)CsrPmemAlloc(bodyLength);
        pMsg->documentBodyLength = bodyLength;
        CsrMemCpy(pMsg->documentBody , (obexPacket + bodyOffset), bodyLength);
    }
    pInst->savedJobChMsg = pMsg;

    if (pMsg->documentBody)
    { /* All headers should be received */
        CsrUint8 status = ObexUtilServSetPutContinueFunc(pInst->obexJobChannelInst,
                                                         csrBtBppsJobChPutDocumentIndHandler);

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServSetPutContinueFunc on jobCh");
        }

        pInst->jobChFinalBitSet = finalBitSet;
        csrBtBppsSendDocumentIndSend(pInst->appHandle, pInst->savedJobChMsg);
        csrBtBppsNextIndSend(pInst->appHandle,
                             (CsrUint32) pInst->jobChLocalServerCh,
                             finalBitSet,
                             NULL,
                             0);

        csrBtBppsInitJobChMsg((CsrBtBppsSendDocumentInd *) pInst->savedJobChMsg);
        csrBtBppsDeinitJobChInst(pInst);
    }
    else
    { /* Still missing a body header */
        CsrUint8 status;

        if (!finalBitSet)
        { /* Ask for the next OBEX PUT Request packet */
            status = ObexUtilServPutResponse(pInst->obexJobChannelInst,
                                             CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                             CSR_BT_OBEX_SRMP_INVALID);
        }
        else
        { /* The last OBEX PUT Request packet is received without any BODY
             header. Return an error response */
            status = ObexUtilServPutResponse(pInst->obexJobChannelInst,
                                             CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                             CSR_BT_OBEX_SRMP_INVALID);

            csrBtBppsDeinitJobChInst(pInst);
        }

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServPutResponse on jobCh");
        }
    }
    CsrPmemFree(obexPacket);
    CSR_UNUSED(authenticated);
}

static void csrBtBppsJobChPutIndHandler(void          *instData,
                                        CsrUint16     obexResponsePacketLength,
                                        CsrBool       authenticated,
                                        CsrBool       finalBitSet,
                                        CsrUint16     bodyLength,
                                        CsrUint16     bodyOffset,
                                        CsrUint16     obexPacketLength,
                                        CsrUint8    *obexPacket)
{ /* The first Put Request packet is received */
    BppsInstanceData *pInst = (BppsInstanceData *) instData;
    CsrUint8 status         = ObexUtilServSetPutContinueFunc(pInst->obexJobChannelInst,
                                                             csrBtBppsJobChPutCollectHeaderIndHandler);

    pInst->savedJobChMsg    = (CsrBtBppsSendDocumentInd *)
                               CsrPmemAlloc(sizeof(CsrBtBppsSendDocumentInd));

    pInst->jobChObexCommand = CSR_BT_OBEX_PUT_OPCODE;

    csrBtBppsInitJobChMsg((CsrBtBppsSendDocumentInd *) pInst->savedJobChMsg);

    if (authenticated)
    {
        csrBtBppsAuthenticateCfmSend(pInst->appHandle);
    }

    csrBtBppsJobChPutCollectHeaderIndHandler(instData,
                                             obexResponsePacketLength,
                                             authenticated,
                                             finalBitSet,
                                             bodyLength,
                                             bodyOffset,
                                             obexPacketLength,
                                             obexPacket);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServSetPutContinueFunc on jobCh");
    }
}

static void csrBtBppsJobChGetSoapActionIndHandler(void          *instData,
                                                  CsrUint16     obexResponsePacketLength,
                                                  CsrBool       authenticated,
                                                  CsrBool       finalBitSet,
                                                  CsrUint16     obexPacketLength,
                                                  CsrUint8    *obexPacket)
{
    CsrBool                 messageSend;
    CsrUint16               consumedBytes;
    BppsInstanceData        *pInst = (BppsInstanceData *) instData;

    CsrUint8                status = csrBtBppsSoapActionHandler(pInst->obexJobChannelInst,
                                                                obexResponsePacketLength,
                                                                pInst->savedJobChSoapAction,
                                                                pInst->savedJobChSoapActionLength,
                                                                pInst->savedJobChSoapActionIndex,
                                                                &messageSend,
                                                                &consumedBytes,
                                                                FALSE);
    if (messageSend)
    {
        CsrPmemFree(pInst->savedJobChSoapAction);
        pInst->savedJobChSoapAction         = NULL;
        pInst->savedJobChSoapActionLength   = 0;
    }
    else
    {
        pInst->savedJobChSoapActionIndex = (CsrUint16)(pInst->savedJobChSoapActionIndex + consumedBytes);
    }

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServGetWithHeaderResponse on jobCh");
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppsJobChGetIndHandler(void          *instData,
                                        CsrUint16     obexResponsePacketLength,
                                        CsrBool       authenticated,
                                        CsrBool       finalBitSet,
                                        CsrUint16     obexPacketLength,
                                        CsrUint8    *obexPacket)
{ /* The first OBEX GET packet is received. Collect all headers */
    CsrUint8         status;
    BppsInstanceData *pInst           = (BppsInstanceData *) instData;
    pInst->jobChResponsePacketLength  = obexResponsePacketLength;
    pInst->jobChObexCommand           = CSR_BT_OBEX_GET_OPCODE;
    pInst->savedJobChSoapAction       = csrBtBppsSoapActionMsgIndHandler(&pInst->savedJobChSoapAction,
                                                                         pInst->savedJobChSoapActionLength,
                                                                         obexPacket,
                                                                         obexPacketLength,
                                                                         &pInst->savedJobChSoapActionLength);

    if (authenticated)
    {
        csrBtBppsAuthenticateCfmSend(pInst->appHandle);
    }

    if (finalBitSet)
    { /* The first OBEX GET packet contain a complete SOAP action command */
        status = csrBtBppsSoapActionTypeHandler(pInst,
                                                CSR_BT_BPPS_JOB_CH,
                                                pInst->obexJobChannelInst,
                                                pInst->savedJobChSoapAction);
        csrBtBppsDeinitJobChInst(pInst);
    }
    else
    { /* The SOAP action command will be sent as a multiple OBEX GET operation */
        status = csrBtBppsGetWithHeadersResponseHandler(pInst->obexJobChannelInst,
                                                        CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                        CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                                        0,
                                                        NULL,
                                                        0,
                                                        NULL,
                                                        NULL);
    }

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServGetWithHeaderResponse on jobCh");
    }
    CsrPmemFree(obexPacket);
}

static void csrBtBppsJobChAuthenticateIndHandler(void            *instData,
                                                 CsrBtDeviceAddr deviceAddr,
                                                 CsrUint8        options,
                                                 CsrUint16       realmLength,
                                                 CsrUint8      *realm)
{ /* The JobCh has been OBEX authenticated */
    BppsInstanceData * pInst = (BppsInstanceData *) instData;

    csrBtBppsAuthenticateIndSend(pInst->appHandle,
                                 deviceAddr,
                                 options,
                                 realmLength,
                                 realm);
}

/* Status Channel callback functions */
static void csrBtBppsStatusChActivateCfmHandler(void            *instData,
                                                CsrUint8   serverChannel,
                                                psm_t           psm,
                                                CsrBtResultCode resultCode,
                                                CsrBtSupplier   resultSupplier)
{
    BppsInstanceData * pInst     = (BppsInstanceData *) instData;
    pInst->statusChLocalServerCh = serverChannel;


    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS &&
        resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The Status Channel is activate. Activate the Job Channel */
        pInst->serviceRecord[CSR_BT_BPPS_RFCOMM_SERVER_CHANNEL_PRINTING_STATUS_INDEX] = pInst->statusChLocalServerCh;

        if (csrBtBppsActivateJobCh(pInst) == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServActivateStart on jobCh");
        }
    }
    else
    { /* The Status Channel were not activate. Try again */
        if (csrBtBppsActivateStatusCh(pInst) == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServActivateStart on statusCh");
        }
    }
    CSR_UNUSED(psm);
}

static void csrBtBppsStatusChDeactivateCfmHandler(void          *instData,
                                                  CsrUint8 serverChannel,
                                                  psm_t         psm)
{
    BppsInstanceData * pInst      = (BppsInstanceData *) instData;
    pInst->statusChLocalServerCh  = serverChannel;
    pInst->statusChState          = CSR_BT_BPPS_STATUS_CHANNEL_IDLE_STATE;
    pInst->statusChGetEventActive = FALSE;
    csrBtBppsChannelRelease(pInst);
    csrBtBppsDeinitStatusChInst(pInst);

    if (pInst->jobChState == CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE)
    { /* The Job and Status channel is deactivate. Send a CsrBtBppsDeactivateCfm
         to the application */
        csrBtBppsDeactivateCfmSend(pInst->appHandle);
    }
    else
    { /* Deactivate the Job Channel */
        CsrUint8 status = ObexUtilServDeactivateStart(pInst->obexJobChannelInst,
                                                      pInst->jobChLocalServerCh,
                                                      L2CA_PSM_INVALID,
                                                      csrBtBppsJobChDeactivateCfmHandler);

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServDeactivateStart on jobCh");
        }
    }
    CsrPmemFree(pInst->serviceRecord);
    pInst->serviceRecord = NULL;
    CSR_UNUSED(psm);
}

static void csrBtBppsStatusChConnectIndHandler(void            *instData,
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
    CsrUint8                status;
    CsrBtObexResponseCode   responseCode;
    BppsInstanceData        *pInst          = (BppsInstanceData *) instData;
    CsrUcs2String           *description    = NULL;
    pInst->targetId                         = csrBtBppsCheckTargetHeader(CSR_BT_BPPS_STATUS_CHANNEL_INST_ID,
                                                                         target,
                                                                         targetHeaderLength);

    if (pInst->jobChState == CSR_BT_BPPS_JOB_CHANNEL_CONNECTED_STATE &&
        CsrBtBdAddrEq(&deviceAddr, &pInst->deviceAddr))
    {
        if (pInst->targetId == CSR_BT_BPPS_INVALID_TARGET)
        {
            description  = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target Header is missing");
            responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
        }
        else
        {
            csrBtBppsChannelConnected(pInst);
            responseCode     = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        }
    }
    else
    { /* The jobCh is not connected or it is connected to another device adress.
         Reject this connection */
        responseCode = CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE;
    }

    status = csrBtBppsConnectResponseHandler(pInst->obexStatusChannelInst,
                                             pInst->targetId,
                                             (CsrUint32) pInst->statusChLocalServerCh,
                                             responseCode,
                                             NULL,
                                             description);


    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServConnectResponse on statusCh");
    }
    else
    {
        if (responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
        { /* The status channel is now connected */
            pInst->statusChState = CSR_BT_BPPS_STATUS_CHANNEL_CONNECTED_STATE;
#ifdef CSR_AMP_ENABLE
            pInst->statusChConnId = btConnId;
            updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
        }
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(length);
    CSR_UNUSED(count);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtBppsStatusChNonConnectableCfmHandler(void          *instData,
                                                      CsrUint8 serverChannel,
                                                      psm_t         psm)
{ /* The status channel has been set non-connectable in order to release the
     transport connection. Activate it again. Note this is only done if the bpp
     client releases the job channel before the status channel */
    BppsInstanceData * pInst     = (BppsInstanceData *) instData;
    CsrUint8 status              = ObexUtilServConnectableStart(pInst->obexStatusChannelInst,
                                                                CSR_BT_BPPS_PROFILE_DEFAULT_MTU_SIZE,
                                                                pInst->maxLocalObexPacketLength);
    pInst->statusChLocalServerCh  = serverChannel;
    pInst->statusChGetEventActive = FALSE;
    csrBtBppsChannelRelease(pInst);
    csrBtBppsDeinitStatusChInst(pInst);

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServConnectableStart on statusCh");
    }
    CSR_UNUSED(psm);
}

static void csrBtBppsStatusChDisconnectIndHandler(void            *instData,
                                                  CsrBtDeviceAddr deviceAddr,
                                                  CsrBtReasonCode reasonCode,
                                                  CsrBtSupplier   reasonSupplier,
                                                  CsrUint16       obexPacketLength,
                                                  CsrUint8      *obexPacket)
{
    BppsInstanceData * pInst        = (BppsInstanceData *) instData;
    pInst->statusChState            = CSR_BT_BPPS_STATUS_CHANNEL_IDLE_STATE;

    if (pInst->statusChGetEventActive)
    {
        pInst->statusChGetEventActive = FALSE;
        csrBtBppsGetEventIndSend(pInst->appHandle, pInst->statusChGetEventJobId, FALSE);
    }
#ifdef CSR_AMP_ENABLE
    pInst->statusChConnId = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    csrBtBppsChannelRelease(pInst);
    csrBtBppsDeinitStatusChInst(pInst);
    CsrPmemFree(obexPacket);
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
}

static void csrBtBppsStatusChAbortIndHandler(void              *instData,
                                             CsrUint16         descriptionOffset,
                                             CsrUint16         descriptionLength,
                                             CsrUint16         obexPacketLength,
                                             CsrUint8        *obexPacket)
{
    BppsInstanceData * pInst        = (BppsInstanceData *) instData;

    if (pInst->statusChGetEventActive)
    {
        pInst->statusChGetEventActive = FALSE;
        csrBtBppsGetEventIndSend(pInst->appHandle, pInst->statusChGetEventJobId, FALSE);
    }
    csrBtBppsDeinitStatusChInst(pInst);
    CsrPmemFree(obexPacket);
}

static void csrBtBppsStatusChGetSoapActionIndHandler(void          *instData,
                                                     CsrUint16     obexResponsePacketLength,
                                                     CsrBool       authenticated,
                                                     CsrBool       finalBitSet,
                                                     CsrUint16     obexPacketLength,
                                                     CsrUint8    *obexPacket)
{
    BppsInstanceData        *pInst = (BppsInstanceData *) instData;

    if (pInst->savedStatusChSoapAction)
    {
        CsrBool                 messageSend;
        CsrUint16               consumedBytes;
        CsrUint8                status =  csrBtBppsSoapActionHandler(pInst->obexStatusChannelInst,
                                                                     obexResponsePacketLength,
                                                                     pInst->savedStatusChSoapAction,
                                                                     pInst->savedStatusChSoapActionLength,
                                                                     pInst->savedStatusChSoapActionIndex,
                                                                     &messageSend,
                                                                     &consumedBytes,
                                                                     pInst->statusChGetEventActive);

        if (messageSend)
        {
            CsrPmemFree(pInst->savedStatusChSoapAction);
            pInst->savedStatusChSoapAction       = NULL;
            pInst->savedStatusChSoapActionLength = 0;
        }
        else
        {
            pInst->savedStatusChSoapActionIndex = (CsrUint16)(pInst->savedStatusChSoapActionIndex + consumedBytes);
        }

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServGetWithHeaderResponse on statusCh");
        }
    }
    else
    {
        if (pInst->statusChGetEventActive)
        {
            pInst->statusChGetReadyToSend = TRUE;
        }
    }
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtBppsStatusChGetIndHandler(void          *instData,
                                           CsrUint16     obexResponsePacketLength,
                                           CsrBool       authenticated,
                                           CsrBool       finalBitSet,
                                           CsrUint16     obexPacketLength,
                                           CsrUint8    *obexPacket)
{ /* The first OBEX GET packet is received. Collect all headers */

    CsrUint8         status;
    BppsInstanceData *pInst              = (BppsInstanceData *) instData;
    pInst->statusChResponsePacketLength  = obexResponsePacketLength;
    pInst->savedStatusChSoapAction       = csrBtBppsSoapActionMsgIndHandler(&pInst->savedStatusChSoapAction,
                                                                            pInst->savedStatusChSoapActionLength,
                                                                            obexPacket,
                                                                            obexPacketLength,
                                                                            &pInst->savedStatusChSoapActionLength);

    if (finalBitSet)
    { /* The first OBEX GET packet contain a complete SOAP action command */
        status = csrBtBppsSoapActionTypeHandler(pInst,
                                                CSR_BT_BPPS_STATUS_CH,
                                                pInst->obexStatusChannelInst,
                                                pInst->savedStatusChSoapAction);
        csrBtBppsDeinitStatusChInst(pInst);
    }
    else
    { /* The SOAP action command will be sent as a multiple OBEX GET operation */
        status = csrBtBppsGetWithHeadersResponseHandler(pInst->obexStatusChannelInst,
                                                        CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                        CSR_BT_OBEX_CONTINUE_RESPONSE_CODE,
                                                        0,
                                                        NULL,
                                                        0,
                                                        NULL,
                                                        NULL);
    }

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrBtBppsGeneralException("not allow to call ObexUtilServGetWithHeaderResponse on statusCh");
    }
    CSR_UNUSED(authenticated);
    CsrPmemFree(obexPacket);
}

/* Object Channel callback functions */
static void csrBtBppsObjectChGetReferencedObjResultHandler(void                  *instData,
                                                           CsrBtObexResponseCode responseCode,
                                                           CsrBool               bodyHeader,
                                                           CsrUint16             bodyLength,
                                                           CsrUint16             bodyOffset,
                                                           CsrUint8            *obexPacket,
                                                           CsrUint16             obexPacketLength)
{
    BppsInstanceData * pInst    = (BppsInstanceData *) instData;
    CsrInt32         fileSize   = CSR_BT_BPPS_OBJECT_CHANNEL_UNKNOWN_FILESIZE_VALUE;

    if (!pInst->objectChHeadersFound)
    { /* Look for the OBEX App Parameter header */
        CsrUint16 offset;
        CsrUint16 length;

        if (CsrBtObexGetAppParametersOffset(CSR_BT_OBEX_REQUEST, obexPacket, obexPacketLength, &offset, &length))
        {
            if (obexPacket[offset] == CSR_BT_BPPS_APP_PARAMS_FILE_SIZE_TAG)
            {
                pInst->objectChHeadersFound = TRUE;
                fileSize = (CsrInt32) CSR_BT_OBEX_GET_D_WORD_AT(obexPacket, (offset + CSR_BT_AUTH_TAG_LENGTH_SCHEME));
            }
        }
    }

    if (responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        csrBtBppsGetReferencedObjIndSend(pInst->appHandle,
                                         obexPacket,
                                         fileSize,
                                         bodyOffset,
                                         bodyLength,
                                         obexPacketLength);
    }
    else
    {
        pInst->objectChState            = CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE;
        pInst->savedObjectChUpStreamMsg = csrBtBppsBuildGetReferencedObjCfmMsg(obexPacket,
                                                                               fileSize,
                                                                               bodyOffset,
                                                                               bodyLength,
                                                                               (CsrBtResultCode) responseCode,
                                                                               CSR_BT_SUPPLIER_IRDA_OBEX,
                                                                               obexPacketLength);

        if (ObexUtilDisconnectRequest(pInst->obexObjectChannelInst, TRUE, NULL) == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilDisconnectRequest on objectCh");
        }
    }
}

static void csrBtBppsObjectChConnectResultHandler(void                      *instData,
                                                  CsrBtResultCode           resultCode,
                                                  CsrBtSupplier             resultSupplier,
                                                  CsrBtDeviceAddr           deviceAddr,
                                                  CsrBtConnId               btConnId,
                                                  CsrUint16                 maxPeerObexPacketLength,
                                                  CmnCsrBtLinkedListStruct  *sdpTag,
                                                  CsrUint16                 obexPacketLength,
                                                  CsrUint8                  *obexPacket)
{
    BppsInstanceData * pInst = (BppsInstanceData *) instData;

    if(resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE &&
       resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    { /* The Object Channel is establish. */
        CsrUint16 appParametersHeaderLength;
        CsrUint8 headerPriorityTable[5] = {CSR_BT_OBEX_UTIL_TYPE_HEADER,
                                           CSR_BT_OBEX_UTIL_NAME_HEADER,
                                           CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER,
                                           CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

        CsrUint8 * appParametersHeader  = csrBtBppsObjectChBuildAppParamsGetRefObjHeader(pInst,
                                                                                         &appParametersHeaderLength);

        CsrUcs2String * name            = csrBtBppsObjectChGetRefObjName(pInst);

        CsrUint8 status                 = ObexUtilGetRequest(pInst->obexObjectChannelInst,
                                                             headerPriorityTable,
                                                             CSR_BT_BPPS_REFOBJ_HEADER_TYPE,
                                                             &name,
                                                             NULL,
                                                             appParametersHeaderLength,
                                                             &appParametersHeader,
                                                             0,
                                                             NULL,
                                                             CSR_BT_OBEX_SRMP_INVALID,
                                                             csrBtBppsObjectChGetReferencedObjResultHandler,
                                                             NULL);
        pInst->objectChHeadersFound     = FALSE;
        csrBtBppsChannelConnected(pInst);
#ifdef CSR_AMP_ENABLE
        pInst->objectChConnId = btConnId;
        updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilGetRequest on objectCh");
        }
    }
    else
    { /* Failed to setup the object channel */
        void *msg = csrBtBppsBuildGetReferencedObjCfmMsg(NULL,
                                                         0,
                                                         0,
                                                         0,
                                                         resultCode,
                                                         resultSupplier,
                                                         0);


        pInst->objectChState = CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE;
        csrBtBppsDeinitObjectChInst(pInst);
        csrBtBppsGetReferencedObjCfmSend(pInst->appHandle, msg);


        if (pInst->jobChState == CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE)
        { /* The job channel is already disconnected. Send a
             CSR_BT_BPPS_DISCONNECT_IND message to the application */
            csrBtBppsDisconnectIndSend(pInst->appHandle,
                                       (CsrUint32) pInst->jobChLocalServerCh,
                                       pInst->jobChReasonCode,
                                       pInst->jobChReasonSupplier);
        }

    }

    CsrBtUtilBllFreeLinkedList(&(sdpTag), CsrBtUtilBllPfreeWrapper);
    CSR_UNUSED(obexPacketLength);
    CSR_UNUSED(deviceAddr);
    CsrPmemFree(obexPacket);
}

static void csrBtBppsObjectChDisconnectResultHandler(void               *instData,
                                                     CsrBtReasonCode    reasonCode,
                                                     CsrBtSupplier      reasonSupplier,
                                                     CsrUint8           *obexPacket,
                                                     CsrUint16          obexPacketLength)
{ /* The object channel is released */
    BppsInstanceData * pInst = (BppsInstanceData *) instData;
    pInst->objectChState     = CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE;
#ifdef CSR_AMP_ENABLE
    pInst->objectChConnId = 0;
    updateAmpm(pInst);
#endif /* CSR_AMP_ENABLE */
    csrBtBppsChannelRelease(pInst);
    if (pInst->objectChState == CSR_BT_BPPS_OBJECT_CHANNEL_SENDING_OBJECT_STATE)
    { /* The object channel has been release abnormal */
        void *msg = csrBtBppsBuildGetReferencedObjCfmMsg(NULL,
                                                         0,
                                                         0,
                                                         0,
                                                         (CsrBtResultCode) reasonCode,
                                                         (CsrBtSupplier) reasonSupplier,
                                                         0);

        csrBtBppsGetReferencedObjCfmSend(pInst->appHandle, msg);
    }
    else if (pInst->savedObjectChUpStreamMsg)
    {
        csrBtBppsGetReferencedObjCfmSend(pInst->appHandle, pInst->savedObjectChUpStreamMsg);
        pInst->savedObjectChUpStreamMsg = NULL;
    }

    if (pInst->jobChState == CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE)
    { /* The job channel has been released. Send a CSR_BT_BPPS_DISCONNECT_IND
         message to the application. */
        csrBtBppsDisconnectIndSend(pInst->appHandle,
                                   (CsrUint32) pInst->jobChLocalServerCh,
                                   pInst->jobChReasonCode,
                                   pInst->jobChReasonSupplier);
    }

    csrBtBppsDeinitObjectChInst(pInst);
    CSR_UNUSED(reasonCode);
    CSR_UNUSED(reasonSupplier);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

/* Static help functions */
static CsrUint8 csrBtBppsActivateStatusCh(BppsInstanceData *pInst)
{ /* Activate the Status Channel */
    dm_security_level_t  secIncoming;
    CsrBtScSetSecInLevel(&secIncoming,
                         CSR_BT_SEC_DEFAULT,
                         CSR_BT_SEC_NONE,
                         CSR_BT_BPPS_SECONDAR_CHANNEL_SECURITY,
                         CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                         CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    return (ObexUtilServActivateStart(pInst->obexStatusChannelInst,
                                      secIncoming,
                                      CSR_BT_PRINTING_STATUS_UUID,
                                      CSR_BT_BPPS_LP_SUPERVISION_TIMEOUT,
                                      pInst->statusChLocalServerCh,
                                      L2CA_PSM_INVALID,
                                      0,
                                      0,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      NULL,
                                      (CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK | CSR_BT_RENDERING_MAJOR_SERVICE_MASK),
                                      csrBtBppsStatusChConnectIndHandler,
                                      NULL,
                                      csrBtBppsStatusChGetIndHandler,
                                      NULL,
                                      csrBtBppsStatusChAbortIndHandler,
                                      csrBtBppsStatusChDisconnectIndHandler,
                                      NULL,
                                      csrBtBppsStatusChActivateCfmHandler,
                                      NULL,
                                      pInst->maxLocalObexPacketLength,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

static CsrUint8 csrBtBppsActivateJobCh(BppsInstanceData *pInst)
{ /* Activate the Job Channel */
    CsrUint8 *serviceRecord = (CsrUint8 *) CsrPmemAlloc(pInst->serviceRecordLength);
    pInst->jobChState       = CSR_BT_BPPS_JOB_CHANNEL_ACTIVATED_STATE;
    CsrMemCpy(serviceRecord, pInst->serviceRecord, pInst->serviceRecordLength);
    return (ObexUtilServActivateStart(pInst->obexJobChannelInst,
                                      pInst->secIncoming,
                                      CSR_BT_DIRECT_PRINTING_UUID,
                                      CSR_BT_BPPS_LP_SUPERVISION_TIMEOUT,
                                      pInst->jobChLocalServerCh,
                                      L2CA_PSM_INVALID,
                                      pInst->serviceRecordLength,
                                      CSR_BT_BPPS_RFCOMM_SERVER_CHANNEL_DIRECT_PRINTING_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                                      &serviceRecord,
                                      (CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK | CSR_BT_RENDERING_MAJOR_SERVICE_MASK),
                                      csrBtBppsJobChConnectIndHandler,
                                      csrBtBppsJobChPutIndHandler,
                                      csrBtBppsJobChGetIndHandler,
                                      NULL,
                                      csrBtBppsJobChAbortIndHandler,
                                      csrBtBppsJobChDisconnectIndHandler,
                                      csrBtBppsJobChAuthenticateIndHandler,
                                      csrBtBppsJobChActivateCfmHandler,
                                      NULL,
                                      pInst->maxLocalObexPacketLength,
                                      pInst->windowSize,
                                      pInst->localSrmEnable));
}

static CsrUint8 csrBtBppsSoapActionInitHandler(BppsInstanceData *pInst,
                                               CsrUint8         *soapAction,
                                               CsrUint16        soapActionLength,
                                               CsrBool          operationChannel,
                                               CsrUint16        appParametersHeaderLength,
                                               CsrUint8         *appParametersHeader)
{
    CsrUint16               bodyLength;
    CsrBtObexResponseCode   rspCode;
    CsrUint8                *body;
    void                    *channel;
    CsrUint8                bodyIndex;

    if (appParametersHeader)
    {
        bodyIndex = CSR_BT_BPPS_CREATE_JOB_RESPONSE_BODY_INDEX;
    }
    else
    {
        bodyIndex = CSR_BT_BPPS_SOAP_ACTION_BODY_INDEX;
    }

    if (operationChannel == CSR_BT_BPPS_JOB_CH)
    { /* This response must be sent on the Job Channel */
        channel = pInst->obexJobChannelInst;

        if (pInst->jobChResponsePacketLength >= appParametersHeaderLength + soapActionLength)
        { /* The message can be sent in one OBEX Response packet */
            rspCode    = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            bodyLength = soapActionLength;
            body       = soapAction;
        }
        else
        { /* The message must be sent as a multiple GET operation */
            CsrUint8 status = ObexUtilServSetGetContinueFunc(pInst->obexJobChannelInst,
                                                             csrBtBppsJobChGetSoapActionIndHandler);

            rspCode     = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bodyLength  = pInst->jobChResponsePacketLength;
            body        = (CsrUint8 *) CsrPmemAlloc(bodyLength);
            CsrMemCpy(body, soapAction, bodyLength);
            pInst->savedJobChSoapActionIndex  = bodyLength;
            pInst->savedJobChSoapAction       = soapAction;
            pInst->savedJobChSoapActionLength = soapActionLength;

            if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            {
                CsrBtBppsGeneralException("not allow to call ObexUtilServSetGetContinueFunc on jobCh");
            }
        }
    }
    else
    { /* This response must be sent on the Status Channel */
        CsrUint8 status = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
        channel         = pInst->obexStatusChannelInst;

        if (pInst->statusChResponsePacketLength >= appParametersHeaderLength + soapActionLength)
        { /* The message can be sent in one OBEX Response packet */
            if (pInst->statusChGetEventActive)
            { /* According to BPPS spec revision 1.2 section 7.1.7 the response
                 code must always be CSR_BT_OBEX_CONTINUE_RESPONSE_CODE */
                rspCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
                status  = ObexUtilServSetGetContinueFunc(pInst->obexStatusChannelInst,
                                                         csrBtBppsStatusChGetSoapActionIndHandler);
            }
            else
            {
                rspCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            }
            bodyLength = soapActionLength;
            body       = soapAction;
        }
        else
        { /* The message must be sent as a multiple GET operation */
            status = ObexUtilServSetGetContinueFunc(pInst->obexStatusChannelInst,
                                                    csrBtBppsStatusChGetSoapActionIndHandler);

            rspCode     = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            bodyLength  = pInst->statusChResponsePacketLength;
            body        = (CsrUint8 *) CsrPmemAlloc(bodyLength);
            CsrMemCpy(body, soapAction, bodyLength);
            pInst->savedStatusChSoapActionIndex  = bodyLength;
            pInst->savedStatusChSoapAction       = soapAction;
            pInst->savedStatusChSoapActionLength = soapActionLength;


        }

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilServSetGetContinueFunc on statusCh");
        }
    }

    return (csrBtBppsGetWithHeadersResponseHandler(channel,
                                                   bodyIndex,
                                                   rspCode,
                                                   appParametersHeaderLength,
                                                   appParametersHeader,
                                                   bodyLength,
                                                   body,
                                                   NULL));
}

/* Public BPPS downstream message handlers */
CsrUint8 CsrBtBppsActivateReqHandler(BppsInstanceData *pInst)
{
    CsrBtBppsActivateReq *pMsg  = (CsrBtBppsActivateReq *) pInst->recvMsgP;
    CsrUint8             status = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    pInst->appHandle            = pMsg->appHandle;
    pInst->maxLocalObexPacketLength = pMsg->obexMaxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;

    csrBtBppsCreateServiceRecord(pInst,
                                 pMsg->documentFormatsSupported,
                                 pMsg->characterRepertoiresSupported,
                                 pMsg->characterRepertoiresSupportedLength,
                                 pMsg->imageFormatsSupported,
                                 pMsg->ieee1284Id);

    if (pInst->jobChState == CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE)
    { /* The application is allow to start the activate procedure. Please
         note the Status Channel is always Activated before the Job Channel */
        status = csrBtBppsActivateStatusCh(pInst);
    }

    if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
    {
        CsrPmemFree(pInst->serviceRecord);
        pInst->serviceRecord = NULL;
    }
    CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->recvMsgP);
    return (status);
}

CsrUint8 CsrBtBppsDeactivateReqHandler(BppsInstanceData *pInst)
{ /* As the Status Channel always is activate first, deactivate it first.
     If ObexUtilServDeactivateStart return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION
     the application is breaking the API */
    pInst->statusChState = CSR_BT_BPPS_STATUS_CHANNEL_DEACTIVATING;

    if (pInst->objectChState != CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE)
    {
        CsrUint8 status      = ObexUtilDisconnectRequest(pInst->obexObjectChannelInst, FALSE, NULL);
        pInst->objectChState = CSR_BT_BPPS_OBJECT_CHANNEL_IDLE_STATE;

        if (status == CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
        {
            CsrBtBppsGeneralException("not allow to call ObexUtilDisconnectRequest on objectCh");
        }
    }

    return (ObexUtilServDeactivateStart(pInst->obexStatusChannelInst,
                                        pInst->statusChLocalServerCh,
                                        L2CA_PSM_INVALID,
                                        csrBtBppsStatusChDeactivateCfmHandler));
}

CsrUint8 CsrBtBppsConnectResHandler(BppsInstanceData *pInst)
{
    /* The application has response to an incoming OBEX connection on the jobCh */
    CsrBtBppsConnectRes *pMsg       = (CsrBtBppsConnectRes *) pInst->recvMsgP;

    if((pMsg->responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK) != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    { /* The application has rejected the OBEX connection */
        bd_addr_zero(&(pInst->deviceAddr));
    }
    else
    { /* The job Channel is OBEX connected */
        csrBtBppsChannelConnected(pInst);
        pInst->jobChState = CSR_BT_BPPS_JOB_CHANNEL_CONNECTED_STATE;
    }

    return (csrBtBppsConnectResponseHandler(pInst->obexJobChannelInst,
                                            pInst->targetId,
                                            (CsrUint32) pInst->jobChLocalServerCh,
                                            pMsg->responseCode,
                                            NULL,
                                            NULL));
}

CsrUint8 CsrBtBppsGetReferencedObjReqHandler(BppsInstanceData *pInst)
{
    CsrUint8 status = CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;

    if (!CsrBtBdAddrEqZero(&pInst->deviceAddr) &&
        pInst->jobChState != CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE)
    { /* Establish the object channel. Note if pInst->jobChState ==
         CSR_BT_BPPS_JOB_CHANNEL_IDLE_STATE the profile is deactivated */
        CsrUint16           index;
        dm_security_level_t secOutgoing;

        CsrUint8 tempTarget[CSR_BT_BPPS_REF_OBJ_TARGET_LEN] = CSR_BT_BPPS_REF_OBJ_TARGET;
        CmnCsrBtLinkedListStruct * sdpTag = NULL;
        CsrUint8 *target                  = (CsrUint8 *) CsrPmemAlloc(CSR_BT_BPPS_REF_OBJ_TARGET_LEN);
        pInst->objectChState              = CSR_BT_BPPS_OBJECT_CHANNEL_CONNECTED_STATE;
        CsrMemCpy(target, tempTarget, CSR_BT_BPPS_REF_OBJ_TARGET_LEN);
        sdpTag = CsrBtUtilSdrCreateServiceHandleEntryFromUuid32(sdpTag,
                                                                CSR_BT_DIRECT_PRINTING_REFERENCE_OBJ_UUID,
                                                                &index);

        CsrBtUtilSdrInsertLocalServerChannel(sdpTag, index, CSR_BT_NO_SERVER);

        CsrBtScSetSecOutLevel(&secOutgoing,
                              CSR_BT_SEC_DEFAULT,
                              CSR_BT_SEC_NONE,
                              CSR_BT_BPPS_SECONDAR_CHANNEL_SECURITY,
                              CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                              CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

        status = ObexUtilConnectRequest(pInst->obexObjectChannelInst,
                                        &sdpTag,
                                        pInst->deviceAddr,
                                        secOutgoing,
                                        pInst->maxLocalObexPacketLength,
                                        CSR_BT_BPPS_PROFILE_DEFAULT_MTU_SIZE,
                                        &target,
                                        CSR_BT_BPPS_REF_OBJ_TARGET_LEN,
                                        0,
                                        NULL,
                                        CSR_BT_BPPS_LP_SUPERVISION_TIMEOUT,
                                        NULL,
                                        0,
                                        0,
                                        NULL,
                                        0,
                                        NULL,
                                        csrBtBppsObjectChConnectResultHandler,
                                        NULL,
                                        csrBtBppsObjectChDisconnectResultHandler,
                                        NULL,
                                        pInst->windowSize,
                                        pInst->localSrmEnable);
    }

    if (status == CSR_BT_OBEX_UTIL_STATUS_ACCEPTED)
    { /* Save the CsrBtBppsGetReferencedObjReq message */
        pInst->savedObjectChDownStreamMsg = pInst->recvMsgP;
        pInst->recvMsgP                   = NULL;
    }
    return (status);
}

CsrUint8 CsrBtBppsGetReferencedObjResHandler(BppsInstanceData * pInst)
{
    return (ObexUtilGetContinueRequest(pInst->obexObjectChannelInst, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtBppsSecurityInReqHandler(BppsInstanceData * pInst)
{
    CsrBtBppsSecurityInReq *pMsg = (CsrBtBppsSecurityInReq *) pInst->recvMsgP;

    CsrBtResultCode resultCode   = CsrBtScSetSecInLevel(&pInst->secIncoming,
                                                        pMsg->secLevel,
                                                        CSR_BT_BPPS_MANDATORY_SECURITY_INCOMING,
                                                        CSR_BT_BPPS_DEFAULT_SECURITY_INCOMING,
                                                        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                                                        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    csrBtBppsSecurityInCfmSend(pMsg->appHandle, resultCode);
    ObexUtilServChangeIncomingSecurity(pInst->obexJobChannelInst, pInst->secIncoming);
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBppsAuthenticateResHandler(BppsInstanceData * pInst)
{
    CsrBtBppsAuthenticateRes *pMsg = (CsrBtBppsAuthenticateRes *) pInst->recvMsgP;
    return(ObexUtilServAuthenticateResponse(pInst->obexJobChannelInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
}

CsrUint8 CsrBtBppsNextResHandler(BppsInstanceData * pInst)
{
    CsrBtObexResponseCode rspCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;

    if (pInst->jobChFinalBitSet)
    {
        rspCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    return (ObexUtilServPutResponse(pInst->obexJobChannelInst, rspCode, CSR_BT_OBEX_SRMP_INVALID));
}

CsrUint8 CsrBtBppsCreateJobResHandler(BppsInstanceData * pInst)
{
    CsrUint16             appParametersHeaderLength;
    CsrUint8              *soapAction;
    CsrUint16             soapActionLength;
    char                  buffer[CSR_BT_BPPS_BUF_SIZE];

    CsrBtBppsCreateJobRes *pMsg   = (CsrBtBppsCreateJobRes *) pInst->recvMsgP;
    char *soapString              = (char *) CsrPmemZalloc(CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE);
    CsrUint8 *appParametersHeader = csrBtBppsBuildAppParamsJobId(pMsg->jobId, &appParametersHeaderLength);

    CsrStrCpy(soapString, "<s:Envelope\n\rxmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\"\n");
    CsrStrCat(soapString, "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n");
    CsrStrCat(soapString, "<s:Body>\n");
    CsrStrCat(soapString, "<u:CreateJobResponse xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n");
    CsrIntToBase10(pMsg->jobId, buffer);
    soapAddTagValue(soapString, "JobId", buffer);
    CsrStrCpy(buffer, "0x");
    CsrUInt16ToHex(pMsg->operationStatus, &buffer[CsrStrLen(buffer)]);
    soapAddTagValue(soapString, "OperationStatus", buffer);
    CsrStrCat(soapString, "</u:CreateJobResponse>\n");
    CsrStrCat(soapString, "</s:Body>\n");
    CsrStrCat(soapString, "</s:Envelope>");

    soapAction = csrBtBppsBuildSoapActionCommand(soapString,
                                                 &soapActionLength);
    CsrPmemFree(soapString);

    return (csrBtBppsSoapActionInitHandler(pInst,
                                           soapAction,
                                           soapActionLength,
                                           CSR_BT_BPPS_JOB_CH,
                                           appParametersHeaderLength,
                                           appParametersHeader));
}

CsrUint8 CsrBtBppsGetPrinterAttribsResHandler(BppsInstanceData * pInst)
{
    char                  buffer[CSR_BT_BPPS_BUF_SIZE];
    CsrUint8              *soapAction;
    CsrUint16             soapActionLength;

    CsrBtBppsGetPrinterAttribsRes *pMsg = (CsrBtBppsGetPrinterAttribsRes *) pInst->recvMsgP;
    char *soapString                    = (char *) CsrPmemZalloc(CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE);

    CsrStrCpy(soapString, "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" ");
    CsrStrCat(soapString, "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n");
    CsrStrCat(soapString, "<s:Body>\n");
    CsrStrCat(soapString, "<u:GetPrinterAttributesResponse xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n");

    soapAddTagValue(soapString,         "PrinterName", (char*) pMsg->printerName);
    soapAddTagValue(soapString,         "PrinterLocation", (char*) pMsg->printerLocation);
    soapAddTagValue(soapString,         "PrinterState", (char*) pMsg->printerState);
    soapAddTagValue(soapString,         "PrinterStateReasons", (char*) pMsg->printerStateReasons);
    soapAddTagValue(soapString,         "PrinterGeneralCurrentOperator", (char*) pMsg->printerGeneralCurrentOperator);
    soapAddMultiTagValues(soapString,   "DocumentFormat", (char*) pMsg->documentFormatsSupported, pMsg->documentFormatsSupportedCount);
    soapAddMultiTagValues(soapString,   "ImageFormat", (char*) pMsg->imageFormatsSupported, pMsg->imageFormatsSupportedCount);
    soapAddTagValue(soapString,         "ColorSupported", (char*) pMsg->colorSupported);
    soapAddTagValue(soapString,         "MaxCopiesSupported", (char*) pMsg->maxCopiesSupported);
    soapAddMultiTagValuesNoS(soapString,"Sides", (char*) pMsg->sidesSupported, pMsg->sidesSupportedCount);
    soapAddTagValue(soapString,         "NumberUpSupported", (char*) pMsg->numberUpSupported);
    soapAddMultiTagValues(soapString,   "Orientation", (char*) pMsg->orientationsSupported, pMsg->orientationsSupportedCount);
    soapAddMultiTagValues(soapString,   "MediaSize", (char*) pMsg->mediaSizesSupported, pMsg->mediaSizesSupportedCount);
    soapAddMultiTagValues(soapString,   "MediaType", (char*) pMsg->mediaTypesSupported, pMsg->mediaTypesSupportedCount);

    if(pMsg->mediaLoadedCount > 0)
    {
        soapAddMediaLoaded(soapString, (char*)pMsg->mediaLoaded, pMsg->mediaLoadedCount);
    }

    soapAddMultiTagValuesNoS(soapString,"PrintQuality", (char*) pMsg->printQualitySupported, pMsg->printQualitySupportedCount);
    soapAddTagValue(soapString,         "QueuedJobCount", (char*) pMsg->queuedJobCount);
    soapAddTagValue(soapString,         "BasicTextPageWidth", (char*) pMsg->basicTextPageWidth);
    soapAddTagValue(soapString,         "BasicTextPageHeight", (char*) pMsg->basicTextPageHeight);
    CsrStrCpy(buffer, "0x");
    CsrUInt16ToHex(pMsg->operationStatus, &buffer[CsrStrLen(buffer)]);
    soapAddTagValue(soapString, "OperationStatus", buffer);
    CsrStrCat(soapString, "</u:GetPrinterAttributesResponse>\n");
    CsrStrCat(soapString, "</s:Body>\n");
    CsrStrCat(soapString, "</s:Envelope>");

    soapAction = csrBtBppsBuildSoapActionCommand(soapString,
                                                 &soapActionLength);

    CsrPmemFree(soapString);
    CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->recvMsgP);

    return (csrBtBppsSoapActionInitHandler(pInst,
                                           soapAction,
                                           soapActionLength,
                                           pInst->getPrinterAttrOperationCh,
                                           0,
                                           NULL));
}

CsrUint8 CsrBtBppsGetJobAttribsResHandler(BppsInstanceData * pInst)
{
    char                  buffer[CSR_BT_BPPS_BUF_SIZE];
    CsrUint8              *soapAction;
    CsrUint16             soapActionLength;

    CsrBtBppsGetJobAttribsRes *pMsg = (CsrBtBppsGetJobAttribsRes*) pInst->recvMsgP;
    char *soapString                = (char *) CsrPmemZalloc(CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE);

    CsrStrCpy(soapString, "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" ");
    CsrStrCat(soapString, "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n");
    CsrStrCat(soapString, "<s:Body>\n");
    CsrStrCat(soapString, "<u:GetJobAttributesResponse xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n");
    CsrIntToBase10(pMsg->jobId, buffer);
    soapAddTagValue(soapString, "JobId", buffer);
    soapAddTagValue(soapString, "JobState", (char*) pMsg->jobState);
    soapAddTagValue(soapString, "JobName", (char*) pMsg->jobName);
    soapAddTagValue(soapString, "JobOriginatingUserName", (char*) pMsg->jobOriginatingUserName);

    CsrIntToBase10(pMsg->jobMediaSheetsCompleted, buffer);
    soapAddTagValue(soapString, "JobMediaSheetsCompleted", buffer);

    CsrIntToBase10(pMsg->numberOfInterveningJobs, buffer);
    soapAddTagValue(soapString, "NumberOfInterveningJobs", buffer);

    CsrStrCpy(buffer, "0x");
    CsrUInt16ToHex(pMsg->operationStatus, &buffer[CsrStrLen(buffer)]);
    soapAddTagValue(soapString, "OperationStatus", buffer);

    CsrStrCat(soapString, "</u:GetJobAttributesResponse>\n");
    CsrStrCat(soapString, "</s:Body>\n");
    CsrStrCat(soapString, "</s:Envelope>");

    soapAction = csrBtBppsBuildSoapActionCommand(soapString,
                                                 &soapActionLength);

    CsrPmemFree(soapString);
    CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->recvMsgP);

    return (csrBtBppsSoapActionInitHandler(pInst,
                                           soapAction,
                                           soapActionLength,
                                           pInst->getJobAttrOperationCh,
                                           0,
                                           NULL));
}

CsrUint8 CsrBtBppsCancelJobResHandler(BppsInstanceData * pInst)
{
    char                  buffer[CSR_BT_BPPS_BUF_SIZE];
    CsrUint8              *soapAction;
    CsrUint16             soapActionLength;

    CsrBtBppsCancelJobRes *pMsg = (CsrBtBppsCancelJobRes *) pInst->recvMsgP;
    char *soapString            = (char *) CsrPmemZalloc(CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE);

    CsrStrCpy(soapString, "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" ");
    CsrStrCat(soapString, "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n");
    CsrStrCat(soapString, "<s:Body>\n");
    CsrStrCat(soapString, "<u:CancelJobResponse xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n");
    CsrIntToBase10(pMsg->jobId, buffer);
    soapAddTagValue(soapString, "JobId", buffer);
    CsrStrCpy(buffer, "0x");
    CsrUInt16ToHex(pMsg->operationStatus, &buffer[CsrStrLen(buffer)]);
    soapAddTagValue(soapString, "OperationStatus", buffer);

    CsrStrCat(soapString, "</u:CancelJobResponse>\n");
    CsrStrCat(soapString, "</s:Body>\n");
    CsrStrCat(soapString, "</s:Envelope>");

    soapAction = csrBtBppsBuildSoapActionCommand(soapString,
                                                 &soapActionLength);

    CsrPmemFree(soapString);
    return (csrBtBppsSoapActionInitHandler(pInst,
                                           soapAction,
                                           soapActionLength,
                                           pInst->cancelOperationCh,
                                           0,
                                           NULL));
}

CsrUint8 CsrBtBppsGetEventResHandler(BppsInstanceData * pInst)
{
    CsrUint8 status = CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;

    if (pInst->statusChGetEventActive)
    {
        char                 buffer[CSR_BT_BPPS_BUF_SIZE];
        CsrUint8             *soapAction;
        CsrUint16            soapActionLength;
        CsrBtBppsGetEventRes *pMsg = (CsrBtBppsGetEventRes *) pInst->recvMsgP;
        char *soapString           = (char *) CsrPmemZalloc(CSR_BT_BPPS_MAX_SOAP_ACTION_SIZE);

        CsrStrCpy(soapString, "<s:Envelope\n\rxmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" ");
        CsrStrCat(soapString, "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\n");
        CsrStrCat(soapString, "<s:Body>\n");
        CsrStrCat(soapString, "<u:GetEventResponse xmlns:u=\"urn:schemas-bluetooth-org:service:Printer:1\">\n");

        CsrIntToBase10(pMsg->jobId, buffer);
        soapAddTagValue(soapString, "JobState", (char*) pMsg->jobState);
        soapAddTagValue(soapString, "JobId", buffer);
        soapAddTagValue(soapString, "PrinterState", (char*) pMsg->printerState);
        soapAddTagValue(soapString, "PrinterStateReasons", (char*) pMsg->printerStateReasons);
        CsrStrCpy(buffer, "0x");
        CsrUInt16ToHex(pMsg->operationStatus, &buffer[CsrStrLen(buffer)]);
        soapAddTagValue(soapString, "OperationStatus", buffer);
        CsrStrCat(soapString, "</u:GetEventResponse>\n");
        CsrStrCat(soapString, "</s:Body>\n");
        CsrStrCat(soapString, "</s:Envelope>");
        soapAction = csrBtBppsBuildSoapActionCommand(soapString, &soapActionLength);
        CsrPmemFree(soapString);

        if (pInst->savedStatusChSoapAction)
        { /* Still waiting for a Client to send another GET request asking
             for additional data */

            /* Find the number of bytes it still needs to send from
               previously response */
            CsrUint16 missingBytes = (CsrUint16) (pInst->savedStatusChSoapActionLength - pInst->savedStatusChSoapActionIndex);

            if (missingBytes + soapActionLength > CSR_BT_MAX_OBEX_SIGNAL_LENGTH)
            { /* BPPS is not allowed to saved more data, Pfree this message.
                 Note this means that the client will not get this status */
                CsrPmemFree(soapAction);
            }
            else
            { /* Add this message to the saved one */
                CsrUint8  *modifySoapAction = (CsrUint8 *) CsrPmemAlloc(missingBytes + soapActionLength);
                CsrMemCpy(modifySoapAction, &pInst->savedStatusChSoapAction[pInst->savedStatusChSoapActionIndex], missingBytes);
                CsrMemCpy(&modifySoapAction[missingBytes], soapAction, soapActionLength);
                CsrPmemFree(soapAction);
                CsrPmemFree(pInst->savedStatusChSoapAction);
                pInst->savedStatusChSoapAction       = modifySoapAction;
                pInst->savedStatusChSoapActionIndex  = 0;
                pInst->savedStatusChSoapActionLength = (CsrUint16) (soapActionLength + missingBytes);
            }
        }
        else if (!pInst->statusChGetReadyToSend)
        { /* The last fragment is being send */
            pInst->savedStatusChSoapAction       = soapAction;
            pInst->savedStatusChSoapActionIndex  = 0;
            pInst->savedStatusChSoapActionLength = soapActionLength;
        }
        else
        { /* Ready to sent */
            pInst->statusChGetReadyToSend = FALSE;
            status = csrBtBppsSoapActionInitHandler(pInst,
                                                    soapAction,
                                                    soapActionLength,
                                                    CSR_BT_BPPS_STATUS_CH,
                                                    0,
                                                    NULL);
        }

    }
    else
    { /* This operation has been aborted */
        ;
    }
    CsrBtBppsFreeDownstreamMessageContents(CSR_BT_BPPS_PRIM, pInst->recvMsgP);
    return (status);
}

CsrUint8 CsrBtBppsJobCompleteReqHandler(BppsInstanceData * pInst)
{

    if (pInst->numOfConn > 0)
    {
        pInst->jobCompletion = TRUE;
    }
    else
    {
        pInst->jobCompletion = FALSE;
        csrBtBppsJobCompleteCfmSend(pInst->appHandle);
    }
    return (CSR_BT_OBEX_UTIL_STATUS_ACCEPTED);
}

CsrUint8 CsrBtBppsAuthenticateReqHandler(BppsInstanceData * pInst)
{
    CsrBtBppsAuthenticateReq *pMsg      = (CsrBtBppsAuthenticateReq *) pInst->recvMsgP;
    ObexUtilDigestChallengeType *chal   = ObexUtilReturnDigestChallengePointer(TRUE,
                                                                               &pMsg->password,
                                                                               pMsg->passwordLength,
                                                                               &pMsg->userId,
                                                                               &pMsg->realm,
                                                                               pMsg->realmLength);
    switch (pInst->jobChObexCommand)
    {
        case CSR_BT_OBEX_CONNECT_OPCODE:
        {
            return (csrBtBppsConnectResponseHandler(pInst->obexJobChannelInst,
                                                    pInst->targetId,
                                                    (CsrUint32) pInst->jobChLocalServerCh,
                                                    CSR_BT_OBEX_UNAUTHORIZED_CODE,
                                                    chal,
                                                    NULL));
        }
        case CSR_BT_OBEX_PUT_OPCODE:
        {
            CsrUint8 headerPriorityTable[4];
            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 4);

            return (ObexUtilServPutWithHeaderResponse(pInst->obexJobChannelInst,
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
            return (csrBtBppsGetWithHeadersResponseHandler(pInst->obexJobChannelInst,
                                                           CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX,
                                                           CSR_BT_OBEX_UNAUTHORIZED_CODE,
                                                           0,
                                                           NULL,
                                                           0,
                                                           NULL,
                                                           chal));
        }
        default:
        {
            ObexUtilPfreeDigestChallengePointer(&chal);
            return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    }
}

#ifdef CSR_AMP_ENABLE
static void updateAmpm(BppsInstanceData *pInst)
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


