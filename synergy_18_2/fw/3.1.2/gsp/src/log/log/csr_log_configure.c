/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_log_configure.h"
#include "csr_log_private.h"
#include "csr_log_formats.h"
#include "csr_log_gsp.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_msgconv.h"
#include "csr_macro.h"

#ifdef CSR_LOG_ENABLE

CsrLogDispatchCtx CsrLogCtx;

/*-------------------------------------------------------------------------*/
/*  Private helper functions for filtering on log text warning levels      */
/*-------------------------------------------------------------------------*/

static void csrLogLevelFilterTextTaskSubOriginsClean(CsrLogLevelFilterTextTaskSubOrigin **subOrigins)
{
    CsrLogLevelFilterTextTaskSubOrigin *prev, *next = *subOrigins;

    while (next)
    {
        prev = next;
        next = prev->next;
        CsrPmemFree(prev);
    }
    *subOrigins = NULL;
}

static void csrLogLevelFilterTextTaskSubOriginInit(CsrLogLevelFilterTextTaskSubOrigin **subOrigins, CsrSchedQid subOrigin, CsrLogLevelText warningLevel)
{
    CsrLogLevelFilterTextTaskSubOrigin *prev, *next = *subOrigins;
    prev = *subOrigins;
    while (next)
    {
        prev = next;
        if (prev->subOrigin == subOrigin) /* subOrigin was already in the list */
        {
            prev->warningLevel = warningLevel;
            return;
        }
        next = next->next;
    }

    /* subOrigin was not found in the list so initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrLogLevelFilterTextTaskSubOrigin));
    next->subOrigin = subOrigin;
    next->warningLevel = warningLevel;

    if (prev == NULL) /* *subOrigins was NULL so this is the first entry */
    {
        *subOrigins = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
}

static CsrLogLevelFilterTextTask *csrLogLevelFilterTextTaskInit(CsrLogLevelFilterTextTask **tasks, CsrLogTextTaskId taskId)
{
    CsrLogLevelFilterTextTask *prev, *next = *tasks;
    prev = *tasks;
    while (next)
    {
        prev = next;
        if (prev->taskId == taskId) /* taskId was already in the list */
        {
            return prev;
        }
        next = next->next;
    }

    /* taskId was not found in the list so initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrLogLevelFilterTextTask));
    next->taskId = taskId;
    next->taskWarningLevel = CsrLogCtx.logLevelFilterText.allTaskWarningLevel;

    if (prev == NULL) /* *tasks was NULL so this is the first entry */
    {
        *tasks = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

static void csrLogLevelFilterTextTasksClean(CsrLogLevelFilterTextTask **tasks)
{
    CsrLogLevelFilterTextTask *prev, *next = *tasks;

    while (next)
    {
        prev = next;
        csrLogLevelFilterTextTaskSubOriginsClean(&prev->subOrigins);
        next = prev->next;
        CsrPmemFree(prev);
    }
    *tasks = NULL;
}

static CsrLogLevelFilterTask *csrLogLevelFilterTaskTaskInit(CsrLogLevelFilterTask **tasks, CsrSchedQid taskId)
{
    CsrLogLevelFilterTask *prev, *next = *tasks;
    prev = *tasks;
    while (next)
    {
        prev = next;
        if (prev->taskId == taskId) /* taskId was already in the list */
        {
            return prev;
        }
        next = next->next;
    }

    /* taskId was not found in the list so initialize a new entry */
    next = CsrPmemZalloc(sizeof(CsrLogLevelFilterTask));
    next->taskId = taskId;
    next->taskLogLevel = CSR_LOG_LEVEL_TASK_OFF;

    if (prev == NULL) /* *tasks was NULL so this is the first entry */
    {
        *tasks = next;
    }
    else /* append it to the list */
    {
        prev->next = next;
    }
    return next;
}

static void csrLogLevelFilterTaskTasksClean(CsrLogLevelFilterTask **tasks)
{
    CsrLogLevelFilterTask *prev, *next = *tasks;

    while (next)
    {
        prev = next;
        next = prev->next;
        CsrPmemFree(prev);
    }
    *tasks = NULL;
}

static void csrLogTextHandlesClean(void)
{
    while (CsrLogCtx.logTextHandles != NULL)
    {
        CsrLogTextHandle *handle = CsrLogCtx.logTextHandles;
        CsrPmemFree(handle->subOriginsLogLevel);
        CsrPmemFree(handle->subOriginsLegacy);
        if (handle->self != NULL)
        {
            *(handle->self) = NULL;
        }
        while (handle->subOriginsLogLevelPending != NULL)
        {
            CsrLogLevelTextPending *logLevelTextPending = handle->subOriginsLogLevelPending;
            handle->subOriginsLogLevelPending = logLevelTextPending->next;
            CsrPmemFree(logLevelTextPending);
        }
        CsrLogCtx.logTextHandles = handle->next;
        CsrPmemFree(handle);
    }
}

/*---------------------------------*/
/* Log init/deinit  */
/*---------------------------------*/
void CsrLogInit(CsrUint8 size)
{
    /* Log Formats interface */
    CSR_UNUSED(size);
    CsrLogCtx.logList = NULL;

    /* Log Level Filter Environment */
    CsrLogCtx.logLevelFilterEnvironment = CSR_LOG_LEVEL_ENVIRONMENT_BCI_ACL |
                                          CSR_LOG_LEVEL_ENVIRONMENT_BCI_HCI |
                                          CSR_LOG_LEVEL_ENVIRONMENT_BCI_SCO |
                                          CSR_LOG_LEVEL_ENVIRONMENT_BCI_VENDOR |
                                          CSR_LOG_LEVEL_ENVIRONMENT_TRANSPORTS |
                                          CSR_LOG_LEVEL_ENVIRONMENT_BGINT_REG |
                                          CSR_LOG_LEVEL_ENVIRONMENT_BGINT_UNREG |
                                          CSR_LOG_LEVEL_ENVIRONMENT_PROTO |
                                          CSR_LOG_LEVEL_ENVIRONMENT_PROTO_LOC;


    /* Log Level Filter Tasks */
    CsrLogCtx.logLevelFilterTask.tasks = NULL;
    CsrLogCtx.logLevelFilterTask.allTaskLogLevel = CSR_LOG_LEVEL_TASK_TEXT |
                                                   CSR_LOG_LEVEL_TASK_TEXT_LOC |
                                                   CSR_LOG_LEVEL_TASK_STATE |
                                                   CSR_LOG_LEVEL_TASK_STATE_NAME |
                                                   CSR_LOG_LEVEL_TASK_STATE_LOC |
                                                   CSR_LOG_LEVEL_TASK_TASK_SWITCH |
                                                   CSR_LOG_LEVEL_TASK_MESSAGE_PUT |
                                                   CSR_LOG_LEVEL_TASK_MESSAGE_PUT_LOC |
                                                   CSR_LOG_LEVEL_TASK_MESSAGE_GET |
                                                   CSR_LOG_LEVEL_TASK_MESSAGE_QUEUE_PUSH |
                                                   CSR_LOG_LEVEL_TASK_MESSAGE_QUEUE_POP |
                                                   CSR_LOG_LEVEL_TASK_TIMER_IN |
                                                   CSR_LOG_LEVEL_TASK_TIMER_IN_LOC |
                                                   CSR_LOG_LEVEL_TASK_TIMER_CANCEL |
                                                   CSR_LOG_LEVEL_TASK_TIMER_CANCEL_LOC |
                                                   CSR_LOG_LEVEL_TASK_TIMER_FIRE |
                                                   CSR_LOG_LEVEL_TASK_TIMER_DONE;

    /* Log Level Filter Text */
    CsrLogCtx.logLevelFilterText.tasks = NULL;
    CsrLogCtx.logLevelFilterText.allTaskWarningLevel = CSR_LOG_LEVEL_TEXT_WARNING |
                                                       CSR_LOG_LEVEL_TEXT_ERROR |
                                                       CSR_LOG_LEVEL_TEXT_CRITICAL;

    CsrLogCtx.logTextHandles = NULL;
    CsrLogCtx.dynamicTaskId = (0xFFFFU << 16) | CSR_SCHED_QID_INVALID;
}

void CsrLogDeinit(void)
{
    CsrLogCtx.logList = NULL;

    csrLogLevelFilterTaskTasksClean(&CsrLogCtx.logLevelFilterTask.tasks);

    csrLogLevelFilterTextTasksClean(&CsrLogCtx.logLevelFilterText.tasks);

    csrLogTextHandlesClean();
}

/*---------------------------------*/
/*  Log initialization */
/*---------------------------------*/
CsrBool CsrLogFormatInstRegister(CsrLog *log)
{
    log->next = CsrLogCtx.logList;
    CsrLogCtx.logList = log;
    return TRUE;
}

void CsrLogUnregister(CsrLog *log)
{
    if (CsrLogCtx.logList == log)
    {
        CsrLogCtx.logList = CsrLogCtx.logList->next;
    }
    else
    {
        CsrLog *element = CsrLogCtx.logList;
        while ((element != NULL) && (element->next != log))
        {
            element = element->next;
        }
        if (element != NULL)
        {
            element->next = element->next->next;
        }
    }
}

/*----------------------------------------------------*/
/*  Filtering on environment specific log levels      */
/*----------------------------------------------------*/

/* Set the logging level for the environment outside the scheduler context */
void CsrLogLevelEnvironmentSet(CsrLogLevelEnvironment environmentLogLevel)
{
    CsrLogCtx.logLevelFilterEnvironment = environmentLogLevel;
}

/*----------------------------------------------------*/
/*  Filtering on task specific log levels             */
/*----------------------------------------------------*/

/* Set the logging level for all scheduler tasks */
/* This function call takes precedence over all previous calls to CsrLogLevelTaskSetSpecific() */
void CsrLogLevelTaskSetAll(CsrLogLevelTask tasksLogLevelMask)
{
    csrLogLevelFilterTaskTasksClean(&CsrLogCtx.logLevelFilterTask.tasks);
    CsrLogCtx.logLevelFilterTask.allTaskLogLevel = tasksLogLevelMask;
}

/* Set the logging level for a given Task */
/* This function can be used as a complement to CsrLogLevelTaskSetAll() to add more _or_ less log from a given task than what is set
generally with CsrLogLevelTaskSetAll(). */
void CsrLogLevelTaskSetSpecific(CsrSchedQid taskId, CsrLogLevelTask taskLogLevelMask)
{
    CsrLogLevelFilterTask *entry = csrLogLevelFilterTaskTaskInit(&CsrLogCtx.logLevelFilterTask.tasks, taskId);

    entry->taskLogLevel = taskLogLevelMask;
}

/*--------------------------------------------*/
/*  Filtering on log text warning levels      */
/*--------------------------------------------*/

/* This function call takes precedence over all previous calls to CsrLogLevelTextSetTask() and CsrLogLevelTextSetTaskSubOrigin() */
void CsrLogLevelTextSetAll(CsrLogLevelText warningLevelMask)
{
    CsrLogTextHandle *handle;

    for (handle = CsrLogCtx.logTextHandles;
         handle != NULL;
         handle = handle->next)
    {
        handle->logLevel = warningLevelMask;
        CsrPmemFree(handle->subOriginsLogLevel);
        handle->subOriginsLogLevel = NULL;
        while (handle->subOriginsLogLevelPending != NULL)
        {
            CsrLogLevelTextPending *logLevelTextPending = handle->subOriginsLogLevelPending;
            handle->subOriginsLogLevelPending = logLevelTextPending->next;
            CsrPmemFree(logLevelTextPending);
        }
    }

    csrLogLevelFilterTextTasksClean(&CsrLogCtx.logLevelFilterText.tasks);
    CsrLogCtx.logLevelFilterText.allTaskWarningLevel = warningLevelMask;
}

/* This function call takes precedence over all previous calls to CsrLogLevelTextSetTaskSubOrigin(), but it can be used as a complement to
 * CsrLogLevelTextSetAll() to add more _or_ less log from a given task than what is set generally with CsrLogLevelTextSetAll(). */
void CsrLogLevelTextSetTask(CsrLogTextTaskId taskId, CsrLogLevelText warningLevelMask)
{
    if (taskId >= ((0x2000 << 16) | CSR_SCHED_QID_INVALID))
    {
        CsrLogTextHandle *handle;
        CsrBool exists = FALSE;

        for (handle = CsrLogCtx.logTextHandles;
             handle != NULL;
             handle = handle->next)
        {
            if (handle->taskId == taskId)
            {
                exists = TRUE;
                break;
            }
        }

        if (!exists)
        {
            /* Allocate and fill out handle */
            handle = CsrPmemAlloc(sizeof(CsrLogTextHandle));
            handle->self = NULL;
            handle->subOriginsCount = 0;
            handle->subOriginsLogLevel = NULL;
            handle->logLevel = warningLevelMask;
            handle->taskId = taskId;
            handle->subOriginsLegacy = NULL;
            handle->originName = NULL;
            handle->subOriginsLogLevelPending = NULL;

            /* Insert in list */
            handle->next = CsrLogCtx.logTextHandles;
            CsrLogCtx.logTextHandles = handle;
        }
        else
        {
            CsrPmemFree(handle->subOriginsLogLevel);
            handle->subOriginsLogLevel = NULL;
            handle->logLevel = warningLevelMask;
        }
    }
    else
    {
        CsrLogLevelFilterTextTask *entry = csrLogLevelFilterTextTaskInit(&CsrLogCtx.logLevelFilterText.tasks, taskId);

        /* Clear previous subOrigin entries and set the new warningLevel */
        csrLogLevelFilterTextTaskSubOriginsClean(&entry->subOrigins);
        entry->taskWarningLevel = warningLevelMask;
    }
}

/* This function can be used as a complement to CsrLogLevelTextSetAll() and CsrLogLevelTextSetTask() to add more _or_ less log from a given
 * subOrigin within a task than what is set generally with CsrLogLevelTextSetAll() _or_ CsrLogLevelTextSetTask(). */
void CsrLogLevelTextSetTaskSubOrigin(CsrLogTextTaskId taskId, CsrUint16 subOrigin, CsrLogLevelText warningLevelMask)
{
    if (taskId >= ((0x2000 << 16) | CSR_SCHED_QID_INVALID))
    {
        CsrUint16 i;
        CsrLogTextHandle *handle;
        CsrBool exists = FALSE;

        for (handle = CsrLogCtx.logTextHandles;
             handle != NULL;
             handle = handle->next)
        {
            if (handle->taskId == taskId)
            {
                exists = TRUE;
                break;
            }
        }

        if (!exists)
        {
            /* Allocate and fill out handle */
            handle = CsrPmemAlloc(sizeof(CsrLogTextHandle));
            handle->self = NULL;
            handle->subOriginsCount = 0;
            handle->subOriginsLogLevel = NULL;
            handle->logLevel = CsrLogCtx.logLevelFilterText.allTaskWarningLevel;
            handle->taskId = taskId;
            handle->subOriginsLegacy = NULL;
            handle->originName = NULL;
            handle->subOriginsLogLevelPending = NULL;

            /* Insert in list */
            handle->next = CsrLogCtx.logTextHandles;
            CsrLogCtx.logTextHandles = handle;
        }

        if (handle->subOriginsLogLevel == NULL)
        {
            handle->subOriginsCount = CSRMAX(handle->subOriginsCount, subOrigin);
            handle->subOriginsLogLevel = CsrPmemAlloc(sizeof(CsrLogLevelText) * handle->subOriginsCount);
            for (i = 0; i < handle->subOriginsCount; ++i)
            {
                handle->subOriginsLogLevel[i] = handle->logLevel;
            }
        }
        else if ((subOrigin > handle->subOriginsCount) && (handle->self == NULL))
        {
            /* Integrate subOriginLogLevel information if any exists */
            CsrLogLevelText *subOriginsLogLevel = CsrPmemAlloc(sizeof(CsrLogLevelText) * subOrigin);
            for (i = 0; i < subOrigin; ++i)
            {
                subOriginsLogLevel[i] = handle->logLevel;
            }
            CsrMemCpy(subOriginsLogLevel,
                handle->subOriginsLogLevel,
                CSRMIN(handle->subOriginsCount, subOrigin) * sizeof(CsrLogLevelText));
            CsrPmemFree(handle->subOriginsLogLevel);
            handle->subOriginsLogLevel = subOriginsLogLevel;
            handle->subOriginsCount = subOrigin;
        }
        if (subOrigin <= handle->subOriginsCount)
        {
            handle->subOriginsLogLevel[subOrigin - 1] = warningLevelMask;
        }
    }
    else
    {
        CsrLogLevelFilterTextTask *entry = csrLogLevelFilterTextTaskInit(&CsrLogCtx.logLevelFilterText.tasks, taskId);

        csrLogLevelFilterTextTaskSubOriginInit(&entry->subOrigins, subOrigin, warningLevelMask);
    }
}

void CsrLogLevelTextSet(const CsrCharString *originName, const CsrCharString *subOriginName, CsrLogLevelText warningLevelMask)
{
    if ((originName != NULL) && (*originName != '\0'))
    {
        if ((subOriginName != NULL) && (*subOriginName != '\0'))
        {
            /* Set filter on specific sub origin */
            CsrLogTextHandle *handle;
            CsrBool exists = FALSE;

            for (handle = CsrLogCtx.logTextHandles;
                 handle != NULL;
                 handle = handle->next)
            {
                if ((handle->originName != NULL) && (CsrStrCmp(handle->originName, originName) == 0))
                {
                    exists = TRUE;
                    break;
                }
            }

            if (!exists)
            {
                /* Allocate and fill out handle */
                handle = CsrPmemAlloc(sizeof(CsrLogTextHandle));
                handle->self = NULL;
                handle->subOriginsCount = 0;
                handle->subOriginsLogLevel = NULL;
                handle->logLevel = CsrLogCtx.logLevelFilterText.allTaskWarningLevel;
                handle->taskId = 0;
                handle->subOriginsLegacy = NULL;
                handle->originName = originName;
                handle->subOriginsLogLevelPending = NULL;

                /* Insert in list */
                handle->next = CsrLogCtx.logTextHandles;
                CsrLogCtx.logTextHandles = handle;
            }

            if (handle->subOriginsLegacy != NULL)
            {
                /* Look up sub origin and update log level */
                CsrUint16 i;
                CsrUint16 subOrigin = 0;

                for (i = 0; i < handle->subOriginsCount; ++i)
                {
                    if (CsrStrCmp(handle->subOriginsLegacy[i].subOriginName, subOriginName) == 0)
                    {
                        subOrigin = handle->subOriginsLegacy[i].subOriginNumber;
                        break;
                    }
                }
                if (subOrigin > 0)
                {
                    if (handle->subOriginsLogLevel == NULL)
                    {
                        handle->subOriginsLogLevel = CsrPmemAlloc(sizeof(CsrLogLevelText) * handle->subOriginsCount);
                        for (i = 0; i < handle->subOriginsCount; ++i)
                        {
                            handle->subOriginsLogLevel[i] = handle->logLevel;
                        }
                    }
                    handle->subOriginsLogLevel[subOrigin - 1] = warningLevelMask;
                }
            }
            else if (handle->self == NULL)
            {
                /* Cannot look up sub origin yet, save as pending and process when register occurs */
                CsrLogLevelTextPending **entry = &(handle->subOriginsLogLevelPending);
                CsrLogLevelTextPending *logLevelTextPending = CsrPmemAlloc(sizeof(CsrLogLevelTextPending));
                logLevelTextPending->name = subOriginName;
                logLevelTextPending->logLevel = warningLevelMask;
                logLevelTextPending->next = NULL;
                while (*entry != NULL)
                {
                    entry = &((*entry)->next);
                }
                *entry = logLevelTextPending;
            }
        }
        else
        {
            /* Set filter on specific origin */
            CsrLogTextHandle *handle;
            CsrBool exists = FALSE;

            for (handle = CsrLogCtx.logTextHandles;
                 handle != NULL;
                 handle = handle->next)
            {
                if ((handle->originName != NULL) && (CsrStrCmp(handle->originName, originName) == 0))
                {
                    exists = TRUE;
                    break;
                }
            }

            if (!exists)
            {
                /* Allocate and fill out handle */
                handle = CsrPmemAlloc(sizeof(CsrLogTextHandle));
                handle->self = NULL;
                handle->subOriginsCount = 0;
                handle->subOriginsLogLevel = NULL;
                handle->logLevel = warningLevelMask;
                handle->taskId = 0;
                handle->subOriginsLegacy = NULL;
                handle->originName = originName;
                handle->subOriginsLogLevelPending = NULL;

                /* Insert in list */
                handle->next = CsrLogCtx.logTextHandles;
                CsrLogCtx.logTextHandles = handle;
            }
            else
            {
                CsrPmemFree(handle->subOriginsLogLevel);
                handle->subOriginsLogLevel = NULL;
                while (handle->subOriginsLogLevelPending != NULL)
                {
                    CsrLogLevelTextPending *logLevelTextPending = handle->subOriginsLogLevelPending;
                    handle->subOriginsLogLevelPending = logLevelTextPending->next;
                    CsrPmemFree(logLevelTextPending);
                }
                handle->logLevel = warningLevelMask;
            }
        }
    }
    else
    {
        CsrLogLevelTextSetAll(warningLevelMask);
    }
}

#endif /* CSR_LOG_ENABLE */
