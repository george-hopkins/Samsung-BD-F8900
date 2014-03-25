/*!
    Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
           All rights reserved.
\file   att_l2cap.c

\brief  Attribute Protocol interface to L2CAP.
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

#ifdef BUILD_FOR_ROBINSON
/* Memory reserved for sending ATT data to L2CAP. Array size kept to the
   maximum supported MTU which is 23 (ie., minimum BLE MTU size). */
CsrUint8 ll_data_pdu[ATT_MTU_BLE];

/* 'attUpPrim' variable is used as a memory block to send ATT primitives to higher layer */
ATT_UPRIM_T         att_hl_prim;
#endif /* BUILD_FOR_ROBINSON */

#define INVALID()                       { 0, 0, 0, NULL }
#define HANDLER(min, max, cmd, func)    { min, max, cmd, att_handle_##func }

/* Attribute protocol message info */
const att_handler_t att_handlers[] =
{
    /* min, max, cmd, handler */
    INVALID(),                                  /* 00 */
    HANDLER(5,  5, 0, error_rsp),               /* 01 Error rsp */
    HANDLER(3,  3, 0, exchange_mtu_req),        /* 02 Exchange MTU req */
    HANDLER(3,  3, 0, exchange_mtu_rsp),        /* 03 Exchange MTU rsp */
    HANDLER(5,  5, 0, find_info_req),           /* 04 Read Information rq */
    HANDLER(2,  0, 0, find_info_rsp),           /* 05 Read Information rsp */
    HANDLER(7,  0, 0, read_by_common_req),  /* 06 Find By Type Value req */
    HANDLER(1,  0, 0, find_by_type_value_rsp),  /* 07 Find By Type Value rsp */
    HANDLER(7, 21, 0, read_by_common_req),      /* 08 Read By Type req */
    HANDLER(4,  0, 0, read_by_type_rsp),        /* 09 Read By Type rsp */
    HANDLER(3,  3, 0, read_common_req),         /* 0a Read req */
    HANDLER(1,  0, 0, read_common_rsp),         /* 0b Read rsp */
    HANDLER(5,  5, 0, read_common_req),         /* 0c Read Blob req */
    HANDLER(1,  0, 0, read_common_rsp),         /* 0d Read Blob rsp */
    HANDLER(3,  0, 0, read_multi_req),          /* 0e Read Multiple req */
    HANDLER(1,  0, 0, read_common_rsp),         /* 0f Read Multiple rsp */
    HANDLER(7, 21, 0, read_by_common_req),      /* 10 Read By Group Type req */
    HANDLER(6,  0, 0, read_by_group_type_rsp),  /* 11 Read By Group Type rsp */
    HANDLER(3,  0, 1, write_common_req),        /* 12 Write req */
    HANDLER(1,  1, 0, write_common_rsp),        /* 13 Write rsp */
    INVALID(),                                  /* 14 */
    INVALID(),                                  /* 15 */
    HANDLER(5,  0, 1, prepare_write_req),       /* 16 Prepare Write req */
    HANDLER(5,  0, 0, write_common_rsp),        /* 17 Prepare Write rsp */
    HANDLER(2,  2, 1, execute_write_req),       /* 18 Execute Write req */
    HANDLER(1,  1, 0, execute_write_rsp),       /* 19 Execute Write rsp */
    INVALID(),                                  /* 1a */
    HANDLER(3,  0, 0, read_common_rsp),         /* 1b Handle Value notif */
    INVALID(),                                  /* 1c */
    HANDLER(3,  0, 0, read_common_rsp),         /* 1d Handle Value ind */
    HANDLER(1,  1, 0, handle_value_cfm),        /* 1e Handle Value cfm */
};

#define ATT_UUID_INVALID        (~ATT_UUID_NONE)

#ifndef BUILD_FOR_HOST
static CsrUint16 att_pdu_error_handle(CsrUint8 *pdu);
#endif /* BUILD_FOR_HOST */

#ifndef BUILD_FOR_ROBINSON
att_conn_t *att_conn_add(ATT_FUNC_STATE
                         l2ca_cid_t cid)
{
    att_conn_t *conn = zpnew(att_conn_t);

    if(conn)
    {
        conn->next = ATT_STATE(conn);
        ATT_STATE(conn) = conn;
        conn->cid = cid;
    }
    return conn;
}
#else /* !BUILD_FOR_ROBINSON */
att_conn_t *att_conn_add(l2ca_cid_t cid)
{
    CsrUint16 i;

    for (i = 0; i < ATT_MAX_BLE_U_CONNS; i++)
    {
        /* Search for available slot */
        if (ATT_STATE(conn[i].cid) == ATT_INVALID_CID)
        {
            ATT_STATE(conn[i].cid) = cid;
            return &ATT_STATE(conn[i]);
        }
    }

    return NULL;
}
#endif

