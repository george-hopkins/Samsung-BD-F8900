/*!
 Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   dm_acl_manager.c
\brief  The ACL management entity of the BlueStack Device Manager.
*/

#include "csr_synergy.h"

#include "csr_util.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"

#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "error.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_tasks.h"

#ifdef BUILD_FOR_HOST
#include "csr_sched.h"
#include "csr_env_prim.h"
#include "csr_bt_core_stack_pmalloc.h"
#else
#include "buffer.h"
#include "hostio.h"
#include "radioio.h"
#endif

#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_acl_con_par_cache.h"
#include "dm_security_manager.h"
#include "dm_layer_manager.h"
#include "dm_hci_interface.h"
#include "dm_mgt_interface.h"
#include "dm_sync_manager.h"
#include "dm_acl_connection_fsm_definitions.h"
#include "dm_ble_advertising.h"
#include "l2cap_control.h"
#include "dm_amp_interface.h"
#include "tbdaddr.h"

#define DM_ACL_IS_SET(a) ((connection_flags & (a)) != 0)
#define DM_ACL_SET(a) (connection_flags |= (a))
#define DM_ACL_UNSET(a)  \
    (connection_flags &= ~(a))

#define DM_ACL_OUTGOING_IN_PROGRESS     0x0001
#define DM_ACL_AUTHENTICATION_ENABLED   0x0002
#define DM_ACL_INQUIRY_SCAN_REQUESTED   0x0004
#define DM_ACL_PAGE_SCAN_REQUESTED      0x0008

/* HCI Page scan bit for HCI_WRITE_SCAN_ENABLE */
#define DM_ACL_HCI_INQUIRY_SCAN         1
#define DM_ACL_HCI_PAGE_SCAN            2

/* To create an invalid Bluetooth address, we set the LAP to be the
   General/Unlimited Inquiry Access Code (GIAC), which is one of the
   reserved LAPs that shall not be part of any device's Bluetooth address.
   We use it to prevent an ACL from being found once it is dead. */
#define LAP_INVALID 0x9E8B33

#ifdef BUILD_FOR_HOST
static CsrUint16 connection_flags = 0;
#else
static CsrUint16 connection_flags; /* = 0 */
#endif

#ifdef INSTALL_ULP
/* @todo check default values */
const DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T dm_ble_default_conn_params =
{
    0,
    4, /* scan_interval */
    4, /* scan_window */
    0x0028, /* conn_interval_min */
    0x0028, /* conn_interval_max */
    12, /*conn_latency  */
    360, /* supervision_timeout */
    50, /* connection attempt timeout in 1/10s of a second */
    0x20, /* adv_interval_min */
    0x20, /* adv_interval_max */
    L2CA_CON_SLAVE_LATENCY_MAX,  /*conn_latency_max  */
    L2CA_CON_TIMEOUT_MIN, /* supervision_timeout_min */
    L2CA_CON_TIMEOUT_MAX /* supervision_timeout_max */
};

DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T  *p_dm_ble_conn_params = NULL;
#endif

/* Private Function Prototypes */
#ifdef INSTALL_ULP
static void dm_ble_scan_set_enable(CsrUint8 scan_enable, CsrUint8 filter_duplicates, CsrUint8 sender);
static void send_dm_set_ble_connection_parameters_cfm( CsrUint8 status  );
static void dm_acl_fill_ble_conn_params(HCI_ULP_CREATE_CONNECTION_T *p_prim);
static void dm_acl_send_hci_ulp_create_connection(TYPED_BD_ADDR_T *addrt);
static void dm_acl_send_hci_ulp_create_connection_cancel(void);

#endif
static void acl_fsm_crank(DM_ACL_T *p_acl, fsm_event_t event, CsrUint16 reason);

#ifndef DISABLE_DM_BREDR
static void send_hci_read_remote_supp_features(hci_connection_handle_t handle,
                                                        TYPED_BD_ADDR_T *addrt);
static void dm_acl_send_hci_write_scan_enable(CsrUint8 sender);
#endif

static void dm_acl_idle_timeout(CsrUint16 reason, void *p_acl);
#ifdef INSTALL_ULP
static void dm_acl_advert_timeout(CsrUint16 role, void *p_acl_in);
#endif
static void dm_acl_send_hci_disconnect_req(CsrUint16 reason, void *p_acl);
static fsm_event_t dm_acl_disconnected(DM_ACL_T *p_acl);
static DM_ACL_RESULT_T dm_acl_check_connection(DM_ACL_T *p_acl, CsrBool outgoing);

#ifdef ENABLE_SHUTDOWN
/* Public Function Implementations */

/*! \brief Resets the connection flags.

    \param None
*/
void dm_acl_reset_con_flags(void)
{
    /* reset the connection_flags variable */
    DM_ACL_UNSET(DM_ACL_OUTGOING_IN_PROGRESS | DM_ACL_AUTHENTICATION_ENABLED
                 |DM_ACL_INQUIRY_SCAN_REQUESTED | DM_ACL_PAGE_SCAN_REQUESTED);

}
#endif

/*! \brief Withdraw interest in ACL and optionally force closure.
    To force the ACL to close, set client = dm_acl_client_max.

    \param p_acl Pointer to ACL record structure.
    \param client ACL manager client, or dm_acl_client_max to force detach.
    \param reason reason for closing ACL.
    \param idle_multiplier. Number of times to run idle disconnect timer.
*/
void dm_acl_close(DM_ACL_T *const p_acl,
                  const DM_ACL_CLIENT_T client,
                  hci_reason_t reason,
                  const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier)
{
    CsrUint16 dm_reason = reason;

    /* Remember that we've had at least one disconnect request. */
    p_acl->flags |= DM_ACL_DISCONNECT;

    /* Remove lock. */
    dm_acl_unlock(p_acl, client);

    /* For a forced disconnect, remove any existing timeouts. */
    if (client >= dm_acl_client_max)
    {
        dm_reason |= DM_ACL_REASON_FORCE_DETACH;
        p_acl->disconnect_timer_repeats =
            (CsrUint16)ACL_IDLE_TIMEOUT_AT_LEAST_NONE;
    }

    /* Start timeout. */
    dm_acl_idle_timeout_change(p_acl, idle_multiplier, dm_reason);

    /* See if we can cancel straight away. */
    if (p_acl->locks == 0 &&
            (p_acl->flags & DM_ACL_INITIATOR) == DM_ACL_INIT_LOCAL)
        acl_fsm_crank(p_acl, ACL_EV_CANCEL_REQ, reason);
}

