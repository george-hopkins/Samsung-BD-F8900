/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_server.c

\brief  Attribute Protocol Server Interface
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

/****************************************************************************
NAME
    check_auth - check client authentication

*/
static att_result_t check_auth(ATT_FUNC_STATE att_conn_t *conn, att_attr_t *attr, CsrUint16 op)
{
    att_result_t rc;
    CsrUint16 flags = att_attr_flags(attr);
    CsrUint16 sec;
    TYPED_BD_ADDR_T addrt;
    DM_ACL_T *p_acl;
    CsrUint16 ch_properties = att_attr_perm(ATT_ARG attr);/*
                                                 * variable holds chacateristics properties
                                                 * as defined in table 3.5 of GATT spec.
                                                 */


    rc = ATT_RESULT_SUCCESS;


    if (conn->cid == ATT_CID_LOCAL)
    {
        /* local connection is as secure as it ever can be */
        return ATT_RESULT_SUCCESS;
    }

    /* Shift flags to correct position */
    if (op & ATT_PERM_READ)
        flags = (flags >> ATT_ATTR_SEC_R_OFFS) & ATT_ATTR_SEC_MASK;
    else
        flags = (flags >> ATT_ATTR_SEC_W_OFFS) & ATT_ATTR_SEC_MASK;


#ifndef BUILD_FOR_HOST
    L2CA_GetBdAddrFromCid(conn->cid, &addrt);
#else /* !BUILD_FOR_HOST */
    tbdaddr_copy(&addrt, &conn->addrt);
#endif /* BUILD_FOR_HOST */


    if ((p_acl = dm_acl_find_by_tbdaddr(&addrt)) == NULL)
        return ATT_RESULT_DEVICE_NOT_FOUND;

    if(dm_acl_is_ble(p_acl))
    {
        sec=0x00;

        /* find required security level*/
#ifndef BUILD_FOR_HOST
        if(att_type_declaration==attr->type)
#else /* Not BUILD_FOR_HOST */
        if(ATT_UUID_CHARACTERISTIC==(CsrUint16)(attr->uuid[0] & 0xffff))
#endif /* Not BUILD_FOR_HOST */
        {
            if(ch_properties&ATT_PERM_AUTHENTICATED)
            {
                sec |= DM_SM_LE_SIGNING;
            }
        }

        switch (flags)
        {
            case ATT_ATTR_SEC_NONE:
                /* Nothing to handle - no security upgrade */
            break;

            case ATT_ATTR_SEC_ENCRYPTION:
                /* Set the encryption bit even when signing is
                 * required.
                 */
                sec|=DM_SM_LE_ENCRYPTION;
                break;

            case ATT_ATTR_SEC_AUTHENTICATION:
                sec|=DM_SM_LE_MITM;
                break;

            case ATT_ATTR_SEC_RESERVED:
                /* ATT_ATTR_SEC_RESERVED, just ignore now, perhaps in the
                 * future this might have some meaning */
                break;
        }

        /* if MITM flag is ON, it must be established */
        if((sec & DM_SM_LE_MITM)&&
            !dm_sm_is_ulp_link_authenticated(&addrt))
            return ATT_RESULT_INSUFFICIENT_AUTHENTICATION;

        /* if encryption flag is ON, it must be established */
        if(sec & DM_SM_LE_ENCRYPTION)
        {
#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
            if(p_acl->dm_acl_client_sm.encrypting)
            {
                /* add a delay here */
                return ATT_RESULT_ENCRYPTION_PENDING;
            }
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */
            if(!DM_SM_IS_LINK_ENCRYPTED(p_acl))
                /* Encryption is required but not present */
                rc = ATT_RESULT_INSUFFICIENT_ENCRYPTION;
            else
                return rc;
        }

        /*
         * The code will touch here in two cases -
         * 1. Encryption is not required
         * 2. Encryption is required but failed.
         *
         * Case 1 -
         * If signing is failed, then return insufficient authentication, else rc value is success
         *
         * Case 2 -
         * If signing is success, change return code to success.
         * If signing is failed, then change error code only if encryption failue has not set it.
         *                       otherwise it is already set to insufficient encryption
         *
         */
        if(sec & DM_SM_LE_SIGNING)
        {
            if(conn->in.buf[0] & ATT_OP_AUTH_FLAG)
                rc = ATT_RESULT_SUCCESS;
            else if(!(sec & DM_SM_LE_ENCRYPTION))
                rc=ATT_RESULT_INSUFFICIENT_AUTHENTICATION;
        }
    }
    else
    {
        /* Signing is not defined for BR/EDR and it is not required
           to check characteristics properties flag */
        switch (flags)
        {
            case ATT_ATTR_SEC_ENCRYPTION:
                sec = HCI_MITM_NOT_REQUIRED_NO_BONDING;
                rc = ATT_RESULT_INSUFFICIENT_ENCRYPTION;
                break;

            case ATT_ATTR_SEC_AUTHENTICATION:
                /* As per BT 4.0 specification, when MITM is present for
                 * authentication requirements, first bit is set.
                 * This is true in all there cases "no bonding", dedicated
                 * bonding" and "general bonding" with MITM.
                 * Therefore DM is asked to check MITM only by setting first bit.
                 */
                sec = HCI_MITM_REQUIRED_NO_BONDING;
                rc = ATT_RESULT_INSUFFICIENT_AUTHENTICATION;
                break;
            case ATT_ATTR_SEC_NONE:
                /* Fall through */
            default:
                /* ATT_ATTR_SEC_RESERVED, just ignore now, perhaps in the
                 * future this might have some meaning */
                return ATT_RESULT_SUCCESS;
        }
        if(dm_sm_current_security_acceptable(&addrt, sec) == DM_SM_ACCEPTABLE)
            rc = ATT_RESULT_SUCCESS;
    }
    return rc;
}

#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
/*
 * This fix is added for B-108559 for synergy only.
 * There is race between encryption change indication and
 * ATT_ command from other side. There is actually a encryption change and ATT_
 * command is expecting that change.But encryption change indication is received
 * after the ATT command causing the ATT command to fail.
 *
 * This is a bug in FW B-108465. But for temporary purpose,
 * it is being fixed in Bluestack.
 */
static void att_cmd_after_delay(CsrUint16 cid, void *ptr)
{
    att_conn_t *conn;

#ifndef ATT_GLOBAL_STATE
    struct att_state_tag * att = (struct att_state_tag *)ptr;
#endif /* ATT_GLOBAL_STATE */

    if ((conn = att_conn_find(ATT_ARG cid)) == NULL ||
        conn->handler == NULL)
    {
        /* silently ignore */
        return;
    }

    if (conn->handler(ATT_ARG conn, NULL))
    {
#ifndef BUILD_FOR_ROBINSON
        CsrPmemFree(conn->in.buf);
        conn->in.data = conn->in.buf = NULL;
        conn->in.len = 0;
#endif /* !BUILD_FOR_ROBINSON */
        conn->handler = NULL;
    }
}
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */



