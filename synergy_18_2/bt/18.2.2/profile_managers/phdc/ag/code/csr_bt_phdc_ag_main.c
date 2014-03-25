/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

#include "csr_bt_tasks.h"

#include "csr_bt_phdc_ag_main.h"
#include "csr_bt_phdc_ag_common.h"
#include "csr_bt_phdc_ag_fsm.h"
#include "csr_bt_phdc_ag_out.h"
#include "csr_bt_phdc_ag_prim.h"
#include "csr_bt_phdc_common.h"

static void CsrBtPhdcAgentFillStdConfigWsPrstApdu(CsrBtPhdcAgentInstance *pInst)
{
/* The indexes uncommented are already set to 0 by CsrPmemZalloc() */
    pInst->stdConfigPrstApduWs[0] = 0xE7;
/*    pInst->stdConfigPrstApduWs[1] = 0x00; */
/*    pInst->stdConfigPrstApduWs[2] = 0x00; */
    pInst->stdConfigPrstApduWs[3] = 0x44 ;
/*    pInst->stdConfigPrstApduWs[4] = 0x00; */
    pInst->stdConfigPrstApduWs[5] = 0x42;
/*    pInst->stdConfigPrstApduWs[6] = 0x00; */ /* Invoke id to be updated*/
/*    pInst->stdConfigPrstApduWs[7] = 0x00; */ /* Invoke id to be updated*/
    pInst->stdConfigPrstApduWs[8] = 0x01;
    pInst->stdConfigPrstApduWs[9] = 0x01 ;
/*    pInst->stdConfigPrstApduWs[10] = 0x00; */
    pInst->stdConfigPrstApduWs[11] = 0x3C ;
/*    pInst->stdConfigPrstApduWs[12] = 0x00 ; */
/*    pInst->stdConfigPrstApduWs[13] = 0x00 ; */
    pInst->stdConfigPrstApduWs[14] = 0xFF;
    pInst->stdConfigPrstApduWs[15] = 0xFF;
    pInst->stdConfigPrstApduWs[16] = 0xFF;
    pInst->stdConfigPrstApduWs[17] = 0xFF ;
    pInst->stdConfigPrstApduWs[18] = 0x0D;
    pInst->stdConfigPrstApduWs[19] = 0x1C ;
/*    pInst->stdConfigPrstApduWs[20] = 0x00; */
    pInst->stdConfigPrstApduWs[21] = 0x32 ;
    pInst->stdConfigPrstApduWs[22] = 0x05;
    pInst->stdConfigPrstApduWs[23] = 0xDC ;
/*    pInst->stdConfigPrstApduWs[24] = 0x00; */
    pInst->stdConfigPrstApduWs[25] = 0x01 ;
/*    pInst->stdConfigPrstApduWs[26] = 0x00; */
    pInst->stdConfigPrstApduWs[27] = 0x2C ;
/*    pInst->stdConfigPrstApduWs[28] = 0x00; */
    pInst->stdConfigPrstApduWs[29] = 0x06 ;
/*    pInst->stdConfigPrstApduWs[30] = 0x00; */
    pInst->stdConfigPrstApduWs[31] = 0x01 ;
/*    pInst->stdConfigPrstApduWs[32] = 0x00; */
    pInst->stdConfigPrstApduWs[33] = 0x04 ;
/*    pInst->stdConfigPrstApduWs[34] = 0x00; */
    pInst->stdConfigPrstApduWs[35] = 0x24 ;
    pInst->stdConfigPrstApduWs[36] = 0x09;
    pInst->stdConfigPrstApduWs[37] = 0x2F ;
/*    pInst->stdConfigPrstApduWs[38] = 0x00; */
    pInst->stdConfigPrstApduWs[39] = 0x04 ;
/*    pInst->stdConfigPrstApduWs[40] = 0x00; */
    pInst->stdConfigPrstApduWs[41] = 0x02;
    pInst->stdConfigPrstApduWs[42] = 0xE1;
    pInst->stdConfigPrstApduWs[43] = 0x40 ;
    pInst->stdConfigPrstApduWs[44] = 0x0A;
    pInst->stdConfigPrstApduWs[45] = 0x46 ;
/*    pInst->stdConfigPrstApduWs[46] = 0x00; */
    pInst->stdConfigPrstApduWs[47] = 0x02 ;
    pInst->stdConfigPrstApduWs[48] = 0xF0;
    pInst->stdConfigPrstApduWs[49] = 0x40 ;
    pInst->stdConfigPrstApduWs[50] = 0x09;
    pInst->stdConfigPrstApduWs[51] = 0x96 ;
/*    pInst->stdConfigPrstApduWs[52] = 0x00; */
    pInst->stdConfigPrstApduWs[53] = 0x02 ;
    pInst->stdConfigPrstApduWs[54] = 0x06;
    pInst->stdConfigPrstApduWs[55] = 0xC3 ;
    pInst->stdConfigPrstApduWs[56] = 0x0A;
    pInst->stdConfigPrstApduWs[57] = 0x55 ;
/*    pInst->stdConfigPrstApduWs[58] = 0x00; */
    pInst->stdConfigPrstApduWs[59] = 0x0C ;
/*    pInst->stdConfigPrstApduWs[60] = 0x00; */
    pInst->stdConfigPrstApduWs[61] = 0x02 ;
/*    pInst->stdConfigPrstApduWs[62] = 0x00; */
    pInst->stdConfigPrstApduWs[63] = 0x08 ;
    pInst->stdConfigPrstApduWs[64] = 0x0A;
    pInst->stdConfigPrstApduWs[65] = 0x56;
/*    pInst->stdConfigPrstApduWs[66] = 0x00; */
    pInst->stdConfigPrstApduWs[67] = 0x04 ;
    pInst->stdConfigPrstApduWs[68] = 0x09;
    pInst->stdConfigPrstApduWs[69] = 0x90;
/*    pInst->stdConfigPrstApduWs[70] = 0x00; */
    pInst->stdConfigPrstApduWs[71] = 0x08;
}

