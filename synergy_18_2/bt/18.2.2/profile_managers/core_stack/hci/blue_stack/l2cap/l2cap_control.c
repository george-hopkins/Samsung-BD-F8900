/*!
   Copyright Cambridge Silicon Radio Limited and its licensors 2012. 
     All rights reserved.

\file   l2cap_control.c

\brief  Miscellenous control functions for handling PSMs, fixed
        channels, connectionless rx/tx setup, etc.  We also control
        creation of CIDCBs for both incoming and outgoing connections.
*/

#include "csr_synergy.h"

#include "csr_util.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "csr_bt_common.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "tbdaddr.h"

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_control.h"
#include "l2cap_chme.h"
#include "l2cap_cidme.h"
#include "l2cap_con_handle.h"
#include "l2cap_sig_handle.h"
#include "l2cap_interface.h"
#include "l2cap_flow.h"
#include "l2cap_cid_fsm_definitions.h"
#ifdef INSTALL_AMP_SUPPORT
#include "l2cap_amp_utils.h"
#endif

#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "dm_layer_manager.h"

#include "l2caplib.h"

/*! \brief L2CAP master control block

    This is the only global variable left in L2CAP. It's
    auto-initialised to zero on-chip. When on-host, the MCB_Init()
    function makes sure it's zero-initialised.
*/
MCB_T mcb;

/*! \brief Find protocol

    This function is called to find protocol instance from the specified PSM.

    \param psm PSM.
    \return Pointer to PSM instance.
*/
PSM_T *MCB_FindProtocol(psm_t psm)
{
    PSM_T *psm_ptr;

    for (psm_ptr = mcb.psm_list; psm_ptr != NULL; psm_ptr = psm_ptr->next_ptr)
    {
        if (psm_ptr->psm == psm)
        {
            return psm_ptr;
        }
    }
    return NULL;
}

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
/*! \brief Find fixed channel

    This function is called to find a registered fixed channel.

    \param cid CID
    \return Pointer to fixed channel instance.
*/
static FIXCID_T *MCB_FindFixedCid(l2ca_cid_t cid)
{
    FIXCID_T *cid_ptr;

    for (cid_ptr = mcb.fixcid_list;
         cid_ptr != NULL;
         cid_ptr = cid_ptr->next_ptr)
    {
        if (cid_ptr->cid == cid)
        {
            return cid_ptr;
        }
    }
    return NULL;
}

/*! \brief Find fixed channel (CID numerical value)
    from dynamic CID instance

    This function is called to find a registered fixed channel
    numerical value from dynamic CID instance which is mapped
    to the fixed channel.

    \param pointer to dynamic cid instance
    \return fixed cid CID value
*/
static l2ca_cid_t MCB_FindFixedCidFromDynamicCid(CIDCB_T *cidcb)
{
    FIXCID_T *cid_ptr;

    for (cid_ptr = mcb.fixcid_list;
         cid_ptr != NULL;
         cid_ptr = cid_ptr->next_ptr)
    {
        if (cid_ptr->p_handle== cidcb->p_handle)
        {
            return cid_ptr->cid;
        }
    }
    return L2CA_CID_INVALID;
}
#endif

/*! \brief Initialise L2CAP control.

    This function is called to initialise the L2CAP control module.
*/
void MCB_Init(void)
{
#ifdef BUILD_FOR_HOST
    /* Zero initialise the only global variable we've got */
    CsrMemSet(&(mcb), 0, sizeof(MCB_T));
#endif
}

/*! \brief Indication from the DM that the HCI interface data path is ready.

    Send off the L2CAP register confirmations.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void MCB_DeviceReady(void)
{
    PSM_T *psm_ptr;

    /* Send off the register confirms */
    for(psm_ptr = mcb.psm_list; psm_ptr != NULL; psm_ptr = psm_ptr->next_ptr)
    {
        L2CA_RegisterCfm(psm_ptr->p_handle,
                         psm_ptr->psm,
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                         psm_ptr->mode_mask,
#else
                         L2CA_MODE_MASK_BASIC,
#endif
                         L2CA_MISC_SUCCESS,
                         psm_ptr->reg_ctx,
                         psm_ptr->regflags);
    }
}
#endif

