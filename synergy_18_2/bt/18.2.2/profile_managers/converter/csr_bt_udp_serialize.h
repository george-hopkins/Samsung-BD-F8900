#ifndef CSR_BT_UDP_SERIALIZE_H__
#define CSR_BT_UDP_SERIALIZE_H__
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

void CsrBtUdpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM
CsrSize CsrBtUdpRegisterPortCfmSizeof(void *msg);
CsrUint8 *CsrBtUdpRegisterPortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpRegisterPortCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtUdpRegisterPortCfmSerFree CsrBtUdpPfree
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_CFM */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_IND
CsrSize CsrBtUdpDataIndSizeof(void *msg);
CsrUint8 *CsrBtUdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtUdpDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND
CsrSize CsrBtUdpPortUnreachableIndSizeof(void *msg);
CsrUint8 *CsrBtUdpPortUnreachableIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpPortUnreachableIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtUdpPortUnreachableIndSerFree CsrBtUdpPfree
#endif /* EXCLUDE_CSR_BT_UDP_PORT_UNREACHABLE_IND */

#ifndef EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ
CsrSize CsrBtUdpDeregisterPortReqSizeof(void *msg);
CsrUint8 *CsrBtUdpDeregisterPortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpDeregisterPortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtUdpDeregisterPortReqSerFree CsrBtUdpPfree
#endif /* EXCLUDE_CSR_BT_UDP_DEREGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ
CsrSize CsrBtUdpRegisterPortReqSizeof(void *msg);
CsrUint8 *CsrBtUdpRegisterPortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpRegisterPortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtUdpRegisterPortReqSerFree CsrBtUdpPfree
#endif /* EXCLUDE_CSR_BT_UDP_REGISTER_PORT_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_DATA_REQ
CsrSize CsrBtUdpDataReqSizeof(void *msg);
CsrUint8 *CsrBtUdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtUdpDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND
CsrSize CsrBtUdpShutdownIndSizeof(void *msg);
CsrUint8 *CsrBtUdpShutdownIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtUdpShutdownIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtUdpShutdownIndSerFree CsrBtUdpPfree
#endif /* EXCLUDE_CSR_BT_UDP_SHUTDOWN_IND */

#ifdef __cplusplus
}
#endif

#endif
