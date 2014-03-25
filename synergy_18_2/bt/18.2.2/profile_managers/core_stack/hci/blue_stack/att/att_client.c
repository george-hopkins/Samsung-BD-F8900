/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_client.c

\brief  Attribute Protocol Client Interface and over-the-air handlers
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

/*
 * Requests to the server
 ****************************************************************************/
void att_exchange_mtu_req(ATT_FUNC_STATE
                          ATT_EXCHANGE_MTU_REQ_T *req)
{
    att_result_t rc;
    att_conn_t *conn;

    /* check the request */
    if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
        rc = ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP())
        rc = ATT_RESULT_BUSY;

#ifdef INSTALL_ATT_BREDR
    /* Exchange MTU is not allowed on BR/EDR connections */
    else if (!L2CA_CONFLAG_IS_LE(conn->flags))
        rc = ATT_RESULT_REQUEST_NOT_SUPPORTED;
#endif /* INSTALL_ATT_BREDR */

    else
    {
        if (req->mtu < ATT_MTU_MIN)
            rc = ATT_RESULT_INVALID_MTU;

        else
        {
            /* set local MTU already, flow control will prevent us
             * sending anything too large */
            conn->mtu = CSRMIN(req->mtu, ATT_MTU_MAX);

            att_pkt_create(conn, ATT_OP_EXCHANGE_MTU_REQ);
            write_uint16(&conn->out.data, conn->mtu);
            att_pkt_send(conn);

            return;
        }
    }

    /* send error to the application */
    att_exchange_mtu_cfm(req->phandle, req->cid, req->mtu, rc);
}

/* common read information request + read by type + read by group type
   request */
static att_result_t read_common_uuid_req(ATT_FUNC_STATE
                                         CsrUint8 op, l2ca_cid_t cid,
                                         CsrUint16 start, CsrUint16 end,
                                         att_uuid_type_t uuid_type,
                                         CsrUint32 *uuid)
{
    att_conn_t *conn;
    CsrUint16 i;

    /* check the request for valid cid and corresponding conn */
    if ( (conn = att_conn_find(ATT_ARG cid)) == NULL)
        return ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP())
        return ATT_RESULT_BUSY;

    else if (!start || start > end)
        return ATT_RESULT_INVALID_HANDLE;

    switch (uuid_type)
    {
        case ATT_UUID_NONE:
        case ATT_UUID16:
            break;

        case ATT_UUID128:
            if (ATT_IS_UUID16(uuid))
            {
                uuid_type = ATT_UUID16;
            }
            break;

        default:
            return ATT_RESULT_INVALID_UUID;
    }

    att_pkt_create(conn, op);
    write_uint16(&conn->out.data, start);
    write_uint16(&conn->out.data, end);

    /* add type/filter */
    switch (uuid_type)
    {
        case ATT_UUID16:
            write_uint16(&conn->out.data, (CsrUint16)(uuid[0] & 0xffff));
            break;

        case ATT_UUID128:
            for (i = 0; i < 4; i++)
            {
                write_uint16(&conn->out.data,
                             (CsrUint16)(uuid[3 - i] & 0xffff));
                write_uint16(&conn->out.data, (CsrUint16)(uuid[3 - i] >> 16));
            }
            break;

        default:
            break;
    }

    att_pkt_send(conn);

    return ATT_RESULT_SUCCESS;
}

/* wrapper for read information request */
void att_find_info_req(ATT_FUNC_STATE
                       ATT_FIND_INFO_REQ_T *req)
{
    att_result_t rc;

    rc = read_common_uuid_req(ATT_ARG ATT_OP_FIND_INFO_REQ,
                              req->cid,
                              req->start,
                              req->end,
                              ATT_UUID_NONE, NULL);

    if (rc != ATT_RESULT_SUCCESS)
        att_find_info_cfm(req->phandle, req->cid, req->start,
                          ATT_UUID_NONE, NULL, rc);
}

