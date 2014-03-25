/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file contains functions and variables for
*                handling BT profiles
*	Updates			:
***********************************************************************/
#include "csr_synergy.h"
#include "csr_bt_declare_tasks.h"
#include "csr_pmem.h"
#include "csr_bt_app.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_avrcp_prim.h"
#include "Bluetooth_AVRC.h"
#include "Bluetooth_Common.h"
#include "AppAVRClib.h"

//AvrcAppInst *AvrcInstance;

CsrUint32 sizeOfString(CsrCharString *inputString)
{
    CsrUint32 stringCount = 0;

    while(inputString[stringCount++] != '\0');

    return stringCount;
}


/*****************************************************************************
* Function Name : init_avrc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_avrc(void **gash)
{
	AvrcAppInst *inst;


	*gash = CsrPmemAlloc(sizeof(AvrcAppInst));
	CsrMemSet(*gash, 0, sizeof(AvrcAppInst));
	inst = (AvrcAppInst *)*gash;
	//AvrcInstance=inst;

	inst->qid = TESTQUEUE;

	
}

/*****************************************************************************
* Function Name : deinit_avrc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
#ifdef ENABLE_SHUTDOWN
void deinit_avrc(void **gash)
{
	AvrcAppInst *inst;

	inst = *gash;
	inst->state =SSAvStateAvrcpIdle;

	CsrPmemFree(inst);
	//AvrcInstance=NULL;
}

#else
void deinit_avrc(void **gash)
{
    CSR_UNUSED(gash);
}
#endif


/*****************************************************************************
* Function Name : handleAvrcPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/

void handleAvrcPrim(AvrcAppInst  *instData,CsrPrim    *thePrim)
{

	switch (*thePrim)
	{
        case CSR_BT_AVRCP_CONFIG_CFM:
        {
            CsrBtAvrcpConfigCfm *prim = (CsrBtAvrcpConfigCfm *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CONFIG_CFM\n"));

            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
	
#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
				CsrUint16 pasLen = 0;
				CsrUint8 *pas = NULL;
				CsrBtAvrcpMpFeatureMask mpFeatures;
				/* All opcodes allowed */

				mpFeatures[0] = 0xFFFFFFFF;
				mpFeatures[1] = 0xFFFFFFFF;
				mpFeatures[2] = 7;
				mpFeatures[3] = 0;

#ifdef CSR_BT_INSTALL_AVRCP_TG_13_14
				CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, (CsrUtf8String *)"Equalizer123456789098765678");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF,(CsrUtf8String *) "Off");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EQUALIZER_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_ON, (CsrUtf8String *)"On");

				CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, (CsrUtf8String *)"Equalizer123456789098765678");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SCAN_ATT_ID, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, (CsrUtf8String *)"Off");

				CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, (CsrUtf8String *)"Equalizer123456789098765678Equalizer1Equalizer1Equalizer122");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN+1, CSR_BT_AVRCP_PAS_EQUALIZER_VAL_OFF, (CsrUtf8String *)"Off");

				CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, (CsrUtf8String *)"Shuffle");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_OFF, (CsrUtf8String *)"Off");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_SHUFFLE_ATT_ID, CSR_BT_AVRCP_PAS_SHUFFLE_VAL_ALL, (CsrUtf8String *)"All");

				/* Custom example of equalizer presets */
				CsrBtAvrcpTgLibPasAttribAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, (CsrUtf8String *)"Equalizer presets");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x01, (CsrUtf8String *)"Standard");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x02, (CsrUtf8String *)"Rock");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x03, (CsrUtf8String *)"Pop");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x04, (CsrUtf8String *)"Classic");
				CsrBtAvrcpTgLibPasValueAdd(&pasLen, &pas, CSR_BT_AVRCP_PAS_EXT_ATT_ID_BEGIN, 0x05, (CsrUtf8String *)"Jazz");
