/**********************************************************************
* File Name   : Bluetooth_Av.c
* Description : This file contains functions and variables for
*                handling BT profiles
*	author 			:	yjkim
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
	instData->localSeid[INDEX_LOCAL_SS_SEID]      = SS_CODEC_TYPE;
    instData->localSeidInUse[INDEX_LOCAL_SS_SEID] = FALSE;

    /* Default PCM setup (44100 Hz, mono) */
    instData->pcm_freq = DEFAULT_FREQ;
    instData->pcm_channels = DEFAULT_CHANNELS;
    instData->pcm_bits = DEFAULT_BPS;
    instData->pcm_buffer_size = DEFAULT_BUFFER_SIZE;
	
#ifdef USE_ALSA
	instData->alsa_device_name=CsrStrDup(CSR_BT_AV_ALSA_DEFAULT_DEVICE);
#endif


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


    CsrBtAvActivateReqSend(inst->qid, CSR_BT_AV_AUDIO_SOURCE);
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

#ifdef USE_ALSA
    CsrPmemFree(inst->alsa_device_name);
#endif
    
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
           
		   CL_API_LOGPrintf("AV profile servCap");

            if(conf_len > 0)
            {
                /* Update the codec settings locally */
                if (filtersSetConfig(instData, conf, conf_len) != CSR_BT_AV_ACCEPT)
                {
                   CL_API_LOGPrintf("Error configuring codec (filter set config error)");
                }

				

                /* Open filter to activate new configuration */
                if(!filtersRun(instData, FILTER_OPEN))
                {
                    CL_API_LOGPrintf("Error selecting optimal configuration (filter open error)");
                }
				CL_API_LOGPrintf("AV profile CSR_BT_AV_GET_CAPABILITIES_CFM filtersRun");

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


void handleAv2Prim(Av2AppInst  *instData)
{
    CsrPrim    *thePrim;
    thePrim = (CsrPrim *) instData->recvMsgP;

	switch (*thePrim)
	{
        /* The first few primitives are so trivial that
         * we handle them directly in the message handler
 */

        case CSR_BT_AV_DEACTIVATE_CFM:
            {
				 CsrBtAvDeactivateCfm *prim;
                 prim = (CsrBtAvDeactivateCfm*)thePrim;


           		CL_API_LOGPrintf("AV profile deactivated...\n");
				break;
        	}

        case CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM:
            {
				CL_API_LOGPrintf("AV CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM\n");
               
                break;
            }

        /* From here, we divert all primitive handling into
         * special functions to keep the code clean and easy to
         * read
 */

        case CSR_BT_AV_ACTIVATE_CFM:
            {
				CsrBtAvActivateCfm *prim;
				prim = (CsrBtAvActivateCfm*)thePrim;
				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{

                    instData->state =SsAvStateAvDisconnected;

					instData->avIsActivated = TRUE;


					CL_API_LOGPrintf("AV profile activated...\n");
					if (instData->filter_count == 0)
					{
						filtersInstall(instData,SBC_CODEC_TYPE);
					}
				}
				else
				{
				/* If activation failed, make sure we're in non-activated state */
				// instData->max_num_counter++;
				//  instData->avIsActivated = FALSE;

				}
				break;
			}

        case CSR_BT_AV_STREAM_DATA_IND:
            {
                CsrBtAvStreamDataInd *prim;
                prim = (CsrBtAvStreamDataInd*)thePrim;
				if(instData->streamHandle==prim->shandle)
				{
					if(instData->state != SsAvStateAvConnectedStreaming)
					{
					    CL_API_LOGPrintf("CSR_BT_AV_STREAM_DATA_IND error...");
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
					CL_API_LOGPrintf("AV stream handle error...");

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
				CsrBtAvConnectInd *prim;
				prim = (CsrBtAvConnectInd*)thePrim;

				instData->connectionId=prim->connectionId;
				instData->state =SsAvStateAvConnected;
				instData->remoteAddr = prim->deviceAddr;

				CsrBtCmRoleDiscoveryReqSend(instData->qid,
				                     prim->deviceAddr);
				CL_API_LOGPrintf("AV profile CSR_BT_AV_CONNECT_IND");
				 /* Initialise filters */
				         /* Initialise filters */
				 if( instData->installedCodec !=SBC_CODEC_TYPE)
				 {
					  filtersKill(instData);
					  filtersInstall(instData,SBC_CODEC_TYPE);
				 }

				if(!filtersRun(instData, FILTER_INIT))
				{
				   CL_API_LOGPrintf("AV profile CSR_BT_AV_CONNECT_IND filter err");

				}

			}

                break;
      

        case CSR_BT_AV_CONNECT_CFM:
            {
                CsrBtAvConnectCfm *prim;
				CsrBool result=FALSE;
				 
                prim = (CsrBtAvConnectCfm*)thePrim;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			         result=TRUE;
					CL_API_LOGPrintf("AV profile CSR_BT_AV_CONNECT_CFM");
					instData->state =SsAvStateAvConnected;

				    instData->connectionId=prim->connectionId;
					instData->btConnId     = prim->btConnId;
        			instData->remoteAddr         = prim->deviceAddr;
#ifdef CSR_USE_DSPM
			       	CsrBtAvLpNegConfigReqSend(FALSE);
#endif
                   // CL_API_LOGPrintf("AV profile CsrBtCmRoleDiscoveryReqSend");
			        //CsrBtCmRoleDiscoveryReqSend(instData->qid,
			        //                     prim->deviceAddr);
			        if( instData->installedCodec !=SBC_CODEC_TYPE)
				    {
						 filtersKill(instData);
						 filtersInstall(instData,SBC_CODEC_TYPE);
					}
					filtersRun(instData, FILTER_OPEN);
        			filtersRun(instData, FILTER_CLOSE);
			        /* Initialise filters */
			        if(!filtersRun(instData, FILTER_INIT))
			        {
			         	CL_API_LOGPrintf("filter Run err");
			        }
			    }
			    else
			    {
			        CL_API_LOGPrintf("AV profile CSR_BT_AV_CONNECT_CFM err avResultSupplier[%x] avResultCode [%x]",prim->avResultSupplier, prim->avResultCode);
			        if (instData->avIsActivated == FALSE)
			        {
			        	CsrBtAvActivateReqSend(instData->qid, CSR_BT_AV_AUDIO_SOURCE);
			        }
			    
			    }

                break;
            }

        case CSR_BT_AV_DISCOVER_CFM:
			{
				CsrBtAvDiscoverCfm *prim;
				CsrBtAvSeidInfo seidInfo;
				CsrUint8 i;
//				CsrUint8 conIndex;
				
				prim = (CsrBtAvDiscoverCfm*)thePrim;

		

				CL_API_LOGPrintf("AV profile CSR_BT_AV_DISCOVER_CFM\n");

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
				prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS && prim->connectionId== instData->connectionId)
				{
					

					for(i=0; i<MAX_NUM_REMOTE_SEIDS; i++)
					{
					    instData->remoteSeid[i] = 0;
					}
		            instData->remoteSeidIndex = 0;
					instData->remoteSBCLen =0;

					/* Check availability of audio stream endpoint and select seid */
					for(i=0; (i<prim->seidInfoCount) && (i<MAX_NUM_REMOTE_SEIDS); i++)
					{
					    CsrMemCpy(&seidInfo,
					           &(prim->seidInfo[i]),
					           sizeof(CsrBtAvSeidInfo));

					    /* Looking for an audio sink not in use */
					    if((seidInfo.mediaType == CSR_BT_AV_AUDIO)
					       && (seidInfo.inUse == FALSE))
					    {
					        if ( seidInfo.sepType == CSR_BT_AV_SINK)
					        {
					            instData->remoteSeid[instData->remoteSeidIndex] = seidInfo.acpSeid;
					            instData->remoteSeidIndex++;
					        }
					    }
					}
					instData->remoteSeidMax=instData->remoteSeidIndex;

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



					}
				}
				else
				{



				}
				CsrPmemFree(prim->seidInfo);

				break;
			}

        case CSR_BT_AV_DISCOVER_IND:
			{

				CsrBtAvDiscoverInd *prim;
				CsrBtAvSeidInfo *seidInfoSend = CsrPmemAlloc(sizeof(CsrBtAvSeidInfo)*MAX_NUM_LOCAL_SEIDS);
	            CsrBtAvSeidInfo seidInfo[MAX_NUM_LOCAL_SEIDS];
				CsrUint8 iter;

				prim = (CsrBtAvDiscoverInd*)thePrim;

				CL_API_LOGPrintf("AV profile CSR_BT_AV_DISCOVER_IND");


				for(iter = 0; iter < MAX_NUM_LOCAL_SEIDS; ++iter)
				{
					seidInfo[iter].acpSeid = instData->localSeid[iter];
					seidInfo[iter].inUse = instData->localSeidInUse[iter];
					seidInfo[iter].mediaType = CSR_BT_AV_AUDIO;
				    seidInfo[iter].sepType = CSR_BT_AV_SOURCE;
				}
				CL_API_LOGPrintf("AV seidInfo seidInfo %d",seidInfo->inUse);

				CsrMemCpy(seidInfoSend,seidInfo,sizeof(CsrBtAvSeidInfo)*MAX_NUM_LOCAL_SEIDS);


				CsrBtAvDiscoverResAcpSend(prim->connectionId,
				             prim->tLabel,
				             MAX_NUM_LOCAL_SEIDS,   //seid cnt
				             seidInfoSend);
			}
                break;

        case CSR_BT_AV_GET_CAPABILITIES_CFM:
            {
                CsrBtAvGetCapabilitiesCfm *prim=(CsrBtAvGetCapabilitiesCfm*)thePrim;
   
	//		    CsrUint8 *servCap;
	//		    CsrUint8 servCapLen = 0;
			    CsrBool found = FALSE;
	//		    CsrUint16 index = 0;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			       
				   CL_API_LOGPrintf("AV profile CSR_BT_AV_GET_CAPABILITIES_CFM");

				   if((prim->servCapData[4]>>4==CSR_BT_AV_AUDIO) &&(prim->servCapData[5]==CSR_BT_AV_NON_A2DP_CODEC)&&(prim->servCapData[6]==0x75)) // 0x75 is samsung codec
				   {
						CL_API_LOGPrintf("found ss codec\n" );   
			
					     if( instData->installedCodec !=SS_CODEC_TYPE)
						 {
							 CL_API_LOGPrintf("set ss codec\n" ); 				 
							 filtersKill(instData);
							 filtersInstall(instData,SS_CODEC_TYPE);
							 if( !filtersRun( instData, FILTER_INIT) )
							 {
								CL_API_LOGPrintf("Error configuring FILTER_INIT error\n" );                        
							 }
					
						}
						instData->localSeidIndex=INDEX_LOCAL_SS_SEID;
						 if(btAVsetCodeConfig(instData,prim->servCapData,prim->servCapLen))
						 {
							CL_API_LOGPrintf("succes set ss codec\n" ); 				 
							found=TRUE;
						 }
					   
				   }
				   else if((prim->servCapData[4]>>4==CSR_BT_AV_AUDIO) &&(prim->servCapData[5]==CSR_BT_AV_SBC))
				   {
					 CL_API_LOGPrintf("found sbc codec\n" );
					if(instData->remoteSBCLen ==0)
					{

					  instData->remoteSeidSBCIndex=instData->remoteSeidIndex ;
					  CsrMemCpy(instData->remoteSBCConf,prim->servCapData,prim->servCapLen);
					  instData->remoteSBCLen=prim->servCapLen;
					  CL_API_LOGPrintf("remoteSeidSBCIndex %d\n" , instData->remoteSeidSBCIndex);     

					}

					if((instData->remoteSeidIndex==instData->remoteSeidMax-1) ||
						(instData->remoteSeidIndex==MAX_NUM_REMOTE_SEIDS-1))
					{
						if( instData->installedCodec !=SBC_CODEC_TYPE)
						{
							CL_API_LOGPrintf("set sbc codec\n" ); 				 
							filtersKill(instData);
							filtersInstall(instData,SBC_CODEC_TYPE);
							if( !filtersRun( instData, FILTER_INIT) )
							{
							        CL_API_LOGPrintf("Error configuring FILTER_INIT error\n" );                       
							 }
						    
						}
						instData->remoteSeidIndex=instData->remoteSeidSBCIndex;
						instData->localSeidIndex=INDEX_LOCAL_SS_SEID;

						if(btAVsetCodeConfig(instData,instData->remoteSBCConf,instData->remoteSBCLen))
						{
						     CL_API_LOGPrintf("succes sbc codec\n" );				 
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
			                CL_API_LOGPrintf("Requesting additional remote capabilities...");
			                CsrBtAvGetCapabilitiesReqSend(instData->connectionId,
			                                         instData->remoteSeid[instData->remoteSeidIndex],
			                                         ASSIGN_TLABEL);

			                CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			            }
			            else
			            {
			                CL_API_LOGPrintf("Get capabilities response does not contain supported media/codec type(s)");
#ifdef DEBUG
			                if(prim->servCapData)
			                {
			                    CsrUint8 i;
			                    CL_API_LOGPrintf("Raw capabilities (len=%i): ", prim->servCapLen);
			                    for(i=0; i<prim->servCapLen; i++)
			                    {
			                        CL_API_LOGPrintf("0x%02X ", prim->servCapData[i]);
			                    }
			                    CL_API_LOGPrintf("\n");
			                }
#endif
			            }
			        }
			    }
			    else
			    {
					CL_API_LOGPrintf("CSR_BT_AV_GET_CAPABILITIES_CFM err avResultSupplier[%x] avResultCode [%x]",prim->avResultSupplier, prim->avResultCode);
			    }

			    /* Free primitive members */
			    if(prim)
			    {
			        CsrPmemFree(prim->servCapData);
			    }


                break;
            }

        case CSR_BT_AV_STATUS_IND:
            {
               // CsrBtAvStatusInd *prim= (CsrBtAvStatusInd*)thePrim;
				CL_API_LOGPrintf("CSR_BT_AV_STATUS_IND");
                break;
            }

        case CSR_BT_AV_GET_ALL_CAPABILITIES_IND:
        case CSR_BT_AV_GET_CAPABILITIES_IND:
            {
                CsrBtAvGetCapabilitiesInd *prim;
				//CsrUint8 conIndex;
			    CsrUint8 *caps;
			    CsrUint8 caps_len;
				CsrBool found=FALSE;
				
                prim = (CsrBtAvGetCapabilitiesInd*)thePrim;
				CL_API_LOGPrintf("CSR_BT_AV_GET_CAPABILITIES_IND | CSR_BT_AV_GET_ALL_CAPABILITIES_IND receved");
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
								CL_API_LOGPrintf("CSR_BT_AV_GET_CAPABILITIES_IND Error initialising filters!\n");            
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

			    caps = filtersGetCaps(instData, &caps_len);
			  

			    if ((caps) &&
			        (found)&&(prim->connectionId ==instData->connectionId))
			    {
			        CL_API_LOGPrintf("Remote side requested capabilities, sending response");
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
			        CL_API_LOGPrintf("GetCapabilities command rejected - note that capabilities is not supported ");
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
                CsrBtAvSetConfigurationCfm *prim;
                prim = (CsrBtAvSetConfigurationCfm*)thePrim;


				CL_API_LOGPrintf("CSR_BT_AV_SET_CONFIGURATION_CFM");

		
			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			        instData->streamHandle = prim->shandle;
			

			        /* Open the filters so they are ready to begin processing */
			        if(filtersRun(instData, FILTER_OPEN))
			        {
			            /* Ready to open stream */
			            instData->localSeidInUse[instData->localSeidIndex] = TRUE;
			            CsrBtAvOpenReqSend(instData->streamHandle, ASSIGN_TLABEL);
			        }
			        else
			        {
			            CL_API_LOGPrintf("Unable to configure stream end-point (filter open error)");
			        }
			    }
			    else
			    {
			        CL_API_LOGPrintf("Set configuration rejected, error code 0x%x (0x%04x):",
			               prim->avResultCode,
			               prim->avResultSupplier);


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

			        CL_API_LOGPrintf("Configuration request received");
					
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
										CL_API_LOGPrintf("Error initialising filters!\n");            
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
								
			                        return;
			                    }
			                    else
			                    {
			                       CL_API_LOGPrintf("filtersRun open err");
			                        /* If we fail to initialize filters, some parameters must be wrong */
			                        result = CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT;
			                    }
			                }
							else
						    {
						      CL_API_LOGPrintf("not accept");
							}
			            }
			            else
			            {
			                CL_API_LOGPrintf("localSeidInUse err");
			                result = CSR_BT_RESULT_CODE_A2DP_SEP_IN_USE;
			            }
			        }
			        else
			        {
			            CL_API_LOGPrintf("localSeid err");
			            result = CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID;
			        }
				}
				else
				{
				     CL_API_LOGPrintf("CSR_BT_AV_MAX_NUM_CONNECTIONS err");
					result = CSR_BT_AV_MAX_NUM_CONNECTIONS;
				}


			    /* Reaching here means configuration can not be approved, send reject response */
			    CsrBtAvSetConfigResRejSend(instData->streamHandle,
			                          prim->tLabel,
			                          result,
			                          *servCap);

			    CL_API_LOGPrintf("Configuration has been rejected (Error code: 0x%x)", result);

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
			       CsrUint8 index;
				   	CsrBool ss_filter_support=FALSE;
