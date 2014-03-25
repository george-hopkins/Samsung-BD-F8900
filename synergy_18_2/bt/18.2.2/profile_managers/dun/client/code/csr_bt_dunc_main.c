/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_dunc_main.h"
#include "csr_bt_dunc_prim.h"
#include "csr_bt_dunc_sef.h"
#include "csr_env_prim.h"
#include "csr_pmem.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_sd_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

/* forward declarations */
static void handleCleanUp(DuncInstanceDataType * instData);

/* -------- DUNC Message Jump Table --------- */
static const DuncStateHandlerType duncStateHandlers[DUNC_NUMBER_OF_DOWNSTREAM_STATES][CSR_BT_DUNC_PRIM_DOWNSTREAM_COUNT] =
{
    /* DUNC_DISCONNECTING_STATE */
    {
        NULL,                                                /* CSR_BT_DUNC_CONNECT_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_CANCEL_CONNECT_REQ              */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_REQ                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_RES                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_CONTROL_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_PORTNEG_RES                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_PORTNEG_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DISCONNECT_REQ                  */
        CsrBtDuncXstateRegisterDataPathHandleReqHandler,     /* CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ   */
        CsrBtDuncXStateChangeDataPathStatusReqHandler,       /* CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ     */
        CsrBtDuncXSecurityOutReqHandler,                     /* CSR_BT_DUNC_SECURITY_OUT_REQ                */
    },
    /* DUNC_CONNECTED_STATE */
    {
        NULL,                                                /* CSR_BT_DUNC_CONNECT_REQ                     */
        CsrBtDuncConnectedStateDisconnectReqHandler,         /* CSR_BT_DUNC_CANCEL_CONNECT_REQ              */
        CsrBtDuncConnectedStateDataReqHandler,               /* CSR_BT_DUNC_DATA_REQ                        */
        CsrBtDuncConnectedStateDataResHandler,               /* CSR_BT_DUNC_DATA_RES                        */
        CsrBtDuncConnectedStateControlReqHandler,            /* CSR_BT_DUNC_CONTROL_REQ                     */
        CsrBtDuncConnectedStatePortnegResHandler,            /* CSR_BT_DUNC_PORTNEG_RES                     */
        CsrBtDuncConnectedStatePortnegReqHandler,            /* CSR_BT_DUNC_PORTNEG_REQ                     */
        CsrBtDuncConnectedStateDisconnectReqHandler,         /* CSR_BT_DUNC_DISCONNECT_REQ                  */
        CsrBtDuncXstateRegisterDataPathHandleReqHandler,     /* CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ   */
        CsrBtDuncXStateChangeDataPathStatusReqHandler,       /* CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ     */
        CsrBtDuncXSecurityOutReqHandler,                     /* CSR_BT_DUNC_SECURITY_OUT_REQ                */
    },
    /* DUNC_CONNECT_STATE */
    {
        NULL,                                                /* CSR_BT_DUNC_CONNECT_REQ                     */
        CsrBtDuncConnectStateCancelConnectReqHandler,        /* CSR_BT_DUNC_CANCEL_CONNECT_REQ              */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_REQ                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_RES                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_CONTROL_REQ                     */
        CsrBtDuncConnectStatePortnegResHandler,              /* CSR_BT_DUNC_PORTNEG_RES                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_PORTNEG_REQ                     */
        CsrBtDuncConnectStateCancelConnectReqHandler,        /* CSR_BT_DUNC_DISCONNECT_REQ                  */
        CsrBtDuncXstateRegisterDataPathHandleReqHandler,     /* CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ   */
        CsrBtDuncXStateChangeDataPathStatusReqHandler,       /* CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ     */
        CsrBtDuncXSecurityOutReqHandler,                     /* CSR_BT_DUNC_SECURITY_OUT_REQ                */
    },
    /* DUNC_IDLE_STATE */
    {
        CsrBtDuncIdleStateConnectReqHandler,                 /* CSR_BT_DUNC_CONNECT_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_CANCEL_CONNECT_REQ              */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_REQ                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DATA_RES                        */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_CONTROL_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_PORTNEG_RES                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_PORTNEG_REQ                     */
        CsrBtDuncIgnoreMsg,                                  /* CSR_BT_DUNC_DISCONNECT_REQ                  */
        CsrBtDuncXstateRegisterDataPathHandleReqHandler,     /* CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_REQ   */
        CsrBtDuncXStateChangeDataPathStatusReqHandler,       /* CSR_BT_DUNC_CHANGE_DATA_PATH_STATUS_REQ     */
        CsrBtDuncXSecurityOutReqHandler,                     /* CSR_BT_DUNC_SECURITY_OUT_REQ                */
    },
};

