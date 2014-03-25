/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file contains functions and variables for
*                handling BT profiles
*	Updates			:
***********************************************************************/
#include "csr_synergy.h"

#include "csr_msg_transport.h"

#include "csr_sched.h"
#include "csr_pmem.h"

#include "csr_bt_declare_tasks.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "Bluetooth_Common.h"
#include "Bluetooth_AV.h"
#include "csr_bt_av2_util.h"
#include "csr_bt_ss_prim.h"
#include "AppAVlib.h"



Av2AppInst *Av2Instance;

#define ASSIGN_TLABEL                    ((CsrUint8)(instData->avTLabel++ % 16))
#define DEFAULT_AV_BITRATE    256
/* Default PCM sampling settings */
#define DEFAULT_BPS                      (16)    /* Bits per sample */
#define DEFAULT_FREQ                     (48000) /* Sampling frequency */
#define DEFAULT_CHANNELS                 (2)     /* Number of channels, 1:mono, 2:stereo */
#define DEFAULT_BUFFER_SIZE              (12288) /* Default grab buffer size (based on SBC defaults) */



CsrUint16 targetBitrate       = DEFAULT_AV_BITRATE;

/*****************************************************************************
* Function Name : init_av2()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/

void av2InitInstanceData(Av2AppInst *instData)
{
    CsrUint8 i;
//    extern BD_ADDR_T defGlobalBdAddr;

    /* Application handle */
    instData->qid                 = TESTQUEUE;

    /* Instance */
 //   instData->state               = DaAvStateAppInit;
 //   instData->old_state           = DaAvStateAppInit;
 //   instData->numInList           = 0;
 //   instData->numInListConfig     = 0;

 //   instData->selectedDevice.lap  = 0;
 //   instData->selectedDevice.uap  = 0;
 //   instData->selectedDevice.nap  = 0;

    instData->reconfiguring       = FALSE;
    instData->avIsActivated       = FALSE;
 //   instData->max_num_counter     = 0;
 //   instData->currentConnection   = 0;
    instData->maxFrameSize        = CSR_BT_AV_PROFILE_DEFAULT_MTU_SIZE;
    instData->time_stamp          = 0;
    instData->mediaPayloadType    = MEDIA_PAYLOAD_TYPE;
    instData->filter_timer        = 0;
    instData->avTLabel            = 0;
 //   instData->testMenuActive      = FALSE;
    instData->localSeid[INDEX_LOCAL_SBC_SEID]      = SBC_CODEC_TYPE;
    instData->localSeidInUse[INDEX_LOCAL_SBC_SEID] = FALSE;
#ifndef EXCLUDE_CSR_BT_SS_CODEC	
	instData->localSeid[INDEX_LOCAL_SS_SEID]      = SS_CODEC_TYPE;
    instData->localSeidInUse[INDEX_LOCAL_SS_SEID] = FALSE;
#endif
    /* Default PCM setup (48000 Hz, stereo) */
    instData->pcm_freq = DEFAULT_FREQ;
    instData->pcm_channels = DEFAULT_CHANNELS;
    instData->pcm_bits = DEFAULT_BPS;
    instData->pcm_buffer_size = DEFAULT_BUFFER_SIZE;

  	filtersSetup(instData);

}


void init_av2(void **gash)
{
    Av2AppInst *inst;

    *gash = CsrPmemAlloc(sizeof(Av2AppInst));
    CsrMemSet(*gash, 0, sizeof(Av2AppInst));
    inst = (Av2AppInst *) *gash;

    inst->qid = TESTQUEUE;

    Av2Instance=inst;
    av2InitInstanceData(inst);

   // CsrBtAvActivateReqSend(inst->qid, CSR_BT_AV_AUDIO_SINK);
}

/*****************************************************************************
* Function Name : deinit_av2()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
#ifdef ENABLE_SHUTDOWN
void deinit_av2(void **gash)
{
	Av2AppInst *inst;

	inst = *gash;

	CsrPmemFree(inst);

    
#ifdef USE_WAVE
    CsrPmemFree(inst->wav_filename_in);
    CsrPmemFree(inst->wav_filename_out);
#endif
    
#ifdef USE_SBCFILE
    CsrPmemFree(inst->sbc_filename_in);
    CsrPmemFree(inst->sbc_filename_out);
#endif
    
#ifdef USE_MP3
    CsrPmemFree(inst->mp3_filename_in);
#endif

#ifdef USE_AAC
    CsrPmemFree(inst->aac_filename_in);
#endif

    Av2Instance=NULL;
}

#else
void deinit_av2(void **gash)
{
    CSR_UNUSED(gash);
}
#endif


/*****************************************************************************
* Function Name : handleAv2Prim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
#ifndef EXCLUDE_CSR_BT_SS_CODEC
CsrBool btAVsetCodeConfig(    Av2AppInst *instData,  CsrUint8                *servCapData, CsrUint16               servCapDataLen)
{
        
    CsrUint8 *servCap;
	CsrUint8 servCapLen = 0;
    CsrBool  found= FALSE;
    CsrUint16 index = 0;
#ifdef _DEBUG
        {
            CsrUint8 i;
            
            printf( "My btAVsetCodeConfig (length=%i): ", servCapLen );
          
            for( i = 0 ; i < servCapLen ; i++ )
            {
                printf( "0x%02X ", servCapData[ i ] );
            }
            
            printf(" \n");
        }

       
#endif
	
	while(!found)
    {
        /* Will not free pointers */
        servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_MEDIA_CODEC,
                                  servCapData,
                                  servCapDataLen,
                                  &index);
        servCapLen = servCapDataLen - (CsrUint8)(((CsrUint32)servCap) - (CsrUint32)servCapData);

        if(servCap != NULL)
        {
            CsrUint8 *conf;
            CsrUint8 conf_len;

            /* Feed the remote capabilities to the codec to get the
             * optimal "configuration". Must *not* free servCap pointer */

            conf = filtersRemoteCaps(instData,
                                     servCap,
                                     servCapLen,
                                     &conf_len);
           
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile servCap"));

            if(conf_len > 0)
            {
                /* Update the codec settings locally */
                if (filtersSetConfig(instData, conf, conf_len) != CSR_BT_AV_ACCEPT)
                {
                   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring codec (filter set config error)"));
                }

				

                /* Open filter to activate new configuration */
                if(!filtersRun(instData, FILTER_OPEN))
                {
                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error selecting optimal configuration (filter open error)"));
                }
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CsrBtAvSetConfigReqSend reqest"));

                /* This function will copy the pointers "conf" and
                 * "prim->servCapData" */
                 
                CsrBtAvSetConfigReqSend(instData->connectionId,
                                   ASSIGN_TLABEL,
                                   instData->remoteSeid[instData->remoteSeidIndex],
                                   instData->localSeid[instData->localSeidIndex],
                                   conf_len,
                                   conf);

                 CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

                found = TRUE;
            }
            else
            {
                CsrPmemFree(conf);
            }
        }
        else
        {
            break;
        }
    }
	return found;
}
#endif
const char *getAvServerCapString(CsrUint8 cap)
{
    /* Service capabilities string */
    const char *servcap_str[NUM_AV_CAPABILITIES] =
        {
            "***unknown***",
            "Media transport",
            "Reporting",
            "Recovery ",
            "Content protection",
            "Header compression",
            "Multiplexing",
            "Media codec",
            "Delay Reporting",
        };
    return servcap_str[cap];
}


