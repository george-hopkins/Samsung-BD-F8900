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

void CsrBtAvrcpFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_AVRCP_PRIM)
    {
        CsrBtAvrcpPrim *prim = (CsrBtAvrcpPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
            case CSR_BT_AVRCP_CONFIG_REQ:
            {
                CsrBtAvrcpConfigReq *p = message;
                CsrPmemFree(p->tgDetails.providerName);
                p->tgDetails.providerName = NULL;
                CsrPmemFree(p->tgDetails.serviceName);
                p->tgDetails.serviceName = NULL;
                CsrPmemFree(p->ctDetails.providerName);
                p->ctDetails.providerName = NULL;
                CsrPmemFree(p->ctDetails.serviceName);
                p->ctDetails.serviceName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
            case CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ:
            {
                CsrBtAvrcpCtPasValTxtReq *p = message;
                CsrPmemFree(p->valId);
                p->valId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
            case CSR_BT_AVRCP_TG_PAS_SET_REQ:
            {
                CsrBtAvrcpTgPasSetReq *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
            case CSR_BT_AVRCP_TG_MP_REGISTER_REQ:
            {
                CsrBtAvrcpTgMpRegisterReq *p = message;
                CsrPmemFree(p->pas);
                p->pas = NULL;
                CsrPmemFree(p->playerName);
                p->playerName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
            case CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ:
            {
                CsrBtAvrcpCtInformDispCharsetReq *p = message;
                CsrPmemFree(p->charset);
                p->charset = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
            case CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES:
            {
                CsrBtAvrcpTgGetAttributesRes *p = message;
                CsrPmemFree(p->attribData);
                p->attribData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
            case CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES:
            {
                CsrBtAvrcpTgGetFolderItemsRes *p = message;
                CsrPmemFree(p->items);
                p->items = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
            case CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ:
            {
                CsrBtAvrcpCtUnitInfoCmdReq *p = message;
                CsrPmemFree(p->pData);
                p->pData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
            case CSR_BT_AVRCP_TG_PAS_CURRENT_RES:
            {
                CsrBtAvrcpTgPasCurrentRes *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
            case CSR_BT_AVRCP_CT_SEARCH_REQ:
            {
                CsrBtAvrcpCtSearchReq *p = message;
                CsrPmemFree(p->text);
                p->text = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
            case CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ:
            {
                CsrBtAvrcpCtSubUnitInfoCmdReq *p = message;
                CsrPmemFree(p->pData);
                p->pData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
            case CSR_BT_AVRCP_CT_PAS_CURRENT_REQ:
            {
                CsrBtAvrcpCtPasCurrentReq *p = message;
                CsrPmemFree(p->attId);
                p->attId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
            case CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES:
            {
                CsrBtAvrcpTgSetBrowsedPlayerRes *p = message;
                CsrPmemFree(p->folderNames);
                p->folderNames = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
            case CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ:
            {
                CsrBtAvrcpCtPasAttTxtReq *p = message;
                CsrPmemFree(p->attId);
                p->attId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
            case CSR_BT_AVRCP_CT_PAS_SET_REQ:
            {
                CsrBtAvrcpCtPasSetReq *p = message;
                CsrPmemFree(p->attValPair);
                p->attValPair = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */
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
