#ifndef CSR_BT_HIDD_SERIALIZE_H__
#define CSR_BT_HIDD_SERIALIZE_H__
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

void CsrBtHiddPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM
CsrSize CsrBtHiddUnplugCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddUnplugCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddUnplugCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddUnplugCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_STATUS_IND
CsrSize CsrBtHiddStatusIndSizeof(void *msg);
CsrUint8 *CsrBtHiddStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddStatusIndSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_CFM
CsrSize CsrBtHiddDataCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddDataCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM
CsrSize CsrBtHiddSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddSecurityInCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ
CsrSize CsrBtHiddDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtHiddDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddDeactivateReqSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND
CsrSize CsrBtHiddModeChangeIndSizeof(void *msg);
CsrUint8 *CsrBtHiddModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddModeChangeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddModeChangeIndSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_IND
CsrSize CsrBtHiddUnplugIndSizeof(void *msg);
CsrUint8 *CsrBtHiddUnplugIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddUnplugIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddUnplugIndSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
CsrSize CsrBtHiddDataReqSizeof(void *msg);
CsrUint8 *CsrBtHiddDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHiddDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
CsrSize CsrBtHiddActivateReqSizeof(void *msg);
CsrUint8 *CsrBtHiddActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHiddActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ
CsrSize CsrBtHiddSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtHiddSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddSecurityOutReqSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ
CsrSize CsrBtHiddUnplugReqSizeof(void *msg);
CsrUint8 *CsrBtHiddUnplugReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddUnplugReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddUnplugReqSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM
CsrSize CsrBtHiddSecurityOutCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddSecurityOutCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ
CsrSize CsrBtHiddSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHiddSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddSecurityInReqSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_RESTORE_IND
CsrSize CsrBtHiddRestoreIndSizeof(void *msg);
CsrUint8 *CsrBtHiddRestoreIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddRestoreIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddRestoreIndSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_RESTORE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ
CsrSize CsrBtHiddModeChangeReqSizeof(void *msg);
CsrUint8 *CsrBtHiddModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddModeChangeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddModeChangeReqSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM
CsrSize CsrBtHiddDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddDeactivateCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
CsrSize CsrBtHiddDataIndSizeof(void *msg);
CsrUint8 *CsrBtHiddDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHiddDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
CsrSize CsrBtHiddControlIndSizeof(void *msg);
CsrUint8 *CsrBtHiddControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddControlIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHiddControlIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM
CsrSize CsrBtHiddActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHiddActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddActivateCfmSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND
CsrSize CsrBtHiddReactivateIndSizeof(void *msg);
CsrUint8 *CsrBtHiddReactivateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddReactivateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHiddReactivateIndSerFree CsrBtHiddPfree
#endif /* EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
CsrSize CsrBtHiddControlResSizeof(void *msg);
CsrUint8 *CsrBtHiddControlResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHiddControlResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHiddControlResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */

#ifdef __cplusplus
}
#endif

#endif
