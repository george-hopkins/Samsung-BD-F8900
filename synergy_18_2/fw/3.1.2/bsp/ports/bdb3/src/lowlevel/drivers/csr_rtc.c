/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "platform/csr_rtc.h"
#include "platform/csr_s3c6410.h"

CsrBool RtcTimeSet(const TimeStructure *now)
{
    CsrBool rtcEnabled;

    if (now)
    {
        unsigned char tmpByte;

        BIT_SET(rRTCCON, 0);

        tmpByte = ((now->tm_year / 10) << 4) | (now->tm_year % 10);
        rBCDYEAR = tmpByte;

        tmpByte = (((now->tm_mon + 1) / 10) << 4) | ((now->tm_mon + 1) % 10);
        rBCDMON = tmpByte;

        tmpByte = ((now->tm_mday / 10) << 4) | (now->tm_mday % 10);
        rBCDDATE = tmpByte;

        tmpByte = now->tm_wday + 1;
        rBCDDAY = tmpByte;

        tmpByte = ((now->tm_hour / 10) << 4) | (now->tm_hour % 10);
        rBCDHOUR = tmpByte;

        tmpByte = ((now->tm_min / 10) << 4) | (now->tm_min % 10);
        rBCDMIN = tmpByte;

        tmpByte = ((now->tm_sec / 10) << 4) | (now->tm_sec % 10);
        rBCDSEC = tmpByte;

        rtcEnabled = TRUE;

        BIT_CLEAR(rRTCCON, 0);
    }
    else
    {
        rtcEnabled = FALSE;
    }

    return rtcEnabled;
}

CsrBool RtcTimeGet(TimeStructure *now)
{
    unsigned char tmpByte;

    if (now)
    {
        tmpByte = rBCDYEAR;
        now->tm_year = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

        tmpByte = rBCDMON;
        now->tm_mon = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F)) - 1;

        tmpByte = rBCDDATE;
        now->tm_mday = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

        tmpByte = rBCDDAY;
        now->tm_wday = tmpByte - 1;

        tmpByte = rBCDHOUR;
        now->tm_hour = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

        tmpByte = rBCDMIN;
        now->tm_min = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

        tmpByte = rBCDSEC;
        now->tm_sec = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

        if (now->tm_sec == 0)
        {
            /* safe-guard against roll-over in which case we restart */
            tmpByte = rBCDYEAR;
            now->tm_year = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

            tmpByte = rBCDMON;
            now->tm_mon = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F)) - 1;

            tmpByte = rBCDDATE;
            now->tm_mday = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

            tmpByte = rBCDDAY;
            now->tm_wday = tmpByte - 1;

            tmpByte = rBCDHOUR;
            now->tm_hour = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));

            tmpByte = rBCDMIN;
            now->tm_min = (((tmpByte >> 4) * 10) + (tmpByte & 0x0F));
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
