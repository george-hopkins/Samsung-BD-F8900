/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_log_text_2.h"

/* DSPM Headers */
#include "csr_dspm_task.h"
#include "csr_dspm_prim.h"
#include "csr_dspm_lib.h"
#include "csr_dspm_handler.h"
#include "csr_dspm_sef.h"
#include "csr_dspm_util.h"

/* BCCMD Headers */
#include "csr_bccmd_task.h"
#include "csr_bccmd_prim.h"
#include "csr_bccmd_lib.h"

/* HQ Headers */
#include "csr_hq_task.h"
#include "csr_hq_prim.h"
#include "csr_hq_lib.h"

#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
/* FP Headers */
#include "csr_fp_task.h"
#include "csr_fp_prim.h"
#include "csr_fp_lib.h"
#endif

#ifdef CSR_BLUECORE_ONOFF
/* TM Headers */
#include "csr_tm_bluecore_task.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrDspmLto);

void CsrDspmInit(void **gash)
{
    CsrDspmInstanceData *instanceData = *gash = CsrPmemZalloc(sizeof(CsrDspmInstanceData));

    instanceData->users = NULL;
    instanceData->opids = NULL;
    instanceData->currentRequest = NULL;
    instanceData->saveQueue = NULL;

#ifdef CSR_BLUECORE_ONOFF
    CsrTmBlueCoreRegisterReqSend(CSR_DSPM_IFACEQUEUE);
#else
    instanceData->state = CSR_DSPM_STATE_ACTIVE;
#endif

    CsrDspmHqRegister();

    CSR_LOG_TEXT_REGISTER(&CsrDspmLto, "DSPM", 0, NULL);
}

void CsrDspmHandler(void **gash)
{
    CsrDspmInstanceData *instanceData = *gash;
    CsrUint16 event;
    void *message;

    if (CsrSchedMessageGet(&event, &message))
    {
        CsrPrim *type = message;

        switch (event)
        {
            case CSR_DSPM_PRIM:
            {
                CsrDspmRequestQueuePushHandle(instanceData, message);
                message = NULL;
                break;
            }
            case CSR_BCCMD_PRIM:
            {
                if (*type == CSR_BCCMD_CFM)
                {
                    CsrDspmBccmdCfmHandler(instanceData, message);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, event, *type);
                }
                break;
            }
            case CSR_HQ_PRIM:
            {
                if (*type == CSR_HQ_MSG_IND)
                {
                    CsrDspmHqMsgIndHandler(instanceData, message);
                }
                else if (*type == CSR_HQ_REGISTER_CFM)
                {
                    CsrHqRegisterCfm *confirm = message;
                    CSR_LOG_TEXT_CONDITIONAL_ERROR(confirm->result != CSR_RESULT_SUCCESS,
                        (CsrDspmLto, 0, "Failed to register HQ 0x%04X", confirm->varId))
                    CSR_UNUSED(confirm);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, event, *type);
                }
                break;
            }
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
            case CSR_FP_PRIM:
            {
                if (*type == CSR_FP_CREATE_CFM)
                {
                    CsrDspmFpCreateCfmHandler(instanceData, message);
                }
                else if (*type == CSR_FP_WRITE_CFM)
                {
                    CsrDspmFpWriteCfmHandler(instanceData, message);
                }
                else if (*type == CSR_FP_DESTROY_CFM)
                {
                    CsrDspmFpDestroyCfmHandler(instanceData, message);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, event, *type);
                }
                break;
            }
#endif
#ifdef CSR_BLUECORE_ONOFF
            case CSR_TM_BLUECORE_PRIM:
            {
                if (*type == CSR_TM_BLUECORE_REGISTER_CFM)
                {
                    /* No need to do anything */
                }
                else if (*type == CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND)
                {
                    CsrDspmTmBluecoreActivateTransportIndHandler(instanceData, message);
                }
                else if (*type == CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND)
                {
                    CsrDspmTmBluecoreDeactivateTransportIndHandler(instanceData, message);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, event, *type);
                }
                break;
            }
#endif
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrDspmLto, 0, event, 0);
                break;
            }
        }
    }

    /* Free the message itself */
    CsrPmemFree(message);
}

#ifdef ENABLE_SHUTDOWN
void CsrDspmDeinit(void **gash)
{
    CsrDspmInstanceData *instanceData = *gash;
    CsrUint16 event;
    void *message;

    /* Empty lists */
    CsrDspmInfoTupleRemoveAll(&instanceData->users);
    CsrDspmInfoTupleRemoveAll(&instanceData->opids);

    /* Empty the request queue */
    while (instanceData->currentRequest != NULL)
    {
        CsrDspmRequestQueuePop(instanceData);
    }

    /* Clean message */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_DSPM_PRIM:
                CsrDspmFreeDownstreamMessageContents(CSR_DSPM_PRIM, message);
                break;
            case CSR_BCCMD_PRIM:
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, message);
                break;
            case CSR_HQ_PRIM:
                CsrHqFreeUpstreamMessageContents(CSR_HQ_PRIM, message);
                break;
#ifdef CSR_DSPM_SUPPORT_CAPABILITY_DOWNLOAD
            case CSR_FP_PRIM:
                CsrFpFreeUpstreamMessageContents(CSR_FP_PRIM, message);
                break;
#endif
#ifdef CSR_BLUECORE_ONOFF
            case CSR_TM_BLUECORE_PRIM:
                /* Free function not available (no contents in any received messages) */
                break;
#endif
            default:
                break;
        }
        CsrPmemFree(message);
    }

    /* Free instance data */
    CsrPmemFree(instanceData);
}

#endif
