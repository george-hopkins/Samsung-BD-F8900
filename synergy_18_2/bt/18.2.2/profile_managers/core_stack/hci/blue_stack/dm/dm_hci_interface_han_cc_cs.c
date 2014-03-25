/*!

                (c) CSR plc 2010

                All rights reserved

\file           dm_hci_interface_han_cc_cs.c

\brief          Handle command complete and command status events



*/

#include "csr_synergy.h"

#include "csr_util.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "error.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"
#include "dm_layer_manager.h"
#include "dm_mgt_interface.h"
#include "csr_mblk.h"
#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_security_manager.h"
#include "dm_hci_interface.h"
#include "dm_sync_manager.h"
#ifdef INSTALL_AMP_SUPPORT
#include "dm_amp_interface.h"
#endif
#include "sm.h"
#include "tbdaddr.h"

#ifndef BUILD_FOR_HOST
#include "nonhci_convert.h"
#endif

/*! \brief Handles command status (failure) events by op-code.

    \param prim Pointer to command status event primitive.
    \param ret_prim Pointer to primitive of command that triggered the event.
*/
void handle_hci_cs_fail(HCI_EV_COMMAND_STATUS_T *prim, HCI_UPRIM_T *ret_prim)
{
    CsrUint8 sender = ret_prim->hci_cmd.length;
    hci_return_t status = prim->status;
    hci_op_code_t op_code = prim->op_code;

    switch(op_code)
    {
#ifndef DISABLE_DM_BREDR
        case HCI_INQUIRY:
        case HCI_CHANGE_CONN_LINK_KEY:
        case HCI_MASTER_LINK_KEY:
        case HCI_READ_REMOTE_VER_INFO:
        case HCI_READ_CLOCK_OFFSET:
        case HCI_QOS_SETUP:
        case HCI_FLOW_SPEC:
        case HCI_ENHANCED_FLUSH:
#endif
        case HCI_READ_REMOTE_SUPP_FEATURES:
#ifdef INSTALL_ULP
        case HCI_ULP_READ_REMOTE_USED_FEATURES:

#endif
            dm_unhandled_command((DM_UPRIM_T*)ret_prim, status);
            break;

#ifdef INSTALL_ULP
        case HCI_ULP_CONNECTION_UPDATE:
            send_dm_ble_update_connection_parameters_cfm(
                    status,
                    &ret_prim->hci_ulp_connection_update.addrt);
            break;
#endif

#ifndef DISABLE_DM_BREDR
        case HCI_REMOTE_NAME_REQ:
            Send_DM_HCI_Remote_Name_Cfm(status,
                    &ret_prim->hci_remote_name_req.bd_addr, NULL);
            break;
#endif
        case HCI_CREATE_CONNECTION:
            dm_acl_hci_connect_failed(&ret_prim->hci_create_connection.bd_addr,
                                status);
            break;

        case HCI_DISCONNECT:
            if (dm_acl_find_by_handle(ret_prim->hci_disconnect.handle))
            {
                dm_acl_hci_disconnect_failure(
                            ret_prim->hci_disconnect.handle);
            }
#ifdef INSTALL_DM_SYNC_MODULE
            else if (dm_sync_find_by_handle(ret_prim->hci_disconnect.handle,
                    NULL))
            {
                dm_sync_hci_disconnect_failure(
                        ret_prim->hci_disconnect.handle, status,
                        ret_prim->hci_disconnect.reason);
            }
#endif
            break;

        case HCI_ACCEPT_CONNECTION_REQ:
            BLUESTACK_WARNING(COMMAND_STATUS_ERR);
            break;

        case HCI_REJECT_CONNECTION_REQ:
            /* TODO We don't seem to do anything here */
            break;

#ifndef DISABLE_DM_BREDR
        case HCI_CHANGE_CONN_PKT_TYPE:
#ifdef INSTALL_DM_SYNC_MODULE
            {
                HCI_EV_CONN_PACKET_TYPE_CHANGED_T  errprim;
                DM_ACL_T *p_acl;

                errprim.status = status;
                errprim.handle = ret_prim->hci_change_conn_pkt_type.handle;

                /* See if we have an acl for this handle */
                if(NULL != (p_acl = dm_acl_find_by_handle(errprim.handle)))
                {
                    BD_ADDR_T bd_addr;
                    (void)tbdaddr_copy_to_bd_addr(&bd_addr, &p_acl->addrt);

                    DM_SendBDADDRCommandComplete(DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM,
                            errprim.status, &bd_addr);
                }
                else
                {
                    dm_sync_hci_connection_packet_type_changed(&errprim);
                }
            }
#else
            if ((sender & DM_HCI_SENDER_APPLICATION) != 0)
            {
                BD_ADDR_T bd_addr;
                dm_acl_get_bd_addr_by_handle(
                        &bd_addr, ret_prim->hci_change_conn_pkt_type.handle);
                DM_SendBDADDRCommandComplete(
                        DM_HCI_CONN_PACKET_TYPE_CHANGED_IND, status, &bd_addr);
            }
#endif
            break;

        case HCI_AUTH_REQ:
            dm_sm_hci_auth_failure(ret_prim->hci_auth_req.handle, status);
            break;

        case HCI_SET_CONN_ENCRYPTION:
            dm_sm_hci_enc_complete(ret_prim->hci_set_conn_encryption.handle,
                    status, FALSE);
            break;
        case HCI_READ_REMOTE_EXT_FEATURES:
            dm_sm_hci_remote_host_supported_features(
                        ret_prim->hci_read_rem_ext_features.handle, status, 0);

            dm_unhandled_command((DM_UPRIM_T*)ret_prim, status);
            break;

        case HCI_SNIFF_MODE:
        case HCI_EXIT_SNIFF_MODE:
        case HCI_PARK_MODE:
        case HCI_EXIT_PARK_MODE:
            {
                DM_ACL_T *p_acl;

                if ((p_acl = dm_acl_find_connected_by_bd_addr(
                            &ret_prim->hci_cmd_with_handle.bd_addr)) != NULL)

                {
                    dm_acl_hci_mode_change(p_acl, status, p_acl->current_mode,
                            0);
                    break;
                }
            }
        /* Fallthrough */

        case HCI_HOLD_MODE:
            Send_DM_Mode_Change_Event(status,
                    &ret_prim->hci_cmd_with_handle.bd_addr, 0, 0);
            break;

        case HCI_SWITCH_ROLE:
            dm_acl_hci_role_change(&ret_prim->hci_switch_role.bd_addr,status,0);
            break;
#endif

#ifdef INSTALL_DM_SYNC_MODULE
        case HCI_SETUP_SYNCHRONOUS_CONN:
           /* A connect request has moved from not being doomed to being
              doomed. This means that all other associated connect requests
              are already doomed. See B-56315. So we can get the DM to set
              the 'rejected' flag, which will allow future requests. */
            dm_sync_hci_connect_failed(
                    ret_prim->hci_setup_sync_connection.handle, status, TRUE);
            break;

        case HCI_ACCEPT_SYNCHRONOUS_CONN_REQ:
            dm_sync_hci_connect_response_failed(
                    &ret_prim->hci_accept_sync_connection_req.bd_addr,
                    status,
                    TRUE);
            break;

        case HCI_REJECT_SYNCHRONOUS_CONN_REQ:
            dm_sync_hci_connect_response_failed(
                    &ret_prim->hci_reject_sync_connection_req.bd_addr,
                    status,
                    FALSE);
            break;
#endif

        case HCI_REFRESH_ENCRYPTION_KEY:
            dm_unhandled_command((DM_UPRIM_T*)ret_prim, status);
            dm_sm_hci_enc_complete(
                    ret_prim->hci_refresh_encryption_key.handle, status, TRUE);
            break;

#ifdef INSTALL_ULP
        case HCI_ULP_CREATE_CONNECTION:
            dm_acl_hci_connect_failed(
                    &ret_prim->hci_ulp_create_connection.peer_address,
                    status);
            break;
#endif

        default:
            if (status == HCI_ERROR_ILLEGAL_COMMAND
                    || status == HCI_ERROR_UNSUPPORTED_FEATURE)
            {
                /* Illegal command that would (if supported) not normally
                 * produce a command status event. So try it as a command
                 * complete event. (Parameters not set are never read.)
                 */
                HCI_EV_COMMAND_COMPLETE_T hci_cc_prim;

                hci_cc_prim.op_code = op_code;
                hci_cc_prim.status = status;
                hci_cc_prim.argument_ptr = NULL;
                if ((sender & DM_HCI_SENDER_APPLICATION) != 0)
                    handle_hci_cc_application(&hci_cc_prim, ret_prim);
                else
                    handle_hci_cc_bluestack(&hci_cc_prim, ret_prim);
            }
            else
            {
                /* TODO Unknown command status event. Panic? */
                /* TODO Debug only */
                BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
            }
    }
}

