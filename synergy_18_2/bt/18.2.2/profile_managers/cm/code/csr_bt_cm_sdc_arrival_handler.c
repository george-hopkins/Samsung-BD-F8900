/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_sdc.h"
#include "csr_exceptionhandler.h"

/*************************************************************************************
  cmSdpArrivalHandler:
************************************************************************************/
void CsrBtCmSdcArrivalHandler(cmInstanceData_t *cmData)
{ /* Handles incoming events from SDP layer */
    SDC_UPRIM_T  *sdcPrim;

    sdcPrim = (SDC_UPRIM_T *)cmData->recvMsgP;

    switch(sdcPrim->type)
    {
        case SDC_OPEN_SEARCH_CFM:
        {
            CsrBtCmSdcOpenSearchCfmHandler(cmData);
            break;
        }
        case SDC_SERVICE_SEARCH_CFM:
        {
            CsrBtCmSdcServiceSearchCfmHandler(cmData);
            break;
        }
        case SDC_SERVICE_ATTRIBUTE_CFM:
        {
            CsrBtCmSdcServiceAttributeCfmHandler(cmData);
            break;
        }
        case SDC_CLOSE_SEARCH_IND:
        {
            CsrBtCmSdcCloseSearchIndHandler(cmData);
            break;
        }
        case SDS_REGISTER_CFM:
        {
            CsrBtCmSdsRegisterCfmHandler(cmData);
            break;
        }
        case SDS_UNREGISTER_CFM:
        {
            CsrBtCmSdsUnRegisterCfmHandler(cmData);
            break;
        }
        case SDC_CONFIG_CFM:
            {
                /* Ignore this one */
                break;
            }
        case SDS_CONFIG_CFM:
            {
                /* Ignore this one */
                break;
            }
        default :
        {
            CsrBtCmStateEventException(SDP_PRIM, sdcPrim->type, cmData->globalState);
            break;
        }
    }
}
