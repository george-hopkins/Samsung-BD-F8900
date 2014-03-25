/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_cid.c

\brief  This file implements the L2CAP channel state machine.

        For every L2CAP channel there will exist an instance of the
        CIDCB structure, this structure holds all the state
        information for a L2CAP channel.

        This file includes the channel state machine, upper and lower
        interfaces. The data path is removed from the state machine
        for simplicity and speed reasons.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_bt_core_stack_fsm.h"
#include "csr_mblk.h"
#ifndef BUILD_FOR_HOST
#include "buffers.h"
#endif

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_signal.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_control.h"
#include "l2cap_interface.h"
#include "l2cap_flow.h"
#include "l2caplib.h"
#include "dmlib.h"
#include "dm_security_manager.h"
#include "l2cap_cid_fsm_definitions.h"

#ifdef INSTALL_AMP_SUPPORT
#include "dm_amp_interface.h"
#include "l2cap_amp_utils.h"
#endif

/* Define which HCI error codes received during disconnect are 'normal' */
#define L2CAP_IS_ERROR_NORMAL(x)    ((x) == (CsrUint16)HCI_ERROR_OETC_USER || \
                                     (x) == (CsrUint16)HCI_ERROR_OETC_POWERING_OFF)

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT

/*! \brief Send Connect response signal */
static SIG_SIGNAL_T *CID_CreateConnectRsp(const CIDCB_T *cidcb,
                                          const CsrUint16 result,
                                          l2ca_identifier_t identifier)
{
    return SIG_SignalCreateConnectRsp(
            cidcb->local_cid,
            cidcb->remote_cid,
            result,
            (CsrUint16)(result == L2CA_CONNECT_PENDING
                ? SIGNAL_STATUS_AUTHORISATION : SIGNAL_STATUS_NONE),
            identifier);
}

/*! \brief Create config buffer */
static CsrBool CID_ConfBufCreate(CIDCB_T *cidcb)
{
    /* Handle out-of-memory in caller */
    if (cidcb->confbuf == NULL)
    {
        cidcb->confbuf = xzpnew(L2CAP_CONFIG_CONTAINER_T);
        if(!(cidcb->confbuf))
        {
#ifdef INSTALL_L2CAP_DEBUG
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
            return FALSE;
        }
    }
    return TRUE;
}


/*! \brief Free a config buffer completely */
static void CID_ConfBufFree(CIDCB_T *cidcb, L2CAP_CONFIG_BUFFERENUM_T target)
{
    L2CAP_CONFIG_CONTAINER_T *confbuf;
    L2CAP_CONFIG_BUFFERENUM_T cb;
    CsrBool all_free = TRUE;

    if((confbuf = cidcb->confbuf) != NULL)
    {
        L2CAP_CONFIG_BUFFER_T *buffer = confbuf->buffer;

        /* xapncc ignore '=': cast of 'int' to differing enum */
        for (cb = CONFBUF_MIN; cb != CONFBUF_ALL; ++cb)
        {
            if (target == CONFBUF_ALL || target == cb)
            {
                CsrPmemFree(buffer[cb].buf);
                CsrMemSet(&buffer[cb], 0, sizeof(L2CAP_CONFIG_BUFFER_T));
            }

            if (buffer[cb].buf != NULL)
                all_free = FALSE;
        }

        if (all_free)
        {
            CsrPmemFree(confbuf);
            cidcb->confbuf = NULL;
        }
    }
}

/*! \brief Cache downstream configuration data into config buffer

    Store configuration from peer in the buffer to we can collect the
    entire lot before we send up the signal to the upper layer
*/
static CsrBool CID_ConfBufStoreDown(L2CAP_CONFIG_BUFFER_T *buffer,
                                   L2CA_CONFIG_T *config)
{
    CsrUint8 *serial;

    CsrPmemFree(buffer->buf);
    buffer->off = 0;
    buffer->len = SIG_ConfigOptionsLength(config);
    buffer->buf = xpmalloc(buffer->len);
    if(buffer->buf == NULL)
    {
        /* Out of memory */
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        buffer->len = 0;
        return FALSE;
    }

    /* Serialise data into buffer */
    serial = buffer->buf;
    (void)SIG_ConfigSerialise(&serial, config);
    return TRUE;
}

/*! \brief Cache upstream configuration data into config buffer

    Store configuration from upper layer in the buffer to we can
    collect the entire lot before we start transmitting stuff over the
    air.
*/
static CsrBool CID_ConfBufStoreUp(L2CAP_CONFIG_BUFFER_T *buffer,
                                 const CsrUint8 *options,
                                 CsrUint16 length)
{
    CsrUint8 *dst;

    if (length == 0)
        return TRUE;

    /* Collect the remote configuration responses until we're
     * completely done */
    dst = bpxprealloc(buffer->buf,
                      buffer->len, /* old size */
                      buffer->len + length); /* new size */
    if(dst)
    {
        /* Reallocation ok, copy in new stuff */
        CsrMemCpy(dst + buffer->len, options, length);
        buffer->buf = dst;
        buffer->len += length;
        return TRUE;
    }
    else
    {
        /* Out of memory */
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        CsrPmemFree(buffer->buf);
        buffer->buf = NULL;
        buffer->len = 0;
        buffer->off = 0;
        return FALSE;
    }
}

/*! \brief Read out options from the local config request buffer

    Determine how many bytes we are able to read (up to MTU_signal,
    ie. 48) from the confbuf CIDCB buffer and update the variables
    accordingly. The function does not copy anything but merely updates
    the counters and return the length possible to read
*/
static CsrUint16 CID_ConfBufGet(L2CAP_CONFIG_BUFFER_T *buffer,
                               CIDCB_T *cidcb)
{
    CsrUint8 optsize;
    CsrUint16 length;
    CsrUint16 offset;

    /* Start scanning to find out how much we can read */
    length = 0;
    offset = buffer->off;
    while(offset < buffer->len)
    {
        /* Option data length index is at byte no. 2 */
        optsize = buffer->buf[offset+1] + L2CAP_SIZEOF_OPTION_HEADER;

        /* Make sure options and headers does not exceed the MTU */
        if((length + optsize + L2CAP_SIZEOF_CID_HEADER
                + L2CAP_SIZEOF_SIGNAL_HEADER) > CH_GET_SIGNAL_MTU(cidcb->chcb))
        {
            break;
        }

        length += optsize;
        offset += optsize;
    }

    buffer->off = offset;
    return length;
}

/*! \brief Generate configuration parameters from upstream signal

    Take a byte-stream and parallelise all known configuration options
    into the internal L2CA_CONFIG_T structure. Pointer members will
    automatically be allocated by the SIG_ConfigParallelise fct.
*/
static void CID_GenConfigUpstream(const CsrUint8 **options,
                                  CsrUint16 *size,
                                  L2CA_CONFIG_T *config)
{
    CsrMemSet(config, 0, sizeof(L2CA_CONFIG_T));

    /* Parse all options */
    while(*size > 0 && SIG_ConfigParallelise(options, size, config))
        ;
}

/*! \brief Channel is going down, so do cleanup

    When a disconnect is received either locally or remotely, we must
    discard any pending signals, data and configuration to avoid rogue
    signals being sent upwards after the channel has died.
*/
void CID_DisconnectCleanup(CIDCB_T *cidcb)
{
    /* Only dynamic channels have signals and config buffers */
    if(!CID_IsFixed(cidcb))
    {
        timer_cancel(&cidcb->config_timer_id);

        /* Remove any pending configuration and/or signals */
        CID_ConfBufFree(cidcb, CONFBUF_ALL);
        SIGH_SignalQueueEmptyWithCid(&(cidcb->chcb->signal_queue),
                                     cidcb->local_cid);
        SIGH_SignalQueueEmptyWithCid(&(cidcb->chcb->signal_pending),
                                     cidcb->local_cid);
    }

    /* Flush any F&EC bits */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if(CID_IsFlowControl(cidcb))
    {
        FLOW_Free(cidcb, L2CA_DATA_LINK_TERMINATED);
    }
#endif
}

