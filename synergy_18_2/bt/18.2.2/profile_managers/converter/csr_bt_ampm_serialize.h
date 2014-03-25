#ifndef CSR_BT_AMPM_SERIALIZE_H__
#define CSR_BT_AMPM_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtAmpmPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ
CsrSize CsrBtAmpmTestChangeAmpKeyReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestChangeAmpKeyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestChangeAmpKeyReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestChangeAmpKeyReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
CsrSize CsrBtAmpmControllerCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmControllerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmControllerCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmControllerCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ
CsrSize CsrBtAmpmTestPhyCollisionReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestPhyCollisionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestPhyCollisionReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestPhyCollisionReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ
CsrSize CsrBtAmpmPalRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPalRegisterReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_RES
CsrSize CsrBtAmpmPowerOnResSizeof(void *msg);
CsrUint8 *CsrBtAmpmPowerOnResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPowerOnResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPowerOnResSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_IND
CsrSize CsrBtAmpmPowerOnIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmPowerOnIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPowerOnIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPowerOnIndSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
CsrSize CsrBtAmpmTestHciIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestHciIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestHciIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmTestHciIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ
CsrSize CsrBtAmpmTestIgnoreSduReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestIgnoreSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestIgnoreSduReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestIgnoreSduReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ
CsrSize CsrBtAmpmTestRouteReceivedSduReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestRouteReceivedSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestRouteReceivedSduReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestRouteReceivedSduReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ
CsrSize CsrBtAmpmControllerReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmControllerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmControllerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmControllerReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ
CsrSize CsrBtAmpmTestCancelCreatePhysicalLinkReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestCancelCreatePhysicalLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestCancelCreatePhysicalLinkReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestCancelCreatePhysicalLinkReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ
CsrSize CsrBtAmpmTestDontStartLogLinkReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestDontStartLogLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestDontStartLogLinkReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestDontStartLogLinkReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ
CsrSize CsrBtAmpmDeregisterReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmDeregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmDeregisterReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ
CsrSize CsrBtAmpmRegisterPowerOnReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmRegisterPowerOnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmRegisterPowerOnReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmRegisterPowerOnReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_RES
CsrSize CsrBtAmpmMoveResSizeof(void *msg);
CsrUint8 *CsrBtAmpmMoveResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmMoveResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmMoveResSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
CsrSize CsrBtAmpmPrfMultiConnReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmPrfMultiConnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPrfMultiConnReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmPrfMultiConnReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
CsrSize CsrBtAmpmPalHciEventReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalHciEventReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalHciEventReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmPalHciEventReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ
CsrSize CsrBtAmpmTestDontStartPhyLinkReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestDontStartPhyLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestDontStartPhyLinkReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestDontStartPhyLinkReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM
CsrSize CsrBtAmpmDeregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmDeregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmDeregisterCfmSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ
CsrSize CsrBtAmpmTestGetAssocReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestGetAssocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestGetAssocReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestGetAssocReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM
CsrSize CsrBtAmpmPalDeregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalDeregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPalDeregisterCfmSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
CsrSize CsrBtAmpmPalHciCommandIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalHciCommandIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalHciCommandIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmPalHciCommandIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
CsrSize CsrBtAmpmTestReceivedSduIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestReceivedSduIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestReceivedSduIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmTestReceivedSduIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
CsrSize CsrBtAmpmControllerIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmControllerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmControllerIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmControllerIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
CsrSize CsrBtAmpmTestSduReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestSduReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmTestSduReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM
CsrSize CsrBtAmpmPalRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPalRegisterCfmSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CFM
CsrSize CsrBtAmpmMoveCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmMoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmMoveCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmMoveCfmSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND
CsrSize CsrBtAmpmMoveCmpIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmMoveCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmMoveCmpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmMoveCmpIndSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND
CsrSize CsrBtAmpmAutoMoveCmpIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmAutoMoveCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmAutoMoveCmpIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmAutoMoveCmpIndSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_REQ
CsrSize CsrBtAmpmRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmRegisterReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_REQ
CsrSize CsrBtAmpmMoveReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmMoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmMoveReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmMoveReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ
CsrSize CsrBtAmpmPalDeregisterReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmPalDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmPalDeregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmPalDeregisterReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
CsrSize CsrBtAmpmTestHciReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestHciReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestHciReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAmpmTestHciReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ
CsrSize CsrBtAmpmTestTimerControlReqSizeof(void *msg);
CsrUint8 *CsrBtAmpmTestTimerControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmTestTimerControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmTestTimerControlReqSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_CFM
CsrSize CsrBtAmpmRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtAmpmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmRegisterCfmSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_IND
CsrSize CsrBtAmpmMoveIndSizeof(void *msg);
CsrUint8 *CsrBtAmpmMoveIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAmpmMoveIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAmpmMoveIndSerFree CsrBtAmpmPfree
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_IND */

#ifdef __cplusplus
}
#endif

#endif
