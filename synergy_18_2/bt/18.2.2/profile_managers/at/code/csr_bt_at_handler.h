#ifndef CSR_BT_AT_HANDLER_H__
#define CSR_BT_AT_HANDLER_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_bt_result.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_dg_prim.h"
#include "csr_bt_dg_lib.h"
#include "csr_message_queue.h"
#include "csr_bt_usr_config.h"
#include "csr_bt_at_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================*
  Intern definitions
 *===========================================================================*/
#define EMPTY                   (0)
#define TERMINATORNOTFOUND      (0)
#define TERMINATORFOUND         (1)
#define ECHO_ON                 (1)
#define NO_SEMICOLON            (0)
#define SEMICOLON               (1)
#define NOWAITINGFORRESPONSE    (0)
#define WAITINGFORRESPONSE      (1)
#define ABORT_OFF               (0)
#define    ABORT_ON             (1)
#define NOT_CHANGE              (0)
#define CHANGE                  (1)
#define NO_CFM                  (0)
#define CFM_RECEIVE             (1)
#define NO_APP_RES              (0)
#define    APP_RES              (1)
#define DTR_OFF                 (0)
#define    DISCONNECT           (0)
#define NOT_DISCONNECT          (1)

#define TO_UPPER(x)         ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x)    ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)

#define NO_TOKEN                            0x00
#define    AND_C_TOKEN                      0x01
#define    AND_D_TOKEN                      0x02
#define    AND_F_TOKEN                      0x03
#define    GCAP_TOKEN                       0x04
#define    GMI_TOKEN                        0x05
#define    GMM_TOKEN                        0x06
#define    GMR_TOKEN                        0x07
#define    A_TOKEN                          0x08
#define    D_TOKEN                          0x09
#define    E_TOKEN                          0x0A
#define    H_TOKEN                          0x0B
#define    L_TOKEN                          0x0C
#define    M_TOKEN                          0x0D
#define    O_TOKEN                          0x0E
#define    P_TOKEN                          0x0F
#define    Q_TOKEN                          0x10
#define    T_TOKEN                          0x11
#define    V_TOKEN                          0x12
#define    X_TOKEN                          0x13
#define    Z_TOKEN                          0x14
#define    S0_TOKEN                         0x15
#define    S3_TOKEN                         0x16
#define    S4_TOKEN                         0x17
#define    S5_TOKEN                         0x18
#define    S6_TOKEN                         0x19
#define    S7_TOKEN                         0x1A
#define    S8_TOKEN                         0x1B
#define    S10_TOKEN                        0x1C
#define    S0_READ_TOKEN                    0x1D
#define    S3_READ_TOKEN                    0x1E
#define    S4_READ_TOKEN                    0x1F
#define    S5_READ_TOKEN                    0x20
#define    S6_READ_TOKEN                    0x21
#define    S7_READ_TOKEN                    0x22
#define    S8_READ_TOKEN                    0x23
#define    S10_READ_TOKEN                   0x24
#define    UNKNOWN_EXTENDED_COMMAND_TOKEN    0x25
#define NUMBER_OF_TOKENS                    (38)

#define INTERPRET_ERROR                     (0)
#define COMPARE_PREFIX_FUNC                 (1)
#define COMMAND_TYPE_FUNC                   (2)
#define AND_COMMAND_TYPE_FUNC               (3)
#define EXTENDED_COMMAND_TYPE_FUNC          (4)
#define EXTENDED_M_OR_C_FUNC                (5)
#define EXTENDED_I_OR_M_OR_R_FUNC           (6)
#define S_COMMAND_TYPE_FUNC                 (7)
#define S10_COMMAND_TYPE_FUNC               (8)
#define S_READ_OR_SET_COMMAND_TYPE_FUNC     (9)
#define EXTENDED_A_FUNC                     (10)
#define EXTENDED_GCAP_FUNC                  (11)
#define CONTINUATION_AT_INTERPRET_FUNC      (12)
#define    RETURN_VALUE_FUNC                (13)
#define RETURN_VALUE2_FUNC                  (14)
#define RETURN_VALUE3_FUNC                  (15)
#define RETURN_VALUE4_FUNC                  (16)
#define CONTINUATION_COPY_DIAL_STRING_FUNC  (17)
#define EXTENDED_FINAL_CHARACTER_FUNC       (18)
#define    UNKNOWN_EXTENDED_COMMAND_FUNC     (19)

#define AT_STATE_BASE                       (0)


/*-------------------------------------------------------------*/
/* State definitions */
/*-------------------------------------------------------------*/
typedef enum
{
    atInitialising_s = AT_STATE_BASE,
    atCmd_s,
    atOnlineCmd_s,
    atOnlineData_s,
    atNumOfState
} atStates_t;


/*============================================================================*
  typedefs
  *============================================================================*/
typedef struct
{
    CsrUint8    receiveToken;
    CsrUint8    receiveValue;
} tokenSample_t;

typedef struct
{
    CsrSchedQid                  CsrSchedQid;               /* AT queue id (myself) */
    CsrSchedQid                  appHandle;         /* IWU queue id */
    CsrBtConnId             btConnId;
    CsrUint8                serverChannel;
    atStates_t              state;
    CsrBtAtParameterValues  paramSettings;
    CsrBtAtParameterValues  tempparamSettings;
    CsrBool                 terminatorFlag;
    CsrBool                 responseFlag;
    CsrBool                 abortFlag;
    CsrBool                 atCfmFlag;
    CsrBool                 appResFlag;
    CsrBool                 disconnectFlag;
    CsrUint8                numberOfTokens;
    int                     numberOfRespon;
    CsrUint8                changeSettings;
    CsrUint8                temptoken;
    CsrUint8                tokenValue;
    CsrUint8                extendedTempToken;
    CsrUint16               reachPoint;
    CsrUint16               counter;
    CsrUint16               packetLength;
    CsrUint8                *cmd;
    CsrUint16               tempValue;
    tokenSample_t           tokenFlipArray[CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND];
    CsrMessageQueueType    *cmDataReqQueue;
    void                    *atMsgP;
    CsrUint16               dialStringCounter;
    char                    lastChar;
    CsrUint8                dialString[CSR_BT_MAX_DIAL_STRING_LENGTH];
    CsrUint8                *atUnknownExtendedCmd[CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND];
    CsrUint8                unknownExtendedCmdLength[CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND];
    CsrUint8                numberOfUnknownCmd;
    CsrSchedTid                  timerId;
    CsrBool                 superVisionTimerId;
} atInstanceData_t;

typedef void (* atTokenActionHandler)(atInstanceData_t *atData);
typedef void (* atResultCodeResponsHandler)(atInstanceData_t *atData);
typedef void (* atStateHandlerType)(atInstanceData_t * taskData);

/*------------------------------------------------------------------*/
/*  function interface definition */
/*------------------------------------------------------------------*/

/*************************************************************************************
 These are found under csr_bt_at_main.c
************************************************************************************/
void CsrBtAtInit(void **gash);
void CsrBtAtHandler(void **gash);


/*************************************************************************************
 These are found under csr_bt_at_parent.c
************************************************************************************/
void mainAtInterpret(CsrUint8 *commandLine, atInstanceData_t *atData);
void continuationAtInterpret(CsrUint8 **commandLine, atInstanceData_t *atData);
void continuationCopyDialString(CsrUint8 **commandLine, atInstanceData_t *atData);
void helpReturnValue(CsrUint8 **commandLine, atInstanceData_t *atData);
void helpReturnValue2(CsrUint8 **commandLine, atInstanceData_t *atData);
void helpReturnValue3(CsrUint8 **commandLine, atInstanceData_t *atData);
void helpReturnValue4(CsrUint8 **commandLine, atInstanceData_t *atData);

/*************************************************************************************

************************************************************************************/
void comparePrefix(CsrUint8 *commandLine, atInstanceData_t *atData);
void secondComparePrefix(CsrUint8 *commandLine, atInstanceData_t *atData);
void commandType(CsrUint8 **commandLine, atInstanceData_t *atData);

/*************************************************************************************
 These are found under atHelpfunc.c
************************************************************************************/
void CsrBtAtMessagePut(CsrSchedQid phandle, void *msg);

CsrBool removeSpace(CsrUint8 **commandLine);
CsrBool checkForS3(CsrUint8 **commandLine, atInstanceData_t *atData);
void removeS5(CsrUint8 **commandLine, atInstanceData_t *atData);
CsrBool copyDialString(CsrUint8 **commandLine, atInstanceData_t *atData);
void insertTokenInArray(CsrUint8 token, CsrUint16 value, atInstanceData_t *atData);
void returnValue(CsrUint8 **commandLine, atInstanceData_t *atData);
CsrBool returnValue2(CsrUint8 **commandLine, atInstanceData_t *atData);
CsrBool returnValue3(CsrUint8 **commandLine, atInstanceData_t *atData);
CsrBool returnValue4(CsrUint8 **commandLine, atInstanceData_t *atData);
void abortHandler(atInstanceData_t *atData);
void paramChangeHandler(atInstanceData_t *atData);
void atDgDisconnectCleanUp(atInstanceData_t *atData);
void atStateChangeHandler(atInstanceData_t *atData, CsrUint8 modemstatus);
void initExtendedCmdTable(atInstanceData_t *atData);
void clearExtendedCmdTable(atInstanceData_t *atData);

/*************************************************************************************
 This one is found under at&Handler.c
************************************************************************************/
void andCommandType(CsrUint8 **commandLine, atInstanceData_t *atData);

/*************************************************************************************
 These are found under at+Handler.c
************************************************************************************/
void extendedCommandType(CsrUint8 **commandLine, atInstanceData_t *atData);
void extendedMOrC(CsrUint8 **commandLine, atInstanceData_t *atData);
void extendedIOrMOrR(CsrUint8 **commandLine, atInstanceData_t *atData);
void extendedA(CsrUint8 **commandLine, atInstanceData_t *atData);
void extendedGCAP(CsrUint8 **commandLine, atInstanceData_t *atData);
void extendedFinalCharacter(CsrUint8 **commandLine, atInstanceData_t *atData);
void copyUnknownExtendedCommand(CsrUint8 **commandLine, atInstanceData_t *atData, CsrBool flag);

/*************************************************************************************
 These are found under atSHandler.c
************************************************************************************/
void sCommandType(CsrUint8 **commandLine, atInstanceData_t *atData);
void s10CommandType(CsrUint8 **commandLine, atInstanceData_t *atData);
void sReadOrSetCommandType(CsrUint8 token, CsrUint8 **commandLine, atInstanceData_t *atData);

/*************************************************************************************
 These are found under atTokenHandler.c
************************************************************************************/
void atTokenHandler(atInstanceData_t *atData);
void circuit109(atInstanceData_t *atData);
void circuit108(atInstanceData_t *atData);
void setToFactoryDefinedConfiguration(atInstanceData_t *atData);
void reqCompleteCapabilitiesList(atInstanceData_t *atData);
void reqManufacturerIdentification(atInstanceData_t *atData);
void reqModelIdentification(atInstanceData_t *atData);
void reqRevisionIdentification(atInstanceData_t *atData);
void answer(atInstanceData_t *atData);
void dial(atInstanceData_t *atData);
void commandEcho(atInstanceData_t *atData);
void hookControl(atInstanceData_t *atData);
void monitorSpeakerLoudness(atInstanceData_t *atData);
void monitorSpeakerMode(atInstanceData_t *atData);
void returnToOnlineState(atInstanceData_t *atData);
void selectPulseDialing(atInstanceData_t *atData);
void resultCodeSuppression(atInstanceData_t *atData);
void selectToneDialing(atInstanceData_t *atData);
void dceResponseFormat(atInstanceData_t *atData);
void resultCodeSelection(atInstanceData_t *atData);
void resetToDefaultConfiguration(atInstanceData_t *atData);
void automaticAnswer(atInstanceData_t *atData);
void cmdLineTerminationCharacter(atInstanceData_t *atData);
void responseFormattingCharacter(atInstanceData_t *atData);
void cmdLineEditingCharacter(atInstanceData_t *atData);
void pauseBeforeBlindDialing(atInstanceData_t *atData);
void ConnectionCompletionTimeout(atInstanceData_t *atData);
void commaDialModifierTime(atInstanceData_t *atData);
void automaticDisconnectDelay(atInstanceData_t *atData);
void readAutomaticAnswerValue(atInstanceData_t *atData);
void readCmdTerminationValue(atInstanceData_t *atData);
void readResponseFormattingValue(atInstanceData_t *atData);
void readCmdLineEditingValue(atInstanceData_t *atData);
void readPauseBeforeBlindDialingValue(atInstanceData_t *atData);
void readConnectionCompleteTimeoutValue(atInstanceData_t *atData);
void readCommaDialModifierTimeValue(atInstanceData_t *atData);
void readAutomaticDisconnectValue(atInstanceData_t *atData);
void sendUnknownExtendedCmd(atInstanceData_t *atData);

