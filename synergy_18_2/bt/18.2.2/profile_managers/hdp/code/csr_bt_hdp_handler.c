
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_result.h"
#include "csr_bt_hdp_handler.h"
#include "csr_bt_hdp_sef.h"
#include "csr_bt_sc_private_lib.h"
#include "csr_bt_hdp_util.h"
#include "csr_bt_hdp_prim_out.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif
#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"

#include "csr_bt_mcap_private_prim.h"

extern void CsrBtHdpFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);

static const HdpStateHandlerType CsrBtHdpStateHandlers[HDP_NUMBER_OF_STATES][CSR_BT_HDP_PRIM_DOWNSTREAM_HIGHEST + 1] =
{
    /* HDP_IDLE_STATE */
    {
        CsrBtHdpIdleRegisterReqEvHandler,            /* HDP_REGISTER_REQ */
        CsrBtHdpIdleRegisterRspEvHandler,            /* HDP_REGISTER_RSP */
        CsrBtHdpGetCapabReqHandler,                  /* HDP_GET_CAPAB_REQ */
        CsrBtHdpIdleActivateReqEvHandler,            /* HDP_ACTIVATE_REQ */
        CsrBtHdpConnectAssociateChannelReqHandler,   /* HDP_CONNECT_ASSOCIATE_CHANNEL_REQ*/
        NULL,                                        /* HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES*/
        NULL,                                        /* HDP_CONNECT_VIRTUAL_CHANNEL_REQ*/
        NULL,                                        /* HDP_CONNECT_VIRTUAL_CHANNEL_RES*/
        NULL,                                        /* HDP_DATA_REQ*/
        NULL,                                        /* HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ*/
        NULL,                                        /* HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpInvalidSuspendReqEvHandler,          /* HDP_SUSPEND_REQ */
        CsrBtHdpInvalidResumeReqEvHandler,           /* HDP_RESUME_REQ */
        NULL,                                        /* HDP_RESUME_RSP */
        CsrBtHdpXXXHdpUnregisterReqHandler,          /* HDP_UNREGISTER_REQ*/
        CsrBtHdpDeactivateReqHandler                 /* HDP_DEACTIVATE_REQ*/
    },
    /* HDP_CONNECTING_STATE */
    {
        CsrBtHdpInvalidRegisterReqEvHandler,         /* HDP_REGISTER_REQ */
        CsrBtHdpInvalidRegisterRspEvHandler,         /* HDP_REGISTER_RSP */
        CsrBtHdpGetCapabReqHandler,                  /* HDP_GET_CAPAB_REQ */
        CsrBtHdpInvalidActivateReqEvHandler,         /* HDP_ACTIVATE_REQ */
        CsrBtHdpXXXConnectAssociateChannelReqHandler,/* HDP_CONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpAcceptConnectAssociateChannelResHandler,  /* HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES*/
        NULL,                                        /* HDP_CONNECT_VIRTUAL_CHANNEL_REQ*/
        NULL,                                        /* HDP_CONNECT_VIRTUAL_CHANNEL_RES*/
        NULL,                                        /* HDP_DATA_REQ*/
        CsrBtHdpAssociateChDisconnectReqHandler,     /* HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ*/
        NULL,                                        /* HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpInvalidSuspendReqEvHandler,          /* HDP_SUSPEND_REQ */
        CsrBtHdpXXXResumeReqEvHandler,               /* HDP_RESUME_REQ */
        NULL,                                        /* HDP_RESUME_RSP */
        CsrBtHdpXXXHdpUnregisterReqHandler,          /* HDP_UNREGISTER_REQ*/
        CsrBtHdpDeactivateReqHandler                 /* HDP_DEACTIVATE_REQ*/

    },
    /* HDP_CONNECTED_STATE */
    {
        CsrBtHdpInvalidRegisterReqEvHandler,                       /* HDP_REGISTER_REQ */
        CsrBtHdpInvalidRegisterRspEvHandler,                       /* HDP_REGISTER_RSP */
        CsrBtHdpGetCapabReqHandler,                                /* HDP_GET_CAPAB_REQ */
        CsrBtHdpInvalidActivateReqEvHandler,                       /* HDP_ACTIVATE_REQ */
        CsrBtHdpXXXConnectAssociateChannelReqHandler,              /* HDP_CONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpAcceptConnectAssociateChannelResHandler,           /* HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES*/
        CsrBtHdpDataConnectVirtualChannelReqEvHandler,             /* HDP_CONNECT_VIRTUAL_CHANNEL_REQ */
        CsrBtHdpConnectedAcceptConnectVirtualChannelResEvHandler,  /* HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */
        CsrBtHdpDataReqHandler,                                    /* HDP_DATA_REQ*/
        CsrBtHdpAssociateChDisconnectReqHandler,                   /* HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpVirtualChDisconnectReqHandler,                     /* HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpXXXSuspendReqEvHandler,                            /* HDP_SUSPEND_REQ */
        CsrBtHdpXXXResumeReqEvHandler,                             /* HDP_RESUME_REQ */
        NULL,                                                      /* HDP_RESUME_RSP */
        CsrBtHdpXXXHdpUnregisterReqHandler,                        /* HDP_UNREGISTER_REQ*/
        CsrBtHdpDeactivateReqHandler                               /* HDP_DEACTIVATE_REQ*/

    },
    /* HDP_DATA_STATE */
    {
        CsrBtHdpInvalidRegisterReqEvHandler,              /* HDP_REGISTER_REQ */
        CsrBtHdpInvalidRegisterRspEvHandler,              /* HDP_REGISTER_RSP */
        CsrBtHdpGetCapabReqHandler,                       /* HDP_GET_CAPAB_REQ */
        CsrBtHdpInvalidActivateReqEvHandler,              /* HDP_ACTIVATE_REQ */
        CsrBtHdpXXXConnectAssociateChannelReqHandler,     /* HDP_CONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpAcceptConnectAssociateChannelResHandler,  /* HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES*/
        CsrBtHdpDataConnectVirtualChannelReqEvHandler,    /* HDP_CONNECT_VIRTUAL_CHANNEL_REQ */
        NULL,                                             /* HDP_CONNECT_VIRTUAL_CHANNEL_RES*/
        CsrBtHdpDataReqHandler,                           /* HDP_DATA_REQ*/
        CsrBtHdpAssociateChDisconnectReqHandler,          /* HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpVirtualChDisconnectReqHandler,            /* HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpXXXSuspendReqEvHandler,                   /* HDP_SUSPEND_REQ */
        CsrBtHdpXXXResumeReqEvHandler,                    /* HDP_RESUME_REQ */
        NULL,                                             /* HDP_RESUME_RSP */
        CsrBtHdpXXXHdpUnregisterReqHandler,               /* HDP_UNREGISTER_REQ*/
        CsrBtHdpDeactivateReqHandler                      /* HDP_DEACTIVATE_REQ*/

    },
    /* HDP_SUSPEND_STATE */
    {
        CsrBtHdpInvalidRegisterReqEvHandler,              /* HDP_REGISTER_REQ */
        CsrBtHdpInvalidRegisterRspEvHandler,              /* HDP_REGISTER_RSP */
        CsrBtHdpGetCapabReqHandler,                       /* HDP_GET_CAPAB_REQ */
        CsrBtHdpInvalidActivateReqEvHandler,              /* HDP_ACTIVATE_REQ */
        CsrBtHdpXXXConnectAssociateChannelReqHandler,     /* HDP_CONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpAcceptConnectAssociateChannelResHandler,  /* HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES*/
        CsrBtHdpDataConnectVirtualChannelReqEvHandler,    /* HDP_CONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpConnectedAcceptConnectVirtualChannelResEvHandler,/* HDP_CONNECT_VIRTUAL_CHANNEL_RES*/
        CsrBtHdpDataReqHandler,                           /* HDP_DATA_REQ */
        CsrBtHdpAssociateChDisconnectReqHandler,          /* HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ*/
        CsrBtHdpVirtualChDisconnectReqHandler,            /* HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ*/
        CsrBtHdpXXXSuspendReqEvHandler,                   /* HDP_SUSPEND_REQ */
        CsrBtHdpXXXResumeReqEvHandler,                    /* HDP_RESUME_REQ */
        CsrBtHdpSuspendHdpResumeRspHandler,               /* HDP_RESUME_RSP */
        CsrBtHdpXXXHdpUnregisterReqHandler,               /* HDP_UNREGISTER_REQ*/
        CsrBtHdpDeactivateReqHandler                      /* HDP_DEACTIVATE_REQ*/

    }
};

