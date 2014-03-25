/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_usr_config.h"

#include "csr_dhcp_server_handler.h"
#include "csr_dhcp_server_task.h"
#include "csr_dhcp_server_util.h"

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
#include "csr_ip_ifconfig_lib.h"
#endif

#include "csr_log_text_2.h"

static void csrDhcpServerTimerLeaseOffered(CsrDhcpServerData *server, CsrDhcpServerTimer *timer)
{
    if (timer->removeClient)
    {
        CSR_DHCP_SERVER_CLIENT_REMOVE(&server->clients, timer->client);
    }
    else
    {
        timer->client->info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE;
    }
}

static void csrDhcpServerTimerLeaseExpired(CsrDhcpServerTimer *timer)
{
    timer->client->info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE;
}

void CsrDhcpServerTimerExpired(CsrUint16 mi, void *mv)
{
    CsrDhcpServerData *server = (CsrDhcpServerData *) mv;
    CsrDhcpServerTimer *timer = server->timerList;
    CsrTimeUtc time;

    CsrTimeUtcGet(&time, NULL, NULL);

    while (timer)
    {
        if (timer->sec < time.sec)
        {
            CsrDhcpServerTimer *t = timer->next;

            if (timer->client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED)
            {
                csrDhcpServerTimerLeaseOffered(server, timer);
            }
            else if (timer->client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE)
            {
                csrDhcpServerTimerLeaseExpired(timer);
            }

            CsrDhcpServerTimerRemove(server, timer);
            timer = t;
        }
        else /* No more timers to check */
        {
            break;
        }
    }

    server->tid = CsrSchedTimerSet(CSR_DHCP_SERVER_TIMER_DELAY, CsrDhcpServerTimerExpired, 0, server);
}

static CsrDhcpServerClient *csrDhcpServerFindClient(CsrDhcpServerData *server, CsrDhcpServerClientInfo *info, CsrDhcpServerDhcpMessage *dhcpMsg)
{
    CsrCmnListElm_t *element;
    CsrDhcpServerClient *client;
    CsrDhcpServerClientInfo *cInfo;

    for (element = server->clients.first; element; element = element->next)
    {
        client = (CsrDhcpServerClient *) element;
        cInfo = client->info;

        if ((info->clientIdLen > 0) &&
            (info->clientIdLen == cInfo->clientIdLen) &&
            (CsrMemCmp(info->clientId, cInfo->clientId, info->clientIdLen) == 0))
        { /* The client has been found from the client id */
            return client;
        }
        else if ((dhcpMsg->hlen == CSR_DHCP_SERVER_MAC_ADDRESS_LEN) &&
                 (CsrMemCmp(dhcpMsg->chaddr, cInfo->lease.mac, CSR_DHCP_SERVER_MAC_ADDRESS_LEN) == 0))
        {
            if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->ciaddr)) /* The client does not have an IP address, but we have found a
                 client with same hardware-address, use this client */
            {
                return client;
            }
            else if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(cInfo->lease.ip, dhcpMsg->ciaddr)) /* The client has been found from the key (IP-subnet-number, hardware-address) */
            {
                return client;
            }
        }
    }

    return NULL;
}

static CsrDhcpServerClient *csrDhcpServerGetClientUpdateInfo(CsrDhcpServerData *server, CsrDhcpServerClientInfo *info, CsrDhcpServerDhcpMessage *dhcpMsg)
{
    CsrDhcpServerClient *client = csrDhcpServerFindClient(server, info, dhcpMsg);

    if (client)
    {
        if (client->info->clientIdLen > 0)
        {
            CsrPmemFree(client->info->clientId);
        }

        /* Update the saved info about the client */
        client->info->requestedLeaseTime = info->requestedLeaseTime;
        client->info->clientIdLen = info->clientIdLen;
        client->info->clientId = info->clientId;
        info->clientId = NULL;
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(client->info->requestedIpAddress, info->requestedIpAddress);

        return client;
    }
    else
    {
        return NULL;
    }
}

static CsrBool csrDhcpServerIpAddressNotUsedAndValid(CsrDhcpServerData *server, CsrUint8 *ip) /* An IP address can be associated with several clients, and we must iterate through all clients */
{
    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(ip))
    {
        return FALSE;
    }
    else if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(ip, server->ipAddress)) /* The IP address is the address of the DHCP server */
    {
        return FALSE;
    }

    if ((server->network[0] == (ip[0] & server->networkMask[0])) &&
        (server->network[1] == (ip[1] & server->networkMask[1])) &&
        (server->network[2] == (ip[2] & server->networkMask[2])) &&
        (server->network[3] == (ip[3] & server->networkMask[3])))
    { /* The IP address is in the subnet */
        CsrCmnListElm_t *element;
        CsrDhcpServerClient *client;
        CsrDhcpServerList *list = server->invalidIpList;

        while (list)
        {
            CsrUint8 *invalidIp = list->data;

            if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(invalidIp, ip)) /* The IP address is invalid */
            {
                return FALSE;
            }

            list = list->next;
        }

        for (element = server->clients.first; element; element = element->next)
        {
            client = (CsrDhcpServerClient *) element;

            if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(client->info->lease.ip, ip) &&
                ((client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE) ||
                 (client->info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED)))
            { /* The IP addres is in use */
                return FALSE;
            }
        }

        return TRUE;
    }

    return FALSE;
}

