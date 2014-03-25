/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

static const atResultCodeResponsHandler atResponseHandlers[CSR_BT_NUMBER_OF_RESULTCODES] =
{
    atResultCodeOKHandler,
    atResultCodeConnectHandler,
    atResultCodeRingHandler,
    atResultCodeNoCarrierHandler,
    atResultCodeErrorHandler,
    NULL,
    atResultCodeNoDialToneHandler,
    atResultCodeBusyHandler,
    atResultCodeNoAnswerHandler,
};


/*************************************************************************************
atResultCodeResHandler
************************************************************************************/
void atResultCodeResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcResultCodeRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcResultCodeRes *) atData->atMsgP;

    if((atPrim->atResult < CSR_BT_NUMBER_OF_RESULTCODES) && atResponseHandlers[atPrim->atResult] != NULL)
    {
        atResponseHandlers[atPrim->atResult](atData);
    }
}


/*************************************************************************************
atResultCodeOKHandler
************************************************************************************/
void atResultCodeOKHandler(atInstanceData_t *atData)
{
    if(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == H_TOKEN)
    {
        atData->state = atCmd_s;
    }

    if(atData->changeSettings > EMPTY)
    {
        atData->changeSettings--;
        paramChangeHandler(atData);
    }
    atData->numberOfRespon--;

    if(atData->numberOfRespon <= EMPTY)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }

}

/*************************************************************************************
atResultCodeConnectHandler
************************************************************************************/
void atResultCodeConnectHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;
    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen   = 2;
            dataPtr       = CsrPmemAlloc(valueLen);
            dataPtr[0] = 1;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            valueLen    = 11;
            dataPtr        = CsrPmemAlloc(valueLen);
            dataPtr[0]  = atData->paramSettings.s3Value;
            dataPtr[1]  = atData->paramSettings.s4Value;
            dataPtr[2]  = 'C';
            dataPtr[3]  = 'O';
            dataPtr[4]  = 'N';
            dataPtr[5]  = 'N';
            dataPtr[6]  = 'E';
            dataPtr[7]  = 'C';
            dataPtr[8]  = 'T';
            dataPtr[9]  = atData->paramSettings.s3Value;
            dataPtr[10] = atData->paramSettings.s4Value;

        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
    atData->responseFlag = NOWAITINGFORRESPONSE;
    clearExtendedCmdTable(atData);
    atData->state         = atOnlineData_s;
}


/*************************************************************************************
atResultCodeConnectTextHandler
************************************************************************************/
void atResultCodeConnectTextHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;
    CsrBtAtDgAtcConnectTextRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcConnectTextRes *) atData->atMsgP;

    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen = 2;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = atPrim->atResult;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            int i;
            valueLen = (CsrUint16)(atPrim->payloadLength + 12);
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = atData->paramSettings.s3Value;
            dataPtr[1] = atData->paramSettings.s4Value;
            dataPtr[2] = 'C';
            dataPtr[3] = 'O';
            dataPtr[4] = 'N';
            dataPtr[5] = 'N';
            dataPtr[6] = 'E';
            dataPtr[7] = 'C';
            dataPtr[8] = 'T';
            dataPtr[9] = ' ';
            for(i = 0; i < atPrim->payloadLength; i++)
            {
                dataPtr[i + 10] = atPrim->payload[i];
            }
            dataPtr[atPrim->payloadLength + 10] = atData->paramSettings.s3Value;
            dataPtr[atPrim->payloadLength + 11] = atData->paramSettings.s4Value;
        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
    atData->responseFlag = NOWAITINGFORRESPONSE;
    atData->state         = atOnlineData_s;
    clearExtendedCmdTable(atData);
    CsrPmemFree(atPrim->payload);
}



