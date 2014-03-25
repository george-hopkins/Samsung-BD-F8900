/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "sds_prim.h"
#include "csr_bt_hcrp_handler.h"
#include "csr_bt_hcrp_prim.h"
#include "csr_bt_hcrp_lib.h"
#include "csr_bt_hcrp_sef.h"
#include "csr_bt_hcrp_service_record.h"
#include "csr_bt_sd_private_lib.h"

static void csrBtHcrpHandleControlPacket(hcrpInstanceData_t* me);
static void csrBtHcrpHandle_CR_DataChannelCreditGrantRequest(hcrpInstanceData_t* me);
static void csrBtHcrpHandle_CR_DataChannelCreditRequestRequest(hcrpInstanceData_t* me);

#define SERVICE_RECORD_SET_BYTE_AT(dest, index, value) \
(((CsrUint8 *) dest)[(index)] = (value))

#define SERVICE_RECORD_SET_WORD_AT(dest, index, value) \
((((CsrUint8 *) dest)[index] = ((CsrUint8) (((CsrUint16) value) >> 8))), \
 (((CsrUint8 *) dest)[(index) + 1] = ((CsrUint8) (((CsrUint16) value) & 0x00FF))))

#define SERVICE_RECORD_SET_ATTR_ID(dest, attrid) \
SERVICE_RECORD_SET_BYTE_AT(dest, 0, 0x09), \
SERVICE_RECORD_SET_WORD_AT(dest, 1, attrid), \
dest += (sizeof(CsrUint8) + sizeof(CsrUint16))

#define SERVICE_RECORD_SET_STRING(dest, string) \
SERVICE_RECORD_SET_BYTE_AT(dest, 0, 0x25), \
SERVICE_RECORD_SET_BYTE_AT(dest, 1, (CsrUint8) CsrStrLen((char*) string)), \
CsrMemCpy( (void*)(dest+2), (void*)string, CsrStrLen((char*)string)), \
dest += (sizeof(CsrUint8) + sizeof(CsrUint8) + CsrStrLen((char*)string))

/* -------------------------------------------------------------------------------- */
/* -                                  Internal functionsHCRP                      - */
/* -------------------------------------------------------------------------------- */
static void CsrBtHcrpMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_HCRP_PRIM, msg);
}

static void csrBtHcrpHouseCleaningSend(hcrpInstanceData_t *instData)
{
    CsrBtHcrpHouseCleaning    *prim;

    instData->restoreHcrpFlag = TRUE;

    prim = (CsrBtHcrpHouseCleaning *)CsrPmemAlloc(sizeof(CsrBtHcrpHouseCleaning));
    prim->type = CSR_BT_HCRP_HOUSE_CLEANING;    
    CsrBtHcrpMessagePut(CSR_BT_HCRP_IFACEQUEUE, prim);
}

/* -------------------------------------------------------------------------------- */
/* -                                  Local upstream functionsHCRP                - */
/* -------------------------------------------------------------------------------- */
static void csrBtHcrpDataPathSend(hcrpInstanceData_t *instData, void *msg)
{
    CsrSchedQid target;
    target = (instData->dataHandle != CSR_SCHED_QID_INVALID) ? instData->dataHandle : instData->ctrlHandle;
    CsrBtHcrpMessagePut(target, msg);
}

static void csrBtHcrpDataL2CaConnectIndSend(hcrpInstanceData_t *instData)
{
    CsrBtHcrpL2caConnectInd *prim;

    prim          = (CsrBtHcrpL2caConnectInd*)CsrPmemAlloc(sizeof(CsrBtHcrpL2caConnectInd));
    prim->type    = CSR_BT_HCRP_L2CA_CONNECT_IND;

    CsrBtHcrpMessagePut(instData->ctrlHandle, prim);
}

