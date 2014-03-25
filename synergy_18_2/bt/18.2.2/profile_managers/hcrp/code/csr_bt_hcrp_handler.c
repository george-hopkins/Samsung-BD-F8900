/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "sds_prim.h"
#include "csr_bt_hcrp_prim.h"
#include "csr_bt_hcrp_handler.h"
#include "csr_bt_hcrp_sef.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static const hcrpStateHandler hcrpStateHandlers[HCRP_NUMBER_OF_STATES][CSR_BT_HCRP_PRIM_DOWNSTREAM_COUNT] =
{
    /* HCRP_IDLE_STATE */
    {
        CsrBtHcrpActivateReqHandler,                /* CSR_BT_HCRP_ACTIVATE_REQ */
        CsrBtHcrpIgnoreMessageHandler,              /* CSR_BT_HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
        NULL,                                       /* CSR_BT_HCRP_GET_LPT_STATUS_RES */
        NULL,                                       /* CSR_BT_HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* CSR_BT_HCRP_SECURITY_IN_REQ */
    },
    /* HCRP_INITIALISING_STATE */
    {
        NULL,                                       /* CSR_BT_HCRP_ACTIVATE_REQ */
        CsrBtHcrpSaveMessageHandler,                /* CSR_BT_HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
        CsrBtHcrpGetLptStatusResHandler,            /* CSR_BT_HCRP_GET_LPT_STATUS_RES */
        CsrBtHcrpGetIeee1284ResHandler,             /* CSR_BT_HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* CSR_BT_HCRP_SECURITY_IN_REQ */
    },
    /* HCRP_READY_STATE */
    {
        NULL,                                       /* CSR_BT_HCRP_ACTIVATE_REQ */
        CsrBtHcrpDeactiveReqHandler,                /* CSR_BT_HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
        CsrBtHcrpGetLptStatusResHandler,            /* CSR_BT_HCRP_GET_LPT_STATUS_RES */
        CsrBtHcrpGetIeee1284ResHandler,             /* CSR_BT_HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* CSR_BT_HCRP_SECURITY_IN_REQ */
    },
    /* HCRP_CONNECTING_STATE */
    {
        NULL,                                       /* CSR_BT_HCRP_ACTIVATE_REQ */
        CsrBtHcrpSaveMessageHandler,                /* CSR_BT_HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
        CsrBtHcrpGetLptStatusResHandler,            /* CSR_BT_HCRP_GET_LPT_STATUS_RES */
        CsrBtHcrpGetIeee1284ResHandler,             /* CSR_BT_HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* CSR_BT_HCRP_SECURITY_IN_REQ */
    },
    /* HCRP_CONNECTED_STATE */
    {
        NULL,                                       /* CSR_BT_HCRP_ACTIVATE_REQ */
        CsrBtHcrpSaveMessageHandler,                /* CSR_BT_HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */
        CsrBtHcrpGetLptStatusResHandler,            /* CSR_BT_HCRP_GET_LPT_STATUS_RES */
        CsrBtHcrpGetIeee1284ResHandler,             /* CSR_BT_HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* CSR_BT_HCRP_SECURITY_IN_REQ */
    },
    /* HCRP_DEACTIVATING_STATE */
    {
        CsrBtHcrpSaveMessageHandler,                /* HCRP_ACTIVATE_REQ */
        NULL,                                       /* HCRP_DEACTIVATE_REQ */
        CsrBtHcrpRegisterDataPathReqHandler,        /* HCRP_REGISTER_DATA_PATH_REQ */
        NULL,                                       /* HCRP_GET_LPT_STATUS_RES */
        NULL,                                       /* HCRP_GET_IEEE_1284_RES */
        CsrBtHcrpSecurityInReqHandler,              /* HCRP_SECURITY_IN_REQ */
    }
};

/* Initialize */
void CsrBtHcrpInit(void **gash)
{
    hcrpInstanceData_t *instData;

    instData                    = CsrPmemZalloc(sizeof(hcrpInstanceData_t));
    instData->ctrlHandle        = CSR_SCHED_QID_INVALID;
    instData->dataHandle        = CSR_SCHED_QID_INVALID;

    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_HCRP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HCRP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HCRP_SUCCESS,
        CSR_BT_RESULT_CODE_HCRP_UNACCEPTABLE_PARAMETER);

    *gash = instData;
}

