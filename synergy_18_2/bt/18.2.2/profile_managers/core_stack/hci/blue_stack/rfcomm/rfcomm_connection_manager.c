/*!
   Copyright Cambridge Silicon Radio Limited and its licensors 2011. 
     All rights reserved.

\file   rfcomm_connection_manager.c
\brief  Handles the creation and destruction of the mux and data channels.
*/

#include "csr_synergy.h"

#ifdef INSTALL_RFCOMM_MODULE

#include "rfcomm_private.h"
#include "rfcommlib.h"

#include "csr_util.h"

static CsrUint16 rfc_allocate_conn_id(RFC_CTRL_PARAMS_T *rfc_params);
static void rfc_dlc_destroy(RFC_CTRL_PARAMS_T *rfc_params);
static void rfc_mux_destroy(RFC_CTRL_PARAMS_T *rfc_params);
static void rfc_cancel_all_timers(RFC_CHAN_T *chan);

static RFC_CONNECTION_PARAMETERS_T  *rfc_create_local_config(CsrUint16 context,
                                                             CsrUint16 max_frame_size,
                                                             CsrUint8  priority,
                                                             CsrUint16 total_credits,
                                                             l2ca_controller_t remote_l2cap_control,
                                                             l2ca_controller_t local_l2cap_control,
                                                             CsrUint16 client_security_chan,
                                                             CsrUint8 break_signal,
                                                             CsrUint8 modem_signal,
                                                             CsrUint16 msc_timeout);

static CsrMblk  *rfc_create_parneg_cmdrsp(RFC_CTRL_PARAMS_T   *rfc_params,
                                         RFC_LINK_DESIGNATOR from,
                                         RFC_CRTYPE          type,
                                         CsrUint8             init_creds,
                                         CsrUint16            max_frame_size);

static fsm_event_t rfc_update_msc_handshaking_state(RFC_CTRL_PARAMS_T *rfc_params,
                                                    CsrUint16 msc_flag);

static CsrBool rfc_is_incoming_connection(RFC_CHAN_T  *p_dlc);

static CsrUint16 rfc_validate_max_frame_size(RFC_CTRL_PARAMS_T *rfc_params,
                                            CsrUint16 local_mfs,
                                            CsrUint16 peer_mfs);

/*! \brief stream RFC operator table

    This table contain pointers to functions for the various operations that
    can be performed during the data transfer (kick for new data or, a notification).
*/
#ifndef BUILD_FOR_HOST

static const RFC_ACTIONS_VTABLE_T stream_vtable =
{
    rfc_stream_notify,
    rfc_stream_kick
};

#endif

/*! \brief nostream RFC operator table

    This table contain pointers to functions for the various operations that
    can be performed during the data transfer (kick for new data or, a notification)
    without using streams.
*/

static const RFC_ACTIONS_VTABLE_T nostream_vtable =
{
    rfc_send_dataread_ind,
    rfc_process_txdata
};


/*! \brief Create a new MUX structure.

    \param rfc_params - Pointer to rfcomm instance data.
    \param phandle - Protocol handle used to identify the higher
                     entity to which primitives will be sent.
    \param p_bd_addr - Pointer to the bluetooth address of the peer
    \param local_l2cap_control - Amp specific identifier
    \param initiator - True if the Mux is being created by the initiator of the
                       connection , False otherwise
*/
void rfc_mux_new(RFC_CTRL_PARAMS_T *rfc_params,
                 CsrSchedQid         phandle,
                 BD_ADDR_T         *p_bd_addr,
                 l2ca_controller_t local_l2cap_control,
                 CsrBool            initiator)
{
    RFC_CHAN_T *p_mux;

    /* Allocate new mux structure */
    p_mux = zpnew(RFC_CHAN_T);

    /* Initialise mux */
    p_mux->phandle = phandle;
    p_mux->state = RFC_ST_DISCONNECTED;

    bd_addr_copy(&p_mux->info.mux.bd_addr, p_bd_addr);
    p_mux->info.mux.local_l2cap_control = local_l2cap_control;
    p_mux->timers = zpnew(RFC_TIMER_CONTEXTS_T);

    /* The CID will be completed once the L2CAP channel is up.
    */
    p_mux->info.mux.cid = L2CA_CID_INVALID;

    /* Initialise the flow control flags. We assume that CFC will be used and
       CFC ON is 0 so there is no bit to enable.
    */
    RFC_SET_FC_TX_ENABLED(p_mux->flags);
    RFC_SET_FC_RX_ENABLED(p_mux->flags);

    /* Set the initiator bit for the RFCOMM session. If we are the initiator
       then we must always attempt to use credit based flow control.
    */
    if (initiator)
    {
        RFC_SET_INITIATOR(p_mux->flags);
    }

    /* Precalculate the fcs for data frames received on the Mux channel.
    */
    rfc_precalc_fcs_values(p_mux);

    /* Add new mux to head of list */
    p_mux->p_next = rfc_params->rfc_ctrl->mux_list;
    rfc_params->rfc_ctrl->mux_list = p_mux;

    rfc_params->p_mux = p_mux;
}

/*! \brief Lookup a Mux channel using a CID.

    Searches through the list of Mux channels available looking for a matching
    CID. If one is found then a pointer to its data structure is logged in the
    rfcomm instance.

    \param rfc_params - Pointer to rfcomm instance data.
    \param cid - L2CAP channel id to search for.
*/
void rfc_get_mux_by_cid(RFC_CTRL_PARAMS_T *rfc_params,
                        l2ca_cid_t  cid)
{
    RFC_CHAN_T *p_mux;

    for (p_mux = rfc_params->rfc_ctrl->mux_list;
         p_mux != NULL && p_mux->info.mux.cid != cid;
         p_mux = p_mux->p_next)
        ;

    rfc_params->p_mux = p_mux;
}

/*! \brief Find a MUX using a Bluetooth address.

    Searches through the list of Mux channels available looking for a matching
    bluetooth address. If one is found then a pointer to its data structure is
    logged in the rfcomm instance.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_bd_addrr - Pointer to the bluetooth address to search for.
*/
void rfc_find_mux_by_bd_addr(RFC_CTRL_PARAMS_T *rfc_params,
                             BD_ADDR_T *p_bd_addr)
{
    RFC_CHAN_T *p_mux;

    for (p_mux = rfc_params->rfc_ctrl->mux_list;
         p_mux != NULL && !bd_addr_eq(&p_mux->info.mux.bd_addr, p_bd_addr);
         p_mux = p_mux->p_next)
        ;

    rfc_params->p_mux = p_mux;
}


/*! \brief Find a channel using a DLCI.

    Scan all the channels associated with the supplied mux (as logged in the
    rfcomm instance data) for a matching dlci. NB This could match the mux
    channel itself. If a match is found then the dlc data structure is also
    logged in the rfcomm instance.

    \param rfc_params - Pointer to rfcomm instance data.
    \param dlci - dlc channel id to look for
*/
void rfc_find_chan_by_dlci(RFC_CTRL_PARAMS_T *rfc_params,
                           CsrUint8 dlci)
{
    RFC_CHAN_T *p_dlc;

    if (dlci == 0)
    {
        rfc_params->p_dlc = rfc_params->p_mux;
        return;
    }

    for (p_dlc = rfc_params->p_mux->info.mux.p_dlcs;
         p_dlc != NULL;
         p_dlc = p_dlc->p_next)
    {
        if (p_dlc->dlci == dlci)
        {
            break;
        }
    }

    rfc_params->p_dlc = p_dlc;
}


/*! \brief Find a channel using a connection id.

    Scan all the channels associated with the supplied mux (as logged in the
    rfcomm instance data) for a matching connection id. If a match is found then
    both the mux and the dlc data structures are logged in the rfcomm instance.
    If no match is found then the mux and dlc structures in the rfcomm instance
    are set to NULL.

    \param rfc_params - Pointer to rfcomm instance data.
    \param conn_id - connection id to look for
*/
void rfc_find_chan_by_id(RFC_CTRL_PARAMS_T *rfc_params,
                         CsrUint16 conn_id)
{
    RFC_CHAN_T *p_mux;
    RFC_CHAN_T *p_dlc;

    for (p_mux = rfc_params->rfc_ctrl->mux_list;
         p_mux != NULL;
         p_mux = p_mux->p_next)
    {
        /* Check the client channels
        */
        for (p_dlc = p_mux->info.mux.p_dlcs;
             p_dlc != NULL;
             p_dlc = p_dlc->p_next)
        {
            if (p_dlc->info.dlc.conn_id == conn_id)
            {
                rfc_params->p_mux = p_mux;
                rfc_params->p_dlc = p_dlc;
                return;
            }
        }
    }

    rfc_params->p_mux = NULL;
    rfc_params->p_dlc = NULL;
}


/*! \brief Pre-calculates and stores checksums for data frames.

    The checksums for UIH frames are calculated purely on the address and
    control fields which are fixed for a specific DLC. Thus at the point of DLC
    creation the checksum can be calculated once and stored and then just used
    whenever required.

    \param p_chan - Pointer to DLC to calculate checksum for.
*/
void rfc_precalc_fcs_values(RFC_CHAN_T *p_chan)
{
    CsrUint8 data[RFC_UIH_FCS_CALC_SIZE];
    CsrUint8 *data_p = data;
    CsrUint8 address;
    CsrUint8 dirbit;
    RFC_LINK_DESIGNATOR from;

    /* Outgoing UIH/UIH_PF frames
    */
    dirbit = RFC_DIRBIT(p_chan->flags);
    from = (dirbit == 1) ?  RFC_INITIATOR : RFC_RESPONDER;

    address = rfc_create_address_field(p_chan->dlci,
                                       from,
                                       RFC_DATA,
                                       RFC_FRAMELEVEL);

    /* Calculate and store the FCS without credit based flow control.
    */
    write_uint8(&data_p, address);
    write_uint8(&data_p, RFC_UIH);
    p_chan->fcs_out = rfc_frame_crc(data, RFC_UIH_FCS_CALC_SIZE);

    /* Calculate and store the FCS with credit based flow control. The address
       field is the same so this can be stepped over.
    */
    data_p = data;
    data_p ++;
    write_uint8(&data_p, RFC_UIH_PF);
    p_chan->fcs_out |=
        (rfc_frame_crc(data, RFC_UIH_FCS_CALC_SIZE) << 8);

    /* Incoming UIH/UIH_PF frames.
       Inverse of the direction bit.
    */
    dirbit = (~dirbit) & 0x1;
    from = (dirbit == 1) ?  RFC_INITIATOR : RFC_RESPONDER;
    address = rfc_create_address_field(p_chan->dlci,
                                       from,
                                       RFC_DATA,
                                       RFC_FRAMELEVEL);

    /* Calculate and store the FCS without credit based flow control.
    */
    data_p = data;
    write_uint8(&data_p, address);
    write_uint8(&data_p, RFC_UIH);
    p_chan->fcs_in = rfc_frame_crc(data, RFC_UIH_FCS_CALC_SIZE);

    /* Calculate and store the FCS with credit based flow control. The address
       field is the same so this can be stepped over.
    */
    data_p = data;
    data_p ++;
    write_uint8(&data_p, RFC_UIH_PF);
    p_chan->fcs_in |=
        (rfc_frame_crc(data, RFC_UIH_FCS_CALC_SIZE) << 8);

}