#ifdef BUILD_FOR_HOST
/****************************************************************************
NAME
    att_check_attr_length_in_wq - Check whether the length of attr's in the
    write queue are within defined limits if they are fixed length attr's
*/
static att_result_t att_check_attr_length_in_wq(ATT_FUNC_STATE att_conn_t *conn)
{
    att_writeq_t *wq, *wq_node;
    CsrUint16 handle = 0xffff;
    att_attr_t *attr = NULL;
    att_result_t rc = ATT_RESULT_SUCCESS;
    CsrUint16 max_length = 0;
    CsrUint16 DONE_MASK = 0x8000; /* Safe as attr len will never be >= hex(8000) */

    for (wq = conn->wq;
         wq;
         max_length = 0, handle = 0xffff, wq = att_wq_next(conn, wq))
    {
        if ((rc == ATT_RESULT_SUCCESS) && (wq->handle < handle))
            attr = att_attr_find(ATT_ARG wq->handle, &handle);

        /* Ensure we are not on a flagged-node */
        if(!(wq->len & DONE_MASK))
        {
            for (wq_node = wq;
                 wq_node && (rc == ATT_RESULT_SUCCESS);
                 wq_node = att_wq_next(conn, wq_node))
            {
                /* Check that we are on a matching node of the write queue */
                if (handle == wq_node->handle)
                {
                    /* We found the handle here, tap the max length*/
                    if((wq_node->offs + wq_node->len) > max_length)
                        max_length = wq_node->offs + wq_node->len;

                    /* Flag this node as done */
                    wq_node->len |= DONE_MASK;
                }
            }
        }

        /* Verify that max_length is within limits */
        if(attr
          && !(attr->flags & ATT_ATTR_DYNLEN)
          && max_length
          && max_length > att_attr_size(attr))
            rc = ATT_RESULT_INVALID_LENGTH;

        /* Ensure that this flag is removed by the time we exit the loop */
        wq->len &= ~DONE_MASK;
    }

    return rc;
}
#endif /* BUILD_FOR_HOST */

/****************************************************************************
NAME
    att_is_access_permitted - Check whether the access to the
    attribute is permitted on a certain radio (LE/BREDR)
*/
static CsrBool att_is_access_permitted(att_conn_t *conn, att_attr_t *attr)
{
    CsrBool rc = TRUE;
#ifdef BUILD_FOR_HOST
    CsrUint16 flags = att_attr_flags(attr);

    if (conn->cid == ATT_CID_LOCAL)
    {
        /* On a local channel there is check
         *  not required for radio type.
         */
        return rc;
    }

#ifdef INSTALL_ATT_BREDR
    /* BR/EDR connection */
    if (!L2CA_CONFLAG_IS_LE(conn->flags))
    {
        if(flags & ATT_ATTR_DISABLE_ACCESS_BR_EDR)
        {
            rc = FALSE;
        }
    }
    else
    {  /* LE */
        if(flags & ATT_ATTR_DISABLE_ACCESS_LE)
        {
            rc = FALSE;
        }
    }

#endif /* !INSTALL_ATT_BREDR */
#endif /* !BUILD_FOR_HOST */
     return rc;
}

/*
 * Requests from the client
 ****************************************************************************/
ATT_HANDLER(exchange_mtu_req)
{
    CsrUint16 mtu;

#ifdef INSTALL_ATT_BREDR
    /* Exchange MTU is not allowed on BR/EDR connections */
    if (!L2CA_CONFLAG_IS_LE(conn->flags))
    {
        att_send_error_rsp(conn, ATT_OP_EXCHANGE_MTU_REQ,
                           0, ATT_RESULT_REQUEST_NOT_SUPPORTED);
        return TRUE;
    }
#endif

    /* read pdu */
    mtu = read_uint16(&conn->in.data);

    /* check that MTU is sane */
    if (mtu < ATT_MTU_MIN)
    {
        mtu = ATT_MTU_MIN;
    }
    else if (mtu > ATT_MTU_MAX)
    {
        mtu = ATT_MTU_MAX;
    }

    /* Setting MTU is allowed only once per session... */
    /* ...but there is no error for doing it multiple times */

    att_exchange_mtu_ind(ATT_STATE(phandle), conn->cid, mtu);

    /* store mtu temporarily */
    /* flow control will prevent us using this before application has
     * responded, and in case of timeout we'll just disconnect. In case
     * of notifications we might use too large MTU but client must be
     * able to process those as it requested the MTU. */
    conn->mtu = mtu;

    return TRUE;
}

/* Find Information Request from the client */
ATT_HANDLER(find_info_req)
{
    CsrUint16 start;
    CsrUint16 end;
    att_attr_t *attr;
    CsrUint16 len = 0;
    CsrUint16 needlen = 2; /* handle */
    CsrUint16 handle;

    /* read pdu */
    start = read_uint16(&conn->in.data);
    end = read_uint16(&conn->in.data);

    /* check parameters */
    if (!start || start > end)
    {
        att_send_error_rsp(conn, GET_IN_OP(conn),
                           start, ATT_RESULT_INVALID_HANDLE);
        return TRUE;
    }

    /* traverse through database */
    for (attr = att_attr_find(ATT_ARG start, &handle);
         attr && handle <= end;
         attr = att_attr_next(ATT_ARG attr, &handle))
    {
        /* create packet */
        if (!conn->out.data)
        {
            len = att_pkt_create(conn, ATT_OP_FIND_INFO_RSP);

            /* choose format based on the first attribute */
            if (att_attr_isuuid16(attr))
            {
                write_uint8(&conn->out.data, ATT_FORMAT_UUID16);
                needlen += 2;
            }
            else
            {
                write_uint8(&conn->out.data, ATT_FORMAT_UUID128);
                needlen += 16;
            }
        }

        /* we are filling packet with different sized UUID */
        if (att_attr_isuuid16(attr))
        {
            if (needlen != 4)
                break;
        }
        else
        {
            if (needlen != 18)
                break;
        }

        /* write handle */
        write_uint16(&conn->out.data, handle);

        /* write UUID */
        att_attr_read_uuid(&conn->out.data, attr);

        len -= needlen;

        /* check if there is room for next handle */
        if (needlen > len)
        {
            break;
        }
    }

    if (conn->out.data)
    {
        att_pkt_send(conn);
    }

    else
        att_send_error_rsp(conn, GET_IN_OP(conn),
                           start, ATT_RESULT_ATTR_NOT_FOUND);

    return TRUE;
}

