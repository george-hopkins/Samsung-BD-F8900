#ifndef _SDP_SDS_H_
#define _SDP_SDS_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sds.h

DESCRIPTION:       This file contains SDP SDS declarations.



****************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/
/*------------------------------------------------
 Configuration option defines
--------------------------------------------------*/

/*------------------------------------------------
     Attribute ID's
--------------------------------------------------*/
#define SERVICE_RECORD_HANDLE   0x0000
#define SERVICE_CLASS_ID_LIST   0x0001
#define SERVICE_ID              0x0003
#define PROTOCOL_DESC_LIST      0x0004
#define BROWSE_GROUP_LIST       0x0005
#define BSLUT_PROFILE_DESC_LIST 0x0009

/*------------------------------------------------
      Attribute ID's extensions
--------------------------------------------------*/

#define BASE_ATTRIBUTE_ID      0x0100
#define SERVICE_DESC           0x0001

#define VERSION_NUM_LIST_HIGH       0x02
#define VERSION_NUM_LIST_LOW        0x00

#define SERVICE_DATABASE_STATE_HIGH 0x02
#define SERVICE_DATABASE_STATE_LOW  0x01

#define SERVICE_CLASS_ID_LIST_HIGH  0x00
#define SERVICE_CLASS_ID_LIST_LOW   0x01
/*------------------------------------------------------
      Service discovery server attributes definitions
--------------------------------------------------*/

#define SERVICE_DISCOVERY_SERVER_RECORD_SIZE      0x18

#define DATA_ELEMENT_SERVER_TYPE                  0x35

#define DATA_ELEMENT_SERVER_SIZE                  0x03

#define SERVICE_DISCOVERY_SERVER_SERVICE_CLASS_ID_HIGH 0x10

#define SERVICE_DISCOVERY_SERVER_SERVICE_CLASS_ID_LOW  0x00

#define SERVICE_DISCOVERY_VERSION_NUMBER_MAJOR    0x01

#define SERVICE_DISCOVERY_VERSION_NUMBER_MINOR    0x00

#define SERVICE_DISCOVERY_SERVER_STATE            0x00

#define SERVICE_DISCOVERY_SERVER_INDEX_STATE      0x14

#define SERVICE_DISCOVERY_SERVER_INDEX_HANDLE     (0x06 + sizeof(CsrUint8*))
/*------------------------------------------------
     Attribute data index's
--------------------------------------------------*/
#define ATTRIBUTE_ID_INDEX          0x02
#define ATTRIBUTE_32BIT_DATA_INDEX  0x04
#define ATTRIBUTE_8BIT_DATA_INDEX   0x01
#define NEXT_ATTRIBUTE_ID           0x01
#define DATA_ELEMENT_TYPE_INDEX     0x01
#define ATTRIBUTE_HEADER_SIZE       0x01
#define DATA_ELEMENT_SEND_SIZE      0x03
#define DATA_ELEMENT_SEND_TYPE_16   0x36
#define SR_HANDLE_SIZE              0x08
#define SR_SIZE                     0x02
#define MIN_ATT_RESP_SIZE           0x04
#define MIN_SEARCH_ATT_RESP_SIZE    0x07
#define SEND_DE_HEAD_SIZE           0x03
#define DATA_ELEMENT_SEND_TYPE_32   0x37
#define DATA_ELEMENT_EXTRA_SIZE     0x02

/*------------------------------------------------
     PDU sizes
--------------------------------------------------*/
#define PDU_HEADER_SIZE         0x05
#define PDU_SRCH_TAIL_SIZE      0x05
#define PDU_HANDLE_SIZE         0x04
#define PDU_MAX_COUNT_SIZE      0x02
#define PDU_ATTR_TAIL_SIZE      0x03

/*------------------------------------------------
     Attribute search index's
--------------------------------------------------*/
#define ATTRIBUTE_SEARCH        0x02
#define ATTRIBUTE_SEARCH_RANGE  0x04

/*------------------------------------------------
     Continuation state size
--------------------------------------------------*/
#define CONTINUATION_SIZE         0x01
#define CONTINUATION_SIZE_16BIT   0x02
#define CONTINUATION_SIZE_32BIT   0x04

