/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_converters.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"

extern void CsrBtConverterInit(void);

void CsrBtAppConverterInit(void)
{
    /* Converters are handled entirely by the framework */
    CsrConverterInit();
    CsrBtConverterInit();
}