static CsrDhcpServerClientInfo *csrDhcpServerGetIpAddress(CsrDhcpServerData *server, CsrDhcpServerClientInfo *newInfo, CsrDhcpServerDhcpMessage *dhcpMsg)
{
    CsrDhcpServerClient *client;
    CsrDhcpServerClientInfo *clientInfo;
    CsrBool ipAddressFound = FALSE;
    CsrBool removeClientAgain = FALSE;

    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->giaddr) == FALSE) /* Not handling requests from another subnet */
    {
        return NULL;
    }

    /*
      DHCP_OFFER:
      Return the clients current address
      Return the clients previous address, if not already used and is in the servers pool
      Return the requested address if valid and not in used
      Return a new address
    */
    client = csrDhcpServerFindClient(server, newInfo, dhcpMsg);

    if (client)
    {
        clientInfo = client->info;

        if ((clientInfo->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE) ||
            (clientInfo->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED))
        {
            ipAddressFound = TRUE;
            clientInfo->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED;
        }
        else if (clientInfo->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE)
        {
            if (csrDhcpServerIpAddressNotUsedAndValid(server, clientInfo->lease.ip))
            {
                ipAddressFound = TRUE;
                clientInfo->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED;
            }
        }
    }
    else
    {
        client = CSR_DHCP_SERVER_CLIENT_ADD(&server->clients);
        removeClientAgain = TRUE;

        client->info->leaseSec = 0;
        client->info->leaseSet = 0;
    }

    /* Save the new info about the client */
    clientInfo = client->info;
    clientInfo->requestedLeaseTime = newInfo->requestedLeaseTime;

    if (clientInfo->clientIdLen > 0)
    {
        CsrPmemFree(clientInfo->clientId);
    }

    clientInfo->clientIdLen = newInfo->clientIdLen;
    clientInfo->clientId = newInfo->clientId;
    newInfo->clientId = NULL;
    CSR_DHCP_SERVER_COPY_IP_ADDRESS(clientInfo->requestedIpAddress, newInfo->requestedIpAddress);
    CsrMemCpy(clientInfo->lease.mac, dhcpMsg->chaddr, CSR_DHCP_SERVER_MAC_ADDRESS_LEN);

    if (ipAddressFound == FALSE) /* No address found */
    {
        if (csrDhcpServerIpAddressNotUsedAndValid(server, clientInfo->requestedIpAddress))
        {
            CSR_DHCP_SERVER_COPY_IP_ADDRESS(clientInfo->lease.ip, clientInfo->requestedIpAddress);
            ipAddressFound = TRUE;
            clientInfo->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED;
        }
        else
        {
            CsrUint32 i;
            CsrUint8 data[4];
            CsrUint32 broadcast;
            CsrUint32 network;
            CsrUint32 serverId;
            CsrUint32 ip;
            CsrUint32 tempIp = 0;
            CsrUint8 state;
            CsrCmnListElm_t *element;
            CsrDhcpServerClient *clientTemp;
            CsrDhcpServerList *list;
            CsrBool ipInUse = FALSE;

            data[0] = server->network[0] | ~server->networkMask[0];
            data[1] = server->network[1] | ~server->networkMask[1];
            data[2] = server->network[2] | ~server->networkMask[2];
            data[3] = server->network[3] | ~server->networkMask[3];

            broadcast = CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(data);
            network = CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(server->network);
            serverId = CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(server->ipAddress);

            for (i = network + 1; i < broadcast; i++)
            {
                if (i == serverId)
                {
                    continue;
                }

                list = server->invalidIpList;

                while (list)
                {
                    CsrUint8 *invalidIp = list->data;
                    ip = CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(invalidIp);
                    if (i == ip) /* The IP address is invalid */
                    {
                        ipInUse = TRUE;
                        break;
                    }

                    list = list->next;
                }

                if (ipInUse)
                {
                    ipInUse = FALSE;
                    continue;
                }

                for (element = server->clients.first; element; element = element->next)
                {
                    clientTemp = (CsrDhcpServerClient *) element;

                    ip = CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(clientTemp->info->lease.ip);

                    if (ip == i)
                    {
                        state = clientTemp->info->ipAddressState;

                        if ((tempIp == 0) && (state == CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE)) /* Save the address and use it if no other addresses have been found */
                        {
                            tempIp = i;
                        }

                        /* This ip is in use */
                        ipInUse = TRUE;
                        break;
                    }
                }

                if (ipInUse)
                {
                    ipInUse = FALSE;
                    continue;
                }

                if (ipInUse == FALSE) /* An unused address has been found */
                {
                    CSR_DHCP_SERVER_UINT32_TO_IP_ADDRESS(i, clientInfo->lease.ip);
                    ipAddressFound = TRUE;
                    clientInfo->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED;
                    break;
                }
            }

            if ((ipAddressFound == FALSE) && (tempIp != 0)) /* No ip address found, but an inactive has been found */
            {
                CSR_DHCP_SERVER_UINT32_TO_IP_ADDRESS(tempIp, clientInfo->lease.ip);
                ipAddressFound = TRUE;
                clientInfo->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED;
            }
        }
    }

    if (ipAddressFound) /* Start a timer to either set the state to inactive or remove the client again */
    {
        CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

        if (timer) /* Cancel the timer for the old message */
        {
            CsrDhcpServerTimerRemove(server, timer);
        }

        CsrDhcpServerTimerAdd(server, client, CSR_DHCP_SERVER_TIMER_DHCPOFFER_TIMEOUT, removeClientAgain);

        return clientInfo;
    }

    return NULL;
}

static void csrDhcpServerFreeOptions(CsrDhcpServerDhcpOption *options)
{
    while (options)
    {
        CsrDhcpServerDhcpOption *tmp = options->next;
        CsrPmemFree(options->data);
        CsrPmemFree(options);
        options = tmp;
    }
}

