/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_send_primitive.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_send_primitive
 *
 *  DESCRIPTION
 *      Send DM primitive into the stack. This is for use when sending
 *      primitives that were built but not sent by an earlier call to one of
 *      the DM Library functions, or when sending primitives that have been
 *      constructed by the application.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void dm_send_primitive(
    DM_UPRIM_T *p_prim
    )
{
    if (p_prim)
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}

