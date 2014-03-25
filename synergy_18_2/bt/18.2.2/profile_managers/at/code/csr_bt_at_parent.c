/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
 mainAtInterpret
************************************************************************************/
void mainAtInterpret(CsrUint8 *commandLine, atInstanceData_t *atData)
{
    removeS5(&commandLine, atData);
    if(atData->terminatorFlag == TERMINATORFOUND)
    {
        comparePrefix(commandLine, atData);
    }
    else
    {
        continuationAtInterpret(&commandLine, atData);
    }
}


/*************************************************************************************
continuationAtInterpret
************************************************************************************/
void continuationAtInterpret(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(atData->reachPoint == CONTINUATION_AT_INTERPRET_FUNC)
    {
        atData->terminatorFlag    = TERMINATORFOUND;
    }
    else
    {
        switch(atData->reachPoint)
        {
            case CONTINUATION_COPY_DIAL_STRING_FUNC :
                {
                    continuationCopyDialString(commandLine, atData);
                    break;
                }
            case COMPARE_PREFIX_FUNC:
                {
                    comparePrefix(*commandLine, atData);
                    break;
                }
            case COMMAND_TYPE_FUNC:
                {
                    commandType(commandLine, atData);
                    break;
                }
            case AND_COMMAND_TYPE_FUNC:
                {
                    andCommandType(commandLine, atData);
                    break;
                }
            case EXTENDED_COMMAND_TYPE_FUNC:
                {
                    extendedCommandType(commandLine, atData);
                    break;
                }
            case EXTENDED_M_OR_C_FUNC:
                {
                    extendedMOrC(commandLine, atData);
                    break;
                }
            case EXTENDED_I_OR_M_OR_R_FUNC:
                {
                    extendedIOrMOrR(commandLine, atData);
                    break;
                }
            case S_COMMAND_TYPE_FUNC:
                {
                    sCommandType(commandLine, atData);
                    break;
                }
            case S10_COMMAND_TYPE_FUNC:
                {
                    s10CommandType(commandLine, atData);
                    break;
                }
            case S_READ_OR_SET_COMMAND_TYPE_FUNC:
                {
                    sReadOrSetCommandType(atData->temptoken, commandLine, atData);
                    break;
                }
            case EXTENDED_A_FUNC:
                {
                    extendedA(commandLine, atData);
                    break;
                }
            case EXTENDED_GCAP_FUNC:
                {
                    extendedGCAP(commandLine, atData);
                    break;
                }
            case EXTENDED_FINAL_CHARACTER_FUNC :
                {
                    extendedFinalCharacter(commandLine, atData);
                    break;
                }
            case UNKNOWN_EXTENDED_COMMAND_FUNC :
                {
                    copyUnknownExtendedCommand(commandLine, atData, FALSE);
                    break;
                }
            case RETURN_VALUE_FUNC:
            case RETURN_VALUE2_FUNC:
            case RETURN_VALUE3_FUNC:
            case RETURN_VALUE4_FUNC:
                {
                    helpReturnValue(commandLine, atData);
                    break;
                }
        }
    }
}


/*************************************************************************************
 continuationCopyDialString
************************************************************************************/
void continuationCopyDialString(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    while(copyDialString(commandLine, atData) == FALSE)
    {
        if(atData->dialStringCounter >= CSR_BT_MAX_DIAL_STRING_LENGTH)
        {
            break;
        }
        else if(atData->counter >= atData->packetLength)
        {
            atData->reachPoint = CONTINUATION_COPY_DIAL_STRING_FUNC;
            atData->counter++;
            break;
        }
        else
        {
            atData->dialString[atData->dialStringCounter] = **commandLine;
            atData->dialStringCounter++;
            (*commandLine)++;
            atData->counter++;
        }
    }
    if(atData->dialStringCounter >= CSR_BT_MAX_DIAL_STRING_LENGTH)
    {
        atData->reachPoint = INTERPRET_ERROR;
    }
    else if(**commandLine == atData->paramSettings.s3Value)
    {
        insertTokenInArray(D_TOKEN, NO_SEMICOLON, atData);
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        continuationAtInterpret(commandLine, atData);
    }
    else if(**commandLine == ';')
    {
        insertTokenInArray(D_TOKEN, SEMICOLON, atData);
        (*commandLine)++;
        atData->counter++;
        commandType(commandLine, atData);
    }
}