static CsrBool csrDhcpServerGetOptions(CsrDhcpServerData *server, CsrUint8 *data, CsrUint16 dataLength, CsrUint8 *msgType, CsrDhcpServerDhcpOption **optionList)
{
    CsrUint8 code;
    CsrUint8 len;
    CsrUint16 offset;
    CsrDhcpServerDhcpOption *options;
    CsrBool valid = TRUE;

    options = NULL;
    *optionList = NULL;

    *msgType = CSR_DHCP_SERVER_DHCP_MSG_TYPE_INVALID;

    if (dataLength <= CSR_DHCP_SERVER_DHCP_MSG_SIZE_FIXED + CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC + CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_TYPE) /* Did not receive enough data, ignore the packet */
    {
        return FALSE;
    }

    if (CsrMemCmp(&data[CSR_DHCP_SERVER_DHCP_MSG_OFFSET_OPTION_MAGIC], CSR_DHCP_SERVER_DHCP_MSG_MAGIC_COOKIE, 4)) /* 'Magic cookie' does NOT match */
    {
        return FALSE;
    }

    offset = CSR_DHCP_SERVER_DHCP_MSG_SIZE_FIXED + CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC;

    while (offset < dataLength)
    {
        code = data[offset];

        if (code == CSR_DHCP_SERVER_DHCP_MSG_OPTION_PAD) /* The PAD option does not have a length field. Skip this option */
        {
            offset++;
            continue;
        }
        else if (code == CSR_DHCP_SERVER_DHCP_MSG_OPTION_END) /* The END options does not have a length field. Done parsing options */
        {
            break;
        }

        offset++;

        if (offset >= dataLength) /* The data is not valid, must be space for the length field */
        {
            valid = FALSE;
            break;
        }

        len = data[offset];
        offset++;

        if (offset + len > dataLength) /* The data is not valid */
        {
            valid = FALSE;
            break;
        }

        if ((code == CSR_DHCP_SERVER_DHCP_MSG_OPTION_MSG_TYPE) && (len == 1))
        {
            *msgType = data[offset];
            offset++;
        }
        else
        {
            CsrDhcpServerDhcpOption *opt = (CsrDhcpServerDhcpOption *) CsrPmemAlloc(sizeof(CsrDhcpServerDhcpOption));
            opt->next = NULL;
            opt->code = code;
            opt->len = len;
            opt->data = CsrPmemAlloc(len);
            CsrMemCpy(opt->data, &data[offset], len);

            if (options)
            {
                options->next = opt;
                options = opt;
            }
            else
            {
                options = opt;
                *optionList = opt;
            }

            offset += len;
        }
    }

    if ((valid == FALSE) ||
        (*msgType == CSR_DHCP_SERVER_DHCP_MSG_TYPE_INVALID))
    {
        /* Free allocated options data */
        csrDhcpServerFreeOptions(*optionList);
        *optionList = NULL;

        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

static void csrDhcpServerAddOption(CsrDhcpServerDhcpOption **options, CsrUint8 code, CsrUint8 len, CsrUint8 *data)
{
    CsrDhcpServerDhcpOption *lastOption = NULL;
    CsrDhcpServerDhcpOption *firstOption = *options;
    CsrDhcpServerDhcpOption *opt = CsrPmemAlloc(sizeof(CsrDhcpServerDhcpOption));

    opt->next = NULL;
    opt->code = code;
    opt->len = len;
    opt->data = data;

    while (firstOption)
    {
        lastOption = firstOption;
        firstOption = firstOption->next;
    }

    if (lastOption)
    {
        lastOption->next = opt;
    }
    else
    {
        *options = opt;
    }
}

static void csrDhcpServerAddOptionMessageType(CsrDhcpServerDhcpOption **options, CsrUint8 type)
{
    CsrUint8 *data = CsrPmemAlloc(1);

    data[0] = type;

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_MSG_TYPE, 1, data);
}

static CsrUint32 csrDhcpServerCalculateRequestedLease(CsrUint32 requestedLease, CsrTimeUtc *time)
{
    CsrUint32 sec;

    if (requestedLease == CSR_DHCP_SERVER_LEASE_INFINITE_SEC) /* Infinite lease */
    {
        time->sec = CSR_DHCP_SERVER_LEASE_INFINITE_SEC;
        time->msec = CSR_DHCP_SERVER_LEASE_INFINITE_MSEC;
        sec = CSR_DHCP_SERVER_LEASE_INFINITE_SEC;
    }
    else
    {
        if (requestedLease < 3600)
        {
            sec = 3600;
        }
        else if (requestedLease > 918000)
        {
            sec = 918000;
        }
        else
        {
            sec = requestedLease - (requestedLease % 3600);

            if (sec == 0) /* Make sure that the client gets at least 1 hour */
            {
                sec = 1;
            }
        }

        CsrTimeUtcGet(time, NULL, NULL);
        time->sec += sec;
        time->msec = 0;
    }

    return sec;
}

static CsrUint32 csrDhcpServerAddOptionLease(CsrDhcpServerDhcpOption **options, CsrDhcpServerData *server, CsrDhcpServerClientInfo *info, CsrBool reNew)
{
    CsrTimeUtc time;
    CsrUint32 sec;
    CsrUint8 *data = CsrPmemAlloc(4);

    /* Through the API it is possible to have a lease from 1 hour (3600 seconds)
       to 255 hours (918000 seconds) and only in hours, or a infinite lease */
    if (info->requestedLeaseTime > 0)
    {
        sec = csrDhcpServerCalculateRequestedLease(info->requestedLeaseTime, &time);
    }
    else
    {
        if (reNew == FALSE) /* Reuse the expiration time previously assigned */
        {
            CsrTimeUtcGet(&time, NULL, NULL);

            if (info->leaseSec > 0)
            {
                sec = info->leaseSec;
            }
            else if (info->lease.expiryTime.sec > time.sec)
            {
                sec = info->lease.expiryTime.sec - time.sec;
            }
            else
            {
                sec = 0;
            }

            time = info->lease.expiryTime;
        }
        else if (info->leaseSet > 0)
        {
            sec = csrDhcpServerCalculateRequestedLease(info->leaseSet, &time);
        }
        else
        {
            CsrTimeUtcGet(&time, NULL, NULL);
            time.sec += server->leaseTime * 3600;
            time.msec = 0;
            sec = server->leaseTime * 3600;
        }
    }

    info->lease.expiryTime = time;
    info->leaseSec = sec;

    CSR_COPY_UINT32_TO_BIG_ENDIAN(sec, data);

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_LEASE, 4, data);

    return sec;
}

static void csrDhcpServerAddOptionRenewalTimeValue(CsrDhcpServerDhcpOption **options, CsrDhcpServerData *server, CsrUint32 sec)
{
    if (sec > 0)
    {
        CsrUint8 *data = CsrPmemAlloc(4);
        CsrUint32 t1;

        t1 = (CsrUint32) (sec / 2);
        CSR_COPY_UINT32_TO_BIG_ENDIAN(t1, data);

        csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_RENEWAL_TIME_VALUE, 4, data);
    }
}

