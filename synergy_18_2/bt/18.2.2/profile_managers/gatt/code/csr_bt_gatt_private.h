#ifndef _CSR_BT_GATT_PRIVATE_H_
#define _CSR_BT_GATT_PRIVATE_H_

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_message_queue.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_util.h"
#include "csr_bt_tasks.h"
#include "att_prim.h"
#include "attlib.h"
#include "tbdaddr.h"
#include "l2cap_prim.h"
#include "l2caplib.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "csr_bt_gatt_prim.h"
#include "csr_bt_gatt_lib.h"
#include "csr_bt_gatt_utils.h"
#include "csr_bt_gatt_private_prim.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_uuids.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_gatt_sc_sef.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_list.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_sdc_support.h"
#include "csr_bt_gatt_main.h"
#include "csr_bt_gatt_conn_genfsm.h"
#include "csr_bt_gatt_sef.h"
#include "csr_bt_gatt_att_sef.h"
#include "csr_bt_gatt_cm_sef.h"
#include "csr_bt_gatt_sd_sef.h"
#include "csr_bt_gatt_sef.h"
#include "csr_bt_gatt_upstream.h"
#include "csr_bt_gatt_private_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Empty */

#ifdef __cplusplus
}
#endif

#endif
