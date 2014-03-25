/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_iso8601.h"

static const CsrUint8 characterCompressionTable[] = {
/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
     0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  4,  1,  1,
     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  6,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  7,  1,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
     1,  1,  1,  1,  7,  1,  1,  1,  1,  1,  8,  1,  1,  1,  1,  1,
};

static const CsrUint8 nextStateTable[36][9] = {
/*   0   1   2   3   4   5   6   7 */
  {  0,  2,  3,  2,  3,  6,  2,  2,  2 },   /*  State: 0 */
  {  0,  2,  2,  2,  2,  6,  2,  2,  2 },   /*  State: 1 */
  {  0,  2,  2,  2,  2,  2,  2,  2,  2 },   /*  State: 2 */
  {  0,  2,  2,  2,  2,  4,  2,  2,  2 },   /*  State: 3 */
  {  0,  2,  2,  2,  2,  5,  2,  2,  2 },   /*  State: 4 */
  {  0,  2,  2,  2,  2,  6,  2,  2,  2 },   /*  State: 5 */
  {  0,  2,  2,  2,  2,  7,  2,  2,  2 },   /*  State: 6 */
  {  0,  2,  2,  2,  2,  8,  2,  2,  2 },   /*  State: 7 */
  {  0,  2,  2,  2,  2,  9,  2,  2,  2 },   /*  State: 8 */
  {  0,  2,  2,  2, 10, 11,  2, 16,  2 },   /*  State: 9 */
  {  0,  2,  2,  2,  2, 11,  2,  2,  2 },   /*  State: 10 */
  {  0,  2,  2,  2,  2, 12,  2,  2,  2 },   /*  State: 11 */
  {  0,  2,  2,  2, 13, 14,  2, 16,  2 },   /*  State: 12 */
  {  0,  2,  2,  2,  2, 14,  2,  2,  2 },   /*  State: 13 */
  {  0,  2,  2,  2,  2, 15,  2,  2,  2 },   /*  State: 14 */
  {  0,  2,  2,  2,  2,  2,  2, 16,  2 },   /*  State: 15 */
  {  0,  2,  2,  2,  2, 17,  2,  2,  2 },   /*  State: 16 */
  {  0,  2,  2,  2,  2, 18,  2,  2,  2 },   /*  State: 17 */
  {  0,  2,  2, 32,  2, 20, 19,  2, 25 },   /*  State: 18 */
  {  0,  2,  2,  2,  2, 20,  2,  2,  2 },   /*  State: 19 */
  {  0,  2,  2,  2,  2, 21,  2,  2,  2 },   /*  State: 20 */
  {  0,  2,  2, 33,  2, 23, 22,  2, 25 },   /*  State: 21 */
  {  0,  2,  2,  2,  2, 23,  2,  2,  2 },   /*  State: 22 */
  {  0,  2,  2,  2,  2, 24,  2,  2,  2 },   /*  State: 23 */
  {  0,  2, 26, 34, 26,  2,  2,  2, 25  },  /*  State: 24 */
  {  0,  2,  2,  2,  2,  2,  2,  2,  2  },  /*  State: 25 */
  {  0,  2,  2,  2,  2, 27,  2,  2,  2  },  /*  State: 26 */
  {  0,  2,  2,  2,  2, 28,  2,  2,  2  },  /*  State: 27 */
  {  0,  2,  2,  2,  2, 30, 29,  2,  2  },  /*  State: 28 */
  {  0,  2,  2,  2,  2, 30,  2,  2,  2  },  /*  State: 29 */
  {  0,  2,  2,  2,  2, 31,  2,  2,  2  },  /*  State: 30 */
  {  0,  2,  2,  2,  2,  2,  2,  2,  2  },  /*  State: 31 */
  {  0,  2,  2,  2,  2, 35,  2,  2,  2  },  /*  State: 32 */
  {  0,  2,  2,  2,  2, 35,  2,  2,  2  },  /*  State: 33 */
  {  0,  2,  2,  2,  2, 35,  2,  2,  2  },  /*  State: 34 */
  {  0,  2,  2,  2,  2,  2,  2,  2,  2  },  /*  State: 35 */
};

