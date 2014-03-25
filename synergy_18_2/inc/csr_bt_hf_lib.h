#ifndef CSR_BT_HF_LIB_H__
#define CSR_BT_HF_LIB_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_bt_profiles.h"
#include "csr_bt_hf_prim.h"
#include "csr_msg_transport.h"
#include "csr_bt_tasks.h"
#include "csr_pmem.h"
#include "csr_util.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Common put_message function to reduce code size */
void CsrBtHfMsgTransport(void* msg);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfServiceConnectReqSend
 *
 *  DESCRIPTION
 *        A CSR_BT_HF_SERVICE_CONNECT_REQ will initiate a connection towards a device
 *        specified by the Bluetooth device address.
 *        The HF will send a CSR_BT_HF_SERVICE_CONNECT_IND back to the initiator with
 *        the result of the connection attempt.
 *
 *    PARAMETERS
 *        deviceAddr:            address of device to connect to
 *        connectionType:        type of connection desired
 *----------------------------------------------------------------------------*/
#define CsrBtHfServiceConnectReqSend(_deviceAddr, _connectionType) {    \
        CsrBtHfServiceConnectReq *msg = (CsrBtHfServiceConnectReq *)CsrPmemAlloc(sizeof(CsrBtHfServiceConnectReq)); \
        msg->type        = CSR_BT_HF_SERVICE_CONNECT_REQ;               \
        msg->deviceAddr  = _deviceAddr;                                 \
        msg->connectionType = _connectionType;                          \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfActivateReqSend
 *
 *  DESCRIPTION
 *        Activate the HF service. This signal is used to allowed remote devices
 *        to find and discover the HF service and subsequently connect to it.
 *        The HF will send a CSR_BT_HF_SERVICE_CONNECT_IND back to the initiator with
 *        the result of the connection attempt.
 *
 *    PARAMETERS
 *        phandle:            application handle
 *        maxNumberOfHfConnections   CSRMAX number of simultaneous Hf connections
 *        maxNumberOfHsConnections   CSRMAX number of simultaneous Hs connections
 *        maxSimultaneousConnections CSRMAX total number of sim. connections
 *        supportedFeatures   bitmap with supported features
 *        hfConfig            bitmap used to enable/disable additional features
 *----------------------------------------------------------------------------*/
