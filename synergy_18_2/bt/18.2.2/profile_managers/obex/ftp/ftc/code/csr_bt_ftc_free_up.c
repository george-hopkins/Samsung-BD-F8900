/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_BT_FTC_MODULE
#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_bt_autogen.h"
#include "csr_bt_ftc_lib.h"
#include "csr_bt_ftc_prim.h"

void CsrBtFtcFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_FTC_PRIM)
    {
        CsrBtFtcPrim *prim = (CsrBtFtcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM
            case CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM:
            {
                CsrBtFtcGetListFolderBodyCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_BODY_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM
            case CSR_BT_FTC_GET_OBJ_CFM:
            {
                CsrBtFtcGetObjCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM
            case CSR_BT_FTC_GET_LIST_FOLDER_CFM:
            {
                CsrBtFtcGetListFolderCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_CFM */
#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND
            case CSR_BT_FTC_AUTHENTICATE_IND:
            {
                CsrBtFtcAuthenticateInd *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_IND */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM
            case CSR_BT_FTC_GET_OBJ_BODY_CFM:
            {
                CsrBtFtcGetObjBodyCfm *p = message;
                CsrPmemFree(p->payload);
                p->payload = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_BODY_CFM */
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
#endif /* EXCLUDE_CSR_BT_FTC_MODULE */
