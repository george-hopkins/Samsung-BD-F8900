/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ampm_main.h"

/* Handles an incoming HCI read data block size eventand passes it on
 * to DM */
static void csrBtAmpmHciReadDataBlockSize(
    CsrBtAmpmLocalAmp * pLocalAmp,
    CsrBtAmpHciReadDataBlockSizeReturn * pHciReadDataBlockSizeReturn)
{
    CSR_BT_AMPM_ASSERT(pLocalAmp != NULL);
    CSR_BT_AMPM_ASSERT(pHciReadDataBlockSizeReturn != NULL);

    /* This response needs to be routed back to DM as it triggered
     * AMPM to send the command */
    dm_ampm_read_data_block_size_rsp(pLocalAmp->pAmpInfo->id,
                                     pHciReadDataBlockSizeReturn->status,
                                     FALSE, /* fragmentable (no) */
                                     (CsrUint16)pLocalAmp->pAmpInfo->maxPDUSize,
                                     pHciReadDataBlockSizeReturn->maxAclDataPacketLength,
                                     pHciReadDataBlockSizeReturn->dataBlockLength,
                                     pHciReadDataBlockSizeReturn->totalNumberOfBlocks);
}

/* Handles an incoming HCI Command Status command which indicates
 * whether the commandOpcode parameter has been received and is able
 * to perform the operation requested. */
static void csrBtAmpmHciCommandStatus(
    CsrBtAmpmInstance * inst,
    CsrBtAmpmPalHciEventReq * ampm_hci_prim)
{
    CsrBtAmpHciCommandStatusEvt *pHciCommandStatus;
    CsrBtAmpmId amp_id;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(ampm_hci_prim != NULL);
    CSR_BT_AMPM_ASSERT(ampm_hci_prim->data != NULL);

    amp_id = (CsrBtAmpmId) ampm_hci_prim->ampId;
    pHciCommandStatus = (CsrBtAmpHciCommandStatusEvt *) ampm_hci_prim->data;
    pLocalAmp = CsrBtAmpmLocalGetById(inst, amp_id);

    if (NULL != pLocalAmp)
    {
        void *pRequester =
            CsrBtAmpmLocalStatusForQueuedHciCmd(inst, pLocalAmp,
                                                pHciCommandStatus->commandOpcode);

        switch (pHciCommandStatus->commandOpcode)
        {
            case CSR_BT_AMP_HCI_CREATE_PHYSICAL_LINK_CODE:
                if (pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    CsrBtAmpmPhyLinkCommandStatusSuccess(inst, (CsrBtAmpmPhyLink *) pRequester);
                }
                else
                {
                    CsrBtAmpmPhyLinkCommandStatusNeg(inst, (CsrBtAmpmPhyLink *) pRequester);
                }
                break;

            case CSR_BT_AMP_HCI_ACCEPT_PHYSICAL_LINK_REQUEST_CODE:
                if (pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    CsrBtAmpmPhyLinkCommandStatusSuccess(inst, (CsrBtAmpmPhyLink *) pRequester);
                }
                else
                {
                    CsrBtAmpmPhyLinkCommandStatusNeg(inst, (CsrBtAmpmPhyLink *) pRequester);
                }
                break;

            case CSR_BT_AMP_HCI_DISCONNECT_PHYSICAL_LINK_CODE:
                 CsrBtAmpmPhyHciDisconnectPhysicalLinkComplete(
                        inst,
                        (CsrBtAmpmPhyLink*) pRequester,
                        pHciCommandStatus->status,
                        CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS);
                    /* For the moment behave as if the disconnect was
                     * successful, we probably want to perfrom a reset
                     * if the disconnect fails
                     * but need to agree the conditions that a reset
                     * should happen with the PAL team. */
                break;

            case CSR_BT_AMP_HCI_CREATE_LOGICAL_LINK_CODE:
                if (pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    /* ignore as the FSM is waiting for the command complete */
                }
                else
                {
                    CsrBtAmpmLogicalHciCommandStatusFailure(inst, (CsrBtAmpmLogicalLink *) pRequester);
                }
                break;

            case CSR_BT_AMP_HCI_DISCONNECT_LOGICAL_LINK_CODE:
                if (pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    /* ignore as the FSM is waiting for the command complete */
                }
                else
                {
                    CsrBtAmpmLogicalHciCommandStatusFailure(inst, (CsrBtAmpmLogicalLink *) pRequester);
                }
                break;

            case CSR_BT_AMP_HCI_FLOW_SPEC_MODIFY_CODE:
                if (!pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    /* ignore as the FSM is waiting for the command complete */
                }
                else
                {
                    CsrBtAmpmLogicalHciCommandStatusFailure(inst, (CsrBtAmpmLogicalLink *) pRequester);
                }
                break;

            case CSR_BT_AMP_HCI_READ_LOCAL_AMP_INFO_CODE:
                if (pHciCommandStatus->status == CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    /* ignore as the FSM is waiting for the command complete */
                }
                else
                {
                    /* Don't expect to get a command status event for
                     * the HCI READ LOCAL AMP INFO CMD but we have and
                     * it is indicating that the command failed. */
                    CsrBtAmpmLocalHciReadAmpInfoFailure(inst, pLocalAmp);
                }
                break;

            case CSR_BT_AMP_HCI_READ_LOCAL_AMP_ASSOC_CODE:
                if (pHciCommandStatus->status != CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    CsrBtAmpmLocalHciReadAmpAssocFailure(inst, pLocalAmp);
                }
                break;

            case CSR_BT_AMP_HCI_WRITE_REMOTE_AMP_ASSOC_CODE:
                if (pHciCommandStatus->status != CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS)
                {
                    CsrBtAmpmLocalHciWriteRemoteAmpAssocFailure(inst, pLocalAmp);
                }
                break;
                
            default:
                /* ignore */
                break;
        }
    }
}

