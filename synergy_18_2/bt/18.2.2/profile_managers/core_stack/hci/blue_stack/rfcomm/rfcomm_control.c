/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_control.c
\brief  Contains all the functions used for the control and configuration of
        RFCOMM.
*/

#include "csr_synergy.h"
#include "rfcomm_private.h"

#ifdef ENABLE_SHUTDOWN
#include "rfcommlib.h"
#include "dmlib.h"
#endif

#include "csr_util.h"

STATIC_RFC_CTRL


/* Local function prototypes.
*/
static CsrMblk  *rfc_create_portneg_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                          RFC_LINK_DESIGNATOR from,
                                          RFC_CRTYPE type);

static void rfc_send_port_neg_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                     RFC_CRTYPE  type);

static CsrMblk  *rfc_create_nsc_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                                   CsrUint8 command,
                                   CsrUint8 cmd_res);

static CsrBool rfc_send_credit_only_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint16 send_threshold);

static CsrUint8 rfc_allocate_serv_chan_id(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint8 num_reg_chans);

static CsrBool rfc_is_vm_connected(CsrSchedQid  phandle);

static void rfc_queue_data_packet(RFC_CHAN_T *p_dlc,
                                  CsrMblk *payload,
                                  CsrBool is_stream);

static void rfc_remove_queued_data_packet(RFC_CHAN_T *p_dlc);

static CsrUint16 rfc_is_data_queued(RFC_CTRL_PARAMS_T *rfc_params);

static CsrMblk  *rfc_create_fc_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                     RFC_CRTYPE type,
                                     RFC_FC_T  fc_type);

/* Function specifications.
*/


/*! \brief Wrapper function for state machine invocation

    RFCOMM has only one state machine, rfcomm_channel_fsm.

    \param rfc_params - Pointer to rfcomm instance data.
    \param argv2 - Generic pointer
    \param event - Event to be processed
*/
void crank_channel_fsm(RFC_CTRL_PARAMS_T *rfc_params,
                       void  *argv2,
                       fsm_event_t event)
{
    (void)fsm_16bit_run(&rfcomm_channel_fsm, &(rfc_params->p_dlc->state), event, rfc_params , argv2);
}


/*! \brief System initialisation of RFCOMM

    \param control_data - Pointer to rfcomm instance data.
*/
void rfc_init(void **control_data)
{
    RFC_CTRL_PARAMS_T  rfc_params = {NULL, NULL, NULL, NULL, 0};
    RFC_CTRL_INIT(control_data);

    rfc_params.rfc_ctrl = (RFC_CTRL_T *)*(control_data);

    /* Server channels are allocated from 1 to 30 inclusive.
    */
    rfc_params.rfc_ctrl->serv_chan_list = NULL;
    rfc_params.rfc_ctrl->last_conn_id = RFC_CONN_ID_INITIALISER;
    rfc_params.rfc_ctrl->prim_queue = NULL;

    if(!HciClientEnabled())
        return;
}

/*! \brief System de-initialisation of RFCOMM

    \param control_data - Pointer to rfcomm instance data.
*/
#ifdef ENABLE_SHUTDOWN
void rfc_deinit(void **control_data)
{
    RFC_CTRL_PARAMS_T  rfc_params = {NULL, NULL, NULL, NULL, 0};
    RFC_SERV_CHAN_T *p1;
    RFC_SERV_CHAN_T *p2;
    CsrUint16 mi;
    RFCOMM_UPRIM_T *p_prim;


    rfc_params.rfc_ctrl = (RFC_CTRL_T *)*(control_data);

    /* Free the server channel list.
    */
    for (p1 = rfc_params.rfc_ctrl->serv_chan_list;
         p1 != NULL ; p1 = p2 )
    {
        p2 = p1->p_next;
        CsrPmemFree(p1);
    }

    /* Free and destroy any internally queued RFCOMM prims.
    */
    rfc_flush_prim_queue(&rfc_params);

    /* Empty the scheduler queue.
    */
    while(CsrSchedMessageGet(&mi, (void**)&p_prim))
    {
        switch (mi)
        {
            case HOSTIO_SOURCED_PRIM:
            case RFCOMM_PRIM:
                rfc_free_primitive(p_prim);

                /* hostio sets mi to 0, so assume those are also RFCOMM prims
                */
                break;

            case L2CAP_PRIM:
                L2CA_FreePrimitive((L2CA_UPRIM_T *) p_prim);
                break;

            case DM_PRIM:
                dm_free_upstream_primitive((DM_UPRIM_T*)p_prim);
                break;

            default:
                BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
                break;
        }
    }


    /* Go through each Mux and destroy the corresponding RFCOMM session.
    */
    while((rfc_params.p_mux = rfc_params.rfc_ctrl->mux_list) != NULL)
    {
        while ((rfc_params.p_dlc = rfc_params.p_mux->info.mux.p_dlcs) != NULL)
        {
            rfc_channel_destroy(&(rfc_params.p_mux->info.mux.p_dlcs),
                                &(rfc_params.p_dlc));
        }

        rfc_channel_destroy(&(rfc_params.rfc_ctrl->mux_list),
                            &(rfc_params.p_mux));
    }

    CsrPmemFree(*control_data);
    *control_data = NULL;
}
#endif

/*! \brief Handle acknowledgement timer firing event

    \param arg1 - generic integer value passed to the timer function during the
                  timer_start call.
    \param arg2 - generic pointer value passed to the timer function during the
                  timer_start call.
*/
void rfc_ctrl_ack_timer_event(CsrUint16 arg1, void *arg2)
{
    RFC_CTRL_PARAMS_T *context = (RFC_CTRL_PARAMS_T *) arg2;
    RFC_CTRL_PARAMS_T rfc_params;
    CsrUint16 cid;
    PARAM_UNUSED(arg1);

    rfc_timer_expired(context,
                      &rfc_params);

    cid = rfc_params.p_mux->info.mux.cid;

    /* The default action when a response/ack timeout fires is to take down
       the whole rfcomm session.
    */
    rfc_shutdown_session(&rfc_params,
                         RFC_RES_ACK_TIMEOUT);

    L2CA_DisconnectReq(cid);

}

/*! \brief Handle credit return timer firing event

    \param arg1 - generic integer value passed to the timer function during the
                  timer_start call.
    \param arg2 - generic pointer value passed to the timer function during the
                  timer_start call.
*/
void rfc_credit_return_timer_event(CsrUint16 arg1, void *arg2)
{
    RFC_CTRL_PARAMS_T *context = (RFC_CTRL_PARAMS_T *) arg2;
    RFC_CTRL_PARAMS_T rfc_params;
    PARAM_UNUSED(arg1);

    rfc_timer_expired(context,
                      &rfc_params);

    (void)rfc_send_credit_only_frame(&rfc_params, 0);
}


/*! \brief Wrapper function for timer start function

    A wrapper to timer_start is needed in order to take a pmalloc'd copy of the
    rfcomm instance data to pass to the timer function.

    \param rfc_params - Pointer to rfcomm instance data.
    \param context - Pointer to context pointer
    \param timer_value - timeout in seconds
*/
void rfc_timer_start(RFC_CTRL_PARAMS_T *rfc_params,
                     RFC_CTRL_PARAMS_T **context,
                     RFC_TIMER_VALUES_T timer)
{
    RFC_CTRL_PARAMS_T  *new_context;
    void (*fn)(CsrUint16 mi, void *mv) = rfc_ctrl_ack_timer_event;
    CsrTime delay = 0;

    if(NULL != *context)
    {
        /* There is already a timer running which is using this context! This
           should never happen and if it does indicates a fatal condition!
        */
        BLUESTACK_PANIC(
                        CSR_BT_PANIC_RFCOMM_TIMER_ALREADY_STARTED);
    }

    new_context = zpnew(RFC_CTRL_PARAMS_T);

    /* Store the pointer to pmalloc'd area
    */
    *context = new_context;

    /* Copy the instance data to the context.
    */
    CsrMemCpy((void *)*context,
           (void *)rfc_params,
           sizeof(RFC_CTRL_PARAMS_T));

    switch(timer)
    {
        case RFC_CTRL_ACK_TIMER:

            delay = RFC_CTRL_ACK_TIMER_VALUE;
            break;

        case RFC_CMD_RESP_TIMER:

            delay = RFC_CMD_RESP_TIMER_VALUE;
            break;

        case RFC_L2CAP_RELEASE_TIMER:

            delay = RFC_L2CAP_RELEASE_TIMER_VALUE;
            break;

        case RFC_MUX_SHUTDOWN_TIMER:

            fn = rfc_mux_shutdown_timer_event;
            delay = RFC_MUX_SHUTDOWN_TIMER_VALUE;
            break;

        case RFC_CREDIT_RETURN_TIMER:

            fn = rfc_credit_return_timer_event;
            delay = RFC_CREDIT_RETURN_TIMER_VALUE;
            break;

        case RFC_MSC_INITIALISATION_TIMER:

            fn = rfc_msc_init_timer_event;
            delay = rfc_params->p_dlc->info.dlc.p_config->msc_timeout
                    * CSR_SCHED_MILLISECOND;
            break;

        default:

            /* Error!
            */
            BLUESTACK_PANIC(
                            CSR_BT_PANIC_RFCOMM_INVALID_TIMER_TYPE);
            break;
    }

    timer_start(&((*context)->chan_timer),
                delay,
                fn,
                0,
                (void *)(*context));
}

