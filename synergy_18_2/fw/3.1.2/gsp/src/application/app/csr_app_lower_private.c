/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_app_lower.h"

CsrAppExitFunction CsrAppLowerPrivateExitFunction = NULL;

/* Call to register exit callback function. Must be called before initialising
   the CSR_APP task. The exit callback function will be called to indicate that t
   he user has requested the application to exit. */
void CsrAppExitFunctionRegister(CsrAppExitFunction exitFunction)
{
    CsrAppLowerPrivateExitFunction = exitFunction;
}
