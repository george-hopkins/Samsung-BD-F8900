/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_result.h"

#include "platform/csr_pmem_init.h"
#include "platform/csr_framework_ext_init.h"
#include "platform/csr_console.h"

#include "csr_testcases.h"


CsrResult CsrMain(void)
{
    int i;
    int number_testcases = (sizeof(CsrTsFwext) / sizeof(CsrFwextTcDefs)) - 1;

    /* Init Pmem */
    CsrPmemInit();

    /* Init Framework Extensions */
    CsrThreadInit(8);

    /* Init Console */
    ConsoleRouteSet(CONSOLE_ROUTE_UART);

    /* Run testcases */
    for (i = 0; i < number_testcases; i++)
    {
        printf("\n*** Running test case: %s ***\n", CsrTsFwext[i].tc_name);
        CsrTsFwext[i].tc();
    }

    /* Clean up */
    CsrThreadDeinit();

    return 0;
}
