/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_msg_transport.h"
#include "csr_bt_maps_lib.h"

void CsrBtMapsMsgTransport(CsrSchedQid phandle, void *msg)
{
    CsrMsgTransport(phandle, CSR_BT_MAPS_PRIM, msg);
}
