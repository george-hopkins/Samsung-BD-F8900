/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_private.h
\brief  Private header for use only within the RFCOMM module.
*/

#ifndef _RFCOMM_PRIVATE_H_
#define _RFCOMM_PRIVATE_H_

#include "csr_synergy.h"

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "csr_bt_common.h"
#include "error.h"
#include "csr_bt_core_stack_pmalloc.h"
#include "csr_mblk.h"
#include "csr_sched.h"
#include "tbdaddr.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_core_stack_pmalloc.h"
#endif
#include "csr_util.h"

#include "hci_prim.h"
#include "csr_bt_core_stack_fsm.h"
#include "bkeyval.h"
#include "l2cap_prim.h"
#include "l2cap_common.h"
#include "l2caplib.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "rfcomm_prim.h"
#include "rfcomm.h"
#include "rfcomm_channel_fsm_definitions.h"
#include "rfcomm_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RFCOMM_MULTI_THREAD

#define RFC_CTRL_INIT(t)   *(t) = (void *)zpnew(RFC_CTRL_T)
#define STATIC_RFC_CTRL
#define EXTERN_RFC_CTRL
#else

#define RFC_CTRL_INIT(t)   *(t) = (void *)&rfc_ctrl_allocation
#define STATIC_RFC_CTRL  \
RFC_CTRL_T  rfc_ctrl_allocation;
#define EXTERN_RFC_CTRL \
extern STATIC_RFC_CTRL
#endif

#define RFC_L2CAP_REQ_CONFTAB_SIZE 13
#define RFC_L2CAP_RSP_CONFTAB_SIZE 9

/* Frame types set in the Control octet (2nd octet) of an Rfcomm frame. Bit 1 =
   LSB. These can also be used to identify the frames rather than having a
   separate enumeration.
*/
typedef CsrUint8  RFC_FRAME_TYPE_T;

#define RFC_SABM        0x3F     /* P bit expected to be 1 */
#define RFC_UA          0x73     /* F bit expected to be 1 */
#define RFC_DM          0x1F     /* F bit can be either */
#define RFC_DM_PF       0x0F
#define RFC_DISC        0x53     /* PF bit should be 1 */

/* In UIH frames P/F bit should be 0 for non credit based flow control and one
   for credit based flow control.
*/
#define RFC_UIH         0xEF    /* No credit based flow control */
#define RFC_UIH_PF      0xFF    /* Credit based flow control */


/* All multiplexor control commands and responses are sent within UIH frames on
   dlci 0 and have their own sub types within the information field.
   Within the type field, the EA bit is always set to 1 , the C/R bit to 1 if a
   command and the C/R bit set to 0 if a response.
*/

#define RFC_RAW              0x00

/* Command/Response types , without C/R bit set
*/
#define RFC_PN_TYPE      0x81      /* Parameter Negotiation */
#define RFC_TEST_TYPE    0x21      /* Test */
#define RFC_FCON_TYPE    0xA1      /* FCon  */
#define RFC_FCOFF_TYPE   0x61      /* FCoff */
#define RFC_MSC_TYPE     0xE1      /* Modem Status Command */
#define RFC_RPN_TYPE     0x91      /* Port Negotiation */
#define RFC_RLS_TYPE     0x51      /* Line Status */
#define RFC_NSC_TYPE     0x11      /* Non Supported Command */

/* Used to determine how to calculate the CR bit in frames.
*/
typedef enum
{
    RFC_INITIATOR,
    RFC_RESPONDER

} RFC_LINK_DESIGNATOR;

typedef enum
{
    RFC_CMD,
    RFC_RSP,
    RFC_DATA

} RFC_CRTYPE;

typedef enum
{
    RFC_FRAMELEVEL,
    RFC_MSGLEVEL

} RFC_CRLEVEL;

/* Connection Flags manipulation.
*/
#define RFC_FC_TX_ENABLED          0x8000
#define RFC_FC_RX_ENABLED          0x4000
#define RFC_MSC_FC_TX_ENABLED      0x2000
#define RFC_MSC_FC_RX_ENABLED      0x1000
#define RFC_CREDIT_BASED_FC        0x0800
#define RFC_INBOUND_MSC_COMPLETE   0x0400
#define RFC_OUTBOUND_MSC_COMPLETE  0x0200
#define RFC_INITIATOR_FLAG         0x0080
#define RFC_DISCONNECTING_FLAG     0x0040
/* Defined in rfcomm_prim.h:

        RFC_CONNFLAG_REQUIRE_MODE  0x0002
        RFC_CONNFLAG_ERTM          0x0001
*/

/* L2CAP flow mode flag handling through RFCOMM.
   Not basic mode(0) = enhanced retransmission mode(1)
   Not preferred mode(0) = required mode(1)
*/
#define RFC_IS_BASIC_MODE(fl)            (((fl) & RFC_CONNFLAG_ERTM) == 0)
#define RFC_IS_ERTM_MODE(fl)             (((fl) & RFC_CONNFLAG_ERTM) != 0)
#define RFC_IS_PREFERRED_MODE(fl)        (((fl) & RFC_CONNFLAG_REQUIRE_MODE) == 0)
#define RFC_SET_FLOW_MODE(fls, fl)       ((fls) |= (fl))

