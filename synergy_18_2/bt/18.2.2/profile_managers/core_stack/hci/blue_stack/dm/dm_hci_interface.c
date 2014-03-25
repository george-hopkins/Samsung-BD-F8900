/*!

   Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file              dm_hci_interface.c

\brief             This file contains the Device Manager functions.
*/

#include "csr_synergy.h"

#include "csr_util.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "error.h"
#include "csr_util.h"
#include "csr_mblk.h"
#include "l2cap_config.h"

#include "dm_layer_manager.h"
#include "dm_mgt_interface.h"
#include "dm_acl_core.h"
#include "dm_acl_con_par_cache.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_security_manager.h"
#include "dm_sync_manager.h"
#include "dm_hci_interface.h"
#include "l2cap_control.h"
#include "l2cap_con_handle.h"
#include "l2cap_amp_utils.h"
#include "dm_amp_interface.h"
#include "dm_acl_connection_fsm_definitions.h"
#include "sm.h"
#include "tbdaddr.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_sched.h"
#include "csr_env_prim.h"
#include "csr_bt_hcishim.h"
#else
#include "bg_int.h"
#include "lm.h"
#endif


#if defined(INSTALL_BUILD_FOR_HOST_NO_OVERRIDE_HCI_CMD_FC) && !defined(BUILD_FOR_HOST)
#error "Cannot INSTALL_BUILD_FOR_HOST_NO_OVERRIDE_HCI_CMD_FC unless BUILD_FOR_HOST is defined"
#endif

/*
 * Private typedefs.
 */

/*! \brief Function pointer definition for HCI events. */
typedef void (*HCI_EV_FUNCTION_T)(HCI_UPRIM_T *prim);

/*! \brief Queue definition for HCI commands. */
typedef struct hci_queue_tag
{
    struct hci_queue_tag    *next_item;
    HCI_UPRIM_T             *primitive;
} HCI_QUEUE_T;

/*! \brief
    Some HCI commands interfere with each other and so we must wait for
    each to fully complete before sending the next. Most commands are not
    like this and so are 'NOT_BLOCKED_OR_BLOCKING'. HCI_CREATE_CONNECTION
    can be interfered with, but does not interfere with subsequent commands,
    therefore it is 'BLOCKED_NOT_BLOCKING'. HCI_DISCONNECT and
    HCI_REMOTE_NAME_REQUEST can interfere and be interfered with, therefore
    they are 'BLOCKED_AND_BLOCKING'.
*/
typedef enum
{
    DM_HCI_NOT_BLOCKED_OR_BLOCKING,
    DM_HCI_BLOCKED_NOT_BLOCKING,
    DM_HCI_BLOCKED_AND_BLOCKING
} DM_HCI_BLOCKING_T;

typedef enum
{
    dm_hci_first_queue = 0,
    dm_hci_pending = 0,
    dm_hci_sent,
    dm_hci_completing,
    dm_hci_num_queues       /* Final entry to give number of queues. */
} DM_HCI_COMMAND_QUEUE;

/*
 * Private function prototypes.
 */

/*! \brief Decide if HCI command can block or be blocked. */
#ifndef DISABLE_DM_BREDR
static DM_HCI_BLOCKING_T blocking(HCI_UPRIM_T *p_prim, BD_ADDR_T *p_bd_addr);
#endif

/*! \brief Decide if an HCI command queue allows transmission of blocking command. */
#ifndef DISABLE_DM_BREDR
static CsrBool queue_allows(HCI_QUEUE_T *p_queue, BD_ADDR_T *p_bd_addr);
#endif

/*! \brief Try to send pending commands to HCI. */
static void tx_commands_to_hci(void);

/*! \brief Add an HCI primitive to an HCI queue. */
static void add_to_hci_queue(DM_HCI_COMMAND_QUEUE queue, HCI_UPRIM_T *prim);

/*! \brief Remove HCI command, corresponding to given HCI event, from
    completing queue. */
static CsrUint8 remove_command_from_completing_list(
        hci_op_code_t op_code,
        BD_ADDR_T *bd_addr,
        hci_connection_handle_t handle);

/*! \brief Free downstream HCI primitives properly. */
static void dm_hci_free_downstream_primitive(HCI_UPRIM_T *prim);

/*! \brief Common parts of HCI command complete and command status handling. */
HCI_UPRIM_T *hci_cc_cs_common(hci_op_code_t op_code);

static void tx_commands_to_hci(void);

/*! \brief HCI event handlers. */
static void dm_ev_not_used(HCI_UPRIM_T *hci_prim);
static void dm_ev_disconnect_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_encryption_change(HCI_UPRIM_T *hci_prim);
static void dm_ev_read_remote_ver_info_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_command_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_command_status(HCI_UPRIM_T *hci_prim);
static void dm_ev_refresh_encryption_key_complete(HCI_UPRIM_T *hci_prim);
#ifndef DISABLE_DM_BREDR
static void dm_ev_connect_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_connect_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_inquiry_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_inquiry_result(HCI_UPRIM_T *hci_prim);
static void dm_ev_auth_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_remote_name_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_change_conn_link_key_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_read_rem_supp_features_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_qos_setup_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_role_change_event(HCI_UPRIM_T *hci_prim);
static void dm_ev_mode_change_event(HCI_UPRIM_T *hci_prim);
static void dm_ev_pin_code_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_link_key_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_link_key_notification(HCI_UPRIM_T *hci_prim);
static void dm_ev_read_clock_offset_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_conn_packet_type_changed(HCI_UPRIM_T *hci_prim);
static void dm_ev_inquiry_result_with_rssi(HCI_UPRIM_T *hci_prim);
static void dm_ev_flow_spec_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_read_rem_ext_features_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_sniff_sub_rate(HCI_UPRIM_T *hci_prim);
static void dm_ev_extended_inquiry_result(HCI_UPRIM_T *hci_prim);
static void dm_ev_io_capability_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_io_capability_response(HCI_UPRIM_T *hci_prim);
static void dm_ev_user_confirmation_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_user_passkey_request(HCI_UPRIM_T *hci_prim);
static void dm_ev_remote_oob_data_request (HCI_UPRIM_T *hci_uprim);
static void dm_ev_user_passkey_notification (HCI_UPRIM_T *hci_uprim);
static void dm_ev_keypress_notification (HCI_UPRIM_T *hci_uprim);
static void dm_ev_lst_change(HCI_UPRIM_T *hci_uprim);
static void dm_ev_rem_host_supported_features(HCI_UPRIM_T *hci_uprim);
static void dm_ev_enhanced_flush_complete(HCI_UPRIM_T *hci_prim);
#else
#define dm_ev_connect_complete dm_ev_not_used
#define dm_ev_connect_request   dm_ev_not_used
#define dm_ev_inquiry_complete  dm_ev_not_used
#define dm_ev_inquiry_result  dm_ev_not_used
#define dm_ev_auth_complete  dm_ev_not_used
#define dm_ev_remote_name_complete  dm_ev_not_used
#define dm_ev_change_conn_link_key_complete  dm_ev_not_used
#define dm_ev_read_rem_supp_features_complete  dm_ev_not_used
#define dm_ev_qos_setup_complete  dm_ev_not_used
#define dm_ev_role_change_event  dm_ev_not_used
#define dm_ev_mode_change_event  dm_ev_not_used
#define dm_ev_pin_code_request  dm_ev_not_used
#define dm_ev_link_key_request  dm_ev_not_used
#define dm_ev_link_key_notification  dm_ev_not_used
#define dm_ev_read_clock_offset_complete  dm_ev_not_used
#define dm_ev_conn_packet_type_changed  dm_ev_not_used
#define dm_ev_inquiry_result_with_rssi  dm_ev_not_used
#define dm_ev_flow_spec_complete  dm_ev_not_used
#define dm_ev_read_rem_ext_features_complete  dm_ev_not_used
#define dm_ev_sniff_sub_rate  dm_ev_not_used
#define dm_ev_extended_inquiry_result  dm_ev_not_used
#define dm_ev_io_capability_request  dm_ev_not_used
#define dm_ev_io_capability_response  dm_ev_not_used
#define dm_ev_user_confirmation_request  dm_ev_not_used
#define dm_ev_user_passkey_request  dm_ev_not_used
#define dm_ev_remote_oob_data_request  dm_ev_not_used
#define dm_ev_user_passkey_notification  dm_ev_not_used
#define dm_ev_keypress_notification  dm_ev_not_used
#define dm_ev_lst_change  dm_ev_not_used
#define dm_ev_rem_host_supported_features  dm_ev_not_used
#define dm_ev_enhanced_flush_complete  dm_ev_not_used
#endif

#ifdef INSTALL_DM_SYNC_MODULE
static void dm_ev_synch_connection_complete(HCI_UPRIM_T *hci_prim);
static void dm_ev_synch_connection_changed(HCI_UPRIM_T *hci_prim);
#else
#define dm_ev_synch_connection_complete dm_ev_not_used
#define dm_ev_synch_connection_changed dm_ev_not_used
#endif

#ifdef BUILD_FOR_HOST
static void dm_ev_number_completed_packets(HCI_UPRIM_T *hci_prim);
#else
#define dm_ev_number_completed_packets dm_ev_not_used
#endif

/*
 * Private Data.
 */
/*! \brief Queues of commands: waiting to be sent to the device;
           already sent and waiting for Command Status/Complete;
           and that have received Command Status, and are now waiting
           for the appropriate completion event (not Command Complete).*/
static HCI_QUEUE_T *command_queue[dm_hci_num_queues];

/*! Allow one HCI command to be sent - this is just in case the Host
    Controller does not send a COMMAND_STATUS NOP on startup */
static CsrUint16 command_packets_available = 1;

