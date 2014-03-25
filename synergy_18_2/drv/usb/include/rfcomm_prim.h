/*******************************************************************************
(C) COPYRIGHT Cambridge Consultants Ltd 1999







































*******************************************************************************/
#ifndef BLUESTACK__RFCOMM_PRIM_H /* Once is enough */
#define BLUESTACK__RFCOMM_PRIM_H



#include "hci.h"

#ifdef __cplusplus
extern "C" {
#endif
/* search_string="rfc_prim_t" */
/* conversion_rule="ADD_UNDERSCORE_AND_UPPERCASE_T" */

#ifdef BUILD_FOR_HOST
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define HIDE_EXTENDED_BLUESTACK_PRIMS /* Once is enough */
#endif
#endif

#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#ifdef BLUELAB
#include <sink_.h>
#else
#ifndef Sink
#define Sink uint16_t
#endif
#endif
#endif

/*============================================================================*
 Public Defines
 *============================================================================*/

#define RFCOMM_DEFAULT_FRAME_SIZE   (127)

/*---------------------------------------------------------------*
 Result codes
 *---------------------------------------------------------------*/

/*-----	Applicable to _CFM and RFC_STARTCMP_IND */
#define RFC_SUCCESS		                                    0x00

/*-----	Applicable to RFC_START_CFM
		Codes reflect the codes returned by L2CAP connnect */
#define RFC_CONNECTION_PENDING		        0x01
#define RFC_CONNECTION_REJ_PSM		        0x02      
#define RFC_CONNECTION_REJ_SECURITY		0x03
#define RFC_CONNECTION_REJ_RESOURCES		0x04
#define RFC_CONNECTION_REJ_NOT_READY		0x05
#define RFC_CONNECTION_FAILED			0x06
#define RFC_CONNECTION_TIMEOUT			0x07

    /*-----	Applicable to _CFM */
/*		Remote device initiated disconnection */
#define NORMAL_DISCONNECT                                   0x08
/*		Disconncted due to link loss */
#define ABNORMAL_DISCONNECT                                 0x09

/*-----	Applicable to RFC_START_CFM and RFC_STARTCMP_IND
		Codes reflect those returned by L2CAP Configuration */
#define RFC_CONFIG_UNACCEPTABLE								0x0a
#define RFC_CONFIG_REJECTED									0x0b
/* RESERVED. was #define RFC_CONFIG_INVALID_CID 			0x0c */
#define RFC_CONFIG_UNKNOWN									0x0d
#define RFC_CONFIG_REJECTED_LOCALLY							0x0e
#define RFC_CONFIG_TIMEOUT									0x0f

/*----- Applicable to _CFM */
/*		Refused by remote device */
#define REMOTE_REFUSAL                                      0x11
/*		Remote end initiated at the same time as local end */
#define RACE_CONDITION_DETECTED                             0x12
/*		RFC_START_CFM failed - insufficient pool memory */
#define INSUFFICIENT_RESOURCES                              0x13
/*		Not allowed to change session flow-control mechanism  */
#define CANNOT_CHANGE_FLOW_CONTROL_MECHANISM				0x14
/*		Not allowed to create two DLCs to same remote service  */
#define DLC_ALREADY_EXISTS									0x15
/*      Rejected due to security failure */
#define RFC_DLC_REJ_SECURITY                                0x16

/*		Unknown error received from L2CAP */
#define GENERIC_REFUSAL                                     0x1f

/*----- Applicable to all */
/*		Primitive not expected with RFCOMM in its current state */
#define UNEXPECTED_PRIMITIVE                                0x20
/*		Channel has no phandle registered, or is not yet created */
#define INVALID_SERVER_CHANNEL                              0x21
#define UNKNOWN_MUX_ID			                            0x22
/*		Attempt to close down mux whilst data channels are open */
#define LOCAL_ENTITY_TERMINATED_CONNECTION                  0x23
#define UNKNOWN_PRIMITIVE                                   0x24
/*		RFC_DATA_REQ payload greater than agreed max frame size */
#define MAX_PAYLOAD_EXCEEDED							    0x25
/*		Primitive parameters not self-consistent or protocol-compliant */
#define INCONSISTENT_PARAMETERS								0x26
/*		Insufficient credits to send data */
#define INSUFFICIENT_CREDITS								0x27
/*		Insufficient credits to send data */
#define CREDIT_FLOW_CONTROL_PROTOCOL_VIOLATION				0x28
/*		Multiplexer to the specified device is already open */
#define MUX_ALREADY_OPEN				    0x29

/*----- Applicable to all commands requiring a response */
/*		A response or acknowledgement was not received in time */
#define RFC_RES_ACK_TIMEOUT									0x30

/*---------------------------------------------------------------*
 Port Violation Codes
 *---------------------------------------------------------------*/
#define PORTVIOL_MAXDATARATE_EXCEEDED_INSTANT               0x31
#define PORTVIOL_MAXDATARATE_EXCEEDED_CUMULATIVE            0x32
#define PORTVIOL_MAXPACKETSIZE_EXCEEDED                     0x33

/*---------------------------------------------------------------*
 Security problem (both DLC and MUX)
 *---------------------------------------------------------------*/
#define RFC_CONNECTION_REJ_KEY_MISSING                      0x40

/*---------------------------------------------------------------*
 Invalid Mux Id
 *---------------------------------------------------------------*/
#define INVALID_MUX_ID                                     0xFF

/*---------------------------------------------------------------*
 Port speed indicator octets - per requested speed in bps
 *---------------------------------------------------------------*/
#define PORT_SPEED_2400      0x00
#define PORT_SPEED_4800      0x01
#define PORT_SPEED_7200      0x02
#define PORT_SPEED_9600      0x03
#define PORT_SPEED_19200     0x04
#define PORT_SPEED_38400     0x05
#define PORT_SPEED_57600     0x06
#define PORT_SPEED_115200    0x07
#define PORT_SPEED_230400    0x08
#define PORT_SPEED_UNUSED    0xFF

/*--------------------------------------------*
 #defines to aid CONTROL_PAR_T modem_signal
 *--------------------------------------------*/
#define CONTROL_MODEM_RTC_MASK	0x01
#define CONTROL_MODEM_RTR_MASK  0x02
#define CONTROL_MODEM_IC_MASK	0x04
#define CONTROL_MODEM_DV_MASK   0x08
#define CONTROL_MODEM_FC_MASK   0x10

/*--------------------------------------------*
 #defines to aid RPN primitives
 *--------------------------------------------*/

/* Data bits */
#define DATA_BITS_5          0x00       /* 5 data bits */
#define DATA_BITS_6          0x02       /* 6 data bits */
#define DATA_BITS_7          0x01       /* 7 data bits */
#define DATA_BITS_8          0x03       /* 8 data bits */

/* Stop bits for RPN primitives */
#define STOP_BITS_ONE             0x00  /* 1 stop bit    */
#define STOP_BITS_ONE_AND_A_HALF  0x01  /* 1.5 stop bits */

/* Parity and Parity type indicators for RPN primitives */
#define PARITY_OFF           0x00
#define PARITY_ON            0x01

/* Parity types when parity on */
#define PARITY_TYPE_ODD      0x00
#define PARITY_TYPE_EVEN     0x02
#define PARITY_TYPE_MARK     0x01
#define PARITY_TYPE_SPACE    0x03

/* Flow Control Mask bits */
#define FLC_XONXOFF_INPUT    0x01
#define FLC_XONXOFF_OUTPUT   0x02
#define FLC_RTR_INPUT        0x04
#define FLC_RTR_OUTPUT       0x08
#define FLC_RTC_INPUT        0x10
#define FLC_RTC_OUTPUT       0x20

/* XON and XOFF characters */
#define XON_CHAR_DEFAULT     0x11  /* DC1 character */
#define XOFF_CHAR_DEFAULT    0x13  /* DC3 character */

/* Parameter mask bits */
#define PM_BIT_RATE          0x0001
#define PM_DATA_BITS         0x0002
#define PM_STOP_BITS         0x0004
#define PM_PARITY            0x0008
#define PM_PARITY_TYPE       0x0010
#define PM_XON               0x0020
#define PM_XOFF              0x0040
#define PM_XONXOFF_INPUT     0x0100
#define PM_XONXOFF_OUTPUT    0x0200
#define PM_RTR_INPUT         0x0400
#define PM_RTR_OUTPUT        0x0800
#define PM_RTC_INPUT         0x1000
#define PM_RTC_OUTPUT        0x2000

/*--------------------------------------------*
 #defines to aid RLS primitives
 *--------------------------------------------*/
#define RLS_ERROR_FLAG_OFF   0x00
#define RLS_ERROR_FLAG_ON    0x01

#define RLS_OVERRUN_ERROR    0x01
#define RLS_PARITY_ERROR     0x02
#define RLS_FRAMING_ERROR    0x04

/*----------------------------------------------------------------------------*
 Start of RFCOMM primitives
 *----------------------------------------------------------------------------*/

#define RFCOMM_PRIM_BASE    0x0000

/*----------------------------------------------------------------------------*
 RFCOMM primitives
 *----------------------------------------------------------------------------*/

/* Add primitives here... */
typedef enum rfcomm_prim_tag
{
    ENUM_SEP_RFC_FIRST_PRIM = RFCOMM_PRIM_BASE,
    /* add below here */
    
    ENUM_RFC_INIT_REQ,
	ENUM_RFC_INIT_CFM,
    ENUM_RFC_REGISTER_REQ,
    ENUM_RFC_REGISTER_CFM,
    ENUM_RFC_START_REQ,
    ENUM_RFC_START_RES,
    ENUM_RFC_START_IND,
    ENUM_RFC_START_CFM,
    ENUM_RFC_STARTCMP_IND,
    ENUM_RFC_CLOSE_REQ,
    ENUM_RFC_CLOSE_IND,
    ENUM_RFC_ESTABLISH_REQ,
    ENUM_RFC_ESTABLISH_RES,
    ENUM_RFC_ESTABLISH_IND,
    ENUM_RFC_ESTABLISH_CFM,
    ENUM_RFC_RELEASE_REQ,
    ENUM_RFC_RELEASE_IND,
    ENUM_RFC_FCON_REQ,
    ENUM_RFC_FCON_IND,
    ENUM_RFC_FCON_CFM,
    ENUM_RFC_FCOFF_REQ,
    ENUM_RFC_FCOFF_IND,
    ENUM_RFC_FCOFF_CFM,
    ENUM_RFC_DATA_REQ,
    ENUM_RFC_DATA_IND,
    ENUM_RFC_DATAWRITE_REQ,
    ENUM_RFC_DATAWRITE_CFM,
    ENUM_RFC_CONTROL_REQ,
    ENUM_RFC_CONTROL_IND,
    ENUM_RFC_LINESTATUS_REQ,
    ENUM_RFC_LINESTATUS_IND,
    ENUM_RFC_PARNEG_REQ,
    ENUM_RFC_PARNEG_RES,
    ENUM_RFC_PARNEG_IND,
    ENUM_RFC_PARNEG_CFM,
    ENUM_RFC_PORTNEG_REQ,
    ENUM_RFC_PORTNEG_RES,
    ENUM_RFC_PORTNEG_IND,
    ENUM_RFC_PORTNEG_CFM,
    ENUM_RFC_TEST_REQ,
    ENUM_RFC_TEST_CFM,
    ENUM_RFC_NSC_IND,
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
    ENUM_RFC_EX_RELEASE_IND,
    ENUM_RFC_EX_PARNEG_IND,
    ENUM_RFC_EX_RELEASE_REQ,
    ENUM_RFC_EX_CONTROL_REQ,
    ENUM_RFC_EX_ESTABLISH_IND,
#endif

    /* add above here */
    ENUM_SEP_RFC_LAST_PRIM
}
RFCOMM_PRIM_T;

/* ...and reflect here as a type */
#define RFC_INIT_REQ                   ((rfc_prim_t)ENUM_RFC_INIT_REQ)
#define RFC_INIT_CFM                   ((rfc_prim_t)ENUM_RFC_INIT_CFM)
#define RFC_REGISTER_REQ               ((rfc_prim_t)ENUM_RFC_REGISTER_REQ)
#define RFC_REGISTER_CFM               ((rfc_prim_t)ENUM_RFC_REGISTER_CFM)
#define RFC_START_REQ                  ((rfc_prim_t)ENUM_RFC_START_REQ)
#define RFC_START_RES                  ((rfc_prim_t)ENUM_RFC_START_RES)
#define RFC_START_IND                  ((rfc_prim_t)ENUM_RFC_START_IND)
#define RFC_START_CFM                  ((rfc_prim_t)ENUM_RFC_START_CFM)
#define RFC_STARTCMP_IND               ((rfc_prim_t)ENUM_RFC_STARTCMP_IND)
#define RFC_CLOSE_REQ                  ((rfc_prim_t)ENUM_RFC_CLOSE_REQ)
#define RFC_CLOSE_IND                  ((rfc_prim_t)ENUM_RFC_CLOSE_IND)
#define RFC_ESTABLISH_REQ              ((rfc_prim_t)ENUM_RFC_ESTABLISH_REQ)
#define RFC_ESTABLISH_RES              ((rfc_prim_t)ENUM_RFC_ESTABLISH_RES)
#define RFC_ESTABLISH_IND              ((rfc_prim_t)ENUM_RFC_ESTABLISH_IND)
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define RFC_EX_ESTABLISH_IND           ((rfc_prim_t)ENUM_RFC_EX_ESTABLISH_IND)
#endif
#define RFC_ESTABLISH_CFM              ((rfc_prim_t)ENUM_RFC_ESTABLISH_CFM)
#define RFC_RELEASE_REQ                ((rfc_prim_t)ENUM_RFC_RELEASE_REQ)
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define RFC_EX_RELEASE_REQ             ((rfc_prim_t)ENUM_RFC_EX_RELEASE_REQ)
#endif
#define RFC_RELEASE_IND                ((rfc_prim_t)ENUM_RFC_RELEASE_IND)
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define RFC_EX_RELEASE_IND             ((rfc_prim_t)ENUM_RFC_EX_RELEASE_IND)
#endif
#define RFC_FCON_REQ                   ((rfc_prim_t)ENUM_RFC_FCON_REQ)
#define RFC_FCON_IND                   ((rfc_prim_t)ENUM_RFC_FCON_IND)
#define RFC_FCON_CFM                   ((rfc_prim_t)ENUM_RFC_FCON_CFM)
#define RFC_FCOFF_REQ                  ((rfc_prim_t)ENUM_RFC_FCOFF_REQ)
#define RFC_FCOFF_IND                  ((rfc_prim_t)ENUM_RFC_FCOFF_IND)
#define RFC_FCOFF_CFM                  ((rfc_prim_t)ENUM_RFC_FCOFF_CFM)
#define RFC_DATA_REQ                   ((rfc_prim_t)ENUM_RFC_DATA_REQ)
#define RFC_DATA_IND                   ((rfc_prim_t)ENUM_RFC_DATA_IND)
#define RFC_DATAWRITE_REQ              ((rfc_prim_t)ENUM_RFC_DATAWRITE_REQ)
#define RFC_DATAWRITE_CFM              ((rfc_prim_t)ENUM_RFC_DATAWRITE_CFM)
#define RFC_CONTROL_REQ                ((rfc_prim_t)ENUM_RFC_CONTROL_REQ)
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define RFC_EX_CONTROL_REQ             ((rfc_prim_t)ENUM_RFC_EX_CONTROL_REQ)
#endif
#define RFC_CONTROL_IND                ((rfc_prim_t)ENUM_RFC_CONTROL_IND)
#define RFC_LINESTATUS_REQ             ((rfc_prim_t)ENUM_RFC_LINESTATUS_REQ)
#define RFC_LINESTATUS_IND             ((rfc_prim_t)ENUM_RFC_LINESTATUS_IND)
#define RFC_PARNEG_REQ                 ((rfc_prim_t)ENUM_RFC_PARNEG_REQ)
#define RFC_PARNEG_RES                 ((rfc_prim_t)ENUM_RFC_PARNEG_RES)
#define RFC_PARNEG_IND                 ((rfc_prim_t)ENUM_RFC_PARNEG_IND)
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define RFC_EX_PARNEG_IND              ((rfc_prim_t)ENUM_RFC_EX_PARNEG_IND)
#endif
#define RFC_PARNEG_CFM                 ((rfc_prim_t)ENUM_RFC_PARNEG_CFM)
#define RFC_PORTNEG_REQ                ((rfc_prim_t)ENUM_RFC_PORTNEG_REQ)
#define RFC_PORTNEG_RES                ((rfc_prim_t)ENUM_RFC_PORTNEG_RES)
#define RFC_PORTNEG_IND                ((rfc_prim_t)ENUM_RFC_PORTNEG_IND)
#define RFC_PORTNEG_CFM                ((rfc_prim_t)ENUM_RFC_PORTNEG_CFM)
#define RFC_TEST_REQ                   ((rfc_prim_t)ENUM_RFC_TEST_REQ)
#define RFC_TEST_CFM                   ((rfc_prim_t)ENUM_RFC_TEST_CFM)
#define RFC_NSC_IND					   ((rfc_prim_t)ENUM_RFC_NSC_IND)

/*----------------------------------------------------------------------------*
 end of #define of RFCOMM primitives
 *----------------------------------------------------------------------------*/



/*============================================================================*
 Public Data Types
 *============================================================================*/

/*----------------------------------------------------------------------------*
 primitive type for RFCOMM
 *----------------------------------------------------------------------------*/
typedef uint16_t         rfc_prim_t;

/*----------------------------------------------------------------------------*
 other parameter typedefs
 *----------------------------------------------------------------------------*/
typedef uint8_t          server_chan_t;
typedef uint8_t          mux_id_t;
typedef uint8_t          dlci_t;
typedef bool_t           accept_t;
typedef bool_t           flow_state_t;
typedef uint16_t         data_len_t;
typedef uint8_t          data_ptr_t;
typedef uint8_t          line_status_t;
typedef uint8_t          portviol_code_t;
typedef uint8_t          port_speed_t;
typedef uint8_t          reject_code_t;
typedef uint8_t          result_code_t;
typedef uint8_t          reason_code_t;
typedef uint8_t          ref_prim_code_t;

/*----------------------------------------------------------------------------*
 System parameter typedef
 *----------------------------------------------------------------------------*/
typedef struct 
{
    port_speed_t   port_speed;     /* port speed indicator */
    uint16_t       max_frame_size; /* Maximum frame size (bytes) */
} SYS_PAR_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 DLC parameter typedef
 *----------------------------------------------------------------------------*/
typedef struct 
{
    uint16_t			max_frame_size;   /* Maximumm frame size for DLC */
	uint8_t				credit_flow_ctrl; /* true if requesting credit_based  */
	uint16_t			initial_credits;  /* initial credits */
} DLC_PAR_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 Control parameter typedefs
 *----------------------------------------------------------------------------*/
typedef struct 
{
    uint8_t        modem_signal;     /* modem signal */
    uint8_t        break_signal;     /* break signal (7 LSBs) */
} CONTROL_PAR_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 

/*----------------------------------------------------------------------------*
 Port parameter typedefs
 *----------------------------------------------------------------------------*/
typedef struct 
{
    port_speed_t   port_speed;     /* port speed indicator - see #defines above */
    uint8_t        data_bits;	   /* DATA_BITS_5, _6, _7 or _8 - see above */
    uint8_t        stop_bits;      /* STOP_BITS_ONE or _ONE_AND_A_HALF - see above */
	uint8_t        parity;         /* PARITY_OFF or PARITY_ON */ 
    uint8_t        parity_type;    /* PARITY_TYPE_ODD, _EVEN, _MARK or _SPACE */
	uint8_t        flow_ctrl_mask; /* 6 bits - use FLC_ #defines above (see 07.10) */
	uint8_t        xon;            /* xon character  (default DC1) */
	uint8_t        xoff;           /* xoff character (default DC3) */
	uint16_t       parameter_mask; /* 16 bits (top two reserved) see PM_ #defines */
} PORT_PAR_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 


/*----------------------------------------------------------------------------*
 Primitive typedefs
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     To initialise RFCOMM from a system point of view (rather than at stack
 *     start-up. It registers the psm and protocol handle to which 
 *     remote requests to start an RFCOMM session will be sent via 
 *     RFC_Start_Ind events. This call will cause RFCOMM to register itself 
 *     with L2CAP using the RFCOMM psm value. This primitive is also used to set
 *	   flow control parameters.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
  rfc_prim_t	type;             /* Always RFC_INIT_REQ */
  phandle_t	phandle;          /* Protocol handle */ 
  psm_t		psm_local;        /* psm for control entity communication */
  
  bool_t		use_flow_control;	

  /* The following are NOT used unless use_flow_contro */
  uint16_t	fc_type ;          /* should be set to 1 */
  uint16_t	fc_threshold;      /* flow control messages will be generated every
					      fc_threshold pdus */
  uint32_t	fc_timer;          /* and every fc_timer milliseconds */

  uint32_t	rsvd_4;            /* set to 0 */
  uint32_t	rsvd_5;            /* set to 0 */
} RFC_INIT_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event confirms initialisation of RFCOMM.
 *	   Also reports the current flow control settings.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
	rfc_prim_t  type;             /* Always RFC_INIT_CFM */
	phandle_t   phandle;          /* Protocol handle */ 
	psm_t       psm_local;        /* psm for control entity communication */

	uint16_t  fc_type;	
	uint16_t  fc_threshold;  
        uint32_t  fc_timer ;
	uint32_t  rsvd_4 ;
	uint32_t  rsvd_5 ;
} RFC_INIT_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event allows the application to register a protocol handle with 
 *     RFCOMM. RFCOMM will return a RFC_REGISTER_CFM primitive with an
 *     assigned server channel number.
 *     The registered protocol handle will be used for notifying the 
 *     application of the arrival of events for the given server channel.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_REGISTER_REQ */
   phandle_t      phandle;          /* Protocol handle for callback */   
} RFC_REGISTER_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event will be used to indicate to the higher layer that its
 *     previous registration of a protocol handle with an RFC_REGISTER_REQ
 *     event had been accepted (if accept is set to True) or denied (if accept
 *     is set to false). The server channel number assigned is also returned.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;		      /* Always RFC_REGISTER_CFM */
   phandle_t        phandle;	      /* Protocol handle */
   server_chan_t    server_chan;	  /* Server channel number */
   accept_t         accept;	          /* Accept flag */
} RFC_REGISTER_CFM_T;