/*! \brief Checks whether an LE connection has been tried over BR/EDR
               and vice-versa.

    \param p_acl Pointer to ACL structure.
    \param connection_flags requested connection flags.
    \returns bool type indicating success/failure.
*/
static CsrBool acl_is_allowed(const DM_ACL_T *p_acl, CsrUint16 connection_flags)
{
    CsrUint16 acl_flag = connection_flags & DM_ACL_CONNECTION_MASK;
    CsrBool allowed = TRUE;

    /* Early check , if we have sent a HCI_disconnect anything is allowed */
    if(p_acl->connection_state == ACL_ST_DISCONNECTING)
        return TRUE;

    if(dm_acl_is_ble(p_acl))
    {
        if(acl_flag <= DM_ACL_BR_EDR_FLAG_THRESHOLD)
            allowed = FALSE;
    }
    else
    {
        if(acl_flag > DM_ACL_BR_EDR_FLAG_THRESHOLD)
            allowed = FALSE;
    }
    return allowed;
}

/*! \brief Register interest in ACL and open if not already opened.

    \param TYPED_BD_ADDR_T Pointer to device address and type.
    \param service Pointer to protocol service specification.
    \param client ACL manager client.
    \param flags ACL manager flags.
    \param pp_acl Return pointer to ACL structure.
    \returns DM_ACL_RESULT_T indicating success/failure/pending.
*/
DM_ACL_RESULT_T dm_acl_open(const TYPED_BD_ADDR_T *addrt,
                            const DM_SM_SERVICE_T *service,
                            const DM_ACL_CLIENT_T client,
                            CsrUint16 flags,
                            DM_ACL_T **pp_acl)
{
    DM_ACL_T *p_acl;
    DM_ACL_RESULT_T result;
    fsm_event_t event;
    CsrBool outgoing = (flags & DM_ACL_INITIATOR) == DM_ACL_INIT_LOCAL;

    flags &= DM_ACL_INTERFACE_MASK;

    /* Find existing link. */
    if ((p_acl = dm_acl_find_by_tbdaddr(addrt)) == NULL)
    {
        /* No existing link. If we're initiator then try to create one. */
        if (!outgoing || (p_acl =
                          dm_acl_new(addrt, (CsrUint16)(flags | DM_ACL_ROLE_MASTER))) == NULL)
        {
            return DM_ACL_FAILED;
        }
    }
    else
    {
        /* We know now that we already have a ACL connection to this BD_ADDR
        *   still we need to find out whether -
        *   1) if LE connection, BR/EDR should not be tried over it.
        *   2) if BR/EDR, no LE connection to the same BD ADDR
        */
        if(!acl_is_allowed(p_acl, flags))
            return DM_ACL_FAILED;

        /* An upleasent situation where a HCI_Disconnect has just been sent
            and the p_acl struct has been reset to 0, as there are no locks associated
            with p_acl the time Disconnection was sent. We may want to correctly set the
            connection Flags so that the new queued request on the same p_acl will
            be processed correctly after clean up of the first.
        */
        p_acl->flags |= (flags | DM_ACL_ROLE_MASTER);
    }

    /* For outgoing BR/EDR connections, we may have to pair during link setup
       and might even have to tear down any existing ACL first
       For BLE connections by-pass this. */
    event = FSM_EVENT_NULL;
    if (outgoing && !dm_acl_is_ble(p_acl))
    {
        switch (dm_sm_pair_at_link_setup(p_acl, service))
        {
            case DM_ACL_AUTH_RECONNECT:
                event = ACL_EV_DISCONNECT_REQ;
                /* Fallthrough */

            case DM_ACL_AUTH_CONNECT:
                p_acl->flags |= DM_ACL_WAE;
                break;

            case DM_ACL_CONNECT:
                /* Do nothing. */
                break;
        }
    }

    acl_fsm_crank(p_acl, event, HCI_ERROR_OETC_USER);

    if ((result = dm_acl_check_connection(p_acl, outgoing)) != DM_ACL_FAILED)
    {
        /* Assert ownership. */
        dm_acl_lock(p_acl, client);

        /* Return pointer to ACL structure. */
        if (pp_acl != NULL)
            *pp_acl = p_acl;
    }

    return result;
}

#ifndef DISABLE_DM_BREDR
/* HCI event handling functions */

/*! \brief Handle a connection request event from HCI, indicating
    an incoming peer-initiated connection.

    \param p_prim Pointer to HCI event primitive.
*/
void dm_acl_hci_connect_request(const HCI_EV_CONN_REQUEST_T *const p_prim)
{
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, &p_prim->bd_addr);

    /* Reject if we've temporarily turned on pairing during link setup. */
    if (!DM_ACL_IS_SET(DM_ACL_AUTHENTICATION_ENABLED))
    {
        /* Reject if the connection already exists (collision case),
           or we fail to create a new ACL instance. */
        if (((p_acl = dm_acl_find_by_tbdaddr(&addrt)) != NULL
                || (p_acl = dm_acl_new(&addrt,
                    ((CsrUint16)DM_ACL_INIT_PEER | DM_ACL_ROLE_SLAVE))) != NULL)
#ifdef INSTALL_BD_ADDR_BLACK_LIST_SUPPORT
                && !dm_is_device_in_black_list(&p_prim->bd_addr, NULL)
#endif
                )
        {
            if (p_acl->handle == HCI_HANDLE_INVALID)
            {
                p_acl->flags |= DM_ACL_INIT_PEER;
                p_acl->dev_class = p_prim->dev_class;
                acl_fsm_crank(p_acl, ACL_EV_HCI_CONNECT_IND, 0);
                return;
            }
        }
    }

    Send_HCI_Reject_Connection(&TBDADDR_ADDR(addrt), DM_HCI_SENDER_ACL_MANAGER);
}
#endif

/*! \brief Notification from the HCI interface that a connection
    setup attempt has failed. This can be due to a Command Status or a
    Connection Complete event with an unsuccessful status.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param status Indication of nature of failure.
*/
void dm_acl_hci_connect_failed(const BD_ADDR_T *const p_bd_addr,
        hci_return_t status)
{
    DM_ACL_T *p_acl;
    fsm_event_t event;

    /* Search for the ACL instance, note that its handle will be invalid */
    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL
            && p_acl->handle == HCI_HANDLE_INVALID)
    {
        /* Either fail the connection or discard link key and retry. */
        event = ACL_EV_HCI_CONNECT_FAIL;
        if ((p_acl->flags & DM_ACL_INITIATOR) == DM_ACL_INIT_LOCAL &&
            dm_sm_retry_authentication_event(p_acl, &status) != FSM_EVENT_NULL)
        {
            event = ACL_EV_HCI_CONNECT_RETRY;
        }

        acl_fsm_crank(p_acl, event, status);
    }
}

