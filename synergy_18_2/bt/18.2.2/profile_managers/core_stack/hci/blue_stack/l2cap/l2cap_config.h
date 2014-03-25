/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_config.h

\brief  This file defines the L2CAP configuration.
*/

#ifndef _L2CAP_CONFIG_H
#define _L2CAP_CONFIG_H

#include "csr_synergy.h"

#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BUILD_FOR_HOST
#define INSTALL_L2CAP_DATA_ABORT_SUPPORT
#endif

/* Maximum number of concurrent channels we support. The hard limit is
 * currently 0x3f (=63) but can be raised if the CID_RANDOM_LSH is
 * adjusted appropiately */
#ifdef BUILD_FOR_HOST
#define L2CAP_MAX_NUM_CIDS        (32)
#else
#define L2CAP_MAX_NUM_CIDS        (12)
#endif

/* Maximum MTU: We need to allow for a few extra bytes for the
 * extended L2CAP headers */
#define L2CAP_MTU_MAXIMUM         (0xFFF0)

/* Left-shift random number: 1<<6 = 0x40. This controls how many bits
 * the random part of local CIDs can take up. Do NOT change this
 * unless you know what you're doing! */
#define L2CAP_CID_RANDOM_LSH      (6)

/* Reserve top bit of L2CA_REGISTER_REQ flags to turn off auto_connect
   for a channel. */
#define L2CA_REGISTER_NOAUTO 0x8000

/* Signalling MTUs */
#define L2CAP_SIGNAL_INCOMING_MTU_MAX  (672)    /*!< The MTU we accept on incoming signals */
#define L2CAP_SIGNAL_STD_MTU_MAX       (48)     /*!< Outgoing signalling MTU defined in spec up to 2.1 */
#define L2CAP_SIGNAL_EXT_MTU_MAX       (672)    /*!< Outgoing signalling MTU for 3.0+ devices */
#define L2CAP_SIGNAL_BLE_MTU_MAX       (23)     /*!< Low energy MTU is fixed at 23 octets */

/*! \brief Lengths used for checking against ACL packets */
#define L2CAP_ACL_MTU_MAX              (L2CAP_SIZEOF_MAX_HEADER - L2CAP_SIZEOF_CID_HEADER + L2CAP_SIZEOF_FCS_FIELD)
#define L2CAP_ACL_RAW_LENGTH_MAX       (L2CAP_SIZEOF_MAX_HEADER + L2CAP_SIZEOF_FCS_FIELD)

/* Number of RTX and ERTX retries */
#define L2CAP_RTX_RETRIES         (1)
#define L2CAP_ERTX_RETRIES        (1)

/* The RTX, ERTX and configuration timeouts */
#define CSR_BT_L2CAP_RTX_TIMEOUT         (10 * CSR_SCHED_SECOND)       /*!< Must adhere to: timeout*(2^(retry+1) + 1) <= 60 */
#define CSR_BT_L2CAP_ERTX_TIMEOUT        (60 * CSR_SCHED_SECOND)       /*!< Must adhere to: timeout*(2^(retry+1) + 1) <= 300 */
#define L2CAP_CONFIG_TIMEOUT      (120 * CSR_SCHED_SECOND)      /*!< Configuration must never take more than 2 minutes */

/* Minimum retrans/monitor timeours for inifinite flush timeout channels */
#define L2CAP_NFTO_TIMEOUT_RETRANS (0x07D0)           /*!< 2s:  Minumum value for retransmission timer with NO (inf) flush */
#define L2CAP_NFTO_TIMEOUT_MONITOR (0x2EE0)           /*!< 12s: Minumum value for monitor timer with NO (inf) flush */
#define L2CAP_FFTO_TIMEOUT_RETRANS (0x03E8)           /*!< 1s:  Minimum value for retransmit timer with finite flush */
#define L2CAP_FFTO_TIMEOUT_MONITOR (0x2EE0)           /*!< 12s: Minimum value for monitor timer with finite flush */
#define L2CAP_AC2F_TIMEOUT_RETRANS (0x01F4)           /*!< 500ms: Minimum retrans timeout for class 2 AMPs */
#define L2CAP_AC2F_TIMEOUT_MONITOR (0x01F4)           /*!< 500ms: Minimum monitor timeout class 2 AMPs */