/* find by type value request */
void att_find_by_type_value_req(ATT_FUNC_STATE
                                ATT_FIND_BY_TYPE_VALUE_REQ_T *req)
{
    att_conn_t *conn;
    att_result_t rc;

    /* check the request */
    if (  (conn = att_conn_find(ATT_ARG req->cid)) == NULL)
        rc = ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP())
        rc = ATT_RESULT_BUSY;

    else if (!req->start || req->start > req->end)
        rc = ATT_RESULT_INVALID_HANDLE;

    else
    {
        att_pkt_create(conn, ATT_OP_FIND_BY_TYPE_VALUE_REQ);
        write_uint16(&conn->out.data, req->start);
        write_uint16(&conn->out.data, req->end);
        write_uint16(&conn->out.data, req->uuid);
        att_pkt_write(conn, req->value, req->size_value);

        att_pkt_send(conn);

        return;
    }

    att_find_by_type_value_cfm(req->phandle, req->cid, req->start, 0, rc);
}

/* wrapper for read by type request */
void att_read_by_type_req(ATT_FUNC_STATE
                          ATT_READ_BY_TYPE_REQ_T *req)
{
    att_result_t rc;

    rc = read_common_uuid_req(ATT_ARG ATT_OP_READ_BY_TYPE_REQ,
                              req->cid,
                              req->start,
                              req->end,
                              req->uuid_type,
                              req->uuid);

    if (rc != ATT_RESULT_SUCCESS)
        att_read_by_type_cfm(req->phandle, req->cid, req->start, rc, 0, NULL);
}

/* common read + read blob + read multiple request */
static att_result_t read_common_req(ATT_FUNC_STATE
                                    CsrUint8 op, l2ca_cid_t cid,
                                    CsrUint16 num_handle, CsrUint16 *handle,
                                    CsrUint16 offs)
{
    att_conn_t *conn;
    CsrUint16 u = 0;

    /* check the request */
    if ((conn = att_conn_find(ATT_ARG cid)) == NULL)
        return ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP())
        return ATT_RESULT_BUSY;

    else if (!*handle)
        return ATT_RESULT_INVALID_HANDLE;

    else if (offs > ATT_LEN_MAX)
        return ATT_RESULT_INVALID_OFFSET;

    /* all ok, send the request */
    else
    {
        num_handle = CSRMIN(num_handle, att_pkt_create(conn, op) / 2);

        for (;num_handle;num_handle--)
        {
            u = *(handle++);
            write_uint16(&conn->out.data, u);
        }

        if (op == ATT_OP_READ_BLOB_REQ)
            write_uint16(&conn->out.data, offs);

        att_pkt_send(conn);
    }

    return ATT_RESULT_SUCCESS;
}

/* wrapper for read request */
void att_read_req(ATT_FUNC_STATE
                  ATT_READ_REQ_T *req)
{
    att_result_t rc;

    rc = read_common_req(ATT_ARG ATT_OP_READ_REQ,
                         req->cid,
                         1, &req->handle,
                         0 /* offset */ );

    if (rc != ATT_RESULT_SUCCESS)
        att_read_cfm(req->phandle, req->cid, rc, 0, NULL);
}

/* wrapper for read blob */
void att_read_blob_req(ATT_FUNC_STATE
                       ATT_READ_BLOB_REQ_T *req)
{
    att_result_t rc;

    rc = read_common_req(ATT_ARG ATT_OP_READ_BLOB_REQ,
                         req->cid,
                         1, &req->handle,
                         req->offset);

    if (rc != ATT_RESULT_SUCCESS)
        att_read_blob_cfm(req->phandle, req->cid, rc, 0, NULL);
}

/* wrapper for read multiple */
void att_read_multi_req(ATT_FUNC_STATE
                        ATT_READ_MULTI_REQ_T *req)
{
    att_result_t rc;

    rc = read_common_req(ATT_ARG ATT_OP_READ_MULTI_REQ,
                         req->cid,
                         req->size_handles, req->handles,
                         0 /* offset */);

    if (rc != ATT_RESULT_SUCCESS)
        att_read_multi_cfm(req->phandle, req->cid, rc, 0, NULL);
}

/* wrapper for read by type request */
void att_read_by_group_type_req(ATT_FUNC_STATE
                                ATT_READ_BY_GROUP_TYPE_REQ_T *req)
{
    att_result_t rc;

    switch (req->group_type)
    {
        case ATT_UUID16:
        case ATT_UUID128:
            rc = read_common_uuid_req(ATT_ARG ATT_OP_READ_BY_GROUP_TYPE_REQ,
                                      req->cid,
                                      req->start,
                                      req->end,
                                      req->group_type,
                                      req->group);
            break;

        default:
            rc = ATT_RESULT_INVALID_UUID;
    }

    if (rc != ATT_RESULT_SUCCESS)
        att_read_by_group_type_cfm(req->phandle, req->cid, rc, req->start,
                                   0, 0, NULL);
}