void handleAv2Prim(Av2AppInst  *instData, CsrPrim    *thePrim)
{

	switch (*thePrim)
	{
        /* The first few primitives are so trivial that
         * we handle them directly in the message handler
 */

        case CSR_BT_AV_DEACTIVATE_CFM:
            {
				 //CsrBtAvDeactivateCfm *prim=(CsrBtAvDeactivateCfm*)thePrim;

           		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "AV profile deactivated..."));
				break;
        	}

        case CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM:
            {
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM"));
               
                break;
            }

        /* From here, we divert all primitive handling into
         * special functions to keep the code clean and easy to
         * read
 */

        case CSR_BT_AV_ACTIVATE_CFM:
            {
				CsrBtAvActivateCfm *prim = (CsrBtAvActivateCfm*)thePrim;

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{

					CsrAppServerAvActivateCfm();
					instData->state =SsAvStateAvDisconnected;

					instData->avIsActivated = TRUE;

					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile activated..."));
					if (instData->filter_count == 0)
					{
					#ifdef EXCLUDE_CSR_BT_SS_CODEC
					    filtersInstall(instData);
					#else
						filtersInstall(instData,SBC_CODEC_TYPE);
					#endif
					}
				}
				else
				{
				/* If activation failed, make sure we're in non-activated state */
				// instData->max_num_counter++;
				  instData->avIsActivated = FALSE;
				  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile error..."));
				}
				break;
			}

        case CSR_BT_AV_STREAM_DATA_IND:
            {
                CsrBtAvStreamDataInd *prim = (CsrBtAvStreamDataInd*)thePrim;
				
				if(instData->streamHandle==prim->shandle)
				{
					if(instData->state != SsAvStateAvConnectedStreaming)
					{
					    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_STREAM_DATA_IND error..."));
						CsrPmemFree(prim->data);
						return;
					}

					/* Data received -- send it to the first filter */
	               instData->filters[0]->f_process(0,
	                                        instData->filters,
	                                        TRUE,
	                                        prim->data,
	                                        prim->length);
				}
				else
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV stream handle error..."));

				}
                break;
            }
#if 0
        case CSR_BT_AV_QOS_IND:
            {
               // CsrBtAvQosInd *prim= (CsrBtAvQosInd*)thePrim;

                break;
            }
#endif
        case CSR_BT_AV_CONNECT_IND:
			{
				CsrBtAvConnectInd *prim = (CsrBtAvConnectInd*)thePrim;

				instData->connectionId=prim->connectionId;
				instData->state =SsAvStateAvConnected;
				instData->remoteAddr = prim->deviceAddr;
				
				CsrAppServerAvConnetInd(&(prim->deviceAddr));

				CsrBtCmRoleDiscoveryReqSend(instData->qid,
				                     prim->deviceAddr);
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_CONNECT_IND"));
				 /* Initialise filters */
				         /* Initialise filters */
#ifndef EXCLUDE_CSR_BT_SS_CODEC		 
				 if( instData->installedCodec !=SBC_CODEC_TYPE)
				 {
					  filtersKill(instData);
					  filtersInstall(instData,SBC_CODEC_TYPE);
				 }
#endif
				if(!filtersRun(instData, FILTER_INIT))
				{
				   CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_CONNECT_IND filter err"));

				}

			}
            break;

        case CSR_BT_AV_CONNECT_CFM:
            {
                CsrBtAvConnectCfm *prim = (CsrBtAvConnectCfm*)thePrim;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_CONNECT_CFM"));
					instData->state =SsAvStateAvConnected;

				    instData->connectionId=prim->connectionId;
					instData->btConnId     = prim->btConnId;
        			instData->remoteAddr         = prim->deviceAddr;
#ifdef CSR_USE_DSPM
			       	CsrBtAvLpNegConfigReqSend(FALSE);
#endif
                   // CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CsrBtCmRoleDiscoveryReqSend"));
			        //CsrBtCmRoleDiscoveryReqSend(instData->qid,
			        //                     prim->deviceAddr);
#ifndef EXCLUDE_CSR_BT_SS_CODEC			        
			        if( instData->installedCodec !=SBC_CODEC_TYPE)
				    {
						 filtersKill(instData);
						 filtersInstall(instData,SBC_CODEC_TYPE);
					}
#endif					
					filtersRun(instData, FILTER_OPEN);
        			filtersRun(instData, FILTER_CLOSE);

        			CsrBtAvDiscoverReqSend(instData->connectionId,
                          	ASSIGN_TLABEL);
			        /* Initialise filters */
			        if(!filtersRun(instData, FILTER_INIT))
			        {
			         	CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"filter Run err"));
			        }
			    }
			    else
			    {
			        instData->state =SsAvStateAvDisconnected;
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_CONNECT_CFM err avResultSupplier[%x] avResultCode [%x]",prim->avResultSupplier, prim->avResultCode));
			        if (instData->avIsActivated == FALSE)
			        {
			        	CsrBtAvActivateReqSend(instData->qid, CSR_BT_AV_AUDIO_SINK);
			        }
			    
			    }
				CsrAppServerAvConnetCfm(&(prim->deviceAddr),prim->avResultCode,prim->avResultSupplier);

                break;
            }
		

        case CSR_BT_AV_DISCOVER_CFM:
			{
				CsrBtAvDiscoverCfm *prim=(CsrBtAvDiscoverCfm*)thePrim;
				CsrBtAvSeidInfo seidInfo;
				CsrUint8 i;

				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_DISCOVER_CFM"));

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS && prim->connectionId== instData->connectionId)
				{
					

					for(i=0; i<MAX_NUM_REMOTE_SEIDS; i++)
					{
					    instData->remoteSeid[i] = 0;
					}
					
		            instData->remoteSeidIndex = 0;
#ifndef EXCLUDE_CSR_BT_SS_CODEC						
					instData->remoteSBCLen =0;
#endif
					/* Check availability of audio stream endpoint and select seid */
					for(i=0; (i<prim->seidInfoCount) && (i<MAX_NUM_REMOTE_SEIDS); i++)
					{
					   
					    CsrMemCpy(&seidInfo,
					           &(prim->seidInfo[i]),
					           sizeof(CsrBtAvSeidInfo));
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"The remote device seid %d inuse %d",seidInfo.acpSeid, seidInfo.inUse));

					    /* Looking for an audio sink not in use */
					    if((seidInfo.mediaType == CSR_BT_AV_AUDIO)
					       && (seidInfo.inUse == FALSE))
					    {
					        if ( seidInfo.sepType == CSR_BT_AV_SOURCE)
					        {
					            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile add seid"));
					            instData->remoteSeid[instData->remoteSeidIndex] = seidInfo.acpSeid;
					            instData->remoteSeidIndex++;
								
					        }
					    }
					}
#ifndef EXCLUDE_CSR_BT_SS_CODEC						
					instData->remoteSeidMax=instData->remoteSeidIndex;
