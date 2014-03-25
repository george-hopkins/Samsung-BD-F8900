/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_read_local_oob_data_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_read_local_oob_data_req
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_READ_LOCAL_OOB_DATA_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA
void dm_sm_read_local_oob_data_req(
    DM_UPRIM_T **pp_prim
    )
{
    DM_SM_READ_LOCAL_OOB_DATA_REQ_T *p_prim = pnew(DM_SM_READ_LOCAL_OOB_DATA_REQ_T);
    p_prim->type            = DM_SM_READ_LOCAL_OOB_DATA_REQ;
    p_prim->unused          = 0;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_READ_LOCAL_OOB_DATA */
