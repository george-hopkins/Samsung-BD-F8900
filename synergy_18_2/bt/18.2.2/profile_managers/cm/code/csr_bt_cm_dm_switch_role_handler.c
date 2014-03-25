/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

static void csrBtCmDmSwitchRoleReqSendExt(CsrBtDeviceAddr deviceAddr, CsrUint8 role, CsrUint8 player)
{
    CsrBtCmDmSwitchRoleReq        *prim;

    prim                = pnew(CsrBtCmDmSwitchRoleReq);
    prim->type          = CSR_BT_CM_DM_SWITCH_ROLE_REQ;
    prim->deviceAddr    = deviceAddr;
    prim->role          = role;
    prim->activePlayer  = player;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

static void csrBtCmDmSwitchRoleReqSend(CsrBtDeviceAddr deviceAddr, CsrUint8 role)
{
    csrBtCmDmSwitchRoleReqSendExt(deviceAddr, role, CM_PLAYER);
}

void CsrBtCmRoleSwitchCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode,
                              CsrBtSupplier resultSupplier,
                              CsrBtDeviceAddr deviceAddr, CsrUint8 role, CsrBtCmRoleType roleType)
{
    CsrBtCmSwitchRoleCfm        *prim;

    prim                 = pnew(CsrBtCmSwitchRoleCfm);
    prim->type           = CSR_BT_CM_SWITCH_ROLE_CFM;
    prim->deviceAddr     = deviceAddr;
    prim->role           = role;
    prim->roleType       = roleType;
    prim->resultCode     = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
static void csrBtCmBnepSwitchRoleIndMsgSend(CsrSchedQid appHandle, bnepTable *bnepConnection, CsrUint8 role,
                                            CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{ /* Build and send CSR_BT_CM_BNEP_SWITCH_ROLE_IND to the application */
    CsrBtCmBnepSwitchRoleInd    *cmPrim;

    cmPrim = (CsrBtCmBnepSwitchRoleInd *)CsrPmemAlloc(sizeof(CsrBtCmBnepSwitchRoleInd));

    cmPrim->type            = CSR_BT_CM_BNEP_SWITCH_ROLE_IND;
    cmPrim->id              = bnepConnection->id;
    cmPrim->role            = role;
    cmPrim->resultCode      = resultCode;
    cmPrim->resultSupplier  = resultSupplier;
    CsrBtCmPutMessage(appHandle, cmPrim);
}

static void informProfilesAboutRoleSwitch(cmInstanceData_t *cmData, CsrUint8 role, CsrBtDeviceAddr deviceAddr)
{ /* Inform all involved applications about the current link policy */
    CsrUintFast8 i = 0;

    for( i = 0; i < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS; i++ )
    { /* Search through the BNEP connection table */
        if(cmData->bnepVar.connectVar[i].state == bnepConnected_s)
        {
            if(bd_addr_eq(&(deviceAddr), &(cmData->bnepVar.connectVar[i].deviceAddr)))
            { /* The given device address is right. Build and send
                 CSR_BT_CM_BNEP_SWITCH_ROLE_IND to the profile with the current role. */
                bnepTable *bnepConnection;

                bnepConnection    = &(cmData->bnepVar.connectVar[i]);
                csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                role, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
            }
        }
    }
}

static void informProfileAboutRoleSwitchError(cmInstanceData_t *cmData, CsrUint8 role, CsrBtDeviceAddr deviceAddr,
                                              CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{
    if (cmData->bnepVar.roleSwitchReqIndex != CM_ERROR)
    {
        if(cmData->bnepVar.roleSwitchReqIndex < CSR_BT_MAX_NUM_OF_SIMULTANEOUS_BNEP_CONNECTIONS)
        {
            bnepTable *bnepConnection;

            bnepConnection    = &(cmData->bnepVar.connectVar[cmData->bnepVar.roleSwitchReqIndex]);
            if(bd_addr_eq(&(deviceAddr), &(bnepConnection->deviceAddr)))
            {
                if(bnepConnection->state == bnepConnected_s)
                {
                    csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle,
                                                    bnepConnection, role, resultCode, resultSupplier);
                }
                cmData->bnepVar.roleSwitchReqIndex = CM_ERROR;
            }
        }
    }
}

void CsrBtCmBnepSwitchRoleReqHandler(cmInstanceData_t *cmData)
{
    CsrUint8 theIndex;
    CsrBtCmBnepSwitchRoleReq    *cmPrim;

    cmPrim                      = (CsrBtCmBnepSwitchRoleReq *) cmData->recvMsgP;
    theIndex                    = returnBnepIdIndex(cmData, cmPrim->id);

    if(theIndex != CM_ERROR)
    {
        bnepTable *bnepConnection;

        bnepConnection        = &(cmData->bnepVar.connectVar[theIndex]);

        if(bnepConnection->state == bnepConnected_s)
        { /* The link is in a state where it is allowed to proceed with the
             request. Change link settings before process with dmSwitchRole. */
            aclTable   * aclConnectionElement;

            returnAclConnectionElement(cmData, bnepConnection->deviceAddr, &aclConnectionElement);

            if (aclConnectionElement)
            {
                if (aclConnectionElement->role == cmPrim->role)
                {
                    csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                    cmPrim->role, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);

                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                }
                else
                {
                    if (cmPrim->role == CSR_BT_SLAVE_ROLE)
                    {
                        if (cmData->roleVar.alwaysSupportMasterRole)
                        {
                            csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle, bnepConnection, cmPrim->role,
                                                            CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM);

                            CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        }
                        else
                        {
                            CsrUint8 numOfAcl = returnNumOfAclConnection(cmData);

                            if (numOfAcl > 1)
                            {
                                csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle, bnepConnection,
                                                                cmPrim->role, CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM);

                                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                            }
                            else
                            {
                                cmData->bnepVar.roleSwitchReqIndex  = theIndex;
                                csrBtCmDmSwitchRoleReqSend(bnepConnection->deviceAddr, cmPrim->role);
                            }
                        }
                    }
                    else
                    {
                        cmData->bnepVar.roleSwitchReqIndex  = theIndex;
                        csrBtCmDmSwitchRoleReqSend(bnepConnection->deviceAddr, cmPrim->role);
                    }
                }
            }
            else
            {
                csrBtCmBnepSwitchRoleIndMsgSend(cmData->bnepVar.appHandle, bnepConnection, cmPrim->role,
                                                CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER, CSR_BT_SUPPLIER_CM);

                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
        }
        else
        { /* The application is not allowed to change the current Role. Restore
             the local service manager queue */
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
        }
    }
    else
    { /* Restore the local service manager queue */
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

static void csrBtCmDmWriteLpSettingsRoleSwitchMsgSend(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr, link_policy_settings_t enableMs, CsrUint8 currentRole)
{
    link_policy_settings_t linkPolicySettings;
    CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
    linkPolicySettings |= enableMs;

    cmData->roleVar.roleSwitchFlag = FALSE;

    CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, CM_PLAYER, linkPolicySettings);

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    informProfileAboutRoleSwitchError(cmData, currentRole, deviceAddr, CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM);
#else
    CSR_UNUSED(currentRole);
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
}

static void csrBtCmDmSwitchRoleMoreThanOneAclHandler(cmInstanceData_t *cmData,
                                                     aclTable * aclConnectionElement,
                                                     CsrUint8 currentRole,
                                                     CsrBtDeviceAddr deviceAddr)
{
    if (cmData->roleVar.requestedRole == CSR_BT_MASTER_ROLE)
    { /* Need to change make a role switch */
        if (currentRole == CSR_BT_MASTER_ROLE)
        { /* The role is right just make sure that a MSS is not supported */
            link_policy_settings_t linkPolicySettings;
            CsrBtCmReturnLowPowerSettings(cmData, deviceAddr, &linkPolicySettings);
            linkPolicySettings &= 0xFFFE;
            aclConnectionElement->roleChecked = TRUE;
            CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, KEEP_CURRENT_PLAYER, linkPolicySettings);
        }
        else
        { /* Need to change make a role switch */
            CsrBtCmDmModeChangeRoleSwitchHandler(cmData, deviceAddr);
        }
    }
    else
    { /* Not allow to make a role switch other than to Master because the local
         device has more than 1 ACL connection or the alwaysSupportMasterRole flag
         is set */
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
        informProfileAboutRoleSwitchError(cmData, currentRole, deviceAddr,
                                          CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED, CSR_BT_SUPPLIER_CM);
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */
    }
}

