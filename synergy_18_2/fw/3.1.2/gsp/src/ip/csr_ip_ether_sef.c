/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_ip_handler.h"
#include "csr_ip_util.h"
#include "csr_ip_task.h"
#include "csr_ip_ether_prim.h"
#include "csr_ip_ether_lib.h"
#include "csr_ip_internal_prim.h"
#include "csr_ip_internal_lib.h"
#include "csr_ip_ifconfig_prim.h"
#include "csr_ip_ifconfig_lib.h"
#include "csr_log_text_2.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include "lwip/dhcp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/dns.h"
#include "lwip/igmp.h"
#include "netif/etharp.h"

static err_t linkoutput(struct netif *netif, struct pbuf *p)
{
    CsrIpEtherInstanceData *etherInstance = netif->state;
    struct pbuf *frame;
    CsrUint16 frameLength;

    if (!(netif->flags & NETIF_FLAG_LINK_UP))
    {
        return ERR_OK;
    }
    else if (p->tot_len > 0xffff)
    {
        /* If the frame is larger than 16 bits, we can't send it. */
        return ERR_IF;
    }
    else
    {
        frameLength = (CsrUint16) p->tot_len;
    }

    if (p->len < frameLength)
    {
        struct pbuf *ptr;
        CsrUint16 offset = 0;

        /*
         * If frame is spread across multiple buffers,
         * coalesce it into a single, contiguous buffer.
         */

        frame = pbuf_alloc(PBUF_RAW, frameLength, PBUF_RAM);
        frame->next = NULL;

        for (ptr = p; ptr != NULL; ptr = ptr->next)
        {
            CsrMemCpy((CsrUint8 *) frame->payload + offset, ptr->payload, ptr->len);
            offset += ptr->len;
        }
    }
    else
    {
        frame = p;
    }

    if (etherInstance->frameTxFunction)
    {
        CsrUint8 *payload = frame->payload;

        (void) etherInstance->frameTxFunction(payload, payload + 6, (CsrUint16) (frameLength - 12), payload + 12, etherInstance->ifContext);
    }

    return ERR_OK;
}

static err_t igmp_mac_filter(struct netif *netif, struct ip_addr *group, u8_t action)
{
    CsrIpEtherInstanceData *etherInstance = netif->state;
    CsrUint8 *multicastIpAddress = (CsrUint8 *) &group->addr;
    CsrUint8 multicastMacAddress[6] = {0x01, 0x00, 0x5E, 0x00, 0x00, 0x00};

    /* group->addr is in network byte order, just index the bytes directly */
    multicastMacAddress[3] = multicastIpAddress[1] & 0x7F;
    multicastMacAddress[4] = multicastIpAddress[2];
    multicastMacAddress[5] = multicastIpAddress[3];

    switch (action)
    {
        case IGMP_ADD_MAC_FILTER:
            CsrIpEtherIfMulticastAddrAddIndSend(etherInstance->queue, etherInstance->ifHandle, multicastMacAddress);
            break;
        case IGMP_DEL_MAC_FILTER:
            CsrIpEtherIfMulticastAddrRemoveIndSend(etherInstance->queue, etherInstance->ifHandle, multicastMacAddress);
            break;
        default:
            break;
    }

    return ERR_OK;
}

static struct netif *eligibleGatewayInterfaceFind(CsrIpInstanceData *inst)
{
    CsrUint8 ifIdx;
    struct netif *netif;

    for (ifIdx = 0; ifIdx < CSR_IP_MAX_INTERFACES; ifIdx++)
    {
        netif = inst->interfaces[ifIdx];

        if (netif != NULL)
        {
            if (netif_is_up(netif) && !ip_addr_isany(&netif->gw))
            {
                break;
            }
            else
            {
                netif = NULL;
            }
        }
    }

    return netif;
}

static err_t netifInit(struct netif *netif)
{
    /* Interface name */
    netif->name[0] = 'e';
    netif->name[1] = 'n';
    netif->output = etharp_output;
    netif->linkoutput = linkoutput;
    netif->igmp_mac_filter = igmp_mac_filter;
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;
    return ERR_OK;
}

