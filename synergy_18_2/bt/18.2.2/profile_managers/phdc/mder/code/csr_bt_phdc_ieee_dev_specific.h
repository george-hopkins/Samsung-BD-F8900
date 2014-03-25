#ifndef CSR_BT_PHDC_IEEE_DEVICE_SPECIFIC__
#define CSR_BT_PHDC_IEEE_DEVICE_SPECIFIC__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "csr_types.h"
/* IEEE Headers */
#include "csr_bt_phdc_ieee_bpm.h"
#include "csr_bt_phdc_ieee_ws.h"
#include "csr_bt_phdc_ieee_asn_config_if.h"


typedef struct OBS_SCAN_FIXED_DEV_SPECIFIC_STR_T
{
    CsrUint16 choice;

    #define BPM_SYS_DIA_MAP_CHOSEN 0x0800
    #define BPM_PULSE_RATE_CHOSEN 0x0400
    #define WS_WEIGHT_CHOSEN 0x0200
    #define DATE_TIME_ADJUST_CHOSEN 0x0100

    #define EXT_CONF_OBJ_DATA_CHOSEN 0x0001

    union {
        BPM_SYS_DIA_MAP_T       sysDiaMapPdu;
        BPM_PULSE_RATE_T        pulseRatePdu;
        WS_WEIGHT_T             weightPdu;
        MeasData*               pExtConfDataPdu;
    }u;

}   OBS_SCAN_FIXED_DEV_SPECIFIC_STR_T;

#ifdef __cplusplus
extern "C" }
#endif

#endif /* CSR_BT_PHDC_IEEE_DEVICE_SPECIFIC */

