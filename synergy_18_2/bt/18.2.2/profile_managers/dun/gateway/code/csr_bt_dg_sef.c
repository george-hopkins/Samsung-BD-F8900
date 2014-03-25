/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_dg_main.h"
#include "csr_bt_dg_sef.h"
#include "csr_bt_dg_prim.h"

#define DUN_MAJOR_VERSION                 0x01
#define DUN_MINOR_VERSION                 0x00
#define DG_SERVER_CHANNEL_INDEX           (30)

static const CsrUint8 sdsDgServiceRecord[] =
{
    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    '\x00', '\x01',                            /* ServiceClassIdList */

    '\x35', '\x05',                            /* type: Data Element Sequence, size: additional 8 bits, length: 5 bytes */
    '\x1a',                                    /* type: UUID, size: 4 bytes */
    '\x00', '\x00', '\x11', '\x03',            /* DialupNetworking */

    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    '\x00', '\x04',                            /* Protocol DescriptorList */
    '\x35', '\x10',                            /* type: Data Element Sequence, size: additional 8 bits, length: 16 bytes */
    '\x35', '\x05',                            /* type: Data Element Sequence, size: additional 8 bits, length: 5 bytes */
    '\x1a',                                    /* type: UUID, size: 4 bytes */
    '\x00', '\x00', '\x01', '\x00',            /* L2CAP */
    '\x35', '\x07',                            /* type: Data Element Sequence, size: additional 8 bits, length: 7 bytes */
    '\x1a',                                    /* type: UUID, size: 4 bytes */
    '\x00', '\x00', '\x00', '\x03',            /* RFCOMM */
    '\x08',                                    /* type: Unsigned Integer, size: 1 byte */
    '\x01',                                    /* server channel */

    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    '\x00',    '\x09',                         /* BluetoothProfileDescriptorList */
    '\x35',    '\x0a',                         /* type: Data Element Sequence, size: additional 8 bits, length: 10 bytes */
    '\x35',    '\x08',                         /* type: Data Element Sequence, size: additional 8 bits, length: 8 bytes */
    '\x1a',                                    /* type: UUID, size: 4 byte */
    '\x00',    '\x00',    '\x11',    '\x03',   /* DialupNetworking */
    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    DUN_MAJOR_VERSION, DUN_MINOR_VERSION,      /* version */

    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    '\x01', '\x00',                            /* ServiceName */
    '\x25', '\x12',                            /* type: Text String, size: additional 8 bits, length: 18 */
    'D','i','a','l','-','u','p',' ','n','e','t','w','o','r','k','i','n','g',

    /* BrowseGroupList    */
    '\x09',                                    /* type: Unsigned Integer, size: 2 bytes */
    '\x00', '\x05',                            /* BrowseGroupList */
    '\x35', '\x05',                            /* type: Data Element Sequence, size: additional 8 bits, length: 5 bytes */
    '\x1a',                                    /* type: UUID, size: 4 bytes */
    '\x00', '\x00', '\x10', '\x02',            /* PublicBrowseGroup */
};

void CsrBtDgMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_DG_PRIM, msg);
}

/***********************************************************************************
    Internal message factory functions.
    Used by DG when sending messages internally or to higher layer.
***********************************************************************************/
void CsrBtDgHouseCleaningSend(void)
{
    CsrBtDgHouseCleaning    *dgPrim;

    dgPrim = (CsrBtDgHouseCleaning *) CsrPmemAlloc(sizeof(CsrBtDgHouseCleaning));
    dgPrim->type = CSR_BT_DG_HOUSE_CLEANING;
    CsrBtDgMessagePut(CSR_BT_DG_IFACEQUEUE, dgPrim);
}

void CsrBtDgSdsUnRegisterReqSendWithDelay(DgInstanceData_t *instData)
{
    CsrBtCmSdsUnRegisterReqSend(CSR_BT_DG_IFACEQUEUE, instData->sdsRecHandle);
}

