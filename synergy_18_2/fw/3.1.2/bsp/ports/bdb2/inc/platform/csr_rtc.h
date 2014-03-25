#ifndef CSR_RTC_H__
#define CSR_RTC_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrUint8 tm_sec;     /* seconds after the minute - [0,59]       */
    CsrUint8 tm_min;     /* minutes after the hour - [0,59]         */
    CsrUint8 tm_hour;    /* hours since midnight - [0,23]           */
    CsrUint8 tm_mday;    /* day of the month - [1,31]               */
    CsrUint8 tm_mon;     /* months since January - [0,11]           */
    CsrUint8 tm_year;    /* years since 2000                        */
    CsrUint8 tm_wday;    /* days since Sunday - [0,6]               */
} TimeStructure;

/* Set/read RTC */
CsrBool RtcTimeSet(const TimeStructure *now);
CsrBool RtcTimeGet(TimeStructure *now);

#ifdef __cplusplus
}
#endif

#endif