/* common write + write blob + prepare write */
static att_result_t write_common_req(ATT_FUNC_STATE
                                     CsrUint8 op, l2ca_cid_t cid,
                                     CsrUint16 handle, CsrUint16 offs,
                                     CsrUint16 size_value, CsrUint8 *value)
{
    att_conn_t *conn;
    CsrUint16 bytesleft;

    /* check the request */
    if ((conn = att_conn_find(ATT_ARG cid)) == NULL)
        return ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP() && !(op & ATT_OP_CMD_FLAG))
        return ATT_RESULT_BUSY;

    else if (!handle)
        return ATT_RESULT_INVALID_HANDLE;

    else if (offs > ATT_LEN_MAX) /* offs == 0 if op != blob */
        return ATT_RESULT_INVALID_OFFSET;

    /* all ok, send the request */
    else
    {
        att_pkt_create(conn, op);
        write_uint16(&conn->out.data, handle);

        if (op == ATT_OP_PREPARE_WRITE_REQ)
            write_uint16(&conn->out.data, offs);

        bytesleft = att_pkt_write(conn, value, size_value);

        /* add signature */
        if (op & ATT_OP_AUTH_FLAG)
        {
            if (bytesleft < ATT_OP_AUTH_LEN)
            {
                /* no room for signature */
#ifndef BUILD_FOR_ROBINSON
                CsrPmemFree(conn->out.buf);
#endif /* !BUILD_FOR_ROBINSON */
                conn->out.buf = conn->out.data = NULL;

                return ATT_RESULT_INVALID_MTU;
            }
            else
            {
                att_dm_sm_data_sign_req(conn, FALSE,
                                        (CsrUint16)(conn->out.data - conn->out.buf),
                                        conn->out.buf);

                /* don't free conn->out.buf while SM is using it */
                conn->pend |= PEND_AUTH_SIGN;

                return ATT_RESULT_INSUFFICIENT_AUTHENTICATION;
            }
        }

        att_pkt_send(conn);
    }

    return ATT_RESULT_SUCCESS;
}

/* wrapper for write command/write request */
void att_write_req(ATT_FUNC_STATE
                   ATT_WRITE_REQ_T *req)
{
    att_result_t rc;
    CsrUint8 flags = 0;
    att_conn_t *conn = NULL;
    TYPED_BD_ADDR_T addrt;

    rc = ATT_RESULT_SUCCESS;

    /* Signed writes should be done on un-encrypted
    * link. This should be the first point we check. If
    * it is performed on an encrypted link we will reject
    * the request.
    */
    if(req->flags & ATT_WRITE_SIGNED
            && req->cid != ATT_CID_LOCAL)
    {
        DM_ACL_T *p_acl = NULL;

        /* check the request */
        if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
            rc = ATT_RESULT_INVALID_CID;
        else
        {

#ifndef BUILD_FOR_HOST
            L2CA_GetBdAddrFromCid(conn->cid, &addrt);
#else /* !BUILD_FOR_HOST */
            tbdaddr_copy(&addrt, &conn->addrt);
#endif /* BUILD_FOR_HOST */

            if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) == NULL)
                rc = ATT_RESULT_DEVICE_NOT_FOUND;
            else
            {
                if(DM_SM_IS_LINK_ENCRYPTED(p_acl))
                {
                    rc = ATT_RESULT_SIGNED_DISALLOWED;
                }
            }
        }
    }

    /* NOTE: Magically ATT_WRITE_COMMND flags is equal to ATT_OP_CMD_FLAG,
       and ATT_WRITE_SIGNED is equal to ATT_OP_AUTH_FLAG. */

    /* Write command, may include auth flag as well */
    if (req->flags & ATT_WRITE_COMMAND)
    {
        /* Allow auth flag only in write command */
        flags = req->flags & (ATT_OP_CMD_FLAG | ATT_OP_AUTH_FLAG);
    }

    /* signed write, can only be a command so let's force it */
    else if (req->flags & ATT_WRITE_SIGNED)
    {
        flags = ATT_OP_CMD_FLAG | ATT_OP_AUTH_FLAG;
    }

    if(rc == ATT_RESULT_SUCCESS)
    {

        rc = write_common_req(
            ATT_ARG
            (CsrUint8)(ATT_OP_WRITE_REQ | flags),
            req->cid,
            req->handle,
            0 /* offset */,
            req->size_value, req->value);
    }

    /* data signing to be done, don't send the CFM yet */
    if (rc == ATT_RESULT_INSUFFICIENT_AUTHENTICATION)
        return;

    /* in case of write command, success indicates that the command was sent,
       otherwise send only errors */
    else if ((flags & ATT_OP_CMD_FLAG) || rc != ATT_RESULT_SUCCESS)
        att_write_cfm(req->phandle, req->cid, rc);
}