static void csrBtHcrpDataL2CaDisconnectIndSend(hcrpInstanceData_t *instData,
                                                        CsrBtReasonCode     reasonCode,
                                                        CsrBtSupplier reasonSupplier)
{
    CsrBtHcrpL2caDisconnectInd* prim;

    prim = (CsrBtHcrpL2caDisconnectInd*)CsrPmemAlloc(sizeof(CsrBtHcrpL2caDisconnectInd));
    prim->type              = CSR_BT_HCRP_L2CA_DISCONNECT_IND;
    prim->reasonCode        = reasonCode;
    prim->reasonSupplier    = reasonSupplier;

    CsrBtHcrpMessagePut(instData->ctrlHandle, prim);
}

static void csrBtHcrpDataL2CaDataIndSend(hcrpInstanceData_t *instData,
                                    CsrUint16 payloadLength,
                                    CsrUint8  *payload)
{
    CsrBtHcrpL2caDataInd* prim;

    prim                 = (CsrBtHcrpL2caDataInd*)CsrPmemAlloc(sizeof(CsrBtHcrpL2caDataInd));
    prim->type           = CSR_BT_HCRP_L2CA_DATA_IND;
    prim->payloadLength  = payloadLength;
    prim->payload        = payload;

    csrBtHcrpDataPathSend(instData, prim);
}

static void csrBtHcrpTxPacket(hcrpInstanceData_t* instData)
{
    CsrBtCml2caDataReqSend(instData->cntlCid, instData->payLoadLength, instData->payLoad);
}

static void csrBtHcrpGetLptStatusIndSend(hcrpInstanceData_t *instData,
                                    CsrUint16 pdu,
                                    CsrUint16 trans)
{
    CsrBtHcrpGetLptStatusInd* prim;

    prim          = (CsrBtHcrpGetLptStatusInd*)CsrPmemAlloc(sizeof(CsrBtHcrpGetLptStatusInd));
    prim->type    = CSR_BT_HCRP_GET_LPT_STATUS_IND;
    prim->pduId   = pdu;
    prim->transId = trans;

    csrBtHcrpDataPathSend(instData, prim);
}

static void csrBtHcrpGetIeee1284IndSend(hcrpInstanceData_t *instData,
                                   CsrUint16 pdu,
                                   CsrUint16 trans)
{
    CsrBtHcrpGetIeee1284Ind *prim;

    prim           = (CsrBtHcrpGetIeee1284Ind*)CsrPmemAlloc(sizeof(CsrBtHcrpGetIeee1284Ind));
    prim->type     = CSR_BT_HCRP_GET_IEEE_1284_IND;
    prim->pduId    = pdu;
    prim->transId  = trans;

    CsrBtHcrpMessagePut(instData->ctrlHandle, prim);
}

static void csrBtHcrpStatusIndSend(hcrpInstanceData_t *instData,
                              CsrBool status)
{
    CsrBtHcrpStatusInd *prim;

    if((instData->ctrlHandle != instData->dataHandle) &&
       (instData->dataHandle != CSR_SCHED_QID_INVALID))
    {
        prim           = (CsrBtHcrpStatusInd*)CsrPmemAlloc(sizeof(CsrBtHcrpStatusInd));
        prim->type     = CSR_BT_HCRP_STATUS_IND;
        prim->connect  = status;
        CsrBtHcrpMessagePut(instData->dataHandle, prim);
    }
}

/* -------------------------------------------------------------------------------- */
/* -                                  HCRP handlers                               - */
/* -------------------------------------------------------------------------------- */
void CsrBtHcrpIgnoreMessageHandler(hcrpInstanceData_t* instData)
{
    /* do nothing */
    CSR_UNUSED(instData);
}

void CsrBtHcrpActivateReqHandler(hcrpInstanceData_t* instData)
{
    CsrBtHcrpActivateReq* pMsg = instData->pRecvMsg;

    instData->ctrlHandle  = pMsg->appHandle;
    instData->state       = HCRP_INITIALISING_STATE;
    instData->ieee        = pMsg->ieee1284Id;

    /* HCRP initialisation stage 1 - register the L2CAP HCRP control channel. */
    CsrBtCml2caRegisterReqSend(CSR_BT_HCRP_IFACEQUEUE, HCRP_CNTL_PSM);
}