/****************************************************************************
NAME
    add_read_by_common_attr - add attribute data to common read by uuid
                              response
*/
static CsrBool add_read_by_common_attr(att_conn_t *conn,
                                      CsrUint16 handle,
                                      CsrUint16 end,
                                      att_attr_t **attrp,
                                      ATT_ACCESS_RSP_T **irqp)
{
    att_attr_t *attr = *attrp;
    ATT_ACCESS_RSP_T *irq = *irqp;
    CsrUint16 size_value;
    CsrUint16 hdrlen;
    CsrUint16 len = 0;
    CsrUint8 maxlen = 0;

    /* get data length */
    if (irq)
    {
        size_value = irq->size_value;
    }
    else
    {
        size_value = att_attr_size(attr);
    }

    /* add header length */
    switch (GET_IN_OP(conn))
    {
        case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
            hdrlen = 4; /* handle(2) + end(2) */
            size_value = 0;
            len = hdrlen;
            break;

        case ATT_OP_READ_BY_TYPE_REQ:
            hdrlen = 2; /* handle(2) */
            end = 0; /* no end handle available */
            break;

        case ATT_OP_READ_BY_GROUP_TYPE_REQ:
            hdrlen = 4; /* handle(2) + end(2) */
            break;

        default:
            return FALSE;
    }

    /* create the response packet */
    if (!conn->out.data)
    {
        /* maximum length is 8 bits, so the attribute may get truncated
         * in case of a massive MTU */
        maxlen = att_pkt_create(
            conn, (CsrUint8)(GET_IN_OP(conn) | ATT_OP_RSP_MASK)) & 0xff;

        if (!len)
        {
            len = CSRMIN(maxlen - 1, hdrlen + size_value);
            write_uint8(&conn->out.data, (CsrUint8)len);
        }
    }

    /* get the maximum data length from the packet */
    if (!len)
    {
        len = conn->out.buf[1];

        /* check length */
        if (len != hdrlen + size_value)
        {
            return 0;
        }
    }

    /* add handle */
    write_uint16(&conn->out.data, handle);

    /* write end handle */
    if (end)
    {
        write_uint16(&conn->out.data, end);
    }

    /* write the data */
    if (irq)
    {
        att_pkt_write(conn, irq->value, (CsrUint8)(len - hdrlen));
    }
    else
    {
        att_attr_read(&conn->out.data, attr, 0, (CsrUint8)(len - hdrlen));
    }

    /* reset pointers */
    *irqp = NULL;
    *attrp = NULL;

    /* room for more? */
    return (conn->mtu - (conn->out.data - conn->out.buf)) >= len;
}

/****************************************************************************
NAME
    att_handle_read_by_common_req - handle ATT requests from client

    This function takes care of the following ATT requests:
    - Find By Type Value request
    - Read By Type request
    - Read By Group Type request
*/
ATT_HANDLER(read_by_common_req)
{
    att_result_t rc = ATT_RESULT_READ_NOT_PERMITTED; /* default error */
    att_attr_t *found = NULL;
    CsrUint16 foundh = 0; /* handle of found attribute */
    att_attr_t *attr;
    CsrUint16 start;
    CsrUint16 end;
    uuid_match_t uuid_match;
    CsrUint32 uuid[4];
    CsrUint16 handle;
    CsrUint16 group_end = 0;
    CsrUint16 uuid_type;
    CsrBool get_group = FALSE;

    switch (GET_IN_OP(conn))
    {
        case ATT_OP_FIND_BY_TYPE_VALUE_REQ:
            uuid_type = ATT_UUID16;
            get_group = TRUE;
            break;


        case ATT_OP_READ_BY_GROUP_TYPE_REQ:
            get_group = TRUE;
            /* fall through */

        default:
            uuid_type = ATT_UUID_NONE; /* determine by length */
    }

    /* read pdu */
    conn->in.data = &conn->in.buf[1]; /* set correct starting point */
    start = read_uint16(&conn->in.data);
    end = read_uint16(&conn->in.data);

    /* read filter condition */
    switch (att_read_uuid(conn, uuid, uuid_type))
    {
        case ATT_UUID16:
            uuid_match = att_attr_match_uuid;
            break;

        case ATT_UUID128:
            uuid_match = att_attr_match_uuid128;
            break;

        default:
            att_send_error_rsp(conn, GET_IN_OP(conn),
                               start, ATT_RESULT_INVALID_PDU);
            return TRUE;
    }

    /* check parameters */
    if (!start || start > end)
    {
        att_send_error_rsp(conn, GET_IN_OP(conn),
                           start, ATT_RESULT_INVALID_HANDLE);
        return TRUE;
    }

    if (irq)
    {
        if (irq->result != ATT_RESULT_SUCCESS)
        {
            att_send_error_rsp(conn, GET_IN_OP(conn),
                               irq->handle, irq->result);
            return TRUE;
        }

        start = irq->handle + 1;
        foundh = irq->handle;
        group_end = 0xffff;

        if (GET_IN_OP(conn) == ATT_OP_FIND_BY_TYPE_VALUE_REQ)
        {
            /* compare the value */
            if (irq->size_value == IN_BUF_LEN(conn) &&
                CsrMemCmp(conn->in.data, irq->value, irq->size_value) == 0)
            {
                found = att_attr_find(ATT_ARG foundh, NULL);
            }
        }
        else
        {
            found = att_attr_find(ATT_ARG foundh, NULL);
        }
    }

    /* traverse through database */
    for (attr = att_attr_find(ATT_ARG start, &handle);
         attr && (handle <= end || found);
         attr = att_attr_next(ATT_ARG attr, &handle))
    {
        if (uuid_match(attr, uuid))
        {
            /* add data */
            if (found)
            {
                /* NOTE: In case of Find By Type Value request, according to
                 * the ATT spec, the end handle shall be same as start handle
                 * if UUID is not grouping attribute defined by a higher layer
                 * specification. We have no way of knowing what future specs
                 * may define so only UUIDs recognised by att_attr_isgroup
                 * are considered as a 'grouping attribute'. */
                if (group_end == 0xffff)
                {
                    if (get_group)
                        group_end = handle - 1;
                    else
                        group_end = foundh;
                }

                if (!add_read_by_common_attr(conn, foundh, group_end,
                                             &found, &irq) ||
                    handle > end)
                {
                    break;
                }
            }

            /* Have a good default error code for matched iteration */
            rc = ATT_RESULT_READ_NOT_PERMITTED;

            if(!att_is_access_permitted(conn, attr))
            {
                /* Request failed due to radio channel checks */
                att_send_error_rsp(conn, GET_IN_OP(conn), handle, ATT_RESULT_APPLICATION_ERROR);
                return TRUE;
            }
            /* is read allowed? */
            else if (conn->cid != ATT_CID_LOCAL && /* always allow local */
                (!(att_attr_perm(ATT_ARG attr) & ATT_PERM_READ) ||
                 ((rc = check_auth(ATT_ARG conn, attr, ATT_PERM_READ)) != ATT_RESULT_SUCCESS)))
            {
                if (conn->out.data) break; /* stop at this attribute */

                /* first attribute, fail the whole request */
                att_send_error_rsp(conn, GET_IN_OP(conn), handle, rc);
                return TRUE;
            }

            /* Attribute is handled by the application */
#ifdef BUILD_FOR_HOST
            /* ACCESS_IND to be sent if IRQ, AUTHORIZATION or ENC_KEY_REQUIREMENTS is set */
            else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)
                 || (attr->flags & ATT_ATTR_AUTHORIZATION)
                 || (attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS))
            {
                CsrUint16 flags = ATT_ACCESS_READ;

                if(attr->flags & ATT_ATTR_AUTHORIZATION)
                    flags |= ATT_ACCESS_PERMISSION;

                if(attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS)
                    flags |= ATT_ACCESS_ENCRYPTION_KEY_LEN;

                /* request data from the application */
                att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                              flags, 0, 0, NULL);
                return FALSE;
            }
