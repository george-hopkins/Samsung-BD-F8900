/*!
        (c) CSR plc 2010
        All rights reserved

\file   sm_rand_offchip.c

\brief  Random number handling for Synergy.
*/

#include "csr_synergy.h"
#include "sm_private.h"

#ifdef INSTALL_SM_MODULE
#ifdef SM_HAS_FUNCTION_FOR_RAND

#include "csr_random.h"

void random_number(CsrUint16 output[], CsrUint16 outputlen)
{
    void *ptr = CsrRandomSeed();
    for ( ; outputlen != 0 ; --outputlen)
    {
        CsrUint32 random = CsrRandom(ptr);
        output[outputlen-1] = (CsrUint16) random;
    }
    CsrPmemFree(ptr);
}

#endif /* SM_HAS_FUNCTION_FOR_RAND */
#endif /* INSTALL_SM_MODULE */
