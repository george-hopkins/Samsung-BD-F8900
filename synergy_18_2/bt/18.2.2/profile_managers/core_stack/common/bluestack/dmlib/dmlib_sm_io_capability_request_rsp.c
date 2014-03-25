/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_io_capability_request_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_io_capability_request_res
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_IO_CAPABILITY_REQUEST_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_IO_CAPABILITY_REQUEST
void dm_sm_io_capability_request_rsp(
    TYPED_BD_ADDR_T     *addrt,
    CsrUint8     io_capability,
    CsrUint8     authentication_requirements,
    CsrUint8     oob_data_present,
    CsrUint8     *oob_hash_c,
    CsrUint8     *oob_rand_r,
    CsrUint16    key_distribution,
    DM_UPRIM_T  **pp_prim
    )
{
    DM_SM_IO_CAPABILITY_REQUEST_RSP_T *p_prim
                                    = zpnew(DM_SM_IO_CAPABILITY_REQUEST_RSP_T);
    p_prim->type                = DM_SM_IO_CAPABILITY_REQUEST_RSP;
    p_prim->io_capability       = io_capability;
    p_prim->authentication_requirements = authentication_requirements;
    if ((p_prim->oob_data_present = oob_data_present) != DM_SM_OOB_DATA_NONE)
    {
        p_prim->oob_hash_c = (CsrUint8*) pcopy(oob_hash_c, SIZE_OOB_DATA);
        if(DM_SM_OOB_DATA_BOTH==oob_data_present)
            p_prim->oob_rand_r = (CsrUint8*) pcopy(oob_rand_r, SIZE_OOB_DATA);
    }
    p_prim->key_distribution = key_distribution;
    tbdaddr_copy(&p_prim->addrt, addrt);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_IO_CAPABILITY_REQUEST */
