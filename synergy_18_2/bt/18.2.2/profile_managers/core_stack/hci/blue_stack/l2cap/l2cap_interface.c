/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_interface.c

\brief  This file implements the interface to the upper layer.

        It handles receiving downstream primitives from the upper
        layer and routing the primitives to the correct channel
        instance.  It also includes helper functions to send upstream
        primitives.
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
#include "csr_sched.h"
#include "csr_bt_tasks.h"
#include "tbdaddr.h"
#ifdef BUILD_FOR_HOST
#include "csr_env_prim.h"
#else
#include "patch.h"
#endif

#include "l2cap_common.h"
#include "l2cap_signal.h"
#include "l2cap_control.h"
#include "l2cap_chme.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "l2cap_cidme.h"
#include "l2cap_interface.h"
#include "l2cap_cid_fsm_definitions.h"
#include "l2cap_flow.h"
#include "l2cap_auto.h"
#include "l2cap_signal.h"
#include "l2caplib.h"
#include "l2cap_sig_handle.h"

#include "dmlib.h"
#include "dm_mgt_interface.h"

/*! Macro to allocate a L2CAP primitive. */
#define L2CA_MAKE_PRIM(TYPE, p_handle)                                  \
    TYPE##_T *prim = zpnew(TYPE##_T); prim->type = TYPE; prim->phandle = p_handle;

/*! \brief Send L2CA primitives to specified queue.
    \param queue_id ID of queue to put primitive on, if bit 15 set primitive is
                    put on "to host" queue.
    \param msg_id ID of message.
    \param prim Pointer to primitive.
*/
void L2CA_PutMessage(CsrSchedQid queue_id, CsrUint16 msg_id, void *prim)
{
#ifndef BUILD_FOR_HOST
    /* On-chip L2CAP posts upstream messages to hostio */
    if (queue_id & (CsrSchedQid)0x8000)
    {
        CsrSchedMessagePut(L2CAP_TOHOSTQUEUE, IO_DATA_PROTOCOL_L2CAP, prim);
    }
    else
    {
        /* TODO: Sort out flow control */
        CsrSchedMessagePut(queue_id, msg_id, prim);
    }
#else
    CsrSchedMessagePut(queue_id, msg_id, prim);
#endif
}

/*! \brief Initialise L2CAP.

    This function is called to initialise L2CAP.
*/
void L2CAP_Init(void **gash)
{
    PARAM_UNUSED(gash);

    if (!HciClientEnabled())
    {
        return;
    }

    /* Initialise L2CAP control module */
    MCB_Init();
}

/*! \brief Handler function for L2CAP downstream primitives.

    This function is called whenever there are messages on the L2CAP
    Interface Queue.
*/
void L2CAP_InterfaceHandler(void **gash)
{
    CIDCB_T *cidcb;
    CsrUint16 mi;
    L2CA_UPRIM_T *l2cap_prim;
    PARAM_UNUSED(gash);

    /* Get message from interface queue */
    if (!CsrSchedMessageGet(&mi, (void **)&l2cap_prim))
        return;

#ifndef BUILD_FOR_HOST
    patch_fn(l2cap_shared_patchpoint);
#endif

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
    /* Check for DM primitives */
    if(mi == DM_PRIM)
    {
        DM_UPRIM_T *dm_prim = (DM_UPRIM_T*)l2cap_prim;
        switch(dm_prim->type)
        {
            case DM_SM_ACCESS_CFM:
            {
                CID_SmAccessCfm(&(dm_prim->dm_sm_access_cfm));
            }
            break;

            default:
                BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
                break;
        }

        /* Free the event primitive */
        dm_free_upstream_primitive(dm_prim);
    }
    else
#endif

    /* Check this is actually an L2CAP primitive, message of 0
     * indicates from host */
    if ((mi == L2CAP_PRIM) ||
        (mi == 0))
    {
        /* Handle L2CAP downstream primitive */
        switch (l2cap_prim->type)
        {
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
            case L2CA_REGISTER_REQ:
            {
                /* Pass on to L2CAP Control */
                MCB_RegisterProtocol(&l2cap_prim->l2ca_register_req);
            }
            break;

            case L2CA_UNREGISTER_REQ:
            {
                MCB_UnregisterProtocol(&l2cap_prim->l2ca_unregister_req);
            }
            break;

            case L2CA_CONNECT_REQ:
            {
                /* Pass on the request to L2CAP Control. Ignore result
                 * as that's only used for auto-connect */
                MCB_ConnectReq(&l2cap_prim->l2ca_connect_req);
            }
            break;

            case L2CA_CONNECT_RSP:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_connect_rsp.cid);
                if (cidcb != NULL)
                {
                    /* Pass to CID instance */
                    CID_ConnectRsp(cidcb, &l2cap_prim->l2ca_connect_rsp);
                }
            }
            break;


            case L2CA_CONFIG_REQ:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_config_req.cid);
                if (cidcb != NULL)
                {
                    /* Convert flush timeout from ms to slots */
                    l2cap_prim->l2ca_config_req.config.flush_to =
                        SIG_FlushToL2H(l2cap_prim->l2ca_config_req.config.flush_to);

                    /* Pass to CID instance */
                    CID_ConfigReq(cidcb, &l2cap_prim->l2ca_config_req);
                }
            }
            break;

            case L2CA_CONFIG_RSP:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_config_rsp.cid);
                if (cidcb != NULL)
                {
                    /* Convert flush timeout from ms to slots */
                    l2cap_prim->l2ca_config_rsp.config.flush_to =
                        SIG_FlushToL2H(l2cap_prim->l2ca_config_rsp.config.flush_to);

                    /* Pass to CID instance */
                    CID_ConfigRsp(cidcb, &l2cap_prim->l2ca_config_rsp);
                }
            }
            break;

            case L2CA_DISCONNECT_REQ:
            {
                /* Obtain CID instance */
                cidcb = l2cap_prim->l2ca_disconnect_req.cid != L2CA_CID_INVALID
                    ? CIDME_GetCidcb(l2cap_prim->l2ca_disconnect_req.cid)
                    : CIDME_GetCidcbWithContext(
                            l2cap_prim->l2ca_disconnect_req.con_ctx);
                if (cidcb != NULL)
                {
                    /* Pass to CID instance */
                    CID_DisconnectReq(cidcb);
                }
                else
                    L2CA_BadMessage(sizeof(L2CA_DISCONNECT_REQ_T), l2cap_prim);
            }
            break;

            case L2CA_DISCONNECT_RSP:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_disconnect_rsp.cid);
                if (cidcb != NULL)
                {
                    /* Pass to CID instance */
                    CID_DisconnectRsp(cidcb, &l2cap_prim->l2ca_disconnect_rsp);
                }
            }
            break;

            case L2CA_PING_REQ:
            {
                /* Pass on the request to L2CAP Control */
                MCB_PingReq(&l2cap_prim->l2ca_ping_req);
            }
            break;

            case L2CA_GETINFO_REQ:
            {
                /* Pass on the request to L2CAP Control */
                MCB_GetInfoReq(&l2cap_prim->l2ca_getinfo_req);
            }
            break;

            case L2CA_DATAWRITE_ABORT_REQ:
                if ((cidcb = CIDME_GetCidcb(
                            l2cap_prim->l2ca_datawrite_abort_req.cid)) != NULL)
                    CID_AbortTX(cidcb);
                else
                    L2CA_BadMessage(sizeof(L2CA_DATAWRITE_ABORT_REQ_T),
                                    l2cap_prim);

                break;

