/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_MAPS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_maps_lib.h"
#include "csr_bt_maps_prim.h"

void CsrBtMapsFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_MAPS_PRIM)
    {
        CsrBtMapsPrim *prim = (CsrBtMapsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND
            case CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND:
            {
                CsrBtMapsSelectMasInstanceInd *p = message;
                CsrPmemFree(p->masInstanceList);
                p->masInstanceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_SELECT_MAS_INSTANCE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND
            case CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND:
            {
                CsrBtMapsGetMessageListingHeaderInd *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                CsrPmemFree(p->filterPeriodBegin);
                p->filterPeriodBegin = NULL;
                CsrPmemFree(p->filterPeriodEnd);
                p->filterPeriodEnd = NULL;
                CsrPmemFree(p->filterRecipient);
                p->filterRecipient = NULL;
                CsrPmemFree(p->filterOriginator);
                p->filterOriginator = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND
            case CSR_BT_MAPS_SET_FOLDER_IND:
            {
                CsrBtMapsSetFolderInd *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_SET_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND
            case CSR_BT_MAPS_PUSH_MESSAGE_IND:
            {
                CsrBtMapsPushMessageInd *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND
            case CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND:
            {
                CsrBtMapsPushMessageHeaderInd *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND
            case CSR_BT_MAPS_GET_MESSAGE_HEADER_IND:
            {
                CsrBtMapsGetMessageHeaderInd *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_HEADER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM
            case CSR_BT_MAPS_GET_INSTANCE_IDS_CFM:
            {
                CsrBtMapsGetInstanceIdsCfm *p = message;
                CsrPmemFree(p->instanceIdsList);
                p->instanceIdsList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_INSTANCE_IDS_CFM */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND
            case CSR_BT_MAPS_SET_BACK_FOLDER_IND:
            {
                CsrBtMapsSetBackFolderInd *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_SET_BACK_FOLDER_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND
            case CSR_BT_MAPS_SET_MESSAGE_STATUS_IND:
            {
                CsrBtMapsSetMessageStatusInd *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_SET_MESSAGE_STATUS_IND */
#ifndef EXCLUDE_CSR_BT_MAPS_ABORT_IND
            case CSR_BT_MAPS_ABORT_IND:
            {
                CsrBtMapsAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_ABORT_IND */
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
#endif /* EXCLUDE_CSR_BT_MAPS_MODULE */