/*! \brief Handles command complete events for application

    \param prim Pointer to command complete event primitive.
    \param ret_prim Pointer to primitive for command that triggered event.
*/
void handle_hci_cc_application(HCI_EV_COMMAND_COMPLETE_T *prim,
                                                        HCI_UPRIM_T *ret_prim)
{
    DM_UPRIM_T *dm_prim;
    HCI_COMMAND_COMPLETE_ARGS_T *arg_ptr = prim->argument_ptr;
    CsrUint8 status = prim->status;

    switch(prim->op_code)
    {
        case HCI_READ_LOCAL_NAME:
            dm_prim = (DM_UPRIM_T *) pnew(DM_HCI_READ_LOCAL_NAME_CFM_T);
            dm_prim->dm_read_local_name_cfm.type
                                            = DM_HCI_READ_LOCAL_NAME_CFM;
            CsrMemCpy(&dm_prim->dm_read_local_name_cfm.name_part,
                    &arg_ptr->read_local_name_args.name_part,
                    sizeof(dm_prim->dm_read_local_name_cfm.name_part));
            dm_prim->dm_read_local_name_cfm.status = status;
            DM_SendMessageAMphandle(dm_prim);
            break;

        case HCI_READ_CURRENT_IAC_LAP:
            dm_prim =
                (DM_UPRIM_T *)pnew(DM_HCI_READ_CURRENT_IAC_LAP_CFM_T);
            dm_prim->dm_read_current_iac_lap_cfm.type
                                    = DM_HCI_READ_CURRENT_IAC_LAP_CFM;
            CsrMemCpy(&dm_prim->dm_read_current_iac_lap_cfm.iac_lap,
                    &arg_ptr->read_current_iac_lap_args.iac_lap,
                    sizeof(dm_prim->dm_read_current_iac_lap_cfm.iac_lap));
            dm_prim->dm_read_current_iac_lap_cfm.num_current_iac
                = arg_ptr->read_current_iac_lap_args.num_current_iac;
            dm_prim->dm_read_current_iac_lap_cfm.status = status;
            DM_SendMessageAMphandle(dm_prim);
            break;

#ifndef DISABLE_DM_BREDR
        case HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA:
            dm_prim = (DM_UPRIM_T *)
                pnew(DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM_T);

            dm_prim->dm_read_extended_inquiry_response_data_cfm.type
                        = DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM;

            CsrMemCpy( &dm_prim->dm_read_extended_inquiry_response_data_cfm.eir_data_part,
                    &arg_ptr->read_extended_inquiry_response_data_args.eir_data_part,
                    sizeof(dm_prim->dm_read_extended_inquiry_response_data_cfm.eir_data_part));

            dm_prim->dm_read_extended_inquiry_response_data_cfm.fec_required
                = arg_ptr->read_extended_inquiry_response_data_args.fec_required;

            dm_prim->dm_read_extended_inquiry_response_data_cfm.status
                = status;

            DM_SendMessageAMphandle(dm_prim);
            break;

        case HCI_READ_LOCAL_OOB_DATA:
            dm_sm_hci_read_local_oob_data_cc(
                    arg_ptr->read_local_oob_data_args.c,
                    arg_ptr->read_local_oob_data_args.r, status);
            break;

        /* Commands that don't return command complete to the application. */
        case HCI_USER_CONFIRMATION_REQUEST_REPLY:
        case HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY:
        case HCI_USER_PASSKEY_REQUEST_REPLY:
        case HCI_USER_PASSKEY_REQUEST_NEG_REPLY:
        case HCI_SEND_KEYPRESS_NOTIFICATION:
            break;
#endif

        case HCI_RESET:
            if(status == HCI_SUCCESS)
            {
                /* drop any active connections */
                dm_acl_init();
            }

            DM_SendStandardCommandComplete(DM_HCI_RESET_CFM, status);
            break;

        default:
        /* Automatic HCI command complete handling for BCHS differs
           from Bluestack on chip. If we're on chip then we reuse
           code in hci_convert to read the HCI event primitive. If
           we're part of BCHS then we use an autogenerated switch
           statement. */
#ifdef BUILD_FOR_HOST
        handle_cc_autogen((HCI_UPRIM_T*)prim, ret_prim);
#else
        DM_SendMessageAMphandle(hcievt_cmd_cmplt_to_dm(prim, ret_prim));
#endif
    }
}

