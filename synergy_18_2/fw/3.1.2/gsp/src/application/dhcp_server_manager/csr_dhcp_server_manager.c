/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_unicode.h"
#include "csr_util.h"
#include "csr_formatted_io.h"

#include "csr_log_text_2.h"

#include "csr_app_prim.h"
#include "csr_app_lib.h"

#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

#include "csr_dhcp_server_manager_task.h"
#include "csr_dhcp_server_lib.h"

#define CSR_DHCP_SERVER_MANAGER_IP_ADDRESS                {192, 168, 1, 129}
#define CSR_DHCP_SERVER_MANAGER_NETWORK                   {192, 168, 1, 156}
#define CSR_DHCP_SERVER_MANAGER_NETWORK_MASK              {255, 255, 255, 224}
#define CSR_DHCP_SERVER_MANAGER_GATEWAY                   {192, 168, 1, 129}
#define CSR_DHCP_SERVER_MANAGER_DNS1                      {10, 11, 12, 1}
#define CSR_DHCP_SERVER_MANAGER_DNS2                      {10, 11, 12, 2}
#define CSR_DHCP_SERVER_MANAGER_LEASE_TIME                (2)

#define CSR_DHCP_SERVER_MANAGER_UI_KEY_CREATE             (0)
#define CSR_DHCP_SERVER_MANAGER_UI_KEY_DESTROY            (1)
#define CSR_DHCP_SERVER_MANAGER_UI_KEY_START              (2)
#define CSR_DHCP_SERVER_MANAGER_UI_KEY_STOP               (3)
#define CSR_DHCP_SERVER_MANAGER_UI_KEY_CURRENT_LEASES     (4)

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrDhcpServerManagerLto);

typedef struct
{
    CsrUieHandle hMainMenu;
    CsrUieHandle hCurrentLeasesMenu;
    CsrUieHandle hMainDialog;
    CsrUieHandle HndlbackEvent;
    CsrUieHandle HndlCreateDestroyEvent;
    CsrUieHandle HndlStartStopEvent;
    CsrUieHandle HndlCurrentLeasesEvent;

    CsrBool     uiCreateVisible;
    CsrBool     uiDestroyVisible;
    CsrBool     uiStartVisible;
    CsrBool     uiStopVisible;
    CsrBool     uiDialogVisible;
    CsrBool     uiCurrentLeasesVisible;
    CsrSchedTid uiDialogTimer;

    CsrBool             busy;
    CsrBool             serverCreated;
    CsrBool             serverStarted;
    CsrDhcpServerHandle serverHandle;
    CsrUint16           leasesCount;
    CsrDhcpServerLease *leases;
    CsrBool             leasesSet;
    CsrBool             gettingLeases;
} CsrDhcpServerManagerInstanceData;

void CsrDhcpServerManagerInit(void **gash)
{
    CsrDhcpServerManagerInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    inst->hMainMenu = 0;
    inst->hCurrentLeasesMenu = 0;
    inst->hMainDialog = 0;
    inst->HndlbackEvent = 0;
    inst->HndlCreateDestroyEvent = 0;
    inst->HndlStartStopEvent = 0;
    inst->HndlCurrentLeasesEvent = 0;

    inst->uiCreateVisible = FALSE;
    inst->uiDestroyVisible = FALSE;
    inst->uiStopVisible = FALSE;
    inst->uiDialogVisible = FALSE;
    inst->uiCurrentLeasesVisible = FALSE;
    inst->uiDialogTimer = 0;

    inst->busy = FALSE;
    inst->serverStarted = FALSE;
    inst->serverCreated = FALSE;
    inst->serverHandle = 0;
    inst->leasesCount = 0;
    inst->leases = NULL;
    inst->leasesSet = TRUE;
    inst->gettingLeases = FALSE;

    CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
        CSR_UI_UIETYPE_EVENT);

    CSR_LOG_TEXT_REGISTER(&CsrDhcpServerManagerLto, "DHCP_SERVER_MANAGER", 0, NULL);
}