void CsrBtDgSdsRegisterReqSend(DgInstanceData_t *instData)
{
    CsrUint16    num_rec_bytes;
    CsrUint8        *record;

    /* register the record in SDS */
    num_rec_bytes = sizeof(sdsDgServiceRecord);
    record = (CsrUint8 *) CsrPmemAlloc(num_rec_bytes );
    CsrMemCpy(record, sdsDgServiceRecord, num_rec_bytes);
    record[DG_SERVER_CHANNEL_INDEX] = instData->serverChannel;

    CsrBtCmSdsRegisterReqSend(CSR_BT_DG_IFACEQUEUE, record, num_rec_bytes);
}

void CsrBtDgDeactivateCfmSend(DgInstanceData_t *instData,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtDgDeactivateCfm * dgPrim;
    dgPrim = (CsrBtDgDeactivateCfm *) CsrPmemAlloc(sizeof(CsrBtDgDeactivateCfm));
    dgPrim->type = CSR_BT_DG_DEACTIVATE_CFM;
    dgPrim->resultCode= resultCode;
    dgPrim->resultSupplier= resultSupplier;

    CsrBtDgMessagePut(instData->ctrlHandle, dgPrim);
}

/* Data application send with fallback to control handle */
static void csrBtDgDataPathSend(DgInstanceData_t *instData, void *msg)
{
    CsrUint32 target;
    target = instData->dataHandle != CSR_SCHED_QID_INVALID ? instData->dataHandle : instData->ctrlHandle;
    CsrBtDgMessagePut((CsrSchedQid)target, msg);
}

/* Send CSR_BT_DG_STATUS_IND to data handle */
static void csrBtDgStatusIndSend(DgInstanceData_t *instData, CsrBool connect, CsrUint16 maxMsgSize, CsrBtDeviceAddr addr)
{
    if((instData->dataHandle != CSR_SCHED_QID_INVALID) && (instData->dataHandle != instData->ctrlHandle))
    {
        CsrBtDgStatusInd *prim;
        prim                = (CsrBtDgStatusInd*)CsrPmemAlloc(sizeof(CsrBtDgStatusInd));
        prim->type          = CSR_BT_DG_STATUS_IND;
        prim->btConnId      = instData->btConnId;
        prim->connect       = connect;
        prim->maxMsgSize    = maxMsgSize;
        prim->deviceAddr    = addr;

        CsrBtDgMessagePut(instData->dataHandle, prim);
    }
}

/* Send CSR_BT_DG_DATA_PATH_STATUS_IND to control app */
static void csrBtDgDataPathStatusIndSend(DgInstanceData_t *instData, CsrUint8 status,
    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtDgDataPathStatusInd *prim;
    prim                   = (CsrBtDgDataPathStatusInd*)CsrPmemAlloc(sizeof(CsrBtDgDataPathStatusInd));
    prim->type             = CSR_BT_DG_DATA_PATH_STATUS_IND;
    prim->dgInstanceQueue  = instData->dataHandle;
    prim->status           = status;
    prim->resultCode       = resultCode;
    prim->resultSupplier   = resultSupplier;
    CsrBtDgMessagePut(instData->ctrlHandle, prim);
}


/***********************************************************************************
    Send a disconnect ind message to the registered higher layer
***********************************************************************************/
void CsrBtDgDisconnectIndSend(DgInstanceData_t *instData)
{
    CsrBtCmDisconnectInd *cmPrim;
    CsrBtDgDisconnectInd *dgPrim;
    CsrBtDeviceAddr addr;

    /* Send a status indication (disconnect) to the data handle */
    bd_addr_zero(&addr);
    csrBtDgStatusIndSend(instData, FALSE, 0, addr);

    /* Now send the disconnect */
    cmPrim = (CsrBtCmDisconnectInd *) instData->recvMsgP;
    dgPrim = (CsrBtDgDisconnectInd *) CsrPmemAlloc(sizeof(CsrBtDgDisconnectInd));
    dgPrim->type = CSR_BT_DG_DISCONNECT_IND;
    dgPrim->deviceAddr = instData->bdAddr;
    dgPrim->btConnId = cmPrim->btConnId;
    dgPrim->localTerminated = cmPrim->localTerminated;
    dgPrim->reasonCode = cmPrim->reasonCode;
    dgPrim->reasonSupplier = cmPrim->reasonSupplier;

    CsrBtDgMessagePut(instData->ctrlHandle, dgPrim);
}

