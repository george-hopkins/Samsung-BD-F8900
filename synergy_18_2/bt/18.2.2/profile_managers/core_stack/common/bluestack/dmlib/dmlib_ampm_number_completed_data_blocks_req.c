/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_ampm_number_completed_data_blocks_req.c
*/

#include "csr_synergy.h"

#include "dmlib_private.h"



#ifndef CSR_BT_EXCLUDE_AMPM_NUMBER_COMPLETED_DATA_BLOCKS
void dm_ampm_number_completed_data_blocks_req(const l2ca_controller_t local_amp_id,
                                              const CsrUint16 total_num_data_blocks,
                                              const CsrUint8 number_of_handles,
                                              DM_AMPM_NCB_T **num_completed_blks_ptr)
{
    DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T *p_prim = pnew(DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ_T);

    p_prim->local_amp_id = local_amp_id;
    p_prim->total_num_data_blocks = total_num_data_blocks;
    p_prim->number_of_handles = number_of_handles;

    CsrMemCpy(p_prim->num_completed_blks_ptr, num_completed_blks_ptr, sizeof(p_prim->num_completed_blks_ptr));

    dm_put_message(p_prim, DM_AMPM_NUMBER_COMPLETED_DATA_BLOCKS_REQ);
}

#endif /* CSR_BT_EXCLUDE_AMPM_NUMBER_COMPLETED_DATA_BLOCKS */
