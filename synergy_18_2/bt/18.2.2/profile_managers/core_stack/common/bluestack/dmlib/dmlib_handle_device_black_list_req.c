/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_handle_device_black_list_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_handle_device_black_list_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HANDLE_DEVICE_BLACK_LIST_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_HANDLE_DEVICE_BLACK_LIST
void dm_handle_device_black_list_req(
    TYPED_BD_ADDR_T *addrt,
    CsrUint16 flags,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HANDLE_DEVICE_BLACK_LIST_REQ_T *p_prim = pnew(DM_HANDLE_DEVICE_BLACK_LIST_REQ_T);

    p_prim->type = DM_HANDLE_DEVICE_BLACK_LIST_REQ;
    tbdaddr_copy(&p_prim->addrt, addrt);
    p_prim->flags = flags;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HANDLE_DEVICE_BLACK_LIST */
