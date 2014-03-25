/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_ethernet_prim.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_internet_checksum.h"
#include "csr_bt_internet_primitives.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "csr_sched.h"

#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
#include "csr_exceptionhandler.h"
#endif

#ifndef EXCLUDE_ARP_MODULE
#define ARP_CACHE_SIZE               10
#define ARP_LONGEVITY                600
#define ARP_RETRANSMISSION_TIME      1000000
#define ARP_MAX_RETRANSMISSIONS      10
#endif

#define MAX_IP_DATA_LENGTH_IN_WORDS    4096

#define HOLE_FIRST_INDEX                0
#define HOLE_LAST_INDEX                 1
#define NEXT_HOLE_DESCRIPTOR_INDEX      2
#define PREVIOUS_HOLE_DESCRIPTOR_INDEX  3

#define GET_IP_PROTOCOL(IpDataInd)    (CSR_BT_LAN_WORD(IpDataInd->ipHeader[4]) & 0x00FF)


typedef struct IpReassemblyContainerTag
{
    CsrUint16 destinationIpAddress[2];    /* network order */
    CsrUint16 sourceIpAddress[2];        /* network order */
    CsrUint16 identification;            /* network order */
    CsrUint16 protocol;                    /* network order */
    CsrBtIpDataInd * ipDataInd;                /* if NULL, then packet is being discarded (probably due to packet length > MAX_IP_DATA_LENGTH_IN_WORDS) */
    CsrUint16 HoleFirstDescriptorIndex;
    CsrUint16 secondsUntilDiscard;        /* TTL counted down - discard when zero */
    struct IpReassemblyContainerTag * next;
    struct IpReassemblyContainerTag * previous;
} IpReassemblyContainer;

typedef struct PacketQueueTag
{
    ETHERNET_PACKET * requestPacket;
    struct PacketQueueTag * next;
    struct PacketQueueTag * previous;
} PacketQueue;

#ifndef EXCLUDE_ARP_MODULE
typedef struct
{
    CsrUint16 ethernetAddress[3];    /* 0 means pending arp reply (network order) */
    CsrUint16 ipAddress[2];            /* network order */
    CsrUint16 secondsUntilDirty;        /* 0 means dirty - ie unavailable/free (MUST be renewed)! */
    PacketQueue * queue;
    CsrUint16 arpRetransmissionCount;
    CsrSchedTid arpRetransmissionTimerId;
} ArpCacheEntry;
#endif

typedef struct
{
    CsrUint16 event;
    void * message;
#ifndef EXCLUDE_ARP_MODULE
    ArpCacheEntry arpCache[ARP_CACHE_SIZE];
    CsrSchedTid arpTimerId;
#endif
    CsrUint16 ethernetAddress[3];    /* network order */
    CsrUint16 subnetMask[2];            /* network order */
    CsrUint16 ipAddress[2];            /* network order */
    CsrUint16 gatewayIpAddress[2];    /* network order */
    IpReassemblyContainer    * ipReassemblyLine;
    CsrSchedTid ttlTimerId;
    CsrBool ttlTimerRunning;
    CsrUint16 currentIdentificationNumber;
} IpTaskInstanceData;


#ifndef EXCLUDE_ARP_MODULE
/* -------------- ARP -------------- */

static void SendArpRequest(CsrUint16 * sourceEthernetAddress, CsrUint16 * sourceIpAddress, CsrUint16 * destinationIpAddress)
{
    ETHERNET_PACKET * arpRequest;

    ALLOCATE_ETHERNET_PACKET_REQUEST(arpRequest, 28);

    if (arpRequest != NULL)
    {
        ETHERNET_SET_LENGTH(arpRequest, 28);
        ETHERNET_SET_DESTINATION(arpRequest, 0xFFFF, 0xFFFF, 0xFFFF);
        ETHERNET_SET_SOURCE(arpRequest, sourceEthernetAddress[0], sourceEthernetAddress[1], sourceEthernetAddress[2]);
        ETHERNET_SET_TYPE(arpRequest, 0x0806);    /* ARP */
        ETHERNET_DATA(arpRequest)[0] = 0x0100;    /* CSR_BT_LAN_WORD(0x0001);    Ethernet */
        ETHERNET_DATA(arpRequest)[1] = 0x0008;    /* CSR_BT_LAN_WORD(0x0800);    IPv4 */
        ETHERNET_DATA(arpRequest)[2] = 0x0406;    /* CSR_BT_LAN_WORD(0x0604);    HW len:6, Proto len: 4 */
        ETHERNET_DATA(arpRequest)[3] = 0x0100;    /* CSR_BT_LAN_WORD(1);         Request */
        ETHERNET_DATA(arpRequest)[4] = sourceEthernetAddress[0];
        ETHERNET_DATA(arpRequest)[5] = sourceEthernetAddress[1];
        ETHERNET_DATA(arpRequest)[6] = sourceEthernetAddress[2];
        ETHERNET_DATA(arpRequest)[7] = sourceIpAddress[0];
        ETHERNET_DATA(arpRequest)[8] = sourceIpAddress[1];
        ETHERNET_DATA(arpRequest)[9] = 0x0000;
        ETHERNET_DATA(arpRequest)[10] = 0x0000;
        ETHERNET_DATA(arpRequest)[11] = 0x0000;
        ETHERNET_DATA(arpRequest)[12] = destinationIpAddress[0];
        ETHERNET_DATA(arpRequest)[13] = destinationIpAddress[1];

        SEND_ETHERNET_PACKET(arpRequest);
    }
}

static void SendArpReply(IpTaskInstanceData * instanceData, CsrUint16 * arpRequestSourceAddress, CsrUint16 * arpRequestData)
{
    ETHERNET_PACKET * arpReply;

    ALLOCATE_ETHERNET_PACKET_REQUEST(arpReply, 28);

    if (arpReply != NULL)
    {
        ETHERNET_SET_LENGTH(arpReply, 28);
        ETHERNET_SET_DESTINATION(arpReply, arpRequestSourceAddress[0], arpRequestSourceAddress[1], arpRequestSourceAddress[2]);
        ETHERNET_SET_SOURCE(arpReply, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);
        ETHERNET_SET_TYPE(arpReply, 0x0806);    /* ARP */
        ETHERNET_DATA(arpReply)[0] = 0x0100;    /* CSR_BT_LAN_WORD(0x0001);    Ethernet */
        ETHERNET_DATA(arpReply)[1] = 0x0008;    /* CSR_BT_LAN_WORD(0x0800);    IPv4 */
        ETHERNET_DATA(arpReply)[2] = 0x0406;    /* CSR_BT_LAN_WORD(0x0604);    HW len:6, Proto len: 4 */
        ETHERNET_DATA(arpReply)[3] = 0x0200;    /* CSR_BT_LAN_WORD(2);         Reply */
        ETHERNET_DATA(arpReply)[4] = instanceData->ethernetAddress[0];
        ETHERNET_DATA(arpReply)[5] = instanceData->ethernetAddress[1];
        ETHERNET_DATA(arpReply)[6] = instanceData->ethernetAddress[2];
        ETHERNET_DATA(arpReply)[7] = instanceData->ipAddress[0];
        ETHERNET_DATA(arpReply)[8] = instanceData->ipAddress[1];
        ETHERNET_DATA(arpReply)[9] = arpRequestData[4];
        ETHERNET_DATA(arpReply)[10] = arpRequestData[5];
        ETHERNET_DATA(arpReply)[11] = arpRequestData[6];
        ETHERNET_DATA(arpReply)[12] = arpRequestData[7];
        ETHERNET_DATA(arpReply)[13] = arpRequestData[8];

        SEND_ETHERNET_PACKET(arpReply);
    }
}

static CsrBool ipAddressKnown(CsrUint16 * destinationIpAddress, ArpCacheEntry * arpCache, CsrUint16 * arpCacheIndex)
{
    CsrUint16 index;

    for (index = 0; index < ARP_CACHE_SIZE; index++)
    {
        if (arpCache[index].secondsUntilDirty && ((destinationIpAddress[0] == arpCache[index].ipAddress[0]) && (destinationIpAddress[1] == arpCache[index].ipAddress[1])))
        {
            *arpCacheIndex = index;
            return TRUE;
        }
    }

    return FALSE;
}

static CsrUint16 getFreeArpEntry(ArpCacheEntry * arpCache)    /* returns first free in cache or the one closest to death! */
{
    CsrUint16 index;
    CsrUint16 leastSecondsUntilDeathIndex = 0;
    CsrUint16 leastSecondsUntilDeath = ARP_LONGEVITY;

    for (index = 0; index < ARP_CACHE_SIZE; index++)
    {
        if (arpCache[index].secondsUntilDirty)
        {
            if (arpCache[index].secondsUntilDirty < leastSecondsUntilDeath)
            {
                leastSecondsUntilDeath = arpCache[index].secondsUntilDirty;
                leastSecondsUntilDeathIndex = index;
            }
        }
        else
        {
            return index;
        }
    }

    arpCache[leastSecondsUntilDeathIndex].secondsUntilDirty = 0;
    if (arpCache[leastSecondsUntilDeathIndex].arpRetransmissionTimerId)
    {
        CsrSchedTimerCancel(arpCache[leastSecondsUntilDeathIndex].arpRetransmissionTimerId, NULL, NULL);
        arpCache[leastSecondsUntilDeathIndex].arpRetransmissionTimerId = 0;
    }
    while (arpCache[leastSecondsUntilDeathIndex].queue != NULL)
    {
        PacketQueue * tmp;

        tmp = arpCache[leastSecondsUntilDeathIndex].queue;
        arpCache[leastSecondsUntilDeathIndex].queue = tmp->next;
        FREE_ETHERNET_PACKET_REQUEST(tmp->requestPacket);
        CsrPmemFree((void *) tmp);
    }

    return leastSecondsUntilDeathIndex;
}

static CsrBool ipBroadcastAddress(IpTaskInstanceData * instanceData, CsrUint16 * destinationIpAddress)    /* destination in network order! */
{
    if (((destinationIpAddress[0] & ((CsrUint16) ~instanceData->subnetMask[0])) == ((CsrUint16) ~instanceData->subnetMask[0])) && ((destinationIpAddress[1] & ((CsrUint16) ~instanceData->subnetMask[1])) == ((CsrUint16) ~instanceData->subnetMask[1])))
    {
        return TRUE;
    }
    return FALSE;
}

static CsrBool localSegmentAddress(IpTaskInstanceData * instanceData, CsrUint16 * destinationIpAddress)    /* destination in network order! */
{
    if (((destinationIpAddress[0] & instanceData->subnetMask[0]) == (instanceData->ipAddress[0] & instanceData->subnetMask[0])) && ((destinationIpAddress[1] & instanceData->subnetMask[1]) == (instanceData->ipAddress[1] & instanceData->subnetMask[1])))
    {
        return TRUE;
    }
    return FALSE;
}

static CsrBool localNodeAddress(IpTaskInstanceData * instanceData, CsrUint16 * destinationIpAddress)    /* destination in network order! */
{
    if ((destinationIpAddress[0] == instanceData->ipAddress[0]) && (destinationIpAddress[1] == instanceData->ipAddress[1]))
    {
        return TRUE;
    }
    return FALSE;
}

/*
  routeIpPacket() pseudo code:

  is this a broadcast?
  YES:
  fill in source/destination details
  send the ethernet packet
  NO:
  is this for the local network segment?
  YES:
  is this for the local node?
  YES:
  get a buffer from the free list
  copy request to buffer and fill in source/destination details
  send the buffer back to ip stack
  CsrPmemFree the request buffer
  NO:
  is there an arp entry that matches the destination address?
  YES:
  is the ethernet address known?
  YES:
  fill in source/destination details
  send the ethernet packet
  NO:
  add the packet to the queue
  NO:
  find vacant arp entry and fill in the details
  queue the request
  send an arp request
  start the arp retransmission timer

  NO:
  is there an arp entry that matches the destination address?
  YES:
  is the ethernet address known?
  YES:
  fill in source/destination details
  send the ethernet packet
  NO:
  add the packet to the queue
  NO:
  find vacant arp entry and fill in the details
  queue the request
  send an arp request to the gateway
  start the arp retransmission timer


  handleArpReception() pseudo code:

  is this a request?
  YES:
  does it match our ip address
  YES:
  reply with the details of this node
  is there a matching request in the cache?
  YES:
  set secondsUntilDirty to CSRMAX
  fill in details
  send the ethernet packets on the queue
  stop the arp retransmission timer
  NO:
  find vacant arp entry and fill in the details
  NO:
  is there a matching request in the cache?
  YES:
  set secondsUntilDirty to CSRMAX
  fill in details
  send the ethernet packets on the queue
  stop the arp retransmission timer


  arpRetransmissionTimer() pseudo code (one timer for each arp request - cache index in mi argument):

  is the retransmission counter less than CSRMAX number of requests?
  YES:
  increment retransmission counter
  send new arp request
  start the arp retransmission timer
  NO:
  clear the cache for this index

*/

static void sendIcmpDestinationUnreachableInd(ETHERNET_PACKET * ipPacket)
{
    CsrBtIpDataInd * ipDataInd;
    CsrUint16 dataOffset;
    CsrUint16 checkSum;
    CsrUint16 index;

    ipDataInd = (CsrBtIpDataInd *) CsrPmemAlloc(sizeof(CsrBtIpDataInd));
    ipDataInd->type = CSR_BT_IP_DATA_IND;
    ipDataInd->ipHeaderLength = 20;
    dataOffset = ((CSR_BT_LAN_WORD(ETHERNET_DATA(ipPacket)[0]) >> 8) & 0x000F) * 2;
    ipDataInd->ipDataLength = 8 + 2 * dataOffset + 8;
    ipDataInd->ipHeader[0] = CSR_BT_LAN_WORD(0x4500);
    ipDataInd->ipHeader[1] = CSR_BT_LAN_WORD(ipDataInd->ipHeaderLength + ipDataInd->ipDataLength);
    ipDataInd->ipHeader[2] = ETHERNET_DATA(ipPacket)[2];
    ipDataInd->ipHeader[3] = 0x0000;
    ipDataInd->ipHeader[4] = CSR_BT_LAN_WORD((CSR_BT_LAN_WORD(ETHERNET_DATA(ipPacket)[4]) & 0xFF00) | CSR_BT_IP_PROTOCOL_ICMP);
    ipDataInd->ipHeader[5] = 0x0000; /* reset checksum */
    ipDataInd->ipHeader[6] = ETHERNET_DATA(ipPacket)[6];
    ipDataInd->ipHeader[7] = ETHERNET_DATA(ipPacket)[7];
    ipDataInd->ipHeader[8] = ETHERNET_DATA(ipPacket)[6];
    ipDataInd->ipHeader[9] = ETHERNET_DATA(ipPacket)[7];
    checkSum = CalculateCheckSum(ipDataInd->ipHeaderLength, (CsrUint8 *) ipDataInd->ipHeader);
    ipDataInd->ipHeader[5] = CSR_BT_LAN_WORD(checkSum);
    ipDataInd->ipData = (CsrUint16 *) CsrPmemAlloc(ipDataInd->ipDataLength);
    ipDataInd->ipData[0] = CSR_BT_LAN_WORD(0x0301);    /* Destination Unreachable, Host unreachable */
    ipDataInd->ipData[1] = 0x0000;    /* reset checksum */
    ipDataInd->ipData[2] = 0x0000;    /* CSR_UNUSED */
    ipDataInd->ipData[3] = 0x0000;    /* CSR_UNUSED */
    for (index = 0; index < dataOffset; index++)
    {
        ipDataInd->ipData[4 + index] = ETHERNET_DATA(ipPacket)[index];
    }
    ipDataInd->ipData[4 + index++] = ETHERNET_DATA(ipPacket)[dataOffset + 0];
    ipDataInd->ipData[4 + index++] = ETHERNET_DATA(ipPacket)[dataOffset + 1];
    ipDataInd->ipData[4 + index++] = ETHERNET_DATA(ipPacket)[dataOffset + 2];
    ipDataInd->ipData[4 + index++] = ETHERNET_DATA(ipPacket)[dataOffset + 3];
    checkSum = CalculateCheckSum(ipDataInd->ipDataLength, (CsrUint8 *) ipDataInd->ipData);
    ipDataInd->ipData[1] = CSR_BT_LAN_WORD(checkSum);

    CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataInd);
}