void CsrBtCmDmClearRoleCheckedFlag(cmInstanceData_t *cmData)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        cmData->roleVar.aclVar[i].roleChecked = FALSE;
    }
}

CsrBool CsrBtCmDmIsScoPacketType(hci_pkt_type_t packetType)
{
    if (packetType & 0x0007)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
void CsrBtCmRfcScoConnectReqHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    if (cmData->dmVar.contableIndex != CM_ERROR)
    {
        cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromIndex, &(cmData->dmVar.contableIndex));

        if (theElement && theElement->cmRfcConnInst->state == connected_s && bd_addr_eq(&(deviceAddr), &(theElement->cmRfcConnInst->deviceAddr)))
        {
            cmRfcConnInstType * theLogicalLink = theElement->cmRfcConnInst;

            if (theLogicalLink->eScoParms.handle == NO_SCO)
            {
                CsrBtCmScoCommonParms parms;
                if (CsrBtCmScoGetCurrentNegotiateParms(theLogicalLink->eScoParms.negotiateCnt, &parms))
                {
                    cmData->smVar.activePlayer = RFC_PLAYER;
                    if (CsrBtCmDmIsScoPacketType(parms.audioQuality))
                    {
                        /* Set parameters for legacy SCO */
                        dm_sync_connect_req(CSR_BT_CM_IFACEQUEUE,
                                            0, /* pv_cbarg */
                                            &theLogicalLink->deviceAddr,
                                            8000, /* tx bandwidth */
                                            8000, /* rx bandwidth */
                                            5, /* max latency */
                                            parms.voiceSettings,
                                            HCI_ESCO_NO_RETX, /* retx effort */
                                            parms.audioQuality);
                    }
                    else
                    {
                        dm_sync_connect_req(CSR_BT_CM_IFACEQUEUE,
                                            0, /* pv_cbarg */
                                            &theLogicalLink->deviceAddr,
                                            parms.txBandwidth,
                                            parms.rxBandwidth,
                                            parms.maxLatency,
                                            parms.voiceSettings,
                                            parms.reTxEffort,
                                            parms.audioQuality);
                    }
                }
                else
                { /* No more packets types to try */
                    CsrBtCmDmScoConnectCfmMsgSend(theLogicalLink->appHandle,
                                                  theLogicalLink->btConnId,
                                                  NULL,
                                                  0,
                                                  CSR_BT_RESULT_CODE_CM_SYNCHRONOUS_CONNECTION_ATTEMPT_FAILED,
                                                  CSR_BT_SUPPLIER_CM);
                    CsrBtCmDmLocalQueueHandler();
                }
            }
            else
            { /* The Sco handle is already reserve */
                CsrBtCmDmScoConnectCfmMsgSend(theLogicalLink->appHandle,
                                              theLogicalLink->btConnId,
                                              NULL,
                                              0,
                                              CSR_BT_RESULT_CODE_CM_SYNCHRONOUS_CONNECTION_ALREADY_EXISTS,
                                              CSR_BT_SUPPLIER_CM);
                CsrBtCmDmLocalQueueHandler();
            }
        }
        else
        {
            CsrBtCmDmLocalQueueHandler();
        }
    }
    else
    {
        CsrBtCmDmLocalQueueHandler();
    }

}
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

