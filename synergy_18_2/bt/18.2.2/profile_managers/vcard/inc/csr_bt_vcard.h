#ifndef CSR_BT_CSR_BT_VCARD_H__
#define CSR_BT_CSR_BT_VCARD_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_iso8601.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_VCARD_DEFAULT_BUFFER_SIZE                           8192

#define CSR_BT_VCARD_DATA_TYPE_NONE                                (0x00)
#define CSR_BT_VCARD_DATA_TYPE_STRING_UTF8                         (0x01)
#define CSR_BT_VCARD_DATA_TYPE_STRING_UCS2                         (0x02)
#define CSR_BT_VCARD_DATA_TYPE_URI_UTF8                            (0x03)
#define CSR_BT_VCARD_DATA_TYPE_URI_UCS2                            (0x04)
#define CSR_BT_VCARD_DATA_TYPE_BINARY_UTF8                         (0x05)
#define CSR_BT_VCARD_DATA_TYPE_BINARY_UCS2                         (0x06)
#define CSR_BT_VCARD_DATA_TYPE_ADDRESS_UTF8                        (0x07)
#define CSR_BT_VCARD_DATA_TYPE_ADDRESS_UCS2                        (0x08)
#define CSR_BT_VCARD_DATA_TYPE_NAME_UTF8                           (0x09)
#define CSR_BT_VCARD_DATA_TYPE_NAME_UCS2                           (0x0A)
#define CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UTF8                      (0x0B)
#define CSR_BT_VCARD_DATA_TYPE_TELEPHONE_UCS2                      (0x0C)
#define CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE                        (0x0D)
#define CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME                    (0x0E)
#define CSR_BT_VCARD_DATA_TYPE_LABEL_UTF8                          (0x0F)
#define CSR_BT_VCARD_DATA_TYPE_LABEL_UCS2                          (0x10)
#define CSR_BT_VCARD_DATA_TYPE_TYPED_STRING_UTF8                   (0x0F) /* Must share the same ID as VCARD_DATA_TYPE_LABEL_UTF8 for backward compatibility */
#define CSR_BT_VCARD_DATA_TYPE_TYPED_STRING_UCS2                   (0x10) /* Must share the same ID as VCARD_DATA_TYPE_LABEL_UCS2 for backward compatibility */
#define CSR_BT_VCARD_DATA_TYPE_UNDETERMINED                        (0x11)
#define CSR_BT_VCARD_DATA_TYPE_ISO8601_DATE_TYPED                  (0x12)
#define CSR_BT_VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED              (0x13)
#define CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UTF8                   (0x14)
#define CSR_BT_VCARD_DATA_TYPE_ORGANIZATION_UCS2                   (0x15)

#define CSR_BT_VCARD_FLAGS_DEFAULT                                 (0x0000)
#define CSR_BT_VCARD_FLAGS_UCS2_STRINGS                            (0x0001)
#define CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ATTRIBUTE_NAMES       (0x0002)
#define CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_PARAMETER_NAMES       (0x0004)
#define CSR_BT_VCARD_FLAGS_ALLOW_UNSPECIFIED_ENCODING              (0x0008)
#define CSR_BT_VCARD_FLAGS_NO_MANDATORY_TAGS                       (0x0010)
#define CSR_BT_VCARD_FLAGS_VERSION_2_1                             (0x0000)
#define CSR_BT_VCARD_FLAGS_VERSION_3_0                             (0x0020)
#define CSR_BT_VCARD_FLAGS_NO_VERSION_MATCH                        (0x0040)
#define CSR_BT_VCARD_FLAGS_VERSION_SCAN                            (0x0080)
#define CSR_BT_VCARD_FLAGS_DISABLE_2_1_TYPE_CHECK                  (0x0100)

#define CSR_BT_VCARD_ERROR_SUCCESS                                 (0x00000000)
#define CSR_BT_VCARD_ERROR_UNKNOWN_PARAMETER_NAME                  (0x00000001)
#define CSR_BT_VCARD_ERROR_UNKNOWN_ATTRIBUTE_NAME                  (0x00000002)
#define CSR_BT_VCARD_ERROR_UNKNOWN_ENCODING                        (0x00000004)
#define CSR_BT_VCARD_ERROR_INCOMPLETE                              (0x00000008)
#define CSR_BT_VCARD_ERROR_VERSION_FOUND                           (0x00000010)
#define CSR_BT_VCARD_ERROR_SYNTAX                                  (0x80000020)
#define CSR_BT_VCARD_ERROR_INTERNAL_ERROR                          (0x80000040)
#define CSR_BT_VCARD_ERROR_PARAMETER_NAME                          (0x80000080)
#define CSR_BT_VCARD_ERROR_ATTRIBUTE_NAME                          (0x80000100)
#define CSR_BT_VCARD_ERROR_INVALID_ENCODING                        (0x80000200)
#define CSR_BT_VCARD_ERROR_INVALID_VALUE                           (0x80000400)
#define CSR_BT_VCARD_ERROR_MISSING_MANDATORY_TAGS                  (0x80000800)
#define CSR_BT_VCARD_ERROR_WRONG_VERSION                           (0x80001000)
#define CSR_BT_VCARD_ERROR_MULTIPLE_VERSION_CONSTRUCT              (0x80002000)
#define CSR_BT_VCARD_ERROR_INVALID_PARAMETER                       (0x80004000)

