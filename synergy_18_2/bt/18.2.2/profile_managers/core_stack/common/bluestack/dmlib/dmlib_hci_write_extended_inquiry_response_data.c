/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_extended_inquiry_response_data.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_extended_inquiry_response_data
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA
void dm_hci_write_extended_inquiry_response_data(
    CsrUint8     fec_required,
    CsrUint8     eir_data_length,
    CsrUint8     *eir_data,
    DM_UPRIM_T **pp_prim
    )
{
    CsrUint8 i, octets_copied = 0, remainder;
    DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T *p_prim = zpnew(DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ_T);

    p_prim->common.op_code  = DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ;
    p_prim->fec_required    = fec_required;

    /* Make sure the length of the EIR-data does not exceed allowed limits */
    if (eir_data_length > HCI_EIR_DATA_LENGTH)
    {
        eir_data_length = HCI_EIR_DATA_LENGTH;
    }

    /* Allocate and fill whole blocks */
    for (i = 0; i < (eir_data_length / HCI_EIR_DATA_BYTES_PER_PTR); i++)
    {
        p_prim->eir_data_part[i] = CsrPmemAlloc(HCI_EIR_DATA_BYTES_PER_PTR);
        CsrMemCpy(p_prim->eir_data_part[i], eir_data + octets_copied, HCI_EIR_DATA_BYTES_PER_PTR);
        octets_copied += HCI_EIR_DATA_BYTES_PER_PTR;
    }

    /* Allocate and fill at most one remaining block */
    remainder = eir_data_length % HCI_EIR_DATA_BYTES_PER_PTR;

    if (remainder)
    {
        /* If eir_data_length < HCI_EIR_DATA_BYTES_PER_PTR this will be the first (and only) block allocated */
        p_prim->eir_data_part[i] = CsrPmemAlloc(HCI_EIR_DATA_BYTES_PER_PTR);
        CsrMemCpy(p_prim->eir_data_part[i], eir_data + octets_copied, remainder);

        /* Zero initialise rest of the block */
        CsrMemSet(p_prim->eir_data_part[i] + remainder, 0, HCI_EIR_DATA_BYTES_PER_PTR - remainder);
        i++;
    }

    /* Null initialise empty blocks - i has the correct value at this point */
    for (; i < HCI_EIR_DATA_PACKET_PTRS; i++)
    {
        p_prim->eir_data_part[i] = NULL;
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


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA */
