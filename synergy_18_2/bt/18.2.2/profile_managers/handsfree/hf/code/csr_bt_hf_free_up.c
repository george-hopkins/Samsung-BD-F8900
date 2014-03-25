/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_HF_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf_prim.h"

void CsrBtHfFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_HF_PRIM)
    {
        CsrBtHfPrim *prim = (CsrBtHfPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
            case CSR_BT_HF_SERVICE_CONNECT_CFM:
            {
                CsrBtHfServiceConnectCfm *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->indicatorSupported);
                p->indicatorSupported = NULL;
                CsrPmemFree(p->indicatorValue);
                p->indicatorValue = NULL;
                CsrPmemFree(p->chldString);
                p->chldString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
            case CSR_BT_HF_SERVICE_CONNECT_IND:
            {
                CsrBtHfServiceConnectInd *p = message;
                CsrPmemFree(p->serviceName);
                p->serviceName = NULL;
                CsrPmemFree(p->indicatorSupported);
                p->indicatorSupported = NULL;
                CsrPmemFree(p->indicatorValue);
                p->indicatorValue = NULL;
                CsrPmemFree(p->chldString);
                p->chldString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */
#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
            case CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND:
            {
                CsrBtHfStatusIndicatorUpdateInd *p = message;
                CsrPmemFree(p->name);
                p->name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
            case CSR_BT_HF_C2C_SF_IND:
            {
                CsrBtHfC2cSfInd *p = message;
                CsrPmemFree(p->indicators);
                p->indicators = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
            case CSR_BT_HF_C2C_GET_SMS_CFM:
            {
                CsrBtHfC2cGetSmsCfm *p = message;
                CsrPmemFree(p->smsString);
                p->smsString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
            case CSR_BT_HF_CALL_NOTIFICATION_IND:
            {
                CsrBtHfCallNotificationInd *p = message;
                CsrPmemFree(p->clipString);
                p->clipString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
            case CSR_BT_HF_GET_CURRENT_CALL_LIST_IND:
            {
                CsrBtHfGetCurrentCallListInd *p = message;
                CsrPmemFree(p->clccString);
                p->clccString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */
#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
            case CSR_BT_HF_BT_INPUT_CFM:
            {
                CsrBtHfBtInputCfm *p = message;
                CsrPmemFree(p->dataRespString);
                p->dataRespString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */
#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
            case CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND:
            {
                CsrBtHfCallWaitingNotificationInd *p = message;
                CsrPmemFree(p->ccwaString);
                p->ccwaString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
            case CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM:
            {
                CsrBtHfGetCurrentOperatorSelectionCfm *p = message;
                CsrPmemFree(p->copsString);
                p->copsString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */
#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
            case CSR_BT_HF_AT_CMD_IND:
            {
                CsrBtHfAtCmdInd *p = message;
                CsrPmemFree(p->atCmdString);
                p->atCmdString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */
#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
            case CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM:
            {
                CsrBtHfGetAllStatusIndicatorsCfm *p = message;
                CsrPmemFree(p->indicatorSupported);
                p->indicatorSupported = NULL;
                CsrPmemFree(p->indicatorValue);
                p->indicatorValue = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */
#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
            case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND:
            {
                CsrBtHfGetSubscriberNumberInformationInd *p = message;
                CsrPmemFree(p->cnumString);
                p->cnumString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */
#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
            case CSR_BT_HF_C2C_TXT_IND:
            {
                CsrBtHfC2cTxtInd *p = message;
                CsrPmemFree(p->txtString);
                p->txtString = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */
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
#endif /* EXCLUDE_CSR_BT_HF_MODULE */
