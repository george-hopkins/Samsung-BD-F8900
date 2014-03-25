#ifndef CSR_BT_HF_DEMO_APP_H__
#define CSR_BT_HF_DEMO_APP_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "csr_bt_hf.h"

/*  profile manager include files */
#include "csr_types.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_ui_lib.h"
#include  "csr_bt_hf_prim.h"

#define PROFILE_HF CsrUtf8StrDup("HF")

/*UI index for HF menu items*/
#define CSR_BT_HF_DEFAULT_INFO_UI         (0x00)
#define CSR_BT_HF_REACTIVATE_SERVER_UI             (CSR_BT_HF_DEFAULT_INFO_UI + 0x01)
#define CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI         (CSR_BT_HF_REACTIVATE_SERVER_UI + 0x01)
#define CSR_BT_ENH_CALL_HANDLING_MENU_UI           (CSR_BT_REQUEST_SCO_SPL_SETTINGS_UI + 0x01)
#define CSR_BT_RESPONSE_HOLD_OPTIONS_UI            (CSR_BT_ENH_CALL_HANDLING_MENU_UI + 0x01)
#define CSR_BT_CALL_HANDLING_MENU_UI               (CSR_BT_RESPONSE_HOLD_OPTIONS_UI + 0x01)
#define CSR_BT_CHANGE_AUDIO_CODEC_UI               (CSR_BT_CALL_HANDLING_MENU_UI + 0x01)
#define CSR_BT_C2C_MENU_UI                         (CSR_BT_CHANGE_AUDIO_CODEC_UI + 0x01)
#define CSR_BT_COD_SELECTION_MENU_UI               (CSR_BT_C2C_MENU_UI + 0x01)
#define CSR_BT_IND_INDICATOR_ACT_UI                (CSR_BT_COD_SELECTION_MENU_UI + 0x01)
#define CSR_BT_CHOOSE_ACTIVE_CON_UI                (CSR_BT_IND_INDICATOR_ACT_UI + 0x01)
#define CSR_BT_HF_MAIN_MENU_UI                (CSR_BT_CHOOSE_ACTIVE_CON_UI + 0x01)
#define CSR_BT_HF_CALL_LIST_UI                (CSR_BT_HF_MAIN_MENU_UI + 0x01)
#define CSR_BT_HF_CHLD_SPECIFIC_UI                (CSR_BT_HF_CALL_LIST_UI + 0x01)
#define CSR_BT_HF_DEFAULT_INPUT_UI                (CSR_BT_HF_CHLD_SPECIFIC_UI + 0x01)
#define NUMBER_OF_CSR_BT_HF_APP_UI        (CSR_BT_HF_DEFAULT_INPUT_UI + 0x01)

/* Defines for PTS RF-shielding */
#define ENABLE_TX_VARID  0x4007
#define DISABLE_TX_VARID 0x4008

#define PRINT_AT_CMD                (1)
#define PRINT_AT_CMD_VERBOSE        (1)
#undef INCLUDE_SC_HANDLER
/*  The time to wait for bonding and connection in number of seconds */
#define KEYB_CHECK_TIMEOUT          (200000)

/*  the number of rings and the time between consecutive rings */
#define SPK_VOLUME_CHANGE_GAIN      (5)
#define MIC_VOLUME_CHANGE_GAIN      (5)
#define MAX_MIC_GAIN                (15)
#define MIN_MIC_GAIN                (0)
#define MAX_SPK_GAIN                (15)
#define MIN_SPK_GAIN                (0)

/* SCO/eSCO parameters */
#define SCO_PACKET_TYPE             (0)
#define SCO_TX_BANDWIDTH            (0)
#define SCO_RX_BANDWIDTH            (0)
#define SCO_MAX_LATENCY             (CSR_BT_SCO_DEFAULT_1P1_MAX_LATENCY)
#define SCO_VOICE_SETTINGS          (0)
#define SCO_RETRANSMISSION_EFFORT   (HCI_ESCO_NO_RETX)

#define PCM_SLOT                    (CSR_BT_PCM_DONT_CARE)
#define PCM_SLOT_REALLOCATE         (FALSE)

#define SCO_HANDLE_UNUSED           (0xFFFF)

#define TO_UPPER(x)                 ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x)            ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)

#define MAX_NUM_CONNECTION          10

#define STARTUP_MIC                 0
#define STARTUP_DONE                2

#define LINKTYPE_SCO                "SCO"
#define LINKTYPE_ACL                "ACL"
#define LINKTYPE_ESCO               "eSCO"
#define LINKTYPE_RESERVED           "Resrved for future"

