/*!

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

\file              dm_mgt_interface.c

\brief             This file contains the Device Manager functions.



*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "error.h" /* path removed */
#include "csr_bt_core_stack_fsm.h" /* path removed */
#include "csr_util.h" /* memcpy,memset */
#include "hci_prim.h" /* path modified */
#include "l2cap_prim.h"
#include "dm_prim.h" /* path modified */
#include "csr_bt_tasks.h" /* path removed */
#include "csr_sched.h"
#include "csr_bt_core_stack_pmalloc.h" /* path removed */
#ifndef BUILD_FOR_HOST
#include "io_defs.h"
#else
#include "csr_env_prim.h"
#include "csr_util.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_hcishim.h"
#endif
#include "dm_layer_manager.h" /* path removed */
#include "dm_hci_interface.h" /* path removed */
#include "dm_sync_manager.h"
#include "dm_acl_core.h"
#include "dm_acl_policy_manager.h"
#include "dm_acl_powerstates.h"
#include "dm_acl_manager.h" /* path removed */
#include "dm_acl_con_par_cache.h"
#include "dm_security_manager.h" /* path removed */
#include "csr_mblk.h" /* path removed */
#include "dm_mgt_interface.h" /* path removed */
#include "dm_ble_advertising.h"
#include "l2caplib.h"
#include "sm.h"
#include "tbdaddr.h"


/*! Public Data */
CsrSchedQid am_phandle = CSR_SCHED_QID_INVALID;

/*! Private Data Types */

/*! DM function pointers */
typedef void (*DM_PRIV_FUNCTION_T)(const DM_UPRIM_T *const prim);
typedef void (*DM_SM_FUNCTION_T)(DM_UPRIM_T *const prim);

/*! Private Function Prototypes */
static void dm_am_register_req(const DM_UPRIM_T *const p_uprim);
static void dm_set_bt_version_req(const DM_UPRIM_T *const p_uprim);
static void dm_open_acl_req(const DM_UPRIM_T *const prim);
static void dm_close_acl_req(const DM_UPRIM_T *const prim);
static void send_dm_acl_open_cfm(const TYPED_BD_ADDR_T *const addrt,
        const CsrBool success);

#ifdef INSTALL_ULP
static void dm_ble_set_scan_parameters_req(const DM_UPRIM_T *const p_uprim);
static void dm_ble_set_scan_enable_req(const DM_UPRIM_T *const p_uprim);
#else /* INSTALL_ULP */
#define dm_ble_set_scan_parameters_req NULL
#define dm_ble_set_scan_enable_req NULL
#endif /* INSTALL_ULP */


/*! Function pointer array for DM_PRIV primitives */
static const DM_PRIV_FUNCTION_T dm_priv_function_ptr[] =
{
    dm_am_register_req,                         /* DM_AM_REGISTER_REQ */
    dm_set_bt_version_req,                      /* DM_SET_BT_VERSION_REQ */
    dm_acl_write_cached_page_mode,              /* DM_WRITE_CACHED_PAGE_MODE_REQ */
    dm_acl_write_cached_clock_offset,           /* DM_WRITE_CACHED_CLOCK_OFFSET_REQ */
    dm_acl_clear_conn_cache,                    /* DM_CLEAR_PARAM_CACHE_REQ */
    dm_acl_lp_write_powerstates_req,            /* DM_LP_WRITE_POWERSTATES_REQ */
    dm_acl_set_default_link_policy,             /* DM_SET_DEFAULT_LINK_POLICY_REQ */
    dm_open_acl_req,                            /* DM_ACL_OPEN_REQ */
    dm_close_acl_req,                           /* DM_ACL_CLOSE_REQ */
    send_rnr_to_hci_with_conn_params,           /* DM_HCI_REMOTE_NAME_REQUEST_REQ */
    dm_acl_sniff_mode,                          /* DM_HCI_SNIFF_MODE_REQ */
    dm_acl_exit_sniff_mode,                     /* DM_HCI_EXIT_SNIFF_MODE_REQ */
    dm_acl_park_mode,                           /* DM_HCI_PARK_MODE_REQ */
    dm_acl_exit_park_mode,                      /* DM_HCI_EXIT_PARK_MODE_REQ */
    dm_acl_switch_role,                         /* DM_HCI_SWITCH_ROLE_REQ */
    dm_acl_write_lp_settings,                   /* DM_HCI_WRITE_LP_SETTINGS */
    dm_acl_lp_write_roleswitch_policy_req,      /* DM_LP_WRITE_ROLESWITCH_POLICY_REQ*/
    dm_acl_lp_write_always_master_devices_req,  /* DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ */
    dm_acl_write_scan_enable_req,               /* DM_HCI_WRITE_SCAN_ENABLE_REQ */
    dm_ble_advertising_set_enable_req,          /* DM_HCI_ULP_SET_ADVERTISE_ENABLE_REQ */
    dm_ble_advertising_set_parameters_req,      /* DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_REQ */
    dm_ble_set_connection_parameters_req,       /* DM_SET_BLE_CONNECTION_PARAMETERS_REQ*/
    dm_ble_update_connection_parameters_req,    /* DM_UPDATE_CONNECTION_PARAMETERS_REQ */
    dm_ble_set_scan_parameters_req,             /* DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ */
    dm_ble_set_scan_enable_req,                 /* DM_HCI_ULP_SET_SCAN_ENABLE_REQ */
    dm_handle_bdaddr_black_list_req,            /* DM_HANDLE_DEVICE_BLACK_LIST_REQ */
    dm_acl_change_conn_pkt_type_req,            /* DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ */
    dm_ble_accept_connection_par_update_rsp      /* DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP */
};

