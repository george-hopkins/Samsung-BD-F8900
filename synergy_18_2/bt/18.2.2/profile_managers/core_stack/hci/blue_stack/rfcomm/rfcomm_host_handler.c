/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_host_handler.c
\brief  Handles the interface between RFCOMM and the host.
*/

#include "csr_synergy.h"

#ifdef INSTALL_RFCOMM_MODULE

#include "rfcomm_private.h"


/*! Create a new RFCOMM primitive.
*/
#define RFC_MAKE_PRIM(TYPE, p_handle)\
    TYPE##_T *prim = zpnew(TYPE##_T);prim->type = (RFC_PRIM_T)(TYPE);prim->phandle = p_handle

/*! Shortcut for sending upstream primitives.
*/
#define RFC_SEND_PRIM(prim) \
    rfc_putmessage((prim)->phandle, RFCOMM_PRIM, prim)


/*! \brief Send RFCOMM primitives to specified queue.
    \param queue_id ID of queue to put primitive on, if bit 15 set primitive is
                    put on "to host" queue.
    \param msg_id ID of message.
    \param prim Pointer to primitive.
*/
static void rfc_putmessage(CsrSchedQid queue_id, CsrUint16 msg_id, void *prim)
{
#ifndef BUILD_FOR_HOST
    /* On-chip RFCOMM posts upstream messages to hostio */
    if (queue_id & (CsrSchedQid)0x8000)
    {
        CsrSchedMessagePut(RFCOMM_TOHOSTQUEUE, IO_DATA_PROTOCOL_RFCOMM, prim);
    }
    else
    {
        CsrSchedMessagePut(queue_id, msg_id, prim);
    }
#else
    CsrSchedMessagePut(queue_id, msg_id, prim);
#endif
}

/*! \brief Handler function for RFCOMM downstream primitives.

    \param rfc_params - Pointer to rfcomm instance data.
    \param rfc_prim - received primitive
*/
void rfc_host_handler(RFC_CTRL_PARAMS_T *rfc_params,
                      RFCOMM_UPRIM_T *rfc_prim)
{
    switch (rfc_prim->type)
    {
        case RFC_INIT_REQ:
            rfc_handle_init_req(rfc_params,
                                &rfc_prim->rfc_init_req);
            break;

        case RFC_REGISTER_REQ:
            rfc_handle_register_req(rfc_params,
                                    &rfc_prim->rfc_register_req);
            break;

        case RFC_UNREGISTER_REQ:
            rfc_handle_unregister_req(rfc_params,
                                      &rfc_prim->rfc_unregister_req);
            break;

        case RFC_CLIENT_CONNECT_REQ:
            rfc_handle_client_connect_req(rfc_params,
                                          &rfc_prim->rfc_client_connect_req);
            break;

        case RFC_SERVER_CONNECT_RSP:
            rfc_handle_server_connect_rsp(rfc_params,
                                          &rfc_prim->rfc_server_connect_rsp);
            break;

        case RFC_DISCONNECT_REQ:
            rfc_handle_disconnect_req(rfc_params,
                                      &rfc_prim->rfc_disconnect_req);
            break;

        case RFC_DISCONNECT_RSP:
            rfc_handle_disconnect_rsp(rfc_params,
                                      &rfc_prim->rfc_disconnect_rsp);
            break;

        case RFC_PORTNEG_REQ:
            rfc_handle_portneg_req(rfc_params,
                                   &rfc_prim->rfc_portneg_req);
            break;

        case RFC_PORTNEG_RSP:
            rfc_handle_portneg_rsp(rfc_params,
                                   &rfc_prim->rfc_portneg_rsp);
            break;

        case RFC_FC_REQ:
            rfc_handle_fc_req(rfc_params,
                              &rfc_prim->rfc_fc_req);
            break;

        case RFC_DATAWRITE_REQ:
            rfc_handle_datawrite_req(rfc_params,
                                     &rfc_prim->rfc_datawrite_req);

            break;

        case RFC_DATAREAD_RSP:
            rfc_handle_dataread_rsp(rfc_params,
                                    rfc_prim->rfc_dataread_rsp.conn_id);
            break;

        case RFC_MODEM_STATUS_REQ:
            rfc_handle_modem_status_req(rfc_params,
                                        &rfc_prim->rfc_modem_status_req);
            break;

        case RFC_LINESTATUS_REQ:
            rfc_handle_linestatus_req(rfc_params,
                                      &rfc_prim->rfc_linestatus_req);
            break;

        case RFC_TEST_REQ:
            rfc_handle_test_req(rfc_params,
                                &rfc_prim->rfc_test_req);
            break;

#ifdef INSTALL_AMP_SUPPORT

        case RFC_L2CA_MOVE_CHANNEL_REQ:
            rfc_handle_l2ca_move_req(rfc_params,
                                     &rfc_prim->rfc_l2ca_move_req);
            break;

        case RFC_L2CA_MOVE_CHANNEL_RSP:
            rfc_handle_l2ca_move_rsp(rfc_params,
                                     &rfc_prim->rfc_l2ca_move_rsp);
            break;
#endif

        default :

            /* Unknown primitive from the host.
            */
            rfc_send_error_ind(rfc_params->rfc_ctrl->phandle,
                               rfc_prim->type,
                               RFC_UNKNOWN_PRIMITIVE);
            break;
    }

    CsrPmemFree(rfc_prim);
}


