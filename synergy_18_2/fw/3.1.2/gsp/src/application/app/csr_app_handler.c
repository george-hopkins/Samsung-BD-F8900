/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_ui_lib.h"
#include "csr_arg_search.h"
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"
#include "csr_app_ui_sef.h"
#include "csr_app_util.h"
#include "csr_app_lower_private.h"
#include "csr_app_ui_strings.h"
#include "csr_log_text_2.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrAppLto);

typedef void (*CsrAppEventType)(CsrAppInstData *instData);
static const CsrAppEventType csrUiMsgHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] = /* Jump tabel to handle Upstream CSR_UI messages send from the Phone Emulator UI (CSR_UI)     */
{
    CsrAppCsrUiUieCreateCfmHandler,             /* CSR_UI_UIE_CREATE_CFM                  */
    NULL,                                       /* CSR_UI_MENU_GET_CFM                    */
    CsrAppCsrUiMenuGetitemCfmHandler,           /* CSR_UI_MENU_GETITEM_CFM                */
    NULL,                                       /* CSR_UI_MENU_GETCURSOR_CFM              */
    NULL,                                       /* CSR_UI_EVENT_GET_CFM                   */
    NULL,                                       /* CSR_UI_DIALOG_GET_CFM                  */
    NULL,                                       /* CSR_UI_INPUTDIALOG_GET_CFM             */
    NULL,                                       /* CSR_UI_IDLESCREEN_GET_CFM              */
    NULL,                                       /* CSR_UI_DISPLAY_GETHANDLE_CFM           */
    NULL,                                       /* CSR_UI_KEYDOWN_IND                     */
    CsrAppCsrUiEventIndHandler,                 /* CSR_UI_EVENT_IND                       */
};

#define MAX_BACKLOG_ENTRIES 100

/* task functions */
void CsrAppInitialization(void **gash)
{
    CsrAppInstData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = (void *) inst;

    inst->maxBackLogEntries = MAX_BACKLOG_ENTRIES;

    CSR_LOG_TEXT_REGISTER(&CsrAppLto, "APP", 0, NULL);

    /* Start creation cycle */
    CsrUiUieCreateReqSend(CSR_APP_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
}

void CsrAppDeinitialization(void **gash)
{
    CsrAppInstData *inst;
    CsrAppProfileUie *prof, *prof_next;
    void *msg;
    CsrUint16 event;

    inst = *gash;

    for (prof = inst->profileHandleList;
         prof;
         prof = prof_next)
    {
        prof_next = prof->next;

        CsrPmemFree(prof->menuName);
        CsrPmemFree(prof->subMenuName);
        CsrPmemFree(prof);
    }

    while (CsrMessageQueuePop(&inst->saveQueue, &event, &msg) ||
           CsrSchedMessageGet(&event, &msg))
    {
        if (msg == NULL)
        {
            continue;
        }

        switch (event)
        {
            case CSR_APP_PRIM:
                CsrAppFreeDownstreamMessageContents(event, msg);
                break;

            case CSR_UI_PRIM:
                CsrUiFreeUpstreamMessageContents(event, msg);
                break;

            default:
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAppLto, 0, event, *((CsrPrim *) msg));
                break;
        }

        CsrPmemFree(msg);
    }

    CsrPmemFree(inst->recvMsgP);
    CsrPmemFree(inst);
}