/*! \brief Notification from the HCI interface that
    connection setup attempt has completed successfully.

    \param p_bd_addr Pointer to Bluetooth address for remote device.
    \param handle HCI connection handle for new connection.
    \param enc_mode Indication of whether the link was encrypted during setup.
*/
void dm_acl_hci_connect_success(const BD_ADDR_T *const p_bd_addr,
        const hci_connection_handle_t handle, const CsrUint8 enc_mode)
{
    DM_ACL_T *p_acl;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);

    /* Get the ACL instance. The ACL structure might not yet exist if the
       HCI connection request has been filtered out, so we create one. */
    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) == NULL
            && (p_acl = dm_acl_new(&addrt,
                   ((CsrUint16)DM_ACL_INIT_PEER | DM_ACL_ROLE_SLAVE))) == NULL)
    {
        Send_HCI_Disconnect(handle, HCI_ERROR_OETC_LOW_RESOURCE,
                                DM_HCI_SENDER_ACL_MANAGER);
        return;
    }

    /* Set the connection handle */
    p_acl->handle = handle;

    acl_fsm_crank(p_acl, ACL_EV_HCI_CONNECT_OK, HCI_SUCCESS);
}

/*! \brief Handle the pending credits associated
    with the p_acl. Returns the credits back to the
    layer manager.

    \param p_acl pointer to the DM ACL structure
    \return a bool type for LE or, BR/EDR
*/
static CsrBool dm_acl_handle_pending_credits(DM_ACL_T *p_acl)
{
    DM_HCI_FLOW_CONTROL_T *fc;
    DM_AMP_ACL_TYPE_T type = DM_AMP_ACL_TYPE_BR_EDR;

#ifdef INSTALL_ULP
    if (  (p_acl->flags & DM_ACL_CONNECTION_MASK) != DM_ACL_CONNECTION_BR_EDR &&
          (p_acl->flags & DM_ACL_CONNECTION_MASK) != DM_ACL_CONNECTION_BR_EDR_BROADCAST )
    {
        type = DM_AMP_ACL_TYPE_LE;
    }
#endif

    fc = DM_GET_FC_TYPE(type);
    fc->used_data_blocks -= p_acl->pending_credits;
    p_acl->pending_credits = 0;

    return (CsrBool)type;
}
/*! \brief Notification from the HCI interface that
    ACL disconnection has failed.

    \param handle HCI connection handle.
*/
void dm_acl_hci_disconnect_failure(const hci_connection_handle_t handle)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        (void)dm_acl_handle_pending_credits(p_acl);
        dm_hci_data_credit_audit();

        acl_fsm_crank(p_acl, ACL_EV_HCI_DISCONNECT_FAIL, HCI_SUCCESS);
    }
}


/*! \brief Notification from the HCI interface that
    ACL disconnection has completed successfully.

    \param handle HCI connection handle
    \param reason reason for disconnection
*/
void dm_acl_hci_disconnect_success(const hci_connection_handle_t handle,
                                   hci_error_t reason)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
    {
        /* Create DM_ACL_CLOSED_IND now before we crank the state machine
           because we need the Bluetooth address and this may well
           be about to be intentionally corrupted now the ACL is dead.
           Also in the case of link-loss, some applications find it
           useful to receive this message before any messages from L2CAP
           and other higher-layer protocols. */
        DM_ACL_CLOSED_IND_T *p_prim = zpnew(DM_ACL_CLOSED_IND_T);
        p_prim->type = DM_ACL_CLOSED_IND;
        p_prim->reason = reason;
        tbdaddr_copy(&p_prim->addrt, &p_acl->addrt);

        if ((p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
            p_prim->flags |= DM_ACL_FLAG_INCOMING;

        /* Check the LE flags */
        if(dm_acl_handle_pending_credits(p_acl)
            || (p_acl->flags & DM_ACL_DISCONNECT_IS_LE))
        {
            /* If the ACL struct being re-used for BR/EDR make sure
             * correct indication is send to the application
             */
            p_acl->flags &= ~ DM_ACL_DISCONNECT_IS_LE;

            p_prim->flags |= DM_ACL_FLAG_ULP;
        }

        DM_SendMessageAMphandle(p_prim);

        dm_hci_data_credit_audit();

        p_acl->handle = HCI_HANDLE_INVALID;
        acl_fsm_crank(p_acl, ACL_EV_HCI_DISCONNECT_OK, reason);

        /* Kick credit-handling code */
        dm_hci_handle_completed_packets(HCI_HANDLE_INVALID, 0);
    }
}


#ifndef DISABLE_DM_BREDR
/*! \brief The ACL manager keeps track of the SNIFF remote supported
    feature for each connection. The Security Manager will want to
    know about Secure Simple Pairing support.

    Note that p_prim->lmp_supp_features is an array of CsrUint16s.
    We cannot use the HCI_FEATURE_IS_SUPPORTED() macro because that
    expects an array of CsrUint8s.

    \param p_prim Pointer to HCI event primitive.
*/
void dm_acl_hci_remote_supported_features(
    const HCI_EV_READ_REM_SUPP_FEATURES_COMPLETE_T *const p_prim)
{
    DM_ACL_T *p_acl;

    if (p_prim->status == HCI_SUCCESS
            && (p_acl = dm_acl_find_by_handle(p_prim->handle)) != NULL)
    {
        p_acl->flags &= ~DM_ACL_RSF_SNIFF;
        if ((p_prim->lmp_supp_features[LMP_FEATURES_SNIFF_MODE_BIT / 16]
                    & (1U << ((LMP_FEATURES_SNIFF_MODE_BIT) & 15))) != 0)
            p_acl->flags |= DM_ACL_RSF_SNIFF;

        dm_sm_remote_ssp_controller_support(p_acl,
                (CsrUint16)(p_prim->lmp_supp_features[LMP_FEATURES_SIMPLE_PAIRING_BIT / 16]
                           & (1U << ((LMP_FEATURES_SIMPLE_PAIRING_BIT) & 15))));
    }
}
#endif

#ifndef DISABLE_DM_BREDR
/*! \brief Received command complete event for HCI write auth enable.

    \param status Indication of success or failure.
    \param auth_enable State of authentication_enabled LMP parameter.
*/
void dm_acl_hci_auth_enable_cc(CsrUint8 status,
                               const CsrUint8 auth_enable)
{
    DM_ACL_T *p_acl;

    if (auth_enable == HCI_AUTH_ENABLE_ON)
    {
        if (status != HCI_SUCCESS)
            DM_ACL_UNSET(DM_ACL_AUTHENTICATION_ENABLED);

        /* Send event to all ACLs. If WAE failed then we proceed anyway.
           This was all just to avoid legacy timeout problems. We're
           not enforcing security at this point. */
        for (p_acl = p_acl_list; p_acl != NULL; p_acl = p_acl->p_next)
            acl_fsm_crank(p_acl, ACL_EV_AUTH_OK, 0);
    }
    else
    {
        if (status == HCI_SUCCESS)
            DM_ACL_UNSET(DM_ACL_AUTHENTICATION_ENABLED);
        else
            send_hci_write_auth_enable(HCI_AUTH_ENABLE_OFF, DM_HCI_SENDER_ACL_MANAGER);
    }

    dm_acl_send_hci_write_scan_enable(DM_HCI_SENDER_ACL_MANAGER);
}
#endif

/*! \brief Crank ACL Manager state machine.
    Note. Calling this function with certain events can cause the destruction
    of the ACL record passed in. In this case, the function will set the
    pointer to NULL.

    \param p_acl Pointer to ACL record structure.
    \param event FSM event to pass in.
    \param pv_data Pointer to be passed through state machine in addition to
           the pointer to the ACL record structure.
*/
static void acl_fsm_crank(DM_ACL_T *p_acl, fsm_event_t event, CsrUint16 status)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    fsm_16bit_run(&dm_acl_connection_fsm, &p_acl->connection_state,
            event, p_acl, &status);

    DM_ACL_DEBUG_CHECK_ALLOW_DEAD(p_acl);

    /* Try to connect the first pending ACL */
    for (   p_acl = p_acl_list;
            p_acl != NULL && !DM_ACL_IS_SET(DM_ACL_OUTGOING_IN_PROGRESS);
            p_acl = p_acl->p_next)
    {
        DM_ACL_DEBUG_CHECK_ALLOW_DEAD(p_acl);

        fsm_16bit_run(&dm_acl_connection_fsm, &p_acl->connection_state,
                ACL_EV_START, p_acl, NULL);
    }
}

