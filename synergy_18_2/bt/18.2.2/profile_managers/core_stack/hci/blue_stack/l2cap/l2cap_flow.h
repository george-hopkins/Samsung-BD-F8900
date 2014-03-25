/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_flow.h

\brief  Definitions need for the L2CAP retransmission and flow control
        implementation.
*/

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#ifndef _L2CAP_FLOW_H_
#define _L2CAP_FLOW_H_

#include "csr_synergy.h"

#include "l2cap_common.h"
#include "l2cap_config.h"
#include "l2cap_con_handle.h"
#include "l2cap_cid.h"
#include "csr_mblk.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! F&EC state/exception level */
typedef enum
{
    FST_CLEAR = 0,      /*!< Nothing pending (can be overridden) */
    FST_SREJ_DATA,      /*!< S-REJ list emptied, so data transfers can continue */
    FST_REJ_DATA,       /*!< REJ sent, so data transfers can continue */
    FST_WAIT_FINAL,     /*!< Poll has been sent, waiting for final but can also send data */
    FST_ACK,            /*!< Ack should be sent (can be overridden) */
    /* Above exceptions may use I-frames */
    FST_SREJ_LIST,      /*!< S-REJ list request in progress */
    FST_FORCE_ACK,      /*!< Force an ack through without possibility to send data */
    FST_WAIT_ACK,       /*!< Send poll frame in RR/RNR due to RTM-timeout/busy-cleared */
    FST_BUSY_CLEARED,   /*!< Local busy cleared, poll shall be sent */
    FST_REJ             /*!< REJ in progress */
} FLOWSTATE_T;

/*! Choke levels needed for AMP move channel */
typedef enum
{
    FCOK_OFF = 0,        /*!< Release/choke mode off */
    FCOK_ON,             /*!< Full choke on (except for RR-polls) */
    FCOK_AUTO_RR,        /*!< Choke mode with automatic receive-RR-poll-release */
    FCOK_CONF_START      /*!< Re-configuration started, stop everything */
} FLOWCHOKE_T;

/*! Magic values for the out-of-sequence buffer */
#define CONTEXT_MISSING    0x01 /*!< Out-of-sequence marker for missing frame */
#define CONTEXT_SENT       0x02 /*!< Out-os-sequence marker S-REJ sent */
#define CONTEXT_OK         0x04 /*!< Out-of-sequence marker for received frame */

/*! Special flags for deferred timer starts */
typedef enum
{
    DEFER_NOW = 0,              /*!< Do not defer at all but start timer immediately */
    DEFER_WAIT,                 /*!< Wait for NCPs to arrive */
    DEFER_PENDING               /*!< Start timer if pending credits - retransmission timer only */
} FLOWDEFER_T;

/*! Poll, final, don't care */
typedef enum
{
    PF_NONE = 0,                /*!< No poll/final bit set */
    PF_POLL,                    /*!< Poll bit is set */
    PF_FINAL                    /*!< Final bit is set */
} FLOWPF_T;

/*! Transmit sequence number validatation results */
typedef enum
{
    TXV_VALID,                  /*!< Valid (in sequence or not) */
    TXV_DUPLICATE,              /*!< Duplicate tx_seq */
    TXV_INVALID                 /*!< Invalid */
} FLOWTXVALID_T;

/*! Placeholder for in/out going flow control packet info */
typedef struct FLOWPKT_T_tag
{
    struct FLOWPKT_T_tag    *next;
    CsrMblk                  *mblk;
    CsrUint16                 context;
    CsrUint16                 sdu_length;
    BITFIELD(CsrUint8,        sdu_mode, 2);
    BITFIELD(FLOWSEQ_T,      seq, L2CAP_FLOW_SEQ_BITS);
    CsrUint8                  retrans;
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
    L2CAP_ABORTED_PACKET_T  *aborted;   /*!< List of subsequent packets aborted */
#endif
} FLOWPKT_T;