static void netifStateChange(struct netif *netif)
{
    CsrIpEtherInstanceData *etherInstance = netif->state;
    CsrIpInstanceData *instanceData = etherInstance->instanceData;
    CsrUint8 newState;
    CsrIpIfconfigIfDetails ifDetails;
    struct ip_addr dns1 = dns_getserver(0);
    struct ip_addr dns2 = dns_getserver(1);

    if ((netif->flags & NETIF_FLAG_UP) && (netif->flags & NETIF_FLAG_LINK_UP))
    {
        /* This means that an IP address has been received and the link is up */
        newState = CSR_IP_IFCONFIG_IF_STATE_UP;
    }
    else if ((!(netif->flags & NETIF_FLAG_UP)) && (netif->flags & NETIF_FLAG_LINK_UP))
    {
        /* This means that the link is up but no IP address is given */
        newState = CSR_IP_IFCONFIG_IF_STATE_DOWN;

        if (instanceData->defaultInterface == netif)
        {
            instanceData->defaultInterface = NULL;
        }
    }
    else if (!(netif->flags & NETIF_FLAG_LINK_UP))
    {
        /* This means that the link is down (disconnected), so the NETIF_FLAG_UP is irelevant */
        newState = CSR_IP_IFCONFIG_IF_STATE_MEDIA_DISCONNECTED;

        if (instanceData->defaultInterface == netif)
        {
            instanceData->defaultInterface = NULL;
        }
    }
    else
    {
        newState = CSR_IP_IFCONFIG_IF_STATE_UNAVAILABLE;

        if (instanceData->defaultInterface == netif)
        {
            instanceData->defaultInterface = NULL;
        }
    }

    if (instanceData->defaultInterface == NULL)
    {
        instanceData->defaultInterface = eligibleGatewayInterfaceFind(instanceData);
        netif_set_default(instanceData->defaultInterface);
    }

    if (etherInstance->ifState != newState)
    {
        CsrIpIfconfigListener *listeners;

        CsrIpUtilLwipToCsrIp(&(netif->ip_addr), ifDetails.ipAddress);
        CsrIpUtilLwipToCsrIp(&(netif->netmask), ifDetails.networkMask);
        CsrIpUtilLwipToCsrIp(&(netif->gw), ifDetails.gatewayIpAddress);
        CsrIpUtilLwipToCsrIp(&dns1, ifDetails.dnsPrimaryIpAddress);
        CsrIpUtilLwipToCsrIp(&dns2, ifDetails.dnsSecondaryIpAddress);
        ifDetails.mac[0] = netif->hwaddr[0];
        ifDetails.mac[1] = netif->hwaddr[1];
        ifDetails.mac[2] = netif->hwaddr[2];
        ifDetails.mac[3] = netif->hwaddr[3];
        ifDetails.mac[4] = netif->hwaddr[4];
        ifDetails.mac[5] = netif->hwaddr[5];
        ifDetails.ifHandle = etherInstance->ifHandle;
        ifDetails.ifType = etherInstance->ifType;
        ifDetails.ifState = newState;
        etherInstance->ifState = newState;

        listeners = instanceData->ifconfigListeners;
        while (listeners != NULL)
        {
            /* Sending updates to all the subscribers */
            CsrIpIfconfigUpdateIndSend(listeners->queue, ifDetails);
            listeners = listeners->next;
        }
    }
}

