/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_panic.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"
#include "bluetooth.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_av_main.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cmn_sdc_rfc_util.h"
#include "csr_bt_cmn_sdr_tagbased_lib.h"
#include "csr_bt_cmn_sdp_lib.h"

static const avStateHandlerType avPrimStateHandler[AV_NUMBER_OF_STATES][NUMBER_OF_AV_DOWNSTREAM_MESSAGES] =
{
    /* NULL_s */
    {
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_ACTIVATE_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_DEACTIVATE_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_CONNECT_REQ */
        CsrBtAvCancelConnectReqNullStateHandler,/* CSR_BT_AV_CANCEL_CONNECT_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_DISCONNECT_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_DISCOVER_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_GET_CAPABILITIES_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SET_CONFIGURATION_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_GET_CONFIGURATION_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_RECONFIGURE_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_OPEN_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_START_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_CLOSE_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SUSPEND_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_ABORT_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SECURITY_CONTROL_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_STREAM_DATA_REQ */
        CsrBtAvRegisterStreamHandleReqHandler,/* CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */
        CsrBtAvSecurityInHandler,            /* CSR_BT_AV_SECURITY_IN_REQ */
        CsrBtAvSecurityOutHandler,           /* CSR_BT_AV_SECURITY_OUT_REQ */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_DISCOVER_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_GET_CAPABILITIES_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SET_CONFIGURATION_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_GET_CONFIGURATION_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_RECONFIGURE_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_OPEN_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_START_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_CLOSE_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SUSPEND_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_ABORT_RES */
        CsrBtAvSaveMessage,                  /* CSR_BT_AV_SECURITY_CONTROL_RES */
#ifdef CSR_BT_INSTALL_AV_SET_QOS_INTERVAL
        CsrBtAvSetQosIntervalReqHandler,     /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#else
        NULL,                                /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#endif
        CsrBtAvLpNegConfigReqHandler,        /* CSR_BT_AV_LP_NEG_CONFIG_REQ */
        CsrBtAvDelayReportReqHandler,        /* CSR_BT_AV_DELAY_REPORT_REQ */
        CsrBtAvGetCapabilitiesResHandler,    /* CSR_BT_AV_GET_ALL_CAPABILITIES_RES */
        NULL,                                /* CSR_BT_AV_GET_CHANNEL_INFO_REQ */
        NULL,                                /* CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */
    },
    /* READY_s */
    {
        CsrBtAvActivateReqHandler,           /* CSR_BT_AV_ACTIVATE_REQ */
        CsrBtAvDeactivateReqHandler,         /* CSR_BT_AV_DEACTIVATE_REQ */
        CsrBtAvConnectReqHandler,            /* CSR_BT_AV_CONNECT_REQ */
        CsrBtAvCancelConnectReqHandler,      /* CSR_BT_AV_CANCEL_CONNECT_REQ */
        CsrBtAvDisconnectReqHandler,         /* CSR_BT_AV_DISCONNECT_REQ */
        CsrBtAvDiscoverReqHandler,           /* CSR_BT_AV_DISCOVER_REQ */
        CsrBtAvGetCapabilitiesReqHandler,    /* CSR_BT_AV_GET_CAPABILITIES_REQ */
        CsrBtAvSetConfigurationReqHandler,   /* CSR_BT_AV_SET_CONFIGURATION_REQ */
        CsrBtAvGetConfigurationReqHandler,   /* CSR_BT_AV_GET_CONFIGURATION_REQ */
        CsrBtAvReconfigureReqHandler,        /* CSR_BT_AV_RECONFIGURE_REQ */
        CsrBtAvOpenReqHandler,               /* CSR_BT_AV_OPEN_REQ */
        CsrBtAvStartReqHandler,              /* CSR_BT_AV_START_REQ */
        CsrBtAvCloseReqHandler,              /* CSR_BT_AV_CLOSE_REQ */
        CsrBtAvSuspendReqHandler,            /* CSR_BT_AV_SUSPEND_REQ */
        CsrBtAvAbortReqHandler,              /* CSR_BT_AV_ABORT_REQ */
        CsrBtAvSecurityReqHandler,           /* CSR_BT_AV_SECURITY_CONTROL_REQ */
        CsrBtAvStreamDataReqHandler,         /* CSR_BT_AV_STREAM_DATA_REQ */
        CsrBtAvRegisterStreamHandleReqHandler,/* CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */
        CsrBtAvSecurityInHandler,            /* CSR_BT_AV_SECURITY_IN_REQ */
        CsrBtAvSecurityOutHandler,           /* CSR_BT_AV_SECURITY_OUT_REQ */
        CsrBtAvDiscoverResHandler,           /* CSR_BT_AV_DISCOVER_RES */
        CsrBtAvGetCapabilitiesResHandler,    /* CSR_BT_AV_GET_CAPABILITIES_RES */
        CsrBtAvSetConfigurationResHandler,   /* CSR_BT_AV_SET_CONFIGURATION_RES */
        CsrBtAvGetConfigurationResHandler,   /* CSR_BT_AV_GET_CONFIGURATION_RES */
        CsrBtAvReconfigureResHandler,        /* CSR_BT_AV_RECONFIGURE_RES */
        CsrBtAvOpenResHandler,               /* CSR_BT_AV_OPEN_RES */
        CsrBtAvStartResHandler,              /* CSR_BT_AV_START_RES */
        CsrBtAvCloseResHandler,              /* CSR_BT_AV_CLOSE_RES */
        CsrBtAvSuspendResHandler,            /* CSR_BT_AV_SUSPEND_RES */
        CsrBtAvAbortResHandler,              /* CSR_BT_AV_ABORT_RES */
        CsrBtAvSecurityResHandler,           /* CSR_BT_AV_SECURITY_CONTROL_RES */
#ifdef CSR_BT_INSTALL_AV_SET_QOS_INTERVAL
        CsrBtAvSetQosIntervalReqHandler,     /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#else
        NULL,                                /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#endif
        CsrBtAvLpNegConfigReqHandler,        /* CSR_BT_AV_LP_NEG_CONFIG_REQ */
        CsrBtAvDelayReportReqHandler,        /* CSR_BT_AV_DELAY_REPORT_REQ */
        CsrBtAvGetCapabilitiesResHandler,    /* CSR_BT_AV_GET_ALL_CAPABILITIES_RES */
        CsrBtAvGetChannelInfoReqHandler,     /* CSR_BT_AV_GET_CHANNEL_INFO_REQ */
        CsrBtAvGetStreamChannelInfoReqHandler, /* CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */
    },
    /* CLEANUP_S */
    {
        NULL,                               /* CSR_BT_AV_ACTIVATE_REQ */
        NULL,                               /* CSR_BT_AV_DEACTIVATE_REQ */
        NULL,                               /* CSR_BT_AV_CONNECT_REQ */
        NULL,                               /* CSR_BT_AV_CANCEL_CONNECT_REQ */
        NULL,                               /* CSR_BT_AV_DISCONNECT_REQ */
        NULL,                               /* CSR_BT_AV_DISCOVER_REQ */
        NULL,                               /* CSR_BT_AV_GET_CAPABILITIES_REQ */
        NULL,                               /* CSR_BT_AV_SET_CONFIGURATION_REQ */
        NULL,                               /* CSR_BT_AV_GET_CONFIGURATION_REQ */
        NULL,                               /* CSR_BT_AV_RECONFIGURE_REQ */
        NULL,                               /* CSR_BT_AV_OPEN_REQ */
        NULL,                               /* CSR_BT_AV_START_REQ */
        CsrBtAvIgnoreReqHandler,            /* CSR_BT_AV_CLOSE_REQ */
        NULL,                               /* CSR_BT_AV_SUSPEND_REQ */
        NULL,                               /* CSR_BT_AV_ABORT_REQ */
        CsrBtAvSecurityReqFree,             /* CSR_BT_AV_SECURITY_CONTROL_REQ */
        CsrBtAvStreamDataReqFree,           /* CSR_BT_AV_STREAM_DATA_REQ */
        NULL,                               /* CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */
        CsrBtAvSecurityInHandler,           /* CSR_BT_AV_SECURITY_IN_REQ */
        CsrBtAvSecurityOutHandler,          /* CSR_BT_AV_SECURITY_OUT_REQ */
        NULL,                               /* CSR_BT_AV_DISCOVER_RES */
        CsrBtAvGetCapabilitiesResFree,      /* CSR_BT_AV_GET_CAPABILITIES_RES */
        NULL,                               /* CSR_BT_AV_SET_CONFIGURATION_RES */
        CsrBtAvGetConfigurationResFree,     /* CSR_BT_AV_GET_CONFIGURATION_RES */
        NULL,                               /* CSR_BT_AV_RECONFIGURE_RES */
        NULL,                               /* CSR_BT_AV_OPEN_RES */
        NULL,                               /* CSR_BT_AV_START_RES */
        NULL,                               /* CSR_BT_AV_CLOSE_RES */
        NULL,                               /* CSR_BT_AV_SUSPEND_RES */
        NULL,                               /* CSR_BT_AV_ABORT_RES */
        CsrBtAvSecurityResFree,             /* CSR_BT_AV_SECURITY_CONTROL_RES */
#ifdef CSR_BT_INSTALL_AV_SET_QOS_INTERVAL
        CsrBtAvSetQosIntervalReqHandler,     /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#else
        NULL,                                /* CSR_BT_AV_SET_QOS_INTERVAL_REQ */
#endif
        CsrBtAvLpNegConfigReqHandler,       /* CSR_BT_AV_LP_NEG_CONFIG_REQ */
        CsrBtAvDelayReportReqHandler,       /* CSR_BT_AV_DELAY_REPORT_REQ */
        NULL,                               /* CSR_BT_AV_GET_ALL_CAPABILITIES_RES */
        NULL,                               /* CSR_BT_AV_GET_CHANNEL_INFO_REQ */
        NULL,                               /* CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */
    }
};
/* ---------- End of AV primitive jump tables ---------- */