/* The ACL has either been closed or failed to open.
   If new locks have been applied then we try to reopen the ACL.
   Otherwise we make it safe until it is removed.

   \param p_acl Pointer to ACL record structure.
   \returns ACL_EV_CONNECT_REQ if the ACL should be reopened or FSM_EVENT_NULL
            if it is to be removed.
*/
static fsm_event_t dm_acl_disconnected(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* Cancel any timers. */
    timer_cancel(&p_acl->disconnect_timer_id);

    p_acl->handle = HCI_HANDLE_INVALID;

    if (p_acl->locks != 0)
    {
        /* Someone seems to want to open the ACL again, so restart. */
        return ACL_EV_CONNECT_REQ;
    }

    /* ACL is dead. Make it safe. It will be removed later. */
#ifdef DM_ACL_DEBUG
    p_acl->sanity = 0xDEAD;
#endif

    /* Prevet ACL from being found via a search. */
    TBDADDR_INVALIDATE(p_acl->addrt);

    return FSM_EVENT_NULL;
}

#ifdef INSTALL_ULP
static void dm_acl_advert_timeout(CsrUint16 role, void *p_acl_in)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)p_acl_in;

    if (dm_acl_exists(p_acl))
    {
        TIMER_EXPIRED(p_acl->advert_timeout_timer_id);

        if ( role == DM_ACL_ROLE_MASTER )
        {
            dm_acl_send_hci_ulp_create_connection_cancel();
        }
        else
        {
            acl_fsm_crank(p_acl, ACL_EV_HCI_CONNECT_FAIL, HCI_ERROR_PAGE_TIMEOUT);
            dm_ble_advertising_restore();
        }
    }
}
#endif
static void dm_acl_send_hci_disconnect_req(CsrUint16 reason, void *pa)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pa;

    if (dm_acl_exists(p_acl))
    {
        TIMER_EXPIRED(p_acl->disconnect_timer_id);

        p_acl->flags |= dm_acl_is_ble(p_acl)?
                          DM_ACL_DISCONNECT_IS_LE: DM_ACL_DISCONNECT_IS_BR_EDR;

        Send_HCI_Disconnect(p_acl->handle, (hci_error_t)(reason & 0xFF),
                DM_HCI_SENDER_ACL_MANAGER);

        /* We've not yet disconnected, so make a note of the returned credits
           and add them to the pool when we have disconnected. */
        p_acl->pending_credits = dm_acl_callback_closed(p_acl, reason);
    }
}

static void dm_acl_send_hci_create_connection_cancel_req(CsrUint16 reason, void *pa)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pa;
    CsrUint16 dm_acl_flag = (p_acl->flags) & DM_ACL_CONNECTION_MASK;
    DM_ACL_DEBUG_CHECK(p_acl);


    switch(dm_acl_flag)
    {
#ifdef INSTALL_ULP
    case DM_ACL_CONNECTION_LE_SLAVE_DIRECTED:
        /* fall through */
    case DM_ACL_CONNECTION_LE_SLAVE_WHITELIST:
        /* fall through */
    case DM_ACL_CONNECTION_LE_SLAVE_UNDIRECTED:
    {
        dm_ble_advertising_restore();
        acl_fsm_crank(p_acl, ACL_EV_HCI_CONNECT_FAIL, reason);
    }
    break;
    case DM_ACL_CONNECTION_LE_MASTER_DIRECTED:
        /* fall through */
    case DM_ACL_CONNECTION_LE_MASTER_WHITELIST:
    {
        dm_acl_send_hci_ulp_create_connection_cancel();
    }
    break;
#endif /* INSTALL_ULP */
    case DM_ACL_CONNECTION_BR_EDR:
        /* fall through */
    case DM_ACL_CONNECTION_BR_EDR_BROADCAST:
    {
        HCI_CREATE_CONNECTION_CANCEL_T *p_prim;
        p_prim = pnew(HCI_CREATE_CONNECTION_CANCEL_T);
        (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);
        dm_acl_send_to_hci((HCI_UPRIM_T*)p_prim, HCI_CREATE_CONNECTION_CANCEL);
    }
    break;
    default:
    /* reach here only when application sends flag 0x001C, which is undefined as of now. */
    break;
    }

}


FSM_ACTION_FN(acl_af_send_hci_disconnect_req)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* Schedule immediate disconnect as new task. */
    timer_start(&p_acl->disconnect_timer_id,
                0,
                dm_acl_send_hci_disconnect_req,
                *(CsrUint16*)pv_arg2,
                p_acl);

    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(acl_af_send_hci_connect_req)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;

#ifdef INSTALL_ULP
    CsrUint8 advertising_filter_policy = HCI_ULP_ADV_FP_ALLOW_ANY;
    CsrUint16 role = DM_ACL_ROLE_MASTER;