/*! \brief Function pointer table for HCI event handling. */
static const HCI_EV_FUNCTION_T hci_ev_function_ptr[] =
{
    dm_ev_not_used,                         /* not used */
    dm_ev_inquiry_complete,                 /* 1 HCI_EV_INQUIRY_COMPLETE */
    dm_ev_inquiry_result,                   /* 2 HCI_EV_INQUIRY_RESULT */
    dm_ev_connect_complete,                 /* 3 HCI_EV_CONN_COMPLETE */
    dm_ev_connect_request,                  /* 4 HCI_EV_CONN_REQUEST */
    dm_ev_disconnect_complete,              /* 5 HCI_EV_DISCONNECT_COMPLETE */
    dm_ev_auth_complete,                    /* 6 HCI_EV_AUTH_COMPLETE */
    dm_ev_remote_name_complete,             /* 7 HCI_EV_REMOTE_NAME_REQ_COMPLETE */
    dm_ev_encryption_change,                /* 8 HCI_EV_ENCRYPTION_CHANGE */
    dm_ev_change_conn_link_key_complete,    /* 9 HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE */
    Send_Master_Link_Key_Complete,          /* a HCI_EV_MASTER_LINK_KEY_COMPLETE */
    dm_ev_read_rem_supp_features_complete,  /* b HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE */
    dm_ev_read_remote_ver_info_complete,    /* c HCI_EV_READ_REMOTE_VER_INFO_COMPLETE */
    dm_ev_qos_setup_complete,            /* d HCI_EV_QOS_SETUP_COMPLETE */
    dm_ev_command_complete,                 /* e HCI_EV_COMMAND_COMPLETE */
    dm_ev_command_status,                   /* f HCI_EV_COMMAND_STATUS */
    dm_ev_not_used,                         /* 10 HCI_EV_HARDWARE_ERROR */
    dm_ev_not_used,                         /* 11 HCI_EV_FLUSH_OCCURRED */
    dm_ev_role_change_event,                /* 12 HCI_EV_ROLE_CHANGE */
    dm_ev_number_completed_packets,         /* 13 HCI_EV_NUMBER_COMPLETED_PKTS */
    dm_ev_mode_change_event,                /* 14 HCI_EV_MODE_CHANGE */
    Send_Return_Link_Keys_Complete,         /* 15 HCI_EV_RETURN_LINK_KEYS */
    dm_ev_pin_code_request,                 /* 16 HCI_EV_PIN_CODE_REQ */
    dm_ev_link_key_request,                 /* 17 HCI_EV_LINK_KEY_REQ */
    dm_ev_link_key_notification,            /* 18 HCI_EV_LINK_KEY_NOTIFICATION */
    dm_ev_not_used,                         /* 19 HCI_EV_LOOPBACK_COMMAND */
    dm_ev_not_used,                         /* 1a HCI_EV_DATA_BUFFER_OVERFLOW */
    dm_ev_not_used,                         /* 1b HCI_EV_MAX_SLOTS_CHANGE */
    dm_ev_read_clock_offset_complete,       /* 1c HCI_EV_READ_CLOCK_OFFSET_COMPLETE */
    dm_ev_conn_packet_type_changed,         /* 1d HCI_EV_CONN_PACKET_TYPE_CHANGED */
    dm_ev_not_used,                         /* 1e HCI_EV_QOS_VIOLATION */
    dm_ev_not_used,                         /* 1f HCI_EV_PAGE_SCAN_MODE_CHANGE */
    dm_ev_not_used,                         /* 20 HCI_EV_PAGE_SCAN_REP_MODE_CHANGE */
    dm_ev_flow_spec_complete,               /* 21 flow spec complete */
    dm_ev_inquiry_result_with_rssi,         /* 22 inquiry result with rssi */
    dm_ev_read_rem_ext_features_complete,   /* 23 remote extended features complete */
    dm_ev_not_used,                         /* 24 ???? */
    dm_ev_not_used,                         /* 25 ???? */
    dm_ev_not_used,                         /* 26 ???? */
    dm_ev_not_used,                         /* 27 ???? */
    dm_ev_not_used,                         /* 28 ???? */
    dm_ev_not_used,                         /* 29 ???? */
    dm_ev_not_used,                         /* 2a ???? */
    dm_ev_not_used,                         /* 2b ???? */
    dm_ev_synch_connection_complete,        /* 2c HCI_EV_SYNC_CONN_COMPLETE */
    dm_ev_synch_connection_changed,         /* 2d HCI_EV_SYNC_CONN_CHANGED */
    dm_ev_sniff_sub_rate,                   /* 2e HCI_EV_SNIFF_SUB_RATE */
    dm_ev_extended_inquiry_result,          /* 2f HCI_EV_EXTENDED_INQUIRY_RESULT */
    dm_ev_refresh_encryption_key_complete,  /* 30 HCI_REFRESH_ENCRYPTION_KEY_COMPLETE */
    dm_ev_io_capability_request,            /* 31 HCI_EV_IO_CAPABILITY_REQUEST */
    dm_ev_io_capability_response,           /* 32 HCI_EV_IO_CAPABILITY_RESPONSE */
    dm_ev_user_confirmation_request,        /* 33 HCI_EV_USER_CONFIRMATION_REQUEST */
    dm_ev_user_passkey_request,             /* 34 HCI_EV_USER_PASSKEY_REQUEST */
    dm_ev_remote_oob_data_request,          /* 35 HCI_EV_REMOTE_OOB_DATA_REQUEST */
    dm_sm_hci_simple_pairing_complete,      /* 36 HCI_EV_SIMPLE_PAIRING_COMPLETE */
    dm_ev_not_used,                         /* 37 ???? */
    dm_ev_lst_change,                       /* 38 HCI_EV_LST_CHANGE  */
    dm_ev_enhanced_flush_complete,          /* 39 HCI_ENHANCED_FLUSH_COMPLETE */
    dm_ev_not_used,                         /* 3A ???? */
    dm_ev_user_passkey_notification,        /* 3B HCI_EV_USER_PASSKEY_NOTIFICATION */
    dm_ev_keypress_notification,            /* 3C HCI_EV_KEYPRESS_NOTIFICATION */
    dm_ev_rem_host_supported_features       /* 3D HCI_EV_REM_HOST_SUPPORTED_FEATURES */
};

#ifndef BUILD_FOR_HOST
/*
 *****************************************************************************
 * IMPORTANT:                                                                *
 * If you get a compiler error for the line below like 'Array Size is        *
 * negative' then the table above is out of step with hci.h.  You need to    *
 * think carefully about whether you can stub out the event with a           *
 * "not_used" or if we need to add code to handle it.                        *
 *****************************************************************************
*/
CHECK_CSR_ARRAY_SIZE(hci_ev_function_ptr, HCI_MAX_EVENT_OPCODE);
#endif

#ifndef BUILD_FOR_HOST
#if !defined(DISABLE_DM_BREDR) || defined (INSTALL_ULP)
/*! \brief Decide if there's enough room in the upstream message queue for
           another primitive.
    \returns The upstream message queue ID, if there is room, or NO_QID if
           there isn't.
*/
static CsrSchedQid dm_hci_get_upstream_queue_unless_full(void)
{
    CsrSchedQid q = am_phandle & (CsrSchedQid)0x8000 ? DM_TOHOSTQUEUE : am_phandle;

    return message_queue_length(q) < EVENTS_QUEUE_LEN_LIMIT ? q : NO_QID;
}
#endif /* !defined(DISABLE_DM_BREDR) || defined (INSTALL_ULP) */
#endif /* ndef BUILD_FOR_HOST */

/*===========================================================================*
 Public Function Implementations
*============================================================================*/

/*! \brief Interface handler for HCI ACL data packets at bottom of DM.
    \param data_prim Pointer to HCI ACL data packet.
    \returns TRUE if packet consumed, otherwise FALSE.
*/
#ifndef BUILD_FOR_HOST
static
#endif
CsrBool dm_hci_l2cap_data(
#ifdef INSTALL_AMP_SUPPORT
        l2ca_controller_t controller,
        CsrUint8 physical_link_id,
#endif
        CsrUint16 logical_link_id,
        CsrMblk *mblk)
{
    DM_ACL_T *p_acl;
    CsrBool broadcast;

#ifdef INSTALL_AMP_SUPPORT
    if (controller != L2CA_AMP_CONTROLLER_BT)
    {
        LOGIC_Q_LOOKUP_T *pl;

        /* Explicit cast to (LOGIC_Q_LOOKUP_T*) because lint can't
           seem to work it out by itself. */
        if ((pl = (LOGIC_Q_LOOKUP_T*)
                    *AMP_LogicalLookup(logical_link_id)) == NULL)
            return FALSE;

        /* No fragmentation or broadcast on AMP */
        CH_DataRx(pl->chcb, mblk, FALSE);

        return TRUE;
    }
#endif

#ifdef BUILD_FOR_HOST

    /* L2CA_AclReassemble always consumes data, so we return TRUE. */
    if ((mblk = L2CA_AclReassemble(logical_link_id, mblk)) == NULL)
        return TRUE;

#else /* BUILD_FOR_HOST */

    if((logical_link_id & HCI_PKT_BOUNDARY_FLAG_CONT) != 0)
        return FALSE;

    /*  the message we're about to take out will allow
        radioio to post another, so give it a kick.

        Note that this kick may not always be needed because the
        LC will see the buffer space appear, receive an ACL packet
        and hence kick radioio.

        TODO. Is this still needed now that we've extirpated
        the incqueue decqueue stuff?
    */
    radioio_bg_int();

#endif /* BUILD_FOR_HOST */

    if ((p_acl = dm_acl_find_by_handle((hci_connection_handle_t)
                    (logical_link_id & HCI_CONNECTION_HANDLE_MASK))) == NULL
            || p_acl->connection_state == ACL_ST_DISCONNECTING)
        return FALSE;


    broadcast = TRUE;
    if((logical_link_id & HCI_BROADCAST_FLAG_MASK) == 0)
    {
        broadcast = FALSE;
#ifndef DISABLE_DM_BREDR
        acl_activity(p_acl);
#endif
    }
#ifndef INSTALL_L2CAP_CONNLESS_SUPPORT
    else
        return FALSE;
#endif

    CH_DataRx(&p_acl->dm_acl_client_l2cap, mblk, broadcast);
    return TRUE;
}

