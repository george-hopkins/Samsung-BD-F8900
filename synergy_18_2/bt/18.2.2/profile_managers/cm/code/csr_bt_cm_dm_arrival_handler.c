/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_events_handler.h"
#include "csr_bt_cm_dm_sc_ssp_handler.h"
#include "csr_bt_cm_callback_q.h"
#include "dm_prim.h"
#ifdef CSR_BT_LE_ENABLE
#include "csr_bt_cm_le.h"
#endif

static void csrBtCmDmQoSSetupCfmHandler(cmInstanceData_t *cmData);

#define PUT_SC_MESSAGE(msg)                                             \
    {                                                                   \
        if ((msg) != NULL)                                              \
        {                                                               \
            CsrSchedMessagePut(cmData->scHandle, DM_PRIM, (msg));       \
        }                                                               \
        (msg) = NULL;                                                   \
    }

#define PUT_UPSTREAM_MESSAGE(cmData, msg)                               \
    {                                                                   \
        if ((msg) != NULL)                                              \
        {                                                               \
            if ((cmData)->dmVar.appHandle == cmData->scHandle)          \
            {                                                           \
                PUT_SC_MESSAGE((msg));                                  \
            }                                                           \
            else                                                        \
            {                                                           \
                CsrSchedMessagePut(cmData->dmVar.appHandle, DM_PRIM, (msg)); \
            }                                                           \
        }                                                               \
        (msg) = NULL;                                                   \
    }