#ifdef CSR_BT_INSTALL_CM_PRI_ALWAYS_SUPPORT_MASTER_ROLE
void CsrBtCmAlwaysSupportMasterRoleReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmAlwaysSupportMasterRoleReq        *prim;

    prim = (CsrBtCmAlwaysSupportMasterRoleReq *) cmData->recvMsgP;

    cmData->roleVar.alwaysSupportMasterRole = prim->alwaysSupportMasterRole;
}
#endif

#ifdef CSR_BT_INSTALL_CM_ROLE_SWITCH_CONFIG
void CsrBtCmRoleSwitchConfigReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmRoleSwitchConfigReq* prim;

    prim = (CsrBtCmRoleSwitchConfigReq *) cmData->recvMsgP;

    switch (prim->config)
    {
        case CSR_BT_CM_ROLE_SWITCH_DEFAULT:
            cmData->roleVar.alwaysSupportMasterRole = FALSE;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            cmData->roleVar.doMssBeforeScoSetup     = TRUE;
#endif
            cmData->roleVar.doMssBeforeRnr          = FALSE;
            break;

        case CSR_BT_CM_ROLE_SWITCH_ALWAYS:
            cmData->roleVar.alwaysSupportMasterRole = TRUE;
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            cmData->roleVar.doMssBeforeScoSetup     = TRUE;
#endif
            break;

        case CSR_BT_CM_ROLE_SWITCH_BEFORE_SCO:
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            cmData->roleVar.doMssBeforeScoSetup     = TRUE;
#endif
            break;

        case CSR_BT_CM_ROLE_SWITCH_NOT_BEFORE_SCO:
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
            cmData->roleVar.doMssBeforeScoSetup     = FALSE;
#endif
            break;

        case CSR_BT_CM_ROLE_SWITCH_BEFORE_RNR:
            cmData->roleVar.doMssBeforeRnr          = TRUE;
            break;

        case CSR_BT_CM_ROLE_SWITCH_NOT_BEFORE_RNR:
            cmData->roleVar.doMssBeforeRnr          = FALSE;
            break;

        case CSR_BT_CM_ROLE_SWITCH_ALWAYS_ACL:
            cmData->roleVar.alwaysSupportMasterRole = TRUE;
            break;

        case CSR_BT_CM_ROLE_SWITCH_MULTIPLE_ACL:
            cmData->roleVar.alwaysSupportMasterRole = FALSE;
            break;

        default:
            CsrBtCmStateEventException(CSR_BT_CM_PRIM, prim->type, 0);
            break;
    }
}
#endif

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
CsrBool CsrBtCmRoleSwitchBeforeScoSetupNeeded(cmInstanceData_t *cmData)
{
    return cmData->roleVar.doMssBeforeScoSetup;
}
#endif

CsrBool CsrBtCmRoleSwitchAllowedByUpperLayer(aclTable *aclConnectionElement)
{
    aclRoleVars_t *roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

    if (!aclConnectionElement || roleVars->appHandle != CSR_SCHED_QID_INVALID)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
 
void CsrBtCmPrivateSwitchRoleReqSend(CsrBtDeviceAddr deviceAddr, CsrUint8 role)
{
    CsrBtCmPrivateSwitchRoleReq        *prim;

    prim              = (CsrBtCmPrivateSwitchRoleReq *) CsrPmemAlloc(sizeof(CsrBtCmPrivateSwitchRoleReq));
    prim->type        = CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ;
    prim->deviceAddr  = deviceAddr;
    prim->role        = role;
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, prim);
}

void CsrBtCmPrivateSwitchRoleReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmPrivateSwitchRoleReq * prim;
    aclTable                       *aclConnectionElement;

    prim = (CsrBtCmPrivateSwitchRoleReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);

    if (CsrBtCmBnepRoleSwitchAllowed(cmData) &&
        CsrBtCmRoleSwitchAllowedByUpperLayer(aclConnectionElement) &&
        aclConnectionElement)
    {
        /* Only allow role switch if we
         * 1) Don't have any BNEP connections on this bd_addr
         * 2) Upper layers haven't requested to control role */
        csrBtCmDmSwitchRoleReqSend(prim->deviceAddr, prim->role);
    }
    else
    {
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}

void CsrBtCmDmAclRoleVarsClear(aclRoleVars_t *roleVars)
{
    if (roleVars)
    {
        roleVars->role      = CSR_BT_UNDEFINED_ROLE;
        roleVars->roleType  = CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID;
        roleVars->appHandle = CSR_SCHED_QID_INVALID;
        roleVars->state     = CM_SWITCH_ROLE_IDLE;
    }
}

aclRoleVars_t* CsrBtCmDmGetAclRoleVars(aclTable *aclConnectionElement)
{
    return aclConnectionElement ? &aclConnectionElement->roleVars : NULL;
}

#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
static CsrBool csrBtCmDmAclRoleVarsRegisterAllowed(aclRoleVars_t        *roleVars, CsrSchedQid appHandle)
{
    return roleVars->appHandle == appHandle || roleVars->appHandle == CSR_SCHED_QID_INVALID;
}