#ifndef BUILD_FOR_HOST
static
#endif
void dm_hci_event_handler(HCI_UPRIM_T* hci_prim)
{
    /* No entry in function table for HCI_DEBUG events! */
#ifdef INSTALL_ULP
    if (hci_prim->event_code == HCI_EV_ULP)
    {
        /* ULP Events. */
        switch(hci_prim->hci_ulp_event.ulp_sub_opcode)
        {
            case HCI_EV_ULP_CONNECTION_COMPLETE:
                dm_acl_ulp_opened(&hci_prim->hci_ulp_connection_complete);
                break;

            case HCI_EV_ULP_ADVERTISING_REPORT:
#ifndef BUILD_FOR_HOST
                if (dm_hci_get_upstream_queue_unless_full() != NO_QID)
#endif
                {
                    DM_HCI_ULP_ADVERTISING_REPORT_IND_T *prim =
                            zpnew (DM_HCI_ULP_ADVERTISING_REPORT_IND_T);

                    prim->type          = DM_HCI_ULP_ADVERTISING_REPORT_IND;
                    prim->num_reports   = hci_prim->hci_ulp_advertising_report.num_reports;
                    prim->event_type    = hci_prim->hci_ulp_advertising_report.event_type;
                    tbdaddr_copy_from_bd_addr(
                            &prim->current_addrt,
                            hci_prim->hci_ulp_advertising_report.address_type,
                            &hci_prim->hci_ulp_advertising_report.address);

                    if (hci_prim->hci_ulp_advertising_report.length_data > 0
                            && hci_prim->hci_ulp_advertising_report.length_data <= 31)
                    {
                        prim->length_data = hci_prim->hci_ulp_advertising_report.length_data;
                        prim->data = pcopy(hci_prim->hci_ulp_advertising_report.data,
                                           hci_prim->hci_ulp_advertising_report.length_data);
                    }
                    prim->rssi          = hci_prim->hci_ulp_advertising_report.rssi;

                    sm_handle_advertising_report(prim);
                }
                break;

            case HCI_EV_ULP_CONNECTION_UPDATE_COMPLETE:
            {
                DM_ACL_T *p_acl;

                if ((p_acl = dm_acl_find_by_handle(hci_prim->
                        hci_ulp_connection_update_complete.connection_handle))
                                != NULL)
                {
                    DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T *prim =
                        zpnew(DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T);

                    prim->type = DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND;
                    prim->status =
                            hci_prim->hci_ulp_connection_update_complete.status;

                    tbdaddr_copy(&prim->addrt, &p_acl->addrt);
                    prim->conn_interval = hci_prim->hci_ulp_connection_update_complete.conn_interval;
                    prim->conn_latency = hci_prim->hci_ulp_connection_update_complete.conn_latency;
                    prim->supervision_timeout = hci_prim->hci_ulp_connection_update_complete.supervision_timeout;

                    DM_SendMessageAMphandle(prim);

                    if ( remove_command_from_completing_list(HCI_ULP_CONNECTION_UPDATE,
                                                             NULL,
                                                             p_acl->handle) == DM_HCI_SENDER_ACL_MANAGER )
                    {
                        send_dm_ble_update_connection_parameters_cfm(
                                    hci_prim->hci_ulp_connection_update_complete.status,
                                    &p_acl->addrt);
                    }
                }
                break;
            }

#ifdef INSTALL_SM_MODULE
#ifndef DISABLE_SM_SLAVE
            case HCI_EV_ULP_LONG_TERM_KEY_REQUESTED:
            {
                sm_pairing_ltk_req(
                        hci_prim->hci_ulp_long_term_key_requested.
                        connection_handle,
                        hci_prim->hci_ulp_long_term_key_requested.
                        random_number,
                        hci_prim->hci_ulp_long_term_key_requested.
                        encryption_diversifier);
                break;
             }
#endif
#endif

            default:
                break;
        }
    }
    else
#endif
    if (hci_prim->event_code != HCI_EV_MANUFACTURER_EXTENSION)
        hci_ev_function_ptr[hci_prim->event_code](hci_prim);

    /* Task is about to end, so clear away any dead ACLs */
    dm_acl_cleanup();
}


/*! \brief Interface handler for all HCI events at bottom of DM.
    This now handles l2cap data as well.

    \param gash Unused.
*/
#ifndef BUILD_FOR_HOST
void CsrBtDmHciHandler(void **gash)
{
    CsrUint16 m;
    DM_DATA_FROM_HCI_REQ_T *prim;

    CSR_UNUSED(gash);

    CsrSchedMessageGet(&m , (void**)&prim);

    if (m == DM_PRIM)
    {
        if (!DM_HCI_L2CAP_DATA(0, 0, prim->logical_handle, prim->data))
        {
            /* Data has not been consumed */
            BLUESTACK_WARNING(CON_HANDLE_ERR);
            CsrMblkDestroy(prim->data);
        }
    }
    else
        dm_hci_event_handler((HCI_UPRIM_T*)prim);

    /* Free the primitive */
    CsrPmemFree(prim);
}
#endif

/*! \brief Send HCI command accross HCI, filling in the command length
    field on the way. The function takes ownership of the memory
    in pv_hci_uprim, so the caller must not free it.

    \param Pointer to pv_hci_uprim HCI command to be sent.
*/
void send_to_hci(DM_UPRIM_T *pv_hci_uprim)
{
    add_to_hci_queue(dm_hci_pending, (HCI_UPRIM_T*)pv_hci_uprim);
    tx_commands_to_hci();
}

/*! \brief Converts the BD address in the command into the correct HCI
    connection handle, then sends the command to the HCI. This function will
    only be called for HCI commands that require an ACL connection handle. Any
    commands that require a SCO connection handle must have had the handle
    filled in by the sender of the command, as there can be multiple SCO
    connections to one remote BD address.

    \param pv_hci_uprim Pointer to HCI command to be sent.
    \return TRUE if OK, FALSE if handle could not be found.
*/
void send_to_hci_with_handle(DM_UPRIM_T *uprim)
{
    DM_ACL_T *p_acl = dm_hci_handle_and_tbdaddr(uprim->type)
        ? dm_acl_find_by_tbdaddr(&uprim->dm_hci_ulp_with_handle.addrt)
        : dm_acl_find_by_bd_addr(&uprim->dm_hci_with_handle.bd_addr);

    /* Get the ACL connection handle for this BD address */
    if (p_acl == NULL || p_acl->handle == HCI_HANDLE_INVALID)
    {
        /* This will ignore commands that did not come from the application,
           so no need to check here. */
        dm_unhandled_command(uprim, HCI_ERROR_NO_CONNECTION);

        dm_hci_free_downstream_primitive((HCI_UPRIM_T*)uprim);
    }
    else
    {
        uprim->dm_hci_with_handle.handle = p_acl->handle;
        send_to_hci(uprim);
    }
}

#ifndef DISABLE_DM_BREDR
/*! \brief Attempts to fill in connection parameters to help speed up
    connection time, if they are available in the cache. Then sends the
    command to HCI.

    \param pv_hci_uprim Pointer to HCI command to be sent.
*/
void send_rnr_to_hci_with_conn_params(const DM_UPRIM_T *const p_uprim)
{
    HCI_REMOTE_NAME_REQ_T *p_prim =
        pcopy(p_uprim, sizeof(HCI_REMOTE_NAME_REQ_T));

    p_prim->common.op_code = HCI_REMOTE_NAME_REQ;
    p_prim->common.length = DM_HCI_SENDER_APPLICATION;
    dm_acl_obtain_connection_details(&p_prim->bd_addr, &p_prim->page_scan_mode,
            &p_prim->page_scan_rep_mode, &p_prim->clock_offset);

    send_to_hci((DM_UPRIM_T*)p_prim);
}
#endif

/*! \param handle Connection handle of link to be disconnected.
    \param reason Reason for disconnection.
*/
void Send_HCI_Disconnect(const hci_connection_handle_t handle,
        const hci_error_t reason, const CsrUint8 sender)
{
    HCI_DISCONNECT_T *p_prim;

    p_prim = pnew(HCI_DISCONNECT_T);
    p_prim->common.op_code = HCI_DISCONNECT;
    p_prim->common.length = sender;
    p_prim->handle = handle;
    p_prim->reason = reason;

    send_to_hci((DM_UPRIM_T*)p_prim);
}

