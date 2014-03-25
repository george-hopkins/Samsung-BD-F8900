/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_phdc_mgr_app.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "csr_bt_phdc_mgr_lib.h"
#include "csr_ui_lib.h"
#include "csr_bt_ui_strings.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_gap_app_lib.h"

#include "csr_exceptionhandler.h"

extern BD_ADDR_T defGlobalBdAddr;

CsrUint32 getIntegerUint32(CsrUint32  floatVal32)
{
    CsrUint16 exp, tempCount;
    CsrUint32 den=1;
    CsrUint32 value;

    exp = (((CsrUint8)(floatVal32 >> 24) ) > 0) ?
         (256 - ((CsrUint8)(floatVal32 >> 24) )) : 0;

    value = (CsrUint32)(floatVal32 & 0x00FFFFFF);

    for(tempCount=0;tempCount<exp;tempCount++)
    {
        den = den * 10;
    }
    value = value / den;

    return value;
}

CsrUint32 getDecimalUint32(CsrUint32  floatVal32)
{
    CsrUint16 exp, tempCount;
    CsrUint32 den=1;
    CsrUint32 value;

    exp = (((CsrUint8)(floatVal32 >> 24) ) > 0) ?
         (256 - ((CsrUint8)(floatVal32 >> 24) )) : 0;

    value = (CsrUint32)(floatVal32 & 0x00FFFFFF);

    for(tempCount=0;tempCount<exp;tempCount++)
    {
        den = den * 10;
    }

    value = value % den;

    return value;
}



CsrUint16 getInteger(CsrUint16  floatVal16)
{
    CsrUint8 exp = 16 - ((floatVal16 >> 12) );
    CsrUint16 value = floatVal16 & 0xFFF;

    if (exp == 1)
        value = value / 10;
    else if (exp == 2)
        value = value / 100;
    else if (exp == 3)
        value = value / 1000;

    return value;
}

CsrUint16 getDecimal(CsrUint16  floatVal16)
{
    CsrUint8 exp = ((floatVal16 & 0xF000) >> 12 );
    CsrUint16 value;

    if (exp == 0xF)
        value = floatVal16 % 10;
    else if (exp == 0xE)
        value = floatVal16 % 100;
    else if (exp == 0xD)
        value = floatVal16 % 1000;
    else
        value = 0;

    return value;
}

CsrCharString* PhdcMgrMapDataTypeToString(CsrBtMdepDataType   datatype)
{
    switch (datatype)
    {
        case CSR_BT_HDP_DEVICE_TYPE_BP:
        {
            return "BPM";
        }
        case CSR_BT_HDP_DEVICE_TYPE_SCALE:
        {
            return "WS";

        }
        default :
        {
            return "NULL";
        }

    }
}




