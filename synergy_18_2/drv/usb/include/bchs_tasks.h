#ifndef BCHS_TASKS_H__
#define BCHS_TASKS_H__
/*******************************************************************************

                (c) CSR plc 2008
				coal_sched.h

                All rights reserved

FILE:           bchs_tasks.h

DESCRIPTION:    BCHS task/queue definitions

REVISION:       $Revision: #3 $

*******************************************************************************/

#include "sched/coal_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Make sure that the correct defines are set for BlueStack */
#if !defined(INSTALL_L2CAP_MODULE)
#define INSTALL_L2CAP_MODULE
#endif

#if !defined(INSTALL_SDP_MODULE)
#define INSTALL_SDP_MODULE
#endif

#if !defined(EXCLUDE_BCHS_RFC_MODULE)
#if !defined(INSTALL_RFCOMM_MODULE)
#define INSTALL_RFCOMM_MODULE
#endif
#else
#undef INSTALL_RFCOMM_MODULE
#endif

#if !defined(EXCLUDE_SCO_MODULE)
#if !defined(INSTALL_DM_SYNC_MODULE)
#define INSTALL_DM_SYNC_MODULE
#endif
#else
#undef INSTALL_DM_SYNC_MODULE
#undef MAX_NUM_OF_SIMULTANIOUS_SCO_CONNECTIONS
#ifndef EXCLUDE_SCO_MODULE
#define EXCLUDE_SCO_MODULE
#endif
#endif

/* PAN can only run on HCI firmware */
#if !defined(EXCLUDE_BNEP_MODULE) && !defined(RFC_BUILD)
#if !defined(INSTALL_PAN_MODULE)
#define INSTALL_PAN_MODULE
#endif
#else
#if !defined(EXCLUDE_BNEP_MODULE)
#define EXCLUDE_BNEP_MODULE
#endif
#undef INSTALL_PAN_MODULE
#endif

/* Corestack: DM */
#if defined(BCHS_RUN_TASK_DM) && (BCHS_RUN_TASK_DM == 1)
extern void init_dm(void **gash);
extern void dm_iface_handler(void **gash);
#define DM_INIT init_dm
#define DM_TASK dm_iface_handler
#ifdef ENABLE_SHUTDOWN
extern void deinit_dm_iface(void **gash);
#define DM_DEINIT deinit_dm_iface
#else
#define DM_DEINIT NULL
#endif
#else
#define DM_INIT NULL
#define DM_DEINIT NULL
#define DM_TASK NULL
#endif

/* Corestack: L2CAP proxy */
#if defined(BCHS_RUN_TASK_L2CAP_PROXY) && (BCHS_RUN_TASK_L2CAP_PROXY == 1)
extern void BchsL2capProxyInit(void **gash);
extern void BchsL2capProxyHandler(void **gash);
#define BCHS_L2CAP_PROXY_INIT BchsL2capProxyInit
#define BCHS_L2CAP_PROXY_HANDLER BchsL2capProxyHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsL2capProxyDeinit(void **gash);
#define BCHS_L2CAP_PROXY_DEINIT BchsL2capProxyDeinit
#else
#define BCHS_L2CAP_PROXY_DEINIT NULL
#endif
#else
#define BCHS_L2CAP_PROXY_INIT NULL
#define BCHS_L2CAP_PROXY_DEINIT NULL
#define BCHS_L2CAP_PROXY_HANDLER NULL
#endif

/* Corestack: RFCOMM proxy */
#if defined(BCHS_RUN_TASK_RFCOMM_PROXY) && (BCHS_RUN_TASK_RFCOMM_PROXY == 1) && !defined(EXCLUDE_BCHS_RFC_MODULE)
extern void BchsRfcommProxyInit(void **gash);
extern void BchsRfcommProxyHandler(void **gash);
#define BCHS_RFCOMM_PROXY_INIT BchsRfcommProxyInit
#define BCHS_RFCOMM_PROXY_HANDLER BchsRfcommProxyHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsRfcommProxyDeinit(void **gash);
#define BCHS_RFCOMM_PROXY_DEINIT BchsRfcommProxyDeinit
#else
#define BCHS_RFCOMM_PROXY_DEINIT NULL
#endif
#else
#define BCHS_RFCOMM_PROXY_INIT NULL
#define BCHS_RFCOMM_PROXY_DEINIT NULL
#define BCHS_RFCOMM_PROXY_HANDLER NULL
#endif

/* Corestack: SDP */
#if defined(BCHS_RUN_TASK_SDP) && (BCHS_RUN_TASK_SDP == 1)
extern void init_sdp(void **gash);
extern void sdp_sda_handler(void **gash);
#define SDP_INIT init_sdp
#define SDP_TASK sdp_sda_handler
#ifdef ENABLE_SHUTDOWN
extern void deinit_sdp_sda(void **gash);
#define SDP_DEINIT deinit_sdp_sda
#else
#define SDP_DEINIT NULL
#endif
#else
#define SDP_INIT NULL
#define SDP_DEINIT NULL
#define SDP_TASK NULL
#endif

/* Corestack: DM-HCI */
#define DM_HCI_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_DM_HCI) && (BCHS_RUN_TASK_DM_HCI == 1)
extern void dm_hci_handler(void **gash);
#define DM_HCI_TASK dm_hci_handler
#ifdef ENABLE_SHUTDOWN
extern void dm_hci_deinit(void **gash);
#define DM_HCI_DEINIT dm_hci_deinit
#else
#define DM_HCI_DEINIT NULL
#endif
#else
#define DM_HCI_DEINIT NULL
#define DM_HCI_TASK NULL
#endif

/* Corestack: DM-ACL */
#define DM_ACL_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_DM_ACL) && (BCHS_RUN_TASK_DM_ACL == 1)
extern void dm_acl_handler(void **gash);
#define DM_ACL_TASK dm_acl_handler
#ifdef ENABLE_SHUTDOWN
extern void dm_acl_deinit(void **gash);
#define DM_ACL_DEINIT dm_acl_deinit
#else
#define DM_ACL_DEINIT NULL
#endif
#else
#define DM_ACL_DEINIT NULL
#define DM_ACL_TASK NULL
#endif

