/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_env_prim.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_dm_sc_lib.h"
#include "csr_bt_cm_dm_sc_ssp_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"

#include "csr_bt_sc_main.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_private_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_bond_remote.h"
#include "csr_bt_sc_bond_local.h"
#include "csr_bt_sc_bond_unified.h"
#include "csr_bt_sc_bond_legacy.h"
#include "csr_bt_sc_bondable_mode.h"
#include "csr_bt_sc_utils.h"
#include "csr_bt_sc_authenticate.h"
#include "csr_bt_sc_encrypt.h"
#include "csr_bt_sc_debond.h"
#include "csr_bt_sc_register.h"
#include "csr_bt_sc_authorise.h"
#include "csr_bt_sc_ui.h"
#include "csr_bt_sc_null.h"
#include "csr_bt_sc_rebond.h"
#ifdef CSR_BT_INSTALL_SC_OOB
#include "csr_bt_sc_oob.h"
#endif
#include "csr_bt_sc_le.h"


#ifdef CSR_LOG_ENABLE
static const CsrLogSubOrigin subOrigins[] =
{
    {LOG_TEXT_SO_STATE, "STATE"},
    {LOG_TEXT_SO_SCDB,  "SCDB"},
};
#endif /* CSR_LOG_ENABLE */

void CsrBtScInit(void **gash)
{
    ScInstanceData_t    *scData;

#ifdef CSR_LOG_ENABLE
    CsrLogTextRegister(LOG_TEXT_TASK_ID, "SC", CSR_ARRAY_SIZE(subOrigins), subOrigins);
#endif /* CSR_LOG_ENABLE */

    /*  allocate and initialise instance data space */
    *gash                        = (void *) CsrPmemZalloc(sizeof(ScInstanceData_t));
    scData                       = (ScInstanceData_t *) *gash;
    scData->eventMask            = CSR_BT_SC_DEFAULT_EVENT_MASK;
    scData->appTimeout           = CSR_BT_SC_PASSKEY_RESPONSE_TIMEOUT*1000;
#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
    scData->configMask           = CSR_BT_SC_CONFIG_MASK_USE_STANDARD;
#endif
    scData->bondPhandle          = CSR_BT_APPLICATION_SECURITY_HANDLER;
    scData->cancelBondResult     = CSR_BT_RESULT_CODE_SC_SUCCESS;

    scData->securityMode              = CSR_BT_DEFAULT_SEC_MODE;
    scData->encryptionMode            = CSR_BT_DEFAULT_ENC_MODE3;
    scData->mainAppHandle             = CSR_BT_APPLICATION_SECURITY_HANDLER;
    scData->bondData.localIoCaps      = CSR_BT_DEFAULT_IO_CAPABILITY;
    scData->bondData.localAuth        = CSR_BT_DEFAULT_AUTH_REQUIREMENTS;
    scData->bondData.defaultBondable  = CSR_BT_DEFAULT_SC_MODE;
    scData->bondData.bondable         = CSR_BT_DEFAULT_SC_MODE;

    /* And register ourselves with the CM in order to receive all SC
     * related messages */
    CsrBtCmRegisterHandlerReqSend(CSR_BT_CM_HANDLER_TYPE_SC,
                                  CSR_BT_SC_IFACEQUEUE,
                                  0); /* flags */

    /* Automatically fallback to legacy mode if required.
     * Disable DM automatic response and do it ourselves. */
    scData->sspData.config            = (DM_SM_SEC_MODE_CONFIG_LEGACY_AUTO_PAIR_MISSING_LINK_KEY |
                                         DM_SM_SEC_MODE_CONFIG_NEVER_AUTO_ACCEPT);

    /* Debug mode can always be enabled - this define controls whether
     * it's enabled by default */
#if defined(CSR_BT_SC_ALLOW_SSP_DEBUG_MODE) && (CSR_BT_SC_ALLOW_SSP_DEBUG_MODE == 1)
    scData->sspData.config           |= DM_SM_SEC_MODE_CONFIG_DEBUG_MODE;
#endif

    scData->sspData.writeAuthEnable   = DM_SM_WAE_ACL_OWNER_NONE;

    CsrBtScResetInstData(scData);
    SC_CHANGE_STATE(&scData->state, SC_ST_NULL);

#ifdef CSR_BT_LE_ENABLE
    scData->bondData.leDefaultLocalAuth = CSR_BT_SC_AUTH_REQ_DEFAULT;
    scData->bondData.leLocalAuth = CSR_BT_SC_AUTH_REQ_DEFAULT;
    scData->bondData.keyDistribution = CSR_BT_SC_KEY_DIST_DEFAULT;
    scData->gattId = CSR_BT_GATT_INVALID_GATT_ID;
    CsrBtGattRegisterReqSend(CSR_BT_SC_IFACEQUEUE,
                             0); /* context, not used */
#endif /* CSR_BT_LE_ENABLE */

    /* The SC must wait for a CSR_BT_CM_GET_SECURITY_CONF_IND before
     * it can start the final init procedure  */
}

