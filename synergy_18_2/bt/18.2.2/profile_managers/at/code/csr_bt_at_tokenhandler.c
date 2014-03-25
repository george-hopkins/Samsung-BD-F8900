/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"
#include "csr_bt_util.h"

static const atTokenActionHandler atTokenProviderHandlers[NUMBER_OF_TOKENS] =
{
    NULL,
    circuit109,
    circuit108,
    setToFactoryDefinedConfiguration,
    reqCompleteCapabilitiesList,
    reqManufacturerIdentification,
    reqModelIdentification,
    reqRevisionIdentification,
    answer,
    dial,
    commandEcho,
    hookControl,
    monitorSpeakerLoudness,
    monitorSpeakerMode,
    returnToOnlineState,
    selectPulseDialing,
    resultCodeSuppression,
    selectToneDialing,
    dceResponseFormat,
    resultCodeSelection,
    resetToDefaultConfiguration,
    automaticAnswer,
    cmdLineTerminationCharacter,
    responseFormattingCharacter,
    cmdLineEditingCharacter,
    pauseBeforeBlindDialing,
    ConnectionCompletionTimeout,
    commaDialModifierTime,
    automaticDisconnectDelay,
    readAutomaticAnswerValue,
    readCmdTerminationValue,
    readResponseFormattingValue,
    readCmdLineEditingValue,
    readPauseBeforeBlindDialingValue,
    readConnectionCompleteTimeoutValue,
    readCommaDialModifierTimeValue,
    readAutomaticDisconnectValue,
    sendUnknownExtendedCmd,
};


/*************************************************************************************
 atTokenHandler :
************************************************************************************/
void atTokenHandler(atInstanceData_t *atData)
{
    CsrBool flag = TRUE;
    if(atData->state == atOnlineCmd_s)
    {
        if(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == A_TOKEN ||
           atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == D_TOKEN   )
        {
            flag = FALSE;
            atResultCodeErrorHandler(atData);
        }
    }

    if(flag == TRUE)
    {
        if(atData->tokenFlipArray[EMPTY].receiveToken == NO_TOKEN)
        {
            atOKResultResHandler(atData);
        }
        else if((atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken < NUMBER_OF_TOKENS) && atTokenProviderHandlers[atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken] != NULL)
        {
            atData->tokenValue = atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveValue;
            atTokenProviderHandlers[atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken](atData);
        }

        if(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == A_TOKEN)
        {
            atData->numberOfRespon = 1;
        }
    }
}


