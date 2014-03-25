/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_msg_transport.h"
#include "csr_bt_profiles.h"
#include "csr_bt_fts_lib.h"

void CsrBtFtsMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_FTS_IFACEQUEUE, CSR_BT_FTS_PRIM, msg);
}
