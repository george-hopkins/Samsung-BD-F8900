/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_vcard.h"
#include "csr_bt_base64.h"
#include "csr_bt_util.h"

#define CSR_BT_VCARD_TOKEN_UNKNOWN                (0x00)
#define CSR_BT_VCARD_TOKEN_ADR                    (0x01)
#define CSR_BT_VCARD_TOKEN_AGENT                  (0x02)
#define CSR_BT_VCARD_TOKEN_BDAY                   (0x03)
#define CSR_BT_VCARD_TOKEN_BEGIN                  (0x04)
#define CSR_BT_VCARD_TOKEN_CATEGORIES             (0x05)
#define CSR_BT_VCARD_TOKEN_CLASS                  (0x06)
#define CSR_BT_VCARD_TOKEN_EMAIL                  (0x07)
#define CSR_BT_VCARD_TOKEN_END                    (0x08)
#define CSR_BT_VCARD_TOKEN_FN                     (0x09)
#define CSR_BT_VCARD_TOKEN_GEO                    (0x0A)
#define CSR_BT_VCARD_TOKEN_KEY                    (0x0B)
#define CSR_BT_VCARD_TOKEN_LABEL                  (0x0C)
#define CSR_BT_VCARD_TOKEN_LOGO                   (0x0D)
#define CSR_BT_VCARD_TOKEN_MAILER                 (0x0E)
#define CSR_BT_VCARD_TOKEN_N                      (0x0F)
#define CSR_BT_VCARD_TOKEN_NAME                   (0x10)
#define CSR_BT_VCARD_TOKEN_NICKNAME               (0x11)
#define CSR_BT_VCARD_TOKEN_NOTE                   (0x12)
#define CSR_BT_VCARD_TOKEN_ORG                    (0x13)
#define CSR_BT_VCARD_TOKEN_PHOTO                  (0x14)
#define CSR_BT_VCARD_TOKEN_PRODID                 (0x15)
#define CSR_BT_VCARD_TOKEN_PROFILE                (0x16)
#define CSR_BT_VCARD_TOKEN_REV                    (0x17)
#define CSR_BT_VCARD_TOKEN_ROLE                   (0x18)
#define CSR_BT_VCARD_TOKEN_SORT_STRING            (0x19)
#define CSR_BT_VCARD_TOKEN_SOUND                  (0x1A)
#define CSR_BT_VCARD_TOKEN_SOURCE                 (0x1B)
#define CSR_BT_VCARD_TOKEN_TEL                    (0x1C)
#define CSR_BT_VCARD_TOKEN_TITLE                  (0x1D)
#define CSR_BT_VCARD_TOKEN_TZ                     (0x1E)
#define CSR_BT_VCARD_TOKEN_UID                    (0x1F)
#define CSR_BT_VCARD_TOKEN_URL                    (0x20)
#define CSR_BT_VCARD_TOKEN_VERSION                (0x21)
#define CSR_BT_VCARD_TOKEN_X_IRMC_CALL_DATETIME   (0x22)
#define CSR_BT_VCARD_TOKEN_X_IRMC_LUID            (0x23)
#define CSR_BT_VCARD_TOKEN_X_NAME                 (0x24)

typedef CsrUint8 CsrBtVcardToken;

#define CSR_BT_VCARD_PARAM_NONE                   (0x00000000)
#define CSR_BT_VCARD_PARAM_VALUE                  (0x00000001)
#define CSR_BT_VCARD_PARAM_CONTEXT                (0x00000002)
#define CSR_BT_VCARD_PARAM_X_NAME                 (0x00000004)
#define CSR_BT_VCARD_PARAM_TYPE                   (0x00000008)
#define CSR_BT_VCARD_PARAM_LANGUAGE               (0x00000010)
#define CSR_BT_VCARD_PARAM_ENCODING               (0x00000020)
#define CSR_BT_VCARD_PARAM_CHARSET                (0x00000040)

typedef CsrUint32 CsrBtVcardParam;

#define CSR_BT_VCARD_STATE_NONE                   (0x00)
#define CSR_BT_VCARD_STATE_BEGIN                  (0x01)
#define CSR_BT_VCARD_STATE_PAYLOAD                (0x02)

typedef CsrUint8 CsrBtVcardState;

#define CSR_BT_VCARD_PARAM_VALUE_UNKNOWN          (0x00000000)
#define CSR_BT_VCARD_PARAM_VALUE_URI              (0x00000001)
#define CSR_BT_VCARD_PARAM_VALUE_DATE             (0x00000002)
#define CSR_BT_VCARD_PARAM_VALUE_DATE_TIME        (0x00000004)
#define CSR_BT_VCARD_PARAM_VALUE_BINARY           (0x00000008)
#define CSR_BT_VCARD_PARAM_VALUE_PTEXT            (0x00000010)

typedef CsrUint32 CsrBtVcardParamValue;

#define CSR_BT_VCARD_ENCODING_NONE                (0x00)
#define CSR_BT_VCARD_ENCODING_BINARY              (0x01)
#define CSR_BT_VCARD_ENCODING_BASE64              (0x02)
#define CSR_BT_VCARD_ENCODING_QUOTED_PRINTABLE    (0x03)
#define CSR_BT_VCARD_ENCODING_7BIT                (0x04)
#define CSR_BT_VCARD_ENCODING_8BIT                (0x05)
#define CSR_BT_VCARD_ENCODING_X_WORD              (0x06)
#define CSR_BT_VCARD_ENCODING_OTHER               (0x07)

typedef CsrUint8 CsrBtVcardEncoding;

typedef struct
{
    const char *            paramName;
    CsrBtVcardParam          param;
} CsrBtVcardParamInfo;

typedef struct
{
    const char *            paramValueName;
    CsrBtVcardParamValue     paramValue;
} CsrBtVcardParamValueInfo;

typedef struct
{
    const char *            paramTypeName;
    CsrBtVcardParamType      paramType;
} CsrBtVcardParamTypeInfo;

typedef struct
{
    const char *            tokenName;
    CsrBtVcardToken          token;
    CsrBtVcardParam          params;
    CsrBtVcardParamValue     paramValue;
    CsrBtVcardParamType      paramType;
} CsrBtVcardTokenInfo;

typedef struct
{
    CsrBool                  valid;
    CsrUint8                 version;
    CsrBtVcardData *         vcardData;
    CsrBtVcardData *         vcardDataList;
    CsrBtVcardTokenInfo *    vcardTokenInfo;
    CsrBtVcardParam          currentParam;
    CsrBtVcardParamValue     paramValue;
    CsrBtVcardParamType      paramType;
    CsrCharString *              paramTypeString;
    CsrCharString *              paramEncodingString;
    CsrBtVcardEncoding       encoding;
    CsrBtVcardState          state;
    CsrUint32                flags;
    CsrUint32                errorCode;
    CsrBool                  version_2_1_construct;
    CsrBool                  version_3_0_construct;
    CsrUint32                stringBufferOffset;
    CsrUint8*                stringBuffer;
    CsrUint32                stringBufferSize;
    CsrUint8                 parseState;
    CsrUint8                 parsePrevState;
    CsrUint8                 parseStoredState;
} CsrBtVcardDataInfo;

#define CSR_BT_VCARD_PARAM_SOURCE      (CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_CONTEXT | CSR_BT_VCARD_PARAM_X_NAME)
#define CSR_BT_VCARD_PARAM_TEXT        (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_LANGUAGE | CSR_BT_VCARD_PARAM_X_NAME)
#define CSR_BT_VCARD_PARAM_IMG_SND     (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_TYPE)
#define CSR_BT_VCARD_PARAM_ADR         (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TEXT | CSR_BT_VCARD_PARAM_TYPE)
#define CSR_BT_VCARD_PARAM_KEY         (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE)

#define CSR_BT_VCARD_PARAM_TYPE_TEL    (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE_HOME | CSR_BT_VCARD_PARAM_TYPE_WORK | CSR_BT_VCARD_PARAM_TYPE_PREF | CSR_BT_VCARD_PARAM_TYPE_VOICE | CSR_BT_VCARD_PARAM_TYPE_FAX | CSR_BT_VCARD_PARAM_TYPE_MSG | CSR_BT_VCARD_PARAM_TYPE_CELL | CSR_BT_VCARD_PARAM_TYPE_PAGER | CSR_BT_VCARD_PARAM_TYPE_BBS | CSR_BT_VCARD_PARAM_TYPE_MODEM | CSR_BT_VCARD_PARAM_TYPE_CAR | CSR_BT_VCARD_PARAM_TYPE_ISDN | CSR_BT_VCARD_PARAM_TYPE_VIDEO | CSR_BT_VCARD_PARAM_TYPE_PCS | CSR_BT_VCARD_PARAM_TYPE_X_NAME | CSR_BT_VCARD_PARAM_TYPE_STR)
#define CSR_BT_VCARD_PARAM_TYPE_EMAIL  (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE_INTERNET | CSR_BT_VCARD_PARAM_TYPE_X400 | CSR_BT_VCARD_PARAM_TYPE_X_NAME | CSR_BT_VCARD_PARAM_TYPE_STR | CSR_BT_VCARD_PARAM_TYPE_PREF)
#define CSR_BT_VCARD_PARAM_TYPE_ADR    (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE_DOM | CSR_BT_VCARD_PARAM_TYPE_INTL | CSR_BT_VCARD_PARAM_TYPE_POSTAL | CSR_BT_VCARD_PARAM_TYPE_PARCEL | CSR_BT_VCARD_PARAM_TYPE_HOME | CSR_BT_VCARD_PARAM_TYPE_WORK | CSR_BT_VCARD_PARAM_TYPE_PREF | CSR_BT_VCARD_PARAM_TYPE_X_NAME | CSR_BT_VCARD_PARAM_TYPE_STR)
#define CSR_BT_VCARD_PARAM_TYPE_KEY    (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE_X509 | CSR_BT_VCARD_PARAM_TYPE_PGP | CSR_BT_VCARD_PARAM_TYPE_X_NAME | CSR_BT_VCARD_PARAM_TYPE_STR)
#define CSR_BT_VCARD_PARAM_TYPE_X_IRMC (CSR_BT_VCARD_PARAM_ENCODING | CSR_BT_VCARD_PARAM_TYPE_MISSED | CSR_BT_VCARD_PARAM_TYPE_RECEIVED | CSR_BT_VCARD_PARAM_TYPE_DIALED)

static const CsrBtVcardTokenInfo xNameToken = { "", CSR_BT_VCARD_TOKEN_X_NAME, CSR_BT_VCARD_PARAM_X_NAME, CSR_BT_VCARD_PARAM_VALUE_UNKNOWN, CSR_BT_VCARD_PARAM_TYPE_UNKNOWN };

