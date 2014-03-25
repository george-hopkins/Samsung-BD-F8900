/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
 sCommandType : Determine the S commandtype
************************************************************************************/
void sCommandType(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(atData->counter < atData->packetLength)
    {
        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            continuationAtInterpret(commandLine, atData);
        }
        else
        {
            atData->reachPoint = S_COMMAND_TYPE_FUNC;
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
                switch((**commandLine))
                {
                    case '0':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S0_TOKEN;
                            sReadOrSetCommandType(S0_TOKEN, commandLine, atData);
                            break;
                        }
                    case '3':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S3_TOKEN;
                            sReadOrSetCommandType(S3_TOKEN, commandLine, atData);
                            break;
                        }
                    case '4':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S4_TOKEN;
                            sReadOrSetCommandType(S4_TOKEN, commandLine, atData);
                            break;
                        }
                    case '5':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S5_TOKEN;
                            sReadOrSetCommandType(S5_TOKEN, commandLine, atData);
                            break;
                        }
                    case '6':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S6_TOKEN;
                            sReadOrSetCommandType(S6_TOKEN, commandLine, atData);
                            break;
                        }
                    case '7':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S7_TOKEN;
                            sReadOrSetCommandType(S7_TOKEN, commandLine, atData);
                            break;
                        }
                    case '8':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S8_TOKEN;
                            sReadOrSetCommandType(S8_TOKEN, commandLine, atData);
                            break;
                        }
                    case '1':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S10_COMMAND_TYPE_FUNC;
                            s10CommandType(commandLine, atData);
                            break;
                        }
                }
            }
        }
    }
}




/*************************************************************************************
 s10CommandType : Determine the S10 commandtype
************************************************************************************/
void s10CommandType(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(atData->counter < atData->packetLength)
    {
        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            continuationAtInterpret(commandLine, atData);
        }
        else
        {
            atData->reachPoint = S10_COMMAND_TYPE_FUNC;
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
                switch((**commandLine))
                {
                    case '0':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
                            atData->temptoken = S10_TOKEN;
                            sReadOrSetCommandType(S10_TOKEN, commandLine, atData);
                            break;
                        }
                }
            }
        }
    }
}



/*************************************************************************************
 sReadOrSetCommandType : Determine if it is a Read or Set S commandtype
************************************************************************************/
void sReadOrSetCommandType(CsrUint8 token, CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(atData->counter < atData->packetLength)
    {
        if(checkForS3(commandLine, atData) == TRUE)
        {
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            continuationAtInterpret(commandLine, atData);
        }
        else
        {
            atData->reachPoint = S_READ_OR_SET_COMMAND_TYPE_FUNC;
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
                switch((**commandLine))
                {
                    case '?':
                        {
                            switch(token)
                            {
                                case S0_TOKEN:
                                    {
                                        insertTokenInArray(S0_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S3_TOKEN:
                                    {
                                        insertTokenInArray(S3_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S4_TOKEN:
                                    {
                                        insertTokenInArray(S4_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S5_TOKEN:
                                    {
                                        insertTokenInArray(S5_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S6_TOKEN:
                                    {
                                        insertTokenInArray(S6_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S7_TOKEN:
                                    {
                                        insertTokenInArray(S7_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S8_TOKEN:
                                    {
                                        insertTokenInArray(S8_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                                case S10_TOKEN:
                                    {
                                        insertTokenInArray(S10_READ_TOKEN, EMPTY, atData);
                                        break;
                                    }
                            }
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = COMMAND_TYPE_FUNC;
                            commandType(commandLine, atData);
                            break;
                        }
                    case '=':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = '=';
                            if(atData->counter >= atData->packetLength)
                            {
                                atData->reachPoint = RETURN_VALUE_FUNC;
                                atData->counter++;
                            }
                            else
                            {
                                returnValue(commandLine, atData);
                                switch(token)
                                {
                                    case S3_TOKEN:
                                    case S4_TOKEN:
                                    case S5_TOKEN:
                                        {
                                            if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                            {
                                                ;
                                            }
                                            else if(atData->tempValue <= 127)
                                            {
                                                insertTokenInArray(token, atData->tempValue, atData);
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
                                            if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                            {
                                                ;
                                            }
                                            else if(atData->tempValue <= 255)
                                            {
                                                insertTokenInArray(token, atData->tempValue, atData);
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
                                            if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                            {
                                                ;
                                            }
                                            else if((atData->tempValue >= 2) && (atData->tempValue <= 10))
                                            {
                                                insertTokenInArray(token, atData->tempValue, atData);
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
                                            if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                            {
                                                ;
                                            }
                                            else if(atData->tempValue >= 1)
                                            {
                                                insertTokenInArray(token, atData->tempValue, atData);
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
                                            if(atData->reachPoint >= RETURN_VALUE_FUNC)
                                            {
                                                ;
                                            }
                                            else if((atData->tempValue >= 1) && (atData->tempValue <= 254))
                                            {
                                                insertTokenInArray(token, atData->tempValue, atData);
                                                commandType(commandLine, atData);
                                            }
                                            else
                                            {
                                                atData->reachPoint = INTERPRET_ERROR;
                                            }
                                            break;
                                        }
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