static void arpRequestTimeout(CsrUint16 arpCacheIndex, void * instanceDataPtr)
{
    IpTaskInstanceData * instanceData;

    instanceData = (IpTaskInstanceData *) instanceDataPtr;
    if (--instanceData->arpCache[arpCacheIndex].arpRetransmissionCount > 0)
    {
        /* send new arp request */
        if (localSegmentAddress(instanceData, instanceData->arpCache[arpCacheIndex].ipAddress))
        {
            SendArpRequest(instanceData->ethernetAddress, instanceData->ipAddress, instanceData->arpCache[arpCacheIndex].ipAddress);
        }
        else
        {
            SendArpRequest(instanceData->ethernetAddress, instanceData->ipAddress, instanceData->gatewayIpAddress);
        }

        /* start the arp retransmission timer */
        instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = CsrSchedTimerSet(ARP_RETRANSMISSION_TIME, arpRequestTimeout, arpCacheIndex, (void *) instanceData);
    }
    else
    {
        PacketQueue * searchPtr;
        PacketQueue * tmpPtr;

        /* clear the cache for this index */
        instanceData->arpCache[arpCacheIndex].secondsUntilDirty = 0;
        instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = 0;

        searchPtr = instanceData->arpCache[arpCacheIndex].queue;
        while (searchPtr != NULL)
        {
            tmpPtr = searchPtr;
            searchPtr = searchPtr->next;
            sendIcmpDestinationUnreachableInd(tmpPtr->requestPacket);
            FREE_ETHERNET_PACKET_REQUEST(tmpPtr->requestPacket);
            CsrPmemFree((void *) tmpPtr);
        }
        instanceData->arpCache[arpCacheIndex].queue = NULL;
    }
}