/* Corestack: L2CAP */
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_L2CAP) && (BCHS_RUN_TASK_L2CAP == 1)
extern void init_l2cap(void **gash);
extern void l2cap_iface_handler(void **gash);
#define L2CAP_INIT init_l2cap
#define L2CAP_TASK l2cap_iface_handler
#ifdef ENABLE_SHUTDOWN
extern void l2cap_deinit(void **gash);
#define L2CAP_DEINIT l2cap_deinit
#else
#define L2CAP_DEINIT NULL
#endif
#else
#define L2CAP_INIT NULL
#define L2CAP_DEINIT NULL
#define L2CAP_TASK NULL
#endif

/* Corestack: L2CAP-DM */
#define L2CAP_DM_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_L2CAP_DM) && (BCHS_RUN_TASK_L2CAP_DM == 1)
extern void l2cap_dm_handler(void **gash);
#define L2CAP_DM_TASK l2cap_dm_handler
#ifdef ENABLE_SHUTDOWN
extern void l2cap_dm_deinit(void **gash);
#define L2CAP_DM_DEINIT l2cap_dm_deinit
#else
#define L2CAP_DM_DEINIT NULL
#endif
#else
#define L2CAP_DM_DEINIT NULL
#define L2CAP_DM_TASK NULL
#endif

/* Corestack: RFCOMM */
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_RFCOMM) && (BCHS_RUN_TASK_RFCOMM == 1) && !defined(EXCLUDE_BCHS_RFC_MODULE)
extern void init_rfcomm(void **gash);
extern void rfcomm_iface_handler(void **gash);
#define RFCOMM_INIT init_rfcomm
#define RFCOMM_TASK rfcomm_iface_handler
#ifdef ENABLE_SHUTDOWN
extern void rfcomm_deinit(void **gash);
#define RFCOMM_DEINIT rfcomm_deinit
#else
#define RFCOMM_DEINIT NULL
#endif
#else
#define RFCOMM_INIT NULL
#define RFCOMM_DEINIT NULL
#define RFCOMM_TASK NULL
#endif

/* Corestack: RFCOMM-L2CAP */
#define RFCOMM_L2CAP_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_RFCOMM_L2CAP) && (BCHS_RUN_TASK_RFCOMM_L2CAP == 1) && !defined(EXCLUDE_BCHS_RFC_MODULE)
extern void rfcomm_l2cap_handler(void **gash);
#define RFCOMM_L2CAP_TASK rfcomm_l2cap_handler
#ifdef ENABLE_DEBUG
extern void rfcomm_l2cap_deinit(void **gash);
#define RFCOMM_L2CAP_DEINIT rfcomm_l2cap_deinit
#else
#define RFCOMM_L2CAP_DEINIT NULL
#endif
#else
#define RFCOMM_L2CAP_DEINIT NULL
#define RFCOMM_L2CAP_TASK NULL
#endif

/* Corestack: RFCOMM-DM */
#define RFCOMM_DM_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_RFCOMM_DM) && (BCHS_RUN_TASK_RFCOMM_DM == 1) && !defined(EXCLUDE_BCHS_RFC_MODULE)
extern void rfcomm_dm_handler(void **gash);
#define RFCOMM_DM_TASK rfcomm_dm_handler
#ifdef ENABLE_DEBUG
extern void rfcomm_dm_deinit(void **gash);
#define RFCOMM_DM_DEINIT rfcomm_dm_deinit
#else
#define RFCOMM_DM_DEINIT NULL
#endif
#else
#define RFCOMM_DM_DEINIT NULL
#define RFCOMM_DM_TASK NULL
#endif

/* Corestack: SDP-L2CAP */
#define SDP_L2CAP_INIT NULL
#if !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_SDP_L2CAP) && (BCHS_RUN_TASK_SDP_L2CAP == 1)
extern void sdp_l2cap_handler(void **gash);
#define SDP_L2CAP_TASK sdp_l2cap_handler
#ifdef ENABLE_SHUTDOWN
extern void sdp_l2cap_deinit(void **gash);
#define SDP_L2CAP_DEINIT sdp_l2cap_deinit
#else
#define SDP_L2CAP_DEINIT NULL
#endif
#else
#define SDP_L2CAP_TASK NULL
#define SDP_L2CAP_DEINIT NULL
#endif

/* Profiles: BCCMD */
#if !defined(EXCLUDE_BCCMD_MODULE) && defined(BCHS_RUN_TASK_BCCMD) && (BCHS_RUN_TASK_BCCMD == 1)
extern void BchsBccmdInit(void **gash);
extern void BchsBccmdHandler(void **gash);
#define BCHS_BCCMD_INIT BchsBccmdInit
#define BCHS_BCCMD_HANDLER BchsBccmdHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBccmdDeinit(void **gash);
#define BCHS_BCCMD_DEINIT BchsBccmdDeinit
#else
#define BCHS_BCCMD_DEINIT NULL
#endif
#else
#define BCHS_BCCMD_INIT NULL
#define BCHS_BCCMD_DEINIT NULL
#define BCHS_BCCMD_HANDLER NULL
#endif

/* Profiles: CM */
#if !defined(EXCLUDE_CM_MODULE) && defined(BCHS_RUN_TASK_CM) && (BCHS_RUN_TASK_CM == 1)
extern void BchsCmInit(void **gash);
extern void BchsCmHandler(void **gash);
#define BCHS_CM_INIT BchsCmInit
#define BCHS_CM_HANDLER BchsCmHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsCmDeinit(void **gash);
#define BCHS_CM_DEINIT BchsCmDeinit
#else
#define BCHS_CM_DEINIT NULL
#endif
#else
#define BCHS_CM_INIT NULL
#define BCHS_CM_DEINIT NULL
#define BCHS_CM_HANDLER NULL
#endif

