/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_generate_nonresolvable_private_address_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dmlib_sm_generate_nonresolvable_private_address_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ
 *      primitive. If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS
void dmlib_sm_generate_nonresolvable_private_address_req(
        DM_UPRIM_T **pp_prim)
{
    DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T *prim =
            pnew(DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ_T);

    prim->type = DM_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS_REQ;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_GENERATE_NONRESOLVABLE_PRIVATE_ADDRESS */