static void CsrBtPhdcAgentFillStdConfigBpPrstApdu(CsrBtPhdcAgentInstance *pInst)
{
/* The indexes uncommented are already set to 0 by CsrPmemZalloc() */
    pInst->stdConfigPrstApduBp[0] = 0xE7;
/*    pInst->stdConfigPrstApduBp[1] = 0x00; */
/*    pInst->stdConfigPrstApduBp[2] = 0x00; */
    pInst->stdConfigPrstApduBp[3] = 0x84 ;
/*    pInst->stdConfigPrstApduBp[4] = 0x00; */
    pInst->stdConfigPrstApduBp[5] = 0x82;
/*    pInst->stdConfigPrstApduBp[6] = 0x00; */ /* Invoke id to be updated*/
/*    pInst->stdConfigPrstApduBp[7] = 0x00; */ /* Invoke id to be updated*/
    pInst->stdConfigPrstApduBp[8] = 0x01;
    pInst->stdConfigPrstApduBp[9] = 0x01 ;
/*    pInst->stdConfigPrstApduBp[10] = 0x00; */
    pInst->stdConfigPrstApduBp[11] = 0x7C ;
/*    pInst->stdConfigPrstApduBp[12] = 0x00 ; */
/*    pInst->stdConfigPrstApduBp[13] = 0x00 ; */
    pInst->stdConfigPrstApduBp[14] = 0xFF;
    pInst->stdConfigPrstApduBp[15] = 0xFF;
    pInst->stdConfigPrstApduBp[16] = 0xFF;
    pInst->stdConfigPrstApduBp[17] = 0xFF ;
    pInst->stdConfigPrstApduBp[18] = 0x0D;
    pInst->stdConfigPrstApduBp[19] = 0x1C ;
/*    pInst->stdConfigPrstApduBp[20] = 0x00; */
    pInst->stdConfigPrstApduBp[21] = 0x72 ;
    pInst->stdConfigPrstApduBp[22] = 0x02;
    pInst->stdConfigPrstApduBp[23] = 0xBC ;
/*    pInst->stdConfigPrstApduBp[24] = 0x00; */
    pInst->stdConfigPrstApduBp[25] = 0x02 ;
/*    pInst->stdConfigPrstApduBp[26] = 0x00; */
    pInst->stdConfigPrstApduBp[27] = 0x6C ;
/*    pInst->stdConfigPrstApduBp[28] = 0x00; */
    pInst->stdConfigPrstApduBp[29] = 0x06 ;
/*    pInst->stdConfigPrstApduBp[30] = 0x00; */
    pInst->stdConfigPrstApduBp[31] = 0x01 ;
/*    pInst->stdConfigPrstApduBp[32] = 0x00; */
    pInst->stdConfigPrstApduBp[33] = 0x06 ;
/*    pInst->stdConfigPrstApduBp[34] = 0x00; */
    pInst->stdConfigPrstApduBp[35] = 0x38 ;
    pInst->stdConfigPrstApduBp[36] = 0x09;
    pInst->stdConfigPrstApduBp[37] = 0x2F ;
/*    pInst->stdConfigPrstApduBp[38] = 0x00; */
    pInst->stdConfigPrstApduBp[39] = 0x04 ;
/*    pInst->stdConfigPrstApduBp[40] = 0x00; */
    pInst->stdConfigPrstApduBp[41] = 0x02;
    pInst->stdConfigPrstApduBp[42] = 0x4A;
    pInst->stdConfigPrstApduBp[43] = 0x04 ;
    pInst->stdConfigPrstApduBp[44] = 0x0A;
    pInst->stdConfigPrstApduBp[45] = 0x46 ;
/*    pInst->stdConfigPrstApduBp[46] = 0x00; */
    pInst->stdConfigPrstApduBp[47] = 0x02 ;
    pInst->stdConfigPrstApduBp[48] = 0xB0;
    pInst->stdConfigPrstApduBp[49] = 0x40 ;
    pInst->stdConfigPrstApduBp[50] = 0x0A;
    pInst->stdConfigPrstApduBp[51] = 0x73 ;
/*    pInst->stdConfigPrstApduBp[52] = 0x00; */
    pInst->stdConfigPrstApduBp[53] = 0x02 ;
    pInst->stdConfigPrstApduBp[54] = 0x03;
    pInst->stdConfigPrstApduBp[55] = 0x03 ;
    pInst->stdConfigPrstApduBp[56] = 0x0A;
    pInst->stdConfigPrstApduBp[57] = 0x76 ;
/*    pInst->stdConfigPrstApduBp[58] = 0x00; */
    pInst->stdConfigPrstApduBp[59] = 0x0A ;
/*    pInst->stdConfigPrstApduBp[60] = 0x00; */
    pInst->stdConfigPrstApduBp[61] = 0x03 ;
/*    pInst->stdConfigPrstApduBp[62] = 0x00; */
    pInst->stdConfigPrstApduBp[63] = 0x06 ;
    pInst->stdConfigPrstApduBp[64] = 0x4A;
    pInst->stdConfigPrstApduBp[65] = 0x05;
    pInst->stdConfigPrstApduBp[66] = 0x4A;
    pInst->stdConfigPrstApduBp[67] = 0x06 ;
    pInst->stdConfigPrstApduBp[68] = 0x4A;
    pInst->stdConfigPrstApduBp[69] = 0x07;
    pInst->stdConfigPrstApduBp[70] = 0x09;
    pInst->stdConfigPrstApduBp[71] = 0x96; /*MDC_ATTR_UNIT_CODE */
/*    pInst->stdConfigPrstApduBp[72] = 0x00; */
    pInst->stdConfigPrstApduBp[73] = 0x02 ;  
    pInst->stdConfigPrstApduBp[74] = 0x0F;
    pInst->stdConfigPrstApduBp[75] = 0x20 ;

    
    pInst->stdConfigPrstApduBp[76] = 0x0A;
    pInst->stdConfigPrstApduBp[77] = 0x55; 
/*    pInst->stdConfigPrstApduBp[78] = 0x00; */
    pInst->stdConfigPrstApduBp[79] = 0x0C ;
/*    pInst->stdConfigPrstApduBp[80] = 0x00; */
    pInst->stdConfigPrstApduBp[81] = 0x02 ;
/*    pInst->stdConfigPrstApduBp[82] = 0x00; */
    pInst->stdConfigPrstApduBp[83] = 0x08 ;
    pInst->stdConfigPrstApduBp[84] = 0x0A;
    pInst->stdConfigPrstApduBp[85] = 0x75;
/*    pInst->stdConfigPrstApduBp[86] = 0x00; */
    pInst->stdConfigPrstApduBp[87] = 0x0A;
    pInst->stdConfigPrstApduBp[88] = 0x09;
    pInst->stdConfigPrstApduBp[89] = 0x90;
/*    pInst->stdConfigPrstApduBp[90] = 0x00; */
    pInst->stdConfigPrstApduBp[91] = 0x08 ;
/*    pInst->stdConfigPrstApduBp[92] = 0x00; */
    pInst->stdConfigPrstApduBp[93] = 0x06 ;
/*    pInst->stdConfigPrstApduBp[94] = 0x00; */
    pInst->stdConfigPrstApduBp[95] = 0x02 ;
/*    pInst->stdConfigPrstApduBp[96] = 0x00; */
    pInst->stdConfigPrstApduBp[97] = 0x04 ;
/*    pInst->stdConfigPrstApduBp[98] = 0x00; */
    pInst->stdConfigPrstApduBp[99] = 0x24; 
    pInst->stdConfigPrstApduBp[100] = 0x09;
    pInst->stdConfigPrstApduBp[101] = 0x2F ;
/*    pInst->stdConfigPrstApduBp[102] = 0x00; */
    pInst->stdConfigPrstApduBp[103] = 0x04 ;
/*    pInst->stdConfigPrstApduBp[104] = 0x00; */
    pInst->stdConfigPrstApduBp[105] = 0x02;
    pInst->stdConfigPrstApduBp[106] = 0x48;
    pInst->stdConfigPrstApduBp[107] = 0x2A ;
    pInst->stdConfigPrstApduBp[108] = 0x0A;
    pInst->stdConfigPrstApduBp[109] = 0x46 ;
/*    pInst->stdConfigPrstApduBp[110] = 0x00; */
    pInst->stdConfigPrstApduBp[111] = 0x02 ;
    pInst->stdConfigPrstApduBp[112] = 0xF0;
    pInst->stdConfigPrstApduBp[113] = 0x40 ;
    pInst->stdConfigPrstApduBp[114] = 0x09;
    pInst->stdConfigPrstApduBp[115] = 0x96 ;
/*    pInst->stdConfigPrstApduBp[116] = 0x00; */
    pInst->stdConfigPrstApduBp[117] = 0x02 ;
    pInst->stdConfigPrstApduBp[118] = 0x0A;
    pInst->stdConfigPrstApduBp[119] = 0xA0 ;
    pInst->stdConfigPrstApduBp[120] = 0x0A;
    pInst->stdConfigPrstApduBp[121] = 0x55 ;
/*    pInst->stdConfigPrstApduBp[122] = 0x00; */
    pInst->stdConfigPrstApduBp[123] = 0x0C ;
/*    pInst->stdConfigPrstApduBp[124] = 0x00; */
    pInst->stdConfigPrstApduBp[125] = 0x02 ;
/*    pInst->stdConfigPrstApduBp[126] = 0x00; */
    pInst->stdConfigPrstApduBp[127] = 0x08 ;
    pInst->stdConfigPrstApduBp[128] = 0x0A;
    pInst->stdConfigPrstApduBp[129] = 0x4C;
/*    pInst->stdConfigPrstApduBp[130] = 0x00; */
    pInst->stdConfigPrstApduBp[131] = 0x02 ;
    pInst->stdConfigPrstApduBp[132] = 0x09;
    pInst->stdConfigPrstApduBp[133] = 0x90;
/*    pInst->stdConfigPrstApduBp[134] = 0x00; */
    pInst->stdConfigPrstApduBp[135] = 0x08;
}



