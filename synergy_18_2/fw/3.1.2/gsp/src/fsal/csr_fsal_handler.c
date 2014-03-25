/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_fsal_prim.h"
#include "csr_fsal_lib.h"
#include "csr_fsal_handler.h"
#include "csr_fsal_sef.h"
#include "csr_fsal_sef_utils.h"
#include "csr_fsal_utils.h"
#include "csr_pmem.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrFsalLto);

typedef void (*CsrFsalStateHandlerType)(CsrFsalInstanceData *instanceData);

static const CsrFsalStateHandlerType csrFsalStateHandlers[CSR_FSAL_NUMBER_OF_STATES][CSR_FSAL_PRIM_DOWNSTREAM_COUNT] =
{
    /* INITIALIZED_STATE         */
    {
        CsrFsalSessionCreateReqHandler,                      /* CSR_FSAL_SESSION_CREATE_REQ     */
        CsrFsalSessionDestroyReqHandler,                     /* CSR_FSAL_SESSION_DESTROY_REQ    */
        CsrFsalFileOpenReqHandler,                           /* CSR_FSAL_FILE_OPEN_REQ          */
        CsrFsalFileCloseReqHandler,                          /* CSR_FSAL_FILE_CLOSE_REQ         */
        CsrFsalFileSeekReqHandler,                           /* CSR_FSAL_FILE_SEEK_REQ          */
        CsrFsalFileReadReqHandler,                           /* CSR_FSAL_FILE_READ_REQ          */
        CsrFsalFileWriteReqHandler,                          /* CSR_FSAL_FILE_WRITE_REQ         */
        CsrFsalFileTellReqHandler,                           /* CSR_FSAL_FILE_TELL_REQ          */
        CsrFsalStatReqHandler,                               /* CSR_FSAL_STAT_REQ               */
        CsrFsalRemoveReqHandler,                             /* CSR_FSAL_REMOVE_REQ             */
        CsrFsalRenameReqHandler,                             /* CSR_FSAL_RENAME_REQ             */
        CsrFsalPermissionsSetReqHandler,                     /* CSR_FSAL_PERMISSIONS_SET_REQ    */
        CsrFsalDirMakeReqHandler,                            /* CSR_FSAL_DIR_MAKE_REQ           */
        CsrFsalDirChangeReqHandler,                          /* CSR_FSAL_DIR_CHANGE_REQ         */
        CsrFsalDirFindFirstReqHandler,                       /* CSR_FSAL_DIR_FIND_FIRST_REQ     */
        CsrFsalDirFindNextReqHandler,                        /* CSR_FSAL_DIR_FIND_NEXT_REQ      */
        CsrFsalDirFindCloseReqHandler,                       /* CSR_FSAL_DIR_FIND_CLOSE_REQ     */
        CsrFsalRemoveRecursivelyReqHandler                   /* CSR_FSAL_REMOVE_RECURSIVELY_REQ */
    }
};

/* ---------- End of jump tables ---------- */


void CsrFsalInit(void **gash)
{
    CsrFsalInstanceData *pInst;

    *gash = (CsrFsalInstanceData *) CsrPmemAlloc(sizeof(CsrFsalInstanceData));
    pInst = *gash;
    pInst->recvMsgP = NULL;
    pInst->state = CSR_FSAL_STATE_INITIALIZED;
    pInst->handleList = NULL;

    CSR_LOG_TEXT_REGISTER(&CsrFsalLto, "FSAL", 0, NULL);
}

void CsrFsalDeinit(void **gash)
{
    CsrFsalInstanceData *pInst = *gash;
    CsrUint16 eventClass;

    while (CsrSchedMessageGet(&eventClass, &pInst->recvMsgP))
    {
        CsrFsalFreeDownstreamMessageContents(CSR_FSAL_PRIM, pInst->recvMsgP);
        CsrPmemFree(pInst->recvMsgP);
    }

    while (pInst->handleList)
    {
        CsrFsalClearHandleIndexFromSessionId(pInst, pInst->handleList->sessionId);
    }

    CsrPmemFree(pInst->handleList);
    CsrPmemFree(pInst);
}

void CsrFsalHandler(void **gash)
{
    CsrFsalInstanceData *pInst = *gash;
    CsrUint16 eventClass;
    CsrFsalPrim *type;

    CsrSchedMessageGet(&eventClass, &pInst->recvMsgP);

    type = pInst->recvMsgP;

    switch (eventClass)
    {
        case CSR_FSAL_PRIM:
        {
            if ((*type < CSR_FSAL_PRIM_DOWNSTREAM_COUNT) && (csrFsalStateHandlers[pInst->state][*type] != NULL))
            {
                csrFsalStateHandlers[pInst->state][*type](pInst);

                CsrFsalFreeDownstreamMessageContents(CSR_FSAL_PRIM, pInst->recvMsgP);
            }
            else
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFsalLto, 0, eventClass, *type);
            }
            break;
        }

        default:
        {
            CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrFsalLto, 0, eventClass, *type);
            break;
        }
    }

    if (pInst->recvMsgP)
    {
        CsrPmemFree(pInst->recvMsgP);
    }
}
