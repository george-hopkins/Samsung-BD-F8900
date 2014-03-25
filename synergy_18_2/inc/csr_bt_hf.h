#ifndef CSR_BT_HF_H__
#define CSR_BT_HF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_bt_usr_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Supported HF features (AT+BRSF) */
typedef CsrUint32 CsrBtHfSupportedFeatures;
#define CSR_BT_HF_SUPPORT_EC_NR_FUNCTION                         ((CsrBtHfSupportedFeatures) 0x00000001)
#define CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING         ((CsrBtHfSupportedFeatures) 0x00000002)
#define CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY            ((CsrBtHfSupportedFeatures) 0x00000004)
#define CSR_BT_HF_SUPPORT_VOICE_RECOGNITION                      ((CsrBtHfSupportedFeatures) 0x00000008)
#define CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL                  ((CsrBtHfSupportedFeatures) 0x00000010)
#define CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS                   ((CsrBtHfSupportedFeatures) 0x00000020)
#define CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL                  ((CsrBtHfSupportedFeatures) 0x00000040)
#define CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION                      ((CsrBtHfSupportedFeatures) 0x00000080)

/* Support HFG features (+BRSF) */
typedef CsrUint32 CsrBtHfgSupportedFeatures;
#define CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING                     ((CsrBtHfgSupportedFeatures) 0x00000001)
#define CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION                        ((CsrBtHfgSupportedFeatures) 0x00000002)
#define CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION                     ((CsrBtHfgSupportedFeatures) 0x00000004)
#define CSR_BT_HFG_SUPPORT_INBAND_RINGING                        ((CsrBtHfgSupportedFeatures) 0x00000008)
#define CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG            ((CsrBtHfgSupportedFeatures) 0x00000010)
#define CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL                ((CsrBtHfgSupportedFeatures) 0x00000020)
#define CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS                  ((CsrBtHfgSupportedFeatures) 0x00000040)
#define CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL                 ((CsrBtHfgSupportedFeatures) 0x00000080)
#define CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES                  ((CsrBtHfgSupportedFeatures) 0x00000100)
#define CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION                     ((CsrBtHfgSupportedFeatures) 0x00000200)

#define CSR_BT_HFP_SDP_SUPPORT_MASK                              ((CsrBtHfgSupportedFeatures) 0x0000001F)
/* Codec negotiation bit in the SDP database */
#define CSR_BT_HFP_SDP_CODEC_NEGOTIATION                         (0x20)

/* Indicators used in the CIEV (status indication) command */
#define CSR_BT_SERVICE_INDICATOR                                 ((CsrUint8) 1)
#define CSR_BT_CALL_STATUS_INDICATOR                             ((CsrUint8) 2)
#define CSR_BT_CALL_SETUP_STATUS_INDICATOR                       ((CsrUint8) 3)
#define CSR_BT_CALL_HELD_INDICATOR                               ((CsrUint8) 4)
#define CSR_BT_SIGNAL_STRENGTH_INDICATOR                         ((CsrUint8) 5)
#define CSR_BT_ROAM_INDICATOR                                    ((CsrUint8) 6)
#define CSR_BT_BATTERY_CHARGE_INDICATOR                          ((CsrUint8) 7)
#define CSR_BT_CIEV_NUMBER_OF_INDICATORS                         ((CsrUint8) 8)
#define CSR_BT_GATHERED_CALL_INDICATORS                          ((CsrUint8) 9)

/* CIEV command - values used for SERVICE_INDICATOR */
#define CSR_BT_NO_SERVICE_VALUE                                  ((CsrUint8) 0)
#define CSR_BT_SERVICE_PRESENT_VALUE                             ((CsrUint8) 1)

/* CIEV command - values used for CALL_STATUS_INDICATOR */
#define CSR_BT_NO_CALL_ACTIVE_VALUE                              ((CsrUint8) 0)
#define CSR_BT_CALL_ACTIVE_VALUE                                 ((CsrUint8) 1)

