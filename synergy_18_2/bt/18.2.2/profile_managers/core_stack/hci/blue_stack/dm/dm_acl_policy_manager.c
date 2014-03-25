/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              dm_acl_policy_manager.c

DESCRIPTION:       The ACL policy management entity of the
                   BlueStack Device Manager.



****************************************************************************/

#include "csr_synergy.h"


#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "l2cap_prim.h"
#include "dm_prim.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#else
#include "buffer.h"
#include "hostio.h"
#include "vm_const.h"
#endif

#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_common.h"
#include "tbdaddr.h"

#include "dm_layer_manager.h"
#include "dm_acl_core.h"
#include "dm_acl_policy_manager.h"
#include "dm_acl_manager.h"
#include "dm_mgt_interface.h"
#include "dm_hci_interface.h"
#include "dm_sync_manager.h"
#include "dm_acl_policy_types.h"
#include "l2cap_con_handle.h"
#include "dm_acl_powerstates.h"

#ifndef DISABLE_DM_BREDR

/*
 * The number of times the policy manager will retry a failed mode change
 * before giving up.
 */
#define MODE_CHANGE_RETRIES (3)

/* The DM sometimes uses the top bit of link_policy_settings_t for role
   switch policy, so we mask it off before sending to HCI. */
#define HCI_LP_MASK 0x7FFF

/* Signal the desired type of mode change holdoff. */
/* - No further holdoff required - just CsrSchedTidy up. */
#define DM_MODE_CHANGE_HOLDOFF_COMPLETE         0x0000
/* - Final failure holdoff. */
#define DM_MODE_CHANGE_HOLDOFF_FAILURE_FINAL    0x0001
/* - Initial failure holdoff. To be followed by the final one
     when it expires. The first holdoff is just for 1/10 of a
     second and only after it finishes do we allow the policy
     manager to operate. */
#define DM_MODE_CHANGE_HOLDOFF_FAILURE_INITIAL  0x0002
/* - Holdoff after a remote initiated mode change. */
#define DM_MODE_CHANGE_HOLDOFF_REMOTE           0x0003
/* - Indication that this holdoff request comes from a timeout */
#define DM_MODE_CHANGE_HOLDOFF_TIMEOUT          0x8000

#define DM_BLACK_LIST_MASK                      0x0003

/* Single linked list of Bluetooth addresses. */
typedef struct DM_ACL_BD_ADDR_LIST_T_tag
{
    struct DM_ACL_BD_ADDR_LIST_T_tag *p_next;
    BD_ADDR_T bd_addr;
} DM_ACL_BD_ADDR_LIST_T;

/*! Roleswitch policy table for switches during ACL connection setup */
static CsrUint16 *p_roleswitch_policy;

/* Linked list of remote devices that don't play nicely with roleswitch. */
static DM_ACL_BD_ADDR_LIST_T *p_naughty_devices;

/*! Default link policy settings */
static link_policy_settings_t default_lp_in = 0;
static link_policy_settings_t default_lp_out = 0;

/*! Private Function Prototypes */
static void policy_role_switch(DM_ACL_T *const p_acl);
static void policy_default(DM_ACL_T *const p_acl);

static CsrBool dm_acl_manual_mode_change(const BD_ADDR_T *const p_bd_addr);
static void send_hci_park_mode(const BD_ADDR_T *const p_bd_addr,
        const CsrUint16 max_interval, const CsrUint16 min_interval);
static void send_hci_exit_park_mode(const BD_ADDR_T *const p_bd_addr);
static void send_hci_exit_sniff_mode(const BD_ADDR_T *const p_bd_addr);
static void send_hci_write_lp_settings(const BD_ADDR_T *const p_bd_addr,
        const link_policy_settings_t link_policy_settings,
        const CsrUint8 owner);
static void mode_change_holdoff_timeout(CsrUint16 type, void *pv_acl);
static CsrUint8 dm_acl_connections(CsrUint8 condition);
static CsrUint8 dm_acl_read_roleswitch_action(CsrUint16 policy, CsrBool incoming);
static CsrBool dm_acl_conditions_match(CsrUint16 policy);
static void dm_acl_empty_roleswitch_naughty_device_list(void);
static DM_ACL_BD_ADDR_LIST_T **dm_acl_find_naughty_device(
        const BD_ADDR_T *const p_bd_addr);

#ifdef INSTALL_BD_ADDR_BLACK_LIST_SUPPORT

void send_dm_handle_bdaddr_black_list_cfm(const CsrUint8 status,
        const TYPED_BD_ADDR_T * const bd_addr);
static CsrUint8 dm_add_device_in_black_list(const BD_ADDR_T * p_bd_addr);
static void dm_remove_device_from_black_list(const BD_ADDR_T * p_bd_addr);
#endif


/*! \brief  Set the default link policy to be applied to all new connections.

    This does not affect existing connections.

    \param  p_uprim Pointer to primitive.
    \returns TRUE
*/
void dm_acl_set_default_link_policy(const DM_UPRIM_T *const p_uprim)
{
    HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T *p_hci_prim;

    default_lp_in = p_uprim->dm_set_default_link_policy_req.link_policy_settings_in;
    default_lp_out =
        p_uprim->dm_set_default_link_policy_req.link_policy_settings_out;


    p_hci_prim = pnew(HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS_T);
    p_hci_prim->default_lps = default_lp_out;
    p_hci_prim->common.op_code = HCI_WRITE_DEFAULT_LINK_POLICY_SETTINGS;
    p_hci_prim->common.length = DM_HCI_SENDER_APPLICATION;

    /* set default link policies at link manager */
    send_to_hci((DM_UPRIM_T*)p_hci_prim);
}

CsrUint16 dm_policy_acl_closed(DM_ACL_T *p_acl, CsrUint16 reason)
{
    /* Pending role-switch requests are now doomed. */
    if ((p_acl->flags & DM_ACL_ROLE_SWITCH_PENDING) != 0)
        dm_acl_hci_role_change(&TBDADDR_ADDR(p_acl->addrt), HCI_ERROR_NO_CONNECTION, 0);

    dm_acl_policy_reset(p_acl);
    return 0;
}

