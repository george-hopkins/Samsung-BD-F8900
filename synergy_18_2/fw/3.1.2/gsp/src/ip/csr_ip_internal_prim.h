#ifndef CSR_IP_INTERNAL_PRIM_H__
#define CSR_IP_INTERNAL_PRIM_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_handler.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* search_string="CsrIpInternalPrimType" */
/* conversion_rule="ADD_UNDERSCORE_AND_UPPERCASE_T" */

typedef CsrUint16 CsrIpInternalPrimType;

#define CSR_IP_INTERNAL_ETHER_PACKET_REQ    ((CsrIpInternalPrimType) 0x0000)
#define NUMBER_OF_CSR_IP_INTERNAL_MESSAGES  0x0001

typedef struct
{
    CsrUint16      type;
    CsrUint16      ethType;
    struct pbuf   *buffer;
    struct netif **netif;
} CsrIpInternalEtherPacketReq;

#ifdef __cplusplus
}
#endif

#endif
