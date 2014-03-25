/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_ble_connection_par_update_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_ble_connection_par_update_rsp
 *
 *  DESCRIPTION
 *      Build and send a DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_BLE_ACCEPT_CONNECTION_PAR_UPDATE
void dm_ble_connection_par_update_rsp(
    l2ca_identifier_t signal_id,
    TYPED_BD_ADDR_T bd_addrt,
    CsrUint16 conn_interval_min,
    CsrUint16 conn_interval_max,
    CsrUint16 conn_latency,
    CsrUint16 supervision_timeout,
    CsrUint16 result,
    DM_UPRIM_T **pp_prim
    )
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T *prim = zpnew(DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP_T);

    prim->type = DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_RSP;
    prim->signal_id = signal_id;
    prim->bd_addrt = bd_addrt;
    prim->conn_interval_min = conn_interval_min;
    prim->conn_interval_max = conn_interval_max;
    prim->conn_latency = conn_latency;
    prim->supervision_timeout = supervision_timeout;
    prim->result = result;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}

#endif /* CSR_BT_EXCLUDE_BLE_ACCEPT_CONNECTION_PAR_UPDATE */
