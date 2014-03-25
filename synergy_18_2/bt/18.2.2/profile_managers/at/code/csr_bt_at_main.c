/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"
#include "csr_env_prim.h"



/******************************************************************************
 freeData
*****************************************************************************/
void freeDataReq(atInstanceData_t *atData)
{
    CsrBtAtDgDataReq *atPrim;
    atPrim    = (CsrBtAtDgDataReq *) atData->atMsgP;
    CsrPmemFree(atPrim->payload);
}


/******************************************************************************
atStateHandlerType
*****************************************************************************/
static const atStateHandlerType atStateProviderHandlers[atNumOfState][CSR_BT_AT_PRIM_DOWNSTREAM_COUNT] =
{
    /* atInitialising_s */
    {
        dgActivateReqHandler,                        /* CSR_BT_AT_DG_ACTIVATE_REQ */
        NULL,                                        /* CSR_BT_AT_DG_DEACTIVATE_REQ */
        NULL,                                        /* CSR_BT_AT_DG_DISCONNECT_REQ */
        freeDataReq,                                 /* CSR_BT_AT_DG_DATA_REQ */
        NULL,                                        /* CSR_BT_AT_DG_ATC_RESULT_CODE_RES */
        NULL,                                        /* CSR_BT_AT_DG_DATA_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S0Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S3Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S4Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S5Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S6Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S7Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S8Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S10Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMI_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMM_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMR_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GCAP_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */
        atDgControlReqHandler,                       /* CSR_BT_AT_DG_CONTROL_REQ */
        NULL,                                        /* CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */
        NULL                                         /* CSR_BT_AT_DG_PORTNEG_RES */
    },
    /* atCmd_s */
    {
        dgActivateReqHandler,                        /* CSR_BT_AT_DG_ACTIVATE_REQ */
        dgDeactivateReqHandler,                      /* CSR_BT_AT_DG_DEACTIVATE_REQ */
        dgDisconnectReqHandler,                      /* CSR_BT_AT_DG_DISCONNECT_REQ */
        freeDataReq,                                 /* CSR_BT_AT_DG_DATA_REQ */
        atResultCodeResHandler,                      /* CSR_BT_AT_DG_ATC_RESULT_CODE_RES */
        NULL,                                        /* CSR_BT_AT_DG_DATA_RES */
        atS0QResHandler,                             /* CSR_BT_AT_DG_ATC_S0Q_RES */
        atS3QResHandler,                             /* CSR_BT_AT_DG_ATC_S3Q_RES */
        atS4QResHandler,                             /* CSR_BT_AT_DG_ATC_S4Q_RES */
        atS5QResHandler,                             /* CSR_BT_AT_DG_ATC_S5Q_RES */
        atS6QResHandler,                             /* CSR_BT_AT_DG_ATC_S6Q_RES */
        atS7QResHandler,                             /* CSR_BT_AT_DG_ATC_S7Q_RES */
        atS8QResHandler,                             /* CSR_BT_AT_DG_ATC_S8Q_RES */
        atS10QResHandler,                            /* CSR_BT_AT_DG_ATC_S10Q_RES */
        atGmiResHandler,                             /* CSR_BT_AT_DG_ATC_GMI_RES */
        atGmmResHandler,                             /* CSR_BT_AT_DG_ATC_GMM_RES */
        atGmrResHandler,                             /* CSR_BT_AT_DG_ATC_GMR_RES */
        atGcapResHandler,                            /* CSR_BT_AT_DG_ATC_GCAP_RES */
        atResultCodeConnectTextHandler,              /* CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */
        atDefaultSettingResHandler,                  /* CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */
        atFactorySettingResHandler,                  /* CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */
        atDgControlReqHandler,                       /* CSR_BT_AT_DG_CONTROL_REQ */
        atUnknownExtendedCmdResHandler,               /* CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */
        atDgPortnegResHandler                        /* CSR_BT_AT_DG_PORTNEG_RES */
    },
    /* atOnlineCmd_s */
    {
        dgActivateReqHandler,                        /* CSR_BT_AT_DG_ACTIVATE_REQ */
        dgDeactivateReqHandler,                      /* CSR_BT_AT_DG_DEACTIVATE_REQ */
        dgDisconnectReqHandler,                      /* CSR_BT_AT_DG_DISCONNECT_REQ */
        freeDataReq,                                 /* CSR_BT_AT_DG_DATA_REQ */
        atResultCodeResHandler,                      /* CSR_BT_AT_DG_ATC_RESULT_CODE_RES */
        NULL,                                        /* CSR_BT_AT_DG_DATA_RES */
        atS0QResHandler,                             /* CSR_BT_AT_DG_ATC_S0Q_RES */
        atS3QResHandler,                             /* CSR_BT_AT_DG_ATC_S3Q_RES */
        atS4QResHandler,                             /* CSR_BT_AT_DG_ATC_S4Q_RES */
        atS5QResHandler,                             /* CSR_BT_AT_DG_ATC_S5Q_RES */
        atS6QResHandler,                             /* CSR_BT_AT_DG_ATC_S6Q_RES */
        atS7QResHandler,                             /* CSR_BT_AT_DG_ATC_S7Q_RES */
        atS8QResHandler,                             /* CSR_BT_AT_DG_ATC_S8Q_RES */
        atS10QResHandler,                            /* CSR_BT_AT_DG_ATC_S10Q_RES */
        atGmiResHandler,                             /* CSR_BT_AT_DG_ATC_GMI_RES */
        atGmmResHandler,                             /* CSR_BT_AT_DG_ATC_GMM_RES */
        atGmrResHandler,                             /* CSR_BT_AT_DG_ATC_GMR_RES */
        atGcapResHandler,                            /* CSR_BT_AT_DG_ATC_GCAP_RES */
        atResultCodeConnectTextHandler,              /* CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */
        atDefaultSettingResHandler,                  /* CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */
        atFactorySettingResHandler,                  /* CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */
        atDgControlReqHandler,                       /* CSR_BT_AT_DG_CONTROL_REQ */
        atUnknownExtendedCmdResHandler,               /* CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */
        atDgPortnegResHandler                        /* CSR_BT_AT_DG_PORTNEG_RES */
    },
    /* atOnlineData_s */
    {
        dgActivateReqHandler,                        /* CSR_BT_AT_DG_ACTIVATE_REQ */
        dgDeactivateReqHandler,                      /* CSR_BT_AT_DG_DEACTIVATE_REQ */
        dgDisconnectReqHandler,                      /* CSR_BT_AT_DG_DISCONNECT_REQ */
        dgDataReqHandler,                            /* CSR_BT_AT_DG_DATA_REQ */
        NULL,                                        /* CSR_BT_AT_DG_ATC_RESULT_CODE_RES */
        atDataResHandler,                            /* CSR_BT_AT_DG_DATA_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S0Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S3Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S4Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S5Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S6Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S7Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S8Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_S10Q_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMI_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMM_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GMR_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_GCAP_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_CONNECT_TEXT_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_DEFAULT_SETTING_RES */
        NULL,                                        /* CSR_BT_AT_DG_ATC_FACTORY_SETTING_RES */
        atDgControlReqHandler,                       /* CSR_BT_AT_DG_CONTROL_REQ */
        NULL,                                        /* CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_RES */
        atDgPortnegResHandler                        /* CSR_BT_AT_DG_PORTNEG_RES */
    },
};

