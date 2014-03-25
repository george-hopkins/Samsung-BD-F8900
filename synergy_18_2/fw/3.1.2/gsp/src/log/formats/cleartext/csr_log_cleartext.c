/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_framework_ext.h"
#include "csr_formatted_io.h"
#include "csr_usr_config.h"

#include "csr_log_configure.h"
#include "csr_log_text.h"
#include "csr_log_formats.h"
#include "csr_log_cleartext.h"
#include "csr_logtransport.h"

#ifdef CSR_LOG_ENABLE

#define CSR_LOG_CLEARTEXT_TEMPLATE_COUNT (17)

#define CSR_LOG_CLEARTEXT_TRANSPORT_ALLOC(buf, handle, length) \
    buf = CsrLogTransportAlloc(handle, length);  \
    if (!buf)                                   \
    {                                           \
        return;                                 \
    }

typedef struct CsrLogTextRegisterRecord
{
    CsrLogTextTaskId                 taskId;
    CsrCharString                   *taskName;
    CsrUint16                        subOriginsLen;
    CsrLogSubOrigin                 *subOrigins;
    struct CsrLogTextRegisterRecord *next;
} CsrLogTextRegisterRecord;

static CsrMutexHandle registerLock;
static CsrLogTextRegisterRecord *records;

static void lock(void)
{
    static CsrBool initialised = FALSE;

    CsrGlobalMutexLock();
    if (!initialised)
    {
        CsrMutexCreate(&registerLock);
        initialised = TRUE;
    }
    CsrGlobalMutexUnlock();
    CsrMutexLock(&registerLock);
}

static void unlock(void)
{
    CsrMutexUnlock(&registerLock);
}

static CsrUint8 formatStrLengthTaskName;
static CsrUint8 formatStrLengthSubOrigin;

/* For scheduler event logging */
typedef struct CsrLogTextTaskMap
{
    CsrLogTextTaskId          qid;
    CsrCharString            *taskName;
    struct CsrLogTextTaskMap *next;
} CsrLogTextTaskMap;

static CsrLogTextTaskMap *taskMapHead = NULL;

static void taskMapAdd(CsrSchedQid qid, const CsrCharString *name)
{
    CsrLogTextTaskMap *map;

    map = CsrPmemAlloc(sizeof(*map));
    map->qid = qid;
    map->taskName = CsrStrDup(name);

    lock();
    map->next = taskMapHead;
    taskMapHead = map;
    unlock();
}

static CsrCharString *taskMapRemove(CsrSchedQid qid)
{
    CsrLogTextTaskMap *map, *prev;
    CsrCharString *rv;

    prev = NULL;

    lock();
    map = taskMapHead;

    while (map)
    {
        if (map->qid == qid)
        {
            if (prev == NULL)
            {
                taskMapHead = map->next;
            }
            else
            {
                prev->next = map->next;
            }
            break;
        }
        prev = map;
        map = map->next;
    }
    unlock();

    if (map)
    {
        rv = map->taskName;
        CsrPmemFree(map);
    }
    else
    {
        rv = "";
    }

    return rv;
}

static const CsrCharString *taskMapLookup(CsrSchedQid qid)
{
    CsrLogTextTaskMap *map;
    const CsrCharString *rv;

    rv = "";

    lock();
    for (map = taskMapHead;
         map;
         map = map->next)
    {
        if (map->qid == qid)
        {
            rv = map->taskName;
            break;
        }
    }
    unlock();

    return rv;
}