/* Legacy device flow control modes
*/
#define RFC_IS_FC_TX_ENABLED(fl)         (((fl) & RFC_FC_TX_ENABLED) != 0)
#define RFC_IS_FC_RX_ENABLED(fl)         (((fl) & RFC_FC_RX_ENABLED) != 0)
#define RFC_SET_FC_TX_ENABLED(fl)        ((fl) |= RFC_FC_TX_ENABLED)
#define RFC_SET_FC_RX_ENABLED(fl)        ((fl) |= RFC_FC_RX_ENABLED)
#define RFC_RESET_FC_TX_ENABLED(fl)      ((fl) &= ~RFC_FC_TX_ENABLED)
#define RFC_RESET_FC_RX_ENABLED(fl)      ((fl) &= ~RFC_FC_RX_ENABLED)
#define RFC_TOGGLE_FC_TX_ENABLED(fl)     ((fl) ^= RFC_FC_TX_ENABLED)
#define RFC_TOGGLE_FC_RX_ENABLED(fl)     ((fl) ^= RFC_FC_RX_ENABLED)

#define RFC_IS_MSC_FC_TX_ENABLED(fl)     (((fl) & RFC_MSC_FC_TX_ENABLED) != 0)
#define RFC_IS_MSC_FC_RX_ENABLED(fl)     (((fl) & RFC_MSC_FC_RX_ENABLED) != 0)
#define RFC_SET_MSC_FC_ENABLED_FLAG(fls, fl)   ((fls) |= (fl))
#define RFC_RESET_MSC_FC_ENABLED_FLAG(fls, fl) ((fls) &= ~(fl))

/* Using credit based flow control (mandatory for our implementation but could
   come across a peer with BT 1.0 or less). Note for the CFC flag, 1 = reset.
*/
#define RFC_CREDIT_FC_USED(fl)           (((fl) & RFC_CREDIT_BASED_FC) == 0)
#define RFC_RESET_CREDIT_FC_FLAG(fl)     ((fl) |= RFC_CREDIT_BASED_FC)

/* Internal state information
*/
#define RFC_IS_DISCONNECTING(fl)         (((fl) & RFC_DISCONNECTING_FLAG) != 0)
#define RFC_SET_DISCONNECTING(fl)        ((fl) |= RFC_DISCONNECTING_FLAG)
#define RFC_IS_INITIATOR(fl)             (((fl) & RFC_INITIATOR_FLAG) != 0)
#define RFC_SET_INITIATOR(fl)            ((fl) |= RFC_INITIATOR_FLAG)

/* Flags describing the current state of the first MSC handshake on a DLC.
   Done as part of the connection process.
*/
#define RFC_SET_MSC_FLAG(fls, fl)           ((fls) |= (fl))
#define RFC_IS_OUTBOUND_MSC_COMPLETE(fl)   (((fl) & RFC_OUTBOUND_MSC_COMPLETE) != 0)
#define RFC_IS_INBOUND_MSC_COMPLETE(fl)    (((fl) & RFC_INBOUND_MSC_COMPLETE) != 0)
#define RFC_IS_MSC_COMPLETE(fl)   (RFC_IS_OUTBOUND_MSC_COMPLETE(fl) && RFC_IS_INBOUND_MSC_COMPLETE(fl))

/* Macro to convert internally stored flags to externally (ie in prims)
   reported ones. Internally stored flags are 16 bits wide whereas externally
   reported ones are 8 bits. However currently only the lower 4 bits are used
   for external flags.
*/
#define RFC_CONVERT_FLAGS(fls)  ((CsrUint8)((fls) & 0x000F))


/* UIH Frames are only calculated on the address and control frames.
*/
#define RFC_UIH_FCS_CALC_SIZE  2

/* All outbound control frames are the same length except the data frame.
   For inbound control frames the frame length will depend on the size of the
   length field which may be 1 or 2 bytes.
   For data frames (UIH) there may or may not be an extra byte for credits
   depending upon whether credit based flow control is in use or not.
*/
#define RFC_MIN_FRAME_LEN      4 /* Bytes */
#define RFC_CTRLFRAME_LEN      RFC_MIN_FRAME_LEN    /* Bytes */
#define RFC_CTRLFRAME_CRC_LEN  (RFC_CTRLFRAME_LEN - 1)  /* Bytes */
#define RFC_MIN_FRAME_HDR_LEN  (RFC_MIN_FRAME_LEN - 1)

/* For RFCOMM a command frame must fit completely with the specified max frame
   size. This value will be used when validating the latter.
*/
#define RFC_MAX_NON_DATA_FRAME_LEN    14   /* Size of PN / RPN */