#ifdef CSR_USE_DSPM
    #define CSR_BT_HF_SUPPORT_ALL_FUNCTIONS    (CSR_BT_HF_SUPPORT_EC_NR_FUNCTION | CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING | CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY | CSR_BT_HF_SUPPORT_VOICE_RECOGNITION | CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL | CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS | CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL | CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION)
    #define CSR_BT_HF_SUPPORT_ALL_BUT_VR       (CSR_BT_HF_SUPPORT_EC_NR_FUNCTION | CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING | CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY | CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL | CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS | CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL | CSR_BT_HF_SUPPORT_CODEC_NEGOTIATION)
#else
    #define CSR_BT_HF_SUPPORT_ALL_FUNCTIONS    (CSR_BT_HF_SUPPORT_EC_NR_FUNCTION | CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING | CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY | CSR_BT_HF_SUPPORT_VOICE_RECOGNITION | CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL | CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS | CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL)
    #define CSR_BT_HF_SUPPORT_ALL_BUT_VR       (CSR_BT_HF_SUPPORT_EC_NR_FUNCTION | CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING | CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY | CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL | CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS | CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL)
#endif

typedef struct
{
    CsrUint8         length;
    unsigned char   phoneNumber[50];
} phoneNumber_t;

typedef struct
{
    CsrUint8         ATCommandLen;
    unsigned char   ATCommand[128];
}ATCommand_t;

typedef struct
{
    CsrUint8         index;
}callIdentity_t;

/*  demo app states */
typedef CsrUint8 HfDemoStates_t;
#define idle (HfDemoStates_t) 0x00
#define security (HfDemoStates_t) 0x01
#define dialing (HfDemoStates_t) 0x02
#define memDialing (HfDemoStates_t) 0x03
#define enteringCommand (HfDemoStates_t) 0x04
#define enteringDTMF (HfDemoStates_t) 0x05
#define operatorQuery (HfDemoStates_t) 0x06
#define enteringReactParam (HfDemoStates_t) 0x07
#define cancelState (HfDemoStates_t) 0x08
#define memDialling (HfDemoStates_t) 0x09
#define acc_rej_audio (HfDemoStates_t) 0X0A

typedef CsrUint8 HfDemoStatesCLI_t;
#define cliMainMenu (HfDemoStatesCLI_t) 0x00
#define cliSubMenuAT (HfDemoStatesCLI_t) 0x01
#define cliSubMenuDTMF (HfDemoStatesCLI_t) 0x02
#define cliSubMenuCallHandling (HfDemoStatesCLI_t) 0x03
#define cliSubMenuCallSpecify (HfDemoStatesCLI_t) 0x04
#define cliSubMenuChangeAudioCodec (HfDemoStatesCLI_t) 0x05
#define cliSubMenuChangeAudioCodecHandling (HfDemoStatesCLI_t) 0x06
#define cliSubMenuSpecialAudioSettings (HfDemoStatesCLI_t) 0x07
#define cliActivateMenu (HfDemoStatesCLI_t) 0x08
#define cliReactivateMenu (HfDemoStatesCLI_t) 0x09
#define cliSubMenuC2C (HfDemoStatesCLI_t) 0x0A
#define cliSubMenuChooseCon (HfDemoStatesCLI_t) 0x0B
#define cliSubMenuIIA (HfDemoStatesCLI_t) 0x0C
#define cliSubMenuCod (HfDemoStatesCLI_t) 0x0D

typedef CsrUint8 HfLinkState_t;
#define disconnected_s (HfDemoStatesCLI_t) 0x00
#define connected_s (HfDemoStatesCLI_t) 0x01

#define CSR_BT_HF_STD_PRIO                             0x01
#define CSR_BT_HF_HIGH_PRIO                             0x02

typedef struct
{
    char                           *name;
    CsrUint16                        mode_connection;
    CsrUint16                        format;
} OperatorInfo_t;

typedef struct
{
    CsrUint8                         startup;
    HfLinkState_t                    linkState;
    CsrBtDeviceAddr                  bdAddr;
    CsrBool                          audioOn;
    CsrBool                          inbandRingingActivatedInHfg;
    CsrBool                          inbandRingingActivatedInHf;
    CsrUint8                         speakerGain;
    CsrUint8                         micGain;
    CsrUint8                         atResultCounter;
    CsrBool                          threeWayCallingSupported;
    CsrUint32                        localSupportedFeatures;
    CsrUint32                        localActivatedFeatures;
    CsrUint16                        scoHandle;
    CsrBool                          cmeErrorCodes;
    CsrBool                          cmerStatus;
    CsrUint8                         audioCodecType;
    CsrBtHfConnectionType            linkType;
    CsrUint16                        remoteVersion;
    CsrUint32                        hfgSupportedFeatures;
    CsrUint32                        conId;
    CsrBool                          incScoPending;
    CsrUint8                         hfgC2c[CSR_BT_C2C_NUMBER_OF_INDICATORS];
    CsrUint8                         *cindString;
    CsrUint8                         *cindValues;
    CsrUint16                        indicatorActivation;
    CsrUint16                        tmpIndicatorActivation;
} HfHsCommonInstData_t;