/*===========================================================================*
 Multiplexer connection start up & close down.

		Local Device               Remote Device
	Initiator:
		--------------RFC_START_REQ------------>
		<-------------RFC_START_CFM(Pending)----
		<-------------RFC_START_CFM-------------
	Responder:
		<-------------RFC_START_IND-------------
		--------------RFC_START_RES------------>
		<-------------RFC_START_IND-------------
		--------------RFC_START_RES------------>
		<-------------RFC_STARTCMP_IND----------

    Closing down:
		--------------RFC_CLOSE_REQ------------>
	Or:
		<-------------RFC_CLOSE_IND------------

 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command requests the initiation of a connection between the 
 *     local device and a remote device specified by its Bluetooth device 
 *     address.
 *     The sys_pars fields indicate the desired port speed, (only used if L2CAP
 *     QOS is required), and the maximum frame size that the local device can
 *     handle (this relates to the local MTU to be used on the L2CAP channel).
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_START_REQ */
   BD_ADDR_T      bd_addr;          /* Bluetooth device address */
   psm_t          psm_remote;       /* psm of remote device */
   SYS_PAR_T      sys_pars;         /* System parameters */
   phandle_t      respond_phandle;  /* Reply phandle (overides dflt is set) */
} RFC_START_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is the confirmation (or otherwise) of the opening of a
 *     connection following a start request.
 *     The sys_pars fields indicate the port speed, (only used if L2CAP
 *     QOS is required), and the maximum frame size that apply to the 
 *     connection. Note that the remote device may have negotiated values 
 *     lower than those in the RFC_START_REQ.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_START_CFM */
   phandle_t        phandle;          /* Routing phandle */
   BD_ADDR_T        bd_addr;          /* Bluetooth device address */
   mux_id_t         mux_id;           /* Local multiplexer id */
   result_code_t    result_code;      /* result code */
   SYS_PAR_T        sys_pars;         /* System Parameters */
} RFC_START_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is an indication to the application that a remote device
 *     wishes to start an RFCOMM connection.
 *     The event is sent to the application (at least) twice:
 *
 *     (1) To allow the application to decide whether to accept a connection
 *         from this device. (In which case the sys_pars will be 'don't care').
 *     (2) To inform the application of the system parameters suggested by
 *         the remote device. The application can then suggest lower values
 *         or agree with the values suggested.
 *
 *     In both cases the application responds with RFC_START_RES.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_START_IND */
   phandle_t        phandle;          /* Routing phandle */
   BD_ADDR_T        bd_addr;          /* remote Bluetooth device address */
   mux_id_t         mux_id;           /* Local multiplexer id */
   SYS_PAR_T        sys_pars;         /* System parameters */
} RFC_START_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command is the response to RFC_START_IND events. 
 *
 *     If the application is responding to the first RFC_START_IND, the 
 *     application can decide whether to accept the connection attempt using
 *     the accept field. In this case the sys_pars field should be set to
 *     the values provided in the RFC_START_IND.
 *     
 *     If the application is responding to subsequent RFC_START_IND events, 
 *     the accept field should be set TRUE. The system parameters should either
 *     be set to the values suggested in the RFC_START_IND, or lower values 
 *     may be suggested.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_START_RES */
   mux_id_t       mux_id;           /* local multiplexer id */
   accept_t       accept;           /* Accept flag */
   SYS_PAR_T      sys_pars;         /* System Parameters */
   phandle_t      respond_phandle;  /* Reply phandle (overides dflt if set) */
} RFC_START_RES_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event confirms the opening of a multiplexer connection with the
 *     reported system parameters. When this event has been received, the 
 *     remote device would normally then go on to set up a DLC.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_STARTCMP_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   result_code_t    result_code;      /* Result code */
   SYS_PAR_T        sys_pars;         /* System Parameters */
} RFC_STARTCMP_IND_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command forces closure of the RFCOMM link to a remote device. 
 *     All server channels will be released. 
 *     This command has no confirmation.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_CLOSE_REQ */
   mux_id_t       mux_id;           /* local multiplexer id */
} RFC_CLOSE_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that the remote device has closed the 
 *     RFCOMM connection.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;             /* Always RFC_CLOSE_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t	        mux_id;           /* Local multiplexer id */
   reason_code_t	reason_code;	  /* Reason for closing */
} RFC_CLOSE_IND_T;


