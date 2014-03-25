/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_phdc_mgr_app_main.h"
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
#include "csr_bt_phdc_mgr_app.h"
#include "csr_bt_phdc_mgr_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_app_prim.h"
#include "csr_bt_gap_app_lib.h"

#include "csr_exceptionhandler.h"

CsrCharString *endPointState[3] = {
    "Disconnected",
    "Disassociated",
    "Active"
};

extern BD_ADDR_T defGlobalBdAddr;

PhdcAppMgrEndPoint * PhdcAppMgrEndPointFindFromPopDisplayHandle(PhdcAppMgrInstance * pInstance, CsrUieHandle displayHandle)
{
    CsrUint8 i;

    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++ )
    {
        if ( pInstance->endPoint[i].popUpHdl->displayHandle == displayHandle)
        {
            return &pInstance->endPoint[i];
        }
    }

    return NULL;
}

PhdcAppMgrEndPoint * PhdcAppMgrEndPointFindFromMenuDisplayHandle(PhdcAppMgrInstance * pInstance, CsrUieHandle displayHandle)
{
    CsrUint8 i;

    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++ )
    {
        if ( pInstance->endPoint[i].subMenuHdl->displayHandle == displayHandle)
        {
            return &pInstance->endPoint[i];
        }
    }

    return NULL;
}

CsrBool PhdcAppMgrAreDevicesConnected(PhdcAppMgrInstance * pInstance)
{
    CsrUint8 i;
    CsrBool result = FALSE;

    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++ )
    {
        if ( !CsrBtBdAddrEqZero(&pInstance->endPoint[i].bd ) && (pInstance->endPoint[i].busy == TRUE))
        {
            return FALSE;
        }
        else
        {
            result = TRUE;            
        }
    }
    return result;
}


