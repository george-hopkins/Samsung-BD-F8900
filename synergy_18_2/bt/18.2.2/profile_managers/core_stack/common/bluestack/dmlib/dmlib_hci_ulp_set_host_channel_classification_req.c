/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_set_host_channel_classification_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_set_host_channel_classification_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION
void dm_hci_ulp_set_host_channel_classification_req(
    CsrUint8 *channel_map,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T *prim = zpnew(DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ_T);

    prim->common.op_code = DM_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION_REQ;
    CsrMemCpy(prim->channel_map, channel_map, 5 * sizeof(CsrUint8));

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_SET_HOST_CHANNEL_CLASSIFICATION */
