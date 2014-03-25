/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_panic.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "csr_bt_result.h"
#include "bluetooth.h"
#include "hci_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_exceptionhandler.h"
#include "csr_bt_dg_main.h"
#include "csr_bt_dg_prim.h"
#include "csr_bt_dg_sef.h"
#include "sds_prim.h"
#include "csr_bt_sc_private_lib.h"

void CsrBtDgSaveMessage(DgInstanceData_t *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_DG_PRIM, instData->recvMsgP);
    instData->recvMsgP = NULL;
}

/* ---------- DG jump table ---------- */
static const DgStateHandlerType dgStateHandlers[DgNumberOfStates][CSR_BT_DG_PRIM_DOWNSTREAM_COUNT] =
{
    /* init_s */
    {
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_ACTIVATE_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_DEACTIVATE_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_DISCONNECT_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_DATA_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_DATA_RES */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_CONTROL_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_PORTNEG_RES */
        CsrBtDgSaveMessage,                                    /* FIXME CSR_BT_DG_PORTNEG_REQ */
        CsrBtDgXStateRegisterDataPathHandleReqHandler,         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */
        CsrBtDgSaveMessage,                                    /* CSR_BT_DG_DATA_PATH_STATUS_REQ */
        CsrBtDgSecurityInReqHandler,                           /* CSR_BT_DG_SECURITY_IN_REQ */
    },
    /* idle_s */
    {
        CsrBtDgIdleStateDgActivateReqHandler,                  /* CSR_BT_DG_ACTIVATE_REQ */
        CsrBtDgIdleStateDgDeactivateReqHandler,                /* CSR_BT_DG_DEACTIVATE_REQ */
        CsrBtDgIgnoreMessageHandler,                           /* CSR_BT_DG_DISCONNECT_REQ */
        CsrBtDgNotConnectedDgDataReqHandler,                   /* CSR_BT_DG_DATA_REQ */
        CsrBtDgIgnoreMessageHandler,                           /* CSR_BT_DG_DATA_RES */
        NULL,                                             /* CSR_BT_DG_CONTROL_REQ */
        NULL,                                             /* CSR_BT_DG_PORTNEG_RES */
        NULL,                                             /* FIXME CSR_BT_DG_PORTNEG_REQ */
        CsrBtDgXStateRegisterDataPathHandleReqHandler,         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                             /* CSR_BT_DG_DATA_PATH_STATUS_REQ */
        CsrBtDgSecurityInReqHandler,                           /* CSR_BT_DG_SECURITY_IN_REQ */
    },
    /* activated_s */
    {
        NULL,                                             /* CSR_BT_DG_ACTIVATE_REQ */
        CsrBtDgActivateStateDgDeactivateReqHandler,            /* CSR_BT_DG_DEACTIVATE_REQ */
        NULL,                                             /* CSR_BT_DG_DISCONNECT_REQ */
        CsrBtDgNotConnectedDgDataReqHandler,                   /* CSR_BT_DG_DATA_REQ */
        CsrBtDgIgnoreMessageHandler,                           /* CSR_BT_DG_DATA_RES */
        NULL,                                             /* CSR_BT_DG_CONTROL_REQ */
        CsrBtDgConnectedStateDgPortnegResHandler,              /* CSR_BT_DG_PORTNEG_RES */
        NULL,                                             /* FIXME: CSR_BT_DG_PORTNEG_REQ */
        CsrBtDgXStateRegisterDataPathHandleReqHandler,         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */
        NULL,                                             /* CSR_BT_DG_DATA_PATH_STATUS_REQ */
        CsrBtDgSecurityInReqHandler,                           /* CSR_BT_DG_SECURITY_IN_REQ */
    },
    /* connected_s */
    {
        NULL,                                              /* CSR_BT_DG_ACTIVATE_REQ */
        CsrBtDgConnectedStateDgDeactivateReqHandler,            /* CSR_BT_DG_DEACTIVATE_REQ */
        CsrBtDgConnectedStateDgDisconnectReqHandler,            /* CSR_BT_DG_DISCONNECT_REQ */
        CsrBtDgConnectedStateDgDataReqHandler,                  /* CSR_BT_DG_DATA_REQ */
        CsrBtDgConnectedStateDgDataResHandler,                  /* CSR_BT_DG_DATA_RES */
        CsrBtDgConnectedStateDgControlReqHandler,               /* CSR_BT_DG_CONTROL_REQ */
        CsrBtDgConnectedStateDgPortnegResHandler,               /* CSR_BT_DG_PORTNEG_RES */
        NULL,                                             /* FIXME: CSR_BT_DG_PORTNEG_REQ */
        CsrBtDgXStateRegisterDataPathHandleReqHandler,         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_REQ */
        CsrBtDgConnectedStateDgDataPathStatusReqHandler,       /* CSR_BT_DG_DATA_PATH_STATUS_REQ */
        CsrBtDgSecurityInReqHandler,                           /* CSR_BT_DG_SECURITY_IN_REQ */
    }
};