/*! \brief Wrapper function for timer cancel function

    A wrapper to timer_cancel is needed in order to free the pmalloc'd copy of
    the rfcomm instance data.

    \param context - Pointer to context pointer
*/
void rfc_timer_cancel(RFC_CTRL_PARAMS_T **context)
{
    if(NULL != *context)
    {
        timer_cancel(&((*context)->chan_timer));
        CsrPmemFree(*context);
        *context = NULL;
    }
}

/*! \brief Is the timer running

    \param context - Pointer to context pointer
    \param running - TRUE if running, FALSE otherwise
*/
CsrBool rfc_is_timer_running(RFC_CTRL_PARAMS_T *context)
{
    if(NULL != context)
    {
        return TRUE;
    }

    return FALSE;
}

/*! \brief Tidy up timer context once the timer has fired.

    Restores the rfcomm instance data from the supplied context. A search is
    then made through the p_dlc or the p_mux looking for the matching context.
    If p_dlc is non-NULL then it means it was a data channel for which the timer
    was started. If p_dlc is NULL but p_mux is non-NULL then it was the mux for
    which a timer was started. If both are NULL then something has gone
    seriously wrong!!

    \param rfc_params - Pointer to rfcomm instance data.
    \param context - Context pointer
*/
void rfc_timer_expired(RFC_CTRL_PARAMS_T *context,
                       RFC_CTRL_PARAMS_T *rfc_params)
{
    CsrBool match_dlc = FALSE;
    CsrBool match_mux = FALSE;
    CsrUint8 i;

    CsrMemCpy((void *)rfc_params,
           (void *)context,
           sizeof(RFC_CTRL_PARAMS_T));


    /* Determine which channel the timer was hooked to...
    */
    if(NULL != rfc_params->p_dlc)
    {
        for(i=0; i<RFC_NUM_TIMERS; i++)
        {
            if(rfc_params->p_dlc->timers->contexts[i] == context)
            {
                rfc_params->p_dlc->timers->contexts[i] = NULL;
                match_dlc = TRUE;
                break;
            }
        }
    }

    if(NULL != rfc_params->p_mux)
    {
        for(i=0; i<=RFC_TEST_CONTEXT; i++)
        {
            if(rfc_params->p_mux->timers->contexts[i] == context)
            {
                rfc_params->p_mux->timers->contexts[i] = NULL;
                match_mux = TRUE;
                break;
            }
        }
    }

    if(!match_dlc && !match_mux)
    {
        BLUESTACK_PANIC(
                        CSR_BT_PANIC_RFCOMM_INVALID_TIMER_CONTEXT);
    }

    CsrPmemFree(context);
}

/*! \brief Handler function to determine if RFCOMM is connected to a VM

    If RFCOMM is connected to a VM then all channels being used will be stream
    based.

    \param phandle - the default phandle supplied in rfc_init_req
    \param vm_present(returned) - TRUE/FALSE

*/
static CsrBool rfc_is_vm_connected(CsrSchedQid  phandle)
{
#ifdef INSTALL_VM_MODULE
    return (phandle == VMBS_QUEUE);
#else
    return FALSE;
#endif
}

/*! \brief Handle the receipt of an RFC_INIT_REQ primitive

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the RFC_INIT_REQ primitive

*/
void rfc_handle_init_req(RFC_CTRL_PARAMS_T *rfc_params,
                         RFC_INIT_REQ_T *p_prim)
{
    rfc_params->rfc_ctrl->phandle = p_prim->phandle;

    /* Check if we are connected to a VM or not. If we are then all channels
       will be stream based.
    */
    rfc_params->rfc_ctrl->use_streams = rfc_is_vm_connected(p_prim->phandle);

    L2CA_RegisterReq(RFCOMM_PSM,
                     RFCOMM_IFACEQUEUE,
                     L2CA_MODE_MASK_BASIC | L2CA_MODE_MASK_ENHANCED_RETRANS,
                     0,
                     0);
}

/*! \brief Create a linestatus frame

    Create a linestatus frame (either a command or response) within a MBLK.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - Command or Response
    \param line_status_field - Linestatus data
    \param dlci_field - formatted dlci (address field)
    \param mblk_rls - returned mblk containing the linestatus frame

*/
CsrMblk  *rfc_create_linestatus_cmdrsp(RFC_CTRL_PARAMS_T   *rfc_params,
                                      RFC_CRTYPE          type,
                                      CsrUint8  line_status_field,
                                      CsrUint8  dlci_field)
{
    CsrMblk  *mblk_rls;
    CsrUint8 *rls_frame = NULL;
    CsrUint8  fcs;
    CsrUint8  crbit;
    RFC_LINK_DESIGNATOR from;

    mblk_rls = CsrMblkMallocCreate((void **)&rls_frame, RFC_RLS_LEN);

    if (mblk_rls != NULL)
    {
        /* Create the front end of the UIH data frame which will hold the RLS
           cmd/rsp.
        */
        from = RFC_IS_INITIATOR(rfc_params->p_mux->flags)
               ? RFC_INITIATOR
               : RFC_RESPONDER;

        /* Commands/responses go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &rls_frame,
                              RFC_RLS_FRAME_LEN,
                              from,
                              0);

        /* Get the precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);

        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );

        /* Compose the RLS frame.
        */
        write_uints(
            &rls_frame,
            URW_FORMAT(CsrUint8, 5, UNDEFINED, 0, UNDEFINED, 0),
            RFC_RLS_TYPE | (crbit << 1),
            (RFC_RLS_DATA_LEN << 1) | 0x1,
            dlci_field,
            line_status_field,
            fcs);

    }
    else
    {
        /* Panic??
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_rls;
}

/*! \brief Validate linestatus parameters

    Check the integrity and consistancy of the linestatus values

    \param error_flag - error or no error
    \param line_status_field - Linestatus data

*/
CsrBool rfc_validate_line_status(CsrBool error_flag,
                                CsrUint8 line_status)
{
    if((!error_flag && line_status > 0) ||
       (error_flag &&
        line_status != RFC_OVERRUN_ERROR &&
        line_status != RFC_PARITY_ERROR &&
        line_status != RFC_FRAMING_ERROR))
    {
        return FALSE;
    }

    return TRUE;
}

/*! \brief Handle the receipt of an RFC_LINESTATUS_REQ primitive

    This function handles a linestatus request primitive from the host, composes
    the corresponding linestatus command frame and sends it to the peer.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the RFC_LINESTATUS_REQ primitive

*/
void rfc_handle_linestatus_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_LINESTATUS_REQ_T *p_prim)
{
    CsrMblk  *mblk_rls;
    CsrUint8  line_status_field;
    CsrUint8  dlci_field;
    CsrUint8  error_field;

    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the channel was found then both the p_mux and p_dlc fields will be
       non NULL.
    */
    if(NULL != rfc_params->p_mux &&
       !rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_RLS_CONTEXT]))
    {
        /* A valid connection id has been specified, thus create and send the
           RLS command frame.
        */

        dlci_field = (rfc_params->p_dlc->dlci << 2) |
                     RFC_DEFAULT_CMD_ADDR_BITS;

        /* Validate parameters.
        */
        if(!rfc_validate_line_status(p_prim->error_flag,
                                     (CsrUint8)p_prim->line_status))
        {
            rfc_send_line_status_cfm(rfc_params->p_dlc->phandle,
                                     rfc_params->p_dlc->info.dlc.conn_id,
                                     RFC_INCONSISTENT_PARAMETERS);
            return;

        }

        error_field = (p_prim->error_flag) ? 1 : 0;
        line_status_field = error_field | (p_prim->line_status << 1);

        mblk_rls = rfc_create_linestatus_cmdrsp(rfc_params,
                                                RFC_CMD,
                                                line_status_field,
                                                dlci_field);

        /* Start the response timer for this request */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_dlc->timers->contexts[RFC_RLS_CONTEXT]),
                        RFC_CMD_RESP_TIMER);

         /* Send to L2CAP.
        */
        L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                          0,/* Indicates MBLK interface*/
                          mblk_rls);

        rfc_send_line_status_cfm(rfc_params->p_dlc->phandle,
                                 rfc_params->p_dlc->info.dlc.conn_id,
                                 RFC_SUCCESS);
    }
    else
    {
        /* Invalid channel id specified, reject the command.
        */
        rfc_send_line_status_cfm(rfc_params->rfc_ctrl->phandle,
                                 p_prim->conn_id,
                                 RFC_INVALID_CHANNEL);
    }

}

