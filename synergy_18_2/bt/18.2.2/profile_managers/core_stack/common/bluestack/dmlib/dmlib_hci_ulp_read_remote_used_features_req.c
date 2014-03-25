/*!
        (c) CSR plc 2010
        All rights reserved

\file   dm_hci_ulp_read_remote_used_features_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_ulp_read_remote_used_features_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_ULP_READ_REMOTE_USED_FEATURES
void dm_hci_ulp_read_remote_used_features_req(
    TYPED_BD_ADDR_T *addrt,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T *prim = zpnew(DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ_T);

    prim->common.op_code = DM_HCI_ULP_READ_REMOTE_USED_FEATURES_REQ;

    tbdaddr_copy(&prim->addrt, addrt);

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_ULP_READ_REMOTE_USED_FEATURES */
