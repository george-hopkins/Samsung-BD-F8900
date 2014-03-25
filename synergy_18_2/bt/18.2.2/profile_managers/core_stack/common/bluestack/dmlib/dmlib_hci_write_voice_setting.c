/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_hci_write_voice_setting.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_hci_write_voice_setting
 *
 *  DESCRIPTION
 *      Build and send a DM_HCI_WRITE_VOICE_SETTING_REQ primitive.
 *      If pp_prim != NULL, primitive is returned, not sent.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/


#ifndef CSR_BT_EXCLUDE_HCI_WRITE_VOICE_SETTING
void dm_hci_write_voice_setting(
    CsrUint16 voice_setting,
    DM_UPRIM_T **pp_prim
    )
{
    DM_HCI_WRITE_VOICE_SETTING_REQ_T *p_prim = zpnew(DM_HCI_WRITE_VOICE_SETTING_REQ_T);

    p_prim->common.op_code = DM_HCI_WRITE_VOICE_SETTING_REQ;
    p_prim->voice_setting = voice_setting;

    if (pp_prim)
    {
        *pp_prim = (DM_UPRIM_T *) p_prim;
    }
    else
    {
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, p_prim);
    }
}


#endif /* CSR_BT_EXCLUDE_HCI_WRITE_VOICE_SETTING */