/*! \brief Send an RFC_INIT_CFM primitive to the host.

    \param phandle - protocol handle used to determine the message destination
*/
void rfc_send_init_cfm(CsrSchedQid phandle)
{
    RFC_MAKE_PRIM(RFC_INIT_CFM, phandle);

    RFC_SEND_PRIM(prim);
}


/*! \brief Send an RFC_REGISTER_CFM primitive.

    This function sends the result of a previous register request to the host.

    \param phandle - protocol handle used to determine the message destination
    \param loc_serv_chan - server channel that has been registered
    \param context - opaque context echoed back from the req prim
    \param accept - TRUE if the register was successful, FALSE o/w
*/
void rfc_send_register_cfm(CsrSchedQid phandle,
                           CsrUint8 loc_serv_chan,
                           CsrUint16 context,
                           CsrBool  accept)
{
    RFC_MAKE_PRIM(RFC_REGISTER_CFM, phandle);

    prim->loc_serv_chan = loc_serv_chan;
    prim->context = context;
    prim->accept = accept;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_UNREGISTER_CFM primitive.

    This function sends the result of a previous unregister request to the host.

    \param phandle - protocol handle used to determine the message destination
    \param loc_serv_chan - server channel that has been unregistered
    \param context - opaque context echoed back from the req prim
    \param status - RFC_SUCCESS or error code

*/
void rfc_send_unregister_cfm(CsrSchedQid phandle,
                             CsrUint8   loc_serv_chan,
                             CsrUint16  context,
                             RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_UNREGISTER_CFM, phandle);

    prim->loc_serv_chan = loc_serv_chan;
    prim->context = context;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_SERVER_CONNECT_IND primitive.

    This function notifies the host of an incoming server channel connect
    request from the peer.

    \param rfc_params - Pointer to rfcomm instance data.
*/
void rfc_send_server_connect_ind(RFC_CTRL_PARAMS_T *rfc_params)
{
    RFC_MAKE_PRIM(RFC_SERVER_CONNECT_IND, rfc_params->p_dlc->phandle);

    bd_addr_copy(&prim->bd_addr, &(rfc_params->p_mux->info.mux.bd_addr));
    prim->loc_serv_chan = RFC_GET_SERV_CHAN_FROM_DLCI(rfc_params->p_dlc->dlci);
    prim->conn_id = rfc_params->p_dlc->info.dlc.conn_id;

    prim->flags = RFC_CONVERT_FLAGS(rfc_params->p_dlc->flags);
    prim->local_l2cap_control =
        rfc_params->p_mux->info.mux.local_l2cap_control;
    prim->context = rfc_params->p_dlc->context;

    RFC_SEND_PRIM(prim);
}


/*! \brief Create a RFC_CONNECT_CFM primitive.

    \param rfc_params - Pointer to rfcomm instance data.
    \param conn_id - connection id of the channel for which a connection has
                     been attempted
    \param context - associated with the connect req/ind
    \param status - RFC_SUCCESS or error code
    \param prim - pointer to the primitive to send

*/
static void rfc_create_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                   CsrUint16 conn_id,
                                   CsrUint16 context,
                                   RFC_RESPONSE_T status,
                                   RFC_CONNECT_CFM_T  *prim)
{
    RFC_DLC_INFO_T  *dlc_info;

    prim->conn_id = conn_id;
    prim->status = status;
    prim->context = context;

    if (NULL != rfc_params->p_dlc)
    {
        bd_addr_copy(&prim->bd_addr, &(rfc_params->p_mux->info.mux.bd_addr));
        prim->serv_chan = RFC_GET_SERV_CHAN_FROM_DLCI(rfc_params->p_dlc->dlci);

        if (RFC_SUCCESS == status)
        {
            dlc_info =&(rfc_params->p_dlc->info.dlc);
            prim->flags = RFC_CONVERT_FLAGS(rfc_params->p_dlc->flags);

            /* Now set the agreed parameters and nullify those that are not applicable.
            */
            prim->max_payload_size = rfc_params->p_dlc->info.dlc.max_frame_size ;
            prim->remote_l2cap_control = dlc_info->p_config->remote_l2cap_control;
            prim->local_l2cap_control = dlc_info->p_config->local_l2cap_control;
        }
    }

}