/* -------- DUNC Message Jump Table --------- */
static const DuncStateHandlerType cmStateHandlers[DUNC_NUMBER_OF_UPSTREAM_STATES][CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT] =
{
     /* DUNC_DISCONNECTING_STATE */
    {
        NULL,                                               /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM         */
        NULL,                                               /* CSR_BT_CM_CONNECT_CFM                       */
        NULL,                                               /* CSR_BT_CM_CONNECT_ACCEPT_CFM                */
        NULL,                                               /* CSR_BT_CM_REGISTER_CFM                      */
        CsrBtDuncDisconnectingStateCmDisconnectIndHandler,  /* CSR_BT_CM_DISCONNECT_IND                    */
        NULL,                                               /* CSR_BT_CM_SCO_CONNECT_CFM                   */
        NULL,                                               /* CSR_BT_CM_SCO_DISCONNECT_IND                */
        NULL,                                               /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM            */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_DATA_IND                          */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_DATA_CFM                          */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_CONTROL_IND                       */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_RFC_MODE_CHANGE_IND               */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_PORTNEG_IND                       */
        CsrBtDuncXStateIgnoreCmMsgHandler,                  /* CSR_BT_CM_PORTNEG_CFM                       */
    },
    /* DUNC_CONNECTED_STATE */
    {
        NULL,                                           /* CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM         */
        NULL,                                           /* CSR_BT_CM_CONNECT_CFM                       */
        NULL,                                           /* CSR_BT_CM_CONNECT_ACCEPT_CFM                */
        NULL,                                           /* CSR_BT_CM_REGISTER_CFM                      */
        CsrBtDuncConnectedStateCmDisconnectIndHandler,  /* CSR_BT_CM_DISCONNECT_IND                    */
        NULL,                                           /* CSR_BT_CM_SCO_CONNECT_CFM                   */
        NULL,                                           /* CSR_BT_CM_SCO_DISCONNECT_IND                */
        NULL,                                           /* CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM            */
        CsrBtDuncConnectedStateCmDataIndHandler,        /* CSR_BT_CM_DATA_IND                          */
        CsrBtDuncConnectedStateCmDataCfmHandler,        /* CSR_BT_CM_DATA_CFM                          */
        CsrBtDuncConnectedStateCmControlIndHandler,     /* CSR_BT_CM_CONTROL_IND                       */
        CsrBtDuncConnectedStateCmModeChangeIndHandler,  /* CSR_BT_CM_RFC_MODE_CHANGE_IND               */
        CsrBtDuncConnectedStateCmPortNegIndHandler,     /* CSR_BT_CM_PORTNEG_IND                       */
        CsrBtDuncConnectedStateCmPortNegCfmHandler,     /* CSR_BT_CM_PORTNEG_CFM                       */
    },
};
/* -------- no more jump tables --------- */



/* init function to be called by the scheduler during Synergy BT initialisation */
void CsrBtDuncInit(void ** gash)
{
    DuncInstanceDataType  * instData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(DuncInstanceDataType));
    instData = (DuncInstanceDataType *) * gash;

    /* clear own instance data for first time use */
    instData->connectInst = CsrBtUtilSdpRfcConInit(NULL, CsrBtDuncPortNegIndHandler,
                                    CsrBtDuncConnectResultHandler, CSR_BT_DUNC_IFACEQUEUE);

    /* Tell the SD that it must look for the CSR_BT_DUN_PROFILE_UUID service, when it perform a
       SD_READ_AVAILABLE_SERVICE_REQ                                                            */
    CsrBtSdRegisterAvailableServiceReqSend(CSR_BT_DUN_PROFILE_UUID);
    CsrBtDuncInitInstanceData(instData);
}

