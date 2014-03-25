/*!
 Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   dm_acl_common.c

\brief  Management of the ACL database.
*/

#include "csr_synergy.h"
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "l2cap_prim.h"
#include "dm_prim.h"

#include "csr_bt_core_stack_pmalloc.h"

#include "error.h"
#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_acl_con_par_cache.h"
#include "dm_acl_connection_fsm_definitions.h"
#include "dm_hci_interface.h"
#include "dm_security_manager.h"
#include "dm_mgt_interface.h"
#include "dm_sync_manager.h"
#include "dm_acl_policy_manager.h"
#include "dm_acl_powerstates.h"
#include "dm_amp_interface.h"
#include "l2cap_deinit.h"
#include "l2cap_con_handle.h"
#include "csr_util.h"
#include "csr_util.h"
#include "tbdaddr.h"

dm_acl_client_locks_t dm_acl_client_locks(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client);

/* Callback arrays.

   NOTE: These must all be valid functions. We do NOT check for NULL.
*/

/* Callback ACL opened array by client. */
static const DM_ACL_CALLBACK_OPENED_T dm_acl_callback_function_opened[] =
{
    dm_sm_acl_opened,               /* Security Manager */
    send_dm_acl_opened_ind,         /* Application */
    CH_ConnectInd,                  /* L2CAP */
#ifdef INSTALL_DM_SYNC_MODULE
    dm_sync_acl_opened,             /* Sync Manager */
#endif
#ifdef INSTALL_AMP_SUPPORT
    dm_amp_acl_opened,              /* AMP Manager */
#endif
#ifndef DISABLE_DM_BREDR
    dm_policy_acl_opened            /* Policy Manager */
#endif
};

/* Callback ACL closed array by client. */
static const DM_ACL_CALLBACK_CLOSED_T dm_acl_callback_function_closed[] =
{
    dm_sm_acl_closed,               /* Security Manager */
    NULL,                           /* Application */
    CH_DisconnectInd,               /* L2CAP */
#ifdef INSTALL_DM_SYNC_MODULE
    dm_sync_acl_closed,             /* Sync Manager */
#endif
#ifdef INSTALL_AMP_SUPPORT
    dm_amp_acl_closed,              /* AMP Manager */
#endif
#ifndef DISABLE_DM_BREDR
    dm_policy_acl_closed            /* Policy Manager */
#endif
};

/* Ensure these arrays stay in tune with the DM ACL client enum */
CSR_BT_COMPILE_TIME_ASSERT(sizeof(dm_acl_callback_function_closed)/sizeof(DM_ACL_CALLBACK_CLOSED_T) == dm_acl_client_max, dm_acl_callback_closed_array_size);
CSR_BT_COMPILE_TIME_ASSERT(sizeof(dm_acl_callback_function_opened)/sizeof(DM_ACL_CALLBACK_OPENED_T) == dm_acl_client_max, dm_acl_callback_opened_array_size);

/* Linked list of ACL connections */
DM_ACL_T *p_acl_list = NULL;

static void dm_acl_reset_acl(DM_ACL_T *p_acl)
{
    /* Cancel all timers. */
    timer_cancel(&p_acl->disconnect_timer_id);

#ifdef INSTALL_ULP
    if ( p_acl->ble_conn_params != NULL )
    {
        CsrPmemFree(p_acl->ble_conn_params);
    }
#endif

    /* Zero the ACL structure. */
    CsrMemSet(DM_ACL_RESET_START(p_acl), 0, DM_ACL_RESET_SIZE);

    /* Only flags we need to keep are sniff and WAE. Zero the rest. */
    p_acl->flags &= (DM_ACL_WAE | DM_ACL_RSF_SNIFF
                     | DM_ACL_INITIATOR | DM_ACL_DISCONNECT_IS_LE);

    /* Reset client data. */
    CH_Reset(&p_acl->dm_acl_client_l2cap);     /* L2CAP */
    dm_sm_acl_reset(&p_acl->dm_acl_client_sm); /* Security Manager */
#ifndef DISABLE_DM_BREDR
    dm_acl_policy_reset(p_acl);
#endif
    DM_ACL_DEBUG_CHECK(p_acl);
}