/*! \brief Register PSM.

    This function registers a PSM, if PSM already registered then update
    P-Handle and connectionless flag otherwise register new PSM.  Send
    confirmation if the HCI packet sizes has been set.

    \param psm PSM to register.
    \param phandle Protocol handle for this PSM.
    \param mode_mask Mask of L2CAP modes this PSM supports.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void MCB_RegisterProtocol(L2CA_REGISTER_REQ_T *req)
{
    CsrUint16 mode_mask = req->mode_mask;
    l2ca_misc_result_t result = L2CA_MISC_INVALID_PSM;

    if (req->psm_local == L2CA_PSM_INVALID)
    {
        CsrUint16 psm;

        result = L2CA_MISC_FAILED;

        /* We must register an unused valid PSM from the dynamic range.
           This shouldn't need to be fast and there shouldn't be that many
           PSMs registerd anyway, so do it the easy way. Start from one end
           of the range of Dynamic PSMs and work our way to the other until
           we find one that hasn't been used. We start from the high end
           because it makes the sums easier. */
        for (psm = L2CA_PSM_DYNAMIC_MAX;
                psm >= L2CA_PSM_DYNAMIC_MIN;
                psm = (psm - 2) & ~L2CA_PSM_MASK_INVALID)
        {
            if (MCB_FindProtocol(psm) == NULL)
            {
                /* Found one that's not already registered. */
                req->psm_local = psm;
                break;
            }
        }
    }

    /* PSMs *must* be odd numbered (L2CA_PSM_MASK_REQUIRED) and they must
       have the least significant bit of the more significant octet
       (L2CA_PSM_MASK_INVALID) unset. */
    if ((req->psm_local & (L2CA_PSM_MASK_INVALID | L2CA_PSM_MASK_REQUIRED))
            == L2CA_PSM_MASK_REQUIRED)
    {
        /* Reject if PSM has already been registered. */
        result = L2CA_MISC_ALREADY_REGISTERED;
        if (MCB_FindProtocol(req->psm_local) == NULL)
        {
            PSM_T *psm_ptr;
            result = L2CA_MISC_OUT_OF_MEM;

            if ((psm_ptr = xpnew(PSM_T)) != NULL)
            {
                result = L2CA_MISC_SUCCESS;

                psm_ptr->psm = req->psm_local;
                psm_ptr->next_ptr = mcb.psm_list;
                mcb.psm_list = psm_ptr;

                /* Update PSM with P-Handle */
                psm_ptr->p_handle = req->phandle;

                /* Broadcast connectionless, UCD and autoconnect. */
                psm_ptr->regflags = req->flags;

                /* Registration time opaque context. */
                psm_ptr->reg_ctx = req->reg_ctx;

                /* Set mode mask and auto-connect support. Remove legacy modes
                 * from bitmask */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                mode_mask &= (CsrUint16)~(L2CA_MODE_MASK_RTM_OBSOLETE
                        | L2CA_MODE_MASK_FC_OBSOLETE);
                psm_ptr->mode_mask = mode_mask;
#else
               mode_mask = L2CA_MODE_MASK_BASIC;
#endif
            }
        }
    }

    /* Check if we can send register confirmation immediately */
    if (LYMCB.fc.data_block_length != 0 || result != L2CA_MISC_SUCCESS
#ifdef SUPPORT_SEPARATE_LE_BUFFERS
         || LYMCB.le_fc.data_block_length != 0
#endif
    )
    {
        L2CA_RegisterCfm(req->phandle,
                         req->psm_local,
                         mode_mask,
                         result,
                         req->reg_ctx,
                         req->flags);
    }
}
#endif

/*! \brief Unregister PSM

    The application has requested to unregister a previously registed
    PSM. If the PSM exists, make sure no active CIDCBs are currently
    using it. If that's ok, free it and inform application
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void MCB_UnregisterProtocol(L2CA_UNREGISTER_REQ_T *req)
{
    PSM_T *psm;
    PSM_T **psmp;
    CsrUint16 reg_ctx = 0;
    l2ca_misc_result_t result = L2CA_MISC_INVALID_PSM;

    /* Double indirect search for PSM */
    for (psmp = &mcb.psm_list; (psm = *psmp) != NULL; psmp = &psm->next_ptr)
    {
        if (psm->psm == req->psm_local)
        {
            CIDCB_T **cidcbp;

            /* Match found */
            result = L2CA_MISC_SUCCESS;

            /* Scan for CIDCB using this PSM */
            for (cidcbp = mcb.cid_table;
                    cidcbp < mcb.cid_table + L2CAP_MAX_NUM_CIDS; ++cidcbp)
            {
                if (*cidcbp != NULL && (*cidcbp)->local_psm == psm)
                {
                    /* PSM still in use */
                    result = L2CA_MISC_FAILED;
                    break;
                }
            }

            /* If still success, PSM can be freed */
            if (result == L2CA_MISC_SUCCESS)
            {
                /* Keep context to send in confirmation. */
                reg_ctx = psm->reg_ctx;

                /* Cut out this PSM and free it */
                *psmp = psm->next_ptr;
                CsrPmemFree(psm);
            }

            break;
        }
    }

    /* Send result back to application */
    L2CA_UnregisterCfm(req->phandle, req->psm_local, result, reg_ctx);
}
#endif

/*! \brief Register fixed CID

    This registers a fixed channel CID, if CID already exists then update
    protocol handle otherwise register the new CID.  Confirmation is sent to the
    higher layer.

    \param req Pointer to L2CA_REGISTER_FIXED_CID_REQ primitive.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
void MCB_RegisterFixedCid(L2CA_REGISTER_FIXED_CID_REQ_T *req)
{
    FIXCID_T *cid_ptr;

    /* Sanity check for CID range */
    if((req->fixed_cid <= L2CA_CID_CONNECTIONLESS) ||
       (req->fixed_cid >= L2CA_CID_DYNAMIC_FIRST))
    {
        L2CA_RegisterFixedCidCfm(req->phandle, req->fixed_cid,
                                 L2CA_MISC_INVALID_CID);
        return;
    }

    /* Find existing fixed CID */
    cid_ptr = MCB_FindFixedCid(req->fixed_cid);

    /* Create FIXCID if it doesn't exist already */
    if(cid_ptr == NULL)
    {
        cid_ptr = xzpnew(FIXCID_T);
        if(cid_ptr != NULL)
        {
            cid_ptr->cid = req->fixed_cid;
            cid_ptr->next_ptr = mcb.fixcid_list;
            mcb.fixcid_list = cid_ptr;
        }
    }

    /* Common setup for both existing and new FIXCIDs */
    if(cid_ptr != NULL)
    {
        /* Update protocol handle */
        cid_ptr->p_handle = req->phandle;

        /* Free old configuration */
        L2CA_FreeConfigPtrs(&cid_ptr->config);

        /* Update configuration */
        cid_ptr->config = req->config;

        /* Unlink config pointers from primitive */
        CsrMemSet(&req->config, 0, sizeof(L2CA_CONFIG_T));

        /* Send confirmation */
        L2CA_RegisterFixedCidCfm(cid_ptr->p_handle, cid_ptr->cid,
                                 L2CA_MISC_SUCCESS);
    }
    else
    {
        L2CA_RegisterFixedCidCfm(req->phandle, req->fixed_cid,
                                 L2CA_MISC_OUT_OF_MEM);
    }
}
#endif

