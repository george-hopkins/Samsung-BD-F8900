/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_types.h"
#include "csr_formatted_io.h"
#include "csr_unicode.h"
#include "csr_log_text_2.h"

/* CSR_IP_SOCKET Interface */
#include "csr_ip_socket_task.h"
#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

/* CSR UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* Transceiver Headers */
#include "csr_tcp_transceiver_task.h"
#include "csr_tcp_transceiver_handler.h"
#include "csr_tcp_transceiver_sef.h"

#define MEASURE_TIME_MICRO 2000000

#define MENU_CATEGORY "Network"
#define MENU_ITEM_NAME "TCP Transceiver"

#define MENU_ITEM_CLI_DISABLED_ICON CSR_UI_ICON_MARK_DELETE
#define MENU_ITEM_CLI_ENABLED_ICON CSR_UI_ICON_MARK_ACCEPT
#define MENU_ITEM_CLI_TEXT "Client"
#define MENU_ITEM_CLI_DISABLED_SUBTEXT "Inactive"
#define MENU_ITEM_CLI_CONNECTING_SUBTEXT "Connecting"

#define MENU_ITEM_SRV_DISABLED_ICON CSR_UI_ICON_MARK_DELETE
#define MENU_ITEM_SRV_ENABLED_ICON CSR_UI_ICON_MARK_ACCEPT
#define MENU_ITEM_SRV_TEXT "Server"
#define MENU_ITEM_SRV_DISABLED_SUBTEXT "Inactive"

#define MENU_ITEM_CLI_KEY 0
#define MENU_ITEM_SRV_KEY 1

#define OPT_MENU_ITEM_REMOTE_IP_ADDRESS "Remote IP Address"
#define OPT_MENU_ITEM_REMOTE_IP_ADDRESS_ICON CSR_UI_ICON_SERVER

#define OPT_MENU_ITEM_TX_SIZE "Transmit Size"
#define OPT_MENU_ITEM_TX_SIZE_ICON CSR_UI_ICON_TOOLS

#define OPT_MENU_ITEM_NAGLE "Nagle"
#define OPT_MENU_ITEM_NAGLE_ICON CSR_UI_ICON_TOOLS

#define OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY 0
#define OPT_MENU_ITEM_TX_SIZE_KEY 1
#define OPT_MENU_ITEM_NAGLE_KEY 2

#define SRV_LISTEN_PORT 50000

