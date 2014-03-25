/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_bt_autogen.h"
#include "csr_unicode.h"
#include "sds_prim.h"

#ifndef EXCLUDE_SDS_MODULE

void CsrBtSdsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_SDS_UNREGISTER_CFM
CsrSize SDS_UNREGISTER_CFM_TSizeof(void *msg)
{
    SDS_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_SDS_UNREGISTER_CFM
CsrUint8 *SDS_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_UNREGISTER_CFM_T *primitive;

    primitive = (SDS_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->svc_rec_hndl);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_SDS_UNREGISTER_CFM
void *SDS_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(SDS_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->svc_rec_hndl, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_SDS_REGISTER_CFM
CsrSize SDS_REGISTER_CFM_TSizeof(void *msg)
{
    SDS_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_REGISTER_CFM */

#ifndef EXCLUDE_SDS_REGISTER_CFM
CsrUint8 *SDS_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_REGISTER_CFM_T *primitive;

    primitive = (SDS_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->svc_rec_hndl);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_SDS_REGISTER_CFM */

#ifndef EXCLUDE_SDS_REGISTER_CFM
void *SDS_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(SDS_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->svc_rec_hndl, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDS_REGISTER_CFM */

#ifndef EXCLUDE_SDS_CONFIG_CFM
CsrSize SDS_CONFIG_CFM_TSizeof(void *msg)
{
    SDS_CONFIG_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_CONFIG_CFM */

#ifndef EXCLUDE_SDS_CONFIG_CFM
CsrUint8 *SDS_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_CONFIG_CFM_T *primitive;

    primitive = (SDS_CONFIG_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2cap_mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_SDS_CONFIG_CFM */

#ifndef EXCLUDE_SDS_CONFIG_CFM
void *SDS_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_CONFIG_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_CONFIG_CFM_T *) CsrPmemZalloc(sizeof(SDS_CONFIG_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2cap_mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDS_CONFIG_CFM */

#ifndef EXCLUDE_SDS_CONFIG_REQ
CsrSize SDS_CONFIG_REQ_TSizeof(void *msg)
{
    SDS_CONFIG_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_CONFIG_REQ */

#ifndef EXCLUDE_SDS_CONFIG_REQ
CsrUint8 *SDS_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_CONFIG_REQ_T *primitive;

    primitive = (SDS_CONFIG_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2cap_mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_SDS_CONFIG_REQ */

#ifndef EXCLUDE_SDS_CONFIG_REQ
void *SDS_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_CONFIG_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_CONFIG_REQ_T *) CsrPmemZalloc(sizeof(SDS_CONFIG_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2cap_mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDS_CONFIG_REQ */

#ifndef EXCLUDE_SDS_REGISTER_REQ
CsrSize SDS_REGISTER_REQ_TSizeof(void *msg)
{
    SDS_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->num_rec_bytes * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_REGISTER_REQ */

#ifndef EXCLUDE_SDS_REGISTER_REQ
CsrUint8 *SDS_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_REGISTER_REQ_T *primitive;

    primitive = (SDS_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_rec_bytes);
    CsrMemCpySer(buffer, length, &primitive->service_rec, sizeof(primitive->service_rec));
    if(primitive->service_rec)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->service_rec), ((CsrSize) (primitive->num_rec_bytes * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDS_REGISTER_REQ */

#ifndef EXCLUDE_SDS_REGISTER_REQ
void *SDS_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(SDS_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_rec_bytes, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->service_rec), buffer, &offset, ((CsrSize) sizeof(primitive->service_rec)));
    if (primitive->num_rec_bytes)
    {
        primitive->service_rec = CsrPmemZalloc(((CsrUint32) (primitive->num_rec_bytes * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->service_rec), buffer, &offset, ((CsrSize) (primitive->num_rec_bytes * sizeof(CsrUint8))));
    }
    else
    {
        primitive->service_rec = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDS_REGISTER_REQ */

#ifndef EXCLUDE_SDS_REGISTER_REQ
void SDS_REGISTER_REQ_TSerFree(void *msg)
{
    SDS_REGISTER_REQ_T *primitive;

    primitive = (SDS_REGISTER_REQ_T *) msg;

    if(primitive->service_rec != NULL)
    {
        CsrPmemFree(primitive->service_rec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDS_REGISTER_REQ */

#ifndef EXCLUDE_SDS_UNREGISTER_REQ
CsrSize SDS_UNREGISTER_REQ_TSizeof(void *msg)
{
    SDS_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_SDS_UNREGISTER_REQ
CsrUint8 *SDS_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDS_UNREGISTER_REQ_T *primitive;

    primitive = (SDS_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->svc_rec_hndl);

    return buffer;
}
#endif /* EXCLUDE_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_SDS_UNREGISTER_REQ
void *SDS_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDS_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDS_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(SDS_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->svc_rec_hndl, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDS_UNREGISTER_REQ */

#endif /* EXCLUDE_SDS_MODULE */