/*************************************************************************************
  CsrBtCmDmArrivalHandler:
************************************************************************************/
void CsrBtCmDmArrivalHandler(cmInstanceData_t *cmData)
{
    /* Handles incoming events from the DM layer */
    DM_UPRIM_T  *dmPrim;
    dm_prim_t    dmEventType;
    CsrBool      isHci;

    isHci = FALSE;
    dmPrim = (DM_UPRIM_T *)cmData->recvMsgP;
    dmEventType = (CsrUint16)(dmPrim->type & DM_GROUP_MASK);

    /* Real DM primitives are catched here. For HCI ones, we have a
     * separate switch below this one */
    switch(dmEventType)
    {
        case DM_PRIV_UP_PRIM:
            {
                switch(dmPrim->type)
                {
                    case DM_AM_REGISTER_CFM:
                        {
                            if (cmData->globalState == notReady_s)
                            {
                                dm_sm_register_req(CSR_BT_CM_IFACEQUEUE,
                                                   0, /* context */
                                                   SEC_PROTOCOL_L2CAP,
                                                   0x0001,
                                                   TRUE,
                                                   SECL_NONE,
                                                   0,
                                                   NULL);
                                dm_hci_read_local_version(NULL);
                            }
                            break;
                        }
                    case DM_SET_BT_VERSION_CFM:
                        {
                            CsrUint16 *tab = CsrPmemZalloc(sizeof(CsrUint16));
                            dm_lp_write_roleswitch_policy_req(0, /* version */
                                                              1, /* length */
                                                              tab, /* table */
                                                              NULL); /* pp_prim */
                            break;
                        }
                    case DM_LP_WRITE_ROLESWITCH_POLICY_CFM:
                        {
                            /* This sets both DM and HCI default policy */
                            dm_set_default_link_policy_req((CSR_BT_DEFAULT_LOW_POWER_MODES | ENABLE_MS_SWITCH),
                                                           (CSR_BT_DEFAULT_LOW_POWER_MODES | ENABLE_MS_SWITCH),
                                                           NULL);
                            break;
                        }
                    case DM_WRITE_CACHED_PAGE_MODE_CFM:
                        {
                            CsrBtCmDmWriteCachedPageModeCfmHandler(cmData);
                            break;
                        }
                    case DM_WRITE_CACHED_CLOCK_OFFSET_CFM:
                        {
                            CsrBtCmDmWriteCachedClockOffsetCfmHandler(cmData);
                            break;
                        }
                    case DM_CLEAR_PARAM_CACHE_CFM:
                        {
                            CsrBtCmDmClearParamCacheCfmHandler(cmData);
                            break;
                        }
                    case DM_ACL_OPEN_CFM:
                        {
                            CsrBtCmDmAclOpenCfmInSdcCloseStateHandler(cmData);
                            CsrBtCmDmAclOpenCfmScStateHandler(cmData);
                            PUT_UPSTREAM_MESSAGE(cmData, cmData->recvMsgP);
                            break;
                        }
                    case DM_ACL_CONN_HANDLE_IND:
                        {
                        DM_ACL_CONN_HANDLE_IND_T *prim = (DM_ACL_CONN_HANDLE_IND_T *) dmPrim;
#ifdef CSR_BT_INSTALL_CHANGE_ACL_PACKET_TYPE
                            if (prim->status == HCI_SUCCESS)
                            {
                                dm_hci_change_packet_type_acl(&(prim->addrt.addr), CSR_BT_ACL_PACKET_TYPE);
                            }
#endif /* CSR_BT_INSTALL_CHANGE_ACL_PACKET_TYPE */

#ifdef CSR_BT_LE_ENABLE
                            if(prim->flags & DM_ACL_FLAG_ULP)
                            {
                                CsrBtCmLeAclConnHandleIndHandler(cmData);
                            }
                            else
#endif
                            {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
                                CsrBtCmPropgateEvent(cmData,
                                                     CsrBtCmPropgateAclConnectEvents,
                                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION,
                                                     prim->status,
                                                     prim,
                                                     &prim->status);
#endif

                            CsrBtCmDmAclConnHandleIndHandler(cmData);
                            }

                            break;
                        }
                    case DM_ACL_CLOSED_IND:
                        {
                            DM_ACL_CLOSED_IND_T *prim = (DM_ACL_CLOSED_IND_T *) dmPrim;

#ifdef CSR_BT_LE_ENABLE
                            if(prim->flags & DM_ACL_FLAG_ULP)
                            {
                                CsrBtCmLeAclClosedIndHandler(cmData);
                            }
                            else
#endif
                            {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION
                                CsrBtCmPropgateEvent(cmData,
                                                     CsrBtCmPropgateAclDisconnectEvents,
                                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ACL_CONNECTION,
                                                     HCI_SUCCESS,
                                                     prim,
                                                     &prim->reason);
#endif

                                CsrBtCmDmAclCloseIndHandler(cmData);
                            }
                            break;
                        }

                    case DM_HCI_SWITCH_ROLE_CFM:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropgateRoleSwitchEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE,
                                                 dmPrim->dm_switch_role_cfm.status,
                                                 dmPrim,
                                                 NULL);
#endif

                            CsrBtCmDmHciSwitchRoleCompleteHandler(cmData);
                            break;
                        }

                    case DM_HCI_REMOTE_NAME_CFM:
                        {
                            CsrBtCmDmHciRemoteNameCompleteHandler(cmData);
                            break;
                        }
#ifdef CSR_BT_LE_ENABLE
                    case DM_SET_BLE_CONNECTION_PARAMETERS_CFM:
                        {
                            CsrBtCmCallbackDispatchSimple(cmData, dmPrim);
                            break;
                        }
                    case DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM:
                        {
                            CsrBtCmCallbackDispatchSimple(cmData, dmPrim);
                            break;
                        }

                    case DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND:
                        {
                            CsrBtCmLeAcceptConnparamUpdateIndHandler(cmData);
                            break;
                        }
#endif
                    case DM_BAD_MESSAGE_IND:
                        {
                            dm_free_upstream_primitive(cmData->recvMsgP);
                            cmData->recvMsgP = NULL;
                            break;
                        }
#ifdef CSR_BT_INSTALL_CM_SWITCH_ROLE_PUBLIC
                    case DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM:
                        {
                            CsrBtCmAlwaysMasterDevicesCfmHandler(cmData);
                            break;
                        }
#endif
#ifdef CSR_BT_INSTALL_CHANGE_ACL_PACKET_TYPE
                    case DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM:
                        {
                            /* Ignore */
                            break;
                        }
#endif
                    default:
                        {
                            CsrBtCmStateEventException(DM_PRIM, dmPrim->type,
                                                       (CsrUint16)cmData->globalState);
                            break;
                        }
                }
                break;
            }