/*! \brief Look for registered server channel

    This function scans the registered server channel list for the required
    server channel. If a match is found then the entry is returned, otherwise
    NULL is returned.

    \param rfc_params - Pointer to rfcomm instance data.
    \param serv_chan_id - server channel to search for.
    \param p_serv_chan - Pointer to matching server chan entry returned
*/
RFC_SERV_CHAN_T *rfc_get_reg_server_chan(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint8 serv_chan_id)
{
    RFC_SERV_CHAN_T *p_serv_chan;

    for(p_serv_chan = rfc_params->rfc_ctrl->serv_chan_list;
        p_serv_chan != NULL;
        p_serv_chan = p_serv_chan->p_next)
    {
        if(p_serv_chan->id == serv_chan_id)
        {
            return p_serv_chan;
        }
    }

    return NULL;
}

/*! \brief Look for registered server channel using dlci

    This function extracts the server channel and direction bit from the dlci.
    Validates them and then calls the subfunction to scan the registered server
    channel list for the required server channel. If a match is found then the
    entry is returned, otherwise NULL is returned.

    \param rfc_params - Pointer to rfcomm instance data.
    \param dlci - DLC identifier containing server chan to look for
    \param p_serv_chan - Pointer to matching server chan entry returned
*/
RFC_SERV_CHAN_T *rfc_get_reg_server_chan_by_dlci(RFC_CTRL_PARAMS_T *rfc_params,
                                                 CsrUint8 dlci)
{
    CsrUint8 serv_chan_id = RFC_GET_SERV_CHAN_FROM_DLCI(dlci);
    CsrUint8 dirbit = RFC_GET_DIRBIT_FROM_DLCI(dlci);

    /* First check integrity of incoming direction bit. As this is an incoming
       connection the direction bit in the dlci should be the same as ours.
    */
    if(dirbit == RFC_DIRBIT(rfc_params->p_mux->flags))
    {
        /* Incoming dirbit is the same as ours so is valid.
        */
        return rfc_get_reg_server_chan(rfc_params,
                                       serv_chan_id);
    }

    return NULL;
}

/*! \brief Allocate a new unique server channel number

    This function should ONLY be called when we havent already allocated the
    maximum number of server channels (or else we will get stuck in an infinite
    loop)!!

    \param rfc_params - Pointer to rfcomm instance data.
    \param num_reg_chans - Number of currently registered server channels
    \param new_id (returned) - Next available server channel id.
*/
static CsrUint8 rfc_allocate_serv_chan_id(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint8 num_reg_chans)
{
    CsrUint8 new_id = 1;

    if(num_reg_chans > 0)
    {
        /* There is at least one previously registered server channel thus count
           the number currently registered.
        */
        new_id = rfc_params->rfc_ctrl->serv_chan_list->id;

        while(TRUE)
        {
            new_id = new_id < RFC_MAX_NUM_SERVER_CHANNELS
                ? new_id + 1
                : 1;

            if(rfc_get_reg_server_chan(rfc_params, new_id) == NULL)
            {
                /* No registered server channel with this id thus stop
                   scanning.
                */
                break;
            }
        }
    }

    return new_id;
}

/*! \brief Handle a RFC_REGISTER_REQ primitive

    This function is called when a RFC_REGISTER_REQ primitive is received from
    the host. If there are still available server channels then a new one is
    registered and the server channel id returned to the host.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
void rfc_handle_register_req(RFC_CTRL_PARAMS_T *rfc_params,
                             RFC_REGISTER_REQ_T *p_prim)
{
    RFC_SERV_CHAN_T *p;

    CsrUint8 num_reg_chans = 0;
    CsrUint8 new_id;

    /* Count the number of existing, registered server channels.
    */
    for (p = rfc_params->rfc_ctrl->serv_chan_list;
         p != NULL ;
         p = p->p_next)
    {
        num_reg_chans++;
    }

    /* NB The check against RFC_MAX_LOCAL_SERVER_CHANNEL assumes that
       RFC_INVALID_SERV_CHANNEL also has a value in that range. Should the
       value of RFC_INVALID_SERV_CHANNEL change then this test will need to be
       updated!
    */
    if( num_reg_chans < RFC_MAX_NUM_SERVER_CHANNELS &&
        p_prim->loc_serv_chan_req <= RFC_MAX_LOCAL_SERVER_CHANNEL)
    {
        /* Has the host requested to register a specific server channel?
        */
        if(p_prim->loc_serv_chan_req != RFC_INVALID_SERV_CHANNEL)
        {
            /* Specific channel requested - see if it is free or not.
            */
            if(rfc_get_reg_server_chan(rfc_params,
                                       p_prim->loc_serv_chan_req) == NULL)
            {
                /* Server channel is avaliable thus allocate this one.
                */
                new_id = p_prim->loc_serv_chan_req;
            }
            else
            {
                /* Allocate next available serv channel id.
                */
                new_id = rfc_allocate_serv_chan_id(rfc_params,
                                                   num_reg_chans);
            }
        }
        else
        {
            /* Allocate next available serv channel id.
            */
            new_id = rfc_allocate_serv_chan_id(rfc_params,
                                               num_reg_chans);
        }

        p = pnew(RFC_SERV_CHAN_T);

        p->id = new_id;
        p->phandle = p_prim->phandle;
        p->flags = (CsrUint16)p_prim->flags;
        p->context = p_prim->context;

        /* Add new registered server channel to the head of the list.
        */
        p->p_next = rfc_params->rfc_ctrl->serv_chan_list;
        rfc_params->rfc_ctrl->serv_chan_list = p;

        /* Send a confirmation to the host saying that the registration has been
           accepted.
        */
        rfc_send_register_cfm(p_prim->phandle,
                              p->id,
                              p->context,
                              TRUE);
    }
    else
    {
        /* Send a confirmation to the host saying that the registration has been
           rejected.
        */
        rfc_send_register_cfm(p_prim->phandle,
                              0,
                              p_prim->context,
                              FALSE);
    }
}

/*! \brief Handle a RFC_UNREGISTER_REQ primitive

    This function is called when a RFC_UNREGISTER_REQ primitive is received from
    the host. If the supplied server channel is found on the registered list
    then it is removed and confirmation sent to the host.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to received primitive
*/
void rfc_handle_unregister_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_UNREGISTER_REQ_T *p_prim)
{
    RFC_SERV_CHAN_T *p;
    RFC_SERV_CHAN_T **pp;

    for (pp = &(rfc_params->rfc_ctrl->serv_chan_list);
         (p = *pp) != NULL ;
         pp = &(p->p_next) )
    {
        if(p->id == p_prim->loc_serv_chan)
        {
            /* Found server channel to unregister, thus unhook the list element
               and destroy it.
            */
            *pp = p->p_next;

            rfc_send_unregister_cfm(rfc_params->rfc_ctrl->phandle,
                                    p_prim->loc_serv_chan,
                                    p->context,
                                    (RFC_RESPONSE_T)RFC_SUCCESS);

            CsrPmemFree(p);

            return;
        }
    }

    rfc_send_unregister_cfm(rfc_params->rfc_ctrl->phandle,
                            p_prim->loc_serv_chan,
                            0,
                            (RFC_RESPONSE_T)RFC_INVALID_CHANNEL);

}


