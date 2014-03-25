/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_phdc_ag_app_main.h"
#ifdef _WIN32
#include <windows.h>
#endif


#include "csr_ui_lib.h"
#include "csr_bt_ui_strings.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_app_lib.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_bt_hdp_prim.h"
#include "csr_bt_phdc_ag_app.h"
#include "csr_bt_phdc_ag_lib.h"
#include "csr_bt_util.h"

#include "csr_exceptionhandler.h"

#include "csr_bt_hdp_lib.h"
#include "csr_bt_gap_app_lib.h"

CsrCharString *phdcAppAgendPointState[3] = {
                                "Active",
                                "Suspended",
                                "Disassociated"
                                };

extern BD_ADDR_T defGlobalBdAddr;

PhdcAppAgEndPoint * PhdcAppAgEndPointFindFromPopDisplayHandle(PhdcAppAgInstance * pInstance, CsrUieHandle displayHandle)
{
    CsrUint8 i;

    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT; i++ )
    {
        if ( pInstance->endPoint[i].popUpHdl->displayHandle == displayHandle)
            return &pInstance->endPoint[i];
    }

    return NULL;
}

PhdcAppAgEndPoint * PhdcAppAgEndPointFindFromMenuDisplayHandle(PhdcAppAgInstance * pInstance, CsrUieHandle displayHandle)
{
    CsrUint8 i;

    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT; i++ )
    {
        if ( pInstance->endPoint[i].subMenuHdl->displayHandle == displayHandle)
            return &pInstance->endPoint[i];
    }

    return NULL;
}

CsrBool PhdcAppAgAreDevicesConnected(PhdcAppAgInstance * pInstance)
{
    if ( CsrBtBdAddrEqZero(&pInstance->endPoint[0].bd ))
        return TRUE;
    else
        return FALSE;
}


