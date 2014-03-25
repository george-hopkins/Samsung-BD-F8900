/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_tasks.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_main.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_hidh_util.h"
#include "csr_bt_hidh_sigup.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define RECONNECT_TIMEOUT            30000000 /* 30 seconds */

/*----------------------------------------------------------------------------*
 *  NAME
 *      writeLinkSupervisionTimerValue (internal fct.)
 *
 *  DESCRIPTION
 *      Request a change of the link supervision timeout value if different
 *        from default value.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void writeLinkSupervisionTimerValue( CsrBtDeviceAddr bdAddr, CsrUint16 value)
{
    if( value != HCI_LINK_SUPERVISION_DEFAULT)
    {
        CsrBtcmWriteLinkSuperVisionTimeoutReqSend(CSR_BT_HIDH_IFACEQUEUE, bdAddr, value);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCsrBtCml2caConnectReqSend (internal fct.)
 *
 *  DESCRIPTION
 *      Send CSR_BT_CM_L2CA_CONNECT_REQ with or without extended parameters
 *        for flush timeout and QoS.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCsrBtCml2caConnectReqSend(hidh_instData_t *instData, CsrUint16 deviceId, psm_t psm)
{
    hidh_subInstData_t *subInstData;
    subInstData = &(instData->subInstData[deviceId]);

    /* We don't support QOS yet in L2CAP */
    CsrBtCml2caConnectReqSend( CSR_BT_HIDH_IFACEQUEUE,
                          subInstData->bdAddr,
                          psm,
                          psm,
                          CSR_BT_HIDH_PROFILE_DEFAULT_MTU_SIZE,
                          instData->secOutgoing);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCsrBtCml2caConnectAcceptReqSend (internal fct.)
 *
 *  DESCRIPTION
 *      Send CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ with or without extended parameters
 *        for flush timeout and QoS.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCsrBtCml2caConnectAcceptReqSend(hidh_instData_t *instData, CsrUint16 deviceId, psm_t psm)
{
    hidh_subInstData_t *subInstData;
    CsrUint16 flushTimeout;
    l2ca_data_ch_t ch;

    subInstData = &(instData->subInstData[deviceId]);
    if( psm == CSR_BT_HID_CTRL_PSM )
    {
        ch = subInstData->ctrl_ch;
        flushTimeout = L2CA_FLUSH_TO_DEFAULT;
    }
    else
    {
        ch = subInstData->intr_ch;
        flushTimeout = subInstData->flushTimeout;
    }

    if( ch.qos != NULL)
    {
        L2CA_QOS_T *qos = (L2CA_QOS_T *)CsrPmemAlloc(sizeof(L2CA_QOS_T));
        CsrMemCpy(qos,ch.qos,sizeof(L2CA_QOS_T));
        CsrBtCml2caConnectAcceptReqSendExt( CSR_BT_HIDH_IFACEQUEUE,
                                       psm,
                                       0,
                                       instData->secIncoming,
                                       CSR_BT_HIDH_PROFILE_DEFAULT_MTU_SIZE,
                                       flushTimeout,
                                       qos,
                                       0x1124);
    }
    else
    {
        CsrBtCml2caConnectAcceptReqSend( CSR_BT_HIDH_IFACEQUEUE,
                                    psm,
                                    0,
                                    instData->secIncoming,
                                    CSR_BT_HIDH_PROFILE_DEFAULT_MTU_SIZE,
                                    0x1124);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      reconnectTimeOut
 *
 *  DESCRIPTION
 *      Handle a timeout of the awaiting of reconnect from HID device.
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
static void reconnectTimeOut( CsrUint16 deviceId, void *data)
{
    hidh_instData_t        *instData;
    hidh_subInstData_t    *subInstData;

    instData = data;

    if( deviceId < MAX_HID_CONNECTIONS )
    {
        subInstData = &instData->subInstData[deviceId];

        if( subInstData->isReconnect == TRUE)
        {
            if( subInstData->subState == AWAIT_ESTABLISH)
            {
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_CTRL_PSM);
                CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_INTR_PSM);

                CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, TIMEOUT_ESTABLISH);
            }

            subInstData->timerId = 0;
        }
    }
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for connecting with a HID device
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhConnectReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhConnectReq    *prim;
    CsrUint8 deviceId;
    hidh_subInstData_t *subInstData;

    prim = (CsrBtHidhConnectReq *) instData->recvMsgP;
    instData->doTerminate = FALSE;
    deviceId = getNewSubInst(instData);

    if((getSlotHandle(instData, prim->slotId) != CSR_SCHED_QID_INVALID) &&
       (deviceId != NOT_FOUND))
    {
        subInstData = &instData->subInstData[deviceId];

        /* Profile ready */
        subInstData->isReconnect = FALSE;
        subInstData->bdAddr = prim->deviceAddr;
        subInstData->flushTimeout = prim->flushTimeout;
        if((prim->qosCtrl != NULL) && (prim->qosCtrlCount == 1))
        {
            subInstData->ctrl_ch.qos = prim->qosCtrl;
        }
        if((prim->qosIntr != NULL) && (prim->qosIntrCount == 1))
        {
            subInstData->intr_ch.qos = prim->qosIntr;
        }
        subInstData->userHandle = getSlotHandle(instData, prim->slotId);
        instData->ctrlHandle = prim->ctrlHandle;

        if((prim->sdpInfo != NULL) && (prim->sdpInfoCount == 1))
        {
            CsrMemCpy(&subInstData->sdpInfo,
                   prim->sdpInfo,
                   sizeof(CsrBtHidhSdpInfo));
        }
        else
        {
            CsrMemSet(&subInstData->sdpInfo, 0, sizeof(CsrBtHidhSdpInfo));
        }
        CsrPmemFree(prim->sdpInfo);

        subInstData->serviceName = prim->serviceName;
        subInstData->descriptorLength = prim->descriptorLength;
        subInstData->descriptor = prim->descriptor;

        if((prim->sdpInfoCount == 1) &&
           (subInstData->serviceName != NULL) &&
           (subInstData->descriptorLength > 0) &&
           (subInstData->descriptor != NULL))
        {
            subInstData->sdpInfo.isValid = FALSE;
        }

        if( !subInstData->sdpInfo.isValid)
        {
            /* do service search first */
            CsrBtUuid32 *service = (CsrBtUuid32 *)CsrPmemAlloc(sizeof(CsrBtUuid32) * 2);
            service[0] = CSR_BT_HID_PROFILE_UUID;
            service[1] = CSR_BT_PNP_INFORMATION_UUID;

            instData->hidServiceRecordHandle = 0;
            instData->devIdServiceRecordHandle = 0;

            CsrBtCmSdcSearchReqSend(CSR_BT_HIDH_IFACEQUEUE, subInstData->bdAddr, service, 2);

            subInstData->sdpInfo.flags = 0;
            subInstData->sdpInfo.supervisionTimeout = 8000; /* 5 seconds */

            CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, SDP_SEARCH);
        }
        else
        {
            /* connect right away */
            CsrBtHidhCsrBtCml2caConnectReqSend(instData, deviceId, CSR_BT_HID_CTRL_PSM);

            CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, ESTABLISH_CTRL);
        }

        instData->connectDeviceId = deviceId;
    }
    else
    {
        CsrPmemFree(prim->qosCtrl);
        CsrPmemFree(prim->qosIntr);
        CsrPmemFree(prim->serviceName);
        CsrPmemFree(prim->descriptor);
        CsrPmemFree(prim->sdpInfo);

        CsrBtHidhConnectCfmSend(prim->ctrlHandle,
                                CSR_BT_RESULT_CODE_HIDH_MAX_NUM_OF_CONNECTIONS,
                                CSR_BT_SUPPLIER_HIDH,
                                prim->deviceAddr, NOT_FOUND, NULL,
                                CSR_BT_CONN_ID_INVALID);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhRegisterUserReqHandler
 *
 *  DESCRIPTION
 *      Register user application handle for deviceId
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhRegisterUserReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhRegisterUserReq *prim;
    CsrUint8 i;

    prim = (CsrBtHidhRegisterUserReq*)instData->recvMsgP;

    if(prim->slotId < MAX_HID_CONNECTIONS)
    {
        instData->slots[prim->slotId] = prim->userHandle;

        CsrBtHidhRegisterUserCfmSend(prim->userHandle,
                                prim->slotId,
                                CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                CSR_BT_SUPPLIER_HIDH);
    }
    else if(prim->slotId == CSR_BT_HID_ALL_DEVICE_SLOTS)
    {
        for(i=0; i<MAX_HID_CONNECTIONS; i++)
        {
            instData->slots[i] = prim->userHandle;
        }
        CsrBtHidhRegisterUserCfmSend(prim->userHandle,
                                prim->slotId,
                                CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                CSR_BT_SUPPLIER_HIDH);
    }
    else
    {
        CsrBtHidhRegisterUserCfmSend(prim->userHandle,
                                prim->slotId,
                                CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER,
                                CSR_BT_SUPPLIER_HIDH);
    }
}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhSecurityInReqHandler
 *      CsrBtHidhSecurityOutReqHandler
 *
 *  DESCRIPTION
 *      Change default incoming/outgoing security settings
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhSecurityInReqHandler(hidh_instData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtHidhSecurityInReq *prim;

    prim = (CsrBtHidhSecurityInReq*)instData->recvMsgP;

    rval = CsrBtScSetSecInLevel(&instData->secIncoming, prim->secLevel,
        CSR_BT_HIDH_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HIDH_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HIDH_SUCCESS,
        CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER);

    CsrBtHidhSecurityInCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_HIDH);
}

