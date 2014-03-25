/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_polyphase_arm11.h"
#include <string.h>
#include "csr_cos_cpp.h"
#include "csr_mod_math_cpp.h"

#define COS16_2(a,b) COS16(a), COS16(b)
#define COS16_4(a,b,c,d) COS16_2(a,b), COS16_2(c,d)
#define COS16_8(a,b,c,d,e,f,g,h) COS16_4(a,b,c,d), COS16_4(e,f,g,h)

/* synthesis subband filterbank matrix for (8x4) M = 4 */
static const CsrSbcCoefficientSize S_M4[8][4] =
{
    { COS16_4( 4,12,12, 4) },
    { COS16_4( 6,14, 2,10) },
    { COS16_4( 8, 8, 8, 8) },
    { COS16_4(10, 2,14, 6) },
    { COS16_4(12, 4, 4,12) },
    { COS16_4(14,10, 6, 2) },
    { COS16_4(16,16,16,16) },
    { COS16_4(14,10, 6, 2) }
};

/* synthesis subband filterbank matrix for ( 16x8) M = 8 */
static const CsrSbcCoefficientSize S_M8[16][8] =
{
    { COS16_8( 4,12,12, 4, 4,12,12, 4) },
    { COS16_8( 5,15, 7, 3,13, 9, 1,11) },
    { COS16_8( 6,14, 2,10,10, 2,14, 6) },
    { COS16_8( 7,11, 3,15, 1,13, 5, 9) },
    { COS16_8( 8, 8, 8, 8, 8, 8, 8, 8) },
    { COS16_8( 9, 5,13, 1,15, 3,11, 7) },
    { COS16_8(10, 2,14, 6, 6,14, 2,10) },
    { COS16_8(11, 1, 9,13, 3, 7,15, 5) },
    { COS16_8(12, 4, 4,12,12, 4, 4,12) },
    { COS16_8(13, 7, 1, 5,11,15, 9, 3) },
    { COS16_8(14,10, 6, 2, 2, 6,10,14) },
    { COS16_8(15,13,11, 9, 7, 5, 3, 1) },
    { COS16_8(16,16,16,16,16,16,16,16) },
    { COS16_8(15,13,11, 9, 7, 5, 3, 1) },
    { COS16_8(14,10, 6, 2, 2, 6,10,14) },
    { COS16_8(13, 7, 1, 5,11,15, 9, 3) }
};

#ifndef ENABLE_32BIT_PRECISION
#define MultiplyFilter(a,b) Multiply32by16(a,b)
#define MultiplyCos(a,b) Multiply32by16(a,b)
#define SCALE_OVERFLOW 1
#define SCALE_Y 1
#define SCALE_DOWN(a) ( a + (1 << (12)) ) >> 13
#else
#define MultiplyFilter(a,b) Multiply32by32(a,b)
#define MultiplyCos(a,b) Multiply32by32(a,b)
#define SCALE_OVERFLOW 0
#define SCALE_Y 2
#define SCALE_DOWN(a) ( a + (1 << (11)) ) >> 12
#endif

#define SUBBAND_SHIFT 9

/* Decoder functions (internal) */
void polyPhaseSynthesisSB4(
    SbcHandle_t *hdl,
    CsrUint8 current_block,
    CsrInt16 audio_samples[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS],
    CsrSbcFixptVector **v)
{
    int i, j, ch;

    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        /* Advance the sample window */
        memmove(&v[ch][8], v[ch], (CSR_SBC_BLOCKS_IN_WINDOW*8-8)*sizeof(CsrSbcFixptVector));

        /* Matrixing */
        {
            const CsrSbcFixpt32 *p1 = hdl->sb_sample[current_block][ch];
            const CsrSbcCoefficientSize *p2 = S_M4[0];
            CsrSbcFixptVector *tmp16_ptr1 = &v[ch][0];

#define ROT_SYMM_XPROD_4(x) \
                p2 = S_M4[x]; \
                tmp16_ptr1[x] = (CsrSbcFixptVector) ((MultiplyCos((p1[0]-p1[3]) , p2[0])) + \
                                             (MultiplyCos((p1[1]-p1[2]) , p2[1])));


            /* building v[ch][0:7]; */
            tmp16_ptr1[0] = (CsrSbcFixptVector) (MultiplyCos((p1[0]+p1[3] -p1[1]-p1[2]) , p2[0]));
            ROT_SYMM_XPROD_4(1)
            tmp16_ptr1[2] = 0;
            ROT_SYMM_XPROD_4(3)
            p2 = S_M4[4];   tmp16_ptr1[4] = (CsrSbcFixptVector) (MultiplyCos((p1[0]+p1[3] -p1[1]-p1[2]) , p2[0]));
            ROT_SYMM_XPROD_4(5)

            p2 = S_M4[6];   tmp16_ptr1[6] = (-p1[0]-p1[1]-p1[2]-p1[3])>>1;

            tmp16_ptr1[7] = tmp16_ptr1[5];
        }

        /* Window and calculate the next 4 audio samples */
        for (i=0; i<4; i++)
        {
            CsrSbcFixpt32 tmp_32 = 0;

            const CsrSbcFixptVector *p1 = v[ch] + i;
            const CsrSbcCoefficientSize *p2 = C_SB4 + i;

            for(j = CSR_SBC_BLOCKS_IN_WINDOW/2; j--; p1 += 16, p2 += 8)
                tmp_32 -= MultiplyFilter(*p1 , *p2) + MultiplyFilter(p1[12] , p2[4]);

            if(tmp_32 < -0x00FFFEFF)
            {
                tmp_32 = -0x00FFFEFF;
            }

            if(tmp_32 > 0x00FFFEFF)
            {
                tmp_32 = 0x0FFFEFF;
            }

            audio_samples[current_block][ch][i] = (CsrSbcFixpt16) ((tmp_32+(1<<(SUBBAND_SHIFT-1)))>>SUBBAND_SHIFT);
        }
    }
}