void PhdcAppAgMainMenuSK1Handler(void *context, CsrUieHandle displayHandle,
                                   CsrUieHandle eventHandle, CsrUint16 key)
{
    PhdcAppAgInstance *pInstance = (PhdcAppAgInstance *) context;
    if ( pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle == displayHandle )
    {
        switch (key)
        {
            case PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS :
            {
                pInstance->endpointActivated = PHDC_APP_AG_END_POINT_WS;
                
                if ( pInstance->bRegistered == FALSE )
                {
                    CsrBtPhdcAgConfigureEndpointReqSend(CsrSchedTaskQueueGet(),
                                                        SECL_NONE,
                                                        CsrStrDup(pInstance->pService),
                                                        NULL,
                                                        CsrStrDup(pInstance->pProvider),
                                                        0,
                                                        1,
                                                        CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE);
                }
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_AG_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2("Already Activated"),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);

                    pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_ACTIVATE;

                    CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);

                }
                break;
            }

            case PHDC_APP_AG_MAIN_MENU_ACTIVATE_BPM :
            {
                pInstance->endpointActivated = PHDC_APP_AG_END_POINT_BPM;

                if ( pInstance->bRegistered == FALSE )
                {
                    CsrBtPhdcAgConfigureEndpointReqSend(CsrSchedTaskQueueGet(),
                                                        SECL_NONE,
                                                        CsrStrDup(pInstance->pService),
                                                        NULL,
                                                        CsrStrDup(pInstance->pProvider),
                                                        0,
                                                        1,
                                                        CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE);
                }
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_AG_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2("Already Activated"),
                                          CONVERT_TEXT_STRING_2_UCS2("OK"),
                                          NULL,
                                          pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION,
                                          CSR_UI_DEFAULTACTION);

                    pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_ACTIVATE;

                    CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                        CsrSchedTaskQueueGet(),
                                        CSR_UI_INPUTMODE_AUTO,
                                        CSR_UI_LOWESTPRIORITY);
                }
                break;
            }

            case PHDC_APP_AG_MAIN_GET_CAPAB:
            {
                if(CsrBtBdAddrEqZero(&defGlobalBdAddr))
                {
                    CsrBtGapAppGetSelectedDeviceReqSend(CsrSchedTaskQueueGet());
                }
                else
                {
                    CsrBtBdAddrCopy(&pInstance->deviceAddr, &defGlobalBdAddr);

                    CsrBtPhdcAgGetDeviceCapabReqSend(defGlobalBdAddr, 0xFF);

                }
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("PHDC Agent : Info "),
                                      CONVERT_TEXT_STRING_2_UCS2("Requesting capabilities..."),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);

                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_ACTIVATE;

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);

                if(pInstance->numCapab != 0)
                {
                    CsrUint8 tempCount=0;
                    for(tempCount=0; tempCount<pInstance->numCapab; tempCount++)
                    {
                        CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle,
                                                   tempCount);
                    }
                }
                pInstance->numCapab = 0;
                break;
            }

            default :
                break;

        }
    }
    else if (pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle == displayHandle)
    {
        CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle);

        if(pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_OPERATING)
        {
            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_OPERATING_MENU].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
        }
        else if(pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_MAIN_MENU)
        {
            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
        }
        else if(pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_RESUME_MENU)
        {
            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MENU_RESUME].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
        }
        else if(pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_DEVICE_CAPAB_MENU)
        {
            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
        }
        else if(pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_ACCEPT_BT_LINK)
        {
            CsrBool accept;
            if (key == CSR_UI_KEY_SK1)
            {
                accept = TRUE;
                
            }
            else if (key == CSR_UI_KEY_SK2)
            {
                accept = FALSE;
            }
            CsrBtPhdcAgAcceptBtLinkResSend(pInstance->deviceAddr, accept,/*896*/150);
            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
            if(accept == TRUE)
            {
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                
            }
        }
        
    }
    else if (pInstance->displayHandlers[PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU].displayHandle == displayHandle)
    {
        CsrBtPhdcAgConfigureEndpointReqSend(CsrSchedTaskQueueGet(),
                                            SECL_NONE,
                                            CsrStrDup(pInstance->pService),
                                            CsrStrDup(pInstance->pService),
                                            CsrStrDup(pInstance->pProvider),
                                            0,
                                            1,
                                            CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE);
    }
    else if (pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle == displayHandle)
    {
        CsrUint8 i;
        CsrUint16 devConfigId = 0;

        for (i = 0; i < pInstance->numCapab; i++)
        {
            if(pInstance->deviceCapab[i].mdepDataType == key)
            {
                if(key == CSR_BT_HDP_DEVICE_TYPE_BP)
                {
                    devConfigId = CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_BP;                
                }
                else if(key  == CSR_BT_HDP_DEVICE_TYPE_SCALE) 
                {
                    devConfigId = CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_WS;
                }
                break;
            }
        }

        CsrBtPhdcAgAssociateReqSend(pInstance->deviceCapab[i].psmIdentifier,
                                    defGlobalBdAddr,
                                    pInstance->deviceCapab[i].mdepId,
                                    pInstance->deviceCapab[i].mdepDataType,
                                    100,
                                    devConfigId,
                                    TRUE);

        CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle);     
    }
    else if ( pInstance->displayHandlers[PHDC_APP_AG_UI_OPERATING_MENU].displayHandle == displayHandle )
    {
        switch (key)
        {
            case PHDC_APP_AG_OPERATING_MENU_SEND_DATA:
            {
                CsrUiInputdialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].displayHandle,
                                           CONVERT_TEXT_STRING_2_UCS2("Phdc Agent"),
                                           CONVERT_TEXT_STRING_2_UCS2("Enter Data"),
                                           CSR_UI_ICON_EMPTY,
                                           CONVERT_TEXT_STRING_2_UCS2(" "),
                                           100,
                                           CSR_UI_KEYMAP_NUMERIC,
                                           CONVERT_TEXT_STRING_2_UCS2("OK"),
                                           CSR_UI_DEFAULTACTION,
                                           pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].sk1EventHandle,
                                           CSR_UI_DEFAULTACTION,
                                           CSR_UI_DEFAULTACTION,
                                           CSR_UI_DEFAULTACTION);

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);

                break;
            }
            case PHDC_APP_AG_OPERATING_MENU_DISASSOCIATE:
            {
                CsrBtPhdcAgDisassociateReqSend(pInstance->dataChannelId, 0 /* NORMAL*/);
                break;
            }

            case PHDC_APP_AG_OPERATING_MENU_SUSPEND:
            {
                CsrBtPhdcAgSuspendReqSend(pInstance->dataChannelId);
                break;
            }

            default:
                break;
        }

    }
    else if ( pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].displayHandle == displayHandle )
    {
        CsrUiInputdialogGetReqSend(CsrSchedTaskQueueGet(), pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].displayHandle);
        CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].displayHandle);

    }
    else if ( pInstance->displayHandlers[PHDC_APP_AG_UI_MENU_RESUME].displayHandle == displayHandle )
    {
        CsrBtPhdcAgResumeReqSend(pInstance->dataChannelId);
    }
    else
    {
         PhdcAppAgEndPoint *pEndPoint = PhdcAppAgEndPointFindFromPopDisplayHandle(pInstance,displayHandle);
         if ( pEndPoint != NULL && pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_ACCEPT_BT_LINK )
         {
            if (key == CSR_UI_KEY_SK1)
            {
            }
            else if (key == CSR_UI_KEY_SK2)
            {
                pEndPoint->busy = FALSE;
            }
            /* CsrBtPhdcAcceptBtLinkResSend(pEndPoint->bd, accept,pEndPoint->maxPktLen); */
            CsrUiUieHideReqSend(pEndPoint->popUpHdl->displayHandle);
         }
         else if ( pEndPoint != NULL &&
                   pInstance->popUpInput == PHDC_APP_AG_DEFAULT_POP_UP_NORMAL)
         {
            CsrUiUieHideReqSend(pEndPoint->popUpHdl->displayHandle);
         }
        else if (pEndPoint == NULL)
        {

            pEndPoint = PhdcAppAgEndPointFindFromMenuDisplayHandle(pInstance, displayHandle);

            pInstance->pScratchString[0] = 0;

            if ( key == PHDC_APP_AG_ACTIVE_DEVICE_SUB_MENU_STATUS)
            {
                CsrUiDialogSetReqSend(pEndPoint->popUpHdl->displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_AG_UI_HEADING_1),
                                      CONVERT_TEXT_STRING_2_UCS2(phdcAppAgendPointState[pEndPoint->state]),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pEndPoint->popUpHdl->sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
 
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_NORMAL;

                CsrUiUieShowReqSend(pEndPoint->popUpHdl->displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);

            }
        }

     }

}