/*! \brief Calculates a new , unique connection id.

    \param rfc_params - Pointer to rfcomm instance data.
*/
static CsrUint16 rfc_allocate_conn_id(RFC_CTRL_PARAMS_T *rfc_params)
{
    CsrUint16 conn_id;
    RFC_CHAN_T *p_mux;

    /* The search process can corrupt the p_mux value in rfc_params if a
       matching channel is found and thus we need to preserve this first.
    */
    p_mux = rfc_params->p_mux;

    /* Now pick a new conn id and scan to ensure this new value is not already
       in use. The 'forever' loop should be safe as even assuming we had lots
       and lots of memory available the maximum number of allocated RFCOMM
       channels we could have would be 7 (connections) * 62 channels, which is a
       lot less than the 14 bits of connection id.
    */
    for(;;)
    {
        rfc_params->rfc_ctrl->last_conn_id++;

        /* Preserve conn id marker bits.
        */
        rfc_params->rfc_ctrl->last_conn_id |= RFC_MIN_CONN_ID;
        conn_id = rfc_params->rfc_ctrl->last_conn_id;

        rfc_find_chan_by_id(rfc_params,
                            conn_id);

        if (NULL == rfc_params->p_dlc)
        {
            /* There is no matching DLC with this conn_id thus it is safe to
               use.
            */
            break;
        }
    }

    /* Restore p_mux.
    */
    rfc_params->p_mux = p_mux;

    return conn_id;
}

/*! \brief Create a new DLC structure.

    For outgoing connections the connection parameters will be valid and passed
    down from the connect request. For incoming connections the connection
    parameters will initially be NULL.

    \param rfc_params - Pointer to rfcomm instance data.
    \param phandle - Protocol handle used to identify the higher
                     entity to which primitives will be sent.
    \param dlci - The DLC id for the new connection
    \param p_config - Pointer to connection parameters to be used
    \param initiator - True if the DLC is being created by the initiator of the
                       connection , False otherwise
    \param context - host supplied context value
*/
void rfc_dlc_new(RFC_CTRL_PARAMS_T *rfc_params,
                 CsrUint8 dlci,
                 CsrSchedQid phandle,
                 RFC_CONNECTION_PARAMETERS_T  *p_config,
                 CsrBool   initiator,
                 CsrUint16 context)
{
    RFC_CHAN_T *p_new_dlc;
    RFC_CHAN_T **pp;
    RFC_CHAN_T *p;

    /* Allocate new DLC and config structures */
    p_new_dlc = zpnew(RFC_CHAN_T);
    p_new_dlc->info.dlc.p_config = p_config;

    /* Initialise */
    p_new_dlc->phandle = phandle;
    p_new_dlc->state = RFC_ST_DISCONNECTED;

    p_new_dlc->info.dlc.conn_id = rfc_allocate_conn_id(rfc_params);
    p_new_dlc->dlci = dlci;
    p_new_dlc->context = context;
    p_new_dlc->timers = zpnew(RFC_TIMER_CONTEXTS_T);

    if (initiator)
    {
        RFC_SET_INITIATOR(p_new_dlc->flags);
    }

    /* Precalculate the FCS values for UIH/UIH_PF frames.
    */
    rfc_precalc_fcs_values(p_new_dlc);

#ifndef BUILD_FOR_HOST
    if (rfc_params->rfc_ctrl->use_streams)
    {
        p_new_dlc->info.dlc.vtable = &stream_vtable;
    }
    else
#endif
    {
        p_new_dlc->info.dlc.vtable = &nostream_vtable;
    }

    /* Add new DLC to tail of list */
    for (pp = &(rfc_params->p_mux->info.mux.p_dlcs);
         (p = *pp) != NULL ;
         pp = &(p->p_next) )
    {
        ;
    }

    *pp = p_new_dlc;
    rfc_params->p_dlc = p_new_dlc;
}

/*! \brief Cancel timers.

    \param chan - Pointer to the channel on which to cancel the timers
*/
static void rfc_cancel_all_timers(RFC_CHAN_T *chan)
{
    /* Cancel all timers on the specified channel
    */
    rfc_timer_cancel(&(chan->timers->contexts[RFC_CONNECT_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_DISCONNECT_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_CREDIT_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_RPN_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_MSC_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_RLS_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_TEST_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_MSC_INIT_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_FCON_CONTEXT]));
    rfc_timer_cancel(&(chan->timers->contexts[RFC_FCOFF_CONTEXT]));
}

/*! \brief Destroy a channel structure.

    Removes a channel structure (a MUX or DLC) from the specified channel list
    and then frees the allocated memory.

    \param chan_list - Pointer to the channel list
    \param chan - Pointer to Pointer to the channel to be destroyed
*/
void rfc_channel_destroy(RFC_CHAN_T **chan_list,
                         RFC_CHAN_T **chan)
{
    RFC_CHAN_T *p;
    RFC_CHAN_T **pp;

    /* Search for the channel on the supplied list.
    */
    for (pp = chan_list;
         (p = *pp) != NULL ;
         pp = &(p->p_next) )
    {
        if (p == *chan)
        {
            /* Unhook this channel from the channel list.
            */
            *pp = p->p_next;

            rfc_cancel_all_timers(p);
            CsrPmemFree(p->timers);

            /* Free any allocated data. This would only be on a data channel.
            */
            if (p->dlci != 0)
            {
                CsrPmemFree(p->info.dlc.p_config);
                CsrPmemFree(p->info.dlc.p_temp_pn);
                CsrPmemFree(p->info.dlc.p_temp_rpn);

                if(p->info.dlc.p_data_q != NULL)
                {
                    RFC_QUEUED_DATA_T **pp_rfc_payload = NULL;
                    RFC_QUEUED_DATA_T *p_rfc_payload = NULL;

                    for(pp_rfc_payload = &(p->info.dlc.p_data_q);
                          (p_rfc_payload = *pp_rfc_payload) != NULL;
                         )
                    {
                        /* Unhook the chain */
                        *pp_rfc_payload = p_rfc_payload->p_next;

                        if(p_rfc_payload->payload != NULL)
                        {
                          CsrMblkDestroy (p_rfc_payload->payload);
                        }

                        CsrPmemFree(p_rfc_payload);
                        p_rfc_payload = NULL;
                    }
                    p->info.dlc.p_data_q = NULL;
                }
            }

            CsrPmemFree(p);
            *chan = NULL;
            return;
        }
    }
}

/*! \brief Destroy a Mux structure.

    Removes a MUX structure from the Mux list. Basically wraps the generic
    destroy channel function.

    \param rfc_params - Pointer to rfcomm instance data.
*/
static void rfc_mux_destroy(RFC_CTRL_PARAMS_T *rfc_params)
{
    rfc_channel_destroy(&(rfc_params->rfc_ctrl->mux_list),
                        &(rfc_params->p_mux));
}

/*! \brief Destroy a DLC structure.

    Removes a DLC structure from the DLC list. Basically wraps the generic
    destroy channel function. If this is the last DLC on the list then a MUX
    shutdown timer will be started, if no new connection arrives during that
    time then the Mux will also be destroyed.

    \param rfc_params - Pointer to rfcomm instance data.
*/
static void rfc_dlc_destroy(RFC_CTRL_PARAMS_T *rfc_params)
{
    /* As a special case we check wether the MSC timer is still running
        and there might have a queued MSC request, we dequeue it and
        leave it to be processed by connection manager.
    */
    (void)rfc_dequeue_prim(rfc_params, RFC_MODEM_STATUS_REQ,
                            rfc_params->p_dlc->info.dlc.conn_id);

    rfc_channel_destroy(&(rfc_params->p_mux->info.mux.p_dlcs),
                        &(rfc_params->p_dlc));

    /* If this was the last DLC on the Mux channel then we may need to destroy
       the Mux as well.
    */
    if (NULL == rfc_params->p_mux->info.mux.p_dlcs)
    {
        if (RFC_IS_CHAN_CLOSED(rfc_params->p_mux))
        {
            /* We can only get here if the dlc was cancelled while we were in
               the process of bringing up the l2cap channel. In this instance we
               can destroy the MUX immediately!
            */
            L2CA_DisconnectReq(rfc_params->p_mux->info.mux.cid);
            rfc_mux_destroy(rfc_params);
        }
        else
        {
            /* Update the control parameters to show that the only remaining
               channel is the MUX, ready for when the timer fires.
            */
            rfc_params->p_dlc = rfc_params->p_mux;

            /* Start a Mux shutdown timer.
            */
            rfc_timer_start(rfc_params,
                            &(rfc_params->p_mux->timers->contexts[RFC_DISCONNECT_CONTEXT]),
                            RFC_MUX_SHUTDOWN_TIMER);
        }
    }
}

/*! \brief Terminate a RFCOMM session.

    Removes all DLC structures and the associated Mux as specified in the
    instance data. Closure indications will be sent for all outstanding
    channels.

    \param rfc_params - Pointer to rfcomm instance data.
    \param status - reason for the shutdown
*/
void rfc_shutdown_session(RFC_CTRL_PARAMS_T *rfc_params,
                          RFC_RESPONSE_T status)
{
    RFC_CHAN_T *p_dlc;

    while ((p_dlc = rfc_params->p_mux->info.mux.p_dlcs) != NULL)
    {
        /* Copy the dlc to be destroyed into the current state parameters
           (rfc_params) in order to grind the statemachine.
        */
        rfc_params->p_dlc = p_dlc;
        crank_channel_fsm(rfc_params, &status, RFC_EV_PROTOCOL_DISCONNECT);
    }

    /* Now destroy the MUX.
    */
    rfc_mux_destroy(rfc_params);
}


/*! \brief Handle a Mux shutdown timer event.

    This timer will have been started when the last DLC was destroyed. When it
    fires the Mux is also destroyed.

    \param arg2 (rfc_params) - Pointer to rfcomm instance data.
*/
void rfc_mux_shutdown_timer_event(CsrUint16 arg1, void *arg2)
{
    RFC_CTRL_PARAMS_T *shutdown_context = (RFC_CTRL_PARAMS_T *) arg2;
    RFC_CTRL_PARAMS_T rfc_params;
    PARAM_UNUSED(arg1);

    rfc_timer_expired(shutdown_context,
                      &rfc_params);

    crank_channel_fsm(&rfc_params, NULL, RFC_EV_DISCONNECT_REQ);

}

