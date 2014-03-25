/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_bccmd.h"
#include "csr_bt_sc_private_lib.h"

#define WRITE_SCAN_ENABLE_MAX_RETRY 10

#ifdef CSR_BT_INSTALL_CM_READ_COD
void CsrBtCmReadCodReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadCodReq  *prim;

    prim = (CsrBtCmReadCodReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = prim->appHandle;
    dm_hci_read_class_of_device(NULL);
}

void CsrBtCmDmHciReadClassOfDeviceCompleteHandler(cmInstanceData_t *cmData)
{ /* Read class of device complete */
    DM_HCI_READ_CLASS_OF_DEVICE_CFM_T * dmPrim = (DM_HCI_READ_CLASS_OF_DEVICE_CFM_T *) cmData->recvMsgP;
    CsrBtCmReadCodCfm                 * cmPrim = (CsrBtCmReadCodCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadCodCfm));

    cmPrim->type            = CSR_BT_CM_READ_COD_CFM;
    cmPrim->classOfDevice   = dmPrim->dev_class;

    if(dmPrim->status == HCI_SUCCESS)
    {
        cmPrim->resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        cmPrim->resultCode      = (CsrBtResultCode) dmPrim->status;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_HCI;
    }
    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}
#endif
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
static void csrBtCmAcceptConnectWithTimeOut(CsrUint8 theIndex, cmInstanceData_t *cmData)
{
    cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromIndex, &(theIndex));

    if (theElement)
    {
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;
        theLogicalLink->timerFlag         = FALSE;

        if(cmData->rfcVar.rfcTimeOutIgnoreCounter > 0)
        {
            cmData->rfcVar.unhandleTimeOut = TRUE;
            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, timeOut);
        }
        else
        { /* Remove the COD. Check if the Host still need to be connectable and
             send CSR_BT_CM_CONNECT_ACCEPT_CFM with timeout to the application                */
            CsrBtCmAcceptConnectTimeout  *prim;

            prim = (CsrBtCmAcceptConnectTimeout *) CsrPmemAlloc(sizeof(CsrBtCmAcceptConnectTimeout));

            prim->type                = CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT;
            prim->serverChannel        = theLogicalLink->serverChannel;
            CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, cancelTimer_s);
            cmData->recvMsgP        = prim;
            CsrBtCmServiceManagerProvider(cmData);
            CsrPmemFree(cmData->recvMsgP);
        }
    }
    else
    {
        CsrBtCmGeneralException(CSR_BT_CM_PRIM, CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT, "Did not find any cmRfcConnElement");;
    }
}

#define CSR_BT_MAX_TIME                      (2000000000)

static void csrBtCmStartSuperVisionTimer(cmInstanceData_t *cmData, cmRfcConnInstType * theLink, uuid16_t theTime, CsrUint8 theIndex)
{/* Start supervision timer  and initialise the instance variables,
    if not set to 0, which is infinit */
    if(theTime != CSR_BT_INFINITE_TIME)
    {
        if(theTime > (CSR_BT_MAX_TIME / CSR_BT_MICROSEC2SEC))
        {
            theTime = (CSR_BT_MAX_TIME / CSR_BT_MICROSEC2SEC);
        }
        theLink->timerId   = CsrSchedTimerSet(theTime*CSR_BT_MICROSEC2SEC, (void (*) (CsrUint16, void *)) csrBtCmAcceptConnectWithTimeOut, (CsrUint16) theIndex, (void *) cmData);
        theLink->timerFlag = TRUE;
    }
}

