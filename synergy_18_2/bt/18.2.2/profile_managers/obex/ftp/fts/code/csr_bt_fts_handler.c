/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_fts_handler.h"
#include "csr_bt_fts_sef.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_FTS_INST_ID      0x01

static const FtsStateHandlerType ftsStateHandlers[CSR_BT_FTS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtFtsActivateReqHandler,             /* CSR_BT_FTS_ACTIVATE_REQ              */
    CsrBtFtsDeactivateReqHandler,           /* CSR_BT_FTS_DEACTIVATE_REQ            */
    CsrBtFtsConnectResHandler,              /* CSR_BT_FTS_CONNECT_RES               */
    CsrBtFtsAuthenticateReqHandler,         /* CSR_BT_FTS_AUTHENTICATE_REQ          */
    CsrBtFtsAuthenticateResHandler,         /* CSR_BT_FTS_AUTHENTICATE_RES          */
    CsrBtFtsPutObjResHandler,               /* CSR_BT_FTS_PUT_OBJ_RES               */
    CsrBtFtsPutObjNextResHandler,           /* CSR_BT_FTS_PUT_OBJ_NEXT_RES          */
    CsrBtFtsDelObjResHandler,               /* CSR_BT_FTS_DEL_OBJ_RES               */
    CsrBtFtsGetObjResHandler,               /* CSR_BT_FTS_GET_OBJ_RES               */
    CsrBtFtsGetObjNextResHandler,           /* CSR_BT_FTS_GET_OBJ_NEXT_RES          */
    CsrBtFtsGetListFolderResHandler,        /* CSR_BT_FTS_GET_LIST_FOLDER_RES       */
    CsrBtFtsGetListFolderNextResHandler,    /* CSR_BT_FTS_GET_LIST_FOLDER_NEXT_RES  */
    CsrBtFtsSetFolderResHandler,            /* CSR_BT_FTS_SET_FOLDER_RES            */
    CsrBtFtsSetBackFolderResHandler,        /* CSR_BT_FTS_SET_BACK_FOLDER_RES       */
    CsrBtFtsSetRootFolderResHandler,        /* CSR_BT_FTS_SET_ROOT_FOLDER_RES       */
    CsrBtFtsSetAddFolderResHandler,         /* CSR_BT_FTS_SET_ADD_FOLDER_RES        */
    CsrBtFtsSecurityInReqHandler,           /* CSR_BT_FTS_SECURITY_IN_REQ           */
#ifdef CSR_BT_INSTALL_OBEX_GOEP_20
    CsrBtFtsCopyObjResHandler,              /* CSR_BT_FTS_COPY_OBJ_RES              */
    CsrBtFtsMoveObjResHandler,              /* CSR_BT_FTS_MOVE_OBJ_RES              */
    CsrBtFtsSetObjPermissionsResHandler,    /* CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES   */
#else
    NULL,                                   /* CSR_BT_FTS_COPY_OBJ_RES              */
    NULL,                                   /* CSR_BT_FTS_MOVE_OBJ_RES              */
    NULL,                                   /* CSR_BT_FTS_SET_OBJ_PERMISSIONS_RES   */
#endif
};

void CsrBtFtsInit(void **gash)
{
    FtsInstanceDataType *pInst;

    *gash                = CsrPmemAlloc(sizeof(FtsInstanceDataType));
    pInst                = *gash;
    pInst->localServerCh = CSR_BT_NO_SERVER;
    pInst->localPsm      = L2CA_PSM_INVALID;
    pInst->name          = NULL;
    pInst->obexInst      = ObexUtilInit(CSR_BT_FTS_IFACEQUEUE, pInst, CSR_BT_FTS_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_FILE_TRANSFER_MANDATORY_SECURITY_INCOMING,
        CSR_BT_OBEX_FILE_TRANSFER_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
}

void CsrBtFtsHandler(void **gash)
{
    CsrUint16           eventClass=0;
    CsrPrim             *type;
    void                *msg   = NULL;
    FtsInstanceDataType *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    type = (CsrPrim *) msg;

    switch (eventClass)
    {
        case CSR_BT_FTS_PRIM:
        {
            if (*type < CSR_BT_FTS_PRIM_DOWNSTREAM_COUNT &&
                (ftsStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("FTS", eventClass, *type, 0);
#endif
                CsrBtFtsFreeDownstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_BT_CM_PRIM :
        {
            if (ObexUtilCmMsgHandler(pInst->obexInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* This message is handled by the common OBEX library             */
                ;
            }
            else
            { /* State/Event ERROR!                                             */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("FTS", eventClass, *type, 0);
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
        {
            /* Unknown event type!?!? */
            CsrStateEventException("FTS", eventClass, 0, 0);
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
void CsrBtFtsDeinit(void **gash)
{
    CsrUint16           eventClass=0;
    void                *msg;
    CsrBool             lastMsg = FALSE;
    FtsInstanceDataType *pInst  = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch(eventClass)
            {
                case CSR_BT_FTS_PRIM:
                {
                    CsrBtFtsFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("FTS" ,eventClass, 0, 0);
                    break;
                }
#endif
            }
            CsrPmemFree (msg);
        }
    }
    ObexUtilDeinit(&pInst->obexInst);
    CsrPmemFree(pInst->name);
    CsrPmemFree(pInst);
}
#endif




