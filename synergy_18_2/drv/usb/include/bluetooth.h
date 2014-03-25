/*******************************************************************************

    (c) CSR plc 2008     (C) COPYRIGHT Cambridge Consultants Ltd 1999

















*******************************************************************************/

#ifndef BLUESTACK__BLUETOOTH_H    /* Once is enough */
#define BLUESTACK__BLUETOOTH_H





#ifdef __cplusplus
extern "C" {
#endif

#include "sched/csr_types.h"

/*
  JBS: Added so the BlueLab debugger can quiz the firmware to find out
  if it's running the same version of RFCOMM.
*/

#define BLUESTACK_VERSION       32 /* CCL version number */

/*============================================================================*
Public Defines
*============================================================================*/
#define	LC_PRIM			1	/* Primitive segmentation */ 
#define LM_PRIM			2	/* These definitions are used in the */
#define HCI_PRIM		3	/* get_message and put_message in the */
#define DM_PRIM			4	/* uint16_t parameter. We are using it */
#define L2CAP_PRIM		5	/* as an event id */
#define	RFCOMM_PRIM		6
#define SDP_PRIM		7
#define BCSP_LM_PRIM    8
#define BCSP_HQ_PRIM	9	/* BC01 Host Querier (chip management) */
#define BCSP_BCCMD_PRIM	10	/* BC01 Command      (chip management) */
#define CALLBACK_PRIM	11	/* Used to indicate this is a callback primitive */
#ifdef BTCHIP
/*	TCS, BNEP, UDP and TCP already defined in host stack with other value		*/
#define TCS_PRIM        12
#define BNEP_PRIM       13
#define TCP_PRIM        14
#define UDP_PRIM        15
#endif
#define FB_PRIM		16
#define BCSP_VM_PRIM	17


/* Protocol Service Multiplexors */
#define SDP_PSM                 1
#define RFCOMM_PSM		3 

/*============================================================================*
Public Data Types
*============================================================================*/

/*
 * Bluetooth types
 */

typedef uint16_t    con_handle_t;       /* HCI Connection handle (14 bit) */
typedef uint8_t     reason_t;           /* Error codes (or reasons) */
typedef uint16_t    packet_type_t;      /* Packet type (DM1, DH1) */
typedef	uint16_t	bt_clock_offset_t;	/* Bluetooth clock offset */
typedef uint32_t	device_class_t;		/* Lower 3 bytes */
typedef uint8_t		page_scan_mode_t;	/* 3 bits from FHS */
typedef uint16_t    psm_t;

/* Added UUID's for use by security manager SP2 4 12 2000 */

typedef uint16_t uuid16_t; 

#define UUID16_SDP     ((uuid16_t)0x0001)
#define UUID16_RFCOMM  ((uuid16_t)0x0003)
#define UUID16_TCS_BIN ((uuid16_t)0x0005)
#define UUID16_L2CAP   ((uuid16_t)0x0100)
#define UUID16_IP      ((uuid16_t)0x0009)
#define UUID16_UDP     ((uuid16_t)0x0002)
#define UUID16_TCP     ((uuid16_t)0x0004)
#define UUID16_TCS_AT  ((uuid16_t)0x0006)
#define UUID16_OBEX    ((uuid16_t)0x0008)
#define UUID16_FTP     ((uuid16_t)0x000A)
#define UUID16_HTTP    ((uuid16_t)0x000C)
#define UUID16_WSP     ((uuid16_t)0x000E)

typedef struct
{
    uint24_t lap;		/* Lower Address Part 00..23 */
    uint8_t  uap;               /* upper Address Part 24..31 */
    uint16_t nap;		/* Non-significant    32..47 */
} BD_ADDR_T;

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
*============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      bd_addr_copy
 *
 *  DESCRIPTION
 *      Copy the source BD Address into the destination BD address.
 *
 *  RETURNS
 *      void 
 *
 *----------------------------------------------------------------------------*/

void bd_addr_copy(
    BD_ADDR_T *p_bd_addr_dest,
    const BD_ADDR_T *p_bd_addr_src
    );

/*----------------------------------------------------------------------------*
 *  NAME
 *      bd_addr_zero
 *
 *  DESCRIPTION
 *      Set the BD Address to all zeroes.
 *
 *  RETURNS
 *      void 
 *
 *----------------------------------------------------------------------------*/

void bd_addr_zero(
    BD_ADDR_T *p_bd_addr
    );

/*----------------------------------------------------------------------------*
 *  NAME
 *      bd_addr_eq
 *
 *  DESCRIPTION
 *      Compare two BD Addresses for equality.
 *
 *  RETURNS
 *      bool_t - TRUE if equal, FALSE if different
 *
 *----------------------------------------------------------------------------*/

bool_t bd_addr_eq(
    const BD_ADDR_T *p_bd_addr_1,
    const BD_ADDR_T *p_bd_addr_2
    );

#ifdef __cplusplus
}
#endif 

#endif /* ndef _BLUETOOTH_H */