/*! \brief Handle a DM_SM_ACCESS_CFM primitive

     Function to check the response from the DM security manager to
     a request to allow a new data channel to be opened.

    \param rfc_params - Pointer to rfcomm instance data.
    \param access_cfm - Pointer to the received DM response
*/
void rfc_check_sm_response(RFC_CTRL_PARAMS_T *rfc_params,
                           DM_SM_ACCESS_CFM_T *access_cfm)
{
    /* Get the channel that has been security checked. The context field is
       used to bounce the connection id down to the SM and back.
    */
    rfc_find_chan_by_id(rfc_params,
                        (CsrUint16)access_cfm->context);

    /* Known connection ?
    */
    if (NULL != rfc_params->p_dlc)
    {
        /* Security has either been granted or refused.
        */
        if (HCI_SUCCESS == access_cfm->status)
        {
            crank_channel_fsm(rfc_params,
                              &(access_cfm->conn_setup.incoming),
                              RFC_EV_ACCESS_CONFIRM);
        }
        else
        {
            crank_channel_fsm(rfc_params,
                              &(access_cfm->conn_setup.incoming),
                              RFC_EV_ACCESS_DECLINE);
        }

    }
    else
    {
        /* The context echoed back from the SM (containing the connection
           id) is now not recognised! This indicates corruption somewhere in
           the code or that a link has been destroyed while waiting for a
           security response. This could mean that the channel has been
           destroyed in the meantime so just ignore the prim.
        */
    }

}

/*! \brief Create the local connect config

     Function to create a new connection config structure and populate it from
     the supplied connection parameters.

    \param context - opaque , user supplied context value
    \param max_frame_size - requested max frame size
    \param priority - requested priority
    \param total_credits - total number of rx credits to provide to the peer
    \param remote_l2cap_control - l2cap specific value
    \param local_l2cap_control - l2cap specific value
    \param client_security_chan - Client chan id used for validating security
                                  access
    \param break_signal - default value for break signal in MSC
    \param modem_signal - default value for modem signal in MSC
    \param msc_timeout - user specified timeout for MSC initial handshake
    \param p_config - (Returned) Pointer to the new config
*/
static RFC_CONNECTION_PARAMETERS_T  *rfc_create_local_config(CsrUint16 context,
                                                             CsrUint16 max_frame_size,
                                                             CsrUint8  priority,
                                                             CsrUint16 total_credits,
                                                             l2ca_controller_t remote_l2cap_control,
                                                             l2ca_controller_t local_l2cap_control,
                                                             CsrUint16 client_security_chan,
                                                             CsrUint8 break_signal,
                                                             CsrUint8 modem_signal,
                                                             CsrUint16 msc_timeout)
{
    RFC_CONNECTION_PARAMETERS_T  *p_config = pnew(RFC_CONNECTION_PARAMETERS_T);

    /* Set up the config.
    */
    p_config->max_frame_size = max_frame_size;
    p_config->priority = priority;
    p_config->total_credits = total_credits;
    p_config->remote_l2cap_control = remote_l2cap_control;
    p_config->local_l2cap_control = local_l2cap_control;
    p_config->client_security_chan = client_security_chan;
    p_config->break_signal = break_signal;
    p_config->modem_signal = modem_signal;
    p_config->msc_timeout = msc_timeout;

    return p_config;
}

/*! \brief Queue a host sourced primitive

     Function to queue a primitive from the host when it cant be processed at
     the current time. A copy of the prim is taken and queued as the normal path
     through the code causes an incoming prim to be freed as soon as its been
     processed and doesnt allow for delayed processing.

    \param rfc_ctrl - Pointer to rfcomm control data.
    \param p_prim - Pointer to the received primitive.
    \param len - length of the primitive to be queued.
*/
void rfc_queue_prim(RFC_CTRL_T *rfc_ctrl,
                    RFCOMM_UPRIM_T *p_prim,
                    CsrUint16 len)
{
    RFC_PRIMQ_T  *p;
    RFC_PRIMQ_T  **pp;
    RFC_PRIMQ_T  *p_primq = pnew(RFC_PRIMQ_T);
    RFCOMM_UPRIM_T *p_prim_copy = pcopy(p_prim, len);

    /* Find the end of the queued prim list.
    */
    for (pp = &(rfc_ctrl->prim_queue);
         (p = *pp) != NULL ;
         pp = &(p->p_next) )
    {
        ;
    }

    p_primq->prim = p_prim_copy;
    p_primq->p_next = NULL;

    *pp = p_primq;
}

/*! \brief dequeue the primitive and send it to RFCOMM queue

     Function will dequeue the type primitive if any and send it
     to RFCOMM downstream queue.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - RFCOMM primitive type
*/

CsrBool rfc_dequeue_prim(RFC_CTRL_PARAMS_T *rfc_params,
                          RFC_PRIM_T type, CsrUint16 conn_id)
{
    RFCOMM_UPRIM_T *p_prim;
    RFC_PRIMQ_T  *p;
    RFC_PRIMQ_T  **pp;
    CsrUint16 id = 0;
    CsrBool is_upstream = FALSE;

    /* Iterate through the queued prim list
         to find the desired prim.
    */
    for (pp = &(rfc_params->rfc_ctrl->prim_queue);
         (p = *pp) != NULL ; pp = &(p->p_next))
    {
      if(p->prim->type == type)
      {
         CsrSchedQid p_handle = 0;

         p_prim = (RFCOMM_UPRIM_T *)p->prim;

         switch(type)
         {
             case RFC_MODEM_STATUS_REQ:
             {
                 id = p_prim->rfc_modem_status_req.conn_id;
             }
             break;

             case RFC_CLIENT_CONNECT_REQ:
             {
                 id = 0;
             }
             break;

             case RFC_SERVER_CONNECT_CFM:
             {
                 id = p_prim->rfc_server_connect_cfm.conn_id;
                 is_upstream = TRUE;
                 p_handle = p_prim->rfc_server_connect_cfm.phandle;
             }
             break;

             default:
                break;
         }

         if(id == conn_id)
         {
             *pp = p->p_next;

             if(is_upstream)
             {
                 /* Post the primitive back to the Host interface queue.
                 */
                 rfc_send_upstream_prim(p_prim, p_handle);
             }
             else
             {
                 /* Post the primitive back to the RFCOMM interface queue.
                 */
                 CsrSchedMessagePut(RFCOMM_IFACEQUEUE, RFCOMM_PRIM, p_prim);
             }
             CsrPmemFree(p);
             p= NULL;
             return TRUE;
         }
      }
    }
    return FALSE;
}

/*! \brief flush any pending prim queue elements

     Function to check if there are any primitives stored on the prim queue and
     if there are then if requeue flag is TRUE post them all back to the rfcomm
     interface queue so that they will then get processed as if they had just
     come in from the host. If the flag is FALSE then the primitives are just
     deleted.
     Currently handled primitives -
     1) Client Connect Req
     2) Modem status Req

    \param rfc_params - Pointer to rfcomm instance data.
    \param requeue - TRUE if the prims are to be posted back on the
                     RFCOMM_IFACEQUEUE , FALSE otherwise
*/
void rfc_flush_prim_queue(RFC_CTRL_PARAMS_T *rfc_params)
{
    RFCOMM_UPRIM_T *p_prim;
    RFC_PRIMQ_T  *p1;
    RFC_PRIMQ_T  *p2;

    for (p1 = rfc_params->rfc_ctrl->prim_queue;
         p1 != NULL ; p1 = p2)
    {
        p2 = p1->p_next;

        p_prim = (RFCOMM_UPRIM_T *)p1->prim;

        /* Destroy the prim.
        */
        rfc_free_primitive(p_prim);

        CsrPmemFree(p1);
    }

    rfc_params->rfc_ctrl->prim_queue = NULL;
}

