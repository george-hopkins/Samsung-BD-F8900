#ifndef _SDP_TS_INTERFACE_H_
#define _SDP_TS_INTERFACE_H_

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_ts_interface.h

DESCRIPTION:       This file contains SDP TS interface declarations.



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
 *  void sdp_ts_handler(SDS_UPRIM_T *sds_prim)
 *
 *  DESCRIPTION
 *  SDP interface handler for ts primitives, handles the primitives from the
 *  upper layer to the SDS server, initially described as the Task
 *  Supervisor.
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/

void sdp_ts_handler(SDS_UPRIM_T *sds_prim);

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDS_Register_Cfm
 *
 *  DESCRIPTION
 *  Send register_cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/

#ifdef SDS_INCLUDE
void Send_SDS_Register_Cfm(CsrUint32 svc_rec_hndl, CsrUint16 result,
                           CsrSchedQid phandle);
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *  void Send_SDS_UnRegister_Cfm
 *
 *  DESCRIPTION
 *  Send unregister_cfm primitives upwards
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/

#ifdef SDS_INCLUDE
void Send_SDS_UnRegister_Cfm(CsrUint32 svc_rec_hndl, CsrUint16 result,
                             CsrSchedQid phandle);
#endif

#ifdef __cplusplus
}
#endif

#endif

