#ifndef CSR_BT_RAND_H__
#define CSR_BT_RAND_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
NAME
    random_number  -  get a random number

FUNCTION
    Places a pseudo-random number into the "outputlen" element array
    output[].
*/

void random_number(CsrUint16 output[], CsrUint16 outputlen);

#ifdef __cplusplus
}
#endif

#endif

