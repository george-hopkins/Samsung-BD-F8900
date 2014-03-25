/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_exceptionhandler.h"

/***************************************************************************
  CsrBtCmL2CaArrivalHandler:
****************************************************************************/
void CsrBtCmL2CaArrivalHandler(cmInstanceData_t *cmData)
{ /* Identify the event type from the L2CAP layer and handle it */

    L2CA_UPRIM_T    *l2caPrim;

    l2caPrim = (L2CA_UPRIM_T *)cmData->recvMsgP;

    switch(l2caPrim->type)
    {
        case L2CA_DATAWRITE_CFM :
        {
            CsrBtCmL2caDataWriteCfmHandler(cmData);
            break;
        }
        case L2CA_DATAREAD_IND :
        {
            CsrBtCmL2caDataReadIndHandler(cmData);
            break;
        }
        case L2CA_AUTO_CONNECT_CFM :
        {
            CsrBtCmL2caAutoConnectCfmHandler(cmData);
            break;
        }
        case L2CA_AUTO_CONNECT_IND:
        {
            CsrBtCmL2caAutoConnectIndHandler(cmData);
            break;
        }
        case L2CA_DISCONNECT_IND :
        {
            CsrBtCmL2caDisconnectIndHandler(cmData);
            break;
        }
        case L2CA_DISCONNECT_CFM :
        {
            CsrBtCmL2caDisconnectCfmHandler(cmData);
            break;
        }
        case L2CA_REGISTER_CFM :
        {
            CsrBtCmL2caRegisterCfmHandler(cmData);
            break;
        }
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_UNREGISTER
        case L2CA_UNREGISTER_CFM :
        {
            CsrBtCmL2caUnRegisterCfmHandler(cmData);
            break;
        }
#endif
        case L2CA_TIMEOUT_IND :
        {
            CsrBtCmL2caTimeoutIndHandler(cmData);
            break;
        }
#ifdef CSR_AMP_ENABLE
        case L2CA_MOVE_CHANNEL_IND:
        {
            CsrBtCmL2caMoveChannelIndHandler(cmData);
            break;
        }
        case L2CA_MOVE_CHANNEL_CFM:
        {
            CsrBtCmL2caMoveChannelCfmHandler(cmData);
            break;
        }
        case L2CA_MOVE_CHANNEL_CMP_IND:
        {
            CsrBtCmL2caMoveChannelCmpIndHandler(cmData);
            break;
        }
        case L2CA_AMP_LINK_LOSS_IND:
        {
            CsrBtCmL2caAmpLinkLossIndHandler(cmData);
            break;
        }
#endif
#ifdef CSR_BT_INSTALL_L2CAP_CONNLESS_SUPPORT
        case L2CA_MAP_FIXED_CID_IND:
        {
            CsrBtCmL2caMapFixedCidIndHandler(cmData);
            break;
        }
        case L2CA_MAP_FIXED_CID_CFM:
        {
            CsrBtCmL2caMapFixedCidCfmHandler(cmData);
            break;
        }
        case L2CA_UNMAP_FIXED_CID_IND:
        {
            CsrBtCmL2caUnmapFixedCidIndHandler(cmData);
            break;
        }
#endif        
        case L2CA_BUSY_IND:
        {
            CsrBtCmL2caBusyIndHandler(cmData);
            break;
        }
#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_DATA_ABORT
        case L2CA_DATAWRITE_ABORT_CFM:
        {
            CsrBtCmL2caDatawriteAbortCfmHandler(cmData);
            break;
        }
#endif
        case L2CA_GET_CHANNEL_INFO_CFM:
        {
            CsrBtCmL2caGetChannelInfoCfmHandler(cmData);
            break;
        }
        default:
        {
            CsrBtCmStateEventException(L2CAP_PRIM, l2caPrim->type, cmData->globalState);
            break;
        }
    }
}
#endif