/*===========================================================================*
 DLC establishment & release.

		Local Device               Remote Device
	Initiator:
		 -------------RFC_PARNEG_REQ------------>
		<-------------RFC_PARNEG_CFM------------ 
		 ------------RFC_ESTABLISH_REQ---------->
		<------------RFC_ESTABLISH_CFM---------- 
	Responder:
		<-------------RFC_PARNEG_IND------------ 
		 -------------RFC_PARNEG_RES------------>
		<------------RFC_ESTABLISH_IND---------- 
		 ------------RFC_ESTABLISH_RES---------->

    Closing down:
		 ------------RFC_RELEASE_REQ------------>
		<------------RFC_RELEASE_IND------------
	Or:
		<------------RFC_RELEASE_IND------------

 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command initiates the negotiation for a RFCOMM DLC.
 *
 *     When the remote device responds, an RFC_PARNEG_CFM event is issued
 *     containing the parameters that have been accepted by the peer. These may
 *     differ from those in the request, in particular the frame size may be
 *     smaller and credit-based flow control may have been rejected.
 *
 *     There is no concept of the remote port entity "rejecting" the negotiation
 *     except by responding with a DM frame (RFC_Release_Ind).
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;			  /* Always RFC_PARNEG_REQ */
   mux_id_t	        mux_id;			  /* local multiplexer id */
   server_chan_t	loc_server_chan;  /* local server channel number */
   server_chan_t    rem_server_chan;  /* remote server channel number */
   DLC_PAR_T	    dlc_pars;		  /* DLC Parameters */
} RFC_PARNEG_REQ_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is the confirmation of a locally-initiated negotiation of
 *     DLC parameters, and contains the set of parameters that have been agreed
 *     between the devices. These may differ from those in the original request
 *     because the remote device may reduce the frame size, or it may refuse to
 *     use credit-based flow control.
 *
 *     There is no concept of "rejecting" a parameter negotiation - the values
 *     received in this event are those that will be used when the DLC is
 *     established. If these are still unacceptable to the local port entity,
 *     it may release the DLC with RFC_Release_Req, or it may attempt to
 *     renegotiate different values.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_PARNEG_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   DLC_PAR_T        dlc_pars;         /* DLC Parameters */
} RFC_PARNEG_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that a remote device wishes to set up a connection
 *     (DLC) to a local server channel number.
 *     The dlc_pars field contains the DLC parameters requested. 
 *     The application must respond with RFC_PARNEG_RES including the DLC
 *     acceptable parameters (which may be lower than those suggested).
 *
 *     There is no concept of "rejecting" a parameter negotiation, except by
 *     explicitly releasing the DLC with RFC_Release_Req.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_PARNEG_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   DLC_PAR_T        dlc_pars;         /* DLC Parameters */
} RFC_PARNEG_IND_T;