void CsrAppHandler(void **gash)
{
    CsrAppInstData *inst;
    CsrUint16 event;

    inst = (CsrAppInstData *) *gash;

    /* Obtains a message from the CSR_APP_IFACEQUEUE                                         */
    if (!inst->restoreFlag) /* No messages on the local save queue                                                */
    {
        CsrSchedMessageGet(&event, &(inst->recvMsgP));
    }
    else
    {
        if (!CsrMessageQueuePop(&inst->saveQueue, &event, &inst->recvMsgP)) /* No more messages on the local save queue. Call CsrSchedMessageGet to receive
             a message from the scheduler                                                   */
        {
            inst->restoreFlag = FALSE;
            CsrSchedMessageGet(&event, &(inst->recvMsgP));
        }
        else /* A message has been restored from the local queed                               */
        {
        }
    }

    switch (event)
    {
        case CSR_APP_PRIM:
        {
            CsrUint16 *type;

            if (inst->recvMsgP == NULL)
            {
                break;
            }

            type = (CsrUint16 *) inst->recvMsgP;

            switch (*type)
            {
                case CSR_APP_REGISTER_REQ:
                {
                    if (inst->initialized)
                    {
                        CsrAppRegisterReq *prim = inst->recvMsgP;
                        CsrAppRegisterProfile(inst, prim->phandle, prim->menuName, prim->subMenuName, prim->menuIcon, prim->subMenuIcon, prim->top);
                        CsrPmemFree(prim->menuName);
                        CsrPmemFree(prim->subMenuName);
                    }
                    else
                    {
                        CsrMessageQueuePush(&inst->saveQueue, CSR_APP_PRIM, inst->recvMsgP);
                        inst->recvMsgP = NULL;
                    }
                    break;
                }
                case CSR_APP_BACKLOG_REQ:
                {
                    if (inst->initialized)
                    {
                        CsrAppBacklogReq *prim = inst->recvMsgP;
                        CsrUint16 textLength = 0;
                        CsrUtf8String *textString;

                        textLength = (CsrUint16) CsrStrLen((CsrCharString *) prim->menuName);
                        textLength += (CsrUint16) CsrStrLen((CsrCharString *) prim->subMenuName);

                        textString = CsrPmemAlloc(textLength + 2);

                        CsrStrNCpyZero((CsrCharString *) textString, (CsrCharString *) prim->menuName, textLength + 2);
                        if (prim->subMenuName[0] != '\0')
                        {
                            CsrStrCat((CsrCharString *) textString, " ");
                            CsrStrCat((CsrCharString *) textString, (const CsrCharString *) prim->subMenuName);
                        }

                        if (prim->causeBacklog)
                        {
                            CsrUiMenuAdditemReqSend(inst->hBacklogMenu,
                                CSR_UI_FIRST,
                                inst->backLogEntries,
                                CSR_UI_ICON_NONE,
                                CsrUtf82Utf16String(textString),
                                CsrUtf82Utf16String(prim->logText),
                                inst->hSelectEvent, CSR_UI_DEFAULTACTION,
                                inst->hBackEvent, CSR_UI_DEFAULTACTION);
                            CSR_LOG_TEXT_INFO((CsrAppLto, 0, "%s: %s", textString, prim->logText));

                            inst->backLogEntries++;

                            if (inst->backLogEntries >= inst->maxBackLogEntries)
                            {
                                CsrUiMenuRemoveitemReqSend(inst->hBacklogMenu, (CsrUint16) (inst->backLogEntries - inst->maxBackLogEntries));
                            }
                            if (inst->backLogEntries == 0x8000) /* CSR_UI limitation we just clear the entire history and start over again */
                            {
                                CsrUiMenuRemoveallitemsReqSend(inst->hBacklogMenu);
                                inst->backLogEntries = 0;
                            }
                        }

                        if (prim->causePopup)
                        {
                            CsrAppProfileUie *entry = CsrAppGetProfileUieFromMenuName(inst, prim->menuName, prim->subMenuName);

                            if (entry->popupDialogHandle != 0)
                            {
                                CsrUiDialogSetReqSend(entry->popupDialogHandle,
                                    CsrUtf82Utf16String(textString), CsrUtf82Utf16String(prim->logText),
                                    TEXT_OK_UTF16, NULL,
                                    inst->hBackEvent, CSR_UI_DEFAULTACTION,
                                    inst->hBackEvent, CSR_UI_DEFAULTACTION);
                                CsrUiUieShowReqSend(entry->popupDialogHandle, CSR_APP_IFACEQUEUE, CSR_UI_INPUTMODE_AUTO, 0);
                            }
                        }

                        CsrPmemFree(textString);
                        CsrPmemFree(prim->menuName);
                        CsrPmemFree(prim->subMenuName);
                        CsrPmemFree(prim->logText);
                    }
                    else
                    {
                        CsrMessageQueuePush(&inst->saveQueue, CSR_APP_PRIM, inst->recvMsgP);
                        inst->recvMsgP = NULL;
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case CSR_UI_PRIM:
        {
            CsrUiPrim *primType = (CsrUiPrim *) inst->recvMsgP;
            CsrUint16 index = (CsrUint16) (*primType - CSR_UI_PRIM_UPSTREAM_LOWEST);

            if ((index < CSR_UI_PRIM_UPSTREAM_COUNT) && (csrUiMsgHandlers[index] != NULL)) /* Call the state event function which handle this incoming CSR_UI event message*/
            {
                csrUiMsgHandlers[index](inst);
            }
            else /* An unexpected CSR_UI message is received                                     */
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrAppLto, 0, event, *primType);
            }
            break;
        }
    }

    CsrPmemFree(inst->recvMsgP);
    inst->recvMsgP = NULL;
}