/*! \brief Get phandle.

    Get the corresponding phandle for the PSM.

    \param psm PSM.
    \return P-Handle for the specified PSM.
*/
CsrSchedQid MCB_GetPhandle(psm_t psm)
{
    PSM_T *psm_ptr = MCB_FindProtocol(psm);
    return (psm_ptr != NULL) ? psm_ptr->p_handle : CSR_SCHED_QID_INVALID;
}

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
/*! \brief Get P-Handle from CID

    Get the corresponding P-Handle for the fixed channel ID.

    \param cid CID.
    \return P-Handle for the specified CID.
 */
CsrSchedQid MCB_GetPhandleFromFixedCid(l2ca_cid_t cid)
{
    FIXCID_T *cid_ptr = MCB_FindFixedCid(cid);
    return (cid_ptr != NULL) ? cid_ptr->p_handle : CSR_SCHED_QID_INVALID;
}
#endif

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
/*! \brief Get Config from CID

    Get the corresponding channel config for the fixed channel ID.

    \param cid CID.
    \return Config for the specified CID.
 */
L2CA_CONFIG_T *MCB_GetConfigFromFixedCid(l2ca_cid_t cid)
{
    FIXCID_T *cid_ptr = MCB_FindFixedCid(cid);
    return (cid_ptr != NULL) ? &cid_ptr->config : NULL;
}
#endif

/*! \brief Set raw data reception mode for a channel/chcb
 */
#ifdef INSTALL_L2CAP_RAW_SUPPORT
void MCB_SetRawMode(L2CA_RAW_MODE_REQ_T *req)
{
    L2CAP_CHCB_T *chcb = NULL;
    l2ca_raw_t raw_mode = req->raw_mode;

    if (req->cid == L2CA_CID_INVALID)
    {
        /* Modify first CHCB if possible */
        chcb = CH_GET_CHCB(DM_ACL_FIRST());
    }
    else
    {
        CIDCB_T *cidcb;

        /* Obtain CID instance */
        if ((cidcb = CIDME_GetCidcb(req->cid)) != NULL)
        {
            chcb = cidcb->chcb;
            cidcb->raw_rx = raw_mode == L2CA_RAW_MODE_BYPASS_CHANNEL;

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
            if(CID_IsFlowControl(cidcb) &&
                   (raw_mode == L2CA_RAW_MODE_BYPASS_CHANNEL
                        || raw_mode == L2CA_RAW_MODE_BYPASS_CONNECTION))
            {
                /* Make sure timers are disabled */
                FLOWINFO_T *flow = cidcb->flow;
                timer_cancel(&flow->retrans_timer);
                timer_cancel(&flow->monitor_timer);
                timer_cancel(&flow->ack_timer);
            }
#endif
        }
    }

    /* Modify the CHCB */
    if(chcb != NULL)
    {
        chcb->raw_crc_rejects = raw_mode == L2CA_RAW_MODE_CRC_REJECTS;
        chcb->raw_rx = raw_mode == L2CA_RAW_MODE_BYPASS_CONNECTION;
        chcb->raw_hci_sniff = raw_mode == L2CA_RAW_MODE_SNIFF_HCI_DATA;
        chcb->raw_handle = req->phandle;
    }
}
#endif

/*! \brief Low water mark background interrupt

    This is a background interrupt that gets called by the LC or
    radioio when the receive ACL/MBLK buffers are getting full. At
    this point we must flush as much data as possible in order not to
    enter a systemwide deadlock!
*/
#ifndef BUILD_FOR_HOST
#define MCB_LowMemory l2cap_low_memory
void MCB_LowMemory(void)
{
    l2ca_cid_t index;

    /* Scan list */
    for(index = 0; index < L2CAP_MAX_NUM_CIDS; index++)
    {
        CIDCB_T *cidcb = mcb.cid_table[index];

        /* Return pointer if context matches */
        if(cidcb != NULL)
        {
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
            if(cidcb->flow != NULL)
            {
                FLOW_LowMemory(cidcb, cidcb->flow);
            }
#endif
        }
    }
}
#endif

