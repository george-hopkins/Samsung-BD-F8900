/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sda_interface.c

DESCRIPTION:       This file contains the SDP SDA interface functions.



****************************************************************************/

#include "csr_synergy.h"

/*============================================================================*
ANSI C & System-wide Header Files
*============================================================================*/
#include "csr_bt_bluestack_types.h" /* path modified */
#include "bluetooth.h" /* path modified */
#include "csr_bt_common.h" /* path removed */
#include "error.h" /* path removed */
#include "csr_mblk.h" /* path removed */
#include "csr_bt_core_stack_fsm.h" /* path removed */
#include "csr_bt_tasks.h" /* path removed */
#include "csr_bt_core_stack_pmalloc.h" /* path removed */
#ifdef BUILD_FOR_HOST
#include "csr_env_prim.h"
#ifndef HIDE_EXTENDED_BLUESTACK_PRIMS
#define HIDE_EXTENDED_BLUESTACK_PRIMS /* Once is enough */
#endif
#endif

#ifdef INSTALL_SDP_MODULE

/*============================================================================*
Interface Header Files
*============================================================================*/
#include "l2cap_prim.h" /* path modified */
#include "sdp_control.h" /* path removed */
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */
#include "sdp_sdc.h" /* path removed */
#include "sdp_sds.h" /* path removed */
#include "sdp_ts_interface.h" /* path removed */


/*============================================================================*
Local Header File
*============================================================================*/
#include "sdp_sda_interface.h" /* path removed */

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Private Defines
*============================================================================*/
/* On-chip SDP posts upstream messages to hostio */
#ifndef BUILD_FOR_HOST
#define PUT_MESSAGE(q, m, prim) \
        if (q & (CsrSchedQid)0x8000) \
            CsrSchedMessagePut(SDD_TOHOSTQUEUE, m, prim); \
        else \
            CsrSchedMessagePut(q, m, prim)
#else
#ifdef BAS_HOST_QUEUEIDS
/* Top 3 bits allow different phandles for reply queue */
#define PUT_MESSAGE(q, m, prim) CsrSchedMessagePut(0x1fff & q, m, prim)
#else
#define PUT_MESSAGE(q, m, prim) CsrSchedMessagePut(q, m, prim)
#endif
#endif

/*============================================================================*
Private Data Types
*============================================================================*/
/* None */

/*============================================================================*
Private Function Prototypes
*============================================================================*/
/* Declared 'Static' */
/* None */

/*============================================================================*
Private Data
*============================================================================*/
/* Declared 'Static' */
/* None */

/*============================================================================*
Public Function Implementations
*============================================================================*/


/*----------------------------------------------------------------------------*
 *  NAME
 *  void sdp_sda_handler()
 *
 *  DESCRIPTION
 *  SDP interface handler for SDA primitives
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/

void sdp_sda_handler(void **gash)
{
    CsrUint16 m;
    SDC_UPRIM_T *sdc_prim;
    SDS_UPRIM_T *sds_prim;
    PARAM_UNUSED(gash);

    (void) CsrSchedMessageGet(&m , (void **)&sdc_prim);
#ifdef BUILD_FOR_HOST
    if (m == CSR_SCHED_PRIM)
    {
        CsrPmemFree(sdc_prim);
        return;
    }
#endif

    switch (sdc_prim->type)
    {
#ifdef SDC_INCLUDE
    case SDC_SERVICE_SEARCH_REQ:
            SDC_Search_Request(sdc_prim);
        break;

    case SDC_SERVICE_ATTRIBUTE_REQ:
            SDC_Attribute_Request(sdc_prim);
        break;

    case SDC_SERVICE_SEARCH_ATTRIBUTE_REQ:
        SDC_Search_Attribute_Request(sdc_prim);
        break;

    case SDC_TERMINATE_PRIMITIVE_REQ:
        SDC_Terminate_Primative_Request(sdc_prim);
        break;

    case SDC_OPEN_SEARCH_REQ:
        SDC_Open_Search_Request(sdc_prim);
        break;

    case SDC_CLOSE_SEARCH_REQ:
        SDC_Close_Search_Request(sdc_prim);
        break;

    case SDC_CONFIG_REQ:
        SDC_Config_Request(sdc_prim);
        break;
#endif
    default:
        sds_prim = (SDS_UPRIM_T *)sdc_prim;
        sdp_ts_handler(sds_prim);
        break;
    }

    /* free the event primitive */
    CsrPmemFree(sdc_prim);

}


