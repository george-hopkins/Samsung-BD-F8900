/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_prim.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_demoapp.h"

void tptCmHandler(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType = (CsrPrim *)TptData->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
            {
                CsrBtCmWriteScanEnableCfm *prim;

                prim = (CsrBtCmWriteScanEnableCfm *) TptData->recvMsgP;

                if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    csrBtTptDebugPrint("CM write scan enable success\n");
                }
                else
                {
                    csrBtTptClientPrint("Error: CM write scan enable resultCode: 0x%02X and resultSupplier: 0x%02X\n", prim->resultCode, prim->resultSupplier);
                }

                break;
            }
        default:
            {
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, TptData->recvMsgP);

                csrBtTptClientPrint("Error:  Unexpected CM prim type %u in state init\n", *primType);
                break;
            }
    }

    return;
}
