#ifndef CSR_BT_AMPM_CONFIG_H
#define CSR_BT_AMPM_CONFIG_H

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

/* WARNING! DO NOT CHANGE ANYTHING IN THIS FILE!
 *
 * The values listed in this file are set according to the mandated
 * and/or recommended values on the Bluetooth Core
 * Specification. These values CAN NOT be used to optimise the
 * throughput. They control the A2MP protocol, and changing anything
 * may void the SIG qualification */

#ifdef __cplusplus
extern "C" {
#endif

/* How many controller we support (802.11, UWB, fake) */
#define CSR_BT_AMPM_MAX_LOCAL_AMPS                            (3)

/* Timeout settings (do not change!) */
#define CSR_BT_AMPM_SDU_REQUEST_RESPONSE_TIMEOUT              (20 * CSR_SCHED_SECOND)
#define CSR_BT_AMPM_HCI_RESPONSE_TIMEOUT                      (60 * CSR_SCHED_SECOND)
#define CSR_BT_AMPM_HCI_READ_INFO_RETRY                       (10 * CSR_SCHED_SECOND)

/* Misc constants */
#define CSR_BT_AMPM_L2CONF_MTU_MPS                            (670)
#define CSR_BT_AMPM_L2CONF_MAX_RETX                           (0xFF)
#define CSR_BT_AMPM_L2CONF_TXWIN                              (1)
#define CSR_BT_AMPM_L2CONF_RETRANS_TO                         (0x07d0) /* 2s */
#define CSR_BT_AMPM_L2CONF_MONITOR_TO                         (0x2ee0) /* 12s */
#define CSR_BT_AMPM_MAX_WRITE_REMOTE_ASSOC_SEGMENT            (665)
#define CSR_BT_AMPM_MAX_AMP_ASSOC_SIZE                        (664)
#define CSR_BT_AMPM_INVALID_SDU_IDENTIFIER                    (0)
#define CSR_BT_AMPM_INVALID_PHYSICAL_LINK_HANDLE              (0)
#define CSR_BT_AMPM_MAX_PHYSICAL_LINK_HANDLE                  (0xFF)
#define CSR_BT_AMPM_AMPLIST_CAPS_UNUSED                       (0x00000000)

/* AMP Get Info Response Status codes */
#define CSR_BT_AMPM_GET_INFO_RSP_STATUS_SUCCESS               0x00
#define CSR_BT_AMPM_GET_INFO_RSP_STATUS_INVALID_AMP_ID        0x01

/* AMP Get AMP Assoc Response Status codes */
#define CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_SUCCESS              0x00
#define CSR_BT_AMPM_GET_ASSOC_RSP_STATUS_INVALID_AMP_ID       0x01

/* AMP Create Physical Link Status codes */
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_STARTED              0x00
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_INVALID_AMP_ID       0x01
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_UNABLE_TO_START      0x02
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_COLLISION            0x03
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_RECEIVED_DISCONNECT  0x04
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_ALREADY_EXISTS       0x05
#define CSR_BT_AMPM_CREATE_PHYSICAL_LINK_SECURITY_VIOLATION   0x06

/* AMP Disconnect Physical Link Status codes */
#define CSR_BT_AMPM_DISCONNECT_LINK_SUCCESSFUL                0x00
#define CSR_BT_AMPM_DISCONNECT_LINK_INVALID_CONTROLLER_ID     0x01
#define CSR_BT_AMPM_DISCONNECT_LINK_NOT_IN_PROGRESS           0x02

/* AMP Command Reject Reason codes */
#define CSR_BT_AMPM_REJECT_REASON_NOT_RECOGNISED              0x0000

/* Special value to prevent SC_DB write non fake PALs */
#define CSR_BT_AMPM_SC_NO_DB_WRITE                            0xFF

/* Bit masks for using on the PAL capabilities. */
#define CSR_BT_AMPM_PAL_CAPABILITIES_GUARANTEED_LINKS_SUPPORTED 0x0001

/* Application move-ind move-rsp guard timer */
#define CSR_BT_AMPM_APP_MOVE_GUARD                            (2 * CSR_SCHED_SECOND)

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_AMPM_CONFIG_H */
