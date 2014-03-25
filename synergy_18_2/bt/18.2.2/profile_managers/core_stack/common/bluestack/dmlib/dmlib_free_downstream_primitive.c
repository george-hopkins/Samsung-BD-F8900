/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_free_downstream_primitive.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"

/*----------------------------------------------------------------------------*
 *  NAME
 *      dm_free_downstream_primitive
 *
 *  DESCRIPTION
 *      Free downstream DM primitives, including any referenced memory.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void dm_free_downstream_primitive(DM_UPRIM_T *p_uprim)
{
    if (!p_uprim)
        return;

    /* Action taken depends on the primitive type */
    switch (p_uprim->type)
    {
        case DM_SM_LINK_KEY_REQUEST_RSP:
            CsrPmemFree(p_uprim->dm_sm_link_key_request_rsp.key);
            break;

        case DM_SM_IO_CAPABILITY_REQUEST_RSP:
            CsrPmemFree(p_uprim->dm_sm_io_capability_request_rsp.oob_hash_c);
            CsrPmemFree(p_uprim->dm_sm_io_capability_request_rsp.oob_rand_r);
            break;

        case DM_HCI_WRITE_STORED_LINK_KEY_REQ:
            pdestroy_array((void **) p_uprim->dm_write_stored_link_key_req.link_key_bd_addr, HCI_STORED_LINK_KEY_MAX);
            break;

        case DM_HCI_CHANGE_LOCAL_NAME_REQ:
            pdestroy_array((void **) p_uprim->dm_change_local_name_req.name_part, HCI_LOCAL_NAME_BYTE_PACKET_PTRS);
            break;

        case DM_HCI_WRITE_CURRENT_IAC_LAP_REQ:
            pdestroy_array((void **) p_uprim->dm_write_current_iac_lap_req.iac_lap, HCI_IAC_LAP_PTRS);
            break;

        case DM_HCI_WRITE_EXTENDED_INQUIRY_RESPONSE_DATA_REQ:
            pdestroy_array((void **) p_uprim->dm_write_extended_inquiry_response_data_req.eir_data_part, HCI_EIR_DATA_PACKET_PTRS);
            break;

        case DM_LP_WRITE_ROLESWITCH_POLICY_REQ:
            CsrPmemFree(p_uprim->dm_lp_write_roleswitch_policy_req.rs_table);
            break;

        default:
            break;
    }

    CsrPmemFree(p_uprim);
}
