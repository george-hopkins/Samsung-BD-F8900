/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_renegotiate_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_RENEGOTIATE_RSP primitive.
    \param handle HCI connection handle of existing SCO/eSCO link.
    \param max_latency Maximum permitted latency.
    \param retx_effort Retransmission effort.
    \param packet_type Permitted packet types.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_RENEGOTIATE
void dm_sync_renegotiate_req(hci_connection_handle_t handle,
                             CsrUint16 max_latency,
                             CsrUint8 retx_effort,
                             hci_pkt_type_t packet_type)
{
    DM_SYNC_RENEGOTIATE_REQ_T *prim;
    DM_SYNC_CONFIG_T *config;

    prim = pnew(DM_SYNC_RENEGOTIATE_REQ_T);
    config = prim->u.config = zpnew(DM_SYNC_CONFIG_T);

    config->max_latency             = max_latency;
    config->retx_effort             = retx_effort;
    config->packet_type             = packet_type;

    prim->type                      = DM_SYNC_RENEGOTIATE_REQ;
    prim->handle                    = handle;
    prim->length                    = 0;

    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
}

#endif /* CSR_BT_EXCLUDE_SYNC_RENEGOTIATE */
