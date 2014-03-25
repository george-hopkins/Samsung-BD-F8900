#ifndef CSR_BT_HCRP_SERIALIZE_H__
#define CSR_BT_HCRP_SERIALIZE_H__
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

void CsrBtHcrpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ
CsrSize CsrBtHcrpRegisterDataPathReqSizeof(void *msg);
CsrUint8 *CsrBtHcrpRegisterDataPathReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpRegisterDataPathReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpRegisterDataPathReqSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND
CsrSize CsrBtHcrpL2caDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtHcrpL2caDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpL2caDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpL2caDisconnectIndSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND
CsrSize CsrBtHcrpL2caConnectIndSizeof(void *msg);
CsrUint8 *CsrBtHcrpL2caConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpL2caConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpL2caConnectIndSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_STATUS_IND
CsrSize CsrBtHcrpStatusIndSizeof(void *msg);
CsrUint8 *CsrBtHcrpStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpStatusIndSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM
CsrSize CsrBtHcrpSecurityInCfmSizeof(void *msg);
CsrUint8 *CsrBtHcrpSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpSecurityInCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpSecurityInCfmSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ
CsrSize CsrBtHcrpDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtHcrpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpDeactivateReqSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES
CsrSize CsrBtHcrpGetIeee1284ResSizeof(void *msg);
CsrUint8 *CsrBtHcrpGetIeee1284ResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpGetIeee1284ResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHcrpGetIeee1284ResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
CsrSize CsrBtHcrpL2caDataIndSizeof(void *msg);
CsrUint8 *CsrBtHcrpL2caDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpL2caDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHcrpL2caDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND
CsrSize CsrBtHcrpGetIeee1284IndSizeof(void *msg);
CsrUint8 *CsrBtHcrpGetIeee1284IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpGetIeee1284IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpGetIeee1284IndSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES
CsrSize CsrBtHcrpGetLptStatusResSizeof(void *msg);
CsrUint8 *CsrBtHcrpGetLptStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpGetLptStatusResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpGetLptStatusResSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND
CsrSize CsrBtHcrpGetLptStatusIndSizeof(void *msg);
CsrUint8 *CsrBtHcrpGetLptStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpGetLptStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpGetLptStatusIndSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ
CsrSize CsrBtHcrpSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHcrpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpSecurityInReqSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING
CsrSize CsrBtHcrpHouseCleaningSizeof(void *msg);
CsrUint8 *CsrBtHcrpHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpHouseCleaningDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpHouseCleaningSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
CsrSize CsrBtHcrpActivateReqSizeof(void *msg);
CsrUint8 *CsrBtHcrpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpActivateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHcrpActivateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM
CsrSize CsrBtHcrpRegisterDataPathCfmSizeof(void *msg);
CsrUint8 *CsrBtHcrpRegisterDataPathCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHcrpRegisterDataPathCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHcrpRegisterDataPathCfmSerFree CsrBtHcrpPfree
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM */

#ifdef __cplusplus
}
#endif

#endif