static void routeIpPacket(IpTaskInstanceData * instanceData, CsrUint16 * destinationIpAddress, ETHERNET_PACKET * ipPacket)
{
    /* is this a broadcast? */
    if (ipBroadcastAddress(instanceData, destinationIpAddress))
    {
        /* fill in source/destination details */
        ETHERNET_SET_DESTINATION(ipPacket, 0xFFFF, 0xFFFF, 0xFFFF);
        ETHERNET_SET_SOURCE(ipPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

        /* send the ethernet packet */
        SEND_ETHERNET_PACKET(ipPacket);
    }
    else
    {
        /* is this for the local network segment? */
        if (localSegmentAddress(instanceData, destinationIpAddress))
        {
            /* is this for the local node? */
            if (localNodeAddress(instanceData, destinationIpAddress))
            {
                ETHERNET_PACKET * ipPacketIndication;
                CsrUint16 index;

                /* get a buffer from the free list */
                ALLOCATE_ETHERNET_PACKET_INDICATION(ipPacketIndication, ETHERNET_GET_LENGTH(ipPacket));
                if (ipPacketIndication != NULL)
                {
                    ETHERNET_SET_LENGTH(ipPacketIndication, ETHERNET_GET_LENGTH(ipPacket));

                    /* copy request to buffer and fill in source/destination details */
                    ETHERNET_SET_DESTINATION(ipPacketIndication, ETHERNET_GET_DESTINATION0(ipPacket), ETHERNET_GET_DESTINATION1(ipPacket), ETHERNET_GET_DESTINATION2(ipPacket));
                    ETHERNET_SET_SOURCE(ipPacketIndication, ETHERNET_GET_DESTINATION0(ipPacket), ETHERNET_GET_DESTINATION1(ipPacket), ETHERNET_GET_DESTINATION2(ipPacket));
                    ETHERNET_SET_TYPE(ipPacketIndication, ETHERNET_GET_TYPE(ipPacket));
                    for (index = 0; index < ETHERNET_GET_LENGTH(ipPacket) / 2; index++)
                    {
                        ETHERNET_DATA(ipPacketIndication)[index] = ETHERNET_DATA(ipPacket)[index];
                    }

                    /* send the buffer back to ip stack */
                    CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, ETHERNET_PRIM, ipPacketIndication);
                }

                /* CsrPmemFree the request buffer */
                FREE_ETHERNET_PACKET_REQUEST(ipPacket);
            }
            else
            {
                CsrUint16 arpCacheIndex;

                /* is there an arp entry that matches the destination address? */
                if (ipAddressKnown(destinationIpAddress, instanceData->arpCache, &arpCacheIndex))
                {
                    /* is the ethernet address known? */
                    if (instanceData->arpCache[arpCacheIndex].ethernetAddress[0] | instanceData->arpCache[arpCacheIndex].ethernetAddress[1] | instanceData->arpCache[arpCacheIndex].ethernetAddress[2])
                    {
                        /* fill in source/destination details */
                        ETHERNET_SET_DESTINATION(ipPacket, instanceData->arpCache[arpCacheIndex].ethernetAddress[0], instanceData->arpCache[arpCacheIndex].ethernetAddress[1], instanceData->arpCache[arpCacheIndex].ethernetAddress[2]);
                        ETHERNET_SET_SOURCE(ipPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

                        /* send the ethernet packet */
                        SEND_ETHERNET_PACKET(ipPacket);
                    }
                    else
                    {
                        /* add the packet to the queue */
                        if (instanceData->arpCache[arpCacheIndex].queue != NULL)
                        {
                            PacketQueue * searchPtr;

                            for (searchPtr = instanceData->arpCache[arpCacheIndex].queue; searchPtr->next != NULL; searchPtr = searchPtr->next)
                            {
                                ;
                            }
                            searchPtr->next = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                            searchPtr->next->requestPacket = ipPacket;
                            searchPtr->next->previous = searchPtr;
                            searchPtr->next->next = NULL;
                        }
                        else
                        {
                            instanceData->arpCache[arpCacheIndex].queue = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                            instanceData->arpCache[arpCacheIndex].queue->requestPacket = ipPacket;
                            instanceData->arpCache[arpCacheIndex].queue->previous = NULL;
                            instanceData->arpCache[arpCacheIndex].queue->next= NULL;
                        }
                    }
                }
                else
                {
                    /* find vacant arp entry and fill in the details */
                    arpCacheIndex = getFreeArpEntry(instanceData->arpCache);
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[0] = 0x0000;
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[1] = 0x0000;
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[2] = 0x0000;
                    instanceData->arpCache[arpCacheIndex].queue = NULL;
                    instanceData->arpCache[arpCacheIndex].ipAddress[0] = destinationIpAddress[0];
                    instanceData->arpCache[arpCacheIndex].ipAddress[1] = destinationIpAddress[1];
                    instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionCount = ARP_MAX_RETRANSMISSIONS;

                    /* queue the request */
                    instanceData->arpCache[arpCacheIndex].queue = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                    instanceData->arpCache[arpCacheIndex].queue->requestPacket = ipPacket;
                    instanceData->arpCache[arpCacheIndex].queue->previous = NULL;
                    instanceData->arpCache[arpCacheIndex].queue->next= NULL;

                    /* send an arp request */
                    SendArpRequest(instanceData->ethernetAddress, instanceData->ipAddress, destinationIpAddress);

                    /* start the arp retransmission timer */
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = CsrSchedTimerSet(ARP_RETRANSMISSION_TIME, arpRequestTimeout, arpCacheIndex, (void *) instanceData);
                }
            }
        }
        else /* this is not for the local network segment, so use the gateway */
        {
            CsrUint16 arpCacheIndex;

            /* is there an arp entry that matches the destination address? */
            if (ipAddressKnown(destinationIpAddress, instanceData->arpCache, &arpCacheIndex))
            {
                /* is the ethernet address known? */
                if (instanceData->arpCache[arpCacheIndex].ethernetAddress[0] | instanceData->arpCache[arpCacheIndex].ethernetAddress[1] | instanceData->arpCache[arpCacheIndex].ethernetAddress[2])
                {
                    /* fill in source/destination details */
                    ETHERNET_SET_DESTINATION(ipPacket, instanceData->arpCache[arpCacheIndex].ethernetAddress[0], instanceData->arpCache[arpCacheIndex].ethernetAddress[1], instanceData->arpCache[arpCacheIndex].ethernetAddress[2]);
                    ETHERNET_SET_SOURCE(ipPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

                    /* send the ethernet packet */
                    SEND_ETHERNET_PACKET(ipPacket);
                }
                else
                {
                    /* add the packet to the queue */
                    if (instanceData->arpCache[arpCacheIndex].queue != NULL)
                    {
                        PacketQueue * searchPtr;

                        for (searchPtr = instanceData->arpCache[arpCacheIndex].queue; searchPtr->next != NULL; searchPtr = searchPtr->next)
                        {
                            ;
                        }
                        searchPtr->next = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                        searchPtr->next->requestPacket = ipPacket;
                        searchPtr->next->previous = searchPtr;
                        searchPtr->next->next = NULL;
                    }
                    else
                    {
                        instanceData->arpCache[arpCacheIndex].queue = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                        instanceData->arpCache[arpCacheIndex].queue->requestPacket = ipPacket;
                        instanceData->arpCache[arpCacheIndex].queue->previous = NULL;
                        instanceData->arpCache[arpCacheIndex].queue->next= NULL;
                    }
                }
            }
            else
            {
                /* find vacant arp entry and fill in the details */
                arpCacheIndex = getFreeArpEntry(instanceData->arpCache);
                instanceData->arpCache[arpCacheIndex].ethernetAddress[0] = 0x0000;
                instanceData->arpCache[arpCacheIndex].ethernetAddress[1] = 0x0000;
                instanceData->arpCache[arpCacheIndex].ethernetAddress[2] = 0x0000;
                instanceData->arpCache[arpCacheIndex].queue = NULL;
                instanceData->arpCache[arpCacheIndex].ipAddress[0] = destinationIpAddress[0];
                instanceData->arpCache[arpCacheIndex].ipAddress[1] = destinationIpAddress[1];
                instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;
                instanceData->arpCache[arpCacheIndex].arpRetransmissionCount = ARP_MAX_RETRANSMISSIONS;

                /* queue the request */
                instanceData->arpCache[arpCacheIndex].queue = (PacketQueue *) CsrPmemAlloc(sizeof(PacketQueue));
                instanceData->arpCache[arpCacheIndex].queue->requestPacket = ipPacket;
                instanceData->arpCache[arpCacheIndex].queue->previous = NULL;
                instanceData->arpCache[arpCacheIndex].queue->next= NULL;

                /* send an arp request to the gateway */
                SendArpRequest(instanceData->ethernetAddress, instanceData->ipAddress, instanceData->gatewayIpAddress);

                /* start the arp retransmission timer */
                instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = CsrSchedTimerSet(ARP_RETRANSMISSION_TIME, arpRequestTimeout, arpCacheIndex, (void *) instanceData);
            }
        }
    }
}

static void handleArpReception(IpTaskInstanceData * instanceData, ETHERNET_PACKET * arpEthernetPacket)
{
    /* first some sanity checks (done in network order to save energy!) */
    if ((ETHERNET_DATA(arpEthernetPacket)[0] == 0x0100) && (ETHERNET_DATA(arpEthernetPacket)[1] == 0x0008) && (ETHERNET_DATA(arpEthernetPacket)[2] == 0x0406))
    {
        /* is this a request? (done in network order to save energy!) */
        if (ETHERNET_DATA(arpEthernetPacket)[3] == 0x0100)
        {
            /* does it match our ip address */
            if ((ETHERNET_DATA(arpEthernetPacket)[12] == instanceData->ipAddress[0]) && (ETHERNET_DATA(arpEthernetPacket)[13] == instanceData->ipAddress[1]))
            {
                CsrUint16 arpCacheIndex;
                CsrUint16 sourceEthernetAddress[3];

                sourceEthernetAddress[0] = ETHERNET_GET_SOURCE0(arpEthernetPacket);
                sourceEthernetAddress[1] = ETHERNET_GET_SOURCE1(arpEthernetPacket);
                sourceEthernetAddress[2] = ETHERNET_GET_SOURCE2(arpEthernetPacket);

                /* reply with the details of this node */
                SendArpReply(instanceData, sourceEthernetAddress, (CsrUint16 *) ETHERNET_DATA(arpEthernetPacket));

                /* is there a matching request in the cache? */
                if (ipAddressKnown((CsrUint16 *) &ETHERNET_DATA(arpEthernetPacket)[7], instanceData->arpCache, &arpCacheIndex))
                {
                    /* set secondsUntilDirty to CSRMAX */
                    instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;

                    /* fill in details */
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[0] = ETHERNET_DATA(arpEthernetPacket)[4];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[1] = ETHERNET_DATA(arpEthernetPacket)[5];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[2] = ETHERNET_DATA(arpEthernetPacket)[6];
                    instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionCount = 0;

                    /* send the ethernet packets on the queue */
                    if (instanceData->arpCache[arpCacheIndex].queue != NULL)
                    {
                        PacketQueue * searchPtr;
                        PacketQueue * tmpPtr;

                        searchPtr = instanceData->arpCache[arpCacheIndex].queue;
                        while (searchPtr != NULL)
                        {
                            tmpPtr = searchPtr;
                            searchPtr = searchPtr->next;

                            /* fill in source/destination details */
                            ETHERNET_SET_DESTINATION(tmpPtr->requestPacket, instanceData->arpCache[arpCacheIndex].ethernetAddress[0], instanceData->arpCache[arpCacheIndex].ethernetAddress[1], instanceData->arpCache[arpCacheIndex].ethernetAddress[2]);
                            ETHERNET_SET_SOURCE(tmpPtr->requestPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

                            /* send the ethernet packet */
                            SEND_ETHERNET_PACKET(tmpPtr->requestPacket);
                            CsrPmemFree(tmpPtr);
                        }
                        instanceData->arpCache[arpCacheIndex].queue = NULL;
                    }

                    /* stop the arp retransmission timer */
                    if (instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId)
                    {
                        CsrSchedTimerCancel(instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId, NULL, NULL);
                        instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = 0;
                    }
                }
                else
                {
                    /* find vacant arp entry and fill in the details */
                    arpCacheIndex = getFreeArpEntry(instanceData->arpCache);

                    instanceData->arpCache[arpCacheIndex].ethernetAddress[0] = ETHERNET_DATA(arpEthernetPacket)[4];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[1] = ETHERNET_DATA(arpEthernetPacket)[5];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[2] = ETHERNET_DATA(arpEthernetPacket)[6];
                    instanceData->arpCache[arpCacheIndex].queue = NULL;
                    instanceData->arpCache[arpCacheIndex].ipAddress[0] = ETHERNET_DATA(arpEthernetPacket)[7];
                    instanceData->arpCache[arpCacheIndex].ipAddress[1] = ETHERNET_DATA(arpEthernetPacket)[8];
                    instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionCount = 0;
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = 0;
                }
            }
        }
        else if (ETHERNET_DATA(arpEthernetPacket)[3] == 0x0200) /* is this a reply? (to filter out RARP) */
        {
            CsrUint16 arpCacheIndex;

            if ((instanceData->gatewayIpAddress[0] == ETHERNET_DATA(arpEthernetPacket)[7]) && (instanceData->gatewayIpAddress[1] == ETHERNET_DATA(arpEthernetPacket)[8]))
            {
                CsrUint16 index;

                for (index = 0; index < ARP_CACHE_SIZE; index++)
                {
                    if (instanceData->arpCache[index].secondsUntilDirty && (((instanceData->gatewayIpAddress[0] == instanceData->arpCache[index].ipAddress[0]) && (instanceData->gatewayIpAddress[1] == instanceData->arpCache[index].ipAddress[1])) || (!localSegmentAddress(instanceData, instanceData->arpCache[index].ipAddress))))
                    {
                        /* set secondsUntilDirty to CSRMAX */
                        instanceData->arpCache[index].secondsUntilDirty = ARP_LONGEVITY;
                        instanceData->arpCache[index].arpRetransmissionCount = 0;

                        /* fill in details */
                        instanceData->arpCache[index].ethernetAddress[0] = ETHERNET_DATA(arpEthernetPacket)[4];
                        instanceData->arpCache[index].ethernetAddress[1] = ETHERNET_DATA(arpEthernetPacket)[5];
                        instanceData->arpCache[index].ethernetAddress[2] = ETHERNET_DATA(arpEthernetPacket)[6];

                        /* send the ethernet packets on the queue */
                        if (instanceData->arpCache[index].queue != NULL)
                        {
                            PacketQueue * searchPtr;
                            PacketQueue * tmpPtr;

                            searchPtr = instanceData->arpCache[index].queue;
                            while (searchPtr != NULL)
                            {
                                tmpPtr = searchPtr;
                                searchPtr = searchPtr->next;

                                /* fill in source/destination details */
                                ETHERNET_SET_DESTINATION(tmpPtr->requestPacket, instanceData->arpCache[index].ethernetAddress[0], instanceData->arpCache[index].ethernetAddress[1], instanceData->arpCache[index].ethernetAddress[2]);
                                ETHERNET_SET_SOURCE(tmpPtr->requestPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

                                /* send the ethernet packet */
                                SEND_ETHERNET_PACKET(tmpPtr->requestPacket);
                                CsrPmemFree(tmpPtr);
                            }
                            instanceData->arpCache[index].queue = NULL;
                        }

                        /* stop the arp retransmission timer */
                        CsrSchedTimerCancel(instanceData->arpCache[index].arpRetransmissionTimerId, NULL, NULL);
                        instanceData->arpCache[index].arpRetransmissionTimerId = 0;
                    }
                }
            }
            else /* not the gateway */
            {
                /* is there a matching request in the cache? */
                if (ipAddressKnown((CsrUint16 *) &ETHERNET_DATA(arpEthernetPacket)[7], instanceData->arpCache, &arpCacheIndex))
                {
                    /* set secondsUntilDirty to CSRMAX */
                    instanceData->arpCache[arpCacheIndex].secondsUntilDirty = ARP_LONGEVITY;
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionCount = 0;

                    /* fill in details */
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[0] = ETHERNET_DATA(arpEthernetPacket)[4];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[1] = ETHERNET_DATA(arpEthernetPacket)[5];
                    instanceData->arpCache[arpCacheIndex].ethernetAddress[2] = ETHERNET_DATA(arpEthernetPacket)[6];

                    /* send the ethernet packets on the queue */
                    if (instanceData->arpCache[arpCacheIndex].queue != NULL)
                    {
                        PacketQueue * searchPtr;
                        PacketQueue * tmpPtr;

                        searchPtr = instanceData->arpCache[arpCacheIndex].queue;
                        while (searchPtr != NULL)
                        {
                            tmpPtr = searchPtr;
                            searchPtr = searchPtr->next;

                            /* fill in source/destination details */
                            ETHERNET_SET_DESTINATION(tmpPtr->requestPacket, instanceData->arpCache[arpCacheIndex].ethernetAddress[0], instanceData->arpCache[arpCacheIndex].ethernetAddress[1], instanceData->arpCache[arpCacheIndex].ethernetAddress[2]);
                            ETHERNET_SET_SOURCE(tmpPtr->requestPacket, instanceData->ethernetAddress[0], instanceData->ethernetAddress[1], instanceData->ethernetAddress[2]);

                            /* send the ethernet packet */
                            SEND_ETHERNET_PACKET(tmpPtr->requestPacket);
                            CsrPmemFree(tmpPtr);
                        }
                        instanceData->arpCache[arpCacheIndex].queue = NULL;
                    }

                    /* stop the arp retransmission timer */
                    CsrSchedTimerCancel(instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId, NULL, NULL);
                    instanceData->arpCache[arpCacheIndex].arpRetransmissionTimerId = 0;
                }
            }
        }
    }
}

static void arpDirtyTimer(CsrUint16 mi, void * mv)
{
    CsrUint16 index;
    ArpCacheEntry * arpCache;

    arpCache = (ArpCacheEntry *) mv;

    for (index = 0; index < ARP_CACHE_SIZE; index++)
    {
        if (arpCache[index].secondsUntilDirty)
        {
            arpCache[index].secondsUntilDirty--;
            if (arpCache[index].secondsUntilDirty)
            {
                if (arpCache[index].queue != NULL)
                {
                    ;    /* TODO: decrement time-to-live in each ip packet on the queue and if zero remove from queue and discard! */
                }
            }
            else    /* the cache just became dirty, so clean up */
            {
                PacketQueue * searchPtr;
                PacketQueue * tmpPtr;

                if (arpCache[index].arpRetransmissionTimerId)
                {
                    CsrSchedTimerCancel(arpCache[index].arpRetransmissionTimerId, NULL, NULL);
                    arpCache[index].arpRetransmissionTimerId = 0;
                }
                searchPtr = arpCache[index].queue;
                while (searchPtr != NULL)
                {
                    tmpPtr = searchPtr;
                    searchPtr = searchPtr->next;
                    FREE_ETHERNET_PACKET_REQUEST(tmpPtr->requestPacket);
                    CsrPmemFree(tmpPtr);
                }
                arpCache[index].queue = NULL;
            }
        }
    }

    CsrSchedTimerSet(1000000, arpDirtyTimer, 0, mv);
}
#else /* EXCLUDE_ARP_MODULE */
static void routeIpPacket(IpTaskInstanceData * instanceData, CsrUint16 * destinationIpAddress, ETHERNET_PACKET * ipPacket)
{
    /* send the ethernet packet */
    SEND_ETHERNET_PACKET(ipPacket);
}
#endif /* EXCLUDE_ARP_MODULE */


/* -------------- IP -------------- */

static CsrBtIpDataInd * processReassembly(IpTaskInstanceData * instanceData, CsrUint16 ipHeaderLength, ETHERNET_PACKET * packetInd)
{
    CsrUint16 flagsAndFragmentOffset;

    flagsAndFragmentOffset = CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[3]);
    if (flagsAndFragmentOffset & 0x3FFF)
    {
        IpReassemblyContainer * ipReassemblyContainer;
        CsrUint16 fragmentOffset;
        CsrUint16 totalLength;
        CsrUint16 currentHoleDescriptorIndex;
        CsrUint16 fragmentHoleFirst;
        CsrUint16 fragmentHoleLast;

        /* is it already in the list? */
        for (ipReassemblyContainer = instanceData->ipReassemblyLine; ipReassemblyContainer != NULL; ipReassemblyContainer = ipReassemblyContainer->next)
        {
            if ((ipReassemblyContainer->destinationIpAddress[0] == ETHERNET_DATA(packetInd)[8]) && (ipReassemblyContainer->destinationIpAddress[1] == ETHERNET_DATA(packetInd)[9])
                && (ipReassemblyContainer->sourceIpAddress[0] == ETHERNET_DATA(packetInd)[6]) && (ipReassemblyContainer->sourceIpAddress[1] == ETHERNET_DATA(packetInd)[7])
                && (ipReassemblyContainer->identification == ETHERNET_DATA(packetInd)[2]) && ((CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[4]) & 0x00FF) == ipReassemblyContainer->protocol))
            {
                break;
            }
        }

        /* if it is and the data area pointer is NULL then break out at this point */
        if (ipReassemblyContainer != NULL)
        {
            if (ipReassemblyContainer->ipDataInd == NULL)
            {
                return NULL;
            }
        }
        else
        {
            /* if not create a new entry (and initialize the member variables) */
            ipReassemblyContainer = (IpReassemblyContainer *) CsrPmemAlloc(sizeof(IpReassemblyContainer));
            ipReassemblyContainer->destinationIpAddress[0] = ETHERNET_DATA(packetInd)[8];
            ipReassemblyContainer->destinationIpAddress[1] = ETHERNET_DATA(packetInd)[9];
            ipReassemblyContainer->sourceIpAddress[0] = ETHERNET_DATA(packetInd)[6];
            ipReassemblyContainer->sourceIpAddress[1] = ETHERNET_DATA(packetInd)[7];
            ipReassemblyContainer->identification = ETHERNET_DATA(packetInd)[2];
            ipReassemblyContainer->protocol = CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[4]) & 0x00FF;
            ipReassemblyContainer->HoleFirstDescriptorIndex = 0;
            ipReassemblyContainer->secondsUntilDiscard = 15;    /* rfc791, p.27, last paragraph - we have to set it to something because it may be discarded before it is set to TTL - 1! */
            ipReassemblyContainer->ipDataInd = (CsrBtIpDataInd *) CsrPmemAlloc(sizeof(CsrBtIpDataInd));
            ipReassemblyContainer->ipDataInd->type = CSR_BT_IP_DATA_IND;
            ipReassemblyContainer->ipDataInd->ipDataLength = 0;
            ipReassemblyContainer->ipDataInd->ipData = (CsrUint16 *) CsrPmemAlloc(MAX_IP_DATA_LENGTH_IN_WORDS * sizeof(CsrUint16));
            ipReassemblyContainer->ipDataInd->ipData[HOLE_FIRST_INDEX] = 0;
            ipReassemblyContainer->ipDataInd->ipData[HOLE_LAST_INDEX] = 0xFFFF;
            ipReassemblyContainer->ipDataInd->ipData[NEXT_HOLE_DESCRIPTOR_INDEX] = 0xFFFF;
            ipReassemblyContainer->ipDataInd->ipData[PREVIOUS_HOLE_DESCRIPTOR_INDEX] = 0xFFFF;
            ipReassemblyContainer->previous = NULL;
            ipReassemblyContainer->next = instanceData->ipReassemblyLine;
            if (ipReassemblyContainer->next != NULL)
            {
                ipReassemblyContainer->next->previous = ipReassemblyContainer;
            }
            instanceData->ipReassemblyLine = ipReassemblyContainer;
        }

        fragmentOffset = flagsAndFragmentOffset & 0x1FFF;

        /* if the fragment is too big to fit in the allocated data area then free */
        totalLength = CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[1]) - ipHeaderLength;
        if (8L * ((CsrUint32) fragmentOffset) + ((CsrUint32) totalLength) >= 2L * ((CsrUint32) MAX_IP_DATA_LENGTH_IN_WORDS))
        {
            /* TODO: send ICMP "Destination Unreachable Message" with "fragmentation needed and DF set" */
            CsrPmemFree(ipReassemblyContainer->ipDataInd->ipData);
            CsrPmemFree(ipReassemblyContainer->ipDataInd);
            ipReassemblyContainer->ipDataInd = NULL;
            ipReassemblyContainer->secondsUntilDiscard = 2;    /* delay for a while before discarding to catch any outstanding fragments! */

            return NULL;
        }

        /* is the fragment offset 0 then copy the ip header */
        if (fragmentOffset == 0)
        {
            CsrUint16 index;

            ipReassemblyContainer->ipDataInd->ipHeaderLength = ipHeaderLength;
            for (index = 0; index < ipHeaderLength / 2; index++)
            {
                ipReassemblyContainer->ipDataInd->ipHeader[index] = ETHERNET_DATA(packetInd)[index];
            }
        }

        /* if the fragment is the last, set the length */
        if (!(flagsAndFragmentOffset & 0x2000))
        {
            ipReassemblyContainer->ipDataInd->ipDataLength = 8 * fragmentOffset + totalLength;
        }

        /* update the holes list and insert fragement in list */
        fragmentHoleFirst = 4 * fragmentOffset;
        fragmentHoleLast = (2 * fragmentHoleFirst + totalLength - 1) / 2;
        currentHoleDescriptorIndex = ipReassemblyContainer->HoleFirstDescriptorIndex;
        while (currentHoleDescriptorIndex != 0xFFFF)
        {
            if (fragmentHoleFirst <= ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_LAST_INDEX])
            {
                CsrUint16 index;

                /* already covered? */
                if (fragmentHoleLast < ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_FIRST_INDEX])
                {
                    return NULL;
                }

                /* do we need to split the current hole at the end of the hole? */
                if (fragmentHoleLast < ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_LAST_INDEX])
                {
                    /* only if there are more fragments! */
                    if (flagsAndFragmentOffset & 0x2000)
                    {
                        ipReassemblyContainer->ipDataInd->ipData[fragmentHoleLast + 1 + HOLE_FIRST_INDEX] = fragmentHoleLast + 1;
                        ipReassemblyContainer->ipDataInd->ipData[fragmentHoleLast + 1 + HOLE_LAST_INDEX] = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_LAST_INDEX];
                        ipReassemblyContainer->ipDataInd->ipData[fragmentHoleLast + 1 + NEXT_HOLE_DESCRIPTOR_INDEX] = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX];
                        ipReassemblyContainer->ipDataInd->ipData[fragmentHoleLast + 1 + PREVIOUS_HOLE_DESCRIPTOR_INDEX] = currentHoleDescriptorIndex;

                        if (ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] != 0xFFFF)
                        {
                            ipReassemblyContainer->ipDataInd->ipData[ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] + PREVIOUS_HOLE_DESCRIPTOR_INDEX] = fragmentHoleLast + 1;
                        }
                        ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] = fragmentHoleLast + 1;
                    }
                }
                else
                {
                    /* do we go beyond the end of the current hole and event beyond the start of the next hole? */
                    if ((ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] != 0xFFFF)
                        && (fragmentHoleLast >= ipReassemblyContainer->ipDataInd->ipData[ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] + HOLE_FIRST_INDEX]))
                    {
                        /*
                          TODO: handle multiple hole coverage - not implemented yet because we're not likely to see this case ever!
                          (this can only happen if a datagram is sent via two different paths and one path sends fragments that are more than twice
                          the size of the the path compared to the other path)
 */
                    }
                }

                /* do we need to split the current hole at the start of the hole? */
                if (fragmentHoleFirst > ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_FIRST_INDEX])
                {
                    ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + HOLE_LAST_INDEX] = fragmentHoleFirst - 1;
                }
                else
                {
                    /* do we have to update the previous hole descriptor? If not, update the containers first descriptor index */
                    if (ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + PREVIOUS_HOLE_DESCRIPTOR_INDEX] != 0xFFFF)
                    {
                        ipReassemblyContainer->ipDataInd->ipData[ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + PREVIOUS_HOLE_DESCRIPTOR_INDEX] + NEXT_HOLE_DESCRIPTOR_INDEX] = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX];
                    }
                    else
                    {
                        ipReassemblyContainer->HoleFirstDescriptorIndex = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX];
                    }

                    /* can we update the next hole descriptor to previous hole descriptor? */
                    if (ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] != 0xFFFF)
                    {
                        ipReassemblyContainer->ipDataInd->ipData[ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX] + PREVIOUS_HOLE_DESCRIPTOR_INDEX] = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + PREVIOUS_HOLE_DESCRIPTOR_INDEX];
                    }
                }

                /* copy the data */
                for (index = 0; index < (fragmentHoleLast - fragmentHoleFirst + 1); index++)
                {
                    ipReassemblyContainer->ipDataInd->ipData[fragmentHoleFirst + index] = ETHERNET_DATA(packetInd)[ipHeaderLength / 2 + index];
                }

                /* is the packet complete? */
                if (ipReassemblyContainer->HoleFirstDescriptorIndex == 0xFFFF)
                {
                    CsrBtIpDataInd * ipDataInd;

                    /* put the data length in the ip header. Since a reassembled packet + header may be > 64K, only set the data length! */
                    ipReassemblyContainer->ipDataInd->ipHeader[1] = CSR_BT_LAN_WORD(ipReassemblyContainer->ipDataInd->ipDataLength);

                    ipDataInd = ipReassemblyContainer->ipDataInd;
                    if (ipReassemblyContainer->previous != NULL)
                    {
                        ipReassemblyContainer->previous->next = ipReassemblyContainer->next;
                    }
                    else
                    {
                        instanceData->ipReassemblyLine = ipReassemblyContainer->next;
                    }
                    if (ipReassemblyContainer->next != NULL)
                    {
                        ipReassemblyContainer->next->previous = ipReassemblyContainer->previous;
                    }
                    CsrPmemFree(ipReassemblyContainer);
                    return ipDataInd;
                }

                break;
            }

            currentHoleDescriptorIndex = ipReassemblyContainer->ipDataInd->ipData[currentHoleDescriptorIndex + NEXT_HOLE_DESCRIPTOR_INDEX];
        }

        /* update the "secondsUntilDiscard" if the TTL - 1 is > current secondsUntilDiscard */
        /* TODO: Add compiler switch for standard TTL algorithm */
        /* NOTICE: To save memory we stick to the 15 secs that we initialized the container with! Remove this comment to apply standard TTL!
           if (ipReassemblyContainer->secondsUntilDiscard < (CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[4]) >> 8) - 1)
           {
           ipReassemblyContainer->secondsUntilDiscard = (CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[4]) >> 8) - 1;
           }
 */
    }
    else
    {
        CsrBtIpDataInd * ipDataInd;
        CsrUint16 index;

        ipDataInd = (CsrBtIpDataInd *) CsrPmemAlloc(sizeof(CsrBtIpDataInd));
        ipDataInd->ipHeaderLength = ipHeaderLength;
        for (index = 0; index < ipHeaderLength / 2; index++)
        {
            ipDataInd->ipHeader[index] = ETHERNET_DATA(packetInd)[index];
        }
        ipDataInd->ipDataLength = CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[1]) - ipHeaderLength;
        ipDataInd->ipHeader[1] = CSR_BT_LAN_WORD(ipDataInd->ipDataLength); /* put the data length in the ip header. Since a reassembled packet + header may be > 64K, only set the data length! So it is consistent with the reassembly case. */
        ipDataInd->ipData = (CsrUint16 *) CsrPmemAlloc(((CsrUint32) ipDataInd->ipDataLength) + (ipDataInd->ipDataLength & 0x0001 ? 1L : 0L));

        if(!(ipDataInd->ipDataLength & 0x0001))
        {/* equal */
            for (index = 0; index < ((CsrUint16) ((((CsrUint32) ipDataInd->ipDataLength)) / 2L)); index++)
            {
                ipDataInd->ipData[index] = ETHERNET_DATA(packetInd)[index + ipHeaderLength / 2];
            }
        }
        else
        {/* unequal */
            for (index = 0; index < ((CsrUint16) ((((CsrUint32) ipDataInd->ipDataLength)) / 2L)); index++)
            {
                ipDataInd->ipData[index] = ETHERNET_DATA(packetInd)[index + ipHeaderLength / 2];
            }
            ipDataInd->ipData[index] = (CsrUint16 ) ((CsrUint8 *)(ETHERNET_DATA(packetInd)))[(index + ipHeaderLength / 2)*2];
        }

        ipDataInd->type = CSR_BT_IP_DATA_IND;
        return ipDataInd;
    }

    return NULL;
}


