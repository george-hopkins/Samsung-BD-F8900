/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_write_cached_page_mode_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_write_cached_page_mode_req
 *
 *  DESCRIPTION
 *      Build and send a DM_WRITE_CACHED_PAGE_MODE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_WRITE_CACHED_PAGE_MODE
void dm_write_cached_page_mode_req(
    BD_ADDR_T *p_bd_addr,
    page_scan_mode_t page_scan_mode,
    page_scan_rep_mode_t page_scan_rep_mode,
    DM_UPRIM_T **pp_prim
    )
{
    DM_WRITE_CACHED_PAGE_MODE_REQ_T *p_prim = pnew(DM_WRITE_CACHED_PAGE_MODE_REQ_T);

    p_prim->type = DM_WRITE_CACHED_PAGE_MODE_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->page_scan_mode = page_scan_mode;
    p_prim->page_scan_rep_mode = page_scan_rep_mode;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_WRITE_CACHED_PAGE_MODE */
