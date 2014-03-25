#ifndef CSR_ABCSP_EVENTS_H__
#define CSR_ABCSP_EVENTS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrAbcspEvent(CsrUint8 e);

/* Initialisation of the abcsp library has started. */
#define ABCSP_EVT_START             ((CsrUint8) (1))

/* Initialisation of the abcsp library has been completed. */
#define ABCSP_EVT_INITIATED         ((CsrUint8) (2))

/* The BCSP Link Establishment engine has established sync with its peer
and the choke has been removed locally.  Messages will now be accepted
by CsrAbcspMsgSend() and csrAbcspUartBytesDeliver(). */
#define ABCSP_EVT_LE_SYNC           ((CsrUint8) (3))

/* The BCSP Link Establishment engine has received confirmation from the peer
that it's in its "garrulous" state, so allowing generation of the sync-lost
event below. */
#define ABCSP_EVT_LE_CONF           ((CsrUint8) (4))

/* The BCSP Link Establishment engine has detected that the peer BCSP-LE
engine has restarted.  This presumably means that the peer BCSP stack
(or system) has been restarted.  A common local response would be to
restart (CsrAbcspInit()) the local BCSP stack. */
#define ABCSP_EVT_LE_SYNC_LOST      ((CsrUint8) (5))

/* An attempt has been made to use an uninitialised block of code.  This
should only mean that a call has been made to the abcsp library before
CsrAbcspInit() has been called. */
#define ABCSP_EVT_UNINITIATED       ((CsrUint8) (6))

/* The rx SLIP engine has established sync on the received byte stream. */
#define ABCSP_EVT_SLIP_SYNC         ((CsrUint8) (7))

/* The rx SLIP engine has lost sync on the received byte stream. */
#define ABCSP_EVT_SLIP_SYNC_LOST    ((CsrUint8) (8))

/* A message received by the BCSP stack has been discarded because the local
choke is applied.  This is a reasonable occurrence where the peer BCSP stack
has removed its choke (shortly) before the local choke is removed. */
#define ABCSP_EVT_RX_CHOKE_DISCARD ((CsrUint8) (9))

/* A message submitted to CsrAbcspMsgSend() has been rejected because the
local choke is applied. */
#define ABCSP_EVT_TX_CHOKE_DISCARD ((CsrUint8) (10))

/* A message submitted to CsrAbcspMsgSend() has been rejected because the
BCSP transmit window is full. */
#define ABCSP_EVT_TX_WINDOW_FULL_DISCARD ((CsrUint8) (11))

/* The header of a received message has declared its payload to be
longer than the limit set by the #define ABCSP_RXMSG_MAX_PAYLOAD_LEN
and the message is being discarded.  (See the #define's description
in abcsp.h.) */
#define ABCSP_EVT_OVERSIZE_DISCARD ((CsrUint8) (12))

/* A received reliable message has been rejected because its sequence
number is not that expected by the local BCSP stack. */
#define ABCSP_EVT_MISSEQ_DISCARD ((CsrUint8) (13))

/* A received message's header checksum has failed and the message is
being discarded. */
#define ABCSP_EVT_CHECKSUM       ((CsrUint8) (14))

/* A received message held fewer bytes than its header declared. */
#define ABCSP_EVT_SHORT_PAYLOAD  ((CsrUint8) (15))

/* A received message is being discarded because its CRC check failed. */
#define ABCSP_EVT_CRC_FAIL       ((CsrUint8) (17))

#ifdef __cplusplus
}
#endif

#endif  /* __ABCSP_EVENTS_H__ */
