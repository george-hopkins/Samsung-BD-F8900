#ifndef CSR_ABCSP_RX_H__
#define CSR_ABCSP_RX_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Slip decoder's state. */
enum rxslipstate_enum
{
    rxslipstate_uninit,             /* Uninitialised. */
    rxslipstate_init_nosync,        /* First time unsynchronised. */
    rxslipstate_nosync,             /* No SLIP synchronisation. */
    rxslipstate_start,              /* Sending initial c0. */
    rxslipstate_body,               /* Sending message body. */
    rxslipstate_body_esc            /* Received escape character. */
};

typedef enum rxslipstate_enum rxslipstate;

struct rxslip_state
{
    rxslipstate state;
    CsrUint32   index;
    CsrUint8    buf[ABCSP_RXMSG_MAX_PAYLOAD_LEN]; /*Size could be double up (slip chars) */
};

CsrUint32 CsrTmBlueCoreBcspDeliverMsg(const CsrUint8 *data, CsrUint32 dataLength);

#ifdef __cplusplus
}
#endif

#endif
