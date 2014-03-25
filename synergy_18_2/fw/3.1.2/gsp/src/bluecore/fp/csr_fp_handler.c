/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_message_queue.h"
#include "csr_fp_lib.h"
#include "csr_fp_task.h"
#include "csr_fp_handler.h"
#include "csr_fp_sef.h"
#include "csr_bccmd_lib.h"
#include "csr_hci_lib.h"
#include "csr_log_text_2.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrFpLto);

static void saveMessage(CsrFpInstType *inst)
{
    CsrMessageQueuePush(&inst->queue, CSR_FP_PRIM, inst->msg);
    inst->msg = NULL;
#ifdef CSR_BLUECORE_ONOFF
    CsrFpActivate(inst);
#endif
}

static void discardMessage(CsrFpInstType *inst)
{
    CsrFpFreeDownstreamMessageContents(CSR_FP_PRIM, inst->msg);
}

/* FP jump table - downstream */
typedef void (*CsrFpSignalHandlerType)(CsrFpInstType *inst);
static const CsrFpSignalHandlerType fpHandlers[][CSR_FP_PRIM_DOWNSTREAM_COUNT] =
{
    /* CSR_FP_INIT_ST */
    {
        saveMessage,                            /* CSR_FP_CREATE_REQ   */
        discardMessage,                         /* CSR_FP_WRITE_REQ    */
        discardMessage,                         /* CSR_FP_CLEAR_REQ    */
        discardMessage,                         /* CSR_FP_DESTROY_REQ  */
    },
    /* CSR_FP_ACTIVE_ST */
    {
        CsrFpCreateReqHandler,                  /* CSR_FP_CREATE_REQ   */
        CsrFpWriteReqHandler,                   /* CSR_FP_WRITE_REQ    */
        CsrFpClearReqHandler,                   /* CSR_FP_CLEAR_REQ    */
        CsrFpDestroyReqHandler,                 /* CSR_FP_DESTROY_REQ  */
    },
#ifdef CSR_BLUECORE_ONOFF
    /* CSR_FP_DEACTIVATING_ST */
    {
        discardMessage,                         /* CSR_FP_CREATE_REQ   */
        discardMessage,                         /* CSR_FP_WRITE_REQ    */
        discardMessage,                         /* CSR_FP_CLEAR_REQ    */
        discardMessage,                         /* CSR_FP_DESTROY_REQ  */
    },
#endif
};

/************************************************************/
/* CsrFpHandler                                             */
/************************************************************/
void CsrFpHandler(void **gash)
{
    CsrFpInstType *inst = *gash;
    CsrUint16 event = 0;
    CsrPrim *primType;

    CsrSchedMessageGet(&event, &inst->msg);
    primType = inst->msg;

    switch (event)
    {
        case CSR_FP_PRIM:
        {
            if ((*primType < CSR_FP_PRIM_DOWNSTREAM_COUNT) && (fpHandlers[inst->state][*primType] != NULL))
            {
                fpHandlers[inst->state][*primType](inst);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, *primType);
            }
            break;
        }
        case CSR_BCCMD_PRIM:
        {
            switch (*primType)
            {
                case CSR_BCCMD_CFM:
                {
                    CsrBccmdCfm *confirm = inst->msg;
#ifdef CSR_BLUECORE_ONOFF
                    if (inst->state != CSR_FP_DEACTIVATING_ST)
#endif
                    {
                        CsrFpBccmdCfmHandler(inst);
                    }
                    CsrPmemFree(confirm->payload);
                    break;
                }
                default:
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, *primType);
                    break;
            }
            break;
        }
        case CSR_HCI_PRIM:
        {
            switch (*primType)
            {
                case CSR_HCI_REGISTER_ACL_HANDLER_CFM:
                    break;
                case CSR_HCI_UNREGISTER_ACL_HANDLER_CFM:
                    break;
                case CSR_HCI_ACL_DATA_IND:
#ifdef CSR_BLUECORE_ONOFF
                    if (inst->state != CSR_FP_DEACTIVATING_ST)
#endif
                    {
                        CsrFpHciAclDataIndHandler(inst);
                    }
#ifdef CSR_BLUECORE_ONOFF
                    else
                    {
                        CsrHciAclDataInd *msg = inst->msg;
                        CsrMblkDestroy(msg->data);
                    }
#endif
                    break;
                default:
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, *primType);
                    break;
            }
            break;
        }
