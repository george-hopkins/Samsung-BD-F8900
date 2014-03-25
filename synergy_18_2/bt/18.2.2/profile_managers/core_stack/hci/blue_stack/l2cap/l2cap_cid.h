/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_cid.h

\brief  This file defines the interface to the L2CAP channel state machine.
*/

#ifndef _L2CAP_CID_H
#define _L2CAP_CID_H

#include "csr_synergy.h"

#include "csr_bt_core_stack_fsm.h"
#include "csr_bt_common.h"
#include "csr_mblk.h"
#include "l2cap_config.h"
#include "l2cap_types.h"
#include "dm_acl_core.h"
#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Connect request signal block

    The connect request signal block holds the parameters from the
    connect request signal, this block is used to pass the parameters into
    the state machine.
*/
typedef struct
{
    l2ca_identifier_t     signal_id;
    psm_t                 psm;
    l2ca_cid_t            source_cid;
} CID_CONNECT_REQ_T;

/*! \brief Configure request signal block

    The configure request signal block holds the parameters from the
    configure request signal, this block is used to pass the
    parameters into the state machine.
*/
typedef struct
{
    l2ca_identifier_t     signal_id;
    l2ca_cid_t            dest_cid;
    CsrUint16              flags;
    const CsrUint8         *options;
    CsrUint16              options_size;
} CID_CONFIG_REQ_T;

/*! \brief Configure response signal block

    The configure response signal block holds the parameters from the
    configure response signal, this block is used to pass the
    parameters into the state machine.
*/
typedef struct
{
    l2ca_identifier_t     signal_id;
    l2ca_cid_t            source_cid;
    CsrUint16              flags;
    CsrUint16              result;   /* l2ca_conf_result_t cast to CsrUint16 */
    const CsrUint8         *options;
    CsrUint16              options_size;
} CID_CONFIG_RES_T;

/*! \brief Disconnect request signal block

    The disconnect request signal block holds the parameters from the
    disconnect signal, this block is used to pass the parameters into
    the state machine.
*/
typedef struct
{
    l2ca_identifier_t     signal_id;
    l2ca_cid_t            dest_cid;
    l2ca_cid_t            source_cid;
} CID_DISCONNECT_REQ_T;

/*! \brief AMP create channel request signal block

    Create channel request block that holds the parameters from the
    peer's create channel request -- used for passing data into the
    FSM
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct
{
    l2ca_identifier_t     signal_id;
    psm_t                 psm;
    l2ca_cid_t            source_cid;
    CsrUint8               control_id;
} CID_CREATE_CHAN_REQ_T;
#endif

/*! \brief AMP physical verification block

    When the AMP manager has verified that a physical channel is
    present and read it's class identification, we pass this event
    structure into the FSM
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct
{
    CsrUint32              lsto;
    CsrUint32              flush_to;
} CID_VERIFY_LINK_CFM_T;
#endif

/*! \brief AMP move channel request signal block

    Move channel request block that holds the parameters from the
    peer's move channel request -- used for passing data into the
    FSM
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct
{
    l2ca_identifier_t     signal_id;
    l2ca_cid_t            source_cid;
    CsrUint8               control_id;
} CID_MOVE_CHAN_REQ_T;
#endif

/*! \brief AMP move channel confirm request signal block

    Move channel confirm request block that holds the parameters from
    the peer's move channel confirm "request" signal -- used for
    passing data into the FSM
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct
{
    l2ca_identifier_t     signal_id;
    l2ca_cid_t            source_cid;
    CsrUint16              result;   /* l2ca_move_result_t cast to CsrUint16 */
} CID_MOVE_CHAN_CONFIRM_REQ_T;
#endif

/*! \brief Is CID using a F&EC mode? */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#define CID_IsFlowControl(cidcb) ((cidcb)->flow != NULL)
#else
#define CID_IsFlowControl(cidcb) (FALSE)
#endif

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT

/* Is channel open? */
#define CID_IsOpen(cidcb) ((cidcb)->state == CID_ST_OPEN)

/*! \brief Check for reconfiguring channel */
#define CID_IsReconfiguring(cidcb) (((cidcb)->opened) \
        && ((cidcb)->state == CID_ST_CONFIG) \
        && (!CID_IsFlowControl(cidcb)))
#else

#define CID_IsOpen(cidcb) (TRUE)
#define CID_IsReconfiguring(cidcb)   (FALSE)

#endif

/*! \brief Check for fixed channel */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
#define CID_IsFixed(cidcb) ((cidcb)->remote_cid < L2CA_CID_DYNAMIC_FIRST \
        && (cidcb)->remote_cid != L2CA_CID_INVALID)
#else
#define CID_IsFixed(cidcb) (FALSE)
#endif

/*! \brief Check for fixed/connectionless channel capable of TX.

    Connectionless may only be capable of receiving data. Other types
    of fixed channel are always capable of both sending and receiving.
*/
#if defined (INSTALL_L2CAP_CONNLESS_SUPPORT) || defined (INSTALL_L2CAP_UCD_SUPPORT)
#define CID_IsFixedWithTX(cidcb) (CID_IsFixed(cidcb) && \
        ((cidcb)->remote_cid != L2CA_CID_CONNECTIONLESS \
            || (cidcb)->remote_psm != L2CA_PSM_INVALID))
#else /* INSTALL_L2CAP_CONNLESS_SUPPORT || INSTALL_L2CAP_UCD_SUPPORT */
#define CID_IsFixedWithTX(cidcb) CID_IsFixed(cidcb)
#endif /* INSTALL_L2CAP_CONNLESS_SUPPORT || INSTALL_L2CAP_UCD_SUPPORT */

/*! \brief Check for moving channel */
#ifdef INSTALL_AMP_SUPPORT
#define CID_IsMoving(cidcb) \
    (((cidcb)->state == CID_ST_MOVE_AMP_LOCAL) || \
     ((cidcb)->state == CID_ST_MOVE_AMP_REMOTE))
#else
#define CID_IsMoving(cidcb) (FALSE)
#endif

/*! \brief Get phandle used for upstream data */
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
#define CID_DataPhandle(cidcb) ((cidcb)->p_handle_data)
#else
#define CID_DataPhandle(cidcb) ((cidcb)->p_handle)
#endif

/*! \brief Is CID using a F&EC mode? */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#define CID_IsFlowControl(cidcb) ((cidcb)->flow != NULL)
#else
#define CID_IsFlowControl(cidcb) (FALSE)
#endif

/*! \brief Pending configuration helpers */

/*! \brief AMP support and configuration lockstep pending flags */
#ifdef INSTALL_AMP_SUPPORT
#define CID_IsAmp(cidcb) ((cidcb)->amp_info != NULL)
#define CID_SetLocalPending(cidcb, state)  ((cidcb)->local_pending = (state))
#define CID_SetRemotePending(cidcb, state) ((cidcb)->remote_pending = (state))
#define CID_GetBothPending(cidcb) ((cidcb)->local_pending && (cidcb)->remote_pending)
#define CID_GetAnyPending(cidcb)  ((cidcb)->local_pending || (cidcb)->remote_pending)
#else
#define CID_IsAmp(cidcb) (FALSE)
#define CID_SetLocalPending(cidcb, state)
#define CID_SetRemotePending(cidcb, state)
#define CID_GetBothPending(cidcb) (FALSE)
#define CID_GetAnyPending(cidcb)  (FALSE)
#endif

/* Get queue via AMP if supported, or directly from CHCB */
#ifdef INSTALL_AMP_SUPPORT
#define CID_GetTxQueue(cidcb) AMP_GetTxQueue((cidcb))
#else
#define CID_GetTxQueue(cidcb) (&(cidcb->chcb->queue))
#endif

