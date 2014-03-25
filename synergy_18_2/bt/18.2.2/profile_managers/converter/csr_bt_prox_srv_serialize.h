#ifndef CSR_BT_PROX_SRV_SERIALIZE_H__
#define CSR_BT_PROX_SRV_SERIALIZE_H__
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

void CsrBtProx_srvPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM
CsrSize CsrBtProxSrvUpdateTxPowerCfmSizeof(void *msg);
CsrUint8 *CsrBtProxSrvUpdateTxPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvUpdateTxPowerCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvUpdateTxPowerCfmSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ
CsrSize CsrBtProxSrvDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtProxSrvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvDeactivateReqSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND
CsrSize CsrBtProxSrvDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtProxSrvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvDisconnectIndSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND
CsrSize CsrBtProxSrvAlertEventIndSizeof(void *msg);
CsrUint8 *CsrBtProxSrvAlertEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvAlertEventIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvAlertEventIndSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM
CsrSize CsrBtProxSrvActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtProxSrvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvActivateCfmSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
CsrSize CsrBtProxSrvWriteEventIndSizeof(void *msg);
CsrUint8 *CsrBtProxSrvWriteEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvWriteEventIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtProxSrvWriteEventIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM
CsrSize CsrBtProxSrvUpdateBattLevelCfmSizeof(void *msg);
CsrUint8 *CsrBtProxSrvUpdateBattLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvUpdateBattLevelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvUpdateBattLevelCfmSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
CsrSize CsrBtProxSrvActivateReqSizeof(void *msg);
CsrUint8 *CsrBtProxSrvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtProxSrvActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
CsrSize CsrBtProxSrvDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtProxSrvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtProxSrvDeactivateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ
CsrSize CsrBtProxSrvUpdateTxPowerReqSizeof(void *msg);
CsrUint8 *CsrBtProxSrvUpdateTxPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvUpdateTxPowerReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvUpdateTxPowerReqSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND
CsrSize CsrBtProxSrvConnectIndSizeof(void *msg);
CsrUint8 *CsrBtProxSrvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvConnectIndSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND
CsrSize CsrBtProxSrvTxPowerChangedEventIndSizeof(void *msg);
CsrUint8 *CsrBtProxSrvTxPowerChangedEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvTxPowerChangedEventIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvTxPowerChangedEventIndSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ
CsrSize CsrBtProxSrvUpdateBattLevelReqSizeof(void *msg);
CsrUint8 *CsrBtProxSrvUpdateBattLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtProxSrvUpdateBattLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtProxSrvUpdateBattLevelReqSerFree CsrBtProx_srvPfree
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ */

#ifdef __cplusplus
}
#endif

#endif
