#ifndef CSR_DHCP_SERVER_UTIL_H__
#define CSR_DHCP_SERVER_UTIL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_dhcp_server_handler.h"

#include "csr_ip_socket_prim.h"
#include "csr_ip_socket_lib.h"
#include "csr_macro.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_INVALID                   (0)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPDISCOVER              (1)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPOFFER                 (2)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPREQUEST               (3)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPDECLINE               (4)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPACK                   (5)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPNAK                   (6)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPRELEASE               (7)
#define CSR_DHCP_SERVER_DHCP_MSG_TYPE_DHCPINFORM                (8)

#define CSR_DHCP_SERVER_DHCP_MSG_SIZE_FIXED                     (236)
#define CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_MAGIC              (4)
#define CSR_DHCP_SERVER_DHCP_MSG_SIZE_OPTION_TYPE               (3)

#define CSR_DHCP_SERVER_DHCP_MSG_OFFSET_OPTION_MAGIC            (CSR_DHCP_SERVER_DHCP_MSG_SIZE_FIXED)

#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_PAD                     (0)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_SUBNET_MASK             (1)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_ROUTER                  (3)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_DOMAIN_NAME_SERVER      (6)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_BROADCAST_ADDRESS       (28)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_REQUESTED_IP_ADDRESS    (50)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_LEASE                   (51)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_MSG_TYPE                (53)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_SERVER_IDENTIFIER       (54)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_PARAMETER_REQUEST       (55)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_MSG_MAX_SIZE            (57)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_RENEWAL_TIME_VALUE      (58)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_REBINDING_TIME_VALUE    (59)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_CLIENT_IDENTIFIER       (61)
#define CSR_DHCP_SERVER_DHCP_MSG_OPTION_END                     (255)

#define CSR_DHCP_SERVER_DHCP_MSG_MAGIC_COOKIE                   ("\x63\x82\x53\x63")

#define CSR_DHCP_SERVER_DHCP_MSG_OP_BOOTREPLY                   (2)
#define CSR_DHCP_SERVER_DHCP_MSG_HOPS                           (0)

#define CSR_DHCP_SERVER_BROADCAST_BIT                           (0)

#define CSR_DHCP_SERVER_MAC_ADDRESS_LEN                         (6)

#define CSR_DHCP_SERVER_LEASE_INFINITE_SEC                      (0xFFFFFFFF)
#define CSR_DHCP_SERVER_LEASE_INFINITE_MSEC                     (0xFFFF)

#define CSR_DHCP_SERVER_TIMER_DHCPOFFER_TIMEOUT                 (10)

typedef struct
{
    CsrUint8 op;
    CsrUint8 htype;
    CsrUint8 hlen;
    CsrUint8 hops;

    CsrUint32 xid;

    CsrUint16 secs;
    CsrUint16 flags;

    CsrUint8 ciaddr[4];
    CsrUint8 yiaddr[4];
    CsrUint8 siaddr[4];
    CsrUint8 giaddr[4];

    CsrUint8 chaddr[16];
    CsrUint8 sname[64];
    CsrUint8 file[128];
} CsrDhcpServerDhcpMessage;

typedef struct CsrDhcpServerDhcpOption
{
    struct CsrDhcpServerDhcpOption *next;

    CsrUint8  code;
    CsrUint8  len;
    CsrUint8 *data;
} CsrDhcpServerDhcpOption;

#define CSR_DHCP_SERVER_UINT32_TO_IP_ADDRESS(uint, ip_) CSR_COPY_UINT32_TO_BIG_ENDIAN(uint, ip_)
#define CSR_DHCP_SERVER_IP_ADDRESS_TO_UINT32(ip_)       CSR_GET_UINT32_FROM_BIG_ENDIAN(ip_)
#define CSR_DHCP_SERVER_COPY_IP_ADDRESS(dst_, src_)     CsrMemCpy(dst_, src_, 4)
#define CSR_DHCP_SERVER_IP_ADDRESS_EQUAL(ip1_, ip2_)    (CsrMemCmp(ip1_, ip2_, 4) == 0 ? TRUE : FALSE)
#define CSR_DHCP_SERVER_IP_ADDRESS_IS_ZERO(ip1_)        (ip1_[0] == 0 && ip1_[1] == 0 && ip1_[2] == 0 && ip1_[3] == 0 ? TRUE : FALSE)

void CsrDhcpServerMessageSend(CsrDhcpServerData *server, CsrUint8 *ipAddress, CsrUint16 port, CsrUint16 dataLenght, CsrUint8 *data);
void CsrDhcpServerNetworkBroadcastAddressRetrieve(CsrDhcpServerData *server, CsrUint8 ip[4]);
void CsrDhcpServerTimerExpired(CsrUint16 mi, void *mv);
void CsrDhcpServerHandleMessage(CsrDhcpServerInstanceData *instanceData, CsrDhcpServerData *server, CsrIpSocketUdpDataInd *msg);

#ifdef __cplusplus
}
#endif

#endif