/* AMP uses the 32bit FSM that has a different null event */
#ifndef DISABLE_AMP_FSM_SUPPORT
#define CID_EVENT_NULL FSM32_EVENT_NULL
#else
#define CID_EVENT_NULL FSM_EVENT_NULL
#endif


/*! \brief Collection buffer targets

    We may need to target all or a single specific collection buffer
    for certain operations, so stick them in an enum
*/
typedef enum
{
    CONFBUF_MIN = 0,
    CONFBUF_REQ = 0,                                            /*!< Target the"req" buffer */
    CONFBUF_CFM,                                                /*!< Target the"cfm" buffer */
    CONFBUF_IND,                                                /*!< Target the"ind" buffer */
    CONFBUF_RES,                                                /*!< Target the"res" buffer */
    CONFBUF_ALL                                                 /*!< Target allbuffers */
} L2CAP_CONFIG_BUFFERENUM_T;

/*! \brief Configuration request buffer

    When the configuration options exceed the MTU_signal, we need to
    keep the options in a local buffer to hide the complexities of the
    MTU_signal and L2CAP continuation flags to the upper layer. This
    buffer holds these options while we collect/transmit parts of the
    configuration.

    We need four different data chunks: req, cfm, ind, res plus length
    and offset fields for all of these as the configuration is always
    two-way *and* may happen simultaneous in both directions!

    Note: We only need this buffer when we're in the configuration
    phase, and even then we only need parts of it! We will free this
    buffer as soon as possible!
*/
typedef struct
{
    CsrUint8                              *buf;                  /*!< Pointer toconfiguration */
    CsrUint16                              len;                  /*!< Length of configuration */
    CsrUint16                              off;                  /*!< Current offset */
} L2CAP_CONFIG_BUFFER_T;

typedef struct
{
    L2CAP_CONFIG_BUFFER_T                 buffer[CONFBUF_ALL];
} L2CAP_CONFIG_CONTAINER_T;


/*! \brief AMP sub-states

    We use the standard CID state machine for AMP, but we still need
    to store whether an AMP logical channel creation was triggered by
    a Move or a Create so we can reuse the CONFIG part of the state
    machine. Further, we need the state so we can tell whether we can
    revert to the old channel (needed for moves) or not (creations).
*/
#ifdef INSTALL_AMP_SUPPORT
typedef enum
{
    AMPST_NULL = 0,                                             /*!< AMP channel is unconfigured */
    AMPST_DOWN,                                                 /*!< AMP link is down (resurrect through move) */
    AMPST_CREATING,                                             /*!< Channel create in progress */
    AMPST_MOVING_LOCAL,                                         /*!< Locally initiated channel move in progress */
    AMPST_MOVING_REMOTE,                                        /*!< Remotely initiated channel move in progress */
    AMPST_CONFIRM_LOCAL,                                        /*!< Locally initiated move, confirm move state */
    AMPST_CONFIRM_REMOTE,                                       /*!< Remotely initiated move, confirm move state */
    AMPST_RUNNING                                               /*!< AMP channel active */
} L2CAP_AMPSTATE_T;
#endif