/* Handles an incoming HCI Command Complete event which transmit
 * return status of a command and the other event parameters that are
 * specified for the issued HCI command */
static void csrBtAmpmHciCommandComplete(CsrBtAmpmInstance * inst,
                                        CsrBtAmpmPalHciEventReq * ampm_hci_prim)
{
    CsrBtAmpHciCommandCompleteEvt *pHciCommandComplete;
    CsrBtAmpmId amp_id;
    CsrBtAmpmLocalAmp *pLocalAmp;

    CSR_BT_AMPM_ASSERT(ampm_hci_prim != NULL);
    CSR_BT_AMPM_ASSERT(ampm_hci_prim->data != NULL);

    amp_id = (CsrBtAmpmId) ampm_hci_prim->ampId;

    pHciCommandComplete = (CsrBtAmpHciCommandCompleteEvt *) ampm_hci_prim->data;

    pLocalAmp = CsrBtAmpmLocalGetById(inst, amp_id);

    if (NULL != pLocalAmp)
    {
        switch(pHciCommandComplete->returnParameters.commandCode)
        {
            case CSR_BT_AMP_HCI_NO_OPERATION_CODE:
                {
                    CsrBtAmpmLocalHciNop(inst, pLocalAmp);
                }
                break;

            case CSR_BT_AMP_HCI_READ_LOCAL_AMP_INFO_CODE:
                {
                    (void)CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                           pHciCommandComplete->returnParameters.commandCode);
                    CsrBtAmpmLocalHciReadLocalAmpInfo(inst, pLocalAmp,
                                                      &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                        readLocalAmpInfoReturn));
                }
                break;

            case CSR_BT_AMP_HCI_READ_LOCAL_AMP_ASSOC_CODE:
                {
                    (void)CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                           pHciCommandComplete->returnParameters.commandCode);
                    CsrBtAmpmLocalHciReadLocalAmpAssoc(inst, pLocalAmp,
                                                       &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                         readLocalAmpAssocReturn));
                }
                break;

            case CSR_BT_AMP_HCI_WRITE_REMOTE_AMP_ASSOC_CODE:
                {
                    (void)CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                           pHciCommandComplete->returnParameters.commandCode);
                    CsrBtAmpmLocalHciWriteRemoteAmpAssoc(inst, pLocalAmp,
                                                         &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                           writeRemoteAmpAssocReturn));
                }
                break;

            case CSR_BT_AMP_HCI_READ_DATA_BLOCK_SIZE_CODE:
                {
                    (void) CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                            CSR_BT_AMP_HCI_READ_DATA_BLOCK_SIZE_CODE);
                    csrBtAmpmHciReadDataBlockSize(pLocalAmp,
                                                  &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                    readDataBlockSizeReturn));
                }
                break;

            case CSR_BT_AMP_HCI_READ_LINK_SUPERVISION_TIMEOUT_CODE:
                {
                    CsrBtAmpmPhyLink *pPhyLink;
                    pPhyLink = CsrBtAmpmPhyGetByLocalAmpId(inst, amp_id);
                    (void)CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                           CSR_BT_AMP_HCI_READ_LINK_SUPERVISION_TIMEOUT_CODE);
                    if (NULL != pPhyLink)
                    {
                        CsrBtAmpmPhyHciReadSupervisionTimeout(inst, pPhyLink,
                                                              &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                                readTimeoutReturn));
                    }
                }
                break;

            case CSR_BT_AMP_HCI_LOGICAL_LINK_CANCEL_CODE:
                {
                    void *pRequester = CsrBtAmpmLocalRemoveQueuedHciCmd(inst,
                                                                        pLocalAmp,
                                                                        pHciCommandComplete->returnParameters.commandCode);

                    if (NULL != pRequester)
                    {
                        CsrBtAmpmLogicalHciLogicalLinkCancel(inst, pRequester,
                                                             &(pHciCommandComplete->returnParameters.hciReturnParam.
                                                               logicalLinkCancelReturn));
                    }
                }

            default:
                /* ignore */
                break;
        }
    }
}