static const CsrUint8 actionTable[] = {
/*   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35 */
     0,  0,  1,  2,  0,  0,  0,  0,  0,  3,  4,  0,  5,  4,  0,  6,  7,  0,  8,  4,  0,  9,  4,  0, 10, 11,  0,  0, 12,  4,  0, 13, 14, 15, 16, 17
};

static CsrBool CsrBtIso8601Decode(CsrCharString* iso8601String, CsrBtIso8601Date * iso8601Date, CsrBtIso8601Time * iso8601Time, CsrBtIso8601Flags flags)
{
    CsrUint8  c;
    CsrUint8  state;
    CsrInt8   sign   = 1;
    CsrInt32  value  = 0;
    CsrBool   valid  = TRUE;
    CsrBool   number = FALSE;

    if (iso8601Date)
    {
        if (flags & CSR_BT_ISO8601_FLAGS_EXPANDED_YEAR)
        {
            state = 0;
        }
        else
        {
            state = 1;
        }

        iso8601Date->year  = 0;
        iso8601Date->month = 0;
        iso8601Date->day   = 0;
    }
    else
    {
        state                = 16;
    }

    if (iso8601Time)
    {
        iso8601Time->hour           = 0;
        iso8601Time->minute         = 0;
        iso8601Time->second         = 0;
        iso8601Time->fraction       = 0;
        iso8601Time->timezoneHour   = 0;
        iso8601Time->timezoneMinute = 0;
        iso8601Time->flags          = 0;
    }

    if (iso8601String && (iso8601Date || iso8601Time))
    {
        do
        {
            if ((*iso8601String & 0x80) != 0x80)
            {
                state = nextStateTable[state][c = characterCompressionTable[(*iso8601String & 0x7F)]];
                if (c == 5)
                {
                    value  = (value * 10) + (*iso8601String - 0x30);
                    number = TRUE;
                }

                switch (actionTable[state])
                {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        valid = FALSE;
                        break;
                    }
                    case 2:
                    {
                        if (c == 4)
                        {
                            sign = -1;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (iso8601Date)
                        {
                            iso8601Date->year = ((CsrInt32) value) * sign;
                            value          = 0;
                            number         = FALSE;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (!(flags & CSR_BT_ISO8601_FLAGS_EXTENDED))
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 5:
                    {
                        if (iso8601Date)
                        {
                            iso8601Date->month = (CsrUint8) (value & 0xFF);
                            value           = 0;
                            number          = FALSE;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 6:
                    {
                        if (iso8601Date)
                        {
                            iso8601Date->day = (CsrUint8) (value & 0xFF);
                            value         = 0;
                            number        = FALSE;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 8:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->hour  = (CsrUint8) (value & 0xFF);
                            iso8601Time->flags |= CSR_BT_TIME_FLAGS_HOUR_VALID;
                            value           = 0;
                            number          = FALSE;
                        }
                        break;
                    }
                    case 9:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->minute = (CsrUint8) (value & 0xFF);
                            iso8601Time->flags  |= CSR_BT_TIME_FLAGS_MINUTE_VALID;
                            value            = 0;
                            number           = FALSE;
                        }
                        break;
                    }
                    case 10:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->second = (CsrUint8) (value & 0xFF);
                            iso8601Time->flags  |= CSR_BT_TIME_FLAGS_SECOND_VALID;
                            value            = 0;
                            number           = FALSE;
                        }
                        break;
                    }
                    case 11:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->flags |= CSR_BT_TIME_FLAGS_UTC_TIME;
                        }
                        break;
                    }
                    case 12:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->timezoneHour = (CsrUint8) (value & 0xFF);
                            iso8601Time->flags        |= CSR_BT_TIME_FLAGS_TIMEZONE_VALID;
                            value                     = 0;
                            number                    = FALSE;
                        }
                        break;
                    }
                    case 13:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->timezoneMinute = (CsrUint8) (value & 0xFF);
                            iso8601Time->flags          |= CSR_BT_TIME_FLAGS_TIMEZONE_VALID;
                            value                       = 0;
                            number                      = FALSE;
                        }
                        break;
                    }
                    case 14:
                    {
                        if ( (iso8601Time) && (flags & CSR_BT_ISO8601_FLAGS_ALLOW_HOUR_FACTION))
                        {
                            iso8601Time->flags  |= CSR_BT_TIME_FLAGS_HOUR_FACTION;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 15:
                    {
                        if ( (iso8601Time) && (flags & CSR_BT_ISO8601_FLAGS_ALLOW_MINUTE_FACTION) )
                        {
                            iso8601Time->flags  |= CSR_BT_TIME_FLAGS_MINUTE_FACTION;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 16:
                    {
                        if ( (iso8601Time) && (flags & CSR_BT_ISO8601_FLAGS_ALLOW_SECOND_FACTION))
                        {
                            iso8601Time->flags  |= CSR_BT_TIME_FLAGS_SECOND_FACTION;
                        }
                        else
                        {
                            valid = FALSE;
                        }
                        break;
                    }
                    case 17:
                    {
                        if (!iso8601Time)
                        {
                            valid = FALSE;
                        }
                        else
                        {
                            iso8601Time->fraction = (CsrUint8) (value & 0xFF);
                            value              = 0;
                            number             = FALSE;
                        }
                        break;
                    }
                }
            }
            else
            {
                valid = FALSE;
            }

            iso8601String++;
        }
        while(valid && state);
    }

    if (!(flags & CSR_BT_ISO8601_FLAGS_ALLOW_REDUCED_ACCURACY))
    {
        if (iso8601Date)
        {
            if (!iso8601Date->day || !iso8601Date->month || !iso8601Date->year)
            {
                valid = FALSE;
            }
        }
    }

    return valid && !number;
}