/*! \param reason Reason for rejecting connection.
    \param bd_addr Pointer to Bluetooth address of remote device.
*/
void Send_HCI_Reject_Connection(const BD_ADDR_T *const bd_addr,
                                const CsrUint8 sender)
{
    HCI_REJECT_CONNECTION_REQ_T *p_prim;

    p_prim = pnew(HCI_REJECT_CONNECTION_REQ_T);
    p_prim->common.op_code = HCI_REJECT_CONNECTION_REQ;
    p_prim->common.length = sender;
    p_prim->reason = HCI_ERROR_REJ_BY_REMOTE_NO_RES;
    bd_addr_copy(&p_prim->bd_addr, bd_addr);

    send_to_hci((DM_UPRIM_T*)p_prim);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Change option to authenticate during link setup.

    \param auth_enable Whether to authenticate during link setup.
*/
void send_hci_write_auth_enable(const CsrUint8 auth_enable, const CsrUint8 sender)
{
    HCI_WRITE_AUTH_ENABLE_T *p_prim;

    p_prim = pnew(HCI_WRITE_AUTH_ENABLE_T);
    p_prim->common.op_code = HCI_WRITE_AUTH_ENABLE;
    p_prim->common.length = sender;
    p_prim->auth_enable = auth_enable;

    send_to_hci((DM_UPRIM_T *)p_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \param bd_addr Pointer to Bluetooth address of remote device.
    \param reason Reason for rejecting connection.
*/
void Send_HCI_Reject_Synchronous_Connection(
        const hci_reason_t reason,
        const BD_ADDR_T *const bd_addr,
        const CsrUint8 sender)
{
    HCI_REJECT_SYNCHRONOUS_CONN_REQ_T *p_prim;

    p_prim = pnew(HCI_REJECT_SYNCHRONOUS_CONN_REQ_T);
    p_prim->common.op_code = HCI_REJECT_SYNCHRONOUS_CONN_REQ;
    p_prim->common.length = sender;
    p_prim->reason = reason;
    bd_addr_copy(&(p_prim->bd_addr), bd_addr);

    send_to_hci((DM_UPRIM_T*)p_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param sender Entity sending HCI command. */
void Send_HCI_Delete_Stored_Link_Key(
        const BD_ADDR_T *const p_bd_addr,
        const CsrUint8 sender)
{
    HCI_DELETE_STORED_LINK_KEY_T *p_prim;
    HCI_QUEUE_T *p_item;

    /* Make sure there isn't already a suitable command on the queue.
     * This is to prevent the security manager from flooding us with
     * requests. See TF-722. */
    for (p_item = command_queue[dm_hci_pending];
            p_item != NULL;
            p_item = p_item->next_item)
    {
        HCI_UPRIM_T *p_uprim = p_item->primitive;

        if (p_uprim->op_code == HCI_DELETE_STORED_LINK_KEY)
        {
            if (p_uprim->hci_delete_stored_link_key.flag != DELETE_BDADDR)
            {
                return;
            }
            else if (p_bd_addr != NULL)
            {
                if (bd_addr_eq(p_bd_addr,
                               &p_uprim->hci_delete_stored_link_key.bd_addr))
                    return;
            }
        }
    }

    p_prim = zpnew(HCI_DELETE_STORED_LINK_KEY_T);
    p_prim->common.op_code = HCI_DELETE_STORED_LINK_KEY;
    p_prim->common.length = sender;

    if (p_bd_addr == NULL)
        p_prim->flag = DELETE_ALL;
    else
        bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    send_to_hci((DM_UPRIM_T*)p_prim);
}
#endif

/*============================================================================*
Private Function Implementations
*============================================================================*/

/*! \brief Determines whether a command can block or be blocked.
           Returns DM_HCI_BLOCKED_AND_BLOCKING, DM_HCI_BLOCKED_NOT_BLOCKING
           or DM_HCI_NOT_BLOCKED_OR_BLOCKNG, depending on command op_code.
           On returning DM_HCI_BLOCKED_AND_BLOCKING or DM_HCI_BLOCKED_NOT_BLOCKING,
           will also write Bluetooth address to p_bd_addr

    \param p_prim Pointer to HCI command primitive.
    \param p_bd_addr Pointer to return Bluetooth address.
*/
#ifndef DISABLE_DM_BREDR
static DM_HCI_BLOCKING_T blocking(HCI_UPRIM_T *p_prim, BD_ADDR_T *p_bd_addr)
{
    DM_HCI_BLOCKING_T blocking = DM_HCI_BLOCKED_AND_BLOCKING;

    switch (p_prim->op_code)
    {
        case HCI_DISCONNECT:
            dm_acl_get_bd_addr_by_handle(p_bd_addr,
                                         p_prim->hci_disconnect.handle);
            break;

        case HCI_CREATE_CONNECTION:
            blocking = DM_HCI_BLOCKED_NOT_BLOCKING;

            /* Fallthrough */
        case HCI_REMOTE_NAME_REQ:
            bd_addr_copy(p_bd_addr, &p_prim->hci_cmd_with_bd_addr.bd_addr);
            break;
        default:
            blocking = DM_HCI_NOT_BLOCKED_OR_BLOCKING;
    }

    return blocking;
}
#endif
/*! \brief Determines whether commands on a queue will allow through a blocking
           command for a given Bluetooth address.

    \param p_queue Pointer to head of HCI command queue.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
#ifndef DISABLE_DM_BREDR
static CsrBool queue_allows(HCI_QUEUE_T *p_queue, BD_ADDR_T *p_bd_addr)
{
    /* Search the sent queue for a blocking command. */
    for (/* empty */; p_queue != NULL; p_queue = p_queue->next_item)
    {
        BD_ADDR_T bd_addr_primitive;

        if (blocking(p_queue->primitive, &bd_addr_primitive)
                == DM_HCI_BLOCKED_AND_BLOCKING
                && bd_addr_eq(&bd_addr_primitive, p_bd_addr))
            return FALSE;
    }

    return TRUE;
}
#endif

/*! \brief Determine whether HCI Reset comand response is pending.
    \param None
    \returns boolean TRUE if the reset CC is pending, FALSE otherwise.
*/
static CsrBool reset_is_pending(void)
{
    HCI_QUEUE_T *p_queue;

    for (p_queue = command_queue[dm_hci_sent];
                   p_queue != NULL; p_queue = p_queue->next_item)
    {
        if(p_queue->primitive->op_code == HCI_RESET)
        {
            return TRUE;
        }
    }
    return FALSE;
}

/*! \brief Determine if we can send a command across HCI.
    \param p_prim Pointer to HCI primitive cast to (HCI_UPRIM_T*).
    \returns op_code of command, or HCI_NOP if cannot be sent.
*/
#ifdef DISABLE_DM_BREDR
#define can_send_command(p_prim) ((command_packets_available == 0 \
        || reset_is_pending())? HCI_NOP : ((p_prim)->op_code & DM_HCI_OP_CODE_MASK))
#else
static hci_op_code_t can_send_command(HCI_UPRIM_T *p_prim)
{
    BD_ADDR_T bd_addr;
    hci_op_code_t op_code = p_prim->op_code & DM_HCI_OP_CODE_MASK;

    /* Check if the HCI reset Comand Response is pending */
    if(reset_is_pending())
    {
        /* Cannot send this command */
        return HCI_NOP;
    }

    if (command_packets_available == 0)
    {
#ifndef INSTALL_BUILD_FOR_HOST_NO_OVERRIDE_HCI_CMD_FC
        /* The following commands are allowed to violate HCI
           command flow control. See HCI Implementation document.
           It is necessary to allow them to do so because the
           controller only issues 1 credit and it's possible to
           run out if two operations overlap. This can lead to
           stalled pairing or ACL/SCO connect request. */
        switch (op_code)
        {
            case HCI_ACCEPT_CONNECTION_REQ:
            case HCI_REJECT_CONNECTION_REQ:
            case HCI_ACCEPT_SYNCHRONOUS_CONN_REQ:
            case HCI_REJECT_SYNCHRONOUS_CONN_REQ:
            case HCI_LINK_KEY_REQ_REPLY:
            case HCI_LINK_KEY_REQ_NEG_REPLY:
            case HCI_PIN_CODE_REQ_REPLY:
            case HCI_PIN_CODE_REQ_NEG_REPLY:
            case HCI_IO_CAPABILITY_RESPONSE:
            case HCI_USER_CONFIRMATION_REQUEST_REPLY:
            case HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY:
            case HCI_USER_PASSKEY_REQUEST_REPLY:
            case HCI_USER_PASSKEY_REQUEST_NEG_REPLY:
            case HCI_REMOTE_OOB_DATA_REQUEST_REPLY:
            case HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY:
            case HCI_IO_CAPABILITY_REQUEST_NEG_REPLY:
            case HCI_SEND_KEYPRESS_NOTIFICATION:
            case HCI_RESET:
            case HCI_HOST_NUM_COMPLETED_PACKETS:
                break;

            default:
                /* Cannot send this command */
                return HCI_NOP;
        }
        ++command_packets_available;
#else
        /* Cannot send the command as no command credits available */
        return HCI_NOP;
#endif
    }

    if (blocking(p_prim, &bd_addr) != DM_HCI_NOT_BLOCKED_OR_BLOCKING
            && (!queue_allows(command_queue[dm_hci_sent], &bd_addr)
                || !queue_allows(command_queue[dm_hci_completing], &bd_addr)))
    {
        /* RNR, create connection and disconnect interfere with each
           other. Hold off sending any of them if there is already
           one of them being processed. */
        return HCI_NOP;
    }

    return op_code;
}
#endif

/*! \brief Tries to send pending commands to HCI. */
static void tx_commands_to_hci(void)
{
    HCI_QUEUE_T **pp_item, *p_item;

    for (pp_item = command_queue + dm_hci_pending; (p_item = *pp_item) != NULL;)
    {
        HCI_COMMAND_COMMON_T dm_hci_cmd;
        hci_op_code_t op_code;
        HCI_UPRIM_T *p_prim;

        p_prim = p_item->primitive;
        if ((op_code = can_send_command(p_prim)) == HCI_NOP)
        {
            /* Can't currently send this command. Try the next. */
            pp_item = &p_item->next_item;
            continue;
        }

        /* HCI_HOST_NUM_COMPLETED_PACKETS is a special case - don't
           decrement the number of commands we're allowed to send as
           we don't receive either COMMAND_STATUS or COMMAND_COMPLETE */
        if (op_code != HCI_HOST_NUM_COMPLETED_PACKETS)
        {
            add_to_hci_queue(dm_hci_sent, p_prim);
            --command_packets_available;
        }

        /*! Store DM-altered fields and revert originals to HCI standard. */
        dm_hci_cmd = p_prim->hci_cmd;
        p_prim->hci_cmd.length = 0;
        p_prim->hci_cmd.op_code = op_code;

        /*! Send the command. */
#ifdef BUILD_FOR_HOST
        if (CsrBtHcishimHciCommandReq(p_prim) != HCI_SUCCESS)
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#else
        CsrSchedMessagePut(LM_HCIQUEUE, 0, pcopy(p_prim, psizeof(p_prim)));
#endif

        /*! Restore DM-altered fields. */
        p_prim->hci_cmd = dm_hci_cmd;

        /* Remove from pending queue. */
        *pp_item = p_item->next_item;
        CsrPmemFree(p_item);
    }
}

/*! \brief Add primitive to one of the hci queues at back.

    \param first_qitem Start of queue to which we're adding.
    \param prim Primitive to add.
*/
static void add_to_hci_queue(DM_HCI_COMMAND_QUEUE queue, HCI_UPRIM_T *prim)
{
    HCI_QUEUE_T **pp_qitem, *p_qitem;

    /* Find end of list */
    for(pp_qitem = command_queue + queue;
            (p_qitem = *pp_qitem) != NULL;
            pp_qitem = &p_qitem->next_item)
        ;

    /* create the new queue item */
    *pp_qitem = p_qitem = pnew(HCI_QUEUE_T);

    /* Initalise new element */
    p_qitem->primitive = prim;
    p_qitem->next_item = NULL;
}

/*! \brief
    The "completing" list holds certain commands that have returned a
    Command Status(pending) event and are awaiting the appropriate
    completion event. The list is used at link loss to ensure that a DM
    response is issued upwards for incomplete commands. But when commands
    complete normally, they must be removed from the list. This function
    takes an HCI event structure and searches the "completing" list for
    a command that is expecting this event, then removes it from the list.

    All commands that are placed on this list have a connection handle and
    a BD address in the HCI structure, so they can safely be accessed via
    the HCI_WITH_CH_COMMON_T structure.

    \param op_code Command to remove.
    \param device Pointer to connection handle, or Bluetooth address.
    \returns Sender of command.
*/
static CsrUint8 remove_command_from_completing_list(
        hci_op_code_t op_code,
        BD_ADDR_T *bd_addr,
        hci_connection_handle_t handle)
{
    HCI_QUEUE_T **pp_cmd, *p_cmd;
    CsrUint8 sender = 0;

    for (pp_cmd = command_queue + dm_hci_completing;
                        (p_cmd = *pp_cmd) != NULL; pp_cmd = &p_cmd->next_item)
    {
        hci_op_code_t cmd_op_code = p_cmd->primitive->op_code
                                                        & DM_HCI_OP_CODE_MASK;
        if (op_code == cmd_op_code
#ifndef DISABLE_DM_BREDR
                || (op_code == HCI_NOP && (
                           cmd_op_code == HCI_HOLD_MODE
                        || cmd_op_code == HCI_SNIFF_MODE
                        || cmd_op_code == HCI_EXIT_SNIFF_MODE
                        || cmd_op_code == HCI_PARK_MODE
                        || cmd_op_code == HCI_EXIT_PARK_MODE))
#endif
                )
        {
#ifndef DISABLE_DM_BREDR
            /* Match remote device. */
            if (op_code == HCI_REMOTE_NAME_REQ
                    || op_code == HCI_CREATE_CONNECTION)
            {
                /* Check Bluetooth addresses match */
                if (!bd_addr_eq(bd_addr,
                            &p_cmd->primitive->hci_cmd_with_bd_addr.bd_addr))
                    continue;
            }
            else
#endif
            {
                /* Check the connection handles match */
                if (handle != p_cmd->primitive->hci_cmd_with_handle.handle)
                    continue;

                /* They might want to know the Bluetooth address. */
                if (bd_addr != NULL)
                {
                    if ( dm_hci_handle_and_tbdaddr(op_code) )
                        tbdaddr_copy_to_bd_addr(bd_addr,&p_cmd->primitive->hci_ulp_with_handle.addrt);
                    else
                        bd_addr_copy(bd_addr,&p_cmd->primitive->hci_cmd_with_handle.bd_addr);
                }
            }

            /* We craftily stored the identity of the sender in the length
               field, which we don't otherwise use in Bluestack. */
            sender = p_cmd->primitive->hci_cmd.length;

            /* Primitives on the completing list do not have valid pointers.
               We must just do a basic free. */
            CsrPmemFree(p_cmd->primitive);

            /* Remove command from list and stop searching */
            *pp_cmd = p_cmd->next_item;
            CsrPmemFree(p_cmd);
            break;
        }
    }

    /* Completed command may allow us to send some more. */
    tx_commands_to_hci();

    return sender;
}

/*! \brief Cancels all pending commands for a connection handle on all queues.

    \param handle connection handle.
*/
static void cancel_pending_commands(hci_connection_handle_t handle)
{
    DM_HCI_COMMAND_QUEUE i;

    /* xapncc ignore '=': cast of 'int' to differing enum */
    for(i = dm_hci_first_queue; i < dm_hci_num_queues; ++i)
    {
        HCI_QUEUE_T *p_queue, **pp_queue;

        pp_queue = command_queue + i;
        while ((p_queue = *pp_queue) != NULL)
        {
            HCI_UPRIM_T *p_prim = p_queue->primitive;

            /* Check if command is for lost link. */
            if (((p_prim->op_code & DM_HCI_WITH_HANDLE) != 0
                        && p_prim->hci_cmd_with_handle.handle == handle))
            {
                /* Now issue a negative confirmation for the command.
                   Commands that didn't come from the application get
                   ignored by this function. (So no check here.) */
                dm_unhandled_command((DM_UPRIM_T*)p_prim,
                                     HCI_ERROR_NO_CONNECTION);

                /* The pending queue contains actual commands waiting to be
                   sent across HCI. Any pointers are genuine and point to
                   real memory that must be freed. The other queues contain
                   carbon copies of primitives that have already been sent.
                   The pointers must not be freed because they point to
                   memory that is now owned by lower layers. */
                if (i == dm_hci_pending)
                    dm_hci_free_downstream_primitive(
                            (HCI_UPRIM_T*)p_queue->primitive);
                else
                    CsrPmemFree(p_queue->primitive);

                /* Remove the command from the list */
                *pp_queue = p_queue->next_item;
                CsrPmemFree(p_queue);
            }
            else
                pp_queue = &p_queue->next_item;
        }
    }
}

/*****************************************************************************
 *   hci events
 *****************************************************************************/

/*! \brief Common parts of HCI command complete and command status handling.

    \param op_code Command that caused the event.
    \returns Pointer to matching command (or NULL if something went wrong).
*/
HCI_UPRIM_T *hci_cc_cs_common(hci_op_code_t op_code)
{
    HCI_QUEUE_T **pp_cmd, *p_cmd;
    HCI_UPRIM_T *prim = NULL;

    if(op_code == HCI_NOP)
    {
        /* 0x0000 Indicates that the controller is ready. */
        LYMCB_SET(LYM_ControllerReady);
        LYM_InterrogateController();
    }
    else
    {
        /* Check head of queue to see if it matches, otherwise discard */
        for (pp_cmd = command_queue + dm_hci_sent; (p_cmd = *pp_cmd) != NULL;
                                                    pp_cmd = &p_cmd->next_item)
        {
            /*  Look for matching primitive.
                This code previously only checked for the
                first non-HCI_HOST_NUM_COMPLETED_PACKETS
                op-code, but is incorrect because commands
                can complete out of order.
                We now look for the first entry in the queue
                which matches our op-code.
                We assume that if two of the same command
                are issued, they either complete or
                generate a command status in the correct order.
                If this were not true, we would need to check
                handle or BDADDR match - yuk! */
            if ((p_cmd->primitive->op_code & DM_HCI_OP_CODE_MASK) == op_code)
            {
                /* remove from list and return */
                prim = p_cmd->primitive;
                *pp_cmd = p_cmd->next_item;
                CsrPmemFree(p_cmd);
                break;
            }
        }
    }

    return prim;
}

/*! \brief Deal with hci_command_complete events.

    \param hci_prim Pointer to command complete event primitive.
*/
static void dm_ev_command_complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_COMMAND_COMPLETE_T *prim = (HCI_EV_COMMAND_COMPLETE_T *)hci_prim;
    HCI_UPRIM_T *ret_prim;

    command_packets_available = prim->num_hci_command_pkts;

    if ((ret_prim = hci_cc_cs_common(prim->op_code)) != NULL)
    {
        if ((ret_prim->hci_cmd.length & DM_HCI_SENDER_APPLICATION) != 0)
            handle_hci_cc_application(prim, ret_prim);

        if ((ret_prim->hci_cmd.length & DM_HCI_SENDER_BLUESTACK) != 0)
            handle_hci_cc_bluestack(prim, ret_prim);

        /* free returned primitive, don't need it anymore */
        CsrPmemFree(ret_prim);
    }

    /* Always need to free argument_ptr. prim itself is freed by
       CsrBtDmHciHandler() after we return. */
    CsrPmemFree(prim->argument_ptr);

    /* Try to send any queued commands immediately */
    tx_commands_to_hci();
}

/*! \brief Deal with hci_command_status events.

    \param hci_prim Pointer to command status event primitive.
*/
static void dm_ev_command_status(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_COMMAND_STATUS_T *prim = (HCI_EV_COMMAND_STATUS_T *)hci_prim;
    HCI_UPRIM_T *ret_prim;

    command_packets_available = prim->num_hci_command_pkts;

    if ((ret_prim = hci_cc_cs_common(prim->op_code)) != NULL)
    {
        /* If successful, move certain commands onto completing list
           (just those commands for which we wish to guarantee a response
           to the application even if the link drops). */
        if (prim->status == HCI_COMMAND_CURRENTLY_PENDING)
        {
            switch (prim->op_code)
            {
#ifndef DISABLE_DM_BREDR
                case HCI_CREATE_CONNECTION:
                case HCI_ENHANCED_FLUSH:
                case HCI_SET_CONN_ENCRYPTION:
                case HCI_CHANGE_CONN_LINK_KEY:
                case HCI_REMOTE_NAME_REQ:
                case HCI_READ_REMOTE_SUPP_FEATURES:
                case HCI_READ_CLOCK_OFFSET:
                case HCI_HOLD_MODE:
                case HCI_SNIFF_MODE:
                case HCI_EXIT_SNIFF_MODE:
                case HCI_PARK_MODE:
                case HCI_EXIT_PARK_MODE:
                case HCI_QOS_SETUP:
                case HCI_FLOW_SPEC:
                case HCI_READ_REMOTE_EXT_FEATURES:
#endif
                case HCI_DISCONNECT:
                case HCI_REFRESH_ENCRYPTION_KEY:
                case HCI_READ_REMOTE_VER_INFO:
#ifdef INSTALL_ULP
                case HCI_ULP_CONNECTION_UPDATE:
                case HCI_ULP_READ_CHANNEL_MAP:
                case HCI_ULP_READ_REMOTE_USED_FEATURES:
#endif
                    add_to_hci_queue(dm_hci_completing, ret_prim);
                    ret_prim = NULL;
            }
        }
        else
            handle_hci_cs_fail(prim, ret_prim);

        /* free returned primitive, don't need it anymore */
        CsrPmemFree(ret_prim);
    }

    /* Try to send any queued commands immediately */
    tx_commands_to_hci();
}

/*! \brief Catch unhandled events.

    \param prim Pointer to primitive of unhandled event (unused).
*/
void dm_ev_not_used(HCI_UPRIM_T *prim)
{
    CSR_UNUSED(prim);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Handle the inquiry complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_inquiry_complete(HCI_UPRIM_T *hci_prim)
{
    DM_SendStandardCommandComplete(DM_HCI_INQUIRY_CFM,
                                hci_prim->hci_inquiry_complete_event.status);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle the inquiry result event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_inquiry_result(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_INQUIRY_RESULT_T *p_prim = (HCI_EV_INQUIRY_RESULT_T *) hci_prim;
    CsrUint8 i;
    CsrUint8 res_index, inq_index;

#ifndef BUILD_FOR_HOST
    if (dm_hci_get_upstream_queue_unless_full() == NO_QID)
    {
        /*
         * Toss away the inquiry result if we have too many already waiting
         * to be processed.
         */
        pdestroy_array( (void **) p_prim->result, HCI_MAX_INQ_RESULT_PTRS);
        return;
    }
#endif

    /* Update the cached connection parameter table */
    for (i = 0; i < p_prim->num_responses; i++)
    {
        res_index = i / HCI_MAX_INQ_RESULT_PER_PTR;
        inq_index = i - (res_index * HCI_MAX_INQ_RESULT_PER_PTR);

        dm_acl_update_cache(UPDATE_CLOCK + UPDATE_PAGE,
            &(p_prim->result[res_index][inq_index].bd_addr),
            p_prim->result[res_index][inq_index].clock_offset,
            p_prim->result[res_index][inq_index].page_scan_rep_mode);
    }

    /* pass the results on */
    Send_DM_Inquiry_Result(hci_prim);
}
#endif

/*! \brief Handle the connect complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
#ifndef DISABLE_DM_BREDR
static void dm_ev_connect_complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_CONN_COMPLETE_T *p_prim = (HCI_EV_CONN_COMPLETE_T *) hci_prim;

    remove_command_from_completing_list(HCI_CREATE_CONNECTION,
            &p_prim->bd_addr, HCI_HANDLE_INVALID);

    /* Inform the ACL or SCO entity of the connection result.
       'link_type' is either HCI_LINK_TYPE_ACL or HCI_LINK_TYPE_SCO.
       It is never HCI_LINK_TYPE_ESCO. */
#ifdef INSTALL_DM_SYNC_MODULE
    if (HCI_LINK_TYPE_SCO == p_prim->link_type)
    {
        HCI_EV_SYNC_CONN_COMPLETE_T sync_prim;

        /* This probably means that an incoming SCO connection
           request has been auto-accepted by the LM as a result
           of an event filter. We should assume default parameters. */

        /* Primitives identical until just before tx_interval.
           The rest is zero. */
        CsrMemSet(&sync_prim, 0, sizeof(HCI_EV_SYNC_CONN_COMPLETE_T));
        CsrMemCpy(&sync_prim, p_prim,
                        offsetof(HCI_EV_SYNC_CONN_COMPLETE_T, tx_interval));
        dm_sync_hci_connect_complete(&sync_prim);
    }
    else
#endif
    /* It's not SCO so it must be ACL. */
    if (p_prim->status == HCI_SUCCESS)
    {
        dm_acl_hci_connect_success(&p_prim->bd_addr, p_prim->handle,
                p_prim->enc_mode);
    }
    else
    {
        dm_acl_hci_connect_failed(&p_prim->bd_addr, p_prim->status);
    }

}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle the connect request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_connect_request(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_CONN_REQUEST_T *p_prim = (HCI_EV_CONN_REQUEST_T *) hci_prim;

    switch (p_prim->link_type)
    {
        case HCI_LINK_TYPE_ACL:
            dm_acl_hci_connect_request(p_prim);
            break;

        case HCI_LINK_TYPE_SCO:
        case HCI_LINK_TYPE_ESCO:
#ifdef INSTALL_DM_SYNC_MODULE
            dm_sync_hci_conn_request(p_prim);
#else
        /* reject the connection immediately */
        Send_HCI_Reject_Synchronous_Connection(
                    HCI_ERROR_REJ_BY_REMOTE_NO_RES,
                    &p_prim->bd_addr,
                    DM_HCI_SENDER_SYNC_MANAGER);
#endif
            break;
    }
}
#endif

/*! \brief
    Handle the disconnect complete event. If the link has been disconnected,
    check the list of pending HCI commands for any commands relating to the
    lost link and issue negative confirmations for them.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_disconnect_complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_DISCONNECT_COMPLETE_T *p_prim =
                                 (HCI_EV_DISCONNECT_COMPLETE_T *) hci_prim;
    hci_error_t reason = p_prim->reason;
    hci_return_t status = p_prim->status;
    hci_connection_handle_t handle = p_prim->handle;

    remove_command_from_completing_list(HCI_DISCONNECT, NULL, handle);

    /* Notify the ACL or SCO manager of the event */
    if (dm_acl_find_by_handle(handle))
    {
        if (status == HCI_SUCCESS)
        {
            dm_acl_hci_disconnect_success(handle, reason);

            /* Cancel pending commands from the three command queues */
            cancel_pending_commands(handle);
        }
        else
            dm_acl_hci_disconnect_failure(p_prim->handle);
    }
#ifdef INSTALL_DM_SYNC_MODULE
    else if (dm_sync_find_by_handle(handle, NULL))
    {
        if (status == HCI_SUCCESS)
            dm_sync_hci_disconnect_success(handle, reason);
        else
            dm_sync_hci_disconnect_failure(handle, status, reason);
    }
#endif
}

#ifndef DISABLE_DM_BREDR
/*! \brief Handle auth complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_auth_complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_AUTH_COMPLETE_T *p_prim = (HCI_EV_AUTH_COMPLETE_T *) hci_prim;
    hci_connection_handle_t handle = p_prim->handle;
    hci_return_t status = p_prim->status;

    if (status == HCI_SUCCESS)
        dm_sm_hci_auth_success(handle);
    else
        dm_sm_hci_auth_failure(handle, status);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle name request complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_remote_name_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_REMOTE_NAME_REQ_COMPLETE_T *hci_prim =
                            (HCI_EV_REMOTE_NAME_REQ_COMPLETE_T *)hci_uprim;

    remove_command_from_completing_list(HCI_REMOTE_NAME_REQ,
            &hci_prim->bd_addr, HCI_HANDLE_INVALID);

    Send_DM_HCI_Remote_Name_Cfm(hci_prim->status, &hci_prim->bd_addr,
            hci_prim->name_part);
}
#endif

/*! \brief Handle encryption change event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_encryption_change(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_ENCRYPTION_CHANGE_T *p_prim =
                                (HCI_EV_ENCRYPTION_CHANGE_T *) hci_prim;
    hci_connection_handle_t handle = p_prim->handle;

    remove_command_from_completing_list(HCI_SET_CONN_ENCRYPTION, NULL, handle);

    dm_sm_hci_enc_complete(p_prim->handle, p_prim->status, p_prim->enc_enable);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Handle connection link key change complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_change_conn_link_key_complete(HCI_UPRIM_T *hci_prim)
{
    BD_ADDR_T bd_addr;

    remove_command_from_completing_list(HCI_CHANGE_CONN_LINK_KEY, &bd_addr,
            hci_prim->hci_change_conn_linkkey_coml_event.handle);

    DM_SendBDADDRCommandComplete(DM_HCI_CHANGE_CONN_LINK_KEY_CFM,
            hci_prim->hci_change_conn_linkkey_coml_event.status, &bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle read remote support features complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_read_rem_supp_features_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T *hci_prim =
                    (HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T *)hci_uprim;
    DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T *dm_prim
        = zpnew(DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM_T);

    /* The ACL manager keeps track of remote supported features */
    dm_acl_hci_remote_supported_features(hci_prim);

    /* Send remote features to application only if it requested them */
    if ((remove_command_from_completing_list(
                    HCI_READ_REMOTE_SUPP_FEATURES,
                    &dm_prim->bd_addr,
                    hci_prim->handle) & DM_HCI_SENDER_APPLICATION) != 0)
    {
        dm_prim->type = DM_HCI_READ_REMOTE_SUPP_FEATURES_CFM;
        dm_prim->status = hci_prim->status;
        CsrMemCpy(dm_prim->features, hci_prim->lmp_supp_features,
                                                sizeof(dm_prim->features));
        DM_SendMessageAMphandle(dm_prim);
    }
    else
        CsrPmemFree(dm_prim);
}
#endif