#endif
					if(instData->remoteSeid[0] != 0)
					{
					    /* Ask for the end-points capabilities */

					    CsrBtAvGetCapabilitiesReqSend(instData->connectionId,
					                             instData->remoteSeid[0],
					                             (instData->avTLabel++ % 16));						
						instData->remoteSeidIndex=0;
		
					}
					else
					{
						CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"The remote device does not have available audio end-points %d",instData->remoteSeid[0]));
					}
				}
				else
				{

					CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_DISCOVER_CFM err avResultSupplier[%x] avResultCode [%x]",prim->avResultSupplier, prim->avResultCode));
				}
				CsrPmemFree(prim->seidInfo);
				break;
			}

        case CSR_BT_AV_DISCOVER_IND:
			{

				CsrBtAvDiscoverInd *prim;
				CsrBtAvSeidInfo *seidInfo = CsrPmemAlloc(sizeof(CsrBtAvSeidInfo)*MAX_NUM_LOCAL_SEIDS);
				CsrUint8 iter;

				prim = (CsrBtAvDiscoverInd*)thePrim;

				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_DISCOVER_IND"));


				for(iter = 0; iter < MAX_NUM_LOCAL_SEIDS; ++iter)
				{
					seidInfo[iter].acpSeid = instData->localSeid[iter];
					seidInfo[iter].inUse = instData->localSeidInUse[iter];
					seidInfo[iter].mediaType = CSR_BT_AV_AUDIO;
				    seidInfo[iter].sepType = CSR_BT_AV_SINK;
				}
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV seidInfo seidInfo %d",seidInfo->inUse));

				CsrBtAvDiscoverResAcpSend(prim->connectionId,
				             prim->tLabel,
				             MAX_NUM_LOCAL_SEIDS,   //seid cnt
				             seidInfo);
				break;
			}

        case CSR_BT_AV_GET_CAPABILITIES_CFM:
#ifdef EXCLUDE_CSR_BT_SS_CODEC				
			{
                CsrBtAvGetCapabilitiesCfm *prim=(CsrBtAvGetCapabilitiesCfm*)thePrim;
   


			    CsrUint8 *servCap;
			    CsrUint8 servCapLen = 0;
			    CsrBool found = FALSE;
			    CsrUint16 index = 0;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			       
				   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_GET_CAPABILITIES_CFM\n"));


			        /* Loop through all media codec service descriptors */
			        while(!found)
			        {
			            /* Will not free pointers */
			            servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_MEDIA_CODEC,
			                                      prim->servCapData,
			                                      prim->servCapLen,
			                                      &index);
			            servCapLen = prim->servCapLen - (CsrUint8)(((CsrUint32)servCap) - (CsrUint32)prim->servCapData);

			            if(servCap != NULL)
			            {
			                CsrUint8 *conf;
			                CsrUint8 conf_len;

			                /* Feed the remote capabilities to the codec to get the
			                 * optimal "configuration". Must *not* free servCap pointer */

			                conf = filtersRemoteCaps(instData,
			                                         servCap,
			                                         servCapLen,
			                                         &conf_len);
			               
						   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile servCap\n"));

			                if(conf_len > 0)
			                {
			                    /* Update the codec settings locally */
			                    if (filtersSetConfig(instData, conf, conf_len) != CSR_BT_AV_ACCEPT)
			                    {
			                       CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring codec (filter set config error)\n"));
			                    }

								

			                    /* Open filter to activate new configuration */
			                    if(!filtersRun(instData, FILTER_OPEN))
			                    {
			                        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error selecting optimal configuration (filter open error)\n"));
			                    }
								CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_GET_CAPABILITIES_CFM filtersRun\n"));

			                    /* This function will copy the pointers "conf" and
			                     * "prim->servCapData" */
			                     
			                    CsrBtAvSetConfigReqSend(instData->connectionId,
			                                       ASSIGN_TLABEL,
			                                       instData->remoteSeid[instData->remoteSeidIndex],
			                                       instData->localSeid[instData->localSeidIndex],
			                                       conf_len,
			                                       conf);

			                     CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			                    found = TRUE;
			                }
			                else
			                {
			                    CsrPmemFree(conf);
			                }
			            }
			            else
			            {
			                break;
			            }
			        }

			        if(!found)
			        {
			            instData->remoteSeidIndex++;
			            if((instData->remoteSeidIndex < MAX_NUM_REMOTE_SEIDS) &&
			               (instData->remoteSeid[instData->remoteSeidIndex] != 0) )
			            {
			                /* Ask for the next end-point capabilities */
			               // CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,"Requesting additional remote capabilities...\n");
			                CsrBtAvGetCapabilitiesReqSend(instData->connectionId,
			                                         instData->remoteSeid[instData->remoteSeidIndex],
			                                         ASSIGN_TLABEL);

			                CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			            }
			            else
			            {
			                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Get capabilities response does not contain supported media/codec type(s)\n"));
#ifdef DEBUG
			                if(prim->servCapData)
			                {
			                    CsrUint8 i;
			                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Raw capabilities (len=%i): ", prim->servCapLen));
			                    for(i=0; i<prim->servCapLen; i++)
			                    {
			                        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"0x%02X ", prim->servCapData[i]));
			                    }
			                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"\n"));
			                }
#endif
			            }
			        }
			    }
			    else
			    {
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CAPABILITIES_CFM err %x",prim->avResultCode));

			    }

			    /* Free primitive members */
			    if(prim)
			    {
			        CsrPmemFree(prim->servCapData);
			    }
                break;
            }
