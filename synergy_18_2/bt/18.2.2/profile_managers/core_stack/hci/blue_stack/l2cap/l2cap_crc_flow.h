/*!
        (c) CSR plc 2010
        All rights reserved

\file   l2cap_crc_flow.h

\brief  CRC-16 algorithm and support functions for L2CAP 1.2 when
        running off-chip. For the on-chip case, the FCS is done in
        hardware.
*/

#ifdef INSTALL_L2CAP_ENHANCED_SUPPORT
#ifndef INSTALL_L2CAP_CRC

#ifndef _L2CAP_CRC_FLOW_H_
#define _L2CAP_CRC_FLOW_H_

#include "csr_synergy.h"

#include "l2cap_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CRC_FLOW_AddFcs(TXQE_T *txqe);
extern CsrBool CRC_FLOW_CheckFcs(CsrMblk *mblk, CsrUint16 *payload_size, CsrUint8 *header);

#ifdef __cplusplus
}
#endif

#endif /* _L2CAP_CRC_FLOW_H_ */
#endif /* INSTALL_L2CAP_CRC */
#endif /* INSTALL_L2CAP_ENHANCED_SUPPORT */
