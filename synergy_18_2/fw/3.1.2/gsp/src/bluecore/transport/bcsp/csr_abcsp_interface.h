#ifndef CSR_ABCSP_INTERFACE_H__
#define CSR_ABCSP_INTERFACE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ABCSP_FRAME_SIZE_TEMP            (1024)

/* Note that the ABCSP RX / TX also should hold the size of the SDP
 * records */

/* Holds the TX slip buffer size for tx data*/
#define ABCSP_TXMSG_MAX_PAYLOAD_LEN      ((ABCSP_FRAME_SIZE_TEMP * 2) + 14)

/* If a received message payload is greater than ABCSP_MAX_MSG_LEN the
   messages is discarded without further notis */
#define ABCSP_RXMSG_MAX_PAYLOAD_LEN      ((ABCSP_FRAME_SIZE_TEMP * 2) + 14)

/* The #define ABCSP_MAX_MSG_LEN sets the maximum number of bytes that
   can be carried in a bcsp message payload.  This must be 4095 or
   smaller.  All real systems use messages much smaller than this.
   The abcsp library PANIC()s if an attempt is made to send a message
   with a payload larger than this value.  See
   ABCSP_RXMSG_MAX_PAYLOAD_LEN */
#define ABCSP_MAX_MSG_LEN                (ABCSP_FRAME_SIZE_TEMP + 14)

#ifdef __cplusplus
}
#endif

#endif