static void csrBtCmConnectableHandler(cmInstanceData_t    *cmData,
                                      cmRfcConnElement    *theElement,
                                      CsrUint8             index,
                                      CsrBtResultCode     resultCode,
                                      CsrBtSupplier resultSupplier)
{
    if(resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    { /* The hostcontroller is connectable */
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;

        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, idle_s);
        csrBtCmStartSuperVisionTimer(cmData, theLogicalLink, cmData->rfcVar.connectAcceptTimeOut, index);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
    else
    { /* The hostcontroller could not enter scan mode. Build and send
         CSR_BT_CM_CONNECT_ACCEPT_CFM to the application. Restore the RFC
         the queue */
        CsrBtCmConnectAcceptCfmMsgSend(cmData, theElement, resultCode, resultSupplier);
    }
}

static void csrBtCmWriteScanEnableRfcCompleteHandler(cmInstanceData_t    *cmData,
                                                     cmRfcConnElement    *connElement,
                                                     CsrBtResultCode     resultCode,
                                                     CsrBtSupplier resultSupplier)
{
    if (connElement && connElement->cmRfcConnInst)
    {
        cmRfcConnInstType *theLogicalLink = connElement->cmRfcConnInst;

        switch(theLogicalLink->state)
        {
            case connectAble_s :
                { /* The CM has just make a application connectable. Start supervision
                     timer and restore RFC queue */
                    csrBtCmConnectableHandler(cmData, connElement, connElement->elementId, resultCode, resultSupplier);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case cancelConnectAble_s :
                { /* A connectAble service has been cancel. Restore the RFC queue */
                    if(theLogicalLink->timerFlag)
                    {/* Reset the timer */
                        CsrSchedTimerCancel(theLogicalLink->timerId, NULL, NULL);
                        theLogicalLink->timerFlag = FALSE;
                    }

                    CsrBtScDeregisterReqSend(SEC_PROTOCOL_RFCOMM, theLogicalLink->serverChannel);
                    CsrBtCmDmSyncClearPcmSlotFromTable(cmData, &theLogicalLink->eScoParms);
                    cleanUpConnectionTable(&(connElement->cmRfcConnInst));
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case cancelTimer_s :
                { /* A connectAble service has timeout. Build and send
                     CSR_BT_CM_CONNECT_ACCEPT_CFM to the application. Restore the RFC queue. */
                    CsrBtCmConnectAcceptCfmMsgSend(cmData, connElement, CSR_BT_RESULT_CODE_CM_TIMEOUT, CSR_BT_SUPPLIER_CM);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case connectAccept_s :
            case connectAcceptFinal_s :
                {
                    if(cmData->smVar.resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                       cmData->smVar.resultSupplier == CSR_BT_SUPPLIER_CM)
                    { /* A new RFCOMM connection have just been establish with SUCCESS.
                         Write lp settings and inform the application */
                        CsrUint8       numOfConnection;

                        numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, theLogicalLink->deviceAddr);

                        if(numOfConnection == 0)
                        { /* There is no other connection attach to the device address.
                             Ensure that SNIFF and PARK mode is supported */
                            CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, theLogicalLink->deviceAddr, KEEP_CURRENT_PLAYER);
                        }
                        else
                        { /* Currently there is at least one more connection attach to this
                             device address. The rfc connection has been accepted with with success.
                             Inform the application, and restore the local service manager
                             and DM queue */
                            theLogicalLink->actualMode    = CsrBtCmDmReturnActualMode(cmData, theLogicalLink->deviceAddr);
                            /*cmData->rfcVar.btConnId        = CSR_BT_CONN_ID_INVALID;*/
                            CsrBtCmConnectAcceptCfmMsgSend(cmData, connElement, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                            CsrBtCmDmLocalQueueHandler();

                            if(theLogicalLink->actualMode != CSR_BT_ACTIVE_MODE)
                            {
                                aclTable *aclConnectionElement;

                                returnAclConnectionElement(cmData, theLogicalLink->deviceAddr, &aclConnectionElement);

                                if (aclConnectionElement)
                                {
                                    if (!aclConnectionElement->appControlsLowPower)
                                    {
                                        CsrBtCmRfcModeChangeReqSend(theLogicalLink->btConnId, CSR_BT_ACTIVE_MODE);
                                    }
                                    else
                                    { /* The application is controlling low power. Inform the profile
                                         about current mode                                             */
                                        CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink, aclConnectionElement->interval,
                                                                       aclConnectionElement->mode, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                                    }
                                }
                                else
                                { /* just ignore */
                                    ;
                                }
                            }
                        }
                    }
                    else
                    { /* A new RFCOMM connection have just been establish with FAIL.
                         Restore the RFC queue */
                        cmData->rfcVar.btConnId        = CSR_BT_CONN_ID_INVALID;
                        CsrBtCmConnectAcceptCfmMsgSend(cmData, connElement, cmData->smVar.resultCode, cmData->smVar.resultSupplier);
                        CsrBtCmDmLocalQueueHandler();
                    }
                    break;
                }
            case timeOut:
                {
                    CsrBtCmConnectAcceptCfmMsgSend(cmData, connElement, CSR_BT_RESULT_CODE_CM_TIMEOUT, CSR_BT_SUPPLIER_CM);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            default    :
                {
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
        }
    }
    else
    {
        if (connElement)
        { /* If connElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}

void CsrBtCmRfcCancelConnectAcceptReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmCancelAcceptConnectReq * cmPrim = (CsrBtCmCancelAcceptConnectReq *) cmData->recvMsgP;
    cmConnIdServerType  connIdServ       = CsrBtCmReturnConnIdServerStruct(CSR_BT_CONN_ID_CANCELLED, cmPrim->serverChannel);
    cmRfcConnElement * theElement  = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromConnIdServer, &(connIdServ));

    if(theElement)
    { /* The device is connectable. Remove COD and cancel the service */
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;
        cmData->rfcVar.connElement          = theElement;
        theLogicalLink->btConnId            = CSR_BT_CONN_ID_INVALID;
        CSR_BT_CM_STATE_CHANGE(theLogicalLink->state, cancelConnectAble_s);
        CsrBtCmWriteClassOfDevice(RFC_PLAYER);
    }
    else
    { /* The CM can not cancel the service.Restore the RFC queue. */
        
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

static void csrBtCmWriteScanEnableL2capCompleteHandler(cmInstanceData_t *cmData,
                                                       cmL2caConnElement *connElement,
                                                       CsrBtResultCode resultCode,
                                                       CsrBtSupplier resultSupplier)
{
    if (connElement && connElement->cmL2caConnInst)
    {
        cmL2caConnInstType *l2capLink = connElement->cmL2caConnInst;

        switch(l2capLink->state)
        {
            case l2capConnectAble_s :
                { /* The CM has just try to make a application connectable.
                     Restore the local DM queue */

                    if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
                    {/* The device is now connectable. Restore the local service manager
                        queue */
                        CSR_BT_CM_STATE_CHANGE(l2capLink->state, idle_s);
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                    else
                    { /* The attempt to make the device connectable fails. Inform the
                         application, restore the local l2cap queue, and clean up
                         the l2cap connection table */
                        CsrBtCmL2caConnectAcceptCfmHandler(cmData, connElement, resultCode, resultSupplier);
                    }
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case l2capConnectAcceptFinal_s :
                {/* The l2cap connection is now establish with with success. Inform the
                    application, and restore the  local service manager and DM queue */
                    CsrUint8       numOfConnection;

                    numOfConnection        = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, l2capLink->deviceAddr);
                    if(numOfConnection == 0)
                    { /* There is no other connection attach to the device address.
                         Ensure that SNIFF and PARK mode is supported */
                        CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, l2capLink->deviceAddr, KEEP_CURRENT_PLAYER);
                    }
                    else
                    { /* Currently there is at least one more connection attach to the
                         device address. The l2cap connection has been accepted with with success.
                         Inform the application, and restore the local service manager and
                         DM queue */

                        l2capLink->actualMode    = CsrBtCmDmReturnActualMode(cmData, l2capLink->deviceAddr);
                        CSR_BT_CM_STATE_CHANGE(l2capLink->state, l2capConnected_s);

                        CsrBtCmL2caConnectAcceptCfmHandler(cmData, connElement,
                                                           CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

                        if(l2capLink->actualMode != CSR_BT_ACTIVE_MODE)
                        {
                            aclTable *aclConnectionElement;

                            returnAclConnectionElement(cmData, l2capLink->deviceAddr, &aclConnectionElement);

                            if (aclConnectionElement)
                            {
                                if (!aclConnectionElement->appControlsLowPower)
                                {
                                    CsrBtCml2caModeChangeReqSend(l2capLink->btConnId, CSR_BT_ACTIVE_MODE);
                                }
                                else
                                { /* The application is controlling low power. Inform the profile
                                     about current mode                                             */
                                    CsrBtCmL2caModeChangeIndMsgSend(l2capLink,
                                                                    aclConnectionElement->interval, aclConnectionElement->mode,
                                                                    CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                                }
                            }
                            else
                            { /* Just ignore    */
                                ;
                            }
                        }
                    }
                    break;
                }
            case l2capSaveDisconnect_s :
                {
                    CsrBtCmL2caConnectAcceptCfmHandler(cmData, connElement,
                                                       CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                    break;
                }
            case l2capCancelConnectAble_s :
                { /* The connect accept service is cancel. Send CM_L2CA_CANCEL_ACCEPT_CONNECT_CFM
                     to the application and restore the local service manager and the local DM
                     queue. */
                    CsrUint8    numOfOutgoing;
                    CsrUint8    numOfIncoming;

                    numberOfSecurityRegister(cmData, l2capLink->psm, l2capLink->deviceAddr, &numOfOutgoing, &numOfIncoming);

                    /* Restore the local service manager queue. A confirm has already been sent to application */
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    
                    if (numOfIncoming == 1)
                    { /* Unregister The Incoming security setting */
                        CsrBtScDeregisterReqSend(SEC_PROTOCOL_L2CAP, l2capLink->psm);
                    }
                    else
                    { /* There is more that need this security setting */
                        ;
                    }
                    CsrBtCmL2capClearL2capTableIndex(cmData, &(connElement->cmL2caConnInst));
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            default :
                {
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
        }
    }
    else
    {
        if (connElement)
        { /* If connElement == NULL then this function has allready been called */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

static void csrBtCmWriteScanEnableBnepCompleteHandler(cmInstanceData_t *cmData, bnepTable * bnepLink,
                                                      CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    switch(bnepLink->state)
    {
        case bnepConnectAccept_s :
            { /* The CM has just try to make a application connectable.
                 Restore the local DM queue */
                CsrBtCmBnepConnectAcceptCfmMsgSend(cmData, cmData->bnepVar.appHandle, resultCode, resultSupplier);

                if (resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    CSR_BT_CM_STATE_CHANGE(bnepLink->state, idle_s);
                }
                else
                {
                    CsrBtCmBnepClearBnepTableIndex(bnepLink);
                }
                cmData->bnepVar.connectTypeInProgress = NO_CONNECT;
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case bnepConnectAcceptFinal_s:
            {
                CsrBtCmBnepCancelConnectAcceptCfmMsgSend(cmData->bnepVar.appHandle,
                                                         CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case bnepCancelConnectAccept_s :
            { /* The connect accept service is cancel. Send CM_BNEP_CANCEL_ACCEPT_CONNECT_CFM
                 to the application and restore the local service manager and the local DM
                 queue. */
                CsrBtCmBnepCancelConnectAcceptCfmMsgSend(cmData->bnepVar.appHandle,
                                                         CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                CsrBtCmBnepClearBnepTableIndex(bnepLink);
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
        default :
            {
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmLocalQueueHandler();
                break;
            }
    }
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

static void csrBtCmWriteScanEnableCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmWriteScanEnableCfm *msg = (CsrBtCmWriteScanEnableCfm *)CsrPmemAlloc(sizeof(CsrBtCmWriteScanEnableCfm));
    msg->type                      = CSR_BT_CM_WRITE_SCAN_ENABLE_CFM;
    msg->resultCode                = resultCode;
    msg->resultSupplier            = resultSupplier;
    CsrBtCmPutMessage(appHandle, msg);
}

static void csrBtCmWriteScanEnableCmCompleteHandler(cmInstanceData_t *cmData,
                                                    CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    if(resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && resultSupplier == CSR_BT_SUPPLIER_CM)
    {
        if (cmData->globalState == notReady_s)
        {/* Build and send an RFC_INIT_REQ primitive to RFCOMM */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
            RfcInitReqSend(CSR_BT_CM_IFACEQUEUE);
#else
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            dm_sync_register_req(CSR_BT_CM_IFACEQUEUE, 0);
#else
            CsrBtCmDmSyncRegisterCfmHandler(cmData);
#endif
#endif
        }
        else
        {
            csrBtCmWriteScanEnableCfmMsgSend(cmData->dmVar.appHandle, resultCode, resultSupplier);
            /* Restore the DM queue */
            CsrBtCmDmLocalQueueHandler();
        }
    }
    else
    {
        if ((cmData->globalState == notReady_s) || (cmData->dmVar.retryCounter++ < WRITE_SCAN_ENABLE_MAX_RETRY))
        {
            /* Fail try again */
            CsrUint8 mode = returnConnectAbleParameters(cmData);
            dm_hci_write_scan_enable(mode, NULL);
        }
        else
        {
            csrBtCmWriteScanEnableCfmMsgSend(cmData->dmVar.appHandle, resultCode, resultSupplier);
            /* Restore the DM queue */
            CsrBtCmDmLocalQueueHandler();
        }
    }
}


void CsrBtCmWriteScanEnableCompleteSwitch(cmInstanceData_t *cmData, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    switch(cmData->smVar.activePlayer)
    {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        case RFC_PLAYER:
            {
                csrBtCmWriteScanEnableRfcCompleteHandler(cmData, cmData->rfcVar.connElement, resultCode, resultSupplier);
                break;
            }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
        case L2CAP_PLAYER:
            {
                csrBtCmWriteScanEnableL2capCompleteHandler(cmData, cmData->l2caVar.connElement, resultCode, resultSupplier);
                break;
            }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        case BNEP_PLAYER:
            {
                csrBtCmWriteScanEnableBnepCompleteHandler(cmData, cmData->bnepVar.indexPtr, resultCode, resultSupplier);
                break;
            }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE*/

        case CM_PLAYER:
            {
                csrBtCmWriteScanEnableCmCompleteHandler(cmData, resultCode, resultSupplier);
                break;
            }
        default :
            {
                break;
            }
    }

}

void CsrBtCmDmHciWriteScanEnableCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_WRITE_SCAN_ENABLE_CFM_T    *dmPrim;

    dmPrim = (DM_HCI_WRITE_SCAN_ENABLE_CFM_T *)cmData->recvMsgP;

    if(dmPrim->status == HCI_SUCCESS)
    {
        cmData->dmVar.currentChipScanMode = cmData->dmVar.pendingChipScanMode;
    }

#ifdef CSR_BT_INSTALL_CM_DUT_MODE
    if (cmData->dmVar.deviceUnderTest)
    {
        if (dmPrim->status == HCI_SUCCESS)
        {
            dm_hci_enable_device_ut_mode(NULL);
        }
        else
        {
            CsrBtCmSendDeviceUnderTestComplete(cmData->dmVar.appHandle, dmPrim->status, 2);
        }
    }
    else
#endif
    {
        if (dmPrim->status == HCI_SUCCESS)
        {
            CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
        else
        {
            CsrBtCmWriteScanEnableCompleteSwitch(cmData, (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
        }
    }
}

void CsrBtCmWriteClassOfDeviceReqHandler(cmInstanceData_t *cmData)
{ /* Find the actual Class Of Device and send the command to the hostcontroller */
    CsrUint24    classOfDevice;
    CsrUint8        mode;
    CsrBtCmWriteClassOfDeviceReq    *cmPrim;
    cmPrim = (CsrBtCmWriteClassOfDeviceReq *) cmData->recvMsgP;

    cmData->smVar.activePlayer = cmPrim->activePlayer;
    classOfDevice = CsrBtCmReturnClassOfdevice(cmData);
    mode          = returnConnectAbleParameters(cmData);

    if (classOfDevice != cmData->dmVar.codWrittenToChip)
    {
        cmData->dmVar.pendingCod = classOfDevice;
        dm_hci_write_class_of_device(classOfDevice, NULL);
    }
    else if (mode != cmData->dmVar.currentChipScanMode)
    {
        cmData->dmVar.pendingChipScanMode = mode;
        dm_hci_write_scan_enable(mode, NULL);
    }
    else
    {
        CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
    }
}

#ifdef CSR_BT_INSTALL_CM_WRITE_COD
static void csrBtCmWriteClassOfDeviceCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmWriteCodCfm *msg = (CsrBtCmWriteCodCfm *)CsrPmemAlloc(sizeof(CsrBtCmWriteCodCfm));
    msg->type               = CSR_BT_CM_WRITE_COD_CFM;
    msg->resultCode         = resultCode;
    msg->resultSupplier     = resultSupplier;
    CsrBtCmPutMessage(appHandle, msg);
}

void CsrBtCmWriteCodReqHandler(cmInstanceData_t *cmData)
{ /* Find the actual Class Of Device and send the command to the hostcontroller */
    CsrUint24            classOfDevice;
    CsrBtCmWriteCodReq  *cmPrim;

    cmPrim = (CsrBtCmWriteCodReq*) cmData->recvMsgP;

    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.writingCod, CM_COD_IN_PROGRESS);
    cmData->dmVar.appHandle = cmPrim->appHandle;

    if (cmPrim->updateFlags & CSR_BT_CM_WRITE_COD_UPDATE_FLAG_MAJOR_MINOR_CLASS)
    {
        cmData->dmVar.majorCod = cmPrim->majorClassOfDevice;
        cmData->dmVar.minorCod = cmPrim->minorClassOfDevice;
    }
    if (cmPrim->updateFlags & CSR_BT_CM_WRITE_COD_UPDATE_FLAG_SERVICE_CLASS)
    {
        cmData->dmVar.serviceCod = cmPrim->serviceClassOfDevice;
    }

    classOfDevice = CsrBtCmReturnClassOfdevice(cmData);

    if (classOfDevice != cmData->dmVar.codWrittenToChip)
    {
        cmData->dmVar.pendingCod = classOfDevice;
        dm_hci_write_class_of_device(classOfDevice, NULL);
    }
    else
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.writingCod, CM_COD_NONE);
        csrBtCmWriteClassOfDeviceCfmMsgSend(cmData->dmVar.appHandle, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        CsrBtCmDmLocalQueueHandler();
    }
}
#endif
void CsrBtCmWriteClassOfDevice(CsrUint8 thePlayer)
{ /* Build the intern CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ primitive and send it to itself.
     Hereby it will be control by the CsrBtCmDmProvider */
    CsrBtCmWriteClassOfDeviceReq  *prim;

    prim = (CsrBtCmWriteClassOfDeviceReq *) CsrPmemAlloc(sizeof(CsrBtCmWriteClassOfDeviceReq));
    prim->type            = CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ;
    prim->activePlayer    = thePlayer;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmDmHciWriteClassOfDeviceCompleteHandler(cmInstanceData_t *cmData)
{ /* Write class of device complete */

#if defined(CSR_BT_INSTALL_CM_WRITE_COD) || defined(CSR_BT_INSTALL_CM_PRI_IAC)
    CsrBtResultCode                    resultCode;
    CsrBtSupplier                resultSupplier;
    DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *dmPrim = (DM_HCI_WRITE_CLASS_OF_DEVICE_CFM_T *) cmData->recvMsgP;

    if(dmPrim->status == HCI_SUCCESS)
    {
        cmData->dmVar.codWrittenToChip = cmData->dmVar.pendingCod;
        resultCode                     = CSR_BT_RESULT_CODE_CM_SUCCESS;
        resultSupplier                 = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        resultCode                     = (CsrBtResultCode) dmPrim->status;
        resultSupplier                 = CSR_BT_SUPPLIER_HCI;
    }
#endif
#ifdef CSR_BT_INSTALL_CM_WRITE_COD
    if (cmData->dmVar.writingCod == CM_COD_IN_PROGRESS)
    {
        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.writingCod, CM_COD_NONE);
        csrBtCmWriteClassOfDeviceCfmMsgSend(cmData->dmVar.appHandle, resultCode, resultSupplier);
        CsrBtCmDmLocalQueueHandler();
    }
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
    else if (cmData->dmVar.pendingIac != 0)
    {
        CsrBtCmFinishWriteIac(cmData, resultCode, resultSupplier);
    }
#endif /* CSR_BT_INSTALL_CM_PRI_IAC */
    else
#else /* CSR_BT_INSTALL_CM_WRITE_COD */       
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
    if (cmData->dmVar.pendingIac != 0)
    {
        CsrBtCmFinishWriteIac(cmData, resultCode, resultSupplier);
    }
    else
#endif /* CSR_BT_INSTALL_CM_PRI_IAC */
#endif /* CSR_BT_INSTALL_CM_WRITE_COD */
    {
        CsrUint8 mode = returnConnectAbleParameters(cmData);

        if (mode != cmData->dmVar.currentChipScanMode)
        {
            cmData->dmVar.pendingChipScanMode = mode;
            dm_hci_write_scan_enable(mode, NULL);
        }
        else
        {
            CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
        }
    }
}

#ifdef CSR_BT_INSTALL_CM_READ_SCAN_EANBLE
void CsrBtCmDmHciReadScanEnableCompleteHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadScanEnableCfm * cmPrim = (CsrBtCmReadScanEnableCfm *) CsrPmemAlloc(sizeof(CsrBtCmReadScanEnableCfm));
    DM_HCI_READ_SCAN_ENABLE_CFM_T * dmPrim = (DM_HCI_READ_SCAN_ENABLE_CFM_T *) cmData->recvMsgP;

    cmPrim->type       = CSR_BT_CM_READ_SCAN_ENABLE_CFM;
    cmPrim->scanEnable = dmPrim->scan_enable;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmPrim->resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        cmPrim->resultCode      = (CsrBtResultCode) dmPrim->status;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_HCI;
    }

    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}
#endif
void CsrBtCmWriteScanEnableReqHandler(cmInstanceData_t *cmData)
{
    CsrUint24                    classOfDevice;
    CsrUint8                        mode;
    CsrBtCmWriteScanEnableReq    * cmPrim;

    cmPrim     = (CsrBtCmWriteScanEnableReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle             = cmPrim->appHandle;
    cmData->dmVar.disableInquiryScan    = cmPrim->disableInquiryScan;
    cmData->dmVar.disablePageScan       = cmPrim->disablePageScan;
    cmData->dmVar.retryCounter          = 0;
    cmData->smVar.activePlayer          = CM_PLAYER;
    classOfDevice                       = CsrBtCmReturnClassOfdevice(cmData);

    mode = returnConnectAbleParameters(cmData);
    if (classOfDevice != cmData->dmVar.codWrittenToChip)
    {
        cmData->dmVar.pendingCod = classOfDevice;
        dm_hci_write_class_of_device(classOfDevice, NULL);
    }
    else if (mode != cmData->dmVar.currentChipScanMode)
    {
        cmData->dmVar.pendingChipScanMode = mode;
        dm_hci_write_scan_enable(mode, NULL);
    }
    else
    {
        CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
    }
}

#ifdef CSR_BT_INSTALL_CM_READ_SCAN_EANBLE
void CsrBtCmReadScanEnableReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadScanEnableReq    * cmPrim;

    cmPrim     = (CsrBtCmReadScanEnableReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle     = cmPrim->appHandle;
    dm_hci_read_scan_enable(NULL);
}
#endif
#ifdef CSR_BT_INSTALL_CM_CONNECTABLE
void CsrBtCmConnectAbleReqHandler(cmInstanceData_t *cmData)
{
    CsrUint24                classOfDevice;
    CsrUint8                        mode;
    CsrBtCmConnectableReq    * cmPrim;

    cmPrim     = (CsrBtCmConnectableReq *) cmData->recvMsgP;

    if (cmPrim->connectAble)
    {
        cmData->dmVar.connectAblePhandle = cmPrim->appHandle;
    }
    else
    {
        cmData->dmVar.connectAblePhandle = CSR_SCHED_QID_INVALID;
    }

    cmData->smVar.activePlayer   = CM_PLAYER;
    classOfDevice                = CsrBtCmReturnClassOfdevice(cmData);

    mode = returnConnectAbleParameters(cmData);
    if (classOfDevice != cmData->dmVar.codWrittenToChip)
    {
        cmData->dmVar.pendingCod = classOfDevice;
        dm_hci_write_class_of_device(classOfDevice, NULL);
    }
    else if (mode != cmData->dmVar.currentChipScanMode)
    {
        cmData->dmVar.pendingChipScanMode = mode;
        dm_hci_write_scan_enable(mode, NULL);
    }
    else
    {
        CsrBtCmWriteScanEnableCompleteSwitch(cmData, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
    }
}
#endif

