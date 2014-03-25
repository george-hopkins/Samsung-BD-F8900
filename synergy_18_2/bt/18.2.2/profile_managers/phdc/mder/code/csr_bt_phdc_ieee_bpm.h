#ifndef CSR_BT_PHDC_IEEE_BPM_H__
#define CSR_BT_PHDC_IEEE_BPM_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* IEEE Headers */
#include "csr_bt_phdc_ieee.h"



#define MDC_SYS_DIA_MEAN_OBJ_HANDLE 1
#define MDC_PULSE_RATE_OBJ_HANDLE 2
#define BPM_STANDARD_MEAS_OBJ_NUM 2

#define MDC_SYS_DIA_ATRB_CNT 6
#define BPM_METRIC_ID_COUNT 3
#define BPM_ATTR_VAL_CNT_SYS_DIA_MEAN_OBJ 2
#define BPM_CMPD_VAL_SYS_DIA_MEAN_LEN 10
#define BPM_ABS_TIME_STAMP_LEN 8

#define BPM_PULSE_RATE_INVALID 0xFFFF

#define MDC_PULSE_RATE_ATRB_CNT 4
#define BPM_ATTR_VAL_CNT_PULSE_RATE_OBJ 2
#define BPM_NU_VAL_PULSE_RATE_LEN 2

typedef struct BPM_SYS_DIA_MAP_T
{
    CsrUint16              count;
    CsrUint16              length;
    CsrUint16              systolic;
    CsrUint16              diastolic;
    CsrUint16              map;
    tAbsTimeData        absTime;

} BPM_SYS_DIA_MAP_T;

typedef struct BPM_PULSE_RATE_T
{
    CsrUint16              pulse;
    tAbsTimeData        absTime;

} BPM_PULSE_RATE_T;

extern const ConfigTable stdConfBpm;

/* Function Prototypes */
extern void ieeeCreateBPMDataTransferReq(tIeeeBPMMeasData *measData);


#ifdef __cplusplus
extern "C" }
#endif

#endif

