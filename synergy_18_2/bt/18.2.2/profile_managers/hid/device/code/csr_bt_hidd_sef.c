/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_pmem.h"

#include "csr_bt_hidd_sef.h"
#include "csr_bt_hidd_prim.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_hidd_local_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_hidd_local_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hidd_local_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* to determine which service record is being registered */
#define HIDD_SDS_REQUEST_NONE       0
#define HIDD_SDS_REQUEST_HID        1
#define HIDD_SDS_REQUEST_DEVICE     2

/* HID specific service record attributes */
#define HID_PARSER_VERSION          0x0201
#define HID_DEVICE_SUBCLASS         0x0202
#define HID_COUNTRY_CODE            0x0203
#define HID_VIRTUAL_CABLE           0x0204
#define HID_RECONNECT_INITIATE      0x0205
#define HID_DESCRIPTOR_LIST         0x0206
#define HID_LANGID_BASE_LIST        0x0207
#define HID_SDP_DISABLE             0x0208
#define HID_BATTERY_POWER           0x0209
#define HID_REMOTE_WAKE             0x020A
#define HID_PROFILE_VERSION         0x020B
#define HID_SUPERVISION_TIMEOUT     0x020C
#define HID_NORMALLY_CONNECTABLE    0x020D
#define HID_BOOT_DEVICE             0x020E

/* Reason for disconnecting when connected */
#define HIDD_CONNECTED_UNPLUG_IND   1
#define HIDD_CONNECTED_UNPLUG_REQ   2
#define HIDD_CONNECTED_CHANGE_MODE  3

#define HIDD_NUM_OF_RETRIES     50
#define HIDD_NUM_OF_RECONNECTS  60
#define HIDD_ACCEPT_CONNECT_TIMEOUT           60000000 /* 60 seconds */
#define HIDD_CONNECT_ATTEMPT_TIMEOUT            500000 /* 500 milliseconds */
/* local functions */
static void reconnectTimeOut(CsrUint16 not_used, void *data)
{
    HiddInstanceDataType    *instData;
    CSR_UNUSED(not_used);

    instData = data;
    if(instData->reconnect == TRUE)
    {
        if(instData->state == HIDD_ACCEPT_CONNECTING_STATE)
        {
            CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle, CSR_BT_HID_CTRL_PSM);
            CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle, CSR_BT_HID_INTR_PSM);
        }

        instData->timerId = 0;
    }
}


static void csrBtHiddGetHidFlags(HiddInstanceDataType *instData)
{
    CsrUint8 tmpFlag = 0;

    if(CsrBtSdcGetBoolDirect(instData->hidSdp.recordLen, instData->hidSdp.record, HID_VIRTUAL_CABLE, &tmpFlag))
    {
        if(tmpFlag)
        {
            instData->hidFlags |= CSR_BT_HIDD_FLAGS_VIRTUAL_CABLE_BIT;
            tmpFlag = 0;
        }
    }

    if(CsrBtSdcGetBoolDirect(instData->hidSdp.recordLen, instData->hidSdp.record, HID_RECONNECT_INITIATE, &tmpFlag))
    {
        if(tmpFlag)
        {
            instData->hidFlags |= CSR_BT_HIDD_FLAGS_RECONNECT_INIT_BIT;
            tmpFlag = 0;
        }
    }
}

static void csrBtHiddConnect(HiddInstanceDataType *instData, CsrUint8 psm)
{
    /* We don't support QOS yet in L2CAP */
    CsrBtCml2caConnectReqSend(instData->myAppHandle,
                        instData->bdAddr,
                        psm,
                        psm,
                        CSR_BT_HIDD_PROFILE_DEFAULT_MTU_SIZE,
                        instData->secOutgoing);
}

static void csrBtHiddAcceptConnect(HiddInstanceDataType *instData)
{
    if(instData->reconnect)
    {
        instData->timerId = CsrSchedTimerSet( HIDD_ACCEPT_CONNECT_TIMEOUT,(void (*) (CsrUint16, void *)) reconnectTimeOut, 0, (void *) instData);
    }
    if(instData->ctrlCh.qos == NULL)
    {
        CsrBtCml2caConnectAcceptReqSend( instData->myAppHandle,
                                    CSR_BT_HID_CTRL_PSM,
                                    0, /* Class of Device */
                                    instData->secIncoming,
                                    CSR_BT_HIDD_PROFILE_DEFAULT_MTU_SIZE,
                                    CSR_BT_HID_PROFILE_UUID);
    }
    else
    {
        L2CA_QOS_T *qos = (L2CA_QOS_T *)CsrPmemAlloc(sizeof(L2CA_QOS_T));
        CsrMemCpy(qos,instData->ctrlCh.qos,sizeof(L2CA_QOS_T));
        CsrBtCml2caConnectAcceptReqSendExt( instData->myAppHandle,
                                    CSR_BT_HID_CTRL_PSM,
                                    0, /* Class of Device */
                                    instData->secIncoming,
                                    CSR_BT_HIDD_PROFILE_DEFAULT_MTU_SIZE,
                                    instData->flushTimeout, /* recommended L2CA_FLUSH_TO_DEFAULT */
                                    qos,
                                    CSR_BT_HID_PROFILE_UUID);
    }
    if(instData->intrCh.qos == NULL)
    {
        CsrBtCml2caConnectAcceptReqSend( instData->myAppHandle,
                                    CSR_BT_HID_INTR_PSM,
                                    0, /* Class of Device */
                                    instData->secIncoming,
                                    CSR_BT_HIDD_PROFILE_DEFAULT_MTU_SIZE,
                                    CSR_BT_HID_PROFILE_UUID);
    }
    else
    {
        L2CA_QOS_T *qos = (L2CA_QOS_T *)CsrPmemAlloc(sizeof(L2CA_QOS_T));
        CsrMemCpy(qos,instData->intrCh.qos,sizeof(L2CA_QOS_T));
        CsrBtCml2caConnectAcceptReqSendExt( instData->myAppHandle,
                                    CSR_BT_HID_INTR_PSM,
                                    0, /* Class of Device */
                                    instData->secIncoming,
                                    CSR_BT_HIDD_PROFILE_DEFAULT_MTU_SIZE,
                                    instData->flushTimeout, /* recommended L2CA_FLUSH_TO_DEFAULT */
                                    qos,
                                    CSR_BT_HID_PROFILE_UUID);
    }
}

static void connectTimeOut(CsrUint16 psm, void *data)
{
    HiddInstanceDataType    *instData;

    instData = data;

    if(instData->reconnect == TRUE)
    {
        if(instData->state == HIDD_CONNECTING_STATE)
        {
            csrBtHiddConnect(instData, (CsrUint8)psm);
        }
        instData->timerId = 0;
    }

}

static void csrBtHiddRestoreSavedMessages(HiddInstanceDataType * instData)
{
    CsrBtHiddRestoreInd* prim;
    if (instData->saveQueue != NULL)
    {
        prim = CsrPmemAlloc(sizeof(CsrBtHiddRestoreInd));
        prim->type = CSR_BT_HIDD_RESTORE_IND;
        CsrBtHiddMessagePut(instData->myAppHandle, prim);

        instData->restoreHiddFlag = TRUE;
    }
    else if(instData->saveCmQueue != NULL)
    {
        prim = CsrPmemAlloc(sizeof(CsrBtHiddRestoreInd));
        prim->type = CSR_BT_HIDD_RESTORE_IND;
        CsrBtHiddMessagePut(instData->myAppHandle, prim);

        instData->restoreCmFlag = TRUE;
    }
}