/*! Function pointer array for Security Manager primitives */
static const DM_SM_FUNCTION_T dm_sm_function_ptr[] =
{
    dm_sm_handle_init_req,                 /* DM_SM_INIT_REQ */
    dm_sm_handle_register_req,             /* DM_SM_REGISTER_REQ */
    dm_sm_handle_unregister_req,           /* DM_SM_UNREGISTER_REQ */
    dm_sm_handle_register_outgoing_req,    /* DM_SM_REGISTER_OUTGOING_REQ */
    dm_sm_handle_unregister_outgoing_req,  /* DM_SM_UNREGISTER_OUTGOING_REQ */
    dm_sm_handle_access_req,               /* DM_SM_ACCESS_REQ */
    dm_sm_handle_add_device_req,           /* DM_SM_ADD_DEVICE_REQ */
    dm_sm_handle_remove_device_req,        /* DM_SM_REMOVE_DEVICE_REQ */
    dm_sm_handle_key_request_rsp,          /* DM_SM_KEY_REQUEST_RSP */
    dm_sm_handle_pin_request_rsp,          /* DM_SM_PIN_REQUEST_RSP */
    dm_sm_handle_authorise_rsp,            /* DM_SM_AUTHORISE_RSP */
    dm_sm_handle_authenticate_req,         /* DM_SM_AUTHENTICATE_REQ */
    dm_sm_handle_encrypt_req,              /* DM_SM_ENCRYPT_REQ */
                                    /* DM_SM_IO_CAPABILITY_REQUEST_RSP */
    dm_sm_handle_io_capability_request_rsp,
    dm_sm_handle_bonding_req,               /* DM_SM_BONDING_REQ */
    dm_sm_handle_bonding_cancel_req,        /* DM_SM_BONDING_CANCEL_REQ */
    dm_sm_handle_read_device_req,           /* DM_SM_READ_DEVICE_REQ */
                                    /* DM_SM_IO_CAPABILITY_REQUEST_NEG_RSP */
    dm_sm_handle_io_capability_request_neg_rsp,
    dm_sm_convert_to_hci,       /* DM_SM_USER_CONFIRMATION_REQUEST_RSP */
    dm_sm_convert_to_hci,       /* DM_SM_USER_CONFIRMATION_REQUEST_NEG_RSP */
    dm_sm_convert_to_hci,       /* DM_SM_USER_PASSKEY_REQUEST_RSP */
    dm_sm_convert_to_hci,       /* DM_SM_USER_PASSKEY_REQUEST_NEG_RSP */
    sm_handle_security_req,                 /* DM_SM_SECURITY_REQ */
    sm_handle_configure_local_address_req,
    sm_handle_generate_nonresolvable_private_address_req,
    sm_handle_data_sign_req,
    dm_sm_handle_key_request_neg_rsp
};

/*! Private Functions */

