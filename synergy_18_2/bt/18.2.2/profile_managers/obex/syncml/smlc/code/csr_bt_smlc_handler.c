/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_smlc_handler.h"
#include "csr_bt_smlc_sef.h"

#define CSR_BT_SMLC_CLI_INST_ID 0x01

static const SmlcStateHandlerType csrBtSmlcDownStreamHandlers[CSR_BT_SMLC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtSmlcActivateReqHandler,                /* CSR_BT_SMLC_ACTIVATE_REQ                     */
    CsrBtSmlcDeactivateReqHandler,              /* CSR_BT_SMLC_DEACTIVATE_REQ                   */
    CsrBtSmlcConnectReqHandler,                 /* CSR_BT_SMLC_CONNECT_REQ                      */
    CsrBtSmlcAuthenticateResHandler,            /* CSR_BT_SMLC_AUTHENTICATE_RES                 */
    CsrBtSmlcGetSmlMsgObjReqHandler,            /* CSR_BT_SMLC_GET_SML_MSG_OBJ_REQ              */
    CsrBtSmlcGetNextChunkSmlMsgObjReqHandler,   /* CSR_BT_SMLC_GET_NEXT_CHUNK_SMLMSG_OBJ_REQ    */
    CsrBtSmlcPutSmlMsgObjReqHandler,            /* CSR_BT_SMLC_PUT_SML_MSG_OBJ_REQ              */
    CsrBtSmlcPutNextChunkSmlMsgObjReqHandler,   /* CSR_BT_SMLC_PUT_NEXT_CHUNK_SMLMSG_OBJ_REQ    */
    CsrBtSmlcAbortReqHandler,                   /* CSR_BT_SMLC_ABORT_REQ                        */
    CsrBtSmlcDisconnectReqHandler,              /* CSR_BT_SMLC_DISCONNECT_REQ                   */
    CsrBtSmlcCancelConnectReqHandler,           /* CSR_BT_SMLC_CANCEL_CONNECT_REQ               */
    CsrBtSmlcSecurityInReqHandler,              /* CSR_BT_SMLC_SECURITY_IN_REQ                  */
    CsrBtSmlcSecurityOutReqHandler,             /* CSR_BT_SMLC_SECURITY_OUT_REQ                 */
};

void CsrBtSmlcInit(void **gash)
{

    SmlcInstanceData *pInst;

    *gash               = (SmlcInstanceData *) CsrPmemAlloc(sizeof(SmlcInstanceData));
    pInst               = *gash;
    pInst->obexCliInst  = ObexUtilInit(CSR_BT_SMLC_IFACEQUEUE, pInst, CSR_BT_SMLC_CLI_INST_ID);
    pInst->body         = NULL;
    pInst->appHandle    = CSR_SCHED_QID_INVALID;
    pInst->activated    = FALSE;
    pInst->cliBusy      = FALSE;

    CsrBtScSetSecOutLevel(&pInst->secOutgoing,
                          CSR_BT_SEC_DEFAULT,
                          CSR_BT_OBEX_SYNCML_TRANSFER_MANDATORY_SECURITY_OUTGOING,
                          CSR_BT_OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_OUTGOING,
                          CSR_BT_RESULT_CODE_OBEX_SUCCESS,
                          CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);
}

void CsrBtSmlcHandler(void **gash)
{
    CsrPrim             *type;
    CsrUint16           eventClass=0;
    void                *msg   = NULL;
    SmlcInstanceData    *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);
    type = (CsrPrim *) msg;

    switch(eventClass)
    {
        case CSR_BT_SMLC_PRIM:
        {
            if ((*type < CSR_BT_SMLC_PRIM_DOWNSTREAM_COUNT) &&
                (csrBtSmlcDownStreamHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            {
                CsrBtSmlcStateEventException(eventClass, *type);
                CsrBtSmlcFreeDownstreamMessageContents(eventClass, msg);
            }

            break;
        }
        case CSR_BT_CM_PRIM :
        {
            if (ObexUtilCmMsgHandler(pInst->obexCliInst, &msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION)
            { /* This message is handled by the common OBEX library */
                ;
            }
            else
            {
                CsrBtSmlcStateEventException(eventClass, *type);
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        {
            CsrBtSmlcStateEventException(eventClass, 0);
            break;
        }
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
void CsrBtSmlcDeinit(void ** gash)
{
    CsrUint16        eventClass=0;
    void             *msg=NULL;
    CsrBool          lastMsg     = FALSE;
    SmlcInstanceData *pInst      = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)!CsrSchedMessageGet(&eventClass, &msg);

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_SMLC_PRIM:
                {
                    CsrBtSmlcFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                }
                default:
                {
                    CsrBtSmlcStateEventException(eventClass, 0);
                    break;
                }
            }
            CsrPmemFree (msg);
        }
    }

    ObexUtilDeinit(&pInst->obexCliInst);
    CsrPmemFree(pInst->body);
    CsrPmemFree(pInst);
}
#endif    /* ENABLE_SHUTDOWN */

