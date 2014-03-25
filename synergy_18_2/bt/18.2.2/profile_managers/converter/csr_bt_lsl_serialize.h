#ifndef CSR_BT_LSL_SERIALIZE_H__
#define CSR_BT_LSL_SERIALIZE_H__
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

void CsrBtLslPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_LSL_CONNECT_REQ
CsrSize CsrBtLslConnectReqSizeof(void *msg);
CsrUint8 *CsrBtLslConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtLslConnectReqSerFree CsrBtLslPfree
#endif /* EXCLUDE_CSR_BT_LSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ
CsrSize CsrBtLslDisconnectReqSizeof(void *msg);
CsrUint8 *CsrBtLslDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtLslDisconnectReqSerFree CsrBtLslPfree
#endif /* EXCLUDE_CSR_BT_LSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_STATUS_IND
CsrSize CsrBtLslStatusIndSizeof(void *msg);
CsrUint8 *CsrBtLslStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslStatusIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtLslStatusIndSerFree CsrBtLslPfree
#endif /* EXCLUDE_CSR_BT_LSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_RES
CsrSize CsrBtLslDataResSizeof(void *msg);
CsrUint8 *CsrBtLslDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtLslDataResSerFree CsrBtLslPfree
#endif /* EXCLUDE_CSR_BT_LSL_DATA_RES */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_IND
CsrSize CsrBtLslDataIndSizeof(void *msg);
CsrUint8 *CsrBtLslDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtLslDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_LSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_REQ
CsrSize CsrBtLslDataReqSizeof(void *msg);
CsrUint8 *CsrBtLslDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtLslDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_LSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_LSL_DATA_CFM
CsrSize CsrBtLslDataCfmSizeof(void *msg);
CsrUint8 *CsrBtLslDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtLslDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtLslDataCfmSerFree CsrBtLslPfree
#endif /* EXCLUDE_CSR_BT_LSL_DATA_CFM */

#ifdef __cplusplus
}
#endif

#endif
