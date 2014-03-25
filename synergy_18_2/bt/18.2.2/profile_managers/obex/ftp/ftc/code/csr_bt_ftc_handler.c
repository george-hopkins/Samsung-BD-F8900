/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_bt_result.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#include "csr_bt_ftc_handler.h"
#include "csr_bt_ftc_sef.h"


#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_FTC_INST_ID      0x01

static const FtcStateHandlerType ftcStateHandlers[CSR_BT_FTC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtFtcConnectReqHandler,                  /* CSR_BT_FTC_CONNECT_REQ              */
    CsrBtFtcAuthenticateResHandler,             /* CSR_BT_FTC_AUTHENTICATE_RES         */
    CsrBtFtcGetListFolderReqHandler,            /* CSR_BT_FTC_GET_LIST_FOLDER_REQ      */
    CsrBtFtcGetListFolderBodyReqHandler,        /* CSR_BT_FTC_GET_LIST_FOLDER_BODY_REQ */
    CsrBtFtcGetObjReqHandler,                   /* CSR_BT_FTC_GET_OBJ_REQ              */
    CsrBtFtcGetObjBodyReqHandler,               /* CSR_BT_FTC_GET_OBJ_BODY_REQ         */
    CsrBtFtcPutObjHeaderReqHandler,             /* CSR_BT_FTC_PUT_OBJ_HEADER_REQ       */
    CsrBtFtcPutObjBodyReqHandler,               /* CSR_BT_FTC_PUT_OBJ_BODY_REQ         */
    CsrBtFtcDelObjReqHandler,                   /* CSR_BT_FTC_DEL_OBJ_REQ              */
    CsrBtFtcSetFolderReqHandler,                /* CSR_BT_FTC_SET_FOLDER_REQ           */
    CsrBtFtcSetBackFolderReqHandler,            /* CSR_BT_FTC_SET_BACK_FOLDER_REQ      */
    CsrBtFtcSetRootFolderReqHandler,            /* CSR_BT_FTC_SET_ROOT_FOLDER_REQ      */
    CsrBtFtcSetAddFolderReqHandler,             /* CSR_BT_FTC_SET_ADD_FOLDER_REQ       */
    CsrBtFtcAbortReqHandler,                    /* CSR_BT_FTC_ABORT_REQ                */
    CsrBtFtcDisconnectReqHandler,               /* CSR_BT_FTC_DISCONNECT_REQ           */
    CsrBtFtcCancelConnectReqHandler,            /* CSR_BT_FTC_CANCEL_CONNECT_REQ       */
    CsrBtFtcSecurityOutReqHandler,              /* CSR_BT_FTC_SECURITY_OUT_REQ         */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrBtFtcCopyingObjReqHandler,               /* CSR_BT_FTC_COPYING_OBJ_REQ          */
    CsrBtFtcMovingObjReqHandler,                /* CSR_BT_FTC_MOVING_OBJ_REQ           */
    CsrBtFtcSetObjPermissionsReqHandler,        /* CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ  */
#else
    NULL,                                       /* CSR_BT_FTC_COPYING_OBJ_REQ          */
    NULL,                                       /* CSR_BT_FTC_MOVING_OBJ_REQ           */
    NULL,                                       /* CSR_BT_FTC_SET_OBJ_PERMISSIONS_REQ  */
#endif
};

void CsrBtFtcInit(void **gash)
{
    FtcInstanceData *pInst;

    *gash                           = (FtcInstanceData *) CsrPmemAlloc(sizeof(FtcInstanceData));
    pInst                           = *gash;
    pInst->obexInst                 = ObexUtilInit(CSR_BT_FTC_IFACEQUEUE, pInst, CSR_BT_FTC_INST_ID);

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_FILE_TRANSFER_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_FILE_TRANSFER_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_OBEX_FILE_TRANSFER_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                          */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_FILE_TRANSFER_UUID);
}

void CsrBtFtcHandler(void **gash)
{
    CsrUint16       eventClass=0;
    CsrPrim         *type;
    void            *msg    = NULL;
    FtcInstanceData *pInst  = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    type = (CsrPrim *) msg;

    switch(eventClass)
    {
        case CSR_BT_FTC_PRIM:
        {
            if (*type < CSR_BT_FTC_PRIM_DOWNSTREAM_COUNT &&
                (ftcStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }

            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrStateEventException("FTC", eventClass, *type, 0);
#endif
                CsrBtFtcFreeDownstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            if (ObexUtilCmMsgHandler(pInst->obexInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* This message is handled by the common OBEX library */
                ;
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                /* State/Event ERROR! */
                CsrStateEventException("FTC", eventClass, *type, 0);
#endif
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
        {/* Unknown event type!?!? */
            CsrStateEventException("FTC", eventClass, 0, 0);
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
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtFtcDeinit(void ** gash)
{
    CsrUint16        eventClass=0;
    void            *msg=NULL;
    CsrBool          lastMsg     = FALSE;
    FtcInstanceData *pInst      = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_FTC_PRIM:
                {
                    CsrBtFtcFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("FTC", eventClass, 0, 0);
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
#endif    /* ENABLE_SHUTDOWN */