/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_Search_Cfm
 *
 *  DESCRIPTION
 *  Send search_cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_Search_Cfm(CsrSchedQid phandle, CsrUint16 size,
                         CsrUint16 num_recs_ret, CsrUint8 *rec_list,
                         CsrUint16 response, CsrUint16 err_code, BD_ADDR_T* bd_addr)
{
    CsrUint16 m = SDP_PRIM;

    SDC_SERVICE_SEARCH_CFM_T *sdc_prim = zpnew(SDC_SERVICE_SEARCH_CFM_T);

    sdc_prim->type = SDC_SERVICE_SEARCH_CFM;
    sdc_prim->num_recs_ret = num_recs_ret;
    sdc_prim->size_rec_list = size;
    sdc_prim->rec_list = rec_list;
    sdc_prim->response = response;
    sdc_prim->err_code = err_code;
    sdc_prim->size_err_info = 0; /* Not used in current spec. */
    sdc_prim->err_info = NULL;   /* Not used in current spec. */
    sdc_prim->phandle = phandle;
    if(bd_addr)
        sdc_prim->bd_addr = *bd_addr;
    else
        bd_addr_zero(&sdc_prim->bd_addr);

    PUT_MESSAGE(phandle, m,    sdc_prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_Attribute_Cfm
 *
 *  DESCRIPTION
 *  Send attribute_cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_Attribute_Cfm(
    CsrSchedQid phandle,
    CsrUint16 size_attr_list,
    CsrUint8 *attr_list,
    CsrUint16 response,
    CsrUint16 err_code,
    BD_ADDR_T* bd_addr
    )
{
    CsrUint16  m = SDP_PRIM;

    SDC_SERVICE_ATTRIBUTE_CFM_T *sdc_prim = zpnew(SDC_SERVICE_ATTRIBUTE_CFM_T);

    sdc_prim->type = SDC_SERVICE_ATTRIBUTE_CFM;
    sdc_prim->size_attr_list = size_attr_list;
    sdc_prim->attr_list = attr_list;
    sdc_prim->response = response;
    sdc_prim->err_code = err_code;
    sdc_prim->size_err_info = 0; /* Not used in the current spec version */
    sdc_prim->err_info = NULL;   /* Not used in current spec. */
    sdc_prim->phandle = phandle;
    if(bd_addr)
        sdc_prim->bd_addr = *bd_addr;
    else
        bd_addr_zero(&sdc_prim->bd_addr);

    PUT_MESSAGE(phandle, m,    sdc_prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_SearchAttribute_Cfm
 *
 *  DESCRIPTION
 *  Send searchattribute_cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_SearchAttribute_Cfm(
    CsrSchedQid phandle,
    CsrUint16 total_response_size,
    CsrUint16 size_attr_list,
    CsrUint8 *attr_list,
    CsrBool more_to_come,
    CsrUint16 response,
    CsrUint16 err_code,
    BD_ADDR_T* bd_addr
    )
{
    CsrUint16 m = SDP_PRIM;
    SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T *sdc_prim =
        zpnew(SDC_SERVICE_SEARCH_ATTRIBUTE_CFM_T);

    sdc_prim->type = SDC_SERVICE_SEARCH_ATTRIBUTE_CFM;
    sdc_prim->total_response_size = total_response_size;
    sdc_prim->size_attr_list = size_attr_list;
    sdc_prim->attr_list = attr_list;
    sdc_prim->more_to_come = more_to_come;
    sdc_prim->response = response;
    sdc_prim->err_code = err_code;
    sdc_prim->size_err_info = 0; /* Not used in the current spec version */
    sdc_prim->err_info = NULL;   /* Not used in current spec. */
    sdc_prim->phandle = phandle;
    if(bd_addr)
        sdc_prim->bd_addr = *bd_addr;
    else
        bd_addr_zero(&sdc_prim->bd_addr);

    PUT_MESSAGE(phandle, m, sdc_prim);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_opensearch_Cfm
 *
 *  DESCRIPTION
 *  Send opensearch_Cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_opensearch_Cfm(CsrSchedQid phandle, CsrUint16 result)
{

    SDC_OPEN_SEARCH_CFM_T *sdc_prim;
    CsrUint16 m;

    m = SDP_PRIM;
    sdc_prim =
        (SDC_OPEN_SEARCH_CFM_T *)CsrPmemZalloc(
        sizeof(SDC_OPEN_SEARCH_CFM_T));

    sdc_prim->type = SDC_OPEN_SEARCH_CFM;
    sdc_prim->phandle = phandle;
    sdc_prim->result = result;

    PUT_MESSAGE(phandle, m,    sdc_prim);

}
#endif


/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_CloseSearch_Ind
 *
 *  DESCRIPTION
 *  Send closesearch_Ind primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_CloseSearch_Ind(CsrSchedQid phandle, CsrUint16 result)
{
    SDC_CLOSE_SEARCH_IND_T *sdc_prim;
    CsrUint16 m;

    m = SDP_PRIM;
    sdc_prim =
        (SDC_CLOSE_SEARCH_IND_T *)CsrPmemZalloc(
        sizeof(SDC_CLOSE_SEARCH_IND_T));

    sdc_prim->type = SDC_CLOSE_SEARCH_IND;
    sdc_prim->phandle = phandle;
    sdc_prim->result = result;

    PUT_MESSAGE(phandle, m,    sdc_prim);
}
#endif

#if defined(SDC_INCLUDE) || defined(SDS_INCLUDE)
void Send_SDP_Message(void *message)
{
    PUT_MESSAGE(
        ((SDC_SDS_UPSTREAM_COMMON_T*)message)->phandle,
        SDP_PRIM,
        message);
}
#endif

#endif /* INSTALL_SDP_MODULE */
