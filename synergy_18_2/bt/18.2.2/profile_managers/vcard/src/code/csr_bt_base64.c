/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_base64.h"

static const CsrInt8 characterCompressionTable[] = {
/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

static const CsrUint8 languageTable[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

static const char languagePadding = '=';

CsrBool CsrBtBase64Decode(const CsrUint8* source, CsrUint32 sourceLength, CsrUint8** destination, CsrUint32* destinationLength)
{
    CsrInt8   c;
    CsrUint8  index;
    CsrUint32 offset, offset2;
    CsrUint8* dest;
    CsrBool   result = FALSE;

    if (source && sourceLength && destination && destinationLength)
    {
        *destination = (CsrUint8*) CsrPmemAlloc(sourceLength);
        dest = *destination;
        if (dest)
        {
            index  = 0;
            offset = 0;
            result = TRUE;

            while(*source && sourceLength--)
            {
                c = *source;
                source++;
                if (c == '=')
                {
                    offset2 = offset;

                    switch (index % 4)
                    {
                        case 1:
                        {
                            result = FALSE;
                            break;
                        }
                        case 2:
                        {
                            offset2++;
                        }
                        /*fallthrough*/
                        case 3:
                        {
                            dest[offset2] = 0;
                            break;
                        }
                    }
                    break;
                }

                c = characterCompressionTable[c];
                if (c < 0)
                {
                    continue;
                }

                switch (index++ % 4)
                {
                    case 0:
                    {
                        dest[offset] = (c << 2);
                        break;
                    }
                    case 1:
                    {
                        dest[offset++] |= (c >> 4);
                        dest[offset] = (c & 0x0f) << 4;
                        break;
                    }
                    case 2:
                    {
                        dest[offset++] |= (c >> 2);
                        dest[offset] = (c & 0x03) << 6;
                        break;
                    }
                    case 3:
                    {
                        dest[offset++] |= c;
                        break;
                    }
                }
            }

            if (result)
            {
                dest[offset] = 0;
                *destinationLength = offset;
            }
            else
            {
                CsrPmemFree(*destination);
                *destination = NULL;
            }
        }
    }

    return result;
}

CsrBool CsrBtBase64Encode(const CsrUint8* source, CsrUint32 sourceLength, CsrUint8** destination, CsrUint32* destinationLength)
{
    CsrUint8* buffer;
    CsrBool   result = FALSE;

    if (source && sourceLength && destination && destinationLength)
    {
        if (!*destination)
        {
            buffer = (CsrUint8*) CsrPmemAlloc(((sourceLength + 2) / 3) * 4);
            *destination = buffer;
        }
        else
        {
            buffer = *destination;
        }

        while (sourceLength > 2)
        {
            *buffer++ = languageTable[source[0] >> 2];
            *buffer++ = languageTable[((source[0] & 0x03) << 4) + (source[1] >> 4)];
            *buffer++ = languageTable[((source[1] & 0x0f) << 2) + (source[2] >> 6)];
            *buffer++ = languageTable[source[2] & 0x3f];

            source += 3;
            sourceLength -= 3;
        }

        if (sourceLength != 0)
        {
            *buffer++ = languageTable[source[0] >> 2];
            if (sourceLength > 1)
            {
                *buffer++ = languageTable[((source[0] & 0x03) << 4) + (source[1] >> 4)];
                *buffer++ = languageTable[(source[1] & 0x0f) << 2];
                *buffer++ = languagePadding;
            }
            else
            {
                *buffer++ = languageTable[(source[0] & 0x03) << 4];
                *buffer++ = languagePadding;
                *buffer++ = languagePadding;
            }
        }

        *buffer++ = 0;

        result = TRUE;
    }

    return result;
}
