/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include "csr_types.h"
#include "csr_log.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"
#include "csr_log.h"

#include "csr_log2fts_api.h"

#include <windows.h>
#include "platform/csr_logtransport_init.h"

#define DLL_REL_PATH    "\\Executables\\Core\\liveimportapi.dll"
#define INI_REL_PATH    "\\LiveImport.Ini"

#define CSR_LOG_FTS_PIPE_LOAD_API(ctx, func, handle)                \
    do {                                                                \
        char *func_name = #func;                                        \
        ctx->func = (func *) GetProcAddress(handle, func_name);         \
        if (ctx->func == NULL)                                          \
        {                                                               \
            printf("Failed to load function \"%s\"\n", func_name);      \
            FreeLibrary(handle);                                        \
            CsrPmemFree(ctx);                                                 \
            return NULL;                                                \
        }                                                               \
    } while (0)

static csrLogTransportFtsPipeTable *csrLogTransportFtsPipeLoadLiveImportAPIFunctions(char *libname)
{
    csrLogTransportFtsPipeTable *ftsPipeCtx;

    HMODULE hdl = LoadLibrary(libname);

    if (NULL == hdl)
    {
        printf("Failed to load module \"%s\" - error %d", libname, GetLastError());
        return FALSE;
    }

    ftsPipeCtx = zpnew(csrLogTransportFtsPipeTable);
    ftsPipeCtx->hdl = hdl;

    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, InitializeLiveImport, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, ReleaseLiveImport, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, StillAlive, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, IsAppReady, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendFrame, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendEvent, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendControlSignalChange, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendNondataFlagChange, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendBreak, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendFlowControl, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendConnectionStatus, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendConfigurationString, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendSetIODialogHwnd, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendXmitDialogHwnd, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendSpecialEvent, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendStartOfFrame, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendEndOfFrame, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, InitializeLiveImport, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendAbortedFrame, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendByteSimple, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, CheckForMessages, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, GetAppVersionNumber, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, GetAppSerialNumber, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, GetAppDisplayedConfigurationName, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, GetSerialNumberSectionKeyValuePairs, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, RegisterNotification, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendNotification, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendArraySimple, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendStringSimple, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendNumberOfLostMessages, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, UpdateStat, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, FramesLost, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SetExePath, hdl);
    CSR_LOG_FTS_PIPE_LOAD_API(ftsPipeCtx, SendPostNotify, hdl);

    return ftsPipeCtx;
}

static CsrBool csrLogTransportFtsIsConnected(csrLogTransportFtsPipeTable *table)
{
    if (table->IsAppReady != NULL)
    {
        CsrBool isConnected = FALSE;
        DWORD dwStatus = 0;
        dwStatus = table->IsAppReady(&isConnected);

        return !FAILED(dwStatus) || isConnected;
    }
    else
    {
        return FALSE;
    }
}

static const int csrLogInitiatorLut[] =
{
    0, /* ACL Data (Host) */
    1, /* ACL Data (CTRL) */
    0, /* HCI Command */
    1, /* HCI Event */
};

/* Reference: liveimport.ini */
static const int csrLogLiveFlagsLut[] =
{
    0x2, /* ACL Data (Host) */
    0x2, /* ACL Data (CTRL) */
    0x1, /* HCI Command */
    0x8, /* HCI Event */
};

static CsrSize csrLogTransportFtsPipeWrite(void *ctx, CsrLogContext context,
    void *data, CsrSize size)
{
    int status;
    SYSTEMTIME systemtime;
    __int64 timestamp;
    CsrUint32 flags;
    CsrUint8 *payload = data;

    csrLogTransportFtsPipeTable *table = ctx;

    EnterCriticalSection(&table->logMutex);

    GetLocalTime(&systemtime);
    SystemTimeToFileTime(&systemtime, (FILETIME *) &timestamp);

    /* Copy flags */
    CsrMemCpy(&flags, payload, sizeof(flags));

    CSR_LOG_ASSERT(flags < CSR_ARRAY_SIZE(csrLogInitiatorLut));

    if ((table->SendFrame != NULL) && csrLogTransportFtsIsConnected(table))
    {
        if (FAILED(status = table->SendFrame(
                    /* Payload/size shouldn't include flags  */
                    (int) (size - sizeof(flags)),
                    (int) (size - sizeof(flags)),
                    &payload[sizeof(flags)],
                    csrLogLiveFlagsLut[flags],         /* HCI primitive */
                    csrLogInitiatorLut[flags],         /* Initiator */
                    timestamp)))
        {
            return 0;
        }
    }
    else
    {
        return 0; /* Sniffer sink not connected */
    }

    LeaveCriticalSection(&table->logMutex);

    return size;
}