void PhdcAppMgrMainMenuSK1Handler(void *context, CsrUieHandle displayHandle,
                                  CsrUieHandle eventHandle, CsrUint16 key)
{
    PhdcAppMgrInstance *pInstance = (PhdcAppMgrInstance *) context;
    CsrUint8 i,j,k,l;
    if ( pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle == displayHandle )
    {
        switch (key)
        {
            case PHDC_APP_MGR_MAIN_MENU_ACTIVATE :
            {
                if ( pInstance->bRegistered == FALSE )
                {
                    CsrBtPhdcMgrConfigureEndpointReqSend(CsrSchedTaskQueueGet(),
                                                         SECL_NONE,
                                                         CsrStrDup(pInstance->pService),
                                                         NULL,
                                                         CsrStrDup(pInstance->pProvider),
                                                         0,
                                                         NUMBER_OF_CSR_BT_PHDC_END_POINT,
                                                         CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE,
                                                         NUMBER_OF_CSR_BT_PHDC_END_POINT);
                }
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2("Already Activated"),
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
            case PHDC_APP_MGR_MAIN_MENU_PROXIMITY :
            {
                if ( pInstance->bRegistered == FALSE )
                {
                    pInstance->proximityInUse = TRUE;

                    CsrBtPhdcMgrConfigureEndpointReqSend(CsrSchedTaskQueueGet(),
                                                         SECL_NONE,
                                                         CsrStrDup(pInstance->pService),
                                                         NULL,
                                                         CsrStrDup(pInstance->pProvider),
                                                         0,
                                                         NUMBER_OF_CSR_BT_PHDC_END_POINT,
                                                         CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE,
                                                         NUMBER_OF_CSR_BT_PHDC_END_POINT);
                }
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                          CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                          CONVERT_TEXT_STRING_2_UCS2("Already Activated"),
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
            case PHDC_APP_MGR_MAIN_MENU_DEVICES :
            {
                
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                break;
            }
            case PHDC_APP_MGR_MAIN_MENU_CAPAB :
            {
                k = 0;
                for ( i = 0; i < pInstance->getCapabIndex; i++)
                {
                    for ( j = 0; j < pInstance->getCapab[i].count; j++)
                    {
                        CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].displayHandle, k);
                        k++;
                    }

                }
                
                /* Give a search for all device specilisation */
                pInstance->getCapabIndex = 0;
                
                if(CsrBtBdAddrEqZero(&defGlobalBdAddr))
                {
                    CsrBtGapAppGetSelectedDeviceReqSend(CsrSchedTaskQueueGet());
                }
                else
                {
                    CsrBtPhdcMgrGetDeviceCapabReqSend(defGlobalBdAddr, 0xFF);
                    /* Show dialog get capab*/
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                            CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                            CONVERT_TEXT_STRING_2_UCS2("Device found get capab..."),
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
            case PHDC_APP_MGR_MAIN_MENU_CONNECT_AGENT :
            {
                CsrBool  freeEndPoint = FALSE;
                
                /* optimise the search - 3 loops !*/
                for (i = 0; i < pInstance->getCapabIndex && !freeEndPoint; i++ )
                {
                    for (j = 0; j < pInstance->getCapab[i].count && !freeEndPoint; j++)
                    {
                        if ( pInstance->getCapab[i].mdep[j].menuIndex == pInstance->selectedEndpoint )
                        {
                            for (l =0; l < NUMBER_OF_CSR_BT_PHDC_END_POINT; l++)
                            {
                                if ( pInstance->endPoint[l].mdepDataType == pInstance->getCapab[i].mdep[j].mdepDataType &&
                                    pInstance->endPoint[l].busy == FALSE)
                                {
                                    freeEndPoint = TRUE;
                                    pInstance->endPoint[l].busy = TRUE;
                                    break;
                                }

                            }

                            if(freeEndPoint)
                            {                            
                                pInstance->endPoint[l].bd = defGlobalBdAddr;/* take this from bond, query GAP*/
                                CsrMemSet(pInstance->endPoint[l].obsScan,0,sizeof(pInstance->endPoint[l].obsScan));
                                pInstance->endPoint[l].obsScanIdx = 0;

                                CsrBtPhdcMgrSetupBtLinkReqSend(pInstance->getCapab[i].psmIdentifier,
                                                               defGlobalBdAddr,/* take this from bond, query GAP*/
                                                               pInstance->getCapab[i].mdep[j].mdepId,
                                                               pInstance->getCapab[i].mdep[j].mdepDataType,
                                                               pInstance->endPoint[l].maxPktLen);
                                break;                                
                            }
                        }
                    }
                }
                break;
            }

            default :
                break;

        }
    }
    else if (pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle == displayHandle)
    {
        if ( pInstance->popUpInput == PHDC_APP_MGR_DEFAULT_POP_UP_ACTIVATE &&
             key == CSR_UI_KEY_SK1)
        {
            CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle);
        }
        else
        {

        }
    }
    else if (pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].displayHandle == displayHandle)
    {
        pInstance->selectedEndpoint = key ;
        CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU].displayHandle);
    }
    else
    {
        PhdcAppMgrEndPoint *pEndPoint = PhdcAppMgrEndPointFindFromPopDisplayHandle(pInstance,displayHandle);
         if ( pEndPoint != NULL &&
                  pInstance->popUpInput == PHDC_APP_MGR_DEFAULT_POP_UP_ACCEPT_BT_LINK )
        {
            CsrBool accept;
            if (key == CSR_UI_KEY_SK1)
            {
                accept = TRUE;
            }
            else if (key == CSR_UI_KEY_SK2)
            {
                accept = FALSE;
                pEndPoint->busy = FALSE;
            }
            CsrBtPhdcMgrAcceptBtLinkResSend(pEndPoint->bd, accept,pEndPoint->maxPktLen);
            CsrUiUieHideReqSend(pEndPoint->popUpHdl->displayHandle);
        }
        else if ( pEndPoint != NULL &&
                  pInstance->popUpInput == PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL)
        {
            CsrUiUieHideReqSend(pEndPoint->popUpHdl->displayHandle);
        }
        else if ( pEndPoint != NULL &&
                  pInstance->popUpInput == PHDC_APP_MGR_DEFAULT_POP_UP_ASSOC_COMPLETE )
        {
            pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_INVALID;
            if (key == CSR_UI_KEY_SK1)
            {
                CsrUiUieHideReqSend(pEndPoint->popUpHdl->displayHandle);
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
            }
        }
        else if (pEndPoint == NULL)
        {
            pEndPoint = PhdcAppMgrEndPointFindFromMenuDisplayHandle(pInstance, displayHandle);

            pInstance->pScratchString[0] = 0;

            if ( key == PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_RCV_DATA)
            {
                CsrUint8 len=0;
                for (i=0;i < PHDC_APP_MGR_NO_OF_OBS_SCAN; i++)
                {
                    if ( pEndPoint->obsScan[i] != NULL)
                    {
                        CsrStrCat(&pInstance->pScratchString[len],pEndPoint->obsScan[i]);
                        CsrStrCat(&pInstance->pScratchString[len],"\n");
                    }
                }
                CsrUiDialogSetReqSend(pEndPoint->popUpHdl->displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                      CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pEndPoint->popUpHdl->sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
                pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL;

                CsrUiUieShowReqSend(pEndPoint->popUpHdl->displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);

            }
            else if ( key == PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_STATUS)
            {
                CsrUiDialogSetReqSend(pEndPoint->popUpHdl->displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                      CONVERT_TEXT_STRING_2_UCS2(endPointState[pEndPoint->state]),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pEndPoint->popUpHdl->sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
                pInstance->popUpInput = PHDC_APP_MGR_DEFAULT_POP_UP_NORMAL;

                CsrUiUieShowReqSend(pEndPoint->popUpHdl->displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);

            }
            else if (key == PHDC_APP_MGR_ACTIVE_DEVICE_SUB_MENU_DISCONNECT)
            {
                CsrBtPhdcMgrDisassociateReqSend(pEndPoint->dataChannelId, CSR_BT_PHDC_MGR_RLRQ_REASON_NORMAL);
            }
        }
    }
}




void CsrBtPhdcAppMgrCreateCfmHandler(PhdcAppMgrInstance * pInstance)
{
    CsrUiUieCreateCfm *pCfm = (CsrUiUieCreateCfm *)pInstance->recvMsgP;
    PhdcAppMgrDisplayHandle *pDisplay = &pInstance->displayHandlers[pInstance->uiIndex];

    switch (pInstance->uiIndex )
    {
        case PHDC_APP_MGR_UI_MAIN_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_EVENT);
            }
            else if  (pCfm->elementType == CSR_UI_UIETYPE_EVENT)
            {
                CsrUint8 i = 0;
                for ( i = 0 ;  i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++)
                {
                    pInstance->endPoint[i].popUpHdl->sk1EventHandle = pCfm->handle;
                    pInstance->endPoint[i].popUpHdl->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                    pInstance->endPoint[i].subMenuHdl->sk1EventHandle = pCfm->handle;
                    pInstance->endPoint[i].subMenuHdl->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                }
                pDisplay->sk1EventHandle = pCfm->handle;
                pDisplay->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                CsrUiEventSetReqSend(pDisplay->sk1EventHandle,
                                     CSR_UI_INPUTMODE_AUTO,
                                     CsrSchedTaskQueueGet());

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2("Phdc Mgr : Main Menu"),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        PHDC_APP_MGR_MAIN_MENU_ACTIVATE,
                                        PHDC_APP_MGR_MAIN_MENU_ACTIVATE,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Activate Service"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);
                CsrUiMenuAdditemReqSend(pDisplay->displayHandle,
                                        PHDC_APP_MGR_MAIN_MENU_PROXIMITY,
                                        PHDC_APP_MGR_MAIN_MENU_PROXIMITY,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Activate with Proximity Connect"),
                                        NULL,
                                        pDisplay->sk1EventHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
                pInstance->uiIndex = PHDC_APP_MGR_UI_DEFAULT_POP_UP;
            }
            
            break;
        }
        case PHDC_APP_MGR_UI_DEFAULT_POP_UP :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_DIALOG)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU;

            }
            break;
        }
        case PHDC_APP_MGR_UI_ACTIVE_DEVICE_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_HEADING_1_ACTIVE_DEVICES),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
                pInstance->uiIndex = PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU;

            }

            break;
        }
        case PHDC_APP_MGR_UI_DEVICE_CAPAB_MENU :
        {
            if (pCfm->elementType == CSR_UI_UIETYPE_MENU)
            {
                pDisplay->displayHandle = pCfm->handle;
                pDisplay->sk1EventHandle = pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].sk1EventHandle;
                pDisplay->sk1EventHandleFunc = PhdcAppMgrMainMenuSK1Handler;

                CsrUiMenuSetReqSend(pDisplay->displayHandle,
                                    CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_HEADING_1_GET_CAPAB),
                                    CONVERT_TEXT_STRING_2_UCS2("Select"),
                                    NULL);

                /* while extending the create UI chain,
                a) remember to fill pInstance->uiIndex.
                b) move this comment to the end of the chain */

            }

            break;
        }


                
        default :
        {
            /* for pop-up boxes per device end-point */
            if (pCfm->elementType == CSR_UI_UIETYPE_DIALOG &&
                pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_END_POINT)
            {
                pInstance->endPoint[pInstance->uiIndex].popUpHdl = pDisplay;
                pDisplay->displayHandle = pCfm->handle;

                pInstance->uiIndex++;

                if (pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_END_POINT)
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
                     pInstance->uiIndex < NUMBER_OF_CSR_BT_PHDC_END_POINT*2)
            {
                /* minus 2 ( 2 dialogs are alredy created) to access the right
                end-point entry*/
                pInstance->endPoint[pInstance->uiIndex-2].subMenuHdl = pDisplay;
                pDisplay->displayHandle = pCfm->handle;

                pInstance->uiIndex++;
                /* uiIndex increments and becomes equal to PHDC_APP_MGR_UI_MAIN_MENU */

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_MENU);
            }
            
            break;
        }
    }
}