#endif

    DM_ACL_DEBUG_CHECK(p_acl);
    DM_ACL_SET(DM_ACL_OUTGOING_IN_PROGRESS);

    switch (p_acl->flags & DM_ACL_CONNECTION_MASK)
    {
#ifndef DISABLE_DM_BREDR
        case DM_ACL_CONNECTION_BR_EDR   :
        {
            HCI_CREATE_CONNECTION_T *prim;

            prim = pnew(HCI_CREATE_CONNECTION_T);
            (void)tbdaddr_copy_to_bd_addr(&prim->bd_addr, &p_acl->addrt);
            prim->pkt_type = DEFAULT_ACL_PACKET_TYPE & LYM_Supported_packet_types();

            /* Allow role switch if supported. */
            prim->allow_role_switch = dm_acl_role_policy(FALSE, &TBDADDR_ADDR(p_acl->addrt));

            dm_acl_obtain_connection_details(&prim->bd_addr, &prim->page_scan_mode,
                    &prim->page_scan_rep_mode, &prim->clock_offset);

            dm_acl_send_to_hci((HCI_UPRIM_T *)prim, HCI_CREATE_CONNECTION);

            break;
        }
#endif
#ifdef INSTALL_ULP
        case DM_ACL_CONNECTION_LE_MASTER_DIRECTED   :
        {
            dm_acl_send_hci_ulp_create_connection(&p_acl->addrt);
            break;
        }
        case DM_ACL_CONNECTION_LE_MASTER_WHITELIST  :
        {
            dm_acl_send_hci_ulp_create_connection(NULL);
            break;
        }
        case DM_ACL_CONNECTION_LE_SLAVE_DIRECTED    :
        {
            role = DM_ACL_ROLE_SLAVE;
            dm_ble_advertising_slave_connect(&p_acl->addrt,
                                             HCI_ULP_ADV_FP_ALLOW_ANY);
            break;
        }
        case DM_ACL_CONNECTION_LE_SLAVE_WHITELIST   :
            advertising_filter_policy = HCI_ULP_ADV_FP_ALLOW_SCANNING;
            /* FALLTHROUGH */
        case DM_ACL_CONNECTION_LE_SLAVE_UNDIRECTED  :
        {
            role = DM_ACL_ROLE_SLAVE;
            /*   How to distinguish between whitelist undirected and undirected ?*/
            dm_ble_advertising_slave_connect(NULL,
                                             advertising_filter_policy);
            break;
        }
#endif
        default :
        {
            /* @panic error */
            break;
        }
    }
#ifdef INSTALL_ULP
    if ( (p_acl->flags & DM_ACL_CONNECTION_LE_ADVERT_TIMEOUT) != 0)
    {
        const DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *p =
            p_dm_ble_conn_params == NULL ?
            &dm_ble_default_conn_params : p_dm_ble_conn_params;

        timer_start(&p_acl->advert_timeout_timer_id,
                    p->conn_attempt_timeout * CSR_SCHED_SECOND / 10,
                    dm_acl_advert_timeout,
                    role,
                    (void *) p_acl);
    }
#endif


    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(acl_af_check_write_auth_enable)
{
    fsm_event_t event = FSM_EVENT_NULL;
#ifndef DISABLE_DM_BREDR
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    if ((p_acl->flags & DM_ACL_WAE) != 0
            && !DM_ACL_IS_SET(DM_ACL_AUTHENTICATION_ENABLED))
    {
        DM_ACL_SET(DM_ACL_AUTHENTICATION_ENABLED);
        dm_acl_send_hci_write_scan_enable(DM_HCI_SENDER_ACL_MANAGER);
        send_hci_write_auth_enable(HCI_AUTH_ENABLE_ON, DM_HCI_SENDER_ACL_MANAGER);
    }
    else
#endif
        event = ACL_EV_AUTH_OK;
    return event;
}

FSM_ACTION_FN(acl_af_connect_complete)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;
    hci_return_t status = (hci_return_t)*(CsrUint16*)pv_arg2;

    DM_ACL_DEBUG_CHECK(p_acl);

    dm_acl_callback_opened(p_acl, status);

    DM_ACL_UNSET(DM_ACL_OUTGOING_IN_PROGRESS);
    if ((p_acl->flags & DM_ACL_WAE) != 0)
    {
#ifndef DISABLE_DM_BREDR
        send_hci_write_auth_enable(HCI_AUTH_ENABLE_OFF, DM_HCI_SENDER_ACL_MANAGER);
#endif
        p_acl->flags &= ~DM_ACL_WAE;
    }

    if (status == HCI_SUCCESS)
    {
#ifndef DISABLE_DM_BREDR
        if ( ( p_acl->flags & DM_ACL_CONNECTION_MASK) == DM_ACL_CONNECTION_BR_EDR )
        {
            DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T *p_prim;

            /* Request the remote supported features */
            send_hci_read_remote_supp_features(p_acl->handle, &p_acl->addrt);

            /* Request the remote host supported features */
            p_prim = pnew(DM_HCI_READ_REMOTE_EXT_FEATURES_REQ_T);
            (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);
            p_prim->page_num = 1;
            dm_acl_send_to_hci_with_handle((HCI_UPRIM_T *)p_prim,
                    HCI_READ_REMOTE_EXT_FEATURES);
        }
#endif
        /* enable data buffer */
#ifndef BUILD_FOR_HOST
        hostio_enable_tohost_data(p_acl->handle);
#endif
        return FSM_EVENT_NULL;
    }
    else
    {
        p_acl->locks = 0;

        return dm_acl_disconnected(p_acl);
    }
}

FSM_ACTION_FN(acl_af_send_hci_connect_accept)
{
#ifndef DISABLE_DM_BREDR
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;
    HCI_ACCEPT_CONNECTION_REQ_T *p_prim;

    DM_ACL_DEBUG_CHECK(p_acl);

    p_prim = pnew(HCI_ACCEPT_CONNECTION_REQ_T);
    (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, &p_acl->addrt);

    p_prim->role = dm_acl_role_policy(TRUE, &TBDADDR_ADDR(p_acl->addrt));
    dm_acl_send_to_hci((HCI_UPRIM_T *)p_prim, HCI_ACCEPT_CONNECTION_REQ);
#endif
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(acl_af_disconnect)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;
    CsrUint8 reason = (CsrUint8)*(CsrUint16*)pv_arg2;
    DM_HCI_FLOW_CONTROL_T *fc = NULL;

    DM_ACL_DEBUG_CHECK(p_acl);

   /* Get the pointer to the ACL flow control data structure */
    fc = dm_amp_get_fc_type(p_acl);

    /* We know that we've already disconnected, so we can return credits
       directly back to the pool. */
    fc->used_data_blocks -= dm_acl_callback_closed(p_acl, reason);

    return dm_acl_disconnected(p_acl);
}

FSM_ACTION_FN(acl_af_link_loss)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* For link loss, all locks get removed. */
    p_acl->locks = 0;

    return acl_af_disconnect(pv_arg1, pv_arg2);
}

FSM_ACTION_FN(acl_af_send_hci_connect_cancel)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_arg1;

    DM_ACL_DEBUG_CHECK(p_acl);

    timer_cancel(&p_acl->disconnect_timer_id);

    /* Schedule immediate disconnect as new task. */
    timer_start(&p_acl->disconnect_timer_id,
                0,
                dm_acl_send_hci_create_connection_cancel_req,
                *(CsrUint16*)pv_arg2,
                p_acl);



    return FSM_EVENT_NULL;
}