/* Log text logging */
static void csrLogTextRegister(void *ltHdl,
    CsrLogTextTaskId taskId,
    const CsrCharString *taskName,
    CsrUint16 subOriginsLen,
    const CsrLogSubOrigin *subOrigins)
{
    CsrInt32 originIdx;
    CsrSize strLen;

    /* Allocate a record */
    CsrLogTextRegisterRecord *record = CsrPmemAlloc(sizeof(CsrLogTextRegisterRecord));

    CSR_UNUSED(ltHdl);

    /* Fill in information */
    record->taskId = taskId;
    record->taskName = CsrStrDup(taskName);
    record->subOriginsLen = subOriginsLen;
    record->subOrigins = CsrMemDup(subOrigins, sizeof(CsrLogSubOrigin) * subOriginsLen);
    for (originIdx = 0; originIdx < subOriginsLen; originIdx++)
    {
        /* Duplicate all subOrigin strings */
        record->subOrigins[originIdx].subOriginName = CsrStrDup(record->subOrigins[originIdx].subOriginName);
        strLen = CsrStrLen(record->subOrigins[originIdx].subOriginName);
        formatStrLengthSubOrigin = (CsrUint8) CSRMAX(formatStrLengthSubOrigin, strLen);
    }

    strLen = CsrStrLen(record->taskName);
    formatStrLengthTaskName = (CsrUint8) CSRMAX(formatStrLengthTaskName, strLen);

    lock();
    /* Attach to list */
    record->next = records;
    records = record;
    unlock();
}

static void csrLogTextRegister2(void *ltHdl,
    CsrLogTextTaskId taskId,
    const CsrCharString *originName,
    CsrUint16 subOriginsCount,
    const CsrCharString *subOrigins[])
{
    CsrSize strLen;
    CsrUint16 i;

    CSR_UNUSED(ltHdl);
    CSR_UNUSED(taskId);

    for (i = 0; i < subOriginsCount; ++i)
    {
        strLen = CsrStrLen(subOrigins[i]);
        formatStrLengthSubOrigin = (CsrUint8) CSRMAX(formatStrLengthSubOrigin, strLen);
    }

    strLen = CsrStrLen(originName);
    formatStrLengthTaskName = (CsrUint8) CSRMAX(formatStrLengthTaskName, strLen);
}

static CsrCharString *levelToString(CsrLogLevelText level)
{
    switch (level)
    {
        case CSR_LOG_LEVEL_TEXT_CRITICAL:
            return "CRI";
        case CSR_LOG_LEVEL_TEXT_ERROR:
            return "ERR";
        case CSR_LOG_LEVEL_TEXT_WARNING:
            return "WRN";
        case CSR_LOG_LEVEL_TEXT_INFO:
            return "INF";
        case CSR_LOG_LEVEL_TEXT_DEBUG:
            return "DBG";
    }
    return NULL;
}

#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_YEAR             (0)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_MONTH            (1)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_DAY              (2)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_HOUR             (3)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_MIN              (4)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_SEC         (5)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_MSEC        (6)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_HIGH        (7)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_LOW         (8)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_NAME   (9)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_ID     (10)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_NAME        (11)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_ID          (12)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_NAME   (13)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_ID     (14)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_STRING           (15)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_BUFFER           (16)
#define CSR_LOG_CLEARTEXT_TEMPLATE_ID_APP_STRING       (17)

static CsrUint8 formatPiecesCount;
static struct
{
    CsrUint8       type;
    CsrCharString *str;
} formatPieces[CSR_LOG_CLEARTEXT_TEMPLATE_COUNT * 2 - 1 + 2];

static void csrLogTextSnprintf(CsrCharString *dest, CsrSize n, CsrUint32 *offset, const CsrCharString *fmt, ...)
{
    CsrInt32 r;
    va_list args;

    if (*offset >= n)
    {
        return;
    }

    va_start(args, fmt);
    r = CsrVsnprintf(&dest[*offset], n - *offset, fmt, args);
    va_end(args);

    if ((r >= (CsrInt32) (n - *offset)) || (r < 0))
    {
        *offset = (CsrUint32) n;
    }
    else
    {
        *offset += (CsrUint32) r;
    }
}

static void csrLogTextVsnprintf(CsrCharString *dest, CsrSize n, CsrUint32 *offset, const CsrCharString *fmt, va_list args)
{
    CsrInt32 r;

    if (*offset >= n)
    {
        return;
    }

    r = CsrVsnprintf(&dest[*offset], n - *offset, fmt, args);

    if ((r >= (CsrInt32) (n - *offset)) || (r < 0))
    {
        *offset = (CsrUint32) n;
    }
    else
    {
        *offset += (CsrUint32) r;
    }
}

