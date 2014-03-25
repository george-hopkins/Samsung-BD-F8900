/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_obex.h"
#include "csr_bt_syncs_prim.h"
#include "csr_bt_syncs_handler.h"
#include "csr_bt_syncs_sef.h"
#include "sds_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_obex_auth.h"
#include "csr_bt_obex_common.h"
#include "csr_bt_obex_extraction_lib.h"
#include "csr_bt_obex_lookup_lib.h"

#include "csr_unicode.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_OBEX_SYNCS_TARGET_LENGTH                                     0x09
static const CsrUint8 syncsTargetString[CSR_BT_OBEX_SYNCS_TARGET_LENGTH] =  {'I','R','M','C','-','S','Y','N','C'};
#define CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX                             0x01

#define DEVINFO_TXT_STRING        "devinfo.txt"
#define LOG_STR                   ".log"
#define VCF_STR                   ".vcf"
#define VCS_STR                   ".vcs"
#define PB_TELECOM_STR            "telecom/pb"
#define PB_INFO_LOG_STR           "telecom/pb/info.log"
#define PB_CC_LOG_STR             "telecom/pb/luid/cc.log"
#define PB_VCF_STR                "telecom/pb.vcf"
#define PB_LUID_VCF_STR           "telecom/pb/luid/.vcf"
#define PB_LUID_STR               "telecom/pb/luid/"
#define CAL_TELECOM_STR           "telecom/cal"
#define CAL_INFO_LOG_STR          "telecom/cal/info.log"
#define CAL_CC_LOG_STR            "telecom/cal/luid/cc.log"
#define CAL_VCS_STR               "telecom/cal.vcs"
#define CAL_LUID_VCS_STR          "telecom/cal/luid/.vcs"
#define CAL_LUID_STR              "telecom/cal/luid/"

#define CAPITAL(x)    ((x >= 'a') && (x <= 'z') ? ((x) & 0xDF) : (x))
#define SYNC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX   24
#define SYNC_SUPPORTED_FORMATS_LIST_LENGTH_INDEX          69

#define SYNCS_NONE        (0)
#define SYNCS_CONNECT     (1)
#define SYNCS_GET         (2)
#define SYNCS_GET_NEXT    (3)
#define SYNCS_PUT         (4)
#define SYNCS_PUT_NEXT    (5)
#define SYNCS_SET_PATH    (6)

#define SYNCS_HEADER_SIZE   (16)
static const CsrUint8 serviceRecord[] =
{
    /* Service class ID list */
    0x09,0x00,0x01,        /* AttrID , ServiceClassIDList */
    0x35,0x03,             /* 3 bytes in total DataElSeq */
    0x19,0x11,0x04,        /* 2 byte UUID, Service class = IrMCSync */

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
    0x35,0x08,
    0x35,0x06,             /* 6 bytes in total DataElSeq */
    0x19,0x11,0x04,        /* 2 byte UUID, Service class = IrMCSync */
    0x09,0x01,0x00,        /* 2 byte uint, version = 100 */

    /* service name */
    0x09,0x01,0x00,        /* AttrId - Service Name */
    0x25,0x09,             /* 9 byte string */
    'I','r','M','C',' ','S','y','n','c',

    /* Supported Formats List */
    0x09,0x03, 0x01,       /* AttrId - Supported Formats List */
    0x35,0x02              /* 2 bytes in dataElSeq (61 : length of sequence) */
/*    0x08, 0x01, */ /* Phonebook index:63 */
/*    0x08, 0x03, */ /* Calendar */
/*    0x08, 0x05, */ /* Notes */
/*    0x08, 0x06, */ /* Messages */
};

/* == LOCAL PROTOTYPES ====================================================== */
static void csrBtObexSyncsServerStart(SyncsInstanceDataType *pInst);

static int strICmpBeginLength(char * string1, char * string2, CsrUint16 theBegin, CsrUint16 theLength);

static void csrBtSyncsAddSupportedFormats(SyncsInstanceDataType *pInst)
{
    CsrUint8 *record;
    CsrUint16 supportedFormatsListLength    = 0;
    CsrUint16 recordLength                  = sizeof(serviceRecord);

    if (CSR_BT_SYNC_NO_TYPE_SUPPORT == pInst->supportedFormats)
    {
        supportedFormatsListLength = 1;
    }
    else
    {
        CsrUint32 index;
        CsrUint16 formatMask = 0x0001;

        for (index = 0; index < CSR_BT_SYNC_NUMBER_OF_INDIVIDUAL_FORMATS; index++)
        {
            if (formatMask & pInst->supportedFormats)
            {
                supportedFormatsListLength++;
            }
            formatMask <<= 1;
        }
    }

    record = CsrPmemAlloc(recordLength + (2 * supportedFormatsListLength));
    CsrMemCpy(record, serviceRecord, recordLength);
    record[SYNC_SUPPORTED_FORMATS_LIST_LENGTH_INDEX] = (CsrUint8)(2 * supportedFormatsListLength);

    if (CSR_BT_SYNC_ANY_TYPE_SUPPORT == pInst->supportedFormats)
    {
        record[recordLength++] = 0x08;
        record[recordLength++] = CSR_BT_SYNC_PHONEBOOK_SUPPORT;
    }
    else
    {
        CsrUint32 index;
        CsrUint16 formatMask = 0x0001;

        for (index = 0; index < CSR_BT_SYNC_NUMBER_OF_INDIVIDUAL_FORMATS; index++)
        {
            if (formatMask & pInst->supportedFormats)
            {
                record[recordLength++] = 0x08;
                if (index == 0)
                {
                    record[recordLength++] = 1;
                }
                else if(index == 1)
                {
                    record[recordLength++] = 3;
                }
                else if(index == 2)
                {
                    record[recordLength++] = 5;
                }
                else if(index == 3)
                {
                    record[recordLength++] = 6;
                }
            }
            formatMask <<= 1;
        }
    }
    pInst->serviceRecordPtr = record;
    pInst->serviceRecordLength = recordLength;
}


/* --------------------------------------------------------------------------
 *                             SYNCS Handler
 * -------------------------------------------------------------------------- */
static void csrBtSyncsMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_SYNCS_PRIM, msg);
}

