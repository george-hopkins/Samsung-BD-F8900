#ifndef CSR_DATA_STORE_TASK_H__
#define CSR_DATA_STORE_TASK_H__
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

/* Queue definition for CSR_DATA_STORE */
extern CsrUint16 CSR_DATA_STORE_IFACEQUEUE;

/* Task definition for CSR_DATA_STORE */
void CsrDataStoreInit(void **gash);
void CsrDataStoreDeinit(void **gash);
void CsrDataStoreHandler(void **gash);

/* DATA_STORE */
#define CSR_DATA_STORE_INIT    CsrDataStoreInit
#define CSR_DATA_STORE_HANDLER CsrDataStoreHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_DATA_STORE_DEINIT  CsrDataStoreDeinit
#else
#define CSR_DATA_STORE_DEINIT  NULL
#endif


#ifdef __cplusplus
}
#endif


#endif /* CSR_DATA_STORE_TASK_H__ */