void polyPhaseSynthesisSB8(
    SbcHandle_t *hdl,
    CsrUint8 current_block,
    CsrInt16 audio_samples[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS],
    CsrSbcFixptVector **v)
{
    int i, j, ch;

    for (ch=0; ch < hdl->sbc->channels; ch++)
    {
        /* Advance the sample window */
        memmove(&v[ch][16], v[ch], (CSR_SBC_BLOCKS_IN_WINDOW*16-16)*sizeof(CsrSbcFixptVector));

        /* Matrixing */
        {
            const CsrSbcFixpt32 *p1 = hdl->sb_sample[current_block][ch];
            const CsrSbcCoefficientSize *p2 = S_M8[0];
            CsrSbcFixptVector *tmp16_ptr1 = v[ch];

#define ROT_SYMM_XPROD_8(x) \
                p2 = S_M8[x]; \
                tmp16_ptr1[x] = (CsrSbcFixptVector) ((MultiplyCos((p1[0]-p1[7]) , p2[0])) + \
                                                (MultiplyCos((p1[1]-p1[6]) , p2[1])) + \
                                                (MultiplyCos((p1[2]-p1[5]) , p2[2])) + \
                                                (MultiplyCos((p1[3]-p1[4]) , p2[3])));

#define REFLEC_SYMM_XPROD_8(x) \
                p2 = S_M8[x]; \
                tmp16_ptr1[x] = (CsrSbcFixptVector) ((MultiplyCos((p1[0]+p1[7]-p1[3]-p1[4]) , p2[0])) \
                                            + (MultiplyCos((p1[1]+p1[6]-p1[2]-p1[5]) , p2[1])));


            /* building v[ch][0:15]; */
            tmp16_ptr1[0] = (CsrSbcFixptVector) (MultiplyCos((p1[0]+p1[3]+p1[4]+p1[7]-p1[1]-p1[2]-p1[5]-p1[6]) , p2[0]));
            ROT_SYMM_XPROD_8(1)
            REFLEC_SYMM_XPROD_8(2)
            ROT_SYMM_XPROD_8(3)
            tmp16_ptr1[4] = 0;
            tmp16_ptr1[5] = -tmp16_ptr1[3];
            tmp16_ptr1[6] = -tmp16_ptr1[2];
            tmp16_ptr1[7] = -tmp16_ptr1[1];
            tmp16_ptr1[8] = -tmp16_ptr1[0];
            ROT_SYMM_XPROD_8(9)
            REFLEC_SYMM_XPROD_8(10)
            ROT_SYMM_XPROD_8(11)

            tmp16_ptr1[12] = (-p1[0] -p1[1] -p1[2] -p1[3] -p1[4] -p1[5] -p1[6] -p1[7])>>1;

            tmp16_ptr1[13] = tmp16_ptr1[11];
            tmp16_ptr1[14] = tmp16_ptr1[10];
            tmp16_ptr1[15] = tmp16_ptr1[9];
        }

        /* Window and calculate the next 8 audio samples */
        for (i=0; i<8; i++)
        {
            CsrSbcFixpt32 tmp_32 = 0;

            const CsrSbcFixptVector *p1 = v[ch] + i;
            const CsrSbcCoefficientSize *p2 = C_SB8 + i;

            for(j = CSR_SBC_BLOCKS_IN_WINDOW/2; j--; p1 += 32, p2 += 16)
                tmp_32 -= MultiplyFilter(*p1 , *p2) + MultiplyFilter(p1[24] , p2[8]) ;

            if(tmp_32 < -0x00FFFEFF)
            {
                tmp_32 = -0x00FFFEFF;
            }

            if(tmp_32 > 0x00FFFEFF)
            {
                tmp_32 = 0x0FFFEFF;
            }

            audio_samples[current_block][ch][i] = (CsrSbcFixpt16) ((tmp_32+(1<<(SUBBAND_SHIFT-1)))>>SUBBAND_SHIFT);
        }
    }
}