#ifdef BUILD_FOR_HOST
            case L2CA_GET_CHANNEL_INFO_REQ:
                if ((cidcb = CIDME_GetCidcb(
                            l2cap_prim->l2ca_get_channel_info_req.cid)) != NULL)
                {
                    L2CA_MAKE_PRIM(L2CA_GET_CHANNEL_INFO_CFM, cidcb->p_handle);
                    prim->cid = cidcb->local_cid;
                    prim->remote_cid = cidcb->remote_cid;

                    if (!CH_IS_CONNECTIONLESS(cidcb->chcb))
                        prim->conn_handle = CH_GET_HANDLE(cidcb->chcb);

                    L2CA_PrimSend(prim);
                }
                else
                    L2CA_BadMessage(sizeof(L2CA_GET_CHANNEL_INFO_REQ_T),
                                    l2cap_prim);
                break;
#endif

#endif

#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
            case L2CA_ROUTE_DATA_REQ:
            {
                L2CA_MAKE_PRIM(L2CA_ROUTE_DATA_CFM,
                        l2cap_prim->l2ca_route_data_req.phandle);
                prim->cid = l2cap_prim->l2ca_route_data_req.cid;
                prim->result = L2CA_MISC_FAILED;

                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_route_data_req.cid);
                if (cidcb != NULL && cidcb->state == CID_ST_OPEN)
                {
                    cidcb->p_handle_data
                        = l2cap_prim->l2ca_route_data_req.phandle;

                    prim->con_ctx = cidcb->context;
                    prim->out_mtu = cidcb->remote_mtu;
                    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
                    prim->result = L2CA_MISC_SUCCESS;
                }

                L2CA_PrimSend(prim);
            }
            break;

            case L2CA_UNROUTE_DATA_REQ:
            {
                L2CA_MAKE_PRIM(L2CA_UNROUTE_DATA_CFM,
                        l2cap_prim->l2ca_route_data_req.phandle);
                prim->cid = l2cap_prim->l2ca_route_data_req.cid;
                prim->result = L2CA_MISC_SUCCESS;

                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_unroute_data_req.cid);
                if (cidcb != NULL)
                {
                    /* Restore data P-Handle back to signalling P-Handle */
                    cidcb->p_handle_data = cidcb->p_handle;
                    prim->con_ctx = cidcb->context;
                    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
                }

                L2CA_PrimSend(prim);
            }
            break;
#endif /* ROUTER */

            case L2CA_DATAWRITE_REQ:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_datawrite_req.cid);
                if (cidcb != NULL)
                {
                    /* Pass to CID instance */
                    CID_DataWriteReq(cidcb,
                                     &l2cap_prim->l2ca_datawrite_req.data,
                                     l2cap_prim->l2ca_datawrite_req.req_ctx);
                }
                else
                    L2CA_BadMessage(sizeof(L2CA_DATAWRITE_REQ_T), l2cap_prim);
            }
            break;

#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
            case L2CA_MULTICAST_REQ:
            {
                /* Send data over multiple CIDs */
                CID_DataWriteMulticastReq(l2cap_prim->l2ca_multicast_req.cids,
                                          &l2cap_prim->l2ca_multicast_req.data);
            }
            break;
#endif /* CONNLESS */

            case L2CA_DATAREAD_RSP:
            {
                /* Read ack from upper layer. Only makes sense in the
                   L2CAP 1.2 retransmission/flow control module */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_dataread_rsp.cid);
                if (cidcb != NULL && cidcb->flow != NULL)
                {
                    /* Pass to CID FLOW instance */
                    FLOW_DataReadAck(cidcb, cidcb->flow, 1);
                }
