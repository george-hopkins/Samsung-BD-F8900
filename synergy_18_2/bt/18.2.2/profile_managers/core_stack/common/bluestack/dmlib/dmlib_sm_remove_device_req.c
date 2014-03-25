/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_remove_device_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_remove_device_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_REMOVE_DEVICE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_REMOVE_DEVICE
void dm_sm_remove_device_req(
    CsrSchedQid phandle,
    TYPED_BD_ADDR_T *addrt,
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_REMOVE_DEVICE_REQ_T *prim = zpnew(DM_SM_REMOVE_DEVICE_REQ_T);

    prim->type = DM_SM_REMOVE_DEVICE_REQ;
    prim->phandle = phandle;
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


#endif /* CSR_BT_EXCLUDE_SM_REMOVE_DEVICE */
