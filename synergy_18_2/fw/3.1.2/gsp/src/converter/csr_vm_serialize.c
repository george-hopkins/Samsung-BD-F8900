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
#include "csr_vm_prim.h"

#ifndef EXCLUDE_CSR_VM_MODULE

void CsrVmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_VM_REGISTER_REQ
CsrSize CsrVmRegisterReqSizeof(void *msg)
{
    CsrVmRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_VM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_VM_REGISTER_REQ
CsrUint8 *CsrVmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrVmRegisterReq *primitive;

    primitive = (CsrVmRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_VM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_VM_REGISTER_REQ
void *CsrVmRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrVmRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrVmRegisterReq *) CsrPmemZalloc(sizeof(CsrVmRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_VM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_VM_REGISTER_CFM
CsrSize CsrVmRegisterCfmSizeof(void *msg)
{
    CsrVmRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_VM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_VM_REGISTER_CFM
CsrUint8 *CsrVmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrVmRegisterCfm *primitive;

    primitive = (CsrVmRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_VM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_VM_REGISTER_CFM
void *CsrVmRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrVmRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrVmRegisterCfm *) CsrPmemZalloc(sizeof(CsrVmRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_VM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_VM_DATA_REQ
CsrSize CsrVmDataReqSizeof(void *msg)
{
    CsrVmDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_VM_DATA_REQ */

#ifndef EXCLUDE_CSR_VM_DATA_REQ
CsrUint8 *CsrVmDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrVmDataReq *primitive;

    primitive = (CsrVmDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_VM_DATA_REQ */

#ifndef EXCLUDE_CSR_VM_DATA_REQ
void *CsrVmDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrVmDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrVmDataReq *) CsrPmemZalloc(sizeof(CsrVmDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_VM_DATA_REQ */

#ifndef EXCLUDE_CSR_VM_DATA_REQ
void CsrVmDataReqSerFree(void *msg)
{
    CsrVmDataReq *primitive;

    primitive = (CsrVmDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_VM_DATA_REQ */

#ifndef EXCLUDE_CSR_VM_DATA_IND
CsrSize CsrVmDataIndSizeof(void *msg)
{
    CsrVmDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_VM_DATA_IND */

#ifndef EXCLUDE_CSR_VM_DATA_IND
CsrUint8 *CsrVmDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrVmDataInd *primitive;

    primitive = (CsrVmDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_VM_DATA_IND */

#ifndef EXCLUDE_CSR_VM_DATA_IND
void *CsrVmDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrVmDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrVmDataInd *) CsrPmemZalloc(sizeof(CsrVmDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_VM_DATA_IND */

#ifndef EXCLUDE_CSR_VM_DATA_IND
void CsrVmDataIndSerFree(void *msg)
{
    CsrVmDataInd *primitive;

    primitive = (CsrVmDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_VM_DATA_IND */

#endif /* EXCLUDE_CSR_VM_MODULE */