/* Profiles: SC */
#if !defined(EXCLUDE_CM_MODULE) && defined(BCHS_RUN_TASK_SC) && (BCHS_RUN_TASK_SC == 1)
extern void BchsScInit(void **gash);
extern void BchsScHandler(void **gash);
#define BCHS_SC_INIT BchsScInit
#define BCHS_SC_HANDLER BchsScHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsScDeinit(void **gash);
#define BCHS_SC_DEINIT BchsScDeinit
#else
#define BCHS_SC_DEINIT NULL
#endif
#else
#define BCHS_SC_INIT NULL
#define BCHS_SC_DEINIT NULL
#define BCHS_SC_HANDLER NULL
#endif

/* Profile: DG */
#if !defined(EXCLUDE_DG_MODULE) && defined(BCHS_RUN_TASK_DG) && (BCHS_RUN_TASK_DG == 1)
extern void BchsDgInit(void **gash);
extern void BchsDgHandler(void **gash);
#define BCHS_DG_INIT BchsDgInit
#define BCHS_DG_HANDLER BchsDgHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsDgDeinit(void **gash);
#define BCHS_DG_DEINIT BchsDgDeinit
#else
#define BCHS_DG_DEINIT NULL
#endif
#else
#define BCHS_DG_INIT NULL
#define BCHS_DG_DEINIT NULL
#define BCHS_DG_HANDLER NULL
#endif

/* Profile: AT */
#if !defined(EXCLUDE_AT_MODULE) && defined(BCHS_RUN_TASK_AT) && (BCHS_RUN_TASK_AT == 1)
extern void BchsAtInit(void **gash);
extern void BchsAtHandler(void **gash);
#define BCHS_AT_INIT BchsAtInit
#define BCHS_AT_HANDLER BchsAtHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsAtDeinit(void **gash);
#define BCHS_AT_DEINIT BchsAtDeinit
#else
#define BCHS_AT_DEINIT NULL
#endif
#else
#define BCHS_AT_INIT NULL
#define BCHS_AT_DEINIT NULL
#define BCHS_AT_HANDLER NULL
#endif

/* Profile: DUNC */
#if !defined(EXCLUDE_DUNC_MODULE) && defined(BCHS_RUN_TASK_DUNC) && (BCHS_RUN_TASK_DUNC == 1)
extern void BchsDuncInit(void **gash);
extern void BchsDuncHandler(void **gash);
#define BCHS_DUNC_INIT BchsDuncInit
#define BCHS_DUNC_HANDLER BchsDuncHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsDuncDeinit(void **gash);
#define BCHS_DUNC_DEINIT BchsDuncDeinit
#else
#define BCHS_DUNC_DEINIT NULL
#endif
#else
#define BCHS_DUNC_INIT NULL
#define BCHS_DUNC_DEINIT NULL
#define BCHS_DUNC_HANDLER NULL
#endif

/* Profile: OPS */
#if !defined(EXCLUDE_OPS_MODULE) && defined(BCHS_RUN_TASK_OPS) && (BCHS_RUN_TASK_OPS == 1)
extern void BchsOpsInit(void **gash);
extern void BchsOpsHandler(void **gash);
#define BCHS_OPS_INIT BchsOpsInit
#define BCHS_OPS_HANDLER BchsOpsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsOpsDeinit(void **gash);
#define BCHS_OPS_DEINIT BchsOpsDeinit
#else
#define BCHS_OPS_DEINIT NULL
#endif
#else
#define BCHS_OPS_INIT NULL
#define BCHS_OPS_DEINIT NULL
#define BCHS_OPS_HANDLER NULL
#endif

/* Profile: OPC */
#if !defined(EXCLUDE_OPC_MODULE) && defined(BCHS_RUN_TASK_OPC) && (BCHS_RUN_TASK_OPC == 1)
extern void BchsOpcInit(void **gash);
extern void BchsOpcHandler(void **gash);
#define BCHS_OPC_INIT BchsOpcInit
#define BCHS_OPC_HANDLER BchsOpcHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsOpcDeinit(void **gash);
#define BCHS_OPC_DEINIT BchsOpcDeinit
#else
#define BCHS_OPC_DEINIT NULL
#endif
#else
#define BCHS_OPC_INIT NULL
#define BCHS_OPC_DEINIT NULL
#define BCHS_OPC_HANDLER NULL
#endif

/* Profile: SYNCS */
#if !defined(EXCLUDE_SYNCS_MODULE) && defined(BCHS_RUN_TASK_SYNCS) && (BCHS_RUN_TASK_SYNCS == 1)
extern void BchsSyncsInit(void **gash);
extern void BchsSyncsHandler(void **gash);
#define BCHS_SYNCS_INIT BchsSyncsInit
#define BCHS_SYNCS_HANDLER BchsSyncsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSyncsDeinit(void **gash);
#define BCHS_SYNCS_DEINIT BchsSyncsDeinit
#else
#define BCHS_SYNCS_DEINIT NULL
#endif
#else
#define BCHS_SYNCS_INIT NULL
#define BCHS_SYNCS_DEINIT NULL
#define BCHS_SYNCS_HANDLER NULL
#endif

/* Profile: SMLC */
#if !defined(EXCLUDE_SMLC_MODULE) && defined(BCHS_RUN_TASK_SMLC) && (BCHS_RUN_TASK_SMLC == 1)
extern void BchsSmlcInit(void **gash);
extern void BchsSmlcHandler(void **gash);
#define BCHS_SMLC_INIT BchsSmlcInit
#define BCHS_SMLC_HANDLER BchsSmlcHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSmlcDeinit(void **gash);
#define BCHS_SMLC_DEINIT BchsSmlcDeinit
#else
#define BCHS_SMLC_DEINIT NULL
#endif
#else
#define BCHS_SMLC_INIT NULL
#define BCHS_SMLC_DEINIT NULL
#define BCHS_SMLC_HANDLER NULL
#endif

/* Profile: SMLS */
#if !defined(EXCLUDE_SMLS_MODULE) && defined(BCHS_RUN_TASK_SMLS) && (BCHS_RUN_TASK_SMLS == 1)
extern void BchsSmlsInit(void **gash);
extern void BchsSmlsHandler(void **gash);
#define BCHS_SMLS_INIT BchsSmlsInit
#define BCHS_SMLS_HANDLER BchsSmlsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSmlsDeinit(void **gash);
#define BCHS_SMLS_DEINIT BchsSmlsDeinit
#else
#define BCHS_SMLS_DEINIT NULL
#endif
#else
#define BCHS_SMLS_INIT NULL
#define BCHS_SMLS_DEINIT NULL
#define BCHS_SMLS_HANDLER NULL
#endif