#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that a remote device wishes to set up a connection
 *     (DLC) to a local server channel number.
 *     The dlc_pars field contains the DLC parameters requested. 
 *     The application must respond with RFC_PARNEG_RES including the DLC
 *     acceptable parameters (which may be lower than those suggested).
 *
 *     There is no concept of "rejecting" a parameter negotiation, except by
 *     explicitly releasing the DLC with RFC_Release_Req.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_PARNEG_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   DLC_PAR_T        dlc_pars;         /* DLC Parameters */
   BD_ADDR_T	    bd_addr;          /* Bluetooth device address */
} RFC_EX_PARNEG_IND_T;
#endif

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This is the response to a RFC_PARNEG_IND event. 
 *     The application must fill in the dlc_pars with those from the
 *     ind, replacing any unacceptable parameters with acceptable values. The
 *     possible changes are; reduced frame size, and rejection of credit-based
 *     flow control.
 *
 *     The parameters sent in the response will be the values used for the DLC
 *     when it is established, unless the remote port entity renegotiates new
 *     values by sending another RFC_PARNEG_IND.
 *
 *     There is no concept of "rejecting" a parameter negotiation, except by
 *     explicitly releasing the DLC with RFC_Release_Req.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;				/* Always RFC_PARNEG_RES */
   mux_id_t	        mux_id;				/* local multiplexer id */
   server_chan_t	server_chan;		/* local server channel number */
   DLC_PAR_T	    dlc_pars;			/* DLC Parameters */
} RFC_PARNEG_RES_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command requests the establishment of a new server channel with the 
 *     current set of dlc parameters on the multiplexer. An RFC_ESTABLISH_CFM
 *     primitive will be sent as confirmation (+ve or -ve) of the request
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_ESTABLISH_REQ */
   mux_id_t       mux_id;           /* local multiplexer id */
   server_chan_t  loc_server_chan;  /* local server channel number */
   server_chan_t  rem_server_chan;	/* remote server channel number */ 
} RFC_ESTABLISH_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is the confirmation or rejection of the establishment of a 
 *     DLC on the multiplexer instance with the desired system parameters
 *     following an establish request. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_ESTABLISH_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   result_code_t    result_code;      /* Result code */
} RFC_ESTABLISH_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that a remote device wishes to start up a DLC 
 *     connection to the supplied server channel.
 *     The port entity should respond with RFC_ESTABLISH_RES.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_ESTABLISH_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */ 
} RFC_ESTABLISH_IND_T;