static fsm_event_t CsrBtPhdcAgentIfHandler(CsrBtPhdcAgentInstance *pInstance, CsrBool *device)
{
    fsm_event_t returnEvent = PHDC_INVALID_EVENT;
    void *prim = pInstance->pRecvMsg;

    if(*(CsrUint16 *)prim == CSR_BT_PHDC_AG_ASSOCIATE_REQ || 
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES || 
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_DISASSOCIATE_REQ ||
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_SUSPEND_REQ || 
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_RESUME_REQ ||
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_DATA_REQ || 
       *(CsrUint16 *)prim == CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ)
    {
        *device = TRUE;
    }
    else
    {
        *device = FALSE;
    }

    switch (*(CsrUint16 *) (prim))
    {
        case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ:
        {
            returnEvent = PHDC_AG_EV_PHDC_CONFIGURE_ENDPOINT_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES:
        {
            returnEvent = PHDC_AG_EV_PHDC_CONFIGURE_ENDPOINT_RES;
            break;
        }
        case CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ:
        {
            returnEvent = PHDC_AG_EV_PHDC_CONFIGURE_MDS_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ:
        {
            returnEvent = PHDC_AG_EV_PHDC_DIM_CONFIGURE_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_DIM_OBJECT_RES:
        {
            returnEvent = PHDC_AG_EV_PHDC_DIM_OBJECT_RES;
            break;
        }
        case CSR_BT_PHDC_AG_DIM_ATTRIB_RES:
        {
            returnEvent = PHDC_AG_EV_PHDC_DIM_ATTRIB_RES;
            break;
        }
        case CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ:
        {
            returnEvent = PHDC_AG_EV_PHDC_GET_DEVICE_CAPAB_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_ASSOCIATE_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_ASSOCIATE_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_ACCEPT_BT_LINK_RES;
            break;
        }
        case CSR_BT_PHDC_AG_DISASSOCIATE_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_DISASSOCIATE_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_SUSPEND_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_SUSPEND_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_RESUME_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_RESUME_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_CHANGE_UNIT_CODE_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_DATA_REQ:
        {
            returnEvent = PHDC_AG_DEVICE_EV_PHDC_DATA_REQ;
            break;
        }
        case CSR_BT_PHDC_AG_UNREGISTER_REQ:
        {
            returnEvent = PHDC_AG_EV_PHDC_UNREGISTER_REQ;
            break;
        }
        default:
            break;
    }

    return returnEvent;
}

static fsm_event_t CsrBtPhdcAgentHdpIfHandler(CsrBtPhdcAgentInstance *pInstance, CsrBool *device)
{
    fsm_event_t returnEvent = PHDC_INVALID_EVENT;
    void *prim = pInstance->pRecvMsg;

    if(*(CsrUint16 *)prim == CSR_BT_HDP_REGISTER_CFM || 
       *(CsrUint16 *)prim == CSR_BT_HDP_REGISTER_IND || 
       *(CsrUint16 *)prim == CSR_BT_HDP_CTRL_GET_CAPAB_CFM ||
       *(CsrUint16 *)prim == CSR_BT_HDP_CTRL_GET_CAPAB_IND || 
       *(CsrUint16 *)prim == CSR_BT_HDP_ACTIVATE_CFM ||
       *(CsrUint16 *)prim == CSR_BT_HDP_UNREGISTER_CFM ||
       *(CsrUint16 *)prim == CSR_BT_HDP_DEACTIVATE_CFM)
    {
        *device = FALSE;
    }
    else
    {
        *device = TRUE;
    }

    switch (*(CsrUint16 *) (prim))
    {
        case CSR_BT_HDP_REGISTER_CFM:
        {
            returnEvent = PHDC_AG_EV_HDP_REGISTER_CFM;
            break;
        }
        case CSR_BT_HDP_REGISTER_IND:
        {
            returnEvent = PHDC_AG_EV_HDP_REGISTER_IND;
            break;
        }
        case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
        {
            returnEvent = PHDC_AG_EV_HDP_GET_CAPAB_CFM;
            break;
        }
        case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
        {
            returnEvent = PHDC_AG_EV_HDP_GET_CAPAB_IND;
            break;
        }
        case CSR_BT_HDP_ACTIVATE_CFM:
        {
            returnEvent = PHDC_AG_EV_HDP_ACTIVATE_CFM;
            break;
        }
        case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM;
            break;
        }
        case CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_CONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_DATA_IND:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_DATA_IND;
            break;
        }
        case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM;
            break;
        }
        case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND;
            break;
        }
        case CSR_BT_HDP_DATA_CFM:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_DATA_CFM;
            break;
        }
        case CSR_BT_HDP_SUSPEND_CFM:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_SUSPEND_CFM;
            break;
        }
        case CSR_BT_HDP_RESUME_CFM:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_RESUME_CFM;
            break;
        }
        case CSR_BT_HDP_RESUME_IND:
        {
            returnEvent = PHDC_AG_DEVICE_EV_HDP_RESUME_IND;
            break;
        }
        case CSR_BT_HDP_UNREGISTER_CFM:
        {
            returnEvent = PHDC_AG_EV_HDP_UNREGISTER_CFM;
            break;
        }
        case CSR_BT_HDP_DEACTIVATE_CFM:
        {
            returnEvent = PHDC_AG_EV_HDP_DEACTIVATE_CFM;
            break;
        }
        default:
            break;
    }

    return returnEvent;

}