#ifndef BUILD_FOR_ROBINSON
att_conn_t *att_conn_find(ATT_FUNC_STATE
                           l2ca_cid_t cid)
{
    att_conn_t *conn;

    for (conn = ATT_STATE(conn); conn; conn = conn->next)
    {
        if (conn->cid == cid)
            break;
    }

    return conn;
}
#else /* !BUILD_FOR_ROBINSON */
att_conn_t *att_conn_find(l2ca_cid_t cid)
{
    CsrUint16 i;

    for (i = 0; i < ATT_MAX_BLE_U_CONNS; i++)
    {
        if (ATT_STATE(conn[i].cid) == cid)
        {
            return &ATT_STATE(conn[i]);
        }
    }

    return NULL;
}
#endif /* BUILD_FOR_ROBINSON */

#ifndef BUILD_FOR_ROBINSON
void att_conn_rm(ATT_FUNC_STATE
                 l2ca_cid_t cid)
{
    att_conn_t *conn, **cp;

    for (cp = &ATT_STATE(conn); (conn = *cp) != NULL; cp = &conn->next)
    {
        if (conn->cid == cid)
        {
            /* update server list */
            *cp = conn->next;

            /* clear write queue */
            if (conn->cid != ATT_CID_LOCAL)
            {
                att_wq_free(conn);
            }

            if ((conn->pend & (PEND_AUTH_CHECK)) == PEND_NONE)
                /* free data buffers */
                CsrPmemFree(conn->in.buf);

            if ((conn->pend & (PEND_AUTH_SIGN)) == PEND_NONE)
                /* free data buffers */
                CsrPmemFree(conn->out.buf);

            CsrPmemFree(conn);
            break;
        }
    }
}
#else /* !BUILD_FOR_ROBINSON */
void att_conn_rm(l2ca_cid_t cid)
{
    CsrUint16 i;

    for (i = 0; i < ATT_MAX_BLE_U_CONNS; i++)
    {
        if (ATT_STATE(conn[i].cid) == cid)
        {
            ATT_STATE(conn[i].cid) = ATT_INVALID_CID;
            ATT_STATE(conn[i].size_wq) = 0;
            ATT_STATE(conn[i].pend) = PEND_NONE;
            ATT_STATE(conn[i].flags) = 0;
            break;
        }
    }
}
#endif /* BUILD_FOR_ROBINSON */

#ifdef INSTALL_ATT_BREDR
static CsrUint16 *att_conftab(CsrUint16 *size_conftab)
{
    CsrUint16 *conftab = CsrPmemAlloc(5 * sizeof(CsrUint16));

    *size_conftab = 5;
    conftab[0] = BKV_SEPARATOR;
    conftab[1] = L2CA_AUTOPT_DISABLE_RECONF;
    conftab[2] = L2CA_RECONF_DISABLE_ALL;
    conftab[3] = L2CA_AUTOPT_MTU_IN;
    conftab[4] = ATT_MTU_BREDR;

    return conftab;
}
#endif /* INSTALL_ATT_BREDR */

void att_connect_req(ATT_FUNC_STATE
                     ATT_CONNECT_REQ_T *req)
{
#ifdef INSTALL_ATT_BREDR
    /* BR/EDR connection */
    if (!L2CA_CONFLAG_IS_LE(req->flags))
    {
        CsrUint16 size_conftab;
        CsrUint16 *conftab = att_conftab(&size_conftab);

        L2CA_AutoConnectReq(0, ATT_PSM, &TBDADDR_ADDR(req->addrt), ATT_PSM,
                            0, 0, 0, size_conftab, conftab);
    }
    else
#endif /* INSTALL_ATT_BREDR */

    /* BLE connection */
    {
#ifndef BUILD_FOR_ROBINSON
        L2CA_MapFixedCidReq(&req->addrt,
                            L2CA_CID_ATTRIBUTE_PROTOCOL,
                            0,
                            (l2ca_conflags_t)(req->flags | L2CA_CONFLAG_LOCK_ACL));
#else /* !BUILD_FOR_ROBINSON */
        l2cap_map_fixed_cid_req(
            &req->bd_addr,
            ATTRIBUTE_PROTOCOL,
            FALSE,
            CON_ADDR_TYPE_OWN_PUBLIC | CON_ADDR_TYPE_PEER_PUBLIC);
#endif /* BUILD_FOR_ROBINSON */
    }
}