static const CsrBtVcardTokenInfo tokenInfoTable[] =
{
    { "adr",                   CSR_BT_VCARD_TOKEN_ADR,                   CSR_BT_VCARD_PARAM_ADR,                                CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_ADR     },
    { "agent",                 CSR_BT_VCARD_TOKEN_AGENT,                 CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_ENCODING,  CSR_BT_VCARD_PARAM_VALUE_URI,                                        CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "bday",                  CSR_BT_VCARD_TOKEN_BDAY,                  CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_ENCODING,  CSR_BT_VCARD_PARAM_VALUE_DATE | CSR_BT_VCARD_PARAM_VALUE_DATE_TIME,    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "begin",                 CSR_BT_VCARD_TOKEN_BEGIN,                 CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "categories",            CSR_BT_VCARD_TOKEN_CATEGORIES,            CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "class",                 CSR_BT_VCARD_TOKEN_CLASS,                 CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "email",                 CSR_BT_VCARD_TOKEN_EMAIL,                 CSR_BT_VCARD_PARAM_TYPE | CSR_BT_VCARD_PARAM_ENCODING,   CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_EMAIL   },
    { "end",                   CSR_BT_VCARD_TOKEN_END,                   CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "fn",                    CSR_BT_VCARD_TOKEN_FN,                    CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "geo",                   CSR_BT_VCARD_TOKEN_GEO,                   CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "key",                   CSR_BT_VCARD_TOKEN_KEY,                   CSR_BT_VCARD_PARAM_KEY,                                CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_KEY     },
    { "label",                 CSR_BT_VCARD_TOKEN_LABEL,                 CSR_BT_VCARD_PARAM_ADR,                                CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_ADR     },
    { "logo",                  CSR_BT_VCARD_TOKEN_LOGO,                  CSR_BT_VCARD_PARAM_IMG_SND,                            CSR_BT_VCARD_PARAM_VALUE_URI | CSR_BT_VCARD_PARAM_VALUE_BINARY,        CSR_BT_VCARD_PARAM_TYPE_STR     },
    { "mailer",                CSR_BT_VCARD_TOKEN_MAILER,                CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "n",                     CSR_BT_VCARD_TOKEN_N,                     CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "name",                  CSR_BT_VCARD_TOKEN_NAME,                  CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "nickname",              CSR_BT_VCARD_TOKEN_NICKNAME,              CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "note",                  CSR_BT_VCARD_TOKEN_NOTE,                  CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "org",                   CSR_BT_VCARD_TOKEN_ORG,                   CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "photo",                 CSR_BT_VCARD_TOKEN_PHOTO,                 CSR_BT_VCARD_PARAM_IMG_SND,                            CSR_BT_VCARD_PARAM_VALUE_URI | CSR_BT_VCARD_PARAM_VALUE_BINARY,        CSR_BT_VCARD_PARAM_TYPE_STR     },
    { "prodid",                CSR_BT_VCARD_TOKEN_PRODID,                CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "profile",               CSR_BT_VCARD_TOKEN_PROFILE,               CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "rev",                   CSR_BT_VCARD_TOKEN_REV,                   CSR_BT_VCARD_PARAM_VALUE | CSR_BT_VCARD_PARAM_ENCODING,  CSR_BT_VCARD_PARAM_VALUE_DATE | CSR_BT_VCARD_PARAM_VALUE_DATE_TIME,    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "role",                  CSR_BT_VCARD_TOKEN_ROLE,                  CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "sort-string",           CSR_BT_VCARD_TOKEN_SORT_STRING,           CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "sound",                 CSR_BT_VCARD_TOKEN_SOUND,                 CSR_BT_VCARD_PARAM_IMG_SND,                            CSR_BT_VCARD_PARAM_VALUE_URI | CSR_BT_VCARD_PARAM_VALUE_BINARY,        CSR_BT_VCARD_PARAM_TYPE_STR     },
    { "source",                CSR_BT_VCARD_TOKEN_SOURCE,                CSR_BT_VCARD_PARAM_SOURCE,                             CSR_BT_VCARD_PARAM_VALUE_URI,                                        CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "tel",                   CSR_BT_VCARD_TOKEN_TEL,                   CSR_BT_VCARD_PARAM_TYPE | CSR_BT_VCARD_PARAM_ENCODING,   CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_TEL     },
    { "title",                 CSR_BT_VCARD_TOKEN_TITLE,                 CSR_BT_VCARD_PARAM_TEXT,                               CSR_BT_VCARD_PARAM_VALUE_PTEXT,                                      CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "tz",                    CSR_BT_VCARD_TOKEN_TZ,                    CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "uid",                   CSR_BT_VCARD_TOKEN_UID,                   CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "url",                   CSR_BT_VCARD_TOKEN_URL,                   CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "version",               CSR_BT_VCARD_TOKEN_VERSION,               CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN },
    { "x-irmc-call-datetime",  CSR_BT_VCARD_TOKEN_X_IRMC_CALL_DATETIME,  CSR_BT_VCARD_PARAM_TYPE | CSR_BT_VCARD_PARAM_ENCODING,   CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_X_IRMC  },
    { "x-irmc-luid",           CSR_BT_VCARD_TOKEN_X_IRMC_LUID,           CSR_BT_VCARD_PARAM_ENCODING,                           CSR_BT_VCARD_PARAM_VALUE_UNKNOWN,                                    CSR_BT_VCARD_PARAM_TYPE_UNKNOWN }
};

static const CsrBtVcardParamInfo paramInfoTable[] =
{
    { "value",      CSR_BT_VCARD_PARAM_VALUE   },
    { "context",    CSR_BT_VCARD_PARAM_CONTEXT     },
    { "type",       CSR_BT_VCARD_PARAM_TYPE    },
    { "language",   CSR_BT_VCARD_PARAM_LANGUAGE },
    { "encoding",   CSR_BT_VCARD_PARAM_ENCODING },
    { "charset",    CSR_BT_VCARD_PARAM_CHARSET }
};

static const CsrBtVcardParamValueInfo paramValueInfoTable[] =
{
    { "uri",        CSR_BT_VCARD_PARAM_VALUE_URI        },
    { "url",        CSR_BT_VCARD_PARAM_VALUE_URI        },
    { "date",       CSR_BT_VCARD_PARAM_VALUE_DATE       },
    { "date-time",  CSR_BT_VCARD_PARAM_VALUE_DATE_TIME     },
    { "binary",     CSR_BT_VCARD_PARAM_VALUE_BINARY     },
    { "ptext",      CSR_BT_VCARD_PARAM_VALUE_PTEXT      }
};

static const CsrBtVcardParamTypeInfo paramTypeInfoTable[] =
{
    { "home",       CSR_BT_VCARD_PARAM_TYPE_HOME       },
    { "work",       CSR_BT_VCARD_PARAM_TYPE_WORK       },
    { "pref",       CSR_BT_VCARD_PARAM_TYPE_PREF       },
    { "voice",      CSR_BT_VCARD_PARAM_TYPE_VOICE      },
    { "fax",        CSR_BT_VCARD_PARAM_TYPE_FAX        },
    { "msg",        CSR_BT_VCARD_PARAM_TYPE_MSG        },
    { "cell",       CSR_BT_VCARD_PARAM_TYPE_CELL       },
    { "pager",      CSR_BT_VCARD_PARAM_TYPE_PAGER      },
    { "bbs",        CSR_BT_VCARD_PARAM_TYPE_BBS        },
    { "modem",      CSR_BT_VCARD_PARAM_TYPE_MODEM      },
    { "car",        CSR_BT_VCARD_PARAM_TYPE_CAR        },
    { "isdn",       CSR_BT_VCARD_PARAM_TYPE_ISDN       },
    { "video",      CSR_BT_VCARD_PARAM_TYPE_VIDEO      },
    { "pcs",        CSR_BT_VCARD_PARAM_TYPE_PCS        },
    { "internet",   CSR_BT_VCARD_PARAM_TYPE_INTERNET   },
    { "x400",       CSR_BT_VCARD_PARAM_TYPE_X400       },
    { "x509",       CSR_BT_VCARD_PARAM_TYPE_X509       },
    { "pgp",        CSR_BT_VCARD_PARAM_TYPE_PGP        },
    { "dom",        CSR_BT_VCARD_PARAM_TYPE_DOM        },
    { "intl",       CSR_BT_VCARD_PARAM_TYPE_INTL       },
    { "postal",     CSR_BT_VCARD_PARAM_TYPE_POSTAL     },
    { "parcel",     CSR_BT_VCARD_PARAM_TYPE_PARCEL     },
    { "missed",     CSR_BT_VCARD_PARAM_TYPE_MISSED     },
    { "received",   CSR_BT_VCARD_PARAM_TYPE_RECEIVED   },
    { "dialed",     CSR_BT_VCARD_PARAM_TYPE_DIALED     },
    { "dialled",    CSR_BT_VCARD_PARAM_TYPE_DIALED     }
};

static const CsrUint8 characterCompressionTable[] = {
/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
    0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  1,  1,  4,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    5,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  8,  9, 10,  6,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 13,  6, 14,  6,  6,
    6, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  6,  6,  6,  6,  6,
    6, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
    11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,  6,  6,  6,  6, 15,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};

static const CsrUint8 characterCompressionTable2[] = {
/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F */
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99,100,101,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
};

static const CsrUint8 nextStateTable[29][17] = {
/*   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16 */
    {  0,  2,  0,  2, 15,  0,  2,  2,  2,  3,  2,  3,  2,  2,  2,  2,  2 },   /*  State: 0 */
    {  0,  2,  0,  2, 15,  0,  2,  2,  2,  3,  2,  3,  2,  2,  2,  2,  2 },   /*  State: 1 */
    {  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },   /*  State: 2 */
    {  0,  2, 20,  2, 15, 20,  2,  2,  2,  3,  4,  3, 13,  5,  2,  2,  2 },   /*  State: 3 */
    {  0,  2, 23,  2, 15, 23,  2,  2,  2,  3,  2,  3,  2,  2,  2,  2,  2 },   /*  State: 4 */
    {  0,  2, 25,  2, 15, 25,  2,  2,  2,  6,  2,  6,  2,  2,  2,  2,  2 },   /*  State: 5 */
    {  0,  2, 21,  2, 15, 21,  2,  2,  2,  6,  2,  6, 27, 26,  7,  2,  2 },   /*  State: 6 */
    {  0,  2, 24,  2, 15, 24, 11, 28, 10, 11, 11, 11, 12, 18, 11,  2, 11 },   /*  State: 7 */
    {  0,  2,  8,  2, 15,  8,  8,  9,  8,  8,  8,  8,  8,  8,  8,  2,  8 },   /*  State: 8 */
    {  0,  2,  2,  2, 15,  2,  2,  2, 10,  2,  2,  2, 12, 18,  2,  2,  2 },   /*  State: 9 */
    {  0,  2,  2,  2, 15,  2, 11,  8, 10, 11, 11, 11, 12, 18, 11,  2, 11 },   /*  State: 10 */
    {  0,  2, 22,  2, 15, 22, 11,  2, 10, 11, 11, 11, 12, 18, 11,  2, 11 },   /*  State: 11 */
    {  1,  2, 14,  2, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  2, 14 },   /*  State: 12 */
    {  1,  2, 14,  2, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  2, 14 },   /*  State: 13 */
    {  1,  2, 14,  2, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  2, 14 },   /*  State: 14 */
    {  0,  2,  2, 16, 15,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },   /*  State: 15 */
    {  1,  2, 17, 19, 15, 17, 19, 19, 19, 19, 19, 19, 19, 19, 19,  2, 19 },   /*  State: 16 */
    {  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },   /*  State: 17 */
    {  0,  2, 25,  2, 15, 25,  2,  2,  2,  6,  2,  6,  2,  2,  2,  2,  2 },   /*  State: 18 */
    {  0,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2 },   /*  State: 19 */
    {  0,  2, 20,  2, 15, 20,  2,  2,  2,  2,  4,  2, 13,  5,  2,  2,  2 },   /*  State: 20 */
    {  0,  2, 21,  2, 15, 21,  2,  2,  2,  2,  2,  2,  2, 26,  7,  2,  2 },   /*  State: 21 */
    {  0,  2, 22,  2, 15, 22,  2,  2,  2,  2,  2,  2, 12, 18,  2,  2,  2 },   /*  State: 22 */
    {  0,  2, 23,  2, 15, 23,  2,  2,  2,  3,  2,  3,  2,  2,  2,  2,  2 },   /*  State: 23 */
    {  0,  2, 24,  2, 15, 24, 11,  8, 10, 11, 11, 11, 12, 18, 11,  2, 11 },   /*  State: 24 */
    {  0,  2, 25,  2, 15, 25,  2,  2,  2,  6,  2,  6,  2,  2,  2,  2,  2 },   /*  State: 25 */
    {  0,  2, 25,  2, 15, 25,  2,  2,  2,  6,  2,  6,  2,  2,  2,  2,  2 },   /*  State: 26 */
    {  1,  2, 14,  2, 15, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  2, 14 },   /*  State: 27 */
    {  0,  2,  8,  2, 15,  8,  8,  9,  8,  8,  8,  8,  8,  8,  8,  2,  8 }    /*  State: 28 */
};