static void frameRxFunction(CsrUint8 *destinationMac, CsrUint8 *sourceMac, CsrInt16 rssi, CsrUint16 frameLength, CsrUint8 *frame, CsrIpEtherIpContext ipContext)
{
    struct pbuf *buffer;
    CsrUint16 ethType = (frame[0] << 8) | frame[1];

    switch (ethType)
    {
        case ETHTYPE_IP:
        {
            buffer = pbuf_alloc(PBUF_IP, (u16_t) (frameLength - 2), PBUF_RAM);
            CsrMemCpy(buffer->payload, frame + 2, frameLength - 2);
            break;
        }
        case ETHTYPE_ARP:
        {
            CsrUint8 *payload;
            buffer = pbuf_alloc(PBUF_LINK, (u16_t) (frameLength + 12), PBUF_RAM);
            payload = buffer->payload;
            CsrMemCpy(payload, destinationMac, 6);
            CsrMemCpy(payload + 6, sourceMac, 6);
            CsrMemCpy(payload + 12, frame, frameLength);
            break;
        }
        default:
        {
            return;
        }
    }

    CsrIpInternalEtherPacketReqSend(ethType, buffer, (struct netif **) ipContext);
}

void CsrIpEtherIfAddReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfAddReq *request = (CsrIpEtherIfAddReq *) (instanceData->msg);
    struct ip_addr noip = {0};
    struct netif *netif;
    CsrIpEtherInstanceData *etherInstance;
    CsrUint32 ifHandle = 0;
    CsrIpIfconfigListener *listeners;

    /* Allocate netif and assign handle */
    netif = CsrPmemZalloc(sizeof(*netif));
    if (CsrIpUtilHandleAllocate(instanceData, netif, &ifHandle) != CSR_RESULT_SUCCESS)
    {
        CsrPmemFree(netif);
        CsrIpEtherIfAddCfmSend(request->appHandle, ifHandle, CSR_IP_ETHER_RESULT_NO_MORE_INTERFACES, CSR_IP_ETHER_ENCAPSULATION_ETHERNET, NULL, NULL);
        return;
    }

    /* Allocate private section of netif */
    etherInstance = CsrPmemAlloc(sizeof(*etherInstance));
    etherInstance->queue = request->appHandle;
    etherInstance->ifHandle = ifHandle;
    etherInstance->ifContext = request->ifContext;
    etherInstance->frameTxFunction = request->frameTxFunction;
    etherInstance->instanceData = instanceData;
    etherInstance->ifType = request->ifType;
    etherInstance->ifState = CSR_IP_IFCONFIG_IF_STATE_MEDIA_DISCONNECTED;

    /* Set hardware address */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;
    netif->hwaddr[0] = request->mac[0];
    netif->hwaddr[1] = request->mac[1];
    netif->hwaddr[2] = request->mac[2];
    netif->hwaddr[3] = request->mac[3];
    netif->hwaddr[4] = request->mac[4];
    netif->hwaddr[5] = request->mac[5];

    /* Set maximum transfer unit */
    netif->mtu = request->maxTxUnit;

    /* Add to lwIP */
    netif_add(netif, &noip, &noip, &noip, etherInstance, netifInit, ip_input);

    /* Set callbacks */
    netif_set_status_callback(netif, netifStateChange);
    netif_set_link_callback(netif, netifStateChange);

    CsrIpEtherIfAddCfmSend(request->appHandle, ifHandle, CSR_RESULT_SUCCESS, CSR_IP_ETHER_ENCAPSULATION_ETHERNET, frameRxFunction, (CsrIpEtherIpContext) & instanceData->interfaces[ifHandle]);

    /* Send update to listeners. */
    listeners = instanceData->ifconfigListeners;
    if (listeners)
    {
        CsrIpIfconfigIfDetails tmpDetails;
        struct ip_addr dns1;
        struct ip_addr dns2;

        dns1 = dns_getserver(0);
        dns2 = dns_getserver(1);

        CsrIpUtilLwipToCsrIp(&(netif->ip_addr), tmpDetails.ipAddress);
        CsrIpUtilLwipToCsrIp(&(netif->netmask), tmpDetails.networkMask);
        CsrIpUtilLwipToCsrIp(&(netif->gw), tmpDetails.gatewayIpAddress);
        CsrIpUtilLwipToCsrIp(&(dns1), tmpDetails.dnsPrimaryIpAddress);
        CsrIpUtilLwipToCsrIp(&(dns2), tmpDetails.dnsSecondaryIpAddress);
        CsrMemCpy(tmpDetails.mac, netif->hwaddr, 6);
        tmpDetails.ifType = etherInstance->ifType;
        tmpDetails.ifState = etherInstance->ifState;
        tmpDetails.ifHandle = etherInstance->ifHandle;

        do
        {
            CsrIpIfconfigUpdateIndSend(listeners->queue, tmpDetails);
            listeners = listeners->next;
        } while (listeners != NULL);
    }
}

void CsrIpEtherIfRemoveReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfRemoveReq *request = (CsrIpEtherIfRemoveReq *) (instanceData->msg);

    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];
        CsrIpIfconfigListener *listeners;

        /* Release handle */
        CsrIpUtilHandleRelease(instanceData, request->ifHandle);

        /* Remove from lwIP */
        netif_remove(netif);

        if (netif->dhcp != NULL)
        {
            CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_ETHER, "[%u] Stop DHCP", request->ifHandle));
            dhcp_stop(netif);
        }

        if (netif->autoip != NULL)
        {
            CsrPmemFree(netif->autoip);
        }

        CsrPmemFree(netif->state);
        CsrPmemFree(netif);

        CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);

        listeners = instanceData->ifconfigListeners;
        if (listeners)
        {
            CsrIpIfconfigIfDetails tmpDetails;

            CsrMemSet(&tmpDetails, 0, sizeof(tmpDetails));
            /* Only handle and state need to be set. */
            tmpDetails.ifHandle = request->ifHandle;
            tmpDetails.ifState = CSR_IP_IFCONFIG_IF_STATE_UNAVAILABLE;

            do
            {
                CsrIpIfconfigUpdateIndSend(listeners->queue, tmpDetails);
                listeners = listeners->next;
            } while (listeners != NULL);
        }
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_ETHER, "[%u] Remove failed (invalid handle)", request->ifHandle));
        CsrIpEtherIfRemoveCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
    }
}

void CsrIpEtherIfLinkUpReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkUpReq *request = (CsrIpEtherIfLinkUpReq *) (instanceData->msg);

    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];

        /* Indicate to lwIP that the link is up */
        netif_set_link_up(netif);

        CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_ETHER, "[%u] LinkUp failed (invalid handle)", request->ifHandle));
        CsrIpEtherIfLinkUpCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
    }
}

void CsrIpEtherIfLinkDownReqHandler(CsrIpInstanceData *instanceData)
{
    CsrIpEtherIfLinkDownReq *request = (CsrIpEtherIfLinkDownReq *) (instanceData->msg);

    if ((request->ifHandle < CSR_IP_MAX_INTERFACES) && (instanceData->interfaces[request->ifHandle] != NULL))
    {
        struct netif *netif = instanceData->interfaces[request->ifHandle];

        /* Indicate to lwIP that the link is down */
        netif_set_link_down(netif);

        if (netif->dhcp != NULL)
        {
            CSR_LOG_TEXT_DEBUG((CsrIpLto, CSR_IP_LTSO_ETHER, "[%u] Stop DHCP", request->ifHandle));
            dhcp_release(netif);
            dhcp_stop(netif);
        }

        CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_RESULT_SUCCESS);
    }
    else
    {
        CSR_LOG_TEXT_ERROR((CsrIpLto, CSR_IP_LTSO_ETHER, "[%u] LinkDown failed (invalid handle)", request->ifHandle));
        CsrIpEtherIfLinkDownCfmSend(request->appHandle, request->ifHandle, CSR_IP_ETHER_RESULT_INVALID_HANDLE);
    }
}

void CsrIpEtherIfMulticastAddrAddResHandler(CsrIpInstanceData *instanceData)
{
}

void CsrIpEtherIfMulticastAddrRemoveResHandler(CsrIpInstanceData *instanceData)
{
}

void CsrIpEtherIfMulticastAddrFlushResHandler(CsrIpInstanceData *instanceData)
{
}

void CsrIpEtherIfFlowControlPauseReqHandler(CsrIpInstanceData *instanceData)
{
}

void CsrIpEtherIfFlowControlResumeReqHandler(CsrIpInstanceData *instanceData)
{
}
