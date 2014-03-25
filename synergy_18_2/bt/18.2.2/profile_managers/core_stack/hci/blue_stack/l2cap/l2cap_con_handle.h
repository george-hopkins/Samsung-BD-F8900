/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_con_handle.h

\brief  This file defines the interface for L2CAP connections, there is
        a one-to-one mapping between a L2CAP connection and an ACL.
*/

#ifndef _L2CAP_CON_HANDLE_H
#define _L2CAP_CON_HANDLE_H

#include "csr_synergy.h"

#include "csr_mblk.h"
#include "csr_bt_common.h"
#include "l2cap_config.h"
#include "l2cap_types.h"
#include "dm_acl_core.h"
#include "dm_acl_manager.h"
#include "l2cap_cid.h"
#include "dm_prim.h"
#include "dm_amp_interface.h"
#include "tbdaddr.h"

/* Get L2CA_CHCB_T from DM_ACL_T */
#define CH_GET_CHCB(p_acl) \
    DM_ACL_CLIENT_GET_DATA((p_acl), dm_acl_client_l2cap)

/* Get DM_ACL_T from L2CA_CHCB_T */
#define CH_GET_ACL(chcb) \
    DM_ACL_CLIENT_GET_ACL((chcb), dm_acl_client_l2cap)

/* Get BD_ADDR_T from L2CA_CHCB_T */
#define CH_GET_TBDADDR(chcb) \
    DM_ACL_CLIENT_GET_TBDADDR((chcb), dm_acl_client_l2cap)
#define CH_GET_BD_ADDR(chcb) (&TBDADDR_ADDR(*CH_GET_TBDADDR(chcb)))

/* Get HCI Connection Handle from L2CA_CHCB_T */
#define CH_GET_HANDLE(chcb) \
    DM_ACL_CLIENT_GET_HANDLE((chcb), dm_acl_client_l2cap)

/* Determine whether L2CA_CHCB_T is for BLE or BR/EDR */
#ifdef INSTALL_ULP
#define CH_IS_ULP(chcb)   dm_acl_is_ble(CH_GET_ACL(chcb))
#define CH_ULP_FLAG(chcb) DM_ACL_GET_CONNECTION_FLAGS(CH_GET_ACL(chcb))
#else
#define CH_IS_ULP(chcb) FALSE
#define CH_ULP_FLAG(chcb) ((l2ca_conflags_t)0)
#endif

#ifdef SUPPORT_SEPARATE_LE_BUFFERS
#define CH_GET_ACL_TYPE(chcb)  ((!CH_IS_ULP(chcb))? \
            DM_AMP_ACL_TYPE_BR_EDR : DM_AMP_ACL_TYPE_LE)
#define L2CA_FLAGS_ULP(flags) \
            (((flags) > DM_ACL_BR_EDR_FLAG_THRESHOLD)? TRUE:FALSE)
#else
#define CH_GET_ACL_TYPE(chcb)  DM_AMP_ACL_TYPE_BR_EDR
#define L2CA_FLAGS_ULP(flags)  FALSE
#endif

/* Extend the ACL idle timeout */
#define CH_EXTEND_TIMEOUT(chcb, multiplier, reason) \
    dm_acl_idle_timeout_extend( \
            DM_ACL_CLIENT_GET_ACL((chcb), dm_acl_client_l2cap), \
            (multiplier), \
            (reason))

/* Determine whether L2CA_CHCB_T is for connectionless channel */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
#define CH_IS_CONNECTIONLESS(chcb) ((chcb) == mcb.connectionless)
#else
#define CH_IS_CONNECTIONLESS(chcb) FALSE
#endif

/* Determine whether L2CA_CHCB_T is connected. Connectionless
   channel is alwasy connected. */
#define CH_IS_CONNECTED(chcb) \
    (CH_IS_CONNECTIONLESS(chcb) || dm_acl_is_connected(CH_GET_ACL(chcb)))

/* The outgoing signalling MTU depends on AMP support */
#ifdef INSTALL_AMP_SUPPORT
#define CH_GET_SIGNAL_MTU(chcb) \
    ((chcb)->signal_mtu)
#else
#define CH_GET_SIGNAL_MTU(chcb) \
    (L2CAP_SIGNAL_STD_MTU_MAX)
#endif

