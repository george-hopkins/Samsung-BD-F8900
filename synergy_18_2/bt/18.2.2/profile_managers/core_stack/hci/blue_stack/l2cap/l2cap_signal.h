/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_signal.c

\brief  This file defines the functions to create L2CAP signals.
*/

#ifndef _L2CAP_SIGNAL_H    /* Once is enough */
#define _L2CAP_SIGNAL_H

#include <stdarg.h>
#include "csr_bt_common.h"
#include "l2cap_prim.h"
#include "l2cap_common.h"
#include "l2cap_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! Signal Table

    First goes the signal name, then the minimum length of the
    non-common signal data (i.e. length without the standard
    code/identifier/length header.

    The ordering is important and defines the signal opcode!
*/

#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
#define OLD_SIGNAL_TABLE \
    SIGNAL_TABLE_X(SIGNAL_INVALID,            0x0000) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_COMMAND_REJECT,     0x0002) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_CONNECT_REQ,        0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_CONNECT_RES,        0x0008) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_CONFIG_REQ,         0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_CONFIG_RES,         0x0006) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_DISCONNECT_REQ,     0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_DISCONNECT_RES,     0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_ECHO_REQ,           0x0000) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_ECHO_RES,           0x0000) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_INFO_REQ,           0x0002) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_INFO_RES,           0x0004)
#else
#define OLD_SIGNAL_TABLE \
    SIGNAL_TABLE_X(SIGNAL_INVALID,            0x0000) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_COMMAND_REJECT,     0x0002)
#endif

/*! AMP specific signal. Follows the same structure */
#ifdef INSTALL_AMP_SUPPORT
#define AMP_SIGNAL_TABLE \
    SIGNAL_TABLE_X(SIGNAL_CREATE_CHANNEL_REQ, 0x0005) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_CREATE_CHANNEL_RES, 0x0008) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_MOVE_CHANNEL_REQ,   0x0003) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_MOVE_CHANNEL_RES,   0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_MOVE_CONFIRM_REQ,   0x0004) SIGNAL_TABLE_SEP \
    SIGNAL_TABLE_X(SIGNAL_MOVE_CONFIRM_RES,   0x0002)
#define SIGNAL_TABLE OLD_SIGNAL_TABLE SIGNAL_TABLE_SEP AMP_SIGNAL_TABLE
#else
#define SIGNAL_TABLE OLD_SIGNAL_TABLE
#endif

/* For those signals that follow request/response format (i.e. all of them
   save for SIGNAL_COMMAND_REJECT), you can get the request type by
   subtracting one from the response type. If this ever ceases to be the
   case then this must be changed accordingly. */
#define SIGNAL_REQUEST_FROM_RESPONSE(response) ((response) - 1)

/*! \brief Signal types

    This enum lists all the possible signal types that are used in L2CAP.
*/
typedef enum signal_type
{
#define SIGNAL_TABLE_X(a, b) a
#define SIGNAL_TABLE_SEP ,
    SIGNAL_TABLE
#undef SIGNAL_TABLE_X
#undef SIGNAL_TABLE_SEP
#ifdef INSTALL_ULP
    ,SIGNAL_CONN_PARAM_UPDATE_REQ = 0x12
    ,SIGNAL_CONN_PARAM_UPDATE_RES = 0x13
#endif
    ,SIGNAL_UNKNOWN
} SIGNAL_T;

/*! \brief Signal minimum lengths

    This enum lists the minimum lengths of all BR/EDR/AMP L2CAP signals. */
typedef enum
{
#define SIGNAL_TABLE_X(a, b) a ## _MIN_LENGTH = b
#define SIGNAL_TABLE_SEP ,
    SIGNAL_TABLE
#undef SIGNAL_TABLE_X
#undef SIGNAL_TABLE_SEP
#ifdef INSTALL_ULP
    ,SIGNAL_CONN_PARAM_UPDATE_REQ_MIN_LENGTH = 0x0008
    ,SIGNAL_CONN_PARAM_UPDATE_RES_MIN_LENGTH = 0x0002
#endif
} L2CAP_SIGNAL_MIN_LENGTH_T;

/*! \brief Signal Control Block.

    The signal control block holds the parameters and data needed for
    L2CAP signal transmission and re-transmission.  Signals can be
    stored on a queue pending tranmission or pending ackowledgment.
    Each signal has an independent retranmission (ERTX or RTX) timer.
*/
typedef struct sig_signal_tag
{
    struct sig_signal_tag                *next_ptr;             /*!< Pointer tonext signal */

    BITFIELD(CsrUint8,                     signal_type, 5);      /*!< Signal type */
    BITFIELD(CsrUint8,                     signal_id, 8);        /*!< Unique signal ID */
#if (L2CAP_RTX_RETRIES < 2) && (L2CAP_ERTX_RETRIES < 2)
    BITFIELD(CsrUint8,                     rtx_timer_count, 1);  /*!< Number of RTX retries left */
    BITFIELD(CsrUint8,                     ertx_timer_count, 1); /*!< Number of ERTX retries left */
#else
    CsrUint8                               rtx_timer_count;      /*!< Number of RTX retries left */
    CsrUint8                               ertx_timer_count;     /*!< Number of ERTX retries left */
#endif
    CsrMblk                               *signal;               /*!< Pointer tosignal payload in MBLK */
    CsrSchedTid                                 timer_id;             /*!< ERTX/RTX Timer ID */
    l2ca_cid_t                            local_cid;            /*!< Local CID for this signal */
    CsrSchedQid                             p_handle;             /*!< P-Handle for this signal, used for ECHO/GET_INFO signals */
} SIG_SIGNAL_T;

