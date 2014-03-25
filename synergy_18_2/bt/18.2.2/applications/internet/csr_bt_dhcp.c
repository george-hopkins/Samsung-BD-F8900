/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_bt_internet_primitives.h"
#include "csr_bt_udp_prim.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_dhcp_prim.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#define INITIAL_CONFIGURATION_TIMEOUT                4000000
#define MAX_NUMBER_OF_CONFIGURATION_RETRANSMISSIONS        3    /* 4 + 8 + 16 + 32 = 60 secs. */
#define MAX_DHCP_MESSAGE_SIZE                           1500

#define DHCP_SERVER_PORT        67
#define DHCP_CLIENT_PORT        68

#define DHCP_STATE_DEACTIVATED     0
#define DHCP_STATE_INIT            1
#define DHCP_STATE_SELECTING       2
#define DHCP_STATE_REQUESTING      3
#define DHCP_STATE_BOUND           4
#define DHCP_STATE_RENEWING        5
#define DHCP_STATE_REBINDING       6
#define DHCP_STATE_INIT_REBOOT     7
#define DHCP_STATE_REBOOTING       8

#define END_OPTION                                0xFF
#define DHCP_MESSAGE_TYPE_OPTION                    53
#define DHCP_MESSAGE_TYPE_OPTION_LENGTH              1
#define MAX_DHCP_MESSAGE_SIZE_OPTION                57
#define MAX_DHCP_MESSAGE_SIZE_OPTION_LENGTH          2
#define REQUESTED_IP_ADDRESS_OPTION                 50
#define REQUESTED_IP_ADDRESS_OPTION_LENGTH           4
#define SERVER_IDENTIFIER_OPTION                    54
#define SERVER_IDENTIFIER_OPTION_LENGTH              4
#define IP_ADDRESS_LEASE_TIME_OPTION                51
#define IP_ADDRESS_LEASE_TIME_OPTION_LENGTH          4
#define PARAMETER_REQUEST_LIST_OPTION               55
#define PARAMETER_REQUEST_LIST_OPTION_LENGTH         2
#define SUBNET_MASK_OPTION                           1
#define SUBNET_MASK_OPTION_LENGTH                    4
#define ROUTER_OPTION                                3
#define RENEWAL_TIME_VALUE_OPTION                   58
#define RENEWAL_TIME_VALUE_OPTION_LENGTH             4
#define REBINDING_TIME_VALUE_OPTION                 59
#define REBINDING_TIME_VALUE_OPTION_LENGTH           4
#define HOST_NAME_OPTION                            12
#define HOST_NAME_OPTION_LENGTH                      4


#define DHCP_MESSAGE_TYPE_DHCPDISCOVER     1
#define DHCP_MESSAGE_TYPE_DHCPOFFER        2
#define DHCP_MESSAGE_TYPE_DHCPREQUEST      3
#define DHCP_MESSAGE_TYPE_DHCPDECLINE      4
#define DHCP_MESSAGE_TYPE_DHCPACK          5
#define DHCP_MESSAGE_TYPE_DHCPNAK          6
#define DHCP_MESSAGE_TYPE_DHCPRELEASE      7

#define OPTIONS_OFFSET 4 + 22 + 32 + 64

#define LAN_WORD_OPTION(word)    ((CsrUint8) ((word) >> 8)), ((CsrUint8)((word) & 0x00FF))

static CsrUint8 discoverOptions[] =
{
    99, 130, 83, 99,    /* DHCP magic cookie */
    DHCP_MESSAGE_TYPE_OPTION, DHCP_MESSAGE_TYPE_OPTION_LENGTH, DHCP_MESSAGE_TYPE_DHCPDISCOVER,
    MAX_DHCP_MESSAGE_SIZE_OPTION, MAX_DHCP_MESSAGE_SIZE_OPTION_LENGTH, LAN_WORD_OPTION(MAX_DHCP_MESSAGE_SIZE),
    PARAMETER_REQUEST_LIST_OPTION, PARAMETER_REQUEST_LIST_OPTION_LENGTH, SUBNET_MASK_OPTION, ROUTER_OPTION,
    END_OPTION
};

