#ifndef CSR_ABCSP_TXMSG_H__
#define CSR_ABCSP_TXMSG_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_types.h"
#include "csr_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ABCSP_TXMSG             void

/* Message builder's state. */
enum txstate_enum
{
    txstate_idle,               /* Waiting for new message to send. */
    txstate_msgstart,           /* Sending slip frame start byte. */
    txstate_hdr1,               /* Sending first bcsp header byte. */
    txstate_hdr2,               /* Sending second bcsp header byte. */
    txstate_hdr3,               /* Sending third bcsp header byte. */
    txstate_hdr4,               /* Sending last bcsp header byte. */
    txstate_payload,            /* Sending message payload. */
    txstate_crc1,               /* Sending first crc byte. */
    txstate_crc2,               /* Sending second crc byte. */
    txstate_msgend              /* Sending slip frame end byte. */
};
typedef enum txstate_enum txstate;

/* An internally-generated message (bcsp-le or ack). */
typedef struct
{
    CsrUint8 *buf;                   /* Message byte buffer. */
    CsrUint8  outdex;               /* Buffer extraction point. */
    CsrUint8  chan;                 /* BCSP channel. */
} INTMSG;

/* The message currently being transmitted. */
typedef struct
{
    CsrUint16 len;              /* Message length. */
    CsrBool   rel;            /* Reliable message? */
    /* Discriminated union. */
#define CURRMSG_TYPE_TXMSG      (0)
#define CURRMSG_TYPE_INTMSG     (1)
    CsrBool  type;           /* Discrim: TXMSG or internally-generated. */
    CsrUint8 seq;            /* Sequence number. */
    union
    {
        TXMSG *txmsg;
        INTMSG inter;
    } m;
} CURRMSG;

struct txmsg_state
{
    /* The ''state''. */
    txstate state;

    /* The current message */
    CURRMSG curr;

    /* Array of reliable CsrAbcspMsgSend() messages. */
    TXMSG rel[8];

    /* This is the sequence number of the next packet passed to
     * us.  This is also the index into the circular buffer where
     * we will add a new message.  No message actual has this
     * sequence number at the moment. */
    CsrUint32 msgq_txseq;

    /* This is the sequence number of the oldest packet that we
     * sent that has not ackd yet.  Imagine that msgq_unackd_seq
     * is less than or equal to msgq_txseq.  We have packets in
     * this range (though not including msgq_txseq) in the
     * retransmit Q. */
    CsrUint32 msgq_unackd_txseq;

    /* Reliable packet sequence number - used by fsm to emit
     * packets.  This is the sequence number that will be used for
     * the next packet that we send. */
    CsrUint32 txseq;

    /* Although we dont queue unreliable messages, we still need
     * two slots here.  In one we store the message that we are
     * sending, and in the other we store a new message that might
     * be sent to us.
     *
     * unrel_index points to the slot that we can store a new
     * message into.
 */
    CsrBool   unrelUsed[2];
    TXMSG     unrel[2];
    CsrUint32 unrel_index;

    /* BCSP packet header checksum. */
    CsrUint8 cs;

    /* BCSP packet crc. */
    CsrUint16 crc;
};

#ifdef __cplusplus
}
#endif

#endif
