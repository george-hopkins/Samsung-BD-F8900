/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_connect_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_CONNECT_RSP primitive.
    \param p_bd_addr Pointer to Bluetooth address of remote device.
    \param response Acceptance or rejection of connection.
    \param tx_bdw Required TX Bandwidth.
    \param rx_bdw Required RX Bandwidth.
    \param max_latency Maximum permitted latency.
    \param voice_settings Voice settings.
    \param retx_effort Retransmission effort.
    \param packet_type Permitted packet types.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_CONNECT
void dm_sync_connect_rsp(BD_ADDR_T *p_bd_addr,
                         CsrUint8 response,
                         CsrUint32 tx_bdw,
                         CsrUint32 rx_bdw,
                         CsrUint16 max_latency,
                         CsrUint16 voice_settings,
                         CsrUint8 retx_effort,
                         hci_pkt_type_t packet_type)
{
    DM_SYNC_CONNECT_RSP_T *prim = pnew(DM_SYNC_CONNECT_RSP_T);

    prim->type                      = DM_SYNC_CONNECT_RSP;
    prim->response                  = response;
    prim->config.tx_bdw             = tx_bdw;
    prim->config.rx_bdw             = rx_bdw;
    prim->config.max_latency        = max_latency;
    prim->config.voice_settings     = voice_settings;
    prim->config.retx_effort        = retx_effort;
    prim->config.packet_type        = packet_type;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
}

#endif /* CSR_BT_EXCLUDE_SYNC_CONNECT */
