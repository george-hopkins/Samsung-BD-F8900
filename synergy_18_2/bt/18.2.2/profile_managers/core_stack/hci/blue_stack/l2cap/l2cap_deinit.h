/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_deinit.h

\brief  This file defines the interface for deinitialisation of L2CAP.
*/
#ifdef ENABLE_SHUTDOWN
#ifndef _L2CAP_DEINIT_H
#define _L2CAP_DEINIT_H

#include "csr_synergy.h"

#include "csr_mblk.h"
#include "csr_bt_common.h"
#include "l2cap_types.h"
#include "dm_acl_core.h"

/* ACL manager callback */
extern void dm_acl_client_deinit_l2cap(L2CAP_CHCB_T *chcb);

/* Scheduler shutdown */
extern void L2CAP_Deinit(void **gash);

#ifdef __cplusplus
}
#endif
#endif
#endif
