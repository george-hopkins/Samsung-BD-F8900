#ifndef _SDP_STAT_PRIVATE_H
#define _SDP_STAT_PRIVATE_H

#include "csr_synergy.h"
/****************************************************************************

                   (c) CSR plc 2010

                   All rights reserved

FILE:              sdp_sds.c

DESCRIPTION:       This file contains SDP SDS dbase bc01 private declarations.



****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* UUID Protocols */
#define UUID_UDP                      0x0002
#define UUID_RFCOMM                   0x0003
#define UUID_TCP                      0x0004
#define UUID_TCS_BIN                  0x0005
#define UUID_TCS_AT                   0x0006
#define UUID_OBEX                     0x0008
#define UUID_IP                       0x0009
#define UUID_FTP                      0x000A
#define UUID_HTTP                     0x000C
#define UUID_WSP                      0x000E
#define UUID_L2CAP                    0x0100

/* UUID Service Classes */
#define UUID_BROWSE_GROUP_CLASS_ID    0x1001
#define UUID_PUBLIC_BROWSE_GROUP      0x1002
#define UUID_SERVICE_SERIAL           0x1101
#define UUID_LAN_ACCESS_PPP           0x1102
#define UUID_DIAL_UP_NETWORK          0x1103
#define UUID_IR_MC_SYNC               0x1104
#define UUID_OBEX_OBJECT_PUSH         0x1105
#define UUID_OBEX_FILE_TRANS          0x1106
#define UUID_IR_MC_SYNC_COM           0x1107
#define UUID_SERVICE_HEADSET          0x1108
#define UUID_CORDLESS_TEL             0x1109
#define UUID_INTERCOM                 0x1110
#define UUID_SERVICE_FAX              0x1111
#define UUID_HEADSET_AUDIO_GATE       0x1112
#define UUID_PNP_INFORMATION          0x1200
#define UUID_GENERIC_NETWORKING       0x1201
#define UUID_GENERIC_FILE_TRANSFER    0x1202
#define UUID_GENERIC_AUDIO            0x1203
#define UUID_GENERIC_TELEPHONY        0x1204

/* Attribute Identifiers */
#define SERVICE_CLASS_ID_LIST         0x0001
#define SERVICE_ID                    0x0003
#define PROTOCOL_DESCRIPTOR_LIST      0x0004
#define BROWSE_GROUP_LIST             0x0005
#define LANG_BASE_ATTR_ID_LIST        0x0006
#define SERVICE_INFO_TIME_TO_LIVE     0x0007
#define SERVIVE_AVAILABILITY          0x0008
#define BT_PROFILE_DESCRIPTOR_LIST    0x0009
#define DOCUMENT_URL                  0x000A
#define CLIENT_EXE_URL                0x000B
#define ICON_URL                      0x000D
#define ICON10                        0x000C
#define SERVICE_NAME                  0x0100
#define SERVICE_DESCRIPTION           0x0101
#define PROVIDER_NAME                 0x0102
#define GROUP_ID                      0x0200
#define SERVICE_VERSION               0x0300
#define EXTERNAL_NETWORK              0x0301
#define REMOTE_AUDIO_VOL              0x0302

/*
    Standard Data Representations
*/
/* Standard 16 Bit Number Representation */
#define BIT_16_NUMBER(number) \
        (CsrUint8)((number) >> 8), (CsrUint8)((number) & 0xFF)
/* Standard 32 Bit Number Representation */
#define BIT_32_NUMBER(number) \
        (CsrUint8)((number) >> 24), (CsrUint8)((number) >> 16), \
        (CsrUint8)((number) >> 8), (CsrUint8)((number) & 0xFF)

/*
    Data Representation Header Fields
*/

/*
    NULL type 0
*/
/* NULL Byte Header */
#define NULL_HEADER_0BYTE \
        0x00
/*
    Data Representation Header, Unsigned Integer Type 1
*/
/* 1 Byte Unsigned Integer Header */
#define UNSIGNED_HEADER_1BYTE \
        0x08
