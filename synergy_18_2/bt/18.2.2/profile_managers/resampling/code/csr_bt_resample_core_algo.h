#ifndef CSR_BT_RESAMPLE_CORE_ALGO_H__
#define CSR_BT_RESAMPLE_CORE_ALGO_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* */
#include "csr_types.h"
#include "csr_pmem.h"

/* */
#include "csr_bt_resample_lib.h"
#include "csr_bt_resample_lib_priv.h"

#define FREQ_PRECISION 14

/*--------------------------------------------------------------------------*
 *  NAME
 *      ResamplingDown
 *
 *  DESCRIPTION
 *      Downsampling with a simple linare interpolation
 *
 *  PARAMETERS
 *        inst:         Pointer to instance data
 *        dataIn:       Pointer to input data
 *        blockSize     Size of the curren block
 *--------------------------------------------------------------------------*/
void ResamplingDown(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize);

/*--------------------------------------------------------------------------*
 *  NAME
 *      ResamplingUp
 *
 *  DESCRIPTION
 *      Upsampling with a simple interpolation
 *
 *  PARAMETERS
 *        inst:         Pointer to instance data
 *        dataIn:       Pointer to input data
 *        blockSize     Size of the curren block
 *--------------------------------------------------------------------------*/
void ResamplingUp(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize);


/*--------------------------------------------------------------------------*
 *  NAME
 *      ResamplingNo
 *
 *  DESCRIPTION
 *      No resampling is performed
 *
 *  PARAMETERS
 *        inst:         Pointer to instance data
 *        dataIn:       Pointer to input data
 *        blockSize     Size of the curren block
 *--------------------------------------------------------------------------*/
void ResamplingNo(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize);

#ifdef __cplusplus
}
#endif


#endif     /* RESAMPLECOREALGO */

