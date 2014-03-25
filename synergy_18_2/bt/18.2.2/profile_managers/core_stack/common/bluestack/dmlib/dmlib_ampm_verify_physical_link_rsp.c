/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_verify_physical_link_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_VERIFY_PHYSICAL_LINK
void dm_ampm_verify_physical_link_rsp(const BD_ADDR_T *const p_bd_addr,
                                      const CsrUint16 identifier,
                                      const CsrBool exists,
                                      const l2ca_fs_flush_t link_supervision_timeout,
                                      const l2ca_fs_flush_t best_effort_flush_timeout)
{
    DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T *p_prim = pnew(DM_AMPM_VERIFY_PHYSICAL_LINK_RSP_T);

    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->identifier = identifier;
    p_prim->exists = exists;
    p_prim->link_supervision_timeout = link_supervision_timeout;
    p_prim->best_effort_flush_timeout = best_effort_flush_timeout;

    dm_put_message(p_prim, DM_AMPM_VERIFY_PHYSICAL_LINK_RSP);
}

#endif /* CSR_BT_EXCLUDE_AMPM_VERIFY_PHYSICAL_LINK */