#ifndef EXCLUDE_CSR_BT_SCO_MODULE
        case DM_SYNC_UP_PRIM:
            {
                switch(dmPrim->type)
                {
                    case DM_SYNC_REGISTER_CFM:
                        {
                            CsrBtCmDmSyncRegisterCfmHandler(cmData);
                            break;
                        }
                    case DM_SYNC_CONNECT_CFM:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
                            CsrBool incoming = FALSE;

                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropgateSyncConnectEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION,
                                                 dmPrim->dm_sync_connect_cfm.status,
                                                 dmPrim,
                                                 &incoming);
#endif

                            CsrBtCmDmSyncConnectCfmHandler(cmData);
                            break;
                        }
                    case DM_SYNC_CONNECT_IND:
                        {
                            CsrBtCmDmSyncConnectIndHandler(cmData);
                            break;
                        }
                    case DM_SYNC_DISCONNECT_CFM:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropgateSyncDiscCfmEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION,
                                                 dmPrim->dm_sync_disconnect_cfm.status,
                                                 dmPrim,
                                                 NULL);
#endif

                            CsrBtCmDmSyncDisconnectCfmHandler(cmData);
                            break;
                        }
#ifdef CSR_BT_INSTALL_CM_PRI_SCO_RENEGOTIATE
                    case DM_SYNC_RENEGOTIATE_CFM:
                        {
                            CsrBtCmDmSyncRenegotiateCfmHandler(cmData);
                            break;
                        }
#endif
                    case DM_SYNC_CONNECT_COMPLETE_IND:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
                            CsrBool incoming = TRUE;

                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropgateSyncConnectEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION,
                                                 dmPrim->dm_sync_connect_complete_ind.status,
                                                 dmPrim,
                                                 &incoming);
#endif

                            CsrBtCmDmSyncConnectCompleteIndHandler(cmData);
                            break;
                        }
                    case DM_SYNC_DISCONNECT_IND:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION
                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropgateSyncDiscIndEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_SYNCHRONOUS_CONNECTION,
                                                 dmPrim->dm_sync_disconnect_ind.reason,
                                                 dmPrim,
                                                 NULL);
#endif

                            CsrBtCmDmSyncDisconnectIndHandler(cmData);
                            break;
                        }
                    case DM_SYNC_RENEGOTIATE_IND:
                        {
                            CsrBtCmDmSyncRenegotiateIndHandler(cmData);
                            break;
                        }
                    default:
                        {
                            CsrBtCmStateEventException(DM_PRIM, dmPrim->type, (CsrUint16)cmData->globalState);
                            break;
                        }
                }
                break;
            }
#endif /* EXCLUDE_CSR_BT_SCO_MODULE */

        case DM_SM_UP_PRIM:
            {
                switch(dmPrim->type)
                {
#ifndef EXCLUDE_CSR_BT_RFC_MODULE
                    case DM_SM_ACCESS_CFM:
                        CsrBtCmDmSmAccessCfmHandler(cmData);
                        break;
#endif
#ifdef CSR_BT_INSTALL_SC_ENCRYPTION
                    case DM_SM_ENCRYPT_CFM:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
                            CsrUint8 status = CsrBtCmDmSmEncryptCfmHandler(cmData);

                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropagateEncryptCfmStatusEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE,
                                                 status,
                                                 dmPrim,
                                                 NULL);
#else
                            CsrBtCmDmSmEncryptCfmHandler(cmData);
#endif
                            break;
                        }