/* CIEV command - values used for CALL_SETUP_INDICATOR */
#define CSR_BT_NO_CALL_SETUP_VALUE                               ((CsrUint8) 0)
#define CSR_BT_INCOMING_CALL_SETUP_VALUE                         ((CsrUint8) 1)
#define CSR_BT_OUTGOING_CALL_SETUP_VALUE                         ((CsrUint8) 2)
#define CSR_BT_OUTGOING_REMOTE_ALERT_VALUE                       ((CsrUint8) 3)

/* CIEV command - values used for SIGNAL_STRENGTH_INDICATOR */
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_0                           ((CsrUint8) 0)
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_1                           ((CsrUint8) 1)
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_2                           ((CsrUint8) 2)
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_3                           ((CsrUint8) 3)
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_4                           ((CsrUint8) 4)
#define CSR_BT_SIGNAL_STRENGTH_LEVEL_5                           ((CsrUint8) 5)

/* CIEV command - values used for ROAM_INDICATOR */
#define CSR_BT_ROAM_OFF                                          ((CsrUint8) 0)
#define CSR_BT_ROAM_ON                                           ((CsrUint8) 1)

/* CIEV command - values used for BATTERY_CHARGE_INDICATOR */
#define CSR_BT_BATTERY_CHARGE_LEVEL_0                            ((CsrUint8) 0)
#define CSR_BT_BATTERY_CHARGE_LEVEL_1                            ((CsrUint8) 1)
#define CSR_BT_BATTERY_CHARGE_LEVEL_2                            ((CsrUint8) 2)
#define CSR_BT_BATTERY_CHARGE_LEVEL_3                            ((CsrUint8) 3)
#define CSR_BT_BATTERY_CHARGE_LEVEL_4                            ((CsrUint8) 4)
#define CSR_BT_BATTERY_CHARGE_LEVEL_5                            ((CsrUint8) 5)

/* CIEV command - values used for CALL_HELD_INDICATOR */
#define CSR_BT_NO_CALL_HELD_VALUE                                ((CsrUint8) 0)
#define CSR_BT_CALL_HELD_RETRIEVE_OTHER_CALL_VALUE               ((CsrUint8) 1)
#define CSR_BT_CALL_HELD_NO_ACTIVE_CALL_VALUE                    ((CsrUint8) 2)

/* CIEV command - values used for GATHERED_CALL_INDICATORS */
#define CSR_BT_NO_CALL_ACTIVE_NO_CALL_SETUP                      ((CsrUint8) 0)
#define CSR_BT_CALL_ACTIVE_NO_CALL_SETUP                         ((CsrUint8) 1)
#define CSR_BT_CALL_ACTIVE_CALL_SETUP                            ((CsrUint8) 2)

/* defines realted to GATHERED_CALL_INDICATORS */
#define CSR_BT_SERVICE_INDICATOR_CHANGED                         ((CsrUint8) 0x01)
#define CSR_BT_CALL_INDICATOR_CHANGED                            ((CsrUint8) 0x02)
#define CSR_BT_CALL_SETUP_INDICATOR_CHANGED                      ((CsrUint8) 0x04)
#define CSR_BT_CALL_HELD_INDICATOR_CHANGED                       ((CsrUint8) 0x08)
#define CSR_BT_SIGNAL_INDICATOR_CHANGED                          ((CsrUint8) 0x10)
#define CSR_BT_ROAM_INDICATOR_CHANGED                            ((CsrUint8) 0x20)
#define CSR_BT_BATTERY_INDICATOR_CHANGED                         ((CsrUint8) 0x40)

/* defines for WBS */
#define CSR_BT_WBS_INVALID_CODEC                                 ((CsrUint8) 0x00)
#define CSR_BT_WBS_CVSD_CODEC                                    ((CsrUint8) 0x01)
#define CSR_BT_WBS_MSBC_CODEC                                    ((CsrUint8) 0x02)

#define CSR_BT_WBS_CVSD_CODEC_MASK                               ((CsrUint32) 0x00000001)
#define CSR_BT_WBS_MSBC_CODEC_MASK                               ((CsrUint32) 0x00000002)

