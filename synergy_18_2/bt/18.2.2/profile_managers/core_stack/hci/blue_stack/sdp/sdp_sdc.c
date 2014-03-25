/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sdc.c

DESCRIPTION:       This file contains the SDC functions.



****************************************************************************/

#include "csr_synergy.h"

/*============================================================================*
ANSI C & System-wide Header Files
*============================================================================*/
#include "csr_util.h"

#ifdef BUILD_FOR_HOST
#include "csr_bt_bluestack_types.h"
#else
#include "csr_bt_bluestack_types.h" /* path modified */
#endif

#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "error.h" /* path removed */
#include "csr_mblk.h" /* path removed */
#include "csr_bt_core_stack_fsm.h" /* path removed */
#include "csr_bt_tasks.h" /* path removed */
#include "csr_bt_core_stack_pmalloc.h" /* path removed */

/*============================================================================*
Interface Header Files
*============================================================================*/
#include "l2cap_prim.h" /* path modified */
#include "l2caplib.h" /* path removed */
#include "sdp_control.h" /* path removed */
#include "sdc_fsm_definitions.h" /* path removed */
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */
#include "sdp_sdc.h" /* path removed */
#include "sdp_sds.h" /* path removed */
#include "sdp_sda_interface.h" /* path removed */
#include "dm_prim.h" /* ENHANCEMENT_SDC_ERROR_CODES */

#ifdef SDC_INCLUDE
/*============================================================================*
Public Data
*============================================================================*/
static SDCCB_T sdccb;  /* SDC Data structure */
static DE_TYPE_T dehead; /* Data Element Search Data structure */
/*============================================================================*
Private Defines
*============================================================================*/
/* None */

/*============================================================================*
Private Data Types
*============================================================================*/
/* None */

/*============================================================================*
Private Function Prototypes
*============================================================================*/
static void SDCCB_Free(SDCCB_T *sdccb, CsrBool first_time);
static CsrBool SDC_BuildSearch_PDU(CsrUint8 continuation_size,
    CsrUint8 *p_continuation_data);
static void SDC_Reset_Search_Pattern(void);
static void SDC_Assemble_Results(L2CA_DATAREAD_IND_T *prim);
static void SDC_Dispatch_Results(void);
static void free_response_list(void);
static CsrBool SDC_validate_sda_data(CsrUint16 att_size, CsrUint8 *attrib_list,
       CsrUint16 sch_size, CsrUint8 *search_pttrn, CsrUint16 response_max,
       CsrUint8 srch_type);
static void SDC_set_return_max(CsrUint8 offset);
static void SDC_set_search_data(SDC_UPRIM_T *sdc_prim);
static void SDC_set_attr_data(SDC_UPRIM_T *sdc_prim);
static void SDC_set_search_attr_data(SDC_UPRIM_T *sdc_prim);
static void SDC_search_timer_alarm(CsrUint16 notused_uint16,
       void * notused_pointer);
static CsrBool SDC_test_search_size(CsrUint16 size, CsrUint8 offset);
static void SDC_datawrite_req(CsrUint16 pdu_size, void *pdu_ptr);
static CsrUint32 data_element_size(CsrUint8 *de_ptr);
static CsrUint8 des_length(CsrUint8 *de_ptr);
static CsrBool get_record_size(CsrMblk *p_mblk, CsrUint32 *p_size,
    CsrUint16 *p_des_size);
static void l2cap_autoconnect_req(void);
static fsm_event_t config_complete(void *arg1, void *arg2);

/*============================================================================*
Private Data
*============================================================================*/

/* Data Element Size codes */
static const CsrUint8 des_record_sizes[] =
{
    1, 2, 4, 8, 16, 0, 0, 0
};

static const CsrUint8 des_field_sizes[] =
{
    1, 1, 1, 1, 1, 2, 3, 5
};

/*============================================================================*
Public Function Implementations
*============================================================================*/

