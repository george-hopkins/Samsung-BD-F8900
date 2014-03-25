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
#include "csr_tftp_prim.h"

#ifndef EXCLUDE_CSR_TFTP_MODULE

void CsrTftpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_CFM
CsrSize CsrTftpDeactivateCfmSizeof(void *msg)
{
    CsrTftpDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_CFM
CsrUint8 *CsrTftpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpDeactivateCfm *primitive;

    primitive = (CsrTftpDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_CFM
void *CsrTftpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpDeactivateCfm *) CsrPmemZalloc(sizeof(CsrTftpDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_WRITE_RES
CsrSize CsrTftpWriteResSizeof(void *msg)
{
    CsrTftpWriteRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_RES */

#ifndef EXCLUDE_CSR_TFTP_WRITE_RES
CsrUint8 *CsrTftpWriteResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpWriteRes *primitive;

    primitive = (CsrTftpWriteRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_RES */

#ifndef EXCLUDE_CSR_TFTP_WRITE_RES
void *CsrTftpWriteResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpWriteRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpWriteRes *) CsrPmemZalloc(sizeof(CsrTftpWriteRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_RES */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_REQ
CsrSize CsrTftpDeactivateReqSizeof(void *msg)
{
    CsrTftpDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_REQ
CsrUint8 *CsrTftpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpDeactivateReq *primitive;

    primitive = (CsrTftpDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_DEACTIVATE_REQ
void *CsrTftpDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpDeactivateReq *) CsrPmemZalloc(sizeof(CsrTftpDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_CFM
CsrSize CsrTftpActivateCfmSizeof(void *msg)
{
    CsrTftpActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_CFM
CsrUint8 *CsrTftpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpActivateCfm *primitive;

    primitive = (CsrTftpActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_CFM
void *CsrTftpActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpActivateCfm *) CsrPmemZalloc(sizeof(CsrTftpActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_RES
CsrSize CsrTftpConnectResSizeof(void *msg)
{
    CsrTftpConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_RES */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_RES
CsrUint8 *CsrTftpConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpConnectRes *primitive;

    primitive = (CsrTftpConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_RES */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_RES
void *CsrTftpConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpConnectRes *) CsrPmemZalloc(sizeof(CsrTftpConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_RES */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_REQ
CsrSize CsrTftpActivateReqSizeof(void *msg)
{
    CsrTftpActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_REQ
CsrUint8 *CsrTftpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpActivateReq *primitive;

    primitive = (CsrTftpActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->family);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_ACTIVATE_REQ
void *CsrTftpActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpActivateReq *) CsrPmemZalloc(sizeof(CsrTftpActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->family, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_TFTP_DISCONNECT_IND
CsrSize CsrTftpDisconnectIndSizeof(void *msg)
{
    CsrTftpDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_DISCONNECT_IND
CsrUint8 *CsrTftpDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpDisconnectInd *primitive;

    primitive = (CsrTftpDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_DISCONNECT_IND
void *CsrTftpDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpDisconnectInd *) CsrPmemZalloc(sizeof(CsrTftpDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_IND
CsrSize CsrTftpReadIndSizeof(void *msg)
{
    CsrTftpReadInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_READ_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_IND
CsrUint8 *CsrTftpReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpReadInd *primitive;

    primitive = (CsrTftpReadInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_READ_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_IND
void *CsrTftpReadIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpReadInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpReadInd *) CsrPmemZalloc(sizeof(CsrTftpReadInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_READ_IND */

#ifndef EXCLUDE_CSR_TFTP_READ_RES
CsrSize CsrTftpReadResSizeof(void *msg)
{
    CsrTftpReadRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_READ_RES */

#ifndef EXCLUDE_CSR_TFTP_READ_RES
CsrUint8 *CsrTftpReadResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpReadRes *primitive;

    primitive = (CsrTftpReadRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_READ_RES */

#ifndef EXCLUDE_CSR_TFTP_READ_RES
void *CsrTftpReadResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpReadRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpReadRes *) CsrPmemZalloc(sizeof(CsrTftpReadRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_READ_RES */

#ifndef EXCLUDE_CSR_TFTP_READ_RES
void CsrTftpReadResSerFree(void *msg)
{
    CsrTftpReadRes *primitive;

    primitive = (CsrTftpReadRes *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TFTP_READ_RES */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
CsrSize CsrTftpConnectIndSizeof(void *msg)
{
    CsrTftpConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->fileName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
CsrUint8 *CsrTftpConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpConnectInd *primitive;

    primitive = (CsrTftpConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->family);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->opcode);
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrCharStringSer(buffer, length, primitive->fileName);

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
void *CsrTftpConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpConnectInd *) CsrPmemZalloc(sizeof(CsrTftpConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->family, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->port, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->opcode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrCharStringDes(&primitive->fileName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_CONNECT_IND
void CsrTftpConnectIndSerFree(void *msg)
{
    CsrTftpConnectInd *primitive;

    primitive = (CsrTftpConnectInd *) msg;

    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TFTP_CONNECT_IND */

#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
CsrSize CsrTftpWriteIndSizeof(void *msg)
{
    CsrTftpWriteInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */

#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
CsrUint8 *CsrTftpWriteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTftpWriteInd *primitive;

    primitive = (CsrTftpWriteInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */

#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
void *CsrTftpWriteIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTftpWriteInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTftpWriteInd *) CsrPmemZalloc(sizeof(CsrTftpWriteInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */

#ifndef EXCLUDE_CSR_TFTP_WRITE_IND
void CsrTftpWriteIndSerFree(void *msg)
{
    CsrTftpWriteInd *primitive;

    primitive = (CsrTftpWriteInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_TFTP_WRITE_IND */

#endif /* EXCLUDE_CSR_TFTP_MODULE */
