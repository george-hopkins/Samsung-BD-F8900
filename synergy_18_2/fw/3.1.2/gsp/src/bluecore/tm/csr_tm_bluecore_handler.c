/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_tm_bluecore_handler.h"
#include "csr_tm_bluecore_prim.h"
#include "csr_tm_bluecore_lib.h"
#include "csr_tm_bluecore_sef.h"
#include "csr_tm_bluecore_transport.h"
#include "csr_bccmd_lib.h"
#include "csr_hci_private_lib.h"
#include "csr_log_text_2.h"

void CsrTmBlueCoreHandler(void **gash)
{
    CsrTmBlueCoreInstanceData *tmBcInst = *gash;
    CsrUint16 event;

    CSR_LOG_TEXT_ASSERT(CsrTmBluecoreLto, 0, tmBcInst != NULL);

    CsrSchedMessageGet(&event, &tmBcInst->recvMsgP);

    switch (event)
    {
        case CSR_TM_BLUECORE_PRIM:
            CsrTmBlueCoreTmBlueCoreMsgHandler(tmBcInst);
            break;
        case CSR_BCCMD_PRIM:
            CsrTmBlueCoreBccmdMsgHandler(tmBcInst);
            break;
        case CSR_HCI_PRIM:
            CsrTmBlueCoreHciMsgHandler(tmBcInst);
            break;
        default:
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTmBluecoreLto, 0, event, 0);
            break;
    }

    CsrPmemFree(tmBcInst->recvMsgP);
    tmBcInst->recvMsgP = NULL;
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
void CsrTmBlueCoreDeinit(void **gash)
{
    CsrTmBlueCoreInstanceData *tmBcInst = *gash;
    CsrUint16 event;
    void *message;

#ifdef CSR_BLUECORE_ONOFF
    if (CSR_TM_BLUECORE_TRANSPORT_STARTED(tmBcInst->blueCoreTransportHandle))
    {
        CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
    }
    CsrCmnListDeinit(&tmBcInst->transportDelegates);
#else
    CsrTmBlueCoreTransportStop(tmBcInst->blueCoreTransportHandle);
    CsrCmnListDeinit(&tmBcInst->transportActivators);
#endif
#ifdef CSR_CHIP_MANAGER_ENABLE
    CsrCmnListDeinit(&tmBcInst->cmStatusSubscribers);
    CsrCmnListDeinit(&tmBcInst->cmReplayersRegistered);
    CsrCmnListDeinit(&tmBcInst->cmReplayersStarted);
#endif

#ifdef CSR_BLUECORE_ONOFF
    while (CsrSchedMessageGet(&event, &message) ||
           CsrMessageQueuePop(&tmBcInst->saveQueue, &event, &message))
#else
    while (CsrSchedMessageGet(&event, &message))
#endif
    {
        switch (event)
        {
            case CSR_BCCMD_PRIM:
            {
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, message);
                break;
            }
            case CSR_HCI_PRIVATE_PRIM:
            {
                CsrHciPrivateFreeUpstreamMessageContents(CSR_HCI_PRIVATE_PRIM, message);
                break;
            }
        }
        CsrPmemFree(message);
    }

    CsrPmemFree(tmBcInst);
}

#endif /* ENABLE_SHUTDOWN */
