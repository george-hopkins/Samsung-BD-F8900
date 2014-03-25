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

#ifdef __cplusplus
}
#endif

#endif