/* Handles an incoming HCI Number of completed data blocks event.
 * This event may contain information relating to one or more logical
 * links on the AMP. This event needs to be forwarded to DM. So that
 * it can updates its credits status for each logical link. */
static void csrBtAmpmHciNumberOfCompletedDataBlocks(CsrBtAmpmPalHciEventReq * ampm_hci_prim)
{
    CsrBtAmpHciNumberOfCompletedDataBlocksEvt *pHciNumCompletedDataBlocks;
    CsrBtAmpmId ampId;
    DM_AMPM_NCB_T *ncbs[DM_AMPM_NUM_COMPLETED_BLOCKS_PTRS];

    CSR_BT_AMPM_ASSERT(ampm_hci_prim != NULL);
    CSR_BT_AMPM_ASSERT(ampm_hci_prim->data != NULL);

    ampId = (CsrBtAmpmId) ampm_hci_prim->ampId;
    pHciNumCompletedDataBlocks =
        (CsrBtAmpHciNumberOfCompletedDataBlocksEvt*)ampm_hci_prim->data;

    /* At the moment the common xml is not encoding the event for
     * multiple link entries so the following code will need changing
     * once it is. */
    CSR_BT_AMPM_ASSERT(pHciNumCompletedDataBlocks->numberOfHandles <= 1);

    CsrMemSet(ncbs, 0, sizeof(ncbs));

    if(pHciNumCompletedDataBlocks->numberOfHandles > 0)
    {
        ncbs[0] = CsrPmemAlloc(sizeof(DM_AMPM_NCB_T));
        ncbs[0]->logical_link_handle = pHciNumCompletedDataBlocks->handle;
        ncbs[0]->num_of_completed_packets = pHciNumCompletedDataBlocks->numberOfCompletedPackets;
        ncbs[0]->num_of_completed_blocks = pHciNumCompletedDataBlocks->numberOfCompletedBlocks;
    }

    dm_ampm_number_completed_data_blocks_req(ampId,
                                             pHciNumCompletedDataBlocks->totalNumDataBlocks,
                                             pHciNumCompletedDataBlocks->numberOfHandles,
                                             ncbs);
}