/* Get signal channel ID for CHCB (BT/LE) */
#ifdef INSTALL_ULP
#define CH_GET_SIGNAL_CID(chcb) (CH_IS_ULP(chcb) ? L2CA_CID_LE_SIGNAL \
                                                 : L2CA_CID_SIGNAL)
#else
#define CH_GET_SIGNAL_CID(chcb) (L2CA_CID_SIGNAL)
#endif

/* Get AMP-enabled builds to compile */
#ifndef HCI_BR_EDR_PHANDLE
#define HCI_BR_EDR_PHANDLE CSR_SCHED_QID_INVALID
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief linkable L2CA_DATAWRITE_CFMs and L2CA_DATAWRITE_ABORT_CFMs

    These can be sent as L2CA_DATAWRITE_CFMs or L2CA_DATAWRITE_ABORT_CFMs
    and can also be linked together in a list. The list is
    used to store confirms for aborted packets until
    they can be sent to the application. */
typedef struct L2CAP_ABORTED_PACKET_T_tag
{
    union
    {
        L2CA_UPSTREAM_COMMON_T common;
        L2CA_DATAWRITE_ABORT_CFM_T abort_cfm;
        L2CA_DATAWRITE_CFM_T datawrite_cfm;
    } u;
    struct L2CAP_ABORTED_PACKET_T_tag *next;
} L2CAP_ABORTED_PACKET_T;

/*! \brief Common transmit queue element

    This structure serves as the low-level placeholder for data which
    is about to be sent over an ACL. Once the element has been sent, a
    completion function is called (based on the 'type') together with
    pointer to the current CHCB, calldata and context.

    The transmit function will issue a callback _before_ transmitting
    anything in case the MBLK is a NULL-pointer. In this way, the FEC
    and signal handling code can decide for it's own what data to
    send.

    Note that the 'trailersize' and 'trailersent' on-host is used for
    the forged McDSP packets so we need the full 16 bit length
    range. On-chip is can only ever be used for the FCS, hence only 3
    bits.
*/
typedef struct txqetag
{
        struct txqetag                       *next;                 /*!< Next queue element */
        BITFIELD(FRAMETYPE_T,                 type, 4);             /*!< Frame/data type */
        BITFIELD(CsrUint8,                     headersize, 6);       /*!< Real header size in octets */
        BITFIELD(CsrUint16,                    trailersize, 3);      /*!< Trailer size in octets */
        BITFIELD(CsrUint16,                    trailersent, 3);      /*!< Octets of trailer already sent */
        CsrMblk                               *mblk;                 /*!< MBLK chain */
        l2ca_cid_t                            cid;                  /*!< Transmitting cid (local) */
        CsrUint16                              context;              /*!< Datawrite req/cfm context */
        l2ca_timeout_t                        flush;                /*!< Flush timeout */
        CsrMblkSize                           sent;                 /*!< Octets of the MBLK sent so far */
        CsrUint16                              credits;              /*!< Outstanding credits for this element */
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
        L2CAP_ABORTED_PACKET_T               *aborted;              /*!< List of subsequent packets aborted */
#endif
#ifndef INSTALL_L2CAP_CRC
        CsrUint8                               trailer[2];           /*!< Trailing bytes, used for off-chip CRC */
#endif
        CsrUint8                               header[1];            /*!< Extendable frame header */
} TXQE_T;

/* Transmit queue handling */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
extern void CH_CLDataSendQueued(void);
#else
#define CH_CLDataSendQueued() ((void)0)
#endif
extern void CH_DataSendQueued(L2CAP_CHCB_T *chcb, TXQUEUE_T *queue, CsrUint8 priority, CsrBool from_nhcp);
extern void CH_DataAddHeader(TXQE_T *txqe, CsrUint16 cid, CsrUint16 mblksize,
                             CsrUint8 *head, CsrUint8 headsize);
extern CsrBool CH_DataTxBasic(struct cidtag *cidcb, CsrUint16 context, CsrMblk *mblk);
extern void CH_DataTxRaw(L2CA_RAW_DATA_REQ_T *req);
extern void CH_DataTxSignalCallback(L2CAP_CHCB_T *chcb);
#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
extern void CH_DataTxFecCallback(struct cidtag *cidcb);
#endif
extern CsrBool CH_DataTxPending(L2CAP_CHCB_T *chcb);