/* wrapper for write blob request */
void att_prepare_write_req(ATT_FUNC_STATE
                           ATT_PREPARE_WRITE_REQ_T *req)
{
    att_result_t rc;

    /* Local operations do not support write queue */
    if (req->cid == ATT_CID_LOCAL)
    {
        rc = ATT_RESULT_REQUEST_NOT_SUPPORTED;
    }
    else
    {
        rc = write_common_req(
            ATT_ARG
            ATT_OP_PREPARE_WRITE_REQ,
            req->cid,
            req->handle,
            req->offset,
            req->size_value, req->value);
    }

    /* send error, but ignore value as it was not sent at all */
    if (rc != ATT_RESULT_SUCCESS)
        att_prepare_write_cfm(req->phandle, req->cid, req->handle,
                              req->offset, rc, 0, NULL);
}

void att_execute_write_req(ATT_FUNC_STATE
                           ATT_EXECUTE_WRITE_REQ_T *req)
{
    att_result_t rc;
    att_conn_t *conn;

    /* Local operations do not support write queue */
    if (req->cid == ATT_CID_LOCAL)
        rc = ATT_RESULT_REQUEST_NOT_SUPPORTED;

    else if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
        rc = ATT_RESULT_INVALID_CID;

    else if (PEND_GET_RSP())
        rc = ATT_RESULT_BUSY;

    else
    {
        att_pkt_create(conn, ATT_OP_EXECUTE_WRITE_REQ);
        write_uint8(&conn->out.data, (CsrUint8)req->flags);
        att_pkt_send(conn);
        return;
    }

    att_execute_write_cfm(req->phandle, req->cid, 0 /* handle */, rc);
}

void att_handle_value_rsp(ATT_FUNC_STATE
                          ATT_HANDLE_VALUE_RSP_T *req)
{
    att_conn_t *conn;

    if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
    {
        return;
    }

    else if (!(conn->pend & PEND_HANDLE_RSP))
    {
        return;
    }

    att_pkt_create(conn, ATT_OP_HANDLE_VALUE_CFM);
    att_pkt_send(conn);
}

/*
 * Responses from the server
 ****************************************************************************/
ATT_HANDLER(error_rsp)
{
    CsrUint8 req_op;
    CsrUint16 handle;
    CsrUint8 rc;

    /* read pdu */
    req_op = read_uint8(&conn->in.data);
    handle = read_uint16(&conn->in.data);
    rc = read_uint8(&conn->in.data);

    /* send error to the application */
    if (att_pend_check(conn, (CsrUint8)(req_op | ATT_OP_RSP_MASK)))
    {
        att_error_cfm(ATT_STATE(phandle), conn, req_op, handle, rc);
    }

    return TRUE;
}

ATT_HANDLER(exchange_mtu_rsp)
{
    CsrUint16 mtu;

    /* read pdu */
    mtu = read_uint16(&conn->in.data);

#ifdef INSTALL_ATT_BREDR
    /* Exchange MTU is not allowed on BR/EDR connections */
    if (!L2CA_CONFLAG_IS_LE(conn->flags))
        return TRUE;
#endif /* INSTALL_ATT_BREDR */

    if (mtu < ATT_MTU_MIN)
        mtu = ATT_MTU_MIN;

    conn->mtu = CSRMIN(conn->mtu, mtu);

    /* send response to the application */
    att_exchange_mtu_cfm(ATT_STATE(phandle), conn->cid, conn->mtu,
                         ATT_RESULT_SUCCESS);

    return TRUE;
}

