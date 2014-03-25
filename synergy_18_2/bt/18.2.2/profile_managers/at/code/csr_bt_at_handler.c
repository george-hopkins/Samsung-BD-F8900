/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
 extendedCommandType : Determine the extended command type
************************************************************************************/
void extendedCommandType(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = EXTENDED_COMMAND_TYPE_FUNC;
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
                switch(TO_UPPER(**commandLine))
                {
                    case 'G':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_M_OR_C_FUNC;
                            extendedMOrC(commandLine, atData);
                            break;
                        }
                    default:
                        {
                            if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            else if(atData->counter >= atData->packetLength)
                            {

                            }
                            else
                            {
                                (*commandLine)--;
                                atData->counter--;
                                copyUnknownExtendedCommand(commandLine, atData, TRUE);
                            }
                            break;
                        }
                }
            }
        }
    }
}






/*************************************************************************************
 extendedMOrC :
************************************************************************************/
void extendedMOrC(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = EXTENDED_M_OR_C_FUNC;
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
                switch(TO_UPPER(**commandLine))
                {
                    case 'M':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_I_OR_M_OR_R_FUNC;
                            extendedIOrMOrR(commandLine, atData);
                            break;
                        }
                    case 'C':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_A_FUNC;
                            extendedA(commandLine, atData);
                            break;
                        }
                    default:
                        {
                            if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            else if(atData->counter >= atData->packetLength)
                            {

                            }
                            else
                            {
                                (*commandLine)    = (*commandLine)  - 2;
                                atData->counter = atData->counter - 2;
                                copyUnknownExtendedCommand(commandLine, atData, TRUE);
                            }
                            break;
                        }
                }
            }
        }
    }
}


/*************************************************************************************
 extendedIOrMOrR : Determine if it is a +GMI, +GMM or a +GMR command
************************************************************************************/
void extendedIOrMOrR(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = EXTENDED_I_OR_M_OR_R_FUNC;
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
                switch(TO_UPPER(**commandLine))
                {
                    case 'I':
                        {
                            atData->extendedTempToken    = GMI_TOKEN;
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_FINAL_CHARACTER_FUNC;
                            extendedFinalCharacter(commandLine, atData);
                            break;
                        }
                    case 'M':
                        {
                            atData->extendedTempToken    = GMM_TOKEN;
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_FINAL_CHARACTER_FUNC;
                            extendedFinalCharacter(commandLine, atData);
                            break;
                        }
                    case 'R':
                        {
                            atData->extendedTempToken    = GMR_TOKEN;
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_FINAL_CHARACTER_FUNC;
                            extendedFinalCharacter(commandLine, atData);
                            break;
                        }
                    default:
                        {
                            if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            else if(atData->counter >= atData->packetLength)
                            {

                            }
                            else
                            {
                                (*commandLine)    = (*commandLine)  - 3;
                                atData->counter = atData->counter - 3;
                                copyUnknownExtendedCommand(commandLine, atData, TRUE);
                            }
                            break;
                        }
                }
            }
        }
    }
}



/*************************************************************************************
 extendedA :
************************************************************************************/
void extendedA(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = EXTENDED_A_FUNC;
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
                switch(TO_UPPER(**commandLine))
                {
                    case 'A':
                        {
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_GCAP_FUNC;
                            extendedGCAP(commandLine, atData);
                            break;
                        }
                    default:
                        {
                            if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            else if(atData->counter >= atData->packetLength)
                            {

                            }
                            else
                            {
                                (*commandLine)    = (*commandLine)  - 3;
                                atData->counter = atData->counter - 3;
                                copyUnknownExtendedCommand(commandLine, atData, TRUE);
                            }
                            break;
                        }
                }
            }
        }
    }
}




/*************************************************************************************
 extendedGCAP : Determine if the command is +GCAP or unknown
************************************************************************************/
void extendedGCAP(CsrUint8 **commandLine, atInstanceData_t *atData)
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
            atData->reachPoint = EXTENDED_GCAP_FUNC;
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
                switch(TO_UPPER(**commandLine))
                {
                    case 'P':
                        {
                            atData->extendedTempToken    = GCAP_TOKEN;
                            (*commandLine)++;
                            atData->counter++;
                            atData->reachPoint = EXTENDED_FINAL_CHARACTER_FUNC;
                            extendedFinalCharacter(commandLine, atData);
                            break;
                        }
                    default:
                        {
                            if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                            {
                                atData->reachPoint = INTERPRET_ERROR;
                            }
                            else if(atData->counter >= atData->packetLength)
                            {

                            }
                            else
                            {
                                (*commandLine)    = (*commandLine)  - 4;
                                atData->counter = atData->counter - 4;
                                copyUnknownExtendedCommand(commandLine, atData, TRUE);
                            }
                            break;
                        }
                }
            }
        }
    }
}



