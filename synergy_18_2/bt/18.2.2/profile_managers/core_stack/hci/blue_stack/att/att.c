/*!
        (c) CSR plc 2010
        All rights reserved

\file   att.c

\brief  Attribute Protocol
*/

#include "csr_synergy.h"
#include "att_private.h"

#ifdef INSTALL_ATT_MODULE

#ifdef ATT_GLOBAL_STATE
ATT_T att;
#endif /* ATT_GLOBAL_STATE */

/* Bluetooth Base UUID 00000000-0000-1000-8000-00805f9b34fb */
/* this could be shared with SDP */
const CsrUint32 att_base_uuid[4] = {
    0xffff0000, /* ~UUID16, this is used as a mask */
    0x00001000,
    0x80000080,
    0x5f9b34fb,
};

#ifndef BUILD_FOR_ROBINSON
/* TODO: do not enable L2CAP channels until ATT_REGISTER_REQ is called. */
void att_init(void **gash)
{
    L2CA_CONFIG_T config;
#ifdef ATT_GLOBAL_STATE
    CsrMemSet(&att, 0, sizeof(ATT_T));
#else /* ATT_GLOBAL_STATE */
    ATT_T *att = CsrPmemAlloc(sizeof(ATT_T));

    *gash = (void*)att;
    CsrMemSet(att, 0, sizeof(ATT_T));
#endif /* !ATT_GLOBAL_STATE */

    /* Register ourselves with L2CAP */
    CsrMemSet(&config, 0, sizeof(L2CA_CONFIG_T));

    config.options = L2CA_SPECIFY_MTU;
    config.mtu = ATT_MTU_MAX;

    L2CA_RegisterFixedCidReq(ATT_IFACEQUEUE,
                             L2CA_CID_ATTRIBUTE_PROTOCOL,
                             &config,
                             0);
#ifdef INSTALL_ATT_BREDR
    L2CA_RegisterReq(ATT_PSM, ATT_IFACEQUEUE,
                     L2CA_MODE_MASK_BASIC,
                     0, 0);
#endif /* INSTALL_ATT_BREDR */
}
#else /* !BUILD_FOR_ROBINSON */
void att_init(void **gash)
{
    CsrUint16 cnt = 0;

    /* Initialise ATT Connection */
    for(; cnt < ATT_MAX_BLE_U_CONNS; ++ cnt)
    {
        ATT_STATE(conn[cnt].cid) = ATT_INVALID_CID;
        ATT_STATE(conn[cnt].pend) = PEND_NONE;
        ATT_STATE(conn[cnt].flags) = 0;
        ATT_STATE(conn[cnt].size_wq) = 0;
#if ATT_TIMEOUT
        ATT_STATE(conn[cnt].timer) = 0;
#endif
    }

    ATT_STATE(attr) = NULL;
    ATT_STATE(size_attr) = 0;

}
#endif /* BUILD_FOR_ROBINSON */

/* Shutdown function for Synergy */
#ifdef ENABLE_SHUTDOWN
void att_deinit(void **gash)
{
#ifndef ATT_GLOBAL_STATE
    ATT_T *att = (ATT_T*)*gash;
#endif /* !ATT_GLOBAL_STATE */

    /* free all existing connections */
    while (ATT_STATE(conn))
    {
        ATT_STATE(conn)->pend = PEND_NONE;
        att_conn_rm(ATT_ARG ATT_STATE(conn)->cid);
    }

    /* free the database */
#ifndef BUILD_FOR_HOST
    CsrPmemFree(ATT_STATE(db));
#else /* !BUILD_FOR_HOST */
    att_attr_remove(ATT_ARG 1, 0xffff);
    /* free the main instance */
#ifndef ATT_GLOBAL_STATE
    CsrPmemFree(att);
#endif
#endif /* BUILD_FOR_HOST */
}
#endif /* ENABLE_SHUTDOWN */

#if ATT_TIMEOUT
#ifndef BUILD_FOR_ROBINSON
/****************************************************************************
NAME
    att_timer_start - start a timer for ATT flow control

*/
void att_timer_start(att_conn_t *conn)
{
    if (conn->timer == 0)
    {
        timer_start(&conn->timer, ATT_TIMEOUT * CSR_SCHED_SECOND,
                    att_timer_expired, 0, conn);
    }
}
#endif /* !BUILD_FOR_ROBINSON */

/****************************************************************************
NAME
    att_timer_expired - timer handler for ATT flow control

*/
#ifndef BUILD_FOR_ROBINSON
void att_timer_expired(CsrUint16 mi, void *mv)
{
    att_conn_t *conn = (att_conn_t*)mv;

    TIMER_EXPIRED(conn->timer);

    /* Just disconnect, and let disconnect ind report error */
    att_disconnect(conn);
}
#else /* !BUILD_FOR_ROBINSON */
void att_timer_expired(timer_id const id)
{
    att_conn_t *conn = NULL;
    CsrUint16 i;

    for (i = 0; i < ATT_MAX_BLE_U_CONNS; i++)
    {
        if (ATT_STATE(conn[i].timer) == id)
        {
            conn = &ATT_STATE(conn[i]);
            break;
        }
    }

    att_disconnect(conn);
}
#endif /* BUILD_FOR_ROBINSON */
#endif /* ATT_TIMEOUT */

