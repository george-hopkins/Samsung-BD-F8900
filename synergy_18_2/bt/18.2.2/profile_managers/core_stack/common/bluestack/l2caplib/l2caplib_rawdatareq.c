/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_RawDataReq.c

\brief  Build and send a L2CA_RAW_DATA_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_RAW_DATA_REQ

    Build and send a L2CA_RAW_DATA_REQ primitive to L2CAP.
    See \ref L2CA_RAW_DATA_REQ_T for more details.
*/
void L2CA_RawDataReq(l2ca_cid_t cid,
                     CsrUint16 length,
                     void *p_data,
                     CsrUint16 raw_length,
                     CsrUint16 flush_to)
{
    L2CA_RAW_DATA_REQ_T *prim = pnew(L2CA_RAW_DATA_REQ_T);

    prim->type = L2CA_RAW_DATA_REQ;
    prim->cid = cid;
    prim->length = length;
    prim->raw_length = raw_length;
    prim->flush_to = flush_to;
    prim->data = L2CA_MblkWrap(p_data, length);

    L2CA_PutMsg(prim);
}
