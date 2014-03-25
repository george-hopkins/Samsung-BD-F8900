/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_ts_interface.c

DESCRIPTION:       This file contains the SDP TS interface functions.



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

/*============================================================================*
Interface Header Files
*============================================================================*/
#include "l2cap_prim.h" /* path modified */
#include "sdp_control.h" /* path removed */
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */
#include "sdp_sds.h" /* path removed */

/*============================================================================*
Local Header File
*============================================================================*/
#include "sdp_ts_interface.h" /* path removed */
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
#define PUT_MESSAGE(q, m, prim) \
        CsrSchedMessagePut(q, m, prim)
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

void sdp_ts_handler(SDS_UPRIM_T *sds_prim)
{

    switch (sds_prim->type)
    {
#ifdef SDS_INCLUDE
    case SDS_REGISTER_REQ:
        SDS_Register_Request(sds_prim);
        break;

    case SDS_UNREGISTER_REQ:
        SDS_UnRegister_Request(sds_prim);
        break;

    case SDS_CONFIG_REQ:
        SDS_Config_Request(sds_prim);
        break;
#endif
    default:
        break;
    }

}



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
                           CsrSchedQid phandle)
{
    SDS_REGISTER_CFM_T *sdc_prim;
    CsrUint16 m;

    m = SDP_PRIM;
    sdc_prim = (SDS_REGISTER_CFM_T *)CsrPmemZalloc(sizeof(SDS_REGISTER_CFM_T));

    sdc_prim->type = SDS_REGISTER_CFM;
    sdc_prim->phandle = phandle;
    sdc_prim->svc_rec_hndl = svc_rec_hndl;
    sdc_prim->result = result;

    PUT_MESSAGE(phandle, m,    sdc_prim);
}
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
                             CsrSchedQid phandle)
{

    SDS_UNREGISTER_CFM_T *sdc_prim;
    CsrUint16 m;

    m = SDP_PRIM;
    sdc_prim =
        (SDS_UNREGISTER_CFM_T *)CsrPmemZalloc(sizeof(SDS_UNREGISTER_CFM_T));

    sdc_prim->type = SDS_UNREGISTER_CFM;
    sdc_prim->phandle = phandle;
    sdc_prim->svc_rec_hndl = svc_rec_hndl;
    sdc_prim->result = result;

    PUT_MESSAGE(phandle, m,    sdc_prim);

}
#endif

/*============================================================================*
Private Function Implementations
*============================================================================*/
/* Declared 'Static' */
/* None */

/*============================================================================*
End Of File
*============================================================================*/