#ifdef ENABLE_SHUTDOWN
void CsrBtScDeinit(void **gash)
{
    ScInstanceData_t *scData;
    CsrUint16         eventClass=0;
    void *           msg=NULL;
    CsrBool           lastMsg;

    scData = (ScInstanceData_t *) (*gash);
    lastMsg = TRUE;

    while (lastMsg)
    {
        if ((lastMsg = CsrMessageQueuePop(&scData->saveQueue, &eventClass, &msg)) == TRUE ||
            (lastMsg = CsrMessageQueuePop(&scData->ignoreQueue, &eventClass, &msg)) == TRUE ||
            (lastMsg = CsrSchedMessageGet(&eventClass , &msg)) == TRUE)
        {
            switch (eventClass)
            {
                case DM_PRIM:
                {
                    dm_free_upstream_primitive(msg);
                    msg = NULL;
                    break;
                }
                case CSR_BT_SD_PRIM:
                {
                    CsrBtSdFreeUpstreamMessageContents(eventClass, msg);
                    CsrBtSdPrivateFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }
#ifdef CSR_BT_LE_ENABLE
                case CSR_BT_GATT_PRIM:
                {
                    CsrBtGattFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }
#endif
                default:
                {
                    /* Only SD primitives contain pointers to allocated data - ignore */
                    break;
                }
            }
            CsrPmemFree(msg);
        }
    }

    /* Stop timers */
    CsrBtScStopBondTimer(scData);
#ifdef CSR_BT_INSTALL_SC_MODE
    CsrBtScStopBondableTimer(scData);
#endif

    if (scData->rfcommServices)
    {
        ScServiceRecordType * currentElement;
        ScServiceRecordType * nextElement;

        nextElement     = scData->rfcommServices;
        do
        {
            currentElement = nextElement;
            nextElement = currentElement->nextQueuePool;
            CsrPmemFree(currentElement);
        } while (nextElement);
    }

    if (scData->l2capServices)
    {
        ScServiceRecordType * currentElement;
        ScServiceRecordType * nextElement;

        nextElement     = scData->l2capServices;
        do
        {
            currentElement = nextElement;
            nextElement = currentElement->nextQueuePool;
            CsrPmemFree(currentElement);
        } while (nextElement);
    }

    /* Free LE keys */
#ifdef CSR_BT_LE_ENABLE
    CsrPmemFree(scData->bondData.leKeyEncCentral);
    CsrPmemFree(scData->bondData.leKeyId);
    CsrPmemFree(scData->bondData.leKeySign);

    while(scData->leLinks)
    {
        CsrBtScLeLinks *n = scData->leLinks->next;
        CsrPmemFree(scData->leLinks);
        scData->leLinks = n;
    }
#endif

    CsrPmemFree(scData);
}
#endif

static void CsrBtScScHandler(ScInstanceData_t *scData)
{
    switch(*(CsrUint16*)scData->recvMsgP)
    {
        case CSR_BT_SC_REGISTER_REQ:
            {
                CsrBtScRegisterReqAnyS(scData);
                break;
            }
        case CSR_BT_SC_BOND_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScBondReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
        case CSR_BT_SC_DEBOND_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScDebondReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
        case CSR_BT_SC_PASSKEY_RES:
            {

                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                   || scData->state == SC_ST_AUTHENTICATE
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION                                        
                   || scData->state == SC_ST_ENCRYPT
#endif
                    )
                {
                    CsrBtScPasskeyResLocalBondingS(scData); /* same as CsrBtScPasskeyResAuthenticateS*/
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScPasskeyResRemoteBondingS(scData);
                }
                else /* all other states */
                {
                    CsrBtScIgnoreAnyS(scData);
                }
                break;
            }
        case CSR_BT_SC_AUTHORISE_RES:
            {
                if(scData->state == SC_ST_AUTHORISATION)
                {
                    CsrBtScAuthoriseResAuthoriseS(scData);
                }
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }
                break;
            }
        case CSR_BT_SC_SSP_PASSKEY_RES:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScSspPasskeyResLocalBondingS(scData);
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspPasskeyResRemoteBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspPasskeyResAuthenticateS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspPasskeyResAuthenticateS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }
                break;
            }
        case CSR_BT_SC_SSP_NOTIFICATION_RES:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScSspNotificationResLocalBondingS(scData);
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspNotificationResRemoteBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
               else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspNotificationResAuthenticateS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
               else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspNotificationResAuthenticateS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }

                break;
            }
        case CSR_BT_SC_SSP_COMPARE_RES:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScSspCompareResLocalBondingS(scData);
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspCompareResRemoteBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspCompareResAuthenticateS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspCompareResAuthenticateS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }

                break;
            }
        case CSR_BT_SC_SSP_JUSTWORKS_RES:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScSspJustWorksResLocalBondingS(scData);
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspJustWorksResRemoteBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspJustWorksAuthenticateS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspJustWorksAuthenticateS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }

                break;
            }
        case CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING||scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspKeypressNotificationReqBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspKeypressNotificationReqBondingS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspKeypressNotificationReqBondingS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }

                break;
            }
        case CSR_BT_SC_REBOND_RES:
            {
                if(scData->state == SC_ST_REBOND)
                {
                    CsrBtScRebondResHandlerRebondS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScIgnoreAnyS(scData);
                }
                break;
            }

        case CSR_BT_SC_SSP_PAIRING_RES:
            {
                if(scData->state == SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScSspPairingResLocalBondingS(scData);
                }
                else if(scData->state == SC_ST_REMOTE_BONDING)
                {
                    CsrBtScSspPairingResRemoteBondingS(scData);
                }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                else if(scData->state == SC_ST_AUTHENTICATE)
                {
                    CsrBtScSspPairingResAuthenticateS(scData);
                }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                else if(scData->state == SC_ST_ENCRYPT)
                {
                    CsrBtScSspPairingResAuthenticateS(scData);
                }
#endif
                else
                {
                    CsrBtScIgnoreAnyS(scData);
                }

                break;
            }

        case CSR_BT_SC_HOUSE_CLEANING:       /* Fall-through */
