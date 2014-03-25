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

void CsrBtFtcFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_BT_FTC_PRIM)
    {
        CsrBtFtcPrim *prim = (CsrBtFtcPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ
            case CSR_BT_FTC_PUT_OBJ_HEADER_REQ:
            {
                CsrBtFtcPutObjHeaderReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_HEADER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ
            case CSR_BT_FTC_SET_FOLDER_REQ:
            {
                CsrBtFtcSetFolderReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_SET_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_CONNECT_REQ
            case CSR_BT_FTC_CONNECT_REQ:
            {
                CsrBtFtcConnectReq *p = message;
                CsrPmemFree(p->realm);
                p->realm = NULL;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_CONNECT_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ
            case CSR_BT_FTC_PUT_OBJ_BODY_REQ:
            {
                CsrBtFtcPutObjBodyReq *p = message;
                CsrPmemFree(p->body);
                p->body = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_PUT_OBJ_BODY_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ
            case CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ:
            {
                CsrBtFtcSetObjPermissionsReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ
            case CSR_BT_FTC_COPYING_OBJ_REQ:
            {
                CsrBtFtcCopyingObjReq *p = message;
                CsrPmemFree(p->ucs2SrcName);
                p->ucs2SrcName = NULL;
                CsrPmemFree(p->ucs2DestName);
                p->ucs2DestName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_COPYING_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ
            case CSR_BT_FTC_MOVING_OBJ_REQ:
            {
                CsrBtFtcMovingObjReq *p = message;
                CsrPmemFree(p->ucs2SrcName);
                p->ucs2SrcName = NULL;
                CsrPmemFree(p->ucs2DestName);
                p->ucs2DestName = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_MOVING_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ
            case CSR_BT_FTC_GET_LIST_FOLDER_REQ:
            {
                CsrBtFtcGetListFolderReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_LIST_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES
            case CSR_BT_FTC_AUTHENTICATE_RES:
            {
                CsrBtFtcAuthenticateRes *p = message;
                CsrPmemFree(p->password);
                p->password = NULL;
                CsrPmemFree(p->userId);
                p->userId = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_AUTHENTICATE_RES */
#ifndef EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ
            case CSR_BT_FTC_GET_OBJ_REQ:
            {
                CsrBtFtcGetObjReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_GET_OBJ_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ
            case CSR_BT_FTC_SET_ADD_FOLDER_REQ:
            {
                CsrBtFtcSetAddFolderReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_SET_ADD_FOLDER_REQ */
#ifndef EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ
            case CSR_BT_FTC_DEL_OBJ_REQ:
            {
                CsrBtFtcDelObjReq *p = message;
                CsrPmemFree(p->ucs2name);
                p->ucs2name = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_BT_FTC_DEL_OBJ_REQ */
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
