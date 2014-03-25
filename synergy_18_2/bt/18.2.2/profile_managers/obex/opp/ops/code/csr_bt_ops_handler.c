/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ops_handler.h"
#include "csr_bt_ops_sef.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_OPS_INST_ID      0x01

/* OPS jump table */
static const OpsStateHandlerType opsStateHandlers[CSR_BT_OPS_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtOpsActivateReqHandler,                         /* CSR_BT_OPS_ACTIVATE_REQ      */
    CsrBtOpsDeactivateReqHandler,                       /* CSR_BT_OPS_DEACTIVATE_REQ    */
    CsrBtOpsPutResHandler,                              /* CSR_BT_OPS_PUT_RES           */
    CsrBtOpsPutResHandler,                              /* CSR_BT_OPS_PUT_NEXT_RES == CSR_BT_OPS_PUT_RES*/
#ifdef CSR_BT_INSTALL_OPS_GET
    CsrBtOpsGetResHandler,                              /* CSR_BT_OPS_GET_RES           */
    CsrBtOpsGetNextResHandler,                          /* CSR_BT_OPS_GET_NEXT_RES      */
#else
    NULL,
    NULL,
#endif    
    CsrBtOpsConnectResHandler,                          /* CSR_BT_OPS_CONNECT_RES       */
    CsrBtOpsSecurityInReqHandler,                       /* CSR_BT_OPS_SECURITY_IN_REQ   */
#ifdef CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    CsrBtOpsAuthenticateResHandler,                     /* CSR_BT_OPS_AUTHENTICATE_RES  */
#else
    NULL,                                               /* CSR_BT_OPS_AUTHENTICATE_RES  */
#endif /* CSR_BT_OBEX_AUTH_OPS_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE */
};

/* ---------- End of jump tables ---------- */

void CsrBtOpsInit(void **gash)
{
    OpsInstanceDataType *pInst;

    *gash = CsrPmemZalloc(sizeof(OpsInstanceDataType));

    pInst = *gash;

    pInst->localServerCh = CSR_BT_NO_SERVER;
    pInst->obexInst      = ObexUtilInit(CSR_BT_OPS_IFACEQUEUE, pInst, CSR_BT_OPS_INST_ID);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_OBJECT_PUSH_MANDATORY_SECURITY_INCOMING,
        CSR_BT_OBEX_OBJECT_PUSH_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    pInst->appHandle        = CSR_SCHED_QID_INVALID;
#ifdef CSR_BT_INSTALL_OPS_GET
    pInst->getFinalFlag     = TRUE;
#endif
    pInst->supportedFormats = CSR_BT_ANY_TYPE_SUPPORT;
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
void CsrBtOpsDeinit(void **gash)
{
    CsrUint16           eventClass=0;
    void                *msg=NULL;
    CsrBool             lastMsg = FALSE;
    OpsInstanceDataType *pInst = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_OPS_PRIM:
                {
                    CsrBtOpsFreeDownstreamMessageContents(eventClass, msg);
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
                        CsrStateEventException("OPS", eventClass, 0, 0);
                        break;
                    }
#endif
            }
            CsrPmemFree(msg);
        }
    }

    ObexUtilDeinit(&pInst->obexInst);
    CsrPmemFree(pInst);
}
#endif

void CsrBtOpsHandler(void **gash)
{
    CsrUint16           eventClass=0;
    CsrPrim             *type;
    void                *msg   = NULL;
    OpsInstanceDataType *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    type = (CsrPrim *) msg;

    switch (eventClass)
    {
        case CSR_BT_OPS_PRIM:
        {
            if (*type < CSR_BT_OPS_PRIM_DOWNSTREAM_COUNT && (opsStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("OPS", eventClass, *type, 0);
#endif
                CsrBtOpsFreeDownstreamMessageContents(eventClass, msg);
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
                CsrStateEventException("OPS", eventClass, *type, 0);
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
            CsrStateEventException("OPS", eventClass, 0, 0);
            break;
        }
#endif
    }
    CsrPmemFree(msg);
}