#else
            {
                CsrBtAvGetCapabilitiesCfm *prim=(CsrBtAvGetCapabilitiesCfm*)thePrim;
			    CsrBool found = FALSE;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			       
				   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_GET_CAPABILITIES_CFM"));

				   if((prim->servCapData[4]>>4==CSR_BT_AV_AUDIO) &&(prim->servCapData[5]==CSR_BT_AV_NON_A2DP_CODEC)&&(prim->servCapData[6]==SS_VENDOR_ID)) 
				   {
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"found ss codec\n" ));   
			
					     if( instData->installedCodec !=SS_CODEC_TYPE)
						 {
							 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"set ss codec\n" )); 				 
							 filtersKill(instData);
							 filtersInstall(instData,SS_CODEC_TYPE);
							 if( !filtersRun( instData, FILTER_INIT) )
							 {
								CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring FILTER_INIT error\n" ));                        
							 }
					
						}
						 
						instData->localSeidIndex=INDEX_LOCAL_SS_SEID;
						 if(btAVsetCodeConfig(instData,prim->servCapData,prim->servCapLen))
						 {
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"succes set request config ss codec\n" )); 				 
							found=TRUE;
						 }
					   
				   }
				   else	if((prim->servCapData[4]>>4==CSR_BT_AV_AUDIO) &&(prim->servCapData[5]==CSR_BT_AV_SBC))
				   {
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"found sbc codec\n" ));  /*store sbc codec*/
						if(instData->remoteSBCLen ==0)
						{

							instData->remoteSeidSBCIndex=instData->remoteSeidIndex ;
							CsrMemCpy(instData->remoteSBCConf,prim->servCapData,prim->servCapLen);
							instData->remoteSBCLen=prim->servCapLen;
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"remoteSeidSBCIndex %d\n" , instData->remoteSeidSBCIndex));     
						}
						
			    	}
				   
				    if(!found)
				    {
				       if((instData->remoteSeidIndex==instData->remoteSeidMax-1) ||
						(instData->remoteSeidIndex==MAX_NUM_REMOTE_SEIDS-1))                         /*setting sbc codec*/
						{
							if( instData->installedCodec !=SBC_CODEC_TYPE)
							{
								CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"set sbc codec\n" )); 
								filtersKill(instData);
								filtersInstall(instData,SBC_CODEC_TYPE);
								if( !filtersRun( instData, FILTER_INIT) )
								{
									CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring FILTER_INIT error\n" ));                       
								}
							}

							instData->remoteSeidIndex=instData->remoteSeidSBCIndex;
							instData->localSeidIndex=INDEX_LOCAL_SBC_SEID;

							if(btAVsetCodeConfig(instData,instData->remoteSBCConf,instData->remoteSBCLen))
							{
							     CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"succes set request config sbc codec\n" ));				 
							 	 found=TRUE;
							}
						}
				    }
				   
			        /* Loop through all media codec service descriptors */
			        if(!found)
			        {
			            instData->remoteSeidIndex++;
			            if((instData->remoteSeidIndex < MAX_NUM_REMOTE_SEIDS) &&
			               (instData->remoteSeid[instData->remoteSeidIndex] != 0) )
			            {
			                /* Ask for the next end-point capabilities */
			                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Requesting additional remote capabilities..."));
			                CsrBtAvGetCapabilitiesReqSend(instData->connectionId,
			                                         instData->remoteSeid[instData->remoteSeidIndex],
			                                         ASSIGN_TLABEL);

			                CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			            }
			            else
			            {
			                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Get capabilities response does not contain supported media/codec type(s)"));
#ifdef DEBUG
			                if(prim->servCapData)
			                {
			                    CsrUint8 i;
			                    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Raw capabilities (len=%i): ", prim->servCapLen));
			                    for(i=0; i<prim->servCapLen; i++)
			                    {
			                        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"0x%02X ", prim->servCapData[i]));
			                    }
			                }
#endif
			            }
			        }
			    }
			    else
			    {
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CAPABILITIES_CFM err avResultSupplier[%x] avResultCode [%x]",prim->avResultSupplier, prim->avResultCode));
			    }

			    /* Free primitive members */
			    if(prim)
			    {
			        CsrPmemFree(prim->servCapData);
			    }
                break;
            }
