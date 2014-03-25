/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include "csr_types.h"
#include "csr_time.h"

static CsrUint64 getTime(void)
{
    SYSTEMTIME sysTime;
    FILETIME fileTime;
    CsrUint64 time;

    GetSystemTime(&sysTime);
    SystemTimeToFileTime(&sysTime, &fileTime);

    time = ((CsrUint64) fileTime.dwLowDateTime) | ((CsrUint64) fileTime.dwHighDateTime << 32);
    time -= 0x19DB1DED53E8000; /* convert to 1970-01-01 00:00 (UTC) */
    time /= 10; /* to microseconds */

    return time;
}

CsrTime CsrTimeGet(CsrTime *high)
{
    CsrUint64 time = getTime();

    if (high != NULL)
    {
        *high = (CsrTime) ((time >> 32) & 0xFFFFFFFF);
    }

    return (CsrTime) (time & 0xFFFFFFFF);
}

void CsrTimeUtcGet(CsrTimeUtc *tod, CsrTime *low, CsrTime *high)
{
    CsrUint64 time = getTime();

    if (high != NULL)
    {
        *high = (CsrTime) ((time >> 32) & 0xFFFFFFFF);
    }

    if (low != NULL)
    {
        *low = (CsrTime) (time & 0xFFFFFFFF);
    }

    if (tod != NULL)
    {
        tod->sec = (CsrUint32) (time / ((CsrUint64) 1000000));
        tod->msec = (CsrUint16) ((time / ((CsrUint64) 1000)) % ((CsrUint64) 1000));
    }
}
