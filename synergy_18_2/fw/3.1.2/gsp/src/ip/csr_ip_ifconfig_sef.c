/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_ifconfig_lib.h"
#include "csr_ip_task.h"
#include "csr_ip_internal_lib.h"
#include "csr_ip_handler.h"
#include "lwip/opt.h"
#include "lwip/ip.h"
#include "lwip/netif.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/autoip.h"
#include "lwip/err.h"
#include "netif/etharp.h"
#include "csr_ip_util.h"
#include "csr_log_text_2.h"

void CsrIpIfconfigSubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigSubscribeReq *request = (CsrIpIfconfigSubscribeReq *) instanceData->msg;
    CsrUint16 numOfIf;
    CsrUint16 i, j;
    CsrIpIfconfigListener *listeners;
    CsrIpIfconfigIfDetails *ifDetails = NULL;
    CsrUint16 ifDetailsCount = 0;
    struct ip_addr dns1 = dns_getserver(0);
    struct ip_addr dns2 = dns_getserver(1);

    /* Append to list */
    listeners = instanceData->ifconfigListeners;
    instanceData->ifconfigListeners = CsrPmemAlloc(sizeof(CsrIpIfconfigListener));
    instanceData->ifconfigListeners->queue = request->appHandle;
    instanceData->ifconfigListeners->next = listeners;

    for (i = 0, numOfIf = 0; i < CSR_IP_MAX_INTERFACES; i++)
    {
        if (instanceData->interfaces[i] != NULL)
        {
            numOfIf++;
        }
    }

    if (numOfIf > 0)
    {
        ifDetails = CsrPmemAlloc(numOfIf * sizeof(CsrIpIfconfigIfDetails));
        ifDetailsCount = numOfIf;
    }

    for (i = 0, j = 0; i < CSR_IP_MAX_INTERFACES; i++)
    {
        struct netif *netif = instanceData->interfaces[i];
        if (netif != NULL)
        {
            CsrIpEtherInstanceData *etherInstance = netif->state;
            CsrIpIfconfigIfDetails *tmpDetails;
            tmpDetails = ifDetails + j;
            CsrIpUtilLwipToCsrIp(&netif->ip_addr, tmpDetails->ipAddress);
            CsrIpUtilLwipToCsrIp(&netif->netmask, tmpDetails->networkMask);
            CsrIpUtilLwipToCsrIp(&netif->gw, tmpDetails->gatewayIpAddress);
            CsrIpUtilLwipToCsrIp(&dns1, tmpDetails->dnsPrimaryIpAddress);
            CsrIpUtilLwipToCsrIp(&dns2, tmpDetails->dnsSecondaryIpAddress);
            CsrMemCpy(tmpDetails->mac, netif->hwaddr, 6);
            tmpDetails->ifType = etherInstance->ifType;
            tmpDetails->ifState = etherInstance->ifState;
            tmpDetails->ifHandle = etherInstance->ifHandle;
            j++;
        }
    }

    CsrIpIfconfigSubscribeCfmSend(request->appHandle, ifDetailsCount, ifDetails);
}

void CsrIpIfconfigUnsubscribeReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigUnsubscribeReq *request = (CsrIpIfconfigUnsubscribeReq *) instanceData->msg;
    CsrIpIfconfigListener *listeners = instanceData->ifconfigListeners;

    if (listeners != NULL)
    {
        if (listeners->queue == request->appHandle)
        {
            instanceData->ifconfigListeners = listeners->next;
            CsrPmemFree(listeners);
        }
        else
        {
            CsrIpIfconfigListener *listener = listeners;
            while (listener->next != NULL)
            {
                if (listener->next->queue == request->appHandle)
                {
                    listener->next = listener->next->next;
                    CsrPmemFree(listener->next);
                    break;
                }
                listener = listener->next;
            }
        }
    }

    CsrIpIfconfigUnsubscribeCfmSend(request->appHandle);
}

void CsrIpIfconfigUpReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigUpReq *request = (CsrIpIfconfigUpReq *) (instanceData->msg);

    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];
        CsrIpEtherInstanceData *etherInstance = netif->state;

        if (etherInstance->ifState == CSR_IP_IFCONFIG_IF_STATE_DOWN)
        {
            if (request->configMethod == CSR_IP_IFCONFIG_CONFIG_METHOD_DHCP)
            {
                CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Start DHCP", request->ifHandle));
                CsrIpIfconfigUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
                dhcp_start(netif);
            }
            else if (request->configMethod == CSR_IP_IFCONFIG_CONFIG_METHOD_AUTOIP)
            {
                CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Start AutoIP", request->ifHandle));
                CsrIpIfconfigUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
                autoip_start(netif);
            }
            else
            {
                struct ip_addr ipAddress = {0};
                struct ip_addr networkMask = {0};
                struct ip_addr gatewayIpAddress = {0};
                struct ip_addr dnsPrimaryIpAddress = {0};
                struct ip_addr dnsSecondaryIpAddress = {0};

                CsrIpUtilCsrIpToLwIp(request->ipAddress, &ipAddress);
                CsrIpUtilCsrIpToLwIp(request->networkMask, &networkMask);
                CsrIpUtilCsrIpToLwIp(request->gatewayIpAddress, &gatewayIpAddress);
                CsrIpUtilCsrIpToLwIp(request->dnsPrimaryIpAddress, &dnsPrimaryIpAddress);
                CsrIpUtilCsrIpToLwIp(request->dnsSecondaryIpAddress, &dnsSecondaryIpAddress);

                CsrIpIfconfigUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
                netif_set_addr(netif, &ipAddress, &networkMask, &gatewayIpAddress);
                netif_set_up(netif);
                dns_setserver(0, &dnsPrimaryIpAddress);
                dns_setserver(1, &dnsSecondaryIpAddress);
            }
            CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Up", request->ifHandle));
        }
        else
        {
            CSR_LOG_TEXT_INFO((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Up rejected (wrong state)", request->ifHandle));
            CsrIpIfconfigUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_FAILURE);
        }
    }
    else
    {
        CSR_LOG_TEXT_INFO((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Up rejected (invalid handle)", request->ifHandle));
        CsrIpIfconfigUpCfmSend(request->appHandle, request->ifHandle, CSR_IP_IFCONFIG_RESULT_INVALID_HANDLE);
    }
}

void CsrIpIfconfigDownReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigDownReq *request = (CsrIpIfconfigDownReq *) (instanceData->msg);

    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];
        CsrIpEtherInstanceData *etherInstance = netif->state;

        CsrIpIfconfigDownCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);

        if (etherInstance->ifState == CSR_IP_IFCONFIG_IF_STATE_UP)
        {
            if (netif->dhcp != NULL)
            {
                CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Stop DHCP", request->ifHandle));
                dhcp_release(netif);
                dhcp_stop(netif);
            }
            else
            {
                netif_set_down(netif);
                netif_set_addr(netif, IP_ADDR_ANY, IP_ADDR_ANY, IP_ADDR_ANY);
            }
            CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Down", request->ifHandle));
        }
        else
        {
            CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Down (already down)", request->ifHandle));
        }
    }
    else
    {
        CSR_LOG_TEXT_INFO((CsrIpLto, CSR_IP_LTSO_IFCONFIG, "[%u] Down rejected (invalid handle)", request->ifHandle));
        CsrIpIfconfigDownCfmSend(request->appHandle, request->ifHandle, CSR_IP_IFCONFIG_RESULT_INVALID_HANDLE);
    }
}

void CsrIpIfconfigNatReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigNatReq *request = instanceData->msg;

#if IP_FORWARD
    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];

        if (request->enable == FALSE)
        {
            netif_nat_control(netif, 0);
        }
        else
        {
            netif_nat_control(netif, 1);
        }

        CsrIpIfconfigNatCfmSend(request->appHandle, request->ifHandle,
            CSR_RESULT_SUCCESS);
    }
    else
    {
        CsrIpIfconfigNatCfmSend(request->appHandle, request->ifHandle,
            CSR_IP_IFCONFIG_RESULT_INVALID_HANDLE);
    }
#else
    CsrIpIfconfigNatCfmSend(request->appHandle, request->ifHandle,
        CSR_IP_IFCONFIG_RESULT_NOT_SUPPORTED);
#endif /* IP_FORWARD */
}

void CsrIpIfconfigForwardReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigForwardReq *request = instanceData->msg;

#if IP_FORWARD
    if (request->enable == FALSE)
    {
        ip_forward_control(FALSE);
    }
    else
    {
        ip_forward_control(TRUE);
    }

    CsrIpIfconfigForwardCfmSend(request->appHandle,
        CSR_RESULT_SUCCESS);
#else
    CsrIpIfconfigForwardCfmSend(request->appHandle,
        CSR_IP_IFCONFIG_RESULT_NOT_SUPPORTED);
#endif
}

void CsrIpIfconfigArpEntryAddReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpIfconfigArpEntryAddReq *request = instanceData->msg;
    struct ip_addr ipaddr;
    err_t err;

    CsrMemCpy(&ipaddr.addr, request->ipAddress, 4);

    err = etharp_entry_set(&ipaddr, request->mac);
    if (err == ERR_OK)
    {
        CsrIpIfconfigArpEntryAddCfmSend(request->appHandle,
            CSR_RESULT_SUCCESS);
    }
    else if (err == ERR_RTE)
    {
        CsrIpIfconfigArpEntryAddCfmSend(request->appHandle,
            CSR_IP_IFCONFIG_RESULT_UNKNOWN_NETWORK);
    }
    else
    {
        CsrIpIfconfigArpEntryAddCfmSend(request->appHandle,
            CSR_RESULT_FAILURE);
    }
}
