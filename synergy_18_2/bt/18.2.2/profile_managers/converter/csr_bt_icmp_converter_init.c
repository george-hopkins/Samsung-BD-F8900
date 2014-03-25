/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_icmp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_ICMP_MODULE
#include "csr_bt_icmp_serialize.h"
#include "csr_bt_icmp_prim.h"

static CsrMsgConvMsgEntry csr_bt_icmp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ
    { CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ, CsrBtIcmpDestinationUnreachableReqSizeof, CsrBtIcmpDestinationUnreachableReqSer, CsrBtIcmpDestinationUnreachableReqDes, CsrBtIcmpDestinationUnreachableReqSerFree },
#endif /* EXCLUDE_CSR_BT_ICMP_DESTINATION_UNREACHABLE_REQ */
#ifndef EXCLUDE_CSR_BT_ICMP_PING_CFM
    { CSR_BT_ICMP_PING_CFM, CsrBtIcmpPingCfmSizeof, CsrBtIcmpPingCfmSer, CsrBtIcmpPingCfmDes, CsrBtIcmpPingCfmSerFree },
#endif /* EXCLUDE_CSR_BT_ICMP_PING_CFM */
#ifndef EXCLUDE_CSR_BT_ICMP_PING_REQ
    { CSR_BT_ICMP_PING_REQ, CsrBtIcmpPingReqSizeof, CsrBtIcmpPingReqSer, CsrBtIcmpPingReqDes, CsrBtIcmpPingReqSerFree },
#endif /* EXCLUDE_CSR_BT_ICMP_PING_REQ */
#ifndef EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND
    { CSR_BT_ICMP_PORT_UNREACHABLE_IND, CsrBtIcmpPortUnreachableIndSizeof, CsrBtIcmpPortUnreachableIndSer, CsrBtIcmpPortUnreachableIndDes, CsrBtIcmpPortUnreachableIndSerFree },
#endif /* EXCLUDE_CSR_BT_ICMP_PORT_UNREACHABLE_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtIcmpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_ICMP_PRIM, csr_bt_icmp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_icmp_conv_info = {
    CSR_BT_ICMP_PRIM,
    "CSR_BT_ICMP_PRIM",
    csr_bt_icmp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtIcmpTechInfoGet(void)
{
    return &csr_bt_icmp_conv_info;
}
#endif
#endif