#define REQUESTED_IP_ADDRESS_OPTION_ADDRESS_INDEX     13
#define SERVER_IDENTIFIER_OPTION_ADDRESS_INDEX        19
static CsrUint8 selectingRequestOptions[] =
{
    99, 130, 83, 99,    /* DHCP magic cookie */
    DHCP_MESSAGE_TYPE_OPTION, DHCP_MESSAGE_TYPE_OPTION_LENGTH, DHCP_MESSAGE_TYPE_DHCPREQUEST,
    MAX_DHCP_MESSAGE_SIZE_OPTION, MAX_DHCP_MESSAGE_SIZE_OPTION_LENGTH, LAN_WORD_OPTION(MAX_DHCP_MESSAGE_SIZE),
    REQUESTED_IP_ADDRESS_OPTION, REQUESTED_IP_ADDRESS_OPTION_LENGTH, 0x00, 0x00, 0x00, 0x00, /* address starts at index 13 */
    SERVER_IDENTIFIER_OPTION, SERVER_IDENTIFIER_OPTION_LENGTH, 0x00, 0x00, 0x00, 0x00, /* address starts at index 19 */
    PARAMETER_REQUEST_LIST_OPTION, PARAMETER_REQUEST_LIST_OPTION_LENGTH, SUBNET_MASK_OPTION, ROUTER_OPTION,
    END_OPTION
};

static CsrUint8 renewingRequestOptions[] =
{
    99, 130, 83, 99,    /* DHCP magic cookie */
    DHCP_MESSAGE_TYPE_OPTION, DHCP_MESSAGE_TYPE_OPTION_LENGTH, DHCP_MESSAGE_TYPE_DHCPREQUEST,
    MAX_DHCP_MESSAGE_SIZE_OPTION, MAX_DHCP_MESSAGE_SIZE_OPTION_LENGTH, LAN_WORD_OPTION(MAX_DHCP_MESSAGE_SIZE),
    PARAMETER_REQUEST_LIST_OPTION, PARAMETER_REQUEST_LIST_OPTION_LENGTH, SUBNET_MASK_OPTION, ROUTER_OPTION,
    END_OPTION
};

typedef struct
{
    CsrUint16 state;
    CsrUint16 retransmissionCount;
    CsrTime currentTimeoutValue;
    CsrSchedTid dhcpTimer;
    CsrUint32 remainingLeaseTime;
    CsrSchedQid applicationQueue;
    CsrUint32 currentXid;
    CsrUint16 ethernetAddress[3];
    CsrUint16 ipAddress[2];
    CsrUint16 serverIpAddress[2];
    CsrUint16 receivedEvent;
    void * receivedMessage;
} DhcpInstanceData;


static CsrUint8 * findOption(CsrUint8 optionCode, CsrUint8 * options)
{
    while ((*options != END_OPTION) && (*options != optionCode))
    {
        if (*options)
        {
            options += (2 + options[1]);
        }
        else
        {
            options++;
        }
    }
    if (*options == optionCode)
    {
        return options;
    }
    return NULL;
}

