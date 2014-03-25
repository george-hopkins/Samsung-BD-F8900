/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_MAPC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_mapc_lib.h"
#include "csr_bt_mapc_prim.h"

void CsrBtMapcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_MAPC_PRIM)
    {
        CsrBtMapcPrim *prim = (CsrBtMapcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM
            case CSR_BT_MAPC_GET_FOLDER_LISTING_CFM:
            {
                CsrBtMapcGetFolderListingCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM
            case CSR_BT_MAPC_GET_INSTANCE_IDS_CFM:
            {
                CsrBtMapcGetInstanceIdsCfm *p = message;
                CsrPmemFree(p->instanceIdsList);
                p->instanceIdsList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_INSTANCE_IDS_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND
            case CSR_BT_MAPC_EVENT_NOTIFICATION_IND:
            {
                CsrBtMapcEventNotificationInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM
            case CSR_BT_MAPC_PUSH_MESSAGE_CFM:
            {
                CsrBtMapcPushMessageCfm *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM
            case CSR_BT_MAPC_GET_MESSAGE_CFM:
            {
                CsrBtMapcGetMessageCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND
            case CSR_BT_MAPC_GET_MESSAGE_LISTING_IND:
            {
                CsrBtMapcGetMessageListingInd *p = message;
                CsrPmemFree(p->mseTime);
                p->mseTime = NULL;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND
            case CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND:
            {
                CsrBtMapcSelectMasInstanceInd *p = message;
                CsrPmemFree(p->masInstanceList);
                p->masInstanceList = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_SELECT_MAS_INSTANCE_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM
            case CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM:
            {
                CsrBtMapcGetMessageListingCfm *p = message;
                CsrPmemFree(p->mseTime);
                p->mseTime = NULL;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_CFM */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND
            case CSR_BT_MAPC_GET_MESSAGE_IND:
            {
                CsrBtMapcGetMessageInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND
            case CSR_BT_MAPC_GET_FOLDER_LISTING_IND:
            {
                CsrBtMapcGetFolderListingInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_FOLDER_LISTING_IND */
#ifndef EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND
            case CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND:
            {
                CsrBtMapcEventNotificationAbortInd *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_EVENT_NOTIFICATION_ABORT_IND */
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
#endif /* EXCLUDE_CSR_BT_MAPC_MODULE */
