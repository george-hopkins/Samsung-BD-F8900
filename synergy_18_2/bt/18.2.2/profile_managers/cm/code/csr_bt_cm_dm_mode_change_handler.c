/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_sdc.h"

static CsrBtResultCode csrBtReturnResultCode(CsrUint8 newMode)
{
    if (newMode == CSR_BT_ACTIVE_MODE)
    { /* Because the profile expect that their always can enter
         ACTIVE_MODE the CM must tell them that their can
         consider them that this request has been overruled
         because the application have taking the control or because
         all connection is running amp */
        return (CSR_BT_RESULT_CODE_CM_MODE_CHANGE_REQUEST_OVERRULED);
    }
    else
    { /* The application is not allowed to change the current link
         policy right now, or is already in this mode. Inform the
         profile */
        return (CSR_BT_RESULT_CODE_CM_SUCCESS);
    }
}

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmRfcModeChangeIndMsgSend(cmRfcConnInstType * theLogicalLink, CsrUint16 length,
                                    CsrUint8 mode, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Build and send CSR_BT_CM_RFC_MODE_CHANGE_IND to the application */
    CsrBtCmRfcModeChangeInd    *cmPrim;

    cmPrim = (CsrBtCmRfcModeChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmRfcModeChangeInd));

    cmPrim->type            = CSR_BT_CM_RFC_MODE_CHANGE_IND;
    cmPrim->length          = length;
    cmPrim->mode            = mode;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->btConnId        = theLogicalLink->btConnId;
    cmPrim->context         = theLogicalLink->context;
    CsrBtCmPutMessage(theLogicalLink->appHandle, cmPrim);
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmL2caModeChangeIndMsgSend(cmL2caConnInstType *l2CaConnection, CsrUint16 length,
                                     CsrUint8 mode, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Build and send CSR_BT_CM_L2CA_MODE_CHANGE_IND to the application */
    CsrBtCmL2caModeChangeInd    *cmPrim;

    cmPrim = (CsrBtCmL2caModeChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmL2caModeChangeInd));

    cmPrim->type            = CSR_BT_CM_L2CA_MODE_CHANGE_IND;
    cmPrim->btConnId        = l2CaConnection->btConnId;
    cmPrim->length          = length;
    cmPrim->mode            = mode;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    cmPrim->context         = l2CaConnection->context;
    CsrBtCmPutMessage(l2CaConnection->appHandle, cmPrim);
}
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
static void csrBtCmBnepModeChangeIndMsgSend(CsrSchedQid theAppHandle, bnepTable *bnepConnection, CsrUint16 length,
                                            CsrUint8 mode, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Build and send CSR_BT_CM_L2CA_MODE_CHANGE_IND to the application */
    CsrBtCmBnepModeChangeInd    *cmPrim;

    cmPrim = (CsrBtCmBnepModeChangeInd *)CsrPmemAlloc(sizeof(CsrBtCmBnepModeChangeInd));

    cmPrim->type            = CSR_BT_CM_BNEP_MODE_CHANGE_IND;
    cmPrim->id              = bnepConnection->id;
    cmPrim->length          = length;
    cmPrim->mode            = mode;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(theAppHandle, cmPrim);
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_BT_INSTALL_CM_LOW_POWER_CONFIG_PUBLIC
static void csrBtCmModeChangeConfigCfmMsgSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
                                              CsrBtSupplier resultSupplier, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmModeChangeConfigCfm    *cmPrim;

    cmPrim = (CsrBtCmModeChangeConfigCfm *)CsrPmemAlloc(sizeof(CsrBtCmModeChangeConfigCfm));

    cmPrim->type            = CSR_BT_CM_MODE_CHANGE_CONFIG_CFM;
    cmPrim->deviceAddr      = deviceAddr;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}
#endif

