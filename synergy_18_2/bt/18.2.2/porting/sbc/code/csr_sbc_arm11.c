/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/*
 * Just the routines and data neede both for encoding and decoding SBC streams
 * are left in this file.
 */

#include "csr_pmem.h"
#include "csr_util.h"

#include "csr_sbc_arm11.h"
#include "csr_polyphase_arm11.h"

/* Loudness bit allocation offset table, 4 subbands */
static const signed char SBC_OFFSET4[4][4] =
{
        { -1, 0, 0, 0},
        { -2, 0, 0, 1},
        { -2, 0, 0, 1},
        { -2, 0, 0, 1}
};

/* Loudness bit allocation offset table, 8 subbands */
static const signed char SBC_OFFSET8[4][8] =
{
        { -2, 0, 0, 0, 0, 0, 0, 1},
        { -3, 0, 0, 0, 0, 0, 1, 2},
        { -4, 0, 0, 0, 0, 0, 1, 2},
        { -4, 0, 0, 0, 0, 0, 1, 2}
};

void sbcCalcCRC(CsrInt16 bs, CsrUint16 word, CsrUint8 *crc)
{
    CsrInt16 tmp;

    word <<= (8 - bs);

    do
    {
        tmp = (*crc ^ word) >> 7;
        *crc <<= 1;
        if(tmp & 1)
            *crc ^= 0x1D; /* polynomial: X^8 + X^4 + X^3 + X^2 + 1 */
        word <<= 1;
    } while (--bs);
}

static void distributeBits(SbcHandle_t *hdl, CsrSbcAllocMethod method, CsrUint8 min_ch, CsrUint8 max_ch,
                           CsrUint8 nof_subbands, CsrUint8 sample_freq, CsrUint16 bitpool)
{
    CsrInt32 bitneed[CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];
    CsrUint8 sb, ch;
    CsrInt32 bitslice = 0;
    CsrInt32 bitcount = 0;
    
    /* Bit allocation */
    if (method == CSR_SBC_METHOD_SNR)
    {
        for (ch = min_ch; ch <= max_ch; ch++)
        {
            sb = nof_subbands - 1;
            do
            {
                bitneed[ch][sb] = hdl->scale_factor[ch][sb];
            }while (sb--);
        }
    }
    else
    {
        for (ch = min_ch; ch <= max_ch; ch++)
        {
            for (sb=0; sb<nof_subbands; sb++)
            {
                if (hdl->scale_factor[ch][sb] == 0)
                {
                    bitneed[ch][sb] = -5;
                }
                else
                {
                    CsrInt16 loudness;

                    if (nof_subbands == 4)
                        loudness = hdl->scale_factor[ch][sb] -SBC_OFFSET4[sample_freq][sb];
                    else
                        loudness = hdl->scale_factor[ch][sb] -SBC_OFFSET8[sample_freq][sb];

                    if (loudness > 0)
                          bitneed[ch][sb] = loudness >> 1;
                    else
                        bitneed[ch][sb] = loudness;
                }
            }
            
        }
    }

    /* The maximum bitneed index is searched */
    for (ch = min_ch; ch <= max_ch; ch++)
    {
        sb = nof_subbands - 1;
        do
        {
            if (bitneed[ch][sb] > bitslice)
                bitslice = bitneed[ch][sb];
        } while (sb--);
    }
    bitcount = 0;

    
    for(;;)
    {
        CsrInt16 slicecount = 0;

        for (ch = min_ch; ch <= max_ch; ch++)
        {
            for (sb = 0; sb < nof_subbands; sb++)
            {
                if ((bitneed[ch][sb] > bitslice+1) &&
                    (bitneed[ch][sb] < bitslice+16))
                    slicecount++;
                else if (bitneed[ch][sb] == bitslice+1)
                    slicecount+=2;
            }
        }

        if (bitcount+slicecount > bitpool)
            break;

        bitslice--;
        bitcount += slicecount;

        if (bitcount == bitpool)
            break;
    }

    
    for (ch = min_ch; ch <= max_ch; ch++)
    {
        for(sb=0; sb<nof_subbands; sb++)
        {
            if (bitneed[ch][sb] < bitslice+2)
            {
                hdl->bits[ch][sb] = 0;
            }
            else
            {
                CsrInt16 tmp = bitneed[ch][sb] - bitslice;

                hdl->bits[ch][sb] = tmp < 16 ? tmp : 16;
            }
        }
    }

    for(sb=0; sb < nof_subbands && bitcount < bitpool; sb++)
    {
        for (ch = min_ch; ch <= max_ch && bitcount < bitpool; ch++)
        {
            if (hdl->bits[ch][sb] >= 2 && hdl->bits[ch][sb] < 16)
            {
                hdl->bits[ch][sb]++;
                bitcount++;
            }
            else if (bitneed[ch][sb] == bitslice+1 &&
                     bitpool > bitcount+1)
            {
                hdl->bits[ch][sb] = 2;
                bitcount += 2;
            }
        }
    }

    for(sb=0; sb < nof_subbands && bitcount < bitpool; sb++)
    {
        for (ch = min_ch; ch <= max_ch && bitcount < bitpool; ch++)
        {
            if (hdl->bits[ch][sb] < 16)
            {
                hdl->bits[ch][sb]++;
                bitcount++;
            }
        }
    }
}

