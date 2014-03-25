#ifndef CSR_BT_LOWPASS_FILTER_H__
#define CSR_BT_LOWPASS_FILTER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


#include "csr_types.h"
#include "csr_pmem.h"

/* Filter instance data */
typedef struct
{
    CsrUint16    lengthData;
    CsrUint16    lengthF;      /* Length of filter */
    CsrInt16     *c;     /* Point to filter coeff */
    CsrInt16     d[15];
}filter_t;

/*--------------------------------------------------------------------------*
 *  NAME
 *      filterInit
 *
 *  DESCRIPTION
 *
 *  PARAMETERS
 *
 *--------------------------------------------------------------------------*/
filter_t* filterInit(CsrInt32 fsIn, CsrInt32 fsOut, CsrUint16 lengthData);

/*--------------------------------------------------------------------------*
 *  NAME
 *      filterDeInit
 *
 *  DESCRIPTION
 *      Free allocated memory
 *
 *  PARAMETERS
 *      inst:       Instance data
 *--------------------------------------------------------------------------*/
void filterDeInit(filter_t *instanceData);

/*--------------------------------------------------------------------------*
 *  NAME
 *      filter
 *
 *  DESCRIPTION
 *
 *  RETURNS
 *--------------------------------------------------------------------------*/
void filterSimpleFir(filter_t *instanceData, CsrInt16 *data);

#ifdef __cplusplus
}
#endif


#endif     /* CIRCULARBUFFER */

