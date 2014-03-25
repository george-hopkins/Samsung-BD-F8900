#ifndef CSR_BT_ETHERNET_PRIM_H__
#define CSR_BT_ETHERNET_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_bt_ppp_prim.h"
#include "platform/csr_ethernet.h"

#define OWN_ETHERNET_ADDRESS_0          0x0000
#define OWN_ETHERNET_ADDRESS_1          0x0000
#define OWN_ETHERNET_ADDRESS_2          0x0000
#define OWN_ETHERNET_ADDRESS_0_NET      0x0000
#define OWN_ETHERNET_ADDRESS_1_NET      0x0000
#define OWN_ETHERNET_ADDRESS_2_NET      0x0000

#define ETHERNET_PRIM           0xEEE0
#define ETHERNET_PACKET_IND     0x0001

/* TODO: KR05 hack */
typedef struct CsrBtTesterEthernetPacket
{
    CsrUint16 dataLength;                /* Length of data */
    CsrUint16 destination[3];             /* Destination MAC */
    CsrUint16 source[3];                  /* Source MAC */
    CsrUint16 type;                      /* EtherType */
    CsrUint16 data[750];                 /* Data */
    /* NOTE: For internal buffer management, can be ignored */
    struct CsrBtTesterEthernetPacket *next;
    struct CsrBtTesterEthernetPacket *previous;
} CsrBtTesterEthernetPacket;

typedef struct
{
    CsrUint16 type;
    CsrBtTesterEthernetPacket *packet;
} EthernetPacketInd;

void CsrBtTesterEthernetInitialise(EthernetMacAddress *macAddress);
void CsrBtTesterEthernetDeinitialise(void);

#define PACKET_BUFFER       CsrBtTesterEthernetPacket
#define ETHERNET_PACKET     EthernetPacketInd

#define ALLOCATE_ETHERNET_PACKET_REQUEST(_packet, length)       {_packet = (ETHERNET_PACKET *) CsrPmemAlloc(sizeof(ETHERNET_PACKET));\
                                                                 _packet->packet = (PACKET_BUFFER *) EthernetPacketAllocate();\
                                                                 if (_packet->packet == NULL) {CsrPmemFree(_packet); _packet = NULL;}}

#define ALLOCATE_ETHERNET_PACKET_INDICATION(_packet, length)    {_packet = (ETHERNET_PACKET *) CsrPmemAlloc(sizeof(ETHERNET_PACKET));\
                                                                 _packet->packet = (PACKET_BUFFER *) EthernetPacketAllocate();\
                                                                 if (_packet->packet == NULL) {CsrPmemFree(_packet); _packet = NULL;}}

#define SEND_ETHERNET_PACKET(_packet)                           {EthernetPacketTransmit((EthernetPacket *) _packet->packet);\
                                                                 CsrPmemFree(_packet);}
#define FREE_ETHERNET_PACKET_REQUEST(_packet)                   {EthernetPacketDeallocate((EthernetPacket *)_packet->packet);\
                                                                 CsrPmemFree(_packet);}
#define FREE_ETHERNET_PACKET_INDICATION(_packet)                {EthernetPacketDeallocate((EthernetPacket *)_packet->packet);\
                                                                 CsrPmemFree(_packet);}

#define ETHERNET_LENGTH(_packet)                                (_packet->packet->dataLength)
#define ETHERNET_DATA(_packet)                                  (_packet->packet->data)
#define ETHERNET_SET_LENGTH(_packet, _length)                   {_packet->packet->dataLength = _length;}
#define ETHERNET_GET_LENGTH(_packet)                            (_packet->packet->dataLength)
#define ETHERNET_SET_TYPE(_packet, _type)                       {_packet->packet->type = _type;}
#define ETHERNET_GET_TYPE(_packet)                              (_packet->packet->type)

#define ETHERNET_SET_DESTINATION(_packet, _d0, _d1, _d2)        {_packet->packet->destination[0] = _d0;\
                                                                 _packet->packet->destination[1] = _d1;\
                                                                 _packet->packet->destination[2] = _d2;}
#define ETHERNET_GET_DESTINATION0(_packet)                      (_packet->packet->destination[0])
#define ETHERNET_GET_DESTINATION1(_packet)                      (_packet->packet->destination[1])
#define ETHERNET_GET_DESTINATION2(_packet)                      (_packet->packet->destination[2])

#define ETHERNET_SET_SOURCE(_packet, _s0, _s1, _s2)             {_packet->packet->source[0] = _s0;\
                                                                 _packet->packet->source[1] = _s1;\
                                                                 _packet->packet->source[2] = _s2;}
#define ETHERNET_GET_SOURCE0(_packet)                           (_packet->packet->source[0])
#define ETHERNET_GET_SOURCE1(_packet)                           (_packet->packet->source[1])
#define ETHERNET_GET_SOURCE2(_packet)                           (_packet->packet->source[2])

#ifdef __cplusplus
extern "C" }
#endif

#endif /* CSR_BT_ETHERNET_PRIM_H__ */
