#ifndef CSR_BT_SBC_H__
#define CSR_BT_SBC_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_sbc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* #define ENABLE_32BIT_PRECISION */
/* #define ENABLE_FLOAT_PRECISION */

#ifdef ENABLE_FLOAT_PRECISION
typedef double CsrSbcSbSamples;
typedef double CsrSbcCoefficientSize;
#else
typedef CsrSbcFixpt32 CsrSbcSbSamples;
#ifndef ENABLE_32BIT_PRECISION
typedef CsrSbcFixpt16 CsrSbcCoefficientSize;
#define FIX_SCALE 15
#else
#define FIX_SCALE 30
typedef CsrSbcFixpt32 CsrSbcCoefficientSize;
#endif
#endif

#define SBC_SYNC_WORD                   0x9C
#define SBC_CRC_INIT                    0x0F

typedef struct
{
    CsrUint8 channels;
    CsrUint8 blocks;
    CsrUint8 subbands;
    CsrUint8 bitpool;
    CsrUint8 sample_freq;
    CsrUint8 joint;
    CsrSbcChannelMode ch_mode;
    CsrSbcAllocMethod alloc_method;
    CsrUint16 frame_len;
    CsrUint16 partial_posn;
    CsrUint32 partial;
    CsrUint8 *frame;
} SbcControl_t;

typedef struct
{
    /* SBC module data */
    CsrSbcSbSamples ***sb_sample; /*array of elements [CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];*/

    CsrInt16 **scale_factor; /* array of elements [CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];*/
    CsrUint16 **bits; /* array of elements [CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];*/
    CsrUint16 **levels; /* array of elements [CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS];*/

    SbcControl_t *sbc;

    CsrInt16 **audio_window; /* array of elements [CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_WINDOW];*/ /* encoder */
    CsrSbcFixptVector **v; /* array of elements [CSR_SBC_MAX_CHANNELS][2*CSR_SBC_MAX_WINDOW];        */  /* decoder */

} SbcHandle_t;

/* sample frequency */
typedef enum {
    sample_freq_16k,
    sample_freq_32k,
    sample_freq_44_1k,
    sample_freq_48k
} sbc_sample_freq;


#define Multiply32by16(a,b) (((a >> 16) * b) + (((a & 0x0000FFFF) * b) >> 16))
#define Multiply32by32(a,b) (((a >> 16) * (b >> 16)) + ((((a & 0x0000FFFF) * (b >> 16)) + ((b & 0x0000FFFF) * (a >> 16))) >> 16))

#define MAX_FIX ((CsrSbcCoefficientSize) ((1<<FIX_SCALE)-1))
#define MIN_FIX ((CsrSbcCoefficientSize) (-(1<<FIX_SCALE)))
#define DBL_SCALE(d) ((d)*((double) (1<<FIX_SCALE)))

#define ROUND(d) ((d) > 0.0 ? (int) ((d) + 0.5) : -(int) ((-(d)) + 0.5))

#define SATURATE(a) \
  ((a) > MAX_FIX ? MAX_FIX : (a) < MIN_FIX ? MIN_FIX : (a))

#ifdef ENABLE_FLOAT_PRECISION
#define DBL2FIX(d) (d)
#else
#define DBL2FIX(d) SATURATE(ROUND(DBL_SCALE(d)))
#endif

extern void sbcCalcCRC(CsrInt16 bs, CsrUint16 word, CsrUint8 *crc);
extern void bitAllocate(SbcHandle_t *hdl, CsrSbcAllocMethod method, CsrUint8 nof_channels,
                        CsrSbcChannelMode channel_mode, CsrUint8 nof_subbands,
                        CsrUint8 sample_freq, CsrUint16 bitpool);
extern void calcLevels(SbcHandle_t *hdl);
extern void calc_sbc_frame_len(SbcHandle_t *hdl);

extern void polyPhaseAnalysisSB4(
    SbcHandle_t *hdl,
    CsrUint8 nof_channels, CsrUint8 current_block,
    CsrInt16 **audio_window);
extern void polyPhaseAnalysisSB8(
    SbcHandle_t *hdl,
    CsrUint8 nof_channels, CsrUint8 current_block,
    CsrInt16 **audio_window);
extern void polyPhaseSynthesisSB4(
    SbcHandle_t *hdl,
    CsrUint8 current_block,
    CsrInt16 audio_samples[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS],
    CsrSbcFixptVector **v);
extern void polyPhaseSynthesisSB8(
    SbcHandle_t *hdl,
    CsrUint8 current_block,
    CsrInt16 audio_samples[CSR_SBC_MAX_BLOCKS][CSR_SBC_MAX_CHANNELS][CSR_SBC_MAX_SUBBANDS],
    CsrSbcFixptVector **v);

#ifdef __cplusplus
}
#endif

#endif
