#ifndef CSR_LOGTRANSPORT_INIT_H__
#define CSR_LOGTRANSPORT_INIT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_logtransport.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CSR_LOGTRANSPORT_HAVE_FILE
#define CSR_LOGTRANSPORT_HAVE_FILE
#endif

#ifndef CSR_LOGTRANSPORT_HAVE_STDOUT
#define CSR_LOGTRANSPORT_HAVE_STDOUT
#endif

#ifndef CSR_LOGTRANSPORT_HAVE_FTSPIPE
#define CSR_LOGTRANSPORT_HAVE_FTSPIPE
#endif

#ifndef CSR_LOGTRANSPORT_HAVE_WSPIPE
#define CSR_LOGTRANSPORT_HAVE_WSPIPE
#endif

typedef struct
{
    CsrSize (*write)(void *, CsrLogContext, void *, CsrSize);
    void * (*alloc)(void *, CsrSize);
    void (*free)(void *, void *);
    void *arg;
} csrLogTransportHandle;

/*
 * File log transport API
 */

/* initialize internal stuff needed for transportation of log information
 * return log transport handle on success
 */
void *CsrLogTransportFileOpen(const char *fileName);
void CsrLogTransportFileClose(void *arg);

void *CsrLogTransportFileAsyncOpen(const char *fileName, CsrUint32 bufSize);
void CsrLogTransportFileAsyncClose(void *arg);

/*
 * Stdout log transport API
 */
void *CsrLogTransportStdoutOpen(void);
void CsrLogTransportStdoutClose(void *arg);

/*
 * FTS pipe log transport API
 */
void *CsrLogTransportFtsPipeOpen(const char *fts_dir);
void CsrLogTransportFtsPipeClose(void *arg);

/*
 * WS pipe log transport API
 */
void *CsrLogTransportWSPipeOpen(const char *ws_exec, const char *ws_pipe);
void CsrLogTransportWSPipeClose(void *arg);

/*
 * Internal flushing API for test purposes only.
 */

/* get mark for later usage for 'logTransportFlushBetweenMarks'
 * note: you must have obtained two marks before call logTransportFlushBetweenMarks.
 * note: this functionality give god sense for the file transport interfaces, but not
 * all other interfaces
 */
CsrUint32 CsrLogTransportFileGetFlushMark(void *arg);

/* flush all data between the marks
 * note: if fromMark is zero - the log will be cleared until 'toMark'
 * note: this feature is implemented for file transport but NOT possible for network
 */
void CsrLogTransportFileFlushBetweenMarks(void *arg, CsrUint32 fromMark, CsrUint32 toMark);

/* Set the output to the mark given
 * return 0 on success
 * note: see fseek
 * note: not supported on all transport
 */
int CsrLogTransportFileSetToMark(void *arg, CsrUint32 flushMark);


#ifdef __cplusplus
}
#endif

#endif