static const CsrUint8 actionTable[29] = {
/*   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28 */
    0,  1,  2,  0,  3,  4,  0,  5,  0,  0, 11,  0,  6,  4,  0,  7,  0,  8,  6,  9,  0,  0,  0,  0,  0,  0, 10, 10,  0
};

static const CsrUint8 saveTable[29] = {
/*   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28 */
    0,  0,  0,  1,  0,  0,  1,  0,  1,  0,  0,  1,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

#define CSR_BT_VCARD_ADD_ADDRESS_STEP_POBOX             (0x00)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_EXTENDED_ADDRESS  (0x01)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_STREET            (0x02)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_LOCALITY          (0x03)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_REGION            (0x04)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_POSTAL_CODE       (0x05)
#define CSR_BT_VCARD_ADD_ADDRESS_STEP_COUNTRY_NAME      (0x06)

#define CSR_BT_VCARD_ADD_NAME_FAMILY_NAME               (0x00)
#define CSR_BT_VCARD_ADD_NAME_GIVE_NAME                 (0x01)
#define CSR_BT_VCARD_ADD_NAME_ADDITIONAL_NAMES          (0x02)
#define CSR_BT_VCARD_ADD_NAME_HONORIFIC_PREFIXES        (0x03)
#define CSR_BT_VCARD_ADD_NAME_HONORIFIC_SUFFIXES        (0x04)

static CsrBool csrBtVcardAddString(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrUint32 flags);

static CsrBool csrBtVcardFixupString(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* string, CsrUint32 stringLength)
{
    CsrUint32    index;
    CsrUint32    offset;
    CsrBool      escape;
    CsrBool      result;
    CsrUint8     state;
    CsrUint8     value;
    CsrUint8*    binaryData;
    CsrUint32    binarySize;

    state  = 0;
    offset = 0;
    value  = 0;
    escape = FALSE;
    result = TRUE;

    if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BASE64 || vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BINARY)
    {
        result = CsrBtBase64Decode(string, stringLength, &binaryData, &binarySize);
        if (result)
        {
            /* Just copy the data since the decoded data size is always smaller than the source data size */
            CsrMemCpy(string, binaryData, binarySize);
            string[binarySize] = 0;

            CsrPmemFree(binaryData);
        }
    }
    else
    {
        for (index = 0; index < stringLength && result; index++)
        {
            if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_QUOTED_PRINTABLE)
            {
                if (state == 0)
                {
                    if (string[index] == '=')
                    {
                        state = 1;
                        value = 0;
                    }
                    else
                    {
                        string[offset++] = string[index];
                    }
                }
                else
                {
                    CsrUint8 tmpval;

                    if (string[index] >= '0' && string[index] <= '9')
                    {
                        tmpval = string[index] - '0';
                    }
                    else if (string[index] >= 'A' && string[index] <= 'F')
                    {
                        tmpval = string[index] - 'A' + 10;
                    }
                    else if (string[index] >= 'a' && string[index] <= 'f')
                    {
                        tmpval = string[index] - 'a' + 10;
                    }
                    else
                    {
                        tmpval = 0;
                    }

                    value = value << 4 | tmpval;
                    if (state == 2)
                    {
                        string[offset++] = value;
                        state = 0;
                    }
                    else
                    {
                        state = 2;
                    }
                }
            }
            else
            {
                if (escape)
                {
                    switch (string[index])
                    {
                        case ';':
                        case ',':
                        case '\\':
                            {
                                string[offset++] = string[index];
                                break;
                            }
                        case 'n':
                        case 'N':
                            {
                                string[offset++] = '\r';
                                string[offset++] = '\n';
                                break;
                            }
                        default:
                            {
                                string[offset++] = '\\';
                                string[offset++] = string[index];
                                break;
                            }
                    }
                    escape = FALSE;
                }
                else if (string[index] == '\\')
                {
                    escape = TRUE;
                }
                else
                {
                    string[offset++] = string[index];
                }
            }
        }

        string[offset] = 0;
    }

    if (state != 0)
    {
        result = FALSE;
    }

    return result;
}

static CsrBool csrBtVcardConvertToUnicode(CsrCharString* inputString, CsrUint32 inputLength, CsrCharString* output)
{
    CsrUint32  i;
    CsrUint32  offset = 0;
    CsrUint16* outputString = (CsrUint16*) output;
    CsrUint8* input = (CsrUint8*) inputString;

    for (i = 0; i < inputLength; i++)
    {
        if (input[i] < 0x80)
        {
            outputString[offset++] = input[i];
        }
        else if (input[i] >= 0xC2 && input[i] <= 0xDF)
        {
            outputString[offset++] = (CsrUint16)((((input[i] & 0x03) << 6) + (input[i + 1] & 0x3F)) | (((CsrUint16) (input[i] & 0x1C)) << 6));
            i += 1;
        }
        else if (input[i] >= 0xE0 && input[i] <= 0xEF)
        {
            outputString[offset++] = (CsrUint16)((((input[i + 1] & 0x03) << 6) + (input[i + 2] & 0x3F)) + (((input[i] & 0x0F) << 12) + ((input[i + 1] & 0x3C) << 6)));
            i += 2;
        }
    }

    outputString[offset] = 0;

    return TRUE;
}