/* ---------- CM instance handler table ---------- */

static const avStateHandlerType cmSdcPrimStateHandler[AV_NUMBER_OF_STATES][CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT] =
{
    /* NULL_s */
    {
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                       /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                       /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                       /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* READY_s */
    {
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                       /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtAvCmSdsRegisterCfmHandler,                  /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtAvCmSdsUnregisterCfmHandler,                /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* CLEANUP_S */
    {
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                       /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                       /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                       /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtAvCmSdsRegisterCfmHandlerCleanup,           /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtAvCmSdsUnregisterCfmHandlerCleanup,         /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    }
};


static const avStateHandlerType cmL2caPrimStateHandler[AV_NUMBER_OF_STATES][CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    /* NULL_s */
    {
        CsrBtAvCmL2caRegisterCfmHandler,            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                       /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                       /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* READY_s */
    {
        NULL,                                       /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtAvCmL2caConnectCfmHandler,             /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtAvCmL2caConnectAcceptCfmHandler,       /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtAvCmL2caCancelConnectAcceptCfmHandler, /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtAvCmL2caDataCfmHandler,                /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtAvCmL2caDataIndHandler,                /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtAvCmL2caDisconnectIndHandler,          /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtAvCmL2caModeChangeIndHandler,          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* CLEANUP_S */
    {
        NULL,                                       /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtAvCmL2caConnectCfmHandlerCleanup,      /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtAvCmL2caConnectAcceptCfmHandlerCleanup,/* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtAvCmL2caCancelConnectAcceptCfmHandlerCleanup,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtAvCmL2caDataIndHandlerCleanup,         /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtAvCmL2caDisconnectIndHandlerCleanup,   /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                       /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtAvCmL2caModeChangeIndHandler,          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    }
};

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvInit
 *
 *  DESCRIPTION
 *      Initializes the AV instance
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvInit(void **gash)
{
    av_instData_t  *instData;
    CsrUintFast8 s;

    /* allocate instance data */
    *gash = (void *) CsrPmemZalloc(sizeof(av_instData_t));
    instData = (av_instData_t *) *gash;

    /* initialise instance data */
    instData->appHandle = CSR_SCHED_QID_INVALID;
    instData->streamAppHandle = CSR_SCHED_QID_INVALID;

    CsrBtScSetSecInLevel(&instData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_AV_MANDATORY_SECURITY_INCOMING,
        CSR_BT_AV_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_AV_SUCCESS,
        CSR_BT_RESULT_CODE_AV_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&instData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_AV_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_AV_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_AV_SUCCESS,
        CSR_BT_RESULT_CODE_AV_UNACCEPTABLE_PARAMETER);

#if CSR_BT_AV_QOS_IND_INTERVAL
    instData->qosInterval = CSR_BT_AV_QOS_IND_INTERVAL;
#endif

    instData->allowStartLpNeg = TRUE;

    for (s = 0; s < CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        instData->stream[s].tLabel                  = 0xFF;
        instData->stream[s].bitRate                 = CSR_BT_A2DP_BIT_RATE_STREAM_CLOSED;
    }

    for(s = 0; s < CSR_BT_AV_MAX_NUM_CONNECTIONS; s++)
    {
        instData->con[s].signalCid  = 0xFF;
        instData->con[s].remoteAVDTPVersion      = DEFAULT_AVDTP_VERSION;
    }

    /* Tell the SD that it must look for the CSR_BT_AUDIO_SINK_UUID and the  CSR_BT_AUDIO_SOURCE_UUID service,
       when it perform a SD_READ_AVAILABLE_SERVICE_REQ                                                            */

#ifndef EXCLUDE_CSR_BT_AV_AUDIO_SINK
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_AUDIO_SINK_UUID);
#endif

#ifndef EXCLUDE_CSR_BT_AV_AUDIO_SOURCE
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_AUDIO_SOURCE_UUID);
#endif

    CsrBtCml2caRegisterReqSend(CSR_BT_AV_IFACEQUEUE, CSR_BT_AVDTP_PSM);
    instData->sdpAvSearchData          = CsrBtUtilSdcInit(CsrBtAvSdcResultHandler, CSR_BT_AV_IFACEQUEUE);
    instData->localVersion             = DEFAULT_AVDTP_VERSION;
}

#ifdef ENABLE_SHUTDOWN
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvDeinit
 *
 *  DESCRIPTION
 *      Performs graceful shutdown of AV instance, free signals on queue and
 *      free instance data
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvDeinit(void **gash)
{
    av_instData_t *instData;
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrUintFast8    s;
    CsrBool    lastMsg;

    instData = (av_instData_t *) (*gash);

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop(&instData->saveQueue, &msg_type, &msg_data))
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

                case CSR_BT_AV_PRIM:
                {
                    CsrBtAvFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }

                case CSR_SCHED_PRIM:
                    break;

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                default:
                    { /* Unknown event type!?!? */
                        CsrStateEventException("AV",msg_type, 0, 0);
                        break;
                    }
#endif
            }
            CsrPmemFree (msg_data);
        }
    }

    /* clear any data on stream queues */
    for (s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
    {
        if(instData->stream[s].streamState != IDLE_S)
        {
            instData->stream[s].mediaCid = 0;
            instData->stream[s].streamState = IDLE_S;
            instData->stream[s].bitRate = CSR_BT_A2DP_BIT_RATE_STREAM_CLOSED;
            CsrBtAvClearStream(instData, (CsrUint8)s);
        }
    }

    /* clear any signal messages on connection queues */
    for(s=0; s<CSR_BT_AV_MAX_NUM_CONNECTIONS; s++)
    {
        CsrBtAvClearConnection(instData, (CsrUint8)s);
    }

    if (instData->sdpAvSearchData != NULL)
    {
        CsrBtUtilSdcRfcDeinit(&(instData->sdpAvSearchData));
    }

    CsrPmemFree(instData);
}
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtAvHandler
 *
 *  DESCRIPTION
 *      Entry function that handles all signals sent to AV instance
 *
 *  RETURNS
 *      void
 *
 *---------------------------------------------------------------------------*/