void dm_policy_acl_opened(DM_ACL_T *p_acl, CsrUint8 status)
{

    link_policy_settings_t default_hci_lp;

    DM_ACL_DEBUG_CHECK(p_acl);

    if (status == HCI_SUCCESS &&
       ( (p_acl->flags & DM_ACL_CONNECTION_MASK) == DM_ACL_CONNECTION_BR_EDR ))
    {
        default_hci_lp = default_lp_out;
        if ((p_acl->flags & DM_ACL_INITIATOR) != DM_ACL_INIT_LOCAL)
            default_hci_lp = default_lp_in;

        /* TODO do we really not let people disable all LM modes? */
        if (default_hci_lp != DISABLE_ALL_LM_MODES)
        {
            /* cache the policy for later */
            p_acl->dm_acl_client_policy.current_link_policy = p_acl->dm_acl_client_policy.link_policy = default_hci_lp;
            send_hci_write_lp_settings(&TBDADDR_ADDR(p_acl->addrt), default_hci_lp, DM_HCI_SENDER_ACL_MANAGER);
        }
    }
}

void dm_acl_policy_manager_reset(DM_ACL_T *p_acl)
{
    p_acl->dm_acl_client_policy.mode_change_retries = MODE_CHANGE_RETRIES;
    kick_policy_manager(p_acl);
}

/*! \brief Called to process a DM_LP_WRITE_ROLESWITCH_POLICY_REQ primitive.
    Called to process a DM_LP_WRITE_ROLESWITCH_POLICY_REQ primitive.
    It checks the request is legal and if so stores it, freeing any
    existing table. Illegal tables are freed and rejected. Empty
    tables cause any existing table to be deleted.

    \param p_uprim Pointer to downstream request primitive.
*/
void dm_acl_lp_write_roleswitch_policy_req(const DM_UPRIM_T *const p_uprim)
{
    DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T *p_cfm_prim;
    CsrUint8 status = DM_LP_WRITE_ROLESWITCH_POLICY_SUCCESS;
    CsrUint16 *p_new_table = p_uprim->dm_lp_write_roleswitch_policy_req.rs_table;

    if (p_uprim->dm_lp_write_roleswitch_policy_req.version != 0)
    {
        /* Unsupported version. */
        status = DM_LP_WRITE_ROLESWITCH_POLICY_UNSUPPORTED;
    }
    else if (p_uprim->dm_lp_write_roleswitch_policy_req.length != 0)
    {
        /* We must validate non-zero length tables. */
        CsrUint16 final, i;

        /* Index of final entry. */
        final = p_uprim->dm_lp_write_roleswitch_policy_req.length - 1;

        /* Validate table up to, but not including, last entry. */
        for (i = 0; i != final; ++i)
        {
            CsrUint16 policy = vm_const_fetch(p_new_table + i);

            /* Must not set any reserved bits. */
            if ((policy & DM_LP_ROLESWITCH_RESERVED_MASK) != 0)
            {
                status = DM_LP_WRITE_ROLESWITCH_POLICY_UNSUPPORTED;
                break;
            }

            /* Only last entry is allowed to be unconditional. */
            if ((policy & DM_LP_ROLESWITCH_CONDITION_MASK) == 0)
            {
                status = DM_LP_WRITE_ROLESWITCH_POLICY_ILLEGAL;
                break;
            }
        }

        /* Make sure table is zero-terminated. */
        if (vm_const_fetch(p_new_table + final) != 0)
            status = DM_LP_WRITE_ROLESWITCH_POLICY_NOT_ZERO_TERMINATED;
    }

    /* Replace policy on success, or free new table on failure. */
    if (status == DM_LP_WRITE_ROLESWITCH_POLICY_SUCCESS)
    {
        CsrUint16 *tmp = p_roleswitch_policy;
        p_roleswitch_policy = p_new_table;
        p_new_table = tmp;
    }

    bpfree(p_new_table);

    /* Send response to application. */
    p_cfm_prim = pnew(DM_LP_WRITE_ROLESWITCH_POLICY_CFM_T);
    p_cfm_prim->type = DM_LP_WRITE_ROLESWITCH_POLICY_CFM;
    p_cfm_prim->status = status;
    DM_SendMessageAMphandle(p_cfm_prim);
}

/*! \brief Process a DM_LP_WRITE_ALWAYS_MASTER_DEVICES_REQ primitive.
           It checks the request is legal and if so processes it, either
           adding to, deleting from, or clearing the list.
    \param p_uprim Pointer to request primitive.
*/

void dm_acl_lp_write_always_master_devices_req(const DM_UPRIM_T *const p_uprim)
{
    DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T *p_cfm_prim;
    DM_ACL_BD_ADDR_LIST_T **pp_dev, *p_dev;
    const BD_ADDR_T *const p_bd_addr =
        &p_uprim->dm_lp_write_always_master_devices_req.bd_addr;

    /* See if device is already in the list */
    p_dev = *(pp_dev = dm_acl_find_naughty_device(p_bd_addr));

    /* Create response */
    p_cfm_prim = pnew(DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM_T);
    p_cfm_prim->type = DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CFM;
    p_cfm_prim->phandle = am_phandle;
    p_cfm_prim->status = DM_LP_WRITE_ALWAYS_MASTER_DEVICES_SUCCESS;

    /* Process requested operation */
    switch (p_uprim->dm_lp_write_always_master_devices_req.operation)
    {
        case DM_LP_WRITE_ALWAYS_MASTER_DEVICES_CLEAR:
            dm_acl_empty_roleswitch_naughty_device_list();
            break;

        case DM_LP_WRITE_ALWAYS_MASTER_DEVICES_ADD:
            if (p_dev == NULL)
            {
                /* Add to end of list. */
                p_dev = *pp_dev = zpnew(DM_ACL_BD_ADDR_LIST_T);
                bd_addr_copy(&p_dev->bd_addr, p_bd_addr);
            }
            break;

        case DM_LP_WRITE_ALWAYS_MASTER_DEVICES_DELETE:
            if (p_dev != NULL)
            {
                /* Remove entry from device list. */
                *pp_dev = p_dev->p_next;
                CsrPmemFree(p_dev);
            }
            break;

        default:
            p_cfm_prim->status = DM_LP_WRITE_ALWAYS_MASTER_DEVICES_ILLEGAL;
            break;
    }

    /* Send response to application */
    DM_SendMessage(p_cfm_prim);
}

/*! \brief Destroy list of badly behaving devices */
static void dm_acl_empty_roleswitch_naughty_device_list(void)
{
    while (p_naughty_devices != NULL)
    {
        DM_ACL_BD_ADDR_LIST_T *tmp = p_naughty_devices;
        p_naughty_devices = p_naughty_devices->p_next;
        CsrPmemFree(tmp);
    }
}