/*************************************************************************************
atResultCodeRingHandler
************************************************************************************/
void atResultCodeRingHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;

    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen = 2;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = 2;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            valueLen = 8;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = atData->paramSettings.s3Value;
            dataPtr[1] = atData->paramSettings.s4Value;
            dataPtr[2] = 'R';
            dataPtr[3] = 'I';
            dataPtr[4] = 'N';
            dataPtr[5] = 'G';
            dataPtr[6] = atData->paramSettings.s3Value;
            dataPtr[7] = atData->paramSettings.s4Value;

        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
}


/*************************************************************************************
atResultCodeNoCarrierHandler
************************************************************************************/
void atResultCodeNoCarrierHandler(atInstanceData_t *atData)
{
    if(atData->state == atOnlineData_s)
    {
        atData->state = atCmd_s;
        atData->numberOfRespon = 1;;
    }
    else if(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == D_TOKEN  ||
            atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == A_TOKEN    )
    {
        atData->state = atCmd_s;
    }

    atData->numberOfRespon--;

    if(atData->numberOfRespon <= 0)
    {
        if(atData->paramSettings.qValue == 0)
        {
            CsrUint8 * dataPtr;
            CsrUint16 valueLen;
            if(atData->paramSettings.vValue == 0)
            {
                valueLen = 2;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0] = 3;
                dataPtr[1] = atData->paramSettings.s3Value;
            }
            else
            {
                valueLen = 14;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0]  = atData->paramSettings.s3Value;
                dataPtr[1]  = atData->paramSettings.s4Value;
                dataPtr[2]  = 'N';
                dataPtr[3]  = 'O';
                dataPtr[4]  = ' ';
                dataPtr[5]  = 'C';
                dataPtr[6]  = 'A';
                dataPtr[7]  = 'R';
                dataPtr[8]  = 'R';
                dataPtr[9]  = 'I';
                dataPtr[10] = 'E';
                dataPtr[11] = 'R';
                dataPtr[12] = atData->paramSettings.s3Value;
                dataPtr[13] = atData->paramSettings.s4Value;
            }
            atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
        }
        atData->responseFlag = NOWAITINGFORRESPONSE;
        clearExtendedCmdTable(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}

/*************************************************************************************
atResultCodeErrorHandler
************************************************************************************/
void atResultCodeErrorHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;

    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen = 2;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = 4;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            valueLen = 9;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = atData->paramSettings.s3Value;
            dataPtr[1] = atData->paramSettings.s4Value;
            dataPtr[2] = 'E';
            dataPtr[3] = 'R';
            dataPtr[4] = 'R';
            dataPtr[5] = 'O';
            dataPtr[6] = 'R';
            dataPtr[7] = atData->paramSettings.s3Value;
            dataPtr[8] = atData->paramSettings.s4Value;

        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
    clearExtendedCmdTable(atData);
    atData->responseFlag        = NOWAITINGFORRESPONSE;
}

/*************************************************************************************
atResultCodeNoDialToneHandler
************************************************************************************/
void atResultCodeNoDialToneHandler(atInstanceData_t *atData)
{
    atData->numberOfRespon--;
    if(atData->numberOfRespon <= 0)
    {
        if(atData->paramSettings.qValue == 0)
        {
            CsrUint8 * dataPtr;
            CsrUint16 valueLen;
            if(atData->paramSettings.vValue == 0)
            {
                valueLen = 2;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0] = 6;
                dataPtr[1] = atData->paramSettings.s3Value;
            }
            else
            {
                valueLen = 16;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0]  = atData->paramSettings.s3Value;
                dataPtr[1]  = atData->paramSettings.s4Value;
                dataPtr[2]  = 'N';
                dataPtr[3]  = 'O';
                dataPtr[4]  = ' ';
                dataPtr[5]  = 'D';
                dataPtr[6]  = 'I';
                dataPtr[7]  = 'A';
                dataPtr[8]  = 'L';
                dataPtr[9]  = ' ';
                dataPtr[10] = 'T';
                dataPtr[11] = 'O';
                dataPtr[12] = 'N';
                dataPtr[13] = 'E';
                dataPtr[14] = atData->paramSettings.s3Value;
                dataPtr[15] = atData->paramSettings.s4Value;
            }
            atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
        }
        atData->responseFlag = NOWAITINGFORRESPONSE;
        clearExtendedCmdTable(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}