#endif
        case CSR_BT_AV_STATUS_IND:
            {
               // CsrBtAvStatusInd *prim= (CsrBtAvStatusInd*)thePrim;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_STATUS_IND"));
                break;
            }

        case CSR_BT_AV_GET_ALL_CAPABILITIES_IND:
        case CSR_BT_AV_GET_CAPABILITIES_IND:
            {
                CsrBtAvGetCapabilitiesInd *prim= (CsrBtAvGetCapabilitiesInd*)thePrim;
			    CsrUint8 *caps;
			    CsrUint8 caps_len;
				CsrBool found=FALSE;
				
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CAPABILITIES_IND | CSR_BT_AV_GET_ALL_CAPABILITIES_IND receved"));
#ifdef EXCLUDE_CSR_BT_SS_CODEC	
                if(instData->localSeid[instData->localSeidIndex] == prim->acpSeid)
					found=TRUE;		
#else

				for( instData->localSeidIndex=0;instData->localSeidIndex<MAX_NUM_LOCAL_SEIDS;instData->localSeidIndex++)
				{
				
					if( instData->localSeid[instData->localSeidIndex]==prim->acpSeid)
					{
						 if( instData->installedCodec !=prim->acpSeid)
					     {
							filtersKill(instData);
							filtersInstall(instData,prim->acpSeid);
								 /* Initialise filters */
							if( !filtersRun( instData, FILTER_INIT ) )
							{        
								CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CAPABILITIES_IND Error initialising filters!\n"));            
							}
							else
							{
								  found=TRUE;
							}
						}
						else
						{
							 found=TRUE;
						}
					    break;
					}
						
				}
#endif
			    caps = filtersGetCaps(instData, &caps_len);

			    if ((caps) &&
			        (found)&&(prim->connectionId ==instData->connectionId))
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Remote side requested capabilities, sending response"));
			        if (prim->type == CSR_BT_AV_GET_CAPABILITIES_IND)
			        {
			            CsrBtAvGetCapabilitiesResAcpSend(prim->connectionId,
			                                        prim->tLabel,
			                                        caps_len,
			                                        caps);
			        }
			        else
			        {
			            CsrBtAvGetAllCapabilitiesResAcpSend(prim->connectionId,
			                                        prim->tLabel,
			                                        caps_len,
			                                        caps);
			        }
			    }
			    else
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"GetCapabilities command rejected - note that capabilities is not supported "));
			        if (prim->type == CSR_BT_AV_GET_CAPABILITIES_IND)
			        {
			            CsrBtAvGetCapabilitiesResRejSend(prim->connectionId,
			                                        prim->tLabel,
			                                        CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
			        }
			        else
			        {
			            CsrBtAvGetAllCapabilitiesResRejSend(prim->connectionId,
			                                        prim->tLabel,
			                                        CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
			        }
			    }
                break;
            }

        case CSR_BT_AV_SET_CONFIGURATION_CFM:
            {
                CsrBtAvSetConfigurationCfm *prim= (CsrBtAvSetConfigurationCfm*)thePrim;

				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_SET_CONFIGURATION_CFM"));

		
			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			        instData->streamHandle = prim->shandle;
			

			        /* Open the filters so they are ready to begin processing */
			        if(filtersRun(instData, FILTER_OPEN))
			        {
			            /* Ready to open stream */
			            instData->localSeidInUse[instData->localSeidIndex] = TRUE;
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_SET_CONFIGURATION_CFM instData->localSeidIndex[%d] instData->localSeidInUse[instData->localSeidIndex] [%d] \n",instData->localSeidIndex, instData->localSeidInUse[instData->localSeidIndex]));
						CsrAppServerAvConfigCfm(instData->pcm_freq,instData->pcm_channels);
			            CsrBtAvOpenReqSend(instData->streamHandle, ASSIGN_TLABEL);
			        }
			        else
			        {
			            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unable to configure stream end-point (filter open error)"));
			        }
			    }
			    else
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Set configuration rejected, error code 0x%x (0x%04x):",
			               prim->avResultCode,
			               prim->avResultSupplier));

			    }
                break;
            }

        case CSR_BT_AV_SET_CONFIGURATION_IND:
            {
                CsrBtAvSetConfigurationInd *prim=(CsrBtAvSetConfigurationInd*)thePrim;

			    CsrBtAvResult result;
			    CsrUint16 index;
			    CsrUint8 *servCap = NULL;
				CsrBool found=FALSE;
			
				if(prim->connectionId==instData->connectionId)
				{

			        instData->streamHandle = prim->shandle; /* get tmp. shandle */
			        servCap = prim->servCapData;

			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Configuration request received"));
#ifdef EXCLUDE_CSR_BT_SS_CODEC	
               		if(instData->localSeid[instData->localSeidIndex] == prim->acpSeid)
						found=TRUE;
#else						
					for( instData->localSeidIndex=0;instData->localSeidIndex<MAX_NUM_LOCAL_SEIDS;instData->localSeidIndex++)
					{
						if( instData->localSeid[instData->localSeidIndex]==prim->acpSeid)
						{
							if( instData->installedCodec !=prim->acpSeid)
							{
									filtersKill(instData);
									filtersInstall(instData,prim->acpSeid);
										 /* Initialise filters */
									if( !filtersRun( instData, FILTER_INIT ) )
									{        
										CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error initialising filters!\n"));            
									}
								else
								{
									  found=TRUE;
								}
							}
							else
							{
								 found=TRUE;
							}
							break;	
						}
					}
#endif
			        if(found)
			        {
			            if(instData->localSeidInUse[instData->localSeidIndex] == FALSE)
			            {
			                index = 0;
			                result = CSR_BT_AV_ACCEPT;

			                /* Parse services */
			                while(servCap != NULL)
			                {
			                    if(*servCap == CSR_BT_AV_SC_MEDIA_CODEC)
			                    {
			                        /* Feed the config to the codec */
			                        result = filtersSetConfig(instData, servCap, 0);
			                        if(result != CSR_BT_AV_ACCEPT)
			                        {
			                            break;
			                        }
			                    }
			                    else if(*servCap == CSR_BT_AV_SC_CONTENT_PROTECTION)
			                    {
			                        result = CsrBtAvValidateServiceCap(servCap);
			                        if(result != CSR_BT_AV_ACCEPT)
			                        {
			                            break;
			                        }
			                    }
			                    else
			                    {
			                        result = CsrBtAvValidateServiceCap(servCap);
			                        if(result != CSR_BT_AV_ACCEPT)
			                        {
			                            break;
			                        }
			                    }
			                    servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_NEXT,
			                                              prim->servCapData,
			                                              prim->servCapLen,
			                                              &index);
			                }

			                /* We can accept the proposed configuration */
			                if(result == CSR_BT_AV_ACCEPT)
			                {
			                    index = 0;
			                    servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_MEDIA_CODEC,
			                                              prim->servCapData,
			                                              prim->servCapLen,
			                                              &index);

			                    /* Open filters so they prepare the new accepted config */
			                    if(filtersRun(instData, FILTER_OPEN))
			                    {
			                        instData->localSeidInUse[instData->localSeidIndex] = TRUE;
			                        CsrBtAvSetConfigResAcpSend(prim->shandle,
			                                              prim->tLabel);
			                        CsrPmemFree(prim->servCapData);
			                        //CsrAppBacklogReqSend(TECH_BT, PROFILE_AV, FALSE,"Configuration has been accepted");

			                        CsrBtAvDelayReportReqSend(50,prim->connectionId,ASSIGN_TLABEL);
									CsrAppServerAvConfigInd(instData->pcm_freq,instData->pcm_channels);

			                        return;
			                    }
			                    else
			                    {
			                       CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"filtersRun open err"));
			                        /* If we fail to initialize filters, some parameters must be wrong */
			                        result = CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT;
			                    }
			                }
							else
						    {
						      CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"not accept"));
							}
			            }
			            else
			            {
			                CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"localSeidInUse err"));
			                result = CSR_BT_RESULT_CODE_A2DP_SEP_IN_USE;
			            }
			        }
			        else
			        {
			            CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"localSeid err"));
			            result = CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID;
			        }
				}
				else
				{
				     CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_MAX_NUM_CONNECTIONS err"));
					result = CSR_BT_AV_MAX_NUM_CONNECTIONS;
				}


			    /* Reaching here means configuration can not be approved, send reject response */
			    CsrBtAvSetConfigResRejSend(instData->streamHandle,
			                          prim->tLabel,
			                          result,
			                          *servCap);

			    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Configuration has been rejected (Error code: 0x%x)", result));

			    /* Stream handle not valid anymore */
			    instData->streamHandle = 0;
			    CsrPmemFree(prim->servCapData);
                break;
            }

        case CSR_BT_AV_OPEN_CFM:
            {
                CsrBtAvOpenCfm *prim=(CsrBtAvOpenCfm*)thePrim;
     

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
				    
				   CsrBool ss_filter_support=FALSE;
#ifndef EXCLUDE_CSR_BT_SS_CODEC				   
				   CsrUint8 index;

                   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_OPEN_CFM instData->localSeidIndex [%d] instData->localSeidInUse[ instData->localSeidIndex ]  [%d]", instData->localSeidIndex, instData->localSeidInUse[ instData->localSeidIndex ]));
			       if( instData->localSeidInUse[ instData->localSeidIndex ] == FALSE )
					{
				      for(index=0;index<MAX_NUM_LOCAL_SEIDS;index++)
		              {
				         if( instData->localSeidInUse[index] == TRUE )
				         {
							CsrUint8 codecIdx=SBC_CODEC_TYPE;

							if(index==INDEX_LOCAL_SS_SEID)
							{
							  codecIdx=SS_CODEC_TYPE;

							}
						    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_OPEN_CFM codec %d\n",codecIdx ));                        
							if(codecIdx !=instData->installedCodec) 
							{
								filtersKill(instData);
								filtersInstall(instData,codecIdx);
								if( !filtersRun( instData, FILTER_INIT) )
								{
									CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error CSR_BT_AV_OPEN_IND FILTER_INIT error\n" ));                        
								}
								if(!filtersRun( instData, FILTER_OPEN ) )
								{
									CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error CSR_BT_AV_OPEN_IND FILTER_OPEN error\n" ));               
								}
							}
							instData->localSeidIndex=index;
							break;
				         }
		              }
					 }
					
			 	    if(instData->installedCodec==SS_CODEC_TYPE)
						ss_filter_support=TRUE;
#endif		
					CsrAppServerOpenCfm(ss_filter_support);
					
			        CsrBtAvGetStreamChannelInfoReqSend(prim->shandle);
			        if(instData->streamHandle == prim->shandle)
			        {
			            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Stream opened"));
			            instData->state = SsAvStateAvConnectedOpen;
			        }
					else
					{
					  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Stream opened invalid handle [%x] [%x]",instData->streamHandle,prim->shandle));
					}
			    }
			    else
			    {
			         CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Open stream rejected, error code 0x%04x (0x%04x)",
			               prim->avResultCode,
			               prim->avResultSupplier));
			        
			    }
                break;
            }

        case CSR_BT_AV_OPEN_IND:
            {
                CsrBtAvOpenInd *prim = (CsrBtAvOpenInd*)thePrim;;
				CsrBool ss_filter_support=FALSE;

			    if( instData->streamHandle == prim->shandle)
			    {
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_OPEN_IND"));
			        
			        CsrBtAvGetStreamChannelInfoReqSend(prim->shandle);
			        CsrBtAvOpenResAcpSend(prim->shandle, prim->tLabel);
					instData->state = SsAvStateAvConnectedOpen;

			      //  instData->localSeidInUse = TRUE;
#ifndef EXCLUDE_CSR_BT_SS_CODEC			      
			      if( instData->localSeidInUse[ instData->localSeidIndex ] == FALSE )
			      {
			             CsrUint8 index;
				      for(index=0;index<MAX_NUM_LOCAL_SEIDS;index++)
		              {
				         if( instData->localSeidInUse[index] == TRUE )
				         {
							CsrUint8 codecIdx=SBC_CODEC_TYPE;

							if(index==INDEX_LOCAL_SS_SEID)
							{
							  codecIdx=SS_CODEC_TYPE;

							}
						    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_OPEN_IND codec %d\n",codecIdx ));                        
							if(codecIdx !=instData->installedCodec) 
							{
								filtersKill(instData);
								filtersInstall(instData,codecIdx);
								if( !filtersRun( instData, FILTER_INIT) )
								{
								        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error CSR_BT_AV_OPEN_IND FILTER_INIT error\n" ));                        
								}
								if(!filtersRun( instData, FILTER_OPEN ) )
								{
								        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error CSR_BT_AV_OPEN_IND FILTER_OPEN error\n" ));               
								}
							}
							instData->localSeidIndex=index;
							break;
				         }
		              }
				   }
				  
				  if(instData->installedCodec==SS_CODEC_TYPE)
					ss_filter_support=TRUE;
#endif		
					CsrAppServerOpenInd(ss_filter_support);
			    }
			    else
			    {
			        CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_OPEN_IND err [%d] [%d]\n", instData->streamHandle,prim->shandle));   
			        CsrBtAvOpenResRejSend(instData->streamHandle,
			                         prim->tLabel,
			                         CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
			    }
                break;
            }

        case CSR_BT_AV_START_CFM:
			{
			    CsrBtAvStartCfm *prim=(CsrBtAvStartCfm*)thePrim;

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_START_CFM"));
					instData->state = SsAvStateAvConnectedStreaming;

					if(!filtersRun(instData, FILTER_START))
					{
						CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Could not start stream (filter start error)"));
					}
				}
				else
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Start stream rejected, error code 0x%04x (0x%04x): ",
					   prim->avResultCode,
					   prim->avResultSupplier));
				}
			    break;
			}

        case CSR_BT_AV_START_IND:
            {
                CsrBtAvStartInd *prim=(CsrBtAvStartInd*)thePrim;

			    CsrBtAvResult result;
			    CsrUint8 i;
			    CsrUint8 shandle = 0;
			  
			    result = CSR_BT_AV_ACCEPT;

			    for (i=0; i<prim->listLength; i++)
			    {
			        shandle = prim->list[i];

			        if(shandle == instData->streamHandle)
			        {
			            if(instData->state == SsAvStateAvConnectedOpen)
			         	{
			                /* Prepare (open) again as MTU size might be updated since
			                 * last time. It should not fail since the SBC
			                 * parameters have already been checked when this
			                 * function was called first time */
			                 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_START_IND"));
			                if(!filtersRun(instData, FILTER_OPEN))
			                {
			                    
								CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"filtersRun open err"));

			                    break;
			                }
			            }
			            else
			            {
			                 CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_START_IND status err %d",instData->state));
			                result = CSR_BT_RESULT_CODE_A2DP_BAD_STATE;
			                break;
			            }
			        }
			        else
			        {
			            CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_START_IND shandle err"));
			            result = CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID;
			            break;
			        }
			    }

			    if ((result == CSR_BT_AV_ACCEPT) &&
			       (i == prim->listLength))
			    {
					CsrBtAvStartResAcpSend(prim->tLabel,
					                  prim->listLength,
					                  prim->list);
					instData->state = SsAvStateAvConnectedStreaming;
					CsrAppServerAvStartInd();


					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_START_IND run"));

					/* Now, start the filters */
					if(!filtersRun(instData, FILTER_START))
					{
					    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Unable to start stream (filter start error)"));
					}
			        
			    }
			    else
			    {
			        CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_START_IND reject send"));
			        CsrBtAvStartResRejSend((CsrUint8)shandle,
			                          (CsrUint8)prim->tLabel,
			                          (CsrBtAvResult)result,
			                          prim->listLength,
			                          prim->list);
			    }
                break;
            }

        case CSR_BT_AV_CLOSE_CFM:
            {
                CsrBtAvCloseCfm *prim=(CsrBtAvCloseCfm*)thePrim;
				CsrBool result=FALSE;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			        result=TRUE;
			        instData->state = SsAvStateAvConnected;
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_CLOSE_CFM receved"));

			        instData->localSeidInUse[instData->localSeidIndex] = FALSE;
			    }
			    else
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Close stream rejected, error code 0x%04x (0x%04x)",
			               prim->avResultCode,
			               prim->avResultSupplier));
			    }
				CsrAppServerCloseCfm(result);
                break;
            }

        case CSR_BT_AV_CLOSE_IND:
			{
				CsrBtAvCloseInd *prim=(CsrBtAvCloseInd*)thePrim;
				
				if(prim->shandle == instData->streamHandle)
				{
					instData->state = SsAvStateAvConnected;
					instData->localSeidInUse[instData->localSeidIndex] = FALSE;
					CsrBtAvCloseResAcpSend( prim->shandle, prim->tLabel );
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_CLOSE_IND receved"));

					if(!filtersRun(instData, FILTER_CLOSE))
					{
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error closing stream (filter stop error)"));

					}
					CsrAppServerCloseInd();

				}
				else
				{
				    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_CLOSE_IND reject send"));
					CsrBtAvCloseResRejSend(prim->shandle,
					          prim->tLabel,
					          CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
				}
				break;
			}

        case CSR_BT_AV_SUSPEND_CFM:
            {
                CsrBtAvSuspendCfm *prim=(CsrBtAvSuspendCfm*)thePrim;


			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {

	                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_SUSPEND_CFM receved"));
			        if(instData->state == SsAvStateAvConnectedStreaming)
			        {
			            if(!filtersRun(instData, FILTER_STOP))
			            {
			                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error suspending stream (filter stop error)"));
			            }
						instData->state = SsAvStateAvConnectedOpen;
			        }
			    }
			    else
			    {
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_SUSPEND_CFM err %x",prim->avResultCode));  
			    }
                break;
            }

        case CSR_BT_AV_SUSPEND_IND:
            {
                CsrBtAvSuspendInd *prim= (CsrBtAvSuspendInd*)thePrim;
	
			    CsrUint8 i;
			    CsrUint8 shandle;
			    CsrBool freePtr = TRUE;

			    for(i=0; i<prim->listLength; i++)
			    {
			        shandle = prim->list[i];

			        if(shandle == instData->streamHandle)
			        {
			            if(instData->state == SsAvStateAvConnectedStreaming)
			            {
			                if(i == prim->listLength - 1)
			                {
			                
			                    if(!filtersRun(instData, FILTER_STOP))
			                    {

									CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Stream suspended filter err"));
			                    }
								
								CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_SUSPEND_IND Stream suspended"));
								CsrBtAvSuspendResAcpSend((CsrUint8)prim->tLabel,
								prim->listLength,
								prim->list);
								freePtr = FALSE;
								instData->state = SsAvStateAvConnectedOpen;	
								CsrAppServerAvSuspendInd();
			                }
			            }
			            else
			            {
			                CsrBtAvSuspendResRejSend((CsrUint8)shandle,
			                                    (CsrUint8)prim->tLabel,
			                                    CSR_BT_RESULT_CODE_A2DP_BAD_STATE,
			                                    prim->listLength,
			                                    prim->list);
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Stream suspended not conntcedStreming"));
			                freePtr = FALSE;
			                break;
			            }
						
			       		
			        }
			        else
			        {
			            CsrBtAvSuspendResRejSend((CsrUint8)shandle,
			                                (CsrUint8)prim->tLabel,
			                                CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID,
			                                prim->listLength,
			                                prim->list);
						CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Stream suspended invalid handle"));
			            freePtr = FALSE;
			            break;
			        }
			    }
			    if (freePtr)
			    {
			        CsrPmemFree(prim->list);
			    }
                break;
            }

        case CSR_BT_AV_RECONFIGURE_CFM:
            {
                CsrBtAvReconfigureCfm *prim = (CsrBtAvReconfigureCfm *) thePrim;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_CFM receved"));
			        CsrAppServerAvConfigCfm(instData->pcm_freq,instData->pcm_channels);
			        /* New configuration was accepted, so "open" filter again */
			        if(!filtersRun(instData, FILTER_OPEN))
			        {
			            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_IND filter open err"));
			        }
			    }
			    else
			    {
				   CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_IND err avResultCode %x",prim->avResultCode));
			    }

			    CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);
 
                break;
            }

        case CSR_BT_AV_RECONFIGURE_IND:
            {
                CsrBtAvReconfigureInd *prim=(CsrBtAvReconfigureInd*)thePrim;
			    CsrBtAvResult result;
			    CsrUint16 index;
			    CsrUint8 *servCap;
			   
			    servCap = prim->servCapData;

			    if(instData->streamHandle == prim->shandle)
			    {
			        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_IND receved"));
#ifndef EXCLUDE_CSR_BT_SS_CODEC					
			        if( instData->localSeidInUse[ instData->localSeidIndex ] == FALSE )
					{
				      for(index=0;index<MAX_NUM_LOCAL_SEIDS;index++)
		              {
				         if( instData->localSeidInUse[index] == TRUE )
				         {
				            CsrUint8 codecIdx=SBC_CODEC_TYPE;
						
				            if(index==INDEX_LOCAL_SS_SEID)
				            {
							   codecIdx=SS_CODEC_TYPE;
				            }

						    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"reconfigind codec %d\n",codecIdx ));                       
						    if(codecIdx !=instData->installedCodec)
							{  
								filtersKill(instData);
								filtersInstall(instData,codecIdx);
								if( !filtersRun( instData, FILTER_INIT) )
								{
								        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring FILTER_INIT error\n" ));                        
								}
								if(!filtersRun( instData, FILTER_OPEN ) )
								{
								        CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Error configuring FILTER_OPEN error\n" ));               
								}
							}
							instData->localSeidIndex=index;
							break;
				         }
		              }
					}