/*************************************************************************************
 helpReturnValue
************************************************************************************/
void helpReturnValue(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    switch(atData->lastChar)
    {
        case 'C':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 1)
                {
                    insertTokenInArray(AND_C_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'D':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 2)
                {
                    insertTokenInArray(AND_D_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'F':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if((atData->tempValue == 0) ||  !( atData->tempValue > CSR_BT_SET_TO_FACTORY_DEFINED_CONFIGURATION))
                {
                    insertTokenInArray(AND_F_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'E':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 1)
                {
                    insertTokenInArray(E_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'H':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue == 0)
                {
                    insertTokenInArray(H_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'L':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 3)
                {
                    insertTokenInArray(L_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'M':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 2)
                {
                    insertTokenInArray(M_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'O':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(!(atData->tempValue > CSR_BT_RETURN_TO_ONLINE_DATA_STATE))
                {
                    insertTokenInArray(O_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'Q':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 1)
                {
                    insertTokenInArray(Q_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'V':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 1)
                {
                    insertTokenInArray(V_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'X':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue <= 4)
                {
                    insertTokenInArray(X_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case 'Z':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                if(atData->tempValue == CSR_BT_RESET_TO_DEFAULT_CONFIGURATION)
                {
                    insertTokenInArray(Z_TOKEN, atData->tempValue, atData);
                    commandType(commandLine, atData);
                }
                else
                {
                    atData->reachPoint = INTERPRET_ERROR;
                }
                break;
            }
        case '=':
            {
                switch(atData->reachPoint)
                {
                    case RETURN_VALUE_FUNC:
                        {
                            returnValue(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE2_FUNC:
                        {
                            helpReturnValue2(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE3_FUNC:
                        {
                            helpReturnValue3(commandLine, atData);
                            break;
                        }
                    case RETURN_VALUE4_FUNC:
                        {
                            helpReturnValue4(commandLine, atData);
                            break;
                        }
                }
                switch(atData->temptoken)
                {
                    case S3_TOKEN:
                    case S4_TOKEN:
                    case S5_TOKEN:
                        {
                            if(atData->tempValue <= 127)
                            {
                                insertTokenInArray(atData->temptoken, atData->tempValue, atData);
                                commandType(commandLine, atData);
                            }
                            else
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            break;
                        }
                    case S0_TOKEN:
                    case S8_TOKEN:
                        {
                            if(atData->tempValue <= 255)
                            {
                                insertTokenInArray(atData->temptoken, atData->tempValue, atData);
                                commandType(commandLine, atData);
                            }
                            else
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            break;
                        }
                    case S6_TOKEN:
                        {
                            if((atData->tempValue >= 2) && (atData->tempValue <= 10))
                            {
                                insertTokenInArray(atData->temptoken, atData->tempValue, atData);
                                commandType(commandLine, atData);
                            }
                            else
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            break;
                        }
                    case S7_TOKEN:
                        {
                            if(atData->tempValue >= 1)
                            {
                                insertTokenInArray(atData->temptoken, atData->tempValue, atData);
                                commandType(commandLine, atData);
                            }
                            else
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            break;
                        }
                    case S10_TOKEN:
                        {
                            if((atData->tempValue >= 1) && (atData->tempValue <= 254))
                            {
                                insertTokenInArray(atData->temptoken, atData->tempValue, atData);
                                commandType(commandLine, atData);
                            }
                            else
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            break;
                        }
                }
                break;
            }
    }
}

/*************************************************************************************
 helpReturnValue2
************************************************************************************/
void helpReturnValue2(CsrUint8 **commandLine, atInstanceData_t *atData)
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

/*************************************************************************************
 helpReturnValue3
************************************************************************************/
void helpReturnValue3(CsrUint8 **commandLine, atInstanceData_t *atData)
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


/*************************************************************************************
 helpReturnValue4
************************************************************************************/
void helpReturnValue4(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(returnValue4(commandLine, atData) == FALSE)
    {
        returnValue4(commandLine, atData);
    }
}

#endif

