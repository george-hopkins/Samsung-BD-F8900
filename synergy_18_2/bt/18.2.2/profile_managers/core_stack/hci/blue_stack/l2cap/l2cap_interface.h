/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_interface.h

\brief  This file defines the interface to the upper layer.
*/

#ifndef _L2CAP_INTERFACE_H
#define _L2CAP_INTERFACE_H

#include "csr_synergy.h"

#include "l2cap_config.h"
#include "l2cap_cid.h"
#include "l2cap_signal.h"
#include "dm_acl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! Macro to simplifying send L2CAP primitives. */
#define L2CA_PrimSend(prim)                             \
    L2CA_PutMessage((prim)->phandle, L2CAP_PRIM, prim)

/* Scheduler functions */
extern void L2CAP_Init(void **gash);
extern void L2CAP_DeInit(void **gash);
extern void L2CAP_InterfaceHandler(void **gash);

/* Generic sender (also used by auto-connect) */
extern void L2CA_PutMessage(CsrSchedQid queue_id, CsrUint16 msg_id, void *prim);

/* Upstream functions */
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
extern void L2CA_RegisterCfm(CsrSchedQid p_handle, psm_t psm, CsrUint16 mode_mask, l2ca_misc_result_t result, CsrUint16 reg_ctx, CsrUint16 reg_flags);
extern void L2CA_UnregisterCfm(CsrSchedQid p_handle, psm_t psm, l2ca_misc_result_t result, CsrUint16 reg_ctx);
extern void L2CA_ConnectInd(CIDCB_T *cidcb, CsrUint8 signal_id);
extern void L2CA_ConnectCfm(const BD_ADDR_T *bd_addr,
                            l2ca_cid_t cid,
                            CsrSchedQid p_handle,
                            psm_t local_psm,
                            l2ca_conn_result_t response,
                            CsrUint16 con_ctx,
                            CsrUint16 reg_ctx);
extern void L2CA_ConfigInd(CIDCB_T *cidcb, l2ca_identifier_t id, L2CA_CONFIG_T *conf);
extern void L2CA_ConfigCfm(CIDCB_T *cidcb, l2ca_conf_result_t response, L2CA_CONFIG_T *conf);
extern void L2CA_BusyInd(const CIDCB_T *cidcb, CsrBool busy);
extern void L2CA_DisconnectCfm(CIDCB_T *cidcb, l2ca_disc_result_t result);
extern void L2CA_DisconnectInd(CIDCB_T *cidcb, CsrUint8 signal_id, l2ca_disc_result_t reason);
extern void L2CA_PingCfm(CsrSchedQid p_handle, const BD_ADDR_T *const p_bd_addr, l2ca_misc_result_t result,
                         CsrUint8 *data, CsrUint16 length, CsrUint16 req_ctx);
extern void L2CA_DataWriteAbortCfm(CIDCB_T *cidcb, L2CA_DATAWRITE_ABORT_CFM_T *primret);
extern void L2CA_GetInfoCfm(CsrSchedQid p_handle, const BD_ADDR_T *const p_bd_addr, l2ca_info_result_t result, CsrUint16 info_type,
                            CsrUint8 *data, CsrUint16 length, CsrUint16 req_ctx);
#endif

extern void L2CA_DataReadInd(const CIDCB_T *const cidcb, CsrMblk *mblk_ptr, l2ca_data_result_t result, CsrUint16 packets, L2CA_DATAREAD_IND_T **return_prim);
extern void L2CA_DataWriteCfm(CIDCB_T *cidcb, CsrUint16 context, CsrUint16 length, l2ca_data_result_t result);
extern void L2CA_TimeoutInd(const SIG_SIGNAL_T *sig_ptr,
                            const CIDCB_T *cidcb,
                            l2ca_identifier_t signal_id);
extern void L2CA_MtuViolationInd(const CIDCB_T *cidcb,
                                 l2ca_identifier_t identifier,
                                 l2ca_mtu_t mtu);
extern void L2CA_UnknownSignalInd(const CIDCB_T *cidcb,
                                  l2ca_identifier_t identifier);
extern void L2CA_ConnectionlessModifyCfm(CsrSchedQid p_handle, CsrUint16 operation, l2ca_info_result_t response);
extern void L2CA_CreateGroupCfm(CsrSchedQid p_handle, l2ca_misc_result_t response,
                                l2ca_cid_t cid, psm_t psm);
extern void L2CA_CloseGroupCfm(CsrSchedQid p_handle, l2ca_misc_result_t response,
                               l2ca_cid_t cid, psm_t psm);
extern void L2CA_UnrouteDataInd(CIDCB_T *const cidcb);
extern void L2CA_RawDataInd(CsrSchedQid p_handle, TYPED_BD_ADDR_T *addrt, l2ca_data_result_t result, l2ca_raw_packet_t packet_type , l2ca_cid_t cid, CsrMblk *mblk_ptr);
extern void L2CA_BuildDataWriteCfm(CIDCB_T *cidcb, CsrUint16 context, CsrUint16 length, l2ca_data_result_t result, L2CA_DATAWRITE_CFM_T *prim);
extern void L2CA_BadMessage(CsrUint16 length, void *message);

/* Fixed channels */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
extern void L2CA_RegisterFixedCidCfm(CsrSchedQid p_handle, l2ca_cid_t cid, l2ca_misc_result_t result);
#endif
extern void L2CA_MapFixedCidCfm(const CIDCB_T *cidcb,
                                const L2CA_MAP_FIXED_CID_REQ_T *req,
                                CsrSchedQid phandle,
                                l2ca_misc_result_t response);
void L2CA_MapFixedCidInd(const CIDCB_T *const cidcb);
extern void L2CA_UnmapFixedCidInd(const CIDCB_T *cidcb, l2ca_disc_result_t reason);
#endif

/* AMP extensions */
#ifdef INSTALL_AMP_SUPPORT
extern void L2CA_CreateChannelCfm(const CIDCB_T *cidcb,
                                  const L2CA_CREATE_CHANNEL_REQ_T *req,
                                  CsrSchedQid p_handle,
                                  l2ca_conn_result_t response);
extern void L2CA_CreateChannelInd(const CIDCB_T *cidcb);
extern void L2CA_MoveChannelInd(const CIDCB_T *cidcb);
extern void L2CA_MoveChannelCfm(const CIDCB_T *cidcb,
                                l2ca_move_result_t response,
                                CsrBool ignore_controllers);
extern void L2CA_MoveChannelCmpInd(const CIDCB_T *cidcb);
extern void L2CA_AmpLinkLossInd(const CIDCB_T *cidcb, hci_error_t reason);
#endif

/* BLE */
#ifdef INSTALL_ULP
void L2CA_ConnUpdateCfm(CsrSchedQid phandle, l2ca_misc_result_t result, TYPED_BD_ADDR_T *addrt);
void L2CA_ConnUpdateInd(CsrUint8 sig_id,
                           TYPED_BD_ADDR_T remote_bd_addrt,
                           CsrUint16  conn_interval_min,
                           CsrUint16  conn_interval_max,
                           CsrUint16  conn_latency,
                           CsrUint16  supervision_timeout);
#endif

#ifdef __cplusplus
}
#endif
#endif