/*! \brief Handle the receipt of an RFC_CLIENT_CONNECT_REQ primitive

     Function to process a new connection request from the upper layers.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received RFC connect request
*/
void rfc_handle_client_connect_req(RFC_CTRL_PARAMS_T *rfc_params,
                                   RFC_CLIENT_CONNECT_REQ_T *p_prim)
{
    RFC_CONNECTION_PARAMETERS_T  *p_config;
    CsrBool initiator;
    CsrUint8 dirbit = 1; /* Assume we are the initiator. */
    CsrUint8  break_signal = p_prim->break_signal;
    CsrUint16 max_frame_size = p_prim->max_payload_size;

    /* The validate msc params function checks a flags field to see if credit
       based flow control is active. However as the CFC flag is 0 = ON we can
       just pass 0 to the function.
    */
    /* First sanity check some connect parameters.
    */
    if(!rfc_validate_msc_params(0,
                                p_prim->modem_signal,
                                &break_signal) ||
       (max_frame_size  + RFC_MAX_DATA_HEADER_SIZE ) < RFC_MAX_NON_DATA_FRAME_LEN)
    {
        rfc_send_client_connect_cfm(rfc_params,
                                    p_prim->phandle,
                                    0,
                                    p_prim->context,
                                    (RFC_RESPONSE_T)RFC_INCONSISTENT_PARAMETERS);

        return;
    }

    /* NOTE for outgoing connections Credit based flow control will always be
       set, the MSC FC flag not used and thus there is no need to call the
       rfc_update_msc_fc_status() fn after validation of the MSC parameters.
    */

    /* Only attempt to bring up a DLC immediately if we already have a MUX
       channel up and running.
    */
    rfc_find_mux_by_bd_addr(rfc_params, &p_prim->bd_addr);
    if (rfc_params->p_mux == NULL)
    {
        CsrUint16  conftab_length = RFC_L2CAP_REQ_CONFTAB_SIZE;
        CsrUint16  *conftab = CsrPmemAlloc(RFC_L2CAP_REQ_CONFTAB_SIZE * sizeof(CsrUint16));
        CsrUint8   mode = L2CA_FLOW_MODE_BASIC;
        CsrUint8   fallbkmode = 0;

        /* Outgoing session so we are the initiator */
        initiator = TRUE;

        rfc_mux_new(rfc_params,
                    p_prim->phandle,
                    &(p_prim->bd_addr),
                    p_prim->local_l2cap_control,
                    initiator);

        /* As we dont yet know the l2cap mtu at this point use the maximum
           possible value so as not to limit the supplied max payload size.
        */
        p_config = rfc_create_local_config(p_prim->context,
                                           max_frame_size,
                                           p_prim->priority,
                                           p_prim->total_credits,
                                           p_prim->remote_l2cap_control,
                                           p_prim->local_l2cap_control,
                                           p_prim->client_security_chan,
                                           break_signal,
                                           p_prim->modem_signal,
                                           p_prim->msc_timeout);

        /* Also create the dlc structure. This will sit on the Mux dlc list in
           the DISCONNECTED state until the Mux is up. Again direction bit will
           be 1 as we are the initiator in this case.
        */
        rfc_dlc_new(rfc_params,
                    (CsrUint8)RFC_SET_DLCI(dirbit, p_prim->rem_serv_chan),
                    p_prim->phandle,
                    p_config,
                    initiator,
                    p_prim->context);

#ifndef BUILD_FOR_HOST
        /* If this is a stream connection then we need to notify the stream
           subsystem. If the stream make fails then this is pretty serious so
           we panic!!
        */
        if (rfc_params->rfc_ctrl->use_streams &&
            NULL == stream_rfcomm_make(rfc_params->p_dlc->info.dlc.conn_id,
                                       rfc_params->rfc_ctrl))
        {
            BLUESTACK_PANIC(
                            PANIC_RFCOMM_STREAM_MISMATCH);
        }

#else
        /* If we are off chip then check the flow mode flags and set up the
           config modes accordingly. If on chip we force it to basic mode.
        */
        if (p_prim->remote_l2cap_control == L2CA_AMP_CONTROLLER_BT &&
            p_prim->local_l2cap_control == L2CA_AMP_CONTROLLER_BT)
        {
            if (RFC_IS_ERTM_MODE(p_prim->flags))
            {
                mode = L2CA_FLOW_MODE_ENHANCED_RETRANS;
            }

            if (RFC_IS_PREFERRED_MODE(p_prim->flags))
            {
                if (mode == L2CA_FLOW_MODE_ENHANCED_RETRANS)
                {
                    fallbkmode = L2CA_MODE_MASK_BASIC;
                }
            }
        }
#endif

        /* Now initiate the creation of the l2cap channel. NB passing down a CID
           of 0 creates a new autoconnect rather than renegotiating and existing
           one. To set the default L2CAP MTU and disallow reconfiguration, we
           need to create a conftab to pass the options down.
        */
        conftab[0] = BKV_SEPARATOR;
        conftab[1] = L2CA_AUTOPT_DISABLE_RECONF;
        conftab[2] = L2CA_RECONF_DISABLE_ALL;
        conftab[3] = L2CA_AUTOPT_MTU_IN;
        conftab[4] = 0xFFFF;              /* Max MTU */
        conftab[5] = L2CA_AUTOPT_SECURITY_PROTOCOL;
        /* Tunnel our security requirements through L2CAP. We set a special
           flag in the protocol ID which gets the security manager to look
           only at the SSP-related requirements. This is because SM4 requires
           encryption before the L2CAP channel comes up and we want to
           generate the right type of key first time. SM2 has no such
           requirements, so we'll wait until RFCOMM channel setup where
           pairing is less likely to be interupted by idle timeout on the
           remote device. */
        conftab[6] = SEC_PROTOCOL_RFCOMM | SEC_PROTOCOL_MODIFY_SSP_ONLY;
        conftab[7] = L2CA_AUTOPT_SECURITY_CHANNEL;
        conftab[8] = p_prim->client_security_chan;
        conftab[9] = L2CA_AUTOPT_FLOW_MODE;
        conftab[10] = (mode << 8) | fallbkmode;
        conftab[11] = L2CA_AUTOPT_FCS;
        conftab[12] = L2CA_FCS_OFF;

        L2CA_AutoConnectReq(0,
                            RFCOMM_PSM,
                            &p_prim->bd_addr,
                            RFCOMM_PSM,
                            RFC_CLIENT,
                            p_prim->remote_l2cap_control,
                            p_prim->local_l2cap_control,
                            conftab_length,
                            conftab);

    }
    else
    {
        CsrUint8 dlci;
        RFC_CHAN_T  *p_mux;

        /* Check if the session is currently on the way down or not. This can
           only happen if a DISC command has been sent across the air for the
           Mux and a new connect request has arrived before the UA has been
           received.
        */
        if (!RFC_IS_DISCONNECTING(rfc_params->p_mux->flags))
        {
            /* Determine whether we are the initiator of the RFCOMM session or not.
            */
            initiator = RFC_IS_INITIATOR(rfc_params->p_mux->flags);
            if (!initiator)
            {
                dirbit = 0;
            }

            dlci = (CsrUint8) RFC_SET_DLCI(dirbit, p_prim->rem_serv_chan);

            /* Check that the requested dlci doesnt already exist. The channel
               search is destructive to the rfc_params->p_mux field thus this needs
               to be preserved before searching.
            */
            p_mux = rfc_params->p_mux;

            rfc_find_chan_by_dlci(rfc_params, dlci);
            if(NULL != rfc_params->p_dlc)
            {
                /* Reject the incoming request.
                */
                rfc_send_client_connect_cfm(rfc_params,
                                            p_prim->phandle,
                                            0,
                                            p_prim->context,
                                            (RFC_RESPONSE_T)RFC_CHANNEL_ALREADY_EXISTS);
                return;
            }

            /* Restore session details.
            */
            rfc_params->p_mux = p_mux;
            rfc_params->p_dlc = NULL;

            /* Cancel any mux shutdown timer running.
            */
            rfc_timer_cancel(&(rfc_params->p_mux->timers->contexts[RFC_DISCONNECT_CONTEXT]));

            /* Create and initialise a new dlc structure.
            */
            p_config = rfc_create_local_config(p_prim->context,
                                               max_frame_size,
                                               p_prim->priority,
                                               p_prim->total_credits,
                                               p_prim->remote_l2cap_control,
                                               p_prim->local_l2cap_control,
                                               p_prim->client_security_chan,
                                               p_prim->break_signal,
                                               p_prim->modem_signal,
                                               p_prim->msc_timeout);

            rfc_dlc_new(rfc_params,
                        dlci,
                        p_prim->phandle,
                        p_config,
                        initiator,
                        p_prim->context);

#ifndef BUILD_FOR_HOST
            /* If this is a stream connection then we need to notify the stream
               subsystem. If the stream make fails then this is pretty serious
               so we panic!!
            */
            if (rfc_params->rfc_ctrl->use_streams &&
                NULL == stream_rfcomm_make(rfc_params->p_dlc->info.dlc.conn_id,
                                           rfc_params->rfc_ctrl))
            {
                BLUESTACK_PANIC(
                                PANIC_RFCOMM_STREAM_MISMATCH);
            }
#endif
            /* Can we send the PENDING status now? We can only do that if we
               have a valid l2cap cid at this point.
            */
            if (L2CA_CID_INVALID != rfc_params->p_mux->info.mux.cid)
            {
                rfc_send_client_connect_cfm(rfc_params,
                                            rfc_params->p_dlc->phandle,
                                            rfc_params->p_dlc->info.dlc.conn_id,
                                            p_prim->context,
                                            (RFC_RESPONSE_T)RFC_CONNECTION_PENDING);
            }

            /* Only start the parameter negotiation phase of data channel creation
               if the mux is already fully connected.
            */
            if (RFC_IS_CHAN_OPEN(rfc_params->p_mux))
            {
                crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_REQ);
            }
        }
        else
        {
            /* The mux is currently on its way down thus this request needs to
               be queued.
            */
            rfc_queue_prim(rfc_params->rfc_ctrl,
                           (RFCOMM_UPRIM_T *) p_prim,
                           sizeof(RFC_CLIENT_CONNECT_REQ_T));
        }
    }
}

/*! \brief Handle the receipt of an RFC_DISCONNECT_REQ primitive

     Function to process a close connection request from the upper layers.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received RFC disconnect request
*/
void rfc_handle_disconnect_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_DISCONNECT_REQ_T *p_prim)
{
    /* Disconnect which channel?
    */
    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the channel was found then both the p_mux and p_dlc fields will be
       non NULL.
    */
    if(NULL != rfc_params->p_mux)
    {
        crank_channel_fsm(rfc_params, NULL, RFC_EV_DISCONNECT_REQ);
    }
    else
    {
        /* Unknown channel , send a disconnect CFM with suitable error message.
        */
        rfc_send_disconnect_cfm(rfc_params->rfc_ctrl->phandle,
                                p_prim->conn_id,
                                (RFC_RESPONSE_T)RFC_INVALID_CHANNEL);
    }
}

/*! \brief Validate a server connect request

     This function is called on receipt of a server connect indication. This
     could take the form of a Parneg, Portneg or a SABM on a new DLC.

    \param rfc_params - Pointer to rfcomm instance data.
    \param dlci - requested DLC identifier for the new connection
*/
CsrBool rfc_validate_server_connect_req(RFC_CTRL_PARAMS_T *rfc_params,
                                       CsrUint8 dlci)
{
    RFC_SERV_CHAN_T *serv_chan =
        rfc_get_reg_server_chan_by_dlci(rfc_params, dlci);

    if (NULL != serv_chan)
    {
        /* Valid server channel thus create new dlc
           structure.
        */
        rfc_dlc_new(rfc_params,
                    dlci,
                    serv_chan->phandle,  /* From registered chan */
                    NULL,
                    (CsrBool)RFC_IS_INITIATOR(rfc_params->p_mux->flags),
                    serv_chan->context);

        return TRUE;
    }

    /* Invalid server channel requested.
    */
    return FALSE;
}

/*! \brief Handle a server connect response from the host

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_server_connect_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                                   RFC_SERVER_CONNECT_RSP_T *p_prim)
{
    CsrUint16 max_frame_size = p_prim->max_payload_size ;
    RFC_RESPONSE_T  response = RFC_SUCCESS;
    CsrUint8  break_signal = p_prim->break_signal;

    /* Check if this is a response for a known DLC.
    */
    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the channel was found then both the p_mux and p_dlc fields will be
       non NULL.
    */
    if(NULL != rfc_params->p_dlc)
    {
        /* Check if the host wishes to allow this connection.
        */
        if (p_prim->response == (RFC_RESPONSE_T)RFC_ACCEPT_SERVER_CONNECTION)
        {
            if (!rfc_validate_msc_params(0,
                                         p_prim->modem_signal,
                                         &break_signal) ||
                ( max_frame_size + RFC_MAX_DATA_HEADER_SIZE ) < RFC_MAX_NON_DATA_FRAME_LEN)
            {
                /* Invalid connect rsp parameters supplied, terminate
                   connection.
                */
                response = RFC_INCONSISTENT_PARAMETERS;
            }
            else
            {
                /* Extract config and store in the dlc structure.
                */
                rfc_params->p_dlc->info.dlc.p_config =
                    rfc_create_local_config(0,
                                            max_frame_size,
                                            p_prim->priority,
                                            p_prim->total_credits,
                                            p_prim->remote_l2cap_control,
                                            p_prim->local_l2cap_control,
                                            0,
                                            p_prim->break_signal,
                                            p_prim->modem_signal,
                                            p_prim->msc_timeout);

                switch (rfc_params->p_dlc->info.dlc.start_type)
                {
                    case RFC_START_SABM:
                        crank_channel_fsm(rfc_params, NULL, RFC_EV_OPEN_IND_DLC);
                        break;

                    case RFC_START_PARNEG:
                        if (rfc_params->p_dlc->info.dlc.p_temp_rpn != NULL)
                        {
                            rfc_send_portneg_ind(rfc_params->p_dlc->phandle,
                                                 rfc_params->p_dlc->info.dlc.conn_id,
                                                 rfc_params->p_dlc->info.dlc.p_temp_rpn);
                            CsrPmemFree(rfc_params->p_dlc->info.dlc.p_temp_rpn);
                            rfc_params->p_dlc->info.dlc.p_temp_rpn = NULL;
                        }

                        crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_IND);

                        break;

                    case RFC_START_PORTNEG:

                        rfc_send_portneg_ind(rfc_params->p_dlc->phandle,
                                             rfc_params->p_dlc->info.dlc.conn_id,
                                             rfc_params->p_dlc->info.dlc.p_temp_rpn);

                        CsrPmemFree(rfc_params->p_dlc->info.dlc.p_temp_rpn);
                        rfc_params->p_dlc->info.dlc.p_temp_rpn = NULL;
                        break;

                    case RFC_STARTED:
                    default:
                        /* Do nothing.
                        */
                        break;
                }

                /* Need to double check that the dlc still exists as the
                   parneg rsp function is able to destroy it.
                */
                if (rfc_params->p_dlc != NULL)
                {
                    rfc_params->p_dlc->info.dlc.start_type = RFC_STARTED;
                }
                return;
            }
        }
        else
        {
            response = RFC_DECLINE_SERVER_CONNECTION;
        }

        crank_channel_fsm(rfc_params, &response, RFC_EV_DISC_MODE_RSP);
    }
    else
    {
        /* Check there isnt a pending connect cfm for this conn id.
        */
        if (!rfc_dequeue_prim(rfc_params, RFC_SERVER_CONNECT_CFM,
                                        p_prim->conn_id))
        {
            /* Connect response received for unknown connection id thus send a
               connect cfm with suitable error status.
            */
            rfc_send_server_connect_cfm(rfc_params,
                                        rfc_params->rfc_ctrl->phandle,
                                        p_prim->conn_id,
                                        0,
                                        (RFC_RESPONSE_T)RFC_INVALID_CHANNEL) ;
        }
    }
}

