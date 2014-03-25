/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_modify_advert_filter_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_set_ble_connection_parameters_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SET_BLE_CONNECTION_PARAMETERS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SET_BLE_CONNECTION_PARAMETERS
void dm_set_ble_connection_parameters_req(CsrUint16 scan_interval,
                                         CsrUint16 scan_window,
                                         CsrUint16 conn_interval_min,
                                         CsrUint16 conn_interval_max,
                                         CsrUint16 conn_latency,
                                         CsrUint16 supervision_timeout,
                                         CsrUint16 conn_attempt_timeout,
                                         CsrUint16 adv_interval_min,
                                         CsrUint16 adv_interval_max,
                                         CsrUint16 conn_latency_max,
                                         CsrUint16 supervision_timeout_min,
                                         CsrUint16 supervision_timeout_max,
                                         DM_UPRIM_T  **pp_prim
                                        )
{
    DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T *p_prim
                                    = zpnew(DM_SET_BLE_CONNECTION_PARAMETERS_REQ_T);

    p_prim->type                 = DM_SET_BLE_CONNECTION_PARAMETERS_REQ;
    p_prim->scan_interval        = scan_interval;
    p_prim->scan_window          = scan_window;
    p_prim->conn_interval_min    = conn_interval_min;
    p_prim->conn_interval_max    = conn_interval_max;
    p_prim->conn_latency         = conn_latency;
    p_prim->supervision_timeout  = supervision_timeout;
    p_prim->conn_attempt_timeout = conn_attempt_timeout;
    p_prim->adv_interval_min     = adv_interval_min;
    p_prim->adv_interval_max     = adv_interval_max;
    p_prim->conn_latency_max     = conn_latency_max;
    p_prim->supervision_timeout_min = supervision_timeout_min;
    p_prim->supervision_timeout_max = supervision_timeout_max;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SET_BLE_CONNECTION_PARAMETERS */