#define REMOTE_IP_ADDRESS_STRING_LENGTH 15
static CsrUint8 remoteIpAddress[16] = {127, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#define TX_SIZE_STRING_LENGTH 5
static CsrUint16 txSize = 8 * 1024;
static CsrBool nagle = TRUE;

static CsrCharString *extractNumber(CsrCharString *str, CsrUint8 *number)
{
    CsrUint8 i = 0;

    while (str[i] != '.' && str[i] != '\0')
    {
        i++;
    }

    switch (i)
    {
        case 1:
            *number = (str[0] - 48);
            return str + i;
        case 2:
            *number = (str[0] - 48) * 10 + (str[1] - 48);
            return str + i;
        case 3:
            *number = (str[0] - 48) * 100 + (str[1] - 48) * 10 + (str[2] - 48);
            return str + i;
        default:
            *number = 0;
            return str;
    }
}

static void stringToIpAddress(CsrCharString *ipAddressString, CsrUint8 ipAddress[16])
{
    CsrUint8 i;
    for (i = 0; i < 4; i++)
    {
        ipAddressString = extractNumber(ipAddressString, &ipAddress[i]);
        if (ipAddressString[0] != '.')
        {
            break;
        }
        ipAddressString++;
    }
}

static CsrUtf16String *ipAddressToString(CsrUint8 ipAddress[4])
{
    CsrCharString rias[REMOTE_IP_ADDRESS_STRING_LENGTH + 1];
    CsrSnprintf(rias, sizeof(rias), "%u.%u.%u.%u", ipAddress[0], ipAddress[1], ipAddress[2], ipAddress[3]);
    return CsrUtf82Utf16String((CsrUtf8String *) rias);
}

static CsrUtf16String *nagleToString(void)
{
    if (nagle)
    {
        return CsrUtf82Utf16String((CsrUtf8String *) "Enabled");
    }
    else
    {
        return CsrUtf82Utf16String((CsrUtf8String *) "Disabled");
    }
}

static void transmitFrame(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrUint8 *data = CsrPmemZalloc(txSize);
    CsrIpSocketTcpDataReqSend2(instanceData->cliSocketHandle, txSize, data);
    instanceData->cliFrameSize += txSize;
}

static void cliMeasureUpdate(CsrUint16 mi, void *mv)
{
    CsrTcpTransceiverInstanceData *instanceData = (CsrTcpTransceiverInstanceData *) mv;

    if (instanceData->cliState == STATE_ON)
    {
        CsrCharString infoString[16];
        CsrUint32 throughput = 0;
        CsrTime now = CsrTimeGet(NULL);
        CsrTime elapsedMs = (100 * (now - instanceData->cliStartTime)) / 100000;

        if (elapsedMs > 0)
        {
            throughput = (8 * instanceData->cliFrameSize) / elapsedMs;
        }
        CsrSnprintf(infoString, sizeof(infoString), "%ukbit/s", throughput);
        CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_ENABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT), CsrUtf82Utf16String((CsrUtf8String *) infoString), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);

        instanceData->cliFrameSize = 0;
        instanceData->cliStartTime = now;

        instanceData->cliTimerId = CsrSchedTimerSet(MEASURE_TIME_MICRO, cliMeasureUpdate, 0, mv);
    }
    else
    {
        instanceData->cliTimerId = CSR_SCHED_TID_INVALID;
    }
}

static void srvMeasureUpdate(CsrUint16 mi, void *mv)
{
    CsrTcpTransceiverInstanceData *instanceData = mv;

    if (instanceData->srvState == STATE_ON)
    {
        CsrCharString infoString[16];
        CsrUint32 throughput = 0;
        CsrTime now = CsrTimeGet(NULL);
        CsrTime elapsedMs = (100 * (now - instanceData->srvStartTime)) / 100000;

        if (elapsedMs > 0)
        {
            throughput = (8 * instanceData->srvFrameSize) / elapsedMs;
        }
        CsrSnprintf(infoString, sizeof(infoString), "%ukbit/s", throughput);
        CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_SRV_KEY, MENU_ITEM_SRV_ENABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_TEXT), CsrUtf82Utf16String((CsrUtf8String *) infoString), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);

        instanceData->srvFrameSize = 0;
        instanceData->srvStartTime = now;

        instanceData->srvTimerId = CsrSchedTimerSet(MEASURE_TIME_MICRO, srvMeasureUpdate, 0, mv);
    }
    else
    {
        instanceData->srvTimerId = CSR_SCHED_TID_INVALID;
    }
}

void CsrTcpTransceiverIpSocketTcpNewCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpNewCfm *confirm = instanceData->message;

    if (instanceData->cliState == STATE_ENABLING)
    {
        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Created client socket handle (%u)", confirm->socketHandle));
            instanceData->cliSocketHandle = confirm->socketHandle;
            CsrIpSocketOptionsReqSend2(confirm->socketHandle, 0, 0, nagle, FALSE, FALSE,
                CSR_IP_SOCKET_PRIORITY_DEFAULT, CSR_IP_SOCKET_OPTION_NAGLE)
        }
        else
        {
            CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
                (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME), TRUE, TRUE,
                "Failed to create TCP socket");
            instanceData->cliState = STATE_OFF;
            return;
        }
    }
    else if (instanceData->srvState == STATE_ENABLING)
    {
        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Created listen socket handle (%u)", confirm->socketHandle));
            instanceData->listenSocketHandle = confirm->socketHandle;
            CsrIpSocketTcpBindReqSend2(confirm->socketHandle, NULL, SRV_LISTEN_PORT);
        }
        else
        {
            CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
                (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME), TRUE, TRUE,
                "Failed to create TCP socket");
            instanceData->srvState = STATE_OFF;
            return;
        }
    }
}

