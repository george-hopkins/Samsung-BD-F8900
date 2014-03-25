/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_configure_local_address_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_configure_local_address_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_CONFIGURE_LOCAL_ADDRESS
void dm_sm_configure_local_address_req(
        DM_SM_PERMANENT_ADDRESS_T permanent_address_type,
        TYPED_BD_ADDR_T static_addrt,
        DM_SM_PRIVACY_T privacy,
        DM_UPRIM_T **pp_prim)
{
    DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T *prim = pnew(DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ_T);

    prim->type = DM_SM_CONFIGURE_LOCAL_ADDRESS_REQ;
    prim->privacy = privacy;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_CONFIGURE_LOCAL_ADDRESS */