void att_disconnect(att_conn_t *conn)
{
#ifdef INSTALL_ATT_BREDR
    /* BR/EDR connection */
    if (!L2CA_CONFLAG_IS_LE(conn->flags))
    {
        L2CA_DisconnectReq(conn->cid);
    }
    else
#endif /* INSTALL_ATT_BREDR */

    /* BLE connection */
    {
#ifndef BUILD_FOR_ROBINSON
        L2CA_UnmapFixedCidReq(conn->cid);
#else /* !BUILD_FOR_ROBINSON */
        l2cap_unmap_fixed_cid_req(conn->cid, HCI_ERROR_OETC_USER);
#endif /* BUILD_FOR_ROBINSON */
    }
}

void att_disconnect_req(ATT_FUNC_STATE
                        ATT_DISCONNECT_REQ_T *req)
{
    att_conn_t *conn;

    /* invalid cid */
    if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
    {
        att_disconnect_ind(req->phandle,
                           req->cid,
                           (l2ca_disc_result_t)ATT_RESULT_INVALID_CID);
    }
    else
    {
        att_disconnect(conn);
    }
}

CsrUint16 att_pkt_create(att_conn_t *conn, CsrUint8 op)
{
    CsrUint8 *data;

    /* packet pending, just return bytes left in the buffer */
    if (conn->out.buf)
    {
        return (CsrUint16)(conn->mtu - (conn->out.data - conn->out.buf));
    }

#ifndef BUILD_FOR_ROBINSON
    data = CsrPmemAlloc(conn->mtu);
#else /* !BUILD_FOR_ROBINSON */
    data = ll_data_pdu;
#endif /* BUILD_FOR_ROBINSON */

    conn->out.buf = conn->out.data = data;

    /* store opcode */
    write_uint8(&conn->out.data, op);

    /* bytes available in the buffer */
    return (CsrUint16)(conn->mtu - (conn->out.data - conn->out.buf));
}

/****************************************************************************
NAME
    att_pkt_write - write data to the end of outgoing packet

*/
CsrUint16 att_pkt_write(att_conn_t *conn,
                       const CsrUint8 *value,
                       CsrUint16 size_value)
{
    CsrUint16 len;

    /* make sure not to copy over the buffer */
    len = (CsrUint16)CSRMIN(conn->mtu - (conn->out.data - conn->out.buf),
              size_value);

    CsrMemCpy(conn->out.data, value, len);
    conn->out.data += len;

    /* return bytes left on the buffer */
    return (CsrUint16)(conn->mtu - (conn->out.data - conn->out.buf));
}

void att_pkt_send(att_conn_t *conn)
{
    CsrUint16 len = (CsrUint16)(conn->out.data - conn->out.buf); /* packet length */
#ifndef BUILD_FOR_ROBINSON
    CsrMblk *mblk;

    if ((mblk = CsrMblkDataCreate(conn->out.buf, len, TRUE)) == NULL)
    {
        /* FIXME: this is a good place to panic */
        return;
    }
#endif /* !BUILD_FOR_ROBINSON */

    /* set pending flag */
    att_pend_set(conn, conn->out.buf[0]);

    /* send mblk to the peer */
    if (conn->cid != ATT_CID_LOCAL)
    {
#ifndef BUILD_FOR_ROBINSON
        L2CA_DataWriteReq(conn->cid, 0, mblk);
#else /* !BUILD_FOR_ROBINSON */
        l2cap_data_write_req(conn->cid, len, conn->out.buf);
#endif /* BUILD_FOR_ROBINSON */
    }

    /* send fake dataread back to ourself */
    else
#ifndef BUILD_FOR_ROBINSON
    {
        MAKE_PRIM(L2CA_DATAREAD_IND);
        prim->cid = ATT_CID_LOCAL;
        prim->data = mblk;
        CsrSchedMessagePut(ATT_IFACEQUEUE, L2CAP_PRIM, prim);
    }
#else /* !BUILD_FOR_ROBINSON */
    {
        L2CA_DATA_READ_IND_T ind;

        /* prim->ucid = ATT_INVALID_CID so we treat this as a local data */
        ind.cid = ATT_INVALID_CID;
        ind.num_bytes = len;
        ind.data = conn->out.buf;

        att_l2ca_dataread_ind(&ind, NULL);
    }
#endif /* BUILD_FOR_ROBINSON */

    /* clear outbuffer */
    conn->out.buf = conn->out.data = NULL;
}

