/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"
#include "csr_log.h"

#include "platform/csr_logtransport_init.h"

#define UID_SIZE     (15)

static CRITICAL_SECTION logMutex;

static const char def_ws_cmd[] = "%s -k -i %s";
static const char def_ws_exec[] = "\"C:\\Program Files\\Wireshark\\wireshark.exe\"";
static const char def_ws_pipe_pattern[] = "\\\\.\\pipe\\csr_log%d";


static CsrSize csrLogTransportWSPipeWrite(void *ctx, CsrLogContext context,
    void *data, CsrSize length)
{
    DWORD cbWritten = 0;
    HANDLE *hPipe = ctx;

    CSR_UNUSED(context);

    EnterCriticalSection(&logMutex);

    if (*hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(*hPipe,       /* handle to pipe */
            data,               /* buffer to write from */
            (DWORD) length,     /* number of bytes to write */
            &cbWritten,         /* number of bytes written */
            NULL);              /* not overlapped I/O */

        if (cbWritten != length)
        {
            printf("Log2ws: cbWritten != length\n");
        }
    }

    LeaveCriticalSection(&logMutex);

    return cbWritten;
}

void CsrLogTransportWSPipeClose(void *arg)
{
    csrLogTransportHandle *ltHdl;

    DeleteCriticalSection(&logMutex);

    ltHdl = (csrLogTransportHandle *) arg;

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

static void *csrLogTransportWSPipeAlloc(void *arg, CsrSize size)
{
    return CsrPmemAlloc(size);
}

static void csrLogTransportWSPipeFree(void *arg, void *data)
{
    CsrPmemFree(data);
}

void *CsrLogTransportWSPipeOpen(const char *ws_exec, const char *ws_pipe)
{
    csrLogTransportHandle *ltHdl;
    CsrBool client_connected = FALSE;
    CsrBool process_launched = FALSE;
    char *cmdline = NULL;
    char *pipename = NULL;
    HANDLE hPipe;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    if (!ws_exec)
    {
        ws_exec = def_ws_exec;
    }

    if (!ws_pipe)
    {
        pipename = CsrPmemAlloc(CsrStrLen(def_ws_pipe_pattern) + UID_SIZE);
        sprintf(pipename, def_ws_pipe_pattern, GetCurrentProcessId());
        ws_pipe = pipename;
    }

    cmdline = CsrPmemAlloc(CsrStrLen(ws_pipe) + CsrStrLen(ws_exec) + CsrStrLen(def_ws_cmd));
    sprintf(cmdline, def_ws_cmd, ws_exec, ws_pipe);

    InitializeCriticalSection(&logMutex);

    CsrMemSet(&si, 0, sizeof(si));
    si.cb = sizeof(si);

    hPipe = CreateNamedPipe(pipename,                 /* pipe name */
        PIPE_ACCESS_OUTBOUND,                         /* server -> client */
        PIPE_TYPE_BYTE |                              /* message type pipe */
        PIPE_READMODE_BYTE |                          /* message-read mode */
        PIPE_WAIT,                                    /* blocking mode */
        PIPE_UNLIMITED_INSTANCES,                     /* CSRMAX. instances */
        65535,                                        /* output buffer size */
        65535,                                        /* input buffer size */
        0,                                            /* client time-out */
        NULL);                                        /* default security attribute */

    if (hPipe == INVALID_HANDLE_VALUE)
    {
        printf("CreatePipe for %s failed\n", pipename);
        goto cleanup;
    }

    process_launched = CreateProcess(0,                /* executable name */
        cmdline,                                       /* command line */
        0,                                             /* process security attribute */
        0,                                             /* thread security attribute */
        FALSE,                                         /* inherits system handles */
        0,                                             /* normal attached process */
        0,                                             /* environment block */
        0,                                             /* inherits the current directory */
        &si,                                           /* (in) startup information */
        &pi);                                          /* (out) process information */

    if (!process_launched)
    {
        printf("CreateProcess for %s failed\n", cmdline);
        goto cleanup;
    }

    client_connected = ConnectNamedPipe(hPipe, NULL)
                       ? TRUE
                       : (GetLastError() == ERROR_PIPE_CONNECTED);

    if (!client_connected)
    {
        printf("ConnectNamedPipe for %s failed with error: %d\n", pipename, GetLastError());
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }

cleanup:
    CsrPmemFree(pipename);
    CsrPmemFree(cmdline);

    if ((hPipe != INVALID_HANDLE_VALUE) && process_launched && client_connected)
    {
        HANDLE *hPipePtr;

        ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
        ltHdl->write = csrLogTransportWSPipeWrite;
        ltHdl->alloc = csrLogTransportWSPipeAlloc;
        ltHdl->free = csrLogTransportWSPipeFree;

        hPipePtr = CsrPmemAlloc(sizeof(*hPipePtr));
        *hPipePtr = hPipe;
        ltHdl->arg = hPipePtr;
    }
    else
    {
        ltHdl = NULL;
    }

    return ltHdl;
}