void csrBtPhdcMgrAppAddDeviceToMenu(PhdcAppMgrInstance *pInstance, PhdcAppMgrEndPoint endpoint)
{    
    if (CsrBtBdAddrEqZero(&endpoint.prevBd))
    {
          #ifndef _WIN32_WCE 
        sprintf(pInstance->pScratchString,
                "%04X-%02X-%06X",
                endpoint.bd.nap,
                endpoint.bd.uap,
                endpoint.bd.lap);
#endif
        CsrUiMenuSetReqSend(endpoint.subMenuHdl->displayHandle,
                            CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                            CONVERT_TEXT_STRING_2_UCS2("Select"),
                            NULL);


        CsrUiMenuAdditemReqSend(endpoint.subMenuHdl->displayHandle,
                                CSR_UI_FIRST,
                                PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_RCV_DATA,
                                CSR_UI_ICON_NONE,
                                CONVERT_TEXT_STRING_2_UCS2("Received Data"),
                                NULL,
                                endpoint.subMenuHdl->sk1EventHandle,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION);
        
        CsrUiMenuAdditemReqSend(endpoint.subMenuHdl->displayHandle,
                                CSR_UI_LAST,
                                PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_STATUS,
                                CSR_UI_ICON_NONE,
                                CONVERT_TEXT_STRING_2_UCS2("Status"),
                                NULL,
                                endpoint.subMenuHdl->sk1EventHandle,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION);

        /*  Add BT_ADDR menu for the active device sub menu */
        CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU].displayHandle,
                endpoint.mdepId,
                endpoint.mdepId,
                CSR_UI_ICON_NONE,
                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                NULL,
                endpoint.subMenuHdl->displayHandle,
                CSR_UI_DEFAULTACTION,
                CSR_UI_DEFAULTACTION,
                CSR_UI_DEFAULTACTION);

        /* Now associated  - add Device menu to Main menu */
        CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                            PHDC_APP_MGR_MAIN_MENU_DEVICES,
                            PHDC_APP_MGR_MAIN_MENU_DEVICES,
                            CSR_UI_ICON_NONE,
                            CONVERT_TEXT_STRING_2_UCS2("Devices"),
                            NULL,
                            pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                            CSR_UI_DEFAULTACTION,
                            CSR_UI_DEFAULTACTION,
                            CSR_UI_DEFAULTACTION);
    }
    else if (!CsrBtBdAddrEq(&endpoint.bd, &endpoint.prevBd))
    {
          #ifndef _WIN32_WCE 
        sprintf(pInstance->pScratchString,
                "%04X-%02X-%06X",
                endpoint.bd.nap,
                endpoint.bd.uap,
                endpoint.bd.lap);
#endif
        CsrUiMenuSetReqSend(endpoint.subMenuHdl->displayHandle,
                            CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                            CONVERT_TEXT_STRING_2_UCS2("Select"),
                            NULL);

        CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU].displayHandle,
                                   endpoint.mdepId);

        CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU].displayHandle,
                endpoint.mdepId,
                endpoint.mdepId,
                CSR_UI_ICON_NONE,
                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                NULL,
                endpoint.subMenuHdl->displayHandle,
                CSR_UI_DEFAULTACTION,
                CSR_UI_DEFAULTACTION,
                CSR_UI_DEFAULTACTION);
    }        
    CsrUiMenuAdditemReqSend(endpoint.subMenuHdl->displayHandle,
                            CSR_UI_LAST,
                            PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_DISCONNECT,
                            CSR_UI_ICON_NONE,
                            CONVERT_TEXT_STRING_2_UCS2("Disassociate"),
                            NULL,
                            endpoint.subMenuHdl->sk1EventHandle,
                            CSR_UI_DEFAULTACTION,
                            CSR_UI_DEFAULTACTION,
                            CSR_UI_DEFAULTACTION);
}

/**************************************************************************************************
 * CsrBtAppHandler
 **************************************************************************************************/
void CsrBtPhdcAppTimedEvent(CsrUint16 event, void *pv_arg1)
{
    CsrUieHandle displayHandle = event;
    PhdcAppMgrInstance *pInstance = (PhdcAppMgrInstance *)pv_arg1;

    pInstance->timerId = 0;
    
    if(pInstance->popUpInput != PHDC_APP_MGR_DEFAULT_POP_UP_INVALID)
    {
        pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_INVALID;
    }

    CsrUiUieHideReqSend(displayHandle);   
}



