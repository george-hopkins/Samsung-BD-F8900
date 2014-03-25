/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_disconnect_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_DISCONNECT
void dm_ampm_disconnect_req(const BD_ADDR_T *const p_bd_addr,
                            const l2ca_controller_t local_amp_id,
                            const hci_error_t reason,
                            const CsrUint8 active_links)
{
    DM_AMPM_DISCONNECT_REQ_T *p_prim = pnew(DM_AMPM_DISCONNECT_REQ_T);

    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    p_prim->local_amp_id = local_amp_id;
    p_prim->reason = reason;
    p_prim->active_links = active_links;

    dm_put_message(p_prim, DM_AMPM_DISCONNECT_REQ);
}

#endif /* CSR_BT_EXCLUDE_AMPM_DISCONNECT */