/*! \brief AMP channel information placeholder

    We need to store a great deal of AMP information for each channel,
    so to save memory on non-AMP channels, store it in a structure so
    it can be allocated dynamically.
*/
#ifdef INSTALL_AMP_SUPPORT
typedef struct ampinfotag
{
    BITFIELD(L2CAP_AMPSTATE_T,            amp_state, 6);        /*!< Using AMP instead of BD/EDR */
    BITFIELD(CsrBool,                      logical_pending, 1);  /*!< Logical channel creation pending */
    BITFIELD(CsrBool,                      logical_ok, 1);       /*!< Logical channel OK */
    TXQUEUE_T                            *queue;                /*!< AMP transmit queue (may be NULL during setup) */
    l2ca_controller_t                     local_amp;            /*!< Local AMP controller ID */
    l2ca_controller_t                     remote_amp;           /*!< Remote AMPcontroller ID */
    l2ca_controller_t                     old_local_amp;        /*!< Old local AMP ID (needed for revert) */
    l2ca_controller_t                     old_remote_amp;       /*!< Old remoteAMP ID (needed for revert) */
    CsrUint8                               old_physical;         /*!< Old AMP physical channel ID (needed for revert) */
    amp_link_id_t                         old_logical;          /*!< Old AMP logical channel ID (needed for revert) */
    CsrSchedQid                             old_hci_data_queue;   /*!< Old PAL scheduler queue (needed for revert) */
    l2ca_move_result_t                    move_result;          /*!< Initiator side move result from peer */
    CsrUint32                              lsto;                 /*!< AMP controller link supervision timeout */
    CsrUint32                              be_flush_to;          /*!< AMP controller best effort flush timeout */
} AMPINFO_T;
#endif

/*! \brief Channel Control Block.

    The Channel Control Block holds the parameters needs to manage a L2CAP
    channel.  Each L2CAP channel will have an instance of this control block.
*/
typedef struct cidtag
{
    struct cidtag                        *next_ptr;             /*!< Pointer to next CIDCB */
    struct chtag                         *chcb;                 /*!< Pointer to parent ACL */
    l2ca_cid_t                            local_cid;            /*!< Local channel ID */
    l2ca_cid_t                            remote_cid;           /*!< Remote channel ID */
    l2ca_mtu_t                            local_mtu;            /*!< Local MTU */
    l2ca_mtu_t                            remote_mtu;           /*!< Remote MTU */
    l2ca_timeout_t                        local_flush_to;       /*!< Local flush timeout */
    L2CAP_CONFIG_CONTAINER_T             *confbuf;              /*!< Local config buffer to deal with MTU/continuation */
    DM_SM_SERVICE_T                      *dm_sm_service;        /*!< Substitute Security Service from higher layers. */
    CsrSchedTid                                 config_timer_id;      /*!< Config timer ID */
    struct psm_tag_t                      *local_psm;           /*!< Pointer to local PSM structure */
    psm_t                                 remote_psm;           /*!< Remote PSM */
    CsrSchedQid                             p_handle;             /*!< P-Handle for control primitives */
#ifdef INSTALL_L2CAP_ROUTER_SUPPORT
    CsrSchedQid                             p_handle_data;        /*!< P-Handle for data primitives - only used when routing */
#endif
    CsrUint16                              context;              /*!< Opaque context number for upper layer */
    BITFIELD(l2ca_identifier_t,           rcnreq_signal_id, 8); /*!< Incoming connect-request signal ID */
    BITFIELD(CsrBool,                      local_config_ok, 1);  /*!< Configuration status */
    BITFIELD(CsrBool,                      remote_config_ok, 1); /*!< When response is split */
    BITFIELD(CsrBool,                      opened, 1);           /*!< Has channel ever been been OPENed? */
    BITFIELD(CsrBool,                      allow_retry, 1);      /*!< Can we retry if ACL detach coincides with connect? */
    BITFIELD(CsrUint8,                     priority, 2);         /*!< L2CAP queue priority. */
    BITFIELD(CsrBool,                      acl_unlocked, 1);     /*!< This CIDCB has not locked the ACL */

#ifdef INSTALL_L2CAP_RAW_SUPPORT
    BITFIELD(CsrBool,                      raw_rx, 1);           /*!< Raw packet support */
#endif

    /* Flowspec and lockstep support */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
    BITFIELD(CsrBool,                      use_lockstep, 1);     /*!< Use of flowspecs/lockstep enabled */
    BITFIELD(CsrBool,                      remote_pending, 1);   /*!< Remote side has replied with CONFIG_PENDING */
    BITFIELD(CsrBool,                      local_pending, 1);    /*!< Remote side has replied with CONFIG_PENDING */
    BITFIELD(l2ca_fs_identifier_t,        flowspec_id, 8);      /*!< Flowspec identifier counter */
    L2CA_FLOWSPEC_T                      *tx_flowspec;          /*!< AMP extended Tx flowspec */
    L2CA_FLOWSPEC_T                      *rx_flowspec;          /*!< AMP extended Rx flowspec */
#endif

    /* The FSM bitsize depends on whether AMP FSM support is in */
#ifndef DISABLE_AMP_FSM_SUPPORT
    fsm32_state_t                         state;                /*!< The state of the CID */
#else
    fsm16_state_t                         state;                /*!< The state of the CID */
#endif

    /* Conditional structures for AMP */
#ifdef INSTALL_AMP_SUPPORT
    AMPINFO_T                            *amp_info;             /*!< AMP information placeholder */
#endif

    /* Conditional structure for F&EC module */
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
    struct FLOWINFO_T_tag                *flow;                 /*!< Placeholder for flow control queue etc */
#endif
} CIDCB_T;