/*************************************************************************************
Give response to &C
************************************************************************************/
void circuit109(atInstanceData_t *atData)
{
    CsrBtAtDgAtcAndCInd *atPrim;
    atPrim    = (CsrBtAtDgAtcAndCInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcAndCInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_AND_C_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to &D
************************************************************************************/
void circuit108(atInstanceData_t *atData)
{
    CsrBtAtDgAtcAndDInd *atPrim;
    atPrim    = (CsrBtAtDgAtcAndDInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcAndDInd));

    atData->tempparamSettings.andDValue = atData->tokenValue;
    atData->changeSettings++;

    atPrim->type    = CSR_BT_AT_DG_ATC_AND_D_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to &F
************************************************************************************/
void setToFactoryDefinedConfiguration(atInstanceData_t *atData)
{
    CsrBtAtDgAtcAndFInd *atPrim;
    atPrim    = (CsrBtAtDgAtcAndFInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcAndFInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_AND_F_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to +GCAP
************************************************************************************/
void reqCompleteCapabilitiesList(atInstanceData_t *atData)
{

#ifdef CSR_BT_AT_SEND_GCAP_TOKEN_IND
    CsrBtAtDgAtcGcapInd *atPrim;
    atPrim    = (CsrBtAtDgAtcGcapInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcGcapInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_GCAP_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);

#else
    CsrUint16 Length = (CsrUint16) (CsrStrLen(CSR_BT_AT_GCAP_TEXT));
    atData->numberOfRespon--;
    atGxxHelpResHandler(atData, Length, (CsrUint8*) CSR_BT_AT_GCAP_TEXT);
    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }

#endif
}


/*************************************************************************************
Give response to +GMI
************************************************************************************/
void reqManufacturerIdentification(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_GMI_TOKEN_IND
    CsrBtAtDgAtcGmiInd *atPrim;
    atPrim    = (CsrBtAtDgAtcGmiInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcGmiInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_GMI_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    CsrUint16 Length = (CsrUint16) (CsrStrLen(CSR_BT_AT_GMI_TEXT));
    atData->numberOfRespon--;
    atGxxHelpResHandler(atData, Length, (CsrUint8*) CSR_BT_AT_GMI_TEXT);
    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to +GMM
************************************************************************************/
void reqModelIdentification(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_GMM_TOKEN_IND
    CsrBtAtDgAtcGmmInd *atPrim;
    atPrim    = (CsrBtAtDgAtcGmmInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcGmmInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_GMM_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    CsrUint16 Length = (CsrUint16) (CsrStrLen(CSR_BT_AT_GMM_TEXT));
    atData->numberOfRespon--;
    atGxxHelpResHandler(atData, Length, (CsrUint8*) CSR_BT_AT_GMM_TEXT);
    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to +GMR
************************************************************************************/
void reqRevisionIdentification(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_GMR_TOKEN_IND
    CsrBtAtDgAtcGmrInd *atPrim;
    atPrim    = (CsrBtAtDgAtcGmrInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcGmrInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_GMR_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    CsrUint16 Length = (CsrUint16) (CsrStrLen(CSR_BT_AT_GMM_TEXT));
    atData->numberOfRespon--;
    atGxxHelpResHandler(atData, Length, (CsrUint8*) CSR_BT_AT_GMM_TEXT);
    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to A
************************************************************************************/
void answer(atInstanceData_t *atData)
{
    CsrBtAtDgAtcAInd *atPrim;
    atPrim    = (CsrBtAtDgAtcAInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcAInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_A_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to D
************************************************************************************/
void dial(atInstanceData_t *atData)
{
    CsrBtAtDgAtcDInd *atPrim;
    int index;
    atPrim    = (CsrBtAtDgAtcDInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcDInd));

    atPrim->type            = CSR_BT_AT_DG_ATC_D_IND;
    atPrim->payload            = (CsrUint8 *) CsrPmemAlloc(atData->dialStringCounter);
    for (index = 0; index < atData->dialStringCounter; index++)
    {
        atPrim->payload[index] = atData->dialString[index];
    }
    atPrim->payloadLength    = atData->dialStringCounter;
    atPrim->value            = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}

/*************************************************************************************
Give response to E
************************************************************************************/
void commandEcho(atInstanceData_t *atData)
{
    atData->tempparamSettings.eValue = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_E_TOKEN_IND
    {
        CsrBtAtDgAtcEInd *atPrim;
        atPrim    = (CsrBtAtDgAtcEInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcEInd));

        atPrim->type    = CSR_BT_AT_DG_ATC_E_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to H
************************************************************************************/
void hookControl(atInstanceData_t *atData)
{
    CsrBtAtDgAtcHInd *atPrim;
    atPrim    = (CsrBtAtDgAtcHInd *)  CsrPmemAlloc(sizeof(CsrBtAtDgAtcHInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_H_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}

/*************************************************************************************
Give response to L
************************************************************************************/
void monitorSpeakerLoudness(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_L_TOKEN_IND

    CsrBtAtDgAtcLInd *atPrim;
    atPrim    = (CsrBtAtDgAtcLInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcLInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_L_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atResultCodeOKHandler(atData);
#endif
}


/*************************************************************************************
Give response to M
************************************************************************************/
void monitorSpeakerMode(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_M_TOKEN_IND
    CsrBtAtDgAtcMInd *atPrim;
    atPrim    = (CsrBtAtDgAtcMInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcMInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_M_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atResultCodeOKHandler(atData);
#endif
}


/*************************************************************************************
Give response to O
************************************************************************************/
void returnToOnlineState(atInstanceData_t *atData)
{
    CsrBtAtDgAtcOInd *atPrim;
    atPrim    = (CsrBtAtDgAtcOInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcOInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_O_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to P
************************************************************************************/
void selectPulseDialing(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_P_TOKEN_IND
    CsrBtAtDgAtcPInd *atPrim;
    atPrim    = (CsrBtAtDgAtcPInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcPInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_P_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to Q
************************************************************************************/
void resultCodeSuppression(atInstanceData_t *atData)
{
    atData->tempparamSettings.qValue = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_Q_TOKEN_IND
    {
        CsrBtAtDgAtcQInd *atPrim;
        atPrim    = (CsrBtAtDgAtcQInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcQInd));

        atPrim->type    = CSR_BT_AT_DG_ATC_Q_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}


/*************************************************************************************
Give response to T
************************************************************************************/
void selectToneDialing(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_T_TOKEN_IND
    CsrBtAtDgAtcTInd *atPrim;
    atPrim    = (CsrBtAtDgAtcTInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcTInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_T_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to V
************************************************************************************/
void dceResponseFormat(atInstanceData_t *atData)
{
    atData->tempparamSettings.vValue = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_V_TOKEN_IND
    {
        CsrBtAtDgAtcVInd *atPrim;
        atPrim    = (CsrBtAtDgAtcVInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcVInd));

        atPrim->type    = CSR_BT_AT_DG_ATC_V_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}


/*************************************************************************************
Give response to X
************************************************************************************/
void resultCodeSelection(atInstanceData_t *atData)
{
    CsrBtAtDgAtcXInd *atPrim;
    atPrim    = (CsrBtAtDgAtcXInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcXInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_X_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to Z
************************************************************************************/
void resetToDefaultConfiguration(atInstanceData_t *atData)
{
    CsrBtAtDgAtcZInd *atPrim;
    atPrim    = (CsrBtAtDgAtcZInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcZInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_Z_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}


/*************************************************************************************
Give response to S0=
************************************************************************************/
void automaticAnswer(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S0_TOKEN_IND
    CsrBtAtDgAtcS0Ind *atPrim;
    atPrim    = (CsrBtAtDgAtcS0Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS0Ind));

    atPrim->type    = CSR_BT_AT_DG_ATC_S0_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->paramSettings.s0Value = atData->tokenValue;
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S3=
************************************************************************************/
void cmdLineTerminationCharacter(atInstanceData_t *atData)
{
    atData->tempparamSettings.s3Value = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_S3_TOKEN_IND
    {
        CsrBtAtDgAtcS3Ind *atPrim;
        atPrim    = (CsrBtAtDgAtcS3Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS3Ind));

        atPrim->type    = CSR_BT_AT_DG_ATC_S3_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S4=
************************************************************************************/
void responseFormattingCharacter(atInstanceData_t *atData)
{
    atData->tempparamSettings.s4Value = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_S4_TOKEN_IND
    {
        CsrBtAtDgAtcS4Ind *atPrim;
        atPrim    = (CsrBtAtDgAtcS4Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS4Ind));

        atPrim->type    = CSR_BT_AT_DG_ATC_S4_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S5=
************************************************************************************/
void cmdLineEditingCharacter(atInstanceData_t *atData)
{
    atData->tempparamSettings.s5Value = atData->tokenValue;
    atData->changeSettings++;

#ifdef CSR_BT_AT_SEND_S5_TOKEN_IND
    {
        CsrBtAtDgAtcS5Ind *atPrim;
        atPrim    = (CsrBtAtDgAtcS5Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS5Ind));

        atPrim->type    = CSR_BT_AT_DG_ATC_S5_IND;
        atPrim->value    = atData->tokenValue;
        CsrBtAtMessagePut(atData->appHandle, atPrim);
    }
#else
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S6=
************************************************************************************/
void pauseBeforeBlindDialing(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S6_TOKEN_IND
    CsrBtAtDgAtcS6Ind *atPrim;
    atPrim    = (CsrBtAtDgAtcS6Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS6Ind));

    atPrim->type    = CSR_BT_AT_DG_ATC_S6_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->paramSettings.s6Value = atData->tokenValue;
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S7=
************************************************************************************/
void ConnectionCompletionTimeout(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S7_TOKEN_IND
    CsrBtAtDgAtcS7Ind *atPrim;
    atPrim    = (CsrBtAtDgAtcS7Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS7Ind));

    atPrim->type    = CSR_BT_AT_DG_ATC_S7_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->paramSettings.s7Value = atData->tokenValue;
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S8=
************************************************************************************/
void commaDialModifierTime(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S8_TOKEN_IND
    CsrBtAtDgAtcS8Ind *atPrim;
    atPrim    = (CsrBtAtDgAtcS8Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS8Ind));

    atPrim->type    = CSR_BT_AT_DG_ATC_S8_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->paramSettings.s8Value = atData->tokenValue;
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S10=
************************************************************************************/
void automaticDisconnectDelay(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S10_TOKEN_IND
    CsrBtAtDgAtcS10Ind *atPrim;
    atPrim    = (CsrBtAtDgAtcS10Ind *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS10Ind));

    atPrim->type    = CSR_BT_AT_DG_ATC_S10_IND;
    atPrim->value    = atData->tokenValue;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->paramSettings.s10Value = atData->tokenValue;
    atResultCodeOKHandler(atData);
#endif
}

/*************************************************************************************
Give response to S0?
************************************************************************************/
void readAutomaticAnswerValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S0_TOKEN_IND
    CsrBtAtDgAtcS0qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS0qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS0qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S0Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s0Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}

/*************************************************************************************
Give response to S3?
************************************************************************************/
void readCmdTerminationValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S3_TOKEN_IND
    CsrBtAtDgAtcS3qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS3qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS3qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S3Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s3Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to S4?
************************************************************************************/
void readResponseFormattingValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S4_TOKEN_IND
    CsrBtAtDgAtcS4qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS4qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS4qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S4Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s4Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to S5?
************************************************************************************/
void readCmdLineEditingValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S5_TOKEN_IND
    CsrBtAtDgAtcS5qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS5qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS5qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S5Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s5Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to S6?
************************************************************************************/
void readPauseBeforeBlindDialingValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S6_TOKEN_IND
    CsrBtAtDgAtcS6qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS6qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS6qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S6Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s6Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}

/*************************************************************************************
Give response to S7?
************************************************************************************/
void readConnectionCompleteTimeoutValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S7_TOKEN_IND
    CsrBtAtDgAtcS7qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS7qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS7qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S7Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s7Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to S8?
************************************************************************************/
void readCommaDialModifierTimeValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S8_TOKEN_IND
    CsrBtAtDgAtcS8qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS8qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS8qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S8Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s8Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
Give response to S10?
************************************************************************************/
void readAutomaticDisconnectValue(atInstanceData_t *atData)
{
#ifdef CSR_BT_AT_SEND_S10_TOKEN_IND
    CsrBtAtDgAtcS10qInd *atPrim;
    atPrim    = (CsrBtAtDgAtcS10qInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcS10qInd));

    atPrim->type    = CSR_BT_AT_DG_ATC_S10Q_IND;
    CsrBtAtMessagePut(atData->appHandle, atPrim);
#else
    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atData->paramSettings.s10Value);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
#endif
}


/*************************************************************************************
sendUnknownExtendedCmd : Send a unknown extended command to app
************************************************************************************/
void sendUnknownExtendedCmd(atInstanceData_t *atData)
{
    CsrBtAtDgAtcUnknownExtendedCmdInd *atPrim;

    atPrim        = (CsrBtAtDgAtcUnknownExtendedCmdInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcUnknownExtendedCmdInd));

    atPrim->type            = CSR_BT_AT_DG_ATC_UNKNOWN_EXTENDED_CMD_IND;
    atPrim->payload         = CsrPmemAlloc(atData->unknownExtendedCmdLength[atData->tokenValue]);
    CsrMemCpy(atPrim->payload, atData->atUnknownExtendedCmd[atData->tokenValue], atData->unknownExtendedCmdLength[atData->tokenValue]);
    atPrim->payloadLength    = (CsrUint16) atData->unknownExtendedCmdLength[atData->tokenValue];
    CsrBtAtMessagePut(atData->appHandle, atPrim);
}

#endif