#else /* BUILD_FOR_HOST */
            else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R))
            {
                att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                               (CsrUint16)(ATT_ACCESS_READ | ATT_ACCESS_PERMISSION),
                               0, 0, NULL);
                return FALSE;
            }
#endif /* BUILD_FOR_HOST */
            /* add attribute to response */
            else
            {
                if (GET_IN_OP(conn) == ATT_OP_FIND_BY_TYPE_VALUE_REQ)
                {
                    if (!att_attr_isgroup(attr))
                    {
                        get_group = FALSE;
                    }

                    if (att_attr_size(attr) != IN_BUF_LEN(conn) ||
                        !att_attr_match(attr, conn->in.data))
                    {
                        /* attribute does not match, skip to the next */
                        group_end = handle - 1;
                        continue;
                    }
                }

                found = attr;
                foundh = handle;
                group_end = 0xffff;
            }
        }

        /* set the group end unless already set */
        else if (found && group_end == 0xffff &&
                 get_group && att_attr_isgroup(attr))
        {
            group_end = handle - 1;
        }
    }

    /* add remaining data */
    if (found)
    {
        add_read_by_common_attr(conn, foundh, group_end, &found, &irq);
    }

    /* send packet */
    if (conn->out.data)
    {
        att_pkt_send(conn);
    }

    /* send error */
    else
    {
        att_send_error_rsp(conn, GET_IN_OP(conn),
                           start, ATT_RESULT_ATTR_NOT_FOUND);
    }

    return TRUE;
}

/* Read or Read blob request from client */
ATT_HANDLER(read_common_req)
{
    CsrUint16 handle;
    CsrUint16 offs = 0;
    att_result_t rc = ATT_RESULT_INVALID_HANDLE;
    att_result_t auth_rc;
    CsrUint16 len;
    att_attr_t *attr;

    /* create the response packet */
    len = att_pkt_create(conn, (CsrUint8)(GET_IN_OP(conn) | ATT_OP_RSP_MASK));


    conn->in.data = &conn->in.buf[1]; /* set data to point to handle */

    /* read pdu */
    handle = read_uint16(&conn->in.data);

    if (GET_IN_OP(conn) == ATT_OP_READ_BLOB_REQ)
        offs = read_uint16(&conn->in.data);

    /* response from the application */
    if (irq)
    {
        if (irq->result == ATT_RESULT_SUCCESS)
        {
            /* When IRQ_R is not set and Authorization is required,
             * use attribute value from ATT DB.
             * Fix for Bug B-108170.
             */
            attr = att_attr_find(ATT_ARG irq->handle, NULL);
            /* attr value must not be NULL. Shall we panic if NULL? */
            if(attr!=NULL)
            {
                if(
                    (!(attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)))&&
                    (attr->flags & ATT_ATTR_AUTHORIZATION)
                  )
                    /* read attribute value to the output buffer */
                    att_attr_read(&conn->out.data, attr, offs, len);
                else
                    att_pkt_write(conn, irq->value, irq->size_value);

                att_pkt_send(conn);
                return TRUE;
            }
            /* else rc is already set to ATT_RESULT_INVALID_HANDLE */
        }
        else
            rc = irq->result;

        att_send_error_rsp(conn, GET_IN_OP(conn),
                           irq->handle, rc);
        return TRUE;
    }

    /* get the attribute */
    if ((attr = att_attr_find(ATT_ARG handle, NULL)) == NULL)
        rc = ATT_RESULT_INVALID_HANDLE;

    else if(!att_is_access_permitted(conn, attr))
        rc = ATT_RESULT_APPLICATION_ERROR;
    /* is read allowed? */
    else if (conn->cid != ATT_CID_LOCAL && /* always allow local */
             !(att_attr_perm(ATT_ARG attr) & ATT_PERM_READ))
        rc = ATT_RESULT_READ_NOT_PERMITTED;

    else if ((auth_rc = check_auth(ATT_ARG conn, attr, ATT_PERM_READ)) != ATT_RESULT_SUCCESS)
#ifdef BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE
        if(auth_rc == ATT_RESULT_ENCRYPTION_PENDING)
        {

            /* execute the command again after a time gap */
#ifdef ATT_GLOBAL_STATE
            CsrSchedTimerSet(CSR_SCHED_MILLISECOND*TIME_LAPSE_ENCRYPTION_ATT_CMD, att_cmd_after_delay,
                conn->cid, NULL);
#else
            CsrSchedTimerSet(CSR_SCHED_MILLISECOND*TIME_LAPSE_ENCRYPTION_ATT_CMD, att_cmd_after_delay,
                conn->cid, att);
#endif /* ATT_GLOBAL_STATE */
            return FALSE;
        }
        else
#endif /* BUILD_FOR_HOST_FOR_ENCRYPTION_ATT_RACE */
            rc = auth_rc;

    /* we need error code from the application */
#ifdef BUILD_FOR_HOST
    /* ACCESS_IND to be sent if IRQ, AUTHORIZATION or ENC_KEY_REQUIREMENTS is set */
    else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)
         || (attr->flags & ATT_ATTR_AUTHORIZATION)
         || (attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS))
    {
        CsrUint16 flags = ATT_ACCESS_READ;

        if(attr->flags & ATT_ATTR_AUTHORIZATION)
            flags |= ATT_ACCESS_PERMISSION;

        if(attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS)
            flags |= ATT_ACCESS_ENCRYPTION_KEY_LEN;

        /* request data from the application */
        att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                       flags, offs, 0, NULL);
        return FALSE;
    }
#else /* BUILD_FOR_HOST */
    else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R))
    {
        att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                        (CsrUint16)(ATT_ACCESS_READ | ATT_ACCESS_PERMISSION),
                        offs, 0, NULL);
        return FALSE;
    }
#endif /* BUILD_FOR_HOST */

    /* are we reading inside attribute? */
    else if (offs >= att_attr_size(attr))
        rc = ATT_RESULT_INVALID_OFFSET;

    /* send response */
    else
    {
        /* read attribute value to the output buffer */
        att_attr_read(&conn->out.data, attr, offs, len);
        att_pkt_send(conn);
        return TRUE;
    }

    /* if we got so far something went wrong */
    att_send_error_rsp(conn, GET_IN_OP(conn), handle, rc);
    return TRUE;
}