/* 2 Byte Unsigned Integer Header */
#define UNSIGNED_HEADER_2BYTE \
        0x09
/* 4 Byte Unsigned Integer Header */
#define UNSIGNED_HEADER_4BYTE \
        0x0A
/* 6 Byte Unsigned Integer Header */
#define UNSIGNED_HEADER_8BYTE \
        0x0B
/* 16 Byte Unsigned Integer Header */
#define UNSIGNED_HEADER_16BYTE \
        0x0C

/*
    Data Representation Header, Signed Twos-Complement Integer Headers Type 2
*/
/* 1 Byte Signed Twos-Complement Integer Header */
#define SIGNED_TC_HEADER_1BYTE \
        0x10
/* 2 Byte Signed Twos-Complement Integer Header */
#define SIGNED_TC_HEADER_2BYTE \
        0x11
/* 4 Byte Signed Twos-Complement Integer Header */
#define SIGNED_TC_HEADER_4BYTE \
        0x12
/* 8 Byte Signed Twos-Complement Integer Header */
#define SIGNED_TC_HEADER_8BYTE \
        0x13
/* 16 Byte Signed Twos-Complement Integer Header */
#define SIGNED_TC_HEADER_16BYTE \
        0x14

/*
    Data Representation Header, UUID, universally unique identifier headers
    Type 3
*/
/* 16 Bit UUID Header */
#define UUID_SERVICE_HEADER_16BIT \
        0x19
/* 32 Bit UUID Header */
#define UUID_SERVICE_HEADER_32BIT \
        0x1A

/*
    Data Representation Header, Test String Headers Type 4
*/
/* 8 Bit Text String Header */
#define TEXT_STRING_HEADER_8BIT(size)\
        0x25, (CsrUint8)(size)
/* 16 Bit Text String Header */
#define TEXT_STRING_HEADER_16BIT(size)\
        0x26, BIT_16_NUMBER(size)
/* 32 Bit Text String Header */
#define TEXT_STRING_HEADER_32BIT(size)\
        0x27, BIT_32_NUMBER(size)

/*
    Data Representation Header, Boolean Header Type 5
*/
/* Boolean Header */
#define BOOLEAN_HEADER \
        0x28

/*
    Data Representation Header, Data Element Sequence Headers Type 6
*/
/* 8 Bit Data Element Sequence Header */
#define DE_SEQ_HEADER_8BIT(size) \
        0x35, (CsrUint8)(size)
/* 16 Bit Data Element Sequence Header */
#define DE_SEQ_HEADER_16BIT(size) \
        0x36, BIT_16_NUMBER(size)
/* 32 Bit Data Element Sequence Header */
#define DE_SEQ_HEADER_32BIT(size) \
        0x37, BIT_32_NUMBER(size)

/*
    Data Representation Header, Data Element Alternative Headers Type 7
*/
/* 8 Bit Data Element Alternative Header */
#define DE_ALT_HEADER_8BIT(size) \
        0x3D, (CsrUint8)(size)
/* 16 Bit Data Element Alternative Header */
#define DE_ALT_HEADER_16BIT(size) \
        0x3E, BIT_16_NUMBER(size)
/* 32 Bit Data Element Alternative Header */
#define DE_ALT_HEADER_32BIT(size) \
        0x3F, BIT_32_NUMBER(size)

/*
    Data Representation Header, URL Headers Type 8
*/
/* 8 Bit URL Header */
#define URL_HEADER_8BIT(size) \
        0x3D, (CsrUint8)(size)
/* 16 Bit URL Header */
#define URL_HEADER_16BIT(size) \
        0x3E, BIT_16_NUMBER(size)
/* 32 Bit URL Header */
#define URL_HEADER_32BIT(size) \
        0x3F, BIT_32_NUMBER(size)

