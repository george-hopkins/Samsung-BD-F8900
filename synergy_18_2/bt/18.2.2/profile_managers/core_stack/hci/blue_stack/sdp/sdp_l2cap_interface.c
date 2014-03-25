/****************************************************************************

                   (c) CSR plc 2010

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_l2cap_interface.c

DESCRIPTION:       This file contains the SDP L2CAP interface functions.



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
#endif

#ifdef INSTALL_SDP_MODULE

/*============================================================================*
Interface Header Files
*============================================================================*/
#include "l2cap_prim.h" /* path modified */
#include "l2caplib.h" /* path removed */
#include "l2cap_config.h"
#include "sdc_prim.h" /* path modified */
#include "sds_prim.h" /* path modified */
#include "sdp_control.h" /* path removed */
#include "sdp_sdc.h" /* path removed */
#include "sdp_sds.h" /* path removed */


/*============================================================================*
Local Header File
*============================================================================*/
#include "sdp_l2cap_interface.h" /* path removed */
/*============================================================================*
Public Data
*============================================================================*/
/* None */

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
 *  init_sdp
 *
 *  DESCRIPTION
 *  See header file
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/
void init_sdp(void **gash)
{
    PARAM_UNUSED(gash);

    if (!HciClientEnabled())
        return;

    /* Initialising SDS and SDC */
#ifdef SDC_INCLUDE
    init_SDC();
#endif
#ifdef SDS_INCLUDE
    init_SDS();
#endif

    L2CA_RegisterReq(SDP_PSM,
                     SDP_L2CAP_IFACEQUEUE,
                     L2CA_MODE_MASK_BASIC,
                     0,
                     0);
}



/*----------------------------------------------------------------------------*
 *  NAME
 *  void sdp_l2cap_handler()
 *
 *  DESCRIPTION
 *  SDP interface handler for L2CAP primitives
 *
 *  RETURNS/MODIFIES
 *
 *
 *---------------------------------------------------------------------------*/