/* CID fifo */
/*! \brief Add new CID to retry_cid_fifo
    We add an element to the end of the linked list.

    \param cid L2CAP channel ID.
*/
void MCB_RetryCidFifoPush(l2ca_cid_t cid)
{
    L2CAP_CID_LIST_T **cid_ptr_ptr, *cid_ptr;

    for (cid_ptr_ptr = &mcb.retry_cid_fifo;
            (cid_ptr = *cid_ptr_ptr) != NULL;
            cid_ptr_ptr = &cid_ptr->next_ptr)
        ;

    cid_ptr = zpnew(L2CAP_CID_LIST_T);
    cid_ptr->cid = cid;
    *cid_ptr_ptr = cid_ptr;
}

/*! \brief Remove CID from retry_cid_fifo
    We remove an element from the start of the linked list.

    \returns CID from fifo, or L2CA_CID_INVALID if empty.
*/
l2ca_cid_t MCB_RetryCidFifoPop(void)
{
    l2ca_cid_t cid = L2CA_CID_INVALID;

    if (mcb.retry_cid_fifo != NULL)
    {
        L2CAP_CID_LIST_T *tmp;

        tmp = mcb.retry_cid_fifo;
        mcb.retry_cid_fifo = mcb.retry_cid_fifo->next_ptr;
        cid = tmp->cid;
        CsrPmemFree(tmp);
    }

    return cid;
}

/*! \brief Examine a CID to see if it is available for use.
    \param cid L2CAP channel ID.
    \returns TRUE if CID is available. FALSE if CID is
             unavailable because of existing or reserved CID.
*/
CsrBool MCB_CidIsAvailable(l2ca_cid_t cid)
{
    L2CAP_CID_LIST_T *cid_ptr;

    /* We're only interested in the part of the cid used as an index. */
    cid &= L2CAP_CID_NUMBER_MASK;

    /* Make sure that CID is not currently in use. */
    if (cid >= L2CAP_MAX_NUM_CIDS || mcb.cid_table[cid] != NULL)
        return FALSE;

    /* Make sure that CID is not reserved for L2CAP connect retry. */
    for (cid_ptr = mcb.retry_cid_fifo; cid_ptr != NULL; cid_ptr = cid_ptr->next_ptr)
        if ((cid_ptr->cid & L2CAP_CID_NUMBER_MASK) == cid)
            return FALSE;

    return TRUE;
}


/*! \brief Ping Request

    Received ping request from higher layer protocol. Instance connection
    handle if necessary and start connecting.

    \param prim Pointer to L2CA_PING_REQ primitive.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void MCB_PingReq(L2CA_PING_REQ_T *prim)
{
    L2CAP_CHCB_T *chcb;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, &prim->bd_addr);

    if (CH_Connect(
            &addrt,
            L2CA_PSM_INVALID,
            &chcb,
            L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR)) == L2CA_CONNECT_SUCCESS)
    {
        SIG_SIGNAL_T *sig_ptr;

        /* Create signal structure */
        sig_ptr = SIG_SignalCreateEchoReq(prim->phandle, prim->data,
                                          prim->length, prim->req_ctx);

        SIGH_SignalSend(chcb, sig_ptr);
    }
    else
    {
        /* Reject the request */
        L2CA_PingCfm(prim->phandle, &TBDADDR_ADDR(addrt), L2CA_MISC_FAILED,
                     NULL, 0, prim->req_ctx);
    }
}

/*! \brief GetInfo Request

    Received getinfo request from higher layer protocol. Instance connection
    handle if necessary and start connecting.

    \param prim Pointer to L2CA_GETINFO_REQ primitive.
*/
void MCB_GetInfoReq(L2CA_GETINFO_REQ_T *prim)
{
    L2CAP_CHCB_T *chcb;
    TYPED_BD_ADDR_T addrt;

    tbdaddr_copy_from_public_bd_addr(&addrt, &prim->bd_addr);

    if (CH_Connect(
            &addrt,
            L2CA_PSM_INVALID,
            &chcb,
            L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR)) == L2CA_CONNECT_SUCCESS)
    {
        SIG_SIGNAL_T *sig_ptr;

        /* Create signal structure */
        sig_ptr = SIG_SignalCreateGetInfoReq(prim->phandle, prim->info_type, prim->req_ctx);
        SIGH_SignalSend(chcb, sig_ptr);
    }
    else
    {
        /* Reject the request */
        L2CA_GetInfoCfm(prim->phandle,
                        &TBDADDR_ADDR(addrt),
                        L2CA_GETINFO_FAILED,
                        prim->info_type,
                        NULL,
                        0,
                        prim->req_ctx);
    }
}

/*! \brief Normal outgoing or connectionless connection setup

    This is the main handler for a L2CA_CONNECT_REQ.
 */
