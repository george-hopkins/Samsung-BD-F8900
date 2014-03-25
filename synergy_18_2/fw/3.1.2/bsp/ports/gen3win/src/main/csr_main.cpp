/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include "csr_types.h"
#include "csr_main.h"
#include "csr_arg_search.h"
#include <commctrl.h>

#if 1


/* Main entry point when linked with /subsystem:windows. */
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR    lpCmdLine,
                   int       nCmdShow)                     
{
    CsrArgSearchInit((CsrUint32) __argc, (CsrCharString **) __argv);
    return (int) CsrMain();
}
#else
/* Main entry point when linked with /subsystem:console. This makes the console
   work (including stdin/stderr/stdout). */

// myproject.cpp : Defines the entry point for the console application.
//

int _tmain(int argc, char *argv[])
{
    CsrArgSearchInit((CsrUint32) __argc, (CsrCharString **) __argv);
    return (int) CsrMain();
}
#endif

