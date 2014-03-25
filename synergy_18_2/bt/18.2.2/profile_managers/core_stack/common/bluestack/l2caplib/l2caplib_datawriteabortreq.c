/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2caplib_DataWriteAbortReq.c

\brief  Build and send a L2CA_DATAWRITE_ABORT_REQ primitive to L2CAP.
*/

#include "csr_synergy.h"

#include "l2caplib_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_core_stack_pmalloc.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"

/*! \brief Send L2CA_DATAWRITE_ABORT_REQ

    Build and send a L2CA_DATAWRITE_ABORT_REQ primitive to L2CAP.
    See \ref L2CA_DATAWRITE_ABORT_REQ_T for more details.
*/
void L2CA_DataWriteAbortReq(l2ca_cid_t cid)
{
    L2CA_DATAWRITE_ABORT_REQ_T *prim = pnew(L2CA_DATAWRITE_ABORT_REQ_T);

    prim->type = L2CA_DATAWRITE_ABORT_REQ;
    prim->cid = cid;

    L2CA_PutMsg(prim);
}