/*! \brief Reset the ACL manager. */
void dm_acl_init(void)
{
    DM_ACL_T *p_temp;

    /* Destroy the connection list */
    while (p_acl_list)
    {
        p_temp = p_acl_list;
        p_acl_list = p_acl_list->p_next;

        /* TODO send correct message. */
        p_temp->locks = 0;
        (void)dm_acl_callback_closed(p_temp,
                HCI_ERROR_CONN_TERM_LOCAL_HOST);

        CsrPmemFree(p_temp);
    }

#ifndef DISABLE_DM_BREDR
    /* Reset connection parameter cache */
    reset_conn_param_cache();
#endif
}

#ifdef ENABLE_SHUTDOWN
/* Reset the list of acl connections - used during deinit */
void dm_acl_deinit(void)
{
    /* Destroy the connection list */
    while (p_acl_list)
    {
        DM_ACL_T *tmp;

        /* Client deinit */
        dm_acl_client_deinit_sm(&p_acl_list->dm_acl_client_sm,
                                dm_acl_is_ble(p_acl_list));
        dm_acl_client_deinit_l2cap(&p_acl_list->dm_acl_client_l2cap);
#ifdef INSTALL_DM_SYNC_MODULE
        dm_acl_client_deinit_sync(&p_acl_list->dm_acl_client_sync);
#endif
#ifdef INSTALL_AMP_SUPPORT
        dm_acl_client_deinit_amp(&p_acl_list->dm_acl_client_amp);
#endif
        dm_acl_policy_reset(p_acl_list);

#ifdef INSTALL_ULP
        CsrPmemFree(p_acl_list->ble_conn_params);
#endif /* INSTALL_ULP */

        /* Kill timers */
        timer_cancel(&p_acl_list->disconnect_timer_id);

        /* Remove and destroy ACL structure */
        tmp = p_acl_list;
        p_acl_list = p_acl_list->p_next;
        CsrPmemFree(tmp);
    }

    dm_acl_reset_con_flags();

#ifndef DISABLE_DM_BREDR
    reset_conn_param_cache();
#endif

#ifdef INSTALL_BD_ADDR_BLACK_LIST_SUPPORT
    dm_remove_all_devices_from_black_list();
#endif
}
#endif

/*! \brief Creates a new ACL connection instance using the supplied BD address.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param initiator DM_ACL_INIT_LOCAL or DM_ACL_INIT_PEER.
    \returns Pointer to ACL structure, or NULL if creation fails.
*/
DM_ACL_T *dm_acl_new(const TYPED_BD_ADDR_T *const addrt, const CsrUint16 flags)
{
    DM_ACL_T *p_new_acl;

    /* Get a new connection list entry */
    if ((p_new_acl = xzpnew(DM_ACL_T)) != NULL)
    {
        /* Add the connection to the list */
        p_new_acl->p_next = p_acl_list;
        p_acl_list = p_new_acl;

#ifdef DM_ACL_DEBUG
        p_new_acl->sanity = 0xBEEF;
#endif

        /* Initialise the connection details */
        tbdaddr_copy(&p_new_acl->addrt, addrt);
        p_new_acl->handle = HCI_HANDLE_INVALID;
        p_new_acl->connection_state = ACL_ST_PENDING;

        /* Reset will trigger default values for the rest. */
        dm_acl_reset_acl(p_new_acl);

        p_new_acl->flags = flags;
    }

    return p_new_acl;
}

/*! \brief Release any dead ACL instances. */
void dm_acl_cleanup(void)
{
    DM_ACL_T **pp_acl, *p_acl;

    for (pp_acl = &p_acl_list; (p_acl = *pp_acl) != NULL; /* empty */)
    {
        if (p_acl->connection_state == ACL_ST_IDLE)
        {
            DM_ACL_DEBUG_CHECK_ALLOW_DEAD(p_acl);

            timer_cancel(&p_acl->disconnect_timer_id);

            /* Remove entry from list */
            *pp_acl = p_acl->p_next;
#ifdef INSTALL_ULP
            if(NULL!=p_acl->ble_conn_params)
                CsrPmemFree(p_acl->ble_conn_params);
#endif /* INSTALL_ULP */
            CsrPmemFree(p_acl);
        }
        else
            pp_acl = &p_acl->p_next;
    }
}

