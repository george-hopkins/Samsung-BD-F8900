/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_env_prim.h"

#include "csr_bt_result.h"
#include "csr_bt_hidd_main.h"
#include "csr_bt_hidd_prim.h"
#include "csr_bt_hidd_sef.h"

#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_sd_private_lib.h"

#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

static void csrBtHiddSaveMessage(HiddInstanceDataType *instData)
{
    CsrMessageQueuePush(&instData->saveQueue, CSR_BT_HIDD_PRIM, instData->pRecvMsg);
    instData->pRecvMsg = NULL;
}

static void csrBtHiddSaveCmMessage(HiddInstanceDataType *instData)
{
    CsrMessageQueuePush(&instData->saveCmQueue, CSR_BT_CM_PRIM, instData->pRecvMsg);
    instData->pRecvMsg = NULL;
}

/* -------- HIDD Message Jump Table --------- */
static const HiddStateHandlerType hiddStateHandlers[HIDD_NUMBER_OF_STATES][CSR_BT_HIDD_PRIM_DOWNSTREAM_COUNT] =
{
    /* HIDD_INIT_STATE */
    {
        csrBtHiddSaveMessage,                 /* CSR_BT_HIDD_ACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_IDLE_STATE */
    {
        CsrBtHiddActivateIdleHandler,         /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_REGISTERING_SDP */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_ACCEPT_CONNECTING_STATE */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_CONNECTING_STATE */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_UNREGISTERING_SDP */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        NULL,                            /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_CONNECTED_STATE */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        CsrBtHiddControlRespConnectedHandler, /* CSR_BT_HIDD_CONTROL_RES */
        CsrBtHiddDataConnectedHandler,        /* CSR_BT_HIDD_DATA_REQ */
        CsrBtHiddUnplugConnectedHandler,      /* CSR_BT_HIDD_UNPLUG_REQ */
        CsrBtHiddChangeModeConnectedHandler,  /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    },

    /* HIDD_NOT_CONNECTED_STATE */
    {
        NULL,                            /* CSR_BT_HIDD_ACTIVATE_REQ */
        CsrBtHiddDeactivateHandler,           /* CSR_BT_HIDD_DEACTIVATE_REQ */
        NULL,                            /* CSR_BT_HIDD_CONTROL_RES */
        NULL,                            /* CSR_BT_HIDD_DATA_REQ */
        NULL,                            /* CSR_BT_HIDD_UNPLUG_REQ */
        CsrBtHiddChangeModeNotConnectedHandler, /* CSR_BT_HIDD_MODE_CHANGE_REQ */
        CsrBtHiddSecurityInHandler,           /* CSR_BT_HIDD_SECURITY_IN_REQ */
        CsrBtHiddSecurityOutHandler           /* CSR_BT_HIDD_SECURITY_OUT_REQ */
    }
};

/* -------- HIDD Message Jump Table --------- */
static const HiddStateHandlerType cmSdcStateHandlers[HIDD_NUMBER_OF_STATES][CSR_BT_CM_SDC_PRIM_UPSTREAM_COUNT] =
{
    /* HIDD_INIT_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_IDLE_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_REGISTERING_SDP */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        CsrBtHiddCmRegisterSdpRegisteringHandler,      /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtHiddCmUnregisterSdpRegisteringHandler,    /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_ACCEPT_CONNECTING_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_CONNECTING_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_UNREGISTERING_SDP */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        CsrBtHiddCmUnregisterSdpUnregisteringHandler,  /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_CONNECTED_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    },
    /* HIDD_NOT_CONNECTED_STATE */
    {
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_IND */
        NULL,                                          /* CSR_BT_CM_SDC_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_OPEN_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_ATTRIBUTE_CFM */
        NULL,                                          /* CSR_BT_CM_SDC_CLOSE_IND */
        NULL,                                          /* CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_SDS_UNREGISTER_CFM */
    }
};