/* CSR-2-CSR vendor specific indicators */
#define CSR_BT__UNUSED_C2C_IND                                   ((CsrUint8) 0)
#define CSR_BT_C2C_NAME_IND                                      ((CsrUint8) 1)
#define CSR_BT_C2C_TXT_IND                                       ((CsrUint8) 2)
#define CSR_BT_C2C_SMS_IND                                       ((CsrUint8) 3)
#define CSR_BT_C2C_BAT_IND                                       ((CsrUint8) 4)
#define CSR_BT_C2C_PWR_IND                                       ((CsrUint8) 5)
#define CSR_BT_C2C_ADPCM_IND                                     ((CsrUint8) 6)
#define CSR_BT_C2C_SAMPLE_RATE_IND                               ((CsrUint8) 7)
#define CSR_BT_C2C_NUMBER_OF_INDICATORS                          ((CsrUint8) 8)

/* C2C command - values for C2C_NAME_IND */
#define CSR_BT_C2C_NAME_OFF_VALUE                                ((CsrUint8) 0)
#define CSR_BT_C2C_NAME_ON_VALUE                                 ((CsrUint8) 1)

/* C2C command - values for C2C_TXT_IND */
#define CSR_BT_C2C_TXT_OFF_VALUE                                 ((CsrUint8) 0)
#define CSR_BT_C2C_TXT_ON_VALUE                                  ((CsrUint8) 1)

/* C2C command - values for C2C_SMS_IND */
#define CSR_BT_C2C_SMS_OFF_VALUE                                 ((CsrUint8) 0)
#define CSR_BT_C2C_SMS_ON_VALUE                                  ((CsrUint8) 1)

/* C2C command - values for C2C_BAT_IND */
#define CSR_BT_C2C_BAT_OFF_VALUE                                 ((CsrUint8) 0)
#define CSR_BT_C2C_BAT_ON_VALUE                                  ((CsrUint8) 1)

/* C2C command - values for C2C_PWR_IND */
#define CSR_BT_C2C_PWR_OFF_VALUE                                 ((CsrUint8) 0)
#define CSR_BT_C2C_PWR_ON_VALUE                                  ((CsrUint8) 1)

/* C2C command - values for C2C_ADPCM_IND */
typedef CsrUint8         CsrBtC2cCodecType;
#define CSR_BT_C2C_ADPCM_OFF_VALUE  /*CVSD*/                     ((CsrBtC2cCodecType) 0x01)
#define CSR_BT_C2C_ADPCM_2BIT_VALUE                              ((CsrBtC2cCodecType) 0x02)
#define CSR_BT_C2C_ADPCM_4BIT_VALUE                              ((CsrBtC2cCodecType) 0x04)

/* C2C command - values for C2C_SAMPLE_RATE_IND */
typedef CsrUint8         CsrBtC2cSamplaRateType;
#define CSR_BT_C2C_SAMPLE_8KHZ_VALUE                             ((CsrBtC2cSamplaRateType) 0x01)
#define CSR_BT_C2C_SAMPLE_16KHZ_VALUE                            ((CsrBtC2cSamplaRateType) 0x02)

/* Call handling (AT+CHLD) commands from headset */
typedef CsrUint8         CsrBtCallHandlingCommand;
#define CSR_BT_RELEASE_ALL_HELD_CALL                             ((CsrBtCallHandlingCommand) 0) /* AT+CHLD=0  */
#define CSR_BT_RELEASE_ACTIVE_ACCEPT                             ((CsrBtCallHandlingCommand) 1) /* AT+CHLD=1  */
#define CSR_BT_RELEASE_SPECIFIED_CALL                            ((CsrBtCallHandlingCommand) 2) /* AT+CHLD=1x */
#define CSR_BT_HOLD_ACTIVE_ACCEPT                                ((CsrBtCallHandlingCommand) 3) /* AT+CHLD=2  */
#define CSR_BT_REQUEST_PRIVATE_WITH_SPECIFIED                    ((CsrBtCallHandlingCommand) 4) /* AT+CHLD=2x */
#define CSR_BT_ADD_CALL                                          ((CsrBtCallHandlingCommand) 5) /* AT+CHLD=3  */
#define CSR_BT_CONNECT_TWO_CALLS                                 ((CsrBtCallHandlingCommand) 6) /* AT+CHLD=4  */