/******************************************************************************
atStateHandlerType
*****************************************************************************/
static const atStateHandlerType atDgStateHandlers[atNumOfState][CSR_BT_DG_PRIM_UPSTREAM_COUNT] =
{
    /* atInitialising_s */
    {
        atDgDeactivateCfmHandler,                     /* CSR_BT_DG_DEACTIVATE_CFM */
        atDgDisconnectIndHandler,                     /* CSR_BT_DG_DISCONNECT_IND */
        atDgDataCfmHandler,                           /* CSR_BT_DG_DATA_CFM */
        atDgdataIndInUnknownStateHandler,              /* CSR_BT_DG_DATA_IND */
        atDgConnectIndHandler,                        /* CSR_BT_DG_CONNECT_IND */
        atDgControlIndHandler,                        /* CSR_BT_DG_CONTROL_IND */
        atDgPortnegIndHandler,                        /* CSR_BT_DG_PORTNEG_IND */
        NULL,                                         /* CSR_BT_DG_PORTNEG_CFM */
        NULL,                                         /* CSR_BT_DG_STATUS_IND */
        NULL,                                         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */
        atDgDataPathStatusIndHandle                   /* CSR_BT_DG_DATA_PATH_STATUS_IND */
    },
    /* atCmd_s */
    {
        atDgDeactivateCfmHandler,                     /* CSR_BT_DG_DEACTIVATE_CFM */
        atDgDisconnectIndHandler,                     /* CSR_BT_DG_DISCONNECT_IND */
        atDgDataCfmHandler,                           /* CSR_BT_DG_DATA_CFM */
        atDgdataIndAtCmdAtOnlineCmdStateHandler,      /* CSR_BT_DG_DATA_IND */
        atDgConnectIndHandler,                        /* CSR_BT_DG_CONNECT_IND */
        atDgControlIndHandler,                        /* CSR_BT_DG_CONTROL_IND */
        atDgPortnegIndHandler,                        /* CSR_BT_DG_PORTNEG_IND */
        NULL,                                         /* CSR_BT_DG_PORTNEG_CFM */
        NULL,                                         /* CSR_BT_DG_STATUS_IND */
        NULL,                                         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */
        atDgDataPathStatusIndHandle                   /* CSR_BT_DG_DATA_PATH_STATUS_IND */
    },
    /* atOnlineCmd_s */
    {
        atDgDeactivateCfmHandler,                     /* CSR_BT_DG_DEACTIVATE_CFM */
        atDgDisconnectIndHandler,                     /* CSR_BT_DG_DISCONNECT_IND */
        atDgDataCfmHandler,                           /* CSR_BT_DG_DATA_CFM */
        atDgdataIndAtCmdAtOnlineCmdStateHandler,      /* CSR_BT_DG_DATA_IND */
        atDgConnectIndHandler,                        /* CSR_BT_DG_CONNECT_IND */
        atDgControlIndHandler,                        /* CSR_BT_DG_CONTROL_IND */
        atDgPortnegIndHandler,                        /* CSR_BT_DG_PORTNEG_IND */
        NULL,                                         /* CSR_BT_DG_PORTNEG_CFM */
        NULL,                                         /* CSR_BT_DG_STATUS_IND */
        NULL,                                         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */
        atDgDataPathStatusIndHandle                   /* CSR_BT_DG_DATA_PATH_STATUS_IND */
    },
    /* atOnlineData_s */
    {
        atDgDeactivateCfmHandler,                     /* CSR_BT_DG_DEACTIVATE_CFM */
        atDgDisconnectIndHandler,                     /* CSR_BT_DG_DISCONNECT_IND */
        atDgDataCfmHandler,                           /* CSR_BT_DG_DATA_CFM */
        atDgJustDataIndHandler,                       /* CSR_BT_DG_DATA_IND */
        atDgConnectIndHandler,                        /* CSR_BT_DG_CONNECT_IND */
        atDgControlIndHandler,                        /* CSR_BT_DG_CONTROL_IND */
        atDgPortnegIndHandler,                        /* CSR_BT_DG_PORTNEG_IND */
        NULL,                                         /* CSR_BT_DG_PORTNEG_CFM */
        NULL,                                         /* CSR_BT_DG_STATUS_IND */
        NULL,                                         /* CSR_BT_DG_REGISTER_DATA_PATH_HANDLE_CFM */
        atDgDataPathStatusIndHandle                   /* CSR_BT_DG_DATA_PATH_STATUS_IND */
    },
};

