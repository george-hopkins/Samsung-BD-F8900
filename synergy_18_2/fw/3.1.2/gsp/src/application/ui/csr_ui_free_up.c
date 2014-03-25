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

void CsrUiFreeUpstreamMessageContents(CsrUint16 eventClass, void *message)
{
    if (eventClass == CSR_UI_PRIM)
    {
        CsrUiPrim *prim = (CsrUiPrim *) message;
        switch (*prim)
        {
#ifndef EXCLUDE_CSR_UI_MENU_GET_CFM
            case CSR_UI_MENU_GET_CFM:
            {
                CsrUiMenuGetCfm *p = message;
                CsrPmemFree(p->heading);
                p->heading = NULL;
                CsrPmemFree(p->textSK1);
                p->textSK1 = NULL;
                CsrPmemFree(p->textSK2);
                p->textSK2 = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_MENU_GET_CFM */
#ifndef EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM
            case CSR_UI_IDLESCREEN_GET_CFM:
            {
                CsrUiIdlescreenGetCfm *p = message;
                CsrPmemFree(p->text);
                p->text = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_IDLESCREEN_GET_CFM */
#ifndef EXCLUDE_CSR_UI_MENU_GETITEM_CFM
            case CSR_UI_MENU_GETITEM_CFM:
            {
                CsrUiMenuGetitemCfm *p = message;
                CsrPmemFree(p->label);
                p->label = NULL;
                CsrPmemFree(p->sublabel);
                p->sublabel = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_MENU_GETITEM_CFM */
#ifndef EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM
            case CSR_UI_INPUTDIALOG_GET_CFM:
            {
                CsrUiInputdialogGetCfm *p = message;
                CsrPmemFree(p->text);
                p->text = NULL;
                break;
            }
#endif /* EXCLUDE_CSR_UI_INPUTDIALOG_GET_CFM */
#ifndef EXCLUDE_CSR_UI_DIALOG_GET_CFM
            case CSR_UI_DIALOG_GET_CFM:
            {
                CsrUiDialogGetCfm *p = message;
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
#endif /* EXCLUDE_CSR_UI_DIALOG_GET_CFM */
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
