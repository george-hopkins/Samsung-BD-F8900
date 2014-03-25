/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_resample_lib.h"
#include "csr_bt_resample_lib_priv.h"
#include "csr_bt_resample_core_algo.h"

/*
 * Resampling Stereo
 */
void CsrBtResamplingStereo(instanceData_t *inst, CsrInt16 *dataChan0, CsrInt16 *dataChan1, CsrInt32 blockSize)
{
    CsrInt32 i   = 0;
    CsrInt32 tmp = 0;

    /* If both input and output is Stereo */
    if( inst->c[0]->file[OUTPUT].chan == RESAMPLING_STEREO )
    {
        inst->c[0]->Resampling(inst->c[0], dataChan0, blockSize);
        inst->c[1]->Resampling(inst->c[1], dataChan1, blockSize);
    }
    else /* A simple Stereo to Mono converter by just adding the two values and divide by 2, to avoid overflow. */
    {    /* It is assumed that the corss-correlation ( rxy = 1 ) between the to channels equals 1. */
         /* If rxy = 0 the divider factor should be sqrt(2) to get at proper power scale, but in voice/music signals */
         /* this is not the case. The assumption of rxy = 1 result in a worst case scale factor. */

        for( i=0; i<blockSize; i++ )
        {
            tmp = dataChan0[i] + dataChan1[i];
            dataChan0[i] = (CsrInt16)(tmp>>1);   /* Divide    by 2 */
        }
        inst->c[0]->Resampling(inst->c[0], dataChan0, blockSize);
    }
}


void CsrBtResamplingMono(instanceData_t *inst, CsrInt16 *dataChan0, CsrInt32 blockSize)
{
    inst->c[0]->Resampling(inst->c[0], dataChan0, blockSize);
}

/*
 * RresamplingCfg
 */
CsrBtResamplingHandler CsrBtResamplingInit(CsrInt32 fs0, CsrUint8 chan0, CsrUint32 blockSize0, CsrInt32 fs1, CsrUint8 chan1, CsrUint32 blockSize1, CsrUint32 bufferSize)
{
    CsrUint32 tmp    = 0;
    CsrUint8 i       = 0;
    instanceData_t *inst = NULL;
    inst = CsrPmemAlloc(sizeof(instanceData_t));


    inst->cnt = 0;

    for( i=0; i<((chan0&chan1)+1); i++ )
    {
        inst->cnt += 1;
        inst->c[i] = CsrPmemAlloc(sizeof(localInstanceData_t));
        inst->c[i]->filter.d              = 1<<FREQ_PRECISION;
        inst->c[i]->filter.dataOld        = 0;
        inst->c[i]->filter.oldBlockData   = 0;

        /* Config resample parameters */
        inst->c[i]->file[INPUT].fs         = fs0;
        inst->c[i]->file[INPUT].chan       = chan0;
        inst->c[i]->file[INPUT].blockSize  = blockSize0;

        inst->c[i]->file[OUTPUT].fs        = fs1;
        inst->c[i]->file[OUTPUT].chan      = chan1;
        inst->c[i]->file[OUTPUT].blockSize = blockSize1;

        /* Find buffer size */
        if( bufferSize == 0 )
        {
            if( inst->c[i]->file[INPUT].blockSize > inst->c[i]->file[OUTPUT].blockSize )
            {
                tmp = inst->c[i]->file[INPUT].blockSize*8;
            }
            else
            {
                tmp = inst->c[i]->file[OUTPUT].blockSize*8;
            }
        }
        else
        {
            tmp = bufferSize;
        }

        /* Init circular buffer */
        inst->c[i]->filter.buf = CircBufferInit(1, inst->c[i]->file[OUTPUT].blockSize, tmp);


        /* Filter */
        if( inst->c[i]->file[INPUT].fs < inst->c[i]->file[OUTPUT].fs )
        {
            inst->c[i]->filter.stepIncrease   = 1<<FREQ_PRECISION;
            inst->c[i]->filter.step           = (inst->c[i]->file[INPUT].fs*inst->c[i]->filter.stepIncrease)/inst->c[i]->file[OUTPUT].fs;
        }
        else
        {
            inst->c[i]->filter.stepIncrease    = 1<<FREQ_PRECISION;  /* 2^(FREQ_PRECISION) */                  /* Must be moved to init */
            inst->c[i]->filter.step            = (inst->c[i]->file[INPUT].fs*inst->c[i]->filter.stepIncrease)/inst->c[i]->file[OUTPUT].fs;    /* Must be moved to init */
        }


        /* Initialize function pointers */
        if( inst->c[i]->file[INPUT].fs > inst->c[i]->file[OUTPUT].fs )
        {
            inst->c[i]->Resampling = ResamplingDown;
        }
        else if( inst->c[i]->file[INPUT].fs < inst->c[i]->file[OUTPUT].fs )
        {
            inst->c[i]->Resampling = ResamplingUp;
        }
        else
        {
            inst->c[i]->Resampling = ResamplingNo;
        }
    }
    return inst;
}


/*
 * CsrBtResamplingDeInit
 */
void CsrBtResamplingDeInit(instanceData_t *inst)
{
    CsrUint8 i =0;


    for( i=0; i<inst->cnt; i++ )
    {
        /* Free buffer data */
        CircBufferDeInit(inst->c[i]->filter.buf);

        /* Free buffer data */
        CsrPmemFree(inst->c[i]);
    }

    /* Free instance data */
    CsrPmemFree(inst);

    /* Instance data freed */
}

/*
 * CsrBtResamplingGetDataMono
 */
CsrUint8 CsrBtResamplingGetDataMono(instanceData_t *inst, CsrInt16 *dataChan0)
{
    CsrUint8 tmp = 0;

    /* Get data if any! */
    tmp = CircBufferGet(inst->c[0]->filter.buf, dataChan0);

    if( tmp == CIRC_BUFFER_DATA )
    {
       return 1;
    }
    else
    {
        return 0;
    }
}


/*
 * CsrBtResamplingGetDataStereo
 */
CsrUint8 CsrBtResamplingGetDataStereo(instanceData_t *inst, CsrInt16 *dataChan0, CsrInt16 *dataChan1)
{
    CsrUint8 tmp = 0;
    CsrInt32 i   = 0;

    /* Get data if any! */
    tmp = CircBufferGet(inst->c[RESAMPLING_MONO]->filter.buf, dataChan0);

    if( tmp == CIRC_BUFFER_DATA )
    {
        /* If both input and output is stereo */
        if( (inst->c[0]->file[INPUT].chan == RESAMPLING_STEREO) && (inst->c[0]->file[OUTPUT].chan == RESAMPLING_STEREO))
        {
            tmp = CircBufferGet(inst->c[RESAMPLING_STEREO]->filter.buf, dataChan1);
        }
        else
        {
            for( i=0; i<inst->c[0]->file[OUTPUT].blockSize; i++ )
            {
                dataChan1[i] = dataChan0[i];
            }
        }

        return 1;
    }
    else
    {
        return 0;
    }
}