/* The max rfcomm frame size used in parneg requests and responses is the
   maximum size of the rfcomm frame that can be sent to L2CAP. This includes any
   data payload plus the RFCOMM frame header. The size of the header depends on
   the size of the data payload (whether the length field requires 7 or 15 bits
   to hold it) and whether or not a credit field is used. Thus
   RFC_MAX_DATA_HEADER_SIZE is set to the maximum possible header size.
*/
#define RFC_MAX_DATA_HEADER_SIZE  6

#define RFC_CMD_HEADER_LEN    2            /* Bytes , Type and Length fields */
#define RFC_DEFAULT_CMD_ADDR_BITS    0x3   /* corresponds to EA and C/R bits both 1 */

/* Offsets into frame and command headers.
*/
#define RFC_FRAME_ADDR_OFFSET    0
#define RFC_FRAME_CTRL_OFFSET    1
#define RFC_FRAME_LEN_OFFSET     2

#define RFC_CMD_TYPE_OFFSET      0
#define RFC_CMD_LEN_OFFSET       1

/* Parameter Negotiation (PN)
*/
#define RFC_PN_DATA_LEN       8            /* Bytes */
#define RFC_PN_FRAME_LEN      (RFC_CMD_HEADER_LEN + RFC_PN_DATA_LEN)
#define RFC_PN_LEN            (RFC_CTRLFRAME_LEN + RFC_PN_FRAME_LEN)
#define RFC_PN_CL_I_CREDIT_BASED_FC_REQ      0xF0
#define RFC_PN_CL_I_CREDIT_BASED_FC_RSP      0xE0
#define RFC_PN_CL_I_NON_CREDIT_BASED_FC_REQ  0x00
#define RFC_PN_CL_I_NON_CREDIT_BASED_FC_RSP  0x00
#define RFC_PN_ACK_TIMER                     0x00
#define RFC_PN_MAX_NUM_RTX                   0x00
#define RFC_MAX_INIT_CREDS   7   /* 3 bit field in the parneg. */

/* Port Negotiation (RPN).
*/
#define RFC_RPN_DATA_LEN      8  /* Bytes */
#define RFC_RPN_FRAME_LEN      (RFC_CMD_HEADER_LEN + RFC_RPN_DATA_LEN)
#define RFC_RPN_LEN            (RFC_CTRLFRAME_LEN + RFC_RPN_FRAME_LEN)

#define RFC_RPN_REQUEST_DATA_LEN   1  /* Bytes - len if requesting peer param */
#define RFC_RPN_REQUEST_FRAME_LEN  (RFC_CMD_HEADER_LEN + RFC_RPN_REQUEST_DATA_LEN)
#define RFC_RPN_REQUEST_LEN        (RFC_CTRLFRAME_LEN + RFC_RPN_REQUEST_FRAME_LEN)

#define RFC_DATA_CTRL_MASK    0x3F
#define RFC_FC_MASK           0x3F

/* Remote Line Status (RLS)
*/
#define RFC_RLS_DATA_LEN      2         /* Bytes */
#define RFC_RLS_FRAME_LEN     (RFC_CMD_HEADER_LEN + RFC_RLS_DATA_LEN)
#define RFC_RLS_LEN           (RFC_CTRLFRAME_LEN + RFC_RLS_FRAME_LEN)
#define RFC_RLS_ERROR_FLAG    0x1

/* Test request. The length is dependent on the amount of data provided in the
   test pattern. Thus the total len is RFC_CTRLFRAME_LEN + RFC_CMD_HEADER_LEN +
   pattern length
*/
#define RFC_TEST_LEN     (RFC_CTRLFRAME_LEN + RFC_CMD_HEADER_LEN)

/* Flow control request (FC)
*/
#define RFC_FC_LEN            (RFC_CTRLFRAME_LEN + RFC_CMD_HEADER_LEN)
#define RFC_FC_FRAME_LEN      RFC_CMD_HEADER_LEN

/* Modem Status Command (MSC)
*/
#define RFC_MSC_DATA_LEN      3   /* Bytes */
#define RFC_MSC_FRAME_LEN     (RFC_CMD_HEADER_LEN + RFC_MSC_DATA_LEN)
#define RFC_MSC_LEN           (RFC_CTRLFRAME_LEN + RFC_MSC_FRAME_LEN)
#define RFC_MSC_IS_BREAK_ENCODED(v)  ((v & 0x1) != 0)
#define RFC_MSC_ENCODE_BREAK_AND_EA   0x3
#define RFC_MSC_MAX_BREAK_VAL   0xF
#define RFC_MSC_BREAK_VAL_SHIFT  0x3

/* Non Supported Command (NSC)
*/
#define RFC_NSC_DATA_LEN      1   /* Byte */
#define RFC_NSC_FRAME_LEN     (RFC_CMD_HEADER_LEN + RFC_NSC_DATA_LEN)
#define RFC_NSC_LEN           (RFC_CTRLFRAME_LEN + RFC_NSC_FRAME_LEN)


/* In Mux commands/responses the each length octet contains 7 bits of length
   information and one EA bit.
*/
#define RFC_CMD_LEN_SIZE  7
#define RFC_CMD_MAX_LEN_OCTET_VAL  0x7F

