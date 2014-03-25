/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_l2cap_handler.c
\brief  Handles the interface between RFCOMM and L2CAP.
*/

#include "csr_synergy.h"

#ifdef INSTALL_RFCOMM_MODULE

#include "rfcomm_private.h"

/*! Local function prototypes.
*/
static void rfc_handle_l2ca_auto_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_AUTO_CONNECT_CFM_T *p_prim);
static void rfc_handle_l2ca_register_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                         L2CA_REGISTER_CFM_T *p_prim);
static void rfc_handle_l2ca_auto_connect_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_AUTO_CONNECT_IND_T *p_prim);
static void rfc_handle_l2ca_dataread_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                         L2CA_DATAREAD_IND_T *p_prim);
static void rfc_handle_l2ca_disconnect_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                           L2CA_DISCONNECT_IND_T *p_prim);
static void rfc_handle_l2ca_datawrite_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                          L2CA_DATAWRITE_CFM_T *p_prim);

#ifdef INSTALL_AMP_SUPPORT
static void rfc_handle_l2ca_move_channel_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_MOVE_CHANNEL_IND_T *p_prim);
static void rfc_handle_l2ca_move_channel_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_MOVE_CHANNEL_CFM_T *p_prim);
static void rfc_handle_l2ca_move_channel_cmp_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                                 L2CA_MOVE_CHANNEL_CMP_IND_T *p_prim);
#endif

/*! \brief Handler an L2CAP register confirm

    This function processes a L2CA_REGISTER_CFM prim. It should always be
    successful unless something serious has gone wrong!

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_register_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                         L2CA_REGISTER_CFM_T *p_prim)
{
    if(L2CA_MISC_SUCCESS != p_prim->result)
    {
        BLUESTACK_PANIC(
                        CSR_BT_PANIC_RFCOMM_L2CAP_REGISTER_FAILED);
    }

    /* Build and send an RFC_INIT_CFM prim.
    */
    rfc_send_init_cfm(rfc_params->rfc_ctrl->phandle);
}