/*! \brief Pass read remote version information complete event upwards.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_read_remote_ver_info_complete(HCI_UPRIM_T *hci_uprim)
{
    DM_ACL_T *p_acl;
    HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T *hci_prim =
                 (HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T *)hci_uprim;
    DM_HCI_READ_REMOTE_VER_INFO_CFM_T *dm_prim
        = zpnew(DM_HCI_READ_REMOTE_VER_INFO_CFM_T);

    remove_command_from_completing_list(HCI_READ_REMOTE_VER_INFO,
            &TBDADDR_ADDR(dm_prim->addrt), hci_prim->handle);

    /* fill in type */
    p_acl = dm_acl_find_by_handle(hci_prim->handle);
    if(NULL!=p_acl)
        dm_prim->addrt=p_acl->addrt;


    dm_prim->type = DM_HCI_READ_REMOTE_VER_INFO_CFM;
    dm_prim->status = hci_prim->status;
    dm_prim->LMP_subversion = hci_prim->lmp_subversion;
    dm_prim->LMP_version = hci_prim->lmp_version;
    dm_prim->manufacturer_name = hci_prim->manuf_name;

    DM_SendMessageAMphandle(dm_prim);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Pass QOS setup complete event upwards.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_qos_setup_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_QOS_SETUP_COMPLETE_T *hci_prim =
                        (HCI_EV_QOS_SETUP_COMPLETE_T *)hci_uprim;
    hci_connection_handle_t handle = hci_prim->handle;
    DM_HCI_QOS_SETUP_CFM_T *dm_prim
        = pnew(DM_HCI_QOS_SETUP_CFM_T);

    /* First check if the event is a result of a local request. */
    if ((remove_command_from_completing_list(
                    HCI_QOS_SETUP,
                    &dm_prim->bd_addr,
                    handle) & DM_HCI_SENDER_APPLICATION) == 0)
    {
        DM_ACL_T *p_acl;

        /* If not then try to look up the handle. */
        if ((p_acl = dm_acl_find_by_handle(handle)) == NULL)
        {
            /* Not a local request and ACL cannot be found, so swallow event. */
            CsrPmemFree(dm_prim);
            return;
        }

        /* Found ACL, so copy BD_ADDR from record structure. */
        (void)tbdaddr_copy_to_bd_addr(&dm_prim->bd_addr, &p_acl->addrt);
    }

    /* BD_ADDR has been found and copied to dm_prim->bd_addr */

    dm_prim->type = DM_HCI_QOS_SETUP_CFM;
    dm_prim->status = hci_prim->status;
    dm_prim->flags = hci_prim->flags;
    dm_prim->service_type = hci_prim->service_type;
    dm_prim->token_rate = hci_prim->token_rate;
    dm_prim->peak_bandwidth = hci_prim->peak_bandwidth;
    dm_prim->latency = hci_prim->latency;
    dm_prim->delay_variation = hci_prim->delay_variation;

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle role change event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_role_change_event(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_ROLE_CHANGE_T *p_prim = (HCI_EV_ROLE_CHANGE_T *)hci_prim;
    dm_acl_hci_role_change(&p_prim->bd_addr, p_prim->status, p_prim->new_role);
}
#endif

