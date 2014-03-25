/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_time.h"

#include "platform/csr_timer.h"
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrTimeGet
 *
 *  DESCRIPTION
 *      Return the current microseconds system time in two 32bit high/low parts.
 *      The microseconds low part is the function return value. The high part is
 *      incremented on low part wraps and returned as output parameter 'high'.
 *
 *  NOTE
 *      A NULL pointer may be provided as 'high' parameter. In this case the
 *      function just returns the low part and leaves the 'high' parameter
 *      unchanged.
 *
 *  RETURNS
 *      CsrTime - the 32 bit low part system time in microseconds.
 *----------------------------------------------------------------------------*/
CsrTime CsrTimeGet(CsrTime *high)
{
    CsrTime realNow;
    realNow = GetTime(high);
    return realNow;
}
