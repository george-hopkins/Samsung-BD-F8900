/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_tpt_prim.h"
#include "csr_bt_tpt_srv.h"


void tptScHandler(CsrBtTptServerInstanceDataType *TptData)
{
    CsrPrim *primType;

    primType = (CsrPrim *) TptData->recvMsgP;

    switch(*primType)
    {
        case CSR_BT_SC_PASSKEY_IND:
            {
                CsrBtScPasskeyInd *prim;
                prim = (CsrBtScPasskeyInd *)TptData->recvMsgP;

                TptData->peerAddr = prim->deviceAddr;

                CsrBtScPasskeyResSend(TRUE, TptData->peerAddr, PASSKEY_LEN, (CsrUint8 *) PASSKEY, TRUE, TRUE);
                break;
            }

        case CSR_BT_SC_SSP_JUSTWORKS_IND:
            {
                CsrBtScSspJustworksInd *prim;
                prim = (CsrBtScSspJustworksInd *)TptData->recvMsgP;

                TptData->peerAddr = prim->deviceAddr;

                CsrBtScSspJustWorksResSend(TRUE, TptData->peerAddr, TRUE, TRUE);
                break;
            }
        case CSR_BT_SC_BOND_IND:
            {
                /* Bonding complete */
                break;
            }
        default:
            {
                break;
            }
    }
    return;
}