/*************************************************************************************
 These are found under atIndhandler.c
************************************************************************************/
void atDgDisconnectIndHandler(atInstanceData_t *atData);
void atDgConnectIndHandler(atInstanceData_t *atData);
void atDgDataPathStatusIndHandle(atInstanceData_t *atData);
void atDgJustDataIndHandler(atInstanceData_t *atData);
void setAbortFlag (CsrUint16 m, atInstanceData_t * atData);
void atDgdataIndHandler(atInstanceData_t *atData);
void atDgDeactivateCfmHandler(atInstanceData_t *atData);
void atDgDataCfmHandler(atInstanceData_t *atData);
void atDgControlIndHandler(atInstanceData_t *atData);
void atDgPortnegIndHandler(atInstanceData_t *atData);
void atDgdataIndInUnknownStateHandler(atInstanceData_t *atData);
void atDgdataIndAtCmdAtOnlineCmdStateHandler(atInstanceData_t *atData);

/*************************************************************************************
 These are found under csr_bt_at_msg_factory.c
************************************************************************************/
void dgDataEchoSend(atInstanceData_t *atData);
void dgActivateReqHandler(atInstanceData_t *atData);
void dgDeactivateReqHandler(atInstanceData_t *atData);
void dgDisconnectReqHandler(atInstanceData_t *atData);
void dgDataReqHandler(atInstanceData_t *atData);
void atDgDataReqHandler(CsrBtConnId btConnId, CsrUint16 payloadLength, CsrUint8 *payload, atInstanceData_t *atData);
void atDgControlReqHandler(atInstanceData_t *atData);
void atDgControlIndMsgSend(CsrSchedQid theAppHandleMuxID_t, CsrBtConnId btConnId, CsrUint8 theModemStatus);
void atDgDisconnectIndMsgSend(CsrSchedQid theAppHandle, CsrBtConnId btConnId,
    CsrBool localTerminated, CsrBtReasonCode reasonCode, CsrBtSupplier reasonSupplier);
void atDgPortnegIndMsgSend(CsrSchedQid theAppHandle, CsrBtConnId btConnId, RFC_PORTNEG_VALUES_T portPar, CsrBool request);
void atDgPortnegResHandler(atInstanceData_t *atData);

/*************************************************************************************
 These are found under atSQResHandler.c
************************************************************************************/
void atS0QResHandler(atInstanceData_t *atData);
void atS3QResHandler(atInstanceData_t *atData);
void atS4QResHandler(atInstanceData_t *atData);
void atS5QResHandler(atInstanceData_t *atData);
void atS6QResHandler(atInstanceData_t *atData);
void atS7QResHandler(atInstanceData_t *atData);
void atS8QResHandler(atInstanceData_t *atData);
void atS10QResHandler(atInstanceData_t *atData);
void atSxQInfoResHandler(atInstanceData_t *atData, CsrUint8 value);
void atOKResultResHandler(atInstanceData_t *atData);
void atDataResHandler(atInstanceData_t *atData);

/*************************************************************************************
 These are found under atResultResHandler.c
************************************************************************************/
void atResultCodeResHandler(atInstanceData_t *atData);
void atResultCodeOKHandler(atInstanceData_t *atData);
void atResultCodeConnectHandler(atInstanceData_t *atData);
void atResultCodeConnectTextHandler(atInstanceData_t *atData);
void atResultCodeRingHandler(atInstanceData_t *atData);
void atResultCodeNoCarrierHandler(atInstanceData_t *atData);
void atResultCodeErrorHandler(atInstanceData_t *atData);
void atResultCodeNoDialToneHandler(atInstanceData_t *atData);
void atResultCodeBusyHandler(atInstanceData_t *atData);
void atResultCodeNoAnswerHandler(atInstanceData_t *atData);
void atGmmResHandler(atInstanceData_t *atData);
void atGmiResHandler(atInstanceData_t *atData);
void atGmrResHandler(atInstanceData_t *atData);
void atGcapResHandler(atInstanceData_t *atData);
void atGxxHelpResHandler(atInstanceData_t *atData, CsrUint16 payloadLength, CsrUint8 *payload);
void atDefaultSettingResHandler(atInstanceData_t *atData);
void atFactorySettingResHandler(atInstanceData_t *atData);
void atUnknownExtendedCmdResHandler(atInstanceData_t *atData);


/*************************************************************************************
 These are found under csr_bt_at_free_down.c
************************************************************************************/
void CsrBtAtFreeDownstreamMessageContents(CsrUint16 eventClass, void * message);




#ifdef __cplusplus
}
#endif

#endif

