/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_vcard.h"
#include "csr_bt_vcard_private.h"
#include "csr_bt_base64.h"
#include "csr_bt_util.h"

#define CSR_BT_VCARD_ADDRESS                       "ADR"
#define CSR_BT_VCARD_BEGIN                         "BEGIN:VCARD\r\n"
#define CSR_BT_VCARD_BIRTHDATE                     "BDAY"
#define CSR_BT_VCARD_CATAGORIES                    "CATEGORIES"
#define CSR_BT_VCARD_CLASSIFICATION                "CLASS"
#define CSR_BT_VCARD_EMAIL                         "EMAIL"
#define CSR_BT_VCARD_END                           "END:VCARD\r\n"
#define CSR_BT_VCARD_FORMATTED_NAME                "FN"
#define CSR_BT_VCARD_FORMATTED_NAME_EMPTY          "FN:\r\n"
#define CSR_BT_VCARD_GLOBALPOSITION                "GEO"
#define CSR_BT_VCARD_JOBTITLE                      "TITLE"
#define CSR_BT_VCARD_JOBROLE                       "ROLE"
#define CSR_BT_VCARD_JOBLOGO                       "LOGO"
#define CSR_BT_VCARD_JOBAGENT                      "AGENT"
#define CSR_BT_VCARD_KEY                           "KEY"
#define CSR_BT_VCARD_LABEL                         "LABEL"
#define CSR_BT_VCARD_MAILER                        "MAILER"
#define CSR_BT_VCARD_NAME                          "N"
#define CSR_BT_VCARD_NAME_EMPTY                    "N:\r\n"
#define CSR_BT_VCARD_NICKNAMES                     "NICKNAME"
#define CSR_BT_VCARD_NOTE                          "NOTE"
#define CSR_BT_VCARD_ORGANIZATION                  "ORG"
#define CSR_BT_VCARD_PHOTO                         "PHOTO"
#define CSR_BT_VCARD_PRODUCTIDENTIFIER             "PRODID"
#define CSR_BT_VCARD_REVISION                      "REV"
#define CSR_BT_VCARD_SORTSTRING                    "SORT-STRING"
#define CSR_BT_VCARD_SOUND                         "SOUND"
#define CSR_BT_VCARD_SOURCE                        "SOURCE"
#define CSR_BT_VCARD_TEL                           "TEL"
#define CSR_BT_VCARD_TIMEZONE                      "TZ"
#define CSR_BT_VCARD_UID                           "UID"
#define CSR_BT_VCARD_URL                           "URL"
#define CSR_BT_VCARD_VERSION_2_1_TEXT              "VERSION:2.1\r\n"
#define CSR_BT_VCARD_VERSION_3_0_TEXT              "VERSION:3.0\r\n"
#define CSR_BT_VCARD_XIRMCCALLDATETIME             "X-IRMC-CALL-DATETIME"
#define CSR_BT_VCARD_XIRMCLUID                     "X-IRMC-LUID"