/* HIDD handler functions */
void CsrBtHiddActivateIdleHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddActivateReq* prim;
    BD_ADDR_T zeroBdAddr;

    prim = (CsrBtHiddActivateReq *) instData->pRecvMsg;
    bd_addr_zero(&zeroBdAddr);

    if ((instData->hidSdp.record && prim->hidSdp) ||
        (instData->deviceIdSdp.record && prim->deviceIdSdp))
    {
        /* if a service record is provided in the prim and a service record is already registered */
        instData->registeringNewRec = TRUE;

        if (prim->hidSdp)
        {
            CsrPmemFree(instData->hidSdp.record);
            instData->hidSdp.record = (CsrUint8 *) CsrPmemAlloc(prim->hidSdpLen);
            CsrMemCpy(instData->hidSdp.record, prim->hidSdp, prim->hidSdpLen);
            instData->hidSdp.recordLen = prim->hidSdpLen;
        }
        if (prim->deviceIdSdp)
        {
            CsrPmemFree(instData->deviceIdSdp.record);
            instData->deviceIdSdp.record = (CsrUint8 *) CsrPmemAlloc(prim->deviceIdSdpLen);
            CsrMemCpy(instData->deviceIdSdp.record, prim->deviceIdSdp, prim->deviceIdSdpLen);
            instData->deviceIdSdp.recordLen = prim->deviceIdSdpLen;
        }
        /* unregister old records (both records will be registered again before sending CSR_BT_HIDD_ACTIVATE_CFM) */
        instData->numOfRetries = 0;
        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
        CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
        CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->deviceIdSdp.recordHandle);
        instData->newRecUnregCfmRec = FALSE;
        instData->hidSdp.recordHandle = HIDD_SDS_REQUEST_HID;
        instData->deviceIdSdp.recordHandle = HIDD_SDS_REQUEST_DEVICE;
    }
    else
    {
        if(instData->active && (instData->disconnectReason == 0))
        {
            /* if already active and not unplugged */
            instData->reconnect = TRUE;
            if(instData->appHandle == prim->appHandle)
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                if(!bd_addr_eq(&(instData->bdAddr),&(prim->deviceAddr)))
                {
                    CsrGeneralException("HIDD",
                         (CsrUint16) instData->state,
                         prim->type,
                         "Wrong bdAddr handle");
                }
#endif
                CsrPmemFree(prim->deviceIdSdp);
                CsrPmemFree(prim->hidSdp);
                CsrPmemFree(prim->qosCtrl);
                CsrPmemFree(prim->qosIntr);
            }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            else
            {
            CsrGeneralException("HIDD",
                     (CsrUint16) instData->state,
                     prim->type,
                     "Wrong app handle");
            }
#endif
        }
        else
        {
            instData->active = TRUE;
            if( !bd_addr_eq(&(instData->bdAddr), &(prim->deviceAddr)) )
            {
                bd_addr_copy(&(instData->bdAddr), &(prim->deviceAddr));
            }

            instData->appHandle = prim->appHandle;
            instData->deviceIdSdp.recordLen = prim->deviceIdSdpLen;
            instData->deviceIdSdp.record = prim->deviceIdSdp;
            instData->hidSdp.recordLen = prim->hidSdpLen;
            instData->hidSdp.record = prim->hidSdp;
            if((prim->qosCtrl != NULL) && (prim->qosCtrlCount == 1))
            {
                instData->ctrlCh.qos = prim->qosCtrl;
            }
            if((prim->qosIntr) && (prim->qosIntrCount == 1))
            {
                instData->intrCh.qos = prim->qosIntr;
            }
            instData->flushTimeout = prim->flushTimeout;
            instData->disconnectReason = 0;

            csrBtHiddGetHidFlags(instData);
        }

        instData->deactivating = FALSE;

        /* Bluetooth address present or not */
        if( bd_addr_eq(&zeroBdAddr, &(prim->deviceAddr)) )
        {
            /* no address - register sdp */
            CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_REGISTERING_SDP);
            instData->hidSdp.recordHandle = HIDD_SDS_REQUEST_HID;
            if(instData->hidSdp.record)
            {
                CsrUint8        *record;
                record = (CsrUint8 *) CsrPmemAlloc(instData->hidSdp.recordLen);
                CsrMemCpy(record, instData->hidSdp.record, instData->hidSdp.recordLen);

                CsrBtCmSdsRegisterReqSend(instData->myAppHandle, record, instData->hidSdp.recordLen);
            }
            else
            {
                CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED, CSR_BT_SUPPLIER_HIDD);
            }
        }
        else
        {
            if(instData->hidFlags & CSR_BT_HIDD_FLAGS_RECONNECT_INIT_BIT)
            {
                /* connect request */
                instData->reconnect = TRUE;
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTING_STATE);
                CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                csrBtHiddConnect(instData, CSR_BT_HID_CTRL_PSM);
            }
            else
            {
                /* register sdp */
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_REGISTERING_SDP);
                instData->hidSdp.recordHandle = HIDD_SDS_REQUEST_HID;
                if(instData->hidSdp.record)
                {
                    CsrUint8        *record;
                    record = (CsrUint8 *) CsrPmemAlloc(instData->hidSdp.recordLen);
                    CsrMemCpy(record, instData->hidSdp.record, instData->hidSdp.recordLen);

                    CsrBtCmSdsRegisterReqSend(instData->myAppHandle, record, instData->hidSdp.recordLen);
                }
                else
                {
                    CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED, CSR_BT_SUPPLIER_HIDD);
                }
            }
        }
    }
}

void CsrBtHiddReactivateIdleHandler(HiddInstanceDataType *instData)
{
    if(instData->hidFlags & CSR_BT_HIDD_FLAGS_RECONNECT_INIT_BIT)
    {
        /* connect request */
        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTING_STATE);
        instData->numOfRetries = 0;
        csrBtHiddConnect(instData, CSR_BT_HID_CTRL_PSM);
    }
    else
    {

        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_ACCEPT_CONNECTING_STATE);
        csrBtHiddAcceptConnect(instData);
    }
}

void CsrBtHiddDeactivateHandler(HiddInstanceDataType *instData)
{
    instData->deactivating = TRUE;
    instData->active = FALSE;

    if(instData->reconnect)
    {
        instData->reconnect = FALSE;
    }
    if(instData->ctrlCh.cid)
    {
        /* Already connecting - await connect cfm to be able to disconnect */
        if(HIDD_CONNECTED_STATE == instData->state)
        {
            if(instData->intrCh.cid)
            {
                CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
            }
        }
    }
    else
    {
        switch(instData->state)
        {
        case HIDD_ACCEPT_CONNECTING_STATE:
            {
                /* Cancel accept connect */
                CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle, CSR_BT_HID_CTRL_PSM);
                CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle, CSR_BT_HID_INTR_PSM);
            }
            break;
        case HIDD_CONNECTING_STATE:
            {
                /* Cancel connect request */
                CsrBtCml2caCancelConnectReqSend(instData->myAppHandle,instData->bdAddr, CSR_BT_HID_CTRL_PSM);
            }
            break;
        case HIDD_IDLE_STATE:
            /* Fall through */
        case HIDD_NOT_CONNECTED_STATE:
            {
                instData->numOfRetries = 0;
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                instData->prevError = CSR_BT_RESULT_CODE_HIDD_CONNECTION_TERM_BY_LOCAL_HOST;
                instData->prevErrorSupplier = CSR_BT_SUPPLIER_HIDD;
                CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
                instData->deactUnregCfmRec = FALSE;
            }
            break;
        default:
            /* REGISTER and UNREGISTER */
            break;
        }
    }
}

void CsrBtHiddControlRespConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddControlRes *prim;

    prim = (CsrBtHiddControlRes *) instData->pRecvMsg;

    if(!(prim->data))
    {
        prim->data = CsrPmemAlloc(1);
        prim->dataLen = 1;
    }
    prim->data[0] = prim->transactionType;
    prim->data[0] |= prim->parameter;


    if(prim->dataLen >= instData->ctrlCh.hostMtu)
    {
        CsrUint8* tmp;

        instData->ctrlCh.sendMsg = prim->data;
        instData->ctrlCh.sendMsgLength = prim->dataLen;

        instData->ctrlCh.sendMsgOffset = instData->intrCh.hostMtu;
        tmp = CsrPmemAlloc(instData->ctrlCh.sendMsgOffset);

        CsrMemCpy(tmp, prim->data, instData->ctrlCh.sendMsgOffset);

        CsrBtCml2caDataReqSend(instData->ctrlCh.cid, instData->ctrlCh.sendMsgOffset, tmp);
    }
    else
    {
        CsrBtCml2caDataReqSend(instData->ctrlCh.cid, prim->dataLen, prim->data);
    }
}

void CsrBtHiddDataConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddDataReq *prim;

    prim = (CsrBtHiddDataReq *) instData->pRecvMsg;

    prim->report[0] = CSR_BT_HIDD_DATA;
    prim->report[0] |= CSR_BT_HIDD_INPUT_REPORT;

    if(prim->reportLen >= instData->intrCh.hostMtu)
    {
        CsrUint8* tmp;

        instData->intrCh.sendMsg = prim->report;
        instData->intrCh.sendMsgLength = prim->reportLen;

        instData->intrCh.sendMsgOffset = instData->intrCh.hostMtu;
        tmp = CsrPmemAlloc(instData->intrCh.sendMsgOffset);

        CsrMemCpy(tmp, prim->report, instData->intrCh.sendMsgOffset);

        CsrBtCml2caDataReqSend(instData->intrCh.cid, instData->intrCh.sendMsgOffset, tmp);
    }
    else
    {
        CsrBtCml2caDataReqSend(instData->intrCh.cid, prim->reportLen, prim->report);
    }
}

void CsrBtHiddUnplugConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddUnplugReq *prim;

    prim = (CsrBtHiddUnplugReq *) instData->pRecvMsg;

    if(bd_addr_eq(&(instData->bdAddr), &(prim->deviceAddr)))
    {
        /* Unplug request */
        CsrUint8* tmpData = CsrPmemAlloc(1);
        tmpData[0] = CSR_BT_HIDD_CONTROL;
        tmpData[0] |= CSR_BT_HIDD_VC_UNPLUG;
        instData->disconnectReason = HIDD_CONNECTED_UNPLUG_REQ;
        CsrBtCml2caDataReqSend(instData->ctrlCh.cid, 1, tmpData);
    }
    else
    {
        /* Unknown device */
    }
}

void CsrBtHiddChangeModeConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddModeChangeReq *prim;

    prim = (CsrBtHiddModeChangeReq *)instData->pRecvMsg;

    if(instData->currentMode != prim->mode)
    {
        instData->nextMode = prim->mode;
        switch(prim->mode)
        {
        case CSR_BT_HIDD_ACTIVE_MODE:
            /* FALL THROUGH */
        case CSR_BT_HIDD_SNIFF_MODE:
            /* Change mode request */
            instData->ctrlCh.modeChangeSuccess = FALSE;
            instData->intrCh.modeChangeSuccess = FALSE;
            CsrBtCml2caModeChangeReqSend(instData->ctrlCh.cid, (CsrUint8) prim->mode);
            break;
        case CSR_BT_HIDD_DISCONNECT_MODE:
            /* Disconnect connection */
            instData->disconnectReason = HIDD_CONNECTED_CHANGE_MODE;
            CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
            break;
        default:
            /* Unknown mode */
            break;
        }
    }
    else
    {
        /* nothing to change */
        CsrBtHiddModeChangeIndSend(instData, instData->currentMode, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
    }
}

void CsrBtHiddChangeModeNotConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtHiddModeChangeReq *prim;

    prim = (CsrBtHiddModeChangeReq *)instData->pRecvMsg;

    if(instData->currentMode != prim->mode)
    {
        instData->nextMode = prim->mode;
        switch(prim->mode)
        {
        case CSR_BT_HIDD_ACTIVE_MODE:
            /* FALL THROUGH */
        case CSR_BT_HIDD_SNIFF_MODE:
            {
                /* connect request */
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTING_STATE);
                instData->numOfRetries = 0;
                instData->reconnect = TRUE;
                csrBtHiddConnect(instData, CSR_BT_HID_CTRL_PSM);
            }
            break;
        default:
            /* Unknown mode */
            break;
        }
    }
    else
    {
        /* nothing to change */
        CsrBtHiddModeChangeIndSend(instData,instData->currentMode, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
    }
}

/* CM handler functions */
void CsrBtHiddCmRegisterInitHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caRegisterCfm *prim;
    prim = (CsrBtCmL2caRegisterCfm *) instData->pRecvMsg;

    if( prim->localPsm == CSR_BT_HID_CTRL_PSM )
    {
        CsrBtCml2caRegisterReqSend(instData->myAppHandle, CSR_BT_HID_INTR_PSM);
    }
    else if( prim->localPsm == CSR_BT_HID_INTR_PSM )
    {
        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
        csrBtHiddRestoreSavedMessages(instData);
    }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    else
    {
    CsrGeneralException("HIDD",
                 (CsrUint16) instData->state,
                 prim->type,
                 "Unknown PSM");
    }
#endif
}