/***********************************************************************************
    Cleanup from lost control/data handle
***********************************************************************************/
void CsrBtDgEnvironmentCleanupHandler(DgInstanceData_t *instData)
{
    CsrCleanupInd *prim;
    CsrBtDeviceAddr addr;

    prim              = (CsrCleanupInd*)instData->recvMsgP;

    /* Dead control application */
    if(prim->phandle == instData->ctrlHandle)
    {
        /* Send status indication to data app before the disconnect */
        bd_addr_zero(&addr);
        csrBtDgStatusIndSend(instData, FALSE, 0, addr);

        /* If we're in the connected state, disconnect but don't wait for confirm as it doesn't matter */
        if((instData->state == Activated_s) || (instData->state == Connected_s))
        {
            instData->state = Idle_s;
            CsrBtCmDisconnectReqSend(instData->btConnId);
        }
        instData->ctrlHandle = CSR_SCHED_QID_INVALID;
    }

    /* Dead data application */
    else if(prim->phandle == instData->dataHandle)
    {
        /* Send a data path lost indication to control app */
        csrBtDgDataPathStatusIndSend(instData, CSR_BT_DATA_PATH_STATUS_LOST,
            CSR_BT_RESULT_CODE_DG_UNSPECIFIED_ERROR,
            CSR_BT_SUPPLIER_DG);
        instData->dataHandle = CSR_SCHED_QID_INVALID;
    }
}

/*************************************************************************************
    Set data application handle
************************************************************************************/
void CsrBtDgXStateRegisterDataPathHandleReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgRegisterDataPathHandleReq *reqPrim;

    /* Set handle */
    reqPrim = (CsrBtDgRegisterDataPathHandleReq*)instData->recvMsgP;
    instData->dataHandle = reqPrim->dataAppHandle;

    if (instData->ctrlHandle != CSR_SCHED_QID_INVALID)
    {/* Send confirm */
        CsrBtDgRegisterDataPathHandleCfm *cfmPrim;
        cfmPrim = (CsrBtDgRegisterDataPathHandleCfm*)CsrPmemAlloc(sizeof(CsrBtDgRegisterDataPathHandleCfm));
        cfmPrim->type = CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM;
        cfmPrim->resultCode = CSR_BT_RESULT_CODE_DG_SUCCESS;
        cfmPrim->resultSupplier = CSR_BT_SUPPLIER_DG;
        CsrBtDgMessagePut(instData->ctrlHandle, cfmPrim);
    }
    else
    {
        instData->registerDataPathPending = TRUE;
    }
}


/*************************************************************************************
     Handle data path status changes - send indication to control handle
************************************************************************************/
void CsrBtDgConnectedStateDgDataPathStatusReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgDataPathStatusReq *reqPrim;
    CsrBtDgDataPathStatusInd *indPrim;

    reqPrim = (CsrBtDgDataPathStatusReq*)instData->recvMsgP;
    indPrim = (CsrBtDgDataPathStatusInd*)CsrPmemAlloc(sizeof(CsrBtDgDataPathStatusInd));

    indPrim->type             = CSR_BT_DG_DATA_PATH_STATUS_IND;
    indPrim->dgInstanceQueue  = reqPrim->dgInstanceQueue;
    indPrim->status           = reqPrim->status;
    indPrim->resultCode       = CSR_BT_RESULT_CODE_DG_SUCCESS;
    indPrim->resultSupplier   = CSR_BT_SUPPLIER_DG;

    CsrBtDgMessagePut(instData->ctrlHandle, indPrim);
}


/*************************************************************************************
    cm register cfm response
************************************************************************************/
void CsrBtDgInitStateCmRegisterCfmHandler(DgInstanceData_t *instData)
{
    CsrBtCmRegisterCfm *cmPrim;

    cmPrim = (CsrBtCmRegisterCfm *) instData->recvMsgP;
    instData->serverChannel  = cmPrim->serverChannel;
    instData->state          = Idle_s;
    instData->restoreDgFlag  = TRUE;
    CsrBtDgHouseCleaningSend();
}