#define CSR_BT_VCARD_PARAM_TYPE_2_1                ";"
#define CSR_BT_VCARD_PARAM_TYPE_3_0                ";TYPE="
#define CSR_BT_VCARD_PARAM_TYPE_HOME_2_1           ";HOME"
#define CSR_BT_VCARD_PARAM_TYPE_HOME_3_0           ";TYPE=HOME"
#define CSR_BT_VCARD_PARAM_TYPE_WORK_2_1           ";WORK"
#define CSR_BT_VCARD_PARAM_TYPE_WORK_3_0           ";TYPE=WORK"
#define CSR_BT_VCARD_PARAM_TYPE_PREF_2_1           ";PREF"
#define CSR_BT_VCARD_PARAM_TYPE_PREF_3_0           ";TYPE=PREF"
#define CSR_BT_VCARD_PARAM_TYPE_VOICE_2_1          ";VOICE"
#define CSR_BT_VCARD_PARAM_TYPE_VOICE_3_0          ";TYPE=VOICE"
#define CSR_BT_VCARD_PARAM_TYPE_FAX_2_1            ";FAX"
#define CSR_BT_VCARD_PARAM_TYPE_FAX_3_0            ";TYPE=FAX"
#define CSR_BT_VCARD_PARAM_TYPE_MSG_2_1            ";MSG"
#define CSR_BT_VCARD_PARAM_TYPE_MSG_3_0            ";TYPE=MSG"
#define CSR_BT_VCARD_PARAM_TYPE_CELL_2_1           ";CELL"
#define CSR_BT_VCARD_PARAM_TYPE_CELL_3_0           ";TYPE=CELL"
#define CSR_BT_VCARD_PARAM_TYPE_PAGER_2_1          ";PAGER"
#define CSR_BT_VCARD_PARAM_TYPE_PAGER_3_0          ";TYPE=PAGER"
#define CSR_BT_VCARD_PARAM_TYPE_BBS_2_1            ";BBS"
#define CSR_BT_VCARD_PARAM_TYPE_BBS_3_0            ";TYPE=BBS"
#define CSR_BT_VCARD_PARAM_TYPE_MODEM_2_1          ";MODEM"
#define CSR_BT_VCARD_PARAM_TYPE_MODEM_3_0          ";TYPE=MODEM"
#define CSR_BT_VCARD_PARAM_TYPE_CAR_2_1            ";CAR"
#define CSR_BT_VCARD_PARAM_TYPE_CAR_3_0            ";TYPE=CAR"
#define CSR_BT_VCARD_PARAM_TYPE_ISDN_2_1           ";ISDN"
#define CSR_BT_VCARD_PARAM_TYPE_ISDN_3_0           ";TYPE=ISDN"
#define CSR_BT_VCARD_PARAM_TYPE_VIDEO_2_1          ";VIDEO"
#define CSR_BT_VCARD_PARAM_TYPE_VIDEO_3_0          ";TYPE=VIDEO"
#define CSR_BT_VCARD_PARAM_TYPE_PCS_2_1            ";PCS"
#define CSR_BT_VCARD_PARAM_TYPE_PCS_3_0            ";TYPE=PCS"
#define CSR_BT_VCARD_PARAM_TYPE_INTERNET_2_1       ";INTERNET"
#define CSR_BT_VCARD_PARAM_TYPE_INTERNET_3_0       ";TYPE=INTERNET"
#define CSR_BT_VCARD_PARAM_TYPE_X400_2_1           ";X400"
#define CSR_BT_VCARD_PARAM_TYPE_X400_3_0           ";TYPE=X400"
#define CSR_BT_VCARD_PARAM_TYPE_X509_2_1           ";X509"
#define CSR_BT_VCARD_PARAM_TYPE_X509_3_0           ";TYPE=X509"
#define CSR_BT_VCARD_PARAM_TYPE_PGP_2_1            ";PGP"
#define CSR_BT_VCARD_PARAM_TYPE_PGP_3_0            ";TYPE=PGP"
#define CSR_BT_VCARD_PARAM_TYPE_DOM_2_1            ";DOM"
#define CSR_BT_VCARD_PARAM_TYPE_DOM_3_0            ";TYPE=DOM"
#define CSR_BT_VCARD_PARAM_TYPE_INTL_2_1           ";INTL"
#define CSR_BT_VCARD_PARAM_TYPE_INTL_3_0           ";TYPE=INTL"
#define CSR_BT_VCARD_PARAM_TYPE_POSTAL_2_1         ";POSTAL"
#define CSR_BT_VCARD_PARAM_TYPE_POSTAL_3_0         ";TYPE=POSTAL"
#define CSR_BT_VCARD_PARAM_TYPE_PARCEL_2_1         ";PARCEL"
#define CSR_BT_VCARD_PARAM_TYPE_PARCEL_3_0         ";TYPE=PARCEL"
#define CSR_BT_VCARD_PARAM_TYPE_MISSED_2_1         ";MISSED"
#define CSR_BT_VCARD_PARAM_TYPE_MISSED_3_0         ";TYPE=MISSED"
#define CSR_BT_VCARD_PARAM_TYPE_RECEIVED_2_1       ";RECEIVED"
#define CSR_BT_VCARD_PARAM_TYPE_RECEIVED_3_0       ";TYPE=RECEIVED"
#define CSR_BT_VCARD_PARAM_TYPE_DIALED_2_1         ";DIALED"
#define CSR_BT_VCARD_PARAM_TYPE_DIALED_3_0         ";TYPE=DIALED"
#define CSR_BT_VCARD_PARAM_TYPE_PERSONAL_2_1       ";PERSONAL"
#define CSR_BT_VCARD_PARAM_TYPE_PERSONAL_3_0       ";TYPE=PERSONAL"
#define CSR_BT_VCARD_PARAM_TYPE_BUSINESS_2_1       ";BUSINESS"
#define CSR_BT_VCARD_PARAM_TYPE_BUSINESS_3_0       ";TYPE=BUSINESS"
#define CSR_BT_VCARD_PARAM_TYPE_MOBILE_2_1         ";MOBILE"
#define CSR_BT_VCARD_PARAM_TYPE_MOBILE_3_0         ";TYPE=MOBILE"
#define CSR_BT_VCARD_PARAM_URI_VALUE_TYPE_2_1      ";VALUE=URL;"
#define CSR_BT_VCARD_PARAM_URI_VALUE_TYPE_3_0      ";VALUE=URI;TYPE="
#define CSR_BT_VCARD_PARAM_URI_VALUE_3_0           ";VALUE=URL"
#define CSR_BT_VCARD_PARAM_URI_VALUE_2_1           ";VALUE=URI"
#define CSR_BT_VCARD_PARAM_BINARY_ENCODING_2_1     ";ENCODING=BASE64"
#define CSR_BT_VCARD_PARAM_BINARY_ENCODING_3_0     ";ENCODING=b"
#define CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE_TEXT   ";ENCODING=QUOTED-PRINTABLE"
#define CSR_BT_VCARD_PARAM_CHARSET_TEXT            ";CHARSET=UTF-8"

#define CSR_BT_VCARD_COLON                         ":"
#define CSR_BT_VCARD_SEMICOLON                     ";"
#define CSR_BT_VCARD_SEMICOLON_UCS2                L";"
#define CSR_BT_VCARD_CRLF                          "\r\n"
#define CSR_BT_VCARD_FOLDING_STRING                "\r\n "
#define CSR_BT_VCARD_HELPER_LF_STRING_2_1          "=0A"
#define CSR_BT_VCARD_HELPER_LF_STRING_3_0          "\\r"
#define CSR_BT_VCARD_HELPER_CR_STRING_2_1          "=0D"
#define CSR_BT_VCARD_HELPER_CR_STRING_3_0          "\\n"
#define CSR_BT_VCARD_HELPER_21_STRING_2_1          "=21"
#define CSR_BT_VCARD_HELPER_22_STRING_2_1          "=22"
#define CSR_BT_VCARD_HELPER_23_STRING_2_1          "=23"
#define CSR_BT_VCARD_HELPER_24_STRING_2_1          "=24"
#define CSR_BT_VCARD_HELPER_2C_STRING_3_0          "\\,"
#define CSR_BT_VCARD_HELPER_3B_STRING_3_0          "\\;"
#define CSR_BT_VCARD_HELPER_3D_STRING_2_1          "=3D"
#define CSR_BT_VCARD_HELPER_40_STRING_2_1          "=40"
#define CSR_BT_VCARD_HELPER_5B_STRING_2_1          "=5B"
#define CSR_BT_VCARD_HELPER_5C_STRING_2_1          "=5C"
#define CSR_BT_VCARD_HELPER_5C_STRING_3_0          "\\\\"
#define CSR_BT_VCARD_HELPER_5D_STRING_2_1          "=5D"
#define CSR_BT_VCARD_HELPER_7B_STRING_2_1          "=7B"
#define CSR_BT_VCARD_HELPER_7C_STRING_2_1          "=7C"
#define CSR_BT_VCARD_HELPER_7D_STRING_2_1          "=7D"
#define CSR_BT_VCARD_HELPER_88_STRING_2_1          "=88"
#define CSR_BT_VCARD_HELPER_91_STRING_2_1          "=91"
#define CSR_BT_VCARD_HELPER_98_STRING_2_1          "=98"

#define VCARD_GEN_MIN(a,b)                            (((a) < (b)) ? (a) : (b))

static const CsrUint8 hexString[17] = "0123456789ABCDEF";

