#ifndef _DM_ACL_CON_PAR_CACHE_H_
#define _DM_ACL_CON_PAR_CACHE_H_

#include "csr_synergy.h"
/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_acl_con_par_cache.h

\brief  ACL connection parameter cache.
*/

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Bitmap identifying which parameters are to be changed when
   the connection parameter cache is updated. */
typedef CsrUint8 cache_contents_t;
#define UPDATE_PAGE     ((cache_contents_t) 0x01)
#define UPDATE_CLOCK    ((cache_contents_t) 0x02)

void reset_conn_param_cache(void);

#ifndef DISABLE_DM_BREDR
void dm_acl_write_cached_page_mode(const DM_UPRIM_T *const p_uprim);
void dm_acl_write_cached_clock_offset(const DM_UPRIM_T *const p_uprim);
void dm_acl_clear_conn_cache(const DM_UPRIM_T *const p_uprim);
CsrBool dm_acl_update_cache(
    const cache_contents_t update_flag,
    const BD_ADDR_T *const p_bd_addr,
    const clock_offset_t clock_offset,
    const page_scan_rep_mode_t page_scan_rep_mode
    );
#else
#define dm_acl_write_cached_page_mode  NULL
#define dm_acl_write_cached_clock_offset  NULL
#define dm_acl_clear_conn_cache  NULL
#define dm_acl_update_cache  NULL
#endif

void dm_acl_obtain_connection_details(
    const BD_ADDR_T *const p_bd_addr,
    page_scan_mode_t *const p_page_scan_mode,
    page_scan_rep_mode_t *const p_page_scan_rep_mode,
    clock_offset_t *const p_clock_offset
    );

#ifdef __cplusplus
}
#endif

#endif /* _DM_ACL_CON_PAR_CACHE_H_ */