/*************************************************************************************
atResultCodeBusyHandler
************************************************************************************/
void atResultCodeBusyHandler(atInstanceData_t *atData)
{
    atData->numberOfRespon--;
    if(atData->numberOfRespon <= 0)
    {
        if(atData->paramSettings.qValue == 0)
        {
            CsrUint8 * dataPtr;
            CsrUint16 valueLen;
            if(atData->paramSettings.vValue == 0)
            {
                valueLen = 2;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0] = 7;
                dataPtr[1] = atData->paramSettings.s3Value;
            }
            else
            {
                valueLen = 8;
                dataPtr = CsrPmemAlloc(valueLen);
                dataPtr[0]  = atData->paramSettings.s3Value;
                dataPtr[1]  = atData->paramSettings.s4Value;
                dataPtr[2]  = 'B';
                dataPtr[3]  = 'U';
                dataPtr[4]  = 'S';
                dataPtr[5]  = 'Y';
                dataPtr[6] = atData->paramSettings.s3Value;
                dataPtr[7] = atData->paramSettings.s4Value;
            }
            atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
        }
        atData->responseFlag = NOWAITINGFORRESPONSE;
        clearExtendedCmdTable(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}

/*************************************************************************************
atResultCodeNoAnswerHandler
************************************************************************************/
void atResultCodeNoAnswerHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;
    int i;

    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen = 2;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = 8;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            valueLen = 13;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0]  = atData->paramSettings.s3Value;
            dataPtr[1]  = atData->paramSettings.s4Value;
            dataPtr[2]  = 'N';
            dataPtr[3]  = 'O';
            dataPtr[4]  = ' ';
            dataPtr[5]  = 'A';
            dataPtr[6]  = 'N';
            dataPtr[7]  = 'S';
            dataPtr[8]  = 'W';
            dataPtr[9]  = 'E';
            dataPtr[10] = 'R';
            dataPtr[11] = atData->paramSettings.s3Value;
            dataPtr[12] = atData->paramSettings.s4Value;

        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
    atData->responseFlag = NOWAITINGFORRESPONSE;
    atData->reachPoint            = CONTINUATION_AT_INTERPRET_FUNC;
    atData->terminatorFlag        = TERMINATORFOUND;
    atData->counter                = EMPTY;
    atData->tempValue            = EMPTY;
    atData->numberOfTokens        = EMPTY;
    atData->numberOfRespon        = EMPTY;
    atData->dialStringCounter    = EMPTY;
    atData->changeSettings        = EMPTY;
    atData->state                = atCmd_s;
    for( i = 0; i < CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND; i++)
    {
        atData->tokenFlipArray[i].receiveToken = NO_TOKEN;
        atData->tokenFlipArray[i].receiveValue = EMPTY;
    }
    clearExtendedCmdTable(atData);
}


/*************************************************************************************
atGmmResHandler
************************************************************************************/
void atGmmResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcGmmRes    *atPrim;

    atPrim    = (CsrBtAtDgAtcGmmRes *) atData->atMsgP;

    if(atPrim->payloadLength > 2044)
    {
        atPrim->payloadLength = 2044;
        CsrPmemFree(atPrim->payload);
    }
    else
    {
        atData->numberOfRespon--;

        atGxxHelpResHandler(atData, atPrim->payloadLength, atPrim->payload);
        CsrPmemFree(atPrim->payload);

        if(atData->numberOfRespon <= 0)
        {
            atOKResultResHandler(atData);
        }
        else
        {
            atTokenHandler(atData);
        }
    }

}