static void timeToLiveTimeout(CsrUint16 mi, CsrUint16 * mv)
{
    IpTaskInstanceData * instanceData;
    IpReassemblyContainer * ipReassemblyContainer;
    IpReassemblyContainer * tmp;

    instanceData = (IpTaskInstanceData *) mv;

    ipReassemblyContainer = instanceData->ipReassemblyLine;
    while (ipReassemblyContainer != NULL)
    {
        if (ipReassemblyContainer->secondsUntilDiscard)
        {
            ipReassemblyContainer->secondsUntilDiscard--;
        }
        if (!ipReassemblyContainer->secondsUntilDiscard)
        {
            tmp = ipReassemblyContainer->next;
            if (ipReassemblyContainer->previous != NULL)
            {
                ipReassemblyContainer->previous->next = ipReassemblyContainer->next;
            }
            else
            {
                instanceData->ipReassemblyLine = ipReassemblyContainer->next;
            }
            if (ipReassemblyContainer->next != NULL)
            {
                ipReassemblyContainer->next->previous = ipReassemblyContainer->previous;
            }
            if (ipReassemblyContainer->ipDataInd != NULL)
            {
                CsrPmemFree(ipReassemblyContainer->ipDataInd->ipData);
                CsrPmemFree(ipReassemblyContainer->ipDataInd);
            }
            CsrPmemFree(ipReassemblyContainer);
            ipReassemblyContainer = tmp;
        }
        else
        {
            ipReassemblyContainer = ipReassemblyContainer->next;
        }
    }
    if (instanceData->ipReassemblyLine != NULL)
    {
        instanceData->ttlTimerId = CsrSchedTimerSet(1000000, (void (*)(CsrUint16, void *)) timeToLiveTimeout, 0, (void *) instanceData);
    }
    else
    {
        instanceData->ttlTimerId = 0x0000;
        instanceData->ttlTimerRunning = FALSE;
    }
}