static const HdpStateHandlerType mcapHdpEvHandlers[HDP_NUMBER_OF_STATES][CSR_BT_MCAP_CTRL_PRIM_UPSTREAM_HIGHEST - CSR_BT_MCAP_PRIM_UPSTREAM_LOWEST + 1] =
{
   /* HDP_IDLE_STATE */
    {

        CsrBtHdpIdleMcapActivateCfmEvHandler,/*MCAP_ACTIVATE_CFM*/
        CsrBtHdpMcapDeactivateCfmHandler,    /*MCAP_DEACTIVATE_CFM*/
        CsrBtHdpMcapConnectMclIndHandler,    /*MCAP_CONNECT_MCL_IND*/
        NULL,                                /*MCAP_CONNECT_MCL_CFM*/
        NULL,                                /*MCAP_DISCONNECT_MCL_IND*/
        NULL,                                /*MCAP_DISCONNECT_MCL_CFM*/
        NULL,                                /*MCAP_CREATE_MDL_IND*/
        NULL,                                /*MCAP_RECONNECT_MDL_IND*/
        NULL,                                /*MCAP_RECONNECT_MDL_CFM*/
        NULL,                                /*MCAP_CONNECT_MDL_IND*/
        NULL,                                /*MCAP_CONNECT_MDL_CFM*/
        NULL,                                /*MCAP_DISCONNECT_MDL_IND*/
        NULL,                                /*MCAP_DISCONNECT_MDL_CFM*/
        NULL,                                /*MCAP_ABORT_MDL_IND*/
        NULL,                                /*MCAP_DELETE_MDL_IND*/
        NULL,                                /*MCAP_DELETE_MDL_CFM*/
        NULL,                                /*MCAP_DATA_SEND_IND*/
        NULL                                 /*MCAP_DATA_SEND_CFM*/
    },
    /* HDP_CONNECTING_STATE */
    {

       NULL,                                      /*MCAP_ACTIVATE_CFM*/
       CsrBtHdpMcapDeactivateCfmHandler,          /*MCAP_DEACTIVATE_CFM*/
       CsrBtHdpMcapConnectMclIndHandler,          /*MCAP_CONNECT_MCL_IND*/
       CsrBtHdpMcapConnectMclCfmHandler,          /*MCAP_CONNECT_MCL_CFM*/
       CsrBtHdpConnectingMcapMclDiscIndHandler,   /*MCAP_DISCONNECT_MCL_IND*/
       CsrBtHdpXXXMcapMclDisconnectCfmHandler,    /*MCAP_DISCONNECT_MCL_CFM*/
       CsrBtHdpMcapCreateMdlIndHandler,           /*MCAP_CREATE_MDL_IND*/
       CsrBtHdpXXXMcapReconnectMdlIndHandler,     /*MCAP_RECONNECT_MDL_IND*/
       NULL,                                      /*MCAP_RECONNECT_MDL_CFM*/
       CsrBtHdpMcapConnectMdlIndHandler,          /*MCAP_CONNECT_MDL_IND*/
       CsrBtHdpMcapConnectMdlCfmHandler,          /*MCAP_CONNECT_MDL_CFM*/
       NULL,                                      /*MCAP_DISCONNECT_MDL_IND*/
       NULL,                                      /*MCAP_DISCONNECT_MDL_CFM*/
       NULL,                                      /*MCAP_ABORT_MDL_IND*/
       NULL,                                      /*MCAP_DELETE_MDL_IND*/
       NULL,                                      /*MCAP_DELETE_MDL_CFM*/
       NULL,                                      /*MCAP_DATA_SEND_IND*/
       NULL                                       /*MCAP_DATA_SEND_CFM*/
    },
    /* HDP_CONNECTED_STATE */
    {

       NULL,                                       /*MCAP_ACTIVATE_CFM*/
       CsrBtHdpMcapDeactivateCfmHandler,           /*MCAP_DEACTIVATE_CFM*/
       CsrBtHdpMcapConnectMclIndHandler,           /*MCAP_CONNECT_MCL_IND*/
       NULL,                                       /*MCAP_CONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapMclDisconnectIndHandler,     /*MCAP_DISCONNECT_MCL_IND*/
       CsrBtHdpXXXMcapMclDisconnectCfmHandler,     /*MCAP_DISCONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapCreateMdlIndEvHandler,       /*MCAP_CREATE_MDL_IND*/
       CsrBtHdpXXXMcapReconnectMdlIndHandler,      /*MCAP_RECONNECT_MDL_IND*/
       NULL,                                       /*MCAP_RECONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapConnectMdlIndEvHandler,      /*MCAP_CONNECT_MDL_IND*/
       CsrBtHdpConnectedMcapConnectMdlCfmEvHandler,/*MCAP_CONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapMdlDisconnectIndHandler,     /*MCAP_DISCONNECT_MDL_IND*/
       CsrBtHdpXXXMcapMdlDisconnectCfmHandler,     /*MCAP_DISCONNECT_MDL_CFM*/
       NULL,                                       /*MCAP_ABORT_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteIndHandler,         /*MCAP_DELETE_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteCfmHandler,         /*MCAP_DELETE_MDL_CFM*/
       CsrBtHdpMcapDataIndHandler,                 /*MCAP_DATA_SEND_IND*/
       NULL                                        /*MCAP_DATA_SEND_CFM*/


    },
    /* HDP_DATA_STATE */
    {

       NULL,                                       /*MCAP_ACTIVATE_CFM*/
       CsrBtHdpMcapDeactivateCfmHandler,           /*MCAP_DEACTIVATE_CFM*/
       CsrBtHdpMcapConnectMclIndHandler,           /*MCAP_CONNECT_MCL_IND*/
       NULL,                                       /*MCAP_CONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapMclDisconnectIndHandler,     /*MCAP_DISCONNECT_MCL_IND*/
       CsrBtHdpXXXMcapMclDisconnectCfmHandler,     /*MCAP_DISCONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapCreateMdlIndEvHandler,       /*MCAP_CREATE_MDL_IND*/
       CsrBtHdpXXXMcapReconnectMdlIndHandler,      /*MCAP_RECONNECT_MDL_IND*/
       NULL,                                       /*MCAP_RECONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapConnectMdlIndEvHandler,      /*MCAP_CONNECT_MDL_IND*/
       CsrBtHdpConnectedMcapConnectMdlCfmEvHandler,/*MCAP_CONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapMdlDisconnectIndHandler,     /*MCAP_DISCONNECT_MDL_IND*/
       CsrBtHdpXXXMcapMdlDisconnectCfmHandler,     /*MCAP_DISCONNECT_MDL_CFM*/
       NULL,                                       /*MCAP_ABORT_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteIndHandler,         /*MCAP_DELETE_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteCfmHandler,         /*MCAP_DELETE_MDL_CFM*/
       CsrBtHdpMcapDataIndHandler,                 /*MCAP_DATA_SEND_IND*/
       CsrBtHdpMcapDataCfmHandler                  /*MCAP_DATA_SEND_CFM*/


    },
    /* HDP_SUSPEND_STATE */
    {
       NULL,                                       /*MCAP_ACTIVATE_CFM*/
       NULL,                                       /*MCAP_DEACTIVATE_CFM*/
       CsrBtHdpMcapConnectMclIndHandler,           /*MCAP_CONNECT_MCL_IND*/
       CsrBtHdpSuspendMcapMclConnectCfmHandler,    /*MCAP_CONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapMclDisconnectIndHandler,     /*MCAP_DISCONNECT_MCL_IND*/
       CsrBtHdpSuspendMcapMclDisconnectCfmHandler, /*MCAP_DISCONNECT_MCL_CFM*/
       CsrBtHdpXXXMcapCreateMdlIndEvHandler,       /*MCAP_CREATE_MDL_IND*/
       CsrBtHdpXXXMcapReconnectMdlIndHandler,      /*MCAP_RECONNECT_MDL_IND*/
       CsrBtHdpSuspendMcapReconnectMdlCfmHandler,  /*MCAP_RECONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapConnectMdlIndEvHandler,      /*MCAP_CONNECT_MDL_IND*/
       CsrBtHdpConnectedMcapConnectMdlCfmEvHandler,/*MCAP_CONNECT_MDL_CFM*/
       CsrBtHdpXXXMcapMdlDisconnectIndHandler,     /*MCAP_DISCONNECT_MDL_IND */
       CsrBtHdpSuspendMcapMdlDisconnectCfmHandler, /*MCAP_DISCONNECT_MDL_CFM*/
       NULL,                                       /*MCAP_ABORT_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteIndHandler,         /*MCAP_DELETE_MDL_IND*/
       CsrBtHdpXXXMcapMdlDeleteCfmHandler,         /*MCAP_DELETE_MDL_CFM*/
       CsrBtHdpMcapDataIndHandler,                 /*MCAP_DATA_SEND_IND*/
       CsrBtHdpMcapDataCfmHandler                  /*MCAP_DATA_SEND_CFM*/


    }
};

