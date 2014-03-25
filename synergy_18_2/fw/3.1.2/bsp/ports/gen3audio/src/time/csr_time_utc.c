/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_panic.h"
#include "csr_time.h"

#include "platform/csr_timer.h"
#include "platform/csr_rtc.h"


#define CSR_GET_TIME_STRINGIFY_REAL(a) #a
#define CSR_GET_TIME_STRINGIFY(a) CSR_GET_TIME_STRINGIFY_REAL(a)

#define CSR_GET_TIME_ASSERT(cond, str)              \
    do {                                                \
        if (!(cond))                                    \
        {                                               \
            char *panic_arg = "[" __FILE__ ":" CSR_GET_TIME_STRINGIFY(__LINE__) "] - " CSR_GET_TIME_STRINGIFY(cond); \
            char *panic_str = "[" __FILE__ ":" CSR_GET_TIME_STRINGIFY(__LINE__) "] - " str; \
            char *panic = CsrPmemZalloc(CsrStrLen(panic_arg) + CsrStrLen(panic_str) + 3); \
            CsrStrCat(panic, panic_arg);                                                \
            CsrStrCat(panic, "\n");                                                     \
            CsrStrCat(panic, panic_str);                                                \
            CsrStrCat(panic, "\n");                                                     \
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_ASSERTION_FAIL, panic);                  \
            CsrPmemFree(panic);                                                            \
        }                                                                               \
    } while (0)

#define CSR_MAX_YEARS_DIFF (106)  /* Seconds from 1970 wraps when year > 2106 */
static const CsrUint32 secondsUntilYear[CSR_MAX_YEARS_DIFF + 1] =
{
    946684800UL, /* 2000 */
    978307200UL, /* 2001 */
    1009843200UL, /* 2002 */
    1041379200UL, /* 2003 */
    1072915200UL, /* 2004 */
    1104537600UL, /* 2005 */
    1136073600UL, /* 2006 */
    1167609600UL, /* 2007 */
    1199145600UL, /* 2008 */
    1230768000UL, /* 2009 */
    1262304000UL, /* 2010 */
    1293840000UL, /* 2011 */
    1325376000UL, /* 2012 */
    1356998400UL, /* 2013 */
    1388534400UL, /* 2014 */
    1420070400UL, /* 2015 */
    1451606400UL, /* 2016 */
    1483228800UL, /* 2017 */
    1514764800UL, /* 2018 */
    1546300800UL, /* 2019 */
    1577836800UL, /* 2020 */
    1609459200UL, /* 2021 */
    1640995200UL, /* 2022 */
    1672531200UL, /* 2023 */
    1704067200UL, /* 2024 */
    1735689600UL, /* 2025 */
    1767225600UL, /* 2026 */
    1798761600UL, /* 2027 */
    1830297600UL, /* 2028 */
    1861920000UL, /* 2029 */
    1893456000UL, /* 2030 */
    1924992000UL, /* 2031 */
    1956528000UL, /* 2032 */
    1988150400UL, /* 2033 */
    2019686400UL, /* 2034 */
    2051222400UL, /* 2035 */
    2082758400UL, /* 2036 */
    2114380800UL, /* 2037 */
    2145916800UL, /* 2038 */
    2177452800UL, /* 2039 */
    2208988800UL, /* 2040 */
    2240611200UL, /* 2041 */
    2272147200UL, /* 2042 */
    2303683200UL, /* 2043 */
    2335219200UL, /* 2044 */
    2366841600UL, /* 2045 */
    2398377600UL, /* 2046 */
    2429913600UL, /* 2047 */
    2461449600UL, /* 2048 */
    2493072000UL, /* 2049 */
    2524608000UL, /* 2050 */
    2556144000UL, /* 2051 */
    2587680000UL, /* 2052 */
    2619302400UL, /* 2053 */
    2650838400UL, /* 2054 */
    2682374400UL, /* 2055 */
    2713910400UL, /* 2056 */
    2745532800UL, /* 2057 */
    2777068800UL, /* 2058 */
    2808604800UL, /* 2059 */
    2840140800UL, /* 2060 */
    2871763200UL, /* 2061 */
    2903299200UL, /* 2062 */
    2934835200UL, /* 2063 */
    2966371200UL, /* 2064 */
    2997993600UL, /* 2065 */
    3029529600UL, /* 2066 */
    3061065600UL, /* 2067 */
    3092601600UL, /* 2068 */
    3124224000UL, /* 2069 */
    3155760000UL, /* 2070 */
    3187296000UL, /* 2071 */
    3218832000UL, /* 2072 */
    3250454400UL, /* 2073 */
    3281990400UL, /* 2074 */
    3313526400UL, /* 2075 */
    3345062400UL, /* 2076 */
    3376684800UL, /* 2077 */
    3408220800UL, /* 2078 */
    3439756800UL, /* 2079 */
    3471292800UL, /* 2080 */
    3502915200UL, /* 2081 */
    3534451200UL, /* 2082 */
    3565987200UL, /* 2083 */
    3597523200UL, /* 2084 */
    3629145600UL, /* 2085 */
    3660681600UL, /* 2086 */
    3692217600UL, /* 2087 */
    3723753600UL, /* 2088 */
    3755376000UL, /* 2089 */
    3786912000UL, /* 2090 */
    3818448000UL, /* 2091 */
    3849984000UL, /* 2092 */
    3881606400UL, /* 2093 */
    3913142400UL, /* 2094 */
    3944678400UL, /* 2095 */
    3976214400UL, /* 2096 */
    4007836800UL, /* 2097 */
    4039372800UL, /* 2098 */
    4070908800UL, /* 2099 */
    4102444800UL, /* 2100 */
    4133980800UL, /* 2101 */
    4165516800UL, /* 2102 */
    4197052800UL, /* 2103 */
    4228588800UL, /* 2104 */
    4260211200UL, /* 2105 */
    4291747200UL, /* 2106 */
};

