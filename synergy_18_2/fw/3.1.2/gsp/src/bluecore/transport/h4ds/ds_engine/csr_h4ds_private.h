#ifndef CSR_H4DS_PRIVATE_H__
#define CSR_H4DS_PRIVATE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_h4ds.h"
#include "csr_h4ds_msgs.h"
#include "csr_h4ds_config.h"
#include "csr_h4ds_panics.h"
#include "csr_h4ds_events.h"
#include "csr_util.h"

#define H4DS_ONHOST

#ifdef H4DS_TEST
#include "csr_h4ds_test.h"
#endif /* H4DS_TEST */

#ifdef __cplusplus
extern "C" {
#endif

/* LE message identifiers - signals sent to/from h4ds-le state machine. */
enum lemsgid_enum
{
    lemsgid_none,                 /* No message. */
    lemsgid_sync,                 /* Sync message. */
    lemsgid_sack,                 /* Sync-acknowledge message. */
    lemsgid_conf,                 /* Conf message. */
    lemsgid_cack,                 /* Conf-acknowledge message. */
    lemsgid_tsync_timeout,        /* Message indicating Tsync timeout. */
    lemsgid_tconf_timeout         /* Message indicating Tconf timeout. */
};
typedef enum lemsgid_enum lemsgid;


/****************************************************************************
NAME
        CsrH4DSTxInit  -  initialise the h4ds transmit path

FUNCTION
        Initialises the transmit path of the h4ds library.  This must be
        called before any calls are made to other functions in h4ds_tx.c.
*/

void CsrH4DSTxInit(void);


/****************************************************************************
NAME
        CsrH4DSTxPump  -  run the transmit state machine

FUNCTION
        Run the h4ds library's transmit path and send any h4ds messages it
        emits to the UART.

        This function is intended to be called from CsrH4DSPump() only.
*/

void CsrH4DSTxPump(void);


/****************************************************************************
NAME
        CsrH4DSReqTxLeSync  -  req to send an le-sync msg to the peer

FUNCTION
        Request the transmit path to send a link establishment Sync message
        to the peer.
*/

void CsrH4DSReqTxLeSync(void);


/****************************************************************************
NAME
        CsrH4DSReqTxLeSack  -  req to send an le-sack msg to the peer

FUNCTION
        Request the transmit path to send a link establishment Sack message
        to the peer.
*/

void CsrH4DSReqTxLeSack(void);


/****************************************************************************
NAME
        CsrH4DSReqTxLeConf  -  req to send an le-conf msg to the peer

FUNCTION
        Request the transmit path to send a link establishment Conf message
        to the peer.
*/

void CsrH4DSReqTxLeConf(void);


/****************************************************************************
NAME
        CsrH4DSReqTxLeCack  -  req to send an le-cack msg to the peer

FUNCTION
        Request the transmit path to send a link establishment Cack message
        to the peer.
*/

void CsrH4DSReqTxLeCack(void);


/****************************************************************************
NAME
        CsrH4DSReqTxDsWu  -  request a wake-up msg to be sent to the peer

FUNCTION
        Request the transmit path to send a deep sleep Wake-Up message to the
        peer.
*/

void CsrH4DSReqTxDsWu(void);


/****************************************************************************
NAME
        CsrH4DSReqTxDsIa  -  request an i'm-awake msg to be sent to the peer

FUNCTION
        Request the transmit path to send a deep sleep I'm-Awake message to
        the peer.
*/

void CsrH4DSReqTxDsIa(void);


/****************************************************************************
NAME
        CsrH4DSReqTxDsYms  -  req a you-may-sleep msg to be sent to the peer

FUNCTION
        Request the transmit path to send a deep sleep You-May-Sleep message
        to the peer.
*/

void CsrH4DSReqTxDsYms(void);


/****************************************************************************
NAME
        CsrH4DsTxPathClear  -  are any h4ds tx messages pending?

RETURNS
        TRUE if the transmit path holds no h4ds messages awaiting
        transmission to the UART, else FALSE.

        In this context, the "h4ds messages" means the four link
        establishment messages and three deep sleep messages.  It does not
        include H4 traffic.
*/

CsrBool CsrH4DsTxPathClear(void);


/****************************************************************************
NAME
        CsrH4DSTxPumpCalled  -  signal call has been made to h4dx_tx_pump

FUNCTION
        Tells the deep sleep code that a call has been made to
        CsrH4DSTxPump().  This is needed for some rather awkward scheduling to
        determine when the transmit path is clear before taking a nap.
*/

void CsrH4DSTxPumpCalled(void);


/****************************************************************************
NAME
        CsrH4DSRxInit  -  initialise the received message scanner

FUNCTION
        Initialise the h4ds library's receive state machine.  This must be
        called before any use is made of the h4ds receive path.
*/

void CsrH4DSRxInit(void);


/****************************************************************************
NAME
        CsrH4DSRxInitMatchMsg  -  initialise h4ds message scanner

FUNCTION
        Prepare CsrH4DSRxMatchMsg() to match a fresh message.
*/

void CsrH4DSRxInitMatchMsg(void);


/****************************************************************************
NAME
        CsrH4DSRxMatchMsg  -  h4ds message scanner

FUNCTION
        This function matches the contents of the "len" byte buffer at "buf"
        with the set of h4ds messages, and reports when it matches a complete
        message.

        The treatment of a Wake-Up message is unusual.  A Wake-Up message is
        build from one or more "pre_wu" sequences, one "prez_wu" sequence and
        one "wu" sequence.  This function treats each of these sequences as a
        separate message.  Thus, a cleanly received Wake-Up message will
        cause this function to report one or more CsrSchedMsgId_pre_wu messages, one
        CsrSchedMsgId_prez_wu message and one CsrSchedMsgId_wu message.  It is up to the
        calling function to ensure these components appear in the correct
        order.

        The initialisation function CsrH4DSRxInitMatchMsg() must be called
        before making a first call to CsrH4DSRxMatchMsg().  After this, it
        is expected that a sequence of calls will be made to
        CsrH4DSRxMatchMsg(), each offering a block of bytes received, in
        sequence, from the h4ds peer, until CsrH4DSRxMatchMsg() has matched
        a complete h4ds message.  The function then reports the message
        matched.

RETURNS
        When a sequence of calls to this function has provided all of the
        bytes of an h4ds message, the function returns an identifier for the
        message matched.

        If this function has been given insufficient bytes to match a
        complete message, it returns CsrSchedMsgId_none.

        If this function cannot match "buf" to (part of) an h4ds message it
        returns CsrSchedMsgId_duff.

        *ntaken is set to the number of bytes consumed by this call to
        CsrH4DSRxMatchMsg().  If the function returns CsrSchedMsgId_duff, it does
        not consume the byte that could not be matched to an h4ds message.
        (For example, if the correct message is "abcd" and the caller offers
        "abde" then only "ab" will be consumed.)

NOTE
        This function internally retains state about a message being matched,
        so it cannot be used to match multiple received message streams
        simultaneously.
*/

ds_msgid CsrH4DSRxMatchMsg(const CsrUint8 *buf, CsrUint16 len, CsrUint16 *ntaken);


/****************************************************************************
NAME
        CsrH4DSFsmInit  -  initialise the h4ds deep sleep state machine

FUNCTION
        Initialise the h4ds deep sleep state machine.  This must be
        called before any other h4ds_ds.c functions.
*/

void CsrH4DSFsmInit(void);


/****************************************************************************
NAME
        CsrH4DSWuMsgRxed  -  wake-up message received

FUNCTION
        Tells the h4ds deep sleep state machine that a Wake-Up message
        has been received from the peer.  It is expected that the local
        h4ds deep sleep state machine will respond with an I'm-Awake message.
*/

void CsrH4DSWuMsgRxed(void);


/****************************************************************************
NAME
        CsrH4DSIaMsgRxed  -  i'm-awake message received

FUNCTION
        Tells the deep sleep state machine that an I'm-Awake message has been
        received from the peer.
*/

void CsrH4DSIaMsgRxed(void);


/****************************************************************************
NAME
        CsrH4DSYmsMsgRxed  -  you-may-sleep message received

FUNCTION
        Tells the h4ds deep sleep state machine that You-May-Sleep message
        has received from the peer.
*/

void CsrH4DSYmsMsgRxed(void);


/****************************************************************************
NAME
        CsrH4DSHlTxMsgAvail  -  higher layer transmit message available

FUNCTION
        Tells the h4ds deep sleep state machine that a higher layer message
        is available for transmission.  In this context, "higher layer" means
        either an h4 message or an h4ds link establishment message.
*/

void CsrH4DSHlTxMsgAvail(void);


/****************************************************************************
NAME
        CsrH4DSCanTransmit  -  does the h4ds deep sleep engine allows tx

RETURNS
        TRUE if the deep sleep state machine considers it safe to send h4 or
        link establishment messages to the peer, else FALSE.
*/

CsrBool CsrH4DSCanTransmit(void);


/****************************************************************************
NAME
        CsrH4DSCanReceive  -  does the h4ds deep sleep engine allows rx

RETURNS
        TRUE if the deep sleep state machine considers it safe to receive h4
        or link establishment messages from the peer, else FALSE.
*/

CsrBool CsrH4DSCanReceive(void);


/****************************************************************************
NAME
        CsrH4DSUartActive  -  inform ds sm of uart traffic

FUNCTION
        Tell the deep sleep state machine that some UART activity has been
        detected.  This is used to help determine whether the local machine
        should be allowed to enter its deep sleep state.
*/

void CsrH4DSUartActive(void);


/****************************************************************************
NAME
        CsrH4DSPumpInit  -  initialise the message pump

FUNCTION
        Initialise the h4ds pump code.  This must be called before any other
        CsrH4DSPump*() function is called.
*/

void CsrH4DSPumpInit(void);


/****************************************************************************
NAME
        CsrH4DSPumpTxReq  -  request a call to pump the h4ds tx code

FUNCTION
        Ensures we have made a request to H4DS_PUMP_REQ(), which asks the
        external code to call to CsrH4DSPump() at its earliest convenience.
*/

void CsrH4DSPumpTxReq(void);


/****************************************************************************
NAME
        CsrH4DSLeInit  -  initialise the link establishment entity

FUNCTION
        Initialise the h4ds link establishment state machine.  This will turn
        on the choke, preventing the flow of h4 traffic until link
        establishment has completed.
*/

void CsrH4DSLeInit(void);


/****************************************************************************
NAME
        CsrH4DSLeFsm  -  the h4ds link establishment entity state machine

FUNCTION
        Feeds the message "msg" into the h4ds link establishment state
        machine.  With proper feeding the link establishment state machine
        will be kind enough to remove the choke, allowing h4 traffic to
        flow.
*/

void CsrH4DSLeFsm(lemsgid msg);


/****************************************************************************
NAME
        CsrH4DSLeTxChoked  -  is the h4ds transmit path choked?

RETURNS
        TRUE if the h4ds link establishment state machine has completed the
        link establishment signalling with the peer and consequently is
        allowing h4 traffic to be sent to (and from) the peer, else FALSE.
*/

CsrBool CsrH4DSLeTxChoked(void);

/* Has the local LE engine completed its work? */
#define h4ds_le_complete()      (!CsrH4DSLeTxChoked())


/****************************************************************************
NAME
        CsrH4DSLeRxChoked  -  is the h4ds receive path choked?

RETURNS
        TRUE if the h4ds link establishment state machine has completed the
        first stage of link establishment signalling with the peer, and
        consequently is allowing h4 traffic to flow from the peer, else
        FALSE.
*/

CsrBool CsrH4DSLeRxChoked(void);


/****************************************************************************
NAME
        CsrH4DSLePause  -  pause the link establishment engine

FUNCTION
        Pause the link establishment state machine until it is unpaused by a
        call to CsrH4DSLeUnpause().

        This function exists to allow the deep sleep state machine to shut
        down the link establishment state machine when the peer cannot be
        contacted (e.g., UART wire drop out or peer crash).  In this state it
        is pointless to continue to run local code to try to rouse the peer
        as it just wastes power.  Hence the pause function to hold the link
        establishment state machine.
*/

void CsrH4DSLePause(void);


/****************************************************************************
NAME
        CsrH4DSLeUnpause  -  unpause the link establishment engine

FUNCTION
        Unpause the link establishment state machine.
*/

void CsrH4DSLeUnpause(void);


#ifdef __cplusplus
}
#endif

#endif