void sendDhcpBootRequest(CsrUint32 xid, CsrUint16 * serverIpAddress, CsrUint16 * ciaddr, CsrUint16 * chaddr, CsrUint16 optionsLength, CsrUint8 * options)
{
    CsrBtUdpDataReq * udpDataReq;
    CsrUint8 * octetPtr;
    CsrUint16 index;

    udpDataReq = (CsrBtUdpDataReq *) CsrPmemAlloc(sizeof(CsrBtUdpDataReq));
    udpDataReq->type = CSR_BT_UDP_DATA_REQ;
    udpDataReq->typeOfService= 0;
    udpDataReq->timeToLive = 128;
    udpDataReq->destinationAddress[0] = serverIpAddress[0];
    udpDataReq->destinationAddress[1] = serverIpAddress[1];
    udpDataReq->udpDataLength = 8 + 236 + optionsLength;
    udpDataReq->udpData = (CsrUint16 *) CsrPmemZalloc(udpDataReq->udpDataLength + (udpDataReq->udpDataLength & 0x1));
    udpDataReq->udpData[0] = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
    udpDataReq->udpData[1] = CSR_BT_LAN_WORD(DHCP_SERVER_PORT);
    udpDataReq->udpData[4] = CSR_BT_LAN_WORD(0x0101);    /* op=BOOTREQUEST, htype=ethernet */
    udpDataReq->udpData[5] = CSR_BT_LAN_WORD(0x0600); /* hlen=6, hops=0 */
    udpDataReq->udpData[6] = CSR_BT_LAN_WORD((CsrUint16) (xid >> 16));
    udpDataReq->udpData[7] = CSR_BT_LAN_WORD((CsrUint16) (xid & 0x0000FFFF));
    udpDataReq->udpData[8] = 0x0000;    /* secs=0 */
    udpDataReq->udpData[9] = 0x0000;    /* flags=0 */
    udpDataReq->udpData[10] = ciaddr[0];
    udpDataReq->udpData[11] = ciaddr[1];
    udpDataReq->udpData[12] = 0x0000;    /* yiaddr=0 */
    udpDataReq->udpData[13] = 0x0000;    /* yiaddr=0 */
    udpDataReq->udpData[14] = 0x0000;    /* siaddr=0 */
    udpDataReq->udpData[15] = 0x0000;    /* siaddr=0 */
    udpDataReq->udpData[16] = 0x0000;    /* giaddr=0 */
    udpDataReq->udpData[17] = 0x0000;    /* giaddr=0 */
    udpDataReq->udpData[18] = chaddr[0];
    udpDataReq->udpData[19] = chaddr[1];
    udpDataReq->udpData[20] = chaddr[2];
    udpDataReq->udpData[4 + 22] = 0x0000;    /* sname[0]&[1]='\0' */
    udpDataReq->udpData[4 + 22 + 32] = 0x0000;    /* file[0]&[1]='\0' */
    octetPtr = (CsrUint8 *) &udpDataReq->udpData[OPTIONS_OFFSET];
    for (index = 0; index < optionsLength; index++)
    {
        octetPtr[index] = options[index];
    }
    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, udpDataReq);
}

static void configurationTimeout(CsrUint16 mi, void * mv)
{
    DhcpInstanceData * instanceData;

    instanceData = (DhcpInstanceData *) mv;
    if (instanceData->retransmissionCount < MAX_NUMBER_OF_CONFIGURATION_RETRANSMISSIONS)
    {
        switch (instanceData->state)
        {
            case DHCP_STATE_SELECTING:
                sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(discoverOptions), discoverOptions);
                break;

            case DHCP_STATE_REQUESTING:
                sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(selectingRequestOptions), selectingRequestOptions);
                break;

            case DHCP_STATE_RENEWING:
            case DHCP_STATE_REBINDING:
                sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(renewingRequestOptions), renewingRequestOptions);
                break;

            default:
                {
                    CsrBtDhcpActivateClientCfm * activateCfm;
                    CsrBtUdpDeregisterPortReq * deregisterPortReq;

                    deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
                    deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
                    deregisterPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
                    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);

                    activateCfm = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                    activateCfm->type = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                    activateCfm->result = CSR_BT_DHCP_RESULT_FAILED;
                    CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);

                    instanceData->dhcpTimer = 0;
                    instanceData->state = DHCP_STATE_DEACTIVATED;

                    return;
                }
        }
        instanceData->currentTimeoutValue <<= 1;
        instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);
        instanceData->retransmissionCount++;
    }
    else
    {
        if (instanceData->state == DHCP_STATE_RENEWING)
        {
            instanceData->serverIpAddress[0] = 0xFFFF;
            instanceData->serverIpAddress[1] = 0xFFFF;

            instanceData->currentXid = (CsrUint32) CsrTimeGet(NULL);
            sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(renewingRequestOptions), renewingRequestOptions);

            instanceData->retransmissionCount = 0;
            instanceData->currentTimeoutValue = INITIAL_CONFIGURATION_TIMEOUT;
            instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);

            instanceData->state = DHCP_STATE_REBINDING;
        }
        else if (instanceData->state == DHCP_STATE_REBINDING)
        {
            CsrBtDhcpDeactivateClientInd * deactivateInd;
            CsrBtUdpDeregisterPortReq * deregisterPortReq;

            deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
            deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
            deregisterPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
            CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);

            deactivateInd = (CsrBtDhcpDeactivateClientInd *) CsrPmemAlloc(sizeof(CsrBtDhcpDeactivateClientInd));
            deactivateInd->type = CSR_BT_DHCP_DEACTIVATE_CLIENT_IND;
            CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, deactivateInd);

            instanceData->dhcpTimer = 0;
            instanceData->state = DHCP_STATE_DEACTIVATED;
        }
        else
        {
            CsrBtDhcpActivateClientCfm * activateCfm;
            CsrBtUdpDeregisterPortReq * deregisterPortReq;

            deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
            deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
            deregisterPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
            CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);

            activateCfm = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
            activateCfm->type = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
            activateCfm->result = CSR_BT_DHCP_RESULT_FAILED;
            CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);

            instanceData->dhcpTimer = 0;
            instanceData->state = DHCP_STATE_DEACTIVATED;
        }
    }
}

