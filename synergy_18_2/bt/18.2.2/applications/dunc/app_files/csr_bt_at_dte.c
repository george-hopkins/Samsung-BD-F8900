/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>
#include <string.h>

#include "csr_bt_dunc_lib.h"
#include "csr_bt_at_dte.h"
#include "csr_bt_util.h"

CsrBool ATSetup(CsrUint8 procedureStep)
{
    char *command;
    CsrBool retVal = FALSE;

    switch (procedureStep)
    {
        /* send "AT" sequence */
        case 0:
        {
            command = (char *) CsrPmemAlloc(4);
            sprintf(command, "AT\r");   /* Attention */
            CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 3);
            retVal = FALSE;
            break;
        }
        case 1:
        {
            command = (char *) CsrPmemAlloc(6);
            sprintf(command, "AT&F\r"); /* factory settings */
            CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 5);
            retVal = FALSE;
            break;
        }
        case 2:
        {
            command = (char *) CsrPmemAlloc(18);
            sprintf(command, "ATV1E0S0=0&D2&C1\r"); /* long verbose, echo off, auto answer off */
            CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 17);
            retVal = FALSE;
            break;
        }
        case 3:
        {
            command = (char *) CsrPmemAlloc(15);
            sprintf(command, "ATS7=60L0M1X3\r"); /* wait CSRMAX. 60 sec. for modem detection */
            CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 14);
            retVal = TRUE;
            break;
        }
    }
    return retVal;
}

void ATDial(CsrInt8 *dialNumber)
{
    char * command;
    CsrUint16 len;
    /* alloc for dial number + AT specific stuff */
    len = (CsrUint16) (strlen((char *)dialNumber) + 5);
    command = (char *) CsrPmemAlloc(len);
    sprintf(command, "ATD%s\r", dialNumber);
    CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, (CsrUint16) (len-1));
}

void SendHookOn(CsrUint16 m ,void *data)
{
    char * command;

    command = (char *) CsrPmemAlloc(5);
    sprintf(command, "ATH\r");
    CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 4);
}

static void SendSinglePlus(CsrUint16 m ,void *data)
{
    char * command;

    command = (char *) CsrPmemAlloc(2);
    sprintf(command, "+");
    CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 1);
}

void ATHangup(void)
{
    CsrSchedTimerSet(100000, SendSinglePlus, 0, NULL);
    CsrSchedTimerSet(100000, SendSinglePlus, 0, NULL);
    CsrSchedTimerSet(100000, SendSinglePlus, 0, NULL);
    /* some modems need a small delay before sending ATH */
    CsrSchedTimerSet(500000, SendHookOn, 0, NULL);
}

void ATAnswer(void)
{
    char * command;

    command = (char *) CsrPmemAlloc(5);
    sprintf(command, "ATA\r");
    CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 4);
}

void SendTestData(void)
{
    char * command;

    command = (char *) CsrPmemAlloc(14);
    sprintf(command, "TEST STRING!\r");
    CsrBtDuncDataReqSend(CSR_BT_DUNC_IFACEQUEUE, (CsrUint8 *) command, 13);
}