/*! \brief Create a test frame

    Create a test frame (either a command or response) within a MBLK.
    The test data to be sent within the frame is passed in within its own MBLK,
    thus the frame is constructed of an MBLK chain.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - Command or Response
    \param p_data - Pointer to the MBLK containing the data
    \param mblk_test - Pointer to returned mblk chain containing the test frame

*/
CsrMblk  *rfc_create_test_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                RFC_CRTYPE type,
                                CsrMblk *p_data)
{
    CsrMblk   *mblk_test;
    CsrUint8  *test_frame = NULL;
    CsrMblk   *mblk_fcs;
    CsrUint8  *fcs_frame = NULL;
    CsrUint8  crbit;
    CsrUint8  len_octet;
    CsrBool   extended;
    CsrUint16 len = CsrMblkGetLength(p_data);
    CsrUint8  fcs;

    /* Create a new MBLK to contain the frame headers for the test
       command/response. NB there is a -1 in the length to account for the FCS
       value which will have to be appended at the end in a 2nd MBLK.
    */
    mblk_test = CsrMblkMallocCreate((void **)&test_frame,
                                   RFC_TEST_LEN - 1);

    mblk_fcs = CsrMblkMallocCreate((void **)&fcs_frame, 1);

    if (mblk_test != NULL && mblk_fcs != NULL)
    {
        RFC_LINK_DESIGNATOR from =
            RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

        /* Create the front end of the UIH data frame which will hold the test
           request/response. These go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &test_frame,
                              (CsrUint16)(RFC_CMD_HEADER_LEN + len),
                              from,
                              0);

        /* Compose the TEST frame header itself.
        */
        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );
        write_uint8(&test_frame, (CsrUint8)(RFC_TEST_TYPE | (crbit << 1)));

        /* Partition the data length using the extended addressing format
           detailed in the specification.
        */
        extended = TRUE;
        while( extended )
        {
            len_octet = len & RFC_CMD_MAX_LEN_OCTET_VAL;
            len_octet = len_octet << 1;
            len = len >> RFC_CMD_LEN_SIZE;
            if( len == 0)
            {
                extended = FALSE;
                RFC_SET_EA_BIT(len_octet);
            }
            write_uint8(&test_frame, len_octet);
        }

        /* Append this MBLK to the front of the data one.
        */
        mblk_test = CsrMblkAddHead(mblk_test, p_data);

        /* Get the precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);
        write_uint8(&fcs_frame, fcs);

        mblk_test = CsrMblkAddTail(mblk_fcs, mblk_test);

    }
    else
    {
        /* Out of resources - panic
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_test;
}

/*! \brief Handle a RFC_TEST_REQ primitive

    Create and send a test command frame to the peer device. The test data to be
    sent within the frame is passed in within its own MBLK, thus the frame is
    constructed of an MBLK chain.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_test_req(RFC_CTRL_PARAMS_T *rfc_params,
                         RFC_TEST_REQ_T *p_prim)
{
    CsrMblk   *mblk_test;

    rfc_find_mux_by_bd_addr(rfc_params,
                            &(p_prim->bd_addr));

    /* If the Mux was found then p_mux will be non NULL.
    */
    if(NULL != rfc_params->p_mux &&
       RFC_IS_CHAN_OPEN(rfc_params->p_mux) &&
       !rfc_is_timer_running(rfc_params->p_mux->timers->contexts[RFC_TEST_CONTEXT]))
    {
        /* A valid connection id has been specified, thus create and send the
           TEST command frame.
        */
        mblk_test = rfc_create_test_cmdrsp(rfc_params,
                                           RFC_CMD,
                                           p_prim->test_data);

        /* Unhook test data MBLK from prim.
        */
        p_prim->test_data = NULL;

        /* Start the response timer for this request */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_mux->timers->contexts[RFC_TEST_CONTEXT]),
                        RFC_CMD_RESP_TIMER);

        /* Send to L2CAP.
        */
        L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                          0,/* Indicates MBLK interface*/
                          mblk_test);

    }
    else
    {
        /* Invalid command so reject it.
        */
        rfc_send_test_cfm(rfc_params->rfc_ctrl->phandle,
                          &(p_prim->bd_addr),
                          NULL,
                          RFC_INVALID_CHANNEL);
    }
}



/*! \brief Kick all the DLCs in turn to see if there is pending data to send

    This function will typically be called when flow control has been asserted
    and then de-asserted on a complete rfcomm session. During the intervening
    period data could have queued up within rfcomm (on the internal queue) and
    thus each channel needs to be kicked to ensure that data has a chance of
    being sent.

    \param rfc_params - Pointer to rfcomm instance data.
*/
void rfc_data_kick_all(RFC_CTRL_PARAMS_T  *rfc_params)
{
    RFC_CHAN_T *p_dlc;

    for (p_dlc = rfc_params->p_mux->info.mux.p_dlcs;
         p_dlc != NULL;
         p_dlc = p_dlc->p_next)
    {
        rfc_params->p_dlc = p_dlc;
        rfc_params->p_dlc->info.dlc.vtable->kick_fn(rfc_params, FALSE);
    }
}

/*! \brief Create a modem status frame

    Create a modem status frame (either a command or response) within a MBLK.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - Command or Response
    \param dlci_field - Address field containing the targetted dlci
    \param modem_signal - data to be sent
    \param break_signal - data to be sent
    \param mblk_msc - Pointer to returned mblk chain containing the modem status
                      frame

*/
CsrMblk  *rfc_create_modem_status_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                        RFC_CRTYPE type,
                                        CsrUint8  dlci_field,
                                        CsrUint8  modem_signal,
                                        CsrUint8  break_signal)
{
    CsrMblk  *mblk_msc;
    CsrUint8 *msc_frame = NULL;
    CsrUint8 fcs;
    CsrUint8 crbit;
    CsrUint8 frame_len;
    CsrUint8 msc_len;
    CsrUint8 msc_data_len;
    CsrUint8 modem_ea;

    /* Is there a break signal to send ?
    */
    if(break_signal == 0)
    {
        /* No break signal thus all lengths are one less.
        */
        frame_len = RFC_MSC_FRAME_LEN - 1;
        msc_len = RFC_MSC_LEN - 1;
        msc_data_len = RFC_MSC_DATA_LEN - 1;
        modem_ea = 1;
    }
    else
    {
        frame_len = RFC_MSC_FRAME_LEN;
        msc_len = RFC_MSC_LEN;
        msc_data_len = RFC_MSC_DATA_LEN;
        modem_ea = 0;
    }

    mblk_msc = CsrMblkMallocCreate((void **)&msc_frame,
                                   msc_len);

    if (mblk_msc != NULL)
    {
        RFC_LINK_DESIGNATOR from =
            RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

        /* Create the front end of the UIH data frame which will hold the MSC
           cmd/rsp. These go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &msc_frame,
                              frame_len,
                              from,
                              0);

        /* Get the precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);
        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );

        /* Compose the MSC frame.
        */
        write_uints(
            &msc_frame,
            URW_FORMAT(CsrUint8, 4, UNDEFINED, 0, UNDEFINED, 0),
            RFC_MSC_TYPE | (crbit << 1),
            (msc_data_len << 1) | 0x1,
            dlci_field,
            modem_signal | modem_ea);

        if(break_signal != 0)
        {
            write_uint8(&msc_frame, break_signal);
        }

        write_uint8(&msc_frame, fcs);
    }
    else
    {
        /* Out of resources - panic
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_msc;
}

/*! \brief Sends a RFC_MODEM_STATUS_REQ primitive

    Send a modem status command frame to the peer device. Can be either as part
    of the automatic MSC initialisation sequence or as a result of a request
    from the host.

    \param rfc_params - Pointer to rfcomm instance data.
    \param dlci - Targetted dlci
    \param modem_signal - data to be sent
    \param break_signal - data to be sent
*/
void rfc_send_modem_status_req(RFC_CTRL_PARAMS_T *rfc_params,
                               CsrUint8  dlci,
                               CsrUint8  modem_signal,
                               CsrUint8  break_signal)
{
    CsrMblk   *mblk_msc;
    CsrUint8  dlci_field;

    dlci_field = (dlci << 2) | RFC_DEFAULT_CMD_ADDR_BITS;

    mblk_msc = rfc_create_modem_status_cmdrsp(rfc_params,
                                              RFC_CMD,
                                              dlci_field,
                                              modem_signal,
                                              break_signal);


    /* Start the response timer for this request */
    rfc_timer_start(rfc_params,
                    &(rfc_params->p_dlc->timers->contexts[RFC_MSC_CONTEXT]),
                    RFC_CMD_RESP_TIMER);

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_msc);

}

/*! \brief Update MSC flow control status

    This function checks the flow control bit in the modem signal and updates
    the internal flow control flag accordingly.

    \param flags - Current connection flags (modified by fn).
    \param msc_flag - TX or RX flag to be updated
    \param modem_signal - data to be sent
*/
void rfc_update_msc_fc_status(CsrUint16 *flags,
                              CsrUint16 mux_flag,
                              CsrUint16 msc_flag,
                              CsrUint8  modem_signal)
{
    /* Update flow control status if applicable.
    */
    if (!RFC_CREDIT_FC_USED(mux_flag))
    {
        if (modem_signal & RFC_MSC_FC_BIT)
        {
            RFC_RESET_MSC_FC_ENABLED_FLAG(*flags, msc_flag);
        }
        else
        {
            RFC_SET_MSC_FC_ENABLED_FLAG(*flags, msc_flag);
        }
    }
}

/*! \brief Validate modem status parameters

    This function checks that the supplied MSC parameters are valid.
    Also checks if there is a break_signal present and if so updates the
    formatting ready for transmission across the air.

    \param flags - Current connection flags .
    \param modem_signal - data to be sent
    \param break_signal - data to be sent (modified by fn)
    \param valid - TRUE if params are valid, FALSE o/w
*/
CsrBool rfc_validate_msc_params(CsrUint16 flags,
                               CsrUint8  modem_signal,
                               CsrUint8  *break_signal)
{
    if ((modem_signal & RFC_MSC_FC_BIT &&
        RFC_CREDIT_FC_USED(flags)) ||
        ((*break_signal >> RFC_MSC_BREAK_VAL_SHIFT) > RFC_MSC_MAX_BREAK_VAL))
    {
        return FALSE;
    }

    if(RFC_MSC_IS_BREAK_ENCODED(*break_signal))
    {
        *break_signal = *break_signal << 1;
        RFC_SET_EA_BIT(*break_signal);
    }

    return TRUE;
}

/*! \brief Handle a RFC_MODEM_STATUS_REQ primitive

    Create and send a modem status command frame to the peer device.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_modem_status_req(RFC_CTRL_PARAMS_T *rfc_params,
                                 RFC_MODEM_STATUS_REQ_T *p_prim)
{
    CsrUint8  break_signal = p_prim->break_signal;

    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the connection was found then p_mux and p_dlc will be non NULL. Check
       to see if there is already an outstanding MSC req as it is illegal to
       have more than one pending across the air. The DLC should also be open
       prior to changing MSC settings.
    */
    if(NULL != rfc_params->p_dlc && RFC_IS_CHAN_OPEN(rfc_params->p_dlc))
    {
         /* There might be a case when the MSC timeout is expired and we haven't got a
             response for an outstanding MSC request, this request is as a consequence
             of a RFC_CLIENT_CONNECT_CFM sent to the application. We would queue
             the incoming RFC_<ODEM_STATUS_REQ from the application.
        */
        if(!rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_MSC_CONTEXT]))
        {
            /* A valid connection id has been specified, thus create and send the
                MSC command frame. First ensure that an invalid flow control mode is
                not being used. If we are using credit based flow control then
                setting the FC bit is illegal.
            */
            if(!rfc_validate_msc_params(rfc_params->p_mux->flags,
                                         p_prim->modem_signal,
                                         &break_signal))
            {
                 /* This is illegal, send a cfm with an error status.
                 */
                  rfc_send_msc_cfm(rfc_params->p_dlc->phandle,
                                 p_prim->conn_id,
                                 RFC_INCONSISTENT_PARAMETERS);
                  return;
            }

            rfc_send_modem_status_req(rfc_params,
                                      rfc_params->p_dlc->dlci,
                                      p_prim->modem_signal,
                                      break_signal);

            rfc_update_msc_fc_status(&(rfc_params->p_dlc->flags),
                                     rfc_params->p_mux->flags,
                                     (CsrUint16)RFC_MSC_FC_RX_ENABLED,
                                     p_prim->modem_signal);
         }
         else
         {
             /* In case RFC_MSC_CONTEXT timer is running and a new request
                  comes in for a DLCI channel which is open, we would queue it.
             */
             rfc_queue_prim(rfc_params->rfc_ctrl,
                             (RFCOMM_UPRIM_T *) p_prim,
                             sizeof(RFC_MODEM_STATUS_REQ_T));
         }
    }
    else
    {
        rfc_send_msc_cfm(rfc_params->rfc_ctrl->phandle,
                         p_prim->conn_id,
                         RFC_INVALID_CHANNEL);
    }
}


