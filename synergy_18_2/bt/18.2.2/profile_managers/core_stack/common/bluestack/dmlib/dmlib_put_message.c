/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_put_message.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

void dm_put_message(void *p_prim, const dm_prim_t type)
{
    *((dm_prim_t*)p_prim) = type;
    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
}
