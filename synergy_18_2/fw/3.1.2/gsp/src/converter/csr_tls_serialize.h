#ifndef CSR_TLS_SERIALIZE_H__
#define CSR_TLS_SERIALIZE_H__
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

void CsrTlsPfree(void *ptr);

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
CsrSize CsrTlsConfigCipherCfmSizeof(void *msg);
CsrUint8 *CsrTlsConfigCipherCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCipherCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCipherCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
CsrSize CsrTlsSessionDestroyCfmSizeof(void *msg);
CsrUint8 *CsrTlsSessionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsSessionDestroyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsSessionDestroyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_REQ
CsrSize CsrTlsSessionCreateReqSizeof(void *msg);
CsrUint8 *CsrTlsSessionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsSessionCreateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTlsSessionCreateReqSerFree CsrTlsPfree
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
CsrSize CsrTlsConfigCertVerifyReqSizeof(void *msg);
CsrUint8 *CsrTlsConfigCertVerifyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCertVerifyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCertVerifyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
CsrSize CsrTlsSessionDestroyReqSizeof(void *msg);
CsrUint8 *CsrTlsSessionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsSessionDestroyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsSessionDestroyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
CsrSize CsrTlsConnectionDataCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDataCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDataCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
CsrSize CsrTlsConnectionCertIndSizeof(void *msg);
CsrUint8 *CsrTlsConnectionCertIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionCertIndDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionCertIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
CsrSize CsrTlsConnectionBindReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionBindReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionBindReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
CsrSize CsrTlsConfigCertClientCfmSizeof(void *msg);
CsrUint8 *CsrTlsConfigCertClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCertClientCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCertClientCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
CsrSize CsrTlsConnectionDisconnectResSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDisconnectResDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDisconnectResSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
CsrSize CsrTlsConnectionCreateReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionCreateReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionCreateReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
CsrSize CsrTlsConnectionCreateCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionCreateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionCreateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
CsrSize CsrTlsConnectionDataResSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDataResDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDataResSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
CsrSize CsrTlsConnectionDataIndSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
CsrSize CsrTlsConnectionDataReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
CsrSize CsrTlsConnectionDestroyReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDestroyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDestroyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
CsrSize CsrTlsConnectionBindCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionBindCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionBindCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
CsrSize CsrTlsConfigCompressionCfmSizeof(void *msg);
CsrUint8 *CsrTlsConfigCompressionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCompressionCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCompressionCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
CsrSize CsrTlsConnectionDisconnectCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDisconnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDisconnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
CsrSize CsrTlsConnectionConnectReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionConnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionConnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
CsrSize CsrTlsConnectionDestroyCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDestroyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDestroyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
CsrSize CsrTlsConnectionConnectCfmSizeof(void *msg);
CsrUint8 *CsrTlsConnectionConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionConnectCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionConnectCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
CsrSize CsrTlsConnectionDisconnectReqSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDisconnectReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDisconnectReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
CsrSize CsrTlsConnectionCertResSizeof(void *msg);
CsrUint8 *CsrTlsConnectionCertResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionCertResDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionCertResSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
CsrSize CsrTlsConfigCompressionReqSizeof(void *msg);
CsrUint8 *CsrTlsConfigCompressionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCompressionReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCompressionReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
CsrSize CsrTlsConfigCertVerifyCfmSizeof(void *msg);
CsrUint8 *CsrTlsConfigCertVerifyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCertVerifyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCertVerifyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
CsrSize CsrTlsConfigCipherReqSizeof(void *msg);
CsrUint8 *CsrTlsConfigCipherReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCipherReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCipherReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
CsrSize CsrTlsConfigCertClientReqSizeof(void *msg);
CsrUint8 *CsrTlsConfigCertClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConfigCertClientReqDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConfigCertClientReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
CsrSize CsrTlsConnectionDisconnectIndSizeof(void *msg);
CsrUint8 *CsrTlsConnectionDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsConnectionDisconnectIndDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsConnectionDisconnectIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
CsrSize CsrTlsSessionCreateCfmSizeof(void *msg);
CsrUint8 *CsrTlsSessionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTlsSessionCreateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrTlsSessionCreateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */

#ifdef __cplusplus
}
#endif

#endif
