/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_sbc.h"
#include "csr_sbc_memmove.h"

#ifdef ENABLE_FLOAT_PRECISION
#define SCALE_FACTOR(a) (((double)a))
#define QUANTIZE_CORRECTION 1
#define FIX_CORRECTION 0
#else
#ifndef ENABLE_32BIT_PRECISION
#define FIX_CORRECTION 13
#define SCALE_FACTOR(a) (a << FIX_CORRECTION)
#define QUANTIZE_CORRECTION 1
#else
#define FIX_CORRECTION 12
#define SCALE_FACTOR(a) (a << FIX_CORRECTION)
#define QUANTIZE_CORRECTION 2
#endif
#endif

static void init_putbits(SbcHandle_t *hdl)
{
    hdl->sbc->partial = 0;
    hdl->sbc->partial_posn = 32;
}

static void flush_putbits(SbcHandle_t *hdl)
{
    while(hdl->sbc->partial_posn < 32)
    {
        *hdl->sbc->frame++ = ((CsrUint8) ((hdl->sbc->partial >> 24) & 0xff));
        hdl->sbc->partial <<= 8;
        hdl->sbc->partial_posn += 8;
    }
    init_putbits(hdl);
}

/* Values of 16 or fewer bits only */
static void putbits(SbcHandle_t *hdl, const CsrUint16 n, const CsrUint16 x)
{
    hdl->sbc->partial_posn = (CsrUint16) (hdl->sbc->partial_posn - n);
    hdl->sbc->partial |= x << hdl->sbc->partial_posn;
    if (hdl->sbc->partial_posn <= 16)
    {
        *hdl->sbc->frame++ = ((CsrUint8) ((hdl->sbc->partial>>24) & 0xff));
        *hdl->sbc->frame++ = ((CsrUint8) ((hdl->sbc->partial>>16) & 0xff));
        hdl->sbc->partial <<= 16;
        hdl->sbc->partial_posn += 16;
    }
}

/* Values of 8 or fewer bits only */
static void putbits_with_crc(SbcHandle_t *hdl, const CsrUint16 n, const CsrUint16 x,
                             CsrUint8 *crc)
{
    putbits(hdl, n, x);
    sbcCalcCRC(n, x, crc);
}

static CsrUint8 getScaleFactor(CsrSbcSbSamples sb_val)
{
    if (sb_val < 0)
    {
        sb_val = -sb_val;
    }
    if (sb_val > SCALE_FACTOR(32768))
        return 15;
    else if(sb_val > SCALE_FACTOR(16384))
        return 14;
    else if(sb_val > SCALE_FACTOR(8192))
        return 13;
    else if(sb_val > SCALE_FACTOR(4096))
        return 12;
    else if(sb_val > SCALE_FACTOR(2048))
        return 11;
    else if(sb_val > SCALE_FACTOR(1024))
        return 10;
    else if(sb_val > SCALE_FACTOR(512))
        return 9;
    else if(sb_val > SCALE_FACTOR(256))
        return 8;
    else if(sb_val > SCALE_FACTOR(128))
        return 7;
    else if(sb_val > SCALE_FACTOR(64))
        return 6;
    else if(sb_val > SCALE_FACTOR(32))
        return 5;
    else if(sb_val > SCALE_FACTOR(16))
        return 4;
    else if(sb_val > SCALE_FACTOR(8))
        return 3;
    else if(sb_val > SCALE_FACTOR(4))
        return 2;
    else if(sb_val > SCALE_FACTOR(2))
        return 1;
    else
        return 0;
}

static void calcScaleFactors1(
    SbcHandle_t *hdl,
    CsrUint8 num_chans, 
    CsrUint8 num_subbands,
    CsrSbcSbSamples samps[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS],
    CsrInt16 facts[CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS]
                              )
{
    CsrUint8 ch, sb, b;

    for (ch=0; ch<num_chans; ch++)
    {
        for (sb=0; sb<num_subbands; sb++)
        {
            CsrSbcSbSamples maxSamp = 0;

            for (b=0; b < hdl->sbc->blocks; b++)
            {
                CsrSbcSbSamples samp = samps[b][ch][sb];
                if (samp < 0)
                    samp = -samp;
                if (samp > maxSamp)
                    maxSamp = samp;
            }

            facts[ch][sb] = getScaleFactor(maxSamp);
        }
    }
}

