/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_read_num_supported_iac.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_read_num_supported_iac
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_READ_NUM_SUPPORTED_IAC_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC
void dm_hci_read_num_supported_iac(
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T *p_prim = zpnew(DM_HCI_READ_NUM_SUPPORTED_IAC_REQ_T);

    p_prim->common.op_code = DM_HCI_READ_NUM_SUPPORTED_IAC_REQ;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_READ_NUM_SUPPORTED_IAC */