void CsrTcpTransceiverIpSocketOptionsCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketOptionsCfm *confirm = instanceData->message;

    if (instanceData->cliState == STATE_ENABLING)
    {
        if ((confirm->validOptions & CSR_IP_SOCKET_OPTION_NAGLE) != 0)
        {
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Client socket options set (%u)", confirm->socketHandle));
        }
        else
        {
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Client socket options not set (%u)", confirm->socketHandle));
        }
        instanceData->cliState = STATE_ON;
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_ENABLED_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_CONNECTING_SUBTEXT),
            instanceData->hSelectEvent, instanceData->hOptionsMenu,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        CsrIpSocketTcpConnectReqSend2(confirm->socketHandle, remoteIpAddress, SRV_LISTEN_PORT);
    }
}

void CsrTcpTransceiverIpSocketTcpBindCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpBindCfm *confirm = instanceData->message;

    if ((instanceData->srvState == STATE_ENABLING) &&
        (instanceData->listenSocketHandle == confirm->socketHandle))
    {
        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            CsrIpSocketTcpListenReqSend2(confirm->socketHandle);
        }
        else
        {
            CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
                (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME), TRUE, TRUE,
                "Failed to bind TCP socket");
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing listen socket handle (%u)", instanceData->listenSocketHandle));
            CsrIpSocketTcpCloseReqSend2(instanceData->listenSocketHandle);
            instanceData->srvState = STATE_OFF;
        }
    }
}

void CsrTcpTransceiverIpSocketTcpListenCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpListenCfm *confirm = instanceData->message;

    if ((instanceData->srvState == STATE_ENABLING) &&
        (instanceData->listenSocketHandle == confirm->socketHandle))
    {
        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            instanceData->srvState = STATE_ON;
            CsrUiMenuSetitemReqSend(instanceData->hMenu,
                MENU_ITEM_SRV_KEY, MENU_ITEM_SRV_ENABLED_ICON,
                CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_TEXT),
                CsrUtf82Utf16String((CsrUtf8String *) "Listening"),
                instanceData->hSelectEvent, instanceData->hOptionsMenu,
                instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        }
        else
        {
            CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
                (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME), TRUE, TRUE,
                "Failed to listen on TCP socket");
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing listen socket handle (%u)", instanceData->listenSocketHandle));
            CsrIpSocketTcpCloseReqSend2(instanceData->listenSocketHandle);
            instanceData->srvState = STATE_OFF;
        }
    }
}

void CsrTcpTransceiverIpSocketTcpConnectCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpConnectCfm *confirm = instanceData->message;

    if ((instanceData->cliState == STATE_ON) &&
        (instanceData->cliSocketHandle == confirm->socketHandle))
    {
        if (confirm->result == CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Connected on client socket handle (%u)", instanceData->cliSocketHandle));
            instanceData->cliFrameSize = 0;
            instanceData->cliStartTime = CsrTimeGet(NULL);
            cliMeasureUpdate(0, instanceData);
            transmitFrame(instanceData);
        }
        else
        {
            CsrAppBacklogReqSend2((CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
                (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME), TRUE, TRUE,
                "Failed to connect to server");
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing client socket handle (%u)", instanceData->cliSocketHandle));
            CsrIpSocketTcpCloseReqSend2(instanceData->cliSocketHandle);
            CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            instanceData->cliState = STATE_OFF;
        }
    }
}

