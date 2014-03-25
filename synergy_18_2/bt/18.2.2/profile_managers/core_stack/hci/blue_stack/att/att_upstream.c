/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_upstream.c

\brief  Attribute Protocol upstream interface
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

void att_error_cfm(CsrSchedQid phandle, att_conn_t *conn, CsrUint8 op,
                   CsrUint16 handle, att_result_t rc)
{
    switch (op)
    {
        case ATT_OP_EXCHANGE_MTU_REQ:
        case ATT_OP_EXCHANGE_MTU_RSP:
            att_exchange_mtu_cfm(phandle, conn->cid, conn->mtu, rc);
            break;

        case ATT_OP_FIND_INFO_REQ:
        case ATT_OP_FIND_INFO_RSP:
            att_find_info_cfm(phandle, conn->cid, handle,
                              ATT_UUID_NONE, NULL, rc);
            break;

        case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
        case ATT_OP_FIND_BY_TYPE_VALUE_RSP:
            att_find_by_type_value_cfm(phandle, conn->cid, handle, 0, rc);
            break;

        case ATT_OP_READ_BY_TYPE_REQ:
        case ATT_OP_READ_BY_TYPE_RSP:
            att_read_by_type_cfm(phandle, conn->cid, handle, rc, 0, NULL);
            break;

        case ATT_OP_READ_REQ:
        case ATT_OP_READ_RSP:
            att_read_cfm(phandle, conn->cid, rc, 0, NULL);
            break;

        case ATT_OP_READ_BLOB_REQ:
        case ATT_OP_READ_BLOB_RSP:
            att_read_blob_cfm(phandle, conn->cid, rc, 0, NULL);
            break;

        case ATT_OP_READ_MULTI_REQ:
        case ATT_OP_READ_MULTI_RSP:
            att_read_multi_cfm(phandle, conn->cid, rc, 0, NULL);
            break;

        case ATT_OP_READ_BY_GROUP_TYPE_REQ:
        case ATT_OP_READ_BY_GROUP_TYPE_RSP:
            att_read_by_group_type_cfm(phandle, conn->cid, rc, handle, 0,
                                       0, NULL);
            break;

        case ATT_OP_WRITE_REQ:
        case ATT_OP_WRITE_RSP:
            att_write_cfm(phandle, conn->cid, rc);
            break;

        case ATT_OP_PREPARE_WRITE_REQ:
        case ATT_OP_PREPARE_WRITE_RSP:
            att_prepare_write_cfm(phandle, conn->cid, handle, 0 /* offs */,
                                  rc, 0, NULL);
            break;

        case ATT_OP_EXECUTE_WRITE_REQ:
        case ATT_OP_EXECUTE_WRITE_RSP:
            att_execute_write_cfm(phandle, conn->cid, handle, rc);
            break;

        case ATT_OP_HANDLE_VALUE_IND:
            att_handle_value_cfm(phandle, conn->cid, rc);
            break;

        default:
            /* silently ignore */
            break;
    }
}

void att_register_cfm(CsrSchedQid phandle, att_result_t rc)
{
    MAKE_PRIM(ATT_REGISTER_CFM);

    prim->phandle = phandle;
    prim->result = rc;

    SEND_PRIM();
}

void att_unregister_cfm(CsrSchedQid phandle, att_result_t rc)
{
    MAKE_PRIM(ATT_UNREGISTER_CFM);

    prim->phandle = phandle;
    prim->result = rc;

    SEND_PRIM();
}

#ifndef BUILD_FOR_HOST
void att_add_db_cfm(CsrSchedQid phandle, att_result_t rc)
{
    MAKE_PRIM(ATT_ADD_DB_CFM);

    prim->phandle = phandle;
    prim->result = rc;

    SEND_PRIM();
}
#endif /* !BUILD_FOR_HOST */

#ifdef BUILD_FOR_HOST
void att_add_cfm(CsrSchedQid phandle, att_result_t rc)
{
    MAKE_PRIM(ATT_ADD_CFM);

    prim->phandle = phandle;
    prim->result = rc;

    SEND_PRIM();
}
#endif /* BUILD_FOR_HOST */

#ifdef BUILD_FOR_HOST
void att_remove_cfm(CsrSchedQid phandle, CsrUint16 num_attr, att_result_t rc)
{
    MAKE_PRIM(ATT_REMOVE_CFM);

    prim->phandle = phandle;
    prim->num_attr = num_attr;
    prim->result = rc;

    SEND_PRIM();
}
#endif /* BUILD_FOR_HOST */

void att_connect_cfm(CsrSchedQid phandle, TYPED_BD_ADDR_T *addrt, l2ca_conflags_t flags,
                     CsrUint16 mtu, l2ca_cid_t cid, l2ca_conn_result_t result)
{
    MAKE_PRIM(ATT_CONNECT_CFM);

    prim->phandle = phandle;
    prim->addrt = *addrt;
    prim->flags = flags;
    prim->mtu = mtu;
    prim->cid = cid;
    prim->result = result;

    SEND_PRIM();
}

