/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_resample_core_algo.h"


/*
 * ResamplingUp
 */
void ResamplingUp(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize)
{
    CsrInt32     diff    = 0;
    CsrInt16     x2      = 0;
    CsrInt16     y       = 0;
    CsrInt32     n       = 0;
    CsrInt32     tmp     = 0;

    while( n < blockSize )
    {
        x2                      = dataIn[n++];  /*!!! Access to out of memory data */
        diff                    = x2-inst->filter.dataOld;
        inst->filter.d         -= inst->filter.stepIncrease;

        while( inst->filter.d < inst->filter.stepIncrease )
        {
            tmp = (CsrInt32)(diff*inst->filter.d);
            y = inst->filter.dataOld + (CsrInt16)(tmp>>FREQ_PRECISION);

            CircBufferPut(inst->filter.buf, &y);     /* Put value in buffer */

            inst->filter.d = inst->filter.d + inst->filter.step;
        }

        inst->filter.dataOld    = x2;
    }
}

/*
 * ResamplingDown
 */
void ResamplingDown(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize)
{
    CsrInt32     index   = 0;
    CsrInt16     y       = 0;

    index           = inst->filter.d >> FREQ_PRECISION;

    while( index < blockSize )
    {
        y = (CsrInt16)((inst->filter.dataOld + dataIn[index])>>1);

        CircBufferPut(inst->filter.buf, &y);     /* Put value in buffer */

        inst->filter.d += inst->filter.step; /* Update step */

        if( (index-1) < 0 )     /* Check if the first value is at index 0 */ /* NOTE that it is poissible to remove the "if" check if inputdata and oldBlockData are alinged in memory */
        {
            inst->filter.dataOld = inst->filter.oldBlockData;
        }
        else
        {
            inst->filter.dataOld = dataIn[index-1];
        }
        index = inst->filter.d >> FREQ_PRECISION;
    }

    inst->filter.oldBlockData   = dataIn[blockSize -1];
    inst->filter.d              = inst->filter.d - inst->filter.stepIncrease*blockSize;
}



/*
 * ResamplingNo
 */
void ResamplingNo(localInstanceData_t *inst, CsrInt16 *dataIn, CsrInt32 blockSize)
{
    CsrInt32 i = 0;
    CsrUint8 status = 0;

    for( i=0; i<blockSize; i++ )
    {
        status = CircBufferPut(inst->filter.buf, &dataIn[i]);
    }

    if( status == CIRC_BUFFER_ERROR )
    {
        /* Error, buffer full*/
    }
}
