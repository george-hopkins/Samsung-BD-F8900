/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_app_lib.h"
#include "csr_pmem.h"
#include "csr_util.h"

/* --------------------------------------------------------------------
   Name
       CSR_APP_REGISTER_REQ

   Description
        Register with the CsrApp task

 * -------------------------------------------------------------------- */
CsrAppRegisterReq *CsrAppRegisterReq_struct(CsrSchedQid phandle, CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrUint16 menuIcon, CsrUint16 subMenuIcon, CsrBool top)
{
    CsrAppRegisterReq *prim;

    prim = (CsrAppRegisterReq *) CsrPmemAlloc(sizeof(CsrAppRegisterReq));
    prim->type = CSR_APP_REGISTER_REQ;
    prim->phandle = phandle;
    prim->menuName = menuName;
    prim->subMenuName = subMenuName;
    prim->menuIcon = menuIcon;
    prim->subMenuIcon = subMenuIcon;
    prim->top = top;

    return prim;
}

/* --------------------------------------------------------------------
   Name
       CSR_APP_BACKLOG_REQ

   Description
        Send a backlog entry to app task (this is a replacement for printf)
        NB: Use with care

 * -------------------------------------------------------------------- */
#define MAX_LOG_TEXT_LENGTH 255

void CsrAppBacklogReqSend2(CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrBool causePopup, CsrBool causeBacklog, const CsrCharString *fmt, ...)
{
    CsrAppBacklogReq *prim;
    CsrCharString logText[MAX_LOG_TEXT_LENGTH];
    va_list argptr;

    va_start(argptr, fmt);
    CsrVsnprintf(logText, MAX_LOG_TEXT_LENGTH, fmt, argptr);
    va_end(argptr);
    logText[MAX_LOG_TEXT_LENGTH - 1] = '\0';


    prim = (CsrAppBacklogReq *) CsrPmemAlloc(sizeof(CsrAppBacklogReq));
    prim->type = CSR_APP_BACKLOG_REQ;
    prim->menuName = menuName;
    prim->subMenuName = subMenuName;

    if (CsrStrLen(logText))
    {
        prim->logText = CsrPmemAlloc(CsrStrLen(logText) + 1);
        CsrStrCpy((CsrCharString *) prim->logText, logText);
    }
    else
    {
        prim->logText = NULL;
    }

    prim->causePopup = causePopup;
    prim->causeBacklog = causeBacklog;

    CsrMsgTransport(CSR_APP_IFACEQUEUE, CSR_APP_PRIM, prim);
}

void CsrAppBacklogReqSend(CsrUtf8String *menuName, CsrUtf8String *subMenuName, CsrBool causePopup, const CsrCharString *fmt, ...)
{
    CsrAppBacklogReq *prim;
    CsrCharString logText[MAX_LOG_TEXT_LENGTH];
    va_list argptr;

    va_start(argptr, fmt);
    CsrVsnprintf(logText, MAX_LOG_TEXT_LENGTH, fmt, argptr);
    va_end(argptr);
    logText[MAX_LOG_TEXT_LENGTH - 1] = '\0';

    prim = (CsrAppBacklogReq *) CsrPmemAlloc(sizeof(CsrAppBacklogReq));
    prim->type = CSR_APP_BACKLOG_REQ;
    prim->menuName = menuName;
    prim->subMenuName = subMenuName;

    if (CsrStrLen(logText))
    {
        prim->logText = CsrPmemAlloc(CsrStrLen(logText) + 1);
        CsrStrCpy((CsrCharString *) prim->logText, logText);
    }
    else
    {
        prim->logText = NULL;
    }

    prim->causePopup = causePopup;
    prim->causeBacklog = TRUE;

    CsrMsgTransport(CSR_APP_IFACEQUEUE, CSR_APP_PRIM, prim);
}
