/****************************************************************************

   Copyright Cambridge Silicon Radio Limited and its licensors 2011.

   All rights reserved.

REVISION:      $Revision: #2 $
 ****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_profiles.h"
#include "csr_bt_therm_srv_lib.h"
#include "csr_msg_transport.h"

void CsrBtThermSrvMsgTransport(void* msg)
{
    CsrMsgTransport(CSR_BT_THERM_SRV_IFACEQUEUE, CSR_BT_THERM_SRV_PRIM, msg);
}