/*------------------------------------------------
     Data Element Size Descriptors
--------------------------------------------------*/
#define SIZE_INDEX_1BYTE    0x0000
#define SIZE_INDEX_2BYTES    0x0001
#define SIZE_INDEX_4BYTES    0x0002
#define SIZE_INDEX_8BYTES    0x0003
#define SIZE_INDEX_16BYTES    0x0004
#define SIZE_INDEX_8ABITS    0x0005
#define SIZE_INDEX_16ABITS    0x0006
#define SIZE_INDEX_32ABITS    0x0007

/*------------------------------------------------
     UUID header type / size
--------------------------------------------------*/
#define UUID_HEADER_16BITS        0x19
#define UUID_HEADER_32BITS        0x1A
#define UUID_HEADER_128BITS       0x1C
#define ATTRIBUTE_HEADER_8BITS    0x08
#define ATTRIBUTE_HEADER_16BITS   0x09
#define ATTRIBUTE_HEADER_32BITS   0x0A

/*------------------------------------------------
     Service Discovery Service Class ID
--------------------------------------------------*/
#define SERVICE_DISCOV_UUID   0x00001000

/*------------------------------------------------
     Data Element Header Masks
--------------------------------------------------*/
#define DATA_ELEMENT_SIZE_MASK  0x07
#define DATA_ELEMENT_TYPE_MASK  0xF8

#define DATA_ELEMNET_NULL      0x00
#define DATA_ELEMNET_INT       0x08
#define DATA_ELEMNET_TWO_COMP  0x10
#define DATA_ELEMNET_UUID      0x18
#define DATA_ELEMNET_TEXT      0x20
#define DATA_ELEMNET_BOOL      0x28
#define DATA_ELEMENT_SEQUENCE  0x30
#define DATA_ELEMENT_ALT       0x38
#define DATA_ELEMENT_URL       0x40

/*------------------------------------------------
    MTU offset
--------------------------------------------------*/
#define MTU_OFFSET   0x00

/*------------------------------------------------
    Data Element types
--------------------------------------------------*/
#define DE_TYPE_ERROR          0x00
#define DE_TYPE_SEQUENCE       0x01
#define DE_TYPE_NON_SEQUENCE   0x02

/*------------------------------------------------
    Data Element types
--------------------------------------------------*/
#define MAX_UINT_16            0xFFFF

/*------------------------------------------------
    Error response sizes
--------------------------------------------------*/
#define ERROR_PDU_SIZE        0xA
#define ERROR_DATA_SIZE       0x5

/*============================================================================*
Public Data Types
*============================================================================*/

/*------------------------------------------------
 SDS Connection Block Data Structure
--------------------------------------------------*/
typedef struct
{

    /* first connection */
    CsrUint8 *first_connection;

    /* current connection */
    CsrUint8 *current_connection;

    /* connection number */
    CsrUint8 connection_number;

    /* Service Record Handle */
    CsrUint32 SR_Handle;

    /* SDA phandle */
    CsrSchedQid sda_phandle;

    /* service record state */
    CsrUint8 sr_state;

#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    /* pre-defined service record number */
    CsrUint8 sr_dbase_number;
#endif

} SDSCON_T;

/*------------------------------------------------
 SDS Control Block Data Structure
--------------------------------------------------*/
typedef struct
{

    /* pointer to the next connection data item */
    CsrUint8 *next_connection;

    /* pointer to the previous connection data item */
    CsrUint8 *previous_connection;

    /* bd address */
    BD_ADDR_T bd_addr;

    /* Channel Identifier */
    l2ca_cid_t cid;

    /* state of the SDC */
    CsrUint8 state;

        /* MTU for packets we want to send over the air. */
        CsrUint16 mtu_tx;

    /* Search Pointer */
    CsrUint8 *srch_ptr;

    /* Search size */
    CsrUint16 srch_size;

    /* attribute size */
    CsrUint16 attr_size;

    /* Response Pointer */
    CsrUint8 *resp_ptr;

    /* Response Size */
    CsrUint16 resp_size;

    CsrBool is_waiting_for_l2c_datawrite_confirm;

    /* Transaction ID */
    CsrUint16 transaction_id;

    /* first linked list handle */
    CsrUint8 *handle_db_ptr;

    /* number of handles in linked list */
    CsrUint16 number_handles;

    /* number of handles left in linked list */
    CsrUint16 left_number_handles;

    /* current linked list handle */
    CsrUint8 *current_hdl_ptr;

    /* maximum number of response data */
    CsrUint16 max_num_rch;

    /* first linked list attribute */
    CsrUint8 *attribute_db_ptr;

    /* number of attributes in linked list */
    CsrUint16 number_attributes;

    /* response number of attributes in linked list */
    CsrUint16 response_number_attributes;

    /* number of attributes left in linked list */
    CsrUint16 left_number_attributes;

    /* size of attributes in linked list */
    CsrUint16 size_attributes;

    /* response size of attributes in linked list */
    CsrUint16 response_size_attributes;

    /* response size of data elements in linked list */
    CsrUint16 response_size_de;

    /* size of attributes left in linked list */
    CsrUint16 left_size_attributes;

    /* current linked list attribute */
    CsrUint8 *current_attribute_ptr;

    /* total data element header size */
    CsrUint16 total_de_head_size;

    /* left data element header size */
    CsrUint16 left_de_head_size;

    /* error communication */
    CsrUint16 error_com;

    /* error state */
    CsrBool error_state;

    /* error code */
    CsrUint16 error_code;

    /* continuation state set when a search response requires separating */
    CsrBool continuation_state;

} SDSCB_T;

