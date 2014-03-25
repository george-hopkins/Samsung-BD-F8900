/*!
        (c) CSR plc 2010
        All rights reserved

\file   rfcomm_config.h
\brief  Configuration options for use only within the RFCOMM module.
*/

#ifndef _RFCOMM_CONFIG_H_
#define _RFCOMM_CONFIG_H_

#include "csr_synergy.h"

#include "csr_sched.h"
#include "l2cap_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RFCOMM response and acknowledgement timers
   RFC_CTRL_ACK_TIMER_VALUE :- Timeout for SABM frames sent
   RFC_CMD_RESP_TIMER_VALUE :- Timeout for commands sent in UIH frames on dlci 0
   RFC_L2CAP_RELEASE_TIMER_VALUE :-Timeout for DISC frames sent after which the l2cap
                                   channel is dropped.
   RFC_MUX_SHUTDOWN_TIMER_VALUE :- Timeout after which the MUX is dropped after the
                                   last DLC is closed. Cancelled if a new DLC is
                                   opened within that time.
   RFC_CREDIT_RETURN_TIMER_VALUE :- Timeout after which any outstanding credits on
                                    a particular DLC will automatically be
                                    returned to the peer device

   WARNING: Altering the values of these timeouts could seriously compromise the
            functionality of RFCOMM!!
*/
#define RFC_CTRL_ACK_TIMER_VALUE              (60 * CSR_SCHED_SECOND)
#define RFC_CMD_RESP_TIMER_VALUE              (60 * CSR_SCHED_SECOND)
#define RFC_L2CAP_RELEASE_TIMER_VALUE         (30 * CSR_SCHED_SECOND)
#define RFC_MUX_SHUTDOWN_TIMER_VALUE          (1 * CSR_SCHED_SECOND)
#define RFC_CREDIT_RETURN_TIMER_VALUE         (500 *CSR_SCHED_MILLISECOND)  /* 0.5 seconds */

/* This is the fraction of the number of allocated rx credits that must have
   been accumulated before automatically triggering a credit only data frame
   to be sent to the peer.
*/
#define RFC_CREDIT_SEND_THRESHOLD  4     /* 1/4 of the total allocated */

/* Set l2cap config ERTM options. If we are on host then set the preferred
   mode to Enhanced retransmission and the fallback mode to Basic. However if
   we are on chip force the mode to be Basic.
*/
#ifdef BUILD_FOR_HOST

#define RFC_L2CAP_RSP_MODE CSR_BT_RFCOMM_PREFERRED_MODE
#define RFC_L2CAP_RSP_FALLBKMODE L2CA_FLOW_MODE_BASIC

#else

#define RFC_L2CAP_RSP_MODE CSR_BT_RFCOMM_PREFERRED_MODE
#define RFC_L2CAP_RSP_FALLBKMODE L2CA_FLOW_MODE_BASIC

#endif

#ifdef __cplusplus
}
#endif

#endif /* _RFCOMM_PRIVATE_H_ */