typedef void (* CsrUiHfEventHandlerFuncType)(void * instData, CsrUieHandle displayHandle, CsrUieHandle eventHandle, CsrUint16 key);

typedef struct
{
    CsrUieHandle                               displayHandle;
    CsrUieHandle                               sk1EventHandle;
    CsrUieHandle                               sk2EventHandle;
    CsrUieHandle                               backEventHandle;
    CsrUieHandle                               deleteEventHandle;
    CsrUiHfEventHandlerFuncType                sk1EventHandleFunc;
    CsrUiHfEventHandlerFuncType                sk2EventHandleFunc;
    CsrUiHfEventHandlerFuncType                deleteEventHandleFunc;
    CsrUiHfEventHandlerFuncType                backEventHandleFunc;
} CsrBtHfDisplayHandlesType;

typedef struct
{
    CsrUint8                        uiIndex;
    CsrUint8                        eventState;
    CsrBool                         inHfMenu;
    CsrBool                         goToMainmenu;
    CsrSchedTid                     popupTimerId;
    CsrBtHfDisplayHandlesType       displayesHandlers[NUMBER_OF_CSR_BT_HF_APP_UI];
} CsrBtHfCsrUiType;

typedef struct
{
    HfDemoStates_t                  state;
    HfDemoStates_t                  prevState;
    HfHsCommonInstData_t            conInstData[MAX_NUM_CONNECTION];
    CsrUint8                        currentConnection;
    phoneNumber_t                   myPhoneNumber;
    phoneNumber_t                   myMemNumber;
    void                            *recvMsgP;
    CsrBool                         serverActivated;
    HfDemoStatesCLI_t               cliState;
    ATCommand_t                     myATCommand;
    OperatorInfo_t                  operatorInfo;
    callIdentity_t                  selectedCall;
    CsrSchedQid                     app_hdl;
    CsrUint8                        batteryLevel;
    HfDemoStates_t                  preState;
    CsrUint32                       hfgSupportedFeatures;
    CsrBool                         rfShield;
    CsrBool                         codecEnabled;
    CsrBool                         adpcmLocalSupported;
    CsrUint16                       bccmdSeqNo;
    CsrUint8                        audioCodecType;
    CsrBtDeviceAddr                 remoteAddr;
    ScAppInstData_t                 scInstData;
    CsrBool                         voiceRecognitionOn;
    CsrUint8                        audioSampleRate;
    CsrBool                         localCodecNegStarted;
    CsrUint8                        locC2c[CSR_BT_C2C_NUMBER_OF_INDICATORS];
    CsrBool                         connReqPending;
    CsrUint8                        maxHFrecords;
    CsrUint8                        maxHSrecords;
    CsrUint8                        maxSimulCons;
    CsrUint8                        reactKey; /*to store the menu key of reactivation menu*/
    CsrBtHfCsrUiType                csrUiVar;
    CsrSchedTid                     deactGuard;
/*    CsrSchedTid                        discGuard;*/
    CsrUint8                        callIndex;
    CsrUint8                        *subscriberInfo;
    CsrUint8                        *operatorName;
} DemoInstdata_t;

void handleHfPrim(DemoInstdata_t * instData);
void hfHandleCmPrim(DemoInstdata_t *instData);
void hfHandleCsrBccmdPrim(DemoInstdata_t *instData);
void hfHandleKeybPrim(DemoInstdata_t * instData);

char * getHfHsString(CsrBtHfConnectionType theConnectionType);

void playMenu(DemoInstdata_t * instData);
void playIntro(DemoInstdata_t * instData);
void printLocalSupportedFeatures(DemoInstdata_t * instData);
void printHfgSupportedFeatures(DemoInstdata_t * instData, CsrUint32 features);
void playCalls(DemoInstdata_t *instData);
void playResponseAndHoldOptions(DemoInstdata_t * instData);

void sendC2cInit(DemoInstdata_t *instData,CsrBtHfConnectionId connectionId, CsrBool enable);
void sendAudioCodec(CsrBtHfConnectionId connId, CsrUint8 codec, CsrUint8 sampleRate);
void sendBatteryLevel(CsrBtHfConnectionId connId, CsrUint8 batteryLevel);
void initInstanceData(DemoInstdata_t * instData);
void hfDeinit(DemoInstdata_t *instData);

CsrUint8 HfGetNrActiveConnections(DemoInstdata_t * instData);
CsrBool findActiveConnectionType(DemoInstdata_t * instData,CsrBtHfConnectionType theConnectionType);
void findFirstActiveConnection(DemoInstdata_t * instData);
int HfGetAtToken(CsrUint8  *line, char    **nextChar2Interpret, CsrUint16 *charsConsumed);

typedef void (* CsrBtHfEventType)(DemoInstdata_t * instData);

#ifdef __cplusplus
}
#endif

#endif