/*------------------------------------------------
Service Record Linked List Structure
--------------------------------------------------*/
typedef struct
{
    /* current record */
    CsrUint8 *current_rc;

    /* first record */
    CsrUint8 *first_rc;

} SDSLL_T;

/*------------------------------------------------
Handle Response Linked List Structure
--------------------------------------------------*/
typedef struct
{
    /* record handle */
    CsrUint32 rchandle;

    /* first record */
    CsrUint8 *next_rch;

} SDSRCH_T;

/*------------------------------------------------
Attribute Response Linked List Structure
--------------------------------------------------*/
typedef struct
{
    /* attribute id */
    CsrUint8 *attrib_id;

    /* attribute size */
    CsrUint16 size;

    /* first record */
    CsrUint8 *next_att;

    /* data element header size */
    CsrUint16 de_head_size;

    /* total size of attributes in the sr handle to be sent */
    CsrUint16 total_attribute_size;

    /* service record handle */
    CsrUint32 srhandle;

    /* size of the attribute sent */
    CsrUint16 sent_size;

    /* size of the de header sent */
    CsrUint16 de_sent_size;

    /* size of the attribute to be sent */
    CsrUint16 send_size;

    /* size of the attribute not to be sent */
    CsrUint16 left_size;

} SDSATT_T;


/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
*============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *  void init_SDS(void)
 *
 *  DESCRIPTION
 *  Initialisation function
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void init_SDS(void);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Auto_Connect_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Connection Indication
 *  1. Save connection details
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Auto_Connect_Indication(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Connect Confirm.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Dataread Indication
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Datawrite Confirm
 *  1. test write result
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Indication
 *  1. determine current state
 *  2. If connected send disconnect response
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Disconnect_Confirm(SDSCB_T *sdscb, L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Confirm
 *  1. Test the current state
 *  2. If we are communicating us the state machine DISCONNECT IND to clear
 *     the connection/search and handle/attribute link lists and drop into
 *     the ready state
 *  3. If we are in any other state ignore the message.  We only issue a
 *     disconnect req in the communicating state.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Register_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received Register Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Register_Request(SDS_UPRIM_T *sds_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_UnRegister_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received UnRegister Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_UnRegister_Request(SDS_UPRIM_T *sds_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Config_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received Config Request
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Config_Request(SDS_UPRIM_T *sds_prim);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool sds_cid_match(cid_t cid)
 *
 *  DESCRIPTION
 *  Compare local CID with incoming CID
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool sds_cid_match(l2ca_cid_t cid);
#endif

#ifdef ENABLE_SHUTDOWN
/*----------------------------------------------------------------------------*
 *  NAME
 *      void sds_deinit()
 *
 *  DESCRIPTION
 *      Cleanup the SDS by freeing the records, handles and whatnot.
 *
 *  RETURNS/MODIFIES
 *      None
 *
 *---------------------------------------------------------------------------*/
void sds_deinit(void);
#endif

/* Functions needed for deinit */
extern void free_handle_list(void);
extern void free_attribute_list(void);
extern CsrUint32 store_service_record_handle(CsrUint8 *pointer);
#ifdef BUILD_FOR_HOST
extern void writePtrValue(CsrUint8 *toPtr, const CsrUint8 *ptrVal);
#endif

#ifdef __cplusplus
}
#endif

#endif