static void csrBtSyncsPutNextIndSend(CsrSchedQid hApp,
                                CsrUint32 connectionId,
                                CsrUint8 isFinal,
                                CsrUint16 bodyLength,
                                CsrUint16 bodyOffset,
                                CsrUint8 *payload,
                                CsrUint16 payloadLength)
{
    CsrBtSyncsPutNextInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutNextInd));

    pMsg->type = CSR_BT_SYNCS_PUT_NEXT_IND;
    pMsg->finalFlag = isFinal;
    pMsg->connectionId = connectionId;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsPutPbAddEntryIndSend(CsrSchedQid hApp,
                                      CsrUint32 connectionId,
                                      CsrUint8 isFinal,
                                      CsrUint16 wnameOffset,
                                      CsrUint16 bodyLength,
                                      CsrUint16 bodyOffset,
                                      CsrUint8 *payload,
                                      CsrUint16 payloadLength,
                                      CsrUint32 lengthOfObject)
{
    CsrBtSyncsPutPbAddEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutPbAddEntryInd));;

    pMsg->type = CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->finalFlag = isFinal;
    pMsg->ucs2nameOffset = wnameOffset;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    pMsg->lengthOfObject = lengthOfObject;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsPutPbEntryIndSend(CsrSchedQid hApp,
                                   CsrUint32 connectionId,
                                   CsrUint8 isFinal,
                                   CsrUint16 wnameOffset,
                                   CsrUint16 bodyLength,
                                   CsrUint16 bodyOffset,
                                   CsrUint8 *payload,
                                   CsrUint16 payloadLength,
                                   CsrUint32 lengthOfObject)
{
    CsrBtSyncsPutPbEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutPbEntryInd));

    pMsg->type = CSR_BT_SYNCS_PUT_PB_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->finalFlag = isFinal;
    pMsg->ucs2nameOffset = wnameOffset;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    pMsg->lengthOfObject = lengthOfObject;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsPutIndSend(CsrSchedQid hApp,
                            CsrUint32 connectionId,
                            CsrUint8 isFinal,
                            CsrUint16 nameOffset,
                            CsrUint16 bodyLength,
                            CsrUint16 bodyOffset,
                            CsrUint8 *payload,
                            CsrUint16 payloadLength,
                            CsrUint32 lengthOfObject)
{
    CsrBtSyncsPutInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutInd));

    pMsg->type = CSR_BT_SYNCS_PUT_IND;
    pMsg->connectionId = connectionId;
    pMsg->finalFlag = isFinal;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    pMsg->lengthOfObject = lengthOfObject;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsPutCalAddEntryIndSend(CsrSchedQid hApp,
                                       CsrUint32 connectionId,
                                       CsrUint8 isFinal,
                                       CsrUint16 nameOffset,
                                       CsrUint16 bodyLength,
                                       CsrUint16 bodyOffset,
                                       CsrUint8 *payload,
                                       CsrUint16 payloadLength,
                                       CsrUint32 lengthOfObject)
{
    CsrBtSyncsPutCalAddEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutCalAddEntryInd));

    pMsg->type = CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->finalFlag = isFinal;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    pMsg->totalObjectSize = lengthOfObject;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsPutCalEntryIndSend(CsrSchedQid hApp,
                                    CsrUint32 connectionId,
                                    CsrUint8 isFinal,
                                    CsrUint16 nameOffset,
                                    CsrUint16 bodyLength,
                                    CsrUint16 bodyOffset,
                                    CsrUint8 *payload,
                                    CsrUint16 payloadLength,
                                    CsrUint32 lengthOfObject)
{
    CsrBtSyncsPutCalEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsPutCalEntryInd));

    pMsg->type = CSR_BT_SYNCS_PUT_CAL_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->finalFlag = isFinal;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->bodyLength = bodyLength;
    pMsg->bodyOffset = bodyOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;
    pMsg->totalObjectSize = lengthOfObject;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetDeviceInfoIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetDeviceInfoInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetDeviceInfoInd));

    pMsg->type = CSR_BT_SYNCS_GET_DEVICE_INFO_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetPbInfoLogIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetPbInfoLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetPbInfoLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_PB_INFO_LOG_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetPbCurChangeLogIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetPbCurChangeLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetPbCurChangeLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetPbAllIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetPbAllInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetPbAllInd));

    pMsg->type = CSR_BT_SYNCS_GET_PB_ALL_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetPbEntryIndSend(CsrSchedQid hApp,
                                   CsrUint32 connectionId,
                                   CsrUint16 nameOffset,
                                   CsrUint8 *payload,
                                   CsrUint16 payloadLength)
{
    CsrBtSyncsGetPbEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetPbEntryInd));

    pMsg->type = CSR_BT_SYNCS_GET_PB_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetPbChangeLogIndSend(CsrSchedQid hApp,
                                       CsrUint32 connectionId,
                                       CsrUint16 nameOffset,
                                       CsrUint8 *payload,
                                       CsrUint16 payloadLength)
{
    CsrBtSyncsGetPbChangeLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetPbChangeLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND;
    pMsg->connectionId = connectionId;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetIndSend(CsrSchedQid hApp,
                            CsrUint32 connectionId,
                            CsrUint16 nameOffset,
                            CsrUint8 *payload,
                            CsrUint16 payloadLength)
{
    CsrBtSyncsGetInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetInd));

    pMsg->type = CSR_BT_SYNCS_GET_IND;
    pMsg->connectionId = connectionId;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetCalInfoLogIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetCalInfoLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetCalInfoLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetCalAllIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetCalAllInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetCalAllInd));

    pMsg->type = CSR_BT_SYNCS_GET_CAL_ALL_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetCalCurChangeLogIndSend(CsrSchedQid hApp, CsrUint32 connectionId)
{
    CsrBtSyncsGetCalCurChangeLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetCalCurChangeLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND;
    pMsg->connectionId = connectionId;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetCalEntryIndSend(CsrSchedQid hApp,
                                    CsrUint32 connectionId,
                                    CsrUint16 nameOffset,
                                    CsrUint8 *payload,
                                    CsrUint16 payloadLength)
{
    CsrBtSyncsGetCalEntryInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetCalEntryInd));

    pMsg->type = CSR_BT_SYNCS_GET_CAL_ENTRY_IND;
    pMsg->connectionId = connectionId;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetCalChangeLogIndSend(CsrSchedQid hApp,
                                        CsrUint32 connectionId,
                                        CsrUint16 nameOffset,
                                        CsrUint8 *payload,
                                        CsrUint16 payloadLength)
{
    CsrBtSyncsGetCalChangeLogInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsGetCalChangeLogInd));

    pMsg->type = CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND;
    pMsg->connectionId = connectionId;
    pMsg->ucs2nameOffset = nameOffset;
    pMsg->payload = payload;
    pMsg->payloadLength = payloadLength;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsGetNextIndSend(void                    *instData,
                                     CsrUint16               obexResponsePacketLength,
                                     CsrBool                 authenticated,
                                     CsrBool                 finalBitSet,
                                     CsrUint16               obexPacketLength,
                                     CsrUint8              *obexPacket)
{
    CsrBtSyncsGetNextInd *pMsg      = CsrPmemAlloc(sizeof(CsrBtSyncsGetNextInd));
    SyncsInstanceDataType * pInst   = (SyncsInstanceDataType *) instData;

    pMsg->type = CSR_BT_SYNCS_GET_NEXT_IND;
    pMsg->connectionId = pInst->serverChannel;
    csrBtSyncsMessagePut(pInst->appHandle, pMsg);
    
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static void csrBtSyncsConnectIndSend(CsrSchedQid             hApp, 
                                     CsrUint32          connectionId, 
                                     CsrInt16           nObexPacketMax, 
                                     CsrBtDeviceAddr    *pDevice,
                                     CsrUint32          length,
                                     CsrUint32          count,
                                     CsrBtConnId        btConnId)
{
    CsrBtSyncsConnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsConnectInd));

    pMsg->type                  = CSR_BT_SYNCS_CONNECT_IND;
    pMsg->connectionId          = connectionId;
    pMsg->obexPeerMaxPacketSize = (CsrUint16)(nObexPacketMax - SYNCS_HEADER_SIZE);
    pMsg->deviceAddr            = *pDevice;
    pMsg->length                = length;   
    pMsg->count                 = count;    
    pMsg->btConnId              = btConnId;

    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsAuthCfmSend(CsrSchedQid hApp)
{   /* Digest validation success */
    CsrBtSyncsAuthenticateCfm *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsAuthenticateCfm));

    pMsg->type = CSR_BT_SYNCS_AUTHENTICATE_CFM;
    csrBtSyncsMessagePut(hApp, pMsg);
}

