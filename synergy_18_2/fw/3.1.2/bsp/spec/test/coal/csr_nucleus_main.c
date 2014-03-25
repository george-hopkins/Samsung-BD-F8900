/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_usr_config.h"

#include "csr_types.h"
#include "csr_result.h"
#include "csr_sched.h"

#include "csr_sched_init.h"

#include "platform/csr_pmem_init.h"
#include "platform/csr_framework_ext_init.h"
#include "platform/csr_console.h"

#include "csr_sched_test.h"

CsrResult CsrMain(void)
{
    void *schedInstance;
    CsrUint16 i;

    /* Init Pmem */
    CsrPmemInit();

    /* Init Framework Extensions */
    CsrThreadInit(8 + CSR_SCHEDULER_INSTANCES);

    /* Init Console */
    ConsoleRouteSet(CONSOLE_ROUTE_DISPLAY_UART);

    init_ts_verdict();

    for (i = 0; i < CSR_SCHEDULER_INSTANCES; i++)
    {
        schedInstance = CsrSchedInit(i, 0, 0);
    }

    CsrSched(schedInstance);

    CsrThreadDeinit();

    deinit_ts_verdict();

    return 0;
}
