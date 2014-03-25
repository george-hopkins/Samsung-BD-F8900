/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_LOG_ENABLE

#include <stdio.h>
#include <nucleus.h>

#include "csr_macro.h"
#include "csr_file.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_logtransport.h"
#include "platform/csr_logtransport_init.h"

typedef struct
{
    CsrUint8      *buf;
    CsrSize        bufSize;
    INT            intrLevel;
    CsrFileHandle *fileHdl;
} csrLogTransportFileStatic;

#define TAKE_MUTEX() hdlPtr->intrLevel = NU_Control_Interrupts(NU_DISABLE_INTERRUPTS)
#define RELEASE_MUTEX() NU_Control_Interrupts(hdlPtr->intrLevel)

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

    CSR_UNUSED(context);

    hdlPtr = (csrLogTransportFileStatic *) arg;

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

    ltHdl = CsrPmemAlloc(sizeof(*ltHdl));
    ltHdl->write = csrLogTransportFileStaticWrite;
    ltHdl->alloc = csrLogTransportFileStaticAlloc;
    ltHdl->free = NULL;

    hdlPtr = CsrPmemAlloc(sizeof(*hdlPtr));
    hdlPtr->bufSize = bufSize;
    hdlPtr->buf = CsrPmemAlloc(hdlPtr->bufSize);
    ltHdl->arg = hdlPtr;

    (void) CsrFileOpen(&hdlPtr->fileHdl, (CsrUtf8String *) logFileName,
        CSR_FILE_OPEN_FLAGS_CREATE | CSR_FILE_OPEN_FLAGS_READ_WRITE | CSR_FILE_OPEN_FLAGS_TRUNCATE,
        CSR_FILE_PERMS_USER_READ | CSR_FILE_PERMS_USER_WRITE);

    return ltHdl;
}

void CsrLogTransportFileStaticClose(void *arg)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    internalLogTransportClose(hdlPtr->fileHdl);

    CsrPmemFree(hdlPtr->buf);
    CsrPmemFree(hdlPtr);
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

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        TAKE_MUTEX();
        CsrFileTell(hdlPtr->fileHdl, &retVal);
        RELEASE_MUTEX();
    }
    return retVal;
}

void CsrLogTransportFileStaticFlushBetweenMarks(void *arg, CsrUint32 fromMark, CsrUint32 toMark)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;
    CsrSize readBytes, writtenBytes;
    CsrUint32 restBytes;
    CsrUint32 eofMark;
    CsrUint32 blobSize;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        TAKE_MUTEX();

        CsrFileTell(hdlPtr->fileHdl, &eofMark);

        /*Calculate number of bytes to move */
        restBytes = eofMark - toMark;

        /* move data */
        do
        {
            blobSize = CSRMIN(restBytes, hdlPtr->bufSize);

            CsrFileSeek(hdlPtr->fileHdl, toMark, CSR_SEEK_SET);
            CsrFileRead(hdlPtr->buf, blobSize, hdlPtr->fileHdl, &readBytes);
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
            CsrFileWrite(hdlPtr->buf, blobSize, hdlPtr->fileHdl, &writtenBytes);
            CsrFileFlush(hdlPtr->fileHdl);
            fromMark += blobSize;
            toMark += blobSize;
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* zero until eof file */
        CsrMemSet(hdlPtr->buf, 0, hdlPtr->bufSize);

        restBytes = eofMark - fromMark; /* fromMark currently contain position for next write */
        do
        {
            blobSize = CSRMIN(restBytes, hdlPtr->bufSize);

            CsrFileWrite(hdlPtr->buf, blobSize, hdlPtr->fileHdl, &writtenBytes);
            restBytes -= blobSize;
        } while (restBytes > 0);

        /* set fpointer to location of next write */
        CsrFileSeek(hdlPtr->fileHdl, fromMark, CSR_SEEK_SET);

        CsrFileSetEndOfFile(hdlPtr->fileHdl);

        RELEASE_MUTEX();
    }
}

int CsrLogTransportFileStaticSetToMark(void *arg, CsrUint32 flushMark)
{
    csrLogTransportHandle *ltHdl;
    csrLogTransportFileStatic *hdlPtr;
    int retVal = -1;

    ltHdl = (csrLogTransportHandle *) arg;
    hdlPtr = (csrLogTransportFileStatic *) ltHdl->arg;

    if (hdlPtr->fileHdl != CSR_FILE_INVALID_HANDLE)
    {
        TAKE_MUTEX();
        retVal = CsrFileSeek(hdlPtr->fileHdl, flushMark, CSR_SEEK_SET);
        RELEASE_MUTEX();
    }
    return retVal;
}

#endif
