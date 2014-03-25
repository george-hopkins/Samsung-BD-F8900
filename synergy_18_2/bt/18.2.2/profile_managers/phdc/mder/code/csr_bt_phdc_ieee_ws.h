#ifndef CSR_BT_PHDC_IEEE_WS_H__
#define CSR_BT_PHDC_IEEE_WS_H__

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


#define MDC_WEIGHT_OBJ_HANDLE 1
#define WS_STANDARD_MEAS_OBJ_NUM 1
#define SCALE_ABS_TIME_STAMP_LEN 8

#define MDC_WEIGHT_ATRB_CNT 4
#define SCALE_ATTR_VAL_CNT_WEIGHT_OBJ 2
#define SCALE_VAL_WEIGHT_LEN 4

typedef struct WS_WEIGHT_T
{
    CsrUint32              weight;
    tAbsTimeData        absTime;

} WS_WEIGHT_T;

extern const ConfigTable stdConfWs;

/* Function Prototypes */

extern void ieeeCreateScaleDataTransferReq(tIeeeWSMeasData *measData);

#ifdef __cplusplus
extern "C" }
#endif

#endif