#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that a remote device wishes to start up a DLC 
 *     connection to the supplied server channel.
 *     The port entity should respond with RFC_ESTABLISH_RES.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_ESTABLISH_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */ 
   BD_ADDR_T	    bd_addr;          /* Bluetooth device address */
} RFC_EX_ESTABLISH_IND_T;
#endif


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is the response to the previous RFC_ESTABLISH_IND event. If 
 *     the application accepts the connection then accept
 *     shall be set to TRUE. Otherwise accept shall be set to FALSE.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_ESTABLISH_RES */
   mux_id_t       mux_id;           /* local multiplexer id */
   server_chan_t  server_chan;      /* local server channel number */
   accept_t       accept;           /* Accept flag */ 
} RFC_ESTABLISH_RES_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     The application wishes to close down the data link connection 
 *     referenced by the server channel within the RFCOMM multiplexer to the 
 *     remote Bluetooth device indexed by its mux_id. This primitive is
 *     confirmed by RFC_RELEASE_IND.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_RELEASE_REQ */
   mux_id_t       mux_id;           /* local multiplexer id */
   server_chan_t  server_chan;      /* loca server channel number */
} RFC_RELEASE_REQ_T;

#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     The application wishes to close down the data link connection 
 *     to remote Bluetooth device referenced by it's sink. This primitive is
 *     confirmed by RFC_RELEASE_IND.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_RELEASE_REQ */
   Sink           sink;
   uint16_t       dummy;
} RFC_EX_RELEASE_REQ_T;
#endif

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is an indication that the data link connection referenced 
 *     by the server channel within the multiplexor session indexed by mux_id
 *     has closed down.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_RELEASE_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Local server channel number */
   reason_code_t	reason_code;	  /* Reason for releasing */
} RFC_RELEASE_IND_T;