static void calcScaleFactors2(
    SbcHandle_t *hdl,    
    CsrUint8 num_chans, CsrUint8 num_subbands,
    CsrSbcSbSamples ***samps,
    CsrInt16 **facts
)
{
    CsrUint8 ch, sb, b;

    for (ch=0; ch<num_chans; ch++)
    {
        for (sb=0; sb<num_subbands; sb++)
        {
            CsrSbcSbSamples maxSamp = 0;

            for (b=0; b < hdl->sbc->blocks; b++)
            {
                CsrSbcSbSamples samp = samps[b][ch][sb];
                if (samp < 0)
                    samp = -samp;
                if (samp > maxSamp)
                    maxSamp = samp;
            }

            facts[ch][sb] = getScaleFactor(maxSamp);
        }
    }
}

static void jointEncode(SbcHandle_t *hdl)
{
    CsrUint8 ch, sb, b;
    CsrSbcSbSamples sb_sample_js[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];
    CsrInt16 scale_factor_js[CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];

    /*
     * The spec says that the last subband is never joint encoded which is why
     * these loops run from 0..sbc.subbands-2 and why we or in 2 instead of 1
     * (we've got one fewer loop iterations than we expect so we have one
     * shift too few).
     */

    for (b=0; b < hdl->sbc->blocks; b++)
    {
        for (sb=0; sb < hdl->sbc->subbands-1; sb++)
        {
            /*
             * In joint encoding, channel 0 is the average of the two streams
             * and channel 1 is the difference between the average and the
             * original second channel.
             */
            sb_sample_js[b][0][sb] =
#ifdef ENABLE_FLOAT_PRECISION
                (hdl->sb_sample[b][0][sb] + hdl->sb_sample[b][1][sb] + 1) / 2;
#else
                (hdl->sb_sample[b][0][sb] + hdl->sb_sample[b][1][sb] + 1) >>1;
#endif
            sb_sample_js[b][1][sb] =
                sb_sample_js[b][0][sb] - hdl->sb_sample[b][1][sb];
        }
    }

    calcScaleFactors1(hdl, 2, (CsrUint8)(hdl->sbc->subbands-1), sb_sample_js, scale_factor_js);

    hdl->sbc->joint = 0;
    for (sb=0; sb < hdl->sbc->subbands-1; sb++)
    {
        hdl->sbc->joint <<= 1;

        if (hdl->scale_factor[0][sb] + hdl->scale_factor[1][sb] >
            scale_factor_js[0][sb] + scale_factor_js[1][sb])
        {
            /*
             * This subband must use joint stereo encoding so mark the
             * appropriate bit and copy the joint stereo data over the
             * original data.
             */
            hdl->sbc->joint |= 2;

            for (ch=0; ch<2; ch++)
                for (b=0; b < hdl->sbc->blocks; b++)
                    hdl->sb_sample[b][ch][sb] = sb_sample_js[b][ch][sb];

            hdl->scale_factor[0][sb] = scale_factor_js[0][sb];
            hdl->scale_factor[1][sb] = scale_factor_js[1][sb];

        }
    }
}