/*! \brief Send an RFC_SERVER_CONNECT_CFM primitive.

    \param rfc_params - Pointer to rfcomm instance data.
    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which a connection has
                     been attempted
    \param context - associated with the connect req/ind
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_server_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                 CsrSchedQid phandle,
                                 CsrUint16 conn_id,
                                 CsrUint16 context,
                                 RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_SERVER_CONNECT_CFM, phandle);
    rfc_create_connect_cfm(rfc_params,
                           conn_id,
                           context,
                           status,
                           (RFC_CONNECT_CFM_T  *)prim);

    RFC_SEND_PRIM(prim);
}

/*! \brief Queue a RFC_SERVER_CONNECT_CFM primitive.

    \param rfc_params - Pointer to rfcomm instance data.
    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which a connection has
                     been attempted
    \param context - associated with the connect req/ind
    \param status - Error code
*/
void rfc_queue_server_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                  CsrSchedQid phandle,
                                  CsrUint16 conn_id,
                                  CsrUint16 context,
                                  RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_SERVER_CONNECT_CFM, phandle);
    rfc_create_connect_cfm(rfc_params,
                           conn_id,
                           context,
                           status,
                           (RFC_CONNECT_CFM_T  *)prim);

    rfc_queue_prim(rfc_params->rfc_ctrl,
                   (RFCOMM_UPRIM_T *)prim,
                   sizeof(RFC_SERVER_CONNECT_CFM_T));

    /* Primitive queued, free the allocated prim */
    CsrPmemFree(prim);
}

/*! \brief Send an RFC_CLIENT_CONNECT_CFM primitive.

    \param rfc_params - Pointer to rfcomm instance data.
    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which a connection has
                     been attempted
    \param context - associated with the connect req/ind
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_client_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                 CsrSchedQid phandle,
                                 CsrUint16 conn_id,
                                 CsrUint16 context,
                                 RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_CLIENT_CONNECT_CFM, phandle);
    rfc_create_connect_cfm(rfc_params,
                           conn_id,
                           context,
                           status,
                           (RFC_CONNECT_CFM_T  *)prim);

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_DISCONNECT_IND.

    This function notifies the host of a remote disconnection of a specific
    channel.

    \param p_dlc - pointer to the data for the disconnected channel
    \param reason - disconnection code
*/
void rfc_send_disconnect_ind(RFC_CHAN_T     *p_dlc,
                             RFC_RESPONSE_T reason)
{
    RFC_MAKE_PRIM(RFC_DISCONNECT_IND, p_dlc->phandle);
    prim->conn_id = p_dlc->info.dlc.conn_id;
    prim->reason = reason;
    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_DISCONNECT_CFM.

    This function notifies the host of the result of a previous disconnect
    request.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the disconnect req
                     was issued.
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_disconnect_cfm(CsrSchedQid phandle,
                             CsrUint16  conn_id,
                             RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_DISCONNECT_CFM, phandle);
    prim->conn_id = conn_id;
    prim->status = status;
    RFC_SEND_PRIM(prim);
}