#endif /* ENHANCED */
            }
            break;

            case L2CA_BUSY_REQ:
            {
                /* Toggle local busy state for F&EC modes. */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_busy_req.cid);
                if (cidcb != NULL && cidcb->flow != NULL)
                {
                    /* Pass to CID FLOW instance */
                    FLOW_BusyReq(cidcb, l2cap_prim->l2ca_busy_req.busy);
                }
                else
                    L2CA_BadMessage(sizeof(L2CA_BUSY_REQ_T), l2cap_prim);
#endif /* ENHANCED */
            }
            break;

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
            case L2CA_REGISTER_FIXED_CID_REQ:
            {
                /* Pass on to L2CAP Control */
                MCB_RegisterFixedCid(&l2cap_prim->l2ca_register_fixed_cid_req);
            }
            break;
#endif /* INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT */

            case L2CA_MAP_FIXED_CID_REQ:
            {
                /* Pass on request to L2CAP control */
                MCB_MapFixedCidReq(&l2cap_prim->l2ca_map_fixed_cid_req);
            }
            break;

            case L2CA_MAP_FIXED_CID_RSP:
            {
                /* Pass on to L2CAP control */
                MCB_MapFixedCidRsp(&l2cap_prim->l2ca_map_fixed_cid_rsp);
            }
            break;

            case L2CA_UNMAP_FIXED_CID_REQ:
            {
                /* Pass on to L2CAP control */
                MCB_UnmapFixedCidReq(&l2cap_prim->l2ca_unmap_fixed_cid_req);
            }
            break;
#endif /* FIXED_CHANNEL */

#ifdef INSTALL_AMP_SUPPORT
            case L2CA_CREATE_CHANNEL_REQ:
            {
                /* Pass on the request to L2CAP control. Ignore result
                 * as that's only used for auto-connect */
                MCB_AmpCreateReq(&l2cap_prim->l2ca_create_channel_req);
            }
            break;

            case L2CA_CREATE_CHANNEL_RSP:
            {
                /* Pass into state machine */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_create_channel_rsp.cid);
                if(cidcb != NULL)
                {
                    CID_AmpCreateRsp(cidcb, &l2cap_prim->l2ca_create_channel_rsp);
                }
            }
            break;

            case L2CA_MOVE_CHANNEL_REQ:
            {
                /* Pass into state machine */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_move_channel_req.cid);
                if(cidcb != NULL)
                {
                    CID_AmpMoveReq(cidcb, &l2cap_prim->l2ca_move_channel_req);
                }
                else
                    L2CA_BadMessage(sizeof(L2CA_MOVE_CHANNEL_REQ_T),
                                    l2cap_prim);
            }
            break;

            case L2CA_MOVE_CHANNEL_RSP:
            {
                /* Pass into state machine */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_move_channel_rsp.cid);
                if(cidcb != NULL)
                {
                    CID_AmpMoveRsp(cidcb, &l2cap_prim->l2ca_move_channel_rsp);
                }
            }
            break;
#endif /* AMP_SUPPORT */

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
            case L2CA_AUTO_CONNECT_REQ:
            {
                if (L2CAUTO_HandleL2ca(
                            l2cap_prim->l2ca_auto_connect_req.cid,
                            &l2cap_prim->l2ca_auto_connect_req.bd_addr,
                            l2cap_prim->l2ca_auto_connect_req.psm_local,
                            NULL,
                            (L2CA_UPRIM_T*)l2cap_prim) == L2RS_UNHANDLED)
                {
                    L2CA_BadMessage(sizeof(L2CA_AUTO_CONNECT_REQ_T),
                                    l2cap_prim);
                }
            }
            break;

            case L2CA_AUTO_CONNECT_RSP:
            {
                if (L2CAUTO_HandleL2ca(
                            l2cap_prim->l2ca_auto_connect_rsp.cid,
                            NULL, /* no bd_addr */
                            L2CA_PSM_INVALID, /* no psm */
                            NULL,
                            (L2CA_UPRIM_T*)l2cap_prim) == L2RS_UNHANDLED)
                {
                    L2CA_BadMessage(sizeof(L2CA_AUTO_CONNECT_RSP_T),
                                    l2cap_prim);
                }
            }
            break;
#endif
#ifdef INSTALL_L2CAP_RAW_SUPPORT
            case L2CA_RAW_MODE_REQ:
            {
                MCB_SetRawMode(&l2cap_prim->l2ca_raw_mode_req);
            }
            break;

            case L2CA_RAW_DATA_REQ:
            {
                CH_DataTxRaw(&l2cap_prim->l2ca_raw_data_req);
            }
            break;
#endif

#if defined(INSTALL_L2CAP_ENHANCED_SUPPORT) && defined(INSTALL_L2CAP_DEBUG)
            case L2CA_DEBUG_DROP_REQ:
            {
                /* Obtain CID instance */
                cidcb = CIDME_GetCidcb(l2cap_prim->l2ca_debug_drop_req.cid);
                if (cidcb != NULL)
                {
                    FLOW_DebugDrop(cidcb,
                                   l2cap_prim->l2ca_debug_drop_req.rx_interval,
                                   l2cap_prim->l2ca_debug_drop_req.rx_number,
                                   l2cap_prim->l2ca_debug_drop_req.tx_interval,
                                   l2cap_prim->l2ca_debug_drop_req.tx_number);
                }
            }
            break;
