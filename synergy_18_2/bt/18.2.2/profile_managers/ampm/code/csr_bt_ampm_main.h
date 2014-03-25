#ifndef CSR_BT_AMPM_MAIN_H
#define CSR_BT_AMPM_MAIN_H

#include "csr_synergy.h"
/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2012.
   
   All rights reserved.

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_list.h"
#include "bluestacklib.h"
#include "csr_mblk.h"
#include "l2cap_prim.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_ampm_prim.h"
#include "csr_bt_amp_hci.h"
#include "csr_bt_tasks.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_profiles.h"
#include "csr_bt_util.h"
#include "csr_bt_cmn_linked_list.h"
#include "csr_bt_ampm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_ampm_config.h"
#include "csr_bt_ampm_gen_list.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Assertion helpers */
#if !defined(__KERNEL__) && defined(_DEBUG)
#include <assert.h>
#define CSR_BT_AMPM_ASSERT(x) assert(x)
#endif
#ifndef CSR_BT_AMPM_ASSERT
#define CSR_BT_AMPM_ASSERT(x)
#endif

typedef struct
{
    void *msg;
    CsrUint16 msg_class;
    BD_ADDR_T local_bd_addr;
    CsrSchedQid ampPowerOnQid;
    CsrBool ampSspEnabled;
    CsrUint8 sduIdentifier;
    CsrBool phyHandleInUse[CSR_BT_AMPM_MAX_PHYSICAL_LINK_HANDLE];
    CstBtAmpmGenListAnchor *pPhyLinkListAnchor;
    CstBtAmpmGenListAnchor *pLogicalLinkListAnchor;
    CstBtAmpmGenListAnchor *pL2caChanListAnchor;
    CstBtAmpmGenListAnchor *pBTDeviceKeysAnchor;
    CstBtAmpmGenListAnchor *pBTDeviceListAnchor;

    CsrCmnList_t localAmpList;
    CsrCmnList_t appList;
    CsrCmnList_t discoveryList;
    CsrCmnList_t multiConnList;
    CsrUint8 lastAllocatedAmpPhysicalHandle;

    /* Used for testing */
    CsrBool sendCancelEnabled;
    CsrBool sendCancelAfterCreatePhyLinkRsp;
    CsrBool sendCancelWithInvalidResponderAmpId;
    CsrUint32 sendCancelDelayAfterCreatePhyLinkReq;
    CsrBool dontStartPhyLinkEnabled;
    CsrBtAmpmType dontStartPhyLinkType;
    CsrBool phyLinkCollisionEnabled;
    CsrBool dontStartLogicalLinkEnabled;
    CsrBool ampmTimersEnabled;
    CsrBool ampmIgnoreSduEnabled;
    CsrUint8 ampmIgnoreSduId;
    CsrUint8 ampmIgnoreSduCount;
    CsrUint8 ampmIgnoreSduTimes;
    CsrBool ampmChangeAmpKeyEnabled;
    CsrUint8 ampmChangeAmpKeyType;
    CsrUint8 ampmChangeAmpKeyCount;
    CsrUint8 ampmChangeAmpKeyTimes;
    CsrBtAmpmControl ampmRxSduControl;
    CsrSchedQid ampmRxSduQid;
} CsrBtAmpmInstance;

/* First parameter passed to the FSM function. */
typedef struct
{
    CsrBtAmpmInstance *inst;
    void *argp;
} CsrBtAmpmFsmParam;

extern void CsrBtAmpmHandler(CsrBtAmpmInstance * inst);
extern void CsrBtAmpmHciHandler(CsrBtAmpmInstance * inst);
extern void CsrBtAmpmDmHandler(CsrBtAmpmInstance * inst);
extern void CsrBtAmpmFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

#ifdef __cplusplus
}
#endif

/* Include local headers */
#include "csr_bt_ampm_btdev_genfsm.h"
#include "csr_bt_ampm_l2cap_genfsm.h"
#include "csr_bt_ampm_local_genfsm.h"
#include "csr_bt_ampm_logical_genfsm.h"
#include "csr_bt_ampm_phy_genfsm.h"
#include "csr_bt_ampm_data.h"
#include "csr_bt_ampm_app.h"
#include "csr_bt_ampm_btdev.h"
#include "csr_bt_ampm_local.h"
#include "csr_bt_ampm_phy.h"
#include "csr_bt_ampm_sdu.h"
#include "csr_bt_ampm_hmac_sha256.h"
#include "csr_bt_ampm_l2cap.h"
#include "csr_bt_ampm_logical.h"
#include "csr_bt_ampm_send_hci_command.h"
#include "csr_bt_ampm_send_upstream.h"

#endif /* CSR_BT_AMPM_MAIN_H */