/*! Generic flow control instance data */
typedef struct FLOWINFO_T_tag
{
#ifdef INSTALL_L2CAP_DEBUG
    CsrUint16                 guard;                                   /*!< Debugging guard */
#endif

    struct                                                            /*!< Placeholder for the actual configuration */
    {
        BITFIELD(FLOWSEQ_T,              remote_window, L2CAP_FLOW_SEQ_BITS); /*!< Remote window size */
        BITFIELD(FLOWSEQ_T,              local_window, L2CAP_FLOW_SEQ_BITS);  /*!< Local window size */
        BITFIELD(l2ca_flow_mode_t,       mode, 4);                    /*!< Current mode */
        BITFIELD(CsrBool,                 fcs, 1);                     /*!< Use FCS */
        BITFIELD(CsrBool,                 ext_window, 1);              /*!< Use extended window sizes */
        BITFIELD(l2ca_max_retransmit_t,  max_retransmit, 8);          /*!< Local maximum transmit count */
        l2ca_mtu_t                       remote_mps;                  /*!< Remote maximum payload size */
        l2ca_mtu_t                       local_mps;                   /*!< Remote maximum payload size */
        l2ca_mtu_t                       effective_mps;               /*!< Effective outgoing MPS - clipped by actual baseband packet size */
        CsrTime                             monitor_timeout;             /*!< Local monitor timeout in ms */
        CsrTime                             retrans_timeout;             /*!< Local retransmission timeout in ms */
        CsrTime                             ack_delay;                   /*!< Local delayed ack timeout in ms */
    } config;

    CsrUint16                 credits_seq;                             /*!< Number of upper layer pending requests */

    FLOWPKT_T               *tx_head;                                 /*!< Transmit PDU queue head element */
    FLOWPKT_T               *tx_tail;                                 /*!< Transmit PDU queue tail element */
    FLOWPKT_T               *re_head;                                 /*!< Unacknowledged/retransmission backup buffer */
    FLOWPKT_T               *rt_head;                                 /*!< Retransmission priority buffer */
    FLOWPKT_T               *oos_head;                                /*!< Out-of-Sequence receive-buffer for ERTM mode */

    L2CA_DATAREAD_IND_T     *l2ca_dataread_ind;                       /*!< Data primitive being reassembled */
    CsrUint16                 sar_offset;                              /*!< SDU segmentation offset for transmissions */
#ifdef BUILD_FOR_HOST
    CsrUint16                 sar_count;                               /*!< Number of MBLKs in SAR reassembly chain */
#endif

#ifdef INSTALL_AMP_SUPPORT
    CsrUint16                 move_sar_offset;                         /*!< Move channel resegmentation offset */
#endif

    FLOWSTATE_T              state;                                   /*!< Exception state (see FST_ defines) */
    CsrSchedTid                    retrans_timer;                           /*!< Retransmission timer id */
    CsrSchedTid                    monitor_timer;                           /*!< Monitor timer id */
    CsrSchedTid                    ack_timer;                               /*!< Acknowledgment delay timer id */

    BITFIELD(FLOWSEQ_T,      next_tx_seq, L2CAP_FLOW_SEQ_BITS);       /*!< TX: Sequence number of next I frame to be transmitted */
    BITFIELD(FLOWSEQ_T,      expected_ack_seq, L2CAP_FLOW_SEQ_BITS);  /*!< TX: Sequence number of the next frame expected to be acked */
    BITFIELD(CsrBool,         rnr_sent, 1);                            /*!< RNR has been sent due to local busy condition */
    BITFIELD(CsrBool,         rej_actioned, 1);                        /*!< REJ/poll-final crossover detection */
    BITFIELD(CsrBool,         remote_poll, 1);                         /*!< Peer has polled us */

    CsrUint8                  local_poll_retrans;                      /*!< Transmission counter for S-poll-frames */

    BITFIELD(FLOWSEQ_T,      expected_tx_seq, L2CAP_FLOW_SEQ_BITS);   /*!< RX: Expected tx_seq in I-frame */
    BITFIELD(FLOWSEQ_T,      highest_tx_seq, L2CAP_FLOW_SEQ_BITS);    /*!< RX: Highest tx_seq seen - used for SREJ loss and duplicate detection */
    BITFIELD(FLOWCHOKE_T,    choke, 3);                               /*!< Choke transmission (needed for AMP moves) */
    BITFIELD(CsrBool,         srej_actioned, 1);                       /*!< S-REJ/poll-final crossover detection */

    BITFIELD(FLOWSEQ_T,      req_seq_next, L2CAP_FLOW_SEQ_BITS);      /*!< RX: Next outgoing req_seq*/
    BITFIELD(FLOWSEQ_T,      req_seq_last, L2CAP_FLOW_SEQ_BITS);      /*!< RX: Lastest sent req_seq */
    BITFIELD(CsrBool,         remote_busy, 1);                         /*!< Peer is busy */
    BITFIELD(CsrBool,         local_busy, 1);                          /*!< Local busy condition */
    BITFIELD(CsrBool,         scheduled, 1);                           /*!< Presend function has been scheduled in con_handle */
    BITFIELD(CsrBool,         defer_monitor, 1);                       /*!< Deferred start of monitor timer when NCP arrive */

    BITFIELD(FLOWSEQ_T,      defer_retrans, L2CAP_FLOW_SEQ_BITS);     /*!< Deferred retrans timer start - counts number of pending NCPs */
    BITFIELD(FLOWSEQ_T,      srej_save_seq, L2CAP_FLOW_SEQ_BITS);     /*!< Duplicate SREJ detection sequence */

#ifdef INSTALL_AMP_SUPPORT
    BITFIELD(CsrBool,         move_channel, 1);                        /*!< Move channel has been started */
#endif

#ifdef INSTALL_L2CAP_DEBUG
    BITFIELD(CsrBool,         drop_active_rx, 1);                      /*!< Rx drop mode */
    BITFIELD(CsrBool,         drop_active_tx, 1);                      /*!< Tx drop mode */
    CsrUint16                 drop_rx_interval;                        /*!< Run Rx dropping this often */
    CsrUint16                 drop_rx_number;                          /*!< Rx drop this many packets when on */
    CsrUint16                 drop_tx_interval;                        /*!< Run Tx dropping this often */
    CsrUint16                 drop_tx_number;                          /*!< Tx drop this many packets when on */
    CsrUint16                 drop_cnt_rx;                             /*!< Current Rx packet count */
    CsrUint16                 drop_cnt_tx;                             /*!< Current Tx packet count */
#endif

} FLOWINFO_T;

