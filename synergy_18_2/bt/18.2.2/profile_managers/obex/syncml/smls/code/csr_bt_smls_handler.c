/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_smls_handler.h"
#include "csr_bt_smls_sef.h"
#include "csr_bt_sc_private_lib.h"

#define CSR_BT_SMLS_INST_ID      0x01

static const SmlsStateHandlerType smlsStateHandlers[CSR_BT_SMLS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtSmlsActivateReqHandler,        /* CSR_BT_SMLS_ACTIVATE_REQ             */
    CsrBtSmlsDeactivateReqHandler,      /* CSR_BT_SMLS_DEACTIVATE_REQ           */
    CsrBtSmlsConnectResHandler,         /* CSR_BT_SMLS_CONNECT_RES              */
    CsrBtSmlsAuthenticateReqHandler,    /* CSR_BT_SMLS_AUTHENTICATE_REQ         */
    CsrBtSmlsAuthenticateResHandler,    /* CSR_BT_SMLS_AUTHENTICATE_RES         */
    CsrBtSmlsPutSmlMsgObjResHandler,    /* CSR_BT_SMLS_PUT_SMLMSG_OBJ_RES       */
    CsrBtSmlsPutSmlMsgObjNextResHandler,/* CSR_BT_SMLS_PUT_SMLMSG_OBJ_NEXT_RES  */
    CsrBtSmlsGetSmlMsgObjResHandler,    /* CSR_BT_SMLS_GET_SMLMSG_OBJ_RES       */
    CsrBtSmlsGetSmlMsgObjNextResHandler,/* CSR_BT_SMLS_GET_SMLMSG_OBJ_NEXT_RES  */
    CsrBtSmlsSecurityInReqHandler,      /* CSR_BT_SMLS_SECURITY_IN_REQ          */
};

void CsrBtSmlsInit(void **gash)
{
    SmlsInstanceDataType *pInst;

    *gash                = CsrPmemAlloc(sizeof(SmlsInstanceDataType));
    pInst                = *gash;
    pInst->localServerCh = CSR_BT_NO_SERVER;
    pInst->obexInst      = ObexUtilInit(CSR_BT_SMLS_IFACEQUEUE, pInst, CSR_BT_SMLS_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_SYNCML_TRANSFER_MANDATORY_SECURITY_INCOMING,
        CSR_BT_OBEX_SYNCML_TRANSFER_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

}

void CsrBtSmlsHandler(void **gash)
{
    CsrUint16            eventClass=0;
    CsrPrim              *type;
    void                 *msg   = NULL;
    SmlsInstanceDataType *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);
    type = (CsrPrim *) msg;

    switch (eventClass)
    {
        case CSR_BT_SMLS_PRIM:
        {
            if ((*type < CSR_BT_SMLS_PRIM_DOWNSTREAM_COUNT) &&
                (smlsStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR!                                             */
                CsrBtSmlsStateEventException(eventClass, *type);
                CsrBtSmlsFreeDownstreamMessageContents(CSR_BT_SMLS_PRIM, msg);
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
                CsrBtSmlsStateEventException(eventClass, *type);
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            break;
        }
        default:
        { /* Unknown event type!?!?                                             */
            CsrBtSmlsStateEventException(eventClass, *type);
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
void CsrBtSmlsDeinit(void **gash)
{
    CsrUint16            eventClass=0;
    void                 *msg=NULL;
    CsrBool              lastMsg = FALSE;
    SmlsInstanceDataType *pInst  = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_SMLS_PRIM:
                {
                    CsrBtSmlsFreeDownstreamMessageContents(CSR_BT_SMLS_PRIM, msg);
                    break;
                }
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                    break;
                }
                default:
                {
                    break;
                }
            }
            CsrPmemFree(msg);
        }
    }
    ObexUtilDeinit(&pInst->obexInst);
    CsrPmemFree(pInst);
}
#endif /* ENABLE_SHUTDOWN */

