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
#include "csr_bt_therm_srv_converter_init.h"
#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE
#include "csr_bt_therm_srv_serialize.h"
#include "csr_bt_therm_srv_prim.h"

static CsrMsgConvMsgEntry csr_bt_therm_srv_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM
    { CSR_BT_THERM_SRV_ACTIVATE_CFM, CsrBtThermSrvActivateCfmSizeof, CsrBtThermSrvActivateCfmSer, CsrBtThermSrvActivateCfmDes, CsrBtThermSrvActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
    { CSR_BT_THERM_SRV_ACTIVATE_REQ, CsrBtThermSrvActivateReqSizeof, CsrBtThermSrvActivateReqSer, CsrBtThermSrvActivateReqDes, CsrBtThermSrvActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND
    { CSR_BT_THERM_SRV_CONNECT_IND, CsrBtThermSrvConnectIndSizeof, CsrBtThermSrvConnectIndSer, CsrBtThermSrvConnectIndDes, CsrBtThermSrvConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
    { CSR_BT_THERM_SRV_DEACTIVATE_CFM, CsrBtThermSrvDeactivateCfmSizeof, CsrBtThermSrvDeactivateCfmSer, CsrBtThermSrvDeactivateCfmDes, CsrBtThermSrvDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ
    { CSR_BT_THERM_SRV_DEACTIVATE_REQ, CsrBtThermSrvDeactivateReqSizeof, CsrBtThermSrvDeactivateReqSer, CsrBtThermSrvDeactivateReqDes, CsrBtThermSrvDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND
    { CSR_BT_THERM_SRV_DISCONNECT_IND, CsrBtThermSrvDisconnectIndSizeof, CsrBtThermSrvDisconnectIndSer, CsrBtThermSrvDisconnectIndDes, CsrBtThermSrvDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM
    { CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM, CsrBtThermSrvUpdateBattLevelCfmSizeof, CsrBtThermSrvUpdateBattLevelCfmSer, CsrBtThermSrvUpdateBattLevelCfmDes, CsrBtThermSrvUpdateBattLevelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ
    { CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ, CsrBtThermSrvUpdateBattLevelReqSizeof, CsrBtThermSrvUpdateBattLevelReqSer, CsrBtThermSrvUpdateBattLevelReqDes, CsrBtThermSrvUpdateBattLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM
    { CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM, CsrBtThermSrvUpdateHealthInfoCfmSizeof, CsrBtThermSrvUpdateHealthInfoCfmSer, CsrBtThermSrvUpdateHealthInfoCfmDes, CsrBtThermSrvUpdateHealthInfoCfmSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
    { CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ, CsrBtThermSrvUpdateHealthInfoReqSizeof, CsrBtThermSrvUpdateHealthInfoReqSer, CsrBtThermSrvUpdateHealthInfoReqDes, CsrBtThermSrvUpdateHealthInfoReqSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM
    { CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM, CsrBtThermSrvUpdateTemperatureCfmSizeof, CsrBtThermSrvUpdateTemperatureCfmSer, CsrBtThermSrvUpdateTemperatureCfmDes, CsrBtThermSrvUpdateTemperatureCfmSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
    { CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ, CsrBtThermSrvUpdateTemperatureReqSizeof, CsrBtThermSrvUpdateTemperatureReqSer, CsrBtThermSrvUpdateTemperatureReqDes, CsrBtThermSrvUpdateTemperatureReqSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */
#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
    { CSR_BT_THERM_SRV_WRITE_EVENT_IND, CsrBtThermSrvWriteEventIndSizeof, CsrBtThermSrvWriteEventIndSer, CsrBtThermSrvWriteEventIndDes, CsrBtThermSrvWriteEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtThermSrvConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_THERM_SRV_PRIM, csr_bt_therm_srv_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_therm_srv_conv_info = {
    CSR_BT_THERM_SRV_PRIM,
    "CSR_BT_THERM_SRV_PRIM",
    csr_bt_therm_srv_conv_lut
};

CsrLogPrimitiveInformation* CsrBtThermSrvTechInfoGet(void)
{
    return &csr_bt_therm_srv_conv_info;
}
#endif
#endif