static void csrBtCmModeChangeCfmMsgSend(cmInstanceData_t *cmData, CsrUint16 interval, CsrUint8 mode,
                                        CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    CsrBtCmModeChangeCfm    *cmPrim;

    cmPrim = (CsrBtCmModeChangeCfm *)CsrPmemAlloc(sizeof(CsrBtCmModeChangeCfm));

    cmPrim->type            = CSR_BT_CM_MODE_CHANGE_CFM;
    cmPrim->deviceAddr      = cmData->smVar.operatingBdAddr;
    cmPrim->interval        = interval;
    cmPrim->mode            = mode;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(cmData->smVar.appHandle, cmPrim);

    CsrBtCmDmLocalQueueHandler();
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

static CsrBool returnInitStateIndexToThisAddress(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr,
                                                 CsrBool linkLost, CsrUint8 *activePlayer,
                                                 CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{/* Goes through the rfcomm and l2cap connections tables to see if the CM is
    in a process where it establish a releasing a connection, or the
    application has just change the link policy to active */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8    i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *l2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *rfcElement = CsrBtCmRfcFindRfcConnElementFromDeviceAddrState1OrState2(cmData, &deviceAddr, connectInit_s, releaseInit_s);

    if (rfcElement)
    {
        rfcElement->cmRfcConnInst->actualMode = CSR_BT_ACTIVE_MODE;
        *activePlayer                         = RFC_PLAYER;
        return (TRUE);
    }

#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (l2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); l2caElem; l2caElem = l2caElem->next)
    { /* Search through the l2ca list */
        if (l2caElem->cmL2caConnInst)
        {
            cmL2caConnInstType *l2CaConnection = l2caElem->cmL2caConnInst;

            if(bd_addr_eq(&(deviceAddr), &(l2CaConnection->deviceAddr)))
            {
                if((l2CaConnection->state == l2capConnectInit_s) ||
                   (l2CaConnection->state == l2capReleaseInit_s) ||
                   (l2CaConnection->state == l2capReleaseInitXover_s))
                {
                    if (linkLost && l2CaConnection->state == l2capReleaseInit_s)
                    {
                        l2CaConnection->state = l2capReleaseInitXover_s;
                        cmData->smVar.resultCode        = resultCode;
                        cmData->smVar.resultSupplier    = resultSupplier;
                    }
                    else
                    {
                        ;
                    }
                    l2CaConnection->actualMode  = CSR_BT_ACTIVE_MODE;
                    *activePlayer               = L2CAP_PLAYER;
                    return (TRUE);
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */
        if(bd_addr_eq(&(deviceAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
        {
            if((cmData->bnepVar.connectVar[i].state == bnepConnectInit_s) ||
               (cmData->bnepVar.connectVar[i].state == bnepDisconnectInit_s))
            {
                cmData->bnepVar.connectVar[i].actualMode    = CSR_BT_ACTIVE_MODE;
                *activePlayer                               = BNEP_PLAYER;
                return (TRUE);
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    if (cmData->sdcVar.sdcSearchList)
    {
        if (cmData->sdcVar.state == sdcSearchInit_s && bd_addr_eq(&(deviceAddr), &(cmData->sdcVar.currentElement->deviceAddr)))
        {
            *activePlayer  = SDC_PLAYER;
            return (TRUE);
        }
    }
    return FALSE;
}
static void nextSwitchRoleHandler(cmInstanceData_t *cmData)
{
    CsrUint8 numOfAcl = returnNumOfAclConnection(cmData);

    if (numOfAcl > 1)
    {
        aclTable   * nextAclConnectionElement;

        returnNextAclConnectionElement(cmData, &nextAclConnectionElement);

        if (nextAclConnectionElement)
        {
            if (nextAclConnectionElement->role == CSR_BT_MASTER_ROLE)
            {/* The role is right just make sure that MSS is not supported */
                CsrBtCmDmWriteLpSettingsOnlyLowPowerMsgSend(cmData, nextAclConnectionElement->deviceAddr, CM_PLAYER);
                cmData->roleVar.roleSwitchFlag        = FALSE;
                nextAclConnectionElement->roleChecked = TRUE;
            }
            else
            { /* Need to change make a role switch */
                CsrBtCmDmModeChangeRoleSwitchHandler(cmData, nextAclConnectionElement->deviceAddr);
            }
        }
        else
        { /* The process is finish restore the queues */
            CsrBtCmDmLocalQueueHandler();
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    {/* The process is finish restore the queues */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void requestConnectOrDoneStateHandler(cmInstanceData_t *cmData, CsrUint8 theMode, CsrBtDeviceAddr theAddr, CsrBool linkLost,
                                             CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if(theMode == CSR_BT_ACTIVE_MODE || linkLost)
    { /* The CM is setting up or releasing a new RFCOMM/L2CAP/BNEP connection */
        CsrUint8 activePlayer;

        if(returnInitStateIndexToThisAddress(cmData, theAddr, linkLost, &activePlayer, resultCode, resultSupplier))
        { /* Write lp settings    because the CM is in a process where it
             establish or a releasing a connection */
            if (activePlayer == SDC_PLAYER)
            {
                CsrBtCmSdcStartHandler(cmData, theAddr);
                CsrBtCmDmLocalQueueHandler();
            }
            else
            {
                CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, theAddr, activePlayer);
            }
        }
        else
        { /* The application has just change the link policy to active mode */
            CsrBtCmDmLocalQueueHandler();
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Has just change the link policy to CSR_BT_SNIFF_MODE or CSR_BT_PARK_MODE */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void writeLinkPolicyStateHandler(cmInstanceData_t *cmData, CsrUint8 theMode, CsrBtDeviceAddr theAddr)
{
    if(theMode == CSR_BT_ACTIVE_MODE)
    { /* The application has requested a link policy change */
        CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, theAddr, CM_LINK_POLICY_PLAYER);
    }
    else
    { /* Has just change the link policy to CSR_BT_SNIFF_MODE or CSR_BT_PARK_MODE */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void switchRoleStateHandler(cmInstanceData_t *cmData, CsrUint8 theMode, CsrBtDeviceAddr theAddr)
{
    if(theMode == CSR_BT_ACTIVE_MODE)
    { /* The CM has force the link in ACTIVE mode in order to
         be able to perform the role switch */
        cmData->roleVar.roleSwitchFlag                  = TRUE;
        CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, theAddr, CM_PLAYER, ENABLE_MS_SWITCH);
    }
    else
    { /* Has just change the link policy back to CSR_BT_SNIFF_MODE or CSR_BT_PARK_MODE.
         Found out if more ACL connection need to make a role switch */
        cmData->dmVar.roleSwitchLinkMode                = CSR_BT_ACTIVE_MODE;
        nextSwitchRoleHandler(cmData);
    }
}

static void csrBtCmDmInitRfcScoStateHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr)
{/* The CM has force the link in ACTIVE mode in order to be able to perform
    the role switch */
    cmData->smVar.activePlayer            = RFC_PLAYER;
    cmData->dmVar.roleSwitchInProgress    = TRUE;
    cmData->dmVar.switchRoleDeviceAddr    = theAddr;
    dm_hci_switch_role(&theAddr, CSR_BT_MASTER_ROLE, NULL);
}

static void csrBtCmDmHciModeChangeEventLinkPolicyStateHandler(cmInstanceData_t *cmData, CsrUint8 theMode,
                                                              CsrBtDeviceAddr theAddr, CsrUint16 theLength,
                                                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    switch (cmData->dmVar.linkPolicyState)
    {
        case LINK_POLICY_CONNECT_OR_DONE_STATE:
            {
                requestConnectOrDoneStateHandler(cmData, theMode,
                                                 theAddr, FALSE, resultCode, resultSupplier);
                break;
            }
        case LINK_POLICY_WRITE_LINK_POLICY_STATE:
            {
                writeLinkPolicyStateHandler(cmData, theMode, theAddr);
                break;
            }
        case LINK_POLICY_ROLE_SWITCH_STATE:
            {
                switchRoleStateHandler(cmData, theMode, theAddr);
                break;
            }
        case LINK_POLICY_INIT_RFC_SCO_STATE:
            {
                csrBtCmDmInitRfcScoStateHandler(cmData, theAddr);
                break;
            }
        case LINK_POLICY_APP_CONTROL_STATE:
            { /* The application request of changing the Low power mode
                 is fullfilled. Inform the application */
                csrBtCmModeChangeCfmMsgSend(cmData, theLength, theMode,
                                            CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                break;
            }
#ifdef CSR_AMP_ENABLE
        case LINK_POLICY_AMP_CONTROL_STATE:
            {
                if (theMode == CSR_BT_SNIFF_MODE)
                { /* The ACL link has been force into Sniff mode by AMP */
                    aclTable * aclConnectionElement;
                    returnAclConnectionElement(cmData, theAddr, &aclConnectionElement);

                    if (aclConnectionElement)
                    {
                        if (CsrBtCmBtControllerActive(cmData, theAddr) == CTRL_ACTIVE_AMP_ONLY)
                        {
                            aclConnectionElement->sniffForceByAmp = TRUE;
                        }
                    }
                }
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
#endif /* CSR_AMP_ENABLE */
        default:
            { /* This must never happen */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
    }
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_IDLE_STATE);
}

static void informInvolvedApplicationsWithError(cmInstanceData_t                *cmData,
                                                DM_HCI_MODE_CHANGE_EVENT_IND_T  *dmPrim,
                                                CsrUint8                         requestedMode,
                                                CsrBtResultCode                 resultCode,
                                                CsrBtSupplier             resultSupplier)
{ /* It has not possible to change the current link policy. Inform all
     involved applications */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;

    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
    { /* Search through the RFC connection table */
        if (currentRfcElem->cmRfcConnInst)
        {
            if(currentRfcElem->cmRfcConnInst->state == connected_s)
            { /* The application has a RFCOMM connection */
                if(bd_addr_eq(&(dmPrim->bd_addr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
                {/* The given device address is right. */
                    if(currentRfcElem->cmRfcConnInst->requestedMode == requestedMode)
                    { /* This application is involved. Build and send
                         CSR_BT_CM_RFC_MODE_CHANGE_IND to it */
                        cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;

                        CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink,
                                                       dmPrim->length, dmPrim->mode, resultCode, resultSupplier);

                        theLogicalLink->actualMode = dmPrim->mode;
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem = currentL2caElem->next)
    { /* Search through the L2CAP connection list */
        if (currentL2caElem->cmL2caConnInst)
        {
            if(currentL2caElem->cmL2caConnInst->state == l2capConnected_s)
            { /* The application has a L2CAP connection */
                if(bd_addr_eq(&(dmPrim->bd_addr), &(currentL2caElem->cmL2caConnInst->deviceAddr)))
                { /* The given device address is right. */
                    if(currentL2caElem->cmL2caConnInst->requestedMode == requestedMode)
                    { /* This application is involved. Build and send
                         CSR_BT_CM_L2CA_MODE_CHANGE_IND to it */
                        cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

                        CsrBtCmL2caModeChangeIndMsgSend(l2CaConnection, dmPrim->length,
                                                        dmPrim->mode, resultCode, resultSupplier);
                        l2CaConnection->actualMode = dmPrim->mode;
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */
        if(cmData->bnepVar.connectVar[i].state == bnepConnected_s)
        { /* The application has a BNEP connection */
            if(bd_addr_eq(&(dmPrim->bd_addr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* The given device address is right. */
                if(cmData->bnepVar.connectVar[i].requestedMode == requestedMode)
                { /* This application is involved. Build and send
                     CSR_BT_CM_BNEP_MODE_CHANGE_IND to it */
                    bnepTable *bnepConnection;

                    bnepConnection    = &(cmData->bnepVar.connectVar[i]);
                    csrBtCmBnepModeChangeIndMsgSend(cmData->bnepVar.appHandle,
                                                    bnepConnection, dmPrim->length, dmPrim->mode, resultCode, resultSupplier);
                    cmData->bnepVar.connectVar[i].actualMode = dmPrim->mode;
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
}

static void csrBtCmDmModeChangeEventErrorLinkPolicyHandler(cmInstanceData_t *cmData,
                                                           CsrBool linkLost,
                                                           DM_HCI_MODE_CHANGE_EVENT_IND_T *dmPrim,
                                                           CsrBtResultCode resultCode,
                                                           CsrBtSupplier resultSupplier)
{ /* The ACL link has been closed or the remote device did not support the
     requested link policy. Find out why the link policy were changed and
     inform the involved applications */
    switch (cmData->dmVar.linkPolicyState)
    {
        case LINK_POLICY_CONNECT_OR_DONE_STATE:
            { /* The Cm must check if it were initiating or releasing a RFC,
                 L2CAP or BENEP connection, or it just had try to change
                 the link mode */
                requestConnectOrDoneStateHandler(cmData, dmPrim->mode, dmPrim->bd_addr, linkLost, resultCode, resultSupplier);
                break;
            }
        case LINK_POLICY_ROLE_SWITCH_STATE:
            { /* Found out if another ACL connection need to make a role switch */
                nextSwitchRoleHandler(cmData);
                break;
            }
        case LINK_POLICY_INIT_RFC_SCO_STATE:
            { /* The CM tries to exit sniff mode, because an application has
                 requested it to initiate a SCO connection. The reason for
                 exit sniff in the first place is that CM wants to be MASTER
                 before making the SCO connection. If the mode is different from
                 sniff we must consider the ACL as closed. Just restore the DM
                 queue to prevent a deadlock, because the DM queue is taking by
                 the CSR_BT_CM_SCO_CONNECT_REQ
                 message. No need to send a confimation message to the
                 application as it must have or will very soon received a
                 CSR_BT_CM_L2CA_DISCONNECT_IND or a CSR_BT_CM_DISCONNECT_IND. */
                CsrBtCmDmLocalQueueHandler();
                break;
            }
        case LINK_POLICY_APP_CONTROL_STATE:
            { /* The CM tries to exit sniff mode, because an application has
                 requested to goto ACTIVE mode or re-enter sniff mode with
                 different parameter. Inform the application of the result */
                csrBtCmModeChangeCfmMsgSend(cmData, dmPrim->length, dmPrim->mode, resultCode, resultSupplier);
                break;
            }
#ifdef CSR_AMP_ENABLE
        case LINK_POLICY_AMP_CONTROL_STATE:
            { /* The CM tries to change the mode because of AMP*/
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
#endif /* CSR_AMP_ENABLE */
        default:
            { /* cmData->dmVar.linkPolicyState must be
                 LINK_POLICY_WRITE_LINK_POLICY_STATE. E.g. the application has
                 requested a CSR_BT_CM_WRITE_LINK_POLICY_REQ. If the remote device
                 don't support sniff or park, or the ACL connection is closed
                 just restore the SM and the DM queue to prevent deadlock.
                 There is confimation message on a CSR_BT_CM_WRITE_LINK_POLICY_REQ */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
    }
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_IDLE_STATE);

    if (!linkLost)
    { /* Tell the applications the mode change procedure fails */
        CsrUint8 requestedMode = CsrBtCmDmReturnRequestedMode(cmData, dmPrim->bd_addr);

        informInvolvedApplicationsWithError(cmData, dmPrim, requestedMode, resultCode, resultSupplier);
    }
    else
    { /* No need to tell anything because the ACL link is lost and the
         applications will or have receive a disconnect message */
        ;
    }
}

static void csrBtCmDmModeChangeEventErrorHandler(cmInstanceData_t *cmData, DM_HCI_MODE_CHANGE_EVENT_IND_T *dmPrim,
                                                 CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* The request of changing the link policy fail. Find the reason and react on
     it */
    switch (dmPrim->status)
    {
        case HCI_ERROR_NO_CONNECTION:
        case HCI_ERROR_CONN_TIMEOUT:
        case HCI_ERROR_OETC_USER:
        case HCI_ERROR_OETC_LOW_RESOURCE:
        case HCI_ERROR_OETC_POWERING_OFF:
        case HCI_ERROR_CONN_TERM_LOCAL_HOST:
            { /* The ACL link has been closed */
                csrBtCmDmModeChangeEventErrorLinkPolicyHandler(cmData, TRUE, dmPrim, resultCode, resultSupplier);
                break;
            }
        default:
            {
                switch(dmPrim->mode)
                { /* Find the reason */
                    case CSR_BT_SNIFF_MODE :
                        { /* Fail then trying to exit Sniff mode, just try again */
                            dm_hci_exit_sniff_mode(&(dmPrim->bd_addr), NULL);
                            break;
                        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    case CSR_BT_PARK_MODE :
                        { /* Fail then trying to exit park mode, just try again */
                            dm_hci_exit_park_mode(&(dmPrim->bd_addr), NULL);
                            break;
                        }
#endif
                    default :
                        { /* The remote device did not support the
                             requested link policy. Find out why the link policy were
                             changed and inform the involved applications. */
                            csrBtCmDmModeChangeEventErrorLinkPolicyHandler(cmData, FALSE, dmPrim, resultCode, resultSupplier);
                            break;
                        }
                }
                break;
            }
    }
}

static void informInvolvedApplications(cmInstanceData_t *cmData,
                                       CsrBtDeviceAddr deviceAddr, CsrUint8 mode, CsrUint16 length)
{ /* Inform all involved applications about the current link policy */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
    CsrBool       enterSsr = CsrBtCmReturnNumOfConnectionsToPeerDevice(cmData, deviceAddr) > 0 &&
        mode == CSR_BT_SNIFF_MODE && cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1
        ? TRUE : FALSE;

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;

    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
    { /* Search through the RFC connection table */
        if (currentRfcElem->cmRfcConnInst)
        {
            if(currentRfcElem->cmRfcConnInst->state == connected_s ||
               currentRfcElem->cmRfcConnInst->state == releaseInit_s)
            { /* The application has a RFCOMM connection */
                if(bd_addr_eq(&(deviceAddr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
                { /* The given device address is right. Build and send
                     CSR_BT_CM_RFC_MODE_CHANGE_IND to the application with the current
                     link policy. */
                    cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;
                    theLogicalLink->actualMode          = mode;

                    if (theLogicalLink->state == connected_s)
                    {
                        CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink, length, mode,
                                                       CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                    }

                    if(mode == CSR_BT_SNIFF_MODE && theLogicalLink->state == connected_s)
                    {
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
                        if(theLogicalLink->ssrSettings.maxRemoteLatency  != CM_SSR_DISABLED &&
                           theLogicalLink->ssrAccepted == TRUE)
                        {
                            /* do nothing - at the end of this function SSR will be activated if no */
                            /* other connections exist which can not accept SSR */
                        }
                        else
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
                        {
                            /* connection which does not want SSR exist */
                            enterSsr = FALSE;
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem = currentL2caElem->next)
    { /* Search through the L2CAP connection list */
        if (currentL2caElem->cmL2caConnInst)
        {
            cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

            if(l2CaConnection->state == l2capConnected_s ||
               l2CaConnection->state == l2capReleaseInit_s)
            {/* The application has a valid L2CAP connection */
                if(bd_addr_eq(&(deviceAddr), &(l2CaConnection->deviceAddr)))
                { /* The given device address is right. Build and send
                     CSR_BT_CM_L2CA_MODE_CHANGE_IND to the application with the current
                     link policy. */
                    l2CaConnection->actualMode = mode;

                    if (l2CaConnection->state == l2capConnected_s)
                    {
                        CsrBtCmL2caModeChangeIndMsgSend(l2CaConnection, length, mode,
                                                        CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

                        if(mode == CSR_BT_SNIFF_MODE)
                        {
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
                            if(l2CaConnection->ssrSettings.maxRemoteLatency != CM_SSR_DISABLED &&
                               l2CaConnection->ssrAccepted == TRUE)
                            {
                                /* do nothing - at the end of this function SSR will be activated if no */
                                /* other connections exist which can not accept SSR */
                            }
                            else
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
                            {
                                /* connection which does not want SSR exist */
                                enterSsr = FALSE;
                            }
                        }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */

        if( cmData->bnepVar.connectVar[i].state == bnepConnected_s ||
            cmData->bnepVar.connectVar[i].state == bnepDisconnectInit_s)
        {
            if(bd_addr_eq(&(deviceAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* Thie given address is right. Store the actual link policy in the connection table */
                bnepTable *bnepConnection;

                bnepConnection    = &(cmData->bnepVar.connectVar[i]);
                cmData->bnepVar.connectVar[i].actualMode = mode;

                if( cmData->bnepVar.connectVar[i].state == bnepConnected_s)
                { /* Inform the application with the current link policy */
                    csrBtCmBnepModeChangeIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                    length, mode, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                }

                if(mode == CSR_BT_SNIFF_MODE && cmData->bnepVar.connectVar[i].state == bnepConnected_s)
                {
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1

                    if(bnepConnection->ssrSettings.maxRemoteLatency != CM_SSR_DISABLED &&
                       bnepConnection->ssrAccepted == TRUE)
                    {
                        /* do nothing - at the end of this function SSR will be activated if no */
                        /* other connections exist which can not accept SSR */
                    }
                    else
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
                    {
                        /* connection which does not want SSR exist */
                        enterSsr = FALSE;
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1
    if (enterSsr == TRUE)
    {
        aclTable *aclConnectionElement;

        returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

        if (aclConnectionElement && aclConnectionElement->appControlsLowPower == TRUE)
        { /* The Application is controlling low power, no need to check sniff
             sub rating parameters */
            ;
        }
        else
        {
            CsrBtCmDmCheckSsrReqSend(deviceAddr);
        }
    }
#endif /* CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1 */
}

static CsrBool csrBtCmModeChangeAllowed(aclTable   *aclConnectionElement)
{
    if (!aclConnectionElement->appControlsLowPower 
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        || aclConnectionElement->mode == CSR_BT_PARK_MODE
#endif
        )
    {
        if (aclConnectionElement->sniffForceByAmp &&
            aclConnectionElement->mode == CSR_BT_SNIFF_MODE)
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
        return (FALSE);
    }
}

void CsrBtCmDmStoreActualMode(cmInstanceData_t *cmData)
{ /* Store the actual mode identify by the given device address */
    DM_HCI_MODE_CHANGE_EVENT_IND_T *dmPrim;
    aclTable *aclConnectionElement;

    dmPrim    = (DM_HCI_MODE_CHANGE_EVENT_IND_T *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        aclConnectionElement->mode     = dmPrim->mode;
        aclConnectionElement->interval = dmPrim->length;

        if (dmPrim->status == HCI_SUCCESS)
        {
            aclConnectionElement->sniffForceByAmp = FALSE;
        }
    }
}

CsrUint8 CsrBtCmDmReturnActualMode(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr)
{ /* Return the actual mode identify by the given device address */
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8    i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *l2caElement;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *rfcElement = CsrBtCmRfcFindRfcConnElementFromDeviceAddrState(cmData, &theAddr, connected_s);

    if (rfcElement)
    {
        return (rfcElement->cmRfcConnInst->actualMode);
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    l2caElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromConnectedSDeviceAddr, &theAddr);

    if (l2caElement)
    {
        return (l2caElement->cmL2caConnInst->actualMode);
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for ( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */
        if( bd_addr_eq( &(theAddr), &(cmData->bnepVar.connectVar[i].deviceAddr) ) )
        {
            if( (cmData->bnepVar.connectVar[i].state == bnepConnected_s))
            {
                return cmData->bnepVar.connectVar[i].actualMode;
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_BT_LE_ENABLE
    {
        aclTable   *aclConnectionElement; 
        
        returnAclConnectionElement(cmData, theAddr, &aclConnectionElement);
        /* Nothing but GATT-over-BR/EDR is using this link. we should be in sniff mode */
        if (aclConnectionElement
            && (aclConnectionElement->gattConnectionActive))
        {
            return aclConnectionElement->mode;
        }
    }
#endif
    return CSR_BT_ACTIVE_MODE;
}

static CsrBool csrBtCmRequestedModeSupported(aclTable *aclConnectionElement, CsrUint8 requestedMode)
{
    switch (requestedMode)
    {
        case CSR_BT_ACTIVE_MODE:
            {
                return TRUE;
            }
        case CSR_BT_SNIFF_MODE:
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case CSR_BT_PARK_MODE:
#endif
            {
                if (aclConnectionElement)
                {
                    if (requestedMode == CSR_BT_SNIFF_MODE)
                    {
                        if ((aclConnectionElement->linkPolicySettings & ENABLE_SNIFF) == ENABLE_SNIFF)
                        {
                            return TRUE;
                        }
                    }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    else
                    {
                        if ((aclConnectionElement->linkPolicySettings & ENABLE_PARK) == ENABLE_PARK)
                        {
                            return TRUE;
                        }
                    }
#endif
                }
                else
                {
                    return TRUE;
                }
            }
        default:
            {
                /* Fall through */
            }
    }

    return FALSE;
}

static void csrBtCmModeChangeUnSupportedFeatureHandler(cmInstanceData_t *cmData, link_policy_settings_t linkPolicySettings, CsrBtDeviceAddr theAddr, CsrUint8 actualMode)
{
    if (actualMode == CSR_BT_SNIFF_MODE && ((linkPolicySettings & ENABLE_SNIFF) != ENABLE_SNIFF))
    {
        CsrBtCmDmHciModeChangeReqMsgSend(cmData, theAddr, CSR_BT_ACTIVE_MODE, actualMode);
    }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    else if (actualMode == CSR_BT_PARK_MODE && ((linkPolicySettings  & ENABLE_PARK) != ENABLE_PARK))
    {
        CsrBtCmDmHciModeChangeReqMsgSend(cmData, theAddr, CSR_BT_ACTIVE_MODE, actualMode);
    }
#endif
    else
    {
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void csrBtCmDmSdcModeChangeMsgSend(cmInstanceData_t *cmData)
{
    if (cmData->sdcVar.sdcSearchList && cmData->sdcVar.state == sdcSearchInit_s)
    {
        CsrBtDeviceAddr deviceAddr    = cmData->sdcVar.currentElement->deviceAddr;
        CsrUint8         actualMode = CsrBtCmDmReturnActualMode(cmData, deviceAddr);
        if(actualMode == CSR_BT_ACTIVE_MODE)
        {
            CsrBtCmSdcStartHandler(cmData, deviceAddr);
            CsrBtCmDmLocalQueueHandler();
        }
        else
        {
            cmData->smVar.operatingBdAddr        = deviceAddr;
            CsrBtCmDmHciModeChangeReqMsgSend(cmData, deviceAddr, CSR_BT_ACTIVE_MODE, actualMode);
        }
    }
    else
    {
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void csrBtCmDmLinkPolicyPlayerModeChangeMsgSend(cmInstanceData_t *cmData, CsrUint8 theIndex)
{
    aclTable           *aclConnectionElement;

    aclConnectionElement = &cmData->roleVar.aclVar[theIndex];

    if (aclConnectionElement && !CsrBtBdAddrEqZero(&aclConnectionElement->deviceAddr))
    {
        CsrUint8 actualMode = CsrBtCmDmReturnActualMode(cmData, aclConnectionElement->deviceAddr);

        if (actualMode == CSR_BT_SNIFF_MODE && ((aclConnectionElement->linkPolicySettings & ENABLE_SNIFF) != ENABLE_SNIFF))
        {
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_WRITE_LINK_POLICY_STATE);
            cmData->smVar.operatingBdAddr   = aclConnectionElement->deviceAddr;
            cmData->dmVar.requestedLinkMode = CSR_BT_ACTIVE_MODE;
            dm_hci_exit_sniff_mode(&(aclConnectionElement->deviceAddr), NULL);
        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        else if (actualMode == CSR_BT_PARK_MODE && ((aclConnectionElement->linkPolicySettings & ENABLE_PARK) != ENABLE_PARK))
        {
            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_WRITE_LINK_POLICY_STATE);
            cmData->smVar.operatingBdAddr   = aclConnectionElement->deviceAddr;
            cmData->dmVar.requestedLinkMode = CSR_BT_ACTIVE_MODE;
            dm_hci_exit_park_mode(&(aclConnectionElement->deviceAddr), NULL);
        }
#endif
        else
        {
            CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, aclConnectionElement->deviceAddr, CM_LINK_POLICY_PLAYER);
        }
    }
    else
    {
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

static void csrBtCmHciSniffReqSend(cmInstanceData_t   *cmData,
                                   CsrBtSniffSettings sniffSettings,
                                   CsrBtDeviceAddr    deviceAddr)
{
    aclTable *aclConnectionElement;

    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement && sniffSettings.max_interval > aclConnectionElement->lsto)
    { /* The Sniff_Max_Interval shall be less than the Link Supervision Timeout */
        sniffSettings.max_interval = (CsrUint16) (aclConnectionElement->lsto - CM_HCI_SNIFF_DRAWBACK);
    }

    if (sniffSettings.max_interval > CM_HCI_MAX_SNIFF_INTERVAL)
    {
        sniffSettings.max_interval = CM_HCI_MAX_SNIFF_INTERVAL;
    }

    if (sniffSettings.min_interval > sniffSettings.max_interval)
    {
        sniffSettings.min_interval = (CsrUint16) (sniffSettings.max_interval - CM_HCI_SNIFF_DRAWBACK);
    }


    if (sniffSettings.min_interval < CM_HCI_MIN_SNIFF_INTERVAL)
    {
        sniffSettings.min_interval = CM_HCI_MIN_SNIFF_INTERVAL;
    }


    if (sniffSettings.attempt > CM_HCI_MAX_SNIFF_ATTEMPT)
    {
        sniffSettings.attempt = CM_HCI_MAX_SNIFF_ATTEMPT;
    }
    else if (sniffSettings.attempt < CM_HCI_MIN_SNIFF_ATTEMPT)
    {
        sniffSettings.attempt = CM_HCI_MIN_SNIFF_ATTEMPT;
    }

    if (sniffSettings.timeout > CM_HCI_MAX_SNIFF_TIMEOUT)
    {
        sniffSettings.timeout = CM_HCI_MAX_SNIFF_TIMEOUT;
    }

    /* Note only even values are valid */
    sniffSettings.max_interval = (sniffSettings.max_interval & CM_HCI_MAX_SNIFF_INTERVAL);
    sniffSettings.min_interval = (sniffSettings.min_interval & CM_HCI_MAX_SNIFF_INTERVAL);

    dm_hci_sniff_mode(&(deviceAddr),
                      sniffSettings.max_interval,
                      sniffSettings.min_interval,
                      sniffSettings.attempt,
                      sniffSettings.timeout, NULL);
}


static void csrBtCmDmAppPlayerModeChangeMsgSend(cmInstanceData_t *cmData, CsrUint8 requestedMode, CsrBtDeviceAddr deviceAddr)
{
    aclTable            *aclConnectionElement;

    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    { /* The ACL connection represented by the given device address is connected */
        if (requestedMode == CSR_BT_SNIFF_MODE || requestedMode == CSR_BT_ACTIVE_MODE)
        {
            switch (aclConnectionElement->mode)
            {
                case CSR_BT_ACTIVE_MODE:
                    {
                        if (requestedMode == CSR_BT_SNIFF_MODE)
                        { /* Try to go to SNIFF mode */
                            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_APP_CONTROL_STATE);
                            cmData->dmVar.requestedLinkMode = requestedMode;
                            cmData->smVar.operatingBdAddr   = deviceAddr;
                            csrBtCmHciSniffReqSend(cmData, cmData->dmVar.requestedSniffSetting, deviceAddr);
                        }
                        else
                        { /* The ACL connection is already in active mode */
                            csrBtCmModeChangeCfmMsgSend(cmData,  aclConnectionElement->interval,
                                                        CSR_BT_ACTIVE_MODE, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                        }
                        break;
                    }
                case CSR_BT_SNIFF_MODE:
                    {
                        if (requestedMode == CSR_BT_SNIFF_MODE && !cmData->dmVar.forceSniffSettings)
                        { /* The mode is the same as the requested mode and the
                             application don't want to force new sniff setting.
                             Inform the application */
                            csrBtCmModeChangeCfmMsgSend(cmData,  aclConnectionElement->interval,
                                                        CSR_BT_SNIFF_MODE, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                        }
                        else
                        { /* Either the application has requested SNIFF mode with new
                             setting that must be force or it has requested ACTIVE mode.*/
                            CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_APP_CONTROL_STATE);
                            cmData->dmVar.requestedLinkMode = requestedMode;
                            cmData->smVar.operatingBdAddr   = deviceAddr;
                            dm_hci_exit_sniff_mode(&(deviceAddr), NULL);
                        }
                        break;
                    }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                case CSR_BT_PARK_MODE:
                    { /* Must exit park in order to get the link into the requested mode,
                         which must either be ACTIVE or SNIFF mode */
                        CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_APP_CONTROL_STATE);
                        cmData->dmVar.requestedLinkMode  = requestedMode;
                        cmData->smVar.operatingBdAddr   = deviceAddr;
                        dm_hci_exit_park_mode(&(deviceAddr), NULL);
                        break;
                    }
#endif
                default:
                    {
                        csrBtCmModeChangeCfmMsgSend(cmData, aclConnectionElement->interval, aclConnectionElement->mode,
                                                    CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR, CSR_BT_SUPPLIER_CM);
                    }
            }
        }
        else
        { /* The mode requested by the application is invalid */
            csrBtCmModeChangeCfmMsgSend(cmData, aclConnectionElement->interval, aclConnectionElement->mode,
                                        CSR_BT_RESULT_CODE_CM_UNACCEPTABLE_PARAMETER, CSR_BT_SUPPLIER_CM);
        }
    }
    else
    { /* No ACL connection, return Error */
        csrBtCmModeChangeCfmMsgSend(cmData, 0, CSR_BT_ACTIVE_MODE,
                                    CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER, CSR_BT_SUPPLIER_CM);
    }
}

CsrUint8 CsrBtCmDmReturnRequestedMode(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr)
{ /* Return the link policy for the given device address */
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    aclTable   *aclConnectionElement;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    CsrUint8    newMode = CSR_BT_PARK_MODE;
#else
    CsrUint8    newMode = CSR_BT_SNIFF_MODE;
#endif

    returnAclConnectionElement(cmData, theAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        if (aclConnectionElement->linkPolicySettings == DISABLE_ALL_LM_MODES)
        {
            return CSR_BT_ACTIVE_MODE;
        }
    }

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
    {
        if (currentRfcElem->cmRfcConnInst)
        {
            if(bd_addr_eq(&(theAddr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
            {
                if (connected_s == currentRfcElem->cmRfcConnInst->state)
                {
                    switch(currentRfcElem->cmRfcConnInst->requestedMode)
                    {
                        case CSR_BT_ACTIVE_MODE :
                            {/* The link policy must be ACTIVE */
                                return CSR_BT_ACTIVE_MODE;
                            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                        case CSR_BT_SNIFF_MODE :
                            {/* The link policy must be SNIFF */
                                newMode = CSR_BT_SNIFF_MODE;
                                break;
                            }
#endif
                        default :
                            {
                                break;
                            }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem = currentL2caElem->next)
    { /* Search through the L2CAP connection list */
        if (currentL2caElem->cmL2caConnInst)
        {
            if(currentL2caElem->cmL2caConnInst->state == l2capConnected_s)
            { /* The application has a l2cap connection */
                if(bd_addr_eq(&(theAddr), &(currentL2caElem->cmL2caConnInst->deviceAddr)))
                { /* The given device address is right */
                    switch(currentL2caElem->cmL2caConnInst->requestedMode)
                    {
                        case CSR_BT_ACTIVE_MODE :
                            {/* The link policy must be ACTIVE */
                                return CSR_BT_ACTIVE_MODE;
                            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                        case CSR_BT_SNIFF_MODE :
                            {/* The link policy must be SNIFF */
                                newMode = CSR_BT_SNIFF_MODE;
                                break;
                            }
#endif
                        default :
                            {
                                break;
                            }
                    }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */
        if(cmData->bnepVar.connectVar[i].state == bnepConnected_s)
        { /* The application has a bnep connection */
            if(bd_addr_eq(&(theAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* The given device address is right */
                switch(cmData->bnepVar.connectVar[i].requestedMode)
                {
                    case CSR_BT_ACTIVE_MODE :
                        {/* The link policy must be ACTIVE */
                            return CSR_BT_ACTIVE_MODE;
                        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    case CSR_BT_SNIFF_MODE :
                        {/* The link policy must be SNIFF */
                            newMode = CSR_BT_SNIFF_MODE;
                            break;
                        }
#endif
                    default:
                        {
                            break;
                        }
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    if (cmData->sdcVar.sdcSearchList)
    {
        sdcSearchElement    * theElement;
        theElement     = cmData->sdcVar.sdcSearchList;
        do
        {
            if (bd_addr_eq(&(theElement->deviceAddr), &theAddr))
            {
                return CSR_BT_ACTIVE_MODE;
            }
            theElement = theElement->nextSdcElement;
        } while (theElement);

    }
#ifdef CSR_BT_LE_ENABLE
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    /* Nothing but GATT-over-BR/EDR is using this link. we should use
     * sniff mode */
    if (aclConnectionElement
        && (aclConnectionElement->gattConnectionActive))
    {
        newMode = CSR_BT_SNIFF_MODE;
    }
#endif
#endif

    if (aclConnectionElement)
    {
        if (newMode == CSR_BT_SNIFF_MODE)
        {
            if ((aclConnectionElement->linkPolicySettings & ENABLE_SNIFF) == ENABLE_SNIFF)
            { /* SNIFF MODE is supported */
            }
            else
            { /* SNIFF MODE is currently not supported return the Active mode */
                return CSR_BT_ACTIVE_MODE;
            }
        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        else
        {/* The newMode must be CSR_BT_PARK_MODE */
            if ((aclConnectionElement->linkPolicySettings & ENABLE_PARK) == ENABLE_PARK)
            { /* PARK MODE is supported */
            }
            else
            { /* PARK MODE is currently not supported return the actual mode */
                return CSR_BT_ACTIVE_MODE;
            }
        }
#endif
    }
    return newMode;
}

#ifdef CSR_BT_INSTALL_CM_PARK_MODE
static void csrBtCmReturnLowPowerInterval(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr,
                                          CsrBtSniffSettings *sniffSettings, CsrBtParkSettings *parkSettings)
#else
static void csrBtCmReturnLowPowerInterval(cmInstanceData_t *cmData, CsrBtDeviceAddr devAddr,
                                          CsrBtSniffSettings *sniffSettings)
#endif
{
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    cmRfcConnElement *currentRfcElem;
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    cmL2caConnElement *currentL2caElem;
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    CsrUintFast8 i;
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    CsrUint8 maxPriorityDetected = DEFAULT_PRIORITY;

    *sniffSettings              = cmData->dmVar.defaultSniffSettings;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
    *parkSettings               = cmData->dmVar.defaultParkSettings;
#endif

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
    for (currentRfcElem = CM_RFC_GET_FIRST(cmData->rfcVar.connList); currentRfcElem; currentRfcElem = currentRfcElem->next)
    {
        if (currentRfcElem->cmRfcConnInst)
        {
            if(bd_addr_eq(&(devAddr), &(currentRfcElem->cmRfcConnInst->deviceAddr)))
            { /* The given device address is right. Check mode settings */
                if(currentRfcElem->cmRfcConnInst->lowPowerPriority <= maxPriorityDetected)
                {
                    cmRfcConnInstType *theLogicalLink   = currentRfcElem->cmRfcConnInst;

                    maxPriorityDetected = theLogicalLink->lowPowerPriority;
                    *sniffSettings      = theLogicalLink->sniffSettings;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    *parkSettings       = theLogicalLink->parkSettings;
#endif
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
    for (currentL2caElem = CM_L2CA_GET_FIRST(cmData->l2caVar.connList); currentL2caElem; currentL2caElem= currentL2caElem->next)
    { /* Search through the L2CAP connection list */
        if (currentL2caElem->cmL2caConnInst)
        {
            if(bd_addr_eq(&(devAddr), &(currentL2caElem->cmL2caConnInst->deviceAddr)))
            { /* The given device address is right. Check mode settings */
                if(currentL2caElem->cmL2caConnInst->lowPowerPriority <= maxPriorityDetected)
                {
                    cmL2caConnInstType *l2CaConnection = currentL2caElem->cmL2caConnInst;

                    maxPriorityDetected = l2CaConnection->lowPowerPriority;
                    *sniffSettings      = l2CaConnection->sniffSettings;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    *parkSettings       = l2CaConnection->parkSettings;
#endif
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */

        if( cmData->bnepVar.connectVar[i].state == bnepConnected_s ||
            cmData->bnepVar.connectVar[i].state == bnepDisconnectInit_s)
        {
            if(bd_addr_eq(&(devAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* Thie given address is right. Store the actual link policy in the connection table */
                if(cmData->bnepVar.connectVar[i].lowPowerPriority <= maxPriorityDetected)
                {
                    bnepTable *bnepConnection;

                    bnepConnection      = &(cmData->bnepVar.connectVar[i]);
                    maxPriorityDetected = bnepConnection->lowPowerPriority;
                    *sniffSettings      = bnepConnection->sniffSettings;
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    *parkSettings       = bnepConnection->parkSettings;
#endif
                }
            }
        }
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
}

void CsrBtCmDmHciModeChangeFromActive(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr, CsrUint8 newMode)
{
    CsrBtSniffSettings sniffSettings;
#ifdef CSR_AMP_ENABLE
    if (cmData->dmVar.linkPolicyState == LINK_POLICY_AMP_CONTROL_STATE &&
        CsrBtCmBtControllerActive(cmData, theAddr) == CTRL_ACTIVE_AMP_ONLY)
    {
        /* The link must be force to sniff mode because of AMP */
        sniffSettings.max_interval  = CSR_BT_AMP_SNIFF_MAX_TIME_INTERVAL;
        sniffSettings.min_interval  = CSR_BT_AMP_SNIFF_MIN_TIME_INTERVAL;
        sniffSettings.attempt       = CSR_BT_AMP_SNIFF_ATTEMPT;
        sniffSettings.timeout       = CSR_BT_AMP_SNIFF_TIMEOUT;
        csrBtCmHciSniffReqSend(cmData, sniffSettings, theAddr);
    }
    else
#endif /* CSR_AMP_ENABLE */
    {
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        CsrBtParkSettings         parkSettings;
        csrBtCmReturnLowPowerInterval(cmData, theAddr, &sniffSettings, &parkSettings);
#else
        csrBtCmReturnLowPowerInterval(cmData, theAddr, &sniffSettings);
#endif
        if(newMode == CSR_BT_SNIFF_MODE)
        { /* The new requested link policy is CSR_BT_SNIFF_MODE. */
            if( cmData->dmVar.forceSniffSettings )
            {
                csrBtCmHciSniffReqSend(cmData, cmData->dmVar.requestedSniffSetting, theAddr);
                cmData->dmVar.forceSniffSettings = FALSE;
            }
            else
            {
                csrBtCmHciSniffReqSend(cmData, sniffSettings, theAddr);
            }
        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        else
        { /* The new requested link policy is CSR_BT_PARK_MODE. */
            dm_hci_park_mode(&(theAddr), parkSettings.max_interval , parkSettings.min_interval, NULL);
        }
#endif        
    }
}

void CsrBtCmDmHciModeChangeReqMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr theAddr, CsrUint8 theNewMode, CsrUint8 actualMode)
{
    CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_CONNECT_OR_DONE_STATE);
    cmData->dmVar.requestedLinkMode                    = theNewMode;

    switch(actualMode)
    {
        case CSR_BT_SNIFF_MODE :
            { /* The link is currently in CSR_BT_SNIFF_MODE, need to exit
                 sniff to change mode */
                dm_hci_exit_sniff_mode(&(theAddr), NULL);
                break;
            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case CSR_BT_PARK_MODE :
            { /* The link is currently in CSR_BT_PARK_MODE, need to exit
                 park to change mode */
                dm_hci_exit_park_mode(&(theAddr), NULL);
                break;
            }
#endif
        default :
            { /* The link is currently in active mode. Change the link policy to
                 sniff or park */
                CsrBtCmDmHciModeChangeFromActive(cmData, theAddr, theNewMode);
                break;
            }
    }
}

void CsrBtCmDmHciModeChangeEventHandler(cmInstanceData_t *cmData)
{ /* A mode change event has occurred */

    DM_HCI_MODE_CHANGE_EVENT_IND_T    * dmPrim;

    dmPrim = (DM_HCI_MODE_CHANGE_EVENT_IND_T *) cmData->recvMsgP;

    if(dmPrim->status == HCI_SUCCESS)
    { /* The link policy is change. Find out the reason */
        informInvolvedApplications(cmData, dmPrim->bd_addr, dmPrim->mode, dmPrim->length);

        if (cmData->dmVar.linkPolicyState != LINK_POLICY_IDLE_STATE)
        { /* The local device has requested a link policy change. See if
             the given address agree to the operating address */
            if(bd_addr_eq(&(dmPrim->bd_addr), &(cmData->smVar.operatingBdAddr)) )
            { /* There is a large probability for that the local device
                 requested a this link policy change */

                if(cmData->dmVar.requestedLinkMode == dmPrim->mode)
                { /* The link policy request is a with success. */
                    csrBtCmDmHciModeChangeEventLinkPolicyStateHandler(cmData, dmPrim->mode,
                                                                      dmPrim->bd_addr, dmPrim->length, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                }
                else
                { /* There can be three reason for this. The remote device
                     initiated another link policy, or the Host controller
                     has just exit sniff or park */
                    if(dmPrim->mode == CSR_BT_ACTIVE_MODE)
                    {
                        if (cmData->dmVar.requestedLinkMode == CSR_BT_SNIFF_MODE
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                            ||  cmData->dmVar.requestedLinkMode == CSR_BT_PARK_MODE
#endif
                            )
                        { /* The host controller has just exit a mode in order
                             to enter CSR_BT_SNIFF_MODE or CSR_BT_PARK_MODE */
                            CsrBtCmDmHciModeChangeFromActive(cmData, dmPrim->bd_addr, cmData->dmVar.requestedLinkMode);
                        }
                        else
                        {
                            ;
                        }
                    }
                    else
                    {
                        csrBtCmDmModeChangeEventErrorHandler(cmData, dmPrim, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                    }

                }
            }
        }
    }
    else
    { /* The request of changing the link policy fail. Find the reason and
         react on it. */
      /* We will receive a HCI_ERROR_COMMAND_DISALLOWED in case a mode change request is made from both
         sides at the same time. In that case neither dmPrim->mode nor dmPrim->handle nor dmPrim->bdaddr
         is valid, and we need to discard the event. */
        if (cmData->dmVar.linkPolicyState != LINK_POLICY_IDLE_STATE &&
            bd_addr_eq(&(dmPrim->bd_addr), &(cmData->smVar.operatingBdAddr)) )
        {
            csrBtCmDmModeChangeEventErrorHandler(cmData, dmPrim, (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
        }
    }
}

void CsrBtCmDmModeChangeReqMsgSend(CsrUint8 player, CsrUint8 theIndex, CsrUint8 modeRequest, CsrBtDeviceAddr deviceAddr)
{
    CsrBtCmDmModeChangeReq        *prim;

    prim    = (CsrBtCmDmModeChangeReq *)CsrPmemAlloc(sizeof(CsrBtCmDmModeChangeReq));

    prim->type          = CSR_BT_CM_DM_MODE_CHANGE_REQ;
    prim->theIndex      = theIndex;
    prim->theMode       = modeRequest;
    prim->thePlayer     = player;
    prim->deviceAddr    = deviceAddr;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

#ifdef CSR_BT_INSTALL_CM_LOW_POWER_CONFIG_PUBLIC
static void csrBtCmChangeAllAclConnectionsAppControlsLowPower(cmInstanceData_t *cmData, CsrBool appControlsLowPower)
{
    CsrUintFast8 i;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&(cmData->roleVar.aclVar[i].deviceAddr)))
        {
            cmData->roleVar.aclVar[i].appControlsLowPower = appControlsLowPower;

            if (appControlsLowPower == FALSE)
            { /* The application has diable low power control.
                 Make sure that the profile receives the right
                 actual link mode */
                informInvolvedApplications(cmData, cmData->roleVar.aclVar[i].deviceAddr,
                                           cmData->roleVar.aclVar[i].mode, cmData->roleVar.aclVar[i].interval);
            }
        }
    }
}


static CsrBtResultCode csrBtCmUpdateControlLowPowerParameter(cmInstanceData_t *cmData,
                                                             CsrBtDeviceAddr deviceAddr, CsrBool controlField)
{
    if (CsrBtBdAddrEqZero(&deviceAddr))
    { /* Either will the application have full low power control
         or it will not control low power control at all */
        cmData->dmVar.appControlsAllLowPower = controlField;
        csrBtCmChangeAllAclConnectionsAppControlsLowPower(cmData, controlField);
        return (CSR_BT_RESULT_CODE_CM_SUCCESS);
    }
    else
    { /* The application will only have low power control on a
         specific ACL connection or disable low power control
         on this ACL connection */
        aclTable            *aclConnectionElement;

        returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

        if (aclConnectionElement)
        { /* The ACL connection represented by the given device
             address is connected */
            aclConnectionElement->appControlsLowPower = controlField;

            if (controlField == FALSE)
            { /* The application has diable low power control.
                 Make sure that the profile receives the right
                 actual link mode */
                informInvolvedApplications(cmData, deviceAddr,
                                           aclConnectionElement->mode, aclConnectionElement->interval);
            }
            return (CSR_BT_RESULT_CODE_CM_SUCCESS);
        }
        else
        { /* The ACL connection represented by the given device
             address is not connected */
            return (CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER);
        }
    }
}

void CsrBtCmModeChangeConfigReqHandler(cmInstanceData_t *cmData)
{
    CsrBtResultCode            result;
    CsrBtCmModeChangeConfigReq * cmPrim;

    cmPrim = (CsrBtCmModeChangeConfigReq *) cmData->recvMsgP;

    if ((cmPrim->config & CSR_BT_CM_MODE_CHANGE_ENABLE) == CSR_BT_CM_MODE_CHANGE_ENABLE)
    { /* The application wants full low power control */
        result = csrBtCmUpdateControlLowPowerParameter(cmData, cmPrim->deviceAddr, TRUE);
    }
    else if ((cmPrim->config & CSR_BT_CM_MODE_CHANGE_DISABLE) == CSR_BT_CM_MODE_CHANGE_DISABLE)
    { /* The application wants to disable low power control */
        result = csrBtCmUpdateControlLowPowerParameter(cmData, cmPrim->deviceAddr, FALSE);
    }
    else
    { /* Invalid config parameter inform the application */
        result = CSR_BT_RESULT_CODE_CM_UNACCEPTABLE_PARAMETER;
    }
    csrBtCmModeChangeConfigCfmMsgSend(cmPrim->phandle, result, CSR_BT_SUPPLIER_CM, cmPrim->deviceAddr);
}
#endif
#ifdef CSR_BT_INSTALL_CM_LOW_POWER_CONFIG_PUBLIC
void CsrBtCmModeChangeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmModeChangeReq    *cmPrim;

    cmPrim = (CsrBtCmModeChangeReq *) cmData->recvMsgP;


    cmData->smVar.appHandle             = cmPrim->phandle;
    cmData->dmVar.requestedSniffSetting = cmPrim->sniffSettings;
    cmData->dmVar.forceSniffSettings    = cmPrim->forceSniffSettings;
    CsrBtCmDmModeChangeReqMsgSend(APP_PLAYER, CM_ERROR, cmPrim->mode, cmPrim->deviceAddr);
}
#endif

#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmRfcModeChangeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmRfcModeChangeReq *cmPrim;
    cmRfcConnElement *theElement;
    
    cmPrim = (CsrBtCmRfcModeChangeReq *) cmData->recvMsgP;
    theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromBtConnId,
                                     &(cmPrim->btConnId));
    if(theElement)
    {
        cmRfcConnInstType *theLogicalLink   = theElement->cmRfcConnInst;

        if(theLogicalLink->state == connected_s)
        {
            /* The link is in a state where it is allowed to proceed
             * with the request. The request is sent to the DM
             * module */
            CsrUint8 player = (cmPrim->forceSniff
                               ? AMP_PLAYER
                               : RFC_PLAYER);
            CsrBtCmDmModeChangeReqMsgSend(player, theElement->elementId,
                                          cmPrim->requestedMode, theLogicalLink->deviceAddr);
        }
        else
        {
            /* The application is not allowed to change the current link policy */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    {
        /* Restore the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmDmRfcModeChangeMsgSend(cmInstanceData_t *cmData, CsrUint8 theIndex, CsrUint8 requestedMode)
{
    cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromIndex, &(theIndex));
    aclTable *aclConnectionElement;

    if (theElement && returnAclConnectionElement(cmData, theElement->cmRfcConnInst->deviceAddr, &aclConnectionElement) != CM_ERROR)
    {
        cmRfcConnInstType *theLogicalLink = theElement->cmRfcConnInst;
        cmData->smVar.operatingBdAddr     = theLogicalLink->deviceAddr;

        switch(theLogicalLink->state)
        {
            case connected_s :
                {
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    if((theLogicalLink->eScoParms.handle != NO_SCO ) && (requestedMode == CSR_BT_PARK_MODE) )
                    { /* Trying to request park mode while having or has accepted a SCO
                         connection. Return error, and keep old requestedMode */
                        CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink, 0, theLogicalLink->actualMode,
                                                       CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM);
                        CsrBtCmDmLocalQueueHandler();
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                    else
#endif
#endif
                    {
                        if (csrBtCmRequestedModeSupported(aclConnectionElement, requestedMode))
                        {/* Save requested link mode and find out if the hostcontroller need
                            to change the current link policy */
                            CsrUint8                newMode;

                            theLogicalLink->requestedMode    = requestedMode;
                            newMode                          = CsrBtCmDmReturnRequestedMode(cmData, theLogicalLink->deviceAddr);

                            if(newMode != theLogicalLink->actualMode)
                            { /* Check if the profile is allowed to change the current link
                                 policy */
                                if (csrBtCmModeChangeAllowed(aclConnectionElement))
                                { /* The profile is allowed to change the current link
                                     policy */
                                    CsrBtCmDmHciModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr,
                                                                     newMode, theLogicalLink->actualMode);
                                }
                                else
                                { /* The application is handling the low power mode */
                                    CsrBtResultCode resultCode = csrBtReturnResultCode(newMode);
                                    CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink, aclConnectionElement->interval,
                                                                   theLogicalLink->actualMode, resultCode, CSR_BT_SUPPLIER_CM);
                                    CsrBtCmDmLocalQueueHandler();
                                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                                }
                            }
                            else
                            { /* The application is not allowed to change the current link
                                 policy right now, or is already in this mode. Inform the
                                 application */
                                CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink, aclConnectionElement->interval,
                                                               theLogicalLink->actualMode,
                                                               CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                                CsrBtCmDmLocalQueueHandler();
                                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                            }
                        }
                        else
                        {/* The application is not allowed to change the current link
                            policy because it is not supported */
                            CsrBtCmRfcModeChangeIndMsgSend(theLogicalLink,
                                                           aclConnectionElement->interval,
                                                           theLogicalLink->actualMode,
                                                           CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                                                           CSR_BT_SUPPLIER_CM);

                            csrBtCmModeChangeUnSupportedFeatureHandler(cmData,
                                                                       aclConnectionElement->linkPolicySettings,
                                                                       theLogicalLink->deviceAddr,
                                                                       theLogicalLink->actualMode);
                        }
                    }
                    break;
                }
            case connectInit_s :
                {
                    theLogicalLink->actualMode        = CsrBtCmDmReturnActualMode(cmData, theLogicalLink->deviceAddr);
                    if(theLogicalLink->actualMode == CSR_BT_ACTIVE_MODE)
                    {/* Currently the actual link policy mode is ACTIVE_MODE.
                        Ensure that the link policy can't be change doing connect */
                        CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, theLogicalLink->deviceAddr, RFC_PLAYER);
                    }
                    else
                    {/* Currently the actual link policy mode is different from
                        ACTIVE_MODE. Change the link policy to ACTIVE_MODE */
                        CsrBtCmDmHciModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr, CSR_BT_ACTIVE_MODE, theLogicalLink->actualMode);
                    }
                    break;
                }
            case releaseInit_s :
                {
                    theLogicalLink->requestedMode    = CSR_BT_ACTIVE_MODE;

                    if(theLogicalLink->actualMode == CSR_BT_ACTIVE_MODE)
                    { /* Currently the link policy is set ACTIVE to the given Bluetooth
                         address. Ensure that the DM only accept ACTIVE link policy
                         before start disconnecting */
                        CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, theLogicalLink->deviceAddr, RFC_PLAYER);
                    }
                    else
                    { /* The link policy against the given Bluetooth address must be
                         change to active, before CM can ensure that the DM only accept
                         ACTIVE link policy. */
                        CsrBtCmDmHciModeChangeReqMsgSend(cmData, theLogicalLink->deviceAddr, CSR_BT_ACTIVE_MODE, theLogicalLink->actualMode);
                    }
                    break;
                }
            default :
                { /* The CM is not allowed to change the current link policy */
                    CsrBtCmDmLocalQueueHandler();
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    break;
                }
        }
    }
    else
    { /* The CM is not allowed to change the current link policy */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
void CsrBtCmL2caModeChangeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmL2caModeChangeReq    *cmPrim = (CsrBtCmL2caModeChangeReq *) cmData->recvMsgP;
    cmL2caConnElement * theElement      = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromBtConnId, &(cmPrim->btConnId));

    if (theElement)
    {
        cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;

        if(l2CaConnection->state == l2capConnected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. The request is sent to the DM module */
            CsrUint8 player;

            if (!cmPrim->forceSniff)
            {
                player = L2CAP_PLAYER;
            }
            else
            {
                player = AMP_PLAYER;
            }
            CsrBtCmDmModeChangeReqMsgSend(player,
                                          theElement->elementId,
                                          cmPrim->requestedMode,
                                          l2CaConnection->deviceAddr);
        }
        else
        { /* The application is not allowed to change the current link policy */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Restore the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}


void CsrBtCmDmL2capModeChangeMsgSend(cmInstanceData_t *cmData, CsrUint8 theIndex, CsrUint8 requestedMode)
{
    cmL2caConnElement *theElement = CM_FIND_L2CA_ELEMENT(CsrBtCmL2caFindL2caConnElementFromIndex, &theIndex);

    if (theElement)
    {
        cmL2caConnInstType *l2CaConnection = theElement->cmL2caConnInst;
        cmData->smVar.operatingBdAddr      = l2CaConnection->deviceAddr;

        switch(l2CaConnection->state)
        {
            case l2capConnected_s :
                { /* The link is in a state where it is allow to change proceed with the
                     link policy request */
                    aclTable   *aclConnectionElement;

                    returnAclConnectionElement(cmData, l2CaConnection->deviceAddr, &aclConnectionElement);

                    if ((aclConnectionElement != NULL) &&
                        csrBtCmRequestedModeSupported(aclConnectionElement, requestedMode))
                    {/* Save requested link mode and find out if the hostcontroller
                        need to change the current link policy */
                        CsrUint8            newMode;

                        l2CaConnection->requestedMode = requestedMode;
                        newMode                       = CsrBtCmDmReturnRequestedMode(cmData, l2CaConnection->deviceAddr);

                        if(newMode != l2CaConnection->actualMode)
                        { /* Check if the profile is allowed to change the current link
                             policy */
                            if (csrBtCmModeChangeAllowed(aclConnectionElement))
                            { /* The profile is allowed to change the current link
                                 policy */
                                CsrBtCmDmHciModeChangeReqMsgSend(cmData, l2CaConnection->deviceAddr,
                                                                 newMode, l2CaConnection->actualMode);
                            }
                            else
                            { /* The application is handling the low power mode */
                                CsrBtResultCode resultCode = csrBtReturnResultCode(newMode);
                                CsrBtCmL2caModeChangeIndMsgSend(l2CaConnection, aclConnectionElement->interval,
                                                                l2CaConnection->actualMode, resultCode, CSR_BT_SUPPLIER_CM);
                                CsrBtCmDmLocalQueueHandler();
                                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                            }
                        }
                        else
                        { /* The application is not allowed to change the current link
                             policy right now, or is already in this mode. Inform the
                             application */
                            CsrBtCmL2caModeChangeIndMsgSend(l2CaConnection,
                                                            aclConnectionElement->interval, l2CaConnection->actualMode,
                                                            CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

                            CsrBtCmDmLocalQueueHandler();
                            CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        }
                    }
                    else
                    {/* The application is not allowed to change the current link
                        policy because it is not supported */
                        CsrBtCmL2caModeChangeIndMsgSend(l2CaConnection, 0,
                                                        l2CaConnection->actualMode,
                                                        CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                                                        CSR_BT_SUPPLIER_CM);
                        if (aclConnectionElement)
                        {
                            csrBtCmModeChangeUnSupportedFeatureHandler(cmData, aclConnectionElement->linkPolicySettings, l2CaConnection->deviceAddr, l2CaConnection->actualMode);
                        }
                        else
                        {
                            CsrBtCmDmLocalQueueHandler();
                            CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        }
                    }
                    break;
                }
            case l2capReleaseInit_s :
                {
                    l2CaConnection->requestedMode    = CSR_BT_ACTIVE_MODE;

                    if(l2CaConnection->actualMode == CSR_BT_ACTIVE_MODE)
                    { /* Currently the link policy is set ACTIVE to the given Bluetooth
                         address. Ensure that the DM only accept ACTIVE link policy
                         before start disconnecting */
                        CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, l2CaConnection->deviceAddr, L2CAP_PLAYER);
                    }
                    else
                    { /* The link policy against the given Bluetooth address must be
                         change to active, before CM can ensure that the DM only accept
                         ACTIVE link policy. */
                        CsrBtCmDmHciModeChangeReqMsgSend(cmData, l2CaConnection->deviceAddr, CSR_BT_ACTIVE_MODE, l2CaConnection->actualMode);
                    }
                    break;
                }
            case l2capReleaseInitXover_s:
                { /* Crossover between local and remote disconnect. Goto l2capReleaseFinal_s */
                    CsrBtCmL2caFinalReleaseHandler(cmData, theElement, TRUE,
                                                   (CsrBtReasonCode) cmData->smVar.resultCode, cmData->smVar.resultSupplier, FALSE);
                    CsrBtCmDmLocalQueueHandler();
                    break;
                }
            case l2capConnectInit_s :
                {
                    l2CaConnection->actualMode        = CsrBtCmDmReturnActualMode(cmData, l2CaConnection->deviceAddr);

                    if(l2CaConnection->actualMode == CSR_BT_ACTIVE_MODE)
                    {/* Currently the actual link policy mode is CSR_BT_ACTIVE_MODE.
                        Ensure that the link policy can't be change doing connect */
                        CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, l2CaConnection->deviceAddr, L2CAP_PLAYER);
                    }
                    else
                    {/* Currently the actual link policy mode is different from
                        CSR_BT_ACTIVE_MODE. Change the link policy to CSR_BT_ACTIVE_MODE */
                        CsrBtCmDmHciModeChangeReqMsgSend(cmData, l2CaConnection->deviceAddr, CSR_BT_ACTIVE_MODE, l2CaConnection->actualMode);
                    }
                    break;
                }
            default    :
                {  /* The CM is not allowed to change the current link policy */
                    CsrBtCmDmLocalQueueHandler();
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    break;
                }
        }
    }
    else
    { /* The CM is not allowed to change the current link policy */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

#ifdef CSR_AMP_ENABLE
static void csrBtCmDmL2capAmpModeChangeHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    /* A logical l2cap connection has been move by AMP. */
    aclTable *aclConnectionElement;
    returnAclConnectionElement(cmData, deviceAddr, &aclConnectionElement);

    if (aclConnectionElement && !aclConnectionElement->appControlsLowPower)
    {
        CsrUint8 activity;
        activity = CsrBtCmBtControllerActive(cmData, deviceAddr);

        if (activity == CTRL_ACTIVE_BREDR_ACTIVE)
        {
            /* At least one logical connection is using CSR_BT_AMP_CONTROLLER_BREDR */
            if (aclConnectionElement->mode == CSR_BT_SNIFF_MODE &&
                aclConnectionElement->sniffForceByAmp)
            {
                /* The ACL link were force in SNIFF mode by AMP. Exit sniff */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_AMP_CONTROL_STATE);
                cmData->dmVar.requestedLinkMode = CsrBtCmDmReturnRequestedMode(cmData, deviceAddr);
                cmData->smVar.operatingBdAddr   = deviceAddr;
                dm_hci_exit_sniff_mode(&(deviceAddr), NULL);
            }
            else
            {
                /* The ACL link were not force in SNIFF mode by AMP,
                 * keep current settings */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
        }
        else
        {
            if (activity == CTRL_ACTIVE_AMP_ONLY && 
                csrBtCmRequestedModeSupported(aclConnectionElement, CSR_BT_SNIFF_MODE))
            {
                /* Try to force the ACL link into SNIFF mode in order
                 * give AMP more radio time */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_AMP_CONTROL_STATE);
                cmData->dmVar.requestedLinkMode = CSR_BT_SNIFF_MODE;
                cmData->smVar.operatingBdAddr   = deviceAddr;

                switch(aclConnectionElement->mode)
                {
                    case CSR_BT_SNIFF_MODE:
                        {
                            /* The ACL link is allready in Sniff
                             * mode. Exit sniff in order to negotiate
                             * new Sniff settings */
                            dm_hci_exit_sniff_mode(&(deviceAddr), NULL);
                            break;
                        }
                    case CSR_BT_ACTIVE_MODE:
                        {
                            /* Try to go to SNIFF mode */
                            CsrBtCmDmHciModeChangeFromActive(cmData, deviceAddr, cmData->dmVar.requestedLinkMode);
                            break;
                        }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                    case CSR_BT_PARK_MODE:
                        {
                            /* Must exit park first */
                            dm_hci_exit_park_mode(&(deviceAddr), NULL);
                            break;
                        }
#endif
                    default:
                        {
                            cmData->dmVar.linkPolicyState = LINK_POLICY_IDLE_STATE;
                            CsrBtCmDmLocalQueueHandler();
                            CsrBtCmServiceManagerLocalQueueHandler(cmData);
                            break;
                        }
                }
            }
            else
            {
                /* Sniff mode is not supported, or no connections
                 * exist. Just ignore. */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
        }
    }
    else
    {
        /* No ACL connection or the application is controlling low power */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}
#endif /* #ifdef CSR_AMP_ENABLE */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
void CsrBtCmBnepModeChangeReqHandler(cmInstanceData_t *cmData)
{
    CsrUint8 theIndex;
    CsrBtCmBnepModeChangeReq    *cmPrim;

    cmPrim        = (CsrBtCmBnepModeChangeReq *) cmData->recvMsgP;
    theIndex    = returnBnepIdIndex(cmData, cmPrim->id);

    if(theIndex != CM_ERROR)
    {
        bnepTable *bnepConnection;

        bnepConnection        = &(cmData->bnepVar.connectVar[theIndex]);

        if(bnepConnection->state == bnepConnected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. The request is sent to the DM module */
            CsrBtCmDmModeChangeReqMsgSend(BNEP_PLAYER, theIndex,
                                          cmPrim->requestedMode, bnepConnection->deviceAddr);
        }
        else
        { /* The application is not allowed to change the current link policy */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Restore the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmDmBnepModeChangeMsgSend(cmInstanceData_t *cmData, CsrUint8 theIndex, CsrUint8 requestedMode)
{
    bnepTable *bnepConnection;

    bnepConnection                    = &(cmData->bnepVar.connectVar[theIndex]);
    cmData->smVar.operatingBdAddr    = bnepConnection->deviceAddr;

    switch(bnepConnection->state)
    {
        case bnepConnectInit_s :
            {
                bnepConnection->actualMode        = CsrBtCmDmReturnActualMode(cmData, bnepConnection->deviceAddr);

                if(bnepConnection->actualMode == CSR_BT_ACTIVE_MODE)
                {/* Currently the actual link policy mode is CSR_BT_ACTIVE_MODE.
                    Ensure that the link policy can't be change doing connect */
                    CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, bnepConnection->deviceAddr, BNEP_PLAYER);
                }
                else
                {/* Currently the actual link policy mode is different from
                    CSR_BT_ACTIVE_MODE. Change the link policy to CSR_BT_ACTIVE_MODE */
                    CsrBtCmDmHciModeChangeReqMsgSend(cmData, bnepConnection->deviceAddr, CSR_BT_ACTIVE_MODE, bnepConnection->actualMode);
                }
                break;
            }
        case bnepConnectCfm_s:
            {
                bnepConnection->actualMode        = CsrBtCmDmReturnActualMode(cmData, bnepConnection->deviceAddr);

                if(bnepConnection->actualMode == CSR_BT_ACTIVE_MODE)
                {/* Currently the actual link policy mode is CSR_BT_ACTIVE_MODE.
                    Ensure that the link policy can't be change doing connect */
                    CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, bnepConnection->deviceAddr, BNEP_PLAYER);
                }
                else
                {/* Currently the actual link policy mode is different from
                    CSR_BT_ACTIVE_MODE. Change the link policy to CSR_BT_ACTIVE_MODE */
                    CsrBtCmDmHciModeChangeReqMsgSend(cmData, bnepConnection->deviceAddr, CSR_BT_ACTIVE_MODE, bnepConnection->actualMode);
                }
                break;
            }
        case bnepConnected_s :
            { /* The link is in a state where it is allow to change proceed with the
                 link policy request */
                aclTable   *aclConnectionElement;

                returnAclConnectionElement(cmData, bnepConnection->deviceAddr, &aclConnectionElement);

                if (csrBtCmRequestedModeSupported(aclConnectionElement, requestedMode))
                {/* Save requested link mode and find out if the hostcontroller need
                    to change the current link policy */
                    CsrUint8            newMode;

                    bnepConnection->requestedMode = requestedMode;
                    newMode                       = CsrBtCmDmReturnRequestedMode(cmData, bnepConnection->deviceAddr);

                    if(newMode != bnepConnection->actualMode)
                    { /* Check if the profile is allowed to change the current link
                         policy */
                        if (csrBtCmModeChangeAllowed(aclConnectionElement))
                        { /* The profile is allowed to change the current link
                             policy */
                            CsrBtCmDmHciModeChangeReqMsgSend(cmData, bnepConnection->deviceAddr,
                                                             newMode, bnepConnection->actualMode);
                        }
                        else
                        { /* The application is handling the low power mode */
                            CsrBtResultCode resultCode = csrBtReturnResultCode(newMode);
                            csrBtCmBnepModeChangeIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                            aclConnectionElement->interval, bnepConnection->actualMode,
                                                            resultCode, CSR_BT_SUPPLIER_CM);

                            CsrBtCmDmLocalQueueHandler();
                            CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        }
                    }
                    else
                    { /* The application is not allowed to change the current link
                         policy right now, or is already in this mode. Inform the
                         application */
                        csrBtCmBnepModeChangeIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                        0, bnepConnection->actualMode, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
                        CsrBtCmDmLocalQueueHandler();
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                }
                else
                {/* The application is not allowed to change the current link
                    policy because it is not supported */
                    csrBtCmBnepModeChangeIndMsgSend(cmData->bnepVar.appHandle, bnepConnection, 0, bnepConnection->actualMode,
                                                    CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE, CSR_BT_SUPPLIER_CM);
                    if (aclConnectionElement)
                    {
                        csrBtCmModeChangeUnSupportedFeatureHandler(cmData, aclConnectionElement->linkPolicySettings, bnepConnection->deviceAddr, bnepConnection->actualMode);
                    }
                    else
                    {
                        CsrBtCmDmLocalQueueHandler();
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                    }
                }
                break;
            }
        case bnepDisconnectInit_s :
            {
                bnepConnection->requestedMode    = CSR_BT_ACTIVE_MODE;

                if(bnepConnection->actualMode == CSR_BT_ACTIVE_MODE)
                { /* Currently the link policy is set ACTIVE to the given Bluetooth
                     address. Ensure that the DM only accept ACTIVE link policy
                     before start disconnecting */
                    CsrBtCmDmWriteLpSettingsOnlyMssMsgSend(cmData, bnepConnection->deviceAddr, BNEP_PLAYER);
                }
                else
                { /* The link policy against the given Bluetooth address must be
                     change to active, before CM can ensure that the DM only accept
                     ACTIVE link policy. */
                    CsrBtCmDmHciModeChangeReqMsgSend(cmData, bnepConnection->deviceAddr, CSR_BT_ACTIVE_MODE, bnepConnection->actualMode);
                }
                break;
            }
        default    :
            {  /* The CM is not allowed to change the current link policy */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
    }
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

#ifdef CSR_BT_LE_ENABLE
static void csrBtCmDmLeModeChangeMsgSend(cmInstanceData_t *cmData,
                                         CsrBtDeviceAddr deviceAddr)
{
    aclTable *acl = NULL;
    CsrUint8 oldMode;
    CsrUint8 newMode;
    
    if (returnAclConnectionElement(cmData, deviceAddr,
                                   &acl) != CM_ERROR)
    {
        oldMode = CsrBtCmDmReturnActualMode(cmData, deviceAddr);
        newMode = CsrBtCmDmReturnRequestedMode(cmData, deviceAddr);
        if (oldMode != newMode)
        {
            cmData->smVar.operatingBdAddr = deviceAddr;
            CsrBtCmDmHciModeChangeReqMsgSend(cmData,
                                             deviceAddr,
                                             newMode, oldMode);
            return;
        }
    }
    
    /* Action above returned, so unlock if we did nonthing */
    CsrBtCmDmLocalQueueHandler();
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}
#endif

void CsrBtCmDmModeChangeReqHandler(cmInstanceData_t *cmData)
{
    /* One of the application has requested a link policy change. */
    CsrBtCmDmModeChangeReq *prim;

    prim   = (CsrBtCmDmModeChangeReq *) cmData->recvMsgP;

    switch (prim->thePlayer)
    {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
        case RFC_PLAYER :
            { /* The application which has requested the link policy is using the
                 RFC module */
                CsrBtCmDmRfcModeChangeMsgSend(cmData, prim->theIndex, prim->theMode);
                break;
            }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
        case L2CAP_PLAYER :
            { /* The application which has requested the link policy is using the
                 L2CAP module */
                CsrBtCmDmL2capModeChangeMsgSend(cmData, prim->theIndex, prim->theMode);
                break;
            }
#ifdef CSR_AMP_ENABLE
        case AMP_PLAYER:
            {
                csrBtCmDmL2capAmpModeChangeHandler(cmData, prim->deviceAddr);
                break;
            }
#endif /* CSR_AMP_ENABLE */
#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        case BNEP_PLAYER :
            { /* The application which has requested the link policy is using the
                 BNEP module */
                CsrBtCmDmBnepModeChangeMsgSend(cmData, prim->theIndex, prim->theMode);
                break;
            }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

        case SDC_PLAYER:
            {
                csrBtCmDmSdcModeChangeMsgSend(cmData);
                break;
            }
        case CM_LINK_POLICY_PLAYER:
            {
                csrBtCmDmLinkPolicyPlayerModeChangeMsgSend(cmData, prim->theIndex);
                break;
            }
        case APP_PLAYER:
            {
                csrBtCmDmAppPlayerModeChangeMsgSend(cmData, prim->theMode, prim->deviceAddr);
                break;
            }
#ifdef CSR_BT_LE_ENABLE
        case LE_PLAYER:
        {
            csrBtCmDmLeModeChangeMsgSend(cmData, prim->deviceAddr);
            break;
        }
#endif
        default :
            { /* Restore the local Service Manager and DM queue in order to prevent
                 a deadlock */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
    }
}