/* Utility functions */
extern l2ca_conn_result_t CH_Connect(const TYPED_BD_ADDR_T *const addrt, psm_t psm, L2CAP_CHCB_T **pp_chcb, l2ca_conflags_t flags);
extern void CH_ConnectInd(DM_ACL_T *p_acl, CsrUint8 status);
extern CsrUint16 CH_DisconnectInd(DM_ACL_T *p_acl, CsrUint16 reason);
extern void CH_RemoveCidcb(L2CAP_CHCB_T *chcb, struct cidtag *cidcb);
extern void CH_DataRx(L2CAP_CHCB_T *chcb, CsrMblk *mblk, CsrBool bcast);
extern void CH_Reset(L2CAP_CHCB_T *chcb);
extern void CH_Close(L2CAP_CHCB_T *chcb, hci_reason_t reason, ACL_IDLE_TIMEOUT_MULTIPLIER_T timeout);
extern void CH_StandardClose(L2CAP_CHCB_T *chcb);
extern void CH_Exterminate(L2CAP_CHCB_T *chcb);
extern void CH_CompletedPackets(L2CAP_CHCB_T *chcb, CsrUint16 completed_packets);
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
extern void CH_AbortTX(CIDCB_T *cidcb);
extern void CH_EmptyAbortQueue(CIDCB_T *cidcb, L2CAP_ABORTED_PACKET_T **abort_queue, l2ca_data_result_t result);
extern void CH_AbortElement(CIDCB_T *cidcb, CsrUint16 context, L2CAP_ABORTED_PACKET_T ***abort_queue);
extern void CH_AbortCfm(CIDCB_T *cidcb, L2CAP_ABORTED_PACKET_T **abort_queue);
#else
#define CH_EmptyAbortQueue(cidcb, abort_queue, result)
#endif /* INSTALL_L2CAP_DATA_ABORT_SUPPORT */

/* HCI data credit debugging. */
#ifdef L2CAP_HCI_DATA_CREDIT_SLOW_CHECKS
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
CsrUint16 CH_CLUsedDataCredits(void);
#else
#define CH_CLUsedDataCredits() 0
#endif
CsrUint16 CH_UsedDataCredits(L2CAP_CHCB_T *chcb);
#endif

/* Cleanup functions */
extern void CH_FlushTxqe(TXQE_T *txqe, l2ca_data_result_t result);
extern CsrUint16 CH_FlushPendingQueueWithCid(TXQUEUE_T *queue, l2ca_cid_t cid, l2ca_data_result_t result);
extern void CH_FlushCidcbData(L2CAP_CHCB_T *chcb, CIDCB_T *cidcb, l2ca_data_result_t result);
extern CsrUint16 CH_FlushChcb(L2CAP_CHCB_T *chcb, l2ca_data_result_t result);
extern CsrUint16 CH_FlushDoneQueue(TXQUEUE_T *queue, l2ca_data_result_t result);

/* Connectionless support */
#ifdef INSTALL_L2CAP_CONNLESS_SUPPORT
extern void CH_CompletedPacketsCL(CsrUint16 completed_packets);
#else
#define CH_CompletedPacketsCL(x) ((void)0)
#endif

/* AMP extensions */
#ifdef INSTALL_AMP_SUPPORT
extern void CH_AmpPhysicalConnectCfm(L2CAP_CHCB_T *chcb, hci_return_t status, l2ca_cid_t cid,
                                     l2ca_controller_t remote_amp_id);

extern void CH_AmpLogicalConnectCfm(
        L2CAP_CHCB_T *chcb,
        DM_AMPM_CONNECT_CHANNEL_RSP_T *prim);
extern void CH_AmpDisconnectInd(L2CAP_CHCB_T *chcb, l2ca_controller_t local_amp_id, hci_error_t reason);
extern void CH_AmpVerifyPhysicalLinkCfm(L2CAP_CHCB_T *chcb, CsrUint16 identifier, CsrBool exists,
                                        l2ca_fs_flush_t link_supervision_timeout,
                                        l2ca_fs_flush_t best_effort_flush_timeout);
extern void CH_AmpCompletedPackets(const DM_AMPM_NCB_T *const ncb);
extern void CH_AmpRecalculateMPS(l2ca_controller_t local_amp_id);
#endif

/* ACL reassemble - implemented in l2cap_acl_reassembly.c */
#ifdef BUILD_FOR_HOST
extern CsrMblk *L2CA_AclReassemble(hci_connection_handle_t hci_flags, CsrMblk *mblk);
#endif

#ifdef __cplusplus
}
#endif
#endif