void CsrBtHcrpDeactiveReqHandler(hcrpInstanceData_t* instData)
{
    instData->state = HCRP_DEACTIVATING_STATE;

    CsrBtCmSdsUnRegisterReqSend(CSR_BT_HCRP_IFACEQUEUE, instData->hServiceRec);
}

void CsrBtHcrpRegisterDataPathReqHandler(hcrpInstanceData_t *instData)
{
    CsrBtHcrpRegisterDataPathReq *pMsg;
    CsrBtHcrpRegisterDataPathCfm *cfm;

    /* Store handle */
    pMsg = (CsrBtHcrpRegisterDataPathReq*)instData->pRecvMsg;
    instData->dataHandle = pMsg->dataHandle;

    /* Send confirm */
    cfm = (CsrBtHcrpRegisterDataPathCfm*)CsrPmemAlloc(sizeof(CsrBtHcrpRegisterDataPathCfm));
    cfm->type = CSR_BT_HCRP_REGISTER_DATA_PATH_CFM;
    CsrBtHcrpMessagePut(instData->dataHandle, cfm);
}

void CsrBtHcrpSaveMessageHandler(hcrpInstanceData_t* instData)
{
    CsrMessageQueuePush(&instData->pSaveQueue, CSR_BT_HCRP_PRIM, instData->pRecvMsg);
    instData->pRecvMsg = NULL;
}

void CsrBtHcrpGetLptStatusResHandler(hcrpInstanceData_t* instData)
{
    CsrBtHcrpGetLptStatusRes* pMsg = instData->pRecvMsg;
    CsrUint16 len;

    CsrPmemFree(instData->payLoad);

    len = HCRP_PDU_ID_SIZE + HCRP_TRANS_ID_SIZE + HCRP_PARAM_LENGTH_SIZE +
        HCRP_STATUS_CODE_SIZE + HCRP_LPT_STATUS_SIZE;

    instData->payLoad = CsrPmemAlloc(len);

    HCRP_SET_PDU_ID(instData->payLoad, pMsg->pduId);
    HCRP_SET_TRANS_ID(instData->payLoad, pMsg->transId);
    HCRP_SET_PARAM_LENGTH(instData->payLoad, HCRP_STATUS_CODE_SIZE + HCRP_LPT_STATUS_SIZE);
    HCRP_SET_STATUS_CODE(instData->payLoad, 0x01);    /* status code = success. */
    HCRP_SET_LPT_STATUS(instData->payLoad, pMsg->lptStatus);
    instData->payLoadLength = len;

    csrBtHcrpTxPacket(instData);
}

void CsrBtHcrpGetIeee1284ResHandler(hcrpInstanceData_t* instData)
{
    CsrBtHcrpGetIeee1284Res* pMsg = instData->pRecvMsg;
    CsrUint16 len;

    CsrPmemFree(instData->payLoad);

    len = (CsrUint16)(HCRP_PDU_ID_SIZE + HCRP_TRANS_ID_SIZE + HCRP_PARAM_LENGTH_SIZE +
                     HCRP_STATUS_CODE_SIZE + 2 + CsrStrLen((char*) pMsg->ieee1284));

    instData->payLoad = CsrPmemAlloc(len);

    HCRP_SET_PDU_ID(instData->payLoad, pMsg->pduId);
    HCRP_SET_TRANS_ID(instData->payLoad, pMsg->transId);
    HCRP_SET_PARAM_LENGTH(instData->payLoad, HCRP_STATUS_CODE_SIZE + CsrUtf8StringLengthInBytes(pMsg->ieee1284));
    HCRP_SET_STATUS_CODE(instData->payLoad, 0x01); /* status code = success. */
    HCRP_SET_WORD_AT(instData->payLoad, 8, CsrStrLen((char*) pMsg->ieee1284));
    CsrMemCpy(&instData->payLoad[10], pMsg->ieee1284, CsrUtf8StringLengthInBytes(pMsg->ieee1284));
    instData->payLoadLength = len;

    csrBtHcrpTxPacket(instData);
}