static void leaseTimeout(CsrUint16 mi, void * mv)
{
    DhcpInstanceData * instanceData;
    CsrTime nextLeaseTimerTimeout;

    instanceData = (DhcpInstanceData *) mv;

    if (instanceData->remainingLeaseTime > 1000)
    {
        nextLeaseTimerTimeout = 1000000000;
    }
    else if (instanceData->remainingLeaseTime > 100)
    {
        nextLeaseTimerTimeout = 100000000;
    }
    else if (instanceData->remainingLeaseTime > 10)
    {
        nextLeaseTimerTimeout = 10000000;
    }
    else
    {
        if (instanceData->remainingLeaseTime > 0)
        {
            nextLeaseTimerTimeout = 1000000;
        }
        else
        {
            instanceData->dhcpTimer = 0;
            instanceData->currentXid = (CsrUint32) CsrTimeGet(NULL);
            sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(renewingRequestOptions), renewingRequestOptions);

            instanceData->retransmissionCount = 0;
            instanceData->currentTimeoutValue = INITIAL_CONFIGURATION_TIMEOUT;
            instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);

            instanceData->state = DHCP_STATE_RENEWING;

            return;
        }
    }
    instanceData->remainingLeaseTime -= (nextLeaseTimerTimeout / 1000000);
    instanceData->dhcpTimer = CsrSchedTimerSet(nextLeaseTimerTimeout, leaseTimeout, 0, (void *) instanceData);
}


void CsrBtDhcpInit(void ** gash)
{
    DhcpInstanceData * instanceData;

    instanceData = (DhcpInstanceData *) CsrPmemAlloc(sizeof(DhcpInstanceData));
    *gash = (void *) instanceData;

    instanceData->state = DHCP_STATE_DEACTIVATED;
    instanceData->ipAddress[0] = 0x0000;
    instanceData->ipAddress[1] = 0x0000;
    instanceData->serverIpAddress[0] = 0xFFFF;
    instanceData->serverIpAddress[1] = 0xFFFF;
    instanceData->dhcpTimer = 0;
    instanceData->remainingLeaseTime = 0;
}

