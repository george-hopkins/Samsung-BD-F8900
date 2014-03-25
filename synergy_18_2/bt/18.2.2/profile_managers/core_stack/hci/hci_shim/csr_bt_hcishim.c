/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $

****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_hcishim.h"
#include "csr_bt_hcishim_private.h"
#include "csr_bt_tasks.h"
#include "csr_bt_hci_convert.h"
#include "csr_bt_hcisizelut.h"
#include "csr_hci_prim.h"
#include "csr_hci_lib.h"

#include "dm_hci_interface.h"
#ifdef CSR_CHIP_MANAGER_ENABLE
#include "csr_bt_hci_cm.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_task.h"
#include "csr_exceptionhandler.h"
#endif

#ifdef CSR_CHIP_MANAGER_ENABLE
static void csrBtHciCmReplayRegisterCfmHandler(CsrTmBluecoreCmReplayStartInd *recvPrim)
{
    /* do nothing */
    CSR_UNUSED(recvPrim);
}

static void csrBtHciCmReplayStartIndHandler(CsrTmBluecoreCmReplayStartInd *recvPrim)
{
    CSR_UNUSED(recvPrim);
    CsrBtHciCmRestartDetectedRequest();
    CsrBtHciCmResendStart();
}

static void csrBtHciCmStatusResetIndHandler(CsrTmBluecoreCmStatusResetInd *recvPrim)
{
    CSR_UNUSED(recvPrim);

    CsrBtHciCmResetDetectedRequest();
}
#endif

/* Register handlers with HCI task */
void CsrBtHcishimInit(void)
{
    /* Register the DM for HCI events and ACL data handling */
    CsrHciRegisterEventHandlerReqSend(DM_HCI_IFACEQUEUE);
    CsrHciRegisterAclHandlerReqSend(DM_HCI_IFACEQUEUE, 0xFFFF); /* all handles */
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrTmBlueCoreCmReplayRegisterReqSend(DM_HCI_IFACEQUEUE);
    CsrBtHciCmInit(DM_HCI_IFACEQUEUE);
#endif
}

/* Shutdown function */
#ifdef ENABLE_SHUTDOWN
void CsrBtHcishimDeinit(void)
{
    CsrUint16 type;
    void *prim;

    while(CsrSchedMessageGet(&type, &prim))
    {
        CsrHciFreeUpstreamMessageContents(type, prim);
        CsrPmemFree(prim);
    }
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrBtHciCmDeinit();
#endif
}
#endif

/* Send HCI command to HCI task */
CsrUint8 CsrBtHcishimHciCommandReq(HCI_UPRIM_T *cmd)
{
    CsrBool sendHciCmd;
    CsrUint8 *dest;
    CsrUint16 len;

    dest = CsrPmemZalloc(HCI_COMMAND_DATA_SIZE + HCI_COMMAND_HEADER_SIZE);
    CsrBtHcishimInsertLengthByOpcode(cmd);
    len = cmd->hci_cmd.length + HCI_COMMAND_HEADER_SIZE;

    if(CsrBtConvertHciCommand(cmd, dest, &sendHciCmd))
    {
        if( sendHciCmd )
        {
            CsrHciCommandReqSend(len, dest);
        }
        else
        {
            CsrPmemFree(dest);
        }
        return HCI_SUCCESS;
    }
    else
    {
        CsrPmemFree(dest);
        return HCI_ERROR_UNSPECIFIED;
    }
}

/* Send ACL data to HCI task */
void CsrBtHcishimAclDataReq(l2ca_controller_t controller,
                            CsrUint8 physical,
                            CsrUint16 logical,
                            CsrMblk *data,
                            CsrSchedQid qid)
{
    if(controller == L2CA_AMP_CONTROLLER_BT)
    {
        CsrHciAclDataReqSend(logical, data);
    }
    else
    {
        DM_DATA_TO_HCI_IND_T *prim;
        prim = CsrPmemAlloc(sizeof(DM_DATA_TO_HCI_IND_T));
        prim->type = DM_DATA_TO_HCI_IND;
        prim->controller = controller;
        prim->physical_handle = physical;
        prim->logical_handle = logical;
        prim->data = data;

        CsrSchedMessagePut(qid, DM_PRIM, prim);
    }
}

