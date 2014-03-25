/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_MCAP_MODULE

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_result.h"
#include "csr_bt_mcap_private_prim.h"
#include "csr_bt_mcap_handler.h"
#include "csr_bt_mcap_sef.h"
#include "csr_bt_mcap_util.h"
#include "csr_bt_sc_private_lib.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#include "csr_bt_cm_prim.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_lib.h"

static const McapMclStateHandlerType mcapMclStateHandlers[MCAP_NUMBER_OF_MCL_STATES][CSR_BT_MCAP_PRIM_DOWNSTREAM_HIGHEST + 1] =
{
    /* MCAP_MCL_NULL_STATE */
    {
        NULL,                                   /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                   /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                   /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapInvalidDisconnectMclReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapInvalidCreateMdlResHandler,         /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapInvalidReconnectMdlReqHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapInvalidReconnectMdlResHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapInvalidConnectMdlReqHandler,        /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapInvalidDisconnectMdlReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapInvalidAbortMdlResHandler,          /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapInvalidDeleteMdlReqHandler,         /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapInvalidDeleteMdlResHandler,         /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapInvalidDataSendReqHandler,          /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapInvalidSyncCapReqHandler,           /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapInvalidSyncCapResHandler,           /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapInvalidSyncSetReqHandler,           /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapInvalidSyncSetResHandler,           /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapInvalidSyncInfoReqHandler,          /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
    /* MCAP_MCL_CONNECTING_STATE */
    {
        NULL,                                   /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                   /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                   /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapDisconnectMclReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapInvalidCreateMdlResHandler,         /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapInvalidReconnectMdlReqHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapInvalidReconnectMdlResHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapInvalidConnectMdlReqHandler,        /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapInvalidDisconnectMdlReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapInvalidAbortMdlResHandler,          /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapInvalidDeleteMdlReqHandler,         /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapInvalidDeleteMdlResHandler,         /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapInvalidDataSendReqHandler,          /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapInvalidSyncCapReqHandler,           /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapInvalidSyncCapResHandler,           /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapInvalidSyncSetReqHandler,           /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapInvalidSyncSetResHandler,           /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapInvalidSyncInfoReqHandler,          /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
    /* MCAP_MCL_CONNECTED_STATE */
    {
        NULL,                                        /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapDisconnectMclReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapCreateMdlResHandler,                /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapReconnectMdlReqHandler,             /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapReconnectMdlResHandler,             /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapConnectMdlReqHandler,               /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapDisconnectMdlReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapAbortMdlResHandler,                 /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapDeleteMdlReqHandler,                /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapDeleteMdlResHandler,                /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapInvalidDataSendReqHandler,          /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapSyncCapReqHandler,                  /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapSyncCapResHandler,                  /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapSyncSetReqHandler,                  /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapSyncSetResHandler,                  /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapSyncInfoReqHandler,                 /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
    /* MCAP_MCL_ACTIVE_STATE */
    {
        NULL,                                        /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapDisconnectMclReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapCreateMdlResHandler,                /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapReconnectMdlReqHandler,             /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapReconnectMdlResHandler,             /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapConnectMdlReqHandler,               /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapDisconnectMdlReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapAbortMdlResHandler,                 /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapDeleteMdlReqHandler,                /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapDeleteMdlResHandler,                /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapDataSendReqHandler,                 /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapSyncCapReqHandler,                  /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapSyncCapResHandler,                  /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapSyncSetReqHandler,                  /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapSyncSetResHandler,                  /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapSyncInfoReqHandler,                 /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
    /* MCAP_MCL_DISCONNECTING_STATE */
    {
        NULL,                                        /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapDisconnectMclReqHandler,            /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapInvalidCreateMdlResHandler,         /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapInvalidReconnectMdlReqHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapInvalidReconnectMdlResHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapInvalidConnectMdlReqHandler,        /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapInvalidDisconnectMdlReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapInvalidAbortMdlResHandler,          /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapInvalidDeleteMdlReqHandler,         /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapInvalidDeleteMdlResHandler,         /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapInvalidDataSendReqHandler,          /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapInvalidSyncCapReqHandler,           /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapInvalidSyncCapResHandler,           /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapInvalidSyncSetReqHandler,           /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapInvalidSyncSetResHandler,           /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapInvalidSyncInfoReqHandler,          /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
    /* MCAP_MCL_DEACTIVATING_STATE */
    {
        NULL,                                        /* CSR_BT_MCAP_ACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_DEACTIVATE_REQ */
        NULL,                                        /* CSR_BT_MCAP_CONNECT_MCL_REQ */
        CsrBtMcapInvalidDisconnectMclReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MCL_REQ */
        CsrBtMcapInvalidCreateMdlResHandler,         /* CSR_BT_MCAP_CREATE_MDL_RES */
        CsrBtMcapInvalidReconnectMdlReqHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_REQ */
        CsrBtMcapInvalidReconnectMdlResHandler,      /* CSR_BT_MCAP_RECONNECT_MDL_RES */
        CsrBtMcapInvalidConnectMdlReqHandler,        /* CSR_BT_MCAP_CONNECT_MDL_REQ */
        CsrBtMcapInvalidDisconnectMdlReqHandler,     /* CSR_BT_MCAP_DISCONNECT_MDL_REQ */
        CsrBtMcapInvalidAbortMdlResHandler,          /* CSR_BT_MCAP_ABORT_MDL_RES */
        CsrBtMcapInvalidDeleteMdlReqHandler,         /* CSR_BT_MCAP_DELETE_MDL_REQ */
        CsrBtMcapInvalidDeleteMdlResHandler,         /* CSR_BT_MCAP_DELETE_MDL_RES */
        CsrBtMcapInvalidDataSendReqHandler,          /* CSR_BT_MCAP_DATA_SEND_REQ */
        CsrBtMcapInvalidSyncCapReqHandler,           /* CSR_BT_MCAP_SYNC_CAP_REQ */
        CsrBtMcapInvalidSyncCapResHandler,           /* CSR_BT_MCAP_SYNC_CAP_RES */
        CsrBtMcapInvalidSyncSetReqHandler,           /* CSR_BT_MCAP_SYNC_SET_REQ */
        CsrBtMcapInvalidSyncSetResHandler,           /* CSR_BT_MCAP_SYNC_SET_RES */
        CsrBtMcapInvalidSyncInfoReqHandler,          /* CSR_BT_MCAP_SYNC_INFO_REQ */
    },
};



/* --------- CM jump table ---------- */
static const McapMclStateHandlerType cmMclStateHandlers[MCAP_NUMBER_OF_MCL_STATES][CSR_BT_CM_L2CA_PRIM_UPSTREAM_COUNT] =
{
    /* MCAP_MCL_NULL_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndIgnore,                    /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* MCAP_MCL_CONNECTING_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmL2caConnectCfmConnectingMclHandler,   /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmL2caDataCfmHandler,                   /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndHandler,                   /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* MCAP_MCL_CONNECTED_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmL2caConnectCfmHandler,                /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmL2caDataCfmHandler,                   /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndHandler,                   /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* MCAP_MCL_ACTIVE_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmL2caConnectCfmHandler,                /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmL2caDataCfmHandler,                   /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndHandler,                   /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* MCAP_MCL_DISCONNECTING_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmL2caConnectCfmDisconnectingMclHandler,/* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndIgnore,                    /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
    /* MCAP_MCL_DEACTIVATING_STATE */
    {
        NULL,                                            /* CSR_BT_CM_L2CA_REGISTER_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_CONNECT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */
        NULL,                                            /* CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_DATA_CFM */
        CsrBtMcapCmL2caDataIndIgnore,                    /* CSR_BT_CM_L2CA_DATA_IND */
        NULL,                                            /* CSR_BT_CM_L2CA_DISCONNECT_IND */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_ACCEPT_CONNECT_CFM */
        CsrBtMcapCmIgnoreMsg,                            /* CSR_BT_CM_L2CA_SCO_DISCONNECT_IND */
        CsrBtMcapCmL2caModeChangeIndHandler,             /* CSR_BT_CM_L2CA_MODE_CHANGE_IND */
    },
};
/* ---------- End of jump tables ---------- */


void CsrBtMcapInit(void **gash)
{
    McapInstanceData *pInst;

    *gash = CsrPmemAlloc(sizeof(McapInstanceData));
    pInst = *gash;

    CsrBtScSetSecOutLevel(&pInst->secOutgoing, CSR_BT_SEC_DEFAULT,
        CSR_BT_MCAP_MANDATORY_SECURITY_OUTGOING,
        CSR_BT_MCAP_DEFAULT_SECURITY_OUTGOING,
        CSR_BT_RESULT_CODE_MCAP_SUCCESS,
        CSR_BT_RESULT_CODE_MCAP_UNACCEPTABLE_PARAMETER);

    CsrBtScSetSecInLevel(&pInst->secIncoming, CSR_BT_SEC_DEFAULT,
        CSR_BT_MCAP_MANDATORY_SECURITY_INCOMING,
        CSR_BT_MCAP_DEFAULT_SECURITY_INCOMING,
        CSR_BT_RESULT_CODE_MCAP_SUCCESS,
        CSR_BT_RESULT_CODE_MCAP_UNACCEPTABLE_PARAMETER);
    CsrCmnListInit(&pInst->mclList, 0, CsrBtMcapUtilCmnListMclAdd, CsrBtMcapUtilCmnListMclRemove);
    CsrCmnListInit(&pInst->psmList, 0, CsrBtMcapUtilCmnListPsmAdd, CsrBtMcapUtilCmnListPsmRemove);

    pInst->currentLocalMode = CSR_BT_ACTIVE_MODE;
    pInst->requestedMode = CSR_BT_ACTIVE_MODE;
    pInst->sniffTimeout = 1; /* Sniff timeout is set to the lowest possible value. Zero means that sniff is never requested */
    pInst->timerId = 0;
}

void CsrBtMcapHandler(void **gash)
{
    CsrUint16 eventClass=0;
    McapInstanceData *pInst = *gash;

    CsrSchedMessageGet(&eventClass, &pInst->recvMsgP);

    switch(eventClass)
    {
        case CSR_BT_MCAP_PRIM:
            {
                McapPrimType *type = pInst->recvMsgP;

                if ((*type < CSR_BT_MCAP_PRIM_UPSTREAM_HIGHEST) && (*type > CSR_PRIM_UPSTREAM))
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrStateEventException("MCAP (BchsMcapHandler1)",
                                            eventClass,
                                            *type,
                                            MCAP_UNDEFINED_STATE);
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                }
                else if (*type <= CSR_BT_MCAP_PRIM_DOWNSTREAM_HIGHEST)
                {
                    if (*type == CSR_BT_MCAP_ACTIVATE_REQ)
                    {
                        CsrBtMcapActivateReqHandler(pInst);
                    }
                    else if (*type == CSR_BT_MCAP_DEACTIVATE_REQ)
                    {
                        CsrBtMcapDeactivateReqHandler(pInst);
                    }
                    else if (*type == CSR_BT_MCAP_CONNECT_MCL_REQ)
                    {
                        CsrBtMcapConnectMclReqHandler(pInst);
                    }
                    else
                    {
                        CsrUint8 mclState;

                        mclState = getMclState(pInst, CSR_BT_MCAP_PRIM);
                        if (mclState == MCAP_UNDEFINED_STATE)
                        {
                            CSR_BT_MCAP_STATE_CHANGE(mclState, MCAP_MCL_NULL_STATE);
                        }

                        /*Based on the MCL state, find the correct handler function */
                        if ((mcapMclStateHandlers[mclState - MCAP_MCL_NULL_STATE][*type] != NULL) && (mclState != MCAP_UNDEFINED_STATE))
                        {
                            mcapMclStateHandlers[mclState - MCAP_MCL_NULL_STATE][*type](pInst);
                        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        else
                        {
                            CsrStateEventException("MCAP (BchsMcapHandler2)",
                                                    eventClass,
                                                    *type,
                                                    mclState);
                        }
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */

                    }
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                else
                {   /*  State/Event ERROR! */
                    CsrStateEventException("MCAP (BchsMcapHandler3)",
                        eventClass,
                        *type,
                        MCAP_UNDEFINED_STATE);
                }
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                break;
            }
        case CSR_BT_CM_PRIM :
        {
            CsrPrim *type = pInst->recvMsgP;

            if (*type == CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM)
            {
                CsrBtMcapCmL2caCancelConnectAcceptCfmHandler(pInst);
            }
            else if (*type == CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM)
            {
                CsrBtMcapCmL2caConnectAcceptCfmHandler(pInst);
            }
            else if (*type == CSR_BT_CM_L2CA_DISCONNECT_IND)
            {
                CsrBtMcapCmL2caDisconnectIndHandler(pInst);
            }
            else if (*type == CSR_BT_CM_L2CA_MODE_CHANGE_IND)
            {
                CsrBtMcapCmL2caModeChangeIndHandler(pInst);
            }
            else if (((CsrUint16)*type >= CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST) && ((CsrUint16)*type <= CSR_BT_CM_L2CA_PRIM_UPSTREAM_HIGHEST))
            {
                CsrUint8 mclState;

                mclState = getMclState(pInst, CSR_BT_CM_PRIM);
                if ((cmMclStateHandlers[mclState - MCAP_MCL_NULL_STATE][(CsrUint16) (*type - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST)] != NULL) && (mclState != MCAP_UNDEFINED_STATE))
                {
                    cmMclStateHandlers[mclState - MCAP_MCL_NULL_STATE][(CsrUint16) (*type - CSR_BT_CM_L2CA_PRIM_UPSTREAM_LOWEST)](pInst);
                }
                else if (mclState == MCAP_UNDEFINED_STATE)
                {
                    CsrBtMcapCmIgnoreMsg(pInst);
                }
                else
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("MCAP (BchsMcapHandler4)",
                                        eventClass,
                                        *type,
                                        mclState);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
            }

            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("MCAP (BchsMcapHandler5)",
                                        eventClass,
                                        *type,
                                        MCAP_UNDEFINED_STATE);
#endif /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, pInst->recvMsgP);
            }
            break;
        }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        default: /* Unknown event type!?!? */
        CsrStateEventException("MCAP (BchsMcapHandler6)",
                                eventClass,
                                0,
                                MCAP_UNDEFINED_STATE);
        break;
#endif    /* EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE */
    }
    CsrPmemFree(pInst->recvMsgP);
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
void CsrBtMcapDeinit(void **gash)
{
    CsrUint16            msg_type=0;
    void                *msg_data=NULL;
    McapInstanceData    *pInst = *gash;
    McapMclListEntry    *currentMclPtr = NULL;
    CsrUint32            mclCount, mdlCount, psmCount;
    CsrUint32            i, j;

    if(pInst->timerId)
    {
        CsrSchedTimerCancel(pInst->timerId, NULL, NULL);
        pInst->timerId = 0;
    }

    while (CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case CSR_BT_MCAP_PRIM:
                {
                    CsrBtMcapFreeUpstreamMessageContents(msg_data);
                    CsrBtMcapPrivateFreeUpstreamMessageContents(CSR_BT_MCAP_PRIVATE_PRIM,msg_data);
                    break;
                }
            case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg_data);
                    break;
                }
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            default:
                {
                    CsrStateEventException("MCAP",msg_type, 0, 0);
                    break;
                }
#endif
        }
        CsrPmemFree(msg_data);
    }

    /* Free instance data and linked list data */
    psmCount = pInst->psmList.count;
    {
        McapPsmListEntry *psmNodeToRemove = MCAP_PSM_GET_FIRST(pInst->psmList);
        for (i = 0; ((i < psmCount) && psmNodeToRemove); i++)
        {
            McapPsmListEntry *tmpNode = psmNodeToRemove->next;
            MCAP_PSM_REMOVE_ENTRY(pInst->psmList, psmNodeToRemove);
            psmNodeToRemove = tmpNode;
        }
    }
    mclCount = pInst->mclList.count;
    currentMclPtr = (McapMclListEntry *)pInst->mclList.first;
    
    for (i = 0; ((i < mclCount) && currentMclPtr); i++)
    {
        McapMclListEntry *nextMclListPtr = currentMclPtr->next;
        McapMdlListEntry *nodeToRemove = (McapMdlListEntry *)currentMclPtr->mdlList.first;

        mdlCount = currentMclPtr->mdlList.count;
           
        for (j = 0; ((j < mdlCount) && nodeToRemove); j++)
        {
            McapMdlListEntry *nextNode = nodeToRemove->next;
            MCAP_MDL_REMOVE_ENTRY(currentMclPtr->mdlList, nodeToRemove);
            nodeToRemove = nextNode;
        }
        
        MCAP_MCL_REMOVE_ENTRY(pInst->mclList, currentMclPtr);
        
        currentMclPtr  = nextMclListPtr;
    }

    CsrPmemFree(pInst);
}

#endif  /* ENABLE_SHUTDOWN */
#endif  /* EXCLUDE_CSR_BT_MCAP_MODULE */
