/****************************************************************************

                (c) CSR plc 2010

                All rights reserved

FILE:           dmlib_free_primitive.h

DESCRIPTION:    Helper function for freeing DM primitives.



****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_tasks.h"

#include "dm_prim.h"
#include "bluestacklib.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_free_primitive
 *
 *  DESCRIPTION
 *      Free downstream and upstream DM primitives, including any referenced memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

void dm_free_primitive(
    DM_UPRIM_T *p_uprim
    )
{
    if (!p_uprim)
        return;

    switch (p_uprim->type)
    {
        case DM_SM_LINK_KEY_REQUEST_RSP:
        case DM_SM_IO_CAPABILITY_REQUEST_RSP:
        case DM_HCI_WRITE_STORED_LINK_KEY_REQ:
        case DM_HCI_CHANGE_LOCAL_NAME_REQ:
        case DM_HCI_WRITE_CURRENT_IAC_LAP_REQ:
        case DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ:
        case DM_LP_WRITE_ROLESWITCH_POLICY_REQ:
        {
            dm_free_downstream_primitive(p_uprim);
            break;
        }

        case DM_HCI_INQUIRY_RESULT_IND:
        case DM_HCI_INQUIRY_RESULT_WITH_RSSI_IND:
        case DM_HCI_RETURN_LINK_KEYS_IND:
        case DM_HCI_REMOTE_NAME_CFM:
        case DM_HCI_READ_LOCAL_NAME_CFM:
        case DM_HCI_READ_CURRENT_IAC_LAP_CFM:
        case DM_HCI_READ_EXTENDED_INQUIRY_RESPONSE_DATA_CFM:
        case DM_HCI_EXTENDED_INQUIRY_RESULT_IND:
        case DM_SM_READ_LOCAL_OOB_DATA_CFM:
#ifdef INSTALL_ULP
        case DM_HCI_ULP_ADVERTISING_REPORT_IND:
#endif
        case DM_BAD_MESSAGE_IND:
#ifdef BUILD_FOR_HOST
        case DM_DATA_TO_HCI_IND:
#endif
        case DM_SM_READ_DEVICE_CFM:
        case DM_SM_KEYS_IND:
        {
            dm_free_upstream_primitive(p_uprim);
            break;
        }

        default:
        {
            CsrPmemFree(p_uprim);
            break;
        }
    }
}

