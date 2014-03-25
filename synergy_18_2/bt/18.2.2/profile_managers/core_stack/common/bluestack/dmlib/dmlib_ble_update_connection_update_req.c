/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ble_update_connection_update_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_ble_update_connection_update_req
 *
 *  DESCRIPTION
 *      Build and send a DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_BLE_UPDATE_CONNECTION_PARAMETERS
void dm_ble_update_connection_update_req(
    TYPED_BD_ADDR_T *addrt,
    CsrUint16 conn_interval_min,
    CsrUint16 conn_interval_max,
    CsrUint16 conn_latency,
    CsrUint16 supervision_timeout,
    CsrUint16 minimum_ce_length,
    CsrUint16 maximum_ce_length,
    DM_UPRIM_T **pp_prim
    )
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T *prim = zpnew(DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ_T);

    prim->common.op_code = DM_BLE_UPDATE_CONNECTION_PARAMETERS_REQ;
    prim->conn_interval_min = conn_interval_min;
    prim->conn_interval_max = conn_interval_max;
    prim->conn_latency = conn_latency;
    prim->supervision_timeout = supervision_timeout;
    prim->minimum_ce_length = minimum_ce_length;
    prim->maximum_ce_length = maximum_ce_length;

    tbdaddr_copy(&prim->addrt, addrt);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_BLE_UPDATE_CONNECTION_PARAMETERS */