/* Response/hold (AT+BTRH) commands from headset. These must be a
 * continuation of the AT+CHLD commands as they are are combined in
 * the CSR_BT_HFG_CALL_HANDLING_IND and CSR_BT_HF_CALL_HANDLING_REQ signals */
#define CSR_BT_BTRH_PUT_ON_HOLD                                  ((CsrBtCallHandlingCommand) 7) /* AT+BTRH=0 */
#define CSR_BT_BTRH_ACCEPT_INCOMING                              ((CsrBtCallHandlingCommand) 8) /* AT+BTRH=1 */
#define CSR_BT_BTRH_REJECT_INCOMING                              ((CsrBtCallHandlingCommand) 9) /* AT+BTRH=2 */
#define CSR_BT_BTRH_READ_STATUS                                  ((CsrBtCallHandlingCommand) 10) /* AT+BTRH? query status */

/* Call handling (+BTRH) events from AG */
typedef CsrUint8         CsrBtCallHandlingEvent;
#define CSR_BT_BTRH_INCOMING_ON_HOLD                             ((CsrBtCallHandlingEvent) 0) /* +BTRH:0 */
#define CSR_BT_BTRH_INCOMING_ACCEPTED                            ((CsrBtCallHandlingEvent) 1) /* +BTRH:1 */
#define CSR_BT_BTRH_INCOMING_REJECTED                            ((CsrBtCallHandlingEvent) 2) /* +BTRH:2 */

/* AT command response */
#define CSR_BT_AT_COMMAND_MALFORMED                              (0)
#define CSR_BT_AT_COMMAND_OK                                     (1)

/* AT command exchange timer in seconds */
#define CSR_BT_AT_DEFAULT_RESPONSE_TIME                          (2)

/* AT tokens */
#define CSR_BT_ATA_TOKEN                                         (0)
#define CSR_BT_ATD_TOKEN                                         (1)
#define CSR_BT_BINP_TOKEN                                        (2)
#define CSR_BT_BLDN_TOKEN                                        (3)
#define CSR_BT_BRSF_TOKEN                                        (4)
#define CSR_BT_BRSF_STATUS_TOKEN                                 (5)
#define CSR_BT_BTRH_QUERY_TOKEN                                  (6)
#define CSR_BT_BTRH_REQUEST_TOKEN                                (7)
#define CSR_BT_BTRH_STATUS_TOKEN                                 (8)
#define CSR_BT_BVRA_TOKEN                                        (9)
#define CSR_BT_CHLD_TOKEN                                        (10)
#define CSR_BT_CCWA_TOKEN                                        (11)
#define CSR_BT_CHLD_REQUEST_TOKEN                                (12)
#define CSR_BT_CHLD_QUESTION_TOKEN                               (13)
#define CSR_BT_CHLD_STATUS_TOKEN                                 (14)
#define CSR_BT_CHUP_TOKEN                                        (15)
#define CSR_BT_CIEV_TOKEN                                        (16)
#define CSR_BT_CIND_SUPPORT_TOKEN                                (17)
#define CSR_BT_CIND_STATUS_TOKEN                                 (18)
#define CSR_BT_CIND_QUESTION_TOKEN                               (19)
#define CSR_BT_CKPD_TOKEN                                        (20)
#define CSR_BT_CLIP_TOKEN                                        (21)
#define CSR_BT_CLCC_QUERY_TOKEN                                  (22)
#define CSR_BT_CLCC_STATUS_TOKEN                                 (23)
#define CSR_BT_CMEE_TOKEN                                        (24)
#define CSR_BT_CMEE_REQUEST_TOKEN                                (25)
#define CSR_BT_CMEE_STATUS_TOKEN                                 (26)
#define CSR_BT_CMER_TOKEN                                        (27)
#define CSR_BT_CNUM_QUESTION_TOKEN                               (28)
#define CSR_BT_CNUM_STATUS_TOKEN                                 (29)
#define CSR_BT_COPS_QUESTION_TOKEN                               (30)
#define CSR_BT_COPS_REQUEST_TOKEN                                (31)
#define CSR_BT_COPS_TOKEN                                        (32)
#define CSR_BT_NREC_TOKEN                                        (33)
#define CSR_BT_VGM_TOKEN                                         (34)
#define CSR_BT_VGS_TOKEN                                         (35)
#define CSR_BT_VTS_TOKEN                                         (36)
#define CSR_BT_OK_TOKEN                                          (50)
#define CSR_BT_ERROR_TOKEN                                       (51)
#define CSR_BT_RING_TOKEN                                        (52)
#define CSR_BT_ATD_MEMORY_DIAL_TOKEN                             (53)
#define CSR_BT_BIA_TOKEN                                         (54)
#define CSR_BT_BSIR_TOKEN                                        (55)
#define CSR_BT_BAC_TOKEN                                         (56)
#define CSR_BT_BCC_TOKEN                                         (57)
#define CSR_BT_BCS_TOKEN                                         (58)