static void csrDhcpServerAddOptionRebindingTimeValue(CsrDhcpServerDhcpOption **options, CsrDhcpServerData *server, CsrUint32 sec)
{
    if (sec > 0)
    {
        CsrUint8 *data = CsrPmemAlloc(4);
        CsrUint32 t1;

        t1 = (CsrUint32) ((sec * 7) / 8);
        CSR_COPY_UINT32_TO_BIG_ENDIAN(t1, data);

        csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_REBINDING_TIME_VALUE, 4, data);
    }
}

static void csrDhcpServerAddOptionServerIdentifier(CsrDhcpServerDhcpOption **options, CsrUint8 *id)
{
    CsrUint8 *data = CsrPmemAlloc(4);

    CSR_DHCP_SERVER_COPY_IP_ADDRESS(data, id);

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_SERVER_IDENTIFIER, 4, data);
}

static void csrDhcpServerAddOptionSubnetMask(CsrDhcpServerDhcpOption **options, CsrUint8 *subnetMask)
{
    CsrUint8 *data = CsrPmemAlloc(4);

    CSR_DHCP_SERVER_COPY_IP_ADDRESS(data, subnetMask);

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_SUBNET_MASK, 4, data);
}

static void csrDhcpServerAddOptionRouter(CsrDhcpServerDhcpOption **options, CsrUint8 *router)
{
    CsrUint8 *data = CsrPmemAlloc(4);

    CSR_DHCP_SERVER_COPY_IP_ADDRESS(data, router);

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_ROUTER, 4, data);
}

static void csrDhcpServerAddOptionDomainNameServer(CsrDhcpServerDhcpOption **options, CsrUint8 *dns1, CsrUint8 *dns2)
{
    CsrUint8 *data;
    CsrUint8 dataLen = 0;
    CsrUint8 offset = 0;
    CsrBool addDns1 = FALSE;
    CsrBool addDns2 = FALSE;

    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dns1) == FALSE)
    {
        dataLen += 4;
        addDns1 = TRUE;
    }

    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dns2) == FALSE)
    {
        dataLen += 4;
        addDns2 = TRUE;
    }

    if (dataLen > 0)
    {
        data = CsrPmemAlloc(dataLen);

        if (addDns1)
        {
            CSR_DHCP_SERVER_COPY_IP_ADDRESS(&data[offset], dns1);
            offset += 4;
        }

        if (addDns2)
        {
            CSR_DHCP_SERVER_COPY_IP_ADDRESS(&data[offset], dns2);
        }

        csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_DOMAIN_NAME_SERVER, dataLen, data);
    }
}

static void csrDhcpServerAddOptionBroadcastAddress(CsrDhcpServerDhcpOption **options, CsrUint8 *network, CsrUint8 *subnetMask)
{
    CsrUint8 *data = CsrPmemAlloc(4);

    data[0] = network[0] | ~subnetMask[0];
    data[1] = network[1] | ~subnetMask[1];
    data[2] = network[2] | ~subnetMask[2];
    data[3] = network[3] | ~subnetMask[3];

    csrDhcpServerAddOption(options, CSR_DHCP_SERVER_DHCP_MSG_OPTION_BROADCAST_ADDRESS, 4, data);
}

