/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_read_bd_addr_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_READ_BD_ADDR
void dm_ampm_read_bd_addr_req(void)
{
    DM_AMPM_READ_BD_ADDR_REQ_T *p_prim = pnew(DM_AMPM_READ_BD_ADDR_REQ_T);

    dm_put_message(p_prim, DM_AMPM_READ_BD_ADDR_REQ);
}

#endif /* CSR_BT_EXCLUDE_AMPM_READ_BD_ADDR */
