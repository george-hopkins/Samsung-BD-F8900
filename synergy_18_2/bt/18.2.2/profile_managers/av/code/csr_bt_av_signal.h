#ifndef CSR_BT_AV_SIGNAL_H__
#define CSR_BT_AV_SIGNAL_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_av_main.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Transaction label */
#define TLABLE_MASK                           ((CsrUint8) 0xf0)

/* Packet type */
#define PKT_TYPE_SINGLE                       ((CsrUint8) 0x00)
#define PKT_TYPE_START                        ((CsrUint8) 0x04)
#define PKT_TYPE_CONTINUE                     ((CsrUint8) 0x08)
#define PKT_TYPE_END                          ((CsrUint8) 0x0c)
#define PKT_TYPE_MASK                         ((CsrUint8) 0x0c)

/* Message types */
#define MSG_TYPE_MASK                         ((CsrUint8) 0x03)
#define MSG_TYPE_CMD                          ((CsrUint8) 0x00)
#define MSG_TYPE_GENERAL_REJ                  ((CsrUint8) 0x01)
#define MSG_TYPE_ACP                          ((CsrUint8) 0x02)
#define MSG_TYPE_REJ                          ((CsrUint8) 0x03)

/* Signal identifiers */
#define AVDTP_DISCOVER_SID                    ((CsrUint8) 0x01)
#define AVDTP_GET_CAPABILITIES_SID            ((CsrUint8) 0x02)
#define AVDTP_SET_CONFIGURATION_SID           ((CsrUint8) 0x03)
#define AVDTP_GET_CONFIGURATION_SID           ((CsrUint8) 0x04)
#define AVDTP_RECONFIGURE_SID                 ((CsrUint8) 0x05)
#define AVDTP_OPEN_SID                        ((CsrUint8) 0x06)
#define AVDTP_START_SID                       ((CsrUint8) 0x07)
#define AVDTP_CLOSE_SID                       ((CsrUint8) 0x08)
#define AVDTP_SUSPEND_SID                     ((CsrUint8) 0x09)
#define AVDTP_ABORT_SID                       ((CsrUint8) 0x0a)
#define AVDTP_SECURITY_CONTROL_SID            ((CsrUint8) 0x0b)
#define AVDTP_GET_ALL_CAPABILITIES_SID        ((CsrUint8) 0x0c)
#define AVDTP_DELAYREPORT_SID                 ((CsrUint8) 0x0d)
#define SIGNAL_ID_MASK                        ((CsrUint8) 0x3f)

/* Message lengths */
#define AVDTP_SIGNALLING_HDR_LEN              (0x02)
#define AVDTP_DISCOVER_SID_LEN                (0x02)
#define AVDTP_GET_CAPABILITIES_SID_LEN        (0x03)
#define AVDTP_GET_CONFIGURATION_SID_LEN       (0x03)
#define AVDTP_SET_CONFIGURATION_SID_MIN_LEN   (0x06)
#define AVDTP_RECONFIGURE_SID_MIN_LEN         (0x03)
#define AVDTP_OPEN_SID_LEN                    (0x03)
#define AVDTP_START_SID_MIN_LEN               (0x02)
#define AVDTP_CLOSE_SID_LEN                   (0x03)
#define AVDTP_SUSPEND_SID_MIN_LEN             (0x02)
#define AVDTP_ABORT_SID_LEN                   (0x03)
#define AVDTP_SECURITY_CONTROL_SID_MIN_LEN    (0x03)
#define AVDTP_DELAYREPORT_SID_LEN             (0x05)

#define BUILD_MSG_HDR(hdr,msgType,label,id) hdr[0] = ((CsrUint8) (msgType | (label << 4))); \
                                                hdr[1] = id;

#define AV_START_SIGNAL_TIMER(sid,label,stream) \
                CsrSchedTimerSet( (1000 * CSR_BT_AV_CMD_SIGNAL_TIMEOUT), \
                                (void (*) (CsrUint16, void *)) timeOutHandler, \
                                (CsrUint16) ((sid << 8) | (label & 0x0f)<<4 | (stream & 0x0f) ), \
                                (void *) instData);

typedef void (* avSignalHandler_t)(av_instData_t * instData, CsrUint8 conId);

#ifdef __cplusplus
}
#endif

#endif