/*! \brief Handler an L2CAP auto connect confirm

    This function processes a L2CA_AUTO_CONNECT_CFM prim. It first validates
    that the connection matches a known pending Mux . If one is found and the
    connection succeeded and we are the initiator then attempt to actually bring
    up the Mux channel.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_auto_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_AUTO_CONNECT_CFM_T *p_prim)
{
    CsrBool incoming;

    rfc_find_mux_by_bd_addr(rfc_params,
                            &p_prim->bd_addr);


    /* Is this connect cfm from a known device? If its unknown then we just
       ignore the primitive and assume that the rogue L2CAP channel will
       disappear downline.
    */
    if (rfc_params->p_mux != NULL)
    {
        switch (p_prim->result)
        {
            case L2CA_CONNECT_SUCCESS:
            {
                CsrUint16  mode_flag = RFC_CONNFLAG_BASIC;

                rfc_params->p_mux->info.mux.l2cap_mtu = p_prim->config.mtu;

                /* Store the selected l2cap flow mode.
                */
                if (p_prim->config.flow != NULL)
                {
                    if (p_prim->config.flow->mode != L2CA_FLOW_MODE_BASIC)
                    {
                        mode_flag = RFC_CONNFLAG_ERTM;
                    }
                }

                RFC_SET_FLOW_MODE(rfc_params->p_mux->flags, mode_flag);

                /* State transitions always use the p_dlc parameter so for Mux
                   connections copy the pointer.
                */
                rfc_params->p_dlc = rfc_params->p_mux;

                /* Check if this is a confirmation to an outgoing request or an
                   incoming response! No action is required for the latter.
                */
                if (p_prim->con_ctx == RFC_CLIENT)
                {
                    /* Initiate Mux connection. This is an outgoing connection so FALSE will
                       be passed as the 2nd argument (the parameter is 'bool
                       incoming_connection').
                    */
                    incoming = FALSE;
                    crank_channel_fsm(rfc_params, &incoming, RFC_EV_OPEN_REQ_MUX);

                }

                break;
            }

            case L2CA_CONNECT_INITIATING:

                /* Store the cid so that we can tear down the L2cap channel
                   quicker if the host pulls the plug!
                */
                rfc_params->p_mux->info.mux.cid = p_prim->cid;

                /* Now send a connection pending status to the host for each
                   queued dlc channel.
                */
                for (rfc_params->p_dlc = rfc_params->p_mux->info.mux.p_dlcs;
                     rfc_params->p_dlc != NULL;
                     rfc_params->p_dlc = rfc_params->p_dlc->p_next)
                {
                    rfc_send_client_connect_cfm(rfc_params,
                                                rfc_params->p_dlc->phandle,
                                                rfc_params->p_dlc->info.dlc.conn_id,
                                                rfc_params->p_dlc->context,
                                                (RFC_RESPONSE_T)RFC_CONNECTION_PENDING);
                }

                break;

            case L2CA_CONNECT_PENDING:
            case L2CA_CONNECT_RETRYING:
                break;


            default:

                /* Failure to bring up an L2CAP channel is pretty serious!
                   Destroy pending mux and any associated dlc structures created and
                   then notify the host that the connection has failed.
                   Note: If we are the initiator then connect confirm prims will need to
                   be sent to the host for each pending connect request. If we are the
                   responder then we can just die silently as the host wont know about
                   us yet. Note we do not need to destroy the l2cap channel in
                   this case because we dont have one!
                */
                rfc_shutdown_session(rfc_params,
                                     (RFC_RESPONSE_T)p_prim->result);
                break;

        }
    }
    else
    {
        /* An autoconnect success for an unknown Mux is bad! Destroy the l2cap
           connection.
        */
        if (p_prim->result == L2CA_CONNECT_SUCCESS)
        {
            L2CA_DisconnectReq(p_prim->cid);
        }
    }
}


/*! \brief Handler an L2CAP auto connect indication

    This function processes a L2CA_AUTO_CONNECT_IND prim. It first Checks if we
    already have a Mux channel to the specified bluetooth address. If we do then
    the incoming l2cap channel is rejected, if not then we accept the connection
    and create a new Mux structure.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_auto_connect_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_AUTO_CONNECT_IND_T *p_prim)
{
    CsrUint16  conftab_length = RFC_L2CAP_RSP_CONFTAB_SIZE;
    CsrUint16  *conftab = CsrPmemAlloc(RFC_L2CAP_RSP_CONFTAB_SIZE * sizeof(CsrUint16));
    CsrUint8   mode = RFC_L2CAP_RSP_MODE;
    CsrUint8   fallbkmode = RFC_L2CAP_RSP_FALLBKMODE;

    rfc_find_mux_by_bd_addr(rfc_params,
                            &p_prim->bd_addr);

    if (rfc_params->p_mux != NULL)
    {
        /* Error , we already have a Mux to this BDADDR. Reject the incoming
           connection.
        */
        L2CA_AutoConnectRsp(p_prim->identifier,
                            p_prim->cid,
                            L2CA_CONNECT_REJ_PSM,
                            0,
                            0,
                            NULL);
        CsrPmemFree(conftab);
        return;
    }

    /* Send acceptance of l2cap channel. Now initiate the creation of the l2cap
       channel. NB To set the default L2CAP MTU and disallow reconfiguration, we
       need to create a conftab to pass the options down.
    */
    conftab[0] = BKV_SEPARATOR;
    conftab[1] = L2CA_AUTOPT_DISABLE_RECONF;
    conftab[2] = L2CA_RECONF_DISABLE_ALL;
    conftab[3] = L2CA_AUTOPT_MTU_IN;
    conftab[4] = 0xFFFF;              /* Max MTU */
    conftab[5] = L2CA_AUTOPT_FLOW_MODE;
    conftab[6] = (mode << 8) | fallbkmode;
    conftab[7] = L2CA_AUTOPT_FCS;
    conftab[8] = L2CA_FCS_OFF;


    L2CA_AutoConnectRsp(p_prim->identifier,
                        p_prim->cid,
                        L2CA_CONNECT_SUCCESS,
                        RFC_SERVER,
                        conftab_length,
                        conftab);

    /* There is no Mux associated with this BDADDR thus create a new
       structure. Incoming session so we are the responder.
    */
    rfc_mux_new(rfc_params,
                rfc_params->rfc_ctrl->phandle,
                &(p_prim->bd_addr),
                p_prim->local_control,
                FALSE);

    rfc_params->p_mux->info.mux.cid = p_prim->cid;
}


