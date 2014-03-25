/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifndef EXCLUDE_CSR_UI_MODULE
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_mblk.h"
#include "csr_autogen.h"
#include "csr_ui_lib.h"
#include "csr_ui_prim.h"

void CsrUiFreeDownstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_UI_PRIM)
    {
        CsrUiPrim *prim = (CsrUiPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ
            case CSR_UI_IDLESCREEN_SET_REQ:
            {
                CsrUiIdlescreenSetReq *p = message;
                CsrPmemFree(p->heading);
                p->heading = NULL;
                CsrPmemFree(p->text);
                p->text = NULL;
                CsrPmemFree(p->textSK1);
                p->textSK1 = NULL;
                CsrPmemFree(p->textSK2);
                p->textSK2 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_SET_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_ADDITEM_REQ
            case CSR_UI_MENU_ADDITEM_REQ:
            {
                CsrUiMenuAdditemReq *p = message;
                CsrPmemFree(p->label);
                p->label = NULL;
                CsrPmemFree(p->sublabel);
                p->sublabel = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_MENU_ADDITEM_REQ */
#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ
            case CSR_UI_INPUTDIALOG_SET_REQ:
            {
                CsrUiInputdialogSetReq *p = message;
                CsrPmemFree(p->heading);
                p->heading = NULL;
                CsrPmemFree(p->message);
                p->message = NULL;
                CsrPmemFree(p->text);
                p->text = NULL;
                CsrPmemFree(p->textSK1);
                p->textSK1 = NULL;
                CsrPmemFree(p->textSK2);
                p->textSK2 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_SET_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_SETITEM_REQ
            case CSR_UI_MENU_SETITEM_REQ:
            {
                CsrUiMenuSetitemReq *p = message;
                CsrPmemFree(p->label);
                p->label = NULL;
                CsrPmemFree(p->sublabel);
                p->sublabel = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_MENU_SETITEM_REQ */
#ifndef EXCLUDE_CSR_UI_DIALOG_SET_REQ
            case CSR_UI_DIALOG_SET_REQ:
            {
                CsrUiDialogSetReq *p = message;
                CsrPmemFree(p->heading);
                p->heading = NULL;
                CsrPmemFree(p->message);
                p->message = NULL;
                CsrPmemFree(p->textSK1);
                p->textSK1 = NULL;
                CsrPmemFree(p->textSK2);
                p->textSK2 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_DIALOG_SET_REQ */
#ifndef EXCLUDE_CSR_UI_MENU_SET_REQ
            case CSR_UI_MENU_SET_REQ:
            {
                CsrUiMenuSetReq *p = message;
                CsrPmemFree(p->heading);
                p->heading = NULL;
                CsrPmemFree(p->textSK1);
                p->textSK1 = NULL;
                CsrPmemFree(p->textSK2);
                p->textSK2 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_MENU_SET_REQ */
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
#endif /* EXCLUDE_CSR_UI_MODULE */