void CsrTcpTransceiverIpSocketTcpAcceptIndHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpAcceptInd *indication = instanceData->message;

    if ((instanceData->srvState == STATE_ON) &&
        (instanceData->listenSocketHandle == indication->listenSocketHandle) &&
        (instanceData->srvTimerId == CSR_SCHED_TID_INVALID))
    {
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Accepted server socket handle (%u)", indication->socketHandle));
        instanceData->srvSocketHandle = indication->socketHandle;
        instanceData->srvFrameSize = 0;
        instanceData->srvStartTime = CsrTimeGet(NULL);
        srvMeasureUpdate(0, instanceData);
    }
    else
    {
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Rejecting socket handle (%u)", indication->socketHandle));
        CsrIpSocketTcpCloseReqSend2(indication->socketHandle);
    }
}

void CsrTcpTransceiverIpSocketTcpDataCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpDataCfm *confirm = instanceData->message;

    if ((instanceData->cliState == STATE_ON) &&
        (instanceData->cliSocketHandle == confirm->socketHandle))
    {
        transmitFrame(instanceData);
    }
}

void CsrTcpTransceiverIpSocketTcpDataIndHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpDataInd *indication = instanceData->message;

    if ((instanceData->srvState == STATE_ON) &&
        (instanceData->srvTimerId != CSR_SCHED_TID_INVALID) &&
        (instanceData->srvSocketHandle == indication->socketHandle))
    {
        instanceData->srvFrameSize += indication->dataLength;
        CsrIpSocketTcpDataResSend2(instanceData->srvSocketHandle);
    }
    CsrPmemFree(indication->data);
    indication->data = NULL;
}

void CsrTcpTransceiverIpSocketTcpCloseIndHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrIpSocketTcpCloseInd *indication = instanceData->message;

    if ((instanceData->srvState == STATE_ON) &&
        (instanceData->srvTimerId != CSR_SCHED_TID_INVALID) &&
        (instanceData->srvSocketHandle == indication->socketHandle))
    {
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closed server socket handle (%u)", indication->socketHandle));
        CsrSchedTimerCancel(instanceData->srvTimerId, NULL, NULL);
        instanceData->srvTimerId = CSR_SCHED_TID_INVALID;
        CsrUiMenuSetitemReqSend(instanceData->hMenu,
            MENU_ITEM_SRV_KEY, MENU_ITEM_SRV_ENABLED_ICON,
            CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_TEXT),
            CsrUtf82Utf16String((CsrUtf8String *) "Listening"),
            instanceData->hSelectEvent, instanceData->hOptionsMenu,
            instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
    }
    else if ((instanceData->cliState == STATE_ON) &&
             (instanceData->cliSocketHandle == indication->socketHandle))
    {
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closed client socket handle (%u)", indication->socketHandle));
        CsrSchedTimerCancel(instanceData->cliTimerId, NULL, NULL);
        instanceData->cliTimerId = CSR_SCHED_TID_INVALID;
        CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        instanceData->cliState = STATE_OFF;
    }
}

void CsrTcpTransceiverAppRegisterCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrAppRegisterCfm *confirm = instanceData->message;
    instanceData->controlId = confirm->controlId;
}

void CsrTcpTransceiverAppTakeControlIndHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrAppTakeControlInd *indication = (CsrAppTakeControlInd *) instanceData->message;

    if (indication->controlId == instanceData->controlId)
    {
        CsrUiUieShowReqSend(instanceData->hMenu,
            CSR_TCP_TRANSCEIVER_IFACEQUEUE,
            CSR_UI_INPUTMODE_AUTO,
            CSR_UI_LOWESTPRIORITY);
    }
}

void CsrTcpTransceiverUiUieCreateCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrUiUieCreateCfm *confirm = instanceData->message;
    CsrBool uiCreationCycleDone = FALSE;

    switch (confirm->elementType)
    {
        case CSR_UI_UIETYPE_EVENT:
        {
            CsrBool lastEventCreated = FALSE;
            if (!instanceData->hSelectEvent)
            {
                instanceData->hSelectEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_AUTO, CSR_TCP_TRANSCEIVER_IFACEQUEUE);
            }
            else if (!instanceData->hBackEvent)
            {
                instanceData->hBackEvent = confirm->handle;
                CsrUiEventSetReqSend(confirm->handle, CSR_UI_INPUTMODE_BLOCK, CSR_TCP_TRANSCEIVER_IFACEQUEUE);
                lastEventCreated = TRUE;
            }

            if (!lastEventCreated)
            {
                CsrUiUieCreateReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, CSR_UI_UIETYPE_EVENT);
            }
            else
            {
                CsrUiUieCreateReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, CSR_UI_UIETYPE_INPUTDIALOG);
            }
            break;
        }
        case CSR_UI_UIETYPE_INPUTDIALOG:
        {
            instanceData->hInputDialog = confirm->handle;
            CsrUiUieCreateReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            break;
        }
        case CSR_UI_UIETYPE_MENU:
        {
            CsrBool lastMenuCreated = FALSE;
            if (!instanceData->hOptionsMenu)
            {
                instanceData->hOptionsMenu = confirm->handle;

                CsrUiMenuSetReqSend(confirm->handle,
                    CsrUtf82Utf16String((const CsrUtf8String *) (MENU_ITEM_NAME)),
                    CsrUtf82Utf16String((const CsrUtf8String *) ("Modify")), NULL);
                CsrUiMenuAdditemReqSend(confirm->handle,
                    CSR_UI_LAST, OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY,
                    OPT_MENU_ITEM_REMOTE_IP_ADDRESS_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_REMOTE_IP_ADDRESS),
                    ipAddressToString(remoteIpAddress),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle,
                    CSR_UI_LAST, OPT_MENU_ITEM_TX_SIZE_KEY,
                    OPT_MENU_ITEM_TX_SIZE_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_TX_SIZE),
                    CsrUint32ToUtf16String(txSize),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle,
                    CSR_UI_LAST, OPT_MENU_ITEM_NAGLE_KEY,
                    OPT_MENU_ITEM_NAGLE_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_NAGLE),
                    nagleToString(),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            }
            else if (!instanceData->hMenu)
            {
                instanceData->hMenu = confirm->handle;

                CsrUiMenuSetReqSend(confirm->handle, CsrUtf82Utf16String((const CsrUtf8String *) (MENU_ITEM_NAME)), CsrUtf82Utf16String((const CsrUtf8String *) ("Select")), CsrUtf82Utf16String((const CsrUtf8String *) ("Options")));
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_FIRST, MENU_ITEM_SRV_KEY, MENU_ITEM_SRV_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(confirm->handle, CSR_UI_LAST, MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);

                lastMenuCreated = TRUE;
            }

            if (!lastMenuCreated)
            {
                CsrUiUieCreateReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, CSR_UI_UIETYPE_MENU);
            }
            else
            {
                uiCreationCycleDone = TRUE;
            }
            break;
        }
        default:
            break;
    }

    if (uiCreationCycleDone)
    {
        /* Add menu entry to main menu */
        CsrAppRegisterReqSend2(CSR_TCP_TRANSCEIVER_IFACEQUEUE,
            (CsrUtf8String *) CsrStrDup(MENU_CATEGORY),
            (CsrUtf8String *) CsrStrDup(MENU_ITEM_NAME),
            CSR_UI_ICON_CONNECT,
            CSR_UI_ICON_NONE,
            FALSE);
    }
}

