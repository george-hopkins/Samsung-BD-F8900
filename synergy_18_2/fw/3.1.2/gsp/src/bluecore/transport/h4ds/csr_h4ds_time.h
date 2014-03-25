#ifndef CSR_H4DS_TIME_H__
#define CSR_H4DS_TIME_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Timer periods in ms */
#define TSYNC_TIME               250000
#define TCONF_TIME               250000
#define TWU_TIME                 250000
#define TFLUSH_TIME             1000000
#define TUART_IDLE_TIME           50000
#define TTX_WATCH_TIME            10000

#ifdef __cplusplus
}
#endif

#endif
