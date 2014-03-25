/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdlib.h>

#include "csr_panic.h"
#include "platform/csr_panic_init.h"

static CsrPanicHandler panic = NULL;

void CsrPanicInit(CsrPanicHandler cb)
{
    panic = cb;
}

void CsrPanic(CsrUint8 tech, CsrUint16 reason, const char *p)
{
    if (panic)
    {
        panic(tech, reason, p);
    }
#ifndef CSR_BT_APP_SAMSUNG	
    exit(1);
#endif	
}
