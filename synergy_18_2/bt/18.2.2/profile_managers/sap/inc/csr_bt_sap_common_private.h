#ifndef CSR_BT_SAP_COMMON_PRIVATE_H__
#define CSR_BT_SAP_COMMON_PRIVATE_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_BT_FIND_PADDING(x) ((x % 4) ? 4 - (x % 4) : 0)

/* ConnectionStatus */
#define CSR_BT_OK_CONNECT                              0x00
#define CSR_BT_UNABLE_TO_ESTABLISH_CONNECT             0x01
#define CSR_BT_MAX_MSG_SIZE_NOT_SUPPORTED              0x02
#define CSR_BT_MSG_SIZE_TO_SMALL                       0x03
#define CSR_BT_OK_ONGOING_CALL                         0x04

/* ResultCodes */
#define CSR_BT_RSLT_OK_REQUEST                         0x00
#define CSR_BT_RSLT_ERR_NO_REASON                      0x01
#define CSR_BT_RSLT_ERR_CARD_NOT_ACC                   0x02
#define CSR_BT_RSLT_ERR_CARD_POW_OFF                   0x03
#define CSR_BT_RSLT_ERR_CARD_REMOVED                   0x04
#define CSR_BT_RSLT_ERR_CARD_POW_ON                    0x05
#define CSR_BT_RSLT_ERR_DATA_NOT_AVAIL                 0x06
#define CSR_BT_RSLT_ERR_NOT_SUPPORTED                  0x07
#define CSR_BT_RSLT_ERR_INVALID_MSG                    0xFF

/* Length of the message header */
#define CSR_BT_MSG_HEADER_LENGTH                       0x04
#define CSR_BT_MIN_PAR_LENGTH                          0x04

/* Indexes in to the payload */
#define CSR_BT_MSGID_INDEX                             0x00
#define CSR_BT_NUMBER_OF_PARAMETER_INDEX               0x01
#define CSR_BT_FIRST_PARAMETER_INDEX                   0x04
#define CSR_BT_FIRST_PARAMETER_VALUE_INDEX             0x08
#define CSR_BT_MAX_MSG_SIZE_INDEX_LSB                  0x09
#define CSR_BT_MAX_MSG_SIZE_INDEX_MSB                  0x08

/* Message Ids */
#define CSR_BT_MSGID_CONNECT_REQ                       0x00
#define CSR_BT_MSGID_CONNECT_RES                       0x01
#define CSR_BT_MSGID_DISCONNECT_REQ                    0x02
#define CSR_BT_MSGID_DISCONNECT_RES                    0x03
#define CSR_BT_MSGID_DISCONNECT_IND                    0x04
#define CSR_BT_MSGID_TRANSFER_APDU_REQ                 0x05
#define CSR_BT_MSGID_TRANSFER_APDU_RES                 0x06
#define CSR_BT_MSGID_TRANSFER_ATR_REQ                  0x07
#define CSR_BT_MSGID_TRANSFER_ATR_RES                  0x08
#define CSR_BT_MSGID_POWER_SIM_OFF_REQ                 0x09
#define CSR_BT_MSGID_POWER_SIM_OFF_RES                 0x0A
#define CSR_BT_MSGID_POWER_SIM_ON_REQ                  0x0B
#define CSR_BT_MSGID_POWER_SIM_ON_RES                  0x0C
#define CSR_BT_MSGID_RESET_SIM_REQ                     0x0D
#define CSR_BT_MSGID_RESET_SIM_RES                     0x0E
#define CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_REQ   0x0F
#define CSR_BT_MSGID_TRANSFER_CARD_READER_STATUS_RES   0x10
#define CSR_BT_MSGID_STATUS_IND                        0x11
#define CSR_BT_MSGID_ERROR_RES                         0x12
#define CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_REQ         0x13
#define CSR_BT_MSGID_SET_TRANSFER_PROTOCOL_RES         0x14

/* Parameter IDs */
#define CSR_BT_PARAM_ID_MAX_MSG_SIZE                   0x00
#define CSR_BT_PARAM_ID_CONNECTION_STATUS              0x01
#define CSR_BT_PARAM_ID_RESULT_CODE                    0x02
#define CSR_BT_PARAM_ID_DISCONNECTION_TYPE             0x03
#define CSR_BT_PARAM_ID_COMMAND_APDU                   0x04
#define CSR_BT_PARAM_ID_RESPONSE_APDU                  0x05
#define CSR_BT_PARAM_ID_ATR                            0x06
#define CSR_BT_PARAM_ID_CARD_READER_STATUS             0x07
#define CSR_BT_PARAM_ID_STATUS_CHANGE                  0x08
#define CSR_BT_PARAM_ID_TRANSFER_PROTOCOL              0x09
#define CSR_BT_PARAM_ID_COMMAND_APDU7816               0x10

/* SAP Command length */
#define CSR_BT_LENGTH_CONNECT_REQ                      12
#define CSR_BT_LENGTH_CONNECT_RES                      20
#define CSR_BT_LENGTH_STATUS_IND                       12
#define CSR_BT_LENGTH_DISCONNECT_RES                   4
#define CSR_BT_LENGTH_DISCONNECT_REQ                   4
#define CSR_BT_LENGTH_DISCONNECT_IND                   12
#define CSR_BT_LENGTH_ERROR_RES                        4
#define CSR_BT_LENGTH_TRANSFER_ATR_REQ                 4
#define CSR_BT_LENGTH_TRANSFER_ATR_RES                 16 /* variable parameter length */
#define CSR_BT_LENGTH_TRANSFER_APDU_RES                16 /* variable parameter length */
#define CSR_BT_LENGTH_TRANSFER_APDU_REQ                8
#define CSR_BT_LENGTH_POWER_SIM_OFF_REQ                4
#define CSR_BT_LENGTH_POWER_OFF_RES                    12
#define CSR_BT_LENGTH_POWER_SIM_ON_REQ                 4
#define CSR_BT_LENGTH_POWER_ON_RES                     12
#define CSR_BT_LENGTH_RESET_SIM_REQ                    4
#define CSR_BT_LENGTH_RESET_SIM_RES                    12
#define CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_REQ  4
#define CSR_BT_LENGTH_TRANSFER_CARD_READER_STATUS_RES  20
#define CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_REQ        12
#define CSR_BT_LENGTH_SET_TRANSFER_PROTOCOL_RES        12

#ifdef __cplusplus
}
#endif

#endif