/* CSR Extension tokens */
#define CSR_BT_CSR_SF_TOKEN                                      (80)
#define CSR_BT_CSR_TOKEN                                         (81)
#define CSR_BT_CSR_BAT_TOKEN                                     (82)
#define CSR_BT_CSR_PWR_TOKEN                                     (83)
#define CSR_BT_CSR_GETSMS_TOKEN                                  (84)
#define CSR_BT_CSR_FN_TOKEN                                      (85)
#define CSR_BT_CSR_SMS_TOKEN                                     (86)
#define CSR_BT_CSR_TXT_TOKEN                                     (87)

/* Special HFP-related tokens */
#define CSR_BT_BUSY_TOKEN                                        (100)
#define CSR_BT_NO_CARRIER_TOKEN                                  (101)
#define CSR_BT_NO_ANSWER_TOKEN                                   (102)
#define CSR_BT_DELAYED_TOKEN                                     (103)
#define CSR_BT_BLACKLISTED_TOKEN                                 (104)

#define CSR_BT_OTHER_TOKEN                                       (0xFF)

/* CME ERRORS */
typedef CsrUint16        CsrBtCmeeResultCode;
#define CSR_BT_CME_AG_FAILURE                                    ((CsrBtCmeeResultCode) 0)
#define CSR_BT_CME_NO_CONNECTION_TO_PHONE                        ((CsrBtCmeeResultCode) 1)
#define CSR_BT_CME_OPERATION_NOT_ALLOWED                         ((CsrBtCmeeResultCode) 3)
#define CSR_BT_CME_OPERATION_NOT_SUPPORTED                       ((CsrBtCmeeResultCode) 4)
#define CSR_BT_CME_PH_SIM_PIN_REQUIRED                           ((CsrBtCmeeResultCode) 5)
#define CSR_BT_CME_PH_FSIM_PIN                                   ((CsrBtCmeeResultCode) 6)   /* not defined in spec */
#define CSR_BT_CME_PH_FSIM_PUK                                   ((CsrBtCmeeResultCode) 7)   /* not defined in spec */
#define CSR_BT_CME_SIM_NOT_INSERTED                              ((CsrBtCmeeResultCode) 10)
#define CSR_BT_CME_SIM_PIN_REQUIRED                              ((CsrBtCmeeResultCode) 11)
#define CSR_BT_CME_SIM_PUK_REQUIRED                              ((CsrBtCmeeResultCode) 12)
#define CSR_BT_CME_SIM_FAILURE                                   ((CsrBtCmeeResultCode) 13)
#define CSR_BT_CME_SIM_BUSY                                      ((CsrBtCmeeResultCode) 14)
#define CSR_BT_CME_SIM_WRONG                                     ((CsrBtCmeeResultCode) 15)  /* not defined in spec */
#define CSR_BT_CME_INCORRECT_PASSWORD                            ((CsrBtCmeeResultCode) 16)
#define CSR_BT_CME_SIM_PIN2_REQUIRED                             ((CsrBtCmeeResultCode) 17)
#define CSR_BT_CME_PUK2_REQUIRED                                 ((CsrBtCmeeResultCode) 18)
#define CSR_BT_CME_MEMORY_FULL                                   ((CsrBtCmeeResultCode) 20)
#define CSR_BT_CME_INVALID_INDEX                                 ((CsrBtCmeeResultCode) 21)
#define CSR_BT_CME_NOT_FOUND                                     ((CsrBtCmeeResultCode) 22)  /* not defined in spec */
#define CSR_BT_CME_MEMORY_FAILURE                                ((CsrBtCmeeResultCode) 23)
#define CSR_BT_CME_TEXT_STRING_TOO_LONG                          ((CsrBtCmeeResultCode) 24)
#define CSR_BT_CME_INVALID_CHARACTERS_IN_TEXT_STRING             ((CsrBtCmeeResultCode) 25)
#define CSR_BT_CME_DIAL_STRING_TOO_LONG                          ((CsrBtCmeeResultCode) 26)
#define CSR_BT_CME_INVALID_CHARACTERS_IN_DIAL_STRING             ((CsrBtCmeeResultCode) 27)
#define CSR_BT_CME_NO_NETWORK_SERVICE                            ((CsrBtCmeeResultCode) 30)
#define CSR_BT_CME_NETWORK_TIMEOUT                               ((CsrBtCmeeResultCode) 31)  /* not defined in spec */
#define CSR_BT_CME_NETWORK_NOT_ALLOWED_EMERGENCY_CALLS_ONLY      ((CsrBtCmeeResultCode) 32)
#define CSR_BT_CME_NETWORK_PERSONALIZATION_PIN_REQUIRED          ((CsrBtCmeeResultCode) 40)  /* not defined in spec */
#define CSR_BT_CME_NETWORK_PERSONALIZATION_PUK_REQUIRED          ((CsrBtCmeeResultCode) 41)  /* not defined in spec */
#define CSR_BT_CME_NETWORK_SUBSET_PERSONALIZATION_PIN_REQUIRED   ((CsrBtCmeeResultCode) 42)  /* not defined in spec */
#define CSR_BT_CME_NETWORK_SUBSET_PERSONALIZATION_PUK_REQUIRED   ((CsrBtCmeeResultCode) 43)  /* not defined in spec */
#define CSR_BT_CME_SERVICE_PROVIDER_PERSONALIZATION_PIN_REQUIRED ((CsrBtCmeeResultCode) 44)  /* not defined in spec */
#define CSR_BT_CME_SERVICE_PROVIDER_PERSONALIZATION_PUK_REQUIRED ((CsrBtCmeeResultCode) 45)  /* not defined in spec */
#define CSR_BT_CME_CORPORATE_PERSONALIZATION_PIN_REQUIRED        ((CsrBtCmeeResultCode) 46)  /* not defined in spec */
#define CSR_BT_CME_CORPORATE_PERSONALIZATION_PUK_REQUIRED        ((CsrBtCmeeResultCode) 47)  /* not defined in spec */
#define CSR_BT_CME_HIDDEN_KEY_REQUIRED                           ((CsrBtCmeeResultCode) 48)  /* not defined in spec */
#define CSR_BT_CME_EAP_METHOD_NOT_SUPPORTED                      ((CsrBtCmeeResultCode) 49)  /* not defined in spec */
#define CSR_BT_CME_INCORRECT_PARAMETERS                          ((CsrBtCmeeResultCode) 50)  /* not defined in spec */
#define CSR_BT_CME_UNKNOWN                                       ((CsrBtCmeeResultCode) 100) /* not defined in spec */
#define CSR_BT_CME_BUSY                                          ((CsrBtCmeeResultCode) 101) /* not defined in GSM spec */
#define CSR_BT_CME_NO_CARRIER                                    ((CsrBtCmeeResultCode) 102) /* not defined in GSM spec */
#define CSR_BT_CME_NO_ANSWER                                     ((CsrBtCmeeResultCode) 103) /* not defined in GSM spec */
#define CSR_BT_CME_DELAYED                                       ((CsrBtCmeeResultCode) 104) /* not defined in GSM spec */
#define CSR_BT_CME_BLACKLISTED                                   ((CsrBtCmeeResultCode) 105) /* not defined in GSM spec */
#define CSR_BT_CME_SUCCESS                                       ((CsrBtCmeeResultCode) 0xffff) /* not defined in spec, internal CSR */