/* Task handler */
void CsrBtHcrpHandler(void **gash)
{
    hcrpInstanceData_t *instData;
    instData = (hcrpInstanceData_t*)*gash;

    if(!instData->restoreHcrpFlag)
    {
        CsrSchedMessageGet(&instData->event, &instData->pRecvMsg);
    }
    else
    {
        if (!CsrMessageQueuePop(&instData->pSaveQueue, &instData->event, &instData->pRecvMsg))
        {
            instData->pSaveQueue = NULL;
            CsrSchedMessageGet(&instData->event, &instData->pRecvMsg);
        }
    }

    switch (instData->event)
    {
        case CSR_BT_HCRP_PRIM:
            {
                CsrBtHcrpPrim* pType = instData->pRecvMsg;

                if ((*pType < CSR_BT_HCRP_PRIM_DOWNSTREAM_COUNT) && hcrpStateHandlers[instData->state][*pType] != NULL)
                {
                    hcrpStateHandlers[instData->state][*pType](instData);
                }
                else if (*pType != CSR_BT_HCRP_HOUSE_CLEANING)
                {
                    CsrBtHcrpFreeDownstreamMessageContents(instData->event, instData->pRecvMsg);
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    /* state/event error! */
                    CsrGeneralException("HCRP",
                     instData->state,
                     instData->event,
                     "CSR_BT_HCRP_PRIM");
#endif
                }

                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrPrim* primType = instData->pRecvMsg;

            switch (*primType)
            {
                case CSR_BT_CM_L2CA_REGISTER_CFM:
                    CsrBtHcrpCmL2caRegisterCfm(instData);
                    break;

                case CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM:
                    CsrBtHcrpcmL2caConnectAcceptCfm(instData);
                    break;

                case CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM:
                    CsrBtHcrpcmL2caCancelConnectAcceptCfm(instData);
                    break;

                case CSR_BT_CM_L2CA_DISCONNECT_IND:
                    CsrBtHcrpCmL2CaDisconnectInd(instData);
                    break;

                case CSR_BT_CM_L2CA_DATA_CFM:
                    break;

                case CSR_BT_CM_L2CA_DATA_IND:
                    CsrBtHcrpcmL2caDataInd(instData);
                    break;

                case CSR_BT_CM_SDS_REGISTER_CFM:
                    CsrBtHcrpCmSdsRegisterCfm(instData);
                    break;

                case CSR_BT_CM_SDS_UNREGISTER_CFM:
                    CsrBtHcrpcmSdsUnRegisterCfm(instData);
                    break;

                default:
                    CsrBtCmFreeUpstreamMessageContents(instData->event, instData->pRecvMsg);
                    break;
            }
            break;
        }

        default:
            break;
    }

    /* Free received message */
    CsrPmemFree(instData->pRecvMsg);
}

/* Shutdown */
#ifdef ENABLE_SHUTDOWN
void CsrBtHcrpDeinit(void **gash)
{
    hcrpInstanceData_t *instData;
    CsrBool     lastMsg;
    void       *msgData;
    CsrUint16   msgType;

    instData = (hcrpInstanceData_t*)*gash;
    msgData = NULL;
    msgType = 0;
    lastMsg = FALSE;

    while(!lastMsg)
    {
        /* Try to get message from save-queue, otherwise from the scheduler */
        if (NULL == instData->pSaveQueue)
        {
            lastMsg = !CsrSchedMessageGet(&msgType, &msgData);
        }
        else
        {
            if (!CsrMessageQueuePop(&instData->pSaveQueue, &msgType, &msgData))
            {
                instData->pSaveQueue = NULL;
                lastMsg = !CsrSchedMessageGet(&msgType, &msgData);
            }
        }

        if(msgType == CSR_BT_HCRP_PRIM)
        {
            CsrBtHcrpFreeDownstreamMessageContents(msgType, msgData);
            break;
        }
        else if(msgType == CSR_BT_CM_PRIM)
        {
            CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
        }
        else
        {
            /* Unknown message type, assume simple */
        }

        /* Free signal */
        CsrPmemFree(msgData);
    }

    /* Free instance */
    CsrPmemFree(instData);
    *gash = NULL;
}
#endif