/*! \brief Remove and delete a queued data packet

    \param p_dlc - Pointer to the channel for which the data is destined.
*/
static void rfc_remove_queued_data_packet(RFC_CHAN_T *p_dlc)
{
    RFC_QUEUED_DATA_T *p = p_dlc->info.dlc.p_data_q->p_next;

    p_dlc->info.dlc.p_data_q->payload = NULL;
    CsrPmemFree(p_dlc->info.dlc.p_data_q);
    p_dlc->info.dlc.p_data_q = p;

}

/*! \brief Queue a received data packet

    If a data packet (host or stream sourced) cannot be immediately/fully
    processed on receipt it is added to an internal queue.

    \param p_dlc - Pointer to the channel for which the data is destined.
    \param payload - pointer to the MBLK containing the data
*/
static void rfc_queue_data_packet(RFC_CHAN_T *p_dlc,
                                  CsrMblk *payload,
                                  CsrBool is_stream)
{
    RFC_QUEUED_DATA_T *p;
    RFC_QUEUED_DATA_T **pp;
    RFC_QUEUED_DATA_T *new_p;

    pp = &(p_dlc->info.dlc.p_data_q);

    if(NULL != *pp)
    {
        /* Find end of queued data.
        */
        for (p = *pp; p->p_next != NULL ; p = p->p_next)
        {
            ;
        }

        /* p now points to the last element in the q.
        */
        pp = &(p->p_next);

        /* Now determine what action to take. If this channel is a stream then we
           may be able to combine message frags to save a tx credit. If it's not a
           stream, or the queue is currently empty, then we just create a new
           queue element. If the mblk coalesce is successful then the length
           returned will be the sum of the two payloads and we can just return.
           If it fails then we have a problem.
        */
        if(is_stream)
        {
            CsrUint16 exp_new_len =
                CsrMblkGetLength(p->payload) + CsrMblkGetLength(payload);

            if(CsrMblkCoalesce(p->payload, &payload) == exp_new_len)
            {
                return;
            }
            else
            {
                BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_MSGFRAG_COALESCE_FAILURE);
            }
        }
    }

    /* Create a new queue element and add this payload to it.
    */
    new_p = pnew(RFC_QUEUED_DATA_T);
    new_p->p_next = NULL;
    new_p->payload = payload;

    *pp = new_p;
}


/*! \brief Handle receipt of a new data packet

    This function is called when a data kick has been received from either the
    host (by receipt of a datawrite req) or from the stream subsystem. The kick
    may have been just for credits, just for data or both. If data is present
    then it is added to the data queue. The main process data function is then
    called.

    \param rfc_params - Pointer to rfcomm instance data.
    \param payload - pointer to the MBLK containing the raw data
*/
void rfc_new_data_packet(RFC_CTRL_PARAMS_T *rfc_params,
                         CsrMblk *payload, CsrBool credit_frame_send)
{
    if( NULL != payload )
    {
        rfc_queue_data_packet(rfc_params->p_dlc,
                              payload,
                              rfc_params->rfc_ctrl->use_streams);
    }

     /* Kick the rfc_process_txdata, it will take care of all the cases
      */
    rfc_process_txdata(rfc_params, credit_frame_send);
}