/*----------------------------------------------------------------------------*
 *  NAME
 *  void init_SDC(void)
 *
 *  DESCRIPTION
 *  Initialisation function
 *  1. Initialise all the connection data.
 *  2. Initialise all the search data.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void init_SDC(void)
{

        /* clear SDC data */
        SDCCB_Free(&sdccb, TRUE);
        /* clear any search requests */
    SDC_Reset_Search_Pattern();

}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received a connection confirm from l2cap.
 *  1. Test for the connecting state and test the response.
 *  2. If the response is successful instigate connection configuration.
 *  3. If the response is pending wait for success.
 *     Otherwise connection has failed, CsrSchedTidy up connection data and send a
 *     confirm with an error state to the application.
 *  4. Otherwise comment the state error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Auto_Connect_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T *)l2cap_prim;
    fsm_event_t event = SDC_EV_CONNECT_CFM_NEG;


    switch (prim->result)
    {
        case L2CA_CONNECT_INITIATING:
            /* Store CID */
            sdccb.cid = prim->cid;
            break;

        case L2CA_CONNECT_PENDING:
        case L2CA_CONNECT_RETRYING:
            /* Ignore */
            break;

        case L2CA_CONNECT_SUCCESS:
            /* call the state machine with connection confirm positive */
            event = SDC_EV_CONNECT_CFM_POS;

            /* Fallthrough */
        default:
            if (sdccb.cid == prim->cid)
                (void) fsm_16bit_run(&sdc_fsm, &sdccb.state, event, prim, 0);
            break;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Indication from l2cap.
 *  1. Respond to the disconnect indication with a disconnect response.
 *  2. Test for the configure or connection states, instigate connection CsrSchedTidy
 *     up, via the state machine.
 *  2. Otherwise comment the state error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Disconnect_Indication(L2CA_UPRIM_T *l2cap_prim)
{

    L2CA_DISCONNECT_IND_T *prim = (L2CA_DISCONNECT_IND_T *)l2cap_prim;

    /* send disconnect response to L2CAP */
    L2CA_DisconnectRsp(prim->identifier, prim->cid);

    /* call the state machine with the disconnect indication */
    fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_DISCONNECT_IND, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Disconnect Confirm from l2cap.
 *  1. Test for not the ready and the connecting states for a connection
 *     disconnection confirm and instigate the connection CsrSchedTidy up.
 *  2. Otherwise comment the state error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Disconnect_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DISCONNECT_CFM_T *prim = (L2CA_DISCONNECT_CFM_T *)l2cap_prim;

    PARAM_UNUSED(prim);

    /* call the state machine with the disconnect confirmation */
    fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_DISCONNECT_CFM, 0, 0);
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received Datawrite Confirm from l2cap.
 *  1. Test for the communicating state and there is a search currently in
 *     operation.  If we are in the communicating state and there is a search
 *     currently operating, then test write confirm result.
 *     If the state in the confirm is not a success, instigate the connection
 *     disconnection, as we cannot send data via L2CAP.
 *  2. Otherwise comment on the error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Datawrite_Confirm(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DATAWRITE_CFM_T *prim = (L2CA_DATAWRITE_CFM_T *)l2cap_prim;

    /* test write result if  WRITE_SUCCESS wait for dataread indication */
    if (prim->result != L2CA_DATA_SUCCESS)
    {
        /* call the state machine with the datawite confirm negative */
        fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_DATAWRITE_CFM_NEG, 0, 0);
    }
    else
    {
        /* call the state machine with the datawite confirm */
        fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_DATAWRITE_CFM, 0, 0);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
 *
 *  DESCRIPTION
 *  Received dataread indication from SDS with PDU data via l2cap.
 *  1. Test for the communicating state and that there is a search currently
 *     operating.  If we are in the communicating state and there is a search
 *     currently operating, test the PDU data.
 *     If the PDU data is ok continue to store the PDU search data.
 *     Otherwise respond with an error to SDS and send the appropriate response
 *     to the upper layer.
 *  2. Otherwise free the data and indicate an error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Dataread_Indication(L2CA_UPRIM_T *l2cap_prim)
{
    L2CA_DATAREAD_IND_T *prim = (L2CA_DATAREAD_IND_T *)l2cap_prim;

    /* Convert MBLK into pmalloc, store back into primitive */
    CsrMblkSize mblk_length = CsrMblkGetLength(prim->data);
    CsrUint8 *data_ptr = CsrPmemAlloc(mblk_length);
    CsrMblkCopyToMemory(prim->data, 0, mblk_length, data_ptr);
    CsrMblkDestroy(prim->data);
    prim->data = (CsrMblk*)data_ptr;
    prim->length = mblk_length;

    /* test SDS response data */
    if (prim->length) /* SDS response data ok */
    {
        /* call the state machine with the dataread indication */
        fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_DATAREAD_IND,
                      0, prim);
    }

    CsrPmemFree(prim->data);
    prim->data = NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Search Request from the upper layer.
 *  1. Test the search type to make sure there is not a search currently taking
 *     place
 *  2. Test the size of the search will fit into the mtu L2CAP packet.
 *  3. Test the search data to make sure it is ok.
 *  4. Test for the ready state, store the search parameters. Initiate the
 *     connection.
 *  5. Test for the communicating state and that the connection was opened by a
 *     open search, store the search parameters. Initiate the search.
 *  6. Otherwise send a confirm indicating the search is currently busy/error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Search_Request(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_SEARCH_REQ_T *prim = (SDC_SERVICE_SEARCH_REQ_T *)sdc_prim;
    CsrUint16 search_error = SDC_RESPONSE_SUCCESS; /* search error state */

    /* test the to see if an search is currently running search data */
    if (0 != dehead.srch_type)
    {
        /* there is a search currently running, so set the response to busy */
        search_error = SDC_SEARCH_BUSY;
    }
    else if (SDC_validate_sda_data(0, NULL, prim->size_srch_pttrn,
                                   prim->srch_pttrn, prim->max_num_recs,
                                   SDP_SERVICE_SEARCH_REQUEST))
    {
        /* Pass the search into the state machine, which may return an error
        * via &search_error.
        */
        if (fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_SEARCH_REQ, prim,
            &search_error) == FSM_RESULT_FAIL)
        {
            /* The state machine is not in the right state for a search */
            search_error = SDC_SEARCH_BUSY;
        }
    }
    else
    {
        /* the search data has an error, so set the response to error */
        search_error = SDC_SEARCH_DATA_ERROR;
    }
    /* test the search data error flag for an error/busy */
    if (SDC_RESPONSE_SUCCESS != search_error)
    {
        /* the search data has an error or there is a search currently */
        /* taking place send an error response */
        /* free primitive data */
        CsrPmemFree(prim->srch_pttrn);

        /* send confirm with error/busy */
        Send_SDC_Search_Cfm(prim->phandle, 0, 0, NULL, search_error, 0, &sdccb.bd_addr);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Attribute_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Attribute Request from the upper layer.
 *  1. Test the search type to make sure there is not a search currently taking
 *     place
 *  2. Test the size of the search will fit into the mtu L2CAP packet.
 *  3. Test the search data to make sure it is ok.
 *  4. Test for the ready state, store the search parameters. Initiate the
 *     connection.
 *  5. Test for the communicating state and that the connection was opened by a
 *     open search, store the search parameters. Initiate the search.
 *  6. Otherwise send a confirm indicating the search is currently busy/error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Attribute_Request(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *prim =
        (SDC_SERVICE_ATTRIBUTE_REQ_T *)sdc_prim;
    CsrUint16 search_error = SDC_RESPONSE_SUCCESS; /* search error state */


    /* test the to see if an search is currently running search data */
    if (0 != dehead.srch_type)
    {
        /* there is a search currently running, so set the response to busy */
        search_error = SDC_SEARCH_BUSY;
    }
    else if (SDC_validate_sda_data(prim->size_attr_list,
        prim->attr_list,0, NULL, prim->max_num_attr,
        SDP_SERVICE_ATTRIBUTE_REQUEST))
    {
        /* Pass the search into the state machine, which may return an error
        * via &search_error.
        */
        if (fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_ATTRIBUTE_REQ, prim,
            &search_error) == FSM_RESULT_FAIL)
        {
            /* The state machine is not in the right state for a search */
            search_error = SDC_SEARCH_BUSY;
        }
    }
    else
    {
        /* the search data has an error, so set the response to error */
        search_error = SDC_SEARCH_DATA_ERROR;
    }
    /* test the search data error flag for an error/busy */
    if (SDC_RESPONSE_SUCCESS != search_error)
    {
        /* the search data has an error or there is a search currently */
        /* taking place send an error response */
        /* free primitive data */
        CsrPmemFree(prim->attr_list);

        /* send confirm with error, search in progress */
        Send_SDC_Attribute_Cfm(prim->phandle, 0, NULL, search_error, 0, &sdccb.bd_addr);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Search_Attribute_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Search Attribute Request from the upper layer.
 *  1. Test the search type to make sure there is not a search currently taking
 *     place
 *  2. Test the size of the search will fit into the mtu L2CAP packet.
 *  3. Test the search data to make sure it is ok.
 *  4. Test for the ready state, store the search parameters. Initiate the
 *     connection.
 *  5. Test for the communicating state and that the connection was opened by a
 *     open search, store the search parameters. Initiate the search.
 *  6. Otherwise send a confirm indicating the search is currently busy/error.
 *
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Search_Attribute_Request(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *prim =
        (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *)sdc_prim;
    CsrUint16 search_error = SDC_RESPONSE_SUCCESS; /* search error state */

    /* test the to see if an search is currently running search data */
    if (0 != dehead.srch_type)
    {
        /* there is a search currently running, so set the response to busy */
        search_error = SDC_SEARCH_BUSY;
    }
    else if (SDC_validate_sda_data(prim->size_attr_list, prim->attr_list,
                                   prim->size_srch_pttrn, prim->srch_pttrn,
                                   prim->max_num_attr,
                                   SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST))
    {
        /* Pass the search into the state machine, which may return an error
        * via &search_error.
        */
        if (fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_SEARCHATTRIBUTE_REQ,
                          prim, &search_error) == FSM_RESULT_FAIL)
        {
            /* The state machine is not in the right state for a search */
            search_error = SDC_SEARCH_BUSY;
        }
    }
    else
    {
        /* the search data has an error, so set the response to error */
        search_error = SDC_SEARCH_DATA_ERROR;
    }
    /* test the search data error flag for an error/busy */
    if (SDC_RESPONSE_SUCCESS != search_error)
    {
        /* the search data has an error or there is a search currently */
        /* taking place send an error response */
        /* free primitive data */
        CsrPmemFree(prim->srch_pttrn);
        CsrPmemFree(prim->attr_list);

        /* send confirm with error/busy */
        Send_SDC_SearchAttribute_Cfm(prim->phandle, 0, 0, NULL, FALSE,
                                     search_error, 0, &sdccb.bd_addr);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Terminate_Primative_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Terminate Primitive Request from the upper layer.
 *  1. Test to see if a search is currently in operation and the phandle. If
 *     there is a search in operation and the phandle matches, we can proceed
 *     to terminate the search request. First by setting the terminate
 *     primitive flag.
 *  2. Then test for the configuring or communicating state. If there is a match
 *     we can terminate the search request primitive immediately.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Terminate_Primative_Request(SDC_UPRIM_T *sdc_prim)
{
    SDC_TERMINATE_PRIMITIVE_REQ_T *prim = (SDC_TERMINATE_PRIMITIVE_REQ_T *)sdc_prim;

    /* test the to see if a search currently running and the phandle matched */
    /* the curent phandle */
    if (0 != dehead.srch_type &&
        sdccb.sda_phandle_current == prim->phandle)
    {
        /* call the state machine with the terminate primitives */
        fsm_16bit_run(&sdc_fsm, &sdccb.state, SDC_EV_TERMINATE_PRIMATIVE, 0,
                      0);
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Open_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Open Search Request from the upper layer.
 *  1. Test for the correct state, if in the ready state where no connection
 *     is presently open, initiate the opening of the connection.
 *  2. Otherwise test the current connection bd address and the existing
 *     phandle, if a match is found with the bd address and the phandle
 *     the connection is already open, so increment the connection counter and
 *     issue the confirm.  If the match is no successful a connection is open
 *     to another bd address and respond with a busy state confirm.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Open_Search_Request(SDC_UPRIM_T *sdc_prim)
{

    SDC_OPEN_SEARCH_REQ_T *prim = (SDC_OPEN_SEARCH_REQ_T *)sdc_prim;

    /* test the current state */
    if (sdccb.state == SDC_ST_READY)
    {
        /* during ready state the first open search */
        sdccb.sda_phandle_number++;
        /* call the state machine with the search open request */
        (void) fsm_16bit_run(&sdc_fsm, &sdccb.state,
                             SDC_EV_OPEN_SEARCH_REQ, sdc_prim, 0);
    }
    else
    {
        /* test incomming prim bd address against current bd address and */
        /* the phandle matched the curent phandle */
        if ((sdccb.state == SDC_ST_COMMUNICATING) &&
            bd_addr_eq(&sdccb.bd_addr, &prim->bd_addr) &&
            (sdccb.sda_phandle_current == prim->phandle))
        {
            /* we are currently connected to the bd address, so increment */
            /* the number of upper layers connected */
            /* and send a open search cfm with the open state */
            /* increment handle record */
            sdccb.sda_phandle_number++;
            /* send open search conformation */
            Send_SDC_opensearch_Cfm(prim->phandle, (CsrUint16)SDC_OPEN_SEARCH_OPEN);
        }
        else
        {
            /* we are currently connected to another bd address, send a open */
            /* search cfm with a busy state */
            Send_SDC_opensearch_Cfm(prim->phandle, (CsrUint16)SDC_OPEN_SEARCH_BUSY);
        }
    }

}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Close_Search_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Close Search Request from the upper layer.
 *  1. Test for the correct state, if in the communicating state and the
 *     connection was opened with the open search command then initiate
 *     the closing of the connection, only if this is the last close request
 *     that matches the open request.
 *  2. Otherwise indicate an error.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Close_Search_Request(SDC_UPRIM_T *sdc_prim)
{
    PARAM_UNUSED(sdc_prim);

    /* Check that there was a persistent connection */
    if (sdccb.sdc_open_search && sdccb.sda_phandle_number)
    {
        /* during communicating state */
        /* decrement handle record */
        sdccb.sda_phandle_number--;
        /* test handle record reached the last one */
    if (sdccb.sda_phandle_number == 0)
    {
            /* call the state machine with the close search request */
        (void) fsm_16bit_run(&sdc_fsm, &sdccb.state,
                          SDC_EV_CLOSE_SEARCH_REQ, 0, 0);
    }
    }

    /* close request when there is no connection, there is no response */
    /* so no action is taken. */
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Config_Request(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Received Configure Request from the upper layer.
 *  1. test the the MTU against the minimum MTU value,
 *     if greater set the local MTU.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_Config_Request(SDC_UPRIM_T *sdc_prim)
{
    SDC_CONFIG_CFM_T *cfm_prim;

    /* set current MTU size */
    if (sdc_prim->sdc_config_req.l2cap_mtu >= L2CA_MTU_MINIMUM)
        sdccb.mtu_rx = sdc_prim->sdc_config_req.l2cap_mtu;

    /* _REQ and _CFM prims identical except for type. */
    cfm_prim = (SDC_CONFIG_CFM_T*)pcopy(sdc_prim, sizeof(SDC_CONFIG_CFM_T));
    cfm_prim->type = SDC_CONFIG_CFM;

    /* Report actual MTU. */
    cfm_prim->l2cap_mtu = sdccb.mtu_rx;

    /* All flags currently ignored. */
    cfm_prim->flags = 0;

    Send_SDP_Message(cfm_prim);
}

/*============================================================================*
State Machine Action Function Implementations
*============================================================================*/

fsm_event_t sdc_fn_search_req(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;

    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Service search request received from the application.
        Start the make the connection.
    */


    /* Store the search details */
    SDC_set_search_data(p_prim);

    l2cap_autoconnect_req();

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/
fsm_event_t sdc_fn_search_req_open(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;
    CsrUint16 *p_error = (CsrUint16 *) arg2;
    fsm_event_t ret = FSM_EVENT_NULL;

    /*
        This function is part of the SDC state machine.
        Service search request received from the application while the
        connection is open.
        Start to build the search PDU.
    */

    if (p_prim->sdc_service_search_req.phandle != sdccb.sda_phandle_current)
    {
        /* Wrong client, reject the request */
        *p_error = SDC_SEARCH_BUSY;
        return FSM_EVENT_NULL;
    }

    /* Store the search details */
    SDC_set_search_data(p_prim);

    /* initilaize the search return maximum */
    dehead.return_max = dehead.response_max;

    /* Build search */
    if (!SDC_BuildSearch_PDU(0, NULL))
    {
        /* Failed to build the search - cause an error response to be issued */
        ret = SDC_EV_SEARCH_COMPLETE_OPEN;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
fsm_event_t sdc_fn_attrib_req(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;

    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Service attribute request received from the application.
        Start the make the connection.
    */

    /* Store the search details */
    SDC_set_attr_data(p_prim);

    /* send connection req */
    l2cap_autoconnect_req();

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/
fsm_event_t sdc_fn_attrib_req_open(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;
    CsrUint16 *p_error = (CsrUint16 *) arg2;
    fsm_event_t ret = FSM_EVENT_NULL;

    /*
        This function is part of the SDC state machine.
        Service attribute request received from the application while the
        connection is open.
        Start to build the search PDU.
    */

    if (p_prim->sdc_service_attribute_req.phandle != sdccb.sda_phandle_current)
    {
        /* Wrong client, reject the request */
        *p_error = SDC_SEARCH_BUSY;
        return FSM_EVENT_NULL;
    }

    /* Store the search details */
    SDC_set_attr_data(p_prim);

    /* set the return maximum required for the attribute request */
    SDC_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
        CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);

    /* Build search */
    if (!SDC_BuildSearch_PDU(0, NULL))
    {
        /* Failed to build the search - cause an error response to be issued */
        ret = SDC_EV_SEARCH_COMPLETE_OPEN;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_search_attrib_req(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;

    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Service search attribute request received from the application.
        Start the make the connection.
    */

    /* Store the search details */
    SDC_set_search_attr_data(p_prim);

    /* send connection request */
    l2cap_autoconnect_req();

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_search_attrib_req_open(void *arg1, void *arg2)
{
    SDC_UPRIM_T *p_prim = (SDC_UPRIM_T *) arg1;
    CsrUint16 *p_error = (CsrUint16 *) arg2;
    fsm_event_t ret = FSM_EVENT_NULL;

    /*
        This function is part of the SDC state machine.
        Service search attribute request received from the application while
        the connection is open.
        Start to build the search PDU.
    */

    if (p_prim->sdc_service_search_attribute_req.phandle != sdccb.sda_phandle_current)
    {
        /* Wrong client, reject the request */
        *p_error = SDC_SEARCH_BUSY;
        return FSM_EVENT_NULL;
    }

    /* Store the search details */
    SDC_set_search_attr_data(p_prim);

    /* set the return maximum required for the search attribute request */
    SDC_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
        CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);

    /* Build search */
    if (!SDC_BuildSearch_PDU(0, NULL))
    {
        /* Failed to build the search - cause an error response to be issued */
        ret = SDC_EV_SEARCH_COMPLETE_OPEN;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_connect_cfm_neg(void *arg1, void *arg2)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T *)arg1;
    PARAM_UNUSED(arg2);

    /* Set the response code and defer to the sdc_fn_disconnected() function */
    dehead.response_response = prim->result;

    if (!sdccb.sdc_open_search)
    {
        dehead.finished = TRUE;
    }

    return sdc_fn_disconnected(arg1, arg2);
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_connect_cfm_pos(void *arg1, void *arg2)
{
    L2CA_AUTO_CONNECT_CFM_T *prim = (L2CA_AUTO_CONNECT_CFM_T *)arg1;
    PARAM_UNUSED(arg2);

    sdccb.mtu_tx = prim->config.mtu;

    /*
    This function is part of the SDC state machine.
    A connection confirm with a positive state has been recived from l2cap.
    If a termination command has been recived, start to disconnect the
    connection.
    Otherwise start the configuration of the connection.
    */

    /* test to see if we have recived a terminate primitive command */
    if (sdccb.connect_trm_prim)
    {
        /* we have recived a terminate primitive command from the */
        /* upper layer */
        /* send a disconnect request to close the connection */
        L2CA_DisconnectReq(sdccb.cid);
        return FSM_EVENT_NULL;
    }

    return config_complete(arg1, arg2);
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_terminate_primative(void *arg1, void *arg2)
{
    fsm_event_t ret = FSM_EVENT_NULL;

    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Terminate primitive has been received from the upper layer.
        These is a search currently operating, test how the connection
        was created.
        If the connection was created with a search request,
        close the connection. The terminate primitive confirm will be sent to
        the upper layer when the disconnct confirm is recived.
        If the connection was created with the open search command, just create
        the response to the upper layer and clear the search data.
    */

    /* Remember that termination has been requested */
    sdccb.connect_trm_prim = TRUE;

    /* Set the current search to finished, with a terminated response */
    dehead.response_response = SDC_RESPONSE_TERMINATED;
    dehead.finished = TRUE;

    /* Return a search complete event, which will respond to the user */
    if (sdccb.sdc_open_search)
    {
        /* Connection was opened with the open search command, so
         * keep the connection open.
         */
        ret = SDC_EV_SEARCH_COMPLETE_OPEN;
    }
    else
    {
        /* Connection was opened with the search request command, so
         * now disconnect.
         */
        ret = SDC_EV_SEARCH_COMPLETE;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/

fsm_event_t config_complete(void *arg1, void *arg2)
{
    fsm_event_t ret = FSM_EVENT_NULL;

    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);
    /*
        This function is part of the SDC state machine.
        The connection configuration has been successfully been completed,
        if the connection was opened by a search request calculate the correct
        max return size then start to build the search PDU,
        else the connection was opened by the open search
        command so send the open search conformation to the application.
    */

    /* test the way the connection was created */
    if (!sdccb.sdc_open_search)
    {
        /* connection opened with the service request */
        /* set the return maximum required for the search/attribute */
        /* request */
        SDC_set_return_max(PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                           CONTINUATION_SIZE_16BIT + CONTINUATION_SIZE);

        /* test the search type, if search request increment to the */
        /* correct offset */
        if (SDP_SERVICE_SEARCH_REQUEST == dehead.srch_type)
        {
             /* initilaize the search return maximum */
             dehead.return_max = dehead.response_max;
        }

        /* Build search */
        if (!SDC_BuildSearch_PDU(0, NULL))
        {
            /* Failed to build the search - cause an error response to be issued */
            ret = SDC_EV_SEARCH_COMPLETE;
        }
    }
    else
    {
        /* connection opened with the open search request */
        /* send open search cfm to the application */
     Send_SDC_opensearch_Cfm(sdccb.sda_phandle_current,
                            (CsrUint16)SDC_OPEN_SEARCH_OK);
    }
    return ret;
}


/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_datawrite_cfm(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
    L2CAP has sent data, therefore can send some more
    */

    if (sdccb.pdu_ptr)
    {
        L2CA_DataWriteReq(sdccb.cid, sdccb.pdu_size, sdccb.pdu_ptr);
        sdccb.pdu_ptr = NULL;

        /* set the search event timer just after we send the search request */
        /* to SDS. Storing the timer event id, so we can clear the timer if */
        /* the search response is received  before the timer expires. */
        timer_start(&dehead.evenCsrSchedTid,
                    SEARCH_TIMER,
                    SDC_search_timer_alarm,
                    0,
                    NULL);

        sdccb.is_waiting_for_l2c_datawrite_confirm = TRUE;
    }
    else
    {
        sdccb.is_waiting_for_l2c_datawrite_confirm = FALSE;
    }


    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_datawrite_cfm_neg(void *arg1, void *arg2)
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
    L2CA_DisconnectReq(sdccb.cid);

    /* set the error responses to be use in the search/open confirm */
    dehead.response_response = SDC_DATA_CFM_ERROR;
    dehead.response_error = 0;
    dehead.finished = TRUE;

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t  sdc_fn_dataread_ind(void *arg1, void *arg2)
{
    L2CA_DATAREAD_IND_T *sdc_prim = (L2CA_DATAREAD_IND_T *)arg2;
    CsrUint16 transaction;
    fsm_event_t ret = FSM_EVENT_NULL;

    PARAM_UNUSED(arg1);

    /*
        This function is part of the SDC state machine.
        Data Read Indication has been received from l2cap.
        If there is a terminate primitive or a disconnection taking place
        just ignore the new data and await the disconnection to clear the
        flags.
        Store the search response data. Only if the transaction id matches
        the expected one and we are not in the process of terminating the
        primitive.
    */

    /* Process the data unless we're terminating or there has been an error */
    if ((SDC_RESPONSE_SUCCESS == dehead.response_response) &&
        !sdccb.connect_trm_prim)
    {
        /* store the transaction ID from response PDU, bytes 1 and 2 in */
        /* the PDU header */
        transaction = SDP_UINT16_R(sdc_prim->data, PDU_HEADER_ID_INDEX);

        /* test the trasaction id received in the response PDU with the
           expected */
        if(sdccb.transaction == transaction)
        {
            /* Assemble the results from and decide if the transaction has
             * finished.
             */
            SDC_Assemble_Results(sdc_prim);
            if (dehead.finished)
            {
                /* The transaction has completed (in success or error), so we
                 * return a completion event to the state machine which will
                 * cause the final responses to be sent to the application.
                 * We can't send the final responses here because we might need
                 * to disconnect first.
                 */
                if (sdccb.sdc_open_search)
                {
                    /* Connection was opened with the open search command, so
                     * keep the connection open.
                     */
                    ret = SDC_EV_SEARCH_COMPLETE_OPEN;
                }
                else
                {
                    /* Connection was opened with the search request command, so
                     * now disconnect.
                     */
                    ret = SDC_EV_SEARCH_COMPLETE;
                }
            }
            else
            {
                /* The transaction has not yet completed, but see if there are
                 * any intermediate results to send to the application.
                 */
                SDC_Dispatch_Results();
            }
        }
    }

    return ret;
}


/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_search_complete(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        The search has been completed. So disconnect the connection. Send the
        response when we recive the disconnect cfm.
    */


    /* start to disconnect the connection */
    L2CA_DisconnectReq(sdccb.cid);

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_search_complete_open(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        The search has been completed after an open search,
        so send the response back to the application.  Then reset the search
        pattern data.
    */

    /* Send any final results to the application */
    SDC_Dispatch_Results();

    /* reset search patten */
    SDC_Reset_Search_Pattern();

    return FSM_EVENT_NULL;
}


/*---------------------------------------------------------------------------*/


fsm_event_t sdc_fn_disconnect_ind(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /* Set the response for the current search to disconnected */
    if (0 != dehead.srch_type)
    {
        dehead.response_response = SDC_CON_DISCONNECTED;
        dehead.finished = TRUE;
    }

    /* Handle the disconnection */
    return sdc_fn_disconnected(arg1, arg2);
}


/*---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 *  NAME
 *      sdc_fn_disconnected
 *
 *  DESCRIPTION
 *      The connection to the server has been disconnected, it does not matter
 *      whether disconnection was initiated locally or remotely. Send the
 *      appropriate event to the application.
 *
 *      If an open search request has failed
 *          issue open search cfm(fail).
 *      Else
 *          If there is a search in progress
 *              issue a search cfm with the current response code.
 *          If it is a multiple-search connection
 *              Issue a close search ind.
 *
 *----------------------------------------------------------------------------*/
fsm_event_t sdc_fn_disconnected(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /* Response code will only be less than SDC_ERRORCODE_BASE if it contains
       an L2CAP sourced error. These are only set if a connect or config
       request fails.
    */
    if (sdccb.sdc_open_search &&
        (SDC_CONFIGURE_ERROR == dehead.response_response ||
         (dehead.response_response != SDC_RESPONSE_SUCCESS &&
          dehead.response_response < SDC_ERRORCODE_BASE)))
    {
        /* Open search has failed during the L2CAP configuration phase,
         * tell the user.
         */
        Send_SDC_opensearch_Cfm(sdccb.sda_phandle_current,
                                dehead.response_response);

        sdccb.sda_phandle_number--;
    }
    else
    {
        if (dehead.srch_type)
        {
            /* Send a search completion primitive to the user */
            SDC_Dispatch_Results();
        }

        if (sdccb.sdc_open_search)
        {
            /* This was a persistent connection, so tell the user it has gone */
            Send_SDC_CloseSearch_Ind(sdccb.sda_phandle_current,
                                     (CsrUint16)SDC_SDC_DISCONNECTED);
        }

        /* reset the flag indicating user opened this connection */
        sdccb.sdc_open_search = FALSE;

    }

    /* Clear search parameters */
    SDC_Reset_Search_Pattern();

    /* Clear connection parameters */
    SDCCB_Free(&sdccb, FALSE);

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_open_search_req(void *arg1, void *arg2)
{
    SDC_OPEN_SEARCH_REQ_T *prim = (SDC_OPEN_SEARCH_REQ_T *)arg1;

    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Open search request has been received from the application.
        Store the connection details and start the connection.
    */

    /* set the open search flag to indicate the connection was opened with */
    /* the open search command and no the actual search request */
    sdccb.sdc_open_search = TRUE;

    /* store the bd address for the device we are to connect to */
    sdccb.bd_addr = prim->bd_addr;

    /* store the phandle from the upper layer */
    sdccb.sda_phandle_current = prim->phandle;

    /* start the connection by sending a connection request */
    l2cap_autoconnect_req();

    return FSM_EVENT_NULL;
}

/*---------------------------------------------------------------------------*/

fsm_event_t sdc_fn_close_search_req(void *arg1, void *arg2)
{
    PARAM_UNUSED(arg1);
    PARAM_UNUSED(arg2);

    /*
        This function is part of the SDC state machine.
        Close search request has been received from the upper layer.
        Issue a disconnect request to end the connection and test to see if
        a search is currently in opperation. If there is set the error
        response.
    */

    /* send the disconnect request to close the connection */
    L2CA_DisconnectReq(sdccb.cid);

    /* test to see if there is a search currenly operating */
    if (0 != dehead.srch_type)
    {
        /* there is a search currently in operation */
        /* set the error responses to be use in the search confirm */
        dehead.response_response = SDC_CON_DISCONNECTED;
        dehead.response_error = 0;
        dehead.finished = TRUE;
    }
    else
    {
        dehead.response_response = SDC_SDC_DISCONNECTED;
    }

    return FSM_EVENT_NULL;
}


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDCCB_Free(SDCCB_T *sdccb, CsrBool first_time)
 *
 *  DESCRIPTION
 *  Set connection information
 *  1. Set the connection details after a connetion has been terminated. The
 *     first time flag is used to denote the initial initialisation at
 *     hard reset.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *----------------------------------------------------------------------------*/

void SDCCB_Free(SDCCB_T *sdccb, CsrBool first_time)
{
    CsrUint8 state = sdccb->state;
    CsrUint16 mtu_rx = sdccb->mtu_rx;

    CsrMemSet(sdccb, 0, sizeof(SDCCB_T));

    /* Handle first-time initialisation */
    if (first_time)
    {
        sdccb->state = SDC_ST_READY;
        sdccb->mtu_rx = L2CA_MTU_MINIMUM;
    }
    else
    {
        /* Restore certain parameters */
        sdccb->state = state;
        sdccb->mtu_rx = mtu_rx;
    }

    /* reset remote mtu */
    sdccb->mtu_tx = L2CA_MTU_DEFAULT;
}


/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool sdc_cid_match(cid_t cid)
 *
 *  DESCRIPTION
 *  Compare the stored CID to the primitive CID, to ascertain if the l2cap
 *  information destination is for SDC.
 *
 *  RETURNS/MODIFIES
 *  CsrBool return_flag set to TRUE is the CID's match
 *
 *----------------------------------------------------------------------------*/

CsrBool sdc_cid_match(l2ca_cid_t cid)
{
    return (sdccb.cid == cid);
}


/*----------------------------------------------------------------------------*
 *  NAME
 *  SDC_BuildSearch_PDU
 *
 *  DESCRIPTION
 *  Build the search PDU to send to SDS, including the supplied continuation
 *  data if present.
 *  1. Test there is a search to build.
 *  2. Test the size of the search will fit into the L2CAP packet. If not set
       the error response and return FALSE.
 *  3. Create the search. Create the memory for the search and then fill in
 *     the PDU header data.
 *  4. Test for the specific search type and create the search data.
 *  5. Add the supplied continuation data to the end of the PDU.
 *  6. Send search PDU to the SDS.
 *
 *  RETURNS/MODIFIES
 *  CsrBool - FALSE if the search request cannot be built
 *
 *----------------------------------------------------------------------------*/

CsrBool SDC_BuildSearch_PDU(
                           CsrUint8 continuation_size,
                           CsrUint8 *p_continuation_data
                           )
{
    CsrUint16 size_pdu = 0;        /* search PDU size */
    CsrUint8 *pdu_ptr = NULL;      /* search PDU pointer */

    /* test to see if there is a search in operation */
    if (0 != dehead.srch_type)
    {
        /* test the search size will fit into the L2CAP packet */
        if (!SDC_test_search_size((CsrUint16)(dehead.sch_size + dehead.att_size),
            (CsrUint8)(dehead.pdu_offset + continuation_size)))
        {
            /* the search will not fit into the L2CAP packet */
            /* set the error responses to be use in the search response */
            /* to the upper layer */
            dehead.response_response = SDC_SEARCH_SIZE_TO_BIG;
            dehead.response_error = 0;
            dehead.finished = TRUE;

            return FALSE;
        }
        else
        {
            /* the search will fit into the L2CAP packet */
            /* set PDU transcation id */
            sdccb.transaction++;

            /* set size of PDU */
            size_pdu = (CsrUint16)dehead.sch_size + dehead.att_size +
                dehead.pdu_offset + continuation_size;
            /* create the memory for the PDU */
            pdu_ptr = (CsrUint8 *)CsrPmemAlloc(size_pdu);

            /* add PDU id */
            pdu_ptr[0] = dehead.srch_type;

            /* add transaction id */
            SDP_UINT16_W(pdu_ptr,PDU_HEADER_ID_INDEX,sdccb.transaction);

            /* add parameter length */
            SDP_UINT16_W(pdu_ptr,PDU_HEADER_LENGTH_INDEX,
                (CsrUint16)(dehead.sch_size + dehead.att_size +
                (dehead.pdu_offset - PDU_HEAD_SIZE) + continuation_size));

            /* test for the correct search type */
            switch (dehead.srch_type)
            {
            case SDP_SERVICE_SEARCH_REQUEST:

                /* copy search parameters */
                (void) CsrMemCpy(pdu_ptr + PDU_HEAD_SIZE, dehead.search_pttrn,
                    dehead.sch_size);

                /* add maximum record count */
                SDP_UINT16_W(pdu_ptr,(CsrUint16)(dehead.sch_size+PDU_HEAD_SIZE),
                    (CsrUint16)(dehead.return_max / PDU_HANDLE_SIZE));
                break;
            case SDP_SERVICE_ATTRIBUTE_REQUEST:

                /* add record handle */
                SDP_UINT32_W(pdu_ptr,PDU_HEAD_SIZE,dehead.svc_rec_hndle);

                /* add maximum record count */
                SDP_UINT16_W(pdu_ptr,(PDU_HEAD_SIZE + PDU_ATTRIB_HANDLE_SIZE),
                    dehead.return_max);

                /* copy attribute parameters */
                (void) CsrMemCpy(pdu_ptr + PDU_HEAD_SIZE + PDU_ATTRIB_HANDLE_SIZE +
                    PDU_MAX_COUNT_SIZE, dehead.attrib_list, dehead.att_size);
                break;
            case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:

                /* copy search parameters */
                (void) CsrMemCpy(pdu_ptr + PDU_HEAD_SIZE, dehead.search_pttrn,
                    dehead.sch_size);

                /* add maximum record count */
                SDP_UINT16_W(pdu_ptr,(CsrUint16)(dehead.sch_size+PDU_HEAD_SIZE),
                    dehead.return_max);

                /* copy attribute parameters */
                (void) CsrMemCpy(pdu_ptr + dehead.sch_size + PDU_HEAD_SIZE +
                    PDU_MAX_COUNT_SIZE, dehead.attrib_list, dehead.att_size);
                break;
            default:
                /* error state search type not correct */
                break;
            }

            /* Include the continuation data */
            pdu_ptr[size_pdu - continuation_size - 1] = continuation_size;
            if (p_continuation_data)
            {
                CsrMemCpy(pdu_ptr+(size_pdu-continuation_size),
                    p_continuation_data, continuation_size);
            }

            /* send datawrite request with search */
            SDC_datawrite_req(size_pdu, pdu_ptr);
        }
    }
    else
    {
        /* error state there is no search type */
        pdu_ptr = NULL;
    }

    return TRUE;
}


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Reset_Search_Pattern()
 *
 *  DESCRIPTION
 *  Sets the search pattern
 *  1. Clears all the search related data, so there is no lagecy data
 *     remaining for when a new search is received.  The first time flag is
 *     used to denote the initial initialisation at hard reset.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

void SDC_Reset_Search_Pattern()
{
    if(dehead.sch_size != 0)
    {
        /* free srch pattern */
        CsrPmemFree(dehead.search_pttrn);
    }

    if(dehead.att_size != 0)
    {
        /* free attribute list */
        CsrPmemFree(dehead.attrib_list);
    }

    /* Free any responses */
    free_response_list();

    timer_cancel(&dehead.evenCsrSchedTid);

    /* Zero everything */
    CsrMemSet(&dehead, 0, sizeof(DE_TYPE_T));

    /* Initialise response to success */
    dehead.response_response = SDC_RESPONSE_SUCCESS;

    /* Reset the termination request flag */
    sdccb.connect_trm_prim = FALSE;
}


/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_Assemble_results()
 *
 *  DESCRIPTION
 *  Assembles the search results from SDS
 *  1. Add the response data to the list of responses.
 *  2. Update length counters.
 *  3. Extract any continuation data from the response and use it to send a
 *     continuation request.
 *  4. Set a response code in dehead.response_response - SDC_RESPONSE_SUCCESS
 *     if everything is OK, or some other error code if there was a problem
 *     meaning that the search must be stopped.
 *  5. The 'dehead.finished' flag is filled in to indicate to the caller
 *     whether the transaction has now completed.
 *
 *  The next step for the caller is to call the SDC_Dispatch_Results() function,
 *  which looks at the current search type, the data received so far, and the
 *  response code, in order to decide whether to send a response to the user.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/

void SDC_Assemble_Results(L2CA_DATAREAD_IND_T *prim)
{
    CsrUint8 *p_data;
    CsrUint8 *p_continuation_data;
    CsrUint16 rx_length = 0;         /* size of attributes or handles */
    CsrUint8 continuation_size = 0;  /* size of continuation data */
    CsrMblk *p_response;
    L2CA_DATAREAD_IND_T *p_prim = prim;
    CsrUint8 pdu_id = ((CsrUint8 *)(p_prim->data))[0];

    /* we have just received a response to a search request. */
    /* we must clear the search timer that was set after the search */
    /* request was sent to SDS.  This will stop the timer from expiring, */
    /* causing erroneous error messages. */
    timer_cancel(&dehead.evenCsrSchedTid);

    /* Check we have received the response we were expecting. */
    if ((pdu_id == SDP_SERVICE_SEARCH_RESPONSE &&
         dehead.srch_type == SDP_SERVICE_SEARCH_REQUEST) ||
        (pdu_id == SDP_SERVICE_ATTRIBUTE_RESPONSE &&
         dehead.srch_type == SDP_SERVICE_ATTRIBUTE_REQUEST) ||
        (pdu_id == SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE &&
         dehead.srch_type == SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST) ||
         pdu_id == SDP_ERROR_RESPONSE)
    {
        switch(pdu_id)
        {
            case SDP_SERVICE_SEARCH_RESPONSE:
                /* drop though to SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE state */
            case SDP_SERVICE_ATTRIBUTE_RESPONSE:
                /* drop though to SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE state */
            case SDP_SERVICE_SEARCH_ATTRIBUTE_RESPONSE:

                /* Test the type of response, to calculate the size of data received */
                if(pdu_id == SDP_SERVICE_SEARCH_RESPONSE)
                {
                    /* If this is the first response, store the total response size,
                     * which is the (size of a handle) * (TotalServiceRecordCount).
                     */
                    if(dehead.build_number == 0)
                    {
                        dehead.total_response = (CsrUint32) PDU_HANDLE_SIZE *
                                                (SDP_UINT16_R(p_prim->data, PDU_HEAD_SIZE));
                    }

                    /* Calculate the size of data in this response, which is
                     * (size of a handle) * (CurrentServiceRecordCount).
                     */
                    rx_length = PDU_HANDLE_SIZE * SDP_UINT16_R(p_prim->data,
                                                               (PDU_HEAD_SIZE + PDU_MAX_COUNT_SIZE));

                    /* Calculate the offset to the start of the response data */
                    p_data = (CsrUint8 *)p_prim->data + PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                             PDU_MAX_COUNT_SIZE;
                }
                else
                {
                    /* If this is the first response, store the total response size,
                     * which is given by the initial DES descriptor. Note we do not
                     * include the initial DES descriptor in the response to the user.
                     */
                    if(dehead.build_number == 0)
                    {
                        CsrUint8 initial_des_length;

                        dehead.total_response = data_element_size((CsrUint8 *)p_prim->data +
                                                                  PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE);

                        /* For the first response, exclude the initial DES from the
                         * attribute data.
                         */
                        initial_des_length = des_length((CsrUint8 *)p_prim->data + PDU_HEADER_SIZE +
                                                        PDU_MAX_COUNT_SIZE);
                        rx_length = SDP_UINT16_R(p_prim->data, PDU_HEAD_SIZE) - initial_des_length;
                        p_data = (CsrUint8 *)p_prim->data + PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE +
                                 initial_des_length;
                    }
                    else
                    {
                        /* Calculate the offset and size of the data */
                        rx_length = SDP_UINT16_R(p_prim->data, PDU_HEAD_SIZE);
                        p_data = (CsrUint8 *)p_prim->data + PDU_HEADER_SIZE + PDU_MAX_COUNT_SIZE;
                    }
                }

                /* Get the continuation data size and a pointer to the start of it */
                continuation_size = p_data[rx_length];
                p_continuation_data = p_data + rx_length + 1;

                /* Allocate MBLK for the new response */
                p_response = CsrMblkDataCreate(p_prim->data,
                                              (CsrMblkSize)(p_prim->length - 1 - continuation_size),
                                              TRUE);
                if(!p_response)
                {
                    /* We have run out of memory for the response */
                    dehead.response_response = SDC_RESPONSE_OUT_OF_MEMORY;
                    dehead.response_error = 0;
                    dehead.finished = TRUE;

                    /* The data memory is freed at the end of this function */
                    break;
                }

                /* Drop header from start. This *may* destroy the MBLK encase
                 * no more data is present. */
                CsrMblkDiscardHead(&p_response,
                                  (CsrMblkSize)(p_data - (CsrUint8 *)(p_prim->data)));

                /* Remove memory block from primitive to prevent it from being freed */
                p_prim->data = NULL;

                /* Add the response to the list. MBLK may have been discarded! */
                if(p_response != NULL)
                {
                    dehead.p_response_head = CsrMblkAddTail(p_response, dehead.p_response_head);
                }

                /* store size of parameters */
                dehead.rx_total += rx_length;
                /* increment the number of searches responses */
                dehead.build_number++;

                /* test the response totals add up */
                /* the total response from all the responses is less than the */
                /* current total or */
                /* the response maximum to the upper layer has not been exeeded */
                if(dehead.rx_total > dehead.total_response)
                {
                    /* an error has occured, the response is to large, stop */
                    /* asking for more data and tell the SDS an error has ocured */
                    /* set the response state and error, for the resonse */
                    /* to the upper layer */
                    dehead.response_response = SDC_RESPONSE_PDU_SIZE_ERROR;
                    dehead.response_error = 0;
                    dehead.finished = TRUE;
                }
                else if(rx_length > dehead.response_max)
                {
                    /* an error has occured, the response is to large, stop */
                    /* asking for more data */
                    /* set the response state and error, for the response */
                    /* to the upper layer */
                    dehead.response_response = SDC_RESPONSE_PDU_SIZE_ERROR;
                    dehead.response_error = 0;
                    dehead.finished = TRUE;
                }
                else if(continuation_size == 0)
                {
                    /* continuation data is not present no more data is required */
                    /* set the response state and error, for the resonse */
                    /* to the upper layer */
                    dehead.response_response = SDC_RESPONSE_SUCCESS;
                    dehead.response_error = 0;
                    dehead.finished = TRUE;
                    /* test the build size to make sure there is suffucuent data */
                    /* present */
                    if(dehead.rx_total < PDU_MIN_SIZE)
                    {
                        /* there is not data to send, so set the response to no */
                        /* response data */
                        dehead.response_response = SDC_NO_RESPONSE_DATA;
                        dehead.response_error = 0;
                        dehead.finished = TRUE;
                        /* make sure we don't send the data to the upper layer, */
                        /* by setting the build number to zero. */
                        /* NOTE: the response link list will be cleared */
                        /* when the response is created for the upper layer */
                        dehead.build_number = 0;
                    }
                }
                else
                {
                    /* continuation data is present more data required */
                    /* Re-build the search request for more data. I suppose this could
                     * fail if the addition of continuation data takes the message
                     * over the MTU size.
                     */
                    if(!SDC_BuildSearch_PDU(continuation_size, p_continuation_data))
                    {
                        dehead.finished = TRUE;
                    }
                }
                return;

            case SDP_ERROR_RESPONSE:
                /* capture error parameter */
                dehead.response_error = SDP_UINT16_R(p_prim->data, PDU_HEAD_SIZE);

                /* set the response status */
                dehead.response_response = SDC_ERROR_RESPONSE_PDU;
                dehead.finished = TRUE;
                return;
        }
    }

    /* error state the PDU header is incorrect */
    /* set the response state and error, for the resonse */
    /* to the upper layer */
    dehead.response_response = SDC_RESPONSE_PDU_HEADER_ERROR;
    dehead.response_error = 0;
    dehead.finished = TRUE;

}

/*----------------------------------------------------------------------------*
 *  NAME
 *      SDC_Dispatch_Results
 *
 *  DESCRIPTION
 *      This function, which may be called several times during a single
 *      transaction, examines the status of the current search responses and
 *      decides whether to send a response to the application. For the service
 *      and service-attribute searches, this happens when all response data
 *      has been received, and a single response is issued. For the
 *      service-search-attribute transaction, this function will send all
 *      individual service records to the application, each in a single
 *      primitive - so the application will receive several primitives during
 *      the transaction.
 *
 *      If there has been an error (dehead.response_response != SUCCESS), then
 *      this function will issue a final response indicating the unsuccessful
 *      completion of the transaction.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/

static void SDC_Dispatch_Results(void)
{
    CsrUint8 *p_data = NULL;
    CsrUint16 response = dehead.response_response;
    CsrUint16 count = 0;

    /* send search information to SDA */
    switch (dehead.srch_type)
    {
    case SDP_SERVICE_SEARCH_REQUEST:
    case SDP_SERVICE_ATTRIBUTE_REQUEST:
        /* For these two searches, the response data is collated into a single
        * buffer for the application when finished.
        */
        if (!dehead.finished)
        {
            break;
        }

        if (dehead.response_response == SDC_RESPONSE_SUCCESS)
        {
            /* Build up the data for the application */
            count = CsrMblkGetLength(dehead.p_response_head);
            if (count)
            {
                p_data = (CsrUint8 *) xpmalloc(count);
                if (p_data)
                {
                    /* Copy the responses into the buffer, freeing the MBLKs
                    * along the way.
                    */
                    CsrMblkReadHead(&dehead.p_response_head, p_data, count);
                }
                else
                {
                    response = SDC_RESPONSE_OUT_OF_MEMORY;
                    count = 0;
                }
            }

            /* Send the response */
            if (dehead.srch_type == SDP_SERVICE_SEARCH_REQUEST)
            {
                Send_SDC_Search_Cfm(sdccb.sda_phandle_current,
                    count, (CsrUint16) (count / PDU_HANDLE_SIZE),
                    p_data, response, dehead.response_error, &sdccb.bd_addr);
            }
            else
            {
                Send_SDC_Attribute_Cfm(sdccb.sda_phandle_current,
                    count, p_data, response, dehead.response_error, &sdccb.bd_addr);
            }
        }
        else
        {
            /* There was an error, ignore any received data */
            if (dehead.srch_type == SDP_SERVICE_SEARCH_REQUEST)
            {
                Send_SDC_Search_Cfm(sdccb.sda_phandle_current, 0, 0, NULL,
                    dehead.response_response, dehead.response_error, &sdccb.bd_addr);
            }
            else
            {
                Send_SDC_Attribute_Cfm(sdccb.sda_phandle_current, 0, NULL,
                    dehead.response_response, dehead.response_error, &sdccb.bd_addr);
            }
        }
        break;

    case SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST:
        {
            CsrUint32 record_size = 0;
            CsrUint16 des_size;
            CsrUint16 bytes_left;
            CsrBool des_valid;

            /* First check for an error */
            if (dehead.response_response != SDC_RESPONSE_SUCCESS)
            {
                Send_SDC_SearchAttribute_Cfm(sdccb.sda_phandle_current, 0, 0, NULL,
                    FALSE, dehead.response_response, dehead.response_error, &sdccb.bd_addr);
                dehead.finished = TRUE;
                break;
            }

            /* The data pointer at the head of the received responses list
            * points to a DES descriptor for a complete service record. Therefore
            * we analyse the DES descriptor to determine if the complete record
            * has been received yet. If so, the record is sent to the application.
            * This is done in a loop, as there may be multiple records to send up.
            * The minor complication is that the DES descriptor itself may be
            * split across responses.
            *
            * As the received data is sent to the application, we keep freeing
            * MBLKs from the list as we finish with them.
            *
            * The DES descriptor is not included in the record sent to the
            * application, so we must be careful to take account of this in any
            * calculations.
            */
            des_valid = get_record_size(dehead.p_response_head, &record_size,
                &des_size);
            bytes_left = CsrMblkGetLength(dehead.p_response_head);
            while (des_valid && ((record_size + des_size) <= bytes_left))
            {
                /* Advance the data pointer over the DES descriptor */
                CsrMblkDiscardHead(&dehead.p_response_head, des_size);

                /* Build the application response */
                p_data = (CsrUint8 *) xpmalloc((size_t) record_size);
                if (p_data)
                {
                    CsrMblkReadHead(&dehead.p_response_head, p_data,
                        (CsrMblkSize) record_size);
                }
                else
                {
                    CsrMblkDiscardHead(&dehead.p_response_head,
                        (CsrMblkSize) record_size);
                }

                /* See how many bytes are left to process */
                bytes_left = CsrMblkGetLength(dehead.p_response_head);

                if (p_data)
                {
                    /* Send the service record to the application */
                    Send_SDC_SearchAttribute_Cfm(sdccb.sda_phandle_current,
                        (CsrUint16) dehead.total_response, (CsrUint16) record_size, p_data,
                        (CsrBool) (!dehead.finished || bytes_left),
                        SDC_RESPONSE_SUCCESS, 0, &sdccb.bd_addr);
                }
                else
                {
                    /* We can't send this service record to the application, but
                    * we let the application know.
                    */
                    Send_SDC_SearchAttribute_Cfm(sdccb.sda_phandle_current,
                        (CsrUint16) dehead.total_response, 0, NULL,
                        (CsrBool) (!dehead.finished || bytes_left),
                        SDC_RESPONSE_OUT_OF_MEMORY, 0, &sdccb.bd_addr);
                }

                /* Is the next record complete? */
                des_valid = get_record_size(dehead.p_response_head, &record_size,
                    &des_size);
            }

            /* If the finished flag is set, make sure that we do finish otherwise
            * invalid data from the server could leave us hanging around for ever.
            */
            if (dehead.finished && (bytes_left > 0))
            {
                /* Some data is left that we could not handle - send a cfm */
                Send_SDC_SearchAttribute_Cfm(sdccb.sda_phandle_current,
                    (CsrUint16) dehead.total_response, 0, NULL, FALSE, SDC_RESPONSE_SUCCESS,
                    0, &sdccb.bd_addr);
            }
        }
        break;

    default:
        break;
    }

    if (dehead.finished)
    {
        /* Release any leftover response data */
        free_response_list();
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void free_response_list(void)
 *
 *  DESCRIPTION
 *  Frees list of service response
 *  1. The search response to the application can be built up of many
 *     responses from the SDS.
 *     A linked list of responses is created so each one can be combined
 *     to make one response back the application.
 *     This function steps though the linked list and frees the memory. But
 *     only ocures if there are any lists present.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void free_response_list(void)
{
    CsrMblkDestroy(dehead.p_response_head);
    dehead.p_response_head = NULL;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool SDC_validate_sda_data(CsrUint16 att_size, CsrUint8 *attrib_list,
 *      CsrUint16 sch_size, CsrUint8 *search_pttrn, CsrUint16 response_max,
 *      CsrUint8 srch_type)
 *
 *  DESCRIPTION
 *  Validate the data to be sent to SDS by SDA
 *  1. Test that the search data header is formatted correctly and the
 *     search, attribute data size and maximum return values
 *     are not zero..
 *  2. If an error is discovered the error flag is set the false, this will
 *     instigate an error response to the upper layer.
 *
 *  RETURNS/MODIFIES
 *  FALSE iff error found
 *
 *---------------------------------------------------------------------------*/
CsrBool SDC_validate_sda_data(CsrUint16 att_size, CsrUint8 *attrib_list,
       CsrUint16 sch_size, CsrUint8 *search_pttrn, CsrUint16 response_max,
       CsrUint8 srch_type)
{
    /* Validate the ServiceSearchPattern, if there is one */
    if (srch_type != SDP_SERVICE_ATTRIBUTE_REQUEST)
    {
        /* Check that:
           - length is valid (spec says min 1 max 12 UUIDs),
           - pointer is valid,
           - pattern is a data element sequence */
        if (sch_size < 2 + 3 /* DES8 + UUID16 */ ||
            sch_size > 5 + 12 * 17 /* DES32 * 12*UUID128 */ ||
            search_pttrn == NULL ||
            DATA_ELEMENT_SEQUENCE !=
                (search_pttrn[0] & DATA_ELEMENT_TYPE_MASK))
        {
            return FALSE;
        }
    }

    /* Validate the AttributeIDList, if there is one */
    if (srch_type != SDP_SERVICE_SEARCH_REQUEST)
    {
        /* Check that:
           - MaximumAttributeByteCount is valid (spec says min 7)
           - length is valid (spec implies min 1 CsrUint16 but no max),
           - pointer is valid,
           - list is a data element sequence */
        if (response_max < 7 ||
            att_size < 2 + 3 /* DES8 + CsrUint16 */ ||
            attrib_list == NULL ||
            DATA_ELEMENT_SEQUENCE !=
                (attrib_list[0] & DATA_ELEMENT_TYPE_MASK))
        {
            return FALSE;
        }
    }
    else
    {
        /* Check that:
           - MaximumServiceRecordCount is valid (spec says min 1) */
        if (response_max < 1)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_set_return_max(CsrUint8 offset)
 *
 *  DESCRIPTION
 *  Sets the return maximum for the attribute search's
 *  1. Tests the response maximum required by the upper layer against the
 *     local mtu and the remote mtu and sets the return maximum used in the
 *     attribute search's to the lowest.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_set_return_max(CsrUint8 offset)
{
    CsrUint16 max;

    max = (sdccb.mtu_tx > sdccb.mtu_rx ? sdccb.mtu_rx : sdccb.mtu_tx) - offset;

    dehead.return_max = dehead.response_max > max ? max : dehead.response_max;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_set_search_data(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Store the service search data items for the search
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_set_search_data(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_SEARCH_REQ_T *prim = (SDC_SERVICE_SEARCH_REQ_T *)sdc_prim;

    /* store the service search size */
    dehead.sch_size = prim->size_srch_pttrn;
    /* store the service search pointer */
    dehead.search_pttrn = prim->srch_pttrn;
    /* store the service search bd address */
    sdccb.bd_addr = prim->bd_addr;
    /* store the service search p handle */
    sdccb.sda_phandle_current = prim->phandle;
    /* store the service search maximum number of handles to be returned */
    dehead.response_max = prim->max_num_recs * PDU_HANDLE_SIZE;
    /* store the service search type state */
    dehead.srch_type = SDP_SERVICE_SEARCH_REQUEST;
    /* store the pdu offset, which plus the sch_size will be the search size */
    dehead.pdu_offset = PDU_HEAD_SIZE + PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_set_attr_data(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Store the service attribute data items for the search
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_set_attr_data(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_ATTRIBUTE_REQ_T *prim =
        (SDC_SERVICE_ATTRIBUTE_REQ_T *)sdc_prim;

    /* store the service attribute size */
    dehead.att_size = prim->size_attr_list;
    /* store the service attribute pointer */
    dehead.attrib_list = prim->attr_list;
    /* store the service attribute bd address */
    sdccb.bd_addr = prim->bd_addr;
    /* store the service attribute p handle */
    sdccb.sda_phandle_current = prim->phandle;
    /* store the service attribute service record handle */
    dehead.svc_rec_hndle = prim->svc_rec_hndl;
    /* store the service attribute maximum number of bytes to be returned */
    dehead.response_max = prim->max_num_attr;
    /* store the service attribute type state */
    dehead.srch_type = SDP_SERVICE_ATTRIBUTE_REQUEST;
    /* store the pdu offset, which plus the att_size will be the search size */
    dehead.pdu_offset = PDU_HEAD_SIZE + PDU_ATTRIB_HANDLE_SIZE +
        PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_set_search_attr_data(SDC_UPRIM_T *sdc_prim)
 *
 *  DESCRIPTION
 *  Store the service search attribute data items for the search
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_set_search_attr_data(SDC_UPRIM_T *sdc_prim)
{
    SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *prim =
        (SDC_SERVICE_SEARCH_ATTRIBUTE_REQ_T *)sdc_prim;

    /* store the attribute size */
    dehead.att_size = prim->size_attr_list;
    /* store the attribute pointer */
    dehead.attrib_list = prim->attr_list;
    /* store the search size */
    dehead.sch_size = prim->size_srch_pttrn;
    /* store the search pointer */
    dehead.search_pttrn = prim->srch_pttrn;
    /* store the service search attribute bd address */
    sdccb.bd_addr = prim->bd_addr;
    /* store the service search attribute p handle */
    sdccb.sda_phandle_current = prim->phandle;
    /* store the service attribute maximum number of bytes to be returned */
    dehead.response_max = prim->max_num_attr;
    /* store the service search attribute type state */
    dehead.srch_type = SDP_SERVICE_SEARCH_ATTRIBUTE_REQUEST;
    /* store the pdu offset, which plus the sch_size plus the att_size  will */
    /* be the search size */
    dehead.pdu_offset = PDU_HEAD_SIZE + PDU_MAX_COUNT_SIZE + CONTINUATION_SIZE;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_search_timer_alarm(CsrUint16 notused_uint16, void * notused_pointer)
 *
 *  DESCRIPTION
 *  This function is called if the SEARCH_TIMER expires. The timer has expired
 *  we have not reserved a response from SDS, we conclude an error has occurred
 *  in SDS, so we need to tell the upper layer that we have received no data
 *  and the timer has expired.  We also need to identify the potential response
 *  from further search responses.
 *  1. Test to see if the search has not completed.
 *  2. If not increment the transaction id, so we can identify the potential
 *     response if it occurs.
 *  3. Set the appropriate error response and response to the upper layer.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
void SDC_search_timer_alarm(CsrUint16 notused_uint16, void * notused_pointer)
{
    PARAM_UNUSED(notused_uint16);
    PARAM_UNUSED(notused_pointer);

    /* Expire the timer id */
    TIMER_EXPIRED(dehead.evenCsrSchedTid);

    /* test to see if the search has finnished and we are in the */
    /* communicating state */
    if(0 != dehead.srch_type && sdccb.state == SDC_ST_COMMUNICATING)
    {
        /* the search has not finished, so we need to identify the PDU */
        /* response if it does occur and send a timeout error response to */
        /* the upper layer to indicate we have no response data */

        /* increment the transaction id, to make sure we can identify the */
        /* expected response and ignore it, if it occurs later on */
        sdccb.transaction++;
        /* set error response codes */
        dehead.response_response = SDC_RESPONSE_TIMEOUT_ERROR;
        dehead.response_error = 0;
        dehead.finished = TRUE;

        /* The search has failed, so decide how to finish and send the final
        * response to the application.
        */
        if (sdccb.sdc_open_search)
        {
            /* Connection was opened with the open search command, so
            * keep the connection open.
            */
            fsm_16bit_run(&sdc_fsm, &sdccb.state,
                SDC_EV_SEARCH_COMPLETE_OPEN, 0, 0);
        }
        else
        {
            /* Connection was opened with the search request command, so
            * now disconnect.
            */
            fsm_16bit_run(&sdc_fsm, &sdccb.state,
                SDC_EV_SEARCH_COMPLETE, 0, 0);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *  CsrBool SDC_test_search_size(CsrUint16 size, CsrUint8 offset)
 *
 *  DESCRIPTION
 *  Test the size of the search pattern against the MTU size
 *  1. Test the search size plus the PDU offset against the local MTU L2CAP
 *     packet size. To make sure the search will fit.
 *  2. Test the search size plus the PDU offset against the remote MTU L2CAP
 *     packet size. To make sure the search will fit.
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
CsrBool SDC_test_search_size(CsrUint16 size, CsrUint8 offset)
{
    CsrUint16 length = size + offset;

    return length <= sdccb.mtu_rx && length <= sdccb.mtu_tx;
}





/*----------------------------------------------------------------------------*
 *  NAME
 *  void SDC_datawrite_req(CsrUint16 pdu_size, void *pdu_ptr)
 *
 *  DESCRIPTION
 *        Checks to see that there is no pending datawrite confirm before sending
 *        data, otherwise waits
 *
 *  RETURNS/MODIFIES
 *  None
 *
 *---------------------------------------------------------------------------*/
static void SDC_datawrite_req(CsrUint16 pdu_size, void *pdu_ptr)
{
    /* store the data */
    sdccb.pdu_size = pdu_size;
    sdccb.pdu_ptr = pdu_ptr;

    if (sdccb.is_waiting_for_l2c_datawrite_confirm)
    {
        /*  Don't send */
        /* EMPTY */
    }
    else
    {
        L2CA_DataWriteReq(sdccb.cid, sdccb.pdu_size, sdccb.pdu_ptr);

        /* set the search event timer just after we send the search request */
        /* to SDS. Storing the timer event id, so we can clear the timer if */
        /* the search response is received  before the timer expires. */
        timer_start(&dehead.evenCsrSchedTid,
                    SEARCH_TIMER,
                    SDC_search_timer_alarm,
                    0,
                    NULL);

        sdccb.pdu_ptr = NULL;

        sdccb.is_waiting_for_l2c_datawrite_confirm = TRUE;
    }
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      data_element_size
 *
 *  DESCRIPTION
 *      Calculates the size of data referenced by a Data Element Size
 *      descriptor.
 *
 *  RETURNS
 *      CsrUint32 - Size of data following the DES
 *
 *----------------------------------------------------------------------------*/

static CsrUint32 data_element_size(
    CsrUint8 *de_ptr
    )
{
    CsrUint32 de_size = 0;  /* date element size */

    /* find the data element size */
    switch(de_ptr[0] & DATA_ELEMENT_SIZE_MASK)
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
        de_size = SDP_UINT8_R(de_ptr, 1);
        break;
    case SIZE_INDEX_16ABITS:
        de_size = SDP_UINT16_R(de_ptr, 1);
        break;
    case SIZE_INDEX_32ABITS:
        de_size = SDP_UINT32_R(de_ptr, 1);
        break;
    default:
        /* error not UUID default index */
        de_size = 0;
        break;
    };

    return de_size;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      des_length
 *
 *  DESCRIPTION
 *      Calculates the length of the Data Element Size descriptor, ie the
 *      size of the DES itself rather than the following data.
 *
 *  RETURNS
 *      CsrUint8 - DES size
 *
 *----------------------------------------------------------------------------*/

static CsrUint8 des_length(
    CsrUint8 *de_ptr
    )
{
    CsrUint8 de_length = 0;
    CsrUint8 des_type;

    des_type = de_ptr[0] & DATA_ELEMENT_SIZE_MASK;
    if (des_type <= SIZE_INDEX_32ABITS)
    {
        de_length = des_field_sizes[des_type];
    }

    return de_length;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      get_record_size
 *
 *  DESCRIPTION
 *      Analyses the byte stream in MBLK to identify a complete DES descriptor.
 *      If one is found, returns the record length contained in the descriptor,
 *      and the size of the descriptor itself in bytes.
 *
 *  RETURNS
 *      CsrBool - TRUE if a complete and valid DES descriptor was found
 *
 *----------------------------------------------------------------------------*/

static CsrBool get_record_size(
    CsrMblk *p_mblk,
    CsrUint32 *p_record_size,
    CsrUint16 *p_des_size
    )
{
    CsrUint8 des_type;
    CsrUint8 i;
    CsrUint8 count;
    CsrUint32 record_size;
    CsrUint16 des_size;
    CsrUint8 size_bytes[4];

    /* Get the DES field type first */
    if (CsrMblkCopyToMemory(p_mblk, 0, 1, &des_type) != 1)
        return FALSE;

    des_type &= DATA_ELEMENT_SIZE_MASK;
    if (des_type > SIZE_INDEX_32ABITS)
        return FALSE;

    /* Get the size of the DES descriptor field, and the size of the
     * described record - for some descriptor types this is zero and the
     * actual record size is in the following bytes.
     */
    des_size = des_field_sizes[des_type];
    record_size = des_record_sizes[des_type];

    /* Read any additional bytes required */
    if ((count = des_size - 1) != 0)
    {
        if (CsrMblkCopyToMemory(p_mblk, 1, count, size_bytes) != count)
            return FALSE;

        /* Got all the bytes, now shift them into place */
        for (i = 0; i < count; i++)
            record_size = (record_size << 8) | size_bytes[i];
    }

    /* Return the sizes */
    *p_record_size = record_size;
    *p_des_size = des_size;

    return TRUE;
}

static void l2cap_autoconnect_req(void)
{
    /* send connection request */
    L2CA_AutoConnectReq(L2CA_CID_INVALID,
                        SDP_PSM,
                        &sdccb.bd_addr,
                        SDP_PSM,
                        0,
                        L2CA_AMP_CONTROLLER_BT,
                        L2CA_AMP_CONTROLLER_BT,
                        SDP_L2CAP_CONFTAB_SIZE,
                        sdp_control_conftab(sdccb.mtu_rx));
}

#endif /* SDC_INCLUDE */

/*============================================================================*
End Of File
*============================================================================*/



