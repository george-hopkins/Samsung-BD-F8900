/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_connect_channel_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_CONNECT_CHANNEL
void dm_ampm_connect_channel_rsp(const DM_AMPM_CONNECT_CHANNEL_IND_T *const p_ind_prim,
                                 const amp_link_id_t logical_link_id,
                                 const CsrUint8 physical_link_id,
                                 const CsrSchedQid hci_data_queue,
                                 const hci_return_t status)
{
    DM_AMPM_CONNECT_CHANNEL_RSP_T *p_prim = pnew(DM_AMPM_CONNECT_CHANNEL_RSP_T);

    CsrMemCpy(&p_prim->tx_flowspec, &p_ind_prim->tx_flowspec, sizeof(L2CA_FLOWSPEC_T));
    CsrMemCpy(&p_prim->rx_flowspec, &p_ind_prim->rx_flowspec, sizeof(L2CA_FLOWSPEC_T));
    bd_addr_copy(&p_prim->bd_addr, &p_ind_prim->bd_addr);

    p_prim->cid             = p_ind_prim->cid;
    p_prim->local_amp_id    = p_ind_prim->local_amp_id;
    p_prim->logical_link_id = logical_link_id;
    p_prim->physical_link_id = physical_link_id;
    p_prim->hci_data_queue = hci_data_queue;
    p_prim->status          = status;

    dm_put_message(p_prim, DM_AMPM_CONNECT_CHANNEL_RSP);
}

#endif /* CSR_BT_EXCLUDE_AMPM_CONNECT_CHANNEL */