/* Basic defines and macros
*/
#define RFC_MIN_CONN_ID                   0xC000
#define RFC_CONN_ID_INITIALISER           (RFC_MIN_CONN_ID - 1)

#define RFC_GET_DLCI_FROM_ADDR(addr)      ((addr) >> 2)
#define RFC_GET_SERV_CHAN_FROM_DLCI(dlci)    ((dlci) >> 1)

#define RFC_MUX_CMD_RSP_ADDR_INITIATOR      0x03
#define RFC_MUX_CMD_RSP_ADDR_RESPONDER      0x01


/* A dlci for a remote connection is formed by taking the remote server
   channel number shifting left by 1 bit and ORing in the inverse of the
   parent Mux's direction bit.
*/
#define RFC_NOT(x)                        (((x) & 0x1)^0x1)
#define RFC_SET_DLCI(dirbit,serv)         (((serv) << 1) | RFC_NOT(dirbit))
#define RFC_GET_DIRBIT_FROM_DLCI(d)       ((d) & 0x1)
#define RFC_DIRBIT(fl)                    (RFC_IS_INITIATOR(fl) ? 1 : 0)
#define RFC_IS_SERVER_CHAN(fl,d)  (RFC_DIRBIT((fl)) == RFC_GET_DIRBIT_FROM_DLCI((d)))

#define RFC_GET_CRBIT_FROM_DATA_FIELD(f)  (((f) >> 1) & 0x1)
#define RFC_MASKOUT_CRBIT                 0xFD
#define RFC_IS_CMD(f)   (RFC_GET_CRBIT_FROM_DATA_FIELD((f)) == 1)

#define RFC_IS_EA_BIT_SET(val)    (((val) & 0x01) != 0)
#define RFC_SET_EA_BIT(val)       ((val) |= 0x01)

/* EA bit is only reset on 15 bit length fields.
*/
#define RFC_IS_EA_BIT_RESET(val)  (((val) & 0x01) == 0)
#define RFC_RESET_EA_BIT(val)     ((val) &= 0xFFFE)

/* Macros to get te precalculated FCS for data frames.
   Bits 0-7 standard FCS,
   Bits 8-15 FCS if credit based flow control is being used.
*/
#define RFC_FCS_UIH(f)      ((f) & 0xFF)
#define RFC_FCS_UIH_PF(f)   (((f) >> 8) & 0xFF)


#define RFC_IS_CHAN_OPEN(p_ch)    ((p_ch)->state == RFC_ST_CONNECTED)
#define RFC_IS_CHAN_CLOSED(p_ch)  ((p_ch)->state == RFC_ST_DISCONNECTED)
#define RFC_CHAN_CLOSE(p_ch)      ((p_ch)->state = RFC_ST_DISCONNECTED)

/* Up to 30 server channels can be registered with RFCOMM, numbered 1 - 30.
*/
#define RFC_MAX_NUM_SERVER_CHANNELS  30
#define RFC_MAX_LOCAL_SERVER_CHANNEL 30

#define RFC_DEFAULT_MAX_FRAME_SIZE   127
#define RFC_MAX_RX_CREDITS_PER_FRAME 255

#define HOSTIO_SOURCED_PRIM   0

typedef enum
{
    RFC_CTRL_ACK_TIMER,
    RFC_CMD_RESP_TIMER,
    RFC_L2CAP_RELEASE_TIMER,
    RFC_MUX_SHUTDOWN_TIMER,
    RFC_CREDIT_RETURN_TIMER,
    RFC_MSC_INITIALISATION_TIMER

} RFC_TIMER_VALUES_T;

typedef enum
{
    RFC_CONNECT_CONTEXT,
    RFC_DISCONNECT_CONTEXT,
    RFC_TEST_CONTEXT,
    RFC_CREDIT_CONTEXT,
    RFC_RPN_CONTEXT,
    RFC_MSC_CONTEXT,
    RFC_RLS_CONTEXT,
    RFC_MSC_INIT_CONTEXT,
    RFC_FCON_CONTEXT,
    RFC_FCOFF_CONTEXT,
    RFC_NUM_TIMERS

} RFC_TIMERS_T;

typedef enum
{
    RFC_SERVER = 0x0F,
    RFC_CLIENT = 0xF0

}RFC_CHAN_DIR_T;

typedef enum
{
    RFC_START_SABM,
    RFC_START_PARNEG,
    RFC_START_PORTNEG,
    RFC_STARTED
} RFC_SERVER_START_T;

typedef struct RFC_SERV_CHAN_T_tag
{
    struct RFC_SERV_CHAN_T_tag *p_next;
    CsrSchedQid   phandle;
    CsrUint16    context;
    CsrUint16    flags;
    CsrUint8     id;
} RFC_SERV_CHAN_T;


typedef struct
{
    BD_ADDR_T          bd_addr;
    l2ca_cid_t         cid;
    CsrUint16           l2cap_mtu;
    l2ca_controller_t  local_l2cap_control;
    CsrUint8            serv_init_creds;
    CsrUint16           serv_max_frame_size;
    struct RFC_CHAN_T_tag *p_dlcs;

} RFC_MUX_INFO_T;