static void handleIpReception(IpTaskInstanceData * instanceData, ETHERNET_PACKET * packetInd)
{
    CsrUint16 ipHeaderLength;
    
    ipHeaderLength = 4 * ((CSR_BT_LAN_WORD(ETHERNET_DATA(packetInd)[0]) >> 8) & 0x000F);
    if (CalculateCheckSum(ipHeaderLength, (CsrUint8 *) ETHERNET_DATA(packetInd)) == 0x0000)
    {
        CsrBtIpDataInd * ipDataInd;

        ipDataInd = processReassembly(instanceData, ipHeaderLength, packetInd);
        if (instanceData->ipReassemblyLine != NULL)
        {
            if (!instanceData->ttlTimerRunning)
            {
                instanceData->ttlTimerId = CsrSchedTimerSet(1000000, (void (*)(CsrUint16, void *)) timeToLiveTimeout, 0, (void *) instanceData);
                instanceData->ttlTimerRunning = TRUE;
            }
        }
        else
        {
            if (instanceData->ttlTimerRunning)
            {
                CsrSchedTimerCancel(instanceData->ttlTimerId, NULL, NULL);
                instanceData->ttlTimerId = 0x0000;
                instanceData->ttlTimerRunning = FALSE;
            }
        }
        if (ipDataInd != NULL)
        {
            switch (GET_IP_PROTOCOL(ipDataInd))
            {
                case CSR_BT_IP_PROTOCOL_UDP:
                    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataInd);
                    break;

                case CSR_BT_IP_PROTOCOL_ICMP:
                    CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_IP_PRIM, ipDataInd);
                    break;

                default:
                    /* TODO: send destination unreachable with code set to protocol unreachable */
                    CsrPmemFree(ipDataInd->ipData);
                    CsrPmemFree(ipDataInd);
                    break;
            }
        }
    }
    else /* check sum check failed so throw the packet away! (rfc791, page 3, 2nd paragraph) */
    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
        CsrStateEventException("IP_N_ARP - Check sum error",
                               instanceData->event,
                               ETHERNET_GET_TYPE(packetInd),
                               0);
