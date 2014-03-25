/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_read_tx_power_level.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_read_tx_power_level
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_READ_TX_POWER_LEVEL_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL
void dm_hci_read_tx_power_level(
    TYPED_BD_ADDR_T *addrt,
    CsrUint8 type,       /* 0=current 1=Max */
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_READ_TX_POWER_LEVEL_REQ_T *p_prim = zpnew(DM_HCI_READ_TX_POWER_LEVEL_REQ_T);

    p_prim->common.op_code = DM_HCI_READ_TX_POWER_LEVEL_REQ;
    tbdaddr_copy(&p_prim->addrt, addrt);
    p_prim->type = type;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_READ_TX_POWER_LEVEL */