/* State machine frontend functions */
extern void CID_ConnectReq(CIDCB_T *cidcb);
extern void CID_ConnectRsp(CIDCB_T *cidcb, L2CA_CONNECT_RSP_T *rsp);
extern void CID_ConnectInd(CIDCB_T *cidcb, CID_CONNECT_REQ_T *signal_args);
extern void CID_ConnectIndRetry(CIDCB_T *cidcb, CID_CONNECT_REQ_T *signal_args);
extern void CID_ConnectCfm(CIDCB_T *cidcb, l2ca_cid_t dest_cid, l2ca_conn_result_t result);
extern void CID_DisconnectReq(CIDCB_T *cidcb);
extern void CID_DisconnectRsp(CIDCB_T *cidcb, L2CA_DISCONNECT_RSP_T *rsp);
extern void CID_DisconnectInd(CIDCB_T *cidcb, CID_DISCONNECT_REQ_T *signal_args);
extern void CID_DisconnectCfm(CIDCB_T *cidcb);
extern void CID_DisconnectReqInternal(CIDCB_T *cidcb, l2ca_disc_result_t result);
extern void CID_DisconnectIndInternal(CIDCB_T *cidcb, l2ca_disc_result_t result);
extern void CID_ConfigInd(CIDCB_T *cidcb, CID_CONFIG_REQ_T *signal_args);
extern void CID_ConfigCfm(CIDCB_T *cidcb, CID_CONFIG_RES_T *signal_args);
extern void CID_ConfigReq(CIDCB_T *cidcb, L2CA_CONFIG_REQ_T *req);
extern void CID_ConfigRsp(CIDCB_T *cidcb, L2CA_CONFIG_RSP_T *rsp);
extern void CID_Timeout(CIDCB_T *cidcb);
extern void CID_InvalidCid(CIDCB_T *cidcb);
extern void CID_AclReady(CIDCB_T *cidcb);
extern void CID_AclCloseInd(CIDCB_T *cidcb, CsrUint16 status);
extern void CID_SignalNcpInd(CIDCB_T *cidcb);
extern void CID_Destroy(CIDCB_T *cidcb, hci_reason_t reason, ACL_IDLE_TIMEOUT_MULTIPLIER_T timeout);
extern void CID_StandardDestroy(CIDCB_T *cidcb);
extern void CID_SmAccessCfm(DM_SM_ACCESS_CFM_T *p_prim);
extern void CID_SendConnectCfm(CIDCB_T *cidcb, l2ca_conn_result_t result);

/* Data path - bypasses state machine */
extern void CID_DataWriteReq(CIDCB_T *cidcb, CsrMblk **mblk_ptr, CsrUint16 context);
extern CsrBool CID_DataReadInd(CIDCB_T *cidcb, CsrMblk *mblk_ptr, CsrMblkSize mblk_size, CsrUint8 *header);