/*! \brief Register application phandle.

    \param p_uprim Pointer to primitive.
*/
static void dm_am_register_req(const DM_UPRIM_T *const p_uprim)
{
    if((am_phandle = p_uprim->dm_am_register_req.phandle) == CSR_SCHED_QID_INVALID)
    {
        /* Not allowed to register an invalid handle */
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
    }

    LYMCB_SET(LYM_ApplicationReady);
    LYM_InterrogateController();

    if(LYMCB_TEST(LYM_Ready))
        send_dm_am_register_cfm();
}

/*! \brief Set Bluetooth version.

    \param p_uprim Pointer to primitive.
*/
static void dm_set_bt_version_req(const DM_UPRIM_T *const p_uprim)
{
    /* BT version is only 8 bits internally thus convert here.
    */
    LYM_Set_BT_Version((CsrUint8)(p_uprim->dm_set_bt_version_req.version & 0xFF));
}

/*! Public Functions */

/*! \brief Initialisation function for the whole device manager.

    \param gash Unused.
*/
void CsrBtDmInit(void **gash)
{
    CSR_UNUSED(gash);

    if (!HciClientEnabled())
        return;

    am_phandle = CSR_SCHED_QID_INVALID;

    /*! Initialise HCI interface */
    dm_hci_interface_deinit();

#ifdef BUILD_FOR_HOST
    /* Initialise HCI Top. For BCHS, the queue id is needed. */
    CsrBtHcishimInit();
#endif

    LYM_Initialise();

    dm_acl_init();
    dm_acl_policy_manager_init();
    dm_sm_init();
    dm_sync_init();
    dm_amp_init();
}

/*! \brief Handle downstream DM primitives.

    \param prim Pointer to primitive case to (DM_UPRIM_T*)
*/
static void *dm_iface_handler_dm(DM_UPRIM_T *prim)
{
    dm_prim_t type = prim->type;

    switch(type & DM_GROUP_MASK)
    {
        case DM_PRIV_PRIM:
            dm_priv_function_ptr[type & DM_TYPE_MASK](prim);
            break;

#ifdef INSTALL_DM_SYNC_MODULE
        case DM_SYNC_PRIM:
            dm_sync_handler(prim);
            break;
#endif

        case DM_SM_PRIM:
            dm_sm_function_ptr[type & DM_TYPE_MASK](prim);
            break;

#ifdef INSTALL_AMP_SUPPORT
        case DM_AMPM_PRIM:
            dm_amp_handler(prim);
            break;
#endif
        default:
            /* DM_HCI_..._REQ primitives */
            if (type >= DM_HCI_COMMANDS)
            {
                /*! Length field is abused by the DM to store sender. */
                prim->dm_hci_common.length = DM_HCI_SENDER_APPLICATION;

                if ((type & DM_HCI_WITH_HANDLE) == 0)
                {
                    /*! Vanilla HCI command */
                    send_to_hci(prim);
                }
                else
                    send_to_hci_with_handle(prim);

                /* The controller now has ownership of the memory. */
                return NULL;
            }

#ifndef BUILD_FOR_HOST
            if (type == DM_AM_REGISTER_WRONG_API_REQ &&
                    (prim->dm_am_register_wrong_api_req.phandle & 0x8000) != 0)
            {
                /* We're on-chip with an off-chip host that is trying
                   to send a DM_AM_REGISTER_REQ using the old API. We
                   send back the confirmation with an invalid phandle */
                /* Reuse memory from incoming message. */
                prim->dm_upstream_common.type =
                        DM_AM_REGISTER_WRONG_API_CFM;
                prim->dm_upstream_common.phandle = CSR_SCHED_QID_INVALID;
                CsrSchedMessagePut(DM_TOHOSTQUEUE,
                            IO_DATA_PROTOCOL_DEVICE_MGT,
                            prim);

                /* Hostio now has ownership of the memory. */
                return NULL;
            }
#endif

            /* Unsupported primitive. */
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
    }

    return (void*)prim;
}