/* Flow control parameters */
#define L2CAP_FLOW_MAX_ACK_DELAY  (500 * CSR_SCHED_MILLISECOND) /*!< Max time to delay RR/RNR acks */
#define L2CAP_FLOW_MIN_ACK_DELAY  (10 * CSR_SCHED_MILLISECOND)  /*!< Min time to delay RR/RNR acks */
#define L2CAP_FLOW_IMM_ACK_MINWIN (2)                 /*!< If local window is < value, don't use delayed acks */
#define L2CAP_FLOW_IMM_ACK_SHIFT  (2)                 /*!< If ack-level < (ack-level >> shift), send ack immediately */
#define L2CAP_PKT_MASS_LOSS_THRES (10)                /*!< Missing frame detection method normal/many threshold */
#define L2CAP_FLOW_REJ_THRESHOLD  (2)                 /*!< Shift-fraction (>>) of window that will trigger REJ */

/* Clip ECHO_RES replies */
#define L2CAP_ECHO_RESPONSE_MAX_DATA_SIZE   (16)

/* Connectionless MTU */
#if !defined(L2CAP_CL_MTU)
/* Define the MTU for connectionless traffic. This excludes the PSM
 * field of a connectionless message */
#define L2CAP_CL_MTU              (670)
#endif

/* The special HCI handle used for broadcasts - we just need to make
 * sure we're out of the normal HCI assignment range :-D */
#define L2CAP_BCAST_HCI_HANDLE    (0x0BBC)

/* Reassembly coalesce limit. Once we've reached this many MBLKs in
 * the reassembly-chain, we'll coalesce the MBLK into a single
 * block */
#define L2CAP_SAR_COALESCE_LIMIT                 (20)

/* Auto-connect retry counter - maximum number of requests per conftab block */
#define L2CAP_AUTOCONNECT_RETRY                  (5)

/* Auto-connect min/max default boundaries */
#define L2CAP_DEFAUTO_MTU_MIN                    (L2CA_MTU_MINIMUM)
#define L2CAP_DEFAUTO_MTU_MAX                    (L2CA_MTU_DEFAULT) /* may be clipped on-chip */
#define L2CAP_DEFAUTO_FLOWSPEC_MAX_SDU_MIN       (0)
#define L2CAP_DEFAUTO_FLOWSPEC_MAX_SDU_MAX       (L2CAP_DEFAUTO_MTU_MAX)
#define L2CAP_DEFAUTO_FLOWSPEC_INTARR_MIN        (0)
#define L2CAP_DEFAUTO_FLOWSPEC_INTARR_MAX        (L2CA_FLOWSPEC_INTERAR_DEFAULT)
#define L2CAP_DEFAUTO_FLOWSPEC_LATENCY_MIN       (0)
#define L2CAP_DEFAUTO_FLOWSPEC_LATENCY_MAX       (L2CA_FLOWSPEC_ACCESS_DEFAULT)
#define L2CAP_DEFAUTO_FLOWSPEC_FLUSH_TO_MIN      (L2CA_FLOWSPEC_FLUSH_TO_IMMEDIATE)
#define L2CAP_DEFAUTO_FLOWSPEC_FLUSH_TO_MAX      (L2CA_FLOWSPEC_FLUSH_TO_INFINITE)
#define L2CAP_DEFAUTO_QOS_TOKEN_RATE_MIN         (0)
#define L2CAP_DEFAUTO_QOS_TOKEN_RATE_MAX         (L2CA_QOS_DEFAULT_TOKEN_RATE)
#define L2CAP_DEFAUTO_QOS_TOKEN_BUCKET_MIN       (0)
#define L2CAP_DEFAUTO_QOS_TOKEN_BUCKET_MAX       (L2CA_QOS_DEFAULT_TOKEN_BUCKET)
#define L2CAP_DEFAUTO_QOS_PEAK_BW_MIN            (0)
#define L2CAP_DEFAUTO_QOS_PEAK_BW_MAX            (L2CA_QOS_DEFAULT_PEAK_BW)
#define L2CAP_DEFAUTO_QOS_LATENCY_MIN            (0)
#define L2CAP_DEFAUTO_QOS_LATENCY_MAX            (L2CA_QOS_DEFAULT_LATENCY)
#define L2CAP_DEFAUTO_QOS_DELAY_VAR_MIN          (0)
#define L2CAP_DEFAUTO_QOS_DELAY_VAR_MAX          (L2CA_QOS_DEFAULT_DELAY_VAR)

