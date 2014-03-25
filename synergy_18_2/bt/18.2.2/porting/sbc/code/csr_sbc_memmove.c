/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #1 $
***************************************************************************/

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_util.h"


void *CsrSbcMemMove(void *dest, const void* src, CsrSize count)
{
    return(CsrMemMove(dest,src,count));
}