#endif

				CsrBtAvrcpTgMpRegisterReqSend(instData->qid,							 /* Player handle */
										 CSR_BT_AVRCP_NOTI_FLAG_NONE,				 /* Notifications */
										 CSR_BT_AVRCP_TG_MP_REGISTER_CONFIG_SET_DEFAULT,   /* Other configuration */
										 pasLen,
										 pas,
										 CSR_BT_AVRCP_MP_TYPE_MAJOR_AUDIO,
										 CSR_BT_AVRCP_MP_TYPE_SUB_NONE,
										 mpFeatures,
										 CsrUtf8StrDup("Samsung Player"));
#endif


			    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"send CsrBtAvrcpActivateReqSend\n"));

            	CsrBtAvrcpActivateReqSend(1);  //_maxIncoming
            }
            else
            {
            	CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CONFIG_CFM err resultCode[%x],resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));
            }
            break;
        }

        case CSR_BT_AVRCP_REMOTE_FEATURES_IND:
        {


            /* Incomplete feature - just ignore */
            CsrBtAvrcpRemoteFeaturesInd *prim = (CsrBtAvrcpRemoteFeaturesInd *)thePrim;


            if (prim->ctFeatures.roleSupported)
            {

			    
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_REMOTE_FEATURES_IND ctFeatures.roleSupported  srAvrcpVersion %x\n",prim->ctFeatures.srAvrcpVersion));

                instData->remoteAvrcpVersion = prim->ctFeatures.srAvrcpVersion;
            }



            if (prim->tgFeatures.roleSupported)
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_REMOTE_FEATURES_IND tgFeatures.roleSupported %x\n",prim->tgFeatures.srAvrcpVersion));
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_REMOTE_FEATURES_IND connectedid %x\n",prim->connectionId));
				instData->connectionId = prim->connectionId;

                instData->remoteAvrcpVersion = prim->tgFeatures.srAvrcpVersion;
				CsrAppServerAVRCRemoteFeatureInd(prim->tgFeatures.srFeatures);
				if(prim->tgFeatures.srAvrcpVersion>=CSR_BT_AVRCP_CONFIG_SR_VERSION_13)
				{

#if 0  //notification test							
						    CsrBtAvrcpCtNotiRegisterReqSend(instData->qid,
                           prim->connectionId,
                           CSR_BT_AVRCP_NOTI_FLAG_PLAYBACK_STATUS,
                           0,
                           CSR_BT_AVRCP_NOTI_REG_STANDARD);
#endif
					//		CsrBtAvrcpCtGetPlayStatusReqSend(instData->qid,prim->connectionId);
				}
            }

            CsrBtAvrcpUtilFreeRoleDetails(&prim->tgFeatures);
            CsrBtAvrcpUtilFreeRoleDetails(&prim->ctFeatures);
            break;
        }

        case CSR_BT_AVRCP_DISCONNECT_IND:
        {
			CsrBtAvrcpDisconnectInd * prim = (CsrBtAvrcpDisconnectInd *) thePrim;
			instData->state =SSAvStateAvrcpIdle;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_DISCONNECT_IND  resultCode[%x] resultSupplier[%x] localTerminated[%x]\n",prim->reasonCode, prim->reasonSupplier,prim->localTerminated));
		
			if(prim->localTerminated)
				CsrAppServerAVRCDisconectCfm(&(instData->remoteAddr),prim->reasonCode,prim->reasonSupplier);
			else
				CsrAppServerAVRCDisconectInd(&(instData->remoteAddr),prim->reasonCode,prim->reasonSupplier);


            break;
        }

        case CSR_BT_AVRCP_ACTIVATE_CFM:
        {
            CsrBtAvrcpActivateCfm * prim = (CsrBtAvrcpActivateCfm *) thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_ACTIVATE_CFM\n"));
		

            if(prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
				instData->state =SSAvStateAvrcpIdle;
				CsrAppServerAVRCActivateCfm();


            }
            else
            {
				CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_ACTIVATE_CFM err resultSupplier[%x] resultCode[%x]\n",prim->resultSupplier, prim->resultCode));

            }

            break;
        }

        case CSR_BT_AVRCP_DEACTIVATE_CFM:
        {
			
            CsrBtAvrcpDeactivateCfm * prim = (CsrBtAvrcpDeactivateCfm *) thePrim;
			CsrAppServerAVRCDeActivateCfm();

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_DEACTIVATE_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));

            break;
        }

        case CSR_BT_AVRCP_CONNECT_CFM:
        {

            CsrBtAvrcpConnectCfm * prim= (CsrBtAvrcpConnectCfm *) thePrim;

			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CONNECT_CFM\n"));

            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
			   instData->connectionId = prim->connectionId;
			   instData->remoteAddr = prim->deviceAddr;
			   instData->state  =SSAvStateAvrcpConnected;
			    CsrBtAvrcpCtNotiRegisterReqSend(instData->qid,
                           instData->connectionId,
                           CSR_BT_AVRCP_NOTI_FLAG_ALL,
                           0,
                           CSR_BT_AVRCP_NOTI_REG_STANDARD);

            }
			else
			{
			   CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CONNECT_CFM err resultSupplier[%x] resultCode[%x]\n",prim->resultSupplier, prim->resultCode));
			   instData->state  =SSAvStateAvrcpIdle;
			}

			CsrAppServerAVRCConnetCfm(&(prim->deviceAddr),prim->resultCode,prim->resultSupplier);
            break;
        }

        case CSR_BT_AVRCP_CONNECT_IND:
        {
            CsrBtAvrcpConnectInd *prim = (CsrBtAvrcpConnectInd *) thePrim;
             CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CONNECT_IND\n"));
			instData->connectionId = prim->connectionId;
			instData->remoteAddr = prim->deviceAddr;
			instData->state  =SSAvStateAvrcpConnected;
			CsrAppServerAVRCConnetInd(&(prim->deviceAddr));


    
            break;
        }
			break;
        case CSR_BT_AVRCP_TG_PASS_THROUGH_IND:
		{
            CsrBtAvrcpTgPassThroughInd *prim = (CsrBtAvrcpTgPassThroughInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_TG_PASS_THROUGH_IND\n"));

			CsrAppServerAVRCTgPassThroughInd(prim->operationId, prim->state);
           
            CsrBtAvrcpTgPassThroughResSend(prim->connectionId,
                                      prim->msgId,
                                      CSR_BT_AVRCP_PT_STATUS_ACCEPT);
    
            break;
        }


        case CSR_BT_AVRCP_CT_PASS_THROUGH_CFM:
        {
			CsrBtAvrcpCtPassThroughCfm *prim = (CsrBtAvrcpCtPassThroughCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PASS_THROUGH_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));

			CsrAppServerAVRCCtPassThroughCfm(prim->operationId,prim->resultCode,prim->resultSupplier);

            break;
        }