static void csrLogTextHexPrintf(CsrCharString *dest, CsrSize n, CsrUint32 *offset, CsrSize bufferLength, const CsrUint8 *buffer)
{
    const CsrCharString *sep = " ";
    CsrSize bufferIndex = 0;

    if (*offset >= n)
    {
        return;
    }

    while (bufferIndex < bufferLength && *offset < n)
    {
        CsrInt32 r = CsrSnprintf(&dest[*offset], n - *offset, "%s%02X", sep, buffer[bufferIndex]);
        if ((r >= (CsrInt32) (n - *offset)) || (r < 0))
        {
            *offset = (CsrUint32) n;
        }
        else
        {
            *offset += (CsrUint32) r;
        }

        bufferIndex++;
        sep = ":";
    }
}

static CsrLogTextRegisterRecord *findRecord(CsrLogTextTaskId taskId)
{
    CsrLogTextRegisterRecord *record;

    record = records;
    while (record != NULL)
    {
        if (record->taskId == taskId)
        {
            break;
        }
        record = record->next;
    }
    return record;
}

static const CsrCharString *findSuboriginName(CsrLogTextRegisterRecord *record, CsrUint16 subOrigin)
{
    CsrUint32 originIdx;

    for (originIdx = 0; originIdx < record->subOriginsLen; ++originIdx)
    {
        if (record->subOrigins[originIdx].subOriginNumber == subOrigin)
        {
            return record->subOrigins[originIdx].subOriginName;
        }
    }
    return NULL;
}

