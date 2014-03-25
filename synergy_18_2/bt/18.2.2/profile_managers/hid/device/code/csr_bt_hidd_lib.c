/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_profiles.h"
#include "csr_msg_transport.h"
#include "csr_bt_hidd_lib.h"

void CsrBtHiddMsgTransport(CsrSchedQid phandle, void *msg)
{
    CsrMsgTransport(phandle, CSR_BT_HIDD_PRIM, msg);
}