void CsrBtHdpInit(void **gash)
{
    HdpDb *pInstance;

    *gash = CsrPmemZalloc(sizeof(HdpDb));

    pInstance = *gash;

    CsrCmnListInit(&pInstance->IdList, 0, NULL, NULL);

    CsrBtScSetSecOutLevel(&pInstance->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_HDP_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_HDP_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_HDP_SUCCESS,
        CSR_BT_RESULT_CODE_HDP_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecInLevel(&pInstance->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_HDP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_HDP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_HDP_SUCCESS,
        CSR_BT_RESULT_CODE_HDP_UNACCEPTABLE_PARAMETER);
}

void CsrBtHdpHandler(void **gash)
{
    HdpDb *pInstance = *gash;
    CsrUintFast32 i = 0;
    CsrPrim *primType;


    CsrSchedMessageGet(&pInstance->eventClass, &pInstance->recvMsgP);

    primType = pInstance->recvMsgP;

    switch(pInstance->eventClass)
    {
        case CSR_BT_HDP_PRIM:
        {
            switch ( *primType )
            {
                case CSR_BT_HDP_REGISTER_REQ :
                {
                   CsrBtHdpRegisterReq *pMsg = (CsrBtHdpRegisterReq*)pInstance->recvMsgP;
                    /* check if a new instance can be created  */
                    while ( ( i  < HDP_NUM_OF_INSTANCES) &&
                            ( pInstance->instance[i] != NULL ))
                    {
                        if(pInstance->instance[i]->appHandle == pMsg->appHandle)
                        {
                            CsrBtHdpRegisterCfmSend(pMsg->appHandle,
                                                  HDP_PSM_INVALID ,
                                                  CSR_BT_RESULT_CODE_HDP_ALREADY_REGISTERED);
                            break;
                            /*Send negative cfm*/
                        }
                        else
                        {
                          i++;
                        }
                    }

                    if ( i < HDP_NUM_OF_INSTANCES )
                    {
                        pInstance->instance[i] = CsrPmemAlloc(sizeof(HdpInstanceData));
                        CsrBtHdpAssignDefaultValues(pInstance->instance[i]);
                        pInstance->instance[i]->sdpSearchData = CsrBtUtilSdcInit(CsrBtHdpSdcResultHandler, CSR_BT_HDP_IFACEQUEUE);
                        CsrCmnListInit(&pInstance->instance[i]->mdepList, 0, NULL, NULL);

                        CsrCmnListInit(&pInstance->instance[i]->connectionList,
                                       0,
                                       NULL,
                                       NULL);

                        pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
                        pInstance->instance[i]->eventClass = pInstance->eventClass;
                        CsrBtHdpIdleRegisterReqEvHandler(pInstance->instance[i]);
                    }
                    else
                    {
                        CsrBtHdpRegisterCfmSend(((CsrBtHdpRegisterReq *)pInstance->recvMsgP)->appHandle,
                                                  CSR_BT_RESULT_CODE_HDP_MAX_INSTANCES,
                                                  CSR_BT_SUPPLIER_HDP);
                    }
                }
                break;



                default :
                {
                    CsrBtHdpRegisterRsp *pMsg = ( CsrBtHdpRegisterRsp *) pInstance->recvMsgP;

                    while ( i < HDP_NUM_OF_INSTANCES )
                    {
                        if ( IS_VALID_PTR(pInstance->instance[i]) &&
                             pInstance->instance[i]->appHandle == pMsg->appHandle )
                        {
                            pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
                            pInstance->instance[i]->eventClass = pInstance->eventClass;
                            if ( (*primType <= CSR_BT_HDP_PRIM_DOWNSTREAM_HIGHEST  ) &&
                                 IS_VALID_PTR(CsrBtHdpStateHandlers[pInstance->instance[i]->state][*primType]) )
                            {
                                CsrBtHdpStateHandlers[pInstance->instance[i]->state][*primType](pInstance->instance[i]);
                            }
                            else
                            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                                CsrStateEventException("HDP : Msg for Invalid HDP Instance",
                                                pInstance->eventClass,
                                                *primType,
                                                0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                            }
                            break;
                        }
                        else
                        {
                            i++;
                        }
                    }

                
                }
                break;
            }
        }
        break;

    case CSR_BT_CM_PRIM :
    {
        pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
        pInstance->instance[i]->eventClass = pInstance->eventClass;

        if (CsrBtUtilSdcVerifyCmMsg(pInstance->recvMsgP))
        {
            while ( i < HDP_NUM_OF_INSTANCES )
            {
                if ( IS_VALID_PTR(pInstance->instance[i]))
                {
                    if ( pInstance->instance[i]->sdpState == HDP_SDP_ACTIVE_STATE )
                    {
                        if (CsrBtUtilSdcCmMsgHandler(pInstance->instance[i], pInstance->instance[i]->sdpSearchData, pInstance->instance[i]->recvMsgP))
                        {
                            CsrBtHdpSdpRestartSearch(pInstance->instance[i]);
                        }
                        break;
                    }
                }

                i++;
            }

        }
        else if ( IN_RANGE(*primType, CSR_BT_CM_SDC_PRIM_UPSTREAM_LOWEST, CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST + 1 )  )
        {
            if (*primType == CSR_BT_CM_L2CA_REGISTER_CFM)
            {
                while ( i < HDP_NUM_OF_INSTANCES )
                {
                    if ( IS_VALID_PTR(pInstance->instance[i]))
                    {
                        if ( pInstance->instance[i]->aux_state == HDP_IDLE_W4_PSM )
                        {
                            pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
                            pInstance->instance[i]->eventClass = pInstance->eventClass;
                            CsrBtHdpIdleL2CaRegisterCfmEvHandler(pInstance->instance[i]);
                            break;
                        }
                    }
                    i++;
                }

            }
            else if (*primType == CSR_BT_CM_SDS_REGISTER_CFM)
            {
                while ( i < HDP_NUM_OF_INSTANCES )
                {
                    if ( IS_VALID_PTR(pInstance->instance[i]))
                    {
                        if ( pInstance->instance[i]->aux_state == HDP_IDLE_W4_SERVICE_HANDLE)
                        {
                            pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
                            pInstance->instance[i]->eventClass = pInstance->eventClass;
                            CsrBtHdpIdleCmSdsRegisterCfmHandler(pInstance->instance[i]);
                            break;
                        }
                    }
                    i++;
                }

            }
            else if (*primType == CSR_BT_CM_SDS_UNREGISTER_CFM)
            {
                while ( i < HDP_NUM_OF_INSTANCES )
                {
                    if ( IS_VALID_PTR(pInstance->instance[i]))
                    {
                        if ( pInstance->instance[i]->state == HDP_IDLE_STATE)
                        {
                            HdpInstanceIdListEntry *instanceNode,*nextNode;

                            pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
                            pInstance->instance[i]->eventClass = pInstance->eventClass;
                            CsrBtHdpIdleCmSdsUnregisterCfmHandler(pInstance->instance[i]);

                            CsrPmemFree(pInstance->instance[i]);
                            pInstance->instance[i] = NULL;

                            for (instanceNode = HDP_INSTANCE_GET_FIRST(pInstance->IdList); instanceNode; instanceNode = nextNode)
                            {
                                nextNode = (HdpInstanceIdListEntry *)instanceNode->next;
                                if ( instanceNode->instanceId == i )
                                {
                                    HDP_INSTANCE_DEL_ENTRY(pInstance->IdList, instanceNode);
                                }

                            }
                            break;
                        }
                    }
                    i++;
                }

            }
            else
            {
              CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInstance->recvMsgP);
            }
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
          CsrStateEventException("HDP - CM-PRIM not handled",
                            pInstance->eventClass,
                            *primType,
                            0);


#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
          CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInstance->recvMsgP);
        }
    }
    break;
    case CSR_BT_MCAP_PRIM:
    {
        CsrBool bProcessEvent = FALSE;

        switch (*primType )
        {
            case CSR_BT_MCAP_CONNECT_MCL_CFM :
            {
                HdpInstanceIdListEntry *instanceNode = HDP_INSTANCE_GET_FIRST(pInstance->IdList);
                CsrBtMcapConnectMclCfm *msg = (CsrBtMcapConnectMclCfm *)pInstance->recvMsgP;
                CsrBool bFound = FALSE;

                /* search the entire instance[i] to find the right instance of
                   the outgoing connection cfm based on the remote ctrl PSM */
                while ( i < HDP_NUM_OF_INSTANCES )
                {
                    if ( IS_VALID_PTR(pInstance->instance[i]) &&
                         (pInstance->instance[i]->psm.control == msg->psm) )
                    {
                        while ( IS_VALID_PTR(instanceNode)  )
                        {
                            if ( CsrBtBdAddrEq(&instanceNode->bd_addr,&msg->bd_addr)  )
                            {
                                /* possible reconnect case */
                                instanceNode->instanceId = (CsrUint8)i;
                                instanceNode->mclId = msg->mclId;
                                /* The remote control psm is stored */
                                instanceNode->ctrlPsm = HDP_GET_CTRL_PSM( pInstance->instance[i]->hdpInstanceId ) ;
                                bFound = TRUE;
                                break;
                            }
                            instanceNode = (HdpInstanceIdListEntry *)instanceNode->next;
                        }
                        if(bFound == FALSE )
                        {
                           instanceNode = HDP_INSTANCE_ADD_ENTRY(pInstance->IdList);
                           instanceNode->bd_addr = msg->bd_addr;
                           instanceNode->mclId = msg->mclId;
                           /* The remote control psm is stored */
                           instanceNode->ctrlPsm = HDP_GET_CTRL_PSM( pInstance->instance[i]->hdpInstanceId ) ; ;
                           instanceNode->IsManager = FALSE;
                           instanceNode->instanceId = (CsrUint8)i;
                        }
                        bProcessEvent = TRUE;
                        break;
                    }
                    i++;
                }

            }
            break;

            case  CSR_BT_MCAP_CONNECT_MCL_IND :
            {
                HdpInstanceIdListEntry *instanceNode = HDP_INSTANCE_GET_FIRST(pInstance->IdList);
                CsrBtMcapConnectMclInd *msg = (CsrBtMcapConnectMclInd *)pInstance->recvMsgP;
                CsrBool bFound = FALSE;

                /* search the entire instance[i] to find the right instance of
                   the incoming connection based on the local PSM it wants to connect to */
                while ( i < HDP_NUM_OF_INSTANCES )
                {
                    if ( IS_VALID_PTR(pInstance->instance[i]) &&
                         (pInstance->instance[i]->psm.control == msg->localPsm ) )
                    {
                        /* this connection is meant for PSM for this instance  */
                        while ( IS_VALID_PTR(instanceNode) )
                        {
                            if(  CsrBtBdAddrEq(&instanceNode->bd_addr,&msg->bd_addr) )
                            {
                               /* there is an address match implying that the device
                                  is connecting again. possible reconnect */
                               instanceNode->instanceId = (CsrUint8)i;
                               instanceNode->mclId = msg->mclId;
                               instanceNode->ctrlPsm = msg->remotePsm;
                               bFound = TRUE;
                               break;
                            }
                            instanceNode = (HdpInstanceIdListEntry *)instanceNode->next;
                        }

                        if(bFound == FALSE )
                        {
                            /* a new connection, add mapping between the MCL connection list
                               and the Instance List*/
                            instanceNode = HDP_INSTANCE_ADD_ENTRY(pInstance->IdList);
                            instanceNode->bd_addr = msg->bd_addr;
                            instanceNode->mclId = msg->mclId;
                            instanceNode->IsManager = FALSE;
                            instanceNode->instanceId = (CsrUint8)i;
                            instanceNode->ctrlPsm = msg->remotePsm;
                        }
                        bProcessEvent = TRUE;
                        break;
                    }
                    
                    i++;
               }
            }
            break;

            case CSR_BT_MCAP_ACTIVATE_CFM :
            case CSR_BT_MCAP_DEACTIVATE_CFM :
            /* assumption : structures for activate and deactivate are the same */
            {
                CsrBtMcapActivateCfm *msg = (CsrBtMcapActivateCfm *)pInstance->recvMsgP;

                while ( (i < HDP_NUM_OF_INSTANCES) &&
                        IS_VALID_PTR(pInstance->instance[i]))
                {
                    if ( pInstance->instance[i]->psm.control == msg->psm )
                    {
                        bProcessEvent = TRUE;
                        break;
                    }
                    i++;
                }
            }
            break;

            default :
            {
                CsrBtMcapConnectMdlCfm *msg = (CsrBtMcapConnectMdlCfm *)pInstance->recvMsgP;

                /* assumption that mclId is at a fixed offset in all Mcap structures. */
                HdpInstanceIdListEntry *instanceNode = HDP_INSTANCE_GET_FIRST(pInstance->IdList);
                while ( IS_VALID_PTR(instanceNode) ) 
                {
                    if ( instanceNode->mclId == msg->mclId )
                    {
                        i = instanceNode->instanceId;
                        bProcessEvent = TRUE;
                        break;
                    }
                    instanceNode = (HdpInstanceIdListEntry *)instanceNode->next;
                }
            }
        }


        if ( (i < HDP_NUM_OF_INSTANCES) && ( bProcessEvent == TRUE) )
        {
            HdpStateHandlerType fp;

            pInstance->instance[i]->recvMsgP = pInstance->recvMsgP;
            pInstance->instance[i]->eventClass = pInstance->eventClass;
            fp = mcapHdpEvHandlers[pInstance->instance[i]->state][*primType - CSR_BT_MCAP_PRIM_UPSTREAM_LOWEST];

            if (IN_RANGE(*primType, CSR_BT_MCAP_PRIM_UPSTREAM_LOWEST, ((CSR_BT_MCAP_CTRL_PRIM_UPSTREAM_HIGHEST + 1)|CSR_BT_MCAP_PRIM_UPSTREAM_LOWEST)) &&
                IS_VALID_PTR(fp))
            {
                mcapHdpEvHandlers[pInstance->instance[i]->state][*primType - CSR_BT_MCAP_PRIM_UPSTREAM_LOWEST](pInstance->instance[i]);
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("HDP (CsrBtHdpHandler - MCAP) - Event not handled",
                            pInstance->eventClass,
                            *primType,
                            0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
            }
        }
        else
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("HDP (CsrBtHdpHandler - MCAP) - Instance not found",
                            pInstance->eventClass,
                            *primType,
                            0);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

        }
    }
    break;

    default: /* Unknown event type!?!? */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
    CsrStateEventException("HDP (BchsHdpHandler5)",
                 pInstance->eventClass,
                 *primType,
                 0);

#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
    break;

    }
    CsrPmemFree(pInstance->recvMsgP);
}