static CsrBool csrBtVcardAddAddress(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrBtVcardParamType paramType, CsrUint32 flags)
{
    CsrBool                  result = FALSE;
    CsrBool                  escape = FALSE;
    CsrUint32                index;
    CsrUint32                position;
    CsrBtVcardDataAddress*   address;
    CsrCharString *              tempString;
    CsrUint8                 step;

    if (dataType)
    {
        dataType->payload.address   = (CsrBtVcardDataAddress*) CsrPmemAlloc(sizeof(CsrBtVcardDataAddress));
        dataType->dataType          = flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS ? CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2 : CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8;
        step                        = CSR_BT_VCARD_ADD_ADDRESS_STEP_POBOX;
        address                     = dataType->payload.address;

        if (address)
        {
            position                    = 0;
            address->addressType        = paramType;
            address->poBox              = NULL;
            address->extendedAddress    = NULL;
            address->street             = NULL;
            address->locality           = NULL;
            address->region             = NULL;
            address->postalCode         = NULL;
            address->countryName        = NULL;

            for (index = 0; index < stringLength; index++)
            {
                if ((string[index] == ';' && !escape) || (index + 1 == stringLength))
                {
                    if (index > position)
                    {
                        result = csrBtVcardFixupString(vcardDataInfo, &string[position], index - position);

                        if (!result)
                        {
                            break;
                        }

                        if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
                        {
                            tempString = (CsrCharString*) CsrPmemAlloc((index - position + 1) << 1);
                            if (tempString)
                            {
                                if (!csrBtVcardConvertToUnicode((CsrCharString*)&string[position], index - position, tempString))
                                {
                                    CsrPmemFree(tempString);
                                    tempString = NULL;
                                }
                                else
                                {
                                    CsrUint16 * tempStringRef = (CsrUint16 *) tempString;

                                    tempStringRef[index - position] = '\0';
                                }
                            }
                        }
                        else
                        {
                            tempString = (CsrCharString*) CsrPmemAlloc(index - position + 1);
                            if (tempString)
                            {
                                CsrMemCpy(tempString, &string[position], index - position);
                                tempString[index - position] = '\0';
                            }
                        }
                    }
                    else
                    {
                        tempString = NULL;
                    }

                    position = index + 1;

                    switch (step)
                    {
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_POBOX:
                            {
                                address->poBox = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_EXTENDED_ADDRESS:
                            {
                                address->extendedAddress = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_STREET:
                            {
                                address->street = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_LOCALITY:
                            {
                                address->locality = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_REGION:
                            {
                                address->region = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_POSTAL_CODE:
                            {
                                address->postalCode = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_ADDRESS_STEP_COUNTRY_NAME:
                            {
                                address->countryName = tempString;
                                break;
                            }
                        default:
                            {
                                CsrPmemFree(tempString);
                                break;
                            }
                    }

                    step++;
                }
                else if (string[index] == '\\')
                {
                    escape = TRUE;
                }
                else
                {
                    escape = FALSE;
                }
            }

            result = TRUE;
        }
    }

    return result;
}

static CsrBool csrBtVcardAddBinary(CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrCharString* paramTypeValue, CsrUint32 flags)
{
    CsrBtVcardDataBinary*    binary;
    CsrBool                  result = FALSE;

    if (dataType)
    {
        dataType->payload.binary    = (CsrBtVcardDataBinary*) CsrPmemAlloc(sizeof(CsrBtVcardDataBinary));
        dataType->dataType          = flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS ? CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2 : CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8;
        binary                      = dataType->payload.binary;

        if (binary)
        {
            result = CsrBtBase64Decode(string, stringLength, &binary->binaryData, &binary->binarySize);

            binary->binaryType = paramTypeValue;
        }
    }

    return result;
}

static CsrBtVcardDataType* csrBtVcardAddDataType(CsrBtVcardDataType** dataType)
{
    CsrBtVcardDataType*  tempDataType;
    CsrBtVcardDataType*  nextDataType;

    tempDataType = (CsrBtVcardDataType*) CsrPmemAlloc(sizeof(CsrBtVcardDataType));
    if (tempDataType)
    {
        tempDataType->next           = NULL;
        tempDataType->dataType       = CSR_BT_VCARD_DATA_TYPE_NONE;
        tempDataType->payload.string = NULL;

        if (*dataType)
        {
            nextDataType = (*dataType);
            while(nextDataType->next)
            {
                nextDataType = nextDataType->next;
            }

            nextDataType->next = tempDataType;
        }
        else
        {
            *dataType = tempDataType;
        }
    }

    return tempDataType;
}

static CsrBool csrBtVcardAddDateTimeData(CsrBtVcardDataType* dataType, CsrCharString* string, CsrBtVcardParamValue paramValue, CsrBtVcardParamType paramType, CsrBool typedData)
{
    CsrBool result = TRUE;

    if (paramValue == CSR_BT_VCARD_PARAM_VALUE_DATE)
    {
        CsrBtIso8601Date* iso8601Date;

        if (typedData)
        {
            dataType->payload.iso8601DateTyped           = (CsrBtVcardDataIso8601Date*) CsrPmemAlloc(sizeof(CsrBtVcardDataIso8601Date));
            dataType->dataType                           = CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE_TYPED;
            iso8601Date                                  = &dataType->payload.iso8601DateTyped->iso8601Date;
            dataType->payload.iso8601DateTyped->type     = paramType;
        }
        else
        {
            dataType->payload.iso8601Date                = (CsrBtIso8601Date*) CsrPmemAlloc(sizeof(CsrBtIso8601Date));
            dataType->dataType                           = CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE;
            iso8601Date                                  = dataType->payload.iso8601Date;
        }

        if (string)
        {
            result = Iso8601DateToCsrBtDate(string, iso8601Date, CSR_BT_ISO8601_FLAGS_EXTENDED | CSR_BT_ISO8601_FLAGS_ALLOW_SECOND_FACTION);
        }
        else
        {
            CsrMemSet(iso8601Date, 0, sizeof(*iso8601Date));
        }
    }
    else
    {
        CsrBtIso8601DateTime* iso8601DateTime;

        if (typedData)
        {
            dataType->payload.iso8601DateTimeTyped       = (CsrBtVcardDataIso8601DateTime*) CsrPmemAlloc(sizeof(CsrBtVcardDataIso8601DateTime));
            dataType->dataType                           = CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED;
            iso8601DateTime                              = &dataType->payload.iso8601DateTimeTyped->iso8601DateTime;
            dataType->payload.iso8601DateTimeTyped->type = paramType;
        }
        else
        {
            dataType->payload.iso8601DateTime            = (CsrBtIso8601DateTime*) CsrPmemAlloc(sizeof(CsrBtIso8601DateTime));
            dataType->dataType                           = CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME;
            iso8601DateTime                              = dataType->payload.iso8601DateTime;
        }

        if (string)
        {
            result = Iso8601DateToCsrBtDateTime(string, iso8601DateTime, CSR_BT_ISO8601_FLAGS_EXTENDED | CSR_BT_ISO8601_FLAGS_ALLOW_SECOND_FACTION);
        }
        else
        {
            CsrMemSet(iso8601DateTime, 0, sizeof(*iso8601DateTime));
        }
    }

    return result;
}

static CsrBool csrBtVcardAddDateTime(CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrBtVcardParamValue paramValue, CsrBtVcardParamType paramType, CsrBool typedData)
{
    CsrCharString*   tempString = NULL;
    CsrUint32    index;
    CsrUint32    position = 0;
    CsrBool      result = TRUE;

    if (dataType)
    {
        dataType->dataType = CSR_BT_VCARD_DATA_TYPE_NONE;

        position = 0;

        for (index = 0; index < stringLength; index++)
        {
            if (string[index] == ',' || (index + 1 == stringLength))
            {
                if (index > position)
                {
                    tempString = (CsrCharString*) CsrPmemAlloc(index - position + 1);
                    if (tempString)
                    {
                        CsrMemCpy(tempString, &string[position], index - position);
                        tempString[index - position] = '\0';
                    }
                }
                else
                {
                    tempString = NULL;
                }

                position = index + 1;
            }

            if (tempString)
            {
                result = csrBtVcardAddDateTimeData(dataType, tempString, paramValue, paramType, typedData);

                CsrPmemFree(tempString);
                tempString = NULL;

                break;
            }
        }

        if (dataType->dataType == CSR_BT_VCARD_DATA_TYPE_NONE)
        {
            result = csrBtVcardAddDateTimeData(dataType, NULL, paramValue, paramType, typedData);
        }
    }

    return result;
}

static CsrBool csrBtVcardAddGeo(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrUint32 flags)
{
    return csrBtVcardAddString(vcardDataInfo, dataType, string, stringLength, flags);
}

static CsrBool csrBtVcardAddTypedString(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrBtVcardParamType paramType, CsrUint32 flags)
{
    CsrBool result = FALSE;

    if (dataType)
    {
        CsrBtVcardDataTypedString* typedString;

        dataType->payload.typedString = (CsrBtVcardDataTypedString*) CsrPmemAlloc(sizeof(CsrBtVcardDataTypedString));
        typedString                   = dataType->payload.typedString;

        if (typedString)
        {
            typedString->type = paramType;
            result            = csrBtVcardFixupString(vcardDataInfo, string, stringLength);

            if (result)
            {
                if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
                {
                    typedString->string = (CsrCharString*) CsrPmemAlloc((stringLength + 1) << 1);
                    dataType->dataType  = CSR_BT_VCARD_DATA_TYPE_LABEL_UCS2;

                    if (typedString->string)
                    {
                        result = csrBtVcardConvertToUnicode((CsrCharString*)string, stringLength, typedString->string);
                    }
                }
                else
                {
                    typedString->string = (CsrCharString*) CsrPmemAlloc(stringLength);
                    dataType->dataType  = CSR_BT_VCARD_DATA_TYPE_LABEL_UTF8;

                    if (typedString->string)
                    {
                        CsrMemCpy(typedString->string, string, stringLength);

                        result = TRUE;
                    }
                }
            }
        }
    }

    return result;
}

static CsrBool csrBtVcardAddName(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrUint32 flags)
{
    CsrBool              result = FALSE;
    CsrBool              escape = FALSE;
    CsrUint32            index;
    CsrUint32            position;
    CsrBtVcardDataName*  name;
    CsrCharString*           tempString;
    CsrUint8             step;

    if (dataType)
    {
        dataType->payload.name  = (CsrBtVcardDataName*) CsrPmemAlloc(sizeof(CsrBtVcardDataName));
        dataType->dataType      = flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS ? CSR_BT_VCARD_DATA_TYPE_NAME_UCS2 : CSR_BT_VCARD_DATA_TYPE_NAME_UTF8;
        step                    = CSR_BT_VCARD_ADD_NAME_FAMILY_NAME;
        name                    = dataType->payload.name;

        if (name)
        {
            position                = 0;
            name->familyName        = NULL;
            name->givenName         = NULL;
            name->additionalNames   = NULL;
            name->honorificPrefixes = NULL;
            name->honorificSuffixes = NULL;

            for (index = 0; index < stringLength; index++)
            {
                if ((string[index] == ';' && !escape) || (index + 1 == stringLength))
                {
                    if (index > position)
                    {
                        result = csrBtVcardFixupString(vcardDataInfo, &string[position], index - position);

                        if (!result)
                        {
                            break;
                        }

                        if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
                        {
                            tempString = (CsrCharString*) CsrPmemAlloc((index - position + 1) << 1);
                            if (tempString)
                            {
                                if (!csrBtVcardConvertToUnicode((CsrCharString*)&string[position], index - position, tempString))
                                {
                                    CsrPmemFree(tempString);
                                    tempString = NULL;
                                }
                                else
                                {
                                    CsrUint16 * tempStringRef = (CsrUint16 *) tempString;

                                    tempStringRef[index - position] = '\0';
                                }
                            }
                        }
                        else
                        {
                            tempString = (CsrCharString*) CsrPmemAlloc(index - position + 1);
                            if (tempString)
                            {
                                CsrMemCpy(tempString, &string[position], index - position);
                                tempString[index - position] = '\0';
                            }
                        }
                    }
                    else
                    {
                        tempString = NULL;
                    }

                    position = index + 1;

                    switch (step)
                    {
                        case CSR_BT_VCARD_ADD_NAME_FAMILY_NAME:
                            {
                                name->familyName = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_NAME_GIVE_NAME:
                            {
                                name->givenName = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_NAME_ADDITIONAL_NAMES:
                            {
                                name->additionalNames = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_NAME_HONORIFIC_PREFIXES:
                            {
                                name->honorificPrefixes = tempString;
                                break;
                            }
                        case CSR_BT_VCARD_ADD_NAME_HONORIFIC_SUFFIXES:
                            {
                                name->honorificSuffixes = tempString;
                                break;
                            }
                        default:
                            {
                                CsrPmemFree(tempString);
                                break;
                            }
                    }

                    step++;
                }
                else if (string[index] == '\\')
                {
                    escape = TRUE;
                }
                else
                {
                    escape = FALSE;
                }
            }

            result = TRUE;
        }
    }

    return result;
}

static CsrBool csrBtVcardAddString(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrUint32 flags)
{
    CsrBool result = FALSE;

    if (dataType)
    {
        result = csrBtVcardFixupString(vcardDataInfo, string, stringLength);
        if (result)
        {
            if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
            {
                dataType->payload.string    = (CsrCharString*) CsrPmemAlloc((stringLength + 1) << 1);
                dataType->dataType          = CSR_BT_VCARD_DATA_TYPE_STRING_UCS2;

                if (dataType->payload.string)
                {
                    result = csrBtVcardConvertToUnicode((CsrCharString*)string, stringLength, dataType->payload.string);
                }
            }
            else
            {
                dataType->payload.string    = (CsrCharString*) CsrPmemAlloc(stringLength);
                dataType->dataType          = CSR_BT_VCARD_DATA_TYPE_STRING_UTF8;

                if (dataType->payload.string)
                {
                    CsrMemCpy(dataType->payload.string, string, stringLength);

                    result = TRUE;
                }
            }
        }
    }

    return result;
}

static CsrBool csrBtVcardAddTelephone(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrBtVcardParamType paramType, CsrUint32 flags)
{
    CsrBtVcardDataTelephone* telephone;
    CsrBool                  result = FALSE;

    result = csrBtVcardFixupString(vcardDataInfo, string, stringLength);
    if (result && dataType)
    {
        dataType->payload.telephone = (CsrBtVcardDataTelephone*) CsrPmemAlloc(sizeof(CsrBtVcardDataTelephone));
        telephone                   = dataType->payload.telephone;

        if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
        {
            dataType->dataType = CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UCS2;
            telephone->number  = (CsrCharString*) CsrPmemAlloc((stringLength + 1) << 1);
            telephone->type    = paramType;

            if (telephone->number)
            {
                result = csrBtVcardConvertToUnicode((CsrCharString*)string, stringLength, telephone->number);
            }
        }
        else
        {
            dataType->dataType = CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UTF8;
            telephone->number = (CsrCharString*) CsrPmemAlloc(stringLength);
            telephone->type   = paramType;

            if (telephone->number)
            {
                CsrMemCpy(telephone->number, string, stringLength);

                result = TRUE;
            }
        }
    }

    return result;
}

static CsrBool csrBtVcardAddUndetermined(CsrBtVcardDataType* dataType, CsrUint8* encoding)
{
    CsrBool result = FALSE;

    if (dataType)
    {
        dataType->dataType = CSR_BT_VCARD_DATA_TYPE_UNDETERMINED;

        CsrPmemFree(encoding);

        result = TRUE;
    }

    return result;
}

static CsrBool csrBtVcardAddUriString(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* uriString, CsrUint32 uriStringLength, CsrUint32 flags)
{
    CsrBool              result = FALSE;

    if (dataType)
    {
        result = csrBtVcardFixupString(vcardDataInfo, uriString, uriStringLength);
        if (result)
        {
            dataType->payload.uriString       = (CsrBtVcardDataUriString*) CsrPmemAlloc(sizeof(CsrBtVcardDataUriString));
            dataType->payload.uriString->type = vcardDataInfo->paramTypeString;

            if (vcardDataInfo->paramTypeString)
            {
                vcardDataInfo->paramTypeString = NULL;
            }

            if (flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
            {
                dataType->payload.uriString->string = (CsrCharString*) CsrPmemAlloc((uriStringLength + 1) << 1);
                dataType->dataType                  = CSR_BT_VCARD_DATA_TYPE_URI_UCS2;

                result = csrBtVcardConvertToUnicode((CsrCharString*)uriString, uriStringLength, dataType->payload.uriString->string);
            }
            else
            {
                dataType->payload.uriString->string = (CsrCharString*) CsrPmemAlloc(uriStringLength);
                dataType->dataType                  = CSR_BT_VCARD_DATA_TYPE_URI_UTF8;

                CsrMemCpy(dataType->payload.uriString->string, uriString, uriStringLength);
                result = TRUE;
            }
        }
    }

    return result;
}

static CsrBool csrBtVcardAddUtcOffsetValue(CsrBtVcardDataInfo* vcardDataInfo, CsrBtVcardDataType* dataType, CsrUint8* string, CsrUint32 stringLength, CsrUint32 flags)
{
    return csrBtVcardAddString(vcardDataInfo, dataType, string, stringLength, flags);
}


static CsrBool csrBtVcardStrCompare(CsrUint8* s1, CsrUint8* s2)
{
    CsrBool result = TRUE;

    while (*s1 && *s2)
    {
        if (characterCompressionTable2[*s1] != characterCompressionTable2[*s2])
        {
            result = FALSE;
            break;
        }

        s1++;
        s2++;
    }

    return !*s1 && !*s2 && result;
}

static CsrBool csrBtVcardStrXname(CsrUint8* s1)
{
    CsrBool result = FALSE;
    CsrInt8 index;

    index = 0;

    while (*s1)
    {
        if (index == 0 && characterCompressionTable2[*s1] != characterCompressionTable2['x'])
        {
            break;
        }
        else if (index == 1 && characterCompressionTable2[*s1] != characterCompressionTable2['-'])
        {
            break;
        }
        else if (index == 2)
        {
            result = TRUE;
            break;
        }

        index++;
        s1++;
    }

    return result;
}

static void csrBtVcardLineGroup(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    /* We don't support grouping and simply ignores it */

    CSR_UNUSED(vcardDataInfo);
    CSR_UNUSED(line);
    CSR_UNUSED(lineLength);
}

static void csrBtVcardLineName(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    CsrUint32    index;

    CSR_UNUSED(lineLength);

    vcardDataInfo->vcardTokenInfo       = NULL;
    vcardDataInfo->paramTypeString      = NULL;
    vcardDataInfo->paramEncodingString  = NULL;
    vcardDataInfo->paramType            = CSR_BT_VCARD_PARAM_TYPE_UNKNOWN;
    vcardDataInfo->paramValue           = CSR_BT_VCARD_PARAM_VALUE_UNKNOWN;
    vcardDataInfo->encoding             = CSR_BT_VCARD_ENCODING_NONE;
    vcardDataInfo->currentParam         = CSR_BT_VCARD_PARAM_NONE;

    for (index = 0; index < (sizeof(tokenInfoTable) / sizeof(CsrBtVcardTokenInfo)); index++)
    {
        if (csrBtVcardStrCompare((CsrUint8*) tokenInfoTable[index].tokenName, line))
        {
            vcardDataInfo->vcardTokenInfo = (CsrBtVcardTokenInfo*) &tokenInfoTable[index];
            break;
        }
    }

    if (!vcardDataInfo->vcardTokenInfo)
    {
        if (csrBtVcardStrXname(line))
        {
            vcardDataInfo->vcardTokenInfo = (CsrBtVcardTokenInfo*) &xNameToken;
        }

        if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES)
        {
            vcardDataInfo->errorCode      |= CSR_BT_VCARD_ERROR_UNKNOWN_ATTRIBUTE_NAME;
            vcardDataInfo->vcardTokenInfo = (CsrBtVcardTokenInfo*) &xNameToken;
        }
    }

    if (vcardDataInfo->vcardTokenInfo)
    {
        if (!(vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN))
        {
            if ((vcardDataInfo->state == CSR_BT_VCARD_STATE_NONE && vcardDataInfo->vcardTokenInfo->token != CSR_BT_VCARD_TOKEN_BEGIN) ||
                (vcardDataInfo->state == CSR_BT_VCARD_STATE_BEGIN && (vcardDataInfo->vcardTokenInfo->token == CSR_BT_VCARD_TOKEN_BEGIN || vcardDataInfo->vcardTokenInfo->token == CSR_BT_VCARD_TOKEN_END)) ||
                (vcardDataInfo->state == CSR_BT_VCARD_STATE_PAYLOAD && vcardDataInfo->vcardTokenInfo->token == CSR_BT_VCARD_TOKEN_BEGIN))
            {
                vcardDataInfo->valid     = FALSE;
                vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
            }
        }
    }
    else
    {
        vcardDataInfo->valid     = FALSE;
        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_ATTRIBUTE_NAME;
    }
}

static void csrBtVcardLineParamName(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    CsrUint32        index;
    CsrBtVcardParam  param = CSR_BT_VCARD_PARAM_NONE;

    CSR_UNUSED(lineLength);

    if (!vcardDataInfo->vcardTokenInfo)
    {
        return;
    }

    for (index = 0; index < (sizeof(paramInfoTable) / sizeof(CsrBtVcardParamInfo)); index++)
    {
        if (csrBtVcardStrCompare((CsrUint8*) paramInfoTable[index].paramName, line))
        {
            param = paramInfoTable[index].param;
            break;
        }
    }

    if (param == CSR_BT_VCARD_PARAM_NONE && csrBtVcardStrXname(line))
    {
        param = CSR_BT_VCARD_PARAM_X_NAME;
    }

    if (param != CSR_BT_VCARD_PARAM_NONE)
    {
        if (vcardDataInfo->vcardTokenInfo->params & param)
        {
            vcardDataInfo->currentParam = param;
        }
    }
    else
    {
        if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES)
        {
            vcardDataInfo->errorCode    |= CSR_BT_VCARD_ERROR_UNKNOWN_PARAMETER_NAME;
            param                       = CSR_BT_VCARD_PARAM_X_NAME;
        }
        else
        {
            vcardDataInfo->errorCode    |= CSR_BT_VCARD_ERROR_PARAMETER_NAME;
            vcardDataInfo->valid        = FALSE;
        }
    }
}

static void csrBtVcardLineParamValue(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    CsrUint32 index;

    if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_VALUE)
    {
        CsrBtVcardParamValue paramValue = CSR_BT_VCARD_PARAM_VALUE_UNKNOWN;

        for (index = 0; index < (sizeof(paramValueInfoTable) / sizeof(CsrBtVcardParamValueInfo)); index++)
        {
            if (csrBtVcardStrCompare((CsrUint8*) paramValueInfoTable[index].paramValueName, line))
            {
                paramValue = paramValueInfoTable[index].paramValue;
                break;
            }
        }

        if (paramValue == CSR_BT_VCARD_PARAM_VALUE_UNKNOWN || vcardDataInfo->paramValue != CSR_BT_VCARD_PARAM_VALUE_UNKNOWN ||
            !(vcardDataInfo->vcardTokenInfo->paramValue & paramValue))
        {
            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
            vcardDataInfo->valid     = FALSE;
        }
        else
        {
            vcardDataInfo->paramValue |= paramValue;
        }
    }
    else if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_CONTEXT)
    {
        /* We don't care about this entry */
    }
    else if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_X_NAME)
    {
        /* We don't care about this entry */
    }
    else if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_TYPE)
    {
        CsrBtVcardParamType paramType = CSR_BT_VCARD_PARAM_TYPE_UNKNOWN;

        for (index = 0; index < (sizeof(paramTypeInfoTable) / sizeof(CsrBtVcardParamTypeInfo)); index++)
        {
            if (csrBtVcardStrCompare((CsrUint8*) paramTypeInfoTable[index].paramTypeName, line))
            {
                paramType = paramTypeInfoTable[index].paramType;
                break;
            }
        }

        if (paramType == CSR_BT_VCARD_PARAM_TYPE_UNKNOWN)
        {
            if (csrBtVcardStrXname(line))
            {
                paramType = CSR_BT_VCARD_PARAM_TYPE_X_NAME;
            }
            else
            {
                paramType = CSR_BT_VCARD_PARAM_TYPE_STR;
            }

            if (!vcardDataInfo->paramTypeString)
            {
                if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
                {
                    vcardDataInfo->paramTypeString = (CsrCharString*) CsrPmemAlloc((lineLength + 1) << 1);
                    if (vcardDataInfo->paramTypeString)
                    {
                        csrBtVcardConvertToUnicode((CsrCharString*)line, lineLength, vcardDataInfo->paramTypeString);
                    }
                }
                else
                {
                    vcardDataInfo->paramTypeString = (CsrCharString*) CsrPmemAlloc(lineLength);
                    if (vcardDataInfo->paramTypeString)
                    {
                        CsrMemCpy(vcardDataInfo->paramTypeString, line, lineLength);
                    }
                }
            }
        }

        if (vcardDataInfo->vcardTokenInfo->paramType & paramType)
        {
            vcardDataInfo->paramType |= paramType;
        }
        else
        {
            if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_DISABLE_2_1_TYPE_CHECK)
            {
                vcardDataInfo->version_2_1_construct = TRUE;
                vcardDataInfo->paramType |= paramType;
            }
            else
            {
                vcardDataInfo->valid     = FALSE;
                vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
            }
        }
    }
    else if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_LANGUAGE)
    {
        /* We don't care about this entry */
    }
    else if (vcardDataInfo->currentParam & CSR_BT_VCARD_PARAM_ENCODING)
    {
        if (csrBtVcardStrCompare((CsrUint8*) "b", line))
        {
            vcardDataInfo->encoding                 = CSR_BT_VCARD_ENCODING_BINARY;
            vcardDataInfo->version_3_0_construct    = TRUE;
        }
        else if (csrBtVcardStrCompare((CsrUint8*) "base64", line))
        {
            vcardDataInfo->encoding                 = CSR_BT_VCARD_ENCODING_BASE64;
            vcardDataInfo->version_2_1_construct    = TRUE;
        }
        else if (csrBtVcardStrCompare((CsrUint8*) "quoted-printable", line))
        {
            vcardDataInfo->encoding                 = CSR_BT_VCARD_ENCODING_QUOTED_PRINTABLE;
            vcardDataInfo->version_2_1_construct    = TRUE;
        }

        if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_NONE)
        {
            if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING)
            {
                vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING;

                if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_UCS2_STRINGS)
                {
                    vcardDataInfo->paramEncodingString = (CsrCharString*) CsrPmemAlloc((lineLength + 1) << 1);
                    if (vcardDataInfo->paramEncodingString)
                    {
                        csrBtVcardConvertToUnicode((CsrCharString*)line, lineLength, vcardDataInfo->paramEncodingString);
                    }
                }
                else
                {
                    vcardDataInfo->paramEncodingString = (CsrCharString*) CsrPmemAlloc(lineLength);
                    if (vcardDataInfo->paramEncodingString)
                    {
                        CsrMemCpy(vcardDataInfo->paramEncodingString, line, lineLength);
                    }
                }

                vcardDataInfo->encoding = CSR_BT_VCARD_ENCODING_OTHER;
            }
            else
            {
                vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_ENCODING;
                vcardDataInfo->valid = FALSE;
            }
        }
    }

    vcardDataInfo->currentParam = 0;
}

