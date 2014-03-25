/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_class_of_device.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_class_of_device
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_CLASS_OF_DEVICE_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_CLASS_OF_DEVICE
void dm_hci_write_class_of_device(
    CsrUint24 dev_class,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T *p_prim = zpnew(DM_HCI_WRITE_CLASS_OF_DEVICE_REQ_T);

    p_prim->common.op_code = DM_HCI_WRITE_CLASS_OF_DEVICE_REQ;
    p_prim->dev_class = dev_class;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_CLASS_OF_DEVICE */
