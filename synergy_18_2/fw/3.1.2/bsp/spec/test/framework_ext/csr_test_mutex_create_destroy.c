/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>
#include "csr_types.h"
#include "csr_framework_ext.h"

int testMutexCreateDestroy(void)
{
    CsrResult rc;
    CsrMutexHandle mutex;


    rc = CsrMutexCreate(&mutex);
    if (rc != CSR_RESULT_SUCCESS)
    {
        printf("Error on CsrMutexCreate(), rc=%d\n", rc);
        printf("Test FAILED\n");
        return 0;
    }

    CsrMutexDestroy(&mutex);

    printf("Test PASSED\n");
    return 1;
}