void CsrBtVcardGeneratorInitialize(CsrBtVcardGeneratorControl* control, CsrUint32 flags)
{
    control->vcardData              = NULL;
    control->currentVcardDataType   = NULL;
    control->generatorEntry         = CSR_BT_VCARD_GEN_ENTRY_START;
    control->flags                  = flags;
    control->entryState             = CSR_BT_VCARD_GEN_ENTRY_STATE_NAME;
    control->currentString          = NULL;
    control->helperString           = NULL;
    control->foldingString          = NULL;
    control->decodingString         = NULL;
    control->performEncoding        = FALSE;
    control->stringIsUCS2           = FALSE;
    control->expandLine             = FALSE;
    control->currentLineLength      = 0;
    control->binaryOffset           = 0;
    control->extendedType           = 0;
}

static CsrUint32 csrBtVcardAddString(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
{
    CsrUint32    result;
    CsrUint8     c = 0;
    CsrUint8     lineLength = CSR_BT_VCARD_GEN_NUMBER_OF_CHAR_ON_LINE;

    if (control->expandLine)
    {
        /* This prevents folding CRLF */
        lineLength++;
        lineLength++;
    }

    for (result = 0; control->currentString && result < bufferSize; result++)
    {
        do
        {
            if (++control->currentLineLength > lineLength && *(control->currentString) )
            {
                control->currentLineLength = 2;

                control->foldingString = (CsrUint8 *) CSR_BT_VCARD_FOLDING_STRING;

                c = *control->foldingString++;
            }
            else if (control->foldingString)
            {
                control->currentLineLength--;
                c = *control->foldingString++;
            }
            else if (control->helperString)
            {
                c = *control->helperString++;
            }
            else if (control->decodingString)
            {
                c = *control->decodingString++;
            }
            else if (control->currentString)
            {
                if (control->stringIsUCS2)
                {
                    CsrUint16 wc = *((CsrUint16*) (control->currentString));

                    control->currentString += sizeof(CsrUint16);

                    if (wc < 0x0080)
                    {
                        c = (CsrUint8) wc;
                    }
                    else if (wc < 0x0800)
                    {
                        c = (CsrUint8) ((wc >> 6) | 0xC0);
                        control->tempBuffer[0]  = (CsrUint8) ((wc & 0x3F) | 0x80);
                        control->tempBuffer[1]  = 0;
                        control->decodingString = (CsrUint8*)control->tempBuffer;
                    }
                    else
                    {
                        c = (CsrUint8) ((wc >> 12) | 0xE0);
                        control->tempBuffer[0]  = (CsrUint8) ((wc >> 6) | 0x80);
                        control->tempBuffer[1]  = (CsrUint8) ((wc & 0x3F) | 0x80);
                        control->tempBuffer[2]  = 0;
                        control->decodingString = (CsrUint8*)control->tempBuffer;
                    }
                }
                else
                {
                    c = *control->currentString++;
                }

                switch (c)
                {
                    case '!':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_21_STRING_2_1;
                        }
                        break;
                    }
                    case '"':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_22_STRING_2_1;
                        }
                        break;
                    }
                    case '#':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_23_STRING_2_1;
                        }
                        break;
                    }
                    case '$':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_24_STRING_2_1;
                        }
                        break;
                    }
                    case ',':
                    {
                        if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 && control->performEncoding)
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_2C_STRING_3_0;
                        }
                        break;
                    }
                    case ';':
                    {
                        if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 && control->performEncoding)
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_3B_STRING_3_0;
                        }
                        break;
                    }
                    case '@':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_40_STRING_2_1;
                        }
                        break;
                    }
                    case '[':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_5B_STRING_2_1;
                        }
                        break;
                    }
                    case '=':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0) && control->performEncoding)
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_3D_STRING_2_1;
                        }
                        break;
                    }
                    case '\\':
                    {
                        if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_5C_STRING_3_0;
                        }
                        else
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_5C_STRING_2_1;
                        }
                        break;
                    }
                    case ']':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_5D_STRING_2_1;
                        }
                        break;
                    }
                    case '{':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_7B_STRING_2_1;
                        }
                        break;
                    }
                    case '|':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_7C_STRING_2_1;
                        }
                        break;
                    }
                    case '}':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_7D_STRING_2_1;
                        }
                        break;
                    }
                    case '^':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_88_STRING_2_1;
                        }
                        break;
                    }
                    case '\'':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_91_STRING_2_1;
                        }
                        break;
                    }
                    case '~':
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_98_STRING_2_1;
                        }
                        break;
                    }
                    case '\r':
                    {
                        if (control->performEncoding)
                        {
                            if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                            {
                                control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_CR_STRING_3_0;
                                if (*control->currentString == '\n')
                                {
                                    if (control->stringIsUCS2)
                                    {
                                        control->currentString += sizeof(CsrUint16);
                                    }
                                    else
                                    {
                                        control->currentString++;
                                    }
                                }
                            }
                            else
                            {
                                control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_CR_STRING_2_1;
                            }
                        }
                        break;
                    }
                    case '\n':
                    {
                        if (control->performEncoding)
                        {
                            if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                            {
                                control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_CR_STRING_3_0;
                                if (*control->currentString == '\r')
                                {
                                    if (control->stringIsUCS2)
                                    {
                                        control->currentString += sizeof(CsrUint16);
                                    }
                                    else
                                    {
                                        control->currentString++;
                                    }
                                }
                            }
                            else
                            {
                                control->helperString = (CsrUint8*) CSR_BT_VCARD_HELPER_LF_STRING_2_1;
                            }
                        }
                        break;
                    }
                    default:
                    {
                        if (!(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0))
                        {
                            if (((c > 0) && (c < 0x20)) || c >= 0x7F)
                            {
                                control->encodeBuffer[0] = '=';
                                control->encodeBuffer[1] = hexString[(c & 0xF0) >> 4];
                                control->encodeBuffer[2] = hexString[c & 0x0F];
                                control->encodeBuffer[3] = '\0';
                                control->helperString = control->encodeBuffer;
                            }
                        }

                        break;
                    }
                }

                if (control->helperString)
                {
                    c = *control->helperString++;
                }
            }

            if (c)
            {
                buffer[result] = c;
            }
            else
            {
                if (control->currentLineLength)
                {
                    control->currentLineLength--;
                }

                if (control->foldingString)
                {
                    control->foldingString = NULL;
                }
                else if (control->helperString)
                {
                    control->helperString = NULL;
                }
                else if (control->decodingString)
                {
                    control->decodingString = NULL;
                }
                else if (control->currentString)
                {
                    control->currentString = NULL;
                    result--;
                    break;
                }
            }
        }
        while (c == 0);
    }

    return result;
}