/* Read information response from server */
ATT_HANDLER(find_info_rsp)
{
    CsrUint16 handle;
    CsrUint32 uuid[4];
    CsrUint16 len;
    CsrUint16 dlen;
    att_uuid_type_t uuid_type;

    /* read pdu */
    switch (read_uint8(&conn->in.data))
    {
        case ATT_FORMAT_UUID16:
            uuid_type = ATT_UUID16;
            dlen = 4; /* 4 bytes of data per entry */
            break;

        case ATT_FORMAT_UUID128: /* UUID128 */
            uuid_type = ATT_UUID128;
            dlen = 18; /* 18 bytes of data per entry */
            break;

        default:
            /* invalid packet, set length to something that will fail
             * length check later and cause disconnect and cleanup */
            dlen = 0xffff;
            uuid_type = ATT_UUID_NONE; /* keep compiler happy */
    }

    len = IN_BUF_LEN(conn);

    /* check packet length */
    if (len % dlen)
    {
        return TRUE;
    }

    while (len)
    {
        att_result_t rc;

        handle = read_uint16(&conn->in.data);
        att_read_uuid(conn, uuid, uuid_type);
        len -= dlen;

        if (len)
        {
            rc = ATT_RESULT_SUCCESS_MORE;
        }
        else
        {
            rc = ATT_RESULT_SUCCESS;
        }

        att_find_info_cfm(ATT_STATE(phandle), conn->cid, handle, uuid_type,
                          uuid, rc);
    }

    return TRUE;
}

/* find by type value response from server */
ATT_HANDLER(find_by_type_value_rsp)
{
    CsrUint16 len;

    /* read pdu */
    len = IN_BUF_LEN(conn);

    /* check packet length */
    if (len % 4)
    {
        return TRUE;
    }

    while (len)
    {
        att_result_t rc;
        CsrUint16 handle;
        CsrUint16 end;

        handle = read_uint16(&conn->in.data);
        end = read_uint16(&conn->in.data);

        len -= 4;

        if (len)
        {
            rc = ATT_RESULT_SUCCESS_MORE;
        }
        else
        {
            rc = ATT_RESULT_SUCCESS;
        }

        att_find_by_type_value_cfm(ATT_STATE(phandle), conn->cid, handle, end,
                                   rc);
    }

    return TRUE;
}

/* Read by type response from server */
ATT_HANDLER(read_by_type_rsp)
{
    CsrUint16 handle;
    CsrUint16 len;
    CsrUint16 dlen;
    CsrUint8 *data = NULL;

    /* read pdu */
    dlen = read_uint8(&conn->in.data);
    len = IN_BUF_LEN(conn);

    /* check packet length */
    if (len % dlen)
        return TRUE;

    dlen -= 2; /* remove handle size from datalen */

    while (len)
    {
        att_result_t rc;

        len -= 2 + dlen;

        handle = read_uint16(&conn->in.data);

#ifndef BUILD_FOR_ROBINSON
        if (dlen && (data = xpmalloc(dlen)) != NULL)
        {
            CsrMemCpy(data, conn->in.data, dlen);
            conn->in.data += dlen;
        }
#else /* !BUILD_FOR_ROBINSON */
        data = conn->in.data;
        conn->in.data += dlen;
#endif /* BUILD_FOR_ROBINSON */

        if (len)
        {
            rc = ATT_RESULT_SUCCESS_MORE;
        }
        else
        {
            rc = ATT_RESULT_SUCCESS;
        }

        att_read_by_type_cfm(ATT_STATE(phandle), conn->cid, handle, rc, dlen,
                             data);
    }

    return TRUE;
}

/* Read, Read blob, Read multi response, handle value notification or
   indication from server */