/* ---------- CM jump table ---------- */
static const DgStateHandlerType cmRfcStateHandlers[DgNumberOfStates][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
    /* init_s */
    {
        NULL,                                              /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        CsrBtDgInitStateCmRegisterCfmHandler,              /* CSR_BT_CM_REGISTER_CFM */
        NULL,                                              /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtDgNotConnectedCmDataIndHandler,               /* CSR_BT_CM_DATA_IND */
        NULL,                                              /* CSR_BT_CM_DATA_CFM */
        NULL,                                              /* CSR_BT_CM_CONTROL_IND */
        NULL,                                              /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                              /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                              /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Idle_s */
    {
        NULL,                                              /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                              /* CSR_BT_CM_REGISTER_CFM */
        CsrBtDgIdleStateCmDisconnectIndHandler,            /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_CONNECT_CFM */
        CsrBtDgIgnoreMessageHandler,                       /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtDgNotConnectedCmDataIndHandler,               /* CSR_BT_CM_DATA_IND */
        NULL,                                              /* CSR_BT_CM_DATA_CFM */
        NULL,                                              /* CSR_BT_CM_CONTROL_IND */
        NULL,                                              /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        NULL,                                              /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                              /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Activate_s */
    {
        CsrBtDgActivateStateCmCancelAcceptConnectCfmHandler,    /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                                   /* CSR_BT_CM_CONNECT_CFM */
        CsrBtDgDeOrActivateStateCmConnectAcceptCfmHandler,      /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                                   /* CSR_BT_CM_REGISTER_CFM */
        CsrBtDgActivateStateCmDisconnectIndHandler,             /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                                   /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                                   /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                                   /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtDgNotConnectedCmDataIndHandler,                    /* CSR_BT_CM_DATA_IND */
        NULL,                                                   /* CSR_BT_CM_DATA_CFM */
        NULL,                                                   /* CSR_BT_CM_CONTROL_IND */
        CsrBtDgModeChangeIndHandler,                            /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtDgConnectedStateCmPortnegIndHandler,               /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                                   /* CSR_BT_CM_PORTNEG_CFM */
    },
    /* Connected_s */
    {
        NULL,                                              /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_CONNECT_ACCEPT_CFM */
        NULL,                                              /* CSR_BT_CM_REGISTER_CFM */
        CsrBtDgConnectedStateCmDisconnectIndHandler,       /* CSR_BT_CM_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_CONNECT_CFM */
        NULL,                                              /* CSR_BT_CM_SCO_DISCONNECT_IND */
        NULL,                                              /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */
        CsrBtDgConnectedStateCmDataIndHandler,             /* CSR_BT_CM_DATA_IND */
        CsrBtDgConnectedStateCmDataCfmHandler,             /* CSR_BT_CM_DATA_CFM */
        CsrBtDgConnectedStateCmControlIndHandler,          /* CSR_BT_CM_CONTROL_IND */
        CsrBtDgModeChangeIndHandler,                       /* CSR_BT_CM_RFC_MODE_CHANGE_IND */
        CsrBtDgConnectedStateCmPortnegIndHandler,          /* CSR_BT_CM_PORTNEG_IND */
        NULL,                                              /* CSR_BT_CM_PORTNEG_CFM */
    }
};

static const DgStateHandlerType cmSdcStateHandlers[DgNumberOfStates][CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT] =
{
    /* init_s */
    {
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                              /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtDgInitStateSdsRegisterCfm,                         /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtDgInitStateSdsUnregisterCfm,                       /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* Idle_s */
    {
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                              /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* Activate_s */
    {
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                              /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* Connected_s */
    {
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                              /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                              /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                              /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                              /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    }
};
/* ---------- End of jump tables ---------- */

void CsrBtDgInit(void **gash)
{
    DgInstanceData_t  *dgData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(DgInstanceData_t));
    dgData = (DgInstanceData_t *) *gash;

    CsrBtScSetSecInLevel(&dgData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_DUN_GW_MANDATORY_SECURITY_INCOMING,
        CSR_BT_DUN_GW_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_DG_SUCCESS,
        CSR_BT_RESULT_CODE_DG_UNACCEPTABLE_PARAMETER);

    /* Both handles must start invalid */
    dgData->dataHandle          = CSR_SCHED_QID_INVALID;
    dgData->ctrlHandle          = CSR_SCHED_QID_INVALID;
    dgData->modemStatus         = CSR_BT_MODEM_SEND_CTRL_DCE_DEFAULT;
    dgData->breakSignal         = CSR_BT_DEFAULT_BREAK_SIGNAL;
    /* send a register to CM to allocate the internal server channel number for the client role. */
    CsrBtCmRegisterReqSend(CSR_BT_DG_IFACEQUEUE);
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
void CsrBtDgDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;

    /* continue to poll any message of the input queue */
    CsrBool    lastMsg;
    DgInstanceData_t    *dgData;

    dgData = (DgInstanceData_t*)*gash;

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&dgData->saveQueue, &msg_type, &msg_data))
        {
            lastMsg = (CsrBool)(!CsrSchedMessageGet(&msg_type, &msg_data));
        }
        if (!lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg_data);
                    break;
                }

                case CSR_BT_DG_PRIM:
                {
                    CsrBtDgFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    /* Silently ignore environment messages */
                    break;

                default:
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("DG", msg_type, 0, 0);
#endif
                    break;
                }
            }
            CsrPmemFree (msg_data);
        }
    }
    CsrPmemFree(dgData);
}
#endif