void CsrDhcpServerManagerDeinit(void **gash)
{
    CsrDhcpServerManagerInstanceData *inst;

    inst = (CsrDhcpServerManagerInstanceData *) *gash;
    CsrPmemFree(inst->leases);
    CsrPmemFree(inst);
}

static void csrDhcpServerManagerHideDialog(CsrUint16 mi, void *mv)
{
    CsrDhcpServerManagerInstanceData *inst = (CsrDhcpServerManagerInstanceData *) mv;

    if (inst->uiDialogVisible)
    {
        CsrUiUieHideReqSend(inst->hMainDialog);
        inst->uiDialogVisible = FALSE;
        inst->uiDialogTimer = 0;
    }
}

static void csrDhcpServerManagerUpdateMainMenu(CsrDhcpServerManagerInstanceData *inst)
{
    if (inst->uiCreateVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hMainMenu, CSR_DHCP_SERVER_MANAGER_UI_KEY_CREATE);
        inst->uiCreateVisible = FALSE;
    }

    if (inst->uiDestroyVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hMainMenu, CSR_DHCP_SERVER_MANAGER_UI_KEY_DESTROY);
        inst->uiDestroyVisible = FALSE;
    }

    if (inst->uiStopVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hMainMenu, CSR_DHCP_SERVER_MANAGER_UI_KEY_STOP);
        inst->uiStopVisible = FALSE;
    }

    if (inst->uiStartVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hMainMenu, CSR_DHCP_SERVER_MANAGER_UI_KEY_START);
        inst->uiStartVisible = FALSE;
    }

    if (inst->uiCurrentLeasesVisible)
    {
        CsrUiMenuRemoveitemReqSend(inst->hMainMenu, CSR_DHCP_SERVER_MANAGER_UI_KEY_CURRENT_LEASES);
        inst->uiCurrentLeasesVisible = FALSE;
    }

    if (inst->serverCreated)
    {
        if (inst->serverStarted)
        {
            CsrUiMenuAdditemReqSend(inst->hMainMenu,
                CSR_UI_LAST,
                CSR_DHCP_SERVER_MANAGER_UI_KEY_STOP,
                CSR_UI_ICON_NONE,
                CsrUtf82Utf16String((CsrUtf8String *) "Stop server"),
                CsrUtf82Utf16String((CsrUtf8String *) "Save lease data"),
                inst->HndlStartStopEvent,
                CSR_UI_DEFAULTACTION,
                inst->HndlbackEvent,
                CSR_UI_DEFAULTACTION);

            inst->uiStopVisible = TRUE;
        }
        else
        {
            CsrUiMenuAdditemReqSend(inst->hMainMenu,
                CSR_UI_LAST,
                CSR_DHCP_SERVER_MANAGER_UI_KEY_START,
                CSR_UI_ICON_NONE,
                CsrUtf82Utf16String((CsrUtf8String *) "Start server"),
                CsrUtf82Utf16String((CsrUtf8String *) "Restore lease data"),
                inst->HndlStartStopEvent,
                CSR_UI_DEFAULTACTION,
                inst->HndlbackEvent,
                CSR_UI_DEFAULTACTION);

            inst->uiStartVisible = TRUE;
        }

        CsrUiMenuAdditemReqSend(inst->hMainMenu,
            CSR_UI_LAST,
            CSR_DHCP_SERVER_MANAGER_UI_KEY_CURRENT_LEASES,
            CSR_UI_ICON_NONE,
            CsrUtf82Utf16String((CsrUtf8String *) "Current leases"),
            NULL,
            inst->HndlCurrentLeasesEvent,
            CSR_UI_DEFAULTACTION,
            inst->HndlbackEvent,
            CSR_UI_DEFAULTACTION);

        inst->uiCurrentLeasesVisible = TRUE;

        CsrUiMenuAdditemReqSend(inst->hMainMenu,
            CSR_UI_LAST,
            CSR_DHCP_SERVER_MANAGER_UI_KEY_DESTROY,
            CSR_UI_ICON_NONE,
            CsrUtf82Utf16String((CsrUtf8String *) "Destroy server"),
            NULL,
            inst->HndlCreateDestroyEvent,
            CSR_UI_DEFAULTACTION,
            inst->HndlbackEvent,
            CSR_UI_DEFAULTACTION);

        inst->uiDestroyVisible = TRUE;
    }
    else
    {
        CsrUiMenuAdditemReqSend(inst->hMainMenu,
            CSR_UI_LAST,
            CSR_DHCP_SERVER_MANAGER_UI_KEY_CREATE,
            CSR_UI_ICON_NONE,
            CsrUtf82Utf16String((CsrUtf8String *) "Create server"),
            NULL,
            inst->HndlCreateDestroyEvent,
            CSR_UI_DEFAULTACTION,
            inst->HndlbackEvent,
            CSR_UI_DEFAULTACTION);

        inst->uiCreateVisible = TRUE;
    }

    CsrUiUieShowReqSend(inst->hMainMenu,
        CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
        CSR_UI_INPUTMODE_AUTO,
        CSR_UI_LOWESTPRIORITY);
}