static void csrBtHcrpHandle_UnsupportedControlPDU(hcrpInstanceData_t* instData,
                                                  CsrUint16 pdu,
                                                  CsrUint16 trans)
{
    CsrUint16 len;

    /* We have to respond unsupported to any requests not handled */
    len = HCRP_PDU_ID_SIZE + HCRP_TRANS_ID_SIZE + HCRP_PARAM_LENGTH_SIZE + HCRP_STATUS_CODE_SIZE;
    instData->payLoad = CsrPmemAlloc(len);

    HCRP_SET_PDU_ID(instData->payLoad, pdu);
    HCRP_SET_TRANS_ID(instData->payLoad, trans);
    HCRP_SET_PARAM_LENGTH(instData->payLoad, HCRP_STATUS_CODE_SIZE);
    HCRP_SET_STATUS_CODE(instData->payLoad, 0x00);    /* status code = unsupported */
    instData->payLoadLength = len;
    csrBtHcrpTxPacket(instData);

}

/* -------------------------------------------------------------------------------- */
/* -                                  CM handlers                                 - */
/* -------------------------------------------------------------------------------- */
void CsrBtHcrpCmL2caRegisterCfm(hcrpInstanceData_t* instData)
{
    CsrBtCmL2caRegisterCfm* prim = (CsrBtCmL2caRegisterCfm*) instData->pRecvMsg;

    switch (prim->localPsm)
    {
        case HCRP_CNTL_PSM:
            {
                instData->state = HCRP_INITIALISING_STATE;

                /* HCRP initialisation stage 2 - the HCRP control channel has been
                   registered, so register the L2CAP HCRP data channel. */
                CsrBtCml2caRegisterReqSend(CSR_BT_HCRP_IFACEQUEUE, HCRP_DATA_PSM);
                break;
            }
        case HCRP_DATA_PSM:
        {
            if(!instData->sdsRegistered)
            {
                CsrUint8* pServiceRecord;
                CsrUint8* pServiceRecordPosn;
                CsrUint16 serviceRecordLength = sizeof(hcrpServiceRecord);

                instData->state = HCRP_INITIALISING_STATE;

                /* HCRP initialisation stage 3 - both the HCRP control and data channels
                   have now been registered, send the HCRP service record to the connection
                   manager to be registered. */

                if (instData->ieee)
                {
                    serviceRecordLength += (5 + (CsrUint16) CsrStrLen((char*)instData->ieee));
                }

                pServiceRecordPosn = pServiceRecord = (CsrUint8*) CsrPmemAlloc(serviceRecordLength);

                if (pServiceRecordPosn)
                {
                    CsrMemCpy(pServiceRecord, hcrpServiceRecord, serviceRecordLength);

                    pServiceRecordPosn += sizeof(hcrpServiceRecord);

                    if (instData->ieee)
                    {
                        SERVICE_RECORD_SET_ATTR_ID(pServiceRecordPosn, 0x0300);
                        SERVICE_RECORD_SET_STRING(pServiceRecordPosn, instData->ieee);
                    }

                    CsrBtCmSdsRegisterReqSend(CSR_BT_HCRP_IFACEQUEUE, pServiceRecord, serviceRecordLength);
                }
            }
            else
            {
                instData->state = HCRP_READY_STATE;
                csrBtHcrpHouseCleaningSend(instData);
            }
                break;
            }

        default:
        {
            /* error! */
            instData->state = HCRP_IDLE_STATE;
            break;
        }        
    }
}

