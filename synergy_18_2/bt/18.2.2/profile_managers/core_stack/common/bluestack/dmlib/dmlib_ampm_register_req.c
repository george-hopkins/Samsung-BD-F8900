/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_register_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_REGISTER
void dm_ampm_register_req(const CsrSchedQid phandle)
{
    DM_AMPM_REGISTER_REQ_T *p_prim = pnew(DM_AMPM_REGISTER_REQ_T);

    p_prim->phandle = phandle;

    dm_put_message(p_prim, DM_AMPM_REGISTER_REQ);
}

#endif /* CSR_BT_EXCLUDE_AMPM_REGISTER */
