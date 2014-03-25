/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_acl_con_par_cache.c

\brief  ACL connection parameter cache.
*/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "l2cap_prim.h"
#include "dm_prim.h"

#include "csr_bt_core_stack_pmalloc.h"

#include "dm_acl_core.h"
#include "dm_acl_con_par_cache.h"
#include "dm_mgt_interface.h"

#ifndef DISABLE_DM_BREDR

/*! \brief Structure of connection parameters for fast connection setup
    to a particular device. The 'local_master' flag indicates whether
    the local device was master or slave at the time the clock offset
    was obtained. This is important because the clock offset as used in
    connection setup and remote name requests assumes that the local
    device is master, neccessitating an inversion if the local device
    was slave when the clock offset was obtained. */
typedef struct CONN_PARAM_CACHE_T_tag
{
    struct CONN_PARAM_CACHE_T_tag *p_next;
    BD_ADDR_T bd_addr;
    page_scan_rep_mode_t page_scan_rep_mode;
    clock_offset_t clock_offset;
    CsrBool local_master;
} CONN_PARAM_CACHE_T;

/*! Linked list of cached connection parameters */
static CONN_PARAM_CACHE_T *p_conn_param_cache = NULL;

/* Private function prototypes. */
static hci_error_t modify_add_cache_entry(
        cache_contents_t update_flag,
        const BD_ADDR_T *const p_bd_addr,
        const clock_offset_t clock_offset,
        const page_scan_rep_mode_t page_scan_rep_mode);

static CONN_PARAM_CACHE_T **dm_acl_find_conn_param_by_bd_addr(
        const BD_ADDR_T *p_bd_addr);

/* Public functions. */

/*! \brief Delete the entire connection parameter cache. */
void reset_conn_param_cache(void)
{
    CONN_PARAM_CACHE_T *p_next;

    while (p_conn_param_cache)
    {
        p_next = p_conn_param_cache->p_next;
        CsrPmemFree(p_conn_param_cache);
        p_conn_param_cache = p_next;
    }
}


/*! \brief Update the connection parameter cache with page mode
    settings passed down from the application. If the list does
    not contain an element for the device, a new element is added.

    \param p_uprim Pointer to command primitive.
*/
void dm_acl_write_cached_page_mode(const DM_UPRIM_T *const p_uprim)
{
    hci_return_t status;

    status = modify_add_cache_entry(UPDATE_PAGE,
            &p_uprim->dm_write_cached_page_mode_req.bd_addr,
            HCI_CLOCK_OFFSET_INVALID,
            p_uprim->dm_write_cached_page_mode_req.page_scan_rep_mode);

    DM_SendStandardCommandComplete(DM_WRITE_CACHED_PAGE_MODE_CFM, status);
}

/*! \brief Update the connection parameter cache with clock offset
    settings passed down from the application. If the list does not
    contain an element for the device, a new element is added.

    \param p_uprim Pointer to command primitive.
*/
void dm_acl_write_cached_clock_offset(const DM_UPRIM_T *const p_uprim)
{
    hci_return_t status;

    status = modify_add_cache_entry(UPDATE_CLOCK,
            &p_uprim->dm_write_cached_clock_offset_req.bd_addr,
            p_uprim->dm_write_cached_clock_offset_req.clock_offset,
            HCI_PAGE_SCAN_REP_MODE_R2);

    DM_SendStandardCommandComplete(DM_WRITE_CACHED_CLOCK_OFFSET_CFM, status);
}


/*! \brief Remove the cached connection parameters for a device.

    \param p_uprim Pointer to command primitive.
*/
void dm_acl_clear_conn_cache(const DM_UPRIM_T *const p_uprim)
{
    CONN_PARAM_CACHE_T **pp_el, *p_el;
    CsrUint8 status = HCI_ERROR_COMMAND_DISALLOWED;

    if ((p_el = *(pp_el = dm_acl_find_conn_param_by_bd_addr(
                        &p_uprim->dm_clear_param_cache_req.bd_addr))) != NULL)
    {
        *pp_el = p_el->p_next;
        CsrPmemFree(p_el);
        status = HCI_SUCCESS;
    }

    DM_SendStandardCommandComplete(DM_CLEAR_PARAM_CACHE_CFM, status);
}