/*! \brief Find ACL record structure by Bluetooth address.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
DM_ACL_T *dm_acl_find_by_tbdaddr(const TYPED_BD_ADDR_T *const addrt)
{
    DM_ACL_T *p_acl;

    for (p_acl = p_acl_list; p_acl != NULL
            && !tbdaddr_eq(&p_acl->addrt, addrt); p_acl = p_acl->p_next)
        ;

    DM_ACL_DEBUG_CHECK(p_acl);
    return p_acl;
}

/*! \brief Find ACL record structure by Legacy Bluetooth address.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
DM_ACL_T *dm_acl_find_by_bd_addr(const BD_ADDR_T *const p_bd_addr)
{
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, p_bd_addr);
    return dm_acl_find_by_tbdaddr(&addrt);
}

/*! \brief Find ACL record structure by Bluetooth address (connected only).

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns Pointer to ACL record structure, or NULL if not found.
*/
DM_ACL_T *dm_acl_find_connected_by_bd_addr(const BD_ADDR_T *const p_bd_addr)
{
    DM_ACL_T *p_acl = dm_acl_find_by_bd_addr(p_bd_addr);

    /* Don't return records with invalid handles. */
    if (p_acl != NULL && p_acl->handle == HCI_HANDLE_INVALID)
        p_acl = NULL;

    DM_ACL_DEBUG_CHECK(p_acl);
    return p_acl;
}

/*! \brief Find ACL record structure by handle.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns Pointer to ACL record structure, or NULL if not found.
*/
DM_ACL_T *dm_acl_find_by_handle(hci_connection_handle_t handle)
{
    DM_ACL_T *p_acl;

    handle &= HCI_CONNECTION_HANDLE_MASK;

    for (p_acl = p_acl_list; p_acl != NULL && p_acl->handle != handle;
            p_acl = p_acl->p_next)
        ;

    DM_ACL_DEBUG_CHECK(p_acl);
    return p_acl;
}

/*! \brief Searches for a Class Of Device in the ACL instance list.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns The Class Of Device if found and valid, 0 otherwise.
*/
CsrUint24 dm_acl_get_cod_by_bd_addr(const BD_ADDR_T *const p_bd_addr)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL
            && (p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
        return p_acl->dev_class;

    return 0;
}

/*! \brief Converts an HCI connection handle into a Bluetooth address.

    \param handle HCI connection handle.
    \param p_bd_addr Pointer to which Bluetooth address will be copied.
*/
void dm_acl_get_bd_addr_by_handle(BD_ADDR_T *const p_bd_addr,
                                  const hci_connection_handle_t handle)
{
    DM_ACL_T *p_acl;

    if ((p_acl = dm_acl_find_by_handle(handle)) != NULL)
        (void)tbdaddr_copy_to_bd_addr(p_bd_addr, &p_acl->addrt);
}

#ifdef INSTALL_ULP
DM_ACL_T *dm_acl_find_white_list_or_undirected(void)
{
    DM_ACL_T *p_acl;
    CsrUint16 flags;

    for (p_acl = p_acl_list; p_acl != NULL ; p_acl = p_acl->p_next)
    {
        flags = p_acl->flags & DM_ACL_CONNECTION_MASK;
        if ( flags == DM_ACL_CONNECTION_LE_MASTER_WHITELIST ||
             flags == DM_ACL_CONNECTION_LE_SLAVE_WHITELIST  ||
             flags == DM_ACL_CONNECTION_LE_SLAVE_UNDIRECTED  )
             break;
    }

    DM_ACL_DEBUG_CHECK(p_acl);
    return p_acl;
}

#endif



/*! \brief Do ACL opened callbacks.

    \param p_acl Pointer to ACL record structure.
    \param status Indication of success or failure.
*/
void dm_acl_callback_opened(DM_ACL_T *const p_acl, CsrUint8 status)
{
    DM_ACL_CLIENT_T client;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* xapncc ignore '=': cast of 'int' to differing enum */
    for (client = dm_acl_client_first; client != dm_acl_client_max; ++client)
        if (dm_acl_callback_function_opened[client] != NULL)
            dm_acl_callback_function_opened[client](p_acl, status);

    DM_ACL_DEBUG_CHECK(p_acl);
}

