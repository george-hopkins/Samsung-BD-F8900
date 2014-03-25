#ifndef CSR_FRAMEWORK_EXT_INIT_H__
#define CSR_FRAMEWORK_EXT_INIT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_result.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadInit
 *
 *  DESCRIPTION
 *      Initializes static thread pool with size number of threads
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadInit(CsrUint16 size);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrThreadDeinit
 *
 *  DESCRIPTION
 *      Deinit static thread pool
 *
 *  RETURNS
 *      void
 *
 *----------------------------------------------------------------------------*/
void CsrThreadDeinit(void);

#ifdef __cplusplus
}
#endif

#endif /* CSR_FRAMEWORK_EXT_INIT_H__ */
