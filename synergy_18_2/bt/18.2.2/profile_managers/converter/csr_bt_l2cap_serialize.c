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
#include "l2cap_prim.h"

#ifndef EXCLUDE_L2CAP_MODULE

void CsrBtL2capPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP
CsrSize L2CA_CONNECTION_PAR_UPDATE_RSP_TSizeof(void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_RSP_T *primitive;

    primitive = (L2CA_CONNECTION_PAR_UPDATE_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->signal_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP
void *L2CA_CONNECTION_PAR_UPDATE_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECTION_PAR_UPDATE_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECTION_PAR_UPDATE_RSP_T *) CsrPmemZalloc(sizeof(L2CA_CONNECTION_PAR_UPDATE_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->signal_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_RSP */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_IND
CsrSize L2CA_AUTO_CONNECT_IND_TSizeof(void *msg)
{
    L2CA_AUTO_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_IND */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_IND
CsrUint8 *L2CA_AUTO_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_AUTO_CONNECT_IND_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_IND */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_IND
void *L2CA_AUTO_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_AUTO_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_AUTO_CONNECT_IND_T *) CsrPmemZalloc(sizeof(L2CA_AUTO_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_IND */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_REQ
CsrSize L2CA_ROUTE_DATA_REQ_TSizeof(void *msg)
{
    L2CA_ROUTE_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_REQ
CsrUint8 *L2CA_ROUTE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_ROUTE_DATA_REQ_T *primitive;

    primitive = (L2CA_ROUTE_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_REQ
void *L2CA_ROUTE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_ROUTE_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_ROUTE_DATA_REQ_T *) CsrPmemZalloc(sizeof(L2CA_ROUTE_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_CFM
CsrSize L2CA_DATAWRITE_ABORT_CFM_TSizeof(void *msg)
{
    L2CA_DATAWRITE_ABORT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_CFM */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_CFM
CsrUint8 *L2CA_DATAWRITE_ABORT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAWRITE_ABORT_CFM_T *primitive;

    primitive = (L2CA_DATAWRITE_ABORT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_CFM */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_CFM
void *L2CA_DATAWRITE_ABORT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAWRITE_ABORT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAWRITE_ABORT_CFM_T *) CsrPmemZalloc(sizeof(L2CA_DATAWRITE_ABORT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_CFM */

#ifndef EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND
CsrSize L2CA_UNKNOWN_SIGNAL_IND_TSizeof(void *msg)
{
    L2CA_UNKNOWN_SIGNAL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND */

#ifndef EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND
CsrUint8 *L2CA_UNKNOWN_SIGNAL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNKNOWN_SIGNAL_IND_T *primitive;

    primitive = (L2CA_UNKNOWN_SIGNAL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND */

#ifndef EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND
void *L2CA_UNKNOWN_SIGNAL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNKNOWN_SIGNAL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNKNOWN_SIGNAL_IND_T *) CsrPmemZalloc(sizeof(L2CA_UNKNOWN_SIGNAL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNKNOWN_SIGNAL_IND */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_REQ
CsrSize L2CA_DATAWRITE_ABORT_REQ_TSizeof(void *msg)
{
    L2CA_DATAWRITE_ABORT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_REQ
CsrUint8 *L2CA_DATAWRITE_ABORT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAWRITE_ABORT_REQ_T *primitive;

    primitive = (L2CA_DATAWRITE_ABORT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_ABORT_REQ
void *L2CA_DATAWRITE_ABORT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAWRITE_ABORT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAWRITE_ABORT_REQ_T *) CsrPmemZalloc(sizeof(L2CA_DATAWRITE_ABORT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_ABORT_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_REQ
CsrSize L2CA_DATAWRITE_REQ_TSizeof(void *msg)
{
    L2CA_DATAWRITE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAWRITE_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_REQ
CsrUint8 *L2CA_DATAWRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAWRITE_REQ_T *primitive;

    primitive = (L2CA_DATAWRITE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packets_ack);
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_REQ
void *L2CA_DATAWRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAWRITE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAWRITE_REQ_T *) CsrPmemZalloc(sizeof(L2CA_DATAWRITE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint16Des((CsrUint16 *) &primitive->packets_ack, buffer, &offset);
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_REQ
void L2CA_DATAWRITE_REQ_TSerFree(void *msg)
{
    L2CA_DATAWRITE_REQ_T *primitive;

    primitive = (L2CA_DATAWRITE_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_DATAWRITE_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ
CsrSize L2CA_REGISTER_FIXED_CID_REQ_TSizeof(void *msg)
{
    L2CA_REGISTER_FIXED_CID_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ
CsrUint8 *L2CA_REGISTER_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_REGISTER_FIXED_CID_REQ_T *primitive;

    primitive = (L2CA_REGISTER_FIXED_CID_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fixed_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->streams.buffer_size_sink);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->streams.buffer_size_source);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ
void *L2CA_REGISTER_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_REGISTER_FIXED_CID_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_REGISTER_FIXED_CID_REQ_T *) CsrPmemZalloc(sizeof(L2CA_REGISTER_FIXED_CID_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fixed_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->streams.buffer_size_sink, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->streams.buffer_size_source, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ
void L2CA_REGISTER_FIXED_CID_REQ_TSerFree(void *msg)
{
    L2CA_REGISTER_FIXED_CID_REQ_T *primitive;

    primitive = (L2CA_REGISTER_FIXED_CID_REQ_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ
CsrSize L2CA_GET_CHANNEL_INFO_REQ_TSizeof(void *msg)
{
    L2CA_GET_CHANNEL_INFO_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ
CsrUint8 *L2CA_GET_CHANNEL_INFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_GET_CHANNEL_INFO_REQ_T *primitive;

    primitive = (L2CA_GET_CHANNEL_INFO_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ
void *L2CA_GET_CHANNEL_INFO_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_GET_CHANNEL_INFO_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_GET_CHANNEL_INFO_REQ_T *) CsrPmemZalloc(sizeof(L2CA_GET_CHANNEL_INFO_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_CFM
CsrSize L2CA_AUTO_CONNECT_CFM_TSizeof(void *msg)
{
    L2CA_AUTO_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_CFM
CsrUint8 *L2CA_AUTO_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_AUTO_CONNECT_CFM_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ext_feats);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_CFM
void *L2CA_AUTO_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_AUTO_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_AUTO_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(L2CA_AUTO_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ext_feats, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_CFM
void L2CA_AUTO_CONNECT_CFM_TSerFree(void *msg)
{
    L2CA_AUTO_CONNECT_CFM_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_CFM_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_REQ
CsrSize L2CA_MAP_FIXED_CID_REQ_TSizeof(void *msg)
{
    L2CA_MAP_FIXED_CID_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_REQ
CsrUint8 *L2CA_MAP_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MAP_FIXED_CID_REQ_T *primitive;

    primitive = (L2CA_MAP_FIXED_CID_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fixed_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cl_local_psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cl_remote_psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_REQ
void *L2CA_MAP_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MAP_FIXED_CID_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MAP_FIXED_CID_REQ_T *) CsrPmemZalloc(sizeof(L2CA_MAP_FIXED_CID_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fixed_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cl_local_psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cl_remote_psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_IND
CsrSize L2CA_UNROUTE_DATA_IND_TSizeof(void *msg)
{
    L2CA_UNROUTE_DATA_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_IND */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_IND
CsrUint8 *L2CA_UNROUTE_DATA_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNROUTE_DATA_IND_T *primitive;

    primitive = (L2CA_UNROUTE_DATA_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_IND */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_IND
void *L2CA_UNROUTE_DATA_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNROUTE_DATA_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNROUTE_DATA_IND_T *) CsrPmemZalloc(sizeof(L2CA_UNROUTE_DATA_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_IND */

#ifndef EXCLUDE_L2CA_AMP_LINK_LOSS_IND
CsrSize L2CA_AMP_LINK_LOSS_IND_TSizeof(void *msg)
{
    L2CA_AMP_LINK_LOSS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_L2CA_AMP_LINK_LOSS_IND
CsrUint8 *L2CA_AMP_LINK_LOSS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_AMP_LINK_LOSS_IND_T *primitive;

    primitive = (L2CA_AMP_LINK_LOSS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_L2CA_AMP_LINK_LOSS_IND
void *L2CA_AMP_LINK_LOSS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_AMP_LINK_LOSS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_AMP_LINK_LOSS_IND_T *) CsrPmemZalloc(sizeof(L2CA_AMP_LINK_LOSS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_REQ
CsrSize L2CA_AUTO_CONNECT_REQ_TSizeof(void *msg)
{
    L2CA_AUTO_CONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->conftab_length * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_REQ
CsrUint8 *L2CA_AUTO_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_AUTO_CONNECT_REQ_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_remote);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conftab_length);
    CsrMemCpySer(buffer, length, &primitive->conftab, sizeof(primitive->conftab));
    if(primitive->conftab)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->conftab), ((CsrSize) (primitive->conftab_length * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_REQ
void *L2CA_AUTO_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_AUTO_CONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_AUTO_CONNECT_REQ_T *) CsrPmemZalloc(sizeof(L2CA_AUTO_CONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_remote, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conftab_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conftab), buffer, &offset, ((CsrSize) sizeof(primitive->conftab)));
    if (primitive->conftab_length)
    {
        primitive->conftab = CsrPmemZalloc(((CsrUint32) (primitive->conftab_length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->conftab), buffer, &offset, ((CsrSize) (primitive->conftab_length * sizeof(CsrUint16))));
    }
    else
    {
        primitive->conftab = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_REQ
void L2CA_AUTO_CONNECT_REQ_TSerFree(void *msg)
{
    L2CA_AUTO_CONNECT_REQ_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_REQ_T *) msg;

    if(primitive->conftab != NULL)
    {
        CsrPmemFree(primitive->conftab);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_DEBUG_DROP_REQ
CsrSize L2CA_DEBUG_DROP_REQ_TSizeof(void *msg)
{
    L2CA_DEBUG_DROP_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DEBUG_DROP_REQ */

#ifndef EXCLUDE_L2CA_DEBUG_DROP_REQ
CsrUint8 *L2CA_DEBUG_DROP_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DEBUG_DROP_REQ_T *primitive;

    primitive = (L2CA_DEBUG_DROP_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_number);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tx_number);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DEBUG_DROP_REQ */

#ifndef EXCLUDE_L2CA_DEBUG_DROP_REQ
void *L2CA_DEBUG_DROP_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DEBUG_DROP_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DEBUG_DROP_REQ_T *) CsrPmemZalloc(sizeof(L2CA_DEBUG_DROP_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rx_number, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tx_number, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DEBUG_DROP_REQ */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND
CsrSize L2CA_CONNECTION_PAR_UPDATE_IND_TSizeof(void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_IND_T *primitive;

    primitive = (L2CA_CONNECTION_PAR_UPDATE_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addrt.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->signal_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_min);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_interval_max);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervision_timeout);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND
void *L2CA_CONNECTION_PAR_UPDATE_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECTION_PAR_UPDATE_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECTION_PAR_UPDATE_IND_T *) CsrPmemZalloc(sizeof(L2CA_CONNECTION_PAR_UPDATE_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addrt.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->signal_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_min, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_interval_max, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervision_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_RSP
CsrSize L2CA_CREATE_CHANNEL_RSP_TSizeof(void *msg)
{
    L2CA_CREATE_CHANNEL_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_RSP
CsrUint8 *L2CA_CREATE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CREATE_CHANNEL_RSP_T *primitive;

    primitive = (L2CA_CREATE_CHANNEL_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_RSP
void *L2CA_CREATE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CREATE_CHANNEL_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CREATE_CHANNEL_RSP_T *) CsrPmemZalloc(sizeof(L2CA_CREATE_CHANNEL_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_DATAREAD_IND
CsrSize L2CA_DATAREAD_IND_TSizeof(void *msg)
{
    L2CA_DATAREAD_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAREAD_IND */

#ifndef EXCLUDE_L2CA_DATAREAD_IND
CsrUint8 *L2CA_DATAREAD_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAREAD_IND_T *primitive;

    primitive = (L2CA_DATAREAD_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packets);
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAREAD_IND */

#ifndef EXCLUDE_L2CA_DATAREAD_IND
void *L2CA_DATAREAD_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAREAD_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAREAD_IND_T *) CsrPmemZalloc(sizeof(L2CA_DATAREAD_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_data_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->packets, buffer, &offset);
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAREAD_IND */

#ifndef EXCLUDE_L2CA_DATAREAD_IND
void L2CA_DATAREAD_IND_TSerFree(void *msg)
{
    L2CA_DATAREAD_IND_T *primitive;

    primitive = (L2CA_DATAREAD_IND_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_DATAREAD_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CFM
CsrSize L2CA_MOVE_CHANNEL_CFM_TSizeof(void *msg)
{
    L2CA_MOVE_CHANNEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CFM
CsrUint8 *L2CA_MOVE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MOVE_CHANNEL_CFM_T *primitive;

    primitive = (L2CA_MOVE_CHANNEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CFM
void *L2CA_MOVE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MOVE_CHANNEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MOVE_CHANNEL_CFM_T *) CsrPmemZalloc(sizeof(L2CA_MOVE_CHANNEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_move_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_RSP
CsrSize L2CA_CONFIG_RSP_TSizeof(void *msg)
{
    L2CA_CONFIG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONFIG_RSP */

#ifndef EXCLUDE_L2CA_CONFIG_RSP
CsrUint8 *L2CA_CONFIG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONFIG_RSP_T *primitive;

    primitive = (L2CA_CONFIG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONFIG_RSP */

#ifndef EXCLUDE_L2CA_CONFIG_RSP
void *L2CA_CONFIG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONFIG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONFIG_RSP_T *) CsrPmemZalloc(sizeof(L2CA_CONFIG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_conf_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONFIG_RSP */

#ifndef EXCLUDE_L2CA_CONFIG_RSP
void L2CA_CONFIG_RSP_TSerFree(void *msg)
{
    L2CA_CONFIG_RSP_T *primitive;

    primitive = (L2CA_CONFIG_RSP_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_CONFIG_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_IND
CsrSize L2CA_MOVE_CHANNEL_IND_TSizeof(void *msg)
{
    L2CA_MOVE_CHANNEL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_IND
CsrUint8 *L2CA_MOVE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MOVE_CHANNEL_IND_T *primitive;

    primitive = (L2CA_MOVE_CHANNEL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_IND
void *L2CA_MOVE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MOVE_CHANNEL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MOVE_CHANNEL_IND_T *) CsrPmemZalloc(sizeof(L2CA_MOVE_CHANNEL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_REQ
CsrSize L2CA_MOVE_CHANNEL_REQ_TSizeof(void *msg)
{
    L2CA_MOVE_CHANNEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_REQ
CsrUint8 *L2CA_MOVE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MOVE_CHANNEL_REQ_T *primitive;

    primitive = (L2CA_MOVE_CHANNEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_REQ
void *L2CA_MOVE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MOVE_CHANNEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MOVE_CHANNEL_REQ_T *) CsrPmemZalloc(sizeof(L2CA_MOVE_CHANNEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_BUSY_REQ
CsrSize L2CA_BUSY_REQ_TSizeof(void *msg)
{
    L2CA_BUSY_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_BUSY_REQ */

#ifndef EXCLUDE_L2CA_BUSY_REQ
CsrUint8 *L2CA_BUSY_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_BUSY_REQ_T *primitive;

    primitive = (L2CA_BUSY_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->busy);

    return buffer;
}
#endif /* EXCLUDE_L2CA_BUSY_REQ */

#ifndef EXCLUDE_L2CA_BUSY_REQ
void *L2CA_BUSY_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_BUSY_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_BUSY_REQ_T *) CsrPmemZalloc(sizeof(L2CA_BUSY_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->busy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_BUSY_REQ */

#ifndef EXCLUDE_L2CA_DATAWRITE_CFM
CsrSize L2CA_DATAWRITE_CFM_TSizeof(void *msg)
{
    L2CA_DATAWRITE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAWRITE_CFM */

#ifndef EXCLUDE_L2CA_DATAWRITE_CFM
CsrUint8 *L2CA_DATAWRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAWRITE_CFM_T *primitive;

    primitive = (L2CA_DATAWRITE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_CFM */

#ifndef EXCLUDE_L2CA_DATAWRITE_CFM
void *L2CA_DATAWRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAWRITE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAWRITE_CFM_T *) CsrPmemZalloc(sizeof(L2CA_DATAWRITE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_data_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAWRITE_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_IND
CsrSize L2CA_MAP_FIXED_CID_IND_TSizeof(void *msg)
{
    L2CA_MAP_FIXED_CID_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_IND
CsrUint8 *L2CA_MAP_FIXED_CID_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MAP_FIXED_CID_IND_T *primitive;

    primitive = (L2CA_MAP_FIXED_CID_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fixed_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cl_local_psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_IND
void *L2CA_MAP_FIXED_CID_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MAP_FIXED_CID_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MAP_FIXED_CID_IND_T *) CsrPmemZalloc(sizeof(L2CA_MAP_FIXED_CID_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fixed_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cl_local_psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_CONFIG_CFM
CsrSize L2CA_CONFIG_CFM_TSizeof(void *msg)
{
    L2CA_CONFIG_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONFIG_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_CFM
CsrUint8 *L2CA_CONFIG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONFIG_CFM_T *primitive;

    primitive = (L2CA_CONFIG_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONFIG_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_CFM
void *L2CA_CONFIG_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONFIG_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONFIG_CFM_T *) CsrPmemZalloc(sizeof(L2CA_CONFIG_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_conf_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONFIG_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_CFM
void L2CA_CONFIG_CFM_TSerFree(void *msg)
{
    L2CA_CONFIG_CFM_T *primitive;

    primitive = (L2CA_CONFIG_CFM_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_CONFIG_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_RSP
CsrSize L2CA_DISCONNECT_RSP_TSizeof(void *msg)
{
    L2CA_DISCONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DISCONNECT_RSP */

#ifndef EXCLUDE_L2CA_DISCONNECT_RSP
CsrUint8 *L2CA_DISCONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DISCONNECT_RSP_T *primitive;

    primitive = (L2CA_DISCONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_RSP */

#ifndef EXCLUDE_L2CA_DISCONNECT_RSP
void *L2CA_DISCONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DISCONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DISCONNECT_RSP_T *) CsrPmemZalloc(sizeof(L2CA_DISCONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_RSP */

#ifndef EXCLUDE_L2CA_REGISTER_CFM
CsrSize L2CA_REGISTER_CFM_TSizeof(void *msg)
{
    L2CA_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_CFM
CsrUint8 *L2CA_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_REGISTER_CFM_T *primitive;

    primitive = (L2CA_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_CFM
void *L2CA_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(L2CA_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM
CsrSize L2CA_REGISTER_FIXED_CID_CFM_TSizeof(void *msg)
{
    L2CA_REGISTER_FIXED_CID_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM
CsrUint8 *L2CA_REGISTER_FIXED_CID_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_REGISTER_FIXED_CID_CFM_T *primitive;

    primitive = (L2CA_REGISTER_FIXED_CID_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fixed_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM
void *L2CA_REGISTER_FIXED_CID_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_REGISTER_FIXED_CID_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_REGISTER_FIXED_CID_CFM_T *) CsrPmemZalloc(sizeof(L2CA_REGISTER_FIXED_CID_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fixed_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_REGISTER_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_MTU_VIOLATION_IND
CsrSize L2CA_MTU_VIOLATION_IND_TSizeof(void *msg)
{
    L2CA_MTU_VIOLATION_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MTU_VIOLATION_IND */

#ifndef EXCLUDE_L2CA_MTU_VIOLATION_IND
CsrUint8 *L2CA_MTU_VIOLATION_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MTU_VIOLATION_IND_T *primitive;

    primitive = (L2CA_MTU_VIOLATION_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->signal_mtu);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MTU_VIOLATION_IND */

#ifndef EXCLUDE_L2CA_MTU_VIOLATION_IND
void *L2CA_MTU_VIOLATION_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MTU_VIOLATION_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MTU_VIOLATION_IND_T *) CsrPmemZalloc(sizeof(L2CA_MTU_VIOLATION_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->signal_mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MTU_VIOLATION_IND */

#ifndef EXCLUDE_L2CA_BUSY_IND
CsrSize L2CA_BUSY_IND_TSizeof(void *msg)
{
    L2CA_BUSY_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_BUSY_IND */

#ifndef EXCLUDE_L2CA_BUSY_IND
CsrUint8 *L2CA_BUSY_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_BUSY_IND_T *primitive;

    primitive = (L2CA_BUSY_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->busy);

    return buffer;
}
#endif /* EXCLUDE_L2CA_BUSY_IND */

#ifndef EXCLUDE_L2CA_BUSY_IND
void *L2CA_BUSY_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_BUSY_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_BUSY_IND_T *) CsrPmemZalloc(sizeof(L2CA_BUSY_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->busy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_BUSY_IND */

#ifndef EXCLUDE_L2CA_RAW_DATA_REQ
CsrSize L2CA_RAW_DATA_REQ_TSizeof(void *msg)
{
    L2CA_RAW_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_RAW_DATA_REQ */

#ifndef EXCLUDE_L2CA_RAW_DATA_REQ
CsrUint8 *L2CA_RAW_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_RAW_DATA_REQ_T *primitive;

    primitive = (L2CA_RAW_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->raw_length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flush_to);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_RAW_DATA_REQ */

#ifndef EXCLUDE_L2CA_RAW_DATA_REQ
void *L2CA_RAW_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_RAW_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_RAW_DATA_REQ_T *) CsrPmemZalloc(sizeof(L2CA_RAW_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->raw_length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flush_to, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_RAW_DATA_REQ */

#ifndef EXCLUDE_L2CA_RAW_DATA_REQ
void L2CA_RAW_DATA_REQ_TSerFree(void *msg)
{
    L2CA_RAW_DATA_REQ_T *primitive;

    primitive = (L2CA_RAW_DATA_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_RAW_DATA_REQ */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM
CsrSize L2CA_GET_CHANNEL_INFO_CFM_TSizeof(void *msg)
{
    L2CA_GET_CHANNEL_INFO_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM
CsrUint8 *L2CA_GET_CHANNEL_INFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_GET_CHANNEL_INFO_CFM_T *primitive;

    primitive = (L2CA_GET_CHANNEL_INFO_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_handle);

    return buffer;
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM
void *L2CA_GET_CHANNEL_INFO_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_GET_CHANNEL_INFO_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_GET_CHANNEL_INFO_CFM_T *) CsrPmemZalloc(sizeof(L2CA_GET_CHANNEL_INFO_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_CFM
CsrSize L2CA_ROUTE_DATA_CFM_TSizeof(void *msg)
{
    L2CA_ROUTE_DATA_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_CFM
CsrUint8 *L2CA_ROUTE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_ROUTE_DATA_CFM_T *primitive;

    primitive = (L2CA_ROUTE_DATA_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->out_mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_ROUTE_DATA_CFM
void *L2CA_ROUTE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_ROUTE_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_ROUTE_DATA_CFM_T *) CsrPmemZalloc(sizeof(L2CA_ROUTE_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->out_mtu, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_ROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_CFM
CsrSize L2CA_CONNECT_CFM_TSizeof(void *msg)
{
    L2CA_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_CFM
CsrUint8 *L2CA_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECT_CFM_T *primitive;

    primitive = (L2CA_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_CFM
void *L2CA_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(L2CA_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ
CsrSize L2CA_UNMAP_FIXED_CID_REQ_TSizeof(void *msg)
{
    L2CA_UNMAP_FIXED_CID_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ
CsrUint8 *L2CA_UNMAP_FIXED_CID_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNMAP_FIXED_CID_REQ_T *primitive;

    primitive = (L2CA_UNMAP_FIXED_CID_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ
void *L2CA_UNMAP_FIXED_CID_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNMAP_FIXED_CID_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNMAP_FIXED_CID_REQ_T *) CsrPmemZalloc(sizeof(L2CA_UNMAP_FIXED_CID_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_REQ
CsrSize L2CA_CREATE_CHANNEL_REQ_TSizeof(void *msg)
{
    L2CA_CREATE_CHANNEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_REQ
CsrUint8 *L2CA_CREATE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CREATE_CHANNEL_REQ_T *primitive;

    primitive = (L2CA_CREATE_CHANNEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_remote);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_REQ
void *L2CA_CREATE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CREATE_CHANNEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CREATE_CHANNEL_REQ_T *) CsrPmemZalloc(sizeof(L2CA_CREATE_CHANNEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_remote, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_REQ */

#ifndef EXCLUDE_L2CA_PING_CFM
CsrSize L2CA_PING_CFM_TSizeof(void *msg)
{
    L2CA_PING_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_PING_CFM */

#ifndef EXCLUDE_L2CA_PING_CFM
CsrUint8 *L2CA_PING_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_PING_CFM_T *primitive;

    primitive = (L2CA_PING_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_PING_CFM */

#ifndef EXCLUDE_L2CA_PING_CFM
void *L2CA_PING_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_PING_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_PING_CFM_T *) CsrPmemZalloc(sizeof(L2CA_PING_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_PING_CFM */

#ifndef EXCLUDE_L2CA_PING_CFM
void L2CA_PING_CFM_TSerFree(void *msg)
{
    L2CA_PING_CFM_T *primitive;

    primitive = (L2CA_PING_CFM_T *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_PING_CFM */

#ifndef EXCLUDE_L2CA_REGISTER_REQ
CsrSize L2CA_REGISTER_REQ_TSizeof(void *msg)
{
    L2CA_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_REQ
CsrUint8 *L2CA_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_REGISTER_REQ_T *primitive;

    primitive = (L2CA_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_oriented.buffer_size_sink);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connection_oriented.buffer_size_source);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connectionless.buffer_size_sink);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connectionless.buffer_size_source);

    return buffer;
}
#endif /* EXCLUDE_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_L2CA_REGISTER_REQ
void *L2CA_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(L2CA_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_oriented.buffer_size_sink, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connection_oriented.buffer_size_source, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connectionless.buffer_size_sink, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connectionless.buffer_size_source, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_L2CA_CONFIG_REQ
CsrSize L2CA_CONFIG_REQ_TSizeof(void *msg)
{
    L2CA_CONFIG_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONFIG_REQ */

#ifndef EXCLUDE_L2CA_CONFIG_REQ
CsrUint8 *L2CA_CONFIG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONFIG_REQ_T *primitive;

    primitive = (L2CA_CONFIG_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONFIG_REQ */

#ifndef EXCLUDE_L2CA_CONFIG_REQ
void *L2CA_CONFIG_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONFIG_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONFIG_REQ_T *) CsrPmemZalloc(sizeof(L2CA_CONFIG_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONFIG_REQ */

#ifndef EXCLUDE_L2CA_CONFIG_REQ
void L2CA_CONFIG_REQ_TSerFree(void *msg)
{
    L2CA_CONFIG_REQ_T *primitive;

    primitive = (L2CA_CONFIG_REQ_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_CONFIG_REQ */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_RSP
CsrSize L2CA_MAP_FIXED_CID_RSP_TSizeof(void *msg)
{
    L2CA_MAP_FIXED_CID_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_RSP */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_RSP
CsrUint8 *L2CA_MAP_FIXED_CID_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MAP_FIXED_CID_RSP_T *primitive;

    primitive = (L2CA_MAP_FIXED_CID_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucd_remote_psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_RSP */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_RSP
void *L2CA_MAP_FIXED_CID_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MAP_FIXED_CID_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MAP_FIXED_CID_RSP_T *) CsrPmemZalloc(sizeof(L2CA_MAP_FIXED_CID_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucd_remote_psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND
CsrSize L2CA_MOVE_CHANNEL_CMP_IND_TSizeof(void *msg)
{
    L2CA_MOVE_CHANNEL_CMP_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND
CsrUint8 *L2CA_MOVE_CHANNEL_CMP_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MOVE_CHANNEL_CMP_IND_T *primitive;

    primitive = (L2CA_MOVE_CHANNEL_CMP_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND
void *L2CA_MOVE_CHANNEL_CMP_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MOVE_CHANNEL_CMP_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MOVE_CHANNEL_CMP_IND_T *) CsrPmemZalloc(sizeof(L2CA_MOVE_CHANNEL_CMP_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_move_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM
CsrSize L2CA_CONNECTION_PAR_UPDATE_CFM_TSizeof(void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_CFM_T *primitive;

    primitive = (L2CA_CONNECTION_PAR_UPDATE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM
void *L2CA_CONNECTION_PAR_UPDATE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECTION_PAR_UPDATE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECTION_PAR_UPDATE_CFM_T *) CsrPmemZalloc(sizeof(L2CA_CONNECTION_PAR_UPDATE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_CFM */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_RSP
CsrSize L2CA_AUTO_CONNECT_RSP_TSizeof(void *msg)
{
    L2CA_AUTO_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->conftab_length * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_RSP
CsrUint8 *L2CA_AUTO_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_AUTO_CONNECT_RSP_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conftab_length);
    CsrMemCpySer(buffer, length, &primitive->conftab, sizeof(primitive->conftab));
    if(primitive->conftab)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->conftab), ((CsrSize) (primitive->conftab_length * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_RSP
void *L2CA_AUTO_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_AUTO_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_AUTO_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(L2CA_AUTO_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conftab_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conftab), buffer, &offset, ((CsrSize) sizeof(primitive->conftab)));
    if (primitive->conftab_length)
    {
        primitive->conftab = CsrPmemZalloc(((CsrUint32) (primitive->conftab_length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->conftab), buffer, &offset, ((CsrSize) (primitive->conftab_length * sizeof(CsrUint16))));
    }
    else
    {
        primitive->conftab = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_AUTO_CONNECT_RSP
void L2CA_AUTO_CONNECT_RSP_TSerFree(void *msg)
{
    L2CA_AUTO_CONNECT_RSP_T *primitive;

    primitive = (L2CA_AUTO_CONNECT_RSP_T *) msg;

    if(primitive->conftab != NULL)
    {
        CsrPmemFree(primitive->conftab);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_AUTO_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_CONNECT_REQ
CsrSize L2CA_CONNECT_REQ_TSizeof(void *msg)
{
    L2CA_CONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_CONNECT_REQ
CsrUint8 *L2CA_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECT_REQ_T *primitive;

    primitive = (L2CA_CONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_remote);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.protocol_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->service.channel);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_CONNECT_REQ
void *L2CA_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECT_REQ_T *) CsrPmemZalloc(sizeof(L2CA_CONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_remote, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.protocol_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->service.channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_CFM
CsrSize L2CA_DISCONNECT_CFM_TSizeof(void *msg)
{
    L2CA_DISCONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_CFM
CsrUint8 *L2CA_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DISCONNECT_CFM_T *primitive;

    primitive = (L2CA_DISCONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_CFM
void *L2CA_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DISCONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DISCONNECT_CFM_T *) CsrPmemZalloc(sizeof(L2CA_DISCONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_disc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_L2CA_PING_REQ
CsrSize L2CA_PING_REQ_TSizeof(void *msg)
{
    L2CA_PING_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_PING_REQ */

#ifndef EXCLUDE_L2CA_PING_REQ
CsrUint8 *L2CA_PING_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_PING_REQ_T *primitive;

    primitive = (L2CA_PING_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_PING_REQ */

#ifndef EXCLUDE_L2CA_PING_REQ
void *L2CA_PING_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_PING_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_PING_REQ_T *) CsrPmemZalloc(sizeof(L2CA_PING_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_PING_REQ */

#ifndef EXCLUDE_L2CA_PING_REQ
void L2CA_PING_REQ_TSerFree(void *msg)
{
    L2CA_PING_REQ_T *primitive;

    primitive = (L2CA_PING_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_PING_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_CFM
CsrSize L2CA_UNROUTE_DATA_CFM_TSizeof(void *msg)
{
    L2CA_UNROUTE_DATA_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_CFM
CsrUint8 *L2CA_UNROUTE_DATA_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNROUTE_DATA_CFM_T *primitive;

    primitive = (L2CA_UNROUTE_DATA_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_CFM
void *L2CA_UNROUTE_DATA_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNROUTE_DATA_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNROUTE_DATA_CFM_T *) CsrPmemZalloc(sizeof(L2CA_UNROUTE_DATA_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_CFM */

#ifndef EXCLUDE_L2CA_CONNECT_IND
CsrSize L2CA_CONNECT_IND_TSizeof(void *msg)
{
    L2CA_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECT_IND */

#ifndef EXCLUDE_L2CA_CONNECT_IND
CsrUint8 *L2CA_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECT_IND_T *primitive;

    primitive = (L2CA_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECT_IND */

#ifndef EXCLUDE_L2CA_CONNECT_IND
void *L2CA_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECT_IND_T *) CsrPmemZalloc(sizeof(L2CA_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECT_IND */

#ifndef EXCLUDE_L2CA_RAW_MODE_REQ
CsrSize L2CA_RAW_MODE_REQ_TSizeof(void *msg)
{
    L2CA_RAW_MODE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_RAW_MODE_REQ */

#ifndef EXCLUDE_L2CA_RAW_MODE_REQ
CsrUint8 *L2CA_RAW_MODE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_RAW_MODE_REQ_T *primitive;

    primitive = (L2CA_RAW_MODE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->raw_mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_L2CA_RAW_MODE_REQ */

#ifndef EXCLUDE_L2CA_RAW_MODE_REQ
void *L2CA_RAW_MODE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_RAW_MODE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_RAW_MODE_REQ_T *) CsrPmemZalloc(sizeof(L2CA_RAW_MODE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->raw_mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_RAW_MODE_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_REQ
CsrSize L2CA_DISCONNECT_REQ_TSizeof(void *msg)
{
    L2CA_DISCONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_REQ
CsrUint8 *L2CA_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DISCONNECT_REQ_T *primitive;

    primitive = (L2CA_DISCONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_L2CA_DISCONNECT_REQ
void *L2CA_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DISCONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DISCONNECT_REQ_T *) CsrPmemZalloc(sizeof(L2CA_DISCONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_L2CA_UNREGISTER_CFM
CsrSize L2CA_UNREGISTER_CFM_TSizeof(void *msg)
{
    L2CA_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNREGISTER_CFM */

#ifndef EXCLUDE_L2CA_UNREGISTER_CFM
CsrUint8 *L2CA_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNREGISTER_CFM_T *primitive;

    primitive = (L2CA_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNREGISTER_CFM */

#ifndef EXCLUDE_L2CA_UNREGISTER_CFM
void *L2CA_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(L2CA_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNREGISTER_CFM */

#ifndef EXCLUDE_L2CA_UNREGISTER_REQ
CsrSize L2CA_UNREGISTER_REQ_TSizeof(void *msg)
{
    L2CA_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_L2CA_UNREGISTER_REQ
CsrUint8 *L2CA_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNREGISTER_REQ_T *primitive;

    primitive = (L2CA_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_L2CA_UNREGISTER_REQ
void *L2CA_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(L2CA_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_L2CA_TIMEOUT_IND
CsrSize L2CA_TIMEOUT_IND_TSizeof(void *msg)
{
    L2CA_TIMEOUT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_TIMEOUT_IND */

#ifndef EXCLUDE_L2CA_TIMEOUT_IND
CsrUint8 *L2CA_TIMEOUT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_TIMEOUT_IND_T *primitive;

    primitive = (L2CA_TIMEOUT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);

    return buffer;
}
#endif /* EXCLUDE_L2CA_TIMEOUT_IND */

#ifndef EXCLUDE_L2CA_TIMEOUT_IND
void *L2CA_TIMEOUT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_TIMEOUT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_TIMEOUT_IND_T *) CsrPmemZalloc(sizeof(L2CA_TIMEOUT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_TIMEOUT_IND */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_RSP
CsrSize L2CA_MOVE_CHANNEL_RSP_TSizeof(void *msg)
{
    L2CA_MOVE_CHANNEL_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_RSP
CsrUint8 *L2CA_MOVE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MOVE_CHANNEL_RSP_T *primitive;

    primitive = (L2CA_MOVE_CHANNEL_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_MOVE_CHANNEL_RSP
void *L2CA_MOVE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MOVE_CHANNEL_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MOVE_CHANNEL_RSP_T *) CsrPmemZalloc(sizeof(L2CA_MOVE_CHANNEL_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_move_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_REQ
CsrSize L2CA_UNROUTE_DATA_REQ_TSizeof(void *msg)
{
    L2CA_UNROUTE_DATA_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_REQ
CsrUint8 *L2CA_UNROUTE_DATA_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNROUTE_DATA_REQ_T *primitive;

    primitive = (L2CA_UNROUTE_DATA_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_UNROUTE_DATA_REQ
void *L2CA_UNROUTE_DATA_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNROUTE_DATA_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNROUTE_DATA_REQ_T *) CsrPmemZalloc(sizeof(L2CA_UNROUTE_DATA_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNROUTE_DATA_REQ */

#ifndef EXCLUDE_L2CA_GETINFO_REQ
CsrSize L2CA_GETINFO_REQ_TSizeof(void *msg)
{
    L2CA_GETINFO_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_GETINFO_REQ */

#ifndef EXCLUDE_L2CA_GETINFO_REQ
CsrUint8 *L2CA_GETINFO_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_GETINFO_REQ_T *primitive;

    primitive = (L2CA_GETINFO_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->info_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_GETINFO_REQ */

#ifndef EXCLUDE_L2CA_GETINFO_REQ
void *L2CA_GETINFO_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_GETINFO_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_GETINFO_REQ_T *) CsrPmemZalloc(sizeof(L2CA_GETINFO_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->info_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_GETINFO_REQ */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_CFM
CsrSize L2CA_CREATE_CHANNEL_CFM_TSizeof(void *msg)
{
    L2CA_CREATE_CHANNEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_CFM
CsrUint8 *L2CA_CREATE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CREATE_CHANNEL_CFM_T *primitive;

    primitive = (L2CA_CREATE_CHANNEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_CFM
void *L2CA_CREATE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CREATE_CHANNEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CREATE_CHANNEL_CFM_T *) CsrPmemZalloc(sizeof(L2CA_CREATE_CHANNEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_CFM */

#ifndef EXCLUDE_L2CA_CONFIG_IND
CsrSize L2CA_CONFIG_IND_TSizeof(void *msg)
{
    L2CA_CONFIG_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->config.unknown_length * sizeof(CsrUint8)) + (primitive->config.qos ? sizeof(*primitive->config.qos) : 0) + (primitive->config.flow ? sizeof(*primitive->config.flow) : 0) + (primitive->config.flowspec ? sizeof(*primitive->config.flowspec) : 0));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONFIG_IND */

#ifndef EXCLUDE_L2CA_CONFIG_IND
CsrUint8 *L2CA_CONFIG_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONFIG_IND_T *primitive;

    primitive = (L2CA_CONFIG_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.hints);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.unknown_length);
    CsrMemCpySer(buffer, length, &primitive->config.unknown, sizeof(primitive->config.unknown));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flush_to);
    CsrMemCpySer(buffer, length, &primitive->config.qos, sizeof(primitive->config.qos));
    CsrMemCpySer(buffer, length, &primitive->config.flow, sizeof(primitive->config.flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.fcs);
    CsrMemCpySer(buffer, length, &primitive->config.flowspec, sizeof(primitive->config.flowspec));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.ext_window);
    if(primitive->config.unknown)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->config.unknown), ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    if(primitive->config.qos)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.qos->service_type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_rate);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->token_bucket);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->peak_bw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.qos->delay_var);
    }
    if(primitive->config.flow)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->tx_window);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flow->max_retransmit);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->retrans_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->monitor_timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flow->maximum_pdu);
    }
    if(primitive->config.flowspec)
    {
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->config.flowspec->fs_service_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config.flowspec->fs_max_sdu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_interarrival);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_latency);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config.flowspec->fs_flush_to);
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONFIG_IND */

#ifndef EXCLUDE_L2CA_CONFIG_IND
void *L2CA_CONFIG_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONFIG_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONFIG_IND_T *) CsrPmemZalloc(sizeof(L2CA_CONFIG_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.hints, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.unknown_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.unknown), buffer, &offset, ((CsrSize) sizeof(primitive->config.unknown)));
    CsrUint16Des((CsrUint16 *) &primitive->config.mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flush_to, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.qos), buffer, &offset, ((CsrSize) sizeof(primitive->config.qos)));
    CsrMemCpyDes(((void *) &primitive->config.flow), buffer, &offset, ((CsrSize) sizeof(primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.fcs, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->config.flowspec), buffer, &offset, ((CsrSize) sizeof(primitive->config.flowspec)));
    CsrUint16Des((CsrUint16 *) &primitive->config.ext_window, buffer, &offset);
    if (primitive->config.unknown_length)
    {
        primitive->config.unknown = CsrPmemZalloc(((CsrUint32) (primitive->config.unknown_length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->config.unknown), buffer, &offset, ((CsrSize) (primitive->config.unknown_length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->config.unknown = NULL;
    }
    if (primitive->config.qos)
    {
        primitive->config.qos = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.qos)));
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.qos->service_type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_rate, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->token_bucket, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->peak_bw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.qos->delay_var, buffer, &offset);
    }
    if (primitive->config.flow)
    {
        primitive->config.flow = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flow)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->tx_window, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flow->max_retransmit, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->retrans_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->monitor_timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flow->maximum_pdu, buffer, &offset);
    }
    if (primitive->config.flowspec)
    {
        primitive->config.flowspec = CsrPmemZalloc((CsrUint32) (sizeof(*primitive->config.flowspec)));
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->config.flowspec->fs_service_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config.flowspec->fs_max_sdu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_interarrival, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_latency, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config.flowspec->fs_flush_to, buffer, &offset);
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONFIG_IND */

#ifndef EXCLUDE_L2CA_CONFIG_IND
void L2CA_CONFIG_IND_TSerFree(void *msg)
{
    L2CA_CONFIG_IND_T *primitive;

    primitive = (L2CA_CONFIG_IND_T *) msg;

    if(primitive->config.unknown != NULL)
    {
        CsrPmemFree(primitive->config.unknown);
    }
    if(primitive->config.qos != NULL)
    {
        CsrPmemFree(primitive->config.qos);
    }
    if(primitive->config.flow != NULL)
    {
        CsrPmemFree(primitive->config.flow);
    }
    if(primitive->config.flowspec != NULL)
    {
        CsrPmemFree(primitive->config.flowspec);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_CONFIG_IND */

#ifndef EXCLUDE_L2CA_DATAREAD_RSP
CsrSize L2CA_DATAREAD_RSP_TSizeof(void *msg)
{
    L2CA_DATAREAD_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DATAREAD_RSP */

#ifndef EXCLUDE_L2CA_DATAREAD_RSP
CsrUint8 *L2CA_DATAREAD_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DATAREAD_RSP_T *primitive;

    primitive = (L2CA_DATAREAD_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packets);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DATAREAD_RSP */

#ifndef EXCLUDE_L2CA_DATAREAD_RSP
void *L2CA_DATAREAD_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DATAREAD_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DATAREAD_RSP_T *) CsrPmemZalloc(sizeof(L2CA_DATAREAD_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->packets, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_DATAREAD_RSP */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ
CsrSize L2CA_CONNECTION_PAR_UPDATE_REQ_TSizeof(void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ
CsrUint8 *L2CA_CONNECTION_PAR_UPDATE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECTION_PAR_UPDATE_REQ_T *primitive;

    primitive = (L2CA_CONNECTION_PAR_UPDATE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->min_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ */

#ifndef EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ
void *L2CA_CONNECTION_PAR_UPDATE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECTION_PAR_UPDATE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECTION_PAR_UPDATE_REQ_T *) CsrPmemZalloc(sizeof(L2CA_CONNECTION_PAR_UPDATE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->min_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECTION_PAR_UPDATE_REQ */

#ifndef EXCLUDE_L2CA_MULTICAST_REQ
CsrSize L2CA_MULTICAST_REQ_TSizeof(void *msg)
{
    L2CA_MULTICAST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MULTICAST_REQ */

#ifndef EXCLUDE_L2CA_MULTICAST_REQ
CsrUint8 *L2CA_MULTICAST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MULTICAST_REQ_T *primitive;

    primitive = (L2CA_MULTICAST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->cids), ((CsrSize) 2 * (7)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_MULTICAST_REQ */

#ifndef EXCLUDE_L2CA_MULTICAST_REQ
void *L2CA_MULTICAST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MULTICAST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MULTICAST_REQ_T *) CsrPmemZalloc(sizeof(L2CA_MULTICAST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->cids), buffer, &offset, ((CsrSize) 2 * (7)));
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_MULTICAST_REQ */

#ifndef EXCLUDE_L2CA_MULTICAST_REQ
void L2CA_MULTICAST_REQ_TSerFree(void *msg)
{
    L2CA_MULTICAST_REQ_T *primitive;

    primitive = (L2CA_MULTICAST_REQ_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_MULTICAST_REQ */

#ifndef EXCLUDE_L2CA_RAW_DATA_IND
CsrSize L2CA_RAW_DATA_IND_TSizeof(void *msg)
{
    L2CA_RAW_DATA_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_RAW_DATA_IND */

#ifndef EXCLUDE_L2CA_RAW_DATA_IND
CsrUint8 *L2CA_RAW_DATA_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_RAW_DATA_IND_T *primitive;

    primitive = (L2CA_RAW_DATA_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->packet_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_RAW_DATA_IND */

#ifndef EXCLUDE_L2CA_RAW_DATA_IND
void *L2CA_RAW_DATA_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_RAW_DATA_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_RAW_DATA_IND_T *) CsrPmemZalloc(sizeof(L2CA_RAW_DATA_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_data_result_t) data;
    }
    CsrUint8Des((CsrUint8 *) &primitive->packet_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_RAW_DATA_IND */

#ifndef EXCLUDE_L2CA_RAW_DATA_IND
void L2CA_RAW_DATA_IND_TSerFree(void *msg)
{
    L2CA_RAW_DATA_IND_T *primitive;

    primitive = (L2CA_RAW_DATA_IND_T *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_RAW_DATA_IND */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_CFM
CsrSize L2CA_MAP_FIXED_CID_CFM_TSizeof(void *msg)
{
    L2CA_MAP_FIXED_CID_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_CFM
CsrUint8 *L2CA_MAP_FIXED_CID_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_MAP_FIXED_CID_CFM_T *primitive;

    primitive = (L2CA_MAP_FIXED_CID_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addrt.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addrt.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addrt.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fixed_cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cl_local_psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_MAP_FIXED_CID_CFM
void *L2CA_MAP_FIXED_CID_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_MAP_FIXED_CID_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_MAP_FIXED_CID_CFM_T *) CsrPmemZalloc(sizeof(L2CA_MAP_FIXED_CID_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addrt.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addrt.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addrt.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->fixed_cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cl_local_psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_misc_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_MAP_FIXED_CID_CFM */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_IND
CsrSize L2CA_UNMAP_FIXED_CID_IND_TSizeof(void *msg)
{
    L2CA_UNMAP_FIXED_CID_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_IND
CsrUint8 *L2CA_UNMAP_FIXED_CID_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_UNMAP_FIXED_CID_IND_T *primitive;

    primitive = (L2CA_UNMAP_FIXED_CID_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_UNMAP_FIXED_CID_IND
void *L2CA_UNMAP_FIXED_CID_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_UNMAP_FIXED_CID_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_UNMAP_FIXED_CID_IND_T *) CsrPmemZalloc(sizeof(L2CA_UNMAP_FIXED_CID_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->reason = (l2ca_disc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_UNMAP_FIXED_CID_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_IND
CsrSize L2CA_CREATE_CHANNEL_IND_TSizeof(void *msg)
{
    L2CA_CREATE_CHANNEL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_IND
CsrUint8 *L2CA_CREATE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CREATE_CHANNEL_IND_T *primitive;

    primitive = (L2CA_CREATE_CHANNEL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm_local);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_CREATE_CHANNEL_IND
void *L2CA_CREATE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CREATE_CHANNEL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CREATE_CHANNEL_IND_T *) CsrPmemZalloc(sizeof(L2CA_CREATE_CHANNEL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm_local, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CREATE_CHANNEL_IND */

#ifndef EXCLUDE_L2CA_CONNECT_RSP
CsrSize L2CA_CONNECT_RSP_TSizeof(void *msg)
{
    L2CA_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_CONNECT_RSP
CsrUint8 *L2CA_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_CONNECT_RSP_T *primitive;

    primitive = (L2CA_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);

    return buffer;
}
#endif /* EXCLUDE_L2CA_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_CONNECT_RSP
void *L2CA_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(L2CA_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->response = (l2ca_conn_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_L2CA_CONNECT_RSP */

#ifndef EXCLUDE_L2CA_GETINFO_CFM
CsrSize L2CA_GETINFO_CFM_TSizeof(void *msg)
{
    L2CA_GETINFO_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_GETINFO_CFM */

#ifndef EXCLUDE_L2CA_GETINFO_CFM
CsrUint8 *L2CA_GETINFO_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_GETINFO_CFM_T *primitive;

    primitive = (L2CA_GETINFO_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->info_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->info_data, sizeof(primitive->info_data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->req_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    if(primitive->info_data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->info_data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_L2CA_GETINFO_CFM */

#ifndef EXCLUDE_L2CA_GETINFO_CFM
void *L2CA_GETINFO_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_GETINFO_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_GETINFO_CFM_T *) CsrPmemZalloc(sizeof(L2CA_GETINFO_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->info_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info_data), buffer, &offset, ((CsrSize) sizeof(primitive->info_data)));
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->result = (l2ca_info_result_t) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->req_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    if (primitive->length)
    {
        primitive->info_data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->info_data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->info_data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_GETINFO_CFM */

#ifndef EXCLUDE_L2CA_GETINFO_CFM
void L2CA_GETINFO_CFM_TSerFree(void *msg)
{
    L2CA_GETINFO_CFM_T *primitive;

    primitive = (L2CA_GETINFO_CFM_T *) msg;

    if(primitive->info_data != NULL)
    {
        CsrPmemFree(primitive->info_data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_L2CA_GETINFO_CFM */

#ifndef EXCLUDE_L2CA_DISCONNECT_IND
CsrSize L2CA_DISCONNECT_IND_TSizeof(void *msg)
{
    L2CA_DISCONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_L2CA_DISCONNECT_IND
CsrUint8 *L2CA_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    L2CA_DISCONNECT_IND_T *primitive;

    primitive = (L2CA_DISCONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reg_ctx);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->con_ctx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_L2CA_DISCONNECT_IND
void *L2CA_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    L2CA_DISCONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (L2CA_DISCONNECT_IND_T *) CsrPmemZalloc(sizeof(L2CA_DISCONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reg_ctx, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->con_ctx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->reason = (l2ca_disc_result_t) data;
    }

    return primitive;
}
#endif /* EXCLUDE_L2CA_DISCONNECT_IND */

#endif /* EXCLUDE_L2CAP_MODULE */
