/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_macro.h"
#include "csr_util.h"
#include "csr_log_private.h"
#include "csr_log_formats.h"
#include "csr_log_text_2.h"
#include "csr_framework_ext.h"

#ifdef CSR_LOG_ENABLE
static CsrBool csrLogTextIsFiltered(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrLogLevelText warningLevel)
{
    CsrBool ret = FALSE;

    if (CsrLogCtx.logLevelFilterText.tasks)
    {
        CsrBool taskFound = FALSE;
        CsrLogLevelFilterTextTask *task = CsrLogCtx.logLevelFilterText.tasks;
        while (task && !taskFound)
        {
            if (task->taskId == taskId)
            {
                taskFound = TRUE;
                if (task->subOrigins)
                {
                    CsrBool subOriginFound = FALSE;
                    CsrLogLevelFilterTextTaskSubOrigin *subOriginPtr = task->subOrigins;
                    while (subOriginPtr && !subOriginFound)
                    {
                        if (subOriginPtr->subOrigin == subOrigin)
                        {
                            subOriginFound = TRUE;
                            if (!(subOriginPtr->warningLevel & warningLevel))
                            {
                                ret = TRUE;
                            }
                        }
                        else
                        {
                            subOriginPtr = subOriginPtr->next;
                        }
                    }

                    if (!subOriginFound) /* No special filter has been applied to this subOrigin so just match it agains the overall warningLevel for this taskId*/
                    {
                        if (!(task->taskWarningLevel & warningLevel))
                        {
                            ret = TRUE;
                        }
                    }
                }
                else /* No special filter has been applied to this taskId's subOrigin so just match it agains the overall warningLevel for the taskId */
                {
                    if (!(task->taskWarningLevel & warningLevel))
                    {
                        ret = TRUE;
                    }
                }
            }
            else
            {
                task = task->next;
            }
        }

        if (!taskFound) /* No special filter has been applied to this taskId so just match it agains the overall warningLevel */
        {
            if (!(CsrLogCtx.logLevelFilterText.allTaskWarningLevel & warningLevel))
            {
                ret = TRUE;
            }
        }
    }
    else /* No special filter has been applied to this taskId so just match it agains the overall warningLevel */
    {
        if (!(CsrLogCtx.logLevelFilterText.allTaskWarningLevel & warningLevel))
        {
            ret = TRUE;
        }
    }

    return ret;
}

static CsrBool csrLogTextIsFiltered2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrLogLevelText logLevelText)
{
    if ((subOrigin > 0) && (handle->subOriginsLogLevel != NULL))
    {
        return !(handle->subOriginsLogLevel[subOrigin - 1] & logLevelText);
    }
    else
    {
        return !(handle->logLevel & logLevelText);
    }
}

#define CSR_LOG_TEXT_WRITE(taskId, subOrigin, warningLevel, formatString)       \
    {                                                                           \
        if (!csrLogTextIsFiltered(taskId, subOrigin, warningLevel))             \
        {                                                                       \
            CsrLog *p = CsrLogCtx.logList;                                      \
            while (p != NULL)                                                   \
            {                                                                   \
                if (p->ltextprint != NULL)                                      \
                {                                                               \
                    va_list args;                                               \
                    va_start(args, formatString);                               \
                    p->ltextprint(p->ltHdl, taskId, subOrigin, warningLevel,    \
                        formatString, args);                                    \
                    va_end(args);                                               \
                }                                                               \
                p = p->next;                                                    \
            }                                                                   \
        }                                                                       \
    }

#define CSR_LOG_TEXT_WRITE2(handle, subOrigin, logLevelText, formatString)              \
    {                                                                                   \
        if (!csrLogTextIsFiltered2(handle, subOrigin, logLevelText))                    \
        {                                                                               \
            CsrLog *p = CsrLogCtx.logList;                                              \
            const CsrCharString *subOriginName = NULL;                                  \
            if (subOrigin > 0)                                                          \
            {                                                                           \
                subOriginName = handle->subOriginsLegacy[subOrigin - 1].subOriginName;  \
            }                                                                           \
            while (p != NULL)                                                           \
            {                                                                           \
                if (p->ltextprint2 != NULL)                                             \
                {                                                                       \
                    va_list args;                                                       \
                    va_start(args, formatString);                                       \
                    p->ltextprint2(p->ltHdl, handle->taskId, subOrigin,                 \
                        handle->originName, subOriginName,                              \
                        logLevelText, formatString, args);                              \
                    va_end(args);                                                       \
                }                                                                       \
                else if (p->ltextprint != NULL)                                         \
                {                                                                       \
                    va_list args;                                                       \
                    va_start(args, formatString);                                       \
                    p->ltextprint(p->ltHdl, handle->taskId, subOrigin,                  \
                        logLevelText, formatString, args);                              \
                    va_end(args);                                                       \
                }                                                                       \
                p = p->next;                                                            \
            }                                                                           \
        }                                                                               \
    }