#endif					
			        if(instData->localSeidInUse[instData->localSeidIndex] == TRUE)
			        {
			          CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Reconfiguration indication received..."));

			            index = 0;
			            result = CSR_BT_AV_ACCEPT;

			            while(servCap != NULL)
			            {
			                if(*servCap == CSR_BT_AV_SC_MEDIA_CODEC)
			                {
			                    /* Feed the config to the codec */
			                    result = filtersSetConfig(instData,
			                                              servCap,
			                                              0);
			                    if (result == CSR_BT_AV_ACCEPT)
			                    {
			                        break;
			                    }
			                    else
			                    {
			                        result = CSR_BT_RESULT_CODE_A2DP_UNSUPPORTED_CONFIGURATION;
			                    }
			                }
			                else if(*servCap == CSR_BT_AV_SC_MEDIA_TRANSPORT)
			                {
			                    /* Media transport is not allowed in reconfigure */
			                    result = CSR_BT_RESULT_CODE_A2DP_INVALID_CAPABILITIES;
			                }
			                else if(*servCap > CSR_BT_AV_SC_MEDIA_CODEC)
			                {
			                    /* Not defined in spec */
			                    result = CSR_BT_RESULT_CODE_A2DP_BAD_SERV_CATEGORY;
			                }
			                else
			                {
			                    /* The rest is unsupported */
			                    result = CSR_BT_RESULT_CODE_A2DP_UNSUPPORTED_CONFIGURATION;
			                }
			                servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_NEXT,
			                                          prim->servCapData,
			                                          prim->servCapLen,
			                                          &index);
			            }
			        }
			        else
			        {
			            result = CSR_BT_RESULT_CODE_A2DP_SEP_NOT_IN_USE;
			        }

			        if (result == CSR_BT_AV_ACCEPT)
			        {
			            /* Open the filter to use the newly accepted config */
			            if(filtersRun(instData, FILTER_OPEN))
			            {
			                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Reconfiguration parameters OK, sending accept response"));
							CsrAppServerAvConfigInd(instData->pcm_freq,instData->pcm_channels);


			                CsrBtAvReconfigResAcpSend(prim->shandle,
			                                     prim->tLabel);

			                CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			            }
			            else
			            {
			                 CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"AV profile CSR_BT_AV_RECONFIGURE_IND filtersRun err"));
			                /* If we fail to initialize filters, some parameters must be wrong */
			                result = CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT;
			            }
			        }
			    }
			    else
			    {
			        CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_IND  streamHandle err "));
			        result = CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID;
			    }

			    /* Get original capabilities which must either be sent or freed */
			    if(prim->servCapData != NULL)
			    {
			        servCap = prim->servCapData;
			    }
			    else
			    {
			        servCap = NULL;
			    }

			    /* Send response in case of error, otherwise free the caps */
			    if(result != CSR_BT_AV_ACCEPT)
			    {
			         
                    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_RECONFIGURE_IND   err  %d",result));
			        CsrBtAvReconfigResRejSend(prim->shandle,
			                             prim->tLabel,
			                             result,
			                             *servCap);
			    }
			    else CsrPmemFree(servCap);
    
                break;
            }
		

        case CSR_BT_AV_GET_CONFIGURATION_CFM:
			{
				CsrBtAvGetConfigurationCfm *prim =(CsrBtAvGetConfigurationCfm*)thePrim;

				CsrUint16 index;
				CsrUint8 *servCap;
				
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CONFIGURATION_CFM"));

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{
				    CsrUint8 i;
					
					index = 0;
					servCap = prim->servCapData;

					while(servCap != NULL)
					{
					   if(*servCap < NUM_AV_CAPABILITIES)
					   {
						   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"%s (length=%i) : ",
								   getAvServerCapString(*servCap),
								   *(servCap+1)));
						}
						else
						{
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"%s (length=%i) : ",
								   getAvServerCapString(0),
								   *(servCap+1)));

						}
			
						for (i = 0; i < *(servCap + 1); i++)
						{
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"%02X ", *(servCap+2+i)));

						}
						servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_NEXT,
						                      prim->servCapData,
						                      prim->servCapLen,
						                      &index);
					}
					CsrPmemFree(prim->servCapData);
				}
				else
				{
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Error CSR_BT_AV_GET_CONFIGURATION_CFM %x",prim->avResultCode));


				}
				break;
			}

        case CSR_BT_AV_GET_CONFIGURATION_IND:
            {
				CsrBtAvGetConfigurationInd *prim = (CsrBtAvGetConfigurationInd*)thePrim;

				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CONFIGURATION_IND"));

				if(instData->localSeidInUse[instData->localSeidIndex] != 0)
				{
					CsrUint8 *conf,*ptr;
					CsrUint8 conf_len;
					conf = filtersGetConfig(instData, &conf_len);
					if (conf_len > 0)
					{
					    ptr =(CsrUint8 *) CsrPmemAlloc(conf_len);
					    CsrMemCpy(ptr,conf,conf_len);
					}
					else
					{
					    ptr = conf;
					}

					CsrBtAvGetConfigResAcpSend(prim->shandle,
					                      prim->tLabel,
					                      conf_len,
					                      ptr);
				}
				else
				{
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"send CsrBtAvGetConfigResRejSend"));
					CsrBtAvGetConfigResRejSend(prim->shandle,
					                      prim->tLabel,
					                      CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
				}


                break;
            }

        case CSR_BT_AV_ABORT_CFM:
            {
              //  CsrBtAvAbortCfm *prim= (CsrBtAvAbortCfm*)thePrim;
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_ABORT_CFM"));


				/* Close filters */
				if(!filtersRun(instData, FILTER_CLOSE))
				{
					CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Error aborting stream (filter close error)"));

					
				}
				instData->state = SsAvStateAvConnected;
				instData->localSeidInUse[instData->localSeidIndex] = FALSE;


				break;
            }

        case CSR_BT_AV_ABORT_IND:
            {
              //  CsrBtAvAbortInd *prim=(CsrBtAvAbortInd*)thePrim;
		
				instData->state = SsAvStateAvConnected;
			    instData->localSeidInUse[instData->localSeidIndex] = FALSE;

				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_ABORT_IND"));


                break;
            }

        case CSR_BT_AV_DISCONNECT_IND:
            {
				CsrBtAvDisconnectInd *prim=(CsrBtAvDisconnectInd*)thePrim;

				
				instData->localSeidInUse[instData->localSeidIndex] = FALSE;


				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_DISCONNECT_IND reasonSupplier[%x] reasonCode[%x]", prim->reasonSupplier,prim->reasonCode));
				if(prim->connectionId==instData->connectionId)
				{

					instData->state = SsAvStateAvDisconnected;
					/* Close filters */
					if (!filtersRun(instData, FILTER_CLOSE))
					{
					    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Error disconnecting (filter close error)"));
					   
					}

					/* Deinitialise filters */
					if (!filtersRun(instData, FILTER_DEINIT))
					{
					    CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0,"Error initialising filters!"));
					}
					if(prim->localTerminated)
						CsrAppServerAvDisconnectCfm(&(instData->remoteAddr),prim->reasonCode,prim->reasonSupplier);
					else	
						CsrAppServerAvDisconnectInd(&(instData->remoteAddr),prim->reasonCode,prim->reasonSupplier);
				}
				else
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"err CSR_BT_AV_DISCONNECT_IND"));
				}
                break;
            }

        case CSR_BT_AV_STREAM_MTU_SIZE_IND:
            {
                CsrBtAvStreamMtuSizeInd *prim = (CsrBtAvStreamMtuSizeInd*)thePrim;

			    if( (instData->maxFrameSize == 0) || (prim->remoteMtuSize < instData->maxFrameSize) )
			    {
			        instData->maxFrameSize = prim->remoteMtuSize;
			    }
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_STREAM_MTU_SIZE_IND"));

    
                break;
            }
        case CSR_BT_AV_SECURITY_CONTROL_IND:
            {
                CsrBtAvSecurityControlInd *prim = (CsrBtAvSecurityControlInd*)thePrim;

			    /* Testing af parameters is not implemented. An accept response will be sent in any case */
			    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Security Control request received. Sending accept response"));
			    CsrBtAvSecurityControlResAcpSend(prim->shandle, prim->tLabel, prim->contProtMethodLen, prim->contProtMethodData);


                break;
            }
        case CSR_BT_AV_SECURITY_CONTROL_CFM:
            {
                CsrBtAvSecurityControlCfm *prim = (CsrBtAvSecurityControlCfm*)thePrim;

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
					prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Security Control accepted by remote device"));
				}
				else
				{
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Security Control rejected by remote device"));
				}
				CsrPmemFree(prim->contProtMethodData);
		

                break;
            }
        case CSR_BT_AV_DELAY_REPORT_IND:
            {
           //     CsrBtAvDelayReportInd *prim=(CsrBtAvDelayReportInd *)thePrim;
           
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_DELAY_REPORT_IND:"));
               
                break;
            }
        case CSR_BT_AV_GET_CHANNEL_INFO_CFM:
            {
#ifdef CSR_USE_DSPM
                CsrBtAvGetChannelInfoCfm *prim = (CsrBtAvGetChannelInfoCfm *)thePrim;

                if (prim->resultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_AV_GET_CHANNEL_INFO_CFM:"));
                }
#endif
                break;
            }
        default:
            {
                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unhandled AV primitive: 0x%04x", *thePrim));
				CsrBtCmFreeUpstreamMessageContents(CSR_BT_AV_PRIM, thePrim);
            }
    }

}