/* Read Multiple request from client */
ATT_HANDLER(read_multi_req)
{
    att_result_t rc = ATT_RESULT_READ_NOT_PERMITTED; /* default error */
    CsrUint16 handle;
    CsrUint16 len;
    CsrUint16 curr = 0xffff; /* current handle */
    CsrUint16 bytesleft;
    att_attr_t *attr = NULL;

    /* read pdu */
    len = IN_BUF_LEN(conn); /* bytes left to be parsed */

    if (len % 2)
    {
        att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ,
                           0, ATT_RESULT_INVALID_PDU);
        return TRUE;
    }

    /* create packet for response */
    bytesleft = att_pkt_create(conn, ATT_OP_READ_MULTI_RSP);

    /* response from the application, add value to the packet before
     * proceeding to the next attribute */
    if (irq)
    {
        /* copy data to the response */
        if (irq->result == ATT_RESULT_SUCCESS)
        {
            /* When IRQ_R is not set and Authorization is required,
             * use attribute value from ATT DB.
             * Fix for Bug B-108170.
             */
            attr = att_attr_find(ATT_ARG (irq->handle), NULL);
            /* attr value must not be NULL. Shall we panic if NULL? */
            if(attr!=NULL)
            {
                if(
                (!(attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)))&&
                (attr->flags & ATT_ATTR_AUTHORIZATION)
                )
                {
                    /* read attribute value to the output buffer */
                    bytesleft -= att_attr_read(&conn->out.data,
                                               attr,
                                               0, /* offs */
                                               bytesleft);
                }
                else
                {
                    bytesleft = att_pkt_write(conn, irq->value, irq->size_value);
                }
            }
            else
            {
                /* invalid handle, just send an error */
                att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ, irq->handle,
                                   ATT_RESULT_INVALID_HANDLE);
                return TRUE;
            }
        }
        /* error, stop now */
        else
        {
            att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ,
                               irq->handle, irq->result);
            return TRUE;
        }

        /* reset response from the app */
        irq = NULL;
    }

    /* move on to the next handle */
    while (len && bytesleft)
    {
        handle = read_uint16(&conn->in.data);
        len -= 2;

        if (handle < curr)
        {
            /* start searching from the beginning */
            attr = att_attr_find(ATT_ARG handle, &curr);
        }

        /* traverse through database */
        for (/* nop */;
             attr && curr <= handle;
             attr = att_attr_next(ATT_ARG attr, &curr))
        {
            if (curr == handle)
            {
                /* Have a good default error code for each iteration */
                rc = ATT_RESULT_READ_NOT_PERMITTED;

                if(!att_is_access_permitted(conn, attr))
                {
                    att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ, handle,
                                       ATT_RESULT_APPLICATION_ERROR);
                    return TRUE;
                }
                /* is read allowed? */
                else if (conn->cid != ATT_CID_LOCAL && /* always allow local */
                    (!(att_attr_perm(ATT_ARG attr) & ATT_PERM_READ) ||
                     ((rc = check_auth(ATT_ARG conn, attr, ATT_PERM_READ)) != ATT_RESULT_SUCCESS)))
                {
                    /* error rsp takes care of possible pending packet */
                    att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ, handle,
                                       rc);
                    return TRUE;
                }

#ifdef BUILD_FOR_HOST
                /* ACCESS_IND to be sent if IRQ, AUTHORIZATION or ENC_KEY_REQUIREMENTS is set */
                else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)
                     || (attr->flags & ATT_ATTR_AUTHORIZATION)
                     || (attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS))
                {
                    CsrUint16 flags = ATT_ACCESS_READ;

                    if(attr->flags & ATT_ATTR_AUTHORIZATION)
                        flags |= ATT_ACCESS_PERMISSION;

                    if(attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS)
                        flags |= ATT_ACCESS_ENCRYPTION_KEY_LEN;

                    /* request data from the application */
                    att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                                  flags, 0, 0, NULL);
                    return FALSE;
                }
#else /* BUILD_FOR_HOST */
                else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R))
                {
                    att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                                   (CsrUint16)(ATT_ACCESS_READ | ATT_ACCESS_PERMISSION),
                                   0, 0, NULL);
                    return FALSE;
                }
#endif /* BUILD_FOR_HOST */
                /* read attribute data directly to output buffer */
                bytesleft -= att_attr_read(&conn->out.data,
                                           attr,
                                           0, /* offs */
                                           bytesleft);
                break;
            }
        }

        if (!attr || handle > curr)
        {
            /* invalid handle, just send an error */
            att_send_error_rsp(conn, ATT_OP_READ_MULTI_REQ, handle,
                               ATT_RESULT_INVALID_HANDLE);
            return TRUE;
        }
    }

    att_pkt_send(conn);

    return TRUE;
}

/****************************************************************************
NAME
    att_handle_write_common_req - handle ATT requests from client

    This function takes care of the following ATT requests:
    - Write request
    - Write command
    - Signed Write command
*/
ATT_HANDLER(write_common_req)
{
    CsrUint16 handle;
    att_attr_t *attr;
    att_result_t rc = ATT_RESULT_SUCCESS;
    att_result_t auth_rc;
    CsrUint16 perm_mask = 0;/* Permission mask is initially set to none */
    CsrUint16 len;
    CsrUint16 op = 0;


    /* if irq, set the correct starting point */
    if (irq )
        conn->in.data = &conn->in.buf[1];

    /* read pdu */
    handle = read_uint16(&conn->in.data);
    len = IN_BUF_LEN(conn);

    /* set the permission requirements */
    if ((op = GET_IN_PDU(conn)) & ATT_OP_FLAGS)
    {
      if(op & ATT_OP_CMD_FLAG)
        perm_mask |= ATT_PERM_WRITE_CMD;

      if (op & ATT_OP_AUTH_FLAG)
         perm_mask |= ATT_PERM_AUTHENTICATED;
    }
    else
        perm_mask = ATT_PERM_WRITE_REQ;

    if (irq)
    {
        rc = irq->result;

        /* Store the value in db only on special cases */
        if(rc == ATT_RESULT_SUCCESS)
        {
           attr = att_attr_find(ATT_ARG handle, NULL);

           if(!attr)
               rc = ATT_RESULT_INVALID_HANDLE;
           else if(((attr->flags & ATT_ATTR_IRQ) ||
                    ((attr->flags & ATT_ATTR_IRQ_W) && /* IRQ_W is set */
                    !(attr->flags & ATT_ATTR_IRQ_R))) || /* IRQ_R is not set */
                /* When IRQ_R is not set and Authorization is required,
                 * use attribute value from ATT DB.
                 * Fix for Bug B-108170.
                 */
                   ((!(attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_R)))&&
                    (attr->flags & ATT_ATTR_AUTHORIZATION))
                  )
               rc = att_attr_set(ATT_ARG &attr, 0 /* offset */, len, conn->in.data);
        }
    }

    else if ((attr = att_attr_find(ATT_ARG handle, NULL)) == NULL)
        rc = ATT_RESULT_INVALID_HANDLE;

    else if(!att_is_access_permitted(conn, attr))
        rc = ATT_RESULT_APPLICATION_ERROR;

    else if (conn->cid != ATT_CID_LOCAL && /* always allow local */
             !(att_attr_perm(ATT_ARG attr) & perm_mask))
        rc = ATT_RESULT_WRITE_NOT_PERMITTED;

    else if ((auth_rc = check_auth(ATT_ARG conn, attr, ATT_PERM_WRITE_REQ)) != ATT_RESULT_SUCCESS)
        rc = auth_rc;

    /* Attribute is handled by the application */
