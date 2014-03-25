#ifndef _SDP_SDC_H_
#define _SDP_SDC_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sdc.h

DESCRIPTION:       This file contains SDP SDC declarations.



****************************************************************************/

#include "csr_bt_bluestack_types.h"
#include "bluetooth.h"
#include "l2cap_prim.h"
#include "sdp_control.h"
#ifdef SDC_INCLUDE
#include "sdc_prim.h"
#endif
#include "csr_mblk.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/

/*------------------------------------------------
     UUID Size Descriptors
--------------------------------------------------*/
#define UUID_SIZE_16BITS    0x001
#define UUID_SIZE_32BITS    0x002
#define UUID_SIZE_128BITS   0x004

/*------------------------------------------------
     PDU sizes
--------------------------------------------------*/
#define PDU_HEAD_SIZE           0x05
#define PDU_TAIL_SIZE           0x03
#define PDU_ATTRIB_HANDLE_SIZE  0x04
#define PDU_CONTINUATION_NULL   0x00
#define PDU_ERROR_SIZE          0x07
#define PDU_MIN_SIZE            0x03

/*------------------------------------------------
     PDU index
--------------------------------------------------*/
#define PDU_HEADER_ID_INDEX        0x01
#define PDU_HEADER_LENGTH_INDEX    0x03

/*------------------------------------------------
     QOS flag defalt value
--------------------------------------------------*/
#define QOS_FLAGS_DEFAULT       0x00

/*------------------------------------------------
     PDU search / response timer
--------------------------------------------------*/
#define SEARCH_TIMER    (CSR_SCHED_SECOND * 30) /* 30 seconds */

/*------------------------------------------------
     bit index
--------------------------------------------------*/
#define BIT_16_INDEX        0x02

/*------------------------------------------------
     Set data to zero
--------------------------------------------------*/
#define SET_DATA_ZERO       0x00

/*============================================================================*
Public Data Types
*============================================================================*/

/*------------------------------------------------
 SDC Data structure
--------------------------------------------------*/
typedef struct
{
    /* BD address*/
    BD_ADDR_T bd_addr;

    /* Channel IDentifier */
    l2ca_cid_t cid;

    /* Pointer to data to be sent */
    void *pdu_ptr;

    /* size of data to be sent */
    CsrUint16 pdu_size;

    CsrBool is_waiting_for_l2c_datawrite_confirm;

    /* state of the SDC */
    CsrUint8 state;

    /* MTU for packets that we recive from air. */
    CsrUint16 mtu_rx;

    /* MTU for packets that we transmit to air. */
    CsrUint16 mtu_tx;

    /* connection state terminate primative flag */
    CsrBool connect_trm_prim;

    /* sds open search state */
    CsrBool sdc_open_search;

    /* SDA phandle for searching */
    CsrSchedQid sda_phandle_current;

    /* SDA number of phandle using connection */
    CsrUint8 sda_phandle_number;

    CsrUint16 transaction;    /* transaction ID number */

} SDCCB_T;

/*------------------------------------------------
 Data Element Search Data structure
--------------------------------------------------*/
typedef struct
{
    CsrUint8  srch_type;      /* type of search */
    CsrUint16 sch_size;       /* search pattern size */
    CsrUint8 *search_pttrn;   /* search pattern address */
    CsrUint16 att_size;       /* attribute list size */
    CsrUint8 *attrib_list;    /* attribute list address */
    CsrUint32 svc_rec_hndle;  /* service record handle */
    CsrUint16 return_max;     /* maximum records returned */
    CsrUint16 build_number;   /* build number */
    CsrUint16 rx_total;       /* Total bytes received */
    CsrMblk *p_response_head; /* Pointer to the head of the response list */
    CsrUint32 total_response; /* total size of the response to the upper */
                             /* layer */
    CsrUint16 response_max;   /* maximum record to be sent to the upper layer */
    CsrUint16 response_error;  /* error to be sent to the upper layer */
    CsrUint16 response_response;/* response state to the upper layer */
    CsrBool finished;         /* Indicates when the transaction has finished */
    CsrSchedTid evenCsrSchedTid;             /* search timer event id */
    CsrUint8 pdu_offset;     /* search size pdu offset */
} DE_TYPE_T;

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
*============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *  void init_SDC(void)
 *
 *  DESCRIPTION
 *  Initialisation function
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void init_SDC(void);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received a connection confirm
 *  1. examine the response type to determine if the a connection is successful
 *  2. pending or error
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Indication
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Confirm
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Datawrite Confirm
 *  1. test write result
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received dataread indication
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Search Request
 *  1. Store search details
 *  2. Start connection
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Search_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Attribute_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Attribute Request
 *  1. store attribute details
 *  2. Start connection
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Attribute_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Search_Attribute_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Search Attribute Request
 *  1. store search attribute details
 *  2. Start connection
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Search_Attribute_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Terminate_Primative_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Terminate Primative Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
void SDC_Terminate_Primative_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Open_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Open Search Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void SDC_Open_Search_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Close_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Close Search Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void SDC_Close_Search_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Config_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Config Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void SDC_Config_Request(SDC_UPRIM_T *sdc_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool sdc_cid_match(cid_t cid)
 *
 *  DESCRIPTION
 *  Compare the stored CID to the primative CID
 *
 *  RETURNS/MODIFIES
 *  CsrBool return_flag set to TRUE is the CID's match
 *
 *---------------------------------------------------------------------------*/

#ifdef SDC_INCLUDE
CsrBool sdc_cid_match(l2ca_cid_t cid);
#endif

#ifdef __cplusplus
}
#endif

#endif

