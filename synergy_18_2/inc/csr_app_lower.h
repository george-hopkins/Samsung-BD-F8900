#ifndef CSR_APP_LOWER_H__
#define CSR_APP_LOWER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*CsrAppExitFunction)(void);

/* Call to register exit callback function. Must be called before initialising
   the CSR_APP task. The exit callback function will be called to indicate that
   the user has requested the application to exit. */
void CsrAppExitFunctionRegister(CsrAppExitFunction exitFunction);

#ifdef __cplusplus
}
#endif

#endif
