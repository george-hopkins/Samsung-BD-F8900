/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_change_local_name.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_change_local_name
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_CHANGE_LOCAL_NAME_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_CHANGE_LOCAL_NAME
void dm_hci_change_local_name(
    CsrUint8 *sz_name,   /* Nul-terminated name string */
    DM_UPRIM_T **pp_prim
    )
{
    CsrUint16 len;
    CsrUint16 i;
    CsrUint8 block;

    DM_HCI_CHANGE_LOCAL_NAME_REQ_T *p_prim = zpnew(DM_HCI_CHANGE_LOCAL_NAME_REQ_T);
    p_prim->common.op_code = DM_HCI_CHANGE_LOCAL_NAME_REQ;

    /* Get the name length. Due to the way the following loop works, reduce the
     * length to 247 if the name is the maximum size.
     */
    len = (CsrUint16) CsrStrLen((char *)sz_name);
    if (len >= 248)
    {
        len = 247;
    }

    /* The following loop terminates after (len + 1) characters, to ensure that
     * there is always a terminating 0 character if the name length is an exact
     * multiple of the number of bytes per pointer.
     */
    for (block = 0, i = 0; i < len + 1; i += HCI_LOCAL_NAME_BYTES_PER_PTR)
    {
        p_prim->name_part[block] = (CsrUint8 *) CsrPmemAlloc(HCI_LOCAL_NAME_BYTES_PER_PTR);
        CsrStrNCpy((char *)p_prim->name_part[block], (char *)(sz_name + i), HCI_LOCAL_NAME_BYTES_PER_PTR);
        block++;
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


#endif /* CSR_BT_EXCLUDE_HCI_CHANGE_LOCAL_NAME */
