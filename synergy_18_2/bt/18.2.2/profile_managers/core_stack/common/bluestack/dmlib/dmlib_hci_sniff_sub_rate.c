/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_sniff_sub_rate.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_sniff_sub_rate
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_SNIFF_SUB_RATE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_SNIFF_SUB_RATE
void dm_hci_sniff_sub_rate(
    BD_ADDR_T   *p_bd_addr,
    CsrUint16    max_remote_latency,
    CsrUint16    min_remote_timeout,
    CsrUint16    min_local_timeout,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_SNIFF_SUB_RATE_REQ_T *p_prim = zpnew(DM_HCI_SNIFF_SUB_RATE_REQ_T);
    p_prim->common.op_code      = DM_HCI_SNIFF_SUB_RATE_REQ;
    p_prim->max_remote_latency  = max_remote_latency;
    p_prim->min_remote_timeout  = min_remote_timeout;
    p_prim->min_local_timeout   = min_local_timeout;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_SNIFF_SUB_RATE */
