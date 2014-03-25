/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/*
 * Just the polyPhaseAnalysis routines, these can be ommitted from the
 * decoder.
 */

#include "csr_polyphase.h"
#include "csr_cos_cpp.h"
#include "csr_mod_math_cpp.h"

/* We do the wrapping with macros to avoid typing errors. */
#define YCOS__(a,s,b) (s y ## a ## _cos ## b)
#define YCOS_(a,s,b) YCOS__(a,s,b)
#define YCOS(a,b) YCOS_(a, COSSIGN(b), COSWRAP(b))

#ifdef ENABLE_FLOAT_PRECISION
#define MultiplyFilter(a,b) (a*((double)b))
#define Multiply32byCos(a,b)(a*b)
#define SCALE_OVERFLOW
#else
#ifndef ENABLE_32BIT_PRECISION
#define MultiplyFilter(a,b) (a*b)
#define Multiply32byCos(a,b) Multiply32by16(a,b)
#define SCALE_OVERFLOW >> 1
#define SCALE_Y 1
#else
#define MultiplyFilter(a,b) Multiply32by16(a,b)
#define Multiply32byCos(a,b) Multiply32by32(a,b)
#define SCALE_OVERFLOW
#define SCALE_Y 2
#endif
#endif

#define YCALC(i,j)  CsrSbcSbSamples y ## i ## _cos ## j = (Multiply32byCos(y[i], COS16(j)))

/* Encoder functions (internal) */
void polyPhaseAnalysisSB4(SbcHandle_t *hdl,
                          CsrUint8 nof_channels, CsrUint8 current_block,
                          CsrInt16 **audio_window)
{
    CsrUint8 ch;

    for (ch=0; ch<nof_channels; ch++)
    {
        CsrSbcSbSamples y[8];
        CsrUint8 i;

        /* windowing - weighted samples history and partial calculation */
        for (i=0; i<6;i++)
        {
            const CsrSbcCoefficientSize *tmp16_ptr = C_SB4 + i;
            const CsrSbcFixpt16 *xptr = &(audio_window[ch][i]);

            y[i] = (MultiplyFilter(tmp16_ptr[0] , xptr[0]) +
                    MultiplyFilter(tmp16_ptr[8] , xptr[8]) +
                    MultiplyFilter(tmp16_ptr[16] , xptr[16]) +
                    MultiplyFilter(tmp16_ptr[24] , xptr[24]) +
                    MultiplyFilter(tmp16_ptr[32] , xptr[32])) SCALE_OVERFLOW;
        }

        {
            const CsrSbcCoefficientSize *tmp16_ptr = C_SB4 + 7;
            const CsrSbcFixpt16 *xptr = &(audio_window[ch][7]);

            y[7] = (MultiplyFilter(tmp16_ptr[0] , xptr[0]) +
                    MultiplyFilter(tmp16_ptr[8] , xptr[8]) +
                    MultiplyFilter(tmp16_ptr[16] , xptr[16]) +
                    MultiplyFilter(tmp16_ptr[24] , xptr[24]) +
                    MultiplyFilter(tmp16_ptr[32] , xptr[32])) SCALE_OVERFLOW;
        }

        /* See the comments in the 8 sub-band function */
        y[0] = (y[0] + y[4]);
        y[1] = (y[1] + y[3]);
#ifndef ENABLE_FLOAT_PRECISION
        y[2] = (y[2] + (1 << (SCALE_Y - 1))) >> SCALE_Y;
#endif
        y[5] = (y[5] - y[7]);

#define YTERM_4(i,j) YCOS(j, MUL(PLUS(MUL(i,2),1),SUB(MUL(j,2),4)))

        {
            CsrSbcSbSamples *sptr = &hdl->sb_sample[current_block][ch][0];

            YCALC(0,4);

            YCALC(1,2);
            YCALC(1,6);

            YCALC(5,2);
            YCALC(5,6);

#if !defined(ENABLE_32BIT_PRECISION) && !defined(ENABLE_FLOAT_PRECISION)
#define CALCSPTR_4(i) \
            sptr[i] = ((YTERM_4(i,0) + y[2]) SCALE_OVERFLOW) + ((YTERM_4(i,1) + YTERM_4(i,5)) SCALE_OVERFLOW)
#else
#define CALCSPTR_4(i) \
            sptr[i] = y[2] + YTERM_4(i,0) + YTERM_4(i,1) + YTERM_4(i,5)
#endif
            CALCSPTR_4(0);
            CALCSPTR_4(1);
            CALCSPTR_4(2);
            CALCSPTR_4(3);
        }
    }
}