/*************************************************************************************
    The server channel is registered in SDS.
************************************************************************************/
void CsrBtDgInitStateSdsRegisterCfm(DgInstanceData_t *instData)
{
    CsrBtCmSdsRegisterCfm * prim = (CsrBtCmSdsRegisterCfm *) instData->recvMsgP;

    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmConnectAcceptReqSend(CSR_BT_DG_IFACEQUEUE,
                               DG_CLASS_OF_DEVICE,
                               0,
                               CSR_BT_DG_PROFILE_DEFAULT_MTU_SIZE,
                               instData->serverChannel,
                               instData->secIncoming,
                               CSR_BT_DUN_PROFILE_UUID,
                               instData->modemStatus,
                               instData->breakSignal,
                               CSR_BT_DEFAULT_MSC_TIMEOUT);
        instData->state             = Activated_s;
        instData->sdsRecHandle      = prim->serviceRecHandle;
        instData->sdsRecordObtain   = TRUE;
        instData->restoreDgFlag     = TRUE;
        instData->deactivateFlag    = FALSE;
        CsrBtDgHouseCleaningSend();
    }
    else
    {
        CsrBtDgSdsRegisterReqSend(instData);
    }
}

/*************************************************************************************
    Sds record has been removed. Send deactivate cfm to app
************************************************************************************/
void CsrBtDgInitStateSdsUnregisterCfm(DgInstanceData_t *instData)
{
    CsrBtCmSdsUnregisterCfm *cmPrim;

    cmPrim = (CsrBtCmSdsUnregisterCfm *) instData->recvMsgP;
    /*    the cancel is successful so send the cancel confirmation to the higher
          layer and set instance variables */
    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtDgDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_DG_SUCCESS,
            CSR_BT_SUPPLIER_DG);
        instData->state            = Idle_s;
        instData->sdsRecordObtain  = FALSE;
        instData->restoreDgFlag    = TRUE;
        CsrBtDgHouseCleaningSend();
    }
    else
    {/* try again */
        CsrBtDgSdsUnRegisterReqSendWithDelay(instData);
    }
}

/*************************************************************************************
    Activate service. Allow client side to connect.
************************************************************************************/
void CsrBtDgIdleStateDgActivateReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgActivateReq    *prim;

    prim = (CsrBtDgActivateReq *) instData->recvMsgP;
    instData->ctrlHandle = prim->phandle;

    if (instData->registerDataPathPending)
    {
        CsrBtDgRegisterDataPathHandleCfm *cfmPrim;
        cfmPrim = (CsrBtDgRegisterDataPathHandleCfm*)CsrPmemAlloc(sizeof(CsrBtDgRegisterDataPathHandleCfm));
        cfmPrim->type = CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM;
        cfmPrim->resultCode = CSR_BT_RESULT_CODE_DG_SUCCESS;
        cfmPrim->resultSupplier = CSR_BT_SUPPLIER_DG;
        CsrBtDgMessagePut(instData->ctrlHandle, cfmPrim);
    }

    if (instData->sdsRecordObtain)
    {
        CsrBtCmConnectAcceptReqSend(CSR_BT_DG_IFACEQUEUE,
                               DG_CLASS_OF_DEVICE,
                               prim->timeout,
                               CSR_BT_DG_PROFILE_DEFAULT_MTU_SIZE,
                               instData->serverChannel,
                               instData->secIncoming,
                               CSR_BT_DUN_PROFILE_UUID,
                               instData->modemStatus,
                               instData->breakSignal,
                               CSR_BT_DEFAULT_MSC_TIMEOUT);
        instData->state = Activated_s;
        instData->deactivateFlag = FALSE;
    }
    else
    {
        CsrBtDgSdsRegisterReqSend(instData);
        instData->state = Init_s;
        instData->restoreDgFlag = FALSE;
    }
}

/*************************************************************************************
    Downstream data received. Send the data to CM.

    Please note: the original signal is reused in order to optimise.
************************************************************************************/

void CsrBtDgConnectedStateDgDataReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgDataReq    *prim;

    prim = (CsrBtDgDataReq *) instData->recvMsgP;
    prim->type = CSR_BT_CM_DATA_REQ;
    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);
    /*    the received signal is passed on to CM so set pointer to NULL in order
          not to free it in the main function */
    instData->recvMsgP = NULL;
}

/*************************************************************************************
    Downstream data is received and consumed by the CM. Send the confirm
    signal to the higher layer. The higher layer must not send any more data
    before the data is confirmed by the CM

    Please note: the original signal is reused in order to optimise.
************************************************************************************/
void CsrBtDgConnectedStateCmDataCfmHandler(DgInstanceData_t *instData)
{
    CsrBtCmDataCfm    *prim;

    prim = (CsrBtCmDataCfm *) instData->recvMsgP;
    prim->type          = CSR_BT_DG_DATA_CFM;
    prim->btConnId      = instData->btConnId;

    csrBtDgDataPathSend(instData, prim);

    /*    the received signal is passed on to the higher layer so set pointer to NULL in order
          not to free it in the main function */
    instData->recvMsgP = NULL;
}

/*************************************************************************************
    Upstream data is received. Send the data on to the application.

    Please note: the original signal is reused in order to optimise.
************************************************************************************/
void CsrBtDgConnectedStateCmDataIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmDataInd    *prim;

    prim                = (CsrBtCmDataInd *) instData->recvMsgP;
    prim->type          = CSR_BT_DG_DATA_IND;
    prim->btConnId      = instData->btConnId;

    csrBtDgDataPathSend(instData, prim);

    /*    the received signal is passed on to the higher layer so set pointer to NULL in order
          not to free it in the main function */
    instData->recvMsgP = NULL;
}

/*************************************************************************************
    Upstream data is received and consumed by the higher layer. Send the confirm
    signal to the CM. The higher layer will not receive any more data before the
    data is confirmed by the higher layer

    Please note: the original signal is reused in order to optimise.
************************************************************************************/
void CsrBtDgConnectedStateDgDataResHandler(DgInstanceData_t *instData)
{
    CsrBtDgDataRes    *prim;

    prim = (CsrBtDgDataRes *) instData->recvMsgP;
    prim->type = CSR_BT_CM_DATA_RES;
    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);
    /*    the received signal is passed on to CM so set pointer to NULL in order
          not to free it in the main function */
    instData->recvMsgP = NULL;
}


/*************************************************************************************
    Send a control signal.
************************************************************************************/
void CsrBtDgConnectedStateDgControlReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgControlReq    *prim;
    CsrUint8                myModemStatus;

    prim = (CsrBtDgControlReq *) instData->recvMsgP;
    myModemStatus = CsrBtMapSendingControlSignal(prim->modemstatus, CSR_BT_DCE);
    CsrBtCmControlReqSend(prim->btConnId, myModemStatus, prim->break_signal);
    instData->modemStatus = myModemStatus;
    instData->breakSignal = prim->break_signal;
}

/*************************************************************************************
    Send a control signal.
************************************************************************************/
void CsrBtDgConnectedStateCmControlIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmControlInd    *cmPrim;
    CsrBtDgControlInd    *dgPrim;

    cmPrim = (CsrBtCmControlInd *) instData->recvMsgP;
    dgPrim = (CsrBtDgControlInd *)CsrPmemAlloc(sizeof(CsrBtDgControlInd));
    dgPrim->type          = CSR_BT_DG_CONTROL_IND;
    dgPrim->modemstatus   = CsrBtMapReceivedControlSignal(cmPrim->modemstatus, CSR_BT_DCE);
    dgPrim->btConnId      = cmPrim->btConnId;
    dgPrim->break_signal  = cmPrim->break_signal;

    csrBtDgDataPathSend(instData, dgPrim);
}