static CsrUint32 csrBtVcardAddParameter(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
{
    CsrUint32 result = 0;

    while (result < bufferSize && (control->type != 0 || control->currentString))
    {
        if (control->currentString == NULL)
        {
            if (control->type & CSR_BT_VCARD_PARAM_TYPE_HOME)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_HOME_3_0 : CSR_BT_VCARD_PARAM_TYPE_HOME_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_HOME;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_WORK)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_WORK_3_0 : CSR_BT_VCARD_PARAM_TYPE_WORK_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_WORK;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PREF)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PREF_3_0 : CSR_BT_VCARD_PARAM_TYPE_PREF_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PREF;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_VOICE)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_VOICE_3_0 : CSR_BT_VCARD_PARAM_TYPE_VOICE_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_VOICE;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_FAX)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_FAX_3_0 : CSR_BT_VCARD_PARAM_TYPE_FAX_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_FAX;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_MSG)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_MSG_3_0 : CSR_BT_VCARD_PARAM_TYPE_MSG_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_MSG;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_CELL)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_CELL_3_0 : CSR_BT_VCARD_PARAM_TYPE_CELL_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_CELL;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PAGER)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PAGER_3_0 : CSR_BT_VCARD_PARAM_TYPE_PAGER_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PAGER;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_BBS)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_BBS_3_0 : CSR_BT_VCARD_PARAM_TYPE_BBS_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_BBS;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_MODEM)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_MODEM_3_0 : CSR_BT_VCARD_PARAM_TYPE_MODEM_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_MODEM;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_CAR)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_CAR_3_0 : CSR_BT_VCARD_PARAM_TYPE_CAR_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_CAR;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_ISDN)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_ISDN_3_0 : CSR_BT_VCARD_PARAM_TYPE_ISDN_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_ISDN;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_VIDEO)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_VIDEO_3_0 : CSR_BT_VCARD_PARAM_TYPE_VIDEO_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_VIDEO;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PCS)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PCS_3_0 : CSR_BT_VCARD_PARAM_TYPE_PCS_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PCS;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_INTERNET)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_INTERNET_3_0 : CSR_BT_VCARD_PARAM_TYPE_INTERNET_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_INTERNET;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_X400)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_X400_3_0 : CSR_BT_VCARD_PARAM_TYPE_X400_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_X400;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_X509)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_X509_3_0 : CSR_BT_VCARD_PARAM_TYPE_X509_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_X509;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PGP)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PGP_3_0 : CSR_BT_VCARD_PARAM_TYPE_PGP_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PGP;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_DOM)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_DOM_3_0 : CSR_BT_VCARD_PARAM_TYPE_DOM_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_DOM;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_INTL)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_INTL_3_0 : CSR_BT_VCARD_PARAM_TYPE_INTL_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_INTL;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_POSTAL)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_POSTAL_3_0 : CSR_BT_VCARD_PARAM_TYPE_POSTAL_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_POSTAL;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_X_NAME)
            {
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_X_NAME;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_STR)
            {
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_STR;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PARCEL)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PARCEL_3_0 : CSR_BT_VCARD_PARAM_TYPE_PARCEL_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PARCEL;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_MISSED)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_MISSED_3_0 : CSR_BT_VCARD_PARAM_TYPE_MISSED_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_MISSED;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_RECEIVED)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_RECEIVED_3_0 : CSR_BT_VCARD_PARAM_TYPE_RECEIVED_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_RECEIVED;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_DIALED)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_DIALED_3_0 : CSR_BT_VCARD_PARAM_TYPE_DIALED_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_DIALED;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_PERSONAL)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_PERSONAL_3_0 : CSR_BT_VCARD_PARAM_TYPE_PERSONAL_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_PERSONAL;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_BUSINESS)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_BUSINESS_3_0 : CSR_BT_VCARD_PARAM_TYPE_BUSINESS_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_BUSINESS;
            }
            else if (control->type & CSR_BT_VCARD_PARAM_TYPE_MOBILE)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_TYPE_MOBILE_3_0 : CSR_BT_VCARD_PARAM_TYPE_MOBILE_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_TYPE_MOBILE;
            }
            else
            {
                control->mask               = 0;
            }
        }

        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
        if (control->currentString == NULL)
        {
            control->type &= control->mask;
        }
    }

    if (control->type == 0)
    {
        control->entryState++;
    }

    return result;
}