#ifdef ENABLE_SHUTDOWN
/******************************************************************************
CsrBtAtDeinit
*****************************************************************************/
extern void CsrBtDgFreeDownstreamMessageContents(CsrUint16 eventClass, void *msg);

void CsrBtAtDeinit(void **gash)
{
    void                * msg=NULL;
    atInstanceData_t    * atData;
    CsrUint16            eventClass=0;
    CsrBtDgDataReq        * saveMsg;
    void *popMsg;
    int                    i;

    atData    = (atInstanceData_t *) *gash;

    while((CsrMessageQueuePop(&atData->cmDataReqQueue, &eventClass, &popMsg)))
    {
        saveMsg = (CsrBtDgDataReq*)popMsg;
        CsrPmemFree(saveMsg->payload);
        CsrPmemFree(saveMsg);
    }

    for(i = 0; i < CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND; i++)
    {
        CsrPmemFree(atData->atUnknownExtendedCmd[i]);
    }

    if (atData->superVisionTimerId)
    {
        CsrSchedTimerCancel(atData->timerId, NULL, NULL);
    }

    while (CsrSchedMessageGet(&eventClass , &msg))
    {
        switch(eventClass)
        {
            case CSR_BT_AT_PRIM:
                {
                    CsrBtAtFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }
            case CSR_BT_DG_PRIM :
                {
                    CsrBtDgFreeDownstreamMessageContents(eventClass, msg);
                    break;
                }

            case CSR_SCHED_PRIM:
                break;
        }
        CsrPmemFree(msg);
    }
    CsrPmemFree(atData);
}
#endif

