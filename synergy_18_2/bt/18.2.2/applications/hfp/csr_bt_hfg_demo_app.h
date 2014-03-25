#ifndef CSR_BT_HFG_DEMO_APP_H__
#define CSR_BT_HFG_DEMO_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_ui_lib.h"
#include "csr_bt_profiles.h"
#include "csr_bt_ui_strings.h"
#include "csr_message_queue.h"
#include "csr_bt_hfg_prim.h"

#define PROFILE_HFG CsrUtf8StrDup("HFG")

/* UI Index for HFG menu UI  Index             */
/* Main menu */
#define CSR_BT_HFG_MAIN_MENU_UI                0x00
#define CSR_BT_HFG_CALL_SIMULATE_UI                    (CSR_BT_HFG_MAIN_MENU_UI + 0x01)
#define CSR_BT_HFG_AT_CMD_MODE_UI                   (CSR_BT_HFG_CALL_SIMULATE_UI + 0x01)
#define CSR_BT_HFG_CALL_HANDLING_UI                  (CSR_BT_HFG_AT_CMD_MODE_UI + 0x01)
/*#define CSR_BT_HFG_SUPPORTED_FEATURES_UI       (CSR_BT_HFG_CALL_HANDLING_UI + 0x01)*/
#define CSR_BT_HFG_CALL_LIST_MENU_UI                (CSR_BT_HFG_CALL_HANDLING_UI + 0x01)
#define CSR_BT_HFG_C2C_UI                                  (CSR_BT_HFG_CALL_LIST_MENU_UI + 0x01)
#define CSR_BT_HFG_SETTINGS_INDICATORS_UI     (CSR_BT_HFG_C2C_UI + 0x01)
#define CSR_BT_HFG_SCO_ESCO_UI                          (CSR_BT_HFG_SETTINGS_INDICATORS_UI + 0x01)

/*Connect UI index*/
#define CSR_BT_HFG_DISC_DIALOG_UI                    (CSR_BT_HFG_SCO_ESCO_UI + 0x01)
#define CSR_BT_HFG_ALREADY_CON_DIALOG_UI       (CSR_BT_HFG_DISC_DIALOG_UI + 0x01)
#define CSR_BT_HFG_CANCEL_CON_DIALOG_UI         (CSR_BT_HFG_ALREADY_CON_DIALOG_UI + 0x01)
#define CSR_BT_HFG_SUCC_CON_DIALOG_UI                    (CSR_BT_HFG_CANCEL_CON_DIALOG_UI + 0x01)

/* UI Index for the AT CMD mode menu UI Index                */
 #define CSR_BT_HFG_AT_MODE_CFG_CMDS_UI        (CSR_BT_HFG_SUCC_CON_DIALOG_UI + 0x01)

 /* UI Index for changing the call parameteres */
#define CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI     (CSR_BT_HFG_AT_MODE_CFG_CMDS_UI + 0x01)

/*Settings And Indicators menu*/
#define CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI    (CSR_BT_HFG_CALL_LIST_CHNGE_PARM_UI + 0x1)
#define CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI    (CSR_BT_HFG_SETTINGS_IND_GLOBAL_UI+ 0x01)

/* key defines for C2C menu UI  Index                  */
#define CSR_BT_HFG_C2C_CALLER_NAME_UI        (CSR_BT_HFG_SETTINGS_IND_ACTIVE_UI + 0x1)
#define CSR_BT_HFG_C2C_TEXT_STRING_UI        (CSR_BT_HFG_C2C_CALLER_NAME_UI+ 0x01)
#define CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI    (CSR_BT_HFG_C2C_TEXT_STRING_UI + 0x01)

 #define CSR_BT_HFG_SCO_ESCO_INPUT_UI              (CSR_BT_HFG_SETUP_AUDIO_CODEC_SR_UI + 0x01)

/* HFG_DIAL_IND menu UI Index */
#define CSR_BT_HFG_DIAL_IND_UI            (CSR_BT_HFG_SCO_ESCO_INPUT_UI + 0x01)
#define CSR_BT_HFG_DIAL_INFO_DIALOG_UI    (CSR_BT_HFG_DIAL_IND_UI + 0x01)

#define CSR_BT_HFG_DEFAULT_INFO_UI         (CSR_BT_HFG_DIAL_INFO_DIALOG_UI + 0x01)  /*val 26*/
#define CSR_BT_HFG_LDN_INPUT_DIALG_UI      (CSR_BT_HFG_DEFAULT_INFO_UI + 0x01)
#define CSR_BT_HFG_CHANGE_SUP_FEAT_UI    (CSR_BT_HFG_LDN_INPUT_DIALG_UI + 0x01)
#define CSR_BT_HFG_SUP_FEAT_UI            (CSR_BT_HFG_CHANGE_SUP_FEAT_UI + 0x01)
#define NUMBER_OF_CSR_BT_HFG_APP_UI        (CSR_BT_HFG_SUP_FEAT_UI + 0x01)