/*! \brief Clip request direction config by response directions config.

    Some L2CAP config parameters are not negotiated but instead they
    may be clipped by a positive config response. The following parameters
    fall into this category:

    MTU
    MPS
    TX Window
    Extended TX Window

    \param request_config   Pointer to request config. This gets modified.
    \param response_buf     Pointer to response config buffer.
    \param response_len     Length of response config buffer.
    \param window_only      If TRUE then only clip windows, otherwise clip all.
*/
static void CID_ConfigClipByResponse(L2CA_CONFIG_T *request_config,
                                     const CsrUint8 *response_buf,
                                     CsrUint16 response_len,
                                     CsrBool window_only)
{
    L2CA_CONFIG_T response_config;

    if (response_buf == NULL || response_len == 0)
        return;

    CID_GenConfigUpstream(&response_buf, &response_len, &response_config);

    /* Clip indication TX window and MPS by response values. */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if ((response_config.options & L2CA_SPECIFY_FLOW) != 0)
    {
        L2CA_FLOW_T *request_flow;

        if ((request_flow = request_config->flow) != NULL)
        {
            if (!window_only && response_config.flow->maximum_pdu
                    < request_flow->maximum_pdu)
                request_flow->maximum_pdu = response_config.flow->maximum_pdu;

            if (response_config.flow->tx_window < request_flow->tx_window)
                request_flow->tx_window = response_config.flow->tx_window;
        }
    }

    if ((response_config.options & L2CA_SPECIFY_EXT_WINDOW) != 0
            && response_config.ext_window < request_config->ext_window)
        request_config->ext_window = response_config.ext_window;
#endif

    /* Clip MTU by response value. */
    if (!window_only && (response_config.options & L2CA_SPECIFY_MTU) != 0)
    {
        if (response_config.mtu < request_config->mtu)
            request_config->mtu = response_config.mtu;
    }

    L2CA_FreeConfigPtrs(&response_config);
}

/*! \brief Store configuration parameters

    Store the configuration parameter set and set appropiate instance
    variables in the cidcb/chcb/flow/etc. Note that we don't care about
    the confirm/response. In case something needs to be adjusted, that's
    done in the upper layer/auto-connect,
*/
static void CID_ParseConfiguration(CIDCB_T *cidcb)
{
    L2CA_CONFIG_T local;
    L2CA_CONFIG_T remote;
    L2CAP_CONFIG_BUFFER_T *buffer;
    const CsrUint8 *buffer_ptr;
    CsrUint16 bufsize;

    /* Sanity */
    if(!cidcb->confbuf)
    {
#ifdef INSTALL_L2CAP_DEBUG
        BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
#endif
        return;
    }

    buffer = cidcb->confbuf->buffer;

    /* Parse request (local) configuration and clip by application's
       response. We only clip the window size. We don't trust promises
       about MTU and MPS and we can't really take advantage of them
       anyway. We must clip window size because it affects our
       acknowledgement algorithm. */
    buffer_ptr = buffer[CONFBUF_REQ].buf;
    bufsize = buffer[CONFBUF_REQ].len;
    CID_GenConfigUpstream(&buffer_ptr, &bufsize, &local);
    CID_ConfigClipByResponse(&local,
                             buffer[CONFBUF_CFM].buf,
                             buffer[CONFBUF_CFM].len,
                             TRUE);

    /* Parse indication (remote) configuration and clip by our host's
       response. We clip MTU and MPS as well as TX window to ensure that
       L2CAP keeps its promises to the peer. */
    buffer_ptr = buffer[CONFBUF_IND].buf;
    bufsize = buffer[CONFBUF_IND].len;
    CID_GenConfigUpstream(&buffer_ptr, &bufsize, &remote);
    CID_ConfigClipByResponse(&remote,
                             buffer[CONFBUF_RES].buf,
                             buffer[CONFBUF_RES].len,
                             FALSE);

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    /* Incoming information may need to be adjusted */
    FLOW_FixConfig(cidcb, &remote, TRUE); /* from-air */

#if defined(INSTALL_L2CAP_CRC) && !defined(BUILD_FOR_HOST)
    /* Disable FCS if explicitly asked for */
    if(CID_IsFlowControl(cidcb))
    {
        /* If FCS is explicitly requsted to be disabled, do so */
        if((local.options & L2CA_SPECIFY_FCS) &&
           (local.fcs == L2CA_FCS_OFF) &&
           (remote.options & L2CA_SPECIFY_FCS) &&
           (remote.fcs == L2CA_FCS_OFF))
        {
            (void)FLOW_SetLcFcs(cidcb, FALSE);
        }
        else
        {
            (void)FLOW_SetLcFcs(cidcb, TRUE);
        }
    }
    else
    {
        /* Non-flow control channel. Disable FCS */
        (void)FLOW_SetLcFcs(cidcb, FALSE);
    }
#endif
#endif

    /* Only parse options if config_ok or pending are TRUE in pairs */
    if(((cidcb->local_config_ok && cidcb->remote_config_ok) ||
        CID_GetBothPending(cidcb)))
    {
        /* Store options */
        CID_StoreConfiguration(cidcb, &local, &remote);
    }

    /* Free members again */
    L2CA_FreeConfigPtrs(&local);
    L2CA_FreeConfigPtrs(&remote);
}


/*! \brief Ensure unique flowspec identifier

    All tx-flowspecs must have an unique identifier. Fill out this
    field if necessary.
*/
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
static void CID_SetFlowspecId(CIDCB_T *cidcb,
                              L2CA_CONFIG_T *config)
{
    if((config->options & L2CA_SPECIFY_FLOWSPEC) &&
       (config->flowspec != NULL))
    {
        /* Best effort always uses identifier 1 */
        if(config->flowspec->fs_service_type == L2CA_QOS_TYPE_BEST_EFFORT)
        {
            config->flowspec->fs_identifier = 1;
        }
        else if(config->flowspec->fs_identifier == 0)
        {
            /* Wrap around. Don't allow zero. */
            if((cidcb->flowspec_id == 255) ||
               (cidcb->flowspec_id == 0))
            {
                cidcb->flowspec_id = 1;
            }

            /* Increment and set */
            ++cidcb->flowspec_id;
            config->flowspec->fs_identifier = cidcb->flowspec_id;
        }
    }
}
#endif

/*! \brief Set config-response buffer to contain flowspec

    In case the AMP logical link creation fails we need to inform the
    peer about a new, improved/suggested flowspec.  This function
    clear, creates and sets the "res" buffer to contain exactly the
    passed flowspec.
*/
#ifdef INSTALL_AMP_SUPPORT
void CID_SetFlowspecResponse(CIDCB_T *cidcb, L2CA_FLOWSPEC_T *fs)
{
    L2CA_CONFIG_T config;

    /* Generate a temporary config buffer with the failed remote Rx
     * flowspec */
    CsrMemSet(&config, 0, sizeof(L2CA_CONFIG_T));
    config.options |= L2CA_SPECIFY_FLOWSPEC;
    config.flowspec = fs;

    /* Generate outgoing config buffer. Note that neither of these
     * functions can fail on-host */
    (void)CID_ConfBufCreate(cidcb);
    CID_ConfBufFree(cidcb, CONFBUF_RES);
    (void)CID_ConfBufStoreDown(&(cidcb->confbuf->buffer[CONFBUF_RES]), &config);
}
#endif


/*! \brief Notify CID instance of connection request signal.

    This function is called from the connection handler when a connection
    request signal is received.
*/
void CID_ConnectInd(CIDCB_T *cidcb, CID_CONNECT_REQ_T *signal_args)
{
    /* Call the state machine */
    CID_FsmRun(cidcb, CID_EV_CID_CONNECT_IND, signal_args);
}

/*! \brief Notify CID instance of connection request retry signal.

    This function is called from the connection handler when a connection
    request retry signal is received.
*/
void CID_ConnectIndRetry(CIDCB_T *cidcb, CID_CONNECT_REQ_T *signal_args)
{
    /* Call the state machine */
    CID_FsmRun(cidcb, CID_EV_CID_CONNECT_IND_RETRY, signal_args);
}

/*! \brief Notify CID instance of connection response signal.

    This function is called from the connection handler when a connection
    response signal is received.
*/
void CID_ConnectCfm(CIDCB_T *cidcb, l2ca_cid_t dest_cid, l2ca_conn_result_t result)
{
    fsm_event_t event = CID_EV_CID_CONNECT_CFM_OK;
    void *arg2 = &dest_cid;

    switch (result)
    {
        case L2CA_CONNECT_SUCCESS:
            break;

        case L2CA_CONNECT_PENDING:
            event = CID_EV_CID_CONNECT_CFM_PND;
            break;

        case L2CA_CONNECT_REJ_PSM:
            /* -fallthrough */
        case L2CA_CONNECT_REJ_SECURITY:
            /* -fallthrough */
        case L2CA_CONNECT_REJ_RESOURCES:
            /* -fallthrough */
        default:
            arg2 = &result;
            event = CID_EV_CID_CONNECT_CFM_NOK;
            break;
    }

    CID_FsmRun(cidcb, event, arg2);
}

/*! \brief Notify CID instance of configure request signal.

    This function is called from the connection handler when a configure
    request signal is received.
*/
void CID_ConfigInd(CIDCB_T *cidcb, CID_CONFIG_REQ_T *signal_args)
{
    CID_FsmRun(cidcb, CID_EV_CID_CONFIG_IND, signal_args);
}

