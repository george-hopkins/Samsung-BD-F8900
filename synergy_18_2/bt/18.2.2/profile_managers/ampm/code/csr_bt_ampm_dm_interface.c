/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Checks if a physical link exists that matches the supplied criteria
 * and generates the response back to DM */
static void csrBtAmpmDmAmpmVerifyPhysicalLinkInd(
    CsrBtAmpmInstance * inst,
    DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T * prim)
{
    CsrBtAmpmPhyLink *pPhyLink = NULL;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    pPhyLink =
        CsrBtAmpmPhyGetByBdaddrAndLocalAmpId(inst, prim->bd_addr,
                                             CSR_BT_AMPM_L2CA_TO_AMP_ID(prim->local_amp_id));
    if (NULL != pPhyLink)
    {
        if (CsrBtAmpmPhyIsConnected(pPhyLink) && pPhyLink->haveReadLsto)
        {
            dm_ampm_verify_physical_link_rsp(&(prim->bd_addr),
                                             prim->identifier,
                                             TRUE, /*link exists */
                                             CsrBtAmpmLocalGetBestEffortFlushTimeout(pPhyLink->pLocalAMPInfo),
                                             pPhyLink->linkSupervisionTimeout);
        }
        else
        {
            pPhyLink->verifyIdentifier = prim->identifier;
            pPhyLink->verifyResponseRequired = TRUE;
        }
    }
    else
    {
        dm_ampm_verify_physical_link_rsp(&(prim->bd_addr),
                                         prim->identifier,
                                         FALSE, /*link exists */
                                         0, 0); /* dummy timeout values */
    }
}

/* Checks if the specified AMP exists and if it does then generates
 * the HCI READ DATA BLOCK SIZE to the AMP. If the AMP doesn't exist
 * then an error is reported back to DM. */
static void csrBtAmpmDmAmpmReadDataBlockSizeInd(
    CsrBtAmpmInstance * inst,
    DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T * prim)
{
    CsrBtAmpmId amp_id;
    CsrBtAmpmLocalAmp *pLocalAmp = NULL;

    CSR_BT_AMPM_ASSERT(prim != NULL);

    amp_id = CSR_BT_AMPM_L2CA_TO_AMP_ID(prim->local_amp_id);
    pLocalAmp = CsrBtAmpmLocalGetById(inst, amp_id);
    if (NULL != pLocalAmp)
    {
        CsrBtAmpmSendHciReadDataBlockSize(inst, amp_id, NULL);
    }
    else
    {
        /* The amp is invalid */
        DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T *pDmAmpmReadDataBlockSizeRsp =
            pnew(DM_AMPM_READ_DATA_BLOCK_SIZE_RSP_T);
        pDmAmpmReadDataBlockSizeRsp->type = DM_AMPM_READ_DATA_BLOCK_SIZE_RSP;
        pDmAmpmReadDataBlockSizeRsp->local_amp_id = prim->local_amp_id;
        pDmAmpmReadDataBlockSizeRsp->status = 1;    /* anything other than 0 is failure */
        CsrSchedMessagePut(DM_IFACEQUEUE, DM_PRIM,
                           pDmAmpmReadDataBlockSizeRsp);
    }
}

void CsrBtAmpmDmHandler(
    CsrBtAmpmInstance * inst)
{
    void *prim = inst->msg;

    switch (*(CsrUint16 *) prim)
    {
        case DM_AMPM_REGISTER_CFM:
            /* DM is now up and running. Read local address */
            dm_ampm_read_bd_addr_req();
            break;

        case DM_AMPM_READ_BD_ADDR_CFM:
            {
                DM_AMPM_READ_BD_ADDR_CFM_T *pDmAmpmReadBdAddrCfm;

                pDmAmpmReadBdAddrCfm = (DM_AMPM_READ_BD_ADDR_CFM_T *) prim;

                if (0 == pDmAmpmReadBdAddrCfm->status)
                {
                    CsrBtAmpmDataSetLocalBtAddr(inst,
                                                pDmAmpmReadBdAddrCfm->bd_addr);
                }
            }
            break;

        case DM_AMPM_LINK_KEY_NOTIFICATION_IND:
            CsrBtAmpmDataDmAmpmKeyNotificationInd(inst,
                                                  (DM_AMPM_LINK_KEY_NOTIFICATION_IND_T *) prim);
            break;

        case DM_AMPM_CONNECT_IND:
            CsrBtAmpmL2capDmAmpmConnectInd(inst,
                                           (DM_AMPM_CONNECT_IND_T *) prim);
            break;

        case DM_AMPM_CONNECT_CHANNEL_IND:
            CsrBtAmpmL2capDmAmpmConnectChannelInd(inst,
                                                  (DM_AMPM_CONNECT_CHANNEL_IND_T *) prim);
            break;

        case DM_AMPM_DISCONNECT_CHANNEL_IND:
            CsrBtAmpmL2capDmAmpmDisconnectChannelInd(inst,
                                                     (DM_AMPM_DISCONNECT_CHANNEL_IND_T *) prim);
            break;

        case DM_AMPM_VERIFY_PHYSICAL_LINK_IND:
            csrBtAmpmDmAmpmVerifyPhysicalLinkInd(inst,
                                                 (DM_AMPM_VERIFY_PHYSICAL_LINK_IND_T *) prim);
            break;

        case DM_AMPM_READ_DATA_BLOCK_SIZE_IND:
            csrBtAmpmDmAmpmReadDataBlockSizeInd(inst,
                                                (DM_AMPM_READ_DATA_BLOCK_SIZE_IND_T *) prim);
            break;

        case DM_ACL_CONN_HANDLE_IND:
            /* An ACL opening will trigger attempting to perform AMP
             * discovery towards the peer BT device. */
            CsrBtAmpmBtdevDmAclConnHandleInd(inst, (DM_ACL_CONN_HANDLE_IND_T *) prim);
            break;

        case DM_ACL_CLOSED_IND:
            /* An ACL close indication will trigger local clearing of any
             * physical links connected to the peer BT device. */
            CsrBtAmpmBtdevDmAclClosedInd(inst, (DM_ACL_CLOSED_IND_T *) prim);
            break;

        case DM_AMPM_SIMPLE_PAIRING_ENABLED_IND:
            /* Bluetooth SSP mode enabled. We can't start business
             * until this has been enabled */
            inst->ampSspEnabled = TRUE;
            break;

        case DM_AMPM_REMOTE_FEATURES_IND:
            /* DM has fetched the remote features -- i.e. we now know
             * whether the peer at the other end of the ACL supports
             * SSP */
            CsrBtAmpmBtdevRemoteFeaturesInd(inst, (DM_AMPM_REMOTE_FEATURES_IND_T*)prim);
            break;

        default:
            break;
    }
}
