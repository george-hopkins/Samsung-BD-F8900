/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_CM_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_free_handcoded.h"

void CsrBtCmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_CM_PRIM)
    {
        CsrBtCmPrim *prim = (CsrBtCmPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
            case CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ:
            {
                CsrBtCmEirUpdateManufacturerDataReq *p = message;
                CsrPmemFree(p->manufacturerData);
                p->manufacturerData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
            case CSR_BT_CM_MAP_SCO_PCM_RES:
            {
                CsrBtCmMapScoPcmRes *p = message;
                CsrPmemFree(p->parms);
                p->parms = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
            case CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ:
            {
                CsrBtCmBnepExtendedMulticastDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
            case CSR_BT_CM_SET_LOCAL_NAME_REQ:
            {
                CsrBtCmSetLocalNameReq *p = message;
                CsrPmemFree(p->friendlyName);
                p->friendlyName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
            case CSR_BT_CM_SDC_RFC_SEARCH_REQ:
            {
                CsrBtCmSdcRfcSearchReq *p = message;
                CsrPmemFree(p->serviceList);
                p->serviceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
            case CSR_BT_CM_SDC_SEARCH_REQ:
            {
                CsrBtCmSdcSearchReq *p = message;
                CsrPmemFree(p->serviceList);
                p->serviceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
            case CSR_BT_CM_WRITE_LINK_POLICY_REQ:
            {
                CsrBtCmWriteLinkPolicyReq *p = message;
                CsrPmemFree(p->sniffSettings);
                p->sniffSettings = NULL;
                CsrPmemFree(p->parkSettings);
                p->parkSettings = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
            case CSR_BT_CM_SDS_REGISTER_REQ:
            {
                CsrBtCmSdsRegisterReq *p = message;
                CsrPmemFree(p->serviceRecord);
                p->serviceRecord = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
            case CSR_BT_CM_L2CA_DATA_REQ:
            {
                CsrBtCmL2caDataReq *p = message;
                CsrMblkDestroy(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
            case CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ:
            {
                CsrBtCmSdcRfcExtendedSearchReq *p = message;
                CsrPmemFree(p->serviceList);
                p->serviceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
            case CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ:
            {
                CsrBtCmSdcUuid128RfcSearchReq *p = message;
                CsrPmemFree(p->serviceList);
                p->serviceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
            case CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ:
            {
                CsrBtCmL2caConnectAcceptReq *p = message;
                CsrPmemFree(p->conftab);
                p->conftab = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
            case CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ:
            {
                CsrBtCmL2caConnectionlessDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
            case CSR_BT_CM_SCO_CONNECT_REQ:
            {
                CsrBtCmScoConnectReq *p = message;
                CsrPmemFree(p->parms);
                p->parms = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
            case CSR_BT_CM_SDC_UUID128_SEARCH_REQ:
            {
                CsrBtCmSdcUuid128SearchReq *p = message;
                CsrPmemFree(p->serviceList);
                p->serviceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
            case CSR_BT_CM_SM_LINK_KEY_REQUEST_RES:
            {
                CsrBtCmSmLinkKeyRequestRes *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */
#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
            case CSR_BT_CM_L2CA_CONNECT_REQ:
            {
                CsrBtCmL2caConnectReq *p = message;
                CsrPmemFree(p->conftab);
                p->conftab = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
            case CSR_BT_CM_BNEP_EXTENDED_DATA_REQ:
            {
                CsrBtCmBnepExtendedDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
            case CSR_BT_CM_SDC_SERVICE_SEARCH_REQ:
            {
                CsrBtCmSdcServiceSearchReq *p = message;
                CsrPmemFree(p->uuidSet);
                p->uuidSet = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */
#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
            case CSR_BT_CM_LE_ADVERTISE_REQ:
            {
                CsrBtCmLeAdvertiseReq *p = message;
                CsrPmemFree(p->advertisingData);
                p->advertisingData = NULL;
                CsrPmemFree(p->scanResponseData);
                p->scanResponseData = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
            case CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ:
            {
                CsrBtCmDmL2caModeSettingsReq *p = message;
                CsrPmemFree(p->sniffSettings);
                p->sniffSettings = NULL;
                CsrPmemFree(p->parkSettings);
                p->parkSettings = NULL;
                CsrPmemFree(p->ssrSettings);
                p->ssrSettings = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
            case CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ:
            {
                CsrBtCmDmBnepModeSettingsReq *p = message;
                CsrPmemFree(p->sniffSettings);
                p->sniffSettings = NULL;
                CsrPmemFree(p->parkSettings);
                p->parkSettings = NULL;
                CsrPmemFree(p->ssrSettings);
                p->ssrSettings = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
            case CSR_BT_CM_GET_SECURITY_CONF_RES:
            {
                CsrBtCmGetSecurityConfRes *p = message;
                CsrPmemFree(p->leEr);
                p->leEr = NULL;
                CsrPmemFree(p->leIr);
                p->leIr = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */
#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
            case CSR_BT_CM_DATA_REQ:
            {
                CsrBtCmDataReq *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
            case CSR_BT_CM_DM_MODE_SETTINGS_REQ:
            {
                CsrBtCmDmModeSettingsReq *p = message;
                CsrPmemFree(p->sniffSettings);
                p->sniffSettings = NULL;
                CsrPmemFree(p->parkSettings);
                p->parkSettings = NULL;
                CsrPmemFree(p->ssrSettings);
                p->ssrSettings = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */
#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
            case CSR_BT_CM_SM_KEY_REQUEST_RES:
            {
                CsrBtCmSmKeyRequestRes *p = message;
                CsrPmemFree(p->key);
                p->key = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */
            default:
            {
                CsrBtCmFreeHandcoded(prim);
                break;
            }
        } /* End switch */
    } /* End if */
    else
    {
        /* Unknown primitive type, exception handling */
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODULE */
