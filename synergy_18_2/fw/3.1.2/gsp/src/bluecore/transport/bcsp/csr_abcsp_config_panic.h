#ifndef CSR_ABCSP_CONFIG_PANIC_H__
#define CSR_ABCSP_CONFIG_PANIC_H__

/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_panic.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
NAME
        ABCSP_PANIC  -  report an panic from the abcsp code

SYNOPSIS
        void ABCSP_PANIC(CsrUint32 e);

FUNCTION
    Reports the occurrence of the panic "e".   Values for "e" are given
    in abcsp_panics.h.

NOTE

*/

#define ABCSP_PANIC(n, l)    CsrPanic(CSR_TECH_FW, n, l);

#ifdef __cplusplus
}
#endif

#endif
