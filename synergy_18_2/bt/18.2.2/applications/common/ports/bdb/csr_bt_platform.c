/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_unicode.h"
#include "csr_bt_platform.h"
#include "csr_formatted_io.h"
#include "csr_pmem.h"

#include <stdio.h>
#include <stdlib.h>

void system_cls(void)
{
    printf( "%c[2J", 27);
}
