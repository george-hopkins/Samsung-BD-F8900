#ifndef CSR_BT_OBEX_SYNC_DEMO_COMMON_H__
#define CSR_BT_OBEX_SYNC_DEMO_COMMON_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_obex.h"
#include "csr_bt_platform.h"

/* Delete a directory */
int recursiveRmDir(char* targetDir);

#ifdef __cplusplus
}
#endif

#endif