/* Various limits */
#define MAX_NUM_CONNECTION                   (2)
#define MAX_DISCOVERY_RESULTS_IN_LIST        (10)
#define MAX_CONNECT_TIME                     (10)
#define KEYB_CHECK_TIMEOUT                   (100000)
#define MAX_CALLS                            (10)
#define MAX_TEXT_LENGTH                      (126)
#define NO_CALL                              (0xFF)
#define NO_IDX                               (0xFF)

#define CSR_BT_HFG_APP_BIT_0                        (0x01)
#define CSR_BT_HFG_APP_BIT_1                        (0x02)
#define CSR_BT_HFG_APP_BIT_2                        (0x04)
#define CSR_BT_HFG_APP_BIT_3                        (0x08)
#define CSR_BT_HFG_APP_BIT_4                        (0x10)
#define CSR_BT_HFG_APP_BIT_5                        (0x20)
#define CSR_BT_HFG_APP_BIT_6                        (0x40)
#define CSR_BT_HFG_APP_BIT_7                        (0x80)

#define CSR_BT_LDN_MAX            10
#define CALLER_NAME             10
/* Configuration */
#ifdef CSR_USE_DSPM
    #define CSR_BT_HFG_SUP_FEATURES               (CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING |\
                                                  CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION |\
                                                  CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION |\
                                                  CSR_BT_HFG_SUPPORT_INBAND_RINGING |\
                                                  CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG |\
                                                  CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL |\
                                                  CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS |\
                                                  CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL |\
                                                  CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES |\
                                                  CSR_BT_HFG_SUPPORT_CODEC_NEGOTIATION)
#else
    #define CSR_BT_HFG_SUP_FEATURES               (CSR_BT_HFG_SUPPORT_THREE_WAY_CALLING |\
                                                  CSR_BT_HFG_SUPPORT_EC_NR_FUNCTION |\
                                                  CSR_BT_HFG_SUPPORT_VOICE_RECOGNITION |\
                                                  CSR_BT_HFG_SUPPORT_INBAND_RINGING |\
                                                  CSR_BT_HFG_SUPPORT_ATTACH_NUMBER_TO_VOICE_TAG |\
                                                  CSR_BT_HFG_SUPPORT_ABILITY_TO_REJECT_CALL |\
                                                  CSR_BT_HFG_SUPPORT_ENHANCED_CALL_STATUS |\
                                                  CSR_BT_HFG_SUPPORT_ENHANCED_CALL_CONTROL |\
                                                  CSR_BT_HFG_SUPPORT_EXTENDED_ERROR_CODES)
#endif


#define CSR_BT_HFG_SETUP                      (CSR_BT_HFG_CNF_LP_STATUS |\
                                              CSR_BT_HFG_CNF_AUDIO_STATUS)

#define CSR_BT_HFG_CRH_SETUP                  (0)

/* Command related settings */
#define RING_REP_RATE                        (4)
#define RING_NUM_OF_RINGS                    (255)
#define RING_NUMBER                          "+4599324100"   /* CSR-DK */
#define RING_NUMBER_2                        "+4599324222"
#define SUB_NUMBER_1                         "+4599324100"   /* CSR-DK */
#define SUB_NUMBER_2                         "+441223692000" /* CSR-UK */
#define NUMBER_TYPE_INT                       (145)
#define NUMBER_TYPE_STD                      (129)
#define RING_NUMBER_NAME                     "CSR-DK"        /* CSR-DK */
#define RING_NUMBER_2_NAME                   "CSR-DK-222"
#define SUB_NUMBER_1_NAME                    "SUB CSR-DK"    /* CSR-DK */
#define SUB_NUMBER_2_NAME                    "CSR-UK"        /* CSR-UK */
#define NUMBER_TYPE                          (129)
#define MAX_MIC_GAIN                         (15)
#define MIN_MIC_GAIN                         (0)
#define MAX_SPK_GAIN                         (15)
#define MIN_SPK_GAIN                         (0)
#define OPERATOR_NAME                        "CSR plc."
#define BINP_RES                             "+4599324100"
#define SMS_TEXT                             "This is a SMS\nwith multiple\nlines\n:-)"
#define CSR_BT_HFG_SERVICE_NAME              "Voice gateway (Synergy BT HFG demo)"

#define PCM_SLOT                             (CSR_BT_PCM_DONT_CARE)
#define PCM_SLOT_REALLOCATE                  (TRUE)