/* Get UUID value from mblk */
CsrUint16 att_read_uuid(att_conn_t *conn, CsrUint32 *uuid, CsrUint16 uuid_type)
{
    CsrUint16 shifter;
    CsrUint16 pos = 1; /* good for UUID16 */
    CsrUint16 bits = 0; /* uuid size in bits */

    switch (uuid_type)
    {
        case ATT_UUID_NONE:
            /* determine uuid type by length */
            switch (IN_BUF_LEN(conn))
            {
                case 0: /* no filter */
                    return ATT_UUID_NONE;

                case 2: /* uuid16 */
                    bits = 16;
                    break;

                case 16: /* uuid128 */
                    bits = 128;
                    pos = 4;
                    break;

                default:
                    return ((CsrUint16)ATT_UUID_INVALID);
            }
            break;

        case ATT_UUID16:
            bits = 16;
            break;

        case ATT_UUID128:
            bits = 128;
            pos = 4;
            break;
    }

    /* read little endian UUID octet by octet */
    for (shifter = 0; shifter < bits; shifter += 8)
    {
        /* move to the next CsrUint32 (which is previous) */
        if ((shifter & 0x18) == 0x00)
        {
            pos--;
            uuid[pos] = 0;
        }

        /* & 0x18 makes sure we dont shift more than 24 bits at a time */
        uuid[pos] |= (CsrUint32)read_uint8(&conn->in.data) << (shifter & 0x18);
    }

    if (bits == 128 && !ATT_IS_UUID16(uuid))
        return ATT_UUID128;
    else
        return ATT_UUID16;
}

#ifndef BUILD_FOR_ROBINSON
static void l2ca_register_fixed_cid(L2CA_REGISTER_FIXED_CID_CFM_T *cfm)
{
    if (cfm->result != L2CA_MISC_SUCCESS ||
        cfm->fixed_cid != L2CA_CID_ATTRIBUTE_PROTOCOL)
    {
        /* FIXME: send register failure */
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
    }
}
#endif /* !BUILD_FOR_ROBINSON */

#ifndef BUILD_FOR_HOST
/* return the handle to be sent with the error code */
static CsrUint16 att_pdu_error_handle(CsrUint8 *pdu)
{
    CsrUint16 handle = 0x0000;

    /* Check on the opcode and extract handle appropriately */
    switch(pdu[0])
    {
        case ATT_OP_FIND_INFO_REQ:
            /* fall-through */
        case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
            /* fall-through */
        case ATT_OP_READ_BY_TYPE_REQ:
            /* fall-through */
        case ATT_OP_READ_BY_GROUP_TYPE_REQ:
            /* fall-through */
            /* Get the starting handle */
        case ATT_OP_READ_REQ:
            /* fall-through */
        case ATT_OP_READ_BLOB_REQ:
            /* fall-through */
        case ATT_OP_WRITE_REQ:
            /* fall-through */
        case ATT_OP_PREPARE_WRITE_REQ:
            /* Get the attr handle */
            handle = (pdu[2]<<8) | pdu[1];
            break;
        case ATT_OP_READ_MULTI_REQ:
            /* fall-through */
        case ATT_OP_EXECUTE_WRITE_REQ:
            /* fall-through */
            /* return handle as 0 */
        default:
            break;
    }

    return handle;
}
#endif /* BUILD_FOR_HOST */

/* get correct handler for an ATT packet and do basic checks */
const att_handler_t *get_pkt(CsrUint8 *op, att_conn_t *conn)
{
    const att_handler_t *pkt;
    CsrUint8 pdu;
    CsrUint16 extra_len = 0;

    /* read attribute pdu, minimum possible length is 1 byte */
    if (!conn->in.len)
    {
        return NULL;
    }

    /* get opcode */
    conn->in.data = &conn->in.buf[0]; /* set correct starting point */
    pdu = read_uint8(&conn->in.data);
    *op = pdu & ATT_OP_MASK;

    /* known opcode */
    if (*op < (sizeof(att_handlers) / sizeof(att_handler_t)))
        pkt = &att_handlers[*op];
    else
        return NULL;

    /* check that we have a handler */
    if (!pkt->handler)
        return NULL;

    /* check if command is allowed */
    if (pdu & ATT_OP_CMD_FLAG && !pkt->cmd)
        return NULL;

    /* check authentication signature availability */
    if (pdu & ATT_OP_AUTH_FLAG)
    {
        /* Write command is only PDU supporting auth */
        if (pdu != (ATT_OP_WRITE_REQ | ATT_OP_AUTH_FLAG | ATT_OP_CMD_FLAG))
            return NULL; /* auth not allowed */
        else
            extra_len += ATT_OP_AUTH_LEN; /* auth sig must be present */

        /* swap auth bit */
        conn->pend ^= PEND_AUTH_CHECK;
    }

    /* check packet minimum length */
    if (conn->in.len < pkt->min + extra_len)
        return NULL;

    /* check packet maximum length */
    if (pkt->max && conn->in.len + extra_len > pkt->max)
        return NULL;

    /* check authentication signature */
    if (pdu & ATT_OP_AUTH_FLAG)
    {
        if (conn->pend & PEND_AUTH_CHECK)
            att_dm_sm_data_sign_req(conn, TRUE,
                                    (CsrUint16)(conn->in.len - ATT_OP_AUTH_LEN),
                                    conn->in.buf);
        else
            /* signature valid, remove from buffer */
            conn->in.len -= ATT_OP_AUTH_LEN;
    }

    /* if we made it that far the packet is ready to be processed */
    return pkt;
}