/*! \brief Find given device in list of badly behaving devices.

    \param p_bd_addr Pointer to Bluetooth address of device to find.
    \returns Double indirected element pointer (if found) or double
             indirected pointer to end-of-list-NULL (if not found).
*/
static DM_ACL_BD_ADDR_LIST_T **dm_acl_find_naughty_device(
        const BD_ADDR_T *const p_bd_addr
        )
{
    DM_ACL_BD_ADDR_LIST_T **pp_dev, *p_dev;

    for (pp_dev = &p_naughty_devices;
        (p_dev = *pp_dev) != NULL && !bd_addr_eq(p_bd_addr, &p_dev->bd_addr);
        pp_dev = &p_dev->p_next)
        ;

    return pp_dev;
}

/*! \brief Count the number of connected ACLs matching the given condition.

    \param condition To be counted, an ACL must satisfy the given condition.
    \returns Number of ACLs matching the condition.
*/
static CsrUint8 dm_acl_connections(CsrUint8 condition)
{
    DM_ACL_T *p_acl;
    CsrUint8 count = 0;

    for(p_acl = p_acl_list; p_acl != NULL; p_acl = p_acl->p_next)
    {
        /* Only count ACLs that are connected. */
        if (dm_acl_is_connected(p_acl))
        {
            /* Role discovery. */
            CsrBool master = FALSE;
            if ((p_acl->flags & DM_ACL_ROLE) == DM_ACL_ROLE_MASTER)
                master = TRUE;

            /* Increment count if conditions matched. */
            switch (condition)
            {
                case DM_ACL_NUM_SLAVES:
                    if (master)
                        ++count;
                    break;

                case DM_ACL_NUM_UNSNIFFED_MASTERS:
                    if (p_acl->current_mode == HCI_BT_MODE_SNIFF)
                        break;

                    /* Fallthrough */
                case DM_ACL_NUM_MASTERS:
                    if (master)
                        break;

                    /* Fallthrough */
                case DM_ACL_NUM_CONNECTIONS:
                    ++count;
                    break;
            }
        }
    }

    return count;
}

/*! \brief Generate value for 'allow_role_switch' or 'role' fields in
    HCI_CREATE_CONNECTION or HCI_ACCEPT_CONNECTION_REQ primitives,
    respectively.

    \param policy Roleswitch policy.
    \param incoming TRUE implies HCI_ACCEPT_CONNECTION_REQ, FALSE implies
    HCI_CREATE_CONNECTION.
    \returns Value for appropriate 'role' or 'allow_role_switch' field.
*/
static CsrUint8 dm_acl_read_roleswitch_action(CsrUint16 policy, CsrBool incoming)
{
    if (incoming)
    {
        if ((policy & DM_LP_ROLESWITCH_ACTION_INCOMING_RS) != 0)
            return HCI_ROLE_BECOME_MASTER;

        return HCI_ROLE_STAY_SLAVE;
    }

    if ((policy & DM_LP_ROLESWITCH_ACTION_OUTGOING_NO_RS) != 0)
        return HCI_DO_NOT_ALLOW_ROLE_SWITCH;

    return HCI_ALLOW_ROLE_SWITCH;
}