void MCB_ConnectReq(L2CA_CONNECT_REQ_T *req)
{
    l2ca_cid_t local_cid = L2CA_CID_INVALID;
    l2ca_cid_t cfm_cid = L2CA_CID_INVALID;
    l2ca_conn_result_t cfm_result;
    CIDCB_T *cidcb;
    L2CAP_CHCB_T *chcb;
    PSM_T *psm_reg;

    /* Genuine PSMs must be odd. An even PSM at this stage means that
     * this is a connect retry for psm_local + 1. */
    if((req->psm_local & 1) == 0)
    {
        ++req->psm_local;
        local_cid = MCB_RetryCidFifoPop();
    }

    /* Get the phandle from the PSM. */
    if ((psm_reg = MCB_FindProtocol(req->psm_local)) == NULL)
        return;

    /* Alloc CIDCB */
    cfm_result = L2CA_CONNECT_OUT_OF_MEM;
    if((cidcb = CIDME_NewCidcb(local_cid)) != NULL)
    {
        TYPED_BD_ADDR_T addrt;
        tbdaddr_copy_from_public_bd_addr(&addrt, &req->bd_addr);

        /* Connect ACL */
        if ((cfm_result = CH_Connect(&addrt,
                                     req->psm_local,
                                     &chcb,
                                     L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR)))
                == L2CA_CONNECT_SUCCESS)
        {
            cfm_result = L2CA_CONNECT_INITIATING;

            /* Initialise CIDCB variables */
            CIDME_InitCidcb(cidcb,
                            chcb,
                            L2CA_CID_INVALID, /* remote */
                            psm_reg,
                            req->psm_remote,
                            req->con_ctx);
            cidcb->next_ptr  = chcb->cidcb_list;
            chcb->cidcb_list = cidcb;

            cfm_cid = cidcb->local_cid;
        }
        else
        {
            CIDME_FreeCidcb(cidcb);
            cidcb = NULL;
        }
    }

    L2CA_ConnectCfm(&req->bd_addr, cfm_cid, psm_reg->p_handle, req->psm_local,
            cfm_result, req->con_ctx, psm_reg->reg_ctx);

    if (cidcb != NULL)
    {
        if (req->service.protocol_id != SEC_PROTOCOL_NONE)
            cidcb->dm_sm_service =
                pcopy(&req->service, sizeof(DM_SM_SERVICE_T));

        /* Crank FSM and inform app */
        CID_ConnectReq(cidcb);
        if(CH_IS_CONNECTED(cidcb->chcb))
            CID_AclReady(cidcb);
    }
}

/*! \brief Normal incoming connection

    Peer has sent a L2CAP connect request signal for a valid PSM. Create
    the CIDCB structure and kick the state machine.
 */
CsrUint16 MCB_ConnectInd(L2CAP_CHCB_T *chcb, CID_CONNECT_REQ_T *sig_args)
{
    CIDCB_T *cidcb;
    PSM_T *psm_reg;

    if ((psm_reg = MCB_FindProtocol(sig_args->psm)) == NULL)
        return L2CA_CONNECT_REJ_PSM;

    if ((cidcb = CIDME_GetCidcbRemoteWithHandle(chcb, sig_args->source_cid))
            != NULL)
    {
        CID_ConnectIndRetry(cidcb, sig_args);
        return L2CA_CONNECT_SUCCESS;
    }

    if ((cidcb = CIDME_NewCidcb(L2CA_CID_INVALID)) == NULL)
        return L2CA_CONNECT_REJ_RESOURCES;

    /* Connect ACL */
    if (CH_Connect(CH_GET_TBDADDR(chcb),
                   sig_args->psm,
                   &chcb,
                   L2CA_CONFLAG_INCOMING) != L2CA_CONNECT_SUCCESS)
    {
        CIDME_FreeCidcb(cidcb);

        return L2CA_CONNECT_REJ_RESOURCES;
    }

    /* Initialise CIDCB variables */
    CIDME_InitCidcb(cidcb,
                    chcb,
                    sig_args->source_cid,
                    psm_reg,
                    sig_args->psm,
                    0); /* context */

    cidcb->next_ptr  = chcb->cidcb_list;
    chcb->cidcb_list = cidcb;

    /* Crank the FSM */
    CID_ConnectInd(cidcb, sig_args);

    return L2CA_CONNECT_SUCCESS;
}

#endif

/*! \brief AMP outgoing connection

    Main handler for AMP create channel requests. This is similar to
    the connect request, except that the channel will be created for
    an AMP.
 */
#ifdef INSTALL_AMP_SUPPORT
void MCB_AmpCreateReq(L2CA_CREATE_CHANNEL_REQ_T *req)
{
    l2ca_cid_t local_cid;
    PSM_T *psm_reg;
    CIDCB_T *cidcb;
    l2ca_conn_result_t result;

    /* Genuine PSMs must be odd. An even PSM at this stage means that
     * this is a connect retry for psm_local + 1. */
    local_cid = L2CA_CID_INVALID;
    if((req->psm_local & 1) == 0)
    {
        ++req->psm_local;
        local_cid = MCB_RetryCidFifoPop();
    }

    /* Get the phandle from the PSM. */
    if ((psm_reg = MCB_FindProtocol(req->psm_local)) == NULL)
        return;

    /* Alloc CIDCB */
    result = L2CA_CONNECT_OUT_OF_MEM;
    if ((cidcb = CIDME_NewCidcb(local_cid)) != NULL)
    {
        TYPED_BD_ADDR_T addrt;
        L2CAP_CHCB_T *chcb;

        tbdaddr_copy_from_public_bd_addr(&addrt, &req->bd_addr);

        if ((result = CH_Connect(&addrt,
                                 req->psm_local,
                                 &chcb,
                                 L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR))) == L2CA_CONNECT_SUCCESS)
        {
            AMPINFO_T *info;

            result = L2CA_CONNECT_INITIATING;

            CIDME_InitCidcb(cidcb,
                            chcb,
                            L2CA_CID_INVALID,
                            psm_reg,
                            req->psm_remote,
                            req->con_ctx);
            cidcb->next_ptr  = chcb->cidcb_list;
            chcb->cidcb_list = cidcb;

            /* Enable for AMP */
            AMP_AllocInfo(cidcb);
            info = cidcb->amp_info;
            info->amp_state = AMPST_CREATING;
            info->local_amp = req->local_control;
            info->remote_amp = req->remote_control;
        }
        else
        {
            /* Can't connect ACL, so cleanup. */
            CIDME_FreeCidcb(cidcb);
            cidcb = NULL;
        }
    }

    L2CA_CreateChannelCfm(cidcb, req, psm_reg->p_handle, result);

    if (cidcb != NULL)
    {
        if (req->service.protocol_id != SEC_PROTOCOL_NONE)
            cidcb->dm_sm_service =
                pcopy(&req->service, sizeof(DM_SM_SERVICE_T));

        /* Crank FSM */
        CID_AmpCreateReq(cidcb);
        if(CH_IS_CONNECTED(cidcb->chcb))
            CID_AclReady(cidcb);
    }
}
#endif

