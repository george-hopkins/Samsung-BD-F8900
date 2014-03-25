/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_file.h"
#include "csr_logtransport.h"

#include <windows.h>
#include "platform/csr_logtransport_init.h"

typedef struct
{
    CRITICAL_SECTION logMutex;
    CsrFileHandle   *hdl;
} csrLogTransportFile;


/*
 * File log transport
 */

static void csrLogTransportFileCloseInternal(csrLogTransportFile *fHdl)
{
    if (fHdl->hdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileFlush(fHdl->hdl);
        CsrFileClose(fHdl->hdl);
    }
}

static CsrSize csrLogTransportFileWrite(void *arg, CsrLogContext context,
    void *data, CsrSize len)
{
    csrLogTransportFile *fHdl;
    CsrSize numberOfBytesWritten = 0;

    CSR_UNUSED(context);

    fHdl = (csrLogTransportFile *) arg;

    EnterCriticalSection(&fHdl->logMutex);

    if (fHdl->hdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileWrite(data, len, fHdl->hdl, &numberOfBytesWritten);
        /* panic - we can't write anymore */
        if (numberOfBytesWritten != len)
        {
            csrLogTransportFileCloseInternal(fHdl);
            fHdl->hdl = CSR_FILE_INVALID_HANDLE;
        }
    }

    LeaveCriticalSection(&fHdl->logMutex);

    return numberOfBytesWritten;
}

static void *csrLogTransportFileAlloc(void *arg, CsrSize size)
{
    return CsrPmemAlloc(size);
}

static void csrLogTransportFileFree(void *arg, void *data)
{
    CsrPmemFree(data);
}

void *CsrLogTransportFileOpen(const char *logFileName)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFile *fHdl;
    CsrResult res;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));

    fHdl = CsrPmemAlloc(sizeof(*fHdl));

    InitializeCriticalSection(&fHdl->logMutex);

    res = CsrFileOpen(&fHdl->hdl, (CsrUtf8String *) logFileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    if (res == CSR_RESULT_SUCCESS)
    {
        ltHdl->arg = fHdl;

        ltHdl->write = csrLogTransportFileWrite;
        ltHdl->alloc = csrLogTransportFileAlloc;
        ltHdl->free = csrLogTransportFileFree;

        return ltHdl;
    }
    else
    {
        CsrPmemFree(ltHdl);
        CsrPmemFree(fHdl);
        return NULL;
    }
}

void CsrLogTransportFileClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFile *fHdl;

    ltHdl = (csrLogTransportHandle *) arg;
    fHdl = (csrLogTransportFile *) ltHdl->arg;

    csrLogTransportFileCloseInternal(fHdl);

    DeleteCriticalSection(&fHdl->logMutex);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

/*
 * Internal flush API for test purposes only.
 */

CsrUint32 CsrLogTransportFileGetFlushMark(void *arg)
{
    CsrUint32 retVal = 0;
    csrLogTransportHandle *ltHdl;
    csrLogTransportFile *fHdl;

    ltHdl = (csrLogTransportHandle *) arg;

    fHdl = (csrLogTransportFile *) ltHdl->arg;

    EnterCriticalSection(&fHdl->logMutex);

    if (fHdl->hdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileTell(fHdl->hdl, &retVal);
    }

    LeaveCriticalSection(&fHdl->logMutex);

    return retVal;
}

#define BLOCK_SIZE (100 * 1024)
void CsrLogTransportFileFlushBetweenMarks(void *arg, CsrUint32 fromMark, CsrUint32 toMark)
{
    CsrUint8 *tmpData = NULL;
    CsrSize readBytes, writtenBytes;
    CsrUint32 restBytes;
    CsrUint32 eofMark;
    CsrUint32 blobSize;
    csrLogTransportHandle *ltHdl;
    csrLogTransportFile *fHdl;

    ltHdl = (csrLogTransportHandle *) arg;

    fHdl = (csrLogTransportFile *) ltHdl->arg;

    EnterCriticalSection(&fHdl->logMutex);

    if (fHdl->hdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileTell(fHdl->hdl, &eofMark);

        /*Calculate number of bytes to move */
        restBytes = eofMark - toMark;

        tmpData = (CsrUint8 *) CsrPmemAlloc(BLOCK_SIZE);

        /* move data */
        do
        {
            if (restBytes < BLOCK_SIZE)
            {
                blobSize = restBytes;
            }
            else
            {
                blobSize = BLOCK_SIZE;
            }
            CsrFileSeek(fHdl->hdl, toMark, CSR_SEEK_SET);
            CsrFileRead(tmpData, blobSize, fHdl->hdl, &readBytes);
            if (readBytes != blobSize)
            {
                printf("Error during read %lu bytes (read %lu)\n",
                    blobSize, readBytes);
            }
            else
            {
                /* Do nothing */
            }

            CsrFileSeek(fHdl->hdl, fromMark, CSR_SEEK_SET);
            CsrFileWrite(tmpData, blobSize, fHdl->hdl, &writtenBytes);
            CsrFileFlush(fHdl->hdl);
            fromMark += blobSize;
            toMark += blobSize;
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* zero until eof file */
        CsrMemSet(tmpData, 0, BLOCK_SIZE);

        restBytes = eofMark - fromMark; /* fromMark currently contain position for next write */
        do
        {
            if (restBytes < BLOCK_SIZE)
            {
                blobSize = restBytes;
            }
            else
            {
                blobSize = BLOCK_SIZE;
            }
            CsrFileWrite(tmpData, blobSize, fHdl->hdl, &writtenBytes);
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* set fpointer to location of next write */
        CsrFileSeek(fHdl->hdl, fromMark, CSR_SEEK_SET);

        CsrFileSetEndOfFile(fHdl->hdl);

        CsrPmemFree(tmpData);
        tmpData = NULL;
    }
    LeaveCriticalSection(&fHdl->logMutex);
}

int CsrLogTransportFileSetToMark(void *arg, CsrUint32 flushMark)
{
    int retVal = -1;
    csrLogTransportHandle *ltHdl;
    csrLogTransportFile *fHdl;

    ltHdl = (csrLogTransportHandle *) arg;

    fHdl = (csrLogTransportFile *) ltHdl->arg;

    EnterCriticalSection(&fHdl->logMutex);

    if (fHdl->hdl != CSR_FILE_INVALID_HANDLE)
    {
        retVal = CsrFileSeek(fHdl->hdl, flushMark, CSR_SEEK_SET);
    }
    LeaveCriticalSection(&fHdl->logMutex);

    return retVal;
}
