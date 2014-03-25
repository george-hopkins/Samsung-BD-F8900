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

#include "csr_app_prim.h"
#include "csr_app_lib.h"

#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_ifconfig_lib.h"

#include "csr_ethernet_manager_task.h"

typedef struct
{
    CsrUieHandle hMainMenu;
    CsrUieHandle HndlbackEvent;

    CsrUint16  ifCount;
    CsrUint16 *ifHandles;
} CsrEthernetManagerInstanceData;

static void nicMenuAdd(CsrEthernetManagerInstanceData *inst,
    const CsrIpIfconfigIfDetails *ifd)
{
    CsrUint16 *newArray;
    CsrCharString ifDesc[sizeof("If 65535")];
    CsrCharString ipDesc[sizeof("255.255.255.255")];
    CsrSize ifDescSize = sizeof("If 65535");
    CsrSize ipDescSize = sizeof("255.255.255.255");

    newArray = CsrPmemAlloc(sizeof(*newArray) * (inst->ifCount + 1));

    if (inst->ifHandles != NULL)
    {
        CsrMemCpy(newArray, inst->ifHandles,
            sizeof(*newArray) * inst->ifCount);
        CsrPmemFree(inst->ifHandles);
    }
    newArray[inst->ifCount] = (CsrUint16) ifd->ifHandle;

    inst->ifHandles = newArray;
    inst->ifCount++;

    CsrSnprintf(ifDesc, ifDescSize, "If %u", ifd->ifHandle);

    if (ifd->ifState == CSR_IP_IFCONFIG_IF_STATE_DOWN)
    {
        CsrSnprintf(ipDesc, ipDescSize, "Down");
    }
    else if (ifd->ifState == CSR_IP_IFCONFIG_IF_STATE_UP)
    {
        CsrSnprintf(ipDesc,
            ipDescSize,
            "%u.%u.%u.%u",
            ifd->ipAddress[0],
            ifd->ipAddress[1],
            ifd->ipAddress[2],
            ifd->ipAddress[3]);
    }
    else
    {
        CsrSnprintf(ipDesc,
            ipDescSize,
            "No cable",
            ifd->ifHandle);
    }

    CsrUiMenuAdditemReqSend(inst->hMainMenu,
        CSR_UI_LAST,
        (CsrUint16) ifd->ifHandle,
        CSR_UI_ICON_SERVER,
        CsrUtf82Utf16String((CsrUtf8String *) ifDesc),
        CsrUtf82Utf16String((CsrUtf8String *) ipDesc),
        0,
        0,
        inst->HndlbackEvent,
        0);
}

static void nicMenuRemove(CsrEthernetManagerInstanceData *inst,
    const CsrIpIfconfigIfDetails *ifd)
{
    CsrUint16 *newArray;
    CsrUint16 idx, ifidx;

    CsrUiMenuRemoveitemReqSend(inst->hMainMenu, (CsrUint16) ifd->ifHandle);

    newArray = CsrPmemAlloc(sizeof(*newArray) *
        (inst->ifCount - 1));

    idx = 0;
    for (ifidx = 0; ifidx < inst->ifCount; ifidx++)
    {
        if (inst->ifHandles[ifidx] == ifd->ifHandle)
        {
            continue;
        }

        newArray[idx] = inst->ifHandles[ifidx];
        idx++;
    }

    CsrPmemFree(inst->ifHandles);
    inst->ifHandles = newArray;
    inst->ifCount--;
}

void CsrEthernetManagerInit(void **gash)
{
    CsrEthernetManagerInstanceData *inst;

    inst = CsrPmemZalloc(sizeof(*inst));
    *gash = inst;

    CsrUiUieCreateReqSend(CSR_ETHERNET_MANAGER_IFACEQUEUE,
        CSR_UI_UIETYPE_EVENT);
}

void CsrEthernetManagerDeinit(void **gash)
{
    CsrEthernetManagerInstanceData *inst;

    inst = (CsrEthernetManagerInstanceData *) *gash;
    CsrPmemFree(inst->ifHandles);
    CsrPmemFree(inst);
}