/*! \brief AMP incoming connection

    Like a normal incoming L2CAP create request signal, except this
    deals with the AMP one.
 */
#ifdef INSTALL_AMP_SUPPORT
CsrUint16 MCB_AmpCreateInd(L2CAP_CHCB_T *chcb, CID_CREATE_CHAN_REQ_T *sig_args)
{
    PSM_T *psm_reg;
    AMPINFO_T *info;
    CIDCB_T *cidcb;

    if ((psm_reg = MCB_FindProtocol(sig_args->psm)) == NULL)
       return L2CA_CONNECT_REJ_PSM;

    if ((cidcb = CIDME_GetCidcbRemoteWithHandle(chcb, sig_args->source_cid))
            != NULL)
    {
        CID_AmpCreateIndRetry(cidcb, sig_args);
        return L2CA_CONNECT_SUCCESS;
    }

    if ((cidcb = CIDME_NewCidcb(L2CA_CID_INVALID)) == NULL)
        return L2CA_CONNECT_REJ_RESOURCES;

    /* Connect ACL */
    if (CH_Connect(CH_GET_TBDADDR(chcb),
                   sig_args->psm,
                   &chcb,
                   L2CA_CONFLAG_INCOMING) != L2CA_CONNECT_SUCCESS)
    {
        CIDME_FreeCidcb(cidcb);

        return L2CA_CONNECT_REJ_RESOURCES;
    }

    CIDME_InitCidcb(cidcb,
                    chcb,
                    sig_args->source_cid,
                    psm_reg,
                    sig_args->psm,
                    0); /* context */
    cidcb->next_ptr  = chcb->cidcb_list;
    chcb->cidcb_list = cidcb;

    /* Enable for AMP usage */
    AMP_AllocInfo(cidcb);
    info = cidcb->amp_info;
    info->amp_state = AMPST_CREATING;
    info->local_amp = sig_args->control_id;

    /* Crank the FSM */
    CID_AmpCreateInd(cidcb, sig_args);

    return L2CA_CONNECT_SUCCESS;
}
#endif

/*! \brief Map outgoing fixed channel

    Map a fixed channel in, possibly creating the ACL too.
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void MCB_MapFixedCidReq(L2CA_MAP_FIXED_CID_REQ_T *req)
{
    CsrSchedQid phandle;
    L2CA_MAP_FIXED_CID_REQ_T r = *req;
    CIDCB_T *cidcb = NULL;
    PSM_T *psm_reg = NULL;
    CsrBool lock_acl = FALSE;
    l2ca_misc_result_t result = L2CA_MISC_NO_CONNECTION;
    L2CAP_CHCB_T *chcb;

    /* Remove meaningless flags. */
    r.flags &= ~L2CA_CONFLAG_INCOMING;

    if ((r.flags & L2CA_CONFLAG_LOCK_ACL) != 0)
    {
        /* Make sure we ignore silly combinations. */
        if ((r.flags & L2CA_CONFLAG_ENUM_MASK) == L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR_BROADCAST))
            return;

        lock_acl = TRUE;
    }

    /* Make sure registration exists. Retrieve phandle. */
#if defined(INSTALL_L2CAP_CONNLESS_SUPPORT) \
    || defined(INSTALL_L2CAP_UCD_SUPPORT)
    if (r.fixed_cid == L2CA_CID_CONNECTIONLESS)
    {
        if ((psm_reg = MCB_FindProtocol(r.cl_local_psm)) == NULL)
            return;

        phandle = psm_reg->p_handle;
    }
    else
#endif /* INSTALL_L2CAP_CONNLESS_SUPPORT || INSTALL_L2CAP_UCD_SUPPORT */
    {
        if ((phandle = MCB_GetPhandleFromFixedCid(r.fixed_cid))
                == CSR_SCHED_QID_INVALID)
        {
            L2CA_BadMessage(sizeof(L2CA_MAP_FIXED_CID_REQ_T), req);
            return;
        }
    }

    /* Sort out connection handle control block. */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
    if (r.fixed_cid == L2CA_CID_CONNECTIONLESS
            && (r.flags & L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR_BROADCAST)) != 0)
    {
        /* Broadcast */
        result = L2CA_MISC_OUT_OF_MEM;

        if (mcb.connectionless == NULL)
            mcb.connectionless = xzpnew(L2CAP_CHCB_T);

        chcb = mcb.connectionless;
    }
    else
