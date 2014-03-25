/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HFG_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hfg_lib.h"
#include "csr_bt_hfg_prim.h"

void CsrBtHfgFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HFG_PRIM)
    {
        CsrBtHfgPrim *prim = (CsrBtHfgPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
            case CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM:
            {
                CsrBtHfgConfigAtcmdHandlingCfm *p = message;
                CsrPmemFree(p->bitwiseIndicators);
                p->bitwiseIndicators = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */
#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
            case CSR_BT_HFG_DIAL_IND:
            {
                CsrBtHfgDialInd *p = message;
                CsrPmemFree(p->number);
                p->number = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */
#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
            case CSR_BT_HFG_STATUS_AUDIO_IND:
            {
                CsrBtHfgStatusAudioInd *p = message;
                CsrPmemFree(p->audioSetting);
                p->audioSetting = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */
#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
            case CSR_BT_HFG_C2C_SF_IND:
            {
                CsrBtHfgC2cSfInd *p = message;
                CsrPmemFree(p->indicators);
                p->indicators = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */
#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
            case CSR_BT_HFG_SERVICE_CONNECT_IND:
            {
                CsrBtHfgServiceConnectInd *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
            case CSR_BT_HFG_AT_CMD_IND:
            {
                CsrBtHfgAtCmdInd *p = message;
                CsrPmemFree(p->command);
                p->command = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */
            default:
            {
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_MODULE */