#define CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, warningLevel, bufferLength, buffer, formatString)  \
    {                                                                                                   \
        if (!csrLogTextIsFiltered(taskId, subOrigin, warningLevel))                                     \
        {                                                                                               \
            CsrLog *p = CsrLogCtx.logList;                                                              \
            while (p != NULL)                                                                           \
            {                                                                                           \
                if ((p != NULL) && (p->ltextbufprint != NULL))                                          \
                {                                                                                       \
                    va_list args;                                                                       \
                    va_start(args, formatString);                                                       \
                    p->ltextbufprint(p->ltHdl, taskId, subOrigin, warningLevel,                         \
                        bufferLength, buffer, formatString, args);                                      \
                    va_end(args);                                                                       \
                }                                                                                       \
                p = p->next;                                                                            \
            }                                                                                           \
        }                                                                                               \
    }

#define CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, logLevelText, bufferLength, buffer, formatString) \
    {                                                                                                   \
        if (!csrLogTextIsFiltered2(handle, subOrigin, logLevelText))                                    \
        {                                                                                               \
            CsrLog *p = CsrLogCtx.logList;                                                              \
            const CsrCharString *subOriginName = NULL;                                                  \
            if (subOrigin > 0)                                                                          \
            {                                                                                           \
                subOriginName = handle->subOriginsLegacy[subOrigin - 1].subOriginName;                  \
            }                                                                                           \
            while (p != NULL)                                                                           \
            {                                                                                           \
                if (p->ltextbufprint2 != NULL)                                                          \
                {                                                                                       \
                    va_list args;                                                                       \
                    va_start(args, formatString);                                                       \
                    p->ltextbufprint2(p->ltHdl, handle->taskId, subOrigin,                              \
                        handle->originName, subOriginName,                                              \
                        logLevelText, bufferLength, buffer, formatString, args);                        \
                    va_end(args);                                                                       \
                }                                                                                       \
                else if (p->ltextbufprint != NULL)                                                      \
                {                                                                                       \
                    va_list args;                                                                       \
                    va_start(args, formatString);                                                       \
                    p->ltextbufprint(p->ltHdl, handle->taskId, subOrigin,                               \
                        logLevelText, bufferLength, buffer, formatString, args);                        \
                    va_end(args);                                                                       \
                }                                                                                       \
                p = p->next;                                                                            \
            }                                                                                           \
        }                                                                                               \
    }

/*---------------------------------*/
/*  Log Text logging               */
/*---------------------------------*/
void CsrLogTextRegister(CsrLogTextTaskId taskId, const CsrCharString *taskName, CsrUint16 subOriginsLength, const CsrLogSubOrigin *subOrigins)
{
    CsrLog *p = CsrLogCtx.logList;
    while (p != NULL)
    {
        if (p->ltextregister != NULL)
        {
            p->ltextregister(p->ltHdl, taskId, taskName, subOriginsLength, subOrigins);
        }
        p = p->next;
    }
}

