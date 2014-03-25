#ifndef CSR_BT_PPP_HDLC_H__
#define CSR_BT_PPP_HDLC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_ppp_sef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HDLC_FRAME_DELIMITER    0x7E
#define HDLC_ESCAPE_CHARACTER    0x7D
#define ESCAPE_BYTE(x)            ((x) ^ 0x20)
#define UN_ESCAPE_BYTE(x)        ((x) ^ 0x20)

/* RX states */
#define HDLC_STATE_SEARCHING_FOR_FRAME_DELIMITER        0
#define HDLC_STATE_HEADER_1                             1
#define HDLC_STATE_HEADER_2                             2
#define HDLC_STATE_PROTOCOL_1                           3
#define HDLC_STATE_PROTOCOL_2                           4
#define HDLC_STATE_SEARCHING_FOR_END_FRAME_DELIMITER    5

/* TX states */
#define HDLC_STATE_TRANSMIT_START_DELIMITER             0
#define HDLC_STATE_TRANSMIT_HEADER_1                    1
#define HDLC_STATE_TRANSMIT_HEADER_2                    2
#define HDLC_STATE_TRANSMIT_PROTOCOL_1                  3
#define HDLC_STATE_TRANSMIT_PROTOCOL_2                  4
#define HDLC_STATE_TRANSMIT_DATA                        5
#define HDLC_STATE_TRANSMIT_FCS_1                       6
#define HDLC_STATE_TRANSMIT_FCS_2                       7
#define HDLC_STATE_TRANSMIT_END_DELIMITER               8


extern CsrBool PppHdlcReceive(PppInstanceData * instanceData);
extern void PppHdlcTransmit(PppInstanceData * instanceData);

#ifdef __cplusplus
}
#endif

#endif

