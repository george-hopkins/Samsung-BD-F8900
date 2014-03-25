/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_ampm_converter_init.h"
#ifndef EXCLUDE_CSR_BT_AMPM_MODULE
#include "csr_bt_ampm_serialize.h"
#include "csr_bt_ampm_prim.h"

static CsrMsgConvMsgEntry csr_bt_ampm_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND
    { CSR_BT_AMPM_AUTO_MOVE_CMP_IND, CsrBtAmpmAutoMoveCmpIndSizeof, CsrBtAmpmAutoMoveCmpIndSer, CsrBtAmpmAutoMoveCmpIndDes, CsrBtAmpmAutoMoveCmpIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
    { CSR_BT_AMPM_CONTROLLER_CFM, CsrBtAmpmControllerCfmSizeof, CsrBtAmpmControllerCfmSer, CsrBtAmpmControllerCfmDes, CsrBtAmpmControllerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
    { CSR_BT_AMPM_CONTROLLER_IND, CsrBtAmpmControllerIndSizeof, CsrBtAmpmControllerIndSer, CsrBtAmpmControllerIndDes, CsrBtAmpmControllerIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ
    { CSR_BT_AMPM_CONTROLLER_REQ, CsrBtAmpmControllerReqSizeof, CsrBtAmpmControllerReqSer, CsrBtAmpmControllerReqDes, CsrBtAmpmControllerReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM
    { CSR_BT_AMPM_DEREGISTER_CFM, CsrBtAmpmDeregisterCfmSizeof, CsrBtAmpmDeregisterCfmSer, CsrBtAmpmDeregisterCfmDes, CsrBtAmpmDeregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ
    { CSR_BT_AMPM_DEREGISTER_REQ, CsrBtAmpmDeregisterReqSizeof, CsrBtAmpmDeregisterReqSer, CsrBtAmpmDeregisterReqDes, CsrBtAmpmDeregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CFM
    { CSR_BT_AMPM_MOVE_CFM, CsrBtAmpmMoveCfmSizeof, CsrBtAmpmMoveCfmSer, CsrBtAmpmMoveCfmDes, CsrBtAmpmMoveCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND
    { CSR_BT_AMPM_MOVE_CMP_IND, CsrBtAmpmMoveCmpIndSizeof, CsrBtAmpmMoveCmpIndSer, CsrBtAmpmMoveCmpIndDes, CsrBtAmpmMoveCmpIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_IND
    { CSR_BT_AMPM_MOVE_IND, CsrBtAmpmMoveIndSizeof, CsrBtAmpmMoveIndSer, CsrBtAmpmMoveIndDes, CsrBtAmpmMoveIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_REQ
    { CSR_BT_AMPM_MOVE_REQ, CsrBtAmpmMoveReqSizeof, CsrBtAmpmMoveReqSer, CsrBtAmpmMoveReqDes, CsrBtAmpmMoveReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_RES
    { CSR_BT_AMPM_MOVE_RES, CsrBtAmpmMoveResSizeof, CsrBtAmpmMoveResSer, CsrBtAmpmMoveResDes, CsrBtAmpmMoveResSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_RES */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM
    { CSR_BT_AMPM_PAL_DEREGISTER_CFM, CsrBtAmpmPalDeregisterCfmSizeof, CsrBtAmpmPalDeregisterCfmSer, CsrBtAmpmPalDeregisterCfmDes, CsrBtAmpmPalDeregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ
    { CSR_BT_AMPM_PAL_DEREGISTER_REQ, CsrBtAmpmPalDeregisterReqSizeof, CsrBtAmpmPalDeregisterReqSer, CsrBtAmpmPalDeregisterReqDes, CsrBtAmpmPalDeregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
    { CSR_BT_AMPM_PAL_HCI_COMMAND_IND, CsrBtAmpmPalHciCommandIndSizeof, CsrBtAmpmPalHciCommandIndSer, CsrBtAmpmPalHciCommandIndDes, CsrBtAmpmPalHciCommandIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
    { CSR_BT_AMPM_PAL_HCI_EVENT_REQ, CsrBtAmpmPalHciEventReqSizeof, CsrBtAmpmPalHciEventReqSer, CsrBtAmpmPalHciEventReqDes, CsrBtAmpmPalHciEventReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM
    { CSR_BT_AMPM_PAL_REGISTER_CFM, CsrBtAmpmPalRegisterCfmSizeof, CsrBtAmpmPalRegisterCfmSer, CsrBtAmpmPalRegisterCfmDes, CsrBtAmpmPalRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ
    { CSR_BT_AMPM_PAL_REGISTER_REQ, CsrBtAmpmPalRegisterReqSizeof, CsrBtAmpmPalRegisterReqSer, CsrBtAmpmPalRegisterReqDes, CsrBtAmpmPalRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_IND
    { CSR_BT_AMPM_POWER_ON_IND, CsrBtAmpmPowerOnIndSizeof, CsrBtAmpmPowerOnIndSer, CsrBtAmpmPowerOnIndDes, CsrBtAmpmPowerOnIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_RES
    { CSR_BT_AMPM_POWER_ON_RES, CsrBtAmpmPowerOnResSizeof, CsrBtAmpmPowerOnResSer, CsrBtAmpmPowerOnResDes, CsrBtAmpmPowerOnResSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_RES */
#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
    { CSR_BT_AMPM_PRF_MULTI_CONN_REQ, CsrBtAmpmPrfMultiConnReqSizeof, CsrBtAmpmPrfMultiConnReqSer, CsrBtAmpmPrfMultiConnReqDes, CsrBtAmpmPrfMultiConnReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_CFM
    { CSR_BT_AMPM_REGISTER_CFM, CsrBtAmpmRegisterCfmSizeof, CsrBtAmpmRegisterCfmSer, CsrBtAmpmRegisterCfmDes, CsrBtAmpmRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ
    { CSR_BT_AMPM_REGISTER_POWER_ON_REQ, CsrBtAmpmRegisterPowerOnReqSizeof, CsrBtAmpmRegisterPowerOnReqSer, CsrBtAmpmRegisterPowerOnReqDes, CsrBtAmpmRegisterPowerOnReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_REQ
    { CSR_BT_AMPM_REGISTER_REQ, CsrBtAmpmRegisterReqSizeof, CsrBtAmpmRegisterReqSer, CsrBtAmpmRegisterReqDes, CsrBtAmpmRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ
    { CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ, CsrBtAmpmTestCancelCreatePhysicalLinkReqSizeof, CsrBtAmpmTestCancelCreatePhysicalLinkReqSer, CsrBtAmpmTestCancelCreatePhysicalLinkReqDes, CsrBtAmpmTestCancelCreatePhysicalLinkReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ
    { CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ, CsrBtAmpmTestChangeAmpKeyReqSizeof, CsrBtAmpmTestChangeAmpKeyReqSer, CsrBtAmpmTestChangeAmpKeyReqDes, CsrBtAmpmTestChangeAmpKeyReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ
    { CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ, CsrBtAmpmTestDontStartLogLinkReqSizeof, CsrBtAmpmTestDontStartLogLinkReqSer, CsrBtAmpmTestDontStartLogLinkReqDes, CsrBtAmpmTestDontStartLogLinkReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ
    { CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ, CsrBtAmpmTestDontStartPhyLinkReqSizeof, CsrBtAmpmTestDontStartPhyLinkReqSer, CsrBtAmpmTestDontStartPhyLinkReqDes, CsrBtAmpmTestDontStartPhyLinkReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ
    { CSR_BT_AMPM_TEST_GET_ASSOC_REQ, CsrBtAmpmTestGetAssocReqSizeof, CsrBtAmpmTestGetAssocReqSer, CsrBtAmpmTestGetAssocReqDes, CsrBtAmpmTestGetAssocReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
    { CSR_BT_AMPM_TEST_HCI_IND, CsrBtAmpmTestHciIndSizeof, CsrBtAmpmTestHciIndSer, CsrBtAmpmTestHciIndDes, CsrBtAmpmTestHciIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
    { CSR_BT_AMPM_TEST_HCI_REQ, CsrBtAmpmTestHciReqSizeof, CsrBtAmpmTestHciReqSer, CsrBtAmpmTestHciReqDes, CsrBtAmpmTestHciReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ
    { CSR_BT_AMPM_TEST_IGNORE_SDU_REQ, CsrBtAmpmTestIgnoreSduReqSizeof, CsrBtAmpmTestIgnoreSduReqSer, CsrBtAmpmTestIgnoreSduReqDes, CsrBtAmpmTestIgnoreSduReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ
    { CSR_BT_AMPM_TEST_PHY_COLLISION_REQ, CsrBtAmpmTestPhyCollisionReqSizeof, CsrBtAmpmTestPhyCollisionReqSer, CsrBtAmpmTestPhyCollisionReqDes, CsrBtAmpmTestPhyCollisionReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
    { CSR_BT_AMPM_TEST_RECEIVED_SDU_IND, CsrBtAmpmTestReceivedSduIndSizeof, CsrBtAmpmTestReceivedSduIndSer, CsrBtAmpmTestReceivedSduIndDes, CsrBtAmpmTestReceivedSduIndSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ
    { CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ, CsrBtAmpmTestRouteReceivedSduReqSizeof, CsrBtAmpmTestRouteReceivedSduReqSer, CsrBtAmpmTestRouteReceivedSduReqDes, CsrBtAmpmTestRouteReceivedSduReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
    { CSR_BT_AMPM_TEST_SDU_REQ, CsrBtAmpmTestSduReqSizeof, CsrBtAmpmTestSduReqSer, CsrBtAmpmTestSduReqDes, CsrBtAmpmTestSduReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */
#ifndef EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ
    { CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ, CsrBtAmpmTestTimerControlReqSizeof, CsrBtAmpmTestTimerControlReqSer, CsrBtAmpmTestTimerControlReqDes, CsrBtAmpmTestTimerControlReqSerFree },
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtAmpmConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_AMPM_PRIM, csr_bt_ampm_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_ampm_conv_info = {
    CSR_BT_AMPM_PRIM,
    "CSR_BT_AMPM_PRIM",
    csr_bt_ampm_conv_lut
};

CsrLogPrimitiveInformation* CsrBtAmpmTechInfoGet(void)
{
    return &csr_bt_ampm_conv_info;
}
#endif
#endif

