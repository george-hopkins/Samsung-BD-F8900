/*!
        (c) CSR plc 2010
        All rights reserved

\file   att_interface.c

\brief  Attribute Protocol Application Interface
*/

#include "csr_synergy.h"
#include "dmlib.h"

#include "att_private.h"

#if defined(INSTALL_ATT_MODULE) && !defined(BUILD_FOR_ROBINSON)

void att_dm_sm_data_sign_req(att_conn_t *conn, CsrBool verify,
                             CsrUint16 size_data, CsrUint8 *data)
{

    MAKE_PRIM(DM_SM_DATA_SIGN_REQ);

#ifndef BUILD_FOR_HOST
    L2CA_GetBdAddrFromCid(conn->cid, &prim->addrt);
#else /* !BUILD_FOR_HOST */
    tbdaddr_copy(&prim->addrt, &conn->addrt);
#endif /* BUILD_FOR_HOST */

    prim->phandle = ATT_IFACEQUEUE;
    prim->context = conn->cid;
    prim->verify = verify;
    prim->length = size_data;
    prim->data = data;

    CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM, prim);
}

static void dm_sm_data_sign_cfm(ATT_FUNC_STATE
                                DM_SM_DATA_SIGN_CFM_T *cfm)
{
    att_result_t rc;
    att_conn_t *conn;

    if ((conn = att_conn_find(ATT_ARG cfm->context)) == NULL)
    {
        /* connection already closed, free the buffer */
#ifndef BUILD_FOR_ROBINSON
        CsrPmemFree(cfm->data);
#endif /* !BUILD_FOR_ROBINSON */

        return;
    }

    /* response for checking signature */
    if (cfm->verify == TRUE)
    {
        /* invalid signature */
        if (cfm->status != HCI_SUCCESS)
        {
            conn->pend &= ~PEND_AUTH_CHECK;
            /* ignore the packet as per ATT specification 3.4.5.4 */
            /* Free the memory */
#ifndef BUILD_FOR_ROBINSON
            CsrPmemFree(conn->in.buf);
            conn->in.data = conn->in.buf = NULL;
            conn->in.len = 0;
#endif /* !BUILD_FOR_ROBINSON */
            conn->handler = NULL;
        }
        else
        {
            /* handle packet */
            att_l2ca_dataread_ind(ATT_ARG NULL, conn);
        }
#ifdef BUILD_FOR_HOST
        /* Signing verification failed, trigger de-queue */
        att_check_pending_queue(ATT_ARG conn);
#endif /* BUILD_FOR_HOST */
    }

    /* response for signing data */
    else
    {
        conn->pend &= ~PEND_AUTH_SIGN;

        if (cfm->status != HCI_SUCCESS)
        {
#ifndef BUILD_FOR_ROBINSON
            CsrPmemFree(conn->out.buf);
#endif /* !BUILD_FOR_ROBINSON */

            conn->out.buf = conn->out.data = NULL;
            rc = ATT_RESULT_SIGN_FAILED;
        }
        else
        {
            rc = ATT_RESULT_SUCCESS;
            conn->out.data += ATT_OP_AUTH_LEN;
            att_pkt_send(conn);
        }

        /* inform the app */
        att_write_cfm(ATT_STATE(phandle), conn->cid, rc);
    }
}

static void att_dm_handler(ATT_FUNC_STATE
                           CsrUint16 type, DM_UPRIM_T *prim)
{
    switch (type)
    {
        case DM_SM_DATA_SIGN_CFM:
            dm_sm_data_sign_cfm(ATT_ARG
                                &prim->dm_sm_data_sign_cfm);
            break;

        default:
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
            break;
    }

    dm_free_primitive(prim);
}