void CsrBtHcrpcmL2caConnectAcceptCfm(hcrpInstanceData_t* instData)
{
    CsrBtCmL2caConnectAcceptCfm* prim = (CsrBtCmL2caConnectAcceptCfm*) instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->bdAddr = prim->deviceAddr;

        switch (prim->localPsm)
        {
            case HCRP_CNTL_PSM:
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr, prim->btConnId);
                /* HCRP initialisation stage 6 - the client is making an HCRP control
                   channel connection. */
                if(instData->state == HCRP_DEACTIVATING_STATE)
                {
                    CsrBtCml2caDisconnectReqSend(prim->btConnId);
                }
                else
                {
                    instData->cntlConnected = TRUE;
                    instData->cntlCid = prim->btConnId;
                    instData->state = HCRP_CONNECTING_STATE;
                    CsrBtCml2caConnectAcceptReqSend(CSR_BT_HCRP_IFACEQUEUE,
                                            HCRP_DATA_PSM,
                                            CSR_BT_RENDERING_MAJOR_SERVICE_MASK|CSR_BT_IMAGING_MAJOR_DEVICE_MASK|CSR_BT_PRINTER_IMAGING_MINOR_DEVICE_MASK,
                                            instData->secIncoming,
                                            CSR_BT_HCRP_PROFILE_DEFAULT_MTU_SIZE, CSR_BT_DIRECT_PRINTING_UUID);
                }
                break;
            }
            case HCRP_DATA_PSM:
            {
                CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL, prim->deviceAddr, prim->btConnId);
                /* HCRP initialisation stage 7 - the client is making an HCRP data
                   channel connection. */
                instData->dataConnected = TRUE;
                instData->dataCid = prim->btConnId;
                instData->state = HCRP_CONNECTED_STATE;

                /* inform the control application that the data channel has connected. */
                csrBtHcrpDataL2CaConnectIndSend(instData);

                /* inform data application */
                csrBtHcrpStatusIndSend(instData, TRUE);

                break;
            }
            default:
                /* error! */
                break;
        }
    }
}

void CsrBtHcrpcmL2caCancelConnectAcceptCfm(hcrpInstanceData_t* instData)
{
    CsrBtCmL2caCancelConnectAcceptCfm *cmPrim = (CsrBtCmL2caCancelConnectAcceptCfm *) instData->pRecvMsg;
    /* the cancel is successful so send the cancel confirmation to
    * the higher layer and set instance variables */

    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* HCRP deactivating - HouseCleaning */
        instData->state = HCRP_IDLE_STATE;
        csrBtHcrpHouseCleaningSend(instData);
    }
    else
    {
        /* May fail if connect crossing, wait for connection... */
    }
}
void CsrBtHcrpcmL2caDataInd(hcrpInstanceData_t* instData)
{
    CsrBtCmL2caDataInd* cmPrim = (CsrBtCmL2caDataInd*) instData->pRecvMsg;

    CsrBtCmL2caDataResSend(cmPrim->btConnId);

    if (cmPrim->btConnId == instData->cntlCid)
    {
        instData->payLoadLength = cmPrim->length;
        instData->payLoad = cmPrim->payload;
        csrBtHcrpHandleControlPacket(instData);
    }
    else
    {
        /* inform the application of the data. */
        csrBtHcrpDataL2CaDataIndSend(instData, cmPrim->length, cmPrim->payload);
    }
}

void CsrBtHcrpCmL2CaDisconnectInd(hcrpInstanceData_t* instData)
{
    CsrBtCmL2caDisconnectInd* prim = (CsrBtCmL2caDisconnectInd*) instData->pRecvMsg;

    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, instData->bdAddr, prim->btConnId);

        if (prim->btConnId == instData->cntlCid)
        {
            instData->cntlConnected = FALSE;
        }
        else if (prim->btConnId == instData->dataCid)
        {
            instData->dataConnected = FALSE;

            /* inform data app of disconnect */
            csrBtHcrpStatusIndSend(instData, FALSE);

            /* inform the control application that the data channel has disconnected. */
            csrBtHcrpDataL2CaDisconnectIndSend(instData, prim->reasonCode, prim->reasonSupplier);
        }
        else
        {    /* error! */
        }

    if(instData->state == HCRP_DEACTIVATING_STATE)
    {
        instData->state = HCRP_IDLE_STATE;
        csrBtHcrpHouseCleaningSend(instData);
    }
    else
    {
        if (!instData->cntlConnected && !instData->dataConnected)
        {
            instData->state = HCRP_READY_STATE;

            /* place device back into page scan mode so that it can be connected to again - note
               that the CM automatically takes the device out of page scan mode when the connection
               was originally made. */
            CsrBtCml2caConnectAcceptReqSend(CSR_BT_HCRP_IFACEQUEUE,
                                       HCRP_CNTL_PSM,
                                       CSR_BT_RENDERING_MAJOR_SERVICE_MASK|CSR_BT_IMAGING_MAJOR_DEVICE_MASK|CSR_BT_PRINTER_IMAGING_MINOR_DEVICE_MASK,
                                       instData->secIncoming,
                                       CSR_BT_HCRP_PROFILE_DEFAULT_MTU_SIZE,
                                       CSR_BT_DIRECT_PRINTING_UUID);
        }
    }
}