//	               CsrUint8 conIndex;
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
						    CL_API_LOGPrintf("CSR_BT_AV_OPEN_IND codec %d\n",codecIdx );                        
							  
							filtersKill(instData);
							filtersInstall(instData,codecIdx);
							if( !filtersRun( instData, FILTER_INIT) )
							{
							        CL_API_LOGPrintf("Error CSR_BT_AV_OPEN_IND FILTER_INIT error\n" );                        
							}
							if(!filtersRun( instData, FILTER_OPEN ) )
							{
							        CL_API_LOGPrintf("Error CSR_BT_AV_OPEN_IND FILTER_OPEN error\n" );               
							}
							instData->localSeidIndex=index;
							break;
				         }
		              }
					 }
					
			 	    if(instData->installedCodec==SS_CODEC_TYPE)
						ss_filter_support=TRUE;
		
				
					
			        CsrBtAvGetStreamChannelInfoReqSend(prim->shandle);
			        if(instData->streamHandle == prim->shandle)
			        {
			            CL_API_LOGPrintf("Stream opened");
			            instData->state = SsAvStateAvConnectedOpen;
			        }
					else
					{
					  CL_API_LOGPrintf("Stream opened invalid handle [%x] [%x]",instData->streamHandle,prim->shandle);
					}
			    }
			    else
			    {
			         CL_API_LOGPrintf("Open stream rejected, error code 0x%04x (0x%04x)",
			               prim->avResultCode,
			               prim->avResultSupplier);
			        
			    }


                break;
            }

        case CSR_BT_AV_OPEN_IND:
            {
                CsrBtAvOpenInd *prim;
				CsrBool ss_filter_support=FALSE;
					
                prim = (CsrBtAvOpenInd*)thePrim;
			

			    if( instData->streamHandle == prim->shandle)
			    {

					CL_API_LOGPrintf("CSR_BT_AV_OPEN_IND");
			        
			        CsrBtAvGetStreamChannelInfoReqSend(prim->shandle);
			        CsrBtAvOpenResAcpSend(prim->shandle, prim->tLabel);
					instData->state = SsAvStateAvConnectedOpen;

			      //  instData->localSeidInUse = TRUE;
			        instData->localSeidInUse[instData->localSeidIndex] = TRUE;
				  if(instData->installedCodec==SS_CODEC_TYPE)
					ss_filter_support=TRUE;
		
    }
    else
    {
			         CL_API_LOGPrintf("CSR_BT_AV_OPEN_IND err [%d] [%d]\n", instData->streamHandle,prim->shandle);   
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
					CL_API_LOGPrintf("CSR_BT_AV_START_CFM");
					instData->state = SsAvStateAvConnectedStreaming;

					if(!filtersRun(instData, FILTER_START))
					{
						CL_API_LOGPrintf("Could not start stream (filter start error)");
					}
				}
				else
				{
					CL_API_LOGPrintf("Start stream rejected, error code 0x%04x (0x%04x): ",
					   prim->avResultCode,
					   prim->avResultSupplier);
				}

                break;
            }

        case CSR_BT_AV_START_IND:
            {
                CsrBtAvStartInd *prim=(CsrBtAvStartInd*)thePrim;


			    CsrBtAvResult result;
			    CsrUint8 i;
			    CsrUint8 shandle = 0;
			//    CsrUint8 conIndex;

			  
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
			                 CL_API_LOGPrintf("CSR_BT_AV_START_IND");
			                if(!filtersRun(instData, FILTER_OPEN))
			                {
			                    
								CL_API_LOGPrintf("filtersRun open err");

			                    break;
			                }
			            }
			            else
			            {
			                 CL_API_LOGPrintf("AV profile CSR_BT_AV_START_IND status err %d",instData->state);
			                result = CSR_BT_RESULT_CODE_A2DP_BAD_STATE;
			                break;
			            }
			        }
			        else
			        {
			            CL_API_LOGPrintf("AV profile CSR_BT_AV_START_IND shandle err");
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
				 

					CL_API_LOGPrintf("CSR_BT_AV_START_IND run");

			        /* Now, start the filters */
			        if(!filtersRun(instData, FILTER_START))
			        {
			            CL_API_LOGPrintf("Unable to start stream (filter start error)");
			           

			        }

			       CL_API_LOGPrintf("Stream started");

			        
			    }
			    else
			    {
			        CL_API_LOGPrintf("AV profile CSR_BT_AV_START_IND reject send");
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
					CL_API_LOGPrintf("CSR_BT_AV_CLOSE_CFM receved");

			        instData->localSeidInUse[instData->localSeidIndex] = FALSE;
			    }
			    else
			    {
			        CL_API_LOGPrintf("Close stream rejected, error code 0x%04x (0x%04x)",
			               prim->avResultCode,
			               prim->avResultSupplier);
			    }

                break;
            }

        case CSR_BT_AV_CLOSE_IND:
            {
                CsrBtAvCloseInd *prim=(CsrBtAvCloseInd*)thePrim;

  //  CsrUint8 conIndex;

    //conIndex = getIndexFromShandle(instData, prim->shandle);
    if(prim->shandle == instData->streamHandle)
				{
					instData->state = SsAvStateAvConnected;
					instData->localSeidInUse[instData->localSeidIndex] = FALSE;
					CsrBtAvCloseResAcpSend( prim->shandle, prim->tLabel );
					CL_API_LOGPrintf("CSR_BT_AV_CLOSE_IND receved");

					if(!filtersRun(instData, FILTER_CLOSE))
					{
						CL_API_LOGPrintf("Error closing stream (filter stop error)");

					}
					CL_API_LOGPrintf("Stream closed");

       
    }
    else
    {
				    CL_API_LOGPrintf("AV profile CSR_BT_AV_CLOSE_IND reject send");
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
	                CL_API_LOGPrintf("CSR_BT_AV_SUSPEND_CFM receved");
			        if(instData->state == SsAvStateAvConnectedStreaming)
			        {
			            if(!filtersRun(instData, FILTER_STOP))
			            {
			                CL_API_LOGPrintf("Error suspending stream (filter stop error)");
			            }
						instData->state = SsAvStateAvConnectedOpen;


			        }
			    }
			    else
			    {
					CL_API_LOGPrintf("CSR_BT_AV_SUSPEND_CFM err %x",prim->avResultCode);  
			       
			    }


                break;
            }

        case CSR_BT_AV_SUSPEND_IND:
            {
                CsrBtAvSuspendInd *prim= (CsrBtAvSuspendInd*)thePrim;
	
			    CsrUint8 i;
			    CsrUint8 shandle;
//			    CsrUint8 conIndex;
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

									CL_API_LOGPrintf("Stream suspended filter err");
			                    }
								
								CL_API_LOGPrintf("Stream suspended");
								CsrBtAvSuspendResAcpSend((CsrUint8)prim->tLabel,
								prim->listLength,
								prim->list);
								freePtr = FALSE;
								instData->state = SsAvStateAvConnectedOpen;			                   
			                }
			            }
			            else
			            {
			                CsrBtAvSuspendResRejSend((CsrUint8)shandle,
			                                    (CsrUint8)prim->tLabel,
			                                    CSR_BT_RESULT_CODE_A2DP_BAD_STATE,
			                                    prim->listLength,
			                                    prim->list);
							CL_API_LOGPrintf("Stream suspended not conntcedStreming");
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
						CL_API_LOGPrintf("Stream suspended invalid handle");
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
                CsrBtAvReconfigureCfm *prim;
                prim = (CsrBtAvReconfigureCfm *) thePrim;

			    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
			        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
			    {
			        CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_CFM receved");

			        /* New configuration was accepted, so "open" filter again */
			        if(!filtersRun(instData, FILTER_OPEN))
			        {
			            CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_IND filter open err");
			        }
			    }
			    else
			    {
				   CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_IND err avResultCode %x",prim->avResultCode);
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
			        CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_IND receved");
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

						    CL_API_LOGPrintf("reconfigind codec %d\n",codecIdx );                       
							  
							filtersKill(instData);
							filtersInstall(instData,codecIdx);
							if( !filtersRun( instData, FILTER_INIT) )
							{
							        CL_API_LOGPrintf("Error configuring FILTER_INIT error\n" );                        
							}
							if(!filtersRun( instData, FILTER_OPEN ) )
							{
							        CL_API_LOGPrintf("Error configuring FILTER_OPEN error\n" );               
							}
							instData->localSeidIndex=index;
							break;
				         }
		              }
					 }
			        if(instData->localSeidInUse[instData->localSeidIndex] == TRUE)
			        {
			          CL_API_LOGPrintf("Reconfiguration indication received...");

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
			                CL_API_LOGPrintf("Reconfiguration parameters OK, sending accept response");


                CsrBtAvReconfigResAcpSend(prim->shandle,
                                     prim->tLabel);

                    CsrBtAvDelayReportReqSend(50,instData->connectionId,ASSIGN_TLABEL);

			            }
			            else
			            {
			                 CL_API_LOGPrintf("AV profile CSR_BT_AV_RECONFIGURE_IND filtersRun err");
			                /* If we fail to initialize filters, some parameters must be wrong */
			                result = CSR_BT_RESULT_CODE_A2DP_BAD_MEDIA_TRANSPORT_FORMAT;
			            }
			        }
			    }
			    else
			    {
			        CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_IND  streamHandle err ");
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
			         
                    CL_API_LOGPrintf("CSR_BT_AV_RECONFIGURE_IND   err  %d",result);
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
				//    CsrUint8 i;
				CsrUint8 *servCap;
				CL_API_LOGPrintf("CSR_BT_AV_GET_CONFIGURATION_CFM");

 //   CsrCharString temp[50];
//    CsrCharString displayString[500];

    if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
        prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
    {

       
        index = 0;
        servCap = prim->servCapData;

					while(servCap != NULL)
					{
						servCap = CsrBtAvGetServiceCap(CSR_BT_AV_SC_NEXT,
						                      prim->servCapData,
						                      prim->servCapLen,
						                      &index);
					}
					CsrPmemFree(prim->servCapData);
				}
				else
				{
					CL_API_LOGPrintf("Error CSR_BT_AV_GET_CONFIGURATION_CFM %x",prim->avResultCode);
    			}
                break;
            }

        case CSR_BT_AV_GET_CONFIGURATION_IND:
            {
				CsrBtAvGetConfigurationInd *prim;
				prim = (CsrBtAvGetConfigurationInd*)thePrim;
				CL_API_LOGPrintf("CSR_BT_AV_GET_CONFIGURATION_IND");

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
					CL_API_LOGPrintf("send CsrBtAvGetConfigResRejSend");
					CsrBtAvGetConfigResRejSend(prim->shandle,
					                      prim->tLabel,
					                      CSR_BT_RESULT_CODE_A2DP_BAD_ACP_SEID);
				}


                break;
            }

        case CSR_BT_AV_ABORT_CFM:
            {
              //  CsrBtAvAbortCfm *prim= (CsrBtAvAbortCfm*)thePrim;
				CL_API_LOGPrintf("CSR_BT_AV_ABORT_CFM");

   

				/* Close filters */
				if(!filtersRun(instData, FILTER_CLOSE))
				{
					CL_API_LOGPrintf("Error aborting stream (filter close error)");

					
				}
				instData->state = SsAvStateAvConnected;
				instData->localSeidInUse[instData->localSeidIndex] = FALSE;


                break;
            }

        case CSR_BT_AV_ABORT_IND:
            {
                CsrBtAvAbortInd *prim=(CsrBtAvAbortInd*)thePrim;
		
				instData->state = SsAvStateAvConnected;
			    instData->localSeidInUse[instData->localSeidIndex] = FALSE;

				CL_API_LOGPrintf("CSR_BT_AV_ABORT_IND");


                break;
            }

        case CSR_BT_AV_DISCONNECT_IND:
            {
                CsrBtAvDisconnectInd *prim=(CsrBtAvDisconnectInd*)thePrim;


				instData->localSeidInUse[instData->localSeidIndex] = FALSE;


				CL_API_LOGPrintf("CSR_BT_AV_DISCONNECT_IND %x %x",prim->reasonCode, prim->reasonSupplier);
				if(prim->connectionId==instData->connectionId)
				{

					instData->state = SsAvStateAvDisconnected;
					/* Close filters */
					if (!filtersRun(instData, FILTER_CLOSE))
					{
					    CL_API_LOGPrintf("Error disconnecting (filter close error)");
					   
					}

					/* Deinitialise filters */
					if (!filtersRun(instData, FILTER_DEINIT))
					{
					    CL_API_LOGPrintf("Error initialising filters!");
					}
				}
				else
				{
					CL_API_LOGPrintf("err CSR_BT_AV_DISCONNECT_IND\n");
				}
                break;
            }

        case CSR_BT_AV_STREAM_MTU_SIZE_IND:
            {
                CsrBtAvStreamMtuSizeInd *prim;
                prim = (CsrBtAvStreamMtuSizeInd*)thePrim;

			    if( (instData->maxFrameSize == 0) || (prim->remoteMtuSize < instData->maxFrameSize) )
			    {
			        instData->maxFrameSize = prim->remoteMtuSize;
			    }
				CL_API_LOGPrintf("CSR_BT_AV_STREAM_MTU_SIZE_IND\n");

    
                break;
            }
        case CSR_BT_AV_SECURITY_CONTROL_IND:
            {
                CsrBtAvSecurityControlInd *prim;
                prim = (CsrBtAvSecurityControlInd*)thePrim;

			    /* Testing af parameters is not implemented. An accept response will be sent in any case */
			    CL_API_LOGPrintf("Security Control request received. Sending accept response\n");
			    CsrBtAvSecurityControlResAcpSend(prim->shandle, prim->tLabel, prim->contProtMethodLen, prim->contProtMethodData);


                break;
            }
        case CSR_BT_AV_SECURITY_CONTROL_CFM:
            {
                CsrBtAvSecurityControlCfm *prim;
                prim = (CsrBtAvSecurityControlCfm*)thePrim;

				if (prim->avResultSupplier == CSR_BT_SUPPLIER_AV &&
					prim->avResultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
				{
					CL_API_LOGPrintf("Security Control accepted by remote device");
				}
				else
				{
					CL_API_LOGPrintf("Security Control rejected by remote device");
				}
				CsrPmemFree(prim->contProtMethodData);
		

                break;
            }
        case CSR_BT_AV_DELAY_REPORT_IND:
            {
           //     CsrBtAvDelayReportInd *prim=(CsrBtAvDelayReportInd *)thePrim;
           
				CL_API_LOGPrintf("CSR_BT_AV_DELAY_REPORT_IND:");
               
                break;
            }
        case CSR_BT_AV_GET_CHANNEL_INFO_CFM:
            {
#ifdef CSR_USE_DSPM
                CsrBtAvGetChannelInfoCfm *prim = (CsrBtAvGetChannelInfoCfm *)thePrim;

                if (prim->resultCode == CSR_BT_RESULT_CODE_AV_SUCCESS)
                {
                   CL_API_LOGPrintf("CSR_BT_AV_GET_CHANNEL_INFO_CFM:");
                }
#endif
                break;
            }
        default:
            {
                CL_API_LOGPrintf("Unhandled AV primitive: 0x%04x", *thePrim);
				CsrBtCmFreeUpstreamMessageContents(CSR_BT_AV_PRIM, instData->recvMsgP);
            }
    }
	CsrPmemFree(instData->recvMsgP);
	instData->recvMsgP=NULL;

}