void CsrTcpTransceiverUiInputdialogGetCfmHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrUiInputdialogGetCfm *confirm = instanceData->message;
    switch (instanceData->parameterKey)
    {
        case OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY:
        {
            CsrCharString *newIpAddressString = (CsrCharString *) CsrUtf16String2Utf8(confirm->text);
            stringToIpAddress(newIpAddressString, remoteIpAddress);
            CsrPmemFree(newIpAddressString);
            CsrUiMenuSetitemReqSend(instanceData->hOptionsMenu, OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY, OPT_MENU_ITEM_REMOTE_IP_ADDRESS_ICON, CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_REMOTE_IP_ADDRESS), ipAddressToString(remoteIpAddress), instanceData->hSelectEvent, CSR_UI_DEFAULTACTION, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            break;
        }
        case OPT_MENU_ITEM_TX_SIZE_KEY:
        {
            txSize = (CsrUint16) CsrUtf16StringToUint32(confirm->text);
            CsrUiMenuSetitemReqSend(instanceData->hOptionsMenu, OPT_MENU_ITEM_TX_SIZE_KEY, OPT_MENU_ITEM_TX_SIZE_ICON, CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_TX_SIZE), CsrUint32ToUtf16String(txSize), instanceData->hSelectEvent, CSR_UI_DEFAULTACTION, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
            break;
        }
        default:
            break;
    }
    CsrPmemFree(confirm->text);
    confirm->text = NULL;
    CsrUiUieHideReqSend(instanceData->hInputDialog);
}

static void startStopSrv(CsrTcpTransceiverInstanceData *instanceData)
{
    if (instanceData->srvState == STATE_OFF)
    {
        instanceData->srvState = STATE_ENABLING;
        CsrIpSocketTcpNewReqSend2(CSR_TCP_TRANSCEIVER_IFACEQUEUE,
            CSR_IP_SOCKET_FAMILY_IP4);
    }
    else
    {
        if (instanceData->srvTimerId != CSR_SCHED_TID_INVALID)
        {
            CsrSchedTimerCancel(instanceData->srvTimerId, NULL, NULL);
            instanceData->srvTimerId = CSR_SCHED_TID_INVALID;
            CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing server socket handle (%u)", instanceData->srvSocketHandle));
            CsrIpSocketTcpCloseReqSend2(instanceData->srvSocketHandle);
        }
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing listen socket handle (%u)", instanceData->listenSocketHandle));
        CsrIpSocketTcpCloseReqSend2(instanceData->listenSocketHandle);
        CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_SRV_KEY, MENU_ITEM_SRV_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_SRV_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        instanceData->srvState = STATE_OFF;
    }
}

static void startStopCli(CsrTcpTransceiverInstanceData *instanceData)
{
    if (instanceData->cliState == STATE_OFF)
    {
        instanceData->cliState = STATE_ENABLING;
        CsrIpSocketTcpNewReqSend2(CSR_TCP_TRANSCEIVER_IFACEQUEUE,
            CSR_IP_SOCKET_FAMILY_IP4);
    }
    else
    {
        CsrSchedTimerCancel(instanceData->cliTimerId, NULL, NULL);
        instanceData->cliTimerId = CSR_SCHED_TID_INVALID;
        CSR_LOG_TEXT_INFO((CsrTcpTransceiverLto, 0, "Closing client socket handle (%u)", instanceData->cliSocketHandle));
        CsrIpSocketTcpCloseReqSend2(instanceData->cliSocketHandle);
        CsrUiMenuSetitemReqSend(instanceData->hMenu, MENU_ITEM_CLI_KEY, MENU_ITEM_CLI_DISABLED_ICON, CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_TEXT), CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_CLI_DISABLED_SUBTEXT), instanceData->hSelectEvent, instanceData->hOptionsMenu, instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
        instanceData->cliState = STATE_OFF;
    }
}

