#ifndef CSR_BT_DHCP_SERIALIZE_H__
#define CSR_BT_DHCP_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtDhcpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ
CsrSize CsrBtDhcpDeactivateClientReqSizeof(void *msg);
CsrUint8 *CsrBtDhcpDeactivateClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDhcpDeactivateClientReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDhcpDeactivateClientReqSerFree CsrBtDhcpPfree
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM
CsrSize CsrBtDhcpDeactivateClientCfmSizeof(void *msg);
CsrUint8 *CsrBtDhcpDeactivateClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDhcpDeactivateClientCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDhcpDeactivateClientCfmSerFree CsrBtDhcpPfree
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ
CsrSize CsrBtDhcpActivateClientReqSizeof(void *msg);
CsrUint8 *CsrBtDhcpActivateClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDhcpActivateClientReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDhcpActivateClientReqSerFree CsrBtDhcpPfree
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND
CsrSize CsrBtDhcpDeactivateClientIndSizeof(void *msg);
CsrUint8 *CsrBtDhcpDeactivateClientIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDhcpDeactivateClientIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDhcpDeactivateClientIndSerFree CsrBtDhcpPfree
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM
CsrSize CsrBtDhcpActivateClientCfmSizeof(void *msg);
CsrUint8 *CsrBtDhcpActivateClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtDhcpActivateClientCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtDhcpActivateClientCfmSerFree CsrBtDhcpPfree
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM */

#ifdef __cplusplus
}
#endif

#endif