static void csrDhcpServerHandleOptionParameterList(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerDhcpOption **result)
{
    CsrUint8 i;
    CsrInt16 offSetSubnet = -1;
    CsrInt16 offSetRouter = -1;

    /* RFC 2132 Section 3.3: If both subnet mask and router are specified then subnet mask must be first */
    for (i = 0; i < option->len; i++)
    {
        if (option->data[i] == CSR_DHCP_SERVER_DHCP_MSG_OPTION_SUBNET_MASK)
        {
            offSetSubnet = i;
        }
        else if (option->data[i] == CSR_DHCP_SERVER_DHCP_MSG_OPTION_ROUTER)
        {
            offSetRouter = i;
        }
    }

    if ((offSetSubnet != -1) && (offSetRouter != -1) && (offSetRouter < offSetSubnet)) /* The router is before the subnet mask, swap place */
    {
        option->data[offSetSubnet] = CSR_DHCP_SERVER_DHCP_MSG_OPTION_ROUTER;
        option->data[offSetRouter] = CSR_DHCP_SERVER_DHCP_MSG_OPTION_SUBNET_MASK;
    }

    for (i = 0; i < option->len; i++)
    {
        switch (option->data[i])
        {
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_SUBNET_MASK:
            {
                csrDhcpServerAddOptionSubnetMask(result, server->networkMask);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_ROUTER:
            {
                csrDhcpServerAddOptionRouter(result, server->gateway);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_DOMAIN_NAME_SERVER:
            {
                csrDhcpServerAddOptionDomainNameServer(result, server->dns1, server->dns2);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_BROADCAST_ADDRESS:
            {
                csrDhcpServerAddOptionBroadcastAddress(result, server->network, server->networkMask);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

static void csrDhcpServerHandleOptions(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerDhcpOption **result)
{
    while (option)
    {
        switch (option->code)
        {
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_PARAMETER_REQUEST:
            {
                csrDhcpServerHandleOptionParameterList(server, option, result);
                break;
            }
            default:
            {
                break;
            }
        }

        option = option->next;
    }
}

static void csrDhcpServerHandleOptionRequestedIpAddress(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerClientInfo *info)
{
    if (option->len == 4)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(info->requestedIpAddress, option->data);
    }
    else
    {
        CsrMemSet(info->requestedIpAddress, 0, 4);
    }
}

static void csrDhcpServerHandleOptionLease(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerClientInfo *info)
{
    if (option->len == 4)
    {
        info->requestedLeaseTime = CSR_GET_UINT32_FROM_BIG_ENDIAN(option->data);
    }
}

static void csrDhcpServerHandleOptionServerIdentifier(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerClientInfo *info)
{
    if (option->len == 4)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(info->serverId, option->data);
    }
}

static void csrDhcpServerHandleOptionClientIdentifier(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerClientInfo *info)
{
    if (option->len > 0)
    {
        info->clientIdLen = option->len;
        info->clientId = option->data;
        option->data = NULL;
    }
}

static void csrDhcpServerGetClientInfoFromOptions(CsrDhcpServerData *server, CsrDhcpServerDhcpOption *option, CsrDhcpServerClientInfo *info)
{
    CsrMemSet(info, 0, sizeof(CsrDhcpServerClientInfo));

    while (option)
    {
        switch (option->code)
        {
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_REQUESTED_IP_ADDRESS:
            {
                csrDhcpServerHandleOptionRequestedIpAddress(server, option, info);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_LEASE:
            {
                csrDhcpServerHandleOptionLease(server, option, info);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_SERVER_IDENTIFIER:
            {
                csrDhcpServerHandleOptionServerIdentifier(server, option, info);
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_MSG_MAX_SIZE:
            {
                /* Not using this option as we will not send a packet larger than
                   576 octets, which is the minimum IP datagram size an IP host
                   must be prepared to accept */
                break;
            }
            case CSR_DHCP_SERVER_DHCP_MSG_OPTION_CLIENT_IDENTIFIER:
            {
                csrDhcpServerHandleOptionClientIdentifier(server, option, info);
                break;
            }
            default:
            {
                break;
            }
        }

        option = option->next;
    }
}

static void csrDhcpServerDhcpMessage2Data(CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrUint8 **data, CsrUint16 *dataLength)
{
    CsrDhcpServerDhcpOption *opt = options;
    CsrUint16 offset = 0;

    *dataLength = sizeof(CsrDhcpServerDhcpMessage) + CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC + 1; /* 1 for the END field */

    while (opt)
    {
        *dataLength += opt->len + 2; /* 1 for the code field and 1 for the length field */
        opt = opt->next;
    }

    *data = CsrPmemAlloc(*dataLength);
    CsrMemCpy(*data, dhcpMsg, sizeof(CsrDhcpServerDhcpMessage));
    offset += sizeof(CsrDhcpServerDhcpMessage);

    /* Add the ' Magic cookie' */
    CsrMemCpy(&((*data)[offset]), CSR_DHCP_SERVER_DHCP_MSG_MAGIC_COOKIE, CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC);
    offset += CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC;

    opt = options;

    while (opt)
    {
        (*data)[offset] = opt->code;
        (*data)[offset + 1] = opt->len;
        CsrMemCpy(&((*data)[offset + 2]), opt->data, opt->len);
        offset += opt->len + 2;
        opt = opt->next;
    }

    /* Add the END option */
    (*data)[offset] = CSR_DHCP_SERVER_DHCP_MSG_OPTION_END;

    csrDhcpServerFreeOptions(options);
    CsrPmemFree(dhcpMsg);
}

void CsrDhcpServerMessageSend(CsrDhcpServerData *server, CsrUint8 *ipAddress, CsrUint16 port, CsrUint16 dataLenght, CsrUint8 *data)
{
    CsrIpSocketUdpDataReqSend((CsrUint16) server->socketHandle, ipAddress, port, dataLenght, data);
    server->sendingData = TRUE;
}

void CsrDhcpServerNetworkBroadcastAddressRetrieve(CsrDhcpServerData *server, CsrUint8 ip[4])
{
    ip[0] = server->network[0] | ~server->networkMask[0];
    ip[1] = server->network[1] | ~server->networkMask[1];
    ip[2] = server->network[2] | ~server->networkMask[2];
    ip[3] = server->network[3] | ~server->networkMask[3];
}

static void csrDhcpServerMessageAckSend(CsrDhcpServerInstanceData *instanceData,
    CsrDhcpServerData *server, CsrUint8 *ipAddress, CsrDhcpServerDhcpMessage *dhcpMsg, CsrUint16 dataLenght, CsrUint8 *data)
{
    CsrUint8 ip[4] = {0, 0, 0, 0};
    CsrUint16 port = CSR_DHCP_SERVER_PORT_CLIENT;

    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->giaddr) == FALSE)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, dhcpMsg->giaddr);
        port = CSR_DHCP_SERVER_PORT_SERVER;
    }
    else if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->ciaddr) == FALSE)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, dhcpMsg->ciaddr);
    }
    else if (CSR_BIT_IS_SET(dhcpMsg->flags, CSR_DHCP_SERVER_BROADCAST_BIT))
    {
        CsrDhcpServerNetworkBroadcastAddressRetrieve(server, ip);
    }
    else if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(ipAddress) == FALSE)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, ipAddress);
    }
    else
    {
        CsrDhcpServerNetworkBroadcastAddressRetrieve(server, ip);
    }

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
    {
        /*
         * Unicast DHCPOFFER and DHCPACK messages to
         * the client's hardware address and 'yiaddr'
         * address.  We do this by trying to add an ARP
         * entry for the client IP address and then
         * doing a regular unicast to it later.
         */
        CsrDhcpServerResponse *respNew;

        respNew = CsrPmemAlloc(sizeof(*respNew));
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(respNew->ip, ip);
        respNew->port = port;
        respNew->data = data;
        respNew->dataLength = dataLenght;
        respNew->server = server;
        respNew->next = NULL;

        if (instanceData->responseQueue == NULL)
        {
            instanceData->responseQueue = respNew;
        }
        else
        {
            CsrDhcpServerResponse *resp;

            for (resp = instanceData->responseQueue;
                 resp != NULL;
                 resp = resp->next)
            {
                if (resp->next == NULL)
                {
                    break;
                }
            }

            resp->next = respNew;
        }
        CsrIpIfconfigArpEntryAddReqSend(CSR_DHCP_SERVER_IFACEQUEUE,
            dhcpMsg->chaddr, ip);
    }
#else
    CsrDhcpServerMessageSend(server, ip, port, dataLenght, data);
#endif
}

