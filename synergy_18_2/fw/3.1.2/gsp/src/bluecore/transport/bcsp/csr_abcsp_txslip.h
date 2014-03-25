#ifndef CSR_ABCSP_TXSLIP_H__
#define CSR_ABCSP_TXSLIP_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_abcsp_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

struct txslip_state
{
    CsrBool   escaping;
    CsrUint32 ubufindex;
    CsrUint32 ubufsiz;
    CsrUint8  ubuf[ABCSP_TXMSG_MAX_PAYLOAD_LEN]; /* Slip could be double up */
};


/* The delimiter is used to mark the start and end of each frame. */
#define abcsp_txslip_msgstart()          CsrAbcspTxSlipMsgDelim()
#define abcsp_txslip_msgend()            CsrAbcspTxSlipMsgDelim()


#ifdef __cplusplus
}
#endif

#endif
