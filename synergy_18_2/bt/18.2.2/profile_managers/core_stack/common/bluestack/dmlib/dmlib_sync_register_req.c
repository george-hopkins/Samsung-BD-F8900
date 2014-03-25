/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sync_register_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*! \brief Build and send a DM_SYNC_REGISTER_REQ primitive.
    \param phandle Handle/queue for registering entity.
    \param pv_cbarg Semi-opaque context for registering entity.
*/

#ifndef CSR_BT_EXCLUDE_SYNC_REGISTER
void dm_sync_register_req(CsrSchedQid phandle, CsrUint16 pv_cbarg)
{
    DM_SYNC_REGISTER_REQ_T *p_prim = zpnew(DM_SYNC_REGISTER_REQ_T);

    p_prim->type        = DM_SYNC_REGISTER_REQ;
    p_prim->phandle     = phandle;
    p_prim->pv_cbarg    = pv_cbarg;
    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
}

#endif /* CSR_BT_EXCLUDE_SYNC_REGISTER */