static void csrDhcpServerMessageOfferSend(CsrDhcpServerInstanceData *instanceData, CsrDhcpServerData *server, CsrUint8 *ipAddress, CsrDhcpServerDhcpMessage *dhcpMsg, CsrUint16 dataLenght, CsrUint8 *data)
{
    csrDhcpServerMessageAckSend(instanceData, server, ipAddress, dhcpMsg, dataLenght, data);
}

static void csrDhcpServerMessageNakSend(CsrDhcpServerData *server, CsrUint8 *ipAddress, CsrDhcpServerDhcpMessage *dhcpMsg, CsrUint16 dataLenght, CsrUint8 *data)
{
    CsrUint8 ip[4];
    CsrUint16 port = CSR_DHCP_SERVER_PORT_CLIENT;

    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(ipAddress) == FALSE)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, ipAddress);
    }
    else if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->giaddr) == FALSE)
    {
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, dhcpMsg->giaddr);
        port = CSR_DHCP_SERVER_PORT_SERVER;
    }
    else
    {
        CsrDhcpServerNetworkBroadcastAddressRetrieve(server, ip);
    }

    CsrDhcpServerMessageSend(server, ip, port, dataLenght, data);
}

static CsrDhcpServerDhcpMessage *csrDhcpServerAllocateDhcpMessage(CsrUint8 hlen, CsrUint8 htype, CsrUint32 xid, CsrUint16 secs, CsrUint16 flags,
    CsrUint8 *ciaddr, CsrUint8 *yiaddr, CsrUint8 *siaddr, CsrUint8 *giaddr,
    CsrUint8 *chaddr)
{
    CsrDhcpServerDhcpMessage *respondMsg = CsrPmemZalloc(sizeof(CsrDhcpServerDhcpMessage));

    respondMsg->op = CSR_DHCP_SERVER_DHCP_MSG_OP_BOOTREPLY;
    respondMsg->htype = htype;
    respondMsg->hlen = hlen;
    respondMsg->hops = CSR_DHCP_SERVER_DHCP_MSG_HOPS;
    respondMsg->xid = xid;
    respondMsg->secs = secs;
    respondMsg->flags = flags;
    CSR_DHCP_SERVER_COPY_IP_ADDRESS(respondMsg->ciaddr, ciaddr);
    CSR_DHCP_SERVER_COPY_IP_ADDRESS(respondMsg->yiaddr, yiaddr);
    CSR_DHCP_SERVER_COPY_IP_ADDRESS(respondMsg->siaddr, siaddr);
    CSR_DHCP_SERVER_COPY_IP_ADDRESS(respondMsg->giaddr, giaddr);
    CsrMemCpy(respondMsg->chaddr, chaddr, 16);

    return respondMsg;
}

static CsrBool csrDhcpServerMessageIsForServer(CsrDhcpServerData *server, CsrDhcpServerClientInfo *info, CsrBool *serverIdReceived)
{
    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(info->serverId)) /* Server id not received */
    {
        *serverIdReceived = FALSE;
        return TRUE;
    }
    else if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(info->serverId, server->ipAddress)) /* For this server */
    {
        *serverIdReceived = TRUE;
        return TRUE;
    }
    else /* Not for this server */
    {
        *serverIdReceived = TRUE;
        return FALSE;
    }
}

static void csrDhcpServerHandleMsgDhcpDiscover(CsrDhcpServerInstanceData *instanceData, CsrDhcpServerData *server, CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrIpSocketUdpDataInd *udpMsg)
{
    CsrDhcpServerClientInfo info;
    CsrDhcpServerClientInfo *clientInfo;

    csrDhcpServerGetClientInfoFromOptions(server, options, &info);

    clientInfo = csrDhcpServerGetIpAddress(server, &info, dhcpMsg);
    if (clientInfo != NULL)
    {
        CsrUint8 *data;
        CsrUint16 dataLenght;
        CsrDhcpServerDhcpMessage *respondMsg;
        CsrDhcpServerDhcpOption *respondOptions = NULL;
        CsrUint32 sec;
        CsrUint8 ip[] = {0, 0, 0, 0};

        respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, dhcpMsg->secs, dhcpMsg->flags,
            ip, clientInfo->lease.ip, ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

        csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPOFFER);
        csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
        sec = csrDhcpServerAddOptionLease(&respondOptions, server, clientInfo, TRUE);

        csrDhcpServerAddOptionRenewalTimeValue(&respondOptions, server, sec);
        csrDhcpServerAddOptionRebindingTimeValue(&respondOptions, server, sec);
        csrDhcpServerHandleOptions(server, options, &respondOptions);

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
        CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, respondMsg->yiaddr);
#endif
        csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

        csrDhcpServerMessageOfferSend(instanceData, server, ip, dhcpMsg, dataLenght, data);
    }
    else
    {
        CSR_LOG_TEXT_WARNING((CsrDhcpServerLto, 0, "[%s:%d] Could not allocate address", __FILE__, __LINE__));
    }

    CsrPmemFree(info.clientId);
}

