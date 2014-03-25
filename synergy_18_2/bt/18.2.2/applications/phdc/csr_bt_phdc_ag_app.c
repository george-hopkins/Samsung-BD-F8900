/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>

#include "csr_bt_phdc_ag_app.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "csr_bt_phdc_ag_prim.h"
#include "csr_bt_phdc_ag_lib.h"
#include "csr_ui_lib.h"
#include "csr_bt_ui_strings.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_sched.h"
#include "csr_ui_prim.h"
#include "csr_bt_util.h"
#include "csr_bt_cm_lib.h"
#include "csr_formatted_io.h"
#include "csr_exceptionhandler.h"


/**************************************************************************************************
* CsrBtAppHandler
**************************************************************************************************/
void CsrBtPhdcAgAppIfPhdcHandler(PhdcAppAgInstance *pInstance)
{

    CsrPrim *prim;

    prim = pInstance->recvMsgP;

    switch (*prim)
    {
        case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND:
        {
            CsrBtPhdcAgConfigureEndpointResSend(pInstance->endPoint[pInstance->endpointActivated].mdepDataType,
                                                CSR_BT_HDP_MDEP_SOURCE,
                                                (CsrUtf8String*)CsrStrDup(pInstance->endPoint[pInstance->endpointActivated].mdepDesc),
                                                FALSE);
        
            break;
        }

        case CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM:
        {
            CsrBtPhdcAgConfigureEndpointCfm *pCfm = (CsrBtPhdcAgConfigureEndpointCfm *)pInstance->recvMsgP;

            if (pCfm->resultCode ==  CSR_BT_RESULT_CODE_HDP_SUCCESS)
            {
                CsrBtPhdcAgAbsoluteTime time;
                CsrUint8 systemId[8]= {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
                time.century = 0x20;
                time.year = 0x11;
                time.month = 0x02;
                time.day = 0x14;
                time.hour = 0x12;
                time.minute = 0x48;
                time.second = 0x32;
                time.secondFractions = 0x00;

                if(pInstance->endpointActivated == PHDC_APP_AG_END_POINT_WS)
                {
                    CsrBtPhdcAgConfigureMdsReqSend(0x01,CsrStrDup("CSR BT"),CsrStrDup("CSR PHDC WS AG 001"),systemId,
                        0x05DC, CsrStrDup("CSRAGWS123"), CsrStrDup("1.01"), time);

                    CsrBtPhdcAgDimConfigureReqSend(0x05DC,1);
                }


                if(pInstance->endpointActivated == PHDC_APP_AG_END_POINT_BPM)
                {
                    CsrBtPhdcAgConfigureMdsReqSend(0x02,CsrStrDup("CSR BT"),CsrStrDup("CSR PHDC BMP AG 001"),systemId,
                        0x02BC, CsrStrDup("CSRAGBMP123"), CsrStrDup("1.01"), time);

                    CsrBtPhdcAgDimConfigureReqSend(0x02BC,1);
                }

            }
            else
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Configuration"),
                    CONVERT_TEXT_STRING_2_UCS2("Failed"),
                    CONVERT_TEXT_STRING_2_UCS2("OK"),
                    NULL,
                    pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].sk1EventHandle,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION);
            }
            pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_ACTIVATE;


            pInstance->bRegistered = TRUE;

            break;
        }

        case CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM:
        {
            CsrBtPhdcAgDimConfigureCfm *pCfm = (CsrBtPhdcAgDimConfigureCfm *)pInstance->recvMsgP;

            if (pCfm->resultCode ==  CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS)
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Dim Config"),
                    CONVERT_TEXT_STRING_2_UCS2("Success"),
                    CONVERT_TEXT_STRING_2_UCS2("OK"),
                    NULL,
                    pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION);

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CsrSchedTaskQueueGet(),
                    CSR_UI_INPUTMODE_AUTO,
                    CSR_UI_LOWESTPRIORITY);


                /* activation is succesful so remove Activate from the main menu */
                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                           PHDC_APP_AG_MAIN_MENU_ACTIVATE_WS);

                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                           PHDC_APP_AG_MAIN_MENU_ACTIVATE_BPM);

                /*
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_CONFIGURE_DEVICE_MENU].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                */
                
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_MAIN_MENU;


            }
            else
            {

            }
            break;
        }

        case CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND:
        {
            CsrUint8 count;
            char data[100],data1[100];

            CsrBtPhdcAgGetDeviceCapabInd *pInd = (CsrBtPhdcAgGetDeviceCapabInd *)pInstance->recvMsgP;

            if(CsrBtBdAddrEq(&pInstance->deviceAddr, &pInd->deviceAddr))
            {            
                for(count = 0; count < pInd->supportedFeatureListLength; count++)
                {
                    if((pInd->supportedFeatureList[count].dataType == CSR_BT_HDP_DEVICE_TYPE_BP) ||
                        (pInd->supportedFeatureList[count].dataType == CSR_BT_HDP_DEVICE_TYPE_SCALE))
                    {
                        pInstance->deviceCapab[pInstance->numCapab].psmIdentifier = pInd->psmIdentifier;
                        pInstance->deviceCapab[pInstance->numCapab].mdepId = pInd->supportedFeatureList[count].id;
                        pInstance->deviceCapab[pInstance->numCapab].mdepDataType = pInd->supportedFeatureList[count].dataType;
                        pInstance->numCapab++;
                    }
                }

                for(count = 0; count < pInstance->numCapab; count++)
                {
                    CsrMemSet(data,0x00,100);
                    CsrMemSet(data1,0x00,100);
                    CsrSnprintf(data, 100, "PSM ID: %x, Mdep ID: %x",
                                pInstance->deviceCapab[count].psmIdentifier,
                                pInstance->deviceCapab[count].mdepId);

                    if(pInstance->deviceCapab[count].mdepDataType == CSR_BT_HDP_DEVICE_TYPE_BP)
                    {
                        CsrSnprintf(data1, 100, "Data Type: BPM");
                    }
                    else if(pInstance->deviceCapab[count].mdepDataType == CSR_BT_HDP_DEVICE_TYPE_SCALE)
                    {
                        CsrSnprintf(data1, 100, "Data Type: WS");
                    }
                    else
                    {
                        CsrSnprintf(data1, 100, "Data Type not supported");
                    }

                    CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle,
                                            count,
                                            pInstance->deviceCapab[count].mdepDataType,
                                            CSR_UI_ICON_NONE,
                                            CONVERT_TEXT_STRING_2_UCS2(data),
                                            CONVERT_TEXT_STRING_2_UCS2(data1),
                                            pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].sk1EventHandle,
                                            CSR_UI_DEFAULTACTION,
                                            CSR_UI_DEFAULTACTION,
                                            CSR_UI_DEFAULTACTION);
                }               
            }
            break;
        }
        case CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM:
        {
            CsrUiUieHideReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle);

            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEVICE_CAPAB_MENU].displayHandle,
                CsrSchedTaskQueueGet(),
                CSR_UI_INPUTMODE_AUTO,
                CSR_UI_LOWESTPRIORITY);

            break;
        }

        case CSR_BT_PHDC_AG_ASSOCIATE_CFM :
        {
            CsrBtPhdcAgAssociateCfm *pAssocCfm = (CsrBtPhdcAgAssociateCfm *)pInstance->recvMsgP;

            if((pAssocCfm->resultCode == CSR_BT_PHDC_AG_RESULT_CODE_CONFIG_ACCEPTED_CONFIG) ||
                (pAssocCfm->resultCode == CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS))
            {
                pInstance->dataChannelId = pAssocCfm->dataChannelId;
                
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Association"),
                                      CONVERT_TEXT_STRING_2_UCS2("SUCCES"),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_OPERATING;

                /* Add operations menu to main menu */
                CsrUiMenuAdditemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                        CSR_UI_FIRST,
                                        PHDC_APP_AG_MAIN_OPERATION,
                                        CSR_UI_ICON_NONE,
                                        CONVERT_TEXT_STRING_2_UCS2("Operation"),
                                        NULL,
                                        pInstance->displayHandlers[PHDC_APP_AG_UI_OPERATING_MENU].displayHandle,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION,
                                        CSR_UI_DEFAULTACTION);
                

            }
            else
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Association"),
                                      CONVERT_TEXT_STRING_2_UCS2("FAIL"),
                                      CONVERT_TEXT_STRING_2_UCS2("Try Next Config"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);

                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
            }

            break;
        }

        case CSR_BT_PHDC_AG_DATA_CFM :
        {
            CsrBtPhdcAgDataCfm *pDataCfm = (CsrBtPhdcAgDataCfm *)pInstance->recvMsgP;

            if(pDataCfm->resultCode == CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS)
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Operating"),
                                      CONVERT_TEXT_STRING_2_UCS2("Data Transfer Success"),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_OPERATING;
            }
            
            break;
        }

        case CSR_BT_PHDC_AG_DISASSOCIATE_CFM :
        {
            CsrBtPhdcAgDisassociateCfm *pDataCfm = (CsrBtPhdcAgDisassociateCfm *)pInstance->recvMsgP;

            if(pDataCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                      CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Disassociating"),
                                      CONVERT_TEXT_STRING_2_UCS2("Disassociation Success"),
                                      CONVERT_TEXT_STRING_2_UCS2("OK"),
                                      NULL,
                                      pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION,
                                      CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                    CsrSchedTaskQueueGet(),
                                    CSR_UI_INPUTMODE_AUTO,
                                    CSR_UI_LOWESTPRIORITY);
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_MAIN_MENU;

                /* Remove operations menu from main menu */
                CsrUiMenuRemoveitemReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_MAIN_MENU].displayHandle,
                                            PHDC_APP_AG_MAIN_OPERATION);

            }
            
            break;
        }

        case CSR_BT_PHDC_AG_SUSPEND_CFM :
        {
            CsrBtPhdcAgSuspendCfm *pDataCfm = (CsrBtPhdcAgSuspendCfm *)pInstance->recvMsgP;

            if(pDataCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Suspend"),
                    CONVERT_TEXT_STRING_2_UCS2("Suspend Success"),
                    CONVERT_TEXT_STRING_2_UCS2("OK"),
                    NULL,
                    pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CsrSchedTaskQueueGet(),
                    CSR_UI_INPUTMODE_AUTO,
                    CSR_UI_LOWESTPRIORITY);
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_RESUME_MENU;
            }
            else
            {

            }

            break;
        }


        case CSR_BT_PHDC_AG_RESUME_CFM :
        {
            CsrBtPhdcAgResumeCfm *pDataCfm = (CsrBtPhdcAgResumeCfm *)pInstance->recvMsgP;

            if(pDataCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
            {
                CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CONVERT_TEXT_STRING_2_UCS2("Phdc Ag : Resume"),
                    CONVERT_TEXT_STRING_2_UCS2("Resume Success"),
                    CONVERT_TEXT_STRING_2_UCS2("OK"),
                    NULL,
                    pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION,
                    CSR_UI_DEFAULTACTION);
                CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                    CsrSchedTaskQueueGet(),
                    CSR_UI_INPUTMODE_AUTO,
                    CSR_UI_LOWESTPRIORITY);
                pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_OPERATING;
            }
            else
            {

            }

            break;
        }

        case CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND :
        {
            CsrBtPhdcAgAcceptBtLinkInd *pInd = (CsrBtPhdcAgAcceptBtLinkInd *)pInstance->recvMsgP;
                   #ifndef _WIN32_WCE
            sprintf(pInstance->pScratchString,
                    "Accept connection from remote device %04X-%02X-%06X ?",
                    pInd->deviceAddr.nap,
                    pInd->deviceAddr.uap,
                    pInd->deviceAddr.lap);
            #endif
            pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_ACCEPT_BT_LINK;
            
            CsrBtBdAddrCopy(&pInstance->deviceAddr, &pInd->deviceAddr);

            CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CONVERT_TEXT_STRING_2_UCS2("Accept Connection"),
                                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                CONVERT_TEXT_STRING_2_UCS2("Accept"),
                                CONVERT_TEXT_STRING_2_UCS2("Reject"),
                                pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION);

            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);

            break;
        }
        case CSR_BT_PHDC_AG_SETUP_BT_LINK_IND:
        {
            CsrBtPhdcAgSetupBtLinkInd *pInd = (CsrBtPhdcAgSetupBtLinkInd *)pInstance->recvMsgP;
            
            pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_SETUP_BT_LINK;
                  #ifndef _WIN32_WCE         
            if(pInd->resultCode == CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS)
            {           
                sprintf(pInstance->pScratchString,
                        "Connection successful, remote device %04X-%02X-%06X",
                        pInd->deviceAddr.nap,
                        pInd->deviceAddr.uap,
                        pInd->deviceAddr.lap);                          
            }
            else
            {
                sprintf(pInstance->pScratchString,
                        "Connection failed, remote device %04X-%02X-%06X",
                        pInd->deviceAddr.nap,
                        pInd->deviceAddr.uap,
                        pInd->deviceAddr.lap); 
            }
                #endif
            CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CONVERT_TEXT_STRING_2_UCS2("BT Connection"),
                                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                CONVERT_TEXT_STRING_2_UCS2("OK"),
                                NULL,
                                pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION);

            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);
            break;
        }
        case CSR_BT_PHDC_AG_BT_LINK_REL_IND:
        {
            CsrBtPhdcAgBtLinkRelInd *pInd = (CsrBtPhdcAgBtLinkRelInd *)pInstance->recvMsgP;
                  #ifndef _WIN32_WCE         
            sprintf(pInstance->pScratchString,
                    "Disconnected from remote device %04X-%02X-%06X",
                    pInd->deviceAddr.nap,
                    pInd->deviceAddr.uap,
                    pInd->deviceAddr.lap);
     #endif
            pInstance->popUpInput = PHDC_APP_AG_DEFAULT_POP_UP_BT_LINK_REL;
            
            CsrUiDialogSetReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CONVERT_TEXT_STRING_2_UCS2("BT Connection"),
                                CONVERT_TEXT_STRING_2_UCS2(pInstance->pScratchString),
                                CONVERT_TEXT_STRING_2_UCS2("OK"),
                                NULL,
                                pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].sk1EventHandle,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION,
                                CSR_UI_DEFAULTACTION);

            CsrUiUieShowReqSend(pInstance->displayHandlers[PHDC_APP_AG_UI_DEFAULT_POP_UP].displayHandle,
                                CsrSchedTaskQueueGet(),
                                CSR_UI_INPUTMODE_AUTO,
                                CSR_UI_LOWESTPRIORITY);


            break;
        }
        
        default :
        {
            break;
        }
    }
}