/* Various modes for call handling */
#define MOBILE_ORIGINATED                    (0)
#define MOBILE_TERMINATED                    (1)
#define NOT_MULTIPARTY_CALL                  (0)
#define MULTIPARTY_CALL                      (1)
#define VOICE                                (0)
#define DATA                                 (1)
#define FAX                                  (2)
#define UNKNOWN                              (3)

#define CSR_BT_HFG_APP_NUMBER_AT_CMD          6

/* Helper to "invert" a byte */
#define INVERT(b) (b == 0 ? 1 : 0)

/* Demoapp states */
typedef CsrUint8 DemoStates_t;
#define stDeactivated (DemoStates_t) 0x00
#define stIdle (DemoStates_t) 0x01
#define stSecurity (DemoStates_t) 0x02
#define stInquiry (DemoStates_t) 0x03
#define stInquirySelect (DemoStates_t) 0x04
#define stSco (DemoStates_t) 0x05
#define stEditRedial (DemoStates_t) 0x06

typedef CsrUint8 MenuStates_t;
#define menuMain_s (MenuStates_t) 0x00
#define menuC2c_s (MenuStates_t) 0x01
#define menuSetting_s (MenuStates_t) 0x02
#define menuTts_s (MenuStates_t) 0x03
#define menuAdpcm_s (MenuStates_t) 0x04
#define menuName_s (MenuStates_t) 0x05
#define menuFeatures_s (MenuStates_t) 0x06
#define menuCalllist_s (MenuStates_t) 0x07
#define menuDialing_s (MenuStates_t) 0x08
#define menuSco_s (MenuStates_t) 0x09
#define menuCallAlter_s (MenuStates_t) 0x0A
#define menuAtCmdParser_s (MenuStates_t) 0x0B
#define menuAtCmdChoice_s (MenuStates_t) 0x0C

typedef CsrUint8 ScoStates_t;
#define scoTxBandwidth (ScoStates_t) 0x00
#define scoRxBandwidth (ScoStates_t) 0x01
#define scoMaxLatency (ScoStates_t) 0x02
#define scoVoiceSettings (ScoStates_t) 0x03
#define scoAudioQuality (ScoStates_t) 0x04
#define scoReTxEffort (ScoStates_t) 0x05

/* Call status. These defines are local for the demo application and
 * does not have anything particular to do with the CCAP spec! */
typedef CsrUint8 CallStatus_t;
#define CS_ACTIVE      ((CallStatus_t) 0)
#define CS_HELD        ((CallStatus_t) 1)
#define CS_DIALING     ((CallStatus_t) 2)
#define CS_ALERTING    ((CallStatus_t) 3)
#define CS_INCOMING    ((CallStatus_t) 4)
#define CS_WAITING     ((CallStatus_t) 5)

#define CSR_BT_HFG_STD_PRIO                             0x01
#define CSR_BT_HFG_HIGH_PRIO                             0x02

typedef void (* CsrUiHfgEventHandlerFuncType)(void * instData, CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key);

typedef struct
{
    CsrUieHandle                               displayHandle;
    CsrUieHandle                               sk1EventHandle;
    CsrUieHandle                               sk2EventHandle;
    CsrUieHandle                               backEventHandle;
    CsrUieHandle                               deleteEventHandle;
    CsrUiHfgEventHandlerFuncType                sk1EventHandleFunc;
    CsrUiHfgEventHandlerFuncType                sk2EventHandleFunc;
    CsrUiHfgEventHandlerFuncType                deleteEventHandleFunc;
    CsrUiHfgEventHandlerFuncType                backEventHandleFunc;
} CsrBtHfgDisplayHandlesType;

typedef struct
{
    CsrUint8                                 uiIndex;
    CsrUint8                                 eventState;
    CsrSchedTid                                    popupTimerId;
    CsrBool                                  inHfgMenu;
    CsrBool                                  goToMainmenu;
    CsrBtHfgDisplayHandlesType                 displayesHandlers[NUMBER_OF_CSR_BT_HFG_APP_UI];
} CsrBtHfgCsrUiType;

typedef struct
{
    CsrUint32            txBandwidth;
    CsrUint32            rxBandwidth;
    CsrUint16            maxLatency;
    CsrUint16            voiceSettings;
    hci_pkt_type_t      audioQuality;
    CsrUint8             reTxEffort;
} ScoSettings_t;

typedef struct
{
    /* idx is the array index */
    CsrBool              isSet;
    CsrUint8             dir;
    CallStatus_t        status;
    CsrUint8             mode;
    CsrUint8             mpty;
    CsrUint8             number[MAX_TEXT_LENGTH];
    CsrUint8             numType;
} Call_t;