typedef struct
{
    CsrUint8  dlci;
    CsrUint8  cfc_supported;
    CsrUint8  priority;
    CsrUint8  ack_timer;
    CsrUint16 max_frame_size;
    CsrUint8  max_rtx;
    CsrUint8  init_creds;

} RFC_PN_VALUES_T;

typedef struct
{
    CsrUint8  dlci;
    CsrBool   request;
    RFC_PORTNEG_VALUES_T  params;

} RFC_RPN_VALUES_T;

typedef struct RFC_QUEUED_DATA_T_tag
{
    struct RFC_QUEUED_DATA_T_tag *p_next;
    CsrMblk  *payload;  /* Pointer to MBLK containing the data */

} RFC_QUEUED_DATA_T;


typedef struct
{
    CsrUint16    client_security_chan;
    CsrUint16    max_frame_size;
    CsrUint8     priority;
    CsrUint16    total_credits;
    l2ca_controller_t   remote_l2cap_control;
    l2ca_controller_t   local_l2cap_control;
    CsrUint8    modem_signal;
    CsrUint8    break_signal;
    CsrUint16   msc_timeout;

} RFC_CONNECTION_PARAMETERS_T;

typedef struct
{
    struct RFC_CTRL_PARAMS_T_tag *contexts[RFC_NUM_TIMERS];

} RFC_TIMER_CONTEXTS_T;

/* RFC structure containing the function pointers to the
    action(s) (kicking in and notification)
*/
typedef struct
{
    /*! RFC notification function pointer */
    CsrBool (*notify_fn)(struct RFC_CTRL_PARAMS_T_tag *);

    /*! RFC kick function pointer */
    void (*kick_fn)(struct RFC_CTRL_PARAMS_T_tag *, CsrBool );

}RFC_ACTIONS_VTABLE_T;

/* NB A future optimisation might be to split off the DLCs into their own conn
   id ordered list containing a pointer to their parent mux. Might mean less
   memory use....
*/
typedef struct
{
    CsrUint16 conn_id;
    CsrUint8  priority;
    CsrUint16 rx_credits;
    CsrUint16 tx_credits;
    CsrUint16 allocated_rx_credits;

    /* Parameter negotiation parameters */
    CsrUint16 max_frame_size;

    const RFC_ACTIONS_VTABLE_T *vtable;  /*!< Pointer to function table */

    RFC_CONNECTION_PARAMETERS_T  *p_config;
    RFC_SERVER_START_T  start_type;
    RFC_PN_VALUES_T *p_temp_pn;
    RFC_RPN_VALUES_T *p_temp_rpn;
    RFC_QUEUED_DATA_T *p_data_q;

} RFC_DLC_INFO_T;

typedef struct RFC_CHAN_T_tag
{
    struct RFC_CHAN_T_tag *p_next;

    CsrSchedQid   phandle;
    fsm_state_t state;
    CsrUint8  dlci;
    CsrUint16 context;
    RFC_TIMER_CONTEXTS_T *timers;

    /* NB for a Mux channel it will inherit the flags used on the first
       connect request. The only ones applicable to the Mux though will be the
       credit based flow control, initiator and flow control ones.
    */
    CsrUint16  flags;

    /* Precalculated FCS for data frames, both outgoing and incoming.
       Bits 0-7 standard FCS,
       Bits 8-15 FCS if credit based flow control is being used.
       NB You do not get UIH_PF frames on the Mux channel and thus only the
       standard fcs applies in this case.
    */
    CsrUint16 fcs_out;
    CsrUint16 fcs_in;

    union
    {
        RFC_MUX_INFO_T  mux;
        RFC_DLC_INFO_T  dlc;
    } info;

} RFC_CHAN_T;


typedef struct RFC_PRIMQ_T_tag
{
    struct RFC_PRIMQ_T_tag *p_next;
    RFCOMM_UPRIM_T *prim;

} RFC_PRIMQ_T;


typedef struct RFC_CTRL_T_tag
{
    CsrSchedQid   phandle;
    CsrBool      use_streams;
    RFC_CHAN_T  *mux_list;
    RFC_SERV_CHAN_T *serv_chan_list;
    CsrUint16    last_conn_id;
    struct RFC_PRIMQ_T_tag *prim_queue;

} RFC_CTRL_T;

typedef struct RFC_CTRL_PARAMS_T_tag
{
    RFC_CTRL_T  *rfc_ctrl;
    RFC_CHAN_T  *p_mux;
    RFC_CHAN_T  *p_dlc;
    CsrMblk      *mblk;
    CsrSchedTid         chan_timer;

} RFC_CTRL_PARAMS_T;


/* Globally exported variables.
*/
EXTERN_RFC_CTRL



/* Internal RFCOMM function prototypes.
*/


/* rfcomm_control.c
*/