#endif
    }
}

void CsrBtIpInit(void ** gash)
{
#ifndef EXCLUDE_ARP_MODULE
    int index;
#endif
    IpTaskInstanceData * instanceData;
    CsrBtIpAddressInd * addressInd;

    *gash = (void *) CsrPmemAlloc(sizeof(IpTaskInstanceData));
    instanceData = (IpTaskInstanceData *) *gash;
    instanceData->ethernetAddress[0] = OWN_ETHERNET_ADDRESS_0_NET;
    instanceData->ethernetAddress[1] = OWN_ETHERNET_ADDRESS_1_NET;
    instanceData->ethernetAddress[2] = OWN_ETHERNET_ADDRESS_2_NET;
    instanceData->subnetMask[0] = 0x0000;
    instanceData->subnetMask[1] = 0x0000;
    instanceData->ipAddress[0] = 0x0000;
    instanceData->ipAddress[1] = 0x0000;
    instanceData->gatewayIpAddress[0] = 0x0000;
    instanceData->gatewayIpAddress[1] = 0x0000;
#ifndef EXCLUDE_ARP_MODULE
    for (index = 0; index < ARP_CACHE_SIZE; index++)
    {
        instanceData->arpCache[index].secondsUntilDirty = 0;
        instanceData->arpCache[index].arpRetransmissionTimerId = 0;
    }
    instanceData->arpTimerId = CsrSchedTimerSet(1000000, arpDirtyTimer, 0, instanceData->arpCache);
#endif
    instanceData->ipReassemblyLine = NULL;
    instanceData->ttlTimerRunning = FALSE;
    instanceData->currentIdentificationNumber = 0;

    /* let the UDP layer know our own ip address (needed for check sum generation) */
    addressInd = (CsrBtIpAddressInd *) CsrPmemAlloc(sizeof(CsrBtIpAddressInd));
    addressInd->type = CSR_BT_IP_ADDRESS_IND;
    addressInd->ipAddress[0] = instanceData->ipAddress[0];
    addressInd->ipAddress[1] = instanceData->ipAddress[1];
    CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_IP_PRIM, addressInd);
}