void bitAllocate(SbcHandle_t *hdl, CsrSbcAllocMethod method, CsrUint8 nof_channels,
                 CsrSbcChannelMode channel_mode, CsrUint8 nof_subbands,
                 CsrUint8 sample_freq, CsrUint16 bitpool)
{
    CsrUint8 ch;

    if (channel_mode < CSR_SBC_STEREO)
    {
        /* this is mono or dual channel */
        for (ch=0; ch<nof_channels; ch++)
            distributeBits(hdl, method, ch, ch, nof_subbands, sample_freq,
                           bitpool);
    }
    else
    {
        /* stereo or joint stereo */
        distributeBits(hdl, method, 0, 1, nof_subbands, sample_freq, bitpool);
    }
}

void calcLevels(SbcHandle_t *hdl)
{   
   CsrUint8 ch, sb;

    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        for (sb=0; sb < hdl->sbc->subbands; sb++)
        {
            hdl->levels[ch][sb] = (1<< hdl->bits[ch][sb]) - 1;
        }
    }
}


void calc_sbc_frame_len(SbcHandle_t *hdl)
{
    CsrUint8 js_factor = 0;
    CsrUint8 mono_dual_factor = 1;

    if( hdl->sbc->ch_mode < CSR_SBC_STEREO)
    {
        /* mono and dual channels mode */
        mono_dual_factor = hdl->sbc->channels;
    }
    else
    {
        /* stereo and joint stereo channels mode */
        if(hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
            js_factor = 1;
    }

    hdl->sbc->frame_len = (CsrUint16 ) (4 + hdl->sbc->subbands * hdl->sbc->channels /2 + (hdl->sbc->subbands * js_factor + hdl->sbc->blocks * mono_dual_factor * hdl->sbc->bitpool + 7)/8);
}

void *CsrSbcOpen(void)
{
    CsrUint16 i, ii;
    SbcHandle_t *hdl;

    int z;
    /*Copy the coefficients*/
    for (z=0;z<40;z++)
    {
      c_sb4_union.C_SB4_SIMD[z] = C_SB4_SIMD[z];
    }
    for (z=0;z<80;z++)
    {
      c_sb8_union.C_SB8_SIMD[z] = C_SB8_SIMD[z];
    }
    
    hdl = CsrPmemAlloc(sizeof(SbcHandle_t));
    
    hdl->sb_sample = CsrPmemAlloc(sizeof(CsrSbcSbSamples *) * CSR_SBC_MAX_BLOCKS);
    for(i = 0; i < CSR_SBC_MAX_BLOCKS; i++)
    {
        hdl->sb_sample[i] = CsrPmemAlloc(sizeof(CsrSbcSbSamples *) * CSR_SBC_MAX_CHANNELS);
        for(ii = 0; ii < CSR_SBC_MAX_CHANNELS; ii++)
        {
            hdl->sb_sample[i][ii] = CsrPmemAlloc(sizeof(CsrSbcSbSamples) * CSR_SBC_MAX_SUBBANDS);
        }
    }

    hdl->scale_factor = CsrPmemAlloc(sizeof(short *) * CSR_SBC_MAX_CHANNELS);
    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        hdl->scale_factor[i] = CsrPmemAlloc(sizeof(short) * CSR_SBC_MAX_SUBBANDS);
    }

    hdl->bits = CsrPmemAlloc(sizeof(int *) * CSR_SBC_MAX_CHANNELS);
    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        hdl->bits[i] = CsrPmemAlloc(sizeof(int *) * CSR_SBC_MAX_SUBBANDS);
    }

    hdl->levels = CsrPmemAlloc(sizeof(unsigned int *) * CSR_SBC_MAX_CHANNELS);
    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        hdl->levels[i] = CsrPmemAlloc(sizeof(unsigned int) * CSR_SBC_MAX_SUBBANDS);
    }

    hdl->sbc = CsrPmemAlloc(sizeof(*hdl->sbc));

    hdl->audio_window1 = CsrPmemAlloc(sizeof(CsrInt16 *) * CSR_SBC_MAX_CHANNELS);
    hdl->audio_window2 = CsrPmemAlloc(sizeof(CsrInt16 *) * CSR_SBC_MAX_CHANNELS);
    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        hdl->audio_window1[i] = CsrPmemAlloc(sizeof(CsrInt16) * (CSR_SBC_MAX_WINDOW/2));
        hdl->audio_window2[i] = CsrPmemAlloc(sizeof(CsrInt16) * (CSR_SBC_MAX_WINDOW/2));
    }

    hdl->v = CsrPmemAlloc(sizeof(CsrSbcFixptVector *) * CSR_SBC_MAX_CHANNELS);
    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        hdl->v[i] = CsrPmemAlloc(sizeof(CsrSbcFixptVector) * 2*CSR_SBC_MAX_WINDOW);
    }

    return hdl;
}

