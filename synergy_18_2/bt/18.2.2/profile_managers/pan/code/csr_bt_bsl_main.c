/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_bsl_main.h"
#include "csr_bt_bsl_prim.h"
#include "csr_bt_bsl_sef.h"
#include "csr_bt_bsl_util.h"
#include "csr_bt_tasks.h"

/* ---------- BSL jump table ---------- */
static const BslStateHandlerType bslStateHandlers[BslNumberOfStates][CSR_BT_BSL_PRIM_DOWNSTREAM_COUNT] =
{
    /* IdleS */
    {
        CsrBtBslIgnoreMessage,                                       /* CSR_BT_BSL_REGISTER_REQ */
        CsrBtBslXStateBslActivateReqHandler,                         /* CSR_BT_BSL_ACTIVATE_REQ */
        CsrBtBslIdleStateBslConnectReqHandler,                       /* CSR_BT_BSL_CONNECT_REQ */
        CsrBtBslXStateBslDataReqHandler,                             /* CSR_BT_BSL_DATA_REQ */
        CsrBtBslXStateBslMulticastDataReqHandler,                    /* CSR_BT_BSL_MULTICAST_DATA_REQ */
        CsrBtBslXStateBslDisconnectReqHandler,                       /* CSR_BT_BSL_DISCONNECT_REQ */
        CsrBtBslXStateBslDisconnectResHandler,                       /* CSR_BT_BSL_DISCONNECT_RES */
        CsrBtBslXStateBslServiceSearchReqHandler,                    /* CSR_BT_BSL_SERVICE_SEARCH_REQ */
        CsrBtBslSecurityInReqHandler,                                /* CSR_BT_BSL_SECURITY_IN_REQ */
        CsrBtBslSecurityOutReqHandler,                               /* CSR_BT_BSL_SECURITY_OUT_REQ */
        CsrBtBslXStateCancelConnectReqHandler,                       /* CSR_BT_BSL_CANCEL_CONNECT_REQ */
    },
    /* RegisterS */
    {
        CsrBtBslRegisterStateBslRegisterReqHandler,                  /* CSR_BT_BSL_REGISTER_REQ */
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateSaveMessage,
        NULL,
        NULL,
        NULL,
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateSaveMessage,
        CsrBtBslSecurityInReqHandler,                                /* CSR_BT_BSL_SECURITY_IN_REQ */
        CsrBtBslSecurityOutReqHandler,                               /* CSR_BT_BSL_SECURITY_OUT_REQ */
        CsrBtBslXStateCancelConnectReqHandler,                       /* CSR_BT_BSL_CANCEL_CONNECT_REQ */
    },
    /* SdsRegisterS */
    {
        CsrBtBslIgnoreMessage,                                       /* CSR_BT_BSL_REGISTER_REQ */
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateBslDataReqHandler,
        CsrBtBslXStateBslMulticastDataReqHandler,
        CsrBtBslXStateBslDisconnectReqHandler,
        CsrBtBslXStateBslDisconnectResHandler,
        CsrBtBslXStateSaveMessage,
        CsrBtBslSecurityInReqHandler,                                /* CSR_BT_BSL_SECURITY_IN_REQ */
        CsrBtBslSecurityOutReqHandler,                               /* CSR_BT_BSL_SECURITY_OUT_REQ */
        CsrBtBslXStateCancelConnectReqHandler,                       /* CSR_BT_BSL_CANCEL_CONNECT_REQ */
    },
    /* BusyS */
    {
        CsrBtBslIgnoreMessage,                                       /* CSR_BT_BSL_REGISTER_REQ */
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateSaveMessage,
        CsrBtBslXStateBslDataReqHandler,
        CsrBtBslXStateBslMulticastDataReqHandler,
        CsrBtBslXStateBslDisconnectReqHandler,
        CsrBtBslXStateBslDisconnectResHandler,
        CsrBtBslXStateSaveMessage,
        CsrBtBslSecurityInReqHandler,                                /* CSR_BT_BSL_SECURITY_IN_REQ */
        CsrBtBslSecurityOutReqHandler,                               /* CSR_BT_BSL_SECURITY_OUT_REQ */
        CsrBtBslXStateCancelConnectReqHandler,                       /* CSR_BT_BSL_CANCEL_CONNECT_REQ */
    },
};

