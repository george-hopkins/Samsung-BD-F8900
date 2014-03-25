/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_pmem.h"
#include "csr_vm_handler.h"
#include "csr_vm_prim.h"
#include "csr_vm_task.h"
#include "csr_hci_lib.h"
#include "csr_env_prim.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_prim.h"

#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrVmLto);

static void csrVmDataIndSend(CsrVmInstanceData *vmInst, CsrHciVendorSpecificEventInd *msg)
{
    if (vmInst->phandle != CSR_SCHED_QID_INVALID)
    {
#ifdef CSR_BLUECORE_ONOFF
        if (!vmInst->deactivating)
#endif
        {
            CsrVmDataInd *prim = CsrPmemAlloc(sizeof(CsrVmDataInd));
            prim->type = CSR_VM_DATA_IND;
            prim->payloadLength = CsrMblkGetLength(msg->data);
            prim->payload = CsrPmemAlloc(prim->payloadLength);
            CsrMblkCopyToMemory(msg->data, 0, prim->payloadLength, prim->payload);
            CsrSchedMessagePut(vmInst->phandle, CSR_VM_PRIM, prim);
        }
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrVmLto, 0,
                              "CSR_HCI_PRIM received without any receiving task registered"));
    }
    CsrMblkDestroy(msg->data);
}

static void csrSendVmData(CsrVmInstanceData *vmInst, void *msg)
{
    CsrVmDataReq *prim = (CsrVmDataReq *) msg;

    if (vmInst->phandle != CSR_SCHED_QID_INVALID)
    {
#ifdef CSR_BLUECORE_ONOFF
        if (!vmInst->deactivating)
#endif
        {
            CsrMblk *data = CsrMblkDataCreate(prim->payload, prim->payloadLength, TRUE);
            CsrHciVendorSpecificCommandReqSend(TRANSPORT_CHANNEL_VM, data);
        }
#ifdef CSR_BLUECORE_ONOFF
        else
        {
            CsrPmemFree(prim->payload);
        }
#endif
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrVmLto, 0,
                              "CSR_VM_DATA_REQ received without any task registered"));
        CsrPmemFree(prim->payload);
    }
}

static void csrVmRegisterHandler(CsrVmInstanceData *vmInst, void *msg)
{
    CsrVmRegisterReq *prim = (CsrVmRegisterReq *) msg;
    CsrVmRegisterCfm *cfmPrim = (CsrVmRegisterCfm *) CsrPmemAlloc(sizeof(CsrVmRegisterCfm));
    vmInst->phandle = prim->phandle;
    cfmPrim->type = CSR_VM_REGISTER_CFM;
    cfmPrim->result = CSR_RESULT_SUCCESS;
    CsrSchedMessagePut(prim->phandle, CSR_VM_PRIM, cfmPrim);
}

void CsrVmInit(void **gash)
{
    CsrVmInstanceData *vmInst = *gash = CsrPmemAlloc(sizeof(CsrVmInstanceData));

    vmInst->phandle = CSR_SCHED_QID_INVALID;
#ifdef CSR_BLUECORE_ONOFF
    vmInst->deactivating = FALSE;
#endif

    CsrHciRegisterVendorSpecificEventHandlerReqSend(CSR_VM_IFACEQUEUE, TRANSPORT_CHANNEL_VM);
#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_VM_IFACEQUEUE);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrVmLto, "VM", 0, NULL);
}

void CsrVmHandler(void **gash)
{
    CsrVmInstanceData *vmInst = *gash;
    CsrUint16 eventClass;
    void *msg;
    CsrPrim *primType;

    CsrSchedMessageGet(&eventClass, &msg);
    primType = msg;

    switch (eventClass)
    {
        case CSR_VM_PRIM:
        {
            if (*primType == CSR_VM_DATA_REQ)
            {
                csrSendVmData(vmInst, msg);
            }
            else if (*primType == CSR_VM_REGISTER_REQ)
            {
                csrVmRegisterHandler(vmInst, msg);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrVmLto, 0, eventClass, *primType);
            }
            break;
        }
        case CSR_HCI_PRIM:
        {
            switch (*primType)
            {
                case CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM:
                    break;
                case CSR_HCI_VENDOR_SPECIFIC_EVENT_IND:
                    csrVmDataIndSend(vmInst, msg);
                    break;
                default:
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrVmLto, 0, eventClass, *primType);
                    break;
            }
            break;
        }
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_PRIM:
        {
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No actions */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreActivateTransportResSend(CSR_VM_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = msg;
                if (indication->active)
                {
                    vmInst->deactivating = TRUE;
                    CsrTmBluecoreDeactivateTransportResSend(CSR_VM_IFACEQUEUE);
                }
                else
                {
                    vmInst->deactivating = FALSE;
                }
            }
            break;
        }
#endif
        case CSR_SCHED_PRIM:
        {
            if (*primType == CSR_CLEANUP_IND)
            {
                CsrCleanupInd *prim = (CsrCleanupInd *) msg;

                if (prim->phandle == vmInst->phandle)
                {
                    vmInst->phandle = CSR_SCHED_QID_INVALID;
                }
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrVmLto, 0, eventClass, *primType);
            }
            break;
        }
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrVmLto, 0, eventClass, 0);
            break;
        }
    }
    CsrPmemFree(msg);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrVmDeinit(void **gash)
{
    CsrVmInstanceData *vmInst = *gash;
    CsrUint16 eventClass;
    void *msg;

    while (CsrSchedMessageGet(&eventClass, &msg))
    {
        switch (eventClass)
        {
            case CSR_VM_PRIM:
                CsrVmFreeDownstreamMessageContents(eventClass, msg);
                break;
            case CSR_HCI_PRIM:
                CsrHciFreeUpstreamMessageContents(eventClass, msg);
                break;
        }
        CsrPmemFree(msg);
    }

    CsrPmemFree(vmInst);
}

#endif /* ENABLE_SHUTDOWN */