/*************************************************************************************
    port negotiation res from the app send to CM
************************************************************************************/
void CsrBtDgConnectedStateDgPortnegResHandler(DgInstanceData_t *instData)
{
    CsrBtDgPortnegRes    *dgPrim;

    dgPrim =  (CsrBtDgPortnegRes *) instData->recvMsgP;

    CsrBtCmPortnegResSend(dgPrim->btConnId, (RFC_PORTNEG_VALUES_T *)&(dgPrim->portPar));
}
/*************************************************************************************
    Request a disconnect of the logical connection.
************************************************************************************/
void CsrBtDgConnectedStateDgDisconnectReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgDisconnectReq *dgPrim;
    dgPrim = (CsrBtDgDisconnectReq *) instData->recvMsgP;
    CsrBtCmDisconnectReqSend(dgPrim->btConnId);
}

/*************************************************************************************
    Disconnect the logical connection.
************************************************************************************/
void CsrBtDgConnectedStateCmDisconnectIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmDisconnectInd *cmPrim;

    cmPrim = (CsrBtCmDisconnectInd *)instData->recvMsgP;

    if (cmPrim->status == TRUE)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->bdAddr,
                        instData->btConnId);
        /* send the disconnect to the higher layer and set instance variables */
        if (!instData->deactivateFlag)
        {
            instData->state = Activated_s;

            /* This local function will notify both data and control app */
            CsrBtDgDisconnectIndSend(instData);
            
            CsrBtCmConnectAcceptReqSend(CSR_BT_DG_IFACEQUEUE,
                                   DG_CLASS_OF_DEVICE,
                                   0,
                                   CSR_BT_DG_PROFILE_DEFAULT_MTU_SIZE,
                                   instData->serverChannel,
                                   instData->secIncoming,
                                   CSR_BT_DUN_PROFILE_UUID,
                                   instData->modemStatus,
                                   instData->breakSignal,
                                   CSR_BT_DEFAULT_MSC_TIMEOUT);
            instData->deactivateFlag = FALSE;
        }
        else
        {
            instData->state = Init_s;
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_DG_IFACEQUEUE, instData->sdsRecHandle);
        }
    }
    else
    {
        /* Disconnect failed, retry. */
        CsrBtCmDisconnectReqSend(cmPrim->btConnId);
    }
}

/*************************************************************************************
    The link is disconnected, info app if correct state.
************************************************************************************/
void CsrBtDgIdleStateCmDisconnectIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmDisconnectInd *cmPrim;

    cmPrim = (CsrBtCmDisconnectInd *)instData->recvMsgP;

    if (cmPrim->status == TRUE)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->bdAddr,
                        cmPrim->btConnId);
        if (instData->deactivateFlag)
        {
            CsrBtDgDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_DG_SUCCESS,
                CSR_BT_SUPPLIER_DG);
        }
    }
    else
    {
        /* Disconnect failed, retry. */
        CsrBtCmDisconnectReqSend(cmPrim->btConnId);
    }
}

/*************************************************************************************
    Deactivate the profile in connected state, first disconnect when cancel accept.
************************************************************************************/
void CsrBtDgConnectedStateDgDeactivateReqHandler(DgInstanceData_t *instData)
{
    instData->deactivateFlag = TRUE;
    CsrBtCmDisconnectReqSend(instData->btConnId);
}

/*************************************************************************************
    Deactivate the service by turning off inquiry scan. Send message to CM and wait
    for the confirm message that scanning is turned off.
************************************************************************************/
void CsrBtDgActivateStateDgDeactivateReqHandler(DgInstanceData_t *instData)
{
    instData->deactivateFlag    = TRUE;
    instData->restoreDgFlag        = FALSE;
    CsrBtCmCancelAcceptConnectReqSend(CSR_BT_DG_IFACEQUEUE, instData->serverChannel);
}

/*************************************************************************************
    Profile is already deactivated, send cfm to app.
************************************************************************************/
void CsrBtDgIdleStateDgDeactivateReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_DG_SUCCESS,
        CSR_BT_SUPPLIER_DG);
}

