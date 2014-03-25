#ifndef CSR_BT_CSR_BT_ISO8601_H__
#define CSR_BT_CSR_BT_ISO8601_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_ISO8601DATE_MAX_LENGTH                 (12)
#define CSR_BT_ISO8601TIME_MAX_LENGTH                 (14)

typedef struct
{
    CsrInt32  year;
    CsrUint8  month;
    CsrUint8  day;
} CsrBtIso8601Date;

#define CSR_BT_TIME_FLAGS_HOUR_VALID                  (0x01)
#define CSR_BT_TIME_FLAGS_MINUTE_VALID                (0x02)
#define CSR_BT_TIME_FLAGS_SECOND_VALID                (0x04)
#define CSR_BT_TIME_FLAGS_HOUR_FACTION                (0x08)
#define CSR_BT_TIME_FLAGS_MINUTE_FACTION              (0x10)
#define CSR_BT_TIME_FLAGS_SECOND_FACTION              (0x20)
#define CSR_BT_TIME_FLAGS_UTC_TIME                    (0x40)
#define CSR_BT_TIME_FLAGS_TIMEZONE_VALID              (0x80)

typedef struct
{
    CsrUint8  hour;
    CsrUint8  minute;
    CsrUint8  second;
    CsrUint8  fraction;
    CsrInt8   timezoneHour;
    CsrUint8  timezoneMinute;
    CsrUint8  flags;
} CsrBtIso8601Time;

typedef struct
{
    CsrBtIso8601Date date;
    CsrBtIso8601Time time;
} CsrBtIso8601DateTime;

#define CSR_BT_ISO8601_FLAGS_EXPANDED_YEAR            (0x0001)
#define CSR_BT_ISO8601_FLAGS_EXTENDED                 (0x0002)
#define CSR_BT_ISO8601_FLAGS_ALLOW_REDUCED_ACCURACY   (0x0004)
#define CSR_BT_ISO8601_FLAGS_ALLOW_HOUR_FACTION       (0x0008)
#define CSR_BT_ISO8601_FLAGS_ALLOW_MINUTE_FACTION     (0x0010)
#define CSR_BT_ISO8601_FLAGS_ALLOW_SECOND_FACTION     (0x0020)

typedef CsrUint8 CsrBtIso8601Flags;

CsrBool Iso8601DateToCsrBtDate(CsrCharString* iso8601String, CsrBtIso8601Date* iso8601Date, CsrBtIso8601Flags flags);
CsrBool Iso8601DateToCsrBtTime(CsrCharString* iso8601String, CsrBtIso8601Time* iso8601Time, CsrBtIso8601Flags flags);
CsrBool Iso8601DateToCsrBtDateTime(CsrCharString* iso8601String, CsrBtIso8601DateTime* iso8601DateTime, CsrBtIso8601Flags flags);
CsrBool CsrBtDateToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601Date* iso8601Date, CsrBtIso8601Flags flags);
CsrBool CsrBtTimeToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601Time* iso8601Time, CsrBtIso8601Flags flags);
CsrBool CsrBtDateTimeToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601DateTime* iso8601DateTime, CsrBtIso8601Flags flags);

#ifdef __cplusplus
}
#endif

#endif
