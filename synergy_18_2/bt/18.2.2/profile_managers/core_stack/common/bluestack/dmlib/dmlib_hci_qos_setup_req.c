/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_qos_setup_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_qos_setup_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_QOS_SETUP_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_QOS_SETUP
void dm_hci_qos_setup_req(
    BD_ADDR_T *p_bd_addr,
    CsrUint8 flags,              /* Reserved */
    hci_qos_type_t service_type,
    CsrUint32 token_rate,         /* in bytes per second */
    CsrUint32 peak_bandwidth,     /* peak bandwidth in bytes per sec */
    CsrUint32 latency,            /* in microseconds */
    CsrUint32 delay_variation,    /* in microseconds */
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_QOS_SETUP_REQ_T *p_prim = zpnew(DM_HCI_QOS_SETUP_REQ_T);

    p_prim->common.op_code = DM_HCI_QOS_SETUP_REQ;
    bd_addr_copy(&p_prim->bd_addr, p_bd_addr);
    p_prim->flags = flags;
    p_prim->service_type = service_type;
    p_prim->token_rate = token_rate;
    p_prim->peak_bandwidth = peak_bandwidth;
    p_prim->latency = latency;
    p_prim->delay_variation = delay_variation;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_QOS_SETUP */
