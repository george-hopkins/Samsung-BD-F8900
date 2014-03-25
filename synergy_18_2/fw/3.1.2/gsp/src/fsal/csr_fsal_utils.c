/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_fsal_handler.h"
#include "csr_fsal_prim.h"
#include "csr_fsal_task.h"

#include "csr_pmem.h"
#include "csr_log_text_2.h"
#include "csr_panic.h"

CsrFsalFileHandles *CsrFsalFileHandleInit(CsrFsalFileHandles **handles, CsrFileHandle *fileHandle)
{
    CsrFsalFileHandles *prev, *next = *handles;
    prev = *handles;
    while (next)
    {
        prev = next;
        if (prev->fileHandle == fileHandle)
        {
            CSR_LOG_TEXT_WARNING((CsrFsalLto, 0,
                                  "File handle is already in the list!!! This should never happen"));
            return prev;
        }
        next = next->next;
    }

    /* fileHandle was not found in the list (and nor should it be!!!) initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrFsalFileHandles));
    next->fileHandle = fileHandle;

    if (prev == NULL) /* *handles was NULL so this is the first entry */
    {
        *handles = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

CsrFsalDirHandles *CsrFsalDirHandleInit(CsrFsalDirHandles **handles, CsrDirHandle *dirHandle)
{
    CsrFsalDirHandles *prev, *next = *handles;
    prev = *handles;
    while (next)
    {
        prev = next;
        if (prev->dirHandle == dirHandle)
        {
            CSR_LOG_TEXT_WARNING((CsrFsalLto, 0,
                                  "Dir handle is already in the list!!! This should never happen"));
            return prev;
        }
        next = next->next;
    }

    /* dirHandle was not found in the list (and nor should it be!!!) initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrFsalDirHandles));
    next->dirHandle = dirHandle;

    if (prev == NULL) /* *handles was NULL so this is the first entry */
    {
        *handles = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

static CsrFsalHandles *csrFsalHandleInit(CsrFsalHandles **handles, CsrSchedQid appHandle)
{
    CsrFsalHandles *prev, *next = *handles;
    CsrFsalSession sessionId = (appHandle << 16);
    prev = *handles;
    while (next)
    {
        prev = next;
        if (prev->appHandle == appHandle) /* taskId was already in the list so we increase the sessionId because same task can have multiple sessions, when we do it this way each task can actually have 0xFFFF sessions which should be more than enough */
        {
            sessionId++;
        }
        next = next->next;
    }

    /* taskId was not found in the list so initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrFsalHandles));
    next->appHandle = appHandle;
    next->sessionId = sessionId;
    next->currentWorkingDir = NULL;
    next->fileHandles = NULL;
    next->dirHandles = NULL;

    if (prev == NULL) /* *handles was NULL so this is the first entry */
    {
        *handles = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

static CsrFsalHandles *csrFsalHandleGetEntryFromSessionId(CsrFsalHandles **handles, CsrFsalSession sessionId)
{
    CsrFsalHandles *prev, *next = *handles;
    prev = *handles;
    while (next)
    {
        prev = next;
        if (prev->sessionId == sessionId)
        {
            return prev;
        }
        next = next->next;
    }

    return NULL;
}

static CsrFsalFileHandles *csrFsalGetFileHandleEntryFromFileHandle(CsrFsalFileHandles **handles, CsrFileHandle *fileHandle)
{
    CsrFsalFileHandles *next = *handles;
    while (next)
    {
        if (next->fileHandle == fileHandle)
        {
            return next;
        }
        next = next->next;
    }

    return NULL;
}

static CsrFsalDirHandles *csrFsalGetDirHandleEntryFromDirHandle(CsrFsalDirHandles **handles, CsrDirHandle *dirHandle)
{
    CsrFsalDirHandles *next = *handles;
    while (next)
    {
        if (next->dirHandle == dirHandle)
        {
            return next;
        }
        next = next->next;
    }

    return NULL;
}

static void csrFsalFileHandleListClean(CsrFsalFileHandles **handles)
{
    CsrFsalFileHandles *prev, *next = *handles;

    while (next)
    {
        prev = next;
        next = prev->next;
        /* Do we need to close handles here???? */
        CsrPmemFree(prev);
    }
    *handles = NULL;
}

static void csrFsalDirHandleListClean(CsrFsalDirHandles **handles)
{
    CsrFsalDirHandles *prev, *next = *handles;

    while (next)
    {
        prev = next;
        next = prev->next;
        /* Do we need to close handles here???? */
        CsrPmemFree(prev);
    }
    *handles = NULL;
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromSessionId
*
*   Description:
*       The function finds the apphandle which corresponds to the fileHandle and the
*       operationType.
*
*   Input:
*       pInst:          Local instance data.
*       sessionId:      The sessionId which is the input to the look-up to find
*                       the corresponding apphandle.
*
*   Output:
*       The appHandle to be used to send the received answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromSessionId(CsrFsalHandles **handles, CsrFsalSession sessionId)
{
    CsrFsalHandles *next = *handles;
    while (next)
    {
        if (next->sessionId == sessionId)
        {
            return next->appHandle;
        }
        next = next->next;
    }

    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromFileHandle
*
*   Description:
*       The function finds the apphandle which corresponds to the fileHandle and the
*       operationType.
*
*   Input:
*       pInst:          Local instance data.
*       fileHandle:       The fileHandle which is the input to the look-up to find
*                       the corresponding apphandle.
*
*   Output:
*       The appHandle to be used to send the received answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromFileHandle(CsrFsalInstanceData *pInst,
    CsrFileHandle *fileHandle)
{
    CsrFsalHandles *next = pInst->handleList;

    while (next)
    {
        if (csrFsalGetFileHandleEntryFromFileHandle(&next->fileHandles, fileHandle) != NULL)
        {
            return next->appHandle;
        }
        next = next->next;
    }

    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromDirHandle
*
*   Description:
*       The function finds the apphandle which corresponds to the dirHandle and the
*       operationType.
*
*   Input:
*       pInst:          Local instance data.
*       dirHandle:       The dirHandle which is the input to the look-up to find
*                       the corresponding apphandle.
*
*   Output:
*       The appHandle to be used to send the received answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromDirHandle(CsrFsalInstanceData *pInst,
    CsrDirHandle *dirHandle)
{
    CsrFsalHandles *next = pInst->handleList;

    while (next)
    {
        if (csrFsalGetDirHandleEntryFromDirHandle(&next->dirHandles, dirHandle) != NULL)
        {
            return next->appHandle;
        }
        next = next->next;
    }

    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromSessionIdAndStoreDirHandle
*
*   Description:
*       The function finds the apphandle which corresponds to the sessionId
*       and it stores the fileHandle as well.
*
*   Input:
*       pInst:          Local instance data.
*       sessionId:      The sessionId which is globally unique and given by the
*                       CsrFsalReserveNewSession function to identify what the
*                       response is an answer to.
*       fileHandle:     The fileHandle to store and use for communication with the
*                       external file system.
*
*   Output:
*       The appHandle to be used to send the received answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromSessionIdAndStoreDirHandle(CsrFsalInstanceData *pInst,
    CsrFsalSession sessionId,
    CsrDirHandle *dirHandle)
{
    CsrFsalHandles *entry = csrFsalHandleGetEntryFromSessionId(&pInst->handleList, sessionId);

    if (entry)
    {
        CsrFsalDirHandleInit(&entry->dirHandles, dirHandle);

        return entry->appHandle;
    }
    else
    {
        return CSR_SCHED_QID_INVALID;
    }
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromSessionIdAndStoreFileHandle
*
*   Description:
*       The function finds the apphandle which corresponds to the sessionId
*       and it stores the fileHandle as well.
*
*   Input:
*       pInst:          Local instance data.
*       sessionId:      The sessionId which is globally unique and given by the
*                       CsrFsalReserveNewSession function to identify what the
*                       response is an answer to.
*       fileHandle:     The fileHandle to store and use for communication with the
*                       external file system.
*
*   Output:
*       The appHandle to be used to send the received answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromSessionIdAndStoreFileHandle(CsrFsalInstanceData *pInst,
    CsrFsalSession sessionId,
    CsrFileHandle *fileHandle)
{
    CsrFsalHandles *entry = csrFsalHandleGetEntryFromSessionId(&pInst->handleList, sessionId);

    if (entry)
    {
        CsrFsalFileHandleInit(&entry->fileHandles, fileHandle);

        return entry->appHandle;
    }
    else
    {
        return CSR_SCHED_QID_INVALID;
    }
}

/********************************************************************************
*
*   Name:
*       CsrFsalClearHandleIndexFromSeqNum
*
*   Description:
*       The function resets a specific element in the handle list given from the seqNum
*
*   Input:
*       pInst:          Local instance data.
*       sessionId:      The sessionId of the entry to reset
*
*   Output:
*       The appHandle to be used to send the answer to.
*
*********************************************************************************/
CsrSchedQid CsrFsalClearHandleIndexFromSessionId(CsrFsalInstanceData *pInst, CsrFsalSession sessionId)
{
    CsrFsalHandles *prev, *next = pInst->handleList;

    prev = pInst->handleList;

    while (next)
    {
        if (next->sessionId == sessionId)
        {
            CsrSchedQid appHandle;
            if (next == pInst->handleList)
            {
                pInst->handleList = next->next;
            }
            else
            {
                prev->next = next->next;
            }

            csrFsalFileHandleListClean(&next->fileHandles);
            csrFsalDirHandleListClean(&next->dirHandles);

            CsrPmemFree(next->currentWorkingDir);

            appHandle = next->appHandle;
            CsrPmemFree(next);

            return appHandle;
        }
        prev = next;
        next = prev->next;
    }

    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalClearFileHandleIndex
*
*   Description:
*       The function resets a specific element in the handle list given from the
*       fsHandle and the operationType.
*
*   Input:
*       pInst:          Local instance data.
*       fileHandle:     The fileHandle which entry to reset
*
*   Output:
*
*********************************************************************************/
void CsrFsalClearFileHandleIndex(CsrFsalFileHandles **handles, CsrFileHandle *fileHandle)
{
    CsrFsalFileHandles *prev, *next = *handles;

    prev = *handles;

    while (next)
    {
        if (next->fileHandle == fileHandle)
        {
            if (prev == *handles)
            {
                *handles = next->next;
            }
            else
            {
                prev->next = next->next;
            }

            CsrPmemFree(next);
            break;
        }
        prev = next;
        next = prev->next;
    }
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromFileHandleAndClearFileHandleIndex
*
*   Description:
*       The function resets a specific element in the fileHandles list given from the
*       fileHandle.
*
*   Input:
*       pInst:          Local instance data.
*       fileHandle:     The fileHandle which entry to reset
*
*   Output:
*       appHandle:      The appHandle to answer back to
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromFileHandleAndClearFileHandleIndex(CsrFsalHandles **handles, CsrFileHandle *fileHandle)
{
    CsrFsalHandles *nnext = *handles;

    while (nnext)
    {
        CsrFsalFileHandles *prev, *next = nnext->fileHandles;

        prev = nnext->fileHandles;

        while (next)
        {
            if (next->fileHandle == fileHandle)
            {
                if (prev == nnext->fileHandles)
                {
                    nnext->fileHandles = next->next;
                }
                else
                {
                    prev->next = next->next;
                }

                CsrPmemFree(next);
                return nnext->appHandle;
            }
            prev = next;
            next = prev->next;
        }
        nnext = nnext->next;
    }
    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalGetAppHandleFromDirHandleAndClearDirHandleIndex
*
*   Description:
*       The function resets a specific element in the dirHandles list given from the
*       dirHandle.
*
*   Input:
*       pInst:          Local instance data.
*       dirHandle:      The dirHandle which entry to reset
*
*   Output:
*       appHandle:      The appHandle to answer back to
*
*********************************************************************************/
CsrSchedQid CsrFsalGetAppHandleFromDirHandleAndClearDirHandleIndex(CsrFsalHandles **handles, CsrDirHandle *dirHandle)
{
    CsrFsalHandles *nnext = *handles;

    while (nnext)
    {
        CsrFsalDirHandles *prev, *next = nnext->dirHandles;

        prev = nnext->dirHandles;

        while (next)
        {
            if (next->dirHandle == dirHandle)
            {
                if (prev == nnext->dirHandles)
                {
                    nnext->dirHandles = next->next;
                }
                else
                {
                    prev->next = next->next;
                }

                CsrPmemFree(next);
                return nnext->appHandle;
            }
            prev = next;
            next = prev->next;
        }
        nnext = nnext->next;
    }
    return CSR_SCHED_QID_INVALID;
}

/********************************************************************************
*
*   Name:
*       CsrFsalReserveNewSessionId
*
*   Description:
*       The function creates a unique sessionId for the external file system and
*       it stores this as well as the app handle.
*
*   Input:
*       pInst:          Local instance data.
*       appHandle:      The handle which shall receive all indication
*                       and confirm messages.
*
*   Output:
*       A unique sessionId number which is global unique and which shall be used as id
*       in any messages send to an external file system task.
*
*********************************************************************************/
CsrFsalHandles *CsrFsalReserveNewSessionId(CsrFsalInstanceData *pInst,
    CsrUint16 appHandle)
{
    CsrFsalHandles *newHandle = csrFsalHandleInit(&pInst->handleList, appHandle);

    if (CsrDirGetCurrentWorkingDir(&newHandle->currentWorkingDir) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((CsrFsalLto, 0,
                              "Failed to retrieve current working dir defaulting to root ie.'/'"));
        newHandle->currentWorkingDir = (CsrUtf8String *) CsrStrDup("/");
    }

    return newHandle;
}

/********************************************************************************
*
*   Name:
*       CsrFsalVerifySessionId
*
*   Description:
*       The function verifies that the current sessionId is valid and it changes the
*       working directory to here this session is currently operating.
*
*   Input:
*       pInst:          Local instance data.
*       sessionId:      The sessionId to verify
*
*   Output:
*       A unique sessionId number which is global unique and which shall be used as id
*       in any messages send to an external file system task.
*
*********************************************************************************/
CsrFsalHandles *CsrFsalGetSession(CsrFsalInstanceData *pInst, CsrFsalSession sessionId)
{
    CsrFsalHandles *entry = csrFsalHandleGetEntryFromSessionId(&pInst->handleList, sessionId);

    if (entry == NULL) /* We failed to extract the entry all we can do is to panic */
    {
        CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_EXCEPTION, "CsrFsalGetSession: failed to retrieve the session");
    }

    if (CsrDirChange(entry->currentWorkingDir) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_WARNING((CsrFsalLto, 0,
                              "Failed to change current working dir"));
    }

    return entry;
}