void rfc_handle_init_req(RFC_CTRL_PARAMS_T *rfc_params,
                         RFC_INIT_REQ_T *p_prim);
void crank_channel_fsm(RFC_CTRL_PARAMS_T *rfc_params,
                       void  *argv2,
                       fsm_event_t event);
void rfc_handle_register_req(RFC_CTRL_PARAMS_T *rfc_params,
                             RFC_REGISTER_REQ_T *p_prim);
void rfc_handle_unregister_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_UNREGISTER_REQ_T *p_prim);
void rfc_handle_test_req(RFC_CTRL_PARAMS_T *rfc_params,
                         RFC_TEST_REQ_T *p_prim);
void rfc_handle_linestatus_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_LINESTATUS_REQ_T *p_prim);
void rfc_handle_modem_status_req(RFC_CTRL_PARAMS_T *rfc_params,
                                 RFC_MODEM_STATUS_REQ_T *p_prim) ;
void rfc_process_txdata(RFC_CTRL_PARAMS_T *rfc_params, CsrBool credit_frame_send);
void rfc_try_send_credit_only_frame(RFC_CTRL_PARAMS_T *rfc_params, CsrUint16 thresh);
void rfc_new_data_packet(RFC_CTRL_PARAMS_T *rfc_params,
                         CsrMblk *payload, CsrBool credit_frame_send);
void rfc_handle_datawrite_req(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_DATAWRITE_REQ_T *prim) ;
void rfc_handle_fc_req(RFC_CTRL_PARAMS_T *rfc_params,
                       RFC_FC_REQ_T *p_prim) ;
void rfc_send_fc_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                        RFC_CRTYPE  type,
                        RFC_FC_T  fc_type);
RFC_SERV_CHAN_T *rfc_get_reg_server_chan(RFC_CTRL_PARAMS_T *rfc_params,
                                         CsrUint8 serv_chan_id);

RFC_SERV_CHAN_T *rfc_get_reg_server_chan_by_dlci(RFC_CTRL_PARAMS_T *rfc_params,
                                                 CsrUint8 dlci);

CsrMblk  *rfc_create_test_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                RFC_CRTYPE type,
                                CsrMblk *p_data);

CsrMblk  *rfc_create_linestatus_cmdrsp(RFC_CTRL_PARAMS_T   *rfc_params,
                                      RFC_CRTYPE          type,
                                      CsrUint8  line_status_field,
                                      CsrUint8  dlci_field);

CsrBool rfc_validate_line_status(CsrBool error_flag,
                                CsrUint8 line_status);

CsrMblk  *rfc_create_modem_status_cmdrsp(RFC_CTRL_PARAMS_T *rfc_params,
                                        RFC_CRTYPE type,
                                        CsrUint8  dlci_field,
                                        CsrUint8  modem_signal,
                                        CsrUint8  break_signal);

CsrBool rfc_validate_msc_params(CsrUint16 flags,
                               CsrUint8  modem_signal,
                               CsrUint8  *break_signal);

void rfc_update_msc_fc_status(CsrUint16 *flags,
                              CsrUint16 mux_flag,
                              CsrUint16 msc_flag,
                              CsrUint8  modem_signal);

void rfc_handle_portneg_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                            RFC_PORTNEG_RSP_T *p_prim);

void rfc_handle_portneg_req(RFC_CTRL_PARAMS_T *rfc_params,
                            RFC_PORTNEG_REQ_T *p_prim);

void rfc_send_modem_status_req(RFC_CTRL_PARAMS_T *rfc_params,
                               CsrUint8  dlci,
                               CsrUint8  modem_signal,
                               CsrUint8  break_signal);

void rfc_send_raw_uih_frame(RFC_CTRL_PARAMS_T *rfc_params,
                            CsrUint16 payload_len,
                            CsrUint8 rx_credits,
                            CsrMblk   *p_data);

void rfc_data_kick_all(RFC_CTRL_PARAMS_T  *rfc_params);

void rfc_send_nsc_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                      CsrUint8 command,
                      CsrUint8 cmd_res);
void rfc_timer_start(RFC_CTRL_PARAMS_T *rfc_params,
                     RFC_CTRL_PARAMS_T **context,
                     RFC_TIMER_VALUES_T timer);

void rfc_timer_cancel(RFC_CTRL_PARAMS_T **context);

void rfc_timer_expired(RFC_CTRL_PARAMS_T *shutdown_context,
                       RFC_CTRL_PARAMS_T *rfc_params);
CsrBool rfc_is_timer_running(RFC_CTRL_PARAMS_T *context);

void rfc_ctrl_ack_timer_event(CsrUint16 arg1, void *arg2);
void rfc_credit_return_timer_event(CsrUint16 arg1, void *arg2);

void rfc_handle_dataread_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                             CsrUint16 conn_id);

#ifdef INSTALL_AMP_SUPPORT
void rfc_handle_l2ca_move_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_L2CA_MOVE_CHANNEL_RSP_T *p_prim);
void rfc_handle_l2ca_move_req(RFC_CTRL_PARAMS_T *rfc_params,
                              RFC_L2CA_MOVE_CHANNEL_REQ_T *p_prim);