#endif /* INSTALL_L2CAP_CONNLESS_SUPPORT */
    {
        /* Unicast */
        result = L2CA_MISC_NO_CONNECTION;

        chcb = CHME_GetConHandleBdAddr(&r.addrt);

        if (lock_acl)
            result = (l2ca_misc_result_t)CH_Connect(&r.addrt,
                                                    L2CA_PSM_INVALID,
                                                    &chcb,
                                                    r.flags);
    }

    /* If CHCB creation didn't work, we're in trouble */
    /* Actually just an 'if'. We use 'while' so we can also use 'break' */
    while (chcb != NULL)
    {
        result = L2CA_MISC_SUCCESS;

        /* Peer may already have initiated a fixed channel towards us,
           so reuse that one if it is compatible. */
        if ((cidcb = CIDME_GetCLCidcbRemote(chcb, r.fixed_cid,
                        r.cl_local_psm)) == NULL
                || (r.fixed_cid == L2CA_CID_CONNECTIONLESS
                    && cidcb->remote_psm != L2CA_PSM_INVALID
                    && cidcb->remote_psm != r.cl_remote_psm))
        {
            /* No suitable CIDCB to reuse, so create a new one. */
            if ((cidcb = CIDME_NewCidcb(L2CA_CID_INVALID)) == NULL)
            {
                if (lock_acl)
                    CH_StandardClose(chcb);

                /* Failed, so jump to end */
                result = L2CA_MISC_OUT_OF_MEM;
                break;
            }

            cidcb->acl_unlocked = !lock_acl;

            if (!CH_IS_CONNECTIONLESS(chcb))
                result = L2CA_MISC_INITIATING;

            cidcb->p_handle = phandle;
            CIDME_InitCidcb(cidcb,
                            chcb,
                            r.fixed_cid,
                            psm_reg,
                            r.cl_remote_psm,
                            r.con_ctx);

            cidcb->next_ptr  = chcb->cidcb_list;
            chcb->cidcb_list = cidcb;
        }
        else if (lock_acl)
        {
            /* We're reusing an existing CIDCB but we've already applied a lock
               to the ACL. Now we must ensure consistency. */
            if (!cidcb->acl_unlocked)
            {
                /* CIDCB had already locked ACL. Now we've gone and locked
                   it a second time. So undo our locking. */
                CH_StandardClose(cidcb->chcb);
            }

            cidcb->acl_unlocked = FALSE;
        }

        /* Set remote PSM explicitly here in case we are reusing an existing
           CIDCB for UCD and the PSM is currently set to L2CAP_PSM_INVALID. */
        cidcb->remote_psm = r.cl_remote_psm;

        cidcb->context = r.con_ctx;
        cidcb->p_handle = phandle;
        /* Crank FSM and info app */
        CID_MapFixedCidReq(cidcb);
        break;
    }

    L2CA_MapFixedCidCfm(cidcb, req, phandle, result);

    /* Crank FSM and info app */
    if(cidcb != NULL && CH_IS_CONNECTED(cidcb->chcb))
        CID_AclReady(cidcb);
}
#endif

/*! \brief Map incoming fixed channel, indication
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
CIDCB_T *MCB_MapFixedCidInd(L2CAP_CHCB_T *chcb,
                            l2ca_cid_t fixed_cid,
                            psm_t cl_psm)
{
    CsrSchedQid phandle;
    CIDCB_T *cidcb = NULL;
    PSM_T *psm_reg = NULL;

    if ((phandle = MCB_GetPhandleFromFixedCid(fixed_cid)) != CSR_SCHED_QID_INVALID
            || (fixed_cid == L2CA_CID_CONNECTIONLESS
                && (psm_reg = MCB_FindProtocol(cl_psm)) != NULL
                && (phandle = psm_reg->p_handle) != CSR_SCHED_QID_INVALID))
    {
        if ((cidcb = CIDME_NewCidcb(L2CA_CID_INVALID)) != NULL)
        {
            cidcb->p_handle = phandle;
            CIDME_InitCidcb(cidcb,
                            chcb,
                            fixed_cid,
                            psm_reg,
                            L2CA_PSM_INVALID,
                            0); /* context */

            cidcb->acl_unlocked = TRUE;
            cidcb->next_ptr = chcb->cidcb_list;
            chcb->cidcb_list = cidcb;

            /* Crank FSM */
            CID_MapFixedCidInd(cidcb);
        }
    }

    return cidcb;
}
#endif

/*! \brief Fixed Channel map response

    Fixed channel is now up and running. Store context number and
    possibly lock the ACL.

    \param req Pointer to L2CA_MAP_FIXED_CID_RSP primitive.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void MCB_MapFixedCidRsp(L2CA_MAP_FIXED_CID_RSP_T *rsp)
{
    CIDCB_T *cidcb;

    cidcb = CIDME_GetCidcb(rsp->cid);
    if(cidcb != NULL)
    {
        cidcb->context = rsp->con_ctx;

#ifdef INSTALL_L2CAP_UCD_SUPPORT
        if (cidcb->remote_cid == L2CA_CID_CONNECTIONLESS)
        {
            if (!CH_IS_CONNECTIONLESS(cidcb->chcb))
                cidcb->remote_psm = rsp->ucd_remote_psm;
        }
#endif /* INSTALL_L2CAP_UCD_SUPPORT */

        /* Lock the ACL if requested. */
        if ((rsp->flags & L2CA_CONFLAG_LOCK_ACL) != 0
                && cidcb->acl_unlocked
                && !CH_IS_CONNECTIONLESS(cidcb->chcb))
        {
            L2CAP_CHCB_T *dummy_chcb;

            if (CH_Connect(CH_GET_TBDADDR(cidcb->chcb),
                           L2CA_PSM_INVALID,
                           &dummy_chcb,
                           (l2ca_conflags_t)(rsp->flags|L2CA_CONFLAG_INCOMING))
                                    == L2CA_CONNECT_SUCCESS)
            {
                cidcb->acl_unlocked = FALSE;
            }
        }
    }
}
#endif