/*! \brief Send an RFC_LINESTATUS_IND

     This function notifies the host of the receipt of a linestatus cmd from the
     peer.

    \param rfc_params - Pointer to rfcomm instance data.
    \param error_flag - Received error value
    \param line_status - Received line status value

*/
void rfc_send_line_status_ind(RFC_CTRL_PARAMS_T *rfc_params,
                              CsrUint8            error_flag,
                              RFC_LINE_STATUS_T  line_status)
{
    RFC_MAKE_PRIM(RFC_LINESTATUS_IND, rfc_params->p_dlc->phandle);
    prim->conn_id = rfc_params->p_dlc->info.dlc.conn_id;
    prim->error_flag = error_flag;
    prim->line_status = line_status;
    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_LINESTATUS_CFM.

    This function notifies the host of the result of a previous line status
    request.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the line status req
                     was issued.
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_line_status_cfm(CsrSchedQid      phandle,
                              CsrUint16       conn_id,
                              RFC_RESPONSE_T status)
{
    RFC_MAKE_PRIM(RFC_LINESTATUS_CFM, phandle);
    prim->conn_id = conn_id;
    prim->status = status;
    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_MODEM_STATUS_CFM.

    This function notifies the host of the result of a previous modem status
    request.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the modem status req
                     was issued.
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_msc_cfm(CsrSchedQid phandle,
                      CsrUint16  conn_id,
                      RFC_RESPONSE_T   status)
{
    RFC_MAKE_PRIM(RFC_MODEM_STATUS_CFM, phandle);
    prim->conn_id = conn_id;
    prim->status = status;
    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_MODEM_STATUS_IND

     This function notifies the host of the receipt of a modem status cmd from
     the peer.

    \param rfc_params - Pointer to rfcomm instance data.
    \param modem_signal - Received modem signal value
    \param break_signal - Received break signal value

*/
void rfc_send_msc_ind(RFC_CTRL_PARAMS_T *rfc_params,
                      CsrUint8   modem_signal,
                      CsrUint8   break_signal)
{
    RFC_MAKE_PRIM(RFC_MODEM_STATUS_IND, rfc_params->p_mux->phandle);
    prim->conn_id = rfc_params->p_dlc->info.dlc.conn_id;
    prim->modem_signal = modem_signal;
    prim->break_signal = break_signal;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_TEST_CFM.

    This function notifies the host of the result of a previous test request. It
    is sent on receipt of the test response from the peer. Or immediately if
    there is an error in the request.


    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of the peer
    \param mblk - Pointer to the echoed back test data (from the peer) or NULL
                  on error.
    \param status - RFC_SUCCESS or error code
*/
void rfc_send_test_cfm(CsrSchedQid phandle,
                       BD_ADDR_T *bd_addr,
                       CsrMblk    *mblk,
                       RFC_RESPONSE_T   status)
{
    RFC_MAKE_PRIM(RFC_TEST_CFM, phandle);
    bd_addr_copy(&prim->bd_addr, bd_addr);

    /* If we are on chip then we can leave the data in the mblk and let
       non_hci_convert extract and send it across the wire. In this case we
       set the primitive length to 0 to signify mblk. The mblk will be destroyed
       by non_hci_convert.
    */
    prim->test_data_length = 0;
    prim->test_data = mblk;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send a RFC_FC_CFM

    This function notifies the host of the result of a previous fc request.

    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of the peer
    \param fc - Requested flow control
    \param status - RFC_SUCCESS or error code

*/
void rfc_send_fc_cfm(CsrSchedQid phandle,
                     BD_ADDR_T *bd_addr,
                     RFC_FC_T  fc,
                     RFC_RESPONSE_T   status)
{
    RFC_MAKE_PRIM(RFC_FC_CFM, phandle);
    bd_addr_copy(&prim->bd_addr, bd_addr);

    prim->fc = fc;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send a RFC_FC_IND.

     This function notifies the host of the receipt of a flow control cmd from
     the peer.

    \param rfc_params - Pointer to rfcomm instance data.
    \param fc - Requested flow control

*/
void rfc_send_fc_ind(RFC_CTRL_PARAMS_T *rfc_params,
                     RFC_FC_T  fc)
{
    if (!rfc_params->rfc_ctrl->use_streams)
    {
        RFC_MAKE_PRIM(RFC_FC_IND, rfc_params->p_mux->phandle);
        bd_addr_copy(&prim->bd_addr, &(rfc_params->p_mux->info.mux.bd_addr));

        prim->fc = fc;
        RFC_SEND_PRIM(prim);
    }
}

/*! \brief Send a RFC_PORTNEG_CFM

    This function notifies the host of the result of a previous portneg request.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the modem status req
                     was issued.
    \param rpn - pointer to the port parameters received from the peer or
                 NULL if an error was detected with the request.
    \param status - RFC_SUCCESS or error code

*/
void rfc_send_portneg_cfm(CsrSchedQid phandle,
                          CsrUint16  conn_id,
                          RFC_RPN_VALUES_T  *rpn,
                          RFC_RESPONSE_T   status)
{
    RFC_MAKE_PRIM(RFC_PORTNEG_CFM, phandle);

    prim->conn_id = conn_id;
    prim->status = status;

    if (status == RFC_SUCCESS)
    {
        prim->port_pars = rpn->params;
    }

    RFC_SEND_PRIM(prim);
}

/*! \brief Send a RFC_PORTNEG_IND

    This function notifies the host of incoming portneg request.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the modem status req
                     was issued.
    \param rpn - pointer to the port parameters received from the peer or
                 NULL if an error was detected with the request.

*/
void rfc_send_portneg_ind(CsrSchedQid phandle,
                          CsrUint16  conn_id,
                          RFC_RPN_VALUES_T  *rpn)
{
    RFC_MAKE_PRIM(RFC_PORTNEG_IND, phandle);

    prim->conn_id = conn_id;
    prim->request = rpn->request;

    if (!rpn->request)
    {
        prim->port_pars = rpn->params;
    }

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_DATAWRITE_CFM.

    This function is called from the data packet processing code which could be
    for a stream or datawrite request from the host. To avoid having to check
    for streams all over that code, the check is made here and the cfm only sent
    if it is not a stream.

    \param phandle - protocol handle used to determine the message destination
    \param conn_id - connection id of the channel for which the modem status req
                     was issued.
    \param status - RFC_SUCCESS or error code
    \param is_stream - TRUE if the channel is a stream, in which case no cfm is
                       sent.
*/
void rfc_send_datawrite_cfm(CsrSchedQid phandle,
                            CsrUint16  conn_id,
                            RFC_RESPONSE_T   status,
                            CsrBool is_stream)
{
    if(!is_stream)
    {
        RFC_MAKE_PRIM(RFC_DATAWRITE_CFM, phandle);

        prim->conn_id = conn_id;
        prim->status = status;
        RFC_SEND_PRIM(prim);
    }
}

/*! \brief Send an RFC_DATAREAD_IND.

    This function forwards a raw data packet to the host. Note this function has
    a boolean return parameter in order for it to be used in a function pointer
    variable in the DLC data structure as the notify function depending on
    whether streams are in use or not.

    \param rfc_params - Pointer to rfcomm instance data.
    \param result - Always TRUE for this function.
*/
CsrBool rfc_send_dataread_ind(RFC_CTRL_PARAMS_T *rfc_params)
{
    RFC_MAKE_PRIM(RFC_DATAREAD_IND, rfc_params->p_dlc->phandle);

    /* If we are on chip then we can leave the data in the mblk and let
       non_hci_convert extract and send it across the wire. In this case we
       set the primitive length to 0 to signify mblk. The mblk will be destroyed
       by non_hci_convert.
    */
    prim->payload_length = 0;
    prim->payload = rfc_params->mblk;
    prim->conn_id = rfc_params->p_dlc->info.dlc.conn_id;

    RFC_SEND_PRIM(prim);

    return TRUE;
}

/*! \brief Send an RFC_NSC_IND.

    This function forwards a non supported command message to the host.


    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of remote device.
    \param command_type - non supported command type (low level rfcomm)
    \param cmd_res - command response bit from the unsupported command.

*/
void rfc_send_nsc_ind(CsrSchedQid phandle,
                      BD_ADDR_T  *bd_addr,
                      CsrUint8   command_type,
                      CsrUint8   cmd_res)
{
    RFC_MAKE_PRIM(RFC_NSC_IND, phandle);

    prim->command_type = command_type;
    prim->cmd_res = cmd_res;
    bd_addr_copy(&prim->bd_addr, bd_addr);

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_ERROR_IND.

    This function reports a generic error to the host.

    \param phandle - protocol handle used to determine the message destination
    \param err_prim_type - type of the prim causing the error
    \param status - Actual error

*/
void rfc_send_error_ind(CsrSchedQid phandle,
                        RFC_PRIM_T err_prim_type,
                        RFC_RESPONSE_T  status)
{
    RFC_MAKE_PRIM(RFC_ERROR_IND, phandle);

    prim->err_prim_type = err_prim_type;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC upstream primitive.

    \param p_prim - RFCOMM_UPRIM_T type to be sent to the Host.
    \param p_handle - for determining the queue ID.
*/
void rfc_send_upstream_prim(RFCOMM_UPRIM_T *p_prim, CsrUint16 p_handle)
{
    rfc_putmessage(p_handle, RFCOMM_PRIM, p_prim);
}

#ifdef INSTALL_AMP_SUPPORT

/*! \brief Send an RFC_L2CA_MOVE_CHANNEL_CFM to the host.

    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of remote device
    \param local_control - Local L2CAP controller ID actually in use
    \param remote_control - Remote L2CAP controller ID acutally in use
    \param status  - Result code from L2CAP or a generic rfcomm failure code
*/
void rfc_send_l2ca_move_cfm(CsrSchedQid phandle,
                            BD_ADDR_T *bd_addr,
                            l2ca_controller_t local_control,
                            l2ca_controller_t remote_control,
                            RFC_RESPONSE_T  status)
{
    RFC_MAKE_PRIM(RFC_L2CA_MOVE_CHANNEL_CFM, phandle);

    bd_addr_copy(&prim->bd_addr, bd_addr);
    prim->local_control = local_control;
    prim->remote_control = remote_control;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_L2CA_MOVE_CHANNEL_CMP_IND to the host.

    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of remote device
    \param local_control - Local L2CAP controller ID actually in use
    \param status  - Result code from L2CAP or a generic rfcomm failure code
*/
void rfc_send_l2ca_move_cmp_ind(CsrSchedQid phandle,
                                BD_ADDR_T *bd_addr,
                                l2ca_controller_t local_control,
                                RFC_RESPONSE_T  status)
{
    RFC_MAKE_PRIM(RFC_L2CA_MOVE_CHANNEL_CMP_IND, phandle);

    bd_addr_copy(&prim->bd_addr, bd_addr);
    prim->local_control = local_control;
    prim->status = status;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_L2CA_MOVE_CHANNEL_IND to the host.

    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of remote device
    \param identifier - Used to identify the L2CAP move signal
    \param local_control - Local L2CAP controller ID actually in use
*/
void rfc_send_l2ca_move_ind(CsrSchedQid phandle,
                            BD_ADDR_T *bd_addr,
                            l2ca_identifier_t identifier,
                            l2ca_controller_t local_control)
{
    RFC_MAKE_PRIM(RFC_L2CA_MOVE_CHANNEL_IND, phandle);

    bd_addr_copy(&prim->bd_addr, bd_addr);
    prim->identifier = identifier;
    prim->local_control = local_control;

    RFC_SEND_PRIM(prim);
}

/*! \brief Send an RFC_MOVE_L2CAP_CHANNEL_IND to the host.

    \param phandle - protocol handle used to determine the message destination
    \param bd_addr - Pointer to the bluetooth address of remote device
    \param reason - Error code giving the reason for the AMP link loss
*/
void rfc_send_l2ca_amp_link_loss_ind(CsrSchedQid   phandle,
                                     BD_ADDR_T   *bd_addr,
                                     hci_error_t reason)
{
    RFC_MAKE_PRIM(RFC_L2CA_AMP_LINK_LOSS_IND, phandle);

    bd_addr_copy(&prim->bd_addr, bd_addr);
    prim->reason = (RFC_RESPONSE_T) reason;

    RFC_SEND_PRIM(prim);
}

#endif

#endif /* INSTALL_RFCOMM_MODULE */