static void csrLogTransportFtsDelimitByNewline(char *conf_str)
{
    char *p_conf_str;
    CsrSize i;

    p_conf_str = conf_str;

    /* Live Import wants a single string in which each line terminated by a new line */
    while (p_conf_str && ((i = CsrStrLen(p_conf_str)) != 0))
    {
        p_conf_str[i] = '\n';
        p_conf_str += (i + 1);
    }
}

static void *csrLogTransportFtsPipeAlloc(void *arg, CsrSize size)
{
    return CsrPmemAlloc(size);
}

static void csrLogTransportFtsPipeFree(void *arg, void *data)
{
    CsrPmemFree(data);
}

void *CsrLogTransportFtsPipeOpen(const char *fts_dir)
{
    char conf_str[512];
    char connection_str[512];
    csrLogTransportFtsPipeTable *ftsPipeCtx;
    csrLogTransportHandle *ltHdl;
    char live_import_dll[_MAX_PATH];
    char live_import_ini[_MAX_PATH];
    CsrBool success = FALSE;
    HRESULT hr;

    CsrStrCpy(live_import_dll, fts_dir);
    CsrStrCat(live_import_dll, DLL_REL_PATH);

    CsrStrCpy(live_import_ini, fts_dir);
    CsrStrCat(live_import_ini, INI_REL_PATH);

    /* Get the connection string from the shared ini file */
    GetPrivateProfileString("General",
        "ConnectionString",
        "",
        connection_str,
        sizeof(connection_str),
        live_import_ini);

    if (CsrStrLen(connection_str) == 0)
    {
        printf(" !!! ConnectionString not found in: %s !!!\n", live_import_ini);
        return NULL;
    }

    /* Get the configuration data from the shared .INI file */
    GetPrivateProfileSection("Configuration",
        conf_str,
        sizeof(conf_str),
        live_import_ini);

    if (CsrStrLen(conf_str) == 0)
    {
        printf(" !!! Configuration section not found in: %s !!!\n", live_import_ini);
        return NULL;
    }

    csrLogTransportFtsDelimitByNewline(conf_str);

    /* Initialize the FTS LiveImport Server */
    ftsPipeCtx = csrLogTransportFtsPipeLoadLiveImportAPIFunctions(live_import_dll);

    if (!ftsPipeCtx)
    {
        return NULL;
    }

    InitializeCriticalSection(&ftsPipeCtx->logMutex);

    hr = ftsPipeCtx->InitializeLiveImport(connection_str, conf_str, &success);

    if (FAILED(hr) || !success)
    {
        printf(" !!! g_pInitializeLiveImport() failed with status 0x%x !!!\n", hr);
    }

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = csrLogTransportFtsPipeWrite;
    ltHdl->alloc = csrLogTransportFtsPipeAlloc;
    ltHdl->free = csrLogTransportFtsPipeFree;
    ltHdl->arg = ftsPipeCtx;

    return ltHdl;
}

void CsrLogTransportFtsPipeClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFtsPipeTable *table;


    ltHdl = (csrLogTransportHandle *) arg;
    table = (csrLogTransportFtsPipeTable *) ltHdl->arg;

    if (table->ReleaseLiveImport != NULL)
    {
        table->ReleaseLiveImport();
    }

    FreeLibrary(table->hdl);

    DeleteCriticalSection(&table->logMutex);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}
