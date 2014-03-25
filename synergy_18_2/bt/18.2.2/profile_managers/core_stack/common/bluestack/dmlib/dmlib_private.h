/*!
        (c) CSR plc 2010
        All rights reserved

\file   dmlib_private.h

\brief  dmlib private header file
*/

#ifndef __DMLIB_PRIVATE_H__
#define __DMLIB_PRIVATE_H__

#include "csr_synergy.h"

#include "dmlib.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "dm_prim.h"

#include "csr_util.h"
#include "csr_bt_tasks.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_mblk.h"
#include "tbdaddr.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#else
#include "csr_bt_core_stack_pmalloc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void dm_put_message(void *p_prim, const dm_prim_t type);

#ifdef __cplusplus
}
#endif

#endif /* __DMLIB_PRIVATE_H__ */