#define CSR_BT_SCO_DEFAULT_1P1                                   (1)
#define CSR_BT_ESCO_DEFAULT_1P2_S1                               (2)
#define CSR_BT_ESCO_DEFAULT_2P0_S2                               (3)
#define CSR_BT_ESCO_DEFAULT_2P0_S3                               (4)
#define CSR_BT_ESCO_DEFAULT_CONNECT                              (5)
#define CSR_BT_ESCO_DEFAULT_T2                                   (6)
#define CSR_BT_ESCO_DEFAULT_T1                                   (7)
#define CSR_BT_SCO_ATTEMPTS_EXHAUSTED                            (8)


/* eSCO defines */
#define CSR_BT_FIRST_HFP_NO_ESCO                                 (0x0096) /* The first version of the HFP profile with no eSCO support: 00.96 */
#define CSR_BT_LAST_HFP_NO_ESCO                                  (0x0101) /* The last version of the HFP profile with no eSCO support: 01.01 */
#define CSR_BT_FIRST_HFP_ESCO                                    (0x0105) /* the first version of the HFP profile supporting esco was 01.05 */
#define CSR_BT_FIRST_HFP_CODEC_NEG_ESCO                          (0x0105) /* the first version of the HFP profile supporting codec negotiation 01.06 */

#define CSR_BT_ESCO_DEFAULT_2P0_S3_TX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_2P0_S3_RX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_2P0_S3_MAX_LATENCY                   (0x000A)
#define CSR_BT_ESCO_DEFAULT_2P0_S3_VOICE_SETTINGS                (CSR_BT_AIRCODING_CVSD | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_ESCO_DEFAULT_2P0_S3_RE_TX_EFFORT                  (HCI_ESCO_POWER_SAVING_RETX)
#define CSR_BT_ESCO_DEFAULT_2P0_S3_AUDIO_QUALITY                 (0x03C0 & (~HCI_ESCO_PKT_2EV3))

