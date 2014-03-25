/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_util.h"
#include "csr_log_text_2.h"

/* CSR_IP_SOCKET Interface */
#include "csr_ip_socket_task.h"
#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

/* CSR_UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR_APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* TCP Transceiver Headers */
#include "csr_tcp_transceiver_task.h"
#include "csr_tcp_transceiver_handler.h"
#include "csr_tcp_transceiver_sef.h"

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrTcpTransceiverLto);

/* Jump table for CSR_IP_SOCKET upstream messages */
typedef void (*msgHandler)(CsrTcpTransceiverInstanceData *instanceData);
static const msgHandler ipSocketHandlers[CSR_IP_SOCKET_PRIM_UPSTREAM_COUNT] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    CsrTcpTransceiverIpSocketTcpNewCfmHandler,          /* CSR_IP_SOCKET_TCP_NEW_CFM */
    CsrTcpTransceiverIpSocketTcpBindCfmHandler,         /* CSR_IP_SOCKET_TCP_BIND_CFM */
    CsrTcpTransceiverIpSocketTcpListenCfmHandler,       /* CSR_IP_SOCKET_TCP_LISTEN_CFM */
    CsrTcpTransceiverIpSocketTcpConnectCfmHandler,      /* CSR_IP_SOCKET_TCP_CONNECT_CFM */
    CsrTcpTransceiverIpSocketTcpAcceptIndHandler,       /* CSR_IP_SOCKET_TCP_ACCEPT_IND */
    CsrTcpTransceiverIpSocketTcpDataCfmHandler,         /* CSR_IP_SOCKET_TCP_DATA_CFM */
    CsrTcpTransceiverIpSocketTcpDataIndHandler,         /* CSR_IP_SOCKET_TCP_DATA_IND */
    CsrTcpTransceiverIpSocketTcpCloseIndHandler,        /* CSR_IP_SOCKET_TCP_CLOSE_IND */
    NULL,
    NULL,
    NULL,
    NULL,
    CsrTcpTransceiverIpSocketOptionsCfmHandler,         /* CSR_IP_SOCKET_OPTIONS_CFM */
};

/* Jump table for CSR_APP upstream messages */
static const msgHandler appHandlers[CSR_APP_PRIM_UPSTREAM_COUNT] =
{
    CsrTcpTransceiverAppRegisterCfmHandler,             /* CSR_APP_REGISTER_CFM */
    CsrTcpTransceiverAppTakeControlIndHandler,          /* CSR_APP_TAKE_CONTROL_IND */
};

/* Jump table for CSR_UI upstream messages */
static const msgHandler uiHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] =
{
    CsrTcpTransceiverUiUieCreateCfmHandler,             /* CSR_UI_UIE_CREATE_CFM */
    NULL,                                               /* CSR_UI_MENU_GET_CFM */
    NULL,                                               /* CSR_UI_MENU_GETITEM_CFM */
    NULL,                                               /* CSR_UI_MENU_GETCURSOR_CFM */
    NULL,                                               /* CSR_UI_EVENT_GET_CFM */
    NULL,                                               /* CSR_UI_DIALOG_GET_CFM */
    CsrTcpTransceiverUiInputdialogGetCfmHandler,        /* CSR_UI_INPUTDIALOG_GET_CFM */
    NULL,                                               /* CSR_UI_IDLESCREEN_GET_CFM */
    NULL,                                               /* CSR_UI_DISPLAY_GETHANDLE_CFM */
    NULL,                                               /* CSR_UI_KEYDOWN_IND */
    CsrTcpTransceiverUiEventIndHandler,                 /* CSR_UI_EVENT_IND */
};

void CsrTcpTransceiverInit(void **gash)
{
    CsrTcpTransceiverInstanceData *instanceData;

    instanceData = *gash = CsrPmemZalloc(sizeof(CsrTcpTransceiverInstanceData));

    instanceData->srvTimerId = CSR_SCHED_TID_INVALID;
    instanceData->cliTimerId = CSR_SCHED_TID_INVALID;

    /* Start UI creation cycle */
    CsrUiUieCreateReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);

    CSR_LOG_TEXT_REGISTER(&CsrTcpTransceiverLto, "TCP_TRANSCEIVER", 0, NULL);
}

void CsrTcpTransceiverHandler(void **gash)
{
    CsrTcpTransceiverInstanceData *instanceData;
    CsrUint16 event;
    void *message;

    instanceData = (CsrTcpTransceiverInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        instanceData->message = message;

        switch (event)
        {
            case CSR_IP_SOCKET_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) instanceData->message;
                CsrUint16 prim = (*type) - CSR_IP_SOCKET_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_IP_SOCKET_PRIM_UPSTREAM_COUNT) && (ipSocketHandlers[prim] != NULL))
                {
                    ipSocketHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTcpTransceiverLto, 0, event, *type);
                }
                break;
            }
            case CSR_UI_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;
                CsrUint16 prim = (*type) - CSR_UI_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_UI_PRIM_UPSTREAM_COUNT) && (uiHandlers[prim] != NULL))
                {
                    uiHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTcpTransceiverLto, 0, event, *type);
                }
                break;
            }
            case CSR_APP_PRIM:
            {
                CsrUint16 *type = (CsrUint16 *) message;
                CsrUint16 prim = (*type) - CSR_APP_PRIM_UPSTREAM_LOWEST;

                if ((prim < CSR_APP_PRIM_UPSTREAM_COUNT) && (appHandlers[prim] != NULL))
                {
                    appHandlers[prim](instanceData);
                }
                else
                {
                    CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTcpTransceiverLto, 0, event, *type);
                }
                break;
            }
            default:
            {
                CSR_LOG_TEXT_UNHANDLED_PRIMITIVE(CsrTcpTransceiverLto, 0, event, 0);
                break;
            }
        }

        /* Free the message itself */
        CsrPmemFree(instanceData->message);
        instanceData->message = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrTcpTransceiverDeinit(void **gash)
{
    CsrTcpTransceiverInstanceData *instanceData;
    void *message;
    CsrUint16 event;

    instanceData = (CsrTcpTransceiverInstanceData *) *gash;

    /* Stop timers */
    CsrSchedTimerCancel(instanceData->srvTimerId, NULL, NULL);
    CsrSchedTimerCancel(instanceData->cliTimerId, NULL, NULL);

    /* Clean own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_IP_SOCKET_PRIM:
                CsrIpSocketFreeUpstreamMessageContents(event, message);
                break;
            case CSR_APP_PRIM:
                /* No allocated content in upstream messages */
                break;
            case CSR_UI_PRIM:
                CsrUiFreeUpstreamMessageContents(event, message);
                break;
            default:
                break;
        }
        CsrPmemFree(message);
    }

    /* Free instance data */
    CsrPmemFree(instanceData);
}

#endif
