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
#include "rfcomm_prim.h"

#ifndef EXCLUDE_RFCOMM_MODULE

void CsrBtRfcommPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_RFC_REGISTER_REQ
CsrSize RFC_REGISTER_REQ_TSizeof(void *msg)
{
    RFC_REGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_REGISTER_REQ */

#ifndef EXCLUDE_RFC_REGISTER_REQ
CsrUint8 *RFC_REGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_REGISTER_REQ_T *primitive;

    primitive = (RFC_REGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->loc_serv_chan_req);

    return buffer;
}
#endif /* EXCLUDE_RFC_REGISTER_REQ */

#ifndef EXCLUDE_RFC_REGISTER_REQ
void *RFC_REGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_REGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_REGISTER_REQ_T *) CsrPmemZalloc(sizeof(RFC_REGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->loc_serv_chan_req, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_REGISTER_REQ */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP
CsrSize RFC_L2CA_MOVE_CHANNEL_RSP_TSizeof(void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_RSP_T *primitive;

    primitive = (RFC_L2CA_MOVE_CHANNEL_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP
void *RFC_L2CA_MOVE_CHANNEL_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_MOVE_CHANNEL_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_MOVE_CHANNEL_RSP_T *) CsrPmemZalloc(sizeof(RFC_L2CA_MOVE_CHANNEL_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_RSP */

#ifndef EXCLUDE_RFC_DATAREAD_RSP
CsrSize RFC_DATAREAD_RSP_TSizeof(void *msg)
{
    RFC_DATAREAD_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DATAREAD_RSP */

#ifndef EXCLUDE_RFC_DATAREAD_RSP
CsrUint8 *RFC_DATAREAD_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DATAREAD_RSP_T *primitive;

    primitive = (RFC_DATAREAD_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);

    return buffer;
}
#endif /* EXCLUDE_RFC_DATAREAD_RSP */

#ifndef EXCLUDE_RFC_DATAREAD_RSP
void *RFC_DATAREAD_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DATAREAD_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DATAREAD_RSP_T *) CsrPmemZalloc(sizeof(RFC_DATAREAD_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DATAREAD_RSP */

#ifndef EXCLUDE_RFC_PORTNEG_REQ
CsrSize RFC_PORTNEG_REQ_TSizeof(void *msg)
{
    RFC_PORTNEG_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_PORTNEG_REQ */

#ifndef EXCLUDE_RFC_PORTNEG_REQ
CsrUint8 *RFC_PORTNEG_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_PORTNEG_REQ_T *primitive;

    primitive = (RFC_PORTNEG_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->request);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port_pars.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_RFC_PORTNEG_REQ */

#ifndef EXCLUDE_RFC_PORTNEG_REQ
void *RFC_PORTNEG_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_PORTNEG_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_PORTNEG_REQ_T *) CsrPmemZalloc(sizeof(RFC_PORTNEG_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->request, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port_pars.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_PORTNEG_REQ */

#ifndef EXCLUDE_RFC_REGISTER_CFM
CsrSize RFC_REGISTER_CFM_TSizeof(void *msg)
{
    RFC_REGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_REGISTER_CFM */

#ifndef EXCLUDE_RFC_REGISTER_CFM
CsrUint8 *RFC_REGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_REGISTER_CFM_T *primitive;

    primitive = (RFC_REGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->loc_serv_chan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_RFC_REGISTER_CFM */

#ifndef EXCLUDE_RFC_REGISTER_CFM
void *RFC_REGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_REGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_REGISTER_CFM_T *) CsrPmemZalloc(sizeof(RFC_REGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->loc_serv_chan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_REGISTER_CFM */

#ifndef EXCLUDE_RFC_TEST_CFM
CsrSize RFC_TEST_CFM_TSizeof(void *msg)
{
    RFC_TEST_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->test_data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_TEST_CFM */

#ifndef EXCLUDE_RFC_TEST_CFM
CsrUint8 *RFC_TEST_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_TEST_CFM_T *primitive;

    primitive = (RFC_TEST_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->test_data_length);
    CsrMemCpySer(buffer, length, &primitive->test_data, sizeof(primitive->test_data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    if (primitive->test_data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->test_data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->test_data, 0, mblkLength, &buffer[*length]);
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
#endif /* EXCLUDE_RFC_TEST_CFM */

#ifndef EXCLUDE_RFC_TEST_CFM
void *RFC_TEST_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_TEST_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_TEST_CFM_T *) CsrPmemZalloc(sizeof(RFC_TEST_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->test_data_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->test_data), buffer, &offset, ((CsrSize) sizeof(primitive->test_data)));
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->test_data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_TEST_CFM */

#ifndef EXCLUDE_RFC_TEST_CFM
void RFC_TEST_CFM_TSerFree(void *msg)
{
    RFC_TEST_CFM_T *primitive;

    primitive = (RFC_TEST_CFM_T *) msg;

    if(primitive->test_data != NULL)
    {
        CsrMblkDestroy(primitive->test_data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_RFC_TEST_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM
CsrSize RFC_L2CA_MOVE_CHANNEL_CFM_TSizeof(void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_CFM_T *primitive;

    primitive = (RFC_L2CA_MOVE_CHANNEL_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM
void *RFC_L2CA_MOVE_CHANNEL_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_MOVE_CHANNEL_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_MOVE_CHANNEL_CFM_T *) CsrPmemZalloc(sizeof(RFC_L2CA_MOVE_CHANNEL_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_RFC_ERROR_IND
CsrSize RFC_ERROR_IND_TSizeof(void *msg)
{
    RFC_ERROR_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_ERROR_IND */

#ifndef EXCLUDE_RFC_ERROR_IND
CsrUint8 *RFC_ERROR_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_ERROR_IND_T *primitive;

    primitive = (RFC_ERROR_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->err_prim_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_ERROR_IND */

#ifndef EXCLUDE_RFC_ERROR_IND
void *RFC_ERROR_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_ERROR_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_ERROR_IND_T *) CsrPmemZalloc(sizeof(RFC_ERROR_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->err_prim_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_ERROR_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_CFM
CsrSize RFC_SERVER_CONNECT_CFM_TSizeof(void *msg)
{
    RFC_SERVER_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_CFM */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_CFM
CsrUint8 *RFC_SERVER_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_SERVER_CONNECT_CFM_T *primitive;

    primitive = (RFC_SERVER_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serv_chan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_payload_size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_l2cap_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_l2cap_control);

    return buffer;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_CFM */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_CFM
void *RFC_SERVER_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_SERVER_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_SERVER_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(RFC_SERVER_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serv_chan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_payload_size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_l2cap_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_l2cap_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND
CsrSize RFC_L2CA_MOVE_CHANNEL_IND_TSizeof(void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_IND_T *primitive;

    primitive = (RFC_L2CA_MOVE_CHANNEL_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->identifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND
void *RFC_L2CA_MOVE_CHANNEL_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_MOVE_CHANNEL_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_MOVE_CHANNEL_IND_T *) CsrPmemZalloc(sizeof(RFC_L2CA_MOVE_CHANNEL_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->identifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_IND
CsrSize RFC_SERVER_CONNECT_IND_TSizeof(void *msg)
{
    RFC_SERVER_CONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_IND
CsrUint8 *RFC_SERVER_CONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_SERVER_CONNECT_IND_T *primitive;

    primitive = (RFC_SERVER_CONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->loc_serv_chan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_l2cap_control);

    return buffer;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_IND */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_IND
void *RFC_SERVER_CONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_SERVER_CONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_SERVER_CONNECT_IND_T *) CsrPmemZalloc(sizeof(RFC_SERVER_CONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->loc_serv_chan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_l2cap_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_IND */

#ifndef EXCLUDE_RFC_DATAWRITE_CFM
CsrSize RFC_DATAWRITE_CFM_TSizeof(void *msg)
{
    RFC_DATAWRITE_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DATAWRITE_CFM */

#ifndef EXCLUDE_RFC_DATAWRITE_CFM
CsrUint8 *RFC_DATAWRITE_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DATAWRITE_CFM_T *primitive;

    primitive = (RFC_DATAWRITE_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_DATAWRITE_CFM */

#ifndef EXCLUDE_RFC_DATAWRITE_CFM
void *RFC_DATAWRITE_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DATAWRITE_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DATAWRITE_CFM_T *) CsrPmemZalloc(sizeof(RFC_DATAWRITE_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DATAWRITE_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_IND
CsrSize RFC_PORTNEG_IND_TSizeof(void *msg)
{
    RFC_PORTNEG_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_PORTNEG_IND */

#ifndef EXCLUDE_RFC_PORTNEG_IND
CsrUint8 *RFC_PORTNEG_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_PORTNEG_IND_T *primitive;

    primitive = (RFC_PORTNEG_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->request);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port_pars.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_RFC_PORTNEG_IND */

#ifndef EXCLUDE_RFC_PORTNEG_IND
void *RFC_PORTNEG_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_PORTNEG_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_PORTNEG_IND_T *) CsrPmemZalloc(sizeof(RFC_PORTNEG_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->request, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port_pars.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_PORTNEG_IND */

#ifndef EXCLUDE_RFC_FC_CFM
CsrSize RFC_FC_CFM_TSizeof(void *msg)
{
    RFC_FC_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_FC_CFM */

#ifndef EXCLUDE_RFC_FC_CFM
CsrUint8 *RFC_FC_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_FC_CFM_T *primitive;

    primitive = (RFC_FC_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fc);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_FC_CFM */

#ifndef EXCLUDE_RFC_FC_CFM
void *RFC_FC_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_FC_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_FC_CFM_T *) CsrPmemZalloc(sizeof(RFC_FC_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->fc = (RFC_FC_T) data;
    }
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_FC_CFM */

#ifndef EXCLUDE_RFC_FC_IND
CsrSize RFC_FC_IND_TSizeof(void *msg)
{
    RFC_FC_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_FC_IND */

#ifndef EXCLUDE_RFC_FC_IND
CsrUint8 *RFC_FC_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_FC_IND_T *primitive;

    primitive = (RFC_FC_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fc);

    return buffer;
}
#endif /* EXCLUDE_RFC_FC_IND */

#ifndef EXCLUDE_RFC_FC_IND
void *RFC_FC_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_FC_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_FC_IND_T *) CsrPmemZalloc(sizeof(RFC_FC_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->fc = (RFC_FC_T) data;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_FC_IND */

#ifndef EXCLUDE_RFC_INIT_REQ
CsrSize RFC_INIT_REQ_TSizeof(void *msg)
{
    RFC_INIT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_INIT_REQ */

#ifndef EXCLUDE_RFC_INIT_REQ
CsrUint8 *RFC_INIT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_INIT_REQ_T *primitive;

    primitive = (RFC_INIT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_RFC_INIT_REQ */

#ifndef EXCLUDE_RFC_INIT_REQ
void *RFC_INIT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_INIT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_INIT_REQ_T *) CsrPmemZalloc(sizeof(RFC_INIT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_INIT_REQ */

#ifndef EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND
CsrSize RFC_L2CA_AMP_LINK_LOSS_IND_TSizeof(void *msg)
{
    RFC_L2CA_AMP_LINK_LOSS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND
CsrUint8 *RFC_L2CA_AMP_LINK_LOSS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_AMP_LINK_LOSS_IND_T *primitive;

    primitive = (RFC_L2CA_AMP_LINK_LOSS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND
void *RFC_L2CA_AMP_LINK_LOSS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_AMP_LINK_LOSS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_AMP_LINK_LOSS_IND_T *) CsrPmemZalloc(sizeof(RFC_L2CA_AMP_LINK_LOSS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_AMP_LINK_LOSS_IND */

#ifndef EXCLUDE_RFC_INIT_CFM
CsrSize RFC_INIT_CFM_TSizeof(void *msg)
{
    RFC_INIT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_INIT_CFM */

#ifndef EXCLUDE_RFC_INIT_CFM
CsrUint8 *RFC_INIT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_INIT_CFM_T *primitive;

    primitive = (RFC_INIT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_RFC_INIT_CFM */

#ifndef EXCLUDE_RFC_INIT_CFM
void *RFC_INIT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_INIT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_INIT_CFM_T *) CsrPmemZalloc(sizeof(RFC_INIT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_INIT_CFM */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ
CsrSize RFC_L2CA_MOVE_CHANNEL_REQ_TSizeof(void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_REQ_T *primitive;

    primitive = (RFC_L2CA_MOVE_CHANNEL_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ
void *RFC_L2CA_MOVE_CHANNEL_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_MOVE_CHANNEL_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_MOVE_CHANNEL_REQ_T *) CsrPmemZalloc(sizeof(RFC_L2CA_MOVE_CHANNEL_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_RFC_UNREGISTER_CFM
CsrSize RFC_UNREGISTER_CFM_TSizeof(void *msg)
{
    RFC_UNREGISTER_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_UNREGISTER_CFM */

#ifndef EXCLUDE_RFC_UNREGISTER_CFM
CsrUint8 *RFC_UNREGISTER_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_UNREGISTER_CFM_T *primitive;

    primitive = (RFC_UNREGISTER_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->loc_serv_chan);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_RFC_UNREGISTER_CFM */

#ifndef EXCLUDE_RFC_UNREGISTER_CFM
void *RFC_UNREGISTER_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_UNREGISTER_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_UNREGISTER_CFM_T *) CsrPmemZalloc(sizeof(RFC_UNREGISTER_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->loc_serv_chan, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_UNREGISTER_CFM */

#ifndef EXCLUDE_RFC_LINESTATUS_REQ
CsrSize RFC_LINESTATUS_REQ_TSizeof(void *msg)
{
    RFC_LINESTATUS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_LINESTATUS_REQ */

#ifndef EXCLUDE_RFC_LINESTATUS_REQ
CsrUint8 *RFC_LINESTATUS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_LINESTATUS_REQ_T *primitive;

    primitive = (RFC_LINESTATUS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->error_flag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->line_status);

    return buffer;
}
#endif /* EXCLUDE_RFC_LINESTATUS_REQ */

#ifndef EXCLUDE_RFC_LINESTATUS_REQ
void *RFC_LINESTATUS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_LINESTATUS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_LINESTATUS_REQ_T *) CsrPmemZalloc(sizeof(RFC_LINESTATUS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->error_flag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->line_status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_LINESTATUS_REQ */

#ifndef EXCLUDE_RFC_TEST_REQ
CsrSize RFC_TEST_REQ_TSizeof(void *msg)
{
    RFC_TEST_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->test_data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_TEST_REQ */

#ifndef EXCLUDE_RFC_TEST_REQ
CsrUint8 *RFC_TEST_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_TEST_REQ_T *primitive;

    primitive = (RFC_TEST_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->test_data_length);
    CsrMemCpySer(buffer, length, &primitive->test_data, sizeof(primitive->test_data));
    if (primitive->test_data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->test_data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->test_data, 0, mblkLength, &buffer[*length]);
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
#endif /* EXCLUDE_RFC_TEST_REQ */

#ifndef EXCLUDE_RFC_TEST_REQ
void *RFC_TEST_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_TEST_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_TEST_REQ_T *) CsrPmemZalloc(sizeof(RFC_TEST_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->test_data_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->test_data), buffer, &offset, ((CsrSize) sizeof(primitive->test_data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->test_data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_TEST_REQ */

#ifndef EXCLUDE_RFC_TEST_REQ
void RFC_TEST_REQ_TSerFree(void *msg)
{
    RFC_TEST_REQ_T *primitive;

    primitive = (RFC_TEST_REQ_T *) msg;

    if(primitive->test_data != NULL)
    {
        CsrMblkDestroy(primitive->test_data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_RFC_TEST_REQ */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_REQ
CsrSize RFC_CLIENT_CONNECT_REQ_TSizeof(void *msg)
{
    RFC_CLIENT_CONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->reserved_length * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_REQ */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_REQ
CsrUint8 *RFC_CLIENT_CONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_CLIENT_CONNECT_REQ_T *primitive;

    primitive = (RFC_CLIENT_CONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rem_serv_chan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->client_security_chan);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_payload_size);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->priority);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->total_credits);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_l2cap_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_l2cap_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reserved_length);
    CsrMemCpySer(buffer, length, &primitive->reserved, sizeof(primitive->reserved));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modem_signal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->msc_timeout);
    if(primitive->reserved)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->reserved), ((CsrSize) (primitive->reserved_length * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_REQ */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_REQ
void *RFC_CLIENT_CONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_CLIENT_CONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_CLIENT_CONNECT_REQ_T *) CsrPmemZalloc(sizeof(RFC_CLIENT_CONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rem_serv_chan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->client_security_chan, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_payload_size, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->priority, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->total_credits, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_l2cap_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_l2cap_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reserved_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->reserved), buffer, &offset, ((CsrSize) sizeof(primitive->reserved)));
    CsrUint8Des((CsrUint8 *) &primitive->modem_signal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->msc_timeout, buffer, &offset);
    if (primitive->reserved_length)
    {
        primitive->reserved = CsrPmemZalloc(((CsrUint32) (primitive->reserved_length * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->reserved), buffer, &offset, ((CsrSize) (primitive->reserved_length * sizeof(CsrUint16))));
    }
    else
    {
        primitive->reserved = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_REQ */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_REQ
void RFC_CLIENT_CONNECT_REQ_TSerFree(void *msg)
{
    RFC_CLIENT_CONNECT_REQ_T *primitive;

    primitive = (RFC_CLIENT_CONNECT_REQ_T *) msg;

    if(primitive->reserved != NULL)
    {
        CsrPmemFree(primitive->reserved);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_CFM
CsrSize RFC_DISCONNECT_CFM_TSizeof(void *msg)
{
    RFC_DISCONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_RFC_DISCONNECT_CFM
CsrUint8 *RFC_DISCONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DISCONNECT_CFM_T *primitive;

    primitive = (RFC_DISCONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_RFC_DISCONNECT_CFM
void *RFC_DISCONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DISCONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DISCONNECT_CFM_T *) CsrPmemZalloc(sizeof(RFC_DISCONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_RSP
CsrSize RFC_PORTNEG_RSP_TSizeof(void *msg)
{
    RFC_PORTNEG_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_PORTNEG_RSP */

#ifndef EXCLUDE_RFC_PORTNEG_RSP
CsrUint8 *RFC_PORTNEG_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_PORTNEG_RSP_T *primitive;

    primitive = (RFC_PORTNEG_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port_pars.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_RFC_PORTNEG_RSP */

#ifndef EXCLUDE_RFC_PORTNEG_RSP
void *RFC_PORTNEG_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_PORTNEG_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_PORTNEG_RSP_T *) CsrPmemZalloc(sizeof(RFC_PORTNEG_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port_pars.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_PORTNEG_RSP */

#ifndef EXCLUDE_RFC_UNREGISTER_REQ
CsrSize RFC_UNREGISTER_REQ_TSizeof(void *msg)
{
    RFC_UNREGISTER_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_UNREGISTER_REQ */

#ifndef EXCLUDE_RFC_UNREGISTER_REQ
CsrUint8 *RFC_UNREGISTER_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_UNREGISTER_REQ_T *primitive;

    primitive = (RFC_UNREGISTER_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->loc_serv_chan);

    return buffer;
}
#endif /* EXCLUDE_RFC_UNREGISTER_REQ */

#ifndef EXCLUDE_RFC_UNREGISTER_REQ
void *RFC_UNREGISTER_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_UNREGISTER_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_UNREGISTER_REQ_T *) CsrPmemZalloc(sizeof(RFC_UNREGISTER_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->loc_serv_chan, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_UNREGISTER_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_REQ
CsrSize RFC_DISCONNECT_REQ_TSizeof(void *msg)
{
    RFC_DISCONNECT_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_REQ
CsrUint8 *RFC_DISCONNECT_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DISCONNECT_REQ_T *primitive;

    primitive = (RFC_DISCONNECT_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);

    return buffer;
}
#endif /* EXCLUDE_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_REQ
void *RFC_DISCONNECT_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DISCONNECT_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DISCONNECT_REQ_T *) CsrPmemZalloc(sizeof(RFC_DISCONNECT_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_IND
CsrSize RFC_DISCONNECT_IND_TSizeof(void *msg)
{
    RFC_DISCONNECT_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DISCONNECT_IND */

#ifndef EXCLUDE_RFC_DISCONNECT_IND
CsrUint8 *RFC_DISCONNECT_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DISCONNECT_IND_T *primitive;

    primitive = (RFC_DISCONNECT_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reason);

    return buffer;
}
#endif /* EXCLUDE_RFC_DISCONNECT_IND */

#ifndef EXCLUDE_RFC_DISCONNECT_IND
void *RFC_DISCONNECT_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DISCONNECT_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DISCONNECT_IND_T *) CsrPmemZalloc(sizeof(RFC_DISCONNECT_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DISCONNECT_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_CFM
CsrSize RFC_MODEM_STATUS_CFM_TSizeof(void *msg)
{
    RFC_MODEM_STATUS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_CFM */

#ifndef EXCLUDE_RFC_MODEM_STATUS_CFM
CsrUint8 *RFC_MODEM_STATUS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_MODEM_STATUS_CFM_T *primitive;

    primitive = (RFC_MODEM_STATUS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_CFM */

#ifndef EXCLUDE_RFC_MODEM_STATUS_CFM
void *RFC_MODEM_STATUS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_MODEM_STATUS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_MODEM_STATUS_CFM_T *) CsrPmemZalloc(sizeof(RFC_MODEM_STATUS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_CFM
CsrSize RFC_PORTNEG_CFM_TSizeof(void *msg)
{
    RFC_PORTNEG_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_PORTNEG_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_CFM
CsrUint8 *RFC_PORTNEG_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_PORTNEG_CFM_T *primitive;

    primitive = (RFC_PORTNEG_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->port_pars.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->port_pars.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_PORTNEG_CFM */

#ifndef EXCLUDE_RFC_PORTNEG_CFM
void *RFC_PORTNEG_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_PORTNEG_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_PORTNEG_CFM_T *) CsrPmemZalloc(sizeof(RFC_PORTNEG_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->port_pars.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->port_pars.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_PORTNEG_CFM */

#ifndef EXCLUDE_RFC_DATAWRITE_REQ
CsrSize RFC_DATAWRITE_REQ_TSizeof(void *msg)
{
    RFC_DATAWRITE_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->payload) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DATAWRITE_REQ */

#ifndef EXCLUDE_RFC_DATAWRITE_REQ
CsrUint8 *RFC_DATAWRITE_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DATAWRITE_REQ_T *primitive;

    primitive = (RFC_DATAWRITE_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payload_length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rx_credits);
    if (primitive->payload)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->payload);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->payload, 0, mblkLength, &buffer[*length]);
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
#endif /* EXCLUDE_RFC_DATAWRITE_REQ */

#ifndef EXCLUDE_RFC_DATAWRITE_REQ
void *RFC_DATAWRITE_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DATAWRITE_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DATAWRITE_REQ_T *) CsrPmemZalloc(sizeof(RFC_DATAWRITE_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payload_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->rx_credits, buffer, &offset);
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->payload = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_DATAWRITE_REQ */

#ifndef EXCLUDE_RFC_DATAWRITE_REQ
void RFC_DATAWRITE_REQ_TSerFree(void *msg)
{
    RFC_DATAWRITE_REQ_T *primitive;

    primitive = (RFC_DATAWRITE_REQ_T *) msg;

    if(primitive->payload != NULL)
    {
        CsrMblkDestroy(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_RFC_DATAWRITE_REQ */

#ifndef EXCLUDE_RFC_LINESTATUS_CFM
CsrSize RFC_LINESTATUS_CFM_TSizeof(void *msg)
{
    RFC_LINESTATUS_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_LINESTATUS_CFM */

#ifndef EXCLUDE_RFC_LINESTATUS_CFM
CsrUint8 *RFC_LINESTATUS_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_LINESTATUS_CFM_T *primitive;

    primitive = (RFC_LINESTATUS_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_LINESTATUS_CFM */

#ifndef EXCLUDE_RFC_LINESTATUS_CFM
void *RFC_LINESTATUS_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_LINESTATUS_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_LINESTATUS_CFM_T *) CsrPmemZalloc(sizeof(RFC_LINESTATUS_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_LINESTATUS_CFM */

#ifndef EXCLUDE_RFC_FC_REQ
CsrSize RFC_FC_REQ_TSizeof(void *msg)
{
    RFC_FC_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_FC_REQ */

#ifndef EXCLUDE_RFC_FC_REQ
CsrUint8 *RFC_FC_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_FC_REQ_T *primitive;

    primitive = (RFC_FC_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fc);

    return buffer;
}
#endif /* EXCLUDE_RFC_FC_REQ */

#ifndef EXCLUDE_RFC_FC_REQ
void *RFC_FC_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_FC_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_FC_REQ_T *) CsrPmemZalloc(sizeof(RFC_FC_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    {
        CsrUint16 data;
        CsrUint16Des((CsrUint16 *) &data, buffer, &offset);
        primitive->fc = (RFC_FC_T) data;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_FC_REQ */

#ifndef EXCLUDE_RFC_DISCONNECT_RSP
CsrSize RFC_DISCONNECT_RSP_TSizeof(void *msg)
{
    RFC_DISCONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DISCONNECT_RSP */

#ifndef EXCLUDE_RFC_DISCONNECT_RSP
CsrUint8 *RFC_DISCONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DISCONNECT_RSP_T *primitive;

    primitive = (RFC_DISCONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);

    return buffer;
}
#endif /* EXCLUDE_RFC_DISCONNECT_RSP */

#ifndef EXCLUDE_RFC_DISCONNECT_RSP
void *RFC_DISCONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DISCONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DISCONNECT_RSP_T *) CsrPmemZalloc(sizeof(RFC_DISCONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_DISCONNECT_RSP */

#ifndef EXCLUDE_RFC_LINESTATUS_IND
CsrSize RFC_LINESTATUS_IND_TSizeof(void *msg)
{
    RFC_LINESTATUS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_LINESTATUS_IND */

#ifndef EXCLUDE_RFC_LINESTATUS_IND
CsrUint8 *RFC_LINESTATUS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_LINESTATUS_IND_T *primitive;

    primitive = (RFC_LINESTATUS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->error_flag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->line_status);

    return buffer;
}
#endif /* EXCLUDE_RFC_LINESTATUS_IND */

#ifndef EXCLUDE_RFC_LINESTATUS_IND
void *RFC_LINESTATUS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_LINESTATUS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_LINESTATUS_IND_T *) CsrPmemZalloc(sizeof(RFC_LINESTATUS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->error_flag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->line_status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_LINESTATUS_IND */

#ifndef EXCLUDE_RFC_DATAREAD_IND
CsrSize RFC_DATAREAD_IND_TSizeof(void *msg)
{
    RFC_DATAREAD_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->payload) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_DATAREAD_IND */

#ifndef EXCLUDE_RFC_DATAREAD_IND
CsrUint8 *RFC_DATAREAD_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_DATAREAD_IND_T *primitive;

    primitive = (RFC_DATAREAD_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payload_length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if (primitive->payload)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->payload);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->payload, 0, mblkLength, &buffer[*length]);
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
#endif /* EXCLUDE_RFC_DATAREAD_IND */

#ifndef EXCLUDE_RFC_DATAREAD_IND
void *RFC_DATAREAD_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_DATAREAD_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_DATAREAD_IND_T *) CsrPmemZalloc(sizeof(RFC_DATAREAD_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payload_length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->payload = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_RFC_DATAREAD_IND */

#ifndef EXCLUDE_RFC_DATAREAD_IND
void RFC_DATAREAD_IND_TSerFree(void *msg)
{
    RFC_DATAREAD_IND_T *primitive;

    primitive = (RFC_DATAREAD_IND_T *) msg;

    if(primitive->payload != NULL)
    {
        CsrMblkDestroy(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_RFC_DATAREAD_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_IND
CsrSize RFC_MODEM_STATUS_IND_TSizeof(void *msg)
{
    RFC_MODEM_STATUS_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_IND
CsrUint8 *RFC_MODEM_STATUS_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_MODEM_STATUS_IND_T *primitive;

    primitive = (RFC_MODEM_STATUS_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modem_signal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_IND
void *RFC_MODEM_STATUS_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_MODEM_STATUS_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_MODEM_STATUS_IND_T *) CsrPmemZalloc(sizeof(RFC_MODEM_STATUS_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modem_signal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_IND */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_CFM
CsrSize RFC_CLIENT_CONNECT_CFM_TSizeof(void *msg)
{
    RFC_CLIENT_CONNECT_CFM_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_CFM */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_CFM
CsrUint8 *RFC_CLIENT_CONNECT_CFM_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_CLIENT_CONNECT_CFM_T *primitive;

    primitive = (RFC_CLIENT_CONNECT_CFM_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serv_chan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_payload_size);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_l2cap_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_l2cap_control);

    return buffer;
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_CFM */

#ifndef EXCLUDE_RFC_CLIENT_CONNECT_CFM
void *RFC_CLIENT_CONNECT_CFM_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_CLIENT_CONNECT_CFM_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_CLIENT_CONNECT_CFM_T *) CsrPmemZalloc(sizeof(RFC_CLIENT_CONNECT_CFM_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serv_chan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_payload_size, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_l2cap_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_l2cap_control, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_CLIENT_CONNECT_CFM */

#ifndef EXCLUDE_RFC_NSC_IND
CsrSize RFC_NSC_IND_TSizeof(void *msg)
{
    RFC_NSC_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_NSC_IND */

#ifndef EXCLUDE_RFC_NSC_IND
CsrUint8 *RFC_NSC_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_NSC_IND_T *primitive;

    primitive = (RFC_NSC_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->command_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cmd_res);

    return buffer;
}
#endif /* EXCLUDE_RFC_NSC_IND */

#ifndef EXCLUDE_RFC_NSC_IND
void *RFC_NSC_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_NSC_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_NSC_IND_T *) CsrPmemZalloc(sizeof(RFC_NSC_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->command_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cmd_res, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_NSC_IND */

#ifndef EXCLUDE_RFC_MODEM_STATUS_REQ
CsrSize RFC_MODEM_STATUS_REQ_TSizeof(void *msg)
{
    RFC_MODEM_STATUS_REQ_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_REQ */

#ifndef EXCLUDE_RFC_MODEM_STATUS_REQ
CsrUint8 *RFC_MODEM_STATUS_REQ_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_MODEM_STATUS_REQ_T *primitive;

    primitive = (RFC_MODEM_STATUS_REQ_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modem_signal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_REQ */

#ifndef EXCLUDE_RFC_MODEM_STATUS_REQ
void *RFC_MODEM_STATUS_REQ_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_MODEM_STATUS_REQ_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_MODEM_STATUS_REQ_T *) CsrPmemZalloc(sizeof(RFC_MODEM_STATUS_REQ_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modem_signal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_MODEM_STATUS_REQ */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_RSP
CsrSize RFC_SERVER_CONNECT_RSP_TSizeof(void *msg)
{
    RFC_SERVER_CONNECT_RSP_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_RSP */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_RSP
CsrUint8 *RFC_SERVER_CONNECT_RSP_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_SERVER_CONNECT_RSP_T *primitive;

    primitive = (RFC_SERVER_CONNECT_RSP_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conn_id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->response);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->max_payload_size);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->priority);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->total_credits);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remote_l2cap_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_l2cap_control);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modem_signal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->msc_timeout);

    return buffer;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_RSP */

#ifndef EXCLUDE_RFC_SERVER_CONNECT_RSP
void *RFC_SERVER_CONNECT_RSP_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_SERVER_CONNECT_RSP_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_SERVER_CONNECT_RSP_T *) CsrPmemZalloc(sizeof(RFC_SERVER_CONNECT_RSP_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conn_id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->response, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->max_payload_size, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->priority, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->total_credits, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remote_l2cap_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_l2cap_control, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modem_signal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->msc_timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_SERVER_CONNECT_RSP */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND
CsrSize RFC_L2CA_MOVE_CHANNEL_CMP_IND_TSizeof(void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND
CsrUint8 *RFC_L2CA_MOVE_CHANNEL_CMP_IND_TSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *primitive;

    primitive = (RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->local_control);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND
void *RFC_L2CA_MOVE_CHANNEL_CMP_IND_TDes(CsrUint8 *buffer, CsrSize length)
{
    RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (RFC_L2CA_MOVE_CHANNEL_CMP_IND_T *) CsrPmemZalloc(sizeof(RFC_L2CA_MOVE_CHANNEL_CMP_IND_T));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->local_control, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_RFC_L2CA_MOVE_CHANNEL_CMP_IND */

#endif /* EXCLUDE_RFCOMM_MODULE */
