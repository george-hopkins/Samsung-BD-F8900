#ifndef CSR_TFTP_SERIALIZE_H__
#define CSR_TFTP_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrTftpPfree(void *ptr);

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_CFM
CsrSize CsrTftpDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrTftpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpDeactivateCfmSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_WRITE_RES
CsrSize CsrTftpWriteResSizeof(void *msg);
CsrUint8 *CsrTftpWriteResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpWriteResDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpWriteResSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_WRITE_RES */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_REQ
CsrSize CsrTftpDeactivateReqSizeof(void *msg);
CsrUint8 *CsrTftpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpDeactivateReqSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_CFM
CsrSize CsrTftpActivateCfmSizeof(void *msg);
CsrUint8 *CsrTftpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpActivateCfmSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_RES
CsrSize CsrTftpConnectResSizeof(void *msg);
CsrUint8 *CsrTftpConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpConnectResDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpConnectResSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_CONNECT_RES */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_REQ
CsrSize CsrTftpActivateReqSizeof(void *msg);
CsrUint8 *CsrTftpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpActivateReqSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_DISCONNECT_IND
CsrSize CsrTftpDisconnectIndSizeof(void *msg);
CsrUint8 *CsrTftpDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpDisconnectIndSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_IND
CsrSize CsrTftpReadIndSizeof(void *msg);
CsrUint8 *CsrTftpReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpReadIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTftpReadIndSerFree CsrTftpPfree
#endif /* EXCLUDE_CSR_TFTP_READ_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_RES
CsrSize CsrTftpReadResSizeof(void *msg);
CsrUint8 *CsrTftpReadResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpReadResDes(CsrUint8 *buffer, CsrSize length);
void CsrTftpReadResSerFree(void *msg);
#endif /* EXCLUDE_CSR_TFTP_READ_RES */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
CsrSize CsrTftpConnectIndSizeof(void *msg);
CsrUint8 *CsrTftpConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpConnectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrTftpConnectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
CsrSize CsrTftpWriteIndSizeof(void *msg);
CsrUint8 *CsrTftpWriteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTftpWriteIndDes(CsrUint8 *buffer, CsrSize length);
void CsrTftpWriteIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */

#ifdef __cplusplus
}
#endif

#endif