void CsrBtDhcpHandler(void ** gash)
{
    DhcpInstanceData * instanceData;

    instanceData = (DhcpInstanceData *) *gash;

    if (CsrSchedMessageGet(&instanceData->receivedEvent, &instanceData->receivedMessage))
    {
        switch (instanceData->receivedEvent)
        {
            case CSR_BT_UDP_PRIM:
                switch (*((CsrUint16 *) instanceData->receivedMessage))
                {
                    case CSR_BT_UDP_DATA_IND:
                        {
                            CsrBtUdpDataInd * udpDataInd;

                            udpDataInd = (CsrBtUdpDataInd *) instanceData->receivedMessage;
                            if ((CSR_BT_LAN_WORD(udpDataInd->udpData[4]) & 0xFF00) == 0x0200) /* BOOTREPLY */
                            {
                                if ((instanceData->currentXid == ((((CsrUint32) CSR_BT_LAN_WORD(udpDataInd->udpData[6])) << 16) | ((CsrUint32) CSR_BT_LAN_WORD(udpDataInd->udpData[7]))))
                                    && ((instanceData->ethernetAddress[0] == udpDataInd->udpData[18]) && (instanceData->ethernetAddress[1] == udpDataInd->udpData[19]) && (instanceData->ethernetAddress[2] == udpDataInd->udpData[20])))
                                {
                                    CsrUint8 * magicCookiePtr;

                                    magicCookiePtr = (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET];
                                    if ((magicCookiePtr[0] == 99) && (magicCookiePtr[1] == 130) && (magicCookiePtr[2] == 83) && (magicCookiePtr[3] == 99))
                                    {
                                        CsrUint8 * dhcpMessageTypeOptionPtr;

                                        dhcpMessageTypeOptionPtr = findOption(DHCP_MESSAGE_TYPE_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                        if (dhcpMessageTypeOptionPtr != NULL)
                                        {
                                            switch (dhcpMessageTypeOptionPtr[2])
                                            {
                                                case DHCP_MESSAGE_TYPE_DHCPOFFER:
                                                    if (instanceData->state == DHCP_STATE_SELECTING)
                                                    {
                                                        CsrUint8 * serverIdentityOptionPtr;

                                                        serverIdentityOptionPtr = findOption(SERVER_IDENTIFIER_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                                        if (serverIdentityOptionPtr != NULL)
                                                        {
                                                            CsrUint16 index;
                                                            CsrUint8 * yiaddr;

                                                            yiaddr = (CsrUint8 *) &udpDataInd->udpData[12];

                                                            for (index = 0; index < 4; index++)
                                                            {
                                                                selectingRequestOptions[REQUESTED_IP_ADDRESS_OPTION_ADDRESS_INDEX + index] = yiaddr[index];
                                                                selectingRequestOptions[SERVER_IDENTIFIER_OPTION_ADDRESS_INDEX + index] = serverIdentityOptionPtr[2 + index];
                                                            }

                                                            sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(selectingRequestOptions), selectingRequestOptions);
                                                            if (instanceData->dhcpTimer != 0)
                                                            {
                                                                CsrSchedTimerCancel(instanceData->dhcpTimer, NULL, NULL);
                                                                instanceData->dhcpTimer = 0;
                                                            }

                                                            instanceData->retransmissionCount = 0;
                                                            instanceData->currentTimeoutValue = INITIAL_CONFIGURATION_TIMEOUT;
                                                            instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);
                                                            instanceData->state = DHCP_STATE_REQUESTING;
                                                        }
                                                    }
                                                    break;

                                                case DHCP_MESSAGE_TYPE_DHCPACK:
                                                    if (instanceData->state != DHCP_STATE_BOUND)
                                                    {
                                                        CsrUint8 * serverIdentityOptionPtr;
                                                        CsrUint8 * ipAddressLeaseTimeOptionPtr;

                                                        serverIdentityOptionPtr = findOption(SERVER_IDENTIFIER_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                                        ipAddressLeaseTimeOptionPtr = findOption(IP_ADDRESS_LEASE_TIME_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                                        if ((serverIdentityOptionPtr != NULL) && (ipAddressLeaseTimeOptionPtr != NULL))
                                                        {
                                                            CsrBtIpAddressReq * ipAddressReq;
                                                            CsrUint8 * optionPtr;
                                                            CsrTime nextLeaseTimerTimeout;

                                                            ipAddressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
                                                            ipAddressReq->type = CSR_BT_IP_ADDRESS_REQ;
                                                            ipAddressReq->ipAddress[0] = udpDataInd->udpData[12];
                                                            ipAddressReq->ipAddress[1] = udpDataInd->udpData[13];
                                                            instanceData->ipAddress[0] = udpDataInd->udpData[12];
                                                            instanceData->ipAddress[1] = udpDataInd->udpData[13];

                                                            optionPtr = findOption(SUBNET_MASK_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                                            if (optionPtr != NULL)
                                                            {
                                                                ipAddressReq->subnetMask[0] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 0]) << 8) | ((CsrUint16) optionPtr[2 + 1]));
                                                                ipAddressReq->subnetMask[1] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 2]) << 8) | ((CsrUint16) optionPtr[2 + 3]));
                                                            }
                                                            else    /* default to class C */
                                                            {
                                                                ipAddressReq->subnetMask[0] = 0xFFFF;
                                                                ipAddressReq->subnetMask[1] = 0x00FF;
                                                            }
                                                            optionPtr = findOption(ROUTER_OPTION, (CsrUint8 *) &udpDataInd->udpData[OPTIONS_OFFSET + 2]);
                                                            if (optionPtr != NULL)
                                                            {
                                                                ipAddressReq->gatewayIpAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 0]) << 8) | ((CsrUint16) optionPtr[2 + 1]));
                                                                ipAddressReq->gatewayIpAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 2]) << 8) | ((CsrUint16) optionPtr[2 + 3]));
                                                            }
                                                            else    /* default to zero */
                                                            {
                                                                ipAddressReq->gatewayIpAddress[0] = 0;
                                                                ipAddressReq->gatewayIpAddress[1] = 0;
                                                            }
                                                            CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipAddressReq);

                                                            if (instanceData->state == DHCP_STATE_REQUESTING)
                                                            {
                                                                CsrBtDhcpActivateClientCfm * activateCfm;

                                                                activateCfm = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                                                                activateCfm->type = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                                                                activateCfm->result = CSR_BT_DHCP_RESULT_CONFIGURED;
                                                                activateCfm->ipAddress[0] = instanceData->ipAddress[0];
                                                                activateCfm->ipAddress[1] = instanceData->ipAddress[1];
                                                                CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);
                                                            }

                                                            instanceData->remainingLeaseTime = (((CsrUint32) ipAddressLeaseTimeOptionPtr[2 + 0]) << 24) | (((CsrUint32) ipAddressLeaseTimeOptionPtr[2 + 1]) << 16) | (((CsrUint32) ipAddressLeaseTimeOptionPtr[2 + 2]) << 8) | ((CsrUint32) ipAddressLeaseTimeOptionPtr[2 + 3]);
                                                            if (instanceData->remainingLeaseTime > 120)    /* start renewal/rebinding before time expires */
                                                            {
                                                                instanceData->remainingLeaseTime -= 120;
                                                            }

                                                            instanceData->serverIpAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 0]) << 8) | ((CsrUint16) optionPtr[2 + 1]));    /* needed for release */
                                                            instanceData->serverIpAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) optionPtr[2 + 2]) << 8) | ((CsrUint16) optionPtr[2 + 3]));

                                                            if (instanceData->dhcpTimer != 0)
                                                            {
                                                                CsrSchedTimerCancel(instanceData->dhcpTimer, NULL, NULL);
                                                                instanceData->dhcpTimer = 0;
                                                            }

                                                            if (instanceData->remainingLeaseTime != 0xFFFFFFFF)    /* only if not infinite */
                                                            {
                                                                if (instanceData->remainingLeaseTime > 1000)
                                                                {
                                                                    nextLeaseTimerTimeout = 1000000000;
                                                                }
                                                                else if (instanceData->remainingLeaseTime > 100)
                                                                {
                                                                    nextLeaseTimerTimeout = 100000000;
                                                                }
                                                                else if (instanceData->remainingLeaseTime > 10)
                                                                {
                                                                    nextLeaseTimerTimeout = 10000000;
                                                                }
                                                                else
                                                                {
                                                                    nextLeaseTimerTimeout = 1000000;
                                                                }
                                                                instanceData->remainingLeaseTime -= nextLeaseTimerTimeout;
                                                                instanceData->dhcpTimer = CsrSchedTimerSet(nextLeaseTimerTimeout, leaseTimeout, 0, (void *) instanceData);
                                                            }

                                                            instanceData->state = DHCP_STATE_BOUND;
                                                        }
                                                    }
                                                    break;

                                                case DHCP_MESSAGE_TYPE_DHCPNAK:
                                                    if (instanceData->state == DHCP_STATE_REQUESTING)
                                                    {
                                                        CsrBtUdpDeregisterPortReq * deregisterPortReq;
                                                        CsrBtDhcpActivateClientCfm * activateCfm;

                                                        deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
                                                        deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
                                                        deregisterPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
                                                        CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);

                                                        activateCfm = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                                                        activateCfm->type = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                                                        activateCfm->result = CSR_BT_DHCP_RESULT_FAILED;
                                                        CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);

                                                        if (instanceData->dhcpTimer != 0)
                                                        {
                                                            CsrSchedTimerCancel(instanceData->dhcpTimer, NULL, NULL);
                                                            instanceData->dhcpTimer = 0;
                                                        }
                                                        instanceData->state = DHCP_STATE_DEACTIVATED;
                                                    }
                                                    else if ((instanceData->state == DHCP_STATE_RENEWING) || (instanceData->state == DHCP_STATE_REBINDING))
                                                    {
                                                        CsrBtUdpDeregisterPortReq * deregisterPortReq;
                                                        CsrBtDhcpDeactivateClientInd * deactivateInd;

                                                        deregisterPortReq = (CsrBtUdpDeregisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpDeregisterPortReq));
                                                        deregisterPortReq->type = CSR_BT_UDP_DEREGISTER_PORT_REQ;
                                                        deregisterPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
                                                        CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, deregisterPortReq);

                                                        deactivateInd = (CsrBtDhcpDeactivateClientInd *) CsrPmemAlloc(sizeof(CsrBtDhcpDeactivateClientInd));
                                                        deactivateInd->type = CSR_BT_DHCP_DEACTIVATE_CLIENT_IND;
                                                        CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, deactivateInd);

                                                        if (instanceData->dhcpTimer != 0)
                                                        {
                                                            CsrSchedTimerCancel(instanceData->dhcpTimer, NULL, NULL);
                                                            instanceData->dhcpTimer = 0;
                                                        }
                                                        instanceData->state = DHCP_STATE_DEACTIVATED;
                                                    }
                                                    break;

                                                default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                                                    CsrStateEventException("DHCP",
                                                                            dhcpMessageTypeOptionPtr[2],
                                                                            0,
                                                                            0);