/*************************************************************************************
    Peer side has connected.
************************************************************************************/
void CsrBtDgDeOrActivateStateCmConnectAcceptCfmHandler(DgInstanceData_t *instData)
{
    CsrBtCmConnectAcceptCfm    *cmPrim;

    cmPrim = (CsrBtCmConnectAcceptCfm *) instData->recvMsgP;
    /*    a peer device has connected */
    if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_DATA_CHANNEL,cmPrim->deviceAddr,
                        cmPrim->btConnId);
        if (instData->deactivateFlag)
        {
            CsrBtCmDisconnectReqSend(cmPrim->btConnId);
        }
        else
        {
            CsrBtDgConnectInd    *dgPrim;
            dgPrim = (CsrBtDgConnectInd *) CsrPmemAlloc(sizeof(CsrBtDgConnectInd));
            dgPrim->type = CSR_BT_DG_CONNECT_IND;
            dgPrim->resultCode = CSR_BT_RESULT_CODE_DG_SUCCESS;
            dgPrim->resultSupplier = CSR_BT_SUPPLIER_DG;
            dgPrim->serverChannel = cmPrim->serverChannel;
            dgPrim->deviceAddr = cmPrim->deviceAddr;
            dgPrim->btConnId = cmPrim->btConnId;
            dgPrim->profileMaxFrameSize = cmPrim->profileMaxFrameSize;

            instData->bdAddr = cmPrim->deviceAddr;
            instData->state = Connected_s;
            instData->btConnId = cmPrim->btConnId;

            CsrBtDgMessagePut(instData->ctrlHandle, dgPrim);

            /* Send a status indication to the data handle after the connect */
            csrBtDgStatusIndSend(instData, TRUE, cmPrim->profileMaxFrameSize, cmPrim->deviceAddr);
        }
    }
    /*    an error (it might be a timeout or other errors) occurred while accepting peer connections */
    else
    {
        if (!instData->deactivateFlag)
        {
            if (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_TIMEOUT)
            {
                CsrBtDgConnectInd    *dgPrim;

                dgPrim = (CsrBtDgConnectInd *) CsrPmemAlloc(sizeof(CsrBtDgConnectInd));
                dgPrim->type = CSR_BT_DG_CONNECT_IND;
                dgPrim->resultCode = cmPrim->resultCode;
                dgPrim->resultSupplier = cmPrim->resultSupplier;
                dgPrim->serverChannel = cmPrim->serverChannel;
                dgPrim->deviceAddr = cmPrim->deviceAddr;
                dgPrim->btConnId = cmPrim->btConnId;
                dgPrim->profileMaxFrameSize = 0;

                instData->bdAddr = cmPrim->deviceAddr;
                instData->state = Idle_s;
                CsrBtDgMessagePut(instData->ctrlHandle, dgPrim);
            }
            else
            {
                instData->state = Activated_s;
                CsrBtCmConnectAcceptReqSend(CSR_BT_DG_IFACEQUEUE,
                                       DG_CLASS_OF_DEVICE,
                                       0,
                                       CSR_BT_DG_PROFILE_DEFAULT_MTU_SIZE,
                                       instData->serverChannel,
                                       instData->secIncoming,
                                       CSR_BT_DUN_PROFILE_UUID,
                                       instData->modemStatus,
                                       instData->breakSignal,
                                       CSR_BT_DEFAULT_MSC_TIMEOUT);
                instData->deactivateFlag = FALSE;
            }
        }
        else
        {
            instData->state = Init_s;
            CsrBtCmSdsUnRegisterReqSend(CSR_BT_DG_IFACEQUEUE, instData->sdsRecHandle);
        }
    }
}
/*************************************************************************************
    Disconnect the logical connection.
************************************************************************************/
void CsrBtDgActivateStateCmDisconnectIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmDisconnectInd *cmPrim;

    cmPrim = (CsrBtCmDisconnectInd *)instData->recvMsgP;

    if (cmPrim->status == TRUE)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL,instData->bdAddr,
                        instData->btConnId);
        instData->state = Idle_s;
        CsrBtDgDeactivateCfmSend(instData, CSR_BT_RESULT_CODE_DG_SUCCESS,
            CSR_BT_SUPPLIER_DG);
    }
    else
    {
        /* Disconnect failed, retry. */
        CsrBtCmDisconnectReqSend(cmPrim->btConnId);
    }
}