static CsrBool csrBtSyncsCheckTargetHeader(CsrUint8   *target, 
                                           CsrUint16  targetHeaderLength)
{
    if (target && targetHeaderLength == CSR_BT_OBEX_SYNCS_TARGET_LENGTH)
    {
        if (!CsrMemCmp(target, syncsTargetString, CSR_BT_OBEX_SYNCS_TARGET_LENGTH))
        {
            return (TRUE);
        }
    }
    return (FALSE);       
}

static void csrBtSyncsServerAbortIndHandler(void              *instData,
                                            CsrUint16         descriptionOffset,
                                            CsrUint16         descriptionLength,
                                            CsrUint16         obexPacketLength,
                                            CsrUint8        *obexPacket)
{
    SyncsInstanceDataType   *pInst = (SyncsInstanceDataType *) instData;
    CsrBtSyncsAbortInd  *pMsg;
    pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsAbortInd));

    pMsg->type = CSR_BT_SYNCS_ABORT_IND;
    pMsg->connectionId = pInst->serverChannel;
    if (descriptionOffset)
    {
        pMsg->payload = obexPacket;
        pMsg->payloadLength = obexPacketLength;
        pMsg->descriptionOffset = descriptionOffset;
        pMsg->descriptionLength = descriptionLength;
    }
    else
    {
        pMsg->descriptionOffset = 0;
        pMsg->descriptionLength = 0;
        pMsg->payload = NULL;
        pMsg->payloadLength = 0;
        CsrPmemFree(obexPacket);
    }
    csrBtSyncsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSyncsDisconnectIndSend(CsrSchedQid          hApp, 
                                        CsrUint32       connectionId,
                                        CsrBtReasonCode reasonCode,
                                        CsrBtSupplier   reasonSupplier)
{
    CsrBtSyncsDisconnectInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsDisconnectInd));

    pMsg->type           = CSR_BT_SYNCS_DISCONNECT_IND;
    pMsg->connectionId   = connectionId;
    pMsg->reasonCode     = reasonCode;
    pMsg->reasonSupplier = reasonSupplier;   
    csrBtSyncsMessagePut(hApp, pMsg);
}

static void csrBtSyncsServerDisconnectIndHandler(void            *instData,
                                                 CsrBtDeviceAddr deviceAddr,
                                                 CsrBtReasonCode reasonCode,
                                                 CsrBtSupplier   reasonSupplier,
                                                 CsrUint16       obexPacketLength,
                                                 CsrUint8      *obexPacket)
{
    SyncsInstanceDataType * pInst = (SyncsInstanceDataType *) instData;

    csrBtSyncsDisconnectIndSend(pInst->appHandle, 
                                (CsrUint32) pInst->serverChannel, 
                                reasonCode, 
                                reasonSupplier);
    CSR_UNUSED(deviceAddr);
    CSR_UNUSED(obexPacketLength);
    CsrPmemFree(obexPacket);
}

static CsrUint8 csrBtSyncsServerConnectResponseHandler(SyncsInstanceDataType        *pInst,
                                                       CsrBtObexResponseCode        responseCode, 
                                                       ObexUtilDigestChallengeType  *digestChallenge,
                                                       CsrUcs2String                *description)
{
    CsrUint8  numOfHeaders           = 0;
    CsrUint8  headerPriorityTable[1] = {CSR_BT_OBEX_UTIL_DESCRIPTION_HEADER};
    CsrUint16 whoHeaderLength        = CSR_BT_OBEX_SYNCS_TARGET_LENGTH;
    CsrUint8  *who                   = (CsrUint8 *) CsrPmemAlloc(whoHeaderLength);
    responseCode                     = (CsrBtObexResponseCode)(responseCode | CSR_BT_OBEX_FINAL_BIT_MASK);
    
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
    if (description)
    {
        numOfHeaders = 1;
    }
#else
    CSR_UNUSED(description);
#endif
    CsrMemCpy(who, syncsTargetString, whoHeaderLength); 
    
    return (ObexUtilServConnectResponse(pInst->obexServerInst, 
                                        responseCode, 
                                        whoHeaderLength, 
                                        &who, 
                                        (CsrUint32)pInst->serverChannel, 
                                        numOfHeaders, 
                                        headerPriorityTable, 
                                        &digestChallenge, 
                                        &description, 
                                        0, 
                                        NULL,
                                        FALSE));
}