/* Profile: IWU */
#if !defined(EXCLUDE_IWU_MODULE) && defined(BCHS_RUN_TASK_IWU) && (BCHS_RUN_TASK_IWU == 1)
extern void BchsIwuInit(void **gash);
extern void BchsIwuHandler(void **gash);
#define BCHS_IWU_INIT BchsIwuInit
#define BCHS_IWU_HANDLER BchsIwuHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsIwuDeinit(void **gash);
#define BCHS_IWU_DEINIT BchsIwuDeinit
#else
#define BCHS_IWU_DEINIT NULL
#endif
#else
#define BCHS_IWU_INIT NULL
#define BCHS_IWU_DEINIT NULL
#define BCHS_IWU_HANDLER NULL
#endif

/* Profile: SPP */
#if !defined(EXCLUDE_SPP_MODULE) && defined(BCHS_RUN_TASK_SPP) && (BCHS_RUN_TASK_SPP == 1) && !defined(EXCLUDE_SCO_MODULE)
extern void BchsSppInit(void **gash);
extern void BchsSppHandler(void **gash);
#define BCHS_SPP_INIT BchsSppInit
#define BCHS_SPP_HANDLER BchsSppHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSppDeinit(void **gash);
#define BCHS_SPP_DEINIT BchsSppDeinit
#else
#define BCHS_SPP_DEINIT NULL
#endif
#else
#define BCHS_SPP_INIT NULL
#define BCHS_SPP_DEINIT NULL
#define BCHS_SPP_HANDLER NULL
#endif

/* Profile: FAX */
#if !defined(EXCLUDE_FAX_MODULE) && defined(BCHS_RUN_TASK_FAX) && (BCHS_RUN_TASK_FAX == 1)
extern void BchsFaxInit(void **gash);
extern void BchsFaxHandler(void **gash);
#define BCHS_FAX_INIT BchsFaxInit
#define BCHS_FAX_HANDLER BchsFaxHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsFaxDeinit(void **gash);
#define BCHS_FAX_DEINIT BchsFaxDeinit
#else
#define BCHS_FAX_DEINIT NULL
#endif
#else
#define BCHS_FAX_INIT NULL
#define BCHS_FAX_DEINIT NULL
#define BCHS_FAX_HANDLER NULL
#endif

/* Profile: FM */
#if !defined(EXCLUDE_FM_MODULE) && defined(BCHS_RUN_TASK_FM) && (BCHS_RUN_TASK_FM == 1)
extern void BchsFmInit(void **gash);
extern void BchsFmHandler(void **gash);
#define BCHS_FM_INIT BchsFmInit
#define BCHS_FM_HANDLER BchsFmHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsFmDeinit(void **gash);
#define BCHS_FM_DEINIT BchsFmDeinit
#else
#define BCHS_FM_DEINIT NULL
#endif
#else
#define BCHS_FM_INIT NULL
#define BCHS_FM_DEINIT NULL
#define BCHS_FM_HANDLER NULL
#endif

/* Profile: HQ */
#if !defined(EXCLUDE_HQ_MODULE) && !defined(RFC_BUILD) && defined(BCHS_RUN_TASK_HQ) && (BCHS_RUN_TASK_HQ == 1)
extern void BchsHqInit(void **gash);
extern void BchsHqHandler(void **gash);
#define BCHS_HQ_INIT BchsHqInit
#define BCHS_HQ_HANDLER BchsHqHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHqDeinit(void **gash);
#define BCHS_HQ_DEINIT BchsHqDeinit
#else
#define BCHS_HQ_DEINIT NULL
#endif
#else
#define BCHS_HQ_INIT NULL
#define BCHS_HQ_DEINIT NULL
#define BCHS_HQ_HANDLER NULL
#endif

/* Profile: HFG */
#if !defined(EXCLUDE_HFG_MODULE) && defined(BCHS_RUN_TASK_HFG) && (BCHS_RUN_TASK_HFG == 1) && !defined(EXCLUDE_SCO_MODULE)
extern void BchsHfgInit(void **gash);
extern void BchsHfgHandler(void **gash);
#define BCHS_HFG_INIT BchsHfgInit
#define BCHS_HFG_HANDLER BchsHfgHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHfgDeinit(void **gash);
#define BCHS_HFG_DEINIT BchsHfgDeinit
#else
#define BCHS_HFG_DEINIT NULL
#endif
#else
#define BCHS_HFG_INIT NULL
#define BCHS_HFG_DEINIT NULL
#define BCHS_HFG_HANDLER NULL
#endif

/* Profile: HF */
#if !defined(EXCLUDE_HF_MODULE) && defined(BCHS_RUN_TASK_HF) && (BCHS_RUN_TASK_HF == 1) && !defined(EXCLUDE_SCO_MODULE)
extern void BchsHfInit(void **gash);
extern void BchsHfHandler(void **gash);
#define BCHS_HF_INIT BchsHfInit
#define BCHS_HF_HANDLER BchsHfHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHfDeinit(void **gash);
#define BCHS_HF_DEINIT BchsHfDeinit
#else
#define BCHS_HF_DEINIT NULL
#endif
#else
#define BCHS_HF_INIT NULL
#define BCHS_HF_DEINIT NULL
#define BCHS_HF_HANDLER NULL
#endif

/* Profile: PAC */
#if !defined(EXCLUDE_PAC_MODULE) && defined(BCHS_RUN_TASK_PAC) && (BCHS_RUN_TASK_PAC == 1)
extern void BchsPacInit(void **gash);
extern void BchsPacHandler(void **gash);
#define BCHS_PAC_INIT BchsPacInit
#define BCHS_PAC_HANDLER BchsPacHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsPacDeinit(void **gash);
#define BCHS_PAC_DEINIT BchsPacDeinit
#else
#define BCHS_PAC_DEINIT NULL
#endif
#else
#define BCHS_PAC_INIT NULL
#define BCHS_PAC_DEINIT NULL
#define BCHS_PAC_HANDLER NULL
#endif