/*************************************************************************************
 extendedFinalCharacter : Determine if the extende command is the last one,
                          follow by another or is unknown
************************************************************************************/
void extendedFinalCharacter(CsrUint8 **commandLine, atInstanceData_t *atData)
{
    if(atData->counter < atData->packetLength)
    {
        if(checkForS3(commandLine, atData) == TRUE)
        {
            insertTokenInArray(atData->extendedTempToken, EMPTY, atData);
            atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
            continuationAtInterpret(commandLine, atData);
        }
        else
        {
            atData->reachPoint = EXTENDED_FINAL_CHARACTER_FUNC;
            while (removeSpace(commandLine))
            {
                (*commandLine)++;
                atData->counter++;
            }
            if(checkForS3(commandLine, atData) == TRUE)
            {
                insertTokenInArray(atData->extendedTempToken, EMPTY, atData);
                atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
                continuationAtInterpret(commandLine, atData);
            }
            else
            {
                if(**commandLine == ';')
                {
                    insertTokenInArray(atData->extendedTempToken, EMPTY, atData);
                    (*commandLine)++;
                    atData->counter++;
                    atData->reachPoint = COMMAND_TYPE_FUNC;
                    commandType(commandLine, atData);
                }
                else
                {
                    if(atData->counter < atData->packetLength)
                    {
                        atData->reachPoint = UNKNOWN_EXTENDED_COMMAND_FUNC;
                        if(atData->numberOfUnknownCmd >= CSR_BT_MAX_NUMBER_OF_UNKNOWN_EXTENDED_COMMAND)
                        {
                            atData->reachPoint = INTERPRET_ERROR;
                        }
                        else if(atData->counter >= atData->packetLength)
                        {

                        }
                        else
                        {
                            switch(atData->extendedTempToken)
                            {
                                case GMI_TOKEN :
                                case GMM_TOKEN :
                                case GMR_TOKEN :
                                    {
                                        (*commandLine)    = (*commandLine)  - 4;
                                        atData->counter = atData->counter - 4;
                                        copyUnknownExtendedCommand(commandLine, atData, TRUE);
                                        break;
                                    }
                                case GCAP_TOKEN :
                                    {
                                        (*commandLine)    = (*commandLine)  - 5;
                                        atData->counter = atData->counter - 5;
                                        copyUnknownExtendedCommand(commandLine, atData, TRUE);
                                        break;
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
 copyUnknownExtendedCommand :
************************************************************************************/
void copyUnknownExtendedCommand(CsrUint8 **commandLine, atInstanceData_t *atData, CsrBool flag)
{
    CsrUint8        *unKnownPtr = NULL;

    if(flag)
    {
        unKnownPtr            = (CsrUint8 *) CsrPmemAlloc(CSR_BT_MAX_UNKNOWN_EXTENDED_COMMAND_LENGTH + 5);
    }
    else
    {
        unKnownPtr            = atData->atUnknownExtendedCmd[atData->numberOfUnknownCmd];
    }

    atData->reachPoint    = UNKNOWN_EXTENDED_COMMAND_FUNC;

    while(**commandLine != atData->paramSettings.s3Value && **commandLine != ';')
    {
        if(atData->unknownExtendedCmdLength[atData->numberOfUnknownCmd] >= CSR_BT_MAX_UNKNOWN_EXTENDED_COMMAND_LENGTH)
        {
            atData->reachPoint = INTERPRET_ERROR;
            break;
        }
        else if(atData->counter >= atData->packetLength)
        {
            break;
        }
        else
        {
            unKnownPtr[atData->unknownExtendedCmdLength[atData->numberOfUnknownCmd]] = **commandLine;
            atData->unknownExtendedCmdLength[atData->numberOfUnknownCmd]++;
            (*commandLine)++;
            atData->counter++;
        }

    }

    if(**commandLine == ';')
    {
        insertTokenInArray(UNKNOWN_EXTENDED_COMMAND_TOKEN, atData->numberOfUnknownCmd, atData);
        atData->atUnknownExtendedCmd[atData->numberOfUnknownCmd] = unKnownPtr;
        atData->numberOfUnknownCmd++;
        (*commandLine)++;
        atData->counter++;
        atData->reachPoint = COMMAND_TYPE_FUNC;
        commandType(commandLine, atData);
    }
    else if(**commandLine == atData->paramSettings.s3Value)
    {
        insertTokenInArray(UNKNOWN_EXTENDED_COMMAND_TOKEN, atData->numberOfUnknownCmd, atData);
        atData->reachPoint = CONTINUATION_AT_INTERPRET_FUNC;
        atData->atUnknownExtendedCmd[atData->numberOfUnknownCmd] = unKnownPtr;
        continuationAtInterpret(commandLine, atData);
    }
    else if(atData->reachPoint    == UNKNOWN_EXTENDED_COMMAND_FUNC)
    {
        atData->atUnknownExtendedCmd[atData->numberOfUnknownCmd] = unKnownPtr;
    }
    else if(flag)
    {/* Free the allocated pointer that is not used anyway */
        CsrPmemFree(unKnownPtr);
    }
}
#endif