void sdp_l2cap_handler(void **gash)
{
    CsrUint16 m;
    L2CA_UPRIM_T *sdp_l2cap_prim;
    PARAM_UNUSED(gash);

    (void) CsrSchedMessageGet(&m , (void **)&sdp_l2cap_prim);
#ifdef BUILD_FOR_HOST
    if(m==CSR_SCHED_PRIM)
    {
        CsrPmemFree(sdp_l2cap_prim);
        return;
    }
#endif

    switch(sdp_l2cap_prim->type)
    {
        case L2CA_AUTO_CONNECT_CFM:
            {
#ifdef SDS_INCLUDE
                l2ca_cid_t cid = sdp_l2cap_prim->l2ca_auto_connect_cfm.cid;

                if (sds_cid_match(cid))
                {
                    SDS_Auto_Connect_Confirm(sdp_l2cap_prim);
                    break;
                }
#endif
#ifdef SDC_INCLUDE
                /* Always send to SDC because it might not yet know the CID */
                SDC_Auto_Connect_Confirm(sdp_l2cap_prim);
#endif
            }
            break;

#ifdef SDS_INCLUDE
        case L2CA_AUTO_CONNECT_IND:
            /* only send the connection indication to SDS because only */
            /* SDC can initiate a connection */
            SDS_Auto_Connect_Indication(sdp_l2cap_prim);
#endif
            break;

        case L2CA_DISCONNECT_CFM:
#ifdef SDC_INCLUDE
            /* test CID match with SDC, is there a connection */
            if (sdc_cid_match(sdp_l2cap_prim->l2ca_disconnect_cfm.cid))
            {
                /* we have a connection and the CID matches the SDC CID, */
                /* so send the disconnect cfm to SDC */
                SDC_Disconnect_Confirm(sdp_l2cap_prim);
                break;
            }
#endif
#ifdef SDS_INCLUDE
            /* test CID match with SDS, is there a connection */
            if (sds_cid_match(sdp_l2cap_prim->l2ca_disconnect_cfm.cid))
            {
                /* we have a connection and the CID matches the SDS CID, */
                /* so send the disconnect cfm to SDS */
                SDS_Disconnect_Confirm(sdp_l2cap_prim);
                break;
            }
#endif
            /* Behaviour assumes L2CAP requires no response for unmatched CID */
            /* See B-16987 */
            break;

        case L2CA_DATAWRITE_CFM:
#ifdef SDC_INCLUDE
            /* test CID match with SDC, is there a connection */
            if (sdc_cid_match(sdp_l2cap_prim->l2ca_datawrite_cfm.cid))
            {
                /* we have a connection and the CID matches the SDC CID, */
                /* so send the datawrite cfm to SDC */
                SDC_Datawrite_Confirm(sdp_l2cap_prim);
                break;
            }
#endif
#ifdef SDS_INCLUDE
            /* test CID match with SDS, is there a connection */
            if (sds_cid_match(sdp_l2cap_prim->l2ca_datawrite_cfm.cid))
            {
                /* we have a connection and the CID matches the SDS CID, */
                /* so send the datawrite cfm to SDS */
                SDS_Datawrite_Confirm(sdp_l2cap_prim);
                break;
            }
#endif
            /* Behaviour assumes L2CAP requires no response for unmatched CID */
            /* See B-16987 */
            break;

        case L2CA_DISCONNECT_IND:
#ifdef SDC_INCLUDE
            /* test CID match with SDC, is there a connection */
            if (sdc_cid_match(sdp_l2cap_prim->l2ca_disconnect_ind.cid))
            {
                /* we have a connection and the CID matches the SDC CID, */
                /* so send the disconnect ind to SDC */
                SDC_Disconnect_Indication(sdp_l2cap_prim);
                break;
            }
#endif
#ifdef SDS_INCLUDE
            /* test CID match with SDS, is there a connection */
            if (sds_cid_match(sdp_l2cap_prim->l2ca_disconnect_ind.cid))
            {
                /* we have a connection and the CID matches the SDS CID, */
                /* so send the disconnect ind to SDS */
                SDS_Disconnect_Indication(sdp_l2cap_prim);
                break;
            }
#endif
            /* there is no connection as we have no CID */
            /* we need to respond to L2CAP so */
            /* send disconnect response to L2CAP */
            L2CA_DisconnectRsp(sdp_l2cap_prim->l2ca_disconnect_ind.identifier,
                               sdp_l2cap_prim->l2ca_disconnect_ind.cid);
            break;

        case L2CA_DATAREAD_IND:
#ifdef SDC_INCLUDE
            /* test CID match with SDC, is there a connection */
            if (sdc_cid_match(sdp_l2cap_prim->l2ca_dataread_ind.cid))
            {
                /* we have a connection and the CID matches the SDC CID, */
                /* so send the datawrite ind to SDC */
                SDC_Dataread_Indication(sdp_l2cap_prim);
                break;
            }
#endif
#ifdef SDS_INCLUDE
            /* test CID match with SDS, is there a connection */
            if (sds_cid_match(sdp_l2cap_prim->l2ca_dataread_ind.cid))
            {
                /* we have a connection and the CID matches the SDS CID, */
                /* so send the datawrite ind to SDS */
                SDS_Dataread_Indication(sdp_l2cap_prim);
                break;
            }
#endif
            break;

        default:
            /* Unhandled L2CAP primitive */
            /* Currently we just free the primitive; however, if primitive points
               to data then we get a memory leak */
            /* See B-16987 */
            break;
    }

    /* free the event primitive */
    L2CA_FreePrimitive(sdp_l2cap_prim);
}

/*============================================================================*
Private Function Implementations
*============================================================================*/
/* Declared 'Static' */
/* None */

#endif /* INSTALL_SDP_MODULE */

/*============================================================================*
End Of File
*============================================================================*/