/*! \brief Handler an L2CAP data read indication

    This function processes a L2CA_DATAREAD_IND prim. The primitive is first
    checked to see if the received data was successful. If it was then a search
    by CID is made to try and locate a matching Mux channel. The data portion is
    passed to a matching Mux. Failure to match the a Mux is serious and will
    cause an L2CAP disconnect request for the specified CID to be sent.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_dataread_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                         L2CA_DATAREAD_IND_T *p_prim)
{
    rfc_params->mblk = (CsrMblk *)p_prim->data;

    /* Unlink MBLK from primitive so it doesn't get destroyed automatically */
    p_prim->data = NULL;

    /* Check that L2CAP data was actually received successfully */
    if (p_prim->result == L2CA_DATA_SUCCESS)
    {
        /* Get the Mux channel associated with the CID on which this data
           arrived.
        */
        rfc_get_mux_by_cid(rfc_params, p_prim->cid);

        if (NULL != rfc_params->p_mux)
        {
            /* mblk will be CsrSchedTidied up downline in this case.
            */
            rfc_parse_frame(rfc_params,
                            CsrMblkGetLength(rfc_params->mblk));

            if (p_prim->packets > 0)
            {
                L2CA_DataReadRsp(p_prim->cid, p_prim->packets);
            }

            return;
        }
    }

    /* To get here either a dataread ind with an unsuccessful result or an
       unknown CID has been received, either way destroy the MBLK. We do
       nothing about the rogue L2CAP channel though except ignore it.
    */
    CsrMblkDestroy(rfc_params->mblk);
}

/*! \brief Handler an L2CAP disconnect indication

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_disconnect_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                           L2CA_DISCONNECT_IND_T *p_prim)
{
    RFC_RESPONSE_T  reason = RFC_ABNORMAL_DISCONNECT;

    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        /* Found a matching Mux, thus shutdown the associated RFCOMM session.
           A disconnected l2cap channel for which there is an RFCOMM session
           would be considered an ABNORMAL DISCONNECT at the RFCOMM layer.
           However if the l2cap channel has also abnormally dropped then we will
           report that status instead.
        */
        if (p_prim->reason != L2CA_DISCONNECT_NORMAL)
        {
            reason = (RFC_RESPONSE_T)p_prim->reason;
        }
        rfc_shutdown_session(rfc_params, reason);
    }

    L2CA_DisconnectRsp(p_prim->identifier,
                       p_prim->cid);

    /* The session has been terminated, however it is possible we may
      have got a connect request from the host whilst in the middle of
      taking the Mux down and thus that connect request will have been
      queued. In this case we need to kick everything off again.
      This situation might have arised due to a Local Disconnect and
      a Remote disconnect happening at the same time but local disconnect
      in the CID state machine is lost due to race conditions. Anyway we will
      get a L2CA_DISCONNECT_IND.It is used to trigger any queued request.
    */
    (void)rfc_dequeue_prim(rfc_params, RFC_CLIENT_CONNECT_REQ, 0);
}