/* Profile: PAS */
#if !defined(EXCLUDE_PAS_MODULE) && defined(BCHS_RUN_TASK_PAS) && (BCHS_RUN_TASK_PAS == 1)
extern void BchsPasInit(void **gash);
extern void BchsPasHandler(void **gash);
#define BCHS_PAS_INIT BchsPasInit
#define BCHS_PAS_HANDLER BchsPasHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsPasDeinit(void **gash);
#define BCHS_PAS_DEINIT BchsPasDeinit
#else
#define BCHS_PAS_DEINIT NULL
#endif
#else
#define BCHS_PAS_INIT NULL
#define BCHS_PAS_DEINIT NULL
#define BCHS_PAS_HANDLER NULL
#endif

/* Profile: TCS */
#if !defined(EXCLUDE_TCS_MODULE) && defined(BCHS_RUN_TASK_TCS) && (BCHS_RUN_TASK_TCS == 1) && !defined(EXCLUDE_SCO_MODULE)
extern void BchsTcsInit(void **gash);
extern void BchsTcsHandler(void **gash);
#define BCHS_TCS_INIT BchsTcsInit
#define BCHS_TCS_HANDLER BchsTcsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsTcsDeinit(void **gash);
#define BCHS_TCS_DEINIT BchsTcsDeinit
#else
#define BCHS_TCS_DEINIT NULL
#endif
#else
#define BCHS_TCS_INIT NULL
#define BCHS_TCS_DEINIT NULL
#define BCHS_TCS_HANDLER NULL
#endif

/* Profile: FTS */
#if !defined(EXCLUDE_FTS_MODULE) && defined(BCHS_RUN_TASK_FTS) && (BCHS_RUN_TASK_FTS == 1)
extern void BchsFtsInit(void **gash);
extern void BchsFtsHandler(void **gash);
#define BCHS_FTS_INIT BchsFtsInit
#define BCHS_FTS_HANDLER BchsFtsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsFtsDeinit(void **gash);
#define BCHS_FTS_DEINIT BchsFtsDeinit
#else
#define BCHS_FTS_DEINIT NULL
#endif
#else
#define BCHS_FTS_INIT NULL
#define BCHS_FTS_DEINIT NULL
#define BCHS_FTS_HANDLER NULL
#endif

/* Profile: BNEP */
#if !defined(EXCLUDE_BNEP_MODULE) && defined(BCHS_RUN_TASK_BNEP) && (BCHS_RUN_TASK_BNEP == 1)
extern void BchsBnepInit(void **gash);
extern void BchsBnepHandler(void **gash);
#define BCHS_BNEP_INIT BchsBnepInit
#define BCHS_BNEP_HANDLER BchsBnepHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBnepDeinit(void **gash);
#define BCHS_BNEP_DEINIT BchsBnepDeinit
#else
#define BCHS_BNEP_DEINIT NULL
#endif
#else
#define BCHS_BNEP_INIT NULL
#define BCHS_BNEP_DEINIT NULL
#define BCHS_BNEP_HANDLER NULL
#endif

/* Profile: BSL */
#if !defined(EXCLUDE_BSL_MODULE) && defined(BCHS_RUN_TASK_BSL) && (BCHS_RUN_TASK_BSL == 1)
extern void BchsBslInit(void **gash);
extern void BchsBslHandler(void **gash);
#define BCHS_BSL_INIT BchsBslInit
#define BCHS_BSL_HANDLER BchsBslHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBslDeinit(void **gash);
#define BCHS_BSL_DEINIT BchsBslDeinit
#else
#define BCHS_BSL_DEINIT NULL
#endif
#else
#define BCHS_BSL_INIT NULL
#define BCHS_BSL_DEINIT NULL
#define BCHS_BSL_HANDLER NULL
#endif

/* Profile: IP */
#if defined(BCHS_RUN_TASK_IP) && (BCHS_RUN_TASK_IP == 1)
extern void BchsIpInit(void ** gash);
extern void BchsIpHandler(void ** gash);
#define BCHS_IP_INIT BchsIpInit
#define BCHS_IP_TASK BchsIpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsIpDeinit(void ** gash);
#define BCHS_IP_DEINIT BchsIpDeinit
#else
#define BCHS_IP_DEINIT NULL
#endif
#else
#define BCHS_IP_INIT NULL
#define BCHS_IP_DEINIT NULL
#define BCHS_IP_TASK NULL
#endif

/* Profile: ICMP */
#if defined(BCHS_RUN_TASK_ICMP) && (BCHS_RUN_TASK_ICMP == 1)
extern void BchsIcmpInit(void ** gash);
extern void BchsIcmpHandler(void ** gash);
#define BCHS_ICMP_INIT BchsIcmpInit
#define BCHS_ICMP_TASK BchsIcmpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsIcmpDeinit(void ** gash);
#define BCHS_ICMP_DEINIT BchsIcmpDeinit
#else
#define BCHS_ICMP_DEINIT NULL
#endif
#else
#define BCHS_ICMP_INIT NULL
#define BCHS_ICMP_DEINIT NULL
#define BCHS_ICMP_TASK NULL
#endif

/* Profile: UDP */
#if defined(BCHS_RUN_TASK_UDP) && (BCHS_RUN_TASK_UDP == 1)
extern void BchsUdpInit(void ** gash);
extern void BchsUdpHandler(void ** gash);
#define BCHS_UDP_INIT BchsUdpInit
#define BCHS_UDP_HANDLER BchsUdpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsUdpDeinit(void ** gash);
#define BCHS_UDP_DEINIT BchsUdpDeinit
#else
#define BCHS_UDP_DEINIT NULL
#endif
#else
#define BCHS_UDP_INIT NULL
#define BCHS_UDP_DEINIT NULL
#define BCHS_UDP_HANDLER NULL
#endif

