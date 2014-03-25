/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_tls_prim.h"

#ifndef EXCLUDE_CSR_TLS_MODULE

void CsrTlsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
CsrSize CsrTlsConfigCipherCfmSizeof(void *msg)
{
    CsrTlsConfigCipherCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
CsrUint8 *CsrTlsConfigCipherCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCipherCfm *primitive;

    primitive = (CsrTlsConfigCipherCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
void *CsrTlsConfigCipherCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCipherCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCipherCfm *) CsrPmemZalloc(sizeof(CsrTlsConfigCipherCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM
void CsrTlsConfigCipherCfmSerFree(void *msg)
{
    CsrTlsConfigCipherCfm *primitive;

    primitive = (CsrTlsConfigCipherCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
CsrSize CsrTlsSessionDestroyCfmSizeof(void *msg)
{
    CsrTlsSessionDestroyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
CsrUint8 *CsrTlsSessionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsSessionDestroyCfm *primitive;

    primitive = (CsrTlsSessionDestroyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
void *CsrTlsSessionDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsSessionDestroyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsSessionDestroyCfm *) CsrPmemZalloc(sizeof(CsrTlsSessionDestroyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM
void CsrTlsSessionDestroyCfmSerFree(void *msg)
{
    CsrTlsSessionDestroyCfm *primitive;

    primitive = (CsrTlsSessionDestroyCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_REQ
CsrSize CsrTlsSessionCreateReqSizeof(void *msg)
{
    CsrTlsSessionCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_REQ
CsrUint8 *CsrTlsSessionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsSessionCreateReq *primitive;

    primitive = (CsrTlsSessionCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_REQ
void *CsrTlsSessionCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsSessionCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsSessionCreateReq *) CsrPmemZalloc(sizeof(CsrTlsSessionCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
CsrSize CsrTlsConfigCertVerifyReqSizeof(void *msg)
{
    CsrTlsConfigCertVerifyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->trustedCAcertificateLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
CsrUint8 *CsrTlsConfigCertVerifyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCertVerifyReq *primitive;

    primitive = (CsrTlsConfigCertVerifyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrMemCpySer(buffer, length, &primitive->trustedCAcertificate, sizeof(primitive->trustedCAcertificate));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->trustedCAcertificateLength);
    if(primitive->trustedCAcertificate)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->trustedCAcertificate), ((CsrSize) (primitive->trustedCAcertificateLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
void *CsrTlsConfigCertVerifyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCertVerifyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCertVerifyReq *) CsrPmemZalloc(sizeof(CsrTlsConfigCertVerifyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrMemCpyDes(((void *) &primitive->trustedCAcertificate), buffer, &offset, ((CsrSize) sizeof(primitive->trustedCAcertificate)));
    CsrUint32Des((CsrUint32 *) &primitive->trustedCAcertificateLength, buffer, &offset);
    if (primitive->trustedCAcertificateLength)
    {
        primitive->trustedCAcertificate = CsrPmemZalloc(((CsrUint32) (primitive->trustedCAcertificateLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->trustedCAcertificate), buffer, &offset, ((CsrSize) (primitive->trustedCAcertificateLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->trustedCAcertificate = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ
void CsrTlsConfigCertVerifyReqSerFree(void *msg)
{
    CsrTlsConfigCertVerifyReq *primitive;

    primitive = (CsrTlsConfigCertVerifyReq *) msg;

    if(primitive->trustedCAcertificate != NULL)
    {
        CsrPmemFree(primitive->trustedCAcertificate);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
CsrSize CsrTlsSessionDestroyReqSizeof(void *msg)
{
    CsrTlsSessionDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
CsrUint8 *CsrTlsSessionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsSessionDestroyReq *primitive;

    primitive = (CsrTlsSessionDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
void *CsrTlsSessionDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsSessionDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsSessionDestroyReq *) CsrPmemZalloc(sizeof(CsrTlsSessionDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ
void CsrTlsSessionDestroyReqSerFree(void *msg)
{
    CsrTlsSessionDestroyReq *primitive;

    primitive = (CsrTlsSessionDestroyReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_SESSION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
CsrSize CsrTlsConnectionDataCfmSizeof(void *msg)
{
    CsrTlsConnectionDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
CsrUint8 *CsrTlsConnectionDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDataCfm *primitive;

    primitive = (CsrTlsConnectionDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
void *CsrTlsConnectionDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDataCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM
void CsrTlsConnectionDataCfmSerFree(void *msg)
{
    CsrTlsConnectionDataCfm *primitive;

    primitive = (CsrTlsConnectionDataCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
CsrSize CsrTlsConnectionCertIndSizeof(void *msg)
{
    CsrTlsConnectionCertInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->certLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
CsrUint8 *CsrTlsConnectionCertIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionCertInd *primitive;

    primitive = (CsrTlsConnectionCertInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->certLen);
    CsrMemCpySer(buffer, length, &primitive->cert, sizeof(primitive->cert));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    if(primitive->cert)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->cert), ((CsrSize) (primitive->certLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
void *CsrTlsConnectionCertIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionCertInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionCertInd *) CsrPmemZalloc(sizeof(CsrTlsConnectionCertInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint32Des((CsrUint32 *) &primitive->certLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->cert), buffer, &offset, ((CsrSize) sizeof(primitive->cert)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    if (primitive->certLen)
    {
        primitive->cert = CsrPmemZalloc(((CsrUint32) (primitive->certLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->cert), buffer, &offset, ((CsrSize) (primitive->certLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->cert = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_IND
void CsrTlsConnectionCertIndSerFree(void *msg)
{
    CsrTlsConnectionCertInd *primitive;

    primitive = (CsrTlsConnectionCertInd *) msg;

    if(primitive->cert != NULL)
    {
        CsrPmemFree(primitive->cert);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
CsrSize CsrTlsConnectionBindReqSizeof(void *msg)
{
    CsrTlsConnectionBindReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
CsrUint8 *CsrTlsConnectionBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionBindReq *primitive;

    primitive = (CsrTlsConnectionBindReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
void *CsrTlsConnectionBindReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionBindReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionBindReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionBindReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ
void CsrTlsConnectionBindReqSerFree(void *msg)
{
    CsrTlsConnectionBindReq *primitive;

    primitive = (CsrTlsConnectionBindReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
CsrSize CsrTlsConfigCertClientCfmSizeof(void *msg)
{
    CsrTlsConfigCertClientCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
CsrUint8 *CsrTlsConfigCertClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCertClientCfm *primitive;

    primitive = (CsrTlsConfigCertClientCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
void *CsrTlsConfigCertClientCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCertClientCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCertClientCfm *) CsrPmemZalloc(sizeof(CsrTlsConfigCertClientCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM
void CsrTlsConfigCertClientCfmSerFree(void *msg)
{
    CsrTlsConfigCertClientCfm *primitive;

    primitive = (CsrTlsConfigCertClientCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
CsrSize CsrTlsConnectionDisconnectResSizeof(void *msg)
{
    CsrTlsConnectionDisconnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
CsrUint8 *CsrTlsConnectionDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDisconnectRes *primitive;

    primitive = (CsrTlsConnectionDisconnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
void *CsrTlsConnectionDisconnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDisconnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDisconnectRes *) CsrPmemZalloc(sizeof(CsrTlsConnectionDisconnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES
void CsrTlsConnectionDisconnectResSerFree(void *msg)
{
    CsrTlsConnectionDisconnectRes *primitive;

    primitive = (CsrTlsConnectionDisconnectRes *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
CsrSize CsrTlsConnectionCreateReqSizeof(void *msg)
{
    CsrTlsConnectionCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
CsrUint8 *CsrTlsConnectionCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionCreateReq *primitive;

    primitive = (CsrTlsConnectionCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->socketFamily);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
void *CsrTlsConnectionCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionCreateReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint8Des((CsrUint8 *) &primitive->socketFamily, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ
void CsrTlsConnectionCreateReqSerFree(void *msg)
{
    CsrTlsConnectionCreateReq *primitive;

    primitive = (CsrTlsConnectionCreateReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
CsrSize CsrTlsConnectionCreateCfmSizeof(void *msg)
{
    CsrTlsConnectionCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
CsrUint8 *CsrTlsConnectionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionCreateCfm *primitive;

    primitive = (CsrTlsConnectionCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
void *CsrTlsConnectionCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionCreateCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM
void CsrTlsConnectionCreateCfmSerFree(void *msg)
{
    CsrTlsConnectionCreateCfm *primitive;

    primitive = (CsrTlsConnectionCreateCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
CsrSize CsrTlsConnectionDataResSizeof(void *msg)
{
    CsrTlsConnectionDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
CsrUint8 *CsrTlsConnectionDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDataRes *primitive;

    primitive = (CsrTlsConnectionDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
void *CsrTlsConnectionDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDataRes *) CsrPmemZalloc(sizeof(CsrTlsConnectionDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_RES
void CsrTlsConnectionDataResSerFree(void *msg)
{
    CsrTlsConnectionDataRes *primitive;

    primitive = (CsrTlsConnectionDataRes *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
CsrSize CsrTlsConnectionDataIndSizeof(void *msg)
{
    CsrTlsConnectionDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bufLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
CsrUint8 *CsrTlsConnectionDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDataInd *primitive;

    primitive = (CsrTlsConnectionDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bufLen);
    CsrMemCpySer(buffer, length, &primitive->buf, sizeof(primitive->buf));
    if(primitive->buf)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->buf), ((CsrSize) (primitive->bufLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
void *CsrTlsConnectionDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDataInd *) CsrPmemZalloc(sizeof(CsrTlsConnectionDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint32Des((CsrUint32 *) &primitive->bufLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->buf), buffer, &offset, ((CsrSize) sizeof(primitive->buf)));
    if (primitive->bufLen)
    {
        primitive->buf = CsrPmemZalloc(((CsrUint32) (primitive->bufLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->buf), buffer, &offset, ((CsrSize) (primitive->bufLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->buf = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_IND
void CsrTlsConnectionDataIndSerFree(void *msg)
{
    CsrTlsConnectionDataInd *primitive;

    primitive = (CsrTlsConnectionDataInd *) msg;

    if(primitive->buf != NULL)
    {
        CsrPmemFree(primitive->buf);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
CsrSize CsrTlsConnectionDataReqSizeof(void *msg)
{
    CsrTlsConnectionDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bufLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
CsrUint8 *CsrTlsConnectionDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDataReq *primitive;

    primitive = (CsrTlsConnectionDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bufLen);
    CsrMemCpySer(buffer, length, &primitive->buf, sizeof(primitive->buf));
    if(primitive->buf)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->buf), ((CsrSize) (primitive->bufLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
void *CsrTlsConnectionDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDataReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint32Des((CsrUint32 *) &primitive->bufLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->buf), buffer, &offset, ((CsrSize) sizeof(primitive->buf)));
    if (primitive->bufLen)
    {
        primitive->buf = CsrPmemZalloc(((CsrUint32) (primitive->bufLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->buf), buffer, &offset, ((CsrSize) (primitive->bufLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->buf = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ
void CsrTlsConnectionDataReqSerFree(void *msg)
{
    CsrTlsConnectionDataReq *primitive;

    primitive = (CsrTlsConnectionDataReq *) msg;

    if(primitive->buf != NULL)
    {
        CsrPmemFree(primitive->buf);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DATA_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
CsrSize CsrTlsConnectionDestroyReqSizeof(void *msg)
{
    CsrTlsConnectionDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
CsrUint8 *CsrTlsConnectionDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDestroyReq *primitive;

    primitive = (CsrTlsConnectionDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
void *CsrTlsConnectionDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDestroyReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ
void CsrTlsConnectionDestroyReqSerFree(void *msg)
{
    CsrTlsConnectionDestroyReq *primitive;

    primitive = (CsrTlsConnectionDestroyReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
CsrSize CsrTlsConnectionBindCfmSizeof(void *msg)
{
    CsrTlsConnectionBindCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
CsrUint8 *CsrTlsConnectionBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionBindCfm *primitive;

    primitive = (CsrTlsConnectionBindCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
void *CsrTlsConnectionBindCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionBindCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionBindCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionBindCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM
void CsrTlsConnectionBindCfmSerFree(void *msg)
{
    CsrTlsConnectionBindCfm *primitive;

    primitive = (CsrTlsConnectionBindCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_BIND_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
CsrSize CsrTlsConfigCompressionCfmSizeof(void *msg)
{
    CsrTlsConfigCompressionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
CsrUint8 *CsrTlsConfigCompressionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCompressionCfm *primitive;

    primitive = (CsrTlsConfigCompressionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
void *CsrTlsConfigCompressionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCompressionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCompressionCfm *) CsrPmemZalloc(sizeof(CsrTlsConfigCompressionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM
void CsrTlsConfigCompressionCfmSerFree(void *msg)
{
    CsrTlsConfigCompressionCfm *primitive;

    primitive = (CsrTlsConfigCompressionCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
CsrSize CsrTlsConnectionDisconnectCfmSizeof(void *msg)
{
    CsrTlsConnectionDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
CsrUint8 *CsrTlsConnectionDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDisconnectCfm *primitive;

    primitive = (CsrTlsConnectionDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
void *CsrTlsConnectionDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDisconnectCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM
void CsrTlsConnectionDisconnectCfmSerFree(void *msg)
{
    CsrTlsConnectionDisconnectCfm *primitive;

    primitive = (CsrTlsConnectionDisconnectCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
CsrSize CsrTlsConnectionConnectReqSizeof(void *msg)
{
    CsrTlsConnectionConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
CsrUint8 *CsrTlsConnectionConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionConnectReq *primitive;

    primitive = (CsrTlsConnectionConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
void *CsrTlsConnectionConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionConnectReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ
void CsrTlsConnectionConnectReqSerFree(void *msg)
{
    CsrTlsConnectionConnectReq *primitive;

    primitive = (CsrTlsConnectionConnectReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
CsrSize CsrTlsConnectionDestroyCfmSizeof(void *msg)
{
    CsrTlsConnectionDestroyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
CsrUint8 *CsrTlsConnectionDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDestroyCfm *primitive;

    primitive = (CsrTlsConnectionDestroyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
void *CsrTlsConnectionDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDestroyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDestroyCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionDestroyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM
void CsrTlsConnectionDestroyCfmSerFree(void *msg)
{
    CsrTlsConnectionDestroyCfm *primitive;

    primitive = (CsrTlsConnectionDestroyCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DESTROY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
CsrSize CsrTlsConnectionConnectCfmSizeof(void *msg)
{
    CsrTlsConnectionConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
CsrUint8 *CsrTlsConnectionConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionConnectCfm *primitive;

    primitive = (CsrTlsConnectionConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
void *CsrTlsConnectionConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionConnectCfm *) CsrPmemZalloc(sizeof(CsrTlsConnectionConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM
void CsrTlsConnectionConnectCfmSerFree(void *msg)
{
    CsrTlsConnectionConnectCfm *primitive;

    primitive = (CsrTlsConnectionConnectCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CONNECT_CFM */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
CsrSize CsrTlsConnectionDisconnectReqSizeof(void *msg)
{
    CsrTlsConnectionDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
CsrUint8 *CsrTlsConnectionDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDisconnectReq *primitive;

    primitive = (CsrTlsConnectionDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
void *CsrTlsConnectionDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDisconnectReq *) CsrPmemZalloc(sizeof(CsrTlsConnectionDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ
void CsrTlsConnectionDisconnectReqSerFree(void *msg)
{
    CsrTlsConnectionDisconnectReq *primitive;

    primitive = (CsrTlsConnectionDisconnectReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
CsrSize CsrTlsConnectionCertResSizeof(void *msg)
{
    CsrTlsConnectionCertRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
CsrUint8 *CsrTlsConnectionCertResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionCertRes *primitive;

    primitive = (CsrTlsConnectionCertRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accepted);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
void *CsrTlsConnectionCertResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionCertRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionCertRes *) CsrPmemZalloc(sizeof(CsrTlsConnectionCertRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));
    CsrUint8Des((CsrUint8 *) &primitive->accepted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_CERT_RES
void CsrTlsConnectionCertResSerFree(void *msg)
{
    CsrTlsConnectionCertRes *primitive;

    primitive = (CsrTlsConnectionCertRes *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_CERT_RES */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
CsrSize CsrTlsConfigCompressionReqSizeof(void *msg)
{
    CsrTlsConfigCompressionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
CsrUint8 *CsrTlsConfigCompressionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCompressionReq *primitive;

    primitive = (CsrTlsConfigCompressionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->compression);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
void *CsrTlsConfigCompressionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCompressionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCompressionReq *) CsrPmemZalloc(sizeof(CsrTlsConfigCompressionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint8Des((CsrUint8 *) &primitive->compression, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ
void CsrTlsConfigCompressionReqSerFree(void *msg)
{
    CsrTlsConfigCompressionReq *primitive;

    primitive = (CsrTlsConfigCompressionReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_COMPRESSION_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
CsrSize CsrTlsConfigCertVerifyCfmSizeof(void *msg)
{
    CsrTlsConfigCertVerifyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
CsrUint8 *CsrTlsConfigCertVerifyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCertVerifyCfm *primitive;

    primitive = (CsrTlsConfigCertVerifyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
void *CsrTlsConfigCertVerifyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCertVerifyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCertVerifyCfm *) CsrPmemZalloc(sizeof(CsrTlsConfigCertVerifyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM
void CsrTlsConfigCertVerifyCfmSerFree(void *msg)
{
    CsrTlsConfigCertVerifyCfm *primitive;

    primitive = (CsrTlsConfigCertVerifyCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_VERIFY_CFM */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
CsrSize CsrTlsConfigCipherReqSizeof(void *msg)
{
    CsrTlsConfigCipherReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->cipherSuiteLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
CsrUint8 *CsrTlsConfigCipherReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCipherReq *primitive;

    primitive = (CsrTlsConfigCipherReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrMemCpySer(buffer, length, &primitive->cipherSuite, sizeof(primitive->cipherSuite));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cipherSuiteLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keybitsMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keybitsMax);
    if(primitive->cipherSuite)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->cipherSuite), ((CsrSize) (primitive->cipherSuiteLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
void *CsrTlsConfigCipherReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCipherReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCipherReq *) CsrPmemZalloc(sizeof(CsrTlsConfigCipherReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrMemCpyDes(((void *) &primitive->cipherSuite), buffer, &offset, ((CsrSize) sizeof(primitive->cipherSuite)));
    CsrUint16Des((CsrUint16 *) &primitive->cipherSuiteLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keybitsMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keybitsMax, buffer, &offset);
    if (primitive->cipherSuiteLength)
    {
        primitive->cipherSuite = CsrPmemZalloc(((CsrUint32) (primitive->cipherSuiteLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->cipherSuite), buffer, &offset, ((CsrSize) (primitive->cipherSuiteLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->cipherSuite = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ
void CsrTlsConfigCipherReqSerFree(void *msg)
{
    CsrTlsConfigCipherReq *primitive;

    primitive = (CsrTlsConfigCipherReq *) msg;

    if(primitive->cipherSuite != NULL)
    {
        CsrPmemFree(primitive->cipherSuite);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CIPHER_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
CsrSize CsrTlsConfigCertClientReqSizeof(void *msg)
{
    CsrTlsConfigCertClientReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->certificateLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
CsrUint8 *CsrTlsConfigCertClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConfigCertClientReq *primitive;

    primitive = (CsrTlsConfigCertClientReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->certificateType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->certificateLength);
    CsrMemCpySer(buffer, length, &primitive->certificate, sizeof(primitive->certificate));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    if(primitive->certificate)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->certificate), ((CsrSize) (primitive->certificateLength * sizeof(CsrUint8))));
    }
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
void *CsrTlsConfigCertClientReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConfigCertClientReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConfigCertClientReq *) CsrPmemZalloc(sizeof(CsrTlsConfigCertClientReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint8Des((CsrUint8 *) &primitive->certificateType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->certificateLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->certificate), buffer, &offset, ((CsrSize) sizeof(primitive->certificate)));
    CsrUint32Des((CsrUint32 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    if (primitive->certificateLength)
    {
        primitive->certificate = CsrPmemZalloc(((CsrUint32) (primitive->certificateLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->certificate), buffer, &offset, ((CsrSize) (primitive->certificateLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->certificate = NULL;
    }
    if (primitive->passwordLength)
    {
        primitive->password = CsrPmemZalloc(((CsrUint32) (primitive->passwordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->password), buffer, &offset, ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->password = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ
void CsrTlsConfigCertClientReqSerFree(void *msg)
{
    CsrTlsConfigCertClientReq *primitive;

    primitive = (CsrTlsConfigCertClientReq *) msg;

    if(primitive->certificate != NULL)
    {
        CsrPmemFree(primitive->certificate);
    }
    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONFIG_CERT_CLIENT_REQ */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
CsrSize CsrTlsConnectionDisconnectIndSizeof(void *msg)
{
    CsrTlsConnectionDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
CsrUint8 *CsrTlsConnectionDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsConnectionDisconnectInd *primitive;

    primitive = (CsrTlsConnectionDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->conn, sizeof(primitive->conn));

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
void *CsrTlsConnectionDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsConnectionDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsConnectionDisconnectInd *) CsrPmemZalloc(sizeof(CsrTlsConnectionDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conn), buffer, &offset, ((CsrSize) sizeof(primitive->conn)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND
void CsrTlsConnectionDisconnectIndSerFree(void *msg)
{
    CsrTlsConnectionDisconnectInd *primitive;

    primitive = (CsrTlsConnectionDisconnectInd *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_CONNECTION_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
CsrSize CsrTlsSessionCreateCfmSizeof(void *msg)
{
    CsrTlsSessionCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
CsrUint8 *CsrTlsSessionCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTlsSessionCreateCfm *primitive;

    primitive = (CsrTlsSessionCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->session, sizeof(primitive->session));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
void *CsrTlsSessionCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTlsSessionCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTlsSessionCreateCfm *) CsrPmemZalloc(sizeof(CsrTlsSessionCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->session), buffer, &offset, ((CsrSize) sizeof(primitive->session)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */

#ifndef EXCLUDE_CSR_TLS_SESSION_CREATE_CFM
void CsrTlsSessionCreateCfmSerFree(void *msg)
{
    CsrTlsSessionCreateCfm *primitive;

    primitive = (CsrTlsSessionCreateCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TLS_SESSION_CREATE_CFM */

#endif /* EXCLUDE_CSR_TLS_MODULE */
