#ifndef _SDP_SDA_INTERFACE_H_
#define _SDP_SDA_INTERFACE_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sda_interface.h

DESCRIPTION:       This file contains SDP SDA interface declarations.



****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
Public Defines
*============================================================================*/
/* None */

/*============================================================================*
Public Data Types
*============================================================================*/
/* None */

/*============================================================================*
Public Data
*============================================================================*/
/* None */

/*============================================================================*
Public Functions
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

#ifdef SDC_INCLUDE
void sdp_sda_handler(void **gash);
#endif

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
                         CsrUint16 response, CsrUint16 err_code, BD_ADDR_T* bd_addr);
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
    );
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_SearchAttribute_Cfm
 *
 *  DESCRIPTION
 *  Send searchatribute_cfm primitives upwards
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
    BD_ADDR_T* bdaddr
    );
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
void Send_SDC_opensearch_Cfm(CsrSchedQid phandle, CsrUint16 result);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDC_closesearch_Ind
 *
 *  DESCRIPTION
 *  Send closesearch_Ind primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
#ifdef SDC_INCLUDE
void Send_SDC_CloseSearch_Ind(CsrSchedQid phandle, CsrUint16 result);
#endif

#if defined(SDC_INCLUDE) || defined(SDS_INCLUDE)
void Send_SDP_Message(void *message);
#endif

#ifdef __cplusplus
}
#endif

#endif