static CsrUint32 csrBtVcardAddParameterExtended(CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
{
    CsrUint32 result = 0;

    while (result < bufferSize && (control->extendedType != 0 || control->currentString))
    {
        if (control->currentString == NULL)
        {
            if (control->extendedType & CSR_BT_VCARD_PARAM_BINARY_ENCODING)
            {
                control->currentString      = (CsrUint8 *)(control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0 ? CSR_BT_VCARD_PARAM_BINARY_ENCODING_3_0 : CSR_BT_VCARD_PARAM_BINARY_ENCODING_2_1);
                control->mask               = ~CSR_BT_VCARD_PARAM_BINARY_ENCODING;
            }
            else if (control->extendedType & CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE)
            {
                control->currentString      = (CsrUint8 *)(CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE_TEXT);
                control->mask               = ~CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE;
            }
            else if (control->extendedType & CSR_BT_VCARD_PARAM_CHARSET)
            {
                control->currentString      = (CsrUint8 *)(CSR_BT_VCARD_PARAM_CHARSET_TEXT);
                control->mask               = ~CSR_BT_VCARD_PARAM_CHARSET;
            }
            else
            {
                control->mask               = 0;
            }
        }

        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
        if (control->currentString == NULL)
        {
            control->extendedType &= (CsrUint8) control->mask;
        }
    }

    if (control->extendedType == 0)
    {
        control->entryState++;
    }

    return result;
}

static void csrBtVcardFindNextAddressEntry(CsrBtVcardDataType* dataType, CsrBtVcardGeneratorControl* control)
{
    CsrUint8 finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_COMPLETE;

    if (dataType->payload.address->countryName)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_COMPLETE;
    }
    else if (dataType->payload.address->postalCode)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_POSTAL;
    }
    else if (dataType->payload.address->region)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_REGION;
    }
    else if (dataType->payload.address->locality)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_LOCAL;
    }
    else if (dataType->payload.address->street)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_STREET;
    }
    else if (dataType->payload.address->extendedAddress)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_EXADR;
    }
    else if (dataType->payload.address->poBox)
    {
        finalEntry = CSR_BT_VCARD_GEN_ADR_STATE_POBOX;
    }
    else
    {
        control->structState = finalEntry + 1;
    }

    if (control->structState <= finalEntry)
    {
        switch (control->structState)
        {
            case CSR_BT_VCARD_GEN_ADR_STATE_POBOX:
            {
                if (dataType->payload.address->poBox)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->poBox;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_EXADR:
            {
                if (dataType->payload.address->extendedAddress)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->extendedAddress;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_STREET:
            {
                if (dataType->payload.address->street)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->street;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_LOCAL:
            {
                if (dataType->payload.address->locality)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->locality;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_REGION:
            {
                if (dataType->payload.address->region)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->region;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_POSTAL:
            {
                if (dataType->payload.address->postalCode)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->postalCode;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_COUNTRY:
            {
                if (dataType->payload.address->countryName)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.address->countryName;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_POBOX_END:
            case CSR_BT_VCARD_GEN_ADR_STATE_EXADR_END:
            case CSR_BT_VCARD_GEN_ADR_STATE_STREET_END:
            case CSR_BT_VCARD_GEN_ADR_STATE_LOCAL_END:
            case CSR_BT_VCARD_GEN_ADR_STATE_REGION_END:
            case CSR_BT_VCARD_GEN_ADR_STATE_POSTAL_END:
            {
                control->currentString   = control->stringIsUCS2 ? (CsrUint8*) CSR_BT_VCARD_SEMICOLON_UCS2 : (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                control->performEncoding = FALSE;
                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_COMPLETE:
            {
                control->currentString = NULL;
                break;
            }
        }
    }
    else
    {
        control->currentString = NULL;
    }
}

static void csrBtVcardFindNextNameEntry(CsrBtVcardDataType* dataType, CsrBtVcardGeneratorControl* control)
{
    CsrUint8 finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_COMPLETE;

    if (dataType->payload.name->honorificSuffixes)
    {
        finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_COMPLETE;
    }
    else if (dataType->payload.name->honorificPrefixes)
    {
        finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_PREFIXES;
    }
    else if (dataType->payload.name->additionalNames)
    {
        finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_ADDITIONAL_NAMES;
    }
    else if (dataType->payload.name->givenName)
    {
        finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_GIVEN_NAME;
    }
    else if (dataType->payload.name->familyName)
    {
        finalEntry = CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME;
    }
    else
    {
        control->structState = finalEntry + 1;
    }

    if (control->structState <= finalEntry)
    {
        switch (control->structState)
        {
            case CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME:
            {
                if (dataType->payload.name->familyName)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.name->familyName;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_NAME_STATE_GIVEN_NAME:
            {
                if (dataType->payload.name->givenName)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.name->givenName;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_NAME_STATE_ADDITIONAL_NAMES:
            {
                if (dataType->payload.name->additionalNames)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.name->additionalNames;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_PREFIXES:
            {
                if (dataType->payload.name->honorificPrefixes)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.name->honorificPrefixes;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_SUFFIXES:
            {
                if (dataType->payload.name->honorificSuffixes)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.name->honorificSuffixes;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME_END:
            case CSR_BT_VCARD_GEN_NAME_STATE_GIVEN_NAME_END:
            case CSR_BT_VCARD_GEN_NAME_STATE_ADDITIONAL_NAMES_END:
            case CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_PREFIXES_END:
            {
                control->currentString   = control->stringIsUCS2 ? (CsrUint8*) CSR_BT_VCARD_SEMICOLON_UCS2 : (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                control->performEncoding = FALSE;
                break;
            }
            case CSR_BT_VCARD_GEN_ADR_STATE_COMPLETE:
            {
                control->currentString = NULL;
                break;
            }
        }
    }
    else
    {
        control->currentString = NULL;
    }
}

static void csrBtVcardFindNextOrganizationEntry(CsrBtVcardDataType* dataType, CsrBtVcardGeneratorControl* control)
{
    CsrUint8 finalEntry = CSR_BT_VCARD_GEN_ORG_STATE_COMPLETE;

    if (dataType->payload.organization->organizationalUnit2)
    {
        finalEntry = CSR_BT_VCARD_GEN_ORG_STATE_COMPLETE;
    }
    else if (dataType->payload.organization->organizationalUnit1)
    {
        finalEntry = CSR_BT_VCARD_GEN_ORG_STATE_UNIT1;
    }
    else if (dataType->payload.organization->organizationalName)
    {
        finalEntry = CSR_BT_VCARD_GEN_ORG_STATE_NAME;
    }
    else
    {
        control->structState = finalEntry + 1;
    }

    if (control->structState <= finalEntry)
    {
        switch (control->structState)
        {
            case CSR_BT_VCARD_GEN_ORG_STATE_NAME:
            {
                if (dataType->payload.organization->organizationalName)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.organization->organizationalName;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ORG_STATE_UNIT1:
            {
                if (dataType->payload.name->givenName)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.organization->organizationalUnit1;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ORG_STATE_UNIT2:
            {
                if (dataType->payload.name->additionalNames)
                {
                    control->currentString   = (CsrUint8*)dataType->payload.organization->organizationalUnit2;
                    control->performEncoding = TRUE;
                }
                else
                {
                    control->currentString   = (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                    control->performEncoding = FALSE;
                    control->structState++;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ORG_STATE_NAME_END:
            case CSR_BT_VCARD_GEN_ORG_STATE_UNIT1_END:
            {
                control->currentString   = control->stringIsUCS2 ? (CsrUint8*) CSR_BT_VCARD_SEMICOLON_UCS2 : (CsrUint8*) CSR_BT_VCARD_SEMICOLON;
                control->performEncoding = FALSE;
                break;
            }
            case CSR_BT_VCARD_GEN_ORG_STATE_COMPLETE:
            {
                control->currentString = NULL;
                break;
            }
        }
    }
    else
    {
        control->currentString = NULL;
    }
}

static CsrUint32 csrBtVcardAddItem(CsrUint8* string, CsrBtVcardDataType* dataType, CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
{
    CsrUint32 result = 0;

    if (control->currentVcardDataType)
    {
        while (dataType)
        {
            if (dataType == control->currentVcardDataType)
            {
                break;
            }
            else
            {
                dataType = dataType->next;
            }
        }
    }
    else
    {
        control->currentVcardDataType = dataType;
    }

    if (dataType)
    {
        while (result < bufferSize && control->entryState != CSR_BT_VCARD_GEN_ENTRY_STATE_COMPLETE)
        {
            switch (control->entryState)
            {
                case CSR_BT_VCARD_GEN_ENTRY_STATE_NAME:
                {
                    if (control->currentString == NULL)
                    {
                        control->currentString  = (CsrUint8*)string;

                        switch (control->currentVcardDataType->dataType)
                        {
                            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UCS2:
                            {
                                control->type         = control->currentVcardDataType->payload.telephone->type;

                                if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                                {
                                    control->extendedType = 0;
                                }
                                else
                                {
                                    control->extendedType = CSR_BT_VCARD_PARAM_CHARSET | CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE;
                                }
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_NAME_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_NAME_UCS2:
                            case CSR_BT_VCARD_DATA_TYPE_STRING_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_STRING_UCS2:
                            case CSR_BT_VCARD_DATA_TYPE_URI_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_URI_UCS2:
                            {
                                control->type = 0;

                                if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                                {
                                    control->extendedType = 0;
                                }
                                else
                                {
                                    control->extendedType = CSR_BT_VCARD_PARAM_CHARSET | CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE;
                                }
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE:
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME:
                            {
                                control->type         = 0;
                                control->extendedType = 0;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2:
                            {
                                control->type = control->currentVcardDataType->payload.address->addressType;

                                if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                                {
                                    control->extendedType = 0;
                                }
                                else
                                {
                                    control->extendedType = CSR_BT_VCARD_PARAM_CHARSET | CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE;
                                }
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_LABEL_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_LABEL_UCS2:
                            {
                                control->type = control->currentVcardDataType->payload.label->type;

                                if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                                {
                                    control->extendedType = 0;
                                }
                                else
                                {
                                    control->extendedType = CSR_BT_VCARD_PARAM_CHARSET | CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE;
                                }
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE_TYPED:
                            {
                                control->type         = control->currentVcardDataType->payload.iso8601DateTyped->type;
                                control->extendedType = 0;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED:
                            {
                                control->type         = control->currentVcardDataType->payload.iso8601DateTimeTyped->type;
                                control->extendedType = 0;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8:
                            case CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2:
                            {
                                control->type         = 0;
                                control->extendedType = CSR_BT_VCARD_PARAM_BINARY_ENCODING;
                                break;
                            }
                            default:
                            {
                                control->type         = 0;
                                control->extendedType = 0;
                                break;
                            }
                        }
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->entryState++;
                    }

                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_PARAMETER:
                {
                    result += csrBtVcardAddParameter(&buffer[result], bufferSize - result, control);
                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_PARAMETER_EXTENDED:
                {
                    result += csrBtVcardAddParameterExtended(&buffer[result], bufferSize - result, control);
                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_BINARY_OR_URI_TYPE_NAME:
                {
                    if ((control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2) &&
                        control->currentVcardDataType->payload.binary->binaryType)
                    {
                        if (control->currentString == NULL)
                        {
                            if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                            {
                                control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_TYPE_3_0;
                            }
                            else
                            {
                                control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_TYPE_2_1;
                            }
                        }

                        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                        if (control->currentString == NULL)
                        {
                            control->entryState++;
                        }
                    }
                    else if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_URI_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_URI_UCS2)
                    {
                        if (control->currentString == NULL)
                        {
                            if (control->currentVcardDataType->payload.uriString->type)
                            {
                                if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                                {
                                    control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_URI_VALUE_TYPE_3_0;
                                }
                                else
                                {
                                    control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_URI_VALUE_TYPE_2_1;
                                }
                            }
                            else if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                            {
                                control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_URI_VALUE_3_0;
                            }
                            else
                            {
                                control->currentString = (CsrUint8*) CSR_BT_VCARD_PARAM_URI_VALUE_2_1;
                            }
                        }

                        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                        if (control->currentString == NULL)
                        {
                            control->entryState++;
                        }
                    }
                    else
                    {
                        control->entryState = CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE_START;
                    }
                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_BINARY_OR_URI_TYPE_VALUE:
                {
                    if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2)
                    {
                        if (control->currentString == NULL)
                        {
                            control->currentString = (CsrUint8*)control->currentVcardDataType->payload.binary->binaryType;

                            if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2)
                            {
                                control->stringIsUCS2  = TRUE;
                            }
                        }

                        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                        if (control->currentString == NULL)
                        {
                            control->entryState++;
                        }
                    }
                    else if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_URI_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_URI_UCS2)
                    {
                        if (control->currentString == NULL)
                        {
                            if (!control->currentVcardDataType->payload.uriString->type)
                            {
                                control->entryState++;
                                break;
                            }

                            control->currentString = (CsrUint8*)control->currentVcardDataType->payload.uriString->type;

                            if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_URI_UCS2)
                            {
                                control->stringIsUCS2  = TRUE;
                            }
                        }

                        result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                        if (control->currentString == NULL)
                        {
                            control->entryState++;
                        }
                    }
                    else
                    {
                        control->entryState = CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE_START;
                    }

                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE_START:
                {
                    if (control->currentString == NULL)
                    {
                        control->currentString  = (CsrUint8*) CSR_BT_VCARD_COLON;
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->entryState++;
                    }

                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE:
                {
                    if (control->currentString == NULL)
                    {
                        control->performEncoding = TRUE;

                        switch (control->currentVcardDataType->dataType)
                        {
                            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UTF8:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.telephone->number;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UCS2:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.telephone->number;
                                control->stringIsUCS2  = TRUE;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_NAME_UTF8:
                            {
                                control->structState = CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME;

                                csrBtVcardFindNextNameEntry(control->currentVcardDataType, control);

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_NAME_UCS2:
                            {
                                control->structState  = CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME;
                                control->stringIsUCS2 = TRUE;

                                csrBtVcardFindNextNameEntry(control->currentVcardDataType, control);

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_STRING_UTF8:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.string;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_STRING_UCS2:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.string;
                                control->stringIsUCS2  = TRUE;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_URI_UTF8:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.uriString->string;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_URI_UCS2:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.uriString->string;
                                control->stringIsUCS2  = TRUE;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE:
                            {
                                CsrBtDateToIso8601Date(control->tempBuffer, sizeof(control->tempBuffer), control->currentVcardDataType->payload.iso8601Date, 0);

                                control->currentString = (CsrUint8*)control->tempBuffer;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE_TYPED:
                            {
                                CsrBtDateToIso8601Date(control->tempBuffer, sizeof(control->tempBuffer), &control->currentVcardDataType->payload.iso8601DateTyped->iso8601Date, 0);

                                control->currentString = (CsrUint8*)control->tempBuffer;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME:
                            {
                                CsrBtDateTimeToIso8601Date(control->tempBuffer, sizeof(control->tempBuffer), control->currentVcardDataType->payload.iso8601DateTime, 0);

                                control->currentString = (CsrUint8*)control->tempBuffer;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED:
                            {
                                CsrBtDateTimeToIso8601Date(control->tempBuffer, sizeof(control->tempBuffer), &control->currentVcardDataType->payload.iso8601DateTimeTyped->iso8601DateTime, 0);

                                control->currentString = (CsrUint8*)control->tempBuffer;

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8:
                            {
                                control->structState = CSR_BT_VCARD_GEN_ADR_STATE_POBOX;

                                csrBtVcardFindNextAddressEntry(control->currentVcardDataType, control);

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2:
                            {
                                control->structState   = CSR_BT_VCARD_GEN_ADR_STATE_POBOX;
                                control->stringIsUCS2  = TRUE;

                                csrBtVcardFindNextAddressEntry(control->currentVcardDataType, control);

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_LABEL_UTF8:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.label->string;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_LABEL_UCS2:
                            {
                                control->currentString = (CsrUint8*)control->currentVcardDataType->payload.label->string;
                                control->stringIsUCS2  = TRUE;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8:
                            {
                                control->currentString = NULL;
                                control->binaryOffset  = 0;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2:
                            {
                                control->currentString = NULL;
                                control->stringIsUCS2  = TRUE;
                                control->binaryOffset  = 0;
                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UTF8:
                            {
                                control->structState = CSR_BT_VCARD_GEN_ORG_STATE_NAME;

                                csrBtVcardFindNextOrganizationEntry(control->currentVcardDataType, control);

                                break;
                            }
                            case CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UCS2:
                            {
                                control->structState   = CSR_BT_VCARD_GEN_ORG_STATE_NAME;
                                control->stringIsUCS2  = TRUE;

                                csrBtVcardFindNextOrganizationEntry(control->currentVcardDataType, control);

                                break;
                            }
                            default:
                            {
                                control->currentString = NULL;

                                break;
                            }
                        }
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2)
                        {
                            control->structState++;

                            csrBtVcardFindNextAddressEntry(control->currentVcardDataType, control);

                            if (control->currentString == NULL)
                            {
                                control->performEncoding = FALSE;
                                control->stringIsUCS2    = FALSE;
                                control->entryState++;
                            }
                        }
                        else if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_NAME_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_NAME_UCS2)
                        {
                            control->structState++;

                            csrBtVcardFindNextNameEntry(control->currentVcardDataType, control);

                            if (control->currentString == NULL)
                            {
                                control->performEncoding = FALSE;
                                control->stringIsUCS2    = FALSE;
                                control->entryState++;
                            }
                        }
                        else if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UCS2)
                        {
                            control->structState++;

                            csrBtVcardFindNextOrganizationEntry(control->currentVcardDataType, control);

                            if (control->currentString == NULL)
                            {
                                control->performEncoding = FALSE;
                                control->stringIsUCS2    = FALSE;
                                control->entryState++;
                            }
                        }
                        else if (control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8 || control->currentVcardDataType->dataType == CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2)
                        {
                            if (control->binaryOffset < control->currentVcardDataType->payload.binary->binarySize)
                            {
                                CsrUint8* buffer     = (CsrUint8*) &control->tempBuffer;
                                CsrUint32 bufferSize = sizeof(control->tempBuffer) - 1;

                                control->stringIsUCS2 = FALSE;

                                if (CsrBtBase64Encode(&control->currentVcardDataType->payload.binary->binaryData[control->binaryOffset], VCARD_GEN_MIN(control->currentVcardDataType->payload.binary->binarySize - control->binaryOffset, 3), &buffer, &bufferSize))
                                {
                                    buffer[bufferSize]     = 0;
                                    control->currentString = buffer;
                                    control->binaryOffset  += 3;
                                }
                                else
                                {
                                    control->performEncoding = FALSE;
                                    control->stringIsUCS2    = FALSE;
                                    control->entryState++;
                                }
                            }
                            else
                            {
                                control->performEncoding = FALSE;
                                control->stringIsUCS2    = FALSE;
                                control->entryState++;
                            }
                        }
                        else
                        {
                            control->performEncoding = FALSE;
                            control->stringIsUCS2    = FALSE;
                            control->entryState++;
                        }
                    }

                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_EOL:
                {
                    if (control->currentString == NULL)
                    {
                        control->currentString = (CsrUint8*) CSR_BT_VCARD_CRLF;
                        control->expandLine    = TRUE;
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->currentVcardDataType = control->currentVcardDataType->next;

                        if (control->currentVcardDataType)
                        {
                            control->entryState             = CSR_BT_VCARD_GEN_ENTRY_STATE_NAME;
                        }
                        else
                        {
                            control->entryState++;
                        }

                        control->currentLineLength = 0;
                        control->expandLine        = FALSE;
                    }

                    break;
                }
                case CSR_BT_VCARD_GEN_ENTRY_STATE_COMPLETE:
                {
                    break;
                }
                default:
                {
                    control->entryState = CSR_BT_VCARD_GEN_ENTRY_STATE_EOL;

                    break;
                }
            }
        }

        if (control->entryState == CSR_BT_VCARD_GEN_ENTRY_STATE_COMPLETE)
        {
            control->entryState = CSR_BT_VCARD_GEN_ENTRY_STATE_NAME;
            control->generatorEntry++;
            control->currentLineLength = 0;
        }
    }
    else
    {
        control->generatorEntry++;
        result = 0;
    }

    return result;
}

CsrUint32 CsrBtVcardGeneratorWrite(CsrBtVcardData* vcardData, CsrUint8* buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl* control)
{
    CsrUint32    result              = 0;
    CsrBool      generationComplete  = FALSE;

    while (result < bufferSize && !generationComplete)
    {
        switch (control->generatorEntry)
        {
            case CSR_BT_VCARD_GEN_ENTRY_START:
            {
                if (vcardData)
                {
                    if (control->currentString == NULL)
                    {
                        control->vcardData      = vcardData;
                        control->currentString  = (CsrUint8*) CSR_BT_VCARD_BEGIN;
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->generatorEntry++;
                        control->currentLineLength = 0;
                    }
                }
                else
                {
                    generationComplete = TRUE;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_VERSION:
            {
                if (control->currentString == NULL)
                {
                    control->currentString  = (CsrUint8 *)((control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0) ? CSR_BT_VCARD_VERSION_3_0_TEXT : CSR_BT_VCARD_VERSION_2_1_TEXT);
                }

                result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                if (control->currentString == NULL)
                {
                    control->generatorEntry++;
                    control->currentLineLength = 0;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_FORMATTEDNAME:
            {
                if (control->vcardData->formattedName)
                {
                    result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_FORMATTED_NAME, control->vcardData->formattedName, &buffer[result], bufferSize - result, control);
                }
                else if (control->flags & CSR_BT_VCARD_FLAGS_VERSION_3_0)
                {
                    if (control->currentString == NULL)
                    {
                        control->currentString  = (CsrUint8*) CSR_BT_VCARD_FORMATTED_NAME_EMPTY;
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->generatorEntry++;
                        control->currentLineLength = 0;
                    }
                }
                else
                {
                    control->generatorEntry++;
                    control->currentLineLength = 0;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_NAME:
            {
                if (control->vcardData->name)
                {
                    result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_NAME, control->vcardData->name, &buffer[result], bufferSize - result, control);
                }
                else
                {
                    if (control->currentString == NULL)
                    {
                        control->currentString  = (CsrUint8*) CSR_BT_VCARD_NAME_EMPTY;
                    }

                    result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                    if (control->currentString == NULL)
                    {
                        control->generatorEntry++;
                        control->currentLineLength = 0;
                    }
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_NICKNAMES:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_NICKNAMES, control->vcardData->nickNames, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_PHOTO:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_PHOTO, control->vcardData->photo, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_BIRTHDATE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_BIRTHDATE, control->vcardData->birthDate, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_SOURCE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_SOURCE, control->vcardData->source, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_ADDRESS:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_ADDRESS, control->vcardData->address, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_LABEL:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_LABEL, control->vcardData->label, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_TEL:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_TEL, control->vcardData->tel, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_EMAIL:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_EMAIL, control->vcardData->email, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_MAILER:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_MAILER, control->vcardData->mailer, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_TIMEZONE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_TIMEZONE, control->vcardData->timezone, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_GLOBALPOSITION:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_GLOBALPOSITION, control->vcardData->globalPosition, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_JOBTITLE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_JOBTITLE, control->vcardData->jobTitle, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_JOBROLE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_JOBROLE, control->vcardData->jobRole, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_JOBLOGO:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_JOBLOGO, control->vcardData->jobLogo, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_JOBAGENT:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_JOBAGENT, control->vcardData->jobAgent, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_ORGANIZATION:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_ORGANIZATION, control->vcardData->organization, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_CATAGORIES:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_CATAGORIES, control->vcardData->catagories, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_NOTE:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_NOTE, control->vcardData->note, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_PRODUCTIDENTIFIER:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_PRODUCTIDENTIFIER, control->vcardData->productIdentifier, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_REVISION :
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_REVISION, control->vcardData->revision, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_SORTSTRING:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_SORTSTRING, control->vcardData->sortString, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_SOUND:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_SOUND, control->vcardData->sound, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_UID:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_UID, control->vcardData->uid, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_URL:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_URL, control->vcardData->url, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_CLASSIFICATION:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_CLASSIFICATION, control->vcardData->classification, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_KEY:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_KEY, control->vcardData->key, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_XIRMCCALLDATETIME:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_XIRMCCALLDATETIME, control->vcardData->xIrmcCallDatetime, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_XIRMCLUID:
            {
                result += csrBtVcardAddItem((CsrUint8*) CSR_BT_VCARD_XIRMCLUID, control->vcardData->xIrmcLuid, &buffer[result], bufferSize - result, control);
                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_END:
            {
                if (control->currentString == NULL)
                {
                    control->currentString  = (CsrUint8*) CSR_BT_VCARD_END;
                }

                result += csrBtVcardAddString(&buffer[result], bufferSize - result, control);
                if (control->currentString == NULL)
                {
                    control->generatorEntry++;
                    control->currentLineLength = 0;
                }

                break;
            }
            case CSR_BT_VCARD_GEN_ENTRY_COMPLETE:
            {
                if (result < bufferSize)
                {
                    if (control->vcardData->next)
                    {
                        vcardData = control->vcardData->next;
                    }
                    else
                    {
                        generationComplete = TRUE;
                    }

                    control->vcardData              = NULL;
                    control->generatorEntry         = CSR_BT_VCARD_GEN_ENTRY_START;
                    control->entryState             = CSR_BT_VCARD_GEN_ENTRY_STATE_NAME;
                    control->currentString          = NULL;
                    control->helperString           = NULL;
                    control->foldingString          = NULL;
                    control->decodingString         = NULL;
                    control->currentVcardDataType   = NULL;
                    control->performEncoding        = FALSE;
                    control->stringIsUCS2           = FALSE;
                    control->expandLine             = FALSE;
                    control->currentLineLength      = 0;
                    control->binaryOffset           = 0;
                    control->extendedType           = 0;
                }
                else
                {
                    result = result;
                }

                break;
            }
            default:
            {
                control->generatorEntry++;
                break;
            }
        }
    }

    return result;
}