static const HiddStateHandlerType cmL2caStateHandlers[HIDD_NUMBER_OF_STATES][CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    /* HIDD_INIT_STATE */
    {
        CsrBtHiddCmRegisterInitHandler,                /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_IDLE_STATE */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectIgnoreHandler,            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_REGISTERING_SDP */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_ACCEPT_CONNECTING_STATE */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        CsrBtHiddCmConnectAcceptConnectingHandler,     /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        CsrBtHiddCmCancelConnectAcceptConnectingHandler,/* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        csrBtHiddSaveCmMessage,                        /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectHandler,                  /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_CONNECTING_STATE */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtHiddCmConnectConnectingHandler,           /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectHandler,                  /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtHiddCmModeChangeHandler,                  /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_UNREGISTERING_SDP */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_DATA_CFM */
        csrBtHiddSaveCmMessage,                        /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectHandler,                  /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_CONNECTED_STATE */
    {
        NULL,                                          /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtHiddCmDataCfmConnectedHandler,            /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtHiddCmDataIndConnectedHandler,            /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectHandler,                  /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                          /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtHiddCmModeChangeHandler,                  /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* HIDD_NOT_CONNECTED_STATE */
    {
        NULL,                                         /* CSR_BT_CM_L2CA_REGISTER_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_DATA_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_DATA_IND */
        CsrBtHiddCmDisconnectHandler,                 /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        NULL,                                         /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        NULL,                                         /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        NULL,                                         /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    }
};

/* -------- no more jump tables --------- */
static void csrBtHiddCleanUpHandler(HiddInstanceDataType *instData)
{
    instData->cleanUp = TRUE;

    switch(instData->state)
    {
        case HIDD_ACCEPT_CONNECTING_STATE:
            {
                if(!(instData->ctrlCh.cid))
                {
                    /* Cancel accept connect request */
                    CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle,CSR_BT_HID_CTRL_PSM);
                    CsrBtCml2caCancelConnectAcceptReqSend(instData->myAppHandle,CSR_BT_HID_INTR_PSM);
                }
            }
            break;
        case HIDD_CONNECTING_STATE:
            {
                if(!(instData->ctrlCh.cid))
                {
                    /* Cancel connect request */
                    CsrBtCml2caCancelConnectReqSend(instData->myAppHandle,instData->bdAddr,CSR_BT_HID_CTRL_PSM);
                }
            }
            break;
        case HIDD_CONNECTED_STATE:
            {
                /* Disconnect connection */
                CsrBtCml2caDisconnectReqSend(instData->intrCh.cid);
            }
            break;
        case HIDD_NOT_CONNECTED_STATE:
            {
                /* Clean up and return to idle */
                CsrBtHiddCleanUpToIdle(instData);
            }
            break;
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("HIDD", CSR_SCHED_PRIM, CSR_CLEANUP_IND, (CsrUint16) instData->state); /* state - event error! */
            }
            break;
#endif
    }
}

#ifdef ENABLE_SHUTDOWN
static void csrBtHiddReleaseMessage(CsrUint16 msgType, void *msgData)
{
    switch (msgType)
    {
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmFreeUpstreamMessageContents(msgType, msgData);
            break;
        }
        case CSR_BT_HIDD_PRIM:
        {
            CsrBtHiddFreeDownstreamMessageContents(msgType, msgData);
        break;
        }
        default:
        {
            /* No data to free */
        }
        break;
    }
}

static void csrBtHiddCleanUpQueue(HiddInstanceDataType *instData)
{
    CsrUint16 msgType=0;
    void * msgData=NULL;
    CsrBool    msgFound;

    do
    {
        msgFound = CsrMessageQueuePop(&instData->saveQueue, &msgType, &msgData);
        if (msgFound)
        {
            csrBtHiddReleaseMessage(msgType, msgData);
            CsrPmemFree(msgData);
        }
    } while(msgFound);
    do
    {
        msgFound = CsrSchedMessageGet(&msgType, &msgData);
        if (msgFound)
        {
            csrBtHiddReleaseMessage(msgType, msgData);
            CsrPmemFree(msgData);
        }

    } while(msgFound);
}
#endif /* ENABLE_SHUTDOWN */

/* init function to be called by the scheduler during Synergy BT initialisation */
void CsrBtHiddInit(void ** gash)
{
    HiddInstanceDataType  *pInstData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemZalloc(sizeof(HiddInstanceDataType));
    pInstData = (HiddInstanceDataType *) * gash;

    pInstData->myAppHandle = CsrSchedTaskQueueGet();
    pInstData->currentMode = CSR_BT_HIDD_ACTIVE_MODE;
    pInstData->nextMode = CSR_BT_HIDD_ACTIVE_MODE;

    CsrBtScSetSecInLevel(&pInstData->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_HIDD_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HIDD_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HIDD_SUCCESS,
        CSR_BT_RESULT_CODE_HIDD_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecOutLevel(&pInstData->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_HIDD_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HIDD_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HIDD_SUCCESS,
        CSR_BT_RESULT_CODE_HIDD_UNACCEPTABLE_PARAMETER);

    /* Tell the SD that it must look for the CSR_BT_HID_PROFILE_UUID
       service, when it perform a SD_READ_AVAILABLE_SERVICE_REQ   */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_HID_PROFILE_UUID);

    /* register the channel in the CM */
    CsrBtCml2caRegisterReqSend(pInstData->myAppHandle, CSR_BT_HID_CTRL_PSM);
}

