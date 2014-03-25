/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

#include "csr_bt_phdc_mgr_lib.h"
#include "csr_msg_transport.h"

void CsrBtPhdcMgrMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_PHDC_MGR_IFACEQUEUE, CSR_BT_PHDC_MGR_PRIM, msg);
}

#endif
