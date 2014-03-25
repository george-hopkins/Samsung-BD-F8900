/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_result.h"
#include "csr_main.h"
#include "csr_testcases.h"

void testMainFwext(void)
{
    int i;
    int number_testcases = (sizeof(CsrTsFwext) / sizeof(CsrFwextTcDefs)) - 1;

    for (i = 0; i < number_testcases; i++)
    {
        printf("\n*** Running test case: %s ***\n", CsrTsFwext[i].tc_name);
        CsrTsFwext[i].tc();
    }
}

CsrResult CsrMain(void)
{
    testMainFwext();

    return 0;
}