/*! \brief Notify CID instance of configure response signal.

    This function is called from the connection handler when a configure
    response signal is received.
*/
void CID_ConfigCfm(CIDCB_T *cidcb, CID_CONFIG_RES_T *signal_args)
{
    CID_FsmRun(cidcb, CID_EV_CID_CONFIG_CFM, signal_args);
}

/*! \brief Notify CID instance of internal disconnect signal

    This function is called within L2CAP when an error forces us
    to close the channel
*/
void CID_DisconnectReqInternal(CIDCB_T *cidcb, l2ca_disc_result_t result)
{
    CID_FsmRun(cidcb, CID_EV_CID_INTERNAL_DISC_REQ, &result);
}

/*! \brief Notify CID instance of terminate request signal.

    This function is called from the connection handler when a terminate
    request signal is received.
*/
void CID_DisconnectInd(CIDCB_T *cidcb, CID_DISCONNECT_REQ_T *signal_args)
{
    CID_FsmRun(cidcb, CID_EV_CID_DISCONNECT_IND, signal_args);
}

/*! \brief Notify CID instance of terminate response signal.

    This function is called from the connection handler when a terminate
    response signal is received.
*/
void CID_DisconnectCfm(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_DISCONNECT_CFM, NULL);
}

/*! \brief Notify CID instance of local create request.

    This function handles a create request, it just passes the
    appropriate event into the state machine.
*/
void CID_ConnectReq(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_CONNECT_REQ, NULL);
}

/*! \brief Notify CID instance of local create response.

    This function handles a create response, it convert the response parameter
    into the appropriate event and passes it into the state machine.
*/
void CID_ConnectRsp(CIDCB_T *cidcb, L2CA_CONNECT_RSP_T *rsp)
{
    fsm_event_t event;

    cidcb->context = rsp->con_ctx;

    switch(rsp->response)
    {
        case L2CA_CONNECT_SUCCESS:
            event = CID_EV_CID_CONNECT_RSP_OK;
            break;

        case L2CA_CONNECT_PENDING:
            event = CID_EV_CID_CONNECT_RSP_PND;
            break;

        default:
            event = CID_EV_CID_CONNECT_RSP_NOK;
            break;
    }

    CID_FsmRun(cidcb, event, rsp);
}

/*! \brief Notify CID instance of local close request.

    This function handles a close request, it just passes the
    appropriate event into the state machine.
*/
void CID_DisconnectReq(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_DISCONNECT_REQ, NULL);
}

/*! \brief Notify CID instance of local close response.

    This function handles a close response, it just passes the appropriate
    event into the state machine.
*/
void CID_DisconnectRsp(CIDCB_T *cidcb, L2CA_DISCONNECT_RSP_T *rsp)
{
    CID_FsmRun(cidcb, CID_EV_CID_DISCONNECT_RSP, rsp);
}

/*! \brief Notify CID instance of local configure request.

    This function handles a configuration request, it just passes the
    appropriate event into the state machine.
*/
void CID_ConfigReq(CIDCB_T *cidcb, L2CA_CONFIG_REQ_T *req)
{
    CID_FsmRun(cidcb, CID_EV_CID_CONFIG_REQ, req);
}

/*! \brief Notify CID instance of local configure response.

    This function handles a configuration response, it just passed the
    appropriate event into the state machine.
*/
void CID_ConfigRsp(CIDCB_T *cidcb, L2CA_CONFIG_RSP_T *rsp)
{
    CID_FsmRun(cidcb, CID_EV_CID_CONFIG_RSP, rsp);
}

/*! \brief Notify CID that a signal has been completely sent

    When the connection handle receives the "NCP" event for a signal,
    we notify the CID. This allows us to eg. delay freeing the CIDCB
    (and CHCB/ACL) until after the data has actually been sent.
*/
void CID_SignalNcpInd(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_SIGNAL_NCP_IND, NULL);
}


/*! \brief Sends MBLK over multiple logical channels.

    This function attempts to send a data block in a MBLK over multiple
    logical channels.

    Note: Pointer to MBLK is set to NULL if this function takes ownership of the
    MBLK, otherwise the caller has responsibility for free-ing the MBLK.
*/
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
void CID_DataWriteMulticastReq(l2ca_cid_t cid_table[], CsrMblk **mblk_ptr)
{
    CsrMblk *mblk_prev = NULL;
    CIDCB_T *cidcb_prev = NULL;
    CsrUint16 cid_index;

    /* Loop through CIDs in multicast request */
    for (cid_index = 0; cid_index < L2CA_MAX_MULTICAST_CIDS; cid_index++)
    {
        /* Check CID is not empty */
        if (cid_table[cid_index] != L2CA_CID_INVALID)
        {
            CIDCB_T *cidcb = CIDME_GetCidcb(cid_table[cid_index]);
            if (cidcb != NULL)
            {
                /* Check if we have previous MBLK to send */
                if (mblk_prev != NULL)
                {
                    /* Attempt to duplicate this MBLK */
                    *mblk_ptr = CsrMblkDuplicateRegion(mblk_prev, 0, CsrMblkGetLength(mblk_prev));
                    if (*mblk_ptr != NULL)
                    {
                        /* Send previous MBLK */
                        CID_DataWriteReq(cidcb_prev, &mblk_prev, 0);
                    }
                }

                /* Make current MBLK & CID the previous MBLK & CID */
                mblk_prev = *mblk_ptr;
                cidcb_prev = cidcb;
                *mblk_ptr = NULL;
            }
        }
    }

    /* Check if we have previous MBLK to send */
    if (mblk_prev != NULL)
    {
        CID_DataWriteReq(cidcb_prev, &mblk_prev, 0);
    }
}
#endif


/*! \brief Received signal timeout.

    This function is called whenever a signal has timed out (after RTX/ERTX
    retries).  Just passes an event into the CID state machine.
*/
void CID_Timeout(CIDCB_T *cidcb)
{
    if (cidcb != NULL)
        CID_FsmRun(cidcb, CID_EV_CID_TIMEOUT, NULL);
}

/*! \brief Received reject signal

    This function is called when ever a reject signal is received when the
    reason code is "reject CID".  Just passes an event into the CID state
    machine.
*/
void CID_InvalidCid(CIDCB_T *cidcb)
{
    CID_FsmRun(cidcb, CID_EV_CID_INVALID_CID, NULL);
}

/*! \brief Flush all entirely unsent data from TX queue. */
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
void CID_AbortTX(CIDCB_T *cidcb)
{
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    if (cidcb->flow != NULL && cidcb->flow->config.mode != L2CA_FLOW_MODE_BASIC)
    {
        FLOW_AbortTX(cidcb, cidcb->flow);
    }
    else
#endif /* INSTALL_L2CAP_ENHANCED_SUPPORT */
    {
        CH_AbortTX(cidcb);
    }
}
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/*! \brief Configuration timeout

    This function is called when the configuration procedure times-out.
    Just passes an event into the CID state machine.
*/
void CID_ConfigTimeout(CsrUint16 param, void *cidcb_in)
{
    CIDCB_T *cidcb = (CIDCB_T *)cidcb_in;
    PARAM_UNUSED(param);

    TIMER_EXPIRED(cidcb->config_timer_id);
    CID_FsmRun(cidcb, CID_EV_CID_CONFIG_TIMEOUT, NULL);
}

/*! \brief Start the configuration timer if it has not already been started.

    Starts a timer to call CID_ConfigTimeout() after
    L2CAP_CONFIG_TIMEOUT.  We also stop the F&EC code from firing any
    timers from this point onwards as traffic must be halted during
    configurations anyway.

    \param cidcb Pointer to channel record structure.
*/
void CID_ConfigTimerStart(CIDCB_T *cidcb)
{
    if (cidcb->config_timer_id == 0)
    {
        timer_start(&cidcb->config_timer_id,
                    L2CAP_CONFIG_TIMEOUT,
                    CID_ConfigTimeout,
                    0,
                    cidcb);
    }

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    /* Kill F&EC code timers */
    if(CID_IsFlowControl(cidcb) &&
       !CID_IsMoving(cidcb))
    {
        FLOW_Choke(cidcb, FCOK_CONF_START);
    }
#endif
}

