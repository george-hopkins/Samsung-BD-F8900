/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_LOG_ENABLE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_file.h"
#include "csr_logtransport.h"

#include "platform/csr_interrupt.h"
#include "platform/csr_logtransport_init.h"

/* internal/static method to close the file stream.
 * Note: implemented to avoid dead lock on MUTEX from internal funcs.
 */
static void internalLogTransportClose(CsrFileHandle *hdlPtr)
{
    if (hdlPtr != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileFlush(hdlPtr);
        CsrFileClose(hdlPtr);
    }
}

static CsrSize csrLogTransportWriteFile(void *arg, CsrLogContext context,
    void *data, CsrSize length)
{
    CsrFileHandle *hdlPtr;
    CsrSize numberOfBytesWritten = 0;

    hdlPtr = (CsrFileHandle *) arg;

    CSR_UNUSED(context);

    if (hdlPtr != CSR_FILE_INVALID_HANDLE)
    {
        INT_ATOMIC_EXECUTE_HEAD();

        CsrFileWrite(data, length, hdlPtr, &numberOfBytesWritten);
        /* panic - we can't write anymore */
        if (numberOfBytesWritten != length)
        {
            internalLogTransportClose(hdlPtr);
        }

        INT_ATOMIC_EXECUTE_TAIL();
    }

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
    CsrFileHandle *hdlPtr;

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = csrLogTransportWriteFile;
    ltHdl->alloc = csrLogTransportFileAlloc;
    ltHdl->free = csrLogTransportFileFree;

    hdlPtr = CsrPmemAlloc(sizeof(CsrFileHandle *));

    (void) CsrFileOpen(&hdlPtr, (CsrUtf8String *) logFileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    ltHdl->arg = hdlPtr;

    return ltHdl;
}

void CsrLogTransportFileClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    CsrFileHandle *hdlPtr;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (CsrFileHandle *) ltHdl->arg;

    internalLogTransportClose(hdlPtr);

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

/*
 * Internal flushing API for test purposes only.
 */

CsrUint32 CsrLogTransportFileGetFlushMark(void *arg)
{
    csrLogTransportHandle *ltHdl;
    CsrFileHandle *hdlPtr;
    CsrUint32 retVal = 0;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (CsrFileHandle *) ltHdl->arg;

    if (hdlPtr != CSR_FILE_INVALID_HANDLE)
    {
        INT_ATOMIC_EXECUTE_HEAD();
        CsrFileTell(hdlPtr, &retVal);
        INT_ATOMIC_EXECUTE_TAIL();
    }
    return retVal;
}

#define BLOCK_SIZE (100 * 1024)
void CsrLogTransportFileFlushBetweenMarks(void *arg, CsrUint32 fromMark, CsrUint32 toMark)
{
    csrLogTransportHandle *ltHdl;
    CsrFileHandle *hdlPtr;
    CsrUint8 *tmpData = NULL;
    CsrSize readBytes, writtenBytes;
    CsrUint32 restBytes;
    CsrUint32 eofMark;
    CsrUint32 blobSize;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (CsrFileHandle *) ltHdl->arg;

    tmpData = (CsrUint8 *) CsrPmemAlloc(BLOCK_SIZE);

    if (hdlPtr != CSR_FILE_INVALID_HANDLE)
    {
        INT_ATOMIC_EXECUTE_HEAD();

        CsrFileTell(hdlPtr, &eofMark);

        /*Calculate number of bytes to move */
        restBytes = eofMark - toMark;

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
            CsrFileSeek(hdlPtr, toMark, CSR_SEEK_SET);
            CsrFileRead(tmpData, blobSize, hdlPtr, &readBytes);
            if (readBytes != blobSize)
            {
                printf("Error during read %u bytes (read %u)\n",
                    blobSize, readBytes);
            }
            else
            {
                /* Do nothing */
            }

            CsrFileSeek(hdlPtr, fromMark, CSR_SEEK_SET);
            CsrFileWrite(tmpData, blobSize, hdlPtr, &writtenBytes);
            CsrFileFlush(hdlPtr);
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
            CsrFileWrite(tmpData, blobSize, hdlPtr, &writtenBytes);
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* set fpointer to location of next write */
        CsrFileSeek(hdlPtr, fromMark, CSR_SEEK_SET);

        CsrFileSetEndOfFile(hdlPtr);


        INT_ATOMIC_EXECUTE_TAIL();
    }

    CsrPmemFree(tmpData);
    tmpData = NULL;
}

int CsrLogTransportFileSetToMark(void *arg, CsrUint32 flushMark)
{
    csrLogTransportHandle *ltHdl;
    CsrFileHandle *hdlPtr;
    int retVal = -1;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (CsrFileHandle *) ltHdl->arg;

    if (hdlPtr != CSR_FILE_INVALID_HANDLE)
    {
        INT_ATOMIC_EXECUTE_HEAD();
        retVal = CsrFileSeek(hdlPtr, flushMark, CSR_SEEK_SET);
        INT_ATOMIC_EXECUTE_TAIL();
    }
    return retVal;
}

#endif
