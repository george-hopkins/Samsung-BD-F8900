/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_PAC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_pac_lib.h"
#include "csr_bt_pac_prim.h"

void CsrBtPacFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_PAC_PRIM)
    {
        CsrBtPacPrim *prim = (CsrBtPacPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ
            case CSR_BT_PAC_SET_FOLDER_REQ:
            {
                CsrBtPacSetFolderReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_PB_REQ
            case CSR_BT_PAC_PULL_PB_REQ:
            {
                CsrBtPacPullPbReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_PULL_PB_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ
            case CSR_BT_PAC_PULL_VCARD_ENTRY_REQ:
            {
                CsrBtPacPullVcardEntryReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ
            case CSR_BT_PAC_PULL_VCARD_LIST_REQ:
            {
                CsrBtPacPullVcardListReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                CsrPmemFree(p->searchVal);
                p->searchVal = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_PULL_VCARD_LIST_REQ */
#ifndef EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES
            case CSR_BT_PAC_AUTHENTICATE_RES:
            {
                CsrBtPacAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_PAC_CONNECT_REQ
            case CSR_BT_PAC_CONNECT_REQ:
            {
                CsrBtPacConnectReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_PAC_CONNECT_REQ */
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
#endif /* EXCLUDE_CSR_BT_PAC_MODULE */
