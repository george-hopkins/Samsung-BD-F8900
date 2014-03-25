/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_pas_handler.h"
#include "csr_bt_pas_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define CSR_BT_PAS_INST_ID  0x01

static const PasStateHandler pasStateHandlers[CSR_BT_PAS_PRIM_DOWNSTREAM_COUNT] =
{
        CsrBtPasActivateReqHandler,                 /* CSR_BT_PAS_ACTIVATE_REQ */
        CsrBtPasDeactivateReqHandler,               /* CSR_BT_PAS_DEACTIVATE_REQ */
        CsrBtPasConnectResHandler,                  /* CSR_BT_PAS_CONNECT_RES */
        CsrBtPasAuthenticateReqHandler,             /* CSR_BT_PAS_AUTHENTICATE_REQ */
        CsrBtPasAuthenticateResHandler,             /* CSR_BT_PAS_AUTHENTICATE_RES */
        CsrBtPasPullPbResHandler,                   /* CSR_BT_PAS_PULL_PB_RES */
        CsrBtPasPullPbSizeResHandler,               /* CSR_BT_PAS_PULL_PB_SIZE_RES */
        CsrBtPasPullMchResHandler,                  /* CSR_BT_PAS_PULL_MCH_RES */
        CsrBtPasPullMchSizeResHandler,              /* CSR_BT_PAS_PULL_MCH_SIZE_RES */
        CsrBtPasSetFolderResHandler,                /* CSR_BT_PAS_SET_FOLDER_RES */
        CsrBtPasPullvCardListResHandler,            /* CSR_BT_PAS_PULL_VCARD_LIST_RES */
        CsrBtPasPullvCardListSizeResHandler,        /* CSR_BT_PAS_PULL_VCARD_LIST_SIZE_RES */
        CsrBtPasPullvCardMchListResHandler,         /* CSR_BT_PAS_PULL_VCARD_MCH_LIST_RES */
        CsrBtPasPullvCardMchListSizeResHandler,     /* CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_RES */
        CsrBtPasPullvCardEntryResHandler,           /* CSR_BT_PAS_PULL_VCARD_ENTRY_RES */
        CsrBtPasNextResHandler,                     /* CSR_BT_PAS_NEXT_RES */
        CsrBtPasSecurityInReqHandler,               /* CSR_BT_PAS_SECURITY_IN_REQ */
};

/* UFT-16 Big endian */
static const CsrUint8 wzPb[] = {0, 'p', 0, 'b', 0, 0};
static const CsrUint8 wzIch[] = {0, 'i', 0, 'c', 0, 'h', 0, 0};
static const CsrUint8 wzOch[] = {0, 'o', 0, 'c', 0, 'h', 0, 0};
static const CsrUint8 wzMch[] = {0, 'm', 0, 'c', 0, 'h', 0, 0};
static const CsrUint8 wzCch[] = {0, 'c', 0, 'c', 0, 'h', 0, 0};
static const CsrUint8 wzTelecom[] = {0, 't', 0, 'e', 0, 'l', 0, 'e', 0, 'c', 0, 'o', 0, 'm', 0, 0};

static PasFolder pb;
static PasFolder ich;
static PasFolder och;
static PasFolder mch;
static PasFolder cch;
static PasFolder telecom;

static PasFolder root = {
    PAS_ROOT_ID,
    NULL,
    NULL,
    (PasFolder*)&telecom,
    NULL
};
static PasFolder telecom = {
    PAS_TELECOM_ID,
    (CsrUint8*)wzTelecom,
    (PasFolder*)&root,
    (PasFolder*)&pb,
    NULL
};
static PasFolder pb = {
    PAS_PB_ID,
    (CsrUint8*)wzPb,
    (PasFolder*)&telecom,
    NULL,
    (PasFolder*)&ich
};
static PasFolder ich = {
    PAS_ICH_ID,
    (CsrUint8*)wzIch,
    (PasFolder*)&telecom,
    NULL,
    (PasFolder*)&och
};
static PasFolder och = {
    PAS_OCH_ID,
    (CsrUint8*)wzOch,
    (PasFolder*)&telecom,
    NULL,
    (PasFolder*)&mch
};
static PasFolder mch = {
    PAS_MCH_ID,
    (CsrUint8*)wzMch,
    (PasFolder*)&telecom,
    NULL,
    (PasFolder*)&cch
};
static PasFolder cch = {
    PAS_CCH_ID,
    (CsrUint8*)wzCch,
    (PasFolder*)&telecom,
    NULL,
    NULL
};


void CsrBtPasInit(void **gash)
{
    PasInst *pInst;

    pInst                = *gash = CsrPmemAlloc(sizeof(PasInst));
    pInst->localServerCh = CSR_BT_NO_SERVER;
    pInst->obexInst      = ObexUtilInit(CSR_BT_PAS_IFACEQUEUE, pInst, CSR_BT_PAS_INST_ID);
    pInst->pFolder       = (PasFolder*)&root;

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_PBAP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_PBAP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_OBEX_SUCCESS,
        CSR_BT_RESULT_CODE_OBEX_UNACCEPTABLE_PARAMETER);

    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_OBEX_PBA_CLIENT_PROFILE_UUID);
}

void CsrBtPasHandler(void **gash)
{
    CsrUint16           eventClass=0;
    CsrPrim             *type;
    void                *msg   = NULL;
    PasInst *pInst             = *gash;

    CsrSchedMessageGet(&eventClass, &msg);

    type = (CsrPrim *) msg;

    switch (eventClass)
    {
        case CSR_BT_PAS_PRIM:
        {
            if (*type < CSR_BT_PAS_PRIM_DOWNSTREAM_COUNT &&
                (pasStateHandlers[*type](pInst, msg) != CSR_BT_OBEX_UTIL_STATUS_EXCEPTION))
            {
                ;
            }
            else
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("PAS", eventClass, *type, 0);
#endif
                CsrBtPasFreeDownstreamMessageContents(eventClass, msg);

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
                CsrStateEventException("PAS", eventClass, *type, 0);
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
            CsrStateEventException("PAS", eventClass, 0, 0);
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
void CsrBtPasDeinit(void **gash)
{
    CsrUint16   eventClass=0;
    void        *msg=NULL;
    CsrBool     lastMsg = FALSE;
    PasInst     *pInst  = *gash;

    while (!lastMsg)
    {
        lastMsg = (CsrBool)(!CsrSchedMessageGet(&eventClass, &msg));

        if (!lastMsg)
        {
            switch (eventClass)
            {
                case CSR_BT_PAS_PRIM:
                {
                    CsrBtPasFreeDownstreamMessageContents(eventClass, msg);
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
            CsrPmemFree(msg);
        }
    }
    ObexUtilDeinit(&pInst->obexInst);
    CsrPmemFree(pInst);
}
#endif