static void csrGmtimeGet(CsrUint16 *year, CsrUint8 *month, CsrUint8 *day, CsrUint8 *hour, CsrUint8 *min, CsrUint8 *sec, CsrUint16 *msec)
{
#define SECS_PER_DAY        (24 * 60 * 60)
#define IS_LEAP_YEAR(year)  (!((year) % 4) && (((year) % 100) || !((year) % 400)))
#define DAYS_PER_YEAR(year) ((CsrUint16) (IS_LEAP_YEAR(year) ? 366 : 365))

    CsrUint32 daySec, days;
    CsrBool isLeapYear;
    CsrTimeUtc tod = {0, 0};

    static const CsrUint8 daysPerMonth[2][12] =
    {
        /* Non leap year */
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        /* Leap year */
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    CsrTimeUtcGet(&tod, NULL, NULL);

    daySec = tod.sec % SECS_PER_DAY;
    days = tod.sec / SECS_PER_DAY;

    *year = 1970;
    *month = 0;
    *hour = (CsrUint8) (daySec / 3600);
    *min = (CsrUint8) ((daySec % 3600) / 60);
    *sec = (CsrUint8) (daySec % 60);
    *msec = tod.msec;
    while (days >= DAYS_PER_YEAR(*year))
    {
        days -= DAYS_PER_YEAR(*year);
        (*year)++;
    }
    isLeapYear = IS_LEAP_YEAR(*year);
    while (days >= daysPerMonth[isLeapYear][*month])
    {
        days -= daysPerMonth[isLeapYear][*month];
        (*month)++;
    }
    *day = (CsrUint8) (days + 1);

#undef SECS_PER_DAY
#undef IS_LEAP_YEAR
#undef DAYS_PER_YEAR
}

static void csrLogBufferTextPrint2(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    const CsrCharString *originName,
    const CsrCharString *subOriginName,
    CsrLogLevelText level,
    CsrSize bufferLength,
    const void *buffer,
    const CsrCharString *formatString,
    va_list varargs)
{
    CsrCharString *buf;
    CsrUint32 offset = 0;
    CsrUint32 bufSize = CSR_LOG_TEXT_MAX_STRING_LEN;
    CsrUint8 formatPiecesIndex = 0;

    CsrBool timeFetched = FALSE;
    CsrBool utcFetched = FALSE;
    CsrTime low = 0;
    CsrTime high = 0;
    CsrUint16 year = 0;
    CsrUint8 month = 0;
    CsrUint8 day = 0;
    CsrUint8 hour = 0;
    CsrUint8 min = 0;
    CsrUint8 sec = 0;
    CsrUint16 msec = 0;

    /* Allocate extra byte for newline */
    CSR_LOG_CLEARTEXT_TRANSPORT_ALLOC(buf, ltHdl, bufSize + 1);

    while (formatPiecesIndex < formatPiecesCount && offset < bufSize)
    {
        switch (formatPieces[formatPiecesIndex].type)
        {
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_YEAR:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%u", year);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_MONTH:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%02u", month);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_DAY:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%02u", day);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_HOUR:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%02u", hour);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_MIN:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%02u", min);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_SEC:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%02u", sec);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_MSEC:
            {
                if (!utcFetched)
                {
                    csrGmtimeGet(&year, &month, &day, &hour, &min, &sec, &msec);
                    utcFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%03u", msec);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_HIGH:
            {
                if (!timeFetched)
                {
                    low = CsrTimeGet(&high);
                    timeFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%10u", high);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_LOW:
            {
                if (!timeFetched)
                {
                    low = CsrTimeGet(&high);
                    timeFetched = TRUE;
                }
                csrLogTextSnprintf(buf, bufSize, &offset, "%10u", low);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_NAME:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%s", levelToString(level));
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_ID:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%u", level);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_NAME:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%-*s", formatStrLengthTaskName, originName);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_ID:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%8X", taskId);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_NAME:
            {
                if (subOriginName != NULL)
                {
                    csrLogTextSnprintf(buf, bufSize, &offset, "%-*s", formatStrLengthSubOrigin, subOriginName);
                }
                else
                {
                    csrLogTextSnprintf(buf, bufSize, &offset, "%-*s", formatStrLengthSubOrigin, "");
                }
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_ID:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%8X", subOrigin);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_STRING:
            {
                csrLogTextVsnprintf(buf, bufSize, &offset, formatString, varargs);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_BUFFER:
            {
                csrLogTextHexPrintf(buf, bufSize, &offset, bufferLength, buffer);
                break;
            }
            case CSR_LOG_CLEARTEXT_TEMPLATE_ID_APP_STRING:
            {
                csrLogTextSnprintf(buf, bufSize, &offset, "%s", formatPieces[formatPiecesIndex].str);
                break;
            }
            default:
                break;
        }
        ++formatPiecesIndex;
    }
    buf[offset++] = '\n';
    CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
}

static void csrLogBufferTextPrint(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    CsrLogLevelText level,
    CsrSize bufferLength,
    const void *buffer,
    const CsrCharString *formatString,
    va_list varargs)
{
    CsrLogTextRegisterRecord *record;

    lock();
    record = findRecord(taskId);
    unlock();

    if (record != NULL)
    {
        const CsrCharString *subOriginName = findSuboriginName(record, subOrigin);
        csrLogBufferTextPrint2(ltHdl, taskId, subOrigin,
            record->taskName, subOriginName,
            level, bufferLength, buffer, formatString, varargs);
    }
}

static void csrLogTextPrint2(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    const CsrCharString *originName,
    const CsrCharString *subOriginName,
    CsrLogLevelText level,
    const CsrCharString *formatString,
    va_list varargs)
{
    csrLogBufferTextPrint2(ltHdl, taskId, subOrigin,
        originName, subOriginName,
        level, 0, NULL, formatString, varargs);
}

static void csrLogTextPrint(void *ltHdl,
    CsrLogTextTaskId taskId,
    CsrUint16 subOrigin,
    CsrLogLevelText level,
    const CsrCharString *formatString,
    va_list varargs)
{
    csrLogBufferTextPrint(ltHdl, taskId, subOrigin,
        level, 0, NULL, formatString, varargs);
}

static CsrBool parseFormatString(const CsrCharString *outputFormatString)
{
    const CsrCharString *haystack;
    CsrUint8 haystackIndex = 0;

    outputFormatString = outputFormatString ? outputFormatString : CSR_LOG_CLEARTEXT_FORMAT;
    CSR_LOG_ASSERT(outputFormatString != NULL);

    haystack = outputFormatString;

    while (haystack[haystackIndex] != '\0')
    {
        static const struct
        {
            const CsrCharString *templateName;
            const CsrUint8       templateType;
        } templateNameIdMap[] =
        {
            {CSR_LOG_CLEARTEXT_TEMPLATE_YEAR, CSR_LOG_CLEARTEXT_TEMPLATE_ID_YEAR},
            {CSR_LOG_CLEARTEXT_TEMPLATE_MONTH, CSR_LOG_CLEARTEXT_TEMPLATE_ID_MONTH},
            {CSR_LOG_CLEARTEXT_TEMPLATE_DAY, CSR_LOG_CLEARTEXT_TEMPLATE_ID_DAY},
            {CSR_LOG_CLEARTEXT_TEMPLATE_HOUR, CSR_LOG_CLEARTEXT_TEMPLATE_ID_HOUR},
            {CSR_LOG_CLEARTEXT_TEMPLATE_MIN, CSR_LOG_CLEARTEXT_TEMPLATE_ID_MIN},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TIME_SEC, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_SEC},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TIME_MSEC, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_MSEC},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TIME_HIGH, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_HIGH},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TIME_LOW, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TIME_LOW},
            {CSR_LOG_CLEARTEXT_TEMPLATE_LOG_LEVEL_NAME, CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_NAME},
            {CSR_LOG_CLEARTEXT_TEMPLATE_LOG_LEVEL_ID, CSR_LOG_CLEARTEXT_TEMPLATE_ID_LOG_LEVEL_ID},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TASK_NAME, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_NAME},
            {CSR_LOG_CLEARTEXT_TEMPLATE_TASK_ID, CSR_LOG_CLEARTEXT_TEMPLATE_ID_TASK_ID},
            {CSR_LOG_CLEARTEXT_TEMPLATE_SUBORIGIN_NAME, CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_NAME},
            {CSR_LOG_CLEARTEXT_TEMPLATE_SUBORIGIN_ID, CSR_LOG_CLEARTEXT_TEMPLATE_ID_SUBORIGIN_ID},
            {CSR_LOG_CLEARTEXT_TEMPLATE_STRING, CSR_LOG_CLEARTEXT_TEMPLATE_ID_STRING},
            {CSR_LOG_CLEARTEXT_TEMPLATE_BUFFER, CSR_LOG_CLEARTEXT_TEMPLATE_ID_BUFFER}
        };

        if (formatPiecesCount >= CSR_ARRAY_SIZE(formatPieces))
        {
            return FALSE;
        }

        if (haystack[haystackIndex] == '$')
        {
            CsrUint8 mapIndex;
            CsrBool found = FALSE;
            for (mapIndex = 0; mapIndex < CSR_ARRAY_SIZE(templateNameIdMap); ++mapIndex)
            {
                const CsrCharString *templateName = templateNameIdMap[mapIndex].templateName;
                if (CsrStrNCmp(&haystack[haystackIndex], templateName, CsrStrLen(templateName)) == 0)
                {
                    formatPieces[formatPiecesCount].type = templateNameIdMap[mapIndex].templateType;
                    haystackIndex += (CsrUint8) CsrStrLen(templateName);
                    found = TRUE;
                    break;
                }
            }

            if (!found)
            {
                /* Unknown template parameter. Reject format string */
                return FALSE;
            }
        }
        else
        {
            CsrUint8 haystackIndexPrev = haystackIndex;
            while (haystack[haystackIndex] != '\0' && haystack[haystackIndex] != '$')
            {
                ++haystackIndex;
            }
            formatPieces[formatPiecesCount].type = CSR_LOG_CLEARTEXT_TEMPLATE_ID_APP_STRING;
            formatPieces[formatPiecesCount].str = CsrStrDup(&haystack[haystackIndexPrev]);
            /* We don't have CsrStrNDup so this is necessary */
            formatPieces[formatPiecesCount].str[haystackIndex - haystackIndexPrev] = '\0';
        }
        ++formatPiecesCount;
    }
    return TRUE;
}

