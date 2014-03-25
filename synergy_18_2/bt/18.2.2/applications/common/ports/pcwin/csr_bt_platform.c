/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <errno.h>
#include <io.h> /* _findfirst and _findnext set errno if they return -1 */
#include <stdlib.h>
#include <string.h>

#include "csr_bt_platform.h"

void system_cls(void)
{
    system(CLS);
}