/*! \brief Update the cached connection parameter values for a
    particular device if it is in the list.

    \param update_flag Bitmask of fields to update.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param clock_offset Clock offset.
    \param page_scan_rep_mode Page scan repetition mode.
    \returns TRUE if the device is in the list and was updated.
*/
CsrBool dm_acl_update_cache(
    const cache_contents_t update_flag,
    const BD_ADDR_T *const p_bd_addr,
    const clock_offset_t clock_offset,
    const page_scan_rep_mode_t page_scan_rep_mode
    )
{
    CONN_PARAM_CACHE_T *p_el;
    DM_ACL_T *p_acl;

    if ((p_el = *dm_acl_find_conn_param_by_bd_addr(p_bd_addr)) != NULL)
    {
        if (update_flag & UPDATE_CLOCK)
        {
            p_el->clock_offset = clock_offset;

            /* Set the 'local_master' flag if we are currently master of the
               connection to the remote device. If there is no connection,
               then assume master (the device doing inquiry is master). */
            if ((p_acl = dm_acl_find_connected_by_bd_addr(p_bd_addr)) != NULL)
            {
                p_el->local_master =
                        ((p_acl->flags & DM_ACL_ROLE) == DM_ACL_ROLE_MASTER);
            }
            else
            {
                p_el->local_master = TRUE;
            }
        }

        if (update_flag & UPDATE_PAGE)
            p_el->page_scan_rep_mode = page_scan_rep_mode;

        return TRUE;
    }

    return FALSE;
}


/*! \brief Try to obtain ACL connection parameters from the cache.
    Parameters are returned via final three function arguments.

    \param p_bd_addr Pointer to Bluetooth address.
    \param p_page_scan_mode Pointer to page scan mode.
    \param p_page_scan_rep_mode Pointer to page scan repetition mode.
    \param p_clock_offset Pointer to clock offset.
*/
void dm_acl_obtain_connection_details(
    const BD_ADDR_T *const p_bd_addr,
    page_scan_mode_t *const p_page_scan_mode,
    page_scan_rep_mode_t *const p_page_scan_rep_mode,
    clock_offset_t *const p_clock_offset
    )
{
    CONN_PARAM_CACHE_T *p_el;
    clock_offset_t clock_offset;

    *p_page_scan_mode = HCI_PAGE_SCAN_MODE_DEFAULT;

    /* Look for the device in the cache */
    if ((p_el = *dm_acl_find_conn_param_by_bd_addr(p_bd_addr)) != NULL)
    {
        *p_page_scan_rep_mode = p_el->page_scan_rep_mode;
        clock_offset = p_el->clock_offset;
        if (!p_el->local_master)
        {
            /* The clock offset value was obtained when we were
               a slave, so this needs to be negated. */
            clock_offset = -clock_offset;
        }

        *p_clock_offset = clock_offset | HCI_CLOCK_OFFSET_VALID;

        return;
    }

    /* Not in the list, so set some default values */
    *p_page_scan_rep_mode = HCI_PAGE_SCAN_REP_MODE_R2;
    *p_clock_offset = HCI_CLOCK_OFFSET_INVALID;
}

/* Private functions. */

/*! \brief Find connection parameter cache entry by Bluetooth Address.

    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \returns Pointer to pointer to cache entry, or pointer to NULL if not found.
*/
static CONN_PARAM_CACHE_T **dm_acl_find_conn_param_by_bd_addr(
        const BD_ADDR_T *p_bd_addr)
{
    CONN_PARAM_CACHE_T **pp_el, *p_el;

    for (pp_el = &p_conn_param_cache;
            (p_el = *pp_el) != NULL && !bd_addr_eq(&p_el->bd_addr, p_bd_addr);
            pp_el = &p_el->p_next)
        ;

    return pp_el;
}

/*! \brief Modify or add entry to connection parameter cache.

    If there is no existing entry then update_flag is ignored and
    all parameters are used to populate the new cache entry.

    \param update_flag Bitmask of fields to update.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param clock_offset Clock offset.
    \param page_scan_rep_mode Page scan repetition mode.
    \returns HCI_SUCCESS or HCI_ERROR_MEMORY_FULL.
*/
static hci_return_t modify_add_cache_entry(
    cache_contents_t update_flag,
    const BD_ADDR_T *const p_bd_addr,
    const clock_offset_t clock_offset,
    const page_scan_rep_mode_t page_scan_rep_mode
    )
{
    CONN_PARAM_CACHE_T *p_el;

    if ((p_el = *dm_acl_find_conn_param_by_bd_addr(p_bd_addr)) == NULL)
    {
        if ((p_el = xpnew(CONN_PARAM_CACHE_T)) == NULL)
            return (hci_return_t)HCI_ERROR_MEMORY_FULL;

        p_el->p_next = p_conn_param_cache;
        p_conn_param_cache = p_el;

        bd_addr_copy(&p_el->bd_addr, p_bd_addr);

        /* New cache entry so must update everything. */
        update_flag = UPDATE_PAGE | UPDATE_CLOCK;
    }

    dm_acl_update_cache(update_flag, p_bd_addr, clock_offset,
            page_scan_rep_mode);

    return HCI_SUCCESS;
}

#endif