static void csrBtSyncsServerConnectIndHandler( void                    *instData,
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
                                               CsrUint8              *obexPacket)
{
    SyncsInstanceDataType *pInst = (SyncsInstanceDataType *)instData;
    
    pInst->obexCommand = CSR_BT_OBEX_CONNECT_OPCODE;
    if (csrBtSyncsCheckTargetHeader(target, targetHeaderLength))
    {   
        if (authenticated)
        {
            csrBtSyncsAuthCfmSend(pInst->appHandle);
        }
        csrBtSyncsConnectIndSend(pInst->appHandle,
                                 pInst->serverChannel,
                                 maxPeerObexPacketLength,
                                 &deviceAddr,
                                 length,
                                 count,
                                 cid);
    }
    else
    {   
#ifdef CSR_BT_INSTALL_OBEX_HEADER_DESCRIPTION
        CsrUcs2String *description = CsrUtf82Ucs2ByteString((CsrUint8 *) "Target missing");
#endif
        csrBtSyncsServerConnectResponseHandler(pInst,
                                               CSR_BT_OBEX_BAD_REQUEST_RESPONSE_CODE,
                                               NULL, description);
    }   
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(challenged);
    CsrPmemFree(obexPacket);
    CsrPmemFree(target);
}

static void csrBtSyncsObexPutIndHandler(SyncsInstanceDataType *pInst,
                                        CsrUint16 wnameOffset,
                                        CsrBool   finalFlag,
                                        CsrUint16 bodyLength,
                                        CsrUint16 bodyOffset,
                                        CsrUint8 *payload,
                                        CsrUint16 payloadLength,
                                        CsrUint32 lengthOfObjectValue)
{
    char  *utfName;

    /* Create a utf8 name so we can do simple comparisons. This text is never sent
       but only used internally */
    utfName = (char*) CsrUcs2ByteString2Utf8((CsrUint8*)(payload + wnameOffset));

    if (!CsrBtStrICmpLength((char*)utfName, (char*)PB_TELECOM_STR, ((CsrUint16)CsrStrLen((char*)PB_TELECOM_STR))))
    {
        if (!CsrBtStrICmpLength((char*)utfName, (char*)PB_LUID_STR, ((CsrUint16)CsrStrLen((char*)PB_LUID_STR))))
        {
            CsrUint8 begin = 0;

            while(utfName[begin] != '.')
            {
                begin++;
            }

            if (!CsrBtStrICmpLength((char*)utfName, (char*)PB_LUID_VCF_STR,
                               ((CsrUint16)CsrStrLen((char*)PB_LUID_VCF_STR))))
            {
                /* remove the part part name "telecom/pb/luid/" in UCS2 format (16 chars of each 2 byte) */
                csrBtSyncsPutPbAddEntryIndSend(pInst->appHandle,
                                               pInst->serverChannel,
                                               finalFlag,
                                               (CsrUint16)(wnameOffset + 16*2),
                                               bodyLength,
                                               bodyOffset,
                                               payload,
                                               payloadLength,
                                               lengthOfObjectValue);
            }
            else if ((!strICmpBeginLength((char*)utfName, (char*)VCF_STR, begin,
                                          ((CsrUint16)CsrStrLen((char*)VCF_STR)))) && (begin > 16))
            {
                /* remove the part part name "telecom/pb/luid/" in UCS2 format (16 chars of each 2 byte) */
                csrBtSyncsPutPbEntryIndSend(pInst->appHandle,
                                       pInst->serverChannel,
                                       finalFlag,
                                       (CsrUint16)(wnameOffset + 16*2),
                                       bodyLength,
                                       bodyOffset,
                                       payload,
                                       payloadLength,
                                       lengthOfObjectValue);
            }
            else
            {
                csrBtSyncsPutIndSend(pInst->appHandle,
                                pInst->serverChannel,
                                finalFlag,
                                wnameOffset,
                                bodyLength,
                                bodyOffset,
                                payload,
                                payloadLength,
                                lengthOfObjectValue);
            }
        }
        else
        {
            csrBtSyncsPutIndSend(pInst->appHandle,
                            pInst->serverChannel,
                            finalFlag,
                            wnameOffset,
                            bodyLength,
                            bodyOffset,
                            payload,
                            payloadLength,
                            lengthOfObjectValue);
        }
    }
    else if (!CsrBtStrICmpLength(
                 (char*)utfName, (char*)CAL_TELECOM_STR,
                 ((CsrUint16)CsrStrLen((char*)CAL_TELECOM_STR
                                   ))))
    {
        if (!CsrBtStrICmpLength((char*)utfName, (char*)CAL_LUID_STR, ((CsrUint16)CsrStrLen((char*)CAL_LUID_STR))))
        {
            CsrUint8 begin = 0;

            while (utfName[begin] != '.')
            {
                begin++;
            }

            if (!CsrBtStrICmpLength(
                    (char*)utfName, (char*)CAL_LUID_VCS_STR,
                    ((CsrUint16)CsrStrLen((char*)CAL_LUID_VCS_STR
                                      ))))
            {
                /* Remove 17 chars of each 2 byte */
                csrBtSyncsPutCalAddEntryIndSend(pInst->appHandle,
                                           pInst->serverChannel,
                                           finalFlag,
                                           (CsrUint16)(wnameOffset + 17*2),
                                           bodyLength,
                                           bodyOffset,
                                           payload,
                                           payloadLength,
                                           lengthOfObjectValue);
            }
            else if ((!strICmpBeginLength(
                          (char*)utfName, (char*)VCS_STR,
                          begin, (CsrUint8)CsrStrLen((char*)VCS_STR)
                          )) && (begin > 17))
            {
                /* Remove 17 chars of each 2 byte */
                csrBtSyncsPutCalEntryIndSend(pInst->appHandle,
                                        pInst->serverChannel,
                                        finalFlag,
                                        (CsrUint16)(wnameOffset + 17*2),
                                        bodyLength,
                                        bodyOffset,
                                        payload,
                                        payloadLength,
                                        lengthOfObjectValue);
            }
            else
            {
                csrBtSyncsPutIndSend(pInst->appHandle,
                                pInst->serverChannel,
                                finalFlag,
                                wnameOffset,
                                bodyLength,
                                bodyOffset,
                                payload,
                                payloadLength,
                                lengthOfObjectValue);
            }
        }
        else
        {
            csrBtSyncsPutIndSend(pInst->appHandle,
                            pInst->serverChannel,
                            finalFlag,
                            wnameOffset,
                            bodyLength,
                            bodyOffset,
                            payload,
                            payloadLength,
                            lengthOfObjectValue);
        }
    }
    else
    {
        csrBtSyncsPutIndSend(pInst->appHandle,
                        pInst->serverChannel,
                        finalFlag,
                        wnameOffset,
                        bodyLength,
                        bodyOffset,
                        payload,
                        payloadLength,
                        lengthOfObjectValue);
    }

    /* Free the utf8 name */
    CsrPmemFree(utfName);
}

