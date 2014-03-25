/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sds.c

DESCRIPTION:       This file contains the SDS Control functions.



****************************************************************************/

#include "csr_synergy.h"

/*============================================================================*
  ANSI C & System-wide Header Files
  *============================================================================*/
#include "csr_util.h"

#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "error.h" /* path removed */
#include "csr_bt_core_stack_fsm.h" /* path removed */
#include "csr_bt_tasks.h" /* path removed */
#include "csr_bt_core_stack_pmalloc.h" /* path removed */
#include "csr_mblk.h" /* path removed */


/*============================================================================*
  Interface Header Files
  *============================================================================*/
#include "l2cap_prim.h" /* path modified */
#include "l2caplib.h" /* path removed */
#include "sds_fsm_definitions.h" /* path removed */
#include "sdp_control.h" /* path removed */
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */
#include "sdp_sds.h" /* path removed */
#include "sdp_sdc.h" /* path removed */
#include "sdp_ts_interface.h" /* path removed */
#include "sdp_sda_interface.h"
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
#include "sdp_dbase.h" /* path removed */
#endif
/*============================================================================*
  Public Data
  *============================================================================*/
/* None */

/*============================================================================*
  Private Defines
  *============================================================================*/
#ifdef SDS_INCLUDE
SDSCB_T *sdscb;  /* SDS connection data pointer */
SDSLL_T sdsll;   /* SDS Linked List */
SDSCON_T sdscon; /* SDS connection link list structure */
#endif

#define CSR_SCHED_QID_INVALID_SDS      0x0000


/*============================================================================*
  Private Data Types
  *============================================================================*/
/* None */
/*============================================================================*
  Private Function Prototypes
  *============================================================================*/
#ifdef SDS_INCLUDE
static void SDS_sdscb_initialise(void);
#ifdef SDS_DEGUG_LOOK_AT_REC
static void look_at_rec(void);
#endif
static CsrUint8 Capture_search_pattrn(L2CA_DATAREAD_IND_T *sds_prim);
static void Create_Response(void);
static void Create_subsequent_Response(void);
static CsrBool find_search_match(void);
static CsrBool find_attr_match(void);
static CsrBool find_search_attr_match(void);
static void build_no_srch_resp(void);
static void build_no_attr_srch_resp(void);
static void build_sds_error_resp(CsrUint16 error_code);
static void build_srch_resp(void);
static void build_attr_srch_resp(CsrBool first_entry);
static void calculate_attribute_max_size(void);
static void calculate_de_size(void);
static CsrUint8 UUID_head_size(CsrUint8 *de_ptr);
static CsrUint8 Data_Element_Index(CsrUint8 *de_ptr);
static CsrBool search_database(CsrUint8 *srch_ptr, CsrUint16 curr_index);
static CsrBool search_attrib_types(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr);
static CsrBool search_rc_uuid(CsrUint32 *suuid,CsrUint8 *rec_ptr);
static void build_handle_list(CsrUint32 srhandle);
static CsrBool search_uuids(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr,
                           CsrUint16 cur_index);
static CsrBool find_service_record(CsrUint32 sr_handle);
static CsrBool find_attributes(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr,
                              CsrUint16 srch_size, CsrUint32 sr_handle);
static CsrBool get_attributes(CsrUint8 *rc_ptr, CsrUint32 sr_handle,
                             CsrUint16 low_attrib_id, CsrUint16 hi_attrib_id);
static void attrib_capture(CsrUint32 sr_handle, CsrUint8 *rc_ptr,
                           CsrUint16 size, CsrUint8 *sr_ptr);
static CsrBool search_attribute(void);
static void SDS_Reset_search_pattern(void);
static void SDS_Reset_search_response(void);
static CsrBool validate_ts_data(SDS_REGISTER_REQ_T *sds_prim);
static CsrBool validate_sds_dataread_ind(L2CA_DATAREAD_IND_T *prim);
static CsrBool check_input_data(CsrUint16 length,CsrUint8 *data);
static CsrBool step_though_sdc_data_element_sequence(CsrUint8 *data);
static CsrBool step_though_data_elements(CsrUint16 total_size, CsrUint8 *data);
static CsrBool step_though_service_record(CsrUint16 length, CsrUint8 *data);
static CsrBool step_though_data_sequence(CsrUint16 total_size, CsrUint8 *data);
static CsrUint8 data_element_type(CsrUint8 *data);
static void Register_SDS(void);
static void UpdateDatabaseState(void);
static CsrBool register_service_record(CsrUint16 prim_mem_size,
                                      CsrUint8 *prim_service_rec);
static void set_pointer_service_record(CsrUint8 *service_rec);
static CsrBool unregister_service_record(CsrUint32 srhandle);
static CsrBool store_uuid(CsrUint8 *pointer, CsrBool search, CsrUint32 *uuid);
static void store_transaction_id(CsrUint8 *pointer);
static CsrBool total_search_size(CsrUint16 length, CsrUint16 extra_size,
                                CsrUint8 *data, CsrUint8 offset);
static void SDS_free_search_data(void);
static void SDS_set_return_max(CsrUint8 offset);
static void SDS_add_continuation(CsrUint8 continuation_state_size,
                                 CsrUint16 data_size_left);
static void SDS_create_pdu_header(CsrUint8 header_type, CsrUint16 data_size);
static void SDS_create_de_header(CsrUint8 header_type, CsrUint16 data_index,
                                 CsrUint16 de_size);
static void SDS_create_connection_data(void);
static void SDS_clear_connection(void);
static void SDS_initialise_connection_data(void);
static CsrUint8 *SDS_find_next_service_record(CsrUint8 sr_handle_count,
                                             CsrUint8 *rc_ptr, CsrBool status_flag);
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
static CsrUint8 *SDS_create_static_service_record(CsrUint8 sr_handle_count);
static void SDS_clear_static_service_record(CsrUint8 sr_handle_count,
                                            CsrUint8 *rc_ptr);
static void SDS_check_static_service_records(CsrUint8 static_number);
#endif
static void SDS_datawrite_req(void);
static CsrUint16 Data_Element_Size(CsrUint8 *de_ptr);
#endif

/*============================================================================*
  Private Data
  *============================================================================*/
/* local mtu maximum transmission unit*/

/* we select the smallest MTU we can get away with since
   pool sizes are very limited.  This ensures that responses
   to things like Attribute range requests don't blow up the
   pool, since SDP will slice up the response to fit the MTU
*/
#ifdef SDS_INCLUDE
static CsrUint16 sds_mtu_rx = L2CA_MTU_MINIMUM;
#endif

/*============================================================================*
  Private Function Implementations
  *============================================================================*/

/*    copy pointer value in a platform independent manner. Used to copy pointer
      value to/from the service record                                        */
#ifdef BUILD_FOR_HOST
void writePtrValue(CsrUint8 *toPtr, const CsrUint8 *ptrVal)
{
    CsrUint16    i;

    for (i = 0; i < sizeof(CsrUint8 *); i++)
    {
        toPtr[i] = ptrVal[i];
    }
}
#endif


/*============================================================================*
  Public Function Implementations
  *============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *  void init_SDS(void)
 *
 *  DESCRIPTION
 *  Initialisation function
 *  1. clear the SDS data
 *  2. register the server service record
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void init_SDS(void)
{
    /* initialise the SDS data */
    SDS_initialise_connection_data();
    /* resister SDS on the server */
    Register_SDS();
    /* find the number of pre-registered service records, so we can */
    /* increment the start of the registered services records. So the */
    /* pre-defined service records are allocated service handles */
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    sdscon.sr_dbase_number = SDS_DBASE_number_of_records();
    /* test the pre-registered service records */
    SDS_check_static_service_records(sdscon.sr_dbase_number);
    /* set service record handle to the first no reserved handle */
    sdscon.SR_Handle = SR_HANDLE_START + sdscon.sr_dbase_number;
#else
    sdscon.SR_Handle = SR_HANDLE_START;
#endif

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Register_SDS(void)
 *
 *  DESCRIPTION
 *  Register SDS server as first record
 *  1. Create the memory for the service record
 *  2. Store the server service record.
 *  3. Register the service record.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void Register_SDS(void)
{
    CsrUint8 *sr_ptr;         /* service record pointer */
    CsrUint16 index = 0;      /* current service record index */
    CsrUint16 num_rec_bytes = SERVICE_DISCOVERY_SERVER_RECORD_SIZE; /* service*/
    /* record size */
    /* create the memory for the service record */
    sr_ptr = (CsrUint8 *)CsrPmemAlloc(num_rec_bytes);

    /* create server service record */

    /* service class id list attribute */
    sr_ptr[index++] = ATTRIBUTE_HEADER_16BITS;
    sr_ptr[index++] = SERVICE_CLASS_ID_LIST_HIGH;
    sr_ptr[index++] = SERVICE_CLASS_ID_LIST_LOW;

    sr_ptr[index++] = DATA_ELEMENT_SERVER_TYPE;
    sr_ptr[index++] = DATA_ELEMENT_SERVER_SIZE;

    sr_ptr[index++] = UUID_HEADER_16BITS;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_SERVICE_CLASS_ID_HIGH;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_SERVICE_CLASS_ID_LOW;

    /* version number list attribute */
    sr_ptr[index++] = ATTRIBUTE_HEADER_16BITS;
    sr_ptr[index++] = VERSION_NUM_LIST_HIGH;
    sr_ptr[index++] = VERSION_NUM_LIST_LOW;

    sr_ptr[index++] = DATA_ELEMENT_SERVER_TYPE;
    sr_ptr[index++] = DATA_ELEMENT_SERVER_SIZE;

    sr_ptr[index++] = ATTRIBUTE_HEADER_16BITS;
    sr_ptr[index++] = SERVICE_DISCOVERY_VERSION_NUMBER_MAJOR;
    sr_ptr[index++] = SERVICE_DISCOVERY_VERSION_NUMBER_MINOR;

    /* service database state attribute */
    sr_ptr[index++] = ATTRIBUTE_HEADER_16BITS;
    sr_ptr[index++] = SERVICE_DATABASE_STATE_HIGH;
    sr_ptr[index++] = SERVICE_DATABASE_STATE_LOW;

    sr_ptr[index++] = ATTRIBUTE_HEADER_32BITS;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_STATE;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_STATE;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_STATE;
    sr_ptr[index++] = SERVICE_DISCOVERY_SERVER_STATE + 1;

    /* register the server record */
    (void) register_service_record(num_rec_bytes, sr_ptr);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void UpdateDatabaseState(void)
 *
 *  DESCRIPTION
 *  Updates the server database state, to indicate a change to the data base
 *  has occurred.
 *  1. Test to make sure the server service record is present.
 *  2. Store the current server state, increment it indicating a change
 *     has taken place.
 *  3. Update the new server status in the server service record
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void UpdateDatabaseState(void)
{
    CsrUint8 *sr_ptr;                          /* service record pointer */
    CsrUint32 new_state = 0;                   /* current server state */

    /* test that the first service record is present */
    if (sdsll.first_rc != NULL)
    {
        /* set linklist first pointer */
        sr_ptr = sdsll.first_rc;

        /* set the service record pointer to the start of the attributes */
#ifdef BUILD_FOR_HOST
        writePtrValue( (CsrUint8 *) &sr_ptr, &(sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
/*        sr_ptr = (CsrUint8 *) SDP_UINT32_R( sr_ptr, SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE );*/
#else
        sr_ptr = (CsrUint8 *)sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE];
#endif
        /* capture the database state 32 bit value */
        new_state = SDP_UINT32_R(sr_ptr, SERVICE_DISCOVERY_SERVER_INDEX_STATE);

        /* increment state to indicate change in server */
        new_state++;

        /* write new state to database */
        SDP_UINT32_W(sr_ptr, SERVICE_DISCOVERY_SERVER_INDEX_STATE, new_state);
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Auto_Connect_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Connection Indication
 *  1. Test the connection has a valid CID number
 *  2. If the CID number already exists, reject the connection.
 *  3. If the CID number does not exist, accept the connection.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Auto_Connect_Indication(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_AUTO_CONNECT_IND_T *prim = (L2CA_AUTO_CONNECT_IND_T *)l2cap_prim;

    /* test the connection request is valid, by checking we have no CID */
    /* match with any other connection and there is no maintenance of the */
    /* service records taking place */
    if (SDS_ST_READY == sdscon.sr_state && !sds_cid_match(prim->cid))
    {
        /* we don't have a connection for this CID, so accept the connection */
        /* and initialise the structure to hold the data */
        /* create the connection data structure and initialise it */
        SDS_create_connection_data();
        /* store connection data */
        sdscb->bd_addr = prim->bd_addr;
        sdscb->cid = prim->cid;
        (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                             SDS_EV_CONNECT_IND, prim, 0);
    }
    else
    {
        /* we already have a connection with this CID, so reject the */
        /* connection request */
        /* make sure the pointer to the connection data is NULL */
        sdscb = NULL;
        /* send Connection Response rejected */
        L2CA_AutoConnectRsp(prim->identifier, prim->cid,
                L2CA_CONNECT_REJ_RESOURCES, 0, 0, NULL);
    }

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Configure Confirm
 *  1. test the status of the configure confirm.
 *     if the status is in error stop the connection.
 *     if the status is ok continue with the connection.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T *)l2cap_prim;

    /* test for correct SDS state */
    /* NB incorrect state for configure confirm is just ignored */
    if (sdscb->state == SDS_ST_CONNECTING)
    {
        /* test response */
        if (prim->result != L2CA_CONNECT_SUCCESS)
        {
            /* configure confirm status is no ok stop connection */
            (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                                 SDS_EV_CONNECT_CFM_NEG, prim, 0);
        }
        else /* response ok */
        {
            /* configure confirm status is ok continue with connection */
            (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                                 SDS_EV_CONNECT_CFM, prim, 0);
        }
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Dataread Indication
 *  1. Test the search data for incorrect formatting.
 *     If ok continue to search the service records.
 *     If not ok create an error response.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DATAREAD_IND_T *prim = (L2CA_DATAREAD_IND_T *)l2cap_prim;

    /* Convert MBLK into pmalloc, store back into primitive */
    CsrMblkSize mblk_length = CsrMblkGetLength(prim->data);
    CsrUint8 *data_ptr = CsrPmemAlloc(mblk_length);
    CsrMblkCopyToMemory(prim->data, 0, mblk_length, data_ptr);
    CsrMblkDestroy(prim->data);
    prim->data = (CsrMblk*)data_ptr;
    prim->length = mblk_length;

    /* test for correct SDS state */
    if (sdscb->state == SDS_ST_COMMUNICATING)
    {
        /* test the search data */
        if (validate_sds_dataread_ind(prim))
        {
            /* search data ok */
            /* search service records */
            (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                                 SDS_EV_DATAREAD_IND, 0, prim);
        }
        else /* search data error */
        {
            /* store the transaction ID */
            store_transaction_id(data_ptr + 1);

            /* create an error response indicating search data error */
            build_sds_error_resp(sdscb->error_code);

            /* send response data */
            SDS_datawrite_req();

            /* clear the error state and code */
            sdscb->error_state = FALSE;
            sdscb->error_code = 0;
        }
    }

    CsrPmemFree(prim->data);
    prim->data = NULL;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Datawrite Confirm
 *  1. test the status if the datawrite confirm, which indicates the status
 *     of the previous datawrite request.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DATAWRITE_CFM_T *prim = (L2CA_DATAWRITE_CFM_T *)l2cap_prim;

    /* NB incorrect states are ignored. */

    /* test result status for error */
    if (prim->result != L2CA_DATA_SUCCESS)
    {
        /* call the state machine with the datawite confirm negative */
        (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                             SDS_EV_DATAWRITE_CFM_NEG, 0, 0);
        BLUESTACK_WARNING(SDS_ERR);
    }
    else
    {
        /* call the state machine with the datawite confirm */
        (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                             SDS_EV_DATAWRITE_CFM, 0, 0);
    }

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Disconnect_Indication(SDSCB_T *sdscb, L2CA_UPRIM_T *l2cap_prim)
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
void SDS_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim)
{

    L2CA_DISCONNECT_IND_T *prim = (L2CA_DISCONNECT_IND_T *)l2cap_prim;

    /* test for the correct state, must be connected or communication for a */
    /* disconnect to take place. If no connection is present then we just   */
    /* ignore the 'cant disconnect error'.                                  */
    /* send disconnect response to L2CAP */
    L2CA_DisconnectRsp(prim->identifier, prim->cid);

    if (sdscb->state == SDS_ST_CONNECTING ||
        sdscb->state == SDS_ST_COMMUNICATING)
    {
        (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                             SDS_EV_DISCONNECT_IND, 0, 0);
    }
}
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
void SDS_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DISCONNECT_CFM_T *prim = (L2CA_DISCONNECT_CFM_T *)l2cap_prim;

    PARAM_UNUSED(prim);

    /* test for the correct state, if no connection is present then we
       just ignore the error.
    */
    if (sdscb->state == SDS_ST_COMMUNICATING)
    {
        /* use the state machine DISCONNECT IND to clear the */
        /* connection/search and handle/attribute link lists and drop into */
        /* the ready state */
        (void) fsm_16bit_run(&sds_fsm, &sdscb->state,
                             SDS_EV_DISCONNECT_IND, 0, 0);
    }
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Register_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received Register service record Request
 *  1. Test to see if there is no current connection and the service record
 *     data is valid.
 *  2. If there is no current connection and the service record data is valid,
 *     store the service record and send a confirm message.
 *  3. If there is a current connection or the service data is invalid,
 *     reject the service record.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Register_Request(SDS_UPRIM_T *sds_prim)
{
    SDS_REGISTER_REQ_T *prim = (SDS_REGISTER_REQ_T *)sds_prim;
    CsrUint16 error = SDS_ERROR_UNSPECIFIED;

    /* test there is no connection present, the state is correct */
    /* and the service record data is valid */
    if (SDS_ST_READY == sdscon.sr_state)
    {
        if (validate_ts_data(prim))
        {
            /* there is no connection and the data is valid, so register the */
            /* service record */
            /* store the phandle for reply */
            sdscon.sda_phandle = prim->phandle;
            /* service record data ok, store the service record */
            (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                    SDS_EV_REGISTER_REQ, 0, prim);

            return;
        }

        error = SDS_ERROR_INVALID_SR_DATA;
    }

    /* there is a current connection or the data is invalid, so reject */
    /* the service record */

    /* free the service record data if it is in RAM */
    bpfree(prim->service_rec);

    /* Send Register Confirm ERROR */
    Send_SDS_Register_Cfm(sdscon.SR_Handle, error, prim->phandle);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_UnRegister_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received UnRegister service record Request
 *  1. Test to see if there is a current connection.
 *  2. If there is no current connection, start to unregister the service
 *     record.
 *  3. If there is a current connection, reject the unregister request.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_UnRegister_Request(SDS_UPRIM_T *sds_prim)
{

    SDS_UNREGISTER_REQ_T *prim = (SDS_UNREGISTER_REQ_T *)sds_prim;

    /* Send UnRegister Confirm PENDING to get them used to the idea */
    Send_SDS_UnRegister_Cfm(prim->svc_rec_hndl, SDS_PENDING,
            prim->phandle);

    /* test there is no connection present, the state is correct and the */
    /* service record handle is not equal to the server/pre-registered */
    /* service records */
    if (SDS_ST_READY == sdscon.sr_state
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
            && (prim->svc_rec_hndl >=
                (CsrUint32)(SR_HANDLE_START + sdscon.sr_dbase_number))
#endif
       )
    {
        /* there is no current connection, so we can remove the service */
        /* record if it exists */
        /* store the phandle for reply */
        sdscon.sda_phandle = prim->phandle;
        /* correct state, remove the service record */
        (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                             SDS_EV_UNREGISTER_REQ, 0, prim);
    }
    else
    {
        /* there is a current connection, so we reject the service record */
        /* unregister request */
        /* Send UnRegister Confirm ERROR */
        Send_SDS_UnRegister_Cfm(prim->svc_rec_hndl, SDS_ERROR_UNSPECIFIED,
                prim->phandle);
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Config_Request(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Received Config Request
 *  1. Test that the primitive mtu value is greater than the minimum
 *     permitted value. If it is store the received mtu as the local mtu.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Config_Request(SDS_UPRIM_T *sds_prim)
{
    SDS_CONFIG_CFM_T *cfm_prim;

    /* set current MTU size */
    if (sds_prim->sds_config_req.l2cap_mtu >= L2CA_MTU_MINIMUM)
        sds_mtu_rx = sds_prim->sds_config_req.l2cap_mtu;

    /* _REQ and _CFM prims identical except for type. */
    cfm_prim = (SDS_CONFIG_CFM_T*)pcopy(sds_prim, sizeof(SDS_CONFIG_CFM_T));
    cfm_prim->type = SDS_CONFIG_CFM;

    /* Report actual MTU. */
    cfm_prim->l2cap_mtu = sds_mtu_rx;

    /* All flags currently ignored. */
    cfm_prim->flags = 0;

    Send_SDP_Message(cfm_prim);
}
#endif

/*============================================================================*
  State Machine Action Function Implementations
  *============================================================================*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_register_service(void *arg1, void *arg2)
{
    SDS_REGISTER_REQ_T *sdc_prim = (SDS_REGISTER_REQ_T *)arg2;

    PARAM_UNUSED(arg1);

    /*
      Register the service record received from the application,
      register the service record and reply with the appropriate response
    */

    /* test result of registration of service record */
    if (register_service_record(sdc_prim->num_rec_bytes,
                                sdc_prim->service_rec))
    {
        /* reply with register error */
        (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                             SDS_EV_MAINTENANCE_COMP_ERR, 0, 0);
    }
    else /* service record registered */
    {
        /* update the server record to indicate a change in data base */
        UpdateDatabaseState();
        /* reply with register error */
        (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                             SDS_EV_MAINTENANCE_COMP_OK, 0, 0);
    }

#ifdef SDS_DEGUG_LOOK_AT_REC
    look_at_rec();  /* look at the service records debug only */
#endif

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_unregister_service(void *arg1, void *arg2)
{
    SDS_UNREGISTER_REQ_T *sds_prim = (SDS_UNREGISTER_REQ_T *)arg2;

    PARAM_UNUSED(arg1);

    /*
       Unregister the service record received from the application,
       remove the service record from the data base and reply with the
       appropriate response
    */

    /* test result of un registration of service record */
    if (unregister_service_record(sds_prim->svc_rec_hndl))
    {
        /* reply with unregister error */
        (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                             SDS_EV_MAINTENANCE_COMP_UN_ERR, sds_prim, 0);
    }
    else /* service record found to match handle */
    {
        /* update the server record to indicate a change in data base */
        UpdateDatabaseState();
        /* reply with unregister ok */
        (void) fsm_16bit_run(&sds_fsm, &sdscon.sr_state,
                             SDS_EV_MAINTENANCE_COMP_UN_OK, sds_prim, 0);
    }
#ifdef SDS_DEGUG_LOOK_AT_REC
    look_at_rec(); /* look at the service records debug only */
#endif

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_connection_rsp(void *arg1, void *arg2)
{
    L2CA_AUTO_CONNECT_IND_T    *prim = (L2CA_AUTO_CONNECT_IND_T *)arg1;

    PARAM_UNUSED(arg2);

    /*
      A connection indication has been received from l2cap,
      so issue a reply of a connection response to continue with the
      connection.
    */

    /* send Connection Response Successful */
    L2CA_AutoConnectRsp(prim->identifier,
                        prim->cid,
                        L2CA_CONNECT_SUCCESS,
                        0,
                        SDP_L2CAP_CONFTAB_SIZE,
                        sdp_control_conftab(sds_mtu_rx));

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_connect_cfm_pos(void *arg1, void *arg2)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T *)arg1;

    PARAM_UNUSED(arg2);

    /* Check if the remote device has negotiated a lower mtu than the
       one we requested - in which case we should use it */
    sdscb->mtu_tx = prim->config.mtu;

    return SDS_EV_CONFIG_COMPLETE;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_datawrite_cfm(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
      L2CAP has sent data, therfore can send some more
    */

    if (sdscb->resp_ptr)
    {
        L2CA_DataWriteReq(sdscb->cid, sdscb->resp_size, sdscb->resp_ptr);
        sdscb->resp_ptr = NULL;

        sdscb->is_waiting_for_l2c_datawrite_confirm = TRUE;
    }
    else
    {
        sdscb->is_waiting_for_l2c_datawrite_confirm = FALSE;
    }


    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_datawrite_cfm_neg(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
      This function is part of the SDC state machine.
      Data Write Confirm with a negative state has been received from l2cap.
      Disconnect the connection as we can not send any data, also set the
      response error to send the message to the upper layer when the
      disconect cfm is recived
    */

    /* send disconnect request to close the connection */
    L2CA_DisconnectReq(sdscb->cid);

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_register_cfm_ok(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
      A register request has been received from the application,
      the process was successful so issue a reply with a pass status.
      Also sending the service record handle.
    */

    /* Send Register Confirm OK */
    Send_SDS_Register_Cfm(sdscon.SR_Handle-1, SDS_SUCCESS,
                          sdscon.sda_phandle);

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_register_cfm_err(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
      A register request has been received from the application,
      an error has been detected in the process so issue a reply
      with an error status.
    */
    /* set the error status and code */
    sdscb->error_state = FALSE;
    sdscb->error_code = 0;
    /* Send Register Confirm ERROR */
    Send_SDS_Register_Cfm(sdscon.SR_Handle, SDS_ERROR_UNSPECIFIED,
            sdscon.sda_phandle);

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_unregister_cfm_ok(void *arg1, void *arg2)
{
    SDS_UNREGISTER_REQ_T *sds_prim = (SDS_UNREGISTER_REQ_T *)arg1;

    PARAM_UNUSED(arg2);

    /*
      A unregister request has been received from the application,
      the process was successful so issue a reply with a pass status.
    */
    /* Send Un Register OK */
    Send_SDS_UnRegister_Cfm(sds_prim->svc_rec_hndl, SDS_SUCCESS,
                            sdscon.sda_phandle);

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_unregister_cfm_err(void *arg1, void *arg2)
{
    SDS_UNREGISTER_REQ_T *sds_prim = (SDS_UNREGISTER_REQ_T *)arg1;

    PARAM_UNUSED(arg2);

    /*
      A unregister request has been received from the application,
      an error has been detected with the process, so issue a reply
      with a error status.
    */

    /* Send Un Register ERROR */
    Send_SDS_UnRegister_Cfm(sds_prim->svc_rec_hndl, SDS_ERROR_UNSPECIFIED,
                            sdscon.sda_phandle);

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_free_memory(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
      A disconnection indication has been received,
      free all the outstanding memory allocations for the current search
      and connection.
    */

    /* clear response linked lists */
    free_handle_list();
    free_attribute_list();
    /* clear the connection data from the link list */
    SDS_clear_connection();

    return FSM_EVENT_NULL;
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
fsm_event_t sds_fn_search_response(void *arg1, void *arg2)
{
    L2CA_DATAREAD_IND_T *sds_prim = (L2CA_DATAREAD_IND_T *)arg2;
    CsrUint8 search_continuation_size = 0;/* search size of continuation data */

    PARAM_UNUSED(arg1);

    /*
      Create the search response for the search request.
      Store the search pattern data.
      Check the continuation state data in the search pattern.
      If the search contains an error PDU, close the connection.
      If there is no search pattern continuation data, start the search from
      scratch.
      If there is search pattern continuation data, the search has been
      performed and we just have to send the appropriate data in response.
      If we are expecting search continuation data and we got none, clear
      all the search/response data and send an error PDU.
    */

    /* Capture search data */
    search_continuation_size = Capture_search_pattrn(sds_prim);

    if (sdscb->error_com != 0)
    {
        /* an error PDU has been sent by SDC */
        /* so free any lists of data held for the response */
        free_handle_list();
        free_attribute_list();

        /* send disconnect request */
        L2CA_DisconnectReq(sdscb->cid);

        /* sets the search data */
        SDS_Reset_search_pattern();
    }
    else if (search_continuation_size == 0 && !sdscb->continuation_state)
    {
        /* we are expecting a new search and the continuation size is zero */
        /* Create search response data */
        Create_Response();
    }
    else if (search_continuation_size != 0 && sdscb->continuation_state)
    {
        /* we are expecting a continued search and the continuation size */
        /* indicates the search requires more data */
        /* Create search response data */
        Create_subsequent_Response();
    }
    else if (search_continuation_size != 0 && !sdscb->continuation_state)
    {
        /* we are not expecting a continued search but we have received one */
        /* store the transaction ID */
        store_transaction_id(sdscb->srch_ptr+1);
        /* build the error response with continuation error */
        build_sds_error_resp(PDU_ERROR_CONT_STATE);
        /* send response data */
        SDS_datawrite_req();
        /* sets the search data */
        SDS_Reset_search_response();
    }
    else
    {
        /* we are expecting a follow on search to continue to retrieve the */
        /* search response data, but there is no continuation state data */
        /* so we need to free the last search data and start a new search */
        /* so free any lists of data held for the previous response */
        free_handle_list();
        free_attribute_list();
        /* just set the response items in the search */
        SDS_Reset_search_response();
        /* Create search response data */
        Create_Response();
    }

    /* clear error state and code to stop any subsequent correct search */
    /* from failing in error */
    sdscb->error_state = FALSE;
    sdscb->error_code = 0;

    /* free the search data */
    SDS_free_search_data();

    return FSM_EVENT_NULL;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_sdscb_initialise(void)
 *
 *  DESCRIPTION
 *  Initialise sdscb structure connection/search information
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_sdscb_initialise(void)
{
    /* set SDC state */
    sdscb->state = SDS_ST_READY;

    /* set search size and pointer */
    sdscb->srch_size = 0;
    sdscb->srch_ptr = NULL;
    sdscb->attr_size = 0;


    /* set bd address */
    sdscb->bd_addr.lap = 0;
    sdscb->bd_addr.uap = 0;
    sdscb->bd_addr.nap = 0;

    /* set CID */
    sdscb->cid = 0;

    /* set response size and pointer */
    sdscb->resp_ptr = NULL;
    sdscb->resp_size = 0;
    sdscb->is_waiting_for_l2c_datawrite_confirm = FALSE;

    /* set the search data */
    SDS_Reset_search_pattern();

    /* set error state */
    sdscb->error_state = FALSE;
    /* set error code */
    sdscb->error_code = 0;

    /* set the link to the next connection data to NULL */
    sdscb->next_connection = NULL;
    /* set the link to the previous connection data */
    sdscb->previous_connection = NULL;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool sds_cid_match(cid_t cid)
 *
 *  DESCRIPTION
 *  Compare local CID with incoming prim CID to determine if message is for SDS
 *  1. Test to see if there is a any connection data.
 *  2. If there is connection data, start at the first item and step through
 *     the link list of connection structures items, until no more connection
 *     data items exist.
 *  3. If a CID match is found, set the return flag to TRUE to indicate there
 *     is a CID match, set the current connection pointer to the connection data
 *     and stop looking for any more items.
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool cid matched response
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool sds_cid_match(l2ca_cid_t cid)
{
    CsrBool return_flag = FALSE; /* set return flag, for error response */
    SDSCB_T *sdscb_temp;        /* temporary SDS structure pointer */

    /* test to see if there is any connection data */
    if (NULL != sdscon.first_connection)
    {
        /* there is connection data */
        /* initialise the local connection data pointer */
        sdscb_temp = (SDSCB_T *)sdscon.first_connection;
        /* step through the connection data to find the CID match */
        do
        {
            /* test the connection CID against the request CID */
            if (sdscb_temp->cid == cid)
            {
                /* set the return flag to TRUE as we have found the correct */
                /* CID */
                return_flag = TRUE;
                /* set the connection data pointer to the correct data, */
                /* so we can access the correct data */
                sdscb = sdscb_temp;
                /* set the temporary pointer to NULL to stop any further */
                /* search */
                sdscb_temp = NULL;
            }
            else
            {
                /* point to the next connection data item */
                sdscb_temp = (SDSCB_T *)sdscb_temp->next_connection;
            }
        }while(NULL != sdscb_temp);
    }

    return return_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void look_at_rec(void)
 *
 *  DESCRIPTION
 *  Looks a the service records held in the database, by stepping through db
 *  for debug only
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
#ifdef SDS_DEGUG_LOOK_AT_REC
void look_at_rec(void)
{
    CsrUint8 *sr_ptr;            /* service record pointer */
    CsrUint16 data_size;         /* size of data */
    CsrMblk *sr_blk = NULL;      /* service record block */

    /* start at first service record */
    sr_ptr = (CsrUint8 *)sdsll.first_rc;

    /* test for service records */
    if (sr_ptr != NULL) /* service record present */
    {
        /* step through the service records */
        do
        {
            /* store the index to the size of the record */
            /* store the size of the record */
            data_size = SDP_UINT16_R(sr_ptr, sizeof(CsrUint8 *));
            /* step though the service record data */

            /* NOTE: JS - This next bit of code looks to be complete crap */
            /* set up the header sr_blk */
            sr_blk = mblk_new_pmalloc (0);
            sr_blk->pData = (CsrUint8 *)sr_ptr;
            sr_blk->pRelease = (CsrUint8 *)sr_ptr;
            sr_blk->length = data_size + SR_SIZE + sizeof(CsrUint8 *);

            /* free all the data */
            CsrPmemFree(sr_blk);
            /* copy pointer to the next service record */
            (void) CsrMemCpy(&sr_ptr, sr_ptr, sizeof(CsrUint8 *));

        }
        while(sr_ptr != NULL); /* while there are service records */
    }

}
#endif
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 Capture_search_pattrn(L2CA_DATAREAD_IND_T *sds_prim)
 *
 *  DESCRIPTION
 *  captures the search pattern, by taking a copy and freeing the communication
 *  pattern
 *
 *
 *  RETURNS/MODIFIES
 *  none
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint8 Capture_search_pattrn(L2CA_DATAREAD_IND_T *sds_prim)
{
    CsrUint16 size = 0;                   /* data element size */
    CsrUint8 search_continuation_size = 0;/* search size of continuation data */

    /* store the size of the search data */
    sdscb->srch_size = sds_prim->length;

    /* set the search pointer to the primitive data pointer */
    sdscb->srch_ptr = (CsrUint8*)(sds_prim->data);
    sds_prim->data = NULL;

    /* find the max size of response data */
    switch (sdscb->srch_ptr[0])
    {
        case SDP_SERVICE_SEARCH_REQUEST:
            /* maximum service record count */
            sdscb->max_num_rch = SDP_UINT16_R(sdscb->srch_ptr,
                    (CsrUint16)(sdscb->srch_size - PDU_TAIL_SIZE));

            /* set search size as a Service Search */
            size = Data_Element_Size(sdscb->srch_ptr + PDU_HEADER_SIZE ) +
                PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE + DATA_ELEMENT_TYPE_INDEX;
            /* store continuation data size */
            search_continuation_size = sdscb->srch_ptr[size];
            break;
        case SDP_SERVICE_ATTRIBUTE_REQUEST:
            /* maximum attribute byte count */
            sdscb->max_num_rch = SDP_UINT16_R(sdscb->srch_ptr, (PDU_HEADER_SIZE +
                                                                PDU_HANDLE_SIZE));

            /* set the size to point to the continuation data */
            size = Data_Element_Size(sdscb->srch_ptr + PDU_HEADER_SIZE +
                                     PDU_MAX_COUNT_SIZE + PDU_HANDLE_SIZE) +
                PDU_HEADER_SIZE + PDU_HANDLE_SIZE +
                PDU_MAX_COUNT_SIZE + DATA_ELEMENT_TYPE_INDEX;
            /* store continuation data size */
            search_continuation_size = sdscb->srch_ptr[size];
            break;
        case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:


            /* calculate search size */
            sdscb->srch_size = Data_Element_Size(sdscb->srch_ptr + PDU_HEADER_SIZE) +
                PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE + DATA_ELEMENT_TYPE_INDEX +
                DATA_ELEMENT_TYPE_INDEX;
            /* search size plus calculate a attribute search size */
            size = sdscb->srch_size + Data_Element_Size(sdscb->srch_ptr +
                                                        (sdscb->srch_size - DATA_ELEMENT_TYPE_INDEX));
            /* store continuation data size */
            search_continuation_size = sdscb->srch_ptr[size];

            /* store the size of the search attribute data */
            sdscb->attr_size = size;
            /* Temporarily set the maximum attribute byte count high for handle */
            /* search, so we capture all the handles */
            sdscb->max_num_rch = MAX_UINT_16;
            break;
        case SDP_ERROR_RESPONSE:

            /* capture error parameter */
            sdscb->error_com = SDP_UINT16_R(sdscb->srch_ptr, PDU_HEAD_SIZE);

            break;
        default:
            /* error  */
            break;
    };

    return search_continuation_size;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Create_Response(void)
 *
 *  DESCRIPTION
 *  Initiates the responses to a request for information from the database
 *  1) determines the type of search requested
 *  2) initiates a database search
 *  3) build an appropriate response
 *  4) sends the response
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void Create_Response(void)
{
    /* type of search */
    switch (sdscb->srch_ptr[0])
    {
        case SDP_SERVICE_SEARCH_REQUEST:

            /* test a match for the search */
            if (find_search_match() && !sdscb->error_state)
            {
                /* no error has occurred during the search */

                /* Temporarily set the max number of handle to be returned to */
                /* the maximum size of the handles to be returned. So we can */
                /* calculate the max handles that can be returned in the maximum */
                /* MTU size */
                sdscb->max_num_rch = sdscb->max_num_rch * PDU_HANDLE_SIZE;
                /* Calculate the maximum number of handles that can be returned */
                /* within the maximum MTU size */
                SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                                   PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE_16BIT +
                                   CONTINUATION_SIZE);
                /* reset the number of handles that can be returned, back to the */
                /* number of handles than can be returned for the maximum size */
                /* of handles that can the returned */
                sdscb->max_num_rch = sdscb->max_num_rch / PDU_HANDLE_SIZE;
                /* set the current handle pointer */
                sdscb->current_hdl_ptr = sdscb->handle_db_ptr;
                /* create response */
                build_srch_resp();
                /* send response data */
                SDS_datawrite_req();
            }
            else if (!sdscb->error_state) /* no match found during search */
            {
                /* no error has occurred during the search but there is no match */
                /* so free any lists of data held for the response */
                free_handle_list();
                /* create empty search response */
                build_no_srch_resp();
                /* send response data */
                SDS_datawrite_req();
                /* sets the search data */
                SDS_Reset_search_pattern();
            }
            break;
        case SDP_SERVICE_ATTRIBUTE_REQUEST:

            /* test a match for the search */
            if (find_attr_match() && !sdscb->error_state)
            {
                /* no error has occurred during the search */

                /* Calculate the maximum size of attributes that can be returned */
                /* within the maximum MTU size */
                SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                                   CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);
                /* set the current attribute pointer */
                sdscb->current_attribute_ptr = sdscb->attribute_db_ptr;
                /* create response */
                build_attr_srch_resp(TRUE);
                /* send response data */
                SDS_datawrite_req();
            }
            else if (!sdscb->error_state)/* no match found during search */
            {
                /* no error has occurred */
                /* so free any lists of data held for the response */
                free_attribute_list();
                /* no error has occurred during the search but there is no match */
                /* create empty attribute response */
                build_no_attr_srch_resp();
                /* send response data */
                SDS_datawrite_req();
                /* sets the search data */
                SDS_Reset_search_pattern();
            }
            break;
        case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:

            /* test a match for the search */
            if (find_search_attr_match() && !sdscb->error_state)
            {
                /* no error has occurred during the search */

                /* Calculate the maximum size of attributes that can be returned */
                /* within the maximum MTU size */
                SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                                   CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);
                /* set the current attribute pointer */
                sdscb->current_attribute_ptr = sdscb->attribute_db_ptr;
                /* create response */
                build_attr_srch_resp(TRUE);
                /* send response data */
                SDS_datawrite_req();
            }
            else if (!sdscb->error_state)/* no match found during search */
            {
                /* no error has occurred */
                /* so free any lists of data held for the response */
                free_handle_list();
                free_attribute_list();
                /* no error has occurred during the search but there is no match */
                /* create empty attribute search response */
                build_no_attr_srch_resp();
                /* send response data */
                SDS_datawrite_req();
                /* sets the search data */
                SDS_Reset_search_pattern();
            }
            break;
        default:
            /* error  */
            break;
    };

    /* test for error state set during the search */
    if (sdscb->error_state)
    {
        /* an error has occurred */
        /* so free any lists of data held for the response */
        free_handle_list();
        free_attribute_list();
        /* build an error response */
        build_sds_error_resp(sdscb->error_code);
        /* send response data */
        SDS_datawrite_req();
        /* sets the search data */
        SDS_Reset_search_pattern();
    }
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void Create_subsequent_Response(void)
 *
 *  DESCRIPTION
 *  Initiates the responses to a request for information from the database,
 *  for the data that was missed from the first response
 *  1) determines the type of request
 *  2) initiates a database search
 *  3) build an appropriate response
 *  4) sends the response
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void Create_subsequent_Response(void)
{
    /* store the transaction ID */
    store_transaction_id(sdscb->srch_ptr+1);

    /* select request type */
    switch (sdscb->srch_ptr[0])
    {
        case SDP_SERVICE_SEARCH_REQUEST:


            /* Temporarily set the max number of handle to be returned to the */
            /* maximum size of the handles to be returned. So we can calculate */
            /* the max handles that can be returned in the maximum MTU size */
            sdscb->max_num_rch = sdscb->max_num_rch * PDU_HANDLE_SIZE;
            /* Calculate the maximum number of handles that can be returned */
            /* within the maximum MTU size */
            SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                               PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);
            /* reset the number of handles that can be returned, back to the */
            /* number of handles than can be returned for the maximum size of */
            /* handles that can the returned */
            sdscb->max_num_rch = sdscb->max_num_rch / PDU_HANDLE_SIZE;
            /* create search response */
            build_srch_resp();
            /* send response data */
            SDS_datawrite_req();
            break;
        case SDP_SERVICE_ATTRIBUTE_REQUEST:


            /* Calculate the maximum size of attributes that can be returned */
            /* within the maximum MTU size */
            SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                               CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);
            /* create attribute response indicating this is not the first time */
            build_attr_srch_resp(FALSE);
            /* send response data */
            SDS_datawrite_req();
            break;
        case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:


            /* set maximum attribute byte count to the correct value */
            sdscb->max_num_rch = SDP_UINT16_R(sdscb->srch_ptr,
                    (CsrUint16)(sdscb->srch_size - PDU_TAIL_SIZE));
            /* Calculate the maximum size of attributes that can be returned */
            /* within the maximum MTU size */
            SDS_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                               CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);
            /* create attribute search response indicating this is not the first */
            /* time */
            build_attr_srch_resp(FALSE);
            /* send response data */
            SDS_datawrite_req();
            break;
        default:
            /* error  */
            break;
    };
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool find_search_match(void)
 *
 *  DESCRIPTION
 *  initiate a search of the database to find UUID match
 *  for SDP_SERVICE_SEARCH_REQUEST
 *  1. Store the transaction ID
 *  2. Calculate the index to the search data
 *  3. Search the service record database for a UUID match
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool match found
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool find_search_match(void)
{
    CsrBool rnt_flag = FALSE;              /* set return flag, for failure */
                                          /* response */
    CsrUint16 index = PDU_HEADER_SIZE;     /* data element index */

    /* store the transaction ID */
    store_transaction_id(sdscb->srch_ptr+1);


    /* calculate index */
    index += Data_Element_Index(sdscb->srch_ptr + PDU_HEADER_SIZE);

    /* search for match in service records */
    rnt_flag = search_database(sdscb->srch_ptr + index, index);

    /* return match status */
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool find_attr_match(void)
 *
 *  DESCRIPTION
 *  initiate a search of the database to find attributes
 *  1. store the transaction is and the service record handle
 *  2. search for the attributes requested from the service record requested
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool match found
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool find_attr_match(void)
{
    CsrBool rnt_flag = FALSE; /* set return flag, for failure response */
    CsrUint32 sr_handle = 0;  /* service record handle */

    /* store the transaction ID */
    store_transaction_id(sdscb->srch_ptr+1);

    /* store search service record handle */
    sr_handle = store_service_record_handle(sdscb->srch_ptr + PDU_HEADER_SIZE);


    /* search for the attributes requested */
    rnt_flag = find_service_record(sr_handle);

    /* return match status */
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool find_search_attr_match(void)
 *
 *  DESCRIPTION
 *  Initiate a search of the database to find UUID match
 *  1. Search though the service records to find UUID's match with the search
 *     UUID's.
 *  2. Search though the service records matched above to find attribute's
 *     match for the search attribute's.
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool match found
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool find_search_attr_match(void)
{
    CsrBool rnt_flag = FALSE;              /* set return flag, for failure */
                                          /* response */
    CsrUint16 index = PDU_HEADER_SIZE;     /* data element  index */

    /* store the transaction ID */
    store_transaction_id(sdscb->srch_ptr+1);

    /* calculate index */
    index += Data_Element_Index(sdscb->srch_ptr + PDU_HEADER_SIZE);

    /* search for match in service records */
    rnt_flag = search_database(sdscb->srch_ptr + index, index);

    /* search though the handles found to find attributes */
    rnt_flag = search_attribute();

    /* return match status */
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void build_no_srch_resp(void)
 *
 *  DESCRIPTION
 *  Build the search response to the search request, no data found
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_no_srch_resp(void)
{
    /* set response size 10 bytes for no data */
    sdscb->resp_size = ERROR_PDU_SIZE;

    /* create the memory for the search response */
    sdscb->resp_ptr = (CsrUint8 *)CsrPmemAlloc(sdscb->resp_size);

    /* create the PDU header of the response to SDC */
    SDS_create_pdu_header(SDP_SERVICE_SEARCH_RESPONSE, ERROR_DATA_SIZE);

    /* PDU PARAMETERS */
    /* Total Service Record Count byte 2 */
    sdscb->resp_ptr[5] = SET_DATA_ZERO;
    /* Total Service Record Count byte 1 */
    sdscb->resp_ptr[6] = SET_DATA_ZERO;
    /* Current Service Record Count byte 2 */
    sdscb->resp_ptr[7] = SET_DATA_ZERO;
    /* Current Service Record Count byte 1 */
    sdscb->resp_ptr[8] = SET_DATA_ZERO;
    /* no Service Record Handle List */
    /* Continuation State */
    sdscb->resp_ptr[9] = PDU_CONTINUATION_NULL;

}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void build_no_attr_srch_resp(void)
 *
 *  DESCRIPTION
 *  Build the attribute search response to the attribute search request or
 *  attribute request, no data found
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_no_attr_srch_resp(void)
{

    /* set response size 10 bytes for no data */
    sdscb->resp_size = ERROR_PDU_SIZE;

    /* create the memory for the search response */
    sdscb->resp_ptr = (CsrUint8 *)CsrPmemAlloc(sdscb->resp_size);

    /* create the PDU header of the response to SDC */
    SDS_create_pdu_header((CsrUint8)(sdscb->srch_ptr[0] + 1), ERROR_DATA_SIZE);

    /* PDU PARAMETERS */
    /* Attribute List byte Count byte 2 */
    sdscb->resp_ptr[5] = SET_DATA_ZERO;
    /* Attribute List byte Count byte 1 */
    sdscb->resp_ptr[6] = BIT_16_INDEX;
    /* Service Attribute List byte 2 */
    sdscb->resp_ptr[7] = DATA_ELEMENT_SERVER_TYPE;
    /* Service Attribute List byte 1 */
    sdscb->resp_ptr[8] = SET_DATA_ZERO;
    /* Continuation State */
    sdscb->resp_ptr[9] = PDU_CONTINUATION_NULL;

}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void build_sds_error_resp(void)
 *
 *  DESCRIPTION
 *  Build the error response to be sent to SDC
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_sds_error_resp(CsrUint16 error_code)
{

    /* set response size 10 bytes for no data */
    sdscb->resp_size = PDU_ERROR_SIZE;

    /* create the memory for the search response */
    sdscb->resp_ptr = (CsrUint8 *)CsrPmemAlloc(sdscb->resp_size);

    /* create the PDU header of the response to SDC */
    SDS_create_pdu_header(SDP_ERROR_RESPONSE,BIT_16_INDEX);

    /* PDU PARAMETERS */
    /* PDU error code */
    SDP_UINT16_W(sdscb->resp_ptr, PDU_HEAD_SIZE, error_code);

}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  build_srch_resp(void)
 *
 *  DESCRIPTION
 *  Build the search response to the service search request
 *  1. Test that the number of service handles can be sent within the response.
 *     If the handles can't be sent in one response, calculate the number that
 *     can and set the continuation state.  If the handles can be sent in one
 *     response just set the number of handles the be sent to the number of
 *     handles discovered in the search.
 *  2. Build the PDU header information.
 *  3. Step though the link list of service record handles to be sent and store
 *     the handles in the response PDU. Set the current link list pointer to
 *     the last handle in the response, so if there are more handles to be sent
 *     we know where to re-start.
 *  4. Add the continuation state at the end of the PDU.
 *
 *
 *  RETURNS/MODIFIES
 *  CsrBool return state
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_srch_resp(void)
{
    CsrUint16 index = 0;                   /* current index */
    CsrUint16 index_step = 0;              /* next handle index step */
    CsrUint16 handles_to_send = 0;         /* number of handle to be sent */
    CsrUint8 continuation_state_size = 0;  /* size of the continuation state */
    SDSRCH_T *last_rch_ptr;               /* next handle pointer */


    /* test all the handles fit into the maximum return number of handles */
    if (sdscb->left_number_handles > sdscb->max_num_rch )
    {
        /* all the search handles won't fit, so we have to calculate how may */
        /* will in this response */
        /* set the number of handles to be sent */
        handles_to_send = sdscb->max_num_rch;
        /* store the number of handles left */
        sdscb->left_number_handles -= handles_to_send;
        /* test to find what kind of continuation state size is required */

        /* set the continuation state size  16 bit version*/
        continuation_state_size = CONTINUATION_SIZE_16BIT;

        /* set the continuation state */
        sdscb->continuation_state = TRUE;
    }
    else
    {
        /* all remaining handles will fit into this response */
        /* set the number of handles to be sent */
        handles_to_send = sdscb->left_number_handles;
        /* set the continuation state */
        sdscb->continuation_state = FALSE;
    }

    /* store response size */
    sdscb->resp_size = (CsrUint16)(handles_to_send * PDU_HANDLE_SIZE) +
        PDU_HEADER_SIZE + PDU_SRCH_TAIL_SIZE +
        continuation_state_size;


    /* create the memory for the search response */
    sdscb->resp_ptr = (CsrUint8 *)CsrPmemAlloc(sdscb->resp_size);

    /* create the PDU header of the response to SDC */
    SDS_create_pdu_header(SDP_SERVICE_SEARCH_RESPONSE,
                          (CsrUint16)(sdscb->resp_size - PDU_HEADER_SIZE));

    /* PDU PARAMETERS */
    /* Total Service Record Count */
    SDP_UINT16_W(sdscb->resp_ptr, PDU_HEAD_SIZE, sdscb->number_handles);

    /* start at first handle */
    last_rch_ptr = (SDSRCH_T *)sdscb->current_hdl_ptr;

    /* Current Service Record Count */
    SDP_UINT16_W(sdscb->resp_ptr, (PDU_HEAD_SIZE + PDU_MAX_COUNT_SIZE),
                 handles_to_send);

    /* step though the service record handle link list */

    /* index through service record handles */
    for (index = 0;index < handles_to_send;index++)
    {
        /* store Service record handle */
        SDP_UINT32_W(sdscb->resp_ptr,
                     (CsrUint16)(index_step + PDU_HEAD_SIZE +
                      PDU_MAX_COUNT_SIZE + PDU_MAX_COUNT_SIZE),
                     last_rch_ptr->rchandle);

        /* step to next handle */
        last_rch_ptr = (SDSRCH_T *)last_rch_ptr->next_rch;
        /* increment step to next handle in response */
        index_step += PDU_HANDLE_SIZE;
    }
    /* set current pointer to the next attribute */
    sdscb->current_hdl_ptr = (CsrUint8 *)last_rch_ptr;

    /* create the continuation data at the end of the response to SDC */
    SDS_add_continuation(continuation_state_size,
                         (CsrUint16)(sdscb->left_number_handles * PDU_HANDLE_SIZE));
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void calculate_attribute_max_size(void)
 *
 *  DESCRIPTION
 *  calculate attribute size to be sent, if the attributes exceed the maximum
 *  1. step though the attributes to be sent, adding up the sizes of the
 *     data and headers.  Until the size exceeds the maximum size to be sent
 *     in a single PDU.
 *  2. If there is more than one attribute, remove the last one to make sure
 *     the attributes that are included in the PDU are within the size
 *     restriction.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void calculate_attribute_max_size(void)
{
    CsrUint16 index = 0;       /* current index */
    CsrUint16 index_size = 0;  /* current index size */
    CsrUint16 de_size = 0;     /* data element size */
    SDSATT_T *attr_ptr;       /* next attribute pointer */
    SDSATT_T *last_rch_ptr;   /* last attribute pointer */

    /* start with current attribute */
    attr_ptr = (SDSATT_T *)sdscb->current_attribute_ptr;

    /* step though the stored attributes until size of attributes */
    /* exceeds maximum protect against the falling off the end of the */
    /* attributes by testing for a null pointer */
    do
    {
        /* test to see if the current attribute has been divided between */
        /* two responses eg there is some data left to send */
        if (attr_ptr->left_size > 0)
        {
            /* there is some data left to send in the current attribute, */
            /* so add size of this data left to send to the total to send */
            /* in this response. */
            /* we have all ready sent the de header if there was one, */
            /* so we don't need to add the current de header to the total */
            /* de header size */
            /* store the size of data and header to include the current */
            /* attribute */
            index_size += attr_ptr->left_size;
        }
        else
        {
            /* add the complete size of data and de header to include the */
            /* current attribute */
            index_size += attr_ptr->size;
            /* add de head size to the total de header to be sent in this */
            /* response.*/
            de_size += attr_ptr->de_head_size;
        }

        /* store the current attribute pointer for use later in the final */
        /* response size calculations */
        last_rch_ptr = attr_ptr;

        /* set the pointer to the  next attribute */
        attr_ptr = (SDSATT_T *)attr_ptr->next_att;
        /* increment the number of attributes to be included in the response */
        index++;
    }
    while(((index_size + de_size) < sdscb->max_num_rch) && attr_ptr != NULL);


    /* store the size of attributes to be sent in this response */
    sdscb->response_size_attributes = index_size;
    /* store the number of attributes to be sent in this response */
    sdscb->response_number_attributes = index;
    /* store the de header size to be sent in this response */
    sdscb->response_size_de = de_size;


    /* test to see how much of the last attribute to be included in the */
    /* current response can be sent */
    if((index_size + de_size) > sdscb->max_num_rch)
    {
        /* the last attribute to be sent in the current response, will not */
        /* fit into the response in full, so we need to calculate how much */
        /* of the attribute we can include in this response. */

        /* test to see if we can fit more than just the de header associated */
        /* with the current attribute into the current response */
        /* NOTE: we will not split a de header between two responses */

        /* by subtracting total data attribute size */
        /* (including de header size), after the last attribute size has */
        /* been subtracted from the total */
        /* (including any data sent previous response), from the maximum */
        /* size of attributes permitted in this response. */
        /* if the remaining data size left to send the attribute is greater */
        /* than the de header size, we can send some of the attribute. */
        if( sdscb->max_num_rch -
            ( (index_size -
               (last_rch_ptr->size - last_rch_ptr->sent_size)) +
              (de_size -
               (last_rch_ptr->de_head_size - last_rch_ptr->de_sent_size)) ) >
            last_rch_ptr->de_head_size)
        {
            /* only send part of the attribute */

            /* calculate the size that can be sent in the current response */
            last_rch_ptr->send_size =
                (last_rch_ptr->size - last_rch_ptr->sent_size) -
                ((index_size + de_size) - sdscb->max_num_rch);
            /* subtract the size attribute data not to be sent in the */
            /* current response */
            sdscb->response_size_attributes -=
                ((last_rch_ptr->size - last_rch_ptr->sent_size) -
                 last_rch_ptr->send_size);
        }
        else
        {
            /* only the de header will fit into the current response, */
            /* so don't send the last attribute in the current response */

            /* subtract the last attribute item size */
            sdscb->response_size_attributes -= last_rch_ptr->size;
            /* subtract the last attribute item */
            sdscb->response_number_attributes -= 1;
            /* subtract the last attribute item de header size */
            sdscb->response_size_de -= last_rch_ptr->de_head_size;
        }
    }

}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void calculate_de_size(void)
 *
 *  DESCRIPTION
 *  Calculates the size of the data element for each service record attributes
 *  1. step though the attributes and test for a attribute from a new service
 *  record.  If a new service record is present, store the size information in
 *  the first attribute from the previous service record.  So at the each
 *  initial service record attribute contains information on the size of the
 *  data and data element size.
 *  2. Update the final and first items, including the total sizes.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void calculate_de_size(void)
{
    SDSATT_T *current_rch_ptr; /* current attribute pointer */
    SDSATT_T *last_rch_ptr;    /* last attribute pointer */
    SDSATT_T *hold_rch_ptr;    /* store current attribute pointer */
    CsrUint16 size = 0;         /* current pointer data size */


    /* start at first attribute */
    last_rch_ptr = (SDSATT_T *)sdscb->current_attribute_ptr;
    /* set current pointer first attribute  */
    current_rch_ptr = last_rch_ptr;
    /* set hold pointer first attribute  */
    hold_rch_ptr = last_rch_ptr;
    /* initialise the total data element size for the remaining items */
    sdscb->total_de_head_size = 0;

    /* Index though attributes and find the attributes with differing */
    /* handles, if a differing handles are found this indicates a de header */
    /* is required, so store the size of the de header and add it to the */
    /* total, for use during the calculation of the response */
    do
    {
        /* test for a change in the service record handle */
        if (last_rch_ptr->srhandle != current_rch_ptr->srhandle)
        {
            /* this attribute is from a new service record, so add */
            /* the header size to stored sizes */
            /* store the de header size */
            hold_rch_ptr->de_head_size = DATA_ELEMENT_SEND_SIZE;
            /* store the total of attribute sizes */
            hold_rch_ptr->total_attribute_size = size;
            /* add the std data element size */
            sdscb->total_de_head_size += DATA_ELEMENT_SEND_SIZE;
            /* reset size */
            size = 0;
            /* add the current size */
            size += last_rch_ptr->size;
            /* hold pointer of the first attribute in the service record */
            hold_rch_ptr = last_rch_ptr;
        }
        else  /* no change in the service record handle*/
        {
            /* add the current size */
            size += last_rch_ptr->size;
        }

        /* initialise the total attribute size, to make sure it does */
        /* not interfere with any other further calculations */
        last_rch_ptr->total_attribute_size = 0;
        /* initialise the attribute de header size, to make sure it does */
        /* not interfere with any other further calculations */
        last_rch_ptr->de_head_size = 0;

        /* initialise the partial size of attribute data to send, */
        /* to make sure it does not interfere with any other further */
        /* calculations */
        last_rch_ptr->send_size = 0;
        /* initialise the size of attribute data left, to make sure it does */
        /* not interfere with any other further calculations */
        last_rch_ptr->left_size = 0;
        /* initialise the partial size of attribute data already sent, */
        /* to make sure it does not interfere with any other further */
        /* calculations */
        last_rch_ptr->sent_size = 0;
        /* initialise the size of de header data already sent, */
        /* to make sure it does not interfere with any other further */
        /* calculations */
        last_rch_ptr->de_sent_size = 0;

        /* store the current attribute pointer */
        current_rch_ptr = last_rch_ptr;
        /* increment step to next attribute in response */
        last_rch_ptr = (SDSATT_T *)current_rch_ptr->next_att;
    }
    while(current_rch_ptr->next_att != NULL); /* until there is no */
    /* next attribute */

    /* store the size of the attributes for the final item */
    hold_rch_ptr->total_attribute_size = size;

    /* we need to add a de header to the final attribute */

    /* store the de header size for the final attribute */
    hold_rch_ptr->de_head_size += DATA_ELEMENT_SEND_SIZE;
    /* add the de header sizes to de header size total for the final */
    /* attribute */
    sdscb->total_de_head_size += DATA_ELEMENT_SEND_SIZE ;

    /* test for the service search attribute request */
    if (sdscb->srch_ptr[0] == SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST)
    {
        /* the service search attribute response requires an overall de */
        /* header, so we need to add the de header size to the stored */
        /* de header size in the linked list and to the total of the */
        /* de header size. To make sure we include this when we calculate */
        /* the response size*/

        /* point to the first attribute in the linked list */
        hold_rch_ptr = (SDSATT_T *)sdscb->current_attribute_ptr;
        /* add the initial de header size link list de header size */
        hold_rch_ptr->de_head_size += DATA_ELEMENT_SEND_SIZE;
        /*  add the initial de header size to the de header total */
        sdscb->total_de_head_size += DATA_ELEMENT_SEND_SIZE ;
    }

    /* set the de header sizes left the total de header size */
    sdscb->left_de_head_size = sdscb->total_de_head_size;

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void build_attr_srch_resp(CsrBool first_entry)
 *
 *  DESCRIPTION
 *  Build the attribute search response to the search request,
 *  for both the SDP_SERVICE_ATTRIBUTE_REQUEST and the
 *  SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST
 *  1. Calculate the data element size for each attribute and holds the values
 *     in the link list. Only the first time of entry.
 *  2. Test the size of the attributes will fit within the maximum response
 *     size permitted.
 *     If the attributes will not fit calculate the maximum number that will
 *     fit, set up the number and size of attributes to be sent and the
 *     continuation state.
 *     If the attributes will fit, set to the total number and size of
 *     attributes to be sent.
 *  3. If the service attribute response is required subtract
 *     the overall data element size, from the size total.  Which has been
 *     added to the total.
 *  4. Build the response PDU header, then step though the link list of
 *     attributes and add the attributes.  Adding the appropriate DE headers.
 *     Set the current link list pointer to the last attribute in the response,
 *     so if there are more attributes to be sent we know where to re-start.
 *  5. Add the continuation state at the end of the PDU.
 *
 *  RETURNS/MODIFIES
 *  CsrBool return state
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_attr_srch_resp(CsrBool first_entry)
{
    CsrUint16 index = 0;                   /* current index */
    CsrUint16 index_step = 0;              /* next attribute index step */
    CsrUint16 size_attributes_to_send = 0; /* size attributes to be sent */
    CsrUint8 continuation_state_size = 0;  /* size of the continuation state */
    SDSATT_T *last_rch_ptr;               /* next attribute pointer */
    CsrUint16 data_el_size = 0;             /* data element header size */
    /* accumulator  */

    if (first_entry) /*  first time */
    {
        /* calculate the data elements header sizes to be included in the */
        /* response */
        calculate_de_size();
    }

    /* test to see if all the attributes fit into the maximum size permitted */
    if ((sdscb->left_size_attributes + sdscb->left_de_head_size) >
        sdscb->max_num_rch)
    {
        /* calculate the number and size of attributes to be sent */
        calculate_attribute_max_size();

        /* store size attributes to be sent */
        size_attributes_to_send = sdscb->response_size_attributes +
            sdscb->response_size_de;

        /* update the size of attributes left to send */
        sdscb->left_size_attributes -= sdscb->response_size_attributes;
        /* update the size of de headers left to send */
        sdscb->left_de_head_size  -= sdscb->response_size_de;

        /* store number attributes left to send */
        sdscb->left_number_attributes -= sdscb->response_number_attributes;
        /* test to find what kind of continuation state size is required */

        /* set the continuation state size 16 bit */
        continuation_state_size = CONTINUATION_SIZE_16BIT;

        /* set the continuation state */
        sdscb->continuation_state = TRUE;
    }
    else /* all remaining attributes will fit the maximum return attributes */
    {
        /* store size attributes to be sent */
        size_attributes_to_send = (CsrUint16)(sdscb->left_size_attributes +
                                             sdscb->left_de_head_size);
        /* set the number of attributes to send */
        sdscb->response_number_attributes = sdscb->left_number_attributes;
        /* set the continuation state */
        sdscb->continuation_state = FALSE;
    }

    /* store response size */
    sdscb->resp_size = (CsrUint16)size_attributes_to_send +
        continuation_state_size + PDU_HEADER_SIZE +
        PDU_ATTR_TAIL_SIZE;

    /* create the memory for the search response */
    sdscb->resp_ptr = (CsrUint8 *)CsrPmemAlloc(sdscb->resp_size);

    /* create the PDU header for the response to SDC */
    SDS_create_pdu_header((CsrUint8)(sdscb->srch_ptr[0] + 1) ,
                          (CsrUint16)(sdscb->resp_size - PDU_HEADER_SIZE));

    /* PDU PARAMETERS */
    /* Attribute List byte Count */
    SDP_UINT16_W(sdscb->resp_ptr, PDU_HEADER_SIZE, size_attributes_to_send);

    /* start at current attribute */
    last_rch_ptr = (SDSATT_T *)sdscb->current_attribute_ptr;

    /* store the overall data element header for the search attribute */
    /* response */
    /* test for the first response to be sent and the service attribute */
    /* request */
    if (first_entry
            && sdscb->srch_ptr[0] == SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST)
    {
        /*  first time */
        /* set the data element header size accumulator */
        data_el_size = DATA_ELEMENT_SEND_SIZE;

        /* create a data element header of 16 bit size */
        SDS_create_de_header(DATA_ELEMENT_SEND_TYPE_16, PDU_HEAD_SIZE +
                             PDU_MAX_COUNT_SIZE,
                             (CsrUint16)((sdscb->size_attributes + sdscb->total_de_head_size) -
                                        DATA_ELEMENT_SEND_SIZE));
    }

    /* index through attributes to be sent in this response */
    for (index = 0;index < sdscb->response_number_attributes;index++)
    {

        /* test for the stored de header sizes which will occur at the */
        /* beginning of each service record handle, */
        /* also make sure when the attribute associated with the header */
        /* is split over two responses the de header only gets added in */
        /* the first response where the data left will be zero */
        if (last_rch_ptr->de_head_size > 0 && last_rch_ptr->left_size == 0)
        {
            /* create a data element header of 16 bit size */
            SDS_create_de_header(DATA_ELEMENT_SEND_TYPE_16,
                                 (CsrUint16)(index_step + PDU_HEADER_SIZE + data_el_size +
                                            BIT_16_INDEX), last_rch_ptr->total_attribute_size);
            /* increment the data element header size accumulator */
            data_el_size += DATA_ELEMENT_SEND_SIZE;
        }
        /* test for the attribute in the current response to see if it */
        /* is to be divided between more than one response */
        if (last_rch_ptr->send_size > 0)
        {
            /* this attribute is to be divided between more than one response*/
            /* the send size indicates the size of the data to be sent */
            /* copy attribute data to response */
            CsrUint16 i;
            CsrUint8 *src = last_rch_ptr->attrib_id + last_rch_ptr->sent_size;
            CsrUint8 *dest = sdscb->resp_ptr + index_step + PDU_HEADER_SIZE + data_el_size + PDU_MAX_COUNT_SIZE;
            for(i=0;i<last_rch_ptr->send_size;i++)
            {
                dest[i] = SDP_UINT8_R(src,i);
            }

            /* calculate the remaining size of data to be sent in the */
            /* subsequent response */
            last_rch_ptr->left_size =
                ((last_rch_ptr->size - last_rch_ptr->sent_size) -
                 last_rch_ptr->send_size);
            /* store the de header size sent */
            last_rch_ptr->de_sent_size = last_rch_ptr->de_head_size;
            /* add the size to be sent, it to the total of size sent */
            last_rch_ptr->sent_size += last_rch_ptr->send_size;
            /* set the current attribute size to zero to make sure it does */
            /* not interfere with any other further calculations */
            last_rch_ptr->send_size = 0;
            /* we need to revisit this attribute so bump up the number of */
            /* attributes left to send by one */
            sdscb->left_number_attributes++;
            /* NOTE: don't point to the next attribute, because we need to */
            /* revisit this attribute during the next response to complete */
            /* the attribute data */
        }
        else if (last_rch_ptr->left_size > 0)
        {
            /* this attribute is to be divided between many responses, the */
            /* left size indicates the size of the data to be sent last, */
            /* so continue with the last attribute which was divided between */
            /* many responses */
            /* copy attribute data to response */
            CsrUint16 i;
            CsrUint8 *src = last_rch_ptr->attrib_id + last_rch_ptr->sent_size;
            CsrUint8 *dest = sdscb->resp_ptr + index_step + PDU_HEADER_SIZE + data_el_size + PDU_MAX_COUNT_SIZE;
            for(i=0;i<last_rch_ptr->left_size;i++)
            {
                dest[i] = SDP_UINT8_R(src,i);
            }

            /* step to next attribute point to be stored in the response */
            index_step += last_rch_ptr->left_size;
            /* set the remaining attribute size to zero to make sure it does */
            /* not interfere with any other further calculations */
            last_rch_ptr->left_size = 0;
            /* set the total attribute size sent to zero to make sure it */
            /* does not interfere with any other further calculations */
            last_rch_ptr->sent_size = 0;
            /* set the total de header size sent to zero to make sure it */
            /* does not interfere with any other further calculations */
            last_rch_ptr->de_sent_size = 0;
            /* point to next attribute in response list */
            last_rch_ptr = (SDSATT_T *)last_rch_ptr->next_att;
        }
        else
        {
            /* send std attribute in response */
            /* copy attribute data to response */
            CsrUint16 i;
            CsrUint8 *src = last_rch_ptr->attrib_id;
            CsrUint8 *dest = sdscb->resp_ptr + index_step + PDU_HEADER_SIZE + data_el_size + PDU_MAX_COUNT_SIZE;
            for(i=0;i<last_rch_ptr->size;i++)
            {
                dest[i] = SDP_UINT8_R(src,i);
            }


            /* step to next attribute point to be stored in the response */
            index_step += last_rch_ptr->size;
            /* point to next attribute in response list */
            last_rch_ptr = (SDSATT_T *)last_rch_ptr->next_att;
        }
    }
    /* set current pointer to the next attribute for the next response */
    sdscb->current_attribute_ptr = (CsrUint8 *)last_rch_ptr;

    /* create the continuation data at the end of the response to SDC */
    SDS_add_continuation(continuation_state_size,
                         (CsrUint16)(sdscb->left_size_attributes + sdscb->left_de_head_size));
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint16 Data_Element_Size(CsrUint8 *de_ptr)
 *
 *  DESCRIPTION
 *  Calculates the size of data held in a data element package
 *  1. Separates the size header information, then checks the type against known
 *     size descriptors and produces the size of the data element.  If the type
 *     is not recognised an error is indicated.
 *
 *  RETURNS/MODIFIES
 *  returns CsrUint16 size of DE data
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
static CsrUint16 Data_Element_Size(CsrUint8 *de_ptr)
{
    CsrUint16 de_size = 0;  /* date element size */
    CsrUint8 header = SDP_UINT8_R(de_ptr, 0);

    /* find the data element size */
    switch(header & DATA_ELEMENT_SIZE_MASK)
    {
        case SIZE_INDEX_1BYTE:
            de_size = 1;
            break;
        case SIZE_INDEX_2BYTES:
            de_size = 2;
            break;
        case SIZE_INDEX_4BYTES:
            de_size = 4;
            break;
        case SIZE_INDEX_8BYTES:
            de_size = 8;
            break;
        case SIZE_INDEX_16BYTES:
            de_size = 16;
            break;
        case SIZE_INDEX_8ABITS:
            de_size = (CsrUint16)SDP_UINT8_R(de_ptr, 1);
            /* add 8 bit index */
            de_size += 1;
            break;
        case SIZE_INDEX_16ABITS:
            de_size = SDP_UINT16_R(de_ptr, 1);
            /* add 16 bit index */
            de_size += 2;
            break;
        case SIZE_INDEX_32ABITS:
            de_size = SDP_UINT16_R(de_ptr, 3);
            /* just use the lower part of the data element size, as the type */
            /* may be used but there will not be any 32 bit data */
            de_size += 4;
            break;
        default:
            /* error not UUID default index */
            sdscb->error_state = TRUE;
            sdscb->error_code = PDU_ERROR_SYNTAX;
            break;
    };

    /* return index */
    return de_size;
}
#endif
/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 Data_Element_Index(CsrUint8 *de_ptr)
 *
 *  DESCRIPTION
 *  Calculates the index increment to the first data element in the data
 *  element sequence
 *  1. Separates the size header information, then checks the type against
 *     known size descriptors and produces the index to the data element.
 *     If the type is not recognised an error is indicated.
 *
 *
 *  RETURNS/MODIFIES
 *  CsrUint8 data element index
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint8 Data_Element_Index(CsrUint8 *de_ptr)
{
    CsrUint8 de_size = 0;   /* data element size */
    CsrUint8 header = SDP_UINT8_R(de_ptr, 0);

    /* find the data element index */
    switch(header & DATA_ELEMENT_SIZE_MASK)
    {
        case SIZE_INDEX_1BYTE:
            /* drop though same size */
        case SIZE_INDEX_2BYTES:
            /* drop though same size */
        case SIZE_INDEX_4BYTES:
            /* drop though same size */
        case SIZE_INDEX_8BYTES:
            /* drop though same size */
        case SIZE_INDEX_16BYTES:
            /* one index position */
            de_size = 1;
            break;
        case SIZE_INDEX_8ABITS:
            /* two index positions */
            de_size = 2;
            break;
        case SIZE_INDEX_16ABITS:
            /* three index positions */
            de_size = 3;
            break;
        case SIZE_INDEX_32ABITS:
            /* five index positions */
            de_size = 5;
            break;
        default:
            /* error not UUID default index */
            sdscb->error_state = TRUE;
            sdscb->error_code = PDU_ERROR_SYNTAX;
            break;
    };

    /* return index */
    return de_size;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 UUID_head_size(CsrUint8 *de_ptr)
 *
 *  DESCRIPTION
 *  Calculates the index size of the current UUID based on the header
 *  information
 *  1. Checks the type against known UUID header type descriptors and produces
 *     the size to the UUID.  If the type is not recognised an error is
 *     indicated.
 *
 *  RETURNS/MODIFIES
 *  returns CsrUint8 UUID index size
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint8 UUID_head_size(CsrUint8 *de_ptr)
{
    CsrUint8 de_size = 0;   /* data element size */
    CsrUint8 header = SDP_UINT8_R(de_ptr, 0);

    /* find the UUID head index */
    switch(header)
    {
        case ATTRIBUTE_HEADER_8BITS:
            /* one index positions */
            de_size = 1;
            break;
        case ATTRIBUTE_HEADER_16BITS:
            /* one index positions */
            de_size = 2;
            break;
        case ATTRIBUTE_HEADER_32BITS:
            /* four index positions */
            de_size = 4;
            break;
        case UUID_HEADER_16BITS:
            /* two index positions */
            de_size = 2;
            break;
        case UUID_HEADER_32BITS:
            /* four index positions */
            de_size = 4;
            break;
        case UUID_HEADER_128BITS:
            /* sixteen index positions */
            de_size = 16;
            break;
        default:
            /* error not UUID default index */
            sdscb->error_state = TRUE;
            sdscb->error_code = PDU_ERROR_SYNTAX;
            break;
    };

    /* return index */
    return de_size;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool search_database(CsrUint8 *srch_ptr, CsrUint16 curr_index)
 *
 *  DESCRIPTION
 *  steps through the service records
 *  1. store locally the pointer to the service record
 *  2. test for any service records present
 *  3. Step though the service records
 *  4. first searching for the initial UUID then for the other UUID's
 *  5. if a match is found stop searching and store the service record
 *     handle in a linked list
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool UUID match reply
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool search_database(CsrUint8 *srch_ptr, CsrUint16 curr_index)
{
    CsrBool rnt_flag = FALSE;  /* set return flag, for failure response */
    CsrBool rch_flag = FALSE;  /* flag to indicate UUID match */
    CsrUint8 *rc_ptr;          /* service record pointer */
    CsrBool terminate = FALSE; /* flag to end while loop */
    CsrUint32 srhandle;        /* service record handle */
    CsrUint8 hand_counter = 0; /* current service record handle counter */

    /* set the service record pointer to the first service record in the list */
    rc_ptr = sdsll.first_rc;

    /* test that service records are present */
    if (rc_ptr != NULL) /* service records */
    {
        /* step though the service records */
        do
        {
            /* search though the service records UUID's for a match */
            rch_flag = search_uuids(rc_ptr, srch_ptr, curr_index);
            /* all UUIDS match service records*/
            if (rch_flag)
            {
                /* store the service record handle for comparison */
                srhandle = store_service_record_handle(rc_ptr +
                                                       SERVICE_DISCOVERY_SERVER_INDEX_HANDLE);
                /* store handle for response */
                build_handle_list(srhandle);
                /* set return flag response */
                rnt_flag = TRUE;
                /* test maximum number of handles reached */
                if (sdscb->number_handles == sdscb->max_num_rch)
                {
                    /* end while loop */
                    terminate = TRUE;
                }
            }
            /* increment handle counter */
            hand_counter++;
            /* update the pointer to the next service record */
            rc_ptr = SDS_find_next_service_record(hand_counter, rc_ptr, FALSE);
            /* next service record */
            if (NULL == rc_ptr)
            {
                /* end while loop */
                terminate = TRUE;
            }
        } while(!terminate); /* end while loop terminate = TRUE */
    }
    /* return search status flag */
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool search_attrib_types(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr)
 *
 *  DESCRIPTION
 *  Steps through the service record attributes which are UUID types, to find
 *  a match with the search request
 *  1. Store the search uuid and the size of the service record.
 *  2. Step through the service record
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool UUID match reply
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool search_attrib_types(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr)
{
    CsrBool rnt_flag = FALSE;     /* set return flag, for correct response */
    CsrUint16 size = 0;           /* current size */
    CsrUint16 index_total = 0;    /* total index */
    CsrUint16 length;             /* service record length */
    CsrUint8 de_type = 0;         /* data element type */
    CsrUint16 id_type = 0;        /* service record attribute id */
    CsrUint32 rc_uuid[4];        /* service record UUID */
    CsrUint32 srch_uuid[4];      /* search UUID */


    /* store search UUID and test result for a error */
    if (store_uuid(srch_ptr,TRUE,srch_uuid))
    {
        /* correct uuid header */

        /* store size of data in service record */
        length = SDP_UINT16_R(rc_ptr, sizeof(CsrInt8 *));

        /* find the pointer to the start of the service record */
#ifdef BUILD_FOR_HOST
        writePtrValue( (CsrUint8 *) &rc_ptr, &(rc_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
/*        rc_ptr = (CsrUint8 *) SDP_UINT32_R( rc_ptr, SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE );*/
#else
        rc_ptr = (CsrUint8 *)rc_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE];
#endif

        /* step through the service record  */
        do
        {
            /* capture the size of the element */
            size = Data_Element_Size(rc_ptr + index_total) + 1;
            /* store the total element index length */
            index_total += size;

            /* test data element header type */
            if (DE_TYPE_ERROR ==
                (de_type = data_element_type(&rc_ptr[index_total])))
            {                                                /* header error */
                /* set return flag to TRUE indicating an error */
                rnt_flag = TRUE;

                /* set error state and code */
                sdscb->error_state = TRUE;
                sdscb->error_code = PDU_ERROR_SYNTAX;

                /* stop while loop */
                index_total = length;
            }
            else if (DE_TYPE_SEQUENCE == de_type) /* header ok */
            {
                /* capture the size of the element */
                size = Data_Element_Size(rc_ptr + index_total) + 1;
                /* store the current service record attribute id */
                id_type = SDP_UINT16_R(rc_ptr,
                                       (CsrUint16)(index_total - BIT_16_INDEX));

                /* test the current service record id for UUID attribute */

                /* correct search id's */
                if (id_type == SERVICE_CLASS_ID_LIST ||
                    id_type == PROTOCOL_DESC_LIST ||
                    id_type == BROWSE_GROUP_LIST ||
                    id_type == BSLUT_PROFILE_DESC_LIST)
                {
                    /* search the service record for the UUID */
                    rnt_flag = search_rc_uuid(srch_uuid,
                                              rc_ptr + index_total - ATTRIBUTE_ID_INDEX);
                }
                /* increment index to the next element */
                index_total += size;
            }
            else /* header no sequence but data */
            {
                /* store the current service record attribute id */
                id_type = SDP_UINT16_R(rc_ptr,
                                       (CsrUint16)(index_total - BIT_16_INDEX));

                if(id_type == SERVICE_ID ) /* service id type */
                {
                    /* is not in a data sequence */
                    /* extract and compare UUIDs */
                    if (store_uuid(rc_ptr + index_total, FALSE,rc_uuid) &&
                        srch_uuid[0] == rc_uuid[0] &&
                        srch_uuid[1] == rc_uuid[1] &&
                        srch_uuid[2] == rc_uuid[2] &&
                        srch_uuid[3] == rc_uuid[3])
                    {
                        /* uuid match found and uuid header is correct*/
                        rnt_flag = TRUE;
                    }
                }
                /* capture the size of the element */
                size = Data_Element_Size(rc_ptr + index_total)+1;
                /* increment index to the next element */
                index_total += size;
            }
            /* test return flag */
            if (rnt_flag)
            {
                /* stop while loop */
                index_total = length;
            }
        }while(index_total < (CsrUint16)length);
        /* while service record items */
    }
    /* return search status flag */
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool search_rc_uuid(CsrUint32 suuid,CsrUint8 *rec_ptr)
 *
 *  DESCRIPTION
 *  Searches though each UUID in a attribute to discover a match with the
 *  search UUID, sets the return flag to TRUE on a match and stops the search
 *  1. Point to the first UUID in the attribute service record.
 *  2. Step though the attributes UUID's trying to find a match with the Search
 *     UUID. If a match is found stop the search and return a positive
 *     response.
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool UUID match reply
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool search_rc_uuid(CsrUint32 *suuid,CsrUint8 *rec_ptr)
{
    CsrBool rnt_flag = FALSE; /* set return flag, for correct response */
    CsrBool terminate = TRUE; /* flag to end while loop */
    CsrUint16 index_ptr = 0;  /* current index */
    CsrUint16 size_uuid = 0;  /* UUID size */
    CsrUint32 rc_uuid[4];    /* service record UUID */


    /* store service record total UUID's size */
    size_uuid = Data_Element_Size(rec_ptr + ATTRIBUTE_ID_INDEX) +
        ATTRIBUTE_ID_INDEX + DATA_ELEMENT_TYPE_INDEX;

    /* store service records index to the first UUID */
    index_ptr = Data_Element_Index(rec_ptr + ATTRIBUTE_ID_INDEX) +
        ATTRIBUTE_ID_INDEX + DATA_ELEMENT_TYPE_INDEX;

    if (SDP_UINT8_R(rec_ptr,index_ptr-1) != UUID_HEADER_16BITS &&
        SDP_UINT8_R(rec_ptr,index_ptr-1) != UUID_HEADER_32BITS &&
        SDP_UINT8_R(rec_ptr,index_ptr-1) != UUID_HEADER_128BITS )
    {
        /* store service records index to the first UUID */
        index_ptr += Data_Element_Index(rec_ptr + (index_ptr-1));
    }

    /*  step though the service record UUID's to find a match with */
    /* search UUID */
    do
    {
        /* store UUID */
        if (store_uuid(rec_ptr+index_ptr-1, FALSE, rc_uuid) &&
            suuid[0] == rc_uuid[0] &&
            suuid[1] == rc_uuid[1] &&
            suuid[2] == rc_uuid[2] &&
            suuid[3] == rc_uuid[3])
        {

            /* match found and rc_uuid is the correct header */
            /* terminate loop and set return flag */
            rnt_flag = TRUE;
            terminate = FALSE;
        }

        /* calculate index to next service record UUID */
        index_ptr += UUID_head_size(rec_ptr+(index_ptr-1)) + NEXT_ATTRIBUTE_ID;

        /* test for more UUID's in service record */
        if (index_ptr >= size_uuid)
        {
            /* no more UUID's end while loop */
            terminate = FALSE;
        }
        else if ((SDP_UINT8_R(rec_ptr,index_ptr-1) & DATA_ELEMENT_TYPE_MASK) ==
                 DATA_ELEMENT_SEQUENCE)
        {
            /* store service records index to the next UUID */
            index_ptr += Data_Element_Index(rec_ptr+(index_ptr-1));
        }
    }
    while(terminate); /* end while loop terminate = FALSE */

    /* return search status flag */
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool search_uuids(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr,
 *                            CsrUint16 base_index)
 *
 *  DESCRIPTION
 *  Searches though the UUID's in the search list to find a match
 *  1. Steps though the search UUID attributes and then searches the service
 *     record UUID attributes to find a match. If a failure to match occurs
 *     within any service record the search is a failure.
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool UUID match reply
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool search_uuids(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr,
                    CsrUint16 base_index)
{
    CsrBool rnt_flag = FALSE;     /* set return flag, for error response */
    CsrBool response_flag = TRUE; /* set response flag, for error response */
    CsrUint16 index = 0;          /* current index */


    /* step though the service record UUID's to find a match with */
    /* the search UUID's */
    /* while a uuid is left */
    do
    {
        /* search though the service records UUID's for a match */
        rnt_flag = search_attrib_types(rc_ptr, srch_ptr + index);
        /* calculate index to next search UUID */
        index += UUID_head_size(srch_ptr + index) + NEXT_ATTRIBUTE_ID;
        /* test for a failure */
        if (!rnt_flag)
        {
            /* if one uuid match fails then the match is a failure */
            response_flag = FALSE;
        }
    }
    while((index + base_index + PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE) <
          sdscb->srch_size);

    /* return search status flag */
    return response_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void build_handle_list(CsrUint32 srhandle)
 *
 *  DESCRIPTION
 *  Generates linked list of service record handles, which match the search
 *  request
 *  1. Test to see if any handles lists have been stored already. If they
 *     have not initialise the first data base handle pointer, else update the
 *     global link list pointers.
 *  2. Set the link list handle pointer to the search matched service record
 *     handle and increment the number of handles found.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void build_handle_list(CsrUint32 srhandle)
{
    SDSRCH_T *handle_ptr;     /* handle pointer */
    SDSRCH_T *last_rh_ptr;    /* next handle pointer */

    /* create the memory for the handle response */
    handle_ptr = (SDSRCH_T *)CsrPmemAlloc(sizeof(SDSRCH_T));

    /* first handle stored */
    if(sdscb->number_handles == 0)
    {
        /* store first handle */
        sdscb->handle_db_ptr = (CsrUint8 *)handle_ptr;
    }
    else /* existing handles stored */
    {
        /* store last handle */
        last_rh_ptr = (SDSRCH_T *)sdscb->current_hdl_ptr;
        /* set last handles next handle */
        last_rh_ptr->next_rch = (CsrUint8 *)handle_ptr;
    }
    /* set current handle next handle*/
    handle_ptr->next_rch = NULL;
    /* set current handle */
    handle_ptr->rchandle = srhandle;

    /* store current handle */
    sdscb->current_hdl_ptr = (CsrUint8 *)handle_ptr;

    /* increment number of handles */
    sdscb->number_handles++;
    /* store the number handles left */
    sdscb->left_number_handles = sdscb->number_handles;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void free_handle_list(void)
 *
 *  DESCRIPTION
 *  Frees list of service record handles, which matched the search request
 *  1. Initialise the sr handle pointer with the first handle data base
 *     pointer.
 *  2. Test that there are link list items, if so step though the link lists
 *     which point to the sr handles that matched the search and free the
 *     linked list.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void free_handle_list(void)
{
    CsrUint16 i = 0;           /* loop index */
    SDSRCH_T *handle_ptr;     /* handle pointer */
    CsrUint8 *next_rh_ptr;     /* next handle pointer */

    /* store first handle in the link list */
    handle_ptr = (SDSRCH_T *)sdscb->handle_db_ptr;

    /* test there are service record handle in the linked list */
    if (sdscb->number_handles != 0)
    {
        /* step though the link list of handles */
        for (i=0;i<sdscb->number_handles;i++)
        {
            /* store next handle in link list */
            next_rh_ptr = handle_ptr->next_rch;
            /* free current handle */
            CsrPmemFree(handle_ptr);
            /* set next handle to current handle */
            handle_ptr =  (SDSRCH_T *)next_rh_ptr;
        }
        /* set the number of handle to zero, so any further call of this */
        /* function does not cause an error */
        sdscb->number_handles = 0;
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void free_attribute_list(void)
 *
 *  DESCRIPTION
 *  Frees link list of service record attributes, which matched the search
 *  request
 *  1. Initialise the attribute pointer with the first data base pointer.
 *  2. Test that there are link list items, if so step though the link lists
 *     which point to the attributes that matched the search and free the
 *     linked list.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void free_attribute_list(void)
{
    CsrUint16 i = 0;           /* loop index */
    SDSATT_T *attribute_ptr;  /* pointer to current attribute */
    CsrUint8 *next_rh_ptr;     /* pointer the next attribute */

    /* store first handle in the link list */
    attribute_ptr = (SDSATT_T *)sdscb->attribute_db_ptr;

    /* test if any attributes in the link list */
    if (sdscb->number_attributes != 0)
    {
        /* step though the link list of handles */
        for (i=0;i<sdscb->number_attributes;i++)
        {
            /* store next handle in link list */
            next_rh_ptr = attribute_ptr->next_att;

#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
            /* test to see if there is any static/ps service records */
            if (attribute_ptr->srhandle < (CsrUint32)(SR_HANDLE_START +
                                                     sdscon.sr_dbase_number))
            {
                /* the current attribute is taken from a static/ps service */
                /* record */
                /* free the memory which holds the attribute data */
                CsrPmemFree(attribute_ptr->attrib_id);
            }
#endif

            /* free current handle */
            CsrPmemFree(attribute_ptr);
            /* set next handle to current handle */
            attribute_ptr =  (SDSATT_T *)next_rh_ptr;
        }
        /* set the number of attributes to zero, so any further call of this */
        /* function does not cause an error */
        sdscb->number_attributes = 0;
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool find_service_record(CsrUint32 sr_handle)
 *
 *  DESCRIPTION
 *  Find service record
 *  1. Start the search with the first record and test to make sure there
 *     is one
 *  2. Step through the service records to find a service record handle match
 *  3. If a match if found search the service record for the requested
 *     attributes
 *
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool Attribute ID match reply
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool find_service_record(CsrUint32 sr_handle)
{
    CsrBool rnt_flag = FALSE;   /* set return flag, for failed response */
    CsrUint8 *rc_ptr;           /* pointer service record handle */
    CsrBool terminate = FALSE;  /* flag to end while loop */
    CsrUint32 srhandle;         /* service record handle */
    CsrUint8 hand_counter = 0;  /* current service record handle counter */


    /* set the service record pointer to the first service record in the list */
    rc_ptr = sdsll.first_rc;

    /* test that records exist */
    if (NULL != rc_ptr)
    {
        /* step though the service records */
        do
        {
            /* store the service record handle for comparison */
            srhandle =
                store_service_record_handle(rc_ptr +
                                            SERVICE_DISCOVERY_SERVER_INDEX_HANDLE);

            /* teat the service record handle and search handle */
            if (srhandle == sr_handle)
            {
                /* service handle match */

                /* find attributes for this service record handle */
                rnt_flag = find_attributes(rc_ptr, sdscb->srch_ptr +
                                           (PDU_HEADER_SIZE + PDU_HANDLE_SIZE + PDU_MAX_COUNT_SIZE ),
                                           (CsrUint16)(sdscb->srch_size -
                                                      (PDU_HEADER_SIZE + PDU_HANDLE_SIZE + PDU_MAX_COUNT_SIZE)),
                                           sr_handle);
                /* found service record handle match end while loop */
                terminate = TRUE;
            }
            /* increment handle counter */
            hand_counter++;
            /* update the pointer to the next service record */
            rc_ptr = SDS_find_next_service_record(hand_counter, rc_ptr,
                                                  rnt_flag);

            /* test for end of the service records and no service record */
            /* handle match */
            if(NULL == rc_ptr && !terminate)
            {
                /* end of the service records and no service record handle */
                /* match */
                /* set error code */
                sdscb->error_code = PDU_ERROR_SR_HANDLE;
                /* set return flag to FALSE indicating an error */
                sdscb->error_state = TRUE;
                /* end while loop */
                terminate = TRUE;
            }
            else if (NULL == rc_ptr)
            {
                /* end of the service records */
                /* end while loop */
                terminate = TRUE;
            }

        } while(!terminate); /* end while loop terminate = FALSE */
    }
    /* return search status flag */
    return rnt_flag;

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool find_attributes(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr,
 *                         CsrUint16 srch_size,
 *                         CsrUint32 sr_handle)
 *
 *  DESCRIPTION
 *  step through the requested attributes to find a match
 *  1. step through the requested search attributes
 *  2. test for single attribute or range of attributes and
 *     search service record appropriately
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool Attribute ID match reply
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool find_attributes(CsrUint8 *rc_ptr, CsrUint8 *srch_ptr, CsrUint16 srch_size,
                       CsrUint32 sr_handle)
{
    CsrBool rnt_flag = FALSE;      /* flag to detect error */
    CsrBool response_flag = FALSE; /* set return flag, for correct response */
    CsrUint16 index = 0;           /* current index */
    CsrUint8 index_hold = 0;       /* current attribute search type index */
    CsrUint16 attrib_id = 0;       /* current attribute ID */


    /* index to first search attribute */
    index = (CsrUint16)Data_Element_Index(srch_ptr);

    /* step though the attributes */
    do
    {
        /* type of attribute search */
        index_hold = UUID_head_size(srch_ptr + index);

        /* Single 16 bit attribute */
        if (index_hold == ATTRIBUTE_SEARCH)
        {
            attrib_id = SDP_UINT16_R(srch_ptr, (CsrUint16)(index + 1));

            /* search service record for an attribute match */
            rnt_flag = get_attributes(rc_ptr, sr_handle,
                                      attrib_id, attrib_id);
        }
        /* 2 * 16 bit attribute range */
        else if (index_hold == ATTRIBUTE_SEARCH_RANGE)
        {
            CsrUint16 low_attrib_id; /* first in range of attribute ID's */
            CsrUint16 hi_attrib_id;    /* last in range of attribute ID's */

            low_attrib_id = SDP_UINT16_R(srch_ptr, (CsrUint16)(index + 1));
            hi_attrib_id = SDP_UINT16_R(srch_ptr,
                                        (CsrUint16)(index + 1 + BIT_16_INDEX));

            /* search service record for all matching attributes */
            rnt_flag = get_attributes(rc_ptr, sr_handle,
                                      low_attrib_id, hi_attrib_id);
        }
        /* match found */
        if (rnt_flag && !sdscb->error_state)
        {
            /* set response flag */
            response_flag = TRUE;
        }
        /* set index to point to the next attribute id */
        index += index_hold + 1;
    }
    while ((index + 1) < srch_size); /* while request attributes left */

    /* return search status flag */
    return response_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  get_attributes
 *
 *  DESCRIPTION
 *  Searched the service record for all attribute ID's within the given range.
 *  1. store the length of the data within the service record
 *  2. step through the service record comparing the attribute id's
 *  3. if a match is found store a pointer to the data in a link list along
 *     with the size and handle, so it can be retrieved when sending the reply.
 *
 *  RETURNS/MODIFIES
 *        returns TRUE if a matching attribute was found.
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
static CsrBool get_attributes(CsrUint8 *rc_ptr, CsrUint32 sr_handle,
                             CsrUint16 low_attrib_id, CsrUint16 hi_attrib_id)
{
    CsrBool rnt_flag = FALSE;  /* set return flag, for failure response */
    CsrUint16 size = 0;        /* data element size */
    CsrUint16 sr_attrib_id;    /* service record attribute id */
    CsrUint16 index_total = 0; /* total index */
    CsrUint16 length = 0;      /* length of data in the service record */
    CsrUint8 de_type = 0;      /* data element type */

    /* store the length of the service record */
    length = SDP_UINT16_R(rc_ptr, sizeof(CsrInt8 *));

    /* we have to special case a read of the record handle
       because it is now separate to the record itself.
    */

    /* The bluetooth spec states that SERVICE_RECORD_HANDLE is always 0.
       As the service record handle is always the first attribute, this
       should always be attribute 0 (ie low_attrib_id should be 0). So
       the test for the service record handle could just be written as
       if low_attrib_id == 0 however it is better to say
       if low_attrib_id == SERVICE_RECORD_HANDLE.
    */

    if (low_attrib_id == SERVICE_RECORD_HANDLE)
    {
        /* store the pointer to, size and handle of the service */
        /* record attribute in a linked list */
        attrib_capture(sr_handle,
                       rc_ptr + SERVICE_DISCOVERY_SERVER_INDEX_HANDLE - DATA_ELEMENT_SEND_SIZE - 1,
                       (CsrUint16)(PDU_HANDLE_SIZE + DATA_ELEMENT_SEND_SIZE + 1), rc_ptr);

        /* set the return flag to true, indicating that at least
           one attribute has been found and stored in the link list */
        rnt_flag = TRUE;
    }


    /* set the service record pointer to the start of the attributes */
#ifdef BUILD_FOR_HOST
    writePtrValue( (CsrUint8 *) &rc_ptr, &(rc_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
/*    rc_ptr = (CsrUint8 *) SDP_UINT32_R( rc_ptr, SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE );*/
#else
    rc_ptr = (CsrUint8 *)rc_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE];
#endif

    /* step through attributes in the service record */
    do
    {
        /* store the size of the element */
        size = Data_Element_Size(rc_ptr + index_total) + 1;
        /* store the total element index length */
        index_total += size;
        /* test for a valid type of header */
        de_type = data_element_type(&rc_ptr[index_total]);

        if (DE_TYPE_ERROR == de_type)
        {
            /* header error */
            /* set return flag to FALSE indicating an error */
            rnt_flag = FALSE;
            /* set error state and code */
            sdscb->error_state = TRUE;
            sdscb->error_code = PDU_ERROR_SYNTAX;


            /* stop while loop and end search */
            index_total = length - 1;
        }
        else if (DE_TYPE_SEQUENCE == de_type) /* header ok */
        {
            /* capture the size of the element */
            size = Data_Element_Size(rc_ptr + index_total) + 1;
            /* increment index total to the next element */
            index_total += size;
        }
        else /* header no sequence but data */
        {
            /* capture the size of the element */
            size = Data_Element_Size(rc_ptr + index_total) + 1;
            /* increment index total to the next element */
            index_total += size;
        }
        /* store the attribute ID found in search */
        sr_attrib_id = SDP_UINT16_R(rc_ptr, (CsrUint16)(index_total - size -
                                             BIT_16_INDEX));

        /* Is the attribute id within the requested attribute id range? */
        if ((sr_attrib_id >= low_attrib_id) &&
            (sr_attrib_id <= hi_attrib_id))
        {
            /* attribute id within range */


            /* store the pointer to, size and handle of the service */
            /* record attribute in a linked list */
            attrib_capture(sr_handle,
                           rc_ptr + index_total - size - DATA_ELEMENT_SEND_SIZE,
                           (CsrUint16)(size + DATA_ELEMENT_SEND_SIZE), rc_ptr);

            /* set the return flag to true, indicating that at least
               one attribute has been found and stored in the link list */
            rnt_flag = TRUE;
        }
    }
    while(index_total < (length - 1 )); /* while data in service record */

    /* return search status flag */
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void attrib_capture(CsrUint32 sr_handle, CsrUint8 *rc_ptr, CsrUint16 size,
 *                      CsrUint8 *sr_ptr)
 *
 *  DESCRIPTION
 *  Store the attributes for response in the linked list
 *  1. Test to see if this is the first attribute. If it is initialise the
 *     first attribute data base pointer and set-up the link list pointers.
 *     If is isn't just set-up the link list pointers.
 *  2. Set the pointer to the attribute, set the size, initialise the de head,
 *     increment the total attribute size, and increment the number of
 *     attributes.  So a track of the attributes can be kept for when the
 *     reply is sent to SDC.
 *
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void attrib_capture(CsrUint32 sr_handle, CsrUint8 *rc_ptr, CsrUint16 size,
                    CsrUint8 *sr_ptr)
{
    SDSATT_T *attr_ptr;      /* attribute pointer */
    SDSATT_T *current_ptr;   /* current attribute pointer */

    PARAM_UNUSED(sr_ptr);

    /* create the memory for link list for the attribute response */
    attr_ptr = (SDSATT_T *)CsrPmemAlloc(sizeof(SDSATT_T));
    /* test the number of attributes stored so far */
    if (sdscb->number_attributes == 0)
    {
        /* there are no attributes stored so far */
        /* set the attribute pointer to point to the first linked list */
        sdscb->attribute_db_ptr = (CsrUint8 *)attr_ptr;
        /* set current attribute pointer to point to the first linked list */
        sdscb->current_attribute_ptr = sdscb->attribute_db_ptr;
    }
    else
    {
        /* there is more than one attribute stored so far */
        /* store last attribute pointer */
        current_ptr = (SDSATT_T *)sdscb->current_attribute_ptr;
        /* set last link list item's link to the next item to point to the*/
        /* current link list item */
        current_ptr->next_att = (CsrUint8 *)attr_ptr;
        /* set current attribute pointer to point to the current item in the */
        /* linked list */
        sdscb->current_attribute_ptr = (CsrUint8 *)attr_ptr;
    }

#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    /* test to see if there are any static/ps service records */
    if (sr_handle < (CsrUint32)(SR_HANDLE_START + sdscon.sr_dbase_number))
    {
        /* the current service record is stored as static/ps */
        /* create memory to store the service record attribute */
        attr_ptr->attrib_id = (CsrUint8 *)CsrPmemAlloc(size);
        /* copy the continence  of the attribute before the service record */
        /* is deleted */
        CsrMemCpy(attr_ptr->attrib_id, rc_ptr, size);
    }
    else
#endif
    {
        /* there are no static/ps service records */
        /* set current attribute pointer to point to the attribute id */
        attr_ptr->attrib_id = rc_ptr;
    }

    /* store the service record handle */
    attr_ptr->srhandle = sr_handle;
    /* set next pointer item to null */
    attr_ptr->next_att = NULL;
    /* store the size of current attribute */
    attr_ptr->size = size;
    /* initialise de head size */
    attr_ptr->de_head_size = 0;
    /* increment the total size of attributes to be sent */
    sdscb->size_attributes += size;
    /* store the size of the attributes left */
    sdscb->left_size_attributes = sdscb->size_attributes;
    /* increment number of attributes to be sent */
    sdscb->number_attributes++;
    /* store the number of attributes left */
    sdscb->left_number_attributes = sdscb->number_attributes;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void search_attribute(void)
 *
 *  DESCRIPTION
 *  Searched the attributes in the service record for a service search
 *  attribute request
 *  1. Calculate the maximum response size permitted.
 *  2. Point the search pointer to the attributes to be found in the search
 *     pattern and calculate the search size.
 *  3. Step though the handles in the link list and search for matches with
 *     the attributes in the search pattern.
 *  4. Tidy up the search by freeing the link list of handles.
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool UUID status reply
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool search_attribute(void)
{
    CsrBool rnt_flag = FALSE; /* set return flag, for correct response */
    CsrUint16 index = 0;      /* current index */
    SDSRCH_T *last_rch_ptr;  /* next handle pointer */
    CsrUint32 sr_handle;      /* service record handle */
    CsrUint8 *srch_ptr_hold;  /* temp pointer to hold the search pointer */


    /* set maximum attribute byte count to the correct value */
    sdscb->max_num_rch = SDP_UINT16_R(sdscb->srch_ptr,
            (CsrUint16)(sdscb->srch_size - PDU_TAIL_SIZE));

    /* store search pointer to be restored */
    srch_ptr_hold = sdscb->srch_ptr;
    /* set the search pointer as a ATTRIBUTE SEARCH */
    sdscb->srch_ptr = (sdscb->srch_ptr + sdscb->srch_size) - PDU_HEADER_SIZE -
        PDU_MAX_COUNT_SIZE - PDU_HANDLE_SIZE - DATA_ELEMENT_TYPE_INDEX;

    /* set size as a ATTRIBUTE SEARCH */
    sdscb->srch_size = (sdscb->attr_size - sdscb->srch_size) + PDU_HEADER_SIZE +
        + PDU_MAX_COUNT_SIZE + PDU_HANDLE_SIZE + DATA_ELEMENT_TYPE_INDEX;

    /* start at first handle */
    last_rch_ptr = (SDSRCH_T *)sdscb->handle_db_ptr;

    /* index through service record handles */
    for (index = 0;index < sdscb->number_handles;index++)
    {
        /* find Service record handle */
        sr_handle = last_rch_ptr->rchandle;

        /* get service record */
        rnt_flag = find_service_record(sr_handle);
        /* step to next handle */
        last_rch_ptr = (SDSRCH_T *)last_rch_ptr->next_rch;
    }
    /* free handle list*/
    free_handle_list();

    /* restore search pointer */
    sdscb->srch_ptr = srch_ptr_hold;

    /* return search status flag */
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Reset_search_pattern(void)
 *
 *  DESCRIPTION
 *  Set search information
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Reset_search_pattern(void)
{
    /* free the search memory */
    SDS_free_search_data();
    /* set the maximum return size */
    sdscb->max_num_rch = 0;

    /* set the response items in the search */
    SDS_Reset_search_response();
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_Reset_search_response(void)
 *
 *  DESCRIPTION
 *  Set search response data
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_Reset_search_response(void)
{

    /* set transaction id */
    sdscb->transaction_id = 0;

    /* set handle db pointer and numbers */
    sdscb->handle_db_ptr = NULL;
    sdscb->number_handles = 0;
    sdscb->left_number_handles = 0;

    /* set current handle pointer */
    sdscb->current_hdl_ptr = NULL;

    /* set attribute db pointer and numbers */
    sdscb->attribute_db_ptr = NULL;
    sdscb->number_attributes = 0;
    sdscb->left_number_attributes = 0;
    sdscb->response_number_attributes = 0;
    sdscb->size_attributes = 0;
    sdscb->left_size_attributes = 0;
    sdscb->response_size_attributes = 0;
    sdscb->response_size_de = 0;
    sdscb->total_de_head_size = 0;
    sdscb->continuation_state = FALSE;

    /* set current attribute pointer */
    sdscb->current_attribute_ptr = NULL;
    /* set the PDU error command */
    sdscb->error_com = 0;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  boot_t validate_ts_data(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  Validate the data sent to SDS by the application
 *  1. Test the service record data sent by the application.  First test the
 *     size of the data is greater than zero.
 *  2. Then examine the service record in greater detail, for errors.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool validate_ts_data(SDS_REGISTER_REQ_T *sds_prim)
{
    CsrBool rnt_flag = TRUE;  /* set return flag, for correct response */

    /* test size of data from upper layer */
    if (sds_prim->num_rec_bytes == 0)
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    else if (!step_though_service_record(sds_prim->num_rec_bytes,
                                         sds_prim->service_rec))
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }

    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  boot_t validate_sds_dataread_ind(L2CA_DATAREAD_IND_T *prim)
 *
 *  DESCRIPTION
 *  Validate the size of the data sent to SDS by SDC
 *  1. test the data read indication data send by SDC.  First test the size of
 *     the data send is larger than just the PDU header.
 *  2. Then test the search data within the PDU.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool validate_sds_dataread_ind(L2CA_DATAREAD_IND_T *prim)
{
    CsrBool rnt_flag = TRUE; /* set return flag, for correct response */

    /* test size of data from SDC */
    if (prim->length > PDU_HEADER_SIZE)
    {
        /* test data from SDC in greater depth */
        rnt_flag = check_input_data(prim->length, (CsrUint8*)prim->data);
    }
    else
    {
        /* set error code */
        sdscb->error_code = PDU_ERROR_PDU_SIZE;

        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }

    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool check_input_data(CsrUint16 length, CsrUint8 *data)
 *
 *  DESCRIPTION
 *  Validate the data sent to SDS by SDC
 *  1. capture the size of the PDU, then test the value to make sure the
 *     size matches the length of the data primitive.
 *  2. then examine each search type separately, which is indicated by the
 *     first item in the PDU data
 *  3. the service search request, first test makes sure the search pattern
 *     header is correct, then that the number of the return handles is not
 *     equal to zero.  The final test is to step though the search pattern
 *     to make sure the data elements are correct.
 *  4. service attribute request, first test makes sure the search
 *     pattern header is correct, then that the size of the return attributes
 *     is greater then the minimum response size.  The final test is to step
 *     though the search pattern to make sure the data elements are correct.
 *     The final test is to step though the search patterns.
 *  5. service search attribute request, first test makes sure
 *     the first search pattern header is correct, then that the size of the
 *     return attributes is greater then the minimum response size. The second
 *     search pattern header is then tested to make sure it is correct
 *     The final test is to step though the search pattern to make sure the
 *     data elements are correct.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool check_input_data(CsrUint16 length, CsrUint8 *data)
{
    CsrBool rnt_flag = TRUE;     /* set return flag, for correct response */
    CsrUint16 pdu_size = 0;      /* size or search data from PDU*/
    CsrUint16 search_size = 0;   /* index to search data */
    CsrUint16 max_byte_size = 0; /* maximum return data size */
    CsrUint16 index = 0;         /* data element size */

    /* store the PDU size */
    pdu_size = SDP_UINT16_R(data, PDU_HEADER_LENGTH_INDEX);

    /* test the PDU size */
    if (pdu_size != length - PDU_HEADER_SIZE)
    {
        /* set error code */
        sdscb->error_code = PDU_ERROR_PDU_SIZE;
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    else
    {
        /* check for correct state */
        switch (data[0])
        {
            case SDP_SERVICE_SEARCH_REQUEST:
                /* test initial data element sequence */
                if ((data[PDU_HEADER_SIZE] & DATA_ELEMENT_TYPE_MASK) !=
                    DATA_ELEMENT_SEQUENCE)
                {
                    /* set error code */
                    sdscb->error_code = PDU_ERROR_SYNTAX;
                    /* set return flag to FALSE indicating an error */
                    rnt_flag = FALSE;
                }
                else /* test maximum byte size to be returned */
                {
                    /* store the data element size */
                    index = Data_Element_Size(data + PDU_HEADER_SIZE);
                    /* store the maximum return size */
                    max_byte_size = SDP_UINT16_R(data,
                                                 (CsrUint8)(index + PDU_HEADER_SIZE + 1));
                    /* test maximum return size is ok and the size of data */
                    /* within the search */
                    if ((max_byte_size == 0) ||
                        (!total_search_size(length,
                                            (CsrUint16)(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE),
                                            data + PDU_HEADER_SIZE,PDU_MAX_COUNT_SIZE)) )

                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                    else if (!step_though_sdc_data_element_sequence(
                                 &data[PDU_HEADER_SIZE])) /* test the search pattern */
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }

                }
                break;
            case SDP_SERVICE_ATTRIBUTE_REQUEST:
                /* test initial data element sequence */
                if ((data[PDU_HEADER_SIZE + PDU_HANDLE_SIZE + PDU_MAX_COUNT_SIZE]
                     & DATA_ELEMENT_TYPE_MASK) != DATA_ELEMENT_SEQUENCE)
                {
                    /* set error code */
                    sdscb->error_code = PDU_ERROR_SYNTAX;
                    /* set return flag to FALSE indicating an error */
                    rnt_flag = FALSE;
                }
                else /* test maximum byte size to be returned */
                {
                    /* store the maximum return size */
                    max_byte_size = SDP_UINT16_R(data,
                                                 (PDU_HEADER_SIZE + PDU_HANDLE_SIZE));
                    /* test maximum return size is ok and the size of data */
                    /* within the search */
                    if ( (max_byte_size < MIN_ATT_RESP_SIZE ) ||
                         (!total_search_size(length,
                                             (CsrUint16)(PDU_HEADER_SIZE + PDU_HANDLE_SIZE +
                                                        PDU_MAX_COUNT_SIZE),
                                             data + PDU_HEADER_SIZE + PDU_HANDLE_SIZE +
                                             PDU_MAX_COUNT_SIZE,0)) )
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                    else if (!step_though_sdc_data_element_sequence(
                                 &data[PDU_HEADER_SIZE + PDU_HANDLE_SIZE +
                                       PDU_MAX_COUNT_SIZE])) /* test the search pattern */
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }

                }
                break;
            case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:
                /* test initial data element sequence */
                if ((data[PDU_HEADER_SIZE] & DATA_ELEMENT_TYPE_MASK) !=
                    DATA_ELEMENT_SEQUENCE)
                {
                    sdscb->error_code = PDU_ERROR_SYNTAX;
                    /* set return flag to FALSE indicating an error */
                    rnt_flag = FALSE;
                }
                else /* test maximum byte size to be returned */
                {
                    /* store the data element size */
                    index = Data_Element_Size(data + PDU_HEADER_SIZE);
                    /* store the maximum return size */
                    max_byte_size = SDP_UINT16_R(data,
                            (CsrUint16)(index + PDU_HEADER_SIZE + 1));
                    /* test maximum return size is ok */
                    if (max_byte_size < MIN_SEARCH_ATT_RESP_SIZE)
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }

                    search_size = Data_Element_Size(data+PDU_HEADER_SIZE) +
                        PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE + 1;
                    /* test maximum return size is ok and the size of data */
                    /* within the search */
                    if ( ((data[search_size] & DATA_ELEMENT_TYPE_MASK) !=
                          DATA_ELEMENT_SEQUENCE) ||
                         (!total_search_size(length, search_size,
                                             data+search_size,0)) )
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                    else if (!step_though_sdc_data_element_sequence(
                                 &data[PDU_HEADER_SIZE])) /* test the search pattern */
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                    else if (!step_though_sdc_data_element_sequence(
                                 &data[search_size])) /* test the search pattern */
                    {
                        /* set error code */
                        sdscb->error_code = PDU_ERROR_SYNTAX;
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                }
                break;
            case SDP_ERROR_RESPONSE:
                /* no tests */
                break;
            default:
                /* error  */
                /* set error code */
                sdscb->error_code = PDU_ERROR_SYNTAX;
                /* set return flag to FALSE indicating an error */
                rnt_flag = FALSE;
                break;
        };
    }
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool step_though_sdc_data_element_sequence(CsrUint8 *data)
 *
 *  DESCRIPTION
 *  Validate the sdc data element sequence
 *  1. Steps though the search pattern supplied by SDC. If the first header in
 *     search pattern is ok, capture the size of the search pattern and examine
 *     the elements in the de sequence.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool step_though_sdc_data_element_sequence(CsrUint8 *data)
{
    CsrBool rnt_flag = TRUE;      /* set return flag, for correct response */
    CsrUint16 index = 0;          /* current index */
    CsrUint16 total_size = 0;     /* current size */
    CsrUint16 index_total = 0;    /* total index */


    /* test the type of data element header */
    if (DE_TYPE_ERROR != data_element_type(&data[index_total]))
    {
        /* calculate the size of the data element sequence header in the */
        /* search pattern */
        /* capture total data element sequence index */

        total_size = Data_Element_Size(data) + 1;

        /* capture the current index to next element */
        index = Data_Element_Index(data);

        /* store the total de index length */
        index_total = index;
        /* step though the data element sequence */
        if (!step_though_data_elements((CsrUint16)(total_size-index),
                                       &data[index_total]))
        {
            /* set return flag to FALSE indicating an error */
            rnt_flag = FALSE;
        }
    }
    else /* de header error */
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool step_though_data_service_record(CsrUint16 length, CsrUint8 *data)
 *
 *  DESCRIPTION
 *  validate the service record, by stepping though the service record elements
 *  1. step though the service record, attribute by attribute making sure the
 *     attribute header is correct.
 *  2. if correct step though the data element sequence data within the
 *     attribute, also making sure the headers are correct.
 *  3. if the data element header does not contain de sequence data,
 *     just index to the next attribute.
 *  4. Perform a final check on the size of the data found and make sure it
 *     matches the total for the service record.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool step_though_service_record(CsrUint16 length, CsrUint8 *data)
{
    CsrBool rnt_flag = TRUE;      /* set return flag, for correct response */
    CsrUint16 index = 0;          /* current index */
    CsrUint16 size = 0;           /* current size */
    CsrUint16 index_total = 0;    /* total index */
    CsrUint8 de_type = 0;         /* data element type */


    /* step though the service record */
    do
    {

        /* test element header attribute ID header*/
        if (ATTRIBUTE_HEADER_16BITS != SDP_UINT8_R(data,index_total))
        {
            /* the attribute ID header in error */
            /* set return flag to FALSE indicating an header error */
            rnt_flag = FALSE;
        }
        else
        {
            /* the attribute ID header ok */
            /* calculate the size of the first data element header in the */
            /* attribute and add it to the total */

            /* capture the size of the element */
            size = Data_Element_Size(data + index_total) + 1;
            /* add the current size to the total SR size */
            index_total += size;


            /* test the first data element header in the attribute */
            if (DE_TYPE_ERROR ==
                (de_type = data_element_type(&data[index_total])))
            {
                /* set return flag to FALSE indicating an de header error */
                rnt_flag = FALSE;
            }
            else if(DE_TYPE_SEQUENCE == de_type) /* header ok */
            {
                /* the first header is ok so examine the sequence of data */
                /* calculate the size and index of the sequence */

                /* capture the size of the element */
                size = Data_Element_Size(data + index_total) + 1;
                /* capture the current element index */
                index = Data_Element_Index(data + index_total);
                /* add the index to the total index size */
                index_total += index;


                /* test for data element sequence header */
                if ((SDP_UINT8_R(data,index_total) & DATA_ELEMENT_TYPE_MASK) ==
                    DATA_ELEMENT_SEQUENCE)
                {
                    /* step though the data element sequence */
                    if (!step_though_data_sequence((CsrUint16)(size-index),
                                                   &data[index_total]))
                    {
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                }
                else /* header not sequence data must be just element data */
                {
                    /* step though the data element sequence */
                    if (!step_though_data_elements((CsrUint16)(size-index),
                                                   &data[index_total]))
                    {
                        /* set return flag to FALSE indicating an error */
                        rnt_flag = FALSE;
                    }
                }
                /* add the size - the already added index to the total */
                /* index size */
                index_total += (size - index);
            }
            else /* header no sequence but data */
            {
                /* increment to the next attribute ID */

                /* capture the size of the data */
                size = Data_Element_Size(data + index_total)+1;
                /* add the current data size to the total index size */
                index_total += size;

            }
        }


    } while(index_total < length && rnt_flag);
    /* test the overall size of the service record against the incremented */
    /* size as the above stepped though */
    if (index_total != length)
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool step_though_data_sequence(CsrUint16 total_size, CsrUint8 *data)
 *
 *  DESCRIPTION
 *  step though and test the data element sequence
 *  1. step though the data element sequence, if the first data element
 *     sequence header is correct step though the data elements in the
 *     sequence.
 *  2. if the first data element sequence header is not a sequence header,
 *     flag an error and stop the sequence checking.
 *  3. if the first data element sequence header is just a data header index to
 *     the next item.
 *  4. Perform a final check on the size of the data found and make sure it
 *     matches the total for the data element sequence.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool step_though_data_sequence(CsrUint16 total_size, CsrUint8 *data)
{
    CsrBool rnt_flag = TRUE;      /* set return flag, for correct response */
    CsrUint16 index = 0;          /* current index */
    CsrUint16 size = 0;           /* current size */
    CsrUint16 index_total = 0;    /* total index */
    CsrUint8 de_type = 0;         /* data element type */


    /* step though the data element sequence */
    while (index_total < total_size && rnt_flag)
    {

        /* test the first data element sequence header type */
        if (DE_TYPE_ERROR == (de_type =
                              data_element_type(&data[index_total]))) /* header error */
        {
            /* set return flag to FALSE indicating an error */
            rnt_flag = FALSE;
        }
        else if (DE_TYPE_SEQUENCE == de_type) /* header ok */
        {
            /* the first header is ok so examine the sequence of data */
            /* capture the size of the element */
            size = Data_Element_Size(data + index_total) + 1;
            /* capture the current element index */
            index = Data_Element_Index(data + index_total);
            /* add the index to the total index size */
            index_total += index;


            /* step though the data elements in this sequence */
            if (!step_though_data_elements((CsrUint16)(size-index),
                                           &data[index_total]))
            {
                /* set return flag to FALSE indicating an error */
                rnt_flag = FALSE;
            }
            /* add the size - the already added index to the total */
            /* index size */
            index_total += (size - index);
        }
        else /* header not a sequence just data */
        {
            /* capture the size of the element */
            size = Data_Element_Size(data + index_total) + 1;

            /* add the current data size to the total index size */
            index_total += size;
        }
    }
    /* test the overall size of the Data Element Sequence against the */
    /* incremented size as the above stepped though */
    if (index_total != total_size)
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool step_though_data_elements(CsrUint16 total_size, CsrUint8 *data)
 *
 *  DESCRIPTION
 *  step though and test the data element headers
 *  1. step through though data elements until the end of the elements in this
 *     sequence or an error occurs.
 *  2. test each data element header, if it is not correct flag an error else
 *     increment to the next de header.
 *  3. Perform a final check on the size of the data found and make sure it
 *     matches the total for the data elements.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns FALSE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool step_though_data_elements(CsrUint16 total_size, CsrUint8 *data)
{
    CsrBool rnt_flag = TRUE;      /* set return flag, for correct response */
    CsrUint16 size = 0;           /* current size */
    CsrUint16 index_total = 0;    /* total index */


    /* step though the data element sequence */
    while (index_total < total_size && rnt_flag)
    {

        /* determine the element header type */
        if (data_element_type(data + index_total) == 0) /* header error */
        {
            /* set return flag to FALSE indicating an error */
            rnt_flag = FALSE;
        }
        else /* header ok */
        {

            /* capture the size of the element */
            size = Data_Element_Size(data + index_total) + 1;
            /* store the total element index length */
            index_total += size;

        }
    }
    /* test the overall size of the Data Elements against the */
    /* incremented size as the above stepped though */
    if (index_total != total_size)
    {
        /* set return flag to FALSE indicating an error */
        rnt_flag = FALSE;
    }
    return rnt_flag;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 data_element_type(CsrUint8 *data)
 *
 *  DESCRIPTION
 *  Define the data element header type
 *  1. determine the type of de header, by masking off the size information
 *     and then testing for known de header type. If the header is a sequence
 *     return 2, if not return 1. If not a known herder type return 0.
 *
 *  RETURNS/MODIFIES
 *  CsrUint8 rnt_flag returns 0 if error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint8 data_element_type(CsrUint8 *data)
{
    CsrUint8 rnt_flag = 0;      /* set return flag, for error response */

    CsrUint8 header = SDP_UINT8_R(data, 0);
    /* find the data element size */
    switch(header & DATA_ELEMENT_TYPE_MASK)
    {
        case DATA_ELEMNET_NULL:
        case DATA_ELEMNET_INT:
        case DATA_ELEMNET_TWO_COMP:
        case DATA_ELEMNET_UUID:
        case DATA_ELEMNET_TEXT:
        case DATA_ELEMNET_BOOL:
        case DATA_ELEMENT_URL:
            /* data but on in sequence form */
            rnt_flag = DE_TYPE_NON_SEQUENCE;
            break;
        case DATA_ELEMENT_SEQUENCE:
        case DATA_ELEMENT_ALT:
            /* data element sequence types */
            rnt_flag = DE_TYPE_SEQUENCE;
            break;
        default:
            /* error not data element */
            rnt_flag = DE_TYPE_ERROR;
            break;
    };

    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool register_service_record()
 *
 *  DESCRIPTION
 *  register the service record sent by the application
 *  1. calculate the service record size and create memory
 *  2. set the service record link pointer, size and handle
 *  3. copy the service record data and free the original data
 *  4, increment the service record handle
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns TRUE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool register_service_record(CsrUint16 prim_mem_size,
                               CsrUint8 *prim_service_rec)
{
    CsrBool rnt_flag = FALSE;  /* return flag */
    CsrUint8 *service_rec;     /* service record pointer */
    CsrUint16 mem_size = 0;    /* service record size */
    CsrUint16 index = 0;       /* current index */
    CsrUint8 i = 0;            /* counter */

    /* calculate the size of memory required for the service record */
    mem_size =  sizeof(CsrUint8 *) + SR_SIZE + SR_HANDLE_SIZE + sizeof(CsrUint8 *);

    /* create the memory for the service record header */
    service_rec = (CsrUint8 *) xpmalloc(mem_size);


    /* test memory allocation is ok */
    if(NULL != service_rec)
    {
        /* update link list pointers */
        set_pointer_service_record(service_rec);

        /* set pointer to next service record to NULL */
        for (i=0;i<sizeof(CsrUint8 *);i++)
        {
            service_rec[index++] = SET_DATA_ZERO;
        }

        /* add size of data to service record */
        SDP_UINT16_W(service_rec, index, prim_mem_size);

        /* increment the index to point to the SR handle */
        index += PDU_MAX_COUNT_SIZE;

        /* add service record handle, header and data */
        /* attribute ID and header */
        service_rec[index++] = ATTRIBUTE_HEADER_16BITS;
        service_rec[index++] = SERVICE_RECORD_HANDLE;  /* attribute id */
        service_rec[index++] = SERVICE_RECORD_HANDLE;  /* attribute id */

        /* add service record handle header */
        service_rec[index++] = ATTRIBUTE_HEADER_32BITS;
        /* add service record handle */
        SDP_UINT32_W(service_rec, index, sdscon.SR_Handle);

        /* increment the index to point to the SR data */
        index += PDU_ATTRIB_HANDLE_SIZE;

        /* CSR modification.
           CCL code originally put record and header in same pmalloc block.
           We now just put a pointer at the end of the header than can point
           to either pmalloc or const memory */
#ifdef BUILD_FOR_HOST
        writePtrValue( (CsrUint8 *) &(service_rec[index]), (CsrUint8 *) &prim_service_rec);
#else
        *((CsrUint8 **)&service_rec[index]) = prim_service_rec;
#endif


        /* increment the service record handle */
        sdscon.SR_Handle++;
    }
    else /* error in memory allocation */
    {
        /* The alloc() failed, so free the original data if it is in RAM */
        bpfree(prim_service_rec);

        /* set return flag to true */
        rnt_flag = TRUE;
    }

    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool set_pointer_service_record()
 *
 *  DESCRIPTION
 *  set the link list pointers to the service records
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns TRUE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void set_pointer_service_record(CsrUint8 *service_rec)
{
    CsrUint8 *previous_rc = NULL; /* pointer to the previous service record */

    if (NULL == sdsll.first_rc)  /* if first record */
    {
        /* set linklist first pointer */
        sdsll.first_rc = service_rec;
        /* set linklist current pointer */
        sdsll.current_rc = service_rec;
    }
    else  /* if not first record */
    {
        /* set linklist previous pointer */
        previous_rc = sdsll.current_rc;
        /* set linklist current pointer */
        sdsll.current_rc = service_rec;
        /* add pointer to last service record */
        (void) CsrMemCpy(previous_rc, &sdsll.current_rc, sizeof(CsrUint8 *));
    }

}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool unregister_service_record()
 *
 *  DESCRIPTION
 *  unregister a service record
 *  1. step through the service records
 *  2. store the service record handle and compare it to the one to be deleted
 *  3. if a match is found delete the service record and re-order the pointer
 *     links.
 *
 *  RETURNS/MODIFIES
 *  CsrBool rnt_flag returns TRUE if an error occurs
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool unregister_service_record(CsrUint32 handle)
{
    CsrBool rnt_flag = TRUE;         /* return flag */
    CsrUint32 srhandle;              /* service record handle */
    CsrUint8 *sr_ptr;                /* service record pointer */
    CsrUint8 *sr_ptr_last;           /* last service record pointer */

    /* start at first service record */
    sr_ptr_last = sr_ptr = sdsll.first_rc;

    /* test for first record and make sure the first record can't be */
    /* deleted */
    if (sr_ptr != NULL && handle != SERVER_RECORD_HANDLE)
    {
        /* service record present */
        /* step though the service records to find a */
        /* service record handle match */
        do
        {
            /* store the service record handle for comparison */
            srhandle = store_service_record_handle(sr_ptr +
                                                   SERVICE_DISCOVERY_SERVER_INDEX_HANDLE);

            /* test for service record handle match */
            if(srhandle == handle)
            {
                /* service record found */
                /* test to see if the service record is current */
                if (sdsll.current_rc == sr_ptr)
                {
                    /* the service record to be deleted is current, */
                    /* so we need to change the current pointer to point to */
                    /* the last service record */
                    sdsll.current_rc = sr_ptr_last;
                }
                /* store the data pointer to next service record in the */
                /* last service record */
                (void) CsrMemCpy(sr_ptr_last, sr_ptr, sizeof(CsrUint8 *));

                /* do we need to free the service record data? */
#ifdef BUILD_FOR_HOST
                {
                    CsrUint8    *tmp;

                    writePtrValue( (CsrUint8 *) &tmp, &(sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]) );
                    CsrPmemFree(tmp);
/*            CsrPmemFree( (CsrUint8*) SDP_UINT32_R(sr_ptr, SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE));*/
                }
#else
                bpfree((CsrUint16*)sr_ptr[SERVICE_DISCOVERY_SERVER_INDEX_HANDLE+PDU_HANDLE_SIZE]);
#endif

                /* free service record header */
                CsrPmemFree(sr_ptr);

                /* set pointer to NULL to end while loop */
                sr_ptr = NULL;
                /* set return flag the FALSE */
                rnt_flag = FALSE;
            }
            else /* no handle match */
            {
                /* set last pointer to current service record */
                sr_ptr_last = sr_ptr;
                /* pointer to the next service record */
                (void) CsrMemCpy(&sr_ptr, sr_ptr, sizeof(CsrUint8 *));
            }
        }
        while(sr_ptr != NULL); /* while more service records exist */
    }

    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool store_uuid(CsrUint8 *pointer, CsrBool search, CsrUint32 *uuid)
 *
 *  DESCRIPTION
 *  Stores the 128 bit UUID, from a 16, 32 or 128 bit uuid into an array
 *  of 4 CsrUint32's.
 *
 *  RETURNS/MODIFIES
 *  returns TRUE if UUID is valid, FALSE if it isn't.
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool store_uuid(CsrUint8 *pointer, CsrBool search, CsrUint32 *uuid)
{
    CsrUint8 uuid_header = SDP_UINT8_R(pointer,0);

    if (uuid_header == UUID_HEADER_128BITS)
    {
        /* copy all parts of the UUID */
        uuid[0] = SDP_UINT32_R(pointer, 1);
        uuid[1] = SDP_UINT32_R(pointer, 5);
        uuid[2] = SDP_UINT32_R(pointer, 9);
        uuid[3] = SDP_UINT32_R(pointer, 13);
    }
    else
    {
        /*
          The full 128-bit value of a 16-bit or 32-bit UUID may be
          computed by a simple arithmetic operation.

          128_bit_value = 16_bit_value * 2^96 + Bluetooth_Base_UUID
          128_bit_value = 32_bit_value * 2^96 + Bluetooth_Base_UUID
        */

        /* uuid[0] = BT_BASE_UUID0;
           optimised as we know it's zero and saves a 32bit add below */
        uuid[1] = BT_BASE_UUID1;
        uuid[2] = BT_BASE_UUID2;
        uuid[3] = BT_BASE_UUID3;

        if (uuid_header == UUID_HEADER_16BITS)
        {
            /* store the 16 bit UUID (top 16bits become zero) */
            uuid[0] = (CsrUint32)SDP_UINT16_R(pointer, 1);
        }
        else if(uuid_header == UUID_HEADER_32BITS)
        {
            /* store the 32 bit UUID */
            uuid[0] = SDP_UINT32_R(pointer, 1);
        }
        else
        {
            if (search)
            {
                /* uuid header failure, so set the error state and code */
                /* set error code */
                sdscb->error_code = PDU_ERROR_SYNTAX;
                sdscb->error_state = TRUE;
            }

            /* couldn't read the UUID */
            return FALSE;
        }
    }

    return TRUE;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint32 store_service_record_handle(CsrUint8 *pointer)
 *
 *  DESCRIPTION
 *  stores the service record handle
 *
 *  RETURNS/MODIFIES
 *  returns CsrBool service record handle
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint32 store_service_record_handle(CsrUint8 *pointer)
{
    CsrUint32 sr_handle;          /* service record handle */

    /* store the service record handle */
    sr_handle = SDP_UINT32_R(pointer, 0);

    return sr_handle;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void store_transaction_id(CsrUint8 *pointer)
 *
 *  DESCRIPTION
 *  stores the transaction id for the current search
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void store_transaction_id(CsrUint8 *pointer)
{
    /* store the transaction ID from search PDU, bytes 0 and 1 */
    sdscb->transaction_id = SDP_UINT16_R(pointer, 0);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  total_search_size(CsrUint16 length, CsrUint16 extra, CsrUint8 *data,
 *                    CsrUint8 offset)
 *
 *  DESCRIPTION
 *  add up the size of the search and make sure it fits within the PDU
 *  1. calculate the size of the current search pattern.
 *  2. test to see if there is a continuation state, if so calculate the
 *     size and add it to the extra data.
 *  3. test the size of the internal search pattern and extra PDU data
 *     against the PDU size.
 *  NOTE: the extra size will contain the size of the service search
 *  pattern in a service search attribute request.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrBool total_search_size(CsrUint16 length, CsrUint16 extra_size, CsrUint8 *data,
                         CsrUint8 offset)
{
    CsrUint16 size = 0;          /* size */
    CsrBool rnt_flag = FALSE;    /* return flag */

    /* find the size of the search pattern */
    size = Data_Element_Size(data) + 1;
    /* test the end of the PDU for the continuation state */
    if (data[size+offset] == PDU_CONTINUATION_NULL) /*  no continuation data*/
    {
        /* add one to the extra size to include the continuation state*/
        extra_size ++;
    }
    else /*  continuation data */
    {
        /* add the continuation state to the extra size */
        extra_size += data[size+offset] + 1;
    }

    /* test the size of the search pattern and the other data in the PDU */
    /* against the length of the PDU */
    if (size + extra_size == length)
    {
        /* the size is correct return a true state */
        rnt_flag = TRUE;
    }

    return rnt_flag;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_free_search_data(void)
 *
 *  DESCRIPTION
 *  Frees the search data
 *  1. test the search data exists, then frees the memory and sets the sizes to
 *     zero.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_free_search_data(void)
{
    /* test search data is present */
    if (sdscb->srch_size != 0)
    {
        /* free search data */
        CsrPmemFree(sdscb->srch_ptr);
        sdscb->srch_ptr = NULL;
        /* set search size */
        sdscb->srch_size = 0;
        /* set attribute size */
        sdscb->attr_size = 0;
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_set_return_max(CsrUint8 offset)
 *
 *  DESCRIPTION
 *  Sets the return maximum size for the search's
 *  1. Tests the response maximum required by the upper layer against the
 *     local mtu and the remote mtu and sets the return maximum used in the
 *     attribute search's to the lowest.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_set_return_max(CsrUint8 offset)
{
    CsrUint16 max;

    max = (sdscb->mtu_tx > sds_mtu_rx ? sds_mtu_rx : sdscb->mtu_tx) - offset;

    if (sdscb->max_num_rch > max)
        sdscb->max_num_rch = max;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_add_continuation(CsrUint8 continuation_state_size,
 *                            CsrUint16 data_size_left)
 *
 *  DESCRIPTION
 *  Add the continuation data to the end of the response to SDC
 *  1. Test the continuation state size, to see if there is any data
 *     remaining to be sent.
 *  2. If there is more data to be sent, add the size of the remaining data to
 *     the end of the response to SDC.
 *  3. If there is no more data to be sent, just add zero to the end of the
 *     response to SDC and free all the search data
 *
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_add_continuation(CsrUint8 continuation_state_size,
                          CsrUint16 data_size_left)
{
    /* add the continuation state data to the end of the response to SDC */
    /* test for the continuation state size required */
    if ( continuation_state_size != 0)
    {
        /* there is continuation data, so we need to add it to the end of */
        /* the response to SDC */
        /* add the continuation state length */
        sdscb->resp_ptr[sdscb->resp_size-continuation_state_size-1] =
            continuation_state_size;
        /* add the continuation state size for the 16 bit version */
        SDP_UINT16_W(sdscb->resp_ptr,
                     (CsrUint16)(sdscb->resp_size - BIT_16_INDEX),
                     data_size_left);
    }
    else
    {
        /* there is no continuation data, so add the std continuation data */
        /* of zero */
        /* set the continuation state */
        sdscb->resp_ptr[sdscb->resp_size-1] = PDU_CONTINUATION_NULL;
        /* there is no more data to be sent so we can clear the search data */
        /* free all attributes in linked list */
        free_attribute_list();
        /* free all service record handles in linked list */
        free_handle_list();
        /* set the search data */
        SDS_Reset_search_pattern();
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_create_pdu_header(CsrUint8 header_type, CsrUint16 data_size)
 *
 *  DESCRIPTION
 *  Creates the PDU header for the response to SDC
 *  1. Create the header type.  This is the search response type to SDC.
 *  2. Create the transaction ID. This is matching transaction number to the
 *     original request.
 *  3. Create the parameter length. This is the size of the data in the
 *     response.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_create_pdu_header(CsrUint8 header_type, CsrUint16 data_size)
{
    /* create the PDU header for the response to SDC*/

    /* PDU Header type */
    sdscb->resp_ptr[0] = header_type;
    /* PDU Header Transaction ID byte 2 */
    SDP_UINT16_W(sdscb->resp_ptr, PDU_HEADER_ID_INDEX, sdscb->transaction_id);
    /* PDU Header Parameter Length byte 2 */
    SDP_UINT16_W(sdscb->resp_ptr, PDU_HEADER_LENGTH_INDEX, data_size);
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_create_de_header(CsrUint8 header_type, CsrUint16 data_index,
 *                            CsrUint16 de_size)
 *
 *  DESCRIPTION
 *  Create the data element header in the response to SDC. 16 bit only.
 *  1. Create the header type.
 *  2. Create the size part of the de header with a 16 bit length.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_create_de_header(CsrUint8 header_type, CsrUint16 data_index,
                          CsrUint16 de_size)
{
    /* create the data element header */

    /* create the data element header type */
    sdscb->resp_ptr[data_index] = header_type;

    /* the de header type required is 16 bit size, so create 16 bit */
    /* length size information */
    SDP_UINT16_W(sdscb->resp_ptr, (CsrUint16)(data_index + 1), de_size);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_create_connection_data(void)
 *
 *  DESCRIPTION
 *  Create the connection data structure to hold the current connection data.
 *  1. Temporarily store the current connection pointer and create the memory
 *     to store the new connection data.
 *  2. Test to see if this is the only connection.
 *  3. If this is the only connection initialise the first linked list pointer.
 *  4. If there are already connections present, copy the current link list
 *     pointer in to the last connection data structure to create the link.
 *  5. Set the current connection pointer to the link list current connection
 *     data, initialise the connection data and increment the number of
 *     connections.
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_create_connection_data(void)
{
    SDSCB_T *sdscb_temp; /* temporary SDS structure pointer */


    /* store the current connection pointer, so we can set the next */
    /* pointer to point to the current connection data, if required */
    sdscb_temp = (SDSCB_T *)sdscon.current_connection;
    /* create memory to store the connection data */
    sdscon.current_connection = (CsrUint8 *)CsrPmemAlloc(sizeof(SDSCB_T));

    /* test to see if we have any connections present */
    if (NULL == sdscon.first_connection)
    {
        /* we have no connections present */
        /* set the first connection data pointer to the current, */
        /* as we only have one item so far */
        sdscon.first_connection = sdscon.current_connection;
    }
    else
    {
        /* we have current connections */
        /* set the last connection data next pointer to the current */
        /* connection data */
        sdscb_temp->next_connection = sdscon.current_connection;
    }

    /* set the current connection data pointer to the new connection data, */
    /* so we can fill it with the correct data */
    sdscb = (SDSCB_T *)sdscon.current_connection;
    /* initialise connection data */
    SDS_sdscb_initialise();
    /* test the number of connections are greater then zero */
    if (0 != sdscon.connection_number)
    {
        /* the number of connections is greater then zero, */
        /* so we have to set the previous pointer to create the link */
        /* set the current previous connection structure pointer to the */
        /* previous structure */
        sdscb->previous_connection = (CsrUint8 *)sdscb_temp;
    }
    /* increment the number of connections */
    sdscon.connection_number++;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_clear_connection(void)
 *
 *  DESCRIPTION
 *  Removes the connection data from the link list.
 *  1. Test to see if there are any connection items.
 *  2. If there are connection items, test to see if there is one connection.
 *     If there is only one item, set all the connection pointers to NULL.
 *  3. If there are more than one connection item, test to see then position
 *     in the linked list the current connection resides.
 *  4. If this is the first connection set the first connection pointer to the
 *     next item in the link list.
 *  5. If this is the last connection, set the current pointer to the previous
 *     item in the linked list and set its next pointer to NULL.
 *  6. Otherwise we must be in the middle of the liked list, so set the
 *     previous item to point to the next item in the list and the next item
 *     to point to the previous item.
 *  7. Decrement the connection counter, free the memory containing the
 *     connection data.
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_clear_connection(void)
{
    SDSCB_T *sdscb_temp_next; /* next temporary SDS structure pointer */
    SDSCB_T *sdscb_temp_prev; /* previous temporary SDS structure pointer */

    /* test to see if there is any connection data */
    if (NULL != sdscon.first_connection)
    {
        /* test the number of connections */
        if (1 == sdscon.connection_number)
        {
            /* there is one connection, so reset the link list pointers */
            /* set the first connection data pointer to NULL*/
            sdscon.first_connection = NULL;
            /* set the current connection data pointer to NULL */
            sdscon.current_connection = NULL;
        }
        else
        {
            /* there is more than one connection */
            /* set the temporary pointer to the next sdscb structure */
            sdscb_temp_next = (SDSCB_T *)sdscb->next_connection;
            /* set the temporary pointer to the previous sdscb structure */
            sdscb_temp_prev = (SDSCB_T *)sdscb->previous_connection;

            /* test to see if the connection data to be removed */
            /* is the first */
            if(sdscon.first_connection == (CsrUint8 *)sdscb)
            {
                /* the connection data to be removed is the first */
                /* one in the linked list */
                /* set the first connection data pointer the next */
                /* connection data item */
                sdscon.first_connection = sdscb->next_connection;
                /* set the pointer to the previous data to NULL */
                sdscb_temp_next->previous_connection = NULL;
            }
            else if(sdscon.current_connection == (CsrUint8 *)sdscb)
            {
                /* the connection data to be removed is the last */
                /* one in the linked list */
                /* set the current connection data pointer to the */
                /* new last connection data item */
                sdscon.current_connection = sdscb->previous_connection;
                /* set the pointer to the next data to NULL */
                sdscb_temp_prev->next_connection = NULL;
            }
            else
            {
                /* the connection data to be removed is in the */
                /* middle of the linked list */
                /* set the pointer to the next data to new next connection */
                /* structure */
                sdscb_temp_prev->next_connection = sdscb->next_connection;
                /* set the pointer to the previous data to new previous */
                /* connection structure */
                sdscb_temp_next->previous_connection = sdscb->previous_connection;
            }
        }
        /* decrement the number of connections */
        sdscon.connection_number--;
        /* free the connection data */
        CsrPmemFree(sdscb);
        /* set the current connection structure pointer to NULL, */
        /* to make sure there is no looking at the incorrect data */
        sdscb = NULL;
    }
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_initialise_connection_data(void)
 *
 *  DESCRIPTION
 *  Initialise the connection data structure
 *  1. Initialise the connection link list
 *  2. Initialise the Service Record handle
 *  3. Initialise the service record link list
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
void SDS_initialise_connection_data(void)
{

    /* set the pointer to the first connection data */
    sdscon.first_connection = NULL;

    /* set the pointer to the current connection data */
    sdscon.current_connection = NULL;

    /* set the number of connections */
    sdscon.connection_number = 0;

    /* set service record handle to the server handle */
    sdscon.SR_Handle = SERVER_RECORD_HANDLE;

    /* set sda phandle */
    sdscon.sda_phandle = CSR_SCHED_QID_INVALID_SDS;

    /* set service record state */
    sdscon.sr_state = SDS_ST_READY;

    /* set the pre-defined service record number */
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    sdscon.sr_dbase_number = 0;
#endif

    /* set the pointer to the connection structure */
    sdscb = NULL;

    /* set service record link list pointers */
    sdsll.current_rc = NULL;
    sdsll.first_rc = NULL;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 *SDS_find_next_service_record(CsrUint8 sr_handle,
 *                                        CsrUint8 *last_rc_ptr,
 *                                        CsrBool status_flag)
 *
 *  DESCRIPTION
 *  Find the next service record to be examined, during the current search.
 *  1. Test to see if the next service record is a pre-defined or a registered
 *     one.
 *  2. If the next service record is pre-defined, we need to create the
 *     service record to match the registered service record format.
 *  3. If the next service record is registered, we only need pass on the
 *     address of the service record.
 *  4. If the current service record did not have a match in the search and the
 *     service record is a temporary one we need to free its memory
 *
 *  RETURNS/MODIFIES
 *  returns pointer to the next service record in the current search
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
CsrUint8 *SDS_find_next_service_record(CsrUint8 sr_handle_count,
                                      CsrUint8 *last_rc_ptr,
                                      CsrBool status_flag)
{
    CsrUint8 *return_ptr; /* return service record pointer */


    /* test to see where the new service record is to come from */
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    if (sr_handle_count <= sdscon.sr_dbase_number && !status_flag)
    {
        /* this service record is pre-defined, has not been registered and */
        /* the current service record has not been successful in the current */
        /* search */
        /* so we need to create the service record so it matches the */
        /* registered ones */
        return_ptr = SDS_create_static_service_record(sr_handle_count);

    }
    else
#endif
    {
        /* this service record has been registered, so just */
        /* update the pointer to the next service record */
        (void) CsrMemCpy(&return_ptr, last_rc_ptr, sizeof(CsrUint8 *));

    }
    /* this is a temporary copy of a static/ps service record.  We can */
    /* free the memory as the service record handle or attribute has */
    /* been saved for the response to SDC */
#ifdef INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT
    SDS_clear_static_service_record((CsrUint8)(sr_handle_count-1),
                                    last_rc_ptr);
#endif

    /* return the pointer to the next service record in the current search */
    return return_ptr;
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrUint8 *SDS_create_static_service_record(CsrUint8 sr_handle_count)
 *
 *  DESCRIPTION
 *  Create the service record from the pre-defined service record.
 *  1. Discover the size of the memory required to hold the temporary service
 *     record.
 *  2. Create the memory to hold the temporary service record.
 *  3. Fill in the link pointer / size / service record handle plus header and
 *     the service record.  So the temporary service record matched the format
 *     of the registered service records.  NOTE: the link pointer is set to the
 *     same as the first service record, so the temporary service records point
 *     to the first registered service record.  This will enable the temporary
 *     service record to be slotted before the first registered service record.
 *
 *  RETURNS/MODIFIES
 *  returns pointer to the next temporary static service record
 *
 *---------------------------------------------------------------------------*/
#if defined(SDS_INCLUDE) && defined(INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT)
CsrUint8 *SDS_create_static_service_record(CsrUint8 sr_handle_count)
{
    CsrUint16   sr_data_size;     /* size of the service record data */
    CsrUint8    index = 0;        /* current index in the temporary service */
                                 /* record */
    CsrUint16   mem_size;         /* size of the temporary service record */
    CsrUint8    *service_rec;     /* temporary service record pointer */


    /* find the size of the static service record, the size just includes */
    /* the sr data */
    sr_data_size = SDS_DBASE_size_of_record(sr_handle_count);

    /* calculate the size of memory required for the temporary service record */
    mem_size =  sizeof(CsrUint8 *) + SR_SIZE + SR_HANDLE_SIZE + sr_data_size;

    /* create the memory for the service record */
    service_rec = (CsrUint8 *)CsrPmemAlloc(mem_size);

    /* test memory allocation is ok */
    if(NULL != service_rec)
    {
        /* set pointer to next service record, use the link from the first */
        /* service record so the temporary service records get added */
        /* in-between the server record and the registered service records */
        (void) CsrMemCpy(service_rec, sdsll.first_rc, sizeof(CsrUint8 *));

        /* set the index to point past the link address */
        index = sizeof(CsrUint8 *);

        /* add size of data to service record */
        SDP_UINT16_W(service_rec, index, (sr_data_size + SR_HANDLE_SIZE));

        /* increment the index to point to the SR handle */
        index += PDU_MAX_COUNT_SIZE;

        /* add service record handle, header and data */
        /* attribute ID and header */
        service_rec[index++] = ATTRIBUTE_HEADER_16BITS;
        service_rec[index++] = SERVICE_RECORD_HANDLE;  /* attribute id */
        service_rec[index++] = SERVICE_RECORD_HANDLE;  /* attribute id */

        /* add service record handle header */
        service_rec[index++] = ATTRIBUTE_HEADER_32BITS;
        /* add service record handle, adjust the handle count to start at 0, */
        /* so the first handle is 0x00010000 */

        SDP_UINT32_W(service_rec, index,
                     (CsrUint32)(SR_HANDLE_START - 1 + sr_handle_count));

        /* increment the index to point to the SR data */
        index += PDU_ATTRIB_HANDLE_SIZE;

        /* capture the static service record data and store it in the */
        /* temporary service record*/
        SDS_DBASE_get_sr(sr_handle_count, sr_data_size,
                         service_rec + index);
    }
    /* return the pointer to temporary service record */
    return service_rec;
}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_clear_static_service_record(CsrUint8 sr_handle_count,
 *                                       CsrUint8 *rc_ptr)
 *
 *  DESCRIPTION
 *  Clear the temporary service record from memory.
 *  1. Test to see if the service record is a temporary one.
 *  2. If it is free its memory.
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#if defined(SDS_INCLUDE) && defined(INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT)
void SDS_clear_static_service_record(CsrUint8 sr_handle_count, CsrUint8 *rc_ptr)
{

    /* test to see if the service record is a temporary one */
    if (sr_handle_count > 0 && sr_handle_count <= sdscon.sr_dbase_number)
    {
        /* the service record is a temporary one, so we can free it from */
        /* memory */
        CsrPmemFree(rc_ptr);
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_check_static_service_records(CsrUint8 static_number)
 *
 *  DESCRIPTION
 *  Steps through the pre-registered service records and test the formatting
 *  of the data elements just the same as when registering a service record.
 *  1. Step through the pre-registered service records.
 *  2. Find the size of the current service record and create memory and copy
 *     the pre-registered service record.
 *  3. Test the temporary service record as if it had just been registered, if
 *     there is problem with the formatting issue an error message.
 *  4. Free the temporary service record data.
 *
 *  RETURNS/MODIFIES
 *  returns none
 *
 *---------------------------------------------------------------------------*/
#if defined(SDS_INCLUDE) && defined(INSTALL_SDP_STATIC_SERVICE_RECORD_SUPPORT)
void SDS_check_static_service_records(CsrUint8 static_number)
{
    CsrUint16   sr_data_size;     /* size of the service record data */
    CsrUint8    index = 1;        /* current index in the temporary service */
                                 /* record */
    CsrUint8    *service_rec;     /* temporary service record pointer */


    /* step though the pre-registered service records */
    while (index <= static_number)
    {
        /* capture the size of the service record */
        sr_data_size = SDS_DBASE_size_of_record(index);
        /* create the memory for the service record */
        service_rec = (CsrUint8 *)CsrPmemAlloc(sr_data_size);
        /* capture the service records */
        SDS_DBASE_get_sr(index, sr_data_size, service_rec);
        /* test the service record to make sure the formatting is correct */
        if (!step_though_service_record(sr_data_size, service_rec))
        {
            /* there is a formatting error, so we need to issue and error */
            /* message */
            /* stop any further analysis */
            index = static_number;
            /* issue a major error message */
            BLUESTACK_PANIC(CSR_BT_PANIC_MYSTERY);
        }
        /* we have finished with the service record so we can free the */
        /* memory */
        CsrPmemFree(service_rec);
        /* increment the index counter to point at the next */
        /* pre-registered service record */
        index++;
    }
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDS_datawrite_req()
 *
 *  DESCRIPTION
 *        Checks to see that there is no pending datawrite confirm before sending
 *        data, otherwise waits
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
#ifdef SDS_INCLUDE
static void SDS_datawrite_req()
{

    if (sdscb->is_waiting_for_l2c_datawrite_confirm)
    {
        /*  Don't do anything ... */
        /* EMPTY */
    }
    else
    {
        L2CA_DataWriteReq(sdscb->cid, sdscb->resp_size, sdscb->resp_ptr);
        sdscb->resp_ptr = NULL;

        sdscb->is_waiting_for_l2c_datawrite_confirm = TRUE;
    }
}
#endif

