/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_macro.h"
#include "csr_ip_ether_prim.h"
#include "csr_ip_ether_lib.h"

#include "platform/csr_ethernet_task.h"
#include "platform/csr_confdata.h"
#include "platform/csr_ethernet.h"

#define CSR_ETHER_PACKET_RX_COUNT 8

typedef struct
{
    EthernetMacAddress        mac;
    CsrIpEtherFrameRxFunction frameRxFunction;
    CsrIpEtherIpContext       ipContext;
} CsrEthernetInstanceData;

static CsrSchedBgint bgintHandle = CSR_SCHED_BGINT_INVALID;

/* The IP stack calls this function to transmit data. */
static CsrResult frameTxFunction(CsrUint8 *destinationMac, CsrUint8 *sourceMac,
    CsrUint16 frameLength, CsrUint8 *frame, CsrIpEtherIfContext ifContext)
{
    EthernetPacket *packet;

    CSR_UNUSED(ifContext);

    packet = EthernetPacketAllocate();

    if (packet != NULL)
    {
        CsrMemCpy(packet->destination, destinationMac, 6);
        CsrMemCpy(packet->source, sourceMac, 6);

        /* +2 to skip the etherType, which we will fill in later */
        CsrMemCpy(packet->data, frame + 2, frameLength - 2);
        packet->dataLength = frameLength - 2;

        packet->type = (frame[0] << 8) | (frame[1]);

        EthernetPacketTransmit(packet);

        return CSR_RESULT_SUCCESS;
    }

    return CSR_IP_ETHER_RESULT_NOT_SENT;
}

static void receiveBackgroundHandler(void *arg)
{
    CsrEthernetInstanceData *inst;
    CsrUint8 i;

    inst = (CsrEthernetInstanceData *) arg;

    /*
     * Retrieve up to CSR_ETHER_PACKET_RX_COUNT
     * packets from the ethernet interface
     */
    for (i = 0; i < CSR_ETHER_PACKET_RX_COUNT; i++)
    {
        EthernetPacket *packet;

        packet = EthernetPacketReceive();

        /* If a packet was received, put it on the IP queue */
        if (packet != NULL)
        {
            CsrUint16 etherType;
            /* Repackage the type field to network byte order (rest is already in network byte order) */
            etherType = packet->type;

            *(((CsrUint8 *) &packet->type) + 0) = (CsrUint8) ((etherType >> 8) & 0xFF);
            *(((CsrUint8 *) &packet->type) + 1) = (CsrUint8) (etherType & 0xFF);

            inst->frameRxFunction(packet->destination, packet->source,
                0, packet->dataLength + 2, (CsrUint8 *) &packet->type, inst->ipContext);

            EthernetPacketDeallocate(packet);
        }
        else
        {
            break;
        }
    }

    if (EthernetPacketAvailable())
    {
        CsrSchedBgintSet(bgintHandle);
    }
}

static void triggerBgint(void)
{
    CsrSchedBgintSet(bgintHandle);
}

void CsrEthernetTaskInitialise(void **gash)
{
    CsrEthernetInstanceData *inst;

    inst = *gash = CsrPmemAlloc(sizeof(CsrEthernetInstanceData));

    /* Get the MAC address stored in NAND Flash */
    ConfDataLightweightGetValue(CONFDATA_TAG_MACADDRESS, 6,
        inst->mac.a);

    /* Add the Ethernet interface to CSR IP */
    CsrIpEtherIfAddReqSend(CSR_ETHERNET_IFACEQUEUE,
        CSR_IP_ETHER_IF_TYPE_WIRED,
        inst->mac.a,
        CSR_IP_ETHER_ENCAPSULATION_ETHERNET,
        1500,
        frameTxFunction,
        inst);
}

void CsrEthernetTaskHandler(void **gash)
{
    CsrEthernetInstanceData *inst;
    void *message;
    CsrUint16 event;

    inst = (CsrEthernetInstanceData *) *gash;

    if (CsrSchedMessageGet(&event, &message))
    {
        if (event == CSR_IP_ETHER_PRIM)
        {
            switch (*((CsrPrim *) message))
            {
                case CSR_IP_ETHER_IF_ADD_CFM:
                {
                    CsrIpEtherIfAddCfm *confirm;

                    confirm = (CsrIpEtherIfAddCfm *) message;

                    /* Save ipContext and frameRxFunction */
                    inst->frameRxFunction = confirm->frameRxFunction;
                    inst->ipContext = confirm->ipContext;

                    /* Set link state up */
                    CsrIpEtherIfLinkUpReqSend(CSR_ETHERNET_IFACEQUEUE,
                        confirm->ifHandle)
                    break;
                }

                case CSR_IP_ETHER_IF_LINK_UP_CFM:
                {
                    /* Register the background interrupt handler */
                    bgintHandle = CsrSchedBgintReg(receiveBackgroundHandler,
                        inst, "Ethernet");

                    /* Initialise the low level driver - will trigger background
                       interrupt when packets are received. */
                    EthernetInitialise(triggerBgint,
                        &(inst->mac),
                        CSR_ETHERNET_MODE_MULTICAST);
                    break;
                }

                case CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND:
                {
                    CsrIpEtherIfMulticastAddrAddInd *ind;
                    ind = (CsrIpEtherIfMulticastAddrAddInd *) message;

                    CsrIpEtherIfMulticastAddrAddResSend(
                        ind->ifHandle,
                        CSR_RESULT_SUCCESS);
                    break;
                }

                case CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND:
                {
                    CsrIpEtherIfMulticastAddrRemoveInd *ind;
                    ind = (CsrIpEtherIfMulticastAddrRemoveInd *) message;

                    CsrIpEtherIfMulticastAddrRemoveResSend(
                        ind->ifHandle,
                        CSR_RESULT_SUCCESS);
                    break;
                }

                case CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND:
                {
                    CsrIpEtherIfMulticastAddrFlushInd *ind;
                    ind = (CsrIpEtherIfMulticastAddrFlushInd *) message;

                    CsrIpEtherIfMulticastAddrFlushResSend(
                        ind->ifHandle,
                        CSR_RESULT_SUCCESS);
                    break;
                }
            }
        }

        CsrPmemFree(message);
    }
}

void CsrEthernetTaskDeinitialise(void **gash)
{
    CsrEthernetInstanceData *inst;
    void *message;
    CsrUint16 event;

    inst = (CsrEthernetInstanceData *) *gash;

    EthernetDeinitialise();

    /* Clean out our own queue */
    while (CsrSchedMessageGet(&event, &message))
    {
        if (event == CSR_IP_ETHER_PRIM)
        {
            CsrIpEtherFreeUpstreamMessageContents(event, message);
        }
    }

    CsrPmemFree(inst);
}