void CsrBtPhdcAgentInitInstanceData(CsrBtPhdcAgentInstance *pInst)
{
    pInst->state = CSR_BT_PHDC_AG_ST_IDLE;
    pInst->deviceState = CSR_BT_PHDC_AG_DEVICE_ST_DISCONNECTED;
    pInst->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_FAILURE;
    CsrBtPhdcAgentFillStdConfigWsPrstApdu(pInst);
    CsrBtPhdcAgentFillStdConfigBpPrstApdu(pInst);
}


void CsrBtPhdcAgentInit(void **gash)
{
    CsrBtPhdcAgentInstance * pInst;

    *gash = CsrPmemZalloc(sizeof(CsrBtPhdcAgentInstance));

    pInst = *gash;
    CsrBtPhdcAgentInitInstanceData(pInst);

}

void CsrBtPhdcAgentDeinit(void **gash)
{
    void *prim=NULL;
    CsrUint16 eventClass=0;
    CsrBtPhdcAgentInstance *pInst = (CsrBtPhdcAgentInstance*)*gash;

    /* Free any message received */
    while(CsrSchedMessageGet(&eventClass, &prim))
    {
        switch(eventClass)
        {
            case CSR_BT_HDP_PRIM:
            {   
                CsrBtHdpFreeUpstreamMessageContents(eventClass, prim);
                break;
            }

            case CSR_BT_PHDC_AG_PRIM:
            {
                CsrBtPhdcAgFreeDownstreamMessageContents(eventClass, prim);
                break;
            }

            default:
                
                break;
        }
        
        CsrPmemFree(prim);
    }

    /* Free instance */
    CsrBtPhdcAgFreeMemory(pInst);
    CsrPmemFree(*gash);
}