/* ---------- CM jump table ---------- */
static const BslStateHandlerType cmStateHandlers[BslNumberOfStates][CSR_BT_CM_BNEP_PRIM_UPSTREAM_COUNT] =
{
    /* Idle_s */
    {
        CsrBtBslXStateCmBnepConnectIndHandler,                       /* CSR_BT_CM_BNEP_CONNECT_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */
        NULL,                                                   /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtBslXStateCmBnepExtendedDataIndHandler,                  /* CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
        CsrBtBslXStateCmBnepModeChangeIndHandler,                    /* CSR_BT_CM_BNEP_MODE_CHANGE_IND */
        CsrBtBslXStateCmSwitchRoleIndHandler,                        /* CSR_BT_CM_BNEP_SWITCH_ROLE_IND */
        CsrBtBslXStateCmBnepDisconnectIndHandler,                    /* CSR_BT_CM_BNEP_DISCONNECT_IND */
    },
    /* Register_s */
    {
        NULL,                                                   /* CSR_BT_CM_BNEP_CONNECT_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */
        NULL,                                                   /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                                   /* CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_MODE_CHANGE_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_SWITCH_ROLE_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_DISCONNECT_IND */
    },
    /* SdsRegister_s */
    {
        CsrBtBslXStateCmBnepConnectIndHandler,                       /* CSR_BT_CM_BNEP_CONNECT_IND */
        CsrBtBslSdsRegisterStateCmBnepConnectAcceptCfmHandler,       /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */
        CsrBtBslSdsRegisterStateCmBnepCancelConnectAcceptCfmHandler, /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtBslXStateCmBnepExtendedDataIndHandler,                  /* CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
        CsrBtBslXStateCmBnepModeChangeIndHandler,                    /* CSR_BT_CM_BNEP_MODE_CHANGE_IND */
        CsrBtBslXStateCmSwitchRoleIndHandler,                        /* CSR_BT_CM_BNEP_SWITCH_ROLE_IND */
        CsrBtBslXStateCmBnepDisconnectIndHandler,                    /* CSR_BT_CM_BNEP_DISCONNECT_IND */
    },
    /* BusyS */
    {
        CsrBtBslXStateCmBnepConnectIndHandler,                       /* CSR_BT_CM_BNEP_CONNECT_IND */
        NULL,                                                   /* CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */
        CsrBtBslBusyStateCmBnepCancelConnectAcceptCfmHandler,        /* CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtBslXStateCmBnepExtendedDataIndHandler,                  /* CSR_BT_CM_BNEP_EXTENDED_DATA_IND */
        CsrBtBslXStateCmBnepModeChangeIndHandler,                    /* CSR_BT_CM_BNEP_MODE_CHANGE_IND */
        CsrBtBslXStateCmSwitchRoleIndHandler,                        /* CSR_BT_CM_BNEP_SWITCH_ROLE_IND */
        CsrBtBslXStateCmBnepDisconnectIndHandler,                    /* CSR_BT_CM_BNEP_DISCONNECT_IND */
    },
};
/* ---------- End of jump tables ---------- */

