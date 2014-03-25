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
#include "csr_bt_ip_converter_init.h"
#ifndef EXCLUDE_CSR_BT_IP_MODULE
#include "csr_bt_ip_serialize.h"
#include "csr_bt_ip_prim.h"

static CsrMsgConvMsgEntry csr_bt_ip_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_IND
    { CSR_BT_IP_ADDRESS_IND, CsrBtIpAddressIndSizeof, CsrBtIpAddressIndSer, CsrBtIpAddressIndDes, CsrBtIpAddressIndSerFree },
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_IND */
#ifndef EXCLUDE_CSR_BT_IP_ADDRESS_REQ
    { CSR_BT_IP_ADDRESS_REQ, CsrBtIpAddressReqSizeof, CsrBtIpAddressReqSer, CsrBtIpAddressReqDes, CsrBtIpAddressReqSerFree },
#endif /* EXCLUDE_CSR_BT_IP_ADDRESS_REQ */
#ifndef EXCLUDE_CSR_BT_IP_DATA_IND
    { CSR_BT_IP_DATA_IND, CsrBtIpDataIndSizeof, CsrBtIpDataIndSer, CsrBtIpDataIndDes, CsrBtIpDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_IP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_IP_DATA_REQ
    { CSR_BT_IP_DATA_REQ, CsrBtIpDataReqSizeof, CsrBtIpDataReqSer, CsrBtIpDataReqDes, CsrBtIpDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_IP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ
    { CSR_BT_IP_MAC_ADDRESS_REQ, CsrBtIpMacAddressReqSizeof, CsrBtIpMacAddressReqSer, CsrBtIpMacAddressReqDes, CsrBtIpMacAddressReqSerFree },
#endif /* EXCLUDE_CSR_BT_IP_MAC_ADDRESS_REQ */
#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_IND
    { CSR_BT_IP_SHUTDOWN_IND, CsrBtIpShutdownIndSizeof, CsrBtIpShutdownIndSer, CsrBtIpShutdownIndDes, CsrBtIpShutdownIndSerFree },
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_IND */
#ifndef EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ
    { CSR_BT_IP_SHUTDOWN_REQ, CsrBtIpShutdownReqSizeof, CsrBtIpShutdownReqSer, CsrBtIpShutdownReqDes, CsrBtIpShutdownReqSerFree },
#endif /* EXCLUDE_CSR_BT_IP_SHUTDOWN_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtIpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_IP_PRIM, csr_bt_ip_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_ip_conv_info = {
    CSR_BT_IP_PRIM,
    "CSR_BT_IP_PRIM",
    csr_bt_ip_conv_lut
};

CsrLogPrimitiveInformation* CsrBtIpTechInfoGet(void)
{
    return &csr_bt_ip_conv_info;
}
#endif
#endif