void dm_hci_handle_completed_packets(CsrUint16 handle,
                                     CsrUint16 completed_packets)
{
    CsrUint8 priority;
    DM_ACL_T *p_acl = NULL, **pp_acl;
    DM_HCI_FLOW_CONTROL_T *fc = NULL;

    /* If this is for an ACL that's not there anymore then do nothing.
       L2CAP will have already returned any pending credits to the DM. */
    if (handle != HCI_HANDLE_INVALID && handle != L2CAP_BCAST_HCI_HANDLE
            && (p_acl = dm_acl_find_by_handle(handle)) == NULL)
        return;

    if (p_acl != NULL)
    {
        /* We first locate whether the handle/NCP is for the BR/EDR controller
             or, for LE (in case of seperate buffers DM_HCI_FLOW_CONTROL_T
             will have different instances for BR/EDR and LE). Subtract the NCP
             directly from the used blocks finally.
        */
        fc = dm_amp_get_fc_type(p_acl);
        fc->used_data_blocks -= completed_packets;

        /* L2CAP may have deferred control of some pending credits to the DM.
           It should only do this if the ACL is dying. The ACL manager keeps
           track of the pending credits in the ACL structure and we should
           make sure that these have all been accounted for before we bother
           L2CAP. */
        if (completed_packets > p_acl->pending_credits)
        {
            completed_packets -= p_acl->pending_credits;
            p_acl->pending_credits = 0;

            CH_CompletedPackets(
                    DM_ACL_CLIENT_GET_DATA(p_acl, dm_acl_client_l2cap),
                    completed_packets);
        }
        else
            p_acl->pending_credits -= completed_packets;
    }
    else if (handle == L2CAP_BCAST_HCI_HANDLE)
    {
        /* TODO Debug here */
        /* Connectionless channel will always be on BR/EDR link */
        LYMCB.fc.used_data_blocks -= completed_packets;
        CH_CompletedPacketsCL(completed_packets);
    }

    /* Debugging to ensure that HCI data credits are consistent. */
    dm_hci_data_credit_audit();

    /* Hand out credits until we've dealt with all ACLs or run out of credits */
    for (priority = 0; priority != L2CAP_MAX_TX_QUEUES; ++priority)
    {
        /* While traversing the list we should take in account the used and
             the total number of credits pertaining to the LE buffers if the
             controller supports twin buffer system.
        */
        for (   pp_acl = &DM_ACL_FIRST();
                (p_acl = *pp_acl) != NULL &&
                (LYMCB.fc.used_data_blocks != LYMCB.fc.total_num_data_blocks
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
                || LYMCB.le_fc.used_data_blocks != LYMCB.le_fc.total_num_data_blocks
#endif
                );
                pp_acl = &p_acl->p_next)
        {
            if (p_acl->handle != HCI_HANDLE_INVALID)
            {
                L2CAP_CHCB_T *target_chcb;

                target_chcb = DM_ACL_CLIENT_GET_DATA(p_acl,dm_acl_client_l2cap);
                CH_DataSendQueued(target_chcb, &target_chcb->queue, priority, TRUE);
            }
        }
    }

    if (p_acl == NULL)
    {
        /* Might still be credits left, so try to send any connectionless. */
        CH_CLDataSendQueued();
    }
    else if (p_acl != DM_ACL_FIRST())
    {
        /* We must have run out of credits half way through the list.
           Chop the list in half and reattach the beginning at the end.
           This will provide a round-robin approach to credit distribution. */
        DM_ACL_T *p_end;

        /* Find last element of list. We know that p_acl != NULL. */
        for (p_end = p_acl; p_end->p_next != NULL; p_end = p_end->p_next)
            ;

        /* Join list into circle. */
        p_end->p_next = DM_ACL_FIRST();

        /* Choose new starting point and break circle. */
        DM_ACL_FIRST() = p_acl;
        *pp_acl = NULL;
    }
}