/* Profile: DHCP */
#if defined(BCHS_RUN_TASK_DHCP) && (BCHS_RUN_TASK_DHCP == 1)
extern void BchsDhcpInit(void ** gash);
extern void BchsDhcpHandler(void ** gash);
#define BCHS_DHCP_INIT BchsDhcpInit
#define BCHS_DHCP_TASK BchsDhcpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsDhcpDeinit(void ** gash);
#define BCHS_DHCP_DEINIT BchsDhcpDeinit
#else
#define BCHS_DHCP_DEINIT NULL
#endif
#else
#define BCHS_DHCP_INIT NULL
#define BCHS_DHCP_DEINIT NULL
#define BCHS_DHCP_TASK NULL
#endif

/* Profile: TFTP */
#if defined(BCHS_RUN_TASK_TFTP) && (BCHS_RUN_TASK_TFTP == 1)
extern void BchsTftpInit(void ** gash);
extern void BchsTftpHandler(void ** gash);
#define BCHS_TFTP_INIT BchsTftpInit
#define BCHS_TFTP_HANDLER BchsTftpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsTftpDeinit(void ** gash);
#define BCHS_TFTP_DEINIT BchsTftpDeinit
#else
#define BCHS_TFTP_DEINIT NULL
#endif
#else
#define BCHS_TFTP_INIT NULL
#define BCHS_TFTP_DEINIT NULL
#define BCHS_TFTP_HANDLER NULL
#endif

/* Helper for tester/tester: Ctrl */
#if defined(BDB2) && defined(BCHS_RUN_TASK_CTRL) && (BCHS_RUN_TASK_CTRL == 1)
extern void CtrlInit(void ** gash);
extern void CtrlHandler(void ** gash);
#define CTRL_INIT CtrlInit
#define CTRL_TASK CtrlHandler
#ifdef ENABLE_SHUTDOWN
extern void CtrlDeinit(void ** gash);
#define CTRL_DEINIT CtrlDeinit
#else
#define CTRL_DEINIT NULL
#endif
#else
#define CTRL_INIT NULL
#define CTRL_DEINIT NULL
#define CTRK_TASK NULL
#endif

/* Profile: BIPC */
#if !defined(EXCLUDE_BIPC_MODULE) && defined(BCHS_RUN_TASK_BIPC) && (BCHS_RUN_TASK_BIPC == 1)
extern void BchsBipcInit(void **gash);
extern void BchsBipcHandler(void **gash);
#define BCHS_BIPC_INIT BchsBipcInit
#define BCHS_BIPC_HANDLER BchsBipcHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBipcDeinit(void **gash);
#define BCHS_BIPC_DEINIT BchsBipcDeinit
#else
#define BCHS_BIPC_DEINIT NULL
#endif
#else
#define BCHS_BIPC_INIT NULL
#define BCHS_BIPC_DEINIT NULL
#define BCHS_BIPC_HANDLER NULL
#endif

/* Profile: BIPS */
#if !defined(EXCLUDE_BIPS_MODULE) && defined(BCHS_RUN_TASK_BIPS) && (BCHS_RUN_TASK_BIPS == 1)
extern void BchsBipsInit(void **gash);
extern void BchsBipsHandler(void **gash);
#define BCHS_BIPS_INIT BchsBipsInit
#define BCHS_BIPS_HANDLER BchsBipsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBipsDeinit(void **gash);
#define BCHS_BIPS_DEINIT BchsBipsDeinit
#else
#define BCHS_BIPS_DEINIT NULL
#endif
#else
#define BCHS_BIPS_INIT NULL
#define BCHS_BIPS_DEINIT NULL
#define BCHS_BIPS_HANDLER NULL
#endif

/* Profile: FTC */
#if !defined(EXCLUDE_FTC_MODULE) && defined(BCHS_RUN_TASK_FTC) && (BCHS_RUN_TASK_FTC == 1)
extern void BchsFtcInit(void **gash);
extern void BchsFtcHandler(void **gash);
#define BCHS_FTC_INIT BchsFtcInit
#define BCHS_FTC_HANDLER BchsFtcHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsFtcDeinit(void **gash);
#define BCHS_FTC_DEINIT BchsFtcDeinit
#else
#define BCHS_FTC_DEINIT NULL
#endif
#else
#define BCHS_FTC_INIT NULL
#define BCHS_FTC_DEINIT NULL
#define BCHS_FTC_HANDLER NULL
#endif

/* Profile: PPP */
#if !defined(EXCLUDE_PPP_MODULE) && defined(BCHS_RUN_TASK_PPP) && (BCHS_RUN_TASK_PPP == 1)
extern void BchsPppInit(void ** gash);
extern void BchsPppHandler(void ** gash);
#define BCHS_PPP_INIT BchsPppInit
#define BCHS_PPP_HANDLER BchsPppHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsPppDeinit(void ** gash);
#define BCHS_PPP_DEINIT BchsPppDeinit
#else
#define BCHS_PPP_DEINIT NULL
#endif
#else
#define BCHS_PPP_INIT NULL
#define BCHS_PPP_DEINIT NULL
#define BCHS_PPP_HANDLER NULL
#endif

/* Profile: BPP */
#if !defined(EXCLUDE_BPP_MODULE) && defined(BCHS_RUN_TASK_BPP) && (BCHS_RUN_TASK_BPP == 1)
extern void BchsBppInit(void **gash);
extern void BchsBppHandler(void **gash);
#define BCHS_BPP_INIT BchsBppInit
#define BCHS_BPP_HANDLER BchsBppHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBppDeinit(void **gash);
#define BCHS_BPP_DEINIT BchsBppDeinit
#else
#define BCHS_BPP_DEINIT NULL
#endif
#else
#define BCHS_BPP_INIT NULL
#define BCHS_BPP_DEINIT NULL
#define BCHS_BPP_HANDLER NULL
#endif

/* Profile: AV */
#if !defined(EXCLUDE_AV_MODULE) && defined(BCHS_RUN_TASK_AV) && (BCHS_RUN_TASK_AV == 1)
extern void BchsAvInit(void **gash);
extern void BchsAvHandler(void **gash);
#define BCHS_AV_INIT BchsAvInit
#define BCHS_AV_HANDLER BchsAvHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsAvDeinit(void **gash);
#define BCHS_AV_DEINIT BchsAvDeinit
#else
#define BCHS_AV_DEINIT NULL
#endif
#else
#define BCHS_AV_INIT NULL
#define BCHS_AV_DEINIT NULL
#define BCHS_AV_HANDLER NULL
#endif

