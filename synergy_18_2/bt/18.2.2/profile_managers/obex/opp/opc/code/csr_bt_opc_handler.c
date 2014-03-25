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
#include "csr_bt_opc_handler.h"
#include "csr_bt_opc_sef.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_OPC_INST_ID      0x01

static const OpcStateHandlerType opcStateHandlers[CSR_BT_OPC_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtOpcConnectReqHandler,               /* CSR_BT_OPC_CONNECT_REQ          */
    CsrBtOpcAbortReqHandler,                 /* CSR_BT_OPC_ABORT_REQ            */
    CsrBtOpcDisconnectReqHandler,            /* CSR_BT_OPC_DISCONNECT_REQ       */
    CsrBtOpcPutHeaderReqHandler,             /* CSR_BT_OPC_PUT_REQ              */
    CsrBtOpcPutBodyReqHandler,               /* CSR_BT_OPC_PUT_OBJECT_RES       */
#ifdef CSR_BT_INSTALL_OPC_GET
    CsrBtOpcGetHeaderReqHandler,             /* CSR_BT_OPC_GET_HEADER_REQ       */
    CsrBtOpcGetObjReqHandler,                /* CSR_BT_OPC_GET_OBJECT_REQ       */
#else
    NULL,
    NULL,
#endif    
    CsrBtOpcCancelConnectReqHandler,         /* CSR_BT_OPC_CANCEL_CONNECT_REQ   */
    CsrBtOpcSecurityOutReqHandler,           /* CSR_BT_OPC_SECURITY_OUT_REQ     */
#ifdef CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE
    CsrBtOpcAuthenticateResHandler,          /* CSR_BT_OPC_AUTHENTICATE_RES     */
#else
    NULL,                                    /* CSR_BT_OPC_AUTHENTICATE_RES     */
#endif /* CSR_BT_OBEX_AUTH_OPC_NON_SPEC_COMPLIANT_TEST_DO_NOT_USE               */
};

void CsrBtOpcInit(void **gash)
{
    OpcInstanceDataType *pInst;
    *gash                   = CsrPmemAlloc(sizeof(OpcInstanceDataType));
    pInst                   = *gash;
    pInst->obexInst         = ObexUtilInit(CSR_BT_OPC_IFACEQUEUE, pInst, CSR_BT_OPC_INST_ID);

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_OBEX_OBJECT_PUSH_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_OBEX_OBJECT_PUSH_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_OBEX_OBJECT_PUSH_SERVICE_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                                  */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_OBJECT_PUSH_SERVICE_UUID);
}

void CsrBtOpcHandler(void **gash)
{
    CsrUint16 eventClass=0;
    void *msg = NULL;
    OpcInstanceDataType *pInst = *gash;
    CsrBtOpcPrim *type;

    if(CsrSchedMessageGet(&eventClass , &msg))
    {
        type = (CsrBtOpcPrim*) msg;
        switch(eventClass)
        {
            case CSR_BT_OPC_PRIM:
                {
                    if (*type < CSR_BT_OPC_PRIM_DOWNSTREAM_COUNT &&
                        (opcStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
                    {
                        ;
                    }
                    else
                    {/* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("OPC", eventClass, *type, 0);
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                        CsrBtOpcFreeDownstreamMessageContents(CSR_BT_OPC_PRIM, msg);
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
                        CsrPrim *type = msg;
                        CsrStateEventException("OPC", eventClass, *type, 0);
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
            default: /* Unknown event type!?!? */
                {
                    CsrStateEventException("OPC", eventClass, 0, 0);
                    break;
                }
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
        }
        CsrPmemFree(msg);
    }
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
void CsrBtOpcDeinit(void **gash)
{
    CsrUint16            eventClass=0;
    void                *msg=NULL;
    CsrBool              lastMsg = FALSE;
    OpcInstanceDataType *pInst  = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_OPC_PRIM:
                {
                    CsrBtOpcFreeDownstreamMessageContents(eventClass, msg);
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
                    CsrStateEventException("OPC",eventClass, 0, 0);
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
#endif
