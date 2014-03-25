/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_avrcp_prim.h"

void CsrBtAvrcpFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AVRCP_PRIM)
    {
        CsrBtAvrcpPrim *prim = (CsrBtAvrcpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
            case CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND:
            {
                CsrBtAvrcpCtPasValTxtInd *p = message;
                CsrPmemFree(p->pasData);
                p->pasData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
            case CSR_BT_AVRCP_TG_PAS_SET_IND:
            {
                CsrBtAvrcpTgPasSetInd *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
            case CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM:
            {
                CsrBtAvrcpCtPasAttTxtCfm *p = message;
                CsrPmemFree(p->pasData);
                p->pasData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
            case CSR_BT_AVRCP_TG_PAS_CURRENT_IND:
            {
                CsrBtAvrcpTgPasCurrentInd *p = message;
                CsrPmemFree(p->attId);
                p->attId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
            case CSR_BT_AVRCP_CONNECT_CFM:
            {
                CsrBtAvrcpConnectCfm *p = message;
                CsrPmemFree(p->tgFeatures.providerName);
                p->tgFeatures.providerName = NULL;
                CsrPmemFree(p->tgFeatures.serviceName);
                p->tgFeatures.serviceName = NULL;
                CsrPmemFree(p->ctFeatures.providerName);
                p->ctFeatures.providerName = NULL;
                CsrPmemFree(p->ctFeatures.serviceName);
                p->ctFeatures.serviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
            case CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND:
            {
                CsrBtAvrcpCtPasAttTxtInd *p = message;
                CsrPmemFree(p->pasData);
                p->pasData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
            case CSR_BT_AVRCP_REMOTE_FEATURES_IND:
            {
                CsrBtAvrcpRemoteFeaturesInd *p = message;
                CsrPmemFree(p->tgFeatures.providerName);
                p->tgFeatures.providerName = NULL;
                CsrPmemFree(p->tgFeatures.serviceName);
                p->tgFeatures.serviceName = NULL;
                CsrPmemFree(p->ctFeatures.providerName);
                p->ctFeatures.providerName = NULL;
                CsrPmemFree(p->ctFeatures.serviceName);
                p->ctFeatures.serviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
            case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM:
            {
                CsrBtAvrcpCtGetAttributesCfm *p = message;
                CsrPmemFree(p->attribData);
                p->attribData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
            case CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM:
            {
                CsrBtAvrcpCtSetBrowsedPlayerCfm *p = message;
                CsrPmemFree(p->folderNames);
                p->folderNames = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
            case CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND:
            {
                CsrBtAvrcpTgInformDispCharsetInd *p = message;
                CsrPmemFree(p->charset);
                p->charset = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
            case CSR_BT_AVRCP_CT_PAS_SET_IND:
            {
                CsrBtAvrcpCtPasSetInd *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
            case CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM:
            {
                CsrBtAvrcpCtUnitInfoCmdCfm *p = message;
                CsrPmemFree(p->pData);
                p->pData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
            case CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM:
            {
                CsrBtAvrcpCtPasValTxtCfm *p = message;
                CsrPmemFree(p->pasData);
                p->pasData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
            case CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM:
            {
                CsrBtAvrcpCtGetFolderItemsCfm *p = message;
                CsrPmemFree(p->itemsData);
                p->itemsData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
            case CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM:
            {
                CsrBtAvrcpCtPasAttIdCfm *p = message;
                CsrPmemFree(p->attId);
                p->attId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
            case CSR_BT_AVRCP_CT_PAS_CURRENT_CFM:
            {
                CsrBtAvrcpCtPasCurrentCfm *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
            case CSR_BT_AVRCP_TG_SEARCH_IND:
            {
                CsrBtAvrcpTgSearchInd *p = message;
                CsrPmemFree(p->text);
                p->text = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
            case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND:
            {
                CsrBtAvrcpCtGetAttributesInd *p = message;
                CsrPmemFree(p->attribData);
                p->attribData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
            case CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM:
            {
                CsrBtAvrcpCtSubUnitInfoCmdCfm *p = message;
                CsrPmemFree(p->pData);
                p->pData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
            case CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM:
            {
                CsrBtAvrcpCtPasValIdCfm *p = message;
                CsrPmemFree(p->valId);
                p->valId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */
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
#endif /* EXCLUDE_CSR_BT_AVRCP_MODULE */