#endif

/*** rfcomm_host_handler.c ***/
void rfc_host_handler(RFC_CTRL_PARAMS_T *rfc_params,
                      RFCOMM_UPRIM_T *rfc_prim);

void rfc_send_init_cfm(CsrSchedQid phandle);

void rfc_send_register_cfm(CsrSchedQid phandle,
                           CsrUint8 loc_serv_chan,
                           CsrUint16 context,
                           CsrBool  accept);

void rfc_send_unregister_cfm(CsrSchedQid phandle,
                             CsrUint8   loc_serv_chan,
                             CsrUint16  context,
                             RFC_RESPONSE_T status);

void rfc_send_server_connect_ind(RFC_CTRL_PARAMS_T *rfc_params);

void rfc_send_server_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                 CsrSchedQid phandle,
                                 CsrUint16 conn_id,
                                 CsrUint16 context,
                                 RFC_RESPONSE_T status);

void rfc_queue_server_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                  CsrSchedQid phandle,
                                  CsrUint16 conn_id,
                                  CsrUint16 context,
                                  RFC_RESPONSE_T status);

void rfc_send_client_connect_cfm(RFC_CTRL_PARAMS_T *rfc_params,
                                 CsrSchedQid phandle,
                                 CsrUint16 conn_id,
                                 CsrUint16 context,
                                 RFC_RESPONSE_T status);

void rfc_send_disconnect_ind(RFC_CHAN_T     *p_dlc,
                             RFC_RESPONSE_T reason);

void rfc_send_disconnect_cfm(CsrSchedQid phandle,
                             CsrUint16  conn_id,
                             RFC_RESPONSE_T status);

void rfc_send_line_status_ind(RFC_CTRL_PARAMS_T *rfc_params,
                              CsrUint8            error_flag,
                              RFC_LINE_STATUS_T  line_status);

void rfc_send_test_cfm(CsrSchedQid phandle,
                       BD_ADDR_T *bd_addr,
                       CsrMblk    *mblk,
                       RFC_RESPONSE_T   status);

void rfc_send_fc_cfm(CsrSchedQid phandle,
                     BD_ADDR_T *bd_addr,
                     RFC_FC_T  fc,
                     RFC_RESPONSE_T status);

void rfc_send_fc_ind(RFC_CTRL_PARAMS_T *rfc_params,
                     RFC_FC_T  fc);

void rfc_send_line_status_cfm(CsrSchedQid      phandle,
                              CsrUint16       conn_id,
                              RFC_RESPONSE_T status);

void rfc_send_msc_cfm(CsrSchedQid phandle,
                      CsrUint16  conn_id,
                      RFC_RESPONSE_T   status);

void rfc_send_msc_ind(RFC_CTRL_PARAMS_T *rfc_params,
                      CsrUint8   modem_signal,
                      CsrUint8   break_signal);

void rfc_send_portneg_ind(CsrSchedQid phandle,
                          CsrUint16  conn_id,
                          RFC_RPN_VALUES_T  *rpn);

void rfc_send_portneg_cfm(CsrSchedQid phandle,
                          CsrUint16  conn_id,
                          RFC_RPN_VALUES_T  *rpn,
                          RFC_RESPONSE_T   status);

void rfc_send_datawrite_cfm(CsrSchedQid phandle,
                            CsrUint16  conn_id,
                            RFC_RESPONSE_T   status,
                            CsrBool is_stream);

CsrBool rfc_send_dataread_ind(RFC_CTRL_PARAMS_T *rfc_params);

void rfc_send_nsc_ind(CsrSchedQid phandle,
                      BD_ADDR_T  *bd_addr,
                      CsrUint8   command_type,
                      CsrUint8   cmd_res);

void rfc_send_error_ind(CsrSchedQid phandle,
                        RFC_PRIM_T err_prim_type,
                        RFC_RESPONSE_T  status);

void rfc_send_upstream_prim(RFCOMM_UPRIM_T *p_prim, CsrUint16 p_handle);

#ifdef INSTALL_AMP_SUPPORT
void rfc_send_l2ca_move_cmp_ind(CsrSchedQid phandle,
                                BD_ADDR_T *bd_addr,
                                l2ca_controller_t local_control,
                                RFC_RESPONSE_T  status);

void rfc_send_l2ca_move_cfm(CsrSchedQid phandle,
                            BD_ADDR_T *bd_addr,
                            l2ca_controller_t local_control,
                            l2ca_controller_t remote_control,
                            RFC_RESPONSE_T  status);

void rfc_send_l2ca_move_ind(CsrSchedQid phandle,
                            BD_ADDR_T *bd_addr,
                            l2ca_identifier_t identifier,
                            l2ca_controller_t local_control);

void rfc_send_l2ca_amp_link_loss_ind(CsrSchedQid   phandle,
                                     BD_ADDR_T   *bd_addr,
                                     hci_error_t reason);
#endif

