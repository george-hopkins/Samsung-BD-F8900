#ifndef CSR_BT_LSL_TO_PRIM_CONVERTER_H__
#define CSR_BT_LSL_TO_PRIM_CONVERTER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_bt_dunc_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LslDataReqSend(length, data) \
        CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, data, length)

#define LslDataResSend() \
        CsrBtDuncDataResSend(CSR_BT_DUNC_IFACEQUEUE)

#define LslDisconnectReqSend()
        /* Disconnect is handled by other app */

#define LslConnectReqSend(queue, bdAddr)
        /* Connect is handled by other app */

#ifdef __cplusplus
}
#endif

#endif
