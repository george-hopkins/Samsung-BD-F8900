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
#include "att_prim.h"

#ifndef EXCLUDE_ATT_MODULE

void CsrBtAttPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_ATT_REGISTER_REQ
CsrSize ATT_REGISTER_REQ_TSizeof(void *msg)
{
    ATT_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_REGISTER_REQ */

#ifndef EXCLUDE_ATT_REGISTER_REQ
CsrUint8 *ATT_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_REGISTER_REQ_T *primitive;

    primitive = (ATT_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_ATT_REGISTER_REQ */

#ifndef EXCLUDE_ATT_REGISTER_REQ
void *ATT_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(ATT_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_REGISTER_REQ */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_CFM
CsrSize ATT_EXCHANGE_MTU_CFM_TSizeof(void *msg)
{
    ATT_EXCHANGE_MTU_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_CFM */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_CFM
CsrUint8 *ATT_EXCHANGE_MTU_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXCHANGE_MTU_CFM_T *primitive;

    primitive = (ATT_EXCHANGE_MTU_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_CFM */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_CFM
void *ATT_EXCHANGE_MTU_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXCHANGE_MTU_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXCHANGE_MTU_CFM_T *) CsrPmemZalloc(sizeof(ATT_EXCHANGE_MTU_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_CFM */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ
CsrSize ATT_READ_BY_GROUP_TYPE_REQ_TSizeof(void *msg)
{
    ATT_READ_BY_GROUP_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ
CsrUint8 *ATT_READ_BY_GROUP_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BY_GROUP_TYPE_REQ_T *primitive;

    primitive = (ATT_READ_BY_GROUP_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->group_type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->group), ((CsrSize) 4 * (4)));

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ
void *ATT_READ_BY_GROUP_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BY_GROUP_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BY_GROUP_TYPE_REQ_T *) CsrPmemZalloc(sizeof(ATT_READ_BY_GROUP_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->group_type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->group), buffer, &offset, ((CsrSize) 4 * (4)));

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_REQ */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_CFM
CsrSize ATT_HANDLE_VALUE_CFM_TSizeof(void *msg)
{
    ATT_HANDLE_VALUE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_CFM */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_CFM
CsrUint8 *ATT_HANDLE_VALUE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_HANDLE_VALUE_CFM_T *primitive;

    primitive = (ATT_HANDLE_VALUE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_CFM */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_CFM
void *ATT_HANDLE_VALUE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_HANDLE_VALUE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_HANDLE_VALUE_CFM_T *) CsrPmemZalloc(sizeof(ATT_HANDLE_VALUE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_CFM */

#ifndef EXCLUDE_ATT_DISCONNECT_IND
CsrSize ATT_DISCONNECT_IND_TSizeof(void *msg)
{
    ATT_DISCONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_DISCONNECT_IND */

#ifndef EXCLUDE_ATT_DISCONNECT_IND
CsrUint8 *ATT_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_DISCONNECT_IND_T *primitive;

    primitive = (ATT_DISCONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_ATT_DISCONNECT_IND */

#ifndef EXCLUDE_ATT_DISCONNECT_IND
void *ATT_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_DISCONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_DISCONNECT_IND_T *) CsrPmemZalloc(sizeof(ATT_DISCONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->reason = (l2ca_disc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_DISCONNECT_IND */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_CFM
CsrSize ATT_READ_BY_TYPE_CFM_TSizeof(void *msg)
{
    ATT_READ_BY_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_CFM
CsrUint8 *ATT_READ_BY_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BY_TYPE_CFM_T *primitive;

    primitive = (ATT_READ_BY_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_CFM
void *ATT_READ_BY_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BY_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BY_TYPE_CFM_T *) CsrPmemZalloc(sizeof(ATT_READ_BY_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_CFM
void ATT_READ_BY_TYPE_CFM_TSerFree(void *msg)
{
    ATT_READ_BY_TYPE_CFM_T *primitive;

    primitive = (ATT_READ_BY_TYPE_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_CFM */

#ifndef EXCLUDE_ATT_FIND_INFO_CFM
CsrSize ATT_FIND_INFO_CFM_TSizeof(void *msg)
{
    ATT_FIND_INFO_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_FIND_INFO_CFM */

#ifndef EXCLUDE_ATT_FIND_INFO_CFM
CsrUint8 *ATT_FIND_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_FIND_INFO_CFM_T *primitive;

    primitive = (ATT_FIND_INFO_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuid_type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid), ((CsrSize) 4 * (4)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_FIND_INFO_CFM */

#ifndef EXCLUDE_ATT_FIND_INFO_CFM
void *ATT_FIND_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_FIND_INFO_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_FIND_INFO_CFM_T *) CsrPmemZalloc(sizeof(ATT_FIND_INFO_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuid_type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid), buffer, &offset, ((CsrSize) 4 * (4)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_FIND_INFO_CFM */

#ifndef EXCLUDE_ATT_UNREGISTER_REQ
CsrSize ATT_UNREGISTER_REQ_TSizeof(void *msg)
{
    ATT_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_UNREGISTER_REQ */

#ifndef EXCLUDE_ATT_UNREGISTER_REQ
CsrUint8 *ATT_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_UNREGISTER_REQ_T *primitive;

    primitive = (ATT_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_ATT_UNREGISTER_REQ */

#ifndef EXCLUDE_ATT_UNREGISTER_REQ
void *ATT_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(ATT_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_UNREGISTER_REQ */

#ifndef EXCLUDE_ATT_CONNECT_IND
CsrSize ATT_CONNECT_IND_TSizeof(void *msg)
{
    ATT_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_CONNECT_IND */

#ifndef EXCLUDE_ATT_CONNECT_IND
CsrUint8 *ATT_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_CONNECT_IND_T *primitive;

    primitive = (ATT_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_ATT_CONNECT_IND */

#ifndef EXCLUDE_ATT_CONNECT_IND
void *ATT_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_CONNECT_IND_T *) CsrPmemZalloc(sizeof(ATT_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_CONNECT_IND */

#ifndef EXCLUDE_ATT_REGISTER_CFM
CsrSize ATT_REGISTER_CFM_TSizeof(void *msg)
{
    ATT_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_REGISTER_CFM */

#ifndef EXCLUDE_ATT_REGISTER_CFM
CsrUint8 *ATT_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_REGISTER_CFM_T *primitive;

    primitive = (ATT_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_REGISTER_CFM */

#ifndef EXCLUDE_ATT_REGISTER_CFM
void *ATT_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(ATT_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_REGISTER_CFM */

#ifndef EXCLUDE_ATT_FIND_INFO_REQ
CsrSize ATT_FIND_INFO_REQ_TSizeof(void *msg)
{
    ATT_FIND_INFO_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_FIND_INFO_REQ */

#ifndef EXCLUDE_ATT_FIND_INFO_REQ
CsrUint8 *ATT_FIND_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_FIND_INFO_REQ_T *primitive;

    primitive = (ATT_FIND_INFO_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);

    return buffer;
}
#endif /* EXCLUDE_ATT_FIND_INFO_REQ */

#ifndef EXCLUDE_ATT_FIND_INFO_REQ
void *ATT_FIND_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_FIND_INFO_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_FIND_INFO_REQ_T *) CsrPmemZalloc(sizeof(ATT_FIND_INFO_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_FIND_INFO_REQ */

#ifndef EXCLUDE_ATT_ACCESS_RSP
CsrSize ATT_ACCESS_RSP_TSizeof(void *msg)
{
    ATT_ACCESS_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_ACCESS_RSP */

#ifndef EXCLUDE_ATT_ACCESS_RSP
CsrUint8 *ATT_ACCESS_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_ACCESS_RSP_T *primitive;

    primitive = (ATT_ACCESS_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_ACCESS_RSP */

#ifndef EXCLUDE_ATT_ACCESS_RSP
void *ATT_ACCESS_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_ACCESS_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_ACCESS_RSP_T *) CsrPmemZalloc(sizeof(ATT_ACCESS_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_ACCESS_RSP */

#ifndef EXCLUDE_ATT_ACCESS_RSP
void ATT_ACCESS_RSP_TSerFree(void *msg)
{
    ATT_ACCESS_RSP_T *primitive;

    primitive = (ATT_ACCESS_RSP_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_ACCESS_RSP */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_REQ
CsrSize ATT_HANDLE_VALUE_REQ_TSizeof(void *msg)
{
    ATT_HANDLE_VALUE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_REQ */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_REQ
CsrUint8 *ATT_HANDLE_VALUE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_HANDLE_VALUE_REQ_T *primitive;

    primitive = (ATT_HANDLE_VALUE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_REQ */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_REQ
void *ATT_HANDLE_VALUE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_HANDLE_VALUE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_HANDLE_VALUE_REQ_T *) CsrPmemZalloc(sizeof(ATT_HANDLE_VALUE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_REQ */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_REQ
void ATT_HANDLE_VALUE_REQ_TSerFree(void *msg)
{
    ATT_HANDLE_VALUE_REQ_T *primitive;

    primitive = (ATT_HANDLE_VALUE_REQ_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_REQ */

#ifndef EXCLUDE_ATT_DISCONNECT_REQ
CsrSize ATT_DISCONNECT_REQ_TSizeof(void *msg)
{
    ATT_DISCONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_DISCONNECT_REQ */

#ifndef EXCLUDE_ATT_DISCONNECT_REQ
CsrUint8 *ATT_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_DISCONNECT_REQ_T *primitive;

    primitive = (ATT_DISCONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_ATT_DISCONNECT_REQ */

#ifndef EXCLUDE_ATT_DISCONNECT_REQ
void *ATT_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_DISCONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_DISCONNECT_REQ_T *) CsrPmemZalloc(sizeof(ATT_DISCONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_DISCONNECT_REQ */

#ifndef EXCLUDE_ATT_CONNECT_REQ
CsrSize ATT_CONNECT_REQ_TSizeof(void *msg)
{
    ATT_CONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_CONNECT_REQ */

#ifndef EXCLUDE_ATT_CONNECT_REQ
CsrUint8 *ATT_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_CONNECT_REQ_T *primitive;

    primitive = (ATT_CONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_ATT_CONNECT_REQ */

#ifndef EXCLUDE_ATT_CONNECT_REQ
void *ATT_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_CONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_CONNECT_REQ_T *) CsrPmemZalloc(sizeof(ATT_CONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_CONNECT_REQ */

#ifndef EXCLUDE_ATT_WRITE_CFM
CsrSize ATT_WRITE_CFM_TSizeof(void *msg)
{
    ATT_WRITE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_WRITE_CFM */

#ifndef EXCLUDE_ATT_WRITE_CFM
CsrUint8 *ATT_WRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_WRITE_CFM_T *primitive;

    primitive = (ATT_WRITE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_WRITE_CFM */

#ifndef EXCLUDE_ATT_WRITE_CFM
void *ATT_WRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_WRITE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_WRITE_CFM_T *) CsrPmemZalloc(sizeof(ATT_WRITE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_WRITE_CFM */

#ifndef EXCLUDE_ATT_CONNECT_CFM
CsrSize ATT_CONNECT_CFM_TSizeof(void *msg)
{
    ATT_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_CONNECT_CFM */

#ifndef EXCLUDE_ATT_CONNECT_CFM
CsrUint8 *ATT_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_CONNECT_CFM_T *primitive;

    primitive = (ATT_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_CONNECT_CFM */

#ifndef EXCLUDE_ATT_CONNECT_CFM
void *ATT_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(ATT_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_conn_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_CONNECT_CFM */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_CFM
CsrSize ATT_PREPARE_WRITE_CFM_TSizeof(void *msg)
{
    ATT_PREPARE_WRITE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_CFM */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_CFM
CsrUint8 *ATT_PREPARE_WRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_PREPARE_WRITE_CFM_T *primitive;

    primitive = (ATT_PREPARE_WRITE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_CFM */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_CFM
void *ATT_PREPARE_WRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_PREPARE_WRITE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_PREPARE_WRITE_CFM_T *) CsrPmemZalloc(sizeof(ATT_PREPARE_WRITE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_CFM */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_CFM
void ATT_PREPARE_WRITE_CFM_TSerFree(void *msg)
{
    ATT_PREPARE_WRITE_CFM_T *primitive;

    primitive = (ATT_PREPARE_WRITE_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_CFM */

#ifndef EXCLUDE_ATT_ACCESS_IND
CsrSize ATT_ACCESS_IND_TSizeof(void *msg)
{
    ATT_ACCESS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_ACCESS_IND */

#ifndef EXCLUDE_ATT_ACCESS_IND
CsrUint8 *ATT_ACCESS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_ACCESS_IND_T *primitive;

    primitive = (ATT_ACCESS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_ACCESS_IND */

#ifndef EXCLUDE_ATT_ACCESS_IND
void *ATT_ACCESS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_ACCESS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_ACCESS_IND_T *) CsrPmemZalloc(sizeof(ATT_ACCESS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_ACCESS_IND */

#ifndef EXCLUDE_ATT_ACCESS_IND
void ATT_ACCESS_IND_TSerFree(void *msg)
{
    ATT_ACCESS_IND_T *primitive;

    primitive = (ATT_ACCESS_IND_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_ACCESS_IND */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_IND
CsrSize ATT_HANDLE_VALUE_IND_TSizeof(void *msg)
{
    ATT_HANDLE_VALUE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_IND */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_IND
CsrUint8 *ATT_HANDLE_VALUE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_HANDLE_VALUE_IND_T *primitive;

    primitive = (ATT_HANDLE_VALUE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_IND */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_IND
void *ATT_HANDLE_VALUE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_HANDLE_VALUE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_HANDLE_VALUE_IND_T *) CsrPmemZalloc(sizeof(ATT_HANDLE_VALUE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_IND */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_IND
void ATT_HANDLE_VALUE_IND_TSerFree(void *msg)
{
    ATT_HANDLE_VALUE_IND_T *primitive;

    primitive = (ATT_HANDLE_VALUE_IND_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_IND */

#ifndef EXCLUDE_ATT_ADD_CFM
CsrSize ATT_ADD_CFM_TSizeof(void *msg)
{
    ATT_ADD_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_ADD_CFM */

#ifndef EXCLUDE_ATT_ADD_CFM
CsrUint8 *ATT_ADD_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_ADD_CFM_T *primitive;

    primitive = (ATT_ADD_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_ADD_CFM */

#ifndef EXCLUDE_ATT_ADD_CFM
void *ATT_ADD_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_ADD_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_ADD_CFM_T *) CsrPmemZalloc(sizeof(ATT_ADD_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_ADD_CFM */

#ifndef EXCLUDE_ATT_DEBUG_IND
CsrSize ATT_DEBUG_IND_TSizeof(void *msg)
{
    ATT_DEBUG_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_debug * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_DEBUG_IND */

#ifndef EXCLUDE_ATT_DEBUG_IND
CsrUint8 *ATT_DEBUG_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_DEBUG_IND_T *primitive;

    primitive = (ATT_DEBUG_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_debug);
    CsrMemCpySer(buffer, length, &primitive->debug, sizeof(primitive->debug));
    if(primitive->debug)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->debug), ((CsrSize) (primitive->size_debug * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_DEBUG_IND */

#ifndef EXCLUDE_ATT_DEBUG_IND
void *ATT_DEBUG_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_DEBUG_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_DEBUG_IND_T *) CsrPmemZalloc(sizeof(ATT_DEBUG_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_debug, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->debug), buffer, &offset, ((CsrSize) sizeof(primitive->debug)));
    if (primitive->size_debug)
    {
        primitive->debug = CsrPmemZalloc(((CsrUint32) (primitive->size_debug * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->debug), buffer, &offset, ((CsrSize) (primitive->size_debug * sizeof(CsrUint8))));
    }
    else
    {
        primitive->debug = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_DEBUG_IND */

#ifndef EXCLUDE_ATT_DEBUG_IND
void ATT_DEBUG_IND_TSerFree(void *msg)
{
    ATT_DEBUG_IND_T *primitive;

    primitive = (ATT_DEBUG_IND_T *) msg;

    if(primitive->debug != NULL)
    {
        CsrPmemFree(primitive->debug);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_DEBUG_IND */

#ifndef EXCLUDE_ATT_REMOVE_REQ
CsrSize ATT_REMOVE_REQ_TSizeof(void *msg)
{
    ATT_REMOVE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_REMOVE_REQ */

#ifndef EXCLUDE_ATT_REMOVE_REQ
CsrUint8 *ATT_REMOVE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_REMOVE_REQ_T *primitive;

    primitive = (ATT_REMOVE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);

    return buffer;
}
#endif /* EXCLUDE_ATT_REMOVE_REQ */

#ifndef EXCLUDE_ATT_REMOVE_REQ
void *ATT_REMOVE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_REMOVE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_REMOVE_REQ_T *) CsrPmemZalloc(sizeof(ATT_REMOVE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_REMOVE_REQ */

#ifndef EXCLUDE_ATT_READ_BLOB_CFM
CsrSize ATT_READ_BLOB_CFM_TSizeof(void *msg)
{
    ATT_READ_BLOB_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BLOB_CFM */

#ifndef EXCLUDE_ATT_READ_BLOB_CFM
CsrUint8 *ATT_READ_BLOB_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BLOB_CFM_T *primitive;

    primitive = (ATT_READ_BLOB_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BLOB_CFM */

#ifndef EXCLUDE_ATT_READ_BLOB_CFM
void *ATT_READ_BLOB_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BLOB_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BLOB_CFM_T *) CsrPmemZalloc(sizeof(ATT_READ_BLOB_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BLOB_CFM */

#ifndef EXCLUDE_ATT_READ_BLOB_CFM
void ATT_READ_BLOB_CFM_TSerFree(void *msg)
{
    ATT_READ_BLOB_CFM_T *primitive;

    primitive = (ATT_READ_BLOB_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_BLOB_CFM */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_REQ
CsrSize ATT_PREPARE_WRITE_REQ_TSizeof(void *msg)
{
    ATT_PREPARE_WRITE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_REQ */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_REQ
CsrUint8 *ATT_PREPARE_WRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_PREPARE_WRITE_REQ_T *primitive;

    primitive = (ATT_PREPARE_WRITE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_REQ */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_REQ
void *ATT_PREPARE_WRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_PREPARE_WRITE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_PREPARE_WRITE_REQ_T *) CsrPmemZalloc(sizeof(ATT_PREPARE_WRITE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_REQ */

#ifndef EXCLUDE_ATT_PREPARE_WRITE_REQ
void ATT_PREPARE_WRITE_REQ_TSerFree(void *msg)
{
    ATT_PREPARE_WRITE_REQ_T *primitive;

    primitive = (ATT_PREPARE_WRITE_REQ_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_PREPARE_WRITE_REQ */

#ifndef EXCLUDE_ATT_WRITE_REQ
CsrSize ATT_WRITE_REQ_TSizeof(void *msg)
{
    ATT_WRITE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_WRITE_REQ */

#ifndef EXCLUDE_ATT_WRITE_REQ
CsrUint8 *ATT_WRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_WRITE_REQ_T *primitive;

    primitive = (ATT_WRITE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_WRITE_REQ */

#ifndef EXCLUDE_ATT_WRITE_REQ
void *ATT_WRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_WRITE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_WRITE_REQ_T *) CsrPmemZalloc(sizeof(ATT_WRITE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_WRITE_REQ */

#ifndef EXCLUDE_ATT_WRITE_REQ
void ATT_WRITE_REQ_TSerFree(void *msg)
{
    ATT_WRITE_REQ_T *primitive;

    primitive = (ATT_WRITE_REQ_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_WRITE_REQ */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM
CsrSize ATT_FIND_BY_TYPE_VALUE_CFM_TSizeof(void *msg)
{
    ATT_FIND_BY_TYPE_VALUE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM
CsrUint8 *ATT_FIND_BY_TYPE_VALUE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_FIND_BY_TYPE_VALUE_CFM_T *primitive;

    primitive = (ATT_FIND_BY_TYPE_VALUE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM
void *ATT_FIND_BY_TYPE_VALUE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_FIND_BY_TYPE_VALUE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_FIND_BY_TYPE_VALUE_CFM_T *) CsrPmemZalloc(sizeof(ATT_FIND_BY_TYPE_VALUE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_CFM */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_CFM
CsrSize ATT_EXECUTE_WRITE_CFM_TSizeof(void *msg)
{
    ATT_EXECUTE_WRITE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_CFM */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_CFM
CsrUint8 *ATT_EXECUTE_WRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXECUTE_WRITE_CFM_T *primitive;

    primitive = (ATT_EXECUTE_WRITE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_CFM */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_CFM
void *ATT_EXECUTE_WRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXECUTE_WRITE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXECUTE_WRITE_CFM_T *) CsrPmemZalloc(sizeof(ATT_EXECUTE_WRITE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_CFM */

#ifndef EXCLUDE_ATT_READ_CFM
CsrSize ATT_READ_CFM_TSizeof(void *msg)
{
    ATT_READ_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_CFM */

#ifndef EXCLUDE_ATT_READ_CFM
CsrUint8 *ATT_READ_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_CFM_T *primitive;

    primitive = (ATT_READ_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_CFM */

#ifndef EXCLUDE_ATT_READ_CFM
void *ATT_READ_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_CFM_T *) CsrPmemZalloc(sizeof(ATT_READ_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_CFM */

#ifndef EXCLUDE_ATT_READ_CFM
void ATT_READ_CFM_TSerFree(void *msg)
{
    ATT_READ_CFM_T *primitive;

    primitive = (ATT_READ_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_CFM */

#ifndef EXCLUDE_ATT_ADD_DB_CFM
CsrSize ATT_ADD_DB_CFM_TSizeof(void *msg)
{
    ATT_ADD_DB_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_ADD_DB_CFM */

#ifndef EXCLUDE_ATT_ADD_DB_CFM
CsrUint8 *ATT_ADD_DB_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_ADD_DB_CFM_T *primitive;

    primitive = (ATT_ADD_DB_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_ADD_DB_CFM */

#ifndef EXCLUDE_ATT_ADD_DB_CFM
void *ATT_ADD_DB_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_ADD_DB_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_ADD_DB_CFM_T *) CsrPmemZalloc(sizeof(ATT_ADD_DB_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_ADD_DB_CFM */

#ifndef EXCLUDE_ATT_REMOVE_CFM
CsrSize ATT_REMOVE_CFM_TSizeof(void *msg)
{
    ATT_REMOVE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_REMOVE_CFM */

#ifndef EXCLUDE_ATT_REMOVE_CFM
CsrUint8 *ATT_REMOVE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_REMOVE_CFM_T *primitive;

    primitive = (ATT_REMOVE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_attr);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_REMOVE_CFM */

#ifndef EXCLUDE_ATT_REMOVE_CFM
void *ATT_REMOVE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_REMOVE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_REMOVE_CFM_T *) CsrPmemZalloc(sizeof(ATT_REMOVE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_attr, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_REMOVE_CFM */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_REQ
CsrSize ATT_EXCHANGE_MTU_REQ_TSizeof(void *msg)
{
    ATT_EXCHANGE_MTU_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_REQ
CsrUint8 *ATT_EXCHANGE_MTU_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXCHANGE_MTU_REQ_T *primitive;

    primitive = (ATT_EXCHANGE_MTU_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_REQ
void *ATT_EXCHANGE_MTU_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXCHANGE_MTU_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXCHANGE_MTU_REQ_T *) CsrPmemZalloc(sizeof(ATT_EXCHANGE_MTU_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_ATT_READ_BLOB_REQ
CsrSize ATT_READ_BLOB_REQ_TSizeof(void *msg)
{
    ATT_READ_BLOB_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BLOB_REQ */

#ifndef EXCLUDE_ATT_READ_BLOB_REQ
CsrUint8 *ATT_READ_BLOB_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BLOB_REQ_T *primitive;

    primitive = (ATT_READ_BLOB_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BLOB_REQ */

#ifndef EXCLUDE_ATT_READ_BLOB_REQ
void *ATT_READ_BLOB_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BLOB_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BLOB_REQ_T *) CsrPmemZalloc(sizeof(ATT_READ_BLOB_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BLOB_REQ */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_RSP
CsrSize ATT_EXCHANGE_MTU_RSP_TSizeof(void *msg)
{
    ATT_EXCHANGE_MTU_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_RSP */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_RSP
CsrUint8 *ATT_EXCHANGE_MTU_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXCHANGE_MTU_RSP_T *primitive;

    primitive = (ATT_EXCHANGE_MTU_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->server_mtu);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_RSP */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_RSP
void *ATT_EXCHANGE_MTU_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXCHANGE_MTU_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXCHANGE_MTU_RSP_T *) CsrPmemZalloc(sizeof(ATT_EXCHANGE_MTU_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->server_mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_RSP */

#ifndef EXCLUDE_ATT_DISCONNECT_CFM
CsrSize ATT_DISCONNECT_CFM_TSizeof(void *msg)
{
    ATT_DISCONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_DISCONNECT_CFM */

#ifndef EXCLUDE_ATT_DISCONNECT_CFM
CsrUint8 *ATT_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_DISCONNECT_CFM_T *primitive;

    primitive = (ATT_DISCONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_ATT_DISCONNECT_CFM */

#ifndef EXCLUDE_ATT_DISCONNECT_CFM
void *ATT_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_DISCONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_DISCONNECT_CFM_T *) CsrPmemZalloc(sizeof(ATT_DISCONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->reason = (l2ca_disc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_DISCONNECT_CFM */

#ifndef EXCLUDE_ATT_UNREGISTER_CFM
CsrSize ATT_UNREGISTER_CFM_TSizeof(void *msg)
{
    ATT_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_UNREGISTER_CFM */

#ifndef EXCLUDE_ATT_UNREGISTER_CFM
CsrUint8 *ATT_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_UNREGISTER_CFM_T *primitive;

    primitive = (ATT_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_ATT_UNREGISTER_CFM */

#ifndef EXCLUDE_ATT_UNREGISTER_CFM
void *ATT_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(ATT_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_UNREGISTER_CFM */

#ifndef EXCLUDE_ATT_READ_MULTI_CFM
CsrSize ATT_READ_MULTI_CFM_TSizeof(void *msg)
{
    ATT_READ_MULTI_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_MULTI_CFM */

#ifndef EXCLUDE_ATT_READ_MULTI_CFM
CsrUint8 *ATT_READ_MULTI_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_MULTI_CFM_T *primitive;

    primitive = (ATT_READ_MULTI_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_MULTI_CFM */

#ifndef EXCLUDE_ATT_READ_MULTI_CFM
void *ATT_READ_MULTI_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_MULTI_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_MULTI_CFM_T *) CsrPmemZalloc(sizeof(ATT_READ_MULTI_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_MULTI_CFM */

#ifndef EXCLUDE_ATT_READ_MULTI_CFM
void ATT_READ_MULTI_CFM_TSerFree(void *msg)
{
    ATT_READ_MULTI_CFM_T *primitive;

    primitive = (ATT_READ_MULTI_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_MULTI_CFM */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ
CsrSize ATT_FIND_BY_TYPE_VALUE_REQ_TSizeof(void *msg)
{
    ATT_FIND_BY_TYPE_VALUE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ
CsrUint8 *ATT_FIND_BY_TYPE_VALUE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_FIND_BY_TYPE_VALUE_REQ_T *primitive;

    primitive = (ATT_FIND_BY_TYPE_VALUE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ
void *ATT_FIND_BY_TYPE_VALUE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_FIND_BY_TYPE_VALUE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_FIND_BY_TYPE_VALUE_REQ_T *) CsrPmemZalloc(sizeof(ATT_FIND_BY_TYPE_VALUE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ */

#ifndef EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ
void ATT_FIND_BY_TYPE_VALUE_REQ_TSerFree(void *msg)
{
    ATT_FIND_BY_TYPE_VALUE_REQ_T *primitive;

    primitive = (ATT_FIND_BY_TYPE_VALUE_REQ_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_FIND_BY_TYPE_VALUE_REQ */

#ifndef EXCLUDE_ATT_CONNECT_RSP
CsrSize ATT_CONNECT_RSP_TSizeof(void *msg)
{
    ATT_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_CONNECT_RSP */

#ifndef EXCLUDE_ATT_CONNECT_RSP
CsrUint8 *ATT_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_CONNECT_RSP_T *primitive;

    primitive = (ATT_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);

    return buffer;
}
#endif /* EXCLUDE_ATT_CONNECT_RSP */

#ifndef EXCLUDE_ATT_CONNECT_RSP
void *ATT_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(ATT_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_conn_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_CONNECT_RSP */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM
CsrSize ATT_READ_BY_GROUP_TYPE_CFM_TSizeof(void *msg)
{
    ATT_READ_BY_GROUP_TYPE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_value * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM
CsrUint8 *ATT_READ_BY_GROUP_TYPE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BY_GROUP_TYPE_CFM_T *primitive;

    primitive = (ATT_READ_BY_GROUP_TYPE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_value);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM
void *ATT_READ_BY_GROUP_TYPE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BY_GROUP_TYPE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BY_GROUP_TYPE_CFM_T *) CsrPmemZalloc(sizeof(ATT_READ_BY_GROUP_TYPE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->size_value)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->size_value * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->size_value * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM */

#ifndef EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM
void ATT_READ_BY_GROUP_TYPE_CFM_TSerFree(void *msg)
{
    ATT_READ_BY_GROUP_TYPE_CFM_T *primitive;

    primitive = (ATT_READ_BY_GROUP_TYPE_CFM_T *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_BY_GROUP_TYPE_CFM */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_IND
CsrSize ATT_EXCHANGE_MTU_IND_TSizeof(void *msg)
{
    ATT_EXCHANGE_MTU_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_IND */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_IND
CsrUint8 *ATT_EXCHANGE_MTU_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXCHANGE_MTU_IND_T *primitive;

    primitive = (ATT_EXCHANGE_MTU_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->client_mtu);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_IND */

#ifndef EXCLUDE_ATT_EXCHANGE_MTU_IND
void *ATT_EXCHANGE_MTU_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXCHANGE_MTU_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXCHANGE_MTU_IND_T *) CsrPmemZalloc(sizeof(ATT_EXCHANGE_MTU_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->client_mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXCHANGE_MTU_IND */

#ifndef EXCLUDE_ATT_READ_REQ
CsrSize ATT_READ_REQ_TSizeof(void *msg)
{
    ATT_READ_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_REQ */

#ifndef EXCLUDE_ATT_READ_REQ
CsrUint8 *ATT_READ_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_REQ_T *primitive;

    primitive = (ATT_READ_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_REQ */

#ifndef EXCLUDE_ATT_READ_REQ
void *ATT_READ_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_REQ_T *) CsrPmemZalloc(sizeof(ATT_READ_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_REQ */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_RSP
CsrSize ATT_HANDLE_VALUE_RSP_TSizeof(void *msg)
{
    ATT_HANDLE_VALUE_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_RSP */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_RSP
CsrUint8 *ATT_HANDLE_VALUE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_HANDLE_VALUE_RSP_T *primitive;

    primitive = (ATT_HANDLE_VALUE_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_RSP */

#ifndef EXCLUDE_ATT_HANDLE_VALUE_RSP
void *ATT_HANDLE_VALUE_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_HANDLE_VALUE_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_HANDLE_VALUE_RSP_T *) CsrPmemZalloc(sizeof(ATT_HANDLE_VALUE_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_HANDLE_VALUE_RSP */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_REQ
CsrSize ATT_READ_BY_TYPE_REQ_TSizeof(void *msg)
{
    ATT_READ_BY_TYPE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_REQ */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_REQ
CsrUint8 *ATT_READ_BY_TYPE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_BY_TYPE_REQ_T *primitive;

    primitive = (ATT_READ_BY_TYPE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuid_type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid), ((CsrSize) 4 * (4)));

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_REQ */

#ifndef EXCLUDE_ATT_READ_BY_TYPE_REQ
void *ATT_READ_BY_TYPE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_BY_TYPE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_BY_TYPE_REQ_T *) CsrPmemZalloc(sizeof(ATT_READ_BY_TYPE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuid_type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid), buffer, &offset, ((CsrSize) 4 * (4)));

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_BY_TYPE_REQ */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_REQ
CsrSize ATT_EXECUTE_WRITE_REQ_TSizeof(void *msg)
{
    ATT_EXECUTE_WRITE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_REQ */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_REQ
CsrUint8 *ATT_EXECUTE_WRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_EXECUTE_WRITE_REQ_T *primitive;

    primitive = (ATT_EXECUTE_WRITE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_REQ */

#ifndef EXCLUDE_ATT_EXECUTE_WRITE_REQ
void *ATT_EXECUTE_WRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_EXECUTE_WRITE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_EXECUTE_WRITE_REQ_T *) CsrPmemZalloc(sizeof(ATT_EXECUTE_WRITE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_ATT_EXECUTE_WRITE_REQ */

#ifndef EXCLUDE_ATT_READ_MULTI_REQ
CsrSize ATT_READ_MULTI_REQ_TSizeof(void *msg)
{
    ATT_READ_MULTI_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_handles * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_ATT_READ_MULTI_REQ */

#ifndef EXCLUDE_ATT_READ_MULTI_REQ
CsrUint8 *ATT_READ_MULTI_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    ATT_READ_MULTI_REQ_T *primitive;

    primitive = (ATT_READ_MULTI_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_handles);
    CsrMemCpySer(buffer, length, &primitive->handles, sizeof(primitive->handles));
    if(primitive->handles)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->handles), ((CsrSize) (primitive->size_handles * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_ATT_READ_MULTI_REQ */

#ifndef EXCLUDE_ATT_READ_MULTI_REQ
void *ATT_READ_MULTI_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    ATT_READ_MULTI_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (ATT_READ_MULTI_REQ_T *) CsrPmemZalloc(sizeof(ATT_READ_MULTI_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_handles, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handles), buffer, &offset, ((CsrSize) sizeof(primitive->handles)));
    if (primitive->size_handles)
    {
        primitive->handles = CsrPmemZalloc(((CsrUint32) (primitive->size_handles * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->handles), buffer, &offset, ((CsrSize) (primitive->size_handles * sizeof(CsrUint16))));
    }
    else
    {
        primitive->handles = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_ATT_READ_MULTI_REQ */

#ifndef EXCLUDE_ATT_READ_MULTI_REQ
void ATT_READ_MULTI_REQ_TSerFree(void *msg)
{
    ATT_READ_MULTI_REQ_T *primitive;

    primitive = (ATT_READ_MULTI_REQ_T *) msg;

    if(primitive->handles != NULL)
    {
        CsrPmemFree(primitive->handles);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_ATT_READ_MULTI_REQ */

#endif /* EXCLUDE_ATT_MODULE */