void CsrBtHiddCmRegisterSdpRegisteringHandler(HiddInstanceDataType *instData)
{
    CsrBtCmSdsRegisterCfm *prim;
    prim = (CsrBtCmSdsRegisterCfm *) instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        instData->numOfRetries = 0;
        if(instData->hidSdp.recordHandle == HIDD_SDS_REQUEST_HID)
        {
             /* Hid service record OK */
            instData->hidSdp.recordHandle = prim->serviceRecHandle;

            if(instData->deactivating)
            {
                /* Hid service record already succeeded therefore need to be unregistered */
                instData->numOfRetries = 0;
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                instData->prevError = CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED;
                instData->prevErrorSupplier = CSR_BT_SUPPLIER_HIDD;
                CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
            }
            else
            {
                if(instData->deviceIdSdp.record)
                {
                    CsrUint8 *record;
                    record = (CsrUint8 *) CsrPmemAlloc(instData->deviceIdSdp.recordLen);
                    CsrMemCpy(record, instData->deviceIdSdp.record, instData->deviceIdSdp.recordLen);

                    CsrBtCmSdsRegisterReqSend(instData->myAppHandle, record, instData->deviceIdSdp.recordLen);
                }
                else
                {
                    /* Accept connect request */
                    if(!instData->reconnect)
                    {
                        CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                    }
                    CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_ACCEPT_CONNECTING_STATE);
                    csrBtHiddAcceptConnect(instData);
                }
            }
        }
        else
        {
            /* Device Id service record OK */
            instData->deviceIdSdp.recordHandle = prim->serviceRecHandle;

            if(instData->deactivating)
            {
                /* Hid service record already succeeded therefore need to be unregistered */
                instData->numOfRetries = 0;
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                instData->prevError = CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED;
                instData->prevErrorSupplier = CSR_BT_SUPPLIER_HIDD;
                CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
            }
            else
            {
                /* Accept connect request */
                if(!instData->reconnect)
                {
                    CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                }
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_ACCEPT_CONNECTING_STATE);
                csrBtHiddAcceptConnect(instData);
            }
        }
    }
    else
    {
        if(instData->deactivating)
        {
            instData->numOfRetries = 0;
            if(instData->hidSdp.recordHandle == HIDD_SDS_REQUEST_HID)
            {
                CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                   CsrBtHiddCleanUpToIdle(instData);
            }
            else
            {
                /* Hid service record already succeeded therefore need to be unregistered */
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                instData->prevError = prim->resultCode;
                instData->prevErrorSupplier = prim->resultSupplier;
                CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
            }
        }
        else
        {
            if(instData->numOfRetries < HIDD_NUM_OF_RETRIES)
            {
                CsrUint8        *record;

                /* Attempt failed. Try again to send a register */
                if(HIDD_SDS_REQUEST_HID == instData->hidSdp.recordHandle)
                {
                    record = (CsrUint8 *) CsrPmemAlloc(instData->hidSdp.recordLen);
                    CsrMemCpy(record, instData->hidSdp.record, instData->hidSdp.recordLen);

                    CsrBtCmSdsRegisterReqSend(instData->myAppHandle, record, instData->hidSdp.recordLen);
                }
                else
                {
                    record = (CsrUint8 *) CsrPmemAlloc(instData->deviceIdSdp.recordLen);
                    CsrMemCpy(record, instData->deviceIdSdp.record, instData->deviceIdSdp.recordLen);

                    CsrBtCmSdsRegisterReqSend(instData->myAppHandle, record, instData->deviceIdSdp.recordLen);
                }
                instData->numOfRetries++;
            }
            else
            {
                /* Number of retries failed. Stop trying */
                if(instData->hidSdp.recordHandle == HIDD_SDS_REQUEST_HID)
                {
                    /*  Respond, cleanup and return to idle */
                    if(!instData->reconnect)
                    {
                        CsrBtHiddActivateCfmSend(instData,
                                                 CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED,
                                                 CSR_BT_SUPPLIER_HIDD);
                    }
                    else
                    {
                        CsrBtHiddStatusIndSend(instData,
                                               CSR_BT_HIDD_CONNECT_FAILED,
                                               CSR_BT_CONN_ID_INVALID);
                    }
                    CsrBtHiddCleanUpToIdle(instData);
                }
                else
                {
                    /* Hid service record already succeeded therefore need to be unregistered */
                    instData->numOfRetries = 0;
                    CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
                }
            }
        }
    }
}

void CsrBtHiddCmUnregisterSdpRegisteringHandler(HiddInstanceDataType *instData)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *) instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if (instData->registeringNewRec)
        {
            /* Old service record is unregistered successfully. Now register the new service record */
            if(!instData->newRecUnregCfmRec)
            {
                CsrBtCmSdsRegisterReqSend(instData->myAppHandle, instData->hidSdp.record, instData->hidSdp.recordLen);
                instData->newRecUnregCfmRec = TRUE;
            }
            else
            {
                instData->newRecUnregCfmRec = FALSE;
            }
        }
        else if (instData->deactivating)
        {
            if(!instData->deactUnregCfmRec)
            {
                if (instData->deviceIdSdp.recordHandle != HIDD_SDS_REQUEST_DEVICE)
                {
                    CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->deviceIdSdp.recordHandle);
                    instData->deactUnregCfmRec = TRUE;
                }
                else
                {
                    CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                    CsrBtHiddCleanUpToIdle(instData);
                }
            }
            else
            {
                instData->deactUnregCfmRec = FALSE;
                CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                CsrBtHiddCleanUpToIdle(instData);
            }
        }
        else
        {
            if(!instData->reconnect)
            {
                CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SDS_REGISTER_FAILED, CSR_BT_SUPPLIER_HIDD);
            }
            else
            {
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_CONNECT_FAILED,
                                       CSR_BT_CONN_ID_INVALID);
            }
            CsrBtHiddCleanUpToIdle(instData);
        }
    }
    else
    {
        if(instData->numOfRetries < HIDD_NUM_OF_RETRIES)
        {
            /* Attempt failed. Try again to send a register */
            CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
            instData->numOfRetries++;
        }
        else
        {
            /* Number of retries failed. Stop trying */
            /* just cleanup and return to idle */
            if(!instData->reconnect)
            {
                CsrBtHiddActivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SDS_UNREGISTER_FAILED, CSR_BT_SUPPLIER_HIDD);
            }
            else
            {
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_UNREGISTER_FAILED,
                                       CSR_BT_CONN_ID_INVALID);
            }
            CsrBtHiddCleanUpToIdle(instData);
        }
    }
}


void CsrBtHiddCmConnectAcceptConnectingHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caConnectAcceptCfm *prim;
    prim = (CsrBtCmL2caConnectAcceptCfm *)instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr, prim->btConnId);

        if(!bd_addr_eq(&(instData->bdAddr), &(prim->deviceAddr)))
        {
            bd_addr_copy(&(instData->bdAddr), &(prim->deviceAddr));
        }
        if(CSR_BT_HID_CTRL_PSM == prim->localPsm)
        {
            instData->ctrlCh.cid = prim->btConnId;
            instData->ctrlCh.hostMtu = prim->mtu;
            instData->ctrlCh.localMtu = prim->localMtu;
            /* Wait for interrupt channel */
        }
        else
        {
            if(instData->deactivating || instData->cleanUp)
            {
                CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
            }
            else
            {
                instData->intrCh.cid = prim->btConnId;
                instData->intrCh.hostMtu = prim->mtu;
                instData->intrCh.localMtu = prim->localMtu;
                if(instData->ctrlCh.cid)
                {
                    CsrBtHiddStatusIndSend(instData,
                                           CSR_BT_HIDD_CONNECTED,
                                           (CsrBtConnId)instData->ctrlCh.cid);
                    CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
                    csrBtHiddRestoreSavedMessages(instData);
                }
                if(instData->reconnect)
                {
                    CsrSchedTimerCancel(instData->timerId, NULL, NULL);
                    instData->reconnect = FALSE;
                }
            }
        }
    }
    else
    {
        if(!instData->deactivating)
        {
            instData->prevError = prim->resultCode;
            instData->prevErrorSupplier = prim->resultSupplier;
            if(instData->reconnect)
            {
                CsrSchedTimerCancel(instData->timerId, NULL, NULL);
                instData->reconnect = FALSE;
            }
        }
        if(instData->ctrlCh.cid)/* if control channel is already connected disconnect */
        {
            CsrBtCml2caDisconnectReqSend(instData->ctrlCh.cid);
            if(instData->reconnect)
            {
                CsrSchedTimerCancel(instData->timerId, NULL, NULL);
                instData->reconnect = FALSE;
            }
        }
        else
        {
            /* do nothing */
        }
    }
}

