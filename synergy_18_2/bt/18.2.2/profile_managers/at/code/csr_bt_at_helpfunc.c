/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

void CsrBtAtMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_AT_PRIM, msg);
}


/*************************************************************************************
 InitExtendedCmdTable :
************************************************************************************/
void initExtendedCmdTable(atInstanceData_t *atData)
{
    CsrUint8    i;
    for(i = 0; i < CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND; i++)
    {
        atData->atUnknownExtendedCmd[i] = NULL;
        atData->unknownExtendedCmdLength[i] = 0;
    }
}


/*************************************************************************************
 clearExtendedCmdTable :
************************************************************************************/
void clearExtendedCmdTable(atInstanceData_t *atData)
{
    CsrUint8    i;
    for(i = 0; i < CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND; i++)
    {
        if(atData->atUnknownExtendedCmd[i] != NULL)
        {
            CsrPmemFree(atData->atUnknownExtendedCmd[i]);
            atData->atUnknownExtendedCmd[i] = NULL;
        }
        atData->unknownExtendedCmdLength[i] = 0;
    }
    atData->numberOfUnknownCmd    = 0;
    CsrPmemFree(atData->cmd);
    atData->cmd = NULL;
    atData->packetLength = 0;
}



/*************************************************************************************
 removeSpace : Remove a space from the receive message
************************************************************************************/
CsrBool removeSpace(CsrUint8 **commandLine)
{

    if(**commandLine == ' ')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*************************************************************************************
 checkForS3 : Return TRUE if the terminator S3 is found
************************************************************************************/
CsrBool checkForS3(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(**commandLine == atData->paramSettings.s3Value)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*************************************************************************************
 removeS5 :
************************************************************************************/
void removeS5(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    int i;

    for(i = 0; i < atData->packetLength; i++)
    {
        if(**commandLine == atData->paramSettings.s3Value)
        {
            break;
        }
        if(**commandLine == atData->paramSettings.s5Value)
        {

            (**commandLine)        = ' ';
            (*commandLine)--;
            (**commandLine)        = ' ';
            (*commandLine)++;
        }
        (*commandLine)++;
    }
    (*commandLine) = (*commandLine) - i;
}



/*************************************************************************************
 copyDialString :
************************************************************************************/
CsrBool copyDialString(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(**commandLine == atData->paramSettings.s3Value)
    {
        return TRUE;
    }
    else if(**commandLine == ';')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*************************************************************************************
 insertTokenInArray : Insert the interpret token in a array and its value
************************************************************************************/
void insertTokenInArray(CsrUint8 token, CsrUint16 value, atInstanceData_t *atData)
{
    if(atData->numberOfTokens < CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND)
    {
        atData->tokenFlipArray[atData->numberOfTokens].receiveToken = token;
        atData->tokenFlipArray[atData->numberOfTokens].receiveValue    = (CsrUint8)value;
        atData->numberOfTokens++;
        atData->numberOfRespon++;
    }
    else
    {
        atData->numberOfTokens++;
    }
}



/*************************************************************************************
 returnValue : Return the Value. If no value is found 0 is return.
                                 If value is more than 3 digits 0xffff is return
************************************************************************************/
void returnValue(CsrUint8 **commandLine, atInstanceData_t *atData)
{

    atData->tempValue = 0;

    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
    }
    else
    {
        while (removeSpace(commandLine))
        {
            (*commandLine)++;
            atData->counter++;
        }

        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        }
        else if(atData->counter >= atData->packetLength)
        {
            atData->reachPoint = RETURN_VALUE_FUNC;
            atData->counter++;
        }
        else
        {

            if ((IS_DIGIT_CHAR(**commandLine)))
            {
                atData->tempValue = (CsrUint16)(**commandLine - '0');
                (*commandLine)++;
                atData->counter++;
                if(atData->counter >= atData->packetLength)
                {
                    atData->reachPoint = RETURN_VALUE2_FUNC;
                    atData->counter++;
                }
                else
                {
                    if(returnValue2(commandLine, atData) == TRUE)
                    {
                        (*commandLine)++;
                        atData->counter++;
                        if(atData->counter >= atData->packetLength)
                        {
                            atData->counter++;
                            atData->reachPoint = RETURN_VALUE3_FUNC;
                        }
                        else
                        {
                            if(returnValue3(commandLine, atData) == TRUE)
                            {
                                (*commandLine)++;
                                atData->counter++;
                                if(atData->counter >= atData->packetLength)
                                {
                                    atData->reachPoint = RETURN_VALUE4_FUNC;
                                    atData->counter++;
                                }
                                else
                                {
                                    if(returnValue4(commandLine, atData) == FALSE)
                                    {
                                        returnValue4(commandLine, atData);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}



/*************************************************************************************
 returnValue2 : Return the Value. If no value is found 0 is return.
                                 If value is more than 3 digits 0xffff is return
************************************************************************************/
CsrBool returnValue2(CsrUint8 **commandLine, atInstanceData_t *atData)
{

    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        return FALSE;
    }

    while (removeSpace(commandLine))
    {
        (*commandLine)++;
        atData->counter++;
    }

    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        return FALSE;
    }
    if(atData->counter >= atData->packetLength)
    {
        atData->reachPoint = RETURN_VALUE2_FUNC;
        return FALSE;
    }

    if ((IS_DIGIT_CHAR(**commandLine)))
    {
        atData->tempValue = (CsrUint16)((atData->tempValue * 10) + **commandLine - '0');
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*************************************************************************************
 returnValue3 : Return the Value. If no value is found 0 is return.
                                 If value is more than 3 digits 0xffff is return
************************************************************************************/
CsrBool returnValue3(CsrUint8 **commandLine, atInstanceData_t *atData)
{

    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        return FALSE;
    }

    while (removeSpace(commandLine))
    {
        (*commandLine)++;
        atData->counter++;
    }


    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        return FALSE;
    }
    if(atData->counter >= atData->packetLength)
    {
        atData->reachPoint = RETURN_VALUE3_FUNC;
        return FALSE;
    }

    if ((IS_DIGIT_CHAR(**commandLine)))
    {
        atData->tempValue = (CsrUint16)((atData->tempValue * 10) + **commandLine - '0');
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}





/*************************************************************************************
 returnValue : Return the Value. If no value is found 0 is return.
                                 If value is more than 3 digits 0xffff is return
************************************************************************************/
CsrBool returnValue4(CsrUint8 **commandLine, atInstanceData_t *atData)
{

    if(checkForS3(commandLine, atData) == TRUE)
    {
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        return FALSE;
    }
    else
    {
        while (removeSpace(commandLine))
        {
            (*commandLine)++;
            atData->counter++;
        }

        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            return FALSE;
        }
        if(atData->counter >= atData->packetLength)
        {
            atData->reachPoint = RETURN_VALUE4_FUNC;
            return FALSE;
        }

        if ((IS_DIGIT_CHAR(**commandLine)))
        {
            atData->tempValue = 0xffff;
        }
    }
    return TRUE;
}



/*************************************************************************************
 abortHandler :
************************************************************************************/
void abortHandler(atInstanceData_t *atData)
{
    if(atData->abortFlag == ABORT_ON)
    {
        if(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == A_TOKEN ||
           atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken == D_TOKEN   )
        {
            CsrBtAtDgAtcAbortInd *atPrim;
            atPrim    = (CsrBtAtDgAtcAbortInd *) CsrPmemAlloc(sizeof(CsrBtAtDgAtcAbortInd));
            atPrim->type    = CSR_BT_AT_DG_ATC_ABORT_IND;
            CsrBtAtMessagePut(atData->appHandle, atPrim);
        }
    }
}


/*************************************************************************************
 paramChangeHandler :
************************************************************************************/
void paramChangeHandler(atInstanceData_t *atData)
{
    switch(atData->tokenFlipArray[(atData->numberOfTokens - atData->numberOfRespon)].receiveToken)
    {
        case AND_D_TOKEN :
            {
                atData->paramSettings.andDValue = atData->tempparamSettings.andDValue;
                break;
            }
        case E_TOKEN :
            {
                atData->paramSettings.eValue = atData->tempparamSettings.eValue;
                break;
            }
        case V_TOKEN :
            {
                atData->paramSettings.vValue = atData->tempparamSettings.vValue;
                break;
            }
        case Q_TOKEN :
            {
                atData->paramSettings.qValue = atData->tempparamSettings.qValue;
                break;
            }
        case S3_TOKEN :
            {
                atData->paramSettings.s3Value = atData->tempparamSettings.s3Value;
                break;
            }
        case S4_TOKEN :
            {
                atData->paramSettings.s4Value = atData->tempparamSettings.s4Value;
                break;
            }
        case S5_TOKEN :
            {
                atData->paramSettings.s5Value = atData->tempparamSettings.s5Value;
                break;
            }
    }
}

/*************************************************************************************
 atDgDisconnectCleanUp :
************************************************************************************/
void atDgDisconnectCleanUp(atInstanceData_t *atData)
{
    CsrUint16                eventClass;
    CsrBtDgDataReq *            msg;
    void *popMsg;
    int i;

    atData->reachPoint                = CONTINUATION_AT_INTERPRET_FUNC;
    atData->terminatorFlag            = TERMINATORFOUND;
    atData->responseFlag              = NOWAITINGFORRESPONSE;
    atData->counter                   = EMPTY;
    atData->tempValue                 = EMPTY;
    atData->numberOfTokens            = EMPTY;
    atData->numberOfRespon            = EMPTY;
    atData->dialStringCounter         = EMPTY;
    atData->changeSettings            = EMPTY;
    atData->atCfmFlag                 = CFM_RECEIVE;
    atData->appResFlag                = APP_RES;
    atData->cmDataReqQueue            = NULL;
    CsrPmemFree(atData->cmd);
    atData->cmd                       = NULL;
    atData->packetLength              = 0;

    for( i = 0; i < CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND; i++)
    {
        atData->tokenFlipArray[i].receiveToken = NO_TOKEN;
        atData->tokenFlipArray[i].receiveValue = EMPTY;
    }
    atData->state = atCmd_s;

    while((CsrMessageQueuePop(&atData->cmDataReqQueue, &eventClass, &popMsg)))
    {
        msg = (CsrBtDgDataReq *)popMsg;

        CsrPmemFree(msg->payload);
        CsrPmemFree(msg);
    }
    clearExtendedCmdTable(atData);
}

/*************************************************************************************
 atStateChangeHandler :
************************************************************************************/
void atStateChangeHandler(atInstanceData_t *atData, CsrUint8 modemstatus)
{
    if(atData->state == atOnlineData_s)
    {
        if(((modemstatus & CSR_BT_MODEM_DTR_MASK) == 0))
        {
            if(atData->appResFlag == NO_APP_RES)
            {
                atDataResHandler(atData);
            }

            switch(atData->paramSettings.andDValue)
            {
                case 1 :
                    {
                        atData->state = atOnlineCmd_s;
                        atOKResultResHandler(atData);
                        break;
                    }
                case 2 :
                    {
                        atData->state = atCmd_s;
                        break;
                    }
            }
        }
    }
}
#endif