/* Profile: AVRCP */
#if !defined(EXCLUDE_AVRCP_MODULE) && defined(BCHS_RUN_TASK_AVRCP) && (BCHS_RUN_TASK_AVRCP == 1)
extern void BchsAvrcpInit(void **gash);
extern void BchsAvrcpHandler(void **gash);
#define BCHS_AVRCP_INIT BchsAvrcpInit
#define BCHS_AVRCP_HANDLER BchsAvrcpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsAvrcpDeinit(void **gash);
#define BCHS_AVRCP_DEINIT BchsAvrcpDeinit
#else
#define BCHS_AVRCP_DEINIT NULL
#endif
#else
#define BCHS_AVRCP_INIT NULL
#define BCHS_AVRCP_DEINIT NULL
#define BCHS_AVRCP_HANDLER NULL
#endif

/* Profile: SAPS */
#if !defined(EXCLUDE_SAPS_MODULE) && defined(BCHS_RUN_TASK_SAPS) && (BCHS_RUN_TASK_SAPS == 1)
extern void BchsSapsInit(void **gash);
extern void BchsSapsHandler(void **gash);
#define BCHS_SAPS_INIT BchsSapsInit
#define BCHS_SAPS_HANDLER BchsSapsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSapsDeinit(void **gash);
#define BCHS_SAPS_DEINIT BchsSapsDeinit
#else
#define BCHS_SAPS_DEINIT NULL
#endif
#else
#define BCHS_SAPS_INIT NULL
#define BCHS_SAPS_DEINIT NULL
#define BCHS_SAPS_HANDLER NULL
#endif

/* Profile: SAPC */
#if !defined(EXCLUDE_SAPC_MODULE) && defined(BCHS_RUN_TASK_SAPC) && (BCHS_RUN_TASK_SAPC == 1)
extern void BchsSapcInit(void **gash);
extern void BchsSapcHandler(void **gash);
#define BCHS_SAPC_INIT BchsSapcInit
#define BCHS_SAPC_HANDLER BchsSapcHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSapcDeinit(void **gash);
#define BCHS_SAPC_DEINIT BchsSapcDeinit
#else
#define BCHS_SAPC_DEINIT NULL
#endif
#else
#define BCHS_SAPC_INIT NULL
#define BCHS_SAPC_DEINIT NULL
#define BCHS_SAPC_HANDLER NULL
#endif

/* Profiles: SD */
#if defined(BCHS_RUN_TASK_SD) && (BCHS_RUN_TASK_SD == 1)
extern void BchsSdInit(void **gash);
extern void BchsSdHandler(void **gash);
#define BCHS_SD_INIT BchsSdInit
#define BCHS_SD_HANDLER BchsSdHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsSdDeinit(void **gash);
#define BCHS_SD_DEINIT BchsSdDeinit
#else
#define BCHS_SD_DEINIT NULL
#endif
#else
#define BCHS_SD_INIT NULL
#define BCHS_SD_DEINIT NULL
#define BCHS_SD_HANDLER NULL
#endif

/* Profile: BPPS */
#if !defined(EXCLUDE_BPPS_MODULE) && defined(BCHS_RUN_TASK_BPPS) && (BCHS_RUN_TASK_BPPS == 1)
extern void BchsBppsInit(void **gash);
extern void BchsBppsHandler(void **gash);
#define BCHS_BPPS_INIT BchsBppsInit
#define BCHS_BPPS_HANDLER BchsBppsHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsBppsDeinit(void **gash);
#define BCHS_BPPS_DEINIT BchsBppsDeinit
#else
#define BCHS_BPPS_DEINIT NULL
#endif
#else
#define BCHS_BPPS_INIT NULL
#define BCHS_BPPS_DEINIT NULL
#define BCHS_BPPS_HANDLER NULL
#endif

/* Profile: HCRP */
#if !defined(EXCLUDE_HCRP_MODULE) && defined(BCHS_RUN_TASK_HCRP) && (BCHS_RUN_TASK_HCRP == 1)
extern void BchsHcrpInit(void **gash);
extern void BchsHcrpHandler(void **gash);
#define BCHS_HCRP_INIT BchsHcrpInit
#define BCHS_HCRP_HANDLER BchsHcrpHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHcrpDeinit(void **gash);
#define BCHS_HCRP_DEINIT BchsHcrpDeinit
#else
#define BCHS_HCRP_DEINIT NULL
#endif
#else
#define BCHS_HCRP_INIT NULL
#define BCHS_HCRP_DEINIT NULL
#define BCHS_HCRP_HANDLER NULL
#endif

/* Profile: HIDH */
#if !defined(EXCLUDE_HIDH_MODULE) && defined(BCHS_RUN_TASK_HIDH) && (BCHS_RUN_TASK_HIDH == 1)
extern void BchsHidhInit(void **gash);
extern void BchsHidhHandler(void **gash);
#define BCHS_HIDH_INIT BchsHidhInit
#define BCHS_HIDH_HANDLER BchsHidhHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHidhDeinit(void **gash);
#define BCHS_HIDH_DEINIT BchsHidhDeinit
#else
#define BCHS_HIDH_DEINIT NULL
#endif
#else
#define BCHS_HIDH_INIT NULL
#define BCHS_HIDH_DEINIT NULL
#define BCHS_HIDH_HANDLER NULL
#endif

/* Profile: HIDD */
#if !defined(EXCLUDE_HIDD_MODULE) && defined(BCHS_RUN_TASK_HIDD) && (BCHS_RUN_TASK_HIDD == 1)
extern void BchsHiddInit(void **gash);
extern void BchsHiddHandler(void **gash);
#define BCHS_HIDD_INIT BchsHiddInit
#define BCHS_HIDD_HANDLE BchsHiddHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsHiddDeinit(void **gash);
#define BCHS_HIDD_DEINIT BchsHiddDeinit
#else
#define BCHS_HIDD_DEINIT NULL
#endif
#else
#define BCHS_HIDD_INIT NULL
#define BCHS_HIDD_HANDLE NULL
#define BCHS_HIDD_DEINIT NULL
#endif