void CsrBtPhdcAppAgCreateCfmHandler(PhdcAppAgInstance * pInstance)
{
    CsrUiUieCreateCfm *pCfm = (CsrUiUieCreateCfm *)pInstance->recvMsgP;
    PhdcAppAgDisplayHandle *pDisplay = &pInstance->displayHandlers[pInstance->uiIndex];

    switch (pInstance->uiIndex )
    {
        case PHDC_APP_AG_UI_MAIN_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {

                CsrUint8 i = 0;
                for ( i = 0 ;  i < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT; i++)
                {
                    pInstance->endPoint[i].popUpHdl->sk1EventHandle = pCfm->handle;
                    pInstance->endPoint[i].popUpHdl->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                    pInstance->endPoint[i].subMenuHdl->sk1EventHandle = pCfm->handle;
                    pInstance->endPoint[i].subMenuHdl->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                }

                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Agent : Main Menu"),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_FIRST,
                                        PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Activate WS"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_FIRST,
                                        PHDC_APP_AG_MAIN_MENU_ACTIVATE_BPM,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Activate BPM"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);
                
                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_LAST,
                                        PHDC_APP_AG_MAIN_GET_CAPAB,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Get Manager Capab"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU;
            }
            else
            {

            }
            break;
        }
        

        case PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);

            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {

                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Agent : Main Menu"),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS,
                                        PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Device Configure"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_AG_UI_DEVICE_CAPAB_MENU;


            }
            break;
        }

        case PHDC_APP_AG_UI_DEVICE_CAPAB_MENU:
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;
                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {

                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Agent : Device Capab"),
                                    CONVERT_TEXT_STRING_2_UCS2("Associate"),
                                    NULL);


                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_AG_UI_OPERATING_MENU;
            }
            break;

        }

        case PHDC_APP_AG_UI_OPERATING_MENU:
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_OPERATING_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;
                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {

                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Operating"),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);
                
                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_FIRST,
                                        PHDC_APP_AG_OPERATING_MENU_SEND_DATA,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Send Data"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_LAST,
                                        PHDC_APP_AG_OPERATING_MENU_DISASSOCIATE,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Disassociate"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        CSR_UI_LAST,
                                        PHDC_APP_AG_OPERATING_MENU_SUSPEND,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Suspend"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_INPUTDIALOG);
                pInstance->uiIndex = PHDC_APP_AG_UI_INPUT_DIALOG_MENU;
            }
            break;

        }


        case PHDC_APP_AG_UI_INPUT_DIALOG_MENU:
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_INPUTDIALOG)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_INPUT_DIALOG_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {
                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());


                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_AG_UI_MENU_RESUME;

            }
            break;
        }

        case PHDC_APP_AG_UI_MENU_RESUME:
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_MENU_RESUME].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;
                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {

                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Agent : Resume"),
                                    CONVERT_TEXT_STRING_2_UCS2("Resume"),
                                    NULL);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        PHDC_APP_AG_RESUME_MENU_RESUME,
                                        PHDC_APP_AG_RESUME_MENU_RESUME,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Resume Service"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
                pInstance->uiIndex = PHDC_APP_AG_UI_DEFAULT_POP_UP;

            }
            break;

        }


        case PHDC_APP_AG_UI_DEFAULT_POP_UP :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_DIALOG)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_AG_UI_ACTIVE_DEVICE_MENU;

            }
            else
            {
            }
            break;
        }

        case PHDC_APP_AG_UI_ACTIVE_DEVICE_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppAgMainMenuSK1Handler;

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_AG_HEADING_1_ACTIVE_DEVICES),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

            }

            break;
        }
        default :
        {
            /* for pop-up boxes per device end-point */
            if (pCfm->elementType == CSR_UI_UIETYPE_DIALOG &&
                pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT)
            {
                pInstance->endPoint[pInstance->uiIndex].popUpHdl = pDisplay;
                pDisplay->displayHandle = pCfm->handle;

                pInstance->uiIndex++;

                if (pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT)
                {
                    CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
                }
                else
                {
                    CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                }
            }
            /* for sub-menus per device end-point */
            else if (pCfm->elementType == CSR_UI_UIETYPE_MENU &&
                pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_AG_END_POINT+2)
            {
                /* minus 2 ( 2 dialogs are alredy created) to access the right
                   end-point entry*/
                pInstance->endPoint[pInstance->uiIndex-2].subMenuHdl = pDisplay;
                pDisplay->displayHandle = pCfm->handle;

                /* uiIndex increments and becomes equal to PHDC_APP_AG_UI_MAIN_MENU*/
                pInstance->uiIndex++;
                
                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
            }
            else
            {

            }
            break;
        }
    }
}