/*! \brief Handles Command complete events to be routed internally.

    \param prim Pointer to command complete event primitive.
    \param ret_prim Pointer to primitive for command that triggered event.
*/
void handle_hci_cc_bluestack(HCI_EV_COMMAND_COMPLETE_T *prim,
                                                    HCI_UPRIM_T *ret_prim)
{
    HCI_COMMAND_COMPLETE_ARGS_T *arg_ptr = prim->argument_ptr;
    CsrUint8 status = prim->status;
#ifndef DISABLE_DM_BREDR
    CsrUint8 sender = ret_prim->hci_cmd.length;
#endif

    switch(prim->op_code)
    {
        case HCI_HOST_BUFFER_SIZE:
        /* check to see if command was successful */
        if(status != HCI_SUCCESS)
        {
            BLUESTACK_WARNING(COMMAND_COMPLETE_ERR);
            /* TODO: Resend or panic ?
             * This is a bit of a show stopper
             * because now initialisation basically stops.
             */
        }
        break;

        case HCI_HOST_NUM_COMPLETED_PACKETS:
        BLUESTACK_WARNING(NUM_COMPLETED_PACKETS);
        break;

#ifndef DISABLE_DM_BREDR
        case HCI_WRITE_AUTH_ENABLE:
            if ((sender & DM_HCI_SENDER_SECURITY_MANAGER) != 0)
                dm_sm_hci_auth_enable(status);

            if ((sender & DM_HCI_SENDER_ACL_MANAGER) != 0)
                dm_acl_hci_auth_enable_cc(status,
                                ret_prim->hci_write_auth_enable.auth_enable);
        break;

        case HCI_WRITE_ENC_MODE:
            dm_sm_hci_write_enc_mode_complete(status);
            break;

        case HCI_WRITE_SIMPLE_PAIRING_MODE:
            dm_sm_hci_write_simple_pairing_mode_cc(status);
            break;

        case HCI_WRITE_SIMPLE_PAIRING_DEBUG_MODE:
            dm_sm_hci_write_simple_pairing_debug_mode_cc(status);
            break;
#endif
        case HCI_SET_EVENT_MASK:
            if(BT_VERSION_4p0!=LYM_READ_USED_BT_VERSION(LYMCB.BT_Version))
            {
                LYM_Set_BT_Version_CFM(status);
            }
            break;

        case HCI_READ_LOCAL_SUPP_FEATURES:
            LYM_Store_Local_Supported_Features(
                                    (HCI_READ_LOCAL_SUPP_FEATURES_RET_T*)arg_ptr);
            break;

        case HCI_RESET:
            break;

#ifdef INSTALL_ULP
        case HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES:
            if((HCI_SUCCESS==status) && (NULL!=arg_ptr))
            {
                LYM_ULP_Store_Local_Supported_Features(
                                        (HCI_ULP_READ_LOCAL_SUPPORTED_FEATURES_RET_T*)arg_ptr);
            }
            break;

        case HCI_ULP_SET_EVENT_MASK:
            LYM_ULP_Interrogate_Controller();
            break;

#if defined(INSTALL_SM_MODULE) && !defined(SM_HAS_FUNCTION_FOR_AES)
        case HCI_ULP_ENCRYPT:
            sm_aes_handle_encrypt_result(
                    status,
                    arg_ptr->ulp_encrypt_args.encrypted_data);
            break;
#endif /* INSTALL_SM_MODULE && !SM_HAS_FUNCTION_FOR_AES */
#endif /* INSTALL_ULP */


        case HCI_READ_BUFFER_SIZE:
        /* check to see if command was successful */
        if(status != HCI_SUCCESS)
        {
            BLUESTACK_WARNING(COMMAND_COMPLETE_ERR);
            /* TODO: Resend or panic ?
             * This is a bit of a show stopper
             * because now initialisation basically stops.
             */
        }
        else
        {
            /* Forward to layer manager */
            LYM_Buffer_Size_Ind(
                    (HCI_READ_BUFFER_SIZE_RET_T*)prim->argument_ptr);
        }

        break;

#ifndef DISABLE_DM_BREDR
        case HCI_WRITE_SCAN_ENABLE:
        case HCI_LINK_KEY_REQ_REPLY:
        case HCI_LINK_KEY_REQ_NEG_REPLY:
        case HCI_PIN_CODE_REQ_REPLY:
        case HCI_PIN_CODE_REQ_NEG_REPLY:
        case HCI_USER_CONFIRMATION_REQUEST_REPLY:
        case HCI_USER_CONFIRMATION_REQUEST_NEG_REPLY:
        case HCI_IO_CAPABILITY_RESPONSE:
        case HCI_IO_CAPABILITY_REQUEST_NEG_REPLY:
        case HCI_REMOTE_OOB_DATA_REQUEST_REPLY:
        case HCI_REMOTE_OOB_DATA_REQUEST_NEG_REPLY:
        case HCI_DELETE_STORED_LINK_KEY:
        case HCI_WRITE_LINK_POLICY_SETTINGS:
        case HCI_USER_PASSKEY_REQUEST_REPLY:
        case HCI_USER_PASSKEY_REQUEST_NEG_REPLY:
        case HCI_CREATE_CONNECTION_CANCEL:
            break;
#endif
#ifdef INSTALL_ULP
        case HCI_ULP_SET_ADVERTISING_PARAMETERS:
        case HCI_ULP_SET_ADVERTISE_ENABLE:
        case HCI_ULP_CREATE_CONNECTION_CANCEL:
        case HCI_ULP_SET_SCAN_ENABLE:
            break;

#ifdef INSTALL_SM_MODULE
#ifndef DISABLE_SM_SLAVE
        case HCI_ULP_LONG_TERM_KEY_REQUESTED_REPLY:
        case HCI_ULP_LONG_TERM_KEY_REQUESTED_NEGATIVE_REPLY:
            break;
#endif

#ifndef SM_HAS_FUNCTION_FOR_RAND
        case HCI_ULP_RAND:
            if (status == HCI_SUCCESS)
                sm_rand_complete(arg_ptr->ulp_rand_args.random_number);
            break;
#endif /* ndef SM_HAS_FUNCTION_FOR_RAND */
#endif /* INSTALL_SM_MODULE */

        case HCI_WRITE_LE_HOST_SUPPORT:
            /* TODO - check status */
            LYM_Set_BT_Version_CFM(status);
            break;

        case HCI_ULP_READ_BUFFER_SIZE:
            LYM_ULP_Buffer_Size_Ind(
                    arg_ptr->ulp_read_buffer_size_args.data_packet_length,
                    arg_ptr->ulp_read_buffer_size_args.num_data_packets);
            break;
#endif

#if defined(INSTALL_AMP_SUPPORT) || defined(INSTALL_SM_MODULE)
        case HCI_READ_BD_ADDR:
#ifdef INSTALL_AMP_SUPPORT
            dm_amp_read_bd_addr_cc(
                            sender,
                            status,
                            &arg_ptr->read_bd_addr_args.bd_addr);
#endif
#ifdef INSTALL_SM_MODULE
            sm_read_bd_addr_cc(status, &arg_ptr->read_bd_addr_args.bd_addr);
#endif
            break;
#endif /* defined(INSTALL_AMP_SUPPORT) || defined(INSTALL_SM_MODULE) */

        default:
            /* TODO DEBUG only */
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
    }
}

/*============================================================================*
End Of File
*============================================================================*/