/*! \brief Access Confirm from Security Manager

    This function is called to handle the access confirmation message
    from the Security Manager.  The function attempts to find the
    L2CAP channel instance from the remote device address and CID. If
    a match is found a event is passed into the CID state machine.

    \param p_prim Pointer to access confirm message.
*/
void CID_SmAccessCfm(DM_SM_ACCESS_CFM_T *p_prim)
{
    L2CAP_CHCB_T *chcb;
    CIDCB_T *cidcb;
    hci_return_t status;

    status = p_prim->status;
    chcb = CHME_GetConHandleBdAddr(&p_prim->conn_setup.connection.addrt);

     if (chcb != NULL)
     {
         if ((cidcb = CIDME_GetCidcbWithHandle(chcb, (l2ca_cid_t)p_prim->context)) != NULL)
         {
             DM_SM_SERVICE_T service;

             if (cidcb->dm_sm_service == NULL)
             {
                 service.protocol_id = SEC_PROTOCOL_L2CAP;
                 service.channel = cidcb->local_psm->psm;
             }
             else
             {
                 service = *cidcb->dm_sm_service;
                 CsrPmemFree(cidcb->dm_sm_service);
                 cidcb->dm_sm_service = NULL;
             }

             if (service.protocol_id == p_prim->conn_setup.connection.service.protocol_id &&
                 service.channel == p_prim->conn_setup.connection.service.channel)
            {
                CID_FsmRun(cidcb,
                           (fsm_event_t)((status == HCI_SUCCESS
                                          ? CID_EV_SM_ACCESS_CFM_OK
                                          : CID_EV_SM_ACCESS_CFM_NOK)),
                           &status);
            }
        }
    }
}

#ifndef BUILD_FOR_HOST
/*! \brief Clip MTU for on-chip buffer sizes
           Currently we don't clip for stream buffer sizes as that will be
           done in the stream subsystem.

    \param cidcb Pointer to Channel ID Control Block
    \param mtu Pointer to maximum transmission unit. Will be clipped.
    \param from_air TRUE for MTU received from air. FALSE otherwise.
                     N.B. MTU received 'from air' will be remote MTU and so
                     will limit the size of L2CAP packets sent 'to air'.
*/
void CID_ClipMTU(CIDCB_T *cidcb, l2ca_mtu_t *mtu, CsrBool from_air)
{
    CsrUint16 mtu_max = ~0;

    /* Top bit of phandle indicates that client is off-chip. If set then we
       should clip the MTU for the hostio buffer sizes. */
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
    if ((cidcb->p_handle_data & 0x8000) != 0)
#else
    if ((cidcb->p_handle & 0x8000) != 0)
#endif
    {
        BUFFER *buffer = buf_l2cap_tohost;
        CsrUint16 overhead = 1 + BUF_RESERVED_SPACE + 2*sizeof(L2CA_DATAREAD_IND_T);

        if (from_air)
        {
            buffer = buf_hci_l2cap_fromhost;
            overhead = 1 + BUF_RESERVED_SPACE + 2*sizeof(L2CA_DATAWRITE_REQ_T);
        }

        mtu_max = buf_get_capacity(buffer) - overhead;
    }

    /* We must be able to fit an entire L2CAP packet in the ACL RX buffer. */
    if (!from_air && mtu_max > ACL_BUFFER_SIZE_RX - L2CAP_ACL_RAW_LENGTH_MAX
            - BUF_RESERVED_SPACE)
        mtu_max = ACL_BUFFER_SIZE_RX - L2CAP_ACL_RAW_LENGTH_MAX
            - BUF_RESERVED_SPACE;

    if (*mtu > mtu_max)
        *mtu = mtu_max;
}
#endif

void CID_SendDMSMAccessReq(CIDCB_T *cidcb, CsrBool incoming)
{
    dm_protocol_id_t protocol = SEC_PROTOCOL_L2CAP;
    CsrUint16 channel = CIDME_LocalPsm(cidcb);
    DM_SM_SERVICE_T *service;

    if ((service = cidcb->dm_sm_service) != NULL)
    {
        /* Use stored service provided by higher layer protocol. */
        protocol = service->protocol_id;
        channel = service->channel;

    }

    dm_sm_access_req(L2CAP_IFACEQUEUE,
                     CH_GET_BD_ADDR(cidcb->chcb),
                     protocol,
                     channel,
                     incoming,
                     (CsrUint32)cidcb->local_cid,
                     NULL);
}