#endif
                                                    break;
                                            }
                                        }
                                    }
                                }
                            }
                            CsrPmemFree(udpDataInd->udpData);
                            break;
                        }

                    case CSR_BT_UDP_REGISTER_PORT_CFM:
                        {
                            CsrBtUdpRegisterPortCfm * registerPortCfm;

                            registerPortCfm = (CsrBtUdpRegisterPortCfm *) instanceData->receivedMessage;
                            if (registerPortCfm->result == CSR_BT_UDP_SUCCESS)
                            {
                                sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(discoverOptions), discoverOptions);
                                instanceData->retransmissionCount = 0;
                                instanceData->currentTimeoutValue = INITIAL_CONFIGURATION_TIMEOUT;
                                instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);

                                instanceData->state = DHCP_STATE_SELECTING;
                            }
                            else
                            {
                                CsrBtDhcpActivateClientCfm * activateCfm;

                                activateCfm = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                                activateCfm->type = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                                activateCfm->result = CSR_BT_DHCP_RESULT_PORT_UNAVAILABLE;
                                CsrSchedMessagePut(instanceData->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);

                                instanceData->state = DHCP_STATE_DEACTIVATED;
                            }
                            break;
                        }

                    case CSR_BT_UDP_SHUTDOWN_IND:
                        {
                            instanceData->state = DHCP_STATE_DEACTIVATED;
                            instanceData->ipAddress[0] = 0x0000;
                            instanceData->ipAddress[1] = 0x0000;
                            instanceData->serverIpAddress[0] = 0xFFFF;
                            instanceData->serverIpAddress[1] = 0xFFFF;
                            if (instanceData->dhcpTimer != 0)
                            {
                                CsrSchedTimerCancel(instanceData->dhcpTimer, NULL, NULL);
                                instanceData->dhcpTimer = 0;
                            }
                            instanceData->remainingLeaseTime = 0;
                            break;
                        }

                    default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("DHCP",
                                                instanceData->receivedEvent,
                                                *(CsrUint16*)instanceData->receivedMessage,
                                                0);