#endif /* ENHANCED && DEBUG */

#ifdef INSTALL_ULP
            case L2CA_CONNECTION_PAR_UPDATE_REQ:
            {
                L2CAP_CHCB_T *chcb;

                if ((chcb = CHME_GetConHandleBdAddr(&l2cap_prim->
                            l2ca_connection_par_update_req.addrt)) != NULL)
                {
                    SIG_SIGNAL_T *sig_ptr;

                    sig_ptr = SIG_SignalConnParamUpdateReq(
                        l2cap_prim->l2ca_connection_par_update_req.min_interval,
                        l2cap_prim->l2ca_connection_par_update_req.max_interval,
                        l2cap_prim->l2ca_connection_par_update_req.latency,
                        l2cap_prim->l2ca_connection_par_update_req.timeout);

                    sig_ptr->p_handle =
                        l2cap_prim->l2ca_connection_par_update_req.phandle;
                    SIGH_SignalSend(chcb, sig_ptr);
                }
                else
                    L2CA_ConnUpdateCfm(
                            l2cap_prim->l2ca_connection_par_update_req.phandle,
                            L2CA_MISC_NO_CONNECTION,
                            &l2cap_prim->l2ca_connection_par_update_req.addrt);
            }
            break;

            case L2CA_CONNECTION_PAR_UPDATE_RSP:
            {
                SIG_SIGNAL_T *signal;
                DM_ACL_T* p_acl;
                L2CAP_CHCB_T *chcb;
                p_acl = dm_acl_find_by_tbdaddr(&l2cap_prim->l2ca_connection_par_update_rsp.bd_addrt);
                if(NULL!=p_acl)
                {
                    chcb = CH_GET_CHCB(p_acl);
                    signal = SIG_SignalConnParamUpdateRsp(l2cap_prim->l2ca_connection_par_update_rsp.signal_id,
                                                         l2cap_prim->l2ca_connection_par_update_rsp.result);
                    SIGH_SignalSend(chcb, signal);
                }
            }
            break;

#endif
            default:
            {
                /* Received unknown primitive */
                BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
            }
            break;
        }

        /* Free the primitive and any other memory associated with primitive */
        L2CA_FreePrimitive(l2cap_prim);
    }
#ifdef BUILD_FOR_HOST
    else if (mi == CSR_SCHED_PRIM)
    {
        /* Free scheduler broadcast primitive */
        CsrPmemFree(l2cap_prim);
    }
#endif
    else
    {
        /* Received unknown primitive */
        BLUESTACK_PANIC(CSR_BT_PANIC_INVALID_BLUESTACK_PRIMITIVE);
    }
}