/******************************************************************************
CsrBtAtInit
*****************************************************************************/
void CsrBtAtInit(void **gash)
{
    atInstanceData_t  *atData;

    *gash                    = (void *) CsrPmemZalloc(sizeof(atInstanceData_t));
    atData                    = (atInstanceData_t *) *gash;

    atData->state                    = atInitialising_s;
    atData->reachPoint               = CONTINUATION_AT_INTERPRET_FUNC;
    atData->terminatorFlag           = TERMINATORFOUND;
    atData->atCfmFlag                = CFM_RECEIVE;
    atData->appResFlag               = APP_RES;
    atData->paramSettings.eValue     = 1;
    atData->paramSettings.s3Value    = 13;
    atData->paramSettings.s4Value    = 10;
    atData->paramSettings.s5Value    = 8;
    atData->paramSettings.vValue     = 1;
    atData->paramSettings.andDValue  = 2;

    atData->CsrSchedQid                      = CSR_BT_AT_IFACEQUEUE;

    initExtendedCmdTable(atData);
#ifndef CSR_BT_AT_SEND_S0_TOKEN_IND
    atData->paramSettings.s0Value    = CSR_BT_AT_S0_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S6_TOKEN_IND
    atData->paramSettings.s6Value    = CSR_BT_AT_S6_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S7_TOKEN_IND
    atData->paramSettings.s7Value    = CSR_BT_AT_S7_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S8_TOKEN_IND
    atData->paramSettings.s8Value    = CSR_BT_AT_S8_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S10_TOKEN_IND
    atData->paramSettings.s10Value    = CSR_BT_AT_S10_DEFAULT_VALUE;
#endif
}


/******************************************************************************
CsrBtAtHandler
*****************************************************************************/
void CsrBtAtHandler(void **gash)
{
    atInstanceData_t  *atData;
    CsrUint16          eventClass=0;
    void *              msg=NULL;

    atData = (atInstanceData_t *) (*gash);
    CsrSchedMessageGet(&eventClass , &msg);
    atData->atMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_AT_PRIM:
            {
                CsrPrim         *primPtr;
                primPtr = (CsrPrim *)msg;

                if ((*primPtr <= CSR_BT_AT_PRIM_DOWNSTREAM_HIGHEST) && atStateProviderHandlers[atData->state][*primPtr] != NULL)
                {
                    atStateProviderHandlers[atData->state][*primPtr](atData);
                    CsrPmemFree(atData->cmd);
                    atData->cmd = NULL;
                    atData->packetLength = 0;
                }
                break;
            }
        case CSR_BT_DG_PRIM :
            {
                CsrPrim         *primPtr;
                primPtr = (CsrPrim *)msg;

                if(((CsrUint16)(*primPtr - CSR_PRIM_UPSTREAM) < CSR_BT_DG_PRIM_UPSTREAM_COUNT) && atDgStateHandlers[atData->state][(CsrUint16)(*primPtr - CSR_PRIM_UPSTREAM)] != NULL)
                {
                    atDgStateHandlers[atData->state][(CsrUint16)(*primPtr - CSR_PRIM_UPSTREAM)](atData);
                }
                break;
            }

        case CSR_SCHED_PRIM:
            break;
    }
    CsrPmemFree(atData->atMsgP);
}

#endif

