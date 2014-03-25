/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_set_bt_version.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_set_bt_version
 *
 *  DESCRIPTION
 *      Build and send a DM_SET_BT_VERSION_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SET_BT_VERSION
void dm_set_bt_version(
    CsrUint8     version,
    DM_UPRIM_T  **pp_prim
    )
{
    DM_SET_BT_VERSION_REQ_T *p_prim = pnew(DM_SET_BT_VERSION_REQ_T);
    p_prim->type                    = DM_SET_BT_VERSION_REQ;
    p_prim->version                 = version;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SET_BT_VERSION */