/*! \brief Interface handler for all events at top of DM.

    \param gash Unused.
*/
void CsrBtDmHandler(void **gash)
{
    CsrUint16 m;
    void *message;

    CSR_UNUSED(gash);

    CsrSchedMessageGet(&m , &message);

    switch (m)
    {
#ifdef BUILD_FOR_HOST
        case CSR_SCHED_PRIM:
            break;
#endif
        case L2CAP_PRIM:
        {

#ifdef INSTALL_ULP
            switch(((L2CA_UPRIM_T*)message)->type)
            {
                case L2CA_CONNECTION_PAR_UPDATE_CFM:
                {
                    L2CA_CONNECTION_PAR_UPDATE_CFM_T * const p_prim = &((L2CA_UPRIM_T*)message)->l2ca_connection_par_update_cfm;
                    send_dm_ble_update_connection_parameters_cfm((CsrUint8)(p_prim->result == L2CA_MISC_SUCCESS ?
                                                                           HCI_SUCCESS : HCI_ERROR_UNSPECIFIED),
                                                                 &p_prim->addrt);

                }
                break;

                case L2CA_CONNECTION_PAR_UPDATE_IND:
                {
                    dm_handle_accept_connection_par_update_signal((L2CA_UPRIM_T*)message);
                }
                break;
#ifdef INSTALL_SM_MODULE
                default:
                {
                    sm_l2cap_handler((L2CA_UPRIM_T*)message);
                }
                break;
#endif /* INSTALL_SM_MODULE */
            }
#endif /* INSTALL_ULP */
            L2CA_FreePrimitive((L2CA_UPRIM_T*)message);
            message = NULL;
            break;
        }

        case DM_PRIM:
        case 0:
            message = dm_iface_handler_dm((DM_UPRIM_T*)message);
            break;

        default:
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
    }

    CsrPmemFree(message);

    /* Task is about to end, so see if there are any dead ACLs to clear away */
    dm_acl_cleanup();
}

/*! \brief Send a DM primitive upstream.

    \param message Pointer to primitive cast to (void*).
*/
void DM_SendMessage(void *message)
{
    CsrSchedQid q = ((DM_UPSTREAM_COMMON_T*)message)->phandle;      /* Queue */
    CsrUint16 mi = DM_PRIM;                          /* Message Identifier */

    if (CSR_SCHED_QID_INVALID == q)
    {
        /* We should never be reaching here, something is wrongly configured */
        /* Dont hide the symptom by freeing prim and returning, but panic */
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
        return;
    }

#ifndef BUILD_FOR_HOST
    if ((q & 0x8000) != 0)
    {
        /* If the top bit is set then we send off chip. */
        q = DM_TOHOSTQUEUE;
        mi = IO_DATA_PROTOCOL_DEVICE_MGT;
    }
#endif

    CsrSchedMessagePut(q, mi, message);
}

/*! \brief Fill in am_phandle before sending primitive upstream.

    \param message Pointer to primitive cast to (void*).
*/
void DM_SendMessageAMphandle(void *message)
{
    if (message == NULL)
        return;

    ((DM_UPSTREAM_COMMON_T*)message)->phandle = am_phandle;
    DM_SendMessage(message);
}

/*! \brief Send basic DM_HCI command complete primitive upstream.

    \param type DM primitive type.
    \param status HCI command status.
*/
void DM_SendStandardCommandComplete(dm_prim_t type, hci_return_t status)
{
    DM_SendBDADDRCommandComplete(type, status, NULL);
}

/*! \brief Send DM_HCI command complete with Bluetooth
           address primitive upstream. Uses HCI handle to find BD_ADDR.

    \param type DM primitive type.
    \param status HCI command status.
    \param handle HCI connection handle.
*/
void DM_SendBDADDRCommandComplete(dm_prim_t type,
        hci_return_t status, const BD_ADDR_T *bd_addr)
{
    DM_HCI_BD_ADDR_COMMAND_CFM_T *p_prim;

    p_prim = zpnew(DM_HCI_BD_ADDR_COMMAND_CFM_T);
    p_prim->type = type;
    p_prim->status = status;

    if (bd_addr != NULL)
        bd_addr_copy(&p_prim->bd_addr, bd_addr);

    DM_SendMessageAMphandle(p_prim);
}

/*! \brief Send DM_HCI command complete with Fat Bluetooth address
           primitive upstream.
    \param type DM primitive type.
    \param status HCI command status.
    \param addrt Pointer to Fat Bluetooth address.
*/
void DM_SendTBDADDRCommandComplete(dm_prim_t type,
                                   hci_return_t status,
                                   const TYPED_BD_ADDR_T *addrt)
{
    DM_HCI_TYPED_BD_ADDR_COMMAND_CFM_T *prim;

    prim = zpnew(DM_HCI_TYPED_BD_ADDR_COMMAND_CFM_T);
    prim->type = type;
    prim->status = status;
    if (addrt != NULL)
        tbdaddr_copy(&prim->addrt, addrt);

    DM_SendMessageAMphandle(prim);
}