#define CSR_SCHED_SECONDS_PER_DAY   (24 * 60 * 60)
#define CSR_SCHED_SECONDS_PER_HOUR  (60 * 60)
#define CSR_SCHED_SECONDS_PER_MIN   (60)

#define CSR_MAX_MONTH (11)
static const CsrUint32 secondsUntilMonth[2][CSR_MAX_MONTH + 1] =
{
    /* Non leap year */
    {
        (CSR_SCHED_SECONDS_PER_DAY) * (0),                                  /* January */
        (CSR_SCHED_SECONDS_PER_DAY) * (31),                                 /* February */
        (CSR_SCHED_SECONDS_PER_DAY) * (28 + 31),                            /* March */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 28 + 31),                       /* April */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 28 + 31),                  /* May */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 28 + 31),             /* June */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 30 + 31 + 28 + 31),        /* July */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 30 + 31 + 28 + 31),   /* August */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 31 + 30 + 31 + 30 + 31 + 28 + 31), /* September */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 31 + 30 + 31 + 30 + 31 + 28 + 31), /* October */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 + 28 + 31), /* November */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 + 28 + 31)    /* December */
    },
    /* Leap year */
    {
        (CSR_SCHED_SECONDS_PER_DAY) * (0),                                  /* January */
        (CSR_SCHED_SECONDS_PER_DAY) * (31),                                 /* February */
        (CSR_SCHED_SECONDS_PER_DAY) * (29 + 31),                            /* March */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 29 + 31),                       /* April */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 29 + 31),                  /* May */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 29 + 31),             /* June */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 30 + 31 + 29 + 31),        /* July */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 30 + 31 + 29 + 31),   /* August */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 31 + 30 + 31 + 30 + 31 + 29 + 31), /* September */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 31 + 30 + 31 + 30 + 31 + 29 + 31), /* October */
        (CSR_SCHED_SECONDS_PER_DAY) * (31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 + 29 + 31), /* November */
        (CSR_SCHED_SECONDS_PER_DAY) * (30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 + 29 + 31)    /* December */
    }
};

static CsrBool csrSchedIsLeapYear(CsrUint8 tm_year)
{
    return (tm_year % 4) == 0 && ((tm_year % 100) != 0 || (tm_year % 400) == 0);
}

static CsrUint32 csrSchedConvertRtcToUtc(TimeStructure *now)
{
    CsrUint8 yearsDiff = now->tm_year;
    CsrUint8 month = now->tm_mon;
    CsrBool isLeapYear = csrSchedIsLeapYear(now->tm_year);
    CsrUint32 seconds = 0;

    if ((yearsDiff > CSR_MAX_YEARS_DIFF) || (month > CSR_MAX_MONTH))
    {
        return 0;
    }
    seconds = secondsUntilYear[yearsDiff];
    seconds += secondsUntilMonth[isLeapYear][month];

    seconds += (now->tm_mday - 1) * CSR_SCHED_SECONDS_PER_DAY +
               now->tm_hour * CSR_SCHED_SECONDS_PER_HOUR +
               now->tm_min * CSR_SCHED_SECONDS_PER_MIN +
               now->tm_sec;

    return seconds;
}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrTimeUtcGet
 *
 *  DESCRIPTION
 *      Get the current system wallclock timestamp in UTC.
 *      Specifically, if tod is non-NULL, the contents will be set to the
 *      number of seconds (plus any fraction of a second in milliseconds)
 *      since January 1st 1970.  If low is non-NULL, the contents will be
 *      set to the low 32 bit part of the current system time in microseconds,
 *      as would have been returned by CsrTimeGet(). If high is non-NULL, the
 *      contents will be set to the high 32 bit part of the current system
 *      time, as would be returned in the high output parameter of CsrTimeGet().
 *
 *  NOTE
 *      NULL pointers may be provided for both low and high parameters.
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrTimeUtcGet(CsrTimeUtc *tod, CsrTime *low, CsrTime *high)
{
    TimeStructure now;

    if (RtcTimeGet(&now))
    {
        if (low != NULL)
        {
            *low = CsrTimeGet(high);
        }

        if (tod != NULL)
        {
            tod->sec = csrSchedConvertRtcToUtc(&now);
            tod->msec = 0;
        }
    }
}