static PhdcAppAgDisplayHandle *PhdcMgrFindDisplayHandle(PhdcAppAgInstance * pInstance)
{
    CsrUiEventInd *pInd = (CsrUiEventInd *)pInstance->recvMsgP;
    PhdcAppAgDisplayHandle *pHandle=NULL;
    CsrUint8 i = 0;
    while ( i < NUMBER_OF_CSR_BT_PHDC_APP_AG_UI)
    {
        if ( pInstance->displayHandlers[i].displayHandle == pInd->displayElement)
        {
            pHandle = &pInstance->displayHandlers[i];
            break;
        }
        else
        {
            i++;
        }
    }
    return pHandle;
}

void CsrBtPhdcAppAgEventIndHandler(PhdcAppAgInstance * pInstance)
{
    CsrUiEventInd *pInd = (CsrUiEventInd *)pInstance->recvMsgP;

    PhdcAppAgDisplayHandle *pHandle=PhdcMgrFindDisplayHandle(pInstance);

    if (pHandle != NULL)
    {
        if (pHandle->sk1EventHandle == pInd->event)
        (*pHandle->sk1EventHandleFunc)(pInstance,
                                   pHandle->displayHandle,
                                   pHandle->sk1EventHandle,
                                   pInd->key);
    }
}



void CsrBtPhdcAppAgIpDialogGetCfmHandler(PhdcAppAgInstance * pInstance)
{
    CsrUint8 *pData;
    CsrUint32 data;
    CsrBtPhdcAgDataNode dataNode;
    CsrUiInputdialogGetCfm *pCfm = (CsrUiInputdialogGetCfm *)pInstance->recvMsgP;

    pData = CsrUtf16String2Utf8(pCfm->text);
    data = CsrStrToInt((char*)pData);
    dataNode.objHandle = 1;
    dataNode.personId = CSR_BT_PHDC_AG_PERSON_ID_NA;
    dataNode.time.century = 0x20;
    dataNode.time.year = 0x11;
    dataNode.time.month = 0x02;
    dataNode.time.day = 0x02;
    dataNode.time.hour = 0x14;
    dataNode.time.minute = 0x34;
    dataNode.time.second = 0x20;
    dataNode.time.secondFractions = 0x00;

    dataNode.dataLength = 2;
    dataNode.data = (CsrUint16 *)CsrPmemAlloc(2*sizeof(CsrUint16));
    if(dataNode.dataLength == 2)
    {
        dataNode.data[0] = (CsrUint16)((data & 0xFFFF0000) >> 16);
        dataNode.data[1] = (CsrUint16)(data & 0x0000FFFF);
    }
    else if(dataNode.dataLength == 1)
    {
        dataNode.data[0] = (CsrUint16)((data & 0xFF00) >> 16);
    }

    CsrBtPhdcAgDataReqSend(pInstance->dataChannelId, CSR_BT_PHDC_TRANSMIT_FLAG_FINAL,dataNode);

    CsrMemSet(pData, 0, sizeof(10));

}