extern SIG_SIGNAL_T *SIG_SignalCreate(l2ca_cid_t local_cid,
                                      CsrUint16 signal_size,
                                      CsrUint8 signal_type,
                                      l2ca_identifier_t signal_id,
                                      unsigned int count,
                                      ...);
#ifndef DISABLE_L2CAP_CONNECTION_FSM_SUPPORT
extern SIG_SIGNAL_T *SIG_SignalCreateConnectReq(l2ca_cid_t local_cid, psm_t psm);
extern SIG_SIGNAL_T *SIG_SignalCreateConnectRsp(l2ca_cid_t dest_cid, l2ca_cid_t src_cid, CsrUint16 result,
                                                CsrUint16 status, l2ca_identifier_t identifier);
extern SIG_SIGNAL_T *SIG_SignalCreateConfigSignal(struct cidtag *cidcb,
                                                  CsrUint16 length,
                                                  const CsrUint8 *config,
                                                  CsrBool more,
                                                  l2ca_identifier_t identifier,
                                                  l2ca_conf_result_t result);
extern SIG_SIGNAL_T *SIG_SignalCreateDisconnectReq(l2ca_cid_t remote_cid, l2ca_cid_t local_cid);
extern SIG_SIGNAL_T *SIG_SignalCreateDisconnectRsp(l2ca_cid_t dest_cid, l2ca_cid_t src_cid,
                                                   l2ca_identifier_t identifier);

extern SIG_SIGNAL_T *SIG_SignalCreateGetInfoReq(CsrSchedQid p_handle, CsrUint16 info_type, CsrUint16 req_ctx);
extern SIG_SIGNAL_T *SIG_SignalCreateEchoReq(CsrSchedQid p_handle, const CsrUint8 *data, CsrUint16 length, CsrUint16 req_ctx);
extern SIG_SIGNAL_T *SIG_SignalCreateEchoRsp(l2ca_identifier_t identifier, const CsrUint8 *data, CsrUint16 length);
extern SIG_SIGNAL_T *SIG_SignalCreateGetInfoRsp(l2ca_identifier_t identifier, CsrUint16 info_type);

extern CsrBool SIG_ConfigParallelise(const CsrUint8 **buf, CsrUint16 *size, L2CA_CONFIG_T *conf);
extern CsrBool SIG_ConfigSerialise(CsrUint8 **buf, L2CA_CONFIG_T *config);
extern CsrUint16 SIG_ConfigOptionsLength(L2CA_CONFIG_T *config);
#endif

extern CsrUint16 SIG_FlushToH2L(CsrUint16 fto);
extern CsrUint16 SIG_FlushToL2H(CsrUint16 fto);
extern CsrUint16 SIG_ReadGetInfoType(const SIG_SIGNAL_T *sig_ptr);

/* Flowspec support for BR/EDR */
#ifdef INSTALL_L2CAP_FLOWSPEC_SUPPORT
extern CsrUint16 SIG_FlushToFS2H(CsrUint32 fto);
#endif

/* AMP extensions */
#ifdef INSTALL_AMP_SUPPORT
extern SIG_SIGNAL_T *SIG_SignalCreateChannelReq(l2ca_cid_t local_cid, psm_t psm, l2ca_controller_t control);
extern SIG_SIGNAL_T *SIG_SignalCreateChannelRsp(l2ca_cid_t dest_cid, l2ca_cid_t src_cid,
                                                CsrUint16 result, CsrUint16 status, l2ca_identifier_t identifier);
extern SIG_SIGNAL_T *SIG_SignalMoveChannelReq(l2ca_cid_t local_cid, l2ca_controller_t control);
extern SIG_SIGNAL_T *SIG_SignalMoveChannelRsp(l2ca_cid_t local_cid, l2ca_cid_t src_cid, CsrUint16 result, l2ca_identifier_t identifier);
extern SIG_SIGNAL_T *SIG_SignalMoveChannelConfirmReq(l2ca_cid_t local_cid, CsrUint16 result);
extern SIG_SIGNAL_T *SIG_SignalMoveChannelConfirmRsp(l2ca_cid_t local_cid, l2ca_cid_t src_cid, l2ca_identifier_t identifier);
#endif

/* Low energy extensions */
#ifdef INSTALL_ULP
extern SIG_SIGNAL_T *SIG_SignalConnParamUpdateReq(CsrUint16 min_interval, CsrUint16 max_interval,
                                                  CsrUint16 latency, CsrUint16 timeout);
extern SIG_SIGNAL_T *SIG_SignalConnParamUpdateRsp(l2ca_identifier_t identifier, CsrUint16 status);
#endif

#ifdef __cplusplus
}
#endif
#endif

