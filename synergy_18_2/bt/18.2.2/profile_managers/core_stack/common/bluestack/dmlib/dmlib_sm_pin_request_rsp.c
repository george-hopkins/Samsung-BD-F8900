/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_sm_pin_request_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_sm_pin_request_res
 *
 *  DESCRIPTION
 *      Build and send a DM_SM_PIN_REQUEST_RSP primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_SM_PIN_REQUEST
void dm_sm_pin_request_rsp(
    BD_ADDR_T *p_bd_addr,
    CsrUint8 pin_length,
    CsrUint8 *p_pin,
    DM_UPRIM_T **pp_prim
    )
{
    CsrUint8 i;
    DM_SM_PIN_REQUEST_RSP_T *p_prim = zpnew(DM_SM_PIN_REQUEST_RSP_T);

    p_prim->type = DM_SM_PIN_REQUEST_RSP;
    tbdaddr_copy_from_public_bd_addr(&p_prim->addrt, p_bd_addr);
    p_prim->pin_length = pin_length;
    if (pin_length)
    {
        for (i = 0; i < pin_length; i++)
        {
            p_prim->pin[i] = p_pin[i];
        }
    }

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_SM_PIN_REQUEST */