static CsrBool csrBtCmDmAclRoleVarsRegisterApphandle(aclRoleVars_t        *roleVars, CsrSchedQid appHandle, CsrUint8 role, CsrBtCmRoleType roleType)
{
    if (csrBtCmDmAclRoleVarsRegisterAllowed(roleVars, appHandle))
    {
        roleVars->role      = role;
        roleVars->roleType  = roleType;
        roleVars->appHandle = appHandle;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void CsrBtCmSwitchRoleReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSwitchRoleReq *prim;
    aclTable             *aclConnectionElement;
    aclRoleVars_t        *roleVars;;

    prim = (CsrBtCmSwitchRoleReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

        if (prim->roleType != CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID && prim->role != CSR_BT_UNDEFINED_ROLE)
        { /* App is attempting to change role */
            if (CsrBtCmBnepRoleSwitchAllowed(cmData) &&
                csrBtCmDmAclRoleVarsRegisterApphandle(roleVars, prim->appHandle, prim->role, prim->roleType))
            {
                if (aclConnectionElement->role == prim->role)
                {
                    /* No need to change the actual role */
                    CsrBtCmRoleSwitchCfmSend(roleVars->appHandle,
                                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                                             CSR_BT_SUPPLIER_CM,
                                             aclConnectionElement->deviceAddr,
                                             aclConnectionElement->role,
                                             roleVars->roleType);
                    CsrBtCmServiceManagerLocalQueueHandler(cmData);
                }
                else
                {
                    cmData->smVar.appHandle = prim->appHandle;
                    CsrBtBdAddrCopy(&cmData->smVar.operatingBdAddr, &prim->deviceAddr);

                    if ((aclConnectionElement->actualLinkPolicySettings & ENABLE_MS_SWITCH) != 0)
                    {
                        /* RS supported. Go ahead and perform it */
                        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, CM_ROLE_SWITCH_PLAYER);
                        csrBtCmDmSwitchRoleReqSendExt(cmData->smVar.operatingBdAddr, roleVars->role, CM_ROLE_SWITCH_PLAYER);
                    }
                    else
                    {
                        /* Role needs to be switched. Ensure that we support it before attempting it */
                        CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_ALLOW_MSS);
                        CsrBtCmDmWriteLpSettingsReqMsgSendExt(prim->deviceAddr,
                                                              CM_ROLE_SWITCH_PLAYER,
                                                              ENABLE_MS_SWITCH);
                    }
                }
            }
            else
            {
                CsrBtCmRoleSwitchCfmSend(prim->appHandle,
                                         CSR_BT_RESULT_CODE_CM_COMMAND_DISALLOWED,
                                         CSR_BT_SUPPLIER_CM,
                                         aclConnectionElement->deviceAddr,
                                         CSR_BT_UNDEFINED_ROLE,
                                         CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
            }
        }
        else
        { /* App is attempting to release control of the given BD_ADDR */
            CsrBtCmRoleSwitchCfmSend(prim->appHandle,
                                     CSR_BT_RESULT_CODE_CM_SUCCESS,
                                     CSR_BT_SUPPLIER_CM,
                                     aclConnectionElement->deviceAddr,
                                     CSR_BT_UNDEFINED_ROLE,
                                     CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
            CsrBtCmServiceManagerLocalQueueHandler(cmData);
            CsrBtCmDmAclRoleVarsClear(roleVars);
        }
    }
    else
    {
        /* No ACL. Restore queue */
        CsrBtCmRoleSwitchCfmSend(prim->appHandle,
                                 CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                 CSR_BT_SUPPLIER_CM,
                                 prim->deviceAddr,
                                 CSR_BT_UNDEFINED_ROLE,
                                 CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
    }
}
#endif
void CsrBtCmDmRoleDiscoveryReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmRoleDiscoveryReq    *cmPrim;

    cmPrim                        = (CsrBtCmRoleDiscoveryReq *) cmData->recvMsgP;

    if (cmPrim->phandle == CSR_BT_CM_IFACEQUEUE)
    {
        dm_hci_role_discovery(&cmPrim->deviceAddr, NULL);
    }
    else
    {
        aclTable                       *aclConnectionElement;

        CsrBtCmRoleDiscoveryCfm     *prim;

        prim = (CsrBtCmRoleDiscoveryCfm *)CsrPmemAlloc(sizeof(CsrBtCmRoleDiscoveryCfm));

        returnAclConnectionElement(cmData, cmPrim->deviceAddr, &aclConnectionElement);

        prim->type                    = CSR_BT_CM_ROLE_DISCOVERY_CFM;
        prim->deviceAddr            = cmPrim->deviceAddr;
        if (aclConnectionElement)
        {
            prim->role = aclConnectionElement->role;
        }
        else
        {
            prim->role = CSR_BT_UNDEFINED_ROLE;
        }
        CsrBtCmPutMessage(cmPrim->phandle, prim);
        CsrBtCmDmLocalQueueHandler();
    }
}

void CsrBtCmDmHciRoleDiscoveryCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_ROLE_DISCOVERY_CFM_T *prim;

    prim = (DM_HCI_ROLE_DISCOVERY_CFM_T *) cmData->recvMsgP;

    if (prim->status == HCI_SUCCESS)
    {
        aclTable           *aclConnectionElement;

        returnAclConnectionElement(cmData, prim->bd_addr, &aclConnectionElement);

        if (aclConnectionElement)
        {
            aclConnectionElement->role = prim->role;
#if (CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT != CSR_BT_HCI_DEFAULT_LSTO)
            if (prim->role == CSR_BT_MASTER_ROLE)
            {
                CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmData, CSR_BT_CM_IFACEQUEUE, CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT, prim->bd_addr);
            }
#endif
        }
    }
    else
    {
        ;
    }
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmDmModeChangeRoleSwitchHandler(cmInstanceData_t *cmData, CsrBtDeviceAddr deviceAddr)
{
    cmData->dmVar.roleSwitchLinkMode = CsrBtCmDmReturnActualMode(cmData, deviceAddr);

    switch (cmData->dmVar.roleSwitchLinkMode)
    {
        case CSR_BT_ACTIVE_MODE:
            { /* Make sure that only role switch is enable */
                cmData->roleVar.roleSwitchFlag = TRUE;
                CsrBtCmDmWriteLpSettingsForceLinkPolicyMsgSend(cmData, deviceAddr, CM_PLAYER, ENABLE_MS_SWITCH);
                break;
            }
        case CSR_BT_SNIFF_MODE:
            { /* Must exit Sniff mode before a role switch can be performed */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_ROLE_SWITCH_STATE);
                cmData->smVar.operatingBdAddr   = deviceAddr;
                cmData->dmVar.requestedLinkMode = CSR_BT_ACTIVE_MODE;
                dm_hci_exit_sniff_mode(&(deviceAddr), NULL);
                break;
            }
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
        case  CSR_BT_PARK_MODE:
            { /* Must exit Park mode before a role switch can be performed */
                CSR_BT_CM_STATE_CHANGE(cmData->dmVar.linkPolicyState, LINK_POLICY_ROLE_SWITCH_STATE);
                cmData->smVar.operatingBdAddr   = deviceAddr;
                cmData->dmVar.requestedLinkMode = CSR_BT_ACTIVE_MODE;
                dm_hci_exit_park_mode(&(deviceAddr), NULL);
                break;
            }
#endif
        default :
            { /* An error has occurred make sure that the queues are restored */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }
    }
}

static void csrBtCmDmSwitchRoleReqRoleSwitchPlayerHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmSwitchRoleReq *prim;
    aclTable   * aclConnectionElement;
    aclRoleVars_t *roleVars;

    prim = (CsrBtCmDmSwitchRoleReq *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);
    roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

    if (aclConnectionElement)
    {
        CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_DO);
        dm_hci_switch_role(&cmData->smVar.operatingBdAddr, prim->role, NULL);
        cmData->dmVar.appHandle = cmData->smVar.appHandle;
        cmData->dmVar.roleSwitchInProgress = TRUE;
        cmData->dmVar.switchRoleDeviceAddr = prim->deviceAddr;
    }
    else
    {
        CsrBtCmRoleSwitchCfmSend(cmData->smVar.appHandle,
                                 CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER,
                                 CSR_BT_SUPPLIER_CM,
                                 cmData->smVar.operatingBdAddr,
                                 CSR_BT_UNDEFINED_ROLE,
                                 CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        CsrBtCmDmAclRoleVarsClear(roleVars);
    }
}