void att_connect_ind(CsrSchedQid phandle, TYPED_BD_ADDR_T *addrt, l2ca_conflags_t flags,
                     CsrUint16 mtu, l2ca_cid_t cid)
{
    MAKE_PRIM(ATT_CONNECT_IND);

    prim->phandle = phandle;
    prim->addrt = *addrt;
    prim->flags = flags;
    prim->mtu = mtu;
    prim->cid = cid;

    SEND_PRIM();
}

#ifdef BUILD_FOR_ROBINSON
void att_disconnect_cfm(CsrSchedQid phandle, CsrUint16 cid, l2ca_disc_result_t reason)
{
    MAKE_PRIM(ATT_DISCONNECT_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->reason = reason;

    SEND_PRIM();
}
#endif /* BUILD_FOR_ROBINSON */

void att_disconnect_ind(CsrSchedQid phandle, CsrUint16 cid, l2ca_disc_result_t reason)
{
    MAKE_PRIM(ATT_DISCONNECT_IND);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->reason = reason;

    SEND_PRIM();
}


void att_exchange_mtu_ind(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 mtu)
{
    MAKE_PRIM(ATT_EXCHANGE_MTU_IND);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->client_mtu = mtu;

    SEND_PRIM();
}

void att_exchange_mtu_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 mtu, att_result_t rc)
{
    MAKE_PRIM(ATT_EXCHANGE_MTU_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->mtu = mtu;
    prim->result = rc;

    SEND_PRIM();
}

void att_find_info_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle, att_uuid_type_t uuid_type, CsrUint32 *uuid, att_result_t rc)
{
    MAKE_PRIM(ATT_FIND_INFO_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;

    switch (uuid_type)
    {
        case ATT_UUID16:
            /* convert UUID16 into UUID128 format */
            /* NOTE: it is still enough to just use uuid[0] to get UUID16 */
            prim->uuid[0] = uuid[0] & 0xffff; /* UUID16 */
            prim->uuid[1] = att_base_uuid[1];
            prim->uuid[2] = att_base_uuid[2];
            prim->uuid[3] = att_base_uuid[3];
            break;

        case ATT_UUID128:
            /* copy UUID128 */
            prim->uuid[0] = uuid[0];
            prim->uuid[1] = uuid[1];
            prim->uuid[2] = uuid[2];
            prim->uuid[3] = uuid[3];

            if (ATT_IS_UUID16(uuid))
                uuid_type = ATT_UUID16;
            break;

        default: /* ATT_UUID_NONE, for errors */
            /* prim->uuid is already zeroed, do nothing */
            break;
    }

    prim->uuid_type = uuid_type;
    prim->result = rc;

    SEND_PRIM();
}

void att_find_by_type_value_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle, CsrUint16 end, att_result_t rc)
{
    MAKE_PRIM(ATT_FIND_BY_TYPE_VALUE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->end = end;
    prim->result = rc;

    SEND_PRIM();
}

void att_read_by_type_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle, att_result_t rc, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_READ_BY_TYPE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->result = rc;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_read_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_READ_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_read_blob_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_READ_BLOB_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_read_multi_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_READ_MULTI_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_read_by_group_type_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc, CsrUint16 handle, CsrUint16 end, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_READ_BY_GROUP_TYPE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;
    prim->handle = handle;
    prim->end = end;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}
void att_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc)
{
    MAKE_PRIM(ATT_WRITE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;

    SEND_PRIM();
}

void att_prepare_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle, CsrUint16 offset, att_result_t rc, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_PREPARE_WRITE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->offset = offset;
    prim->result = rc;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_execute_write_cfm(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle,
                           att_result_t rc)
{
    MAKE_PRIM(ATT_EXECUTE_WRITE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->result = rc;

    SEND_PRIM();
}

void att_handle_value_ind(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle, CsrUint16 flags, CsrUint16 size_value, CsrUint8 *value)
{
    MAKE_PRIM(ATT_HANDLE_VALUE_IND);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->flags = flags;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

void att_handle_value_cfm(CsrSchedQid phandle, l2ca_cid_t cid, att_result_t rc)
{
    MAKE_PRIM(ATT_HANDLE_VALUE_CFM);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->result = rc;

    SEND_PRIM();
}

void att_access_ind(CsrSchedQid phandle, l2ca_cid_t cid, CsrUint16 handle,
                    CsrUint16 flags, CsrUint16 offs, CsrUint16 size_value,
                    CsrUint8 *value)
{
    MAKE_PRIM(ATT_ACCESS_IND);

    prim->phandle = phandle;
    prim->cid = cid;
    prim->handle = handle;
    prim->flags = flags;
    prim->offset = offs;
    prim->size_value = size_value;
    prim->value = value;

    SEND_PRIM();
}

#ifndef BUILD_FOR_ROBINSON
void att_debug_ind(ATT_FUNC_STATE
                   const char *msg, CsrUint16 len)
{
    MAKE_PRIM(ATT_DEBUG_IND);

    if (!len)
        prim->size_debug = (CsrUint16)CsrStrLen(msg);
    else
        prim->size_debug = len;

    if ((prim->debug = xpcopy(msg, prim->size_debug)) == NULL)
        prim->size_debug = 0;

    /* SEND_PRIM(); */
    att_send_message(ATT_STATE(phandle), ATT_PRIM, prim);
}
#endif /* !BUILD_FOR_ROBINSON */

#endif /* INSTALL_ATT_MODULE */
