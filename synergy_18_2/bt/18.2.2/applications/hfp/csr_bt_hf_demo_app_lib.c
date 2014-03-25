/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

/* standard include files */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/* profile manager include files */
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_lib.h"
#include "bluetooth.h"
#include "csr_bt_hf_prim.h"
#include "csr_bt_hf_lib.h"
#include "csr_bt_hf.h"
#include "csr_bt_hf_demo_app.h"
#include "csr_bt_demoapp.h"
#include "csr_app_lib.h"
#include "csr_bt_ui_strings.h"

#define TO_UPPER(x)      ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x) ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)


/*************************************************************************************
whiteSpace : Remove whitespaces from the received message
************************************************************************************/
static CsrBool whiteSpace(CsrUint8 theChar)
{
    switch (theChar)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return TRUE;
    }
    return FALSE;
}


/*************************************************************************************
compareStrNotZero : Check received character(s) and parse them to upper case
************************************************************************************/
static CsrBool compareStrNotZero(CsrUint8 *theString, CsrUint8 ** line, CsrUint16 *charsConsumed, CsrUint16 size)
{
    CsrUint16 i;

    for (i=0; i < size; i++)
    {
        if ((*(*line+i) == '\0') || (TO_UPPER(theString[i]) != TO_UPPER(*(*line+i))))
        {
            return FALSE;
        }
    }
    (*line) += size;
    (*charsConsumed) +=size;
     return TRUE;
}


/*************************************************************************************
getAtToken : InterPret the received message and return define Token and value
************************************************************************************/
int HfGetAtToken(CsrUint8 *line, char **nextChar2Interpret, CsrUint16 *charsConsumed)
{
    int            returnToken;

    returnToken = CSR_BT_OTHER_TOKEN;
    *charsConsumed = 0;
    while (whiteSpace(*line))
    {
        line++;
        (*charsConsumed)++;
    }
    if (compareStrNotZero("ERROR", &line, charsConsumed,5))
    {
        returnToken = CSR_BT_ERROR_TOKEN;
    }
    else if (compareStrNotZero("OK", &line, charsConsumed,2))
    {
        returnToken = CSR_BT_OK_TOKEN;
    }

    *nextChar2Interpret = (char *)line;
    if (returnToken == CSR_BT_OTHER_TOKEN)
    {
        charsConsumed = 0;
    }
    return returnToken;
}


void sendAudioCodec(CsrBtHfConnectionId connId, CsrUint8 codec, CsrUint8 sampleRate)
{
    CsrBtHfSetC2CAudioCodecReqSend(connId,codec);
    CsrBtHfSetC2CSampleRateReqSend(connId,sampleRate);
    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "C2C AudioCodec (%d) returned\n", codec);
}

void sendBatteryLevel(CsrBtHfConnectionId connId, CsrUint8 batteryLevel)
{
    CsrBtHfC2CBattReqSend(connId,batteryLevel);
    CsrAppBacklogReqSend(TECH_BT, PROFILE_HF, FALSE, "C2C battery level (%d) sent\n", batteryLevel);
}

CsrBool findActiveConnectionType(DemoInstdata_t * instData,CsrBtHfConnectionType theConnectionType)
{
    CsrUint8 i;

    for (i=0;i<MAX_NUM_CONNECTION; i++)
    {
        if ((instData->conInstData[i].linkType == theConnectionType) &&
            (instData->conInstData[i].linkState == connected_s))
        {
            instData->currentConnection = i;
            return TRUE;
        }
    }
    /* No active connection found of the connection type given */
    return FALSE;
}

void findFirstActiveConnection(DemoInstdata_t * instData)
{
    CsrUint8 i = 0;

    instData->currentConnection = 0;
    for (i=0;i<MAX_NUM_CONNECTION; i++)
    {
        if (instData->conInstData[i].linkState == connected_s)
        {
            instData->currentConnection = i;
            return;
        }
    }
}