/*! \brief Handle a disconnect response from the host

    The host will send one of these primitives when it has finished with the
    connection which it has previously been notified of closure. The connection
    will have been put on a destructor list which keeps any associated stream
    open until receipt of this prim, but will have allowed rfcomm to have
    already destroyed its connection. On receipt of this prim the connection
    will be removed from the destructor list.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_disconnect_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_DISCONNECT_RSP_T *p_prim)
{
    /* Check if the conn id is on the stream destructor list and if so CsrSchedTidy it
       up now.
    */
    bs_remove_from_distructor_list(RFCOMM_DESTRUCTOR,
                                   p_prim->conn_id);
}

/*! \brief Determine if incoming or outgoing connection

    This function checks the connection direction and returns TRUE if incoming,
    FALSE if outgoing.

    \param p_dlc - Pointer to dlc channel.
*/
static CsrBool rfc_is_incoming_connection(RFC_CHAN_T  *p_dlc)
{
    CsrBool incoming;

    /* An outgoing connection dlci is formed from the remote server channel
       and the inverse of the direction bit associated with the requesting
       entity. Thus if the direction bit for this session matches the dirbit
       stored in the connection id then by logical deduction this must be an
       incoming connection!
    */
    incoming =
        (RFC_GET_DIRBIT_FROM_DLCI(p_dlc->dlci) ==
         RFC_DIRBIT(p_dlc->flags));

    return incoming;
}

/*! \brief Send a RFCOMM connection confirmation to the host

    This function sends a Client or, Server Connect Confirm based
    on the type of request with appropriate error code.

    \param rfc_params - Pointer to rfcomm instance data.
    \param status - Status of the connection fail or, error.

    \return - none.
*/
static void rfc_send_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                       RFC_RESPONSE_T status)
{
    /* Need to determine if this is an outbound or inbound connection in
       order to send the corrent connect cfm to the host.
    */
    if (rfc_is_incoming_connection(rfc_params->p_dlc))
    {
        rfc_send_server_connect_cfm(rfc_params,
                                    rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    rfc_params->p_dlc->context,
                                    status);
    }
    else
    {
        rfc_send_client_connect_cfm(rfc_params,
                                    rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    rfc_params->p_dlc->context,
                                    status);
    }
}



/*! \brief Request security clearance from the DM

    This function sends a dm sm access request to the DM for a new DLC channel.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_access_request)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    CsrUint16 security_id;
    CsrBool incoming;
    PARAM_UNUSED(pv_arg2);

    incoming = rfc_is_incoming_connection(rfc_params->p_dlc);
    if (incoming)
    {
        security_id = (CsrUint16)(RFC_GET_SERV_CHAN_FROM_DLCI(rfc_params->p_dlc->dlci));
    }
    else
    {
        security_id = rfc_params->p_dlc->info.dlc.p_config->client_security_chan;
    }

    /* Send access request to the Security Manager.
    */
    dm_sm_access_req(RFCOMM_IFACEQUEUE,
                     &(rfc_params->p_mux->info.mux.bd_addr),
                     SEC_PROTOCOL_RFCOMM,
                     security_id,
                     incoming,
                     rfc_params->p_dlc->info.dlc.conn_id,
                     NULL);

    return FSM_EVENT_NULL;
}

/*! \brief Security access granted by the DM for a new channel

    This function is called when a dm sm access cfm prim has been received from
    the DM which allows access for the specified new channel. The action taken
    depends on whether this was for an incoming or outgoing connection.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_access_granted)
{
    CsrMblk  *mblk_frame;
    fsm_event_t  event = FSM_EVENT_NULL;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    CsrBool *incoming = (CsrBool *)pv_arg2;

    if (*incoming)
    {
        /* Create and send the UA acknowledgement in response to the incoming
           SABM.
        */
        mblk_frame = rfc_create_ctrl_frame(RFC_UA,
                                           rfc_params->p_dlc->flags,
                                           rfc_params->p_dlc->dlci);

        /* Rattle the state machine again to move to connected state.
        */
        if (rfc_params->p_dlc->dlci == 0)
        {
            event = RFC_EV_ACK_RSP_MUX;
        }
        else
        {
            event =  RFC_EV_ACK_RSP;
        }
    }
    else
    {
        /* Create a SABM frame on the DLCI pointed to within p_dlc. If we are
           creating a MUX channel then p_dlc will equal p_mux on entry.
        */
        mblk_frame = rfc_create_ctrl_frame(RFC_SABM,
                                           rfc_params->p_dlc->flags,
                                           rfc_params->p_dlc->dlci);

        /* Start the acknowledgement timer for this request */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_dlc->timers->contexts[RFC_CONNECT_CONTEXT]),
                        RFC_CTRL_ACK_TIMER);

    }

    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_frame);

    return event;
}

/*! \brief Handle an access denied event

    This function is called if a dm sm access cfm primitive is received which
    denies security access for a new connection.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_access_denied)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    CsrBool *incoming = (CsrBool *)pv_arg2;
    fsm_event_t fsm_event = FSM_EVENT_NULL;

    if (*incoming)
    {
        /* Create and send DM frame to the peer to reject the connection.
        */
        rfc_send_dm_frame(rfc_params->p_dlc->flags,
                          rfc_params->p_dlc->dlci,
                          rfc_params->p_mux->info.mux.cid);

        rfc_send_server_connect_cfm(rfc_params,
                                    rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    rfc_params->p_dlc->context,
                                    (RFC_RESPONSE_T)RFC_CONNECTION_REJ_SECURITY);

        /* Since we are destroying the DLCI, and the state machine has
         * not reached to the DISCONNECTED state we will force the
         * state to be DISCONNECTED
        */
        RFC_CHAN_CLOSE(rfc_params->p_dlc);
        rfc_dlc_destroy(rfc_params);
    }
    else
    {
        rfc_send_client_connect_cfm(rfc_params,
                                    rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    rfc_params->p_dlc->context,
                                    (RFC_RESPONSE_T)RFC_CONNECTION_REJ_SECURITY);

        /* Bring the DLCI down as the security fails */
        /* We have landed here when either PN CMD/RSP or,
         * SABM/UA has been finished so, its safe to send a DISC
         * frame as we no longer want to proceed with this channel
        */
        fsm_event = RFC_EV_DISCONNECT_REQ;
    }

    return fsm_event;
}

/*! \brief Handle a client connect complete event

    This function is called when a client ie outbound connection has
    successfully completed. If that connection was a Mux then the associated DLC
    list is scanned for 'pending' connections. Any found are initiated.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_connected)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Check if this is a DLC or Mux that has been successfully opened.
    */
    if (rfc_params->p_dlc->dlci == 0)
    {
        /* This is a mux channel. As this is a client ie outbound connection
           there should be at least one DLC attached to this mux which is ready
           to be opened.
        */
        for (rfc_params->p_dlc = rfc_params->p_mux->info.mux.p_dlcs;
             rfc_params->p_dlc != NULL;
             rfc_params->p_dlc = rfc_params->p_dlc->p_next)
        {
            /* Start parameter negotiation phase of data channel creation. NB
               for channels that were created before the MUX was up the
               max_frame_size in the p_config will not yet have been limited by
               the l2cap_mtu ,thus do that now.
            */
            if (rfc_params->p_dlc->info.dlc.p_config->max_frame_size >
                rfc_params->p_mux->info.mux.l2cap_mtu - RFC_MAX_DATA_HEADER_SIZE)
            {
                /* the max_frame_size is the size of the information field in the
                   RFCOMM frame. Allow space for the MAX RFCOMM header */
                rfc_params->p_dlc->info.dlc.p_config->max_frame_size =
                    rfc_params->p_mux->info.mux.l2cap_mtu  - RFC_MAX_DATA_HEADER_SIZE ;
            }

            crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_REQ);
        }
    }
    else
    {
        /* Start the MSC synchronisation timer/
        */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]),
                        RFC_MSC_INITIALISATION_TIMER);

        return RFC_EV_MSC_REQ;
    }

    return FSM_EVENT_NULL;
}

/*! \brief Handle a msc request (initial connect) event

    This function is called when a client msc request event has been received
    and it initiats the sending of the initial msc req using the host supplied
    default paramters.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_msc_request)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    rfc_send_modem_status_req(rfc_params,
                              rfc_params->p_dlc->dlci,
                              rfc_params->p_dlc->info.dlc.p_config->modem_signal,
                              rfc_params->p_dlc->info.dlc.p_config->break_signal);

    return FSM_EVENT_NULL;
}

/*! \brief Update the msc initialisation status

    This function updates the specified MSC flag state and then checks to see if
    that has caused the MSC initialisation to finish. If so a corresponding
    event is returned.

    \param rfc_params - Pointer to rfcomm instance data.
    \param msc_flag - flag to be updated
*/
static fsm_event_t rfc_update_msc_handshaking_state(RFC_CTRL_PARAMS_T *rfc_params,
                                                    CsrUint16 msc_flag)
{
    fsm_event_t fsm_event = FSM_EVENT_NULL;

    RFC_SET_MSC_FLAG(rfc_params->p_dlc->flags, msc_flag);

    if(RFC_IS_MSC_COMPLETE(rfc_params->p_dlc->flags) &&
        rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]))
    {
        fsm_event = RFC_EV_MSC_COMPLETE;
    }
    return fsm_event;
}