void CsrBtHidhSecurityOutReqHandler(hidh_instData_t *instData)
{
    CsrBtResultCode rval;
    CsrBtHidhSecurityOutReq *prim;

    prim = (CsrBtHidhSecurityOutReq*)instData->recvMsgP;

    rval = CsrBtScSetSecOutLevel(&instData->secOutgoing, prim->secLevel,
        CSR_BT_HIDH_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HIDH_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HIDH_SUCCESS,
        CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER);

    CsrBtHidhSecurityOutCfmSend(prim->appHandle, rval, CSR_BT_SUPPLIER_HIDH);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhConnectAcceptReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for awaiting a connection from a HID device
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhConnectAcceptReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhConnectAcceptReq    *prim;
    CsrUint8 deviceId;
    hidh_subInstData_t *subInstData;

    prim = (CsrBtHidhConnectAcceptReq *) instData->recvMsgP;
    instData->doTerminate = FALSE;

    deviceId = getNewSubInst(instData);

    if((getSlotHandle(instData, prim->slotId) != CSR_SCHED_QID_INVALID) &&
       (deviceId != NOT_FOUND))
    {
        subInstData = &instData->subInstData[deviceId];

        if( prim->sdpInfo != NULL)
        {
            subInstData->isReconnect = FALSE;
            subInstData->bdAddr = prim->deviceAddr;
            subInstData->flushTimeout = prim->flushTimeout;
            subInstData->ctrl_ch.qos = prim->qosCtrl;
            subInstData->intr_ch.qos = prim->qosIntr;
            subInstData->userHandle = getSlotHandle(instData, prim->slotId);

            instData->ctrlHandle = prim->ctrlHandle;

            if((prim->sdpInfo != NULL) && (prim->sdpInfoCount == 1))
            {
                CsrMemCpy(&subInstData->sdpInfo,
                       prim->sdpInfo,
                       sizeof(CsrBtHidhSdpInfo));
            }
            else
            {
                CsrMemSet(&subInstData->sdpInfo, 0, sizeof(CsrBtHidhSdpInfo));
            }
            CsrPmemFree(prim->sdpInfo);

            subInstData->serviceName = prim->serviceName;
            subInstData->descriptorLength = prim->descriptorLength;
            subInstData->descriptor = prim->descriptor;

            CsrBtHidhCsrBtCml2caConnectAcceptReqSend( instData, deviceId, CSR_BT_HID_CTRL_PSM);
            CsrBtHidhCsrBtCml2caConnectAcceptReqSend( instData, deviceId, CSR_BT_HID_INTR_PSM);
            CsrBtHidhConnectAcceptCfmSend(instData->ctrlHandle,
                                          CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                          CSR_BT_SUPPLIER_HIDH,
                                          subInstData->bdAddr,
                                          deviceId,
                                          CSR_BT_CONN_ID_INVALID);

            instData->connectDeviceId = deviceId;
            CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, AWAIT_ESTABLISH);

            return;
        }
        else
        {
            CsrBtHidhConnectAcceptCfmSend(prim->ctrlHandle,
                                          CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER,
                                          CSR_BT_SUPPLIER_HIDH,
                                          prim->deviceAddr,
                                          deviceId,
                                          CSR_BT_CONN_ID_INVALID);
        }
    }
    else
    {
        CsrBtHidhConnectAcceptCfmSend(prim->ctrlHandle,
                                      CSR_BT_RESULT_CODE_HIDH_MAX_NUM_OF_CONNECTIONS,
                                      CSR_BT_SUPPLIER_HIDH,
                                      prim->deviceAddr, deviceId,
                                      CSR_BT_CONN_ID_INVALID);
    }

    CsrPmemFree(prim->qosCtrl);
    CsrPmemFree(prim->qosIntr);
    CsrPmemFree(prim->serviceName);
    CsrPmemFree(prim->descriptor);
    CsrPmemFree(prim->sdpInfo);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCancelConnectAcceptReqHandler
 *
 *  DESCRIPTION
 *      Handle a request for cancelling awaiting a connection from a HID device
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCancelConnectAcceptReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhCancelConnectAcceptReq    *prim;

    prim = (CsrBtHidhCancelConnectAcceptReq *) instData->recvMsgP;

    if( prim->deviceId < MAX_HID_CONNECTIONS)
    {
        hidh_subInstData_t subInstData = instData->subInstData[prim->deviceId];

        if(subInstData.subState == AWAIT_ESTABLISH)
        {
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_CTRL_PSM);
            CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_HIDH_IFACEQUEUE, CSR_BT_HID_INTR_PSM);

            CSR_BT_HIDH_SUB_STATE_CHANGE(prim->deviceId, CANCEL_ESTABLISH);
        }
        else if(subInstData.subState == CONNECTED)
        {
            CsrBtHidhCancelConnectAcceptCfmSend(instData->ctrlHandle,
                                                prim->deviceId,
                                                CSR_BT_RESULT_CODE_HIDH_CONNECTION_ALREADY_EXISTS,
                                                CSR_BT_SUPPLIER_HIDH);
        }
        else
        {
            CsrBtHidhCancelConnectAcceptCfmSend(instData->ctrlHandle, prim->deviceId,
                                        CSR_BT_RESULT_CODE_HIDH_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_HIDH);
        }
    }
    else
    {
        CsrBtHidhCancelConnectAcceptCfmSend(instData->ctrlHandle, prim->deviceId,
                                CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER, CSR_BT_SUPPLIER_HIDH);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhDisconnectReqHandler
 *
 *  DESCRIPTION
 *      Handle a request disconnecting a HID device
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhDisconnectReqHandler(hidh_instData_t *instData)
{
    CsrBtHidhDisconnectReq    *prim;

    prim = (CsrBtHidhDisconnectReq *) instData->recvMsgP;

    if( prim->deviceId < MAX_HID_CONNECTIONS)
    {
        hidh_subInstData_t subInstData = instData->subInstData[prim->deviceId];

        if(subInstData.subState == CONNECTED)
        {
            CsrBtCml2caDisconnectReqSend(subInstData.intr_ch.cid);

            CSR_BT_HIDH_SUB_STATE_CHANGE(prim->deviceId, DISCONNECTING);
        }
        else
        {
            CsrBtHidhDisconnectCfmSend(instData->ctrlHandle,
                                        CSR_BT_RESULT_CODE_HIDH_NO_CONNECTION_TO_DEVICE,
                                        CSR_BT_SUPPLIER_HIDH,
                                        prim->deviceId);
        }
    }
    else
    {
        CsrBtHidhDisconnectCfmSend(instData->ctrlHandle,
                                    CSR_BT_RESULT_CODE_HIDH_UNACCEPTABLE_PARAMETER,
                                    CSR_BT_SUPPLIER_HIDH,
                                    prim->deviceId);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      cmL2caDisconnectInd
 *
 *  DESCRIPTION
 *      Handle a disconnect indication
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCmL2caDisconnectInd(hidh_instData_t *instData)
{
    CsrUint8                     deviceId;
    CsrBtCmL2caDisconnectInd    *prim;
    prim = (CsrBtCmL2caDisconnectInd *) instData->recvMsgP;

    if( (deviceId = getDeviceIdFromCid(prim->btConnId, instData)) != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_NO_ACTIVE_LOGICAL_CHANNEL, subInstData->bdAddr, prim->btConnId);

        if( subInstData->subState == DISCONNECTING )
        {
            if( subInstData->ctrl_ch.cid == prim->btConnId )
            {
                if( !subInstData->doTerminate )
                {
                    CsrBtHidhStatusIndSend(subInstData->userHandle,
                                      CSR_BT_HIDH_STATUS_DISCONNECTED,
                                      deviceId,
                                      NULL);

                    CsrBtHidhDisconnectCfmSend(instData->ctrlHandle,
                                                CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                                CSR_BT_SUPPLIER_HIDH,
                                                deviceId);

                    /* Call the disconnector extension hook */
                    if(instData->extDisconnectorFct != NULL)
                    {
                        instData->extDisconnectorFct(deviceId);
                    }
                }

                freeSubInst(instData, deviceId);
            }
            else
            {
                CsrBtCml2caDisconnectReqSend(subInstData->ctrl_ch.cid);
            }
        }
        else if( subInstData->subState == UNPLUGGING )
        {
            if( subInstData->ctrl_ch.cid == prim->btConnId )
            {
                CsrBtHidhStatusIndSend(subInstData->userHandle,
                                  CSR_BT_HIDH_STATUS_DISCONNECTED,
                                  deviceId,
                                  NULL);

                CsrBtHidhDisconnectIndSend(instData->ctrlHandle,
                                      CSR_BT_RESULT_CODE_HIDH_CONNECTION_TERM_BY_REMOTE_HOST,
                                      CSR_BT_SUPPLIER_HIDH,
                                      deviceId);

                /* Call the disconnector extension hook */
                if(instData->extDisconnectorFct != NULL)
                {
                    instData->extDisconnectorFct(deviceId);
                }
                freeSubInst(instData, deviceId);
            }
        }
        else
        {
            CsrBtConnId    otherCid;

            if( subInstData->ctrl_ch.cid == prim->btConnId )
            {
                subInstData->ctrl_ch.cid = 0;
                otherCid = subInstData->intr_ch.cid;
            }
            else
            {
                subInstData->intr_ch.cid = 0;
                otherCid = subInstData->ctrl_ch.cid;
            }

            if( otherCid == 0)
            {
                if( subInstData->doTerminate )
                {
                    freeSubInst(instData, deviceId);
                    return;
                }

                if( subInstData->sdpInfo.flags & CSR_BT_FLAGS_VIRTUAL_CABLE_BIT)
                {
                    if((subInstData->sdpInfo.flags & CSR_BT_FLAGS_RECONNECT_INIT_BIT) == FALSE)
                    {
                        /* page device */
                        CsrBtHidhCsrBtCml2caConnectReqSend( instData, deviceId, CSR_BT_HID_CTRL_PSM);

                        CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, ESTABLISH_CTRL);
                    }
                    else
                    {
                        /* enter page scan */
                        CsrBtHidhCsrBtCml2caConnectAcceptReqSend( instData, deviceId, CSR_BT_HID_CTRL_PSM);

                        CsrBtHidhCsrBtCml2caConnectAcceptReqSend( instData, deviceId, CSR_BT_HID_INTR_PSM);

                        CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, AWAIT_ESTABLISH);
                    }

                    subInstData->isReconnect = TRUE;

                    subInstData->timerId = CsrSchedTimerSet( RECONNECT_TIMEOUT,
                                                           (void (*) (CsrUint16, void *)) reconnectTimeOut,
                                                           deviceId,
                                                           (void *) instData);
                }
                else
                {
                    CsrBtHidhStatusIndSend(subInstData->userHandle,
                                      CSR_BT_HIDH_STATUS_DISCONNECTED,
                                      deviceId,
                                      NULL);

                    CsrBtHidhDisconnectIndSend(instData->ctrlHandle,
                                               CSR_BT_RESULT_CODE_HIDH_CONNECTION_TERM_BY_REMOTE_HOST,
                                               CSR_BT_SUPPLIER_HIDH,
                                               deviceId);

                    /* Call the disconnector extension hook */
                    if(instData->extDisconnectorFct != NULL)
                    {
                        instData->extDisconnectorFct(deviceId);
                    }
                    freeSubInst(instData, deviceId);
                }
            }
            else
            {
                CsrBtHidhStatusIndSend(instData->ctrlHandle,
                                  CSR_BT_HIDH_STATUS_RECONNECTING,
                                  deviceId,
                                  NULL);

                /* Call the disconnector extension hook */
                if(instData->extDisconnectorFct != NULL)
                {
                    instData->extDisconnectorFct(deviceId);
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmL2caConnectCfm
 *
 *  DESCRIPTION
 *      Handle a confirm connection establishment completed
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCmL2caConnectCfm(hidh_instData_t *instData)
{
    CsrUint8                     deviceId;
    CsrUint32             findState;
    CsrBtCmL2caConnectCfm    *prim;

    prim = (CsrBtCmL2caConnectCfm *) instData->recvMsgP;

    if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
        prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
    {
        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL, prim->deviceAddr, prim->btConnId);
    }


    if( prim->localPsm == CSR_BT_HID_CTRL_PSM)
    {
        findState = ESTABLISH_CTRL;
    }
    else
    {
        findState = ESTABLISH_INTR;
    }

    if( (deviceId = getDeviceIdFromState(findState, instData)) != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

        if( subInstData->subState == ESTABLISH_CTRL )
        {
            if(subInstData->doTerminate)
            {
                if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {
                    subInstData->ctrl_ch.cid = prim->btConnId;
                    CsrBtCml2caDisconnectReqSend(prim->btConnId);
                }
                freeSubInst(instData, deviceId);
                return;
            }

            if(prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
               prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
               prim->localPsm == CSR_BT_HID_CTRL_PSM)
            {
                subInstData->ctrl_ch.cid = prim->btConnId;
                subInstData->ctrl_ch.mtu = prim->mtu;
                subInstData->ctrl_ch.localMtu = prim->localMtu;

                CsrBtHidhCsrBtCml2caConnectReqSend( instData, deviceId, CSR_BT_HID_INTR_PSM);

                CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, ESTABLISH_INTR);
            }
            else
            {
                if( subInstData->isReconnect && (subInstData->timerId != 0) )
                {
                    /* a failed reconnect attempt, retry while not timed out (timer running) */
                    CsrBtHidhCsrBtCml2caConnectReqSend( instData, deviceId, CSR_BT_HID_CTRL_PSM);
                }
                else
                {
                    if(subInstData->isReconnect)
                    {
                        CsrBtHidhDisconnectIndSend(instData->ctrlHandle,
                                              CSR_BT_RESULT_CODE_HIDH_CONNECTION_TERM_BY_REMOTE_HOST,
                                              CSR_BT_SUPPLIER_HIDH,
                                              deviceId);
                    }
                    else
                    {
                        CsrBtHidhConnectCfmSend( instData->ctrlHandle,
                                                 prim->resultCode,
                                                 prim->resultSupplier,
                                                 subInstData->bdAddr,
                                                 deviceId,
                                                 NULL,
                                                 (CsrBtConnId)subInstData->ctrl_ch.cid);
                    }
                    freeSubInst(instData, deviceId);
                }
            }
        }
        else if(subInstData->subState == ESTABLISH_INTR )
        {
            if(subInstData->doTerminate)
            {
                if (prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
                    prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                {
                    CsrBtCml2caDisconnectReqSend(prim->btConnId);
                    subInstData->intr_ch.cid = prim->btConnId;
                    CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, DISCONNECTING);
                }
                else
                {
                    CsrBtCml2caDisconnectReqSend(subInstData->ctrl_ch.cid);
                    freeSubInst(instData, deviceId);
                }
                return;
            }

            if(prim->resultSupplier == CSR_BT_SUPPLIER_CM &&
               prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
               prim->localPsm == CSR_BT_HID_INTR_PSM)
            {
                subInstData->intr_ch.cid = prim->btConnId;
                subInstData->intr_ch.mtu = prim->mtu;
                subInstData->intr_ch.localMtu = prim->localMtu;

                /* Extended low-level interface */
                if( instData->extConnectorFct != NULL)
                {
                    instData->extConnectorFct(deviceId);
                }

                writeLinkSupervisionTimerValue( subInstData->bdAddr,
                                                subInstData->sdpInfo.supervisionTimeout);

                if( subInstData->isReconnect )
                {
                    CsrBtHidhStatusIndSend(instData->ctrlHandle,
                                      CSR_BT_HIDH_STATUS_CONNECTED,
                                      deviceId,
                                      NULL);
                }
                else
                {
                    CsrBtHidhConnectCfmSend( instData->ctrlHandle,
                                             CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                             CSR_BT_SUPPLIER_HIDH,
                                             subInstData->bdAddr,
                                             deviceId,
                                             subInstData,
                                             subInstData->ctrl_ch.cid);

                    CsrBtHidhStatusIndSend( subInstData->userHandle,
                                            CSR_BT_HIDH_STATUS_CONNECTED,
                                            deviceId,
                                            subInstData);
                }

                CsrSchedTimerCancel(subInstData->timerId, NULL, NULL);

                subInstData->isReconnect = FALSE;

                CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, CONNECTED);
            }
            else
            {
                /* the ctrl channel is already connected, remove */
                CsrBtCml2caDisconnectReqSend(subInstData->ctrl_ch.cid);

                CsrBtHidhConnectCfmSend( instData->ctrlHandle,
                                         prim->resultCode,
                                         prim->resultSupplier,
                                         subInstData->bdAddr,
                                         deviceId,
                                         subInstData,
                                         subInstData->ctrl_ch.cid);

                freeSubInst(instData, deviceId);
            }
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        else
        {
            /* state event error */
            CsrStateEventException("HIDH",
                    CSR_BT_CM_PRIM,
                    CSR_BT_CM_L2CA_CONNECT_CFM,
                    (CsrUint8) subInstData->subState);
        }
#endif
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmL2caConnectAcceptCfm
 *
 *  DESCRIPTION
 *      Handle a confirm(indication) for an arriving connection
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCmL2caConnectAcceptCfm(hidh_instData_t *instData)
{
    CsrUint8 deviceId;
    CsrBtCmL2caConnectAcceptCfm *prim;

    prim = (CsrBtCmL2caConnectAcceptCfm *) instData->recvMsgP;

    if( (deviceId = getDeviceIdFromBdAddr(prim->deviceAddr, instData)) != NOT_FOUND)
    {
        hidh_subInstData_t *subInstData = &instData->subInstData[deviceId];

        CsrBtCmLogicalChannelTypeReqSend(CSR_BT_ACTIVE_CONTROL_CHANNEL,
                                             prim->deviceAddr, prim->btConnId);

        if( subInstData->subState == AWAIT_ESTABLISH )
        {
            if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                prim->resultSupplier == CSR_BT_SUPPLIER_CM)
            {
                if( prim->localPsm == CSR_BT_HID_CTRL_PSM)
                {
                    if( subInstData->doTerminate )
                    {
                        CsrBtCml2caDisconnectReqSend(prim->btConnId);

                        freeSubInst(instData, deviceId);
                        return;
                    }

                    subInstData->ctrl_ch.cid = prim->btConnId;
                    subInstData->ctrl_ch.mtu = prim->mtu;
                    subInstData->ctrl_ch.localMtu = prim->localMtu;
                }
                else
                {
                    CsrSchedTimerCancel(subInstData->timerId, NULL, NULL);

                    if( subInstData->doTerminate )
                    {
                        CsrBtCml2caDisconnectReqSend(prim->btConnId);
                        CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, DISCONNECTING);
                        return;
                    }

                    subInstData->intr_ch.cid = prim->btConnId;
                    subInstData->intr_ch.mtu = prim->mtu;
                    subInstData->intr_ch.localMtu = prim->localMtu;

                    /* Extended low-level interface */
                    if( instData->extConnectorFct != NULL)
                    {
                        instData->extConnectorFct(deviceId);
                    }

                    writeLinkSupervisionTimerValue( subInstData->bdAddr,
                                                    subInstData->sdpInfo.supervisionTimeout);

                    if( subInstData->isReconnect )
                    {
                        CsrBtHidhStatusIndSend(instData->ctrlHandle,
                                          CSR_BT_HIDH_STATUS_CONNECTED,
                                          deviceId,
                                          NULL);

                        subInstData->isReconnect = FALSE;
                    }
                    else
                    {
                        CsrBtHidhConnectAcceptIndSend(instData->ctrlHandle,
                                                 CSR_BT_RESULT_CODE_HIDH_SUCCESS,
                                                 CSR_BT_SUPPLIER_HIDH,
                                                 deviceId);

                        CsrBtHidhStatusIndSend( subInstData->userHandle,
                                           CSR_BT_HIDH_STATUS_CONNECTED,
                                           deviceId,
                                           subInstData);
                    }
                    CSR_BT_HIDH_SUB_STATE_CHANGE(deviceId, CONNECTED);
                }
            }
            else
            {
                /* failed connect accept response */
                if( prim->localPsm == CSR_BT_HID_CTRL_PSM)
                {
                    if( subInstData->isReconnect )
                    {
                        CsrSchedTimerCancel(subInstData->timerId, NULL, NULL);

                        if( !subInstData->doTerminate )
                        {
                            CsrBtHidhStatusIndSend(subInstData->userHandle,
                                              CSR_BT_HIDH_STATUS_DISCONNECTED,
                                              deviceId,
                                              NULL);

                            CsrBtHidhDisconnectIndSend(instData->ctrlHandle,
                                                  CSR_BT_RESULT_CODE_HIDH_CONNECTION_TERM_BY_REMOTE_HOST,
                                                  CSR_BT_SUPPLIER_HIDH,
                                                  deviceId);

                            /* Call the disconnector extension hook */
                            if(instData->extDisconnectorFct != NULL)
                            {
                                instData->extDisconnectorFct(deviceId);
                            }
                        }

                        freeSubInst(instData, deviceId);
                    }
                    else
                    {
                        CsrBtHidhConnectAcceptIndSend(instData->ctrlHandle,
                                                      prim->resultCode,
                                                      prim->resultSupplier,
                                                      deviceId);
                    }
                }

                freeSubInst(instData, deviceId);
            }
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        else
        {
            /* state event error */
            CsrStateEventException("HIDH",
                    CSR_BT_CM_PRIM,
                    CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM,
                    (CsrUint8) subInstData->subState);
        }
#endif
    }
    else
    {
        /* reject connection */
        CsrBtCml2caDisconnectReqSend(prim->btConnId);

        if( (deviceId = getDeviceIdFromState(AWAIT_ESTABLISH, instData)) != NOT_FOUND)
        {
            CsrBtHidhCsrBtCml2caConnectAcceptReqSend( instData, deviceId, prim->localPsm );
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHidhCmL2caCancelConnectAcceptCfm
 *
 *  DESCRIPTION
 *      Handle a confirm for cancelled await for arriving connection
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtHidhCmL2caCancelConnectAcceptCfm(hidh_instData_t *instData)
{
    CsrBtCmL2caCancelConnectAcceptCfm    *prim;

    prim = (CsrBtCmL2caCancelConnectAcceptCfm *) instData->recvMsgP;

    if( prim->localPsm == CSR_BT_HID_CTRL_PSM )
    {
        CsrUint8                 i;
        hidh_subInstData_t    *subInstData;

        /* not a bullet-proof solution for finding the correct device that was cancelled, but.... */
        for(i=0; i<MAX_HID_CONNECTIONS; i++)
        {
            subInstData = &instData->subInstData[i];

            if( subInstData->subState == CANCEL_ESTABLISH)
            {
                if( !subInstData->doTerminate )
                {
                    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        CsrBtHidhCancelConnectAcceptCfmSend(instData->ctrlHandle, i, CSR_BT_RESULT_CODE_HIDH_SUCCESS, CSR_BT_SUPPLIER_HIDH);
                    }
                    else
                    {
                        CsrBtHidhCancelConnectAcceptCfmSend(instData->ctrlHandle, i, prim->resultCode, prim->resultSupplier);
                    }
                }

                if (prim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    /* a connection is arriving, go back and wait for it */
                    CSR_BT_HIDH_SUB_STATE_CHANGE(i, AWAIT_ESTABLISH);
                }
                else
                {
                    /* either a success or a mystery failure, same conclusion: free device instance */
                    freeSubInst(instData, i);
                }
                break;
            }
            else if( subInstData->subState == TIMEOUT_ESTABLISH)
            {
                if( subInstData->isReconnect )
                {
                    if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {
                        if( !subInstData->doTerminate )
                        {
                            CsrBtHidhStatusIndSend( subInstData->userHandle,
                                               CSR_BT_HIDH_STATUS_DISCONNECTED,
                                               i,
                                               subInstData);

                            CsrBtHidhDisconnectIndSend(instData->ctrlHandle,
                                                  CSR_BT_RESULT_CODE_HIDH_CONNECTION_TERM_BY_REMOTE_HOST,
                                                  CSR_BT_SUPPLIER_HIDH,
                                                  i);

                            /* Call the disconnector extension hook */
                            if(instData->extDisconnectorFct != NULL)
                            {
                                instData->extDisconnectorFct(i);
                            }
                        }

                        freeSubInst(instData, i);
                        break;
                    }
                    /* else failed to cancel at lower layers, possibly a connection arriving: NOP */
                }
            }
        }
    }
}

void CsrBtHidhCmL2caModeChangeInd(hidh_instData_t *instData)
{
    CsrBtCmL2caModeChangeInd *cmPrim;
    cmPrim = (CsrBtCmL2caModeChangeInd *) instData->recvMsgP;

    /* Call the low-level extension hook for mode changes */
    if(instData->extModeChangeFct != NULL)
    {
        instData->extModeChangeFct(getDeviceIdFromCid(cmPrim->btConnId, instData),
                                   cmPrim->mode);
    }

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    /* If no mode-change hook, just make sure we never enter park */
    else if(cmPrim->mode == CSR_BT_PARK_MODE)
    {
        CsrBtCml2caModeChangeReqSend(cmPrim->btConnId, CSR_BT_ACTIVE_MODE);
    }
#endif
}