/*! \brief Upper layer requests new connection */
fsm_event_t CID_FsmConnectReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Attempt to create the connect request signal */
    sig_ptr = SIG_SignalCreateConnectReq(cidcb->local_cid, cidcb->remote_psm);

    /* Send the signal */
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Close CID due to security rejection during connection */
fsm_event_t CID_FsmConnectCfmErrorSec(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    hci_return_t status = *((hci_return_t*)arg2);
    l2ca_conn_result_t result = L2CA_CONNECT_REJ_SECURITY;

    if(status == HCI_ERROR_KEY_MISSING)
    {
        result = L2CA_CONNECT_KEY_MISSING;
    }

    /* Connection has failed due to security limitations */
    CID_SendConnectCfm(cidcb, result);

    /* Destroy this instance */
    CID_Destroy(cidcb, HCI_ERROR_AUTH_FAIL, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Connect request from peer */
fsm_event_t CID_FsmConnectInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CONNECT_REQ_T *signal_args = (CID_CONNECT_REQ_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Copy the CID into the CIDCB structure */
    cidcb->remote_cid = signal_args->source_cid;
    cidcb->rcnreq_signal_id = signal_args->signal_id;

    /* Attempt to create the connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_PENDING,
                                   cidcb->rcnreq_signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Send access request to Security Manager */
    CID_SendDMSMAccessReq(cidcb, TRUE);

    /* No more events */
    return CID_EVENT_NULL;
}


/*! \brief Re-transmission of connect request from peer */
fsm_event_t CID_FsmConnectIndRetry(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CONNECT_REQ_T *signal_args = (CID_CONNECT_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Attempt to create connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_PENDING,
                                   signal_args->signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Re-transmission of connect request from peer

    This function is called if we receive another connect request
    while the channel is already active -- this can happen if the
    other device is rather dodgy in it's connect implementation.
    Basically we just tell the peer to bugger off...

*/
fsm_event_t CID_FsmConnectIndRetryActive(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CONNECT_REQ_T *signal_args = (CID_CONNECT_REQ_T*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Attempt to create connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_REJ_RESOURCES,
                                   signal_args->signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Upper layer accepts peer connection request */
fsm_event_t CID_FsmConnectRspOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONNECT_RSP_T *prim = (L2CA_CONNECT_RSP_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Reset configuration negotiation and pending flags */
    cidcb->local_config_ok = FALSE;
    cidcb->remote_config_ok = FALSE;
    CID_SetLocalPending(cidcb, FALSE);
    CID_SetRemotePending(cidcb, FALSE);

    /* Start the configuration timer */
    CID_ConfigTimerStart(cidcb);

    /* Attempt to create the connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_SUCCESS,
                                   prim->identifier);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Upper layer rejects peer connection request

    We send the reject to the peer. The FSM will put us into the "wait
    for NCP" state to ensure that our response signal gets sent before
    we drop the ACL and destroy the CIDCB.
*/
fsm_event_t CID_FsmConnectRspNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONNECT_RSP_T *prim = (L2CA_CONNECT_RSP_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Attempt to create connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   (CsrUint16)(prim->response),
                                   prim->identifier);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Detach ourself from PSM registration to allow it to be deleted. */
    cidcb->local_psm = NULL;

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Security manager allows peer connect request */
fsm_event_t CID_FsmSmAccessCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Send connect indication to higher layer protocol */
    L2CA_ConnectInd(cidcb, cidcb->rcnreq_signal_id);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Security manager rejects peer connect request */
fsm_event_t CID_FsmSmAccessCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Attempt to create the signal indicating rejection due to security */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_REJ_SECURITY,
                                   cidcb->rcnreq_signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Destroy this instance */
    CID_Destroy(cidcb, HCI_ERROR_AUTH_FAIL, ACL_IDLE_TIMEOUT_AT_LEAST_TINY);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Upper layer indicates connection request pending */
fsm_event_t CID_FsmConnectRspPnd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONNECT_RSP_T *prim = (L2CA_CONNECT_RSP_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Attempt to create connect response signal */
    sig_ptr = CID_CreateConnectRsp(cidcb,
                                   L2CA_CONNECT_PENDING,
                                   prim->identifier);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Peer accepts connection request */
fsm_event_t CID_FsmConnectCfmOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_cid_t *dest_cid = (l2ca_cid_t *)arg2;

    /* Extract the remote CID */
    cidcb->remote_cid = *dest_cid;

    /* Send L2CA_CONNECT_CFM */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_SUCCESS);

    /* Reset configuration negotiation and pending flags */
    cidcb->local_config_ok = FALSE;
    cidcb->remote_config_ok = FALSE;
    CID_SetLocalPending(cidcb, FALSE);
    CID_SetRemotePending(cidcb, FALSE);

    /* Start the configuration timer */
    CID_ConfigTimerStart(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Peer rejects connection request */
fsm_event_t CID_FsmConnectCfmNok(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_conn_result_t *response = (l2ca_conn_result_t *)arg2;

    /* Send connect confirmation to higher layer */
    CID_SendConnectCfm(cidcb, *response);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Peer indicates connection request is currently pending */
fsm_event_t CID_FsmConnectCfmPnd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_cid_t *dest_cid = (l2ca_cid_t *)arg2;

    /* Extract the remote cid */
    cidcb->remote_cid = *dest_cid;

    /* Send connect confirmation to higher layer */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_PENDING);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Error occured during outgoing connection setup */
fsm_event_t CID_FsmConnectCfmError(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Send connect confirmation to higher layer */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_FAILED);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}


/*! \brief Connection was aborted during setup

    Send up CONNECT_CFM with error (as upper layer expects a CFM for
    the REQ) and also send over the disconnect request signal to peer.
    The FSM puts us into the DISCONNECT_INTRN state which just waits
    until the peer has responded with DISCONNECT_RES before killing
    the CIDCB.
*/
fsm_event_t CID_FsmConnectCfmAbort(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Create disconnect signal */
    sig_ptr = SIG_SignalCreateDisconnectReq(cidcb->remote_cid, cidcb->local_cid);

    /* Send the signal */
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Send connect confirmation to higher layer */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_FAILED);

    /* No more events */
    return CID_EVENT_NULL;
}


/*! \brief Upper layer has initiated configuration negotiation

    The upper layer has requested sending a config request signal.
    Start the config timer, cache the configuration and send over
    the first config request signal.
*/
fsm_event_t CID_FsmConfigReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONFIG_REQ_T *prim = (L2CA_CONFIG_REQ_T *)arg2;
    L2CAP_CONFIG_BUFFER_T *buffer = NULL;
    SIG_SIGNAL_T *sig_ptr;
    CsrUint8 *cfstr;
    CsrUint16 cflen;
    CsrBool cfcon;

    /* Channel priority */
    cidcb->priority =
            (CsrUint8)((prim->config.options & L2CA_SPECIFY_PRIORITY_INTERNAL)
            >> L2CA_OPTION_PRIORITY_SHIFT);
    prim->config.options &= ~L2CA_SPECIFY_PRIORITY_INTERNAL;

    /* Reset config for local side and start config timer */
    cidcb->local_config_ok = FALSE;
    CID_SetRemotePending(cidcb, FALSE);
    CID_ConfigTimerStart(cidcb);

    /* For enhanced retransmission mode, timers are set
     * automatically. Set timers to zero as required */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    FLOW_FixConfig(cidcb, &(prim->config), FALSE); /* to-air */
#endif

    /* Detect lockstep and ensure that the identifier is set
     * correctly */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if((prim->config.options & L2CA_SPECIFY_FLOWSPEC))
    {
        cidcb->use_lockstep = TRUE;
    }
    CID_SetFlowspecId(cidcb, &(prim->config));
#endif

    /* On-chip, clip the maximum MTU to avoid excessive buffer
     * usage. Always clip MTU to allow for extended L2CAP headers. */
    CID_ClipMTU(cidcb, &prim->config.mtu, FALSE); /* to-air */
    prim->config.mtu = CSRMIN(prim->config.mtu, L2CAP_MTU_MAXIMUM);

    /* Store MTU and flush timeout immediately as the config-cfm
     * signal depends on the values being set sensibly before we would
     * otherwise have a chance to store them */
    if(prim->config.options & L2CA_SPECIFY_MTU)
    {
        cidcb->local_mtu = prim->config.mtu;
    }
    if(prim->config.options & L2CA_SPECIFY_FLUSH)
    {
        cidcb->local_flush_to = prim->config.flush_to;
    }

    /* Donot force FCS to OFF here. */
    /* FCS option sent needs to be in accordance with flow-mode requirements (mode == ERTM or SM) */
    /* which should be checked before sending the config-req to the FSM. */

    /* Cache the configuration to deal with continuation and defer
     * "storing" of the configuration until we're completely done */
    if (CID_ConfBufCreate(cidcb))
    {
        buffer = cidcb->confbuf->buffer;
        if (!CID_ConfBufStoreDown(&(buffer[CONFBUF_REQ]), &prim->config))
            buffer = NULL;
    }

    /* Discard the config buffer */
    L2CA_FreeConfigPtrs(&prim->config);

    /* Parse configuration, primarily for the on-chip case to ensure
     * that FCS is turned on in the LC if necessary. */
    CID_ParseConfiguration(cidcb);

    /* Out of memory caused by upper layer! We can't do anything but
     * close the channel */
    if(buffer == NULL)
    {
        CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_OUT_OF_MEM);
        return CID_EVENT_NULL;
    }

    /* Create the config request signal.

       Note that CID_ConfBufGet will alter the value of buffer[CONFBUF_REQ].off
       and so it is very important to ensure that cfstr is calculated before
       cflen, which is calculated before cfcon. This means that these values
       must be assigned to local variables first and cannot be done on the
       fly in the arguments to the function because function arguments are
       not processed in any well-defined order. */
    cfstr = buffer[CONFBUF_REQ].buf + buffer[CONFBUF_REQ].off; /* options */
    cflen = CID_ConfBufGet(&(buffer[CONFBUF_REQ]), cidcb); /* options length */
    cfcon = buffer[CONFBUF_REQ].off < buffer[CONFBUF_REQ].len; /* cont flag */
    sig_ptr = SIG_SignalCreateConfigSignal(cidcb,
                                           cflen,
                                           cfstr,
                                           cfcon,
                                           0,
                                           L2CA_CONFIG_SUCCESS);

    /* Send the signal */
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Configuration response complete helper

    When we have transferred our last configuration response
    signal we must update the local state. This can be triggered
    from both the locally initiated config_res sender, or from the
    remotely triggered config_req(poll) functions
*/
fsm_event_t CID_ConfigComplete(CIDCB_T *cidcb)
{
    /* Check to see if config is now complete */
    if(cidcb->local_config_ok &&  cidcb->remote_config_ok)
    {
        /* For flowspec mode, we must detect pending/success errors */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
        if(CID_GetAnyPending(cidcb) &&
           !CID_GetBothPending(cidcb))
        {
            CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_LOCKSTEP_ERROR);
            return CID_EVENT_NULL;
        }
#endif

#ifdef INSTALL_AMP_SUPPORT
        /* For AMP channels (setup via create channel request), the
         * lockstep and use of FEC is mandatory */
        if(CID_IsAmp(cidcb) && (AMP_GetState(cidcb) == AMPST_CREATING))
        {
            if(!CID_IsFlowControl(cidcb) ||
               (cidcb->tx_flowspec == NULL) ||
               (cidcb->rx_flowspec == NULL))
            {
                CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_LOCKSTEP_ERROR);
                return CID_EVENT_NULL;
            }
        }
#endif
        /* Config success on both sides */
        return CID_EV_CID_CONFIG_OK;
    }
    else
    {
        return CID_EVENT_NULL;
    }
}

/*! \brief Admission control or logical channel ok

    Proper AMPs can do admission control in the controller, but
    BR/EDR does checks in L2CAP. This function is used to progress
    a positive answer on to the peer and the state machine
*/
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
fsm_event_t CID_LogicalComplete(CIDCB_T *cidcb)
{
    L2CA_CONFIG_T config;
    SIG_SIGNAL_T *sig_ptr;
    const CsrUint8 *confbuf_ptr = NULL;
    CsrUint16 confbuf_len = 0;

    /* We shall not send the flowspec in the config-rsp-success, so we
     * need to regenerate the rsp-buffer without that option */
     if(cidcb->confbuf != NULL)
     {
        L2CAP_CONFIG_BUFFER_T *buffer = cidcb->confbuf->buffer;
        confbuf_ptr = buffer[CONFBUF_RES].buf;
        confbuf_len = buffer[CONFBUF_RES].len;
        CID_GenConfigUpstream(&confbuf_ptr, &confbuf_len, &config);
        config.options &= ~(L2CA_SPECIFY_FLOWSPEC);
        (void)CID_ConfBufStoreDown(&buffer[CONFBUF_RES], &config); /* can't fail on host */
        L2CA_FreeConfigPtrs(&config);

        confbuf_ptr = buffer[CONFBUF_RES].buf;
        confbuf_len = buffer[CONFBUF_RES].len;
    }

    /* Send a configuration success to tell peer that the flowspecs
     * have been accepted and applied */
    sig_ptr = SIG_SignalCreateConfigSignal(
            cidcb,
            confbuf_len, /* length of options */
            confbuf_ptr, /* options */
            FALSE,       /* continuation */
            cidcb->rcnreq_signal_id,
            L2CA_CONFIG_SUCCESS);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Remote config now done */
    cidcb->remote_config_ok = TRUE;
    return CID_ConfigComplete(cidcb);
}
#endif

/*! \brief Logical channel and/or QoS setup

    Once both devices has reached the config-pending state
    we can either try to set up the logical channel (if running
    AMP) or do the QoS aggregation and/or admission control.

    This function will fire off whatever action comes next pending on
    our current state */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
static fsm_event_t CID_LogicalSetup(CIDCB_T *cidcb)
{
    fsm_event_t event;

    /* Don't expect this function to short-circuit anything */
    event = CID_EVENT_NULL;

    /* When creating an AMP channel, flowspecs are used for the
     * logical channel setup */
    if(CID_GetBothPending(cidcb))
    {
        /* Emergency flowspec generation. The spec allows us to do
         * this as long as we use best-effort ones */
        if(cidcb->tx_flowspec == NULL)
        {
            cidcb->tx_flowspec = L2CA_AllocFlowspec();
        }
        if(cidcb->rx_flowspec == NULL)
        {
            cidcb->rx_flowspec = L2CA_AllocFlowspec();
        }

#ifdef INSTALL_AMP_SUPPORT
        if(CID_IsAmp(cidcb))
        {
            dm_amp_logical_connect_req(CH_GET_ACL(cidcb->chcb),
                                       cidcb->local_cid,
                                       cidcb->amp_info->local_amp,
                                       cidcb->tx_flowspec,
                                       cidcb->rx_flowspec);
        }
        else
#endif
        {
            /* TODO : implement flowspec admission control - see
             * description for B-49507. For now, we just cheat and
             * pretend that everything was OK */
            event = CID_LogicalComplete(cidcb);
        }
    }

    return event;
}
#endif

/*! \brief Upper layers responds to remote configuration

    Store configuration in local buffer so we can send multiple
    signals in case we exceed the MTU. Always send the first signal
    across.

    Also note that this _can_ be the last config signal in case we are
    the slave and we don't need the continuation.
*/
fsm_event_t CID_FsmConfigRsp(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_CONFIG_RSP_T *prim = (L2CA_CONFIG_RSP_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;
    CsrUint8 *cfstr;
    CsrUint16 cflen;
    CsrBool cfcon;

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    /* Enforce lockstep */
    if(cidcb->use_lockstep &&
       !(prim->config.options & L2CA_SPECIFY_FLOWSPEC))
    {
        CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_LOCKSTEP_ERROR);
        return CID_EVENT_NULL;
    }

    /* In case an extended flowspec is onboard we cheat a bit and
     * modify the response code from SUCCESS to PENDING in order to
     * hide this step from the upper layer. Also, cache the identifier
     * for use in the final success/failure signal sent from
     * AmpLogicalCfmOk/Nok */
    if((prim->config.options & L2CA_SPECIFY_FLOWSPEC) &&
       (prim->config.flowspec != NULL) &&
       (prim->response == L2CA_CONFIG_SUCCESS))
    {
        prim->response = L2CA_CONFIG_PENDING;
        cidcb->rcnreq_signal_id = prim->identifier;
    }
#endif

    /* Don't allow use of FCS options for basic, RTM or FC mode */
    if((prim->config.flow == NULL) ||
       (prim->config.flow->mode == L2CA_FLOW_MODE_BASIC))
    {
        prim->config.options &= ~L2CA_SPECIFY_FCS;
        prim->config.fcs = L2CA_FCS_OFF;
    }

    /* Be pessimistic about results */
    cidcb->remote_config_ok = FALSE;

    /* Cache the configuration to deal with continuation.  This will
     * free any existing "res" buffer contents. */
    cfcon = CID_ConfBufCreate(cidcb);
    if(cfcon)
    {
        cfcon = CID_ConfBufStoreDown(&(cidcb->confbuf->buffer[CONFBUF_RES]),
                                     &prim->config);
    }

    /* Discard the config buffer */
    L2CA_FreeConfigPtrs(&prim->config);

    /* Deal with these options differently depending on whether
     * they've been accepted, are pending or rejected */
    switch(prim->response)
    {
        case L2CA_CONFIG_SUCCESS:
            /* Application has accepted peer's configuration. Store
             * the "ind" buffer in the accepted configuration */
            cidcb->remote_config_ok = TRUE;
            CID_ParseConfiguration(cidcb);
            break;

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
        case L2CA_CONFIG_PENDING:
            /* Local side now in pending. Process config buffers.  We
             * will send the pending result to the peer later on */
            CID_SetLocalPending(cidcb, TRUE);
            CID_ParseConfiguration(cidcb);
            break;
#endif

        default:
            /* Application has rejected the peer's configuration. We
             * can then clear the "ind" confbuffer */
            if(cidcb->confbuf)
            {
                CID_ConfBufFree(cidcb, CONFBUF_IND);
            }
            break;
    }

    /* Out of memory caused by upper layer! We can't do anything but
     * close the channel */
    if(!cfcon)
    {
        CID_DisconnectReqInternal(cidcb, L2CA_DISCONNECT_OUT_OF_MEM);
        return CID_EVENT_NULL;
    }

    /* Create the config response signal - but only if options are
     * present, are we're not replying with pending */
    cfstr = NULL;
    cflen = 0;
    cfcon = FALSE;
    if (cidcb->confbuf != NULL && prim->response != L2CA_CONFIG_PENDING)
    {
        L2CAP_CONFIG_BUFFER_T *buffer = cidcb->confbuf->buffer;

        if (buffer[CONFBUF_RES].len != 0)
        {
            /* Note that CID_ConfBufGet will alter the value of
               buffer[CONFBUF_RES].off and so it is very important to
               ensure that cfstr is calculated before cflen, which is
               calculated before cfcon. This means that these values
               must be assigned to local variables first and cannot
               be done on the fly in the arguments to the function
               because function arguments are not processed in any
               well-defined order. */
            cfstr = buffer[CONFBUF_RES].buf + buffer[CONFBUF_RES].off;
            cflen = CID_ConfBufGet(&(buffer[CONFBUF_RES]), cidcb);
            cfcon = (buffer[CONFBUF_RES].off < buffer[CONFBUF_RES].len);
        }
    }

    /* Create config response signal (also frees option block!)*/
    sig_ptr = SIG_SignalCreateConfigSignal(cidcb,
                                           cflen,  /* length of options */
                                           cfstr,  /* options */
                                           cfcon,  /* continuation */
                                           prim->identifier,
                                           prim->response);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    if(prim->response == L2CA_CONFIG_PENDING)
    {
        /* Check for lockstep completion */
        return CID_LogicalSetup(cidcb);
    }
    else
#endif
    if(!cfcon)
    {
        /* Check if this response is the final one, and if so try to kick
         * the state machine into the next realm... */
        return CID_ConfigComplete(cidcb);
    }
    else
    {
        /* Stay in config state, no more events */
        return CID_EVENT_NULL;
    }
}

/*! \brief Peer sent us a configuration request

    The peer has sent us a config request signal, we should extract
    the configuration options, start the configure timer if not
    already running and pass the options to the high layer. Note that
    this signal can also be used to "poll" for the remaining parts of
    a continuation-enabled response from our side!

    We can then take three different steps:
    - Poll peer for next request signal  (nil/data,    continue)
    - Send next response signal to peer  (nil,         final, res-in-process)
    - Deliver config ind to app          (nil/data,    final)
*/
fsm_event_t CID_FsmConfigInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CONFIG_REQ_T *signal_args = (CID_CONFIG_REQ_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;
    l2ca_conf_result_t result = L2CA_CONFIG_UNKNOWN;
    fsm_event_t event = CID_EVENT_NULL;
    CsrUint8 *cfstr = NULL;
    CsrUint16 cflen = 0;
    CsrBool cfcon = FALSE;

    /* Reset config for remote side and start the timer */
    cidcb->remote_config_ok = FALSE;
    CID_ConfigTimerStart(cidcb);

     /* Cache configuration to deal with continuation. This also lets
      * us defer accepting anything until we have a definite answer */
    if (CID_ConfBufCreate(cidcb))
    {
        L2CAP_CONFIG_BUFFER_T *buffer = cidcb->confbuf->buffer;

        if (CID_ConfBufStoreUp(&buffer[CONFBUF_IND],
                    signal_args->options,
                    signal_args->options_size))
        {
            /* Handle contination flag
             * Section 4.4 in the spec allows us to send back a success
             * result code with empty options to pump the rest of the
             * configuration request signals from the peer */
            result = L2CA_CONFIG_SUCCESS;
            cfcon = TRUE;

            if((signal_args->flags & L2CAP_SIGNAL_CONTINUATION) == 0)
            {
                if (buffer[CONFBUF_RES].off == buffer[CONFBUF_RES].len)
                {
                    /* Final configuration request received while we were not
                     * transferring any response. We should forward the
                     * request to the application */
                    L2CA_CONFIG_T config;
                    const CsrUint8 *tmp_cfstr = buffer[CONFBUF_IND].buf;
                    CsrUint16 tmp_cflen = buffer[CONFBUF_IND].len;

                    /* Populate configuration structure from signal */
                    CID_GenConfigUpstream(&tmp_cfstr, &tmp_cflen, &config);

                    /* For enhanced retransmission mode, timers are set
                     * automatically.  Fixup these timers so upper layer
                     * knows what they will be */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                    FLOW_FixConfig(cidcb, &config, TRUE); /* from-air */
#endif

                    /* On-chip, clip the maximum MTU to avoid excessive buffer
                     * usage. Always clip MTU to allow for extended L2CAP
                     * headers */
                    CID_ClipMTU(cidcb, &config.mtu, TRUE); /* from-air */
                    config.mtu = CSRMIN(config.mtu, L2CAP_MTU_MAXIMUM);

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
                    if(config.options & L2CA_SPECIFY_FLOWSPEC)
                    {
                        cidcb->use_lockstep = TRUE;
                    }
#endif

                    /* Send indication, ownership of config-pointers pass
                     * to the function. */
                    L2CA_ConfigInd(cidcb, signal_args->signal_id, &config);
                    return CID_EVENT_NULL;
                }

                /* Note that CID_ConfBufGet will alter the value of
                   buffer[CONFBUF_RES].off and so it is very important to
                   ensure that cfstr is calculated before cflen, which is
                   calculated before cfcon. This means that these values
                   must be assigned to local variables first and cannot
                   be done on the fly in the arguments to the function
                   because function arguments are not processed in any
                   well-defined order. */

                /* We are in the process of transferring our config
                 * response to the peer but are not done yet */
                cfstr = buffer[CONFBUF_RES].buf + buffer[CONFBUF_RES].off;

                /* NB: CID_ConfBufGet() modifies buffer[CONFBUF_RES].off,
                 * so don't use a local cached copy. */
                cflen = CID_ConfBufGet(&(buffer[CONFBUF_RES]), cidcb);

                /* This could be the last config response in which case
                 * we'll need to update our configuration valid-flags
                 * (remote ok). Note that the response code can only be
                 * successful at this point as the app is still
                 * responsible for filtering out any bad signals before we
                 * can enter the response-request poll mode */
                if (buffer[CONFBUF_RES].off >= buffer[CONFBUF_RES].len)
                {
                    cfcon = FALSE;
                    cidcb->remote_config_ok = TRUE;
                    event = CID_ConfigComplete(cidcb);
                }
            }
        }
    }

    /* Signal the peer (success or failure) */
    sig_ptr = SIG_SignalCreateConfigSignal(cidcb,
                                           cflen,  /* length of options */
                                           cfstr,  /* options */
                                           cfcon,  /* continuation */
                                           signal_args->signal_id,
                                           result);
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    return event;
}

/*! \brief Received configuration response from peer

    Cache the configuration response options, and then take
    one of the following steps:

    - Poll peer/send next request chunk        (continue)
    - Deliver config cfm to the application    (final)
*/
fsm_event_t CID_FsmConfigCfm(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_CONFIG_RES_T *signal_args = (CID_CONFIG_RES_T *)arg2;
    L2CAP_CONFIG_BUFFER_T *buffer;
    const CsrUint8 *cfstr = NULL;
    CsrUint16 cflen = 0;
    CsrBool cfcon = FALSE;
    fsm_event_t event = CID_EVENT_NULL;

    /* Be pessimistic about results */
    cidcb->local_config_ok = FALSE;

    /* Cache configuration to deal with continuation */
    if (CID_ConfBufCreate(cidcb) &&
        CID_ConfBufStoreUp(&((buffer = cidcb->confbuf->buffer)[CONFBUF_CFM]),
            signal_args->options,
            signal_args->options_size))
    {
        if(signal_args->flags & L2CAP_SIGNAL_CONTINUATION)
        {
            /* Deal with continuation */

            /* Note that CID_ConfBufGet will alter the value of
               buffer[CONFBUF_REQ].off and so it is very important to
               ensure that cfstr is calculated before cflen, which is
               calculated before cfcon. This means that these values
               must be assigned to local variables first and cannot
               be done on the fly in the arguments to the function
               because function arguments are not processed in any
               well-defined order. */

            /* Notice that if peer is polling us, we send the data. In
             * case we're done polling the req-buffer will be empty and
             * the roles will be switched as the request will be empty */
            cfstr = (buffer[CONFBUF_REQ].buf + buffer[CONFBUF_REQ].off);

            cflen = CID_ConfBufGet(&(buffer[CONFBUF_REQ]), cidcb);
            if (buffer[CONFBUF_REQ].off < buffer[CONFBUF_REQ].len)
            {
                cfcon = TRUE;
            }
        }
        else
        {
            /* Generate high-level configuration */
            L2CA_CONFIG_T config;

            /* Enforce lockstep */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
            if(cidcb->use_lockstep &&
               !(cidcb->remote_pending) &&
               (signal_args->result == L2CA_CONFIG_SUCCESS))
            {
                CID_DisconnectReqInternal(cidcb,
                        L2CA_DISCONNECT_LOCKSTEP_ERROR);
                return CID_EVENT_NULL;
            }
#endif

            cfstr = buffer[CONFBUF_CFM].buf;
            cflen = buffer[CONFBUF_CFM].len;
            CID_GenConfigUpstream(&cfstr, &cflen, &config);

            /* Send config configuration to higher layer, except when
             * we're using (non-failed) lockstep. Ownership of pointer
             * members of config pass to function */
            if (signal_args->result != L2CA_CONFIG_PENDING
                    && (signal_args->result != L2CA_CONFIG_SUCCESS
                        || !CID_GetAnyPending(cidcb)))
            {
                L2CA_ConfigCfm(cidcb,
                        (l2ca_conf_result_t)signal_args->result,
                        &config);
            }

            /* Check if remote side rejected local parameters */
            switch(signal_args->result)
            {
                case L2CA_CONFIG_SUCCESS:
                {
                    /* Set 'local config OK' flag and see if we can
                     * break out of the configuration */
                    cidcb->local_config_ok = TRUE;
                    CID_ParseConfiguration(cidcb);
                    event = CID_ConfigComplete(cidcb);
                }
                break;

#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
                case L2CA_CONFIG_PENDING:
                {
                    /* If both sides are pending, ask for the logical
                     * channel id and/or do the QoS */
                    CID_SetRemotePending(cidcb, TRUE);
                    CID_ParseConfiguration(cidcb);
                    event = CID_LogicalSetup(cidcb);
                }
                break;
#endif

                default:
                {
                    /* Peer has has rejected the applications's
                     * configuration. We can then clear the "req"
                     * confbuffer */
                    CID_ConfBufFree(cidcb, CONFBUF_REQ);
                }
                break;
            }
        }
    }
    else
    {
        /* Out of memory due to peer */
        L2CA_CONFIG_T config;

        /* As the config-rsp from the peer usually marks the end of a
         * configuration phase all we can do is tell the upper layer
         * that the peer caused an out-of-memory condition */
        CsrMemSet(&config, 0, sizeof(L2CA_CONFIG_T));
        L2CA_ConfigCfm(cidcb,
                       L2CA_CONFIG_OUT_OF_MEM,
                       &config);
    }

    if(signal_args->flags & L2CAP_SIGNAL_CONTINUATION)
    {
        SIG_SIGNAL_T *sig_ptr = NULL;
        sig_ptr = SIG_SignalCreateConfigSignal(cidcb,
                                               cflen,  /* length of options */
                                               cfstr,  /* options */
                                               cfcon,  /* continuation */
                                               0,
                                               L2CA_CONFIG_SUCCESS);
        /* Send the signal */
        SIGH_SignalSend(cidcb->chcb, sig_ptr);
    }

    /* Still configuring, no more events */
    return event;

}

/*! \brief Configuration has completed successfully */
fsm_event_t CID_FsmConfigOk(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    fsm_event_t event;
    PARAM_UNUSED(arg2);

    /* When lockstep is used, we need to pass up the ConfigCfm as the
     * very last step */
    if(CID_GetAnyPending(cidcb))
    {
        const CsrUint8 *cfstr = NULL;
        CsrUint16 cflen = 0;
        L2CAP_CONFIG_BUFFER_T *buffer = cidcb->confbuf->buffer;
        L2CA_CONFIG_T config;

        cfstr = buffer[CONFBUF_CFM].buf;
        cflen = buffer[CONFBUF_CFM].len;
        CID_GenConfigUpstream(&cfstr, &cflen, &config);

        L2CA_ConfigCfm(cidcb,
                       L2CA_CONFIG_SUCCESS,
                       &config);
    }

    /* Reset configuration negotiation in case someone starts a
     * reconfiguration */
    cidcb->local_config_ok = FALSE;
    cidcb->remote_config_ok = FALSE;
    CID_SetLocalPending(cidcb, FALSE);
    CID_SetRemotePending(cidcb, FALSE);
    cidcb->opened = TRUE;

    /* Cancel config timer */
    timer_cancel(&cidcb->config_timer_id);

    /* Free config request buffer */
    CID_ConfBufFree(cidcb, CONFBUF_ALL);

#ifdef INSTALL_AMP_SUPPORT
    /* Special care needed for AMP channels */
    if(CID_IsAmp(cidcb))
    {
        AMP_SetState(cidcb, AMPST_RUNNING);
        AMP_FinaliseSetup(cidcb);
        AMP_PruneQueues(cidcb->chcb);

        /* Trigger an AMP cleanup */
        event = CID_EV_CID_AMP_CLEANUP_REQ;
    }
    else
#endif
    {
        /* Not AMP enabled - don't do anything */
        event = CID_EVENT_NULL;
    }

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    /* Resume (reschedule) after reconfigurations. For moving channels
     * we'll use the AMP extensions to the CID FSM to deal with
     * unchoking */
    if(CID_IsFlowControl(cidcb) && !CID_IsMoving(cidcb))
    {
        FLOW_Choke(cidcb, FCOK_OFF);
    }
#endif

    return event;
}

/*! \brief Internal L2CAP disconnect.

    This is a nasty function that allows L2CAP to gracefully close a
    channel internally, ie. tell upper layer that channel is dead and
    send a disconnect request to peer
*/
fsm_event_t CID_FsmDisconnectReqInternal(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    l2ca_disc_result_t *reason = (l2ca_disc_result_t*)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Create disconnect signal */
    sig_ptr = SIG_SignalCreateDisconnectReq(cidcb->remote_cid,
                                            cidcb->local_cid);

    /* Send the signal */
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Tell upper layer */
    L2CA_DisconnectInd(cidcb, 0, *reason);

    return CID_EVENT_NULL;
}

/*! \brief Disconnect indication from peer in internal disconnect state

    We've sent the upper layer a disconnect indication, so reply to
    this one ourselves
*/
fsm_event_t CID_FsmDisconnectIndInternal(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_DISCONNECT_REQ_T *signal_args = (CID_DISCONNECT_REQ_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    sig_ptr = SIG_SignalCreateDisconnectRsp(cidcb->local_cid,
                                            cidcb->remote_cid,
                                            signal_args->signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Peer responds to our internal disconnect request

    Peer/application has acknowledged our disconnect request, so kill
    the CID. This event is also used if a connectionless group is
    closed.
*/
fsm_event_t CID_FsmDisconnectCfmInternal(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Disconnect request from higher layer */
fsm_event_t CID_FsmDisconnectReq(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    SIG_SIGNAL_T *sig_ptr;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Create disconnect signal */
    sig_ptr = SIG_SignalCreateDisconnectReq(cidcb->remote_cid, cidcb->local_cid);

    /* Send the signal */
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Disconnect indication from peer */
fsm_event_t CID_FsmDisconnectInd(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_DISCONNECT_REQ_T *signal_args = (CID_DISCONNECT_REQ_T *)arg2;

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Send disconnection indication to higher layer */
    L2CA_DisconnectInd(cidcb, signal_args->signal_id, L2CA_DISCONNECT_NORMAL);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Disconnect req/ind crossover

    If we have colliding disconnect requests, we need to make sure
    that the CIDCB is torn down immediately and that the application
    sees this a a normal, successful disconnect.
*/
fsm_event_t CID_FsmDisconnectIndCross(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CID_DISCONNECT_REQ_T *signal_args = (CID_DISCONNECT_REQ_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    /* Respond to peer. We don't care whether this signal actually
     * makes it across before the ACL dies. */
    sig_ptr = SIG_SignalCreateDisconnectRsp(cidcb->local_cid,
                                            cidcb->remote_cid,
                                            signal_args->signal_id);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Tell app that we're dead */
    L2CA_DisconnectCfm(cidcb, L2CA_DISCONNECT_NORMAL);


    /* Detach ourself from PSM registration to allow it to be deleted. */
    cidcb->local_psm = NULL;

    /* Wait for NCP event before destroying CIDCB. */

    /* No more events */
    return CID_EVENT_NULL;
}


/*! \brief Disconnect confirmation from peer */
fsm_event_t CID_FsmDisconnectCfm(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Send confirmation to higher layer */
    L2CA_DisconnectCfm(cidcb, L2CA_DISCONNECT_NORMAL);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Send Disconnect response to peer */
fsm_event_t CID_FsmDisconnectRsp(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    L2CA_DISCONNECT_RSP_T *prim = (L2CA_DISCONNECT_RSP_T *)arg2;
    SIG_SIGNAL_T *sig_ptr;

    sig_ptr = SIG_SignalCreateDisconnectRsp(cidcb->local_cid,
                                            cidcb->remote_cid,
                                            prim->identifier);

    /* Send the signal */
    SIGH_DuplicateSetResponse(cidcb->chcb, sig_ptr);
    SIGH_SignalSend(cidcb->chcb, sig_ptr);

    /* Detach ourself from PSM registration to allow it to be deleted. */
    cidcb->local_psm = NULL;

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Disconnect response signal has been sent, kill CIDCB

    In peer-initiated disconnects we reply with a disconnect response
    signal. We also defer destroying the CIDCB structure until the HCI
    layer has told us that the signal has actually been sent over the
    air by means of the NCP event.
*/
fsm_event_t CID_FsmDisconnectRspNcp(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Destroy this instance.
     *
     * We are destroying a channel after peer-initiated disconnect. We
     * allow the security manager to determine the idle timeout for
     * the ACL. The timeout here is supposed to give the remote device
     * sufficient time to ask its application for PIN codes and stuff
     * before it can initiate a subsequent connection.
     */
    CID_Destroy(cidcb, HCI_ERROR_OETC_USER,
                dm_sm_acl_timeout_security(CH_GET_ACL(cidcb->chcb)));

    /* No more events */
    return CID_EVENT_NULL;
}


/*! \brief Close CID instance due to connection signalling timeout */
fsm_event_t CID_FsmConnectCfmTimeout(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Send negative confirm */
    CID_SendConnectCfm(cidcb, L2CA_CONNECT_TIMEOUT);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Close CID instance due to ACL link loss */
fsm_event_t CID_FsmDisconnectIndAcl(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    CsrUint16 reason = *(CsrUint16 *)arg2;

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Send L2CA_DISCONNECT_IND */
    if(L2CAP_IS_ERROR_NORMAL(reason))
    {
        /* We disconnected abruptly in connected state, but status */
        /* indicates the remote device did it intentionally. Mark it */
        /* as normal */
        L2CA_DisconnectInd(cidcb, 0, L2CA_DISCONNECT_NORMAL);
    }
    else
    {
        /* If it's not intentional, we deem it as link-loss */
        L2CA_DisconnectInd(cidcb, 0, L2CA_DISCONNECT_LINK_LOSS);
    }

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Close CID instance due to reject command from peer */
fsm_event_t CID_FsmDisconnectIndInvalidCid(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Send L2CA_DISCONNECT_IND */
    L2CA_DisconnectInd(cidcb, 0, L2CA_DISCONNECT_PEER_REJECT_CID);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Signalling timeout, report disconnection to upper layers */
fsm_event_t CID_FsmDisconnectCfmTimeout(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Send confirmation to higher layer */
    L2CA_DisconnectCfm(cidcb, L2CA_DISCONNECT_TIMEOUT);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

/*! \brief Signalling timeout, report disconnection to upper layers */
fsm_event_t CID_FsmDisconnectIndTimeout(void *arg1, void *arg2)
{
    CIDCB_T *cidcb = (CIDCB_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Remove signals and configuration */
    CID_DisconnectCleanup(cidcb);

    /* Send indication to upper layer */
    L2CA_DisconnectInd(cidcb, 0, L2CA_DISCONNECT_TIMEOUT);

    /* Destroy this instance */
    CID_StandardDestroy(cidcb);

    /* No more events */
    return CID_EVENT_NULL;
}

#endif