CsrBool dm_acl_is_connected(const DM_ACL_T *const p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    return p_acl->connection_state == ACL_ST_CONNECTED;
}

static DM_ACL_RESULT_T dm_acl_check_connection(DM_ACL_T *p_acl, CsrBool outgoing)
{
    DM_ACL_RESULT_T result = DM_ACL_PENDING;

    DM_ACL_DEBUG_CHECK(p_acl);

    switch (p_acl->connection_state)
    {
        case ACL_ST_CONNECTED:
            result = DM_ACL_SUCCESS;
            break;

        case ACL_ST_CONNECTING:
            /* result = DM_ACL_PENDING; */
            break;

        case ACL_ST_IDLE:
            result = DM_ACL_FAILED;
            break;

        default:
            /* result = DM_ACL_PENDING; */
            if (!outgoing)
                result = DM_ACL_FAILED;
            break;
    }

    return result;
}

#ifndef DISABLE_DM_BREDR
/*! \brief Read the supported features of the remote controller.

    \param handle ACL handle of existing connection to remote device.
    \param p_TYPED_BD_ADDR_T Pointer to Bluetooth address of remote device.
*/
static void send_hci_read_remote_supp_features(hci_connection_handle_t handle,
                                                        TYPED_BD_ADDR_T *addrt)
{
    HCI_READ_REMOTE_SUPP_FEATURES_T *p_prim;

    p_prim = pnew(HCI_READ_REMOTE_SUPP_FEATURES_T);
    p_prim->handle = handle;
    (void)tbdaddr_copy_to_bd_addr(&p_prim->bd_addr, addrt);

    dm_acl_send_to_hci((HCI_UPRIM_T*)p_prim, HCI_READ_REMOTE_SUPP_FEATURES);
}
#endif

/*! \brief Pretend that a previous timeout has just fired.

    Then change the number of timer repeats if necessary.
    This will either start a new timeout or, if there are none
    left, disconnect immediately.

    \param p_acl Pointer to ACL record structure.
    \param idle_multiplier Number of multiples of the timeout required.
    \param reason Reason for disconnecting.
*/
void dm_acl_idle_timeout_change(
        DM_ACL_T *const p_acl,
        const ACL_IDLE_TIMEOUT_MULTIPLIER_T idle_multiplier,
        CsrUint16 reason)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* We will call change idle timeout only when we are not already disconnecting.
     * as it involves stopping of the disconnect timer started earlier. Certainly its not
     * a good idea to stop disconnection in the middle as it may cause state machines
     * waiting for something to happen to stall.
    */
    if(p_acl->connection_state != ACL_ST_DISCONNECTING)
    {
        CsrUint16 disconnect_timer_repeats;

        timer_cancel(&p_acl->disconnect_timer_id);

        disconnect_timer_repeats = p_acl->disconnect_timer_repeats;
        if ((idle_multiplier & ACL_IDLE_TIMEOUT_AT_MOST) == 0)
        {
            if (idle_multiplier > disconnect_timer_repeats)
                disconnect_timer_repeats = (CsrUint16)idle_multiplier;
        }
        else
        {
            CsrUint16 im = (CsrUint16)idle_multiplier & ~ACL_IDLE_TIMEOUT_AT_MOST;

            if (im < disconnect_timer_repeats)
                disconnect_timer_repeats = im;
        }

        p_acl->disconnect_timer_repeats = disconnect_timer_repeats + 1;
        dm_acl_idle_timeout(reason, p_acl);
    }
}

/*! \brief Standard short ACL timeout. Just calls dm_acl_idle_timeout_change()

    \param p_acl Pointer to ACL record structure.
*/
void dm_acl_idle_timeout_extend_short(DM_ACL_T *const p_acl)
{
    dm_acl_idle_timeout_change(p_acl, ACL_IDLE_TIMEOUT_AT_MOST_SHORT,
            HCI_ERROR_OETC_USER);
}

static void dm_acl_idle_timeout(CsrUint16 reason, void *pa)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pa;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* Make sure that the ACL still exists. */
    if (dm_acl_exists(p_acl))
    {
        /* Idle timeout complete. */
        TIMER_EXPIRED(p_acl->disconnect_timer_id);

        if (--p_acl->disconnect_timer_repeats == 0)
        {
            if ((p_acl->flags & DM_ACL_DISCONNECT) != 0)
            {
                p_acl->flags &= ~DM_ACL_DISCONNECT;

                if (p_acl->locks == 0)
                    acl_fsm_crank(p_acl, ACL_EV_DISCONNECT_REQ, reason);
            }
        }
        else
        {
            /* Timer repeats still greater than zero, so start new timer. */
            timer_start(&p_acl->disconnect_timer_id,
                        DM_CONFIG_ACL_IDLE_TIMEOUT_BASE,
                        dm_acl_idle_timeout,
                        reason,
                        p_acl);
        }
    }
}

#ifndef DISABLE_DM_BREDR
void dm_acl_write_scan_enable_req(const DM_UPRIM_T *const p_uprim)
{
    DM_ACL_UNSET(DM_ACL_PAGE_SCAN_REQUESTED|DM_ACL_INQUIRY_SCAN_REQUESTED);

    if ((p_uprim->dm_write_scan_enable_req.scan_enable & DM_ACL_HCI_INQUIRY_SCAN) != 0)
    {
        DM_ACL_SET(DM_ACL_INQUIRY_SCAN_REQUESTED);
    }

    if ((p_uprim->dm_write_scan_enable_req.scan_enable & DM_ACL_HCI_PAGE_SCAN) != 0)
    {
        DM_ACL_SET(DM_ACL_PAGE_SCAN_REQUESTED);
    }

    dm_acl_send_hci_write_scan_enable(
            (DM_HCI_SENDER_APPLICATION|DM_HCI_SENDER_ACL_MANAGER));
}
#endif

#ifndef DISABLE_DM_BREDR
static void dm_acl_send_hci_write_scan_enable(CsrUint8 sender)
{
    HCI_WRITE_SCAN_ENABLE_T *p_prim;

    p_prim = zpnew(HCI_WRITE_SCAN_ENABLE_T);
    p_prim->common.op_code = HCI_WRITE_SCAN_ENABLE;
    p_prim->common.length = sender;

    if (DM_ACL_IS_SET(DM_ACL_INQUIRY_SCAN_REQUESTED))
        p_prim->scan_enable = DM_ACL_HCI_INQUIRY_SCAN;

    if (DM_ACL_IS_SET(DM_ACL_PAGE_SCAN_REQUESTED) &&
            !DM_ACL_IS_SET(DM_ACL_AUTHENTICATION_ENABLED))
        p_prim->scan_enable |= DM_ACL_HCI_PAGE_SCAN;

    send_to_hci((DM_UPRIM_T*)p_prim);
}
#endif

