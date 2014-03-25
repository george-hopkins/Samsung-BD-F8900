#ifndef CSR_BT_THERM_SRV_SERIALIZE_H__
#define CSR_BT_THERM_SRV_SERIALIZE_H__
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

void CsrBtTherm_srvPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND
CsrSize CsrBtThermSrvConnectIndSizeof(void *msg);
CsrUint8 *CsrBtThermSrvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvConnectIndSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
CsrSize CsrBtThermSrvDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtThermSrvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrBtThermSrvDeactivateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM
CsrSize CsrBtThermSrvUpdateTemperatureCfmSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateTemperatureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateTemperatureCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvUpdateTemperatureCfmSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
CsrSize CsrBtThermSrvUpdateTemperatureReqSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateTemperatureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateTemperatureReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtThermSrvUpdateTemperatureReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM
CsrSize CsrBtThermSrvUpdateHealthInfoCfmSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateHealthInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateHealthInfoCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvUpdateHealthInfoCfmSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ
CsrSize CsrBtThermSrvDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtThermSrvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvDeactivateReqSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
CsrSize CsrBtThermSrvUpdateHealthInfoReqSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateHealthInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateHealthInfoReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtThermSrvUpdateHealthInfoReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
CsrSize CsrBtThermSrvActivateReqSizeof(void *msg);
CsrUint8 *CsrBtThermSrvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtThermSrvActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND
CsrSize CsrBtThermSrvDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtThermSrvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvDisconnectIndSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM
CsrSize CsrBtThermSrvUpdateBattLevelCfmSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateBattLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateBattLevelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvUpdateBattLevelCfmSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
CsrSize CsrBtThermSrvWriteEventIndSizeof(void *msg);
CsrUint8 *CsrBtThermSrvWriteEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvWriteEventIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtThermSrvWriteEventIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ
CsrSize CsrBtThermSrvUpdateBattLevelReqSizeof(void *msg);
CsrUint8 *CsrBtThermSrvUpdateBattLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvUpdateBattLevelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvUpdateBattLevelReqSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM
CsrSize CsrBtThermSrvActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtThermSrvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtThermSrvActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtThermSrvActivateCfmSerFree CsrBtTherm_srvPfree
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM */

#ifdef __cplusplus
}
#endif

#endif
