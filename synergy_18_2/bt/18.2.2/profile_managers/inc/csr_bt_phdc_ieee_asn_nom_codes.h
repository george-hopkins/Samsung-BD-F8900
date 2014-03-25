#ifndef     CSR_BT_PHDC_IEEE_ASN_NOM_CODES_H__
#define     CSR_BT_PHDC_IEEE_ASN_NOM_CODES_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
/*!
@file   ieee_asn_config_if.h

@brief Header file for accessing ASN.1 types defined in IEEE 11073 specification.

These types are used for defining IEEE standard objects and attributes. Refer to
IEEE 11073-20601 specification document for details.

*/


#ifdef __cplusplus
extern "C" {
#endif

/************** IEEE ASN Defined Data Types **************/

/* Partition codes */

#define MDC_PART_OBJ 1 /* Object Infrastr. */
#define MDC_PART_SCADA 2 /* SCADA (Physio IDs */
#define MDC_PART_DIM 4 /* Dimension */
#define MDC_PART_INFRA 8 /* Infrastructure */
#define MDC_PART_PHD_DM 128 /* Disease Mgmt */
#define MDC_PART_PHD_HF 129 /* Health and Fitness */
#define MDC_PART_PHD_AI 130 /* Aging Independently */
#define MDC_PART_RET_CODE 255 /* Return Codes */
#define MDC_PART_EXT_NOM 256 /* Ext. Nomenclature */

/**********************************************************************************

* From Object Infrastructure (MDC_PART_OBJ)

**********************************************************************************/

#define MDC_MOC_VMO_METRIC          4   /* 0x04 */
#define MDC_MOC_VMO_METRIC_ENUM     5   /* 0x05 */
#define MDC_MOC_VMO_METRIC_NU       6   /* 0x06 */
#define MDC_MOC_VMO_METRIC_SA_RT    9   /* 0x0A */
#define MDC_MOC_SCAN                16  /* 0x10 */
#define MDC_MOC_SCAN_CFG            17  /* 0x11 */
#define MDC_MOC_SCAN_CFG_EPI        18  /* 0x12 */
#define MDC_MOC_SCAN_CFG_PERI       19  /* 0x13 */
#define MDC_MOC_VMS_MDS_SIMP        37  /* 0x25 */
#define MDC_MOC_VMO_PMSTORE         61  /* 0x3D */
#define MDC_MOC_PM_SEGMENT          62  /* 0x3E */

#define MDC_ATTR_CONFIRM_MODE               2323 /* */
#define MDC_ATTR_CONFIRM_TIMEOUT            2324 /* */
#define MDC_ATTR_ID_HANDLE                  2337 /* */
#define MDC_ATTR_ID_INSTNO                  2338 /* */
#define MDC_ATTR_ID_LABEL_STRING            2343 /* */
#define MDC_ATTR_ID_MODEL                   2344 /* */
#define MDC_ATTR_ID_PHYSIO                  2347 /* */
#define MDC_ATTR_ID_PROD_SPECN              2349 /* */
#define MDC_ATTR_ID_TYPE                    2351 /* */
#define MDC_ATTR_METRIC_STORE_CAPAC_CNT     2369 /* */
#define MDC_ATTR_METRIC_STORE_SAMPLE_ALG    2371 /* */
#define MDC_ATTR_METRIC_STORE_USAGE_CNT     2372 /* */
#define MDC_ATTR_MSMT_STAT                  2375 /* */
#define MDC_ATTR_NU_ACCUR_MSMT              2378 /* */
#define MDC_ATTR_NU_CMPD_VAL_OBS            2379 /* */
#define MDC_ATTR_NU_VAL_OBS                 2384 /* */
#define MDC_ATTR_NUM_SEG                    2385 /* */
#define MDC_ATTR_OP_STAT                    2387 /* */

#define MDC_ATTR_POWER_STAT                 2389 /* */
#define MDC_ATTR_SA_SPECN                   2413 /* */
#define MDC_ATTR_SCALE_SPECN_I16            2415 /* */
#define MDC_ATTR_SCALE_SPECN_I32            2416 /* */
#define MDC_ATTR_SCALE_SPECN_I8             2417 /* */
#define MDC_ATTR_SCAN_REP_PD                2421 /* */
#define MDC_ATTR_SEG_USAGE_CNT              2427 /* */
#define MDC_ATTR_SYS_ID                     2436 /* */
#define MDC_ATTR_SYS_TYPE                   2438 /* */
#define MDC_ATTR_TIME_ABS                   2439 /* */
#define MDC_ATTR_TIME_BATT_REMAIN           2440 /* */
#define MDC_ATTR_TIME_END_SEG               2442 /* */
#define MDC_ATTR_TIME_PD_SAMP               2445 /* */
#define MDC_ATTR_TIME_REL                   2447 /* */
#define MDC_ATTR_TIME_STAMP_ABS             2448 /* */
#define MDC_ATTR_TIME_STAMP_REL             2449 /* */
#define MDC_ATTR_TIME_START_SEG             2450 /* */
#define MDC_ATTR_TX_WIND                    2453 /* */
#define MDC_ATTR_UNIT_CODE                  2454 /* */
#define MDC_ATTR_UNIT_LABEL_STRING          2457 /* */
#define MDC_ATTR_VAL_BATT_CHARGE            2460 /* */
#define MDC_ATTR_VAL_ENUM_OBS               2462 /* */
#define MDC_ATTR_TIME_REL_HI_RES            2536 /* */
#define MDC_ATTR_TIME_STAMP_REL_HI_RES      2537 /* */
#define MDC_ATTR_DEV_CONFIG_ID              2628 /* */
#define MDC_ATTR_MDS_TIME_INFO              2629 /* */
#define MDC_ATTR_METRIC_SPEC_SMALL          2630 /* */
#define MDC_ATTR_SOURCE_HANDLE_REF          2631 /* */
#define MDC_ATTR_SIMP_SA_OBS_VAL            2632 /* */
#define MDC_ATTR_ENUM_OBS_VAL_SIMP_OID      2633 /* */
#define MDC_ATTR_ENUM_OBS_VAL_SIMP_STR      2634 /* */
#define MDC_ATTR_REG_CERT_DATA_LIST         2635 /* */
#define MDC_ATTR_NU_VAL_OBS_BASIC           2636 /* */
#define MDC_ATTR_PM_STORE_CAPAB             2637 /* */
#define MDC_ATTR_PM_SEG_MAP                 2638 /* */
#define MDC_ATTR_PM_SEG_PERSON_ID           2639 /* */
#define MDC_ATTR_SEG_STATS                  2640 /* */
#define MDC_ATTR_SEG_FIXED_DATA             2641 /* */
#define MDC_ATTR_PM_SEG_ELEM_STAT_ATTR      2642 /* */
#define MDC_ATTR_SCAN_HANDLE_ATTR_VAL_MAP   2643 /* */
#define MDC_ATTR_SCAN_REP_PD_MIN            2644 /* */
#define MDC_ATTR_ATTRIBUTE_VAL_MAP          2645 /* */
#define MDC_ATTR_NU_VAL_OBS_SIMP            2646 /* */
#define MDC_ATTR_PM_STORE_LABEL_STRING      2647 /* */
#define MDC_ATTR_PM_SEG_LABEL_STRING        2648 /* */
#define MDC_ATTR_TIME_PD_MSMT_ACTIVE        2649 /* */
#define MDC_ATTR_SYS_TYPE_SPEC_LIST         2650 /* */
#define MDC_ATTR_METRIC_ID_PART             2655 /* */
#define MDC_ATTR_ENUM_OBS_VAL_PART          2656 /* */
#define MDC_ATTR_SUPPLEMENTAL_TYPES         2657 /* */
#define MDC_ATTR_TIME_ABS_ADJUST            2658 /* */
#define MDC_ATTR_CLEAR_TIMEOUT              2659 /* */
#define MDC_ATTR_TRANSFER_TIMEOUT           2660 /* */
#define MDC_ATTR_ENUM_OBS_VAL_SIMP_BIT_STR  2661 /* */
#define MDC_ATTR_ENUM_OBS_VAL_BASIC_BIT_STR 2662 /* */
#define MDC_ATTR_METRIC_STRUCT_SMALL        2675 /* */
#define MDC_ATTR_NU_CMPD_VAL_OBS_SIMP       2676 /* */
#define MDC_ATTR_NU_CMPD_VAL_OBS_BASIC      2677 /* */
#define MDC_ATTR_ID_PHYSIO_LIST             2678 /* */
#define MDC_ATTR_SCAN_HANDLE_LIST           2679 /* */

/* Partition: ACT */
#define MDC_ACT_SEG_CLR                     3084 /* */
#define MDC_ACT_SEG_GET_INFO                3085 /* */
#define MDC_ACT_SET_TIME                    3095 /* */
#define MDC_ACT_DATA_REQUEST                3099 /* */
#define MDC_ACT_SEG_TRIG_XFER               3100 /* */

#define MDC_NOTI_CONFIG                     3356 /* */
#define MDC_NOTI_SCAN_REPORT_FIXED          3357 /* */
#define MDC_NOTI_SCAN_REPORT_VAR            3358 /* */
#define MDC_NOTI_SCAN_REPORT_MP_FIXED       3359 /* */
#define MDC_NOTI_SCAN_REPORT_MP_VAR         3360 /* */
#define MDC_NOTI_SEGMENT_DATA               3361 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_VAR      3362 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_FIXED    3363 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_GROUPED  3364 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_MP_VAR   3365 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_MP_FIXED 3366 /* */
#define MDC_NOTI_UNBUF_SCAN_REPORT_MP_GROUPED 3367 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_VAR        3368 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_FIXED      3369 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_GROUPED    3370 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_MP_VAR     3371 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_MP_FIXED   3372 /* */
#define MDC_NOTI_BUF_SCAN_REPORT_MP_GROUPED 3373 /* */

/*********************************************************************************
* From Medical supervisory control and data acquisition (MDC_PART_SCADA)
*********************************************************************************/
/* From Personal health device communication - 11073-20601 */
#define MDC_TEMP_BODY               19292 /* TEMPbody */
#define MDC_MASS_BODY_ACTUAL        57664 /* body mass */
/* Partition: SCADA/Other */
#define MDC_BODY_FAT                57676 /* */

#define MDC_PULS_RATE_NON_INV       18474 /* pulse rate */
#define MDC_PRESS_BLD_NONINV        18948 /* systolic, diastolic, MAP */
#define MDC_PRESS_BLD_NONINV_SYS    18949 /* */
#define MDC_PRESS_BLD_NONINV_DIA    18950 /* */
#define MDC_PRESS_BLD_NONINV_MEAN   18951 /* */

/* From Weighing Scale - 11073-10415 */
#define MDC_LEN_BODY_ACTUAL         57668 /* body height */
#define MDC_RATIO_MASS_BODY_LEN_SQ  57680 /* body mass index */

/* From Cardiovascular fitness and Activity Monitor (11073-10441)*/
#define MDC_RESP_RATE               20490 /* */

/*********************************************************************************
* From Dimensions (MDC_PART_DIM)
**********************************************************************************/
#define MDC_DIM_BEAT_PER_MIN    2720 /* bpm */
#define MDC_DIM_KILO_PASCAL     3843 /* kPa */
#define MDC_DIM_MMHG            3872 /* mmHg */

#define MDC_DIM_PERCENT         544 /* % */
#define MDC_DIM_KILO_G          1731 /* kg */
#define MDC_DIM_MIN             2208 /* min */
#define MDC_DIM_HR              2240 /* h */
#define MDC_DIM_DAY             2272 /* d */
#define MDC_DIM_DEGC            6048 /* oC */

/* From Weighing Scale (11073-10415) */
#define MDC_DIM_CENTI_M         1297 /* cm */
#define MDC_DIM_INCH            1376 /* in */
#define MDC_DIM_LB              1760 /* lb */
#define MDC_DIM_KG_PER_M_SQ     1952 /* kg m-2 */

/* From Cardiovascular fitness and Activity Monitor (11073-10441)*/
#define MDC_DIM_DIMLESS         512 /* dimension-less */
#define MDC_DIM_ANG_DEG         736 /* degree */
#define MDC_DIM_X_M             1280 /* m */
#define MDC_DIM_X_FOOT          1344 /* ft */
#define MDC_DIM_X_INCH          1376 /* in */
#define MDC_DIM_X_G             1728 /* g */
#define MDC_DIM_X_M_PER_MIN     6560 /* m min-1 */
#define MDC_DIM_YR              2368 /* y */
#define MDC_DIM_X_JOULES        3968 /* J */
#define MDC_DIM_X_WATT          4032 /* W */
#define MDC_DIM_BEAT_PER_MIN    2720 /* bpm */
#define MDC_DIM_RESP_PER_MIN    2784 /* resp min-1 */
#define MDC_DIM_X_STEP          6656 /* */
#define MDC_DIM_X_FOOT_PER_MIN  6688 /* ft per minute */
#define MDC_DIM_X_INCH_PER_MIN  6720 /* inch per minute */
#define MDC_DIM_X_STEP_PER_MIN  6752 /* step per minute */
#define MDC_DIM_X_CAL           6784 /* cal - calories */
#define MDC_DIM_RPM             6816 /* rpm revolutions per minute */

/*********************************************************************************

* From Communication Infrastructure (MDC_PART_INFRA)

**********************************************************************************/

#define MDC_DEV_SPEC_PROFILE_PULS_OXIM          4100 /* 0x1004 */
#define MDC_DEV_SPEC_PROFILE_BP                 4103 /* 0x1007 */
#define MDC_DEV_SPEC_PROFILE_TEMP               4104 /* 0x1008 */
#define MDC_DEV_SPEC_PROFILE_SCALE              4111 /* 0x100F */
#define MDC_DEV_SPEC_PROFILE_GLUCOSE            4113 /* 0x1011 */
#define MDC_DEV_SPEC_PROFILE_HF_CARDIO          4137 /* 0x1029 */
#define MDC_DEV_SPEC_PROFILE_HF_STRENGTH        4138 /* 0x102A */
#define MDC_DEV_SPEC_PROFILE_AI_ACTIVITY_HUB    4167 /* 0x1047 */
#define MDC_DEV_SPEC_PROFILE_AI_MED_MINDER      4168 /* 0x1048 */

/* Placed 256 back from the start of the last Partition: OptionalPackageIdentifiers (i.e. 8192-256). */

#define MDC_TIME_SYNC_NONE      7936 /* 0x1F00 - no time synchronization protocol supported */
#define MDC_TIME_SYNC_NTPV3     7937 /* 0x1F01 - RFC 1305 1992 Mar obs: 1119,1059,958 */
#define MDC_TIME_SYNC_NTPV4     7938 /* 0x1F02 - <under development at ntp.org> */
#define MDC_TIME_SYNC_SNTPV4    7939 /* 0x1F03 - RFC 2030 1996 Oct obs: 1769 */
#define MDC_TIME_SYNC_SNTPV4330 7940 /* 0x1F04 - RFC 4330 2006 Jan obs: 2030,1769 */
#define MDC_TIME_SYNC_BTV1      7941 /* 0x1F05 - Bluetooth Medical Device Profile */

/*********************************************************************************
* From PHD Health and Fitness (MDC_PART_PHD_HF)
**********************************************************************************/
#define MDC_HF_ALT_GAIN 100 /* */
#define MDC_HF_ALT_LOSS 101 /* */
#define MDC_HF_ALT 102 /* */
#define MDC_HF_DISTANCE 103 /* */
#define MDC_HF_ASC_TME_DIST 104 /* */
#define MDC_HF_DESC_TIME_DIST 105 /* */
#define MDC_HF_LATITUDE 106 /* */
#define MDC_HF_LONGITUDE 107 /* */
#define MDC_HF_PROGRAM_ID 108 /* */
#define MDC_HF_SLOPES 109 /* */
#define MDC_HF_SPEED 110 /* */
#define MDC_HF_CAD 111 /* */
#define MDC_HF_INCLINE 112 /* */
#define MDC_HF_HR_MAX_USER 113 /* */
#define MDC_HF_HR 114 /* */
#define MDC_HF_POWER 115 /* */
#define MDC_HF_RESIST 116 /* */
#define MDC_HF_STRIDE 117 /* */
#define MDC_HF_ENERGY 119 /* */
#define MDC_HF_CAL_INGEST 120 /* */
#define MDC_HF_CAL_INGEST_CARB 121 /* */
#define MDC_HF_SUST_PA_THRESHOLD 122 /* */
#define MDC_HF_SESSION 123 /* */
#define MDC_HF_SUBSESSION 124 /* */
#define MDC_HF_ACTIVITY_TIME 125 /* */
#define MDC_HF_AGE 126 /* */
#define MDC_HF_ACTIVITY_INTENSITY 127 /* */
#define MDC_HF_ACT_AMB 1000 /* */
#define MDC_HF_ACT_REST 1001 /* */
#define MDC_HF_ACT_MOTOR 1002 /* */
#define MDC_HF_ACT_LYING 1003 /* */
#define MDC_HF_ACT_SLEEP 1004 /* */
#define MDC_HF_ACT_PHYS 1005 /* */
#define MDC_HF_ACT_SUS_PHYS 1006 /* */
#define MDC_HF_ACT_UNKNOWN 1007 /* */
#define MDC_HF_ACT_MULTIPLE 1008 /* */
#define MDC_HF_ACT_MONITOR 1009 /* */
#define MDC_HF_ACT_SKI 1010 /* */
#define MDC_HF_ACT_RUN 1011 /* */
#define MDC_HF_ACT_BIKE 1012 /* */
#define MDC_HF_ACT_STAIR 1013 /* */
#define MDC_HF_ACT_ROW 1014 /* */
#define MDC_HF_ACT_HOME 1015 /* */
#define MDC_HF_ACT_WORK 1016 /* */
#define MDC_HF_ACT_WALK 1017 /* */
#define MDC_HF_MEAN_NULL_INCLUDE 2000 /* */
#define MDC_HF_MEAN_NULL_EXLCUDE 2001 /* */
#define MDC_HF_MAX 2002 /* */
#define MDC_HF_MIN 2003 /* */

/*********************************************************************************

* From Return Codes (MDC_PART_RET_CODE)

**********************************************************************************/

#define MDC_RET_CODE_UNKNOWN 1 /* Generic error code */
/* Partition MDC_PART_RET_CODE/OBJ Object errors */
#define MDC_RET_CODE_OBJ_BUSY 1000 /* Object is busy so cannot handle the request */
/* Partition MDC_PART_RETURN_CODES/STORE Storage errors */
#define MDC_RET_CODE_STORE_EXH 2000 /* Storage such as disk is full */
#define MDC_RET_CODE_STORE_OFFLN 2001 /* Storage such as disk is offline */


#ifdef __cplusplus
extern "C" }
#endif

#endif /*CSR_BT_PHDC_IEEE_ASN_NOM_CODES_H_*/