static void csrBtVcardLineParamValueNoName(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    vcardDataInfo->currentParam          = CSR_BT_VCARD_PARAM_TYPE;
    vcardDataInfo->version_2_1_construct = TRUE;

    csrBtVcardLineParamValue(vcardDataInfo, line, lineLength);
}

static void csrBtVcardLinePayload(CsrBtVcardDataInfo* vcardDataInfo, CsrUint8* line, CsrUint32 lineLength)
{
    switch (vcardDataInfo->vcardTokenInfo->token)
    {
        case CSR_BT_VCARD_TOKEN_ADR:
            {
                vcardDataInfo->valid = csrBtVcardAddAddress(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->address),
                    line, lineLength, vcardDataInfo->paramType, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_AGENT:
            {
                if (vcardDataInfo->paramValue & CSR_BT_VCARD_PARAM_VALUE_URI)
                {
                    vcardDataInfo->valid = csrBtVcardAddUriString(
                        vcardDataInfo,
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobAgent),
                        line, lineLength, vcardDataInfo->flags);
                }
                else
                {
                    vcardDataInfo->valid = csrBtVcardAddString(
                        vcardDataInfo,
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobAgent),
                        line, lineLength, vcardDataInfo->flags);
                }

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_BDAY:
            {
                vcardDataInfo->valid = csrBtVcardAddDateTime(
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->birthDate),
                    line, lineLength, vcardDataInfo->paramValue, vcardDataInfo->paramType, FALSE);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_BEGIN:
            {
                if (csrBtVcardStrCompare(line, (CsrUint8*) "vcard") && vcardDataInfo->state == CSR_BT_VCARD_STATE_NONE)
                {
                    vcardDataInfo->state                    = CSR_BT_VCARD_STATE_BEGIN;
                    vcardDataInfo->version                  = CSR_BT_VCARD_VERSION_UNKNOWN;
                    vcardDataInfo->currentParam             = CSR_BT_VCARD_PARAM_NONE;
                    vcardDataInfo->version_2_1_construct    = FALSE;
                    vcardDataInfo->version_3_0_construct    = FALSE;
                    vcardDataInfo->errorCode                = CSR_BT_VCARD_ERROR_INCOMPLETE;
                    vcardDataInfo->vcardData                = (CsrBtVcardData*) CsrPmemAlloc(sizeof(CsrBtVcardData));

                    if (vcardDataInfo->vcardData)
                    {
                        CsrMemSet(vcardDataInfo->vcardData, 0, sizeof(CsrBtVcardData));
                    }
                    else
                    {
                        vcardDataInfo->valid = FALSE;
                    }
                }
                else
                {
                    vcardDataInfo->valid = FALSE;
                }

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_CATEGORIES:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->catagories),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_CLASS:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->classification),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_EMAIL:
            {
                vcardDataInfo->valid = csrBtVcardAddTypedString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->email),
                    line, lineLength, vcardDataInfo->paramType, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_END:
            {
                if (csrBtVcardStrCompare(line, (CsrUint8*) "vcard") && vcardDataInfo->state == CSR_BT_VCARD_STATE_PAYLOAD)
                {
                    if (!(vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_NO_MANDATORY_TAGS) && (!vcardDataInfo->vcardData->formattedName ||
                                                                                           !vcardDataInfo->vcardData->name || vcardDataInfo->version == CSR_BT_VCARD_VERSION_UNKNOWN))
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_MISSING_MANDATORY_TAGS;
                        vcardDataInfo->valid     = FALSE;
                    }
                    else if (!(vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_NO_VERSION_MATCH) && vcardDataInfo->version_2_1_construct && vcardDataInfo->version_3_0_construct)
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_MULTIPLE_VERSION_CONSTRUCT;
                        vcardDataInfo->valid     = FALSE;
                    }
                    else
                    {
                        CsrBtVcardData* nextVcardData;

                        if (vcardDataInfo->vcardDataList)
                        {
                            nextVcardData = vcardDataInfo->vcardDataList;
                            while(nextVcardData->next)
                            {
                                nextVcardData = nextVcardData->next;
                            }

                            nextVcardData->next = vcardDataInfo->vcardData;
                        }
                        else
                        {
                            vcardDataInfo->vcardDataList = vcardDataInfo->vcardData;
                        }

                        vcardDataInfo->vcardData = NULL;

                        vcardDataInfo->errorCode &= ~CSR_BT_VCARD_ERROR_INCOMPLETE;
                    }

                    vcardDataInfo->state = CSR_BT_VCARD_STATE_NONE;
                }
                else
                {
                    vcardDataInfo->valid     = FALSE;
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_FN:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->formattedName),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_GEO:
            {
                vcardDataInfo->valid = csrBtVcardAddGeo(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->globalPosition),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_KEY:
            {
                if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BINARY || vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BASE64)
                {
                    vcardDataInfo->valid = csrBtVcardAddBinary(
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->key),
                        line, lineLength, vcardDataInfo->paramTypeString, vcardDataInfo->flags);

                    if (vcardDataInfo->valid)
                    {
                        vcardDataInfo->paramTypeString = NULL;
                    }
                    else
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }
                }
                else if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_NONE)
                {
                    vcardDataInfo->valid = csrBtVcardAddString(
                        vcardDataInfo,
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->key),
                        line, lineLength, vcardDataInfo->flags);

                    if (!vcardDataInfo->valid)
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING)
                    {
                        vcardDataInfo->valid = csrBtVcardAddUndetermined(
                            csrBtVcardAddDataType(&vcardDataInfo->vcardData->key),
                            (CsrUint8*)vcardDataInfo->paramEncodingString);
                        if (vcardDataInfo->valid)
                        {
                            vcardDataInfo->paramEncodingString = NULL;
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                        }
                        else
                        {
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING;
                        }
                    }
                    else
                    {
                        vcardDataInfo->valid     = FALSE;
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_ENCODING;
                    }
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_LABEL:
            {
                vcardDataInfo->valid = csrBtVcardAddTypedString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->label),
                    line, lineLength, vcardDataInfo->paramType, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_LOGO:
            {
                if (vcardDataInfo->paramValue & CSR_BT_VCARD_PARAM_VALUE_URI)
                {
                    vcardDataInfo->valid = csrBtVcardAddUriString(
                        vcardDataInfo,
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobLogo),
                        line, lineLength, vcardDataInfo->flags);

                    if (!vcardDataInfo->valid)
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }

                }
                else if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BINARY || vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BASE64)
                {
                    vcardDataInfo->valid = csrBtVcardAddBinary(
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobLogo),
                        line, lineLength, vcardDataInfo->paramTypeString, vcardDataInfo->flags);

                    if (vcardDataInfo->valid)
                    {
                        vcardDataInfo->paramTypeString = NULL;
                    }
                    else
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING)
                    {
                        vcardDataInfo->valid = csrBtVcardAddUndetermined(
                            csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobLogo),
                            (CsrUint8 *)vcardDataInfo->paramEncodingString);
                        if (vcardDataInfo->valid)
                        {
                            vcardDataInfo->paramEncodingString = NULL;
                        }
                        else
                        {
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING;
                        }
                    }
                    else
                    {
                        vcardDataInfo->valid     = FALSE;
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_ENCODING;
                    }
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_MAILER:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->mailer),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_N:
            {
                vcardDataInfo->valid = csrBtVcardAddName(
                    vcardDataInfo, csrBtVcardAddDataType(&vcardDataInfo->vcardData->name),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_NAME:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->vcardName),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_NICKNAME:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->nickNames),
                    line,  lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_NOTE:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->note),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_ORG:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->organization),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_PHOTO:
            {
                if (vcardDataInfo->paramValue & CSR_BT_VCARD_PARAM_VALUE_URI)
                {
                    vcardDataInfo->valid = csrBtVcardAddUriString(
                        vcardDataInfo, csrBtVcardAddDataType(&vcardDataInfo->vcardData->photo),
                        line,
                        lineLength,
                        vcardDataInfo->flags);

                    if (!vcardDataInfo->valid)
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }

                }
                else if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BINARY
                         || vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BASE64)
                {
                    vcardDataInfo->valid = csrBtVcardAddBinary(
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->photo),
                        line, lineLength,
                        vcardDataInfo->paramTypeString,
                        vcardDataInfo->flags);
                    if (vcardDataInfo->valid)
                    {
                        vcardDataInfo->paramTypeString = NULL;
                    }
                    else
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING)
                    {
                        vcardDataInfo->valid = csrBtVcardAddUndetermined(
                            csrBtVcardAddDataType(&vcardDataInfo->vcardData->photo),
                            (CsrUint8 *)vcardDataInfo->paramEncodingString);
                        if (vcardDataInfo->valid)
                        {
                            vcardDataInfo->paramEncodingString = NULL;
                        }
                        else
                        {
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING;
                        }
                    }
                    else
                    {
                        vcardDataInfo->valid     = FALSE;
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_ENCODING;
                    }
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_PRODID:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo, csrBtVcardAddDataType(&vcardDataInfo->vcardData->productIdentifier),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_PROFILE:
            {
                if (!csrBtVcardStrCompare(line, (CsrUint8*) "vcard"))
                {
                    vcardDataInfo->valid = FALSE;
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_REV:
            {
                vcardDataInfo->valid = csrBtVcardAddDateTime(
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->revision),
                    line,
                    lineLength,
                    vcardDataInfo->paramValue,
                    vcardDataInfo->paramType, FALSE);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_ROLE:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo, csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobRole),
                    line, lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_SORT_STRING:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->sortString),
                    line, lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_SOUND:
            {
                if (vcardDataInfo->paramValue & CSR_BT_VCARD_PARAM_VALUE_URI)
                {
                    vcardDataInfo->valid = csrBtVcardAddUriString(
                        vcardDataInfo,
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->sound),
                        line, lineLength,
                        vcardDataInfo->flags);

                    if (!vcardDataInfo->valid)
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }

                }
                else if (vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BINARY
                         || vcardDataInfo->encoding == CSR_BT_VCARD_ENCODING_BASE64)
                {
                    vcardDataInfo->valid = csrBtVcardAddBinary(
                        csrBtVcardAddDataType(&vcardDataInfo->vcardData->sound),
                        line,
                        lineLength,
                        vcardDataInfo->paramTypeString,
                        vcardDataInfo->flags);
                    if (vcardDataInfo->valid)
                    {
                        vcardDataInfo->paramTypeString = NULL;
                    }
                    else
                    {
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                    }
                }
                else
                {
                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING)
                    {
                        vcardDataInfo->valid = csrBtVcardAddUndetermined(
                            csrBtVcardAddDataType(&vcardDataInfo->vcardData->sound),
                            (CsrUint8 *)vcardDataInfo->paramEncodingString);

                        if (vcardDataInfo->valid)
                        {
                            vcardDataInfo->paramEncodingString = NULL;
                        }
                        else
                        {
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING;
                        }
                    }
                    else
                    {
                        vcardDataInfo->valid     = FALSE;
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_ENCODING;
                    }
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_SOURCE:
            {
                /* Not supported */
                break;
            }
        case CSR_BT_VCARD_TOKEN_TEL:
            {
                vcardDataInfo->valid = csrBtVcardAddTelephone(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->tel),
                    line,
                    lineLength,
                    vcardDataInfo->paramType, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_TITLE:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->jobTitle),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_TZ:
            {
                vcardDataInfo->valid = csrBtVcardAddUtcOffsetValue(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->timezone),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_UID:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->uid),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_URL:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->url),
                    line,
                    lineLength,
                    vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_VERSION:
            {
                if (csrBtVcardStrCompare(line, (CsrUint8*) "3.0"))
                {
                    vcardDataInfo->version                  = CSR_BT_VCARD_VERSION_3_0;
                    vcardDataInfo->version_3_0_construct    = TRUE;

                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN)
                    {
                        vcardDataInfo->errorCode                |= CSR_BT_VCARD_ERROR_VERSION_FOUND;
                    }
                }
                else if (csrBtVcardStrCompare(line, (CsrUint8*) "2.1"))
                {
                    vcardDataInfo->version                  = CSR_BT_VCARD_VERSION_2_1;
                    vcardDataInfo->version_2_1_construct    = TRUE;

                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN)
                    {
                        vcardDataInfo->errorCode                |= CSR_BT_VCARD_ERROR_VERSION_FOUND;
                    }
                }
                else
                {
                    if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN)
                    {
                        if (csrBtVcardStrCompare(line, (CsrUint8*) "2.0"))
                        {
                            vcardDataInfo->version                  = CSR_BT_VCARD_VERSION_3_0;
                            vcardDataInfo->version_3_0_construct    = TRUE;
                            vcardDataInfo->errorCode                |= CSR_BT_VCARD_ERROR_VERSION_FOUND;
                        }
                        else if (csrBtVcardStrCompare(line, (CsrUint8*) "1.0"))
                        {
                            vcardDataInfo->version                  = CSR_BT_VCARD_VERSION_2_1;
                            vcardDataInfo->version_2_1_construct    = TRUE;
                            vcardDataInfo->errorCode                |= CSR_BT_VCARD_ERROR_VERSION_FOUND;
                        }
                    }
                    else
                    {
                        vcardDataInfo->valid     = FALSE;
                        vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_WRONG_VERSION;
                    }
                }
                break;
            }
        case CSR_BT_VCARD_TOKEN_X_IRMC_CALL_DATETIME:
            {
                vcardDataInfo->valid = csrBtVcardAddDateTime(
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->xIrmcCallDatetime),
                    line, lineLength, vcardDataInfo->paramValue, vcardDataInfo->paramType, TRUE);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_X_IRMC_LUID:
            {
                vcardDataInfo->valid = csrBtVcardAddString(
                    vcardDataInfo,
                    csrBtVcardAddDataType(&vcardDataInfo->vcardData->xIrmcLuid),
                    line, lineLength, vcardDataInfo->flags);

                if (!vcardDataInfo->valid)
                {
                    vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INVALID_VALUE;
                }

                break;
            }
        case CSR_BT_VCARD_TOKEN_X_NAME:
            {
                break;
            }
        default:
            {
                vcardDataInfo->valid = FALSE;
                vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_INTERNAL_ERROR;
                break;
            }
    }

    if (vcardDataInfo->state != CSR_BT_VCARD_STATE_PAYLOAD &&
        vcardDataInfo->vcardTokenInfo->token != CSR_BT_VCARD_TOKEN_BEGIN &&
        vcardDataInfo->vcardTokenInfo->token != CSR_BT_VCARD_TOKEN_END)
    {
        vcardDataInfo->state = CSR_BT_VCARD_STATE_PAYLOAD;
    }

    if (vcardDataInfo->paramEncodingString)
    {
        CsrPmemFree(vcardDataInfo->paramEncodingString);
        vcardDataInfo->paramEncodingString = NULL;
    }

    if (vcardDataInfo->paramTypeString)
    {
        CsrPmemFree(vcardDataInfo->paramTypeString);
        vcardDataInfo->paramTypeString = NULL;
    }
}