static void csrDhcpServerManagerShowDialog(CsrDhcpServerManagerInstanceData *inst, CsrUtf16String *message)
{
    CsrUiDialogSetReqSend(inst->hMainDialog,
        CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server"),
        message,
        CsrUtf82Utf16String((CsrUtf8String *) "OK"),
        NULL,
        inst->HndlbackEvent,
        CSR_UI_DEFAULTACTION,
        CSR_UI_DEFAULTACTION,
        CSR_UI_DEFAULTACTION);

    inst->uiDialogVisible = TRUE;
    inst->uiDialogTimer = CsrSchedTimerSet(CSR_SCHED_SECOND * 3, csrDhcpServerManagerHideDialog, 0, inst);

    /* Show the Dialog */
    CsrUiUieShowReqSend(inst->hMainDialog,
        CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
        CSR_UI_INPUTMODE_AUTO,
        1);

    /* Update the Main menu */
    csrDhcpServerManagerUpdateMainMenu(inst);
}

void CsrDhcpServerManagerHandler(void **gash)
{
    CsrDhcpServerManagerInstanceData *inst;
    void *message;
    CsrUint16 event;

    inst = (CsrDhcpServerManagerInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_DHCP_SERVER_PRIM:
            {
                CsrDhcpServerPrim *type;

                type = (CsrDhcpServerPrim *) message;

                switch (*type)
                {
                    case CSR_DHCP_SERVER_CREATE_CFM:
                    {
                        CsrDhcpServerCreateCfm *prim = (CsrDhcpServerCreateCfm *) message;

                        if (prim->result == CSR_RESULT_SUCCESS)
                        {
                            inst->serverHandle = prim->serverHandle;
                            inst->serverCreated = TRUE;

                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server has been created"));
                        }
                        else
                        {
                            inst->serverCreated = FALSE;

                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server could NOT be created!"));
                        }

                        inst->busy = FALSE;

                        break;
                    }
                    case CSR_DHCP_SERVER_DESTROY_CFM:
                    {
                        CsrDhcpServerDestroyCfm *prim = (CsrDhcpServerDestroyCfm *) message;

                        if (prim->result == CSR_RESULT_SUCCESS)
                        {
                            inst->serverCreated = FALSE;
                            inst->serverStarted = FALSE;

                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server has been destroyed"));
                        }
                        else
                        {
                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server could NOT be destroyed!"));
                        }

                        inst->busy = FALSE;

                        break;
                    }
                    case CSR_DHCP_SERVER_START_CFM:
                    {
                        CsrDhcpServerStartCfm *prim = (CsrDhcpServerStartCfm *) message;

                        if (prim->result == CSR_RESULT_SUCCESS)
                        {
                            inst->serverStarted = TRUE;

                            if (inst->leasesSet)
                            {
                                csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server has been started"));
                            }
                            else
                            {
                                csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server has been started, but could NOT set the lease data!"));
                            }
                        }
                        else
                        {
                            inst->serverStarted = FALSE;

                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server could NOT start"));
                        }

                        inst->busy = FALSE;
                        inst->leasesSet = TRUE;

                        break;
                    }
                    case CSR_DHCP_SERVER_STOP_CFM:
                    {
                        CsrDhcpServerStopCfm *prim = (CsrDhcpServerStopCfm *) message;

                        if (prim->result == CSR_RESULT_SUCCESS)
                        {
                            CsrDhcpServerLeasesGetReqSend(prim->serverHandle);
                        }
                        else
                        {
                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server could NOT be stopped!"));
                            inst->busy = FALSE;
                        }

                        break;
                    }
                    case CSR_DHCP_SERVER_LEASES_GET_CFM:
                    {
                        CsrDhcpServerLeasesGetCfm *prim = (CsrDhcpServerLeasesGetCfm *) message;

                        if (inst->gettingLeases)
                        {
                            CsrUiMenuRemoveallitemsReqSend(inst->hCurrentLeasesMenu);

                            if (prim->leasesCount > 0)
                            {
                                CsrUint16 count;

                                for (count = 0; count < prim->leasesCount; count++)
                                {
                                    CsrUint32 hours;
                                    CsrUint32 minutes;
                                    CsrUint32 seconds;
                                    CsrUint32 sec;
                                    CsrCharString label[50];
                                    CsrCharString sublabel[20];
                                    CsrUint8 *ip = prim->leases[count].ip;
                                    CsrUint8 *mac = prim->leases[count].mac;
                                    CsrTimeUtc expiryTime = prim->leases[count].expiryTime;
                                    CsrTimeUtc currentTime;

                                    CsrTimeUtcGet(&currentTime, NULL, NULL);

                                    if (expiryTime.sec > currentTime.sec)
                                    {
                                        sec = expiryTime.sec - currentTime.sec;

                                        hours = sec / 3600;
                                        minutes = (sec % 3600) / 60;
                                        seconds = (sec % 3600) % 60;
                                    }
                                    else
                                    {
                                        hours = 0;
                                        minutes = 0;
                                        seconds = 0;
                                    }

                                    CsrSnprintf(label, 50, "%d.%d.%d.%d (%dh %dm %ds)", ip[0], ip[1], ip[2], ip[3], hours, minutes, seconds);
                                    CsrSnprintf(sublabel, 20, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

                                    CsrUiMenuAdditemReqSend(inst->hCurrentLeasesMenu,
                                        CSR_UI_LAST,
                                        CSR_DHCP_SERVER_MANAGER_UI_KEY_CURRENT_LEASES,
                                        CSR_UI_ICON_NONE,
                                        CsrUtf82Utf16String((CsrUtf8String *) label),
                                        CsrUtf82Utf16String((CsrUtf8String *) sublabel),
                                        inst->HndlbackEvent,
                                        CSR_UI_DEFAULTACTION,
                                        inst->HndlbackEvent,
                                        CSR_UI_DEFAULTACTION);
                                }
                            }

                            CsrUiUieShowReqSend(inst->hCurrentLeasesMenu,
                                CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
                        }
                        else
                        {
                            inst->serverStarted = FALSE;
                            csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) "The server has been stopped"));

                            if (prim->leasesCount > 0)
                            {
                                inst->leases = prim->leases;
                                inst->leasesCount = prim->leasesCount;
                                prim->leases = NULL;
                            }
                        }

                        CsrPmemFree(prim->leases);

                        inst->busy = FALSE;
                        inst->gettingLeases = FALSE;

                        break;
                    }
                    case CSR_DHCP_SERVER_LEASES_SET_CFM:
                    {
                        CsrDhcpServerLeasesSetCfm *prim = (CsrDhcpServerLeasesSetCfm *) message;

                        if (prim->result != CSR_RESULT_SUCCESS)
                        {
                            inst->leasesSet = FALSE;
                        }

                        CsrDhcpServerStartReqSend(inst->serverHandle);

                        break;
                    }
                    case CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND:
                    {
                        CsrDhcpServerRogueIpAddressInd *prim = (CsrDhcpServerRogueIpAddressInd *) message;
                        CsrCharString label[50];

                        CsrSnprintf(label, 50, "Rogue IP address: %d.%d.%d.%d", prim->ipAddress[0], prim->ipAddress[1], prim->ipAddress[2], prim->ipAddress[3]);

                        csrDhcpServerManagerShowDialog(inst, CsrUtf82Utf16String((CsrUtf8String *) label));
                        break;
                    }
                    default:
                    {
                        CSR_LOG_TEXT_ERROR((CsrDhcpServerManagerLto, 0, "[%s:%d] Unknown CSR_DHCP_SERVER_PRIM (0x%X)", __FILE__, __LINE__, *type));
                        inst->busy = FALSE;
                        break;
                    }
                }

                break;
            }
            case CSR_APP_PRIM:
            {
                CsrAppPrim *type;

                type = (CsrAppPrim *) message;

                if (*type == CSR_APP_REGISTER_CFM)
                {
                    /* Ignore */
                }
                else if (*type == CSR_APP_TAKE_CONTROL_IND)
                {
                    csrDhcpServerManagerUpdateMainMenu(inst);
                }
                else
                {
                    CSR_LOG_TEXT_ERROR((CsrDhcpServerManagerLto, 0, "[%s:%d] Unknown CSR_APP_PRIM (0x%X)", __FILE__, __LINE__, *type));
                }

                break;
            }
            case CSR_UI_PRIM:
            {
                CsrUint16 *type;

                type = (CsrUint16 *) message;

                if (*type == CSR_UI_EVENT_IND)
                {
                    CsrUiEventInd *indication;

                    indication = (CsrUiEventInd *) message;

                    if (indication->event == inst->HndlbackEvent)
                    {
                        if (inst->uiDialogVisible)
                        {
                            CsrSchedTimerCancel(inst->uiDialogTimer, NULL, NULL);

                            inst->uiDialogVisible = FALSE;
                            inst->uiDialogTimer = 0;
                        }

                        CsrUiUieHideReqSend(indication->displayElement);
                    }
                    else if (indication->event == inst->HndlCreateDestroyEvent)
                    {
                        if (inst->serverCreated)
                        {
                            CsrDhcpServerDestroyReqSend(inst->serverHandle);
                            inst->busy = TRUE;
                        }
                        else
                        {
                            CsrUint8 ipAddress[4] = CSR_DHCP_SERVER_MANAGER_IP_ADDRESS;
                            CsrUint8 network[4] = CSR_DHCP_SERVER_MANAGER_NETWORK;
                            CsrUint8 networkMask[4] = CSR_DHCP_SERVER_MANAGER_NETWORK_MASK;
                            CsrUint8 gateway[4] = CSR_DHCP_SERVER_MANAGER_GATEWAY;
                            CsrUint8 dns1[4] = CSR_DHCP_SERVER_MANAGER_DNS1;
                            CsrUint8 dns2[4] = CSR_DHCP_SERVER_MANAGER_DNS2;
                            CsrUint8 leaseTime = CSR_DHCP_SERVER_MANAGER_LEASE_TIME;

                            CsrDhcpServerCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                ipAddress,
                                network,
                                networkMask,
                                gateway,
                                dns1,
                                dns2,
                                leaseTime);
                            inst->busy = TRUE;
                        }
                    }
                    else if (indication->event == inst->HndlStartStopEvent)
                    {
                        if (inst->serverStarted)
                        {
                            CsrDhcpServerStopReqSend(inst->serverHandle);
                            inst->busy = TRUE;
                        }
                        else
                        {
                            if (inst->leases)
                            {
                                CsrDhcpServerLeasesSetReqSend(inst->serverHandle, inst->leasesCount, inst->leases);
                                inst->leases = NULL;
                                inst->leasesCount = 0;
                            }
                            else
                            {
                                CsrDhcpServerStartReqSend(inst->serverHandle);
                            }

                            inst->busy = TRUE;
                        }
                    }
                    else if (indication->event == inst->HndlCurrentLeasesEvent)
                    {
                        inst->gettingLeases = TRUE;
                        CsrDhcpServerLeasesGetReqSend(inst->serverHandle);
                        inst->busy = TRUE;
                    }
                }
                else if (*type == CSR_UI_UIE_CREATE_CFM)
                {
                    CsrUiUieCreateCfm *confirm;

                    confirm = (CsrUiUieCreateCfm *) message;

                    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
                    {
                        if (!inst->HndlbackEvent)
                        {
                            inst->HndlbackEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->HndlbackEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_DHCP_SERVER_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_EVENT);
                        }
                        else if (!inst->HndlStartStopEvent)
                        {
                            inst->HndlStartStopEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->HndlStartStopEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_DHCP_SERVER_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_EVENT);
                        }
                        else if (!inst->HndlCreateDestroyEvent)
                        {
                            inst->HndlCreateDestroyEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->HndlCreateDestroyEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_DHCP_SERVER_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_EVENT);
                        }
                        else if (!inst->HndlCurrentLeasesEvent)
                        {
                            inst->HndlCurrentLeasesEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->HndlCurrentLeasesEvent,
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_DHCP_SERVER_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_MENU);
                        }
                    }
                    else if (confirm->elementType == CSR_UI_UIETYPE_MENU)
                    {
                        if (!inst->hMainMenu)
                        {
                            inst->hMainMenu = confirm->handle;

                            CsrUiMenuSetReqSend(inst->hMainMenu,
                                CsrUtf82Utf16String((CsrUtf8String *) "DHCP Server"),
                                CsrUtf82Utf16String((CsrUtf8String *) "Select"), NULL);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_MENU);
                        }
                        else if (!inst->hCurrentLeasesMenu)
                        {
                            inst->hCurrentLeasesMenu = confirm->handle;

                            CsrUiMenuSetReqSend(inst->hCurrentLeasesMenu,
                                CsrUtf82Utf16String((CsrUtf8String *) "Current leases"),
                                NULL, NULL);

                            CsrUiUieCreateReqSend(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_DIALOG);
                        }
                    }
                    else if (confirm->elementType == CSR_UI_UIETYPE_DIALOG)
                    {
                        if (!inst->hMainDialog)
                        {
                            inst->hMainDialog = confirm->handle;

                            CsrAppRegisterReqSend2(CSR_DHCP_SERVER_MANAGER_IFACEQUEUE,
                                (CsrUtf8String *) CsrStrDup("Network"),
                                (CsrUtf8String *) CsrStrDup("DHCP Server Manager"),
                                CSR_UI_ICON_CONNECT,
                                CSR_UI_ICON_NONE,
                                TRUE);
                        }
                    }
                    else
                    {
                        CSR_LOG_TEXT_ERROR((CsrDhcpServerManagerLto, 0, "[%s:%d] Unknown CSR_UI elementType (0x%X)", __FILE__, __LINE__, confirm->elementType));
                    }
                }
                else
                {
                    CSR_LOG_TEXT_ERROR((CsrDhcpServerManagerLto, 0, "[%s:%d] Unknown CSR_UI_PRIM (0x%X)", __FILE__, __LINE__, *type));
                }

                break;
            }
        }

        CsrPmemFree(message);
    }
}
