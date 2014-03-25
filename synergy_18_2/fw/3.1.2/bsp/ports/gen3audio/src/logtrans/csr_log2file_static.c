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
#include "nucleus.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_file.h"
#include "csr_logtransport.h"

#include "platform/csr_interrupt.h"
#include "platform/csr_logtransport_init.h"

typedef struct
{
    CsrUint8      *buf;
    INT            bufMutex;
    CsrFileHandle *fileHdl;
} csrLogTransportFileStatic;

#define TAKE_MUTEX() hdlPtr->bufMutex = NU_Local_Control_Interrupts(NU_DISABLE_INTERRUPTS)
#define RELEASE_MUTEX() hdlPtr->bufMutex = NU_Local_Control_Interrupts(hdlPtr->bufMutex)

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

static CsrSize csrLogTransportFileStaticWrite(void *arg, CsrLogContext context,
    void *data, CsrSize length)
{
    csrLogTransportFileStatic *hdlPtr;
    CsrSize numberOfBytesWritten = 0;

    hdlPtr = (csrLogTransportFileStatic *) arg;

    CSR_UNUSED(context);

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileWrite(data, length, hdlPtr->fileHdl, &numberOfBytesWritten);
        /* panic - we can't write anymore */
        if (numberOfBytesWritten != length)
        {
            internalLogTransportClose(hdlPtr->fileHdl);
        }
    }
    RELEASE_MUTEX();

    return numberOfBytesWritten;
}

static void *csrLogTransportFileStaticAlloc(void *arg, CsrSize size)
{
    csrLogTransportFileStatic *hdlPtr = (csrLogTransportFileStatic *) arg;

    TAKE_MUTEX();
    return hdlPtr->buf;
}

void *CsrLogTransportFileStaticOpen(const char *logFileName, CsrUint32 bufSize)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = csrLogTransportFileStaticWrite;
    ltHdl->alloc = csrLogTransportFileStaticAlloc;
    ltHdl->free = NULL;

    hdlPtr = CsrPmemAlloc(sizeof(*hdlPtr));
    hdlPtr->buf = CsrPmemAlloc(bufSize);
    ltHdl->arg = hdlPtr;

    (void) CsrFileOpen(&hdlPtr->fileHdl, (CsrUtf8String *) logFileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    INT_ATOMIC_EXECUTE_TAIL();

    return ltHdl;
}

void CsrLogTransportFileStaticClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    internalLogTransportClose(hdlPtr->fileHdl);
    INT_ATOMIC_EXECUTE_TAIL();

    CsrPmemFree(ltHdl->arg);
    CsrPmemFree(ltHdl);
}

/*
 * Internal flushing API for test purposes only.
 */

CsrUint32 CsrLogTransportFileStaticGetFlushMark(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;
    CsrUint32 retVal = 0;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileTell(hdlPtr->fileHdl, &retVal);
    }
    INT_ATOMIC_EXECUTE_TAIL();
    return retVal;
}

#define BLOCK_SIZE (100 * 1024)
void CsrLogTransportFileStaticFlushBetweenMarks(void *arg, CsrUint32 fromMark, CsrUint32 toMark)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;
    CsrUint8 *tmpData = NULL;
    CsrSize readBytes, writtenBytes;
    CsrUint32 restBytes;
    CsrUint32 eofMark;
    CsrUint32 blobSize;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        CsrFileTell(hdlPtr->fileHdl, &eofMark);

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
            CsrFileSeek(hdlPtr->fileHdl, toMark, CSR_SEEK_SET);
            CsrFileRead(tmpData, blobSize, hdlPtr->fileHdl, &readBytes);
            if (readBytes != blobSize)
            {
                printf("Error during read %u bytes (read %u)\n",
                    blobSize, readBytes);
            }
            else
            {
                /* Do nothing */
            }

            CsrFileSeek(hdlPtr->fileHdl, fromMark, CSR_SEEK_SET);
            CsrFileWrite(tmpData, blobSize, hdlPtr->fileHdl, &writtenBytes);
            CsrFileFlush(hdlPtr->fileHdl);
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
            CsrFileWrite(tmpData, blobSize, hdlPtr->fileHdl, &writtenBytes);
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* set fpointer to location of next write */
        CsrFileSeek(hdlPtr->fileHdl, fromMark, CSR_SEEK_SET);

        CsrFileSetEndOfFile(hdlPtr->fileHdl);

        CsrPmemFree(tmpData);
        tmpData = NULL;
    }
    INT_ATOMIC_EXECUTE_TAIL();
}

int CsrLogTransportFileStaticSetToMark(void *arg, CsrUint32 flushMark)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;
    int retVal = -1;

    INT_ATOMIC_EXECUTE_HEAD();

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        retVal = CsrFileSeek(hdlPtr->fileHdl, flushMark, CSR_SEEK_SET);
    }
    INT_ATOMIC_EXECUTE_TAIL();
    return retVal;
}

#endif