void cmDmAclKickRoleSwitch(cmInstanceData_t *cmData)
{
    aclTable *aclConnectionElement;

    returnNextAvailableAclConnectionElement(cmData, &aclConnectionElement);

    if (aclConnectionElement)
    {
        if (cmData->roleVar.alwaysSupportMasterRole)
        {
            CsrBtCmPrivateSwitchRoleReqSend(aclConnectionElement->deviceAddr, CSR_BT_MASTER_ROLE);
        }
        else
        {
            CsrUint8 numOfAcl = returnNumOfAclConnection(cmData);

            if (numOfAcl > 1)
            {
                CsrBtCmPrivateSwitchRoleReqSend(aclConnectionElement->deviceAddr, CSR_BT_MASTER_ROLE);
            }
        }
    }
}

static void csrBtCmDmSwitchRoleReqDefaultPlayerHandler(cmInstanceData_t *cmData)
{ /* The CM has requested a RS */
    CsrBtCmDmSwitchRoleReq        * prim;

    CsrUint8     currentRole;
    aclTable   * aclConnectionElement;

    CsrUint8 numOfAcl                    = returnNumOfAclConnection(cmData);
    prim                                = (CsrBtCmDmSwitchRoleReq *) cmData->recvMsgP;
    cmData->dmVar.roleSwitchLinkMode    = CSR_BT_ACTIVE_MODE;
    cmData->roleVar.requestedRole       = prim->role;
    CsrBtCmDmClearRoleCheckedFlag(cmData);
    returnAclConnectionElement(cmData, prim->deviceAddr, &aclConnectionElement);

    if (aclConnectionElement)
    { /* The Role is know */
        currentRole = aclConnectionElement->role;
    }
    else
    { /* The Role is unknown */
        currentRole = CSR_BT_UNDEFINED_ROLE;
    }

    if (cmData->roleVar.requestedRole == CSR_BT_UNDEFINED_ROLE)
    {
        cmData->roleVar.roleSwitchPerformed = FALSE;
    }
    else
    {
        cmData->roleVar.roleSwitchPerformed = TRUE;
    }

    switch (numOfAcl)
    {
        case 0:
            { /* The local device has no ACL connection therefor a role switch cannot be
                 performed. Just restore the queues */
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                break;
            }

        case 1:
            { /* The local device has 1 ACL connection. */
                if (!cmData->roleVar.alwaysSupportMasterRole)
                {
                    if (currentRole == cmData->roleVar.requestedRole || cmData->roleVar.requestedRole == CSR_BT_UNDEFINED_ROLE)
                    { /* The role is right just make sure the MSS is supported */
                        csrBtCmDmWriteLpSettingsRoleSwitchMsgSend(cmData, prim->deviceAddr, ENABLE_MS_SWITCH, currentRole);
                    }
                    else
                    { /* Need to change make a role switch */
                        CsrBtCmDmModeChangeRoleSwitchHandler(cmData, prim->deviceAddr);
                    }
                }
                else
                { /* Synergy BT must try to be master even if it only got one ACL connection */
                    csrBtCmDmSwitchRoleMoreThanOneAclHandler(cmData, aclConnectionElement, currentRole, prim->deviceAddr);
                }
                break;
            }

        default:
            { /* The local device has more than 1 ACL connection. */
                csrBtCmDmSwitchRoleMoreThanOneAclHandler(cmData, aclConnectionElement, currentRole, prim->deviceAddr);
                break;
            }
    }
}

void CsrBtCmDmSwitchRoleReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDmSwitchRoleReq *prim;

    prim = (CsrBtCmDmSwitchRoleReq *) cmData->recvMsgP;

    switch(prim->activePlayer)
    {
        case CM_ROLE_SWITCH_PLAYER:
            csrBtCmDmSwitchRoleReqRoleSwitchPlayerHandler(cmData);
            break;

        default:
            CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, prim->activePlayer);
            csrBtCmDmSwitchRoleReqDefaultPlayerHandler(cmData);
            break;
    }
}