/*
    Attribute, UUID and Data Simplification
*/
/* Attribute ID Header Format Type */
#define ATTRIBUTE_ID_HEADER     UNSIGNED_HEADER_2BYTE
/* Attribute ID 16 Bit Data Format Type */
#define ATTRIBUTE_ID_DATA       BIT_16_NUMBER
/* UUID 16 Bit Data Format Type */
#define UUID_16_BIT_DATA        BIT_16_NUMBER
/* UUID 32 Bit Data FORMAT Type */
#define UUID_32_BIT_DATA        BIT_32_NUMBER
/* Service Record Size Type */
#define SERVICE_REC_SIZE_DATA   BIT_16_NUMBER
/* 16 Bit Data Format Type */
#define UINT_16_DATA            BIT_16_NUMBER


/*============================================================================*
Private Data Types
*============================================================================*/
/*
    The Service Records must be specified correctly in bytes.

    The first 2 bytes of the service record must contain the size of the
    following data.

    Then the service record data must follow. The service record must be
    correctly encased in the correct data element sequences.  The server will
    add the service record handle during a service search.

e.g.
    Service Record 1 for OBBEX object push

    size

    SERVICE_REC_SIZE_DATA(0x0056),

    Attribute ID 0001
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(SERVICE_CLASS_ID_LIST),
            DE_SEQ_HEADER_8BIT(0x03),
                UUID_SERVICE_HEADER_16BIT,
                    UUID_16_BIT_DATA(UUID_OBEX_OBJECT_PUSH),

    Attribute ID 0004
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(PROTOCOL_DESCRIPTOR_LIST),
            DE_SEQ_HEADER_8BIT(0x11),
               DE_SEQ_HEADER_8BIT(0x03),
                   UUID_SERVICE_HEADER_16BIT,
                       UUID_16_BIT_DATA(UUID_L2CAP),
               DE_SEQ_HEADER_8BIT(0x05),
                   UUID_SERVICE_HEADER_16BIT,
                       UUID_16_BIT_DATA(UUID_RFCOMM),
                   UNSIGNED_HEADER_1BYTE,
                       0x03,
               DE_SEQ_HEADER_8BIT(0x03),
                   UUID_SERVICE_HEADER_16BIT,
                       UUID_16_BIT_DATA(UUID_OBEX),

    Attribute ID 0005
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(BROWSE_GROUP_LIST),
            DE_SEQ_HEADER_8BIT(0x03),
                UUID_SERVICE_HEADER_16BIT,
                    UUID_16_BIT_DATA(UUID_PUBLIC_BROWSE_GROUP),

    Attribute ID 0006
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(LANG_BASE_ATTR_ID_LIST),
            DE_SEQ_HEADER_8BIT(0x09),
                UNSIGNED_HEADER_2BYTE,
                    UINT_16_DATA(0x656E),
                UNSIGNED_HEADER_2BYTE,
                    UINT_16_DATA(0x006A),
                UNSIGNED_HEADER_2BYTE,
                    UINT_16_DATA(0x0100),

    Attribute ID 0009
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(BT_PROFILE_DESCRIPTOR_LIST),
            DE_SEQ_HEADER_8BIT(0x08),
                DE_SEQ_HEADER_8BIT(0x06),
                    UUID_SERVICE_HEADER_16BIT,
                        UUID_16_BIT_DATA(UUID_OBEX_OBJECT_PUSH),
                    UNSIGNED_HEADER_2BYTE,
                        UINT_16_DATA(0x0100),

    Attribute ID 0100
    ATTRIBUTE_ID_HEADER,
        ATTRIBUTE_ID_DATA(SERVICE_NAME),
            TEXT_STRING_HEADER_8BIT(0x10),
                0x4F, 0x42, 0x45, 0x58, 0x2C, 0x4F, 0x62, 0x6A, 0x65, 0x63,
                0x74, 0x2C, 0x50, 0x75, 0x73, 0x68

*/

#ifdef __cplusplus
}
#endif
#endif /* ndef _SDP_STAT_PRIVATE_H */
/*============================================================================*
End Of File
*============================================================================*/
