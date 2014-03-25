/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_sbc_arm11.h"

#define LEV_MULT       2
#define LEV_OFFSET     0.5000
#define LEV_OFFSET_FX  ((CsrSbcFixpt16) (LEV_OFFSET*32768))

static const CsrSbcCoefficientSize LEVEL_RECIP[15] =
{
        DBL2FIX(LEV_MULT*2.0/3 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*4.0/7 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*8.0/15 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*16.0/31 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*32.0/63 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*64.0/127 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*128.0/255 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*256.0/511 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*512.0/1023 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*1024.0/2047 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*2048.0/4095 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*4096.0/8191 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*8192.0/16383 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*16384.0/32767 - LEV_OFFSET),
        DBL2FIX(LEV_MULT*32768.0/65535 - LEV_OFFSET)
};

static void init_getbits(SbcHandle_t *hdl)
{
    hdl->sbc->partial = 0;
    hdl->sbc->partial_posn = 0;
}

/* Values of 16 or fewer bits only */
static CsrUint16 getbits(SbcHandle_t *hdl, CsrUint16 n)
{
    CsrUint16 x;

    while(hdl->sbc->partial_posn < n)
    {
        hdl->sbc->partial |= (*hdl->sbc->frame++) << (24 - hdl->sbc->partial_posn);
        hdl->sbc->partial_posn += 8;
    }

    x = (CsrUint16) (hdl->sbc->partial >> (32 - n)) & ((1<<n) - 1);
    hdl->sbc->partial_posn = (CsrUint16) (hdl->sbc->partial_posn - n);
    hdl->sbc->partial <<= n;

    return x;
}

static void skipbits(SbcHandle_t *hdl, CsrUint16 n)
{
    if (n >= hdl->sbc->partial_posn)
    {
        n -= hdl->sbc->partial_posn;
        hdl->sbc->partial_posn = 0;
        hdl->sbc->frame += n / 8;
        n %= 8;
    }
    (void) getbits(hdl, n);
}

/* Values of 8 or fewer bits only */
static CsrUint8 getbits_with_crc(SbcHandle_t *hdl, const CsrUint16 n, CsrUint8 *crc)
{
    const CsrUint8 x = (CsrUint8) getbits(hdl, n);
    sbcCalcCRC(n, x, crc);

    return x;
}

static void read_sbc_audio(SbcHandle_t *hdl)
{
    CsrUint8 b, sb, ch;
    CsrUint16 x[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];

    /* calculate where the bitpool starts */
    init_getbits(hdl);

    skipbits(hdl, (CsrUint16)(32 + hdl->sbc->channels * hdl->sbc->subbands * 4));

    if (hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
        (void) getbits(hdl, hdl->sbc->subbands);

    for (b=0; b < hdl->sbc->blocks; b++)
        for (ch=0; ch < hdl->sbc->channels; ch++)
            for (sb=0; sb < hdl->sbc->subbands; sb++)
                x[b][ch][sb] = (CsrUint16) getbits(hdl, hdl->bits[ch][sb]);

    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        for (sb=0; sb < hdl->sbc->subbands; sb++)
        {
            const CsrInt16 n = hdl->bits[ch][sb];

            if (n == 0 || hdl->levels[ch][sb] == 0)
            {
                for (b=0; b < hdl->sbc->blocks; b++)
                    hdl->sb_sample[b][ch][sb] = 0;
            }
            else
            {
                const CsrUint16 levelRecip = LEVEL_RECIP[n-2] + LEV_OFFSET_FX;
                const CsrUint16 sf = hdl->scale_factor[ch][sb];

                for (b=0; b < hdl->sbc->blocks; b++)
                {
                    /* dequantize */
                    /* This is the first place scaling of the received sbc sound samples is made */
                    CsrSbcFixpt32 tmp32 = ((CsrSbcFixpt32) x[b][ch][sb] * levelRecip) + (levelRecip >> 1);

                    if(tmp32 < 0)
                        tmp32 = 0x7FFFFFFF;

                    if(n >= hdl->scale_factor[ch][sb])
                        hdl->sb_sample[b][ch][sb] = ((((tmp32) >> ( n -sf )) - ( 1 << (sf+2+12) ))+1)>>1;
                    else
                        hdl->sb_sample[b][ch][sb] = ((((tmp32) << ( sf - n )) - ( 1 << (sf+2+12) ))+1)>>1;
                }
            }
        }
    }
}

