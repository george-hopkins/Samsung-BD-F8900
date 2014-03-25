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
#include "csr_logtransport.h"

#include "platform/csr_logtransport_init.h"

void *CsrLogTransportAlloc(void *arg, CsrSize sz)
{
    csrLogTransportHandle *ltHdl;

    ltHdl = (csrLogTransportHandle *) arg;

    return ltHdl->alloc(ltHdl->arg, sz);
}

CsrSize CsrLogTransportWrite(void *arg, CsrLogContext context,
    void *data, CsrSize length)
{
    CsrSize numberOfBytesWritten;
    csrLogTransportHandle *ltHdl;

    ltHdl = (csrLogTransportHandle *) arg;

    CSR_UNUSED(context);

    numberOfBytesWritten = ltHdl->write(ltHdl->arg, context, data, length);

    if (ltHdl->free)
    {
        ltHdl->free(ltHdl->arg, data);
    }

    return numberOfBytesWritten;
}
