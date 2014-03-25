/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_set_event_filter.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_set_event_filter
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_SET_EVENT_FILTER_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_SET_EVENT_FILTER
void dm_hci_set_event_filter(
    filter_type_t filter_type,
    filter_condition_type_t filter_condition_type,
    CONDITION_T *p_condition,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_SET_EVENT_FILTER_REQ_T *p_prim = zpnew(DM_HCI_SET_EVENT_FILTER_REQ_T);

    p_prim->common.op_code = DM_HCI_SET_EVENT_FILTER_REQ;
    p_prim->filter_condition_type = filter_condition_type;
    p_prim->filter_type = filter_type;
    CsrMemCpy(&p_prim->condition, p_condition, sizeof(CONDITION_T));

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_SET_EVENT_FILTER */
