/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
 comparePrefix : Determine the prefix
************************************************************************************/
void comparePrefix(CsrUint8 *commandLine, atInstanceData_t *atData)
{
    if('A' == TO_UPPER(*commandLine))
    {
        (commandLine)++;
        atData->counter++;
        atData->reachPoint    = COMPARE_PREFIX_FUNC;
        secondComparePrefix(commandLine, atData);
    }
    else
    {
        atData->reachPoint = INTERPRET_ERROR;
    }
}


/*************************************************************************************
 secondComparePrefix : Determine the second part of the prefix
************************************************************************************/
void secondComparePrefix(CsrUint8 *commandLine, atInstanceData_t *atData)
{
    if(atData->counter < atData->packetLength)
    {
        switch((TO_UPPER(*commandLine)))
        {
            case 'T':
                {
                    int i;
                    (commandLine)++;
                    atData->counter++;
                    atData->reachPoint    = COMMAND_TYPE_FUNC;
                    for( i = 0; i < CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND; i++)
                    {
                        atData->tokenFlipArray[i].receiveToken = NO_TOKEN;
                        atData->tokenFlipArray[i].receiveValue = EMPTY;;
                    }
                    atData->numberOfTokens    = EMPTY;
                    atData->numberOfRespon  = EMPTY;
                    atData->changeSettings    = EMPTY;
                    commandType(&commandLine, atData);
                    break;
                }
            case '/':
                {
                    atData->changeSettings    = EMPTY;
                    atData->numberOfRespon  = atData->numberOfTokens;
                    atData->reachPoint        = CONTINUATION_AT_INTERPRET_FUNC;
                    continuationAtInterpret(&commandLine, atData);
                    break;
                }
            default :
                {
                    atData->reachPoint = INTERPRET_ERROR;
                    break;
                }
        }
    }
}



/*************************************************************************************
 commandType : Determine the commandtype
************************************************************************************/
void commandType(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    atData->terminatorFlag = TERMINATORNOTFOUND;

    if(atData->counter < atData->packetLength)
    {
        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            continuationAtInterpret(commandLine, atData);
        }
        else if(atData->numberOfTokens >= CSR_BT_MAX_NUMBER_OF_TOKEN_IN_ONE_COMMAND)
        {
            atData->reachPoint = INTERPRET_ERROR;
        }
        else
        {
            atData->reachPoint = COMMAND_TYPE_FUNC;
            while (removeSpace(commandLine))
            {
                (*commandLine)++;
                atData->counter++;
            }
            if(checkForS3(commandLine, atData) == TRUE)
            {
                atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
                continuationAtInterpret(commandLine, atData);
            }
            else
            {
                switch((TO_UPPER(**commandLine)))
                {
                    case '&':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = AND_COMMAND_TYPE_FUNC;
                            andCommandType(commandLine, atData);
                            break;
                        }
                    case '+':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_COMMAND_TYPE_FUNC;
                            extendedCommandType(commandLine, atData);
                            break;
                        }
                    case 'S':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_COMMAND_TYPE_FUNC;
                            sCommandType(commandLine, atData);
                            break;
                        }
                    case 'A':
                        {
                            insertTokenInArray(A_TOKEN, EMPTY, atData);
                            (*commandLine)++;
                            atData->counter++;
                            commandType(commandLine, atData);
                            break;
                        }
                    case 'D':
                        {
                            atData->dialStringCounter    = EMPTY;
                            (*commandLine)++;
                            atData->counter++;
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
                            break;
                        }
                    case 'E':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'E';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 1)
                                {
                                    insertTokenInArray(E_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'H':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'H';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue == 0)
                                {
                                    insertTokenInArray(H_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'L':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'L';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 3)
                                {
                                    insertTokenInArray(L_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'M':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'M';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 2)
                                {
                                    insertTokenInArray(M_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'O':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'O';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(!(atData->tempValue > CSR_BT_RETURN_TO_ONLINE_DATA_STATE))
                                {
                                    insertTokenInArray(O_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'P':
                        {
                            insertTokenInArray(P_TOKEN, EMPTY, atData);
                            (*commandLine)++;
                            atData->counter++;
                            commandType(commandLine, atData);
                            break;
                        }
                    case 'Q':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'Q';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 1)
                                {
                                    insertTokenInArray(Q_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'T':
                        {
                            insertTokenInArray(T_TOKEN, EMPTY, atData);
                            (*commandLine)++;
                            atData->counter++;
                            commandType(commandLine, atData);
                            break;
                        }
                    case 'V':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'V';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 1)
                                {
                                    insertTokenInArray(V_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'X':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'X';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue <= 4)
                                {
                                    insertTokenInArray(X_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'Z':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'Z';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                {
                                    ;
                                }
                                else if(atData->tempValue == CSR_BT_RESET_TO_DEFAULT_CONFIGURATION)
                                {
                                    insertTokenInArray(Z_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                }
            }
        }
    }
}
#endif

