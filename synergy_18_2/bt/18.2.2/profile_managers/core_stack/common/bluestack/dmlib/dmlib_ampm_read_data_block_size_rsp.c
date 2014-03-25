/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_read_data_block_size_rsp.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_READ_DATA_BLOCK_SIZE
void dm_ampm_read_data_block_size_rsp(const l2ca_controller_t local_amp_id,
                                      const CsrUint8 status,
                                      const CsrBool fragmentable,
                                      const CsrUint16 max_pdu_length,
                                      const CsrUint16 max_acl_data_packet_length,
                                      const CsrUint16 data_block_length,
                                      const CsrUint16 total_num_data_blocks)
{
    DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *p_prim = pnew(DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T);

    p_prim->local_amp_id = local_amp_id;
    p_prim->status = status;
    p_prim->fragmentable = fragmentable;
    p_prim->max_pdu_length = max_pdu_length;
    p_prim->max_acl_data_packet_length = max_acl_data_packet_length;
    p_prim->data_block_length = data_block_length;
    p_prim->total_num_data_blocks = total_num_data_blocks;

    dm_put_message(p_prim, DM_AMPM_READ_DATA_BLOCK_SIZE_RSP);
}

#endif /* CSR_BT_EXCLUDE_AMPM_READ_DATA_BLOCK_SIZE */