ATT_HANDLER(read_common_rsp)
{
    CsrUint16 handle = 0;
    CsrUint16 len;
    CsrUint8 *data = NULL;

    /* read pdu */

    if (GET_IN_OP(conn) == ATT_OP_HANDLE_VALUE_NOT ||
        GET_IN_OP(conn) == ATT_OP_HANDLE_VALUE_IND)
    {
        handle = read_uint16(&conn->in.data);
    }

    len = IN_BUF_LEN(conn);

#ifndef BUILD_FOR_ROBINSON
    if (len && (data = xpmalloc(len)) != NULL)
    {
        CsrMemCpy(data, conn->in.data, len);
        conn->in.data += len;
    }
#else /* !BUILD_FOR_ROBINSON */
    data = conn->in.data;
    conn->in.data += len;
#endif /* BUILD_FOR_ROBINSON */

    switch (GET_IN_OP(conn))
    {
        case ATT_OP_READ_RSP:
            att_read_cfm(ATT_STATE(phandle), conn->cid, ATT_RESULT_SUCCESS,
                         len, data);
            break;

        case ATT_OP_READ_BLOB_RSP:
            att_read_blob_cfm(ATT_STATE(phandle), conn->cid,
                              ATT_RESULT_SUCCESS, len, data);
            break;

        case ATT_OP_READ_MULTI_RSP:
            att_read_multi_cfm(ATT_STATE(phandle), conn->cid,
                               ATT_RESULT_SUCCESS, len, data);
            break;

        case ATT_OP_HANDLE_VALUE_NOT:
            att_handle_value_ind(ATT_STATE(phandle), conn->cid, handle,
                                 ATT_HANDLE_VALUE_NOTIFICATION, len, data);
            break;

        case ATT_OP_HANDLE_VALUE_IND:
            att_handle_value_ind(ATT_STATE(phandle), conn->cid, handle,
                                 ATT_HANDLE_VALUE_INDICATION, len, data);
            break;
        default :
            if(data)
            {
                CsrPmemFree(data);
            }
            break;
    }

    return TRUE;
}

/* Read By Group Type response from server */
ATT_HANDLER(read_by_group_type_rsp)
{
    CsrUint16 handle;
    CsrUint16 end;
    CsrUint16 len;
    CsrUint16 dlen;
    CsrUint8 *data = NULL;

    /* read pdu */
    dlen = read_uint8(&conn->in.data);
    len = IN_BUF_LEN(conn);

    /* check packet length */
    if (len % dlen)
        return TRUE;

    dlen -= 4; /* remove handle and end sizes from datalen */

    while (len)
    {
        att_result_t rc;

        len -= 4 + dlen;

        handle = read_uint16(&conn->in.data);
        end = read_uint16(&conn->in.data);

#ifndef BUILD_FOR_ROBINSON
        if (dlen && (data = xpmalloc(dlen)) != NULL)
        {
            CsrMemCpy(data, conn->in.data, dlen);
            conn->in.data += dlen;
        }
#else /* !BUILD_FOR_ROBINSON */
        data = conn->in.data;
        conn->in.data += dlen;
#endif /* BUILD_FOR_ROBINSON */

        if (len)
        {
            rc = ATT_RESULT_SUCCESS_MORE;
        }
        else
        {
            rc = ATT_RESULT_SUCCESS;
        }

        att_read_by_group_type_cfm(ATT_STATE(phandle), conn->cid, rc,
                                   handle, end, dlen, data);
    }

    return TRUE;
}

/* Write, Write blob or Prepare write response from server */
ATT_HANDLER(write_common_rsp)
{
    /* read pdu */

    switch (GET_IN_OP(conn))
    {
        case ATT_OP_WRITE_RSP:
            att_write_cfm(ATT_STATE(phandle), conn->cid, ATT_RESULT_SUCCESS);
            break;

        case ATT_OP_PREPARE_WRITE_RSP:
        {
            CsrUint16 handle;
            CsrUint16 offs;
            CsrUint16 len;
            CsrUint8 *data = NULL;

            handle = read_uint16(&conn->in.data);
            offs = read_uint16(&conn->in.data);
            len = IN_BUF_LEN(conn);

#ifndef BUILD_FOR_ROBINSON
            if (len && (data = xpmalloc(len)) != NULL)
            {
                CsrMemCpy(data, conn->in.data, len);
                conn->in.data += len;
            }
#else /* !BUILD_FOR_ROBINSON */
            data = conn->in.data;
            conn->in.data += len;
#endif /* BUILD_FOR_ROBINSON */

            att_prepare_write_cfm(ATT_STATE(phandle), conn->cid, handle, offs,
                                  ATT_RESULT_SUCCESS, len, data);
            break;
        }
    }

    return TRUE;
}

/* Execute write response from server */
ATT_HANDLER(execute_write_rsp)
{
    att_execute_write_cfm(ATT_STATE(phandle), conn->cid, 0 /* handle */,
                          ATT_RESULT_SUCCESS);

    return TRUE;
}

#endif /* INSTALL_ATT_MODULE */