#ifndef BUILD_FOR_ROBINSON
void att_l2ca_dataread_ind(ATT_FUNC_STATE
                           L2CA_DATAREAD_IND_T *ind, att_conn_t *c)
#else /* !BUILD_FOR_ROBINSON */
void att_l2ca_dataread_ind(L2CA_DATA_READ_IND_T *ind, att_conn_c *c)
#endif /* BUILD_FOR_ROBINSON */
{
    const att_handler_t *pkt = NULL;
    att_conn_t *conn;
    CsrUint8 op = 0;
    CsrBool pkt_done = TRUE;
#ifndef BUILD_FOR_HOST
    CsrUint8 *pdu = NULL;
    CsrMblkSize data_length = 0;
    CsrUint16 err_handle = 0;
#endif /* !BUILD_FOR_HOST */
    att_in_t *p_in = NULL;
    att_in_t *p_conn_in = NULL;

    /* process packet with verified signature */
    if (c)
    {
        conn = c;
    }

    /* process new incoming packet */
    else
    {
#ifndef BUILD_FOR_ROBINSON
        /* ack data */
        if (ind->cid != ATT_CID_LOCAL)
            L2CA_DataReadRsp(ind->cid, ind->packets);
#endif /* !BUILD_FOR_ROBINSON */

        if ((conn = att_conn_find(ATT_ARG ind->cid)) == NULL)
        {
            return;
        }

        p_conn_in = p_in = &conn->in;

        /* Are we waiting for any responses from upper-layer or DM ? */
        if(conn->handler != NULL || conn->pend & PEND_AUTH_CHECK)
        {
            /* As we are awaiting response from upper-layers (acc_rsp) or dm (signing),
               send an error (on-chip) or add the pdu in pending queue (off-chip) */
#ifndef BUILD_FOR_HOST
#ifndef BUILD_FOR_ROBINSON
            /* Get the opcode from the MBLK to send err-rsp */
            data_length = CsrMblkGetLength(ind->data);
            pdu = CsrPmemAlloc(data_length);
            CsrMblkCopyToMemory(ind->data, 0, data_length, pdu);

            /* Check if we received enough bytes to search for handle */
            if(data_length >= 3)
            {
                err_handle = att_pdu_error_handle(pdu);
            }
#else /* !BUILD_FOR_ROBINSON */
            pdu = ind->data;

            /* Check if we received enough bytes to search for handle */
            if(ind->num_bytes >= 3)
            {
                err_handle = att_pdu_error_handle(pdu);
            }
#endif /* BUILD_FOR_ROBINSON */
            att_send_error_rsp(conn, pdu[0], err_handle, ATT_RESULT_INSUFFICIENT_RESOURCES);
#ifndef BUILD_FOR_ROBINSON
            CsrPmemFree(pdu);
#endif /* BUILD_FOR_ROBINSON */
            /* Nothing to do anything further, wait for pending rsp's from outside world */
            return;
#else /* !BUILD_FOR_HOST */
            /* Add a node to the pending queue (conn->in used here) for future processing */
            p_in = (att_in_t *)CsrPmemAlloc(sizeof(att_in_t));

            for(;p_conn_in->next != NULL; p_conn_in = (att_in_t *)p_conn_in->next) /* nop */;

            /* Link the new node */
            p_conn_in->next = (att_in_t *)p_in;
            p_in->next = NULL;

            /* reset the pointer */
            p_conn_in = &conn->in;
#endif /* !BUILD_FOR_HOST */
        }

        /* verify incoming packet length */
        if ((p_in->len = CsrMblkGetLength(ind->data)) > conn->mtu)
        {
            /* MTU violation, just get rid of the remote */
            att_disconnect(conn);
#ifdef BUILD_FOR_HOST
            /* Free the current node if there was already a handler */
            if(conn->handler)
            {
                for(;p_conn_in->next != p_in; p_conn_in = (att_in_t *)p_conn_in->next) /* nop */;
                p_conn_in->next = NULL;
                CsrPmemFree(p_in);
            }
#endif
            return;
        }

        /* get rx buffer */
#ifndef BUILD_FOR_ROBINSON
        /* Get a memory block from an MBLK */
        p_in->buf = CsrPmemAlloc(p_in->len);
        CsrMblkCopyToMemory(ind->data, 0, p_in->len, p_in->buf);
#else /* !BUILD_FOR_ROBINSON */
        p_in->len = ind->num_bytes;
        p_in->buf = ind->data;
#endif /* BUILD_FOR_ROBINSON */
        p_in->data = p_in->buf;
#ifdef BUILD_FOR_HOST
        /* we have copied the data to the list, so don't handle if already one is pending */
        if(conn->handler != NULL || conn->pend & PEND_AUTH_CHECK)
            return;
#endif
    }

    /* handle packet */
    if ((pkt = get_pkt(&op, conn)) != NULL)
    {
        if (conn->pend & PEND_AUTH_CHECK)
        {
            return;
        }

        if (att_pend_check(conn, op))
        {
            /* handle pdu */
            if ((pkt_done = pkt->handler(ATT_ARG conn, NULL)) == FALSE)
            {
                conn->handler = pkt->handler;
            }
        }
    }

    /* send error, but only if this was a request */
    else if (!(op & ATT_OP_RSP_MASK)) {
        att_send_error_rsp(conn, op, 0, ATT_RESULT_REQUEST_NOT_SUPPORTED);
    }

    /* else error in response/command, just ignore for now */

    if (pkt_done)
    {
#ifndef BUILD_FOR_ROBINSON
        CsrPmemFree(conn->in.buf);
        conn->in.data = conn->in.buf = NULL;
        conn->in.len = 0;
#endif /* !BUILD_FOR_ROBINSON */
        conn->handler = NULL;
    }
}