static void csrBtCmDmHciSwitchRoleCompleteRoleSwitchPlayerHandler(cmInstanceData_t *cmData)
{
    DM_HCI_SWITCH_ROLE_CFM_T * dmPrim;
    aclTable *aclConnectionElement;
    aclRoleVars_t *roleVars;

    dmPrim = (DM_HCI_SWITCH_ROLE_CFM_T *) cmData->recvMsgP;

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);
    roleVars = CsrBtCmDmGetAclRoleVars(aclConnectionElement);

    if (dmPrim->status == HCI_SUCCESS && aclConnectionElement)
    {
        switch(roleVars->state)
        {
            case CM_SWITCH_ROLE_RESTORE_MSS:
                /* The peer device RS again before we could disable it via the LP settings.
                   Just move along. We can't do anything about this */
                break;

            case CM_SWITCH_ROLE_DO:
                if (roleVars->role == dmPrim->role)
                { /* We only do role switch when we have to. This implies that if we get here
                     that the requested role was obtained */
                    link_policy_settings_t linkPolicySettings;

                    linkPolicySettings = CsrBtCmDmReturnLinkPolicySettings(cmData, cmData->smVar.operatingBdAddr);

                    if ((linkPolicySettings & ENABLE_MS_SWITCH) != 0)
                    { /* MSS switch was requested by profile so we don't need to disable it */
                        CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_IDLE);
                        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);
                        CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                                 CSR_BT_RESULT_CODE_CM_SUCCESS,
                                                 CSR_BT_SUPPLIER_CM,
                                                 cmData->smVar.operatingBdAddr,
                                                 aclConnectionElement->role,
                                                 roleVars->roleType);
                        CsrBtCmServiceManagerLocalQueueHandler(cmData);
                        CsrBtCmDmLocalQueueHandler();
                    }
                    else
                    {
                        CSR_BT_CM_STATE_CHANGE(roleVars->state, CM_SWITCH_ROLE_RESTORE_MSS);
                        CsrBtCmDmWriteLpSettingsOverrideMssMsgSend(cmData,
                                                                   cmData->smVar.operatingBdAddr,
                                                                   CM_ROLE_SWITCH_PLAYER,
                                                                   linkPolicySettings);
                    }
                    break;
                }
                /* !!Fall-through !!*/
            case CM_SWITCH_ROLE_ALLOW_MSS:
            default:
                CsrBtCmStateEventException(DM_PRIM, dmPrim->type, roleVars->state);
                CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);
                CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                         CSR_BT_RESULT_CODE_CM_INTERNAL_ERROR,
                                         CSR_BT_SUPPLIER_CM,
                                         cmData->smVar.operatingBdAddr,
                                         CSR_BT_UNDEFINED_ROLE,
                                         CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);
                CsrBtCmDmLocalQueueHandler();
                CsrBtCmServiceManagerLocalQueueHandler(cmData);
                CsrBtCmDmAclRoleVarsClear(roleVars);
                break;
        }
    }
    else
    {
        CsrBtResultCode     resultCode;
        CsrBtSupplier resultSupplier;

        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, UNDEFINED_PLAYER);

        if (dmPrim->status == HCI_SUCCESS)
        {
            resultSupplier = CSR_BT_SUPPLIER_CM;
            resultCode     = CSR_BT_RESULT_CODE_CM_UNKNOWN_CONNECTION_IDENTIFIER;
        }
        else
        {
            resultSupplier = CSR_BT_SUPPLIER_HCI;
            resultCode     =(CsrBtResultCode) dmPrim->status;
        }

        CsrBtCmRoleSwitchCfmSend(cmData->dmVar.appHandle,
                                 resultCode,
                                 resultSupplier,
                                 cmData->smVar.operatingBdAddr,
                                 CSR_BT_UNDEFINED_ROLE,
                                 CSR_BT_CM_SWITCH_ROLE_TYPE_INVALID);

        CsrBtCmDmLocalQueueHandler();
        CsrBtCmServiceManagerLocalQueueHandler(cmData);
        CsrBtCmDmAclRoleVarsClear(roleVars);
    }
}

CsrUint8 CsrBtCmDmAclReturnNextMarkedForMss(cmInstanceData_t *cmData, aclTable **aclConnectionElement)
{
    CsrUintFast8 i;

    for ( i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr) &&
            cmData->roleVar.aclVar[i].rnrWorkaround.roleSwitchNeeded)
        {
            *aclConnectionElement = &cmData->roleVar.aclVar[i];
            return (CsrUint8)i;
        }
    }

    *aclConnectionElement = NULL;
    return CM_ERROR;
}

CsrUint8 CsrBtCmDmAclMarkForMss(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr)
{
    CsrUintFast8 i;
    CsrUint8 ret = CM_ERROR;

    for (i = 0; i < NUM_OF_ACL_CONNECTION; i++)
    {
        if (!CsrBtBdAddrEqZero(&cmData->roleVar.aclVar[i].deviceAddr) &&
            !CsrBtBdAddrEq(&cmData->roleVar.aclVar[i].deviceAddr, deviceAddr))
        {
            if (cmData->roleVar.aclVar[i].role == CSR_BT_SLAVE_ROLE &&
                cmData->roleVar.aclVar[i].mode != CSR_BT_SNIFF_MODE 
#ifdef CSR_BT_INSTALL_CM_PARK_MODE
                && cmData->roleVar.aclVar[i].mode != CSR_BT_PARK_MODE
#endif
                )
            {
                cmData->roleVar.aclVar[i].rnrWorkaround.roleSwitchNeeded = TRUE;
                ret = (CsrUint8)i;
            }
            else
            {
                cmData->roleVar.aclVar[i].rnrWorkaround.roleSwitchNeeded = FALSE;
            }
        }
    }
    return ret;
}