static void csrDhcpServerHandleMsgDhcpRequest(CsrDhcpServerInstanceData *instanceData, CsrDhcpServerData *server, CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrIpSocketUdpDataInd *udpMsg)
{
    CsrDhcpServerClientInfo msgInfo;
    CsrBool serverIdReceived;

    csrDhcpServerGetClientInfoFromOptions(server, options, &msgInfo);

    if (csrDhcpServerMessageIsForServer(server, &msgInfo, &serverIdReceived)) /* The message is for this server, or no server id received */
    {
        CsrUint8 *data;
        CsrUint16 dataLenght;
        CsrDhcpServerDhcpMessage *respondMsg;
        CsrDhcpServerDhcpOption *respondOptions = NULL;
        CsrUint8 ip[] = {0, 0, 0, 0};
        CsrDhcpServerClient *client = csrDhcpServerGetClientUpdateInfo(server, &msgInfo, dhcpMsg);
        CsrDhcpServerClientInfo *info = client ? client->info : NULL;

        if (serverIdReceived && CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->ciaddr)) /* Client in SELECTING state */
        {
            if (info && (info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED) && CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(info->lease.ip, msgInfo.requestedIpAddress)) /* The client has been found (The address in lease.ip is the same as given in DHCP OFFER yiaddr) */
            {
                CsrUint32 sec;
                CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                if (timer) /* Cancel the timer for the old message */
                {
                    CsrDhcpServerTimerRemove(server, timer);
                }

                respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, dhcpMsg->secs, dhcpMsg->flags,
                    dhcpMsg->ciaddr, info->lease.ip,
                    ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK);
                csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                sec = csrDhcpServerAddOptionLease(&respondOptions, server, info, FALSE);
                csrDhcpServerAddOptionRenewalTimeValue(&respondOptions, server, sec);
                csrDhcpServerAddOptionRebindingTimeValue(&respondOptions, server, sec);
                csrDhcpServerHandleOptions(server, options, &respondOptions);

#ifdef CSR_DHCP_SERVER_USE_IFCONFIG_ARP
                CSR_DHCP_SERVER_COPY_IP_ADDRESS(ip, respondMsg->yiaddr);
#endif
                csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                csrDhcpServerMessageAckSend(instanceData, server, ip, dhcpMsg, dataLenght, data);

                info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE;
                if (sec != CSR_DHCP_SERVER_LEASE_INFINITE_SEC) /* Only start a timer if the lease is not infinite */
                {
                    CsrDhcpServerTimerAdd(server, client, sec, FALSE);
                }

                info->leaseSec = 0; /* Set to 0 as it is only used for the client getting same value in a DHCPOFFER and a DHCPREQUEST */
            }
            else
            {
                respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, 0, dhcpMsg->flags,
                    ip, ip, ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPNAK);
                csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                csrDhcpServerMessageNakSend(server, ip, dhcpMsg, dataLenght, data);
            }
        }
        else if ((serverIdReceived == FALSE) && CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->ciaddr) && (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(msgInfo.requestedIpAddress) == FALSE)) /* CLIENT in INIT-REBOOT state */
        {
            if (info)
            {
                CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                if (timer) /* Cancel the timer for the old message */
                {
                    CsrDhcpServerTimerRemove(server, timer);
                }

                /* Mark as inactive such that it will not be in use */
                info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE;

                if (CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(info->lease.ip, msgInfo.requestedIpAddress) && csrDhcpServerIpAddressNotUsedAndValid(server, msgInfo.requestedIpAddress))
                {
                    CsrUint32 sec;

                    respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, dhcpMsg->secs, dhcpMsg->flags,
                        dhcpMsg->ciaddr, info->lease.ip,
                        ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                    csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK);
                    csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                    sec = csrDhcpServerAddOptionLease(&respondOptions, server, info, TRUE);
                    csrDhcpServerAddOptionRenewalTimeValue(&respondOptions, server, sec);
                    csrDhcpServerAddOptionRebindingTimeValue(&respondOptions, server, sec);
                    csrDhcpServerHandleOptions(server, options, &respondOptions);

                    csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                    csrDhcpServerMessageAckSend(instanceData, server, ip, dhcpMsg, dataLenght, data);

                    info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE;

                    if (sec != CSR_DHCP_SERVER_LEASE_INFINITE_SEC) /* Only start a timer if the lease is not infinite */
                    {
                        CsrDhcpServerTimerAdd(server, client, sec, FALSE);
                    }

                    info->leaseSec = 0; /* Set to 0 as it is only used for the client getting same value in a DHCPOFFER and a DHCPREQUEST */
                }
                else
                {
                    CsrUint16 flags = dhcpMsg->flags;

                    if (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(dhcpMsg->giaddr) == FALSE) /* The relay agent must broadcast the message */
                    {
                        flags = 1;
                    }

                    respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, 0, flags,
                        ip, ip, ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                    csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPNAK);
                    csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                    csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                    csrDhcpServerMessageNakSend(server, ip, dhcpMsg, dataLenght, data);
                }
            }
            else
            {
                /* Shall remain silent */
            }
        }
        else if ((serverIdReceived == FALSE) && CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(msgInfo.requestedIpAddress)) /* CLIENT in RENEWING/REBINDING state */
        {
            if (info && CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(dhcpMsg->ciaddr, info->lease.ip)) /* Client found */
            {
                CsrUint32 sec;
                CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                if (timer) /* Cancel the timer for the old message */
                {
                    CsrDhcpServerTimerRemove(server, timer);
                }

                respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, dhcpMsg->secs, dhcpMsg->flags,
                    dhcpMsg->ciaddr, info->lease.ip,
                    ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK);
                csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                sec = csrDhcpServerAddOptionLease(&respondOptions, server, info, TRUE);
                csrDhcpServerAddOptionRenewalTimeValue(&respondOptions, server, sec);
                csrDhcpServerAddOptionRebindingTimeValue(&respondOptions, server, sec);
                csrDhcpServerHandleOptions(server, options, &respondOptions);

                csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                csrDhcpServerMessageAckSend(instanceData, server, dhcpMsg->ciaddr, dhcpMsg, dataLenght, data);

                info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE;
                if (sec != CSR_DHCP_SERVER_LEASE_INFINITE_SEC) /* Only start a timer if the lease is not infinite */
                {
                    CsrDhcpServerTimerAdd(server, client, sec, FALSE);
                }

                info->leaseSec = 0; /* Set to 0 as it is only used for the client getting same value in a DHCPOFFER and a DHCPREQUEST */
            }
            else
            {
                respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, 0, dhcpMsg->flags,
                    ip, ip, ip, dhcpMsg->giaddr, dhcpMsg->chaddr);

                csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPNAK);
                csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
                csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

                csrDhcpServerMessageNakSend(server, dhcpMsg->ciaddr, dhcpMsg, dataLenght, data);
            }
        }
    }
    else
    {
        CsrDhcpServerClient *client = csrDhcpServerFindClient(server, &msgInfo, dhcpMsg);

        if (client)
        {
            CsrDhcpServerClientInfo *info = client->info;

            if (info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_OFFERED) /* The client has rejected the offer from this server. Remove the
                 client again and cancel the timer for the offered address */
            {
                CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                if (timer) /* Cancel the timer for the old message */
                {
                    CsrDhcpServerTimerRemove(server, timer);
                }

                CSR_DHCP_SERVER_CLIENT_REMOVE(&server->clients, client);
            }
        }
    }

    CsrPmemFree(msgInfo.clientId);
}