/*
 * Scheduler event callbacks
 */

static CsrCharString *csrLogTestSchedBufPrepare(void *ltHdl, CsrUint32 *offset)
{
    CsrCharString *buf;
    CsrUint16 unused1, msec;
    CsrUint8 unused2, hour, min, sec;

    buf = CsrLogTransportAlloc(ltHdl, CSR_LOG_TEXT_MAX_STRING_LEN + 1);

    if (buf == NULL)
    {
        return NULL;
    }

    *offset = 0;

    csrGmtimeGet(&unused1, &unused2, &unused2, &hour, &min, &sec, &msec);

    csrLogTextSnprintf(buf,
        CSR_LOG_TEXT_MAX_STRING_LEN,
        offset,
        "%02u:%02u:%02u.%03u ",
        hour, min, sec, msec);

    return buf;
}

static void csrLogTextTaskInit(void *ltHdl,
    CsrUint8 thread_id,
    CsrSchedQid qid,
    const CsrCharString *taskName)
{
    CsrCharString *buf;
    CsrUint32 offset;

    taskMapAdd(qid, taskName);

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Task init %s <%u>", taskName, qid);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
    }
}

static void csrLogTextTaskDeinit(void *ltHdl,
    CsrSchedQid qid)
{
    CsrCharString *taskName;
    CsrCharString *buf;
    CsrUint32 offset;

    taskName = taskMapRemove(qid);

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Task deinit %s <%u>", taskName, qid);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }

    CsrPmemFree(taskName);
}