#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is an indication that the data link connection referenced 
 *     by the server channel within the multiplexor session indexed by mux_id
 *     has closed down.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_EX_RELEASE_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Local server channel number */
   reason_code_t    reason_code;      /* Reason for releasing */
   uint8_t          num_server_chans; /* Number of open DLC's on this Mux */
} RFC_EX_RELEASE_IND_T;
#endif

/*===========================================================================*
 DLC Control Parameter service

  The DLC Control Parameter service is used to convey control parameters 
  between Port Emulation Entities (using the Modem Status Command - MSC).
  This service is to control a specific DLC. It includes parameters such as 
  flow control, Modem signals, Break.

  Each device must send control parameters, prior to sending user data, after 
  creation of a DLC.

		Local Device               Remote Device
		 ------------RFC_CONTROL_REQ------------>
		<------------RFC_CONTROL_IND------------ 

 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used by application to convey control 
 *     information to the remote port (including flow control on a server
 *     channel). The control information is set in control_pars.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_CONTROL_REQ */
   mux_id_t	  mux_id;           /* local multiplexer id */
   server_chan_t  server_chan;      /* local server channel number */
   CONTROL_PAR_T  control_pars;     /* Control parameters */
} RFC_CONTROL_REQ_T;


#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used by application to convey control 
 *     information to the remote port (including flow control on a server
 *     channel). The control information is set in control_pars.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_EX_CONTROL_REQ */
   Sink           sink;
   uint16_t       dummy;
   CONTROL_PAR_T  control_pars;     /* Control parameters */
} RFC_EX_CONTROL_REQ_T;
#endif

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates to the port entity that the remote port emulation
 *     entity wishes to use the control parameters set in control_pars.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_CONTROL_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   CONTROL_PAR_T    control_pars;     /* Control parameters */
} RFC_CONTROL_IND_T;


/*===========================================================================*
 Port Emulation Services

	These primitives are used to communicate the port parameters - port speed,
	data bits, stop bits, parity. 

		Local Device               Remote Device
	Initiator:
		 ------------RFC_PORTNEG_REQ------------>
		<------------RFC_PORTNEG_CFM------------ 
	Responder:
		<------------RFC_PORTNEG_IND------------ 
		 ------------RFC_PORTNEG_RES------------>

 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This command indicates to the remote port entity that the local
 *     port entity requests it to set port parameters for the DLC
 *     as given in the Port Parameters. If a response is received from the
 *     remote port entity then a RFC_PORTNEG_REQ event is received even if
 *     the parameters are not agreed. Note that the rem_server_chan is not 
 *     required once a DLC has been established - RFCOMM can identify the 
 *	   DLC using the local server channel.
 *
 *     If request is set to TRUE, this command is a request for the remote
 *     port to report it's current values.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;             /* Always RFC_PORTNEG_REQ */
   mux_id_t	        mux_id;           /* local multiplexer id */
   server_chan_t	loc_server_chan;  /* local server channel number */
   server_chan_t    rem_server_chan;  /* remote server channel number */
   bool_t           request;          /* a request (See TS07.10 5.4.3.6.9) */
   PORT_PAR_T       port_pars;        /* Port Parameters */
} RFC_PORTNEG_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is the confirmation of the locally-initiated port negotiation
 *     procedure. The contents of the port parameters will be the same as those
 *     in the original request, except where the corresponding bit in the
 *     parameter_mask is set to zero, in which case that parameter is set to
 *     an alternative value proposed by the remote device.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_PORTNEG_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   PORT_PAR_T       port_pars;        /* Port Parameters */
} RFC_PORTNEG_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is indicates to the higher port entity that the remote
 *     port entity requests it to set port parameters for the DLCI as given in
 *     the Port Parameters. The port entity should respond with RFC_PORTNEG_RES.
 *
 *     If request is true, the indication shows the values of the port at the
 *     remote port entity.
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_PORTNEG_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   bool_t           request;          /* a request (See TS07.10 5.4.3.6.9) */
   PORT_PAR_T       port_pars;        /* Port Parameters */
} RFC_PORTNEG_IND_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *      This event is the response to the previous RFC_PORTNEG_REQ event. The
 *      responding port entity must set the 'port_pars.parameter_mask' bits to
 *      '1' to indicate which new parameters have been accepted, or to '0' to
 *      indicate any which are rejected.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;			/* Always RFC_PORTNEG_RES */
   mux_id_t	        mux_id;			/* local multiplexer id */
   server_chan_t    server_chan;	/* local server channel number */
   PORT_PAR_T       port_pars;		/* Port Parameters */
} RFC_PORTNEG_RES_T;