/* Utility functions */
extern void CID_SendDMSMAccessReq(CIDCB_T *cidcb, CsrBool incoming);
extern void CID_FsmRun(CIDCB_T *cidcb, fsm_event_t event, void *param2);
extern void CID_ConfigTimeout(CsrUint16 param, void *cidcb_in);
extern void CID_DisconnectCleanup(CIDCB_T *cidcb);
extern void CID_ConfigTimerStart(CIDCB_T *cidcb);
extern void CID_StoreConfiguration(CIDCB_T *cidcb,
                                   L2CA_CONFIG_T *local,
                                   L2CA_CONFIG_T *remote);
extern fsm_event_t CID_ConfigComplete(CIDCB_T *cidcb);

#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
extern void CID_AbortTX(CIDCB_T *cidcb);
#else
#define CID_AbortTX(cidcb) L2CA_DataWriteAbortCfm(cidcb, NULL)
#endif

#ifdef BUILD_FOR_HOST
#define CID_ClipMTU(cidcb, mtu, in_out)
#else
/* Clip MTU for on-chip buffer sizes */
extern void CID_ClipMTU(CIDCB_T *cidcb, l2ca_mtu_t *mtu, CsrBool from_air);
#endif

/* Connectionless support */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
extern void CID_DataWriteMulticastReq(l2ca_cid_t cid_table[], CsrMblk **mblk_ptr);
#endif

/* Fixed channel support */
#ifdef INSTALL_L2CAP_FIXED_CHANNEL_BASE_SUPPORT
extern void CID_MapFixedCidReq(CIDCB_T *cidcb);
extern void CID_MapFixedCidInd(CIDCB_T *cidcb);
#endif

/* BR/EDR and AMP flowspec support */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
extern fsm_event_t CID_LogicalComplete(CIDCB_T *cidcb);
#endif

/* AMP extensions */
#ifdef INSTALL_AMP_SUPPORT
extern void CID_SetFlowspecResponse(CIDCB_T *cidcb, L2CA_FLOWSPEC_T *fs);
extern void CID_AmpCreateReq(CIDCB_T *cidcb);
extern void CID_AmpCreateCfm(CIDCB_T *cidcb, l2ca_cid_t cid, l2ca_conn_result_t result);
extern void CID_AmpCreateInd(CIDCB_T *cidcb, CID_CREATE_CHAN_REQ_T *req);
extern void CID_AmpCreateIndRetry(CIDCB_T *cidcb, CID_CREATE_CHAN_REQ_T *req);
extern void CID_AmpCreateRsp(CIDCB_T *cidcb, L2CA_CREATE_CHANNEL_RSP_T *res);

extern void CID_AmpMoveReq(CIDCB_T *cidcb, L2CA_MOVE_CHANNEL_REQ_T *req);
extern void CID_AmpMoveCfm(CIDCB_T *cidcb, l2ca_move_result_t result);
extern void CID_AmpMoveInd(CIDCB_T *cidcb, CID_MOVE_CHAN_REQ_T *req);
extern void CID_AmpMoveRsp(CIDCB_T *cidcb, L2CA_MOVE_CHANNEL_RSP_T *res);

extern void CID_AmpConfirmInd(CIDCB_T *cidcb, CID_MOVE_CHAN_CONFIRM_REQ_T *req);
extern void CID_AmpConfirmCfm(CIDCB_T *cidcb);

extern void CID_AmpCreatePhysicalCfm(CIDCB_T *cidcb, hci_return_t result, l2ca_controller_t remote_amp_id);
extern void CID_AmpCreateLogicalCfm(CIDCB_T *cidcb, DM_AMPM_CONNECT_CHANNEL_RSP_T *prim);
extern void CID_AmpVerifyPhysicalLinkCfm(CIDCB_T *cidcb, CsrBool exists, CID_VERIFY_LINK_CFM_T *cfm);

extern void CID_AmpDisconnectInd(CIDCB_T *cidcb, hci_error_t reason);
#endif

#ifdef __cplusplus
}
#endif
#endif