#ifdef L2CAP_HCI_DATA_CREDIT_SLOW_CHECKS
/*! \brief Ensure DM and L2CAP agree on how many credits are outstanding. */
void dm_hci_data_credit_audit(void)
{
    DM_ACL_T *p_acl;
    CsrUint16 credits = CH_CLUsedDataCredits();

    for (p_acl = DM_ACL_FIRST(); p_acl != NULL; p_acl = p_acl->p_next)
        credits += p_acl->pending_credits + CH_UsedDataCredits(
                DM_ACL_CLIENT_GET_DATA(p_acl, dm_acl_client_l2cap));

    /* Total credits taken by all the acl's will be equal to the used data blocks
        of the corresponding BR/EDR and LE data structures in the layer
        manager. In case the controller has common buffer for both used
        data blocks from LYMCB.fc will only be used.
    */
    if (credits != (LYMCB.fc.used_data_blocks
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
        + LYMCB.le_fc.used_data_blocks
#endif
     ))
    {
#ifdef L2CAP_HCI_DATA_CREDIT_CHECKS_PANIC
        BLUESTACK_PANIC(CSR_BT_PANIC_L2CAP_HCI_DATA_CREDITS_INCONSISTENT);
#else
        BLUESTACK_WARNING(CONTROL_ERR);
#endif
    }
}
#endif