/*! \brief Handler an L2CAP data write confirmation

    This function processes a L2CA_DATAWRITE_CFM prim.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_datawrite_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                          L2CA_DATAWRITE_CFM_T *p_prim)
{
    RFC_CHAN_T *p_mux;

    /* Check that the datawrite was successful, if not then this indicates
       terminal failure of the L2CAP link and means we have no choice but to
       issue a disconnect request for that cid. First lookup the cid for which
       the datawrite was received. If it is unknown then we will just ignore
       this prim.
    */
    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        if (p_prim->result == L2CA_DATA_SUCCESS)
        {
            /* Valid cid, check user context to see if this is the confirmation of a raw data
               packet being sent as these are the only ones we care about.
            */
            if (0 != p_prim->req_ctx)
            {
                /* Preserve mux matched by cid as the channel search will
                   overwrite it.
                */
                p_mux = rfc_params->p_mux;
                rfc_find_chan_by_id(rfc_params, p_prim->req_ctx);

                /* If the channel was found then both the p_mux and p_dlc fields will be
                   non NULL. Ensure the channel matched mux matches that matched to
                   the cid.
                */
                if(NULL != rfc_params->p_dlc &&
                   p_mux == rfc_params->p_mux)
                {
                    /* This is a datawrite confirm for a raw data packet on a
                       known channel thus send datawrite confirm to host.
                    */
                    rfc_send_datawrite_cfm(rfc_params->p_dlc->phandle,
                                           rfc_params->p_dlc->info.dlc.conn_id,
                                           RFC_SUCCESS,
                                           rfc_params->rfc_ctrl->use_streams);
                }
            }
        }
        else
        {
            /* Rfcomm assumes it is being used over a reliable channel and thus
               failure to send the L2CAP data is seen as terminal!
            */
            L2CA_DisconnectReq(p_prim->cid);
            rfc_shutdown_session(rfc_params, RFC_ABNORMAL_DISCONNECT);
        }
    }
}


#ifdef INSTALL_AMP_SUPPORT

/*! \brief Handle a L2CA_MOVE_CHANNEL_IND primitive

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_move_channel_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_MOVE_CHANNEL_IND_T *p_prim)
{
    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        rfc_send_l2ca_move_ind(rfc_params->p_mux->phandle,
                               &(rfc_params->p_mux->info.mux.bd_addr),
                               p_prim->identifier,
                               p_prim->local_control);
    }
    else
    {
        /* Receiving an unknown CID should not be possible! However if one is
           received then the primitive will be ignored as this would seem the
           safest course of action!
        */
    }

}

/*! \brief Handle a L2CA_MOVE_CHANNEL_CMP_IND primitive

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_move_channel_cmp_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                                 L2CA_MOVE_CHANNEL_CMP_IND_T *p_prim)
{
    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        rfc_send_l2ca_move_cmp_ind(rfc_params->p_mux->phandle,
                                   &(rfc_params->p_mux->info.mux.bd_addr),
                                   p_prim->local_control,
                                   (RFC_RESPONSE_T)p_prim->result);
    }
    else
    {
        /* Receiving an unknown CID should not be possible! However if one is
           received then the primitive will be ignored as this would seem the
           safest course of action!
        */
    }

}

/*! \brief Handle a L2CA_MOVE_CHANNEL_CFM primitive

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_move_channel_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                             L2CA_MOVE_CHANNEL_CFM_T *p_prim)
{
    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        rfc_send_l2ca_move_cfm(rfc_params->p_mux->phandle,
                               &(rfc_params->p_mux->info.mux.bd_addr),
                               p_prim->local_control,
                               p_prim->remote_control,
                               (RFC_RESPONSE_T)p_prim->result);
    }
    else
    {
        /* Receiving an unknown CID should not be possible! However if one is
           received then the primitive will be ignored as this would seem the
           safest course of action!
        */
    }
}