static void l2ca_map_fixed_cid_cfm(ATT_FUNC_STATE
                                   L2CA_MAP_FIXED_CID_CFM_T *cfm)
{
    att_conn_t *conn = NULL;

    /* Find conn if already added else add an instance of conn */
    if ((conn = att_conn_find(ATT_ARG cfm->cid)) == NULL
     && (conn = att_conn_add(ATT_ARG cfm->cid)) == NULL)
    {
#ifndef BUILD_FOR_ROBINSON
        L2CA_UnmapFixedCidReq(cfm->cid);
#else /* !BUILD_FOR_ROBINSON */
        l2cap_unmap_fixed_cid_req(cfm->cid, HCI_ERROR_OETC_LOW_RESOURCE);
#endif /* BUILD_FOR_ROBINSON */
        return;
    }
#ifndef BUILD_FOR_ROBINSON
    conn->flags = cfm->flags; /* TODO: check that this is not needed
                                   * on Robinson */
#endif /* !BUILD_FOR_ROBINSON */
    conn->mtu = ATT_MTU_BLE;

#ifdef BUILD_FOR_HOST
    tbdaddr_copy(&conn->addrt, &cfm->addrt);
#endif /* BUILD_FOR_HOST */

#ifndef BUILD_FOR_ROBINSON
    att_connect_cfm(ATT_STATE(phandle), &cfm->addrt, cfm->flags, ATT_MTU_BLE,
                    cfm->cid, (l2ca_conn_result_t)cfm->result);
#else /* !BUILD_FOR_ROBINSON */
    att_connect_cfm(ATT_STATE(phandle), cfm->bd_addr, 0, 0, /* FIXME: remove */
                    cfm->cid, cfm->status);
#endif /* BUILD_FOR_ROBINSON */

    /* Incase of failure, unmap cid */
    if(cfm->result == L2CA_MISC_FAILED)
    {
#ifndef BUILD_FOR_ROBINSON
        L2CA_UnmapFixedCidReq(cfm->cid);
#else /* !BUILD_FOR_ROBINSON */
        l2cap_unmap_fixed_cid_req(cfm->cid, HCI_ERROR_OETC_USER);
#endif /* BUILD_FOR_ROBINSON */
    }
}

static void l2ca_map_fixed_cid_ind(ATT_FUNC_STATE
                                   L2CA_MAP_FIXED_CID_IND_T *ind)
{
    att_conn_t *conn;

    if ((conn = att_conn_add(ATT_ARG ind->cid)) != NULL)
    {
#ifndef BUILD_FOR_ROBINSON
        conn->flags = ind->flags;
#endif /* !BUILD_FOR_ROBINSON */
        conn->mtu = ATT_MTU_BLE;

#ifdef BUILD_FOR_HOST
        tbdaddr_copy(&conn->addrt, &ind->addrt);
#endif /* BUILD_FOR_HOST */
    }

#ifndef BUILD_FOR_ROBINSON
    /* Lock ACL. The idle timeout may unlock it later. If clients are
     * active, idle timeout won't unlock as that's up to the client */
    L2CA_MapFixedCidRsp(ind->cid, 0, (l2ca_conflags_t)(ind->flags|L2CA_CONFLAG_LOCK_ACL));
    att_connect_ind(ATT_STATE(phandle), &ind->addrt, ind->flags, ATT_MTU_BLE,
                    ind->cid);
#else /* !BUILD_FOR_ROBINSON */
    att_connect_ind(ATT_STATE(phandle), ind->bd_addr, 0, 0 /* FIXME: remove */,
                    ind->cid);
#endif /* BUILD_FOR_ROBINSON */
}

