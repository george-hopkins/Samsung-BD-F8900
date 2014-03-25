/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_syncs_lib.h"
#include "csr_bt_syncs_prim.h"

void CsrBtSyncsFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_SYNCS_PRIM)
    {
        CsrBtSyncsPrim *prim = (CsrBtSyncsPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
            case CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES:
            {
                CsrBtSyncsGetCalChangeLogRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
            case CSR_BT_SYNCS_PUT_PB_ENTRY_RES:
            {
                CsrBtSyncsPutPbEntryRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
            case CSR_BT_SYNCS_AUTHENTICATE_RES:
            {
                CsrBtSyncsAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
            case CSR_BT_SYNCS_GET_PB_ALL_RES:
            {
                CsrBtSyncsGetPbAllRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
            case CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES:
            {
                CsrBtSyncsGetCalInfoLogRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
            case CSR_BT_SYNCS_GET_CAL_ALL_RES:
            {
                CsrBtSyncsGetCalAllRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
            case CSR_BT_SYNCS_GET_PB_ENTRY_RES:
            {
                CsrBtSyncsGetPbEntryRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
            case CSR_BT_SYNCS_PUT_CAL_ENTRY_RES:
            {
                CsrBtSyncsPutCalEntryRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
            case CSR_BT_SYNCS_PUT_NEXT_RES:
            {
                CsrBtSyncsPutNextRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
            case CSR_BT_SYNCS_PUT_RES:
            {
                CsrBtSyncsPutRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
            case CSR_BT_SYNCS_GET_CAL_ENTRY_RES:
            {
                CsrBtSyncsGetCalEntryRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
            case CSR_BT_SYNCS_AUTHENTICATE_REQ:
            {
                CsrBtSyncsAuthenticateReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
            case CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES:
            {
                CsrBtSyncsPutPbAddEntryRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
            case CSR_BT_SYNCS_GET_PB_INFO_LOG_RES:
            {
                CsrBtSyncsGetPbInfoLogRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
            case CSR_BT_SYNCS_GET_RES:
            {
                CsrBtSyncsGetRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
            case CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES:
            {
                CsrBtSyncsPutCalAddEntryRes *p = message;
                CsrPmemFree(p->appParameter);
                p->appParameter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
            case CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES:
            {
                CsrBtSyncsGetPbCurChangeLogRes *p = message;
                CsrPmemFree(p->changeCounter);
                p->changeCounter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
            case CSR_BT_SYNCS_GET_DEVICE_INFO_RES:
            {
                CsrBtSyncsGetDeviceInfoRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
            case CSR_BT_SYNCS_GET_NEXT_RES:
            {
                CsrBtSyncsGetNextRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
            case CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES:
            {
                CsrBtSyncsGetCalCurChangeLogRes *p = message;
                CsrPmemFree(p->changeCounter);
                p->changeCounter = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */
#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
            case CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES:
            {
                CsrBtSyncsGetPbChangeLogRes *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */
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
#endif /* EXCLUDE_CSR_BT_SYNCS_MODULE */