#define CSR_BT_VCARD_SUCCEEDED(error) ((error & 0x80000000) != 0x80000000)

typedef CsrUint32 CsrBtVcardParamType;

/* Standard type information */
#define CSR_BT_VCARD_PARAM_TYPE_UNKNOWN       ((CsrUint32) 0x00000000)
#define CSR_BT_VCARD_PARAM_TYPE_HOME          ((CsrUint32) 0x00000001)
#define CSR_BT_VCARD_PARAM_TYPE_WORK          ((CsrUint32) 0x00000002)
#define CSR_BT_VCARD_PARAM_TYPE_PREF          ((CsrUint32) 0x00000004)
#define CSR_BT_VCARD_PARAM_TYPE_VOICE         ((CsrUint32) 0x00000008)
#define CSR_BT_VCARD_PARAM_TYPE_FAX           ((CsrUint32) 0x00000010)
#define CSR_BT_VCARD_PARAM_TYPE_MSG           ((CsrUint32) 0x00000020)
#define CSR_BT_VCARD_PARAM_TYPE_CELL          ((CsrUint32) 0x00000040)
#define CSR_BT_VCARD_PARAM_TYPE_PAGER         ((CsrUint32) 0x00000080)
#define CSR_BT_VCARD_PARAM_TYPE_BBS           ((CsrUint32) 0x00000100)
#define CSR_BT_VCARD_PARAM_TYPE_MODEM         ((CsrUint32) 0x00000200)
#define CSR_BT_VCARD_PARAM_TYPE_CAR           ((CsrUint32) 0x00000400)
#define CSR_BT_VCARD_PARAM_TYPE_ISDN          ((CsrUint32) 0x00000800)
#define CSR_BT_VCARD_PARAM_TYPE_VIDEO         ((CsrUint32) 0x00001000)
#define CSR_BT_VCARD_PARAM_TYPE_PCS           ((CsrUint32) 0x00002000)
#define CSR_BT_VCARD_PARAM_TYPE_INTERNET      ((CsrUint32) 0x00004000)
#define CSR_BT_VCARD_PARAM_TYPE_X400          ((CsrUint32) 0x00008000)
#define CSR_BT_VCARD_PARAM_TYPE_X509          ((CsrUint32) 0x00010000)
#define CSR_BT_VCARD_PARAM_TYPE_PGP           ((CsrUint32) 0x00020000)
#define CSR_BT_VCARD_PARAM_TYPE_DOM           ((CsrUint32) 0x00040000)
#define CSR_BT_VCARD_PARAM_TYPE_INTL          ((CsrUint32) 0x00080000)
#define CSR_BT_VCARD_PARAM_TYPE_POSTAL        ((CsrUint32) 0x00100000)
#define CSR_BT_VCARD_PARAM_TYPE_PARCEL        ((CsrUint32) 0x00200000)
#define CSR_BT_VCARD_PARAM_TYPE_X_NAME        ((CsrUint32) 0x00400000)
#define CSR_BT_VCARD_PARAM_TYPE_STR           ((CsrUint32) 0x00800000)

/* Bluetooth PBAP extension */
#define CSR_BT_VCARD_PARAM_TYPE_MISSED        ((CsrUint32) 0x01000000)
#define CSR_BT_VCARD_PARAM_TYPE_RECEIVED      ((CsrUint32) 0x02000000)
#define CSR_BT_VCARD_PARAM_TYPE_DIALED        ((CsrUint32) 0x04000000)

/* Instant messaging extension */
#define CSR_BT_VCARD_PARAM_TYPE_PERSONAL      ((CsrUint32) 0x08000000)
#define CSR_BT_VCARD_PARAM_TYPE_BUSINESS      ((CsrUint32) 0x10000000)
#define CSR_BT_VCARD_PARAM_TYPE_MOBILE        ((CsrUint32) 0x20000000)