#ifdef INSTALL_ULP

static void dm_acl_fill_ble_conn_params(HCI_ULP_CREATE_CONNECTION_T *p_prim)
{
    if (p_dm_ble_conn_params == NULL)
    {
        p_dm_ble_conn_params = pcopy(&dm_ble_default_conn_params,
                                     sizeof(DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T));
    }

    /* Connect agressively by scanning constantly */
    p_prim->scan_interval       =  p_dm_ble_conn_params->scan_interval;
    p_prim->scan_window         =  p_dm_ble_conn_params->scan_window;

    /* Connection interval in units of 1.25ms - so 50ms. */
    p_prim->conn_interval_min   = p_dm_ble_conn_params->conn_interval_min;
    p_prim->conn_interval_max   = p_dm_ble_conn_params->conn_interval_max;

    /* Sniff Subrate type asymmetry. */
    p_prim->conn_latency        =  p_dm_ble_conn_params->conn_latency;

    /* In units of 10ms. Use 6 * Interval * latency, which will allow
       us to try sending data 6 times before giving up. */
    p_prim->supervision_timeout =  p_dm_ble_conn_params->supervision_timeout;
}

static void dm_acl_send_hci_ulp_create_connection(TYPED_BD_ADDR_T *addrt)
{
    HCI_ULP_CREATE_CONNECTION_T *p_prim;

    dm_ble_scan_set_enable(HCI_ULP_SCAN_DISABLED,
                           HCI_ULP_FILTER_DUPLICATES_DISABLED,
                           DM_HCI_SENDER_BLUESTACK);


    p_prim = zpnew(HCI_ULP_CREATE_CONNECTION_T);
    p_prim->common.op_code = HCI_ULP_CREATE_CONNECTION;
    p_prim->common.length = DM_HCI_SENDER_ACL_MANAGER;

    if ( addrt != NULL)
    {

        p_prim->peer_address_type =
            tbdaddr_copy_to_bd_addr(&p_prim->peer_address, addrt);
    }
    else
    {
        p_prim->initiator_filter_policy = 1;
    }

    dm_acl_fill_ble_conn_params(p_prim);
    /*
        p_prim->initiator_filter_policy = 0;
        p_prim->peer_address_type = 0;
        p_prim->own_address_type = 0;
        p_prim->minimum_ce_length = 0;
        p_prim->maximum_ce_length = 0;
    */

    send_to_hci((DM_UPRIM_T*)p_prim);
}

static void dm_acl_send_hci_ulp_create_connection_cancel(void)
{
    HCI_ULP_CREATE_CONNECTION_CANCEL_T *p_prim;

    p_prim = zpnew(HCI_ULP_CREATE_CONNECTION_CANCEL_T);
    p_prim->common.op_code = HCI_ULP_CREATE_CONNECTION_CANCEL;
    p_prim->common.length = DM_HCI_SENDER_ACL_MANAGER;

    send_to_hci((DM_UPRIM_T*)p_prim);
}

void dm_acl_ulp_opened(const HCI_EV_ULP_CONNECTION_COMPLETE_T *prim)
{
    DM_ACL_T *p_acl;
    CsrUint16 flags;
    CsrUint16 event;
    TYPED_BD_ADDR_T addrt;
    CsrBool disconnect = TRUE;

    tbdaddr_copy_from_bd_addr(&addrt,
                              prim->peer_address_type,
                              &prim->peer_address);

    /* Get the ACL instance. The ACL structure might not yet exist because
       there is no HCI connection request for BLE. */
    p_acl = dm_acl_find_by_tbdaddr(&addrt);
    if (p_acl == NULL)
    {
        p_acl = dm_acl_find_white_list_or_undirected();
        if (p_acl == NULL && prim->status == HCI_SUCCESS)
        {
            p_acl = dm_acl_new(&addrt, DM_ACL_INIT_PEER | DM_ACL_ROLE_SLAVE
                         | DM_ACL_CONNECTION_LE_SLAVE_DIRECTED);
        }
    }

    if (p_acl != NULL)
    {
        /* We are sure that if handle is invalid then we don't have a active
        *   BR/EDR or, LE connection with the specified BD_ADDR.
        */
        if(p_acl->handle == HCI_HANDLE_INVALID)
        {
            DM_ACL_BLE_CONN_PARAMS_T *ble_pars;

            disconnect = FALSE;
            tbdaddr_copy(&p_acl->addrt, &addrt);

            /* Make sure that we have the correct role. */
            p_acl->flags &= ~DM_ACL_ROLE_SLAVE;
            if (prim->role != HCI_MASTER)
            {
                p_acl->flags = p_acl->flags | DM_ACL_ROLE_SLAVE | DM_ACL_INIT_PEER;
            }

            flags = p_acl->flags & DM_ACL_CONNECTION_MASK;

            if ( flags == DM_ACL_CONNECTION_LE_SLAVE_WHITELIST  ||
                 flags == DM_ACL_CONNECTION_LE_SLAVE_UNDIRECTED )
            {
                dm_ble_advertising_restore();
            }

            if ( (p_acl->flags & DM_ACL_CONNECTION_LE_ADVERT_TIMEOUT) != 0)
            {
                timer_cancel(&p_acl->advert_timeout_timer_id);
            }


            /* Set the connection handle */
            p_acl->handle = (hci_connection_handle_t)prim->connection_handle;

            if ( p_acl->ble_conn_params == NULL )
            {
                p_acl->ble_conn_params = pnew(DM_ACL_BLE_CONN_PARAMS_T);
            }

            ble_pars = p_acl->ble_conn_params;
            ble_pars->conn_interval   = prim->conn_interval;
            ble_pars->conn_latency    = prim->conn_latency;
            ble_pars->supervision_timeout = prim->supervision_timeout;
            ble_pars->clock_accuracy  = prim->clock_accuracy;

            /* enable data buffer */
#ifndef BUILD_FOR_HOST
            hostio_enable_tohost_data(p_acl->handle);
#endif
            event = ( prim->status == HCI_SUCCESS ) ? ACL_EV_HCI_CONNECT_OK : ACL_EV_HCI_CONNECT_FAIL;

            acl_fsm_crank(p_acl, event, prim->status);
        }
        /* This connection can not be handled, just disconnect the physical link */
    }

    if(disconnect)
    {
        Send_HCI_Disconnect(prim->connection_handle,
                            HCI_ERROR_OETC_LOW_RESOURCE,
                            DM_HCI_SENDER_ACL_MANAGER);
    }
}