#ifdef BUILD_FOR_HOST
/*! \brief Handle HCI_EV_NUMBER_COMPLETED_PKTS.

    This function is called to handle a HCI_EV_NUMBER_COMPLETED_PKTS primitive.
    For each HCI handle in the primitive, a call to
    MCB_HandleCompletedPackets() is made.
    Note: This is normally only called when running off-chip, when running
    on-chip MCB_HandleCompletedPackets() is called directly from the baseband.

    \param ind Pointer to HCI_EV_NUMBER_COMPLETED_PKTS primitive
    cast to (HCI_UPRIM_T*).
*/
static void dm_ev_number_completed_packets(HCI_UPRIM_T *hci_prim)
{
    CsrUint8 num_handles = hci_prim->hci_num_compl_pkts_event.num_handles;
    HANDLE_COMPLETE_T **pp_hc =
            hci_prim->hci_num_compl_pkts_event.num_completed_pkts_ptr;

    for (;; ++pp_hc)
    {
        HANDLE_COMPLETE_T *p_hc = *pp_hc;
        HANDLE_COMPLETE_T *p_hc_end = p_hc + HCI_EV_HANDLE_COMPLETES_PER_PTR;

        /* For each pointer, read the maximum number of handle-completes. */
        for (; p_hc != p_hc_end; ++p_hc, --num_handles)
        {
            if (num_handles == 0)
            {
                pdestroy_array((void **)
                    hci_prim->hci_num_compl_pkts_event.num_completed_pkts_ptr,
                    HCI_HOST_NUM_COMPLETED_PACKET_PTRS);
                return;
            }

            /* Process NCP event */
            dm_hci_handle_completed_packets(p_hc->handle, p_hc->num_completed);
        }
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle mode change event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_mode_change_event(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_MODE_CHANGE_T *p_prim = (HCI_EV_MODE_CHANGE_T*)hci_prim;
    BD_ADDR_T bd_addr;
    DM_ACL_T *p_acl;

    remove_command_from_completing_list(HCI_NOP, &bd_addr, p_prim->handle);

    if ((p_acl = dm_acl_find_by_handle(p_prim->handle)) != NULL)
        dm_acl_hci_mode_change(p_acl, p_prim->status, p_prim->curr_mode,
                p_prim->interval);
    else
        Send_DM_Mode_Change_Event(p_prim->status, &bd_addr,
                p_prim->curr_mode, p_prim->interval);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle PIN code request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_pin_code_request(HCI_UPRIM_T *hci_prim)
{
    dm_sm_hci_pin_code_request(&hci_prim->hci_pin_code_req_event.bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle link key request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_link_key_request(HCI_UPRIM_T *hci_prim)
{
    dm_sm_hci_link_key_request(&hci_prim->hci_link_key_req_event.bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle link key notification event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_link_key_notification(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_LINK_KEY_NOTIFICATION_T *p_prim =
                                (HCI_EV_LINK_KEY_NOTIFICATION_T*)hci_prim;

    dm_sm_hci_link_key_notification(&p_prim->bd_addr, p_prim->key_type,
                                                            p_prim->link_key);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle read clock offset complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_read_clock_offset_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T *hci_prim =
                    (HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T *)hci_uprim;
    DM_HCI_READ_CLOCK_OFFSET_CFM_T *dm_prim =
        pnew(DM_HCI_READ_CLOCK_OFFSET_CFM_T);

    /* Check if the command is still to be handled by the stack, it returns
      * a 0 when the command it already handled. No need to send a prim
      * then. Free the allocated prim.
      */
    if(remove_command_from_completing_list(HCI_READ_CLOCK_OFFSET,
            &dm_prim->bd_addr, hci_prim->handle))
    {
        dm_prim->type = DM_HCI_READ_CLOCK_OFFSET_CFM;
        dm_prim->status = hci_prim->status;
        dm_prim->clock_offset = hci_prim->clock_offset;

        /* cache the value if the read was successful. */
        if (hci_prim->status == HCI_SUCCESS)
        {
          dm_acl_update_cache(UPDATE_CLOCK, &dm_prim->bd_addr,
                            dm_prim->clock_offset,
                            HCI_PAGE_SCAN_REP_MODE_R2); /* Not used */
        }

        DM_SendMessageAMphandle(dm_prim);
    }
    else
    {
        CsrPmemFree(dm_prim);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle connection packet type changed event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_conn_packet_type_changed(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T *p_prim =
                                (HCI_EV_CONN_PACKET_TYPE_CHANGED_T*)hci_prim;
    DM_ACL_T *p_acl;

    /* We ideally should not receive this event without sending out a request,
     * so we send a cfm */
    if(NULL != (p_acl = dm_acl_find_by_handle(p_prim->handle)))
    {
        /* Found a matching acl, send corresponding cfm */
        BD_ADDR_T bd_addr;
        (void)tbdaddr_copy_to_bd_addr(&bd_addr, &p_acl->addrt);

        DM_SendBDADDRCommandComplete(DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM,
                p_prim->status, &bd_addr);
    }
#ifdef INSTALL_DM_SYNC_MODULE
    else
    {
        /* Inform dm_sync_manager that packet type has changed */
        dm_sync_hci_connection_packet_type_changed(p_prim);
    }
#endif
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle Sniff Subrate event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_sniff_sub_rate(HCI_UPRIM_T *hci_uprim)
{
    DM_ACL_T *acl;

    acl = dm_acl_find_by_handle(hci_uprim->hci_sniff_sub_rate_event.handle);
    if (acl != NULL)
    {
        DM_HCI_SNIFF_SUB_RATING_IND_T *dm_prim =
                pnew(DM_HCI_SNIFF_SUB_RATING_IND_T);

        dm_prim->type = DM_HCI_SNIFF_SUB_RATING_IND;
        dm_prim->status = hci_uprim->hci_sniff_sub_rate_event.status;
        (void)tbdaddr_copy_to_bd_addr(&dm_prim->bd_addr, &acl->addrt);
        dm_prim->transmit_latency =
                hci_uprim->hci_sniff_sub_rate_event.transmit_latency;
        dm_prim->receive_latency =
                hci_uprim->hci_sniff_sub_rate_event.receive_latency;
        dm_prim->remote_timeout =
                hci_uprim->hci_sniff_sub_rate_event.remote_timeout;
        dm_prim->local_timeout =
                hci_uprim->hci_sniff_sub_rate_event.local_timeout;
        DM_SendMessageAMphandle(dm_prim);
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle enhanced flush complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_enhanced_flush_complete(HCI_UPRIM_T *hci_prim)
{
    BD_ADDR_T bd_addr;
    HCI_EV_ENHANCED_FLUSH_COMPLETE_T *prim =
                                (HCI_EV_ENHANCED_FLUSH_COMPLETE_T*)hci_prim;

    remove_command_from_completing_list(HCI_ENHANCED_FLUSH, &bd_addr,
           prim->handle);

    DM_SendBDADDRCommandComplete(DM_HCI_ENHANCED_FLUSH_CFM,
            HCI_SUCCESS, &bd_addr);
}

#endif

/*! \brief Handle refresh encryption key complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_refresh_encryption_key_complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T *prim =
        (HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T*)hci_prim;

    remove_command_from_completing_list(HCI_REFRESH_ENCRYPTION_KEY,
                                        NULL,
                                        prim->handle);

    dm_sm_hci_refresh_encryption_key_complete(prim->handle, prim->status);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Common parts of inquiry result event handling.

    \param result Inquiry results.
    \param num_responses Number of results.
    \returns FALSE if we should discard the result, TRUE otherwise.
*/
static CsrBool dm_ev_inquiry_result_common(HCI_INQ_RESULT_WITH_RSSI_T **result,
                                                        CsrUint8 num_responses)
{
    CsrUint8 i;
    CsrUint8 res_index, inq_index;

#ifndef BUILD_FOR_HOST
    CsrSchedQid q;

    if ((q = dm_hci_get_upstream_queue_unless_full()) == NO_QID
            || scan_queue(q, check_inquiry_result_message, &result[0]->bd_addr))
        return FALSE;
#endif

    /* Update the cached connection parameter table */
    for (i = 0; i < num_responses; i++)
    {
        res_index = i / HCI_MAX_INQ_RESULT_PER_PTR;
        inq_index = i - (res_index * HCI_MAX_INQ_RESULT_PER_PTR);

        dm_acl_update_cache(UPDATE_CLOCK + UPDATE_PAGE,
            &(result[res_index][inq_index].bd_addr),
            result[res_index][inq_index].clock_offset,
            result[res_index][inq_index].page_scan_rep_mode);
    }

    return TRUE;
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle inquiry result with RSSI event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_inquiry_result_with_rssi( HCI_UPRIM_T *hci_prim)
{
    HCI_EV_INQUIRY_RESULT_WITH_RSSI_T *p_prim =
                        (HCI_EV_INQUIRY_RESULT_WITH_RSSI_T *) hci_prim;

    if (dm_ev_inquiry_result_common(p_prim->result, p_prim->num_responses))
        Send_DM_Inquiry_Result_With_RSSI(hci_prim);
    else
        pdestroy_array((void **)p_prim->result, HCI_MAX_INQ_RESULT_PTRS);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle extended inquiry result event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_extended_inquiry_result( HCI_UPRIM_T *hci_prim)
{
    HCI_EV_EXTENDED_INQUIRY_RESULT_T *prim =
                        (HCI_EV_EXTENDED_INQUIRY_RESULT_T*)hci_prim;
    HCI_INQ_RESULT_WITH_RSSI_T *result = &prim->result;

    if(dm_ev_inquiry_result_common(&result, 1))
        Send_DM_Extended_Inquiry_Result(hci_prim);
    else
        pdestroy_array((void **)prim->eir_data_part, HCI_EIR_DATA_PACKET_PTRS);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle read remote extended features complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_read_rem_ext_features_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_READ_REM_EXT_FEATURES_COMPLETE_T *hci_prim =
                        (HCI_EV_READ_REM_EXT_FEATURES_COMPLETE_T*)hci_uprim;
    DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T *dm_prim
        = pnew(DM_HCI_READ_REMOTE_EXT_FEATURES_CFM_T);

    CsrUint8 sender;

    sender = remove_command_from_completing_list(HCI_READ_REMOTE_EXT_FEATURES,
            &dm_prim->bd_addr, hci_prim->handle);

    /* ACL manager keeps track of SSP/SM4 host support (page 1 bit 1). */
    if ((sender & DM_HCI_SENDER_ACL_MANAGER) != 0)
        dm_sm_hci_remote_host_supported_features(hci_prim->handle,
                                                 hci_prim->status,
                                                 hci_prim->lmp_ext_features[0]);

    if ((sender & DM_HCI_SENDER_APPLICATION) != 0)
    {
        /* Forward to application. */
        dm_prim->type = DM_HCI_READ_REMOTE_EXT_FEATURES_CFM;
        dm_prim->status = hci_prim->status;
        dm_prim->page_num = hci_prim->page_num ;
        dm_prim->max_page_num = hci_prim->max_page_num ;
        CsrMemCpy(dm_prim->lmp_ext_features, hci_prim->lmp_ext_features,
                                         sizeof(dm_prim->lmp_ext_features));

        DM_SendMessageAMphandle(dm_prim);
    }
    else
        CsrPmemFree(dm_prim);

}
#endif


#ifndef DISABLE_DM_BREDR
/*! \brief Handle flow spec complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_flow_spec_complete(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_FLOW_SPEC_COMPLETE_T *hci_prim =
                                    (HCI_EV_FLOW_SPEC_COMPLETE_T*)hci_uprim;
    DM_HCI_FLOW_SPEC_CFM_T *dm_prim
        = pnew(DM_HCI_FLOW_SPEC_CFM_T);

    remove_command_from_completing_list(HCI_FLOW_SPEC, &dm_prim->bd_addr,
            hci_prim->handle);

    dm_prim->type = DM_HCI_FLOW_SPEC_CFM;
    dm_prim->status = hci_prim->status;

#ifdef BUILD_FOR_HOST
    dm_prim->flags              = hci_prim->flags;
    dm_prim->flow_direction     = hci_prim->flow_direction;
    dm_prim->service_type       = hci_prim->service_type;
    dm_prim->token_rate         = hci_prim->token_rate;
    dm_prim->token_bucket_size  = hci_prim->token_bucket_size;
    dm_prim->peak_bandwidth     = hci_prim->peak_bandwidth;
    dm_prim->access_latency     = hci_prim->access_latency;
#else
    /* NOTE This is not alignment safe and so is not portable. */
    CsrMemCpy(&dm_prim->flags, &hci_prim->flags,
                            sizeof(DM_HCI_FLOW_SPEC_CFM_T)
                            - offsetof(DM_HCI_FLOW_SPEC_CFM_T, flags));
#endif

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifdef INSTALL_DM_SYNC_MODULE
/*! \brief Handle synchronous connection complete event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_synch_connection_complete(HCI_UPRIM_T *hci_prim)
{
    dm_sync_hci_connect_complete((HCI_EV_SYNC_CONN_COMPLETE_T*)hci_prim);
}

/*! \brief Handle synchronous connection changed event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_synch_connection_changed(HCI_UPRIM_T *hci_prim)
{
    dm_sync_hci_connection_changed((HCI_EV_SYNC_CONN_CHANGED_T*)hci_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle IO capability request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_io_capability_request(HCI_UPRIM_T *hci_uprim)
{
    dm_sm_hci_io_capability_request(
                        &hci_uprim->hci_io_capability_request_event.bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle IO capability response event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_io_capability_response(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_IO_CAPABILITY_RESPONSE_T *prim =
                              (HCI_EV_IO_CAPABILITY_RESPONSE_T *) hci_uprim;

    dm_sm_hci_io_capability_response(&prim->bd_addr, prim->io_capability,
                prim->oob_data_present, prim->authentication_requirements);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle user confirmation request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_user_confirmation_request(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_USER_CONFIRMATION_REQUEST_T *prim =
                                (HCI_EV_USER_CONFIRMATION_REQUEST_T*)hci_uprim;

    dm_sm_hci_user_confirmation_request(&prim->bd_addr, &prim->numeric_value);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle user passkey request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_user_passkey_request(HCI_UPRIM_T *hci_uprim)
{
    dm_sm_hci_user_passkey_request(
                        &hci_uprim->hci_user_passkey_request_event.bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle remote Out Of Band (OOB) data request event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_remote_oob_data_request (HCI_UPRIM_T *hci_uprim)
{
    dm_sm_hci_remote_oob_data_request(
                        &hci_uprim->hci_remote_oob_data_request_event.bd_addr);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle user passkey notification event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_user_passkey_notification (HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_USER_PASSKEY_NOTIFICATION_T *prim =
                        (HCI_EV_USER_PASSKEY_NOTIFICATION_T*) hci_uprim;

    dm_sm_hci_user_passkey_notification(&prim->bd_addr, &prim->passkey);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle keypress notification event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_keypress_notification (HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_KEYPRESS_NOTIFICATION_T *prim =
                        (HCI_EV_KEYPRESS_NOTIFICATION_T*) hci_uprim;

    dm_sm_hci_keypress_notification(&prim->bd_addr, prim->notification_type);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle link supervision timeout change event.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_lst_change(HCI_UPRIM_T *hci_uprim)
{
    HCI_EV_LST_CHANGE_T *hci_prim = (HCI_EV_LST_CHANGE_T *)hci_uprim;
    DM_HCI_LINK_SUPERV_TIMEOUT_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_LINK_SUPERV_TIMEOUT_IND_T);
    dm_prim->type = DM_HCI_LINK_SUPERV_TIMEOUT_IND;
    dm_acl_get_bd_addr_by_handle(&dm_prim->bd_addr, hci_prim->handle);
    dm_prim->timeout = hci_prim->timeout;

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Handle remote host supported features event.

    You sometimes get one of these
    when you do a REMOTE_NAME_REQUEST in security mode 4. It's part of
    the solution to the legacy timeout problems.

    If there's an ACL up then we record the features before sending
    them up to the host.

    \param hci_prim Pointer to HCI event primitive.
*/
static void dm_ev_rem_host_supported_features(HCI_UPRIM_T *hci_uprim)
{
    DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T *dm_prim;
    DM_ACL_T *p_acl;

    /* Set ACL flags for SSP host support */
    if ((p_acl = dm_acl_find_by_bd_addr(
            &hci_uprim->hci_rem_host_supported_features_event.bd_addr)) != NULL)
    {
        dm_sm_hci_remote_host_supported_features(
            p_acl->handle,
            HCI_SUCCESS,
            hci_uprim->hci_rem_host_supported_features_event.host_features[0]);
    }

    /* Send to application. */
    dm_prim = pnew(DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND_T);
    dm_prim->type = DM_HCI_REM_HOST_SUPPORTED_FEATURES_IND;
    bd_addr_copy(&dm_prim->bd_addr,
                 &hci_uprim->hci_rem_host_supported_features_event.bd_addr);

    CsrMemCpy(dm_prim->host_features,
           hci_uprim->hci_rem_host_supported_features_event.host_features,
           sizeof(dm_prim->host_features));

    DM_SendMessageAMphandle(dm_prim);
}
#endif

/*! \brief Free downstream HCI primitive.

    \param prim HCI primitive to be freed.
*/
static void dm_hci_free_downstream_primitive(HCI_UPRIM_T *prim)
{
    switch (prim->op_code)
    {
#ifndef DISABLE_DM_BREDR
        case HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA:
            pdestroy_array((void**)prim->hci_write_extended_inquiry_response_data.eir_data_part,
                           HCI_EIR_DATA_PACKET_PTRS);
            break;

        case HCI_WRITE_CURRENT_IAC_LAP:
            pdestroy_array((void**)prim->hci_write_curr_iac_lap.iac_lap,
                           HCI_IAC_LAP_PTRS);
            break;

        case HCI_CHANGE_LOCAL_NAME:
            pdestroy_array((void**)prim->hci_change_local_name.name_part,
                           HCI_LOCAL_NAME_BYTE_PACKET_PTRS);
            break;

        case HCI_WRITE_STORED_LINK_KEY:
            pdestroy_array((void**)prim->hci_write_stored_link_key.link_key_bd_addr,
                           HCI_STORED_LINK_KEY_MAX);
            break;
#endif
        case HCI_HOST_NUM_COMPLETED_PACKETS:
            pdestroy_array((void**)prim->hci_host_num_coml_pkts.num_completed_pkts_ptr,
                           HCI_HOST_NUM_COMPLETED_PACKET_PTRS);
            break;

        default:
            /* Just ignore - the message has no additional data to free */
            break;
    }

    CsrPmemFree(prim);
}

CsrBool dm_hci_handle_and_tbdaddr(dm_prim_t type)
{
    switch (type & ~DM_HCI_WITH_HANDLE)
    {
        case HCI_READ_REMOTE_VER_INFO:
        case HCI_READ_TX_POWER_LEVEL:
        case HCI_READ_RSSI:
        case HCI_REFRESH_ENCRYPTION_KEY:
#ifdef INSTALL_ULP
        case HCI_ULP_CONNECTION_UPDATE:
        case HCI_ULP_READ_CHANNEL_MAP:
        case HCI_ULP_READ_REMOTE_USED_FEATURES:
#endif
            return TRUE;
    }

    return FALSE;
}

#ifdef ENABLE_SHUTDOWN
static void dm_hci_reset_command_queue(DM_HCI_COMMAND_QUEUE i)
{
    HCI_QUEUE_T *p_queue;

    while((p_queue = command_queue[i]) != NULL)
    {
        command_queue[i] = p_queue->next_item;
        dm_hci_free_downstream_primitive(p_queue->primitive);
        CsrPmemFree(p_queue);
    }
}

/*! \brief Deinitialise HCI queues. */
void dm_hci_interface_deinit(void)
{
    CsrUint16 i;

    for(i = 0; i < sizeof(command_queue)/sizeof(command_queue[0]); ++i)
        dm_hci_reset_command_queue((DM_HCI_COMMAND_QUEUE)i);

    CsrBtDmHciInit(NULL);
}
#endif /* ENABLE_SHUTDOWN */

#ifdef BUILD_FOR_HOST
void CsrBtDmHciInit(void **gash)
{
    command_packets_available = 1;
}
#endif

/*============================================================================*
End Of File
*============================================================================*/