#define CsrBtHfActivateReqSend(_phandle, _maxNumberOfHfConnections, _maxNumberOfHsConnections, _maxSimultaneousConnections, _supportedFeatures, _hfConfig, _atResponseTime) { \
        CsrBtHfActivateReq *msg = (CsrBtHfActivateReq *)CsrPmemAlloc(sizeof(CsrBtHfActivateReq)); \
        msg->type              = CSR_BT_HF_ACTIVATE_REQ;                \
        msg->phandle           = _phandle;                              \
        msg->supportedFeatures = _supportedFeatures;                    \
        msg->hfConfig          = _hfConfig;                             \
        msg->maxHFConnections  = _maxNumberOfHfConnections;             \
        msg->maxHSConnections  = _maxNumberOfHsConnections;             \
        msg->maxSimultaneousConnections = _maxSimultaneousConnections;  \
        msg->atResponseTime    = _atResponseTime;                       \
        CsrBtHfMsgTransport(msg);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfDeactivateReqSend
 *
 *  DESCRIPTION
 *        Deactivate the HF service. This signal deactivates the HF service and
 *        removes the service records
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfDeactivateReqSend() {                                    \
        CsrBtHfDeactivateReq *msg = (CsrBtHfDeactivateReq *)CsrPmemAlloc(sizeof(CsrBtHfDeactivateReq)); \
        msg->type = CSR_BT_HF_DEACTIVATE_REQ;                           \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfCancelConnectReqSend
 *
 *  DESCRIPTION
 *        Cancel an SLC establishment in the HF manager. The HF manager decides
 *        upon the actual state how to handle the cancel signal. I.e. if the
 *        RFC conneciton is already established it will be disconnected; else
 *        the connection operation will be cancelled.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfCancelConnectReqSend(_deviceAddr) {                      \
        CsrBtHfCancelConnectReq *msg = (CsrBtHfCancelConnectReq *)CsrPmemAlloc(sizeof(CsrBtHfCancelConnectReq)); \
        msg->type        = CSR_BT_HF_CANCEL_CONNECT_REQ;                \
        msg->deviceAddr  = _deviceAddr;                                 \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfDisconnectReqSend
 *
 *  DESCRIPTION
 *        Disconnect an SLC in the HF manager. The HF manager decides upon the
 *        actual state how to handle the disconnect signal. I.e. if the manager is
 *        connecting, the connection operation will be cancelled. If a conneciton
 *        is established it will be disconnected.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfDisconnectReqSend(_connectionId) {                       \
        CsrBtHfDisconnectReq *msg = (CsrBtHfDisconnectReq *)CsrPmemAlloc(sizeof(CsrBtHfDisconnectReq)); \
        msg->type           = CSR_BT_HF_DISCONNECT_REQ;                 \
        msg->connectionId = _connectionId;                              \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfConfigLowPowerReqSend
 *
 *  DESCRIPTION
 *        Configure the way the HF manager shall handle low power.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfConfigLowPowerReqSend(_connectionId, _mask) {            \
        CsrBtHfConfigLowPowerReq *msg = (CsrBtHfConfigLowPowerReq *)CsrPmemAlloc(sizeof(CsrBtHfConfigLowPowerReq)); \
        msg->type = CSR_BT_HF_CONFIG_LOW_POWER_REQ;                     \
        msg->connectionId  = _connectionId;                             \
        msg->mask          = _mask;                                     \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfGetAllStatusIndicatorsReqSend
 *
 *  DESCRIPTION
 *        Configure the way the HF manager shall handle low power.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfGetAllStatusIndicatorsReqSend(_connectionId) {           \
        CsrBtHfGetAllStatusIndicatorsReq *msg = (CsrBtHfGetAllStatusIndicatorsReq *)CsrPmemAlloc(sizeof(CsrBtHfGetAllStatusIndicatorsReq)); \
        msg->type           = CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ;  \
        msg->connectionId = _connectionId;                              \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfCallEndReqSend
 *
 *  DESCRIPTION
 *        Reject an incoming call in the HF manager.The call will be disconnected.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfCallEndReqSend(_connectionId) {                          \
        CsrBtHfCallEndReq *msg = (CsrBtHfCallEndReq *)CsrPmemAlloc(sizeof(CsrBtHfCallEndReq)); \
        msg->type = CSR_BT_HF_CALL_END_REQ;                             \
        msg->connectionId = _connectionId;                              \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAnswerReqSend
 *
 *  DESCRIPTION
 *        Accept an incoming call in the HF manager.
 *
 *    PARAMETERS
 *----------------------------------------------------------------------------*/
#define CsrBtHfAnswerReqSend(_connectionId) {                           \
        CsrBtHfCallAnswerReq *msg = (CsrBtHfCallAnswerReq *)CsrPmemAlloc(sizeof(CsrBtHfCallAnswerReq)); \
        msg->type           = CSR_BT_HF_CALL_ANSWER_REQ;                \
        msg->connectionId = _connectionId;                              \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAudioConnectReqSend
 *
 *  DESCRIPTION
 *        Connect the audio path on the established service level connection.
 *        The audio switching is confirmed by a CSR_BT_HF_AUDIO_CONNECT_CFM.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfAudioConnectReqSend(_connectionId, _audioParametersLength, _audioParameters, _pcmSlot, _pcmRealloc) { \
        CsrBtHfAudioConnectReq *msg = (CsrBtHfAudioConnectReq *)CsrPmemAlloc(sizeof(CsrBtHfAudioConnectReq)); \
        msg->type              = CSR_BT_HF_AUDIO_CONNECT_REQ;           \
        msg->connectionId      = _connectionId;                         \
        msg->audioParameters   = _audioParameters;                      \
        msg->audioParametersLength = _audioParametersLength;            \
        msg->pcmSlot           = _pcmSlot;                              \
        msg->pcmRealloc        = _pcmRealloc;                           \
        CsrBtHfMsgTransport(msg);}

#ifdef CSR_BT_INSTALL_HF_CONFIG_AUDIO
/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAudioConfigReqSend
 *
 *  DESCRIPTION
 *        Configure audio settings. The settings to configure are determined
 *        by the "audioType" parameter
 *
 *    PARAMETERS
 *        connectionId:       connectionID to apply the settings to
 *        audioType:          type of settings to set
 *        audioSetting:       pointer to data
 *        audioSettingLen   : length in bytes of the data given
 *----------------------------------------------------------------------------*/
#define CsrBtHfAudioConfigReqSend(_connectionId, _audioType, _audioSetting, _audioSettingLen) { \
        CsrBtHfConfigAudioReq *msg = (CsrBtHfConfigAudioReq *)CsrPmemAlloc(sizeof(CsrBtHfConfigAudioReq)); \
        msg->type = CSR_BT_HF_CONFIG_AUDIO_REQ;                         \
        msg->connectionId       = _connectionId;                        \
        msg->audioType          = _audioType;                           \
        msg->audioSetting       = _audioSetting;                        \
        msg->audioSettingLen    = _audioSettingLen;                     \
        CsrBtHfMsgTransport(msg);}
#endif /* CSR_BT_INSTALL_HF_CONFIG_AUDIO */

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAudioAcceptResSend
 *
 *  DESCRIPTION
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfAudioAcceptResSend(_connectionId, _acceptResponse, _acceptParameters, _pcmSlot, _pcmReassign) { \
        CsrBtHfAudioAcceptConnectRes *msg = (CsrBtHfAudioAcceptConnectRes *)CsrPmemAlloc(sizeof(CsrBtHfAudioAcceptConnectRes)); \
        msg->type           = CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES;       \
        msg->connectionId   = _connectionId;                            \
        msg->acceptResponse = _acceptResponse;                          \
        msg->acceptParameters = _acceptParameters;                      \
        msg->acceptParametersLength = (_acceptParameters ? 1 : 0);      \
        msg->pcmSlot        = _pcmSlot;                                 \
        msg->pcmReassign    = _pcmReassign;                             \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAudioDisconnectReqSend
 *
 *  DESCRIPTION
 *        disconnect the audio path on the established service level connection.
 *        The audio switching is confirmed by a CSR_BT_HF_AUDIO_DISCONNECT_CFM.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfAudioDisconnectReqSend(_connectionId, _scoHandle) {      \
        CsrBtHfAudioDisconnectReq *msg = (CsrBtHfAudioDisconnectReq *)CsrPmemAlloc(sizeof(CsrBtHfAudioDisconnectReq)); \
        msg->type              = CSR_BT_HF_AUDIO_DISCONNECT_REQ;        \
        msg->connectionId      = _connectionId;                         \
        msg->scoHandle         = _scoHandle;                            \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfAtCmdReqSend
 *
 *  DESCRIPTION
 *        Send and at-command defined by the higher layer to HF.
 *
 *    PARAMETERS
 *        len:                number of chars in the payload.
 *        payload:            pointer to a at-command string. The pointer will be
 *                            handed over to the HF which eventually will free it.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfAtCmdReqSend(_len, _payload, _connectionId) {            \
        CsrBtHfAtCmdReq *msg = (CsrBtHfAtCmdReq *)CsrPmemAlloc(sizeof(CsrBtHfAtCmdReq)); \
        msg->type           = CSR_BT_HF_AT_CMD_REQ;                     \
        msg->atCmdString    = (CsrCharString *)_payload;                \
        msg->connectionId   = _connectionId;                            \
        CsrBtHfMsgTransport(msg);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfGetSubscriberNumberInformationReqSend
 *
 *  DESCRIPTION
 *        Send the AT+CNUM at-command to the remote device.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfGetSubscriberNumberInformationReqSend(_connectionId) {   \
        CsrBtHfGetSubscriberNumberInformationReq * msg = (CsrBtHfGetSubscriberNumberInformationReq *)CsrPmemAlloc(sizeof(CsrBtHfGetSubscriberNumberInformationReq)); \
        msg->type  = CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ;   \
        msg->connectionId  = _connectionId;                             \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfGetCurrentCallListReqSend
 *
 *  DESCRIPTION
 *        Send the AT+CLCC at-command to the remote device.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfGetCurrentCallListReqSend(_connectionId) {               \
        CsrBtHfGetCurrentCallListReq *msg = (CsrBtHfGetCurrentCallListReq *)CsrPmemAlloc(sizeof(CsrBtHfGetCurrentCallListReq)); \
        msg->type          = CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ;       \
        msg->connectionId  = _connectionId;                             \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetCallNotificationIndicationReqSend
 *
 *  DESCRIPTION
 *        Send the AT+CLIP  at-command to the remote device.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetCallNotificationIndicationReqSend(_connectionId, _enable) { \
        CsrBtHfSetCallNotificationIndicationReq *msg = (CsrBtHfSetCallNotificationIndicationReq *)CsrPmemAlloc(sizeof(CsrBtHfSetCallNotificationIndicationReq)); \
        msg->type          = CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ; \
        msg->connectionId  = _connectionId;                             \
        msg->enable        = _enable;                                   \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetCallWaitingNotificationReqSend
 *
 *  DESCRIPTION
 *        Send the AT+CCWA  at-command to the remote device.
 *
 *    PARAMETERS
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetCallWaitingNotificationReqSend(_connectionId, _enable) { \
        CsrBtHfSetCallWaitingNotificationReq *msg = (CsrBtHfSetCallWaitingNotificationReq *)CsrPmemAlloc(sizeof(CsrBtHfSetCallWaitingNotificationReq)); \
        msg->type          = CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ; \
        msg->connectionId  = _connectionId;                             \
        msg->enable        = _enable;                                   \
        CsrBtHfMsgTransport(msg);}


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetEchoAndNoiseReqSend
 *
 *  DESCRIPTION
 *        Send request to change the echo & noise reduction settings.
 *
 *    PARAMETERS
 *        enable:     Echo&Noise reduction ON or OFF.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetEchoAndNoiseReqSend(_connectionId,_enable) {          \
        CsrBtHfSetEchoAndNoiseReq *msg = (CsrBtHfSetEchoAndNoiseReq *)CsrPmemAlloc(sizeof(CsrBtHfSetEchoAndNoiseReq)); \
        msg->type          = CSR_BT_HF_SET_ECHO_AND_NOISE_REQ;          \
        msg->connectionId  = _connectionId;                             \
        msg->enable        = _enable;                                   \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetVoiceRecognitionReqSend
 *
 *  DESCRIPTION
 *        Send request to start or stop voice recognition.
 *
 *    PARAMETERS
 *        enable:     Voice recognition ON or OFF.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetVoiceRecognitionReqSend(_connectionId, _enable) {     \
        CsrBtHfSetVoiceRecognitionReq *msg = (CsrBtHfSetVoiceRecognitionReq *)CsrPmemAlloc(sizeof(CsrBtHfSetVoiceRecognitionReq)); \
        msg->type          = CSR_BT_HF_SET_VOICE_RECOGNITION_REQ;       \
        msg->connectionId  = _connectionId;                             \
        msg->start         = _enable;                                   \
        CsrBtHfMsgTransport(msg);}



/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfGenerateDTMFReqSend
 *
 *  DESCRIPTION
 *        Send request to generate a DTMF tone.
 *
 *    PARAMETERS
 *        value:     DTMF tone/value to generate
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfGenerateDTMFReqSend(_connectionId, _value) {             \
        CsrBtHfGenerateDtmfReq *msg = (CsrBtHfGenerateDtmfReq *)CsrPmemAlloc(sizeof(CsrBtHfGenerateDtmfReq)); \
        msg->type          = CSR_BT_HF_GENERATE_DTMF_REQ;               \
        msg->connectionId  = _connectionId;                             \
        msg->dtmf          = _value;                                    \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSpeakerGainStatusReqSend
 *
 *  DESCRIPTION
 *        Send request to change the speaker gain.
 *
 *    PARAMETERS
 *        gain:                New speaker gain.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSpeakerGainStatusReqSend(_gain, _connectionId) {         \
        CsrBtHfSpeakerGainStatusReq *msg = (CsrBtHfSpeakerGainStatusReq *)CsrPmemAlloc(sizeof(CsrBtHfSpeakerGainStatusReq)); \
        msg->type           = CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ;        \
        msg->gain           = _gain;                                    \
        msg->connectionId   = _connectionId;                            \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfMicGainStatusReqSend
 *
 *  DESCRIPTION
 *        Send request to change the microphone gain.
 *
 *    PARAMETERS
 *        gain:                New microphone gain.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfMicGainStatusReqSend(_gain, _connectionId) {             \
        CsrBtHfMicGainStatusReq *msg = (CsrBtHfMicGainStatusReq *)CsrPmemAlloc(sizeof(CsrBtHfMicGainStatusReq)); \
        msg->type           = CSR_BT_HF_MIC_GAIN_STATUS_REQ;            \
        msg->gain           = _gain;                                    \
        msg->connectionId   = _connectionId;                            \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfCallHandlingReqSend
 *
 *  DESCRIPTION
 *        Send call handling request.
 *
 *    PARAMETERS
 *        index:        request Id
 *        value:        value to be applied to the request Id if any.
 *        connectionId: connection where the request shall be sent to.
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfCallHandlingReqSend(_command, _index, _connectionId) {   \
        CsrBtHfCallHandlingReq *msg = (CsrBtHfCallHandlingReq *) CsrPmemAlloc(sizeof(CsrBtHfCallHandlingReq)); \
        msg->type  = CSR_BT_HF_CALL_HANDLING_REQ;                       \
        msg->command = _command;                                        \
        msg->index = _index;                                            \
        msg->connectionId = _connectionId;                              \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfCopsReqSend
 *
 *  DESCRIPTION
 *        initiate cops procedure.
 *
 *  PARAMETERS
 *    mode:            The mode of the current cop signal
 *      format:                 The format for later query results
 *      connectionId:           The connection through which to send the at command
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfCopsReqSend(_mode, _format, _forceResendFormat, _connectionId) { \
        CsrBtHfGetCurrentOperatorSelectionReq *msg = (CsrBtHfGetCurrentOperatorSelectionReq*) CsrPmemAlloc(sizeof(CsrBtHfGetCurrentOperatorSelectionReq)); \
        msg->type    = CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ;    \
        msg->mode    = _mode;                                           \
        msg->format  = _format;                                         \
        msg->connectionId = _connectionId;                              \
        msg->forceResendingFormat = _forceResendFormat;                 \
        CsrBtHfMsgTransport(msg); }



/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetExtendedAgErrorResultCodeReqSend
 *
 *  DESCRIPTION
 *        initiate cops procedure.
 *
 *  PARAMETERS
 *      enable:                 Whether to enable or disable the extended error mode
 *      connectionId:           The connection through which to send the at command
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetExtendedAgErrorResultCodeReqSend(_connectionId, _enable) { \
        CsrBtHfSetExtendedAgErrorResultCodeReq *msg = (CsrBtHfSetExtendedAgErrorResultCodeReq *)CsrPmemAlloc(sizeof(CsrBtHfSetExtendedAgErrorResultCodeReq)); \
        msg->type          = CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ; \
        msg->connectionId  = _connectionId;                             \
        msg->enable        = _enable;                                   \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetStatusIndicatorUpdateReqSend
 *
 *  DESCRIPTION
 *      Get the actual status indicator values
 *
 *  PARAMETERS
 *       connectionId        The connection to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetStatusIndicatorUpdateReqSend(_connectionId,_enable) { \
        CsrBtHfSetStatusIndicatorUpdateReq *msg = (CsrBtHfSetStatusIndicatorUpdateReq *)CsrPmemAlloc(sizeof(CsrBtHfSetStatusIndicatorUpdateReq)); \
        msg->type          = CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ; \
        msg->connectionId  = _connectionId;                             \
        msg->enable        = _enable;                                   \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfBtInputReqSend
 *
 *  DESCRIPTION
 *      Send the AT+BINP command to request data input from the AG (so far only
 *      phone number is specified: AT+BINP=1")
 *
 *  PARAMETERS
 *       connectionId        The connection to use
 *       dataRequest         The type of data to request (only 1 specified so far)
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfBtInputReqSend(_connectionId,_dataRequest) {             \
        CsrBtHfBtInputReq *msg = (CsrBtHfBtInputReq *)CsrPmemAlloc(sizeof(CsrBtHfBtInputReq)); \
        msg->type          = CSR_BT_HF_BT_INPUT_REQ;                    \
        msg->connectionId  = _connectionId;                             \
        msg->dataRequest   = _dataRequest;                              \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfDialReqSend
 *
 *  DESCRIPTION
 *      Send the ATD command to the AG
 *
 *  PARAMETERS
 *       connectionId        The connection to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfDialReqSend(_connectionId,_command,_number) {            \
        CsrBtHfDialReq *msg = (CsrBtHfDialReq *)CsrPmemAlloc(sizeof(CsrBtHfDialReq)); \
        msg->type          = CSR_BT_HF_DIAL_REQ;                        \
        msg->connectionId  = _connectionId;                             \
        msg->command       = _command;                                  \
        msg->number = _number;                                          \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSecurityInReqSend
 *      CsrBtHfSecurityOutReqSend
 *
 *  DESCRIPTION
 *      Set the default security settings for new incoming/outgoing connections
 *
 *  PARAMETERS
 *       secLevel        The security level to use
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSecurityInReqSend(_appHandle, _secLevel) {               \
        CsrBtHfSecurityInReq *msg = (CsrBtHfSecurityInReq*)CsrPmemAlloc(sizeof(CsrBtHfSecurityInReq)); \
        msg->type = CSR_BT_HF_SECURITY_IN_REQ;                          \
        msg->appHandle = _appHandle;                                    \
        msg->secLevel = _secLevel;                                      \
        CsrBtHfMsgTransport(msg);}

#define CsrBtHfSecurityOutReqSend(_appHandle, _secLevel) {              \
        CsrBtHfSecurityOutReq *msg = (CsrBtHfSecurityOutReq*)CsrPmemAlloc(sizeof(CsrBtHfSecurityOutReq)); \
        msg->type = CSR_BT_HF_SECURITY_OUT_REQ;                         \
        msg->appHandle = _appHandle;                                    \
        msg->secLevel = _secLevel;                                      \
        CsrBtHfMsgTransport(msg);}

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetC2CAudioCodecReqSend
 *
 *  DESCRIPTION
 *    Set the AudioCodec Used for sco connections
 *
 *  PARAMETERS
 *    conType              Connection type
 *    value                Codec to use
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetC2CAudioCodecReqSend(_connectionId,_value){           \
        CsrBtHfSetC2cAudioCodecReq *msg = (CsrBtHfSetC2cAudioCodecReq*) CsrPmemAlloc(sizeof(CsrBtHfSetC2cAudioCodecReq)); \
        msg->type          = CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ;         \
        msg->connectionId  = _connectionId;                             \
        msg->value         = _value;                                    \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfGetC2CAdpcmLocalSupportedReqSend
 *
 *  DESCRIPTION
 *    Request info about local support for ADPCM
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfGetC2CAdpcmLocalSupportedReqSend(){                      \
        CsrBtHfGetC2cAdpcmLocalSupportedReq *msg = (CsrBtHfGetC2cAdpcmLocalSupportedReq*) CsrPmemAlloc(sizeof(CsrBtHfGetC2cAdpcmLocalSupportedReq)); \
        msg->type     = CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ;    \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfC2CSfReqSend
 *
 *  DESCRIPTION
 *    Request C2C settings
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfC2CSfReqSend(_connectionId,_number,_value){              \
        CsrBtHfC2cSfReq *msg = (CsrBtHfC2cSfReq *)CsrPmemAlloc(sizeof(CsrBtHfC2cSfReq)); \
        msg->type          = CSR_BT_HF_C2C_SF_REQ;                      \
        msg->number        = _number;                                   \
        msg->value         = _value;                                    \
        msg->connectionId  = _connectionId;                             \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetC2CSampleRateReqSend
 *
 *  DESCRIPTION
 *    Set the preferred sample rate for audio connections
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetC2CSampleRateReqSend(_connectionId,_value){           \
        CsrBtHfSetC2cSampleRateReq *msg = (CsrBtHfSetC2cSampleRateReq *)CsrPmemAlloc(sizeof(CsrBtHfSetC2cSampleRateReq)); \
        msg->type  = CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ;                 \
        msg->connectionId  = _connectionId;                             \
        msg->value = _value;                                            \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfC2CPowerReqSend
 *
 *  DESCRIPTION
 *    Set the power status of the HF device and inform the AG
 *    if the feature is suported in the remote device
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfC2CPowerReqSend(_connectionId,_value){                   \
        CsrBtHfC2cPwrReq *msg = (CsrBtHfC2cPwrReq *)CsrPmemAlloc(sizeof(CsrBtHfC2cPwrReq)); \
        msg->type  = CSR_BT_HF_C2C_PWR_REQ;                             \
        msg->connectionId  = _connectionId;                             \
        msg->value = _value;                                            \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfC2CBattReqSend
 *
 *  DESCRIPTION
 *    Set the battery status of the HF device and inform the AG
 *    if the feature is suported in the remote device
 *
 *----------------------------------------------------------------------------*/
#define CsrBtHfC2CBattReqSend(_connectionId,_value){                    \
        CsrBtHfC2cBattReq *msg = (CsrBtHfC2cBattReq *)CsrPmemAlloc(sizeof(CsrBtHfC2cBattReq)); \
        msg->type  = CSR_BT_HF_C2C_BATT_REQ;                            \
        msg->connectionId  = _connectionId;                             \
        msg->value = _value;                                            \
        CsrBtHfMsgTransport(msg); }

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetC2CGetSmsReqSend
 *
 *  DESCRIPTION
 *    Get the SMS received in the AG at the position given
 *
 *  PARAMETERS
 *      index       index of the SMS to read.
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetC2CGetSmsReqSend(_connectionId,_index){               \
        CsrBtHfC2cGetSmsReq *msg = (CsrBtHfC2cGetSmsReq *)CsrPmemAlloc(sizeof(CsrBtHfC2cGetSmsReq)); \
        msg->type  = CSR_BT_HF_C2C_GET_SMS_REQ;                         \
        msg->connectionId  = _connectionId;                             \
        msg->index = _index;                                            \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfFreeUpstreamMessageContents
 *
 *  DESCRIPTION
 *      During Bluetooth shutdown all allocated payload in the Synergy BT HF
 *      message must be deallocated. This is done by this function
 *
 *
 *    PARAMETERS
 *      eventClass :  Must be CSR_BT_HF_PRIM,
 *      msg:          The message received from Synergy BT HF
 *----------------------------------------------------------------------------*/
void CsrBtHfFreeUpstreamMessageContents(CsrUint16 eventClass, void * message);

/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfSetDeregisterTimeReqSend
 *
 *  DESCRIPTION
 *    Set the number of seconds to wait before deregistering a service record
 *    when an incoming SLC is established.
 *
 *  PARAMETERS
 *      waitSeconds:            number of seconds
 *----------------------------------------------------------------------------*/
#define CsrBtHfSetDeregisterTimeReqSend(_waitSeconds){                  \
        CsrBtHfDeregisterTimeReq *msg = (CsrBtHfDeregisterTimeReq *)CsrPmemAlloc(sizeof(CsrBtHfDeregisterTimeReq)); \
        msg->type  = CSR_BT_HF_DEREGISTER_TIME_REQ;                     \
        msg->waitSeconds = _waitSeconds;                                \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfIndicatorActivationReqSend
 *
 *  DESCRIPTION
 *    Enable and/or disable determined indicator indications
 *
 *  PARAMETERS
 *     indicatorBitMask:         bitmask with the indicators to enable/disable
 *----------------------------------------------------------------------------*/
#define CsrBtHfIndicatorActivationReqSend(_indicatorBitMask,_connectionId){ \
        CsrBtHfIndicatorActivationReq *msg = (CsrBtHfIndicatorActivationReq *)CsrPmemAlloc(sizeof(CsrBtHfIndicatorActivationReq)); \
        msg->type  = CSR_BT_HF_INDICATOR_ACTIVATION_REQ;                \
        msg->indicatorBitMask = _indicatorBitMask;                      \
        msg->connectionId     = _connectionId;                          \
        CsrBtHfMsgTransport(msg); }


/*----------------------------------------------------------------------------*
 *  NAME
 *      CsrBtHfUpdateSupportedCodecReqSend
 *
 *  DESCRIPTION
 *      Add support for Codec specified
 *
 *  PARAMETERS
 *     codec:       Codec to enable/disable    
 *     enable:      TRUE/FALSE
 *----------------------------------------------------------------------------*/
#define CsrBtHfUpdateSupportedCodecReqSend(_codec, _enable, _update){ \
        CsrBtHfUpdateSupportedCodecReq *msg = (CsrBtHfUpdateSupportedCodecReq *)CsrPmemAlloc(sizeof(CsrBtHfUpdateSupportedCodecReq)); \
        msg->type  = CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ;                \
        msg->codecMask = _codec;                  \
        msg->enable = _enable;                    \
        msg->sendUpdate = _update;                    \
        CsrBtHfMsgTransport(msg); }

#ifdef __cplusplus
}
#endif

#endif

