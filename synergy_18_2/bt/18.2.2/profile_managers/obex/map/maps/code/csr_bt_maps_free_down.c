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

void CsrBtMapsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_MAPS_PRIM)
    {
        CsrBtMapsPrim *prim = (CsrBtMapsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES
            case CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES:
            {
                CsrBtMapsGetMessageListingHeaderRes *p = message;
                CsrPmemFree(p->mseTime);
                p->mseTime = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES
            case CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES:
            {
                CsrBtMapsPushMessageHeaderRes *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_HEADER_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES
            case CSR_BT_MAPS_GET_FOLDER_LISTING_RES:
            {
                CsrBtMapsGetFolderListingRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_FOLDER_LISTING_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES
            case CSR_BT_MAPS_PUSH_MESSAGE_RES:
            {
                CsrBtMapsPushMessageRes *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_PUSH_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ
            case CSR_BT_MAPS_ACTIVATE_REQ:
            {
                CsrBtMapsActivateReq *p = message;
                CsrPmemFree(p->name);
                p->name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES
            case CSR_BT_MAPS_EVENT_NOTIFICATION_RES:
            {
                CsrBtMapsEventNotificationRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_EVENT_NOTIFICATION_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES
            case CSR_BT_MAPS_GET_MESSAGE_RES:
            {
                CsrBtMapsGetMessageRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES
            case CSR_BT_MAPS_GET_MESSAGE_LISTING_RES:
            {
                CsrBtMapsGetMessageListingRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPS_GET_MESSAGE_LISTING_RES */
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
