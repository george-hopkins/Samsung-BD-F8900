/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_tls_converter_init.h"
#ifndef EXCLUDE_CSR_TLS_MODULE
#include "csr_tls_serialize.h"
#include "csr_tls_prim.h"

static CsrMsgConvMsgEntry csr_tls_conv_lut[] = {
#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
    { CSR_TLS_CONFIG_CERT_CLIENT_CFM, CsrTlsConfigCertClientCfmSizeof, CsrTlsConfigCertClientCfmSer, CsrTlsConfigCertClientCfmDes, CsrTlsConfigCertClientCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */
#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
    { CSR_TLS_CONFIG_CERT_CLIENT_REQ, CsrTlsConfigCertClientReqSizeof, CsrTlsConfigCertClientReqSer, CsrTlsConfigCertClientReqDes, CsrTlsConfigCertClientReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */
#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
    { CSR_TLS_CONFIG_CERT_VERIFY_CFM, CsrTlsConfigCertVerifyCfmSizeof, CsrTlsConfigCertVerifyCfmSer, CsrTlsConfigCertVerifyCfmDes, CsrTlsConfigCertVerifyCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */
#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
    { CSR_TLS_CONFIG_CERT_VERIFY_REQ, CsrTlsConfigCertVerifyReqSizeof, CsrTlsConfigCertVerifyReqSer, CsrTlsConfigCertVerifyReqDes, CsrTlsConfigCertVerifyReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */
#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
    { CSR_TLS_CONFIG_CIPHER_CFM, CsrTlsConfigCipherCfmSizeof, CsrTlsConfigCipherCfmSer, CsrTlsConfigCipherCfmDes, CsrTlsConfigCipherCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */
#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
    { CSR_TLS_CONFIG_CIPHER_REQ, CsrTlsConfigCipherReqSizeof, CsrTlsConfigCipherReqSer, CsrTlsConfigCipherReqDes, CsrTlsConfigCipherReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */
#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
    { CSR_TLS_CONFIG_COMPRESSION_CFM, CsrTlsConfigCompressionCfmSizeof, CsrTlsConfigCompressionCfmSer, CsrTlsConfigCompressionCfmDes, CsrTlsConfigCompressionCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */
#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
    { CSR_TLS_CONFIG_COMPRESSION_REQ, CsrTlsConfigCompressionReqSizeof, CsrTlsConfigCompressionReqSer, CsrTlsConfigCompressionReqDes, CsrTlsConfigCompressionReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
    { CSR_TLS_CONNECTION_BIND_CFM, CsrTlsConnectionBindCfmSizeof, CsrTlsConnectionBindCfmSer, CsrTlsConnectionBindCfmDes, CsrTlsConnectionBindCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
    { CSR_TLS_CONNECTION_BIND_REQ, CsrTlsConnectionBindReqSizeof, CsrTlsConnectionBindReqSer, CsrTlsConnectionBindReqDes, CsrTlsConnectionBindReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
    { CSR_TLS_CONNECTION_CERT_IND, CsrTlsConnectionCertIndSizeof, CsrTlsConnectionCertIndSer, CsrTlsConnectionCertIndDes, CsrTlsConnectionCertIndSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
    { CSR_TLS_CONNECTION_CERT_RES, CsrTlsConnectionCertResSizeof, CsrTlsConnectionCertResSer, CsrTlsConnectionCertResDes, CsrTlsConnectionCertResSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
    { CSR_TLS_CONNECTION_CONNECT_CFM, CsrTlsConnectionConnectCfmSizeof, CsrTlsConnectionConnectCfmSer, CsrTlsConnectionConnectCfmDes, CsrTlsConnectionConnectCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
    { CSR_TLS_CONNECTION_CONNECT_REQ, CsrTlsConnectionConnectReqSizeof, CsrTlsConnectionConnectReqSer, CsrTlsConnectionConnectReqDes, CsrTlsConnectionConnectReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
    { CSR_TLS_CONNECTION_CREATE_CFM, CsrTlsConnectionCreateCfmSizeof, CsrTlsConnectionCreateCfmSer, CsrTlsConnectionCreateCfmDes, CsrTlsConnectionCreateCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
    { CSR_TLS_CONNECTION_CREATE_REQ, CsrTlsConnectionCreateReqSizeof, CsrTlsConnectionCreateReqSer, CsrTlsConnectionCreateReqDes, CsrTlsConnectionCreateReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
    { CSR_TLS_CONNECTION_DATA_CFM, CsrTlsConnectionDataCfmSizeof, CsrTlsConnectionDataCfmSer, CsrTlsConnectionDataCfmDes, CsrTlsConnectionDataCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
    { CSR_TLS_CONNECTION_DATA_IND, CsrTlsConnectionDataIndSizeof, CsrTlsConnectionDataIndSer, CsrTlsConnectionDataIndDes, CsrTlsConnectionDataIndSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
    { CSR_TLS_CONNECTION_DATA_REQ, CsrTlsConnectionDataReqSizeof, CsrTlsConnectionDataReqSer, CsrTlsConnectionDataReqDes, CsrTlsConnectionDataReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
    { CSR_TLS_CONNECTION_DATA_RES, CsrTlsConnectionDataResSizeof, CsrTlsConnectionDataResSer, CsrTlsConnectionDataResDes, CsrTlsConnectionDataResSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
    { CSR_TLS_CONNECTION_DESTROY_CFM, CsrTlsConnectionDestroyCfmSizeof, CsrTlsConnectionDestroyCfmSer, CsrTlsConnectionDestroyCfmDes, CsrTlsConnectionDestroyCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
    { CSR_TLS_CONNECTION_DESTROY_REQ, CsrTlsConnectionDestroyReqSizeof, CsrTlsConnectionDestroyReqSer, CsrTlsConnectionDestroyReqDes, CsrTlsConnectionDestroyReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
    { CSR_TLS_CONNECTION_DISCONNECT_CFM, CsrTlsConnectionDisconnectCfmSizeof, CsrTlsConnectionDisconnectCfmSer, CsrTlsConnectionDisconnectCfmDes, CsrTlsConnectionDisconnectCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
    { CSR_TLS_CONNECTION_DISCONNECT_IND, CsrTlsConnectionDisconnectIndSizeof, CsrTlsConnectionDisconnectIndSer, CsrTlsConnectionDisconnectIndDes, CsrTlsConnectionDisconnectIndSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
    { CSR_TLS_CONNECTION_DISCONNECT_REQ, CsrTlsConnectionDisconnectReqSizeof, CsrTlsConnectionDisconnectReqSer, CsrTlsConnectionDisconnectReqDes, CsrTlsConnectionDisconnectReqSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */
#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
    { CSR_TLS_CONNECTION_DISCONNECT_RES, CsrTlsConnectionDisconnectResSizeof, CsrTlsConnectionDisconnectResSer, CsrTlsConnectionDisconnectResDes, CsrTlsConnectionDisconnectResSerFree },
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */
#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
    { CSR_TLS_SESSION_CREATE_CFM, CsrTlsSessionCreateCfmSizeof, CsrTlsSessionCreateCfmSer, CsrTlsSessionCreateCfmDes, CsrTlsSessionCreateCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */
#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_REQ
    { CSR_TLS_SESSION_CREATE_REQ, CsrTlsSessionCreateReqSizeof, CsrTlsSessionCreateReqSer, CsrTlsSessionCreateReqDes, CsrTlsSessionCreateReqSerFree },
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_REQ */
#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
    { CSR_TLS_SESSION_DESTROY_CFM, CsrTlsSessionDestroyCfmSizeof, CsrTlsSessionDestroyCfmSer, CsrTlsSessionDestroyCfmDes, CsrTlsSessionDestroyCfmSerFree },
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */
#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
    { CSR_TLS_SESSION_DESTROY_REQ, CsrTlsSessionDestroyReqSizeof, CsrTlsSessionDestroyReqSer, CsrTlsSessionDestroyReqDes, CsrTlsSessionDestroyReqSerFree },
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrTlsConverterInit(void)
{
    CsrMsgConvInsert(CSR_TLS_PRIM, csr_tls_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_tls_conv_info = {
    CSR_TLS_PRIM,
    "CSR_TLS_PRIM",
    csr_tls_conv_lut
};

CsrLogPrimitiveInformation* CsrTlsTechInfoGet(void)
{
    return &csr_tls_conv_info;
}
#endif
#endif