void CsrBtPhdcAgentHandler(void **gash)
{
    CsrBtPhdcAgentInstance * pAgentInstance;
    CsrBool deviceEventHandler = FALSE;
    fsm_event_t event =  PHDC_INVALID_EVENT;
    CsrUint16 msgClass=0;        
    pAgentInstance = (CsrBtPhdcAgentInstance*)*gash;

    if(CsrSchedMessageGet(&msgClass, &pAgentInstance->pRecvMsg))
    {
        switch (msgClass)
        {
            case CSR_BT_HDP_PRIM:
                event = CsrBtPhdcAgentHdpIfHandler(pAgentInstance, &deviceEventHandler);
                break;

            case CSR_BT_PHDC_AG_PRIM:
                event = CsrBtPhdcAgentIfHandler(pAgentInstance, &deviceEventHandler);
                break;

            case CSR_SCHED_PRIM:
                /* Ignore */
                break;

            default:
                /* Ignore */
                break;
        }

        if(deviceEventHandler)
        {
            CsrBtPhdcAgentDeviceFsmRun(pAgentInstance, event);
        }
        else
        {
            CsrBtPhdcAgentFsmRun(pAgentInstance, event);
        }


        CsrBtPhdcAgFreeDownstreamMessageContents(msgClass, pAgentInstance->pRecvMsg);
        CsrPmemFree(pAgentInstance->pRecvMsg);
        pAgentInstance->pRecvMsg = NULL;
    }
}

#endif