void CsrBtIpHandler(void ** gash)
{
    IpTaskInstanceData * instanceData;

    instanceData = (IpTaskInstanceData *) *gash;

    if (CsrSchedMessageGet(&instanceData->event, &instanceData->message))
    {

        switch (instanceData->event)
        {
            case ETHERNET_PRIM:
                {
                    switch (*((CsrUint16 *) instanceData->message))
                    {
                        case ETHERNET_PACKET_IND:
                            {
                                ETHERNET_PACKET * packetInd;

                                packetInd = (ETHERNET_PACKET *) instanceData->message;

                                switch (ETHERNET_GET_TYPE(packetInd))
                                {
                                    case 0x0800:
                                        /* TODO: update ARP cache "secondsUntilDirty" upon IP reception */
                                        handleIpReception(instanceData, packetInd);
                                        break;

#ifndef EXCLUDE_ARP_MODULE
                                    case 0x0806:
                                        handleArpReception(instanceData, packetInd);
                                        break;
#endif
                                    default:
#if 0
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                                        CsrStateEventException("IP_N_ARP - Unhandled ETHERNET_PRIM",
                                                                instanceData->event,
                                                                ETHERNET_GET_TYPE(packetInd),
                                                                0);
#endif
#endif
                                        break;
                                }

                                FREE_ETHERNET_PACKET_INDICATION(packetInd);
                                instanceData->message = NULL;
                                break;
                            }
                    }
                    break;
                }

            case CSR_BT_IP_PRIM:
                {
                    switch (*((CsrUint16 *) instanceData->message))
                    {
                        case CSR_BT_IP_DATA_REQ:
                            {
                                CsrBtIpDataReq * ipDataReq;
                                ETHERNET_PACKET * transmissionBuffer;
                                CsrUint16 checkSum;
                                CsrUint16 index;
                                CsrUint16 currentIndex;
                                CsrUint16 bytesRemaining;
                                CsrUint16 bytesInPacket;

                                ipDataReq = (CsrBtIpDataReq *) instanceData->message;
                                if ((ipDataReq->sourceAddress[0] == 0x0000) && (ipDataReq->sourceAddress[1] == 0x0000))
                                {
                                    ipDataReq->sourceAddress[0] = instanceData->ipAddress[0];
                                    ipDataReq->sourceAddress[1] = instanceData->ipAddress[1];
                                }
                                bytesRemaining = ipDataReq->ipDataLength;
                                currentIndex = 0;
                                do
                                {
                                    bytesInPacket = bytesRemaining > 1480 ? 1480 : bytesRemaining;
                                    ALLOCATE_ETHERNET_PACKET_REQUEST(transmissionBuffer, 20 + bytesInPacket);
                                    if (transmissionBuffer != NULL)
                                    {
                                        bytesRemaining -= bytesInPacket;
                                        ETHERNET_SET_LENGTH(transmissionBuffer, 20 + bytesInPacket);
                                        ETHERNET_SET_TYPE(transmissionBuffer, 0x0800);
                                        ETHERNET_DATA(transmissionBuffer)[0] = CSR_BT_LAN_WORD(0x4500 | ipDataReq->typeOfService);
                                        ETHERNET_DATA(transmissionBuffer)[1] = CSR_BT_LAN_WORD(ETHERNET_GET_LENGTH(transmissionBuffer));
                                        ETHERNET_DATA(transmissionBuffer)[2] = CSR_BT_LAN_WORD(instanceData->currentIdentificationNumber);
                                        ETHERNET_DATA(transmissionBuffer)[3] = CSR_BT_LAN_WORD((bytesRemaining ? 0x2000 : 0x0000) | (currentIndex >> 2));
                                        ETHERNET_DATA(transmissionBuffer)[4] = CSR_BT_LAN_WORD((ipDataReq->timeToLive << 8) | ipDataReq->protocol);
                                        ETHERNET_DATA(transmissionBuffer)[5] = 0x0000;
                                        ETHERNET_DATA(transmissionBuffer)[6] = ipDataReq->sourceAddress[0];
                                        ETHERNET_DATA(transmissionBuffer)[7] = ipDataReq->sourceAddress[1];
                                        ETHERNET_DATA(transmissionBuffer)[8] = ipDataReq->destinationAddress[0];
                                        ETHERNET_DATA(transmissionBuffer)[9] = ipDataReq->destinationAddress[1];
                                        for (index = 0; index < (bytesInPacket + 1) / 2; index++)
                                        {
                                            ETHERNET_DATA(transmissionBuffer)[10 + index] = ipDataReq->ipData[currentIndex + index];
                                        }
                                        checkSum = CalculateCheckSum(20, (CsrUint8 *) ETHERNET_DATA(transmissionBuffer));
                                        ETHERNET_DATA(transmissionBuffer)[5] = CSR_BT_LAN_WORD(checkSum);
                                        routeIpPacket(instanceData, (CsrUint16 *) &ETHERNET_DATA(transmissionBuffer)[8], transmissionBuffer);
                                        currentIndex += (bytesInPacket / 2);
                                    }
                                    else
                                    {
                                        bytesRemaining = 0;
                                    }
                                } while (bytesRemaining > 0);

                                CsrPmemFree(ipDataReq->ipData);
                                instanceData->currentIdentificationNumber++;
                                break;
                            }

                        case CSR_BT_IP_ADDRESS_REQ:
                            {
                                CsrBtIpAddressReq * addressReq;
                                CsrBtIpAddressInd * addressInd;

                                addressReq = (CsrBtIpAddressReq *) instanceData->message;
                                instanceData->ipAddress[0] = addressReq->ipAddress[0];
                                instanceData->ipAddress[1] = addressReq->ipAddress[1];
                                instanceData->subnetMask[0] = addressReq->subnetMask[0];
                                instanceData->subnetMask[1] = addressReq->subnetMask[1];
                                instanceData->gatewayIpAddress[0] = addressReq->gatewayIpAddress[0];
                                instanceData->gatewayIpAddress[1] = addressReq->gatewayIpAddress[1];

                                /* let the UDP layer know our own ip address (needed for check sum generation) */
                                addressInd = (CsrBtIpAddressInd *) CsrPmemAlloc(sizeof(CsrBtIpAddressInd));
                                addressInd->type = CSR_BT_IP_ADDRESS_IND;
                                addressInd->ipAddress[0] = instanceData->ipAddress[0];
                                addressInd->ipAddress[1] = instanceData->ipAddress[1];
                                CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_IP_PRIM, addressInd);
                                break;
                            }

                        case CSR_BT_IP_MAC_ADDRESS_REQ:
                            {
                                CsrBtIpMacAddressReq * macAddressReq;

                                macAddressReq = (CsrBtIpMacAddressReq *) instanceData->message;

                                instanceData->ethernetAddress[0] = macAddressReq->macAddress[0];
                                instanceData->ethernetAddress[1] = macAddressReq->macAddress[1];
                                instanceData->ethernetAddress[2] = macAddressReq->macAddress[2];
                                break;
                            }

                        case CSR_BT_IP_SHUTDOWN_REQ:
                            {
                                CsrBtIpShutdownInd * shutdownInd;
#ifndef EXCLUDE_ARP_MODULE
                                int index;

                                CsrSchedTimerCancel(instanceData->arpTimerId,NULL,NULL);
                                for (index = 0; index < ARP_CACHE_SIZE; index++)
                                {
                                    if (instanceData->arpCache[index].secondsUntilDirty)
                                    {
                                        if (instanceData->arpCache[index].arpRetransmissionTimerId)
                                        {
                                            CsrSchedTimerCancel(instanceData->arpCache[index].arpRetransmissionTimerId, NULL, NULL);
                                            instanceData->arpCache[index].arpRetransmissionTimerId = 0;
                                        }
                                        while (instanceData->arpCache[index].queue != NULL)
                                        {
                                            PacketQueue * tmp;

                                            tmp = instanceData->arpCache[index].queue;
                                            instanceData->arpCache[index].queue = tmp->next;
                                            FREE_ETHERNET_PACKET_REQUEST(tmp->requestPacket);
                                            CsrPmemFree((void *) tmp);
                                        }
                                    }
                                    instanceData->arpCache[index].secondsUntilDirty = 0;
                                }
#endif
                                instanceData->ethernetAddress[0] = OWN_ETHERNET_ADDRESS_0_NET;
                                instanceData->ethernetAddress[1] = OWN_ETHERNET_ADDRESS_1_NET;
                                instanceData->ethernetAddress[2] = OWN_ETHERNET_ADDRESS_2_NET;
                                instanceData->subnetMask[0] = 0x0000;
                                instanceData->subnetMask[1] = 0x0000;
                                instanceData->ipAddress[0] = 0x0000;
                                instanceData->ipAddress[1] = 0x0000;
                                instanceData->gatewayIpAddress[0] = 0x0000;
                                instanceData->gatewayIpAddress[1] = 0x0000;

                                shutdownInd = (CsrBtIpShutdownInd *) CsrPmemAlloc(sizeof(CsrBtIpShutdownInd));
                                shutdownInd->type = CSR_BT_IP_SHUTDOWN_IND;
                                CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_IP_PRIM, shutdownInd);

                                shutdownInd = (CsrBtIpShutdownInd *) CsrPmemAlloc(sizeof(CsrBtIpShutdownInd));
                                shutdownInd->type = CSR_BT_IP_SHUTDOWN_IND;
                                CsrSchedMessagePut(CSR_BT_UDP_IFACEQUEUE, CSR_BT_IP_PRIM, shutdownInd);

                                /* add TCP here when available... */
                                break;
                            }

                        default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                            CsrStateEventException("IP_N_ARP - Unhandled CSR_BT_IP_PRIM",
                                                    instanceData->event,
                                                    *((CsrUint16 *) instanceData->message),
                                                    0);
#endif
                            break;
                    }
                    break;
                }

            default:
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrStateEventException("IP_N_ARP - unhandled event",
                                        instanceData->event,
                                        0,
                                        0);
#endif
                break;

        }

        CsrPmemFree(instanceData->message);
    }
}


#ifdef ENABLE_SHUTDOWN
void CsrBtIpDeinit(void ** gash)
{
    IpTaskInstanceData * instanceData;
    IpReassemblyContainer * next;
    IpReassemblyContainer * ptr;
#ifndef EXCLUDE_ARP_MODULE
    int index;
#endif

    instanceData = (IpTaskInstanceData *)(* gash);

#ifndef EXCLUDE_ARP_MODULE
    CsrSchedTimerCancel(instanceData->arpTimerId,NULL,NULL);

    for (index = 0; index < ARP_CACHE_SIZE; index++)
    {
        CsrSchedTimerCancel(instanceData->arpCache[index].arpRetransmissionTimerId,NULL,NULL);
    }
#endif

    if(instanceData->ttlTimerRunning)
    {
        CsrSchedTimerCancel(instanceData->ttlTimerId,NULL,NULL);
    }

    if(NULL != instanceData->ipReassemblyLine)
    {
        ptr = instanceData->ipReassemblyLine;

        while( NULL != ptr )
        {
            next = ptr->next;
            CsrPmemFree(ptr);
            ptr = next;
        }
    }

    CsrPmemFree(instanceData); /* instanceData->message is free'ed in CsrBtIpHandler */
}
#endif