static void csrLogTextTaskActivate(void *ltHdl,
    CsrSchedQid qid)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Task activate %s", taskName);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextTaskDeactivate(void *ltHdl,
    CsrSchedQid qid)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Task deactivate %s", taskName);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextMsgPut(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid srcQid,
    CsrSchedQid dstQid,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *srcName, *dstName;

        srcName = taskMapLookup(srcQid);
        dstName = taskMapLookup(dstQid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Put: src %s dst %s msg %p type 0x%04x",
            srcName,
            dstName,
            msg,
            prim_type);
        if (msg)
        {
            CsrPrim *prim;

            prim = (CsrPrim *) msg;

            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                " prim %#04x",
                *prim);
        }

        if (file)
        {
            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                " [%s:%u]",
                file,
                line);
        }

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextMsgGet(void *ltHdl,
    CsrSchedQid srcQid,
    CsrSchedQid dstQid,
    CsrBool get_res,
    CsrSchedMsgId msg_id,
    CsrUint16 prim_type,
    const void *msg,
    CsrBool onlyPrimType,
    CsrBool limitPrim)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *dstName;

        dstName = taskMapLookup(dstQid);

        if (get_res)
        {
            const CsrCharString *srcName;

            srcName = taskMapLookup(srcQid);

            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                "Get: src %s dst %s msg %p type 0x%04x",
                srcName,
                dstName,
                msg,
                prim_type);

            if (msg)
            {
                CsrPrim *prim;

                prim = (CsrPrim *) msg;

                csrLogTextSnprintf(buf,
                    CSR_LOG_TEXT_MAX_STRING_LEN,
                    &offset,
                    " prim %#04x",
                    *prim);
            }
        }
        else
        {
            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                "Get: %s: no message", dstName);
        }


        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextTimerSet(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid qid,
    CsrSchedTid tid,
    CsrTime delay,
    CsrUint16 fniarg,
    const CsrUint8 *fnvarg)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Timer set: %s tid %u (%u ms 0x%04x %p)",
            taskName,
            tid,
            delay / 1000,
            fniarg,
            fnvarg);

        if (file)
        {
            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                " [%s:%u]",
                file,
                line);
        }

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextTimerFire(void *ltHdl,
    CsrSchedQid qid,
    CsrSchedTid tid)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Timer fire: %s tid %u",
            taskName,
            tid);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextTimerDone(void *ltHdl,
    CsrSchedQid qid,
    CsrSchedTid tid)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Timer done: %s tid %u",
            taskName,
            tid);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextTimerCancel(void *ltHdl,
    CsrUint32 line,
    const CsrCharString *file,
    CsrSchedQid qid,
    CsrSchedTid tid,
    CsrBool cancelled)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        const CsrCharString *taskName;

        taskName = taskMapLookup(qid);

        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Timer cancel: %s tid %u %scancelled",
            taskName,
            tid,
            cancelled ? "" : "not ");

        if (file)
        {
            csrLogTextSnprintf(buf,
                CSR_LOG_TEXT_MAX_STRING_LEN,
                &offset,
                " [%s:%u]",
                file,
                line);
        }

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextBgintRegister(void *ltHdl,
    CsrUint8 thread_id,
    CsrSchedBgint irq,
    const CsrCharString *id,
    const void *arg)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Bgint register: %s irq %u arg %p",
            id,
            irq,
            arg);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
    }
}

