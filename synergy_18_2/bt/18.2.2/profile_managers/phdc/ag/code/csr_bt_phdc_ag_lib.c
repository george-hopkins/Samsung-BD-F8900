/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

#include "csr_msg_transport.h"
#include "csr_bt_phdc_ag_lib.h"

void CsrBtPhdcAgMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_PHDC_AG_IFACEQUEUE, CSR_BT_PHDC_AG_PRIM, msg);
}

#endif
