/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_inquiry.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"


/*============================================================================*
    Private Data Types
 *============================================================================*/
/* None */

/*============================================================================*
    Private Data
 *============================================================================*/
/* None */

/*============================================================================*
    Private Function Prototypes
 *============================================================================*/
/* None */

/*============================================================================*
    Public Function Implementations
 *============================================================================*/


/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_inquiry
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_INQUIRY_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_INQUIRY
void dm_hci_inquiry(
    CsrUint24 lap,
    CsrUint8 inquiry_length,
    CsrUint8 num_responses,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_INQUIRY_REQ_T *p_prim = zpnew(DM_HCI_INQUIRY_REQ_T);

    p_prim->common.op_code = DM_HCI_INQUIRY_REQ;
    p_prim->lap = lap;
    p_prim->inquiry_length = inquiry_length;
    p_prim->num_responses = num_responses;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_INQUIRY */