#endif
                    case DM_SM_AUTHORISE_IND:
                        CsrBtCmDmAuthoriseIndHandler(cmData);
                        break;
                    case DM_SM_LINK_KEY_REQUEST_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_PIN_REQUEST_IND:
                        if (cmData->dmVar.rebond.linkKeyType == HCI_COMBINATION_KEY)
                        {
                            cmData->dmVar.rebond.linkKeyType = DM_SM_LINK_KEY_NONE;
                        }
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_LINK_KEY_IND:
                        {
                            /* An all-zero address is debug info from
                             * the DM/SM - discard of those */
                            DM_SM_LINK_KEY_IND_T *dm = cmData->recvMsgP;
                            if(!CsrBtBdAddrEqZero(&dm->bd_addr))
                            {
                                PUT_SC_MESSAGE(cmData->recvMsgP);
                            }
                        }
                        break;
                    case DM_SM_ACCESS_IND:
                        CsrBtCmDmSmAccessIndHandler(cmData);
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_IO_CAPABILITY_RESPONSE_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_IO_CAPABILITY_REQUEST_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_USER_CONFIRMATION_REQUEST_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_USER_PASSKEY_REQUEST_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_USER_PASSKEY_NOTIFICATION_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_KEYPRESS_NOTIFICATION_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_SIMPLE_PAIRING_COMPLETE_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;
                    case DM_SM_INIT_CFM:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        if (cmData->globalState == notReady_s)
                        {
                            CsrBtCmDmSmInitCfmHandler(cmData);
                        }
                        else
                        {
                            CsrBtCmDmLocalQueueHandler();
                        }
                        break;

                    case DM_SM_BONDING_CFM:
                        /* DM queue not locked during bonding, so do
                         * not unlock it here */
                        CsrBtCmDmSmBondingScStateHandler(cmData);
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;

                    case DM_SM_ADD_DEVICE_CFM:
                        /*fallthrough*/
                    case DM_SM_REMOVE_DEVICE_CFM:
                        /*fallthrough*/
                    case DM_SM_READ_DEVICE_CFM:
                        /*fallthrough*/
                    case DM_SM_AUTHENTICATE_CFM:
                        {
                            PUT_SC_MESSAGE(cmData->recvMsgP);
                            CsrBtCmDmLocalQueueHandler();
                            break;
                        }
                    case DM_SM_ENCRYPTION_CHANGE_IND:
                        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE
                            CsrBtCmPropgateEvent(cmData,
                                                 CsrBtCmPropagateEncryptIndStatusEvents,
                                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ENCRYPT_CHANGE,
                                                 HCI_SUCCESS,
                                                 dmPrim,
                                                 NULL);
