/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_am_register_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_am_register_req
 *
 *  DESCRIPTION
 *      Build and send a DM_AM_REGISTER_REQ primitive to the DM cmd interface.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_AM_REGISTER
void dm_am_register_req(
    CsrSchedQid phandle
    )
{
    DM_AM_REGISTER_REQ_T *p_prim = zpnew(DM_AM_REGISTER_REQ_T);

    p_prim->type = DM_AM_REGISTER_REQ;
    p_prim->phandle = phandle;

    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
}


#endif /* CSR_BT_EXCLUDE_AM_REGISTER */