static CsrUint16 build_sbc_frame(SbcHandle_t *hdl)
{
    CsrUint16 x[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];
    CsrUint8 ch, sb, b;
    CsrUint8 *frame_crc = hdl->sbc->frame+3;
    CsrUint8 *frame_start = hdl->sbc->frame;

    /* init crc and bit position */
    *frame_crc = SBC_CRC_INIT;
    init_putbits(hdl);

    /* Build up the frame header */
    putbits(hdl, 8, SBC_SYNC_WORD);
    putbits_with_crc(hdl, 2, hdl->sbc->sample_freq, frame_crc);
    putbits_with_crc(hdl, 2, (CsrUint16)((hdl->sbc->blocks - 4)>>2), frame_crc);
    putbits_with_crc(hdl, 2, hdl->sbc->ch_mode, frame_crc);
    putbits_with_crc(hdl, 1, hdl->sbc->alloc_method, frame_crc);
    putbits_with_crc(hdl, 1, (CsrUint16)(hdl->sbc->subbands == 8 ? 1 : 0), frame_crc);
    putbits_with_crc(hdl, 8, hdl->sbc->bitpool, frame_crc);
    flush_putbits(hdl);

    hdl->sbc->frame++; /* skip over CRC */

    /* add joint stereo information if used */
    if (hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
        putbits_with_crc(hdl, hdl->sbc->subbands, hdl->sbc->joint, frame_crc);

    /* add scale factors */
    for (ch=0; ch < hdl->sbc->channels; ch++)
        for (sb=0; sb < hdl->sbc->subbands; sb++)
            putbits_with_crc(hdl, 4, hdl->scale_factor[ch][sb], frame_crc);

    /* add samples */
    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        for (sb=0; sb < hdl->sbc->subbands; sb++)
        {
            if (hdl->bits[ch][sb] != 0)
            {
                const int sf = hdl->scale_factor[ch][sb];
                const unsigned int lvs = hdl->levels[ch][sb];
                const CsrSbcFixpt32 pow_sf =(1<<(sf + 1 + FIX_CORRECTION));

                /* quantize */
                for (b=0; b < hdl->sbc->blocks; b++)
                {
#ifdef ENABLE_FLOAT_PRECISION
                    x[b][ch][sb] = (CsrUint16) (((hdl->sb_sample[b][ch][sb]) + (pow_sf))* lvs/(2.0*pow_sf));
#else
                    CsrSbcFixpt32 tmp32 = ((hdl->sb_sample[b][ch][sb]) + (pow_sf)) << QUANTIZE_CORRECTION;
                    tmp32 =  Multiply32by16( tmp32, lvs);
                    x[b][ch][sb] = (CsrUint16) (tmp32 >> sf);
#endif
                }
            }


        }
    }

    for (b=0; b < hdl->sbc->blocks; b++)
        for (ch=0; ch < hdl->sbc->channels; ch++)
            for (sb=0; sb < hdl->sbc->subbands; sb++)
            {
                const CsrUint16 n = hdl->bits[ch][sb];
                if (n != 0)
                    putbits(hdl, n, x[b][ch][sb]);
            }

    /* do padding */
    flush_putbits(hdl);

    return (CsrUint16) (hdl->sbc->frame - frame_start);
}


/*
 * External SBC functions
 *
 * These are exposed via csr_sbc_api.h.
 */

CsrUint8 CsrSbcCalcBitPool(CsrUint8 *bitPoolAlt, CsrUint8 *togglePeriod,
                      CsrSbcChannelMode channel_mode, CsrUint16 sample_freq,
                      CsrUint8 nof_blocks, CsrUint8 nof_subbands, CsrUint16 bitrate)
{
    CsrUint16 bitpool = 0;
    CsrInt32 tmp;
    CsrInt32 rem;
    CsrUint8 dual_factor = 1;
    CsrUint8 mono_factor = 0;
    CsrUint8 js_factor = 0;
    CsrInt32 divisor;
    CsrInt32 tmp_low;

    /*
     * NOTE:  This function may return invalid bitpool values at least for
     * mono streams which per spec only supports bitrates up to 266kbps for
     * encoding.
     */

    *togglePeriod = 0;

    sample_freq /= 1000;

    /* derive the bitpool size from the wanted bitrate and SBC settings */
    switch(channel_mode)
    {
    case CSR_SBC_STEREO:
        break;
    case CSR_SBC_JOINT_STEREO:
        js_factor = nof_subbands;
        break;
    case CSR_SBC_MONO:
        mono_factor = 1;
        break;
    case  CSR_SBC_DUAL:
        dual_factor = 2;
        break;
    default:
        break;
    }

    /*
     * If the bitpool needed to give a more accurate bit rate (closer to user
     * required) is not too close to a whole number, then set the toggle
     * period equal to 2 and specify the alternate bitpool against which the
     * main bitpool will oscillate
     *
     * The algorithm below can be made more complex to give an even more
     * accurate bit rate. This can be done by checking what level bitpool is
     * between two whole numbers and setting the toggle rate to produce the
     * appropriate ideal (average) bitpool.
     */

    tmp = ((bitrate * nof_subbands * nof_blocks / sample_freq)  - (8>>mono_factor) * nof_subbands - 32);

    divisor = dual_factor * nof_blocks;
    bitpool = (CsrUint16)((tmp - js_factor) / divisor);

    tmp_low = ((js_factor + divisor * bitpool + 7) >> 3);

    if(tmp == (tmp_low << 3))
    {
        *bitPoolAlt = (CsrUint8) bitpool;
        return (CsrUint8) bitpool;
    }
    else
    {
        rem = (tmp - js_factor) % divisor;

        *bitPoolAlt = (CsrUint8) bitpool;

        if(rem != 0 && rem > divisor/4)
        {
            if (rem*4 > divisor*3) {
                bitpool += 1;
            } else {
                *togglePeriod = 2;
                *bitPoolAlt = (CsrUint8) (bitpool+1);
            }
        }

    }
    return (CsrUint8) bitpool;
}