static void att_prim_handler(ATT_FUNC_STATE
                             ATT_UPRIM_T *prim)
{
    switch (prim->type)
    {
        /* att_l2cap.c
         ********************************************************************/
        case ATT_CONNECT_REQ:
            att_connect_req(ATT_ARG
                            &prim->att_connect_req);
            break;

        case ATT_DISCONNECT_REQ:
            att_disconnect_req(ATT_ARG
                               &prim->att_disconnect_req);
            break;

        /* att_client.c
         ********************************************************************/
        case ATT_EXCHANGE_MTU_REQ:
            att_exchange_mtu_req(ATT_ARG
                                 &prim->att_exchange_mtu_req);
            break;

        case ATT_FIND_INFO_REQ:
            att_find_info_req(ATT_ARG
                              &prim->att_find_info_req);
            break;

        case ATT_FIND_BY_TYPE_VALUE_REQ:
            att_find_by_type_value_req(ATT_ARG
                                       &prim->att_find_by_type_value_req);
            break;

        case ATT_READ_BY_TYPE_REQ:
            att_read_by_type_req(ATT_ARG
                                 &prim->att_read_by_type_req);
            break;

        case ATT_READ_REQ:
            att_read_req(ATT_ARG
                         &prim->att_read_req);
            break;

        case ATT_READ_BLOB_REQ:
            att_read_blob_req(ATT_ARG
                              &prim->att_read_blob_req);
            break;

        case ATT_READ_MULTI_REQ:
            att_read_multi_req(ATT_ARG
                               &prim->att_read_multi_req);
            break;

        case ATT_READ_BY_GROUP_TYPE_REQ:
            att_read_by_group_type_req(ATT_ARG
                                       &prim->att_read_by_group_type_req);
            break;

        case ATT_WRITE_REQ:
            att_write_req(ATT_ARG
                          &prim->att_write_req);
            break;

        case ATT_PREPARE_WRITE_REQ:
            att_prepare_write_req(ATT_ARG
                                  &prim->att_prepare_write_req);
            break;

        case ATT_EXECUTE_WRITE_REQ:
            att_execute_write_req(ATT_ARG
                                  &prim->att_execute_write_req);
            break;

        case ATT_HANDLE_VALUE_RSP:
            att_handle_value_rsp(ATT_ARG
                                 &prim->att_handle_value_rsp);
            break;

        /* att_server.c
         ********************************************************************/
        case ATT_REGISTER_REQ:
            att_register_req(ATT_ARG
                             &prim->att_register_req);
            break;

        case ATT_UNREGISTER_REQ:
            att_unregister_req(ATT_ARG
                               &prim->att_unregister_req);
            break;

#ifndef BUILD_FOR_HOST
        case ATT_ADD_DB_REQ:
            att_add_db_req(ATT_ARG
                           &prim->att_add_db_req);
            break;

#else /* !BUILD_FOR_HOST */
        case ATT_ADD_REQ:
            att_add_req(ATT_ARG
                        &prim->att_add_req);
            break;

        case ATT_REMOVE_REQ:
            att_remove_req(ATT_ARG
                           &prim->att_remove_req);
            break;
#endif /* BUILD_FOR_HOST */

        case ATT_EXCHANGE_MTU_RSP:
            att_exchange_mtu_rsp(ATT_ARG
                                 &prim->att_exchange_mtu_rsp);
            break;

        case ATT_HANDLE_VALUE_REQ:
            att_handle_value_req(ATT_ARG
                                 &prim->att_handle_value_req);
            break;

        case ATT_ACCESS_RSP:
            att_access_rsp(ATT_ARG
                           &prim->att_access_rsp);
            break;

#ifdef INSTALL_ATT_BREDR
        case ATT_CONNECT_RSP:
            att_connect_rsp(ATT_ARG
                           &prim->att_connect_rsp);
            break;
#endif

        default:
            BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
            break;
    }

    /* at this point primitive can be freed - all special cases are
     * handled in primitive handlers */
    attlib_free(prim);
}

void att_handler(void **gash)
{
#ifndef ATT_GLOBAL_STATE
    ATT_T *att = (ATT_T*)*gash;
#endif
    CsrUint16 type;
    void *msg;

    while (CsrSchedMessageGet(&type, &msg))
    {
        switch (type)
        {
            case DM_PRIM:
                att_dm_handler(ATT_ARG
                               ((DM_UPRIM_T*)msg)->type,
                               (DM_UPRIM_T*)msg);
                break;

            case L2CAP_PRIM:
                att_l2cap_handler(ATT_ARG
                                  ((L2CA_UPRIM_T*)msg)->type,
                                  (L2CA_UPRIM_T*)msg);
                break;

            case ATT_PRIM:
                att_prim_handler(ATT_ARG
                                 (ATT_UPRIM_T*)msg);
                break;

            default:
                BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
                break;
        }
    }

    return;

}

#endif /* INSTALL_ATT_MODULE && !BUILD_FOR_ROBINSON */