static void csrBtSyncsServerPutNextIndHandler(void       *instData,
                                              CsrUint16  obexResponsePacketLength,
                                              CsrBool    authenticated,
                                              CsrBool    finalBitSet,
                                              CsrUint16  bodyLength,
                                              CsrUint16  bodyOffset,
                                              CsrUint16  obexPacketLength,
                                              CsrUint8 *obexPacket)
{
    SyncsInstanceDataType *pInst = (SyncsInstanceDataType *)instData;
    
    pInst->putFinalFlag = finalBitSet;
    csrBtSyncsPutNextIndSend(pInst->appHandle,
                             (CsrUint32)pInst->serverChannel,
                             finalBitSet,
                             bodyLength,
                             bodyOffset,
                             obexPacket,
                             obexPacketLength);
    CSR_UNUSED(obexResponsePacketLength);
    CSR_UNUSED(authenticated);
}

static void csrBtSyncsServerPutIndHandler(void                    *instData,
                                          CsrUint16               obexResponsePacketLength,
                                          CsrBool                 authenticated,
                                          CsrBool                 finalBitSet,
                                          CsrUint16               bodyLength,
                                          CsrUint16               bodyOffset,
                                          CsrUint16               obexPacketLength,
                                          CsrUint8              *obexPacket)
{ /* The first Put Request packet is received */
    CsrUint16  nameOffset;
    CsrUint32  lengthOfObjectValue;
    SyncsInstanceDataType *pInst = (SyncsInstanceDataType *)instData;
    
    pInst->obexCommand = CSR_BT_OBEX_PUT_OPCODE;
    CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &nameOffset);
    
    if (authenticated)
    { 
        csrBtSyncsAuthCfmSend(pInst->appHandle);
    }

    CsrBtObexObjectLengthHeaderIndex(CSR_BT_OBEX_RESPONSE_NORMAL, obexPacket, &lengthOfObjectValue);

    pInst->putFinalFlag = finalBitSet;
    csrBtSyncsObexPutIndHandler(pInst,
                                nameOffset,
                                finalBitSet,
                                bodyLength,
                                bodyOffset,
                                obexPacket,
                                obexPacketLength,
                                lengthOfObjectValue);
    if (!finalBitSet)
    { /* More OBEX PUT request packets to come */
        ObexUtilServSetPutContinueFunc(pInst->obexServerInst, csrBtSyncsServerPutNextIndHandler);
    }
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtSyncsGetHandler(SyncsInstanceDataType *pInst, CsrUint8 *payload, CsrUint16 payloadLength)
{
    char        *utfName;
    CsrUint16   nameOffset;

    if (!CsrBtObexGetNameOffset(CSR_BT_OBEX_RESPONSE_NORMAL, payload, &nameOffset))
    {
        nameOffset = 0;
    }
    
    utfName = (char*)CsrUcs2ByteString2Utf8(payload + nameOffset);

    if (!strICmpBeginLength(utfName, DEVINFO_TXT_STRING,
                            (CsrUint16)(CsrStrLen(utfName) - CsrStrLen(DEVINFO_TXT_STRING)),
                            (CsrUint16)CsrStrLen(DEVINFO_TXT_STRING)))
    {
        csrBtSyncsGetDeviceInfoIndSend(pInst->appHandle, pInst->serverChannel);
        CsrPmemFree(payload);
    }
    else if (!CsrBtStrICmpLength(utfName, PB_TELECOM_STR, (CsrUint8)CsrStrLen(PB_TELECOM_STR)))
    {
        if (!CsrBtStrICmp(utfName, PB_INFO_LOG_STR))
        {
            csrBtSyncsGetPbInfoLogIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmp(utfName, PB_CC_LOG_STR))
        {
            csrBtSyncsGetPbCurChangeLogIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmp(utfName, PB_VCF_STR))
        {
            csrBtSyncsGetPbAllIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmpLength(utfName, PB_LUID_STR, (CsrUint8) CsrStrLen(PB_LUID_STR)))
        {
            CsrUint8 begin = 0;

            while (utfName[begin] != '.')
            {
                begin++;
            }

            if (!strICmpBeginLength(utfName, VCF_STR, begin, (CsrUint8)CsrStrLen(VCF_STR)))
            {
                /* remove the part part name "telecom/pb/luid/" in UCS2 format */
                csrBtSyncsGetPbEntryIndSend(pInst->appHandle,
                                           (CsrUint32)pInst->serverChannel,
                                           (CsrUint16)(nameOffset + 16*2),
                                           payload,
                                           payloadLength);

            }
            else if (!strICmpBeginLength(utfName, LOG_STR, begin, (CsrUint8)CsrStrLen(LOG_STR)))
            {
                csrBtSyncsGetPbChangeLogIndSend(pInst->appHandle,
                                           (CsrUint32)pInst->serverChannel,
                                           (CsrUint16)(nameOffset + 16*2),
                                           payload,
                                           payloadLength);
            }
            else
            {
                csrBtSyncsGetIndSend(pInst->appHandle,
                                pInst->serverChannel,
                                nameOffset,
                                payload,
                                payloadLength);
            }
        }
        else
        {
            csrBtSyncsGetIndSend(pInst->appHandle,
                            pInst->serverChannel,
                            nameOffset,
                            payload,
                            payloadLength);
        }
    }
    else if (!CsrBtStrICmpLength(utfName, CAL_TELECOM_STR, (CsrUint16)CsrStrLen(CAL_TELECOM_STR)))
    {
        if (!CsrBtStrICmp(utfName, CAL_INFO_LOG_STR))
        {
            csrBtSyncsGetCalInfoLogIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmp(utfName, CAL_CC_LOG_STR))
        {
            csrBtSyncsGetCalCurChangeLogIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmp(utfName, CAL_VCS_STR))
        {
            csrBtSyncsGetCalAllIndSend(pInst->appHandle, pInst->serverChannel);
            CsrPmemFree(payload);
        }
        else if (!CsrBtStrICmpLength(utfName, CAL_LUID_STR, (CsrUint16)CsrStrLen(CAL_LUID_STR)))
        {
            CsrUint8 begin = 0;

            while (utfName[begin] != '.')
            {
                begin++;
            }

            if (!strICmpBeginLength(utfName, VCS_STR, begin, (CsrUint8)CsrStrLen(VCS_STR)))
            {
                csrBtSyncsGetCalEntryIndSend(pInst->appHandle,
                                        (CsrUint32)pInst->serverChannel,
                                        (CsrUint16)(nameOffset + 17*2),
                                        payload,
                                        payloadLength);
            }
            else if (!strICmpBeginLength(utfName, LOG_STR, begin, (CsrUint8)CsrStrLen(LOG_STR)))
            {
                /* remove the part part name "telecom/cal/luid/" in UCS2 format */
                csrBtSyncsGetCalChangeLogIndSend(pInst->appHandle,
                                            (CsrUint32)pInst->serverChannel,
                                            (CsrUint16)(nameOffset + 17*2),
                                            payload,
                                            payloadLength);
            }
            else
            {
                csrBtSyncsGetIndSend(pInst->appHandle,
                                    (CsrUint32)pInst->serverChannel,
                                    nameOffset,
                                    payload,
                                    payloadLength);

            }
        }
        else
        {
            csrBtSyncsGetIndSend(pInst->appHandle,
                                (CsrUint32)pInst->serverChannel,
                                nameOffset,
                                payload,
                                payloadLength);

        }
    }
    else
    {
        csrBtSyncsGetIndSend(pInst->appHandle,
                            (CsrUint32)pInst->serverChannel,
                            nameOffset,
                            payload,
                            payloadLength);

    }

    /* Free utf8 name */
    CsrPmemFree(utfName);
}