/* handler function to be called by the scheduler, whenever a message is available for DUN Client */
void CsrBtDuncHandler(void ** gash)
{
    DuncInstanceDataType    * duncData;
    CsrUint16                  eventClass=0;
    void                    * msg=NULL;

    duncData = (DuncInstanceDataType *) (* gash);

    CsrSchedMessageGet(&eventClass, &msg);

    duncData->recvMsgP = msg;

    switch(eventClass)
    {
        case CSR_BT_DUNC_PRIM:
        {
            CsrBtDuncPrim * primType = (CsrBtDuncPrim *)duncData->recvMsgP;

            if ((*primType < CSR_BT_DUNC_PRIM_DOWNSTREAM_COUNT) &&
                (duncStateHandlers[duncData->state][*primType] != NULL))
            {
                duncStateHandlers[duncData->state][*primType](duncData);
            }
            else
            { /* state - event error! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("DUNC", eventClass, *primType, duncData->state);
#endif
                CsrBtDuncReleaseMessage(CSR_BT_DUNC_PRIM, duncData->recvMsgP);
            }
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrPrim * primType = (CsrPrim *)duncData->recvMsgP;
            CsrUint16     index      = (CsrUint16) (*primType - CSR_BT_CM_RFC_PRIM_UPSTREAM_LOWEST);

            if (((CsrUint16)(index) < CSR_BT_CM_RFC_PRIM_UPSTREAM_COUNT) &&
                duncData->state < DUNC_NUMBER_OF_UPSTREAM_STATES    &&
                (cmStateHandlers[duncData->state][index] != NULL))
            {
                cmStateHandlers[duncData->state][index](duncData);
            }
            else if (duncData->state == DUNC_CONNECT_STATE &&
                     CsrBtUtilRfcConVerifyCmMsg(duncData->recvMsgP))
            {
                CsrBtUtilRfcConCmMsgHandler(duncData, duncData->connectInst, duncData->recvMsgP);
            }
            else
            {
                if (*primType != CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM)
                {  /* state - event error! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("DUNC", eventClass, *primType, duncData->state);
#endif
                    CsrBtDuncReleaseMessage(CSR_BT_CM_PRIM, duncData->recvMsgP);
                }
            }
            break;
        }
        case CSR_SCHED_PRIM:
        {
            CsrEnvPrim * primType;
            primType = (CsrEnvPrim *) duncData->recvMsgP;
            switch(*primType)
            {
                case CSR_CLEANUP_IND:
                {
                    CsrCleanupInd * prim = (CsrCleanupInd*) duncData->recvMsgP;
                    if(prim->phandle == duncData->ctrlHandle)
                    {
                        handleCleanUp(duncData);
                    }
                    else if(prim->phandle == duncData->dataHandle)
                    { /* do nothing - only unregister dataHandle and inform the ctrl part about data path loss*/
                        CsrBtDuncSendDataPathStatusInd(duncData, CSR_BT_DATA_PATH_STATUS_LOST, CSR_BT_DUNC_IFACEQUEUE,
                            CSR_BT_RESULT_CODE_DUNC_UNSPECIFIED_ERROR, CSR_BT_SUPPLIER_DUNC);
                        duncData->dataHandle = CSR_SCHED_QID_INVALID;
                    }
                    else
                    { /* do nothing - the cleanup appHandle was not for me */
                        ;
                    }
                    break;
                }
                default:
                { /* state - event error! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("DUNC", eventClass, *primType, duncData->state);
#endif
                    break;
                }
            }
            break;
        }
        default:
        { /* state - event error! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrStateEventException("DUNC", eventClass, 0xFF, duncData->state);
#endif
            break;
        }
    }
    CsrPmemFree(duncData->recvMsgP);  /* free the received msg, if the ptr is NULL the CsrSched just ignores it */
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
void CsrBtDuncDeinit(void ** gash)
{
    DuncInstanceDataType * duncData;
    duncData = (DuncInstanceDataType *) * gash;

    CsrBtDuncCleanupQueue();
    CsrBtUtilSdcRfcDeinit(&(duncData->connectInst));
    CsrPmemFree(duncData);
}
#endif /* ENABLE_SHUTDOWN */


/* local functions needed for the profile handler */

/*---------------------------------------------------------------------------
 *  handleCleanUp
 *--------------------------------------------------------------------------*/
static void handleCleanUp(DuncInstanceDataType * instData)
{
    switch(instData->state)
    {
        case DUNC_CONNECT_STATE:
        {
            instData->doingCleanup = TRUE;
            CsrBtUtilRfcConCancel(instData, instData->connectInst);
            break;
        }
        case DUNC_CONNECTED_STATE:
        {
            instData->doingCleanup = TRUE;
            CsrBtDuncConnectedStateDisconnectReqHandler(instData);
            break;
        }
        case DUNC_DISCONNECTING_STATE:
        {
            instData->doingCleanup = TRUE;
            break;
        }
        case DUNC_IDLE_STATE:
        { /* just ignore the msg as no apps have registered yet */
            break;
        }
        default:
        { /* unknown state - should never happen                */
            break;
        }
    }
}