void CsrEthernetManagerHandler(void **gash)
{
    CsrEthernetManagerInstanceData *inst;
    void *message;
    CsrUint16 event;

    inst = (CsrEthernetManagerInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        switch (event)
        {
            case CSR_IP_IFCONFIG_PRIM:
            {
                CsrIpIfconfigPrim *type;

                type = (CsrIpIfconfigPrim *) message;

                if (*type == CSR_IP_IFCONFIG_SUBSCRIBE_CFM)
                {
                    CsrIpIfconfigSubscribeCfm *cfm;
                    CsrUint16 idx;

                    cfm = (CsrIpIfconfigSubscribeCfm *) message;

                    inst->ifHandles = NULL;
                    inst->ifCount = 0;

                    for (idx = 0; idx < cfm->ifDetailsCount; idx++)
                    {
                        CsrIpIfconfigIfDetails *ifd;

                        ifd = &cfm->ifDetails[idx];

                        if (ifd->ifType == CSR_IP_IFCONFIG_IF_TYPE_WIRED)
                        {
                            if (ifd->ifState == CSR_IP_IFCONFIG_IF_STATE_DOWN)
                            {
                                CsrIpIfconfigUpReqSend(
                                    CSR_ETHERNET_MANAGER_IFACEQUEUE,
                                    ifd->ifHandle,
                                    CSR_IP_IFCONFIG_CONFIG_METHOD_DHCP,
                                    NULL, NULL, NULL, NULL, NULL);
                            }
                        }

                        nicMenuAdd(inst, ifd);
                    }

                    CsrPmemFree(cfm->ifDetails);
                }
                else if (*type == CSR_IP_IFCONFIG_UPDATE_IND)
                {
                    CsrIpIfconfigUpdateInd *ind;
                    CsrIpIfconfigIfDetails *ifd;
                    CsrUint16 ifidx;

                    ind = (CsrIpIfconfigUpdateInd *) message;

                    ifd = &ind->ifDetails;

                    if (ifd->ifState == CSR_IP_IFCONFIG_IF_STATE_DOWN)
                    {
                        CsrIpIfconfigUpReqSend(CSR_ETHERNET_MANAGER_IFACEQUEUE,
                            ifd->ifHandle,
                            CSR_IP_IFCONFIG_CONFIG_METHOD_DHCP,
                            NULL, NULL, NULL, NULL, NULL);
                    }

                    for (ifidx = 0; ifidx < inst->ifCount; ifidx++)
                    {
                        if (ifd->ifHandle == inst->ifHandles[ifidx])
                        {
                            break;
                        }
                    }

                    if (ifidx == inst->ifCount)
                    {
                        /* New interface */
                        nicMenuAdd(inst, ifd);
                    }
                    else
                    {
                        /* Update existing interface */
                        nicMenuRemove(inst, ifd);
                        if (ifd->ifState != CSR_IP_IFCONFIG_IF_STATE_UNAVAILABLE)
                        {
                            nicMenuAdd(inst, ifd);
                        }
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
                    CsrIpIfconfigSubscribeReqSend(
                        CSR_ETHERNET_MANAGER_IFACEQUEUE);
                }
                else if (*type == CSR_APP_TAKE_CONTROL_IND)
                {
                    CsrUiUieShowReqSend(inst->hMainMenu,
                        CSR_ETHERNET_MANAGER_IFACEQUEUE,
                        CSR_UI_INPUTMODE_AUTOPASS,
                        1);
                }

                break;
            }

            case CSR_UI_PRIM:
            {
                CsrUint16 *type;

                type = (CsrUint16 *) message;

                if (*type == CSR_UI_UIE_CREATE_CFM)
                {
                    CsrUiUieCreateCfm *confirm;

                    confirm = (CsrUiUieCreateCfm *) message;

                    if (confirm->elementType == CSR_UI_UIETYPE_EVENT)
                    {
                        if (!inst->HndlbackEvent)
                        {
                            inst->HndlbackEvent = confirm->handle;

                            CsrUiEventSetReqSend(inst->HndlbackEvent,
                                CSR_UI_INPUTMODE_PASS,
                                CSR_ETHERNET_MANAGER_IFACEQUEUE);

                            CsrUiUieCreateReqSend(
                                CSR_ETHERNET_MANAGER_IFACEQUEUE,
                                CSR_UI_UIETYPE_MENU);
                        }
                    }
                    else if (confirm->elementType == CSR_UI_UIETYPE_MENU)
                    {
                        if (!inst->hMainMenu)
                        {
                            inst->hMainMenu = confirm->handle;

                            CsrUiMenuSetReqSend(inst->hMainMenu,
                                CsrUtf82Utf16String((CsrUtf8String *) "Ethernet Manager"),
                                CsrUtf82Utf16String((CsrUtf8String *) "Select"), NULL);

                            CsrAppRegisterReqSend2(
                                CSR_ETHERNET_MANAGER_IFACEQUEUE,
                                (CsrUtf8String *) CsrStrDup("Network"),
                                (CsrUtf8String *) CsrStrDup("Ethernet Manager"),
                                CSR_UI_ICON_CONNECT,
                                CSR_UI_ICON_NONE,
                                TRUE);
                        }
                    }
                }
                else if (*type == CSR_UI_EVENT_IND)
                {
                    CsrUiEventInd *indication = message;

                    if (indication->event == inst->HndlbackEvent)
                    {
                        CsrUiUieHideReqSend(indication->displayElement);
                    }
                }
                else if (*type == CSR_UI_KEYDOWN_IND)
                {
                    CsrUiKeydownInd *indication = message;

                    if ((indication->displayElement == inst->hMainMenu) &&
                        (indication->key == CSR_UI_KEY_BACK))
                    {
                        CsrUiUieHideReqSend(indication->displayElement);
                    }
                }
                break;
            }
        }

        CsrPmemFree(message);
    }
}