static void csrBtSyncsServerGetIndHandler(void          *instData,
                                          CsrUint16     obexResponsePacketLength,
                                          CsrBool       authenticated,
                                          CsrBool       finalBitSet,
                                          CsrUint16     obexPacketLength,
                                          CsrUint8    *obexPacket)
{ /* The first Get Request packet is received                                   */
    SyncsInstanceDataType *pInst = (SyncsInstanceDataType *)instData;

    pInst->obexCommand          = CSR_BT_OBEX_GET_OPCODE;

    if (authenticated)
    { 
        csrBtSyncsAuthCfmSend(pInst->appHandle);
    }
    csrBtSyncsGetHandler(pInst,
                         obexPacket,
                         obexPacketLength);
    ObexUtilServSetGetContinueFunc(pInst->obexServerInst, csrBtSyncsGetNextIndSend);
    CSR_UNUSED(finalBitSet);
    CSR_UNUSED(obexResponsePacketLength);
}

static void csrBtSyncsServerAuthenticateIndHandler(void            *instData,
                                                   CsrBtDeviceAddr deviceAddr,
                                                   CsrUint8        options,
                                                   CsrUint16       realmLength,
                                                   CsrUint8      *realm)
{
    SyncsInstanceDataType * pInst   = (SyncsInstanceDataType *) instData;
    CsrBtSyncsAuthenticateInd *pMsg = CsrPmemAlloc(sizeof(CsrBtSyncsAuthenticateInd));
    pMsg->type                      = CSR_BT_SYNCS_AUTHENTICATE_IND;
    pMsg->deviceAddr                = deviceAddr;
    pMsg->options                   = options;
    pMsg->realmLength               = realmLength;
    pMsg->realm                     = realm;
    csrBtSyncsMessagePut(pInst->appHandle, pMsg);
}

static void csrBtSyncsServerActivateCfmHandler(void            *instData,
                                               CsrUint8   serverChannel,
                                               psm_t           psm,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier   resultSupplier)
{
    SyncsInstanceDataType * pInst = (SyncsInstanceDataType *) instData;
    pInst->serverChannel          = serverChannel;
    
    if (resultCode     == CSR_BT_RESULT_CODE_CM_SUCCESS && 
        resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        pInst->activated = TRUE;
        ObexUtilServConnectableStart(pInst->obexServerInst, 
                                     CSR_BT_SYNCS_PROFILE_DEFAULT_MTU_SIZE,
                                     pInst->localMaxPacketSize);
    }
    else
    { /* The server cannot send give this information to the application
         as it has no API for it. Only thing to do is to activate it again      */
        csrBtObexSyncsServerStart(pInst);
    }
    CSR_UNUSED(psm);
}

static void csrBtObexSyncsServerStart(SyncsInstanceDataType *pInst)
{
    csrBtSyncsAddSupportedFormats(pInst);
    ObexUtilServActivateStart(pInst->obexServerInst,
                              pInst->secIncoming,
                              CSR_BT_OBEX_IR_MC_SYNC_SERVICE_UUID,
                              CSR_BT_SYNCS_LP_SUPERVISION_TIMEOUT,
                              pInst->serverChannel,
                              L2CA_PSM_INVALID,
                              pInst->serviceRecordLength,
                              SYNC_RFCOMM_SERVER_CHANNEL_SERVICE_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              CSR_BT_OBEX_UTIL_INVALID_SDP_RECORD_INDEX,
                              &pInst->serviceRecordPtr,
                              CSR_BT_OBJECT_TRANSFER_COD,
                              csrBtSyncsServerConnectIndHandler,
                              csrBtSyncsServerPutIndHandler,
                              csrBtSyncsServerGetIndHandler,
                              NULL,
                              csrBtSyncsServerAbortIndHandler,
                              csrBtSyncsServerDisconnectIndHandler,
                              csrBtSyncsServerAuthenticateIndHandler,
                              csrBtSyncsServerActivateCfmHandler,
                              NULL,
                              pInst->localMaxPacketSize,
                              pInst->windowSize,
                              pInst->localSrmEnable);
}

CsrUint8 CsrBtSyncsActivateReqHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsActivateReq *pMsg = (CsrBtSyncsActivateReq *)msg;

    pInst->appHandle = pMsg->appHandle;
    pInst->supportedFormats = pMsg->supportedFormats;
    pInst->localMaxPacketSize = pMsg->obexMaxPacketSize;
    pInst->windowSize = pMsg->windowSize;
    pInst->localSrmEnable = pMsg->srmEnable;
      
    csrBtObexSyncsServerStart(pInst);    
    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

void CsrBtObexSyncsDeactivateCfmHandler(void            *instData, 
                                        CsrUint8   serverChannel,
                                        psm_t           psm)
{
    SyncsInstanceDataType *pInst    = (SyncsInstanceDataType *) instData;
    CsrBtSyncsDeactivateCfm *pCfm   = pnew(CsrBtSyncsDeactivateCfm);

    pInst->activated = FALSE;
    pInst->serverChannel = serverChannel;

    pCfm->type = CSR_BT_SYNCS_DEACTIVATE_CFM;
    
    csrBtSyncsMessagePut(pInst->appHandle, pCfm);
    CSR_UNUSED(psm);
}

