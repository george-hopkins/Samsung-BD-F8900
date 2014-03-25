/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "stdafx.h"
#include "csr_synergy.h"

#include <errno.h>
#ifndef _WIN32_WCE  
#include <io.h> /* _findfirst and _findnext set errno if they return -1 */
#endif
#include <stdlib.h>
#include <string.h>

#include "csr_bt_platform.h"

void system_cls(void)
{
#ifndef _WIN32_WCE
    system(CLS);
#endif
}