#ifdef BUILD_FOR_HOST
            /* ACCESS_IND to be sent if IRQ, AUTHORIZATION or ENC_KEY_REQUIREMENTS is set */
    else if((attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_W | ATT_ATTR_IRQ_R)
         || (attr->flags & ATT_ATTR_AUTHORIZATION)
         || (attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS))
         && (conn->cid == ATT_CID_LOCAL?(attr->flags & ATT_ATTR_IRQ_R):TRUE))
    {
        CsrUint8 *data = NULL;
        CsrUint16 flags = ATT_ACCESS_WRITE | ATT_ACCESS_WRITE_COMPLETE;

        if(attr->flags & ATT_ATTR_AUTHORIZATION)
            flags |= ATT_ACCESS_PERMISSION;

        if(attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS)
            flags |= ATT_ACCESS_ENCRYPTION_KEY_LEN;

        if (len && (data = xpmalloc(len)) != NULL)
        {
            CsrMemCpy(data, conn->in.data, len);
        }

        att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                       flags, 0 /* offs */, len, data);
        return FALSE;
    }
#else /* BUILD_FOR_HOST */
    else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_W | ATT_ATTR_IRQ_R)
         && (conn->cid == ATT_CID_LOCAL?(attr->flags & ATT_ATTR_IRQ_R):TRUE))
    {
        CsrUint8 *data = NULL;

        if (len && (data = xpmalloc(len)) != NULL)
        {
            CsrMemCpy(data, conn->in.data, len);
        }

        att_access_ind(ATT_STATE(phandle), conn->cid, handle,
                       (CsrUint16)(ATT_ACCESS_WRITE | ATT_ACCESS_PERMISSION
                       | ATT_ACCESS_WRITE_COMPLETE),
                       0 /* offs */, len, data);
        return FALSE;
    }
#endif /* BUILD_FOR_HOST */
    /* store the value */
    else
    {
        rc = att_attr_set(ATT_ARG &attr, 0 /* offset */, len, conn->in.data);
    }

    /* send response */
    if (!(GET_IN_PDU(conn) & ATT_OP_CMD_FLAG))
    {
        if (rc == ATT_RESULT_SUCCESS)
        {
            att_pkt_create(conn, (CsrUint8)(GET_IN_OP(conn) | ATT_OP_RSP_MASK));
            att_pkt_send(conn);
        }
        else
        {
            att_send_error_rsp(conn, GET_IN_OP(conn), handle, rc);
        }
    }

    return TRUE;
}

/****************************************************************************
NAME
    att_handle_prepare_write_req - handle ATT requests from client

    This function takes care of the following ATT requests:
    - Prepare Write request
*/
ATT_HANDLER(prepare_write_req)
{
    CsrUint16 handle;
    att_result_t rc = ATT_RESULT_SUCCESS;
    att_attr_t *attr;
    CsrUint16 len = 0;
    CsrUint16 offs;

    /* if irq, set the correct starting point */
    if (irq )
       conn->in.data = &conn->in.buf[1];

    /* read pdu */
    handle = read_uint16(&conn->in.data);
    offs = read_uint16(&conn->in.data);

    /* if irq do all handling here */
    if (irq)
    {
        /* Store the result of the permissions */
        rc = irq->result;
        if(rc == ATT_RESULT_SUCCESS)
        {
            attr = att_attr_find(ATT_ARG handle, NULL);

            if(!attr)
            {
                rc = ATT_RESULT_INVALID_HANDLE;
            }
            else
            {
                len = IN_BUF_LEN(conn);
                rc = att_wq_add(conn, handle, offs, len, conn->in.data);
            }
        }
    }

    else if ((attr = att_attr_find(ATT_ARG handle, NULL)) == NULL)
        rc = ATT_RESULT_INVALID_HANDLE;

    else if(!att_is_access_permitted(conn, attr))
        rc = ATT_RESULT_APPLICATION_ERROR;

    else if (conn->cid != ATT_CID_LOCAL && /* always allow local */
             !(att_attr_perm_ext(ATT_ARG attr) & ATT_PERM_RELIABLE_WRITE)
#ifdef DO_NOT_HONOR_RELIABLE_PERMISSIONS
             && !(att_attr_perm(ATT_ARG attr) & ATT_PERM_WRITE_REQ)
#endif
             )
        rc = ATT_RESULT_WRITE_NOT_PERMITTED;

    else if ((rc = check_auth(ATT_ARG conn, attr, ATT_PERM_WRITE_REQ)) != ATT_RESULT_SUCCESS)
        /* nop, rc already set */;

#ifdef BUILD_FOR_HOST
    else if((attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_W | ATT_ATTR_IRQ_R)
         || (attr->flags & ATT_ATTR_AUTHORIZATION)
         || (attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS))
         /* Donot send access_ind if already authorized by the app */
         && ((conn->wq == NULL) || (handle != conn->wq->handle)))
    {
        /* Send the WRITE flag so that upper-layers now which operation it is */
        CsrUint16 flags = ATT_ACCESS_WRITE;

        /* Enable permissions flag if authorization required or IRQ's set */
        if(attr->flags & ATT_ATTR_AUTHORIZATION)
            flags |= ATT_ACCESS_PERMISSION;

        if(attr->flags & ATT_ATTR_ENC_KEY_REQUIREMENTS)
            flags |= ATT_ACCESS_ENCRYPTION_KEY_LEN;

        /* Send ind for permissions (authorization,encryption key size) */
        /* Value checking done during access_ind at execute write (no data sent here) */

        att_access_ind(ATT_STATE(phandle), conn->cid,
                       handle, flags, 0, 0, NULL);
        return FALSE;
    }
#else /* BUILD_FOR_HOST */
    else if (attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_W | ATT_ATTR_IRQ_R)
            /* Donot send access_ind if already authorized by the app */
            && ((conn->wq == NULL) || (handle != conn->wq->handle))
           )
    {
        /* Send ind for permissions (authorization and encryption key size) */
        /* Value checking done during access_ind at execute write */
        att_access_ind(ATT_STATE(phandle), conn->cid,
                       handle, ATT_ACCESS_PERMISSION, 0, 0, NULL);
        return FALSE;
    }
#endif /* BUILD_FOR_HOST */
    /* add to write queue */
    else
    {
        len = IN_BUF_LEN(conn);
        rc = att_wq_add(conn, handle, offs, len, conn->in.data);
    }

    if (rc == ATT_RESULT_SUCCESS)
    {
        /* send response */
        att_pkt_create(conn, ATT_OP_PREPARE_WRITE_RSP);
        write_uint16(&conn->out.data, handle);
        write_uint16(&conn->out.data, offs);
        att_pkt_write(conn, conn->in.data, len);
        att_pkt_send(conn);
    }
    else
    {
        /* send error */
        att_send_error_rsp(conn, ATT_OP_PREPARE_WRITE_REQ, handle, rc);
    }

    return TRUE;
}

