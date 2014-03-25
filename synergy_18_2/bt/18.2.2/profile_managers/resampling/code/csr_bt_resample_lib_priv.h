#ifndef CSR_BT_RESAMPLE_LIB_PRIV_H__
#define CSR_BT_RESAMPLE_LIB_PRIV_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif


#define INPUT   0
#define OUTPUT  1
#define RESAMPLING_MONO     0
#define RESAMPLING_STEREO   1



#include "csr_types.h"
/*#include "CsrSched/csr_bt_pmalloc.h" */

#include "csr_bt_circular_buffer.h"

/* File parameters */
typedef struct
{
    CsrInt32     fs;             /* Sample frequency */
    CsrUint8     chan;           /* Channel type = {MONE, STEREO} */
    CsrInt32     blockSize;      /* Block size */
} fileCfg_t;

/* Resampling filter */
typedef struct
{
    CsrInt32     d;
    CsrInt16     dataOld;
    CsrInt16     oldBlockData;
    CsrInt32     stepIncrease;
    CsrInt32     step;
    circBuf_t   *buf;
} resampleFilter_t;

typedef struct
{
    fileCfg_t           file[2];
    resampleFilter_t    filter;
    CsrUint8             bufStatus;
    void                (*Resampling)(CsrBtResamplingHandler inst, CsrInt16 *dataIn, CsrInt32 blockSize);
} localInstanceData_t;

/* Instance data */
typedef struct
{
    localInstanceData_t *c[2];    /* Need this struct to hold the STEREO channel if needed*/
    CsrUint8 cnt;
} instanceData_t;

#ifdef __cplusplus
}
#endif

#endif  /* RESAMPLE_LIB */