#endif
                        break;
                }
                break;



            case CSR_BT_DHCP_PRIM:
                switch (*((CsrUint16 *) instanceData->receivedMessage))
                {

                    case CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ:
                        {

                            CsrSchedTimerCancel(instanceData->dhcpTimer,NULL,NULL);
                            instanceData->state = DHCP_STATE_INIT; /*UDP still init */
                            break;
                        }

                    case CSR_BT_DHCP_ACTIVATE_CLIENT_REQ:
                        {
                            CsrBtDhcpActivateClientReq * activateReq;

                            activateReq = (CsrBtDhcpActivateClientReq *) instanceData->receivedMessage;


                            if (instanceData->state == DHCP_STATE_DEACTIVATED)
                            {
                                CsrBtIpMacAddressReq * ipMacAddressReq;
                                CsrBtUdpRegisterPortReq * registerPortReq;

                                instanceData->currentXid = (CsrUint32) CsrTimeGet(NULL);

                                instanceData->applicationQueue = activateReq->applicationQueue;
                                instanceData->ethernetAddress[0] = activateReq->ethernetAddress[0];
                                instanceData->ethernetAddress[1] = activateReq->ethernetAddress[1];
                                instanceData->ethernetAddress[2] = activateReq->ethernetAddress[2];

                                ipMacAddressReq = (CsrBtIpMacAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpMacAddressReq));
                                ipMacAddressReq->type = CSR_BT_IP_MAC_ADDRESS_REQ;
                                ipMacAddressReq->macAddress[0] = activateReq->ethernetAddress[0];
                                ipMacAddressReq->macAddress[1] = activateReq->ethernetAddress[1];
                                ipMacAddressReq->macAddress[2] = activateReq->ethernetAddress[2];
                                CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, ipMacAddressReq);

                                registerPortReq = (CsrBtUdpRegisterPortReq *) CsrPmemAlloc(sizeof(CsrBtUdpRegisterPortReq));
                                registerPortReq->type = CSR_BT_UDP_REGISTER_PORT_REQ;
                                registerPortReq->destinationQueue = CSR_BT_DHCP_IFACEQUEUE;
                                registerPortReq->port = CSR_BT_LAN_WORD(DHCP_CLIENT_PORT);
                                CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_UDP_PRIM, registerPortReq);

                                instanceData->state = DHCP_STATE_INIT;
                            }
                            else if(instanceData->state == DHCP_STATE_BOUND)
                            {
                                CsrBtDhcpActivateClientCfm * activateCfm;
                                activateCfm               = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                                activateCfm->ipAddress[0] = instanceData->ipAddress[0];
                                activateCfm->ipAddress[1] = instanceData->ipAddress[1];
                                activateCfm->type         = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                                activateCfm->result       = CSR_BT_DHCP_RESULT_ALREADY_ACTIVE;
                                CsrSchedMessagePut(activateReq->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);
                            }
                            else if(instanceData->state == DHCP_STATE_INIT)
                            {
                                sendDhcpBootRequest(instanceData->currentXid, instanceData->serverIpAddress, instanceData->ipAddress, instanceData->ethernetAddress, sizeof(discoverOptions), discoverOptions);
                                instanceData->retransmissionCount = 0;
                                instanceData->currentTimeoutValue = INITIAL_CONFIGURATION_TIMEOUT;
                                instanceData->dhcpTimer = CsrSchedTimerSet(instanceData->currentTimeoutValue, configurationTimeout, 0, (void *) instanceData);
                                instanceData->state = DHCP_STATE_SELECTING;
                            }
                            else
                            {
                                CsrBtDhcpActivateClientCfm * activateCfm;
                                activateCfm               = (CsrBtDhcpActivateClientCfm *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientCfm));
                                activateCfm->ipAddress[0] = 0x0000;
                                activateCfm->ipAddress[1] = 0x0000;
                                activateCfm->type         = CSR_BT_DHCP_ACTIVATE_CLIENT_CFM;
                                activateCfm->result       = CSR_BT_DHCP_RESULT_FAILED;
                                CsrSchedMessagePut(activateReq->applicationQueue, CSR_BT_DHCP_PRIM, activateCfm);
                            }
                            break;
                        }

                    default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrStateEventException("DHCP",
                                               instanceData->receivedEvent,
                                               *(CsrUint16*)instanceData->receivedMessage,
                                               0);
#endif
                        break;
                }
                break;

            default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("DHCP",
                                        instanceData->receivedEvent,
                                        0,
                                        0);
#endif
                break;
        }

        if (instanceData->receivedMessage != NULL)
        {
            CsrPmemFree(instanceData->receivedMessage);
            instanceData->receivedMessage = NULL;
        }
    }
}



#ifdef ENABLE_SHUTDOWN
void CsrBtDhcpDeinit(void ** gash)
{
    DhcpInstanceData * instanceData;
    instanceData = (DhcpInstanceData *) (* gash);

    CsrSchedTimerCancel(instanceData->dhcpTimer,NULL,NULL);

    CsrPmemFree(instanceData); /* instanceData->receivedMessage is free'ed in CsrBtDhcpHandler */
}
#endif