/*************************************************************************************
atGmiResHandler
************************************************************************************/
void atGmiResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcGmiRes    *atPrim;

    atPrim    = (CsrBtAtDgAtcGmiRes *) atData->atMsgP;

    if(atPrim->payloadLength > 2044)
    {
        atPrim->payloadLength = 2044;
        CsrPmemFree(atPrim->payload);

    }
    else
    {
        atData->numberOfRespon--;

        atGxxHelpResHandler(atData, atPrim->payloadLength, atPrim->payload);
        CsrPmemFree(atPrim->payload);

        if(atData->numberOfRespon <= 0)
        {
            atOKResultResHandler(atData);
        }
        else
        {
            atTokenHandler(atData);
        }
    }
}

/*************************************************************************************
atGmrResHandler
************************************************************************************/
void atGmrResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcGmrRes    *atPrim;

    atPrim    = (CsrBtAtDgAtcGmrRes *) atData->atMsgP;

    if(atPrim->payloadLength > 2044)
    {
        atPrim->payloadLength = 2044;
        CsrPmemFree(atPrim->payload);

    }
    else
    {
        atData->numberOfRespon--;

        atGxxHelpResHandler(atData, atPrim->payloadLength, atPrim->payload);
        CsrPmemFree(atPrim->payload);

        if(atData->numberOfRespon <= 0)
        {
            atOKResultResHandler(atData);
        }
        else
        {
            atTokenHandler(atData);
        }
    }
}



/*************************************************************************************
atGcapResHandler
************************************************************************************/
void atGcapResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcGcapRes    *atPrim;

    atPrim    = (CsrBtAtDgAtcGcapRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atGxxHelpResHandler(atData, atPrim->payloadLength, atPrim->payload);
    CsrPmemFree(atPrim->payload);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}



/*************************************************************************************
atGxxHelpResHandler
************************************************************************************/
void atGxxHelpResHandler(atInstanceData_t *atData, CsrUint16 payloadLength, CsrUint8 *payload)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;

    if(atData->paramSettings.vValue == 0)
    {
        int i;
        valueLen = (CsrUint16)(payloadLength + 2);
        dataPtr = CsrPmemAlloc(valueLen);
        for(i = 0; i < payloadLength; i++)
        {
            dataPtr[i] = payload[i];
        }
        dataPtr[payloadLength]     = atData->paramSettings.s3Value;
        dataPtr[payloadLength + 1] = atData->paramSettings.s4Value;
    }
    else
    {
        int i;
        valueLen = (CsrUint16)(payloadLength + 4);
        dataPtr = CsrPmemAlloc(valueLen);
        dataPtr[0] = atData->paramSettings.s3Value;
        dataPtr[1] = atData->paramSettings.s4Value;
        for(i = 0; i < payloadLength; i++)
        {
            dataPtr[i + 2] = payload[i];
        }
        dataPtr[payloadLength + 2] = atData->paramSettings.s3Value;
        dataPtr[payloadLength + 3] = atData->paramSettings.s4Value;
    }
    atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
}




/*************************************************************************************
atDefaultSettingResHandler
************************************************************************************/
void atDefaultSettingResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcDefaultSettingRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcDefaultSettingRes *) atData->atMsgP;

    atData->paramSettings.andDValue     =    atPrim->parameterSetting.andDValue;
    atData->paramSettings.eValue     =    atPrim->parameterSetting.eValue;
    atData->paramSettings.qValue     =    atPrim->parameterSetting.qValue;
    atData->paramSettings.s3Value     =    atPrim->parameterSetting.s3Value;
    atData->paramSettings.s4Value     =    atPrim->parameterSetting.s4Value;
    atData->paramSettings.s5Value     =    atPrim->parameterSetting.s5Value;
    atData->paramSettings.vValue     =    atPrim->parameterSetting.vValue;
#ifndef CSR_BT_AT_SEND_S0_TOKEN_IND
    atData->paramSettings.s0Value    = CSR_BT_AT_S0_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S6_TOKEN_IND
    atData->paramSettings.s6Value    = CSR_BT_AT_S6_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S7_TOKEN_IND
    atData->paramSettings.s7Value    = CSR_BT_AT_S7_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S8_TOKEN_IND
    atData->paramSettings.s8Value    = CSR_BT_AT_S8_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S10_TOKEN_IND
    atData->paramSettings.s10Value    = CSR_BT_AT_S10_DEFAULT_VALUE;
