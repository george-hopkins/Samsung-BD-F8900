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

void CsrBtMapcFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_MAPC_PRIM)
    {
        CsrBtMapcPrim *prim = (CsrBtMapcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ
            case CSR_BT_MAPC_GET_MESSAGE_REQ:
            {
                CsrBtMapcGetMessageReq *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ
            case CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ:
            {
                CsrBtMapcGetMessageListingReq *p = message;
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
#endif /* EXCLUDE_CSR_BT_MAPC_GET_MESSAGE_LISTING_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES
            case CSR_BT_MAPC_PUSH_MESSAGE_RES:
            {
                CsrBtMapcPushMessageRes *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_RES */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ
            case CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ:
            {
                CsrBtMapcSetMessageStatusReq *p = message;
                CsrPmemFree(p->messageHandle);
                p->messageHandle = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_SET_MESSAGE_STATUS_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ
            case CSR_BT_MAPC_SET_FOLDER_REQ:
            {
                CsrBtMapcSetFolderReq *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ
            case CSR_BT_MAPC_PUSH_MESSAGE_REQ:
            {
                CsrBtMapcPushMessageReq *p = message;
                CsrPmemFree(p->folderName);
                p->folderName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_MAPC_PUSH_MESSAGE_REQ */
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