static void conn_clean(ATT_FUNC_STATE
                       att_conn_t *conn)
{
    /* cancel any outstanding timers */
    att_timer_cancel();

    /* send pending responses, we don't know handle, so lets just use 0 */

    if (conn->pend & PEND_HANDLE_CFM)
    {
        att_error_cfm(ATT_STATE(phandle), conn, ATT_OP_HANDLE_VALUE_IND,
                      0, ATT_RESULT_TIMEOUT);
    }

    if (PEND_GET_RSP())
    {
        att_error_cfm(ATT_STATE(phandle), conn, (CsrUint8)PEND_GET_RSP(),
                      0, ATT_RESULT_TIMEOUT);
    }

#if 0
    if (PEND_GET_RSP())
    {
        att_error_cfm(conn, (CsrUint8)PEND_GET_RSP(), 0, ATT_RESULT_TIMEOUT);
    }
#endif
}

#ifdef BUILD_FOR_ROBINSON
static void l2ca_unmap_fixed_cid_cfm(L2CA_UNMAP_FIXED_CID_CFM_T *cfm)
{
    att_conn_t *conn;

    if (!(conn = att_conn_find(ATT_ARG cfm->cid)))
    {
        return;
    }

    conn_clean(ATT_ARG conn);

    att_disconnect_cfm(ATT_STATE(phandle), cfm->cid, cfm->status);

    att_conn_rm(cfm->cid);
}
#endif /* BUILD_FOR_ROBINSON */

static void l2ca_disconnect_ind(ATT_FUNC_STATE
                                l2ca_cid_t cid, l2ca_disc_result_t reason)
{
    att_conn_t *conn;

    if ((conn = att_conn_find(ATT_ARG cid)) == NULL)
    {
        return;
    }

    conn_clean(ATT_ARG conn);

    att_disconnect_ind(ATT_STATE(phandle), cid, reason);

    att_conn_rm(ATT_ARG cid);
}

#ifdef INSTALL_ATT_BREDR
static void l2ca_auto_connect_cfm(ATT_FUNC_STATE
                                  L2CA_AUTO_CONNECT_CFM_T *cfm)
{
    att_conn_t *conn = NULL;
    CsrUint16 mtu = 0;
    TYPED_BD_ADDR_T addrt;
    l2ca_conflags_t cfm_flags = L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR);

    /* Copy the BD ADDR for att connect cfm */
    tbdaddr_copy_from_public_bd_addr(&addrt, &cfm->bd_addr);

    if ((conn = att_conn_find(ATT_ARG cfm->cid)) != NULL)
    {
        /* A Non-NULL value means the conn instance is incoming type */
        cfm_flags |= L2CA_CONFLAG_INCOMING;
    }

    if (cfm->result == L2CA_CONNECT_SUCCESS)
    {
        if(conn == NULL)
        {
            /* If conn is still NULL then it means that the conn is outgoing
               type and we have to allocate an instance for it.
            */
            if ((conn = att_conn_add(ATT_ARG cfm->cid)) == NULL)
            {
                L2CA_DisconnectReq(cfm->cid);
                return;
            }
        }

        /* Copy BD_ADDR in the att data structure (applicable for HOST Build only) */
#ifdef BUILD_FOR_HOST
        tbdaddr_copy(&conn->addrt, &addrt);
#endif /* BUILD_FOR_HOST */
        mtu = conn->mtu = CSRMIN(cfm->config.mtu, ATT_MTU_BREDR);
    }
    else
    {
        CsrPmemFree(conn);
    }

    att_connect_cfm(ATT_STATE(phandle), &addrt, cfm_flags, mtu, cfm->cid, cfm->result);
}

static void l2ca_auto_connect_ind(ATT_FUNC_STATE
                                  L2CA_AUTO_CONNECT_IND_T *ind)
{

    att_conn_t *conn;
    TYPED_BD_ADDR_T addrt;

    if ((conn = att_conn_add(ATT_ARG ind->cid)) != NULL)
    {
        conn->flags = L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR)
                                          |L2CA_CONFLAG_INCOMING;
        conn->mtu = ATT_MTU_BREDR;

        tbdaddr_copy_from_public_bd_addr(&addrt, &ind->bd_addr);

        conn->identifier = ind->identifier;
        conn->con_ctx = ind->reg_ctx;

        att_connect_ind(ATT_STATE(phandle), &addrt, conn->flags, conn->mtu,
                ind->cid);
    }
    else
    {
        L2CA_AutoConnectRsp(ind->identifier, ind->cid, (l2ca_conn_result_t)L2CA_CONNECT_REJ_RESOURCES,
                0,0, NULL);
    }
}