CsrUint16 CsrSbcConfig(void *inst,
                  CsrSbcChannelMode channel_mode,
                  CsrSbcAllocMethod alloc_method,
                  CsrUint16 sample_freq,
                  CsrUint8 nof_blocks,
                  CsrUint8 nof_subbands,
                  CsrUint8 bitpool)
{
    CsrUint16 i, ii;
    SbcHandle_t *hdl = inst;

    /* check that we have got valid settings */
    if ((nof_subbands != 8 && nof_subbands != 4) ||
        (nof_blocks > 16 || (nof_blocks & 3) != 0) ||
        bitpool<2)
        return 0;

    switch(sample_freq)
    {
    case 16000:
        hdl->sbc->sample_freq = sample_freq_16k;
        break;
    case 32000:
        hdl->sbc->sample_freq = sample_freq_32k;
        break;
    case 44100:
        hdl->sbc->sample_freq = sample_freq_44_1k;
        break;
    case 48000:
        hdl->sbc->sample_freq = sample_freq_48k;
        break;
    default:
        return 0;
    }

    if (channel_mode < CSR_SBC_STEREO)
    {
        if (bitpool > (nof_subbands<<4))
            return 0;
    }
    else
    {
        if (bitpool > (nof_subbands<<5))
            return 0;
    }

    /* Settings seem OK, configure values */
    hdl->sbc->ch_mode = channel_mode;
    if (channel_mode == CSR_SBC_MONO)
        hdl->sbc->channels = 1;
    else
        hdl->sbc->channels = 2;

    hdl->sbc->blocks = nof_blocks;
    hdl->sbc->subbands = nof_subbands;
    hdl->sbc->alloc_method = alloc_method;
    hdl->sbc->bitpool = bitpool;

    for(i=0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        for(ii=0; ii < CSR_SBC_MAX_WINDOW; ii++)
        {
            hdl->audio_window[i][ii] = 0;
        }
    }

    calc_sbc_frame_len(hdl);

    return hdl->sbc->frame_len;
}

CsrUint16 CsrSbcEncode(void *inst, const CsrInt16 audio_samples[][2],
                  CsrUint8 *frame, CsrUint8 bit_pool)
{
    SbcHandle_t *hdl = inst;
    CsrUint8 ch, blk, i;
    CsrInt16 *ptrDest;
    const CsrInt16 *ptrSrc;
    
    hdl->sbc->bitpool = bit_pool;

    for (blk=0; blk < hdl->sbc->blocks; blk++)
    {
        for (ch=0; ch < hdl->sbc->channels; ch++)
        {
            /* advance the sample window x */
            CsrSbcMemMove(&hdl->audio_window[ch][hdl->sbc->subbands], hdl->audio_window[ch],
                    (CSR_SBC_BLOCKS_IN_WINDOW-1) * hdl->sbc->subbands * sizeof(CsrInt16));

            /*  input next audio samples */
            ptrDest = hdl->audio_window[ch];

            if(hdl->sbc->channels == 2)
                ptrSrc = &audio_samples[hdl->sbc->subbands-1][ch];
            else
                ptrSrc = &audio_samples[(hdl->sbc->subbands>>1)-1][1];

            for(i = hdl->sbc->subbands; i>0; i--, ptrSrc -= hdl->sbc->channels)
                *ptrDest++ = *ptrSrc;

        }

        /* apply polyphase filter */
        if(hdl->sbc->subbands == 4)
        {
            polyPhaseAnalysisSB4(hdl, hdl->sbc->channels, blk, hdl->audio_window);
        }
        else
        {
            polyPhaseAnalysisSB8(hdl, hdl->sbc->channels, blk, hdl->audio_window);
        }

        audio_samples += (hdl->sbc->subbands * hdl->sbc->channels >> 1);
    }
    
    /* calculate scale factors per subband*/
    calcScaleFactors2(hdl, hdl->sbc->channels, hdl->sbc->subbands, hdl->sb_sample, hdl->scale_factor);

    if(hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
        jointEncode(hdl);

    bitAllocate(hdl, hdl->sbc->alloc_method, hdl->sbc->channels, hdl->sbc->ch_mode, hdl->sbc->subbands,
                hdl->sbc->sample_freq, hdl->sbc->bitpool);

    calcLevels(hdl);

    /* build frame*/
    hdl->sbc->frame = frame; /* set the global frame pointer*/

    return build_sbc_frame(hdl);
}