/*===========================================================================*
 Port Flow control services (aggregate).

	These primitives are used to communicate the flow control for all
	connections on the RFCOMM multiplexer.

    This mechanism should not be used if the DLC has been established with
	credit_flow_ctrl==FALSE. The service is retained for backwards 
	compatability with v1.0b of the Bluetooth specification.

		Local Device               Remote Device
		<--Data arrives from remote port------
		 ------------RFC_FCOFF_REQ------------>
		<------------RFC_FCOFF_CFM------------ 
		 --------remote device stops sending--
		 ------------RFC_FCON_REQ------------->
		<------------RFC_FCON_CFM------------- 
		<--Data arrives from remote port------

		 --Sending data to remote port-------->
		<------------RFC_FCOFF_IND------------ 
         --Stop sending data------------------
		<------------RFC_FCON_IND-------------
		 --Sending data to remote port-------->
		
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It indicates that the local
 *     calling entity is not able to receive new information. The opposite 
 *     entity is not allowed to transmit frames except on the control channel.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t     type;             /* Always RFC_FCOFF_REQ */
   mux_id_t       mux_id;           /* local multiplexer id */
} RFC_FCOFF_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It is a confirmation
 *     to a previously sent FCoff_Req
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_FCOFF_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* local multiplexer id */
} RFC_FCOFF_CFM_T;

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It indicates that the 
 *     local calling entity is able to receive new information.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	  type;             /* Always RFC_FCON_REQ */
   mux_id_t	      mux_id;           /* local multiplexer id */
} RFC_FCON_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It is a confirmation
 *     to a previously sent FCon_Req
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_FCON_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* local multiplexer id */
} RFC_FCON_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It indicates that the
 *     remote entity is not able to receive new information. The
 *     local entity is not allowed to transmit frames except on the control
 *     channel.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_FCOFF_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* local multiplexer id */
} RFC_FCOFF_IND_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is used to handle aggregate flow. It indicates that the
 *     remote entity is able to receive new information.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_FCON_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* local multiplexer id */
} RFC_FCON_IND_T;


/*===========================================================================*
 Port Data services

	These primitives are used to send and receive data on the port.

		Local Device               Remote Device
		 ------------RFC_DATA_REQ------------->
		 ------------RFC_DATA_REQ------------->
		<------------RFC_DATA_IND-------------
		 ------------RFC_DATA_REQ------------->
		 ----------RFC_DATAWRITE_REQ---------->
		<----------RFC_DATAWRITE_CFM----------
		<------------RFC_DATA_IND-------------
		<------------RFC_DATA_IND-------------
	
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is a request to RFCOMM to transmit the data given in the 
 *     payload to the remote address on the DLC identified by the server channel.
 *     The payload will be transmitted in the form of UIH frames. The maximum frame
 *     size will either be the default, or negotiated during mux startup or 
 *     parameter negotiation. This primitive is not confirmed
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	  type;			    /* Always RFC_DATA_REQ */
   mux_id_t	      mux_id;			/* local multiplexer id */
   server_chan_t  server_chan;		/* local server channel number */
   uint16_t		  credits;			/* credits - for credit-based flow-control */
   data_len_t	  payload_length;	/* Length of the payload */
   data_ptr_t	 *payload;		    /* Pointer to the data */
} RFC_DATA_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is a request to RFCOMM to transmit the data given in the 
 *     payload to the remote address along the chosen server channel. The 
 *     payload will be transmitted in the form of UIH frames. The maximum frame
 *     size will either be the default, or negotiated during mux startup or 
 *     parameter negotiation. This primitive will result in an RFC_DATAWRITE_CFM.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	  type;			    /* Always RFC_DATAWRITE_REQ */
   mux_id_t	      mux_id;			/* local multiplexer id */
   server_chan_t  server_chan;		/* local server channel number */
   uint16_t		  credits;			/* credits - for credit-based flow-control */
   data_len_t	  payload_length;	/* Length of the payload */
   data_ptr_t	 *payload;		    /* Pointer to the data */
} RFC_DATAWRITE_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates that the previous RFC_DATAWRITE_REQ was successfully
 *     sent from L2CAP. 
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	  type;			    /* Always RFC_DATAWRITE_CFM */
   phandle_t      phandle;        /* Routing phandle */
   mux_id_t	      mux_id;			/* local multiplexer id */
   server_chan_t  server_chan;		/* local server channel number */
} RFC_DATAWRITE_CFM_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates to the higher port entity that data has arrived
 *     from the remote device and is stored in the location given by payload.
 *
 *		NOTE:   To minimise copy operations within RFCOMM, 'payload' points
 *				to the first byte of RFCOMM payload data within the L2CAP packet
 *				supplied. The prelease field points to the first byte of
 *				the L2CAP packet itself.
 *				When the payload has been processed, the recipient must 
 *				release the data pointed to by the 'prelease' pointer.
 *				
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_DATA_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   uint16_t			credits;		  /* credits - for credit-based flow-control */
   data_len_t       payload_length;   /* Length of the payload */
   data_ptr_t      *payload;          /* Pointer to the data */
   data_ptr_t      *prelease;         /* Pointer to be used when free'ing */
} RFC_DATA_IND_T;


/*===========================================================================*
 DLC Line status indication service

	These primitives are used to indicate a DLC's line status.

		Local Device               Remote Device
		 ---------RFC_LINESTATUS_REQ----------->
		<---------RFC_LINESTATUS_IND-----------
	
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This primitive is used to send the line status to the remote port entity.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;           /* Always RFC_LINESTATUS_REQ */
   mux_id_t	        mux_id;         /* local multiplexer id */
   server_chan_t	server_chan;    /* local server channel number */
   uint8_t          error_flag;     /* error flag : RLS_ERROR_FALG_ON/OFF */
   line_status_t	line_status;    /* Line Status Parameter */
} RFC_LINESTATUS_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event indicates the line status of the remote port entity.
 *     There is no response.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_LINESTATUS_IND */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   server_chan_t    server_chan;      /* Server channel number */
   uint8_t          error_flag;       /* error flag : RLS_ERROR_FLAG_ON/OFF*/
   line_status_t    line_status;      /* Line Status Parameter ( */
} RFC_LINESTATUS_IND_T;