/* Transmitting, receiving and acknowledging data */
extern void FLOW_DataWrite(CIDCB_T *cidcb, CsrMblk *mblk, CsrUint16 context);
extern void FLOW_DataWriteComplete(CIDCB_T *cidcb, TXQE_T *txqe, l2ca_data_result_t result);
extern void FLOW_BusyReq(CIDCB_T *cidcb, CsrBool busy);
extern void FLOW_DataRead(CIDCB_T *cidcb,CsrMblk *mblk, CsrUint16 payload_size, CsrUint8 *header);
extern void FLOW_DataReadAck(CIDCB_T *cidcb, FLOWINFO_T *flow, FLOWSEQ_T inc);
extern void FLOW_DataPresend(TXQE_T *txqe);
extern l2ca_disc_result_t FLOW_Setup(CIDCB_T *cidcb, L2CA_CONFIG_T *local, L2CA_CONFIG_T *remote);
extern void FLOW_Free(CIDCB_T *cidcb, l2ca_data_result_t result);
extern void FLOW_FixConfig(CIDCB_T *cidcb, L2CA_CONFIG_T *config, CsrBool from_air);
extern void FLOW_Choke(CIDCB_T *cidcb, FLOWCHOKE_T choke);
#ifdef INSTALL_AMP_SUPPORT
extern void FLOW_ClipMPS(CIDCB_T *cidcb);
#endif
extern void FLOW_FlushQueue(CIDCB_T *cidcb, FLOWINFO_T *flow, FLOWPKT_T **head, FLOWSEQ_T credits, l2ca_data_result_t result);
#ifdef INSTALL_L2CAP_DATA_ABORT_SUPPORT
extern FLOWPKT_T *FLOW_FindLastTransmitted(FLOWINFO_T *flow);
extern void FLOW_AbortTX(CIDCB_T *cidcb, FLOWINFO_T *flow);
#endif

/* Debug interface */
#ifdef INSTALL_L2CAP_DEBUG
extern void FLOW_DebugDrop(CIDCB_T *cidcb, CsrUint16 rx_i, CsrUint16 rx_n, CsrUint16 tx_i, CsrUint16 tx_n);
#endif

/* On-chip low memory contigency handling */
#ifndef BUILD_FOR_HOST
extern void FLOW_LowMemory(CIDCB_T *cidcb, FLOWINFO_T *flow);
#endif

/* Special AMP helper functions */
#ifdef INSTALL_AMP_SUPPORT
extern void FLOW_AmpMovePrepare(CIDCB_T *cidcb);
extern void FLOW_AmpMoveResume(CIDCB_T *cidcb, FLOWCHOKE_T choke);
#endif

/* On-chip L2CAP FCS */
#if defined(INSTALL_L2CAP_CRC) && !defined(BUILD_FOR_HOST)
CsrBool FLOW_LcSetL2capCrc(CIDCB_T *cidcb, CsrBool use_fcs);
#define FLOW_SetLcFcs(cidcb, use_fcs) \
    (FLOW_LcSetL2capCrc((cidcb), (use_fcs)))
#else
#define FLOW_SetLcFcs(cidcb, use_fcs) (TRUE)
#endif

#ifdef __cplusplus
}
#endif
#endif
#endif

