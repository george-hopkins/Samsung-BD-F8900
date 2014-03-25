/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"


void tptCmHandler(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
            {
                /* Ready to accept connection */
                break;
            }
        default:
            {
                break;
            }
    }

    return;
}