typedef struct
{
    /* Connection identification */
    CsrBtHfgConnectionId index;
    CsrBtHfgConnection   connType;
    CsrBtDeviceAddr        address;
    CsrUint16            scoHandle;
    CsrBool              active;
    CsrBool              audioOn;
    CsrBool              dialOk;

    /* Indicators/settings */
    CsrBool              nrec;
    CsrBool              voiceRecognition;
    CsrBool              inbandRinging;
    CsrUint8             speakerGain;
    CsrUint8             micGain;

    /* Features */
    CsrUint32            hfSupportedFeatures;
    CsrUint16            remoteVersion;
    CsrUint8             c2cIndicators[CSR_BT_C2C_NUMBER_OF_INDICATORS];
} Connection_t;

typedef struct
{
    DemoStates_t        state;
    DemoStates_t        prevState;
    ScoStates_t         scoMenu;
    CsrUint8            callTarget;
    CsrUint8            inputLength;
    CsrUint8            input[MAX_TEXT_LENGTH];
    Connection_t        conInst[MAX_NUM_CONNECTION];
    CsrBtDeviceAddr     bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];
    CsrSchedQid              phandle;
    void                *recvMsgP;
    CsrUint8            numInList;
    CsrUint8            selectedDevice;
    CsrUint32           current;
    Call_t              calls[MAX_CALLS];

    /* Indicator settings */
    CsrUint32           hfgSupportedFeatures;
    CsrUint16           remoteVersion;
    CsrBool             regStatus;
    CsrBool             roamStatus;
    CsrBool             redial;
    CsrBool             rfShield;
    CsrUint8            signal;
    CsrUint8            battery;
    CsrUint8            text[MAX_TEXT_LENGTH];
    CsrUint8            textLen;
    CsrUint8            name[MAX_TEXT_LENGTH];
    CsrUint8            nameLen;
    CsrUint8            c2cIndicators[CSR_BT_C2C_NUMBER_OF_INDICATORS];
    ScoSettings_t       sco;
    CsrUint8            supportedAudioCodecMask;
    CsrUint8            supportedSampleRate;

    /* redial number */
    CsrUint8            redialNumber[MAX_TEXT_LENGTH];
    /* AT parser mode */
    CsrUint8            parserMode;
    CsrUint8            parserCmdList[CSR_BT_HFG_APP_NUMBER_AT_CMD];
    /* support for AT+CLCC command */
    CsrBool             callListCmdSupport;
    CsrBool             copsSupport;
    /* automatic audio transfer to HF upon SLC establishment */
    CsrBool             autoAudioTransfer;
    CsrBtHfgCsrUiType   csrUiVar;
    CsrUint8       scoKey;
    CsrUint8         dialogShow;
    CsrBool         outgoingCall;
    CsrBool          restoreFlag;
    CsrBool          isAnyMenuItem;
    CsrSchedTid          discGuard;               /* Gaurds the disconnect procedure*/
    CsrSchedTid          deactGuard;          /* Gaurds the deactivate  procedure*/
    CsrMessageQueueType * saveQueue;
} hfgInstance_t;


typedef void (* CsrBtHfgEventType)(hfgInstance_t * instData);

/* Forward declarations for handler functions */
void handleKeybPrim(hfgInstance_t *instData);
void handleHfgPrim(hfgInstance_t *instData);
void handleSdPrim(hfgInstance_t *instData);

/* Main functions */
void clearConnection(hfgInstance_t *inst, Connection_t *con);

/* Utility functions */
CsrUint8 callGetUnused(hfgInstance_t *inst);
CsrUint8 callGetCount(hfgInstance_t *inst);
CsrUint8 callGetStatus(hfgInstance_t *inst, CallStatus_t st);
CsrUint8 callChangeStatus(hfgInstance_t *inst, CallStatus_t old, CallStatus_t new);
CsrUint8 callUnsetStatus(hfgInstance_t *inst, CallStatus_t st);
char *callGetStringStatus(hfgInstance_t *inst, CsrUint8 index);
void callUnsetIndex(hfgInstance_t *inst, CsrUint8 i);

Connection_t *getIdConnection(hfgInstance_t *inst, CsrBtHfgConnectionId id);
Connection_t *getAddrConnection(hfgInstance_t *inst, CsrBtDeviceAddr *addr);
Connection_t *getActiveConnection(hfgInstance_t *inst);
Connection_t *getUnusedConnection(hfgInstance_t *inst);
CsrUint8 getIndex(hfgInstance_t *inst, Connection_t *con);
CsrUint8 getNumActive(hfgInstance_t *inst);
void syncSettings(hfgInstance_t *inst);
#ifndef EXCLUDE_CSR_BT_HFG_MODULE_OPTIONAL
void syncScoSettings(hfgInstance_t *inst);
#endif
CsrUint8 *StringDup(char *str);
void initHfgData(hfgInstance_t *inst);


#ifdef __cplusplus
}
#endif

#endif
