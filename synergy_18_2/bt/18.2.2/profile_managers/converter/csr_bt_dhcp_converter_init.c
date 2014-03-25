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
#include "csr_bt_dhcp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_DHCP_MODULE
#include "csr_bt_dhcp_serialize.h"
#include "csr_bt_dhcp_prim.h"

static CsrMsgConvMsgEntry csr_bt_dhcp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM
    { CSR_BT_DHCP_ACTIVATE_CLIENT_CFM, CsrBtDhcpActivateClientCfmSizeof, CsrBtDhcpActivateClientCfmSer, CsrBtDhcpActivateClientCfmDes, CsrBtDhcpActivateClientCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM */
#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ
    { CSR_BT_DHCP_ACTIVATE_CLIENT_REQ, CsrBtDhcpActivateClientReqSizeof, CsrBtDhcpActivateClientReqSer, CsrBtDhcpActivateClientReqDes, CsrBtDhcpActivateClientReqSerFree },
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ */
#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM
    { CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM, CsrBtDhcpDeactivateClientCfmSizeof, CsrBtDhcpDeactivateClientCfmSer, CsrBtDhcpDeactivateClientCfmDes, CsrBtDhcpDeactivateClientCfmSerFree },
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM */
#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND
    { CSR_BT_DHCP_DEACTIVATE_CLIENT_IND, CsrBtDhcpDeactivateClientIndSizeof, CsrBtDhcpDeactivateClientIndSer, CsrBtDhcpDeactivateClientIndDes, CsrBtDhcpDeactivateClientIndSerFree },
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND */
#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ
    { CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ, CsrBtDhcpDeactivateClientReqSizeof, CsrBtDhcpDeactivateClientReqSer, CsrBtDhcpDeactivateClientReqDes, CsrBtDhcpDeactivateClientReqSerFree },
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtDhcpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_DHCP_PRIM, csr_bt_dhcp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_dhcp_conv_info = {
    CSR_BT_DHCP_PRIM,
    "CSR_BT_DHCP_PRIM",
    csr_bt_dhcp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtDhcpTechInfoGet(void)
{
    return &csr_bt_dhcp_conv_info;
}
#endif
#endif