/*! \brief Handle a msc cfm (initial connect) event

    This function is called when a msc cfm event is received for an outbound
    (client) connection.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_msc_confirm)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* A MSC CFM prim will only be sent to the host if this is a response
       to a host issued MSC req rather than the one sent during connection
       set up.
    */
    if (RFC_IS_CHAN_OPEN(rfc_params->p_dlc))
    {
        /* Check to see if this response is a late one for the initialisation
           sequence. If it is we just ignore it and set the inbound complete
           flag.
        */
        if (!RFC_IS_OUTBOUND_MSC_COMPLETE(rfc_params->p_dlc->flags))
        {
            RFC_SET_MSC_FLAG(rfc_params->p_dlc->flags, RFC_OUTBOUND_MSC_COMPLETE);
        }
        else
        {
            rfc_send_msc_cfm(rfc_params->p_dlc->phandle,
                             rfc_params->p_dlc->info.dlc.conn_id,
                             RFC_SUCCESS);
        }
    }
    else
    {
        /* Check if this cfm is part of the intialisation sequence.
        */
        return rfc_update_msc_handshaking_state(rfc_params,
                                            (CsrUint16)RFC_OUTBOUND_MSC_COMPLETE);
    }

    /* Just kick the prim queue to see if a MSC command is pending, process it now */
    (void)rfc_dequeue_prim(rfc_params, RFC_MODEM_STATUS_REQ,
                             rfc_params->p_dlc->info.dlc.conn_id);

    return FSM_EVENT_NULL;
}

/*! \brief Handle a msc rsp (initial connect) event

    This function is called when a msc rsp event is received for an inbound
    (server) connection.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_server_msc_response)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    return rfc_update_msc_handshaking_state(rfc_params,
                                            (CsrUint16)RFC_INBOUND_MSC_COMPLETE);
}

/*! \brief Handle a msc initialisation complete event

    This function is called when a msc complete event is received.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_msc_complete)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Cancel the initialisation sequence timer, if running. If we have arrived
       here as a consequence of the timer firing then the timer will already
       have been cancelled.
    */
    if (rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]))
    {
        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]));
    }

#ifndef BUILD_FOR_HOST
        /* If this is a stream, notify the stream subsystem of the parameters
           and get the number of available credits.
        */
        if (rfc_params->rfc_ctrl->use_streams)
        {
            if(!stream_rfcomm_set_chan_params(rfc_params->p_dlc->info.dlc.conn_id,
                                              rfc_params->p_dlc->info.dlc.max_frame_size,
                                              RFC_CREDIT_FC_USED(rfc_params->p_mux->flags),
                                              &(rfc_params->p_dlc->info.dlc.rx_credits)))
            {
                /* This can only fail if we couldnt find a stream that matched
                   the connection id - this is catastrophic and fatal!
                */
                BLUESTACK_PANIC(
                                PANIC_RFCOMM_STREAM_MISMATCH);
            }
        }
#endif

        /* Connection is fully complete , update the Flow mode flag in the DLC
           by copying from the MUX.
        */
        if (RFC_IS_ERTM_MODE(rfc_params->p_mux->flags))
        {
            RFC_SET_FLOW_MODE(rfc_params->p_dlc->flags, RFC_CONNFLAG_ERTM);
        }

        /* Send a proper(Client/Server) confirmation to the host. */
        rfc_send_connect_cfm(rfc_params,
                               (RFC_RESPONSE_T)RFC_SUCCESS);

        /* Can now destroy the config structure as it is no longer needed.
        */
        CsrPmemFree(rfc_params->p_dlc->info.dlc.p_config);
        rfc_params->p_dlc->info.dlc.p_config = NULL;

        return FSM_EVENT_NULL;
}


/*! \brief Handle msc initialisation timer firing event

    \param arg1 - generic integer value passed to the timer function during the
                  timer_start call.
    \param arg2 - generic pointer value passed to the timer function during the
                  timer_start call.
*/
void rfc_msc_init_timer_event(CsrUint16 arg1, void *arg2)
{
    RFC_CTRL_PARAMS_T *context = (RFC_CTRL_PARAMS_T *) arg2;
    RFC_CTRL_PARAMS_T rfc_params;
    PARAM_UNUSED(arg1);

    rfc_timer_expired(context,
                      &rfc_params);

    /* In order to deal with potentially naughty peer devices , the
       initialisation timer firing, although a system error, will effectively
       be ignored and just move the state machine into a MSC complete state!
    */
    crank_channel_fsm(&rfc_params, NULL, RFC_EV_MSC_COMPLETE);
}


/*! \brief Handle a server connect complete event

    This function is called when a server ie inbound connection has successfully
    completed. The host is only notified if this is a DLC.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_server_connected)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Check to ensure that we have connection parameters ie there has
       been a parneg ind. If not then use the current defaults.
    */
    if (rfc_params->p_dlc->info.dlc.max_frame_size == 0)
    {
        /* None have been specified thus use the defaults stored for
           this session.
        */
        rfc_params->p_dlc->info.dlc.max_frame_size =
            rfc_params->p_mux->info.mux.serv_max_frame_size;

        rfc_params->p_dlc->info.dlc.tx_credits =
            rfc_params->p_mux->info.mux.serv_init_creds;
    }

    /* Start the MSC synchronisation timer/
    */
    rfc_timer_start(rfc_params,
                    &(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]),
                    RFC_MSC_INITIALISATION_TIMER);


    return RFC_EV_MSC_REQ;
}

/*! \brief Create a parneg frame

    Create a parneg frame (either a command or response) within a MBLK.

    \param rfc_params - Pointer to rfcomm instance data.
    \param from - Initiator or responder
    \param type - Command or response
    \param init_creds - initial credits to be sent in the parneg frame
    \param max_frame_size - maximum rfcomm frame size to be sent to the peer
    \param mblk_parneg - returned mblk containing the parneg frame
*/
static CsrMblk  *rfc_create_parneg_cmdrsp(RFC_CTRL_PARAMS_T   *rfc_params,
                                         RFC_LINK_DESIGNATOR from,
                                         RFC_CRTYPE          type,
                                         CsrUint8             init_creds,
                                         CsrUint16            max_frame_size)
{
    CsrMblk  *mblk_parneg;
    CsrUint8 *parneg_frame = NULL;
    CsrUint8  cl_i_field = 0;
    CsrUint8  len_field;
    CsrUint8  fcs;
    CsrUint8  crbit;

    /* Create and send a PARNEG frame.
    */
    mblk_parneg = CsrMblkMallocCreate((void **)&parneg_frame, RFC_PN_LEN);

    if (mblk_parneg != NULL)
    {
        /* Create the front end of the UIH data frame which will hold the parneg
           frame. Commands/responses go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &parneg_frame,
                              RFC_PN_FRAME_LEN,
                              from,
                              0);

        /* Determine flow control options. This is only done if the DLC is not
           yet open.
        */
        if (!RFC_IS_CHAN_OPEN(rfc_params->p_dlc))
        {
            if (type == RFC_CMD)
            {
                if (RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
                {
                    cl_i_field = RFC_PN_CL_I_CREDIT_BASED_FC_REQ;
                }
                else
                {
                    cl_i_field = RFC_PN_CL_I_NON_CREDIT_BASED_FC_REQ;
                }
            }
            else
            {
                if (RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
                {
                    cl_i_field = RFC_PN_CL_I_CREDIT_BASED_FC_RSP;
                }
                else
                {
                    cl_i_field = RFC_PN_CL_I_NON_CREDIT_BASED_FC_RSP;
                }
            }
        }

        /* Read the precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);

        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );

        len_field =  RFC_PN_DATA_LEN << 1;
        RFC_SET_EA_BIT(len_field);

        /* Compose the PN frame.
        */
        write_uints(
            &parneg_frame,
            URW_FORMAT(CsrUint8, 6, CsrUint16, 1, CsrUint8, 3),
            RFC_PN_TYPE | (crbit << 1),
            len_field,
            rfc_params->p_dlc->dlci,
            cl_i_field,
            rfc_params->p_dlc->info.dlc.priority,
            RFC_PN_ACK_TIMER,
            max_frame_size,
            RFC_PN_MAX_NUM_RTX,
            init_creds,
            fcs);

    }

    return mblk_parneg;
}

/*! \brief Determine initial and stored number of receive credits

    The host specifies a total number of credits available to the connection.
    This function determines how many get sent as initial credits to the peer in
    the parneg and how many are stored locally.

    \param rfc_params - Pointer to rfcomm instance data.
    \param init - Returned number of initial credits
*/
CsrUint8 rfc_configure_rx_credits(RFC_CTRL_PARAMS_T *rfc_params)
{
    CsrUint8 init = 1;

#ifndef BUILD_FOR_HOST
    if (!rfc_params->rfc_ctrl->use_streams)
#endif
    {
        /* Determine how many credits we can provide as initial credits in the
           parneg req and how many we can then store for later use.
        */
        rfc_params->p_dlc->info.dlc.allocated_rx_credits =
            rfc_params->p_dlc->info.dlc.p_config->total_credits;
        if (rfc_params->p_dlc->info.dlc.p_config->total_credits >
            RFC_MAX_INIT_CREDS)
        {
            rfc_params->p_dlc->info.dlc.rx_credits =
                rfc_params->p_dlc->info.dlc.p_config->total_credits - RFC_MAX_INIT_CREDS;

            /* The remaining credits in the total credits field will be used to source
               the initial credits in the parneg
            */
            init = RFC_MAX_INIT_CREDS;
        }
        else
        {
            rfc_params->p_dlc->info.dlc.rx_credits = 0;
            init = (CsrUint8)rfc_params->p_dlc->info.dlc.p_config->total_credits;
        }
    }

    return init;
}


/*! \brief Create and send a parneg command to the peer

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_par_neg_cmd)
{
    CsrMblk  *mblk_parneg;
    CsrUint8 init = 1;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_LINK_DESIGNATOR from;
    RFC_CONNECTION_PARAMETERS_T  *p_config = rfc_params->p_dlc->info.dlc.p_config;
    PARAM_UNUSED(pv_arg2);

    from = (RFC_IS_INITIATOR(rfc_params->p_mux->flags))
            ? RFC_INITIATOR
            : RFC_RESPONDER;

    /* How many initial credits to send? If this channel is streamed based then
       we wont know how many credits are available until the parameter
       negotiation is completed, thus give only 1 initial credit.
    */
    init = rfc_configure_rx_credits(rfc_params);

    rfc_params->p_dlc->info.dlc.priority = p_config->priority;

    /* Ensure host received max frame size doesnt exceed the L2CAP MTU.
       Also take care of the header which RFCOMM will add to the payload
    */
    if (p_config->max_frame_size > rfc_params->p_mux->info.mux.l2cap_mtu - RFC_MAX_DATA_HEADER_SIZE)
    {
        p_config->max_frame_size = rfc_params->p_mux->info.mux.l2cap_mtu -  RFC_MAX_DATA_HEADER_SIZE;
    }

    /* Create the PARNEG command frame.
    */
    mblk_parneg = rfc_create_parneg_cmdrsp(rfc_params,
                                           from,
                                           RFC_CMD,
                                           init,
                                           p_config->max_frame_size);

    /* Start the response timer for this request */
    rfc_timer_start(rfc_params,
                    &(rfc_params->p_dlc->timers->contexts[RFC_CONNECT_CONTEXT]),
                    RFC_CMD_RESP_TIMER);

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_parneg);

    return FSM_EVENT_NULL;
}