/*** rfcomm_l2cap_handler.c ***/
void rfc_l2cap_handler(RFC_CTRL_PARAMS_T *rfc_params,
                       L2CA_UPRIM_T *l2cap_prim);

/*** rfcomm_connection_manager.c ***/
void rfc_mux_new(RFC_CTRL_PARAMS_T *rfc_params,
                 CsrSchedQid         phandle,
                 BD_ADDR_T         *p_bd_addr,
                 l2ca_controller_t local_l2cap_control,
                 CsrBool            initiator);

void rfc_find_mux_by_bd_addr(RFC_CTRL_PARAMS_T *rfc_params,
                             BD_ADDR_T *p_bd_addr);

void rfc_get_mux_by_cid(RFC_CTRL_PARAMS_T *rfc_params,
                        l2ca_cid_t  cid);

RFC_CHAN_T * rfc_scan_chan_list_for_id(RFC_CHAN_T *chan_list,
                                       CsrUint16 conn_id);

void rfc_find_chan_by_dlci(RFC_CTRL_PARAMS_T *rfc_params,
                           CsrUint8 dlci);

void rfc_find_chan_by_id(RFC_CTRL_PARAMS_T *rfc_params,
                        CsrUint16 conn_id);

void rfc_dlc_new(RFC_CTRL_PARAMS_T *rfc_params,
                 CsrUint8 dlci,
                 CsrSchedQid phandle,
                 RFC_CONNECTION_PARAMETERS_T  *p_config,
                 CsrBool   initiator,
                 CsrUint16 context);

void rfc_handle_disconnect_req(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_DISCONNECT_REQ_T *p_prim) ;

void rfc_handle_disconnect_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                               RFC_DISCONNECT_RSP_T *p_prim);

void rfc_handle_server_connect_rsp(RFC_CTRL_PARAMS_T *rfc_params,
                                   RFC_SERVER_CONNECT_RSP_T *p_prim);

void rfc_handle_client_connect_req(RFC_CTRL_PARAMS_T *rfc_params,
                                   RFC_CLIENT_CONNECT_REQ_T *p_prim);

void rfc_check_sm_response(RFC_CTRL_PARAMS_T *rfc_params,
                           DM_SM_ACCESS_CFM_T *access_cfm);

void rfc_precalc_fcs_values(RFC_CHAN_T *p_chan);

void rfc_mux_shutdown_timer_event(CsrUint16 arg1, void *arg2);
void rfc_msc_init_timer_event(CsrUint16 arg1, void *arg2);

CsrBool rfc_validate_server_connect_req(RFC_CTRL_PARAMS_T *rfc_params,
                                       CsrUint8 dlci);
CsrUint8 rfc_configure_rx_credits(RFC_CTRL_PARAMS_T *rfc_params);
void rfc_shutdown_session(RFC_CTRL_PARAMS_T *rfc_params,
                          RFC_RESPONSE_T status);

void rfc_queue_prim(RFC_CTRL_T *rfc_ctrl,
                    RFCOMM_UPRIM_T *p_prim,
                    CsrUint16 len);
CsrBool rfc_dequeue_prim(RFC_CTRL_PARAMS_T *rfc_params,
                          RFC_PRIM_T type, CsrUint16 conn_id);

void rfc_flush_prim_queue(RFC_CTRL_PARAMS_T *rfc_params);

void rfc_channel_destroy(RFC_CHAN_T **chan_list,
                         RFC_CHAN_T **chan);

/*** rfcomm_frame.c ***/
CsrMblk  *rfc_create_ctrl_frame(RFC_FRAME_TYPE_T frame,
                               CsrUint16 flags,
                               CsrUint8 dlci);

void rfc_send_dm_frame(CsrUint16 flags,
                       CsrUint8 dlci,
                       l2ca_cid_t cid);

void rfc_parse_frame(RFC_CTRL_PARAMS_T *rfc_params,
                     CsrUint16        mblk_len);
CsrUint8 rfc_frame_crc(const CsrUint8 *frame,
                      CsrUint16 length);
CsrUint8 rfc_create_address_field(CsrUint8             dlci,
                                 RFC_LINK_DESIGNATOR from,
                                 RFC_CRTYPE          cr_type,
                                 RFC_CRLEVEL         level);
CsrUint8 rfc_set_len_EA_bit(CsrUint16 *len);
void rfc_create_uih_header(CsrUint8 dlci,
                           CsrUint8 **frame,
                           CsrUint16 len,
                           RFC_LINK_DESIGNATOR from,
                           CsrUint8 rx_credits);

CsrUint8 rfc_calc_crbit(RFC_LINK_DESIGNATOR from,
                       RFC_CRTYPE type,
                       RFC_CRLEVEL level);

/* rfcomm_stream_handler.c
*/
#ifndef BUILD_FOR_HOST
void rfc_stream_kick(RFC_CTRL_PARAMS_T  *rfc_params, CsrBool credit_frame_send);
CsrBool rfc_stream_notify(RFC_CTRL_PARAMS_T *rfc_params);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _RFCOMM_PRIVATE_H_ */