static PhdcAppMgrDisplayHandle *PhdcMgrFindDisplayHandle(PhdcAppMgrInstance * pInstance)
{
    CsrUiEventInd *pInd = (CsrUiEventInd *)pInstance->recvMsgP;
    PhdcAppMgrDisplayHandle *pHandle=NULL;
    CsrUint8 i = 0;
    while ( i < NUMBER_OF_CSR_BT_PHDC_APP_UI)
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

void CsrBtPhdcAppMgrEventIndHandler(PhdcAppMgrInstance * pInstance)
{
    CsrUiEventInd *pInd = (CsrUiEventInd *)pInstance->recvMsgP;

    PhdcAppMgrDisplayHandle *pHandle = PhdcMgrFindDisplayHandle(pInstance);

    if (pHandle != NULL)
    {
        if (pHandle->sk1EventHandle == pInd->event)
        {
            (*pHandle->sk1EventHandleFunc)(pInstance, pHandle->displayHandle,pHandle->sk1EventHandle, pInd->key);
        }
    }
}


static const CsrBtPhdcEventHandler csrUiPhdcMsgHandlers[CSR_UI_PRIM_UPSTREAM_COUNT] =
{ /* Jump tabel to handle Upstream CSR_UI messages send from the Phone Emulator UI (CSR_UI)     */
    CsrBtPhdcAppMgrCreateCfmHandler,        /* CSR_UI_UIE_CREATE_CFM                  */
    NULL,                                   /* CSR_UI_MENU_GET_CFM                    */
    NULL,                                   /* CSR_UI_MENU_GETITEM_CFM                */
    NULL,                                   /* CSR_UI_MENU_GETCURSOR_CFM              */
    NULL,                                   /* CSR_UI_EVENT_GET_CFM                   */
    NULL,                                   /* CSR_UI_DIALOG_GET_CFM                  */
    NULL,                                   /* CSR_UI_INPUTDIALOG_GET_CFM             */
    NULL,                                   /* CSR_UI_IDLESCREEN_GET_CFM              */
    NULL,                                   /* CSR_UI_DISPLAY_GETHANDLE_CFM           */
    NULL,                                   /* CSR_UI_KEYDOWN_IND                     */
    CsrBtPhdcAppMgrEventIndHandler,         /* CSR_UI_EVENT_IND                       */
};

static void csrBtPhdcAppMgrInitEndpoint(PhdcAppMgrEndPoint *endpoint)
{   
    endpoint->bd.nap = 0;
    endpoint->bd.uap = 0;
    endpoint->bd.lap = 0;
    endpoint->prevBd.nap = 0;
    endpoint->prevBd.uap = 0;
    endpoint->prevBd.lap = 0;
    endpoint->dataChannelId = 0;

    endpoint->state = PHDC_APP_MGR_EP_ST_DISCONNECTED;
    endpoint->busy = FALSE;

    endpoint->maxPktLen = 224;
    CsrMemCpy(endpoint->pSystemId,"12345678",8);

    endpoint->popUpHdl = NULL;
    endpoint->subMenuHdl = NULL;
    endpoint->obsScanIdx = 0;
    CsrMemSet(endpoint->obsScan, 0, sizeof(endpoint->obsScan));
}

static void CsrBtPhdcAppMgrIntialiseInstance(PhdcAppMgrInstance *pInstance)
{
    CsrCharString bpm[] = "BPM", ws[] = "WS";
    CsrCharString service[] = "Health";
    CsrCharString provider[] = "C S R";

    pInstance->pService = CsrPmemZalloc(CsrStrLen(service)+1);
    CsrStrCpy(pInstance->pService,service);

    pInstance->pProvider = CsrPmemZalloc(CsrStrLen(provider)+1);
    CsrStrCpy(pInstance->pProvider,provider);

    pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepId = PHDC_APP_END_POINT_BPM;
    pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDataType = CSR_BT_HDP_DEVICE_TYPE_BP;
    pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDesc = CsrPmemZalloc(CsrStrLen(bpm)+ 1);
    CsrUtf8StrCpy(pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDesc,(CsrUtf8String *) bpm);
    csrBtPhdcAppMgrInitEndpoint(&pInstance->endPoint[PHDC_APP_END_POINT_BPM]);

    pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepId = PHDC_APP_END_POINT_WS;
    pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDataType = CSR_BT_HDP_DEVICE_TYPE_SCALE;
    pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDesc = CsrPmemZalloc(CsrStrLen(ws) + 1);
    CsrUtf8StrCpy(pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDesc,(CsrUtf8String *) ws);
    csrBtPhdcAppMgrInitEndpoint(&pInstance->endPoint[PHDC_APP_END_POINT_WS]);

    pInstance->pScratchString = CsrPmemZalloc(250);

    pInstance->selectedEndpoint = CSR_BT_PHDC_MGR_INVALID_VAL;
    pInstance->proximityInUse = FALSE;
    pInstance->timerId = 0;

}
/**************************************************************************************************
* CsrBtAppInit
**************************************************************************************************/
void CsrBtPhdcMgrAppInit(void **gash)
{
    PhdcAppMgrInstance *pInstance;
       #ifndef _WIN32_WCE
#ifdef _WIN32
    /* Windows is sloppy, so boost ourselves */
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif
#endif
    *gash       = (void *) CsrPmemZalloc(sizeof(PhdcAppMgrInstance));
    pInstance    = (PhdcAppMgrInstance*)*gash;

    CsrBtPhdcAppMgrIntialiseInstance(pInstance);

    CsrAppRegisterReqSend(CsrSchedTaskQueueGet(), TECH_BT, PROFILE_PHDC_MGR);
}

/**************************************************************************************************
* CsrBtAppDeinit
**************************************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtPhdcMgrAppDeinit(void **gash)
{
    PhdcAppMgrInstance *pInstance;

    pInstance    = (PhdcAppMgrInstance*)*gash;

    CsrPmemFree(pInstance->pScratchString);
    /*CsrPmemFree(pInstance->pService);
    CsrPmemFree(pInstance->pProvider);
    CsrPmemFree(pInstance->endPoint[PHDC_APP_END_POINT_BPM].mdepDesc);
    CsrPmemFree(pInstance->endPoint[PHDC_APP_END_POINT_WS].mdepDesc);*/
    CsrPmemFree(pInstance);

}
#else
void CsrBtPhdcMgrAppDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif



/**************************************************************************************************
* CsrBtAppHandler
**************************************************************************************************/
void CsrBtPhdcMgrAppHandler(void **gash)
{
    PhdcAppMgrInstance *pInstance;
    CsrPrim *prim;
    CsrUint16   evIndex;

    pInstance = (PhdcAppMgrInstance *) (*gash);
    CsrSchedMessageGet(&pInstance->eventType, &pInstance->recvMsgP);

    prim = pInstance->recvMsgP;

    switch (pInstance->eventType)
    {
        case CSR_APP_PRIM :
        {
            if (*prim == CSR_APP_TAKE_CONTROL_IND)
            {
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_MAIN_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
            }
            else if (*prim == CSR_APP_REGISTER_CFM)
            {
                CsrAppRegisterCfm *pAppRegCfm = (CsrAppRegisterCfm *) pInstance->recvMsgP;

                pInstance->controlId = pAppRegCfm->controlId;
                pInstance->uiIndex  = 0;

                CsrUiUieCreateReqSend(CsrSchedTaskQueueGet(), CSR_UI_UIETYPE_DIALOG);
            }
            else
            {
                CsrGeneralException("CSR_APP", 0, *prim, "Did not handle CSR_APP_PRIM Upstream message");
            }
            break;
        }

        case CSR_UI_PRIM:
        {
            evIndex  = (CsrUint16)(*prim - CSR_UI_PRIM_UPSTREAM_LOWEST);

            if ( ( evIndex <  CSR_UI_PRIM_UPSTREAM_COUNT  )&&
                 ( csrUiPhdcMsgHandlers[evIndex] != NULL  ) )
            {
                csrUiPhdcMsgHandlers[evIndex](pInstance);
            }
            break;

        }
        case CSR_BT_PHDC_MGR_PRIM :
        {
            CsrBtPhdcAppMgrIfPhdcHandler(pInstance);
            break;
        }
        case CSR_BT_GAP_APP_PRIM :
        {
            if (*prim == CSR_BT_GAP_APP_GET_SELECTED_DEVICE_CFM)
            {
                CsrBtGapAppGetSelectedDeviceCfm *pCfm = (CsrBtGapAppGetSelectedDeviceCfm *) pInstance->recvMsgP;
                CsrUint8 i;

                defGlobalBdAddr = pCfm->deviceAddr;

                if(CsrBtBdAddrEqZero(&defGlobalBdAddr))
                {
                    for ( i = 0; i < NUMBER_OF_CSR_BT_PHDC_END_POINT; i++ )
                    {
                        if ( !CsrBtBdAddrEqZero(&pInstance->endPoint[i].bd ) )
                        {
                            defGlobalBdAddr = pInstance->endPoint[i].bd;
                            break;
                        }
                    }
                }
                
                if(!CsrBtBdAddrEqZero(&defGlobalBdAddr))
                {
                    CsrBtPhdcMgrGetDeviceCapabReqSend(defGlobalBdAddr, 0xFF);
                    /* Show dialog get capab*/
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                            CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                            CONVERT_TEXT_STRING_2_UCS2("Device found get capab..."),
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
                else
                {
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                            CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                            CONVERT_TEXT_STRING_2_UCS2("No device address found"),
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

            }
            else if (*prim == CSR_BT_GAP_APP_PAIR_PROXIMITY_DEVICE_CFM)
            {
                CsrBtGapAppPairProximityDeviceCfm * pCfm = (CsrBtGapAppPairProximityDeviceCfm*) pInstance->recvMsgP;
                CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle);

                if (pCfm->resultCode == CSR_BT_GAP_APP_SUCCESS) 
                {
                    defGlobalBdAddr = pCfm->deviceAddr;

                    /* Get capab... */
                    CsrBtPhdcMgrGetDeviceCapabReqSend(defGlobalBdAddr, 0xFF);

                    /* Show dialog connecting device*/
                    CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_MGR_UI_DEFAULT_POP_UP].displayHandle,
                                            CONVERT_TEXT_STRING_2_UCS2(PHDC_APP_MGR_UI_HEADING_1),
                                            CONVERT_TEXT_STRING_2_UCS2("Device found get capab..."),
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
            }
        }
        default :
        {

            break;
        }
    }
    CsrPmemFree(pInstance->recvMsgP);
}