void CsrSbcInitDecoder(void *inst)
{
    CsrUint16 i, ii;
    SbcHandle_t *hdl = inst;

    for(i=0; i < CSR_SBC_MAX_CHANNELS; i++)
    {
        for(ii=0; ii < 2*CSR_SBC_MAX_WINDOW; ii++)
        {
            hdl->v[i][ii] = 0;
        }
    }
}

/* External SBC functions */
CsrUint16 CsrSbcReadHeader(void *inst, CsrUint8 *frame)
{
    CsrUint8 ch, sb;
    CsrUint8 crc, frame_crc;
    SbcHandle_t *hdl = inst;

    hdl->sbc->frame = frame;

    /* init crc check value */
    init_getbits(hdl);
    crc = SBC_CRC_INIT;

    /* check syncword */
    if(getbits(hdl, 8) != SBC_SYNC_WORD)
    {
        return 0;
    }

    hdl->sbc->sample_freq = (CsrUint8) getbits_with_crc(hdl, 2, &crc);
    hdl->sbc->blocks = (CsrUint8) (getbits_with_crc(hdl, 2, &crc) * 4 + 4);
    hdl->sbc->ch_mode = (CsrSbcChannelMode) getbits_with_crc(hdl, 2, &crc);
    hdl->sbc->alloc_method = (CsrSbcAllocMethod) getbits_with_crc(hdl, 1, &crc);
    hdl->sbc->subbands = (CsrUint8) (getbits_with_crc(hdl, 1, &crc) ? 8 : 4);
    hdl->sbc->bitpool = getbits_with_crc(hdl, 8, &crc);

    frame_crc = (CsrUint8) getbits(hdl, 8);

    if (hdl->sbc->ch_mode == CSR_SBC_MONO)
        hdl->sbc->channels = 1;
    else
        hdl->sbc->channels = 2;

    /* extract joint stereo information if included */
    if (hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
    {
        hdl->sbc->joint = getbits_with_crc(hdl, hdl->sbc->subbands, &crc);
    }

    /* extract scale factors */
    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        for (sb=0; sb < hdl->sbc->subbands; sb++)
        {
            hdl->scale_factor[ch][sb] = getbits_with_crc(hdl, 4, &crc);
        }
    }

    if (crc != frame_crc)
    {
        return 0;
    }

    calc_sbc_frame_len(hdl);

    return hdl->sbc->frame_len;
}

static void jointDecode(SbcHandle_t *hdl)
{
    CsrUint8 b, sb;

    for (b=0; b < hdl->sbc->blocks; b++)
    {
        CsrUint8 j = hdl->sbc->joint;

        for (sb = hdl->sbc->subbands; sb-- ; j >>= 1)
        {
            if (j & 1)
            {
                /*
                 * In joint mode, channel 0 is the sum of the samples for the
                 * two streams and channel 1 is the difference.
 */
                CsrSbcFixpt32 s0 = hdl->sb_sample[b][0][sb], s1 = hdl->sb_sample[b][1][sb];
                hdl->sb_sample[b][0][sb] = s0 + s1;
                hdl->sb_sample[b][1][sb] = s0 - s1;
            }
        }
    }
}

void CsrSbcDecode(
    void *inst,
    CsrUint8 *frame,
    CsrInt16 audio_samples[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS])
{
    CsrUint8 block_count;
    SbcHandle_t *hdl = inst;

    bitAllocate(hdl, hdl->sbc->alloc_method, hdl->sbc->channels, hdl->sbc->ch_mode, hdl->sbc->subbands,
                hdl->sbc->sample_freq, hdl->sbc->bitpool);

    calcLevels(hdl);

    hdl->sbc->frame = frame;
    read_sbc_audio(hdl);

    if (hdl->sbc->ch_mode == CSR_SBC_JOINT_STEREO)
        jointDecode(hdl);

    if (hdl->sbc->subbands == 4)
    {
        for(block_count = 0; block_count < hdl->sbc->blocks; block_count++)
            polyPhaseSynthesisSB4(hdl, block_count, audio_samples, hdl->v);
    }
    else
    {
        for(block_count = 0; block_count < hdl->sbc->blocks; block_count++)
            polyPhaseSynthesisSB8(hdl, block_count, audio_samples, hdl->v);
    }
}