/*************************************************************************************
    Port negotiation indication. forward to the application.
************************************************************************************/
void CsrBtDgConnectedStateCmPortnegIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmPortnegInd  *cmPrim;
    CsrBtDgPortnegInd  *dgPrim;

    cmPrim = (CsrBtCmPortnegInd *) instData->recvMsgP;

    dgPrim = (CsrBtDgPortnegInd*)CsrPmemAlloc(sizeof(CsrBtDgPortnegInd));
    dgPrim->type          = CSR_BT_DG_PORTNEG_IND;
    dgPrim->btConnId      = cmPrim->btConnId;
    dgPrim->portPar       = cmPrim->portPar;
    dgPrim->request       = cmPrim->request;

    csrBtDgDataPathSend(instData, dgPrim);
}


/*************************************************************************************
    Cancel of activate confirmed. Continue with connection.
************************************************************************************/
void CsrBtDgActivateStateCmCancelAcceptConnectCfmHandler(DgInstanceData_t *instData)
{
    CsrBtCmCancelAcceptConnectCfm *cmPrim;

    cmPrim = (CsrBtCmCancelAcceptConnectCfm *) instData->recvMsgP;
    /*    the cancel is successful send the cancel confirmation to the higher layer and set instance variables */
    if(cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        CsrBtCmSdsUnRegisterReqSend(CSR_BT_DG_IFACEQUEUE, instData->sdsRecHandle);
        instData->state = Init_s;
    }
    /*    the cancel may fail if a connect is in progress */
    else
    {
        /*    ignore the signal and wait for the connect that will follow */
    }
}

/*************************************************************************************
    Data is received when not expected. Free the data and ignore the signal
************************************************************************************/
void CsrBtDgNotConnectedCmDataIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmDataInd    *prim;

    prim = (CsrBtCmDataInd *) instData->recvMsgP;
    CsrPmemFree(prim->payload);
}

/*************************************************************************************
    React to mode change by peer
************************************************************************************/
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
void CsrBtDgModeChangeIndHandler(DgInstanceData_t *instData)
{
    CsrBtCmRfcModeChangeInd * cmPrim;
    cmPrim = (CsrBtCmRfcModeChangeInd *) instData->recvMsgP;

    if(cmPrim->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCmRfcModeChangeReqSend(cmPrim->btConnId, CSR_BT_ACTIVE_MODE);
    }
}
#endif
/*************************************************************************************
    Data is received when not expected. Free the data and ignore the signal
************************************************************************************/
void CsrBtDgNotConnectedDgDataReqHandler(DgInstanceData_t *instData)
{
    CsrBtDgDataReq    *prim;

    prim = (CsrBtDgDataReq *) instData->recvMsgP;
    CsrPmemFree(prim->payload);
}

/*************************************************************************************
    A message is received which should be ignored. This may be due to the
    chance/risc of a race condition.
************************************************************************************/
void CsrBtDgIgnoreMessageHandler(DgInstanceData_t *instData)
{
    CSR_UNUSED(instData);
    /*  do nothing */
}

/*************************************************************************************
    Incoming security level changes/confirmation
************************************************************************************/
void CsrBtDgSecurityInReqHandler(DgInstanceData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtDgSecurityInReq *prim;

    prim = (CsrBtDgSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_DUN_GW_MANDATORY_SECURITY_INCOMING,
        CSR_BT_DUN_GW_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_DG_SUCCESS,
        CSR_BT_RESULT_CODE_DG_UNACCEPTABLE_PARAMETER);

    CsrBtDgSecurityInCfmSend(prim->appHandle,
        rval, CSR_BT_SUPPLIER_DG);
}

void CsrBtDgSecurityInCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
    CsrBtSupplier resultSupplier)
{
    CsrBtDgSecurityInCfm *prim;
    prim = (CsrBtDgSecurityInCfm*)CsrPmemAlloc(sizeof(CsrBtDgSecurityInCfm));
    prim->type = CSR_BT_DG_SECURITY_IN_CFM;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtDgMessagePut(appHandle, prim);
}


