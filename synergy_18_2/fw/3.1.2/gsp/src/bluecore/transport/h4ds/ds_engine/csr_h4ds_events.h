#ifndef CSR_H4DS_EVENTS_H__
#define CSR_H4DS_EVENTS_H__
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

/* Initialisation of the h4ds library has started. */

#define H4DS_EVT_START          ((CsrUint8) (1))


/* Initialisation of the h4ds library has been completed. */

#define H4DS_EVT_INITIATED         ((CsrUint8) (2))


/* The h4ds engine has lost sync while in its awake state.  Something has
gone seriously wrong.  External code should treat this alert as seriously as
an h4 sync loss. */

#define H4DS_EVT_SYNC_LOSS      ((CsrUint8) (3))


/* The h4ds code has allowed the peer to sleep. Consequently, any subsequent
calls to h4ds_tx_msg() should be rejected until the peer is perceived to be
awake again.  While the peer is apparently asleep all calls to h4ds_tx_msg()
will be rejected. */

#define H4DS_EVT_PEER_MAY_SLEEP ((CsrUint8) (4))


/* The h4ds code believes the peer is awake and able to receive
link establishment and h4 traffic.  If the H4DS_EVT_LE_CONF event has
also been emitted and if the H4DS_EVT_PEER_RESTART event has not been
emitted then the h4ds code will normally pass h4 traffic. */

#define H4DS_EVT_PEER_AWAKE     ((CsrUint8) (5))


/* The h4ds library allows the local device to enter its deep sleep state, in
which the local UART can be disabled.  See the description of
H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP. */

#define H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP ((CsrUint8) (6))


/* The h4ds library no longer allows the local device to enter its deep sleep
state; the local UART must remain enabled.  External code may use
H4DS_EVT_LOCAL_DEVICE_MAY_SLEEP and this event to decide whether the local
device may enter its deep sleep state. */

#define H4DS_EVT_LOCAL_DEVICE_MAY_NOT_SLEEP ((CsrUint8) (7))


/* The h4ds link establishment engine has established sync with its peer and
the choke has been removed locally.  Messages may now be accepted by
h4ds_tx_msg() and CsrH4DSUartDeliverbytes(). */

#define H4DS_EVT_LE_SYNC        ((CsrUint8) (8))


/* The h4ds link establishment engine has received confirmation from the peer
that it's in its "garrulous" state, so allowing generation of the sync-lost
event below. */

#define H4DS_EVT_LE_CONF        ((CsrUint8) (9))


/* The h4ds link establishment engine has detected that the peer h4ds-le
engine has restarted.  This presumably means that the peer h4ds stack or
system has been restarted (e.g., crash and reboot).  The common local
response would be to restart (CsrH4DSInit()) the local h4ds stack.  The h4ds
engine will not pass any h4 traffic after emitting this event. */

#define H4DS_EVT_PEER_RESTART   ((CsrUint8) (10))

#ifdef __cplusplus
}
#endif

#endif