/*! \brief Build and send a DM_AM_REGISTER_CFM primitive. */
void send_dm_am_register_cfm(void)
{
    DM_AM_REGISTER_CFM_T *dm_prim;

    dm_prim = pnew(DM_AM_REGISTER_CFM_T);
    dm_prim->type = DM_AM_REGISTER_CFM;
    dm_prim->version = (BLUESTACK_VERSION_MAJOR << 8) | BLUESTACK_VERSION_MINOR;
    dm_prim->patch_level = BLUESTACK_VERSION_PATCHLEVEL;

    DM_SendMessageAMphandle(dm_prim);
}

/*! \brief Build and send a DM_ACL_OPEN_CFM primitive.

    \param p_bd_addr Pointer to Bluetooth address.
    \param success Indication of success or failure.
*/

static void send_dm_acl_open_cfm(const TYPED_BD_ADDR_T *const addrt,
        const CsrBool success)
{
    DM_ACL_OPEN_CFM_T *p_prim = pnew(DM_ACL_OPEN_CFM_T);

    p_prim->type = DM_ACL_OPEN_CFM;
    tbdaddr_copy(&p_prim->addrt, addrt);
    p_prim->success = success;

    DM_SendMessageAMphandle(p_prim);
}

/*! \brief Build and send a DM_ACL_OPENED_IND primitive.

    \param p_acl Pointer to ACL record structure.
    \param status Indication of success or failure.
*/
void send_dm_acl_opened_ind(DM_ACL_T *p_acl, CsrUint8 status)
{
#if !(defined(BUILD_FOR_HOST) && defined(INSTALL_BUILD_FOR_HOST_DM_ACL_CON_HANDLE_IND))
    DM_ACL_OPENED_IND_T *p_prim = zpnew(DM_ACL_OPENED_IND_T);

    p_prim->type = DM_ACL_OPENED_IND;
#else
    DM_ACL_CONN_HANDLE_IND_T *p_prim = zpnew(DM_ACL_CONN_HANDLE_IND_T);

    p_prim->type = DM_ACL_CONN_HANDLE_IND;
    p_prim->acl_conn_handle = p_acl->handle;
#endif
    tbdaddr_copy(&p_prim->addrt, &p_acl->addrt);
    p_prim->dev_class = p_acl->dev_class;
    p_prim->status = status;

#ifdef INSTALL_ULP
    if (dm_acl_is_ble(p_acl))
    {
        p_prim->flags |= DM_ACL_FLAG_ULP;

        if (p_acl->ble_conn_params != NULL )
            p_prim->ble_conn_params = *p_acl->ble_conn_params;
    }
#endif

    if ((p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
        p_prim->flags |= DM_ACL_FLAG_INCOMING;

    DM_SendMessageAMphandle(p_prim);

    if (DM_ACL_CLIENT_LOCKS(p_acl, dm_acl_client_application) != 0)
        send_dm_acl_open_cfm(&p_acl->addrt, (CsrBool)(status == HCI_SUCCESS));
}

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_INQUIRY_RESULT_IND primitive.

    \param hci_prim Pointer to HCI_EV_INQUIRY_RESULT primitive, cast
           to (HCI_UPRIM_T*).
*/
void Send_DM_Inquiry_Result(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_INQUIRY_RESULT_T *prim = (HCI_EV_INQUIRY_RESULT_T *)hci_prim;
    DM_HCI_INQUIRY_RESULT_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_INQUIRY_RESULT_IND_T);
    dm_prim->type = DM_HCI_INQUIRY_RESULT_IND;
    dm_prim->num_responses = prim->num_responses;

    CsrMemCpy(dm_prim->result, prim->result, sizeof(dm_prim->result));

    DM_SendMessageAMphandle(dm_prim);
}

#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_MODE_CHANGE_EVENT_IND primitive.

    \param status Indication of success or failure.
    \param p_bd_addr Pointer to Bluetooth address.
    \param mode Current mode.
    \param length Interval.
*/
void Send_DM_Mode_Change_Event(const CsrUint8 status,
        const BD_ADDR_T *const p_bd_addr, const CsrUint8 mode,
        const CsrUint16 length)
{
    DM_HCI_MODE_CHANGE_EVENT_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_MODE_CHANGE_EVENT_IND_T);
    dm_prim->type = DM_HCI_MODE_CHANGE_EVENT_IND;
    dm_prim->status = status;
    bd_addr_copy(&dm_prim->bd_addr, p_bd_addr);
    dm_prim->mode = mode;
    dm_prim->length = length;

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_SWITCH_ROLE_CFM primitive.

    \param status Indication of success or failure.
    \param p_bd_addr Pointer to Bluetooth address.
    \param role Current role.
*/
void Send_DM_Switch_Role_Complete(const CsrUint8 status,
        const BD_ADDR_T *const p_bd_addr, const CsrUint8 role)
{
    DM_HCI_SWITCH_ROLE_CFM_T *dm_prim;

    dm_prim = pnew(DM_HCI_SWITCH_ROLE_CFM_T);
    dm_prim->type = DM_HCI_SWITCH_ROLE_CFM;
    bd_addr_copy(&dm_prim->bd_addr, p_bd_addr);
    dm_prim->status  = status;
    dm_prim->role    = role;

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_RETURN_LINK_KEYS_IND primitive.

    \param hci_prim Pointer to HCI event primitive.
*/
void Send_Return_Link_Keys_Complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_RETURN_LINK_KEYS_T *in_prim = (HCI_EV_RETURN_LINK_KEYS_T*)hci_prim;
    DM_HCI_RETURN_LINK_KEYS_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_RETURN_LINK_KEYS_IND_T);
    dm_prim->type = DM_HCI_RETURN_LINK_KEYS_IND;

    dm_prim->num_keys = HCI_STORED_LINK_KEY_MAX < in_prim->number_keys ?
        HCI_STORED_LINK_KEY_MAX : in_prim->number_keys;

    CsrMemCpy(dm_prim->link_key_bd_addr, in_prim->link_key_bd_addr,
            sizeof(dm_prim->link_key_bd_addr));

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_MASTER_LINK_KEY_CFM primitive.

    \param hci_prim Pointer to HCI event primitive.
*/
void Send_Master_Link_Key_Complete(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_MASTER_LINK_KEY_COMPLETE_T *in_prim;
    DM_HCI_MASTER_LINK_KEY_CFM_T *dm_prim;

    in_prim = (HCI_EV_MASTER_LINK_KEY_COMPLETE_T *)hci_prim;

    dm_prim = pnew(DM_HCI_MASTER_LINK_KEY_CFM_T);
    dm_prim->type = DM_HCI_MASTER_LINK_KEY_CFM;
    dm_prim->status = in_prim->status;
    dm_prim->key_flag = in_prim->key_flag;

    DM_SendMessageAMphandle(dm_prim);
}
#endif

/*! \brief
    Issues a negative confirmation for any DM commands that could not be
    handled. This is for link-related commands, in case the link has
    disappeared and the BD Address -> HCI handle mapping is not found.

    \param p_prim Pointer to DM command primitive.
    \param status Error that occured.
*/
void dm_unhandled_command(DM_UPRIM_T *req, hci_error_t status)
{
    dm_prim_t type = req->type;
    DM_UPRIM_T *cfm;

    /* If it didn't come from the application then no point bothering them. */
    if ((req->dm_hci_common.length & DM_HCI_SENDER_APPLICATION) == 0)
        return;

#ifndef DISABLE_DM_BREDR
    /* Deal with mode changes by hand. */
    if (type == DM_HCI_HOLD_MODE_REQ
        || type == DM_HCI_SNIFF_MODE_REQ
        || type == DM_HCI_EXIT_SNIFF_MODE_REQ
        || type == DM_HCI_PARK_MODE_REQ
        || type == DM_HCI_EXIT_PARK_MODE_REQ)
    {
        Send_DM_Mode_Change_Event(status,
                                  &req->dm_hci_with_handle.bd_addr,
                                  0,
                                  0);
        return;
    }

    /* Deal with Role switch by hand. */
    if (type == DM_HCI_SWITCH_ROLE_REQ)
    {
        Send_DM_Switch_Role_Complete(status,
                                     &req->dm_switch_role_req.bd_addr,
                                     0);
        return;
    }
#endif

    /* We've dealt with the special cases. Everything else follows a pattern.
       DM_UPRIM_T is not enormous, so we can allocate for that. */
    cfm = zpnew(DM_UPRIM_T);
    cfm->type = type ^ DM_HCI_WITH_HANDLE;
    cfm->dm_standard_command_cfm.status = status;
    if ((type & DM_HCI_WITH_HANDLE) != 0)
    {
        if (dm_hci_handle_and_tbdaddr(type))
        {
            tbdaddr_copy(&cfm->dm_typed_bd_addr_command_cfm.addrt,
                         &req->dm_hci_ulp_with_handle.addrt);
        }
        else
        {
            bd_addr_copy(&cfm->dm_bd_addr_command_cfm.bd_addr,
                         &req->dm_hci_with_handle.bd_addr);
        }
    }
    DM_SendMessageAMphandle(cfm);
}

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND primitive.

    \param hci_prim Pointer to HCI event primitive.
*/
void Send_DM_Inquiry_Result_With_RSSI(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_INQUIRY_RESULT_WITH_RSSI_T *prim =
                                (HCI_EV_INQUIRY_RESULT_WITH_RSSI_T*)hci_prim;
    DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND_T);
    dm_prim->type = DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND;
    dm_prim->num_responses = prim->num_responses;

    CsrMemCpy(dm_prim->result, prim->result, sizeof(dm_prim->result));

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_EXTENDED_INQUIRY_RESULT_IND primitive.

    \param hci_prim Pointer to HCI event primitive.
*/
void Send_DM_Extended_Inquiry_Result(HCI_UPRIM_T *hci_prim)
{
    HCI_EV_EXTENDED_INQUIRY_RESULT_T *prim =
                                (HCI_EV_EXTENDED_INQUIRY_RESULT_T*)hci_prim;
    DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T *dm_prim;

    dm_prim = pnew(DM_HCI_EXTENDED_INQUIRY_RESULT_IND_T);
    dm_prim->type = DM_HCI_EXTENDED_INQUIRY_RESULT_IND;
    dm_prim->num_responses = prim->num_responses;
    dm_prim->result = prim->result;

    CsrMemCpy(dm_prim->eir_data_part, prim->eir_data_part,
            sizeof(prim->eir_data_part));

    DM_SendMessageAMphandle(dm_prim);
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Build and send a DM_HCI_REMOTE_NAME_CFM primitive.

    \param status Indication of success or failure.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param pp_name Pointer to array of HCI_LOCAL_NAME_BYTE_PACKET_PTRS pointers
           to parts of the user-friendly name of the remote device.
*/
void Send_DM_HCI_Remote_Name_Cfm(
        const CsrUint8 status,
        const BD_ADDR_T *const p_bd_addr,
        CsrUint8 *pp_name[HCI_LOCAL_NAME_BYTE_PACKET_PTRS])
{
    DM_HCI_REMOTE_NAME_CFM_T *dm_prim;

    dm_prim = zpnew(DM_HCI_REMOTE_NAME_CFM_T);
    dm_prim->type = DM_HCI_REMOTE_NAME_CFM;

    dm_prim->status = status;
    bd_addr_copy(&dm_prim->bd_addr, p_bd_addr);
    if (pp_name != NULL)
        CsrMemCpy(dm_prim->name_part, pp_name, sizeof(dm_prim->name_part));

    DM_SendMessageAMphandle(dm_prim);
}

#endif

/*! \brief Build and send a DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM primitive.

    \param status Indication of success or failure.
    \param TYPED_BD_ADDR_T Pointer to Bluetooth device address and type.
*/
void send_dm_ble_update_connection_parameters_cfm(
        const CsrUint8 status,
        const TYPED_BD_ADDR_T *const addrt)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *dm_prim;

    dm_prim = zpnew(DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T);
    dm_prim->type = DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM;
    dm_prim->status = status;
    tbdaddr_copy(&dm_prim->addrt, addrt);
    DM_SendMessageAMphandle(dm_prim);
}

/*! \brief Application request to open ACL.

    \param p_uprim Pointer to command primitive.
*/
void dm_open_acl_req(const DM_UPRIM_T *const p_uprim)
{
    DM_ACL_RESULT_T result;

    /* Try to connect. */
    result = dm_acl_open(&p_uprim->dm_acl_open_req.addrt,
                         NULL,
                         dm_acl_client_application,
                         (CsrUint16)(p_uprim->dm_acl_open_req.flags | DM_ACL_INIT_LOCAL),
                         NULL);
    if (result != DM_ACL_PENDING)
        send_dm_acl_open_cfm(&p_uprim->dm_acl_open_req.addrt,
                             (CsrBool)(result == DM_ACL_SUCCESS));
}

/*! \brief Application request to close ACL.

    \param p_uprim Pointer to command primitive.
*/
void dm_close_acl_req(const DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_ACL_CLIENT_T client = dm_acl_client_application;
    hci_reason_t reason = HCI_ERROR_OETC_USER;

    if ((p_uprim->dm_acl_close_req.flags & DM_ACL_FLAG_FORCE) != 0)
    {
        client = dm_acl_client_max;
        reason = p_uprim->dm_acl_close_req.reason;
    }

    if ((p_uprim->dm_acl_close_req.flags & DM_ACL_FLAG_ALL) != 0)
    {
        /* Close all ACLs. */
        for (p_acl = DM_ACL_FIRST(); p_acl != NULL; p_acl = p_acl->p_next)
            dm_acl_close(p_acl, client, reason, ACL_IDLE_TIMEOUT_AT_LEAST_NONE);
    }
    else
    {
        /* Just close the specified ACL. */
        if ((p_acl = dm_acl_find_by_tbdaddr(
                        &p_uprim->dm_acl_close_req.addrt)) != NULL)
        {
            dm_acl_close(p_acl, client, reason, ACL_IDLE_TIMEOUT_AT_LEAST_NONE);
        }
    }
}

/*! \brief Application has sent us a message that we can't handle.
    Send it back to them.

    \param reason Why we can't handle the messge.
    \param protocol L2CAP_PRIM/RFCOMM_PRIM/etc.
    \param length sizeof(message)
    \param message Pointer to message that we can't handle.
*/
void DM_SendBadMessageInd(CsrUint16 reason,
                          CsrUint8 protocol,
                          CsrUint16 length,
                          void *message)
{
    if (am_phandle == CSR_SCHED_QID_INVALID)
    {
        CsrPmemFree(message);
    }
    else if (message != NULL)
    {
        DM_BAD_MESSAGE_IND_T *prim = pnew(DM_BAD_MESSAGE_IND_T);
        prim->type = DM_BAD_MESSAGE_IND;
        prim->reason = reason;
        prim->protocol = protocol;
        prim->request_type = *(CsrUint16*)message;
        prim->message_length = length;
        prim->message = message;

        DM_SendMessageAMphandle(prim);
    }
}

#ifdef INSTALL_ULP
/*! \brief Application sets scan parameters. If privacy is enabled then we
           make sure that we've selected a random address type.
*/
static void dm_ble_set_scan_parameters_req(const DM_UPRIM_T *const p_uprim)
{
    DM_UPRIM_T *copy = pcopy(p_uprim,
                             sizeof(DM_HCI_ULP_SET_SCAN_PARAMETERS_REQ_T));

    copy->dm_hci_common.op_code = HCI_ULP_SET_SCAN_PARAMETERS;
    copy->dm_hci_common.length = DM_HCI_SENDER_APPLICATION;

#ifdef INSTALL_SM_MODULE
    if (sm_privacy_is_enabled())
        copy->dm_ulp_set_scan_parameters_req.own_address_type = HCI_ULP_ADDRESS_RANDOM;
#endif

    send_to_hci(copy);
}

/*! \brief Application enables scanning. If privacy is enable then we
           write a non-resolvable random address first.
*/
static void dm_ble_set_scan_enable_req(const DM_UPRIM_T *const p_uprim)
{
    DM_UPRIM_T *copy = pcopy(p_uprim,
                             sizeof(DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T));

    copy->dm_hci_common.op_code = HCI_ULP_SET_SCAN_ENABLE;
    copy->dm_hci_common.length = DM_HCI_SENDER_APPLICATION;

#ifdef INSTALL_SM_MODULE
#ifndef DISABLE_SM_PRIVACY
    if (sm_privacy_is_enabled())
        sm_privacy_generate_nonresolvable_private_address(NULL);
#endif
#endif

    send_to_hci(copy);
}
#endif /* INSTALL_ULP */