void CsrBtHcrpCmSdsRegisterCfm(hcrpInstanceData_t* instData)
{
    CsrBtCmSdsRegisterCfm* pMsg = (CsrBtCmSdsRegisterCfm*) instData->pRecvMsg;

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        /* HCRP initialisation stage 4 - service record register succeeded - store
           the service handle and place device in state where its waiting to be
           discovered. */
        instData->state = HCRP_READY_STATE;
        instData->hServiceRec = pMsg->serviceRecHandle;
        instData->sdsRegistered = TRUE;

        /* HCRP initialisation stage 5 - place device into page scan mode so that
           it can be discovered. */
        CsrBtCml2caConnectAcceptReqSend(CSR_BT_HCRP_IFACEQUEUE,
                                   HCRP_CNTL_PSM,
                                   CSR_BT_RENDERING_MAJOR_SERVICE_MASK|CSR_BT_IMAGING_MAJOR_DEVICE_MASK|CSR_BT_PRINTER_IMAGING_MINOR_DEVICE_MASK,
                                   instData->secIncoming,
                                   CSR_BT_HCRP_PROFILE_DEFAULT_MTU_SIZE,
                                   CSR_BT_DIRECT_PRINTING_UUID);
    }
    else
    {
        instData->state = HCRP_IDLE_STATE;
    }
    csrBtHcrpHouseCleaningSend(instData);
}

void CsrBtHcrpcmSdsUnRegisterCfm(hcrpInstanceData_t* instData)
{
    CsrBtCmSdsUnregisterCfm* pMsg = (CsrBtCmSdsUnregisterCfm*) instData->pRecvMsg;

    /* HCRP deactivating - cancel AcceptConnect */
    CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HCRP_IFACEQUEUE, HCRP_CNTL_PSM);

    if (pMsg->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && pMsg->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->sdsRegistered = FALSE;
    }
}

/* == LOCAL FUNCTIONS ============================================================ */

static void csrBtHcrpHandleControlPacket(hcrpInstanceData_t* instData)
{
    CsrUint16 pduId, transId;

    pduId    = HCRP_GET_PDU_ID(instData->payLoad);
    transId    = HCRP_GET_TRANS_ID(instData->payLoad);

    switch (pduId)
    {
        case CR_DataChannelCreditGrant:
            csrBtHcrpHandle_CR_DataChannelCreditGrantRequest(instData);
            break;

        case CR_DataChannelCreditRequest:
            csrBtHcrpHandle_CR_DataChannelCreditRequestRequest(instData);
            break;

        case CR_GetLPTStatus:
            csrBtHcrpGetLptStatusIndSend(instData, pduId, transId);
            break;

        case CR_Get1284ID:
            csrBtHcrpGetIeee1284IndSend(instData, pduId, transId);
            break;

        default:
            csrBtHcrpHandle_UnsupportedControlPDU(instData, pduId, transId);
            break;
    }
}

