/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_disconnect_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_DISCONNECT_REQ primitive.
    \param handle HCI connection handle of SCO/eSCO link.
    \param reason HCI reason code for disconnection.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_DISCONNECT
void dm_sync_disconnect_req(hci_connection_handle_t handle, hci_reason_t reason)
{
    DM_SYNC_DISCONNECT_REQ_T *prim = pnew(DM_SYNC_DISCONNECT_REQ_T);

    prim->type              = DM_SYNC_DISCONNECT_REQ;
    prim->handle            = handle;
    prim->reason            = reason;
    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
}

#endif /* CSR_BT_EXCLUDE_SYNC_DISCONNECT */
