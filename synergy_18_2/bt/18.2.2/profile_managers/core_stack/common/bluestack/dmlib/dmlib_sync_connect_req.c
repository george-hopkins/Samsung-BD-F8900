/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_connect_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_CONNECT_REQ primitive.
    \param phandle Handle/queue of registration.
    \param pv_cbarg Semi-opaque context of registration.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param tx_bdw Required TX Bandwidth.
    \param rx_bdw Required RX Bandwidth.
    \param max_latency Maximum permitted latency.
    \param voice_settings Voice settings.
    \param retx_effort Retransmission effort.
    \param packet_type Permitted packet types.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_CONNECT
void dm_sync_connect_req(CsrSchedQid phandle,
                         CsrUint16 pv_cbarg,
                         BD_ADDR_T *p_bd_addr,
                         CsrUint32 tx_bdw,
                         CsrUint32 rx_bdw,
                         CsrUint16 max_latency,
                         CsrUint16 voice_settings,
                         CsrUint8 retx_effort,
                         hci_pkt_type_t packet_type)
{
    DM_SYNC_CONFIG_T *config;
    DM_SYNC_CONNECT_REQ_T *prim;

    prim = pnew(DM_SYNC_CONNECT_REQ_T);
    config = prim->u.config = pnew(DM_SYNC_CONFIG_T);

    config->tx_bdw                  = tx_bdw;
    config->rx_bdw                  = rx_bdw;
    config->max_latency             = max_latency;
    config->voice_settings          = voice_settings;
    config->retx_effort             = retx_effort;
    config->packet_type             = packet_type;

    prim->type                      = DM_SYNC_CONNECT_REQ;
    prim->phandle                   = phandle;
    prim->pv_cbarg                  = pv_cbarg;
    prim->length                    = 0;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);

    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);

}

#endif /* CSR_BT_EXCLUDE_SYNC_CONNECT */