/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)    empty the input message queue and free any allocated memory in the
        messages.
    2)    free any instance data that may be allocated.
****************************************************************************/
void CsrBtHdpDeinit(void **gash)
{
    CsrUint16            msg_type=0;
    void                 *msg_data=NULL;
    HdpDb                *pInstance = *gash;
    HdpInstanceData      *pInst;
    CsrUint32            connectionCount,mdepCount,instanceCount,i,j;


    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case CSR_BT_HDP_PRIM:
                {
                    CsrBtHdpFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }
            case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            default:
                {
                    CsrStateEventException("HDP",msg_type, 0, 0);
                    break;
                }
#endif
        }
        CsrPmemFree(msg_data);
    }

    /* Free instance data and linked list data */
    for(j = 0; j < HDP_NUM_OF_INSTANCES; j++)
    {
        if(IS_VALID_PTR(pInstance->instance[j] ))
        {
            pInst = pInstance->instance[j];
            connectionCount = pInst->connectionList.count;
            {
                HdpConnectionListEntry *nodeToRemove = (HdpConnectionListEntry *)pInst->connectionList.first;
                
                for (i = 0; ((i < connectionCount) && nodeToRemove); i++)
                {
                    HdpConnectionListEntry *nextNode = nodeToRemove->next;
                    HDP_CONNECTION_DEL_ENTRY(pInst->connectionList, nodeToRemove);
                    nodeToRemove = nextNode;
                }
            }
            mdepCount = pInst->mdepList.count;
            {
                HdpMdepListEntry *nodeToRemove = (HdpMdepListEntry *)pInst->mdepList.first;

                for (i = 0; ((i < mdepCount) && nodeToRemove); i++)
                {
                    HdpMdepListEntry *nextNode = nodeToRemove->next;
                    HDP_MDEP_DEL_ENTRY(pInst->mdepList, nodeToRemove);
                    nodeToRemove = nextNode;                    
                }
            }
            CsrPmemFree(pInst->serviceDescription);
            CsrPmemFree(pInst->serviceName);
            CsrPmemFree(pInst->providerName);

            CsrPmemFree(pInst->sdpSearchData);
            CsrPmemFree(pInst);
        }
    }
    instanceCount = pInstance->IdList.count;
    {
        HdpInstanceIdListEntry *nodeToRemove = (HdpInstanceIdListEntry *)pInstance->IdList.first;
        
        for (i = 0; ((i < instanceCount) && nodeToRemove); i++)
        {
            HdpInstanceIdListEntry *nextNode = nodeToRemove->next;
            HDP_INSTANCE_DEL_ENTRY(pInstance->IdList, nodeToRemove);
            nodeToRemove = nextNode;            
        }
    }
    CsrPmemFree(pInstance);

}