CsrUint8 CsrBtSyncsDeactivateReqHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CSR_UNUSED(msg);
    
    if (pInst->activated)
    {
        ObexUtilServDeactivateStart(pInst->obexServerInst, 
                                    pInst->serverChannel, 
                                    L2CA_PSM_INVALID,
                                    CsrBtObexSyncsDeactivateCfmHandler);
    }
    else
    {
        CsrBtObexSyncsDeactivateCfmHandler(pInst, pInst->serverChannel, L2CA_PSM_INVALID);
    }

    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

CsrUint8 CsrBtSyncsConnectResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsConnectRes *pMsg = (CsrBtSyncsConnectRes *) msg;

    if (pInst->activated)
    {
        return (csrBtSyncsServerConnectResponseHandler(pInst, pMsg->responseCode, NULL, NULL));
    }
    else
    {
        return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
}

CsrUint8 CsrBtSyncsAuthenticateResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsAuthenticateRes *pMsg = (CsrBtSyncsAuthenticateRes *) msg;
    if (pInst->activated)
    {
        return (ObexUtilServAuthenticateResponse(pInst->obexServerInst, pMsg->passwordLength, &pMsg->password, &pMsg->userId));
    }
    else
    {
        return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
}


CsrUint8 CsrBtSyncsAuthenticateReqHandler(SyncsInstanceDataType *pInst, void *msg) 
{
    CsrBtSyncsAuthenticateReq   *pMsg   = (CsrBtSyncsAuthenticateReq *) msg;
    ObexUtilDigestChallengeType *chal   = ObexUtilReturnDigestChallengePointer(TRUE, 
                                                                               &pMsg->password, 
                                                                               pMsg->passwordLength,  
                                                                               &pMsg->userId, 
                                                                               &pMsg->realm, 
                                                                               pMsg->realmLength);

    switch (pInst->obexCommand)
    {
        case CSR_BT_OBEX_CONNECT_OPCODE:
        {
            return (csrBtSyncsServerConnectResponseHandler(pInst, CSR_BT_OBEX_UNAUTHORIZED_CODE, 
                                                           chal, NULL));
        }
        case CSR_BT_OBEX_PUT_OPCODE:
        {
            CsrUint8 headerPriorityTable[4]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

            return (ObexUtilServPutWithHeaderResponse(pInst->obexServerInst, 
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
            CsrUint8 headerPriorityTable[6]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                                CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};

            return (ObexUtilServGetWithHeaderResponse(pInst->obexServerInst, 
                                                      CSR_BT_OBEX_UNAUTHORIZED_CODE,
                                                      &chal, 
                                                      headerPriorityTable, 
                                                      CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX, 
                                                      0,
                                                      NULL, 
                                                      NULL, 
                                                      0, 
                                                      NULL, 
                                                      0, 
                                                      NULL, 
                                                      NULL, 
                                                      0,
                                                      CSR_BT_OBEX_SRMP_INVALID)); 
        }
        default:
        {
            ObexUtilPfreeDigestChallengePointer(&chal);
            return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
        }
    } 
}

static void csrBtSyncsMapResponseCode(CsrBool               finalFlag,
                                      CsrBtObexResponseCode *responseCode)
{
    CsrBtObexResponseCode rspCode = (*responseCode & ~CSR_BT_OBEX_FINAL_BIT_MASK);

    if (rspCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && !finalFlag)
    { 
        *responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }
}

CsrUint8 CsrBtSyncsServerCommonGetResHandler(SyncsInstanceDataType  *pInst, 
                                             CsrUint8               finalFlag,
                                             CsrBtObexResponseCode  responseCode,
                                             CsrUint8               firstHeader,
                                             CsrUint8               secondHeader,
                                             CsrUint8               bodyHeaderPriorityIndex,
                                             CsrUint32              totalObjSize,
                                             CsrUint8               *body,
                                             CsrUint16              bodyLength)
{
    if (finalFlag && responseCode == CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
    {
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else if (!finalFlag && responseCode == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE)
    {
        responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
    }

    if (pInst->activated)
    {    
        if (finalFlag)
        {
            CsrUint8 headerPriorityTable[6];

            CsrMemSet(headerPriorityTable, CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 6);

            if (totalObjSize > 0)
            { /* The lengthOfObject header must be included                     */
                headerPriorityTable[0] = firstHeader;
                headerPriorityTable[1] = secondHeader;
            }
            else
            { /* The lengthOfObject header is not included                      */
                headerPriorityTable[0] = secondHeader;
            }

            return (ObexUtilServGetWithHeaderResponse(pInst->obexServerInst, 
                                                      responseCode,
                                                      NULL, 
                                                      headerPriorityTable, 
                                                      bodyHeaderPriorityIndex, 
                                                      totalObjSize,
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
        else
        {
            return (ObexUtilServGetResponse(pInst->obexServerInst,
                                            responseCode,
                                            &body,
                                            bodyLength,
                                            CSR_BT_OBEX_SRMP_INVALID));
        }
    }
    else
    {
        CsrPmemFree(body);
        return CSR_BT_OBEX_UTIL_STATUS_EXCEPTION;
    }
}

CsrUint8 CsrBtSyncsServerCommonPutResHandler(SyncsInstanceDataType *pInst, CsrBtObexResponseCode msgResponseCode, void *receivedMessage)
{
        CsrBtObexResponseCode responseCode;
        CsrBtSyncsPutRes *pMsg           = (CsrBtSyncsPutRes *)receivedMessage;
        CsrUint8 headerPriorityTable[4]  = {CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER, 
                                            CSR_BT_OBEX_UTIL_UNDEFINE_HEADER};
        CsrUint8  *applicationHeader;
        CsrUint16 totalLength;

        totalLength = (CsrUint16)(pMsg->appParameterLength + CSR_BT_OBEX_HEADER_LENGTH);
        applicationHeader = (CsrUint8 *) CsrPmemAlloc(totalLength);
    
        applicationHeader[0] = CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER;
        applicationHeader[1] = (CsrUint8)(totalLength >> 8);
        applicationHeader[2] = (CsrUint8)(totalLength);
        CsrMemCpy(&applicationHeader[3], pMsg->appParameter, pMsg->appParameterLength);
    

        
        if (msgResponseCode != CSR_BT_OBEX_SUCCESS_RESPONSE_CODE && msgResponseCode != CSR_BT_OBEX_CONTINUE_RESPONSE_CODE)
        {
            responseCode = msgResponseCode;
        }
        else
        {
            if (pInst->putFinalFlag)
            {
                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            }
            else
            {
                responseCode = CSR_BT_OBEX_CONTINUE_RESPONSE_CODE;
            }
        }
        
        if (pMsg->appParameterLength)
        {
            headerPriorityTable[0] = CSR_BT_OBEX_UTIL_APP_PARAMETERS_HEADER;
            CsrPmemFree(pMsg->appParameter);
        }

        return (ObexUtilServPutWithHeaderResponse(pInst->obexServerInst,
                                                  responseCode,
                                                  NULL,
                                                  headerPriorityTable,
                                                  NULL,   
                                                  NULL,
                                                  totalLength,
                                                  &applicationHeader,
                                                  0,
                                                  NULL,
                                                  CSR_BT_OBEX_SRMP_INVALID));
        
}


/******************* calendar functions start ***************************/
CsrUint8 CsrBtSyncsPutCalAddEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutCalAddEntryRes *pMsg = (CsrBtSyncsPutCalAddEntryRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsPutCalEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutCalEntryRes *pMsg = (CsrBtSyncsPutCalEntryRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsGetCalAllResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetCalAllRes *pMsg = (CsrBtSyncsGetCalAllRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                CSR_BT_UNUSED_LENGTH,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetCalEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetCalEntryRes *pMsg = (CsrBtSyncsGetCalEntryRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetCalInfoLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetCalInfoLogRes *pMsg = (CsrBtSyncsGetCalInfoLogRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetCalCurChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetCalCurChangeLogRes *pMsg = (CsrBtSyncsGetCalCurChangeLogRes *)msg;

    csrBtSyncsMapResponseCode(TRUE, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                TRUE,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->changeCounterLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->changeCounterLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->changeCounter, 
                                                (CsrUint16)pMsg->changeCounterLength));
}

CsrUint8 CsrBtSyncsGetCalChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetCalChangeLogRes *pMsg = (CsrBtSyncsGetCalChangeLogRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}
/******************** calendar functions end ****************************/

/******************* phonebook functions start **************************/
CsrUint8 CsrBtSyncsPutPbAddEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutPbAddEntryRes *pMsg = (CsrBtSyncsPutPbAddEntryRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsPutPbEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutPbEntryRes *pMsg = (CsrBtSyncsPutPbEntryRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsGetPbAllResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetPbAllRes *pMsg = (CsrBtSyncsGetPbAllRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                CSR_BT_UNUSED_LENGTH,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetPbEntryResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetPbEntryRes *pMsg = (CsrBtSyncsGetPbEntryRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetPbChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetPbChangeLogRes *pMsg = (CsrBtSyncsGetPbChangeLogRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                 CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}


CsrUint8 CsrBtSyncsGetPbInfoLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetPbInfoLogRes *pMsg = (CsrBtSyncsGetPbInfoLogRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetDeviceInfoResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetDeviceInfoRes *pMsg = (CsrBtSyncsGetDeviceInfoRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetPbCurChangeLogResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetPbCurChangeLogRes *pMsg = (CsrBtSyncsGetPbCurChangeLogRes *)msg;

    csrBtSyncsMapResponseCode(TRUE, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                TRUE,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->changeCounterLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                          CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->changeCounterLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                          CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->changeCounter, 
                                                (CsrUint16)pMsg->changeCounterLength));
}

CsrUint8 CsrBtSyncsPutResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutRes *pMsg = (CsrBtSyncsPutRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsPutNextResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsPutNextRes *pMsg = (CsrBtSyncsPutNextRes *) msg;

    return (CsrBtSyncsServerCommonPutResHandler(pInst, pMsg->responseCode, msg));
}

CsrUint8 CsrBtSyncsGetNextResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetNextRes *pMsg = (CsrBtSyncsGetNextRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                CSR_BT_UNUSED_LENGTH,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}

CsrUint8 CsrBtSyncsGetResHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtSyncsGetRes *pMsg = (CsrBtSyncsGetRes *)msg;

    csrBtSyncsMapResponseCode(pMsg->finalFlag, &pMsg->responseCode);

    return (CsrBtSyncsServerCommonGetResHandler(pInst,
                                                pMsg->finalFlag,
                                                pMsg->responseCode,
                                                CSR_BT_OBEX_UTIL_LENGTH_HEADER,
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_UTIL_BODY_HEADER:
                                                                                 CSR_BT_OBEX_UTIL_UNDEFINE_HEADER)),
                                                (CsrUint8)(((pMsg->bodyLength) ? CSR_BT_OBEX_SYNCS_GET_BODY_HEADER_INDEX : 
                                                                                 CSR_BT_OBEX_UTIL_INVALID_BODY_HEADER_INDEX)),
                                                pMsg->totalObjectSize,
                                                pMsg->body, 
                                                (CsrUint16)pMsg->bodyLength));
}
/******************** phonebook functions end ***************************/


/* == LOCAL FUNCTIONS ======================================================= */
static int strICmpBeginLength(char * string1, char * string2, CsrUint16 theBegin, CsrUint16 theLength)
{
    CsrUint16    n;

    string1 += theBegin;
    for ( n = theBegin; n < (theBegin + theLength); n++)
    {
        if (CAPITAL(*string1) != CAPITAL(*string2))
        {
            return (*string1 - *string2);
        }
        string1++;
        string2++;
    }

    return 0;
}

/* Security level change and confirm-sender */
void CsrBtSyncsSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtSyncsSecurityInCfm *prim;
    prim = (CsrBtSyncsSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtSyncsSecurityInCfm));
    prim->type = CSR_BT_SYNCS_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    csrBtSyncsMessagePut(appHandle, prim);
}


CsrUint8 CsrBtSyncsSecurityInReqHandler(SyncsInstanceDataType *pInst, void *msg)
{
    CsrBtResultCode rval;
    CsrBtSyncsSecurityInReq *prim;

    prim = (CsrBtSyncsSecurityInReq*)msg;

    rval = CsrBtScSetSecInLevel(&pInst->secIncoming, prim->secLevel,
        CSR_BT_OBEX_IR_MC_SYNC_MANDATORY_SECURITY_INCOMING,
        CSR_BT_OBEX_IR_MC_SYNC_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER),

    CsrBtSyncsSecurityInCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_OBEX_PROFILES);
    ObexUtilServChangeIncomingSecurity(pInst->obexServerInst, pInst->secIncoming);
    return CSR_BT_OBEX_UTIL_STATUS_ACCEPTED;
}

