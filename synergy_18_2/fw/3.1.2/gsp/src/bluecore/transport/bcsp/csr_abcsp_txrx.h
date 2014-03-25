#ifndef CSR_ABCSP_TXRX_H__
#define CSR_ABCSP_TXRX_H__
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

/* Database of info common to tx and rx paths. */
struct txrx_info
{
    CsrBool  choke;                     /* Is the choke applied? */
    CsrBool  txsync_req;                /* bcsp-le sync requested. */
    CsrBool  txsyncresp_req;            /* bcsp-le syncresp requested. */
    CsrBool  txconf_req;                /* bcsp-le conf requested. */
    CsrBool  txconfresp_req;            /* bcsp-le confresp requested. */
    CsrUint8 rxseq_txack;               /* rxseq == txack. */
    CsrBool  txack_req;                 /* Request tx rxseq_txack. */
    CsrUint8 rxack;                     /* Received acknowledgement. */
    CsrBool  bcsple_muzzled;            /* bcsple passive start. */
    CsrBool  rxdemux_bcsple_msg;        /* rxdemux switch. */
    CsrBool  txcrc;                     /* Update crc on tx byte. */
};

#ifdef __cplusplus
}
#endif

#endif