static void csrDhcpServerHandleMsgDhcpDecline(CsrDhcpServerData *server, CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrIpSocketUdpDataInd *udpMsg)
{
    CsrDhcpServerClientInfo msgInfo;
    CsrBool serverIdReceived;

    csrDhcpServerGetClientInfoFromOptions(server, options, &msgInfo);

    if (csrDhcpServerMessageIsForServer(server, &msgInfo, &serverIdReceived)) /* The message is for this server, or no server id received */
    {
        if (serverIdReceived) /* The message is for this server. Mark the address as not available and inform the application */
        {
            CsrDhcpServerClient *client = csrDhcpServerFindClient(server, &msgInfo, dhcpMsg);

            if (client && (CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(msgInfo.requestedIpAddress) == FALSE))
            {
                CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                if (timer) /* Cancel the timer */
                {
                    CsrDhcpServerTimerRemove(server, timer);
                }

                CSR_DHCP_SERVER_CLIENT_REMOVE(&server->clients, client);

                CsrDhcpServerRogueIpAddressIndSend(server->qid, server, msgInfo.requestedIpAddress);
                CSR_DHCP_SERVER_INVALID_IP_ADD(server->invalidIpList, msgInfo.requestedIpAddress);
            }
        }
    }

    CsrPmemFree(msgInfo.clientId);
}

static void csrDhcpServerHandleMsgDhcpRelease(CsrDhcpServerData *server, CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrIpSocketUdpDataInd *udpMsg)
{
    CsrDhcpServerClientInfo msgInfo;
    CsrBool serverIdReceived;

    csrDhcpServerGetClientInfoFromOptions(server, options, &msgInfo);

    if (csrDhcpServerMessageIsForServer(server, &msgInfo, &serverIdReceived)) /* The message is for this server, or no server id received */
    {
        if (serverIdReceived) /* The message is for this server */
        {
            CsrDhcpServerClient *client = csrDhcpServerFindClient(server, &msgInfo, dhcpMsg);

            if (client)
            {
                CsrDhcpServerClientInfo *info = client->info;

                if (info->ipAddressState == CSR_DHCP_SERVER_IP_ADDRESS_STATE_ACTIVE)
                {
                    CsrDhcpServerTimer *timer = CsrDhcpServerTimerFindFromClient(server, client);

                    if (timer) /* Cancel the timer for the old message */
                    {
                        CsrDhcpServerTimerRemove(server, timer);
                    }

                    info->ipAddressState = CSR_DHCP_SERVER_IP_ADDRESS_STATE_INACTIVE;
                    info->lease.expiryTime.sec = 0;
                    info->lease.expiryTime.msec = 0;
                }
            }
        }
    }

    CsrPmemFree(msgInfo.clientId);
}

static void csrDhcpServerHandleMsgDhcpInform(CsrDhcpServerInstanceData *instanceData,
    CsrDhcpServerData *server, CsrDhcpServerDhcpMessage *dhcpMsg, CsrDhcpServerDhcpOption *options, CsrIpSocketUdpDataInd *udpMsg)
{
    CsrUint8 *data;
    CsrUint16 dataLenght;
    CsrDhcpServerDhcpMessage *respondMsg;
    CsrDhcpServerDhcpOption *respondOptions = NULL;
    CsrUint8 ip[] = {0, 0, 0, 0};

    respondMsg = csrDhcpServerAllocateDhcpMessage(dhcpMsg->hlen, dhcpMsg->htype, dhcpMsg->xid, dhcpMsg->secs, dhcpMsg->flags,
        ip, ip, ip, ip, dhcpMsg->chaddr);

    csrDhcpServerAddOptionMessageType(&respondOptions, CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK);
    csrDhcpServerAddOptionServerIdentifier(&respondOptions, server->ipAddress);
    csrDhcpServerHandleOptions(server, options, &respondOptions);
    csrDhcpServerDhcpMessage2Data(respondMsg, respondOptions, &data, &dataLenght);

    csrDhcpServerMessageAckSend(instanceData, server, dhcpMsg->ciaddr, dhcpMsg, dataLenght, data);
}

void CsrDhcpServerHandleMessage(CsrDhcpServerInstanceData *instanceData,
    CsrDhcpServerData *server, CsrIpSocketUdpDataInd *msg)
{
    CsrDhcpServerDhcpMessage *dhcpMsg;
    CsrDhcpServerDhcpOption *options;
    CsrUint8 messageType;

    if (csrDhcpServerGetOptions(server, msg->data, msg->dataLength, &messageType, &options) == FALSE)
    {
        return;
    }

    dhcpMsg = (CsrDhcpServerDhcpMessage *) msg->data;

    switch (messageType)
    {
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPDISCOVER:
        {
            csrDhcpServerHandleMsgDhcpDiscover(instanceData, server, dhcpMsg, options, msg);
            break;
        }
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPREQUEST:
        {
            csrDhcpServerHandleMsgDhcpRequest(instanceData, server, dhcpMsg, options, msg);
            break;
        }
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPDECLINE:
        {
            csrDhcpServerHandleMsgDhcpDecline(server, dhcpMsg, options, msg);
            break;
        }
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPRELEASE:
        {
            csrDhcpServerHandleMsgDhcpRelease(server, dhcpMsg, options, msg);
            break;
        }
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPINFORM:
        {
            csrDhcpServerHandleMsgDhcpInform(instanceData, server, dhcpMsg, options, msg);
            break;
        }
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_INVALID:
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPOFFER:
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPNAK:
        case CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK:
        default:
        {
            CSR_LOG_TEXT_INFO((CsrDhcpServerLto, 0, "[%s:%d] Invalid message type 0x%X", __FILE__, __LINE__, messageType));
            break;
        }
    }

    /* Free allocated options data */
    csrDhcpServerFreeOptions(options);
}