/*===========================================================================*
 Test service

	These primitives are used to test the communication link between two 
	RFCOMM entities.

		Local Device               Remote Device
		 ------------RFC_TEST_REQ-------------->
		<------------RFC_TEST_CFM--------------
	
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event requests a test of the communication link. The test data is
 *     sent to the remote RFCOMM on the control channel which shall 
 *     automatically loop it back. If the data arrives back then a RFC_TEST_CFM 
 *     event shall be sent to the application.    
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t	    type;			  /* Always RFC_TEST_REQ */
   mux_id_t	        mux_id;			  /* local multiplexer id */
   data_len_t	    test_data_length; /* Length of the test data */
   data_ptr_t	   *test_data;		  /* Pointer to the data */
} RFC_TEST_REQ_T;


/*----------------------------------------------------------------------------*
 * PURPOSE
 *     This event is confirmation of a previously sent RFC_TEST_REQ event. The
 *     contents of test_data will be that which has been looped back from the
 *     remote RFCOMM.
 *
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_TEST_CFM */
   phandle_t        phandle;          /* Routing phandle */
   mux_id_t         mux_id;           /* Local multiplexer id */
   data_len_t       test_data_length; /* Length of the test data */
   data_ptr_t      *test_data;        /* Pointer to the data */
} RFC_TEST_CFM_T;


/*===========================================================================*
 Non Supported Command Response (NSC)

	This primitive is sent to the application whenever a command type is not 
	supported by the receiving entity.

		Local Device               Remote Device
		 ----------Any _REQ or _RES------------>
		<------------RFC_NSC_IND---------------
	
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *	   Non Supported Command Response. See TS07.10 5.4.6.3.8.
 *     Indicates that the remote device did not understand the last frame sent.
 *	   NB the command_type given is the RFCOMM command type for the 
 *	   command, not one of the rfc_prim_t 'type' enumerations.
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;             /* Always RFC_NSC_IND */
   phandle_t        phandle;          /* Routing phandle */ 
   mux_id_t         mux_id;           /* Local multiplexer id */
   uint8_t		    command_type;     /* NSC command type */
   uint8_t			cmd_res;          /* C/R bit value */
} RFC_NSC_IND_T;


/*===========================================================================*
 *===========================================================================*/

/*----------------------------------------------------------------------------*
 * PURPOSE
 *		Common elements of an RFCOMM upstream primitive.
 *----------------------------------------------------------------------------*/
typedef struct
{
   rfc_prim_t       type;
   phandle_t        phandle;

} RFC_UPSTREAM_COMMON_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 
/*----------------------------------------------------------------------------*
 * PURPOSE
 *     Union of the primitives
 *
 *----------------------------------------------------------------------------*/
typedef union
{
   rfc_prim_t                   type;
   RFC_UPSTREAM_COMMON_T        rfc_upstream_common ;
   RFC_INIT_REQ_T               rfc_init_req;    
   RFC_INIT_CFM_T               rfc_init_cfm;    
   RFC_REGISTER_REQ_T           rfc_register_req;    
   RFC_REGISTER_CFM_T           rfc_register_cfm;   
   RFC_START_REQ_T              rfc_start_req;   
   RFC_START_RES_T              rfc_start_res;    
   RFC_START_IND_T              rfc_start_ind;    
   RFC_START_CFM_T              rfc_start_cfm;   
   RFC_STARTCMP_IND_T           rfc_startcmp_ind;    
   RFC_CLOSE_REQ_T              rfc_close_req;    
   RFC_CLOSE_IND_T              rfc_close_ind;    
   RFC_ESTABLISH_REQ_T          rfc_establish_req;    
   RFC_ESTABLISH_RES_T          rfc_establish_res;    
   RFC_ESTABLISH_IND_T          rfc_establish_ind;    
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
   RFC_EX_ESTABLISH_IND_T         rfc_ex_establish_ind;
#endif
   RFC_ESTABLISH_CFM_T          rfc_establish_cfm;    
   RFC_RELEASE_REQ_T            rfc_release_req;
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
   RFC_EX_RELEASE_REQ_T         rfc_ex_release_req;
#endif
   RFC_RELEASE_IND_T            rfc_release_ind;
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
   RFC_EX_RELEASE_IND_T		rfc_ex_release_ind;
#endif
   RFC_FCON_REQ_T               rfc_fcon_req;    
   RFC_FCON_IND_T               rfc_fcon_ind;
   RFC_FCON_CFM_T               rfc_fcon_cfm;
   RFC_FCOFF_REQ_T              rfc_fcoff_req;    
   RFC_FCOFF_IND_T              rfc_fcoff_ind;    
   RFC_FCOFF_CFM_T              rfc_fcoff_cfm;    
   RFC_DATA_REQ_T               rfc_data_req;    
   RFC_DATA_IND_T               rfc_data_ind;    
   RFC_DATAWRITE_REQ_T          rfc_datawrite_req;    
   RFC_DATAWRITE_CFM_T          rfc_datawrite_cfm;    
   RFC_CONTROL_REQ_T            rfc_control_req;
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
   RFC_EX_CONTROL_REQ_T         rfc_ex_control_req;    
#endif
   RFC_CONTROL_IND_T            rfc_control_ind;    
   RFC_LINESTATUS_REQ_T         rfc_linestatus_req;    
   RFC_LINESTATUS_IND_T         rfc_linestatus_ind;    
   RFC_PARNEG_REQ_T             rfc_parneg_req;    
   RFC_PARNEG_RES_T             rfc_parneg_res;    
   RFC_PARNEG_IND_T             rfc_parneg_ind;
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
   RFC_EX_PARNEG_IND_T          rfc_ex_parneg_ind;    
#endif
   RFC_PARNEG_CFM_T             rfc_parneg_cfm;    
   RFC_PORTNEG_REQ_T            rfc_portneg_req;    
   RFC_PORTNEG_RES_T            rfc_portneg_res;    
   RFC_PORTNEG_IND_T            rfc_portneg_ind;    
   RFC_PORTNEG_CFM_T            rfc_portneg_cfm;    
   RFC_TEST_REQ_T               rfc_test_req;    
   RFC_TEST_CFM_T               rfc_test_cfm;
   RFC_NSC_IND_T				rfc_nsc_ind;
} RFCOMM_UPRIM_T;/* autogen_makefile_ignore_this (DO NOT REMOVE THIS COMMENT) */ 
  
/*============================================================================*
Public Data
 *============================================================================*/
/* None */

/*============================================================================*
Public Functions
 *============================================================================*/
/* None */
          
#ifdef __cplusplus
}
#endif 
          
#endif /* ndef _RFCOMM_PRIM_H */
