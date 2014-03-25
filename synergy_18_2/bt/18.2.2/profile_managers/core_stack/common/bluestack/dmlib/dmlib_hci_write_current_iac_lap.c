/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_current_iac_lap.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_current_iac_lap
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_CURRENT_IAC_LAP_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_CURRENT_IAC_LAP
void dm_hci_write_current_iac_lap(
    CsrUint8 num_iac,
    CsrUint24 *a_iacs,   /* Array of IACs */
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T *p_prim = zpnew(DM_HCI_WRITE_CURRENT_IAC_LAP_REQ_T);
    CsrUint8 i, j;
    CsrUint8 block;

    p_prim->common.op_code = DM_HCI_WRITE_CURRENT_IAC_LAP_REQ;
    p_prim->num_current_iac = num_iac;
    for (block = 0, i = 0; i < num_iac; i += HCI_IAC_LAP_PER_PTR)
    {
        p_prim->iac_lap[block] = (CsrUint24 *) CsrPmemAlloc(sizeof(CsrUint24) * HCI_IAC_LAP_PER_PTR);
        for (j = i; (j < i + HCI_IAC_LAP_PER_PTR) && (j < num_iac); j++)
        {
            p_prim->iac_lap[block][j - i] = a_iacs[j];
        }

        block++;
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


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_CURRENT_IAC_LAP */
