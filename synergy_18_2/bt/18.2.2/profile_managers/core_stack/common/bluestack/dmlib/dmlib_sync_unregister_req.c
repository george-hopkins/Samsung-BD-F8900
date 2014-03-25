/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_unregister_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_UNREGISTER_REQ primitive.
    \param phandle Handle/queue of registration.
    \param pv_cbarg Semi-opaque context of registration.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_UNREGISTER
void dm_sync_unregister_req(CsrSchedQid phandle, CsrUint16 pv_cbarg)
{
    DM_SYNC_UNREGISTER_REQ_T *p_prim = pnew(DM_SYNC_UNREGISTER_REQ_T);

    p_prim->type        = DM_SYNC_UNREGISTER_REQ;
    p_prim->phandle     = phandle;
    p_prim->pv_cbarg    = pv_cbarg;
    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
}

#endif /* CSR_BT_EXCLUDE_SYNC_UNREGISTER */