/**************************************************************************************************
 * setMediaPayloadType
 **************************************************************************************************/
void setMediaPayloadType(CsrUint8 mpt)
{

    Av2Instance->mediaPayloadType = mpt;
}

/**************************************************************************************************
 * setTimestamp
 **************************************************************************************************/
void setTimestamp(CsrUint32 ts)
{

    Av2Instance->time_stamp = ts;
}

/**************************************************************************************************
 * getSampleBufferSize
 **************************************************************************************************/
void getSampleBufferSize(CsrUint16 *bs)
{
    *bs = Av2Instance->pcm_buffer_size;
}

/**************************************************************************************************
 * setSampleBufferSize
 **************************************************************************************************/
void setSampleBufferSize(CsrUint16 bs)
{
    Av2Instance->pcm_buffer_size = bs;
}

/**************************************************************************************************
 * getMaxFrameSize
 **************************************************************************************************/
CsrUint16 getMaxFrameSize(void)
{
    return Av2Instance->maxFrameSize;
}

/**************************************************************************************************
 * setMaxFrameSize
 **************************************************************************************************/
void setMaxFrameSize(CsrUint16 mfs)
{
    Av2Instance->maxFrameSize = mfs;
}

/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
void getPcmSettings(CsrUint8 *channels, CsrUint8 *bits, CsrUint16 *freq)
{
    extern Av2AppInst *Av2Instance;
    *channels = Av2Instance->pcm_channels;
    *freq = Av2Instance->pcm_freq;
    *bits = Av2Instance->pcm_bits;
}


/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
void setPcmSettings(CsrUint8 channels, CsrUint8 bits, CsrUint16 freq)
{
    Av2Instance->pcm_channels = channels;
    Av2Instance->pcm_freq = freq;
    Av2Instance->pcm_bits = bits;
}

/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
CsrBool getPcmLocked()
{
    return Av2Instance->pcm_locked;
}

/**************************************************************************************************
 * setPcmSettings
 **************************************************************************************************/
void setPcmLocked(CsrBool locked)
{
    Av2Instance->pcm_locked = locked;
}