/*! \brief Handle a parneg response received event

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_par_neg_cfm)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    CsrUint16 peer_mfs;
    PARAM_UNUSED(pv_arg2);

    /* Validate received parneg parameters.
    */
    if ( rfc_params->p_dlc->info.dlc.p_temp_pn->cfc_supported != RFC_PN_CL_I_CREDIT_BASED_FC_RSP)
    {
        /* Remote end doesnt support credit based flow control.
        */
        RFC_RESET_CREDIT_FC_FLAG(rfc_params->p_mux->flags);
        rfc_params->p_dlc->info.dlc.tx_credits = 0;
    }
    else
    {
        rfc_params->p_dlc->info.dlc.tx_credits =
            rfc_params->p_dlc->info.dlc.p_temp_pn->init_creds;
    }

    /* Temporarily store the peer's max frame size so that we can free the
       p_temp_pn structure now.
    */
    peer_mfs = rfc_params->p_dlc->info.dlc.p_temp_pn->max_frame_size;
    CsrPmemFree(rfc_params->p_dlc->info.dlc.p_temp_pn);
    rfc_params->p_dlc->info.dlc.p_temp_pn = NULL;

    /* Agree max frame size.
    */
    if (peer_mfs <= rfc_params->p_dlc->info.dlc.p_config->max_frame_size)
    {
        rfc_params->p_dlc->info.dlc.max_frame_size = peer_mfs;
    }
    else
    {
        /* It is illegal for the peer to respond with a bigger max frame size
           thus reject the connection.
        */
        crank_channel_fsm(rfc_params, NULL, RFC_EV_DISCONNECT_REQ);

        return FSM_EVENT_NULL;

    }

    rfc_params->p_dlc->info.dlc.start_type = RFC_STARTED;

    return RFC_EV_OPEN_REQ_DLC;

}

/*! \brief Queue a disconnect initiated locally event

    This function is called when a disconnect request has been received from the
    host while the DLC is in the CONNECTING state (ie it has sent an SABM and is
    awaiting a response). It is illegal to send a DISC at this point and thus we
    need to effectively queue the disconnect. This can be done simply by setting
    the disconnecting flag.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_local_disconnect_pending)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Set the disconnecting flag , this can be checked on receipt of a UA
    */
    RFC_SET_DISCONNECTING(rfc_params->p_dlc->flags);

    return FSM_EVENT_NULL;
}

/*! \brief Handle a disconnect initiated locally event

    This function is called when a close / disconnect request has been received
    from the host. It causes a DISC frame to be issued to the peer.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_local_disconnect_request)
{
    CsrMblk  *mblk_disc;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* As this channel is going into disconnecting mode, free any outstanding
       timers.
    */
    rfc_cancel_all_timers(rfc_params->p_dlc);

    /* Create a DISC frame on the DLCI pointed to within p_dlc. If we are
       disconnecting a MUX channel then p_dlc will equal p_mux on entry.
    */
    mblk_disc = rfc_create_ctrl_frame(RFC_DISC,
                                      rfc_params->p_dlc->flags,
                                      rfc_params->p_dlc->dlci);


    /* Start the release timer for this request. This should now be the only
       outstanding timer on this channel.
    */
    rfc_timer_start(rfc_params,
                    &(rfc_params->p_dlc->timers->contexts[RFC_DISCONNECT_CONTEXT]),
                    RFC_L2CAP_RELEASE_TIMER);

    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_disc);

    /* Set disconnecting flag.
    */
    RFC_SET_DISCONNECTING(rfc_params->p_dlc->flags);

    return FSM_EVENT_NULL;

}

/*! \brief Handle a cancel connection event

    This function is called when a disconnect request has been received for a
    client channel when we are still in the process of bringing the MUX up.
*/
FSM_ACTION_FN(rfc_af_cancel_connection)
{
    PARAM_UNUSED(pv_arg1);
    PARAM_UNUSED(pv_arg2);

    /* Simulate receiving confirmation of a disconnect request , the actions
       taken are the same so it is more efficient to do this than adding extra
       states to the FSM!
    */
    return RFC_EV_ACK_CFM_DISC;
}

/*! \brief Handle a disconnect confirm event

    This function is called when a UA frame has been received in response to a
    locally sent DISC.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_local_disconnect_confirm)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Is this a Mux or DLC disconnect confirmation?
    */
    if (rfc_params->p_dlc->dlci == 0)
    {
        /* Drop the L2CAP channel and destroy the Mux. NB for a Mux channel both
           p_mux and p_dlc will point to the same channel structure.
        */
        L2CA_DisconnectReq(rfc_params->p_mux->info.mux.cid);

        /* Do a protocol disconnect as we are going to tear down all DLCI's
        */
        rfc_shutdown_session(rfc_params,
                             (RFC_RESPONSE_T)RFC_ABNORMAL_DISCONNECT);

    }
    else
    {
        /* Send confirmation to host.
        */
        rfc_send_disconnect_cfm(rfc_params->p_dlc->phandle,
                                rfc_params->p_dlc->info.dlc.conn_id,
                                (RFC_RESPONSE_T)RFC_SUCCESS);

        /* If this is a stream based channel then the stream will be destroyed
           automatically once the disconnect cfm has been read by the layer
           above.
        */
        rfc_dlc_destroy(rfc_params);
    }

    return FSM_EVENT_NULL;
}


/*! \brief Handle a remote disconnect indication event

    This function is called when a peer device has sent a DM frame in response
    to a local command frame on a client connection.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_client_disconnect)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    CsrUint16 cid = rfc_params->p_mux->info.mux.cid;

    PARAM_UNUSED(pv_arg2);

    if (rfc_params->p_dlc->dlci == 0)
    {
        /* A valid DM frame has been received on the Mux channel thus the Mux
           and any associated DLCs will be disconnected then the l2cap channel
           will be dropped.
        */
        rfc_shutdown_session(rfc_params,
                             (RFC_RESPONSE_T)RFC_ABNORMAL_DISCONNECT);

        L2CA_DisconnectReq(cid);

    }
    else
    {
        /* A valid DM frame has been received on a DLC channel thus disconnect
           this channel.
        */
        if (RFC_IS_DISCONNECTING(rfc_params->p_dlc->flags))
        {
            /* Channel is already disconnecting so issue a confirm.
            */
            rfc_send_disconnect_cfm(rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    (RFC_RESPONSE_T)RFC_SUCCESS);
        }
        else
        {
            /* Otherwise we are still setting up the connection so a connect
               confirm with error status will be sent.
            */
            rfc_send_client_connect_cfm(rfc_params,
                                        rfc_params->p_dlc->phandle,
                                        rfc_params->p_dlc->info.dlc.conn_id,
                                        rfc_params->p_dlc->context,
                                        (RFC_RESPONSE_T)RFC_REMOTE_REFUSAL);

        }

        rfc_dlc_destroy(rfc_params);
    }

    return FSM_EVENT_NULL;
}


/*! \brief Handle a remote disconnect request event

    This function is called when a DISC frame is received from the peer.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_remote_disconnect_request)
{
    CsrMblk  *mblk_frame;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Create and send the UA acknowledgement in response to the incoming DISC.
    */
    mblk_frame = rfc_create_ctrl_frame(RFC_UA,
                                       rfc_params->p_dlc->flags,
                                       rfc_params->p_dlc->dlci);

    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_frame);

    /* Check if this is a disconnect for a DLC or MUX.
    */
    if (rfc_params->p_dlc == rfc_params->p_mux)
    {
        /* Drop the L2CAP channel and destroy the Mux.
        */
        L2CA_DisconnectReq(rfc_params->p_mux->info.mux.cid);
        rfc_shutdown_session(rfc_params,
                             (RFC_RESPONSE_T)RFC_ABNORMAL_DISCONNECT);
    }
    else
    {
        /* Now
           Add the conn id to the destructor list,
           destroy the DLC and
           notify the host .
        */
#ifndef BUILD_FOR_HOST
        if (rfc_params->rfc_ctrl->use_streams)
        {
            bs_add_to_destructor_list(RFCOMM_DESTRUCTOR,
                                      rfc_params->p_dlc->info.dlc.conn_id,
                                      stream_rfcomm_get(rfc_params->p_dlc->info.dlc.conn_id),
                                      &(rfc_params->p_mux->info.mux.bd_addr));
        }
#endif

        /* Only send a Disconnect IND when application has already received
           successful connection confirmation */
        if(rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_MSC_INIT_CONTEXT]))
        {
            rfc_send_connect_cfm(rfc_params,
                                       (RFC_RESPONSE_T)RFC_NORMAL_DISCONNECT);
        }
        else
        {
            rfc_send_disconnect_ind(rfc_params->p_dlc,
                                       (RFC_RESPONSE_T)RFC_NORMAL_DISCONNECT);
        }
        rfc_dlc_destroy(rfc_params);
    }

    return FSM_EVENT_NULL;

}



/*! \brief Handle a protocol disconnect event in the open state

    This function is called when a L2CAP_DISCONNECT_IND primitive is received
    while the DLC is open.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
    \params pv_arg2 (status) - status from L2CAP
*/
FSM_ACTION_FN(rfc_af_protocol_disconnect_open)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_RESPONSE_T *status = (RFC_RESPONSE_T *)pv_arg2;

    rfc_send_disconnect_ind(rfc_params->p_dlc, *status);

    /* Now
       Add the conn id to the destructor list,
       destroy the DLC and
       notify the host .
    */
#ifndef BUILD_FOR_HOST
    if (rfc_params->rfc_ctrl->use_streams)
    {
        bs_add_to_destructor_list(RFCOMM_DESTRUCTOR,
                                  rfc_params->p_dlc->info.dlc.conn_id,
                                  stream_rfcomm_get(rfc_params->p_dlc->info.dlc.conn_id),
                                  &(rfc_params->p_mux->info.mux.bd_addr));
    }
#endif

    rfc_channel_destroy(&(rfc_params->p_mux->info.mux.p_dlcs),
                        &(rfc_params->p_dlc));

    return FSM_EVENT_NULL;

}

/*! \brief Handle a protocol disconnect event when in the server config state.

    This function is called when a DLC which has sent a server connect ind to
    the host but has yet to get a server connect response, has been disconnected
    abnormally ( E.g. by an L2CAP disconnect ind).

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
    \params pv_arg2 (status) - status from L2CAP
*/
FSM_ACTION_FN(rfc_af_protocol_disconnect_cfg)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_RESPONSE_T *status = (RFC_RESPONSE_T *)pv_arg2;

    /* Have we already had a server connect rsp or not? If not then we need to
       delay the sending of the connect cfm with error status until we do.
    */
    if (rfc_params->p_dlc->info.dlc.p_config != NULL)
    {
        /* Have already had the rsp so we can send a connect cfm immediately!
        */
        rfc_send_server_connect_cfm(rfc_params,
                                    rfc_params->p_dlc->phandle,
                                    rfc_params->p_dlc->info.dlc.conn_id,
                                    rfc_params->p_dlc->context,
                                    *status);
    }
    else
    {
        /* create a server connect cfm and queue it.
        */
        rfc_queue_server_connect_cfm(rfc_params,
                                     rfc_params->p_dlc->phandle,
                                     rfc_params->p_dlc->info.dlc.conn_id,
                                     rfc_params->p_dlc->context,
                                     *status);
    }

    rfc_channel_destroy(&(rfc_params->p_mux->info.mux.p_dlcs),
                        &(rfc_params->p_dlc));

    return FSM_EVENT_NULL;
}