#ifdef CSR_BLUECORE_ONOFF
        case CSR_TM_BLUECORE_PRIM:
        {
            if (*primType == CSR_TM_BLUECORE_REGISTER_CFM)
            {
                /* No need to do anything */
            }
            else if (*primType == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreActivateTransportResSend(CSR_FP_IFACEQUEUE);
            }
            else if (*primType == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
            {
                CsrTmBluecoreDeactivateTransportInd *indication = inst->msg;
                if (indication->active)
                {
                    /* Transport has already been stopped (by TM), make sure
                       that no requests are handled. */
                    inst->state = CSR_FP_DEACTIVATING_ST;

                    /* Destroy all state relating to existing pipes */
                    CsrFpContainerDestroyAll(inst);

                    /* Restore all saved messages before responding. These will
                       subsequently be discarded before the second
                       CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND can arrive. */
                    CsrMessageQueueRestore(&inst->queue, CSR_FP_IFACEQUEUE);
                    CsrTmBluecoreDeactivateTransportResSend(CSR_FP_IFACEQUEUE);
                }
                else
                {
                    inst->state = CSR_FP_INIT_ST;
                }
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, *primType);
            }
            break;
        }
#endif
        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, *primType);
            break;
        }
    }

    CsrPmemFree(inst->msg);
}

/************************************************************/
/* CsrFpInit                                                */
/************************************************************/
void CsrFpInit(void **gash)
{
    CsrFpInstType *inst = *gash = CsrPmemAlloc(sizeof(CsrFpInstType));
    CsrUint32 i;

    inst->state = CSR_FP_INIT_ST;
    inst->last = 0;
    inst->queue = NULL;

    for (i = 0; i <= CSR_FP_NUM_MAX; i++)
    {
        inst->list[i] = NULL;
    }

#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_FP_IFACEQUEUE);
#else
    CsrFpActivate(inst);
#endif

    CSR_LOG_TEXT_REGISTER(&CsrFpLto, "FP", 0, NULL);
}

#ifdef ENABLE_SHUTDOWN
/******************************************************************************/
/* CsrDeinit                                                                  */
/*   This function is called by the scheduler to perform a graceful shutdown  */
/*   of a scheduler task.                                                     */
/*   This function must:                                                      */
/*   1)    empty the input message queue and free any allocated memory in the */
/*         messages.                                                          */
/*   2)    free any instance data that may be allocated.                      */
/*                                                                            */
/******************************************************************************/
void CsrFpDeinit(void **gash)
{
    CsrFpInstType *inst = *gash;
    CsrUint16 event;
    void *message;

    while (CsrSchedMessageGet(&event, &message) ||
           CsrMessageQueuePop(&inst->queue, &event, &message))
    {
        switch (event)
        {
            case CSR_FP_PRIM:
                CsrFpFreeDownstreamMessageContents(event, message);
                break;
            case CSR_BCCMD_PRIM:
                CsrBccmdFreeUpstreamMessageContents(event, message);
                break;
            case CSR_HCI_PRIM:
                CsrHciFreeUpstreamMessageContents(event, message);
                break;
#ifdef CSR_BLUECORE_ONOFF
            case CSR_TM_BLUECORE_PRIM:
                /* Free function not available (no contents in any received messages) */
                break;
#endif
            default:
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFpLto, 0, event, 0);
                break;
        }
        CsrPmemFree(message);
    }

    CsrFpContainerDestroyAll(inst);
    CsrPmemFree(inst);
}

#endif
