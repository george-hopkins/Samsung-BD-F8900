/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_enable_device_ut_mode.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_enable_device_ut_mode
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ENABLE_DUT_MODE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ENABLE_DUT_MODE
void dm_hci_enable_device_ut_mode(
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ENABLE_DUT_MODE_REQ_T *p_prim = zpnew(DM_HCI_ENABLE_DUT_MODE_REQ_T);

    p_prim->common.op_code = DM_HCI_ENABLE_DUT_MODE_REQ;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ENABLE_DUT_MODE */