#define CSR_BT_ESCO_DEFAULT_2P0_S2_TX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_2P0_S2_RX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_2P0_S2_MAX_LATENCY                   (0x0007)
#define CSR_BT_ESCO_DEFAULT_2P0_S2_VOICE_SETTINGS                (CSR_BT_AIRCODING_CVSD | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_ESCO_DEFAULT_2P0_S2_RE_TX_EFFORT                  (HCI_ESCO_POWER_SAVING_RETX)
#define CSR_BT_ESCO_DEFAULT_2P0_S2_AUDIO_QUALITY                 (0x03C0 & (~HCI_ESCO_PKT_2EV3))

#define CSR_BT_ESCO_DEFAULT_1P2_S1_TX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_1P2_S1_RX_BANDWIDTH                  (8000)
#define CSR_BT_ESCO_DEFAULT_1P2_S1_MAX_LATENCY                   (0x0007)
#define CSR_BT_ESCO_DEFAULT_1P2_S1_VOICE_SETTINGS                (CSR_BT_AIRCODING_CVSD | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_ESCO_DEFAULT_1P2_S1_RE_TX_EFFORT                  (HCI_ESCO_NO_RETX) /*(HCI_ESCO_POWER_SAVING_RETX)*/
#define CSR_BT_ESCO_DEFAULT_1P2_S1_AUDIO_QUALITY                 (HCI_ESCO_PKT_EV3 | 0x03C0)

