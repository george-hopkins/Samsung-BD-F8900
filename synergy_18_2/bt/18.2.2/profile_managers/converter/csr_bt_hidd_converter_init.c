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
#include "csr_bt_hidd_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HIDD_MODULE
#include "csr_bt_hidd_serialize.h"
#include "csr_bt_hidd_prim.h"

static CsrMsgConvMsgEntry csr_bt_hidd_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM
    { CSR_BT_HIDD_ACTIVATE_CFM, CsrBtHiddActivateCfmSizeof, CsrBtHiddActivateCfmSer, CsrBtHiddActivateCfmDes, CsrBtHiddActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
    { CSR_BT_HIDD_ACTIVATE_REQ, CsrBtHiddActivateReqSizeof, CsrBtHiddActivateReqSer, CsrBtHiddActivateReqDes, CsrBtHiddActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
    { CSR_BT_HIDD_CONTROL_IND, CsrBtHiddControlIndSizeof, CsrBtHiddControlIndSer, CsrBtHiddControlIndDes, CsrBtHiddControlIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
    { CSR_BT_HIDD_CONTROL_RES, CsrBtHiddControlResSizeof, CsrBtHiddControlResSer, CsrBtHiddControlResDes, CsrBtHiddControlResSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */
#ifndef EXCLUDE_CSR_BT_HIDD_DATA_CFM
    { CSR_BT_HIDD_DATA_CFM, CsrBtHiddDataCfmSizeof, CsrBtHiddDataCfmSer, CsrBtHiddDataCfmDes, CsrBtHiddDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
    { CSR_BT_HIDD_DATA_IND, CsrBtHiddDataIndSizeof, CsrBtHiddDataIndSer, CsrBtHiddDataIndDes, CsrBtHiddDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
    { CSR_BT_HIDD_DATA_REQ, CsrBtHiddDataReqSizeof, CsrBtHiddDataReqSer, CsrBtHiddDataReqDes, CsrBtHiddDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM
    { CSR_BT_HIDD_DEACTIVATE_CFM, CsrBtHiddDeactivateCfmSizeof, CsrBtHiddDeactivateCfmSer, CsrBtHiddDeactivateCfmDes, CsrBtHiddDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ
    { CSR_BT_HIDD_DEACTIVATE_REQ, CsrBtHiddDeactivateReqSizeof, CsrBtHiddDeactivateReqSer, CsrBtHiddDeactivateReqDes, CsrBtHiddDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND
    { CSR_BT_HIDD_MODE_CHANGE_IND, CsrBtHiddModeChangeIndSizeof, CsrBtHiddModeChangeIndSer, CsrBtHiddModeChangeIndDes, CsrBtHiddModeChangeIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ
    { CSR_BT_HIDD_MODE_CHANGE_REQ, CsrBtHiddModeChangeReqSizeof, CsrBtHiddModeChangeReqSer, CsrBtHiddModeChangeReqDes, CsrBtHiddModeChangeReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND
    { CSR_BT_HIDD_REACTIVATE_IND, CsrBtHiddReactivateIndSizeof, CsrBtHiddReactivateIndSer, CsrBtHiddReactivateIndDes, CsrBtHiddReactivateIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_RESTORE_IND
    { CSR_BT_HIDD_RESTORE_IND, CsrBtHiddRestoreIndSizeof, CsrBtHiddRestoreIndSer, CsrBtHiddRestoreIndDes, CsrBtHiddRestoreIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_RESTORE_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM
    { CSR_BT_HIDD_SECURITY_IN_CFM, CsrBtHiddSecurityInCfmSizeof, CsrBtHiddSecurityInCfmSer, CsrBtHiddSecurityInCfmDes, CsrBtHiddSecurityInCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ
    { CSR_BT_HIDD_SECURITY_IN_REQ, CsrBtHiddSecurityInReqSizeof, CsrBtHiddSecurityInReqSer, CsrBtHiddSecurityInReqDes, CsrBtHiddSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM
    { CSR_BT_HIDD_SECURITY_OUT_CFM, CsrBtHiddSecurityOutCfmSizeof, CsrBtHiddSecurityOutCfmSer, CsrBtHiddSecurityOutCfmDes, CsrBtHiddSecurityOutCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ
    { CSR_BT_HIDD_SECURITY_OUT_REQ, CsrBtHiddSecurityOutReqSizeof, CsrBtHiddSecurityOutReqSer, CsrBtHiddSecurityOutReqDes, CsrBtHiddSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_HIDD_STATUS_IND
    { CSR_BT_HIDD_STATUS_IND, CsrBtHiddStatusIndSizeof, CsrBtHiddStatusIndSer, CsrBtHiddStatusIndDes, CsrBtHiddStatusIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM
    { CSR_BT_HIDD_UNPLUG_CFM, CsrBtHiddUnplugCfmSizeof, CsrBtHiddUnplugCfmSer, CsrBtHiddUnplugCfmDes, CsrBtHiddUnplugCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM */
#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_IND
    { CSR_BT_HIDD_UNPLUG_IND, CsrBtHiddUnplugIndSizeof, CsrBtHiddUnplugIndSer, CsrBtHiddUnplugIndDes, CsrBtHiddUnplugIndSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_IND */
#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ
    { CSR_BT_HIDD_UNPLUG_REQ, CsrBtHiddUnplugReqSizeof, CsrBtHiddUnplugReqSer, CsrBtHiddUnplugReqDes, CsrBtHiddUnplugReqSerFree },
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHiddConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HIDD_PRIM, csr_bt_hidd_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hidd_conv_info = {
    CSR_BT_HIDD_PRIM,
    "CSR_BT_HIDD_PRIM",
    csr_bt_hidd_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHiddTechInfoGet(void)
{
    return &csr_bt_hidd_conv_info;
}
#endif
#endif

