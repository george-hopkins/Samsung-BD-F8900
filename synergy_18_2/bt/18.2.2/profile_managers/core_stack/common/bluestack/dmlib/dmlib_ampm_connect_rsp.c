/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_connect_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_CONNECT
void dm_ampm_connect_rsp(const DM_AMPM_CONNECT_IND_T *const p_ind_prim,
                         const hci_return_t status)
{
    DM_AMPM_CONNECT_RSP_T *p_prim = pnew(DM_AMPM_CONNECT_RSP_T);

    bd_addr_copy(&p_prim->bd_addr, &p_ind_prim->bd_addr);

    p_prim->cid             = p_ind_prim->cid;
    p_prim->local_amp_id    = p_ind_prim->local_amp_id;
    p_prim->remote_amp_id   = p_ind_prim->remote_amp_id;
    p_prim->status          = status;

    dm_put_message(p_prim, DM_AMPM_CONNECT_RSP);
}

#endif /* CSR_BT_EXCLUDE_AMPM_CONNECT */
