/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
 andCommandType : Determine the &commandtype
************************************************************************************/
void andCommandType(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = AND_COMMAND_TYPE_FUNC;
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
                    case 'C':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'C';
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
                                    insertTokenInArray(AND_C_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'D':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'D';
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
                                    insertTokenInArray(AND_D_TOKEN, atData->tempValue, atData);
                                    commandType(commandLine, atData);
                                }
                                else
                                {
                                    atData->reachPoint = INTERPRET_ERROR;
                                }
                            }
                            break;
                        }
                    case 'F':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->lastChar = 'F';
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
                                else if((atData->tempValue == 0) ||  !( atData->tempValue > CSR_BT_SET_TO_FACTORY_DEFINED_CONFIGURATION))
                                {
                                    insertTokenInArray(AND_F_TOKEN, atData->tempValue, atData);
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

