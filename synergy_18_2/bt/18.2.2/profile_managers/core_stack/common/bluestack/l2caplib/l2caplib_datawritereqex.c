/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_DataWriteReqEx.c

\brief  Build and send an L2CA_DATAWRITE_REQ primitive with context to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_sched.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"


/*! \brief Send L2CA_DATAWRITE_REQ

    Build and send an L2CA_DATAWRITE_REQ primitive with context to L2CAP.
    See \ref L2CA_DATAWRITE_REQ_T for more details.
*/
void L2CA_DataWriteReqEx(l2ca_cid_t cid,
                         CsrUint16 length,
                         void *p_data,
                         CsrUint16 context)
{
    L2CA_DATAWRITE_REQ_T *prim = pnew(L2CA_DATAWRITE_REQ_T);

    prim->type = L2CA_DATAWRITE_REQ;
    prim->cid = cid;
    prim->length = 0;
    prim->packets_ack = 0;
    prim->data = L2CA_MblkWrap(p_data, length);
    prim->req_ctx = context;

    L2CA_PutMsg(prim);
}