void CsrSbcClose(void **gash)
{
    CsrUint16 i, ii;
    SbcHandle_t *hdl = (SbcHandle_t *) *gash;

    for(i = 0; i < CSR_SBC_MAX_BLOCKS; i++)
    {
        for(ii = 0; ii < CSR_SBC_MAX_CHANNELS; ii++)
        {
            CsrPmemFree(hdl->sb_sample[i][ii]);
        }
        CsrPmemFree(hdl->sb_sample[i]);
    }
    CsrPmemFree(hdl->sb_sample);

    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        CsrPmemFree(hdl->scale_factor[i]);
    }
    CsrPmemFree(hdl->scale_factor);

    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        CsrPmemFree(hdl->bits[i]);
    }
    CsrPmemFree(hdl->bits);

    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        CsrPmemFree(hdl->levels[i]);
    }
    CsrPmemFree(hdl->levels);

    CsrPmemFree(hdl->sbc);

    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        CsrPmemFree(hdl->audio_window1[i]);
        CsrPmemFree(hdl->audio_window2[i]);
    }
    CsrPmemFree(hdl->audio_window1);
    CsrPmemFree(hdl->audio_window2);

    for(i = 0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        CsrPmemFree(hdl->v[i]);
    }
    CsrPmemFree(hdl->v);
    CsrPmemFree(hdl);

    *gash = NULL;
 
    return;
}

CsrSbcChannelMode CsrSbcGetChannelMode(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->ch_mode;
}

CsrSbcAllocMethod CsrSbcGetAllocMethod(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->alloc_method;
}

CsrUint16 CsrSbcGetSampleFreq(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->sample_freq;
}

CsrUint8 CsrSbcGetNumBlocks(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->blocks;
}

CsrUint8 CsrSbcGetNumSubBands(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->subbands;
}

CsrUint8 CsrSbcGetBitPool(void *hdl)
{
    return ((SbcHandle_t *)hdl)->sbc->bitpool;
}