/*! \brief Do ACL closed callbacks.

    We only send closed messages to those clients with zero locks.
    Non-zero locks indicates that there's been an ACL open request
    from that client that is about to bring the ACL back up again.

    If there are no locks left then we reset the ACL structure
    (which involves zeroing the client parts of it).

    \param p_acl Pointer to ACL record structure.
    \param reason Reason for ACL being closed.
    \returns credits returned.
*/
CsrUint16 dm_acl_callback_closed(DM_ACL_T *const p_acl, CsrUint16 reason)
{
    DM_ACL_CLIENT_T client;
    CsrUint16 credits = 0;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* xapncc ignore '=': cast of 'int' to differing enum */
    for (client = dm_acl_client_first; client != dm_acl_client_max; ++client)
    {
        CsrUint16 r = reason;

        if (dm_acl_client_locks(p_acl, client) != 0)
            r |= DM_ACL_REASON_RECONNECTING_AND_LOCKED;

        if (dm_acl_callback_function_closed[client] != NULL)
            credits += dm_acl_callback_function_closed[client](p_acl, r);
    }

    if (p_acl->locks == 0)
        dm_acl_reset_acl(p_acl);

    return credits;
}

dm_acl_client_locks_t dm_acl_unlock(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    if (client < dm_acl_client_max)
    {
        /* Suppress the xapncc warning because call to dm_acl_client_locks()
           will return zero if 1 << (client << 2) wraps around. */
        if (dm_acl_client_locks(p_acl, client) != 0)
            /* xapncc ignore lower precision in wider context: '<<' */
            p_acl->locks -= 1 << (client << 2);
    }
    else
        p_acl->locks = 0;

    return p_acl->locks;
}

dm_acl_client_locks_t dm_acl_client_locks(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client)
{
    dm_acl_client_locks_t locks = p_acl->locks;

    /* If a client is specified then mask out other clients. */
    if (client < dm_acl_client_max)
        locks = (locks >> (client << 2)) & 0xF;

    return locks;
}

void dm_acl_lock(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* Suppress the xapncc warning because only the policy manager client
       could cause a wrap around and the policy manager never locks the ACL */
    if (client < dm_acl_client_max && dm_acl_client_locks(p_acl, client) < 0xF)
        /* xapncc ignore lower precision in wider context: '<<' */
        p_acl->locks += 1 << (client << 2);
    else
        BLUESTACK_PANIC(CSR_BT_PANIC_DM_ACL_LOCKS_EXHAUSED);
}

void dm_acl_send_to_hci(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code)
{
    hci_uprim->hci_cmd.op_code = op_code;
    hci_uprim->hci_cmd.length = DM_HCI_SENDER_ACL_MANAGER;

    send_to_hci((DM_UPRIM_T*)hci_uprim);
}

void dm_acl_send_to_hci_with_handle(HCI_UPRIM_T *hci_uprim, hci_op_code_t op_code)
{
    hci_uprim->hci_cmd.op_code = op_code;
    hci_uprim->hci_cmd.length = DM_HCI_SENDER_ACL_MANAGER;

    send_to_hci_with_handle((DM_UPRIM_T*)hci_uprim);
}

CsrBool dm_acl_exists(DM_ACL_T *p_acl)
{
    DM_ACL_T *pa;

    for (pa = p_acl_list; pa != NULL; pa = pa->p_next)
    {
        if (pa == p_acl)
        {
            DM_ACL_DEBUG_CHECK(p_acl);
            return TRUE;
        }
    }
    return FALSE;
}

#ifdef INSTALL_ULP
CsrBool dm_acl_is_ble(const DM_ACL_T *p_acl)
{
    return DM_ACL_GET_CONNECTION_FLAGS(p_acl) != DM_ACL_CONNECTION_BR_EDR
        && DM_ACL_GET_CONNECTION_FLAGS(p_acl) != DM_ACL_CONNECTION_BR_EDR_BROADCAST;
}
#endif