/*! \brief Handle a L2CA_AMP_LINK_LOSS_IND primitive

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
static void rfc_handle_l2ca_amp_link_loss_ind(RFC_CTRL_PARAMS_T *rfc_params,
                                              L2CA_AMP_LINK_LOSS_IND_T *p_prim)
{
    rfc_get_mux_by_cid(rfc_params, p_prim->cid);

    if (NULL != rfc_params->p_mux)
    {
        rfc_send_l2ca_amp_link_loss_ind(rfc_params->p_mux->phandle,
                                        &(rfc_params->p_mux->info.mux.bd_addr),
                                        p_prim->reason);
    }
    else
    {
        /* Receiving an unknown CID should not be possible! However if one is
           received then the primitive will be ignored as this would seem the
           safest course of action!
        */
    }
}

#endif

/*! \brief Process an L2CAP primitive

    This function processes an l2cap primitive and then frees it.

    \param rfc_params - Pointer to rfcomm instance data.
    \param l2cap_prim - Pointer to received primitive
*/
void rfc_l2cap_handler(RFC_CTRL_PARAMS_T *rfc_params,
                       L2CA_UPRIM_T *l2cap_prim)
{
    switch (l2cap_prim->type)
    {
        case L2CA_REGISTER_CFM:
            rfc_handle_l2ca_register_cfm(rfc_params,
                                         &(l2cap_prim->l2ca_register_cfm));
            break;

        case L2CA_AUTO_CONNECT_CFM:
            rfc_handle_l2ca_auto_connect_cfm(rfc_params,
                                             &l2cap_prim->l2ca_auto_connect_cfm);
            break;

        case L2CA_AUTO_CONNECT_IND:
            rfc_handle_l2ca_auto_connect_ind(rfc_params,
                                             &l2cap_prim->l2ca_auto_connect_ind);
            break;

        case L2CA_DATAWRITE_CFM:
            rfc_handle_l2ca_datawrite_cfm(rfc_params,
                                          &l2cap_prim->l2ca_datawrite_cfm);
            break;

        case L2CA_DATAREAD_IND:
            rfc_handle_l2ca_dataread_ind(rfc_params,
                                         &l2cap_prim->l2ca_dataread_ind);
            break;

        case L2CA_DISCONNECT_IND:
            rfc_handle_l2ca_disconnect_ind(rfc_params,
                                           &l2cap_prim->l2ca_disconnect_ind);
            break;

        case L2CA_DISCONNECT_CFM:
            /* The session has been terminated, however it is possible we may
               have got a connect request from the host whilst in the middle of
               taking the Mux down and thus that connect request will have been
               queued. In this case we need to kick everything off again...
            */
            (void)rfc_dequeue_prim(rfc_params, RFC_CLIENT_CONNECT_REQ, 0);
            break;

        case L2CA_TIMEOUT_IND:
            /* Do nothing.
            */
            break;

#ifdef INSTALL_AMP_SUPPORT
        case L2CA_MOVE_CHANNEL_IND:
            rfc_handle_l2ca_move_channel_ind(rfc_params,
                                             &l2cap_prim->l2ca_move_channel_ind);
            break;

        case L2CA_MOVE_CHANNEL_CMP_IND:
            rfc_handle_l2ca_move_channel_cmp_ind(rfc_params,
                                                 &l2cap_prim->l2ca_move_channel_cmp_ind);
            break;

        case L2CA_MOVE_CHANNEL_CFM:
            rfc_handle_l2ca_move_channel_cfm(rfc_params,
                                             &l2cap_prim->l2ca_move_channel_cfm);
            break;

        case L2CA_AMP_LINK_LOSS_IND:
            rfc_handle_l2ca_amp_link_loss_ind(rfc_params,
                                             &l2cap_prim->l2ca_amp_link_loss_ind);
            break;
#endif
        default:
            break;
    }

    L2CA_FreePrimitive(l2cap_prim);
}




#endif /* INSTALL_RFCOMM_MODULE */