void csrBtVcardDataTypeClear(CsrBtVcardDataType* vcardDataType)
{
    CsrBtVcardDataType* tempVcardDataType;

    while (vcardDataType)
    {
        tempVcardDataType = vcardDataType->next;

        switch (vcardDataType->dataType)
        {
            case CSR_BT_VCARD_DATA_TYPE_NONE:
                {
                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_URI_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_URI_UTF8:
                {
                    CsrBtVcardDataUriString* uriString = vcardDataType->payload.uriString;

                    if (uriString)
                    {
                        CsrPmemFree(uriString->type);

                        CsrPmemFree(uriString->string);

                        CsrPmemFree(uriString);
                    }

                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8:
                {
                    CsrBtVcardDataBinary* binary = vcardDataType->payload.binary;

                    if (binary)
                    {
                        CsrPmemFree(binary->binaryData);

                        CsrPmemFree(binary->binaryType);

                        CsrPmemFree(binary);
                    }

                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8:
                {
                    CsrBtVcardDataAddress* address = vcardDataType->payload.address;

                    if (address)
                    {
                        CsrPmemFree(address->countryName);

                        CsrPmemFree(address->extendedAddress);

                        CsrPmemFree(address->locality);

                        CsrPmemFree(address->poBox);

                        CsrPmemFree(address->postalCode);

                        CsrPmemFree(address->region);

                        CsrPmemFree(address->street);

                        CsrPmemFree(address);
                    }

                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_NAME_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_NAME_UTF8:
                {
                    CsrBtVcardDataName* name = vcardDataType->payload.name;

                    if (name)
                    {
                        CsrPmemFree(name->additionalNames);

                        CsrPmemFree(name->familyName);

                        CsrPmemFree(name->givenName);

                        CsrPmemFree(name->honorificPrefixes);

                        CsrPmemFree(name->honorificSuffixes);

                        CsrPmemFree(name);
                    }
                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UTF8:
                {
                    CsrBtVcardDataTelephone* telephone = vcardDataType->payload.telephone;

                    if (telephone)
                    {
                        CsrPmemFree(telephone->number);

                        CsrPmemFree(telephone);
                    }
                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_TYPED_STRING_UTF8:
            case CSR_BT_VCARD_DATA_TYPE_TYPED_STRING_UCS2:
                {
                    CsrBtVcardDataTypedString * typedString = vcardDataType->payload.label;

                    if (typedString)
                    {
                        CsrPmemFree(typedString->string);

                        CsrPmemFree(typedString);
                    }

                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_STRING_UCS2:
            case CSR_BT_VCARD_DATA_TYPE_STRING_UTF8:
            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE:
            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME:
            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE_TYPED:
            case CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED:
                {
                    CsrPmemFree(vcardDataType->payload.data);

                    break;
                }
            case CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UTF8:
            case CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UCS2:
                {
                    CsrBtVcardDataOrganization* organization = vcardDataType->payload.organization;

                    if (organization)
                    {
                        CsrPmemFree(organization->organizationalName);

                        CsrPmemFree(organization->organizationalUnit1);

                        CsrPmemFree(organization->organizationalUnit2);

                        CsrPmemFree(organization);
                    }
                }
            default:
                {
                    break;
                }
        }

        CsrPmemFree(vcardDataType);

        vcardDataType = tempVcardDataType;
    }
}

void CsrBtVcardFree(CsrBtVcardData* vcardData)
{
    CsrBtVcardData*    tempVcardData;

    while(vcardData)
    {
        tempVcardData = vcardData->next;

        csrBtVcardDataTypeClear(vcardData->formattedName);
        csrBtVcardDataTypeClear(vcardData->vcardName);
        csrBtVcardDataTypeClear(vcardData->name);
        csrBtVcardDataTypeClear(vcardData->nickNames);
        csrBtVcardDataTypeClear(vcardData->photo);
        csrBtVcardDataTypeClear(vcardData->birthDate);
        csrBtVcardDataTypeClear(vcardData->source);
        csrBtVcardDataTypeClear(vcardData->address);
        csrBtVcardDataTypeClear(vcardData->label);
        csrBtVcardDataTypeClear(vcardData->tel);
        csrBtVcardDataTypeClear(vcardData->email);
        csrBtVcardDataTypeClear(vcardData->mailer);
        csrBtVcardDataTypeClear(vcardData->timezone);
        csrBtVcardDataTypeClear(vcardData->globalPosition);
        csrBtVcardDataTypeClear(vcardData->jobTitle);
        csrBtVcardDataTypeClear(vcardData->jobRole);
        csrBtVcardDataTypeClear(vcardData->jobLogo);
        csrBtVcardDataTypeClear(vcardData->jobAgent);
        csrBtVcardDataTypeClear(vcardData->organization);
        csrBtVcardDataTypeClear(vcardData->catagories);
        csrBtVcardDataTypeClear(vcardData->note);
        csrBtVcardDataTypeClear(vcardData->productIdentifier);
        csrBtVcardDataTypeClear(vcardData->revision);
        csrBtVcardDataTypeClear(vcardData->sortString);
        csrBtVcardDataTypeClear(vcardData->sound);
        csrBtVcardDataTypeClear(vcardData->uid);
        csrBtVcardDataTypeClear(vcardData->url);
        csrBtVcardDataTypeClear(vcardData->classification);
        csrBtVcardDataTypeClear(vcardData->key);
        csrBtVcardDataTypeClear(vcardData->xIrmcCallDatetime);
        csrBtVcardDataTypeClear(vcardData->xIrmcLuid);

        CsrPmemFree(vcardData);

        vcardData = tempVcardData;
    }
}

void CsrBtVcardParseInitialize(CsrBtVcardParserControl* control, CsrUint32 flags)
{
    CsrBtVcardDataInfo* vcardDataInfo;

    vcardDataInfo = (CsrBtVcardDataInfo *) CsrPmemAlloc(sizeof(CsrBtVcardDataInfo));
    vcardDataInfo->valid                = TRUE;
    vcardDataInfo->version              = 0;
    vcardDataInfo->state                = CSR_BT_VCARD_STATE_NONE;
    vcardDataInfo->vcardDataList        = NULL;
    vcardDataInfo->flags                = flags;
    vcardDataInfo->vcardData            = NULL;
    vcardDataInfo->errorCode            = CSR_BT_VCARD_ERROR_SUCCESS;
    vcardDataInfo->paramValue           = 0;
    vcardDataInfo->paramType            = 0;
    vcardDataInfo->paramTypeString      = NULL;
    vcardDataInfo->paramEncodingString  = NULL;
    vcardDataInfo->encoding             = CSR_BT_VCARD_ENCODING_NONE;
    vcardDataInfo->currentParam         = 0;
    vcardDataInfo->stringBufferSize     = CSR_BT_VCARD_DEFAULT_BUFFER_SIZE;
    vcardDataInfo->stringBuffer         = (CsrUint8*) CsrPmemAlloc(vcardDataInfo->stringBufferSize);
    vcardDataInfo->parseState           = 0;
    vcardDataInfo->parsePrevState       = 0;
    vcardDataInfo->stringBufferOffset   = 0;
    vcardDataInfo->parseStoredState     = 2;
    vcardDataInfo->errorCode            = CSR_BT_VCARD_ERROR_INCOMPLETE;

    control->vcardData      = NULL;
    control->flags          = flags;
    control->internalData   = (CsrUint8 *) vcardDataInfo;
}

CsrUint32 CsrBtVcardParseRead(CsrUint8 * buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control)
{
    CsrBtVcardDataInfo*  vcardDataInfo;
    CsrUint32            index;
    CsrUint32            errorCode = CSR_BT_VCARD_ERROR_INVALID_PARAMETER;

    if (control && control->internalData)
    {
        vcardDataInfo = (CsrBtVcardDataInfo*) control->internalData;

        if (!(vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN))
        {
            for (index = 0; index < bufferSize && vcardDataInfo->valid; index++)
            {
                vcardDataInfo->parsePrevState = vcardDataInfo->parseState;
                vcardDataInfo->parseState     = nextStateTable[vcardDataInfo->parseState][bufferSize == index ? 0 : characterCompressionTable[buffer[index]]];

                if (saveTable[vcardDataInfo->parseState])
                {
                    if (vcardDataInfo->stringBufferOffset < (vcardDataInfo->stringBufferSize - 1))
                    {
                        vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = buffer[index];
                    }
                    else
                    {
                        vcardDataInfo->valid = FALSE;
                    }
                }

                switch (actionTable[vcardDataInfo->parseState])
                {
                    case 0:
                        {
                            break;
                        }
                    case 1:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLinePayload(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 2:
                        {
                            index                    = bufferSize;
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
                            break;
                        }
                    case 3:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineGroup(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 4:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineName(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 5:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineParamName(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 6:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineParamValue(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 7:
                        {
                            if (vcardDataInfo->parsePrevState != 16)
                            {
                                vcardDataInfo->parseStoredState = vcardDataInfo->parsePrevState;
                            }
                            break;
                        }
                    case 8:
                        {
                            vcardDataInfo->parseState = vcardDataInfo->parseStoredState;
                            break;
                        }
                    case 9:
                        {
                            if ((vcardDataInfo->parseStoredState >= 12 && vcardDataInfo->parseStoredState <= 14) || vcardDataInfo->parseStoredState == 27)
                            {
                                vcardDataInfo->parseState = 1;
                                vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                                csrBtVcardLinePayload(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                                vcardDataInfo->stringBufferOffset = 0;
                            }
                            else
                            {
                                vcardDataInfo->parseState = 2;
                            }
                            --index;
                            break;
                        }
                    case 10:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineParamValueNoName(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 11:
                        {
                            CsrBtVcardParam currentParam = vcardDataInfo->currentParam;
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineParamValue(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            vcardDataInfo->currentParam = currentParam;
                            break;
                        }
                }
            }
        }
        else
        {
            vcardDataInfo->errorCode = CSR_BT_VCARD_ERROR_INTERNAL_ERROR;
        }

        errorCode = vcardDataInfo->errorCode;
    }

    return errorCode;
}

CsrUint32 CsrBtVcardParseReadVersionOnly(CsrUint8 * buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control)
{
    CsrBtVcardDataInfo*  vcardDataInfo;
    CsrUint32            index;
    CsrUint32            errorCode = CSR_BT_VCARD_ERROR_INVALID_PARAMETER;

    if (control && control->internalData)
    {
        vcardDataInfo = (CsrBtVcardDataInfo*) control->internalData;

        if ((vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN))
        {
            for (index = 0; index < bufferSize && vcardDataInfo->valid; index++)
            {
                vcardDataInfo->parsePrevState = vcardDataInfo->parseState;
                vcardDataInfo->parseState     = nextStateTable[vcardDataInfo->parseState][bufferSize == index ? 0 : characterCompressionTable[buffer[index]]];

                if (saveTable[vcardDataInfo->parseState])
                {
                    if (vcardDataInfo->stringBufferOffset < (vcardDataInfo->stringBufferSize - 1))
                    {
                        vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = buffer[index];
                    }
                    else
                    {
                        vcardDataInfo->valid = FALSE;
                    }
                }

                switch (actionTable[vcardDataInfo->parseState])
                {
                    case 1:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;

                            if (vcardDataInfo->vcardTokenInfo->token == CSR_BT_VCARD_TOKEN_VERSION)
                            {
                                csrBtVcardLinePayload(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            }

                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 2:
                        {
                            index                    = bufferSize;
                            vcardDataInfo->errorCode |= CSR_BT_VCARD_ERROR_SYNTAX;
                            break;
                        }
                    case 4:
                        {
                            vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;
                            csrBtVcardLineName(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                            vcardDataInfo->stringBufferOffset = 0;
                            break;
                        }
                    case 7:
                        {
                            vcardDataInfo->parseStoredState = vcardDataInfo->parsePrevState;
                            break;
                        }
                    case 8:
                        {
                            vcardDataInfo->parseState = vcardDataInfo->parseStoredState;
                            break;
                        }
                    case 9:
                        {
                            if ((vcardDataInfo->parseStoredState >= 12 && vcardDataInfo->parseStoredState <= 14) || vcardDataInfo->parseStoredState == 27)
                            {
                                vcardDataInfo->parseState = 1;
                                vcardDataInfo->stringBuffer[vcardDataInfo->stringBufferOffset++] = 0;

                                if (vcardDataInfo->vcardTokenInfo->token == CSR_BT_VCARD_TOKEN_VERSION)
                                {
                                    csrBtVcardLinePayload(vcardDataInfo, vcardDataInfo->stringBuffer, vcardDataInfo->stringBufferOffset);
                                }

                                vcardDataInfo->stringBufferOffset = 0;
                            }
                            else
                            {
                                vcardDataInfo->parseState = 2;
                            }
                            --index;
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
            }
        }
        else
        {
            vcardDataInfo->errorCode = CSR_BT_VCARD_ERROR_INTERNAL_ERROR;
        }

        errorCode = vcardDataInfo->errorCode;
    }

    return errorCode;
}

CsrUint32 CsrBtVcardParseComplete(CsrUint32 * bchsVcardDataEntries, CsrBtVcardData ** vcardData, CsrUint8* version, CsrBtVcardParserControl* control)
{
    CsrBtVcardDataInfo*  vcardDataInfo;
    CsrBtVcardData*      nextVcardData;
    CsrUint32            errorCode = CSR_BT_VCARD_ERROR_INVALID_PARAMETER;

    if (control && control->internalData)
    {
        vcardDataInfo = (CsrBtVcardDataInfo*) control->internalData;

        if (vcardDataInfo->flags & CSR_BT_VCARD_FLAGS_VERSION_SCAN)
        {
            CsrBtVcardParseReadVersionOnly((CsrUint8*) "\0", 1, control);
        }
        else
        {
            CsrBtVcardParseRead((CsrUint8*) "\0", 1, control);
        }

        if (vcardDataInfo->vcardData)
        {
            CsrBtVcardFree(vcardDataInfo->vcardData);
            vcardDataInfo->valid = FALSE;
        }

        if (!vcardDataInfo->valid)
        {
            CsrPmemFree(vcardDataInfo->paramTypeString);

            CsrPmemFree(vcardDataInfo->paramEncodingString);
        }

        if (!vcardDataInfo->valid && CSR_BT_VCARD_SUCCEEDED(vcardDataInfo->errorCode))
        {
            vcardDataInfo->errorCode = CSR_BT_VCARD_ERROR_INTERNAL_ERROR;
        }

        if (vcardData && CSR_BT_VCARD_SUCCEEDED(vcardDataInfo->errorCode))
        {
            *vcardData = vcardDataInfo->vcardDataList;
        }
        else
        {
            if (!(CSR_BT_VCARD_SUCCEEDED(vcardDataInfo->errorCode)))
            {
                CsrBtVcardFree(vcardDataInfo->vcardDataList);
                vcardDataInfo->vcardDataList = FALSE;
            }
        }

        if (bchsVcardDataEntries)
        {
            *bchsVcardDataEntries   = 0;
            nextVcardData           = *vcardData;

            while(nextVcardData)
            {
                nextVcardData = nextVcardData->next;
                (*bchsVcardDataEntries)++;
            }
        }

        if (version)
        {
            *version = vcardDataInfo->version;
        }

        CsrPmemFree(vcardDataInfo->stringBuffer);

        errorCode = vcardDataInfo->errorCode;

        CsrPmemFree(vcardDataInfo);
    }

    return errorCode;
}

CsrUint32 CsrBtVcardParse(CsrUint8* buffer, CsrUint32 bufferSize, CsrUint32* vcardDataEntries, CsrBtVcardData** vcardData, CsrUint32 flags)
{
    CsrBtVcardParserControl control;

    CsrBtVcardParseInitialize(&control, flags);

    CsrBtVcardParseRead(buffer, bufferSize, &control);

    return CsrBtVcardParseComplete(vcardDataEntries, vcardData, NULL, &control);
}

CsrBool CsrBtVcardIsDateValid(CsrBtIso8601Date * iso8601Date)
{
    return ((iso8601Date && iso8601Date->year) ? TRUE : FALSE);
}

CsrBool CsrBtVcardIsTimeValid(CsrBtIso8601Time * iso8601Time)
{
    return ((iso8601Time && iso8601Time->flags) ? TRUE : FALSE);
}
