/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	Kim Jin Cheol	(SW Platform., HYUNDAI MOBIS)
*	Updates			:
***********************************************************************/
#include "csr_bt_declare_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf_prim.h"
#include "Bluetooth_HF.h"
#include "Bluetooth_Common.h"
#include "csr_pmem.h"
/*****************************************************************************
* Function Name : init_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/



void init_hf(void **gash)
{
	HfAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(HfAppInst));
	CsrMemSet(*gash, 0, sizeof(HfAppInst));
	inst = (HfAppInst *)*gash;

	inst->qid = TESTQUEUE;
	inst->connectionId=0xff;
	CsrBtHfGetC2CAdpcmLocalSupportedReqSend();
	
	CsrBtHfActivateReqSend(inst->qid, 1,1,
						  2,(CSR_BT_HF_SUPPORT_EC_NR_FUNCTION | CSR_BT_HF_SUPPORT_CALL_WAITING_THREE_WAY_CALLING | CSR_BT_HF_SUPPORT_CLI_PRESENTATION_CAPABILITY | CSR_BT_HF_SUPPORT_VOICE_RECOGNITION | CSR_BT_HF_SUPPORT_REMOTE_VOLUME_CONTROL | CSR_BT_HF_SUPPORT_ENHANCED_CALL_STATUS | CSR_BT_HF_SUPPORT_ENHANCED_CALL_CONTROL),
						  CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CCWA_ACTIVATION|CSR_BT_HF_CNF_DISABLE_AUTOMATIC_CMEE_ACTIVATION,5);

}

/*****************************************************************************
* Function Name : deinit_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
#ifdef ENABLE_SHUTDOWN
void deinit_hf(void **gash)
{
	HfAppInst *inst;

	inst = *gash;

	CsrPmemFree(inst);
}

#else
void deinit_hf(void **gash)
{
    CSR_UNUSED(gash);
}
#endif


/*****************************************************************************
* Function Name : handleHfPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/

void handleHfPrim(HfAppInst  *instData)
{
    CsrPrim    *thePrim;
    thePrim = (CsrPrim *) instData->recvMsgP;

	switch (*thePrim)
	{
        /******************************************************** INDICATION MESSAGES *********************************/
        case CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND:
            {
                CsrBtHfAudioAcceptConnectInd *hfPrim;
                hfPrim = (CsrBtHfAudioAcceptConnectInd *)thePrim;
				instData->connectionId=hfPrim->connectionId;


				CsrBtHfAudioAcceptResSend(instData->connectionId, HCI_SUCCESS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);

				//CsrBtHfAudioAcceptResSend(instData->connectionId, HCI_ERROR_REJ_BY_REMOTE_PERS, NULL, PCM_SLOT, PCM_SLOT_REALLOCATE);

					 
			    CL_API_LOGPrintf("CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND \n");
                break;
            }
        case CSR_BT_HF_DISCONNECT_IND:
        case CSR_BT_HF_DISCONNECT_CFM:
            {
                CsrBtHfDisconnectInd *prim;

                prim = (CsrBtHfDisconnectInd *) thePrim;

				 CL_API_LOGPrintf("CSR_BT_HF_DISCONNECT_CFM"); //CSR_BT_RESULT_CODE_HF_SUCCESS//CSR_BT_SUPPLIER_HF

				instData->connectionId=prim->connectionId;

				
                break;
            }
        case CSR_BT_HF_AUDIO_DISCONNECT_CFM:
            {
                CsrBtHfAudioDisconnectCfm * prim;
                prim = (CsrBtHfAudioDisconnectCfm *) thePrim;
				instData->connectionId=prim->connectionId;

				CL_API_LOGPrintf("CSR_BT_HF_AUDIO_DISCONNECT_CFM %x\n",prim->resultCode);
 //               if ((prim->resultSupplier == CSR_BT_SUPPLIER_HF) && (prim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS))
 //               {
 //                   CsrHciDeRegisterScoHandle(prim->scoHandle);
 //               }

                break;
            }
        case CSR_BT_HF_AUDIO_DISCONNECT_IND:
            {
                CsrBtHfAudioDisconnectInd * prim;
                prim = (CsrBtHfAudioDisconnectInd *) thePrim;
				instData->connectionId=prim->connectionId;
		//		CsrHciDeRegisterScoHandle(prim->scoHandle);

			    CL_API_LOGPrintf("CSR_BT_HF_AUDIO_DISCONNECT_IND %x\n",prim->reasonCode);
                break;
            }
        case CSR_BT_HF_AUDIO_CONNECT_CFM:
        case CSR_BT_HF_AUDIO_CONNECT_IND:
            {
//                CsrBool retVal;
                CsrBtHfAudioConnectInd *prim;
                prim = (CsrBtHfAudioConnectInd *) thePrim;
				instData->connectionId=prim->connectionId;
/*                   if ((prim->resultSupplier == CSR_BT_SUPPLIER_HF) && (prim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS))
                {
                 retVal = CsrHciRegisterScoHandle(prim->scoHandle, (CsrHciScoHandlerFuncType)inComingBcspSound);
                    if (TRUE == retVal)
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "Telling Chip to use audio codec: 0x%02x\n", instData->audioCodecType);
                    }
                    else
                    {
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "SCO connection was not set up due to missing free SCO handles.\n");
                    }
                }
*/
				CL_API_LOGPrintf("CSR_BT_HF_AUDIO_CONNECT_IND %x\n",prim->resultCode);		

                break;
            }

        case CSR_BT_HF_SERVICE_CONNECT_CFM:
        case CSR_BT_HF_SERVICE_CONNECT_IND:
			{
			    CsrBtHfServiceConnectInd *myPrim;

			    myPrim = (CsrBtHfServiceConnectInd *) thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_SERVICE_CONNECT_IND %x\n",myPrim->connectionId);

				instData->connectionId =myPrim->connectionId;


			    if ((myPrim->resultCode == CSR_BT_RESULT_CODE_HF_SUCCESS) &&
			        (myPrim->resultSupplier == CSR_BT_SUPPLIER_HF) )
			    {
			        

			        if (myPrim->connectionType == CSR_BT_HF_CONNECTION_HF)
			        {
			            CL_API_LOGPrintf( "SLC established to: %s Remote HFP version supported: 0x%02x \n", myPrim->serviceName, myPrim->remoteVersion);

			            /* Send microphone level */
			//	                        if(instData->conInstData[instData->currentConnection].startup == STARTUP_MIC)
			//	                        {
			//	                            CsrSchedTimerSet(CSR_SCHED_SECOND*2,
			//	                                           startup_mic_gain,
			//	                                           instData->currentConnection,
			//	                                           instData);
			//	                        }
			//	                        instData->conInstData[instData->currentConnection].hfgSupportedFeatures = myPrim->supportedFeatures;
			//	                        instData->conInstData[instData->currentConnection].remoteVersion  = myPrim->remoteVersion;
			        }
			        else
			        {
			            CL_API_LOGPrintf("Headset connected to %s\n", myPrim->serviceName);
			        }


			        if (myPrim->indicatorSupported != NULL)
			        {
			            CsrUint16 len = (CsrUint16)(CsrStrLen((char*)myPrim->indicatorSupported) + 1);
			            CL_API_LOGPrintf("Remote indicators supported: %s\n",myPrim->indicatorSupported);

			            CsrPmemFree(myPrim->indicatorSupported);
			        }
			        if (myPrim->indicatorValue != NULL)
			        {
			            CL_API_LOGPrintf( "Actual indicator values: %s \n",myPrim->indicatorValue);

			            CsrPmemFree(myPrim->indicatorValue);
			        }
			        if (myPrim->chldString != NULL)
			        {
			            CL_API_LOGPrintf( "Call hold supported string: %s\n", myPrim->chldString);
			            CsrPmemFree(myPrim->chldString);
			        }
			        CsrPmemFree(myPrim->serviceName);
			    }
			    else
			    {

			       CL_API_LOGPrintf("Service level connect failed, code: 0x%02X, supplier: 0x%02X\n", myPrim->resultCode, myPrim->resultSupplier);

			    }
			    break;
			}
        case CSR_BT_HF_STATUS_LOW_POWER_IND:
            {
                CsrBtHfStatusLowPowerInd *myPrim;
                myPrim = (CsrBtHfStatusLowPowerInd *) thePrim;
				instData->connectionId=myPrim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_STATUS_LOW_POWER_IND \n");

                break;
            }

        case CSR_BT_HF_SPEAKER_GAIN_IND:
            {
                CsrBtHfSpeakerGainInd *myPrim;

                myPrim = (CsrBtHfSpeakerGainInd *) thePrim;
				instData->connectionId=myPrim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_SPEAKER_GAIN_IND \n");

                break;
            }
        case CSR_BT_HF_MIC_GAIN_IND:
            {
                CsrBtHfMicGainInd *myPrim;

                myPrim = (CsrBtHfMicGainInd *) thePrim;
				instData->connectionId=myPrim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_MIC_GAIN_IND \n");

                break;
            }
        case CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND:
            {
                CsrBtHfInbandRingSettingChangedInd *myPrim;

                myPrim = (CsrBtHfInbandRingSettingChangedInd *) thePrim;
                instData->connectionId=myPrim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND \n");

                break;
            }
        case CSR_BT_HF_CALL_RINGING_IND:
            {
                CsrBtHfCallRingingInd *myPrim;

                myPrim = (CsrBtHfCallRingingInd *) thePrim;
				instData->connectionId=myPrim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_CALL_RINGING_IND \n");

                break;
            }
        case CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND:
            {
                CsrBtHfStatusIndicatorUpdateInd * prim;
                prim = (CsrBtHfStatusIndicatorUpdateInd *) thePrim;
				instData->connectionId=prim->connectionId;
				CL_API_LOGPrintf("CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND \n");

                CsrPmemFree(prim->name);
                break;
            }

        case CSR_BT_HF_CALL_HANDLING_IND:
            {
                CsrBtHfCallHandlingInd *prim;
                prim = (CsrBtHfCallHandlingInd *) thePrim;
				instData->connectionId=prim->connectionId;

                CL_API_LOGPrintf("CSR_BT_HF_CALL_HANDLING_IND \n");
                break;
            }
        case CSR_BT_HF_CALL_NOTIFICATION_IND:
            {
                CsrBtHfCallNotificationInd *prim;

                prim = (CsrBtHfCallNotificationInd *) thePrim;
                instData->connectionId=prim->connectionId;
                CL_API_LOGPrintf("CSR_BT_HF_CALL_NOTIFICATION_IND \n");
                CsrPmemFree(prim->clipString);
                return;
             }
        case CSR_BT_HF_SET_VOICE_RECOGNITION_IND:
            {
                CsrBtHfSetVoiceRecognitionInd *myPrim;
                myPrim = (CsrBtHfSetVoiceRecognitionInd *)thePrim;
				instData->connectionId=myPrim->connectionId;
                CL_API_LOGPrintf("CSR_BT_HF_SET_VOICE_RECOGNITION_IND \n");
                break;
            }
        case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND:
            {
                CsrBtHfGetSubscriberNumberInformationInd *prim;
                prim = (CsrBtHfGetSubscriberNumberInformationInd *) thePrim;
                
                instData->connectionId=prim->connectionId;

                if (prim->cnumString != NULL)
                {   
 
                    CsrPmemFree(prim->cnumString);
                }
                else
                {   
                    CL_API_LOGPrintf( "No subscriber number information received \n");
                }


                break;
            }
        case CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND:
            {
				CL_API_LOGPrintf("CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND \n");

                break;
            }
        case CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND:
            {
				CL_API_LOGPrintf("CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND \n");

                break;
            }
        case CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND:
            {
                CsrBtHfCallWaitingNotificationInd *myPrim;
                myPrim = (CsrBtHfCallWaitingNotificationInd *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND \n");


                CsrPmemFree(myPrim->ccwaString);
                break;
            }
        case CSR_BT_HF_AT_CMD_IND:
            {
                CsrBtHfAtCmdInd *myPrim;
                myPrim = (CsrBtHfAtCmdInd *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_AT_CMD_IND \n");

                CsrPmemFree(myPrim->atCmdString);
                return; /*nothing else to be done*/
            }
        case CSR_BT_HF_GET_CURRENT_CALL_LIST_IND:
            {
                CsrBtHfGetCurrentCallListInd *myPrim;

                myPrim = (CsrBtHfGetCurrentCallListInd *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_GET_CURRENT_CALL_LIST_IND \n");

                CsrPmemFree(myPrim->clccString);
                break;
            }
        case CSR_BT_HF_C2C_SF_IND:
            {
                CsrBtHfC2cSfInd *myPrim = (CsrBtHfC2cSfInd *) thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_C2C_SF_IND \n");

                CsrPmemFree(myPrim->indicators);
                break;
            }
        case CSR_BT_HF_C2C_BATT_IND:
            {/* The remote device asks about the battery status: send it */
                CsrBtHfC2cBattInd *myPrim = (CsrBtHfC2cBattInd *)thePrim;
  		        CL_API_LOGPrintf("CSR_BT_HF_C2C_BATT_IND \n");
				CsrBtHfC2CBattReqSend(myPrim->connectionId, 0x05);
                break;
            }
        case CSR_BT_HF_C2C_TXT_IND:
            {
//                CsrCharString *buf2;
                CsrBtHfC2cTxtInd *myPrim = (CsrBtHfC2cTxtInd *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_C2C_TXT_IND \n");

                if (myPrim->txtString != NULL)
                {
                    CsrPmemFree(myPrim->txtString);
                }
                return;
            }
        case CSR_BT_HF_C2C_SMS_IND:
            {/* New SMS received: get text of SMS 'index' */
                CsrBtHfC2cSmsInd *myPrim = (CsrBtHfC2cSmsInd *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_C2C_SMS_IND \n");
				CsrBtHfSetC2CGetSmsReqSend(myPrim->connectionId,myPrim->index);

                break;
            }
         /***************************************** CONFIRMATION MESSAGES **********************************/
        case CSR_BT_HF_CONFIG_AUDIO_CFM:
            {
               CL_API_LOGPrintf("CSR_BT_HF_CONFIG_AUDIO_CFM \n");
                break;
            }
        case CSR_BT_HF_CONFIG_LOW_POWER_CFM:
            {
          CL_API_LOGPrintf("CSR_BT_HF_CONFIG_LOW_POWER_CFM \n");
                break;
            }
        case CSR_BT_HF_ACTIVATE_CFM:
            {
              CL_API_LOGPrintf("CSR_BT_HF_ACTIVATE_CFM \n");
                break;
            }

        case CSR_BT_HF_MIC_GAIN_STATUS_CFM:
            {
                CsrBtHfMicGainStatusCfm *myPrim;
                myPrim = (CsrBtHfMicGainStatusCfm *)thePrim;

                break;
            }
        case CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM:
            {
                CsrBtHfSpeakerGainStatusCfm *myPrim;
                myPrim = (CsrBtHfSpeakerGainStatusCfm *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM \n");

                break;
            }
        case CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM:
            {
                CsrBtHfSetCallWaitingNotificationCfm *myPrim;
                myPrim = (CsrBtHfSetCallWaitingNotificationCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM \n");

                break;
            }
        case CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM:
            {
                CsrBtHfGetSubscriberNumberInformationCfm *myPrim;
                myPrim = (CsrBtHfGetSubscriberNumberInformationCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM \n");

                break;
            }
        case CSR_BT_HF_SET_VOICE_RECOGNITION_CFM:
            {
                CsrBtHfSetVoiceRecognitionCfm *myPrim;
                myPrim = (CsrBtHfSetVoiceRecognitionCfm *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_SET_VOICE_RECOGNITION_CFM \n");

                break;
            }
        case CSR_BT_HF_SET_ECHO_AND_NOISE_CFM:
            {
                CsrBtHfSetEchoAndNoiseCfm *myPrim;
                myPrim = (CsrBtHfSetEchoAndNoiseCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_SET_ECHO_AND_NOISE_CFM \n");

                break;
            }
        case CSR_BT_HF_GENERATE_DTMF_CFM:
            {
                CsrBtHfGenerateDtmfCfm *myPrim;
                myPrim = (CsrBtHfGenerateDtmfCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_GENERATE_DTMF_CFM \n");

                break;
            }
        case CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM:
            {
                CsrBtHfGetCurrentOperatorSelectionCfm * myPrim;

                myPrim = (CsrBtHfGetCurrentOperatorSelectionCfm *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM \n");


                break;
            }
        case CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM:
            {
                CsrBtHfSetExtendedAgErrorResultCodeCfm *myPrim;
                myPrim = (CsrBtHfSetExtendedAgErrorResultCodeCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM \n");

                break;
            }
        case CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM:
            {
                CsrBtHfSetCallNotificationIndicationCfm *myPrim;
                myPrim = (CsrBtHfSetCallNotificationIndicationCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM \n");
				
                break;
            }
        case CSR_BT_HF_BT_INPUT_CFM:
            {
                CsrBtHfBtInputCfm *myPrim;
                myPrim = (CsrBtHfBtInputCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_BT_INPUT_CFM \n");

                if (myPrim->dataRespString != NULL)
                {
                    CsrPmemFree(myPrim->dataRespString);
                }
                break;
            }

        case CSR_BT_HF_DEACTIVATE_CFM:
            {
                CsrBtHfDeactivateCfm *myPrim = (CsrBtHfDeactivateCfm *)thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_DEACTIVATE_CFM \n");

                break;
            }
        case CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM:
            {
                CsrBtHfGetCurrentCallListCfm *myPrim;
                myPrim = (CsrBtHfGetCurrentCallListCfm *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM \n");

                break;
            }
        case CSR_BT_HF_DIAL_CFM:
            {
                CsrBtHfDialCfm *myPrim;
                myPrim = (CsrBtHfDialCfm *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_DIAL_CFM \n");


                break;
            }
        case CSR_BT_HF_CALL_HANDLING_CFM:
            {
                CsrBtHfCallHandlingCfm *myPrim;

                myPrim = (CsrBtHfCallHandlingCfm *) thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_CALL_HANDLING_CFM \n");

                break;
            }
        case CSR_BT_HF_CALL_ANSWER_CFM:
            {
                CsrBtHfCallAnswerCfm *myPrim;
                myPrim = (CsrBtHfCallAnswerCfm *) thePrim;

				CL_API_LOGPrintf("CSR_BT_HF_CALL_ANSWER_CFM \n");

                break;
            }
        case CSR_BT_HF_CALL_END_CFM:
            {
                CsrBtHfCallEndCfm *myPrim;
                myPrim = (CsrBtHfCallEndCfm *) thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_CALL_END_CFM \n");


                break;
            }
        case CSR_BT_HF_AT_CMD_CFM:
            {
                CsrBtHfAtCmdCfm *myPrim = (CsrBtHfAtCmdCfm *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_AT_CMD_CFM \n");

                break;
            }
        case CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM:
            {
                CsrBtHfGetAllStatusIndicatorsCfm *myPrim = (CsrBtHfGetAllStatusIndicatorsCfm *)thePrim;
                if (myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS)
                {
                    CL_API_LOGPrintf( "The remote device's status indicators are: %s \n and have the values: %s\n",myPrim->indicatorSupported,myPrim->indicatorValue);
                    CsrPmemFree(myPrim->indicatorSupported);
                    CsrPmemFree(myPrim->indicatorValue);
                }
                else
                {
                    CL_API_LOGPrintf( "The AT command on connection 0x%4X failed; result: 0x%x\n",myPrim->connectionId,myPrim->cmeeResultCode);
                }

                break;
            }
        case CSR_BT_HF_C2C_BATT_CFM:
            {
                CsrBtHfC2cBattCfm *myPrim = (CsrBtHfC2cBattCfm *)thePrim;
                CL_API_LOGPrintf( "CSR_BT_HF_C2C_BATT_CFM received; result code: %d\n", myPrim->cmeeResultCode);
                break;
            }
        case CSR_BT_HF_C2C_PWR_CFM:
            {
                CsrBtHfC2cPwrCfm *myPrim = (CsrBtHfC2cPwrCfm *)thePrim;
                CL_API_LOGPrintf( "CSR_BT_HF_C2C_PWR_CFM received; result code: %d\n", myPrim->cmeeResultCode);
                break;
            }
        case CSR_BT_HF_C2C_GET_SMS_CFM:
            {
                CsrBtHfC2cGetSmsCfm *myPrim = (CsrBtHfC2cGetSmsCfm *)thePrim;
                if ((myPrim->cmeeResultCode == CSR_BT_CME_SUCCESS) && (myPrim->smsString != NULL))
                {
                    CL_API_LOGPrintf("SMS retrieved: %s \n",myPrim->smsString);
                }
                else
                {
                    CL_API_LOGPrintf("CSR_BT_HF_C2C_GET_SMS_CFM received; result Code: %d \n",myPrim->cmeeResultCode);
                }

                if (myPrim->smsString != NULL)
                {
                    CsrPmemFree(myPrim->smsString);
                }
                break;
            }
        case CSR_BT_HF_INDICATOR_ACTIVATION_CFM:
            {
                CsrBtHfIndicatorActivationCfm *myPrim = (CsrBtHfIndicatorActivationCfm  *)thePrim;
				CL_API_LOGPrintf("CSR_BT_HF_INDICATOR_ACTIVATION_CFM \n");

            }
        default:
            {
                CL_API_LOGPrintf( "####### default in hf prim handler 0x%04x,\n", *thePrim);
                CsrBtHfFreeUpstreamMessageContents(*thePrim, thePrim);
                /*  unexpected primitive received   */
                break;
            }
    }

}