void CsrBtHiddCmCancelConnectAcceptConnectingHandler(HiddInstanceDataType *instData)
{
    instData->cancelChCount++;

    if(2 == instData->cancelChCount)
    {
        if(!instData->cleanUp)
        {
            if(instData->deactivating)
            {
                CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
            }
            else if (instData->reconnect)
            {
                /* Connection established */
                instData->prevError = CSR_BT_RESULT_CODE_HIDD_TIMEOUT;
                instData->prevErrorSupplier = CSR_BT_SUPPLIER_HIDD;
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_CONNECTED,
                                       (CsrBtConnId)instData->ctrlCh.cid);
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
                csrBtHiddRestoreSavedMessages(instData);
            }
        }
        CsrBtHiddCleanUpToIdle(instData);
    }
}

void CsrBtHiddCmConnectConnectingHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caConnectCfm *prim;
    prim = (CsrBtCmL2caConnectCfm *)instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr, prim->btConnId);

        if(CSR_BT_HID_CTRL_PSM == prim->localPsm)
        {
            instData->ctrlCh.cid = prim->btConnId;
            instData->ctrlCh.hostMtu = prim->mtu;
            instData->ctrlCh.localMtu = prim->localMtu;
            if(instData->deactivating || instData->cleanUp)
            {
                CsrBtCml2caDisconnectReqSend(instData->ctrlCh.cid);
            }
            else
            {
                instData->numOfRetries = 0;
                csrBtHiddConnect(instData, CSR_BT_HID_INTR_PSM);
            }
        }
        else if(CSR_BT_HID_INTR_PSM == prim->localPsm)
        {
            if(instData->deactivating || instData->cleanUp)
            {
                if(instData->ctrlCh.cid)
                {
                    CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
                }
            }
            else
            {
                instData->intrCh.cid = prim->btConnId;
                instData->intrCh.hostMtu = prim->mtu;
                instData->intrCh.localMtu = prim->localMtu;
                if(instData->ctrlCh.cid)
                {
                    instData->reconnect = FALSE;
                    if(instData->currentMode == CSR_BT_HIDD_DISCONNECT_MODE)
                    {
                        instData->ctrlCh.modeChangeSuccess = FALSE;
                        instData->intrCh.modeChangeSuccess = FALSE;
                        CsrBtCml2caModeChangeReqSend(instData->ctrlCh.cid, (CsrUint8)instData->nextMode);
                    }
                    else
                    {
                        instData->numOfRetries = 0;
                        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
                        CsrBtHiddStatusIndSend(instData,
                                               CSR_BT_HIDD_CONNECTED,
                                               (CsrBtConnId)instData->ctrlCh.cid);
                    }
                }
                else
                {
                    /* Control channel not connected, it should have been!!! */
                }
            }
        }
        else
        {
            /* Unknown channel */
        }

    }
    else
    {
        /* if cancel connect request as success */
        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_CANCELLED && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            if(instData->deactivating && (CSR_BT_HID_CTRL_PSM == prim->localPsm))
            {
                CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                CsrBtHiddCleanUpToIdle(instData);
            }
        }
        else
        {
            if(instData->reconnect && (instData->numOfRetries < HIDD_NUM_OF_RECONNECTS) )
            {
                if(CSR_BT_HID_CTRL_PSM == prim->localPsm)
                {
                    if(instData->deactivating || instData->cleanUp)
                    {
                        CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                        CsrBtHiddCleanUpToIdle(instData);
                    }
                    else
                    {
                        instData->timerId = CsrSchedTimerSet( HIDD_CONNECT_ATTEMPT_TIMEOUT,(void (*) (CsrUint16, void *)) connectTimeOut, CSR_BT_HID_CTRL_PSM, (void *) instData);
                    }
                    instData->numOfRetries++;
                }
                else if((CSR_BT_HID_INTR_PSM == prim->localPsm) && (instData->ctrlCh.cid != 0)) 
                    /* If we do not have a ctrl channel, just ignore this cfm, since it will be from before the reconnect was initiated. */
                {
                    if(instData->deactivating || instData->cleanUp)
                    {
                        CsrBtCml2caDisconnectReqSend(instData->ctrlCh.cid);
                    }
                    else
                    {
                        instData->timerId = CsrSchedTimerSet( HIDD_CONNECT_ATTEMPT_TIMEOUT,(void (*) (CsrUint16, void *)) connectTimeOut, CSR_BT_HID_INTR_PSM, (void *) instData);
                    }
                    instData->numOfRetries++;
                }
            }
            else /* !reconnect */
            {
                if(instData->ctrlCh.cid)
                {
                    instData->prevError = prim->resultCode;
                    instData->prevErrorSupplier = prim->resultSupplier;
                    CsrBtCml2caDisconnectReqSend(instData->ctrlCh.cid);
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else if(instData->intrCh.cid)
                {
                    CsrGeneralException("HIDD",
                             (CsrUint16) instData->state,
                             prim->type,
                             "Only interrupt channel connected");
                }
#endif
                else
                {
                    if(!(instData->cleanUp))
                    {
                        if(instData->currentMode == CSR_BT_HIDD_DISCONNECT_MODE)
                        {
                            CsrBtHiddModeChangeIndSend(instData, instData->currentMode, prim->resultCode, prim->resultSupplier);
                            CsrBtHiddCleanUpToIdle(instData);
                        }
                        else
                        {
                            CsrBtHiddStatusIndSend(instData,
                                                   CSR_BT_HIDD_CONNECT_FAILED,
                                                   CSR_BT_CONN_ID_INVALID);
                            CsrBtHiddCleanUpToIdle(instData);
                        }

                    }
                }
            }
        }
    }
}

void CsrBtHiddCmUnregisterSdpUnregisteringHandler(HiddInstanceDataType *instData)
{
    CsrBtCmSdsUnregisterCfm *prim = (CsrBtCmSdsUnregisterCfm *) instData->pRecvMsg;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if(instData->hidSdp.recordHandle == prim->serviceRecHandle)
        {
            instData->hidSdp.recordHandle = 0;
            if(instData->deviceIdSdp.recordHandle)
            {
                instData->numOfRetries = 0;
                CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->deviceIdSdp.recordHandle);
            }
            else if(!(instData->deviceIdSdp.record))
            {
                if(instData->deactivating || instData->cleanUp)
                {
                    if(instData->prevError)
                    {
                        if(instData->deactivating)
                        {
                            CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                        }
                           CsrBtHiddCleanUpToIdle(instData);
                    }
                    else if(instData->intrCh.cid)
                    {
                        CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
                    }
                }
                else if(instData->reconnect && instData->prevError)
                {
                    CsrBtHiddStatusIndSend(instData,
                                           CSR_BT_HIDD_CONNECT_FAILED,
                                           CSR_BT_CONN_ID_INVALID);
                       CsrBtHiddCleanUpToIdle(instData);
                }
                else
                {
                    /* proceed to connected */
                    CsrBtHiddStatusIndSend(instData,
                                           CSR_BT_HIDD_CONNECTED,
                                           (CsrBtConnId)instData->ctrlCh.cid);
                    CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
                    csrBtHiddRestoreSavedMessages(instData);
                }
            }
            else
            {
                /* Abnormal flow */
            }
        }
        else
        {
            instData->deviceIdSdp.recordHandle = 0;
            if(instData->deactivating || instData->cleanUp)
            {
                if(instData->prevError)
                {
                    if(!(instData->cleanUp))
                    {
                        CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                    }
                    CsrBtHiddCleanUpToIdle(instData);
                }
                else if(instData->intrCh.cid)
                {
                    CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
                }
                else
                {
                    /* Abnormal flow */
                }
            }
            else
            {
                if(instData->prevError) /* not success */
                {
                    /* cleanup and return to idle */
                    CsrBtHiddStatusIndSend(instData,
                                           CSR_BT_HIDD_CONNECT_FAILED,
                                           CSR_BT_CONN_ID_INVALID);
                    CsrBtHiddCleanUpToIdle(instData);
                }
                else
                {
                    /* proceed to connected */
                    CsrBtHiddStatusIndSend(instData,
                                           CSR_BT_HIDD_CONNECTED,
                                           (CsrBtConnId)instData->ctrlCh.cid);
                    CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
                    csrBtHiddRestoreSavedMessages(instData);
                }
            }
        }

    }
    else
    {
        if(instData->numOfRetries < HIDD_NUM_OF_RETRIES)
        {
            /* Attempt failed. Try again to send a register */
            CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
            instData->numOfRetries++;
        }
        else
        {
            /* Max number of retries failed.
               Send status and disconnect connection */
            instData->prevError = prim->resultCode;
            instData->prevErrorSupplier = prim->resultSupplier;
            if(!instData->deactivating)
            {
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_UNREGISTER_FAILED,
                                       CSR_BT_CONN_ID_INVALID);
            }
            CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
        }

    }
}

void CsrBtHiddCmDataCfmConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caDataCfm *prim;

    prim = (CsrBtCmL2caDataCfm *)instData->pRecvMsg;

    if(instData->intrCh.cid == prim->btConnId)
    {
        if(instData->intrCh.sendMsg)
        {
            CsrUint8* tmp;

            if((instData->intrCh.sendMsgLength - instData->intrCh.sendMsgOffset) >= (instData->intrCh.hostMtu-1))
            {
                tmp = CsrPmemAlloc(instData->intrCh.hostMtu);

                tmp[0] = CSR_BT_HIDD_DATC;
                tmp[0] |= CSR_BT_HIDD_INPUT_REPORT;

                CsrMemCpy(&(tmp[1]), &(instData->intrCh.sendMsg[instData->intrCh.sendMsgOffset]), instData->intrCh.hostMtu-1);

                instData->intrCh.sendMsgOffset += (instData->intrCh.hostMtu - 1);
                CsrBtCml2caDataReqSend(instData->intrCh.cid, instData->intrCh.hostMtu, tmp);

            }
            else
            {
                CsrUint16 size = instData->intrCh.sendMsgLength - instData->intrCh.sendMsgOffset;
                tmp = CsrPmemAlloc(size + 1);

                tmp[0] = CSR_BT_HIDD_DATC;
                tmp[0] |= CSR_BT_HIDD_INPUT_REPORT;

                if(size)
                {
                    CsrMemCpy(&(tmp[1]), &(instData->intrCh.sendMsg[instData->intrCh.sendMsgOffset]), size);
                }

                CsrBtCml2caDataReqSend(instData->intrCh.cid, (CsrUint16)(size+1), tmp);
                CsrPmemFree(instData->intrCh.sendMsg);
                instData->intrCh.sendMsg = NULL;
            }
        }
        else
        {
            if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                CsrBtHiddDataCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
            }
            else
            {
                CsrBtHiddDataCfmSend(instData, prim->resultCode, prim->resultSupplier);
            }
        }
    }
    else
    {
        /* Control channel confirm on control response */
        if(instData->ctrlCh.sendMsg)
        {
            CsrUint8* tmp;

            if((instData->ctrlCh.sendMsgLength - instData->ctrlCh.sendMsgOffset) >= (instData->ctrlCh.hostMtu-1))
            {
                tmp = CsrPmemAlloc(instData->ctrlCh.hostMtu);

                tmp[0] = CSR_BT_HIDD_DATC;
                tmp[0] |= (instData->ctrlCh.sendMsg[0] & 0x0F);

                CsrMemCpy(&(tmp[1]), &(instData->ctrlCh.sendMsg[instData->ctrlCh.sendMsgOffset]), instData->ctrlCh.hostMtu-1);

                instData->ctrlCh.sendMsgOffset += (instData->ctrlCh.hostMtu-1);
                CsrBtCml2caDataReqSend(instData->ctrlCh.cid, instData->ctrlCh.hostMtu, tmp);

            }
            else
            {
                CsrUint16 size = instData->ctrlCh.sendMsgLength - instData->ctrlCh.sendMsgOffset;
                tmp = CsrPmemAlloc(size + 1);

                tmp[0] = CSR_BT_HIDD_DATC;
                tmp[0] |= (instData->ctrlCh.sendMsg[0] & 0x0F);;

                if(size)
                {
                    CsrMemCpy(&(tmp[1]), &(instData->ctrlCh.sendMsg[instData->ctrlCh.sendMsgOffset]), size);
                }

                CsrBtCml2caDataReqSend(instData->ctrlCh.cid, (CsrUint16)(size+1), tmp);
                CsrPmemFree(instData->ctrlCh.sendMsg);
                instData->ctrlCh.sendMsg = NULL;
            }
        }
        else
        {
            /* No action */
        }
    }
}

void CsrBtHiddCmDataIndConnectedHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caDataInd *prim;
    HiddChInfo *pCurrentCh = NULL;

    prim = (CsrBtCmL2caDataInd *)instData->pRecvMsg;

    CsrBtCmL2caDataResSend(prim->btConnId);

    if(prim->btConnId == instData->ctrlCh.cid)
    {
        pCurrentCh = &(instData->ctrlCh);
    }
    else /* if(prim->btConnId == instData->intrCh.cid) */
    {
        pCurrentCh = &(instData->intrCh);
    }

    if(pCurrentCh->recvMsg && pCurrentCh->cid)
    {
        if( prim->length == 1)
        {
            CsrPmemFree(prim->payload);
            if(pCurrentCh->cid == instData->ctrlCh.cid)
            {
                CsrBtHiddControlIndSend(instData, (CsrUint8)(pCurrentCh->recvMsg[0] & 0xF0), (CsrUint8)(pCurrentCh->recvMsg[0] & 0x0F),
                                    pCurrentCh->recvMsgLength, pCurrentCh->recvMsg);
            }
            else
            {
                CsrBtHiddDataIndSend(instData, (CsrUint8)(pCurrentCh->recvMsg[0] & 0x0F), pCurrentCh->recvMsgLength, pCurrentCh->recvMsg);
            }
            pCurrentCh->recvMsg = NULL;
        }
        else if( prim->length <= pCurrentCh->localMtu)
        {
            CsrUint8* tmp;
            CsrUint16 size;

            size = pCurrentCh->recvMsgLength + prim->length - 1;
            tmp = CsrPmemAlloc(size);

            CsrMemCpy(tmp, pCurrentCh->recvMsg, pCurrentCh->recvMsgLength);
            CsrMemCpy(&(tmp[pCurrentCh->recvMsgLength]), prim->payload + 1, prim->length - 1);

            CsrPmemFree(pCurrentCh->recvMsg);
            pCurrentCh->recvMsg = tmp;
            pCurrentCh->recvMsgLength = size;

            CsrPmemFree(prim->payload);
            if( prim->length < pCurrentCh->localMtu)
            {
                if(pCurrentCh->cid ==  instData->ctrlCh.cid)
                {
                    if(pCurrentCh->reportType)
                    {
                        CsrBtHiddControlIndSend(instData, (CsrUint8)CSR_BT_HIDD_SET_REPORT, pCurrentCh->reportType, pCurrentCh->recvMsgLength, pCurrentCh->recvMsg);
                    }
                    else
                    {
                        /* no such data */
                        CsrBtHiddControlIndSend(instData, (CsrUint8)(pCurrentCh->recvMsg[0] & 0xF0), (CsrUint8)(pCurrentCh->recvMsg[0] & 0x0F),
                                        pCurrentCh->recvMsgLength, pCurrentCh->recvMsg);
                    }
                }
                else
                {
                    CsrBtHiddDataIndSend(instData, (CsrUint8)(pCurrentCh->recvMsg[0] & 0x0F), pCurrentCh->recvMsgLength, pCurrentCh->recvMsg);
                }
                pCurrentCh->recvMsg = NULL;
            }
        }
        else
        {
            /* wrong payload size */
            CsrPmemFree(prim->payload);
        }
    }
    else if(pCurrentCh->cid)
    {
        if( prim->length < pCurrentCh->localMtu )
        {
            if(pCurrentCh->cid ==  instData->ctrlCh.cid)
            {
                if( ((prim->payload[0]) & (CSR_BT_HIDD_CONTROL + CSR_BT_HIDD_VC_UNPLUG)) == (CSR_BT_HIDD_CONTROL + CSR_BT_HIDD_VC_UNPLUG) )
                {
                    instData->disconnectReason = HIDD_CONNECTED_UNPLUG_IND;
                    CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
                    CsrPmemFree(prim->payload);
                }
                else if( ((prim->payload[0]) & CSR_BT_HIDD_SET_REPORT) == CSR_BT_HIDD_SET_REPORT )
                {
                    CsrBtHiddControlIndSend(instData, (CsrUint8)(prim->payload[0] & 0xF0), (CsrUint8)(prim->payload[0] & 0x0F), prim->length, prim->payload);
                }
                else if( ((prim->payload[0]) & CSR_BT_HIDD_GET_REPORT) == CSR_BT_HIDD_GET_REPORT )
                {
                    if(prim->length == 1)
                    {
                        CsrBtHiddControlIndSend(instData, (CsrUint8)(prim->payload[0] & 0xF0), (CsrUint8)(prim->payload[0] & 0x0F), 0, NULL);
                        CsrPmemFree(prim->payload);
                    }
                    else
                    {
                        CsrBtHiddControlIndSend(instData, (CsrUint8)(prim->payload[0] & 0xF0), (CsrUint8)(prim->payload[0] & 0x0F), prim->length, prim->payload);
                    }
                }
                else if( ((prim->payload[0]) & CSR_BT_HIDD_SET_IDLE) == CSR_BT_HIDD_SET_IDLE )
                {
                    CsrBtHiddControlIndSend(instData, (CsrUint8)(prim->payload[0] & 0xF0), (CsrUint8)(prim->payload[0] & 0x0F), prim->length, prim->payload);
                }
                else
                {
                    CsrBtHiddControlIndSend(instData, (CsrUint8)(prim->payload[0] & 0xF0), (CsrUint8)(prim->payload[0] & 0x0F), 0, NULL);
                    CsrPmemFree(prim->payload);
                }
            }
            else if( pCurrentCh->cid ==  instData->intrCh.cid)
            {
                CsrBtHiddDataIndSend(instData, (CsrUint8)(prim->payload[0] & 0x0F), prim->length, prim->payload);
            }
            else
            {
                CsrPmemFree(prim->payload);
            }
        }
        else if(prim->length == pCurrentCh->localMtu)
        {
            if( (pCurrentCh->cid == instData->ctrlCh.cid) && (((prim->payload[0]) & CSR_BT_HIDD_SET_REPORT) == CSR_BT_HIDD_SET_REPORT) )
            {
                pCurrentCh->reportType = (CsrUint8)(prim->payload[0] & 0x0F);
            }
            else
            {
                pCurrentCh->reportType = 0;
            }
            pCurrentCh->recvMsgLength = prim->length;
            pCurrentCh->recvMsg = CsrPmemAlloc(pCurrentCh->recvMsgLength);
            CsrMemCpy(pCurrentCh->recvMsg, prim->payload, prim->length);

            CsrPmemFree(prim->payload);
        }
        else
        {
            /* Something wrong*/
            CsrPmemFree(prim->payload);
        }
    }
    else
    {
        /* No cid */
        CsrPmemFree(prim->payload);
    }

}

void CsrBtHiddCmModeChangeHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caModeChangeInd *prim;

    prim =  (CsrBtCmL2caModeChangeInd *)instData->pRecvMsg;

    if(instData->nextMode != CSR_BT_HIDD_DISCONNECT_MODE)
    {
        if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS   &&
           prim->resultSupplier == CSR_BT_SUPPLIER_CM)
        {
            if(instData->ctrlCh.cid == prim->btConnId)
            {
                if(!(instData->intrCh.modeChangeSuccess) && !(instData->ctrlCh.modeChangeSuccess))
                {
                    if(instData->intrCh.cid)
                    {
                        CsrBtCml2caModeChangeReqSend(instData->intrCh.cid, (CsrUint8)instData->nextMode);
                    }
                    else
                    {
                        CsrBtHiddModeChangeIndSend(instData, CSR_BT_HIDD_DISCONNECT_MODE,
                                                   CSR_BT_RESULT_CODE_HIDD_CONNECTION_TERM_BY_REMOTE_HOST,
                                                   CSR_BT_SUPPLIER_HIDD);
                    }
                }
                instData->ctrlCh.modeChangeSuccess = TRUE;
            }
            else if(instData->intrCh.cid == prim->btConnId)
            {
                instData->intrCh.modeChangeSuccess = TRUE;
            }

            if(instData->intrCh.modeChangeSuccess && instData->ctrlCh.modeChangeSuccess && (instData->currentMode != prim->mode))
            {
                instData->currentMode = prim->mode;
                instData->nextMode = prim->mode;
                CsrBtHiddModeChangeIndSend(instData, prim->mode, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_CONNECTED_STATE);
            }
        }
        else
        {
            instData->nextMode = prim->mode;
            instData->currentMode = prim->mode;
            CsrBtHiddModeChangeIndSend(instData, prim->mode, prim->resultCode, prim->resultSupplier);
        }
    }
}