/*! \brief Send a raw data UIH frame to the peer

    \param rfc_params - Pointer to rfcomm instance data.
    \param payload_len - Size of actual raw data. Will be 0 if a pure credit
                         frame is required.
    \param rx_credits - Number of credits to sent to the peer
    \param p_data - pointer to the MBLK containing the raw data
*/
void rfc_send_raw_uih_frame(RFC_CTRL_PARAMS_T *rfc_params,
                            CsrUint16 payload_len,
                            CsrUint8 rx_credits,
                            CsrMblk   *p_data)
{
    CsrMblk   *mblk_data;
    CsrUint8  *data_frame = NULL;
    CsrMblk   *mblk_fcs;
    CsrUint8  *fcs_frame = NULL;
    CsrUint8  frame_len = RFC_MIN_FRAME_LEN;
    CsrUint8  fcs;
    CsrUint16 conn_id = 0;
    RFC_LINK_DESIGNATOR from =
        RFC_IS_INITIATOR(rfc_params->p_mux->flags)
        ? RFC_INITIATOR
        : RFC_RESPONDER;

    if(payload_len > 0)
    {
        conn_id = rfc_params->p_dlc->info.dlc.conn_id;
        if(payload_len <= RFC_CMD_MAX_LEN_OCTET_VAL)
        {
            /* If sending data then the FCS will be put in a separate MBLK and
               appended to the data, thus reduce the default frame length.
               However if the payload length is >7 bits worth then the frame
               length would be increased by one byte and thus the addition and
               subtraction cancel each other out.
            */
            frame_len --;
        }
    }

    if(rx_credits > 0)
    {
        frame_len ++; /* For the credit field */

        /* Precalculated fcs for a data frame with credits
        */
        fcs = RFC_FCS_UIH_PF(rfc_params->p_dlc->fcs_out);

        /* If we are sending credits then we can cancel any outstanding credit
           timer.
        */
        rfc_timer_cancel(&(rfc_params->p_dlc->timers->contexts[RFC_CREDIT_CONTEXT]));
    }
    else
    {
        /* Precalculated fcs for a pure data frame
        */
        fcs = RFC_FCS_UIH(rfc_params->p_dlc->fcs_out);
    }

    mblk_data = CsrMblkMallocCreate((void **)&data_frame,
                                   frame_len);

    if (NULL == mblk_data)
    {
        /* PANIC , memory exhaustion!
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    /* Create the front end of the UIH data frame.
    */
    rfc_create_uih_header(rfc_params->p_dlc->dlci,
                          &data_frame,
                          payload_len,
                          from,
                          rx_credits);

    if(payload_len > 0)
    {
        /* Append this MBLK to the front of the data one.
        */
        mblk_data = CsrMblkAddHead(mblk_data, p_data);

        mblk_fcs = CsrMblkMallocCreate((void **)&fcs_frame, 1);
        if (NULL == mblk_fcs)
        {
            /* PANIC , memory exhaustion!
            */
            BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
        }
        write_uint8(&fcs_frame, fcs);
        mblk_data = CsrMblkAddTail(mblk_fcs, mblk_data);
    }
    else
    {
        write_uint8(&data_frame, fcs);
    }

    /* Send to L2CAP. Conn id sent as user context in order to track
       datawrite confirms for raw data packets.
    */
    L2CA_DataWriteReqEx(rfc_params->p_mux->info.mux.cid,
                        0,/* Indicates MBLK interface*/
                        mblk_data,
                        conn_id);

#ifdef BUILD_FOR_HOST
    /* If this is an off chip host stack and this packet contains data (ie not a
       credit only one) then inform the host that a data packet has been sent to
       L2CAP.
    */
    if(payload_len > 0)
    {
        rfc_send_datawrite_cfm(rfc_params->p_dlc->phandle,
                               rfc_params->p_dlc->info.dlc.conn_id,
                               RFC_DATAWRITE_PENDING,
                               FALSE);
    }
#endif
}

/*! \brief Send a credit only data frame to the peer

    Create and send a credit only data packet to the peer device providing there
    are some credits available.

    \param rfc_params - Pointer to rfcomm instance data.
    \param send_threshold - number of credits which will trigger the send
    \param sent - TRUE if the frame was sent
*/
static CsrBool rfc_send_credit_only_frame(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint16 send_threshold)
{
    if(rfc_params->p_dlc->info.dlc.rx_credits > send_threshold)
    {
        CsrUint8 rx_credits_to_send =
            CSRMIN(RFC_MAX_RX_CREDITS_PER_FRAME, rfc_params->p_dlc->info.dlc.rx_credits);

        rfc_send_raw_uih_frame(rfc_params,
                               0,
                               rx_credits_to_send,
                               NULL);
        rfc_params->p_dlc->info.dlc.rx_credits -= rx_credits_to_send;
        return TRUE;
    }

    return FALSE;
}


/*! \brief Handle a RFC_DATAWRITE_REQ primitive

    This function validates a datawrite request from the host and if there is
    valid data and/or credits to send to the peer device then it is passed to
    the tx data processing function.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_data - Pointer to the data to be sent
*/
void rfc_handle_datawrite_req(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_DATAWRITE_REQ_T *prim)
{
    RFC_RESPONSE_T status = RFC_INVALID_PAYLOAD;
    CsrUint16 payload_len = CsrMblkGetLength(prim->payload) ;
    CsrBool credit_frame_send = FALSE;

    if(prim->rx_credits > 0 || payload_len > 0)
    {
        rfc_find_chan_by_id(rfc_params,
                            prim->conn_id);

        if( NULL != rfc_params->p_dlc )
        {
            /* Check the payload length against the negotiated max frame
                size
            */
            if( payload_len <= rfc_params->p_dlc->info.dlc.max_frame_size)
            {
                rfc_params->p_dlc->info.dlc.rx_credits += prim->rx_credits;

                if(!payload_len)
                {
                   credit_frame_send = TRUE;
                }

                rfc_new_data_packet(rfc_params,
                                prim->payload, credit_frame_send);
                prim->payload = NULL;
                return;
            }
        }
        else
        {
            status = RFC_INVALID_CHANNEL;
        }
    }

    CsrMblkDestroy(prim->payload);

    rfc_send_datawrite_cfm(rfc_params->rfc_ctrl->phandle,
                           prim->conn_id,
                           status,
                           rfc_params->rfc_ctrl->use_streams);

}

/*! \brief Is there any queued data

    \param rfc_params - Pointer to rfcomm instance data.
    \params data_size(returned) - Amount of data stored at the head of the
                                  queue.
*/
static CsrUint16 rfc_is_data_queued(RFC_CTRL_PARAMS_T *rfc_params)
{
    if(NULL != rfc_params->p_dlc->info.dlc.p_data_q)
    {
        return(CsrMblkGetLength(rfc_params->p_dlc->info.dlc.p_data_q->payload));
    }

    return 0;
}

/*! \brief Process data from the Host or Stream interfaces

    This function will try and process any currently queued data for as long as
    there are tx credits available.

    \param rfc_params - Pointer to rfcomm instance data.
*/
void rfc_process_txdata(RFC_CTRL_PARAMS_T *rfc_params, CsrBool credit_frame_send)
{
    CsrUint8 rx_credits_to_send = 0;
    CsrUint16 packet_len = 0;
    CsrMblk *p_data_new = NULL;
    CsrMblk *p_data;
    CsrUint16 payload_len;
    CsrBool data_frame_sent = FALSE;

    while( (payload_len = rfc_is_data_queued(rfc_params)) > 0)
    {
        p_data = rfc_params->p_dlc->info.dlc.p_data_q->payload;

        if (RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
        {
            /* Check whether we have Tx credits available on this DLCI
               to transmit the data.
            */
            if(rfc_params->p_dlc->info.dlc.tx_credits != 0)
            {
                /* Calculate how many credits to return to the peer. We can do this
                   here irrespective of whether credit flow control is being used or
                   not . If it's not then the result will be 0 anyway.
                */
                rx_credits_to_send =
                    (CsrUint8)(CSRMIN(RFC_MAX_RX_CREDITS_PER_FRAME, rfc_params->p_dlc->info.dlc.rx_credits));

                rfc_params->p_dlc->info.dlc.tx_credits -- ;
            }
            else
            {
                /* Just pass the control to the place where we can send a
                   credit only frame
                */
                break;
            }
        }
        else
        {
            if(!RFC_IS_FC_TX_ENABLED(rfc_params->p_mux->flags) ||
               !RFC_IS_MSC_FC_TX_ENABLED(rfc_params->p_dlc->flags))
            {
                /* Flow control is asserted so process nothing.
                */
                return;
            }
        }

        /* Do we need to partition the current payload into max frame size
           packets?
        */
        if(payload_len > rfc_params->p_dlc->info.dlc.max_frame_size )
        {
            packet_len = rfc_params->p_dlc->info.dlc.max_frame_size;

            /* Split the MBLK here. The new mblk will point to the data
               after the split point and thus the mblk pointers need to be
               manipulated in order to send the correct block and also be
               correctly positioned for the next time round the loop.
            */
            p_data_new = CsrMblkSplit(p_data, packet_len);
            rfc_params->p_dlc->info.dlc.p_data_q->payload = p_data_new;
        }
        else
        {
            packet_len = payload_len;

            /* remove payload from head of the queue as it has been fully
               processed.
            */
            rfc_remove_queued_data_packet(rfc_params->p_dlc);
        }

        rfc_send_raw_uih_frame(rfc_params,
                               packet_len,
                               rx_credits_to_send,
                               p_data);

        rfc_params->p_dlc->info.dlc.rx_credits -= rx_credits_to_send;
        data_frame_sent = TRUE;
    }

    if(!data_frame_sent)
    {
        CsrUint16 thresh =
            rfc_params->p_dlc->info.dlc.allocated_rx_credits / RFC_CREDIT_SEND_THRESHOLD;

        if(credit_frame_send)
        {
            thresh = 0;
        }
        /* Try to send a credit only frame, for DATAWRITES is payload size 0
             a credit only frame is immediatly sent.
        */
        rfc_try_send_credit_only_frame(rfc_params, thresh);
     }

}

/*! \brief try to send a credit only frame

    This function will try to send a credit only frame based on watermark
    if it is unable to send then it starts a timer of 500ms to send the
    credit only frame afterwards unconditionally.

    \param rfc_params - Pointer to rfcomm instance data.
    \param thresh        - water mark for sending the credit only frame
*/
void rfc_try_send_credit_only_frame(RFC_CTRL_PARAMS_T *rfc_params, CsrUint16 thresh)
{
    /* Check if we have hit the credit threshold which triggers a
         credit only data frame to be sent to the peer.
    */
    if(rfc_params->p_dlc != NULL)
    {
       if(!rfc_send_credit_only_frame(rfc_params, thresh))
       {
           /* If we have credits that werent sufficient to trigger the
                threshold and we dont have a timer currently running, then
                start one.
           */
           if(NULL == rfc_params->p_dlc->timers->contexts[RFC_CREDIT_CONTEXT])
           {
               /* Start credit timer of 500ms, we will send a credit frame anyway on its expiry.
                    if a frame with credit is sent from somewhere else this timer is stopped.
               */
               rfc_timer_start(rfc_params,
                           &(rfc_params->p_dlc->timers->contexts[RFC_CREDIT_CONTEXT]),
                           RFC_CREDIT_RETURN_TIMER);
            }
        }
    }
}

/*! \brief Handle a RFC_DATAREAD_RSP primitive

    Handles a dataread response from the host. This indicates that the previous
    data sent has been used and thus a credit can be released (if credit based
    flow control is being used).

    \param rfc_params - Pointer to rfcomm instance data.
    \param conn_id - connection id of the channel for which the dataread rsp
                     was issued.
*/
void rfc_handle_dataread_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                             CsrUint16 conn_id)
{
    rfc_find_chan_by_id(rfc_params, conn_id);

    if(NULL != rfc_params->p_dlc)
    {
        /* Response has been received for a valid channel thus update the
           credits if required.
        */
        if(RFC_CREDIT_FC_USED(rfc_params->p_mux->flags))
        {
            CsrUint16 thresh =
            rfc_params->p_dlc->info.dlc.allocated_rx_credits / RFC_CREDIT_SEND_THRESHOLD;

            rfc_params->p_dlc->info.dlc.rx_credits ++;

            rfc_try_send_credit_only_frame(rfc_params, thresh);
        }
    }
}

/*! \brief Create a FC ON/OFF frame

    Create a flow control frame (either a command or response) within a MBLK.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - Command or Response
    \param fc_type - fc_on or fc_off
    \param mblk_fc - Pointer to returned mblk chain containing the fc on/off
                     frame

*/
static CsrMblk  *rfc_create_fc_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                     RFC_CRTYPE type,
                                     RFC_FC_T  fc_type)
{
    CsrMblk   *mblk_fc;
    CsrUint8  *fc_frame = NULL;
    CsrUint8  len_field;
    CsrUint8  fcs;
    CsrUint8  crbit;

    mblk_fc = CsrMblkMallocCreate((void **)&fc_frame,
                                  RFC_FC_LEN);

    if (mblk_fc != NULL)
    {
        RFC_LINK_DESIGNATOR from =
            RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

        /* NB. This command instructs the peer device to either stop
           sending or restart sending of data frames. Should the peer
           ignore the command then we will continue to process the data
           and let the host decide what further course of action to take.
        */

        /* Create the front end of the UIH data frame which will hold the fc
           request. These go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &fc_frame,
                              RFC_FC_FRAME_LEN,
                              from,
                              0);

        /* Compose the FC frame.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);
        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );

        write_uint8(&fc_frame, (CsrUint8)(fc_type | (crbit << 1)));

        /* No associated data with this request thus the length field is 0
           (but still needs EA bit set).
        */
        len_field = 0;
        RFC_SET_EA_BIT(len_field);

        write_uint8(&fc_frame, len_field);
        write_uint8(&fc_frame, fcs);
    }
    else
    {
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_fc;
}

/*! \brief Send a FC ON/OFF frame to the peer

    This function is called when a FC_ON or FC_OFF frame is to be sent to the
    peer device.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - Command or Response
    \param fc_type - fc_on or fc_off
*/
void rfc_send_fc_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                        RFC_CRTYPE  type,
                        RFC_FC_T  fc_type)
{
    CsrMblk   *mblk_fc = rfc_create_fc_cmdrsp(rfc_params, type, fc_type);
    RFC_TIMERS_T  context = RFC_FCOFF_CONTEXT;

    if(type == RFC_CMD)
    {
        if(fc_type == RFC_FC_ON)
        {
            context = RFC_FCON_CONTEXT;
        }

        /* Start the response timer for this request */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_mux->timers->contexts[context]),
                        RFC_CMD_RESP_TIMER);
    }

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_fc);

}