/* WBS defines */
#define CSR_BT_ESCO_DEFAULT_T2_TX_BANDWIDTH                      (8000)
#define CSR_BT_ESCO_DEFAULT_T2_RX_BANDWIDTH                      (8000)
#define CSR_BT_ESCO_DEFAULT_T2_MAX_LATENCY                       (0x000D)
#define CSR_BT_ESCO_DEFAULT_T2_VOICE_SETTINGS                    (CSR_BT_AIRCODING_TRANSPARENT_DATA | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_ESCO_DEFAULT_T2_AUDIO_QUALITY                     (0x03C0 & (~HCI_ESCO_PKT_2EV3))
#define CSR_BT_ESCO_DEFAULT_T2_RE_TX_EFFORT                      (HCI_ESCO_LINK_QUALITY_RETX)

#define CSR_BT_ESCO_DEFAULT_T1_TX_BANDWIDTH                      (8000)
#define CSR_BT_ESCO_DEFAULT_T1_RX_BANDWIDTH                      (8000)
#define CSR_BT_ESCO_DEFAULT_T1_MAX_LATENCY                       (0x0008)
#define CSR_BT_ESCO_DEFAULT_T1_VOICE_SETTINGS                    (CSR_BT_AIRCODING_TRANSPARENT_DATA | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_ESCO_DEFAULT_T1_AUDIO_QUALITY                     (HCI_ESCO_PKT_EV3 | 0x03C0)
#define CSR_BT_ESCO_DEFAULT_T1_RE_TX_EFFORT                      (HCI_ESCO_LINK_QUALITY_RETX)

/* Regular SCO */
#define CSR_BT_SCO_DEFAULT_1P1_TX_BANDWIDTH                      (8000)
#define CSR_BT_SCO_DEFAULT_1P1_RX_BANDWIDTH                      (8000)
#define CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY                       (0x0005)
#define CSR_BT_SCO_DEFAULT_1P1_VOICE_SETTINGS                    (CSR_BT_AIRCODING_CVSD | CSR_BT_VOICE_SETTING_INPUT)
#define CSR_BT_SCO_DEFAULT_1P1_RE_TX_EFFORT                      (HCI_ESCO_NO_RETX)  /* Retransmissions not supported on simple SCO */
#define CSR_BT_SCO_DEFAULT_1P1_AUDIO_QUALITY                     (HCI_ESCO_PKT_HV3 | HCI_ESCO_PKT_HV2 | HCI_ESCO_PKT_HV1 | 0x03C0)

/* Audio manager related defines: audio endpoints, configuration possibilities, etc... */
#define CSR_BT_FIRST_USE_AM_BUILD_ID   0x0000  /*6817*/ /* BC7 with DSP support for WBS */

#ifdef CSR_USE_DSPM

/* Audio streaming definitions */
/* Connection Point Type */
typedef CsrUint16 CsrBtHfpDspmCpType;
#define CSR_BT_HFP_DSPM_CP_TYPE_PCM        ((CsrBtHfpDspmCpType) 1)
#define CSR_BT_HFP_DSPM_CP_TYPE_I2S        ((CsrBtHfpDspmCpType) 2)
#define CSR_BT_HFP_DSPM_CP_TYPE_CODEC      ((CsrBtHfpDspmCpType) 3)
#define CSR_BT_HFP_DSPM_CP_TYPE_FM         ((CsrBtHfpDspmCpType) 4)
#define CSR_BT_HFP_DSPM_CP_TYPE_SPDIF      ((CsrBtHfpDspmCpType) 5)
#define CSR_BT_HFP_DSPM_CP_TYPE_DM         ((CsrBtHfpDspmCpType) 6)
#define CSR_BT_HFP_DSPM_CP_TYPE_A2DP       ((CsrBtHfpDspmCpType) 7)
#define CSR_BT_HFP_DSPM_CP_TYPE_FASTPIPE   ((CsrBtHfpDspmCpType) 8)
#define CSR_BT_HFP_DSPM_CP_TYPE_SCO        ((CsrBtHfpDspmCpType) 9)

/* definition of DSP currently not pending */
#define DSPM_NOP                             0xFF

#endif

#ifdef __cplusplus
}
#endif

#endif