/* Execute write request from client */
ATT_HANDLER(execute_write_req)
{
    att_result_t rc = ATT_RESULT_SUCCESS;
    att_writeq_t *wq;
    att_attr_t *attr = NULL;
    CsrUint16 handle = 0xffff;
    CsrBool send_irq = FALSE;
    CsrUint16 irq_handle = 0xffff;
    CsrUint8 flags;

    /* if irq, set the correct starting point */
    if (irq )
       conn->in.data = &conn->in.buf[1];

    /* read pdu */
    flags = read_uint8(&conn->in.data);

    switch (flags)
    {
        case ATT_EXECUTE_WRITE:
#ifdef BUILD_FOR_HOST
            if(!irq)
            {
                /* Check the sanity of length of attr values on fresh execute req. */
                /* To be done only once */
                rc = att_check_attr_length_in_wq(ATT_ARG conn);
            }
#endif /* BUILD_FOR_HOST */
            if(irq && ((rc = irq->result) != ATT_RESULT_SUCCESS))
            {
                /* nop : rc already set */
            }
            else
            {
            /* Either irq with success or fresh execute-write */
            for (wq = conn->wq; wq && rc == ATT_RESULT_SUCCESS ; wq = att_wq_next(conn, wq))
            {
                if (wq->handle < handle)
                {
                    /* start searching from the beginning */
                    attr = att_attr_find(ATT_ARG wq->handle, &handle);
                }

                /* traverse through database */
                for (/* nop */;
                     attr && handle <= wq->handle && rc == ATT_RESULT_SUCCESS;
                     attr = att_attr_next(ATT_ARG attr, &handle))
                {
                    if (handle == wq->handle)
                    {
                        if (!irq &&
                               attr->flags & (ATT_ATTR_IRQ | ATT_ATTR_IRQ_W | ATT_ATTR_IRQ_R))
                       {
                            CsrUint8 *data = NULL;
#ifndef BUILD_FOR_HOST
                            /* We allocate 1 extra byte if wq->len is an Odd number */
                            if (wq->len && (data = xpmalloc((wq->len) + (wq->len & 1))) != NULL)
                            {
                                attr_unpack_CsrMemCpy((CsrUint8*)data, (CsrUint16*)&wq->data, wq->len, wq->offs);
                            }
#else
                            if (wq->len && (data = xpmalloc(wq->len)) != NULL)
                            {
                                CsrMemCpy(data, wq->value, wq->len);
                            }
#endif
                            att_access_ind(ATT_STATE(phandle), conn->cid,
                                          handle, ATT_ACCESS_WRITE,
                                           wq->offs, wq->len, data);
                            send_irq = TRUE;
                            irq_handle = handle;
                        }
                        else if (irq && (rc = irq->result) == ATT_RESULT_SUCCESS
                                     && !(attr->flags & ATT_ATTR_IRQ_R)) /* Write to db only if we have ownership */
                        {
                            if((rc = att_attr_exe_set(ATT_ARG &attr, wq)) != ATT_RESULT_SUCCESS)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            /* If we sent an access_ind then send the final one */
            /* Donot write to database or clear the queue till we get a rsp */
            if(send_irq)
            {
                att_access_ind(ATT_STATE(phandle), conn->cid,
                              irq_handle, ATT_ACCESS_WRITE_COMPLETE,
                              0, 0, NULL);
                return FALSE;
            }
            else if(!irq && rc == ATT_RESULT_SUCCESS)
            {
                /* No IRQ set and no access_rsp, write all data into database */
                handle = 0xffff;
                attr = NULL;
                for (wq = conn->wq; wq && rc == ATT_RESULT_SUCCESS ; wq = att_wq_next(conn, wq))
                {
                    if (wq->handle < handle)
                    {
                        /* start searching from the beginning */
                        attr = att_attr_find(ATT_ARG wq->handle, &handle);
                    }
                    for (/* nop */;
                         attr && handle <= wq->handle && rc == ATT_RESULT_SUCCESS;
                         attr = att_attr_next(ATT_ARG attr, &handle))
                    {
                        if (handle == wq->handle)
                        {
                            if((rc = att_attr_exe_set(ATT_ARG &attr, wq)) != ATT_RESULT_SUCCESS)
                            {
                                break;
                            }
                        }
                    }
                }
            }
            }

        /* fall through to free the queue */

        case ATT_EXECUTE_CANCEL:
            att_wq_free(conn);
            break;

        default:
            rc = ATT_RESULT_INVALID_PDU;
    }

    if (rc == ATT_RESULT_SUCCESS)
    {
        /* send response */
        att_pkt_create(conn, ATT_OP_EXECUTE_WRITE_RSP);
        att_pkt_send(conn);
    }
    else
    {
        /* send error - the queue is freed on fall through ATT_EXECUTE_CANCEL */
        att_send_error_rsp(conn, ATT_OP_EXECUTE_WRITE_REQ, handle, rc);
    }

    return TRUE;
}

/* Handle value confirmation from client */
ATT_HANDLER(handle_value_cfm)
{
    /* read pdu */
    /* empty packet */

    att_handle_value_cfm(ATT_STATE(phandle), conn->cid, ATT_RESULT_SUCCESS);

    return TRUE;
}

/*
 * Responses from the application
 ****************************************************************************/
void att_exchange_mtu_rsp(ATT_FUNC_STATE
                          ATT_EXCHANGE_MTU_RSP_T *req)
{
    att_conn_t *conn;

    if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL)
        return;

    /* check that our MTU is sane */
    if (req->server_mtu < ATT_MTU_MIN)
    {
        req->server_mtu = ATT_MTU_MIN;
    }
    else if (req->server_mtu > ATT_MTU_MAX)
    {
        req->server_mtu = ATT_MTU_MAX;
    }

    att_pkt_create(conn, ATT_OP_EXCHANGE_MTU_RSP);
    write_uint16(&conn->out.data, req->server_mtu);
    att_pkt_send(conn);

    /* choose the new mtu */
    conn->mtu = CSRMIN(conn->mtu, req->server_mtu);
}

/*
 * Responses to the client
 ****************************************************************************/
void att_send_error_rsp(att_conn_t *conn, CsrUint8 op, CsrUint16 handle, att_result_t err)
{
    /* destroy outgoing packet */
    if (conn->out.data)
    {
#ifndef BUILD_FOR_ROBINSON
        CsrPmemFree(conn->out.buf);
#endif /* !BUILD_FOR_ROBINSON */
        conn->out.buf = conn->out.data = NULL;
    }

    /* Do not send error if original opcode was a command */
    if (op & ATT_OP_CMD_FLAG)
    {
        return;
    }

    att_pkt_create(conn, ATT_OP_ERROR_RSP);

    write_uint8(&conn->out.data, op);
    write_uint16(&conn->out.data, handle);
    write_uint8(&conn->out.data, (CsrUint8)err);

    att_pkt_send(conn);
}

/*
 * Requests from the application
 ****************************************************************************/
#ifndef BUILD_FOR_ROBINSON
void att_register_req(ATT_FUNC_STATE
                      ATT_REGISTER_REQ_T *req)
{
    att_conn_t *conn;
    att_result_t rc;

    /* TODO: we may want to register initial server for GATT attributes
     * during initialisation - at least have something unless app registers
     * own values */

    /* check if server already exists */
    if (att_conn_find(ATT_ARG ATT_CID_LOCAL) != NULL)
    {
        rc = ATT_RESULT_INVALID_PHANDLE;
    }

    /* add new server */
    else if ((conn = att_conn_add(ATT_ARG ATT_CID_LOCAL)) != NULL)
    {
#ifndef BUILD_FOR_HOST
        conn->mtu = ATT_MTU_BREDR;
#else /* !BUILD_FOR_HOST */
        conn->mtu = 0xfff0; /* Insane local MTU for host, see dg 748690 */
#endif /* BUILD_FOR_HOST */
        ATT_STATE(phandle) = req->phandle;

        rc = ATT_RESULT_SUCCESS;
    }

    /* failed to create server */
    else
    {
        rc = ATT_RESULT_INSUFFICIENT_RESOURCES;
    }

    att_register_cfm(req->phandle, rc);
}

void att_unregister_req(ATT_FUNC_STATE
                        ATT_UNREGISTER_REQ_T *req)
{
    att_result_t rc = ATT_RESULT_INVALID_PHANDLE;

    if (att_conn_find(ATT_ARG ATT_CID_LOCAL) != NULL &&
        ATT_STATE(phandle) == req->phandle)
    {
        att_conn_rm(ATT_ARG ATT_CID_LOCAL);
        ATT_STATE(phandle) = 0;

        /* delete attributes associated with this server */
        att_attr_remove(ATT_ARG ATT_HANDLE_MIN, ATT_HANDLE_MAX);

        rc = ATT_RESULT_SUCCESS;
    }

    att_unregister_cfm(req->phandle, rc);
}
#endif /* !BUILD_FOR_ROBINSON */

#ifndef BUILD_FOR_HOST
void att_add_db_req(ATT_FUNC_STATE
                    ATT_ADD_DB_REQ_T *req)
{
    att_result_t rc;

    /* free existing database if overwite is allowed */
    if (ATT_STATE(attr) && req->flags & ATT_ADD_DB_OVERWRITE)
    {
        att_attr_remove(ATT_ARG ATT_HANDLE_MIN, ATT_HANDLE_MAX);
    }

    rc = att_attr_add(ATT_ARG (att_attr_t**)&req->db, req->size_db);

    att_add_db_cfm(req->phandle, rc);
}
#endif /* !BUILD_FOR_HOST */

#ifdef BUILD_FOR_HOST
void att_add_req(ATT_FUNC_STATE
                 ATT_ADD_REQ_T *req)
{
    att_result_t rc;

    if (req->phandle != ATT_STATE(phandle))
    {
        rc = ATT_RESULT_INVALID_PHANDLE;
    }
    else if (!req->attrs)
    {
        rc = ATT_RESULT_INVALID_DB;
    }
    else
    {
        rc = att_attr_add(ATT_ARG &req->attrs, 0 /* unused */);
    }

    att_add_cfm(req->phandle, rc);
}
#endif /* BUILD_FOR_HOST */

#ifdef BUILD_FOR_HOST
void att_remove_req(ATT_FUNC_STATE
                    ATT_REMOVE_REQ_T *req)
{
    att_result_t rc;
    CsrUint16 num = 0;

    if (req->phandle != ATT_STATE(phandle))
    {
        rc = ATT_RESULT_INVALID_PHANDLE;
    }
    else if (!req->start || req->start > req->end)
    {
        rc = ATT_RESULT_INVALID_HANDLE;
    }
    else
    {
        rc = ATT_RESULT_SUCCESS;
        num = att_attr_remove(ATT_ARG req->start, req->end);
    }

    att_remove_cfm(req->phandle, num, rc);
}
#endif /* BUILD_FOR_HOST */

void att_handle_value_req(ATT_FUNC_STATE
                          ATT_HANDLE_VALUE_REQ_T *req)
{
    att_conn_t *conn;
    CsrUint8 op;
    att_result_t rc = ATT_RESULT_INVALID_CID;

    if ((conn = att_conn_find(ATT_ARG req->cid)) != NULL)
    {
        if (req->flags & ATT_HANDLE_VALUE_INDICATION)
        {
            if (conn->pend & PEND_HANDLE_CFM)
            {
                /* still waiting for confirmation */
                att_handle_value_cfm(req->phandle, req->cid, ATT_RESULT_BUSY);
                return;
            }

            op = ATT_OP_HANDLE_VALUE_IND;
            rc = ATT_RESULT_SUCCESS_SENT;
        }
        else
        {
            op = ATT_OP_HANDLE_VALUE_NOT;
            rc = ATT_RESULT_SUCCESS;
        }

        att_pkt_create(conn, op);
        write_uint16(&conn->out.data, req->handle);
        att_pkt_write(conn, req->value, req->size_value);
        att_pkt_send(conn);
    }

    att_handle_value_cfm(req->phandle, req->cid, rc);
}

void att_access_rsp(ATT_FUNC_STATE
                    ATT_ACCESS_RSP_T *req)
{
    att_conn_t *conn;

    if ((conn = att_conn_find(ATT_ARG req->cid)) == NULL ||
        conn->handler == NULL)
    {
        /* silently ignore */
        return;
    }

    if (conn->handler(ATT_ARG conn, req))
    {
#ifndef BUILD_FOR_ROBINSON
        CsrPmemFree(conn->in.buf);
        conn->in.data = conn->in.buf = NULL;
        conn->in.len = 0;
#endif /* !BUILD_FOR_ROBINSON */
        conn->handler = NULL;
#ifdef BUILD_FOR_HOST
        att_check_pending_queue(ATT_ARG conn);
#endif
    }
}

#ifdef BUILD_FOR_HOST
void att_check_pending_queue(ATT_FUNC_STATE att_conn_t *conn)
{
    att_in_t *p_in = NULL;
    CsrBool retrigger = FALSE;

    do
    {
        if(conn->in.next)
        {
            /* We have something in the list, start processing it */
            p_in = (att_in_t *)conn->in.next;

            /* Copy the data in the node and free it */
            conn->in = *p_in;
            CsrPmemFree(p_in);

            /* Trigger the handler for the queued req/cmd */
            retrigger = att_process_queued_ind(ATT_ARG conn);
        }
    }while(retrigger == TRUE);

    return;
}

CsrBool att_process_queued_ind(ATT_FUNC_STATE att_conn_t *conn)
{
    CsrUint8 op;
    const att_handler_t *pkt = NULL;
    CsrBool pkt_done = TRUE;

    /* handle packet */
    if ((pkt = get_pkt(&op, conn)) != NULL)
    {
        if (conn->pend & PEND_AUTH_CHECK)
        {
            return FALSE;
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

    if (pkt_done)
    {
        CsrPmemFree(conn->in.buf);
        conn->in.data = conn->in.buf = NULL;
        conn->in.len = 0;
        if(conn->in.next)
        {
            /* More stuff in queue, trigger the processing again */
            return TRUE;
        }
    }
    return FALSE;
}
#endif /* BUILD_FOR_HOST */

/*
 * Requests from the client
 ****************************************************************************/

#endif /* INSTALL_ATT_MODULE */