/****************************************************************************
NAME
    att_pend_set - set state machine lock and start a timer

RETURN
    TRUE if lock was set
*/
CsrBool att_pend_set(att_conn_t *conn, CsrUint8 op)
{
    /* client -> server PDUs ************************************************/
    /* request */
    if (!(op & ATT_OP_RSP_MASK))
    {
        /* commands do not use flow control */
        if (op & ATT_OP_CMD_FLAG)
        {
            return FALSE;
        }

        /* handle value confirmation is actually not a request */
        else if (op == ATT_OP_HANDLE_VALUE_CFM)
        {
            /* reset handle value indication lock */
            conn->pend &= ~PEND_HANDLE_RSP;

            return FALSE;
        }

        /* set the response we are waiting for */
        PEND_SET_RSP(op | ATT_OP_RSP_MASK);
    }

    /* server -> client PDUs ************************************************/
    /* handle value indication, waiting for confirmation */
    else if (op == ATT_OP_HANDLE_VALUE_IND)
    {
        /* set waiting for confirmation flag */
        conn->pend |= PEND_HANDLE_CFM;
    }

    /* generic response */
    else
    {
        return FALSE;
    }

    if (conn->cid == ATT_CID_LOCAL)
    {
        /* do not use timer for local operations */
        return TRUE;
    }

    /* start timer */
    att_timer_start(conn);

    return TRUE;
}

/****************************************************************************
NAME
    att_pend_check - check if state machine allows the operation

    If operation is not allowed the connection will be disconnected.

RETURN
    TRUE if operation is allowed
*/
CsrBool att_pend_check(att_conn_t *conn, CsrUint8 op)
{
    /* client -> server PDUs ************************************************/
    /* request */
    if (!(op & ATT_OP_RSP_MASK))
    {
        /* commands are always allowed */
        if (op & ATT_OP_CMD_FLAG)
        {
            return TRUE;
        }

        /* handle value confirmation is allowed only after we sent ind */
        else if (op == ATT_OP_HANDLE_VALUE_CFM &&
                 conn->pend & PEND_HANDLE_CFM)
        {
            /* reset waiting flag */
            conn->pend &= ~PEND_HANDLE_CFM;
        }

        /* we are already processing a request */
        else if (op == ATT_OP_HANDLE_VALUE_CFM ||
                 PEND_GET_REQ())
        {
            att_disconnect(conn); /* what else could we possibly do? */
            return FALSE;
        }
    }

    /* server -> client PDUs ************************************************/
    /* response */
    else
    {
        /* response to pending request */
        if (op == PEND_GET_RSP())
        {
            PEND_SET_RSP(PEND_NONE);
        }

        /* handle value indication, allow only one at a time */
        else if (op == ATT_OP_HANDLE_VALUE_IND &&
                 !(conn->pend & PEND_HANDLE_RSP))
        {
            /* set waiting for app response flag */
            conn->pend |= PEND_HANDLE_RSP;
            return TRUE;
        }

        /* handle value notification is always allowed and has no timer */
        else if (op == ATT_OP_HANDLE_VALUE_NOT)
        {
            return TRUE;
        }

        /* error response is checked later based on original opcode */
        else if (op != ATT_OP_ERROR_RSP)
        {
            /* something that we were not supposed to get */
            att_disconnect(conn); /* what else could we possibly do? */
            return FALSE;
        }
    }

    /* if we got this far everything is good */

    if (conn->cid == ATT_CID_LOCAL)
    {
        /* do not use timer for local operations */
        return TRUE;
    }

    /* cancel timer, and start new one if we're still waiting */
    /* This can give infinite timeout for handle value confirmation
     * if client keeps on sending requests, but allows us to get on
     * with a single timer. */
    att_timer_cancel();
    if (PEND_RSP())
    {
        att_timer_start(conn);
    }

    return TRUE;
}

#ifndef BUILD_FOR_ROBINSON
/*! \brief Send ATT primitives to specified queue.
    \param queue_id ID of queue to put primitive on, if bit 15 set primitive is
           put on "to host" queue.
    \param msg_id ID of message.
    \param prim Pointer to primitive.
*/
void att_send_message(CsrSchedQid queue_id, CsrUint16 msg_id, void *prim)
{
#ifndef BUILD_FOR_HOST
    /* On-chip ATT posts upstream messages to hostio (using SDP) */
    if (queue_id & (CsrSchedQid)0x8000)
    {
        CsrSchedMessagePut(SDD_TOHOSTQUEUE, ATT_PRIM, prim);
    }
    else
#endif
    {
        CsrSchedMessagePut(queue_id, msg_id, prim);
    }
}
#endif /* !BUILD_FOR_ROBINSON */

#endif /* INSTALL_ATT_MODULE */