/* Version of the parsed vCard */
#define CSR_BT_VCARD_VERSION_UNKNOWN          ((CsrUint8) 0)
#define CSR_BT_VCARD_VERSION_2_1              ((CsrUint8) 1)
#define CSR_BT_VCARD_VERSION_3_0              ((CsrUint8) 2)
#define CSR_BT_VCALENDAR_VERSION_1_0          ((CsrUint8) 3)
#define CSR_BT_VCALENDAR_VERSION_2_0          ((CsrUint8) 4)

typedef struct
{
    CsrCharString *                              type;
    CsrCharString *                              string;
} CsrBtVcardDataUriString;

typedef struct
{
    CsrBtVcardParamType                      addressType;
    CsrCharString *                              poBox;
    CsrCharString *                              extendedAddress;
    CsrCharString *                              street;
    CsrCharString *                              locality;
    CsrCharString *                              region;
    CsrCharString *                              postalCode;
    CsrCharString *                              countryName;
} CsrBtVcardDataAddress;

typedef struct
{
    CsrBtVcardParamType                      type;
    CsrCharString *                              string;
} CsrBtVcardDataLabel, CsrBtVcardDataTypedString;

typedef struct
{
    CsrCharString *                              binaryType;
    CsrUint32                                binarySize;
    CsrUint8 *                               binaryData;
} CsrBtVcardDataBinary;

typedef struct
{
    CsrCharString *                              familyName;
    CsrCharString *                              givenName;
    CsrCharString *                              additionalNames;
    CsrCharString *                              honorificPrefixes;
    CsrCharString *                              honorificSuffixes;
} CsrBtVcardDataName;

typedef struct
{
    CsrBtVcardParamType                      type;
    CsrCharString *                              number;
} CsrBtVcardDataTelephone;

typedef struct
{
    CsrBtVcardParamType                      type;
    CsrBtIso8601Date                         iso8601Date;
} CsrBtVcardDataIso8601Date;

typedef struct
{
    CsrBtVcardParamType                      type;
    CsrBtIso8601DateTime                     iso8601DateTime;
} CsrBtVcardDataIso8601DateTime;

typedef struct
{
    CsrCharString *                              organizationalName;
    CsrCharString *                              organizationalUnit1;
    CsrCharString *                              organizationalUnit2;
} CsrBtVcardDataOrganization;

typedef struct CsrBtVcardDataTypeRef
{
    struct CsrBtVcardDataTypeRef *           next;
    CsrUint8                                 dataType;               /* Datatype for each possible entry is specified below */
    union
    {
        CsrCharString *                          string;                 /* VCARD_DATA_TYPE_STRING_UTF8 or VCARD_DATA_TYPE_STRING_UCS2 */
        CsrBtVcardDataUriString *            uriString;              /* VCARD_DATA_TYPE_URI_UTF8 or VCARD_DATA_TYPE_URI_UCS2 */
        CsrBtVcardDataBinary *               binary;                 /* VCARD_DATA_TYPE_BINARY_UTF8 or VCARD_DATA_TYPE_BINARY_UCS2 */
        CsrBtVcardDataAddress *              address;                /* VCARD_DATA_TYPE_ADDRESS_UTF8 or VCARD_DATA_TYPE_ADDRESS_UCS2 */
        CsrBtVcardDataName *                 name;                   /* VCARD_DATA_TYPE_NAME_UTF8 or VCARD_DATA_TYPE_NAME_UCS2 */
        CsrBtVcardDataTelephone *            telephone;              /* VCARD_DATA_TYPE_TELEPHONE_UTF8 or VCARD_DATA_TYPE_TELEPHONE_UCS2 */
        CsrBtIso8601Date *                   iso8601Date;            /* VCARD_DATA_TYPE_ISO8601_DATE */
        CsrBtIso8601DateTime *               iso8601DateTime;        /* VCARD_DATA_TYPE_ISO8601_DATETIME */
        CsrBtVcardDataLabel *                label;                  /* VCARD_DATA_TYPE_LABEL_UTF8 or VCARD_DATA_TYPE_LABEL_UCS2 */
        CsrBtVcardDataTypedString *          typedString;            /* VCARD_DATA_TYPE_TYPED_STRING_UTF8 or VCARD_DATA_TYPE_TYPED_STRING_UCS2 */
        CsrBtVcardDataIso8601Date *          iso8601DateTyped;       /* VCARD_DATA_TYPE_ISO8601_DATE_TYPED */
        CsrBtVcardDataIso8601DateTime *      iso8601DateTimeTyped;   /* VCARD_DATA_TYPE_ISO8601_DATETIME_TYPED */
        CsrBtVcardDataOrganization *         organization;           /* VCARD_DATA_TYPE_ORGANIZATION_UTF8 or VCARD_DATA_TYPE_ORGANIZATION_UCS2 */
        void *                              data;                   /* Entry used to optimize free functionality */
    } payload;
} CsrBtVcardDataType;

