/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sc_prim.h"
#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_cmd.h"
#include "csr_bt_tpt_app.h"
#include "csr_bt_demoapp.h"

void tptScHandler(CsrBtTptClientInstanceDataType *TptData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) TptData->recvMsgP;

    switch(*primType)
    {
        case CSR_BT_SC_PASSKEY_IND:
            {
                CsrBtScPasskeyInd *prim;

                csrBtTptDebugPrint("CSR_BT_SC_PASSKEY_IND\n");

                prim = (CsrBtScPasskeyInd *)TptData->recvMsgP;

                TptData->tptPeerAddr = prim->deviceAddr;

                CsrBtScPasskeyResSend(TRUE, TptData->tptPeerAddr, PASSKEY_LEN, (CsrUint8 *) PASSKEY, TRUE, TRUE);
                break;
            }
        case CSR_BT_SC_SSP_JUSTWORKS_IND:
            {
                CsrBtScSspJustWorksResSend(TRUE, TptData->tptPeerAddr, TRUE, TRUE);
                break;
            }
        case CSR_BT_SC_BOND_CFM:
            {
                CsrBtScBondCfm *prim;

                csrBtTptDebugPrint("CSR_BT_SC_BOND_CFM\n");

                prim = (CsrBtScBondCfm *) TptData->recvMsgP;

                if (prim->resultCode == CSR_BT_RESULT_CODE_SC_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_SC)
                {
                    csrBtTptClientPrint("Bonding complete.\n\n");
                    tptScheduleRun(TptData, CSR_BT_TPT_STATE_CONNECTING);
                    TptData->retries = 0;
                }
                else if(TptData->retries > CSR_BT_TPT_CONNECT_RETRIES)
                {
                    csrBtTptClientPrint("Error:  Bonding failed.\n");
                }
                else
                {
                    csrBtTptClientPrint("Bonding did not succeed. Trying again...\n");
                    CsrBtScBondReqSend(TptData->phandle, TptData->tptPeerAddr);
                    TptData->retries++;
                }
                break;
            }
        default:
            {
                CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, TptData->recvMsgP);
                csrBtTptClientPrint("Error:  Unexpected SC prim %u in state %u\n", *primType, TptData->state);
                break;
            }
    }
    return;
}