void CsrBtAvHandler(void **gash)
{
    av_instData_t    *instData;
    CsrUint16        eventClass=0;
    void        *msg=NULL;
    instData = (av_instData_t *) (*gash);

    if(!instData->restoreFlag)
    {
        CsrSchedMessageGet(&eventClass , &msg);
    }
    else
    {
        if(!CsrMessageQueuePop( &(instData->saveQueue), &eventClass, &msg))
        {
            instData->restoreFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &msg);
        }
    }

    instData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_AV_PRIM:
            {
                CsrBtAvPrim         *primType;

                primType = (CsrBtAvPrim *)msg;

                if ((*primType <= CSR_BT_AV_PRIM_DOWNSTREAM_HIGHEST) && (avPrimStateHandler[instData->state][*primType] != NULL))
                {
                    avPrimStateHandler[instData->state][*primType](instData);
                }
                else
                {
                    if (*primType != CSR_BT_AV_HOUSE_CLEANING)
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("AV",
                        eventClass,
                        *primType,
                        (CsrUint16) instData->state);
#endif
                        CsrBtAvFreeDownstreamMessageContents(CSR_BT_AV_PRIM, instData->recvMsgP);
                    }
                }
                break;
            }

        case CSR_BT_CM_PRIM:
        {
            CsrBtCmPrim        *primType;

            primType = (CsrBtCmPrim *) msg;

            if ((*primType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST) &&
                (*primType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST))
            {
                /* Jump Table */
                CsrUint16  index;
                index = (CsrUint16)(*primType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST);
                if(cmSdcPrimStateHandler[instData->state][index] != NULL)
                {
                    cmSdcPrimStateHandler[instData->state][index](instData);
                }
                else
                { /* PrimType Not supported in current state    */
                    if (CsrBtUtilSdcVerifyCmMsg(instData->recvMsgP))
                    {
                        CsrBtUtilSdcCmMsgHandler(instData, instData->sdpAvSearchData, instData->recvMsgP);
                    }
                    else
                    {
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
                    }
                }
            }
            else if ((*primType <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST) &&
                     (*primType >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST))
            {
                /* Jump Table */
                CsrUint16  index;
                index = (CsrUint16)(*primType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST);
                if(cmL2caPrimStateHandler[instData->state][index] != NULL)
                {
                    cmL2caPrimStateHandler[instData->state][index](instData);
                }
                else if (*primType == CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM)
                {
                    CsrBtAvCmL2caChannelInfoCfmHandler(instData);
                }
                else
                { /* PrimType Not supported in current state    */
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
                }
            }
            else
            {
                switch(*primType)
                {
                    default:
                    {
                        /* PrimType Not supported */
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->recvMsgP);
                        break;
                    }
                }
            }
            break;
        }

        case CSR_SCHED_PRIM:
            {
                CsrEnvPrim *primType;
                primType = (CsrEnvPrim *) msg;
                switch(*primType)
                {
                    case CSR_CLEANUP_IND:
                        {
                            CsrUintFast8 s, i;
                            CsrCleanupInd* prim;
                            prim = (CsrCleanupInd*) msg;
                            if(prim->phandle == instData->appHandle)
                            {
                                /* The AV application has terminated abnormally - clean up the mess it has created! */
                                for( i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
                                {
                                    if(instData->con[i].conState != DISCONNECTED_S)
                                    {
                                        instData->state = CLEANUP_S;
                                        break;
                                    }
                                }

                                if(instData->state == CLEANUP_S)
                                {
                                    /* connection(s) and possibly streams present */
                                    for(s=0; s<CSR_BT_AV_MAX_NUM_STREAMS; s++)
                                    {
                                        /* initiate stream cleanup */
                                        CsrBtAvClearStream(instData, (CsrUint8)s);
                                    }

                                    for( i=0; i<CSR_BT_AV_MAX_NUM_CONNECTIONS; i++)
                                    {
                                        if(instData->con[i].conState == CONNECTED_S)
                                        {
                                            /* disconnect already established connections */
                                            CsrBtCml2caDisconnectReqSend(instData->con[i].signalCid);
                                            instData->con[i].conState = DISCONNECTING_S;
                                        }
                                    }
                                }

                                if (getNumActivations(instData->roleRegister))
                                {
                                    instData->state = CLEANUP_S;

                                    if( instData->isConnectable )
                                    {
                                        /* cancel accept incoming connections */
                                        CsrBtCml2caCancelConnectAcceptReqSend(CSR_BT_AV_IFACEQUEUE, CSR_BT_AVDTP_PSM);
                                    }

                                    for( i=0; i<4; i++)
                                    {
                                        /* unregister services */
                                        if( instData->roleRegister[i] > 0 )
                                        {
                                            CsrBtCmSdsUnRegisterReqSend(CSR_BT_AV_IFACEQUEUE, instData->serviceRecordHandles[i]);
                                            instData->roleRegister[i] = 0;
                                        }
                                    }
                                }

                            }
                            break;
                        }
                    default:
                        {
                            /* Other primitives are ignored if exception handler module is excluded! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("AV",
                            eventClass,
                            *primType,
                            (CsrUint16) instData->state);
#endif
                        }
                }

                break;
            }

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("AV",
                                        eventClass,
                                        0,
                                        (CsrUint16) instData->state);
            }
#endif
    }
    CsrPmemFree(instData->recvMsgP);    /* free the received message. if the ptr is NULL, CsrSched just ignores */
}




