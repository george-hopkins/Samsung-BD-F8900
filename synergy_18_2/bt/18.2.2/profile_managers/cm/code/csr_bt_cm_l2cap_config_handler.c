/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_dm.h"

void CsrBtCmL2capStoreConfigOptions(L2CA_CONFIG_T *config, cmL2caConnInstType *thelink)
{
    if (config->options & L2CA_SPECIFY_MTU)
    {
        thelink->outgoingMtu = config->mtu;
    }
    else
    {
        thelink->outgoingMtu = L2CA_MTU_DEFAULT;
    }

    if (config->options & L2CA_SPECIFY_FLUSH)
    {
        thelink->outgoingFlush = config->flush_to;
    }
    else
    {
        thelink->outgoingFlush = L2CA_FLUSH_TO_DEFAULT;
    }

    if ((config->options & L2CA_SPECIFY_FLOW) &&
        (config->flow != NULL) &&
        (config->flow->mode == L2CA_FLOW_MODE_ENHANCED_RETRANS))
    {
        thelink->ertm = TRUE;

        thelink->txMaxCount = config->flow->tx_window;
        
        if((config->options & L2CA_SPECIFY_EXT_WINDOW) && (config->flow->tx_window == 0))
        {
            /* use ext win size, overrules flow win ctrl size */
            thelink->txMaxCount = config->ext_window;
        }
    }
    else if (thelink->dataPriority == CSR_BT_CM_PRIORITY_HIGH)
    {
        thelink->ertm = FALSE;;
        thelink->txMaxCount = MAX_BUFFERED_L2CAP_HIGH_PRIORITY_REQUESTS;
    }
    else
    {
        thelink->ertm = FALSE;;
        thelink->txMaxCount = MAX_BUFFERED_L2CAP_REQUESTS;
    }
    L2CA_FreeConfigPtrs(config);
}

void CsrBtCmL2capConfigHandler(cmInstanceData_t *cmData, cmL2caConnInstType * l2CaConnection)
{
    if( l2CaConnection->state == l2capConnect_s )
    {
        /* The connection was initiated by the local device. Ensure
         * that SNIFF and PARK is supported */
        cmData->smVar.resultCode        = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmData->smVar.resultSupplier    = CSR_BT_SUPPLIER_CM;
        CsrBtCmDmWriteLpSettingsReqMsgSend(l2CaConnection->deviceAddr, L2CAP_PLAYER);
    }
    else
    {
        /* The connection was initiated by a remote device. Remove the
         * COD and if necessary turn off discoverable mode */
        CSR_BT_CM_STATE_CHANGE(l2CaConnection->state, l2capConnectAcceptFinal_s);
        CsrBtCmWriteClassOfDevice(L2CAP_PLAYER);
    }
}

#endif /* EXCLUDE_CSR_BT_L2CA_MODULE */