static const CsrBtPhdcAgEventHandler csrUiPhdcMsgHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] =
{ /* Jump tabel to handle Upstream CSR_UI messages send from the Phone Emulator UI (CSR_UI)     */
    CsrBtPhdcAppAgCreateCfmHandler,                 /* CSR_UI_UIE_CREATE_CFM                  */
    NULL,                                           /* CSR_UI_MENU_GET_CFM                    */
    NULL,                                           /* CSR_UI_MENU_GETITEM_CFM                */
    NULL,                                           /* CSR_UI_MENU_GETCURSOR_CFM              */
    NULL,                                           /* CSR_UI_EVENT_GET_CFM                   */
    NULL,                                           /* CSR_UI_DIALOG_GET_CFM                  */
    CsrBtPhdcAppAgIpDialogGetCfmHandler,            /* CSR_UI_INPUTDIALOG_GET_CFM             */
    NULL,                                           /* CSR_UI_IDLESCREEN_GET_CFM              */
    NULL,                                           /* CSR_UI_DISPLAY_GETHANDLE_CFM           */
    NULL,                                           /* CSR_UI_KEYDOWN_IND                     */
    CsrBtPhdcAppAgEventIndHandler,                  /* CSR_UI_EVENT_IND                       */
};

static void CsrBtPhdcAppAgIntialiseInstance(PhdcAppAgInstance *pInstance)
{
    CsrCharString bpm[] = "BPM",ws[] = "WS";
    CsrCharString service[] = "Health";
    CsrCharString provider[] = "C S R";

    pInstance->pService = CsrPmemZalloc(CsrStrLen(service)+1);
    CsrStrCpy(pInstance->pService,service);

    pInstance->pProvider = CsrPmemZalloc(CsrStrLen(provider)+1);
    CsrStrCpy(pInstance->pProvider,provider);

    pInstance->endPoint[PHDC_APP_AG_END_POINT_BPM].mdepDataType = CSR_BT_HDP_DEVICE_TYPE_BP;
    pInstance->endPoint[PHDC_APP_AG_END_POINT_BPM].mdepDesc = CsrPmemZalloc(CsrStrLen(bpm)+1);
    CsrStrCpy(pInstance->endPoint[PHDC_APP_AG_END_POINT_BPM].mdepDesc,bpm);
    pInstance->endPoint[PHDC_APP_AG_END_POINT_BPM].maxPktLen = 896;

    pInstance->endPoint[PHDC_APP_AG_END_POINT_WS].mdepDataType = CSR_BT_HDP_DEVICE_TYPE_SCALE;
    pInstance->endPoint[PHDC_APP_AG_END_POINT_WS].mdepDesc = CsrPmemZalloc(CsrStrLen(ws)+1);
    CsrStrCpy(pInstance->endPoint[PHDC_APP_AG_END_POINT_WS].mdepDesc,ws);
    pInstance->endPoint[PHDC_APP_AG_END_POINT_WS].maxPktLen = 896;

    pInstance->pScratchString = CsrPmemZalloc(250);

}
/**************************************************************************************************
 * CsrBtAppInit
 **************************************************************************************************/
