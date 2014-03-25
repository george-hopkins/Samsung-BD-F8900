#ifndef CSR_BT_OBEX_COMMON_H__
#define CSR_BT_OBEX_COMMON_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_obex.h"
#include "csr_types.h"
#include "csr_message_queue.h"
#include "csr_bt_profiles.h"

#ifdef __cplusplus
extern "C" {
#endif

/* General */
#define CSR_BT_OBJECT_TRANSFER_COD                     0x00100000
#define CSR_BT_OBEX_OBJECT_PUSH_SERVICE_CLASS          0x1105
#define CSR_BT_OBEX_SYNC_SERVER_SERVICE_CLASS          0x1104
#define CSR_BT_OBEX_FILE_TRANSFER_SERVICE_CLASS        0x1106
#define CSR_BT_UNASSIGNED_SERVER_CHANNEL               ((CsrUint8) 0xFF)
#define CSR_BT_MIN_OBEX_CONNECT_LENGTH                 7
#define CSR_BT_OBEX_VERSION_1_0                        0x10
#define CSR_BT_OBEX_CONNECTION_FLAGS                   0x00
#define CSR_BT_OBEX_HEADER_LENGTH                      (3)
#define CSR_BT_OBEX_ABORT_DELAY                        (1000000)
#define CSR_BT_MIN_OBEX_PACKET_LENGTH                  255

/* Invalid values */
#define CSR_BT_UNUSED_CONNECTION_ID                    0xFFFFFFFF
#define CSR_BT_UNUSED_LENGTH                           0x0000
#define CSR_BT_UNUSED_BODY                             NULL
#define CSR_BT_UNUSED_FINALFLAG                        TRUE
#define CSR_BT_UNUSED_BODYTYPE                         0x00
#define CSR_BT_UNUSED_NAME                             NULL
#define CSR_BT_UNUSED_AUTHENTICATE                     0x0000
#define CSR_BT_UNUSED_APPPARAMETER                     NULL
#define CSR_BT_UNUSED_DIGESTRESPONSE                   NULL

/* Header IDs */
#define CSR_BT_OBEX_HI_MASK                           0xC0
#define CSR_BT_OBEX_HI_UNICODE                        0x00
#define CSR_BT_OBEX_HI_BYTESEQ                        0x40
#define CSR_BT_OBEX_HI_ONEBYTE                        0x80
#define CSR_BT_OBEX_HI_FOURBYTE                       0xC0

/* Mandatory Response size */
#define CSR_BT_OBEX_CONNECTION_HEADER_SIZE            (7)
#define CSR_BT_OBEX_RESPONSE_HEADER_SIZE              (3)
#define CSR_BT_OBEX_LENGTH_HEADER_SIZE                (5)
#define CSR_BT_OBEX_BODY_HEADER_SIZE                  (3)
#define CSR_BT_OBEX_ENDOFBODY_HEADER_SIZE             (3)
#define CSR_BT_OBEX_APP_PAR_HEADER_SIZE               (3)
#define CSR_BT_OBEX_APP_PAR_JOB_ID_SIZE               (6)
#define CSR_BT_OBEX_CONNECTION_ID_HEADER_SIZE         (5)
#define CSR_BT_OBEX_TYPE_HEADER_SIZE                  (3)
#define CSR_BT_OBEX_NAME_HEADER_SIZE                  (3)
#define CSR_BT_OBEX_APP_PAR_HEADER_SIZE               (3)
#define CSR_BT_OBEX_APP_PAR_PB_SIZE                   (4)
#define CSR_BT_OBEX_APP_PAR_NEWMISSEDCALL_SIZE        (3)
#define CSR_BT_OBEX_APP_PAR_FILTER_SIZE               (10)
#define CSR_BT_OBEX_APP_PAR_FORMAT_SIZE               (3)
#define CSR_BT_OBEX_APP_PAR_MAXLISTCNT_SIZE           (4)
#define CSR_BT_OBEX_APP_PAR_ORDER_SIZE                (3)
#define CSR_BT_OBEX_APP_PAR_LISTSTARTOFFSET_SIZE      (4)
#define CSR_BT_OBEX_APP_PAR_COPY_SIZE                 (3)
#define CSR_BT_OBEX_APP_PAR_RETRY_SIZE                (3)
#define CSR_BT_OBEX_APP_PAR_SEARCH_ATT                (3)

/* Headers */
#define CSR_BT_OBEX_NAME_HEADER                       0x01
#define CSR_BT_OBEX_DESCRIPTION_HEADER                0x05
#define CSR_BT_OBEX_TYPE_HEADER                       0x42
#define CSR_BT_OBEX_BODY_HEADER                       0x48
#define CSR_BT_OBEX_END_OF_BODY_HEADER                0x49
#define CSR_BT_OBEX_TARGET_HEADER                     0x46
#define CSR_BT_OBEX_WHO_HEADER                        0x4A
#define CSR_BT_OBEX_LENGTH_HEADER                     0xC3
#define CSR_BT_OBEX_COUNT_HEADER                      0xC0
#define CSR_BT_OBEX_CONNECTION_ID_HEADER              0xCB
#define CSR_BT_OBEX_AUTHENTICATE_CHALLENGE_HEADER     0x4D
#define CSR_BT_OBEX_AUTHENTICATE_RESPONSE_HEADER      0x4E
#define CSR_BT_OBEX_APPLICATION_PARAMETERS_HEADER     0x4C
#define CSR_BT_OBEX_IMG_HANDLE_HEADER                 0x30
#define CSR_BT_OBEX_IMG_DESCRIPTOR_HEADER             0x71
#define CSR_BT_OBEX_SINGLE_RESPONSE_MODE_HEADER       0x97
#define CSR_BT_OBEX_SRMP_HEADER                       0x98
#define CSR_BT_OBEX_ACTION_ID_HEADER                  0x94
#define CSR_BT_OBEX_DESTNAME_HEADER                   0x15
#define CSR_BT_OBEX_PERMISSIONS_HEADER                0xD6

/* Requests */
#define CSR_BT_OBEX_FINAL_BIT_MASK                    0x80
#define CSR_BT_OBEX_CONNECT_OPCODE                    0x80
#define CSR_BT_OBEX_DISCONNECT_OPCODE                 0x81
#define CSR_BT_OBEX_PUT_OPCODE                        0x02
#define CSR_BT_OBEX_GET_OPCODE                        0x03
#define CSR_BT_OBEX_SET_PATH_OPCODE                   0x85
#define CSR_BT_OBEX_ABORT_OPCODE                      0xFF
#define CSR_BT_OBEX_ACTION_OPCODE                     0x86
#define CSR_BT_OBEX_INVALID_OPCODE                    0x10

/* Authentication */
#define CSR_BT_OBEX_MAX_AUTHENTICATE_LENGTH          (60)
#define CSR_BT_AUTH_TAG_LENGTH_SCHEME                (2)
#define CSR_BT_NONCE_LENGTH                          (0x10)
#define CSR_BT_OPTIONS_LENGTH                        (0x01)
#define CSR_BT_NONCE_TAG                             (0x00)
#define CSR_BT_OPTIONS_TAG                           (0x01)
#define CSR_BT_REALM_TAG                             (0x02)
#define CSR_BT_DIGEST_REQUEST_TAG                    (0x00)
#define CSR_BT_DIGEST_USERID_TAG                     (0x01)
#define CSR_BT_DIGEST_NONCE_TAG                      (0x02)
#define CSR_BT_DIGEST_NONCE_LENGTH                   (0x10)
#define CSR_BT_DIGEST_REQUEST_LENGTH                 (0x10)

/* Defines the Basic Imaging primary sessions */
#define CSR_BT_BASIC_IMAGING_IMAGE_PUSH                 {0xE3, 0x3D, 0x95, 0x45, 0x83, 0x74, 0x4A, 0xD7, 0x9E, 0xC5, 0xC1, 0x6B, 0xE3, 0x1E, 0xDE, 0x8E}
#define CSR_BT_BASIC_IMAGING_IMAGE_PULL                 {0x8E, 0xE9, 0xB3, 0xD0, 0x46, 0x08, 0x11, 0xD5, 0x84, 0x1A, 0x00, 0x02, 0xA5, 0x32, 0x5B, 0x4E}
#define CSR_BT_BASIC_IMAGING_ADVANCED_IMAGE_PRINTING    {0x92, 0x35, 0x33, 0x50, 0x46, 0x08, 0x11, 0xD5, 0x84, 0x1A, 0x00, 0x02, 0xA5, 0x32, 0x5B, 0x4E}
#define CSR_BT_BASIC_IMAGING_AUTOMATIC_ARCHIVE          {0x94, 0x01, 0x26, 0xC0, 0x46, 0x08, 0x11, 0xD5, 0x84, 0x1A, 0x00, 0x02, 0xA5, 0x32, 0x5B, 0x4E}
#define CSR_BT_BASIC_IMAGING_REMOTE_CAMERA              {0x94, 0x7E, 0x74, 0x20, 0x46, 0x08, 0x11, 0xD5, 0x84, 0x1A, 0x00, 0x02, 0xA5, 0x32, 0x5B, 0x4E}
#define CSR_BT_BASIC_IMAGING_REMOTE_DISPLAY             {0x94, 0xC7, 0xCD, 0x20, 0x46, 0x08, 0x11, 0xD5, 0x84, 0x1A, 0x00, 0x02, 0xA5, 0x32, 0x5B, 0x4E}

/* Defines the Basic Imaging secondary sessions */
#define CSR_BT_BASIC_IMAGING_REFERENCED_OBJECTS         {0x8E, 0x61, 0xF9, 0x5D, 0x1A, 0x79, 0x11, 0xD4, 0x8E, 0xA4, 0x00, 0x80, 0x5F, 0x9B, 0x98, 0x34}
#define CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS           {0x8E, 0x61, 0xF9, 0x5E, 0x1A, 0x79, 0x11, 0xD4, 0x8E, 0xA4, 0x00, 0x80, 0x5F, 0x9B, 0x98, 0x34}
#define CSR_BT_BASIC_IMAGING_ARCHIVED_OBJECTS_LENGTH    0x10

/* XHTML */
#define CSR_BT_IMAGE_LISTING_HEADER                     "<image-listing version=\"1.0\">\n"
#define CSR_BT_IMAGE_LISTING_TERMINATOR                 "</image-listing>"
#define CSR_BT_IMAGE_DESCRIPTOR_HEADER                  "<image-descriptor version=\"1.0\">\n"
#define CSR_BT_IMAGE_DESCRIPTOR_TERMINATOR              "</image-descriptor>"
#define CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_HEADER       "<attachment-descriptor version=\"1.0\">\n"
#define CSR_BT_IMAGE_ATTACHMENT_DESCRIPTOR_TERMINATOR   "</attachment-descriptor>"
#define CSR_BT_IMAGE_HANDLES_DESCRIPTOR_HEADER          "<image-handles-descriptor version=\"1.0\">\n"
#define CSR_BT_IMAGE_HANDLES_DESCRIPTOR_TERMINATOR      "</image-handles-descriptor>"
#define CSR_BT_IMAGE_HEADER                             "<image "
#define CSR_BT_IMAGE_TERMINATOR                         "/>\n"
#define CSR_BT_ATTACHMENT_HEADER                        "<attachment "
#define CSR_BT_ATTACHMENT_TERMINATOR                    "/>\n"
#define CSR_BT_FILTERING_PARAMETERS_HEADER              "<filtering-parameters "
#define CSR_BT_FILTERING_PARAMETERS_TERMINATOR          "/>\n"
#define CSR_BT_ENCODING_HEADER                          "encoding=\""
#define CSR_BT_PIXEL_HEADER                             "pixel=\""
#define CSR_BT_MAX_SIZE_HEADER                          "maxsize=\""
#define CSR_BT_TRANSFORMATION_HEADER                    "transformation=\""
#define CSR_BT_SIZE_HEADER                              "size=\""
#define CSR_BT_CONTENT_TYPE_HEADER                      "content-type=\""
#define CSR_BT_CHARSET_HEADER                           "charset=\""
#define CSR_BT_NAME_HEADER                              "name=\""
#define CSR_BT_SIMPLE_CREATED_HEADER                    "created="
#define CSR_BT_CREATED_HEADER                           "created=\""
#define CSR_BT_MODIFIED_HEADER                          "modified=\""
#define CSR_BT_HANDLE_HEADER                            "handle="
#define CSR_BT_COMMON_TERMINATOR                        "\" "
#define CSR_BT_SPACE                                    " "

#define CSR_BT_OBEX_REQUEST          (0x00)
#define CSR_BT_OBEX_RESPONSE_CONNECT (0x01)
#define CSR_BT_OBEX_RESPONSE_NORMAL  (0x02)

typedef CsrUint8 CsrBtObexSession;

/* GET macros */
#define CSR_BT_OBEX_GET_BYTE_AT(source, index) \
(((CsrUint8 *) source)[(index)])

#define CSR_BT_OBEX_GET_WORD_AT(source, index) \
((((CsrUint16) (((CsrUint8 *) source)[(index) + 0])) << 8) | \
 (((CsrUint16) (((CsrUint8 *) source)[(index) + 1]))))

#define CSR_BT_OBEX_GET_D_WORD_AT(source, index) \
((((CsrUint32) (((CsrUint8 *) source)[(index) + 0])) << 24) | \
 (((CsrUint32) (((CsrUint8 *) source)[(index) + 1])) << 16) | \
 (((CsrUint32) (((CsrUint8 *) source)[(index) + 2])) << 8) | \
 (((CsrUint32) (((CsrUint8 *) source)[(index) + 3]))))

/* Specific get macros */
#define CSR_BT_OBEX_GET_REQUEST_OPCODE(source)              CSR_BT_OBEX_GET_BYTE_AT((source), 0)
#define CSR_BT_OBEX_GET_REQUEST_LENGTH(source)              CSR_BT_OBEX_GET_WORD_AT((source), 1)
#define CSR_BT_OBEX_GET_RESPONSE_CODE(source)               CSR_BT_OBEX_GET_BYTE_AT((source), 0)
#define CSR_BT_OBEX_GET_RESPONSE_LENGTH(source)             CSR_BT_OBEX_GET_WORD_AT((source), 1)
#define CSR_BT_OBEX_GET_CONNECT_VERSION(source)             CSR_BT_OBEX_GET_BYTE_AT((source), 3)
#define CSR_BT_OBEX_GET_CONNECT_MAX_SIZE(source)            CSR_BT_OBEX_GET_WORD_AT((source), 5)
#define CSR_BT_OBEX_GET_CONNECT_SUCCESS_VERSION(source)     CSR_BT_OBEX_GET_BYTE_AT((source), 3)
#define CSR_BT_OBEX_GET_CONNECT_SUCCESS_MAX_SIZE(source)    CSR_BT_OBEX_GET_WORD_AT((source), 5)
#define CSR_BT_OBEX_GET_SET_PATH_FLAGS(source)              CSR_BT_OBEX_GET_BYTE_AT((source), 3)
#define CSR_BT_OBEX_GET_SET_PATH_CONSTANTS(source)          CSR_BT_OBEX_GET_BYTE_AT((source), 4)

/* SET macros */
#define CSR_BT_OBEX_SET_BYTE_AT(destination, index, value) \
(((CsrUint8 *) destination)[(index)] = (value))

#define CSR_BT_OBEX_SET_WORD_AT(destination, index, value) \
((((CsrUint8 *) destination)[(index) + 0] = ((CsrUint8) (((CsrUint16) value) >> 8))) , \
 (((CsrUint8 *) destination)[(index) + 1] = ((CsrUint8) (((CsrUint16) value) & 0x00FF))))

#define CSR_BT_OBEX_SET_D_WORD_AT(destination, index, value) \
((((CsrUint8 *) destination)[(index) + 0] = ((CsrUint8) ((((CsrUint32) value) >> 24)))) , \
 (((CsrUint8 *) destination)[(index) + 1] = ((CsrUint8) ((((CsrUint32) value) >> 16) & 0x000000FF))) , \
 (((CsrUint8 *) destination)[(index) + 2] = ((CsrUint8) ((((CsrUint32) value) >> 8) & 0x0000FFFF))) , \
 (((CsrUint8 *) destination)[(index) + 3] = ((CsrUint8) ((((CsrUint32) value) & 0x00FFFFFF)))))

/* Specific set macros */
#define CSR_BT_OBEX_SET_RESPONSE_CODE(destination, response_code)          CSR_BT_OBEX_SET_BYTE_AT((destination), 0, (response_code))
#define CSR_BT_OBEX_SET_RESPONSE_LENGTH(destination, length)               CSR_BT_OBEX_SET_WORD_AT((destination), 1, (length))
#define CSR_BT_OBEX_SET_REQUEST_CODE(destination, request_code)            CSR_BT_OBEX_SET_BYTE_AT((destination), 0, (request_code))
#define CSR_BT_OBEX_SET_REQUEST_LENGTH(destination, length)                CSR_BT_OBEX_SET_WORD_AT((destination), 1, (length))

#define CSR_BT_OBEX_SET_CONNECT_RESPONSE_VERSION(destination, version)     CSR_BT_OBEX_SET_BYTE_AT((destination), 3, (version))
#define CSR_BT_OBEX_SET_CONNECT_RESPONSE_FLAGS(destination, flags)         CSR_BT_OBEX_SET_BYTE_AT((destination), 4, (flags))
#define CSR_BT_OBEX_SET_CONNECT_RESPONSE_MAX_SIZE(destination, maxSize)    CSR_BT_OBEX_SET_WORD_AT((destination), 5, (maxSize))
#define CSR_BT_OBEX_SET_CONNECT_VERSION(destination, version)              CSR_BT_OBEX_SET_BYTE_AT((destination), 3, (version))
#define CSR_BT_OBEX_SET_CONNECT_FLAGS(destination, flags)                  CSR_BT_OBEX_SET_BYTE_AT((destination), 4, (flags))
#define CSR_BT_OBEX_SET_CONNECT_MAX_SIZE(destination, maxSize)             CSR_BT_OBEX_SET_WORD_AT((destination), 5, (maxSize))



#ifdef __cplusplus
}
#endif

#endif /* OBEX_COMMON_H_ */