void CsrTcpTransceiverUiEventIndHandler(CsrTcpTransceiverInstanceData *instanceData)
{
    CsrUiEventInd *indication = instanceData->message;

    if ((indication->displayElement == instanceData->hMenu) &&
        (indication->event == instanceData->hSelectEvent))
    {
        if ((instanceData->srvState != STATE_ENABLING) &&
            (instanceData->srvState != STATE_DISABLING) &&
            (instanceData->cliState != STATE_ENABLING) &&
            (instanceData->cliState != STATE_DISABLING))
        {
            switch (indication->key)
            {
                case MENU_ITEM_CLI_KEY:
                    startStopCli(instanceData);
                    break;
                case MENU_ITEM_SRV_KEY:
                    startStopSrv(instanceData);
                    break;
                default:
                    break;
            }
        }
        else
        {
            /* Ignore when enabling or disabling */
        }
    }
    else if ((indication->displayElement == instanceData->hOptionsMenu) &&
             (indication->event == instanceData->hSelectEvent))
    {
        switch (indication->key)
        {
            case OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY:
            {
                instanceData->parameterKey = indication->key;
                CsrUiInputdialogSetReqSend(instanceData->hInputDialog,
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_NAME),
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_REMOTE_IP_ADDRESS),
                    OPT_MENU_ITEM_REMOTE_IP_ADDRESS_ICON,
                    ipAddressToString(remoteIpAddress), REMOTE_IP_ADDRESS_STRING_LENGTH, CSR_UI_KEYMAP_CONTROLNUMERIC,
                    CsrUtf82Utf16String((CsrUtf8String *) "OK"),
                    CsrUtf82Utf16String((CsrUtf8String *) "Cancel"),
                    instanceData->hSelectEvent, instanceData->hBackEvent,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(instanceData->hInputDialog,
                    CSR_TCP_TRANSCEIVER_IFACEQUEUE,
                    CSR_UI_INPUTMODE_AUTO,
                    CSR_UI_LOWESTPRIORITY);
                break;
            }
            case OPT_MENU_ITEM_TX_SIZE_KEY:
            {
                instanceData->parameterKey = indication->key;
                CsrUiInputdialogSetReqSend(instanceData->hInputDialog,
                    CsrUtf82Utf16String((CsrUtf8String *) MENU_ITEM_NAME),
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_TX_SIZE),
                    OPT_MENU_ITEM_TX_SIZE_ICON, CsrUint32ToUtf16String(txSize),
                    TX_SIZE_STRING_LENGTH, CSR_UI_KEYMAP_NUMERIC,
                    CsrUtf82Utf16String((CsrUtf8String *) "OK"),
                    CsrUtf82Utf16String((CsrUtf8String *) "Cancel"),
                    instanceData->hSelectEvent, instanceData->hBackEvent,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(instanceData->hInputDialog,
                    CSR_TCP_TRANSCEIVER_IFACEQUEUE,
                    CSR_UI_INPUTMODE_AUTO,
                    CSR_UI_LOWESTPRIORITY);
                break;
            }
            case OPT_MENU_ITEM_NAGLE_KEY:
            {
                nagle = !nagle;
                CsrUiMenuSetitemReqSend(instanceData->hOptionsMenu,
                    OPT_MENU_ITEM_NAGLE_KEY, OPT_MENU_ITEM_NAGLE_ICON,
                    CsrUtf82Utf16String((CsrUtf8String *) OPT_MENU_ITEM_NAGLE),
                    nagleToString(),
                    instanceData->hSelectEvent, CSR_UI_DEFAULTACTION,
                    instanceData->hBackEvent, CSR_UI_DEFAULTACTION);
                break;
            }
            default:
                break;
        }
    }
    else if ((indication->displayElement == instanceData->hInputDialog) &&
             (indication->event == instanceData->hSelectEvent))
    {
        switch (instanceData->parameterKey)
        {
            case OPT_MENU_ITEM_REMOTE_IP_ADDRESS_KEY:
            case OPT_MENU_ITEM_TX_SIZE_KEY:
                CsrUiInputdialogGetReqSend(CSR_TCP_TRANSCEIVER_IFACEQUEUE, instanceData->hInputDialog);
                break;
            default:
                break;
        }
    }
    else if (indication->event == instanceData->hBackEvent)
    {
        CsrUiUieHideReqSend(indication->displayElement);
    }
}