void CsrBtPhdcAgAppInit(void **gash)
{
    PhdcAppAgInstance *pInstance;
       #ifndef _WIN32_WCE
#ifdef _WIN32
    /* Windows is sloppy, so boost ourselves */
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif
#endif

    *gash       = (void *) CsrPmemAlloc(sizeof(*pInstance));
    pInstance    = (PhdcAppAgInstance*)*gash;
    CsrMemSet(pInstance, 0, sizeof(*pInstance));

    CsrBtPhdcAppAgIntialiseInstance(pInstance);

    CsrAppRegisterReqSend(CsrSchedTaskQueueGet(), TECH_BT, PROFILE_PHDC_AGENT);
    /* printf(" \nsent regreq %d\n",CsrSchedTaskQueueGet());*/
}

/**************************************************************************************************
 * CsrBtAppDeinit
 **************************************************************************************************/

void CsrBtPhdcAgAppDeinit(void **gash)
{

}




/**************************************************************************************************
 * CsrBtAppHandler
 **************************************************************************************************/
void CsrBtPhdcAgAppHandler(void **gash)
{

    PhdcAppAgInstance *pInstance;
    CsrPrim *prim;
    CsrUint16   evIndex;

    pInstance = (PhdcAppAgInstance *) (*gash);
    CsrSchedMessageGet(&pInstance->eventType, &pInstance->recvMsgP);


    prim = pInstance->recvMsgP;
    /* printf(" msg rcvd CsrBtPhdcAgAppHandler, event type %d\n",pInstance->eventType);*/

    switch (pInstance->eventType)
    {
        case CSR_APP_PRIM :
        {
            if (*prim == CSR_APP_TAKE_CONTROL_IND)
            {
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
            }
            else if (*prim == CSR_APP_REGISTER_CFM)
            {
                CsrAppRegisterCfm *pAppRegCfm = (CsrAppRegisterCfm *) pInstance->recvMsgP;

                pInstance->controlId = pAppRegCfm->controlId;
                /*  printf(" rcvd CSR_APP_REGISTER_CFM %d\n",CsrSchedTaskQueueGet());*/

                pInstance->uiIndex  = 0;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
            }
            else
            {
                CsrGeneralException("CSR_APP", 0, *prim,
                                    "Did not handle CSR_APP_PRIM Upstream message");
            }
            break;
        }

        case CSR_UI_PRIM:
        {
            evIndex  = (CsrUint16)(*prim - CSR_UI_PRIM_UPSTREAM_LOWEST);

            if ( ( evIndex <  CSR_UI_PRIM_UPSTREAM_COUNT  )&&
                 ( csrUiPhdcMsgHandlers[evIndex] != NULL  ) )
                csrUiPhdcMsgHandlers[evIndex](pInstance);

            break;

        }
        case CSR_BT_PHDC_AG_PRIM :
        {
            CsrBtPhdcAgAppIfPhdcHandler(pInstance);
            break;
        }

        case CSR_BT_GAP_APP_PRIM :
        {
            CsrBtGapAppGetSelectedDeviceCfm *pCfm = (CsrBtGapAppGetSelectedDeviceCfm *)pInstance->recvMsgP;
            CsrBtBdAddrCopy(&defGlobalBdAddr, &pCfm->deviceAddr);
            CsrBtBdAddrCopy(&pInstance->deviceAddr, &defGlobalBdAddr);
            CsrBtPhdcAgGetDeviceCapabReqSend(pCfm->deviceAddr, 0xFF);
            break;
        }

        default :
        {

            break;
        }
    }
    CsrPmemFree(pInstance->recvMsgP);
}


