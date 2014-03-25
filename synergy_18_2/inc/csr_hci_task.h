#ifndef CSR_HCI_TASK_H__
#define CSR_HCI_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_prim_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Queue definition for CSR_HCI */
extern CsrUint16 CSR_HCI_IFACEQUEUE;

/* Task definition for CSR_HCI */
void CsrHciInit(void **gash);
void CsrHciDeinit(void **gash);
void CsrHciHandler(void **gash);

/* HCI */
#define CSR_HCI_INIT    CsrHciInit
#define CSR_HCI_HANDLER CsrHciHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_HCI_DEINIT  CsrHciDeinit
#else
#define CSR_HCI_DEINIT  NULL
#endif


#ifdef __cplusplus
}
#endif


#endif /* CSR_HCI_TASK_H__ */
