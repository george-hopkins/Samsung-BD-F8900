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

/* Main entry point when linked with /subsystem:windows. */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    CsrArgSearchInit((CsrUint32) __argc, (CsrCharString **) __argv);
    return (int) CsrMain();
}

/* Main entry point when linked with /subsystem:console. This makes the console
   work (including stdin/stderr/stdout). */
int main(int argc, char *argv[])
{
    CsrArgSearchInit((CsrUint32) __argc, (CsrCharString **) __argv);
    return (int) CsrMain();
}