//#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
        case CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM:
		{
				CsrBtAvrcpCtGetFolderItemsCfm * prim = (CsrBtAvrcpCtGetFolderItemsCfm *)thePrim;
				CsrCharString temp[100];
				CsrCharString *displayString = NULL;
				CsrCharString *displayString_CT = "\0";

				//CsrBtSprintf(temp, "CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM received; result: 0x%04x (0x%04x)\n",
				//	prim->resultCode, prim->resultSupplier);

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
							CsrBtAvrcpUid					folderUid;
							CsrBtAvrcpFolderType			folderType;
							CsrBtAvrcpFolderPlayableType	playableType;
							CsrUint16 mediaNameLen;
							CsrUint8 * mediaName;
							CsrUint16						folderNameLen;
							CsrUint8						*folderName;
							CsrUint8 attributeCount;
							CsrUint16 index = CSR_BT_AVRCP_LIB_GFI_HEADER_OFFSET;
							CsrUint8	 idx = 0;
							CsrUint8						i = 0, j = 0;
							CsrBtAvrcpItemType				itemType = CSR_BT_AVRCP_ITEM_TYPE_INVALID;
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
											CsrUint8	*pName;
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
											pName = (CsrUint8	*)CsrPmemAlloc(folderNameLen + 1);
											CsrMemSet(pName, 0, folderNameLen + 1);
											CsrMemCpy(pName, folderName, folderNameLen);

											//CsrBtSprintf(temp_A, "\n   ---	idx=%d\n", i);
											//CsrBtSprintf(temp_B, "folder UID=0x");
											displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(temp_B) - 1);
											CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(temp_B) - 1));
											CsrStrCat(displayString, temp_A);
											CsrStrCat(displayString, temp_B);
											for (j = 0; j < CSR_BT_AVRCP_UID_SIZE; j++)
											{
												tempPtr = displayString;
												CsrMemSet(temp_A, 0 , 200);
												instData->lastFolderUidReceived[j] = folderUid[j];
												//CsrBtSprintf(temp_A,"%02X", folderUid[j]);
												displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(tempPtr) - 1);
												CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(tempPtr) - 1));
												CsrStrCat(displayString, tempPtr);
												CsrStrCat(displayString, temp_A);
											}
											CsrMemSet(temp_A, 0 , 200);
											//CsrBtSprintf(temp_A,"\nfolder type=0x%02X\nplayable type=%s\n",
											//	 folderType, (playableType == CSR_BT_AVRCP_ITEM_FOLDER_PLAYABLE_YES ? "yes" : "no"));
											tempPtr = displayString;
											displayString = (CsrCharString *)CsrPmemAlloc(sizeOfString(temp_A) + sizeOfString(tempPtr) - 1);
											CsrMemSet(displayString, 0, (sizeOfString(temp_A) + sizeOfString(tempPtr) - 1));
											CsrStrCat(displayString, tempPtr);
											CsrStrCat(displayString, temp_A);


											CsrMemSet(temp_A, 0 , 200);
											//CsrBtSprintf(temp_A,"charset=0x%04x\nfolder Name Len=0x%04x\nfolder Name=%s\n",
											//	charset, folderNameLen, (CsrCharString *) pName);
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
											pName = (CsrUint8 *)CsrPmemAlloc(mediaNameLen + 1);
											CsrMemSet(pName,0,mediaNameLen + 1);
											CsrMemCpy(pName,mediaName,mediaNameLen);
											CsrMemSet(temp_A, 0 , 500);

											//CsrBtSprintf(temp_A,"\n	 ---   idx=%d\ncharset=0x%04x\nmedia Name Len=0x%04x\nmedia Name=%s\nattribute count = %d\n",
											//	i, charset, mediaNameLen, (CsrCharString *) pName, attributeCount);

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
												CsrBtAvrcpItemMediaAttributeId	attribId;
												CsrUint16						 attLen;
												CsrUint8						 * att;
												if (CsrBtAvrcpCtLibGfiMediaAttributeGet(prim->itemsDataLen,&index,
																	prim->itemsDataLen,
																	prim->itemsData,
																	&attribId,
																	&charset,
																	&attLen,
																	&att))
												{
													pName = (CsrUint8 *)CsrPmemAlloc(attLen + 1);
													CsrMemSet(pName,0,attLen + 1);
													CsrMemCpy(pName,att,attLen);
													CsrMemSet(temp_A, 0 , 500);

													//CsrBtSprintf(temp_A,"\tAttrib Id: %u\n\tCharset=0x%04x\n\tAttLen=0x%04x\n\tAttributes = %s\n",
													//	   attribId, charset, attLen, (CsrCharString *) pName);

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
											//CsrBtSprintf(temp, "\nUnexpected  item type (0x%02X) received!\n", itemType);
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
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"\n"));
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
							CsrUint8	 mp_idx = 0;
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
								CsrUint8 *pName = (CsrUint8 *)CsrPmemAlloc(playerNameLen + 1);
								CsrMemSet(pName,0,playerNameLen + 1);
								CsrMemCpy(pName,playerName,playerNameLen);

								CsrMemSet(temp_A, 0, 500);
								//CsrBtSprintf(temp_A,"\n	 ---   mp_idx=%d\nplayer ID=0x%04x\nmajorType=0x%02x\nsubType=0x%08x\nplaybackStatus=0x%x\nfeatureMask=[0x%08x,0x%08x,0x%08x,0x%08x]\ncharset=0x%04x\nplayerNameLen=0x%04x\nplayerName=%s\n",
								//	mp_idx, playerId, majorType, subType, playbackStatus,
								//	featureMask[0],featureMask[1],featureMask[2],featureMask[3],
								//	charset, playerNameLen, (CsrCharString *) pName);

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
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"\n"));
							break;
						}
						default:
							break;
					}
				}


				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,displayString_CT));

	

				CsrPmemFree(prim->itemsData);
				break;
			}

        case CSR_BT_AVRCP_CT_PLAY_CFM:
        {
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PLAY_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_SEARCH_CFM:
        {
            CsrBtAvrcpCtSearchCfm *prim = (CsrBtAvrcpCtSearchCfm *)thePrim;
           
		    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SEARCH_CFM\n"));

            instData->lastUidCounter = prim->uidCounter;
            if (prim->numberOfItems > 0)
            {
                CsrBtAvrcpCtGetFolderItemsReqSend(instData->qid,
                                                 prim->connectionId,
                                                 CSR_BT_AVRCP_SCOPE_SEARCH,
                                                 0,
                                                 prim->numberOfItems,
                                                 0);
            }

            break;
        }

        case CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM:
        {
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SEARCH_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM: 
		{
			CsrBtAvrcpCtGetAttributesCfm *prim = (CsrBtAvrcpCtGetAttributesCfm *)thePrim;
			CsrUint16						 index = prim->attribDataPayloadOffset;
			CsrBtAvrcpItemMediaAttributeId	attribId;
			CsrBtAvrcpCharSet				charset;
			CsrUint16						 attLen;
			CsrUint8						 * att;
			CsrUint8						 n=0;
			CsrCharString displayString[50000];
			CsrCharString temp[200];

			CsrMemSet(displayString, 0, 50000);
			while (n < prim->attributeCount)
			{
				CsrUint16 i;

				CsrBtAvrcpCtLibItemsAttributeGet(0xDEAD, &index, prim->attribDataLen, prim->attribData, &attribId, &charset, &attLen, &att);

				CsrMemSet(temp, 0, 200);
				//CsrBtSprintf(temp, "attribID=0x%08x\n", attribId);
				CsrStrCat(displayString, temp);
				CsrMemSet(temp, 0, 200);
				//CsrBtSprintf(temp,"charset=0x%04x\n", charset);
				CsrStrCat(displayString, temp);
				CsrMemSet(temp, 0, 200);
				//CsrBtSprintf(temp,"attLen=0x%04x\n", attLen);
				CsrStrCat(displayString, temp);

				CsrStrCat(displayString, "attribute string =  \n");
				for (i=0; i<attLen; i++)
				{
					CsrMemSet(temp, 0, 200);
					//CsrBtSprintf(temp, "0x%x. ",att[i]);
					CsrStrCat(displayString, temp);
				}

				CsrStrCat(displayString, "\n");
				n++;
			}
			CsrPmemFree(prim->attribData);

			CsrMemSet(temp, 0, 200);
			//CsrBtSprintf(temp, "CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM received; result: 0x%04x (0x%04x)\n",
			//	prim->resultCode, prim->resultSupplier);
			CsrStrCat(displayString, temp);

			instData->proceedFragmentation = TRUE;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,displayString));

			break;
		}


        case CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND:
		{
            CsrBtAvrcpCtGetAttributesInd *prim = (CsrBtAvrcpCtGetAttributesInd *)thePrim;
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
                //CsrBtSprintf(temp, "attribID=0x%08x\n", attribId);
                CsrStrCat(displayString, temp);

                CsrMemSet(temp, 0, 50);
                //CsrBtSprintf(temp, "charset=0x%04x\n", charset);
                CsrStrCat(displayString, temp);

                CsrMemSet(temp, 0, 50);
                //CsrBtSprintf(temp, "attLen=0x%04x\n", attLen);
                CsrStrCat(displayString, temp);

                CsrStrCat(displayString, "attributes = \n");
                for (i=0; i<attLen; i++)
                {
                    CsrMemSet(temp, 0, 50);
                    //CsrBtSprintf(temp, "0x%x. ",att[i]);
                }
                CsrStrCat(displayString, "\n");

                n++;
            }

            CsrPmemFree(prim->attribData);

            CsrStrCat(displayString, "CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND:");
            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,displayString));

            break;
        }
        case CSR_BT_AVRCP_CT_CHANGE_PATH_CFM:
        {
//            CsrBtAvrcpCtChangePathCfm *prim = (CsrBtAvrcpCtChangePathCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_CHANGE_PATH_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_SET_VOLUME_CFM:
        {
            CsrBtAvrcpCtSetVolumeCfm *prim = (CsrBtAvrcpCtSetVolumeCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SET_VOLUME_CFM result resultCode[%x] resultSupplier[%x]\n",prim->resultCode, prim->resultSupplier));

            break;
        }

        case CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM:
        {
    //        CsrBtAvrcpCtSetAddressedPlayerCfm *prim = (CsrBtAvrcpCtSetAddressedPlayerCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM:
        {
      //      CsrBtAvrcpCtSetBrowsedPlayerCfm *prim = (CsrBtAvrcpCtSetBrowsedPlayerCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM:
        {
         //   CsrBtAvrcpCtAddToNowPlayingCfm *prim = (CsrBtAvrcpCtAddToNowPlayingCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM:
        {
          //  CsrBtAvrcpCtInformBatteryStatusCfm *prim = (CsrBtAvrcpCtInformBatteryStatusCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM\n"));

            break;
        }
//#endif
        case CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM:
        {
            CsrBtAvrcpCtUnitInfoCmdCfm *prim = (CsrBtAvrcpCtUnitInfoCmdCfm *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM\n"));

            CsrPmemFree(prim->pData);
            break;
        }

        case CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM:
        {
            CsrBtAvrcpCtSubUnitInfoCmdCfm *prim = (CsrBtAvrcpCtSubUnitInfoCmdCfm *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM\n"));

            CsrPmemFree(prim->pData);
            break;
        }
#ifdef CSR_BT_INSTALL_AVRCP_CT_13_14
        case CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM:
        {
            CsrBtAvrcpCtGetPlayStatusCfm *prim = (CsrBtAvrcpCtGetPlayStatusCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM %d %x %x\n", prim->playStatus,prim->resultCode,prim->resultSupplier));
		 
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM:
        {
             CsrBtAvrcpCtNotiRegisterCfm *prim = (CsrBtAvrcpCtNotiRegisterCfm *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM notiMask[%x]\n",prim->notiMask));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_UIDS_IND:
        {
          //  CsrBtAvrcpCtNotiUidsInd *prim = (CsrBtAvrcpCtNotiUidsInd *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_UIDS_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_VOLUME_IND:
        {
          //  CsrBtAvrcpCtNotiVolumeInd *prim = (CsrBtAvrcpCtNotiVolumeInd *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_VOLUME_IND\n"));


            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND:
        {
        //    CsrBtAvrcpCtNotiBatteryStatusInd *prim = (CsrBtAvrcpCtNotiBatteryStatusInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND:
        {
         //   CsrBtAvrcpCtNotiPlaybackPosInd *prim = (CsrBtAvrcpCtNotiPlaybackPosInd *)thePrim;
   			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND\n"));
            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND:
        {
         //   CsrBtAvrcpCtNotiSystemStatusInd *prim = (CsrBtAvrcpCtNotiSystemStatusInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND:
        {
          //  CsrBtAvrcpCtNotiTrackChangedInd *prim = (CsrBtAvrcpCtNotiTrackChangedInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND:
        {
		//	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND:
        {
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND\n"));


            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND:
        {
            CsrBtAvrcpCtNotiPlaybackStatusInd *prim = (CsrBtAvrcpCtNotiPlaybackStatusInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND %d\n",prim->playbackStatus));

            break;
        }

        case CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND:
        {
  //          CsrBtAvrcpCtSetAddressedPlayerInd *prim = (CsrBtAvrcpCtSetAddressedPlayerInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND:
        {
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND:
        {
    //        CsrBtAvrcpCtNotiNowPlayingInd *prim = (CsrBtAvrcpCtNotiNowPlayingInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_SET_IND:
        {
       //     CsrBtAvrcpCtPasSetInd *prim = (CsrBtAvrcpCtPasSetInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_SET_IND\n"));

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM:
        {
            CsrBtAvrcpCtPasAttIdCfm *prim = (CsrBtAvrcpCtPasAttIdCfm *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));			
			
            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {
                CsrBtAvrcpCtPasAttTxtReqSend(instData->qid, prim->connectionId, prim->attIdCount, prim->attId);

            }
            else
            {
                CsrPmemFree(prim->attId);
            }

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM:
        {
            CsrBtAvrcpCtPasValIdCfm *prim = (CsrBtAvrcpCtPasValIdCfm *)thePrim;
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));
			
            if (prim->resultSupplier == CSR_BT_SUPPLIER_AVRCP &&
                prim->resultCode == CSR_BT_RESULT_CODE_AVRCP_SUCCESS)
            {


                CsrBtAvrcpCtPasValTxtReqSend(instData->qid, prim->connectionId,
                    prim->attId, prim->valIdCount, prim->valId);
            }
            else
            {
                CsrPmemFree(prim->valId);
            }


            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM:
        {
            CsrBtAvrcpCtPasAttTxtCfm *prim = (CsrBtAvrcpCtPasAttTxtCfm *)thePrim;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));			

            CsrPmemFree(prim->pasData);
            instData->proceedFragmentation = TRUE; /* Restore default value */



            break;
        }

        case CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND:
        {
            CsrBtAvrcpCtPasAttTxtInd *prim = (CsrBtAvrcpCtPasAttTxtInd *)thePrim;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND\n"));


            CsrPmemFree(prim->pasData);

            CsrBtAvrcpCtPasAttTxtResSend(prim->connectionId,instData->proceedFragmentation);


            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM:
        {
            CsrBtAvrcpCtPasValTxtCfm *prim = (CsrBtAvrcpCtPasValTxtCfm *)thePrim;
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));			
			
            CsrPmemFree(prim->pasData);


            break;
        }

        case CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND:
        {
            CsrBtAvrcpCtPasValTxtInd *prim = (CsrBtAvrcpCtPasValTxtInd *)thePrim;
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND\n"));			
			
            break;
        }

        case CSR_BT_AVRCP_CT_PAS_CURRENT_CFM:
        {
            CsrBtAvrcpCtPasCurrentCfm *prim = (CsrBtAvrcpCtPasCurrentCfm *)thePrim;
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_CURRENT_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));			
			
            CsrPmemFree(prim->attValPair);

            break;
        }

        case CSR_BT_AVRCP_CT_PAS_SET_CFM:
        {
            CsrBtAvrcpCtPasSetCfm *prim = (CsrBtAvrcpCtPasSetCfm *)thePrim;
			
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AVRCP_CT_PAS_SET_CFM resultCode[%x] resultSupplier[%x]\n",prim->resultCode,prim->resultSupplier));			
			
            break;
        }
//#endif
#endif

        default:
        {
			 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unhandled AVRC primitive: 0x%04x,\n", *thePrim));
			 CsrBtAvrcpFreeUpstreamMessageContents(CSR_BT_AVRCP_PRIM, thePrim);

            break;
        }
	}

}
