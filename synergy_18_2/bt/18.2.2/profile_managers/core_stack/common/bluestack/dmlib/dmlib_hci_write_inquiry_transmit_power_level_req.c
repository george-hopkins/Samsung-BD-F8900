/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_inquiry_transmit_power_level_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_inquiry_transmit_power_level_req
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ
 *      primitive. If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

#ifndef CSR_BT_EXCLUDE_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL
void dm_hci_write_inquiry_transmit_power_level_req(
    CsrInt8 tx_power,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T *prim
        = zpnew(DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ_T);

    prim->common.op_code = DM_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL_REQ;
    prim->tx_power = tx_power;

    if (pp_prim)
        *pp_prim = (DM_UPRIM_T*)prim;
    else
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
}

#endif /* CSR_BT_EXCLUDE_HCI_WRITE_INQUIRY_TRANSMIT_POWER_LEVEL */