/*! \brief Handle a RFC_FC_REQ primitive

    Create and send a FC (flow control) command frame to the peer device. Will
    either be FC_ON or FC_OFF depending on the type specified in the prim.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_fc_req(RFC_CTRL_PARAMS_T *rfc_params,
                       RFC_FC_REQ_T *p_prim)
{
    CsrBool reject;

    rfc_find_mux_by_bd_addr(rfc_params,
                            &(p_prim->bd_addr));

    /* If the Mux was found then p_mux will be non NULL.
    */
    if(NULL != rfc_params->p_mux &&
       RFC_IS_CHAN_OPEN(rfc_params->p_mux) &&
       !(RFC_CREDIT_FC_USED(rfc_params->p_mux->flags)))
    {
        reject = ((p_prim->fc == RFC_FC_ON) &&
                 (rfc_is_timer_running(rfc_params->p_mux->timers->contexts[RFC_FCON_CONTEXT]))) ||
                 ((p_prim->fc == RFC_FC_OFF) &&
                 (rfc_is_timer_running(rfc_params->p_mux->timers->contexts[RFC_FCOFF_CONTEXT])));

        if(!reject)
        {
            if((p_prim->fc != RFC_FC_ON && p_prim->fc != RFC_FC_OFF) ||
               (p_prim->fc == RFC_FC_ON && RFC_IS_FC_RX_ENABLED(rfc_params->p_mux->flags)) ||
               (p_prim->fc == RFC_FC_OFF && !RFC_IS_FC_RX_ENABLED(rfc_params->p_mux->flags)))
            {
                /* The command is invalid for the current state so reject
                   it.
                */
                rfc_send_fc_cfm(rfc_params->p_mux->phandle,
                                &(p_prim->bd_addr),
                                p_prim->fc,
                                RFC_INCONSISTENT_PARAMETERS);
                return;
            }

            rfc_send_fc_cmdrsp(rfc_params, RFC_CMD, p_prim->fc);

            rfc_send_fc_cfm(rfc_params->p_mux->phandle,
                            &(p_prim->bd_addr),
                            p_prim->fc,
                            RFC_SUCCESS);

            RFC_TOGGLE_FC_RX_ENABLED(rfc_params->p_mux->flags);
            return;
        }
    }

    rfc_send_fc_cfm(rfc_params->rfc_ctrl->phandle,
                    &(p_prim->bd_addr),
                    p_prim->fc,
                    RFC_INVALID_CHANNEL);
}

/*! \brief Create a portneg frame

    Creates a portneg command or response within an mblk and returns that to the
    caller.

    \param rfc_params - Pointer to rfcomm instance data.
    \param from - Initiator or Responder
    \param type - Command or Response
    \param mblk_portneg - returned mblk containing portneg frame
*/
static CsrMblk  *rfc_create_portneg_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                          RFC_LINK_DESIGNATOR from,
                                          RFC_CRTYPE type)
{
    CsrMblk  *mblk_portneg;
    CsrUint8 *portneg_frame = NULL;
    CsrUint8  crbit;
    CsrUint8  dlci_field;
    CsrUint8  data_field;
    CsrUint8  rpn_len;
    CsrUint8  rpn_frame_len;
    CsrUint8  rpn_data_len;
    CsrUint8  fcs;

    if(rfc_params->p_dlc->info.dlc.p_temp_rpn->request)
    {
        rpn_len = RFC_RPN_REQUEST_LEN;
        rpn_frame_len = RFC_RPN_REQUEST_FRAME_LEN;
        rpn_data_len =  RFC_RPN_REQUEST_DATA_LEN << 1;
    }
    else
    {
        rpn_len = RFC_RPN_LEN;
        rpn_frame_len = RFC_RPN_FRAME_LEN;
        rpn_data_len =  RFC_RPN_DATA_LEN << 1;
    }
    RFC_SET_EA_BIT(rpn_data_len);

    /* Create PORTNEG frame.
    */
    mblk_portneg = CsrMblkMallocCreate((void **)&portneg_frame, rpn_len);

    if (mblk_portneg != NULL)
    {
        /* Create the front end of the UIH data frame which will hold the
           portneg frame. These go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &portneg_frame,
                              rpn_frame_len,
                              from,
                              0);

        /* Get precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);

        crbit = rfc_calc_crbit( from, type, RFC_MSGLEVEL );
        dlci_field =
            (rfc_params->p_dlc->dlci << 2) |
            RFC_DEFAULT_CMD_ADDR_BITS;

        /* Compose the RPN frame.
        */
        if(rfc_params->p_dlc->info.dlc.p_temp_rpn->request)
        {
            write_uints(
                &portneg_frame,
                URW_FORMAT(CsrUint8, 3, UNDEFINED, 0, UNDEFINED, 0),
                RFC_RPN_TYPE | (crbit << 1),
                rpn_data_len,
                dlci_field);
            write_uint8(&portneg_frame, fcs);
        }
        else
        {
            /* The data field has the following format:

               |  7  6  |   5   4   |   3  |    2    |  1  0   |
               |reserved|parity type|parity|stop bits|data bits|
            */
            data_field =
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.data_bits & 0x3;
            data_field |= ((rfc_params->p_dlc->info.dlc.p_temp_rpn->params.stop_bits & 0x1) << 2);
            data_field |= ((rfc_params->p_dlc->info.dlc.p_temp_rpn->params.parity & 0x1) << 3);
            data_field |= ((rfc_params->p_dlc->info.dlc.p_temp_rpn->params.parity_type & 0x3) << 4);

            write_uints(
                &portneg_frame,
                URW_FORMAT(CsrUint8, 4, CsrUint8, 4, CsrUint16, 1),
                RFC_RPN_TYPE | (crbit << 1),
                rpn_data_len,
                dlci_field,
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.baud_rate,
                data_field,
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.flow_ctrl_mask,
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.xon,
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.xoff,
                rfc_params->p_dlc->info.dlc.p_temp_rpn->params.parameter_mask);
            write_uint8(&portneg_frame, fcs);
        }
    }
    else
    {
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_portneg;
}

/*! \brief Handle a portneg command/response event

    This function is called when a RPN frame is to be sent to the peer device.

    \param rfc_params - Pointer to rfcomm instance data.
    \param type - command or response
*/
static void rfc_send_port_neg_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                     RFC_CRTYPE  type)
{
    CsrMblk  *mblk_portneg;
    RFC_LINK_DESIGNATOR from;

    from = RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

    mblk_portneg = rfc_create_portneg_cmdrsp(rfc_params,
                                             from,
                                             type);

    if(type == RFC_CMD)
    {
        /* Start the response timer for this request */
        rfc_timer_start(rfc_params,
                        &(rfc_params->p_dlc->timers->contexts[RFC_RPN_CONTEXT]),
                        RFC_CMD_RESP_TIMER);
    }

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_portneg);

}


