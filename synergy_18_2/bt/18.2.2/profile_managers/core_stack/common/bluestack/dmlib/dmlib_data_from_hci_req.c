/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_data_from_hci_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

#ifndef BUILD_FOR_HOST
#ifndef DM_HCI_IFACEQUEUE
/* Define this to a known queue on-chip just to allow us to compile */
#define DM_HCI_IFACEQUEUE DM_IFACEQUEUE
#endif
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_data_from_hci_req
 *
 *  DESCRIPTION
 *      Build and send a DM_DATA_FROM_HCI_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_DATA_FROM_HCI
extern void dm_data_from_hci_req(const l2ca_controller_t controller,
                                 const CsrUint8 physical_handle,
                                 const CsrUint16 logical_handle,
                                 CsrMblk *data,
                                 DM_UPRIM_T **pp_prim)
{
    DM_DATA_FROM_HCI_REQ_T *prim = pnew(DM_DATA_FROM_HCI_REQ_T);

    prim->type = DM_DATA_FROM_HCI_REQ;
    prim->controller = controller;
    prim->physical_handle = physical_handle;
    prim->logical_handle = logical_handle;
    prim->data = data;

    if (pp_prim == NULL)
        CsrSchedMessagePut(DM_HCI_IFACEQUEUE, DM_PRIM, prim);
    else
        *pp_prim = (DM_UPRIM_T*)prim;
}

#endif /* CSR_BT_EXCLUDE_DATA_FROM_HCI */