/* Profile: JSR82 */
#if !defined(EXCLUDE_JSR82_MODULE) && defined(BCHS_RUN_TASK_JSR82) && (BCHS_RUN_TASK_JSR82 == 1)
extern void BchsJsr82Init(void **gash);
extern void BchsJsr82Handler(void **gash);
#define BCHS_JSR82_HANDLER BchsJsr82Handler
#define BCHS_JSR82_INIT BchsJsr82Init
#ifdef ENABLE_SHUTDOWN
extern void BchsJsr82Deinit(void **gash);
#define BCHS_JSR82_DEINIT BchsJsr82Deinit
#else
#define BCHS_JSR82_DEINIT NULL
#endif
#else
#define BCHS_JSR82_INIT NULL
#define BCHS_JSR82_HANDLER NULL
#define BCHS_JSR82_DEINIT NULL
#endif

/* Application */
#if defined(BCHS_RUN_TASK_EXTRA_TASK) && (BCHS_RUN_TASK_EXTRA_TASK == 1)
extern void BchsExtraTaskInit(void **gash);
extern void BchsExtraTaskHandler(void **gash);
#define BCHS_EXTRA_TASK_INIT BchsExtraTaskInit
#define BCHS_EXTRA_TASK_HANDLER BchsExtraTaskHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsExtraTaskDeinit(void **gash);
#define BCHS_EXTRA_TASK_DEINIT BchsExtraTaskDeinit
#else
#define BCHS_EXTRA_TASK_DEINIT NULL
#endif
#else
#define BCHS_EXTRA_TASK_INIT NULL
#define BCHS_EXTRA_TASK_DEINIT NULL
#define BCHS_EXTRA_TASK_HANDLER NULL
#endif

/* Application */
#if !defined(EXCLUDE_TEST_MODULE) && defined(BCHS_RUN_TASK_TEST_TASK) && (BCHS_RUN_TASK_TEST_TASK == 1)
extern void BchsTestInit(void ** gash);
extern void BchsTestHandler(void ** gash);
#define BCHS_TEST_INIT BchsTestInit
#define BCHS_TEST_HANDLER BchsTestHandler
#ifdef ENABLE_SHUTDOWN
extern void BchsTestDeinit(void ** gash);
#define BCHS_TEST_DEINIT BchsTestDeinit
#else
#define BCHS_TEST_DEINIT NULL
#endif
#else
#define BCHS_TEST_INIT NULL
#define BCHS_TEST_DEINIT NULL
#define BCHS_TEST_HANDLER NULL
#endif

/* BCHS queue definitions */
extern uint16_t DM_IFACEQUEUE;
extern uint16_t DM_ACLQUEUE;
extern uint16_t DM_HCIQUEUE;
extern uint16_t L2CAP_IFACEQUEUE;
extern uint16_t L2CAP_PROXY_IFACEQUEUE;
extern uint16_t L2CAP_DMQUEUE;
extern uint16_t RFCOMM_IFACEQUEUE;
extern uint16_t RFCOMM_PROXY_IFACEQUEUE;
extern uint16_t RFCOMM_L2CAPQUEUE;
extern uint16_t RFCOMM_DMQUEUE;
extern uint16_t BCCMD_IFACEQUEUE;
extern uint16_t SDP_L2CAPQUEUE;
extern uint16_t SDP_IFACEQUEUE;
extern uint16_t CM_IFACEQUEUE;
extern uint16_t SC_IFACEQUEUE;
extern uint16_t DG_IFACEQUEUE;
extern uint16_t AT_IFACEQUEUE;
extern uint16_t OPS_IFACEQUEUE;
extern uint16_t OPC_IFACEQUEUE;
extern uint16_t SYNCS_IFACEQUEUE;
extern uint16_t IWU_IFACEQUEUE;
extern uint16_t SPP_IFACEQUEUE;
extern uint16_t FAX_IFACEQUEUE;
extern uint16_t HFG_IFACEQUEUE;
extern uint16_t HF_IFACEQUEUE;
extern uint16_t TCS_IFACEQUEUE;
extern uint16_t FTS_IFACEQUEUE;
extern uint16_t BNEP_QUEUE;
extern uint16_t BSL_IFACEQUEUE;
extern uint16_t IP_QUEUE;
extern uint16_t ICMP_QUEUE;
extern uint16_t UDP_QUEUE;
extern uint16_t DHCP_QUEUE;
extern uint16_t TFTP_QUEUE;
extern uint16_t BIPC_IFACEQUEUE;
extern uint16_t FTC_IFACEQUEUE;
extern uint16_t PPP_QUEUE;
extern uint16_t BPP_IFACEQUEUE;
extern uint16_t AV_IFACEQUEUE;
extern uint16_t AVRCP_IFACEQUEUE;
extern uint16_t SAPS_IFACEQUEUE;
extern uint16_t SAPC_IFACEQUEUE;
extern uint16_t SD_IFACEQUEUE;
extern uint16_t HIDH_IFACEQUEUE;
extern uint16_t BPPS_IFACEQUEUE;
extern uint16_t HCRP_IFACEQUEUE;
extern uint16_t BIPS_IFACEQUEUE;
extern uint16_t SMLC_IFACEQUEUE;
extern uint16_t SMLS_IFACEQUEUE;
extern uint16_t DUNC_IFACEQUEUE;
extern uint16_t JSR82_IFACEQUEUE;
extern uint16_t HIDD_IFACEQUEUE;
extern uint16_t FM_IFACEQUEUE;
extern uint16_t HQ_IFACEQUEUE;
extern uint16_t PAC_IFACEQUEUE;
extern uint16_t PAS_IFACEQUEUE;
extern uint16_t TESTQUEUE;
extern uint16_t EXTRA_IFACEQUEUE;

#ifdef __cplusplus
}
#endif

#endif
