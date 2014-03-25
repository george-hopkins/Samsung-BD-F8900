#ifndef CSR_ABCSP_CONFIG_RXMSG_H__
#define CSR_ABCSP_CONFIG_RXMSG_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Use #defines rather than typedefs to aid portability.  The abcsp code
treats these types as opaque references, so void* is appropriate. */

#include "csr_abcsp_intf_hl.h"
#include "csr_tm_bluecore_transport_descriptor.h"
#include "csr_tm_bluecore_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ABCSP_RXMSG
#define ABCSP_RXMSG             void
#endif

/* The maximum length of the payload field of a received BCSP packet.

A check is made on the payload length field in received BCSP headers
and any that declare a payload length greater than this value are
consumed and discarded without bothering the ABCSP_RXMSG_*()
functions.  It is presumed the length value is invalid because of UART
line corruption.  If this value is #defined then its value must be set
with knowledge of the transmit packet length settings on the peer.
This length excludes the BCSP header and possible CRC.

If this value is not #defined then no check or limit on the length of packets
is imposed. */


/****************************************************************************
NAME
        ABCSP_DELIVERMSG  -  pass abcsp message to higher level code

SYNOPSIS
        void ABCSP_DELIVERMSG(abcsp *_this, ABCSP_RXMSG *msg, CsrUint32 chan, CsrUint32 rel);

FUNCTION
    Passes the BCSP message "msg" up to higher level code.  The message
    was received on BCSP channel "chan".  If the message was received on
    the reliable BCSP path then "rel" is 1, else it is 0.

    This function only passes a reference to the message to the higher
    level code.  The reference was obtained by a previous call to
    ABCSP_RXMSG_CREATE() - see config_rxmsg.h.  This call indicates that
    the abcsp library passes all control of the message back to external
    code.
*/
#define ABCSP_DELIVERMSG(m, c, r)    CsrTmBlueCoreTransportMsgRx(CsrBluecoreTransportBcsp, m)


/****************************************************************************
NAME
        ABCSP_RXMSG_DESTROY  -  destroy a abcsp receive message

SYNOPSIS
        void ABCSP_RXMSG_DESTROY(abcsp *_this, ABCSP_RXMSG *m);

FUNCTION
        Destroys the message identified by the message reference "m".

    All resources identified by "m" are no longer of interest to the
    abcsp library.
*/

#define ABCSP_RXMSG_DESTROY(m)    CsrAbcspRxMsgDestroy(m)

#ifdef __cplusplus
}
#endif

#endif
