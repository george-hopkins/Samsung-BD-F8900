/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #4 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_formatted_io.h"

#include "csr_bt_util.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_avrcp.h"
#include "csr_bt_avrcp_app_util.h"
#include "csr_bt_avrcp_app_ui_sef.h"

#include "csr_app_lib.h"
#include "csr_ui_lib.h"





/* Set this to send actual AV commands on play/stop instead
 * of just halting the filter chains */
/* #define SEND_AV_CMD */

#define ATT_TITLE                     "Track03"
#define ATT_TITLE_SIZE                7
#define ATT_ARTIST                    "Unknown"
#define ATT_ARTIST_SIZE               7
#define ATT_ALBUM                     "Unknown"
#define ATT_ALBUM_SIZE                7
#define ATT_MEDIA_NUMBER              "03"
#define ATT_TOTAL_NUMBER              "10"
#define ATT_NUMBER_SIZE               2
#define ATT_GENRE                     "Pop-Rock"
#define ATT_GENRE_SIZE                8
#define ATT_TIME                      "180000"  /* 180000 msec = 3 minutes */
#define ATT_TIME_SIZE                 6

#define ATT_ID_LENGTH                  4
#define ATT_VAL_LENGTH                 2
#define ATT_MIN_LENGTH                 (ATT_ID_LENGTH + ATT_VAL_LENGTH + CSR_BT_AVRCP_CHARACTER_SET_SIZE) /* 8 bytes in all */
#define ATT_LONG_TITLE                "CSR Synergy BT AVRCP fragmentation test"
#define NUM_OF_TITLE_REPEATS           20


#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
static CsrCharString *displayString_CT = "\0";
#endif
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
static CsrCharString *displayString_TG = "\0";
#endif
/************************************ Constant definitions *******************************************************************************/
const AvrcpMediaListData_t mediaData[NUM_MEDIA_ITEMS * 2] =
{
    {"Jingle Bells",{1,2,3,4,5,6,7,8}},
    {"Once upon a time",{0,3,6,9,12,15,18,21}},
    {"The alphabet song",{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}},
    {"Angel song",{8,2,3,4,5,6,7,1}},
    {"Once bitten twice shy",{21,3,6,9,12,15,18,0}},
    {"The number song",{43,19,23,29,31,37,41,17}}

};

const AvrcpFolderListData_t emptyFolder = { "empty", { 1, 1, 2, 2, 3, 3, 4, 4 }};

const AvrcpFolderListData_t folderData[NUM_FOLDERS] =
{
    {"songlists",{0,1,2,3,4,5,6,7}},
    {"SECOND folder",{0,4,8,12,16,20,24,28}},
    {"Last (3rd) folder",{17,19,23,29,31,37,41,43}},
};


const AvrcpAttributesData_t attributesTimeData[NUM_ATTRIBUTES] =
{
    {"201000", CSR_BT_AVRCP_ITEM_ATT_MASK_TIME, CSR_BT_AVRCP_ITEM_ATT_TIME},
    {"021000", CSR_BT_AVRCP_ITEM_ATT_MASK_TIME, CSR_BT_AVRCP_ITEM_ATT_TIME},
    {"102000", CSR_BT_AVRCP_ITEM_ATT_MASK_TIME, CSR_BT_AVRCP_ITEM_ATT_TIME},
};

const AvrcpAttributesData_t attributesGenreData[NUM_ATTRIBUTES] =
{
    {"Christmas", CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE, CSR_BT_AVRCP_ITEM_ATT_GENRE},
    {"Ska-Pop-Indie-Blues-Funk-Country-Fusion-Rock-Jazz", CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE, CSR_BT_AVRCP_ITEM_ATT_GENRE},
    {"Children", CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE, CSR_BT_AVRCP_ITEM_ATT_GENRE},
};

const AvrcpAttributesData_t attributesTitleData[NUM_ATTRIBUTES * 2] =
{
    {"The perfect Christmas song", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
    {"The perfect song", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
    {"ABC", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
    {"Christmas song II", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
    {"This is a perfect song", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
    {"123", CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, CSR_BT_AVRCP_ITEM_ATT_TITLE},
};

const AvrcpAttributesData_t attributesArtistData[NUM_ATTRIBUTES] =
{
    {"Elvis", CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST, CSR_BT_AVRCP_ITEM_ATT_ARTIST},
    {"Cliff Richard", CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST, CSR_BT_AVRCP_ITEM_ATT_ARTIST},
    {"The Kids", CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST, CSR_BT_AVRCP_ITEM_ATT_ARTIST},
};

/********************************************************************************************************************************************/
static void av2HandleAvrcpCmnPrim(avrcpinstance_t * instData, void *msg)
{      
#ifndef _WIN32_WCE
    switch (*(CsrBtAvrcpPrim *)msg)
    {
        case CSR_BT_AVRCP_CONFIG_CFM:
        {
            CsrCharString displayString[200];
            CsrCharString temp[100];
            CsrBtAvrcpConfigCfm *prim = (CsrBtAvrcpConfigCfm *)msg;


      #ifndef _WIN32_WCE
            sprintf(displayString, "CSR_BT_AVRCP_CONFIG_CFM (res: 0x%02X)\n", prim->resultCode);
#endif
            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                  #ifndef _WIN32_WCE
                sprintf(temp, "AVRCP succesfully configured! Accepting incoming connections...\n");
                  #endif
                CsrStrCat(displayString, temp);
                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

                if ((instData->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_CT) ||
                    (instData->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_ONLY))
                {/* This is a target - media players should be registered */
                    CsrUint16 pasLen = 0;
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    CsrUint16 pasLen2 = 0;
                    CsrUint8 *pas2 = NULL;
                    CsrBtAvrcpMpFeatureMask mpFeatures2;
#endif
                    CsrUint8 *pas = NULL;
                    CsrBtAvrcpMpFeatureMask mpFeatures;
                    /* All opcodes allowed */
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    mpFeatures[0] = mpFeatures2[0] = 0xFFFFFFFF;
                    mpFeatures[1] = mpFeatures2[1] = 0xFFFFFFFF;
                    mpFeatures[2] = mpFeatures2[2] = 7;
                    mpFeatures[3] = mpFeatures2[3] = 0;
#else
                    mpFeatures[0] = 0xFFFFFFFF;
                    mpFeatures[1] = 0xFFFFFFFF;
                    mpFeatures[2] = 7;
                    mpFeatures[3] = 0;
#endif

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, "Equalizer123456789098765678");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON, "On");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, "Equalizer123456789098765678");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, "Equalizer123456789098765678Equalizer1Equalizer1Equalizer122");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, "Shuffle");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_OFF, "Off");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_ALL, "All");

                    /* Custom example of equalizer presets */
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, "Equalizer presets");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x01, "Standard");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x02, "Rock");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x03, "Pop");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x04, "Classic");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x05, "Jazz");
#endif

                    CsrBtAvrcpTgMpRegisterReqSend(instData->CsrSchedQid,                             /* Player handle */
                                             CSR_BT_AVRCP_NOTI_FLAG_ALL,                 /* Notifications */
                                             CSR_BT_AVRCP_TG_MP_REGISTER_CONFIG_SET_DEFAULT,   /* Other configuration */
                                             pasLen,
                                             pas,
                                             CSR_BT_AVRCP_MP_TYPE_MAJOR_AUDIO,
                                             CSR_BT_AVRCP_MP_TYPE_SUB_NONE,
                                             mpFeatures,
                                             CsrUtf8StrDup("CSR Player"));

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, "Equalizer098765678123456789");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON, "On");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, "Equalizer123456789098765678");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, "Equalizer123456789098765678Equalizer2Equalizer2Equalizer222");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, "Off");

                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, "Shuffle");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_OFF, "Off");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_ALL, "All");

                    /* Custom example of equalizer presets */
                    CsrBtAvrcpTgLibPasAttribAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, "Equalizer presets");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x01, "Standard");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x02, "Rock");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x03, "Pop");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x04, "Classic");
                    CsrBtAvrcpTgLibPasValueAdd(&pasLen2, &pas2, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x05, "Jazz");
                    CsrBtAvrcpTgMpRegisterReqSend(instData->CsrSchedQid,                             /* Player handle */
                                             CSR_BT_AVRCP_NOTI_FLAG_ALL,                 /* Notifications */
                                             CSR_BT_AVRCP_TG_MP_REGISTER_CONFIG_NONE,   /* Other configuration */
                                             pasLen2,
                                             pas2,
                                             CSR_BT_AVRCP_MP_TYPE_MAJOR_AUDIO,
                                             CSR_BT_AVRCP_MP_TYPE_SUB_NONE,
                                             mpFeatures2,
                                             CsrUtf8StrDup("CSR Player - 2"));
#endif

                }
