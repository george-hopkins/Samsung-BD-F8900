#ifndef CSR_ETHERNET_H__
#define CSR_ETHERNET_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    CsrUint8 a[6];
} EthernetMacAddress;

typedef struct EthernetPacket
{
    CsrUint16 dataLength;                /* Length of data */
    CsrUint8  destination[6];            /* Destination MAC */
    CsrUint8  source[6];                 /* Source MAC */
    CsrUint16 type;                      /* EtherType */
    CsrUint8  data[1500];                /* Data */
    /* NOTE: For internal buffer management, can be ignored */
    struct EthernetPacket *next;
    struct EthernetPacket *previous;
} EthernetPacket;

typedef void (*EthernetCallbackFunction)(void);

typedef CsrUint8 EthernetMode;
#define CSR_ETHERNET_MODE_NORMAL        (EthernetMode) 0
#define CSR_ETHERNET_MODE_MULTICAST     (EthernetMode) 1
#define CSR_ETHERNET_MODE_PROMISCUOUS   (EthernetMode) 2

/* Initialise the Ethernet Adaptor and set the receive callback function */
void EthernetInitialise(EthernetCallbackFunction callback,
    EthernetMacAddress *address,
    EthernetMode mode);

/* Deinitialise the Ethernet Adaptor */
void EthernetDeinitialise(void);

/* Enqueue a packet in the buffer */
void EthernetPacketTransmit(EthernetPacket *packet);

/* Check if more packets are available */
CsrBool EthernetPacketAvailable(void);

/* Receive a packet from the buffer */
EthernetPacket *EthernetPacketReceive(void);

/* Packet Memory Allocation */
EthernetPacket *EthernetPacketAllocate(void);
void EthernetPacketDeallocate(EthernetPacket *packet);

#ifdef __cplusplus
}
#endif

#endif
