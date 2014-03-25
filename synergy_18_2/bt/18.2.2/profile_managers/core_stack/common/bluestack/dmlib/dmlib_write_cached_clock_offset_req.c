/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_write_cached_clock_offset_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_write_cached_clock_offset_req
 *
 *  DESCRIPTION
 *      Build and send a DM_WRITE_CACHED_CLOCK_OFFSET_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_WRITE_CACHED_CLOCK_OFFSET
void dm_write_cached_clock_offset_req(
    BD_ADDR_T *p_bd_addr,
    CsrUint16 clock_offset,
    DM_UPRIM_T **pp_prim
    )
{
    DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T *p_prim = pnew(DM_WRITE_CACHED_CLOCK_OFFSET_REQ_T);

    p_prim->type = DM_WRITE_CACHED_CLOCK_OFFSET_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->clock_offset = clock_offset;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_WRITE_CACHED_CLOCK_OFFSET */
