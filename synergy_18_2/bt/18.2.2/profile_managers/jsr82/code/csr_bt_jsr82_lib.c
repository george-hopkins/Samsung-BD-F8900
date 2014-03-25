/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_msg_transport.h"
#include "csr_bt_jsr82_lib.h"

void CsrBtJsr82MsgTransport(void *msg)
{
    CsrMsgTransport(CSR_BT_JSR82_IFACEQUEUE, CSR_BT_JSR82_PRIM, msg);
}