void polyPhaseAnalysisSB8(SbcHandle_t *hdl,
                          CsrUint8 nof_channels, CsrUint8 current_block,
                          CsrInt16 **audio_window)
{
    CsrUint8 ch;
    CsrSbcSbSamples y[16];

    for (ch=0; ch<nof_channels; ch++)
    {
        CsrUint8 i;
        const CsrSbcCoefficientSize *tmp16_ptr;
        const CsrSbcFixpt16 *xptr;

        /* windowing - weighted samples history and partial calculation */
        for (tmp16_ptr = C_SB8, xptr = &audio_window[ch][0], i=0; i<12;
             i++, tmp16_ptr++, xptr++)
        {
            y[i] =
                (MultiplyFilter(tmp16_ptr[0] , xptr[0]) +
                MultiplyFilter(tmp16_ptr[16] , xptr[16]) +
                MultiplyFilter(tmp16_ptr[32] , xptr[32]) +
                MultiplyFilter(tmp16_ptr[48] , xptr[48]) +
                MultiplyFilter(tmp16_ptr[64] , xptr[64])) SCALE_OVERFLOW;
        }

        for (tmp16_ptr = C_SB8 + 13, xptr = &audio_window[ch][13], i=13; i<16;
             i++, tmp16_ptr++, xptr++)
        {
            y[i] =
                (MultiplyFilter(tmp16_ptr[0] , xptr[0]) +
                MultiplyFilter(tmp16_ptr[16] , xptr[16]) +
                MultiplyFilter(tmp16_ptr[32] , xptr[32]) +
                MultiplyFilter(tmp16_ptr[48] , xptr[48]) +
                MultiplyFilter(tmp16_ptr[64] , xptr[64])) SCALE_OVERFLOW;
        }

        /*
         * We can exploit some symmetry in this table to reduce the number of
         * multiplies.
         *
         *   A_M8[i][x] = cos((x-4)*(2*i+1)*2*pi/32)
         *
         * First off, we observe that:
         *
         *   A_M8[i][4] = cos((4-4)*(2*i+1)*2*pi/32) = cos(0) = 1
         *
         *   A_M8[i][12] = cos((12-4)*(2*i+1)*2*pi/32)
         *               = cos((i+0.5)*pi) = 0.
         *
         * Since cos(-x) = cos(x), we can also work out that:
         *
         *   A_M8[i][0] = A_M8[i][8]
         *   A_M8[i][1] = A_M8[i][7]
         *   A_M8[i][2] = A_M8[i][6]
         *   A_M8[i][3] = A_M8[i][5]
         *
         * And since cos((i+1)/2*pi+x) = -cos((i+1)/2*pi-x), we also get:
         *
         *   A_M8[i][9]  = -A_M8[i][15]
         *   A_M8[i][10] = -A_M8[i][14]
         *   A_M8[i][11] = -A_M8[i][13]
         *
         * This means that we can combine some y array elements.
         *
         * We've now reduced our original 8*16 = 128 multiplies down to
         * 8*7 = 56.
         */

        y[0] = (y[0] + y[8]);
        y[1] = (y[1] + y[7]);
        y[2] = (y[2] + y[6]);
        y[3] = (y[3] + y[5]);
#ifndef ENABLE_FLOAT_PRECISION
        y[4] = (y[4] + (1 << (SCALE_Y - 1))) >> SCALE_Y;
#endif
        y[9] = (y[9] - y[15]);
        y[10] = (y[10] - y[14]);
        y[11] = (y[11] - y[13]);

        /*
         * Now we've eliminated redundancies across the row, we can look at
         * calculations that are repeated down the columns. We'll use:
         *
         *   COS16{+/-a, +/-b, +/-c ...}
         *
         * as a shorthand for:
         *
         *   { +/-cos(a*pi/16), +/-cos(b*pi/16), +/-cos(c*pi/16), ...}
         *
         * so note that COS16{-4} means -cos(pi/4) and not cos(-pi/4) which is
         * +cos(pi/4).
         *
         * A_M8[i][0]  = COS16{  4, 12, 20, 28, 36, 44, 52,  60 }
         * A_M8[i][1]  = COS16{  3,  9, 15, 21, 27, 33, 39,  45 }
         * A_M8[i][2]  = COS16{  2,  6, 10, 14, 18, 22, 26,  30 }
         * A_M8[i][3]  = COS16{  1,  3,  5,  7,  9, 11, 13,  15 }
         * A_M8[i][9]  = COS16{  5, 15, 25, 35, 45, 55, 65,  75 }
         * A_M8[i][10] = COS16{  6, 18, 30, 42, 54, 66, 78,  90 }
         * A_M8[i][11] = COS16{  7, 21, 35, 49, 63, 77, 91, 105 }
         *
         * First we can eliminate the repetition every 2*pi so that
         * COS16{x+32} = COS16{x}:
         *
         * A_M8[i][0]  = COS16{  4, 12, 20, 28,  4, 12, 20, 28 }
         * A_M8[i][1]  = COS16{  3,  9, 15, 21, 27,  1,  7, 13 }
         * A_M8[i][2]  = COS16{  2,  6, 10, 14, 18, 22, 26, 30 }
         * A_M8[i][3]  = COS16{  1,  3,  5,  7,  9, 11, 13, 15 }
         * A_M8[i][9]  = COS16{  5, 15, 25,  3, 13, 23,  1, 11 }
         * A_M8[i][10] = COS16{  6, 18, 30, 10, 22,  2, 14, 26 }
         * A_M8[i][11] = COS16{  7, 21,  3, 15, 31, 13, 27,  9 }
         *
         * Next we eliminate the symmetry COS16{32-x} = COS16{x} to reduce the
         * range to 0..16.
         *
         * A_M8[i][0]  = COS16{  4, 12, 12,  4,  4, 12, 12,  4 }
         * A_M8[i][1]  = COS16{  3,  9, 15, 11,  5,  1,  7, 13 }
         * A_M8[i][2]  = COS16{  2,  6, 10, 14, 14, 10,  6,  2 }
         * A_M8[i][3]  = COS16{  1,  3,  5,  7,  9, 11, 13, 15 }
         * A_M8[i][9]  = COS16{  5, 15,  7,  3, 13,  9,  1, 11 }
         * A_M8[i][10] = COS16{  6, 14,  2, 10, 10,  2, 14,  6 }
         * A_M8[i][11] = COS16{  7, 11,  3, 15,  1, 13,  5,  9 }
         *
         * Then we reduce the range further with the relationship COS16{16-x} =
         * -COS16{x} to bring us into 0..8.
         *
         * A_M8[i][0]  = COS16{  4, -4, -4,  4,  4, -4, -4,  4 }
         * A_M8[i][1]  = COS16{  3, -7, -1, -5,  5,  1,  7, -3 }
         * A_M8[i][2]  = COS16{  2,  6, -6, -2, -2, -6,  6,  2 }
         * A_M8[i][3]  = COS16{  1,  3,  5,  7, -7, -5, -3, -1 }
         * A_M8[i][9]  = COS16{  5, -1,  7,  3, -3, -7,  1, -5 }
         * A_M8[i][10] = COS16{  6, -2,  2, -6, -6,  2, -2,  6 }
         * A_M8[i][11] = COS16{  7, -5,  3, -1,  1, -3,  5, -7 }
         *
         * This means that each element in the y array has to be multiplied by
         * at most 4 constants.
         *
         * We can precalculate these and then add them. This gives us just 21
         * multiplies instead of the 128 we started off with.
         */

        {
            /*
             * We create a new context so we can initialise the variables
             * when we declare them.
             */

            CsrSbcSbSamples *sptr = &hdl->sb_sample[current_block][ch][0];

            YCALC(0,4);

            YCALC(1,1);
            YCALC(1,3);
            YCALC(1,5);
            YCALC(1,7);

            YCALC(2,2);
            YCALC(2,6);

            YCALC(3,1);
            YCALC(3,3);
            YCALC(3,5);
            YCALC(3,7);

            YCALC(9,1);
            YCALC(9,3);
            YCALC(9,5);
            YCALC(9,7);

            YCALC(10,2);
            YCALC(10,6);

            YCALC(11,1);
            YCALC(11,3);
            YCALC(11,5);
            YCALC(11,7);

#define YTERM_8(i,j) YCOS(j, MUL(PLUS(MUL(i,2),1),SUB(j,4)))

#if !defined(ENABLE_32BIT_PRECISION) && !defined(ENABLE_FLOAT_PRECISION)
#define CALCSPTR_8(i) \
            sptr[i] = ((((YTERM_8(i,0) + y[4]) SCALE_OVERFLOW) + ((YTERM_8(i,2) + YTERM_8(i,10)) SCALE_OVERFLOW)) SCALE_OVERFLOW) + \
                       ((((YTERM_8(i,1) + YTERM_8(i,9)) SCALE_OVERFLOW) + ((YTERM_8(i,3) + YTERM_8(i,11)) SCALE_OVERFLOW)) SCALE_OVERFLOW)
#else
#define CALCSPTR_8(i) \
            sptr[i] = y[4] + YTERM_8(i,0) + YTERM_8(i,2) + YTERM_8(i,10) + YTERM_8(i,1) + YTERM_8(i,9) + YTERM_8(i,3) + YTERM_8(i,11)
#endif
            CALCSPTR_8(0);
            CALCSPTR_8(1);
            CALCSPTR_8(2);
            CALCSPTR_8(3);
            CALCSPTR_8(4);
            CALCSPTR_8(5);
            CALCSPTR_8(6);
            CALCSPTR_8(7);
        }
    }
}