/**************************************************************************************************
 * setMediaPayloadType
 **************************************************************************************************/
void setMediaPayloadType(CsrUint8 mpt)
{
    extern Av2AppInst *Av2Instance;
    Av2Instance->mediaPayloadType = mpt;
}

/**************************************************************************************************
 * setTimestamp
 **************************************************************************************************/
void setTimestamp(CsrUint32 ts)
{
    extern Av2AppInst *Av2Instance;
    Av2Instance->time_stamp = ts;
}

/**************************************************************************************************
 * getSampleBufferSize
 **************************************************************************************************/
void getSampleBufferSize(CsrUint16 *bs)
{
    extern Av2AppInst *Av2Instance;
    *bs = Av2Instance->pcm_buffer_size;
}

/**************************************************************************************************
 * setSampleBufferSize
 **************************************************************************************************/
void setSampleBufferSize(CsrUint16 bs)
{
    extern Av2AppInst *Av2Instance;
    Av2Instance->pcm_buffer_size = bs;
}

/**************************************************************************************************
 * getMaxFrameSize
 **************************************************************************************************/
CsrUint16 getMaxFrameSize(void)
{
    extern Av2AppInst *Av2Instance;
    return Av2Instance->maxFrameSize;
}

/**************************************************************************************************
 * setMaxFrameSize
 **************************************************************************************************/
void setMaxFrameSize(CsrUint16 mfs)
{
    extern Av2AppInst *Av2Instance;
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
    extern Av2AppInst *Av2Instance;
    Av2Instance->pcm_channels = channels;
    Av2Instance->pcm_freq = freq;
    Av2Instance->pcm_bits = bits;
}

/**************************************************************************************************
 * getPcmSettings
 **************************************************************************************************/
CsrBool getPcmLocked()
{
    extern Av2AppInst *Av2Instance;
    return Av2Instance->pcm_locked;
}

/**************************************************************************************************
 * setPcmSettings
 **************************************************************************************************/
void setPcmLocked(CsrBool locked)
{
    extern Av2AppInst *Av2Instance;
    Av2Instance->pcm_locked = locked;
}