/* handler function to be called by the scheduler, whenever a message is available for DUN Client */
void CsrBtHiddHandler(void ** gash)
{
    HiddInstanceDataType*   pInstData;
    CsrUint16                eventClass=0;

    pInstData = (HiddInstanceDataType *) (* gash);

    if(pInstData->restoreHiddFlag)
    {
        if(!CsrMessageQueuePop(&pInstData->saveQueue, &eventClass, &(pInstData->pRecvMsg)))
        {
            pInstData->restoreHiddFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &(pInstData->pRecvMsg));
        }
    }
    else if(pInstData->restoreCmFlag)
    {
        if(!CsrMessageQueuePop(&pInstData->saveCmQueue, &eventClass, &(pInstData->pRecvMsg)))
        {
            pInstData->restoreCmFlag = FALSE;
            CsrSchedMessageGet(&eventClass , &(pInstData->pRecvMsg));
        }
    }
    else
    {
        CsrSchedMessageGet(&eventClass , &(pInstData->pRecvMsg));
    }

    switch(eventClass)
    {
        case CSR_BT_HIDD_PRIM:
            {
                /* find the message type */
                CsrBtHiddPrim*   pPrimType = (CsrBtHiddPrim *)pInstData->pRecvMsg;
                if ((*pPrimType < CSR_BT_HIDD_PRIM_DOWNSTREAM_COUNT) && hiddStateHandlers[pInstData->state][*pPrimType] != NULL)
                {
                    /* Jump Table */
                    hiddStateHandlers[pInstData->state][*pPrimType](pInstData);
                }
                else if(*pPrimType == CSR_BT_HIDD_REACTIVATE_IND)
                {
                    if(HIDD_IDLE_STATE == pInstData->state)
                    {
                        CsrBtHiddReactivateIdleHandler(pInstData);
                    }
                    else
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("HIDD",
                        eventClass,
                        *pPrimType,
                        (CsrUint16) pInstData->state); /* state - event error! */
#endif
                        CsrBtHiddFreeDownstreamMessageContents(eventClass,
                                                          pInstData->pRecvMsg);

                    }

                }
                else if (*pPrimType != CSR_BT_HIDD_RESTORE_IND)
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("HIDD",
                        eventClass,
                        *pPrimType,
                        (CsrUint16) pInstData->state); /* state - event error! */
#endif
                        CsrBtHiddFreeDownstreamMessageContents(eventClass,
                                                          pInstData->pRecvMsg);
                }
            }
            break;
        case CSR_BT_CM_PRIM:
            {
                /* find the message type */
                CsrPrim*  pPrimType = (CsrPrim *)pInstData->pRecvMsg;
                if (*pPrimType >= CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST &&
                    *pPrimType <= CSR_BT_CM_SDC_PRIM_UPSTREAM_HIGHEST)
                {
                    /* Jump Table */
                    CsrUint16  index;
                    index = (CsrUint16)(*pPrimType - CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST);
                    if(cmSdcStateHandlers[pInstData->state][index] != NULL)
                    {
                        cmSdcStateHandlers[pInstData->state][index](pInstData);
                    }
                    else
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("HIDD", eventClass, *pPrimType, (CsrUint16) pInstData->state);
#endif
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInstData->pRecvMsg);
                    }

                }
                else if (*pPrimType >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST &&
                         *pPrimType <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST)
                {
                    /* Jump Table */
                    CsrUint16  index;
                    index = (CsrUint16)(*pPrimType - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST);
                    if(cmL2caStateHandlers[pInstData->state][index] != NULL)
                    {
                        cmL2caStateHandlers[pInstData->state][index](pInstData);
                    }
                    else
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("HIDD", eventClass, *pPrimType, (CsrUint16) pInstData->state);
#endif
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInstData->pRecvMsg);
                    }

                }
                else if (*pPrimType != CSR_BT_HIDD_RESTORE_IND)
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("HIDD",
                        eventClass,
                        *pPrimType,
                        (CsrUint16) pInstData->state); /* state - event error! */