void CsrBtBslInit(void **gash)
{
    BslInstanceData_t  *instData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(BslInstanceData_t));
    instData = (BslInstanceData_t *) *gash;

    /* clear own instance data for first time use */
    instData->state                 = RegisterS;
    instData->napSdsRecHandle       = SDS_REC_HANDLE_UNUSED;
    instData->gnSdsRecHandle        = SDS_REC_HANDLE_UNUSED;
    instData->panuSdsRecHandle      = SDS_REC_HANDLE_UNUSED;

    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_PAN_MANDATORY_SECURITY_INCOMING,
        CSR_BT_PAN_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_BSL_SUCCESS,
        CSR_BT_RESULT_CODE_BSL_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_PAN_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_PAN_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_BSL_SUCCESS,
        CSR_BT_RESULT_CODE_BSL_UNACCEPTABLE_PARAMETER);
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
void CsrBtBslDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrBool    lastMsg;
    BslInstanceData_t  *instData;

    instData = (BslInstanceData_t *) (*gash);

    CsrSchedTimerCancel(instData->bnepConnectReqTimerId,NULL,NULL);

    lastMsg = FALSE;
    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data))
        {
            lastMsg = !CsrSchedMessageGet(&msg_type, &msg_data);
        }
        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_BT_BSL_PRIM:
                {
                    CsrBtBslFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;


            }    /* end switch */
            CsrPmemFree(msg_data);
        }
    }
    if (instData->sdcSearchRec != NULL)
    {
        CsrPmemFree(instData->sdcSearchRec->serviceName);
        CsrPmemFree(instData->sdcSearchRec->serviceDescription);
        CsrPmemFree(instData->sdcSearchRec->ipv4Subnet);
        CsrPmemFree(instData->sdcSearchRec->ipv6Subnet);
        CsrPmemFree(instData->sdcSearchRec);
    }
    CsrPmemFree(instData);
}
#endif

void CsrBtBslHandler(void **gash)
{
    BslInstanceData_t    *instData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    instData = (BslInstanceData_t *) (*gash);

    if(!instData->restoreFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop(&instData->saveQueue, &eventClass, &msg))
        {
            instData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
        else
        {

        }
    }
    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_BSL_PRIM:
            {
                CsrBtBslPrim         *primType;

                /* find the message type */
                primType = (CsrBtBslPrim *)msg;
                if ((*primType < CSR_BT_BSL_PRIM_DOWNSTREAM_COUNT) &&
                    (bslStateHandlers[instData->state][*primType] != NULL))
                {
                    bslStateHandlers[instData->state][*primType](instData);
                }
                else if (*primType != CSR_BT_BSL_HOUSE_CLEANING)
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrGeneralException("BSL",
                     instData->state,
                     *primType,
                     "Unhandled BSL prim in BSL main");
#endif
                    CsrBtBslFreeDownstreamMessageContents(eventClass, msg);
                }
                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrPrim         *primType;

            /* find the message type */
            primType = (CsrPrim *)msg;

            /* the read local bd addr is not in the bnep range of
             * signals so handle separately to save jump table
             * space */
            if (*primType == CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM)
            {
                CsrBtBslRegisterStateCmReadLocalBdAddrCfmHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDC_SEARCH_IND)
            {
                CsrBtBslBusyStateCmSdcSearchIndHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDC_SEARCH_CFM)
            {
                CsrBtBslBusyStateCmSdcSearchCfmHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDC_ATTRIBUTE_CFM)
            {
                CsrBtBslBusyStateCmSdcAttributeCfmHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDC_CLOSE_IND)
            {
                CsrBtBslBusyStateCmSdcCloseIndHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDS_REGISTER_CFM)
            {
                CsrBtBslSdsRegisterStateCmSdsRegisterCfmHandler(instData);
            }
            else if (*primType == CSR_BT_CM_SDS_UNREGISTER_CFM)
            {
                CsrBtBslSdsRegisterStateCmSdsUnregisterCfmHandler(instData);
            }
            else if (((CsrUint16)(*primType - CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST) < CSR_BT_CM_BNEP_PRIM_UPSTREAM_COUNT) &&
                     (cmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST)] != NULL))
            {
                cmStateHandlers[instData->state][(CsrUint16)(*primType - CSR_BT_CM_BNEP_PRIM_UPSTREAM_LOWEST)](instData);
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("BSL", instData->state, *primType, "Unhandled CM prim in BSL main");
#endif
                CsrBtCmFreeUpstreamMessageContents(eventClass, instData->recvMsgP);
            }
            break;
        }

        case CSR_SCHED_PRIM:
            break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrGeneralException("BSL",
                     instData->state,
                     eventClass,
                     "Unhandled EVENT class in BSL main");
                break;
            }
#endif

    }
    /* free the received message. if the ptr is NULL just CsrSched just ignores */
    CsrPmemFree(instData->recvMsgP);
}