void CsrLogTextRegister2(CsrLogTextHandle **handle,
    const CsrCharString *originName,
    CsrUint16 subOriginsCount,
    const CsrCharString *subOrigins[])
{
    CsrBool exists = FALSE;
    CsrUint16 i = 0;
    CsrLogTextTaskId taskId;
    CsrLog *p = CsrLogCtx.logList;

    CsrGlobalMutexLock();

    /* Assign a taskId */
    taskId = CsrLogCtx.dynamicTaskId;
    CsrLogCtx.dynamicTaskId -= 1 << 16;

    /* Check if a matching handle already exists (if filtering has been
       applied before registration) */
    for (*handle = CsrLogCtx.logTextHandles;
         *handle != NULL;
         *handle = (*handle)->next)
    {
        if (((*handle)->taskId == taskId) ||
            (((*handle)->originName != NULL) && (CsrStrCmp((*handle)->originName, originName) == 0)))
        {
            exists = TRUE;
            break;
        }
    }

    if (!exists)
    {
        /* Create a handle if it doesn't exist */
        *handle = CsrPmemAlloc(sizeof(CsrLogTextHandle));
        (*handle)->logLevel = CsrLogCtx.logLevelFilterText.allTaskWarningLevel;
        (*handle)->subOriginsLogLevel = NULL;
        (*handle)->subOriginsLogLevelPending = NULL;
    }
    else if (((*handle)->subOriginsLogLevel != NULL) &&
             ((*handle)->subOriginsCount != subOriginsCount))
    {
        /* Integrate subOriginLogLevel information if any exists */
        CsrLogLevelText *subOriginsLogLevel = CsrPmemAlloc(sizeof(CsrLogLevelText) * subOriginsCount);
        for (i = 0; i < subOriginsCount; ++i)
        {
            subOriginsLogLevel[i] = (*handle)->logLevel;
        }
        CsrMemCpy(subOriginsLogLevel,
            (*handle)->subOriginsLogLevel,
            CSRMIN((*handle)->subOriginsCount, subOriginsCount) * sizeof(CsrLogLevelText));
        CsrPmemFree((*handle)->subOriginsLogLevel);
        (*handle)->subOriginsLogLevel = subOriginsLogLevel;
    }

    /* Convert sub origins to legacy array format */
    if (subOriginsCount > 0)
    {
        (*handle)->subOriginsLegacy = CsrPmemAlloc(sizeof(CsrLogSubOrigin) * subOriginsCount);
        for (i = 0; i < subOriginsCount; ++i)
        {
            (*handle)->subOriginsLegacy[i].subOriginNumber = i + 1;
            (*handle)->subOriginsLegacy[i].subOriginName = subOrigins[i];
        }
    }
    else
    {
        (*handle)->subOriginsLegacy = NULL;
    }
    (*handle)->subOriginsCount = subOriginsCount;
    (*handle)->self = handle;
    (*handle)->originName = originName;
    (*handle)->taskId = taskId;

    /* Process pending log level commands on sub origin */
    while ((*handle)->subOriginsLogLevelPending != NULL)
    {
        CsrLogLevelTextPending *logLevelTextPending = (*handle)->subOriginsLogLevelPending;
        CsrUint16 subOrigin = 0;

        for (i = 0; i < (*handle)->subOriginsCount; ++i)
        {
            if (CsrStrCmp((*handle)->subOriginsLegacy[i].subOriginName, logLevelTextPending->name) == 0)
            {
                subOrigin = (*handle)->subOriginsLegacy[i].subOriginNumber;
                break;
            }
        }
        if (subOrigin > 0)
        {
            if ((*handle)->subOriginsLogLevel == NULL)
            {
                (*handle)->subOriginsLogLevel = CsrPmemAlloc(sizeof(CsrLogLevelText) * (*handle)->subOriginsCount);
                for (i = 0; i < (*handle)->subOriginsCount; ++i)
                {
                    (*handle)->subOriginsLogLevel[i] = (*handle)->logLevel;
                }
            }
            (*handle)->subOriginsLogLevel[subOrigin - 1] = logLevelTextPending->logLevel;
        }
        (*handle)->subOriginsLogLevelPending = logLevelTextPending->next;
        CsrPmemFree(logLevelTextPending);
    }

    /* Insert in list */
    if (!exists)
    {
        (*handle)->next = CsrLogCtx.logTextHandles;
        CsrLogCtx.logTextHandles = *handle;
    }

    CsrGlobalMutexUnlock();

    while (p != NULL)
    {
        if (p->ltextregister2 != NULL)
        {
            p->ltextregister2(p->ltHdl, (*handle)->taskId,
                originName, subOriginsCount, subOrigins);
        }
        else if (p->ltextregister != NULL)
        {
            p->ltextregister(p->ltHdl, (*handle)->taskId,
                originName, subOriginsCount, (*handle)->subOriginsLegacy);
        }
        p = p->next;
    }
}

void CsrLogTextCritical(CsrLogTextTaskId taskId, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_CRITICAL, formatString);
}

void CsrLogTextCritical2(CsrLogTextHandle *handle, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_CRITICAL, formatString);
}

void CsrLogTextBufferCritical(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_CRITICAL, bufferLength, buffer, formatString);
}

void CsrLogTextBufferCritical2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_CRITICAL, bufferLength, buffer, formatString);
}

void CsrLogTextError(CsrLogTextTaskId taskId, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_ERROR, formatString);
}

void CsrLogTextError2(CsrLogTextHandle *handle, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_ERROR, formatString);
}

void CsrLogTextBufferError(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_ERROR, bufferLength, buffer, formatString);
}

void CsrLogTextBufferError2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_ERROR, bufferLength, buffer, formatString);
}

void CsrLogTextWarning(CsrLogTextTaskId taskId, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_WARNING, formatString);
}

void CsrLogTextWarning2(CsrLogTextHandle *handle, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_WARNING, formatString);
}

void CsrLogTextBufferWarning(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_WARNING, bufferLength, buffer, formatString);
}

void CsrLogTextBufferWarning2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_WARNING, bufferLength, buffer, formatString);
}

void CsrLogTextInfo(CsrLogTextTaskId taskId, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_INFO, formatString);
}

void CsrLogTextInfo2(CsrLogTextHandle *handle, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_INFO, formatString);
}

void CsrLogTextBufferInfo(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_INFO, bufferLength, buffer, formatString);
}

void CsrLogTextBufferInfo2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_INFO, bufferLength, buffer, formatString);
}

void CsrLogTextDebug(CsrLogTextTaskId taskId, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_DEBUG, formatString);
}

void CsrLogTextDebug2(CsrLogTextHandle *handle, CsrUint16 subOrigin, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_DEBUG, formatString);
}

void CsrLogTextBufferDebug(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE(taskId, subOrigin, CSR_LOG_LEVEL_TEXT_DEBUG, bufferLength, buffer, formatString);
}

void CsrLogTextBufferDebug2(CsrLogTextHandle *handle, CsrUint16 subOrigin, CsrSize bufferLength, const void *buffer, const CsrCharString *formatString, ...)
{
    CSR_LOG_TEXT_BUFFER_WRITE2(handle, subOrigin, CSR_LOG_LEVEL_TEXT_DEBUG, bufferLength, buffer, formatString);
}

#endif /* CSR_LOG_ENABLE */
