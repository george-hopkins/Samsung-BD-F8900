/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_AT_MODULE

#include "csr_bt_at_handler.h"

/*************************************************************************************
atS0QResHandler
************************************************************************************/
void atS0QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS0qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS0qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS3QResHandler
************************************************************************************/
void atS3QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS3qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS3qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS4QResHandler
************************************************************************************/
void atS4QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS4qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS4qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS5QResHandler
************************************************************************************/
void atS5QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS5qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS5qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS6QResHandler
************************************************************************************/
void atS6QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS6qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS6qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS7QResHandler
************************************************************************************/
void atS7QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS7qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS7qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS8QResHandler
************************************************************************************/
void atS8QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS8qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS8qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atS10QResHandler
************************************************************************************/
void atS10QResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgAtcS10qRes    *atPrim;
    atPrim    = (CsrBtAtDgAtcS10qRes *) atData->atMsgP;

    atData->numberOfRespon--;
    atSxQInfoResHandler(atData, atPrim->value);

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
atSxQInfoResHandler
************************************************************************************/
void atSxQInfoResHandler(atInstanceData_t *atData, CsrUint8 value)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;
    if(atData->paramSettings.vValue == 0)
    {

        valueLen = 5;
        dataPtr = CsrPmemAlloc(valueLen);
        dataPtr[0] = (char) (value / 100 + '0');
        value = (CsrUint8)(value % 100);
        dataPtr[1] = (char) (value / 10  + '0');
        dataPtr[2] = (char) (value % 10 + '0');
        dataPtr[3] = atData->paramSettings.s3Value;
        dataPtr[4] = atData->paramSettings.s4Value;
    }
    else
    {
        valueLen = 7;
        dataPtr = CsrPmemAlloc(valueLen);
        dataPtr[0] = atData->paramSettings.s3Value;
        dataPtr[1] = atData->paramSettings.s4Value;
        dataPtr[2] = (char) (value / 100 + '0');
        value = (CsrUint8)(value % 100);
        dataPtr[3] = (char) (value / 10  + '0');
        dataPtr[4] = (char) (value % 10  + '0');
        dataPtr[5] = dataPtr[0];
        dataPtr[6] = dataPtr[1];
    }
    atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
}


/*************************************************************************************
atOKResultResHandler
************************************************************************************/
void atOKResultResHandler(atInstanceData_t *atData)
{
    CsrUint8 * dataPtr;
    CsrUint16 valueLen;

    if(atData->paramSettings.qValue == 0)
    {
        if(atData->paramSettings.vValue == 0)
        {
            valueLen = 2;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = 0;
            dataPtr[1] = atData->paramSettings.s3Value;
        }
        else
        {
            valueLen = 6;
            dataPtr = CsrPmemAlloc(valueLen);
            dataPtr[0] = atData->paramSettings.s3Value;
            dataPtr[1] = atData->paramSettings.s4Value;
            dataPtr[2] = 'O';
            dataPtr[3] = 'K';
            dataPtr[4] = atData->paramSettings.s3Value;
            dataPtr[5] = atData->paramSettings.s4Value;

        }
        atDgDataReqHandler(atData->btConnId, valueLen, dataPtr, atData);
    }
    atData->responseFlag = NOWAITINGFORRESPONSE;
    clearExtendedCmdTable(atData);
}






/*************************************************************************************
atDataResHandler
************************************************************************************/
void atDataResHandler(atInstanceData_t *atData)
{
    CsrBtAtDgDataRes    *atPrim;

    atPrim    = (CsrBtAtDgDataRes *) atData->atMsgP;
    CsrBtDgDataResSend(atPrim->btConnId);
    atData->appResFlag = APP_RES;
}

#endif

