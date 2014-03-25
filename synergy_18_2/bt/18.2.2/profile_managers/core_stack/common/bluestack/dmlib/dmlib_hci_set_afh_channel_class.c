/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_set_afh_channel_class.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_set_afh_channel_class
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_SET_AFH_CHANNEL_CLASS_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_SET_AFH_CHANNEL_CLASS
void dm_hci_set_afh_channel_class(
    CsrUint8 * map,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T *p_prim = zpnew(DM_HCI_SET_AFH_CHANNEL_CLASS_REQ_T);
    p_prim->common.op_code = DM_HCI_SET_AFH_CHANNEL_CLASS_REQ;
    CsrMemCpy(p_prim->map, map, 10);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_SET_AFH_CHANNEL_CLASS */
