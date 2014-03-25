#ifndef CSR_HCI_PRIVATE_LIB_H__
#define CSR_HCI_PRIVATE_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_hci_private_prim.h"
#include "csr_hci_task.h"
#include "csr_msg_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrHciPrivateFreeUpstreamMessageContents(CsrUint16 eventClass, void *message);
void CsrHciPrivateFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_HCI_PRIVATE_LIB_H__ */