void CsrBtCmDmAclProcessNextMarkedForMss(cmInstanceData_t *cmData)
{
    aclTable *aclConnectionElement;

    if (CsrBtCmDmAclReturnNextMarkedForMss(cmData, &aclConnectionElement) != CM_ERROR)
    {
        /* This isn't quite safe since we haven't locked the SM queue */
        CM_CHANGE_PLAYER(&cmData->smVar.activePlayer, RNR_PLAYER);
        cmData->dmVar.roleSwitchInProgress = TRUE;
        cmData->dmVar.switchRoleDeviceAddr = aclConnectionElement->deviceAddr;
        dm_hci_switch_role(&aclConnectionElement->deviceAddr, CSR_BT_MASTER_ROLE, NULL);
    }
    else
    {
        CsrBtCmReadRemoteNameSetup(cmData);
    }
}

static void csrBtCmDmHciSwitchRoleCompleteRnrPlayerHandler(cmInstanceData_t *cmData, aclTable *aclConnectionElement)
{
    if (aclConnectionElement)
    {
        aclConnectionElement->rnrWorkaround.roleSwitchNeeded = FALSE;
    }
    CsrBtCmDmAclProcessNextMarkedForMss(cmData);
}

void CsrBtCmDmHciSwitchRoleCompleteHandler(cmInstanceData_t *cmData)
{ /* A role change event has occurred. */
    aclTable                 *aclConnectionElement;
    DM_HCI_SWITCH_ROLE_CFM_T *dmPrim = (DM_HCI_SWITCH_ROLE_CFM_T *) cmData->recvMsgP;

#if (CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT != CSR_BT_HCI_DEFAULT_LSTO)
    if (dmPrim->status == HCI_SUCCESS && dmPrim->role == CSR_BT_MASTER_ROLE)
    {
        CsrBtCmWriteDmLinkSuperVisionTimeoutHandler(cmData, CSR_BT_CM_IFACEQUEUE, CSR_BT_DEFAULT_LINK_SUPERVISION_TIMEOUT, dmPrim->bd_addr);
    }
#endif

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE
    if (dmPrim->status == HCI_SUCCESS)
    { /* A role change event has occurred. Inform the involved profiles */
        informProfilesAboutRoleSwitch(cmData, dmPrim->role, dmPrim->bd_addr);
        cmData->bnepVar.roleSwitchReqIndex = CM_ERROR;
    }
    else
    { /* The request af changing the role failed.
         Inform the profile that made the request about the error */
        informProfileAboutRoleSwitchError(cmData, dmPrim->role, dmPrim->bd_addr,
                                          (CsrBtResultCode) dmPrim->status, CSR_BT_SUPPLIER_HCI);
    }
#endif /* EXCLUDE_CSR_BT_BNEP_MODULE */

    returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

    if (aclConnectionElement)
    {
        aclConnectionElement->role = dmPrim->role;
    }

    if (cmData->dmVar.roleSwitchInProgress &&
        bd_addr_eq(&dmPrim->bd_addr, &cmData->dmVar.switchRoleDeviceAddr))
    { /* The local device has requested a role switch. */
        cmData->dmVar.roleSwitchInProgress = FALSE;
        cmData->roleVar.roleSwitchFlag     = FALSE;

        switch (cmData->smVar.activePlayer)
        {
#ifndef EXCLUDE_CSR_BT_SCO_MODULE
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
            case RFC_PLAYER:
                {
                    cmRfcConnElement * theElement = CM_FIND_RFC_ELEMENT(CsrBtCmRfcFindRfcConnElementFromIndex, &(cmData->dmVar.contableIndex));

                    if (theElement)
                    {
                        cmData->dmVar.scoConnectAddr = dmPrim->bd_addr;

                        if (theElement->cmRfcConnInst->eScoParms.pcmSlot != CSR_BT_PCM_DONT_MAP)
                        {
                            CsrBtCmBccmdMapScoPcmReqSend(theElement->cmRfcConnInst->eScoParms.pcmSlot, RFC_SCO_CONNECT);
                        }
                        else
                        { /* Skip map SCO over PCM and go directly for the SCO setup procedure */
                            theElement->cmRfcConnInst->eScoParms.pcmSlot = 0;
                            CsrBtCmRfcScoConnectReqHandler(cmData, cmData->dmVar.scoConnectAddr);
                        }
                    }
                    else
                    {
                        CsrBtCmDmLocalQueueHandler();
                    }
                    break;
                }
#endif /* EXCLUDE_CSR_BT_RFC_MODULE */
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

            case CM_ROLE_SWITCH_PLAYER:
                csrBtCmDmHciSwitchRoleCompleteRoleSwitchPlayerHandler(cmData);
                break;

            case RNR_PLAYER:
                csrBtCmDmHciSwitchRoleCompleteRnrPlayerHandler(cmData, aclConnectionElement);
                break;

            default :
                {
                    if (aclConnectionElement)
                    {
                        aclConnectionElement->roleChecked = TRUE;
                    }

                    CsrBtCmDmWriteLpSettingsAllMsgSend(cmData, dmPrim->bd_addr, KEEP_CURRENT_PLAYER);
                    break;
                }
        }
    }
}