void CsrBtPhdcAppMgrIfPhdcHandler(PhdcAppMgrInstance *pInstance)
{
    CsrPrim *prim;
    CsrUint8 i;

    prim = pInstance->recvMsgP;

    switch (*prim)
    {
        case CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND:
        {
            CsrBtPhdcMgrConfigureEndpointInd *pInd = (CsrBtPhdcMgrConfigureEndpointInd *)pInstance->recvMsgP;

            if (pInd->mdepId == PHDC_APP_END_POINT_BPM)
            {
                CsrBtPhdcMgrConfigureEndpointResSend(pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDataType,
                                                  CSR_BT_HDP_MDEP_SINK,
                                                  CsrUtf8StrDup(pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDesc),
                                                  FALSE);
            }
            else if (pInd->mdepId == PHDC_APP_END_POINT_WS)
            {
                CsrBtPhdcMgrConfigureEndpointResSend(pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDataType,
                                                  CSR_BT_HDP_MDEP_SINK,
                                                  CsrUtf8StrDup(pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDesc),
                                                  FALSE);

            }
            else
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                    CONVERT_TEXT_STRING_2_UCS2("Endpoint not supported! "),
                                    NULL,
                                    NULL,
                                    CSR_UI_DEFAULTACTION,
                                    CSR_UI_DEFAULTACTION,
                                    CSR_UI_DEFAULTACTION,
                                    CSR_UI_DEFAULTACTION);

                pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_ACTIVATE;

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                        CsrSchedTaskQueueGet(),
                        CSR_UI_INPUTMODE_AUTO,
                        CSR_UI_LOWESTPRIORITY);

                CsrBtPhdcMgrConfigureEndpointResSend(0,
                                                     0xFF,
                                                     CsrUtf8StrDup(CsrUtf8StrCpy(pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDesc,(CsrUtf8String *) "none")),
                                                     FALSE);

            }

            break;
        }

        case CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM:
        {
            CsrBtPhdcMgrConfigureEndpointCfm *pCfm = (CsrBtPhdcMgrConfigureEndpointCfm *)pInstance->recvMsgP;

            if (pCfm->resultCode ==  CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS &&
                pCfm->resultSupplier == CSR_BT_SUPPLIER_PHDC)
            {
                if(pInstance->proximityInUse)
                {
                    /* Give a search for all device specilisation */
                    pInstance->getCapabIndex = 0;

                    /* Request Proximity pairing with Health device */
                    CsrBtGapAppPairProximityDeviceReqSend(CsrSchedTaskQueueGet(), 0, 0, FALSE);
                    /* Wait for connect ... */
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2("Searching for device..."),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);
                }
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                        CONVERT_TEXT_STRING_2_UCS2("Phdc Mgr : Activation"),
                                        CONVERT_TEXT_STRING_2_UCS2("Success"),
                                        CONVERT_TEXT_STRING_2_UCS2("OK"),
                                        NULL,
                                        pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);
                    
                }
                /* place holder to display relevant menu items when the device is
                       activated and connectable */
                CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                        PHDC_APP_MGR_MAIN_MENU_CAPAB,
                                        PHDC_APP_MGR_MAIN_MENU_CAPAB,
                                        CSR_UI_ICON_NONE,
                                        PHDC_APP_MGR_MAIN_MENU_CAPAB_TITLE,
                                        NULL,
                                        pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);
                
                /* activation is succesful so remove Activate from the main menu */
                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                           PHDC_APP_MGR_MAIN_MENU_ACTIVATE);
                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                           PHDC_APP_MGR_MAIN_MENU_PROXIMITY);
            }
            else
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("Phdc Mgr : Activation"),
                                      CONVERT_TEXT_STRING_2_UCS2("Failed"),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
            }
        
            pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_ACTIVATE;

            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
        
            pInstance->bRegistered = TRUE;

            break;
        }

        case CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND :
        {
            CsrBtPhdcMgrAcceptBtLinkInd *pInd = (CsrBtPhdcMgrAcceptBtLinkInd *)pInstance->recvMsgP;
            CsrBool  freeEndPoint = FALSE;

            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++ )
            {
                if ( pInstance->endPoint[i].busy == FALSE)
                {
                    freeEndPoint = TRUE;
                    pInstance->endPoint[i].busy = TRUE;
                    pInstance->endPoint[i].state = PHDC_APP_MGR_EP_ST_DISASSOCIATED;
                    break;
                }
            }

            if (freeEndPoint)
            {
                if ( !CsrBtBdAddrEq(&pInstance->endPoint[i].bd, &pInd->deviceAddr))
                {
                    pInstance->endPoint[i].bd = pInd->deviceAddr;
                    CsrMemSet(pInstance->endPoint[i].obsScan,0,sizeof(pInstance->endPoint[i].obsScan));
                    pInstance->endPoint[i].obsScanIdx = 0;

                }
                CsrBtPhdcMgrAcceptBtLinkResSend(pInd->deviceAddr, TRUE, pInstance->endPoint[i].maxPktLen);
            }
            else
            {
                CsrBtPhdcMgrAcceptBtLinkResSend(pInd->deviceAddr,FALSE, 0xFF);
            }
            break;


        }

        case CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND :
        {
            CsrBtPhdcMgrAcceptAssociateInd *pInd = (CsrBtPhdcMgrAcceptAssociateInd *)pInstance->recvMsgP;
            CsrBool found = FALSE;
           
            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( CsrBtBdAddrEq(&pInstance->endPoint[i].bd, &pInd->deviceAddr))
                {
                    CsrBtPhdcMgrAcceptAssociateResSend(pInstance->endPoint[i].bd, (CsrUint8 *) pInstance->endPoint[i].pSystemId, TRUE);
                    found = TRUE;
                    break;
                }
            }

            if(!found)
            {
                CsrBtPhdcMgrAcceptAssociateResSend(pInd->deviceAddr, (CsrUint8 *) pInstance->endPoint[0].pSystemId, FALSE);
            }
            break;
        }


        case CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND:
        {
            CsrBtPhdcMgrAssociateCompleteInd *pInd = (CsrBtPhdcMgrAssociateCompleteInd *)pInstance->recvMsgP;

            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( CsrBtBdAddrEq(&pInstance->endPoint[i].bd, &pInd->deviceAddr))
                {
                    if ( pInd->resultSupplier == CSR_BT_SUPPLIER_PHDC &&
                         pInd->resultCode == CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS)
                    {
                          #ifndef _WIN32_WCE 
                        sprintf(pInstance->pScratchString,
                                "Association Successfull from remote device %04X-%02X-%06X",
                                pInd->deviceAddr.nap,
                                pInd->deviceAddr.uap,
                                pInd->deviceAddr.lap);
                          #endif
                        pInstance->endPoint[i].dataChannelId = pInd->dataChannelId;
                        pInstance->endPoint[i].state = PHDC_APP_MGR_EP_ST_ACTIVE;
                        pInstance->noActiveDevices++;                        
                    }
                    else
                    {
                           #ifndef _WIN32_WCE
                        sprintf(pInstance->pScratchString,
                                "Association Failure from remote device %04X-%02X-%06X !",
                                pInd->deviceAddr.nap,
                                pInd->deviceAddr.uap,
                                pInd->deviceAddr.lap);
                           #endif
                    }

                    pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_ASSOC_COMPLETE;

                    CsrUiDialogSetReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->endPoint[i].popUpHdl->sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);

                    CsrUiUieShowReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);

                    if ( pInd->resultSupplier == CSR_BT_SUPPLIER_PHDC &&
                        pInd->resultCode == CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS)
                    {
                        /* Add device to device menu */
                        csrBtPhdcMgrAppAddDeviceToMenu(pInstance, pInstance->endPoint[i]);
                    }

                    CsrSchedTimerCancel(pInstance->timerId,NULL,NULL);
                    pInstance->timerId = CsrSchedTimerSet(PHDC_APP_POPUP_TIMEOUT, CsrBtPhdcAppTimedEvent, pInstance->endPoint[i].popUpHdl->displayHandle, (void*)pInstance);
                  
                    break;
                }

            }
            break;
        }
        case CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND:
        {
            break;
        }
        case CSR_BT_PHDC_MGR_BPM_BP_IND:
        {
            CsrBtPhdcMgrBpmBpInd *pInd = (CsrBtPhdcMgrBpmBpInd *) pInstance->recvMsgP;

            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( pInd->dataChannelId == pInstance->endPoint[i].dataChannelId)
                {
                       #ifndef _WIN32_WCE
                    sprintf(pInstance->endPoint[i].obsScan[pInstance->endPoint[i].obsScanIdx],
                            "PID:%d\nTime:%02X%02X-%02X-%02X %02X:%02X:%02X.%02X Sys:%d.%d Dias:%d.%d MAP:%d.%d %s\n",
                            (pInd->personId == CSR_BT_PHDC_MGR_PERSON_ID_NA ? 0 : pInd->personId),
                            pInd->time.century,pInd->time.year,pInd->time.month,pInd->time.day,
                            pInd->time.hour,pInd->time.minute,pInd->time.second,pInd->time.secondFractions,
                            getInteger(pInd->systolicPressure),getDecimal(pInd->systolicPressure),
                            getInteger(pInd->diastolicPressure),getDecimal(pInd->diastolicPressure),
                            getInteger(pInd->meanArterialPressure),getDecimal(pInd->meanArterialPressure),
                            (pInd->unitCode == CSR_BT_PHDC_MGR_DIM_UNIT_CODE_MMHG ? "MMHg" : "Kpa"));
#endif
                    pInstance->endPoint[i].obsScanIdx = (++pInstance->endPoint[i].obsScanIdx) % PHDC_APP_MGR_NO_OF_OBS_SCAN;
                    break;
                }
            }
            break;
        }
        case CSR_BT_PHDC_MGR_BPM_PULSE_IND:
        {
            CsrBtPhdcMgrBpmPulseInd *pInd = (CsrBtPhdcMgrBpmPulseInd *) pInstance->recvMsgP;
            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( pInd->dataChannelId == pInstance->endPoint[i].dataChannelId)
                {
                       #ifndef _WIN32_WCE
                    sprintf(pInstance->endPoint[i].obsScan[pInstance->endPoint[i].obsScanIdx],
                            "Time:%02X%02X-%02X-%02X %02X:%02X:%02X.%02X Pulse:%d.%d %s\n", 
                            pInd->time.century,pInd->time.year,pInd->time.month,pInd->time.day,
                            pInd->time.hour,pInd->time.minute,pInd->time.second,pInd->time.secondFractions,
                            getInteger(pInd->pulse), 
                            getDecimal(pInd->pulse), 
                            "Bpm");
                       #endif
                    pInstance->endPoint[i].obsScanIdx = (++pInstance->endPoint[i].obsScanIdx) % PHDC_APP_MGR_NO_OF_OBS_SCAN;
                    break;
                }
            }
            break;
        }

        case CSR_BT_PHDC_MGR_WS_WEIGHT_IND:
        {
            CsrBtPhdcMgrWsWeightInd *pInd = (CsrBtPhdcMgrWsWeightInd *) pInstance->recvMsgP;
            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( pInd->dataChannelId == pInstance->endPoint[i].dataChannelId)
                {
                      #ifndef _WIN32_WCE 
                    sprintf(pInstance->endPoint[i].obsScan[pInstance->endPoint[i].obsScanIdx],
                            "Time:%02X%02X-%02X-%02X %02X:%02X:%02X.%02X Weight:%u.%u%s\n", 
                            pInd->time.century,pInd->time.year,pInd->time.month,pInd->time.day,
                            pInd->time.hour,pInd->time.minute,pInd->time.second,pInd->time.secondFractions,
                            getIntegerUint32(pInd->weight), 
                            getDecimalUint32(pInd->weight),
                            (pInd->unitCode == CSR_BT_PHDC_MGR_DIM_UNIT_CODE_KILO_G ? "kg" : "lb"));
                      #endif
                    pInstance->endPoint[i].obsScanIdx = (++pInstance->endPoint[i].obsScanIdx) % PHDC_APP_MGR_NO_OF_OBS_SCAN;
                    break;
                }
            }
            break;
        }

        case CSR_BT_PHDC_MGR_DS_IND :
        {
            CsrBtPhdcMgrDsInd *pInd = (CsrBtPhdcMgrDsInd *) pInstance->recvMsgP;
            /* Unknown data received */
            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( pInd->dataChannelId == pInstance->endPoint[i].dataChannelId)
                {
                       #ifndef _WIN32_WCE
                    sprintf(pInstance->endPoint[i].obsScan[pInstance->endPoint[i].obsScanIdx],
                            "%s", "Unknown data received");
                       #endif
                    pInstance->endPoint[i].obsScanIdx = (++pInstance->endPoint[i].obsScanIdx) % PHDC_APP_MGR_NO_OF_OBS_SCAN;
                    break;
                }
            }
            break;
        }
        case CSR_BT_PHDC_MGR_DISASSOCIATE_IND :
        case CSR_BT_PHDC_MGR_DISASSOCIATE_CFM :
        {
            CsrBtPhdcMgrDisassociateCfm *pCfm = (CsrBtPhdcMgrDisassociateCfm *)pInstance->recvMsgP;

            pInstance->noActiveDevices--;
            /*if(pInstance->noActiveDevices == 0)
            {
                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                           PHDC_APP_MGR_MAIN_MENU_DEVICES);
            }*/

            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( pCfm->dataChannelId == pInstance->endPoint[i].dataChannelId)
                {
                       #ifndef _WIN32_WCE
                    if ( pCfm->resultSupplier == CSR_BT_SUPPLIER_PHDC &&
                        (pCfm->resultCode == CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS ||
                         pCfm->resultCode == CSR_BT_PHDC_MGR_RLRQ_REASON_NORMAL) )
                    {
                        sprintf(pInstance->pScratchString,
                                "Normal Disassociation from remote device %04X-%02X-%06X",
                                pCfm->deviceAddr.nap,
                                pCfm->deviceAddr.uap,
                                pCfm->deviceAddr.lap);
                }
                    else
                    {
                        sprintf(pInstance->pScratchString,
                                "Disassociation from remote device %04X-%02X-%06X Reason unknown",
                                pCfm->deviceAddr.nap,
                                pCfm->deviceAddr.uap,
                                pCfm->deviceAddr.lap);
                    }
#endif
                    pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL;

                    CsrUiDialogSetReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->endPoint[i].popUpHdl->sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);

                    CsrUiUieShowReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);

                    pInstance->endPoint[i].state = PHDC_APP_MGR_EP_ST_DISASSOCIATED;
                    pInstance->endPoint[i].busy  = FALSE;
                    pInstance->endPoint[i].prevBd = pInstance->endPoint[i].bd;

                    CsrUiMenuRemoveitemReqSend(pInstance->endPoint[i].subMenuHdl->displayHandle,
                                               PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_DISCONNECT);
                    CsrSchedTimerCancel(pInstance->timerId,NULL,NULL);
                    pInstance->timerId = CsrSchedTimerSet(PHDC_APP_POPUP_TIMEOUT, CsrBtPhdcAppTimedEvent, pInstance->endPoint[i].popUpHdl->displayHandle, (void*)pInstance);

                    break;
                }

            }
            break;
        }
        

        case CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND:
        {
           CsrBtPhdcMgrGetDeviceCapabInd *pInd = (CsrBtPhdcMgrGetDeviceCapabInd *)pInstance->recvMsgP;
           CsrBtHdpMdep   *pMdep;

           pInstance->getCapab[pInstance->getCapabIndex].psmIdentifier = pInd->psmIdentifier;
           
           pMdep = &pInd->supportedFeatureList[0];

           for(i=0;i<pInd->supportedFeatureListLength;i++,pMdep++)
           {
              pInstance->getCapab[pInstance->getCapabIndex].mdep[i].mdepId = pMdep->id;
              pInstance->getCapab[pInstance->getCapabIndex].mdep[i].mdepDataType = pMdep->dataType;
           }

           pInstance->getCapab[pInstance->getCapabIndex].count = i;

           CsrPmemFree(pInd->supportedFeatureList);
           CsrPmemFree(pInd->providerName);
           CsrPmemFree(pInd->serviceName);

           /* In case more services than NUMBER_OF_SERVICE_RECORDS are received
              wrap around */
           pInstance->getCapabIndex = ++pInstance->getCapabIndex % NUMBER_OF_SERVICE_RECORDS;

           /* If the device is connectable, the get capab cfm will return success
              even though it does not report any device matching the mask.
              In that case, report a error to the user */
           break;
        }
        case CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM :
        {
            CsrUint8 j = 0,k = 0;
            CsrUint8 *pScratchString = CsrPmemAlloc(250);
            CsrBtPhdcMgrGetDeviceCapabCfm *pCfm = (CsrBtPhdcMgrGetDeviceCapabCfm *)pInstance->recvMsgP;

            CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle);

            if (pCfm->resultCode == CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS &&
                pCfm->resultSupplier == CSR_BT_SUPPLIER_PHDC )
            {
                for ( i = 0; i < pInstance->getCapabIndex; i++)
                {
                    for ( j = 0; j < pInstance->getCapab[i].count; j++)
                    {
                           #ifndef _WIN32_WCE
                        sprintf(pInstance->pScratchString,
                                "PSM:%4X MDEP:%2X",
                                pInstance->getCapab[i].psmIdentifier,
                                pInstance->getCapab[i].mdep[j].mdepId);
                        
                        sprintf((char *) pScratchString, 
                                "Role:%4X Device:%s",
                                pInstance->getCapab[i].mdep[j].mdepRole,
                                PhdcMgrMapDataTypeToString(pInstance->getCapab[i].mdep[j].mdepDataType));
#endif
                        pInstance->getCapab[i].mdep[j].menuIndex = k;

                        CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].displayHandle,
                                                k,
                                                k,
                                                CSR_UI_ICON_NONE,
                                                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                                CONVERT_TEXT_STRING_2_UCS2(pScratchString),
                                                pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].sk1EventHandle,
                                                CSR_UI_DEFAULTACTION,
                                                CSR_UI_DEFAULTACTION,
                                                CSR_UI_DEFAULTACTION);
                        k++;
                    }

                }

                    /* add the get capab and connect agent menu's to the main menu */
                    CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                            PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT,
                                            PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT,
                                            CSR_UI_ICON_NONE,
                                            PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT_TITLE,
                                            NULL,
                                            pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].sk1EventHandle,
                                            CSR_UI_DEFAULTACTION,
                                            CSR_UI_DEFAULTACTION,
                                            CSR_UI_DEFAULTACTION);

                    CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);

            }
            else
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                      CONVERT_TEXT_STRING_2_UCS2("Error ! No services found "),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);

                pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_ACTIVATE;

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                        CsrSchedTaskQueueGet(),
                        CSR_UI_INPUTMODE_AUTO,
                        CSR_UI_LOWESTPRIORITY);
            }
            break;
        }
        case CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM :
        {
            CsrBtPhdcMgrSetupBtLinkCfm *pCfm = (CsrBtPhdcMgrSetupBtLinkCfm *)pInstance->recvMsgP;

            if(pInstance->proximityInUse)
            {
                 CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle);
            }
            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if ( CsrBtBdAddrEq(&pInstance->endPoint[i].bd, &pCfm->deviceAddr))
                {
                    if ( pCfm->resultSupplier == CSR_BT_SUPPLIER_PHDC &&
                         pCfm->resultCode == CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS)
                    {
                           #ifndef _WIN32_WCE
                        sprintf(pInstance->pScratchString,
                                   "Link to device %04X-%02X-%06X : Success",
                                   pCfm->deviceAddr.nap,
                                   pCfm->deviceAddr.uap,
                                   pCfm->deviceAddr.lap);
                           #endif
                        pInstance->endPoint[i].dataChannelId = pCfm->dataChannelId;

                            CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                                       PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT);
                    }
                    else
                    {
                           #ifndef _WIN32_WCE
                        sprintf(pInstance->pScratchString,
                                "Link to device %04X-%02X-%06X : Failed",
                                pCfm->deviceAddr.nap,
                                pCfm->deviceAddr.uap,
                                pCfm->deviceAddr.lap);
                           #endif
                        pInstance->endPoint[i].busy = FALSE;
                        pInstance->endPoint[i].state = PHDC_APP_MGR_EP_ST_DISASSOCIATED;
                    }

                    pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL;

                    CsrUiDialogSetReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->endPoint[i].popUpHdl->sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);

                    CsrUiUieShowReqSend(pInstance->endPoint[i].popUpHdl->displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);
                    break;
                }
            }
            break;
        }

        case CSR_BT_PHDC_MGR_BT_LINK_REL_IND:
        {
            CsrBtPhdcMgrBtLinkRelInd *pInd = (CsrBtPhdcMgrBtLinkRelInd *)pInstance->recvMsgP;

            for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
            {
                if(pInstance->endPoint[i].state == PHDC_APP_MGR_EP_ST_ACTIVE || pInstance->endPoint[i].state == PHDC_APP_MGR_EP_ST_DISASSOCIATED)
                {
                    if ( pInd->dataChannelId == pInstance->endPoint[i].dataChannelId)
                    {
                        if(pInstance->endPoint[i].state == PHDC_APP_MGR_EP_ST_ACTIVE)
                        {
                            pInstance->noActiveDevices--;
                        }
                        pInstance->endPoint[i].state = PHDC_APP_MGR_EP_ST_DISCONNECTED;
                        pInstance->endPoint[i].busy  = FALSE;
                        pInstance->endPoint[i].prevBd = pInstance->endPoint[i].bd;

                        break;
                    }
                }
            }
            
            break;
        }

        
        default :
        {

            break;
        }
    }
}