/*! \brief Handle a protocol disconnect event when not in an open state.

    This function is called when a DLC, which is not in the CONNECTED state, has
    been disconnected abnormally ( E.g. by an L2CAP disconnect ind).

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
    \params pv_arg2 (status) - status from L2CAP
*/
FSM_ACTION_FN(rfc_af_protocol_disconnect)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_RESPONSE_T *status = (RFC_RESPONSE_T *)pv_arg2;

    if (RFC_IS_DISCONNECTING(rfc_params->p_dlc->flags))
    {
        rfc_send_disconnect_cfm(rfc_params->p_dlc->phandle,
                                rfc_params->p_dlc->info.dlc.conn_id,
                                (RFC_RESPONSE_T)RFC_SUCCESS);
    }
    else
    {
        if (RFC_IS_SERVER_CHAN(rfc_params->p_dlc->flags,rfc_params->p_dlc->dlci))
        {
            rfc_send_server_connect_cfm(rfc_params,
                                        rfc_params->p_dlc->phandle,
                                        rfc_params->p_dlc->info.dlc.conn_id,
                                        rfc_params->p_dlc->context,
                                        *status);
        }
        else
        {
            rfc_send_client_connect_cfm(rfc_params,
                                        rfc_params->p_dlc->phandle,
                                        rfc_params->p_dlc->info.dlc.conn_id,
                                        rfc_params->p_dlc->context,
                                        *status);
        }
    }

    /* If this is a stream based channel then the stream will be destroyed
       automatically once the disconnect cfm has been read by the layer
       above.
    */
    rfc_channel_destroy(&(rfc_params->p_mux->info.mux.p_dlcs),
                        &(rfc_params->p_dlc));

    return FSM_EVENT_NULL;

}

/*! \brief Handle a server disconnect event

    This function is called if the host has declined an incoming server channel
    open request.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_server_disconnect)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_RESPONSE_T *response = (RFC_RESPONSE_T *)pv_arg2;

    /* Host has declined server channel request, thus respond with a DM frame
       to the peer.
    */
    rfc_send_dm_frame(rfc_params->p_dlc->flags,
                      rfc_params->p_dlc->dlci,
                      rfc_params->p_mux->info.mux.cid);

    /* Confirm the rejection to the host.
    */
    rfc_send_server_connect_cfm(rfc_params,
                                rfc_params->p_dlc->phandle,
                                rfc_params->p_dlc->info.dlc.conn_id,
                                rfc_params->p_dlc->context,
                                (RFC_RESPONSE_T)*response);

    rfc_dlc_destroy(rfc_params);

    return FSM_EVENT_NULL;

}

/*! \brief Handle a server request command event

    This function is called whenever a portneg, parneg or open indication is
    received as the first primitive for a new DLC.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_server_request_cmd)
{
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    if (rfc_params->p_dlc->info.dlc.start_type == RFC_START_SABM)
    {
        /* A server connection has been initiated without prior parameter
           negotiation. Check if there has been parameter negotiation for any DLCs on this Mux.
        */
        if (rfc_params->p_mux->info.mux.serv_max_frame_size == 0)
        {
            /* This is the very first server connection and there has been no parneg
               request at all thus use system defaults.
            */
            rfc_params->p_mux->info.mux.serv_max_frame_size =
                RFC_DEFAULT_MAX_FRAME_SIZE;

            /* Initiating a server connection without any prior parneg means
               this is likely to be a pre btv1.1 device thus disable CFC.
            */
            RFC_RESET_CREDIT_FC_FLAG(rfc_params->p_mux->flags);
        }
    }

    /* If this is a stream connection then we need to notify the stream
       subsystem. If the stream make fails then the remote connection is
       refused.
    */
#ifndef BUILD_FOR_HOST
    if (rfc_params->rfc_ctrl->use_streams)
    {
        if (NULL == stream_rfcomm_make(rfc_params->p_dlc->info.dlc.conn_id,
                                       rfc_params->rfc_ctrl))
        {
            rfc_send_dm_frame(rfc_params->p_mux->flags,
                              rfc_params->p_dlc->dlci,
                              rfc_params->p_mux->info.mux.cid);

            rfc_dlc_destroy(rfc_params);
            return FSM_EVENT_NULL;
        }
    }
#endif

    /* Notify host of new incoming connect request.
    */
    rfc_send_server_connect_ind(rfc_params);

    return FSM_EVENT_NULL;
}

/*! \brief Determine the max frame size to be used.

    \param rfc_params - Pointer to rfcomm instance data.
    \param local_mfs - locally agreed max frame size
    \param peer_mfs - peer requested max frame size
*/
static CsrUint16 rfc_validate_max_frame_size(RFC_CTRL_PARAMS_T *rfc_params,
                                            CsrUint16 local_mfs,
                                            CsrUint16 peer_mfs)
{
    CsrUint16 agreed_mfs = 0;

    /* If the peer has supplied a max frame size value that cannot hold all the
       possible basic frame types then it is rejected.
    */
    if ((peer_mfs + RFC_MAX_DATA_HEADER_SIZE) < RFC_MAX_NON_DATA_FRAME_LEN)
    {
        return 0;
    }

    if (rfc_params->p_dlc->info.dlc.max_frame_size == 0)
    {
        /* This is the first parneg negotiation.
        */
        if (local_mfs < peer_mfs)
        {
            agreed_mfs = local_mfs;
        }
        else
        {
            agreed_mfs = peer_mfs;
        }
    }
    else
    {
        /* This isnt the first parneg on this dlc, thus check whether or not
           the channel is already open. If it's open then we no longer allow
           the Max Frame Size to be negotiated and will indicate this by
           returning our current value.
        */
        if (RFC_IS_CHAN_OPEN(rfc_params->p_dlc))
        {
            agreed_mfs = rfc_params->p_dlc->info.dlc.max_frame_size;
        }
        else
        {
            if (peer_mfs <= local_mfs)
            {
                agreed_mfs = peer_mfs;
            }
        }
    }

    /* Ensure agreed max frame size doesnt exceed the L2CAP MTU.
       Also take care of the header which RFCOMM will add to the payload
    */
    if (agreed_mfs > rfc_params->p_mux->info.mux.l2cap_mtu - RFC_MAX_DATA_HEADER_SIZE)
    {
        agreed_mfs = rfc_params->p_mux->info.mux.l2cap_mtu -  RFC_MAX_DATA_HEADER_SIZE;
    }

    return agreed_mfs;
}


/*! \brief Handle a server parneg response event

    This function is called every time a parneg cmd is received from the peer.
    The received parameters are checked against host supplied ones and a
    suitable parneg response frame is generated and sent. The default parameters
    are also updated to allow for the case where no further parameter
    negotiation is performed.

    \param pv_arg1 (rfc_params) - Pointer to rfcomm instance data.
*/
FSM_ACTION_FN(rfc_af_server_par_neg_rsp)
{
    CsrMblk *mblk_parneg;
    CsrUint8 init = 0;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    RFC_LINK_DESIGNATOR  from;
    CsrUint16 agreed_mfs;
    CsrUint16 local_mfs;
    PARAM_UNUSED(pv_arg2);

    /* Check the parameters received from the host against those requested for
       the connection and send a PN response accordingly. Different actions are
       required depending upon whether this is the first parneg on this DLC (ie
       if max_frame is 0).
    */
    if (rfc_params->p_dlc->info.dlc.p_config != NULL)
    {
        local_mfs = rfc_params->p_dlc->info.dlc.p_config->max_frame_size;
    }
    else
    {
        local_mfs = rfc_params->p_dlc->info.dlc.max_frame_size;
    }

    agreed_mfs = rfc_validate_max_frame_size(
        rfc_params,
        local_mfs,
        rfc_params->p_dlc->info.dlc.p_temp_pn->max_frame_size);

    if (agreed_mfs == 0)
    {
        /* Peer has sent an unacceptable max_frame_size thus respond with a DM
           frame.
        */
        rfc_send_dm_frame(rfc_params->p_mux->flags,
                          rfc_params->p_dlc->dlci,
                          rfc_params->p_mux->info.mux.cid);

        /* Now destroy the dlc.
        */
        rfc_dlc_destroy(rfc_params);
        return FSM_EVENT_NULL;
    }

    /* Perform any other configuration that needs to be done on the very first
       parneg.
    */
    if( rfc_params->p_dlc->info.dlc.max_frame_size == 0 )
    {
        if (rfc_params->p_dlc->info.dlc.p_temp_pn->cfc_supported)
        {
            rfc_params->p_mux->info.mux.serv_init_creds =
                rfc_params->p_dlc->info.dlc.p_temp_pn->init_creds;
            rfc_params->p_dlc->info.dlc.tx_credits =
                rfc_params->p_dlc->info.dlc.p_temp_pn->init_creds;

            /* How many initial credits to send? If this channel is streamed based then
               we wont know how many credits are available until the parameter
               negotiation is completed, thus give only 1 initial credit.
            */
            init = rfc_configure_rx_credits(rfc_params);
        }
        else
        {
            RFC_RESET_CREDIT_FC_FLAG(rfc_params->p_mux->flags);
            rfc_params->p_mux->info.mux.serv_init_creds = 0;
            rfc_params->p_dlc->info.dlc.tx_credits = 0;
            rfc_params->p_dlc->info.dlc.rx_credits = 0;
        }

        /* Always use initiator's priority.
        */
        rfc_params->p_dlc->info.dlc.priority =
            rfc_params->p_dlc->info.dlc.p_temp_pn->priority;

    }

    /* Store the agreed max frame size and update default max_frame_size for any
           subsequent connections which have no parameter negotiation.
    */
    rfc_params->p_dlc->info.dlc.max_frame_size = agreed_mfs;

    rfc_params->p_mux->info.mux.serv_max_frame_size =
        rfc_params->p_dlc->info.dlc.max_frame_size;

    from = RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

    /* Create the response.
    */
    mblk_parneg = rfc_create_parneg_cmdrsp(rfc_params,
                                           from,
                                           RFC_RSP,
                                           init,
                                           rfc_params->p_dlc->info.dlc.max_frame_size);

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_parneg);

    /* It is now safe to remove the stored parneg values.
    */
    CsrPmemFree(rfc_params->p_dlc->info.dlc.p_temp_pn);
    rfc_params->p_dlc->info.dlc.p_temp_pn = NULL;

    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(rfc_af_disconnect_xover)
{
    CsrMblk  *mblk_frame;
    RFC_CTRL_PARAMS_T *rfc_params = (RFC_CTRL_PARAMS_T *)pv_arg1;
    PARAM_UNUSED(pv_arg2);

    /* Create and send the UA acknowledgement in response to the incoming DISC.
    */
    mblk_frame = rfc_create_ctrl_frame(RFC_UA,
                                       rfc_params->p_dlc->flags,
                                       rfc_params->p_dlc->dlci);

    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_frame);

    return FSM_EVENT_NULL;

}

#endif /* INSTALL_RFCOMM_MODULE */