#endif

    if(atData->state == atInitialising_s)
    {
        atData->state                = atCmd_s;
    }
    else
    {
        atData->state = atCmd_s;
        atData->numberOfRespon--;
        if(atData->numberOfRespon <= 0)
        {
            atOKResultResHandler(atData);
        }
        else
        {
            atTokenHandler(atData);
        }
    }
}


/*************************************************************************************
atFactorySettingResHandler
************************************************************************************/
void atFactorySettingResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcFactorySettingRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcFactorySettingRes *) atData->atMsgP;

    atData->paramSettings.andDValue     =    atPrim->parameterSetting.andDValue;
    atData->paramSettings.eValue     =    atPrim->parameterSetting.eValue;
    atData->paramSettings.qValue     =    atPrim->parameterSetting.qValue;
    atData->paramSettings.s3Value     =    atPrim->parameterSetting.s3Value;
    atData->paramSettings.s4Value     =    atPrim->parameterSetting.s4Value;
    atData->paramSettings.s5Value     =    atPrim->parameterSetting.s5Value;
    atData->paramSettings.vValue     =    atPrim->parameterSetting.vValue;
#ifndef CSR_BT_AT_SEND_S0_TOKEN_IND
    atData->paramSettings.s0Value    = CSR_BT_AT_S0_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S6_TOKEN_IND
    atData->paramSettings.s6Value    = CSR_BT_AT_S6_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S7_TOKEN_IND
    atData->paramSettings.s7Value    = CSR_BT_AT_S7_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S8_TOKEN_IND
    atData->paramSettings.s8Value    = CSR_BT_AT_S8_DEFAULT_VALUE;
#endif
#ifndef CSR_BT_AT_SEND_S10_TOKEN_IND
    atData->paramSettings.s10Value    = CSR_BT_AT_S10_DEFAULT_VALUE;
#endif
    atData->numberOfRespon--;

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}


/*************************************************************************************
atUnknownExtendedCmdHelpResHandler
************************************************************************************/
void atUnknownExtendedCmdHelpResHandler(atInstanceData_t *atData, CsrUint16 payloadLength, CsrUint8 *payload)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;

    if(atData->paramSettings.vValue == 0)
    {
        int i;
        valueLen = (CsrUint16)(payloadLength + 2);
        dataPtr = CsrPmemAlloc(valueLen);
        for(i = 0; i < payloadLength; i++)
        {
            dataPtr[i] = payload[i];
        }
        dataPtr[payloadLength]     = atData->paramSettings.s3Value;
        dataPtr[payloadLength + 1] = atData->paramSettings.s4Value;
    }
    else
    {
        int i;
        valueLen = (CsrUint16)(payloadLength + 4);
        dataPtr = CsrPmemAlloc(valueLen);
        dataPtr[0] = atData->paramSettings.s3Value;
        dataPtr[1] = atData->paramSettings.s4Value;
        for(i = 0; i < payloadLength; i++)
        {
            dataPtr[i + 2] = payload[i];
        }
        dataPtr[payloadLength + 2] = atData->paramSettings.s3Value;
        dataPtr[payloadLength + 3] = atData->paramSettings.s4Value;
    }
    atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
}





/*************************************************************************************
atUnknownExtendedCmdResHandler
************************************************************************************/
void atUnknownExtendedCmdResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcUnknownExtendedCmdRes    *atPrim;

    atPrim    = (CsrBtAtDgAtcUnknownExtendedCmdRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atUnknownExtendedCmdHelpResHandler(atData, atPrim->payloadLength, atPrim->payload);
    CsrPmemFree(atPrim->payload);

    if(atData->numberOfRespon <= 0)
    {
        atOKResultResHandler(atData);
    }
    else
    {
        atTokenHandler(atData);
    }
}

#endif