void CsrBtDgHandler(void **gash)
{
    DgInstanceData_t    *dgData;
    CsrUint16            eventClass=0;
    void                *msg=NULL;

    dgData = (DgInstanceData_t *) (*gash);

    if(!dgData->restoreDgFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);

    }
    else
    {
        if(!CsrMessageQueuePop(&dgData->saveQueue, &eventClass, &msg))
        {
            dgData->restoreDgFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
    }
    dgData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_SCHED_PRIM:
            {
                /* Environment cleanup */
                CsrEnvPrim *primType;
                primType = (CsrEnvPrim*)msg;
                if(*primType == CSR_CLEANUP_IND)
                {
                    CsrBtDgEnvironmentCleanupHandler(dgData);
                }
                break;
            }

        case CSR_BT_DG_PRIM:
            {
                CsrBtDgPrim         *primType;

                /* find the message type */
                primType = (CsrBtDgPrim *)msg;
                if ((*primType < CSR_BT_DG_PRIM_DOWNSTREAM_COUNT) && dgStateHandlers[dgData->state][*primType] != NULL)
                {
                    dgStateHandlers[dgData->state][*primType](dgData);
                }
                else if (*primType != CSR_BT_DG_HOUSE_CLEANING)
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("DG",
                        eventClass,
                        *primType,
                        dgData->state);/* State/Event ERROR! */
#endif
                    CsrBtDgFreeDownstreamMessageContents(eventClass, msg);
                }
                break;
            }
        case CSR_BT_CM_PRIM:
        {
            CsrPrim         *primType;

            /* find the message type */
            primType = (CsrPrim *)msg;

            if (*primType >= CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST &&
                *primType <= CSR_BT_CM_RFC_PRIM_UPSTREAM_HIGHEST &&
                cmRfcStateHandlers[dgData->state][(CsrUint16)(*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)] != NULL)
            {
                cmRfcStateHandlers[dgData->state][(CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST)](dgData);
            }
            else if (*primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST &&
                     *primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST &&
                     cmSdcStateHandlers[dgData->state][(CsrUint16)(*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST)] != NULL)
            {
                cmSdcStateHandlers[dgData->state][(CsrUint16) (*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST)](dgData);
            }
            else
            { /* State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("DG", eventClass, *primType, dgData->state);
#endif
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
            }

            break;
        }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("DG",
                    eventClass,
                    0xFF,
                    dgData->state);/* State/Event ERROR! */
            }
#endif
    }

    /* free the received message. if the ptr is NULL just CsrSched just ignores */
    CsrPmemFree(dgData->recvMsgP);
}