typedef struct CsrBtVcardDataRef
{
    struct CsrBtVcardDataRef *               next;

    /* Standard vCard parameters */
    CsrBtVcardDataType *                     formattedName;
    CsrBtVcardDataType *                     vcardName;
    CsrBtVcardDataType *                     name;
    CsrBtVcardDataType *                     nickNames;
    CsrBtVcardDataType *                     photo;
    CsrBtVcardDataType *                     birthDate;
    CsrBtVcardDataType *                     source;
    CsrBtVcardDataType *                     address;
    CsrBtVcardDataType *                     label;
    CsrBtVcardDataType *                     tel;
    CsrBtVcardDataType *                     email;
    CsrBtVcardDataType *                     mailer;
    CsrBtVcardDataType *                     timezone;
    CsrBtVcardDataType *                     globalPosition;
    CsrBtVcardDataType *                     jobTitle;
    CsrBtVcardDataType *                     jobRole;
    CsrBtVcardDataType *                     jobLogo;
    CsrBtVcardDataType *                     jobAgent;
    CsrBtVcardDataType *                     organization;
    CsrBtVcardDataType *                     catagories;
    CsrBtVcardDataType *                     note;
    CsrBtVcardDataType *                     productIdentifier;
    CsrBtVcardDataType *                     revision;
    CsrBtVcardDataType *                     sortString;
    CsrBtVcardDataType *                     sound;
    CsrBtVcardDataType *                     uid;
    CsrBtVcardDataType *                     url;
    CsrBtVcardDataType *                     classification;
    CsrBtVcardDataType *                     key;

    /* Instant messaging parameters */
    CsrBtVcardDataType *                     impp;

    /* IRDA vCard parameters */
    CsrBtVcardDataType *                     xIrmcCallDatetime;
    CsrBtVcardDataType *                     xIrmcLuid;
} CsrBtVcardData;

typedef struct
{
    CsrBtVcardData *                         vcardData;
    CsrBtVcardDataType *                     currentVcardDataType;
    CsrUint32                                flags;
    CsrUint32                                currentLineLength;
    CsrUint32                                type;
    CsrUint32                                mask;
    CsrUint32                                binaryOffset;
    CsrUint8 *                               currentString;
    CsrUint8 *                               helperString;
    CsrUint8 *                               decodingString;
    CsrUint8 *                               foldingString;
    CsrUint8                                 generatorEntry;
    CsrUint8                                 entryState;
    CsrUint8                                 structState;
    CsrUint8                                 extendedType;
    CsrBool                                  performEncoding;
    CsrBool                                  stringIsUCS2;
    CsrBool                                  expandLine;
    CsrCharString                                tempBuffer[30];
    CsrUint8                                 encodeBuffer[4];
} CsrBtVcardGeneratorControl;

typedef struct
{
    CsrBtVcardData *                         vcardData;
    CsrUint32                                flags;
    CsrUint8*                                internalData;
} CsrBtVcardParserControl;

void     CsrBtVcardFree(CsrBtVcardData *csrBtVcardData); /* Only for use with data returned from the parser */

void     CsrBtVcardParseInitialize(CsrBtVcardParserControl* control, CsrUint32 flags);
CsrUint32 CsrBtVcardParseRead(CsrUint8 * buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control);
CsrUint32 CsrBtVcardParseReadVersionOnly(CsrUint8 * buffer, CsrUint32 bufferSize, CsrBtVcardParserControl* control);
CsrUint32 CsrBtVcardParseComplete(CsrUint32 * csrBtVcardDataEntries, CsrBtVcardData ** vcardData, CsrUint8* version, CsrBtVcardParserControl* control);
CsrUint32 CsrBtVcardParse(CsrUint8 * buffer, CsrUint32 bufferSize, CsrUint32 * csrBtVcardDataEntries, CsrBtVcardData ** csrBtVcardData, CsrUint32 flags);

void     CsrBtVcardGeneratorInitialize(CsrBtVcardGeneratorControl * control, CsrUint32 flags);
CsrUint32 CsrBtVcardGeneratorWrite(CsrBtVcardData * csrBtVcardData, CsrUint8 * buffer, CsrUint32 bufferSize, CsrBtVcardGeneratorControl * control);

CsrBool   CsrBtVcardIsDateValid(CsrBtIso8601Date * iso8601Date);
CsrBool   CsrBtVcardIsTimeValid(CsrBtIso8601Time * iso8601Time);

#ifdef __cplusplus
}
#endif

#endif
