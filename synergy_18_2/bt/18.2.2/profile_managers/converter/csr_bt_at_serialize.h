#ifndef CSR_BT_AT_SERIALIZE_H__
#define CSR_BT_AT_SERIALIZE_H__
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

void CsrBtAtPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_AT_DG_CONNECT_IND
CsrSize CsrBtAtDgConnectIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgConnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgConnectIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_D_IND
CsrSize CsrBtAtDgAtcDIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcDIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcDIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcDIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_E_IND
CsrSize CsrBtAtDgAtcEIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcEIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcEIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcEIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_E_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES
CsrSize CsrBtAtDgAtcUnknownExtendedCmdResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcUnknownExtendedCmdResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcUnknownExtendedCmdResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcUnknownExtendedCmdResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND
CsrSize CsrBtAtDgAtcS8qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS8qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS8qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS8qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND
CsrSize CsrBtAtDgPortnegIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgPortnegIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgPortnegIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND
CsrSize CsrBtAtDgAtcAbortIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcAbortIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcAbortIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_M_IND
CsrSize CsrBtAtDgAtcMIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcMIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcMIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcMIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_M_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_REQ
CsrSize CsrBtAtDgDataReqSizeof(void *msg);
CsrUint8 *CsrBtAtDgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ
CsrSize CsrBtAtDgDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtAtDgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDisconnectReqSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ
CsrSize CsrBtAtDgControlReqSizeof(void *msg);
CsrUint8 *CsrBtAtDgControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgControlReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgControlReqSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES
CsrSize CsrBtAtDgAtcS6qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS6qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS6qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS6qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND
CsrSize CsrBtAtDgAtcS0IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS0IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS0IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS0IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND
CsrSize CsrBtAtDgAtcS7IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS7IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS7IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS7IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_CFM
CsrSize CsrBtAtDgDataCfmSizeof(void *msg);
CsrUint8 *CsrBtAtDgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDataCfmSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND
CsrSize CsrBtAtDgAtcS3IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS3IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS3IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS3IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND
CsrSize CsrBtAtDgAtcAndDIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcAndDIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcAndDIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcAndDIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_D_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND
CsrSize CsrBtAtDgAtcQIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcQIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcQIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcQIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND
CsrSize CsrBtAtDgAtcS6IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS6IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS6IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS6IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND
CsrSize CsrBtAtDgAtcS7qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS7qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS7qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS7qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND
CsrSize CsrBtAtDgAtcAndFIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcAndFIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcAndFIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcAndFIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_F_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND
CsrSize CsrBtAtDgAtcS3qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS3qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS3qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS3qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND
CsrSize CsrBtAtDgAtcGmiIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmiIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcGmiIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND
CsrSize CsrBtAtDgAtcS4qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS4qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS4qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS4qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_L_IND
CsrSize CsrBtAtDgAtcLIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcLIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcLIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcLIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_L_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES
CsrSize CsrBtAtDgAtcGmrResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmrResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmrResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcGmrResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND
CsrSize CsrBtAtDgAtcS5IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS5IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS5IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS5IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND
CsrSize CsrBtAtDgAtcS0qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS0qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS0qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS0qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND
CsrSize CsrBtAtDgAtcS10qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS10qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS10qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS10qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_V_IND
CsrSize CsrBtAtDgAtcVIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcVIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcVIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcVIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_V_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_O_IND
CsrSize CsrBtAtDgAtcOIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcOIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcOIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcOIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_O_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_A_IND
CsrSize CsrBtAtDgAtcAIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcAIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcAIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcAIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_A_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND
CsrSize CsrBtAtDgAtcGmrIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmrIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmrIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcGmrIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMR_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND
CsrSize CsrBtAtDgDisconnectIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDisconnectIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND
CsrSize CsrBtAtDgAtcS4IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS4IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS4IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS4IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES
CsrSize CsrBtAtDgAtcGmmResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmmResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmmResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcGmmResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND
CsrSize CsrBtAtDgDataPathStatusIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgDataPathStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDataPathStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDataPathStatusIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_PATH_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_RES
CsrSize CsrBtAtDgDataResSizeof(void *msg);
CsrUint8 *CsrBtAtDgDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDataResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES
CsrSize CsrBtAtDgAtcS7qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS7qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS7qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS7qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S7Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND
CsrSize CsrBtAtDgAtcZIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcZIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcZIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcZIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_Z_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES
CsrSize CsrBtAtDgAtcS0qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS0qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS0qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS0qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S0Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES
CsrSize CsrBtAtDgAtcS5qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS5qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS5qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS5qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES
CsrSize CsrBtAtDgAtcS4qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS4qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS4qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS4qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S4Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_P_IND
CsrSize CsrBtAtDgAtcPIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcPIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcPIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcPIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_P_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM
CsrSize CsrBtAtDgDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtAtDgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDeactivateCfmSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND
CsrSize CsrBtAtDgAtcGcapIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGcapIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGcapIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcGcapIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES
CsrSize CsrBtAtDgAtcResultCodeResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcResultCodeResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcResultCodeResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcResultCodeResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_RESULT_CODE_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_DATA_IND
CsrSize CsrBtAtDgDataIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND
CsrSize CsrBtAtDgAtcAndCIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcAndCIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcAndCIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcAndCIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_AND_C_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES
CsrSize CsrBtAtDgPortnegResSizeof(void *msg);
CsrUint8 *CsrBtAtDgPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgPortnegResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgPortnegResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES
CsrSize CsrBtAtDgAtcS8qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS8qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS8qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS8qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES
CsrSize CsrBtAtDgAtcFactorySettingResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcFactorySettingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcFactorySettingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcFactorySettingResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_X_IND
CsrSize CsrBtAtDgAtcXIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcXIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcXIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcXIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_X_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES
CsrSize CsrBtAtDgAtcGcapResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGcapResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGcapResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcGcapResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GCAP_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES
CsrSize CsrBtAtDgAtcGmiResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmiResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmiResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcGmiResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMI_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_T_IND
CsrSize CsrBtAtDgAtcTIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcTIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcTIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcTIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_T_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ
CsrSize CsrBtAtDgActivateReqSizeof(void *msg);
CsrUint8 *CsrBtAtDgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgActivateReqSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND
CsrSize CsrBtAtDgAtcS10IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS10IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS10IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS10IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND
CsrSize CsrBtAtDgAtcS6qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS6qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS6qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS6qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S6Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES
CsrSize CsrBtAtDgAtcS3qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS3qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS3qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS3qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S3Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND
CsrSize CsrBtAtDgAtcS8IndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS8IndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS8IndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS8IndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S8_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_H_IND
CsrSize CsrBtAtDgAtcHIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcHIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcHIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcHIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_H_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES
CsrSize CsrBtAtDgAtcS10qResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS10qResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS10qResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS10qResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S10Q_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND
CsrSize CsrBtAtDgAtcGmmIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcGmmIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcGmmIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcGmmIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_GMM_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ
CsrSize CsrBtAtDgDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtAtDgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgDeactivateReqSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND
CsrSize CsrBtAtDgAtcS5qIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcS5qIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcS5qIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcS5qIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_S5Q_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES
CsrSize CsrBtAtDgAtcConnectTextResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcConnectTextResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcConnectTextResDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcConnectTextResSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES
CsrSize CsrBtAtDgAtcDefaultSettingResSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcDefaultSettingResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcDefaultSettingResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgAtcDefaultSettingResSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */

#ifndef EXCLUDE_CSR_BT_AT_DG_CONTROL_IND
CsrSize CsrBtAtDgControlIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgControlIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtAtDgControlIndSerFree CsrBtAtPfree
#endif /* EXCLUDE_CSR_BT_AT_DG_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND
CsrSize CsrBtAtDgAtcUnknownExtendedCmdIndSizeof(void *msg);
CsrUint8 *CsrBtAtDgAtcUnknownExtendedCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtAtDgAtcUnknownExtendedCmdIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtAtDgAtcUnknownExtendedCmdIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND */

#ifdef __cplusplus
}
#endif

#endif