static void dm_ble_scan_set_enable(CsrUint8 scan_enable, CsrUint8 filter_duplicates, CsrUint8 sender)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T *p_prim = pnew(DM_HCI_ULP_SET_SCAN_ENABLE_REQ_T);

    p_prim->common.op_code      = HCI_ULP_SET_SCAN_ENABLE;
    p_prim->common.length       = sender;
    p_prim->scan_enable         = scan_enable;
    p_prim->filter_duplicates   = filter_duplicates;

    send_to_hci((DM_UPRIM_T *) p_prim);
}


#endif

#ifdef INSTALL_ULP
static void send_dm_set_ble_connection_parameters_cfm( CsrUint8 status  )
{
    DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *p_prim;

    p_prim = pnew(DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T);

    p_prim->type = DM_SET_BLE_CONNECTION_PARAMETERS_CFM;
    p_prim->phandle = am_phandle;
    p_prim->status = status;

    DM_SendMessage(p_prim);
}


void dm_ble_set_connection_parameters_req(const DM_UPRIM_T * const uprim)
{
    CsrUint8 status = HCI_ERROR_UNSUPPORTED_FEATURE;
    const DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *p_prim = &uprim->dm_set_ble_connection_parameters_req;

    if (p_prim->conn_interval_min >= L2CA_CON_INTERVAL_MIN
            && p_prim->conn_interval_min <= p_prim->conn_interval_max
            && p_prim->conn_interval_max <= L2CA_CON_INTERVAL_MAX
            && p_prim->supervision_timeout >= L2CA_CON_TIMEOUT_MIN
            && p_prim->supervision_timeout <= L2CA_CON_TIMEOUT_MAX
            && p_prim->conn_latency <= L2CA_CON_SLAVE_LATENCY_MAX
            && p_prim->conn_latency + 1 <= ( p_prim->supervision_timeout * 8)/ p_prim->conn_interval_max
            && p_prim->supervision_timeout >= p_prim->supervision_timeout_min
            && p_prim->supervision_timeout <= p_prim->supervision_timeout_max
            && p_prim->conn_latency_max <= L2CA_CON_SLAVE_LATENCY_MAX )
    {

        status = HCI_SUCCESS;
        CsrPmemFree(p_dm_ble_conn_params);
        /*  take a copy because uprim gets freed */
        p_dm_ble_conn_params = pcopy(&uprim->dm_set_ble_connection_parameters_req,
                                     sizeof(DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T));
    }
   send_dm_set_ble_connection_parameters_cfm(status);
}
void dm_ble_update_connection_parameters_req(const DM_UPRIM_T * const uprim)
{

    CsrUint8 status = HCI_ERROR_UNSUPPORTED_FEATURE;
    const DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *p_prim = &uprim->dm_ble_update_connection_parameters_req;

    DM_UPRIM_T *p_uprim_copy;
    DM_ACL_T *p_acl = NULL;

    if ( ( p_acl = dm_acl_find_by_tbdaddr(&p_prim->addrt)) != NULL   &&
         ( p_acl->connection_state == ACL_ST_CONNECTED)  )
    {
        if ( (p_acl->flags & DM_ACL_ROLE) == DM_ACL_ROLE_MASTER )
        {
            p_uprim_copy = pcopy(uprim,
                               sizeof(DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T));

            p_uprim_copy->dm_ble_update_connection_parameters_req.connection_handle = p_acl->handle;

            dm_acl_send_to_hci_with_handle((HCI_UPRIM_T*)p_uprim_copy,HCI_ULP_CONNECTION_UPDATE);
        }
        else
        {
            L2CA_ConnUpdateReq(DM_IFACEQUEUE,
                               &p_prim->addrt,
                               p_prim->conn_interval_min,
                               p_prim->conn_interval_max,
                               p_prim->conn_latency,
                               p_prim->supervision_timeout);
        }

    }
    else
    {
        status = HCI_ERROR_COMMAND_DISALLOWED;
        send_dm_ble_update_connection_parameters_cfm(status, &p_prim->addrt);
    }
}
#endif

#ifdef INSTALL_ULP

void dm_ble_accept_connection_par_update_rsp(const DM_UPRIM_T * const uprim)
{
    DM_ACL_T *p_acl;
    const DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *p_prim =
        &uprim->dm_ble_accept_connection_par_update_rsp;
    CsrUint16 result = L2CAP_CONNECTION_PARAMETER_UPDATE_REJECT;

    /* We need to verify protocol correctness of the parameters and acceptability from app */
    if (p_prim->conn_interval_min >= L2CA_CON_INTERVAL_MIN
        && p_prim->conn_interval_min <= p_prim->conn_interval_max
        && p_prim->conn_interval_max <= L2CA_CON_INTERVAL_MAX
        && p_prim->supervision_timeout >= L2CA_CON_TIMEOUT_MIN
        && p_prim->supervision_timeout <= L2CA_CON_TIMEOUT_MAX
        && p_prim->conn_latency <= L2CA_CON_SLAVE_LATENCY_MAX
        &&(p_prim->conn_latency + 1) <= (p_prim->supervision_timeout * 8)/ p_prim->conn_interval_max
        && p_prim->result == L2CAP_CONNECTION_PARAMETER_UPDATE_ACCEPT)
    {
        HCI_ULP_CONNECTION_UPDATE_T *p_hci_prim;

        /* Find connection handle and apply to HCI */
        p_acl = dm_acl_find_by_tbdaddr(&p_prim->bd_addrt);
        if(NULL != p_acl)
        {
            result = L2CAP_CONNECTION_PARAMETER_UPDATE_ACCEPT;

            p_hci_prim = zpnew(HCI_ULP_CONNECTION_UPDATE_T);
            p_hci_prim->common.op_code = HCI_ULP_CONNECTION_UPDATE;
            p_hci_prim->common.length = DM_HCI_SENDER_BLUESTACK;
            p_hci_prim->connection_handle = p_acl->handle;
            p_hci_prim->conn_interval_min = p_prim->conn_interval_min;
            p_hci_prim->conn_interval_max = p_prim->conn_interval_max;
            p_hci_prim->conn_latency = p_prim->conn_latency;
            p_hci_prim->supervision_timeout = p_prim->supervision_timeout;
            p_hci_prim->minimum_ce_length = 1;
            p_hci_prim->maximum_ce_length = 1;
            send_to_hci((DM_UPRIM_T*)p_hci_prim);
        }
    }

    L2CA_AcceptConnUpdateRsp(L2CAP_IFACEQUEUE,
                             &p_prim->bd_addrt,
                             result,
                             p_prim->signal_id);
}

void dm_handle_accept_connection_par_update_signal(L2CA_UPRIM_T* message)
{
    /* send dm_ble_l2cap_connection_par_update_ind */
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *p_prim;

    L2CA_CONNECTION_PAR_UPDATE_IND_T* p_uprim=&(message->l2ca_connection_par_update_ind);

    p_prim = pcopy(p_uprim, sizeof(DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T));
    p_prim->type = DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND;
    DM_SendMessageAMphandle(p_prim);
}
#endif
