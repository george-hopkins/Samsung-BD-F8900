#ifndef CSR_TYPE_A_COM_H__
#define CSR_TYPE_A_COM_H__
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

#define SDIO_CHANNEL_HCI (1)
#define SDIO_CHANNEL_ACL (2)
#define SDIO_CHANNEL_SCO (3)
#define SDIO_CHANNEL_HCI_EVT (4)

CsrBool CsrSdioBtADrvStart(CsrUint8 reset);
CsrBool CsrSdioBtADrvStop(void);
CsrBool CsrSdioBtADrvRestart(void);

/*************************************************************
 * NAME:
 *      CsrSdioBtADrvTx
 *
 * DESCRIPTION:
 *      Store a SDIO Type-A packet for transmit to chip.
 * PARAM:
 *      data the SDIO Type-A formattet packet
 *      size the of packet
 *
 * Note: channel is NOT used in current implementation
 *
 * RETURNS:
 *      TRUE when stored on transmit queue
 *************************************************************/
CsrBool CsrSdioBtADrvTx(CsrUint8 *data, CsrUint32 size);

/* Note: Rx is performed by function 'SdioBtADrvRx' calling CsrTmBlueCoreTypeADeliverMsg in
 *  file csr_type_a_sched.c
 */

#ifdef __cplusplus
}
#endif

#endif