void CsrBtAmpmHciHandler(CsrBtAmpmInstance *inst)
{
    CsrUint8 *data;
    CsrUint8 ampId;
    CsrBtAmpmPalHciEventReq *prim;

    prim = (CsrBtAmpmPalHciEventReq *) inst->msg;
    CSR_BT_AMPM_ASSERT(prim != NULL);

    data = prim->data;
    ampId = prim->ampId;
    CSR_BT_AMPM_ASSERT(data != NULL);

    /* HCI test mode redirection */
    if(CsrBtAmpmSendRxHciToTester(inst))
    {
        CsrBtAmpmUpstreamTestHciInd(ampId,
                                    prim->data,
                                    prim->length,
                                    inst->ampmRxSduQid);
        prim->data = NULL;
        prim->length = 0;
        return;
    }

    /* Top 8 bits are the opcode */
    switch (*(CsrUint8*)data)
    {
        case CSR_BT_AMP_HCI_COMMAND_COMPLETE_CODE:
            {
                CsrBtAmpHciCommandCompleteEvt *pHciCommandComplete;

                pHciCommandComplete = (CsrBtAmpHciCommandCompleteEvt *) (data);

                csrBtAmpmHciCommandComplete(inst, prim);

                if (CSR_BT_AMP_HCI_READ_LOCAL_AMP_ASSOC_CODE == pHciCommandComplete->returnParameters.commandCode)
                {
                    if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS ==
                        pHciCommandComplete->returnParameters.hciReturnParam.readLocalAmpAssocReturn.status)
                    {
                        CsrPmemFree(pHciCommandComplete->returnParameters.
                                    hciReturnParam.readLocalAmpAssocReturn.
                                    assocFragment.data);
                    }
                }
            }
            break;

        case CSR_BT_AMP_HCI_COMMAND_STATUS_CODE:
            csrBtAmpmHciCommandStatus(inst, prim);
            break;

        case CSR_BT_AMP_HCI_HARDWARE_ERROR_CODE:
            /* can't do much here */
            break;

        case CSR_BT_AMP_HCI_DATA_BUFFER_OVERFLOW_CODE:
            /* can't do much here */
            break;

        case CSR_BT_AMP_HCI_PHYSICAL_LINK_COMPLETE_CODE:
            CsrBtAmpmPhyHciPhysicalLinkComplete(inst, prim);
            break;

        case CSR_BT_AMP_HCI_DISCONNECT_PHYSICAL_LINK_COMPLETE_CODE:
            {
                CsrBtAmpHciDisconnectPhysicalLinkCompleteEvt *pAmpmHciDisconnectPhysicalLinkComplete;
                CsrBtAmpmPhyLink *pPhyLink;
                CsrBtAmpmLocalAmp *pLocalAmp;

                pLocalAmp = CsrBtAmpmLocalGetById(inst, ampId);

                /* We don't get a status for these commands make sure that
                 * they aren't sitting on the queue preventing
                 * transmission if the underlying physical link is no
                 * longer connected. */
                (void)CsrBtAmpmLocalRemoveQueuedHciCmd(inst, pLocalAmp,
                                                       CSR_BT_AMP_HCI_LOGICAL_LINK_CANCEL_CODE);

                pAmpmHciDisconnectPhysicalLinkComplete = (CsrBtAmpHciDisconnectPhysicalLinkCompleteEvt *) data;
                pPhyLink = CsrBtAmpmPhyGetByAmpPhyHandle(inst,
                                                         pAmpmHciDisconnectPhysicalLinkComplete->physicalLinkHandle);
                if (NULL != pPhyLink)
                {
                    CsrBtAmpmPhyHciDisconnectPhysicalLinkComplete(
                        inst,
                        pPhyLink,
                        pAmpmHciDisconnectPhysicalLinkComplete->status,
                        pAmpmHciDisconnectPhysicalLinkComplete->reason);
                }
            }
            break;

        case CSR_BT_AMP_HCI_LOGICAL_LINK_COMPLETE_CODE:
            CsrBtAmpmLogicalHciLogicalLinkComplete(inst, prim);
            break;

        case CSR_BT_AMP_HCI_DISCONNECT_LOGICAL_LINK_COMPLETE_CODE:
            CsrBtAmpmLogicalHciDisconnectLogicalLinkComplete(inst, prim);
            break;

        case CSR_BT_AMP_HCI_PHYSICAL_LINK_LOSS_EARLY_WARNING_CODE:
            /* ignore for the time being */
            break;

        case CSR_BT_AMP_HCI_QOS_VIOLATION_CODE:
            /* can't do much here */
            break;

        case CSR_BT_AMP_HCI_PHYSICAL_LINK_RECOVERY_CODE:
            /* ignore for the time being */
            break;

        case CSR_BT_AMP_HCI_CHANNEL_SELECT_CODE:
            CsrBtAmpmPhyHciChannelSelect(inst, prim);
            break;

        case CSR_BT_AMP_HCI_FLOW_SPEC_MODIFY_COMPLETE_CODE:
            CsrBtAmpmLogicalHciFlowSpecModifyComplete(inst, prim);
            break;

        case CSR_BT_AMP_HCI_NUMBER_OF_COMPLETED_DATA_BLOCKS_CODE:
            csrBtAmpmHciNumberOfCompletedDataBlocks(prim);
            break;

        case CSR_BT_AMP_HCI_FLUSH_OCCURRED_CODE:
            /* Ignore this one. We only rely on NoCPs to free buffer
             * space */
            break;

        case CSR_BT_AMP_HCI_AMP_STATUS_CHANGE_CODE:
            {
                CsrBtAmpHciAmpStatusChangeEvt *pHciAmpStatusChange;

                pHciAmpStatusChange = (CsrBtAmpHciAmpStatusChangeEvt *) data;
                if (CSR_BT_AMP_HCI_STATUS_CODE_SUCCESS == pHciAmpStatusChange->status)
                {
                    CsrBtAmpmLocalStatusChange(inst, ampId,
                                               pHciAmpStatusChange->ampStatus);
                }
            }
            break;

        default:
            /* ignore */
            break;
    }
}