/* Main HCI and ACL data receiver function.
 * HCI shim layer replacement for dm_hci_handler() */
void CsrBtDmHciHandler(void **gash)
{
    CsrUint16 type;
    void *prim;

    if(CsrSchedMessageGet(&type, &prim))
    {
        switch(type)
        {
            case CSR_HCI_PRIM:
                switch(*(CsrUint16*)prim)
                {
                    case CSR_HCI_REGISTER_EVENT_HANDLER_CFM:
                        /* ignore */
                        break;

                    case CSR_HCI_REGISTER_ACL_HANDLER_CFM:
                        /* ignore */
                        break;

                    case CSR_HCI_REGISTER_SCO_HANDLER_CFM:
                        /* ignore */
                        break;

                    case CSR_HCI_EVENT_IND:
                        {
                            HCI_UPRIM_T *evt;
                            evt = CsrBtConvertHciEvent(((CsrHciEventInd*)prim)->payload);
                            if( evt != NULL )
                            {
                                dm_hci_event_handler(evt);
                                CsrPmemFree(evt);
                            }
                        }
                        break;

                    case CSR_HCI_ACL_DATA_IND:
                        {
                            CsrHciAclDataInd *acl = (CsrHciAclDataInd*)prim;
                            CsrBool          consumed = FALSE;
                            
                            consumed = dm_hci_l2cap_data(
#ifdef INSTALL_AMP_SUPPORT /* must be in sync with Bluestack */
                                L2CA_AMP_CONTROLLER_BT,
                                0, /* physical */
#endif
                                acl->handlePlusFlags, /* logical */
                                acl->data);

                            if (!consumed)
                            {
                                CsrMblkDestroy(acl->data);
                            }
                            acl->data = NULL; /* payload has been consumed */
                        }
                        break;

                    default:
                        /* panic */
                        break;
                }
                CsrHciFreeUpstreamMessageContents(type, prim);
                break;

            case DM_PRIM:
                switch(*(CsrUint16*)prim)
                {
#ifndef CSR_BT_EXCLUDE_DATA_FROM_HCI
                    case DM_DATA_FROM_HCI_REQ:
                        {
                            DM_DATA_FROM_HCI_REQ_T *acl = (DM_DATA_FROM_HCI_REQ_T*)prim;
                            CsrBool                consumed = FALSE;
                            
                            consumed = dm_hci_l2cap_data(
#ifdef INSTALL_AMP_SUPPORT /* must be in sync with Bluestack */
                                acl->controller,
                                acl->physical_handle,
#endif
                                acl->logical_handle,
                                acl->data);

                            if (!consumed)
                            {
                                CsrMblkDestroy(acl->data);
                            }
                            acl->data = NULL; /* payload has been consumed */

                        }
                        break;
#endif
                    default:
                        /* panic */
                        break;
                }
                /* do not consume primitive members */
                break;

#ifdef CSR_CHIP_MANAGER_ENABLE
            case CSR_TM_BLUECORE_PRIM:
                switch(*(CsrUint16*)prim)
                {
                    case CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM:
                        {
                            csrBtHciCmReplayRegisterCfmHandler(prim);
                        }
                        break;

                    case CSR_TM_BLUECORE_CM_REPLAY_START_IND:
                        {
                            csrBtHciCmReplayStartIndHandler(prim);
                        }
                        break;

                    case CSR_TM_BLUECORE_CM_STATUS_RESET_IND:
                        {
                            csrBtHciCmStatusResetIndHandler(prim);
                        }
                        break;

                    default:
                        break;
                }
                break;
#endif
            default:
                /* panic */
                break;
        }

        /* Free primitive container */
        CsrPmemFree(prim);
    }

    CSR_UNUSED(gash);
}