static CsrBool CsrBtIso8601Encode(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601Date * iso8601Date, CsrBtIso8601Time * iso8601Time, CsrBtIso8601Flags flags)
{
    CsrBool   success = TRUE;
    CsrUint16 size    = 0;
    CsrUint16 result  = 0;

    size = (iso8601Date ? CSR_BT_ISO8601DATE_MAX_LENGTH : 0) + (iso8601Time ? CSR_BT_ISO8601TIME_MAX_LENGTH : 0);

    if (size < iso8601StringLength - 1)
    {
        if (iso8601Date && iso8601Date->year)
        {
            if (flags & CSR_BT_ISO8601_FLAGS_EXPANDED_YEAR)
            {
                if (result + 2 < iso8601StringLength)
                {
                    if (iso8601Date->year < 0)
                    {
                        iso8601String[result++] = '-';
                        iso8601Date->year = -iso8601Date->year;
                    }
                    else
                    {
                        iso8601String[result++] = '+';
                    }

                    iso8601String[result++] = (CsrUint8) (((iso8601Date->year / 10000) % 10) + '0');
                }
            }

            iso8601String[result++] = (CsrUint8) (((iso8601Date->year / 1000) % 10) + '0');
            iso8601String[result++] = (CsrUint8) (((iso8601Date->year / 100) % 10) + '0');
            iso8601String[result++] = (CsrUint8) (((iso8601Date->year / 10) % 10) + '0');
            iso8601String[result++] = (CsrUint8) (((iso8601Date->year ) % 10) + '0');

            if (iso8601Date->month)
            {
                if (flags & CSR_BT_ISO8601_FLAGS_EXTENDED)
                {
                    iso8601String[result++] = (CsrUint8) '-';
                }

                iso8601String[result++] = (CsrUint8) (((iso8601Date->month / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Date->month ) % 10) + '0');
            }

            if (iso8601Date->day)
            {
                if (flags & CSR_BT_ISO8601_FLAGS_EXTENDED)
                {
                    iso8601String[result++] = (CsrUint8) '-';
                }

                iso8601String[result++] = (CsrUint8) (((iso8601Date->day / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Date->day ) % 10) + '0');
            }
        }
        if (iso8601Time && iso8601Time->flags && success)
        {
            if (iso8601Date)
            {
                iso8601String[result++] = (CsrUint8) 'T';
            }

            iso8601String[result++] = (CsrUint8) (((iso8601Time->hour / 10) % 10) + '0');
            iso8601String[result++] = (CsrUint8) (((iso8601Time->hour ) % 10) + '0');

            if (iso8601Time->flags & CSR_BT_TIME_FLAGS_MINUTE_VALID)
            {
                if (flags & CSR_BT_ISO8601_FLAGS_EXTENDED)
                {
                    iso8601String[result++] = (CsrUint8) ':';
                }

                iso8601String[result++] = (CsrUint8) (((iso8601Time->minute / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Time->minute ) % 10) + '0');

                if (iso8601Time->flags & CSR_BT_TIME_FLAGS_SECOND_VALID)
                {
                    if (flags & CSR_BT_ISO8601_FLAGS_EXTENDED)
                    {
                        iso8601String[result++] = (CsrUint8) ':';
                    }

                    iso8601String[result++] = (CsrUint8) (((iso8601Time->second / 10) % 10) + '0');
                    iso8601String[result++] = (CsrUint8) (((iso8601Time->second ) % 10) + '0');

                    if (iso8601Time->flags & CSR_BT_TIME_FLAGS_SECOND_FACTION)
                    {
                        iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction / 10) % 10) + '0');
                        iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction ) % 10) + '0');
                    }
                }
                else if (iso8601Time->flags & CSR_BT_TIME_FLAGS_MINUTE_FACTION)
                {
                    iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction / 10) % 10) + '0');
                    iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction ) % 10) + '0');
                }
            }
            else if (iso8601Time->flags & CSR_BT_TIME_FLAGS_HOUR_FACTION)
            {
                iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Time->fraction ) % 10) + '0');
            }

            if (iso8601Time->flags & CSR_BT_TIME_FLAGS_UTC_TIME)
            {
                iso8601String[result++] = (CsrUint8) 'Z';
            }
            else if (iso8601Time->flags & CSR_BT_TIME_FLAGS_TIMEZONE_VALID)
            {
                iso8601String[result++] = (CsrUint8) (iso8601Time->timezoneHour < 0 ? '-' : '+');

                if (iso8601Time->timezoneHour < 0)
                {
                    iso8601Time->timezoneHour = -iso8601Time->timezoneHour;
                }

                iso8601String[result++] = (CsrUint8) (((iso8601Time->timezoneHour / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Time->timezoneHour ) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Time->timezoneMinute / 10) % 10) + '0');
                iso8601String[result++] = (CsrUint8) (((iso8601Time->timezoneMinute ) % 10) + '0');
            }
        }

        iso8601String[result++] = 0;
    }
    else
    {
        success = FALSE;
    }

    return success;
}

