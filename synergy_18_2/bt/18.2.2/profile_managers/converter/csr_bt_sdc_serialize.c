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
#include "sdc_prim.h"

#ifndef EXCLUDE_SDC_MODULE

void CsrBtSdcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ
CsrSize SDC_TERMINATE_PRIMITIVE_REQ_TSizeof(void *msg)
{
    SDC_TERMINATE_PRIMITIVE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ */

#ifndef EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ
CsrUint8 *SDC_TERMINATE_PRIMITIVE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_TERMINATE_PRIMITIVE_REQ_T *primitive;

    primitive = (SDC_TERMINATE_PRIMITIVE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ */

#ifndef EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ
void *SDC_TERMINATE_PRIMITIVE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_TERMINATE_PRIMITIVE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_TERMINATE_PRIMITIVE_REQ_T *) CsrPmemZalloc(sizeof(SDC_TERMINATE_PRIMITIVE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_TERMINATE_PRIMITIVE_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_REQ
CsrSize SDC_OPEN_SEARCH_REQ_TSizeof(void *msg)
{
    SDC_OPEN_SEARCH_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_REQ
CsrUint8 *SDC_OPEN_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_OPEN_SEARCH_REQ_T *primitive;

    primitive = (SDC_OPEN_SEARCH_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_REQ
void *SDC_OPEN_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_OPEN_SEARCH_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_OPEN_SEARCH_REQ_T *) CsrPmemZalloc(sizeof(SDC_OPEN_SEARCH_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_IND
CsrSize SDC_CLOSE_SEARCH_IND_TSizeof(void *msg)
{
    SDC_CLOSE_SEARCH_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_IND
CsrUint8 *SDC_CLOSE_SEARCH_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_CLOSE_SEARCH_IND_T *primitive;

    primitive = (SDC_CLOSE_SEARCH_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_IND
void *SDC_CLOSE_SEARCH_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_CLOSE_SEARCH_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_CLOSE_SEARCH_IND_T *) CsrPmemZalloc(sizeof(SDC_CLOSE_SEARCH_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ
CsrSize SDC_SERVICE_ATTRIBUTE_REQ_TSizeof(void *msg)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_attr_list * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ
CsrUint8 *SDC_SERVICE_ATTRIBUTE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *primitive;

    primitive = (SDC_SERVICE_ATTRIBUTE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->svc_rec_hndl);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_attr_list);
    CsrMemCpySer(buffer, length, &primitive->attr_list, sizeof(primitive->attr_list));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_num_attr);
    if(primitive->attr_list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attr_list), ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ
void *SDC_SERVICE_ATTRIBUTE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_ATTRIBUTE_REQ_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_ATTRIBUTE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->svc_rec_hndl, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_attr_list, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attr_list), buffer, &offset, ((CsrSize) sizeof(primitive->attr_list)));
    CsrUint16Des((CsrUint16 *) &primitive->max_num_attr, buffer, &offset);
    if (primitive->size_attr_list)
    {
        primitive->attr_list = CsrPmemZalloc(((CsrUint32) (primitive->size_attr_list * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attr_list), buffer, &offset, ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attr_list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ
void SDC_SERVICE_ATTRIBUTE_REQ_TSerFree(void *msg)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *primitive;

    primitive = (SDC_SERVICE_ATTRIBUTE_REQ_T *) msg;

    if(primitive->attr_list != NULL)
    {
        CsrPmemFree(primitive->attr_list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_CFM
CsrSize SDC_OPEN_SEARCH_CFM_TSizeof(void *msg)
{
    SDC_OPEN_SEARCH_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_CFM */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_CFM
CsrUint8 *SDC_OPEN_SEARCH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_OPEN_SEARCH_CFM_T *primitive;

    primitive = (SDC_OPEN_SEARCH_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_CFM */

#ifndef EXCLUDE_SDC_OPEN_SEARCH_CFM
void *SDC_OPEN_SEARCH_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_OPEN_SEARCH_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_OPEN_SEARCH_CFM_T *) CsrPmemZalloc(sizeof(SDC_OPEN_SEARCH_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_OPEN_SEARCH_CFM */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_REQ
CsrSize SDC_CLOSE_SEARCH_REQ_TSizeof(void *msg)
{
    SDC_CLOSE_SEARCH_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_REQ
CsrUint8 *SDC_CLOSE_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_CLOSE_SEARCH_REQ_T *primitive;

    primitive = (SDC_CLOSE_SEARCH_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CLOSE_SEARCH_REQ
void *SDC_CLOSE_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_CLOSE_SEARCH_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_CLOSE_SEARCH_REQ_T *) CsrPmemZalloc(sizeof(SDC_CLOSE_SEARCH_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_CLOSE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_CFM
CsrSize SDC_SERVICE_SEARCH_CFM_TSizeof(void *msg)
{
    SDC_SERVICE_SEARCH_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_rec_list * sizeof(CsrUint8)) + (primitive->size_err_info * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_CFM
CsrUint8 *SDC_SERVICE_SEARCH_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_SEARCH_CFM_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->num_recs_ret);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_rec_list);
    CsrMemCpySer(buffer, length, &primitive->rec_list, sizeof(primitive->rec_list));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->err_code);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_err_info);
    CsrMemCpySer(buffer, length, &primitive->err_info, sizeof(primitive->err_info));
    if(primitive->rec_list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->rec_list), ((CsrSize) (primitive->size_rec_list * sizeof(CsrUint8))));
    }
    if(primitive->err_info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->err_info), ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_CFM
void *SDC_SERVICE_SEARCH_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_SEARCH_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_SEARCH_CFM_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_SEARCH_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->num_recs_ret, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_rec_list, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->rec_list), buffer, &offset, ((CsrSize) sizeof(primitive->rec_list)));
    CsrUint16Des((CsrUint16 *) &primitive->response, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->err_code, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_err_info, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->err_info), buffer, &offset, ((CsrSize) sizeof(primitive->err_info)));
    if (primitive->size_rec_list)
    {
        primitive->rec_list = CsrPmemZalloc(((CsrUint32) (primitive->size_rec_list * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->rec_list), buffer, &offset, ((CsrSize) (primitive->size_rec_list * sizeof(CsrUint8))));
    }
    else
    {
        primitive->rec_list = NULL;
    }
    if (primitive->size_err_info)
    {
        primitive->err_info = CsrPmemZalloc(((CsrUint32) (primitive->size_err_info * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->err_info), buffer, &offset, ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }
    else
    {
        primitive->err_info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_CFM
void SDC_SERVICE_SEARCH_CFM_TSerFree(void *msg)
{
    SDC_SERVICE_SEARCH_CFM_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_CFM_T *) msg;

    if(primitive->rec_list != NULL)
    {
        CsrPmemFree(primitive->rec_list);
    }
    if(primitive->err_info != NULL)
    {
        CsrPmemFree(primitive->err_info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_SDC_CONFIG_REQ
CsrSize SDC_CONFIG_REQ_TSizeof(void *msg)
{
    SDC_CONFIG_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_CONFIG_REQ */

#ifndef EXCLUDE_SDC_CONFIG_REQ
CsrUint8 *SDC_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_CONFIG_REQ_T *primitive;

    primitive = (SDC_CONFIG_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2cap_mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_SDC_CONFIG_REQ */

#ifndef EXCLUDE_SDC_CONFIG_REQ
void *SDC_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_CONFIG_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_CONFIG_REQ_T *) CsrPmemZalloc(sizeof(SDC_CONFIG_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2cap_mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_CONFIG_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM
CsrSize SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSizeof(void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_attr_list * sizeof(CsrUint8)) + (primitive->size_err_info * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM
CsrUint8 *SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->total_response_size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_attr_list);
    CsrMemCpySer(buffer, length, &primitive->attr_list, sizeof(primitive->attr_list));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->more_to_come);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->err_code);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_err_info);
    CsrMemCpySer(buffer, length, &primitive->err_info, sizeof(primitive->err_info));
    if(primitive->attr_list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attr_list), ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    if(primitive->err_info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->err_info), ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM
void *SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->total_response_size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_attr_list, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attr_list), buffer, &offset, ((CsrSize) sizeof(primitive->attr_list)));
    CsrUint8Des((CsrUint8 *) &primitive->more_to_come, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->response, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->err_code, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_err_info, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->err_info), buffer, &offset, ((CsrSize) sizeof(primitive->err_info)));
    if (primitive->size_attr_list)
    {
        primitive->attr_list = CsrPmemZalloc(((CsrUint32) (primitive->size_attr_list * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attr_list), buffer, &offset, ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attr_list = NULL;
    }
    if (primitive->size_err_info)
    {
        primitive->err_info = CsrPmemZalloc(((CsrUint32) (primitive->size_err_info * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->err_info), buffer, &offset, ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }
    else
    {
        primitive->err_info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM
void SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_TSerFree(void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *) msg;

    if(primitive->attr_list != NULL)
    {
        CsrPmemFree(primitive->attr_list);
    }
    if(primitive->err_info != NULL)
    {
        CsrPmemFree(primitive->err_info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ
CsrSize SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSizeof(void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_srch_pttrn * sizeof(CsrUint8)) + (primitive->size_attr_list * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ
CsrUint8 *SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_srch_pttrn);
    CsrMemCpySer(buffer, length, &primitive->srch_pttrn, sizeof(primitive->srch_pttrn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_attr_list);
    CsrMemCpySer(buffer, length, &primitive->attr_list, sizeof(primitive->attr_list));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_num_attr);
    if(primitive->srch_pttrn)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->srch_pttrn), ((CsrSize) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
    }
    if(primitive->attr_list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attr_list), ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ
void *SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_srch_pttrn, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->srch_pttrn), buffer, &offset, ((CsrSize) sizeof(primitive->srch_pttrn)));
    CsrUint16Des((CsrUint16 *) &primitive->size_attr_list, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attr_list), buffer, &offset, ((CsrSize) sizeof(primitive->attr_list)));
    CsrUint16Des((CsrUint16 *) &primitive->max_num_attr, buffer, &offset);
    if (primitive->size_srch_pttrn)
    {
        primitive->srch_pttrn = CsrPmemZalloc(((CsrUint32) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->srch_pttrn), buffer, &offset, ((CsrSize) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
    }
    else
    {
        primitive->srch_pttrn = NULL;
    }
    if (primitive->size_attr_list)
    {
        primitive->attr_list = CsrPmemZalloc(((CsrUint32) (primitive->size_attr_list * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attr_list), buffer, &offset, ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attr_list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ
void SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_TSerFree(void *msg)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *) msg;

    if(primitive->srch_pttrn != NULL)
    {
        CsrPmemFree(primitive->srch_pttrn);
    }
    if(primitive->attr_list != NULL)
    {
        CsrPmemFree(primitive->attr_list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_ATTRIBUTE_REQ */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM
CsrSize SDC_SERVICE_ATTRIBUTE_CFM_TSizeof(void *msg)
{
    SDC_SERVICE_ATTRIBUTE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_attr_list * sizeof(CsrUint8)) + (primitive->size_err_info * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM
CsrUint8 *SDC_SERVICE_ATTRIBUTE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_ATTRIBUTE_CFM_T *primitive;

    primitive = (SDC_SERVICE_ATTRIBUTE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_attr_list);
    CsrMemCpySer(buffer, length, &primitive->attr_list, sizeof(primitive->attr_list));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->err_code);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_err_info);
    CsrMemCpySer(buffer, length, &primitive->err_info, sizeof(primitive->err_info));
    if(primitive->attr_list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attr_list), ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    if(primitive->err_info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->err_info), ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM
void *SDC_SERVICE_ATTRIBUTE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_ATTRIBUTE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_ATTRIBUTE_CFM_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_ATTRIBUTE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_attr_list, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attr_list), buffer, &offset, ((CsrSize) sizeof(primitive->attr_list)));
    CsrUint16Des((CsrUint16 *) &primitive->response, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->err_code, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_err_info, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->err_info), buffer, &offset, ((CsrSize) sizeof(primitive->err_info)));
    if (primitive->size_attr_list)
    {
        primitive->attr_list = CsrPmemZalloc(((CsrUint32) (primitive->size_attr_list * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attr_list), buffer, &offset, ((CsrSize) (primitive->size_attr_list * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attr_list = NULL;
    }
    if (primitive->size_err_info)
    {
        primitive->err_info = CsrPmemZalloc(((CsrUint32) (primitive->size_err_info * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->err_info), buffer, &offset, ((CsrSize) (primitive->size_err_info * sizeof(CsrUint8))));
    }
    else
    {
        primitive->err_info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM
void SDC_SERVICE_ATTRIBUTE_CFM_TSerFree(void *msg)
{
    SDC_SERVICE_ATTRIBUTE_CFM_T *primitive;

    primitive = (SDC_SERVICE_ATTRIBUTE_CFM_T *) msg;

    if(primitive->attr_list != NULL)
    {
        CsrPmemFree(primitive->attr_list);
    }
    if(primitive->err_info != NULL)
    {
        CsrPmemFree(primitive->err_info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_ATTRIBUTE_CFM */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_REQ
CsrSize SDC_SERVICE_SEARCH_REQ_TSizeof(void *msg)
{
    SDC_SERVICE_SEARCH_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->size_srch_pttrn * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_REQ
CsrUint8 *SDC_SERVICE_SEARCH_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_SERVICE_SEARCH_REQ_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->size_srch_pttrn);
    CsrMemCpySer(buffer, length, &primitive->srch_pttrn, sizeof(primitive->srch_pttrn));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_num_recs);
    if(primitive->srch_pttrn)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->srch_pttrn), ((CsrSize) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_REQ
void *SDC_SERVICE_SEARCH_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_SERVICE_SEARCH_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_SERVICE_SEARCH_REQ_T *) CsrPmemZalloc(sizeof(SDC_SERVICE_SEARCH_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->size_srch_pttrn, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->srch_pttrn), buffer, &offset, ((CsrSize) sizeof(primitive->srch_pttrn)));
    CsrUint16Des((CsrUint16 *) &primitive->max_num_recs, buffer, &offset);
    if (primitive->size_srch_pttrn)
    {
        primitive->srch_pttrn = CsrPmemZalloc(((CsrUint32) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->srch_pttrn), buffer, &offset, ((CsrSize) (primitive->size_srch_pttrn * sizeof(CsrUint8))));
    }
    else
    {
        primitive->srch_pttrn = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_SERVICE_SEARCH_REQ
void SDC_SERVICE_SEARCH_REQ_TSerFree(void *msg)
{
    SDC_SERVICE_SEARCH_REQ_T *primitive;

    primitive = (SDC_SERVICE_SEARCH_REQ_T *) msg;

    if(primitive->srch_pttrn != NULL)
    {
        CsrPmemFree(primitive->srch_pttrn);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_SDC_CONFIG_CFM
CsrSize SDC_CONFIG_CFM_TSizeof(void *msg)
{
    SDC_CONFIG_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_SDC_CONFIG_CFM */

#ifndef EXCLUDE_SDC_CONFIG_CFM
CsrUint8 *SDC_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    SDC_CONFIG_CFM_T *primitive;

    primitive = (SDC_CONFIG_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2cap_mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_SDC_CONFIG_CFM */

#ifndef EXCLUDE_SDC_CONFIG_CFM
void *SDC_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    SDC_CONFIG_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (SDC_CONFIG_CFM_T *) CsrPmemZalloc(sizeof(SDC_CONFIG_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2cap_mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_SDC_CONFIG_CFM */

#endif /* EXCLUDE_SDC_MODULE */