/* Auto-connect non-negotiated default values */
#define L2CAP_DEFAUTO_FCS_DEFAULT                (0)
#define L2CAP_DEFAUTO_FLUSH_DEFAULT              (L2CA_FLUSH_TO_INFINITE)

/* Auto-connect F&EC paramters where in/out are different */
#define L2CAP_DEFAUTO_FLOW_TX_WINDOW_IN_MIN      (1)
#ifdef BUILD_FOR_HOST
#define L2CAP_DEFAUTO_FLOW_TX_WINDOW_IN_MAX      (CSR_BT_RFCOMM_WINSIZE)
#else
#define L2CAP_DEFAUTO_FLOW_TX_WINDOW_IN_MAX      (5)
#endif
#define L2CAP_DEFAUTO_FLOW_TX_WINDOW_OUT_MIN     (1)
#define L2CAP_DEFAUTO_FLOW_TX_WINDOW_OUT_MAX     (63)
#define L2CAP_DEFAUTO_FLOW_MAX_RETX_IN_MIN       (0)
#define L2CAP_DEFAUTO_FLOW_MAX_RETX_IN_MAX       (255)
#define L2CAP_DEFAUTO_FLOW_MAX_RETX_OUT_MIN      (0)
#define L2CAP_DEFAUTO_FLOW_MAX_RETX_OUT_MAX      (10)
#define L2CAP_DEFAUTO_FLOW_MPS_IN_MIN            (L2CA_MTU_MINIMUM)
#define L2CAP_DEFAUTO_FLOW_MPS_IN_MAX            (0x1000) /* may be clipped on-chip */
#define L2CAP_DEFAUTO_FLOW_MPS_OUT_MIN           (1)
#define L2CAP_DEFAUTO_FLOW_MPS_OUT_MAX           (0xFFFF)

#ifdef BUILD_FOR_HOST
#define L2CAP_MAX_TX_QUEUES                      (3)
#else
#define L2CAP_MAX_TX_QUEUES                      (1)
#endif

/* Extended features */
#define L2CA_EXT_FEAT_SUPPORTED_BIDIR_QOS       0
#define L2CA_EXT_FEAT_SUPPORTED_EXT_FLOW_SPEC   0

#ifdef INSTALL_AMP_SUPPORT
#define L2CA_EXT_FEAT_SUPPORTED_EXT_WIN_SIZE    L2CA_EXT_FEAT_EXT_WIN_SIZE
#else
#define L2CA_EXT_FEAT_SUPPORTED_EXT_WIN_SIZE    0
#endif

#ifdef INSTALL_L2CAP_FIXED_CHANNEL_SUPPORT
#define L2CA_EXT_FEAT_SUPPORTED_FIXED_CH        L2CA_EXT_FEAT_FIXED_CH
#else
#define L2CA_EXT_FEAT_SUPPORTED_FIXED_CH        0
#endif

#ifdef __cplusplus
}
#endif

#endif
