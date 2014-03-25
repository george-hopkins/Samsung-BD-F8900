#ifndef CSR_BT_CSR_BT_VCARD_PRIVATE_H__
#define CSR_BT_CSR_BT_VCARD_PRIVATE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_VCARD_GEN_ENTRY_START                           (0x00)
#define CSR_BT_VCARD_GEN_ENTRY_VERSION                         (0x01)
#define CSR_BT_VCARD_GEN_ENTRY_FORMATTEDNAME                   (0x02)
#define CSR_BT_VCARD_GEN_ENTRY_VCARDNAME                       (0x03)
#define CSR_BT_VCARD_GEN_ENTRY_NAME                            (0x04)
#define CSR_BT_VCARD_GEN_ENTRY_NICKNAMES                       (0x05)
#define CSR_BT_VCARD_GEN_ENTRY_PHOTO                           (0x06)
#define CSR_BT_VCARD_GEN_ENTRY_BIRTHDATE                       (0x07)
#define CSR_BT_VCARD_GEN_ENTRY_SOURCE                          (0x08)
#define CSR_BT_VCARD_GEN_ENTRY_ADDRESS                         (0x09)
#define CSR_BT_VCARD_GEN_ENTRY_LABEL                           (0x0A)
#define CSR_BT_VCARD_GEN_ENTRY_TEL                             (0x0B)
#define CSR_BT_VCARD_GEN_ENTRY_EMAIL                           (0x0C)
#define CSR_BT_VCARD_GEN_ENTRY_MAILER                          (0x0D)
#define CSR_BT_VCARD_GEN_ENTRY_TIMEZONE                        (0x0E)
#define CSR_BT_VCARD_GEN_ENTRY_GLOBALPOSITION                  (0x0F)
#define CSR_BT_VCARD_GEN_ENTRY_JOBTITLE                        (0x10)
#define CSR_BT_VCARD_GEN_ENTRY_JOBROLE                         (0x11)
#define CSR_BT_VCARD_GEN_ENTRY_JOBLOGO                         (0x12)
#define CSR_BT_VCARD_GEN_ENTRY_JOBAGENT                        (0x13)
#define CSR_BT_VCARD_GEN_ENTRY_ORGANIZATION                    (0x14)
#define CSR_BT_VCARD_GEN_ENTRY_CATAGORIES                      (0x15)
#define CSR_BT_VCARD_GEN_ENTRY_NOTE                            (0x16)
#define CSR_BT_VCARD_GEN_ENTRY_PRODUCTIDENTIFIER               (0x17)
#define CSR_BT_VCARD_GEN_ENTRY_REVISION                        (0x18)
#define CSR_BT_VCARD_GEN_ENTRY_SORTSTRING                      (0x19)
#define CSR_BT_VCARD_GEN_ENTRY_SOUND                           (0x1A)
#define CSR_BT_VCARD_GEN_ENTRY_UID                             (0x1B)
#define CSR_BT_VCARD_GEN_ENTRY_URL                             (0x1C)
#define CSR_BT_VCARD_GEN_ENTRY_CLASSIFICATION                  (0x1D)
#define CSR_BT_VCARD_GEN_ENTRY_KEY                             (0x1E)
#define CSR_BT_VCARD_GEN_ENTRY_XIRMCCALLDATETIME               (0x1F)
#define CSR_BT_VCARD_GEN_ENTRY_XIRMCLUID                       (0x20)
#define CSR_BT_VCARD_GEN_ENTRY_END                             (0x21)
#define CSR_BT_VCARD_GEN_ENTRY_COMPLETE                        (0x22)

#define CSR_BT_VCARD_GEN_ENTRY_STATE_NAME                      (0x00)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_PARAMETER                 (0x01)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_PARAMETER_EXTENDED        (0x02)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_BINARY_OR_URI_TYPE_NAME   (0x03)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_BINARY_OR_URI_TYPE_VALUE  (0x04)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE_START               (0x05)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_VALUE                     (0x06)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_EOL                       (0x07)
#define CSR_BT_VCARD_GEN_ENTRY_STATE_COMPLETE                  (0x08)

#define CSR_BT_VCARD_GEN_ADR_STATE_POBOX                       (0x00)
#define CSR_BT_VCARD_GEN_ADR_STATE_POBOX_END                   (0x01)
#define CSR_BT_VCARD_GEN_ADR_STATE_EXADR                       (0x02)
#define CSR_BT_VCARD_GEN_ADR_STATE_EXADR_END                   (0x03)
#define CSR_BT_VCARD_GEN_ADR_STATE_STREET                      (0x04)
#define CSR_BT_VCARD_GEN_ADR_STATE_STREET_END                  (0x05)
#define CSR_BT_VCARD_GEN_ADR_STATE_LOCAL                       (0x06)
#define CSR_BT_VCARD_GEN_ADR_STATE_LOCAL_END                   (0x07)
#define CSR_BT_VCARD_GEN_ADR_STATE_REGION                      (0x08)
#define CSR_BT_VCARD_GEN_ADR_STATE_REGION_END                  (0x09)
#define CSR_BT_VCARD_GEN_ADR_STATE_POSTAL                      (0x0A)
#define CSR_BT_VCARD_GEN_ADR_STATE_POSTAL_END                  (0x0B)
#define CSR_BT_VCARD_GEN_ADR_STATE_COUNTRY                     (0x0C)
#define CSR_BT_VCARD_GEN_ADR_STATE_COMPLETE                    (0x0D)

#define CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME                (0x00)
#define CSR_BT_VCARD_GEN_NAME_STATE_FAMILY_NAME_END            (0x01)
#define CSR_BT_VCARD_GEN_NAME_STATE_GIVEN_NAME                 (0x02)
#define CSR_BT_VCARD_GEN_NAME_STATE_GIVEN_NAME_END             (0x03)
#define CSR_BT_VCARD_GEN_NAME_STATE_ADDITIONAL_NAMES           (0x04)
#define CSR_BT_VCARD_GEN_NAME_STATE_ADDITIONAL_NAMES_END       (0x05)
#define CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_PREFIXES         (0x06)
#define CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_PREFIXES_END     (0x07)
#define CSR_BT_VCARD_GEN_NAME_STATE_HONORIFIC_SUFFIXES         (0x08)
#define CSR_BT_VCARD_GEN_NAME_STATE_COMPLETE                   (0x09)

#define CSR_BT_VCARD_GEN_ORG_STATE_NAME                        (0x00)
#define CSR_BT_VCARD_GEN_ORG_STATE_NAME_END                    (0x01)
#define CSR_BT_VCARD_GEN_ORG_STATE_UNIT1                       (0x02)
#define CSR_BT_VCARD_GEN_ORG_STATE_UNIT1_END                   (0x03)
#define CSR_BT_VCARD_GEN_ORG_STATE_UNIT2                       (0x04)
#define CSR_BT_VCARD_GEN_ORG_STATE_COMPLETE                    (0x05)

#define CSR_BT_VCARD_PARAM_BINARY_ENCODING                     ((CsrUint32) 0x00000001)
#define CSR_BT_VCARD_PARAM_QUOTED_PRINTABLE                    ((CsrUint32) 0x00000002)
#define CSR_BT_VCARD_PARAM_CHARSET                             ((CsrUint32) 0x00000004)

#define CSR_BT_VCARD_GEN_NUMBER_OF_CHAR_ON_LINE                (75)

#ifdef __cplusplus
}
#endif

#endif