/*! \brief Determine whether the quantity and state of all connected ACLs
    matches the conditions specified in the given policy.

    \param policy Roleswitch policy.
    \returns TRUE if current state matches policy, otherwise FALSE.
*/
static CsrBool dm_acl_conditions_match(CsrUint16 policy)
{
    CsrUint8 condition;

    for(condition = 0; condition < DM_ACL_NUM_END; ++condition)
    {
        if (dm_acl_connections(condition) <
                DM_LP_ROLESWITCH_READ_CONDITION(policy, condition))
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*!\brief Determine role-switch policy for new pending connection.
   If there is a policy table then we walk down it until current
   conditons match and read then read the actions specified.
   Otherwise we revert to default (legacy) behaviour, which is
   to allow role-switch on outgoing connections and only to request
   a role-switch on an incoming connection if there is an existing
   connection to another device.

   \param incoming TRUE for an incoming connection, otherwise FALSE.
   \returns Value for appropriate 'role' or 'allow_role_switch' field.
*/
CsrUint8 dm_acl_role_policy(CsrBool incoming, const BD_ADDR_T *const p_bd_addr)
{
    CsrUint16 policy;

    if (!LYMCB_IS_FLAG_SET(LYM_Role_Switch_Supported))
        return incoming ? HCI_ROLE_STAY_SLAVE : HCI_DO_NOT_ALLOW_ROLE_SWITCH;

    /* If device in naughty list then always try to become master. */
    if (*dm_acl_find_naughty_device(p_bd_addr) != NULL)
        return incoming ? HCI_ROLE_BECOME_MASTER : HCI_DO_NOT_ALLOW_ROLE_SWITCH;

    if (p_roleswitch_policy != NULL)
    {
        CsrUint16 *p_policy;

        /* Role-switch table present, so walk down until we get a match.
           We've already made sure that the table is zero-terminated,
           so an eventual match is guaranteed. */
        for (p_policy = p_roleswitch_policy; /* empty */; ++p_policy)
        {
            policy = vm_const_fetch(p_policy);
            if (dm_acl_conditions_match(policy))
                break;
        }
    }
    else
    {
        /* Preserve old behaviour as default. */
        policy = 0;
        if (dm_acl_connections(DM_ACL_NUM_CONNECTIONS) != 0)
            policy = DM_LP_ROLESWITCH_ACTION_INCOMING_RS;
    }

    return dm_acl_read_roleswitch_action(policy, incoming);
}

/*! \brief Handle a request to perform a role switch.

    Validate that there is an ACL in the connected state - if not, issues
    a negative mode change event.

    \param  p_uprim Pointer to primitive.
    \returns TRUE
*/
void dm_acl_switch_role(const DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    CsrUint8 error = HCI_ERROR_NO_CONNECTION;
    const BD_ADDR_T *const p_bd_addr = &p_uprim->dm_switch_role_req.bd_addr;

    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
    {
        if ((p_acl->flags & DM_ACL_ROLE_SWITCH_PENDING) == 0)
        {
            /* Role switch will fail if the link is in sniff, so we queue up
               the request and let the policy manager decide when to send it. */
            p_acl->flags |= p_uprim->dm_switch_role_req.role == HCI_MASTER ?
                DM_ACL_ROLE_SWITCH_MASTER : DM_ACL_ROLE_SWITCH_SLAVE;

            dm_acl_set_active_mode(p_acl, dm_acl_client_policy);
            return;
        }

        /* Can't deal with more than one roleswitch at a time. */
        error = HCI_ERROR_ROLE_SWITCH_PENDING;
    }

    /* Failure */
    Send_DM_Switch_Role_Complete(error, p_bd_addr, 0);
}

/*! \brief Find out if a manual mode change can proceed and make preparations.

    Mode change requests are only allowed if the DM link policy
    is set to DEFAULT and there's not one in progress.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns Pointer to ACL record structure, or NULL on failure.
*/
static CsrBool dm_acl_manual_mode_change(const BD_ADDR_T *const p_bd_addr)
{
    DM_ACL_T *p_acl;
    hci_error_t status = HCI_ERROR_NO_CONNECTION;
    hci_bt_mode_t current_mode = 0;

    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
    {
        /* Mode change requests are rejected if the DM is in one of its
           pro-active management modes or a mode change is currently in
           progress. Inform the application of the current mode. */
        if ((p_acl->flags & DM_ACL_MODE_CHANGE_REQUESTED) == 0
                && !DM_ACL_IS_USING_POWERSTATES(p_acl)
                && p_acl->dm_acl_client_policy.active == 0)
        {
            p_acl->flags |= DM_ACL_MODE_CHANGE_REQUESTED;
            return TRUE;
        }

        status = HCI_ERROR_COMMAND_DISALLOWED;
        current_mode = p_acl->current_mode;
    }

    Send_DM_Mode_Change_Event(status, p_bd_addr, current_mode, 0);
    return FALSE;
}

/*! \brief  Handle a request to enter sniff mode.
    \param  p_uprim Pointer to primitive.
*/
void dm_acl_sniff_mode(const DM_UPRIM_T *const p_uprim)
{
    const BD_ADDR_T *const p_bd_addr = &p_uprim->dm_sniff_mode_req.bd_addr;

    if (dm_acl_manual_mode_change(p_bd_addr))
        send_hci_sniff_mode(p_bd_addr,
                p_uprim->dm_sniff_mode_req.max_interval,
                p_uprim->dm_sniff_mode_req.min_interval,
                p_uprim->dm_sniff_mode_req.attempt,
                p_uprim->dm_sniff_mode_req.timeout);
}

/*! \brief  Handle a request to exit sniff mode.
    \param  p_uprim Pointer to primitive.
*/
void dm_acl_exit_sniff_mode(const DM_UPRIM_T *const p_uprim)
{
    const BD_ADDR_T *const p_bd_addr = &p_uprim->dm_exit_sniff_mode_req.bd_addr;

    if (dm_acl_manual_mode_change(p_bd_addr))
        send_hci_exit_sniff_mode(p_bd_addr);
}

/*! \brief  Handle a request to enter park mode.
    \param  p_uprim Pointer to primitive.
*/
void dm_acl_park_mode(const DM_UPRIM_T *const p_uprim)
{
    const BD_ADDR_T *const p_bd_addr = &p_uprim->dm_park_mode_req.bd_addr;

    if (dm_acl_manual_mode_change(p_bd_addr))
        send_hci_park_mode(p_bd_addr, p_uprim->dm_park_mode_req.max_interval,
                p_uprim->dm_park_mode_req.min_interval);
}

/*! \brief  Handle a request to exit park mode.
    \param  p_uprim Pointer to primitive.
*/
void dm_acl_exit_park_mode(const DM_UPRIM_T *const p_uprim)
{
    const BD_ADDR_T *const p_bd_addr = &p_uprim->dm_exit_park_mode_req.bd_addr;

    if (dm_acl_manual_mode_change(p_bd_addr))
        send_hci_exit_park_mode(p_bd_addr);
}

/*! \brief Handle a request to set the link policy for an ACL link.

    The LM link policy settings are sent over HCI, and the DM settings
    are stored in the ACL data structure.

    \param  p_uprim Pointer to primitive.
*/
void dm_acl_write_lp_settings(const DM_UPRIM_T *const p_uprim)
{
    link_policy_settings_t link_policy =
        p_uprim->dm_write_link_policy_settings_req.link_policy_settings;
    const BD_ADDR_T *const p_bd_addr =
        &p_uprim->dm_write_link_policy_settings_req.bd_addr;
    DM_ACL_T *p_acl;

    /* Write LM settings. */
    send_hci_write_lp_settings(p_bd_addr, link_policy, DM_HCI_SENDER_ACL_MANAGER|DM_HCI_SENDER_APPLICATION);

    /* Store settings. */
    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
    {
        /* Write LM settings. */
        p_acl->dm_acl_client_policy.current_link_policy = p_acl->dm_acl_client_policy.link_policy = link_policy;

        /* The policy manager will decide what to do next */
        kick_policy_manager(p_acl);
    }
}


/*! \brief Handle role change event

    If status != HCI_SUCCESS and our role is unknown then this function
    should be called with the 'role' argument set to HCI_MASTER.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param role Our role (HCI_MASTER or HCI_SLAVE).
    \param status Indication of success or failure.
*/
void dm_acl_hci_role_change(const BD_ADDR_T *const p_bd_addr,
                            const hci_return_t status,
                            hci_role_t role)
{
    DM_ACL_T *p_acl;

    /* Role change can happen before outgoing connection has completed,
     * so also look for an ACL instance with no HCI handle yet.
     */
    if ((p_acl = dm_acl_find_by_bd_addr(p_bd_addr)) != NULL)
    {
        /* Role-switch no longer in progress. */
        if ((p_acl->flags & DM_ACL_ROLE_SWITCH_PENDING) == DM_ACL_ROLE_SWITCH_PENDING)
        {
            p_acl->flags &= ~DM_ACL_ROLE_SWITCH_PENDING;
            dm_acl_clear_active_mode(p_acl, dm_acl_client_policy);
        }

        if (status == HCI_SUCCESS)
        {
            p_acl->flags &= ~DM_ACL_ROLE_SLAVE;
            if (role != HCI_MASTER)
                p_acl->flags |= DM_ACL_ROLE_SLAVE;

        /* After a successful rolechange, we reset our retries
           as it many now be possible to retry. */
            dm_acl_policy_manager_reset(p_acl);
        }
        else
        {
            role = HCI_MASTER;
            if ((p_acl->flags & DM_ACL_ROLE) != DM_ACL_ROLE_MASTER)
                role = HCI_SLAVE;
        }
    }

    Send_DM_Switch_Role_Complete(status, p_bd_addr, role);
}

/*! \brief Callback from the HCI interface whenever some upstream
    or downstream ACL traffic is seen.

    The purpose of this function is to return us to the top
    state in our powerstate table, or to take us out of park mode
    if we're not using powerstate tables and happen to find
    ourselves parked.

    Powerstate tables have a resolution of 1 second, so there's
    no point in trying to reset them much more often than that.

    \param p_acl Pointer to ACL record structure.
*/
void acl_activity(DM_ACL_T *p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* Remove scenarios where we don't need to bother the policy manager. */
#ifdef INSTALL_DM_POWERSTATES_MODULE
    if (DM_ACL_IS_USING_POWERSTATES(p_acl))
    {
        if (!dm_acl_powerstates_activity(p_acl))
            return;
    }
    else
#endif
    {
        if (p_acl->current_mode != HCI_BT_MODE_PARK)
            return;
    }

    /* The policy manager will decide what to do next */
    kick_policy_manager(p_acl);
}

/*! \brief Handle HCI mode change event.

    \param p_acl Pointer to ACL record structure.
    \param status Indication of success or failure.
    \param current_mode
    \param interval
*/
void dm_acl_hci_mode_change(DM_ACL_T *const p_acl,
                            const hci_return_t status,
                            const hci_bt_mode_t current_mode,
                            const CsrUint16 interval)
{
    CsrUint16 type = DM_MODE_CHANGE_HOLDOFF_COMPLETE;
    p_acl->current_mode = current_mode;

    /* Issue mode change events - keep clients informed */
    Send_DM_Mode_Change_Event(status, &TBDADDR_ADDR(p_acl->addrt), current_mode, interval);
#ifdef INSTALL_AMP_SUPPORT
    dm_amp_acl_mode_changed(p_acl, status, interval);
#endif

    if (status == HCI_SUCCESS)
    {
        /* reset mode change retries on success (either end initiated) */
        p_acl->dm_acl_client_policy.mode_change_retries = MODE_CHANGE_RETRIES;

        if ((p_acl->flags & DM_ACL_MODE_CHANGE_REQUESTED) == 0)
            type = DM_MODE_CHANGE_HOLDOFF_REMOTE;
    }
    else
    {
        type = DM_MODE_CHANGE_HOLDOFF_FAILURE_INITIAL;

        if (p_acl->dm_acl_client_policy.mode_change_retries != 0)
            --p_acl->dm_acl_client_policy.mode_change_retries;
    }

    p_acl->flags &= ~DM_ACL_MODE_CHANGE_REQUESTED;

    /* Start a mode change request hold-off timeout, to prevent rapid
       mode change requests if the baseband keeps rejecting.
       This will kick the policy manager. */
    mode_change_holdoff_timeout(type, p_acl);
}

/*! \brief Reset Policy Manager record structure to default values,
           free any pointers hanging off it and cancel all timers.
           It is necessary to call this when the ACL is opened
           or closed.

    \param p_acl Pointer to ACL record structure.
*/
void dm_acl_policy_reset(DM_ACL_T *p_acl)
{
    p_acl->current_mode = HCI_BT_MODE_ACTIVE;
    p_acl->flags &= ~DM_ACL_MODE_CHANGE_REQUESTED;
    p_acl->dm_acl_client_policy.active = 0;
    p_acl->dm_acl_client_policy.mode_change_retries = MODE_CHANGE_RETRIES;

    timer_cancel(&p_acl->dm_acl_client_policy.mode_change_holdoff_timer);

    dm_acl_client_deinit_policy_powerstates(&p_acl->dm_acl_client_policy);
}

void dm_acl_set_active_mode(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    p_acl->dm_acl_client_policy.active |= (1 << client);

    /* Update the policy settings to disable all LP modes. */
    dm_acl_set_link_policy(p_acl, DM_ACL_NO_LP_MASK);

    /* Start the process of trying to get back to active */
    kick_policy_manager(p_acl);
}

void dm_acl_clear_active_mode(DM_ACL_T *const p_acl, const DM_ACL_CLIENT_T client)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    p_acl->dm_acl_client_policy.active &= ~(1 << client);

    /* If we're no longer enforcing active mode, then go back to
       whatever policy we had before. */
    if (p_acl->dm_acl_client_policy.active == 0)
        dm_acl_policy_manager_reset(p_acl);
}

/*! \brief Handle DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_REQ primitive.

    \param p_uprim Pointer to DM primitive DM_UPRIM_T
*/
void dm_acl_change_conn_pkt_type_req(const DM_UPRIM_T *const p_uprim)
{
    DM_ACL_T *p_acl;
    DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T *p_prim = pnew(DM_HCI_CHANGE_CONN_PKT_TYPE_REQ_T);

    p_prim->common.op_code = DM_HCI_CHANGE_CONN_PKT_TYPE_REQ;
    p_prim->common.length = DM_HCI_SENDER_APPLICATION;

    p_prim->pkt_type = p_uprim->dm_handle_change_acl_conn_pkt_type_req.pkt_type;

    /* Check if we have a valid acl handle */
    if(NULL != (p_acl = dm_acl_find_by_bd_addr(&p_uprim->dm_handle_change_acl_conn_pkt_type_req.bd_addr)))
    {
        p_prim->handle = p_acl->handle;

        send_to_hci((DM_UPRIM_T *)p_prim);
    }
    else
    {
        /* No matching acl found, send an error */
        BD_ADDR_T bd_addr;
        bd_addr_copy(&bd_addr, &p_uprim->dm_handle_change_acl_conn_pkt_type_req.bd_addr);

        DM_SendBDADDRCommandComplete(DM_HCI_CHANGE_ACL_CONN_PKT_TYPE_CFM,
                HCI_ERROR_ILLEGAL_FORMAT, &bd_addr);

        CsrPmemFree(p_prim);
    }
}

/*! \brief Check if a mode change is needed and can proceed.

           You tell the function what mode you'd like and it works
           out how to get there. If it needs to exit an existing mode
           then it will do so.

    \param p_acl Pointer to ACL record sctructure.
    \param mode  The mode you'd like to change to.
    \parem state Pointer to powerstate structure, or NULL. Only relevant
                 for a request to enter SNIFF mode.
    \returns TRUE if the current mode is HCI_BT_MODE_ACTIVE and there is
             is not already a locally-initiated mode change in progress.
             FALSE otherwise. So TRUE means that we're ready to change to
             the right mode (unless that mode is active, in which case
             we're already there).
*/
CsrBool check_mode_change(DM_ACL_T *const p_acl,
        const hci_bt_mode_t mode, const LP_POWERSTATE_T *const state)
{
    void (*exit_mode)(const BD_ADDR_T*const);

    DM_ACL_DEBUG_CHECK(p_acl);

    /* Can't do anything if there's a mode change request in progress. */
    if ((p_acl->flags & DM_ACL_MODE_CHANGE_REQUESTED) != 0)
        return FALSE;

    switch (p_acl->current_mode)
    {
        case HCI_BT_MODE_SNIFF:
            /* See if the new mode is the same as the old one. */
            if (mode != HCI_BT_MODE_SNIFF
#ifdef INSTALL_DM_POWERSTATES_MODULE
                    /* When using powerstates, check to see if the new sniff is the same
                       as the old sniff. */
                    || state == NULL
                    || state->max_interval != p_acl->dm_acl_client_policy.sniff.max_interval
                    || state->min_interval != p_acl->dm_acl_client_policy.sniff.min_interval
                    || state->attempt != p_acl->dm_acl_client_policy.sniff.attempt
                    || state->timeout != p_acl->dm_acl_client_policy.sniff.timeout
#endif
                    )
            {
                exit_mode = send_hci_exit_sniff_mode;
                break;
            }

            return FALSE;

        case HCI_BT_MODE_PARK:
            /* See if the new mode is the same as the old one. */
            if (mode != HCI_BT_MODE_PARK)
            {
                exit_mode = send_hci_exit_park_mode;
                break;
            }

            /* Fallthrough */
        case HCI_BT_MODE_HOLD:
            /* Can't get out of HOLD mode. */
            return FALSE;

        default:
            /* We're ACTIVE. */
            return TRUE;
    }

    /* Need to exit the current mode */
    exit_mode(&TBDADDR_ADDR(p_acl->addrt));
    p_acl->flags |= DM_ACL_MODE_CHANGE_REQUESTED;

    return FALSE;
}

/*! \brief Deal with possible changes of link policy.

    \param p_acl Pointer to ACL record structure.
    \param link_policy Requested link policy.
*/
void dm_acl_set_link_policy(DM_ACL_T *const p_acl,
        const link_policy_settings_t mask)
{
    link_policy_settings_t link_policy;

    DM_ACL_DEBUG_CHECK(p_acl);

    link_policy = p_acl->dm_acl_client_policy.link_policy & mask;
    if (p_acl->dm_acl_client_policy.current_link_policy != link_policy)
    {
        p_acl->dm_acl_client_policy.current_link_policy = link_policy;
        send_hci_write_lp_settings(&TBDADDR_ADDR(p_acl->addrt), link_policy, DM_HCI_SENDER_ACL_MANAGER);
    }
}

/*! \brief There's a role switch pending.

    We return to ACTIVE mode and then start the role switch.
    we use a holdoff timer to keep us in active while we're doing it.

    \param p_acl Pointer to ACL record structure.
*/
static void policy_role_switch(DM_ACL_T *const p_acl)
{
    HCI_SWITCH_ROLE_T *p_prim;

    DM_ACL_DEBUG_CHECK(p_acl);

    if ((p_acl->flags & DM_ACL_ROLE_SWITCH_PENDING) != DM_ACL_ROLE_SWITCH_PENDING)
    {
        /* We've not already requested the switch, so do so now. */;
        p_prim = pnew(HCI_SWITCH_ROLE_T);

        bd_addr_copy(&p_prim->bd_addr, &TBDADDR_ADDR(p_acl->addrt));

        p_prim->role = (p_acl->flags & DM_ACL_ROLE_SWITCH_MASTER) == 0 ?
            HCI_SLAVE : HCI_MASTER;

        dm_acl_send_to_hci((HCI_UPRIM_T*)p_prim, HCI_SWITCH_ROLE);

        p_acl->flags |= DM_ACL_ROLE_SWITCH_PENDING;
    }
}

/*! \brief Deal with DM_LINK_POLICY_DEFAULT.

    We just need to unpark the ACL when necessary.

    \param p_acl Pointer to ACL record structure.
*/
static void policy_default(DM_ACL_T *const p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    /* Update the policy settings to enable all LP modes. */
    dm_acl_set_link_policy(p_acl, 0xFFFF);

    /* If we are in park mode and outgoing data is pending or we
       have a SCO, we need to exit now */
    if (p_acl->current_mode == HCI_BT_MODE_PARK)
    {
        if (CH_DataTxPending(&p_acl->dm_acl_client_l2cap)
#ifdef INSTALL_DM_SYNC_MODULE
                    || dm_sync_num_connections_acl(p_acl->handle) != 0
#endif
               )
        {
            send_hci_exit_park_mode(&TBDADDR_ADDR(p_acl->addrt));
            p_acl->flags |= DM_ACL_MODE_CHANGE_REQUESTED;
        }
    }
}

/*! \brief Kicks the policy management algorithm.

    \param p_acl Pointer to ACL record structure.
*/
void kick_policy_manager(DM_ACL_T *const p_acl)
{
    DM_ACL_DEBUG_CHECK(p_acl);

    if ((p_acl->flags & (DM_ACL_BLOCK_POLICY_MANAGER |
                    DM_ACL_MODE_CHANGE_REQUESTED)) == 0)
    {
        /* Decide which policy to follow. */
        if (p_acl->dm_acl_client_policy.active == 0)
        {
            /* Normal operation, so follow powerstates or default policy. */
            if (DM_ACL_IS_USING_POWERSTATES(p_acl))
                dm_acl_policy_powerstates(p_acl);
            else
                policy_default(p_acl);
        }
        else if (check_mode_change(p_acl, HCI_BT_MODE_ACTIVE, NULL))
        {
            /* We're in enforced active mode, so see if we can start
               whatever it is we're trying to accomplish. */
            if ((p_acl->flags & DM_ACL_ROLE_SWITCH_PENDING) != 0)
                policy_role_switch(p_acl);
#ifdef INSTALL_DM_SYNC_MODULE
            else if ((p_acl->flags & DM_ACL_SCO_PENDING) != 0)
                dm_sync_acl_ready(p_acl);
#endif
        }
    }
}

/*! \brief Request that we enter Park Mode.

    \param handle ACL handle of existing connection to remote device.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param max_interval
    \param min_interval
*/
static void send_hci_park_mode(const BD_ADDR_T *const p_bd_addr,
        const CsrUint16 max_interval, const CsrUint16 min_interval)
{
    HCI_PARK_MODE_T *p_prim;

    p_prim = pnew(HCI_PARK_MODE_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->max_interval = max_interval;
    p_prim->min_interval = min_interval;

    dm_acl_send_to_hci_with_handle((HCI_UPRIM_T*)p_prim, HCI_PARK_MODE);
}

/*! \brief Request that we exit Park Mode.

    \param handle ACL handle of existing connection to remote device.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_hci_exit_park_mode(const BD_ADDR_T *const p_bd_addr)
{
    HCI_EXIT_PARK_MODE_T *p_prim;

    p_prim = pnew(HCI_EXIT_PARK_MODE_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    dm_acl_send_to_hci_with_handle((HCI_UPRIM_T*)p_prim, HCI_EXIT_PARK_MODE);
}

/*! \brief Request that we enter Sniff Mode.

    \param handle ACL handle of existing connection to remote device.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param max_interval
    \param min_interval
    \param attempt
    \param timeout
*/
void send_hci_sniff_mode(const BD_ADDR_T *const p_bd_addr,
        const CsrUint16 max_interval, const CsrUint16 min_interval,
        const CsrUint16 attempt, const CsrUint16 timeout)
{
    HCI_SNIFF_MODE_T *p_prim;

    p_prim = pnew(HCI_SNIFF_MODE_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->max_interval = max_interval;
    p_prim->min_interval = min_interval;
    p_prim->attempt = attempt;
    p_prim->timeout = timeout;

    dm_acl_send_to_hci_with_handle((HCI_UPRIM_T*)p_prim, HCI_SNIFF_MODE);
}

/*! \brief Request that we exit Sniff Mode.

    \param handle ACL handle of existing connection to remote device.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
*/
static void send_hci_exit_sniff_mode(const BD_ADDR_T *const p_bd_addr)
{
    HCI_EXIT_SNIFF_MODE_T *p_prim;

    p_prim = pnew(HCI_EXIT_SNIFF_MODE_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    dm_acl_send_to_hci_with_handle((HCI_UPRIM_T*)p_prim, HCI_EXIT_SNIFF_MODE);
}

/*! \brief Write link policy settings.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param link_policy_settings New link policy settings to be written.
*/
static void send_hci_write_lp_settings(const BD_ADDR_T *const p_bd_addr,
        const link_policy_settings_t link_policy_settings,
        const CsrUint8 owner)
{
    HCI_WRITE_LINK_POLICY_SETTINGS_T *p_prim;

    p_prim = pnew(HCI_WRITE_LINK_POLICY_SETTINGS_T);
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->link_policy_settings = link_policy_settings;

    p_prim->common.op_code = HCI_WRITE_LINK_POLICY_SETTINGS|DM_HCI_WITH_HANDLE;
    p_prim->common.length = owner;
    send_to_hci_with_handle((DM_UPRIM_T*)p_prim);
}



/*! \brief Deal with mode change holdoff timer.

    This can be called either as a result of the timer firing, or directly
    as a result of a remote-initiated or failed locally-initiated mode change.

    We cancel/expire any old timer and then start a new one specified
    by the type. If the type is 'failure initial' then we block the
    policy manager until the timer fires. When it does fire, we unblock
    the policy manager and then automatically start the normal failed
    mode change timer (which only blocks the sniff mode powerstate).

    \param type The type of holdoff timer.
    \param pv_acl Pointer to ACL record structure cast to (void*).
*/
static void mode_change_holdoff_timeout(CsrUint16 type, void *pv_acl)
{
    DM_ACL_T *p_acl = (DM_ACL_T*)pv_acl;
    CsrTime holdoff;
    CsrUint16 next_type;

    DM_ACL_DEBUG_CHECK(p_acl);

    /* If we were called as the timer fired then expire it. */
    if ((type & DM_MODE_CHANGE_HOLDOFF_TIMEOUT) != 0)
        TIMER_EXPIRED(p_acl->dm_acl_client_policy.mode_change_holdoff_timer);

    /* We unblock the policy manager here and block it further down, but
       only if we're starting the very short initial holdoff timer. */
    p_acl->flags &= ~DM_ACL_BLOCK_POLICY_MANAGER;

    type &= ~DM_MODE_CHANGE_HOLDOFF_TIMEOUT;

    switch (type)
    {
        case DM_MODE_CHANGE_HOLDOFF_FAILURE_INITIAL:
            /* This is the very short initial holdoff timer. When this
               fires, it automatically starts the final timer. We block
               the policy manager completely for the duration. */
            holdoff = CSR_SCHED_MILLISECOND * 100;
            next_type = DM_MODE_CHANGE_HOLDOFF_FAILURE_FINAL
                    | DM_MODE_CHANGE_HOLDOFF_TIMEOUT;
            p_acl->flags |= DM_ACL_BLOCK_POLICY_MANAGER;
            break;

        case DM_MODE_CHANGE_HOLDOFF_REMOTE:
            /* Holdoff after a remotely initiated mode change has occured. */
            holdoff = DM_CONFIG_MODE_CHANGE_REMOTE_HOLDOFF_TIME;
            next_type = DM_MODE_CHANGE_HOLDOFF_COMPLETE |
                    DM_MODE_CHANGE_HOLDOFF_TIMEOUT;
            break;

        case DM_MODE_CHANGE_HOLDOFF_FAILURE_FINAL:
            /* This is the normal holdoff timer after a failed mode change.
               It doesn't block the policy manager completely. All it does
               is delay locally initiated mode changes away from active mode. */
            holdoff = DM_CONFIG_MODE_CHANGE_FAILED_HOLDOFF_TIME;
            next_type = DM_MODE_CHANGE_HOLDOFF_COMPLETE |
            DM_MODE_CHANGE_HOLDOFF_TIMEOUT;
            break;

        default:
            /* No holdoff requested. */
            kick_policy_manager(p_acl);
            return;
    }

    timer_start(&p_acl->dm_acl_client_policy.mode_change_holdoff_timer,
                holdoff,
                mode_change_holdoff_timeout,
                next_type,
                p_acl);

    kick_policy_manager(p_acl);
}

#ifdef BUILD_FOR_HOST
void dm_acl_policy_manager_init(void)
{
    default_lp_in = default_lp_out = 0;
    p_roleswitch_policy = NULL;
    p_naughty_devices = NULL;
}
#endif

#ifdef ENABLE_SHUTDOWN
void dm_acl_policy_manager_deinit(void)
{
    while (p_naughty_devices != NULL)
    {
        DM_ACL_BD_ADDR_LIST_T *tmp = p_naughty_devices;
        p_naughty_devices = p_naughty_devices->p_next;
        CsrPmemFree(tmp);
    }

    CsrPmemFree(p_roleswitch_policy);

    dm_acl_policy_manager_init();
}
#endif


#ifdef INSTALL_BD_ADDR_BLACK_LIST_SUPPORT

/*! Linked list of cached BD Address with which we don't want to connect */
static DM_ACL_BD_ADDR_LIST_T *p_bdaddr_black_list = NULL;

/*! \brief Build and send a DM_HANDLE_DEVICE_BLACK_LIST_CFM primitive.

    \param status Indication of success or failure.
    \param TYPED_BD_ADDR_T Pointer to Bluetooth device address
*/
void send_dm_handle_bdaddr_black_list_cfm(
        const CsrUint8 status,
        const TYPED_BD_ADDR_T * const addrt)
{

    DM_HANDLE_DEVICE_BLACK_LIST_CFM_T *dm_prim;

    dm_prim = zpnew(DM_HANDLE_DEVICE_BLACK_LIST_CFM_T);
    dm_prim->type = DM_HANDLE_DEVICE_BLACK_LIST_CFM;
    dm_prim->status = status;

    if (addrt != NULL)
        tbdaddr_copy(&dm_prim->addrt, addrt);

    DM_SendMessageAMphandle(dm_prim);

}

/*! \brief Handle DM_HANDLE_DEVICE_BLACK_LIST_REQ primitive.

    \param status Indication of success or failure.
    \param p_uprim Pointer to DM primitive DM_UPRIM_T
*/
void dm_handle_bdaddr_black_list_req(const DM_UPRIM_T *const p_uprim)
{
    CsrUint8 status = DM_BLACK_LIST_CFM_SUCCESS;
    CsrUint8 no_of_devices = 0;

    /* See if device is already in the list */
    if(dm_is_device_in_black_list(&p_uprim->dm_handle_device_black_list_req.addrt.addr
                                                         , &no_of_devices))
        status = DM_BLACK_LIST_CFM_DEVICE_ALREADY_IN_LIST;

    if(no_of_devices >= DM_MAX_NUM_DEVICES_IN_BLACK_LIST)
        status = DM_BLACK_LIST_CFM_EXCEEDED_NO_OF_DEVICES;


    switch(p_uprim->dm_handle_device_black_list_req.flags & DM_BLACK_LIST_MASK)
    {
        case DM_BLACK_LIST_REMOVE_ALL_DEVICES:
        {
            dm_remove_all_devices_from_black_list();
            status = DM_BLACK_LIST_CFM_SUCCESS;
        }
        break;

        case DM_BLACK_LIST_ADD_DEVICE:
        {
           if(status == DM_BLACK_LIST_CFM_SUCCESS)
                  status = dm_add_device_in_black_list(
                                 &p_uprim->dm_handle_device_black_list_req.addrt.addr);
        }
        break;

        case DM_BLACK_LIST_REMOVE_DEVICE:
        {
            dm_remove_device_from_black_list(
                                &p_uprim->dm_handle_device_black_list_req.addrt.addr);
            status = DM_BLACK_LIST_CFM_SUCCESS;
        }
        break;

        default:
        {
            status = DM_BLACK_LIST_CFM_ILLEGAL_OPERATION;
        }
        break;
    }
    /* Send an event back to application for confirmation. */
    send_dm_handle_bdaddr_black_list_cfm(status,
               (const TYPED_BD_ADDR_T * const) &p_uprim->dm_handle_device_black_list_req.addrt);
}

/*! \brief Add the BD_ADDR in the Black List.

    \param pointer to the BD_ADDR
    \returns status SUCCESS/FAIL
*/
static CsrUint8 dm_add_device_in_black_list(const BD_ADDR_T * p_bd_addr)
{
    DM_ACL_BD_ADDR_LIST_T *p_new;
    CsrUint8 status = DM_BLACK_LIST_CFM_SUCCESS;

    /* Get a new connection list entry */
    if ((p_new = zpnew(DM_ACL_BD_ADDR_LIST_T)) == NULL)
         status = DM_BLACK_LIST_CFM_MEMORY_FULL;

    bd_addr_copy(&p_new->bd_addr, p_bd_addr);

    /* Add the BD_ADDR to the list */
    p_new->p_next = p_bdaddr_black_list;
    p_bdaddr_black_list = p_new;

    return status;
}

/*! \brief Remove the BD_ADDR in the Black List.

    \param pointer to the BD_ADDR
    \returns none
*/
static void dm_remove_device_from_black_list(const BD_ADDR_T * p_bd_addr)
{
    DM_ACL_BD_ADDR_LIST_T *p,**pp;

    for(pp=&p_bdaddr_black_list; (p=*pp)!= NULL; )
    {
       if(bd_addr_eq(&p->bd_addr, p_bd_addr))
       {
           *pp = p->p_next;
           CsrPmemFree(p);
           p = NULL;
           break;
       }
       else
       {
           pp = &(p->p_next);
       }
    }
}

/*! \brief Check whether the supplied BD_ADDR is present in the black list.

    \param pointer to the BD_ADDR
    \returns True if present False otherwise
*/
CsrBool dm_is_device_in_black_list(const BD_ADDR_T *p_bd_addr, CsrUint8 * no_of_devices)
{
    CsrBool rc = FALSE;
    DM_ACL_BD_ADDR_LIST_T *p;
    CsrUint8 bdaddrs = 0;

    for(p=p_bdaddr_black_list; p!= NULL; p= p->p_next)
    {
        if(bd_addr_eq(&p->bd_addr, p_bd_addr))
        {
            return rc = TRUE;
        }
        bdaddrs++;
    }

    if(no_of_devices)
    {
        *no_of_devices = bdaddrs;
    }

     return rc;
}

/*! \brief Clear the entire black list.

    \param status Indication of success or failure.
    \param none
    \returns none
*/
void dm_remove_all_devices_from_black_list(void)
{
    DM_ACL_BD_ADDR_LIST_T *p,**pp;

    for(pp=&p_bdaddr_black_list; (p=*pp)!= NULL; )
    {
        *pp = p->p_next;
         CsrPmemFree(p);
         p = NULL;
    }
}

#endif


#endif