static void csrBtHcrpHandle_CR_DataChannelCreditGrantRequest(hcrpInstanceData_t* instData)
{
    CsrUint16 pduId;
    CsrUint16 transId;
    CsrUint16 len;

    pduId          = HCRP_GET_PDU_ID(instData->payLoad);
    transId        = HCRP_GET_TRANS_ID(instData->payLoad);

    instData->totalCredit = HCRP_GET_CREDIT_GRANT(instData->payLoad);

    /* send response packet. */
    CsrPmemFree(instData->payLoad);

    len = HCRP_PDU_ID_SIZE + HCRP_TRANS_ID_SIZE + HCRP_PARAM_LENGTH_SIZE + HCRP_STATUS_CODE_SIZE;
    instData->payLoad = CsrPmemAlloc(len);

    HCRP_SET_PDU_ID(instData->payLoad, pduId);
    HCRP_SET_TRANS_ID(instData->payLoad, transId);
    HCRP_SET_PARAM_LENGTH(instData->payLoad, HCRP_STATUS_CODE_SIZE);
    HCRP_SET_STATUS_CODE(instData->payLoad, 0x01);    /* status code = success. */
    instData->payLoadLength = len;
    csrBtHcrpTxPacket(instData);
}

static void    csrBtHcrpHandle_CR_DataChannelCreditRequestRequest(hcrpInstanceData_t* instData)
{
    CsrUint16 pduId;
    CsrUint16 transId;
    CsrUint16 len;

    pduId              = HCRP_GET_PDU_ID(instData->payLoad);
    transId            = HCRP_GET_TRANS_ID(instData->payLoad);

    instData->totalCredit -= HCRP_CREDIT_10K;

    /* send response packet. */
    CsrPmemFree(instData->payLoad);

    /*From spec: The server returns an amount of credit that it wishes
    to grant the client. An extremely fast server device with very large buffers might grant
    several megabytes of credit at once. A server may deny a credit request by returning
    zero bytes of credit granted. Returning zero bytes of credit is a successful
    transaction as long as nothing else went wrong.    */
    
    len = HCRP_PDU_ID_SIZE + HCRP_TRANS_ID_SIZE + HCRP_PARAM_LENGTH_SIZE + HCRP_STATUS_CODE_SIZE + 4;
    instData->payLoad = CsrPmemAlloc(len);

    HCRP_SET_PDU_ID(instData->payLoad, pduId);
    HCRP_SET_TRANS_ID(instData->payLoad, transId);
    HCRP_SET_PARAM_LENGTH(instData->payLoad, HCRP_STATUS_CODE_SIZE + 4);
    HCRP_SET_STATUS_CODE(instData->payLoad, 0x01);    /* status code = success. */
    
    if (instData->totalCredit > HCRP_CREDIT_10K)
    {/* there is still sufficient credit available. */
        HCRP_SET_D_WORD_AT(instData->payLoad, 8, HCRP_CREDIT_10K);
    }
    else
    {/* we've run out of pseudo credit, so responsed with zero and wait for new request... */
        HCRP_SET_D_WORD_AT(instData->payLoad, 8, HCRP_CREDIT_0k); /* respond with 0 for temporary no credits */
    }
    instData->payLoadLength = len;
    csrBtHcrpTxPacket(instData);
}

void CsrBtHcrpSecurityInReqHandler(hcrpInstanceData_t* instData)
{
    CsrBtResultCode rval;
    CsrBtHcrpSecurityInReq *prim;

    prim = (CsrBtHcrpSecurityInReq*)instData->pRecvMsg;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_HCRP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HCRP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HCRP_SUCCESS,
        CSR_BT_RESULT_CODE_HCRP_UNACCEPTABLE_PARAMETER);

    CsrBtHcrpSecurityInCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_HCRP);
}

void CsrBtHcrpSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtHcrpSecurityInCfm *prim;
    prim = (CsrBtHcrpSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtHcrpSecurityInCfm));
    prim->type = CSR_BT_HCRP_SECURITY_IN_CFM;
    prim->resultCode = resultCode;
    prim->resultCode = resultSupplier;
    CsrBtHcrpMessagePut(appHandle, prim);
}