CsrBool Iso8601DateToCsrBtDate(CsrCharString* iso8601String, CsrBtIso8601Date * iso8601Date, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Decode(iso8601String, iso8601Date, NULL, flags);
}

CsrBool Iso8601DateToCsrBtTime(CsrCharString* iso8601String, CsrBtIso8601Time* iso8601Time, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Decode(iso8601String, NULL, iso8601Time, flags);
}

CsrBool Iso8601DateToCsrBtDateTime(CsrCharString* iso8601String, CsrBtIso8601DateTime* iso8601DateTime, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Decode(iso8601String, &iso8601DateTime->date, &iso8601DateTime->time, flags);
}

CsrBool CsrBtDateToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601Date* iso8601Date, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Encode(iso8601String, iso8601StringLength, iso8601Date, NULL, flags);
}

CsrBool CsrBtTimeToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601Time* iso8601Time, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Encode(iso8601String, iso8601StringLength, NULL, iso8601Time, flags);
}

CsrBool CsrBtDateTimeToIso8601Date(CsrCharString* iso8601String, CsrUint16 iso8601StringLength, CsrBtIso8601DateTime* iso8601DateTime, CsrBtIso8601Flags flags)
{
    return CsrBtIso8601Encode(iso8601String, iso8601StringLength, &iso8601DateTime->date, &iso8601DateTime->time, flags);
}

