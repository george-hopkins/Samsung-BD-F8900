#ifndef CSR_H4_EVENTS_H__
#define CSR_H4_EVENTS_H__
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

#define H4_EVENT(n)             CsrH4Event(n)


void CsrH4Event(CsrUint8 e);

/* Initialisation of the h4 transport protocol has started. */
#define H4_EVT_START                ((CsrUint8) (1))

/* Initialisation of the h4 transport protocol has been completed. */
#define H4_EVT_INITIATED               ((CsrUint8) (2))

/* Deinitialisation of the h4 transport protocol has been completed. */
#define H4_EVT_DEINITIATED             ((CsrUint8) (3))

/* The function h4_txmsg() has been called with dubious parameters. */
#define H4_EVT_DUFF_TX_PARAMS       ((CsrUint8) (4))

/* An attempt to allocate memory has failed. (Using the local malloc(),
alloc(), CsrPmemAlloc(), calloc(), etc.)  The h4 code will normally handle
this gracefully, but this event indicates the system is ill, and will
probably croak soon. */
#define H4_EVT_MALLOC               ((CsrUint8) (5))

/* The H4 driver has lost sync with the incoming byte stream */
#define H4_EVT_H4_SYNC_LOST         ((CsrUint8) (6))

/* An H4 message was received that's payload was too large. */
#define H4_EVT_OVERSIZE_RX_PAYLOAD  ((CsrUint8) (7))

/* The H4 driver has sent more data than the UART can handle */
#define H4_EVT_OVERRUN              ((CsrUint8) (8))

/* The H4 driver has sent more data than instructed to */
#define H4_EVT_TOO_MUCH_TX          ((CsrUint8) (9))

/* Received unknown parameter in RX message */
#define H4_EVT_DUFF_RX_PARAMS       ((CsrUint8) (10))

#ifdef __cplusplus
}
#endif

#endif