#endif
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInstData->pRecvMsg);
                }
            }
            break;
        case CSR_SCHED_PRIM:
            {
                /* find the message type */
                CsrEnvPrim *pPrimType = (CsrEnvPrim *) pInstData->pRecvMsg;
                if(*pPrimType == CSR_CLEANUP_IND)
                {
                    CsrCleanupInd* prim;
                    prim = (CsrCleanupInd*) pInstData->pRecvMsg;
                    if(prim->phandle == pInstData->appHandle)
                    {
                        csrBtHiddCleanUpHandler(pInstData);
                    }
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else
                {
                    CsrStateEventException("HIDD",
                        eventClass,
                        *pPrimType,
                        (CsrUint16) pInstData->state); /* state - event error! */
                }
#endif
            }
            break;
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default:
            {
                CsrStateEventException("HIDD",
                    eventClass,
                    0xFF,
                    (CsrUint16) pInstData->state); /* state - event error! */
            }
            break;
#endif
    }
    CsrPmemFree(pInstData->pRecvMsg);  /* free the received msg, if the ptr is NULL the CsrSched just ignores it */
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
void CsrBtHiddDeinit(void ** gash)
{
    HiddInstanceDataType *pInstData;
    pInstData = (HiddInstanceDataType *) *gash;

    pInstData->cleanUp = TRUE;
    csrBtHiddCleanUpQueue(pInstData);
    CsrBtHiddCleanUpToIdle(pInstData);
    CsrPmemFree(pInstData);
}
#endif /* ENABLE_SHUTDOWN */

void CsrBtHiddCleanUpToIdle(HiddInstanceDataType *instData)
{
    /* reset instance data */
    instData->saveQueue = NULL;
    instData->saveCmQueue = NULL;
    instData->deviceIdSdp.recordHandle = 0;
    instData->hidSdp.recordHandle = 0;
    instData->ctrlCh.cid = 0;
    instData->ctrlCh.hostMtu = 0;
    instData->ctrlCh.localMtu = 0;
    instData->ctrlCh.recvMsgLength = 0;
    instData->ctrlCh.sendMsgLength = 0;
    instData->ctrlCh.modeChangeSuccess = FALSE;
    instData->intrCh.cid = 0;
    instData->intrCh.hostMtu = 0;
    instData->intrCh.localMtu = 0;
    instData->intrCh.recvMsgLength = 0;
    instData->intrCh.sendMsgLength = 0;
    instData->intrCh.modeChangeSuccess = FALSE;
    CsrPmemFree(instData->ctrlCh.recvMsg);
    instData->ctrlCh.recvMsg = NULL;
    CsrPmemFree(instData->ctrlCh.sendMsg);
    instData->ctrlCh.sendMsg = NULL;
    CsrPmemFree(instData->intrCh.recvMsg);
    instData->intrCh.recvMsg = NULL;
    CsrPmemFree(instData->intrCh.sendMsg);
    instData->intrCh.sendMsg = NULL;

    if(instData->deactivating)
    {
        instData->appHandle = 0;
    }
    if(instData->deactivating || instData->cleanUp)
    {
        bd_addr_zero(&(instData->bdAddr));

        CsrPmemFree(instData->deviceIdSdp.record);
        instData->deviceIdSdp.record = NULL;
        instData->deviceIdSdp.recordLen = 0;

        CsrPmemFree(instData->hidSdp.record);
        instData->hidSdp.record = NULL;
        instData->hidSdp.recordLen = 0;

        CsrPmemFree(instData->ctrlCh.qos);
        instData->ctrlCh.qos = NULL;

        CsrPmemFree(instData->intrCh.qos);
        instData->intrCh.qos = NULL;
        instData->flushTimeout = 0;
        instData->hidFlags = 0;
    }
    instData->currentMode = CSR_BT_HIDD_ACTIVE_MODE;
    instData->nextMode = CSR_BT_HIDD_ACTIVE_MODE;
    instData->prevError = 0;
    instData->numOfRetries = 0;
    instData->disconnectReason = 0;
    instData->cancelChCount = 0;
    instData->reconnect = FALSE;
    instData->deactivating = FALSE;
    instData->cleanUp = FALSE;
    instData->registeringNewRec = FALSE;
    instData->newRecUnregCfmRec = FALSE;
    instData->deactUnregCfmRec = FALSE;
    instData->restoreHiddFlag = FALSE;
    instData->restoreCmFlag = FALSE;
    CSR_BT_HIDD_STATE_CHANGE(instData->state, HIDD_IDLE_STATE);
}

#ifdef CSR_LOG_ENABLE
void CsrBtHiddChangeState(const CsrCharString *file, CsrUint32 line,
                                      const CsrCharString *fsmName,
                                      CsrUint32 *state, CsrUint32 newState, const CsrCharString *newStateName)
{
    CsrLogStateTransition((CSR_LOG_STATE_TRANSITION_MASK_FSM_NAME |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE |
                           CSR_LOG_STATE_TRANSITION_MASK_NEXT_STATE_STR |
                           CSR_LOG_STATE_TRANSITION_MASK_PREV_STATE),
                          0,
                          fsmName,
                          *state,
                          NULL,
                          0,
                          NULL,
                          newState,
                          newStateName,
                          line,
                          file);

    *state = newState;
}
#endif