#endif
            CsrBtAvrcpActivateReqSend(2);
            }
            else
            {
                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                CONVERT_TEXT_STRING_2_UCS2("Configuration of AVRCP failed...\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }

        case CSR_BT_AVRCP_REMOTE_FEATURES_IND:
        {
            CsrCharString displayString[500];
            CsrCharString temp[100];

            /* Incomplete feature - just ignore */
            CsrBtAvrcpRemoteFeaturesInd *prim = (CsrBtAvrcpRemoteFeaturesInd *)msg;
      #ifndef _WIN32_WCE
            sprintf(displayString, "AVRCP connection established to remote device with the following parameters:\n");
            sprintf(temp, "Controller (%s)\n", prim->ctFeatures.roleSupported ? "supported" : "not supported");
            #endif
            CsrStrCat(displayString, temp);

            if (prim->ctFeatures.roleSupported)
            {
                CsrMemSet(temp, 0, 100);
                      #ifndef _WIN32_WCE
                sprintf(temp, "    Version: %04X, features: 0x%04X, service: %s, provider: %s\n", prim->ctFeatures.srAvrcpVersion,
                        prim->ctFeatures.srFeatures, prim->ctFeatures.serviceName, prim->ctFeatures.providerName);
                CsrStrCat(displayString, temp);
                #endif
                instData->remoteAvrcpVersion = prim->ctFeatures.srAvrcpVersion;
            }

            CsrMemSet(temp, 0, 100);
            sprintf(temp, "  Target (%s)\n", prim->tgFeatures.roleSupported ? "supported" : "not supported");
            CsrStrCat(displayString, temp);

            if (prim->tgFeatures.roleSupported)
            {
                CsrMemSet(temp, 0, 100);
                sprintf(temp, "    Versions: %04X, features: 0x%04X, service: %s, provider: %s\n", prim->tgFeatures.srAvrcpVersion,
                        prim->tgFeatures.srFeatures, prim->tgFeatures.serviceName, prim->tgFeatures.providerName);
                CsrStrCat(displayString, temp);
                instData->remoteAvrcpVersion = prim->tgFeatures.srAvrcpVersion;
            }

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrBtAvrcpUtilFreeRoleDetails(&prim->tgFeatures);
            CsrBtAvrcpUtilFreeRoleDetails(&prim->ctFeatures);
            break;
        }

        case CSR_BT_AVRCP_DISCONNECT_IND:
        {
            CsrBtAvrcpDisconnectInd * prim = (CsrBtAvrcpDisconnectInd *) msg;
            CsrUint8 index = getIndexFromAvrcpConnId(instData, prim->connectionId);

            if (index < MAX_CONNECTIONS)
            {
                instData->avrcpCon[index].state = DaAvStateAvrcpDisconnected;
                instData->avrcpCon[index].inUse = FALSE;
            }

            instData->inEmptyFolder = FALSE;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                CONVERT_TEXT_STRING_2_UCS2("AVRCP disconnected!\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_ACTIVATE_CFM:
        {
            CsrBtAvrcpActivateCfm * prim = (CsrBtAvrcpActivateCfm *) msg;

            if(prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(("AVRCP profile activated...\n")) , TEXT_OK_UCS2, NULL);

                if (instData->state == DaAvStateAppInit)
                {
                    DA_AV_STATE_CHANGE_APP(DaAvStateAppActive);
                }
            }
            else
            {
                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(("ERROR : AVRCP profile could not be activated\n")) , TEXT_OK_UCS2, NULL);
            }

                            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_DEACTIVATE_CFM:
        {
/*            startAVRCPActivate(instData);*/

            instData->state = DaAvStateAppInit;
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(("CSR_BT_AVRCP_DEACTIVATE_CFM received \n")) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CONNECT_CFM:
        {

            CsrBtAvrcpConnectCfm * prim;
            prim = (CsrBtAvrcpConnectCfm *) msg;

            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                instData->avrcpCon[instData->currentConnection].connectionId = prim->connectionId;
                instData->avrcpCon[instData->currentConnection].state = DaAvStateAvrcpConnected;
                instData->avrcpCon[instData->currentConnection].inUse = TRUE;
                instData->avrcpConnectPending = FALSE;

                instData->remoteAddr[instData->currentConnection] = prim->deviceAddr;

                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                                CONVERT_TEXT_STRING_2_UCS2("AVRCP connection established successfully\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);


                if(instData->avrcpConfig == DA_AV_CONFIG_AVRCP_CT_ONLY)
                {
                    CsrCharString displayString[100];
                    CsrBtAVRCPCsrUiType    * csrUiVar  = &(instData->csrUiVar);
                    CsrUieHandle prim_handle        = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].backEventHandle;
                    CsrUieHandle displayHandle      = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].displayHandle;
                    CsrUieHandle sk1Handle          = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].sk1EventHandle;

                    /* Reconfigure the filter menu */
                    CsrBtAvrcpMenuRemoveAllItems(instData, CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INFORM_DISPLAYABLE_CHR_SET_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Inform displayable"), CONVERT_TEXT_STRING_2_UCS2("characterset"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_WITH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get item attributes"), CONVERT_TEXT_STRING_2_UCS2("with wrong UID counter"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for"), CONVERT_TEXT_STRING_2_UCS2("notifications menu"),  csrUiVar->displayesHandlers[CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_STATUS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get play status"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_RETRIEVE_ALL_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Retrieve all PAS "), CONVERT_TEXT_STRING_2_UCS2("(attributes + values + all text) "), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_CURRENT_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get current PAS values"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    sprintf((char*)displayString, "(currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle Equalizer from CT "), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, " (currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_TG_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle Equalizer from TG"), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter"), CONVERT_TEXT_STRING_2_UCS2(" media players menu"), csrUiVar->displayesHandlers[CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get playlist"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter"), CONVERT_TEXT_STRING_2_UCS2(" pass through menu"),  csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter "), CONVERT_TEXT_STRING_2_UCS2(" unit-info subunit info menu"), csrUiVar->displayesHandlers[CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_UID_LAST_SEARCH_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes"), CONVERT_TEXT_STRING_2_UCS2("(UID from last search)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Now "), CONVERT_TEXT_STRING_2_UCS2("Playing menu"), csrUiVar->displayesHandlers[CSR_BT_NOW_PLAYING_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  File"), CONVERT_TEXT_STRING_2_UCS2(" System menu"), csrUiVar->displayesHandlers[CSR_BT_FILE_SYSTEM_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_SEND_SEARCH_CIMMAND_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send search "), CONVERT_TEXT_STRING_2_UCS2(" command; text: (tomorrow)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_LAST_UID_RECEIVED_IN_SEARCH_GFI_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get Item Attributes"), CONVERT_TEXT_STRING_2_UCS2(" of last UID received in search (GFI) command"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_ITEM_FROM_SEARCH_LAST_UID_RECEIVED_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play item "), CONVERT_TEXT_STRING_2_UCS2(" from search (last UID received)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, " Current value: %d\n",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change Volume (from CT)."), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_DOWN_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (down)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_UP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (up)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Change path (wrong UID)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu : Enter"), CONVERT_TEXT_STRING_2_UCS2("Test Menu"), csrUiVar->displayesHandlers[CSR_BT_TOGGLE_TEST_STATUS_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);
                }
                else if(instData->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_CT)
                {
                    CsrCharString displayString[100];
                    CsrBtAVRCPCsrUiType    * csrUiVar  = &(instData->csrUiVar);
                    CsrUieHandle prim_handle        = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].backEventHandle;
                    CsrUieHandle displayHandle      = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].displayHandle;
                    CsrUieHandle sk1Handle          = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].sk1EventHandle;

                    /* Reconfigure the filter menu */
                    CsrBtAvrcpMenuRemoveAllItems(instData, CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_UID_CNT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change UID counter (mpId = 0; UID cnt: 30)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_NOW_PLAYING_CHG_TO_CT_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate now playing change notification to CT (player Id 1)"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_TRACK_CHG_TO_CT_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate track change notification to CT (player Id 1)"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);
                    
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_REGISTER_ONE_OR_MORE_MEDIA_PLAYER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register one more media player"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_UNREGISTER_ONE_MEDIA_PLAYER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Unregister one media player"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Change Volume. Current value: %d",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Toggle the equalizer (currently: %s)\n",instData->avrcpCon[instData->currentConnection].equalizerOn ? "OFF" : "ON");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Toggle TG test status (currently: %s)\n",((instData->tgTest == TRUE) ? "ON" : "OFF"));
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INFORM_DISPLAYABLE_CHR_SET_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Inform displayable"), CONVERT_TEXT_STRING_2_UCS2("characterset"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_WITH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get item attributes"), CONVERT_TEXT_STRING_2_UCS2("with wrong UID counter"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for"), CONVERT_TEXT_STRING_2_UCS2("notifications menu"), csrUiVar->displayesHandlers[CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_STATUS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get play status"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_RETRIEVE_ALL_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Retrieve all PAS "), CONVERT_TEXT_STRING_2_UCS2("(attributes + values + all text) "), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_CURRENT_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get current PAS values"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "(currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle Equalizer from CT "), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_TG_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Toggle Equalizer from TG"), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter "), CONVERT_TEXT_STRING_2_UCS2("media players menu"), csrUiVar->displayesHandlers[CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get playlist"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter "), CONVERT_TEXT_STRING_2_UCS2("pass through menu"), csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter "), CONVERT_TEXT_STRING_2_UCS2("unit-info subunit info menu"), csrUiVar->displayesHandlers[CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_UID_LAST_SEARCH_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes "), CONVERT_TEXT_STRING_2_UCS2("(UID from last search)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Now "), CONVERT_TEXT_STRING_2_UCS2("Playing menu"), csrUiVar->displayesHandlers[CSR_BT_NOW_PLAYING_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  File "), CONVERT_TEXT_STRING_2_UCS2("System menu"), csrUiVar->displayesHandlers[CSR_BT_FILE_SYSTEM_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_SEND_SEARCH_CIMMAND_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send search "), CONVERT_TEXT_STRING_2_UCS2("command;text:(tomorrow)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_LAST_UID_RECEIVED_IN_SEARCH_GFI_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get Item Attributes "), CONVERT_TEXT_STRING_2_UCS2("of last UID received in search (GFI) command"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_ITEM_FROM_SEARCH_LAST_UID_RECEIVED_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play item from "), CONVERT_TEXT_STRING_2_UCS2("search (last UID received)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, " Current value: %d\n",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change Volume (from CT)."), CONVERT_TEXT_STRING_2_UCS2(displayString), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_DOWN_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (down)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_UP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (up)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Change path"), CONVERT_TEXT_STRING_2_UCS2(" (wrong UID)"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu : Enter"), CONVERT_TEXT_STRING_2_UCS2("Test Menu"), csrUiVar->displayesHandlers[CSR_BT_TOGGLE_TEST_STATUS_MENU_UI].displayHandle,
                                             CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                }

            }
            else
            {
                instData->avrcpCon[instData->currentConnection].state = DaAvStateAvrcpDisconnected;

                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("AVRCP connection failed!\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }

        case CSR_BT_AVRCP_CONNECT_IND:
        {
            CsrBtAvrcpConnectInd *prim = (CsrBtAvrcpConnectInd *) msg;
            CsrUint8 conId = connectionSlotLocateAvrcp(instData);

            if (conId < MAX_CONNECTIONS)
            {

                instData->avrcpCon[conId].connectionId  = prim->connectionId;
                instData->avrcpCon[conId].state         = DaAvStateAvrcpConnected;
                instData->avrcpCon[conId].inUse         = TRUE;

                if(instData->avrcpConfig == DA_AV_CONFIG_AVRCP_CT_ONLY)
                {
                    CsrCharString displayString[100];
                    CsrBtAVRCPCsrUiType    * csrUiVar  = &(instData->csrUiVar);
                    CsrUieHandle prim_handle        = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].backEventHandle;
                    CsrUieHandle displayHandle      = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].displayHandle;
                    CsrUieHandle sk1Handle          = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].sk1EventHandle;

                    /* Reconfigure the filter menu */
                    CsrBtAvrcpMenuRemoveAllItems(instData, CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INFORM_DISPLAYABLE_CHR_SET_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Inform displayable characterset"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_WITH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get item attributes with wrong UID counter"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for notifications menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_STATUS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get play status"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_RETRIEVE_ALL_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Retrieve all PAS (attributes + values + all text)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_CURRENT_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get current PAS values"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    sprintf((char*)displayString, "Toggle Equalizer from CT (currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Toggle Equalizer from TG (currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_TG_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter media players menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get playlist"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter pass through menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter unit-info subunit info menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_UID_LAST_SEARCH_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes (UID from last search)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Now Playing menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_NOW_PLAYING_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  File System menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_FILE_SYSTEM_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_SEND_SEARCH_CIMMAND_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send search command; text: (tomorrow)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_LAST_UID_RECEIVED_IN_SEARCH_GFI_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get Item Attributes of last UID received in search (GFI) command"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_ITEM_FROM_SEARCH_LAST_UID_RECEIVED_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play item from search (last UID received)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Change Volume (from CT). Current value: %d\n",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_DOWN_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (down)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_UP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (up)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Change path (wrong UID)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu : Enter"), CONVERT_TEXT_STRING_2_UCS2("Test Menu"), csrUiVar->displayesHandlers[CSR_BT_TOGGLE_TEST_STATUS_MENU_UI].displayHandle,
                                             CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                }
                else if(instData->avrcpConfig == DA_AV_CONFIG_AVRCP_TG_CT)
                {
                    CsrCharString displayString[100];
                    CsrBtAVRCPCsrUiType    * csrUiVar  = &(instData->csrUiVar);
                    CsrUieHandle prim_handle        = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].backEventHandle;
                    CsrUieHandle displayHandle      = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].displayHandle;
                    CsrUieHandle sk1Handle          = csrUiVar->displayesHandlers[CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI].sk1EventHandle;

                    /* Reconfigure the filter menu */
                    CsrBtAvrcpMenuRemoveAllItems(instData, CSR_BT_AVRCP_ESTABLISH_CONN_MEANU_UI);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_FIRST, CSR_BT_AVRCP_DISCONNECT_KEY,
                         CSR_UI_ICON_NONE, TEXT_AVRCP_DISOCONNECT_UCS2, NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_UID_CNT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change UID counter (mpId = 0; UID cnt: 30)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_NOW_PLAYING_CHG_TO_CT_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate now playing change notification to CT (player Id 1)"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SIMULATE_TRACK_CHG_TO_CT_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Simulate track change notification to CT (player Id 1)"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);
                    
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_REGISTER_ONE_OR_MORE_MEDIA_PLAYER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register one more media player"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_UNREGISTER_ONE_MEDIA_PLAYER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Unregister one media player"), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    sprintf((char*)displayString, "Change Volume. Current value: %d",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    sprintf((char*)displayString, "Toggle the equalizer (currently: %s)\n",instData->avrcpCon[instData->currentConnection].equalizerOn ? "OFF" : "ON");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    sprintf((char*)displayString, "Toggle TG test status (currently: %s)\n",((instData->tgTest == TRUE) ? "ON" : "OFF"));
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_TG_TEST_STATUS_KEY,
                                    CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                                    CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_SEND_SET_ADDRESSED_PLAYER_CMD_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send SetAddrssdPlayer CMD"), CONVERT_TEXT_STRING_2_UCS2("to send RegisterNTFNRject"), sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION)

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INFORM_DISPLAYABLE_CHR_SET_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Inform displayable characterset"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_WITH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get item attributes with wrong UID counter"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Register for notifications menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_REGISTER_FOR_NOTIFICATIONS_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_STATUS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get play status"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_RETRIEVE_ALL_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Retrieve all PAS (attributes + values + all text)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_CURRENT_PAS_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get current PAS values"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    sprintf((char*)displayString, "Toggle Equalizer from CT (currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Toggle Equalizer from TG (currently %s)",instData->avrcpCon[instData->currentConnection].equalizerOn ? "ON" : "OFF");
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_TOGGLE_EQUALIZER_FROM_TG_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);


                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter media players menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_MEDIA_PLAYER_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_PLAY_LIST_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get playlist"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter pass through menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_PASS_THROUGH_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Enter unit-info subunit info menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_ENTER_UNIT_INFO_SUBUNIT_INFO_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_GET_MEDIA_ATTRIBUTES_UID_LAST_SEARCH_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get media attributes (UID from last search)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  Now Playing menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_NOW_PLAYING_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu:  File System menu"), NULL, csrUiVar->displayesHandlers[CSR_BT_FILE_SYSTEM_MENU_UI].displayHandle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_SEND_SEARCH_CIMMAND_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Send search command; text: (tomorrow)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_GET_ITEM_ATTRBUTE_LAST_UID_RECEIVED_IN_SEARCH_GFI_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Get Item Attributes of last UID received in search (GFI) command"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_PLAY_ITEM_FROM_SEARCH_LAST_UID_RECEIVED_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Play item from search (last UID received)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrMemSet(displayString, 0, 100);
                    sprintf((char*)displayString, "Change Volume (from CT). Current value: %d\n",instData->avrcpCon[instData->currentConnection].currentVolume);
                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_VOLUME_FROM_CT_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(displayString), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_DOWN_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (down)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_UP_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Change path (up)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_CHANGE_PATH_WRONG_UID_KEY,
                         CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2(" Change path (wrong UID)"), NULL, sk1Handle,
                         CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                    CsrUiMenuAdditemReqSend(displayHandle, CSR_UI_LAST, CSR_BT_AVRCP_INVALID_KEY,
                                             CSR_UI_ICON_NONE, CONVERT_TEXT_STRING_2_UCS2("Submenu : Enter"), CONVERT_TEXT_STRING_2_UCS2("Test Menu"), csrUiVar->displayesHandlers[CSR_BT_TOGGLE_TEST_STATUS_MENU_UI].displayHandle,
                                             CSR_UI_DEFAULTACTION, prim_handle, CSR_UI_DEFAULTACTION);

                }

            }
            else
            {
                CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("No more connections allowed!\n") , TEXT_OK_UCS2, NULL);

                /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            }
            break;
        }

        default:
        {
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2("Unknown common AVRCP prim") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
    }
    #endif
}

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE

static void av2HandleAvrcpTgPrim(avrcpinstance_t *instData, void *msg)
{
      #ifndef _WIN32_WCE
    switch (*(CsrBtAvrcpPrim *)msg)
    {
        case CSR_BT_AVRCP_TG_MP_REGISTER_CFM:
        {
            CsrCharString displayString[100];
            CsrCharString temp[50];

            CsrBtAvrcpTgMpRegisterCfm *prim = (CsrBtAvrcpTgMpRegisterCfm *)msg;
            sprintf((char *)displayString, "Media player registered (%u)!\n", prim->playerId);
            instData->playerIdList[instData->registeredMpIdx] = prim->playerId;
            if (instData->registeredMpIdx < MAX_NUMBER_OF_MP -1)
            {
                instData->registeredMpIdx++;
            }
            sprintf((char *)temp, "Actual number of MPs registered: %d\n", instData->registeredMpIdx);
            CsrStrCat(displayString, temp);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM:
        {
            CsrCharString displayString[200];
            CsrCharString temp[100];
            CsrBtAvrcpTgMpUnregisterCfm *prim = (CsrBtAvrcpTgMpUnregisterCfm *)msg;

            instData->playerIdList[instData->registeredMpIdx] = 0xFF;
            sprintf((char *)displayString, "Actual number of MPs registered: %d\n", instData->registeredMpIdx);
            if (instData->registeredMpIdx > 0)
            {
                instData->registeredMpIdx--;
            }

            sprintf((char *)temp, "CSR_BT_AV_TG_MP_UNREGISTER_CFM received; player ID (%u)!\n", prim->playerId);
            CsrStrCat(displayString, temp);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
        case CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM:
        {
            CsrCharString displayString[100];
            CsrBtAvrcpTgSetAddressedPlayerCfm *prim = (CsrBtAvrcpTgSetAddressedPlayerCfm *)msg;

            sprintf((char *)displayString, "CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM; addressed player: %u\n", prim->playerId);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_TG_NOTI_IND:
        {
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            CsrBtAvrcpTgNotiInd *prim = (CsrBtAvrcpTgNotiInd *)msg;

            switch (prim->notiId)
            {
                case CSR_BT_AVRCP_NOTI_ID_PLAYBACK_STATUS:
                {
                    sprintf((char *)temp, "CSR_BT_AVRCP_NOTI_ID_PLAYBACK_STATUS\n");
                    CsrBtAvrcpTgNotiPlaybackStatusRes(prim->connectionId,
                                                 CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                 prim->msgId,
                                                 CSR_BT_AVRCP_PLAYBACK_STATUS_STOPPED);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_PLAYBACK_STATUS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_TRACK:
                {
                    CsrBtAvrcpUid uid;

                    if (instData->avrcpVersion < CSR_BT_AVRCP_CONFIG_SR_VERSION_14)
                    {
                        CsrMemSet(uid, 0x00, CSR_BT_AVRCP_UID_SIZE);
                    }
                    else
                    {

                        CsrMemCpy(&uid[0], &(instData->nowPlayingUid[0]), CSR_BT_AVRCP_UID_SIZE);
                    }

                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_TRACK\n");
                    CsrBtAvrcpTgNotiTrackRes(prim->connectionId,
                                              CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                              prim->msgId,
                                              uid);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_TRACK;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_TRACK_END:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_TRACK_END\n");
                    CsrBtAvrcpTgNotiTrackStartEndRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                FALSE);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_TRACK_END;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_TRACK_START:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_TRACK_START\n");
                    CsrBtAvrcpTgNotiTrackStartEndRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                TRUE);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_TRACK_START;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_PLAYBACK_POS\n");
                    CsrBtAvrcpTgNotiPlaybackPositionRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                0);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_PLAYBACK_POS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_BATT_STATUS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_BATT_STATUS\n");
                    CsrBtAvrcpTgNotiBatStatusRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                CSR_BT_AVRCP_BATTERY_STATUS_FULL_CHARGE);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_BATT_STATUS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_SYSTEM_STATUS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_SYSTEM_STATUS\n");
                    CsrBtAvrcpTgNotiSystemStatusRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                CSR_BT_AVRCP_SYSTEM_STATUS_POWER_ON);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_SYSTEM_STATUS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                case CSR_BT_AVRCP_NOTI_ID_PAS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_PAS\n");

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_PAS;
                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_NOW_PLAYING_CONTENT:
                {
                    sprintf(temp,"CSR_BT_AVRCP_NOTI_ID_NOW_PLAYING_CONTENT\n");
                    CsrBtAvrcpTgNotiNowPlayingRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_NOW_PLAYING_CONTENT;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_AVAILABLE_PLAYERS\n");

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_AVAILABLE_PLAYERS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_ADDRESSED_PLAYER\n");

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_ADDRESSED_PLAYER;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_UIDS:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_UIDS\n");
                    CsrBtAvrcpTgNotiUidsRes(prim->connectionId,
                                                CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE,
                                                prim->msgId,
                                                0x0042);
                    instData->lastUidCounter = 0x0042;

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_UIDS;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }
                case CSR_BT_AVRCP_NOTI_ID_VOLUME:
                {
                    sprintf(temp, "CSR_BT_AVRCP_NOTI_ID_VOLUME; sending response\n");
                    CsrBtAvrcpTgNotiVolumeRes(prim->connectionId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE, prim->msgId,
                        instData->avrcpCon[instData->currentConnection].currentVolume);

                    instData->avrcpNotiMask += CSR_BT_AVRCP_NOTI_FLAG_VOLUME;

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_TG);
                    if(*displayString_TG != '\0')
                    {
                        CsrPmemFree(displayString_TG);
                    }
                    CsrStrCat(displayString, temp);

                    displayString_TG = displayString;
                    displayString = NULL;

                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                        CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }

                default:
                {
                    sprintf(temp, "Unknown notification ID\n");
                    CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                        CONVERT_TEXT_STRING_2_UCS2(temp) , TEXT_OK_UCS2, NULL);

                    /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
                    CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
                    break;
                }
            }
            break;
        }
        case CSR_BT_AVRCP_TG_PLAY_IND:
        {
            CsrBtAvrcpTgPlayInd *prim = (CsrBtAvrcpTgPlayInd *)msg;
            CsrBool             uidOk = FALSE;
            CsrUint8            i;
            CsrCharString displayString[100];
            CsrCharString temp[50];

            sprintf(displayString, "CSR_BT_AVRCP_TG_PLAY_IND received\n");



            for (i = 0; i < NUM_MEDIA_ITEMS*2; i++)
            {
                if (prim->uid[0] == mediaData[i].Uid[0] &&
                    prim->uid[1] == mediaData[i].Uid[1] &&
                    prim->uid[2] == mediaData[i].Uid[2] &&
                    prim->uid[3] == mediaData[i].Uid[3] &&
                    prim->uid[4] == mediaData[i].Uid[4] &&
                    prim->uid[5] == mediaData[i].Uid[5] &&
                    prim->uid[6] == mediaData[i].Uid[6] &&
                    prim->uid[7] == mediaData[i].Uid[7])
                {
                    uidOk = TRUE;
                    CsrMemCpy(&(instData->nowPlayingUid[0]), &(prim->uid[0]), CSR_BT_AVRCP_UID_SIZE);
                }
            }

            if (uidOk)
            {
                sprintf(temp, "Updated 'playstate'\n");
                instData->playstate = CSR_BT_AVRCP_PLAYBACK_STATUS_PLAYING;
                CsrBtAvrcpTgPlayResSend(prim->connectionId, prim->uid, prim->scope, prim->msgId, CSR_BT_RESULT_CODE_AVCTP_OPERATION_COMPLETE);
            }
            else
            {
                sprintf(temp, "Wrong UID received... rejecting\n");
                CsrBtAvrcpTgPlayResSend(prim->connectionId, prim->uid, prim->scope, prim->msgId, CSR_BT_RESULT_CODE_AVCTP_DOES_NOT_EXIST);
            }


            CsrStrCat(displayString, temp);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_TG_SEARCH_IND:
        {
            CsrBtAvrcpTgSearchInd *prim = (CsrBtAvrcpTgSearchInd *)msg;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("CSR_BT_AVRCP_TG_SEARCH_IND received; response sent\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrBtAvrcpTgSearchResSend(prim->connectionId, 22, NUM_MEDIA_ITEMS, prim->msgId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
            CsrPmemFree(prim->text);
            break;
        }

        case CSR_BT_AVRCP_TG_PAS_CURRENT_IND:
        {
            CsrBtAvrcpTgPasCurrentInd *prim = (CsrBtAvrcpTgPasCurrentInd *)msg;
            CsrBtAvrcpPasAttValPair *pas = CsrPmemAlloc(prim->attIdCount * sizeof(CsrBtAvrcpPasAttValPair));
            CsrUint8 i;
            CsrCharString displayString_[100];
            CsrCharString temp[50];
            CsrCharString *displayString = NULL;

            sprintf(displayString_, "CSR_BT_AVRCP_TG_PAS_CURRENT_IND (attribute IDs: ");

            for (i = 0; i < prim->attIdCount; i++)
            {
                CsrMemSet(temp, 0, 50);
                sprintf(temp, "0x%02X, \n", prim->attId[i]);
                pas[i].attribId = prim->attId[i];
                pas[i].valueId  = 0x01;
                CsrStrCat(displayString_, temp);
            }


            CsrBtAvrcpTgPasCurrentResSend(prim->connectionId,
                                     prim->msgId,
                                     prim->attIdCount,
                                     pas,
                                     CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(displayString_) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(displayString_) - 1);
            CsrStrCat(displayString, displayString_TG);
            if(*displayString_TG != '\0')
            {
                CsrPmemFree(displayString_TG);
            }
            CsrStrCat(displayString, displayString_);

            displayString_TG = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrPmemFree(prim->attId);
            break;
        }

        case CSR_BT_AVRCP_TG_CHANGE_PATH_IND:
        {
            CsrBtAvrcpTgChangePathInd *prim = (CsrBtAvrcpTgChangePathInd *)msg;
            CsrUint8    i = 0;
            CsrBool     uidOk = FALSE;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("CSR_BT_AVRCP_TG_CHANGE_PATH_IND received; send response\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            if (prim->folderDir == CSR_BT_AVRCP_CHANGE_PATH_UP)
            {/* According to 1.4 spec, if folder direction is "UP" then the folderUid is reserved and should be ignored */
                uidOk = TRUE;
            }
            else if (prim->folderUid[0] == emptyFolder.folderUid[0] &&
                     prim->folderUid[1] == emptyFolder.folderUid[1] &&
                     prim->folderUid[2] == emptyFolder.folderUid[2] &&
                     prim->folderUid[3] == emptyFolder.folderUid[3] &&
                     prim->folderUid[4] == emptyFolder.folderUid[4] &&
                     prim->folderUid[5] == emptyFolder.folderUid[5] &&
                     prim->folderUid[6] == emptyFolder.folderUid[6] &&
                     prim->folderUid[7] == emptyFolder.folderUid[7])
            { /* The empty folder */
                CsrBtAvrcpTgChangePathResSend(prim->connectionId, 0,
                                               prim->msgId,
                                               CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
                instData->inEmptyFolder = TRUE;
                return;
            }
            else
            {
                for (i = 0; ((i < NUM_FOLDERS) && (uidOk == FALSE)); i++)
                {
                    if (prim->folderUid[0] == folderData[i].folderUid[0] &&
                        prim->folderUid[1] == folderData[i].folderUid[1] &&
                        prim->folderUid[2] == folderData[i].folderUid[2] &&
                        prim->folderUid[3] == folderData[i].folderUid[3] &&
                        prim->folderUid[4] == folderData[i].folderUid[4] &&
                        prim->folderUid[5] == folderData[i].folderUid[5] &&
                        prim->folderUid[6] == folderData[i].folderUid[6] &&
                        prim->folderUid[7] == folderData[i].folderUid[7])
                    {
                        uidOk = TRUE;
                    }
                }
            }
            if (!uidOk)
            {
                CsrBtAvrcpTgChangePathResSend(prim->connectionId, NUM_MEDIA_ITEMS,
                                               prim->msgId,
                                               CSR_BT_AVRCP_STATUS_NOT_A_DIRECTORY);
            }
            else
            {
                CsrBtAvrcpTgChangePathResSend(prim->connectionId, NUM_MEDIA_ITEMS,
                                               prim->msgId,
                                               CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
                
                if (instData->itemAttribOffset == 1)
                {
                    instData->itemAttribOffset = 2;
                }
                else
                {
                    instData->itemAttribOffset = 1;
                }

                instData->inEmptyFolder = FALSE;
            }
            break;
        }
#endif /* CSR_BT_INSTALL_AVRCP_TG_13_14 */
        case CSR_BT_AVRCP_TG_PASS_THROUGH_IND:
        {
            CsrCharString displayString[500];
            CsrBtAvrcpTgPassThroughInd *prim = (CsrBtAvrcpTgPassThroughInd *)msg;
            sprintf(displayString, "Pass-through command received (0x%02X - %s)\n", prim->operationId, prim->state == CSR_BT_AVRCP_PT_STATE_PRESS ? "pressed" : "released");

            CsrBtAvrcpTgPassThroughResSend(prim->connectionId,
                                      prim->msgId,
                                      CSR_BT_AVRCP_PT_STATUS_ACCEPT);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
        case CSR_BT_AVRCP_TG_PAS_SET_IND:
        {
            CsrBtAvrcpTgPasSetInd *prim = (CsrBtAvrcpTgPasSetInd *)msg;
            CsrUint8 i;
            CsrCharString temp[200];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_TG_PAS_SET_IND received; value pair count: %d\n", prim->attValPairCount);
            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_TG);
            if(*displayString_TG != '\0')
            {
                CsrPmemFree(displayString_TG);
            }
            CsrStrCat(displayString, temp);

            displayString_TG = displayString;
            displayString = NULL;

            for (i= 0; i< prim->attValPairCount ; i++)
            {
                CsrMemSet(temp, 0, 200);
                sprintf(temp, "\tAttrib ID: %d; Value to set: %d\n", prim->attValPair[i].attribId, prim->attValPair[i].valueId);
                displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
                CsrStrCat(displayString, displayString_TG);
                if(*displayString_TG != '\0')
                {
                    CsrPmemFree(displayString_TG);
                }
                CsrStrCat(displayString, temp);

                displayString_TG = displayString;
                displayString = NULL;
            }

            CsrBtAvrcpTgPasSetResSend(prim->connectionId, prim->msgId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);


            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_TG_SET_VOLUME_IND:
        {
            CsrCharString displayString[100];
            CsrBtAvrcpTgSetVolumeInd *prim = (CsrBtAvrcpTgSetVolumeInd *)msg;

            sprintf(displayString, "CSR_BT_AVRCP_TG_SET_VOLUME_IND received; volume: %d\n",prim->volume);
            if (prim->volume <= 0x7F)
            {
                instData->avrcpCon[instData->currentConnection].currentVolume = prim->volume;
                CsrBtAvrcpTgSetVolumeResSend(prim->connectionId, prim->volume, prim->msgId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
                CsrBtAvrcpTgNotiVolumeReq(0,instData->avrcpCon[instData->currentConnection].currentVolume);
            }
            else
            {
                CsrBtAvrcpTgSetVolumeResSend(prim->connectionId, instData->avrcpCon[instData->currentConnection].currentVolume,
                                            prim->msgId,CSR_BT_AVRCP_STATUS_INTERNAL_ERROR);
            }

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND:
        {
            CsrUint8     *data = NULL;
            CsrUint16    total_len, index=0, media_index=0;
            CsrUint8     i;
            CsrBtAvrcpUid uid;
            CsrBtAvrcpTgGetFolderItemsInd *prim = (CsrBtAvrcpTgGetFolderItemsInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            total_len = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET;

            switch (prim->scope)
            {
                case CSR_BT_AVRCP_SCOPE_MP_LIST:
                {
                    for (i=1; i<=NUM_MEDIA_ITEMS; i++)
                    {
                        total_len += CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE + CsrStrLen((char*)mediaData[(i*instData->itemAttribOffset) -1].name) + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_COUNT_SIZE;
                    }
                    for (i=1; i<=NUM_MEDIA_ITEMS; i++)
                    {
                        CsrMemCpy(uid, mediaData[(i*instData->itemAttribOffset) -1].Uid, CSR_BT_AVRCP_UID_SIZE);
                        CsrBtAvrcpTgLibGfiMediaAdd(total_len, &index, &data, &media_index, &uid, CSR_BT_AVRCP_ITEM_FOLDER_TYPE_TITLES, CSR_BT_AVRCP_CHARACTER_SET_UTF_8, mediaData[i].name);
                        if (prim->scope == CSR_BT_AVRCP_SCOPE_SEARCH)
                        {
                            CsrMemCpy(instData->lastUidReceived,uid,sizeof(CsrBtAvrcpUid));
                        }
                    }

                    CsrBtAvrcpTgGetFolderItemsResSend(prim->connectionId, 3, instData->lastUidCounter, total_len, data, prim->msgId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
                    break;
                }
                case CSR_BT_AVRCP_SCOPE_SEARCH:
                case CSR_BT_AVRCP_SCOPE_NPL:
                case CSR_BT_AVRCP_SCOPE_MP_FS:
                {
                    if ((prim->startItem >= NUM_MEDIA_ITEMS) || (prim->startItem > prim->endItem))
                    {
                        CsrBtAvrcpTgGetFolderItemsResSend(prim->connectionId, NUM_FOLDERS, instData->lastUidCounter, 0, NULL, prim->msgId, CSR_BT_AVRCP_STATUS_RANGE_OOB);
                    }
                                        else if (instData->inEmptyFolder)
                    {
                        CsrBtAvrcpTgGetFolderItemsResSend(prim->connectionId, 0, instData->lastUidCounter, 0, NULL, prim->msgId, CSR_BT_AVRCP_STATUS_RANGE_OOB);
                                                break;
                    }
                    else
                    {
                        CsrUint8 numItems;
                        CsrUint8 k = 0, j = 0;
                        CsrBtAvrcpUid uid;

                        if (prim->endItem >= NUM_MEDIA_ITEMS)
                        {
                            numItems = (CsrUint8)(NUM_MEDIA_ITEMS - prim->startItem);
                        }
                        else
                        {
                            numItems = (CsrUint8)(1 + prim->endItem - prim->startItem);
                        }

                        /* Add length of first and second folder item */
                        if (prim->scope != CSR_BT_AVRCP_SCOPE_NPL)
                        {
                            if (numItems > (NUM_MEDIA_ITEMS + 2))
                            {
                                numItems = NUM_MEDIA_ITEMS + 2;
                            }

                                                    total_len += CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE + CsrStrLen(emptyFolder.name);
                            total_len += CSR_BT_AVRCP_LIB_GFI_FOLDER_PART_SIZE + CsrStrLen((char*)folderData[instData->itemAttribOffset-1].name);
                            i = j = k = 2;
                        }
                                                else
                        {
                            if (numItems > NUM_MEDIA_ITEMS)
                            {
                                numItems = NUM_MEDIA_ITEMS;
                            }

                            i = j = k = 0;
                        }
                        for (; i < numItems; i++) /* Start at i = 2 since first and second item is the folder item */
                        {
                            total_len += CSR_BT_AVRCP_LIB_GFI_MEDIA_PART_SIZE + CsrStrLen((char*)mediaData[(i*instData->itemAttribOffset) - k].name) + CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_COUNT_SIZE;
                            if (prim->attributeMask != CSR_BT_AVRCP_ITEM_ATT_MASK_NONE)
                            {
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE)
                                {
                                    total_len += (CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + CsrStrLen((char*)attributesTitleData[(i * instData->itemAttribOffset) - k].val));
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TIME)
                                {
                                    total_len += CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + CsrStrLen((char*)attributesTimeData[i - k].val);
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE)
                                {
                                    total_len += CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + CsrStrLen((char*)attributesGenreData[i - k].val);
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST)
                                {
                                    total_len += CSR_BT_AVRCP_LIB_GFI_MEDIA_ATT_PART_SIZE + CsrStrLen((char*)attributesArtistData[i - k].val);
                                }
                            }
                        }

                        if (prim->scope != CSR_BT_AVRCP_SCOPE_NPL)
                        {
                            /* Put in a folder item */
                            CsrMemCpy(uid, &emptyFolder.folderUid, CSR_BT_AVRCP_UID_SIZE);
                            CsrBtAvrcpTgLibGfiFolderAdd(total_len, &index, &data, &uid, CSR_BT_AVRCP_ITEM_FOLDER_TYPE_TITLES, CSR_BT_AVRCP_ITEM_FOLDER_PLAYABLE_NO, CSR_BT_AVRCP_CHARACTER_SET_UTF_8, emptyFolder.name);

                            CsrMemCpy(uid, &(folderData[instData->itemAttribOffset - 1].folderUid), CSR_BT_AVRCP_UID_SIZE);
                            CsrBtAvrcpTgLibGfiFolderAdd(total_len, &index, &data, &uid, CSR_BT_AVRCP_ITEM_FOLDER_TYPE_TITLES, CSR_BT_AVRCP_ITEM_FOLDER_PLAYABLE_NO, CSR_BT_AVRCP_CHARACTER_SET_UTF_8, folderData[0].name);
                        }

                        for (; j <= numItems; j++) /* Start at j = 2 since first and second item is the folder item */
                        {
                            CsrMemCpy(uid, mediaData[(j * instData->itemAttribOffset) - k].Uid, CSR_BT_AVRCP_UID_SIZE);
                            CsrBtAvrcpTgLibGfiMediaAdd(total_len, &index, &data, &media_index, &uid, CSR_BT_AVRCP_ITEM_FOLDER_TYPE_TITLES, CSR_BT_AVRCP_CHARACTER_SET_UTF_8, mediaData[j - k].name);
                            if (prim->attributeMask != CSR_BT_AVRCP_ITEM_ATT_MASK_NONE)
                            {
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE)
                                {
                                    CsrBtAvrcpTgLibGfiMediaAttributeAdd(total_len,
                                                                        &index,
                                                                        &data,
                                                                        media_index,
                                                                        CSR_BT_AVRCP_ITEM_ATT_TITLE,
                                                                        CSR_BT_AVRCP_CHARACTER_SET_UTF_8,
                                                                        (CsrUint16)CsrStrLen((char*)attributesTitleData[(j * instData->itemAttribOffset) - k].val),
                                                                        (CsrUint8 *)attributesTitleData[(j * instData->itemAttribOffset) - k].val);
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TIME)
                                {
                                    CsrBtAvrcpTgLibGfiMediaAttributeAdd(total_len,
                                                                        &index,
                                                                        &data,
                                                                        media_index,
                                                                        CSR_BT_AVRCP_ITEM_ATT_TIME,
                                                                        CSR_BT_AVRCP_CHARACTER_SET_UTF_8,
                                                                        (CsrUint16)CsrStrLen((char*)attributesTimeData[j - k].val),
                                                                        (CsrUint8 *)attributesTimeData[j - k].val);
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE)
                                {
                                    CsrBtAvrcpTgLibGfiMediaAttributeAdd(total_len,
                                                                        &index,
                                                                        &data,
                                                                        media_index,
                                                                        CSR_BT_AVRCP_ITEM_ATT_GENRE,
                                                                        CSR_BT_AVRCP_CHARACTER_SET_UTF_8,
                                                                        (CsrUint16)CsrStrLen((char*)attributesGenreData[j - k].val),
                                                                        (CsrUint8 *)attributesGenreData[j - k].val);
                                }
                                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST)
                                {
                                    CsrBtAvrcpTgLibGfiMediaAttributeAdd(total_len,
                                                                        &index,
                                                                        &data,
                                                                        media_index,
                                                                        CSR_BT_AVRCP_ITEM_ATT_ARTIST,
                                                                        CSR_BT_AVRCP_CHARACTER_SET_UTF_8,
                                                                        (CsrUint16)CsrStrLen((char*)attributesArtistData[j - k].val),
                                                                        (CsrUint8 *)attributesArtistData[j - k].val);
                                }
                            }
                        }
                        CsrBtAvrcpTgGetFolderItemsResSend(prim->connectionId, numItems, instData->lastUidCounter, total_len, data, prim->msgId, CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
                    }
                    break;
                }
                default:
                    break;
            }


            sprintf(temp, "CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND received; answer sent...\n");


            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_TG) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_TG);
            if(*displayString_TG != '\0')
            {
                CsrPmemFree(displayString_TG);
            }
            CsrStrCat(displayString, temp);

            displayString_TG = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----TG Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_TG) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_TG_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND:
        {
            CsrBtAvrcpTgGetPlayStatusInd *prim = (CsrBtAvrcpTgGetPlayStatusInd *)msg;

            CsrBtAvrcpTgGetPlayStatusResSend(prim->connectionId,
                                            0xFFFFFFFF, /* Meaning not supported */
                                            0xFFFFFFFF, /* Meaning not supported */
                                            instData->playstate,
                                            prim->msgId,
                                            CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND received; sending response\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND:
        {
            CsrBtAvrcpTgSetAddressedPlayerInd *prim = (CsrBtAvrcpTgSetAddressedPlayerInd *)msg;

            CsrBtAvrcpTgSetAddressedPlayerResSend(prim->connectionId,prim->playerId,10,prim->msgId,CSR_BT_RESULT_CODE_AVCTP_OPERATION_COMPLETE);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND received; response sent\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND:
        {
            CsrBtAvrcpTgSetBrowsedPlayerInd *prim = (CsrBtAvrcpTgSetBrowsedPlayerInd *)msg;

            CsrBtAvrcpTgSetBrowsedPlayerResSend(prim->connectionId,prim->playerId,10,3,1,14,(CsrUint8 *)CsrStrDup("/music/tophits"),
                                                prim->msgId,CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2("CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND received; response sent\n") , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND:
        {
            CsrBtAvrcpTgAddToNowPlayingInd *prim = (CsrBtAvrcpTgAddToNowPlayingInd *)msg;
            CsrBool  uidOk = FALSE;
            CsrUint8 i;

            CsrCharString displayString[100];
            CsrCharString temp[50];

            sprintf(displayString, "CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND received; sending response: ");

            for (i = 0; i < NUM_MEDIA_ITEMS; i++)
            {
                if (prim->uid[0] == mediaData[i].Uid[0] &&
                    prim->uid[1] == mediaData[i].Uid[1] &&
                    prim->uid[2] == mediaData[i].Uid[2] &&
                    prim->uid[3] == mediaData[i].Uid[3] &&
                    prim->uid[4] == mediaData[i].Uid[4] &&
                    prim->uid[5] == mediaData[i].Uid[5] &&
                    prim->uid[6] == mediaData[i].Uid[6] &&
                    prim->uid[7] == mediaData[i].Uid[7])
                {
                    uidOk = TRUE;
                }
            }

            if (uidOk)
            {
                sprintf(temp, "operation complete\n");
                CsrBtAvrcpTgAddToNowPlayingResSend(prim->connectionId,prim->msgId,CSR_BT_RESULT_CODE_AVCTP_OPERATION_COMPLETE);
            }
            else
            {
                sprintf(temp, "wrong UID received\n");
                CsrBtAvrcpTgAddToNowPlayingResSend(prim->connectionId,prim->msgId,CSR_BT_RESULT_CODE_AVCTP_DOES_NOT_EXIST);
            }

            CsrStrCat(displayString, temp);
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND:
        {
            CsrBtAvrcpTgInformDispCharsetInd *prim = (CsrBtAvrcpTgInformDispCharsetInd *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "Disp. char set indication received. Number of sets supported = 0x%02x \n", prim->charsetCount);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrPmemFree(prim->charset);
            /* No response required */
            break;
        }

        case CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND:
        {
            CsrBtAvrcpTgInformBatteryStatusInd *prim = (CsrBtAvrcpTgInformBatteryStatusInd *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "Inform battery status of CT: 0x%02x \n",prim->batStatus);
            /* No response required */

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_TG_NOTI_CFM:
        {
            CsrBtAvrcpTgNotiCfm  *prim = (CsrBtAvrcpTgNotiCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_TG_NOTI_CFM received; type = %d\tresult = 0x%04x (0x%04x)\n",
                prim->notiId, prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_TG_PAS_SET_CFM:
        {
            CsrBtAvrcpTgPasSetCfm *prim = (CsrBtAvrcpTgPasSetCfm *)msg;
            CsrCharString displayString[100];

            CsrSnprintf(displayString, 100, "CSR_BT_AVRCP_TG_PAS_SET_CFM received; type = %u\tresult = 0x%04x (0x%04x)\n",
                prim->playerId, prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
        case CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND:
        {
            CsrBtAvrcpTgGetAttributesInd *prim = (CsrBtAvrcpTgGetAttributesInd *)msg;
            CsrUint8    *pData = NULL;
            CsrUint16   pDataLen = 0;
            CsrUint16   dataIndex = 0;
            CsrUint8    attCnt = 0;
            CsrUint8    i = 0;
            CsrBool     uidOk = FALSE;
            CsrUint32   maskToReturn = 0;
            CsrBtAvrcpUid uidZero = {0,0,0,0,0,0,0,0};
            CsrCharString displayString[100];

            CsrBtAvrcpStatus cmdStatus = CSR_BT_AVRCP_STATUS_OPERATION_COMPLETE;

            for (i = 0; i < NUM_MEDIA_ITEMS; i++)
            {
                if (prim->uid[0] == mediaData[i].Uid[0] &&
                    prim->uid[1] == mediaData[i].Uid[1] &&
                    prim->uid[2] == mediaData[i].Uid[2] &&
                    prim->uid[3] == mediaData[i].Uid[3] &&
                    prim->uid[4] == mediaData[i].Uid[4] &&
                    prim->uid[5] == mediaData[i].Uid[5] &&
                    prim->uid[6] == mediaData[i].Uid[6] &&
                    prim->uid[7] == mediaData[i].Uid[7])
                {
                    uidOk = TRUE;
                }
            }
            if (!CsrMemCmp((CsrUint8*)prim->uid,(CsrUint8*)(&uidZero),CSR_BT_AVRCP_UID_SIZE)) 
            { /* Allow UID to be 0x0000000000000000 even for v1.4 devices; not just for 1.3 and lower */
                uidOk = TRUE;
            }


            CsrSnprintf(displayString, 100, "CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND received; attrib mask: %u\n",prim->attributeMask);

            if (!uidOk)
            {
                CsrStrCat(displayString, "Wrong UID in request....\n");
                cmdStatus = CSR_BT_AVRCP_STATUS_INVALID_PARAMETER;
            }
            else if (instData->lastUidCounter != prim->uidCounter)
            {
                CsrStrCat(displayString, "Wrong UID Counter in request....\n");
                cmdStatus = CSR_BT_AVRCP_STATUS_UID_CHANGED;
            }


            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);


            if (instData->tgTest)
            {
                CsrUint8    *pTitleTextData = NULL;
                CsrUint16   i = 0;
                pTitleTextData = CsrPmemAlloc((CsrStrLen((char*)ATT_LONG_TITLE) * NUM_OF_TITLE_REPEATS) + 1);
                pTitleTextData[0] = '\0';
                for (i = 0; i < NUM_OF_TITLE_REPEATS; i++)
                {
                    CsrStrCat((char*)pTitleTextData, ATT_LONG_TITLE);
                }
                pDataLen = CsrStrLen((char*)pTitleTextData) + ATT_MIN_LENGTH;
                pDataLen += 2 * sizeof(CsrUint8); /* allocate place for status and number of attributes too */
                pData = CsrPmemAlloc(pDataLen);
                dataIndex = 2 * sizeof(CsrUint8); /* Make room to fill in the status and number of attributes */

                CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE, &pData[dataIndex]); /* Attribute Id */
                dataIndex += 4;
                CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                dataIndex += 2;
                CSR_COPY_UINT16_TO_BIG_ENDIAN(CsrStrLen((char*)pTitleTextData), &pData[dataIndex]); /* Attrib value len */
                dataIndex += 2;
                CsrMemCpy(&pData[dataIndex], pTitleTextData, CsrStrLen((char*)pTitleTextData));
                CsrPmemFree(pTitleTextData);
                attCnt = 1;
            }
            else
            {
                pDataLen += 2 * sizeof(CsrUint8); /* allocate place for status and number of attributes too */
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE)
                {
                    pDataLen += ATT_TITLE_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST)
                {
                    pDataLen += ATT_ARTIST_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ALBUM)
                {
                    pDataLen += ATT_ALBUM_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_ALBUM;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_MEDIA_NUMBER)
                {
                    pDataLen += ATT_NUMBER_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_MEDIA_NUMBER;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TOTAL_NUMBER)
                {
                    pDataLen += ATT_NUMBER_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_TOTAL_NUMBER;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE)
                {
                    pDataLen += ATT_GENRE_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE;
                    attCnt++;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TIME)
                {
                    pDataLen += ATT_TIME_SIZE + ATT_MIN_LENGTH;
                    maskToReturn += CSR_BT_AVRCP_ITEM_ATT_MASK_TIME;
                    attCnt++;
                }
                if (attCnt == 0)
                {
                    cmdStatus = CSR_BT_AVRCP_STATUS_INVALID_PARAMETER;
                }

                pData = CsrPmemAlloc(pDataLen);
                dataIndex = 2 * sizeof(CsrUint8); /* Make room to fill in the status and number of attributes */

                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TITLE)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_TITLE, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_TITLE_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_TITLE, ATT_TITLE_SIZE);
                    dataIndex += ATT_TITLE_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ARTIST)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_ARTIST, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_ARTIST_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_ARTIST, ATT_ARTIST_SIZE);
                    dataIndex += ATT_ARTIST_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_ALBUM)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_ALBUM, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_ALBUM_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_ALBUM, ATT_ALBUM_SIZE);
                    dataIndex += ATT_ALBUM_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_MEDIA_NUMBER)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_MEDIA_NUMBER, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_NUMBER_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_MEDIA_NUMBER, ATT_NUMBER_SIZE);
                    dataIndex += ATT_NUMBER_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TOTAL_NUMBER)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_TOTAL_NUMBER, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_NUMBER_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_TOTAL_NUMBER, ATT_NUMBER_SIZE);
                    dataIndex += ATT_NUMBER_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_GENRE)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_GENRE, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_GENRE_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_GENRE, ATT_GENRE_SIZE);
                    dataIndex += ATT_GENRE_SIZE;
                }
                if (prim->attributeMask & CSR_BT_AVRCP_ITEM_ATT_MASK_TIME)
                {
                    CSR_COPY_UINT32_TO_BIG_ENDIAN(CSR_BT_AVRCP_ITEM_ATT_TIME, &pData[dataIndex]); /* Attribute Id */
                    dataIndex += 4;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(CSR_BT_AVRCP_CHARACTER_SET_UTF_8, &pData[dataIndex]); /* char set ID */
                    dataIndex += 2;
                    CSR_COPY_UINT16_TO_BIG_ENDIAN(ATT_TIME_SIZE, &pData[dataIndex]); /* Attrib value len */
                    dataIndex += 2;
                    CsrMemCpy(&pData[dataIndex], ATT_TIME, ATT_TIME_SIZE);
                    dataIndex += ATT_GENRE_SIZE;
                }
                if (attCnt == 0)
                {
                    cmdStatus = CSR_BT_AVRCP_STATUS_INVALID_PARAMETER;
                    break;
                }
            }
            CsrBtAvrcpTgGetAttributesResSend(prim->connectionId,attCnt,
                    pDataLen,pData,prim->msgId,cmdStatus);


            break;
        }
#endif /* CSR_BT_INSTALL_AVRCP_TG_13_14 */
        default:
        {
            CsrCharString displayString[100];

            sprintf(displayString, "Unknown target specific AVRCP prim: 0x%04X\n", *(CsrBtAvrcpPrim *)msg);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
    }
#endif
}
#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE */


#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE

static void av2HandleAvrcpCtPrim(avrcpinstance_t *instData, void *msg)
{
      #ifndef _WIN32_WCE
    static CsrCharString *displayString_PAS = "\0";
    switch (*(CsrBtAvrcpPrim *)msg)
    {
        case CSR_BT_AVRCP_CT_PASS_THROUGH_CFM:
        {
            CsrBtAvrcpCtPassThroughCfm *prim = (CsrBtAvrcpCtPassThroughCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_PASS_THROUGH_CFM received 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
        case CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM:
        {
            CsrBtAvrcpCtGetFolderItemsCfm * prim = (CsrBtAvrcpCtGetFolderItemsCfm *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);
            displayString_CT = displayString;
            displayString = NULL;
            CsrMemSet(temp, 0 , 100);

            instData->lastUidCounter = prim->uidCounter;

            if (prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                switch(prim->scope)
                {
                    case CSR_BT_AVRCP_SCOPE_SEARCH:
                    case CSR_BT_AVRCP_SCOPE_NPL:
                    case CSR_BT_AVRCP_SCOPE_MP_FS:
                    {
                        CsrBtAvrcpUid  mediaUid;
                        CsrBtAvrcpMediaType mediaType;
                        CsrBtAvrcpCharSet charset;
                        CsrBtAvrcpUid                   folderUid;
                        CsrBtAvrcpFolderType            folderType;
                        CsrBtAvrcpFolderPlayableType    playableType;
                        CsrUint16 mediaNameLen;
                        CsrUint8 * mediaName;
                        CsrUint16                       folderNameLen;
                        CsrUint8                        *folderName;
                        CsrUint8 attributeCount;
                        CsrUint16 index = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET;
                        CsrUint8     idx = 0;
                        CsrUint8                        i = 0, j = 0;
                        CsrBtAvrcpItemType              itemType = CSR_BT_AVRCP_ITEM_TYPE_INVALID;
                        CsrCharString tempString[100];


                        CsrSnprintf(temp, 100, "uidCounter=%u, startItem=%u, endItem=%u\n", prim->uidCounter, prim->startItem, prim->endItem);
                        CsrSnprintf(tempString, 100, "itemsCount=%u, itemsLength=%u\n", prim->itemsCount, prim->itemsDataLen);

                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp)
                                                     + sizeOfString(tempString) - 2);

                        CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) + sizeOfString(tempString) - 2);
                        CsrStrCat(displayString, displayString_CT);
                        if(*displayString_CT != '\0')
                        {
                            CsrPmemFree(displayString_CT);
                        }
                        CsrStrCat(displayString, temp);
                        CsrStrCat(displayString, tempString);
                        displayString_CT = displayString;
                        displayString = NULL;
                        CsrMemSet(temp, 0 , 100);

                        for (i = 0; i < prim->itemsCount; i++)
                        {
                            itemType = *(prim->itemsData + index);

                            switch(itemType)
                            {
                                case CSR_BT_AVRCP_ITEM_TYPE_FOLDER:
                                    {
                                        CsrUint8    *pName;
                                        CsrCharString temp_A[200];
                                        CsrCharString temp_B[200];
                                        CsrCharString *tempPtr = NULL;

                                        CsrBtAvrcpCtLibGfiFolderGet(&index,
                                                                    prim->itemsDataLen,
                                                                    prim->itemsData,
                                                                    &folderUid,
                                                                    &folderType,
                                                                    &playableType,
                                                                    &charset,
                                                                    &folderNameLen,
                                                                    &folderName);
                                        pName = CsrPmemAlloc(folderNameLen + 1);
                                        CsrMemSet(pName, 0, folderNameLen + 1);
                                        CsrMemCpy(pName, folderName, folderNameLen);

                                        sprintf(temp_A, "\n   ---   idx=%d\n", i);
                                        sprintf(temp_B, "folder UID=0x");
                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(temp_B) - 1);
                                        CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(temp_B) - 1));
                                        CsrStrCat(displayString, temp_A);
                                        CsrStrCat(displayString, temp_B);
                                        for (j = 0; j < CSR_BT_AVRCP_UID_SIZE; j++)
                                        {
                                            tempPtr = displayString;
                                            CsrMemSet(temp_A, 0 , 200);
                                            instData->lastFolderUidReceived[j] = folderUid[j];
                                            sprintf(temp_A,"%02X", folderUid[j]);
                                            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(tempPtr) - 1);
                                            CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(tempPtr) - 1));
                                            CsrStrCat(displayString, tempPtr);
                                            CsrStrCat(displayString, temp_A);
                                        }
                                        CsrMemSet(temp_A, 0 , 200);
                                        sprintf(temp_A,"\nfolder type=0x%02X\nplayable type=%s\n",
                                             folderType, (playableType == CSR_BT_AVRCP_ITEM_FOLDER_PLAYABLE_YES ? "yes" : "no"));
                                        tempPtr = displayString;
                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(tempPtr) - 1);
                                        CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(tempPtr) - 1));
                                        CsrStrCat(displayString, tempPtr);
                                        CsrStrCat(displayString, temp_A);


                                        CsrMemSet(temp_A, 0 , 200);
                                        sprintf(temp_A,"charset=0x%04x\nfolder Name Len=0x%04x\nfolder Name=%s\n",
                                            charset, folderNameLen, (CsrCharString *) pName);
                                        tempPtr = displayString;
                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(tempPtr) - 1);
                                        CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(tempPtr) - 1));
                                        CsrStrCat(displayString, tempPtr);
                                        CsrStrCat(displayString, temp_A);

                                        tempPtr = displayString;
                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(tempPtr) - 1);
                                        CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(tempPtr) - 1);
                                        CsrStrCat(displayString, displayString_CT);
                                        if(*displayString_CT != '\0')
                                        {
                                            CsrPmemFree(displayString_CT);
                                        }
                                        CsrStrCat(displayString, tempPtr);
                                        displayString_CT = displayString;
                                        displayString = NULL;

                                        CsrPmemFree(pName);
                                        break;
                                    }
                                case CSR_BT_AVRCP_ITEM_TYPE_MEDIA_ELEMENT:
                                    {
                                        CsrUint16 count;
                                        CsrUint8 *pName;
                                        CsrCharString temp_A[500];

                                        CsrBtAvrcpCtLibGfiMediaGet(&index,
                                                                   prim->itemsDataLen,
                                                                   prim->itemsData,
                                                                   &mediaUid,
                                                                   &mediaType,
                                                                   &charset,
                                                                   &mediaNameLen,
                                                                   &mediaName,
                                                                   &attributeCount);
                                        pName = CsrPmemAlloc(mediaNameLen + 1);
                                        CsrMemSet(pName,0,mediaNameLen + 1);
                                        CsrMemCpy(pName,mediaName,mediaNameLen);
                                        CsrMemSet(temp_A, 0 , 500);

                                        sprintf(temp_A,"\n   ---   idx=%d\ncharset=0x%04x\nmedia Name Len=0x%04x\nmedia Name=%s\nattribute count = %d\n",
                                            i, charset, mediaNameLen, (CsrCharString *) pName, attributeCount);

                                        CsrPmemFree(pName);

                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                                        CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                                        CsrStrCat(displayString, displayString_CT);
                                        if(*displayString_CT != '\0')
                                        {
                                            CsrPmemFree(displayString_CT);
                                        }
                                        CsrStrCat(displayString, temp_A);
                                        displayString_CT = displayString;
                                        displayString = NULL;

                                        for (count = 0; count < attributeCount; count++)
                                        {/* Get attributes */
                                            CsrBtAvrcpItemMediaAttributeId  attribId;
                                            CsrUint16                        attLen;
                                            CsrUint8                         * att;
                                            if (CsrBtAvrcpCtLibGfiMediaAttributeGet(prim->itemsDataLen,&index,
                                                                prim->itemsDataLen,
                                                                prim->itemsData,
                                                                &attribId,
                                                                &charset,
                                                                &attLen,
                                                                &att))
                                            {
                                                pName = CsrPmemAlloc(attLen + 1);
                                                CsrMemSet(pName,0,attLen + 1);
                                                CsrMemCpy(pName,att,attLen);
                                                CsrMemSet(temp_A, 0 , 500);

                                                sprintf(temp_A,"\tAttrib Id: %u\n\tCharset=0x%04x\n\tAttLen=0x%04x\n\tAttributes = %s\n",
                                                       attribId, charset, attLen, (CsrCharString *) pName);

                                                CsrPmemFree(pName);

                                                displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                                                CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                                                CsrStrCat(displayString, displayString_CT);
                                                if(*displayString_CT != '\0')
                                                {
                                                    CsrPmemFree(displayString_CT);
                                                }
                                                CsrStrCat(displayString, temp_A);
                                                displayString_CT = displayString;
                                                displayString = NULL;
                                            }
                                            idx++;
                                            CsrMemCpy(instData->lastUidReceived,mediaUid,sizeof(CsrBtAvrcpUid));
                                        }
                                        break;
                                    }
                                case CSR_BT_AVRCP_ITEM_TYPE_INVALID: /* Intended fall-through */
                                default:
                                    {
                                        sprintf(temp, "\nUnexpected  item type (0x%02X) received!\n", itemType);
                                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
                                        CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
                                        CsrStrCat(displayString, displayString_CT);
                                        if(*displayString_CT != '\0')
                                        {
                                            CsrPmemFree(displayString_CT);
                                        }
                                        CsrStrCat(displayString, temp);
                                        displayString_CT = displayString;
                                        displayString = NULL;
                                        break;
                                    }
                            }
                        }
                        printf("\n");
                        break;
                    }
                    case CSR_BT_AVRCP_SCOPE_MP_LIST:
                    {
                        CsrBtAvrcpMpTypeMajor majorType;
                        CsrBtAvrcpMpTypeSub subType;
                        CsrBtAvrcpPlaybackStatus playbackStatus;
                        CsrBtAvrcpMpFeatureMask featureMask;
                        CsrBtAvrcpCharSet charset;
                        CsrUint16 playerId;
                        CsrUint16 playerNameLen;
                        CsrUint8 * playerName;
                        CsrUint16 index = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET;
                        CsrUint8     mp_idx = 0;
                        CsrCharString tempString[100];


                        CsrSnprintf(temp, 100, "uidCounter=%u, startItem=%u, endItem=%u", prim->uidCounter, prim->startItem, prim->endItem);
                        CsrSnprintf(tempString, 100, "itemsCount=%u, itemsLength=%u", prim->itemsCount, prim->itemsDataLen);

                        displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp)
                             + sizeOfString(tempString) - 2);

                        CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) + sizeOfString(tempString) - 2);
                        CsrStrCat(displayString, displayString_CT);
                        if(*displayString_CT != '\0')
                        {
                            CsrPmemFree(displayString_CT);
                        }
                        CsrStrCat(displayString, temp);
                        CsrStrCat(displayString, tempString);
                        displayString_CT = displayString;
                        displayString = NULL;
                        CsrMemSet(temp, 0 , 100);

                        while (CsrBtAvrcpCtLibExtGfiMpGet(&index,
                                                    prim->itemsDataLen,
                                                    prim->itemsData,
                                                    &playerId,
                                                    &majorType,
                                                    &subType,
                                                    &playbackStatus,
                                                    &featureMask,
                                                    &charset,
                                                    &playerNameLen,
                                                    &playerName))
                        {/* The playerName does not have a NULL terminator: add it! */
                            CsrCharString temp_A[500];
                            CsrUint8 *pName = CsrPmemAlloc(playerNameLen + 1);
                            CsrMemSet(pName,0,playerNameLen + 1);
                            CsrMemCpy(pName,playerName,playerNameLen);

                            CsrMemSet(temp_A, 0, 500);
                            sprintf(temp_A,"\n   ---   mp_idx=%d\nplayer ID=0x%04x\nmajorType=0x%02x\nsubType=0x%08x\nplaybackStatus=0x%x\nfeatureMask=[0x%08x,0x%08x,0x%08x,0x%08x]\ncharset=0x%04x\nplayerNameLen=0x%04x\nplayerName=%s\n",
                                mp_idx, playerId, majorType, subType, playbackStatus,
                                featureMask[0],featureMask[1],featureMask[2],featureMask[3],
                                charset, playerNameLen, (CsrCharString *) pName);

                            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp_A) - 1);
                            CsrStrCat(displayString, displayString_CT);
                            if(*displayString_CT != '\0')
                            {
                                CsrPmemFree(displayString_CT);
                            }
                            CsrStrCat(displayString, temp_A);
                            displayString_CT = displayString;
                            displayString = NULL;

                            mp_idx++;
                            CsrPmemFree(pName);
                        }
                        CsrAppBacklogReqSend(TECH_BT, PROFILE_AVRCP, FALSE,"\n");
                        break;
                    }
                    default:
                        break;
                }
            }

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrPmemFree(prim->itemsData);
            break;
        }

        case CSR_BT_AVRCP_CT_PLAY_CFM:
        {
            CsrBtAvrcpCtPlayCfm *prim = (CsrBtAvrcpCtPlayCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_PLAY_CFM received; result 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_SEARCH_CFM:
        {
            CsrBtAvrcpCtSearchCfm *prim = (CsrBtAvrcpCtSearchCfm *)msg;
            CsrCharString displayString[500];
            CsrCharString temp[200];

            CsrSnprintf(displayString, 500, "CSR_BT_AVRCP_CT_SEARCH_CFM received; result: 0x%04x (0x%04x)\n",
                   prim->resultCode, prim->resultSupplier);
            CsrSnprintf(temp, 200, "Number of items found: %d\n",prim->numberOfItems);
            CsrStrCat(displayString, temp);

            CsrMemSet(temp, 0, 200);
            CsrSnprintf(temp, 200, "UID Counter is: %d\n\n", prim->uidCounter);
            CsrStrCat(displayString, temp);

            instData->lastUidCounter = prim->uidCounter;
            if (prim->numberOfItems > 0)
            {
                CsrBtAvrcpCtGetFolderItemsReqSend(instData->CsrSchedQid,
                                                 instData->avrcpCon[instData->currentConnection].connectionId,
                                                 CSR_BT_AVRCP_SCOPE_SEARCH,
                                                 0,
                                                 prim->numberOfItems,
                                                 0);
            }

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM:
        {
            CsrBtAvrcpCtInformDispCharsetCfm * prim = (CsrBtAvrcpCtInformDispCharsetCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM:
        {
            CsrBtAvrcpCtGetAttributesCfm *prim = (CsrBtAvrcpCtGetAttributesCfm *)msg;
            CsrUint16                        index = prim->attribDataPayloadOffset;
            CsrBtAvrcpItemMediaAttributeId  attribId;
            CsrBtAvrcpCharSet               charset;
            CsrUint16                        attLen;
            CsrUint8                         * att;
            CsrUint8                         n=0;
            CsrCharString displayString[50000];
            CsrCharString temp[200];

            CsrMemSet(displayString, 0, 50000);
            while (n < prim->attributeCount)
            {
                CsrUint16 i;

                CsrBtAvrcpCtLibItemsAttributeGet(0xDEAD, &index, prim->attribDataLen, prim->attribData, &attribId, &charset, &attLen, &att);

                CsrMemSet(temp, 0, 200);
                sprintf(temp, "attribID=0x%08x\n", attribId);
                CsrStrCat(displayString, temp);
                CsrMemSet(temp, 0, 200);
                sprintf(temp,"charset=0x%04x\n", charset);
                CsrStrCat(displayString, temp);
                CsrMemSet(temp, 0, 200);
                sprintf(temp,"attLen=0x%04x\n", attLen);
                CsrStrCat(displayString, temp);

                CsrStrCat(displayString, "attribute string =  \n");
                for (i=0; i<attLen; i++)
                {
                    CsrMemSet(temp, 0, 200);
                    sprintf(temp, "0x%x. ",att[i]);
                    CsrStrCat(displayString, temp);
                }

                CsrStrCat(displayString, "\n");
                n++;
            }
            CsrPmemFree(prim->attribData);

            CsrMemSet(temp, 0, 200);
            sprintf(temp, "CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);
            CsrStrCat(displayString, temp);

            instData->proceedFragmentation = TRUE;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, CSR_UI_INPUTMODE_AUTO,
                CSR_BT_AVRCP_STD_PRIO);
            break;
        }



        case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND:
        {
            CsrBtAvrcpCtGetAttributesInd *prim = (CsrBtAvrcpCtGetAttributesInd *)msg;
            CsrUint16                        index = prim->attribDataPayloadOffset;
            CsrBtAvrcpItemMediaAttributeId  attribId;
            CsrBtAvrcpCharSet               charset;
            CsrUint16                        attLen;
            CsrUint8                         * att;
            CsrUint8                         n=0;
            CsrCharString displayString[50000];
            CsrCharString temp[50];


            CsrBtAvrcpCtGetAttributesResSend(prim->connectionId,instData->proceedFragmentation);

            CsrMemSet(displayString, 0, 50000);
            while (n < prim->attributeCount)
            {
                CsrUint16 i;

                CsrBtAvrcpCtLibGfiMediaAttributeGet(0xDEAD, &index, prim->attribDataLen, prim->attribData, &attribId, &charset, &attLen, &att);

                CsrMemSet(temp, 0, 50);
                sprintf(temp, "attribID=0x%08x\n", attribId);
                CsrStrCat(displayString, temp);

                CsrMemSet(temp, 0, 50);
                sprintf(temp, "charset=0x%04x\n", charset);
                CsrStrCat(displayString, temp);

                CsrMemSet(temp, 0, 50);
                sprintf(temp, "attLen=0x%04x\n", attLen);
                CsrStrCat(displayString, temp);

                CsrStrCat(displayString, "attributes = \n");
                for (i=0; i<attLen; i++)
                {
                    CsrMemSet(temp, 0, 50);
                    sprintf(temp, "0x%x. ",att[i]);
                }
                CsrStrCat(displayString, "\n");

                n++;
            }

            CsrPmemFree(prim->attribData);

            CsrStrCat(displayString, "CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND:");
            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }
        case CSR_BT_AVRCP_CT_CHANGE_PATH_CFM:
        {
            CsrBtAvrcpCtChangePathCfm *prim = (CsrBtAvrcpCtChangePathCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_CHANGE_PATH_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_SET_VOLUME_CFM:
        {
            CsrBtAvrcpCtSetVolumeCfm *prim = (CsrBtAvrcpCtSetVolumeCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_SET_VOLUME_CFM received; volume: %d; result: 0x%04x (0x%04x)\n",
                prim->volume, prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM:
        {
            CsrBtAvrcpCtSetAddressedPlayerCfm *prim = (CsrBtAvrcpCtSetAddressedPlayerCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM:
        {
            CsrBtAvrcpCtSetBrowsedPlayerCfm *prim = (CsrBtAvrcpCtSetBrowsedPlayerCfm *)msg;
            CsrCharString displayString[100];

            CsrPmemFree(prim->folderNames);
            sprintf(displayString, "CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM:
        {
            CsrBtAvrcpCtAddToNowPlayingCfm *prim = (CsrBtAvrcpCtAddToNowPlayingCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM:
        {
            CsrBtAvrcpCtInformBatteryStatusCfm *prim = (CsrBtAvrcpCtInformBatteryStatusCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
#endif
        case CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM:
        {
            CsrBtAvrcpCtUnitInfoCmdCfm *prim = (CsrBtAvrcpCtUnitInfoCmdCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_UNIT_INFO_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrPmemFree(prim->pData);
            break;
        }

        case CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM:
        {
            CsrBtAvrcpCtSubUnitInfoCmdCfm *prim = (CsrBtAvrcpCtSubUnitInfoCmdCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            CsrPmemFree(prim->pData);
            break;
        }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14

        case CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM:
        {
            CsrBtAvrcpCtGetPlayStatusCfm *prim = (CsrBtAvrcpCtGetPlayStatusCfm *)msg;
            CsrCharString displayString[500];
            CsrCharString temp[50];

            CsrSnprintf(displayString, 50, "CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM received; result: 0x%04x (0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            sprintf(temp, "\tSong length: %u\n",prim->songLength);
            CsrStrCat(displayString, temp);

            CsrMemSet(temp, 0, 50);
            CsrSnprintf(temp, 50, "\tSong position: %u\n",prim->songPosition);
            CsrStrCat(displayString, temp);

            CsrMemSet(temp, 0, 50);
            CsrSnprintf(temp, 50, "\tPlay status: %u\n",prim->playStatus);
            CsrStrCat(displayString, temp);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM:
        {
            CsrBtAvrcpCtNotiRegisterCfm *prim = (CsrBtAvrcpCtNotiRegisterCfm *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM received (notiMask: 0x%08X)\n", prim->notiMask);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_UIDS_IND:
        {
            CsrBtAvrcpCtNotiUidsInd *prim = (CsrBtAvrcpCtNotiUidsInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            instData->lastUidCounter = prim->uidCounter;
            CsrSnprintf(temp, 100, "CSR_BT_AVRCP_CT_NOTI_UIDS_IND; UidCounter: %u\n", prim->uidCounter);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_VOLUME_IND:
        {
            CsrBtAvrcpCtNotiVolumeInd *prim = (CsrBtAvrcpCtNotiVolumeInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;


            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_VOLUME_IND (volume: 0x%02X)\n", prim->volume);
            if (prim->volume <= 15)
            {
                instData->avrcpCon[instData->currentConnection].currentVolume = prim->volume;
            }
            else
            {
                CsrStrCat(temp, "Volume value received invalid: higher than 15\n");
            }

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);


            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND:
        {
            CsrBtAvrcpCtNotiBatteryStatusInd *prim = (CsrBtAvrcpCtNotiBatteryStatusInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND (status: 0x%02X)\n", prim->batteryStatus);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND:
        {
            CsrBtAvrcpCtNotiPlaybackPosInd *prim = (CsrBtAvrcpCtNotiPlaybackPosInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND (position: 0x%08X)\n", prim->playbackPos);


            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND:
        {
            CsrBtAvrcpCtNotiSystemStatusInd *prim = (CsrBtAvrcpCtNotiSystemStatusInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND (status: 0x%02X)\n", prim->systemStatus);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND:
        {
            CsrBtAvrcpCtNotiTrackChangedInd *prim = (CsrBtAvrcpCtNotiTrackChangedInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND (UID: 0x%016X)\n", prim->trackUid);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND:
        {
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;


            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND\n");
            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND:
        {
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND\n");
            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND:
        {
            CsrBtAvrcpCtNotiPlaybackStatusInd *prim = (CsrBtAvrcpCtNotiPlaybackStatusInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND (status: 0x%02X)\n", prim->playbackStatus);
            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND:
        {
            CsrBtAvrcpCtSetAddressedPlayerInd *prim = (CsrBtAvrcpCtSetAddressedPlayerInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp,"CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND (UID counter: 0x%04X, Player ID: 0x%04X)\n", prim->uidCounter, prim->playerId);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND:
        {
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND\n");

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND:
        {
            CsrBtAvrcpCtNotiNowPlayingInd *prim = (CsrBtAvrcpCtNotiNowPlayingInd *)msg;
            CsrCharString temp[100];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND\n");
            /* Get now playing information - needed for TP/MCN/NP/BV-04-I */
            CsrStrCat(temp, "Getting playlist (now playing)...\n");
            CsrBtAvrcpCtGetFolderItemsReqSend(instData->CsrSchedQid,
                                         prim->connectionId,
                                         CSR_BT_AVRCP_SCOPE_NPL,
                                         0,
                                         3,
                                         0);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, temp);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_SET_IND:
        {
            CsrBtAvrcpCtPasSetInd *prim = (CsrBtAvrcpCtPasSetInd *)msg;
            CsrUint8 i;
            CsrCharString temp[100];
            CsrCharString tempString[200];
            CsrCharString *displayString = NULL;

            sprintf(tempString, "CSR_BT_AVRCP_CT_PAS_SET_IND received:\n");
            for (i=0; i < prim->attValPairCount ;i++)
            {
                CsrMemSet(temp, 0, 100);
                sprintf(temp, "attrib: %x; value: %x\n",prim->attValPair[i].attribId, prim->attValPair[i].valueId);
                CsrStrCat(tempString, temp);
            }
            CsrPmemFree(prim->attValPair);

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_CT) + sizeOfString(tempString) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_CT) + sizeOfString(tempString) - 1);
            CsrStrCat(displayString, displayString_CT);
            if(*displayString_CT != '\0')
            {
                CsrPmemFree(displayString_CT);
            }
            CsrStrCat(displayString, tempString);

            displayString_CT = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CONVERT_TEXT_STRING_2_UCS2("----CT Notification-----"),
                CONVERT_TEXT_STRING_2_UCS2(displayString_CT) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_CT_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM:
        {
            CsrBtAvrcpCtPasAttIdCfm *prim = (CsrBtAvrcpCtPasAttIdCfm *)msg;
            CsrUint8 i;
            CsrCharString temp[200];
            CsrCharString *displayString = NULL;

            sprintf(temp, "CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM - getting text and values for attributes...\n");

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_PAS);
            if(*displayString_PAS != '\0')
            {
                CsrPmemFree(displayString_PAS);
            }
            CsrStrCat(displayString, temp);
            displayString_PAS = displayString;
            displayString = NULL;


            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                CsrBtAvrcpCtPasAttTxtReqSend(instData->CsrSchedQid, prim->connectionId, prim->attIdCount, prim->attId);

                for (i = 0; i < prim->attIdCount; i++)
                {
                    CsrMemSet(temp, 0, 200);
                    sprintf(temp, "\tAsking for value IDs for attribute ID: %d\n",prim->attId[i]);
                    CsrBtAvrcpCtPasValIdReqSend(instData->CsrSchedQid, prim->connectionId, prim->attId[i]);

                    displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
                    CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
                    CsrStrCat(displayString, displayString_PAS);
                    if(*displayString_PAS != '\0')
                    {
                        CsrPmemFree(displayString_PAS);
                    }
                    CsrStrCat(displayString, temp);
                    displayString_PAS = displayString;
                    displayString = NULL;
                }
            }
            else
            {
                CsrPmemFree(prim->attId);
            }

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString_PAS) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM:
        {
            CsrBtAvrcpCtPasValIdCfm *prim = (CsrBtAvrcpCtPasValIdCfm *)msg;
            CsrUint8 i;
            CsrCharString temp[1024];
            CsrCharString *displayString = NULL;
            
            CsrMemSet(temp, 0, 1024);
            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                CsrUint32 sizeOfTemp = 0;
                sprintf(temp, "CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM - getting text for all values for attribute 0x%02X\n\tReceived ValIdCount: %d;\n",
                    prim->attId, prim->valIdCount);

                displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
                CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
                CsrStrCat(displayString, displayString_PAS);
                if(*displayString_PAS != '\0')
                {
                    CsrPmemFree(displayString_PAS);
                }
                CsrStrCat(displayString, temp);
                displayString_PAS = displayString;
                displayString = NULL;

                CsrMemSet(temp, 0, 1024);
                for (i=0; i<prim->valIdCount; i++)
                {
                    sprintf(temp + sizeOfTemp,"Value Id(%d): %d.\n",i,prim->valId[i]);
                    sizeOfTemp = (sizeOfString(temp) - 1);
                }

                CsrBtAvrcpCtPasValTxtReqSend(instData->CsrSchedQid, prim->connectionId,
                    prim->attId, prim->valIdCount, prim->valId);
            }
            else
            {
                CsrPmemFree(prim->valId);
            }

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_PAS);
            if(*displayString_PAS != '\0')
            {
                CsrPmemFree(displayString_PAS);
            }
            CsrStrCat(displayString, temp);
            displayString_PAS = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString_PAS) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM:
        {
            CsrBtAvrcpCtPasAttTxtCfm *prim = (CsrBtAvrcpCtPasAttTxtCfm *)msg;
            CsrUint16 index = 0;
            CsrBtAvrcpPasAttId attId;
            CsrBtAvrcpCharSet charset;
            CsrUint8 attTxtLen;
            CsrUint8 *attTxt;
            CsrCharString temp[1000];
            CsrCharString *displayString = NULL;
            CsrUint32 sizeOfTemp = 0;

            sprintf(temp, "CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM:\n");

            while (CsrBtAvrcpCtLibPasAttribTxtGet(prim->pasDataLen, prim->pasData, &index, &attId, &charset, &attTxtLen, &attTxt))
            {
                CsrUint16 i;

                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp, "\tAttId: %d\n",attId);
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp, "\tTextLength: %d  ",attTxtLen);
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp, "Text: ");
                for (i=0; i<attTxtLen; i++)
                {
                    sizeOfTemp =( sizeOfString(temp) - 1);
                    sprintf(temp + sizeOfTemp, "%c",attTxt[i]);
                }
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp, "\n\n");
            }

            CsrPmemFree(prim->pasData);
            instData->proceedFragmentation = TRUE; /* Restore default value */

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_PAS);
            if(*displayString_PAS != '\0')
            {
                CsrPmemFree(displayString_PAS);
            }
            CsrStrCat(displayString, temp);
            displayString_PAS = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString_PAS) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND:
        {
            CsrBtAvrcpCtPasAttTxtInd *prim = (CsrBtAvrcpCtPasAttTxtInd *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND:");

            /*for (i=0; i<prim->pasDataLen; i++)
            {
                printf("0x%x.",prim->pasData[i]);
            }
            printf("\n");*/
            CsrPmemFree(prim->pasData);

            CsrBtAvrcpCtPasAttTxtResSend(prim->connectionId,instData->proceedFragmentation);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM:
        {
            CsrBtAvrcpCtPasValTxtCfm *prim = (CsrBtAvrcpCtPasValTxtCfm *)msg;
            CsrUint16 index = 0;
            CsrBtAvrcpPasValId valId;
            CsrBtAvrcpCharSet charset;
            CsrUint8 valTxtLen;
            CsrUint8 *valTxt;
            CsrCharString temp[1000];
            CsrCharString *displayString = NULL;
            CsrUint32 sizeOfTemp = 0;

            sprintf(temp, "CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM (res: 0x%04x/0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            while (CsrBtAvrcpCtLibPasValueTxtGet(prim->pasDataLen, prim->pasData, &index, &valId, &charset, &valTxtLen, &valTxt))
            {
                CsrUint8 i;
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp,"\tValue Id: %d\n",valId);
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp,"\tValue text length: %d  ",valTxtLen);
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp,"Text: ");
                for (i=0; i<valTxtLen; i++)
                {
                    sizeOfTemp = (sizeOfString(temp) - 1);
                    sprintf(temp + sizeOfTemp,"%c",valTxt[i]);
                }
                sizeOfTemp = (sizeOfString(temp) - 1);
                sprintf(temp + sizeOfTemp,"\n\n");
            }
            CsrPmemFree(prim->pasData);
            instData->proceedFragmentation = TRUE; /* Restore default value */

            displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrMemSet(displayString, 0,  sizeOfString(displayString_PAS) + sizeOfString(temp) - 1);
            CsrStrCat(displayString, displayString_PAS);
            if(*displayString_PAS != '\0')
            {
                CsrPmemFree(displayString_PAS);
            }
            CsrStrCat(displayString, temp);
            displayString_PAS = displayString;
            displayString = NULL;

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString_PAS) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND:
        {
            CsrBtAvrcpCtPasValTxtInd *prim = (CsrBtAvrcpCtPasValTxtInd *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND: ");
            /*for (i=0; i<prim->pasDataLen; i++)
            {
                printf("0x%x.",prim->pasData[i]);
            }
            printf("\n");*/
            CsrPmemFree(prim->pasData);

            CsrBtAvrcpCtPasValTxtResSend(prim->connectionId,instData->proceedFragmentation);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_CURRENT_CFM:
        {
            CsrBtAvrcpCtPasCurrentCfm *prim = (CsrBtAvrcpCtPasCurrentCfm *)msg;
            CsrUint8 i;
            CsrBtAvrcpPasAttValPair *attValPtr;
            CsrCharString temp_A[1000];
            CsrCharString temp_B[100];

            sprintf(temp_A,"CSR_BT_AVRCP_CT_PAS_CURRENT_CFM (res: 0x%04x/0x%04x)\n",
                prim->resultCode, prim->resultSupplier);
            attValPtr = prim->attValPair;
            for (i=0; i<prim->attValPairCount; i++)
            {
                CsrMemSet(temp_B, 0, 100);
                sprintf(temp_B, "\tAttribute: %d; Value: %d\n",attValPtr->attribId,attValPtr->valueId);
                CsrStrCat(temp_A, temp_B);
                attValPtr++;
            }
            CsrPmemFree(prim->attValPair);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(temp_A), TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_PAS_NOTFCN_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_SET_CFM:
        {
            CsrBtAvrcpCtPasSetCfm *prim = (CsrBtAvrcpCtPasSetCfm *)msg;
            CsrCharString displayString[100];

            sprintf(displayString, "CSR_BT_AVRCP_CT_PAS_SET_CFM (res: 0x%04x/0x%04x)\n",
                prim->resultCode, prim->resultSupplier);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_EVENT_NOTFCN_1_DIALOG_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
#endif
        default:
        {
            CsrCharString displayString[100];

            sprintf(displayString, "Unknown controller AVRCP prim: 0x%04X\n", *(CsrBtAvrcpPrim *)msg);

            CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

            /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
            CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
            break;
        }
    }
#endif
}
#endif /* #ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE */
/**************************************************************************************************
 * handleAvrcpPrim
 **************************************************************************************************/
void av2HandleAvrcpPrim(avrcpinstance_t *instData, void *msg)
{
    CsrBtAvrcpPrim primType = *(CsrBtAvrcpPrim *)msg;

    if ((primType >= CSR_BT_AVRCP_PRIM_UPSTREAM_LOWEST) && (primType < CSR_BT_AVRCP_PRIM_UPSTREAM_COUNT + CSR_BT_AVRCP_PRIM_UPSTREAM_LOWEST))
    {/* Common prims */
        av2HandleAvrcpCmnPrim(instData, msg);
    }
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
    else if ((primType >= CSR_BT_AVRCP_TG_PRIM_UPSTREAM_LOWEST) && (primType < CSR_BT_AVRCP_TG_PRIM_UPSTREAM_COUNT + CSR_BT_AVRCP_TG_PRIM_UPSTREAM_LOWEST))
    {/* Target prims */
        av2HandleAvrcpTgPrim(instData, msg);
    }
#endif
#ifndef EXCLUDE_CSR_BT_AVRCP_CT_MODULE
    else if ((primType >= CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST) && (primType < CSR_BT_AVRCP_CT_PRIM_UPSTREAM_COUNT + CSR_BT_AVRCP_CT_PRIM_UPSTREAM_LOWEST))
    {/* Controller prims */
        av2HandleAvrcpCtPrim(instData, msg);
    }
#endif
    else
    {
        CsrCharString displayString[100];
      #ifndef _WIN32_WCE
        sprintf(displayString, "Invalid AVRCP prim: 0x%04X\n", primType);
#endif
        CsrBtAvrcpSetDialog(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, NULL,
                CONVERT_TEXT_STRING_2_UCS2(displayString) , TEXT_OK_UCS2, NULL);

        /* Show the CSR_BT_AVRCP_DEFAULT_INFO_UI on the display                   */
        CsrBtAvrcpShowUi(instData, CSR_BT_AVRCP_DEFAULT_INFO_UI, CSR_UI_INPUTMODE_AUTO, CSR_BT_AVRCP_STD_PRIO);
    }
}


