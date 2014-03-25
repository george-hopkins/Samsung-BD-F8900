#ifndef CSR_BT_ETHERNET_PRIM_H__
#define CSR_BT_ETHERNET_PRIM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_ppp_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OWN_ETHERNET_ADDRESS_0        0x0002
#define OWN_ETHERNET_ADDRESS_1        0x5B01
#define OWN_ETHERNET_ADDRESS_2        0x6C15
#define OWN_ETHERNET_ADDRESS_0_NET    0x0200
#define OWN_ETHERNET_ADDRESS_1_NET    0x015B
#define OWN_ETHERNET_ADDRESS_2_NET    0x156C

#define ETHERNET_PRIM            CSR_BT_PPP_PRIM
#define ETHERNET_PACKET_IND      CSR_BT_PPP_DATA_IND

#define ETHERNET_PACKET    CsrBtPppDataInd

#define ALLOCATE_ETHERNET_PACKET_REQUEST(packet, length) \
    (packet) = (ETHERNET_PACKET *) CsrPmemAlloc(sizeof(ETHERNET_PACKET)); \
    (packet)->payload = (CsrUint8 *) CsrPmemAlloc((length) + ((length) & 0x1))

#define ALLOCATE_ETHERNET_PACKET_INDICATION(packet, length) \
    (packet) = (ETHERNET_PACKET *) CsrPmemAlloc(sizeof(ETHERNET_PACKET)); \
    (packet)->payload = (CsrUint8 *) CsrPmemAlloc((length) + ((length) & 0x1))

#define SEND_ETHERNET_PACKET(packet) \
    (packet)->type = CSR_BT_PPP_DATA_REQ; \
    (packet)->protocol = CSR_BT_LAN_WORD(0x0021); \
    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, (packet))

#define FREE_ETHERNET_PACKET_REQUEST(packet) \
    CsrPmemFree((packet)->payload); \
    CsrPmemFree((packet))

#define FREE_ETHERNET_PACKET_INDICATION(packet) \
    CsrPmemFree((packet)->payload); \
    CsrPmemFree((packet))

#define ETHERNET_SET_LENGTH(packet, length) \
    (packet)->payloadLength = (length)

#define ETHERNET_GET_LENGTH(packet) \
    (packet)->payloadLength

#define ETHERNET_SET_DESTINATION(packet, destination0, destination1, destination2)
#define ETHERNET_GET_DESTINATION0(packet)                    0xFFFF
#define ETHERNET_GET_DESTINATION1(packet)                    0xFFFF
#define ETHERNET_GET_DESTINATION2(packet)                    0xFFFF
#define ETHERNET_SET_SOURCE(packet, source0, source1, source2)
#define ETHERNET_GET_SOURCE0(packet)                        0xFFFF
#define ETHERNET_GET_SOURCE1(packet)                        0xFFFF
#define ETHERNET_GET_SOURCE2(packet)                        0xFFFF

#define ETHERNET_SET_TYPE(packet, type) \
    (packet)->protocol = CSR_BT_LAN_WORD(0x0021)

#define ETHERNET_GET_TYPE(packet)                            0x0800

#define ETHERNET_DATA(packet) \
    ((CsrUint16 *) (packet)->payload)

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_ETHERNET_PRIM_H__ */