static void csrLogTextBgintUnregister(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Bgint unregister: irq %u",
            irq);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
    }
}

static void csrLogTextBgintSet(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Bgint set: irq %u",
            irq);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_NORMAL, buf, offset);
    }
}

static void csrLogTextBgintFire(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Bgint fire: irq %u",
            irq);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
    }
}

static void csrLogTextBgintDone(void *ltHdl,
    CsrSchedBgint irq)
{
    CsrCharString *buf;
    CsrUint32 offset;

    buf = csrLogTestSchedBufPrepare(ltHdl, &offset);

    if (buf)
    {
        csrLogTextSnprintf(buf,
            CSR_LOG_TEXT_MAX_STRING_LEN,
            &offset,
            "Bgint done: irq %u",
            irq);

        buf[offset++] = '\n';
        CsrLogTransportWrite(ltHdl, LOG_HEADER, buf, offset);
    }
}

/*
 * Public interface
 */
CsrLog *CsrLogCleartextCreate(void *ltHdl, const CsrCharString *outputFormatString, CsrBool schedEvents)
{
    CsrLog *l;

    if (!parseFormatString(outputFormatString))
    {
        return NULL;
    }

    l = CsrPmemZalloc(sizeof(*l));

    l->ltextregister = csrLogTextRegister;
    l->ltextregister2 = csrLogTextRegister2;
    l->ltextbufprint = csrLogBufferTextPrint;
    l->ltextbufprint2 = csrLogBufferTextPrint2;
    l->ltextprint = csrLogTextPrint;
    l->ltextprint2 = csrLogTextPrint2;
    if (schedEvents)
    {
        l->linittask = csrLogTextTaskInit;
        l->ldeinittask = csrLogTextTaskDeinit;
        l->lactivate = csrLogTextTaskActivate;
        l->ldeactivate = csrLogTextTaskDeactivate;
        l->lputmsg = csrLogTextMsgPut;
        l->lgetmsg = csrLogTextMsgGet;
        l->ltimedeventin = csrLogTextTimerSet;
        l->ltimedeventfire = csrLogTextTimerFire;
        l->ltimedeventdone = csrLogTextTimerDone;
        l->ltimedeventcancel = csrLogTextTimerCancel;
        l->bgintreg = csrLogTextBgintRegister;
        l->bgintunreg = csrLogTextBgintUnregister;
        l->bgintset = csrLogTextBgintSet;
        l->bgintservicestart = csrLogTextBgintFire;
        l->bgintservicedone = csrLogTextBgintDone;
    }
    l->ltHdl = ltHdl;

    return l;
}

void CsrLogCleartextDestroy(CsrLog *l)
{
    CsrUint8 i = 0;

    CsrMutexDestroy(&registerLock);

    while (i < formatPiecesCount)
    {
        CsrPmemFree(formatPieces[i].str);
        formatPieces[i].str = NULL;
        ++i;
    }

    formatPiecesCount = 0;

    /* Clean out the record list */
    while (records != NULL)
    {
        CsrLogTextRegisterRecord *record = records;

        CsrPmemFree(record->taskName);

        for (i = 0; i < record->subOriginsLen; i++)
        {
            CsrPmemFree((void *) record->subOrigins[i].subOriginName);
        }

        CsrPmemFree(record->subOrigins);

        records = records->next;
        CsrPmemFree(record);
    }

    CsrPmemFree(l);
}

#endif /* CSR_LOG_ENABLE */