#ifndef CSR_BT_INSTALL_SC_ENCRYPTION
        case CSR_BT_SC_ENCRYPTION_REQ:       /* Fall-through */
#endif
            {
                CsrBtScIgnoreAnyS(scData);
                break;
            }
#ifdef CSR_BT_INSTALL_SC_SECURITY_MODE
        case CSR_BT_SC_SET_SECURITY_MODE_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScSetSecurityModeReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
        case CSR_BT_SC_ENCRYPTION_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScEncryptionReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
#endif
        case CSR_BT_SC_DEREGISTER_REQ:
            {
                if(scData->state != SC_ST_NULL)
                {
                    CsrBtScDeregisterReqAnyS(scData);
                }
                else
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
        case CSR_BT_SC_ACTIVATE_REQ:
            {
                CsrBtScActivateReqAnyS(scData);
                break;
            }
#ifdef CSR_BT_INSTALL_SC_TRUST_LEVEL
        case CSR_BT_SC_SET_TRUST_LEVEL_REQ:
            {
                CsrBtScSetTrustLevelReqAnyS(scData);
                break;
            }
#endif
        case CSR_BT_SC_READ_DEVICE_RECORD_REQ:
            {
                CsrBtScReadDeviceRecordReqHandler(scData);
                break;
            }
        case CSR_BT_SC_UPDATE_DEVICE_REQ:
            {
                CsrBtScUpdateDeviceReqHandler(scData);
                break;
            }
        case CSR_BT_SC_CANCEL_BOND_REQ:
            {
                if(scData->state != SC_ST_LOCAL_POST_LISBON_BONDING)
                {
                    CsrBtScCancelBondReqAnyS(scData);
                }
                else
                {
                    CsrBtScCancelBondReqBondingPostLisbonS(scData);
                }
                break;
            }
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
        case CSR_BT_SC_AUTHENTICATE_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScAuthenticateReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_SET_IO_CAPABILITY
        case CSR_BT_SC_SET_IO_CAPABILITY_REQ:
            {
                CsrBtScSetIoCapabilityReqHandler(scData);
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_MODE
        case CSR_BT_SC_MODE_REQ:
            {
                if(scData->state == SC_ST_IDLE||scData->state == SC_ST_NULL)
                {
                    CsrBtScModeReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_OOB
        case CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScReadLocalOobDataReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
        case CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ:
            {
                CsrBtScAddRemoteOobDataReqHandler(scData);
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_SET_AUTH_REQUIREMENTS
        case CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ:
            {
                CsrBtScSetAuthRequirementsReqHandler(scData);
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_SET_EVENT_MASK
        case CSR_BT_SC_SET_EVENT_MASK_REQ:
            {
                CsrBtScSetEventMaskAnyS(scData);
                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_DEBUG_MODE
        case CSR_BT_SC_DEBUG_MODE_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScDebugModeReqIdleS(scData);
                }
                else /* all states but IDLE */
                {
                    CsrBtScSaveMessage(scData);
                }

                break;
            }
#endif
#ifdef CSR_BT_INSTALL_SC_CONFIG_MASK
        case CSR_BT_SC_CONFIG_REQ:
            {
                CsrBtScConfigReqHandler(scData);
                break;
            }
#endif
        case CSR_BT_SC_DATABASE_REQ:
            {
                CsrBtScDatabaseReqHandler(scData);
                break;
            }
        case CSR_BT_SC_ACCEPT_BOND_REQ:
            {
                CsrBtScAcceptBondReqHandler(scData);
                break;
            }
        case CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ:
            {
                CsrBtScCancelAcceptBondReqHandler(scData);
                break;
            }
        case CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ:
            {
                CsrBtScEncryptionKeySizeReqHandler(scData);
                break;
            }
#ifdef CSR_BT_LE_ENABLE
#ifdef CSR_BT_INSTALL_SC_LE_KEY_DISTRIBUTION
        case CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ:
            {
                CsrBtScLeKeyDistributionReqHandler(scData);
                break;
            }
#endif
        case CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ:
            {
                CsrBtScSetEncryptionKeySizeReqHandler(scData);
                break;
            }
        case CSR_BT_SC_LE_SECURITY_REQ:
            {
                if(scData->state == SC_ST_IDLE)
                {
                    CsrBtScLeSecurityReqHandler(scData);
                }
                else
                {
                    CsrBtScSaveMessage(scData);
                }
                break;
            }
        case CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ:
            {
                CsrBtScLeAddServiceChangedReqHandler(scData);
                break;
            }
#endif
        default:
            {
                CsrBtScStateEventException(CSR_BT_SC_PRIM, *((CsrPrim *)scData->recvMsgP), scData->state);
            }
    }

}


void CsrBtScHandler(void **gash)
{
    ScInstanceData_t  *scData;
    CsrUint16          eventClass=0;
    CsrPrim *    msg=NULL;

    scData = (ScInstanceData_t *) (*gash);

    if(scData->restoreMsgFlag == FALSE)
    {
        CsrSchedMessageGet(&eventClass, (void *) &msg);
        /* For backward compability */
        CsrBtScDmSmLocalUserConfirmationRequestIndMap(scData, eventClass, msg);

        /* Should we ignore this message? */
        if (CsrBtScIgnoreMessage(&scData->ignoreQueue, eventClass, *msg))
        {
            switch (eventClass)
            {
                case CSR_BT_SD_PRIM:
                {
                    CsrBtSdFreeUpstreamMessageContents(eventClass, msg);
                    CsrBtSdPrivateFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }

                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }

                default:
                {
                    break;
                }
            }
            CsrPmemFree(msg);
            return;
        }
    }
    else
    {
        if(!CsrMessageQueuePop(&scData->saveQueue, &eventClass, (void *) &msg))
        {
            scData->restoreMsgFlag = FALSE;
            CsrSchedMessageGet(&eventClass, (void *) &msg);
        }
    }

    scData->recvMsgP   = msg;

    switch(eventClass)
    {
#ifdef CSR_BT_LE_ENABLE
        case CSR_BT_GATT_PRIM:
            {
                CsrBtScGattHandler(scData);
                CsrBtGattFreeUpstreamMessageContents(eventClass, msg);
                break;
            }
#endif
        case CSR_BT_SC_PRIM:
        {
            CsrBtScScHandler(scData);
            CsrBtScFreeDownstreamMessageContents(eventClass, msg);
            break;
        }

        case CSR_BT_CM_PRIM:
        {
            switch(*msg)
            {
#ifdef CSR_BT_LE_ENABLE
                case CSR_BT_CM_LE_EVENT_ADVERTISING_IND:
                    /* fallthrough */
                case CSR_BT_CM_LE_EVENT_SCAN_IND:
                    /* ignore */
                    break;

                case CSR_BT_CM_LE_EVENT_CONNECTION_IND:
                {
                    CsrBtScCmLeEventConnectionIndHandler(scData);
                    break;
                }
#endif

                case CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM:
                {
                    /* Special handler - completely stateless */
                    CsrBtScCmEncryptionKeySizeCfmHandler(scData);
                    break;
                }

                case CSR_BT_CM_ACL_DETACH_CFM:
                {
                    /* We don't care if it failed. In this case we'll just have to wait it out */
                    break;
                }
                case CSR_BT_CM_SET_EVENT_MASK_CFM:
                {
                    if (scData->state == SC_ST_NULL)
                    {
                        CsrBtScCmSetEventMaskCfmNullS(scData);
                    }
                    else
                    {
                        CsrBtScStateEventException(eventClass, *msg, scData->state);
                    }
                    break;
                }
                case CSR_BT_CM_ACL_CONNECT_IND:
                {
                    if (scData->state == SC_ST_REMOTE_BONDING)
                    {
                        CsrBtScCmAclConnectIndRemoteBondingS(scData);
                    }
                    else
                    {
                        CsrBtScCmAclConnectIndAnyS(scData);
                    }
                    break;
                }
                case CSR_BT_CM_ACL_DISCONNECT_IND:
                {
                    if (scData->state == SC_ST_REMOTE_BONDING)
                    {
                        CsrBtScCmAclDisconnectIndRemoteBondingS(scData);
                    }
                    else
                    {
                        CsrBtScCmAclDisconnectIndAnyS(scData);
                    }
                    break;
                }
                case CSR_BT_CM_GET_SECURITY_CONF_IND:
                {
                    if (scData->state == SC_ST_NULL)
                    {
                        CsrBtScCmGetSecurityConfIndNullS(scData);
                    }
                    else
                    {
                        CsrBtScStateEventException(eventClass, *msg, scData->state);
                    }
                    break;
                }
                case CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM:
                {
                    if (scData->state == SC_ST_NULL)
                    {
                        CsrBtScCmReadLocalBdAddrCfmNullS(scData);
                    }
                    else
                    {
                        CsrBtScStateEventException(eventClass, *msg, scData->state);
                    }
                    break;
                }
                case CSR_BT_CM_CONNECTION_REJ_SECURITY_IND:
                {
                    if (scData->state == SC_ST_REMOTE_BONDING ||
                        scData->state == SC_ST_REBOND)
                    {
                        CsrBtScCmConnectionRejSecurityIndRemoteBondingS(scData);
                    }
                    break;
                }
                case CSR_BT_CM_READ_REMOTE_NAME_CFM:
                {
                    if (scData->state == SC_ST_LOCAL_POST_LISBON_BONDING ||
                        scData->state == SC_ST_REBOND ||
                        scData->state == SC_ST_REMOTE_BONDING ||
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                        scData->state == SC_ST_AUTHENTICATE ||
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                        scData->state == SC_ST_ENCRYPT ||
#endif
                        scData->state == SC_ST_AUTHORISATION)
                    {
                        CsrBtScCmReadRemoteNameCfmUiS(scData);
                    }
                    else
                    {
                        CsrBtScStateEventException(eventClass, *msg, scData->state);
                    }
                    break;
                }

                case CSR_BT_CM_SM_REPAIR_IND:
                {
                    CsrBtScCmSmRepairIndHandler(scData);
                    break;
                }
#ifndef EXCLUDE_CSR_BT_L2CA_MODULE
                case CSR_BT_CM_L2CA_REGISTER_CFM:
                {
                    CsrBtCmL2caRegisterCfm *cmPrim = (CsrBtCmL2caRegisterCfm *)scData->recvMsgP;

                    if ((cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS) && (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM))
                    {
                        /* This means we are trying to make the device discoverable:
                           - store the local (dynamic) PSM and
                           - start accepting incoming connections on it */
                        scData->localPsm = cmPrim->localPsm;
                        if (scData->bondTime != 0)
                        {
                            /* (re-)start timer*/
                            scData->bondTime = CsrSchedTimerSet(scData->bondTime, csrBtAcceptBondPeriodTimeout,
                                                                (CsrUint16)scData->bondPhandle, scData);
                        }
                        scData->acceptingBond = TRUE;

                        CsrBtCml2caConnectAcceptReqSend(CSR_BT_SC_IFACEQUEUE,
                                                   scData->localPsm,
                                                   0,
                                                   scData->securityMode,
                                                   40,
                                                   0);
                    }
                    else
                    {
                        /* retry! */
                        CsrBtCml2caRegisterReqSend(CSR_BT_SC_IFACEQUEUE,CSR_BT_ASSIGN_DYNAMIC_PSM);
                    }
                    break;
                }
                case CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM:
                {
                    CsrBtCmL2caConnectAcceptCfm *cmPrim = (CsrBtCmL2caConnectAcceptCfm *)scData->recvMsgP;

                    if (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                        cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM)
                    {/* Just disconnect immediately. This is a fake record! we don't want to accept connections */
                        CsrBtCml2caDisconnectReqSend(cmPrim->btConnId);
                    }
                    /* else ignore.... */
                    break;
                }
                case CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM:
                {
                    CsrBtCmL2caCancelConnectAcceptCfm *cmPrim = (CsrBtCmL2caCancelConnectAcceptCfm *) scData->recvMsgP;
                    if (((cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                         || (cmPrim->resultCode == CSR_BT_RESULT_CODE_CM_NOTHING_TO_CANCEL))
                        && (cmPrim->resultSupplier == CSR_BT_SUPPLIER_CM))
                    {
                        /* Keep the PSM in case it is needed later */
                        scData->acceptingBond = FALSE;
                        CsrBtScCancelAcceptBondIndHandler(scData);
                        scData->bondTime      = 0;
                        scData->bondPhandle   = CSR_BT_APPLICATION_SECURITY_HANDLER;
                    }
                    else
                    {
                        /* retry...*/
                        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_SC_IFACEQUEUE,scData->localPsm);
                    }
                    break;
                }
#endif
                default:
                {
                    CsrBtScStateEventException(eventClass, *msg, scData->state);
                    break;
                }
            }
            CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            break;
        }

        case CSR_BT_SD_PRIM:
        {
            switch(*msg)
            {
                case CSR_BT_SD_READ_DEVICE_INFO_CFM:
                {
                    if (scData->state == SC_ST_LOCAL_POST_LISBON_BONDING ||
                        scData->state == SC_ST_REBOND ||
                        scData->state == SC_ST_REMOTE_BONDING ||
#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                        scData->state == SC_ST_AUTHENTICATE ||
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                        scData->state == SC_ST_ENCRYPT ||
#endif
                        scData->state == SC_ST_AUTHORISATION)
                    {
                       CsrBtScSdReadDeviceInfoCfmUiS(scData);
                    }
                    else
                    {
                        CsrBtScStateEventException(eventClass, *msg, scData->state);
                    }
                    break;
                }

                default:
                    CsrBtScStateEventException(eventClass, *msg, scData->state);
                    break;
            }
            CsrBtSdFreeUpstreamMessageContents(eventClass, msg);
            CsrBtSdPrivateFreeUpstreamMessageContents(eventClass, msg);
            break;
        }

        case DM_PRIM:
        {
            /* Some primitives need special handling for LE */
#ifdef CSR_BT_LE_ENABLE
            if (*msg == DM_SM_ENCRYPTION_CHANGE_IND)
            {
                CsrBtScLeEncryptionChangeHandler(scData);
                dm_free_upstream_primitive(scData->recvMsgP);
                scData->recvMsgP = NULL;
                break;
            }
#endif
#ifdef CSR_BT_LE_ENABLE
            if(*msg == DM_SM_CSRK_COUNTER_CHANGE_IND)
            {
                CsrBtScLeCsrkCounterChangeIndHandler(scData);
                dm_free_upstream_primitive(scData->recvMsgP);
                scData->recvMsgP = NULL;
                break;
            }
            if (*msg == DM_SM_KEYS_IND)
            {
                CsrBtScSmKeysIndHandler(scData);
                dm_free_upstream_primitive(scData->recvMsgP);
                scData->recvMsgP = NULL;
                break;
            }
#endif

            /* Special case for the primitive we _always_ ignore */
            if (*msg == DM_SM_ADD_DEVICE_CFM
                || *msg == DM_SM_ENCRYPTION_CHANGE_IND
                || *msg == DM_HCI_REFRESH_ENCRYPTION_KEY_IND)
            {
                CsrBtScIgnoreAnyS(scData);
                break;
            }

            switch(scData->state)
            {
                case SC_ST_NULL:
                {
                    switch(*msg)
                    {
                        case DM_SM_INIT_CFM:
                        {
                            CsrBtScDmSmInitCfmNullS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmPinRequestInd(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndIdleS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScRejectDmSmAuthoriseInd(scData);
                            break;
                        }
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:/* Fall-through */
                        case DM_SM_ACCESS_IND:                   /* Fall-through */
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:              /* Fall-through */
#endif
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmIoCapabilityRequestInd(scData);
                            break;
                        }
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                  break;
                }

                case SC_ST_IDLE:
                {
                    switch(*msg)
                    {
                        case DM_ACL_OPEN_CFM:                      /* Fall-through */
                        case DM_SM_ACCESS_IND:                     /* Fall-through */
                        case DM_SM_REMOVE_DEVICE_CFM:              /* Fall-through */
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:  /* Fall-through */
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:    /* Fall-through */
                        case DM_SM_INIT_CFM:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScDmSmPinRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndIdleS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScDmSmAuthoriseIndIdleS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScSmIoCapabilityResponseIndIdleS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScSmIoCapabilityRequestIndIdleS(scData);
                            break;
                        }
                        case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
                        {
                            /* WORKAROUND: B-25587 */
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:
                            CsrBtScSmKeyRequestIndHandler(scData);
                            break;
                        case DM_SM_SECURITY_CFM:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScLeSecurityIndHandler(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                  break;
                }

                case SC_ST_AUTHORISATION:
                {
                    switch(*msg)
                    {
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM: /* Fall-through */
                        case DM_SM_ACCESS_IND:                    /* Fall-through */
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:    /* Fall-through */
                        case DM_SM_REMOVE_DEVICE_CFM:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmPinRequestInd(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndIdleS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScRejectDmSmAuthoriseInd(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmIoCapabilityRequestInd(scData);
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:
                            CsrBtScDmSaveMessage(scData);
                            break;
                        case DM_SM_SECURITY_CFM:
                        {
                            CsrBtScLeSecurityCfmHandler(scData);
                            break;
                        }
                        case DM_SM_SECURITY_IND:
                            CsrBtScDmSaveMessage(scData);
                            break;
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                  break;
                }

                case SC_ST_LOCAL_POST_LISBON_BONDING:
                {
                    switch(*msg)
                    {
                        case DM_SM_REMOVE_DEVICE_CFM:              /* Fall-through */
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:  /* Fall-through */
                        case DM_SM_ACCESS_IND:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScDmSmPinRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScSmIoCapabilityResponseIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScSmIoCapabilityRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmLocalUserConfirmationRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmUserConfirmationRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_REQUEST_IND:
                        {
                            CsrBtScDmSmUserPasskeyRequestIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmUserPasskeyNotificationIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_KEYPRESS_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmKeypressNotificationIndBondingS(scData);
                            break;
                        }
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        {
                            CsrBtScDmSimplePairingCompleteIndLocalBondingS(scData);
                            break;
                        }
                        case DM_SM_BONDING_CFM:
                        {
                            CsrBtScSmBondingCfmBondingS(scData);
                            break;
                        }
                        case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
                        {
                            /* WORKAROUND: B-25587 */
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:
                        {
                            CsrBtScSmKeyRequestIndHandler(scData);
                            break;
                        }
                        case DM_SM_SECURITY_CFM:
                        {
                            CsrBtScLeSecurityCfmHandler(scData);
                            break;
                        }
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }

                case SC_ST_REMOTE_BONDING:
                {
                    switch(*msg)
                    {
                        case DM_SM_REMOVE_DEVICE_CFM:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScDmSmPinRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:
                        {
                            CsrBtScDmHciCreateConnectionCancelCompleteRemoteBonding(scData);
                            break;
                        }
                        case DM_SM_ACCESS_IND:
                        {
                            CsrBtScDmAccessIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScSmIoCapabilityResponseIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScSmIoCapabilityRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmLocalUserConfirmationRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmUserConfirmationRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_REQUEST_IND:
                        {
                            CsrBtScDmSmUserPasskeyRequestIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmUserPasskeyNotificationIndRemoteBondingS(scData);
                            break;
                        }
                        case DM_SM_KEYPRESS_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmKeypressNotificationIndBondingS(scData);
                            break;
                        }
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        {
                            CsrBtScDmSimplePairingCompleteIndRemoteBondingS(scData);
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:
                        {
                            CsrBtScSmKeyRequestIndHandler(scData);
                            break;
                        }
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }

#ifdef CSR_BT_INSTALL_SC_AUTHENTICATE
                case SC_ST_AUTHENTICATE:
                {
                    switch(*msg)
                    {
                        case DM_SM_REMOVE_DEVICE_CFM:
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM: /* Fall-through */
                        case DM_SM_ACCESS_IND:                    /* Fall-through */
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScDmSmPinRequestIndBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScRejectDmSmAuthoriseInd(scData);
                            break;
                        }
                        case DM_SM_AUTHENTICATE_CFM:
                        {
                            CsrBtScDmSmAuthenticateCfmAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScSmIoCapabilityResponseIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScSmIoCapabilityRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmLocalUserConfirmationRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmUserConfirmationRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_REQUEST_IND:
                        {
                            CsrBtScDmSmUserPasskeyRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmUserPasskeyNotificationIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_KEYPRESS_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmKeypressNotificationIndBondingS(scData);
                            break;
                        }
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        case DM_SM_BONDING_CFM:
                        {
                            /* Ignored, we rely on DM_SM_AUTHENTICATE_CFM instead. */
                             break;
                         }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND: /* Fall-through*/
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }
#endif /* CSR_BT_INSTALL_SC_AUTHENTICATE */
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                case SC_ST_ENCRYPT:
                {
                    switch(*msg)
                    {
                        case DM_SM_REMOVE_DEVICE_CFM:             /* Fall-through */
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM: /* Fall-through */
                        case DM_SM_ACCESS_IND:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScDmSmPinRequestIndBondingS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScRejectDmSmAuthoriseInd(scData);
                            break;
                        }
                        case DM_SM_ENCRYPT_CFM:
                        {
                            CsrBtScDmEncryptCfmEncryptionS(scData);
                            break;
                        }
                        case DM_SM_AUTHENTICATE_CFM:
                        {
                            CsrBtScDmSmAuthenticateCfmEncryptionS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScSmIoCapabilityResponseIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScSmIoCapabilityRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_LOCAL_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmLocalUserConfirmationRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_CONFIRMATION_REQUEST_IND:
                        {
                            CsrBtScDmSmUserConfirmationRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_REQUEST_IND:
                        {
                            CsrBtScDmSmUserPasskeyRequestIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_USER_PASSKEY_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmUserPasskeyNotificationIndAuthenticateS(scData);
                            break;
                        }
                        case DM_SM_KEYPRESS_NOTIFICATION_IND:
                        {
                            CsrBtScDmSmKeypressNotificationIndBondingS(scData);
                            break;
                        }
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        case DM_SM_BONDING_CFM:
                        {
                            /* Ignored, we rely on DM_SM_AUTHENTICATE_CFM and
                             * DM_SM_ENCRYPT_CFM instead. */
                             break;
                         }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:  /* Fall-throught */
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }
#endif /*CSR_BT_INSTALL_SC_ENCRYPTION*/

                case SC_ST_PENDING:
                {
                    switch(*msg)
                    {
                        case DM_SM_INIT_CFM:
                        {
                            CsrBtScDmSmInitCfmPendingS(scData);
                            break;
                        }
                        case DM_SM_REMOVE_DEVICE_CFM:
                        {
                            CsrBtScDmRemoveDeviceCfmPendingS(scData);
                            break;
                        }
                        case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
                        {
                            CsrBtScDmHciDeleteStoredLinkKeyCompletePendingS(scData);
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmPinRequestInd(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_ACCESS_IND:                    /* Fall-through */
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM: /* Fall-through */
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndIdleS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScRejectDmSmAuthoriseInd(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmIoCapabilityRequestInd(scData);
                            break;
                        }
#ifdef CSR_BT_INSTALL_SC_OOB
                        case DM_SM_READ_LOCAL_OOB_DATA_CFM:
                        {
                            CsrBtScSmReadLocalOobDataCfmPendingS(scData);
                            break;
                        }
#endif
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND: /* Fall-through */
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }

                case SC_ST_REBOND:
                {
                    switch(*msg)
                    {
                        case DM_SM_REMOVE_DEVICE_CFM:
                        {
#ifdef CSR_BT_LE_ENABLE
                            if (CSR_BT_SC_IS_LOW_ENERGY(scData))
                            {
                                CsrBtScDmLeRemoveDeviceCfmRebondS(scData);
                            }
                            else
                            {
#endif
                                CsrBtScDmRemoveDeviceCfmRebondS(scData);
#ifdef CSR_BT_LE_ENABLE
                            }
#endif
                            break;
                        }
                        case DM_SM_PIN_REQUEST_IND:
                        {
                            CsrBtScRejectDmSmPinRequestInd(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_IND:
                        {
                            CsrBtScDmSmLinkKeyIndIdleS(scData);
                            break;
                        }
                        case DM_SM_LINK_KEY_REQUEST_IND:
                        {
                            CsrBtScDmSmLinkKeyRequestIndIdleS(scData);
                            break;
                        }
                        case DM_SM_AUTHORISE_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM: /* Fall-through */
                        case DM_SM_ACCESS_IND:                    /* Fall-through */
                        case DM_SM_IO_CAPABILITY_RESPONSE_IND:    /* Fall-through */
                        case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        {
                            CsrBtScIgnoreAnyS(scData);
                            break;
                        }
                        case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        {
#ifdef CSR_BT_LE_ENABLE
                            if (CSR_BT_SC_IS_LOW_ENERGY(scData))
                            {
                                CsrBtScSmIoCapabilityRequestIndIdleS(scData);
                            }
                            else
                            {
#endif                            
                                CsrBtScRejectDmSmIoCapabilityRequestInd(scData);
#ifdef CSR_BT_LE_ENABLE
                            }
#endif
                            break;
                        }
                        case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
                        {
                            CsrBtScDmHciDeleteStoredLinkKeyCompleteRebondS(scData);
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                        case DM_SM_KEY_REQUEST_IND:
                            CsrBtScSmKeyRequestIndHandler(scData);
                            break;
                        case DM_SM_SECURITY_CFM:
                        {
                            CsrBtScLeSecurityCfmHandler(scData);
                            break;
                        }
                        case DM_SM_SECURITY_IND:
                        {
                            CsrBtScDmSaveMessage(scData);
                            break;
                        }
#endif
                        default:
                            CsrBtScStateEventException(eventClass, *msg, scData->state);
                            break;
                    }
                    break;
                }
            }

            /* Free DM primitive and members */
            dm_free_upstream_primitive(scData->recvMsgP);
            scData->recvMsgP = NULL;
            break;
        }

        case CSR_SCHED_PRIM:
            break;

        default:
            CsrBtScStateEventException(eventClass, *msg, scData->state);
            break;

    }

    CsrPmemFree(scData->recvMsgP);
    scData->recvMsgP = NULL;
}