/*! \brief Fixed Channel unmap request

    \param req Pointer to L2CA_UNMAP_FIXED_CID_REQ primitive.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void MCB_UnmapFixedCidReq(L2CA_UNMAP_FIXED_CID_REQ_T *req)
{
    CIDCB_T *cidcb;
    l2ca_cid_t fixed_cid=L2CA_CID_INVALID;

    cidcb = CIDME_GetCidcb(req->cid);
    if(cidcb != NULL)
    {
        /*
         * This change is required only for LE-ATT protocol.
         * In LE, ATT (CID=4) L2CAP channel is
         * created the moment ACL link is up. And destryoed when
         * ACL is down.
         * L2CAP should not unmap the CID of ATT protocol unless
         * ACL is down. Though ATT CID unmapping and ACL closure will
         * happen back to back in real time, but in race conditions
         * when
         * 1. ATT CID is mapped but ACL link is still connecting
         * 2. ATT issues disconnect and disconnect lead to HCI cancel create
         * connection
         * 3. Due to race, link is up before HCI cancel create connection is sent down
         * 4. HCI cancel create connection will fail.
         * 4. ATT will assume the link is down but actully it is not.
         *
         * Therfore the below change will cause unmap cid for ATT
         * only when ACL is down.
         */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
        fixed_cid=MCB_FindFixedCidFromDynamicCid(cidcb);
#endif
        if(L2CA_CID_ATTRIBUTE_PROTOCOL != fixed_cid)
        {
            /* Send response to upper layer. */
            L2CA_UnmapFixedCidInd(cidcb, L2CA_DISCONNECT_NORMAL);
        }

        /* Clean up everything -- this function will stop timers and flush
         * buffers, free the F&EC instance etc */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
        CID_DisconnectCleanup(cidcb);
#endif

        /* Unlock the ACL by closing it - use standard idle timeout  */
        if (!cidcb->acl_unlocked)
            CH_StandardClose(cidcb->chcb);

        if(L2CA_CID_ATTRIBUTE_PROTOCOL != fixed_cid)
        {
            /* Remove CIDCB from connection handle and free it */
            CH_RemoveCidcb(cidcb->chcb, cidcb);

#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
            CIDME_Unroute(cidcb);
#endif
            CIDME_FreeCidcb(cidcb);
            cidcb = NULL;
        }
    }
    else
        L2CA_BadMessage(sizeof(L2CA_UNMAP_FIXED_CID_REQ_T), req);
}
#endif

/*! \brief Determine whether we can receive connectionless data from air.

    \param psm L2CAP PSM of protocol.
    \param bc TRUE for broadcast, FALSE for unicast.
    \returns TRUE if we can receive, otherwise FALSE.
*/
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
CsrBool MCB_ReceiveConnectionlessAllowed(psm_t psm, CsrBool bc)
{
    PSM_T *protocol;

    return (protocol = MCB_FindProtocol(psm)) != NULL && (protocol->regflags &
            (bc ? L2CA_REGFLAG_ALLOW_RX_BCD : L2CA_REGFLAG_ALLOW_RX_UCD)) != 0;
}
#endif /* INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT */

#ifdef INSTALL_ULP

void L2CA_ConnUpdateReq(CsrSchedQid phandle,
                        const TYPED_BD_ADDR_T *addrt,
                        CsrUint16 min_interval,
                        CsrUint16 max_interval,
                        CsrUint16 latency,
                        CsrUint16 timeout
                        )
{
    L2CA_CONNECTION_PAR_UPDATE_REQ_T *prim = zpnew(L2CA_CONNECTION_PAR_UPDATE_REQ_T);

    prim->type = L2CA_CONNECTION_PAR_UPDATE_REQ;

    prim->phandle        = phandle;
    prim->addrt         = *addrt;
    prim->min_interval  = min_interval;
    prim->max_interval  = max_interval;
    prim->latency       = latency;
    prim->timeout       = timeout;

    L2CA_PutMsg(prim);
}

void L2CA_AcceptConnUpdateRsp(CsrSchedQid phandle,
                              const TYPED_BD_ADDR_T *addrt,
                              CsrUint16 result,
                              l2ca_identifier_t signal_id)
{
    L2CA_CONNECTION_PAR_UPDATE_RSP_T *prim = pnew(L2CA_CONNECTION_PAR_UPDATE_RSP_T);

    prim->type = L2CA_CONNECTION_PAR_UPDATE_RSP;

    prim->phandle = phandle;
    prim->bd_addrt = *addrt;
    prim->result = result;
    prim->signal_id = signal_id;
    L2CA_PutMsg(prim);
}
#endif