/*! \brief Handle a RFC_PORTNEG_REQ primitive

    Create and send a portneg request to the peer device.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_portneg_req(RFC_CTRL_PARAMS_T *rfc_params,
                            RFC_PORTNEG_REQ_T *p_prim)
{
    RFC_RPN_VALUES_T  *rpn_values;

    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the connection was found then p_mux and p_dlc will be non NULL.
    */
    if(NULL != rfc_params->p_dlc &&
       !rfc_is_timer_running(rfc_params->p_dlc->timers->contexts[RFC_RPN_CONTEXT]))
    {
        /* Compose the Portneg parameters and send to the peer.
        */
        rpn_values = pnew(RFC_RPN_VALUES_T);
        rpn_values->dlci = rfc_params->p_dlc->dlci;
        rpn_values->request = p_prim->request;
        rpn_values->params = p_prim->port_pars;

        rfc_params->p_dlc->info.dlc.p_temp_rpn = rpn_values;

        if(RFC_IS_CHAN_OPEN(rfc_params->p_dlc))
        {
            rfc_send_port_neg_cmdrsp(rfc_params, RFC_CMD);
        }
    }
    else
    {
        rfc_send_portneg_cfm(rfc_params->rfc_ctrl->phandle,
                             p_prim->conn_id,
                             NULL,
                             RFC_INVALID_CHANNEL);
    }

}

/*! \brief Handle a RFC_PORTNEG_RSP primitive

    Process a portneg response received from the host. If the response is
    received for an unknown channel it is just ignored.

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_portneg_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                            RFC_PORTNEG_RSP_T *p_prim)
{
    RFC_RPN_VALUES_T *rpn_values;

    /* Check if this is a response for a known DLC.
    */
    rfc_find_chan_by_id(rfc_params, p_prim->conn_id);

    /* If the channel was found then both the p_mux and p_dlc fields will be
       non NULL.
    */
    if(NULL != rfc_params->p_dlc)
    {
        /* Compose the Portneg parameters and send to the peer.
        */
        rpn_values = pnew(RFC_RPN_VALUES_T);
        rpn_values->dlci = rfc_params->p_dlc->dlci;
        rpn_values->request = FALSE;
        rpn_values->params = p_prim->port_pars;

        rfc_params->p_dlc->info.dlc.p_temp_rpn = rpn_values;

        rfc_send_port_neg_cmdrsp(rfc_params, RFC_RSP);

        /* It is now safe to remove the stored portneg values.
        */
        CsrPmemFree(rfc_params->p_dlc->info.dlc.p_temp_rpn);
        rfc_params->p_dlc->info.dlc.p_temp_rpn = NULL;

        /* Check if there is an outstanding parneg due to queued configuration
           messages received.
        */
        if(NULL != rfc_params->p_dlc->info.dlc.p_temp_pn)
        {
            crank_channel_fsm(rfc_params, NULL, RFC_EV_PARNEG_IND);
        }
    }
}


/*! \brief Create a nsc frame

    Create a nsc frame response within a MBLK.

    \param rfc_params - Pointer to rfcomm instance data.
    \param command  - The unsupported command type (including EA bit)
    \param cmd_res -  The command/response type of the unsupported command
    \param mblk_nsc - Pointer to returned mblk containing the nsc frame

*/
static CsrMblk  *rfc_create_nsc_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                                   CsrUint8 command,
                                   CsrUint8 cmd_res)
{
    CsrMblk  *mblk_nsc;
    CsrUint8 *nsc_frame = NULL;
    CsrUint8 fcs;
    CsrUint8 value;
    CsrUint8 length;
    CsrUint8 crbit;

    /* Create a new MBLK to contain the frame header for the nsc response.
    */
    mblk_nsc = CsrMblkMallocCreate((void **)&nsc_frame,
                                   RFC_NSC_LEN);

    if (mblk_nsc != NULL)
    {
        RFC_LINK_DESIGNATOR from =
            RFC_IS_INITIATOR(rfc_params->p_mux->flags)
            ? RFC_INITIATOR
            : RFC_RESPONDER;

        /* Create the front end of the UIH data frame which will hold the nsc
           response. These go on dlci 0 (ie Mux) and no credits are
           sent on those.
        */
        rfc_create_uih_header(0,
                              &nsc_frame,
                              RFC_NSC_FRAME_LEN,
                              from,
                              0);

        /* Compose the NSC frame header itself.
        */
        crbit = rfc_calc_crbit( from, RFC_RSP, RFC_MSGLEVEL );
        write_uint8(&nsc_frame, (CsrUint8)(RFC_NSC_TYPE | crbit << 1));

        length = RFC_NSC_DATA_LEN << 1;
        RFC_SET_EA_BIT(length);
        write_uint8(&nsc_frame, length);

        /* Write the NSC value byte.
        */
        value = command | cmd_res << 1 ;
        write_uint8(&nsc_frame, value);

        /* Get the precalculated checksum.
        */
        fcs = RFC_FCS_UIH(rfc_params->p_mux->fcs_out);
        write_uint8(&nsc_frame, fcs);

    }
    else
    {
        /* Out of resources - panic
        */
        BLUESTACK_PANIC(CSR_BT_PANIC_MBLK_CREATE_FAILURE);
    }

    return mblk_nsc;
}


/*! \brief Send a NSC response

    This function is called when a NSC frame is to be sent to the peer device.

    \param rfc_params - Pointer to rfcomm instance data.
    \param command  - The unsupported command type (including EA bit)
    \param cmd_res -  The command/response type of the unsupported command
*/
void rfc_send_nsc_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                      CsrUint8 command,
                      CsrUint8 cmd_res)
{
    CsrMblk  *mblk_nsc;

    mblk_nsc = rfc_create_nsc_rsp(rfc_params,
                                  command,
                                  cmd_res);

    /* Send to L2CAP.
    */
    L2CA_DataWriteReq(rfc_params->p_mux->info.mux.cid,
                      0,/* Indicates MBLK interface*/
                      mblk_nsc);

}

#ifdef INSTALL_AMP_SUPPORT

/*! \brief Handle a move L2CAP channel request

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_l2ca_move_req(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_L2CA_MOVE_CHANNEL_REQ_T *p_prim)
{
    rfc_find_mux_by_bd_addr(rfc_params,
                            &(p_prim->bd_addr));

    /* If the Mux was found then p_mux will be non NULL.
    */
    if(NULL != rfc_params->p_mux)
    {
        /* Create the associated L2CAP primitive.
        */
        L2CA_MoveChannelReq(rfc_params->p_mux->info.mux.cid,
                            p_prim->remote_control,
                            p_prim->local_control);
    }
    else
    {
        /* Unknown bluetooth address supplied! Thus send a CFM with invalid
           status.
        */
        rfc_send_l2ca_move_cfm(rfc_params->rfc_ctrl->phandle,
                               &(p_prim->bd_addr),
                               p_prim->local_control,
                               p_prim->remote_control,
                               RFC_INVALID_CHANNEL);
    }

}


/*! \brief Handle a move L2CAP channel response

    \param rfc_params - Pointer to rfcomm instance data.
    \param p_prim - Pointer to the received primitive
*/
void rfc_handle_l2ca_move_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_L2CA_MOVE_CHANNEL_RSP_T *p_prim)
{
    rfc_find_mux_by_bd_addr(rfc_params,
                            &(p_prim->bd_addr));

    /* If the Mux was found then p_mux will be non NULL.
    */
    if(NULL != rfc_params->p_mux)
    {
        /* Create the associated L2CAP primitive.
        */
        L2CA_MoveChannelRsp(p_prim->identifier,
                            rfc_params->p_mux->info.mux.cid,
                            (l2ca_move_result_t)p_prim->status);
    }
    else
    {
        /* Unknown bluetooth address supplied! Thus send a CFM with invalid
           status.
        */
        rfc_send_l2ca_move_cmp_ind(rfc_params->rfc_ctrl->phandle,
                                   &(p_prim->bd_addr),
                                   0,
                                   RFC_INVALID_CHANNEL);
    }
}
#endif
