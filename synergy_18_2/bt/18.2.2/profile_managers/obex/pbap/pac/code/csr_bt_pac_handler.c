/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_pac_handler.h"
#include "csr_bt_pac_sef.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_PAC_INST_ID      0x01

static const PacStateHandlerType pacStateHandlers[CSR_BT_PAC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtPacConnectReqHandler,                       /* CSR_BT_PAC_CONNECT_REQ          */
    CsrBtPacAuthenticateResHandler,                  /* CSR_BT_PAC_AUTHENTICATE_RES     */
    CsrBtPacPullPbReqHandler,                        /* CSR_BT_PAC_PULL_PB_REQ          */
    CsrBtPacPullPbResHandler,                        /* CSR_BT_PAC_PULL_PB_RES          */
    CsrBtPacSetFolderReqHandler,                     /* CSR_BT_PAC_SET_FOLDER_REQ       */
    CsrBtPacSetBackFolderReqHandler,                 /* CSR_BT_PAC_SET_BACK_FOLDER_REQ  */
    CsrBtPacSetRootFolderReqHandler,                 /* CSR_BT_PAC_SET_ROOT_FOLDER_REQ  */
    CsrBtPacPullvCardListReqHandler,                 /* CSR_BT_PAC_PULL_VCARD_LIST_REQ  */
    CsrBtPacPullvCardListResHandler,                 /* CSR_BT_PAC_PULL_VCARD_LIST_RES  */
    CsrBtPacPullvCardEntryReqHandler,                /* CSR_BT_PAC_PULL_VCARD_ENTRY_REQ */
    CsrBtPacPullvCardEntryResHandler,                /* CSR_BT_PAC_PULL_VCARD_ENTRY_RES */
    CsrBtPacAbortReqHandler,                         /* CSR_BT_PAC_ABORT_REQ            */
    CsrBtPacDisconnectReqHandler,                    /* CSR_BT_PAC_DISCONNECT_REQ       */
    CsrBtPacCancelConnectReqHandler,                 /* CSR_BT_PAC_CANCEL_CONNECT_REQ   */
    CsrBtPacSecurityOutReqHandler,                   /* CSR_BT_PAC_SECURITY_OUT_REQ     */
};

void CsrBtPacInit(void **gash)
{
    PacInst *pInst;

    *gash           = CsrPmemAlloc(sizeof(PacInst));
    pInst           = *gash;
    pInst->obexInst = ObexUtilInit(CSR_BT_PAC_IFACEQUEUE, pInst, CSR_BT_PAC_INST_ID);

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_PBAP_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_PBAP_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Register the PAC own serviceRecord                                       */
    CsrBtPacCmSdsRegisterReqHandler(pInst);

    /* Tell the SD that it must look for the CSR_BT_OBEX_PBA_SERVER_PROFILE_UUID
       service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ                 */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_PBA_SERVER_PROFILE_UUID);
}

void CsrBtPacHandler(void **gash)
{
    CsrUint16    eventClass=0;
    void        *msg   = NULL;
    PacInst     *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    switch(eventClass)
    {
        case CSR_BT_PAC_PRIM:
        {
            CsrBtPacPrim *type = msg;

            if (*type < CSR_BT_PAC_PRIM_DOWNSTREAM_COUNT &&
                (pacStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            else
            {/* State/Event ERROR! */
                CsrGeneralException("PAC", 0, *type, "unhandled CSR_BT_PAC_PRIM");
                CsrBtPacFreeDownstreamMessageContents(eventClass, msg);
            }
#endif
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            CsrPrim *type = msg;

            if (*type == CSR_BT_CM_SDS_REGISTER_CFM)
            {
                CsrBtPacCmSdsRegistertCfmHandler(pInst, msg);
            }
            else if (ObexUtilCmMsgHandler(pInst->obexInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* This message is handled by the common OBEX library */
                ;
            }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            else
            {
                /* State/Event ERROR! */
                CsrPrim *type = msg;
                CsrGeneralException("PAC", 0, *type, "Unhandled CSR_BT_CM_PRIM");
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
#endif
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
        {/* Unknown event type!?!? */
            CsrGeneralException("PAC", eventClass, 0, 0);
            break;
        }
#endif
    }
    CsrPmemFree(msg);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)  empty the input pInstmessage queue and free any allocated pInstmemory in the
        pInstmessages.
    2)  free any instance data that may be allocated.
****************************************************************************/
void CsrBtPacDeinit(void **gash)
{
    CsrUint16        eventClass=0;
    void            *msg=NULL;
    CsrBool          lastMsg = FALSE;
    PacInst *pInst          = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_PAC_PRIM:
                {
                    CsrBtPacFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                default:
                {
                    CsrStateEventException("PAC",eventClass, 0, 0);
                    break;
                }
#endif
            }
            CsrPmemFree (msg);
        }
    }
    ObexUtilDeinit(&pInst->obexInst);
    CsrPmemFree(pInst);
}
#endif /* ENABLE_SHUTDOWN       */


