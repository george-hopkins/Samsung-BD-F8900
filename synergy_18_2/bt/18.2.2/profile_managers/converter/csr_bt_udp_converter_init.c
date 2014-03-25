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
#include "csr_bt_udp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_UDP_MODULE
#include "csr_bt_udp_serialize.h"
#include "csr_bt_udp_prim.h"

static CsrMsgConvMsgEntry csr_bt_udp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
    { CSR_BT_UDP_DATA_IND, CsrBtUdpDataIndSizeof, CsrBtUdpDataIndSer, CsrBtUdpDataIndDes, CsrBtUdpDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
    { CSR_BT_UDP_DATA_REQ, CsrBtUdpDataReqSizeof, CsrBtUdpDataReqSer, CsrBtUdpDataReqDes, CsrBtUdpDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ
    { CSR_BT_UDP_DEREGISTER_PORT_REQ, CsrBtUdpDeregisterPortReqSizeof, CsrBtUdpDeregisterPortReqSer, CsrBtUdpDeregisterPortReqDes, CsrBtUdpDeregisterPortReqSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ */
#ifndef EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND
    { CSR_BT_UDP_PORT_UNREACHABLE_IND, CsrBtUdpPortUnreachableIndSizeof, CsrBtUdpPortUnreachableIndSer, CsrBtUdpPortUnreachableIndDes, CsrBtUdpPortUnreachableIndSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND */
#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM
    { CSR_BT_UDP_REGISTER_PORT_CFM, CsrBtUdpRegisterPortCfmSizeof, CsrBtUdpRegisterPortCfmSer, CsrBtUdpRegisterPortCfmDes, CsrBtUdpRegisterPortCfmSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM */
#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ
    { CSR_BT_UDP_REGISTER_PORT_REQ, CsrBtUdpRegisterPortReqSizeof, CsrBtUdpRegisterPortReqSer, CsrBtUdpRegisterPortReqDes, CsrBtUdpRegisterPortReqSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ */
#ifndef EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND
    { CSR_BT_UDP_SHUTDOWN_IND, CsrBtUdpShutdownIndSizeof, CsrBtUdpShutdownIndSer, CsrBtUdpShutdownIndDes, CsrBtUdpShutdownIndSerFree },
#endif /* EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtUdpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_UDP_PRIM, csr_bt_udp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_udp_conv_info = {
    CSR_BT_UDP_PRIM,
    "CSR_BT_UDP_PRIM",
    csr_bt_udp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtUdpTechInfoGet(void)
{
    return &csr_bt_udp_conv_info;
}
#endif
#endif

