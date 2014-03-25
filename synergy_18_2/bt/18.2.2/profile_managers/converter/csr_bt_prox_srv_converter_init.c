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
#include "csr_bt_prox_srv_converter_init.h"
#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE
#include "csr_bt_prox_srv_serialize.h"
#include "csr_bt_prox_srv_prim.h"

static CsrMsgConvMsgEntry csr_bt_prox_srv_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM
    { CSR_BT_PROX_SRV_ACTIVATE_CFM, CsrBtProxSrvActivateCfmSizeof, CsrBtProxSrvActivateCfmSer, CsrBtProxSrvActivateCfmDes, CsrBtProxSrvActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
    { CSR_BT_PROX_SRV_ACTIVATE_REQ, CsrBtProxSrvActivateReqSizeof, CsrBtProxSrvActivateReqSer, CsrBtProxSrvActivateReqDes, CsrBtProxSrvActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND
    { CSR_BT_PROX_SRV_ALERT_EVENT_IND, CsrBtProxSrvAlertEventIndSizeof, CsrBtProxSrvAlertEventIndSer, CsrBtProxSrvAlertEventIndDes, CsrBtProxSrvAlertEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND
    { CSR_BT_PROX_SRV_CONNECT_IND, CsrBtProxSrvConnectIndSizeof, CsrBtProxSrvConnectIndSer, CsrBtProxSrvConnectIndDes, CsrBtProxSrvConnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
    { CSR_BT_PROX_SRV_DEACTIVATE_CFM, CsrBtProxSrvDeactivateCfmSizeof, CsrBtProxSrvDeactivateCfmSer, CsrBtProxSrvDeactivateCfmDes, CsrBtProxSrvDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ
    { CSR_BT_PROX_SRV_DEACTIVATE_REQ, CsrBtProxSrvDeactivateReqSizeof, CsrBtProxSrvDeactivateReqSer, CsrBtProxSrvDeactivateReqDes, CsrBtProxSrvDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND
    { CSR_BT_PROX_SRV_DISCONNECT_IND, CsrBtProxSrvDisconnectIndSizeof, CsrBtProxSrvDisconnectIndSer, CsrBtProxSrvDisconnectIndDes, CsrBtProxSrvDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND
    { CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND, CsrBtProxSrvTxPowerChangedEventIndSizeof, CsrBtProxSrvTxPowerChangedEventIndSer, CsrBtProxSrvTxPowerChangedEventIndDes, CsrBtProxSrvTxPowerChangedEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM
    { CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM, CsrBtProxSrvUpdateBattLevelCfmSizeof, CsrBtProxSrvUpdateBattLevelCfmSer, CsrBtProxSrvUpdateBattLevelCfmDes, CsrBtProxSrvUpdateBattLevelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ
    { CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ, CsrBtProxSrvUpdateBattLevelReqSizeof, CsrBtProxSrvUpdateBattLevelReqSer, CsrBtProxSrvUpdateBattLevelReqDes, CsrBtProxSrvUpdateBattLevelReqSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM
    { CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM, CsrBtProxSrvUpdateTxPowerCfmSizeof, CsrBtProxSrvUpdateTxPowerCfmSer, CsrBtProxSrvUpdateTxPowerCfmDes, CsrBtProxSrvUpdateTxPowerCfmSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ
    { CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ, CsrBtProxSrvUpdateTxPowerReqSizeof, CsrBtProxSrvUpdateTxPowerReqSer, CsrBtProxSrvUpdateTxPowerReqDes, CsrBtProxSrvUpdateTxPowerReqSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ */
#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
    { CSR_BT_PROX_SRV_WRITE_EVENT_IND, CsrBtProxSrvWriteEventIndSizeof, CsrBtProxSrvWriteEventIndSer, CsrBtProxSrvWriteEventIndDes, CsrBtProxSrvWriteEventIndSerFree },
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtProxSrvConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_PROX_SRV_PRIM, csr_bt_prox_srv_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_prox_srv_conv_info = {
    CSR_BT_PROX_SRV_PRIM,
    "CSR_BT_PROX_SRV_PRIM",
    csr_bt_prox_srv_conv_lut
};

CsrLogPrimitiveInformation* CsrBtProxSrvTechInfoGet(void)
{
    return &csr_bt_prox_srv_conv_info;
}
#endif
#endif