void CsrBtHiddCmDisconnectHandler(HiddInstanceDataType *instData)
{
    CsrBtCmL2caDisconnectInd *prim;
    CsrBool disconnected;

    prim = (CsrBtCmL2caDisconnectInd *)instData->pRecvMsg;
    disconnected = FALSE;

    CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, instData->bdAddr, prim->btConnId);

    if(instData->intrCh.cid == prim->btConnId)
    {
        instData->intrCh.cid = 0;
        if(instData->ctrlCh.cid)
        {
            if(prim->localTerminated)
            {
                CsrBtCml2caDisconnectReqSend(instData->ctrlCh.cid);
            }
            else
            {
                /* Wait for disconnect on control channel */
            }
        }
        else
        {
            /* Normaly this won't happen */
            disconnected = TRUE;
        }
    }
    else if(instData->ctrlCh.cid == prim->btConnId)
    {
        instData->ctrlCh.cid = 0;
        if(instData->intrCh.cid)
        {
            /* Interrupt channel is not disconnected, it should have been, so now disconnect!!! */
            if(prim->localTerminated)
            {
                CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
            }
            else
            {
                /* Wait for disconnect on interrupt channel */
            }
        }
        else
        {
            disconnected = TRUE;
        }
    }

    if(disconnected)
    {
        if(!instData->deactivating && !instData->cleanUp &&
           (prim->reasonSupplier == CSR_BT_SUPPLIER_HCI &&
            (prim->reasonCode == HCI_ERROR_CONN_TIMEOUT ||
             prim->reasonCode == HCI_ERROR_OETC_USER)))
        {
            if(instData->hidFlags & CSR_BT_HIDD_FLAGS_RECONNECT_INIT_BIT)
            {
                instData->reconnect = TRUE;
            }
            else
            {
                if(instData->hidFlags & CSR_BT_HIDD_FLAGS_VIRTUAL_CABLE_BIT)
                {
                    instData->reconnect = TRUE;
                }
            }
            if(instData->reconnect)
            {
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_RECONNECTING,
                                       CSR_BT_CONN_ID_INVALID);
                CsrBtHiddReactivateIndSend(instData);
            }
            else
            {
                CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
                CsrBtHiddStatusIndSend(instData,
                                       CSR_BT_HIDD_DISCONNECTED,
                                       CSR_BT_CONN_ID_INVALID);
            }
        }
        else
        {
            switch(instData->state)
            {
            case HIDD_ACCEPT_CONNECTING_STATE:
                {
                    if(instData->prevError || instData->deactivating || instData->cleanUp)
                    {
                        /* Case: Failed but one channel was connected.
                                 Now unregister service records */
                        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                        instData->numOfRetries = 0;
                        CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
                    }
                    else
                    {
                        /* Unexpected local disconnect */
                        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
                        CsrBtHiddStatusIndSend(instData,
                                               CSR_BT_HIDD_DISCONNECTED,
                                               CSR_BT_CONN_ID_INVALID);
                    }
                }
                break;
            case HIDD_CONNECTING_STATE:
                /* FALL THROUGH */
            case HIDD_UNREGISTERING_SDP:
                {
                    if(instData->cleanUp)
                    {
                        CsrBtHiddCleanUpToIdle(instData);
                    }
                    else if(instData->deactivating)
                    {
                        CsrBtHiddDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                        CsrBtHiddCleanUpToIdle(instData);
                    }
                    else if(instData->prevError)
                    {
                        /* Case: Failed but one channel was connected.
                                 Cleanup and return to idle */
                        CsrBtHiddStatusIndSend(instData,
                                               CSR_BT_HIDD_CONNECT_FAILED,
                                               CSR_BT_CONN_ID_INVALID);
                        CsrBtHiddCleanUpToIdle(instData);
                    }
                    else
                    {
                        /* Case: DisconnectInd while changing mode
                                 Cleanup and return to idle */
                        CsrBtHiddStatusIndSend(instData,
                                               CSR_BT_HIDD_DISCONNECTED,
                                               CSR_BT_CONN_ID_INVALID);
                        CsrBtHiddCleanUpToIdle(instData);

                    }
                }
                break;
            case HIDD_CONNECTED_STATE:
                {
                    if(instData->cleanUp)
                    {
                        CsrBtHiddCleanUpToIdle(instData);
                    }
                    else if(instData->deactivating)
                    {
                        CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_UNREGISTERING_SDP);
                        instData->numOfRetries = 0;
                        instData->deactUnregCfmRec = FALSE;
                        if (prim->localTerminated)
                        {
                            instData->prevError = CSR_BT_RESULT_CODE_HIDD_CONNECTION_TERM_BY_LOCAL_HOST;
                        }
                        else
                        {
                            instData->prevError = CSR_BT_RESULT_CODE_HIDD_CONNECTION_TERM_BY_REMOTE_HOST;
                        }
                        CsrBtCmSdsUnRegisterReqSend(instData->myAppHandle, instData->hidSdp.recordHandle);
                    }
                    else
                    {
                        if(HIDD_CONNECTED_UNPLUG_IND == instData->disconnectReason)
                        {
                            CsrBtHiddUnplugIndSend(instData, instData->bdAddr, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                            instData->cleanUp = TRUE;
                            CsrBtHiddCleanUpToIdle(instData);
                            instData->disconnectReason = HIDD_CONNECTED_UNPLUG_IND;
                        }
                        else if(HIDD_CONNECTED_UNPLUG_REQ == instData->disconnectReason)
                        {
                            CsrBtHiddUnplugCfmSend(instData, CSR_BT_RESULT_CODE_HIDD_SUCCESS, CSR_BT_SUPPLIER_HIDD);
                            instData->cleanUp = TRUE;
                            CsrBtHiddCleanUpToIdle(instData);
                            instData->disconnectReason = HIDD_CONNECTED_UNPLUG_REQ;
                        }
                        else if(HIDD_CONNECTED_CHANGE_MODE == instData->disconnectReason)
                        {
                            CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_NOT_CONNECTED_STATE);
                            instData->currentMode = CSR_BT_HIDD_DISCONNECT_MODE;
                            CsrBtHiddModeChangeIndSend(instData,
                                                       CSR_BT_HIDD_DISCONNECT_MODE,
                                                       CSR_BT_RESULT_CODE_HIDD_SUCCESS,
                                                       CSR_BT_SUPPLIER_HIDD);
                        }
                        else
                        {
                            /* Unexpected local disconnect */
                            CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
                            CsrBtHiddStatusIndSend(instData,
                                                   CSR_BT_HIDD_DISCONNECTED,
                                                   CSR_BT_CONN_ID_INVALID);
                        }
                    }
                }
                break;
            case HIDD_NOT_CONNECTED_STATE:
                {
                    CsrBtHiddModeChangeIndSend(instData, instData->currentMode,
                                               instData->prevError, instData->prevErrorSupplier);
                    CsrBtHiddCleanUpToIdle(instData);
                }
                break;
            default:
                /* Unexspected state */
                break;
            }
        }
    }
}

void CsrBtHiddCmDisconnectIgnoreHandler(HiddInstanceDataType *instData)
{
    CSR_UNUSED(instData);
    /*
     * HIDD_IDLE_STATE
 */
}

void CsrBtHiddSecurityInHandler(HiddInstanceDataType *instData)
{
    CsrBtResultCode rval;
    CsrBtHiddSecurityInReq *prim;

    prim = (CsrBtHiddSecurityInReq*)instData->pRecvMsg;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_HIDD_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HIDD_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HIDD_SUCCESS,
        CSR_BT_RESULT_CODE_HIDD_UNACCEPTABLE_PARAMETER);

    CsrBtHiddSecurityInCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_HIDD);
}

void CsrBtHiddSecurityOutHandler(HiddInstanceDataType *instData)
{
    CsrBtResultCode rval;
    CsrBtHiddSecurityOutReq *prim;

    prim = (CsrBtHiddSecurityOutReq*)instData->pRecvMsg;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_HIDD_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HIDD_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HIDD_SUCCESS,
        CSR_BT_RESULT_CODE_HIDD_UNACCEPTABLE_PARAMETER);

    CsrBtHiddSecurityOutCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_HIDD);
}