#endif

                            CsrBtCmDmSmEncryptionChangeHandler(cmData);
                            PUT_SC_MESSAGE(cmData->recvMsgP);
                            break;
                        }

                    case DM_SM_REGISTER_CFM:
                        /* fallthrough */
                    case DM_SM_REGISTER_OUTGOING_CFM:
                        /* fallthrough */
                    case DM_SM_UNREGISTER_CFM:
                        /* fallthrough */
                    case DM_SM_UNREGISTER_OUTGOING_CFM:
                        /* Ignore and consume these. Then exit completely. */
                        dm_free_upstream_primitive(cmData->recvMsgP);
                        cmData->recvMsgP = NULL;
                        return;

                    case DM_SM_KEYS_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;

                    case DM_SM_SECURITY_CFM:
                        {
                            PUT_SC_MESSAGE(cmData->recvMsgP);
                            CsrBtCmDmLocalQueueHandler();
                            break;
                        }

                    case DM_SM_SECURITY_IND:
                        /* fallthrough */
                    case DM_SM_CSRK_COUNTER_CHANGE_IND:
                        /* fallthrough */
                    case DM_SM_KEY_REQUEST_IND:
                        PUT_SC_MESSAGE(cmData->recvMsgP);
                        break;

                    default:
                        {
                            CsrBtCmStateEventException(DM_PRIM,
                                                       dmPrim->type,
                                                       (CsrUint16)cmData->globalState);
                            break;
                        }
                }
                break;
            }

        default:
            {
                /* This must be a HCI event then... */
                isHci = TRUE;
                break;
            }
    }

    /* Only enter this if it's a HCI event of some sort */
    if(isHci)
    {
        /* These are special HCI events that doesn't use the normal
         * grouping system. Note that all cases _return_ if
         * matched. */
        switch (dmPrim->type)
        {
            case DM_HCI_INQUIRY_RESULT_IND:
                {
                    CsrBtCmDmHciInquiryResultHandler(cmData);
                    return;
                }
            case DM_HCI_MODE_CHANGE_EVENT_IND:
                {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
                    CsrBtCmPropgateEvent(cmData,
                                         CsrBtCmPropgateModeChangeEvents,
                                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE,
                                         dmPrim->dm_mode_change_event_ind.status,
                                         dmPrim,
                                         NULL);
#endif

                    CsrBtCmDmStoreActualMode(cmData);
                    CsrBtCmDmHciModeChangeEventHandler(cmData);
                    return;
                }
            case DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND:
                {
                    CsrBtCmDmHciInquiryResultWithRssiHandler(cmData);
                    return;
                }
#if (CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1)
            case DM_HCI_SNIFF_SUB_RATING_IND:
                {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE
                    CsrBtCmPropgateEvent(cmData,
                                         CsrBtCmPropgateSsrEvents,
                                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_MODE_CHANGE,
                                         dmPrim->dm_sniff_sub_rating_ind.status,
                                         dmPrim,
                                         NULL);
#endif

                    CsrBtCmDmHciSniffSubRatingIndHandler(cmData);
                    return;
                }
            case DM_HCI_EXTENDED_INQUIRY_RESULT_IND:
                {
                    CsrBtCmDmHciExtendedInquiryResultIndHandler(cmData);
                    return;
                }
#endif
            case DM_HCI_LINK_SUPERV_TIMEOUT_IND:
                {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE
                    CsrBtCmPropgateEvent(cmData,
                                         CsrBtCmPropgateLstoChangeEvents,
                                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LSTO_CHANGE,
                                         HCI_SUCCESS,
                                         dmPrim,
                                         NULL);
#endif

                    CsrBtCmDmHciLinkSupervisionTimeoutIndHandler(cmData);
                    return;
                }
            case DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND:
                {
                    /* Ignore this one for now */
                    return;
                }
            case DM_HCI_CONN_PACKET_TYPE_CHANGED_IND:
                /* Fall through */
            case DM_HCI_RETURN_LINK_KEYS_IND:
                {
                    CsrBtCmStateEventException(DM_PRIM, dmPrim->type, (CsrUint16)cmData->globalState);
                    return;
                }

#ifdef CSR_BT_LE_ENABLE
            case DM_HCI_ULP_ADVERTISING_REPORT_IND:
                {
                    CsrBtCmLeReportIndHandler(cmData,
                                              (DM_HCI_ULP_ADVERTISING_REPORT_IND_T*)dmPrim);
                    dm_free_upstream_primitive(cmData->recvMsgP);
                    cmData->recvMsgP = NULL;
                    return;
                }

            case DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND:
                {
                    CsrBtCmLeConnectionUpdateCmpIndHandler(cmData,
                                                           (DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T*)dmPrim);
                    dm_free_upstream_primitive(cmData->recvMsgP);
                    cmData->recvMsgP = NULL;
                    return;
                }
#endif
        }

        /* Remaining HCI events use the standard grouping */
        dmEventType = (CsrUint16)(dmPrim->type & DM_OGF_MASK);
        switch(dmEventType)
        {
            case DM_LC_PRIM|DM_HCI_WITH_HANDLE:
            case DM_LC_PRIM:
                {
                    switch(dmPrim->type)
                    {
                        case DM_HCI_INQUIRY_CFM:
                            {
                                CsrBtCmDmHciInquiryCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_INQUIRY_CANCEL_CFM:
                            {
                                CsrBtCmDmHciInquiryCancelCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_CREATE_CONNECTION_CANCEL_CFM:
                            {
                                PUT_SC_MESSAGE(cmData->recvMsgP);
                                break;
                            }
                        case DM_HCI_REMOTE_NAME_REQ_CANCEL_CFM:
                            {
                                /* This is not handled. Wait for DM_HCI_REMOTE_NAME_CFM instead */
                                break;
                            }
                        case DM_HCI_READ_REMOTE_VER_INFO_CFM:
                            {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION
                                CsrBtCmPropgateEvent(cmData,
                                                     CsrBtCmPropgateReadRemoteVersionEvents,
                                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_REMOTE_VERSION,
                                                     dmPrim->dm_read_remote_version_cfm.status,
                                                     dmPrim,
                                                     NULL);
#endif

                                CsrBtCmDmHciReadRemoteVersionCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM:
                            {
                                CsrBtCmDmHciReadRemoteFeaturesCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_READ_CLOCK_OFFSET_CFM:
                            {
                                CsrBtCmDmHciReadClockOffsetCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_READ_REMOTE_EXT_FEATURES_CFM:
                            {
                                CsrBtCmDmHciReadRemoteExtFeaturesCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM:
                            {
                                CsrBtCmDmHciWriteInquiryTransmitPowerLevelCompleteHandler(cmData);
                                break;
                            }
                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM, dmPrim->type, (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }

            case DM_LP_PRIM|DM_HCI_WITH_HANDLE:
            case DM_LP_PRIM:
                {
                    switch(dmPrim->type)
                    {
                        case DM_HCI_WRITE_LINK_POLICY_SETTINGS_CFM:
                            {
                                CsrBtCmDmHciWriteLpSettingsCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_ROLE_DISCOVERY_CFM:
                            {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE
                                CsrBtCmPropgateEvent(cmData,
                                                     CsrBtCmPropgateRoleSwitchEvents,
                                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_ROLE_CHANGE,
                                                     dmPrim->dm_role_discovery_cfm.status,
                                                     dmPrim,
                                                     NULL);
#endif

                                CsrBtCmDmHciRoleDiscoveryCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_QOS_SETUP_CFM:
                            {
                                csrBtCmDmQoSSetupCfmHandler(cmData);
                                break;
                            }
#if (CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1)
#ifdef CSR_BT_INSTALL_CM_PRI_MODE_SETTINGS
                        case DM_HCI_SNIFF_SUB_RATE_CFM:
                            {
                                CsrBtCmDmHciSniffSubRateCompleteHandler(cmData);
                                break;
                            }
#endif
#endif
                        case DM_HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_CFM:
                            {
                                CsrBtCmDmHciWriteDefaultLinkPolicySettingsCompleteHandler(cmData);
                                break;
                            }
                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM, dmPrim->type, (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }

            case DM_BB_PRIM|DM_HCI_WITH_HANDLE:
            case DM_BB_PRIM:
                {
                    switch(dmPrim->type)
                    {
#ifndef  CSR_BT_EXCLUDE_HCI_READ_LOCAL_NAME
                        case DM_HCI_READ_LOCAL_NAME_CFM:
                            {
                                CsrBtCmDmHciReadLocalNameCompleteHandler(cmData);
                                break;
                            }
#endif                        
                        case DM_HCI_CHANGE_LOCAL_NAME_CFM:
                            {
                                CsrBtCmDmHciChangeLocalNameCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_CLASS_OF_DEVICE_CFM:
                            {
                                CsrBtCmDmHciWriteClassOfDeviceCompleteHandler(cmData);
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_READ_COD
                        case DM_HCI_READ_CLASS_OF_DEVICE_CFM:
                            {
                                CsrBtCmDmHciReadClassOfDeviceCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_WRITE_PAGESCAN_ACTIVITY_CFM:
                            {
                                CsrBtCmDmHciWritePageScanActivityCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_INQUIRYSCAN_ACTIVITY_CFM:
                            {
                                CsrBtCmDmHciWriteInquiryScanActivityCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_SCAN_ENABLE_CFM:
                            {
                                CsrBtCmDmHciWriteScanEnableCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_AUTO_FLUSH_TIMEOUT_CFM:
                            {
                                CsrBtCmDmHciWriteAutoFlushTimeoutCompleteHandler(cmData);
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_READ_SCAN_EANBLE
                        case DM_HCI_READ_SCAN_ENABLE_CFM:
                            {
                                CsrBtCmDmHciReadScanEnableCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_DELETE_STORED_LINK_KEY_CFM:
                            {
                                PUT_SC_MESSAGE(cmData->recvMsgP);
                                CsrBtCmDmLocalQueueHandler();
                                break;
                            }

                        case DM_HCI_WRITE_VOICE_SETTING_CFM:
                            {
                                CsrBtCmDmHciWriteVoiceSettingCompleteHandler(cmData);
                                break;
                            }

                        case DM_HCI_WRITE_LINK_SUPERV_TIMEOUT_CFM:
                            {
                                CsrBtCmDmHciWriteLinkSuperVisionTimeoutCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_PAGE_TIMEOUT_CFM:
                            {
                                CsrBtCmDmHciWritePageToCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_SET_EVENT_FILTER_CFM:
                            {
#ifdef CSR_BT_INSTALL_CM_DUT_MODE
                                if (cmData->dmVar.deviceUnderTest)
                                {
                                    DM_HCI_SET_EVENT_FILTER_CFM_T    *dmPrim;

                                    dmPrim = (DM_HCI_SET_EVENT_FILTER_CFM_T *) cmData->recvMsgP;
                                    if (dmPrim->status == HCI_SUCCESS)
                                    {
                                        cmData->dmVar.pendingChipScanMode = HCI_SCAN_ENABLE_INQ_AND_PAGE;
                                        dm_hci_write_scan_enable(HCI_SCAN_ENABLE_INQ_AND_PAGE, NULL);
                                    }
                                    else
                                    {
                                        CsrBtCmSendDeviceUnderTestComplete(cmData->dmVar.appHandle, dmPrim->status, 1);
                                    }
                                }
                                else
#endif                                    
                                {
                                    CsrBtCmSetEventFilterCommonCfmHandler(cmData);
                                }
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_READ_TX_POWER_LEVEL
                        case DM_HCI_READ_TX_POWER_LEVEL_CFM:
                            {
                                CsrBtCmDmHciReadTxPowerLevelCompleteHandler(cmData);
                                break;
                            }
#endif
#ifdef CSR_BT_INSTALL_CM_PRI_IAC
                        case DM_HCI_READ_CURRENT_IAC_LAP_CFM:
                            {
                                CsrBtCmDmHciReadIacCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_CURRENT_IAC_LAP_CFM:
                            {
                                CsrBtCmDmHciWriteIacCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_READ_NUM_SUPPORTED_IAC_CFM:
                            {
                                CsrBtCmDmHciReadNumSupportedIacCompleteHandler(cmData);
                                break;
                            }
#endif
#ifdef CSR_BT_INSTALL_CM_AFH
                        case DM_HCI_SET_AFH_CHANNEL_CLASS_CFM:
                            {
                                CsrBtCmDmHciSetAfhChannelClassCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_WRITE_INQUIRY_SCAN_TYPE_CFM:
                            {
                                CsrBtCmDmHciWriteInquiryScanTypeCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_INQUIRY_MODE_CFM:
                            {
                                CsrBtCmDmHciWriteInquiryModeCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_PAGE_SCAN_TYPE_CFM:
                            {
                                CsrBtCmDmHciWritePageScanTypeCompleteHandler(cmData);
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_AFH
                        case DM_HCI_READ_AFH_CHANNEL_CLASS_M_CFM:
                            {
                                CsrBtCmDmHciReadAfhChannelAssesModeCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_AFH_CHANNEL_CLASS_M_CFM:
                            {
                                CsrBtCmDmHciWriteAfhChannelAssesModeCompleteHandler(cmData);
                                break;
                            }
#endif
#if (CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_2P1)
                        case DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_CFM:
                            {
                                CsrBtCmDmHciWriteExtendedInquiryResponseDataCompleteHandler(cmData);
                                break;
                            }
                        case DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_CFM:
                            {
                                CsrBtCmDmHciWriteInquiryTransmitPowerLevelCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_REFRESH_ENCRYPTION_KEY_IND:
                            {
                                PUT_SC_MESSAGE(cmData->recvMsgP);
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_OOB
                        case DM_SM_READ_LOCAL_OOB_DATA_CFM:
                            {
                                PUT_SC_MESSAGE(cmData->recvMsgP);
                                CsrBtCmDmLocalQueueHandler();
                                break;
                            }
#endif
                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM,
                                                           dmPrim->type,
                                                           (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }

            case DM_INF_PRIM|DM_HCI_WITH_HANDLE:
            case DM_INF_PRIM:
                {
                    switch(dmPrim->type)
                    {
                        case DM_HCI_READ_BD_ADDR_CFM:
                            {
                                CsrBtCmDmHciReadBdAddrCompleteHandler(cmData);
                                break;
                            }
#ifdef CSR_BT_INSTALL_CM_READ_LOCAL_EXT_FEATURES
                        case DM_HCI_READ_LOCAL_EXT_FEATURES_CFM:
                            {
                                CsrBtCmDmHciReadLocalExtFeaturesCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_READ_LOCAL_SUPP_FEATURES_CFM:
                            {
                                CsrBtCmDmHciReadSuppFeaturesCfmHandler(cmData);
                                break;
                            }
                        case DM_HCI_READ_LOCAL_VER_INFO_CFM:
                            {
                                CsrBtCmDmHciReadLocalVersionCompleteHandler(cmData);
                                break;
                            }

                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM,
                                                           dmPrim->type,
                                                           (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }

            case DM_STATUS_PRIM|DM_HCI_WITH_HANDLE:
            case DM_STATUS_PRIM:
                {
                    switch(dmPrim->type)
                    {
#ifdef CSR_BT_INSTALL_CM_READ_RSSI
                        case DM_HCI_READ_RSSI_CFM:
                            {
                                CsrBtCmDmHciReadRssiCompleteHandler(cmData);
                                break;
                            }
#endif
#ifdef CSR_BT_INSTALL_CM_GET_LINK_QUALITY
                        case DM_HCI_GET_LINK_QUALITY_CFM:
                            {
                                CsrBtCmDmHciGetLinkQualityCompleteHandler(cmData);
                                break;
                            }
#endif
#ifdef CSR_BT_INSTALL_CM_AFH
                        case DM_HCI_READ_AFH_CHANNEL_MAP_CFM:
                            {
                                CsrBtCmReadAfhChannelMapCfmHandler(cmData);
                                break;
                            }
#endif
#ifdef CSR_BT_INSTALL_CM_READ_CLOCK
                        case DM_HCI_READ_CLOCK_CFM:
                            {
                                CsrBtCmDmHciReadClockCompleteHandler(cmData);
                                break;
                            }
#endif
                        case DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM:
                        {
                            CsrBtCmCallbackDispatchSimple(cmData, dmPrim);
                            break;
                        }
#ifdef CSR_BT_INSTALL_CM_READ_FAILED_CONTACT_COUNTER
                        case DM_HCI_READ_FAILED_CONTACT_COUNT_CFM:
                            {
                                CsrBtCmDmHciReadFailedContactCounterCompleteHandler(cmData);
                                break;
                            }
#endif
                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM,
                                                           dmPrim->type,
                                                           (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }

#ifdef CSR_BT_INSTALL_CM_DUT_MODE
            case DM_TEST_PRIM|DM_HCI_WITH_HANDLE:
            case DM_TEST_PRIM:
                {
                    switch(dmPrim->type)
                    {
                        case DM_HCI_ENABLE_DUT_MODE_CFM:
                            {
                                CsrBtCmDmHciDeviceUnderTestCompleteHandler(cmData);
                                break;
                            }
                        default:
                            {
                                CsrBtCmStateEventException(DM_PRIM,
                                                           dmPrim->type,
                                                           (CsrUint16)cmData->globalState);
                                break;
                            }
                    }
                    break;
                }
#endif
#ifdef CSR_BT_LE_ENABLE
        case DM_ULP_PRIM|DM_HCI_WITH_HANDLE:
        case DM_ULP_PRIM:
            {
                CsrBtCmCallbackDispatchSimple(cmData, dmPrim);
                break;
            }
#endif

            default:
                {
                    CsrBtCmStateEventException(DM_PRIM,
                                               (CsrUint16)dmEventType,
                                               (CsrUint16)cmData->globalState);
                    break;
                }
        }
    } /* isHci */
}


static void csrBtCmDmQoSSetupCfmHandler(cmInstanceData_t *cmData)
{
    DM_HCI_QOS_SETUP_CFM_T *dmPrim = (DM_HCI_QOS_SETUP_CFM_T *) cmData->recvMsgP;

    /* Only if the HCI QoS request asks for a latency (Tpoll) of less
     * than 14 slots (8.75 milliseconds) do we try to adjust the QoS
     * to 40 slots. This is consistent with B-4441. Also ensure that
     * we only attempt the QoS-adjustment only once for these
     * devices */
    if (dmPrim->latency < 8750 &&
        dmPrim->status == HCI_SUCCESS)
    {
        aclTable *aclConnectionElement = NULL;
        returnAclConnectionElement(cmData, dmPrim->bd_addr, &aclConnectionElement);

        if (aclConnectionElement &&
            !aclConnectionElement->unsolicitedQosSetup)
        {
            /* Try once and only once to correct QoS */
            aclConnectionElement->unsolicitedQosSetup = TRUE;
            dm_hci_qos_setup_req(&(dmPrim->bd_addr),
                                 0,
                                 HCI_QOS_BEST_EFFORT,
                                 CSR_BT_CM_DEFAULT_QOS_TOKEN_RATE,
                                 CSR_BT_CM_DEFAULT_QOS_PEAK_BANDWIDTH,
                                 CSR_BT_CM_DEFAULT_QOS_LATENCY,
                                 CSR_BT_CM_DEFAULT_QOS_DELAY_VARIATION,
                                 NULL);
        }
    }
}