/*! \brief Wrapper function for sending L2CA_UNROUTE_DATA_IND primitives to
           higher layer.
*/
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
void L2CA_UnrouteDataInd(CIDCB_T *const cidcb)
{
    L2CA_MAKE_PRIM(L2CA_UNROUTE_DATA_IND, cidcb->p_handle_data);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_REGISTER_CFM primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_RegisterCfm(CsrSchedQid p_handle, psm_t local_psm, CsrUint16 mode_mask,
       l2ca_misc_result_t result, CsrUint16 reg_ctx, CsrUint16 reg_flags)
{
    L2CA_MAKE_PRIM(L2CA_REGISTER_CFM, p_handle);
    prim->psm_local = local_psm;
    prim->mode_mask = mode_mask;
    prim->result = result;
    prim->reg_ctx = reg_ctx;
    prim->flags = reg_flags;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_UNREGISTER_CFM primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_UnregisterCfm(CsrSchedQid p_handle, psm_t local_psm, l2ca_misc_result_t result, CsrUint16 reg_ctx)
{
    L2CA_MAKE_PRIM(L2CA_UNREGISTER_CFM, p_handle);
    prim->psm_local = local_psm;
    prim->reg_ctx = reg_ctx;
    prim->result = result;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_CONNECT_IND primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConnectInd(CIDCB_T *cidcb, CsrUint8 signal_id)
{
    if(cidcb != NULL)
    {
        L2CA_MAKE_PRIM(L2CA_CONNECT_IND, cidcb->p_handle);
        prim->cid = cidcb->local_cid;
        prim->psm_local = CIDME_LocalPsm(cidcb);
        prim->reg_ctx = CIDME_RegistrationContext(cidcb);
        prim->identifier = signal_id;
        prim->flags = 0; /*FIXME le support */

        bd_addr_copy(&prim->bd_addr, CH_GET_BD_ADDR(cidcb->chcb));

        /* Divert this into the auto-connect system */
        if(L2CAUTO_HandleL2ca(cidcb->local_cid, &prim->bd_addr, prim->psm_local,
                              NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
        {
            L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
            return;
        }
        L2CA_PrimSend(prim);
    }
}
#endif

/*! \brief Wrapper function for sending L2CA_CONNECT_CFM primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConnectCfm(const BD_ADDR_T *bd_addr,
                     l2ca_cid_t cid,
                     CsrSchedQid p_handle,
                     psm_t local_psm,
                     l2ca_conn_result_t response,
                     CsrUint16 con_ctx,
                     CsrUint16 reg_ctx)
{
    L2CA_MAKE_PRIM(L2CA_CONNECT_CFM, p_handle);
    prim->cid = cid;
    prim->con_ctx = con_ctx;
    prim->reg_ctx = reg_ctx;
    prim->psm_local = local_psm;
    prim->result = response;
    bd_addr_copy(&prim->bd_addr, bd_addr);

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(cid, bd_addr, local_psm,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_CONFIG_IND primitives to
           higher layer.

    Function will take ownership of the 'config' pointer member in the
    signal_args primitive.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConfigInd(CIDCB_T *cidcb,
                    l2ca_identifier_t id,
                    L2CA_CONFIG_T *conf)
{
    L2CA_MAKE_PRIM(L2CA_CONFIG_IND, cidcb->p_handle);

    /* Trivial settings */
    prim->cid = cidcb->local_cid;
    prim->identifier = id;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->config.options = conf->options;
    prim->config.hints = conf->hints;
    prim->config.unknown_length = conf->unknown_length;
    prim->config.unknown = conf->unknown;
    prim->config.qos = conf->qos;
    prim->config.flow = conf->flow;
    prim->config.fcs = conf->fcs;
    prim->config.flowspec = conf->flowspec;
    prim->config.ext_window = conf->ext_window;

    /* Fill out default MTU from CIDCB if not specified */
    prim->config.mtu = ((conf->options & L2CA_SPECIFY_MTU)
                        ? conf->mtu
                        : cidcb->remote_mtu);

    /* We don't store the remote flush, so quess the value encase it's
     * not there */
    prim->config.flush_to = (conf->options & L2CA_SPECIFY_FLUSH
                             ? SIG_FlushToH2L(conf->flush_to)
                             : L2CA_FLUSH_TO_DEFAULT);

    /* Default FCS is "on", so if unspecified, set this. Unless mode
     * is basic, it's always off */
    if(!(conf->options & L2CA_SPECIFY_FCS))
    {
        prim->config.fcs = L2CA_FCS_ON;
    }
    if((conf->flow == NULL) || (conf->flow->mode == L2CA_FLOW_MODE_BASIC))
    {
        prim->config.fcs = L2CA_FCS_OFF;
    }

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(cidcb->local_cid, NULL, L2CA_PSM_INVALID,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_CONFIG_CFM primitives to
           higher layer.

    Function will take ownership of the 'config' pointer member in the
    signal_args primitive.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_ConfigCfm(CIDCB_T *cidcb,
                    l2ca_conf_result_t response,
                    L2CA_CONFIG_T *conf)
{
    L2CA_MAKE_PRIM(L2CA_CONFIG_CFM, cidcb->p_handle);

    /* Trivial settings */
    prim->cid = cidcb->local_cid;
    prim->result = response;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->config.options = conf->options;
    prim->config.hints = conf->hints;
    prim->config.unknown_length = conf->unknown_length;
    prim->config.unknown = conf->unknown;
    prim->config.qos = conf->qos;
    prim->config.flow = conf->flow;
    prim->config.fcs = conf->fcs;
    prim->config.flowspec = conf->flowspec;
    prim->config.ext_window = conf->ext_window;

    /* Always use local MTU unless we're passing up an error because
     * of IOP problems discovered by the late CPH:
     *
     * It is unclear in the spec as to what the remote device is
     * saying when it sends a lower MTU.  We have seen devices that
     * send a smaller value but then send packets up to the original
     * MTU.  They claim they are qualified and compliant.
     *
     * To guarantee interop, we ignore the remote device's lowering of
     * the MTU and accept up to the original MTU size.  The case of
     * the local device lowering the MTU is handled in the store
     * function and so we still limit the outgoing PDU.
     */
    prim->config.mtu = ((response == L2CA_CONFIG_SUCCESS)
                        ? cidcb->local_mtu
                        : conf->mtu);

    /* Fill out flush with default CIDCB paramter */
    prim->config.flush_to = ((conf->options & L2CA_SPECIFY_FLUSH)
                             ? SIG_FlushToH2L(conf->flush_to)
                             : SIG_FlushToH2L(cidcb->local_flush_to));

    /* Make sure ext_window is zero if unused */
    if(!(conf->options & L2CA_SPECIFY_EXT_WINDOW))
    {
        prim->config.ext_window = 0;
    }

    /* Default FCS is "on", so if unspecified, set this. Unless mode
     * is basic, it's always off */
    if(!(conf->options & L2CA_SPECIFY_FCS))
    {
        prim->config.fcs = L2CA_FCS_ON;
    }
    if((conf->flow == NULL) || (conf->flow->mode == L2CA_FLOW_MODE_BASIC))
    {
        prim->config.fcs = L2CA_FCS_OFF;
    }

    /* Remove basic flush timeout and old QoS if flowspecs are being
     * used */
    if(conf->options & L2CA_SPECIFY_FLOWSPEC)
    {
        prim->config.options &= ~(L2CA_SPECIFY_FLUSH | L2CA_SPECIFY_QOS);
        prim->config.flush_to = L2CA_FLUSH_TO_IN_FLOWSPEC;
        CsrPmemFree(prim->config.qos);
        prim->config.qos = NULL;
    }

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(cidcb->local_cid, NULL, L2CA_PSM_INVALID,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_DISCONNECT_CFM primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_DisconnectCfm(CIDCB_T *cidcb, l2ca_disc_result_t result)
{
    L2CA_MAKE_PRIM(L2CA_DISCONNECT_CFM, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->result = result;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(cidcb->local_cid, NULL, L2CA_PSM_INVALID,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_DISCONNECT_IND primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_DisconnectInd(CIDCB_T *cidcb, CsrUint8 signal_id, l2ca_disc_result_t reason)
{
    L2CA_MAKE_PRIM(L2CA_DISCONNECT_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->identifier = signal_id;
    prim->reason = reason;

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(cidcb->local_cid, NULL, L2CA_PSM_INVALID,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_TIMEOUT_IND primitives to
           higher layer.
*/
void L2CA_TimeoutInd(const SIG_SIGNAL_T *sig_ptr,
                     const CIDCB_T *cidcb,
                     l2ca_identifier_t signal_id)
{
    L2CA_MAKE_PRIM(L2CA_TIMEOUT_IND, sig_ptr->p_handle);
    prim->identifier = signal_id;

    if (cidcb != NULL)
    {
        prim->phandle = cidcb->p_handle;
        prim->cid = cidcb->local_cid;
        prim->reg_ctx = CIDME_RegistrationContext(cidcb);
        prim->con_ctx = cidcb->context;
    }

    L2CA_PrimSend(prim);
}

/*! \brief Wrapper function for sending L2CA_MTU_VIOLATION_IND primitives to
           higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_MtuViolationInd(const CIDCB_T *cidcb,
                          l2ca_identifier_t identifier,
                          l2ca_mtu_t mtu)
{
    L2CA_MAKE_PRIM(L2CA_MTU_VIOLATION_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->identifier = identifier;
    prim->signal_mtu = mtu;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_UNKNOWN_SIGNAL_IND primitives to
           higher layer.
*/
void L2CA_UnknownSignalInd(const CIDCB_T *cidcb,
                           l2ca_identifier_t identifier)
{
    L2CA_MAKE_PRIM(L2CA_UNKNOWN_SIGNAL_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->identifier = identifier;
    L2CA_PrimSend(prim);
}

/*! \brief Set elements of given L2CA_DATAWRITE_CFM.

    \param cidcb Pointer to channel control block.
    \param context Opaque context from L2CA_DATAWRITE_REQ.
    \param length Length of data that has been sent.
    \param result Indication of success or failure.
    \param prim Pointer to allocation for primitive.
*/
void L2CA_BuildDataWriteCfm(CIDCB_T *cidcb,
                            CsrUint16 context,
                            CsrUint16 length,
                            l2ca_data_result_t result,
                            L2CA_DATAWRITE_CFM_T *prim)
{
    prim->type = L2CA_DATAWRITE_CFM;
    prim->phandle = CID_DataPhandle(cidcb);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->req_ctx = context;
    prim->con_ctx = cidcb->context;
    prim->length = length;
    prim->result = result;
}

/*! \brief Wrapper function for sending L2CA_DATAWRITE_CFM primitives to
           higher layer.
*/
void L2CA_DataWriteCfm(CIDCB_T *cidcb, CsrUint16 context, CsrUint16 length, l2ca_data_result_t result)
{
    if(cidcb != NULL)
    {
        L2CA_DATAWRITE_CFM_T *prim = pnew(L2CA_DATAWRITE_CFM_T);
        L2CA_BuildDataWriteCfm(cidcb, context, length, result, prim);
        L2CA_PrimSend(prim);
    }
}

/*! \brief Wrapper function for sending L2CA_DATAREAD_IND primitives to
     higher layer.
*/
void L2CA_DataReadInd(const CIDCB_T *const cidcb,
                      CsrMblk *mblk_ptr,
                      l2ca_data_result_t result,
                      CsrUint16 packets,
                      L2CA_DATAREAD_IND_T **return_prim)
{
    L2CA_DATAREAD_IND_T *prim = pnew(L2CA_DATAREAD_IND_T);
    prim->type = L2CA_DATAREAD_IND;
    prim->phandle = CID_DataPhandle(cidcb),
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->data = mblk_ptr;
    prim->result = result;
    prim->length = 0;
    prim->packets = packets;

    if (return_prim == NULL)
        L2CA_PrimSend(prim);
    else
        *return_prim = prim;
}

/*! \brief Issue a busy indication to upper layer
*/
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
void L2CA_BusyInd(const CIDCB_T *cidcb, CsrBool busy)
{
    L2CA_MAKE_PRIM(L2CA_BUSY_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->busy = busy;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_RAW_DATA_IND primitives to
     higher layer.
*/
#ifdef INSTALL_L2CAP_RAW_SUPPORT
void L2CA_RawDataInd(CsrSchedQid p_handle,
                     TYPED_BD_ADDR_T *addrt,
                     l2ca_data_result_t result,
                     l2ca_raw_packet_t packet_type,
                     l2ca_cid_t cid,
                     CsrMblk *mblk_ptr)
{
    L2CA_RAW_DATA_IND_T *prim;

    prim = zpnew(L2CA_RAW_DATA_IND_T);
    prim->type = L2CA_RAW_DATA_IND;
    prim->phandle = p_handle;
    tbdaddr_copy(&prim->addrt, addrt);
    prim->cid = cid;
    prim->result = result;
    prim->packet_type = packet_type;
    prim->data = mblk_ptr;

    L2CA_PutMessage(p_handle, L2CAP_PRIM, prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_PING_CFM primitives to
     higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_PingCfm(CsrSchedQid p_handle, const BD_ADDR_T *const p_bd_addr,
                  l2ca_misc_result_t result, CsrUint8 *data, CsrUint16 length,
                  CsrUint16 req_ctx)
{
    L2CA_MAKE_PRIM(L2CA_PING_CFM, p_handle);
    prim->result = result;
    prim->length = length;
    prim->data = data;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    prim->req_ctx = req_ctx;

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_GETINFO_CFM primitives to
     higher layer.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_GetInfoCfm(CsrSchedQid p_handle, const BD_ADDR_T *const p_bd_addr,
                     l2ca_info_result_t result, CsrUint16 info_type,
                     CsrUint8 *data, CsrUint16 length,
                     CsrUint16 req_ctx)
{
    L2CA_MAKE_PRIM(L2CA_GETINFO_CFM, p_handle);
    prim->result = result;
    prim->length = length;
    prim->info_type = info_type;
    prim->info_data = data;
    bd_addr_copy(&prim->bd_addr, p_bd_addr);
    prim->req_ctx = req_ctx;

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(prim->req_ctx, NULL, L2CA_PSM_INVALID,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MAP_FIXED_IND primitives to
           higher layer.
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void L2CA_MapFixedCidInd(const CIDCB_T *const cidcb)
{
    L2CA_MAKE_PRIM(L2CA_MAP_FIXED_CID_IND, cidcb->p_handle);
    tbdaddr_copy(&prim->addrt, CH_GET_TBDADDR(cidcb->chcb));
    prim->fixed_cid = cidcb->remote_cid;
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->cl_local_psm = CIDME_LocalPsm(cidcb);
    prim->flags = CH_ULP_FLAG(cidcb->chcb);

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MAP_FIXED_IND primitives to
           higher layer.
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void L2CA_UnmapFixedCidInd(const CIDCB_T *cidcb, l2ca_disc_result_t reason)
{
    L2CA_MAKE_PRIM(L2CA_UNMAP_FIXED_CID_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->reason = reason;

    L2CA_PrimSend(prim);
}
#endif


/*! \brief Wrapper function for sending L2CA_CREATE_CHANNEL_CFM primitives to
           higher layer.
 */
#ifdef INSTALL_AMP_SUPPORT
void L2CA_CreateChannelCfm(const CIDCB_T *cidcb,
                           const L2CA_CREATE_CHANNEL_REQ_T *req,
                           CsrSchedQid p_handle,
                           l2ca_conn_result_t response)
{
    PSM_T *psm_reg;
    const BD_ADDR_T *bd_addr;
    L2CA_MAKE_PRIM(L2CA_CREATE_CHANNEL_CFM, p_handle);

    if (cidcb != NULL)
    {
        bd_addr = CH_GET_BD_ADDR(cidcb->chcb);
        prim->con_ctx = cidcb->context;
        prim->local_control = cidcb->amp_info->local_amp;
        psm_reg = cidcb->local_psm;
        prim->cid = cidcb->local_cid;
    }
    else
    {
        bd_addr = &req->bd_addr;
        prim->con_ctx = req->con_ctx;
        prim->local_control = req->local_control;
        psm_reg = MCB_FindProtocol(req->psm_local);
        prim->cid = L2CA_CID_INVALID;
    }

    if (psm_reg != NULL)
    {
        prim->psm_local = psm_reg->psm;
        prim->reg_ctx = psm_reg->reg_ctx;
    }

    bd_addr_copy(&prim->bd_addr, bd_addr);
    prim->result = response;

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(prim->cid, bd_addr, prim->psm_local,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_CREATE_CHANNEL_CFM primitives to
           higher layer.
 */
#ifdef INSTALL_AMP_SUPPORT
void L2CA_CreateChannelInd(const CIDCB_T *cidcb)
{
    L2CA_MAKE_PRIM(L2CA_CREATE_CHANNEL_IND, cidcb->p_handle);

    prim->identifier = cidcb->rcnreq_signal_id;
    bd_addr_copy(&prim->bd_addr, CH_GET_BD_ADDR(cidcb->chcb));
    prim->cid = cidcb->local_cid,
    prim->psm_local = CIDME_LocalPsm(cidcb);
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->local_control = cidcb->amp_info->local_amp;

    /* Divert this into the auto-connect system */
    if(L2CAUTO_HandleL2ca(prim->cid, &prim->bd_addr, prim->psm_local,
                          NULL, (L2CA_UPRIM_T*)prim) != L2RS_UNHANDLED)
    {
        L2CA_FreePrimitive((L2CA_UPRIM_T*)prim);
        return;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MOVE_CHANNEL_CFM primitives to
           higher layer.
 */
#ifdef INSTALL_AMP_SUPPORT
void L2CA_MoveChannelCfm(const CIDCB_T *cidcb,
                         l2ca_move_result_t response,
                         CsrBool ignore_controllers)
{
    L2CA_MAKE_PRIM(L2CA_MOVE_CHANNEL_CFM, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->result = response;
    prim->con_ctx = cidcb->context;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);

    if (ignore_controllers || cidcb->amp_info == NULL)
    {
        prim->remote_control = L2CA_AMP_CONTROLLER_UNKNOWN;
        prim->local_control = L2CA_AMP_CONTROLLER_UNKNOWN;
    }
    else
    {
        prim->remote_control = cidcb->amp_info->local_amp;
        prim->local_control = cidcb->amp_info->remote_amp;
    }

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MOVE_CHANNEL_CMP_IND
           primitives to higher layer.
 */
#ifdef INSTALL_AMP_SUPPORT
void L2CA_MoveChannelCmpInd(const CIDCB_T *cidcb)
{
    L2CA_MAKE_PRIM(L2CA_MOVE_CHANNEL_CMP_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->local_control = cidcb->amp_info->local_amp;
    prim->result = cidcb->amp_info->move_result;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MOVE_CHANNEL_IND primitives to
           higher layer.
 */
#ifdef INSTALL_AMP_SUPPORT
void L2CA_MoveChannelInd(const CIDCB_T *cidcb)
{
    L2CA_MAKE_PRIM(L2CA_MOVE_CHANNEL_IND, cidcb->p_handle);

    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->identifier = cidcb->rcnreq_signal_id;
    prim->local_control = cidcb->amp_info->local_amp;

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_AMP_LINK_LOSS_IND
           primitives to higher layer.
*/
#ifdef INSTALL_AMP_SUPPORT
void L2CA_AmpLinkLossInd(const CIDCB_T *cidcb, hci_error_t reason)
{
    L2CA_MAKE_PRIM(L2CA_AMP_LINK_LOSS_IND, cidcb->p_handle);
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;
    prim->reason = reason;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_REGISTER_CID_CFM primitives to
           higher layer.
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
void L2CA_RegisterFixedCidCfm(CsrSchedQid p_handle, l2ca_cid_t cid, l2ca_misc_result_t result)
{
    L2CA_MAKE_PRIM(L2CA_REGISTER_FIXED_CID_CFM, p_handle);
    prim->fixed_cid = cid;
    prim->reg_ctx = 0;
    prim->result = result;
    L2CA_PrimSend(prim);
}
#endif

/*! \brief Wrapper function for sending L2CA_MAP_FIXED_CFM primitives to
           higher layer.
 */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
void L2CA_MapFixedCidCfm(const CIDCB_T *cidcb,
                         const L2CA_MAP_FIXED_CID_REQ_T *req,
                         CsrSchedQid phandle,
                         l2ca_misc_result_t response)
{
    PSM_T *psm_reg;
    L2CA_MAKE_PRIM(L2CA_MAP_FIXED_CID_CFM, phandle);

    if (cidcb != NULL)
    {
        if (CH_IS_CONNECTIONLESS(cidcb->chcb))
        {
            prim->flags |= L2CA_CONFLAG_ENUM(L2CA_CONNECTION_BR_EDR_BROADCAST);
        }
        else
        {
            prim->flags = CH_ULP_FLAG(cidcb->chcb);
            tbdaddr_copy(&prim->addrt, CH_GET_TBDADDR(cidcb->chcb));
        }

        psm_reg = cidcb->local_psm;
        prim->fixed_cid = cidcb->remote_cid;
        prim->cid = cidcb->local_cid;
        prim->con_ctx = cidcb->context;
    }
    else
    {
        prim->flags = req->flags & ~L2CA_CONFLAG_LOCK_ACL;
        tbdaddr_copy(&prim->addrt, &req->addrt);
        psm_reg = MCB_FindProtocol(req->cl_local_psm);
        prim->fixed_cid = req->fixed_cid;
        prim->cid = L2CA_CID_INVALID;
        prim->con_ctx = req->con_ctx;
    }

    if (psm_reg != NULL && prim->fixed_cid == L2CA_CID_CONNECTIONLESS)
    {
        prim->cl_local_psm = psm_reg->psm;
        prim->reg_ctx = psm_reg->reg_ctx;
    }

    prim->result = response;

    L2CA_PrimSend(prim);
}
#endif

/*! \brief Send (L2CA_DATAWRITE_ABORT_CFM to application.

    \param cidcb Pointer to channel control block.
*/
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
void L2CA_DataWriteAbortCfm(CIDCB_T *cidcb,
                            L2CA_DATAWRITE_ABORT_CFM_T *primret)
{
    L2CA_DATAWRITE_ABORT_CFM_T *prim = primret == NULL ?
        pnew(L2CA_DATAWRITE_ABORT_CFM_T) : primret;

    prim->type = L2CA_DATAWRITE_ABORT_CFM;
    prim->phandle = cidcb->p_handle;
    prim->cid = cidcb->local_cid;
    prim->reg_ctx = CIDME_RegistrationContext(cidcb);
    prim->con_ctx = cidcb->context;

    if (primret == NULL)
        L2CA_PrimSend(prim);
}
#endif

/*! \brief Get the DM to send DM_BAD_MESSAGE_IND
    We take a copy of the message, so caller must free the original.

    \param length sizeof(message)
    \param message Pointer to request primitive.
*/
void L2CA_BadMessage(CsrUint16 length, void *message)
{
    DM_SendBadMessageInd(DM_BAD_MESSAGE_NO_PHANDLE,
                         L2CAP_PRIM,
                         length,
                         pcopy(message, length));
}

#ifdef INSTALL_ULP
void L2CA_ConnUpdateCfm(CsrSchedQid phandle, l2ca_misc_result_t result, TYPED_BD_ADDR_T *addrt)
{
    L2CA_MAKE_PRIM(L2CA_CONNECTION_PAR_UPDATE_CFM, phandle);
    prim->addrt  = *addrt;
    prim->result = result;
    L2CA_PrimSend(prim);
}

/*! \brief Ask DM to accept or reject connection parameter update.
*/
void L2CA_ConnUpdateInd(CsrUint8 sig_id,
                           TYPED_BD_ADDR_T remote_bd_addrt,
                           CsrUint16  conn_interval_min,
                           CsrUint16  conn_interval_max,
                           CsrUint16  conn_latency,
                           CsrUint16  supervision_timeout)
{
    L2CA_MAKE_PRIM(L2CA_CONNECTION_PAR_UPDATE_IND, DM_IFACEQUEUE);

    prim->signal_id = sig_id;
    prim->bd_addrt = remote_bd_addrt;
    prim->conn_interval_max = conn_interval_max;
    prim->conn_interval_min = conn_interval_min;
    prim->conn_latency = conn_latency;
    prim->supervision_timeout = supervision_timeout;

    L2CA_PrimSend(prim);
}
#endif