void att_connect_rsp(ATT_FUNC_STATE
                    ATT_CONNECT_RSP_T *rsp)
{
    att_conn_t *conn;

    CsrUint16 size_conftab = 0;
    CsrUint16 *conftab = NULL;

    if ((conn = att_conn_find(ATT_ARG rsp->cid)) == NULL)
    {
        /* silently ignore */
        return;
    }

    if(rsp->response == ATT_RESULT_SUCCESS)
    {
        conftab = att_conftab(&size_conftab);
    }

    L2CA_AutoConnectRsp(conn->identifier, rsp->cid, rsp->response,
                        conn->con_ctx, size_conftab, conftab);
}
#endif /* INSTALL_ATT_BREDR */

#ifndef BUILD_FOR_ROBINSON
void att_l2cap_handler(ATT_FUNC_STATE
                       CsrUint16 type, L2CA_UPRIM_T *prim)
{
    switch (type)
    {
#ifdef INSTALL_ATT_BREDR
        case L2CA_REGISTER_CFM:
            break;

        case L2CA_AUTO_CONNECT_IND:
            l2ca_auto_connect_ind(ATT_ARG &prim->l2ca_auto_connect_ind);
            break;

        case L2CA_AUTO_CONNECT_CFM:
            l2ca_auto_connect_cfm(ATT_ARG &prim->l2ca_auto_connect_cfm);
            break;

        case L2CA_DISCONNECT_IND:
            L2CA_DisconnectRsp(
                prim->l2ca_disconnect_ind.identifier,
                prim->l2ca_disconnect_ind.cid);
            l2ca_disconnect_ind(
                ATT_ARG
                prim->l2ca_disconnect_ind.cid,
                prim->l2ca_disconnect_ind.reason);
            break;

        case L2CA_DISCONNECT_CFM:
            l2ca_disconnect_ind(
                ATT_ARG
                prim->l2ca_disconnect_cfm.cid,
                prim->l2ca_disconnect_cfm.result);
            break;

        case L2CA_TIMEOUT_IND:
            /* ignore, and let the link eventually drop down
             * in case the peer really is dead. */
            break;
#endif /* INSTALL_ATT_BREDR */

        case L2CA_REGISTER_FIXED_CID_CFM:
            l2ca_register_fixed_cid(&prim->l2ca_register_fixed_cid_cfm);
            break;

        case L2CA_DATAREAD_IND:
            att_l2ca_dataread_ind(ATT_ARG &prim->l2ca_dataread_ind, NULL);
            break;

        case L2CA_DATAWRITE_CFM:
            /* ignore for now, even errors */
            break;

        case L2CA_MAP_FIXED_CID_IND:
            l2ca_map_fixed_cid_ind(ATT_ARG &prim->l2ca_map_fixed_cid_ind);
            break;

        case L2CA_MAP_FIXED_CID_CFM:
            l2ca_map_fixed_cid_cfm(ATT_ARG &prim->l2ca_map_fixed_cid_cfm);
            break;

        case L2CA_UNMAP_FIXED_CID_IND:
            l2ca_disconnect_ind(
                ATT_ARG
                prim->l2ca_unmap_fixed_cid_ind.cid,
                prim->l2ca_unmap_fixed_cid_ind.reason);
            break;

        default:
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
            break;
    }

    L2CA_FreePrimitive(prim);
}

#else /* !BUILD_FOR_ROBINSON */

void att_l2cap_handler(CsrUint16 type, void *prim)
{
    switch (type)
    {
        case L2CAP_DATA_READ_IND:
            l2ca_dataread_ind((L2CA_DATA_READ_IND_T *)prim);
            break;

        case L2CAP_DATA_WRITE_CFM:
            /* ignore for now, even errors */
            break;

        case L2CAP_MAP_FIXED_CID_IND:
            l2ca_map_fixed_cid_ind((L2CA_MAP_FIXED_CID_IND_T *)prim);
            break;

        case L2CAP_MAP_FIXED_CID_CFM:
            l2ca_map_fixed_cid_cfm((L2CA_MAP_FIXED_CID_CFM_T *)prim);
            break;

        case L2CAP_UNMAP_FIXED_CID_IND:
            l2ca_disconnect_ind(((L2CA_UNMAP_FIXED_CID_IND_T *)prim)->cid,
                                ((L2CA_UNMAP_FIXED_CID_IND_T *)prim)->reason);
            break;

        case L2CAP_UNMAP_FIXED_CID_CFM:
            l2ca_unmap_fixed_cid_cfm((L2CA_UNMAP_FIXED_CID_CFM_T *)prim);
            break;
        default:
            fault(type);
            break;
    }
}
#endif /* BUILD_FOR_ROBINSON */

#endif /* INSTALL_ATT_MODULE */
